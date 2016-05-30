#include "DemoStableHeaders.h"
#if DBS_LM_DEFERRED_RENDERING

#include <OgreHardwareBufferManager.h>
#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreLight.h>
#include <OgreTechnique.h>
#include <OgreSceneManager.h>

#include "System\LightShapeGeom.h"
#include "System\DeferredMaterialGenerator.h"
#include "DLight.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	#define ENABLE_BIT(mask, flag) (mask) |= (flag)
	#define DISABLE_BIT(mask, flag) (mask) &= ~(flag)
	//------------------------------------------------------------
	DLight::DLight( MaterialGenerator::shared_ptr mg, Ogre::Light* parentLight )
		: mParentLight(parentLight)
		, bIgnoreWorld(false)
		, mGenerator(mg)
		, mPermutation(0)
	{
		// Set up geometry
		// Allocate render operation
		mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		mRenderOp.indexData = 0;
		mRenderOp.vertexData = 0;
		mRenderOp.useIndexes = true;

		updateFromParent();
	}
	//-----------------------------------------------------------------------
	DLight::~DLight()
	{
		// need to release IndexData and vertexData created for renderable
		delete mRenderOp.indexData;
		delete mRenderOp.vertexData;
	}
	//-----------------------------------------------------------------------
	void DLight::setAttenuation( float c, float b, float a )
	{
		// Set Attenuation parameter to shader
		//setCustomParameter(3, Vector4(c, b, a, 0));
		float outerRadius = mParentLight->getAttenuationRange();
		/// There is attenuation? Set material accordingly
		if( c != 1.0f || b != 0.0f || a != 0.0f )
		{
			ENABLE_BIT( mPermutation, DeferredLightMaterialGenerator::LIT_HAS_ATTENUATION );
			if( mParentLight->getType() == Ogre::Light::LT_POINT )
			{
				//// Calculate radius from Attenuation
				int threshold_level = 10;// difference of 10-15 levels deemed unnoticeable
				float threshold = 1.0f/((float)threshold_level/256.0f); 

				//// Use quadratic formula to determine outer radius
				c = c - threshold;
				float d = sqrt( b*b - 4*a*c );
				outerRadius = (-2*c)/(b+d);
				outerRadius *= 1.2f;
			}
		}
		else
		{
			DISABLE_BIT(mPermutation,DeferredLightMaterialGenerator::LIT_HAS_ATTENUATION);
		}
	    
		rebuildGeometry(outerRadius);
	}
	//-----------------------------------------------------------------------
	void DLight::setSpecularColour( const Ogre::ColourValue &col )
	{
		//setCustomParameter(2, Vector4(col.r, col.g, col.b, col.a));

		/// There is a specular component? Set material accordingly		
		if( col.r != 0.0f || col.g != 0.0f || col.b != 0.0f )
			ENABLE_BIT(mPermutation,DeferredLightMaterialGenerator::LIT_HAS_SPECULAR);
		else
			DISABLE_BIT(mPermutation,DeferredLightMaterialGenerator::LIT_HAS_SPECULAR);
	}
	//-----------------------------------------------------------------------
	void DLight::rebuildGeometry(float radius)
	{
		//Disable all 3 bits
		DISABLE_BIT(mPermutation, DeferredLightMaterialGenerator::LIT_TYPE_POINT);
		DISABLE_BIT(mPermutation, DeferredLightMaterialGenerator::LIT_TYPE_SPOT);
		DISABLE_BIT(mPermutation, DeferredLightMaterialGenerator::LIT_TYPE_DIRECT);

		switch (mParentLight->getType())
		{
		case Ogre::Light::LT_DIRECTIONAL:
			createRectangle2D();
			ENABLE_BIT(mPermutation,DeferredLightMaterialGenerator::LIT_TYPE_DIRECT);
			break;

		case Ogre::Light::LT_POINT:
			/// XXX some more intelligent expression for rings and segments
			createSphere(radius, 10, 10);
			ENABLE_BIT(mPermutation,DeferredLightMaterialGenerator::LIT_TYPE_POINT);
			break;

		case Ogre::Light::LT_SPOTLIGHT:
			Ogre::Real height = mParentLight->getAttenuationRange();
			Ogre::Radian coneRadiusAngle = mParentLight->getSpotlightOuterAngle() / 2;
			Ogre::Real radius = Ogre::Math::Tan(coneRadiusAngle) * height;
			createCone(radius, height, 20);
			ENABLE_BIT(mPermutation,DeferredLightMaterialGenerator::LIT_TYPE_SPOT);
			break;
		}	
	}
	//-----------------------------------------------------------------------
	void DLight::createRectangle2D()
	{
		/// XXX this RenderOp should really be re-used between DLight objects,
		/// not generated every time
		delete mRenderOp.vertexData; 
		delete mRenderOp.indexData; 

		mRenderOp.vertexData = new Ogre::VertexData();
		mRenderOp.indexData = 0;

		LightShapeGeom::createQuad(mRenderOp.vertexData);

		mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP; 
		mRenderOp.useIndexes = false; 

		// Set bounding
		setBoundingBox(Ogre::AxisAlignedBox(-10000,-10000,-10000,10000,10000,10000));
		mRadius = 15000;
		bIgnoreWorld = true;
	}
	//-----------------------------------------------------------------------
	void DLight::createSphere(float radius, int nRings, int nSegments)
	{
		delete mRenderOp.vertexData; 
		delete mRenderOp.indexData;
		mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		mRenderOp.indexData = new Ogre::IndexData();
		mRenderOp.vertexData = new Ogre::VertexData();
		mRenderOp.useIndexes = true;

		LightShapeGeom::createSphere( mRenderOp.vertexData, mRenderOp.indexData
			, radius
			, nRings, nSegments
			, false // no normals
			, false // no texture coordinates
			);

		// Set bounding box and sphere
		setBoundingBox( Ogre::AxisAlignedBox( Ogre::Vector3(-radius, -radius, -radius), Ogre::Vector3(radius, radius, radius) ) );
		mRadius = radius;
		bIgnoreWorld = false;
	}
	//-----------------------------------------------------------------------
	void DLight::createCone(float radius, float height, int segments)
	{
		delete mRenderOp.vertexData;
		delete mRenderOp.indexData;
		mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		mRenderOp.indexData = new Ogre::IndexData();
		mRenderOp.vertexData = new Ogre::VertexData();
		mRenderOp.useIndexes = true;

		LightShapeGeom::createCone( mRenderOp.vertexData, mRenderOp.indexData, radius, height, segments );

		// Set bounding box and sphere
		setBoundingBox( Ogre::AxisAlignedBox( Ogre::Vector3(-radius, 0, -radius), Ogre::Vector3(radius, height, radius) ) );

		mRadius = radius;
		bIgnoreWorld = false;
	}
	//-----------------------------------------------------------------------
	Ogre::Real DLight::getBoundingRadius(void) const
	{
		return mRadius;
	}
	//-----------------------------------------------------------------------
	Ogre::Real DLight::getSquaredViewDepth(const Ogre::Camera* cam) const
	{
		if(bIgnoreWorld)
		{
			return 0.0f;
		}
		else
		{
			Ogre::Vector3 dist = cam->getDerivedPosition() - getParentSceneNode()->_getDerivedPosition();
			return dist.squaredLength();
		}
	}
	//-----------------------------------------------------------------------
	const Ogre::MaterialPtr& DLight::getMaterial(void)
	{
		if(bPermutationDirty)
		{
			mMaterial = mGenerator->getMaterial(mPermutation);
			mMaterial->load();
			bPermutationDirty = false;
		}

		return mMaterial;
	}
	//-----------------------------------------------------------------------
	void DLight::getWorldTransforms(Ogre::Matrix4* xform) const
	{
		if (mParentLight->getType() == Ogre::Light::LT_SPOTLIGHT)
		{
			Ogre::Quaternion quat = Ogre::Vector3::UNIT_Y.getRotationTo(mParentLight->getDerivedDirection());
			xform->makeTransform(mParentLight->getDerivedPosition(), Ogre::Vector3::UNIT_SCALE, quat );
		}
		else
		{
			xform->makeTransform(mParentLight->getDerivedPosition(), Ogre::Vector3::UNIT_SCALE, Ogre::Quaternion::IDENTITY );
		}
	}
	//-----------------------------------------------------------------------
	void DLight::updateFromParent()
	{
		//TODO : Don't do this unless something changed
		setAttenuation( 
			mParentLight->getAttenuationConstant(),
			mParentLight->getAttenuationLinear(),
			mParentLight->getAttenuationQuadric() );

		setSpecularColour(mParentLight->getSpecularColour());

		if (getCastChadows())
		{
			ENABLE_BIT( mPermutation, DeferredLightMaterialGenerator::LIT_HAS_SHADOW );
		}
		else
		{
			DISABLE_BIT( mPermutation, DeferredLightMaterialGenerator::LIT_HAS_SHADOW );
		}

		mMaterial = mGenerator->getMaterialClone(mPermutation);
	}
	//-----------------------------------------------------------------------
	bool DLight::isCameraInsideLight( Ogre::Camera* camera )
	{
		switch( mParentLight->getType() )
		{
		case Ogre::Light::LT_DIRECTIONAL:
			return false;
		case Ogre::Light::LT_POINT:
			{
				Ogre::Real distanceFromLight = camera->getDerivedPosition().distance(mParentLight->getDerivedPosition());
				//Small epsilon fix to account for near clip range and the fact that we aren't a true sphere.
				return distanceFromLight <= mRadius + camera->getNearClipDistance() + 0.1;
			}
		case Ogre::Light::LT_SPOTLIGHT:
			{
				Ogre::Vector3 lightToCamDir = camera->getDerivedPosition() - mParentLight->getDerivedPosition();
				Ogre::Real distanceFromLight = lightToCamDir.normalise();
				Ogre::Real cosAngle = lightToCamDir.dotProduct(mParentLight->getDerivedDirection());
				Ogre::Radian angle = Ogre::Math::ACos(cosAngle);
				return (distanceFromLight <= mParentLight->getAttenuationRange()) && (angle < mParentLight->getSpotlightOuterAngle());
			}
		default:
			//Please the compiler
			return false;
		}
	}
	//-----------------------------------------------------------------------
	bool DLight::getCastChadows() const
	{
		return mParentLight->_getManager()->isShadowTechniqueInUse() && mParentLight->getCastShadows() &&
			( mParentLight->getType() == Ogre::Light::LT_DIRECTIONAL || mParentLight->getType() == Ogre::Light::LT_SPOTLIGHT );
	}
	//-----------------------------------------------------------------------
	void DLight::updateFromCamera(Ogre::Camera* camera)
	{
		//Set shader params
		const Ogre::MaterialPtr& mat = getMaterial();

		Ogre::Technique* tech = mat->getBestTechnique();
		Ogre::Vector3 farCorner = camera->getViewMatrix(true) * camera->getWorldSpaceCorners()[4];

		for (unsigned short i = 0, ei = tech->getNumPasses(); i < ei; ++i) 
		{
			Ogre::Pass* pass = tech->getPass(i);

			Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
			// set the camera's far-top-right corner
			params->setNamedConstant("farCorner", farCorner);
		    
			params = pass->getFragmentProgramParameters();
			params->setNamedConstant("farCorner", farCorner);

			//If inside light geometry, render back faces with CMPF_GREATER, otherwise normally
			if (mParentLight->getType() == Ogre::Light::LT_DIRECTIONAL)
			{
				pass->setCullingMode(Ogre::CULL_CLOCKWISE);
				pass->setDepthCheckEnabled(false);
			}
			else
			{
				pass->setDepthCheckEnabled(true);
				if (isCameraInsideLight(camera))
				{
					pass->setCullingMode(Ogre::CULL_ANTICLOCKWISE);
					pass->setDepthFunction(Ogre::CMPF_GREATER_EQUAL);
				}
				else
				{
					pass->setCullingMode(Ogre::CULL_CLOCKWISE);
					pass->setDepthFunction(Ogre::CMPF_LESS_EQUAL);
				}
			}

			if(getCastChadows())
			{
				Ogre::Camera shadowCam("ShadowCameraSetupCam", 0);
				shadowCam._notifyViewport(camera->getViewport());
				Ogre::SceneManager* sm = mParentLight->_getManager();
				sm->getShadowCameraSetup()->getShadowCamera( sm, camera, camera->getViewport(), mParentLight, &shadowCam, 0 );
				
				//Get the shadow camera position
				params->setNamedConstant("shadowCamPos", shadowCam.getPosition());
				params->setNamedConstant("shadowFarClip", shadowCam.getFarClipDistance());
			}

		}//for
	}

}
}
#endif
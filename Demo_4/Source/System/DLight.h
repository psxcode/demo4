#pragma once
#include <OgreSimpleRenderable.h>
#include "System\MaterialGenerator.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	class DLight: public Ogre::SimpleRenderable
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DLight)
		//--------------------------------//
		DLight( MaterialGenerator::shared_ptr gen, Ogre::Light* parentLight );
		~DLight();

		/** Update the information from the light that matches this one 
		*/
		void updateFromParent();

		/** Update the information that is related to the camera
		*/
		void updateFromCamera(Ogre::Camera* camera);

		/** Does this light cast shadows?
		*/
		virtual bool getCastChadows() const;

		/** @copydoc MovableObject::getBoundingRadius */
		virtual Ogre::Real getBoundingRadius(void) const;

		/** @copydoc Renderable::getSquaredViewDepth */
		virtual Ogre::Real getSquaredViewDepth(const Ogre::Camera*) const;

		/** @copydoc Renderable::getMaterial */
		virtual const Ogre::MaterialPtr& getMaterial(void);

		/** @copydoc Renderable::getBoundingRadius */
		virtual void getWorldTransforms(Ogre::Matrix4* xform) const;

	protected:
		/** Check if the camera is inside a light
		*/
		bool isCameraInsideLight(Ogre::Camera* camera);

		/** Create geometry for this light.
		*/
		void rebuildGeometry(float radius);

		/** Create a sphere geometry.
		*/
		void createSphere(float radius, int nRings, int nSegments);

		/** Create a rectangle.
		*/
		void createRectangle2D();
		
		/** Create a cone.
		*/
		void createCone(float radius, float height, int nVerticesInBase);

		/** Set constant, linear, quadratic Attenuation terms 
		 */
		void setAttenuation(float c, float b, float a);

		/** Set the specular colour
		 */
		void setSpecularColour(const Ogre::ColourValue &col);

		/// The light that this DLight renders
		Ogre::Light* mParentLight;

		/// Mode to ignore world orientation/position
		bool bIgnoreWorld;

		/// Bounding sphere radius
		float mRadius;

		/// Deferred shading system this minilight is part of
		MaterialGenerator::shared_ptr mGenerator;
		Ogre::MaterialPtr mMaterial;

		/// Material permutation
		Ogre::uint32 mPermutation;
	};
	//------------------------------------------------------------
	class DeferredShadingSystem;
	class DFRLight
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DFRLight)
		//--------------------------------//
		DFRLight(Ogre::Light* pRefLight, DeferredShadingSystem* pDSS);
		virtual ~DFRLight();

		void updateFromParent(Ogre::Light* pRefLight = 0);
		void notifyCamera(Ogre::Camera* pCam);

		const Ogre::MaterialPtr & getMaterial()const {return m_spMaterial;}

	protected:
		void prepareGeometry();


		Ogre::Light* m_pRefLight;
		Ogre::Entity* m_pRefEntity;
		DeferredShadingSystem* m_pDSSystem;
		Ogre::MaterialPtr m_spMaterial;
		MaterialGenerator::perm_type mPermutation;
	};
	//------------------------------------------------------------
}
}

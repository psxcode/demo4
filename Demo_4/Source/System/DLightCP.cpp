#include "DemoStableHeaders.h"
#if DBS_LM_DEFERRED_RENDERING

#include <OgreTechnique.h>
#include <OgreSceneManager.h>
#include <OgreCompositorChain.h>
#include "System\DeferredMaterialGenerator.h"
#include "DLightCP.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	void injectTechnique( Ogre::SceneManager* sm, Ogre::Technique* tech, Ogre::Renderable* rend, const Ogre::LightList* lightList )
	{
		unsigned short numPasses( tech->getNumPasses() );
		for( unsigned short i = 0; i < numPasses; ++i )
		{
			Ogre::Pass* pass = tech->getPass( i );
			if( lightList != 0 )
				sm->_injectRenderWithPass( pass, rend, false, false, lightList );
			else
				sm->_injectRenderWithPass( pass, rend, false );
		}
	}
	//------------------------------------------------------------
	DLightRenderOperation::DLightRenderOperation( Ogre::CompositorInstance *inst, const Ogre::CompositionPass *pass, MaterialGenerator::shared_ptr lightMG )
	{
		viewport = inst->getChain()->getViewport();

		//light material generator
		if(lightMG)
		{
			lightMatGen = lightMG;
		}
		else
		{
			DeferredLightMaterialGenerator::InitData mat_idata;
			mat_idata.mgName = "dfrlightro";
			lightMatGen = DeferredLightMaterialGenerator::getInstance(mat_idata);
		}
	}
	//------------------------------------------------------------
	DLightRenderOperation::~DLightRenderOperation()
	{
		for(auto iter = lightMap.begin(), end_iter = lightMap.end(); iter != end_iter; ++iter)
		{
			Ogre::Light* light = iter->first;
			if(!light->getUserAny().isEmpty())
				light->setUserAny(Ogre::Any());
		}
		lightMap.clear();
	}
	//------------------------------------------------------------
	void DLightRenderOperation::execute( Ogre::SceneManager* sm, Ogre::RenderSystem* rs )
	{
		Ogre::Camera* cam = viewport->getCamera();
		Ogre::Technique* tech(0);

		const Ogre::LightList &lightList = sm->_getLightsAffectingFrustum();
		for( auto iter = lightList.begin(), end_iter = lightList.end(); iter != end_iter; ++iter )
		{
			Ogre::Light* light = *iter;
			Ogre::LightList ll;
			ll.push_back( light );

			DLight* pDLight = 0;
			if(!light->getUserAny().isEmpty())
			{
				pDLight = Ogre::any_cast<DLight*>(light->getUserAny());
			}
			else
			{
				LightMap::iterator liter = lightMap.find( light );
				if( liter == lightMap.end() )
				{
					pDLight = createLight(light).get();
				}
				else
				{
					pDLight = liter->second.get();
				}
				light->setUserAny(Ogre::Any(pDLight));
			}
			

			pDLight->updateFromCamera(cam);
			tech = pDLight->getMaterial()->getBestTechnique();

			//update shadow texture
			if( pDLight->getCastChadows() )
			{
				Ogre::SceneManager::RenderContext* context = sm->_pauseRendering();
				sm->prepareShadowTextures( cam, viewport, &ll );
				sm->_resumeRendering( context );

				Ogre::Pass* pass = tech->getPass(0);
				Ogre::TextureUnitState* tus = pass->getTextureUnitState( "ShadowMap" );
				assert(tus);
				const Ogre::TexturePtr &shadowTex = sm->getShadowTexture(0);
				if( tus->_getTexturePtr() != shadowTex )
					tus->_setTexturePtr( shadowTex );
			}

			injectTechnique( sm, tech, pDLight, &ll );
		}
	}
	//------------------------------------------------------------
	DLight::shared_ptr DLightRenderOperation::createLight(Ogre::Light *light)
	{
		DLight::shared_ptr rv( new DLight( lightMatGen, light ) );
		lightMap[ light ] = rv;
		return rv;
	}
	//------------------------------------------------------------
}
}
#endif
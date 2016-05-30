#pragma once
#include <OgreCompositorInstance.h>
#include <OgreCustomCompositionPass.h>

#include "System\DLight.h"
#include "System\MaterialGenerator.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	/** The render operation that will be called each frame in the custom composition pass
		This is the class that will send the actual render calls of the spheres (point lights),
		cones (spotlights) and quads (directional lights) after the GBuffer has been constructed
	*/
	class DLightRenderOperation : public Ogre::CompositorInstance::RenderSystemOperation
	{
	public:
		//--------------------------------//
		typedef Ogre::map< Ogre::Light*, DLight::shared_ptr >::type LightMap;
		//--------------------------------//
		DLightRenderOperation( Ogre::CompositorInstance* inst, const Ogre::CompositionPass* pass, MaterialGenerator::shared_ptr lightMG );
		virtual ~DLightRenderOperation();
		virtual void execute( Ogre::SceneManager* sm, Ogre::RenderSystem* rs );

	private:
		//create new light
		DLight::shared_ptr createLight( Ogre::Light* light );

		//the material generator for light geometry
		MaterialGenerator::shared_ptr lightMatGen;

		//the map of deferred lights geometries already constructed
		LightMap lightMap;

		//the viewport we are rendering to
		Ogre::Viewport* viewport;
	};
	//------------------------------------------------------------
	/** The custom composition pass that is used for rendering the light geometry
		This class needs to be registered with the CompositorManager
	*/
	class DLightCompositionPass : public Ogre::CustomCompositionPass
	{
	public:
		DLightCompositionPass() {}
		explicit DLightCompositionPass(MaterialGenerator::shared_ptr mg) : lightMatGen(mg) {}
		virtual Ogre::CompositorInstance::RenderSystemOperation* createOperation (
			Ogre::CompositorInstance* inst, const Ogre::CompositionPass* pass )
		{
			return OGRE_NEW DLightRenderOperation( inst, pass, lightMatGen );
		}

		MaterialGenerator::shared_ptr lightMatGen;

	protected:
		virtual ~DLightCompositionPass()
		{
		}
	};
	//------------------------------------------------------------
}
}
#include "OgrePhysXWorld.h"
#if DBS_LM_PHYSX

#define DBS_PHYSX_CHARACTER_CONTROLLER 0

#if DBS_PHYSX_CHARACTER_CONTROLLER
#	include <NxControllerManager.h>
#endif

#if DBS_LOGGING_ENABLED
#	include "OgrePhysXLogOutputStream.h"
#endif

#include "OgrePhysXScene.h"


namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
#if DBS_PHYSX_CHARACTER_CONTROLLER
	NxControllerManager	*gControllerManager = 0;
#endif

	NxPhysicsSDK *gSDK = 0;
//------------------------------------------------------------
World::World()
{
}
//------------------------------------------------------------
World::~World()
{
	shutdown();
}
//------------------------------------------------------------
void World::shutdown()
{
	if(gSDK)
	{
		gSDK->release();
		gSDK = 0;
	}
}
//------------------------------------------------------------
bool World::init()
{
	if(gSDK == 0)
	{
		gSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, 0,
#if DBS_LOGGING_ENABLED
			new LogOutputStream()
#else
			0
#endif
			);

		if(!gSDK)
		{
#if DBS_LOGGING_ENABLED
			Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: SDK initialisation failed.");
#endif
			return false;
		}

		gSDK->setParameter(NX_SKIN_WIDTH, 0.02f);

#if DBS_PHYSX_CHARACTER_CONTROLLER
		gControllerManager = NxCreateControllerManager(&gSDK->getFoundationSDK().getAllocator());
		if(!gControllerManager)
		{
#if DBS_LOGGING_ENABLED
			Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: Controller initialisation failed.");
#endif
			return false;
		}
#endif

#if DBS_LOGGING_ENABLED
		Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] SDK created.");
#endif
		return true;
	}
}
//------------------------------------------------------------
Scene* World::createMainScene( const NxSceneDesc &desc )
{
	Scene* scene = new
}
//------------------------------------------------------------
NxControllerManager* World::getControllerManager()
{
#if DBS_PHYSX_CHARACTER_CONTROLLER
	return gControllerManager;
#else
	return 0;
#endif
}
//------------------------------------------------------------
NxPhysicsSDK* World::getPhysicsSDK()
{
	return gSDK;
}
//------------------------------------------------------------
}
}
#endif
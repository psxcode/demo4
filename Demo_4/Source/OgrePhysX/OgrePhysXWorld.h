#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <NxPhysics.h>
#include "OgrePhysXClasses.h"

class NxControllerManager;

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
class World
{
public:
	bool init();
	void shutdown();

	NxPhysicsSDK* getPhysicsSDK();
	NxControllerManager* getControllerManager();

	Scene* createMainScene();
	Scene* createMainScene( const NxSceneDesc &desc );
	Scene* getMainScene();
	void releaseMainScene();

	void simulateMain(Ogre::Real time);
	void fetchMain();
	void syncMain();

private:
	World();
	~World();

	Scene* mpMainScene;
};
//------------------------------------------------------------
}
}
#endif
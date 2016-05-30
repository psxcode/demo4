#include "DemoStableHeaders.h"

#include "OgreInterface.h"
#include "MainController.h"

#if DBS_LM_PHYSX
#	include "PhysXInterface.h"
#endif

namespace Demo
{
namespace System
{
//------------------------------------------------------------
/*static*/ Ogre::RenderWindow* OgreInterface::getRenderWindow()
{
	return MainController::getSingletonPtr()->getOgreRenderWindow();
}
//------------------------------------------------------------
#if DBS_LM_PHYSX
NxPhysicsSDK* PhysXInterface::getPhysicsSDK()
{
	return MainController::getSingleton().getNxPhysicsSDK();
}
//------------------------------------------------------------
NxControllerManager* PhysXInterface::getControllerManager()
{
	return 0;
}
#endif
//------------------------------------------------------------
}
}
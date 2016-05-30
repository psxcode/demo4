#pragma once
#if DBS_LM_PHYSX

#include <NxPhysicsSDK.h>
#include <NxControllerManager.h>

namespace Demo
{
namespace System
{
//------------------------------------------------------------
class PhysXInterface
{
public:
	static NxPhysicsSDK* getPhysicsSDK();
	static NxControllerManager* getControllerManager();
};
//------------------------------------------------------------
}
}
#endif
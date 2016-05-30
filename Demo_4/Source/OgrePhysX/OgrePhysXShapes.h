#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <OgreEntity.h>
#include <NxBoxShapeDesc.h>
#include <NxSphereShapeDesc.h>
#include <NxCapsuleShapeDesc.h>
#include <NxPlaneShapeDesc.h>
#include "OgrePhysXConvert.h"

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
template<typename RealValue>
bool realValueNearZero( RealValue value )
{
	return fabs(value) < 0.001f;
}
//------------------------------------------------------------

}
}
#endif
#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
class RenderableBinding
{
public:
	virtual ~RenderableBinding() {}

	virtual void sync() = 0;
};
//------------------------------------------------------------
}
}
#endif
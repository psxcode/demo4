#pragma once

#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <NxScene.h>

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	class PhysXSceneInterface
	{
		friend class PhysXController;
	public:
		static inline NxScene* get(){ return _ptr; }
	private:
		static NxScene* _ptr;
	};
	//------------------------------------------------------------
}
}
#endif
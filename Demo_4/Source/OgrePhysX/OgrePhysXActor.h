#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <NxActor.h>
#include "shared_ptr_traits.h"

namespace Demo {
namespace OgrePhysX {
/*
class Actor
Wraps an NxActor.
*/
//------------------------------------------------------------
class Actor
{
	friend class Scene;
	friend class Ragdoll;
public:
	DEFINE_CLASS_SHARED_PTR(Actor)
	virtual ~Actor()
	{
		if(mNxActor) mNxActor->getScene().releaseActor(*mNxActor);
		mNxActor = 0;
	}

	inline NxActor* getNxActor() { return mNxActor; }
	
protected:
	Actor( NxActor *actor )
		: mNxActor(actor)
	{
	}

	NxActor *mNxActor;
};
//------------------------------------------------------------
}
}
#endif

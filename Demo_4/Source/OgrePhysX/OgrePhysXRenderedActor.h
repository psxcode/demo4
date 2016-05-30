#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include "OgrePhysXRenderableBinding.h"
#include "OgrePhysXActor.h"
#include "OgrePhysXPointRenderable.h"
#include "OgrePhysXConvert.h"

namespace Demo {
namespace OgrePhysX {
/*
class RenderedActor
implements the RenderableBinding interface.
*/
//------------------------------------------------------------
class RenderedActor : public Actor, public RenderableBinding
{
	friend class Scene;
public:
	DEFINE_CLASS_SHARED_PTR(RenderedActor)
	virtual ~RenderedActor() {}
	virtual void sync() {
		mRenderable->setTransform(Convert::toOgre(mNxActor->getGlobalPosition()), Convert::toOgre(mNxActor->getGlobalOrientation()));
	}

protected:
	RenderedActor( NxActor *actor, PointRenderable::shared_ptr renderable )
		: Actor(actor)
		, mRenderable(renderable)
	{
	}

	PointRenderable::shared_ptr mRenderable;
};
//------------------------------------------------------------
}
}
#endif
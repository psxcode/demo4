#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <OgreVector3.h>
#include "shared_ptr_traits.h"

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
class PointRenderable
{
public:
	DEFINE_CLASS_SHARED_PTR(PointRenderable)
	virtual ~PointRenderable() {}
	virtual void setTransform(Ogre::Vector3 const &position, Ogre::Quaternion const &rotation) = 0;
};
//------------------------------------------------------------
}
}
#endif
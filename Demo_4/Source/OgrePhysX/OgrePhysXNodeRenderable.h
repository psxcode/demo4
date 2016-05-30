#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <OgreSceneNode.h>
#include "OgrePhysXPointRenderable.h"

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
class NodeRenderable : public PointRenderable
{
public:
	NodeRenderable(Ogre::SceneNode* node)
		: mNode(node)
	{
	}
	virtual ~NodeRenderable() {}

	virtual void setTransform(const Ogre::Vector3 &position, const Ogre::Quaternion &rotation) {
		mNode->setPosition(position);
		mNode->setOrientation(rotation);
	}

private:
	Ogre::SceneNode *mNode;
};
//------------------------------------------------------------
}
}
#endif
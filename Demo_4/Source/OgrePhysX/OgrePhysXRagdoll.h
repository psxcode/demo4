#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <Ogre.h>
#include <NxD6Joint.h>
#include <NxD6JointDesc.h>

#include "OgrePhysXRenderableBinding.h"
#include "OgrePhysXActor.h"

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
struct sD6Joint
{
	NxD6Joint *mJoint;
	NxD6JointDesc mDescription;
};
//------------------------------------------------------------
struct sJointLimitParams
{
	Ogre::Real mValue;
	Ogre::Real mDamping;
	Ogre::Real mRestitution;
	Ogre::Real mSpring;
};
//------------------------------------------------------------
struct sBoneActorBindConfig
{
	typedef Demo::vector< sBoneActorBindConfig >::type Container;
	string_traits::str mBoneName;
	string_traits::str mParentName;
	Ogre::Real mBoneLength;
	Ogre::Vector3 mBoneOffset;
	Ogre::Quaternion mBoneOrientation;
	Ogre::Real mRadius;
	Ogre::Quaternion mJointOrientation;
	bool mNeedsJointOrientation;
	bool mHinge;
	sJointLimitParams mSwing1;
	sJointLimitParams mSwing2;
	sJointLimitParams mTwistMax;
	sJointLimitParams mTwistMin;
};
//------------------------------------------------------------
struct sBoneActorBind
{
	typedef Demo::vector< sBoneActorBind >::type Container;
	Ogre::Bone *mBone;
	Actor *mActor;
	sBoneActorBind *mParent;
	string_traits::str mParentBoneName;
	sD6Joint mJoint;
	bool mNeedsJointOrientation;
	Ogre::Quaternion mJointOrientation;
	Ogre::Vector3 mOffset;
	Ogre::Real mBoneLength;
	Ogre::Real mBoneRadius;
	bool mHinge;
	sJointLimitParams mSwing1;
	sJointLimitParams mSwing2;
	sJointLimitParams mTwistMax;
	sJointLimitParams mTwistMin;
	Ogre::Quaternion mBoneGlobalBindOrientation;
	Ogre::Quaternion mBoneActorGlobalBindOrientationInverse;
};
//------------------------------------------------------------
class Ragdoll : public RenderableBinding
{
	friend class Scene;
private:
	NxScene *mNxScene;
	std::vector<sD6Joint> mJoints;
	Ogre::Entity *mEntity;
	Ogre::SceneNode *mNode;
	bool mControlledByActors;
	sBoneActorBind::Container mSkeleton;

	Ragdoll(NxScene* scene, Ogre::Entity *ent, Ogre::SceneNode *node);
	virtual ~Ragdoll(void);

	/*
	CreateBoneActors and CreateJoints create a ragdoll skeleton from scratch.
	They are only used the first time the ragdoll is created, after that a config file will be exported.
	*/
	sBoneActorBindConfig::Container generateAutoSkeleton();
	void createJoints();

	/*
	Creates a skeleton from a config file.
	*/
	void createSkeleton(sBoneActorBindConfig::Container &config);

	void updateBoneActors();
	void updateVisualBones();
	void setAllBonesToManualControl(bool manual);

public:
	void setControlToActors();
	void setControlToBones();
	bool isControlledByActors() { return mControlledByActors; }
	void resetBones();

	inline sBoneActorBind::Container& getSkeleton() { return mSkeleton; }
	void serialise(sBoneActorBindConfig::Container const &config, string_traits::in filename);

	inline Ogre::Entity* getEntity() { return mEntity; }

	virtual void sync() {
		if (mControlledByActors) updateVisualBones();
		else updateBoneActors();
	}
};
//------------------------------------------------------------
}
}
#endif
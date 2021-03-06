#include "OgrePhysXRagdoll.h"
#if DBS_LM_PHYSX

#include <fstream>
#include <NxUserContactReport.h>
#include "OgrePhysXShapes.h"
#include "OgrePhysXConvert.h"

#pragma warning (disable:4482)

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
/*virtual*/ Ragdoll::~Ragdoll(void)
{
	auto i = mSkeleton.begin();
	auto ei = mSkeleton.end();
	for(; i != ei; ++i)
	{
		if ((*i).mParent) mNxScene->releaseJoint(*(*i).mJoint.mJoint);
		delete (*i).mActor;
	}
}
//------------------------------------------------------------
Ragdoll::Ragdoll(NxScene *scene, Ogre::Entity *ent, Ogre::SceneNode *node)
{
	mNxScene = scene;
	mNode = node;
	Ogre::Vector3 scale = mNode->getScale();
	mEntity = ent;
	if (!mEntity->getSkeleton()) OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Entity " + mEntity->getName() + " has no skeleton!", "OgrePhysX::Ragdoll::Ragdoll");
	resetBones();
	if (Ogre::ResourceGroupManager::getSingleton().resourceExists("General", mEntity->getMesh()->getName() + ".bones"))
	{
		sBoneActorBindConfig::Container bones;
		Ogre::ConfigFile cf;
		cf.loadFromResourceSystem(mEntity->getMesh()->getName() + ".bones", "General");
		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
		Ogre::String secName;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *Settings = seci.getNext();
			if (secName.find("Bone") != Ogre::String::npos)
			{
				Ogre::String bonename, boneparentname, shaperadius, shapelength, shapeoffset, shapeorientation, jointorientation, hinge, swing1_value,swing1_damping,swing1_restitution,swing1_spring, swing2_value,swing2_damping,swing2_restitution,swing2_spring, twistmax_value,twistmax_damping,twistmax_restitution,twistmax_spring, twistmin_value,twistmin_damping,twistmin_restitution,twistmin_spring;
				auto i = Settings->begin();
				auto ei = Settings->end();
				for( ; i != ei; ++i )
				{
					if (i->first == "BoneName") bonename = i->second;
					if (i->first == "ParentName") boneparentname = i->second;
					if (i->first == "BoneLength") shapelength = i->second;
					if (i->first == "BoneOffset") shapeoffset = i->second;
					if (i->first == "BoneOrientation") shapeorientation = i->second;
					if (i->first == "Radius") shaperadius = i->second;
					if (i->first == "JointOrientation") jointorientation = i->second;
					if (i->first == "Hinge") hinge = i->second;

					if (i->first == "Swing1_Value") swing1_value = i->second;
					if (i->first == "Swing1_Damping") swing1_damping = i->second;
					if (i->first == "Swing1_Restitution") swing1_restitution = i->second;
					if (i->first == "Swing1_Spring") swing1_spring = i->second;

					if (i->first == "Swing2_Value") swing2_value = i->second;
					if (i->first == "Swing2_Damping") swing2_damping = i->second;
					if (i->first == "Swing2_Restitution") swing2_restitution = i->second;
					if (i->first == "Swing2_Spring") swing2_spring = i->second;

					if (i->first == "TwistMax_Value") twistmax_value = i->second;
					if (i->first == "TwistMax_Damping") twistmax_damping = i->second;
					if (i->first == "TwistMax_Restitution") twistmax_restitution = i->second;
					if (i->first == "TwistMax_Spring") twistmax_spring = i->second;

					if (i->first == "TwistMin_Value") twistmin_value = i->second;
					if (i->first == "TwistMin_Damping") twistmin_damping = i->second;
					if (i->first == "TwistMin_Restitution") twistmin_restitution = i->second;
					if (i->first == "TwistMin_Spring") twistmin_spring = i->second;
				}
				if (bonename == "" || boneparentname == "")
				{
#if DBS_LOGGING_ENABLED
					Ogre::LogManager::getSingleton().logMessage("Ragdoll::Ragdoll: Error in Section " + secName + "! -> continue");
#endif
					continue;
				}
				sBoneActorBindConfig boneconfig;
				if (jointorientation == "")
				{
					boneconfig.mJointOrientation = Ogre::Quaternion();
					boneconfig.mNeedsJointOrientation = true;
				}
				else
				{
					boneconfig.mJointOrientation = Ogre::StringConverter::parseQuaternion(jointorientation);
					boneconfig.mNeedsJointOrientation = false;
				}
				boneconfig.mBoneName = bonename;
				boneconfig.mParentName = boneparentname;
				boneconfig.mBoneLength = Ogre::StringConverter::parseReal(shapelength);
				boneconfig.mBoneOffset = Ogre::StringConverter::parseVector3(shapeoffset);
				boneconfig.mBoneOrientation = Ogre::StringConverter::parseQuaternion(shapeorientation);
				boneconfig.mRadius = Ogre::StringConverter::parseReal(shaperadius);
				boneconfig.mHinge = Ogre::StringConverter::parseBool(hinge);
				boneconfig.mSwing1.mValue = Ogre::StringConverter::parseReal(swing1_value);
				boneconfig.mSwing1.mDamping = Ogre::StringConverter::parseReal(swing1_damping);
				boneconfig.mSwing1.mRestitution = Ogre::StringConverter::parseReal(swing1_restitution);
				boneconfig.mSwing1.mSpring = Ogre::StringConverter::parseReal(swing1_spring);

				boneconfig.mSwing2.mValue = Ogre::StringConverter::parseReal(swing2_value);
				boneconfig.mSwing2.mDamping = Ogre::StringConverter::parseReal(swing2_damping);
				boneconfig.mSwing2.mRestitution = Ogre::StringConverter::parseReal(swing2_restitution);
				boneconfig.mSwing2.mSpring = Ogre::StringConverter::parseReal(swing2_spring);

				boneconfig.mTwistMax.mValue = Ogre::StringConverter::parseReal(twistmax_value);
				boneconfig.mTwistMax.mDamping = Ogre::StringConverter::parseReal(twistmax_damping);
				boneconfig.mTwistMax.mRestitution = Ogre::StringConverter::parseReal(twistmax_restitution);
				boneconfig.mTwistMax.mSpring = Ogre::StringConverter::parseReal(twistmax_spring);

				boneconfig.mTwistMin.mValue = Ogre::StringConverter::parseReal(twistmin_value);
				boneconfig.mTwistMin.mDamping = Ogre::StringConverter::parseReal(twistmin_damping);
				boneconfig.mTwistMin.mRestitution = Ogre::StringConverter::parseReal(twistmin_restitution);
				boneconfig.mTwistMin.mSpring = Ogre::StringConverter::parseReal(twistmin_spring);

				bones.push_back(boneconfig);
			}
		}	
		createSkeleton(bones);
	}
	else
	{
#if DBS_LOGGING_ENABLED
		Ogre::LogManager::getSingleton().logMessage("Could not find " + mEntity->getMesh()->getName() + ".bones, generating new...");
#endif
		sBoneActorBindConfig::Container boneconfig = generateAutoSkeleton();
		createSkeleton(boneconfig);
	}
	setControlToBones();
}
//------------------------------------------------------------
void Ragdoll::serialise(sBoneActorBindConfig::Container const &boneconfig, string_traits::in filename)
{
	std::fstream f;
	f.open(filename.c_str(), std::ios::out | std::ios::trunc);
	auto i = boneconfig.begin();
	auto ei = boneconfig.end();
	for(; i != ei; ++i)
	{
		f << "[Bone_"					<< (*i).mBoneName.c_str() << "]" << std::endl;
		f << "BoneName = "				<< (*i).mBoneName.c_str() << std::endl;
		if ((*i).mParentName != "")
		{
			f << "ParentName = "		<< (*i).mParentName.c_str() << std::endl;
		}
		else
		{
			f << "ParentName = None" << std::endl;
		}
		f << "BoneLength = "			<< (*i).mBoneLength << std::endl;
		f << "BoneOffset = "			<< Ogre::StringConverter::toString((*i).mBoneOffset) << std::endl;
		f << "BoneOrientation = "		<< Ogre::StringConverter::toString((*i).mBoneOrientation) << std::endl;
		f << "Radius = "				<< Ogre::StringConverter::toString((*i).mRadius) << std::endl;
		f << "JointOrientation = "		<< Ogre::StringConverter::toString((*i).mJointOrientation) << std::endl;
		f << "Hinge = False"			<< std::endl;

		f << "Swing1_Value = "			<< (*i).mSwing1.mValue << std::endl;
		f << "Swing1_Damping = "		<< (*i).mSwing1.mDamping << std::endl;
		f << "Swing1_Restitution = "	<< (*i).mSwing1.mRestitution << std::endl;
		f << "Swing1_Spring = "			<< (*i).mSwing1.mSpring << std::endl;

		f << "Swing2_Value = "			<< (*i).mSwing2.mValue << std::endl;
		f << "Swing2_Damping = "		<< (*i).mSwing2.mDamping << std::endl;
		f << "Swing2_Restitution = "	<< (*i).mSwing2.mRestitution << std::endl;
		f << "Swing2_Spring = "			<< (*i).mSwing2.mSpring << std::endl;

		f << "TwistMax_Value = "		<< (*i).mTwistMax.mValue << std::endl;
		f << "TwistMax_Damping = "		<< (*i).mTwistMax.mDamping << std::endl;
		f << "TwistMax_Restitution = "	<< (*i).mTwistMax.mRestitution << std::endl;
		f << "TwistMax_Spring = "		<< (*i).mTwistMax.mSpring << std::endl;

		f << "TwistMin_Value = "		<< (*i).mTwistMin.mValue << std::endl;
		f << "TwistMin_Damping = "		<< (*i).mTwistMin.mDamping << std::endl;
		f << "TwistMin_Restitution = "	<< (*i).mTwistMin.mRestitution << std::endl;
		f << "TwistMin_Spring = "		<< (*i).mTwistMin.mSpring << std::endl;

		f << std::endl;
	}
	f.close();
}
//------------------------------------------------------------
void Ragdoll::createSkeleton(sBoneActorBindConfig::Container &config)
{
	Ogre::Vector3 scale = mEntity->getParentSceneNode()->_getDerivedScale();
	std::for_each(config.begin(), config.end(), [this, &scale](sBoneActorBindConfig &i)
	{
		Ogre::Bone *bone = mEntity->getSkeleton()->getBone(i.mBoneName);
		Ogre::Vector3 mypos = bone->_getDerivedPosition();
		Ogre::Vector3 parentpos = mEntity->getParentSceneNode()->_getDerivedPosition();
		if (i.mParentName != "None") parentpos = mEntity->getSkeleton()->getBone(i.mParentName)->_getDerivedPosition();
		if (fabs(i.mBoneLength) < Ogre::Real(.001))
		{
#if DBS_LOGGING_ENABLED
			Ogre::LogManager::getSingleton().logMessage("Error in Ragdoll::CreateSkeleton: Bone length for " + (*i).mBoneName + " is 0.");
#endif
			return;
		}
		sBoneActorBind bone_actor_bind;
		bone_actor_bind.mBone = bone;
		Ogre::Real scale_factor = ((scale.x + scale.y + scale.z) / 3);
		Ogre::Real capsule_radius = i.mRadius * scale_factor;
		if (capsule_radius <= 0.05f) capsule_radius = 0.05f;
		Ogre::Real scaled_bonelength = i.mBoneLength * scale_factor;
		Ogre::Real capsule_height = scaled_bonelength - capsule_radius * 2.0f;
		if (capsule_height <= 0.0f) capsule_height = 0.01f;

		bone_actor_bind.mOffset = i.mBoneOffset;
		Ogre::Vector3 localBonePos = (bone->_getDerivedPosition() + i.mBoneOffset) * scale;
			
		bone_actor_bind.mActor = new Actor(mNxScene,
			CapsuleShape(capsule_radius, capsule_height).mass(10).localPose(Ogre::Vector3(0,scaled_bonelength/2,0)),
			localBonePos + mEntity->getParentSceneNode()->_getDerivedPosition(),
			i.mBoneOrientation);
		bone_actor_bind.mActor->getNxActor()->setSolverIterationCount(12);
		bone_actor_bind.mParent = 0;
		bone_actor_bind.mParentBoneName = i.mParentName;
		bone_actor_bind.mNeedsJointOrientation = i.mNeedsJointOrientation;
		bone_actor_bind.mJointOrientation = i.mJointOrientation;
		bone_actor_bind.mSwing1 = i.mSwing1;
		bone_actor_bind.mSwing2 = i.mSwing2;
		bone_actor_bind.mTwistMin = i.mTwistMin;
		bone_actor_bind.mTwistMax = i.mTwistMax;

		bone_actor_bind.mBoneLength = i.mBoneLength;
		bone_actor_bind.mBoneRadius = i.mRadius;
		bone_actor_bind.mBoneGlobalBindOrientation = bone_actor_bind.mBone->_getDerivedOrientation();
		bone_actor_bind.mBoneActorGlobalBindOrientationInverse = bone_actor_bind.mActor->getGlobalOrientation().Inverse();
		mSkeleton.push_back(bone_actor_bind);
	});
	if (mSkeleton.size() != config.size())
	{
#if DBS_LOGGING_ENABLED
		Ogre::LogManager::getSingleton().logMessage("Error in Ragdoll::CreateSkeleton: Invalid bone configuration, canceling skeleton generation...");
#endif
		return;
	}
	auto i = mSkeleton.begin();	auto ei = mSkeleton.end();
	for(; i != ei; ++i)
	{
		auto x = mSkeleton.begin(); auto ex = mSkeleton.end();
		for(; x != ex; ++x)
		{
			if ((*i).mParentBoneName == (*x).mBone->getName())
			{
				(*i).mParent = &(*x);

				auto j = config.begin(); auto ej = config.end();
				for(; j != ej; ++j)
				{
					if ((*j).mNeedsJointOrientation && (*j).mBoneName == (*i).mBone->getName())
					{
						(*j).mJointOrientation = (*i).mParent->mActor->getGlobalOrientation().Inverse() * (*i).mActor->getGlobalOrientation();
						(*j).mNeedsJointOrientation = false;
						(*i).mJointOrientation = (*j).mJointOrientation;
					}
				}
			}
		}
	}
	createJoints();
}
//------------------------------------------------------------
sBoneActorBindConfig::Container Ragdoll::generateAutoSkeleton()
{
	sBoneActorBindConfig::Container returner;
	Ogre::SkeletonInstance* skeletonInst = mEntity->getSkeleton();
	Ogre::Skeleton::BoneIterator boneI=skeletonInst->getBoneIterator();
	Ogre::Vector3 scale = mEntity->getParentSceneNode()->_getDerivedScale();
	while(boneI.hasMoreElements())
	{
		Ogre::Bone* bone=boneI.getNext();
		Ogre::String bName=bone->getName();

		Ogre::Vector3 difference = Ogre::Vector3(0,1,0);
		Ogre::Vector3 mypos = bone->_getDerivedPosition();
		Ogre::Quaternion estimated_orientation = Ogre::Quaternion();//0.707107, 0, 0, 0.707107);
		if (bone->numChildren() == 1 && bone->getChild(0))
		{
			//points towards child
			Ogre::Vector3 childpos = bone->getChild(0)->_getDerivedPosition();
			if (childpos == mypos) continue;
			difference = childpos - mypos;
			estimated_orientation = Ogre::Vector3::UNIT_Y.getRotationTo(difference.normalisedCopy());
		}
		else if (bone->getParent())
		{
			//points away from parent
			difference = Ogre::Vector3(1,0,0);
			Ogre::Vector3 parentpos = bone->getParent()->_getDerivedPosition();
			if (parentpos == mypos) continue;
			difference = parentpos - mypos;
			estimated_orientation = Ogre::Vector3::UNIT_Y.getRotationTo(-difference.normalisedCopy());
			difference.normalise();
			difference = difference * 0.1f * (mEntity->getBoundingRadius() / ((scale.x + scale.y + scale.z) / 3.0f));
		}
		if (difference.length() > 0)
		{
				sBoneActorBindConfig bone_actor_bind;
				bone_actor_bind.mBoneName = bone->getName();
				if (!bone->getParent()) bone_actor_bind.mParentName = "None";
				else bone_actor_bind.mParentName = bone->getParent()->getName();//GetRealParent(bone)->getName();

				bone_actor_bind.mBoneOffset = Ogre::Vector3(0,0,0);
				bone_actor_bind.mJointOrientation = Ogre::Quaternion();
				bone_actor_bind.mNeedsJointOrientation = true;
				bone_actor_bind.mTwistMax.mValue = 20.0f;
				bone_actor_bind.mTwistMax.mDamping = 0.5f;
				bone_actor_bind.mTwistMax.mRestitution = 0.0f;
				bone_actor_bind.mTwistMax.mSpring = 0.0f;

				bone_actor_bind.mTwistMin.mValue = -20.0f;
				bone_actor_bind.mTwistMin.mDamping = 0.5f;
				bone_actor_bind.mTwistMin.mRestitution = 0.0f;
				bone_actor_bind.mTwistMin.mSpring = 0.0f;

				bone_actor_bind.mSwing1.mValue = 30.0f;
				bone_actor_bind.mSwing1.mDamping = 0.5f;
				bone_actor_bind.mSwing1.mRestitution = 0.0f;
				bone_actor_bind.mSwing1.mSpring = 0.0f;

				bone_actor_bind.mSwing2.mValue = 30.0f;
				bone_actor_bind.mSwing2.mDamping = 0.5f;
				bone_actor_bind.mSwing2.mRestitution = 0.0f;
				bone_actor_bind.mSwing2.mSpring = 0.0f;

				bone_actor_bind.mHinge = false;
				float boneLength = difference.length();
				bone_actor_bind.mBoneLength = boneLength * 0.98f;
				float bradius = mEntity->getBoundingRadius() / ((scale.x + scale.y + scale.z) / 3.0f);
				float minRadius = bradius * 0.02f;
				float maxRadius = bradius * 0.035f;
				float estimatedRadius = bone_actor_bind.mBoneLength * 0.25f;
				if (estimatedRadius < minRadius) bone_actor_bind.mRadius = minRadius;
				else if (estimatedRadius > maxRadius) bone_actor_bind.mRadius = maxRadius;
				else bone_actor_bind.mRadius = estimatedRadius;
				bone_actor_bind.mBoneOrientation = estimated_orientation;
				returner.push_back(bone_actor_bind);
			}
		}
	return returner;
}
//------------------------------------------------------------
void Ragdoll::createJoints()
{
	Ogre::Vector3 scale = mEntity->getParentSceneNode()->_getDerivedScale();
	int counter = 0;
	std::for_each(mSkeleton.begin(), mSkeleton.end(), [&]( sBoneActorBind &i ) {
		if (i.mParent == 0) return;
		mNxScene->setActorPairFlags(*i.mParent->mActor->getNxActor(), *i.mActor->getNxActor(), NX_IGNORE_PAIR);
		NxD6JointDesc d6Desc;
		d6Desc.actor[0] = i.mParent->mActor->getNxActor();
		d6Desc.actor[1] = i.mActor->getNxActor();
		Ogre::Vector3 position = i.mActor->getGlobalPosition() - i.mParent->mActor->getGlobalPosition();
		Ogre::Quaternion orientation = i.mJointOrientation;
		position = position * scale;

#if DBS_LOGGING_ENABLED
		Ogre::LogManager::getSingleton().logMessage("Creating joint: " + i.mParent->mBone->getName() + " + " + i.mBone->getName());
#endif

		NxMat34 basePose = NxMat34(NxMat33(Convert::toNx(i.mActor->getGlobalOrientation())), Convert::toNx(i.mActor->getGlobalPosition()));
		NxMat34 localPose = NxMat34(NxMat33(Convert::toNx(orientation)), Convert::toNx(position));
		d6Desc.localAxis[0] = localPose.M*NxVec3(1.0f,0.0f,0.0f);
		d6Desc.localAxis[1]=NxVec3(1.0f,0.0f,0.0f);
		d6Desc.localNormal[0]=localPose.M*NxVec3(0.0f,1.0f,0.0f);
		d6Desc.localNormal[1]=NxVec3(0.0f,1.0f,0.0f);
		d6Desc.setGlobalAnchor(basePose*NxVec3(0.0f,0.0f,0.0f));
		d6Desc.xMotion = NX_D6JOINT_MOTION_LOCKED;
		d6Desc.yMotion = NX_D6JOINT_MOTION_LOCKED;
		d6Desc.zMotion = NX_D6JOINT_MOTION_LOCKED;
		d6Desc.twistMotion = NX_D6JOINT_MOTION_FREE;
		d6Desc.swing1Motion = NX_D6JOINT_MOTION_FREE;
		d6Desc.swing2Motion = NX_D6JOINT_MOTION_FREE;

		d6Desc.flags|=NX_D6JOINT_SLERP_DRIVE;
		d6Desc.slerpDrive.driveType=NX_D6JOINT_DRIVE_POSITION;// | NX_D6JOINT_DRIVE_VELOCITY;
		d6Desc.slerpDrive.forceLimit=0.0f; //Not used with the current drive model.
		d6Desc.slerpDrive.spring=250.0f;
		d6Desc.slerpDrive.damping=0.01f;

		NxD6Joint *d6Joint=(NxD6Joint*)mNxScene->createJoint(d6Desc);
		sD6Joint joint;
		joint.mJoint = d6Joint;
		joint.mDescription = d6Desc;
		i.mJoint = joint;
		counter++;
	});
#if DBS_LOGGING_ENABLED
	Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(counter) + " Joints wurden erstellt.");
#endif
}
//------------------------------------------------------------
void Ragdoll::updateBoneActors()
{
	Ogre::Vector3 scale = mEntity->getParentSceneNode()->_getDerivedScale();
	std::for_each(mSkeleton.begin(), mSkeleton.end(), [&]( sBoneActorBind &i ) {
		Ogre::Vector3 position;
		//update Bone Actor
		if (i.mParent)
		{
			position = (mEntity->getParentSceneNode()->_getDerivedOrientation() * (i.mBone->_getDerivedPosition()+i.mOffset) * scale) + mEntity->getParentSceneNode()->_getDerivedPosition();
			i.mActor->setGlobalPosition(position);  
			Ogre::Quaternion drive_orientation = i.mBone->_getDerivedOrientation();
			drive_orientation = i.mParent->mBone->_getDerivedOrientation().Inverse() * drive_orientation;
			i.mJoint.mJoint->setDriveOrientation(Convert::toNx(drive_orientation));
		}
		else
		{
			position = (mEntity->getParentSceneNode()->_getDerivedOrientation() * (i.mBone->_getDerivedPosition() + i.mOffset) * scale) + mEntity->getParentSceneNode()->_getDerivedPosition();
			i.mActor->setGlobalPosition(position);  
		}
	});
}
//------------------------------------------------------------
void Ragdoll::updateVisualBones()
{
	Ogre::Quaternion PhysxRotation, OgreGlobalQuat, NodeRotation = mEntity->getParentSceneNode()->_getDerivedOrientation();
	std::for_each(mSkeleton.begin(), mSkeleton.end(), [&]( sBoneActorBind& i ) {
		Ogre::Bone *bone = i.mBone;
		PhysxRotation = i.mActor->getGlobalOrientation() * i.mBoneActorGlobalBindOrientationInverse;
		OgreGlobalQuat = PhysxRotation * i.mBoneGlobalBindOrientation;
		Ogre::Quaternion ParentInverse = NodeRotation.Inverse();
		if (i.mParent == 0)
		{
			Ogre::Vector3 newPos = i.mActor->getGlobalPosition() - (mEntity->getParentSceneNode()->_getDerivedOrientation() * (bone->_getDerivedPosition() * mEntity->getParentSceneNode()->getScale()));
			mEntity->getParentSceneNode()->setPosition(newPos);
		}
		bone->setOrientation(ParentInverse * OgreGlobalQuat);
	});
}
//------------------------------------------------------------
void Ragdoll::setAllBonesToManualControl(bool manual)
{
	Ogre::SkeletonInstance* skeletonInst = mEntity->getSkeleton();
	Ogre::Skeleton::BoneIterator boneI=skeletonInst->getBoneIterator();

	while(boneI.hasMoreElements())
	{
		Ogre::Bone *bone = boneI.getNext();
		bone->setManuallyControlled(manual); 
	}
}
//------------------------------------------------------------
void Ragdoll::resetBones()
{
	Ogre::SkeletonInstance* skeletonInst = mEntity->getSkeleton();
	Ogre::Skeleton::BoneIterator boneI=skeletonInst->getBoneIterator();

	while(boneI.hasMoreElements())
		boneI.getNext()->reset();
}
//------------------------------------------------------------
void Ragdoll::setControlToActors()
{
	mControlledByActors = true;
	Ogre::AnimationStateSet* set = mEntity->getAllAnimationStates();
	Ogre::AnimationStateIterator it = set->getAnimationStateIterator();
	Ogre::AnimationState *anim = 0;   
	while(it.hasMoreElements())
	{
		anim = it.getNext();
		anim->setLoop(false);
		anim->setEnabled(false);
		anim->setWeight(0);
	}

	setAllBonesToManualControl(true);
	std::for_each(mSkeleton.begin(), mSkeleton.end(), []( sBoneActorBind& i ) {
		//i.mBoneGlobalBindOrientation = mEntity->getParentSceneNode()->_getDerivedOrientation() * i.mBone->_getDerivedOrientation();
		//i.mBoneActorGlobalBindOrientationInverse = i.mActor->getGlobalOrientationAsOgreQuaternion().Inverse();
		i.mBone->setInheritOrientation(false); //!!!

		i.mActor->getNxActor()->clearBodyFlag(NxBodyFlag::NX_BF_FROZEN_POS);
		if (i.mParent)
		{
			i.mJoint.mDescription.slerpDrive.driveType=0;
			i.mJoint.mDescription.slerpDrive.forceLimit=0.0f;
			i.mJoint.mDescription.slerpDrive.spring=0.0f;
			i.mJoint.mDescription.slerpDrive.damping=0.0f;

			i.mJoint.mDescription.twistMotion = NX_D6JOINT_MOTION_LOCKED;
			i.mJoint.mDescription.swing1Motion = NX_D6JOINT_MOTION_LOCKED;
			i.mJoint.mDescription.swing2Motion = NX_D6JOINT_MOTION_LOCKED;

			if (i.mTwistMax.mValue != 0.0f || i.mTwistMax.mValue != 0.0f)
			{
				i.mJoint.mDescription.twistMotion = NX_D6JOINT_MOTION_LIMITED;
				i.mJoint.mDescription.twistLimit.low.value=NxMath::degToRad(i.mTwistMin.mValue);
				i.mJoint.mDescription.twistLimit.low.damping=i.mTwistMin.mDamping;
				i.mJoint.mDescription.twistLimit.low.restitution=i.mTwistMin.mRestitution;
				i.mJoint.mDescription.twistLimit.low.spring=i.mTwistMin.mSpring;
				i.mJoint.mDescription.twistLimit.high.value=NxMath::degToRad(i.mTwistMax.mValue);
				i.mJoint.mDescription.twistLimit.high.damping=i.mTwistMax.mDamping;
				i.mJoint.mDescription.twistLimit.high.restitution=i.mTwistMax.mRestitution;
				i.mJoint.mDescription.twistLimit.high.spring=i.mTwistMax.mSpring;
			}
			if (i.mSwing1.mValue > 0.0f)
			{
				i.mJoint.mDescription.swing1Motion = NX_D6JOINT_MOTION_LIMITED;
				i.mJoint.mDescription.swing1Limit.value=NxMath::degToRad(i.mSwing1.mValue);
				i.mJoint.mDescription.swing1Limit.damping=i.mSwing1.mDamping;
				i.mJoint.mDescription.swing1Limit.restitution=i.mSwing1.mRestitution;
				i.mJoint.mDescription.swing1Limit.spring=i.mSwing1.mSpring;
			}
			if (i.mSwing2.mValue > 0.0f)
			{
				i.mJoint.mDescription.swing2Motion = NX_D6JOINT_MOTION_LIMITED;
				i.mJoint.mDescription.swing2Limit.value=NxMath::degToRad(i.mSwing2.mValue);
				i.mJoint.mDescription.swing2Limit.damping=i.mSwing2.mDamping;
				i.mJoint.mDescription.swing2Limit.restitution=i.mSwing2.mRestitution;
				i.mJoint.mDescription.swing2Limit.spring=i.mSwing2.mSpring;
			}
			i.mJoint.mJoint->loadFromDesc(i.mJoint.mDescription);
		}
	});
}
//------------------------------------------------------------
void Ragdoll::setControlToBones()
{
	mControlledByActors = false;
	setAllBonesToManualControl(false);
	std::for_each(mSkeleton.begin(), mSkeleton.end(), []( sBoneActorBind &i ) {
		//(*i).mActor->raiseActorFlag(NxActorFlag::NX_AF_DISABLE_RESPONSE);
		i.mActor->getNxActor()->raiseBodyFlag(NxBodyFlag::NX_BF_FROZEN_POS);
		if (i.mParent) {
			i.mJoint.mDescription.slerpDrive.driveType=NX_D6JOINT_DRIVE_POSITION;
			i.mJoint.mDescription.slerpDrive.forceLimit=0.0f;
			i.mJoint.mDescription.slerpDrive.spring=250.0f;
			i.mJoint.mDescription.slerpDrive.damping=0.0f;
			i.mJoint.mJoint->loadFromDesc(i.mJoint.mDescription);
		}
	});
}
//------------------------------------------------------------
}
}
#endif
#pragma once

#include <NxQuat.h>
#include <OgreVector3.h>

namespace Demo {
//------------------------------------------------------------
class OgrePhysXConverter
{
public:
	static inline Ogre::Vector3 toOgre(const NxVec3 &vec3)
	{
		return Ogre::Vector3(vec3.x, vec3.y, vec3.z);
	}
	static inline Ogre::Quaternion toOgre(const NxQuat &q)
	{
		return Ogre::Quaternion(q.w, q.x, q.y, q.z);
	}
	static inline NxVec3 toNx(const Ogre::Vector3 &vec3)
	{
		return NxVec3(vec3.x, vec3.y, vec3.z);
	}
	static inline NxQuat toNx(const Ogre::Quaternion &q)
	{
		NxQuat nxq; nxq.setWXYZ(q.w, q.x, q.y, q.z);
		return nxq;
	}
};
//------------------------------------------------------------
}
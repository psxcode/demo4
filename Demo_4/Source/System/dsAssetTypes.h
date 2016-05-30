#pragma once

#include "dsSceneTypes.h"

namespace Demo
{
namespace System
{
	
	//------------------------------------------------------------
	class dsLightGeom
	{
	public:
		dsLightGeom();
		~dsLightGeom();

		string_traits::str name;
		string_traits::str lightMeshTypeName;

		bool meshWithGeomLods;
		dsEntityLodBias meshLodBias;

		bool meshWithMaterialLods;
		Ogre::Real materialLodBias;

		Ogre::Real maxSize;
	};
	//------------------------------------------------------------
	class dsAsset
	{
	public:
		virtual ~dsAsset()
		{
		}

		string_traits::str name;

		dsSceneNode::shared_ptr sn;
	};
	//------------------------------------------------------------
}
}
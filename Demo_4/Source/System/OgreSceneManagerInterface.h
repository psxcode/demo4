#pragma once
#include <OgreSceneManager.h>

#include "dotScene/SceneTypes.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	class OgreSceneManagerInterface
	{
		friend class OgreController;
	public:
		static inline Ogre::SceneManager* get(){ return _ptr; }
		static void destroySceneNodeAndObjects( Ogre::SceneNode* osn );
		static Ogre::SceneNode* createSceneNode( dotScene::SceneNode::const_shared_ptr dsn, void *objPtr );
		static Ogre::MovableObject* createMovableObject( dotScene::MovableObject::const_shared_ptr mo, void *objPtr );
		static Ogre::Entity* createEntity( dotScene::Entity::const_shared_ptr ent, void *objPtr );
		static Ogre::Light* createLight( dotScene::Light::const_shared_ptr light, void *objPtr );
		static Ogre::Camera* createCamera( dotScene::Camera::const_shared_ptr cam, void *objPtr );

	private:
		static Ogre::SceneManager* _ptr;
	};
	//------------------------------------------------------------
}
}
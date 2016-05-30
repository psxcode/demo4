#include "DemoStableHeaders.h"
#include "Factory.h"
#include <OgreString.h>
#include <OgreSubEntity.h>
#include <OgreMeshManager.h>
#include <OgreLodStrategyManager.h>

namespace Demo {
namespace System {
//------------------------------------------------------------
Ogre::uint32 DefaultSceneFactory::msNodeCounter = 0;
Ogre::uint32 DefaultSceneFactory::msObjCounter = 0;
//------------------------------------------------------------
/*virtual*/ Ogre::SceneNode* DefaultSceneFactory::createSceneNode_default(string_traits::in namePrefix)
{
	return mpSceneManager->createSceneNode(_generateNodeName(namePrefix));
}
//------------------------------------------------------------
/*virtual*/ Ogre::SceneNode* DefaultSceneFactory::createSceneNode(System::dsSceneNode::const_shared_ptr sn)
{
	Ogre::SceneNode *osn = mpSceneManager->createSceneNode(_generateNodeName(sn->name));
	osn->setPosition( sn->position );
	osn->setOrientation( sn->orientation );
	osn->setScale( sn->scale );

	auto oit = sn->getObjectIterator();
	while( oit.hasMoreElements() )
	{
		auto store_obj = oit.getNextCRef();
		Ogre::MovableObject *attach_obj(0);
		switch(store_obj.second)
		{
		case dsMovableObject::OT_ENTITY:
			attach_obj = createEntity_collection(store_obj.first);
		case dsMovableObject::OT_CAMERA:
			attach_obj = createCamera_collection(store_obj.first);
		case dsMovableObject::OT_LIGHT:
			attach_obj = createLight_collection(store_obj.first);
		}
		if(attach_obj)
			osn->attachObject(attach_obj);
	}

	auto snit = sn->getSceneNodeIterator();
	while( snit.hasMoreElements() )
	{

		Ogre::SceneNode* childSN = createSceneNode( snit.getNextCRef() );
		if( childSN ) osn->addChild( childSN );
	}

	return osn;
}
//------------------------------------------------------------
/*virtual*/ Ogre::Entity* DefaultSceneFactory::createEntity(System::dsEntity::const_shared_ptr ent)
{
	Ogre::Entity* oent = mpSceneManager->createEntity(_generateObjName(ent->name), ent->meshFilename );

	//adjust entity lods
	oent->setMeshLodBias(ent->meshLodBias.factor, ent->meshLodBias.max, ent->meshLodBias.min);
	oent->setMaterialLodBias(ent->materialLodBias.factor, ent->materialLodBias.max, ent->materialLodBias.min);

	unsigned int subEntitiesCount = oent->getNumSubEntities();
	for( unsigned int i = 0; i < subEntitiesCount; ++i )
	{
		Ogre::SubEntity* subEntity = oent->getSubEntity( i );
		if( !ent->subMatNames[ i ].empty() )
			subEntity->setMaterialName( ent->subMatNames[ i ] );
	}
	return oent;
}
//------------------------------------------------------------
/*virtual*/ Ogre::Light* DefaultSceneFactory::createLight_default(string_traits::in namePrefix)
{
	return mpSceneManager->createLight(_generateObjName(namePrefix));
}
//------------------------------------------------------------
/*virtual*/ Ogre::Light* DefaultSceneFactory::createLight(System::dsLight::const_shared_ptr light)
{
	Ogre::Light* olit = mpSceneManager->createLight(_generateObjName(light->name));
	olit->setType( light->lightType );
	olit->setCastShadows( light->castShadows );
	olit->setPowerScale( light->power );
	olit->setDiffuseColour( light->diffuseColour );
	olit->setSpecularColour( light->specularColour );
	return olit;
}
//------------------------------------------------------------
/*virtual*/ Ogre::Camera* DefaultSceneFactory::createCamera_default(string_traits::in namePrefix)
{
	return mpSceneManager->createCamera(_generateObjName(namePrefix));
}
//------------------------------------------------------------
/*virtual*/ Ogre::Camera* DefaultSceneFactory::createCamera(System::dsCamera::const_shared_ptr cam)
{
	Ogre::Camera* ocam = mpSceneManager->createCamera(_generateObjName(cam->name));
	ocam->setFOVy( Ogre::Radian( cam->fov ) );
	ocam->setAspectRatio( cam->aspectRatio );
	ocam->setProjectionType( cam->projectionType );
	ocam->setNearClipDistance( cam->nearClip );
	ocam->setFarClipDistance( cam->farClip );
	ocam->setPosition( cam->position );
	ocam->setOrientation( cam->orientation );
	ocam->setDirection( cam->direction );
	return ocam;
}
//------------------------------------------------------------
/*virtual*/ Ogre::Entity* DefaultSceneFactory::cloneEntity(const Ogre::Entity *pEnt, string_traits::in namePrefix)
{
	return pEnt->clone(_generateObjName(namePrefix), mpSceneManager);
}
//------------------------------------------------------------
#if DBS_LM_COLLECTIONS
//------------------------------------------------------------
/*virtual*/ Ogre::Camera* DefaultSceneFactory::createCamera_collection(string_traits::in name)
{
	dsCamera::const_shared_ptr cam = _retrieveCamera_collection(name);
	if(cam)
		return createCamera(cam);
}
//------------------------------------------------------------
/*virtual*/ Ogre::Light* DefaultSceneFactory::createLight_collection(string_traits::in name)
{
	dsLight::const_shared_ptr light = _retrieveLight_collection(name);
	if(light)
		return createLight(light);
}
//------------------------------------------------------------
/*virtual*/ Ogre::Entity* DefaultSceneFactory::createEntity_collection(string_traits::in name)
{
	dsEntity::const_shared_ptr ent = _retrieveEntity_collection(name);
	if(ent)
	{
		Ogre::MeshManager &mm = Ogre::MeshManager::getSingleton();
		return createEntity(ent);
	}
}
//------------------------------------------------------------
/*virtual*/ Ogre::SceneNode* DefaultSceneFactory::createSceneNode_collection(string_traits::in name)
{
	dsSceneNode::const_shared_ptr node = _retrieveSceneNode_collection(name);
	if(node)
		return createSceneNode(node);
}
//------------------------------------------------------------
#endif
//------------------------------------------------------------
dsSceneNode::const_shared_ptr DefaultSceneFactory::_retrieveSceneNode_collection(string_traits::in name)const
{
	for(auto it = mSceneCollCont.begin(), eit = mSceneCollCont.end(); it != eit; ++it)
	{
		dsSceneNode::const_shared_ptr sn = (*it)->getSceneNode(name);
		if(sn)
			return sn;
	}
	return dsSceneNode::const_shared_ptr();
}
//------------------------------------------------------------
dsEntity::const_shared_ptr DefaultSceneFactory::_retrieveEntity_collection(string_traits::in name)const
{
	for(auto it = mSceneCollCont.begin(), eit = mSceneCollCont.end(); it != eit; ++it)
	{
		dsEntity::const_shared_ptr ent = (*it)->getEntity(name);
		if(ent)
			return ent;
	}
	return dsEntity::const_shared_ptr();
}
//------------------------------------------------------------
dsLight::const_shared_ptr DefaultSceneFactory::_retrieveLight_collection(string_traits::in name)const
{
	for(auto it = mSceneCollCont.begin(), eit = mSceneCollCont.end(); it != eit; ++it)
	{
		dsLight::const_shared_ptr light = (*it)->getLight(name);
		if(light)
			return light;
	}
	return dsLight::const_shared_ptr();
}
//------------------------------------------------------------
dsCamera::const_shared_ptr DefaultSceneFactory::_retrieveCamera_collection(string_traits::in name)const
{
	for(auto it = mSceneCollCont.begin(), eit = mSceneCollCont.end(); it != eit; ++it)
	{
		dsCamera::const_shared_ptr cam = (*it)->getCamera(name);
		if(cam)
			return cam;
	}
	return dsCamera::const_shared_ptr();
}
//------------------------------------------------------------
dsMeshLod::const_shared_ptr DefaultSceneFactory::_retrieveMeshLod_collection(string_traits::in name)const
{
	for(auto it = mSceneCollCont.begin(), eit = mSceneCollCont.end(); it != eit; ++it)
	{
		dsMeshLod::const_shared_ptr ml = (*it)->getMeshLod(name);
		if(ml)
			return ml;
	}
	return dsMeshLod::const_shared_ptr();
}
//------------------------------------------------------------
void DefaultSceneFactory::_checkMeshLods(string_traits::in name)
{
	Ogre::MeshManager &mm = Ogre::MeshManager::getSingleton();
	Ogre::MeshPtr mesh = mm.getByName(name);
	dsMeshLod::const_shared_ptr ml = _retrieveMeshLod_collection(name);
	if(!ml || !mesh) return;

	if(ml->meshResourceState == 0 || ml->meshResourceState != mesh->getStateCount())
	{
		mesh->removeLodLevels();
		Ogre::LodStrategy* ls = Ogre::LodStrategyManager::getSingleton().getStrategy(ml->lodStrategy);
		if(ls)
			mesh->setLodStrategy(ls);
		for(auto i = ml->lods.begin(), ei = ml->lods.end(); i != ei; ++i)
		{
			mesh->createManualLodLevel(Ogre::Real(i->first), i->second.name, i->second.group);
		}
		mesh->_dirtyState();
		ml->meshResourceState = mesh->getStateCount();
	}
}
//------------------------------------------------------------
}
}
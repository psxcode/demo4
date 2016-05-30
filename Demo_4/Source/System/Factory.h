#pragma once

#include "Interface/FactoryInterface.h"

#if DBS_LM_COLLECTIONS
#include "SceneCollectionInterface.h"
#endif

#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
#include "VehicleCollectionInterface.h"
#endif

#if DBS_LM_NXU_COLLECTIONS
#	include "NxuCollectionInterface.h"
#endif

namespace Demo {
namespace System {
//------------------------------------------------------------
class DefaultSceneFactory : public SceneFactoryInterface
{
public:
	DEFINE_CLASS_SHARED_PTR(DefaultSceneFactory)
	//--------------------------------//
	class traits
	{
	public:
#if DBS_LM_COLLECTIONS
		typedef type_traits< SceneCollection_FactoryInterface::shared_ptr >::cref scene_collection_in;
		typedef Demo::set< SceneCollection_FactoryInterface::shared_ptr >::type scene_collection_container;
#endif
	};
	//--------------------------------//
	explicit DefaultSceneFactory(const InitData &idata)
		: mpSceneManager(idata.pSceneManager)
		, mNodeNamePrefix(idata.nodeNamePrefix)
		, mObjNamePrefix(idata.objNamePrefix)
	{
		if(mNodeNamePrefix.empty()) mNodeNamePrefix = "_";
		if(mObjNamePrefix.empty()) mObjNamePrefix = "_";
	}
	virtual ~DefaultSceneFactory() {}

#if DBS_LM_COLLECTIONS
	//The returned SceneNode already has all childObjects (entities, lights) attached
	virtual Ogre::SceneNode* createSceneNode_collection(string_traits::in name);
	virtual Ogre::Entity* createEntity_collection(string_traits::in name);
	virtual Ogre::Light* createLight_collection(string_traits::in name);
	virtual Ogre::Camera* createCamera_collection(string_traits::in name);
#endif

	virtual Ogre::SceneNode* createSceneNode_default(string_traits::in namePrefix);
	virtual Ogre::SceneNode* createSceneNode(System::dsSceneNode::const_shared_ptr dsn);
	virtual Ogre::Entity* createEntity(System::dsEntity::const_shared_ptr ent);
	virtual Ogre::Light* createLight_default(string_traits::in namePrefix);
	virtual Ogre::Light* createLight(System::dsLight::const_shared_ptr light);
	virtual Ogre::Camera* createCamera_default(string_traits::in namePrefix);
	virtual Ogre::Camera* createCamera(System::dsCamera::const_shared_ptr cam);

	virtual Ogre::Entity* cloneEntity(const Ogre::Entity *pEnt, string_traits::in namePrefix);

#if DBS_LM_COLLECTIONS
	void addCollection( traits::scene_collection_in coll ) {mSceneCollCont.insert(coll);}
	void removeCollection( traits::scene_collection_in coll ) {mSceneCollCont.erase(coll);}
#endif

protected:
	dsEntity::const_shared_ptr _retrieveEntity_collection(string_traits::in name)const;
	dsLight::const_shared_ptr _retrieveLight_collection(string_traits::in name)const;
	dsCamera::const_shared_ptr _retrieveCamera_collection(string_traits::in name)const;
	dsSceneNode::const_shared_ptr _retrieveSceneNode_collection(string_traits::in name)const;
	dsMeshLod::const_shared_ptr _retrieveMeshLod_collection(string_traits::in name)const;
	void _checkMeshLods(string_traits::in name);
	string_traits::str _generateNodeName(string_traits::in prefix)const
	{
		Ogre::StringStream ss;
		ss << prefix << mNodeNamePrefix << msNodeCounter++;
		return ss.str();
	}
	string_traits::str _generateObjName(string_traits::in prefix)const
	{
		Ogre::StringStream ss;
		ss << prefix << mObjNamePrefix << msObjCounter++;
		return ss.str();
	}
#if DBS_LM_COLLECTIONS
	traits::scene_collection_container mSceneCollCont;
#endif
	Ogre::SceneManager *mpSceneManager;
	string_traits::str mNodeNamePrefix;
	string_traits::str mObjNamePrefix;

	static Ogre::uint32 msNodeCounter;
	static Ogre::uint32 msObjCounter;
};
//------------------------------------------------------------
#if DBS_LM_ABSTRACT_VEHICLE
class DefaultVehicleFactory : public VehicleFactoryInterface
{
public:
	DEFINE_CLASS_SHARED_PTR(DefaultVehicleFactory)
	//--------------------------------//
	class traits
	{
	public:
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
		typedef type_traits< VehicleCollection_FactoryInterface::shared_ptr >::cref vehicle_collection_in;
		typedef Demo::set< VehicleCollection_FactoryInterface::shared_ptr >::type vehicle_collection_container;
#endif
	};
	//--------------------------------//
	DefaultVehicleFactory()
		: mpSceneManager(0)
	{
	}
	explicit DefaultSceneFactory(const InitData &idata)
		: mpSceneManager(idata.scene)
	{
	}
	virtual ~DefaultVehicleFactory() {}
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
	void addCollection(traits::vehicle_collection_in coll) {mVehicleCollCont.insert(coll);}
	void removeCollection(traits::vehicle_collection_in coll) {mVehicleCollCont.erase(coll);}
#endif

	void setSceneManager( Ogre::SceneManager *osm ) {mpSceneManager = osm;}

protected:
	Ogre::SceneManager *mpSceneManager;
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
	traits::vehicle_collection_container mVehicleCollCont;
#endif
};
#endif
//------------------------------------------------------------
#if DBS_LM_PHYSX
class DefaultPhysXFactory : public PhysXFactoryInterface
{
public:
	DEFINE_CLASS_SHARED_PTR(DefaultPhysXFactory)
	//--------------------------------//
	class traits
	{
	public:
#if DBS_LM_NXU_COLLECTIONS
		typedef type_traits< NxuCollection_FactoryInterface::shared_ptr >::cref nxu_collection_in;
		typedef Demo::set< NxuCollection_FactoryInterface::shared_ptr >::type nxu_collection_container;
#endif
	};
	//--------------------------------//
	DefaultPhysXFactory()
		: mpScene(0)
	{
	}
	explicit DefaultPhysXFactory(const InitData &idata)
		: mpScene(idata.scene)
	{
	}
	virtual ~DefaultPhysXFactory() {}
#if DBS_LM_NXU_COLLECTIONS
	void addCollection( traits::nxu_collection_in coll ) {mNxuCollCont.insert(coll);}
	void removeCollection( traits::nxu_collection_in coll ) {mNxuCollCont.erase(coll);}
#endif

	void setScene( NxScene *scn ) {mpScene = scn;}

protected:
#if DBS_LM_NXU_COLLECTIONS
	traits::nxu_collection_container mNxuCollCont;
#endif
	NxScene* mpScene;
};
#endif //DBS_LM_PHYSX
//------------------------------------------------------------
}
}
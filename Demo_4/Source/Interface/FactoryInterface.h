#pragma once

#include <OgreSceneManager.h>
#include "System/dsSceneTypes.h"

#if DBS_LM_PHYSX
#	include <NxActor.h>
#	include <NxScene.h>
#	include <NxPlaneShapeDesc.h>
#	include <NxBoxShapeDesc.h>
#	include <NxSphereShapeDesc.h>
#	include <NxCapsuleShapeDesc.h>
#	include "Interface/OgrePhysXConverter.h"
#endif

namespace Demo
{
//------------------------------------------------------------
class SceneFactoryInterface
{
public:
	DEFINE_CLASS_SHARED_PTR(SceneFactoryInterface)
	//--------------------------------//
	struct InitData
	{
		InitData() : pSceneManager(0) {}
		explicit InitData(Ogre::SceneManager *osm) : pSceneManager(osm) {}
		bool isValid()const {return pSceneManager != 0;}
		Ogre::SceneManager *pSceneManager;
		string_traits::str nodeNamePrefix;
		string_traits::str objNamePrefix;
	};
	//--------------------------------//
	static SceneFactoryInterface::shared_ptr getInstance(const InitData &idata);
	static SceneFactoryInterface::shared_ptr getInstance(Ogre::SceneManager*);
	static void destroySceneNodeAndObjects(Ogre::SceneNode *&node);
	//--------------------------------//
	virtual ~SceneFactoryInterface() {}

#if DBS_LM_COLLECTIONS
	//The returned SceneNode already has all childObjects (entities, lights) attached
	virtual Ogre::SceneNode* createSceneNode_collection(string_traits::in name) = 0;
	virtual Ogre::Entity* createEntity_collection(string_traits::in name) = 0;
	virtual Ogre::Light* createLight_collection(string_traits::in name) = 0;
	virtual Ogre::Camera* createCamera_collection(string_traits::in name) = 0;
#endif
	
	virtual Ogre::SceneNode* createSceneNode_default(string_traits::in namePrefix) = 0;
	virtual Ogre::SceneNode* createSceneNode(System::dsSceneNode::const_shared_ptr dsn) = 0;
	virtual Ogre::Entity* createEntity(System::dsEntity::const_shared_ptr ent) = 0;
	virtual Ogre::Light* createLight_default(string_traits::in namePrefix) = 0;
	virtual Ogre::Light* createLight(System::dsLight::const_shared_ptr light) = 0;
	virtual Ogre::Camera* createCamera_default(string_traits::in namePrefix) = 0;
	virtual Ogre::Camera* createCamera(System::dsCamera::const_shared_ptr cam) = 0;

	virtual Ogre::Entity* cloneEntity(const Ogre::Entity *pEnt, string_traits::in namePrefix) = 0;
};
//------------------------------------------------------------
#if DBS_LM_ABSTRACT_VEHICLE
class VehicleFactoryInterface
{
public:
	DEFINE_CLASS_SHARED_PTR(VehicleFactoryInterface)
	//--------------------------------//
	struct InitData
	{
		InitData() : scene(0) {}
		explicit InitData(Ogre::SceneManager *osm) : scene(osm) {}
		bool isValid()const {return scene != 0;}
		Ogre::SceneManager *scene;
	};
	//--------------------------------//
	static VehicleFactoryInterface::shared_ptr getInstance(const InitData &idata);
	//--------------------------------//
	virtual ~VehicleFactoryInterface() {}

#if DBS_LM_PHYSX_VEHICLE
	//virtual void createVehicle( string_traits::in vehicleSetName ) = 0;
#endif
};
#endif //DBS_LM_ABSTRACT_VEHICLE
//------------------------------------------------------------
#if DBS_LM_PHYSX
class OgrePhysXActor
{
public:
	OgrePhysXActor()
		: osn(0)
		, actor(0)
	{
	}
	inline void sync() {
		osn->setPosition(OgrePhysXConverter::toOgre(actor->getGlobalPosition()));
		osn->setOrientation(OgrePhysXConverter::toOgre(actor->getGlobalOrientation()));
	}

	Ogre::SceneNode* osn;
	NxActor* actor;
};
//------------------------------------------------------------
class PhysXFactoryInterface
{
public:
	DEFINE_CLASS_SHARED_PTR(PhysXFactoryInterface)
	//--------------------------------//
	struct InitData
	{
		InitData() : scene(0) {}
		explicit InitData(NxScene *scn) : scene(scn) {}
		bool isValid()const {return scene != 0;}
		NxScene *scene;
	};
	//--------------------------------//
	static PhysXFactoryInterface::shared_ptr getInstance(const InitData &idata);
	static NxScene* createScene( const NxSceneDesc &desc );
	static void releaseScene( NxScene *&scene );
	static void createPlaneShapeDesc( NxPlaneShapeDesc &desc, const Ogre::Vector3 &normal, Ogre::Real d ) {
		desc.setToDefault();
		desc.mass = 0;
		desc.density = 0;
		desc.normal = OgrePhysXConverter::toNx(normal);
		desc.d = d;
	}
	static void createBoxShapeDesc( NxBoxShapeDesc &desc, const Ogre::Vector3 &dims ) {
		desc.setToDefault();
		desc.dimensions.set(dims.x * 0.5f, dims.y * 0.5f, dims.z * 0.5f);
	}
	static void createBoxShapeDesc( NxBoxShapeDesc &desc, const Ogre::Entity *ent, const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE )	{
		Ogre::Vector3 size = ent->getBoundingBox().getSize() * scale;
		createBoxShapeDesc(desc, size);
	}
	static void createSphereShapeDesc( NxSphereShapeDesc &desc, Ogre::Real radius )	{
		desc.setToDefault();
		desc.radius = radius;
	}
	static void createSphereShapeDesc( NxSphereShapeDesc &desc, const Ogre::Entity *ent, Ogre::Real scale = 1.0f ) {
		Ogre::Real radius = ent->getBoundingRadius() * scale;
		createSphereShapeDesc(desc, radius);
	}
	static void createCapsuleShapeDesc( NxCapsuleShapeDesc &desc, Ogre::Real radius, Ogre::Real height ) {
		desc.setToDefault();
		desc.radius = radius;
		desc.height = height;
	}
	//--------------------------------//
	virtual ~PhysXFactoryInterface() {}
};
#endif //DBS_LM_PHYSX
//------------------------------------------------------------
}
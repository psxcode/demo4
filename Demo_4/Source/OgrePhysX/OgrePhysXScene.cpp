#include "OgrePhysXScene.h"
#if DBS_LM_PHYSX

#include "System/PhysXController.h"
#include "OgrePhysXWorld.h"
#include "OgrePhysXRagdoll.h"
#include "OgrePhysXContactReport.h"
#include "OgrePhysXTriggerReport.h"

namespace Demo
{
namespace OgrePhysX
{
//------------------------------------------------------------
Scene::Scene(void)
{
	NxSceneDesc desc;
	desc.gravity = NxVec3( NxReal(0), NxReal(-9.81), NxReal(0) );
	desc.simType = NX_SIMULATION_SW;
	create(desc);
}
//------------------------------------------------------------
Scene::Scene(const NxSceneDesc &desc)
{
	create(desc);
}
//------------------------------------------------------------
Scene::~Scene(void)
{
	sceneClosingDelComposite();
	if (!World::getSingleton().getSDK())
		World::getSingleton().getSDK()->releaseScene(*mNxScene);
}
//------------------------------------------------------------
void Scene::create(const NxSceneDesc &desc)
{
	if (!World::getSingleton().getSDK())
	{
#if DBS_LOGGING_ENABLED
		Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: Cannot create scene because World is not initialised properly.");
#endif
		return;
	}
	mNxScene = World::getSingleton().getSDK()->createScene(desc);

	NxMaterial* defaultMaterial = mNxScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(NxReal(0.1));
	defaultMaterial->setStaticFriction(NxReal(0.5));
	defaultMaterial->setDynamicFriction(NxReal(0.5));
}
//------------------------------------------------------------
void Scene::bindMaterial(string_traits::in matName, NxMaterialIndex physXMat, bool replace)
{
	if(replace) mMaterialBindings.erase( matName );
	mMaterialBindings.insert( std::make_pair(matName, physXMat) );
}
//------------------------------------------------------------
Actor* Scene::createActor(PrimitiveShape& shape, const Ogre::Vector3 &position, const Ogre::Quaternion &orientation)
{
	Actor *actor = new Actor(mNxScene, shape, position, orientation);
	mActors.push_back(actor);
	return actor;
}
//------------------------------------------------------------
RenderedActor* Scene::createRenderedActor(PointRenderable *pointRenderable, PrimitiveShape& shape, const Ogre::Vector3 &position, const Ogre::Quaternion &orientation)
{
	RenderedActor *actor = new RenderedActor(mNxScene, pointRenderable, shape, position, orientation);
	mOgrePhysXBindings.push_back(actor);
	mActors.push_back(actor);
	return actor;
}
//------------------------------------------------------------
Actor* Scene::createActor(BaseMeshShape& shape, const Ogre::Vector3 &position, const Ogre::Quaternion &orientation)
{
	Actor *actor = new Actor(mNxScene, shape, position, orientation);
	mActors.push_back(actor);
	return actor;
}
//------------------------------------------------------------
RenderedActor* Scene::createRenderedActor(PointRenderable *pointRenderable, BaseMeshShape& shape, const Ogre::Vector3 &position, const Ogre::Quaternion &orientation)
{
	RenderedActor *actor = new RenderedActor(mNxScene, pointRenderable, shape, position, orientation);
	mOgrePhysXBindings.push_back(actor);
	mActors.push_back(actor);
	return actor;
}
//------------------------------------------------------------
void Scene::releaseActor(Actor *&actor)
{
	mActors.remove(actor);
	RenderableBinding *bind = dynamic_cast<RenderableBinding*>(actor);
	if (bind) mOgrePhysXBindings.remove(bind);
	delete actor;
	actor = 0;
}
//------------------------------------------------------------
void Scene::releaseRenderedActor(RenderedActor *&actor)
{
	mActors.remove(actor);
	mOgrePhysXBindings.remove(actor);
	delete actor;
	actor = 0;
}
//------------------------------------------------------------
Ragdoll* Scene::createRagdoll(Ogre::Entity *ent, Ogre::SceneNode *node)
{
	Ragdoll *rag = new Ragdoll(getNxScene(), ent, node);
	mOgrePhysXBindings.push_back(rag);
	return rag;
}
//------------------------------------------------------------
void Scene::destroyRagdoll(Ragdoll *&rag)
{
	mOgrePhysXBindings.remove(rag);
	delete rag;
	rag = 0;
}
//------------------------------------------------------------
void Scene::capsuleSweep(QueryHit::Container &result, const Ogre::Vector3 &origin, const Ogre::Vector3 &direction, Ogre::Real radius, Ogre::Real distance, int maxNumResult, NxU32 flags, NxU32 activeGroups, const NxGroupsMask* groupsMask)
{
	NxCapsule capsule;
	capsule.radius	= radius;
	capsule.setOriginDirection(Convert::toNx(origin), Convert::toNx(direction));
	    
	NxSweepQueryHit *sqh_result = new NxSweepQueryHit[maxNumResult];
	    
	NxU32 numHits = mNxScene->linearCapsuleSweep(capsule,
		Convert::toNx(direction*distance),
		flags,
		0,
		maxNumResult,
		sqh_result,
		0,
		activeGroups,
		groupsMask);

	for (NxU32 i = 0; i < numHits; i++)
	{
		NxSweepQueryHit hit = sqh_result[i];
		if (hit.hitShape->getActor().userData)
		{
			QueryHit sHit;
			sHit.distance = hit.t;
			sHit.hitActor = (Actor*)hit.hitShape->getActor().userData;
			sHit.normal = Convert::toOgre(hit.normal);
			sHit.point = Convert::toOgre(hit.point);
			result.push_back(sHit);
		}
	}	
}
//------------------------------------------------------------
void Scene::boxSweep(QueryHit::Container &result, const Ogre::Vector3 &origin, const Ogre::Vector3 &direction, BoxShape &shape, Ogre::Real distance, int maxNumResult, NxU32 flags, NxU32 activeGroups, const NxGroupsMask* groupsMask)
{
	NxBox box;
	NxBoxShapeDesc *desc = (NxBoxShapeDesc*)shape.getDesc();
	box.center = Convert::toNx(origin);
	box.extents = desc->dimensions;
	    
	NxSweepQueryHit *sqh_result = new NxSweepQueryHit[maxNumResult];
	    
	NxU32 numHits = mNxScene->linearOBBSweep(box,
		Convert::toNx(direction*distance),
		flags,
		0,
		maxNumResult,
		sqh_result,
		0,
		activeGroups,
		groupsMask);

	for (NxU32 i = 0; i < numHits; i++)
	{
		NxSweepQueryHit hit = sqh_result[i];
		if (hit.hitShape->getActor().userData)
		{
			QueryHit sHit;
			sHit.distance = hit.t;
			sHit.hitActor = (Actor*)hit.hitShape->getActor().userData;
			sHit.normal = Convert::toOgre(hit.normal);
			sHit.point = Convert::toOgre(hit.point);
			result.push_back(sHit);
		}
	}	
}
//------------------------------------------------------------
NxForceField* Scene::createForceField(BoxShape &shape, NxForceFieldDesc &fieldDesc, NxForceFieldLinearKernelDesc &linearKernelDesc)
{
	NxForceFieldLinearKernel* linearKernel = mNxScene->createForceFieldLinearKernel(linearKernelDesc);
	fieldDesc.kernel = linearKernel;
	NxForceField *forceField = mNxScene->createForceField(fieldDesc);

	NxBoxForceFieldShapeDesc boxDesc;
	NxBoxShapeDesc *desc = (NxBoxShapeDesc*)shape.getDesc();
	boxDesc.dimensions = desc->dimensions;
	boxDesc.pose = desc->localPose;
	forceField->getIncludeShapeGroup().createShape(boxDesc);

	return forceField;
}
//------------------------------------------------------------
NxForceField* Scene::createForceField(SphereShape &shape, NxForceFieldDesc &fieldDesc, NxForceFieldLinearKernelDesc &linearKernelDesc)
{
	NxForceFieldLinearKernel* linearKernel = mNxScene->createForceFieldLinearKernel(linearKernelDesc);
	fieldDesc.kernel = linearKernel;
	NxForceField *forceField = mNxScene->createForceField(fieldDesc);

	NxSphereForceFieldShapeDesc sphereDesc;
	NxSphereShapeDesc *desc = (NxSphereShapeDesc*)shape.getDesc();
	sphereDesc.radius = desc->radius;
	sphereDesc.pose = desc->localPose;
	forceField->getIncludeShapeGroup().createShape(sphereDesc);

	return forceField;
}
//------------------------------------------------------------
NxForceField* Scene::createForceField(CapsuleShape &shape, NxForceFieldDesc &fieldDesc, NxForceFieldLinearKernelDesc &linearKernelDesc)
{
	NxForceFieldLinearKernel* linearKernel = mNxScene->createForceFieldLinearKernel(linearKernelDesc);
	fieldDesc.kernel = linearKernel;
	NxForceField *forceField = mNxScene->createForceField(fieldDesc);

	NxCapsuleForceFieldShapeDesc capsuleDesc;
	NxCapsuleShapeDesc *desc = (NxCapsuleShapeDesc*)shape.getDesc();
	capsuleDesc.radius = desc->radius;
	capsuleDesc.height = desc->height;
	capsuleDesc.pose = desc->localPose;
	forceField->getIncludeShapeGroup().createShape(capsuleDesc);

	return forceField;
}
//------------------------------------------------------------
void Scene::destroyForcefield(NxForceField *&forceField)
{
	NxForceFieldLinearKernel *kernel = (NxForceFieldLinearKernel*)forceField->getForceFieldKernel();
	mNxScene->releaseForceField(*forceField);
	mNxScene->releaseForceFieldLinearKernel(*kernel);
	forceField = 0;
}
//------------------------------------------------------------
}
}
#endif
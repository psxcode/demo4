#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <NxScene.h>
#include <OgreRay.h>
#include "OgrePhysXRenderableBinding.h"
#include "OgrePhysXRenderedActor.h"
#include "OgrePhysXContactReport.h"
#include "OgrePhysXTriggerReport.h"

#include "delegate_traits.h"

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
class Scene
{
	friend class World;
public:
	//--------------------------------//
	struct QueryHit
	{
		typedef Demo::vector<QueryHit>::type Container;
		Actor *hitActor;
		Ogre::Real distance;
		Ogre::Vector3 point;
		Ogre::Vector3 normal;
	};
	//--------------------------------//
	class RaycastReport : public NxUserRaycastReport
	{
	public:
		RaycastReport(QueryHit::Container &report)
			: mReport(report)
		{
		}
		virtual ~RaycastReport() {}
		virtual bool onHit(const NxRaycastHit& hit) {
			QueryHit sHit;
			sHit.distance = hit.distance;
			sHit.normal = Convert::toOgre(hit.worldNormal);
			sHit.point = Convert::toOgre(hit.worldImpact);
			if (hit.shape->getActor().userData)
				sHit.hitActor = (Actor*)hit.shape->getActor().userData;
			else sHit.hitActor = 0;
			mReport.push_back(sHit);
			return true;
		}

		QueryHit::Container mReport;
	};
	//--------------------------------//
	class traits
	{
	public:
		typedef Demo::list<RenderableBinding*>::type renderable_binding_cont;
		typedef Demo::list<Actor*>::type actor_cont;
		typedef Demo::map<string_traits::str, NxMaterialIndex>::type material_binding_cont;
		typedef delegate_traits::composite0 del_composite;
		typedef del_composite::traits::del_in del_in;
	};
	//--------------------------------//
	void addSceneClosingDel( traits::del_in del ) { sceneClosingDelComposite.add(del); }
	void removeSceneClosingDel( traits::del_in del ) { sceneClosingDelComposite.remove(del); }

	inline NxScene* getNxScene() { return mNxScene; }

	/*
	bindMaterial
	Binds an ogre material to a PhysX material.
	By default, all ogre materials are bound to the physX default material (0).
	Setup your material binds BEFORE you cook meshes.
	*/
	void bindMaterial(string_traits::in matName, NxMaterialIndex physXMat, bool replace = true);

	/*
	getMaterialBinding
	returns the material index assigned to the Ogre material.
	returns 0 by default.
	*/
	inline NxMaterialIndex getMaterialBinding(string_traits::in matName) {
		traits::material_binding_cont::iterator it( mMaterialBindings.find(matName) );
		if( it != mMaterialBindings.end() )	return it->second;
		return 0;
	}

	/*
	getMaterialBindings
	return all material bindings.
	*/
	traits::material_binding_cont& getMaterialBindings() { return mMaterialBindings; }

	/*
	Creates an actor
	*/
	Actor* createActor(PrimitiveShape& shape, const Ogre::Vector3 &position = Ogre::Vector3(0,0,0), const Ogre::Quaternion &orientation = Ogre::Quaternion());
	Actor* createActor(BaseMeshShape& shape, const Ogre::Vector3 &position = Ogre::Vector3(0,0,0), const Ogre::Quaternion &orientation = Ogre::Quaternion());
	void releaseActor(Actor *&actor);

	/*
	Creates an actor and binds a PointRenderable to it.
	Example usage:
	Ogre::SceneNode *node = ...
	Ogre::Entity *ent = ...
	node->attachObject(ent);
	mScene->createRenderedActor(new NodeRenderable(node), BoxShape(ent).density(10));
	*/
	RenderedActor* createRenderedActor(PointRenderable *PointRenderable, PrimitiveShape& shape, const Ogre::Vector3 &position = Ogre::Vector3(0,0,0), const Ogre::Quaternion &orientation = Ogre::Quaternion());
	RenderedActor* createRenderedActor(PointRenderable *PointRenderable, BaseMeshShape& shape, const Ogre::Vector3 &position = Ogre::Vector3(0,0,0), const Ogre::Quaternion &orientation = Ogre::Quaternion());
	void releaseRenderedActor(RenderedActor *&actor);

	inline void setContactReport(ContactReportListener *crl) { mNxScene->setUserContactReport(new ContactReport(crl)); }
	inline void setTriggerReport(TriggerReportListener *trl) { mNxScene->setUserTriggerReport(new TriggerReport(trl)); }

	/*
	Experimental. Do not use this!
	*/
	Ragdoll* createRagdoll(Ogre::Entity *ent, Ogre::SceneNode *node);
	void destroyRagdoll(Ragdoll *&rag);

	//Raycasting - this is only basic wrapping, if you want full control use PhysX directly!
	inline bool raycastAnyBounds(const Ogre::Ray &ray, NxShapesType shapeTypes = NX_ALL_SHAPES, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask *groupsMask=0) {
		NxRay nRay(Convert::toNx(ray.getOrigin()), Convert::toNx(ray.getDirection()));
		return mNxScene->raycastAnyBounds(nRay, shapeTypes, groups, maxDist, groupsMask);
	}
	inline bool raycastAnyShape(const Ogre::Ray &ray, NxShapesType shapeTypes = NX_ALL_SHAPES, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask *groupsMask=0) {
		NxRay nRay(Convert::toNx(ray.getOrigin()), Convert::toNx(ray.getDirection()));
		return mNxScene->raycastAnyShape(nRay, shapeTypes, groups, maxDist, groupsMask);
	}
	inline bool raycastClosestBounds(QueryHit &result, const Ogre::Ray &ray, NxShapesType shapeTypes = NX_ALL_SHAPES, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask *groupsMask=0) {
		NxRay nRay(Convert::toNx(ray.getOrigin()), Convert::toNx(ray.getDirection()));
		NxRaycastHit hit;
		result.hitActor = 0;
		NxShape *shape = mNxScene->raycastClosestBounds(nRay, shapeTypes, hit, groups, maxDist, hintFlags, groupsMask);
		if (shape) {
			if (shape->getActor().userData)
				result.hitActor = (Actor*)shape->getActor().userData;
			result.distance = hit.distance;
			result.normal = Convert::toOgre(hit.worldNormal);
			result.point = Convert::toOgre(hit.worldImpact);
			return true;
		}
		return false;
	}
	inline bool raycastClosestShape(QueryHit &result, const Ogre::Ray &ray, NxShapesType shapeTypes = NX_ALL_SHAPES, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask *groupsMask=0) {
		NxRay nRay(Convert::toNx(ray.getOrigin()), Convert::toNx(ray.getDirection()));
		NxRaycastHit hit;
		result.hitActor = 0;
		NxShape *shape = mNxScene->raycastClosestShape(nRay, shapeTypes, hit, groups, maxDist, hintFlags, groupsMask);
		if (shape) {
			if (shape->getActor().userData)
				result.hitActor = (Actor*)shape->getActor().userData;
			result.distance = hit.distance;
			result.normal = Convert::toOgre(hit.worldNormal);
			result.point = Convert::toOgre(hit.worldImpact);
			return true;
		}
		return false;
	}
	inline void raycastAllBounds(QueryHit::Container &result, const Ogre::Ray &ray, NxShapesType shapeTypes = NX_ALL_SHAPES, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask *groupsMask=0) {
		NxRay nRay(Convert::toNx(ray.getOrigin()), Convert::toNx(ray.getDirection()));
		RaycastReport report(result);
		mNxScene->raycastAllBounds(nRay, report, shapeTypes, groups, maxDist, hintFlags, groupsMask);
	}
	inline void raycastAllShapes(QueryHit::Container &result, const Ogre::Ray &ray, NxShapesType shapeTypes = NX_ALL_SHAPES, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask *groupsMask=0) {
		NxRay nRay(Convert::toNx(ray.getOrigin()), Convert::toNx(ray.getDirection()));
		RaycastReport report(result);
		mNxScene->raycastAllShapes(nRay, report, shapeTypes, groups, maxDist, hintFlags, groupsMask);
	}
	
	//Sweeps - untested!
	void capsuleSweep(QueryHit::Container &result, const Ogre::Vector3 &origin, const Ogre::Vector3 &direction, Ogre::Real radius, Ogre::Real distance, int maxNumResult = 20, NxU32 flags = NX_SF_STATICS|NX_SF_DYNAMICS, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=0);
	void boxSweep(QueryHit::Container &result, const Ogre::Vector3 &origin, const Ogre::Vector3 &direction, BoxShape &shape, Ogre::Real distance, int maxNumResult = 20, NxU32 flags = NX_SF_STATICS|NX_SF_DYNAMICS, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=0);

	//Forcefields - untested!
	/*
	Important note: You don't have to fill NxForceFieldDesc::kernel. This method does that for you.
	*/
	NxForceField* createForceField(BoxShape &shape, NxForceFieldDesc &fieldDesc, NxForceFieldLinearKernelDesc &linearKernelDesc);
	NxForceField* createForceField(SphereShape &shape, NxForceFieldDesc &fieldDesc, NxForceFieldLinearKernelDesc &linearKernelDesc);
	NxForceField* createForceField(CapsuleShape &shape, NxForceFieldDesc &fieldDesc, NxForceFieldLinearKernelDesc &linearKernelDesc);
	void destroyForcefield(NxForceField *&forceField);

	inline void syncRenderables() { 
		auto it = mOgrePhysXBindings.begin();
		auto eit = mOgrePhysXBindings.end();
		for( ; it != eit; ++it )
			(*it)->sync();
	}

private:
	Scene(void);
	Scene(const NxSceneDesc &desc);
	~Scene(void);

	void create(const NxSceneDesc &desc);

	NxScene *mNxScene;
	traits::renderable_binding_cont mOgrePhysXBindings;
	traits::actor_cont mActors;
	traits::material_binding_cont mMaterialBindings;
	traits::del_composite sceneClosingDelComposite;
};
//------------------------------------------------------------
}
}
#endif
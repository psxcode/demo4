#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <NxPhysics.h>
#include <OgreVector3.h>
#include "OgrePhysXClasses.h"
#include "OgrePhysXConvert.h"

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
class ContactReportListener
{
public:
	virtual ~ContactReportListener() {}
	virtual void onActorContact(Actor *actor1, Actor *actor2) {};
	/*
	This method is intended for playing 3D sounds.
	*/
	virtual void onMaterialContact(NxMaterialIndex material1, NxMaterialIndex material2, Ogre::Vector3 position, float force) {};
};
//------------------------------------------------------------
class ContactReport : public NxUserContactReport
{
public:
	ContactReport(ContactReportListener *listener)
		: mListener(listener)
	{
	}
	virtual ~ContactReport()
	{
		delete mListener;
	}
	virtual void onContactNotify(NxContactPair &pair, NxU32 events)
	{
		// Iterate through contact points
		NxContactStreamIterator i(pair.stream);
		//user can call getNumPairs() here
		while(i.goNextPair())
		{
			NxShape *shape1 = i.getShape(0);
			NxShape *shape2 = i.getShape(1);

			//First material callback
			NxMaterialIndex material1 = shape1->getMaterial();
			NxMaterialIndex material2 = shape2->getMaterial();
			if (shape1->getType() == NxShapeType::NX_SHAPE_MESH)
			{
				NxTriangleMeshShape *meshShape = (NxTriangleMeshShape*)shape1;
				material1 = meshShape->getTriangleMesh().getTriangleMaterial(i.getFeatureIndex0());
			}
			if (shape2->getType() == NxShapeType::NX_SHAPE_MESH)
			{
				NxTriangleMeshShape *meshShape = (NxTriangleMeshShape*)shape2;
				material2 = meshShape->getTriangleMesh().getTriangleMaterial(i.getFeatureIndex1());
			}
			float summed_force = 0.0f;
			Ogre::Vector3 contactPoint;
			while(i.goNextPatch())
			{
				//user can also call getPatchNormal() and getNumPoints() here
				const NxVec3& contactNormal = i.getPatchNormal();
				while(i.goNextPoint())
				{
					//user can also call getPoint() and getSeparation() here
					contactPoint = Convert::toOgre(i.getPoint());
					summed_force += i.getPointNormalForce();
				}
			}
			//The last contact point is returned, not for any particular reason, but in most cases there is only ohne contact point
			mListener->onMaterialContact(material1, material2, contactPoint, summed_force);

			//Simple actor-based callback
			if (shape1->getActor().userData && shape2->getActor().userData)
			{
				Actor *actor1 = (Actor*)shape1->getActor().userData;
				Actor *actor2 = (Actor*)shape2->getActor().userData;
				mListener->onActorContact(actor1, actor2);
			}
		}
	}

private:
	ContactReportListener *mListener;
};
//------------------------------------------------------------
}
}
#endif
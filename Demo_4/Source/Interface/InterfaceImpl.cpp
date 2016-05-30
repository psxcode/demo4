#include "DemoStableHeaders.h"

#include "FactoryInterface.h"
#include "System/MainController.h"

namespace Demo
{
//------------------------------------------------------------
/*static*/ SceneFactoryInterface::shared_ptr SceneFactoryInterface::getInstance(Ogre::SceneManager* osm)
{
	return getInstance(InitData(osm));
}
//------------------------------------------------------------
/*static*/ SceneFactoryInterface::shared_ptr SceneFactoryInterface::getInstance(const InitData &idata)
{
	return System::MainController::getSingleton().getFactoryCollectionService()->getSceneFactory(idata);
}
//------------------------------------------------------------
/*static*/ void SceneFactoryInterface::destroySceneNodeAndObjects(Ogre::SceneNode *&osn)
{
	if( osn == 0 )return;
	unsigned short objectCount = osn->numAttachedObjects();
	for( unsigned short i = 0; i < objectCount; ++i )
	{
		Ogre::MovableObject* obj = osn->detachObject(i);
		obj->_getManager()->destroyMovableObject(obj);
	}

	Ogre::Node::ChildNodeIterator iter = osn->getChildIterator();
	while( iter.hasMoreElements() )
	{
		Ogre::SceneNode* node = static_cast< Ogre::SceneNode* >( iter.getNext() );
		destroySceneNodeAndObjects(node);
	}

	osn->removeAndDestroyAllChildren();
	osn->getCreator()->destroySceneNode(osn);
	osn = 0;
}
//------------------------------------------------------------
#if DBS_LM_ABSTRACT_VEHICLE
/*static*/ VehicleFactoryInterface::shared_ptr VehicleFactoryInterface::getInstance(const InitData &idata)
{
	return System::MainController::getSingleton().getFactoryCollectionService()->getVehicleFactory(idata);
}
#endif //DBS_LM_ABSTRACT_VEHICLE
//------------------------------------------------------------ 
#if DBS_LM_PHYSX
//------------------------------------------------------------
/*static*/ PhysXFactoryInterface::shared_ptr PhysXFactoryInterface::getInstance(const InitData &idata)
{
	return System::MainController::getSingleton().getFactoryCollectionService()->getPhysXFactory(idata);
}
//------------------------------------------------------------
/*static*/ NxScene* PhysXFactoryInterface::createScene( const NxSceneDesc &desc )
{
	NxPhysicsSDK *sdk = System::MainController::getSingleton().getNxPhysicsSDK();
	NxScene *scene(0);
	if( sdk )
	{
		scene = sdk->createScene(desc);
	}
	return scene;
}
//------------------------------------------------------------
/*static*/ void PhysXFactoryInterface::releaseScene( NxScene *&scene )
{
	NxPhysicsSDK *sdk = System::MainController::getSingleton().getNxPhysicsSDK();
	if( sdk && scene )
	{
		sdk->releaseScene(*scene);
	}
	scene = 0;
}
//------------------------------------------------------------
#endif //DBS_LM_PHYSX
//------------------------------------------------------------
//------------------------------------------------------------

}
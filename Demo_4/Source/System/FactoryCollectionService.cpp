#include "DemoStableHeaders.h"
#include <OgreResourceGroupManager.h>
#include <OgreConfigFile.h>

#if DBS_LM_COLLECTIONS
#	include "DefaultSceneCollection.h"
#endif

#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
#	include "DefaultVehicleCollection.h"
#endif

#if DBS_LM_NXU_COLLECTIONS
#	include "DefaultNxuCollection.h"
#endif

#include "Factory.h"
#include "FactoryCollectionService.h"

namespace Demo {
namespace System {
//------------------------------------------------------------
string_traits::cstr DotSceneLoader::ProfileDispatcher::PROFILE_FIELD = "profile";
//------------------------------------------------------------
/*virtual*/ DotSceneLoader::ProfileDispatcher::traits::pen_cont_out DotSceneLoader::ProfileDispatcher::getProcessElementNames()const 
{
	shared_ptr_traits< traits::pen_cont >::type pnn( new traits::pen_cont );
	pnn->insert( &PROFILE_FIELD );
	return pnn;
}
//------------------------------------------------------------
/*virtual*/ bool DotSceneLoader::ProfileDispatcher::processElement( const TiXmlElement *element )
{
	return dsLoaderAuto_CompositeDispatcher::processElement(element);
}
//------------------------------------------------------------
DotSceneLoader::DotSceneLoader()
	: loaders(new dsLoaderAuto_CompositeDispatcher)
	, profileLoaders(new ProfileDispatcher)
{
	loaders->addChildLoader(profileLoaders);
	mScriptPatterns.push_back("*.scene");
}
//------------------------------------------------------------
/*virtual*/ DotSceneLoader::~DotSceneLoader()
{
}
//------------------------------------------------------------
/*virtual*/ void DotSceneLoader::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
{
	TiXmlDocument doc;
	if( !doc.LoadFile(stream) ) return;
	loaders->processElement( doc.RootElement() );
}
//------------------------------------------------------------
FactoryCollectionService::FactoryCollectionService()
{
}
//------------------------------------------------------------
FactoryCollectionService::~FactoryCollectionService()
{
#if DBS_LM_COLLECTIONS
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(mDotSceneLoader.get());
	mDotSceneLoader.reset();
	mSceneCollection.reset();
#endif

#if DBS_LM_ABSTRACT_VEHICLE
	mProfiledVehicleCollection.reset();
	mNonProfiledVehicleCollection.reset();
#endif
	
#if DBS_LM_NXU_COLLECTIONS
	Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(mNxuLoader.get());
	mNxuLoader.reset();
	mNxuCollection.reset();
#endif
}
//------------------------------------------------------------
bool FactoryCollectionService::initialize()
{
#if DBS_LM_COLLECTIONS
	mDotSceneLoader.bind( new DotSceneLoader );
	mSceneCollection.bind( new DefaultSceneCollection );
	DefaultSceneLoader::shared_ptr sceneLoader( new DefaultSceneLoader );
	sceneLoader->addCollection(mSceneCollection, SceneCollection_LoaderInterface::AO_NO_OPTIONS);
	mDotSceneLoader->addLoader(sceneLoader);

	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(mDotSceneLoader.get());
#endif


#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
	mProfiledVehicleCollection.bind( new DefaultVehicleCollection );
	mNonProfiledVehicleCollection.bind( new DefaultVehicleCollection );

	DefaultVehicleLoader::shared_ptr profiledVehicleLoader( new DefaultVehicleLoader );
	DefaultVehicleLoader::shared_ptr nonProfiledVehicleLoader( new DefaultVehicleLoader );
	profiledVehicleLoader->addCollection(mProfiledVehicleCollection, VehicleCollection_LoaderInterface::AO_NO_OPTIONS);
	nonProfiledVehicleLoader->addCollection(mNonProfiledVehicleCollection, VehicleCollection_LoaderInterface::AO_NO_OPTIONS);

	mDotSceneLoader->addProfileLoader(profiledVehicleLoader);
	mDotSceneLoader->addLoader(nonProfiledVehicleLoader);
#endif

#if DBS_LM_NXU_COLLECTIONS
	mNxuCollection.bind( new DefaultNxuCollection );
	mNxuLoader.bind( new DefaultNxuLoader );
	mNxuLoader->addCollection(mNxuCollection, NxuCollection_LoaderInterface::AO_NO_OPTIONS);
	Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(mNxuLoader.get());
#endif

	//test Prepare resources------------------------------------
	Ogre::ConfigFile cf;
    cf.load("resources.cfg");
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    string_traits::str ResGroupName, ArchType, ArchPathName;
    while( seci.hasMoreElements() )
    {
        ResGroupName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *sMap = seci.getNextRef();
		auto iter = sMap->begin();
		auto end_iter = sMap->end();
		for( ; iter != end_iter; ++iter )
		{
			ArchType = iter->first;
			ArchPathName = iter->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation( ArchPathName, ArchType, ResGroupName, false );
		}
	}
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	//test Prepare resources-----------------------------------

	return true;
}
//------------------------------------------------------------
Demo::SceneFactoryInterface::shared_ptr FactoryCollectionService::getSceneFactory(Demo::SceneFactoryInterface::InitData const &idata)
{
	if(!idata.isValid()) return DefaultSceneFactory::shared_ptr();
	DefaultSceneFactory::shared_ptr factory(new DefaultSceneFactory(idata));

#if DBS_LM_COLLECTIONS
	factory->addCollection(mSceneCollection);
#endif

	return factory;
}
//------------------------------------------------------------
#if DBS_LM_ABSTRACT_VEHICLE
Demo::VehicleFactoryInterface::shared_ptr FactoryCollectionService::getVehicleFactory(Demo::VehicleFactoryInterface::InitData const &idata)
{
	if(!idata.isValid()) return DefaultVehicleFactory::shared_ptr();
	DefaultVehicleFactory::shared_ptr factory(new DefaultVehicleFactory(idata));
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
	factory->addCollection(mProfiledVehicleCollection);
	factory->addCollection(mNonProfiledVehicleCollection);
#endif
	return factory;
}
#endif
//------------------------------------------------------------
#if DBS_LM_PHYSX
Demo::PhysXFactoryInterface::shared_ptr FactoryCollectionService::getPhysXFactory(Demo::PhysXFactoryInterface::InitData const &idata)
{
	if(!idata.isValid()) return DefaultPhysXFactory::shared_ptr();
	DefaultPhysXFactory::shared_ptr factory(new DefaultPhysXFactory(idata));

#if DBS_LM_NXU_COLLECTIONS
	factory->addCollection(mNxuCollection);
#endif

	return factory;
}
#endif
//------------------------------------------------------------
}
}
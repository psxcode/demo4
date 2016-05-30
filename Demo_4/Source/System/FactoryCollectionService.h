#pragma once

#if DBS_LM_COLLECTIONS
#	include <OgreScriptLoader.h>
#	include "SceneCollectionInterface.h"
#	include "dsLoader.h"
#endif

#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
#	include "VehicleCollectionInterface.h"
#endif

#if DBS_LM_NXU_COLLECTIONS
#	include "NxuCollectionInterface.h"
#endif

#include "Interface/FactoryInterface.h"

namespace Demo {
namespace System {
//------------------------------------------------------------
#if DBS_LM_COLLECTIONS
class DotSceneLoader : public Ogre::ScriptLoader
{
public:
	DEFINE_CLASS_SHARED_PTR(DotSceneLoader)
	//--------------------------------//
	class ProfileDispatcher : public dsLoaderAuto_CompositeDispatcher
	{
	public:
		DEFINE_CLASS_SHARED_PTR(ProfileDispatcher)
		static string_traits::cstr PROFILE_FIELD;
		virtual traits::pen_cont_out getProcessElementNames()const;
		virtual bool processElement( const TiXmlElement *element );
	};
	//--------------------------------//
	DotSceneLoader();
	virtual ~DotSceneLoader();

	//DotSceneLoader interface
	virtual const Ogre::StringVector& getScriptPatterns(void) const { return mScriptPatterns; }
	virtual Ogre::Real getLoadingOrder(void) const { return 9999.0f; }
	virtual void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);

	void addLoader( dsLoaderAuto_CompositeInterface::traits::child_ldr_in ldr ) {
		loaders->addChildLoader(ldr);
	}
	void removeLoader( dsLoaderAuto_CompositeInterface::traits::child_ldr_in ldr ) {
		loaders->removeChildLoader(ldr);
	}
	void addProfileLoader(dsLoaderAuto_CompositeInterface::traits::child_ldr_in ldr) {
		profileLoaders->addChildLoader(ldr);
	}
	void removeProfileLoader(dsLoaderAuto_CompositeInterface::traits::child_ldr_in ldr) {
		profileLoaders->removeChildLoader(ldr);
	}

protected:
	Ogre::StringVector mScriptPatterns;
	ProfileDispatcher::shared_ptr profileLoaders;
	dsLoaderAuto_CompositeDispatcher::shared_ptr loaders;
};
#endif //DBS_LM_COLLECTIONS
//------------------------------------------------------------
class FactoryCollectionService
{
public:
	DEFINE_CLASS_SHARED_PTR(FactoryCollectionService)
	//--------------------------------//
	FactoryCollectionService();
	~FactoryCollectionService();

	bool initialize();

	Demo::SceneFactoryInterface::shared_ptr getSceneFactory(Demo::SceneFactoryInterface::InitData const &idata);
	
#if DBS_LM_ABSTRACT_VEHICLE
	Demo::VehicleFactoryInterface::shared_ptr getVehicleFactory(Demo::VehicleFactoryInterface::InitData const &idata);
#endif

#if DBS_LM_PHYSX
	Demo::PhysXFactoryInterface::shared_ptr getPhysXFactory(Demo::PhysXFactoryInterface::InitData const &idata);
#endif

protected:	
#if DBS_LM_COLLECTIONS
	SceneCollection_FactoryInterface::shared_ptr mSceneCollection;
	DotSceneLoader::shared_ptr mDotSceneLoader;
#endif

#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
	VehicleCollection_FactoryInterface::shared_ptr mNonProfiledVehicleCollection;
	VehicleCollection_FactoryInterface::shared_ptr mProfiledVehicleCollection;
#endif

#if DBS_LM_NXU_COLLECTIONS
	NxuCollection_FactoryInterface::shared_ptr mNxuCollection;
	NxuLoader_CollectionInterface::shared_ptr mNxuLoader;
#endif
};
//------------------------------------------------------------
}
}
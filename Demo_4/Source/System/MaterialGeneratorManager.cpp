#include "DemoStableHeaders.h"
#if DBS_LM_DEFERRED_RENDERING

#include <OgreStringConverter.h>
#include <OgreException.h>
#include <OgreHighLevelGpuProgramManager.h>
#include "MaterialGeneratormanager.h"
#include "DeferredMaterialGenerator.h"
#include "LightMaterialGenerator.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	MaterialGeneratorManager::MaterialGeneratorManager()
	{
		initializeGenerators();
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	void MaterialGeneratorManager::initializeGenerators()
	{
		//Deferred Material Generator
		DeferredGeometryMaterialGenerator::InitData::shared_ptr dfridata( new DeferredGeometryMaterialGenerator::InitData() );
		dfridata->mgName = "DFR/";
		dfridata->mConvertSchemeName = "convert_gbuffer";
		dfridata->mCreateSchemeName = "gbuffer";
		DeferredGeometryMaterialGenerator::shared_ptr dfrMG = DeferredGeometryMaterialGenerator::getInstance( dfridata );
		mGenerators.insert( dfrMG );
	}
	//------------------------------------------------------------
	bool MaterialGeneratorManager::modifyMaterial( Ogre::Material& mat )
	{
		Ogre::SetIterator< GeneratorSet > iter( mGenerators );
		while( iter.hasMoreElements() )
		{
			if( iter.getNext()->modifyMaterial( mat ) )
				return true;
		}
		//none of the generators has modified the material
		return false;
	}
	
}
}
#endif
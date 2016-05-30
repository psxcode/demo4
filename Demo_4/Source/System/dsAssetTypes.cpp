#include "DemoStableHeaders.h"
#include "dsAssetTypes.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	dsEntityLodBias::dsEntityLodBias()
		: meshLodBias(1.0f)
		, meshMaxDetailIndex(0)
		, meshMinDetailIndex(0)
	{
	}
	//------------------------------------------------------------
	dsLightGeom::dsLightGeom()
		: meshWithGeomLods(true)
	{
	}
	//------------------------------------------------------------
}
}
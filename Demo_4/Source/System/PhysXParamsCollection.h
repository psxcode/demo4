#pragma once

#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX_COLLECTIONS

#include <NxMaterialDesc.h>
#include <NxSceneDesc.h>
#include <NxConvexMesh.h>

#if DBS_LM_SCRIPT_OGREMAX_TIXML
#include "System\OgremaxTinyXmlInterface.h"
#endif

#include "collection_traits.h"

namespace Demo
{
namespace System
{
//------------------------------------------------------------
class PhysXParamsCollection
{
	friend class PhysXController;
public:
	//--------------------------------//
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	typedef OgremaxTinyXmlInterface::traits< TiXmlElement > tixmlel_traits;
#endif
	//--------------------------------//
	struct RemoteDebuggerParams
	{
		RemoteDebuggerParams()
			: connect( false )
			, port(0)
		{
		}
		bool connect;
		string_traits::str hostName;
		int port;
	};
	//--------------------------------//
	typedef shared_ptr_traits< NxSceneDesc >::shared_ptr NxSceneDescPtr;
	typedef shared_ptr_traits< NxMaterialDesc >::shared_ptr NxMaterialDescPtr;
	typedef shared_ptr_traits< NxConvexMesh >::shared_ptr NxConvexMeshPtr;
	//--------------------------------//
	typedef collection_traits< NxSceneDescPtr >::container SceneContainer;
	typedef collection_traits< NxMaterialDescPtr >::container MaterialContainer;
	//--------------------------------//
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	void AddParamsFrom( tixmlel_traits::in );
#endif

	void AddSceneDesc( SceneContainer::value_type const &sd, bool replace = true );
	void AddMaterialDesc( MaterialContainer::value_type const &md, bool replace = true );

	NxSceneDescPtr CreateSceneDesc( string_traits::in name )const;
	NxMaterialDescPtr CreateMaterialDesc( string_traits::in name )const;

	NxMaterialDescPtr GetDefaultMaterialDesc()const{ return defaultMaterialDesc; }
	NxSceneDescPtr GetDefaultSceneDesc()const{ return defaultSceneDesc; }
	const RemoteDebuggerParams& GetRDParams()const{ return rdParams; }
	string_traits::outref GetNxuFilename()const{ return nxuFilename; }

private:
	//--------------------------------//
	static string_traits::str defaultMaterialName;
	static string_traits::str defaultSceneName;
	//--------------------------------//
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	void LoadSDK( tixmlel_traits::in element, bool replace = true );
	void LoadRDParams( tixmlel_traits::in element, bool replace = true );
	void LoadMaterials( tixmlel_traits::in element, bool replace = true );
	void LoadMaterial( tixmlel_traits::in element, bool replace = true );
	void LoadScenes( tixmlel_traits::in element, bool replace = true );
	void LoadScene( tixmlel_traits::in element, bool replace = true );
#endif

	MaterialContainer materialContainer;
	SceneContainer sceneContainer;
	RemoteDebuggerParams rdParams;

	string_traits::str nxuFilename;

	NxMaterialDescPtr defaultMaterialDesc;
	NxSceneDescPtr defaultSceneDesc;
};
//------------------------------------------------------------
}
}
#endif
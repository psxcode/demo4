#include "PhysXController.h"
#if DBS_LM_PHYSX

namespace Ogre
{
	template<>
	Demo::System::PhysXController* Singleton<Demo::System::PhysXController>::ms_Singleton = 0;
}

namespace Demo
{
namespace System
{
//------------------------------------------------------------
NxConvexMesh* PhysXController::CreateConvexMesh( NxuConvexMeshDesc &desc )
{
	if( _physicsSDK == 0 )return 0;
	NxConvexMesh* retMesh(0);
	NxConvexMeshDesc meshDesc;

	desc.copyTo( meshDesc );

	if( desc.mCookedDataSize == 0 )
	{
		bool statusOK = true;
		InitCooking();
		ImportMemoryWriteBuffer writeBuffer;
		statusOK = CookConvexMesh( meshDesc, writeBuffer );
		CloseCooking();
		if( statusOK )
			retMesh = _physicsSDK->createConvexMesh( ImportMemoryReadBuffer(writeBuffer.data) );
	}
	else
	{
		ImportMemoryReadBuffer readBuffer( (const NxU8*)&desc.mCookedData[0] );
		retMesh = _physicsSDK->createConvexMesh( readBuffer );
	}

	return retMesh;
}
//------------------------------------------------------------
NxTriangleMesh* PhysXController::CreateTriangleMesh( string_traits::in name )
{
	NxuTriangleMeshDesc::shared_ptr meshPtr = NPC::GetSingleton().GetTriangleMeshDesc( name );
	if( meshPtr == 0 )return 0;
	return CreateTriangleMesh( meshPtr.getReference() );
}
//------------------------------------------------------------
NxTriangleMesh* PhysXController::CreateTriangleMesh( NxuTriangleMeshDesc &desc )
{
	if( _physicsSDK == 0 )return 0;
	NxTriangleMesh *retMesh(0);
	NxTriangleMeshDesc meshDesc;

	desc.copyTo( meshDesc );

	if( desc.mCookedDataSize == 0 )
	{
		meshDesc.pmap = 0;
		bool statusOK = true;
		InitCooking();
		ImportMemoryWriteBuffer writeBuffer;
		statusOK = CookTriangleMesh( meshDesc, writeBuffer );
		CloseCooking();
		if( statusOK )
			retMesh = _physicsSDK->createTriangleMesh( ImportMemoryReadBuffer( writeBuffer.data ) );
	}
	else
	{
		ImportMemoryReadBuffer readBuffer( (const NxU8*)&desc.mCookedData[0] );
		retMesh = _physicsSDK->createTriangleMesh( readBuffer );
	}

	if( retMesh != 0 && desc.mPmapData.size() != 0 )
	{
		NxU8 *data = &desc.mPmapData[0];
	  	NxPMap p;
		p.data = data;
		p.dataSize = desc.mPmapData.size();
		retMesh->loadPMap(p);
	}

	return retMesh;
}
//------------------------------------------------------------
void PhysXController::ReleaseConvexMesh( NxConvexMesh*& mesh )
{
	if( mesh == 0 || _physicsSDK == 0 )return;
	_physicsSDK->releaseConvexMesh( *mesh );
	mesh = 0;
}
//------------------------------------------------------------
void PhysXController::ReleaseTriangleMesh( NxTriangleMesh*& mesh )
{
	if( mesh == 0 || _physicsSDK == 0 )return;
	_physicsSDK->releaseTriangleMesh( *mesh );
	mesh = 0;
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
PhysXController::PhysXController()
	: _physicsSDK(0)
{
}
//------------------------------------------------------------
/*virtual*/ PhysXController::~PhysXController()
{
	if(_physicsSDK)
	{
		_physicsSDK->release();
		_physicsSDK = 0;
	}
}
//------------------------------------------------------------
bool PhysXController::initialize()
{
	if(_physicsSDK) return false;

	_physicsSDK = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION, NULL, NULL );
	if (!_physicsSDK) return false;

	/*
	//remote debugger
	PPC::RDParams rdParams = PPC::GetSingleton().GetRDParams();
	if ( rdParams.connect )
	{
		if( rdParams.hostName.empty() ){CloseController(); return false;}
		_physicsSDK->getFoundationSDK().getRemoteDebugger()->connect( rdParams.hostName.c_str(), rdParams.port );
	}
	//remote debugger
	*/
	return true;
}
//------------------------------------------------------------
}
}
#endif //DBS_LM_PHYSX
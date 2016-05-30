#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <NxPhysics.h>
#include <NxCooking.h>
#include <OgreSingleton.h>
#include "Core/Controller.h"
#include "delegate_traits.h"


namespace Demo {
namespace System {
//------------------------------------------------------------
class PhysXController
	: public Ogre::Singleton< PhysXController >
	, public Core::Controller
{
	friend class Ogre::Singleton< PhysXController >;
public:
	//--------------------------------//
	class traits
	{
	public:
		typedef Demo::map< string_traits::str, shared_ptr_traits<OgrePhysX::Scene>::shared_ptr >::type scene_container;
	};
	//--------------------------------//
	static bool HasCookingLibrary();
	static bool InitCooking();
	static void CloseCooking();
	static bool CookConvexMesh( const NxConvexMeshDesc&, NxStream& );
	static bool CookTriangleMesh( const NxTriangleMeshDesc&, NxStream& );
	//--------------------------------//
	virtual void CloseController();
	virtual void OperateController();
	virtual bool ConnectChild( Controller* ) { return false; }
	virtual bool ConnectingToParent( Controller* ) {return false;}

	bool Initialize();
	void startSimulate( Ogre::Real time );
	void fetchSimulate();
	void syncRenderables();

	bool CreateScene( string_traits::in sceneDescName );
	void ReleaseScene();
	bool SetSDKParameter( ::NxParameter, float );
	NxConvexMesh* CreateConvexMesh( string_traits::in name );
	NxConvexMesh* CreateConvexMesh( NxuConvexMeshDesc& );
	NxTriangleMesh* CreateTriangleMesh( string_traits::in name );
	NxTriangleMesh* CreateTriangleMesh( NxuTriangleMeshDesc& );
	void ReleaseConvexMesh( NxConvexMesh*& mesh );
	void ReleaseTriangleMesh( NxTriangleMesh*& mesh );

private:
	PhysXController();
	PhysXController( const PhysXController& );
	PhysXController& operator=( const PhysXController& );
	~PhysXController();

	void OperateScene();
	void OperateFalseScene();

	delegate_traits::del0<void>::type sceneOpDel;

	NxPhysicsSDK* _physicsSDK;
};
//------------------------------------------------------------
}
}
#endif


#include "DemoPrerequisites.h"
#if DBS_LM_GAME_OBJECTS

#include "DemoSceneController.h"
#include "System\ScriptService.h"
#include "System\PhysXSceneInterface.h"
#include "System\PhysXController.h"
#include "System\OgrePhysxConverter.h"
#include "System\OgreRenderWindowInterface.h"
#include "System\OgreSceneManagerInterface.h"
#include "System\VehicleParamsCollection.h"
#include "System\DistanceUnitInterface.h"
#include "GameObjects\SimpleGUIController.h"
#include "Vehicle\drive_options.h"

#include <NxActorDesc.h>
#include <NxPlaneShapeDesc.h>
#include <NxTriangleMeshShapeDesc.h>
#include <NxBoxShapeDesc.h>
#include <OgreMeshManager.h>

namespace Demo
{
	//------------------------------------------------------------
	typedef System::ScriptService ScrS;
	typedef System::PhysXController PsxC;
	typedef System::PhysXSceneInterface PSI;
	typedef System::OgrePhysxConverter OPC;
	typedef System::OgreRenderWindowInterface ORWI;
	typedef System::OgreSceneManagerInterface OSMI;
	typedef System::VehicleParamsCollection VPC;
	typedef System::DistanceUnitInterface DUI;
	//------------------------------------------------------------
	bool DemoSceneController::Initialize()
	{
		if( isInitializedController() ) return false;

		int loadOptions(0);
		loadOptions |= ScrS::LO_LOAD_FROM_OGRE;
		ScrS::GetSingleton().LoadFile( String( "profile.xml" ), loadOptions );

		if ( !PsxC::GetSingleton().CreateScene( "default" )){ CloseDemoSceneController(); return false; }

		/*
		NxActorDesc planeDesc;
		NxPlaneShapeDesc planeShapeDesc;
		planeDesc.shapes.pushBack( &planeShapeDesc );
		Ogre::Vector3 ogrePosition(0,0,0);
		planeDesc.globalPose.t = OPC::toNx( ogrePosition );
		planeDesc.globalPose.M.fromQuat(OPC::toNx( Ogre::Quaternion::IDENTITY ));
		if( !planeDesc.isValid()){ CloseDemoSceneController(); return false; }
		planePhysx = PSI::get()->createActor( planeDesc );
		/*

		/*
		Ogre::Plane tempPlane( Ogre::Vector3::UNIT_Y, 0 );
		String tempString = "PhysxPlane.mesh";
		Ogre::MeshManager::getSingleton().createPlane( tempString, "Default", tempPlane, DUI::DistanceToUnits(1e4), DUI::DistanceToUnits(1e4), 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z );
		planeEntity = OSMI::get()->createEntity( "PhysxPlaneEntity", tempString );
		planeEntity->setMaterialName( "ironwheel" );
		planeNode = OSMI::get()->getRootSceneNode()->createChildSceneNode();
		planeNode->attachObject( planeEntity );
		*/

		/*
		NxActorDesc boxDesc;
		NxBodyDesc boxBodyDesc;
		NxBoxShapeDesc boxShapeDesc;
		Ogre::Vector3 boxDimensions( DUI::DistanceToUnits( Ogre::Vector3(50, 50, 50)));
		Ogre::Vector3 boxPosition( DUI::DistanceToUnits( Ogre::Vector3(0, 1000, 0)));
	
		boxBodyDesc.mass = 20;
		boxShapeDesc.dimensions = OPC::toNx( boxDimensions );
		boxDesc.shapes.pushBack( &boxShapeDesc );
		boxDesc.body = &boxBodyDesc;
		boxDesc.globalPose.t = OPC::toNx( boxPosition );
		if( !boxDesc.isValid()) { CloseDemoSceneController(); return false; }
		physxBox = PSI::get()->createActor( boxDesc );

		Ogre::Entity* boxEntity = OSMI::get()->createEntity( "PhysxBoxEntity", Ogre::SceneManager::PT_CUBE );
		boxEntity->setMaterialName( "modrim" );
		boxNode = OSMI::get()->getRootSceneNode()->createChildSceneNode();
		boxNode->attachObject( boxEntity );
		boxNode->setPosition( boxPosition );
		boxNode->setScale( boxDimensions/50 );
		*/


		/*
		//Physx ground
		NxActorDesc groundActorDesc;
		NxTriangleMeshShapeDesc triDesc;
		triDesc.meshData = System::PhysXController::GetSingleton().CreateTriangleMesh( "TriangleMesh_0" );
		if( triDesc.meshData == 0 ) { 
			CloseDemoSceneController(); return false;
		}
		groundActorDesc.shapes.push_back( &triDesc );
		if( !groundActorDesc.isValid() ) {
			CloseDemoSceneController(); return false;
		}
		groundActor = PSI::get()->createActor( groundActorDesc );
		*/

		Ogre::Entity* groundEntity = OSMI::get()->createEntity( "ground", "ground.mesh" );
		groundEntity->setMaterialName( "iron_wheel" );
		Ogre::SceneNode* groundNode = OSMI::get()->getRootSceneNode()->createChildSceneNode();
		groundNode->attachObject( groundEntity );

		/*
		Ogre::Entity* planeEntity = OSMI::get()->createEntity( "floorPlane", "plane.mesh" );
		planeEntity->setMaterialName( "iron_wheel" );
		planeNode = OSMI::get()->getRootSceneNode()->createChildSceneNode();
		planeNode->attachObject( planeEntity );
		*/

		
		//test vehicle
		/*
		createTestVehicle();
		createTestDriver();
		*/
		//test vehicle
		


		/*
		//test camera
		OrbitCameraController::InitData oidata;
		oidata.moveSpeed = DUI::DistanceToUnits(100);
		oidata.rotSpeed = DUI::DistanceToUnits(40);
		oidata.position = DUI::DistanceToUnits( Ogre::Vector3( 0, 1000, 1000 ));
		oidata.nearClipDistance = 5;
		ocam = new OrbitCameraController();
		if( !ocam->Initialise( oidata ) ){ CloseDemoSceneController(); return false; }
		Ogre::Camera* cam = 0;
		try{ cam = OSMI::get()->createCamera( "PlayerCam" ); }
		catch(...){ return false; }
		ocam->SetCamera( cam );
		ocam->SetParentOSN( vehicle->getOSN() );
		//test camera
		*/

		//sdkCamera
		createTestSdkCamera();
		//sdkCamera

		//test light
		Ogre::Light* light = OSMI::get()->createLight( "Light1" );
		light->setType( Ogre::Light::LT_DIRECTIONAL );
		light->setDiffuseColour( 0.7f, 0.7f, 0.7f );
		light->setPosition( 0, 1e3f, 1e3f );
		light->setDirection( Ogre::Vector3::NEGATIVE_UNIT_Y );
		//test light

		
		setInitializeController();
		return true;
	}
	//------------------------------------------------------------
	void DemoSceneController::CloseDemoSceneController()
	{
		if( ocam != 0 ){ delete ocam; ocam = 0; }
		if( groundActor != 0 ){ PSI::get()->releaseActor( *groundActor ); groundActor = 0; }
		
		if( GUIController != 0 )GUIController = 0;
		Core::Controller::CloseController();
	}
	//------------------------------------------------------------
	void DemoSceneController::createTestVehicle()
	{
		vehicle = Vehicle::PhysXVehicle::CreatePhysXVehicle( "cars/demo_vaz_2106" );
		if( vehicle != 0 )
		{
			Vehicle::PhysXVehicle::StartPhysXVehicleInitData startPhysXIData;
			startPhysXIData.position = Ogre::Vector3( 0,DUI::DistanceToUnits(300),0 );
			startPhysXIData.orientation = Ogre::Quaternion::IDENTITY;
			if( !vehicle->StartPhysX( startPhysXIData )){ vehicle->ClosePhysXVehicle(); vehicle = 0; return; }

			OSMI::get()->getRootSceneNode()->addChild( vehicle->getOSN() );

			ConnectChildController( vehicle.getPointer() );
		}
	}
	//------------------------------------------------------------
	void DemoSceneController::createTestDriver()
	{
		if( vehicle == 0 )return;

		Vehicle::VehicleDriver::CreateData vdcdata;
		int driveOptions = VPC::GetSingleton().CreateDriveOptions();
		vdcdata.driveOptions = driveOptions;
		vdcdata.vtdData = vehicle->getVehicleToDriverData();
		driver = Vehicle::VehicleDriver::CreateVehicleDriver( vdcdata );
		if( driver != 0 )
		{
			if( (driveOptions & Vehicle::DO_USER_CONTROLS) != 0 )
				driver->CreateUserInput();
			ConnectChildController( driver.getPointer() );
		}
	}
	//------------------------------------------------------------
	void DemoSceneController::createTestSdkCamera()
	{
		sdkCamera.bind( new SdkCameraMan() );
		Ogre::Camera* cam = OSMI::get()->createCamera( "PlayerCam" );
		sdkCamera->Initialize();
		sdkCamera->setCamera( cam );
		sdkCamera->setStyle( SdkCameraMan::CS_FREELOOK );
		sdkCamera->CreateUserInput();

		ConnectChildController( sdkCamera.getPointer() );

		Ogre::Viewport* vp = ORWI::get()->addViewport( cam );
		vp->setBackgroundColour(Ogre::ColourValue(0,1,0));
		cam->setAspectRatio( Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	}
	//------------------------------------------------------------
	void DemoSceneController::createTestVehicleSimpleGUI()
	{
		SimpleGuiController::SimpleGuiControllerInitData GUIInitdata;
		GUIInitdata.vehicle = vehicle;
		GUIInitdata.driver = driver;
		GUIController.bind( new SimpleGuiController() );
		if(!GUIController->Initialize( GUIInitdata )){ GUIController = 0; return; }
	}
	//------------------------------------------------------------
	void DemoSceneController::OperateController()
	{
		//driver operates automatically
		//vehicle operates automatically
		//sdkCamera operates automatically
		
		PsxC::GetSingleton().OperateController();

		//ocam->OperateController();
		//boxNode->setPosition( OPC::toOgre( physxBox->getGlobalPosition()));
		//boxNode->setOrientation( OPC::toOgre( physxBox->getGlobalOrientation()));
		//GUIController->OperateController();
	}
	//------------------------------------------------------------
}
#endif
#pragma once
#include "Core\Controller.h"
#include "Vehicle\PhysXVehicle.h"
#include "Vehicle\VehicleDriver.h"
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <NxActor.h>
#include "GameObjects\SimpleGUIController.h"
#include "GameObjects\OrbitCameraController.h"
#include "GameObjects\SdkCameraMan.h"

namespace Demo
{
	//------------------------------------------------------------
	class DemoSceneController : public Core::Controller
	{
	public:
		//--------------------------------//
		typedef shared_ptr_traits< DemoSceneController >::shared_ptr shared_ptr;
		typedef shared_ptr_traits< DemoSceneController >::const_shared_ptr const_shared_ptr;
		//--------------------------------//
		DemoSceneController()
			: Core::Controller( Core::CT_NOT_DEFINED )
			, planeNode(0)
			, planeEntity(0)
			, boxNode(0)
			, physxBox(0)
			, groundActor(0)
			, vehicle(0)
			, driver(0)
			, ocam(0)
			, GUIController(0)
		{
		}
		virtual ~DemoSceneController(){CloseDemoSceneController();}
		virtual void OperateController();

		bool Initialize();
		void CloseDemoSceneController();

	protected:
		void createTestVehicle();
		void createTestVehicleSimpleGUI();
		void createTestDriver();
		void createTestSdkCamera();

		Ogre::SceneNode* planeNode;
		Ogre::Entity* planeEntity;
		NxActor* groundActor;

		Ogre::SceneNode* boxNode;
		NxActor* physxBox;
		Vehicle::PhysXVehicle::shared_ptr vehicle;
		Vehicle::VehicleDriver::shared_ptr driver;
		OrbitCameraController* ocam;
		SdkCameraMan::shared_ptr sdkCamera;
		SimpleGuiController::shared_ptr GUIController;
	};
	//------------------------------------------------------------
}

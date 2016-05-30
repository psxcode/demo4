#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_GAME_OBJECTS

#include <OgreOverlayElement.h>
#include "Vehicle\Abstract\BaseVehicle.h"
#include "Vehicle\Driver\VehicleDriver.h"
#include "Vehicle\Driver\IGearboxControllerState.h"

namespace Demo
{
//------------------------------------------------------------
class SimpleGuiController : public Core::Controller
{
public:
	DEFINE_CLASS_SHARED_PTR(SimpleGuiController)
	//--------------------------------//
	struct SimpleGuiControllerInitData
	{
		Vehicle::BaseVehicle::shared_ptr vehicle;
		Vehicle::VehicleDriver::shared_ptr driver;
	};
	//--------------------------------//
	SimpleGuiController()
		: overlayEl(0)
	{
	}
	virtual ~SimpleGuiController()
	{
		CloseController();
	}

	//Controller interface
	virtual void OperateController(){ PrepareStats(); }
	virtual void CloseController();
	
	bool initialize( const SimpleGuiControllerInitData& initdata );

private:
	//--------------------------------//
	struct GUIElements
	{ 
		static string_traits::cstr MOTOR;
		static string_traits::cstr TORQUE;
		static string_traits::cstr GEARBOX;
		static string_traits::cstr THROTTLE;
		static string_traits::cstr STEER;
		static string_traits::cstr BRAKE;
		static string_traits::cstr CLUTCH;
		static string_traits::cstr SPTR;
		static string_traits::cstr CHART_OP;
		static string_traits::cstr CHART_ED;
		static string_traits::cstr CHART_ELEMENT;
		static string_traits::cstr CHART_FILLER;
		static string_traits::cstr ENDL;
		static int chartSize;
		/** Функция добавляет к передаваемой строке строку вида <|||....>, chartElCount показывает количество элементов(|||)
		chartElCount должен иметь значение в диапазоне 0-1*/
		static void addValueToChart( string_traits::inout, float chartElCount );
		static void addValueToDoubleChart( string_traits::inout, float chartElCount );
	};
	//--------------------------------//
	static string_traits::str overlayElementName;
	//--------------------------------//
	void PrepareStats();
	Vehicle::IMotorState::shared_ptr motorState;
	Vehicle::IGearboxState::shared_ptr gearboxState;
	Vehicle::IMechGearboxControllerState::shared_ptr gcState;
	Vehicle::IPedalState::shared_ptr throttleState;
	Vehicle::ISteerState::shared_ptr steerState;
	Vehicle::IPedalState::shared_ptr brakeState;
	Vehicle::IPedalState::shared_ptr clutchState;
	Vehicle::IMechanicsState::shared_ptr diffState;
	Vehicle::IMechanicsState::shared_ptr RLWheelNodeState;
	Vehicle::ISpeedometerState::shared_ptr speedometerState;

	Ogre::OverlayElement* overlayEl;
};
//------------------------------------------------------------
}
#endif //DBS_LM_GAME_OBJECTS
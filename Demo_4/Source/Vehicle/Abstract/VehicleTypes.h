#pragma once
#if DBS_LM_ABSTRACT_VEHICLE

namespace Demo {
namespace Vehicle {
//------------------------------------------------------------
enum VEHICLE_CONTROL_AXES
{
	VCA_THROTTLE = 0x1,
	VCA_BRAKE = 0x2,
	VCA_CLUTCH = 0x4,
	VCA_STEER = 0x8,
	VCA_HANDBRAKE = 0x10
};
//------------------------------------------------------------
enum VEHICLE_CONTROL_SYSTEMS
{
	VCS_TCS = 0x1,
	VCS_ABS = 0x2,
	VCS_AUTO_CLUTCH = 0x4,
	VCS_SPEED_STEERING = 0x8
};
//------------------------------------------------------------
enum VEHICLE_SIDE
{
	VS_NOT_DEFINED,
	VS_LEFT,
	VS_RIGHT,
	VS_CENTER
};
//------------------------------------------------------------
struct VehicleOSNNames
{
	static string_traits::outcopy ComposeWheelNodeName( unsigned int axleIndex, string_traits::in sideStr );
	static string_traits::cstr SPTR;
	static string_traits::cstr WHEEL_NODE;
	static string_traits::cstr IRON_WHEEL;
	static string_traits::cstr RUBBER_WHEEL;
	static string_traits::cstr BRAKE_BLOCK;
	static string_traits::cstr FRONT;
	static string_traits::cstr BACK;
	static string_traits::cstr LEFT;
	static string_traits::cstr RIGHT;
	static string_traits::cstr AXLE;
};
//------------------------------------------------------------
}
}
#endif
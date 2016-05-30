#pragma once

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	enum MECHANICS_TYPE
	{
		MT_NOT_DEFINED,
		MT_MOTOR,
		MT_GEARBOX,
		MT_DIFFERENTIAL,
		MT_FLYWHEEL,
		MT_WHEEL_NODE
	};
	//------------------------------------------------------------
	enum MOTOR_INTERFACE_TYPE
	{
		MIT_NOT_DEFINED,
		MIT_STATE,
		MIT_CONTROL,
		MIT_CALLBACK,
		MIT_TWEAK
	};
	//------------------------------------------------------------
	enum GEARBOX_TYPE
	{
		GT_NOT_DEFINED,
		GT_MECHANICAL,
		GT_SEQUENTAL,
		GT_AUTOMATIC
	};
	//------------------------------------------------------------
	enum AUTO_GEARBOX_MODE
	{
		AGM_NOT_DEFINED = 0,
		AGM_DIRECT = 0x1,
		AGM_NEUTRAL = 0x2,
		AGM_REVERSE = 0x4,
		AGM_PARK = 0x8,
		AGM_SEQ_EMU = 0x10,
		AGM_SPORT = 0x20
	};
	//------------------------------------------------------------
	enum GEARBOX_INTERFACE_TYPE
	{
		GIT_NOT_DEFINED,
		GIT_STATE,
		GIT_CONTROL,
		GIT_CALLBACK,
		GIT_TWEAK
	};
	//------------------------------------------------------------
	enum DIFFERENTIAL_MODE
	{
		DM_NOT_DEFINED,
		DM_LOCK,
		DM_DIS_FIRST,
		DM_DIS_SECOND
	};
	//------------------------------------------------------------
	enum THROTTLE_CONTROL_TYPE
	{
		TCS_NOT_DEFINED,
		TCS_CFEED,
	};
	//------------------------------------------------------------
}
}
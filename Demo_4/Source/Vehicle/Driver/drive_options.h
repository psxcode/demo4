#pragma once

namespace Demo
{
namespace Vehicle
{
	//--------------------------------//
	enum DRIVE_OPTIONS
	{
		DO_NO_OPTIONS = 0,
		DO_THROTTLE_ASSIST = 0x1,
		DO_BRAKE_ASSIST = 0x2,
		DO_CLUTCH_ASSIST = 0x4,
		DO_STEER_ASSIST = 0x8,
		DO_SHIFT_ASSIST = 0x10,
		DO_REVERSE_PREVENT = 0x40,
		DO_NEUTRAL_PREVENT = 0x80,
		DO_USER_CONTROLS = 0x100,
		DO_USER_CLUTCH = 0x200,
		DO_THROTTLE_AXIS = 0x1000,
		DO_BRAKE_AXIS = 0x2000,
		DO_CLUTCH_AXIS = 0x4000,
		DO_STEER_AXIS = 0x8000
	};
	//--------------------------------//
}
}
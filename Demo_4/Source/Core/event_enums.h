#pragma once

namespace Demo
{
namespace Core
{
	//------------------------------------------------------------
	enum EVENT_TYPE
	{
		ET_NOT_DEFINED,
		ET_GENERIC,
		ET_SYSTEM_INPUT
	};
	//------------------------------------------------------------
	enum CALLBACK_TYPE
	{
		CALL_NOT_DEFINED,
		CALL_GENERIC,
		CALL_CONVERTOR,
		CALL_INPUT,
		CALL_INPUT_JOYSTICK
	};
	//------------------------------------------------------------
	enum EVENT_ID
	{
		EID_NOT_DEFINED,
		EID_NULL_EVENT,

		EID_INPUT_KEYBOARD,
		EID_INPUT_MOUSE_BUTTON,
		EID_INPUT_MOUSE_MOVED,
		EID_INPUT_JOYSTICK_BUTTON,
		EID_INPUT_JOYSTICK_AXIS,
		EID_INPUT_JOYSTICK_SLIDER,
		EID_INPUT_JOYSTICK_POV
	};
	//------------------------------------------------------------
}
}
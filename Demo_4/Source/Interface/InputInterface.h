#pragma once

#include <OgreSingleton.h>
#include <OIS/OISMouse.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISJoystick.h>
#include "input_traits.h"
#include "delegate_traits.h"

namespace Demo
{
//------------------------------------------------------------
class InputRequest
{
public:
	typedef Demo::vector<InputRequest>::type container_t;
	InputRequest()
	{
	}
	InputRequest(
		delegate_traits::composite0::traits::del_in del,
		input_traits::INPUT_ID id = input_traits::IID_NOT_DEFINED,
		input_traits::INPUT_STATE state = input_traits::IS_NOT_DEFINED)
		: del(del) , id(id)	, state(state)
	{
	}
	delegate_traits::composite0::traits::del_t del;
	input_traits::INPUT_ID id;
	input_traits::INPUT_STATE state;
};
//------------------------------------------------------------
class InputInterface
{
public:
	DEFINE_CLASS_SHARED_PTR(InputInterface)
	//--------------------------------//
	static InputInterface::shared_ptr getInstance();
	//--------------------------------//
	InputInterface( OIS::Mouse* m, OIS::Keyboard* k )
		: _mouse(m)
		, _keyboard(k)
	{
	}

	inline const OIS::Axis& getMouseX()const { return _mouse->getMouseState().X; }
	inline const OIS::Axis& getMouseY()const { return _mouse->getMouseState().Y; }
	inline const OIS::Axis& getMouseZ()const { return _mouse->getMouseState().Z; }
	inline bool isKeyDown( OIS::KeyCode key ) { return _keyboard->isKeyDown( key ); }
	inline bool isKeyModifierDown( OIS::Keyboard::Modifier km ) { return _keyboard->isModifierDown( km ); }
	inline bool isMouseButtonDown( OIS::MouseButtonID button )const { return (_mouse->getMouseState().buttons & ( 1L << button )) != 0; }
	inline OIS::Pov getPov( int n )const { return _joy->getJoyStickState().mPOV[n]; }
	inline const OIS::Axis& getAxis( int n )const { return _joy->getJoyStickState().mAxes[n]; }
	inline const OIS::Slider& getSlider( int n )const { return _joy->getJoyStickState().mSliders[n]; }
	inline bool isJoyButtonDown( int button )const { return _joy->getJoyStickState().mButtons[button]; }
	inline float getAxisValueAsFloat( const OIS::Axis& axis )const { return float( axis.abs ) / 32768; }

	bool addInputDel(InputRequest const &request);
	bool addInputDel(InputRequest::container_t const &request);
	void removeInputDel(InputRequest const &request);
	void removeInputDel(InputRequest::container_t const &request);

private:
	OIS::Mouse* _mouse;
	OIS::Keyboard* _keyboard;
	OIS::JoyStick* _joy;
};
//------------------------------------------------------------
}



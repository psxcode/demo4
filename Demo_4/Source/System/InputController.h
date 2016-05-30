#pragma once

#include <OgreWindowEventUtilities.h>
#include <OgreRenderWindow.h>
#include <OIS/OIS.h>
#include "Core/Controller.h"
#include "Interface/InputInterface.h"
#include "input_traits.h"

namespace Demo {
namespace System {
//------------------------------------------------------------
class InputController
	: public Ogre::WindowEventListener
	, public Core::Controller
{
protected:
	//--------------------------------//
	struct InputIdData
	{
		InputIdData()
			: type(OIS::OISUnknown)
			, componentType(OIS::OIS_Unknown)
			, code(-1)
		{
		}

		OIS::Type type;
		OIS::ComponentType componentType;
		int code;
	};
	//--------------------------------//

public:
	DEFINE_CLASS_SHARED_PTR(InputController)
	//--------------------------------//
	class traits : public Controller::traits
	{
	public:
		typedef Demo::map< input_traits::INPUT_ID, InputIdData >::type id_cont;
	};
	//--------------------------------//
	InputController();
	virtual ~InputController();

	//Controller interface
	virtual void OperateController();
	virtual void CloseController();
	virtual void SetActiveController( bool active );

	//WindowEventListener interface
	virtual void windowResized( Ogre::RenderWindow *rw );
	virtual void windowClosed( Ogre::RenderWindow *rw );

	bool initialize( Ogre::RenderWindow *rw );

	bool addInputDel(InputRequest const &request);
	void removeInputDel(InputRequest const &request);

	const InputInterface::shared_ptr& getInputInterface() {return mpInputInterface;}
	
protected:
	//--------------------------------//
	class KeyboardListener : public OIS::KeyListener
	{
	public:
		KeyboardListener() {
			pressedComps.resize(256);
			releasedComps.resize(256);
		}
		virtual ~KeyboardListener() {}

		//OIS::KeyListener interface
		virtual bool keyPressed( const OIS::KeyEvent& evt ) { pressedComps[evt.key](); return true; }
		virtual bool keyReleased( const OIS::KeyEvent& evt ) { releasedComps[evt.key](); return true; }

		Demo::vector< delegate_traits::composite0 >::type pressedComps;
		Demo::vector< delegate_traits::composite0 >::type releasedComps;
	};
	//--------------------------------//
	class MouseListener : public OIS::MouseListener
	{
	public:
		MouseListener() {
			pressedComps.resize(8);
			releasedComps.resize(8);
		}
		virtual ~MouseListener() {}

		//OIS::MouseListener interface
		virtual bool mouseMoved( const OIS::MouseEvent& ) { mouseMovedComp(); return true; }
		virtual bool mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id ) { pressedComps[id](); return true; }
		virtual bool mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id ) { releasedComps[id](); return true; }

		Demo::vector< delegate_traits::composite0 >::type pressedComps;
		Demo::vector< delegate_traits::composite0 >::type releasedComps;
		delegate_traits::composite0 mouseMovedComp;
	};
	//--------------------------------//
	class JoystickListener : public OIS::JoyStickListener
	{
	public:
		JoystickListener()
		{
			pressedComps.resize(32);
			releasedComps.resize(32);
		}
		virtual ~JoystickListener() {}

		//JoyStickListener interface
		virtual bool axisMoved( const OIS::JoyStickEvent &arg, int axis ) { return true; }
		virtual bool buttonPressed( const OIS::JoyStickEvent &arg, int button ) { pressedComps[button](); return true; }
		virtual bool buttonReleased( const OIS::JoyStickEvent &arg, int button ) { releasedComps[button](); return true; }
		virtual bool povMoved( const OIS::JoyStickEvent &arg, int pov ) { return true; }
		virtual bool sliderMoved( const OIS::JoyStickEvent &arg, int sliderID ) { return true; }

		Demo::vector< delegate_traits::composite0 >::type pressedComps;
		Demo::vector< delegate_traits::composite0 >::type releasedComps;
	};
	//--------------------------------//
	template<typename T>
	bool _addButtonDel(
		T &listenerRef,
		input_traits::INPUT_STATE state,
		int code,
		delegate_traits::composite0::traits::del_in del )
	{
		switch(state)
		{
		case input_traits::IS_PRESSED: listenerRef.pressedComps[code].add(del);
			return true;
		case input_traits::IS_RELEASED: listenerRef.releasedComps[code].add(del);
			return true;
		default:
			return false;
		}
	}
	template<typename T>
	void _removeButtonDel(
		T &listenerRef,
		input_traits::INPUT_STATE state,
		int code,
		delegate_traits::composite0::traits::del_in del )
	{
		switch(state)
		{
		case input_traits::IS_PRESSED:
			_removeButtonDel(listenerRef.pressedComps, code, del);
			break;
		case input_traits::IS_RELEASED:
			_removeButtonDel(listenerRef.releasedComps, code, del);
			break;
		default:
			_removeButtonDel(listenerRef.pressedComps, code, del);
			_removeButtonDel(listenerRef.releasedComps, code, del);
			break;
		}
	}
	template<typename T>
	void _removeButtonDel(
		T &container,
		int code,
		delegate_traits::composite0::traits::del_in del )
	{
		if(code < 0) std::for_each(container.begin(), container.end(),
			[&del](delegate_traits::composite0 &dc) { dc.remove(del); });
		else container[code].remove(del);
	}

	
	InputController( const InputController& );
	InputController& operator=( const InputController& );

	//Приготовить настройки раскладки инпута
	//Позже надо заменить на чтение со скрипта
	bool prepareInputIDs();

	bool createInputSystemAndDevices();
	void destroyInputSystemAndDevices();

	void createBindListeners();

	void keyboardValidCapture() {mpKeyboard->capture();}
	void mouseValidCapture() {mpMouse->capture();}
	void deviceValidCapture(OIS::Object *obj) {obj->capture();}
	void deviceInvalidCapture() {}

	OIS::InputManager *mpInputManager;
	OIS::Keyboard *mpKeyboard;
	OIS::Mouse *mpMouse;
	OIS::JoyStick *mpJoystick;
	Ogre::RenderWindow *mpRenderWindow;

	delegate_traits::del0<void>::type keyboardCaptureDel;
	delegate_traits::del0<void>::type mouseCaptureDel;

	InputInterface::shared_ptr mpInputInterface;

	shared_ptr_traits<KeyboardListener>::type keyboardListener;
	shared_ptr_traits<MouseListener>::type mouseListener;
	shared_ptr_traits<JoystickListener>::type joystickListener;

	traits::id_cont idCont;
};
//------------------------------------------------------------
}
}

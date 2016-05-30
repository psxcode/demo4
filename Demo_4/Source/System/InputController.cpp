#include "DemoStableHeaders.h"
#include "System\InputController.h"
#include "Interface\InputInterface.h"

namespace Demo
{
namespace System
{
//------------------------------------------------------------
InputController::InputController()
	: mpInputManager(0)
	, mpMouse(0)
	, mpKeyboard(0)
	, mpJoystick(0)
	, mpRenderWindow(0)
{
	keyboardCaptureDel.bind(this, &InputController::deviceInvalidCapture);
	mouseCaptureDel.bind(this, &InputController::deviceInvalidCapture);
}
//------------------------------------------------------------
/*virtual*/ InputController::~InputController()
{
	CloseController();
}
//------------------------------------------------------------
/*virtual*/ void InputController::OperateController()
{
	keyboardCaptureDel();
	mouseCaptureDel();
}
//------------------------------------------------------------
/*virtual*/ void InputController::CloseController()
{
	Core::Controller::CloseController();

	Ogre::WindowEventUtilities::removeWindowEventListener(mpRenderWindow, this);
	
	destroyInputSystemAndDevices();

	mpRenderWindow = 0;

	mouseListener.reset();
	keyboardListener.reset();
	joystickListener.reset();
}
//------------------------------------------------------------
/*virtual*/ void InputController::SetActiveController( bool active )
{
	Controller::SetActiveController(active);
	if(active)
	{
		if(mpKeyboard) keyboardCaptureDel.bind(this, &InputController::keyboardValidCapture);
		if(mpMouse) mouseCaptureDel.bind(this, &InputController::mouseValidCapture);
	}
	else
	{
		keyboardCaptureDel.bind(this, &InputController::deviceInvalidCapture);
		mouseCaptureDel.bind(this, &InputController::deviceInvalidCapture);
	}
}
//------------------------------------------------------------
/*virtual*/ void InputController::windowResized( Ogre::RenderWindow *rw )
{
	if( rw == mpRenderWindow && mpMouse )
	{
		const OIS::MouseState &ms = mpMouse->getMouseState();
		ms.width = rw->getWidth();
		ms.height = rw->getHeight();
	}
}
//------------------------------------------------------------
/*virtual*/ void InputController::windowClosed( Ogre::RenderWindow *rw )
{
	if( rw == mpRenderWindow )
	{
		destroyInputSystemAndDevices();
	}
}
//------------------------------------------------------------
bool InputController::initialize( Ogre::RenderWindow *rw )
{
	if( isInitializedController() ) return false;

	if( mpInputManager ) return false;
	if( !rw ) return false;
	mpRenderWindow = rw;

	if(!createInputSystemAndDevices()) return false;
	createBindListeners();
	
	mpInputInterface.reset( new InputInterface(mpMouse, mpKeyboard) );

	if( !prepareInputIDs() )return false;

	Ogre::WindowEventUtilities::addWindowEventListener(mpRenderWindow, this);
	windowResized(mpRenderWindow);

	setInitializeController();
	return true;
}
//------------------------------------------------------------
bool InputController::createInputSystemAndDevices()
{
	std::size_t hwnd = 0;
	mpRenderWindow->getCustomAttribute( "WINDOW", &hwnd );

	try
	{
		mpInputManager = OIS::InputManager::createInputSystem( hwnd );
	}
	catch(...){ mpInputManager = 0; return false; }

	try
	{
		mpKeyboard = static_cast< OIS::Keyboard* >( mpInputManager->createInputObject( OIS::OISKeyboard, true ) );
	}
	catch(...){ mpKeyboard = 0; return false; }

	try
	{
		mpMouse = static_cast< OIS::Mouse* >( mpInputManager->createInputObject( OIS::OISMouse, true ) ); 
	}
	catch(...){ mpMouse = 0; return false; }

	/*
	try{ mpJoystick = static_cast< OIS::JoyStick* >( mpInputManager->createInputObject( OIS::OISJoyStick, true ) ); }
	catch(...){ mpJoystick = 0; }
	*/

	//Binds device delegates
	SetActiveController(isActiveController());

	return true;
}
//------------------------------------------------------------
void InputController::destroyInputSystemAndDevices()
{
	if(mpInputManager)
	{
		if(mpMouse)
		{
			mpInputManager->destroyInputObject( mpMouse );
			mpMouse = 0;
			mouseCaptureDel.bind(this, &InputController::deviceInvalidCapture);
		}
		if(mpKeyboard)
		{
			mpInputManager->destroyInputObject( mpKeyboard );
			mpKeyboard = 0;
			keyboardCaptureDel.bind(this, &InputController::deviceInvalidCapture);
		}
		
		OIS::InputManager::destroyInputSystem( mpInputManager );
		mpInputManager = 0;
	}
}
//------------------------------------------------------------
void InputController::createBindListeners()
{
	if(!keyboardListener) keyboardListener.reset(new KeyboardListener);
	if(!mouseListener) mouseListener.reset(new MouseListener);

	if(mpKeyboard) mpKeyboard->setEventCallback(keyboardListener.get());
	if(mpMouse) mpMouse->setEventCallback(mouseListener.get());
}
//------------------------------------------------------------
bool InputController::prepareInputIDs()
{
	typedef input_traits it;
	auto add = [this]( it::INPUT_ID id, const InputIdData &idata ) {
		this->idCont[id] = idata;
	};

	InputIdData idata;

	//Mouse
	idata.type = OIS::OISMouse;
	idata.componentType = OIS::OIS_Axis;
	add( it::IID_MOUSE_MOVED, idata );

	idata.componentType = OIS::OIS_Button;
	idata.code = OIS::MB_Left;
	add( it::IID_MOUSE_LBUTTON, idata );

	idata.code = OIS::MB_Right;
	add( it::IID_MOUSE_RBUTTON, idata );

	idata.code = OIS::MB_Middle;
	add( it::IID_MOUSE_MBUTTON, idata );

	//Keyboard
	idata.type = OIS::OISKeyboard;
	idata.componentType = OIS::OIS_Button;

	idata.code = OIS::KC_W;
	add(it::IID_MOVE_FORWARD, idata);

	idata.code = OIS::KC_S;
	add(it::IID_MOVE_BACK, idata);

	idata.code = OIS::KC_A;
	add(it::IID_MOVE_LEFT, idata);

	idata.code = OIS::KC_D;
	add(it::IID_MOVE_RIGHT, idata);

	//EscapeKey
	idata.code = OIS::KC_ESCAPE;
	add(it::IID_ESCAPE_KEY, idata);

	//DriverControls
	idata.code = OIS::KC_UP;
	add( it::IID_VEHICLE_DRIVER_THROTTLE_BUTTON, idata );

	idata.code = OIS::KC_DOWN;
	add( it::IID_VEHICLE_DRIVER_BRAKE_BUTTON, idata );

	idata.code = OIS::KC_LEFT;
	add( it::IID_VEHICLE_DRIVER_STEER_LEFT_BUTTON, idata );

	idata.code = OIS::KC_RIGHT;
	add( it::IID_VEHICLE_DRIVER_STEER_RIGHT_BUTTON, idata );

	idata.code = OIS::KC_X;
	add( it::IID_VEHICLE_DRIVER_HANDBRAKE_BUTTON, idata );

	idata.code = OIS::KC_A;
	add( it::IID_VEHICLE_DRIVER_SHIFT_UP_BUTTON, idata );

	idata.code = OIS::KC_Z;
	add( it::IID_VEHICLE_DRIVER_SHIFT_DOWN_BUTTON, idata );
	
	return true;
}
//------------------------------------------------------------
bool InputController::addInputDel(InputRequest const &r)
{
	traits::id_cont::iterator it = idCont.find(r.id);
	if( it == idCont.end() ) return false;

	switch( it->second.type )
	{
	case OIS::OISKeyboard:
		if(keyboardListener) {
			return _addButtonDel( *keyboardListener, r.state, it->second.code, r.del );
		}
		break;
	case OIS::OISMouse:
		if(mouseListener) {
			switch( it->second.componentType ) 
			{
			case OIS::OIS_Button:
				return _addButtonDel( *mouseListener, r.state, it->second.code, r.del );
			case OIS::OIS_Axis:
				mouseListener->mouseMovedComp.add(r.del); return true;
			default: return false;
			}
		}
		break;
	case OIS::OISJoyStick:
		if(joystickListener) {
			switch( it->second.componentType )
			{
			case OIS::OIS_Button:
				return _addButtonDel( *joystickListener, r.state, it->second.code, r.del );
			default:
				return false;
			}
		}
		break;
	default: return false;
	}
	return false;
}
//------------------------------------------------------------
void InputController::removeInputDel(InputRequest const &r)
{
	InputIdData idData;
	if( r.id != input_traits::IID_NOT_DEFINED ) {
		traits::id_cont::iterator it = idCont.find(r.id);
		if( it != idCont.end() )
			idData = it->second;
	}

	switch(idData.type)
	{
	case OIS::OISKeyboard:
		if(keyboardListener) { 
			_removeButtonDel( *keyboardListener, r.state, idData.code, r.del );
		}
		return;
	case OIS::OISMouse:
		if(mouseListener) {
			switch(idData.componentType)
			{
			case OIS::OIS_Button:
				_removeButtonDel( *mouseListener, r.state, idData.code, r.del );
				return;
			case OIS::OIS_Axis:
				mouseListener->mouseMovedComp.remove(r.del);
				return;
			default:
				_removeButtonDel( *mouseListener, r.state, idData.code, r.del );
				mouseListener->mouseMovedComp.remove(r.del);
				return;
			}
		}
		break;
	case OIS::OISJoyStick:
		if(joystickListener) {
			switch(idData.componentType)
			{
			case OIS::OIS_Button:
				_removeButtonDel( *joystickListener, r.state, idData.code, r.del );
				return;
			}
		}
		break;
	default:
		if(keyboardListener) {
			_removeButtonDel( *keyboardListener, r.state, idData.code, r.del );
		}
		if(mouseListener) {
			_removeButtonDel( *mouseListener, r.state, idData.code, r.del );
			mouseListener->mouseMovedComp.remove(r.del);
		}
		if(joystickListener) {
			_removeButtonDel( *joystickListener, r.state, idData.code, r.del );
		}
	}
}
//------------------------------------------------------------
}
}
#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "AutoGearbox.h"
#include "IGearboxControl.h"
#include "IGearboxCallback.h"
#include "IGearboxState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	void AutoGearbox::CloseMechanics()
	{
		shared_ptr_reset(iControl);
		shared_ptr_reset(iCallback);
		shared_ptr_reset(iState);
		Gearbox::CloseMechanics();
	}
	//------------------------------------------------------------
	bool AutoGearbox::Initialize( const GearboxInitData& data )
	{
		return true;
	}
	//------------------------------------------------------------
	IAutoGearboxControl::shared_ptr AutoGearbox::GetControlInterface()
	{
		if( shared_ptr_null(iControl) ) iControl.bind( new IAutoGearboxControl( *this ) );
		return iControl;
	}
	//------------------------------------------------------------
	IAutoGearboxCallback::shared_ptr AutoGearbox::GetCallbackInterface()
	{
		if( shared_ptr_null(iCallback) ) iCallback.bind( new IAutoGearboxCallback( *this ) );
		return iCallback;
	}
	//------------------------------------------------------------
	IAutoGearboxState::shared_ptr AutoGearbox::GetStateInterface()
	{
		if( shared_ptr_null(iState) ) iState.bind( new IAutoGearboxState( *this ) );
		return iState;
	}
	//------------------------------------------------------------
	IGearbox::shared_ptr AutoGearbox::GetInterface( GEARBOX_INTERFACE_TYPE type )
	{
		switch( type )
		{
		case GIT_CONTROL:return GetControlInterface();
		case GIT_CALLBACK:return GetCallbackInterface();
		case GIT_STATE:return GetStateInterface();
		default:return IGearbox::shared_ptr();
		}
	}
	//------------------------------------------------------------
	IPedal::shared_ptr AutoGearbox::GetClutchInterface( PEDAL_INTERFACE_TYPE type )
	{
		switch( type )
		{
		case PIT_PCALLBACK:return GetClutchCallbackInterface();
		case PIT_PSTATE:return GetClutchStateInterface();
		default:return IPedal::shared_ptr();
		}
	}
	//------------------------------------------------------------
	float AutoGearbox::OperateMechanics(float t, float d)
	{
		torque = t;
		drag = d;
		return radPS;
	}
	//------------------------------------------------------------
	void AutoGearbox::DisconnectChildMech(int slotIndex)
	{
	}
	//------------------------------------------------------------
	void AutoGearbox::DisconnectChildMech(traits::child_mech_in child)
	{
	}
	//------------------------------------------------------------
	bool AutoGearbox::ConnectChildMech( traits::child_mech_in, int slotIndex )
	{
		return true;
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE AutoGearbox::GetActiveMode() const
	{
		return AGM_NOT_DEFINED;
	}
	//------------------------------------------------------------
	int AutoGearbox::GetAvailableModes() const
	{
		return 0;
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE AutoGearbox::GetModeAlt() const
	{
		return AGM_NOT_DEFINED;
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE AutoGearbox::GetModeUp()const
	{
		return AGM_NOT_DEFINED;
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE AutoGearbox::GetModeDown() const
	{
		return AGM_NOT_DEFINED;
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE AutoGearbox::GetTargetMode() const
	{
		return AGM_NOT_DEFINED;
	}
	//------------------------------------------------------------
	bool AutoGearbox::ModeAlt()
	{
		return false;
	}
	//------------------------------------------------------------
	bool AutoGearbox::ModeUp()
	{
		return false;
	}
	//------------------------------------------------------------
	bool AutoGearbox::ModeDown()
	{
		return false;
	}
	//------------------------------------------------------------
	bool AutoGearbox::SwitchMode( AUTO_GEARBOX_MODE mode )
	{
		return false;
	}
	//------------------------------------------------------------
	bool AutoGearbox::PrepareMode( AUTO_GEARBOX_MODE mode )
	{
		return false;
	}
	//------------------------------------------------------------
	bool AutoGearbox::PrepareModeAlt()
	{
		return false;
	}
	//------------------------------------------------------------
	bool AutoGearbox::PrepareModeUp()
	{
		return false;
	}
	//------------------------------------------------------------
	bool AutoGearbox::PrepareModeDown()
	{
		return false;
	}
	//------------------------------------------------------------
	void AutoGearbox::UndoPrepareMode()
	{
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
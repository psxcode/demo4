#pragma once
#include "Gearbox.h"
#include "IGearboxControl.h"
#include "IGearboxCallback.h"
#include "IGearboxState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class AutoGearbox : public Gearbox
	{
	public:
		DEFINE_CLASS_SHARED_PTR(AutoGearbox)
		//--------------------------------//
		AutoGearbox()
			: Gearbox( GT_AUTOMATIC )
		{
		}
		virtual ~AutoGearbox(){CloseMechanics();}

		//MechanicsInterface
		virtual void CloseMechanics();
		virtual float OperateMechanics( float, float );
		virtual bool ConnectChildMech( traits::child_mech_in child, int slotIndex = 0 );
		virtual void DisconnectChildMech( int slotIndex = 0 );
		virtual void DisconnectChildMech( traits::child_mech_in child );
		virtual IMechanicsState::shared_ptr GetMechanicsStateInterface(){ return GetStateInterface(); }
		//MechanicsInterface

		//интерфейс класса VehicleGearbox
		virtual IGearbox::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE );
		virtual IPedal::shared_ptr GetClutchInterface( PEDAL_INTERFACE_TYPE );
		//интерфейс класса VehicleGearbox

		//интерфейс для IAutoGearboxControl
		bool SwitchMode( AUTO_GEARBOX_MODE );
		bool ModeUp();
		bool ModeDown();
		bool ModeAlt();
		bool PrepareModeUp();
		bool PrepareModeDown();
		bool PrepareModeAlt();
		bool PrepareMode( AUTO_GEARBOX_MODE );
		void UndoPrepareMode();
		//интерфейс для IAutoGearboxControl

		//интерфейс для IAutoGearboxCallback
		inline void AddSwitchInitiatedOnceDel( traits::del_in del ){ switchInitiatedOnceCall.add( del ); }
		inline void AddSwitchInitiatedTempDel( traits::del_in del ){ switchInitiatedTempCall.add( del ); }
		inline void AddSwitchCompletedOnceDel( traits::del_in del ){ switchCompletedOnceCall.add( del ); }
		inline void AddSwitchCompletedTempDel( traits::del_in del ){ switchCompletedTempCall.add( del ); }
		inline void AddSwitchFailedOnceDel( traits::del_in del ){ switchFailedOnceCall.add( del ); }
		inline void AddSwitchFailedTempDel( traits::del_in del ){ switchFailedTempCall.add( del ); }

		inline void RemoveSwitchInitiatedOnceDel( traits::del_in del ){ switchInitiatedOnceCall.remove( del ); }
		inline void RemoveSwitchInitiatedTempDel( traits::del_in del ){ switchInitiatedTempCall.remove( del ); }
		inline void RemoveSwitchCompletedOnceDel( traits::del_in del ){ switchCompletedOnceCall.remove( del ); }
		inline void RemoveSwitchCompletedTempDel( traits::del_in del ){ switchCompletedTempCall.remove( del ); }
		inline void RemoveSwitchFailedOnceDel( traits::del_in del ){ switchFailedOnceCall.remove( del ); }
		inline void RemoveSwitchFailedTempDel( traits::del_in del ){ switchFailedTempCall.remove( del ); }
		//интерфейс для IAutoGearboxCallback		

		//интерфейс для IAutoGearboxState
		AUTO_GEARBOX_MODE GetModeUp()const;
		AUTO_GEARBOX_MODE GetModeDown()const;
		AUTO_GEARBOX_MODE GetModeAlt()const;

		int GetAvailableModes()const;
		AUTO_GEARBOX_MODE GetActiveMode()const;//AGM_NOT_DEFINED no active mode at this time
		AUTO_GEARBOX_MODE GetTargetMode()const;//AGM_NOT_DEFINED no target mode
		//интерфейс для IAutoGearboxState

		bool Initialize( const GearboxInitData& );
		IAutoGearboxControl::shared_ptr GetControlInterface();
		IAutoGearboxCallback::shared_ptr GetCallbackInterface();
		IAutoGearboxState::shared_ptr GetStateInterface();
		inline IPedalCallback::shared_ptr GetClutchCallbackInterface(){ return clutch.GetCallbackInterface(); }
		inline IPedalState::shared_ptr GetClutchStateInterface(){ return clutch.GetStateInterface(); }

	protected:
		IAutoGearboxControl::shared_ptr iControl;
		IAutoGearboxCallback::shared_ptr iCallback;
		IAutoGearboxState::shared_ptr iState;

		delegate_traits::composite0 switchInitiatedOnceCall;
		delegate_traits::composite0 switchInitiatedTempCall;
		delegate_traits::composite0 switchCompletedOnceCall;
		delegate_traits::composite0 switchCompletedTempCall;
		delegate_traits::composite0 switchFailedOnceCall;
		delegate_traits::composite0 switchFailedTempCall;
	};
	//------------------------------------------------------------
}
}
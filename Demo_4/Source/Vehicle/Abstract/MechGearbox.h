#pragma once

#include "Gearbox.h"
#include "Core\DelTimer.h"
#include "IGearboxControl.h"
#include "IGearboxCallback.h"
#include "IGearboxState.h"


namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class MechGearbox : public Gearbox
	{
	public:
		DEFINE_CLASS_SHARED_PTR(MechGearbox)
		//--------------------------------//
		/*
		Переменная используется при переключении передачи
		Показывает значение clutchRatio, выше которого переключение невозможно
		Чтобы передача включилась clutchRatio должен быть меньше clutchLimit
		*/
		static const float clutchLimit;
		//--------------------------------//
		MechGearbox()
			: Gearbox( GT_MECHANICAL )
			, isShiftInitiated( false )
		{
		}
		virtual ~MechGearbox() { CloseMechanics(); }

		//MechanicsInterface
		virtual void CloseMechanics();
		virtual float OperateMechanics( float, float );
		virtual bool ConnectChildMech( traits::child_mech_in child, int slotIndex = 0 );
		virtual void DisconnectChildMech( int slotIndex = 0 );
		virtual void DisconnectChildMech( traits::child_mech_in child );
		virtual IMechanicsState::shared_ptr GetMechanicsStateInterface() { return GetStateInterface(); }
		//MechanicsInterface
		
		//интерфейс класса VehicleGearbox
		virtual IGearbox::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE );
		virtual IPedal::shared_ptr GetClutchInterface( PEDAL_INTERFACE_TYPE );
		//интерфейс класса VehicleGearbox

		//интерфейс для IMechGearboxControl
		bool ShiftToNeutral();
		bool ShiftToReverse( int gearIndex = 1 );//1 is first reverse, //2 is second reverse
		bool ShiftTo( int gearIndex );//0 is neutral, //negatives are reverse, //positives are direct
		//интерфейс для IMechGearboxControl

		

		bool Initialize( const GearboxInitData& );
		IMechGearboxCallback::shared_ptr GetCallbackInterface();
		IMechGearboxControl::shared_ptr GetControlInterface();
		IMechGearboxState::shared_ptr GetStateInterface();
		inline IPedalControl::shared_ptr GetClutchControlInterface(){ return clutch.GetControlInterface(); }
		inline IPedalCallback::shared_ptr GetClutchCallbackInterface(){ return clutch.GetCallbackInterface(); }
		inline IPedalState::shared_ptr GetClutchStateInterface(){ return clutch.GetStateInterface(); }

	protected:
		void operateConnectionChanged();

		void OperateWithChildNoConnection();
		void OperateWithFullConnection();
		void OperateNoConnection();

		void CallShiftInitiated();
		void CallShiftCompleted();
		void CallShiftFailed();
		void CalcActiveRatio();
		bool CheckGear( int gearIndex );
		void OnShiftOp();

		IMechGearboxControl::shared_ptr iControl;
		IMechGearboxState::shared_ptr iState;
		IMechGearboxCallback::shared_ptr iCallback;

		Core::DelTimer shiftOpTimer;
		bool isShiftInitiated;

		delegate_traits::del0<void>::shared_ptr shiftOpDel;
		delegate_traits::del0<void>::type operateDel;
	};
	//------------------------------------------------------------
}
}
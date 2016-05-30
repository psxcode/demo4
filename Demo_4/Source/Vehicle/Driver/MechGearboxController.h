#pragma once

#include "Vehicle\Abstract\RadPSController.h"
#include "Vehicle\Abstract\IPedalControl.h"
#include "Vehicle\Abstract\IPedalState.h"
#include "Vehicle\Abstract\IPedalCallback.h"
#include "Vehicle\Abstract\SpeedToGearCalculator.h"

#include "GearboxController.h"
#include "IGearboxControllerState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class MechGearboxController : public GearboxController
	{
	public:
		DEFINE_CLASS_SHARED_PTR(MechGearboxController)
		//--------------------------------//
		struct MechGearboxControllerInitData : public GearboxController::GearboxControllerInitData
		{
			virtual bool isValid()const
			{
				return !iGearboxControl.isNull() && GearboxControllerInitData::isValid();
			}

			SpeedToGearCalculator::const_shared_ptr speedToGear;//для функции ShiftDirect и ShiftReverse
			MotorChartRadPSController::shared_ptr radPSController;//зона для помощи юзеру
			IPedalControl::shared_ptr iClutchControl;//чтобы клутчить удобно было
			IMechGearboxControl::shared_ptr iGearboxControl;
		};
		//--------------------------------//
		static const int gearboxRequestTimesLimit;
		//--------------------------------//
		MechGearboxController()
			: targetGear(0)
			, lastGear(0)
			, gearboxRequestTimes(0)
			, isShiftInitiated( false )
			, isShiftLastStage( false )
		{
		}
		virtual ~MechGearboxController(){ CloseMechGearboxController(); }

		//GearboxController interface
		virtual bool IsNeutral()const;
		virtual bool IsDirect()const;
		virtual bool IsReverse()const;

		virtual bool ShiftUp();
		virtual bool ShiftDown();
		virtual bool ShiftToDirect();
		virtual bool ShiftToNeutral();
		virtual bool ShiftToReverse();

		virtual IGearboxController::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE );

		virtual void CloseGearboxController(){ CloseMechGearboxController(); }
		//GearboxController interface
		
		//Controller interface
		virtual void OperateController();
		//Controller interface

		bool Initialize( const MechGearboxControllerInitData& data );
		void CloseMechGearboxController();

		IMechGearboxControllerState::shared_ptr GetStateInterface();

		inline int getLastGear()const{ return lastGear; }
		inline int getTargetGear()const{ return targetGear; }
		
	protected:
		//Когда всё закончится
		void OnShiftEnded();

		//Зона
		void OnRpmBelowNorm();
		void OnRpmAboveNorm();
		void OnRpmSpeedInc();
		void OnRpmSpeedDec();

		//Нога нажала сцепление
		void OnClutchPressed();
		void OnClutchReleased();

		//Срабатывание таймеров
		void OnShiftDelayTimer();
		void OnShiftToNeutralTimer();
		void OnShiftOpTimer();

		//Проверка Гира на вменяемость
		int GetDirectGearBySpeed();
		int GetReverseGearBySpeed();

		//Начало переключения
		void InitiateShift();
		void CancelShift();

		void ShiftTo( int gear );

		//Вкл\Выкл для делегатов сидящих на зоне
		void ActivateRpmDels( bool activate = true );

		//рычаг коробки
		IMechGearboxControl::shared_ptr iGControl;
		IMechGearboxCallback::shared_ptr iGCallback;
		IMechGearboxState::shared_ptr iGState;

		IPedalControl::shared_ptr iClutchControl;
		IPedalCallback::shared_ptr iClutchCallback;
		MotorChartRadPSController::shared_ptr radPSController;
		SpeedToGearCalculator::const_shared_ptr speedToGear;

		//делегация..
		delegate_traits::active_del0<void>::shared_ptr gearRpmBelowNormDel;
		delegate_traits::active_del0<void>::shared_ptr gearRpmAboveNormDel;
		delegate_traits::active_del0<void>::shared_ptr gearRpmSpeedIncDel;
		delegate_traits::active_del0<void>::shared_ptr gearRpmSpeedDecDel;
		delegate_traits::del0<void>::shared_ptr clutchPressedDel;
		delegate_traits::del0<void>::shared_ptr clutchReleasedDel;
		delegate_traits::active_del0<void>::shared_ptr shiftDelayDel;
		delegate_traits::active_del0<void>::shared_ptr shiftToNeutralDel;
		delegate_traits::active_del0<void>::shared_ptr shiftOpDel;
		delegate_traits::del0<void>::shared_ptr shiftEndedDel;

		//Таймеры
		Core::DelTimer shiftDelayTimer;
		Core::DelTimer shiftOpTimer;

		//целевая передача
		int targetGear;

		//текущая передача
		int lastGear;

		int gearboxRequestTimes;

		//флажки
		bool isShiftInitiated;//типа начали
		bool isShiftLastStage;//типа финальная стадия

		IMechGearboxControllerState::shared_ptr iState;
	};
	//------------------------------------------------------------
}
}
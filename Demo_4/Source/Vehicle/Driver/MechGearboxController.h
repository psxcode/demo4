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

			SpeedToGearCalculator::const_shared_ptr speedToGear;//��� ������� ShiftDirect � ShiftReverse
			MotorChartRadPSController::shared_ptr radPSController;//���� ��� ������ �����
			IPedalControl::shared_ptr iClutchControl;//����� �������� ������ ����
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
		//����� �� ����������
		void OnShiftEnded();

		//����
		void OnRpmBelowNorm();
		void OnRpmAboveNorm();
		void OnRpmSpeedInc();
		void OnRpmSpeedDec();

		//���� ������ ���������
		void OnClutchPressed();
		void OnClutchReleased();

		//������������ ��������
		void OnShiftDelayTimer();
		void OnShiftToNeutralTimer();
		void OnShiftOpTimer();

		//�������� ���� �� �����������
		int GetDirectGearBySpeed();
		int GetReverseGearBySpeed();

		//������ ������������
		void InitiateShift();
		void CancelShift();

		void ShiftTo( int gear );

		//���\���� ��� ��������� ������� �� ����
		void ActivateRpmDels( bool activate = true );

		//����� �������
		IMechGearboxControl::shared_ptr iGControl;
		IMechGearboxCallback::shared_ptr iGCallback;
		IMechGearboxState::shared_ptr iGState;

		IPedalControl::shared_ptr iClutchControl;
		IPedalCallback::shared_ptr iClutchCallback;
		MotorChartRadPSController::shared_ptr radPSController;
		SpeedToGearCalculator::const_shared_ptr speedToGear;

		//���������..
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

		//�������
		Core::DelTimer shiftDelayTimer;
		Core::DelTimer shiftOpTimer;

		//������� ��������
		int targetGear;

		//������� ��������
		int lastGear;

		int gearboxRequestTimes;

		//������
		bool isShiftInitiated;//���� ������
		bool isShiftLastStage;//���� ��������� ������

		IMechGearboxControllerState::shared_ptr iState;
	};
	//------------------------------------------------------------
}
}
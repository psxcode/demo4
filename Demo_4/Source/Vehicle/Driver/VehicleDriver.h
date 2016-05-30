#pragma once
#include "Vehicle\Abstract\PedalFilter.h"
#include "GearboxController.h"
#include "Vehicle\Abstract\VehicleToDriverData.h"
#include "Vehicle\Abstract\RadPSController.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class VehicleDriver : public Core::Controller
	{
	public:
		DEFINE_CLASS_SHARED_PTR(VehicleDriver)
		//--------------------------------//
		struct InitData
		{
			bool isValid()const{ return driverTimingCalc != 0 && mcmCalc != 0 && vtdData !=0 && vtdData->isValid(); }

			IPedalState::shared_ptr iDriveStyleState;
			DriverTimingCalculator::const_shared_ptr driverTimingCalc;
			MotorChartMarksCalculator::shared_ptr mcmCalc;
			VehicleToDriverData::const_shared_ptr vtdData;
		};
		struct CreateData
		{
			CreateData()
				: driveOptions(0)
			{
			}

			bool isValid()const{
				return vtdData != 0 && vtdData->isValid();
			}

			int driveOptions;
			IPedalState::shared_ptr iDriveStyleState;
			VehicleToDriverData::const_shared_ptr vtdData;
		};
		//--------------------------------//
		static VehicleDriver::shared_ptr CreateVehicleDriver( const CreateData& );
		//--------------------------------//
		VehicleDriver();
		virtual ~VehicleDriver();

		virtual void OperateController();

		bool Initialize( const InitData& );
		void CloseVehicleDriver();

		void ApplyDriveOptions( int options );
		bool CreateUserInput();
		void RemoveUserInput();

		inline int GetDriveOptions()const{ return driveOptions; }
		inline IGearboxController::shared_ptr GetGearboxControllerInterface( GEARBOX_INTERFACE_TYPE type )const{ return gearbox != 0 ? gearbox->GetInterface( type ) : IGearboxController::shared_ptr(); }

	protected:
#pragma region driveStyle
		//driveStyle////////////////////////
		//--------------------------------//
		IPedalState::shared_ptr iDriveStyleState;
		//--------------------------------//
#pragma endregion

#pragma region zone
		//zone//////////////////////////////
		//--------------------------------//
		void OnAboveMax();
		void OnAboveNorm();
		void OnNorm();
		void OnBelowNorm();
		void OnBelowMin();
		void OnRpmSpeed();

		BaseRadPSTimingCalculator::shared_ptr radPSTC;
		MotorChartRadPSController::shared_ptr radPSController;
		MotorChartMarksCalculator::shared_ptr mcmCalc;
		//--------------------------------//
#pragma endregion

		//simple_controls///////////////////
		//--------------------------------//
		void OnGoPress();
		void OnStopPress();
		void OnGoStopRelease();

		delegate_traits::del0<void>::shared_ptr goPressDel;
		delegate_traits::del0<void>::shared_ptr goStopReleaseDel;
		delegate_traits::del0<void>::shared_ptr stopPressDel;
		bool userGoPress;
		bool userStopPress;
		//--------------------------------//

#pragma region throttle
		//throttle//////////////////////////
		//--------------------------------//
		bool CreateThrottleAssist();
		void RemoveThrottleAssist();

		void OnThrottleAxisMoved();
		void OnThrottlePressEngineOnAssistOn();
		void OnThrottlePressEngineOnAssistOff();
		void OnThrottlePressEngineOff();
		void OnThrottleReleaseEngineOnAssistOn();
		void OnThrottleReleaseEngineOnAssistOff();
		void OnThrottleReleaseEngineOff();
		void ActivateThrottleInputDels( bool activate = true );
		void ActivateThrottleRpmDels( bool activate = true );

		void OnThrottleRpmNorm();
		void OnThrottleRpmAboveNorm();
		void OnThrottleRpmAboveMax();
		void OnThrottleRpmSpeedInc();

		IPedalControl::shared_ptr iThrottleControl;
		IPedalCallback::shared_ptr iThrottleCallback;
		IPedalState::shared_ptr iThrottleState;
		
		delegate_traits::active_del0<void>::shared_ptr throttlePressDel;
		delegate_traits::active_del0<void>::shared_ptr throttleReleaseDel;
		delegate_traits::active_del0<void>::shared_ptr throttleRpmNormDel;
		delegate_traits::active_del0<void>::shared_ptr throttleRpmAboveNormDel;
		delegate_traits::active_del0<void>::shared_ptr throttleRpmAboveMaxDel;
		delegate_traits::active_del0<void>::shared_ptr throttleRpmSpeedIncDel;
		bool userThrottlePress;
		//--------------------------------//
#pragma endregion

#pragma region clutch
		//clutch////////////////////////////
		//--------------------------------//
		bool CreateClutchAssist();
		void RemoveClutchAssist();

		void OnClutchAxisMoved();
		void OnClutchPress();
		void OnClutchReleaseAssistOn();
		void OnClutchReleaseAssistOff();
		void ActivateClutchInputDels( bool activate = true );
		void ActivateClutchRpmDels( bool activate = true );

		void OnClutchRpmBelowMin();
		void OnClutchRpmBelowNorm();
		void OnClutchRpmNorm();
		//void OnClutchRpmAboveMax();
		//void OnClutchRpmSpeedInc();
		//void OnClutchRpmSpeedDec();

		IPedalControl::shared_ptr iClutchControl;
		IPedalCallback::shared_ptr iClutchCallback;
		IPedalState::shared_ptr iClutchState;

		delegate_traits::active_del0<void>::shared_ptr clutchAxisDel;
		delegate_traits::active_del0<void>::shared_ptr clutchPressDel;
		delegate_traits::active_del0<void>::shared_ptr clutchReleaseDel;

		//vdel_shptr clutchRpmSpeedIncDel;
		//vdel_shptr clutchRpmSpeedDecDel;
		delegate_traits::active_del0<void>::shared_ptr clutchRpmBelowMinDel;
		delegate_traits::active_del0<void>::shared_ptr clutchRpmBelowNormDel;
		delegate_traits::active_del0<void>::shared_ptr clutchRpmNormDel;
		//vdel_shptr clutchRpmAboveMaxDel;
		
		Core::DelTimer clutchSmPressZoneTimer;
		delegate_traits::active_del0<void>::shared_ptr clutchSmPressZoneTimerDel;
		void OnClutchSmPressZoneTimer();

		bool userClutchPress;
		Core::puFloat userClutchAxisValue;
		//--------------------------------//
#pragma endregion

#pragma region brake
		//brake/////////////////////////////
		//--------------------------------//
		void OnBrakeAxisMoved();
		void OnBrakePress();
		void OnBrakeRelease();

		IPedalControl::shared_ptr iBrakeControl;
		IPedalCallback::shared_ptr iBrakeCallback;
		IPedalState::shared_ptr iBrakeState;

		delegate_traits::del0<void>::shared_ptr brakePressDel;
		delegate_traits::del0<void>::shared_ptr brakeReleaseDel;
		bool userBrakePress;
		//--------------------------------//
#pragma endregion

#pragma region steer
		//steer/////////////////////////////
		//--------------------------------//
		void OnSteerAxisMoved();
		void OnSteerPressLeft();
		void OnSteerPressRight();
		void OnSteerRelease();

		ISteerControl::shared_ptr iSteerControl;
		ISteerCallback::shared_ptr iSteerCallback;
		ISteerState::shared_ptr iSteerState;

		delegate_traits::del0<void>::shared_ptr steerPressLeftDel;
		delegate_traits::del0<void>::shared_ptr steerPressRightDel;
		delegate_traits::del0<void>::shared_ptr steerReleaseDel;
		bool userSteerPress;
		//--------------------------------//
#pragma endregion

#pragma region handbrake
		//handbrake/////////////////////////
		//--------------------------------//
		void OnHandbrakePress();
		void OnHandbrakeRelease();

		IPedalControl::shared_ptr iHandbrakeControl;
		IPedalCallback::shared_ptr iHandbrakeCallback;
		IPedalState::shared_ptr iHandbrakeState;

		delegate_traits::del0<void>::shared_ptr handbrakePressDel;
		delegate_traits::del0<void>::shared_ptr handbrakeReleaseDel;
		bool userHandbrakePress;
		//--------------------------------//
#pragma endregion

#pragma region dsGearbox
		//dsGearbox///////////////////////////
		//--------------------------------//
		bool CreateGearboxController();
		void CloseGearboxController();

		void OnShiftUpPress();
		void OnShiftDownPress();
		void OnShiftStarted();
		void OnShiftEnded();

		IGearboxControl::shared_ptr iGearboxControl;
		delegate_traits::del0<void>::shared_ptr shiftUpPressDel;
		delegate_traits::del0<void>::shared_ptr shiftDownPressDel;
		delegate_traits::del0<void>::shared_ptr shiftStartedDel;
		delegate_traits::del0<void>::shared_ptr shiftEndedDel;
		GearboxController::shared_ptr gearbox;
		//--------------------------------//
#pragma endregion
		//Motor/////////////////////////////
		//--------------------------------//
		void OnMotorStall();
		void OnMotorStart();

		delegate_traits::del0<void>::shared_ptr motorStallDel;
		delegate_traits::del0<void>::shared_ptr motorStartDel;

		IMotorControl::shared_ptr iMotorControl;
		IMotorState::shared_ptr iMotorState;
		IMotorCallback::shared_ptr iMotorCallback;
		//--------------------------------//

		int driveOptions;
		int controlAxes;
		int controlSystems;

		ISpeedometerState::shared_ptr speedometer;
		SpeedToGearCalculator::const_shared_ptr speedToGear;
		DriverTimingCalculator::const_shared_ptr driverTimingCalc;
	};
	//------------------------------------------------------------
}//namespace Vehicle
}//namespace Demo
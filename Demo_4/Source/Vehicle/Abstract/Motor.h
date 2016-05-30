#pragma once
#include "MotorParams.h"
#include "IPedalState.h"
#include "IGearboxState.h"
#include "IGearboxCallback.h"
#include "IMotorControl.h"
#include "IMotorCallback.h"
#include "IMotorState.h"
#include "Mechanics.h"
#include "Flywheel.h"
#include "RadPSController.h"


namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class Motor : public Mechanics
	{
	public:
		DEFINE_CLASS_SHARED_PTR(Motor)
		//--------------------------------//
		enum RADPS_ID
		{
			RID_STALL_MARK,
			RID_START_MARK,
			RID_RED_MARK,
			RID_MAX_MARK,
			RID_STALL_IDLE_ZONE
		};
		//--------------------------------//
		struct MotorInitData
		{
			/*
			Параметры будут скопированы.
			*/
			MotorParams::const_shared_ptr motorParams;
			/*
			Состояние сцепления
			*/
			IPedalState::const_shared_ptr clutchState;
			/*
			Состояние коробки передач
			*/
			IGearboxState::const_shared_ptr gearboxState;
			IGearboxCallback::shared_ptr gearboxCallback;
		};
		//--------------------------------//
		Motor()
			: Mechanics( MT_MOTOR )
			, childMechanics(0)
			, activeStarterTorque(0)
			, maxStarterTorque(0)
			, belowIdleTorqueMult(0.0f)
			, torqueMultFunc( MotorParams::TMF_NOT_DEFINED )
			, _isStall(true)
			, _isTorqueMultActive(false)
		{
		}
		virtual ~Motor(){CloseMotor();}

		//Mechanics interface
		virtual void CloseMechanics() { CloseMotor(); }
		virtual float OperateMechanics( float, float );
		virtual MechanicsParams::shared_ptr CreateMechanicsParams()const { return CreateMotorParams(); }
		virtual bool isMechanicsChainValid()const;
		virtual IMechanicsState::shared_ptr GetMechanicsStateInterface() { return GetStateInterface(); }

		virtual bool ConnectChildMech( traits::child_mech_in, int index = 0 );
		virtual void DisconnectChildMech( int index = 0 );
		virtual void DisconnectChildMech( traits::child_mech_in child );

		virtual int GetMechSlotCount()const{ return 1; }
		virtual traits::child_mech_type GetChildMech( int = 0 ) { return childMechanics; }
		//Mechanics interface

		virtual MotorParams::shared_ptr CreateMotorParams()const;
		virtual void CloseMotor();
		virtual IMotor::shared_ptr GetInterface( MOTOR_INTERFACE_TYPE );
		IMotorControl::shared_ptr GetControlInterface();
		IMotorCallback::shared_ptr GetCallbackInterface();
		IMotorState::shared_ptr GetStateInterface();

		bool Initialize( const MotorInitData& );
				
		//IMotorControl interface
		inline void SetThrottle( const Core::puFloat& t ) { throttle = t; }
		inline void SetStarter( bool run = true ) {
			activeStarterTorque = run ? maxStarterTorque : 0;
		}
		//IMotorControl interface

		//IMotorCallback interface
		void AddStallRPMDel( traits::del_in );
		void AddStartRPMDel( traits::del_in );
		void AddRedRPMDel( traits::del_in );
		void AddMaxRPMDel( traits::del_in );
		
		void RemoveStallRPMDel( traits::del_in );
		void RemoveStartRPMDel( traits::del_in );
		void RemoveRedRPMDel( traits::del_in );
		void RemoveMaxRpmDel( traits::del_in );
		//IMotorCallback interface

		//IMotorState interface
		inline float getTorque()const{ return torque; }
		inline float getDrag()const{ return drag; }
		inline float getRadPS()const{ return radPS; }
		float getFlywheelInertia()const;
		inline float getMaxTorque()const{ return maxTorque; }
		inline float getMaxDrag()const{ return maxDrag; }
		inline const MotorChart& getChart()const{ return chart; }
		inline bool isStall()const{ return _isStall; }
		//IMotorState interface

	protected:
		void operateConnectionChanged();
		void operateNoConnection();
		void operateWithConnection();
		void operateWithConnectionNoClutch();
		void operateGearboxNeutral();
		delegate_traits::del0<void>::type operateDel;

		void torqueCalcChanged();
		void torqueCalc_MotorOn_TorqueMultOn();
		void torqueCalc_MotorOn_TorqueMultOff();
		void torqueCalc_MotorOff();
		delegate_traits::del0<void>::type torqueCalcDel;

		bool _isTorqueMultActive;
		void setTorqueMultOn();
		void setTorqueMultOff();
		delegate_traits::del0<void>::shared_ptr torqueMultOnDel;
		delegate_traits::del0<void>::shared_ptr torqueMultOffDel;
		//функции расчёта множителя крутящего момента
		void activeTorqueMultCalc_Const();
		void activeTorqueMultCalc_Linear();
		delegate_traits::del0<void>::type activeTorqueMultCalcDel;

		void onGearboxShift();
		delegate_traits::del0<void>::shared_ptr gearboxShiftDel;

		//отложенное создание radPSController и его меток
		void createRadPSController();
		void createStallMark();
		void createStartMark();
		void createRedMark();
		void createMaxMark();
		void createStallIdleZone();

		void onStallMotor();
		void onStartMotor();
		delegate_traits::del0<void>::shared_ptr stallMotorDel;
		delegate_traits::del0<void>::shared_ptr startMotorDel;

		MotorChart chart;
		float maxTorque;
		float maxStarterTorque;
		float belowIdleTorqueMult;
		MotorParams::TORQUE_MULT_FUNC torqueMultFunc;

		Flywheel::shared_ptr flywheel;

		//Выборка Момента Тяги из Оборотов
		Core::OgreRealVBV::shared_ptr torqueLookup;
		//Выборка Момента Тяги из Оборотов( В радианах в секунду )
		Core::OgreRealVBV::shared_ptr torqueRadPSLookup;
		//Выборка Момента Сопротивления из Оборотов( В радианах в секунду )
		Core::OgreRealVBV::shared_ptr dragRadPSLookup;

		//Последний участок педали газа( после всех модофикаций )
		Core::puFloat throttle;
		float activeTorqueMult;
		float activeStarterTorque;

		//Интерфейсы двигателя
		IMotorControl::shared_ptr iControl;
		IMotorState::shared_ptr iState;
		IMotorCallback::shared_ptr iCallback;

		//radPSController и метки
		RadPSController::shared_ptr radPSController;
		BaseRadPSMark::shared_ptr stallMark;
		BaseRadPSMark::shared_ptr startMark;
		BaseRadPSMark::shared_ptr redMark;
		BaseRadPSMark::shared_ptr maxMark;
		BaseRadPSZone::shared_ptr stallIdleZone;
		//Флаг остановки двигателя
		bool _isStall;
		
		//Подключение
		traits::child_mech_type childMechanics;
		/*
		Состояние сцепления
		Двигатель считает, что к нему подключается сцепление
		и использует соответствующие формулы расчёта.
		*/
		IPedalState::const_shared_ptr clutchState;
		/*
		Интерфейсы коробки передач
		При подключенной к двигателю коробки
		используются соответствующие формулы расчёта
		*/
		IGearboxState::const_shared_ptr gearboxState;
		IGearboxCallback::shared_ptr gearboxCallback;
	
		

	private:
		Motor( const Motor& );
		Motor& operator=( const Motor& );
	};
	//------------------------------------------------------------
}
}
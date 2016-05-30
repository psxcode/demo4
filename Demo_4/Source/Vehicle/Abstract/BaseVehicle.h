#pragma once
#include "Core\Controller.h"
#include "SpeedToGearCalculator.h"
#include "VehicleToDriverData.h"
#include "Motor.h"
#include "Gearbox.h"
#include "Differential.h"
#include "ThrottleControl.h"
#include "BrakeControl.h"
#include "SteerControl.h"
#include "Speedometer.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	/*
	Базовый класс для реализации логики машины
	Для PhysX будет определён субкласс PhysXVehicle
	*/
	class BaseVehicle : public Core::Controller, public Core::OsnController
	{
	public:
		DEFINE_CLASS_SHARED_PTR(BaseVehicle)
		//--------------------------------//
		virtual ~BaseVehicle()
		{
		}
		virtual IMechanicsState::shared_ptr GetMechanicsStateInterface( MECHANICS_TYPE type, int axleIndex = -1, VEHICLE_SIDE side = VS_NOT_DEFINED ) = 0;

		IMotor::shared_ptr getMotorInterface( MOTOR_INTERFACE_TYPE type) { return motor != 0 ? motor->GetInterface( type ) : IMotor::shared_ptr(0); }
		IGearbox::shared_ptr getGearboxInterface( GEARBOX_INTERFACE_TYPE type ) { return gearbox != 0 ? gearbox->GetInterface( type ) : IGearbox::shared_ptr(0); }

		IPedal::shared_ptr getSteerInterface( PEDAL_INTERFACE_TYPE type ) { return steerControl != 0 ? steerControl->GetSteerInterface( type ) : IPedal::shared_ptr(0); }
		IPedal::shared_ptr getThrottleInterface( PEDAL_INTERFACE_TYPE type ) { return throttleControl != 0 ? throttleControl->GetThrottleInterface( type ) : IPedal::shared_ptr(0); }
		IPedal::shared_ptr getBrakeInterface( PEDAL_INTERFACE_TYPE type ) { return brakeControl != 0 ? brakeControl->GetBrakeInterface( type ) : IPedal::shared_ptr(0); }
		IPedal::shared_ptr getClutchInterface( PEDAL_INTERFACE_TYPE type ) { return gearbox != 0 ? gearbox->GetClutchInterface( type ) : IPedal::shared_ptr(0); }

		SpeedToGearCalculator::const_shared_ptr getSpeedToGearCalculator()const { return speedToGear; }
		VehicleToDriverData::const_shared_ptr getVehicleToDriverData()const { return vehicleToDriver; }

		ISpeedometerState::shared_ptr GetSpeedometerStateInterface() { return speedometer.GetStateInterface(); }

		inline float getMass()const { return mass; }
		inline const Ogre::Vector3& getBoundBox()const { return boundBox; }
		inline const Ogre::Vector3& getPositionOSN()const { return osn ? osn->getPosition() : Ogre::Vector3::ZERO; }

	protected:
		BaseVehicle()
			: motor(0)
			, gearbox(0)
			, differential(0)
			, steerControl(0)
			, throttleControl(0)
			, brakeControl(0)
			, mass(0)
			, boundBox( Ogre::Vector3::ZERO )
			, massOffset( Ogre::Vector3::ZERO )
		{
		}

		//Имена
		string_traits::str vehicleSetName;
		string_traits::str vehicleBodyName;

		//Габариты корпуса
		Ogre::Vector3 boundBox;
		//Смещение центра масс от default центра
		Ogre::Vector3 massOffset;
		float mass;
		
		//Установленный двигатель
		Motor::shared_ptr motor;
		//Установленная Коробка передач
		Gearbox::shared_ptr gearbox;
		//Цепь дифференциалов
		Differential::shared_ptr differential;

		//SteerControl
		SteerControl::shared_ptr steerControl;
		//ThrottleControl
		BaseThrottleControl::shared_ptr throttleControl;
		//BrakeControl
		BrakeControl::shared_ptr brakeControl;

		//расчетник оборотов
		SpeedToGearCalculator::shared_ptr speedToGear;
		//Установленный спидометр
		Speedometer speedometer;
		//данные для водителя
		VehicleToDriverData::shared_ptr vehicleToDriver;

	private:
		BaseVehicle( const BaseVehicle& );
		BaseVehicle& operator=( const BaseVehicle& );
	};
	//------------------------------------------------------------
}
}
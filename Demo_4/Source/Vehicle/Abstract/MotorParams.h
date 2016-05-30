#pragma once
#include "MechanicsParams.h"
#include "FlywheelParams.h"
#include "MotorChart.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class MotorParams : public MechanicsParams
	{
	public:
		DEFINE_CLASS_SHARED_PTR(MotorParams)
		//--------------------------------//
		enum TORQUE_MULT_FUNC
		{
			TMF_NOT_DEFINED,
			TMF_CONST,
			TMF_LINEAR
		};
		//--------------------------------//
		MotorParams()
			: MechanicsParams( MT_MOTOR )
			, maxTorque(0)
			, maxStarterTorque(0)
			, belowIdleTorqueMult(0.0f)
			, belowIdleTorqueMultFunc( TMF_NOT_DEFINED )
		{
		}
		MotorParams( const MotorParams& motor )
			: MechanicsParams( MT_MOTOR )
			, maxTorque(0)
			, maxStarterTorque(0)
			, belowIdleTorqueMult(0.0f)
			, belowIdleTorqueMultFunc( TMF_NOT_DEFINED )
		{
			(*this) = motor;
		}
		virtual ~MotorParams(){CloseMotorParams();}
		virtual MechanicsParams::shared_ptr CreateCopyPtr()const{ return CreateMotorCopyPtr(); }
		MotorParams& operator=( const MotorParams& p )
		{
			if( this == &p )
				return *this;

			CopyMotorParams( p );
			return *this;
		}
		virtual bool isValid()const
		{
			return 
				chart.isValid()						&&
				maxTorque > 0						&&
				maxStarterTorque > 0				&&
				shared_ptr_valid(torqueLookup)		&&
				shared_ptr_valid(flywheel)			&&
				flywheel->isValid()					&&
				MechanicsParams::isValid();
		}
		void CloseMotorParams()
		{
			chart = MotorChart();
			maxTorque = 0;
			maxStarterTorque = 0;
			torqueLookup.reset();
			flywheel.reset();
			
			CloseMechanicsParams();
		}
		MotorParams::shared_ptr CreateMotorCopyPtr()const { return MotorParams::shared_ptr( new MotorParams( *this ) ); }

		MotorChart chart;
		float maxTorque;
		float maxStarterTorque;
		float belowIdleTorqueMult;
		TORQUE_MULT_FUNC belowIdleTorqueMultFunc;

		/*
		В двигателе используется маховик.
		Так как двигатель и коробка разделены сцеплением,
		то в преобразовании момнета в обороты будет помогать маховик.
		*/
		FlywheelParams::shared_ptr flywheel;

		/*
		Выборка моментов в скрипте обозначена именами ( см. VehicleMotorParamsExample )
		При запросе VehicleMotorParams из коллекции	имена будут заменены на готовые объекты.
		Нужно учитывать что данные указатели скрывают копии объектов.
		Поэтому при удалении VehicleMotorParams эти объекты должны быть явно удалены.
		*/
		//Выборка Момента Тяги из Оборотов
		Core::OgreRealVBV::shared_ptr torqueLookup;

	protected:
		void CopyMotorParams( const MotorParams& p )
		{
			chart = p.chart;
			maxTorque = p.maxTorque;
			maxStarterTorque = p.maxStarterTorque;
			flywheel.reset();
			if(shared_ptr_valid(p.flywheel)) flywheel = p.flywheel->CreateFlywheelCopyPtr();
			torqueLookup.reset();
			if(shared_ptr_valid(p.torqueLookup)) torqueLookup = torqueLookup->CreateCopyPtr();
			belowIdleTorqueMult = p.belowIdleTorqueMult;
			belowIdleTorqueMultFunc = p.belowIdleTorqueMultFunc;
			MechanicsParams::CopyMechanicsParams( p );
		}
	};
	//------------------------------------------------------------
}
}
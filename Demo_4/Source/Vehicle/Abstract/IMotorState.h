#pragma once
#include "IMotor.h"
#include "MotorParams.h"
#include "IMechanicsState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	struct MotorChart;
	//------------------------------------------------------------
	class IMotorState : public IMotor, public IMechanicsState
	{
		friend class Motor;
	public:
		DEFINE_CLASS_SHARED_PTR(IMotorState)
		//--------------------------------//
		virtual ~IMotorState()
		{
		}
		//Èםעונפויס IMotor
		virtual IMotor::shared_ptr GetInterface( MOTOR_INTERFACE_TYPE );
		//Èםעונפויס IMotor

		//Èםעונפויס VehicleMotor
		float getFlywheelInertia()const;
		float getMaxTorque()const;
		float getMaxDrag()const;
		const MotorChart& getMotorChart()const;
		bool IsStall()const;
		//Èםעונפויס VehicleMotor

	protected:
		IMotorState( Motor &m );
	};
	//------------------------------------------------------------
}
}
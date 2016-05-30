#pragma once
#include "IMotor.h"
#include "Core\RangeVar.h"

namespace Demo
{
namespace Vehicle
{	
	//------------------------------------------------------------
	class IMotorControl : public IMotor
	{
		friend class Motor;
	public:
		DEFINE_CLASS_SHARED_PTR(IMotorControl)
		//--------------------------------//
		virtual ~IMotorControl()
		{
		}
		//��������� IMotor
		virtual IMotor::shared_ptr GetInterface( MOTOR_INTERFACE_TYPE );
		//��������� IMotor

		//��������� VehicleMotor
		void SetThrottle( const Core::puFloat& );
		void SetStarter( bool run = true );
		//��������� VehicleMotor

	protected:
		IMotorControl( Motor &m )
			: IMotor( MIT_CONTROL, m )
		{
		}
	};
	//------------------------------------------------------------
}
}
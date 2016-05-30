#pragma once
#include "mechanics_enums.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class Motor;
	//------------------------------------------------------------
	class IMotor
	{
	public:
		DEFINE_CLASS_SHARED_PTR(IMotor)
		//--------------------------------//
		virtual ~IMotor()
		{
		}
		virtual IMotor::shared_ptr GetInterface( MOTOR_INTERFACE_TYPE ) = 0;

		inline MOTOR_INTERFACE_TYPE GetIType()const{ return iType; }

	protected:
		IMotor( MOTOR_INTERFACE_TYPE t, Motor &m )
			: iType( t )
			, motor(m)
		{
		}

		Motor &motor;

	private:
		IMotor();
		IMotor( const IMotor& );
		IMotor& operator=( const IMotor& );

		MOTOR_INTERFACE_TYPE iType;
	};
	//------------------------------------------------------------
}
}

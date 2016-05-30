#pragma once
#include "IMotor.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class VehicleMotor;
	//------------------------------------------------------------
	class IMotorCallback : public IMotor
	{
		friend class Motor;
	public:
		DEFINE_CLASS_SHARED_PTR(IMotorCallback)
		//--------------------------------//
		typedef delegate_traits::composite0::traits::del_in del_in;
		//--------------------------------//
		virtual ~IMotorCallback()
		{
		}
		//Èםעונפויס IMotor
		virtual IMotor::shared_ptr GetInterface( MOTOR_INTERFACE_TYPE );
		//Èםעונפויס IMotor

		//Èםעונפויס VehicleMotor
		void AddStallRPMDel( del_in );
		void AddStartRPMDel( del_in );
		void AddRedRPMDel( del_in );
		void AddMaxRPMDel( del_in );
		
		void RemoveStallRPMDel( del_in );
		void RemoveStartRPMDel( del_in );
		void RemoveRedRPMDel( del_in );
		void RemoveMaxRpmDel( del_in );
		//Èםעונפויס VehicleMotor

	protected:
		IMotorCallback( Motor &m )
			: IMotor( MIT_CALLBACK, m )
		{
		}
	};
	//------------------------------------------------------------
}
}
#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "Vehicle/Abstract/Motor.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	void IMotorControl::SetThrottle( const Core::puFloat &t )
	{
		motor.SetThrottle( t );
	}
	//------------------------------------------------------------
	void IMotorControl::SetStarter( bool run )
	{
		motor.SetStarter( run );
	}
	//------------------------------------------------------------
	IMotor::shared_ptr IMotorControl::GetInterface( MOTOR_INTERFACE_TYPE type )
	{
		return motor.GetInterface( type );
	}
	//------------------------------------------------------------
	IMotor::shared_ptr IMotorCallback::GetInterface( MOTOR_INTERFACE_TYPE type )
	{
		return motor.GetInterface( type );
	}
	//------------------------------------------------------------
	const MotorChart& IMotorState::getMotorChart()const
	{
		return motor.getChart();
	}
	//------------------------------------------------------------
	float IMotorState::getFlywheelInertia() const
	{
		return motor.getFlywheelInertia();
	}
	//------------------------------------------------------------
	IMotor::shared_ptr IMotorState::GetInterface( MOTOR_INTERFACE_TYPE type )
	{
		return motor.GetInterface( type );
	}
	//------------------------------------------------------------
	IMotorState::IMotorState( Motor &m )
		: IMotor( MIT_STATE, m )
		, IMechanicsState( m )
	{
	}
	//------------------------------------------------------------
	void IMotorCallback::AddMaxRPMDel(del_in del)
	{
		motor.AddMaxRPMDel( del );
	}
	//------------------------------------------------------------
	void IMotorCallback::AddRedRPMDel(del_in del)
	{
		motor.AddRedRPMDel( del );
	}
	//------------------------------------------------------------
	void IMotorCallback::AddStallRPMDel(del_in del)
	{
		motor.AddStallRPMDel( del );
	}
	//------------------------------------------------------------
	void IMotorCallback::AddStartRPMDel(del_in del)
	{
		motor.AddStartRPMDel( del );
	}
	//------------------------------------------------------------
	void IMotorCallback::RemoveMaxRpmDel(del_in del)
	{
		motor.RemoveMaxRpmDel( del );
	}
	//------------------------------------------------------------
	void IMotorCallback::RemoveRedRPMDel(del_in del)
	{
		motor.RemoveRedRPMDel( del );
	}
	//------------------------------------------------------------
	void IMotorCallback::RemoveStallRPMDel(del_in del)
	{
		motor.RemoveStallRPMDel( del );
	}
	//------------------------------------------------------------
	void IMotorCallback::RemoveStartRPMDel(del_in del)
	{
		motor.RemoveStartRPMDel( del );
	}
	//------------------------------------------------------------
	bool IMotorState::IsStall()const
	{
		return motor.isStall();
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
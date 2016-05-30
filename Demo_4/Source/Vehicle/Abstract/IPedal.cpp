#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "PedalFilter.h"
#include "IPedalControl.h"
#include "IPedalCallback.h"
#include "IPedalState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	IPedal::shared_ptr IPedalControl::GetInterface( PEDAL_INTERFACE_TYPE type )
	{
		return pedal.GetInterface( type );
	}
	//------------------------------------------------------------
	void IPedalControl::Press()
	{
		pedal.Press();
	}
	//------------------------------------------------------------
	void IPedalControl::Press( const Core::puFloat &value )
	{
		pedal.Press( value );
	}
	//------------------------------------------------------------
	void IPedalControl::Release()
	{
		pedal.Release();
	}
	//------------------------------------------------------------
	void IPedalControl::SetPressTime( float time )
	{
		pedal.SetPressTime( time );
	}
	//------------------------------------------------------------
	void IPedalControl::SetReleaseTime( float time )
	{
		pedal.SetReleaseTime( time );
	}
	//------------------------------------------------------------
	IPedal::shared_ptr ISteerControl::GetInterface( PEDAL_INTERFACE_TYPE type )
	{
		return steer.GetInterface( type );
	}
	//------------------------------------------------------------
	void ISteerControl::Press( const Core::npuFloat &value )
	{
		steer.Press( value );
	}
	//------------------------------------------------------------
	void ISteerControl::PressLeft()
	{
		steer.PressLeft();
	}
	//------------------------------------------------------------
	void ISteerControl::PressRight()
	{
		steer.PressRight();
	}
	//------------------------------------------------------------
	void ISteerControl::Release()
	{
		steer.Release();
	}
	//------------------------------------------------------------
	void ISteerControl::SetPressTime( float time )
	{
		steer.SetPressTime( time );
	}
	//------------------------------------------------------------
	void ISteerControl::SetReleaseTime( float time )
	{
		steer.SetReleaseTime( time );
	}
	//------------------------------------------------------------
	void IPedalCallback::AddPressedOnceDel( del_in del )
	{
		pedal.AddPressedOnceDel( del );
	}
	//------------------------------------------------------------
	void IPedalCallback::AddPressedTempDel( del_in del )
	{
		pedal.AddPressedTempDel( del );
	}
	//------------------------------------------------------------
	void IPedalCallback::AddReleasedOnceDel( del_in del )
	{
		pedal.AddReleasedOnceDel( del );
	}
	//------------------------------------------------------------
	void IPedalCallback::AddReleasedTempDel( del_in del )
	{
		pedal.AddReleasedTempDel( del );
	}
	//------------------------------------------------------------
	void IPedalCallback::RemovePressedOnceDel( del_in del )
	{
		pedal.RemovePressedOnceDel( del );
	}
	//------------------------------------------------------------
	void IPedalCallback::RemovePressedTempDel( del_in del )
	{
		pedal.RemovePressedTempDel( del );
	}
	//------------------------------------------------------------
	void IPedalCallback::RemoveReleasedOnceCall( del_in del )
	{
		pedal.RemoveReleasedOnceCall( del );
	}
	//------------------------------------------------------------
	void IPedalCallback::RemoveReleasedTempCall( del_in del )
	{
		pedal.RemoveReleasedTempCall( del );
	}
	//------------------------------------------------------------
	void ISteerCallback::AddPressedOnceDel( del_in del )
	{
		steer.AddPressedOnceDel( del );
	}
	//------------------------------------------------------------
	void ISteerCallback::AddPressedTempDel( del_in del )
	{
		steer.AddPressedTempDel( del );
	}
	//------------------------------------------------------------
	void ISteerCallback::AddReleasedOnceDel( del_in del )
	{
		steer.AddReleasedOnceDel( del );
	}
	//------------------------------------------------------------
	void ISteerCallback::AddReleasedTempDel( del_in del )
	{
		steer.AddReleasedTempDel( del );
	}
	//------------------------------------------------------------
	void ISteerCallback::RemovePressedOnceDel( del_in del )
	{
		steer.RemovePressedOnceDel( del );
	}
	//------------------------------------------------------------
	void ISteerCallback::RemovePressedTempDel( del_in del )
	{
		steer.RemovePressedTempDel( del );
	}
	//------------------------------------------------------------
	void ISteerCallback::RemoveReleasedOnceCall( del_in del )
	{
		steer.RemoveReleasedOnceCall( del );
	}
	//------------------------------------------------------------
	void ISteerCallback::RemoveReleasedTempCall( del_in del )
	{
		steer.RemoveReleasedTempCall( del );
	}
	//------------------------------------------------------------
	IPedal::shared_ptr IPedalCallback::GetInterface( PEDAL_INTERFACE_TYPE type )
	{
		return pedal.GetInterface( type );
	}
	//------------------------------------------------------------
	IPedal::shared_ptr ISteerCallback::GetInterface( PEDAL_INTERFACE_TYPE type )
	{
		return steer.GetInterface( type );
	}
	//------------------------------------------------------------
	IPedal::shared_ptr IPedalState::GetInterface( PEDAL_INTERFACE_TYPE type )
	{
		return pedal.GetInterface( type );
	}
	//------------------------------------------------------------
	Core::puFloat IPedalState::Get() const
	{
		return pedal.Get();
	}
	//------------------------------------------------------------
	bool IPedalState::IsWorking() const
	{
		return pedal.IsWorking();
	}
	//------------------------------------------------------------
	bool IPedalState::IsFullPressed() const
	{
		return pedal.IsFullPressed();
	}
	//------------------------------------------------------------
	bool IPedalState::IsFullReleased() const
	{
		return pedal.IsFullReleased();
	}
	//------------------------------------------------------------
	float IPedalState::GetPressSpeed() const
	{
		return pedal.GetPressSpeed();
	}
	//------------------------------------------------------------
	float IPedalState::GetReleaseSpeed() const
	{
		return pedal.GetReleaseSpeed();
	}
	//------------------------------------------------------------
	float IPedalState::GetCurrentValue() const
	{
		return pedal.GetCurrentValue();
	}
	//------------------------------------------------------------
	float IPedalState::GetTargetValue() const
	{
		return pedal.GetTargetValue();
	}
	//------------------------------------------------------------
	Core::npuFloat ISteerState::Get() const
	{
		return steer.Get();
	}
	//------------------------------------------------------------
	float ISteerState::GetPressSpeed() const
	{
		return steer.GetPressSpeed();
	}
	//------------------------------------------------------------
	float ISteerState::GetReleaseSpeed() const
	{
		return steer.GetReleaseSpeed();
	}
	//------------------------------------------------------------
	float ISteerState::GetCurrentValue() const
	{
		return steer.GetCurrentValue();
	}
	//------------------------------------------------------------
	float ISteerState::GetTargetValue() const
	{
		return steer.GetTargetValue();
	}
	//------------------------------------------------------------
	bool ISteerState::IsWorking() const
	{
		return steer.IsWorking();
	}
	//------------------------------------------------------------
	bool ISteerState::IsFullPressed() const
	{
		return steer.IsFullPressed();
	}
	//------------------------------------------------------------
	bool ISteerState::IsFullReleased() const
	{
		return steer.IsFullReleased();
	}
	//------------------------------------------------------------
	IPedal::shared_ptr ISteerState::GetInterface( PEDAL_INTERFACE_TYPE type )
	{
		return steer.GetInterface( type );
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "ThrottleControl.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	ThrottleControlParams::ThrottleControlParams()
		: tcsType( TCS_NOT_DEFINED )
	{
	}
	//------------------------------------------------------------
	bool ThrottleControlParams::isValid()const
	{
		return tcsType != TCS_NOT_DEFINED && !name.empty();
	}
	//------------------------------------------------------------
	void BaseThrottleControl::OperateController()
	{
		throttle.Operate();
		iMotorControl->SetThrottle( throttle );
	}
	//------------------------------------------------------------
	IPedal::shared_ptr BaseThrottleControl::GetThrottleInterface( PEDAL_INTERFACE_TYPE type )
	{
		return throttle.GetInterface( type );
	}
	//------------------------------------------------------------
	bool ConstFeedThrottleControl::Initialize(const InitData &data)
	{
		if( !data.isValid() )return false;
		iMotorControl = data.motorControl;
		throttle.SetMinLimit( data.tcParams->throttleFeed.get() );
		return true;
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
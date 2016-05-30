#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "Speedometer.h"
#include "ISpeedometerState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	ISpeedometerState::ISpeedometerState( Speedometer &meter )
		: sm( meter )
	{
	}
	//------------------------------------------------------------
	float ISpeedometerState::GetWheelRadius()const
	{
		return sm.getWheelRadius();
	}
	//------------------------------------------------------------
	float ISpeedometerState::GetSpeedMS() const
	{
		return sm.getSpeedMS();
	}
	//------------------------------------------------------------
	float ISpeedometerState::GetSpeedKMH() const
	{
		return sm.getSpeedKMH();
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
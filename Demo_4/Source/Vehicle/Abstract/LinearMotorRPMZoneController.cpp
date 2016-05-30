#include "LinearMotorRPMZoneController.h"

namespace Demo
{
namespace vehicle
{
	//------------------------------------------------------------
	bool RPMZoneTimingCalculator::Initialize(const RPMZoneTimingCalculator::ZTInitData &data)
	{
		if( !data.isValid() )return false;
		
		iStyle = data.iStyle;
		calmTiming = data.calmTiming;
		agrTiming = data.agrTiming;
		return true;
	}
	//------------------------------------------------------------
	float RPMZoneTimingCalculator::GetRpmZoneReactionTime() const
	{
		return Calculate( calmTiming.zoneReactionTime, agrTiming.zoneReactionTime );
	}
	//------------------------------------------------------------
	float RPMZoneTimingCalculator::GetRpmSpeedReactionTime() const
	{
		return Calculate( calmTiming.speedReactionTime, agrTiming.speedReactionTime );
	}
	//------------------------------------------------------------
}
}
#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "Speedometer.h"
#include "Interface/UnitConverter.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	bool Speedometer::Initialize( const InitData& data )
	{
		if( !data.isValid() )return false;

		revSensor = data.revSensor;
		setWheelRadius( data.wheelRadius );
		return true;
	}
	//------------------------------------------------------------
	void Speedometer::setWheelRadius( float newRadius )
	{
		if( newRadius <= 0 )return;
		wheelRadius = newRadius;
		revToMS = wheelRadius;
		revToKMH = UnitConverter::MStoKMH * revToMS;
	}
	//------------------------------------------------------------
	ISpeedometerState::shared_ptr Speedometer::GetStateInterface()
	{
		if( shared_ptr_null(iState) )iState.bind( new ISpeedometerState( *this ) );
		return iState;
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
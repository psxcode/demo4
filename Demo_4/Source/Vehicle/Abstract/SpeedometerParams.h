#pragma once

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class SpeedometerParams
	{
	public:
		SpeedometerParams()
			: revSensorAxis(0)
			, revSensorSide(0)
			, wheelRadius(0)
		{
		}

		int revSensorAxis;
		int revSensorSide;
		float wheelRadius;
	};
	//------------------------------------------------------------
}
}
#pragma once

namespace Demo
{
namespace Vehicle
{
	class Speedometer;
	//------------------------------------------------------------
	class ISpeedometerState
	{
	public:
		DEFINE_CLASS_SHARED_PTR(ISpeedometerState)
		//--------------------------------//
		ISpeedometerState( Speedometer & );

		float GetWheelRadius()const;
		float GetSpeedMS()const;
		float GetSpeedKMH()const;

	protected:
		Speedometer &sm;

	private:
		ISpeedometerState();
		ISpeedometerState( const ISpeedometerState& );
		ISpeedometerState& operator=( const ISpeedometerState& );
	};
	//------------------------------------------------------------
}
}
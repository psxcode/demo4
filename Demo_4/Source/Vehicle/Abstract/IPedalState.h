#pragma once
#include "IPedal.h"
#include "Core\RangeVar.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class PedalFilter;
	class SteerFilter;
	//------------------------------------------------------------
	class IPedalState : public IPedal
	{
		friend class PedalFilter;
	public:
		DEFINE_CLASS_SHARED_PTR(IPedalState)
		//--------------------------------//
		IPedalState( PedalFilter &p )
			: IPedal( PIT_PSTATE )
			, pedal(p)
		{
		}
		//��������� IPedal
		virtual IPedal::shared_ptr GetInterface( PEDAL_INTERFACE_TYPE );
		//��������� IPedal

		//��������� PedalFilter
		Core::puFloat Get()const;
		bool IsWorking()const;
		bool IsFullPressed()const;
		bool IsFullReleased()const;
		float GetPressSpeed()const;
		float GetReleaseSpeed()const;
		float GetCurrentValue()const;
		float GetTargetValue()const;
		//��������� PedalFilter

	protected:
		PedalFilter &pedal;
	};
	//------------------------------------------------------------
	class ISteerState : public IPedal
	{
		friend class SteerFilter;
	public:
		DEFINE_CLASS_SHARED_PTR(ISteerState)
		//--------------------------------//
		ISteerState( SteerFilter &s )
			: IPedal( PIT_SSTATE )
			, steer(s)
		{
		}
		//��������� IPedal
		virtual IPedal::shared_ptr GetInterface( PEDAL_INTERFACE_TYPE );
		//��������� IPedal

		//��������� SteerFilter
		Core::npuFloat Get()const;
		float GetPressSpeed()const;
		float GetReleaseSpeed()const;
		float GetCurrentValue()const;
		float GetTargetValue()const;
		bool IsWorking()const;
		bool IsFullReleased()const;
		bool IsFullPressed()const;
		//��������� SteerFilter

	protected:
		SteerFilter &steer;
	};
	//------------------------------------------------------------
}
}
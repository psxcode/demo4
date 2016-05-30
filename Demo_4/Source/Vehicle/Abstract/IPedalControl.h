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
	class IPedalControl : public IPedal
	{
		friend class PedalFilter;
	public:
		DEFINE_CLASS_SHARED_PTR(IPedalControl)
		//--------------------------------//
		IPedalControl(PedalFilter &p )
			: IPedal( PIT_PCONTROL )
			, pedal(p)
		{
		}
		//Èםעונפויס IPedal
		virtual IPedal::shared_ptr GetInterface( PEDAL_INTERFACE_TYPE );
		//Èםעונפויס IPedal

		//Èםעונפויס PedalFilter
		void Press();
		void Press( const Core::puFloat& value );
		void Release();
		void SetPressTime( float time );
		void SetReleaseTime( float time );
		//Èםעונפויס PedalFilter

	protected:
		PedalFilter &pedal;
	};
	//------------------------------------------------------------
	class ISteerControl : public IPedal
	{
		friend class SteerFilter;
	public:
		DEFINE_CLASS_SHARED_PTR(ISteerControl)
		//--------------------------------//
		ISteerControl( SteerFilter &s )
			: IPedal( PIT_SCONTROL )
			, steer(s)
		{
		}
		//Èםעונפויס IPedal
		virtual IPedal::shared_ptr GetInterface( PEDAL_INTERFACE_TYPE );
		//Èםעונפויס IPedal

		//Èםעונפויס SteerFilter
		void Press( const Core::npuFloat& value );
		void PressRight();
		void PressLeft();
		void Release();
		void SetPressTime( float time );
		void SetReleaseTime( float time );
		//Èםעונפויס SteerFilter

	protected:
		SteerFilter &steer;
	};
	//------------------------------------------------------------
}
}
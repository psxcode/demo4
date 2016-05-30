#pragma once
#include "IPedal.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class PedalFilter;
	class SteerFilter;
	//------------------------------------------------------------
	class IPedalCallback : public IPedal
	{
		friend class PedalFilter;
	public:
		DEFINE_CLASS_SHARED_PTR(IPedalCallback)
		//--------------------------------//
		typedef delegate_traits::composite0::traits::del_in del_in;
		//--------------------------------//
		IPedalCallback( PedalFilter &p )
			: IPedal( PIT_PCALLBACK )
			, pedal(p)
		{
		}
		//��������� IPedal
		virtual IPedal::shared_ptr GetInterface( PEDAL_INTERFACE_TYPE );
		//��������� IPedal

		//��������� PedalFilter
		void AddPressedOnceDel( del_in del );
		void AddPressedTempDel( del_in del );
		void AddReleasedOnceDel( del_in del );
		void AddReleasedTempDel( del_in del );
		void RemovePressedOnceDel( del_in del );
		void RemovePressedTempDel( del_in del );
		void RemoveReleasedOnceCall( del_in del );
		void RemoveReleasedTempCall( del_in del );
		//��������� PedalFilter

	protected:
		PedalFilter &pedal;
	};
	//------------------------------------------------------------
	class ISteerCallback : public IPedal
	{
		friend class SteerFilter;
	public:
		DEFINE_CLASS_SHARED_PTR(ISteerCallback)
		//--------------------------------//
		typedef delegate_traits::composite0::traits::del_in del_in;
		//--------------------------------//
		ISteerCallback( SteerFilter &s )
			: IPedal( PIT_SCALLBACK )
			, steer(s)
		{
		}
		//��������� IPedal
		virtual IPedal::shared_ptr GetInterface( PEDAL_INTERFACE_TYPE );
		//��������� IPedal

		//��������� SteerFilter
		void AddPressedOnceDel( del_in del );
		void AddPressedTempDel( del_in del );
		void AddReleasedOnceDel( del_in del );
		void AddReleasedTempDel( del_in del );
		void RemovePressedOnceDel( del_in del );
		void RemovePressedTempDel( del_in del );
		void RemoveReleasedOnceCall( del_in del );
		void RemoveReleasedTempCall( del_in del );
		//��������� SteerFilter

	protected:
		SteerFilter &steer;
	};
	//------------------------------------------------------------
}
}
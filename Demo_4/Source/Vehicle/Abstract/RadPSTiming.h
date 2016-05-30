#pragma once
#include "DriveStyleCalculator.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class RadPSTiming
	{
	public:
		DEFINE_CLASS_SHARED_PTR(RadPSTiming)
		//--------------------------------//
		RadPSTiming()
			: zoneReactionTime(0)
			, speedReactionTime(0)
		{
		}
		float zoneReactionTime;
		float speedReactionTime;
	};
	//------------------------------------------------------------
	class BaseRadPSTimingCalculator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(BaseRadPSTimingCalculator)
		static const float timeLimit;
		//--------------------------------//
		virtual ~BaseRadPSTimingCalculator()
		{
		}
		virtual float GetZoneReactionTime()const = 0;
		virtual float GetSpeedReactionTime()const = 0;
	};
	//------------------------------------------------------------
	class ConstantRadPSTimingCalculator : public BaseRadPSTimingCalculator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(ConstantRadPSTimingCalculator)
		//--------------------------------//
		virtual ~ConstantRadPSTimingCalculator()
		{
		}
		virtual float GetZoneReactionTime()const{ return constantTiming.zoneReactionTime; }
		virtual float GetSpeedReactionTime()const{ return constantTiming.speedReactionTime; }

		bool Initialize( const RadPSTiming& time )
		{
			if( time.speedReactionTime < timeLimit || time.speedReactionTime < timeLimit )return false;
			constantTiming = time;
			return true;
		}

	protected:
		RadPSTiming constantTiming;
	};
	//------------------------------------------------------------
	class LinearRadPSTimingCalculator : public BaseRadPSTimingCalculator, public DriveStyleCalculator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(LinearRadPSTimingCalculator)
		//--------------------------------//
		struct LinearInitData : public DriveStyleCalculator::InitData
		{
			virtual ~LinearInitData()
			{
			}
			virtual bool isValid()const
			{
				return lowTiming.speedReactionTime > timeLimit &&
					lowTiming.zoneReactionTime > timeLimit &&
					highTiming.speedReactionTime > timeLimit &&
					highTiming.zoneReactionTime > timeLimit &&
					DriveStyleCalculator::InitData::isValid();
			}
			RadPSTiming lowTiming;
			RadPSTiming highTiming;
		};
		//--------------------------------//
		virtual ~LinearRadPSTimingCalculator()
		{
		}
		virtual float GetZoneReactionTime()const{ return Calculate( lowTiming.zoneReactionTime, highTiming.zoneReactionTime ); }
		virtual float GetSpeedReactionTime()const{ return Calculate( lowTiming.speedReactionTime, highTiming.speedReactionTime ); }

		bool Initialize( const LinearInitData& idata )
		{
			if( !idata.isValid() )return false;
			iStyle = idata.iStyle;
			lowTiming = idata.lowTiming;
			highTiming = idata.highTiming;
			return true;
		}

	protected:
		RadPSTiming lowTiming;
		RadPSTiming highTiming;
	};
	//------------------------------------------------------------
	class ExtFeedRadPSTimingCalculator : public BaseRadPSTimingCalculator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(ExtFeedRadPSTimingCalculator)
		//--------------------------------//
		virtual ~ExtFeedRadPSTimingCalculator()
		{
		}
		virtual float GetZoneReactionTime()const{ return extTiming->zoneReactionTime; }
		virtual float GetSpeedReactionTime()const{ return extTiming->speedReactionTime; }

		bool Initialize( const RadPSTiming::shared_ptr &time )
		{
			if( time->speedReactionTime < timeLimit || time->zoneReactionTime < timeLimit )return false;
			extTiming = time;
			return true;
		}

	protected:
		RadPSTiming::shared_ptr extTiming;
	};
	//------------------------------------------------------------
}
}
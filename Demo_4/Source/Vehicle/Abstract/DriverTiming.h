#pragma once

#include "DriveStyleCalculator.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	/*
	—труктура содержит значени€ времени реакции водител€
	*/
	struct DriverTiming
	{
		DEFINE_CLASS_SHARED_PTR(DriverTiming)
		//--------------------------------//
		DriverTiming::DriverTiming()
			: throttlePressTime( 0.1f )
			, throttleReleaseTime( 0.1f )
			, brakePressTime( 0.1f )
			, brakeReleaseTime( 0.1f )
			, clutchPressTime( 0.1f )
			, clutchReleaseTime( 0.1f )
			, handbrakePressTime( 0.1f )
			, handbrakeReleaseTime( 0.1f )
			, steerPressTime( 0.1f )
			, steerReleaseTime( 0.1f )
			, shiftDelayTime( 0.1f )
			, shiftToNeutralTime( 0.1f )
			, shiftToDirectTime( 0.1f )
			, shiftToReverseTime( 0.2f )
			, switchModeTime( 0.1f )
			, switchSeqTime( 0.1f )
			, rpmZoneReactionTime( 0.1f )
			, rpmSpeedReactionTime( 0.15f )
		{
		}

		//Axes
		float throttlePressTime;
		float throttleReleaseTime;
		float brakePressTime;
		float brakeReleaseTime;
		float clutchPressTime;
		float clutchReleaseTime;
		float handbrakePressTime;
		float handbrakeReleaseTime;
		float steerPressTime;
		float steerReleaseTime;

		//Manual gearbox timing
		float shiftToNeutralTime;
		float shiftToDirectTime;
		float shiftToReverseTime;
		float shiftDelayTime;

		//Auto gearbox timing
		float switchModeTime;
		float switchSeqTime;

		//Zone timing
		float rpmZoneReactionTime;
		float rpmSpeedReactionTime;

		string_traits::str name;
	};
	//------------------------------------------------------------
	enum DRIVER_TIMING_CALC_TYPE
	{
		DTCT_NOT_DEFINED,
		DTCT_CONSTANT,
		DTCT_LINEAR
	};
	//------------------------------------------------------------
	/*
	—ери€ классов помогает расчитать промежуточное значени€ времени
	при использовании агрессивного и спокойного стил€ вождени€
	*/
	class DriverTimingCalculator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DriverTimingCalculator)
		//--------------------------------//
		virtual ~DriverTimingCalculator()
		{
		}

		virtual float GetThrottlePressTime()const = 0;
		virtual float GetThrottleReleaseTime()const = 0;
		virtual float GetBrakePressTime()const = 0;
		virtual float GetBrakeReleaseTime()const = 0;
		virtual float GetClutchPressTime()const = 0;
		virtual float GetClutchReleaseTime()const = 0;
		virtual float GetHandbrakePressTime()const = 0;
		virtual float GetHandbrakeReleaseTime()const = 0;
		virtual float GetSteerPressTime()const = 0;
		virtual float GetSteerReleaseTime()const = 0;
		virtual float GetShiftToNeutralTime()const = 0;
		virtual float GetShiftToDirectTime()const = 0;
		virtual float GetShiftToReverseTime()const = 0;
		virtual float GetShiftDelayTime()const = 0;
		virtual float GetSwitchModeTime()const = 0;
		virtual float GetSwitchSeqTime()const = 0;
		virtual float GetRpmZoneReactionTime()const = 0;
		virtual float GetRpmSpeedReactionTime()const = 0;

		inline DRIVER_TIMING_CALC_TYPE getCalcType()const{ return calcType; }

	protected:
		DriverTimingCalculator( DRIVER_TIMING_CALC_TYPE ct )
			: calcType( ct )
		{
		}

	private:
		DriverTimingCalculator( const DriverTimingCalculator& );
		DriverTimingCalculator& operator=( const DriverTimingCalculator& );

		DRIVER_TIMING_CALC_TYPE calcType;
	};
	//------------------------------------------------------------
	class ConstantDriverTimingCalculator : public DriverTimingCalculator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(ConstantDriverTimingCalculator)
		//--------------------------------//
		struct ConstantInitData
		{
			bool isValid()const{ return timing != 0; }
			DriverTiming::const_shared_ptr timing;
		};
		//--------------------------------//
		ConstantDriverTimingCalculator()
			: DriverTimingCalculator( DTCT_CONSTANT )
		{
		}
		virtual ~ConstantDriverTimingCalculator()
		{
		}

		bool Initialize( const ConstantInitData& idata )
		{
			if( !idata.isValid() )return false;
			timing = idata.timing;
			return true;
		}

		virtual float GetThrottlePressTime()const{ return timing->throttlePressTime; }
		virtual float GetThrottleReleaseTime()const{ return timing->throttleReleaseTime; }
		virtual float GetBrakePressTime()const{ return timing->brakePressTime; }
		virtual float GetBrakeReleaseTime()const{ return timing->brakeReleaseTime; }
		virtual float GetClutchPressTime()const{ return timing->clutchPressTime; }
		virtual float GetClutchReleaseTime()const{ return timing->clutchReleaseTime; }
		virtual float GetHandbrakePressTime()const{ return timing->handbrakePressTime; }
		virtual float GetHandbrakeReleaseTime()const{ return timing->handbrakeReleaseTime; }
		virtual float GetSteerPressTime()const{ return timing->steerPressTime; }
		virtual float GetSteerReleaseTime()const{ return timing->steerReleaseTime; }
		virtual float GetShiftToNeutralTime()const{ return timing->shiftToNeutralTime; }
		virtual float GetShiftToDirectTime()const{ return timing->shiftToDirectTime; }
		virtual float GetShiftToReverseTime()const{ return timing->shiftToReverseTime; }
		virtual float GetShiftDelayTime()const{ return timing->shiftDelayTime; }
		virtual float GetSwitchModeTime()const{ return timing->switchModeTime; }
		virtual float GetSwitchSeqTime()const{ return timing->switchSeqTime; }
		virtual float GetRpmZoneReactionTime()const{ return timing->rpmZoneReactionTime; }
		virtual float GetRpmSpeedReactionTime()const{ return timing->rpmSpeedReactionTime; }

		inline DriverTiming::const_shared_ptr getTiming()const{ return timing; }

	protected:
		DriverTiming::const_shared_ptr timing;
	};
	//------------------------------------------------------------
	class LinearDriverTimingCalculator : public DriverTimingCalculator, public DriveStyleCalculator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(LinearDriverTimingCalculator)
		//--------------------------------//
		struct LinearInitData : public DriveStyleCalculator::InitData
		{
			virtual ~LinearInitData(){}
			virtual bool isValid()const{ return agr !=0 && calm !=0 && DriveStyleCalculator::InitData::isValid(); }

			DriverTiming::const_shared_ptr agr;
			DriverTiming::const_shared_ptr calm;			
		};
		//--------------------------------//
		LinearDriverTimingCalculator()
			: DriverTimingCalculator( DTCT_LINEAR )
		{
		}
		virtual ~LinearDriverTimingCalculator()
		{
		}

		bool Initialize( const LinearInitData& idata )
		{
			if( !idata.isValid() )return false;
			iStyle = idata.iStyle;
			agr = idata.agr;
			calm = idata.calm;
			return true;
		}

		virtual float GetThrottlePressTime()const{ return Calculate( calm->throttlePressTime, agr->throttleReleaseTime ); }
		virtual float GetThrottleReleaseTime()const{ return Calculate( calm->throttleReleaseTime, agr->throttleReleaseTime ); }
		virtual float GetBrakePressTime()const{ return Calculate( calm->brakePressTime, agr->brakePressTime ); }
		virtual float GetBrakeReleaseTime()const{ return Calculate( calm->brakeReleaseTime, agr->brakeReleaseTime ); }
		virtual float GetClutchPressTime()const{ return Calculate( calm->clutchPressTime, agr->clutchPressTime ); }
		virtual float GetClutchReleaseTime()const{ return Calculate( calm->clutchReleaseTime, agr->clutchReleaseTime ); }
		virtual float GetHandbrakePressTime()const{ return Calculate( calm->handbrakePressTime, agr->handbrakePressTime ); }
		virtual float GetHandbrakeReleaseTime()const{ return Calculate( calm->handbrakeReleaseTime, agr->handbrakeReleaseTime ); }
		virtual float GetSteerPressTime()const{ return Calculate( calm->steerPressTime, agr->steerPressTime ); }
		virtual float GetSteerReleaseTime()const{ return Calculate( calm->steerReleaseTime, agr->steerReleaseTime ); }
		virtual float GetShiftToNeutralTime()const{ return Calculate( calm->shiftToNeutralTime, agr->shiftToNeutralTime ); }
		virtual float GetShiftToDirectTime()const{ return Calculate( calm->shiftToDirectTime, agr->shiftToDirectTime ); }
		virtual float GetShiftToReverseTime()const{ return Calculate( calm->shiftToReverseTime, agr->shiftToReverseTime ); }
		virtual float GetShiftDelayTime()const{ return Calculate( calm->shiftDelayTime, agr->shiftDelayTime ); }
		virtual float GetSwitchModeTime()const{ return Calculate( calm->switchModeTime, agr->switchModeTime ); }
		virtual float GetSwitchSeqTime()const{ return Calculate( calm->switchSeqTime, agr->switchSeqTime ); }
		virtual float GetRpmZoneReactionTime()const{ return Calculate( calm->rpmZoneReactionTime, agr->rpmZoneReactionTime ); }
		virtual float GetRpmSpeedReactionTime()const{ return Calculate( calm->rpmSpeedReactionTime, agr->rpmSpeedReactionTime ); }		

		inline DriverTiming::const_shared_ptr getAgrTiming()const{ return agr; }
		inline DriverTiming::const_shared_ptr getCalmTiming()const{ return calm; }

	protected:
		DriverTiming::const_shared_ptr agr;
		DriverTiming::const_shared_ptr calm;
	};
	//------------------------------------------------------------
}
}
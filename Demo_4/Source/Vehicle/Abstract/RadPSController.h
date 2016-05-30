#pragma once
#include "Core\Controller.h"
#include "Core\DelTimer.h"
#include "MotorChart.h"
#include "RadPSControllerTypes.h"
#include "RadPSTiming.h"
#include "IMotorState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class RadPSController : public Core::Controller
	{
	public:
		DEFINE_CLASS_SHARED_PTR(RadPSController)
		//--------------------------------//
		class traits : public Controller::traits
		{
		public:
			typedef delegate_traits::composite0::traits::del_in del_in;
			typedef Demo::map< int, BaseRadPSEntry::shared_ptr >::type entry_container;
		};
		//--------------------------------//
		struct InitData
		{
			virtual bool isValid()const{ return !mechanicsState.isNull() && !timing.isNull(); }

			BaseRadPSTimingCalculator::shared_ptr timing;
			IMechanicsState::shared_ptr mechanicsState;
		};
		//--------------------------------//
		RadPSController()
		{
		}
		virtual ~RadPSController()
		{
		}
		//Controller interface
		virtual void CloseController();
		virtual void OperateController();
		//Controller interface

		virtual void ReOperateEntries(){ onZoneTimer(); }
		virtual void ReCallEntries();

		bool Initialize( const InitData& );
		
		bool addEntry( const traits::entry_container::value_type& );
		void removeEntry( int id );
		BaseRadPSEntry::const_shared_ptr getEntry( int id )const;
		BaseRadPSEntry::shared_ptr getEntry( int id );

		void measureSpeed( float timeMult );
		void measureSpeed( float timeMult, traits::del_in tempDel );
		void measureSpeedIncrease( float timeMult, traits::del_in tempDel );
		void measureSpeedDecrease( float timeMult, traits::del_in tempDel );
		void removeSpeedDel( traits::del_in );

	protected:
		void onSpeedTimer();
		void onZoneTimer();

		delegate_traits::del0<void>::shared_ptr zoneDel;
		delegate_traits::del0<void>::shared_ptr speedDel;

		Core::DelTimer zoneTimer;
		Core::DelTimer speedTimer;

		traits::entry_container eMap;
		BaseRadPSTimingCalculator::shared_ptr timing;

		IMechanicsState::shared_ptr iState;

		//radPS speed
		float speedMeasured;
		float startRadPS;
		delegate_traits::composite0 speedMeasureCallback;
		delegate_traits::composite0 speedIncMeasureCallback;
		delegate_traits::composite0 speedDecMeasureCallback;
		//radPS speed

	private:
		RadPSController( const RadPSController& );
		RadPSController& operator=( const RadPSController& );
	};
	//------------------------------------------------------------
	class MotorChartRadPSController : public RadPSController
	{
	public:
		DEFINE_CLASS_SHARED_PTR(MotorChartRadPSController)
		//--------------------------------//
		//Идентификаторы для Entries. Чтобы добавить в eMap
		enum RADPS_IDS
		{
			RID_MIN_MARK,
			RID_NORM_LOW_MARK,
			RID_NORM_HIGH_MARK,
			RID_MAX_MARK,
			RID_MIN_ZONE,
			RID_NORM_ZONE,
			RID_MAX_ZONE
		};
		//--------------------------------//
		//Имена зон. Чтобы знать в какой зоне сейчас находится radPS
		enum ZONE_ID
		{
			ZONE_NOT_DEFINED,
			ZONE_BELOW_MIN,
			ZONE_MIN,
			ZONE_NORM,
			ZONE_MAX,
			ZONE_ABOVE_MAX
		};
		//--------------------------------//
		struct MCInitData : public RadPSController::InitData
		{
			virtual bool isValid()const{ return iMotorState != 0 && mcmCalc != 0 && timing != 0; }
			MotorChartMarksCalculator::const_shared_ptr mcmCalc;
			IMotorState::shared_ptr iMotorState;
		};
		//--------------------------------//
		MotorChartRadPSController();
		virtual ~MotorChartRadPSController()
		{
		}
		//Controller interface
		virtual void CloseController();
		//Controller interface

		virtual void ReOperateEntries(){ onMCZoneTimer(); }
		
		bool Initialize( const MCInitData& );

		//В какой зоне последний раз был radPS
		ZONE_ID getZone()const{ return lastZone; }

		inline float getMinRadPS()const{ return *minValue; }
		inline float getNormLowRadPS()const{ return *normLowValue; }
		inline float getNormHighRadPS()const{ return *normHighValue; }
		inline float getMaxRadPS()const{ return *maxValue; }

		inline BaseRadPSMark::const_shared_ptr getMinMark()const{ return minMark; }
		inline BaseRadPSMark::const_shared_ptr getNormLowMark()const{ return normLowMark; }
		inline BaseRadPSMark::const_shared_ptr getNormHighMark()const{ return normHighMark; }
		inline BaseRadPSMark::const_shared_ptr getMaxMark()const{ return maxMark; }

		inline BaseRadPSZone::const_shared_ptr getMinZone()const{ return minZone; }
		inline BaseRadPSZone::const_shared_ptr getNormZone()const{ return normZone; }
		inline BaseRadPSZone::const_shared_ptr getMaxZone()const{ return maxZone; }

#pragma region delegateInterface
		inline void addMinBelowOnceDel( traits::del_in  del ){ if( !minMark.isNull() )minMark->addBelowOnceDel( del ); }
		inline void addMinAboveOnceDel( traits::del_in  del ){ if( !minMark.isNull() )minMark->addAboveOnceDel( del ); }
		inline void addNormLowBelowOnceDel( traits::del_in  del ){ if( !normLowMark.isNull() )normLowMark->addBelowOnceDel( del ); }
		inline void addNormLowAboveOnceDel( traits::del_in  del ){ if( !normLowMark.isNull() )normLowMark->addAboveOnceDel( del ); }
		inline void addNormHighBelowOnceDel( traits::del_in  del ){ if( !normHighMark.isNull() )normHighMark->addBelowOnceDel( del ); }
		inline void addNormHighAboveOnceDel( traits::del_in  del ){ if( !normHighMark.isNull() )normHighMark->addAboveOnceDel( del ); }
		inline void addMaxBelowOnceDel( traits::del_in  del ){ if( !maxMark.isNull() )maxMark->addBelowOnceDel( del ); }
		inline void addMaxAboveOnceDel( traits::del_in  del ){ if( !maxMark.isNull() )maxMark->addAboveOnceDel( del ); }

		inline void addMinBelowTempDel( traits::del_in  del ){ if( !minMark.isNull() )minMark->addBelowTempDel( del ); }
		inline void addMinAboveTempDel( traits::del_in  del ){ if( !minMark.isNull() )minMark->addAboveTempDel( del ); }
		inline void addNormLowBelowTempDel( traits::del_in  del ){ if( !normLowMark.isNull() )normLowMark->addBelowTempDel( del ); }
		inline void addNormLowAboveTempDel( traits::del_in  del ){ if( !normLowMark.isNull() )normLowMark->addAboveTempDel( del ); }
		inline void addNormHighBelowTempDel( traits::del_in  del ){ if( !normHighMark.isNull() )normHighMark->addBelowTempDel( del ); }
		inline void addNormHighAboveTempDel( traits::del_in  del ){ if( !normHighMark.isNull() )normHighMark->addAboveTempDel( del ); }
		inline void addMaxBelowTempDel( traits::del_in  del ){ if( !maxMark.isNull() )maxMark->addBelowTempDel( del ); }
		inline void addMaxAboveTempDel( traits::del_in  del ){ if( !maxMark.isNull() )maxMark->addAboveTempDel( del ); }

		inline void removeMinBelowOnceDel( traits::del_in  del ){ if( !minMark.isNull() )minMark->removeBelowOnceDel( del ); }
		inline void removeMinAboveOnceDel( traits::del_in  del ){ if( !minMark.isNull() )minMark->removeAboveOnceDel( del ); }
		inline void removeNormLowBelowOnceDel( traits::del_in  del ){ if( !normLowMark.isNull() )normLowMark->removeBelowOnceDel( del ); }
		inline void removeNormLowAboveOnceDel( traits::del_in  del ){ if( !normLowMark.isNull() )normLowMark->removeAboveOnceDel( del ); }
		inline void removeNormHighBelowOnceDel( traits::del_in  del ){ if( !normHighMark.isNull() )normHighMark->removeBelowOnceDel( del ); }
		inline void removeNormHighAboveOnceDel( traits::del_in  del ){ if( !normHighMark.isNull() )normHighMark->removeAboveOnceDel( del ); }
		inline void removeMaxBelowOnceDel( traits::del_in  del ){ if( !maxMark.isNull() )maxMark->removeBelowOnceDel( del ); }
		inline void removeMaxAboveOnceDel( traits::del_in  del ){ if( !maxMark.isNull() )maxMark->removeAboveOnceDel( del ); }

		inline void removeMinBelowTempDel( traits::del_in  del ){ if( !minMark.isNull() )minMark->removeBelowTempDel( del ); }
		inline void removeMinAboveTempDel( traits::del_in  del ){ if( !minMark.isNull() )minMark->removeAboveTempDel( del ); }
		inline void removeNormLowBelowTempDel( traits::del_in  del ){ if( !normLowMark.isNull() )normLowMark->removeBelowTempDel( del ); }
		inline void removeNormLowAboveTempDel( traits::del_in  del ){ if( !normLowMark.isNull() )normLowMark->removeAboveTempDel( del ); }
		inline void removeNormHighBelowTempDel( traits::del_in  del ){ if( !normHighMark.isNull() )normHighMark->removeBelowTempDel( del ); }
		inline void removeNormHighAboveTempDel( traits::del_in  del ){ if( !normHighMark.isNull() )normHighMark->removeAboveTempDel( del ); }
		inline void removeMaxBelowTempDel( traits::del_in  del ){ if( !maxMark.isNull() )maxMark->removeBelowTempDel( del ); }
		inline void removeMaxAboveTempDel( traits::del_in  del ){ if( !maxMark.isNull() )maxMark->removeAboveTempDel( del ); }

		inline void addMinOnceDel( traits::del_in  del ){ if( !minZone.isNull() )minZone->addNormOnceDel( del ); }
		inline void addMinTempDel( traits::del_in  del ){ if( !minZone.isNull() )minZone->addNormTempDel( del ); }
		inline void removeMinOnceDel( traits::del_in  del ){ if( !minZone.isNull() )minZone->removeNormOnceDel( del ); }
		inline void removeMinTempDel( traits::del_in  del ){ if( !minZone.isNull() )minZone->removeNormTempDel( del ); }

		inline void addNormOnceDel( traits::del_in  del ){ if( !normZone.isNull() )normZone->addNormOnceDel( del ); }
		inline void addNormTempDel( traits::del_in  del ){ if( !normZone.isNull() )normZone->addNormTempDel( del ); }
		inline void removeNormOnceDel( traits::del_in  del ){ if( !normZone.isNull() )normZone->removeNormOnceDel( del ); }
		inline void removeNormTempDel( traits::del_in  del ){ if( !normZone.isNull() )normZone->removeNormTempDel( del ); }

		inline void addMaxOnceDel( traits::del_in  del ){ if( !maxZone.isNull() )maxZone->addNormOnceDel( del ); }
		inline void addMaxTempDel( traits::del_in  del ){ if( !maxZone.isNull() )maxZone->addNormTempDel( del ); }
		inline void removeMaxOnceDel( traits::del_in  del ){ if( !maxZone.isNull() )maxZone->removeNormOnceDel( del ); }
		inline void removeMaxTempDel( traits::del_in  del ){ if( !maxZone.isNull() )maxZone->removeNormTempDel( del ); }
#pragma endregion

	protected:
		void onMCZoneTimer();

		void onBelowMinZone(){ lastZone = ZONE_BELOW_MIN; }
		void onMinZone(){ lastZone = ZONE_MIN; }
		void onNormZone(){ lastZone = ZONE_NORM; }
		void onMaxZone(){ lastZone = ZONE_MAX; }
		void onAboveMaxZone(){ lastZone = ZONE_ABOVE_MAX; }

		delegate_traits::del0<void>::shared_ptr belowMinZoneDel;
		delegate_traits::del0<void>::shared_ptr minZoneDel;
		delegate_traits::del0<void>::shared_ptr normZoneDel;
		delegate_traits::del0<void>::shared_ptr maxZoneDel;
		delegate_traits::del0<void>::shared_ptr aboveMaxZoneDel;

		ZONE_ID lastZone;

		delegate_traits::del0<void>::shared_ptr cdZoneDel;
		delegate_traits::del0<void>::shared_ptr cdSpeedDel;

		IMotorState::const_shared_ptr iMotorState;
		MotorChartMarksCalculator::const_shared_ptr mcmCalc;

		Ogre::SharedPtr< float > minValue;
		Ogre::SharedPtr< float > normLowValue;
		Ogre::SharedPtr< float > normHighValue;
		Ogre::SharedPtr< float > maxValue;

		BaseRadPSMark::shared_ptr minMark;
		BaseRadPSMark::shared_ptr normLowMark;
		BaseRadPSMark::shared_ptr normHighMark;
		BaseRadPSMark::shared_ptr maxMark;
		
		BaseRadPSZone::shared_ptr minZone;
		BaseRadPSZone::shared_ptr normZone;
		BaseRadPSZone::shared_ptr maxZone;
	};
	//------------------------------------------------------------
}
}

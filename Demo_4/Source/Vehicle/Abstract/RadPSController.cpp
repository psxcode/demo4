#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "RadPSController.h"
#include "IPedalState.h"
#include "Interface\UnitConverter.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	typedef UnitConverter UI;
	//------------------------------------------------------------
	const float BaseRadPSTimingCalculator::timeLimit = 0.001f;
	//------------------------------------------------------------
	bool ConstantRadPSMark::Initialize( float markValue )
	{
		this->mark = markValue;
		return true;
	}
	//------------------------------------------------------------
	bool ConstantRadPSZone::Initialize(const ConstantRadPSZone::ConstantZone &constZone)
	{
		this->zone = constZone;
		range = zone.second - zone.first;
		return true;
	}
	//------------------------------------------------------------
	bool LinearRadPSMark::Initialize(const LinearRadPSMark::LinMarkInitData &idata)
	{
		if( idata.iStyle.isNull() )return false;
		this->mark = idata.mark;
		this->iStyle = idata.iStyle;
		return true;
	}
	//------------------------------------------------------------
	bool LinearRadPSZone::Initialize(const LinearRadPSZone::LinZoneInitData &idata)
	{
		if( idata.iStyle.isNull() )return false;
		this->zone = idata.zone;
		this->iStyle = idata.iStyle;
		return true;
	}
	//------------------------------------------------------------
	void BaseRadPSMark::Operate(float radPS)
	{
		if( radPS < GetMark() )
		{
			if( !_isBelow )MoveBelow();
		}
		else
		{
			if( !_isAbove )MoveAbove();
		}
	}
	//------------------------------------------------------------
	void BaseRadPSZone::Operate(float radPS)
	{
		lastRadPS = radPS;
		if( radPS < GetLow() )
		{
			if( !_isBelow )MoveBelow();
		}
		else if( radPS > GetHigh() )
		{
			if( !_isAbove )MoveAbove();
		}
		else
		{
			if( !_isNorm )MoveNorm();
		}
	}
	//------------------------------------------------------------
	void RadPSController::OperateController()
	{
		zoneTimer.Operate();
		speedTimer.Operate();
	}
	//------------------------------------------------------------
	bool RadPSController::Initialize(const InitData& idata )
	{
		if( !idata.isValid() )return false;

		timing = idata.timing;
		iState = idata.mechanicsState;

		zoneDel.bind( new delegate_traits::del0<void>( this, &RadPSController::onZoneTimer ) );
		speedDel.bind( new delegate_traits::del0<void>( this, &RadPSController::onSpeedTimer ) );

		zoneTimer.AddOnceDel( zoneDel );
		zoneTimer.Reset( timing->GetZoneReactionTime() );
		speedTimer.AddOnceDel( speedDel );
		speedTimer.Reset( timing->GetSpeedReactionTime() );

		setInitializeController();
		return true;
	}
	//------------------------------------------------------------
	void RadPSController::CloseController()
	{
		zoneTimer.RemoveAllOnceDels();
		speedTimer.RemoveAllOnceDels();
		zoneDel.setNull();
		speedDel.setNull();
		//closeEntries
		eMap.clear();
		Core::Controller::CloseController();
	}
	//------------------------------------------------------------
	bool RadPSController::addEntry( const traits::entry_container::value_type& pair )
	{
		return eMap.insert( pair ).second;
	}
	//------------------------------------------------------------
	void RadPSController::removeEntry( int id )
	{
		eMap.erase( id );
	}
	//------------------------------------------------------------
	BaseRadPSEntry::shared_ptr RadPSController::getEntry(int id)
	{
		traits::entry_container::iterator iter = eMap.find( id );
		if( iter != eMap.end() ) {
			return iter->second;
		} else {
			return BaseRadPSEntry::shared_ptr();
		}
	}
	//------------------------------------------------------------
	BaseRadPSEntry::const_shared_ptr RadPSController::getEntry(int id)const
	{
		traits::entry_container::const_iterator iter = eMap.find( id );
		if( iter != eMap.end() ) {
			return iter->second;
		} else {
			return BaseRadPSEntry::const_shared_ptr();
		}
	}
	//------------------------------------------------------------
	void RadPSController::onZoneTimer()
	{
		//operateEntries
		traits::entry_container::iterator iter = eMap.begin();
		traits::entry_container::iterator end_iter = eMap.end();
		for( ; iter != end_iter; ++iter ) {
			iter->second->Operate( iState->getRadPS() );
		}
		zoneTimer.Reset( timing->GetZoneReactionTime() );
	}
	//------------------------------------------------------------
	void RadPSController::onSpeedTimer()
	{
		speedMeasured = iState->getRadPS() - startRadPS;
		speedMeasureCallback();
		if( speedMeasured < 0 ) {
			speedDecMeasureCallback();
		} else {
			speedIncMeasureCallback();
		}
	}
	//------------------------------------------------------------
	void RadPSController::removeSpeedDel(traits::del_in del)
	{
		speedMeasureCallback.remove( del );
		speedDecMeasureCallback.remove( del );
		speedIncMeasureCallback.remove( del );
	}
	//------------------------------------------------------------
	void RadPSController::measureSpeed( float timeMult )
	{
		if( speedTimer.IsWorking() )
		{
			if( speedTimer.GetTimeLeft() > timing->GetSpeedReactionTime() * timeMult )
				speedTimer.SetTargetTime( timing->GetSpeedReactionTime() * timeMult );
		}
		else
		{
			speedTimer.Reset( timing->GetSpeedReactionTime() * timeMult );
		}
	}
	//------------------------------------------------------------
	void RadPSController::measureSpeed( float timeMult, traits::del_in tempDel )
	{
		if( tempDel != 0 ) {
			speedMeasureCallback.add( tempDel );
		}
		measureSpeed( timeMult );
	}
	//------------------------------------------------------------
	void RadPSController::measureSpeedIncrease( float timeMult, traits::del_in tempDel )
	{
		if( tempDel != 0 ) {
			speedIncMeasureCallback.add( tempDel );
		}
		measureSpeed( timeMult );
	}
	//------------------------------------------------------------
	void RadPSController::measureSpeedDecrease( float timeMult, traits::del_in tempDel )
	{
		if( tempDel != 0 ) {
			speedDecMeasureCallback.add( tempDel );
		}
		measureSpeed( timeMult );
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	MotorChartRadPSController::MotorChartRadPSController()
		: minValue( new float(0.0f) )
		, normLowValue( new float(0.0f) )
		, normHighValue( new float(0.0f) )
		, maxValue( new float(0.0f) )
		, lastZone( ZONE_NOT_DEFINED )
	{
	}
	//------------------------------------------------------------
	bool MotorChartRadPSController::Initialize(const MCInitData &data)
	{
		if( !data.isValid() )return false;

		timing = data.timing;
		iState = data.iMotorState;
		iMotorState = data.iMotorState;
		mcmCalc = data.mcmCalc;

		cdZoneDel.bind( new delegate_traits::del0<void>( this, &MotorChartRadPSController::onMCZoneTimer ) );
		cdSpeedDel.bind( new delegate_traits::del0<void>( this, &MotorChartRadPSController::onSpeedTimer ) );

		belowMinZoneDel.bind( new delegate_traits::del0<void>( this, &MotorChartRadPSController::onBelowMinZone ) );
		minZoneDel.bind( new delegate_traits::del0<void>( this, &MotorChartRadPSController::onMinZone ) );
		normZoneDel.bind( new delegate_traits::del0<void>( this, &MotorChartRadPSController::onNormZone ) );
		maxZoneDel.bind( new delegate_traits::del0<void>( this, &MotorChartRadPSController::onMaxZone ) );
		aboveMaxZoneDel.bind( new delegate_traits::del0<void>( this, &MotorChartRadPSController::onAboveMaxZone ) );

		zoneTimer.AddOnceDel( cdZoneDel );
		zoneTimer.Reset( timing->GetZoneReactionTime() );
		speedTimer.AddOnceDel( cdSpeedDel );
		speedTimer.Reset( timing->GetSpeedReactionTime() );

		ExtFeedRadPSMark* extFeedMark(0);
		ExtFeedRadPSZone* extFeedZone(0);

		//min mark
		{
			extFeedMark = new ExtFeedRadPSMark();
			extFeedMark->extMark = minValue;
			extFeedMark->addBelowOnceDel( belowMinZoneDel );
			minMark.bind( extFeedMark );
			traits::entry_container::_Pairib ipair = eMap.insert( traits::entry_container::value_type( RID_MIN_MARK, minMark ) );
			if( !ipair.second )return false;
		}
		//normLow mark
		{
			extFeedMark = new ExtFeedRadPSMark();
			extFeedMark->extMark = normLowValue;
			normLowMark.bind( extFeedMark );
			traits::entry_container::_Pairib ipair = eMap.insert( traits::entry_container::value_type( RID_NORM_LOW_MARK, normLowMark ) );
			if( !ipair.second )return false;
		}
		//normHigh mark
		{
			extFeedMark = new ExtFeedRadPSMark();
			extFeedMark->extMark = normHighValue;
			normHighMark.bind( extFeedMark );
			traits::entry_container::_Pairib ipair = eMap.insert( traits::entry_container::value_type( RID_NORM_HIGH_MARK, normHighMark ) );
			if( !ipair.second )return false;
		}
		//max mark
		{
			extFeedMark = new ExtFeedRadPSMark();
			extFeedMark->extMark = maxValue;
			extFeedMark->addAboveOnceDel( aboveMaxZoneDel );
			maxMark.bind( extFeedMark );
			traits::entry_container::_Pairib ipair = eMap.insert( traits::entry_container::value_type( RID_MAX_MARK, maxMark ) );
			if( !ipair.second )return false;
		}
		//minZone
		{
			extFeedZone = new ExtFeedRadPSZone();
			extFeedZone->extZone.first = minValue;
			extFeedZone->extZone.second = normLowValue;
			extFeedZone->addNormOnceDel( minZoneDel );
			minZone.bind( extFeedZone );
			traits::entry_container::_Pairib ipair = eMap.insert( traits::entry_container::value_type( RID_MIN_ZONE, minZone ) );
			if( !ipair.second )return false;
		}
		//normZone
		{
			extFeedZone = new ExtFeedRadPSZone();
			extFeedZone->extZone.first = normLowValue;
			extFeedZone->extZone.second = normHighValue;
			extFeedZone->addNormOnceDel( normZoneDel );
			normZone.bind( extFeedZone );
			traits::entry_container::_Pairib ipair = eMap.insert( traits::entry_container::value_type( RID_NORM_ZONE, normZone ) );
			if( !ipair.second )return false;
		}
		//maxZone
		{
			extFeedZone = new ExtFeedRadPSZone();
			extFeedZone->extZone.first = normHighValue;
			extFeedZone->extZone.second = maxValue;
			extFeedZone->addNormOnceDel( maxZoneDel );
			maxZone.bind( extFeedZone );
			traits::entry_container::_Pairib ipair = eMap.insert( traits::entry_container::value_type( RID_MAX_ZONE, maxZone ) );
			if( !ipair.second )return false;
		}
		return mInitializedController = true;
	}
	//------------------------------------------------------------
	void MotorChartRadPSController::CloseController()
	{
		cdZoneDel.setNull();
		cdSpeedDel.setNull();

		minMark->removeBelowOnceDel( belowMinZoneDel );
		belowMinZoneDel.setNull();

		minZone->removeNormOnceDel( minZoneDel );
		minZoneDel.setNull();

		normZone->removeNormOnceDel( normZoneDel );
		normZoneDel.setNull();
		
		maxZone->removeNormOnceDel( maxZoneDel );
		maxZoneDel.setNull();
		
		maxMark->removeAboveOnceDel( aboveMaxZoneDel );
		aboveMaxZoneDel.setNull();
		
		RadPSController::CloseController();
	}
	//------------------------------------------------------------
	void MotorChartRadPSController::onMCZoneTimer()
	{
		const MotorChart& chart( iMotorState->getMotorChart() );
		float idle( (float)UI::RPMtoRadPS * chart.idleRPM );
		float range( (float)UI::RPMtoRadPS * chart.maxRPM - idle );
		*minValue = mcmCalc->GetMin() * range + idle;
		*normLowValue = mcmCalc->GetNormLow() * range + idle;
		*normHighValue = mcmCalc->GetNormHigh() * range + idle;
		*maxValue = mcmCalc->GetMax() * range + idle;
		onZoneTimer();
	}
	//------------------------------------------------------------
	void RadPSController::ReCallEntries()
	{
		traits::entry_container::iterator iter = eMap.begin();
		traits::entry_container::iterator end_iter = eMap.end();
		for( ; iter != end_iter; ++iter )
			iter->second->ReCallDelegates();
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
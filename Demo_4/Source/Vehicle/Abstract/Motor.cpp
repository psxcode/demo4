#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "Motor.h"
#include "Flywheel.h"
#include "Interface\TimeInterface.h"
#include "Interface\UnitConverter.h"
	
namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	typedef TimeInterface FTI;
	typedef UnitConverter UI;
	//------------------------------------------------------------
	bool Motor::Initialize( const MotorInitData &data )
	{
		if( isInitializedMechanics() )return false;
		if( shared_ptr_null(data.motorParams) || !data.motorParams->isValid() )return false;
		if( shared_ptr_null(data.motorParams->torqueLookup) )return false;
		if( shared_ptr_null(data.motorParams->dragLookup) )return false;
		if( shared_ptr_null(data.motorParams->flywheel) || !data.motorParams->flywheel->isValid() )return false;

		//name
		nameMechanics = data.motorParams->nameMechanics;

		//Lookup
		torqueLookup = data.motorParams->torqueLookup->CreateCopyPtr();
		torqueRadPSLookup = torqueLookup->CreateCopyPtr();
		torqueRadPSLookup->MultiplyVBV( float(UI::RPMtoRadPS), 1.0f );
		dragLookup = data.motorParams->dragLookup->CreateCopyPtr();
		dragRadPSLookup = dragLookup->CreateCopyPtr();
		dragRadPSLookup->MultiplyVBV( float(UI::RPMtoRadPS), 1.0f );
		maxTorque = data.motorParams->maxTorque;
		maxStarterTorque = data.motorParams->maxStarterTorque;
		maxDrag = data.motorParams->maxDrag;

		//Flywheel
		Flywheel::FlywheelInitData fiData;
		fiData.flywheelParams = data.motorParams->flywheel;
		flywheel.bind( new Flywheel() );
		if( !flywheel->Initialize( fiData ) )return false;

		//Chart
		chart = data.motorParams->chart;

		//operate
		operateDel.bind( this, &Motor::operateNoConnection );

		//clutchState
		clutchState = data.clutchState;

		//torque multiplier calc function
		if( belowIdleTorqueMult > 1.0f )
		{
			torqueMultOnDel.bind( new delegate_traits::del0<void>( this, &Motor::setTorqueMultOn ) );
			torqueMultOffDel.bind( new delegate_traits::del0<void>( this, &Motor::setTorqueMultOff ) );
			switch( torqueMultFunc )
			{
			case MotorParams::TMF_LINEAR:
				activeTorqueMultCalcDel.bind( this, &Motor::activeTorqueMultCalc_Linear );
				break;
			default:
				activeTorqueMultCalcDel.bind( this, &Motor::activeTorqueMultCalc_Const );
				break;
			}
			createStallIdleZone();
			stallIdleZone->addNormOnceDel( torqueMultOnDel );
			stallIdleZone->addAboveOnceDel( torqueMultOffDel );
			stallIdleZone->addBelowOnceDel( torqueMultOffDel );
		}

		//torque calc function
		torqueCalcDel.bind( this, &Motor::torqueCalc_MotorOff );

		stallMotorDel.bind( new delegate_traits::del0<void>( this, &Motor::onStallMotor ) );
		startMotorDel.bind( new delegate_traits::del0<void>( this, &Motor::onStartMotor ) );
		AddStallRPMDel( stallMotorDel );
		AddStartRPMDel( startMotorDel );

		//работа с коробкой передач
		gearboxState = data.gearboxState;
		gearboxCallback = data.gearboxCallback;
		gearboxShiftDel.bind( new delegate_traits::del0<void>( this, &Motor::operateConnectionChanged ) );
		if( shared_ptr_valid(gearboxCallback) )
		{
			gearboxCallback->AddShiftCompletedOnceDel( gearboxShiftDel );
			gearboxCallback->AddShiftInitiatedOnceDel( gearboxShiftDel );
			gearboxCallback->AddShiftFailedOnceDel( gearboxShiftDel );
		}
		
		return initializedMechanics = true;
	}
	//------------------------------------------------------------
	void Motor::CloseMotor()
	{
		//Закрываем параметры из VehicleMotorParams
		chart = MotorChart();
		maxTorque = 0;
		torqueLookup.reset();
		torqueRadPSLookup.reset();
		dragRadPSLookup.reset();
		flywheel.reset();
				
		//Закрываем temp данные
		throttle = Core::puFloat();
		clutchState.reset();
		if(shared_ptr_valid(childMechanics))
		{ 
			callDisconnectingFromParent(childMechanics);
			childMechanics.reset();
		}
		operateDel.clear();

		Mechanics::CloseMechanics();
	}
	//------------------------------------------------------------
	bool Motor::ConnectChildMech( traits::child_mech_in m, int )
	{
		if( shared_ptr_null(m) || !m->isInitializedMechanics() )return false;
		if( shared_ptr_valid(childMechanics) )
		{
			if( childMechanics == m )return true;
			else return false;
		}
		if( !callConnectingToParent( m ) )return false;
		childMechanics = m;
		operateConnectionChanged();
		return true;
	}
	//------------------------------------------------------------
	void Motor::DisconnectChildMech( int )
	{
		if( shared_ptr_valid(childMechanics) )
		{
			callDisconnectingFromParent( childMechanics );
			childMechanics.reset();
		}
		operateConnectionChanged();
	}
	//------------------------------------------------------------
	void Motor::DisconnectChildMech( traits::child_mech_in m )
	{
		if( shared_ptr_valid(childMechanics) && childMechanics == m )
			DisconnectChildMech(0);
	}
	//------------------------------------------------------------
	float Motor::OperateMechanics( float, float )
	{
		torqueCalcDel();
		drag = dragRadPSLookup->GetValue( radPS );
		operateDel();
		radPSController->OperateController();
		return radPS > 0 ? radPS : radPS = 0;
	}
	//------------------------------------------------------------
	void Motor::operateNoConnection()
	{
		radPS += (torque - drag) * FTI::frameTime() / flywheel->getFlywheelInertia();
	}
	//------------------------------------------------------------
	void Motor::operateWithConnection()
	{
		float childDeltaRadPS = childMechanics->OperateMechanics( torque, drag ) - radPS;
		float ownDeltaRadPS = (torque - drag) * FTI::frameTime() / flywheel->getFlywheelInertia();
		radPS += (( 1 - clutchState->GetCurrentValue()) * childDeltaRadPS) + (clutchState->GetCurrentValue() * ownDeltaRadPS);
	}
	//------------------------------------------------------------
	void Motor::operateWithConnectionNoClutch()
	{
		radPS = childMechanics->OperateMechanics( torque, drag );
	}
	//------------------------------------------------------------
	void Motor::operateGearboxNeutral()
	{
		operateNoConnection();
		childMechanics->OperateMechanics( 0.0f, 0.0f );
	}
	//------------------------------------------------------------
	void Motor::operateConnectionChanged()
	{
		if( shared_ptr_valid(childMechanics) ) {
			if( shared_ptr_valid(clutchState) )	{
				if( shared_ptr_valid(gearboxState) ) {
					if( gearboxState->GetActiveGearIndex() != 0 )
						operateDel.bind( this, &Motor::operateWithConnection );
					else operateDel.bind( this, &Motor::operateGearboxNeutral );
				}
				else operateDel.bind( this, &Motor::operateWithConnection );
			}
			else operateDel.bind( this, &Motor::operateWithConnectionNoClutch );
		}
		else operateDel.bind( this, &Motor::operateNoConnection );
	}
	//------------------------------------------------------------
	IMotorControl::shared_ptr Motor::GetControlInterface()
	{
		if( shared_ptr_null(iControl) ) iControl.bind( new IMotorControl( *this ) );
		return iControl;
	}
	//------------------------------------------------------------
	IMotorState::shared_ptr Motor::GetStateInterface()
	{
		if( shared_ptr_null(iState) ) iState.bind( new IMotorState( *this ) );
		return iState;
	}
	//------------------------------------------------------------
	IMotorCallback::shared_ptr Motor::GetCallbackInterface()
	{
		if( shared_ptr_null(iCallback) ) iCallback.bind( new IMotorCallback( *this ) );
		return iCallback;
	}
	//------------------------------------------------------------
	IMotor::shared_ptr Motor::GetInterface( MOTOR_INTERFACE_TYPE t )
	{
		switch( t )
		{
		case MIT_CONTROL: return GetControlInterface();
		case MIT_STATE:	return GetStateInterface();
		case MIT_CALLBACK: return GetCallbackInterface();
		default: return IMotor::shared_ptr();
		};
	}
	//------------------------------------------------------------
	bool Motor::isMechanicsChainValid()const
	{
		return isInitializedMechanics() && shared_ptr_valid(childMechanics) ? childMechanics->isMechanicsChainValid() : true;
	}
	//------------------------------------------------------------
	float Motor::getFlywheelInertia() const
	{ 
		return shared_ptr_valid(flywheel) ? flywheel->getFlywheelInertia() : 0;
	}
	//------------------------------------------------------------
	MotorParams::shared_ptr Motor::CreateMotorParams() const
	{
		MotorParams::shared_ptr m( new MotorParams() );

		m->nameMechanics = nameMechanics;
		m->chart = chart;
		m->maxDrag = maxDrag;
		m->maxTorque = maxTorque;
		m->maxStarterTorque = maxStarterTorque;
		m->belowIdleTorqueMult = belowIdleTorqueMult;
		m->belowIdleTorqueMultFunc = torqueMultFunc;
		if( shared_ptr_valid(dragLookup) )
			m->dragLookup = dragLookup->CreateCopyPtr();
		if( shared_ptr_valid(torqueLookup) )
			m->torqueLookup = torqueLookup->CreateCopyPtr();
		if( shared_ptr_valid(flywheel) )
			m->flywheel = flywheel->CreateFlywheelParams();
		
		return m;
	}
	//------------------------------------------------------------
	void Motor::createRadPSController()
	{
		if( shared_ptr_valid(radPSController) )return;
		//prepare timing
		RadPSTiming calcTiming;
		calcTiming.speedReactionTime = 0.001f;
		calcTiming.zoneReactionTime = 0.001f;
		//prepare timingCalculator
		ConstantRadPSTimingCalculator::shared_ptr ctcalc( new ConstantRadPSTimingCalculator() );
		ctcalc->Initialize( calcTiming );
		//prepare radPSConstroller::InitData
		RadPSController::InitData idata;
		idata.timing = ctcalc;
		idata.mechanicsState = GetStateInterface();
		radPSController.bind( new RadPSController() );
		radPSController->Initialize( idata );
	}
	//------------------------------------------------------------
	void Motor::AddMaxRPMDel(traits::del_in del)
	{
		if( shared_ptr_null(radPSController) ) createRadPSController();
		if( shared_ptr_null(maxMark) ) createMaxMark();
		maxMark->addAboveOnceDel( del );
	}
	//------------------------------------------------------------
	void Motor::AddStallRPMDel(traits::del_in del)
	{
		if( shared_ptr_null(radPSController) ) createRadPSController();
		if( shared_ptr_null(stallMark) ) createStallMark();
		stallMark->addBelowOnceDel( del );
	}
	//------------------------------------------------------------
	void Motor::AddStartRPMDel(traits::del_in del)
	{
		if( shared_ptr_null(radPSController) ) createRadPSController();
		if( shared_ptr_null(startMark) ) createStartMark();
		startMark->addAboveOnceDel( del );
	}
	//------------------------------------------------------------
	void Motor::AddRedRPMDel(traits::del_in del)
	{
		if( shared_ptr_null(radPSController) ) createRadPSController();
		if( shared_ptr_null(redMark) ) createRedMark();
		redMark->addAboveOnceDel( del );
	}
	//------------------------------------------------------------
	void Motor::RemoveMaxRpmDel(traits::del_in del)
	{
		if( shared_ptr_valid(maxMark) ) maxMark->removeAboveOnceDel( del );
	}
	//------------------------------------------------------------
	void Motor::RemoveStallRPMDel(traits::del_in del)
	{
		if( shared_ptr_valid(stallMark) ) stallMark->removeBelowOnceDel( del );
	}
	//------------------------------------------------------------
	void Motor::RemoveStartRPMDel(traits::del_in del)
	{
		if( shared_ptr_valid(startMark) ) startMark->removeAboveOnceDel( del );
	}
	//------------------------------------------------------------
	void Motor::RemoveRedRPMDel(traits::del_in del)
	{
		if( shared_ptr_valid(redMark) ) redMark->removeAboveOnceDel( del );
	}
	//------------------------------------------------------------
	void Motor::createMaxMark()
	{
		if( shared_ptr_valid(maxMark) )return;
		if( radPSController == 0 )createRadPSController();
		ConstantRadPSMark* pMaxMark = new ConstantRadPSMark();
		pMaxMark->Initialize( float(UI::RPMtoRadPS * chart.maxRPM) );
		BaseRadPSEntry::shared_ptr ptr( pMaxMark );
		radPSController->addEntry( RadPSController::traits::entry_container::value_type( RID_MAX_MARK, ptr ) );
	}
	//------------------------------------------------------------
	void Motor::createRedMark()
	{
		if( shared_ptr_valid(redMark) )return;
		if( radPSController == 0 )createRadPSController();
		ConstantRadPSMark* pRedMark = new ConstantRadPSMark();
		pRedMark->Initialize( float(UI::RPMtoRadPS * chart.redRPM) );
		BaseRadPSEntry::shared_ptr ptr( pRedMark );
		radPSController->addEntry( RadPSController::traits::entry_container::value_type( RID_RED_MARK, ptr ) );
	}
	//------------------------------------------------------------
	void Motor::createStallMark()
	{
		if( shared_ptr_valid(stallMark) )return;
		if( radPSController == 0 )createRadPSController();
		ConstantRadPSMark* pStallMark = new ConstantRadPSMark();
		pStallMark->Initialize( float(UI::RPMtoRadPS * chart.stallRPM) );
		stallMark.bind( pStallMark );
		radPSController->addEntry( RadPSController::traits::entry_container::value_type( RID_STALL_MARK, stallMark ) );
	}
	//------------------------------------------------------------
	void Motor::createStartMark()
	{
		if( shared_ptr_valid(startMark) )return;
		if( radPSController == 0 )createRadPSController();
		ConstantRadPSMark* pStartMark = new ConstantRadPSMark();
		pStartMark->Initialize( float(UI::RPMtoRadPS * chart.startRPM) );
		startMark.bind( pStartMark );
		radPSController->addEntry( RadPSController::traits::entry_container::value_type( RID_START_MARK, startMark ) );
	}
	//------------------------------------------------------------
	void Motor::createStallIdleZone()
	{
		if( shared_ptr_valid(stallIdleZone) )return;
		if( radPSController == 0 )createRadPSController();
		ConstantRadPSZone* pStallIdleZone = new ConstantRadPSZone();
		pStallIdleZone->Initialize( ConstantRadPSZone::ConstantZone( float(UI::RPMtoRadPS*chart.stallRPM), float(UI::RPMtoRadPS*chart.idleRPM) ) );
		stallIdleZone.bind( pStallIdleZone );
		radPSController->addEntry( RadPSController::traits::entry_container::value_type( RID_STALL_IDLE_ZONE, stallIdleZone ) );
	}
	//------------------------------------------------------------
	void Motor::onStallMotor()
	{
		_isStall = true;
		stallMark->setActive( false );
		startMark->setActive( true );
		torqueCalcChanged();
	}
	//------------------------------------------------------------
	void Motor::onStartMotor()
	{
		_isStall = false;
		startMark->setActive( false );
		stallMark->setActive( true );
		torqueCalcChanged();
	}
	//------------------------------------------------------------
	void Motor::torqueCalcChanged()
	{
		if( _isStall )
		{
			torqueCalcDel.bind( this, &Motor::torqueCalc_MotorOff );
		}
		else
		{
			if( _isTorqueMultActive )
				torqueCalcDel.bind( this, &Motor::torqueCalc_MotorOn_TorqueMultOn );
			else
				torqueCalcDel.bind( this, &Motor::torqueCalc_MotorOn_TorqueMultOff );
		}
	}
	//------------------------------------------------------------
	void Motor::torqueCalc_MotorOn_TorqueMultOn()
	{
		activeTorqueMultCalcDel();
		torque = torqueRadPSLookup->GetValue( radPS ) * maxTorque * throttle.get() * activeTorqueMult;
	}
	//------------------------------------------------------------
	void Motor::torqueCalc_MotorOn_TorqueMultOff()
	{
		torque = torqueRadPSLookup->GetValue( radPS ) * maxTorque * throttle.get();
	}
	//------------------------------------------------------------
	void Motor::torqueCalc_MotorOff()
	{
		torque = activeStarterTorque;
	}
	//------------------------------------------------------------
	void Motor::activeTorqueMultCalc_Const()
	{
		activeTorqueMult = belowIdleTorqueMult;
	}
	//------------------------------------------------------------
	void Motor::activeTorqueMultCalc_Linear()
	{
		activeTorqueMult = belowIdleTorqueMult * stallIdleZone->GetRangePos().get();
		activeTorqueMult = std::max( 1.0f, activeTorqueMult );
	}
	//------------------------------------------------------------
	void Motor::setTorqueMultOn()
	{
		_isTorqueMultActive = true;
		torqueCalcChanged();
	}
	//------------------------------------------------------------
	void Motor::setTorqueMultOff()
	{
		_isTorqueMultActive = false;
		torqueCalcChanged();
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
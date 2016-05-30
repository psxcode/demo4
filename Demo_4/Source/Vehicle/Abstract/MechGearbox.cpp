#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "MechGearbox.h"
#include "IGearboxControl.h"
#include "IGearboxState.h"
#include "IGearboxCallback.h"
#include "Flywheel.h"
#include "Interface/TimeInterface.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	typedef TimeInterface FTI;
	const float MechGearbox::clutchLimit = 0.1f;
	//------------------------------------------------------------
	bool MechGearbox::Initialize( const GearboxInitData& data )
	{
		if( isInitializedMechanics() ) return false;
		if( shared_ptr_null(data.gearboxParams) ) return false;
		if( shared_ptr_null(data.gearboxParams->flywheel) ) return false;

		//Mechanics параметры
		nameMechanics = data.gearboxParams->nameMechanics;
		maxDrag = data.gearboxParams->maxDrag;
		if( shared_ptr_valid(data.gearboxParams->dragLookup) )
			dragLookup = data.gearboxParams->dragLookup->CreateCopyPtr();

		//Gearbox параметры
		//маховик
		Flywheel::FlywheelInitData fData;
		fData.flywheelParams = data.gearboxParams->flywheel;
		flywheel.bind( new Flywheel() );
		if( !flywheel->Initialize( fData ) )return false;
		//время
		mechShiftTime = data.gearboxParams->mechShiftTime;
		if( mechShiftTime < timeLimit )mechShiftTime = timeLimit;
		//коэффициенты передач
		directRatios = data.gearboxParams->directRatios;
		reverseRatios = data.gearboxParams->reverseRatios;
		directCount = directRatios.size();
		reverseCount = reverseRatios.size();
		if( directCount <= 0 )return false;

		//Собственные параметры
		shiftOpDel.bind( new delegate_traits::del0<void>( this, &MechGearbox::OnShiftOp ) );
		operateDel.bind( this, &MechGearbox::OperateNoConnection );
		shiftOpTimer.AddOnceDel( shiftOpDel );
		
		return initializedMechanics = true;
	}
	//------------------------------------------------------------
	void MechGearbox::CloseMechanics()
	{
		isShiftInitiated = false;

		shiftOpTimer.CloseDelTimer();
		shiftOpDel.reset();
		operateDel.clear();
		iControl.reset();
		iCallback.reset();
		iState.reset();

		Gearbox::CloseMechanics();
	}
	//------------------------------------------------------------
	void MechGearbox::CalcActiveRatio()
	{
		if( CheckGear( activeGear ) )
		{
			if( activeGear > 0 )
			{
				activeRatioSign = activeRatioMod = directRatios[ activeGear - 1 ];
				return;
			}
			else if( activeGear < 0 )
			{
				int gear = -activeGear;
				activeRatioMod = reverseRatios[ gear - 1 ];
				activeRatioSign = -activeRatioMod;
				return;
			}
		}
		activeRatioMod = activeRatioSign = 0;
	}
	//------------------------------------------------------------
	bool MechGearbox::ShiftToNeutral()
	{
		if( isShiftInitiated )return false;

		//Проверка нажатия сцепления
		if( clutch.GetCurrentValue() < (1 - clutchLimit) )return false;

		targetGear = 0;
		activeGear = 0;
		shiftOpTimer.Reset( mechShiftTime );
		CallShiftInitiated();
		operateConnectionChanged();
		return isShiftInitiated = true;
	}
	//------------------------------------------------------------
	bool MechGearbox::ShiftTo( int gearIndex )
	{
		if( isShiftInitiated )return false;

		//Проверка нажатия сцепления
		if( clutch.GetCurrentValue() < (1 - clutchLimit) )return false;

		if( !CheckGear( gearIndex ) )return false;

		targetGear = gearIndex;
		activeGear = 0;
		shiftOpTimer.Reset( mechShiftTime );
		CallShiftInitiated();
		operateConnectionChanged();
		return isShiftInitiated = true;
	}
	//------------------------------------------------------------
	bool MechGearbox::ShiftToReverse( int gearIndex )
	{
		if( isShiftInitiated )return false;

		//Проверка нажатия сцепления
		if( clutch.GetCurrentValue() < (1 - clutchLimit) )return false;

		if( gearIndex > int(reverseCount) )return false;

		targetGear = -gearIndex;
		activeGear = 0;
		shiftOpTimer.Reset( mechShiftTime );
		CallShiftInitiated();
		operateConnectionChanged();
		return isShiftInitiated = true;
	}
	//------------------------------------------------------------
	void MechGearbox::OnShiftOp()
	{
		if( !isShiftInitiated )return;

		activeGear = targetGear;
		CalcActiveRatio();
		isShiftInitiated = false;
		operateConnectionChanged();
		CallShiftCompleted();
	}
	//------------------------------------------------------------
	bool MechGearbox::CheckGear( int gearIndex )
	{
		if( gearIndex > 0 )
		{
			if( gearIndex > int(directCount) )
				return false;
		}
		else if( gearIndex < 0 )
		{
			int gear = -gearIndex;
			if( gear > int(reverseCount) )
				return false;
		}
		else return true;
		return true;
	}
	//------------------------------------------------------------
	void MechGearbox::CallShiftInitiated()
	{
		shiftInitiatedOnceCall();
		if( !shiftInitiatedTempCall.empty() )
		{
			shiftInitiatedTempCall();
			shiftInitiatedTempCall.clear();
		}
	}
	//------------------------------------------------------------
	void MechGearbox::CallShiftCompleted()
	{
		shiftCompletedOnceCall();
		shiftCompletedTempCall();
		shiftCompletedTempCall.clear();
		shiftFailedOnceCall.clear();
		shiftFailedTempCall.clear();
	}
	//------------------------------------------------------------
	void MechGearbox::CallShiftFailed()
	{
		shiftFailedOnceCall();
		shiftFailedTempCall();
		shiftFailedTempCall.clear();
		shiftCompletedOnceCall.clear();
		shiftCompletedTempCall.clear();
	}
	//------------------------------------------------------------
	IMechGearboxCallback::shared_ptr MechGearbox::GetCallbackInterface()
	{
		if( shared_ptr_null(iCallback) ) iCallback.bind( new IMechGearboxCallback( *this ) );
		return iCallback;
	}
	//------------------------------------------------------------
	IMechGearboxControl::shared_ptr MechGearbox::GetControlInterface()
	{
		if( shared_ptr_null(iControl) ) iControl.bind( new IMechGearboxControl( *this ) );
		return iControl;
	}
	//------------------------------------------------------------
	IMechGearboxState::shared_ptr MechGearbox::GetStateInterface()
	{
		if( shared_ptr_null(iState) ) iState.bind( new IMechGearboxState( *this ) );
		return iState;
	}
	//------------------------------------------------------------
	IGearbox::shared_ptr MechGearbox::GetInterface( GEARBOX_INTERFACE_TYPE type )
	{
		switch( type )
		{
		case GIT_CONTROL: return GetControlInterface();
		case GIT_CALLBACK: return GetCallbackInterface();
		case GIT_STATE: return GetStateInterface();
		default: return IGearbox::shared_ptr();
		}
	}
	//------------------------------------------------------------
	IPedal::shared_ptr MechGearbox::GetClutchInterface( PEDAL_INTERFACE_TYPE type )
	{
		switch( type )
		{
		case PIT_PCONTROL: return GetClutchControlInterface();
		case PIT_PCALLBACK: return GetClutchCallbackInterface();
		case PIT_PSTATE: return GetClutchStateInterface();
		default: return IPedal::shared_ptr();
		}
	}
	//------------------------------------------------------------
	float MechGearbox::OperateMechanics( float t, float d )
	{
		float clval = (1-clutch.GetCurrentValue());
		torque = t * clval;
		motorDrag = d * clval;
		drag = dragLookup->GetValue( radPS );
		operateDel();
		clutch.Operate();
		shiftOpTimer.Operate();
		return radPS > 0 ? radPS : radPS = 0;
	}
	//------------------------------------------------------------
	void MechGearbox::OperateWithFullConnection()
	{
		outRadPS = childMechanics->OperateMechanics( torque * activeRatioSign, (drag + motorDrag) * activeRatioMod );
		radPS = outRadPS * activeRatioSign;
	}
	//------------------------------------------------------------
	void MechGearbox::OperateNoConnection()
	{
		radPS += (torque - motorDrag - drag) * FTI::frameTime() / flywheel->getFlywheelInertia();
	}
	//------------------------------------------------------------
	void MechGearbox::OperateWithChildNoConnection()
	{
		outRadPS = childMechanics->OperateMechanics(0,drag);
		radPS = 0;
	}
	//------------------------------------------------------------
	void MechGearbox::operateConnectionChanged()
	{
		if( shared_ptr_valid(childMechanics) )
		{
			if( activeGear == 0 ) operateDel.bind( this, &MechGearbox::OperateWithChildNoConnection );
			else operateDel.bind( this, &MechGearbox::OperateWithFullConnection );
		}
		else
		{
			operateDel.bind( this, &MechGearbox::OperateNoConnection );
		}
	}
	//------------------------------------------------------------
	bool MechGearbox::ConnectChildMech( traits::child_mech_in child, int )
	{
		if( Gearbox::ConnectChildMech( child ) )
		{
			operateConnectionChanged();		
			return true;
		}
		else return false;
	}
	//------------------------------------------------------------
	void MechGearbox::DisconnectChildMech( traits::child_mech_in child )
	{
		Gearbox::DisconnectChildMech( child );
		if( shared_ptr_null(childMechanics) )
			operateConnectionChanged();
	}
	//------------------------------------------------------------
	void MechGearbox::DisconnectChildMech( int )
	{
		Gearbox::DisconnectChildMech();
		if( shared_ptr_null(childMechanics) )
			operateConnectionChanged();
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
#include "SimpleGuiController.h"
#if DBS_LM_GAME_OBJECTS

#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include "Interface/UnitConverter.h"

namespace Demo
{
	//------------------------------------------------------------
	typedef SimpleGuiController SGC;
	//------------------------------------------------------------
	string_traits::str SGC::overlayElementName =			"DemoGUI/Stats";
	string_traits::cstr SGC::GUIElements::MOTOR =			"Motor RPM: ";
	string_traits::cstr SGC::GUIElements::TORQUE =			"Motor torque: ";
	string_traits::cstr SGC::GUIElements::GEARBOX =			"Gear index & ratio: ";
	string_traits::cstr SGC::GUIElements::THROTTLE =		"Throttle: ";
	string_traits::cstr SGC::GUIElements::STEER =			"Steer: ";
	string_traits::cstr SGC::GUIElements::BRAKE =			"Brake: ";
	string_traits::cstr SGC::GUIElements::CLUTCH =			"Clutch: ";
	string_traits::cstr SGC::GUIElements::SPTR =			" ";
	string_traits::cstr SGC::GUIElements::CHART_OP =		"<";
	string_traits::cstr SGC::GUIElements::CHART_ED =		">";
	string_traits::cstr SGC::GUIElements::CHART_ELEMENT =	"|";
	string_traits::cstr SGC::GUIElements::CHART_FILLER =	".";
	string_traits::cstr SGC::GUIElements::ENDL =			"\n";
	int SGC::GUIElements::chartSize = 20;
	//------------------------------------------------------------
	bool SimpleGuiController::initialize( const SimpleGuiControllerInitData& initdata )
	{
		if ( isInitializedController() ) return false;
		if ( initdata.vehicle == 0 ) return false;
		if ( initdata.driver == 0 )return false;

		Ogre::OverlayManager::getSingleton().getByName( "DemoGUI/DebugMainPanel" )->show();

		motorState.downcast_assign( initdata.vehicle->getMotorInterface( Vehicle::MIT_STATE ) );
		gearboxState.downcast_assign( initdata.vehicle->getGearboxInterface( Vehicle::GIT_STATE ) );
		gcState.downcast_assign( initdata.driver->GetGearboxControllerInterface( Vehicle::GIT_STATE ) );
		if( gcState == 0 ){ CloseController(); return false; } 
		throttleState.downcast_assign( initdata.vehicle->getThrottleInterface( Vehicle::PIT_PSTATE ) );
		steerState.downcast_assign( initdata.vehicle->getSteerInterface( Vehicle::PIT_SSTATE ) );
		brakeState.downcast_assign( initdata.vehicle->getBrakeInterface( Vehicle::PIT_PSTATE ) );
		clutchState.downcast_assign( initdata.vehicle->getClutchInterface( Vehicle::PIT_PSTATE ) );
		diffState = initdata.vehicle->GetMechanicsStateInterface( Vehicle::MT_DIFFERENTIAL );
		RLWheelNodeState = initdata.vehicle->GetMechanicsStateInterface( Vehicle::MT_WHEEL_NODE, 1, Vehicle::VS_LEFT );
		speedometerState = initdata.vehicle->GetSpeedometerStateInterface();

		//if ( motorState != 0 || gearboxState != 0 || throttleState != 0 || steerState != 0 || brakeState != 0 || clutchState != 0 ){ Close(); return false; }
		overlayEl = Ogre::OverlayManager::getSingleton().getOverlayElement( overlayElementName );
		if ( !overlayEl ) { CloseController(); return false; } 

		setInitializeController();
		return true;
	}
	//------------------------------------------------------------
	void SimpleGuiController::CloseController()
	{
		motorState.reset();
		gearboxState.reset();
		throttleState.reset();
		steerState.reset();
		brakeState.reset();
		clutchState.reset();
		overlayEl = 0;
		Controller::CloseController();
	}
	//------------------------------------------------------------
	void SimpleGuiController::PrepareStats()
	{
		typedef SimpleGuiController::GUIElements GE;
		typedef Ogre::StringConverter SC;
		static string_traits::str str;
		str.clear();

		//rpm
		str = GE::MOTOR + GE::SPTR;
		float rpm = motorState->getRadPS() * (float)UnitConverter::RadPStoRPM;
		GE::addValueToChart( str, rpm / (motorState->getMotorChart().maxRPM) );
		str.append( GE::SPTR + SC::toString( rpm ) + GE::ENDL );

		//torque
		str.append( GE::TORQUE + GE::SPTR + SC::toString( (int)motorState->getTorque() ) + GE::ENDL + "Drag: " + SC::toString( (int)motorState->getDrag() ) + GE::ENDL );

		str.append( "M/S: " + SC::toString( speedometerState->GetSpeedMS() ) + GE::ENDL );

		//gearbox
		rpm = gearboxState->getRadPS() * (float)UnitConverter::RadPStoRPM;
		str.append( "GearRPM: " + SC::toString( rpm ) + GE::ENDL );
		rpm = gearboxState->GetOutRadPS() * (float)UnitConverter::RadPStoRPM;
		str.append( "GearOut: " + SC::toString( rpm ) + GE::ENDL );
		str.append( "GearDrag: " + SC::toString( (int)gearboxState->getDrag() ) + GE::ENDL );
		str.append( GE::GEARBOX + GE::SPTR + SC::toString( gcState->GetTargetGear() ) + GE::CHART_OP + GE::SPTR + SC::toString( gearboxState->GetActiveGearIndex() ) + GE::SPTR + GE::CHART_ED );
		str.append( GE::SPTR + SC::toString( gearboxState->GetActiveGearRatioSign() ) + GE::ENDL );

		rpm = diffState->getRadPS();
		str.append( "DiffRPM: " + SC::toString( rpm ) + GE::ENDL );

		rpm = RLWheelNodeState->getRadPS();
		str.append( "RLNode: " + SC::toString( rpm ) + GE::ENDL );

		//throttle
		str.append( GE::THROTTLE + GE::SPTR );
		float throttle = throttleState->GetCurrentValue();
		GE::addValueToChart( str, throttle );
		str.append( GE::SPTR + SC::toString( throttle ) + GE::ENDL );

		//brake
		str.append( GE::BRAKE + GE::SPTR );
		float brake = brakeState->GetCurrentValue();
		GE::addValueToChart( str, brake );
		str.append( GE::SPTR + SC::toString( brake ) + GE::ENDL );

		//clutch
		str.append( GE::CLUTCH + GE::SPTR );
		float clutch = clutchState->GetCurrentValue();
		GE::addValueToChart( str, clutch );
		str.append( GE::SPTR + SC::toString( clutch ) + GE::ENDL );

		//steer
		str.append( GE::STEER + GE::SPTR );
		float steer = steerState->GetCurrentValue();
		GE::addValueToDoubleChart( str, steer );
		str.append( GE::SPTR + SC::toString( steer ) + GE::ENDL );

		overlayEl->setCaption(str);
	}
	//------------------------------------------------------------
	void SimpleGuiController::GUIElements::addValueToChart( string_traits::inout str, float chartElCount )
	{
		int count = int( chartElCount * chartSize );
		if( count > chartSize ) count = chartSize;
		str.append( CHART_OP );
		for( int i = 0; i < count; ++i )
		{
			str.append( CHART_ELEMENT );
		}
		for( int i = 0, n = chartSize - count; i < n; ++i )
		{
			str.append( CHART_FILLER );
		}
		str.append( CHART_ED );
	}
	//------------------------------------------------------------
	void SimpleGuiController::GUIElements::addValueToDoubleChart( string_traits::inout str, float chartElCount )
	{
		int count = int( chartElCount * chartSize );
		str.append( CHART_OP );
		int left = 0, right = 0;
		if ( count < 0 )
			left = -1*count;
		else 
			right = count;
		for( int i = 0, n = chartSize - left; i < n; ++i )
		{
			str.append( CHART_FILLER );
		}
		for( int i = 0; i < left; ++i )
		{
			str.append( CHART_ELEMENT );
		}

		str.append( CHART_ELEMENT );

		for( int i = 0; i < right; ++i )
		{
			str.append( CHART_ELEMENT );
		}
		for( int i = 0, n = chartSize - right; i < n; ++i )
		{
			str.append( CHART_FILLER );
		}
		str.append( CHART_ED );
	}
}
#endif
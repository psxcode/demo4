#pragma once
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS

#include "dsVehicleTypes.h"
#include "dsLoader.h"
#include "Vehicle/Abstract/DriverTiming.h"
#include "Vehicle/Abstract/MotorChart.h"
#include "Vehicle/Driver/drive_options.h"

#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
#	include "Vehicle/PhysX/PhysXWheelNodeParams.h"
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static Vehicle::DRIVE_OPTIONS ParseDriveOptions( string_traits::in optionName )
	{
		typedef dsKeys::VehicleDriver VDK;
		if( string_equals<CaseSensitive>()(optionName, VDK::DO_THROTTLE_ASSIST) )return Vehicle::DO_THROTTLE_ASSIST;
		if( string_equals<CaseSensitive>()(optionName, VDK::DO_BRAKE_ASSIST) )return Vehicle::DO_BRAKE_ASSIST;
		if( string_equals<CaseSensitive>()(optionName, VDK::DO_CLUTCH_ASSIST) )return Vehicle::DO_CLUTCH_ASSIST;
		if( string_equals<CaseSensitive>()(optionName, VDK::DO_NEUTRAL_PREVENT) )return Vehicle::DO_NEUTRAL_PREVENT;
		if( string_equals<CaseSensitive>()(optionName, VDK::DO_REVERSE_PREVENT) )return Vehicle::DO_REVERSE_PREVENT;
		if( string_equals<CaseSensitive>()(optionName, VDK::DO_SHIFT_ASSIST) )return Vehicle::DO_SHIFT_ASSIST;
		if( string_equals<CaseSensitive>()(optionName, VDK::DO_STEER_ASSIST) )return Vehicle::DO_STEER_ASSIST;
		if( string_equals<CaseSensitive>()(optionName, VDK::DO_USER_CLUTCH) )return Vehicle::DO_USER_CLUTCH;
		if( string_equals<CaseSensitive>()(optionName, VDK::DO_USER_CONTROLS) )return Vehicle::DO_USER_CONTROLS;
		return Vehicle::DO_NO_OPTIONS;
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadGearboxModes( const TiXmlElement *element, dsGearbox::str_container_inout modes )
	{
		typedef dsKeys::VehicleCommon VC;

		const TiXmlElement *childElement;
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, VC::MODE) )
			{
				modes.push_back( dsGetString<CaseSensitive>(childElement, VC::NAME) );
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadGearboxRatios( const TiXmlElement *element, dsGearbox::real_container_inout ratios )
	{
		typedef dsKeys::VehicleCommon VC;

		const TiXmlElement *childElement;
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, VC::RATIO) )
			{
				ratios.push_back( dsGetReal<CaseSensitive>(childElement, VC::VALUE) );
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadDifferentialModes( const TiXmlElement *element, dsDifferential::str_container_inout modes )
	{
		typedef dsKeys::VehicleCommon VC;

		const TiXmlElement* childElement(0);
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, VC::MODE) )
			{
				modes.push_back( dsGetString<CaseSensitive>(childElement, VC::NAME) );
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadDifferentialChainNode( const TiXmlElement *element, dsDifferentialChain::Node &dcn )
	{
		typedef dsKeys::VehicleCommon VC;

		dcn.name = dsGetString<CaseSensitive>(element, VC::NAME);
		dcn.index = dsGetUInt<CaseSensitive>(element, VC::INDEX);

		const TiXmlElement *childElement(0);
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, VC::NODE) )
			{
				if( dcn.leftNode == 0 )
				{
					dcn.leftNode.bind( new dsDifferentialChain::Node );
					LoadDifferentialChainNode<CaseSensitive>(childElement, *dcn.leftNode);
				}
				else if( dcn.rightNode == 0 )
				{
					dcn.rightNode.bind( new dsDifferentialChain::Node );
					LoadDifferentialChainNode<CaseSensitive>(childElement, *dcn.rightNode);
				}
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadVBVEntries( const TiXmlElement *element, dsVBV::real_container_inout entries )
	{
		typedef dsKeys::VehicleCommon VC;

		const TiXmlElement *childElement(0);
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, VC::ENTRY) )
			{
				entries.push_back( dsGetReal<CaseSensitive>(childElement, VC::VALUE) );
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadMotor( const TiXmlElement *element, dsMotor &mot )
	{
		typedef dsKeys::Motor MK;
		typedef dsKeys::VehicleCommon VC;

		mot.motorName = dsGetString<CaseSensitive>( element, VC::NAME );
		mot.maxTorque = dsGetReal<CaseSensitive>( element, MK::MAX_TORQUE );
		mot.maxStarterTorque = dsGetReal<CaseSensitive>( element, MK::MAX_STARTER_TORQUE );
		mot.maxDrag = dsGetReal<CaseSensitive>( element, VC::MAX_DRAG );
		mot.torqueLookupName = dsGetString<CaseSensitive>( element, MK::TORQUE_LOOKUP );
		mot.dragLookupName = dsGetString<CaseSensitive>( element, VC::DRAG_LOOKUP );
		mot.flywheelName = dsGetString<CaseSensitive>( element, VC::FLYWHEEL_PARAMS );
		mot.startRPM = dsGetReal<CaseSensitive>( element, MK::STALL_RPM );
		mot.idleRPM = dsGetReal<CaseSensitive>( element, MK::IDLE_RPM );
		mot.startRPM = dsGetReal<CaseSensitive>( element, MK::START_RPM );
		mot.redRPM = dsGetReal<CaseSensitive>( element, MK::RED_RPM );
		mot.maxRPM = dsGetReal<CaseSensitive>( element, MK::MAX_RPM );
		mot.calmControlDataName = dsGetString<CaseSensitive>( element, MK::CALM_RPM_CONTROL_DATA );
		mot.agrControlDataName = dsGetString<CaseSensitive>( element, MK::AGR_RPM_CONTROL_DATA );
		mot.belowIdleTorqueMult = dsGetReal<CaseSensitive>( element, MK::TORQUE_MULT );
		mot.belowIdleTorqueMultFunc = dsGetString<CaseSensitive>( element, MK::TORQUE_MULT_FUNC );
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadGearbox( const TiXmlElement *element, dsGearbox &gb )
	{
		typedef dsKeys::Gearbox GK;
		typedef dsKeys::VehicleCommon VC;

		gb.gearboxName = dsGetString<CaseSensitive>( element, VC::NAME );
		gb.dragLookupName = dsGetString<CaseSensitive>( element, VC::DRAG_LOOKUP );
		gb.flywheelName = dsGetString<CaseSensitive>( element, VC::FLYWHEEL_PARAMS );
		gb.maxDrag = dsGetReal<CaseSensitive>( element, VC::MAX_DRAG );
		gb.typeName = dsGetString<CaseSensitive>( element, VC::TYPE );
		gb.mechShiftTime = dsGetReal<CaseSensitive>( element, GK::MECH_SHIFT_TIME );

		const TiXmlElement *childElement(0);
		while( dsIterateChildElements( element, childElement ) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, VC::MODES) )
			{
				LoadGearboxModes<CaseSensitive>( childElement, gb.modeNameContainer );
			}
			else if( string_equals<CaseSensitive>()(childName, GK::DIRECT_RATIOS) )
			{
				LoadGearboxRatios<CaseSensitive>( childElement, gb.directRatios );
			}
			else if( string_equals<CaseSensitive>()(childName, GK::REVERSE_RATIOS) )
			{
				LoadGearboxRatios<CaseSensitive>( childElement, gb.reverseRatios );
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadFlywheel( const TiXmlElement *element, dsFlywheel &fw )
	{
		typedef dsKeys::VehicleCommon VC;

		fw.name = dsGetString<CaseSensitive>(element, VC::NAME);
		fw.dragLookupName = dsGetString<CaseSensitive>(element, VC::DRAG_LOOKUP);
		fw.maxDrag = dsGetReal<CaseSensitive>(element, VC::MAX_DRAG);
		fw.mass = dsGetReal<CaseSensitive>(element, VC::MASS);
		fw.radius = dsGetReal<CaseSensitive>(element, VC::RADIUS);
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadDifferential( const TiXmlElement *element, dsDifferential &diff )
	{
		typedef dsKeys::VehicleCommon VC;
		typedef dsKeys::Differential DK;

		diff.diffName = dsGetString<CaseSensitive>(element, VC::NAME);
		diff.diffRatio = dsGetReal<CaseSensitive>(element, DK::DIFF_RATIO);
		diff.dragLookupName = dsGetString<CaseSensitive>(element, VC::DRAG_LOOKUP);
		diff.maxDrag = dsGetReal<CaseSensitive>(element, VC::MAX_DRAG);
		diff.firstNodeRatio = dsGetReal<CaseSensitive>(element, DK::FIRST_NODE_RATIO);

		const TiXmlElement *childElement(0);
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, VC::MODES) )
			{
				LoadDifferentialModes<CaseSensitive>( childElement, diff.modeNameContainer );
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadDifferentialChain( const TiXmlElement *element, dsDifferentialChain &dc )
	{
		typedef dsKeys::VehicleCommon VC;

		dc.name = dsGetString<CaseSensitive>(element, VC::NAME);

		const TiXmlElement *childElement(0);
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, VC::NODE) )
			{
				if( dc.firstNode == 0 )
				{
					dc.firstNode.bind( new dsDifferentialChain::Node );
					LoadDifferentialChainNode<CaseSensitive>(childElement, *dc.firstNode);
				}
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadVBV( const TiXmlElement *element, dsVBV &vbv )
	{
		typedef dsKeys::VehicleCommon VCK;
		typedef dsKeys::VBV VK;

		vbv.name = dsGetString<CaseSensitive>(element, VCK::NAME);
		vbv.incValue = dsGetReal<CaseSensitive>(element, VK::INC_VALUE);
		vbv.startValue = dsGetReal<CaseSensitive>(element, VK::START_VALUE);
		vbv.multValue = dsGetReal<CaseSensitive>(element, VK::MULT_VALUE);

		const TiXmlElement *childElement(0);
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, VCK::ENTRIES) )
			{
				LoadVBVEntries<CaseSensitive>(childElement, vbv.entries);
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadBrakeBlock( const TiXmlElement *element, dsBrakeBlock &bb )
	{
		typedef dsKeys::VehicleCommon VC;
		typedef dsKeys::BrakeBlock BB;

		bb.name = dsGetString<CaseSensitive>(element, VC::NAME);
		bb.osnName = dsGetString<CaseSensitive>(element, VC::OSN_NAME);
		bb.maxBrakeTorque = dsGetReal<CaseSensitive>(element, BB::MAX_BRAKE_TORQUE);
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadThrottleControl( const TiXmlElement *element, dsThrottleControl &tc )
	{
		typedef dsKeys::VehicleCommon VC;
		typedef dsKeys::TCS TK;

		tc.name = dsGetString<CaseSensitive>(element, VC::NAME);
		tc.throttleFeed = dsGetReal<CaseSensitive>(element, TK::FEED_VALUE);
		tc.idleMark = dsGetReal<CaseSensitive>(element, TK::IDLE_MARK);
		tc.idleRange = dsGetReal<CaseSensitive>(element, TK::IDLE_RANGE);
		tc.tcsTypeName = dsGetString<CaseSensitive>(element, VC::TYPE);
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadDriverTiming( const TiXmlElement *element, Vehicle::DriverTiming &dt )
	{
		typedef dsKeys::VehicleCommon VC;
		typedef dsKeys::DriverTiming DT;

		dt.name = dsGetString<CaseSensitive>(element, VC::NAME);
		dt.throttlePressTime = dsGetReal<CaseSensitive>(element, DT::THROTTLE_PRESS);
		dt.throttleReleaseTime = dsGetReal<CaseSensitive>(element, DT::THROTTLE_RELEASE);
		dt.brakePressTime = dsGetReal<CaseSensitive>(element, DT::BRAKE_PRESS);
		dt.brakeReleaseTime = dsGetReal<CaseSensitive>(element, DT::BRAKE_RELEASE);
		dt.clutchPressTime = dsGetReal<CaseSensitive>(element, DT::CLUTCH_PRESS);
		dt.clutchReleaseTime = dsGetReal<CaseSensitive>(element, DT::CLUTCH_RELEASE);
		dt.handbrakePressTime = dsGetReal<CaseSensitive>(element, DT::HANDBRAKE_PRESS);
		dt.handbrakeReleaseTime = dsGetReal<CaseSensitive>(element, DT::HANDBRAKE_RELEASE);
		dt.steerPressTime = dsGetReal<CaseSensitive>(element, DT::STEER_PRESS);
		dt.steerReleaseTime = dsGetReal<CaseSensitive>(element, DT::STEER_RELEASE);
		dt.shiftDelayTime = dsGetReal<CaseSensitive>(element, DT::SHIFT_DELAY);
		dt.shiftToDirectTime = dsGetReal<CaseSensitive>(element, DT::SHIFT_TO_DIRECT);
		dt.shiftToReverseTime = dsGetReal<CaseSensitive>(element, DT::SHIFT_TO_REVERSE);
		dt.shiftToNeutralTime = dsGetReal<CaseSensitive>(element, DT::SHIFT_TO_NEUTRAL);
		dt.switchModeTime = dsGetReal<CaseSensitive>(element, DT::SWITCH_MODE);
		dt.switchSeqTime = dsGetReal<CaseSensitive>(element, DT::SWITCH_SEQ);
		dt.rpmSpeedReactionTime = dsGetReal<CaseSensitive>(element, DT::RPM_SPEED_REACTION);
		dt.rpmZoneReactionTime = dsGetReal<CaseSensitive>(element, DT::RPM_ZONE_REACTION);
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadDriveOptions( const TiXmlElement *element, int &ops )
	{
		typedef dsKeys::VehicleCommon VC;

		const TiXmlElement *childElement(0);
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, VC::OPTION) )
			{
				string_traits::str optionName = dsGetString<CaseSensitive>(childElement, VC::NAME);
				if( !optionName.empty() && dsGetBool<CaseSensitive>(childElement, VC::VALUE) )
				{
					ops |= ParseDriveOptions<CaseSensitive>(optionName);
				}
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadVehicleDriver( const TiXmlElement *element, dsVehicleDriver &vd )
	{
		typedef dsKeys::VehicleCommon VC;
		typedef dsKeys::VehicleDriver VD;

		vd.agrMCMPName = dsGetString<CaseSensitive>(element, VD::AGR_MCMP);
		vd.agrTimingName = dsGetString<CaseSensitive>(element, VD::AGR_TIMING);
		vd.calmMCMPName = dsGetString<CaseSensitive>(element, VD::CALM_MCMP);
		vd.calmTimingName = dsGetString<CaseSensitive>(element, VD::CALM_TIMING);

		const TiXmlElement *childElement(0);
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, VD::DRIVE_OPTIONS) )
			{
				LoadDriveOptions<CaseSensitive>(childElement, vd.driveOptions);
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadMCM( const TiXmlElement *element, Vehicle::MotorChartMarks &mcm )
	{
		typedef dsKeys::MotorChartMarksPack MK;

		mcm.gearIndex = dsGetInt<CaseSensitive>(element, MK::GEAR);
		mcm.min = dsGetReal<CaseSensitive>(element, MK::MIN);
		mcm.normLow = dsGetReal<CaseSensitive>(element, MK::NORM_LOW);
		mcm.normHigh = dsGetReal<CaseSensitive>(element, MK::NORM_HIGH);
		mcm.max = dsGetReal<CaseSensitive>(element, MK::MAX);
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadMCMP( const TiXmlElement *element, Vehicle::MotorChartMarksPack &mcmp )
	{
		typedef dsKeys::VehicleCommon VC;
		typedef dsKeys::MotorChartMarksPack MK;

		mcmp.name = dsGetString<CaseSensitive>(element, VC::NAME);

		const TiXmlElement *childElement(0);
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, MK::MARKS) )
			{
				Vehicle::MotorChartMarks::shared_ptr marks( new Vehicle::MotorChartMarks );
				LoadMCM<CaseSensitive>( childElement, *marks );
				mcmp.AddChartMarks( marks );
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadSpeedometer( const TiXmlElement *element, dsSpeedometer &spdm )
	{
		typedef dsKeys::VehicleBody VK;

		spdm.revSensorAxis = dsGetUInt<CaseSensitive>(element, VK::SPDM_REV_AXIS);
		spdm.revSensorSide = dsGetUInt<CaseSensitive>(element, VK::SPDM_REV_SIDE);
		spdm.wheelRadius = dsGetReal<CaseSensitive>(element, VK::SPDM_WHEEL_RADIUS);
	}
	//------------------------------------------------------------
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadPhysXSuspension( const TiXmlElement *element, Vehicle::PhysXSuspensionDesc &sus )
	{
		typedef dsKeys::VehicleCommon VC;
		typedef dsKeys::Suspension SK;

		sus.name = dsGetString<CaseSensitive>(element, VC::NAME);
		sus.suspensionTravel = dsGetReal<CaseSensitive>(element, SK::TRAVEL);
		sus.suspension.damper = dsGetReal<CaseSensitive>(element, SK::DAMPER);
		sus.suspension.spring = dsGetReal<CaseSensitive>(element, SK::SPRING);
		sus.suspension.targetValue = dsGetReal<CaseSensitive>(element, SK::TARGET_VALUE);
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadPhysXTireFunction( const TiXmlElement *element, NxTireFunctionDesc &func )
	{
		typedef dsKeys::Tire TK;
		func.stiffnessFactor = dsGetReal<CaseSensitive>( element, TK::STIFFNESS_FACTOR );
		func.asymptoteSlip = dsGetReal<CaseSensitive>( element, TK::ASYMPTOTE_SLIP );
		func.asymptoteValue = dsGetReal<CaseSensitive>( element, TK::ASYMPTOTE_VALUE );
		func.extremumSlip = dsGetReal<CaseSensitive>( element, TK::EXTREMUM_SLIP );
		func.extremumValue = dsGetReal<CaseSensitive>( element, TK::EXTREMUM_VALUE );
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadPhysXTire( const TiXmlElement *element, Vehicle::PhysXTireDesc &tire )
	{
		typedef dsKeys::VehicleCommon VC;
		typedef dsKeys::Tire TK;

		tire.name = dsGetString<CaseSensitive>(element, VC::NAME);
		const TiXmlElement* childElement(0);
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, TK::LONG_FUNC) )
			{
				LoadPhysXTireFunction<CaseSensitive>(childElement, tire.longFunction);
			}
			else if( string_equals<CaseSensitive>()(childName, TK::LAT_FUNC) )
			{
				LoadPhysXTireFunction<CaseSensitive>(childElement, tire.latFunction);
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadPhysXWheel( const TiXmlElement *element, dsPhysXWheel &wheel )
	{
		typedef dsKeys::VehicleCommon VC;
		typedef dsKeys::Wheel WK;

		wheel.name = dsGetString<CaseSensitive>(element, VC::NAME);
		wheel.osnName = dsGetString<CaseSensitive>(element, VC::OSN_NAME);
		wheel.tireName = dsGetString<CaseSensitive>(element, WK::TIRE_NAME);
		wheel.mass = dsGetReal<CaseSensitive>(element, VC::MASS);
		wheel.rimRadius = dsGetReal<CaseSensitive>(element, WK::RIM_RADIUS);
		wheel.tireIncRadius = dsGetReal<CaseSensitive>(element, WK::TIRE_INC_RADIUS);
		wheel.tireWidth = dsGetReal<CaseSensitive>(element, WK::TIRE_WIDTH);
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadPhysXWheelNode( const TiXmlElement *element, PhysXWheelNode &wheelNode )
	{
		typedef dsKeys::VehicleCommon VC;
		typedef dsKeys::WheelNode WK;

		wheelNode.name = dsGetString<CaseSensitive>(element, VC::NAME);
		wheelNode.osnName = dsGetString<CaseSensitive>(element, VC::OSN_NAME);
		wheelNode.susName = dsGetString<CaseSensitive>(element, WK::SUS_NAME);
		wheelNode.brakeBlockName = dsGetString<CaseSensitive>(element, WK::BRAKE_BLOCK_NAME);
		wheelNode.rubberWheelName = dsGetString<CaseSensitive>(element, WK::RUBBER_WHEEL_NAME);
		wheelNode.ironWheelName = dsGetString<CaseSensitive>(element, WK::IRON_WHEEL_NAME);
		wheelNode.dragLookupName = dsGetString<CaseSensitive>(element, VC::DRAG_LOOKUP);
		wheelNode.maxDrag = dsGetReal<CaseSensitive>(element, VC::MAX_DRAG);
		wheelNode.ironIncShift = dsGetReal<CaseSensitive>(element, WK::IRON_INC_SHIFT);
		wheelNode.rubberIncShift = dsGetReal<CaseSensitive>(element, WK::RUBBER_WHEEL_NAME);
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadPhysXVehicleBodyAxle( const TiXmlElement *element, dsPhysXVehicleBody::Axle &axle )
	{
		typedef dsKeys::VehicleCommon VC;
		typedef dsKeys::VehicleBody VK;

		axle.index = dsGetUInt<CaseSensitive>(element, VC::INDEX);
		axle.maxSteerAngle = dsGetReal<CaseSensitive>(element, VK::MAX_STEER_ANGLE);
		axle.brakeMaxPressValue = dsGetReal<CaseSensitive>(element, VK::BRAKE_MAX_PRESS_VALUE);
		axle.leftWheelNode = dsGetString<CaseSensitive>(element, VK::LEFT_WHEEL_NODE);
		axle.rightWheelNode = dsGetString<CaseSensitive>(element, VK::RIGHT_WHEEL_NODE);

		const TiXmlElement *childElement(0);
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, VC::POSITION) )
			{
				dsGetVector3<CaseSensitive>(childElement, axle.position, Ogre::Vector3::ZERO);
			}
		}
	}
	//------------------------------------------------------------
	template<bool CaseSensitive>
	static void LoadPhysXVehicleBody( const TiXmlElement *element, dsPhysXVehicleBody &body )
	{
		typedef dsKeys::VehicleCommon VC;
		typedef dsKeys::VehicleBody VK;

		body.setName = dsGetString<CaseSensitive>(element, VC::NAME);
		body.bodyName = dsGetString<CaseSensitive>(element, VK::BODY_NAME);
		body.osnName = dsGetString<CaseSensitive>(element, VC::OSN_NAME);
		body.boundMeshName = dsGetString<CaseSensitive>(element, VK::BOUND_MESH_NAME);
		body.mass = dsGetReal<CaseSensitive>(element, VC::MASS);
		body.motorName = dsGetString<CaseSensitive>(element, VK::MOTOR_NAME);
		body.gearboxName = dsGetString<CaseSensitive>(element, VK::GEARBOX_NAME);
		body.diffName = dsGetString<CaseSensitive>(element, VK::DIFF_NAME);
		body.tcsName = dsGetString<CaseSensitive>(element, VK::TCS_NAME);
		body.physxMatName = dsGetString<CaseSensitive>(element, VK::PHYSX_MAT_NAME);
		body.solverCount = dsGetUInt<CaseSensitive>(element, VK::SOLVER_COUNT);
		body.addDownForce = dsGetReal<CaseSensitive>(element, VK::ADD_DOWN_FORCE);

		const TiXmlElement *childElement(0);
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<CaseSensitive>()(childName, VC::MASS_OFFSET) )
			{
				dsGetVector3<CaseSensitive>(childElement, body.massOffset, Ogre::Vector3::ZERO );
			}
			else if( string_equals<CaseSensitive>()(childName, VK::BOUND_BOX) )
			{
				dsGetVector3<CaseSensitive>(childElement, body.boundBox, Ogre::Vector3::ZERO );
			}
			else if( string_equals<CaseSensitive>()(childName, VK::SPEEDOMETER) )
			{
				LoadSpeedometer<CaseSensitive>(childElement, body.speedometer);
			}
			else if( string_equals<CaseSensitive>()(childName, VC::AXLE) )
			{
				dsPhysXVehicleBody::Axle axle;
				LoadPhysXVehicleBodyAxle<CaseSensitive>(childElement, axle);
				body.addAxle( axle );
			}
		}
	}
	//------------------------------------------------------------
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS

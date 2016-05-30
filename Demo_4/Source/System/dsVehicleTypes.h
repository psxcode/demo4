#pragma once
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS

#include <OgreVector3.h>

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	class dsDifferentialChain
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsDifferentialChain)
		//--------------------------------//
		class Node
		{
		public:
			//--------------------------------//
			typedef shared_ptr_traits< Node >::type shared_ptr;
			typedef shared_ptr child_node_type;
			//--------------------------------//
			Node();
			Node( const Node &n );
			~Node();
			Node& operator=( const Node &n );

			bool isValid()const;

			int index;

			child_node_type leftNode;
			child_node_type rightNode;
			string_traits::str name;
		};
		//--------------------------------//
		dsDifferentialChain();
		dsDifferentialChain( const dsDifferentialChain &dc );
		dsDifferentialChain& operator=( const dsDifferentialChain &dc );

		Node::child_node_type firstNode;
		string_traits::str name;
	};
	//------------------------------------------------------------
	class dsDifferential
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsDifferential)
		//--------------------------------//
		typedef Demo::vector< string_traits::str >::type str_container;
		typedef type_traits< str_container >::ref str_container_inout;
		//--------------------------------//
		dsDifferential()
			: diffRatio( 1.0f )
			, firstNodeRatio( 0.5f )
			, maxDrag( 0.0f )
		{
		}

		Ogre::Real diffRatio;
		Ogre::Real firstNodeRatio;
		Ogre::Real maxDrag;
		string_traits::str diffName;
		string_traits::str dragLookupName;
		str_container modeNameContainer;
	};
	//------------------------------------------------------------
	class dsMotor
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsMotor)
		//--------------------------------//
		dsMotor()
			: stallRPM( 0.0f )
			, idleRPM( 0.0f )
			, startRPM( 0.0f )
			, redRPM( 0.0f )
			, maxRPM( 0.0f )
			, maxTorque( 0.0f )
			, maxDrag( 0.0f )
			, maxStarterTorque( 0.0f )
			, belowIdleTorqueMult( 0.0f )
		{
		}

		Ogre::Real stallRPM;
		Ogre::Real idleRPM;
		Ogre::Real startRPM;
		Ogre::Real redRPM;
		Ogre::Real maxRPM;
		Ogre::Real maxTorque;
		Ogre::Real maxStarterTorque;
		Ogre::Real maxDrag;
		Ogre::Real belowIdleTorqueMult;
		string_traits::str motorName;
		string_traits::str torqueLookupName;
		string_traits::str dragLookupName;
		string_traits::str flywheelName;
		string_traits::str calmControlDataName;
		string_traits::str agrControlDataName;
		string_traits::str belowIdleTorqueMultFunc;
	};
	//------------------------------------------------------------
	class dsGearbox
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsGearbox)
		//--------------------------------//
		typedef Demo::vector< string_traits::str >::type str_container;
		typedef type_traits< str_container >::ref str_container_inout;
		typedef Demo::vector< Ogre::Real >::type real_container;
		typedef type_traits< real_container >::ref real_container_inout;
		//--------------------------------//
		dsGearbox()
			: maxDrag( 0.0f )
			, mechShiftTime( 0.0f )
		{
		}

		Ogre::Real maxDrag;
		Ogre::Real mechShiftTime;
		string_traits::str gearboxName;
		string_traits::str typeName;
		string_traits::str dragLookupName;
		string_traits::str flywheelName;
		str_container modeNameContainer;
		real_container directRatios;
		real_container reverseRatios;
	};
	//------------------------------------------------------------
	class dsFlywheel
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsFlywheel)
		//--------------------------------//
		dsFlywheel()
			: mass( 0.0f )
			, radius( 0.0f )
			, maxDrag( 0.0f )
		{
		}

		Ogre::Real mass;
		Ogre::Real radius;
		Ogre::Real maxDrag;
		string_traits::str dragLookupName;
		string_traits::str name;
	};
	//------------------------------------------------------------
	class dsSpeedometer
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsSpeedometer)
		//--------------------------------//
		dsSpeedometer()
			: revSensorAxis( 0 )
			, revSensorSide( 0 )
			, wheelRadius( 0.0f )
		{
		}

		int revSensorAxis;
		int revSensorSide;
		Ogre::Real wheelRadius;
	};
	//------------------------------------------------------------
	class dsBrakeBlock
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsBrakeBlock)
		//--------------------------------//
		dsBrakeBlock()
			: maxBrakeTorque( 0.0f )
		{
		}

		Ogre::Real maxBrakeTorque;
		string_traits::str name;
		string_traits::str osnName;
	};
	//------------------------------------------------------------
	class dsVBV
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsVBV)
		//--------------------------------//
		typedef Demo::vector< Ogre::Real >::type real_container;
		typedef type_traits< real_container >::ref real_container_inout;
		//--------------------------------//
		dsVBV()
			: startValue( 0.0f )
			, incValue( 0.0f )
			, multValue( 0.0f )
		{
		}

		Ogre::Real startValue;
		Ogre::Real incValue;
		Ogre::Real multValue;
		string_traits::str name;
		real_container entries;
	};
	//------------------------------------------------------------
	class dsThrottleControl
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsThrottleControl)
		//--------------------------------//
		dsThrottleControl()
			: throttleFeed( 0.0f )
			, idleMark( 0.0f )
			, idleRange( 0.0f )
		{
		}

		//Значение краника для систем с постоянной подачей топлива
		Ogre::Real throttleFeed;

		//Отметка idle оборотов
		Ogre::Real idleMark;
		//Ширина отметки
		Ogre::Real idleRange;

		string_traits::str name;
		string_traits::str tcsTypeName;
	};
	//------------------------------------------------------------
	class dsVehicleDriver
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsVehicleDriver)
		//--------------------------------//
		dsVehicleDriver()
			: driveOptions( 0 )
		{
		}

		int driveOptions;
		string_traits::str name;
		string_traits::str calmTimingName;
		string_traits::str agrTimingName;
		string_traits::str calmMCMPName;
		string_traits::str agrMCMPName;
	};
	//------------------------------------------------------------
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
	class dsPhysXVehicleBody
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsPhysXVehicleBody)
		//--------------------------------//
		class Axle
		{
		public:
			Axle()
				: position( Ogre::Vector3::ZERO )
				, maxSteerAngle( 0.0f )
				, index( -1 )
				, brakeMaxPressValue( 1.0f )
			{
			}

			int index;
			Ogre::Real maxSteerAngle;
			Ogre::Real brakeMaxPressValue;
			Ogre::Vector3 position;
			string_traits::str leftWheelNode;
			string_traits::str rightWheelNode;
		};
		//--------------------------------//
		typedef Demo::vector< Axle >::type axle_container;
		//--------------------------------//
		dsPhysXVehicleBody()
			: mass( 0.0f )
			, addDownForce( 0.0f )
			, boundBox( Ogre::Vector3::ZERO )
			, massOffset( Ogre::Vector3::ZERO )
			, solverCount( 0 )
		{
		}
		inline void addAxle( axle_container::value_type &axle ) { axleContainer.push_back( axle ); }

		string_traits::str setName;
		string_traits::str bodyName;
		string_traits::str osnName;
		string_traits::str boundMeshName;
		string_traits::str motorName;
		string_traits::str gearboxName;
		string_traits::str diffName;
		string_traits::str tcsName;
		string_traits::str physxMatName;
		int solverCount;
		Ogre::Real mass;
		Ogre::Real addDownForce;
		Ogre::Vector3 boundBox;
		Ogre::Vector3 massOffset;
		dsSpeedometer speedometer;
		axle_container axleContainer;
	};
	//------------------------------------------------------------
	class dsPhysXWheel
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsPhysXWheel)
		//--------------------------------//
		dsPhysXWheel()
			: tireWidth( 0.0f )
			, rimRadius( 0.0f )
			, tireIncRadius( 0.0f )
			, mass( 0.0f )
		{
		}

		string_traits::str name;
		string_traits::str osnName;
		string_traits::str tireName;
		Ogre::Real tireWidth;
		Ogre::Real rimRadius;
		Ogre::Real tireIncRadius;
		Ogre::Real mass;
	};
	//------------------------------------------------------------
	class PhysXWheelNode
	{
	public:
		DEFINE_CLASS_SHARED_PTR(PhysXWheelNode)
		//--------------------------------//
		PhysXWheelNode()
			: maxDrag( 0.0f )
			, ironIncShift( 0.0f )
			, rubberIncShift( 0.0f )
		{
		}

		Ogre::Real ironIncShift;
		Ogre::Real rubberIncShift;
		Ogre::Real maxDrag;
		string_traits::str name;
		string_traits::str dragLookupName;
		string_traits::str osnName;
		string_traits::str susName;
		string_traits::str rubberWheelName;
		string_traits::str ironWheelName;
		string_traits::str brakeBlockName;
	};
	//------------------------------------------------------------
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
namespace dsKeys
{
	//------------------------------------------------------------
	struct Motor
	{
		static string_traits::cstr STALL_RPM;
		static string_traits::cstr IDLE_RPM;
		static string_traits::cstr START_RPM;
		static string_traits::cstr RED_RPM;
		static string_traits::cstr MAX_RPM;
		static string_traits::cstr MAX_TORQUE;
		static string_traits::cstr MAX_STARTER_TORQUE;
		static string_traits::cstr TORQUE_LOOKUP;
		static string_traits::cstr CALM_RPM_CONTROL_DATA;
		static string_traits::cstr AGR_RPM_CONTROL_DATA;
		static string_traits::cstr TORQUE_MULT_FUNC;
		static string_traits::cstr TORQUE_MULT;
	};
	//------------------------------------------------------------
	struct Gearbox
	{
		static string_traits::cstr GT_MECHANICAL;
		static string_traits::cstr GT_SEQUENTAL;
		static string_traits::cstr GT_AUTOMATIC;
		static string_traits::cstr AGM_DIRECT;
		static string_traits::cstr AGM_NEUTRAL;
		static string_traits::cstr AGM_REVERSE;
		static string_traits::cstr AGM_PARK;
		static string_traits::cstr AGM_SEQ_EMU;
		static string_traits::cstr AGM_SPORT;
		static string_traits::cstr DIRECT_RATIOS;
		static string_traits::cstr REVERSE_RATIOS;
		static string_traits::cstr MAX_BRAKE_TORQUE;
		static string_traits::cstr MECH_SHIFT_TIME;
	};
	//------------------------------------------------------------
	struct Differential
	{
		static string_traits::cstr DM_LOCK;
		static string_traits::cstr DM_DIS_FIRST;
		static string_traits::cstr DM_DIS_SECOND;
		static string_traits::cstr DIFF_RATIO;
		static string_traits::cstr FIRST_NODE_RATIO;
	};
	//------------------------------------------------------------
	struct BrakeBlock
	{
		static string_traits::cstr MAX_BRAKE_TORQUE;
	};
	//------------------------------------------------------------
	struct VBV
	{
		static string_traits::cstr START_VALUE;
		static string_traits::cstr INC_VALUE;
		static string_traits::cstr MULT_VALUE;
	};
	//------------------------------------------------------------
	struct DriverTiming
	{
		static string_traits::cstr THROTTLE_PRESS;
		static string_traits::cstr THROTTLE_RELEASE;
		static string_traits::cstr BRAKE_PRESS;
		static string_traits::cstr BRAKE_RELEASE;
		static string_traits::cstr CLUTCH_PRESS;
		static string_traits::cstr CLUTCH_RELEASE;
		static string_traits::cstr HANDBRAKE_PRESS;
		static string_traits::cstr HANDBRAKE_RELEASE;
		static string_traits::cstr STEER_PRESS;
		static string_traits::cstr STEER_RELEASE;
		static string_traits::cstr SHIFT_TO_NEUTRAL;
		static string_traits::cstr SHIFT_TO_DIRECT;
		static string_traits::cstr SHIFT_TO_REVERSE;
		static string_traits::cstr SHIFT_DELAY;
		static string_traits::cstr SWITCH_MODE;
		static string_traits::cstr SWITCH_SEQ;
		static string_traits::cstr RPM_ZONE_REACTION;
		static string_traits::cstr RPM_SPEED_REACTION;
	};
	//------------------------------------------------------------
	struct MotorChartMarksPack
	{
		static string_traits::cstr MARKS;
		static string_traits::cstr GEAR;
		static string_traits::cstr MIN;
		static string_traits::cstr NORM_LOW;
		static string_traits::cstr NORM_HIGH;
		static string_traits::cstr MAX;
	};
	//------------------------------------------------------------
	struct TCS
	{
		static string_traits::cstr FEED_VALUE;
		static string_traits::cstr IDLE_MARK;
		static string_traits::cstr IDLE_RANGE;
		static string_traits::cstr TYPE_CONST_FEED;
	};
	//------------------------------------------------------------
	struct VehicleDriver
	{
		static string_traits::cstr CALM_TIMING;
		static string_traits::cstr AGR_TIMING;
		static string_traits::cstr CALM_MCMP;
		static string_traits::cstr AGR_MCMP;
		static string_traits::cstr DRIVE_OPTIONS;
		static string_traits::cstr DO_THROTTLE_ASSIST;
		static string_traits::cstr DO_BRAKE_ASSIST;
		static string_traits::cstr DO_CLUTCH_ASSIST;
		static string_traits::cstr DO_STEER_ASSIST;
		static string_traits::cstr DO_SHIFT_ASSIST;
		static string_traits::cstr DO_REVERSE_PREVENT;
		static string_traits::cstr DO_NEUTRAL_PREVENT;
		static string_traits::cstr DO_USER_CONTROLS;
		static string_traits::cstr DO_USER_CLUTCH;
	};
	//------------------------------------------------------------
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
	struct Suspension
	{
		static string_traits::cstr TRAVEL;
		static string_traits::cstr SPRING;
		static string_traits::cstr DAMPER;
		static string_traits::cstr TARGET_VALUE;
	};
	//------------------------------------------------------------
	struct Tire
	{
		static string_traits::cstr LONG_FUNC;
		static string_traits::cstr LAT_FUNC;
		static string_traits::cstr EXTREMUM_SLIP;
		static string_traits::cstr EXTREMUM_VALUE;
		static string_traits::cstr ASYMPTOTE_SLIP;
		static string_traits::cstr ASYMPTOTE_VALUE;
		static string_traits::cstr STIFFNESS_FACTOR;
	};
	//------------------------------------------------------------
	struct Wheel
	{
		static string_traits::cstr RIM_RADIUS;
		static string_traits::cstr TIRE_INC_RADIUS;
		static string_traits::cstr TIRE_WIDTH;
		static string_traits::cstr TIRE_NAME;
	};
	//------------------------------------------------------------
	struct WheelNode
	{
		static string_traits::cstr SUS_NAME;
		static string_traits::cstr RUBBER_WHEEL_NAME;
		static string_traits::cstr IRON_WHEEL_NAME;
		static string_traits::cstr BRAKE_BLOCK_NAME;
		static string_traits::cstr IRON_INC_SHIFT;
		static string_traits::cstr RUBBER_INC_SHIFT;
	};
	//------------------------------------------------------------
	struct VehicleBody
	{
		static string_traits::cstr BODY_NAME;
		static string_traits::cstr BOUND_MESH_NAME;
		static string_traits::cstr BOUND_BOX;
		static string_traits::cstr BRAKE_CONTROL;
		static string_traits::cstr MAX_STEER_ANGLE;
		static string_traits::cstr MOTOR_NAME;
		static string_traits::cstr GEARBOX_NAME;
		static string_traits::cstr DIFF_NAME;
		static string_traits::cstr LEFT_WHEEL_NODE;
		static string_traits::cstr RIGHT_WHEEL_NODE;
		static string_traits::cstr BRAKE_MAX_PRESS_VALUE;
		static string_traits::cstr TCS_NAME;
		static string_traits::cstr SOLVER_COUNT;
		static string_traits::cstr PHYSX_MAT_NAME;
		static string_traits::cstr ADD_DOWN_FORCE;

		static string_traits::cstr SPEEDOMETER;
		static string_traits::cstr SPDM_REV_AXIS;
		static string_traits::cstr SPDM_REV_SIDE;
		static string_traits::cstr SPDM_WHEEL_RADIUS;
	};
	//------------------------------------------------------------
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
	struct VehicleCommon
	{
		//specific keywords
		static string_traits::cstr VEHICLE_PARAMS;
		static string_traits::cstr BRAKE_BLOCK;
		static string_traits::cstr MOTOR_PARAMS;
		static string_traits::cstr GEARBOX_PARAMS;
		static string_traits::cstr FLYWHEEL_PARAMS;
		static string_traits::cstr DIFFERENTIAL_PARAMS;
		static string_traits::cstr DIFFERENTIAL_CHAIN;
		static string_traits::cstr VBV;
		static string_traits::cstr DRIVER_TIMING;
		static string_traits::cstr DRIVER;
		static string_traits::cstr MCM_PACK;
		static string_traits::cstr TCS;
#	if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		static string_traits::cstr PHYSX_SUSPENSION_DESC;
		static string_traits::cstr PHYSX_TIRE_DESC;
		static string_traits::cstr PHYSX_WHEEL;
		static string_traits::cstr PHYSX_WHEEL_NODE;
		static string_traits::cstr PHYSX_VEHICLE_BODY;
#	endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		//common keywords
		static string_traits::cstr NAME;
		static string_traits::cstr OSN_NAME;
		static string_traits::cstr RATIO;
		static string_traits::cstr ENTRY;
		static string_traits::cstr ENTRIES;
		static string_traits::cstr VALUE;
		static string_traits::cstr MODE;
		static string_traits::cstr MODES;
		static string_traits::cstr NODE;
		static string_traits::cstr TYPE;
		static string_traits::cstr MAX_DRAG;
		static string_traits::cstr DRAG_LOOKUP;
		static string_traits::cstr POSITION;
		static string_traits::cstr AXLE;
		static string_traits::cstr INDEX;
		static string_traits::cstr MASS;
		static string_traits::cstr MASS_OFFSET;
		static string_traits::cstr RADIUS;
		static string_traits::cstr OPTION;
	};
	//------------------------------------------------------------
}
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
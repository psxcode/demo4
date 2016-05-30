#include "VehicleParamsCollection.h"
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS


#include "SceneParamsCollection.h"

namespace Ogre
{
	Demo::System::VehicleParamsCollection* Ogre::Singleton< Demo::System::VehicleParamsCollection >::ms_Singleton = 0;
}

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	VehicleParamsCollection& VehicleParamsCollection::getSingleton()
	{
		assert( ms_Singleton ); return *ms_Singleton;
	}
	VehicleParamsCollection* VehicleParamsCollection::getSingletonPtr()
	{
		return ms_Singleton;
	}
	//------------------------------------------------------------
	typedef VehicleParamsCollection VPC;
	typedef SceneParamsCollection SPC;
	//--------------------------------//
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	//--------------------------------//
	struct MotorKeys
	{
		static VPC::string_traits::cstr STALL_RPM;
		static VPC::string_traits::cstr IDLE_RPM;
		static VPC::string_traits::cstr START_RPM;
		static VPC::string_traits::cstr RED_RPM;
		static VPC::string_traits::cstr MAX_RPM;
		static VPC::string_traits::cstr MAX_TORQUE;
		static VPC::string_traits::cstr MAX_STARTER_TORQUE;
		static VPC::string_traits::cstr TORQUE_LOOKUP;
		static VPC::string_traits::cstr CALM_RPM_CONTROL_DATA;
		static VPC::string_traits::cstr AGR_RPM_CONTROL_DATA;
		static VPC::string_traits::cstr TORQUE_MULT_FUNC;
		static VPC::string_traits::cstr TORQUE_MULT;
	};
	//--------------------------------//
	struct GearboxKeys
	{
		static VPC::string_traits::cstr GT_MECHANICAL;
		static VPC::string_traits::cstr GT_SEQUENTAL;
		static VPC::string_traits::cstr GT_AUTOMATIC;
		static VPC::string_traits::cstr AGM_DIRECT;
		static VPC::string_traits::cstr AGM_NEUTRAL;
		static VPC::string_traits::cstr AGM_REVERSE;
		static VPC::string_traits::cstr AGM_PARK;
		static VPC::string_traits::cstr AGM_SEQ_EMU;
		static VPC::string_traits::cstr AGM_SPORT;
		static VPC::string_traits::cstr DIRECT_RATIOS;
		static VPC::string_traits::cstr REVERSE_RATIOS;
		static VPC::string_traits::cstr MAX_BRAKE_TORQUE;
		static VPC::string_traits::cstr MECH_SHIFT_TIME;
	};
	//--------------------------------//
	struct DifferentialKeys
	{
		static VPC::string_traits::cstr DM_LOCK;
		static VPC::string_traits::cstr DM_DIS_FIRST;
		static VPC::string_traits::cstr DM_DIS_SECOND;
		static VPC::string_traits::cstr DIFF_RATIO;
		static VPC::string_traits::cstr FIRST_NODE_RATIO;
	};
	//--------------------------------//
	struct BrakeBlockKeys
	{
		static VPC::string_traits::cstr MAX_BRAKE_TORQUE;
	};
	//--------------------------------//
	struct VBVKeys
	{
		static VPC::string_traits::cstr START_VALUE;
		static VPC::string_traits::cstr INC_VALUE;
		static VPC::string_traits::cstr MULT_VALUE;
	};
	//--------------------------------//
	struct DriverTimingKeys
	{
		static VPC::string_traits::cstr THROTTLE_PRESS;
		static VPC::string_traits::cstr THROTTLE_RELEASE;
		static VPC::string_traits::cstr BRAKE_PRESS;
		static VPC::string_traits::cstr BRAKE_RELEASE;
		static VPC::string_traits::cstr CLUTCH_PRESS;
		static VPC::string_traits::cstr CLUTCH_RELEASE;
		static VPC::string_traits::cstr HANDBRAKE_PRESS;
		static VPC::string_traits::cstr HANDBRAKE_RELEASE;
		static VPC::string_traits::cstr STEER_PRESS;
		static VPC::string_traits::cstr STEER_RELEASE;
		static VPC::string_traits::cstr SHIFT_TO_NEUTRAL;
		static VPC::string_traits::cstr SHIFT_TO_DIRECT;
		static VPC::string_traits::cstr SHIFT_TO_REVERSE;
		static VPC::string_traits::cstr SHIFT_DELAY;
		static VPC::string_traits::cstr SWITCH_MODE;
		static VPC::string_traits::cstr SWITCH_SEQ;
		static VPC::string_traits::cstr RPM_ZONE_REACTION;
		static VPC::string_traits::cstr RPM_SPEED_REACTION;
	};
	//--------------------------------//
	struct MotorChartMarksPackKeys
	{
		static VPC::string_traits::cstr MARKS;
		static VPC::string_traits::cstr GEAR;
		static VPC::string_traits::cstr MIN;
		static VPC::string_traits::cstr NORM_LOW;
		static VPC::string_traits::cstr NORM_HIGH;
		static VPC::string_traits::cstr MAX;
	};
	//--------------------------------//
	struct TCSKeys
	{
		static VPC::string_traits::cstr FEED_VALUE;
		static VPC::string_traits::cstr IDLE_MARK;
		static VPC::string_traits::cstr IDLE_RANGE;
		static VPC::string_traits::cstr TYPE_CONST_FEED;
	};
	//--------------------------------//
	struct VehicleDriverKeys
	{
		static VPC::string_traits::cstr CALM_TIMING;
		static VPC::string_traits::cstr AGR_TIMING;
		static VPC::string_traits::cstr CALM_MCMP;
		static VPC::string_traits::cstr AGR_MCMP;
		static VPC::string_traits::cstr DRIVE_OPTIONS;
		static VPC::string_traits::cstr DO_THROTTLE_ASSIST;
		static VPC::string_traits::cstr DO_BRAKE_ASSIST;
		static VPC::string_traits::cstr DO_CLUTCH_ASSIST;
		static VPC::string_traits::cstr DO_STEER_ASSIST;
		static VPC::string_traits::cstr DO_SHIFT_ASSIST;
		static VPC::string_traits::cstr DO_REVERSE_PREVENT;
		static VPC::string_traits::cstr DO_NEUTRAL_PREVENT;
		static VPC::string_traits::cstr DO_USER_CONTROLS;
		static VPC::string_traits::cstr DO_USER_CLUTCH;
	};
	//--------------------------------//
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//--------------------------------//
	struct SuspensionKeys
	{
		static VPC::string_traits::cstr TRAVEL;
		static VPC::string_traits::cstr SPRING;
		static VPC::string_traits::cstr DAMPER;
		static VPC::string_traits::cstr TARGET_VALUE;
	};
	//--------------------------------//
	struct TireKeys
	{
		static VPC::string_traits::cstr LONG_FUNC;
		static VPC::string_traits::cstr LAT_FUNC;
		static VPC::string_traits::cstr EXTREMUM_SLIP;
		static VPC::string_traits::cstr EXTREMUM_VALUE;
		static VPC::string_traits::cstr ASYMPTOTE_SLIP;
		static VPC::string_traits::cstr ASYMPTOTE_VALUE;
		static VPC::string_traits::cstr STIFFNESS_FACTOR;
	};
	//--------------------------------//
	struct WheelKeys
	{
		static VPC::string_traits::cstr RIM_RADIUS;
		static VPC::string_traits::cstr TIRE_INC_RADIUS;
		static VPC::string_traits::cstr TIRE_WIDTH;
		static VPC::string_traits::cstr TIRE_NAME;
	};
	//--------------------------------//
	struct WheelNodeKeys
	{
		static VPC::string_traits::cstr SUS_NAME;
		static VPC::string_traits::cstr RUBBER_WHEEL_NAME;
		static VPC::string_traits::cstr IRON_WHEEL_NAME;
		static VPC::string_traits::cstr BRAKE_BLOCK_NAME;
		static VPC::string_traits::cstr IRON_INC_SHIFT;
		static VPC::string_traits::cstr RUBBER_INC_SHIFT;
	};
	//--------------------------------//
	struct VehicleBodyKeys
	{
		static VPC::string_traits::cstr BODY_NAME;
		static VPC::string_traits::cstr BOUND_MESH_NAME;
		static VPC::string_traits::cstr BOUND_BOX;
		static VPC::string_traits::cstr BRAKE_CONTROL;
		static VPC::string_traits::cstr MAX_STEER_ANGLE;
		static VPC::string_traits::cstr MOTOR_NAME;
		static VPC::string_traits::cstr GEARBOX_NAME;
		static VPC::string_traits::cstr DIFF_NAME;
		static VPC::string_traits::cstr LEFT_WHEEL_NODE;
		static VPC::string_traits::cstr RIGHT_WHEEL_NODE;
		static VPC::string_traits::cstr BRAKE_MAX_PRESS_VALUE;
		static VPC::string_traits::cstr TCS_NAME;
		static VPC::string_traits::cstr SOLVER_COUNT;
		static VPC::string_traits::cstr PHYSX_MAT_NAME;
		static VPC::string_traits::cstr ADD_DOWN_FORCE;

		static VPC::string_traits::cstr SPEEDOMETER;
		static VPC::string_traits::cstr SPDM_REV_AXIS;
		static VPC::string_traits::cstr SPDM_REV_SIDE;
		static VPC::string_traits::cstr SPDM_WHEEL_RADIUS;
	};
	//--------------------------------//
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//--------------------------------//
	struct MainKeys
	{
		//specific keywords
		static VPC::string_traits::cstr VEHICLE_PARAMS;
		static VPC::string_traits::cstr BRAKE_BLOCK;
		static VPC::string_traits::cstr MOTOR_PARAMS;
		static VPC::string_traits::cstr GEARBOX_PARAMS;
		static VPC::string_traits::cstr FLYWHEEL_PARAMS;
		static VPC::string_traits::cstr DIFFERENTIAL_PARAMS;
		static VPC::string_traits::cstr DIFFERENTIAL_CHAIN;
		static VPC::string_traits::cstr VBV;
		static VPC::string_traits::cstr DRIVER_TIMING;
		static VPC::string_traits::cstr DRIVER;
		static VPC::string_traits::cstr MCM_PACK;
		static VPC::string_traits::cstr TCS;
#	if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		static VPC::string_traits::cstr PHYSX_SUSPENSION_DESC;
		static VPC::string_traits::cstr PHYSX_TIRE_DESC;
		static VPC::string_traits::cstr PHYSX_WHEEL;
		static VPC::string_traits::cstr PHYSX_WHEEL_NODE;
		static VPC::string_traits::cstr PHYSX_VEHICLE_BODY;
#	endif
		//common keywords
		static VPC::string_traits::cstr NAME;
		static VPC::string_traits::cstr OSN_NAME;
		static VPC::string_traits::cstr RATIO;
		static VPC::string_traits::cstr ENTRY;
		static VPC::string_traits::cstr ENTRIES;
		static VPC::string_traits::cstr VALUE;
		static VPC::string_traits::cstr MODE;
		static VPC::string_traits::cstr MODES;
		static VPC::string_traits::cstr NODE;
		static VPC::string_traits::cstr TYPE;
		static VPC::string_traits::cstr MAX_DRAG;
		static VPC::string_traits::cstr DRAG_LOOKUP;
		static VPC::string_traits::cstr POSITION;
		static VPC::string_traits::cstr AXLE;
		static VPC::string_traits::cstr INDEX;
		static VPC::string_traits::cstr MASS;
		static VPC::string_traits::cstr MASS_OFFSET;
		static VPC::string_traits::cstr RADIUS;
		static VPC::string_traits::cstr OPTION;
	};
	//--------------------------------//
	VPC::string_traits::cstr MainKeys::BRAKE_BLOCK = "brakeblock";
	VPC::string_traits::cstr MainKeys::DIFFERENTIAL_CHAIN = "diffchain";
	VPC::string_traits::cstr MainKeys::DIFFERENTIAL_PARAMS = "diffparams";
	VPC::string_traits::cstr MainKeys::GEARBOX_PARAMS = "gearboxparams";
	VPC::string_traits::cstr MainKeys::MOTOR_PARAMS = "motorparams";
	VPC::string_traits::cstr MainKeys::VBV = "vbv";
	VPC::string_traits::cstr MainKeys::VEHICLE_PARAMS = "vehicleparams";
	VPC::string_traits::cstr MainKeys::NAME = "name";
	VPC::string_traits::cstr MainKeys::OSN_NAME = "osnname";
	VPC::string_traits::cstr MainKeys::VALUE = "value";
	VPC::string_traits::cstr MainKeys::RATIO = "ratio";
	VPC::string_traits::cstr MainKeys::MODE = "mode";
	VPC::string_traits::cstr MainKeys::MODES = "modes";
	VPC::string_traits::cstr MainKeys::NODE = "node";
	VPC::string_traits::cstr MainKeys::MAX_DRAG = "maxdrag";
	VPC::string_traits::cstr MainKeys::DRAG_LOOKUP = "draglookup";
	VPC::string_traits::cstr MainKeys::TYPE = "type";
	VPC::string_traits::cstr MainKeys::FLYWHEEL_PARAMS = "flywheelparams";
	VPC::string_traits::cstr MainKeys::MASS = "mass";
	VPC::string_traits::cstr MainKeys::MASS_OFFSET = "massoffset";
	VPC::string_traits::cstr MainKeys::MCM_PACK = "motorchartmarkspack";
	VPC::string_traits::cstr MainKeys::POSITION = "position";
	VPC::string_traits::cstr MainKeys::DRIVER_TIMING = "drivertiming";
	VPC::string_traits::cstr MainKeys::DRIVER = "driver";
	VPC::string_traits::cstr MainKeys::ENTRY = "entry";
	VPC::string_traits::cstr MainKeys::ENTRIES = "entries";
	VPC::string_traits::cstr MainKeys::AXLE = "axle";
	VPC::string_traits::cstr MainKeys::INDEX = "index";
	VPC::string_traits::cstr MainKeys::RADIUS = "radius";
	VPC::string_traits::cstr MainKeys::OPTION = "option";
	VPC::string_traits::cstr MainKeys::TCS = "tcs";
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	VPC::string_traits::cstr MainKeys::PHYSX_SUSPENSION_DESC = "physxsusdesc";
	VPC::string_traits::cstr MainKeys::PHYSX_TIRE_DESC = "physxtiredesc";
	VPC::string_traits::cstr MainKeys::PHYSX_VEHICLE_BODY = "physxvehiclebody";
	VPC::string_traits::cstr MainKeys::PHYSX_WHEEL = "physxwheel";
	VPC::string_traits::cstr MainKeys::PHYSX_WHEEL_NODE = "physxwheelnode";
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
	VPC::string_traits::cstr MotorKeys::IDLE_RPM = "idlerpm";
	VPC::string_traits::cstr MotorKeys::MAX_RPM = "maxrpm";
	VPC::string_traits::cstr MotorKeys::MAX_TORQUE = "maxtorque";
	VPC::string_traits::cstr MotorKeys::MAX_STARTER_TORQUE = "maxstartertorque";
	VPC::string_traits::cstr MotorKeys::RED_RPM = "redrpm";
	VPC::string_traits::cstr MotorKeys::STALL_RPM = "stallrpm";
	VPC::string_traits::cstr MotorKeys::START_RPM = "startrpm";
	VPC::string_traits::cstr MotorKeys::TORQUE_LOOKUP = "torquelookup";
	VPC::string_traits::cstr MotorKeys::AGR_RPM_CONTROL_DATA = "agrrpmcontroldata";
	VPC::string_traits::cstr MotorKeys::CALM_RPM_CONTROL_DATA = "calmrpmcontroldata";
	VPC::string_traits::cstr MotorKeys::TORQUE_MULT_FUNC = "belowidletorquemultfunc";
	VPC::string_traits::cstr MotorKeys::TORQUE_MULT = "belowidletorquemult";
	//------------------------------------------------------------
	VPC::string_traits::cstr GearboxKeys::GT_MECHANICAL = "mech";
	VPC::string_traits::cstr GearboxKeys::GT_AUTOMATIC = "auto";
	VPC::string_traits::cstr GearboxKeys::GT_SEQUENTAL = "seq";
	VPC::string_traits::cstr GearboxKeys::AGM_DIRECT = "direct";
	VPC::string_traits::cstr GearboxKeys::AGM_NEUTRAL = "neutral";
	VPC::string_traits::cstr GearboxKeys::AGM_PARK = "park";
	VPC::string_traits::cstr GearboxKeys::AGM_REVERSE = "reverse";
	VPC::string_traits::cstr GearboxKeys::AGM_SEQ_EMU = "seqemu";
	VPC::string_traits::cstr GearboxKeys::AGM_SPORT = "sport";
	VPC::string_traits::cstr GearboxKeys::MECH_SHIFT_TIME = "mechshifttime";
	VPC::string_traits::cstr GearboxKeys::DIRECT_RATIOS = "directratios";
	VPC::string_traits::cstr GearboxKeys::REVERSE_RATIOS = "reverseratios";
	//------------------------------------------------------------
	VPC::string_traits::cstr DifferentialKeys::DM_LOCK = "lock";
	VPC::string_traits::cstr DifferentialKeys::DM_DIS_FIRST = "disfirst";
	VPC::string_traits::cstr DifferentialKeys::DM_DIS_SECOND = "dissecond";
	VPC::string_traits::cstr DifferentialKeys::DIFF_RATIO = "diffratio";
	VPC::string_traits::cstr DifferentialKeys::FIRST_NODE_RATIO = "firstnoderatio";
	//------------------------------------------------------------
	VPC::string_traits::cstr BrakeBlockKeys::MAX_BRAKE_TORQUE = "maxbraketorque";
	//------------------------------------------------------------
	VPC::string_traits::cstr TCSKeys::FEED_VALUE = "feedvalue";
	VPC::string_traits::cstr TCSKeys::IDLE_MARK = "idlemark";
	VPC::string_traits::cstr TCSKeys::IDLE_RANGE = "idlerange";
	VPC::string_traits::cstr TCSKeys::TYPE_CONST_FEED = "cfeed";
	//------------------------------------------------------------
	VPC::string_traits::cstr VBVKeys::MULT_VALUE = "multvalue";
	VPC::string_traits::cstr VBVKeys::START_VALUE = "startvalue";
	VPC::string_traits::cstr VBVKeys::INC_VALUE = "incvalue";
	//------------------------------------------------------------
	VPC::string_traits::cstr DriverTimingKeys::BRAKE_PRESS = "brakepress";
	VPC::string_traits::cstr DriverTimingKeys::BRAKE_RELEASE = "brakerelease";
	VPC::string_traits::cstr DriverTimingKeys::CLUTCH_PRESS = "clutchpress";
	VPC::string_traits::cstr DriverTimingKeys::CLUTCH_RELEASE = "clutchrelease";
	VPC::string_traits::cstr DriverTimingKeys::HANDBRAKE_PRESS = "handbrakepress";
	VPC::string_traits::cstr DriverTimingKeys::HANDBRAKE_RELEASE = "handbrakerelease";
	VPC::string_traits::cstr DriverTimingKeys::RPM_SPEED_REACTION = "rpmspeedreaction";
	VPC::string_traits::cstr DriverTimingKeys::RPM_ZONE_REACTION = "rpmzonereaction";
	VPC::string_traits::cstr DriverTimingKeys::SHIFT_DELAY = "shiftdelay";
	VPC::string_traits::cstr DriverTimingKeys::SHIFT_TO_DIRECT = "shifttodirect";
	VPC::string_traits::cstr DriverTimingKeys::SHIFT_TO_NEUTRAL = "shifttoneutral";
	VPC::string_traits::cstr DriverTimingKeys::SHIFT_TO_REVERSE = "shifttoreverse";
	VPC::string_traits::cstr DriverTimingKeys::STEER_PRESS = "steerpress";
	VPC::string_traits::cstr DriverTimingKeys::STEER_RELEASE = "steerrelease";
	VPC::string_traits::cstr DriverTimingKeys::SWITCH_MODE = "switchmode";
	VPC::string_traits::cstr DriverTimingKeys::SWITCH_SEQ = "switchseq";
	VPC::string_traits::cstr DriverTimingKeys::THROTTLE_PRESS = "throttlepress";
	VPC::string_traits::cstr DriverTimingKeys::THROTTLE_RELEASE = "throttlerelease";
	//------------------------------------------------------------
	VPC::string_traits::cstr MotorChartMarksPackKeys::MARKS = "marks";
	VPC::string_traits::cstr MotorChartMarksPackKeys::GEAR = "gear";
	VPC::string_traits::cstr MotorChartMarksPackKeys::MAX = "max";
	VPC::string_traits::cstr MotorChartMarksPackKeys::MIN = "min";
	VPC::string_traits::cstr MotorChartMarksPackKeys::NORM_HIGH = "normhigh";
	VPC::string_traits::cstr MotorChartMarksPackKeys::NORM_LOW = "normlow";
	//------------------------------------------------------------
	VPC::string_traits::cstr VehicleDriverKeys::AGR_MCMP = "agrmotorchartmarkspack";
	VPC::string_traits::cstr VehicleDriverKeys::CALM_MCMP = "calmmotorchartmarkspack";
	VPC::string_traits::cstr VehicleDriverKeys::AGR_TIMING = "agrtiming";
	VPC::string_traits::cstr VehicleDriverKeys::CALM_TIMING = "calmtiming";
	VPC::string_traits::cstr VehicleDriverKeys::DRIVE_OPTIONS = "driveoptions";
	VPC::string_traits::cstr VehicleDriverKeys::DO_BRAKE_ASSIST = "brakeassist";
	VPC::string_traits::cstr VehicleDriverKeys::DO_CLUTCH_ASSIST = "clutchassist";
	VPC::string_traits::cstr VehicleDriverKeys::DO_SHIFT_ASSIST = "shiftassist";
	VPC::string_traits::cstr VehicleDriverKeys::DO_STEER_ASSIST = "steerassist";
	VPC::string_traits::cstr VehicleDriverKeys::DO_THROTTLE_ASSIST = "throttleassist";
	VPC::string_traits::cstr VehicleDriverKeys::DO_NEUTRAL_PREVENT = "neutralprevent";
	VPC::string_traits::cstr VehicleDriverKeys::DO_REVERSE_PREVENT = "reverseprevent";
	VPC::string_traits::cstr VehicleDriverKeys::DO_USER_CLUTCH = "userclutch";
	VPC::string_traits::cstr VehicleDriverKeys::DO_USER_CONTROLS = "usercontrols";
	//------------------------------------------------------------
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
	VPC::string_traits::cstr SuspensionKeys::DAMPER = "damper";
	VPC::string_traits::cstr SuspensionKeys::SPRING = "spring";
	VPC::string_traits::cstr SuspensionKeys::TRAVEL = "travel";
	VPC::string_traits::cstr SuspensionKeys::TARGET_VALUE = "targetvalue";
	//------------------------------------------------------------
	VPC::string_traits::cstr TireKeys::LONG_FUNC = "longfunction";
	VPC::string_traits::cstr TireKeys::LAT_FUNC = "latfunction";
	VPC::string_traits::cstr TireKeys::STIFFNESS_FACTOR = "stiffnessfactor";
	VPC::string_traits::cstr TireKeys::ASYMPTOTE_SLIP = "asymptoteslip";
	VPC::string_traits::cstr TireKeys::ASYMPTOTE_VALUE = "asymptotevalue";
	VPC::string_traits::cstr TireKeys::EXTREMUM_SLIP = "extremumslip";
	VPC::string_traits::cstr TireKeys::EXTREMUM_VALUE = "extremumvalue";
	//------------------------------------------------------------
	VPC::string_traits::cstr WheelKeys::RIM_RADIUS = "rimradius";
	VPC::string_traits::cstr WheelKeys::TIRE_INC_RADIUS = "tireincradius";
	VPC::string_traits::cstr WheelKeys::TIRE_NAME = "tirename";
	VPC::string_traits::cstr WheelKeys::TIRE_WIDTH = "tirewidth";
	//------------------------------------------------------------
	VPC::string_traits::cstr WheelNodeKeys::SUS_NAME = "sus";
	VPC::string_traits::cstr WheelNodeKeys::RUBBER_WHEEL_NAME = "rubberwheel";
	VPC::string_traits::cstr WheelNodeKeys::IRON_WHEEL_NAME = "ironwheel";
	VPC::string_traits::cstr WheelNodeKeys::BRAKE_BLOCK_NAME = "brakeblock";
	VPC::string_traits::cstr WheelNodeKeys::RUBBER_INC_SHIFT = "rubberwheelincshift";
	VPC::string_traits::cstr WheelNodeKeys::IRON_INC_SHIFT = "ironwheelincshift";
	//------------------------------------------------------------
	VPC::string_traits::cstr VehicleBodyKeys::BODY_NAME = "bodyname";
	VPC::string_traits::cstr VehicleBodyKeys::BOUND_BOX = "boundbox";
	VPC::string_traits::cstr VehicleBodyKeys::BOUND_MESH_NAME = "boundmeshname";
	VPC::string_traits::cstr VehicleBodyKeys::BRAKE_CONTROL = "brakecontrol";
	VPC::string_traits::cstr VehicleBodyKeys::BRAKE_MAX_PRESS_VALUE = "brakemaxpressvalue";
	VPC::string_traits::cstr VehicleBodyKeys::DIFF_NAME = "diffchainname";
	VPC::string_traits::cstr VehicleBodyKeys::GEARBOX_NAME = "gearboxname";
	VPC::string_traits::cstr VehicleBodyKeys::LEFT_WHEEL_NODE = "leftwheelnode";
	VPC::string_traits::cstr VehicleBodyKeys::RIGHT_WHEEL_NODE = "rightwheelnode";
	VPC::string_traits::cstr VehicleBodyKeys::MAX_STEER_ANGLE = "maxsteerangle";
	VPC::string_traits::cstr VehicleBodyKeys::MOTOR_NAME = "motorname";
	VPC::string_traits::cstr VehicleBodyKeys::PHYSX_MAT_NAME = "physxmatname";
	VPC::string_traits::cstr VehicleBodyKeys::TCS_NAME = "tcsname";
	VPC::string_traits::cstr VehicleBodyKeys::SOLVER_COUNT = "solvercount";
	VPC::string_traits::cstr VehicleBodyKeys::ADD_DOWN_FORCE = "adddownforce";
	VPC::string_traits::cstr VehicleBodyKeys::SPEEDOMETER = "speedometer";
	VPC::string_traits::cstr VehicleBodyKeys::SPDM_REV_AXIS = "revsensoraxis";
	VPC::string_traits::cstr VehicleBodyKeys::SPDM_REV_SIDE = "revsensorside";
	VPC::string_traits::cstr VehicleBodyKeys::SPDM_WHEEL_RADIUS = "wheelradius";
	//------------------------------------------------------------
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML
	//------------------------------------------------------------
	//typedef SceneParamsCollection SPC;
	typedef Ogre::StringUtil SU;
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	typedef MainKeys MK;
	typedef OgremaxTinyXmlInterface XI;
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML
	//------------------------------------------------------------
	VehicleParamsCollection::VehicleParamsCollection()
	{
	}
	//------------------------------------------------------------
	VehicleParamsCollection::~VehicleParamsCollection()
	{
	}
	//------------------------------------------------------------
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
	VPC::traits::PhysXVehicleBodyParams_traits::out VehicleParamsCollection::GetPhysXVehicleBodyParams(string_traits::in name)
	{
		if( name.empty() )return notFoundPhysXVehicleBodyParams;
		traits::PhysXVehicleBodyParams_traits::container::iterator iter = physxVehicleBodyParamsContainer.find( name );
		if( iter != physxVehicleBodyParamsContainer.end() ) {
			return iter->second;
		} else {
			traits::PhysXVehicleBodyParamsRSD_traits::container::iterator i = physxVehicleBodyParamsRsdContainer.find(name);
			return iter != physxVehicleBodyParamsContainer.end() ? CreatePhysXVehicleBodyParams( i->second.getReference() ) : notFoundPhysXVehicleBodyParams;
		}
	}
	//------------------------------------------------------------
	VPC::traits::PhysXVehicleBodyParams_traits::out VehicleParamsCollection::CreatePhysXVehicleBodyParams(const PhysXVehicleBodyParamsRSD &rsd)
	{
		VPC::traits::PhysXVehicleBodyParams_traits::store_type newBody( new Vehicle::PhysXVehicleBodyParams );
		newBody->setName = rsd.setName;
		newBody->bodyName = rsd.bodyName;
		newBody->bodyOSNParams = SPC::getSingleton().GetVehicleSnp( rsd.osnName );

		if( newBody->bodyOSNParams == 0 )
			return notFoundPhysXVehicleBodyParams;

		newBody->motor =GetMotorParams( rsd.motorName );
		if( newBody->motor == notFoundMotorParams )
			return notFoundPhysXVehicleBodyParams;

		newBody->gearbox = GetGearboxParams( rsd.gearboxName );
		if( newBody->gearbox == notFoundGearboxParams )
			return notFoundPhysXVehicleBodyParams;

		newBody->diff = GetDiffChainParams( rsd.diffName );
		if( newBody->diff == notFoundDifferentialParams )
			return notFoundPhysXVehicleBodyParams;

		newBody->tcs = GetThrottleControlParams( rsd.tcsName );
		if( newBody->tcs == notFoundThrottleControlParams )
			return notFoundPhysXVehicleBodyParams;

		newBody->boundMeshName = rsd.boundMeshName;
		newBody->mass = rsd.mass;
		newBody->massOffset = rsd.massOffset;
		newBody->boundBox = rsd.boundBox;
		newBody->solverCount = rsd.solverCount;
		newBody->physxMatName = rsd.physxMatName;
		newBody->addDownForce = rsd.addDownForce;

		newBody->speedometer.revSensorAxis = rsd.speedometer.revSensorAxis;
		newBody->speedometer.revSensorSide = rsd.speedometer.revSensorSide;
		newBody->speedometer.wheelRadius = rsd.speedometer.wheelRadius;
		
		PhysXVehicleBodyParamsRSD::traits::axle_container::const_iterator iter = rsd.axleArray.begin();
		PhysXVehicleBodyParamsRSD::traits::axle_container::const_iterator end_iter = rsd.axleArray.end();
		for( ; iter != end_iter; ++iter )
		{
			Vehicle::PhysXVehicleBodyParams::traits::axle_type axle( new Vehicle::PhysXVehicleBodyParams::PhysXAxleParams );
			newBody->axles.push_back( axle );
			axle->index = iter->index;
			axle->maxSteerAngle = iter->maxSteerAngle;
			axle->position = iter->position;
			axle->leftWheelNodeParams = GetPhysXWheelNodeParams( iter->leftWheelNode );
			axle->rightWheelNodeParams = GetPhysXWheelNodeParams( iter->rightWheelNode );
			axle->brakeMaxPressValue = iter->brakeMaxPressValue;
		}

		AddPhysXVehicleBodyParams(traits::PhysXVehicleBodyParams_traits::container::value_type( newBody->setName, newBody ));

		return newBody;
	}
	//------------------------------------------------------------
	VPC::traits::PhysXWheelParams_traits::out VehicleParamsCollection::GetPhysXWheelParams(string_traits::in name)
	{
		if( name.empty() )return notFoundPhysXWheelParams;
		traits::PhysXWheelParams_traits::container::iterator iter = physxWheelParamsContainer.find( name );
		if( iter != physxWheelParamsContainer.end() ) {
			return iter->second;
		} else {
			traits::PhysXWheelParamsRSD_traits::container::iterator i = physxWheelParamsRsdContainer.find(name);
			return i != physxWheelParamsRsdContainer.end() ? CreatePhysXWheelParams( i->second.getReference() ) : notFoundPhysXWheelParams;
		}
	}
	//------------------------------------------------------------
	VPC::traits::PhysXWheelParams_traits::out VehicleParamsCollection::CreatePhysXWheelParams(const PhysXWheelParamsRSD &rsd)
	{
		traits::PhysXWheelParams_traits::store_type newWheel( new Vehicle::PhysXWheelParams() );
		newWheel->name = rsd.name;
		newWheel->mass = rsd.mass;
		newWheel->radius = rsd.rimRadius;
		newWheel->tireIncRadius = rsd.tireIncRadius;
		newWheel->width = rsd.tireWidth;

		newWheel->wheelOSN = SPC::getSingleton().GetVehicleSnp( rsd.osnName );
		if( newWheel->wheelOSN == 0 )
			return Vehicle::PhysXWheelParams::shared_ptr();

		newWheel->tireDesc = GetPhysXTireDesc( rsd.tireName );
		if( newWheel->tireDesc == 0 )
			return Vehicle::PhysXWheelParams::shared_ptr();

		AddPhysXWheelParams( traits::PhysXWheelParams_traits::container::value_type( newWheel->name, newWheel ) );

		return newWheel;
	}
	//------------------------------------------------------------
	VPC::traits::PhysXWheelNodeParams_traits::out VehicleParamsCollection::GetPhysXWheelNodeParams(string_traits::in name)
	{
		if( name.empty() )return notFoundPhysXWheelNodeParams;
		traits::PhysXWheelNodeParams_traits::container::iterator iter = physxWheelNodeParamsContainer.find( name );
		if( iter != physxWheelNodeParamsContainer.end() ) {
			return iter->second;
		} else {
			traits::PhysXWheelNodeParamsRSD_traits::container::iterator i = physxWheelNodeParamsRsdContainer.find(name);
			return i != physxWheelNodeParamsRsdContainer.end() ? CreatePhysXWheelNodeParams( i->second.getReference() ) : notFoundPhysXWheelNodeParams;
		}
	}
	//------------------------------------------------------------
	VPC::traits::PhysXWheelNodeParams_traits::out VehicleParamsCollection::CreatePhysXWheelNodeParams(const PhysXWheelNodeParamsRSD &rsd)
	{
		traits::FloatVBV_traits::store_type dragLookup;
		if( !(rsd.dragLookupName.empty()) )
		{
			Core::SquaredLinearVBV< float, float >* newDragLookup = new Core::SquaredLinearVBV< float, float >();
			dragLookup = GetDragLookup( rsd.dragLookupName, newDragLookup );
			if( dragLookup == 0 )
				return notFoundPhysXWheelNodeParams;
		}
		
		traits::PhysXWheelNodeParams_traits::store_type newWheelNode( new Vehicle::PhysXWheelNodeParams() );
		newWheelNode->nameMechanics = rsd.name;
		newWheelNode->maxDrag = rsd.maxDrag;
		newWheelNode->dragLookup = dragLookup;
		newWheelNode->brakeBlockParams = GetBrakeBlockParams( rsd.brakeBlockName );
		newWheelNode->ironWheelIncShift = rsd.ironIncShift;
		newWheelNode->rubberWheelIncShift = rsd.rubberIncShift;
		newWheelNode->ironWheelParams = GetPhysXWheelParams( rsd.ironWheelName );
		newWheelNode->rubberWheelParams = GetPhysXWheelParams( rsd.rubberWheelName );
		newWheelNode->susDesc = GetPhysXSuspensionDesc( rsd.susName );

		AddPhysXWheelNodeParams( traits::PhysXWheelNodeParams_traits::container::value_type( newWheelNode->nameMechanics, newWheelNode ) );

		return newWheelNode;
	}
	//------------------------------------------------------------
#endif
	//------------------------------------------------------------
	VPC::traits::MotorParams_traits::out VehicleParamsCollection::GetMotorParams( string_traits::in name )
	{
		traits::MotorParams_traits::container::iterator iter = motorParamsContainer.find( name );
		if( iter != motorParamsContainer.end() ) {
			return iter->second;
		} else {
			traits::MotorParamsRSD_traits::container::iterator i = motorParamsRsdContainer.find(name);
			return i != motorParamsRsdContainer.end() ? CreateMotorParams(i->second.getReference()) : notFoundMotorParams;
		}
	}
	//------------------------------------------------------------
	VPC::traits::FlywheelParams_traits::out VehicleParamsCollection::GetFlywheelParams( string_traits::in name )
	{
		traits::FlywheelParams_traits::container::iterator iter = flywheelParamsContainer.find( name );
		if( iter != flywheelParamsContainer.end() ) {
			return iter->second;
		} else {
			traits::FlywheelParamsRSD_traits::container::iterator i = flywheelParamsRsdContainer.find(name);
			return i != flywheelParamsRsdContainer.end() ? CreateFlywheelParams(i->second.getReference()) : notFoundFlywheelParams;
		}
	}
	//------------------------------------------------------------
	VPC::traits::GearboxParams_traits::out VehicleParamsCollection::GetGearboxParams( string_traits::in name )
	{
		traits::GearboxParams_traits::container::iterator iter = gearboxParamsContainer.find( name );
		if( iter != gearboxParamsContainer.end() ) {
			return iter->second;
		} else {
			traits::GearboxParamsRSD_traits::container::iterator i = gearboxParamsRsdContainer.find(name);
			return i != gearboxParamsRsdContainer.end() ? CreateGearboxParams(i->second.getReference()) : notFoundGearboxParams;
		}
	}
	//------------------------------------------------------------
	VPC::traits::DifferentialParams_traits::out VehicleParamsCollection::GetDiffChainParams(string_traits::in chainName)
	{
		traits::DifferentialChainRSD_traits::container::iterator iter = differentialChainRsdContainer.find( chainName );
		if( iter != differentialChainRsdContainer.end() ) {
			return CreateDifferentialParams( *(iter->second->firstNode) );
		} else {
			return notFoundDifferentialParams;
		}
	}
	//------------------------------------------------------------
	VPC::traits::DifferentialParams_traits::out VehicleParamsCollection::GetDifferentialParams( string_traits::in name)
	{
		traits::DifferentialParams_traits::container::iterator iter = differentialParamsContainer.find( name );
		if( iter != differentialParamsContainer.end() ) {
			return iter->second;
		} else {
			return GetDiffChainParams(name);
		}
	}
	//------------------------------------------------------------
	VPC::traits::BrakeBlockParams_traits::out VehicleParamsCollection::GetBrakeBlockParams( string_traits::in name )
	{
		traits::BrakeBlockParams_traits::container::iterator iter = brakeBlockParamsContainer.find( name );
		if( iter != brakeBlockParamsContainer.end() ) {
			return iter->second;
		} else {
			traits::BrakeBlockParamsRSD_traits::container::iterator i = brakeBlockParamsRsdContainer.find(name);
			return (i != brakeBlockParamsRsdContainer.end()) ? CreateBrakeBlockParams( i->second.getReference() ) : notFoundBrakeBlockParams;
		}
	}
	//------------------------------------------------------------
	VPC::traits::ThrottleControlParams_traits::out VehicleParamsCollection::GetThrottleControlParams(string_traits::in name)
	{
		traits::ThrottleControlParams_traits::container::iterator iter = throttleControlParamsContainer.find( name );
		if( iter != throttleControlParamsContainer.end() ) {
			return iter->second;
		} else {
			traits::ThrottleControlParamsRSD_traits::container::iterator i = throttleControlParamsRsdContainer.find(name);
			return i != throttleControlParamsRsdContainer.end() ? CreateThrottleControlParams(i->second.getReference()) : notFoundThrottleControlParams;
		}
	}
	//------------------------------------------------------------
	VPC::traits::DriverTiming_traits::out VehicleParamsCollection::GetDriverTiming( string_traits::in name )
	{
		traits::DriverTiming_traits::container::iterator iter = driverTimingContainer.find( name );
		if( iter == driverTimingContainer.end() )
			return notFoundDriverTiming;

		return iter->second;
	}
	//------------------------------------------------------------
	VPC::traits::MotorChartMarksPack_traits::out VehicleParamsCollection::GetMotorChartMarksPack(string_traits::in name)
	{
		traits::MotorChartMarksPack_traits::container::iterator iter = motorChartMarksPackContainer.find( name );
		if( iter == motorChartMarksPackContainer.end() )
			return notFoundMotorChartMarksPack;

		return iter->second;
	}
	//------------------------------------------------------------
	VPC::traits::FloatVBV_traits::out VehicleParamsCollection::GetDragLookup( string_traits::in name, Core::LinearVBV<float,float> *createdVBV )
	{
		Core::OgreRealVBV::shared_ptr returnVBV( createdVBV );
		traits::VBVRSD_traits::container::iterator iter = vbvRsdContainer.find( name );
		if( iter == vbvRsdContainer.end() || !createdVBV )
			return notFoundFloatVBV;

		if( !createdVBV->Initialize( iter->second.getReference() ) )
			return notFoundFloatVBV;

		return returnVBV;
	}
	//------------------------------------------------------------
	VPC::traits::FloatVBV_traits::out VehicleParamsCollection::GetTorqueLookup(string_traits::in name)
	{
		traits::VBVRSD_traits::container::iterator iter = vbvRsdContainer.find( name );
		if( iter == vbvRsdContainer.end() )
			return notFoundFloatVBV;

		Core::InterTableVBV< float, float >* newTable = new Core::InterTableVBV< float, float >();
		Core::OgreRealVBV::shared_ptr returnVBV( newTable );
		if( !newTable->Initialize( iter->second.getReference() ) )
			return notFoundFloatVBV;

		return returnVBV;
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
	VPC::traits::MotorParams_traits::out VehicleParamsCollection::CreateMotorParams( const MotorParamsRSD& rsd )
	{
		Core::OgreRealVBV::shared_ptr torqueVBV;
		Core::OgreRealVBV::shared_ptr dragVBV;
		
		torqueVBV = GetTorqueLookup( rsd.torqueLookupName );
		if( torqueVBV == 0 )return Vehicle::MotorParams::shared_ptr();

		Core::SquaredLinearVBV< float, float >* newDragLookup = new Core::SquaredLinearVBV< float, float >();
		dragVBV = GetDragLookup( rsd.dragLookupName, newDragLookup );
		if( dragVBV == 0 )return Vehicle::MotorParams::shared_ptr();

		Vehicle::FlywheelParams::shared_ptr fP = GetFlywheelParams( rsd.flywheelParamsName );
		if( fP == 0 )return Vehicle::MotorParams::shared_ptr();

		Vehicle::MotorParams::shared_ptr mP( new Vehicle::MotorParams() );
		mP->nameMechanics = rsd.motorName;
		mP->torqueLookup = torqueVBV;
		mP->dragLookup = dragVBV;
		mP->chart.stallRPM = rsd.stallRPM;
		mP->chart.idleRPM = rsd.idleRPM;
		mP->chart.startRPM = rsd.startRPM;
		mP->chart.redRPM = rsd.redRPM;
		mP->chart.maxRPM = rsd.maxRPM;
		mP->maxTorque = rsd.maxTorque;
		mP->maxStarterTorque = rsd.maxStarterTorque;
		mP->belowIdleTorqueMult = rsd.belowIdleTorqueMult;
		mP->belowIdleTorqueMultFunc = ParseMotorTorqueMultFunc( rsd.belowIdleTorqueMultFunc, Vehicle::MotorParams::TMF_NOT_DEFINED );
		mP->flywheel = fP;

		AddMotorParams( traits::MotorParams_traits::container::value_type( mP->nameMechanics, mP ) );
		
		return mP;
	}
	//------------------------------------------------------------
	VPC::traits::FlywheelParams_traits::out VehicleParamsCollection::CreateFlywheelParams( const FlywheelParamsRSD& rsd )
	{
		Core::OgreRealVBV::shared_ptr dragLookup;
		if( !(rsd.dragLookupName.empty()) )
		{
			Core::SquaredLinearVBV< float, float >* newDragLookup = new Core::SquaredLinearVBV< float, float >();
			dragLookup = GetDragLookup( rsd.dragLookupName, newDragLookup );
			if( dragLookup == 0 )return Vehicle::FlywheelParams::shared_ptr();
		}
		
		Vehicle::FlywheelParams::shared_ptr fP( new Vehicle::FlywheelParams() );
		fP->nameMechanics = rsd.name;
		fP->mass = rsd.mass;
		fP->radius = rsd.radius;
		fP->dragLookup = dragLookup;
		fP->maxDrag = rsd.maxDrag;

		AddFlywheelParams( traits::FlywheelParams_traits::container::value_type( fP->nameMechanics, fP ) );

		return fP;
	}
	//------------------------------------------------------------
	VPC::traits::GearboxParams_traits::out VehicleParamsCollection::CreateGearboxParams( const GearboxParamsRSD& rsd )
	{
		Core::OgreRealVBV::shared_ptr dragLookup;
		if( !(rsd.dragLookupName.empty()) )
		{
			Core::SquaredLinearVBV< float, float >* newDragLookup = new Core::SquaredLinearVBV< float, float >();
			dragLookup = GetDragLookup( rsd.dragLookupName, newDragLookup );
			if( dragLookup == 0 )return Vehicle::GearboxParams::shared_ptr();
		}

		Vehicle::FlywheelParams::shared_ptr fP = GetFlywheelParams( rsd.flywheelParamsName );
		if( fP == 0 )return Vehicle::GearboxParams::shared_ptr();

		Vehicle::GEARBOX_TYPE GT = ParseGearboxType( rsd.typeName );
		if( GT == Vehicle::GT_NOT_DEFINED )return Vehicle::GearboxParams::shared_ptr();

		int modes = ParseGearboxModes( rsd.modeNameContainer );
		if( modes == -1 )return Vehicle::GearboxParams::shared_ptr();

		Vehicle::GearboxParams::shared_ptr gP( new Vehicle::GearboxParams() );
		gP->nameMechanics = rsd.gearboxName;
		gP->dragLookup = dragLookup;
		gP->maxDrag = rsd.maxDrag;
		gP->flywheel = fP;
		gP->gearboxType = GT;
		gP->modes = modes;
		gP->mechShiftTime = rsd.mechShiftTime;
		gP->directRatios = rsd.directRatios;
		gP->reverseRatios = rsd.reverseRatios;

		AddGearboxParams( traits::GearboxParams_traits::container::value_type( gP->nameMechanics, gP ) );

		return gP;
	}
	//------------------------------------------------------------
	VPC::traits::DifferentialParams_traits::out VehicleParamsCollection::CreateDifferentialParams( const DifferentialParamsRSD& rsd )
	{
		Core::OgreRealVBV::shared_ptr dragLookup;
		if( !(rsd.dragLookupName.empty()) )
		{
			Core::SquaredLinearVBV< float, float >* newDragLookup = new Core::SquaredLinearVBV< float, float >();
			dragLookup = GetDragLookup( rsd.dragLookupName, newDragLookup );
			if( dragLookup == 0 )return Vehicle::DifferentialParams::shared_ptr();
		}

		int modes = ParseDifferentialModes( rsd.modeNameArray );
		if( modes == -1 )return Vehicle::DifferentialParams::shared_ptr();

		Vehicle::DifferentialParams::shared_ptr dP( new Vehicle::DifferentialParams() );
		dP->nameMechanics = rsd.diffName;
		dP->maxDrag = rsd.maxDrag;
		dP->dragLookup = dragLookup;
		dP->diffRatio = rsd.diffRatio;
		dP->firstNodeRatio = rsd.firstNodeRatio;
		dP->availableModes = modes;

		AddDifferentialParams( traits::DifferentialParams_traits::container::value_type( dP->nameMechanics, dP ) );

		return dP;
	}
	//------------------------------------------------------------
	VPC::traits::DifferentialParams_traits::out VehicleParamsCollection::CreateDifferentialParams( const DifferentialChainRSD::Node &node )
	{
		Vehicle::DifferentialParams::shared_ptr dP = GetDifferentialParams( node.name );
		if( dP == notFoundDifferentialParams )return notFoundDifferentialParams;
		if( node.leftNode != 0 || node.rightNode != 0 )
		{
			if( !(node.leftNode != 0 && node.rightNode != 0) )return notFoundDifferentialParams;

			VPC::traits::DifferentialParams_traits::out leftDP = GetDifferentialParams( node.leftNode->name );
			if( leftDP == notFoundDifferentialParams )return notFoundDifferentialParams;
			dP->leftDiffParams = leftDP;

			VPC::traits::DifferentialParams_traits::out rightDP = GetDifferentialParams( node.rightNode->name );
			if( rightDP == notFoundDifferentialParams )return notFoundDifferentialParams;
			dP->rightDiffParams = rightDP;
		}
		dP->vehicleAxleIndex = node.index;
		return dP;
	}
	//------------------------------------------------------------
	VPC::traits::BrakeBlockParams_traits::out VehicleParamsCollection::CreateBrakeBlockParams(const BrakeBlockParamsRSD &rsd)
	{
		Vehicle::BrakeBlockParams::shared_ptr bb( new Vehicle::BrakeBlockParams() );
		bb->name = rsd.name;
		bb->osnParams = SPC::getSingleton().GetVehicleSnp( rsd.osnName );
		bb->maxBrakeTorque = rsd.maxBrakeTorque;

		AddBrakeBlockParams( traits::BrakeBlockParams_traits::container::value_type( bb->name, bb ) );

		return bb;
	}
	//------------------------------------------------------------
	VPC::traits::ThrottleControlParams_traits::out VehicleParamsCollection::CreateThrottleControlParams(const ThrottleControlParamsRSD& rsd)
	{
		traits::ThrottleControlParams_traits::store_type tcp( new Vehicle::ThrottleControlParams() );
		tcp->name = rsd.name;
		tcp->tcsType = ParseTCSType( rsd.tcsTypeName );
		tcp->throttleFeed = rsd.throttleFeed;
		tcp->idleMark = rsd.idleMark;
		tcp->idleRange = rsd.idleRange;

		AddThrottleControlParams( traits::ThrottleControlParams_traits::container::value_type( tcp->name, tcp ) );

		return tcp;
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
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	void VehicleParamsCollection::AddMotorParams( const traits::MotorParams_traits::container::value_type& pair, bool replace )
	{
		if( pair.first.empty() || pair.second == 0 )return;
		if( replace ) motorParamsContainer.erase( pair.first );
		motorParamsContainer.insert( pair );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::AddGearboxParams( const traits::GearboxParams_traits::container::value_type& pair, bool replace )
	{
		if( pair.first.empty() || pair.second == 0 )return;
		if( replace ) gearboxParamsContainer.erase( pair.first );
		gearboxParamsContainer.insert( pair );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::AddDifferentialParams( const traits::DifferentialParams_traits::container::value_type& pair, bool replace )
	{
		if( pair.first.empty() || pair.second == 0 )return;
		if( replace ) differentialParamsContainer.erase( pair.first );
		differentialParamsContainer.insert( pair );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::AddFlywheelParams( const traits::FlywheelParams_traits::container::value_type &pair, bool replace )
	{
		if( pair.first.empty() || pair.second == 0 )return;
		if( replace ) flywheelParamsContainer.erase( pair.first );
		flywheelParamsContainer.insert( pair );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::AddBrakeBlockParams( const traits::BrakeBlockParams_traits::container::value_type& pair, bool replace /* = true */ )
	{
		if( pair.first.empty() || pair.second == 0 )return;
		if( replace ) brakeBlockParamsContainer.erase( pair.first );
		brakeBlockParamsContainer.insert( pair );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::AddMotorChartMarksPack( const traits::MotorChartMarksPack_traits::container::value_type& pair, bool replace /* = true */ )
	{
		if( pair.first.empty() || pair.second == 0 )return;
		if( replace ) motorChartMarksPackContainer.erase( pair.first );
		motorChartMarksPackContainer.insert( pair );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::AddThrottleControlParams( const traits::ThrottleControlParams_traits::container::value_type& pair, bool replace /* = true */ )
	{
		if( pair.first.empty() || pair.second == 0 )return;
		if( replace ) throttleControlParamsContainer.erase( pair.first );
		throttleControlParamsContainer.insert( pair );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::AddDriverTiming( const traits::DriverTiming_traits::container::value_type& pair, bool replace /* = true */ )
	{
		if( pair.first.empty() || pair.second == 0 )return;
		if( replace ) driverTimingContainer.erase( pair.first );
		driverTimingContainer.insert( pair );
	}
	//------------------------------------------------------------
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
	void VehicleParamsCollection::AddPhysXVehicleBodyParams( const traits::PhysXVehicleBodyParams_traits::container::value_type& pair, bool replace /* = true */ )
	{
		if( pair.first.empty() || pair.second == 0 )return;
		if( replace ) physxVehicleBodyParamsContainer.erase( pair.first );
		physxVehicleBodyParamsContainer.insert( pair );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::AddPhysXWheelParams( const traits::PhysXWheelParams_traits::container::value_type& pair, bool replace /* = true */ )
	{
		if( pair.first.empty() || pair.second == 0 )return;
		if( replace ) physxWheelParamsContainer.erase( pair.first );
		physxWheelParamsContainer.insert( pair );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::AddPhysXWheelNodeParams( const traits::PhysXWheelNodeParams_traits::container::value_type& pair, bool replace /* = true */ )
	{
		if( pair.first.empty() || pair.second == 0 )return;
		if( replace ) physxWheelNodeParamsContainer.erase( pair.first );
		physxWheelNodeParamsContainer.insert( pair );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::AddPhysXSuspensionDesc( const traits::PhysXSuspensionDesc_traits::container::value_type& pair, bool replace /* = true */ )
	{
		if( pair.first.empty() || pair.second == 0 )return;
		if( replace ) physxSuspensionDescContainer.erase( pair.first );
		physxSuspensionDescContainer.insert( pair );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::AddPhysXTireDesc( const traits::PhysXTireDesc_traits::container::value_type& pair, bool replace /* = true */ )
	{
		if( pair.first.empty() || pair.second == 0 )return;
		if( replace ) physxTireDescContainer.erase( pair.first );
		physxTireDescContainer.insert( pair );
	}
	//------------------------------------------------------------
#endif
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	//------------------------------------------------------------
	int VehicleParamsCollection::ParseDifferentialModes( DifferentialParamsRSD::traits::str_container_in modeNameArray, int defaultValue )
	{
		int mode(0);
		DifferentialParamsRSD::traits::str_container::const_iterator iter = modeNameArray.begin();
		DifferentialParamsRSD::traits::str_container::const_iterator end_iter = modeNameArray.end();
		for( ; iter != end_iter; ++iter )
		{
			Vehicle::DIFFERENTIAL_MODE DM = ParseDifferentialMode( *iter, Vehicle::DM_NOT_DEFINED );
			if( DM == Vehicle::DM_NOT_DEFINED )return defaultValue;
			if( (mode & DM) != 0 )return defaultValue;
			mode |= DM;
		}
		return mode;
	}
	//------------------------------------------------------------
	Vehicle::DIFFERENTIAL_MODE VehicleParamsCollection::ParseDifferentialMode(string_traits::in modeName, Vehicle::DIFFERENTIAL_MODE defaultMode )
	{
		typedef DifferentialKeys DK;
		if( modeName == DK::DM_LOCK )return Vehicle::DM_LOCK;
		if( modeName == DK::DM_DIS_FIRST )return Vehicle::DM_DIS_FIRST;
		if( modeName == DK::DM_DIS_SECOND )return Vehicle::DM_DIS_SECOND;
		return defaultMode;
	}
	//------------------------------------------------------------
	Vehicle::GEARBOX_TYPE VehicleParamsCollection::ParseGearboxType( string_traits::in gearboxType, Vehicle::GEARBOX_TYPE defaultType /*= Vehicle::GT_NOT_DEFINED */ )
	{
		typedef GearboxKeys GK;
		if( gearboxType == GK::GT_MECHANICAL )return Vehicle::GT_MECHANICAL;
		if( gearboxType == GK::GT_AUTOMATIC )return Vehicle::GT_AUTOMATIC;
		if( gearboxType == GK::GT_SEQUENTAL )return Vehicle::GT_SEQUENTAL;
		return defaultType;
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::ConvertGearboxModeToString( int gearboxModes, GearboxParamsRSD::traits::str_container_inout modeArray )
	{
		if(( gearboxModes & Vehicle::AGM_DIRECT ) != 0 ) modeArray.push_back( GearboxKeys::AGM_DIRECT );
		if(( gearboxModes & Vehicle::AGM_NEUTRAL ) != 0 ) modeArray.push_back( GearboxKeys::AGM_NEUTRAL );
		if(( gearboxModes & Vehicle::AGM_PARK ) != 0 ) modeArray.push_back( GearboxKeys::AGM_PARK );
		if(( gearboxModes & Vehicle::AGM_REVERSE ) != 0 ) modeArray.push_back( GearboxKeys::AGM_REVERSE );
		if(( gearboxModes & Vehicle::AGM_SEQ_EMU ) != 0 ) modeArray.push_back( GearboxKeys::AGM_SEQ_EMU );
		if(( gearboxModes & Vehicle::AGM_SPORT ) != 0 ) modeArray.push_back( GearboxKeys::AGM_SPORT );
	}
	//------------------------------------------------------------
	VPC::string_traits::str VehicleParamsCollection::ConvertGearboxTypeToString( Vehicle::GEARBOX_TYPE type )
	{
		if( type == Vehicle::GT_AUTOMATIC ) return GearboxKeys::GT_AUTOMATIC;
		if( type == Vehicle::GT_MECHANICAL ) return GearboxKeys::GT_MECHANICAL;
		if( type == Vehicle::GT_SEQUENTAL ) return GearboxKeys::GT_SEQUENTAL;
		return string_traits::str();
	}
	//------------------------------------------------------------
	int VehicleParamsCollection::ParseGearboxModes( GearboxParamsRSD::traits::str_container_in modeNameArray, int defaultValue )
	{
		int mode(0);
		GearboxParamsRSD::traits::str_container::const_iterator iter = modeNameArray.begin();
		GearboxParamsRSD::traits::str_container::const_iterator end_iter = modeNameArray.end();
		for( ; iter != end_iter; ++iter )
		{
			Vehicle::AUTO_GEARBOX_MODE AGM = ParseGearboxMode( *iter );
			if( AGM == Vehicle::AGM_NOT_DEFINED )return defaultValue;
			if( (mode & AGM) != 0 )return defaultValue;
			mode |= AGM;
		}
		return mode;
	}
	//------------------------------------------------------------
	Vehicle::AUTO_GEARBOX_MODE VehicleParamsCollection::ParseGearboxMode( string_traits::in modeName, Vehicle::AUTO_GEARBOX_MODE defaultMode )
	{
		typedef GearboxKeys GK;
		if( modeName == GK::AGM_DIRECT )return Vehicle::AGM_DIRECT;
		if( modeName == GK::AGM_NEUTRAL )return Vehicle::AGM_NEUTRAL;
		if( modeName == GK::AGM_PARK )return Vehicle::AGM_PARK;
		if( modeName == GK::AGM_REVERSE )return Vehicle::AGM_REVERSE;
		if( modeName == GK::AGM_SEQ_EMU )return Vehicle::AGM_SEQ_EMU;
		if( modeName == GK::AGM_SPORT )return Vehicle::AGM_SPORT;
		return defaultMode;
	}
	//------------------------------------------------------------
	Vehicle::DRIVE_OPTIONS VehicleParamsCollection::ParseDriveOptions( string_traits::in optionName )
	{
		typedef VehicleDriverKeys VDK;
		if( optionName == VDK::DO_THROTTLE_ASSIST )return Vehicle::DO_THROTTLE_ASSIST;
		if( optionName == VDK::DO_BRAKE_ASSIST )return Vehicle::DO_BRAKE_ASSIST;
		if( optionName == VDK::DO_CLUTCH_ASSIST )return Vehicle::DO_CLUTCH_ASSIST;
		if( optionName == VDK::DO_NEUTRAL_PREVENT )return Vehicle::DO_NEUTRAL_PREVENT;
		if( optionName == VDK::DO_REVERSE_PREVENT )return Vehicle::DO_REVERSE_PREVENT;
		if( optionName == VDK::DO_SHIFT_ASSIST )return Vehicle::DO_SHIFT_ASSIST;
		if( optionName == VDK::DO_STEER_ASSIST )return Vehicle::DO_STEER_ASSIST;
		if( optionName == VDK::DO_USER_CLUTCH )return Vehicle::DO_USER_CLUTCH;
		if( optionName == VDK::DO_USER_CONTROLS )return Vehicle::DO_USER_CONTROLS;
		return Vehicle::DO_NO_OPTIONS;
	}
	//------------------------------------------------------------
	Vehicle::THROTTLE_CONTROL_TYPE VehicleParamsCollection::ParseTCSType(string_traits::in tcsTypeName)
	{
		if( tcsTypeName == TCSKeys::TYPE_CONST_FEED )return Vehicle::TCS_CFEED;
		return Vehicle::TCS_NOT_DEFINED;
	}
	//------------------------------------------------------------
	Vehicle::MotorParams::TORQUE_MULT_FUNC VehicleParamsCollection::ParseMotorTorqueMultFunc( string_traits::in funcName, Vehicle::MotorParams::TORQUE_MULT_FUNC defaultFunc )
	{
		if( funcName == "const" )return Vehicle::MotorParams::TMF_CONST;
		else if( funcName == "linear" )return Vehicle::MotorParams::TMF_LINEAR;
		else return defaultFunc;
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
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadPhysXSuspensionDesc( tixmlel_traits::in element, bool replace )
	{
		typedef SuspensionKeys SK;
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::PhysXSuspensionDesc_traits::container::iterator iter = physxSuspensionDescContainer.find( name );
		if( iter != physxSuspensionDescContainer.end() )
		{
			if( !replace )return;
			physxSuspensionDescContainer.erase( iter );
		}

		traits::PhysXSuspensionDesc_traits::store_type sus( new Vehicle::PhysXSuspensionDesc );
		sus->name = name;
		sus->suspensionTravel = XI::GetReal( element, SK::TRAVEL );
		sus->suspension.damper = XI::GetReal( element, SK::DAMPER );
		sus->suspension.spring = XI::GetReal( element, SK::SPRING );
		sus->suspension.targetValue = XI::GetReal( element, SK::TARGET_VALUE );

		physxSuspensionDescContainer.insert( traits::PhysXSuspensionDesc_traits::container::value_type( name, sus ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadPhysXTireDesc( tixmlel_traits::in element, bool replace )
	{
		typedef TireKeys TK;
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::PhysXTireDesc_traits::container::iterator iter = physxTireDescContainer.find( name );
		if( iter != physxTireDescContainer.end() )
		{
			if( !replace )return;
			physxTireDescContainer.erase( iter );
		}

		traits::PhysXTireDesc_traits::store_type tire( new Vehicle::PhysXTireDesc );
		tire->name = name;

		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == TK::LONG_FUNC )
			{
				LoadPhysXTireFunction( childElement, tire->longFunction );
			}
			else if( childName == TK::LAT_FUNC )
			{
				LoadPhysXTireFunction( childElement, tire->latFunction );
			}
		}

		physxTireDescContainer.insert( traits::PhysXTireDesc_traits::container::value_type( name, tire ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadPhysXTireFunction( tixmlel_traits::in element, NxTireFunctionDesc &outFunc )
	{
		typedef TireKeys TK;
		outFunc.stiffnessFactor = XI::GetReal( element, TK::STIFFNESS_FACTOR );
		outFunc.asymptoteSlip = XI::GetReal( element, TK::ASYMPTOTE_SLIP );
		outFunc.asymptoteValue = XI::GetReal( element, TK::ASYMPTOTE_VALUE );
		outFunc.extremumSlip = XI::GetReal( element, TK::EXTREMUM_SLIP );
		outFunc.extremumValue = XI::GetReal( element, TK::EXTREMUM_VALUE );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadPhysXWheelParamsRSD( tixmlel_traits::in element, bool replace )
	{
		typedef WheelKeys WK;
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::PhysXWheelParamsRSD_traits::container::iterator iter = physxWheelParamsRsdContainer.find( name );
		if( iter != physxWheelParamsRsdContainer.end() )
		{
			if( !replace )return;
			physxWheelParamsRsdContainer.erase( iter );
		}

		traits::PhysXWheelParamsRSD_traits::store_type wheel( new PhysXWheelParamsRSD );
		wheel->name = name;
		wheel->osnName = XI::GetString( element, MK::OSN_NAME );
		wheel->tireName = XI::GetString( element, WK::TIRE_NAME );
		wheel->mass = XI::GetReal( element, MK::MASS );
		wheel->rimRadius = XI::GetReal( element, WK::RIM_RADIUS );
		wheel->tireIncRadius = XI::GetReal( element, WK::TIRE_INC_RADIUS );
		wheel->tireWidth = XI::GetReal( element, WK::TIRE_WIDTH );

		physxWheelParamsRsdContainer.insert( traits::PhysXWheelParamsRSD_traits::container::value_type( name, wheel ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadPhysXWheelNodeParamsRSD( tixmlel_traits::in element, bool replace )
	{
		typedef WheelNodeKeys WNK;
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::PhysXWheelNodeParamsRSD_traits::container::iterator iter = physxWheelNodeParamsRsdContainer.find( name );
		if( iter != physxWheelNodeParamsRsdContainer.end() )
		{
			if( !replace )return;
			physxWheelNodeParamsRsdContainer.erase( iter );
		}

		traits::PhysXWheelNodeParamsRSD_traits::store_type wheelNode( new PhysXWheelNodeParamsRSD );
		wheelNode->name = name;
		wheelNode->osnName = XI::GetString( element, MK::OSN_NAME );
		wheelNode->susName = XI::GetString( element, WNK::SUS_NAME );
		wheelNode->brakeBlockName = XI::GetString( element, WNK::BRAKE_BLOCK_NAME );
		wheelNode->rubberWheelName = XI::GetString( element, WNK::RUBBER_WHEEL_NAME );
		wheelNode->ironWheelName = XI::GetString( element, WNK::IRON_WHEEL_NAME );
		wheelNode->dragLookupName = XI::GetString( element, MK::DRAG_LOOKUP );
		wheelNode->maxDrag = XI::GetReal( element, MK::MAX_DRAG );
		wheelNode->ironIncShift = XI::GetReal( element, WNK::IRON_INC_SHIFT );
		wheelNode->rubberIncShift = XI::GetReal( element, WNK::RUBBER_INC_SHIFT );
		
		physxWheelNodeParamsRsdContainer.insert( traits::PhysXWheelNodeParamsRSD_traits::container::value_type( name, wheelNode ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadPhysXVehicleBodyParamsRSD(tixmlel_traits::in element, bool replace )
	{
		typedef VehicleBodyKeys VK;
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;
		traits::PhysXVehicleBodyParamsRSD_traits::container::iterator iter = physxVehicleBodyParamsRsdContainer.find( name );
		if( iter != physxVehicleBodyParamsRsdContainer.end() )
		{
			if( !replace )return;
			physxVehicleBodyParamsRsdContainer.erase( iter );
		}

		traits::PhysXVehicleBodyParamsRSD_traits::store_type rsd( new PhysXVehicleBodyParamsRSD );
		rsd->setName = name;
		rsd->bodyName = XI::GetString( element, VK::BODY_NAME );
		rsd->mass = XI::GetReal( element, MK::MASS );
		rsd->boundMeshName = XI::GetString( element, VK::BOUND_MESH_NAME );
		rsd->osnName = XI::GetString( element, MK::OSN_NAME );
		rsd->motorName = XI::GetString( element, VK::MOTOR_NAME );
		rsd->gearboxName = XI::GetString( element, VK::GEARBOX_NAME );
		rsd->diffName = XI::GetString( element, VK::DIFF_NAME );
		rsd->tcsName = XI::GetString( element, VK::TCS_NAME );
		rsd->physxMatName = XI::GetString( element, VK::PHYSX_MAT_NAME );
		rsd->solverCount = XI::GetUInt( element, VK::SOLVER_COUNT );
		rsd->addDownForce = XI::GetReal( element, VK::ADD_DOWN_FORCE );
		
		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == MK::MASS_OFFSET )
			{
				XI::GetVector3( childElement, rsd->massOffset, Ogre::Vector3::ZERO );
			}
			else if( childName == VK::BOUND_BOX )
			{
				XI::GetVector3( childElement, rsd->boundBox, Ogre::Vector3::ZERO );
			}
			else if( childName == VK::SPEEDOMETER )
			{
				rsd->speedometer.revSensorAxis = XI::GetUInt( element, VK::SPDM_REV_AXIS );
				rsd->speedometer.revSensorSide = XI::GetUInt( element, VK::SPDM_REV_SIDE );
				rsd->speedometer.wheelRadius = XI::GetReal( element, VK::SPDM_WHEEL_RADIUS );
			}
			else if( childName == MK::AXLE )
			{
				PhysXVehicleBodyParamsRSD::AxleParamsRSD axle;
				LoadPhysXVehicleBodyParamsRSDAxle( childElement, axle );
				rsd->axleArray.push_back( axle );
			}
		}

		physxVehicleBodyParamsRsdContainer.insert( traits::PhysXVehicleBodyParamsRSD_traits::container::value_type( name, rsd ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadPhysXVehicleBodyParamsRSDAxle(tixmlel_traits::in element, PhysXVehicleBodyParamsRSD::AxleParamsRSD& axle )
	{
		typedef VehicleBodyKeys VK;
		axle.index = XI::GetUInt( element, MK::INDEX );
		axle.maxSteerAngle = XI::GetReal( element, VK::MAX_STEER_ANGLE );
		axle.leftWheelNode = XI::GetString( element, VK::LEFT_WHEEL_NODE );
		axle.rightWheelNode = XI::GetString( element, VK::RIGHT_WHEEL_NODE );
		axle.brakeMaxPressValue = XI::GetReal( element, VK::BRAKE_MAX_PRESS_VALUE );

		const TiXmlElement* childElement = element->FirstChildElement( MK::POSITION );
		if( childElement )
			XI::GetVector3( childElement, axle.position, Ogre::Vector3::ZERO );
	}
	//------------------------------------------------------------
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadMotorParamsRSD( tixmlel_traits::in element, bool replace )
	{
		typedef MotorKeys MotK;
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::MotorParamsRSD_traits::container::iterator iter = motorParamsRsdContainer.find( name );
		if( iter != motorParamsRsdContainer.end() )
		{
			if( !replace )return;
			motorParamsRsdContainer.erase( iter );
		}

		traits::MotorParamsRSD_traits::store_type m( new MotorParamsRSD );
		m->motorName = name;
		m->maxTorque = XI::GetReal( element, MotK::MAX_TORQUE );
		m->maxStarterTorque = XI::GetReal( element, MotK::MAX_STARTER_TORQUE );
		m->maxDrag = XI::GetReal( element, MK::MAX_DRAG );
		m->torqueLookupName = XI::GetString( element, MotK::TORQUE_LOOKUP );
		m->dragLookupName = XI::GetString( element, MK::DRAG_LOOKUP );
		m->flywheelParamsName = XI::GetString( element, MK::FLYWHEEL_PARAMS );
		m->stallRPM = XI::GetReal( element, MotK::STALL_RPM );
		m->idleRPM = XI::GetReal( element, MotK::IDLE_RPM );
		m->startRPM = XI::GetReal( element, MotK::START_RPM );
		m->redRPM = XI::GetReal( element, MotK::RED_RPM );
		m->maxRPM = XI::GetReal( element, MotK::MAX_RPM );
		m->calmControlDataName = XI::GetString( element, MotK::CALM_RPM_CONTROL_DATA );
		m->agrControlDataName = XI::GetString( element, MotK::AGR_RPM_CONTROL_DATA );
		m->belowIdleTorqueMult = XI::GetReal( element, MotK::TORQUE_MULT );
		m->belowIdleTorqueMultFunc = XI::GetString( element, MotK::TORQUE_MULT_FUNC );

		motorParamsRsdContainer.insert( traits::MotorParamsRSD_traits::container::value_type( name, m ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadGearboxParamsRSD( tixmlel_traits::in element, bool replace )
	{
		typedef GearboxKeys GK;

		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::GearboxParamsRSD_traits::container::iterator iter = gearboxParamsRsdContainer.find( name );
		if( iter != gearboxParamsRsdContainer.end() )
		{
			if( !replace )return;
			gearboxParamsRsdContainer.erase( iter );
		}

		traits::GearboxParamsRSD_traits::store_type g( new GearboxParamsRSD );
		g->gearboxName = name;
		g->dragLookupName = XI::GetString( element, MK::DRAG_LOOKUP );
		g->flywheelParamsName = XI::GetString( element, MK::FLYWHEEL_PARAMS );
		g->maxDrag = XI::GetReal( element, MK::MAX_DRAG );
		g->typeName = XI::GetString( element, MK::TYPE );
		g->mechShiftTime = XI::GetReal( element, GK::MECH_SHIFT_TIME );

		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == MK::MODES )
			{
				LoadGearboxModes( childElement, g->modeNameContainer );
			}
			else if( childName == GK::DIRECT_RATIOS )
			{
				LoadGearboxRatios( childElement, g->directRatios );
			}
			else if( childName == GK::REVERSE_RATIOS )
			{
				LoadGearboxRatios( childElement, g->reverseRatios );
			}
		}

		gearboxParamsRsdContainer.insert( traits::GearboxParamsRSD_traits::container::value_type( name, g ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadGearboxRatios(tixmlel_traits::in element, GearboxParamsRSD::traits::real_container &ratios)
	{
		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == MK::RATIO )
			{
				ratios.push_back( XI::GetReal( childElement, MK::VALUE ) );
			}
		}
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadGearboxModes(tixmlel_traits::in element, GearboxParamsRSD::traits::str_container &modes)
	{
		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == MK::MODE )
			{
				modes.push_back( XI::GetString( element, MK::NAME ) );
			}
		}
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadFlywheelParamsRSD( tixmlel_traits::in element, bool replace )
	{
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::FlywheelParamsRSD_traits::container::iterator iter = flywheelParamsRsdContainer.find( name );
		if( iter != flywheelParamsRsdContainer.end() )
		{
			if( !replace )return;
			flywheelParamsRsdContainer.erase( iter );
		}

		traits::FlywheelParamsRSD_traits::store_type f( new FlywheelParamsRSD );
		f->name = name;
		f->dragLookupName = XI::GetString( element, MK::DRAG_LOOKUP );
		f->maxDrag = XI::GetReal( element, MK::MAX_DRAG );
		f->mass = XI::GetReal( element, MK::MASS );
		f->radius = XI::GetReal( element, MK::RADIUS );

		flywheelParamsRsdContainer.insert( traits::FlywheelParamsRSD_traits::container::value_type( name, f ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadDifferentialParamsRSD( tixmlel_traits::in element, bool replace )
	{
		typedef DifferentialKeys DK;
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::DifferentialParamsRSD_traits::container::iterator iter = differentialParamsRsdContainer.find( name );
		if( iter != differentialParamsRsdContainer.end() )
		{
			if( !replace )return;
			differentialParamsRsdContainer.erase( iter );
		}

		traits::DifferentialParamsRSD_traits::store_type d( new DifferentialParamsRSD );
		d->diffName = name;
		d->diffRatio = XI::GetReal( element, DK::DIFF_RATIO );
		d->dragLookupName = XI::GetString( element, MK::DRAG_LOOKUP );
		d->maxDrag = XI::GetReal( element, MK::MAX_DRAG );
		d->firstNodeRatio = XI::GetReal( element, DK::FIRST_NODE_RATIO );
		
		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == MK::MODES )
			{
				LoadDifferentialModes( childElement, d->modeNameArray );
			}
		}

		differentialParamsRsdContainer.insert( traits::DifferentialParamsRSD_traits::container::value_type( name, d ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadDifferentialModes( tixmlel_traits::in element, DifferentialParamsRSD::traits::str_container &modes )
	{
		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == MK::MODE )
			{
				modes.push_back( XI::GetString( childElement, MK::NAME ) );
			}
		}
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadVBVRSD( tixmlel_traits::in element, bool replace )
	{
		typedef VBVKeys V;
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::VBVRSD_traits::container::iterator iter = vbvRsdContainer.find( name );
		if( iter != vbvRsdContainer.end() )
		{
			if( !replace )return;
			vbvRsdContainer.erase( iter );
		}

		traits::VBVRSD_traits::store_type vbv( new VBVRSD );
		vbv->name = name;
		vbv->incValue = XI::GetReal( element, V::INC_VALUE );
		vbv->startValue = XI::GetReal( element, V::START_VALUE );
		vbv->multValue = XI::GetReal( element, V::MULT_VALUE );
		
		const TiXmlElement* entriesElement = element->FirstChildElement( MK::ENTRIES );
		if( entriesElement )
			LoadVBVEntries( entriesElement, vbv->entries );

		vbvRsdContainer.insert( traits::VBVRSD_traits::container::value_type( name, vbv ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadVBVEntries( tixmlel_traits::in element, VBVRSD::traits::real_container &entries )
	{
		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == MK::ENTRY )
			{
				entries.push_back( XI::GetReal( childElement, MK::VALUE ) );
			}
		}
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadDifferentialChainNode(tixmlel_traits::in element, DifferentialChainRSD::Node &node)
	{
		node.name = XI::GetString( element, MK::NAME );
		node.index = XI::GetUInt( element, MK::INDEX );

		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == MK::NODE )
			{
				if( node.leftNode == 0 )
				{
					node.leftNode.bind( new DifferentialChainRSD::Node() );
					LoadDifferentialChainNode( childElement, *node.leftNode );
				}
				else if( node.rightNode == 0 )
				{
					node.rightNode.bind( new DifferentialChainRSD::Node() );
					LoadDifferentialChainNode( childElement, *node.rightNode );
				}
			}
		}
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadDifferentialChainRSD(tixmlel_traits::in element, bool replace )
	{
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::DifferentialChainRSD_traits::container::iterator iter = differentialChainRsdContainer.find( name );
		if( iter != differentialChainRsdContainer.end() )
		{
			if( !replace )return;
			differentialChainRsdContainer.erase( iter );
		}

		traits::DifferentialChainRSD_traits::store_type rsd( new DifferentialChainRSD );
		rsd->name = name;

		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == MK::NODE && rsd->firstNode == 0 )
			{
				rsd->firstNode.bind( new DifferentialChainRSD::Node() );
				LoadDifferentialChainNode( childElement, *rsd->firstNode );
			}
		}

		differentialChainRsdContainer.insert( traits::DifferentialChainRSD_traits::container::value_type( name, rsd ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadBrakeBlockParamsRSD( tixmlel_traits::in element, bool replace )
	{
		typedef BrakeBlockKeys BB;
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::BrakeBlockParamsRSD_traits::container::iterator iter = brakeBlockParamsRsdContainer.find( name );
		if( iter != brakeBlockParamsRsdContainer.end() )
		{
			if( !replace )return;
			brakeBlockParamsRsdContainer.erase( iter );
		}

		traits::BrakeBlockParamsRSD_traits::store_type rsd( new BrakeBlockParamsRSD );
		rsd->name = name;
		rsd->osnName = XI::GetString( element, MK::OSN_NAME );
		rsd->maxBrakeTorque = XI::GetReal( element, BB::MAX_BRAKE_TORQUE );

		brakeBlockParamsRsdContainer.insert( traits::BrakeBlockParamsRSD_traits::container::value_type( name, rsd ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::AddParamsFrom(tixmlel_traits::in element, int loadOptions)
	{
		bool replace = (loadOptions & LO_REPLACE_DUPS) != 0;
		string_traits::str childName;

		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();

			if( childName == MK::BRAKE_BLOCK && (loadOptions & LO_SKIP_BRAKE) == 0 )
			{
				LoadBrakeBlockParamsRSD( childElement, replace );
			}
			else if( childName == MK::MOTOR_PARAMS && (loadOptions & LO_SKIP_MOTOR) == 0 )
			{
				LoadMotorParamsRSD( childElement, replace );
			}
			else if( childName == MK::GEARBOX_PARAMS && (loadOptions & LO_SKIP_GEARBOX) == 0 )
			{
				LoadGearboxParamsRSD( childElement, replace );
			}
			else if( childName == MK::DIFFERENTIAL_PARAMS && (loadOptions & LO_SKIP_DIFF) == 0 )
			{
				LoadDifferentialParamsRSD( childElement, replace );
			}
			else if( childName == MK::DIFFERENTIAL_CHAIN && (loadOptions & LO_SKIP_DIFF) == 0 )
			{
				LoadDifferentialChainRSD( childElement, replace );
			}
			else if( childName == MK::FLYWHEEL_PARAMS && (loadOptions & LO_SKIP_FLYWHEEL) == 0 )
			{
				LoadFlywheelParamsRSD( childElement, replace );
			}
			else if( childName == MK::VBV )
			{
				LoadVBVRSD( childElement, replace );
			}
			else if( childName == MK::MCM_PACK )
			{
				LoadMCMPack( childElement, replace );
			}
			else if( childName == MK::DRIVER_TIMING )
			{
				LoadDriverTiming( childElement, replace );
			}
			else if( childName == MK::DRIVER )
			{
				LoadDriver( childElement );
			}
			else if( childName == MK::TCS )
			{
				LoadThrottleControlParamsRSD( childElement, replace );
			}
#		if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
			else if( childName == MK::PHYSX_SUSPENSION_DESC && (loadOptions & LO_SKIP_SUSPENSION) == 0 )
			{
				LoadPhysXSuspensionDesc( childElement, replace );
			}
			else if( childName == MK::PHYSX_TIRE_DESC && (loadOptions & LO_SKIP_TIRE) == 0 )
			{
				LoadPhysXTireDesc( childElement, replace );
			}
			else if( childName == MK::PHYSX_WHEEL && (loadOptions & LO_SKIP_WHEEL) == 0 )
			{
				LoadPhysXWheelParamsRSD( childElement, replace );
			}
			else if( childName == MK::PHYSX_WHEEL_NODE && (loadOptions & LO_SKIP_WHEEL) == 0 )
			{
				LoadPhysXWheelNodeParamsRSD( childElement, replace );
			}
			else if( childName == MK::PHYSX_VEHICLE_BODY && (loadOptions & LO_SKIP_BODY) == 0 )
			{
				LoadPhysXVehicleBodyParamsRSD( childElement, replace );
			}
#		endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		}
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadDriverTiming(tixmlel_traits::in element, bool replace)
	{
		typedef DriverTimingKeys DT;
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::DriverTiming_traits::container::iterator iter = driverTimingContainer.find( name );
		if( iter != driverTimingContainer.end() )
		{
			if( !replace )return;
			driverTimingContainer.erase( iter );
		}

		traits::DriverTiming_traits::store_type dt( new Vehicle::DriverTiming );
		dt->throttlePressTime = XI::GetReal( element, DT::THROTTLE_PRESS );
		dt->throttleReleaseTime = XI::GetReal( element, DT::THROTTLE_RELEASE );
		dt->brakePressTime = XI::GetReal( element, DT::BRAKE_PRESS );
		dt->brakeReleaseTime = XI::GetReal( element, DT::BRAKE_RELEASE );
		dt->clutchPressTime = XI::GetReal( element, DT::CLUTCH_PRESS );
		dt->clutchReleaseTime = XI::GetReal( element, DT::CLUTCH_RELEASE );
		dt->handbrakePressTime = XI::GetReal( element, DT::HANDBRAKE_PRESS );
		dt->handbrakeReleaseTime = XI::GetReal( element, DT::HANDBRAKE_RELEASE );
		dt->steerPressTime = XI::GetReal( element, DT::STEER_PRESS );
		dt->steerReleaseTime = XI::GetReal( element, DT::STEER_RELEASE );
		dt->shiftDelayTime = XI::GetReal( element, DT::SHIFT_DELAY );
		dt->shiftToDirectTime = XI::GetReal( element, DT::SHIFT_TO_DIRECT );
		dt->shiftToNeutralTime = XI::GetReal( element, DT::SHIFT_TO_NEUTRAL );
		dt->shiftToReverseTime = XI::GetReal( element, DT::SHIFT_TO_REVERSE );
		dt->switchModeTime = XI::GetReal( element, DT::SWITCH_MODE );
		dt->switchSeqTime = XI::GetReal( element, DT::SWITCH_SEQ );
		dt->rpmSpeedReactionTime = XI::GetReal( element, DT::RPM_SPEED_REACTION );
		dt->rpmZoneReactionTime = XI::GetReal( element, DT::RPM_ZONE_REACTION );
		dt->name = name;

		driverTimingContainer.insert( traits::DriverTiming_traits::container::value_type( name, dt ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadDriver( tixmlel_traits::in element )
	{
		typedef VehicleDriverKeys VDK;
		vehicleDriverRsd = VehicleDriverRSD();
		vehicleDriverRsd.agrMCMPName = XI::GetString( element, VDK::AGR_MCMP );
		vehicleDriverRsd.agrTimingName = XI::GetString( element, VDK::AGR_TIMING );
		vehicleDriverRsd.calmMCMPName = XI::GetString( element, VDK::CALM_MCMP );
		vehicleDriverRsd.calmTimingName = XI::GetString( element, VDK::CALM_TIMING );

		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == VDK::DRIVE_OPTIONS )
			{
				LoadDriveOptions( childElement );
			}
		}
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadDriveOptions( tixmlel_traits::in element )
	{
		typedef VehicleDriverKeys VDK;

		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == MK::OPTION )
			{
				string_traits::str optionName = XI::GetString( childElement, MK::NAME );
				if( !optionName.empty() && XI::GetBool( childElement, MK::VALUE ) )
				{
					Vehicle::DRIVE_OPTIONS dopts = ParseDriveOptions( optionName );
					vehicleDriverRsd.driveOptions |= dopts;
				}
			}
		}
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadMCMPack( tixmlel_traits::in element, bool replace )
	{
		typedef MotorChartMarksPackKeys MCMK;
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::MotorChartMarksPack_traits::container::iterator iter = motorChartMarksPackContainer.find( name );
		if( iter != motorChartMarksPackContainer.end() )
		{
			if( !replace )return;
			motorChartMarksPackContainer.erase( iter );
		}

		traits::MotorChartMarksPack_traits::store_type mcmp( new Vehicle::MotorChartMarksPack(name) );
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			if( childElement->ValueStr() == MCMK::MARKS )
			{
				Vehicle::MotorChartMarks::shared_ptr marks( new Vehicle::MotorChartMarks() );
				LoadMCM( childElement, *marks );
				mcmp->AddChartMarks( marks, replace );
			}
		}

		motorChartMarksPackContainer.insert( traits::MotorChartMarksPack_traits::container::value_type( name, mcmp ) );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadMCM(tixmlel_traits::in element, Vehicle::MotorChartMarks &marks)
	{
		typedef MotorChartMarksPackKeys MCMK;
		marks.gearIndex = XI::GetInt( element, MCMK::GEAR );
		marks.min = XI::GetReal( element, MCMK::MIN );
		marks.normLow = XI::GetReal( element, MCMK::NORM_LOW );
		marks.normHigh = XI::GetReal( element, MCMK::NORM_HIGH );
		marks.max = XI::GetReal( element, MCMK::MAX );
	}
	//------------------------------------------------------------
	void VehicleParamsCollection::LoadThrottleControlParamsRSD(tixmlel_traits::in element, bool replace)
	{
		string_traits::str name = XI::GetString( element, MK::NAME );
		if( name.empty() )return;

		traits::ThrottleControlParamsRSD_traits::container::iterator iter = throttleControlParamsRsdContainer.find( name );
		if( iter != throttleControlParamsRsdContainer.end() )
		{
			if( !replace )return;
			throttleControlParamsRsdContainer.erase( iter );
		}

		traits::ThrottleControlParamsRSD_traits::store_type rsd( new ThrottleControlParamsRSD );
		rsd->name = name;
		rsd->throttleFeed = XI::GetReal( element, TCSKeys::FEED_VALUE );
		rsd->idleMark = XI::GetReal( element, TCSKeys::IDLE_MARK );
		rsd->idleRange = XI::GetReal( element, TCSKeys::IDLE_RANGE );
		rsd->tcsTypeName = XI::GetString( element, MK::TYPE );

		throttleControlParamsRsdContainer.insert( traits::ThrottleControlParamsRSD_traits::container::value_type( name, rsd ) );
	}
	//------------------------------------------------------------
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML
}
}
#endif
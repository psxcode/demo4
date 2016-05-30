#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS

#include "dsVehicleTypes.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	dsDifferentialChain::Node::Node()
		: index( -1 )
	{
	}
	//------------------------------------------------------------
	dsDifferentialChain::Node::Node( const Node &n )
		: index( -1 )
	{
		*this = n;
	}
	//------------------------------------------------------------
	dsDifferentialChain::Node& dsDifferentialChain::Node::operator=( const Node &n )
	{
		if( this == &n )
			return *this;

		leftNode.setNull();
		rightNode.setNull();

		if( n.leftNode != 0 )leftNode.bind( new Node( *n.leftNode ) );
		if( n.rightNode != 0 )rightNode.bind( new Node( *n.rightNode ) );
		index = n.index;
		name = n.name;

		return *this;
	}
	//------------------------------------------------------------
	dsDifferentialChain::Node::~Node()
	{
	}
	//------------------------------------------------------------
	bool dsDifferentialChain::Node::isValid()const
	{
		//если присутствует хоть один присоединенный node
		if( leftNode != 0 || rightNode != 0 )
		{
			//должны присутствовать оба нода сразу
			if( leftNode == 0 || rightNode == 0 )return false;
		}
		return index >= 0;
	}
	//------------------------------------------------------------
	dsDifferentialChain::dsDifferentialChain()
	{
	}
	//------------------------------------------------------------
	dsDifferentialChain::dsDifferentialChain( const dsDifferentialChain &dc )
	{
		*this = dc;
	}
	//------------------------------------------------------------
	dsDifferentialChain& dsDifferentialChain::operator =( const dsDifferentialChain &dc )
	{
		if( this == &dc )
			return *this;

		firstNode.setNull();

		if( dc.firstNode != 0 )firstNode.bind( new Node( *dc.firstNode ) );
		name = dc.name;

		return *this;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
namespace dsKeys
{
	//------------------------------------------------------------
	string_traits::cstr VehicleCommon::BRAKE_BLOCK = "brakeblock";
	string_traits::cstr VehicleCommon::DIFFERENTIAL_CHAIN = "diffchain";
	string_traits::cstr VehicleCommon::DIFFERENTIAL_PARAMS = "diffparams";
	string_traits::cstr VehicleCommon::GEARBOX_PARAMS = "gearboxparams";
	string_traits::cstr VehicleCommon::MOTOR_PARAMS = "motorparams";
	string_traits::cstr VehicleCommon::VBV = "vbv";
	string_traits::cstr VehicleCommon::VEHICLE_PARAMS = "vehicleparams";
	string_traits::cstr VehicleCommon::NAME = "name";
	string_traits::cstr VehicleCommon::OSN_NAME = "osnname";
	string_traits::cstr VehicleCommon::VALUE = "value";
	string_traits::cstr VehicleCommon::RATIO = "ratio";
	string_traits::cstr VehicleCommon::MODE = "mode";
	string_traits::cstr VehicleCommon::MODES = "modes";
	string_traits::cstr VehicleCommon::NODE = "node";
	string_traits::cstr VehicleCommon::MAX_DRAG = "maxdrag";
	string_traits::cstr VehicleCommon::DRAG_LOOKUP = "draglookup";
	string_traits::cstr VehicleCommon::TYPE = "type";
	string_traits::cstr VehicleCommon::FLYWHEEL_PARAMS = "flywheelparams";
	string_traits::cstr VehicleCommon::MASS = "mass";
	string_traits::cstr VehicleCommon::MASS_OFFSET = "massoffset";
	string_traits::cstr VehicleCommon::MCM_PACK = "motorchartmarkspack";
	string_traits::cstr VehicleCommon::POSITION = "position";
	string_traits::cstr VehicleCommon::DRIVER_TIMING = "drivertiming";
	string_traits::cstr VehicleCommon::DRIVER = "driver";
	string_traits::cstr VehicleCommon::ENTRY = "entry";
	string_traits::cstr VehicleCommon::ENTRIES = "entries";
	string_traits::cstr VehicleCommon::AXLE = "axle";
	string_traits::cstr VehicleCommon::INDEX = "index";
	string_traits::cstr VehicleCommon::RADIUS = "radius";
	string_traits::cstr VehicleCommon::OPTION = "option";
	string_traits::cstr VehicleCommon::TCS = "tcs";
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	string_traits::cstr VehicleCommon::PHYSX_SUSPENSION_DESC = "physxsusdesc";
	string_traits::cstr VehicleCommon::PHYSX_TIRE_DESC = "physxtiredesc";
	string_traits::cstr VehicleCommon::PHYSX_VEHICLE_BODY = "physxvehiclebody";
	string_traits::cstr VehicleCommon::PHYSX_WHEEL = "physxwheel";
	string_traits::cstr VehicleCommon::PHYSX_WHEEL_NODE = "physxwheelnode";
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
	string_traits::cstr Motor::IDLE_RPM = "idlerpm";
	string_traits::cstr Motor::MAX_RPM = "maxrpm";
	string_traits::cstr Motor::MAX_TORQUE = "maxtorque";
	string_traits::cstr Motor::MAX_STARTER_TORQUE = "maxstartertorque";
	string_traits::cstr Motor::RED_RPM = "redrpm";
	string_traits::cstr Motor::STALL_RPM = "stallrpm";
	string_traits::cstr Motor::START_RPM = "startrpm";
	string_traits::cstr Motor::TORQUE_LOOKUP = "torquelookup";
	string_traits::cstr Motor::AGR_RPM_CONTROL_DATA = "agrrpmcontroldata";
	string_traits::cstr Motor::CALM_RPM_CONTROL_DATA = "calmrpmcontroldata";
	string_traits::cstr Motor::TORQUE_MULT_FUNC = "belowidletorquemultfunc";
	string_traits::cstr Motor::TORQUE_MULT = "belowidletorquemult";
	//------------------------------------------------------------
	string_traits::cstr Gearbox::GT_MECHANICAL = "mech";
	string_traits::cstr Gearbox::GT_AUTOMATIC = "auto";
	string_traits::cstr Gearbox::GT_SEQUENTAL = "seq";
	string_traits::cstr Gearbox::AGM_DIRECT = "direct";
	string_traits::cstr Gearbox::AGM_NEUTRAL = "neutral";
	string_traits::cstr Gearbox::AGM_PARK = "park";
	string_traits::cstr Gearbox::AGM_REVERSE = "reverse";
	string_traits::cstr Gearbox::AGM_SEQ_EMU = "seqemu";
	string_traits::cstr Gearbox::AGM_SPORT = "sport";
	string_traits::cstr Gearbox::MECH_SHIFT_TIME = "mechshifttime";
	string_traits::cstr Gearbox::DIRECT_RATIOS = "directratios";
	string_traits::cstr Gearbox::REVERSE_RATIOS = "reverseratios";
	//------------------------------------------------------------
	string_traits::cstr Differential::DM_LOCK = "lock";
	string_traits::cstr Differential::DM_DIS_FIRST = "disfirst";
	string_traits::cstr Differential::DM_DIS_SECOND = "dissecond";
	string_traits::cstr Differential::DIFF_RATIO = "diffratio";
	string_traits::cstr Differential::FIRST_NODE_RATIO = "firstnoderatio";
	//------------------------------------------------------------
	string_traits::cstr BrakeBlock::MAX_BRAKE_TORQUE = "maxbraketorque";
	//------------------------------------------------------------
	string_traits::cstr TCS::FEED_VALUE = "feedvalue";
	string_traits::cstr TCS::IDLE_MARK = "idlemark";
	string_traits::cstr TCS::IDLE_RANGE = "idlerange";
	string_traits::cstr TCS::TYPE_CONST_FEED = "cfeed";
	//------------------------------------------------------------
	string_traits::cstr VBV::MULT_VALUE = "multvalue";
	string_traits::cstr VBV::START_VALUE = "startvalue";
	string_traits::cstr VBV::INC_VALUE = "incvalue";
	//------------------------------------------------------------
	string_traits::cstr DriverTiming::BRAKE_PRESS = "brakepress";
	string_traits::cstr DriverTiming::BRAKE_RELEASE = "brakerelease";
	string_traits::cstr DriverTiming::CLUTCH_PRESS = "clutchpress";
	string_traits::cstr DriverTiming::CLUTCH_RELEASE = "clutchrelease";
	string_traits::cstr DriverTiming::HANDBRAKE_PRESS = "handbrakepress";
	string_traits::cstr DriverTiming::HANDBRAKE_RELEASE = "handbrakerelease";
	string_traits::cstr DriverTiming::RPM_SPEED_REACTION = "rpmspeedreaction";
	string_traits::cstr DriverTiming::RPM_ZONE_REACTION = "rpmzonereaction";
	string_traits::cstr DriverTiming::SHIFT_DELAY = "shiftdelay";
	string_traits::cstr DriverTiming::SHIFT_TO_DIRECT = "shifttodirect";
	string_traits::cstr DriverTiming::SHIFT_TO_NEUTRAL = "shifttoneutral";
	string_traits::cstr DriverTiming::SHIFT_TO_REVERSE = "shifttoreverse";
	string_traits::cstr DriverTiming::STEER_PRESS = "steerpress";
	string_traits::cstr DriverTiming::STEER_RELEASE = "steerrelease";
	string_traits::cstr DriverTiming::SWITCH_MODE = "switchmode";
	string_traits::cstr DriverTiming::SWITCH_SEQ = "switchseq";
	string_traits::cstr DriverTiming::THROTTLE_PRESS = "throttlepress";
	string_traits::cstr DriverTiming::THROTTLE_RELEASE = "throttlerelease";
	//------------------------------------------------------------
	string_traits::cstr MotorChartMarksPack::MARKS = "marks";
	string_traits::cstr MotorChartMarksPack::GEAR = "gear";
	string_traits::cstr MotorChartMarksPack::MAX = "max";
	string_traits::cstr MotorChartMarksPack::MIN = "min";
	string_traits::cstr MotorChartMarksPack::NORM_HIGH = "normhigh";
	string_traits::cstr MotorChartMarksPack::NORM_LOW = "normlow";
	//------------------------------------------------------------
	string_traits::cstr VehicleDriver::AGR_MCMP = "agrmotorchartmarkspack";
	string_traits::cstr VehicleDriver::CALM_MCMP = "calmmotorchartmarkspack";
	string_traits::cstr VehicleDriver::AGR_TIMING = "agrtiming";
	string_traits::cstr VehicleDriver::CALM_TIMING = "calmtiming";
	string_traits::cstr VehicleDriver::DRIVE_OPTIONS = "driveoptions";
	string_traits::cstr VehicleDriver::DO_BRAKE_ASSIST = "brakeassist";
	string_traits::cstr VehicleDriver::DO_CLUTCH_ASSIST = "clutchassist";
	string_traits::cstr VehicleDriver::DO_SHIFT_ASSIST = "shiftassist";
	string_traits::cstr VehicleDriver::DO_STEER_ASSIST = "steerassist";
	string_traits::cstr VehicleDriver::DO_THROTTLE_ASSIST = "throttleassist";
	string_traits::cstr VehicleDriver::DO_NEUTRAL_PREVENT = "neutralprevent";
	string_traits::cstr VehicleDriver::DO_REVERSE_PREVENT = "reverseprevent";
	string_traits::cstr VehicleDriver::DO_USER_CLUTCH = "userclutch";
	string_traits::cstr VehicleDriver::DO_USER_CONTROLS = "usercontrols";
	//------------------------------------------------------------
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
	string_traits::cstr Suspension::DAMPER = "damper";
	string_traits::cstr Suspension::SPRING = "spring";
	string_traits::cstr Suspension::TRAVEL = "travel";
	string_traits::cstr Suspension::TARGET_VALUE = "targetvalue";
	//------------------------------------------------------------
	string_traits::cstr Tire::LONG_FUNC = "longfunction";
	string_traits::cstr Tire::LAT_FUNC = "latfunction";
	string_traits::cstr Tire::STIFFNESS_FACTOR = "stiffnessfactor";
	string_traits::cstr Tire::ASYMPTOTE_SLIP = "asymptoteslip";
	string_traits::cstr Tire::ASYMPTOTE_VALUE = "asymptotevalue";
	string_traits::cstr Tire::EXTREMUM_SLIP = "extremumslip";
	string_traits::cstr Tire::EXTREMUM_VALUE = "extremumvalue";
	//------------------------------------------------------------
	string_traits::cstr Wheel::RIM_RADIUS = "rimradius";
	string_traits::cstr Wheel::TIRE_INC_RADIUS = "tireincradius";
	string_traits::cstr Wheel::TIRE_NAME = "tirename";
	string_traits::cstr Wheel::TIRE_WIDTH = "tirewidth";
	//------------------------------------------------------------
	string_traits::cstr WheelNode::SUS_NAME = "sus";
	string_traits::cstr WheelNode::RUBBER_WHEEL_NAME = "rubberwheel";
	string_traits::cstr WheelNode::IRON_WHEEL_NAME = "ironwheel";
	string_traits::cstr WheelNode::BRAKE_BLOCK_NAME = "brakeblock";
	string_traits::cstr WheelNode::RUBBER_INC_SHIFT = "rubberwheelincshift";
	string_traits::cstr WheelNode::IRON_INC_SHIFT = "ironwheelincshift";
	//------------------------------------------------------------
	string_traits::cstr VehicleBody::BODY_NAME = "bodyname";
	string_traits::cstr VehicleBody::BOUND_BOX = "boundbox";
	string_traits::cstr VehicleBody::BOUND_MESH_NAME = "boundmeshname";
	string_traits::cstr VehicleBody::BRAKE_CONTROL = "brakecontrol";
	string_traits::cstr VehicleBody::BRAKE_MAX_PRESS_VALUE = "brakemaxpressvalue";
	string_traits::cstr VehicleBody::DIFF_NAME = "diffchainname";
	string_traits::cstr VehicleBody::GEARBOX_NAME = "gearboxname";
	string_traits::cstr VehicleBody::LEFT_WHEEL_NODE = "leftwheelnode";
	string_traits::cstr VehicleBody::RIGHT_WHEEL_NODE = "rightwheelnode";
	string_traits::cstr VehicleBody::MAX_STEER_ANGLE = "maxsteerangle";
	string_traits::cstr VehicleBody::MOTOR_NAME = "motorname";
	string_traits::cstr VehicleBody::PHYSX_MAT_NAME = "physxmatname";
	string_traits::cstr VehicleBody::TCS_NAME = "tcsname";
	string_traits::cstr VehicleBody::SOLVER_COUNT = "solvercount";
	string_traits::cstr VehicleBody::ADD_DOWN_FORCE = "adddownforce";
	string_traits::cstr VehicleBody::SPEEDOMETER = "speedometer";
	string_traits::cstr VehicleBody::SPDM_REV_AXIS = "revsensoraxis";
	string_traits::cstr VehicleBody::SPDM_REV_SIDE = "revsensorside";
	string_traits::cstr VehicleBody::SPDM_WHEEL_RADIUS = "wheelradius";
	//------------------------------------------------------------
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	//------------------------------------------------------------
}//namespace dsKeys
}//namespace System
}//namespace Demo
#endif //DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
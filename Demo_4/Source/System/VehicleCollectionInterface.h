#pragma once
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS

#include "Vehicle/Abstract/DriverTiming.h"
#include "Vehicle/Abstract/MotorChart.h"

#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
#include "Vehicle/PhysX/PhysXWheelNodeParams.h"
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS

#include "dsVehicleTypes.h"
#include "dsLoader.h"
#include "collection_traits.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	template<typename T>
	class vehicle_coll_traits
	{
	public:
		typedef typename collection_traits< T, COST_BUILTIN_SHARED_PTR >::container cont;
		typedef typename collection_traits< T, COST_BUILTIN_SHARED_PTR >::in in;
		typedef typename collection_traits< T, COST_BUILTIN_SHARED_PTR >::out out;
		typedef typename collection_traits< T, COST_BUILTIN_SHARED_PTR >::store_type store_t;
		typedef typename collection_traits< T, COST_BUILTIN_SHARED_PTR >::store_const_type store_ct;
		static inline store_t Create(){ return store_t( new T ); }
		static inline store_t Create( in obj ){ return store_t( new T(*obj) ); }
	};
	//------------------------------------------------------------
	class VehicleCollection_LoaderInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(VehicleCollection_LoaderInterface)
		//--------------------------------//
		enum ADD_OPTIONS
		{
			AO_NO_OPTIONS = 0x0,
			AO_DONT_REPLACE = 0x10
		};
		//--------------------------------//
		class traits
		{
		public:
			typedef vehicle_coll_traits< dsMotor >::in motor_in;
			typedef vehicle_coll_traits< dsGearbox >::in gearbox_in;
			typedef vehicle_coll_traits< dsFlywheel >::in flywheel_in;
			typedef vehicle_coll_traits< dsDifferential >::in differential_in;
			typedef vehicle_coll_traits< dsDifferentialChain >::in diff_chain_in;
			typedef vehicle_coll_traits< dsBrakeBlock >::in brake_block_in;
			typedef vehicle_coll_traits< dsVBV >::in vbv_in;
			typedef vehicle_coll_traits< dsThrottleControl >::in throttle_control_in;
			typedef vehicle_coll_traits< Vehicle::MotorChartMarksPack >::in mcmp_in;
			typedef vehicle_coll_traits< Vehicle::DriverTiming >::in driver_timing_in;
			typedef vehicle_coll_traits< dsVehicleDriver >::in vehicle_driver_in;
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
			typedef vehicle_coll_traits< dsPhysXVehicleBody >::in physx_vehicle_body_in;
			typedef vehicle_coll_traits< dsPhysXWheel >::in physx_wheel_in;
			typedef vehicle_coll_traits< PhysXWheelNode >::in physx_wheel_node_in;
			typedef vehicle_coll_traits< Vehicle::PhysXSuspensionDesc >::in physx_suspension_in;
			typedef vehicle_coll_traits< Vehicle::PhysXTireDesc >::in physx_tire_in;
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		};
		//--------------------------------//
		virtual ~VehicleCollection_LoaderInterface() {}

		virtual void addMotor( traits::motor_in motor, int addOptions ) = 0;
		virtual void addGearbox( traits::gearbox_in gearbox, int addOptions ) = 0;
		virtual void addFlywheel( traits::flywheel_in flywheel, int addOptions ) = 0;
		virtual void addDifferential( traits::differential_in diff, int addOptions ) = 0;
		virtual void addDifferentialChain( traits::diff_chain_in diffChain, int addOptions ) = 0;
		virtual void addBrakeBlock( traits::brake_block_in brakeBlock, int addOptions ) = 0;
		virtual void addVBV( traits::vbv_in vbv, int addOptions ) = 0;
		virtual void addThrottleControl( traits::throttle_control_in throttleControl, int addOptions ) = 0;
		virtual void addMotorChartMarksPack( traits::mcmp_in mcmp, int addOptions ) = 0;
		virtual void addDriverTiming( traits::driver_timing_in driverTiming, int addOptions ) = 0;
		virtual void addVehicleDriver( traits::vehicle_driver_in vehicleDriver, int addOptions ) = 0;
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		virtual void addPhysXVehicleBody( traits::physx_vehicle_body_in vehicleBody, int addOptions ) = 0;
		virtual void addPhysXWheel( traits::physx_wheel_in wheel, int addOptions ) = 0;
		virtual void addPhysXWheelNode( traits::physx_wheel_node_in wheelNode, int addOptions ) = 0;
		virtual void addPhysXSuspension( traits::physx_suspension_in suspension, int addOptions ) = 0;
		virtual void addPhysXTire( traits::physx_tire_in, int addOptions ) = 0;
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	};
	//------------------------------------------------------------
	class VehicleCollection_FactoryInterface : public VehicleCollection_LoaderInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(VehicleCollection_FactoryInterface)
		//--------------------------------//
		class traits : public VehicleCollection_LoaderInterface::traits
		{
		public:
			typedef vehicle_coll_traits< dsMotor >::out motor_out;
			typedef vehicle_coll_traits< dsGearbox >::out gearbox_out;
			typedef vehicle_coll_traits< dsFlywheel >::out flywheel_out;
			typedef vehicle_coll_traits< dsDifferential >::out differential_out;
			typedef vehicle_coll_traits< dsDifferentialChain >::out diff_chain_out;
			typedef vehicle_coll_traits< dsBrakeBlock >::out brake_block_out;
			typedef vehicle_coll_traits< dsVBV >::out vbv_out;
			typedef vehicle_coll_traits< dsThrottleControl >::out throttle_control_out;
			typedef vehicle_coll_traits< Vehicle::MotorChartMarksPack >::out mcmp_out;
			typedef vehicle_coll_traits< Vehicle::DriverTiming >::out driver_timing_out;
			typedef vehicle_coll_traits< dsVehicleDriver >::out vehicle_driver_out;
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
			typedef vehicle_coll_traits< dsPhysXVehicleBody >::out physx_vehicle_body_out;
			typedef vehicle_coll_traits< dsPhysXWheel >::out physx_wheel_out;
			typedef vehicle_coll_traits< PhysXWheelNode >::out physx_wheel_node_out;
			typedef vehicle_coll_traits< Vehicle::PhysXSuspensionDesc >::out physx_suspension_out;
			typedef vehicle_coll_traits< Vehicle::PhysXTireDesc >::out physx_tire_out;
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		};
		//--------------------------------//
		virtual ~VehicleCollection_FactoryInterface() {}

		virtual traits::motor_out getMotor( string_traits::in name ) = 0;
		virtual traits::gearbox_out getGearbox( string_traits::in name ) = 0;
		virtual traits::flywheel_out getFlywheel( string_traits::in name ) = 0;
		virtual traits::differential_out getDifferential( string_traits::in name ) = 0;
		virtual traits::diff_chain_out getDifferentialChain( string_traits::in name ) = 0;
		virtual traits::brake_block_out getBrakeBlock( string_traits::in name ) = 0;
		virtual traits::vbv_out getVBV( string_traits::in name ) = 0;
		virtual traits::throttle_control_out getThrottleControl( string_traits::in name ) = 0;
		virtual traits::mcmp_out getMotorChartMarksPack( string_traits::in name ) = 0;
		virtual traits::driver_timing_out getDriverTiming( string_traits::in name ) = 0;
		virtual traits::vehicle_driver_out getVehicleDriver( string_traits::in name ) = 0;
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		virtual traits::physx_vehicle_body_out getPhysXVehicleBody( string_traits::in name ) = 0;
		virtual traits::physx_wheel_out getPhysXWheel( string_traits::in name ) = 0;
		virtual traits::physx_wheel_node_out getPhysXWheelNode( string_traits::in name ) = 0;
		virtual traits::physx_suspension_out getPhysXSuspension( string_traits::in name ) = 0;
		virtual traits::physx_tire_out getPhysXTire( string_traits::in name ) = 0;
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	};
	//------------------------------------------------------------
	class VehicleLoader_CollectionInterface : public dsLoaderAuto_ProcessInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(VehicleLoader_CollectionInterface)
		//--------------------------------//
		class traits : public dsLoaderAuto_ProcessInterface::traits
		{
		public:
			typedef type_traits< VehicleCollection_LoaderInterface::shared_ptr >::cref vehicle_coll_in;
		};
		//--------------------------------//
		virtual ~VehicleLoader_CollectionInterface() {}

		virtual void addCollection( traits::vehicle_coll_in vehicleCollection, int loadOptions ) = 0;
		virtual void removeCollection( traits::vehicle_coll_in vehicleCollection ) = 0;
	};
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
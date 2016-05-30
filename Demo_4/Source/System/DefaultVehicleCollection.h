#pragma once
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS

#include "VehicleCollectionInterface.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	class DefaultVehicleLoader : public VehicleLoader_CollectionInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DefaultVehicleLoader)
		//--------------------------------//
		class traits : public VehicleLoader_CollectionInterface::traits
		{
		public:
			typedef VehicleCollection_LoaderInterface vehicle_coll_i;
			typedef Demo::map< vehicle_coll_i::shared_ptr, int >::type vehicle_coll_cont;
			typedef Ogre::MapIterator< vehicle_coll_cont > vehicle_coll_oit;
			typedef Ogre::ConstMapIterator< vehicle_coll_cont > vehicle_coll_ocit;
		};
		//--------------------------------//
		DefaultVehicleLoader();
		virtual ~DefaultVehicleLoader();

		//LoaderAuto_ProcessInterface
		virtual traits::pen_cont_out getProcessElementNames()const;
		virtual bool processElement( const TiXmlElement *element );

		//VehicleLoader_CollectionInterface
		virtual void addCollection( traits::vehicle_coll_in vehicleCollection, int collAddOptions ) { collectionCont[vehicleCollection] = collAddOptions; }
		virtual void removeCollection( traits::vehicle_coll_in vehicleCollection ) { collectionCont.erase( vehicleCollection ); }

	protected:
		template< typename T, typename LoadFunc, typename AddFunc >
		static void _ProcessImpl( LoadFunc loadFunc, AddFunc addFunc, const TiXmlElement *element, traits::vehicle_coll_cont &cont )
		{
			vehicle_coll_traits<T>::store_t obj = vehicle_coll_traits<T>::Create();
			loadFunc( element, *obj );

			traits::vehicle_coll_cont::iterator it = cont.begin();
			traits::vehicle_coll_cont::iterator eit = cont.end();
			for( ; it != eit; ++it ) {
				bind( addFunc, it->first.get() )( obj, it->second );
			}
		}

		traits::vehicle_coll_cont collectionCont;
	};
	//------------------------------------------------------------
	class DefaultVehicleCollection : public VehicleCollection_FactoryInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DefaultVehicleCollection)
		//--------------------------------//
		class traits : public VehicleCollection_FactoryInterface::traits
		{
		public:
			typedef vehicle_coll_traits< dsMotor >::cont motor_cont;
			typedef vehicle_coll_traits< dsGearbox >::cont gearbox_cont;
			typedef vehicle_coll_traits< dsFlywheel >::cont flywheel_cont;
			typedef vehicle_coll_traits< dsDifferential >::cont differential_cont;
			typedef vehicle_coll_traits< dsDifferentialChain >::cont diff_chain_cont;
			typedef vehicle_coll_traits< dsBrakeBlock >::cont brake_block_cont;
			typedef vehicle_coll_traits< dsVBV >::cont vbv_cont;
			typedef vehicle_coll_traits< dsThrottleControl >::cont throttle_control_cont;
			typedef vehicle_coll_traits< Vehicle::MotorChartMarksPack >::cont mcmp_cont;
			typedef vehicle_coll_traits< Vehicle::DriverTiming >::cont driver_timing_cont;
			typedef vehicle_coll_traits< dsVehicleDriver >::cont vehicle_driver_cont;
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
			typedef vehicle_coll_traits< dsPhysXVehicleBody >::cont physx_vehicle_body_cont;
			typedef vehicle_coll_traits< dsPhysXWheel >::cont physx_wheel_cont;
			typedef vehicle_coll_traits< PhysXWheelNode >::cont physx_wheel_node_cont;
			typedef vehicle_coll_traits< Vehicle::PhysXSuspensionDesc >::cont physx_suspension_cont;
			typedef vehicle_coll_traits< Vehicle::PhysXTireDesc >::cont physx_tire_cont;
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		};
		//--------------------------------//
		DefaultVehicleCollection();
		virtual ~DefaultVehicleCollection();

		//VehicleCollection_LoaderInterface
		virtual void addMotor( traits::motor_in motor, int addOptions ) { _AddImpl<dsMotor>( motor->motorName, motor, motorCont, addOptions ); }
		virtual void addGearbox( traits::gearbox_in gearbox, int addOptions ) { _AddImpl<dsGearbox>( gearbox->gearboxName, gearbox, gearboxCont, addOptions ); }
		virtual void addFlywheel( traits::flywheel_in flywheel, int addOptions )  { _AddImpl<dsFlywheel>( flywheel->name, flywheel, flywheelCont, addOptions ); }
		virtual void addDifferential( traits::differential_in diff, int addOptions ) { _AddImpl<dsDifferential>( diff->diffName, diff, differentialCont, addOptions ); }
		virtual void addDifferentialChain( traits::diff_chain_in diffChain, int addOptions ) { _AddImpl<dsDifferentialChain>( diffChain->name, diffChain, diffChainCont, addOptions ); }
		virtual void addBrakeBlock( traits::brake_block_in brakeBlock, int addOptions ) { _AddImpl<dsBrakeBlock>( brakeBlock->name, brakeBlock, brakeBlockCont, addOptions ); }
		virtual void addVBV( traits::vbv_in vbv, int addOptions ) { _AddImpl<dsVBV>( vbv->name, vbv, vbvCont, addOptions ); }
		virtual void addThrottleControl( traits::throttle_control_in throttleControl, int addOptions ) { _AddImpl<dsThrottleControl>( throttleControl->name, throttleControl, throttleControlCont, addOptions ); }
		virtual void addMotorChartMarksPack( traits::mcmp_in mcmp, int addOptions ) { _AddImpl<Vehicle::MotorChartMarksPack>( mcmp->name, mcmp, mcmpCont, addOptions ); }
		virtual void addDriverTiming( traits::driver_timing_in driverTiming, int addOptions ) { _AddImpl<Vehicle::DriverTiming>( driverTiming->name, driverTiming, driverTimingCont, addOptions ); }
		virtual void addVehicleDriver( traits::vehicle_driver_in vehicleDriver, int addOptions ) { _AddImpl<dsVehicleDriver>( vehicleDriver->name, vehicleDriver, vehicleDriverCont, addOptions ); }
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		virtual void addPhysXVehicleBody( traits::physx_vehicle_body_in vehicleBody, int addOptions ) { _AddImpl<dsPhysXVehicleBody>( vehicleBody->setName, vehicleBody, physxVehicleBodyCont, addOptions ); }
		virtual void addPhysXWheel( traits::physx_wheel_in wheel, int addOptions ) { _AddImpl<dsPhysXWheel>( wheel->name, wheel, physxWheelCont, addOptions ); }
		virtual void addPhysXWheelNode( traits::physx_wheel_node_in wheelNode, int addOptions ) { _AddImpl<PhysXWheelNode>( wheelNode->name, wheelNode, physxWheelNodeCont, addOptions ); }
		virtual void addPhysXSuspension( traits::physx_suspension_in suspension, int addOptions ) { _AddImpl<Vehicle::PhysXSuspensionDesc>( suspension->name, suspension, physxSuspensionCont, addOptions ); }
		virtual void addPhysXTire( traits::physx_tire_in tire, int addOptions ) { _AddImpl<Vehicle::PhysXTireDesc>( tire->name, tire, physxTireCont, addOptions ); }
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS

		//VehicleCollection_FactoryInterface
		virtual traits::motor_out getMotor( string_traits::in name ) { return _GetImpl<dsMotor>( name, motorCont ); }
		virtual traits::gearbox_out getGearbox( string_traits::in name ) { return _GetImpl<dsGearbox>( name, gearboxCont ); }
		virtual traits::flywheel_out getFlywheel( string_traits::in name ) { return _GetImpl<dsFlywheel>( name, flywheelCont ); }
		virtual traits::differential_out getDifferential( string_traits::in name ) { return _GetImpl<dsDifferential>( name, differentialCont ); }
		virtual traits::diff_chain_out getDifferentialChain( string_traits::in name ) { return _GetImpl<dsDifferentialChain>( name, diffChainCont ); }
		virtual traits::brake_block_out getBrakeBlock( string_traits::in name ) { return _GetImpl<dsBrakeBlock>( name, brakeBlockCont ); }
		virtual traits::vbv_out getVBV( string_traits::in name ){ return _GetImpl<dsVBV>( name, vbvCont ); }
		virtual traits::throttle_control_out getThrottleControl( string_traits::in name ) { return _GetImpl<dsThrottleControl>( name, throttleControlCont ); }
		virtual traits::mcmp_out getMotorChartMarksPack( string_traits::in name ) { return _GetImpl<Vehicle::MotorChartMarksPack>( name, mcmpCont ); }
		virtual traits::driver_timing_out getDriverTiming( string_traits::in name ) { return _GetImpl<Vehicle::DriverTiming>( name, driverTimingCont ); }
		virtual traits::vehicle_driver_out getVehicleDriver( string_traits::in name ) { return _GetImpl<dsVehicleDriver>( name, vehicleDriverCont ); }
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		virtual traits::physx_vehicle_body_out getPhysXVehicleBody( string_traits::in name ) { return _GetImpl<dsPhysXVehicleBody>( name, physxVehicleBodyCont ); }
		virtual traits::physx_wheel_out getPhysXWheel( string_traits::in name ) { return _GetImpl<dsPhysXWheel>( name, physxWheelCont ); }
		virtual traits::physx_wheel_node_out getPhysXWheelNode( string_traits::in name ) { return _GetImpl<PhysXWheelNode>( name, physxWheelNodeCont ); }
		virtual traits::physx_suspension_out getPhysXSuspension( string_traits::in name ) { return _GetImpl<Vehicle::PhysXSuspensionDesc>( name, physxSuspensionCont ); }
		virtual traits::physx_tire_out getPhysXTire( string_traits::in name ) { return _GetImpl<Vehicle::PhysXTireDesc>( name, physxTireCont ); }
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS

	protected:
		template< typename T >
		static void _AddImpl( string_traits::in storeName,
			typename vehicle_coll_traits<T>::in storeObject,
			typename vehicle_coll_traits<T>::cont &cont,
			int addOptions )
		{
			if( (addOptions & AO_DONT_REPLACE) == 0 ) cont.erase( storeName );
			cont.insert( std::make_pair(storeName, vehicle_coll_traits<T>::Create(storeObject)) );
		}

		template< typename T >
		typename vehicle_coll_traits<T>::out _GetImpl( string_traits::in name,
			typename vehicle_coll_traits<T>::cont &cont )
		{
			typename vehicle_coll_traits<T>::cont::iterator iter = cont.find( name );
			if( iter != cont.end() )
				return iter->second;
			return typename vehicle_coll_traits<T>::out();
		}

		traits::motor_cont motorCont;
		traits::gearbox_cont gearboxCont;
		traits::flywheel_cont flywheelCont;
		traits::differential_cont differentialCont;
		traits::diff_chain_cont diffChainCont;
		traits::brake_block_cont brakeBlockCont;
		traits::vbv_cont vbvCont;
		traits::throttle_control_cont throttleControlCont;
		traits::mcmp_cont mcmpCont;
		traits::driver_timing_cont driverTimingCont;
		traits::vehicle_driver_cont vehicleDriverCont;
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		traits::physx_vehicle_body_cont physxVehicleBodyCont;
		traits::physx_wheel_cont physxWheelCont;
		traits::physx_wheel_node_cont physxWheelNodeCont;
		traits::physx_suspension_cont physxSuspensionCont;
		traits::physx_tire_cont physxTireCont;
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	};
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
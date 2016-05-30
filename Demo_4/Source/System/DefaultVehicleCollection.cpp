#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS

#include "DefaultVehicleCollection.h"
#include <boost/algorithm/string.hpp>
#include "dsVehicleTypesLoader.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	static string_traits::cstr VEHICLE_LOADER_FIELD( "vehicleTypes" );
	//------------------------------------------------------------
	DefaultVehicleLoader::DefaultVehicleLoader()
	{
	}
	//------------------------------------------------------------
	DefaultVehicleLoader::~DefaultVehicleLoader()
	{
	}
	//------------------------------------------------------------
	/*virtual*/ DefaultVehicleLoader::traits::pen_cont_out DefaultVehicleLoader::getProcessElementNames()const
	{
		shared_ptr_traits< traits::pen_cont >::type pnn( new traits::pen_cont );
		pnn->insert( &VEHICLE_LOADER_FIELD );
		return pnn;
	}
	//------------------------------------------------------------
	/*virtual*/ bool DefaultVehicleLoader::processElement( const TiXmlElement *element )
	{
		typedef dsKeys::VehicleCommon VC;
		typedef traits::vehicle_coll_i VCI;

		const TiXmlElement *childElement(0);
		while( dsIterateChildElements(element, childElement) )
		{
			string_traits::outref childName = childElement->ValueStr();
			if( string_equals<false>()(childName, VC::MOTOR_PARAMS) )
			{
				_ProcessImpl<dsMotor>( bind(&LoadMotor<false>), &VCI::addMotor, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::GEARBOX_PARAMS) )
			{
				_ProcessImpl<dsGearbox>( bind(&LoadGearbox<false>), &VCI::addGearbox, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::FLYWHEEL_PARAMS) )
			{
				_ProcessImpl<dsFlywheel>( bind(&LoadFlywheel<false>), &VCI::addFlywheel, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::DIFFERENTIAL_PARAMS) )
			{
				_ProcessImpl<dsDifferential>( bind(&LoadDifferential<false>), &VCI::addDifferential, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::DIFFERENTIAL_CHAIN) )
			{
				_ProcessImpl<dsDifferentialChain>( bind(&LoadDifferentialChain<false>), &VCI::addDifferentialChain, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::BRAKE_BLOCK) )
			{
				_ProcessImpl<dsBrakeBlock>( bind(&LoadBrakeBlock<false>), &VCI::addBrakeBlock, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::VBV) )
			{
				_ProcessImpl<dsVBV>( bind(&LoadVBV<false>), &VCI::addVBV, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::MCM_PACK) )
			{
				_ProcessImpl<Vehicle::MotorChartMarksPack>( bind(&LoadMCMP<false>), &VCI::addMotorChartMarksPack, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::TCS) )
			{
				_ProcessImpl<dsThrottleControl>( bind(&LoadThrottleControl<false>), &VCI::addThrottleControl, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::DRIVER_TIMING) )
			{
				_ProcessImpl<Vehicle::DriverTiming>( bind(&LoadDriverTiming<false>), &VCI::addDriverTiming, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::DRIVER) )
			{
				_ProcessImpl<dsVehicleDriver>( bind(&LoadVehicleDriver<false>), &VCI::addVehicleDriver, childElement, collectionCont );
			}
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
			else if( string_equals<false>()(childName, VC::PHYSX_SUSPENSION_DESC) )
			{
				_ProcessImpl<Vehicle::PhysXSuspensionDesc>( bind(&LoadPhysXSuspension<false>), &VCI::addPhysXSuspension, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::PHYSX_TIRE_DESC) )
			{
				_ProcessImpl<Vehicle::PhysXTireDesc>( bind(&LoadPhysXTire<false>), &VCI::addPhysXTire, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::PHYSX_WHEEL) )
			{
				_ProcessImpl<dsPhysXWheel>( bind(&LoadPhysXWheel<false>), &VCI::addPhysXWheel, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::PHYSX_WHEEL_NODE) )
			{
				_ProcessImpl<PhysXWheelNode>( bind(&LoadPhysXWheelNode<false>), &VCI::addPhysXWheelNode, childElement, collectionCont );
			}
			else if( string_equals<false>()(childName, VC::PHYSX_VEHICLE_BODY) )
			{
				_ProcessImpl<dsPhysXVehicleBody>( bind(&LoadPhysXVehicleBody<false>), &VCI::addPhysXVehicleBody, childElement, collectionCont );
			}
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		}

		return true;
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
	DefaultVehicleCollection::DefaultVehicleCollection()
	{
	}
	//------------------------------------------------------------
	/*virtual*/ DefaultVehicleCollection::~DefaultVehicleCollection()
	{
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
} //namespace System
} //namespace Demo
#endif //DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
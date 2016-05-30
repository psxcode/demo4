#pragma once

#include "DemoPrerequisites.h"
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS

#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
#	include <NxWheelShapeDesc.h>
#endif

#include <OgreSingleton.h>

#include "Core\ValueByValue.h"

#include "Vehicle\Abstract\MotorParams.h"
#include "Vehicle\Abstract\DifferentialParams.h"
#include "Vehicle\Abstract\GearboxParams.h"
#include "Vehicle\Abstract\FlywheelParams.h"
#include "Vehicle\Abstract\BrakeBlock.h"
#include "Vehicle\Abstract\DriverTiming.h"
#include "Vehicle\Abstract\ThrottleControlParams.h"

#if DBS_LM_SCRIPT_OGREMAX_TIXML
#	include "System\VehicleParamsRSD.h"
#	include "System\OgremaxTinyXmlInterface.h"
#endif

#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
#	include "Vehicle\PhysX\PhysXWheelNodeParams.h"
#	include "Vehicle\PhysX\PhysXVehicleBodyParams.h"
#endif

#include "Vehicle\Driver\drive_options.h"
#include "collection_traits.h"

namespace Demo
{
namespace System
{
//------------------------------------------------------------
class VehicleParamsCollection : public Ogre::Singleton< VehicleParamsCollection >
{
	friend class Ogre::Singleton< VehicleParamsCollection >;
public:
	//--------------------------------//
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	typedef OgremaxTinyXmlInterface::traits< TiXmlElement > tixmlel_traits;
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML

	typedef Demo::string_traits string_traits;
	//--------------------------------//
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	enum LOAD_OPTIONS
	{
		LO_NO_OPTION		= 0x0,
		LO_REPLACE_DUPS		= 0x1,
			
		LO_SKIP_BRAKE		= 0x10,
		LO_SKIP_MOTOR		= 0x20,
		LO_SKIP_DIFF		= 0x40,
		LO_SKIP_GEARBOX		= 0x80,
		LO_SKIP_FLYWHEEL	= 0x100,

#	if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
		LO_SKIP_SUSPENSION	= 0x1000,
		LO_SKIP_WHEEL		= 0x2000,
		LO_SKIP_TIRE		= 0x4000,
		LO_SKIP_BODY		= 0x8000
#	endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	};
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML
	//--------------------------------//
	class traits
	{
	public:
		enum CollectionObjectStoreType
		{
			COST_PARAMS = COST_BUILTIN_SHARED_PTR,
			COST_RSD = COST_BUILTIN_SHARED_PTR,
			COST_DESC = COST_EXPLICIT_SHARED_PTR
		};
		//RSD collections
	#if DBS_LM_SCRIPT_OGREMAX_TIXML
		typedef collection_traits< MotorParamsRSD, COST_RSD >			MotorParamsRSD_traits;
		typedef collection_traits< DifferentialParamsRSD, COST_RSD >	DifferentialParamsRSD_traits;
		typedef collection_traits< GearboxParamsRSD, COST_RSD >			GearboxParamsRSD_traits;
		typedef collection_traits< FlywheelParamsRSD, COST_RSD >		FlywheelParamsRSD_traits;
		typedef collection_traits< DifferentialChainRSD, COST_RSD >		DifferentialChainRSD_traits;
		typedef collection_traits< VBVRSD, COST_RSD >					VBVRSD_traits;
		typedef collection_traits< BrakeBlockParamsRSD, COST_RSD >		BrakeBlockParamsRSD_traits;
		typedef collection_traits< ThrottleControlParamsRSD, COST_RSD >	ThrottleControlParamsRSD_traits;
	#endif // DBS_LM_SCRIPT_OGREMAX_TIXML

		//Params collections
		typedef collection_traits< Vehicle::MotorParams, COST_PARAMS >				MotorParams_traits;
		typedef collection_traits< Vehicle::DifferentialParams, COST_PARAMS >		DifferentialParams_traits;
		typedef collection_traits< Vehicle::GearboxParams, COST_PARAMS >			GearboxParams_traits;
		typedef collection_traits< Vehicle::FlywheelParams, COST_PARAMS >			FlywheelParams_traits;
		typedef collection_traits< Vehicle::BrakeBlockParams, COST_PARAMS >			BrakeBlockParams_traits;
		typedef collection_traits< Vehicle::DriverTiming, COST_PARAMS >				DriverTiming_traits;
		typedef collection_traits< Vehicle::MotorChartMarksPack, COST_PARAMS >		MotorChartMarksPack_traits;
		typedef collection_traits< Vehicle::ThrottleControlParams, COST_PARAMS >	ThrottleControlParams_traits;
		typedef collection_traits< Core::OgreRealVBV, COST_PARAMS >					FloatVBV_traits;

	#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	#if DBS_LM_SCRIPT_OGREMAX_TIXML
		typedef collection_traits< PhysXVehicleBodyParamsRSD, COST_RSD >			PhysXVehicleBodyParamsRSD_traits;
		typedef collection_traits< PhysXWheelParamsRSD, COST_RSD >					PhysXWheelParamsRSD_traits;
		typedef collection_traits< PhysXWheelNodeParamsRSD, COST_RSD >				PhysXWheelNodeParamsRSD_traits;
	#endif //DBS_LM_SCRIPT_OGREMAX_TIXML
		typedef collection_traits< Vehicle::PhysXVehicleBodyParams, COST_PARAMS >	PhysXVehicleBodyParams_traits;
		typedef collection_traits< Vehicle::PhysXWheelParams, COST_PARAMS >			PhysXWheelParams_traits;
		typedef collection_traits< Vehicle::PhysXWheelNodeParams, COST_PARAMS >		PhysXWheelNodeParams_traits;
		typedef collection_traits< Vehicle::PhysXSuspensionDesc, COST_DESC >		PhysXSuspensionDesc_traits;
		typedef collection_traits< Vehicle::PhysXTireDesc, COST_DESC >				PhysXTireDesc_traits;
	#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	};
	//--------------------------------//
	static VehicleParamsCollection& getSingleton();
	static VehicleParamsCollection* getSingletonPtr();
	//--------------------------------//
	//Parse Convert static functions
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	static Vehicle::GEARBOX_TYPE					ParseGearboxType( string_traits::in gearboxType, Vehicle::GEARBOX_TYPE defaultType = Vehicle::GT_NOT_DEFINED );
	static int										ParseGearboxModes( GearboxParamsRSD::traits::str_container_in modeNameArray, int defaultValue = -1 );
	static int										ParseDifferentialModes( DifferentialParamsRSD::traits::str_container_in, int defaultValue = -1 );
	static Vehicle::AUTO_GEARBOX_MODE				ParseGearboxMode( string_traits::in modeName, Vehicle::AUTO_GEARBOX_MODE defaultMode = Vehicle::AGM_NOT_DEFINED );
	static Vehicle::DIFFERENTIAL_MODE				ParseDifferentialMode( string_traits::in modeName, Vehicle::DIFFERENTIAL_MODE defaultMode = Vehicle::DM_NOT_DEFINED );
	static Core::BaseVBV::VBV_TYPE					ParseVBVType( string_traits::in typeName );
	static Vehicle::DRIVE_OPTIONS					ParseDriveOptions( string_traits::in optionName );
	static Vehicle::THROTTLE_CONTROL_TYPE			ParseTCSType( string_traits::in tcsTypeName );
	static Vehicle::MotorParams::TORQUE_MULT_FUNC	ParseMotorTorqueMultFunc( string_traits::in funcName, Vehicle::MotorParams::TORQUE_MULT_FUNC defaultFunc = Vehicle::MotorParams::TMF_CONST );

	static void	ConvertGearboxModeToString( int gearboxModes, GearboxParamsRSD::traits::str_container_inout modeArray );
	static string_traits::str ConvertGearboxTypeToString( Vehicle::GEARBOX_TYPE type );
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML

	//--------------------------------//
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	void AddParamsFrom( tixmlel_traits::in, int loadOptions = LO_NO_OPTION );
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML

	traits::MotorParams_traits::out GetMotorParams( string_traits::in name );
	traits::GearboxParams_traits::out GetGearboxParams( string_traits::in name );
	traits::DifferentialParams_traits::out GetDiffChainParams( string_traits::in chainName );
	traits::DifferentialParams_traits::out GetDifferentialParams( string_traits::in name );
	traits::FlywheelParams_traits::out GetFlywheelParams( string_traits::in name );
	traits::BrakeBlockParams_traits::out GetBrakeBlockParams( string_traits::in name );
	traits::DriverTiming_traits::out GetDriverTiming( string_traits::in name );
	traits::MotorChartMarksPack_traits::out GetMotorChartMarksPack( string_traits::in name );
	traits::ThrottleControlParams_traits::out GetThrottleControlParams( string_traits::in name );
	traits::FloatVBV_traits::out GetTorqueLookup( string_traits::in name );
	traits::FloatVBV_traits::out GetDragLookup( string_traits::in name, Core::LinearVBV< float, float > *createdVBV );

	int GetDriveOptions()const{ return vehicleDriverRsd.driveOptions; }

	traits::MotorParams_traits::out GetDefaultMotorParams()const { return defaultMotorParams; }
	traits::GearboxParams_traits::out GetDefaultGearboxParams()const { return defaultGearboxParams; }
	traits::DifferentialParams_traits::out GetDefaultDifferentialParams()const { return defaultDifferentialParams; }
	traits::FlywheelParams_traits::out GetDefaultFlywheelParams()const { return defaultFlywheelParams; }

#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	traits::PhysXVehicleBodyParams_traits::out	GetPhysXVehicleBodyParams( string_traits::in name );
	traits::PhysXWheelParams_traits::out		GetPhysXWheelParams( string_traits::in name );
	traits::PhysXWheelNodeParams_traits::out	GetPhysXWheelNodeParams( string_traits::in name );
	traits::PhysXSuspensionDesc_traits::out		GetPhysXSuspensionDesc( string_traits::in name );
	traits::PhysXTireDesc_traits::out			GetPhysXTireDesc( string_traits::in name );

	traits::PhysXSuspensionDesc_traits::out		GetDefaultPhysXSuspensionDesc()const { return defaultPhysXSuspensionDesc; }
	traits::PhysXTireDesc_traits::out			GetDefaultPhysXTireDesc()const { return defaultPhysXTireDesc; }
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	traits::PhysXVehicleBodyParams_traits::out	CreatePhysXVehicleBodyParams( const PhysXVehicleBodyParamsRSD& );
	traits::PhysXWheelParams_traits::out		CreatePhysXWheelParams( const PhysXWheelParamsRSD& );
	traits::PhysXWheelNodeParams_traits::out	CreatePhysXWheelNodeParams( const PhysXWheelNodeParamsRSD& );
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML

#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS

	//Функции Add для заполнения коллекции вручную
	void AddMotorParams( const traits::MotorParams_traits::container::value_type& pair, bool replace = true );
	void AddGearboxParams( const traits::GearboxParams_traits::container::value_type& pair, bool replace = true );
	void AddDifferentialParams( const traits::DifferentialParams_traits::container::value_type& pair, bool replace = true );
	void AddFlywheelParams( const traits::FlywheelParams_traits::container::value_type& pair, bool replace = true );
	void AddBrakeBlockParams( const traits::BrakeBlockParams_traits::container::value_type& pair, bool replace = true );
	void AddMotorChartMarksPack( const traits::MotorChartMarksPack_traits::container::value_type& pair, bool replace = true );
	void AddThrottleControlParams( const traits::ThrottleControlParams_traits::container::value_type& pair, bool replace = true );
	void AddDriverTiming( const traits::DriverTiming_traits::container::value_type& pair, bool replace = true );
#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	void AddPhysXVehicleBodyParams( const traits::PhysXVehicleBodyParams_traits::container::value_type& pair, bool replace = true );
	void AddPhysXWheelParams( const traits::PhysXWheelParams_traits::container::value_type& pair, bool replace = true );
	void AddPhysXWheelNodeParams( const traits::PhysXWheelNodeParams_traits::container::value_type& pair, bool replace = true );
	void AddPhysXSuspensionDesc( const traits::PhysXSuspensionDesc_traits::container::value_type& pair, bool replace = true );
	void AddPhysXTireDesc( const traits::PhysXTireDesc_traits::container::value_type& pair, bool replace = true );
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS

private:
	VehicleParamsCollection();
	VehicleParamsCollection( const VehicleParamsCollection& );
	VehicleParamsCollection& operator=( const VehicleParamsCollection& );
	~VehicleParamsCollection();

#if DBS_LM_SCRIPT_OGREMAX_TIXML
	traits::MotorParams_traits::out CreateMotorParams( const MotorParamsRSD& mp );
	traits::GearboxParams_traits::out CreateGearboxParams( const GearboxParamsRSD& );
	traits::DifferentialParams_traits::out CreateDifferentialParams( const DifferentialParamsRSD& );
	traits::DifferentialParams_traits::out CreateDifferentialParams( const DifferentialChainRSD::Node& node );
	traits::FlywheelParams_traits::out CreateFlywheelParams( const FlywheelParamsRSD& );
	traits::BrakeBlockParams_traits::out CreateBrakeBlockParams( const BrakeBlockParamsRSD& );
	traits::ThrottleControlParams_traits::out CreateThrottleControlParams( const ThrottleControlParamsRSD& );
	//const VehicleDriverRSD& CreateVehicleDriverRSD()const{ return driverRSD; }

	void LoadMotorParamsRSD				( tixmlel_traits::in element, bool replace = true );
	void LoadGearboxParamsRSD			( tixmlel_traits::in element, bool replace = true );
	void LoadGearboxRatios				( tixmlel_traits::in element, GearboxParamsRSD::traits::real_container &ratios );
	void LoadGearboxModes				( tixmlel_traits::in element, GearboxParamsRSD::traits::str_container &modes );
	void LoadFlywheelParamsRSD			( tixmlel_traits::in element, bool replace = true );
	void LoadDifferentialParamsRSD		( tixmlel_traits::in element, bool replace = true );
	void LoadDifferentialModes			( tixmlel_traits::in element, DifferentialParamsRSD::traits::str_container &modes );
	void LoadDifferentialChainRSD		( tixmlel_traits::in element, bool replace = true );
	void LoadDifferentialChainNode		( tixmlel_traits::in element, DifferentialChainRSD::Node &node );
	void LoadVBVRSD						( tixmlel_traits::in element, bool replace = true );
	void LoadVBVEntries					( tixmlel_traits::in element, VBVRSD::traits::real_container &entries );
	void LoadBrakeBlockParamsRSD		( tixmlel_traits::in element, bool replace = true );
	void LoadDriverTiming				( tixmlel_traits::in element, bool replace = true );
	void LoadDriver						( tixmlel_traits::in element );
	void LoadDriveOptions				( tixmlel_traits::in element );
	void LoadMCMPack					( tixmlel_traits::in element, bool replace = true );
	void LoadMCM						( tixmlel_traits::in element, Vehicle::MotorChartMarks &chart );
	void LoadThrottleControlParamsRSD	( tixmlel_traits::in element, bool replace = true );	


#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
	void LoadPhysXSuspensionDesc			( tixmlel_traits::in element, bool replace = true );
	void LoadPhysXTireDesc					( tixmlel_traits::in element, bool replace = true );
	void LoadPhysXTireFunction				( tixmlel_traits::in element, NxTireFunctionDesc& outFunc );
	void LoadPhysXWheelParamsRSD			( tixmlel_traits::in element, bool replace = true );
	void LoadPhysXWheelNodeParamsRSD		( tixmlel_traits::in element, bool replace = true );
	void LoadPhysXVehicleBodyParamsRSD		( tixmlel_traits::in element, bool replace = true );
	void LoadPhysXVehicleBodyParamsRSDAxle	( tixmlel_traits::in element, PhysXVehicleBodyParamsRSD::AxleParamsRSD& axle );
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML

		
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	traits::MotorParamsRSD_traits::container motorParamsRsdContainer;
	traits::GearboxParamsRSD_traits::container gearboxParamsRsdContainer;
	traits::DifferentialParamsRSD_traits::container differentialParamsRsdContainer;
	traits::DifferentialChainRSD_traits::container differentialChainRsdContainer;
	traits::FlywheelParamsRSD_traits::container flywheelParamsRsdContainer;
	traits::VBVRSD_traits::container vbvRsdContainer;
	traits::BrakeBlockParamsRSD_traits::container brakeBlockParamsRsdContainer;
	traits::ThrottleControlParamsRSD_traits::container throttleControlParamsRsdContainer;
	VehicleDriverRSD vehicleDriverRsd;
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML

	traits::MotorParams_traits::container				motorParamsContainer;
	traits::GearboxParams_traits::container				gearboxParamsContainer;
	traits::DifferentialParams_traits::container		differentialParamsContainer;
	traits::FlywheelParams_traits::container			flywheelParamsContainer;
	traits::BrakeBlockParams_traits::container			brakeBlockParamsContainer;
	traits::DriverTiming_traits::container				driverTimingContainer;
	traits::MotorChartMarksPack_traits::container		motorChartMarksPackContainer;
	traits::ThrottleControlParams_traits::container		throttleControlParamsContainer;

	traits::MotorParams_traits::store_type				defaultMotorParams;
	traits::DifferentialParams_traits::store_type		defaultDifferentialParams;
	traits::GearboxParams_traits::store_type			defaultGearboxParams;
	traits::FlywheelParams_traits::store_type			defaultFlywheelParams;

	traits::MotorParams_traits::store_type				notFoundMotorParams;
	traits::DifferentialParams_traits::store_type		notFoundDifferentialParams;
	traits::GearboxParams_traits::store_type			notFoundGearboxParams;
	traits::FlywheelParams_traits::store_type			notFoundFlywheelParams;
	traits::BrakeBlockParams_traits::store_type			notFoundBrakeBlockParams;
	traits::ThrottleControlParams_traits::store_type	notFoundThrottleControlParams;
	traits::DriverTiming_traits::store_type				notFoundDriverTiming;
	traits::MotorChartMarksPack_traits::store_type		notFoundMotorChartMarksPack;
	traits::FloatVBV_traits::store_type					notFoundFloatVBV;

#if DBS_LM_PHYSX_VEHICLE_COLLECTIONS
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	traits::PhysXVehicleBodyParamsRSD_traits::container	physxVehicleBodyParamsRsdContainer;
	traits::PhysXWheelParamsRSD_traits::container		physxWheelParamsRsdContainer;
	traits::PhysXWheelNodeParamsRSD_traits::container	physxWheelNodeParamsRsdContainer;
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML
	traits::PhysXSuspensionDesc_traits::container		physxSuspensionDescContainer;
	traits::PhysXTireDesc_traits::container				physxTireDescContainer;
	traits::PhysXVehicleBodyParams_traits::container	physxVehicleBodyParamsContainer;
	traits::PhysXWheelParams_traits::container			physxWheelParamsContainer;
	traits::PhysXWheelNodeParams_traits::container		physxWheelNodeParamsContainer;

	traits::PhysXSuspensionDesc_traits::store_type		defaultPhysXSuspensionDesc;
	traits::PhysXTireDesc_traits::store_type			defaultPhysXTireDesc;
	traits::PhysXVehicleBodyParams_traits::store_type	defaultPhysXVehicleBodyParams;
	traits::PhysXWheelParams_traits::store_type			defaultPhysXWheelParams;
	traits::PhysXWheelNodeParams_traits::store_type		defaultPhysXWheelNodeParams;

	traits::PhysXSuspensionDesc_traits::store_type		notFoundPhysXSuspensionDesc;
	traits::PhysXTireDesc_traits::store_type			notFoundPhysXTireDesc;
	traits::PhysXVehicleBodyParams_traits::store_type	notFoundPhysXVehicleBodyParams;
	traits::PhysXWheelParams_traits::store_type			notFoundPhysXWheelParams;
	traits::PhysXWheelNodeParams_traits::store_type		notFoundPhysXWheelNodeParams;
#endif //DBS_LM_PHYSX_VEHICLE_COLLECTIONS
};
//------------------------------------------------------------
}
}
#endif
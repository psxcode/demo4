#pragma once
#include "Vehicle\Abstract\MotorParams.h"
#include "Vehicle\Abstract\GearboxParams.h"
#include "Vehicle\Abstract\DifferentialParams.h"
#include "PhysXWheelNodeParams.h"
#include "Vehicle\Abstract\ThrottleControlParams.h"
#include "Vehicle\Abstract\BrakeBlock.h"
#include "Vehicle\Abstract\SpeedometerParams.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class PhysXVehicleBodyParams
	{
	public:
		DEFINE_CLASS_SHARED_PTR(PhysXVehicleBodyParams)
		//--------------------------------//
		class PhysXAxleParams
		{
		public:
			DEFINE_CLASS_SHARED_PTR(PhysXAxleParams)
			//--------------------------------//
			PhysXAxleParams()
				: index(0)
				, position( Ogre::Vector3::ZERO )
				, maxSteerAngle(0)
				, isUnitScale( false )
			{
			}
			PhysXAxleParams( const PhysXAxleParams& p )
				: index(0)
				, position( Ogre::Vector3::ZERO )
				, maxSteerAngle(0)
				, isUnitScale( false )
			{
				(*this) = p;
			}
			virtual ~PhysXAxleParams(){ ClosePhysXVehicleBodyParams(); }
			PhysXAxleParams& operator=( const PhysXAxleParams& p )
			{
				if( this == &p )
					return *this;
				CopyPhysXAxleParams( p );
				return *this;
			}
			bool isValid()const
			{
				return
					!(maxSteerAngle < 0)			&&
					!leftWheelNodeParams.isNull()	&&
					leftWheelNodeParams->isValid()	&&
					!rightWheelNodeParams.isNull()	&&
					rightWheelNodeParams->isValid() ;
			}
			void DistanceToUnits()
			{
				if( !leftWheelNodeParams.isNull() )leftWheelNodeParams->DistanceToUnits();
				if( !rightWheelNodeParams.isNull() )rightWheelNodeParams->DistanceToUnits();
				if( isUnitScale )return;
				position.x = UnitConverter::DistanceToUnits( position.x );
				position.y = UnitConverter::DistanceToUnits( position.y );
				position.z = UnitConverter::DistanceToUnits( position.z );
				isUnitScale = true;
			}
			void UnitsToDistance()
			{
				if( !leftWheelNodeParams.isNull() )leftWheelNodeParams->UnitsToDistance();
				if( !rightWheelNodeParams.isNull() )rightWheelNodeParams->UnitsToDistance();
				if( !isUnitScale )return;
				position.x = UnitConverter::UnitsToDistance( position.x );
				position.y = UnitConverter::UnitsToDistance( position.y );
				position.z = UnitConverter::UnitsToDistance( position.z );
				isUnitScale = false;
			}
			void ClosePhysXVehicleBodyParams()
			{
				leftWheelNodeParams.setNull();
				rightWheelNodeParams.setNull();

				index = 0;
				position = Ogre::Vector3::ZERO;
				maxSteerAngle = 0;
				isUnitScale = false;
			}

			//basic data
			unsigned int index;
			Ogre::Vector3 position;
			float maxSteerAngle;
			Core::puFloat brakeMaxPressValue;
			//basic data

			//PhysX data
			PhysXWheelNodeParams::shared_ptr leftWheelNodeParams;
			PhysXWheelNodeParams::shared_ptr rightWheelNodeParams;
			//PhysX data

		protected:
			void CopyPhysXAxleParams( const PhysXVehicleBodyParams::PhysXAxleParams& p )
			{
				//basic data
				index = p.index;
				position = p.position;
				maxSteerAngle = p.maxSteerAngle;
				//basic data

				//PhysX data
				leftWheelNodeParams.setNull();
				rightWheelNodeParams.setNull();
				if( !p.leftWheelNodeParams.isNull() )leftWheelNodeParams.bind( new PhysXWheelNodeParams( *p.leftWheelNodeParams ) );
				if( !p.rightWheelNodeParams.isNull() )rightWheelNodeParams.bind( new PhysXWheelNodeParams( *p.rightWheelNodeParams ) );
				//PhysX data
			}
			bool isUnitScale;
		};
		//--------------------------------//
		class traits
		{
		public:
			typedef PhysXAxleParams::shared_ptr axle_type;
			typedef Demo::vector< axle_type >::type axle_container;
		};
		//--------------------------------//
		PhysXVehicleBodyParams()
			: mass(0)
			, boundBox( Ogre::Vector3::ZERO )
			, massOffset( Ogre::Vector3::ZERO )
			, solverCount(0)
			, addDownForce(0)
		{
		}
		PhysXVehicleBodyParams( const PhysXVehicleBodyParams& p )
			: mass(0)
			, boundBox( Ogre::Vector3::ZERO )
		{
			(*this) = p;
		}
		virtual ~PhysXVehicleBodyParams(){ ClosePhysXVehicleBodyParams(); }
		PhysXVehicleBodyParams& operator=( const PhysXVehicleBodyParams& p )
		{
			if( this == &p )
				return *this;
			CopyPhysXVehicleBodyParams( p );
			return *this;
		}
		bool isValid()const
		{
			traits::axle_container::const_iterator iter = axles.begin();
			traits::axle_container::const_iterator end_iter = axles.end();
			traits::axle_container::size_type axleCount = axles.size();
			for( ; iter != end_iter; ++iter )
				if( !(*iter)->isValid() )return false;

			return
				axleCount > 1					&&
				!bodyName.empty()				&&
				!bodyOSNParams.isNull()			&&
				boundBox != Ogre::Vector3::ZERO	&&
				solverCount > 0					&&
				mass > 0						&&
				!motor.isNull()					&&
				motor->isValid()				&&
				!gearbox.isNull()				&&
				gearbox->isValid()				&&
				!diff.isNull()					&&
				diff->isValid()					;
		}
		PhysXVehicleBodyParams::shared_ptr CreateCopyPtr()const{ return PhysXVehicleBodyParams::shared_ptr( new PhysXVehicleBodyParams( *this ) ); }
		void ClosePhysXVehicleBodyParams()
		{
			//basic data
			bodyName.clear();
			bodyOSNParams.setNull();
			boundBox = Ogre::Vector3::ZERO;
			mass = 0;
			//basic data

			//PhysX data
			solverCount = 0;
			physxMatName.clear();
			boundMeshName.clear();
			axles.clear();
			//PhysX data
		}

		//basic data
		string_traits::str setName;
		string_traits::str bodyName;
		Core::SceneNodeParams::shared_ptr bodyOSNParams;
		Ogre::Vector3 boundBox;
		Ogre::Vector3 massOffset;
		float mass;
		float addDownForce;
		MotorParams::shared_ptr motor;
		GearboxParams::shared_ptr gearbox;
		DifferentialParams::shared_ptr diff;
		ThrottleControlParams::shared_ptr tcs;
		SpeedometerParams speedometer;
		//basic data

		int solverCount;
		string_traits::str boundMeshName;
		string_traits::str physxMatName;

		traits::axle_container axles;

	protected:
		void CopyPhysXVehicleBodyParams( const PhysXVehicleBodyParams& p )
		{
			//basic data
			motor.setNull();
			if( !p.motor.isNull() )motor = p.motor->CreateMotorCopyPtr();
			gearbox.setNull();
			if( !p.gearbox.isNull() )gearbox = p.gearbox->CreateGearboxCopyPtr();
			diff.setNull();
			if( !p.diff.isNull() )diff = p.diff->CreateDifferentialCopyPtr();
			bodyOSNParams.setNull();
			if( !p.bodyOSNParams.isNull() )bodyOSNParams = p.bodyOSNParams->CreateCopyPtr();
			bodyName = p.bodyName;
			mass = p.mass;
			boundBox = p.boundBox;
			speedometer = p.speedometer;
			//basic data

			solverCount = p.solverCount;
			physxMatName = p.physxMatName;
			boundMeshName = p.boundMeshName;

			axles.clear();
			traits::axle_container::const_iterator iter = p.axles.begin();
			traits::axle_container::const_iterator end_iter = p.axles.end();
			for( ; iter != end_iter; ++iter )
				axles.push_back( traits::axle_type( new PhysXAxleParams(iter->getReference()) ) );
		}
	};
	//------------------------------------------------------------
}
}
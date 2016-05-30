#pragma once
#include "PhysXVehicleBodyParams.h"
#include "Vehicle\Abstract\MotorParams.h"
#include "Vehicle\Abstract\GearboxParams.h"
#include "Vehicle\Abstract\DifferentialParams.h"

namespace Demo
{
namespace Vehicle
{
	class PhysXVehicleSetParams
	{
	public:
		PhysXVehicleSetParams()
			: body(0)
			, motor(0)
			, gearbox(0)
			, diff(0)
		{
		}
		PhysXVehicleSetParams( const PhysXVehicleSetParams& p )
		{
			(*this) = p;
		}
		virtual ~PhysXVehicleSetParams(){ClosePhysXVehicleSetParams();}
		PhysXVehicleSetParams& operator=( const PhysXVehicleSetParams& p )
		{
			if( this == &p )
				return *this;
			CopyPhysXVehicleSetParams( p );
			return *this;
		}
		bool isValid()const
		{
			if( setName.empty() )return false;
			if( !body || !body->isValid() )return false;
			if( !motor || !motor->isValid() )return false;
			if( !gearbox || !gearbox->isValid() )return false;
			if( !diff || !diff->isValid() )return false;
			return true;
		}
		void ClosePhysXVehicleSetParams()
		{
			setName.clear();
			if( body )delete body; body = 0;
			if( motor )delete motor; motor = 0;
			if( gearbox )delete gearbox; gearbox = 0;
			if( diff )delete diff; diff = 0;
		}

		/*
		Имя набора
		*/
		Ogre::String setName;

		PhysXVehicleBodyParams* body;

		/*
		Копия VehicleMotorParams
		Здесь параметры мотора.
		*/
		MotorParams* motor;
		GearboxParams* gearbox;
		DifferentialParams* diff;

	protected:
		void CopyPhysXVehicleSetParams( const PhysXVehicleSetParams& p )
		{
			setName = p.setName;
			if( body ){ delete body; body = 0; }
			if( p.body )body = p.body->CreateCopyPtr();
			if( motor ){ delete motor; motor = 0; }
			if( p.motor )motor = static_cast< MotorParams* >(p.motor->CreateCopyPtr());
			if( gearbox ){ delete gearbox; gearbox = 0; }
			if( p.gearbox )gearbox = static_cast< GearboxParams* >(p.gearbox->CreateCopyPtr());
			if( diff ){ delete diff; diff = 0; }
			if( p.diff )diff = static_cast< DifferentialParams* >(p.diff->CreateCopyPtr());
		}
	};
}
}
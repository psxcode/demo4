#pragma once
#include "MechanicsParams.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class FlywheelParams : public MechanicsParams
	{
	public:
		DEFINE_CLASS_SHARED_PTR(FlywheelParams)
		//--------------------------------//
		FlywheelParams()
			: MechanicsParams( MT_FLYWHEEL )
			, mass(0)
			, radius(0)
		{
		}
		FlywheelParams( const FlywheelParams& flywheel )
			: MechanicsParams( MT_FLYWHEEL )
			, mass(0)
			, radius(0)
		{
			(*this) = flywheel;
		}
		virtual ~FlywheelParams() { CloseFlywheelParams(); }
		virtual MechanicsParams::shared_ptr CreateCopyPtr()const { return CreateFlywheelCopyPtr(); }
		FlywheelParams& operator=( const FlywheelParams& p )
		{
			if( this == &p )
				return *this;
			CopyFlywheelParams( p );
			return *this;
		}
		virtual bool isValid()const
		{
			return mass > 0 && radius > 0 && MechanicsParams::isValid();
		}
		void CloseFlywheelParams()
		{
			mass = 0;
			radius = 0;
			CloseMechanicsParams();
		}
		FlywheelParams::shared_ptr CreateFlywheelCopyPtr()const { return FlywheelParams::shared_ptr( new FlywheelParams( *this ) ); }
		
		float mass;
		float radius;

	protected:
		void CopyFlywheelParams( const FlywheelParams& p )
		{
			mass = p.mass;
			radius = p.radius;
			MechanicsParams::CopyMechanicsParams( p );
		}
	};
	//------------------------------------------------------------
}
}
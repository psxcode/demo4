#pragma once

#include <NxWheelShape.h>
#include "System/dsSceneTypes.h"
#include "Interface/UnitConverter.h"
#include "Vehicle/Abstract/MechanicsParams.h"
#include "Vehicle/Abstract/BrakeBlock.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	struct PhysXSuspensionDesc
	{
		DEFINE_CLASS_SHARED_PTR(PhysXSuspensionDesc)
		//--------------------------------//
		PhysXSuspensionDesc()
			: suspensionTravel(0)
			, isUnitScale( false )
		{
		}
		bool isValid()const{ return !name.empty() && suspensionTravel > 0 && suspension.isValid(); }
		void DistanceToUnits()
		{
			if( isUnitScale )return;
			UnitConverter::DistanceToUnits( suspensionTravel );
			isUnitScale = true;
		}
		void UnitsToDistance()
		{
			if( !isUnitScale )return;
			UnitConverter::UnitsToDistance( suspensionTravel );
			isUnitScale = false;
		}

		string_traits::str name;
		NxReal suspensionTravel;
		NxSpringDesc suspension;

	protected:
		bool isUnitScale;
	};
	//------------------------------------------------------------
	struct PhysXTireDesc
	{
		DEFINE_CLASS_SHARED_PTR(PhysXTireDesc)
		//--------------------------------//
		bool isValid()const{ return !name.empty() && longFunction.isValid() && latFunction.isValid(); }

		string_traits::str name;
		NxTireFunctionDesc longFunction;
		NxTireFunctionDesc latFunction;
	};
	//------------------------------------------------------------
	class PhysXWheelParams
	{
	public:
		DEFINE_CLASS_SHARED_PTR(PhysXWheelParams)
		//--------------------------------//
		PhysXWheelParams()
			: width(0)
			, radius(0)
			, tireIncRadius(0)
			, mass(0)
			, isUnitScale(false)
		{
		}
		PhysXWheelParams( const PhysXWheelParams& p )
			: width(0)
			, radius(0)
			, tireIncRadius(0)
			, mass(0)
			, isUnitScale(false)
		{
			(*this) = p;
		}
		virtual ~PhysXWheelParams(){ ClosePhysXWheelParams(); }
		PhysXWheelParams& operator=( const PhysXWheelParams& p )
		{
			if( this == &p )
				return *this;
			CopyPhysXWheelParams( p );
			return *this;
		}
		bool isValid()const
		{
			return
				width > 0				&&
				radius > 0				&&
				!(tireIncRadius < 0)	&&
				mass > 0				&&
				!name.empty()			&&
				wheelOSN != 0			&&
				tireDesc != 0			&&
				tireDesc->isValid()		;
		}
		void UnitsToDistance()
		{
			if( !isUnitScale )return;
			UnitConverter::UnitsToDistance( width );
			UnitConverter::UnitsToDistance( radius );
			UnitConverter::UnitsToDistance( tireIncRadius );
			isUnitScale = false;
		}
		void DistanceToUnits()
		{
			if( isUnitScale )return;
			UnitConverter::DistanceToUnits( width );
			UnitConverter::DistanceToUnits( radius );
			UnitConverter::DistanceToUnits( tireIncRadius );
			isUnitScale = true;
		}
		void ClosePhysXWheelParams()
		{
			//basic data
			width = 0;
			radius = 0;
			tireIncRadius = 0;
			mass = 0;
			name.clear();
			wheelOSN.setNull();
			//basic data

			//PhysX data
			tireDesc.setNull();
			//PhysX data
		}

		//basic data
		float width;
		float radius;
		float tireIncRadius;
		float mass;
		System::dsSceneNode::shared_ptr wheelOSN;
		string_traits::str name;
		//basic data

		//PhysX data
		PhysXTireDesc::shared_ptr tireDesc;
		//PhysX data

	protected:
		void CopyPhysXWheelParams( const PhysXWheelParams& p )
		{
			//basic data
			name = p.name;
			width = p.width;
			radius = p.radius;
			tireIncRadius = p.tireIncRadius;
			mass = p.mass;
			wheelOSN.setNull();
			if( !p.wheelOSN.isNull() )wheelOSN = p.wheelOSN->CreateCopyPtr();
			//basic data

			//PhysX data
			tireDesc.setNull();
			if( !p.tireDesc.isNull() )tireDesc.bind( new PhysXTireDesc( p.tireDesc.getReference() ) );
			//PhysX data
		}

		bool isUnitScale;
	};
	//------------------------------------------------------------
	class PhysXWheelNodeParams : public MechanicsParams
	{
	public:
		DEFINE_CLASS_SHARED_PTR(PhysXWheelNodeParams)
		//--------------------------------//
		PhysXWheelNodeParams()
			: MechanicsParams( MT_WHEEL_NODE )
			, rubberWheelIncShift(0)
			, ironWheelIncShift(0)
			, isUnitScale(false)
		{
		}
		PhysXWheelNodeParams( const PhysXWheelNodeParams& p )
			: MechanicsParams( MT_WHEEL_NODE )
			, rubberWheelIncShift(0)
			, ironWheelIncShift(0)
			, isUnitScale(false)
		{
			(*this) = p;
		}
		virtual ~PhysXWheelNodeParams(){ ClosePhysXWheelNodeParams(); }
		virtual MechanicsParams::shared_ptr CreateCopyPtr()const{ return CreatePhysXWheelNodeCopyPtr(); }
		PhysXWheelNodeParams& operator=( const PhysXWheelNodeParams& p )
		{
			if( this == &p )
				return *this;
			CopyPhysXWheelNodeParams( p );
			return *this;
		}
		bool isValid()const
		{
			return
				rubberWheelParams != 0 ? rubberWheelParams->isValid() : true	&&
				ironWheelParams != 0 ? ironWheelParams->isValid() : true		&&
				susDesc != 0													&&
				susDesc->isValid()												&&
				brakeBlockParams != 0 ? brakeBlockParams->isValid() : true		;
		}
		void DistanceToUnits()
		{
			if( !rubberWheelParams.isNull() )rubberWheelParams->DistanceToUnits();
			if( !ironWheelParams.isNull() )ironWheelParams->DistanceToUnits();
			if( !susDesc.isNull() )susDesc->DistanceToUnits();
			if( isUnitScale )return;
			rubberWheelIncShift = UnitConverter::DistanceToUnits( rubberWheelIncShift );
			ironWheelIncShift = UnitConverter::DistanceToUnits( ironWheelIncShift );
			isUnitScale = true;
		}
		void UnitsToDistance()
		{
			if( !rubberWheelParams.isNull() )rubberWheelParams->UnitsToDistance();
			if( !ironWheelParams.isNull() )ironWheelParams->UnitsToDistance();
			if( !susDesc.isNull() )susDesc->UnitsToDistance();
			if( !isUnitScale )return;
			rubberWheelIncShift = UnitConverter::UnitsToDistance( rubberWheelIncShift );
			ironWheelIncShift = UnitConverter::UnitsToDistance( ironWheelIncShift );
			isUnitScale = false;
		}
		void ClosePhysXWheelNodeParams()
		{
			susDesc.setNull();
			rubberWheelParams.setNull();
			ironWheelParams.setNull();
			brakeBlockParams.setNull();
			rubberWheelIncShift = 0;
			ironWheelIncShift = 0;
			isUnitScale = false;

			CloseMechanicsParams();
		}
		PhysXWheelNodeParams::shared_ptr CreatePhysXWheelNodeCopyPtr()const{ return PhysXWheelNodeParams::shared_ptr( new PhysXWheelNodeParams( *this ) ); }

		float rubberWheelIncShift;
		float ironWheelIncShift;
		PhysXWheelParams::shared_ptr rubberWheelParams;
		PhysXWheelParams::shared_ptr ironWheelParams;
		PhysXSuspensionDesc::shared_ptr susDesc;
		BrakeBlockParams::shared_ptr brakeBlockParams;

	protected:
		void CopyPhysXWheelNodeParams( const PhysXWheelNodeParams& p )
		{
			MechanicsParams::CopyMechanicsParams( p );
			rubberWheelIncShift = p.rubberWheelIncShift;
			ironWheelIncShift = p.ironWheelIncShift;
			isUnitScale = p.isUnitScale;
			susDesc.setNull();
			if( !p.susDesc.isNull() )susDesc.bind( new PhysXSuspensionDesc( *p.susDesc.get() ) );
			rubberWheelParams.setNull();
			if( !p.rubberWheelParams.isNull() )rubberWheelParams.bind( new PhysXWheelParams( *p.rubberWheelParams.get() ) );
			ironWheelParams.setNull();
			if( !p.ironWheelParams.isNull() )ironWheelParams.bind( new PhysXWheelParams( *p.ironWheelParams.get() ) );
			brakeBlockParams.setNull();
			if( !p.brakeBlockParams.isNull() )brakeBlockParams.bind( new BrakeBlockParams( *p.brakeBlockParams.get() ) );
		}
		bool isUnitScale;
	};
	//------------------------------------------------------------
}
}
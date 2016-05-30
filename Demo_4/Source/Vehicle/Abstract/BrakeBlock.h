#pragma once

#include "Core/RangeVar.h"
#include "System/dsSceneTypes.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class BrakeBlockParams
	{
	public:
		DEFINE_CLASS_SHARED_PTR(BrakeBlockParams)
		//--------------------------------//
		BrakeBlockParams()
			: maxBrakeTorque(0)
		{
		}
		BrakeBlockParams( const BrakeBlockParams& p )
			: maxBrakeTorque(0)
		{
			*this = p;
		}
		BrakeBlockParams& operator=( const BrakeBlockParams& p )
		{
			if( this == &p )
				return *this;
			CopyBrakeBlockParams( p );
			return *this;
		}
		bool isValid()const{ return !name.empty() && maxBrakeTorque > 0; }

		string_traits::str name;
		System::dsSceneNode::shared_ptr osnParams;
		float maxBrakeTorque;

	protected:
		void CopyBrakeBlockParams( const BrakeBlockParams& p )
		{
			name = p.name;
			osnParams.setNull();
			if( !p.osnParams.isNull() ) osnParams = p.osnParams->CreateCopyPtr();
			maxBrakeTorque = p.maxBrakeTorque;
		}
	};
	//------------------------------------------------------------
	class BrakeBlock
	{
	public:
		DEFINE_CLASS_SHARED_PTR(BrakeBlock)
		//--------------------------------//
		struct BrakeBlockInitData
		{
			BrakeBlockParams::const_shared_ptr blockParams;
		};
		//--------------------------------//
		BrakeBlock()
			: maxBrakeTorque(0)
			, activeBrakeTorque(0)
		{
		}
		virtual ~BrakeBlock(){CloseBrakeBlock();}
		virtual void SetBrakeValue( const Core::puFloat& value )
		{
			brakeValue = value;
			activeBrakeTorque = maxBrakeTorque * brakeValue.get();
		}

		bool Initialize( const BrakeBlockInitData& );
		void CloseBrakeBlock();
		BrakeBlockParams::shared_ptr CreateBrakeBlockParams()const;

		inline string_traits::outref getName()const{ return name; }
		inline float getActiveBrakeTorque()const{ return activeBrakeTorque; }
		inline Core::puFloat getBrakeValue()const{ return brakeValue; }
		inline float getMaxBrakeTorque()const{ return maxBrakeTorque; }

	protected:
		string_traits::str name;
		Core::puFloat brakeValue;
		float maxBrakeTorque;
		float activeBrakeTorque;
	};
	//------------------------------------------------------------
}
}
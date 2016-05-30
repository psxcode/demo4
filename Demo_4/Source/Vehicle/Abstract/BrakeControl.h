#pragma once
#include "Core\Controller.h"
#include "BrakeBlock.h"
#include "PedalFilter.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class BrakeControl : public Core::Controller
	{
	public:
		DEFINE_CLASS_SHARED_PTR(BrakeControl)
		//--------------------------------//
		struct Axle
		{
			Axle()
				: maxPressValue( 1.0f )
			{
			}
			virtual ~Axle()
			{
			}
			virtual bool isValid()const{ return leftBlock != 0 && rightBlock != 0; }
			Core::puFloat maxPressValue;
			BrakeBlock::shared_ptr leftBlock;
			BrakeBlock::shared_ptr rightBlock;
		};
		//--------------------------------//
		typedef std::vector< BrakeControl::Axle > AxleArray;
		typedef AxleArray::iterator AIter;
		typedef AxleArray::const_iterator cAIter;
		//--------------------------------//
		struct BrakeControlInitData
		{
			AxleArray brakeAxles;
		};
		//--------------------------------//
		BrakeControl()
		{
		}
		virtual ~BrakeControl(){CloseBrakeControl();}

		//controller interface
		virtual void OperateController();
		//controller interface

		virtual IPedal::shared_ptr GetBrakeInterface( PEDAL_INTERFACE_TYPE type ){ return brake.GetInterface( type ); }

		bool Initialize( const BrakeControlInitData& data );
		void CloseBrakeControl();

	protected:
		PedalFilter brake;
		AxleArray axles;
	};
	//------------------------------------------------------------
}
}
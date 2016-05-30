#pragma once
#include "Core\Controller.h"
#include "VehicleTypes.h"
#include "PedalFilter.h"
#include "VehicleAxle.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class SteerControl : public Core::Controller
	{
	public:
		DEFINE_CLASS_SHARED_PTR(SteerControl)
		//--------------------------------//
		struct SteerControlInitData
		{
			bool isValid()const{ return steerAxle != 0; }
			VehicleAxle::shared_ptr steerAxle;
		};
		//--------------------------------//
		SteerControl()
		{
		}
		virtual ~SteerControl()
		{
		}
		//controller interface
		virtual void OperateController();
		//controller interface

		bool Initialize( const SteerControlInitData& data );

		virtual IPedal::shared_ptr GetSteerInterface( PEDAL_INTERFACE_TYPE type ) { return steer.GetInterface( type ); }
		
	protected:
		SteerFilter steer;		
		VehicleAxle::shared_ptr steerAxle;
	};
	//------------------------------------------------------------
}
}
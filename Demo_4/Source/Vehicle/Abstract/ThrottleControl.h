#pragma once
#include "Core\Controller.h"
#include "PedalFilter.h"
#include "IMotorControl.h"
#include "ThrottleControlParams.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class BaseThrottleControl : public Core::Controller
	{
	public:
		DEFINE_CLASS_SHARED_PTR(BaseThrottleControl)
		//--------------------------------//
		struct InitData
		{
			bool isValid()const{ return motorControl != 0 && tcParams != 0 && tcParams->isValid(); }

			ThrottleControlParams::const_shared_ptr tcParams;
			IMotorControl::shared_ptr motorControl;
		};
		//--------------------------------//
		virtual ~BaseThrottleControl()
		{
		}

		//controller interface
		virtual void OperateController();
		//controller interface

		virtual IPedal::shared_ptr GetThrottleInterface( PEDAL_INTERFACE_TYPE type );

		inline THROTTLE_CONTROL_TYPE getTCSType()const{ return tcsType; }

	protected:
		BaseThrottleControl( THROTTLE_CONTROL_TYPE type )
			: tcsType( type )
		{
		}

		PedalFilter throttle;
		IMotorControl::shared_ptr iMotorControl;

	private:
		BaseThrottleControl();
		BaseThrottleControl( const BaseThrottleControl& );
		BaseThrottleControl& operator=( const BaseThrottleControl& );

		THROTTLE_CONTROL_TYPE tcsType;
	};
	//------------------------------------------------------------
	class ConstFeedThrottleControl : public BaseThrottleControl
	{
	public:
		DEFINE_CLASS_SHARED_PTR(ConstFeedThrottleControl)
		//--------------------------------//
		virtual ~ConstFeedThrottleControl()
		{
		}
		ConstFeedThrottleControl()
			: BaseThrottleControl( TCS_CFEED )
		{
		}

		bool Initialize( const InitData& data );
		void SetConstantThrottleFeed( const Core::puFloat& value );
	};
	//------------------------------------------------------------
}
}
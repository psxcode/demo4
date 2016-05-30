#pragma once

#include "IGearboxController.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class IGearboxControllerState : public IGearboxController
	{
	public:
		DEFINE_CLASS_SHARED_PTR(IGearboxControllerState)
		//--------------------------------//
		virtual ~IGearboxControllerState()
		{
		}

		inline GEARBOX_TYPE getGearboxType()const{ return gearboxType; }

		bool IsNeutral()const;
		bool IsDirect()const;
		bool IsReverse()const;

	protected:
		IGearboxControllerState( GearboxController& GC, GEARBOX_TYPE gT );

		GearboxController& baseGC;

	private:
		IGearboxControllerState();
		IGearboxControllerState( const IGearboxControllerState& );
		IGearboxControllerState& operator=( const IGearboxControllerState& );

		GEARBOX_TYPE gearboxType;
	};
	//------------------------------------------------------------
	class IMechGearboxControllerState : public IGearboxControllerState
	{
		friend class MechGearboxController;
	public:
		DEFINE_CLASS_SHARED_PTR(IMechGearboxControllerState)
		//--------------------------------//
		virtual ~IMechGearboxControllerState()
		{
		}

		int GetLastGear()const;
		int GetTargetGear()const;
		
	protected:
		IMechGearboxControllerState( MechGearboxController& MGC );

		MechGearboxController& mGC;

	private:
		IMechGearboxControllerState();
		IMechGearboxControllerState( const IMechGearboxControllerState& );
		IMechGearboxControllerState& operator=( const IMechGearboxControllerState& );
	};
	//------------------------------------------------------------
	class IAutoGearboxControllerState : public IGearboxControllerState
	{
		friend class AutoGearboxController;
	public:
		DEFINE_CLASS_SHARED_PTR(IAutoGearboxControllerState)
		//--------------------------------//
		virtual ~IAutoGearboxControllerState()
		{
		}

	protected:
		IAutoGearboxControllerState( AutoGearboxController& AGC );

		AutoGearboxController& aGC;

	private:
		IAutoGearboxControllerState();
		IAutoGearboxControllerState( const IAutoGearboxControllerState& );
		IAutoGearboxControllerState& operator=( const IAutoGearboxControllerState& );
	};
	//------------------------------------------------------------
}
}
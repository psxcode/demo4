#pragma once
#include "Core\RangeVar.h"
#include "IGearbox.h"
#include "IMechanicsState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class Gearbox;
	class MechGearbox;
	class AutoGearbox;
	//------------------------------------------------------------
	class IGearboxState : public IGearbox, public IMechanicsState
	{
	public:
		DEFINE_CLASS_SHARED_PTR(IGearboxState)
		//--------------------------------//
		virtual ~IGearboxState()
		{
		}
		//Интерфейс IGearbox
		virtual IGearbox::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE type );
		//Интерфейс IGearbox

		GEARBOX_TYPE GetGType()const{ return gType; }

		//интерфейс из VehicleGearbox
		int GetActiveGearIndex()const;
		int GetTargetGearIndex()const;
		int GetDirectGearsCount()const;
		int GetReverseGearsCount()const;
		float GetActiveGearRatioMod()const;
		float GetActiveGearRatioSign()const;
		float GetOutRadPS()const;
		float GetMechShiftTime()const;
		float GetRatioAtGear( int gear_index )const;
		bool HasGear( int gear_index )const;
		//интерфейс из VehicleGearbox

	protected:
		IGearboxState( Gearbox& g );

	private:
		IGearboxState();

		GEARBOX_TYPE gType;
	};
	//------------------------------------------------------------
	class IMechGearboxState : public IGearboxState
	{
		friend class MechGearbox;
	public:
		DEFINE_CLASS_SHARED_PTR(IMechGearboxState)
		//--------------------------------//
		virtual ~IMechGearboxState()
		{
		}
		//Интерфейс IGearbox
		virtual IGearbox::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE );
		//Интерфейс IGearbox
	protected:
		IMechGearboxState( MechGearbox& mG );

		MechGearbox& mGearbox;
	};
	//------------------------------------------------------------
	class IAutoGearboxState : public IGearboxState
	{
		friend class AutoGearbox;
	public:
		DEFINE_CLASS_SHARED_PTR(IAutoGearboxState)
		//--------------------------------//
		virtual ~IAutoGearboxState()
		{
		}
		//Интерфейс IGearbox
		virtual IGearbox::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE );
		//Интерфейс IGearbox

		//интерфейс из VehicleAutoGearbox
		AUTO_GEARBOX_MODE GetModeUp()const;
		AUTO_GEARBOX_MODE GetModeDown()const;
		AUTO_GEARBOX_MODE GetModeAlt()const;
		AUTO_GEARBOX_MODE GetActiveMode()const;
		AUTO_GEARBOX_MODE GetTargetMode()const;
		int GetAvailableModes()const;
		//интерфейс из VehicleAutoGearbox

	protected:
		IAutoGearboxState( AutoGearbox& aG );

		AutoGearbox& aGearbox;
	};
	//------------------------------------------------------------
}
}
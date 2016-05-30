#pragma once
#include "IGearbox.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class MechGearbox;
	class AutoGearbox;
	//------------------------------------------------------------
	class IGearboxControl : public IGearbox
	{
	public:
		DEFINE_CLASS_SHARED_PTR(IGearboxControl)
		//--------------------------------//
		virtual ~IGearboxControl()
		{
		}
		//Интерфейс IGearbox
		virtual bool isValid()const{ return gType != GT_NOT_DEFINED; }
		virtual IGearbox::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE ) = 0;
		//Интерфейс IGearbox

		GEARBOX_TYPE GetGType()const{ return gType; }

	protected:
		IGearboxControl( Gearbox& g );
		
	private:
		IGearboxControl();
		
		GEARBOX_TYPE gType;
	};
	//------------------------------------------------------------
	class IMechGearboxControl : public IGearboxControl
	{
		friend class MechGearbox;
	public:
		DEFINE_CLASS_SHARED_PTR(IMechGearboxControl)
		//--------------------------------//		
		virtual ~IMechGearboxControl()
		{
		}

		//Интерфейс IGearbox
		virtual IGearbox::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE );
		//Интерфейс IGearbox

		bool ShiftToNeutral();
		bool ShiftToReverse( int gearIndex = 1 );//1 is first reverse, //2 is second reverse
		bool ShiftTo( int gearIndex );//0 is neutral, //negatives are reverse, //positives are direct
		
	protected:
		IMechGearboxControl( MechGearbox& mG );

		MechGearbox& mGearbox;
	};
	//------------------------------------------------------------
	class IAutoGearboxControl : public IGearboxControl
	{
		friend class AutoGearbox;
	public:
		DEFINE_CLASS_SHARED_PTR(IAutoGearboxControl)
		//--------------------------------//
		virtual ~IAutoGearboxControl()
		{
		}

		//Интерфейс IGearbox
		virtual IGearbox::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE );
		//Интерфейс IGearbox
		
		//интерфейс из класса VehicleAutoGearbox
		bool SwitchMode( AUTO_GEARBOX_MODE );
		bool ModeUp();
		bool ModeDown();
		bool ModeAlt();
		bool PrepareModeUp();
		bool PrepareModeDown();
		bool PrepareModeAlt();
		bool PrepareMode( AUTO_GEARBOX_MODE );
		void UndoPrepareMode();
		//интерфейс из класса VehicleAutoGearbox

	protected:
		IAutoGearboxControl( AutoGearbox& aG );
		
		AutoGearbox& aGearbox;
	};
	//------------------------------------------------------------
}
}
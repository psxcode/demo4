#pragma once
#include "Vehicle\Abstract\mechanics_enums.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class GearboxController;
	//------------------------------------------------------------
	class IGearboxController
	{
	public:
		DEFINE_CLASS_SHARED_PTR(IGearboxController)
		//--------------------------------//
		virtual ~IGearboxController()
		{
		}

		inline GEARBOX_INTERFACE_TYPE getGearboxInterfaceType()const{ return gearboxIT; }

	protected:
		IGearboxController( GEARBOX_INTERFACE_TYPE gIT )
			: gearboxIT( gIT )
		{
		}

	private:
		IGearboxController( const IGearboxController& );
		IGearboxController& operator=( const IGearboxController& );

		GEARBOX_INTERFACE_TYPE gearboxIT;
	};
	//------------------------------------------------------------
}
}
#pragma once
#include "mechanics_enums.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class Gearbox;
	//------------------------------------------------------------
	class IGearbox
	{
	public:
		DEFINE_CLASS_SHARED_PTR(IGearbox)
		//--------------------------------//
		virtual ~IGearbox()
		{
		}
		virtual IGearbox::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE ) = 0;
		inline GEARBOX_INTERFACE_TYPE GetIType()const{ return iType; }

	protected:
		IGearbox( GEARBOX_INTERFACE_TYPE t, Gearbox& g )
			: iType( t )
			, baseGearbox( g )
		{
		}

		Gearbox& baseGearbox;

	private:
		IGearbox();
		IGearbox( const IGearbox& );
		IGearbox& operator=( const IGearbox& );

		GEARBOX_INTERFACE_TYPE iType;
	};
	//------------------------------------------------------------
}
}
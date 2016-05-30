#pragma once
#include "pedal_enums.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class IPedal
	{
	public:
		DEFINE_CLASS_SHARED_PTR(IPedal)
		//--------------------------------//
		virtual ~IPedal()
		{
		}
		virtual shared_ptr GetInterface( PEDAL_INTERFACE_TYPE ) = 0;

		PEDAL_INTERFACE_TYPE GetIType()const{ return iType; }

	protected:
		IPedal( PEDAL_INTERFACE_TYPE type )
			: iType( type )
		{
		}

	private:
		PEDAL_INTERFACE_TYPE iType;
	};
	//------------------------------------------------------------
}
}
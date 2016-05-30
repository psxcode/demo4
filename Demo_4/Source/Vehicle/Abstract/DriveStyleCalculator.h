#pragma once

#include "IPedalState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class DriveStyleCalculator
	{
	public:
		//--------------------------------//
		struct InitData
		{
			virtual ~InitData(){}
			bool isValid()const{ return !iStyle.isNull(); }

			IPedalState::const_shared_ptr iStyle;
		};
		//--------------------------------//
		virtual ~DriveStyleCalculator(){}

	protected:
		inline float Calculate( float calmValue, float agrValue )const {
			return calmValue + ( ( agrValue - calmValue ) * iStyle->GetCurrentValue() );
		}

		//Текущий стиль( стиль реализован как педаль )
		IPedalState::const_shared_ptr iStyle;
	};
	//------------------------------------------------------------
}
}
#pragma once

#include "Core\RangeVar.h"
#include "mechanics_enums.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class ThrottleControlParams
	{
	public:
		DEFINE_CLASS_SHARED_PTR(ThrottleControlParams)
		//--------------------------------//
		ThrottleControlParams();
		bool isValid()const;

		string_traits::str name;
		Core::puFloat throttleFeed;
		Core::puFloat idleMark;
		Core::puFloat idleRange;
		THROTTLE_CONTROL_TYPE tcsType;
	};
	//------------------------------------------------------------
}
}
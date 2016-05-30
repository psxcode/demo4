#include "VehicleTypes.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include <OgreStringConverter.h>

namespace Demo {
namespace Vehicle {
//------------------------------------------------------------
string_traits::cstr VehicleOSNNames::WHEEL_NODE = "WheelNode";
string_traits::cstr VehicleOSNNames::IRON_WHEEL = "IronWheel";
string_traits::cstr VehicleOSNNames::RUBBER_WHEEL = "RubberWheel";
string_traits::cstr VehicleOSNNames::BRAKE_BLOCK = "BrakeBlock";
string_traits::cstr VehicleOSNNames::SPTR = "_";
string_traits::cstr VehicleOSNNames::AXLE = "Ax";
string_traits::cstr VehicleOSNNames::FRONT = "Fr";
string_traits::cstr VehicleOSNNames::BACK = "Bk";
string_traits::cstr VehicleOSNNames::LEFT = "Le";
string_traits::cstr VehicleOSNNames::RIGHT = "Ri";
//------------------------------------------------------------
string_traits::outcopy VehicleOSNNames::ComposeWheelNodeName(unsigned int axleIndex, string_traits::in sideStr )
{
	return string_traits::str(WHEEL_NODE + SPTR + AXLE + Ogre::StringConverter::toString( axleIndex ) + SPTR + sideStr);
}
//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
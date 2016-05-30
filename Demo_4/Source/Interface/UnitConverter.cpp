#include "UnitConverter.h"


namespace Demo
{
	const double UnitConverter::RadPStoRPS	= 1.0 / Ogre::Math::PI * 2.0;
	const double UnitConverter::RadPStoRPM	= 60.0 / Ogre::Math::PI * 2.0;
	const double UnitConverter::RPStoRadPS	= Ogre::Math::PI * 2.0;
	const double UnitConverter::RPMtoRadPS	= Ogre::Math::PI * 2.0 / 60.0;
	const float UnitConverter::MStoKMH		= 3.6f;
	const float UnitConverter::KMHtoMS		= 1.0f / UnitConverter::MStoKMH;
}
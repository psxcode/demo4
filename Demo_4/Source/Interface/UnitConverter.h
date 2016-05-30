#pragma once
#include <OgreVector3.h>
#include <OgreMath.h>
#include "System/DistanceUnitInterface.h"

namespace Demo
{
class UnitConverter
{
public:
	static inline float UnitsToDistance( float units ){ return units * DistanceUnitInterface::_coeffInv; }
	static inline Ogre::Vector3 UnitsToDistance(const Ogre::Vector3& units) { return units * DistanceUnitInterface::_coeffInv; };
	static inline float DistanceToUnits( float distance ){ return distance * DistanceUnitInterface::_coeff; }
	static inline Ogre::Vector3 DistanceToUnits( const Ogre::Vector3& distance){ return distance * DistanceUnitInterface::_coeff; };

	static const double RadPStoRPS;
	static const double RadPStoRPM;
	static const double RPMtoRadPS;
	static const double RPStoRadPS;
	static const float MStoKMH;
	static const float KMHtoMS;
};
}
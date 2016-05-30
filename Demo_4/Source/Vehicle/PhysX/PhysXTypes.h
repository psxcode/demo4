#pragma once
#include <NxWheelShapeDesc.h>
#include <OgreString.h>
#include "Vehicle\Abstract\VehicleTypes.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	struct PhysXSuspensionDesc
	{
		PhysXSuspensionDesc()
			: suspensionTravel(0)
		{
		}

		NxReal suspensionTravel;
		NxSpringDesc suspension;
	};
	//------------------------------------------------------------
	struct PhysXTireDesc
	{
		NxTireFunctionDesc longFunction;
		NxTireFunctionDesc latFunction;
	};
	//------------------------------------------------------------
	struct PhysXVehicleDesc
	{
		//--------------------------------//
		struct PhysXAxleDesc
		{
			PhysXAxleDesc()
				: index(0)
			{
			}

			unsigned int index;
			Ogre::String leftSusName;
			Ogre::String rightSusName;
		};
		//--------------------------------//
		typedef std::vector< PhysXAxleDesc > AxleArray;
		//--------------------------------//
		Ogre::String vehicleName;
		unsigned int solverCount;
		AxleArray axles;
	};
	//------------------------------------------------------------
	class PhysXVehicleWheelAxle : public VehicleWheelAxle
	{
	public:
		//--------------------------------//
		class PhysXWheelNode : public VehicleWheelAxle::WheelNode
		{
		};
		//--------------------------------//
		friend class PhysXVehicle;
		//--------------------------------//
	};
	//------------------------------------------------------------
	//------------------------------------------------------------
}
}
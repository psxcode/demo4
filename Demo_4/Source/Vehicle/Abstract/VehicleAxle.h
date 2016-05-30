#pragma once
#include "WheelNode.h"
#include "Core\RangeVar.h"
#include <OgreVector3.h>

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class VehicleAxle
	{
	public:
		DEFINE_CLASS_SHARED_PTR(VehicleAxle)
		//--------------------------------//
		VehicleAxle()
			: position( Ogre::Vector3::ZERO )
			, maxSteerAngle(0)
			, activeSteerAngle(0)
			, index(0)
			, _isDriven(0)
		{
		}
		virtual ~VehicleAxle()
		{
		}

		virtual WheelNode::shared_ptr GetLeftWheelNode() = 0;
		virtual WheelNode::shared_ptr GetRightWheelNode() = 0;
		virtual void SetSteerValue( const Core::npuFloat& value ) = 0;

		inline float getSteerAngle()const{ return activeSteerAngle; }
		inline unsigned int getIndex()const{ return index; }
		inline Ogre::Vector3 getPosition()const{ return position; }
		inline float getMaxSteerAngle()const{ return maxSteerAngle; }
		inline bool isDriven()const{ return _isDriven; }

	protected:
		void CloseVehicleAxle()
		{
			index = 0;
			position = Ogre::Vector3::ZERO;
			maxSteerAngle = 0;
			activeSteerAngle = 0;
		}

		unsigned int index;
		Ogre::Vector3 position;
		float maxSteerAngle;
		float activeSteerAngle;
		Core::puFloat brakeMaxPressValue;
		bool _isDriven;
	};
	//------------------------------------------------------------
}
}
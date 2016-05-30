#pragma once
#include "Vehicle\Abstract\VehicleAxle.h"
#include "PhysXWheelNode.h"
#include "Interface/UnitConverter.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class PhysXVehicleAxle : public VehicleAxle
	{
		friend class PhysXVehicle;
	public:
		DEFINE_CLASS_SHARED_PTR(PhysXVehicleAxle)
		//--------------------------------//
		virtual ~PhysXVehicleAxle(){ClosePhysXVehicleAxle();}
		virtual WheelNode::shared_ptr GetLeftWheelNode(){ return leftWheelNode; }
		virtual WheelNode::shared_ptr GetRightWheelNode(){ return rightWheelNode; }
		virtual void SetSteerValue( const Core::npuFloat& value )
		{
			this->activeSteerAngle = - value.get() * this->maxSteerAngle;
			leftWheelNode->wheelShape->setSteerAngle( this->activeSteerAngle * (float)UnitConverter::DegToRad );
			rightWheelNode->wheelShape->setSteerAngle( this->activeSteerAngle * (float)UnitConverter::DegToRad );
		}
		void ClosePhysXVehicleAxle()
		{
			if( leftWheelNode != 0 )leftWheelNode = 0;
			if( rightWheelNode != 0 )rightWheelNode = 0;
			CloseVehicleAxle();
		}

	protected:
		PhysXVehicleAxle()
		{
		}

		PhysXWheelNode::shared_ptr leftWheelNode;
		PhysXWheelNode::shared_ptr rightWheelNode;

	private:
		PhysXVehicleAxle( const PhysXVehicleAxle& );
		PhysXVehicleAxle& operator=( const PhysXVehicleAxle& );
	};
	//------------------------------------------------------------
}
}
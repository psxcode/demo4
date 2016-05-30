#pragma once
#include "Vehicle\Abstract\WheelNode.h"
#include "Vehicle\Abstract\VehicleTypes.h"
#include "PhysXWheelNodeParams.h"

namespace Demo
{
namespace Vehicle
{	
	//------------------------------------------------------------
	class PhysXWheelNode : public WheelNode
	{
		friend class PhysXVehicleAxle;
		friend class PhysXVehicle;
	public:
		DEFINE_CLASS_SHARED_PTR(PhysXWheelNode)
		//--------------------------------//
		typedef shared_ptr_traits< NxWheelShapeDesc >::type NxWheelShapeDescPtr;
		//--------------------------------//
		struct PhysXWheelNodeInitData
		{
			PhysXWheelNodeInitData()
				: wheelNodeOSN(0)
				, side( VS_NOT_DEFINED )
			{
			}
			PhysXWheelNodeParams::const_shared_ptr physxWheelNodeParams;
			Ogre::SceneNode* wheelNodeOSN;
			VEHICLE_SIDE side;
		};
		//--------------------------------//
		PhysXWheelNode()
			: wheelShape(0)
		{
		}
		virtual ~PhysXWheelNode();

		//Mechanics interface
		virtual float OperateMechanics( float torque, float drag );
		virtual MechanicsParams::shared_ptr CreateMechanicsParams()const{ return CreatePhysXWheelNodeParams(); }
		virtual bool isMechanicsChainValid()const;
		//Mechanics interface

		bool Initialize( const PhysXWheelNodeInitData& );
		void ClosePhysXWheelNode();

		PhysXWheelNodeParams::shared_ptr CreatePhysXWheelNodeParams()const;

	protected:
		void calcDragWithBrake();
		void calcDragNoBrake();

		//Using fast version without virtual operator()
		delegate_traits::del0<void>::type dragCalcDel;

		NxWheelShape* wheelShape;
		NxWheelShapeDescPtr wheelShapeDesc;
		PhysXWheelParams::shared_ptr rubberWheel;
		PhysXWheelParams::shared_ptr ironWheel;
		PhysXSuspensionDesc::shared_ptr susDesc;

	private:
		PhysXWheelNode( const PhysXWheelNode& );
		PhysXWheelNode& operator=( const PhysXWheelNode& );
	};
	//------------------------------------------------------------
}
}
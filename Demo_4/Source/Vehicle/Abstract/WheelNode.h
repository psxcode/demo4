#pragma once
#include "Mechanics.h"
#include "BrakeBlock.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class WheelNode : public Mechanics
	{
	public:
		DEFINE_CLASS_SHARED_PTR(WheelNode)
		//--------------------------------//
		virtual ~WheelNode(){}

		//Mechanics interface
		virtual bool ConnectChildMech( Mechanics::traits::child_mech_in, int = 0 ) { return false; }
		virtual void DisconnectChildMech( int = 0 ) {}
		virtual void DisconnectChildMech( Mechanics::traits::child_mech_in ) {}
		virtual IMechanicsState::shared_ptr GetMechanicsStateInterface() { if(shared_ptr_null(iState)) iState.bind(new IMechanicsState(*this)); return iState; }

		virtual int GetMechSlotCount()const { return 0; }
		virtual Mechanics::traits::child_mech_type GetChildMech( int = 0 ) { return Mechanics::traits::child_mech_type(0); }
		//Mechanics interface

	protected:
		WheelNode()
			: Mechanics( MT_WHEEL_NODE )
			, rubberWheelIncShift(0)
			, ironWheelIncShift(0)
			, wheelNodeOSN(0)
			, ironWheelOSN(0)
			, rubberWheelOSN(0)
		{
		}
		void CloseWheelNode()
		{
			rubberWheelIncShift = 0;
			ironWheelIncShift = 0;
			brakeBlock.reset();
			iState.reset();
			if( ironWheelOSN != 0 ) System::SceneParamsCollection::DestroySceneNodeAndObjects( ironWheelOSN );
			rubberWheelOSN = 0;
			wheelNodeOSN = 0;
			CloseMechanics();
		}
		float rubberWheelIncShift;
		float ironWheelIncShift;
		BrakeBlock::shared_ptr brakeBlock;
		IMechanicsState::shared_ptr iState;
		
		/*
		Принимается от Vehicle
		*/
		Ogre::SceneNode* wheelNodeOSN;
		/*
		*/
		Ogre::SceneNode* ironWheelOSN;
		/*
		*/
		Ogre::SceneNode* rubberWheelOSN;

	private:
		WheelNode( const WheelNode& );
		WheelNode& operator=( const WheelNode& );
	};
	//------------------------------------------------------------
}
}
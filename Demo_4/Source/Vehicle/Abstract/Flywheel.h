#pragma once
#include "Mechanics.h"
#include "FlywheelParams.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class Flywheel : public Mechanics
	{
	public:
		DEFINE_CLASS_SHARED_PTR(Flywheel)
		//--------------------------------//
		class FlywheelInitData
		{
		public:
			FlywheelParams::const_shared_ptr flywheelParams;
		};
		//--------------------------------//
		static const float ILimit;
		//--------------------------------//
		Flywheel()
			: Mechanics( MT_FLYWHEEL )
		{
		}
		virtual ~Flywheel() {CloseMechanics();}

		//Mechanics interface
		virtual void CloseMechanics();
		virtual float OperateMechanics( float, float ) {return radPS;}
		virtual MechanicsParams::shared_ptr CreateMechanicsParams()const { return CreateFlywheelParams(); }
		virtual bool isMechanicsChainValid()const { return false; }
		virtual IMechanicsState::shared_ptr GetMechanicsStateInterface();

		virtual bool ConnectChildMech( traits::child_mech_in, int slotIndex = 0 ) { return false; }
		virtual void DisconnectChildMech( int slotIndex = 0 ) {}
		virtual void DisconnectChildMech( traits::child_mech_in ) {}

		virtual int GetMechSlotCount()const { return 0; }
		virtual traits::child_mech_type GetChildMech( int slotIndex = 0 ) { return traits::child_mech_type(0); }
		//Mechanics interface

		virtual FlywheelParams::shared_ptr CreateFlywheelParams()const;
		bool Initialize( const FlywheelInitData& );
		
		//IFlywheelState interface
		inline float getFlywheelInertia()const { return inertia; }
		inline float getFlywheelMass()const { return mass; }
		inline float getFlywheelRadius()const { return radius; }
		//IFlywheelState interface

	protected:
		float mass;
		float radius;
		float inertia;

		IMechanicsState::shared_ptr iState;

	private:
		Flywheel( const Flywheel& );
		Flywheel& operator=( const Flywheel& );
	};
	//------------------------------------------------------------
}
}
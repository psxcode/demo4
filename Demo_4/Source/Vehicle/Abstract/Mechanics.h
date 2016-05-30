#pragma once
#if DBS_LM_ABSTRACT_VEHICLE

#include "MechanicsParams.h"
#include "IMechanicsState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class Mechanics
	{
	public:
		DEFINE_CLASS_SHARED_PTR(Mechanics)
		//--------------------------------//
		class traits
		{
		public:
			typedef Mechanics::shared_ptr child_mech_type;
			typedef type_traits< traits::child_mech_type >::cref child_mech_in;
			typedef type_traits< Mechanics >::ptr parent_mech_type;
			typedef type_traits< parent_mech_type >::cref parent_mech_in;
			typedef delegate_traits::composite0::traits::del_in del_in;
		};
		//--------------------------------//
		virtual ~Mechanics() { CloseMechanics(); }
		
		virtual float OperateMechanics( float torque, float drag ) = 0;
		virtual MechanicsParams::shared_ptr CreateMechanicsParams()const = 0;
		virtual bool isMechanicsChainValid()const = 0;
		virtual IMechanicsState::shared_ptr GetMechanicsStateInterface() = 0;

		//Connect/Disconnect
		virtual bool ConnectChildMech( traits::child_mech_in child, int slotIndex = 0 ) = 0;
		virtual void DisconnectChildMech( int slotIndex = 0 ) = 0;
		virtual void DisconnectChildMech( traits::child_mech_in childMech ) = 0;
		
		virtual int GetMechSlotCount()const = 0;
		virtual traits::child_mech_type GetChildMech( int slotIndex = 0 ) = 0;
		//Connect/Disconnect

		virtual void CloseMechanics()
		{
			dragLookup.reset();
			maxDrag = 0;
			nameMechanics.clear();
			torque = 0;
			drag = 0;
			radPS = 0;
			initializedMechanics = false;
		}
		virtual void DisconnectFromParentMech(){ if( parentMechanics != 0 )parentMechanics->DisconnectChildMech( traits::child_mech_type(this) ); }
		
		inline MECHANICS_TYPE getTypeMechanics()const{ return mechType; }
		inline bool isInitializedMechanics()const{ return initializedMechanics; }
		inline string_traits::outref getNameMechanics()const{ return nameMechanics; }
		inline float getTorque()const{ return torque; }
		inline float getDrag()const{ return drag; }
		inline float getRadPS()const{ return radPS; }
		inline traits::parent_mech_in getParentMechanics(){ return parentMechanics; }

	protected:
		Mechanics( MECHANICS_TYPE t )
			: mechType( t )
			, maxDrag(0)
			, initializedMechanics( false )
			, torque(0)
			, drag(0)
			, radPS(0)
		{
		}

		virtual bool ConnectingToParentMech( traits::parent_mech_in parent )
		{
			if( !parent )return false;

			if( parentMechanics )
			{
				if( parentMechanics == parent )return true;
				else return false;
			}

			parentMechanics = parent;
			return true;
		}
		virtual void DisconnectingFromParentMech(){	parentMechanics = 0; }

		inline bool callConnectingToParent( traits::child_mech_in child ){ return child->ConnectingToParentMech( this ); }
		inline void callDisconnectingFromParent( traits::child_mech_in child ){ child->DisconnectingFromParentMech(); }
		
		//MechanicsParams
		string_traits::str nameMechanics;
		float maxDrag;
		Core::OgreRealVBV::shared_ptr dragLookup;
		//MechanicsParams

		traits::parent_mech_type parentMechanics;
		//Посчитанный в последнем Operate Крутящий момент
		float torque;
		//Посчитанный в последнем Operate Тормозящий момент
		float drag;
		//Полученные  в последнем Operate Обороты ( радиан в секунду )
		float radPS;

		bool initializedMechanics;

	private:
		Mechanics( const Mechanics& );
		Mechanics& operator=( const Mechanics& );

		MECHANICS_TYPE mechType;
	};
	//------------------------------------------------------------
}
}
#endif
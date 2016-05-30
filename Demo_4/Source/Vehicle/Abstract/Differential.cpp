#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "Differential.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	bool Differential::Initialize( const DifferentialInitData &data )
	{
		if( isInitializedMechanics() )return false;
		if( shared_ptr_null(data.diffParams) )return false;

		nameMechanics = data.diffParams->nameMechanics;
		maxDrag = data.diffParams->maxDrag;
		if( shared_ptr_valid(data.diffParams->dragLookup) )
			dragLookup = data.diffParams->dragLookup->CreateCopyPtr();

		operateDel.bind( this, &Differential::operateNoConnection );
		diffRatio = data.diffParams->diffRatio;
		torqueMult = diffRatio * 0.5f;

		return initializedMechanics = true;
	}
	//------------------------------------------------------------
	void Differential::CloseDifferential()
	{
		shared_ptr_reset(leftNode);
		shared_ptr_reset(rightNode);

		diffRatio = 0;
		availableModes = 0;
		firstNodeRatio = 0;
		torqueMult = 0;
		operateDel.clear();
		shared_ptr_reset(iState);
		Mechanics::CloseMechanics();
	}
	//------------------------------------------------------------
	float Differential::OperateMechanics( float t, float d )
	{
		torque = t;
		drag = d;
		operateDel();
		return radPS;
	}
	//------------------------------------------------------------
	DifferentialParams::shared_ptr Differential::CreateDifferentialParams() const
	{
		DifferentialParams::shared_ptr diff( new DifferentialParams() );

		//Параметры MechanicsParams
		diff->nameMechanics = nameMechanics;
		diff->maxDrag = maxDrag;
		if( shared_ptr_valid(dragLookup) )
			diff->dragLookup = dragLookup->CreateCopyPtr();

		//Параметры DifferentialParams
		diff->availableModes = availableModes;
		diff->diffRatio = diffRatio;
		diff->firstNodeRatio = firstNodeRatio;

		return diff;
	}
	//------------------------------------------------------------
	bool Differential::ConnectChildMech( traits::child_mech_in child, int index )
	{
		if( shared_ptr_null(child) || !child->isInitializedMechanics() )return false;

		//Куда присоединить
		if( index == 0 )//Присоединение на левую сторону
		{
			//Если уже присоединено
			if( shared_ptr_valid(leftNode) )
			{
				//Повторное присоединение этого же узла
				if( leftNode == child )return true;
				else return false;
			}
			//Левый узел пуст, но к правому присоединён этот же объект
			if( child == rightNode )return false;
			//Узел отказывается присоединится
			if( !callConnectingToParent( child ) )return false;
			//Есть присоединение
			leftNode = child;
			//Если оба узла готовы, меняем логику
			if( shared_ptr_valid(leftNode) && shared_ptr_valid(rightNode) )
				operateDel.bind( this, &Differential::operateWithConnection );
			return true;
		}
		else if( index == 1 )//Присоединение на правую сторону
		{
			//Если уже присоединено
			if( shared_ptr_valid(rightNode) )
			{
				//Повторное присоединение этого же узла
				if( rightNode == child )return true;
				else return false;
			}
			//Правый узел пуст, но к левому присоединён этот же объект
			if( child == leftNode )return false;
			//Узел отказывается присоединится
			if( !callConnectingToParent( child ) )return false;
			//Есть присоединение
			rightNode = child;
			//Если оба узла готовы, меняем логику
			if( shared_ptr_valid(leftNode) && shared_ptr_valid(rightNode) )
				operateDel.bind( this, &Differential::operateWithConnection );
			return true;
		}
		//Неправильный индекс
		else return false;
	}
	//------------------------------------------------------------
	void Differential::DisconnectChildMech( int index )
	{
		if( index == 0 )
		{
			if( shared_ptr_valid(leftNode) ) callDisconnectingFromParent( leftNode );
			shared_ptr_reset(leftNode);
			operateDel.bind( this, &Differential::operateNoConnection );
		}
		else if( index == 1 )
		{
			if( shared_ptr_valid(rightNode) ) callDisconnectingFromParent( rightNode );
			shared_ptr_reset(rightNode);
			operateDel.bind( this, &Differential::operateNoConnection );
		}
	}
	//------------------------------------------------------------
	void Differential::DisconnectChildMech( traits::child_mech_in childMech )
	{
		if( leftNode == childMech )
		{
			DisconnectChildMech( 0 );
		}
		else if( rightNode == childMech )
		{
			DisconnectChildMech( 1 );
		}
	}
	//------------------------------------------------------------
	Differential::traits::child_mech_type Differential::GetChildMech( int index )
	{
		if( index == 0 )
			return leftNode;
		else if( index == 1 )
			return rightNode;
		else return Differential::traits::child_mech_type(0);
	}
	//------------------------------------------------------------
	bool Differential::isMechanicsChainValid() const
	{
		return
			leftNode != 0 &&
			rightNode != 0 &&
			leftNode->isMechanicsChainValid() &&
			rightNode->isMechanicsChainValid();
	}
	//------------------------------------------------------------
	void Differential::operateWithConnection()
	{
		float outTorque = torque * torqueMult;
		float outDrag = drag * torqueMult;
		radPS = (leftNode->OperateMechanics( outTorque, outDrag ) + rightNode->OperateMechanics( outTorque, outDrag )) * torqueMult;
	}
	//------------------------------------------------------------
	void Differential::operateNoConnection()
	{
		radPS = 0;
	}
	//------------------------------------------------------------
	IMechanicsState::shared_ptr Differential::GetMechanicsStateInterface()
	{
		if(shared_ptr_null(iState)) iState.bind( new IMechanicsState( *this ) );
		return iState;
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
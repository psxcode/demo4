#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "Flywheel.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	const float Flywheel::ILimit = 0.00001f;
	//------------------------------------------------------------
	bool Flywheel::Initialize( const FlywheelInitData& data )
	{
		if( isInitializedMechanics() )return false;
		if( shared_ptr_null(data.flywheelParams) )return false;

		//Базовые параметры
		nameMechanics = data.flywheelParams->nameMechanics;
		maxDrag = data.flywheelParams->maxDrag;
		if( shared_ptr_valid(data.flywheelParams->dragLookup) )
			dragLookup = data.flywheelParams->dragLookup->CreateCopyPtr();
		
		//Собственные параметры
		mass = data.flywheelParams->mass;
		radius = data.flywheelParams->radius;
		inertia = mass * radius * radius / 2;
		if( inertia < ILimit )
			return false;

		return initializedMechanics = true;
	}
	//------------------------------------------------------------
	void Flywheel::CloseMechanics()
	{
		mass = 0;
		radius = 0;
		inertia = 0;
		iState.reset();
		Mechanics::CloseMechanics();
	}
	//------------------------------------------------------------
	FlywheelParams::shared_ptr Flywheel::CreateFlywheelParams()const
	{
		FlywheelParams::shared_ptr newFlywheel( new FlywheelParams() );

		//Базовые параметры
		newFlywheel->nameMechanics = nameMechanics;
		newFlywheel->maxDrag = maxDrag;
		if( shared_ptr_valid(dragLookup) )
			newFlywheel->dragLookup = dragLookup->CreateCopyPtr();

		//Свои параметры
		newFlywheel->mass = mass;
		newFlywheel->radius = radius;

		return newFlywheel;
	}
	//------------------------------------------------------------
	IMechanicsState::shared_ptr Flywheel::GetMechanicsStateInterface()
	{
		if(shared_ptr_null(iState)) iState.bind( new IMechanicsState( *this ) );
		return iState;
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
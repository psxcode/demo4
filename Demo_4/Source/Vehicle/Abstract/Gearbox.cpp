#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "Gearbox.h"
#include "Flywheel.h"
#include "Differential.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	const float Gearbox::timeLimit = 0.01f;
	//------------------------------------------------------------
	void Gearbox::CloseMechanics()
	{
		shiftCompletedOnceCall.clear();
		shiftCompletedTempCall.clear();
		shiftInitiatedOnceCall.clear();
		shiftInitiatedTempCall.clear();
		shiftFailedOnceCall.clear();
		shiftFailedTempCall.clear();

		activeGear = 0;
		targetGear = 0;
		activeRatioMod = 0;
		activeRatioSign = 0;
		directCount = 0;
		reverseCount = 0;
		flywheel.setNull();
		if( shared_ptr_valid(childMechanics) ) callDisconnectingFromParent( childMechanics );
		shared_ptr_reset(childMechanics);
		
		Mechanics::CloseMechanics();
	}
	//------------------------------------------------------------
	float Gearbox::GetRatioAtGear( int gear_index )const
	{
		if( gear_index > 0 )
		{
			if( gear_index > GetDirectGearsCount() )return 0;
			return directRatios[ gear_index ];
		}
		else if( gear_index < 0 )
		{
			gear_index *= -1;
			if( gear_index > GetReverseGearsCount() )return 0;
			return reverseRatios[ gear_index ];
		}
		else
			return 0;
	}
	//------------------------------------------------------------
	GearboxParams::shared_ptr Gearbox::CreateGearboxParams() const
	{
		GearboxParams::shared_ptr gearbox( new GearboxParams() );

		//Параметры AxleNodeParams
		gearbox->maxDrag = maxDrag;
		gearbox->nameMechanics = nameMechanics;
		if( shared_ptr_valid(dragLookup) )
			gearbox->dragLookup = dragLookup->CreateCopyPtr();

		//Параметры GearboxParams
		if( shared_ptr_valid(flywheel) )
			gearbox->flywheel = flywheel->CreateFlywheelParams();
		gearbox->gearboxType = gType;
		gearbox->mechShiftTime = mechShiftTime;
		gearbox->modes = modes;
		gearbox->directRatios = directRatios;
		gearbox->reverseRatios = reverseRatios;

		return gearbox;
	}
	//------------------------------------------------------------
	bool Gearbox::ConnectChildMech( traits::child_mech_in child, int )
	{
		if( child == 0 || !child->isInitializedMechanics() )return false;

		if( childMechanics != 0 )
		{
			if( childMechanics == child )return true;
			else return false;
		}
		if( !callConnectingToParent( child ) )return false;
		childMechanics = child;
		return true;
	}
	//------------------------------------------------------------
	void Gearbox::DisconnectChildMech( int )
	{
		if( shared_ptr_valid(childMechanics) )
			callDisconnectingFromParent( childMechanics );
		shared_ptr_reset(childMechanics);
	}
	//------------------------------------------------------------
	void Gearbox::DisconnectChildMech( traits::child_mech_in child )
	{
		if( shared_ptr_null(child) || shared_ptr_null(childMechanics) || childMechanics != child )return;
		callDisconnectingFromParent( childMechanics );
		shared_ptr_reset(childMechanics);
	}
	//------------------------------------------------------------
	bool Gearbox::isMechanicsChainValid()const
	{
		return isInitializedMechanics() && shared_ptr_valid(childMechanics) ? childMechanics->isMechanicsChainValid() : false;
	}
	//------------------------------------------------------------
	bool Gearbox::HasGear(int gear_index) const
	{
		if( gear_index > 0 ) return gear_index <= (int)directCount;
		else if( gear_index < 0 ) return -gear_index <= (int)reverseCount;
		else return true;
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
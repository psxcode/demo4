#include "DemoStableHeaders.h"
#if DBS_LM_VEHICLE_DRIVER

#include "IGearboxControllerState.h"
#include "MechGearboxController.h"
#include "AutoGearboxController.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	IGearboxControllerState::IGearboxControllerState( GearboxController& GC, GEARBOX_TYPE gT )
		: IGearboxController( GIT_STATE )
		, baseGC( GC )
		, gearboxType( gT )
	{
	}
	//------------------------------------------------------------
	IMechGearboxControllerState::IMechGearboxControllerState( MechGearboxController& MGC )
		: IGearboxControllerState( MGC, GT_MECHANICAL )
		, mGC( MGC )
	{
	}
	//------------------------------------------------------------
	IAutoGearboxControllerState::IAutoGearboxControllerState( AutoGearboxController& AGC )
		: IGearboxControllerState( AGC, GT_AUTOMATIC )
		, aGC( AGC )
	{
	}
	//------------------------------------------------------------
	bool IGearboxControllerState::IsNeutral()const
	{
		return baseGC.IsNeutral();
	}
	//------------------------------------------------------------
	bool IGearboxControllerState::IsDirect()const
	{
		return baseGC.IsDirect();
	}
	//------------------------------------------------------------
	bool IGearboxControllerState::IsReverse() const
	{
		return baseGC.IsReverse();
	}
	//------------------------------------------------------------
	int IMechGearboxControllerState::GetLastGear() const
	{
		return mGC.getLastGear();
	}
	//------------------------------------------------------------
	int IMechGearboxControllerState::GetTargetGear() const
	{
		return mGC.getTargetGear();
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
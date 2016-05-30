#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "IGearboxState.h"
#include "MechGearbox.h"
#include "AutoGearbox.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	IGearboxState::IGearboxState( Gearbox& g )
		: IGearbox( GIT_STATE, g )
		, IMechanicsState( g )
		, gType( GT_NOT_DEFINED )
	{
		gType = g.GetGearboxType();
	}
	//------------------------------------------------------------
	IMechGearboxState::IMechGearboxState( MechGearbox &mG )
		: IGearboxState( mG )
		, mGearbox(mG)
	{
	}
	//------------------------------------------------------------
	IAutoGearboxState::IAutoGearboxState( AutoGearbox &aG )
		: IGearboxState( aG )
		, aGearbox(aG)
	{
	}
	//------------------------------------------------------------
	int IGearboxState::GetActiveGearIndex() const
	{
		return baseGearbox.GetActiveGearIndex();
	}
	//------------------------------------------------------------
	int IGearboxState::GetTargetGearIndex() const
	{
		return baseGearbox.GetTargetGearIndex();
	}
	//------------------------------------------------------------
	float IGearboxState::GetActiveGearRatioMod() const
	{
		return baseGearbox.GetActiveGearRatioMod();
	}
	//------------------------------------------------------------
	float IGearboxState::GetActiveGearRatioSign() const
	{
		return baseGearbox.GetActiveGearRatioSign();
	}
	//------------------------------------------------------------
	float IGearboxState::GetOutRadPS()const
	{
		return baseGearbox.GetOutRadPS();
	}
	//------------------------------------------------------------
	int IGearboxState::GetDirectGearsCount() const
	{
		return baseGearbox.GetDirectGearsCount();
	}
	//------------------------------------------------------------
	int IGearboxState::GetReverseGearsCount() const
	{
		return baseGearbox.GetReverseGearsCount();
	}
	//------------------------------------------------------------
	float IGearboxState::GetMechShiftTime()const
	{
		return baseGearbox.GetMechShiftTime();
	}
	//------------------------------------------------------------
	float IGearboxState::GetRatioAtGear( int gear_index )const
	{
		return baseGearbox.GetRatioAtGear( gear_index );
	}
	//------------------------------------------------------------
	bool IGearboxState::HasGear( int gear_index )const
	{
		return baseGearbox.HasGear( gear_index );
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE IAutoGearboxState::GetModeUp() const
	{
		return aGearbox.GetModeUp();
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE IAutoGearboxState::GetModeDown() const
	{
		return aGearbox.GetModeDown();
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE IAutoGearboxState::GetModeAlt() const
	{
		return aGearbox.GetModeAlt();
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE IAutoGearboxState::GetActiveMode() const
	{
		return aGearbox.GetActiveMode();
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE IAutoGearboxState::GetTargetMode() const
	{
		return aGearbox.GetTargetMode();
	}
	//------------------------------------------------------------
	int IAutoGearboxState::GetAvailableModes() const
	{
		return aGearbox.GetAvailableModes();
	}
	//------------------------------------------------------------
	IGearbox::shared_ptr IGearboxState::GetInterface( GEARBOX_INTERFACE_TYPE type )
	{
		return baseGearbox.GetInterface( type );
	}
	//------------------------------------------------------------
	IGearbox::shared_ptr IMechGearboxState::GetInterface( GEARBOX_INTERFACE_TYPE type )
	{
		return mGearbox.GetInterface( type );
	}
	//------------------------------------------------------------
	IGearbox::shared_ptr IAutoGearboxState::GetInterface( GEARBOX_INTERFACE_TYPE type )
	{
		return aGearbox.GetInterface( type );
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
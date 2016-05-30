#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "IGearboxControl.h"
#include "MechGearbox.h"
#include "AutoGearbox.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	IGearboxControl::IGearboxControl( Gearbox& g )
		: IGearbox( GIT_CONTROL, g )
		, gType( GT_NOT_DEFINED )
	{
		gType = baseGearbox.GetGearboxType();
	}
	//------------------------------------------------------------
	IMechGearboxControl::IMechGearboxControl( MechGearbox& mG )
		: IGearboxControl( mG )
		, mGearbox(mG)
	{
	}
	//------------------------------------------------------------
	IAutoGearboxControl::IAutoGearboxControl( AutoGearbox& aG )
		: IGearboxControl( aG )
		, aGearbox( aG )
	{
	}
	//------------------------------------------------------------
	bool IMechGearboxControl::ShiftTo( int gearIndex )
	{
		return mGearbox.ShiftTo( gearIndex );
	}
	//------------------------------------------------------------
	bool IMechGearboxControl::ShiftToNeutral()
	{
		return mGearbox.ShiftToNeutral();
	}
	//------------------------------------------------------------
	bool IMechGearboxControl::ShiftToReverse( int gearIndex )
	{
		return mGearbox.ShiftToReverse( gearIndex );
	}
	//------------------------------------------------------------
	bool IAutoGearboxControl::SwitchMode( AUTO_GEARBOX_MODE mode )
	{
		return aGearbox.SwitchMode( mode );
	}
	//------------------------------------------------------------
	bool IAutoGearboxControl::ModeUp()
	{
		return aGearbox.ModeUp();
	}
	//------------------------------------------------------------
	bool IAutoGearboxControl::ModeDown()
	{
		return aGearbox.ModeDown();
	}	
	//------------------------------------------------------------
	bool IAutoGearboxControl::ModeAlt()
	{
		return aGearbox.ModeAlt();
	}
	//------------------------------------------------------------
	bool IAutoGearboxControl::PrepareModeUp()
	{
		return aGearbox.PrepareModeUp();
	}
	//------------------------------------------------------------
	bool IAutoGearboxControl::PrepareModeDown()
	{
		return aGearbox.PrepareModeDown();
	}
	//------------------------------------------------------------
	bool IAutoGearboxControl::PrepareModeAlt()
	{
		return aGearbox.PrepareModeAlt();
	}
	//------------------------------------------------------------
	bool IAutoGearboxControl::PrepareMode( AUTO_GEARBOX_MODE mode )
	{
		return aGearbox.PrepareMode( mode );
	}
	//------------------------------------------------------------
	void IAutoGearboxControl::UndoPrepareMode()
	{
		return aGearbox.UndoPrepareMode();
	}
	//------------------------------------------------------------
	IGearbox::shared_ptr IMechGearboxControl::GetInterface( GEARBOX_INTERFACE_TYPE type )
	{
		return mGearbox.GetInterface( type );
	}
	//------------------------------------------------------------
	IGearbox::shared_ptr IAutoGearboxControl::GetInterface( GEARBOX_INTERFACE_TYPE type )
	{
		return aGearbox.GetInterface( type );
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
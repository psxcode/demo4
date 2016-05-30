#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "IGearboxCallback.h"
#include "MechGearbox.h"
#include "AutoGearbox.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	IGearboxCallback::IGearboxCallback( Gearbox& g )
		: IGearbox( GIT_CALLBACK, g )
	{
		gType = g.GetGearboxType();
	}
	//------------------------------------------------------------
	IMechGearboxCallback::IMechGearboxCallback( MechGearbox& mG )
		: IGearboxCallback( mG )
		, mGearbox( mG )
	{
	}
	//------------------------------------------------------------
	IAutoGearboxCallback::IAutoGearboxCallback( AutoGearbox& aG )
		: IGearboxCallback( aG )
		, aGearbox( aG )
	{
	}
	//------------------------------------------------------------
	void IGearboxCallback::AddShiftInitiatedOnceDel(del_in del)
	{
		baseGearbox.AddShiftInitiatedOnceDel( del );
	}
	//------------------------------------------------------------
	void IGearboxCallback::AddShiftInitiatedTempDel(del_in del)
	{
		baseGearbox.AddShiftInitiatedTempDel( del );
	}
	//------------------------------------------------------------
	void IGearboxCallback::AddShiftCompletedOnceDel(del_in del)
	{
		baseGearbox.AddShiftCompletedOnceDel( del );
	}
	//------------------------------------------------------------
	void IGearboxCallback::AddShiftCompletedTempDel(del_in del)
	{
		baseGearbox.AddShiftCompletedTempDel( del );
	}
	//------------------------------------------------------------
	void IGearboxCallback::AddShiftFailedOnceDel(del_in del)
	{
		baseGearbox.AddShiftFailedOnceDel( del );
	}
	//------------------------------------------------------------
	void IGearboxCallback::AddShiftFailedTempDel(del_in del)
	{
		baseGearbox.AddShiftFailedTempDel( del );
	}
	//------------------------------------------------------------
	void IGearboxCallback::RemoveShiftInitiatedOnceDel(del_in del)
	{
		baseGearbox.RemoveShiftInitiatedOnceDel( del );
	}
	//------------------------------------------------------------
	void IGearboxCallback::RemoveShiftInitiatedTempDel(del_in del)
	{
		baseGearbox.RemoveShiftInitiatedTempDel( del );
	}
	//------------------------------------------------------------
	void IGearboxCallback::RemoveShiftCompletedOnceDel(del_in del)
	{
		baseGearbox.RemoveShiftCompletedOnceDel( del );
	}
	//------------------------------------------------------------
	void IGearboxCallback::RemoveShiftCompletedTempDel(del_in del)
	{
		baseGearbox.RemoveShiftCompletedTempDel( del );
	}
	//------------------------------------------------------------
	void IGearboxCallback::RemoveShiftFailedOnceDel(del_in del)
	{
		baseGearbox.RemoveShiftFailedOnceDel( del );
	}
	//------------------------------------------------------------
	void IGearboxCallback::RemoveShiftFailedTempDel(del_in del)
	{
		baseGearbox.RemoveShiftFailedTempDel( del );
	}
	//------------------------------------------------------------
	void IAutoGearboxCallback::AddSwitchInitiatedOnceDel(del_in del)
	{
		aGearbox.AddSwitchInitiatedOnceDel( del );
	}
	//------------------------------------------------------------
	void IAutoGearboxCallback::AddSwitchInitiatedTempDel(del_in del)
	{
		aGearbox.AddSwitchInitiatedTempDel( del );
	}
	//------------------------------------------------------------
	void IAutoGearboxCallback::AddSwitchCompletedOnceDel(del_in del)
	{
		aGearbox.AddSwitchCompletedOnceDel( del );
	}
	//------------------------------------------------------------
	void IAutoGearboxCallback::AddSwitchCompletedTempDel(del_in del)
	{
		aGearbox.AddSwitchCompletedTempDel( del );
	}
	//------------------------------------------------------------
	void IAutoGearboxCallback::AddSwitchFailedOnceDel(del_in del)
	{
		aGearbox.AddSwitchFailedOnceDel( del );
	}
	//------------------------------------------------------------
	void IAutoGearboxCallback::AddSwitchFailedTempDel(del_in del)
	{
		aGearbox.AddSwitchFailedTempDel( del );
	}
	//------------------------------------------------------------
	void IAutoGearboxCallback::RemoveSwitchInitiatedOnceDel(del_in del)
	{
		aGearbox.RemoveSwitchInitiatedOnceDel( del );
	}
	//------------------------------------------------------------
	void IAutoGearboxCallback::RemoveSwitchInitiatedTempDel(del_in del)
	{
		aGearbox.RemoveSwitchInitiatedTempDel( del );
	}
	//------------------------------------------------------------
	void IAutoGearboxCallback::RemoveSwitchCompletedOnceDel(del_in del)
	{
		aGearbox.RemoveSwitchCompletedOnceDel( del );
	}
	//------------------------------------------------------------
	void IAutoGearboxCallback::RemoveSwitchCompletedTempDel(del_in del)
	{
		aGearbox.RemoveSwitchCompletedTempDel( del );
	}
	//------------------------------------------------------------
	void IAutoGearboxCallback::RemoveSwitchFailedOnceDel(del_in del)
	{
		aGearbox.RemoveSwitchFailedOnceDel( del );
	}
	//------------------------------------------------------------
	void IAutoGearboxCallback::RemoveSwitchFailedTempDel(del_in del)
	{
		aGearbox.RemoveSwitchFailedTempDel( del );
	}
	//------------------------------------------------------------
	IGearbox::shared_ptr IMechGearboxCallback::GetInterface( GEARBOX_INTERFACE_TYPE type )
	{
		return mGearbox.GetInterface( type );
	}
	//------------------------------------------------------------
	IGearbox::shared_ptr IAutoGearboxCallback::GetInterface( GEARBOX_INTERFACE_TYPE type )
	{
		return aGearbox.GetInterface( type );
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
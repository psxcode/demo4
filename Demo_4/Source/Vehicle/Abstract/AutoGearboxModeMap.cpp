#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "AutoGearboxModeMap.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	AutoGearboxModeMap::~AutoGearboxModeMap()
	{
		MIter iter = modeSet.begin();
		MIter end_iter = modeSet.end();
		for( ; iter != end_iter; ++iter )
		{
			if( (*iter) )delete (*iter);
		}
		modeSet.clear();
	}
	//------------------------------------------------------------
	AutoGearboxModeMap::Mode::Mode()
		: mode( AGM_NOT_DEFINED )
		, modeUp(0)
		, modeDown(0)
		, modeAlt(0)
	{
	}
	//------------------------------------------------------------
	AutoGearboxModeMap::AutoGearboxModeMap()
		: startMode(0)
		, currentMode(0)
	{
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE AutoGearboxModeMap::GetCurrentMode() const
	{
		if( currentMode ) return currentMode->mode;
		else return AGM_NOT_DEFINED;
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE AutoGearboxModeMap::GetAltMode() const
	{
		if( currentMode && currentMode->modeAlt )
			return currentMode->modeAlt->mode;
		else return AGM_NOT_DEFINED;
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE AutoGearboxModeMap::GetUpMode() const
	{
		if( currentMode && currentMode->modeUp )
			return currentMode->modeUp->mode;
		else return AGM_NOT_DEFINED;
	}
	//------------------------------------------------------------
	AUTO_GEARBOX_MODE AutoGearboxModeMap::GetDownMode() const
	{
		if( currentMode && currentMode->modeDown )
			return currentMode->modeDown->mode;
		else return AGM_NOT_DEFINED;
	}
	//------------------------------------------------------------
	bool AutoGearboxModeMap::MoveAlt()
	{
		if( currentMode && currentMode->modeAlt )
		{
			currentMode = currentMode->modeAlt;
			return true;
		}
		else return false;
	}
	//------------------------------------------------------------
	bool AutoGearboxModeMap::MoveUp()
	{
		if( currentMode && currentMode->modeUp )
		{
			currentMode = currentMode->modeUp;
			return true;
		}
		else return false;
	}
	//------------------------------------------------------------
	bool AutoGearboxModeMap::MoveDown()
	{
		if( currentMode && currentMode->modeDown )
		{
			currentMode = currentMode->modeDown;
			return true;
		}
		else return false;
	}
	//------------------------------------------------------------
	void AutoGearboxModeMap::ResetMove()
	{
		currentMode = startMode;
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
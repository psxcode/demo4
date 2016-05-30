#pragma once
#include "mechanics_enums.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class AutoGearboxModeMap
	{
	public:
		//--------------------------------//
		struct Mode
		{
			Mode();
			AUTO_GEARBOX_MODE mode;

			Mode* modeUp;
			Mode* modeDown;
			Mode* modeAlt;
		};
		//--------------------------------//
		typedef std::set< const AutoGearboxModeMap::Mode* > ModeSet;
		typedef ModeSet::iterator MIter;
		typedef ModeSet::const_iterator cMIter;
		//--------------------------------//

		AutoGearboxModeMap();
		~AutoGearboxModeMap();

		AUTO_GEARBOX_MODE GetCurrentMode()const;
		AUTO_GEARBOX_MODE GetUpMode()const;
		AUTO_GEARBOX_MODE GetDownMode()const;
		AUTO_GEARBOX_MODE GetAltMode()const;

		bool MoveUp();
		bool MoveDown();
		bool MoveAlt();

		void ResetMove();

	protected:
		ModeSet modeSet;
		AutoGearboxModeMap::Mode* startMode;
		AutoGearboxModeMap::Mode* currentMode;
	};
	//------------------------------------------------------------	
}
}
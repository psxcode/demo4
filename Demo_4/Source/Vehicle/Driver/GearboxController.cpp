#include "DemoStableHeaders.h"
#if DBS_LM_VEHICLE_DRIVER

#include "GearboxController.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	void GearboxController::CloseGearboxController()
	{
		shiftStartedDelArray.clear();
		shiftEndedDelArray.clear();
		
		Core::Controller::CloseController();
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
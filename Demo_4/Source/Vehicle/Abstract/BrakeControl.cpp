#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "BrakeControl.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	bool BrakeControl::Initialize( const BrakeControlInitData& data )
	{
		if( isInitializedController() )return false;

		axles = data.brakeAxles;

		setInitializeController();
		return true;
	}
	//------------------------------------------------------------
	void BrakeControl::CloseBrakeControl()
	{
		axles.clear();
			
		brake.Reset();
		Controller::CloseController();
	}
	//------------------------------------------------------------
	void BrakeControl::OperateController()
	{
		brake.Operate();
		AIter iter = axles.begin();
		AIter end_iter = axles.end();
		for( ; iter != end_iter; ++iter )
		{
			Core::puFloat pressVal = (iter->maxPressValue) * brake.Get();
			iter->leftBlock->SetBrakeValue( pressVal );
			iter->rightBlock->SetBrakeValue( pressVal );
		}
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
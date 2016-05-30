#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "SteerControl.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	bool SteerControl::Initialize(const SteerControlInitData &data)
	{
		if( !data.isValid() )return false;

		this->steerAxle = data.steerAxle;

		return true;
	}
	//------------------------------------------------------------
	void SteerControl::OperateController()
	{
		steer.Operate();
		steerAxle->SetSteerValue( steer.Get() );
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
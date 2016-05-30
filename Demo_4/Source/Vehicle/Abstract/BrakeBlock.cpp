#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "BrakeBlock.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	bool BrakeBlock::Initialize(const BrakeBlock::BrakeBlockInitData &data)
	{
		if( data.blockParams.isNull() || !data.blockParams->isValid() )return false;
		name = data.blockParams->name;
		maxBrakeTorque = data.blockParams->maxBrakeTorque;
		if( !data.blockParams->osnParams.isNull() )
		{
			osn = SPC::CreateSceneNode( *data.blockParams->osnParams, static_cast< void* >( this ) );
		}
		return true;
	}
	//------------------------------------------------------------
	void BrakeBlock::CloseBrakeBlock()
	{
		maxBrakeTorque = 0;
		brakeValue = 0;
		activeBrakeTorque = 0;
		if( osn != 0 )SPC::DestroySceneNodeAndObjects( osn );
		CloseOsn();
	}
	//------------------------------------------------------------
	BrakeBlockParams::shared_ptr BrakeBlock::CreateBrakeBlockParams() const
	{
		BrakeBlockParams::shared_ptr p( new BrakeBlockParams() );
		p->maxBrakeTorque = maxBrakeTorque;
		p->name = name;
		if( osn != 0 )p->osnParams = SPC::CreateSceneNodeParams( osn );
		return p;
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
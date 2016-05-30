#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "IMechanicsState.h"
#include "Mechanics.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	float IMechanicsState::getDrag() const
	{
		return baseMechanics.getDrag();
	}
	//------------------------------------------------------------
	float IMechanicsState::getRadPS() const
	{
		return baseMechanics.getRadPS();
	}
	//------------------------------------------------------------
	float IMechanicsState::getTorque() const
	{
		return baseMechanics.getTorque();
	}
	//------------------------------------------------------------
	MECHANICS_TYPE IMechanicsState::getTypeMechanics() const
	{
		return baseMechanics.getTypeMechanics();
	}
	//------------------------------------------------------------
	bool IMechanicsState::isInitializedMechanics() const
	{
		return baseMechanics.isInitializedMechanics();
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
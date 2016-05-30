#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <NxPhysics.h>
#include "OgrePhysXClasses.h"

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
class TriggerReportListener
{
public:
	virtual ~TriggerReportListener() {}
	virtual void onEnter(Actor *trigger, Actor *other) {};
	virtual void onLeave(Actor *trigger, Actor *other) {};
};
//------------------------------------------------------------
class TriggerReport : public NxUserTriggerReport
{
private:
	TriggerReportListener *mListener;

public:
	TriggerReport(TriggerReportListener *listener)
		: mListener(listener)
	{
	}
	virtual ~TriggerReport()
	{
		delete mListener;
	}
	virtual void onTrigger(NxShape &triggerShape, NxShape &otherShape, NxTriggerFlag status) {
		if (triggerShape.getActor().userData && otherShape.getActor().userData)	{
			Actor *trigger = (Actor*)triggerShape.getActor().userData;
			Actor *other = (Actor*)otherShape.getActor().userData;
			if (status & NX_TRIGGER_ON_ENTER) {
				mListener->onEnter(trigger, other);
			} else if (status & NX_TRIGGER_ON_LEAVE) {
				mListener->onLeave(trigger, other);
			}
		}
	}
};
//------------------------------------------------------------
}
}
#endif
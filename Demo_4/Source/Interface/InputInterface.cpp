#include "DemoStableHeaders.h"
#include "InputInterface.h"
#include "System\MainController.h"

namespace Demo
{
//------------------------------------------------------------
InputInterface::shared_ptr InputInterface::getInstance()
{
	return System::MainController::getSingleton().getInputController()->getInputInterface();
}
//------------------------------------------------------------
bool InputInterface::addInputDel(InputRequest const &r)
{
	return System::MainController::getSingleton().getInputController()->addInputDel(r);
}
bool InputInterface::addInputDel(InputRequest::container_t const &request)
{
	bool ok = true;
	for( auto iter = request.begin(), end_iter = request.end(); iter != end_iter; ++iter)
	{
		if( !addInputDel(*iter) ) ok = false;
	}
	return ok;
}
//------------------------------------------------------------
void InputInterface::removeInputDel(InputRequest const &r)
{
	System::MainController::getSingleton().getInputController()->removeInputDel(r);
}
void InputInterface::removeInputDel(InputRequest::container_t const &request)
{
	for( auto iter = request.begin(), end_iter = request.end(); iter != end_iter; ++iter)
	{
		removeInputDel(*iter);
	}
}
//------------------------------------------------------------
}
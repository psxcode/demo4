#include "DemoStableHeaders.h"

#include <Ogre.h>
#include <NxPhysics.h>

// Logic includes
#include "GameObjects/DRTest01.h"
//--------------------------------

#include "MainController.h"

namespace Ogre
{
	template<>
	Demo::System::MainController* Singleton<Demo::System::MainController>::ms_Singleton = 0;
}

namespace Demo {
namespace System {
//------------------------------------------------------------
#if DBS_LM_PHYSX
	NxPhysicsSDK* MainController::ms_pPhysicsSDK = 0;
#endif
//------------------------------------------------------------
/*static*/ MainController* MainController::getSingletonPtr()
{
	return ms_Singleton;
}
/*static*/ MainController& MainController::getSingleton()
{
	_ASSERT( ms_Singleton ); return (*ms_Singleton);
}
//------------------------------------------------------------
MainController::MainController()
	: m_pRenderWindow(0)
{
}
//------------------------------------------------------------
/*virtual*/ MainController::~MainController()
{
	shutdown();
}
//------------------------------------------------------------
bool MainController::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	m_spInput->OperateController();
	OperateController();
	return true;
}
//------------------------------------------------------------
void MainController::windowClosed(Ogre::RenderWindow* rw)
{
	if(rw == m_pRenderWindow)
		Ogre::Root::getSingleton().queueEndRendering();
}
//------------------------------------------------------------
void MainController::eskapeKey()
{
	Ogre::Root::getSingleton().queueEndRendering();
}
//------------------------------------------------------------
void MainController::run()
{
	Ogre::Root *pRoot = getOgreRoot();

	if( !pRoot->showConfigDialog() ) return;

	//initialize Root with auto created window
	m_pRenderWindow = Ogre::Root::getSingleton().initialise(true);

	//Input
	m_spInput.reset(new InputController);
	if(!m_spInput->initialize(m_pRenderWindow)) return;

	//Factories and collections
	m_spFactoryCollection.reset(new FactoryCollectionService);
	if(!m_spFactoryCollection->initialize()) return;

	Ogre::WindowEventUtilities::addWindowEventListener(m_pRenderWindow, this);
	pRoot->addFrameListener(this);

	m_spEscapeDel.reset(new delegate_traits::del0<void>(this, &MainController::eskapeKey));
	m_spInput->addInputDel(InputRequest(m_spEscapeDel, input_traits::IID_ESCAPE_KEY, input_traits::IS_PRESSED));

	if(!prepareLogic()) return;

	pRoot->startRendering();
}
//------------------------------------------------------------
void MainController::shutdown()
{
	shutdownLogic();

	m_spFactoryCollection.reset();
	m_spInput.reset();


#if DBS_LM_PHYSX
	if(ms_pPhysicsSDK)
	{
		ms_pPhysicsSDK->release();
		ms_pPhysicsSDK = 0;
	}
#endif

	if(Ogre::Root::getSingletonPtr())
	{
		Ogre::Root::getSingleton().removeFrameListener(this);
		Ogre::WindowEventUtilities::removeWindowEventListener(m_pRenderWindow, this);
		OGRE_DELETE Ogre::Root::getSingletonPtr();
	}
	m_pRenderWindow = 0;
}
//------------------------------------------------------------
Ogre::Root* MainController::getOgreRoot() throw(Ogre::Exception)
{
	if(Ogre::Root::getSingletonPtr() == 0)
		OGRE_NEW Ogre::Root;
	return Ogre::Root::getSingletonPtr();
}
//------------------------------------------------------------
#if DBS_LM_PHYSX
NxPhysicsSDK* MainController::getNxPhysicsSDK(bool create) throw(Ogre::Exception)
{
	if(!ms_pPhysicsSDK && create)
	{
		ms_pPhysicsSDK = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION );
		if(!ms_pPhysicsSDK)
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "PhysX was not created", "");
	}
	return ms_pPhysicsSDK;
}
#endif
//------------------------------------------------------------
bool MainController::prepareLogic()
{
	//DRTest01 Scenery
	if(!getController<DRTest01>()->initialize()) return false;
	ConnectChildController(getController<DRTest01>().get());
	return true;
}
//------------------------------------------------------------
void MainController::shutdownLogic()
{
	Ogre::MapIterator<traits::controller_cont> iter(controllers);
	while(iter.hasMoreElements())
	{
		iter.getNextRef()->CloseController();
	}
	DisconnectAllChildControllers();
	controllers.clear();
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
}
}
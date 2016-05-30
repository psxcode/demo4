#pragma once
#include <OgreRenderWindow.h>
#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>
#include <OgreScriptCompiler.h>

#if DBS_LM_PHYSX
#include <NxPhysicsSDK.h>
#endif

#include "InputController.h"
#include "FactoryCollectionService.h"

namespace Demo {
namespace System {
//------------------------------------------------------------
class MainController
	: public Core::Controller
	, public Ogre::FrameListener
	, public Ogre::WindowEventListener
	, public Ogre::Singleton<MainController>
{
	friend class Ogre::Singleton<MainController>;
public:
	//--------------------------------//
	class traits : public Core::Controller::traits
	{
	public:
		typedef Demo::map<size_t, Core::Controller::shared_ptr>::type controller_cont;
	};
	//--------------------------------//
	static MainController& getSingleton();
	static MainController* getSingletonPtr();

	MainController();
	virtual ~MainController();

	//Controller interface

	//FrameListener interface
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
	//WindowEventListener interface
	virtual void windowClosed(Ogre::RenderWindow *rw);

	void run();
	void shutdown();

	Ogre::Root* getOgreRoot() throw(Ogre::Exception);
	Ogre::RenderWindow* getOgreRenderWindow() {return m_pRenderWindow;}
	const InputController::shared_ptr& getInputController() {return m_spInput;}
#if DBS_LM_PHYSX
	NxPhysicsSDK* getNxPhysicsSDK(bool create = true) throw(Ogre::Exception);
#endif
	const FactoryCollectionService::shared_ptr& getFactoryCollectionService() {return m_spFactoryCollection;}

protected:
#if DBS_LM_PHYSX
	static NxPhysicsSDK *ms_pPhysicsSDK;
#endif
	Ogre::RenderWindow *m_pRenderWindow;
	InputController::shared_ptr m_spInput;

	// EscapeKey exit - May be removed later
	void eskapeKey();
	delegate_traits::del0<void>::shared_ptr m_spEscapeDel;
	//--------------------------------//
	template<typename T>
	typename T::shared_ptr getController(bool create = true)
	{
		size_t tag = Core::TagHolder<T, Core::Controller>::get();
		auto iter = controllers.find(tag);
		if(iter != controllers.end())
		{
			return shared_ptr_static_cast<T>(iter->second);
		}
		else
		{
			typename T::shared_ptr ptr;
			if(create)
			{
				ptr.reset(new T);
				controllers.insert(std::make_pair(tag, ptr));
			}
			return ptr;
		}
	}
	bool prepareLogic();
	void shutdownLogic();

	FactoryCollectionService::shared_ptr m_spFactoryCollection;
	traits::controller_cont controllers;
	
private:
	MainController( const MainController& );
	MainController& operator=( const MainController& );
};
//------------------------------------------------------------
}
}
#pragma once
#include "DemoPrerequisites.h"

#include <OgreSingleton.h>
#include <OgreFrameListener.h>
#include <OgreScriptCompiler.h>
#include "System\MaterialGeneratorManager.h"

namespace Ogre
{
	class Root;
	class RenderWindow;
	class SceneManager;
}

namespace Demo {
namespace System {
//------------------------------------------------------------
class OgreController : public Ogre::Singleton< OgreController >
{
	friend class Ogre::Singleton< OgreController >;
public:
	//--------------------------------//
	class OgreScriptCompilerListener : public Ogre::ScriptCompilerListener
	{
	public:
		//--------------------------------//
		typedef shared_ptr_traits< OgreScriptCompilerListener >::type shared_ptr;
		typedef shared_ptr_traits< const OgreScriptCompilerListener >::type const_shared_ptr;
		//--------------------------------//
		typedef shared_ptr_traits< Ogre::ScriptCompilerListener >::type ListenerPtr;
		//--------------------------------//
		OgreScriptCompilerListener();
		virtual ~OgreScriptCompilerListener();
		virtual bool handleEvent( Ogre::ScriptCompiler* compiler, Ogre::ScriptCompilerEvent* evt, void* retVal );
		void addListener( ListenerPtr lis ){ listeners.insert( lis ); }
		void removeListener( ListenerPtr lis ){ listeners.erase( lis ); }

	protected:
		//--------------------------------//
		typedef Ogre::set< ListenerPtr >::type Listeners;
		//--------------------------------//
		Listeners listeners;
	};
	//--------------------------------//
	static OgreController& getSingleton();
	static OgreController* getSingletonPtr();
	//--------------------------------//

	bool initRoot();
	bool initWindow();
	void close();
	void operate();

private:
	OgreController();
	virtual ~OgreController();
	OgreController( const OgreController& );
	OgreController& operator=( const OgreController& );

	Ogre::Root* _pRoot;
	Ogre::SceneManager* _pSceneManager;
	Ogre::RenderWindow* _pRenderWindow;

	MaterialGeneratorManager::shared_ptr matGen;
	OgreScriptCompilerListener::shared_ptr ogreSCL;
};
//------------------------------------------------------------
}
}
#include "OgreController.h"

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>

#include "System\OgreSceneManagerInterface.h"
#include "System\OgreRenderWindowInterface.h"
#include "System\FrameTimeInterface.h"
#include "System\SystemInterface.h"
#include "shared_ptr_traits.h"


namespace Demo
{
namespace System
{
//------------------------------------------------------------
class MaterialScriptCompilerListener : public Ogre::ScriptCompilerListener
{
public:
	//--------------------------------//
	typedef shared_ptr_traits< MaterialScriptCompilerListener >::shared_ptr shared_ptr;
	typedef shared_ptr_traits< MaterialScriptCompilerListener >::const_shared_ptr const_shared_ptr;
	//--------------------------------//
	MaterialScriptCompilerListener( MaterialGeneratorManager::shared_ptr mg )
		: matGen( mg )
	{
	}
	virtual ~MaterialScriptCompilerListener()
	{
	}

	virtual bool handleEvent( Ogre::ScriptCompiler* compiler, Ogre::ScriptCompilerEvent* evt, void* retVal )
	{
		if( evt->mType == "preApplyTextureAliases" )
		{
			Ogre::PreApplyTextureAliasesScriptCompilerEvent* preEvt =
				dynamic_cast< Ogre::PreApplyTextureAliasesScriptCompilerEvent* >(evt);
			if( preEvt != 0 )
			{
				return matGen->modifyMaterial( *(preEvt->mMaterial) );
			}
		}
		return false;
	}

protected:
	MaterialGeneratorManager::shared_ptr matGen;
};
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
OgreController::OgreScriptCompilerListener::OgreScriptCompilerListener()
{
	Ogre::ScriptCompilerManager::getSingleton().setListener( this );
}
//------------------------------------------------------------
OgreController::OgreScriptCompilerListener::~OgreScriptCompilerListener()
{
	Ogre::ScriptCompilerManager::getSingleton().setListener(0);
}
//------------------------------------------------------------
bool OgreController::OgreScriptCompilerListener::handleEvent( Ogre::ScriptCompiler* compiler, Ogre::ScriptCompilerEvent* evt, void* retVal )
{
	Listeners::iterator iter = listeners.begin(); Listeners::iterator end_iter = listeners.end();
	for( ; iter != end_iter; ++iter )
	{
		if( (*iter)->handleEvent( compiler, evt, retVal ) )
			return true;
	}
	return false;
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
OgreController::OgreController()
	: _pRoot(0)
	, _pSceneManager(0)
	, _pRenderWindow(0)
{
	_pRoot = OGRE_NEW Ogre::Root();

	if( !_pRoot->restoreConfig() )
		if( !_pRoot->showConfigDialog() )
			return false;

	_pRenderWindow = _pRoot->initialise( true );
	_pSceneManager = _pRoot->createSceneManager( Ogre::ST_GENERIC );

	OgreRenderWindowInterface::_ptr = _pRenderWindow;
	OgreSceneManagerInterface::_ptr = _pSceneManager;

	matGen.bind( new MaterialGeneratorManager() );
	ogreSCL.bind( new OgreScriptCompilerListener() );
	ogreSCL->addListener( OgreScriptCompilerListener::ListenerPtr( new MaterialScriptCompilerListener( matGen ) ) );
}
//------------------------------------------------------------
OgreController::~OgreController()
{
}
//------------------------------------------------------------
bool OgreController::initialize()
{
	_pRoot = OGRE_NEW Ogre::Root();
	
	if( !_pRoot->restoreConfig() )
		if( !_pRoot->showConfigDialog() )
			return false;

	_pRenderWindow = _pRoot->initialise( true );
	_pSceneManager = _pRoot->createSceneManager( Ogre::ST_GENERIC );

	OgreRenderWindowInterface::_ptr = _pRenderWindow;
	OgreSceneManagerInterface::_ptr = _pSceneManager;

	_pRoot->addFrameListener( this );

	matGen.bind( new MaterialGeneratorManager() );
	ogreSCL.bind( new OgreScriptCompilerListener() );
	ogreSCL->addListener( OgreScriptCompilerListener::ListenerPtr( new MaterialScriptCompilerListener( matGen ) ) );

	return true;
}
//------------------------------------------------------------
void OgreController::Close()
{
	initializedController = false;
	ogreSCL.setNull();
	OgreRenderWindowInterface::_ptr = 0;
	OgreSceneManagerInterface::_ptr = 0;
	if( _pRoot )OGRE_DELETE _pRoot;
	_pRoot = 0;
}
//------------------------------------------------------------
void OgreController::OperateController()
{
	Ogre::WindowEventUtilities::messagePump();
	if( !_pRoot->renderOneFrame() )
		SystemInterface::_operate = false;
}
//------------------------------------------------------------
bool OgreController::frameEnded( const Ogre::FrameEvent& evt )
{
	TimeInterface::_frameTime = evt.timeSinceLastFrame;
	return true;
}
//------------------------------------------------------------
}
}
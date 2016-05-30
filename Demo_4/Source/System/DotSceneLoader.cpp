#include "DotSceneLoader.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	DotSceneLoader::DotSceneLoader()
	{
		mScriptPatterns.push_back("*.scn");
	}
	//------------------------------------------------------------
	/*virtual*/ DotSceneLoader::~DotSceneLoader()
	{
	}
	//------------------------------------------------------------
	/*virtual*/ void DotSceneLoader::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
	{
		doc.setNull();
		doc.bind( new TiXmlDocument );
		if( !doc->LoadFile(stream) )
			return;

		loaders->processElement( doc->RootElement() );
	}
	//------------------------------------------------------------
	void DotSceneLoader::addLoader( traits::ldr_in ldr )
	{
		if( doc != 0 && !doc->Error() )
			ldr->processElement(doc->RootElement());
		loaders->addChildLoader(ldr);
	}
	//------------------------------------------------------------
	void DotSceneLoader::removeLoader( traits::ldr_in ldr )
	{
		loaders->removeChildLoader(ldr);
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
}
}
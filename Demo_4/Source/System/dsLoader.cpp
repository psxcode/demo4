#include "DemoStableHeaders.h"
#include "dsLoader.h"
#include <OgreVector3.h>
#include <OgreColourValue.h>

namespace Demo
{
namespace System
{
//------------------------------------------------------------
dsLoaderAuto_CompositeSimple::dsLoaderAuto_CompositeSimple()
{
}
//------------------------------------------------------------
dsLoaderAuto_CompositeSimple::~dsLoaderAuto_CompositeSimple()
{
}
//------------------------------------------------------------
bool dsLoaderAuto_CompositeSimple::processElement( const TiXmlElement *element )
{
	traits::child_ldr_ocit iter( childLoaders );
	while( iter.hasMoreElements() )	{
		if( iter.getNextCRef()->processElement(element) )
			return true;
	}
	return false;
}
//------------------------------------------------------------
dsLoader_BaseInterface::traits::pen_cont_out dsLoaderAuto_CompositeSimple::getProcessElementNames()const
{
	shared_ptr_traits<traits::pen_cont>::type pnn( new traits::pen_cont );
	traits::child_ldr_ocit iter( childLoaders );
	while( iter.hasMoreElements() )	{
		traits::pen_cont_out child_pnn = iter.getNextCRef()->getProcessElementNames();
		pnn->insert( child_pnn->begin(), child_pnn->end() );
	}
	return traits::pen_cont_out( pnn );
}
//------------------------------------------------------------
dsLoaderAuto_CompositeDispatcher::dsLoaderAuto_CompositeDispatcher()
{
}
//------------------------------------------------------------
dsLoaderAuto_CompositeDispatcher::~dsLoaderAuto_CompositeDispatcher()
{
}
//------------------------------------------------------------
bool dsLoaderAuto_CompositeDispatcher::processElement( const TiXmlElement *element )
{
	bool processed = false;
	const TiXmlElement* childElement = 0;
	while( dsIterateChildElements( element, childElement ) ) {
		string_traits::outref childName = childElement->ValueStr();
		traits::child_ldr_cont::iterator iter = childLoaders.find( childName );
		if( iter != childLoaders.end() )
			if( iter->second->processElement(childElement) )
				processed = true;
	}
	return processed;
}
//------------------------------------------------------------
void dsLoaderAuto_CompositeDispatcher::addChildLoader( traits::child_ldr_in ldr )
{
	traits::pen_cont_out names = ldr->getProcessElementNames();
	traits::pen_cont::const_iterator iter = names->begin();
	traits::pen_cont::const_iterator end_iter = names->end();
	for( ; iter != end_iter; ++iter ) {
		findCreateLoaderComposite( **iter )->addChildLoader( ldr );
	}
}
//------------------------------------------------------------
void dsLoaderAuto_CompositeDispatcher::removeChildLoader( traits::child_ldr_in ldr )
{
	traits::child_ldr_ocit iter( childLoaders );
	while(iter.hasMoreElements())
	{
		iter.getNextCRef()->removeChildLoader(ldr);
	}
}
//------------------------------------------------------------
dsLoaderAuto_CompositeInterface::shared_ptr dsLoaderAuto_CompositeDispatcher::findCreateLoaderComposite( string_traits::in nodeName )
{
	traits::child_ldr_cont::iterator iter = childLoaders.find(nodeName);
	if( iter != childLoaders.end() ) {
		return iter->second;
	} else {
		dsLoaderAuto_CompositeInterface::shared_ptr newLoaderComposite(new dsLoaderAuto_CompositeSimple);
		childLoaders.insert( traits::child_ldr_cont::value_type( nodeName, newLoaderComposite ) );
		return newLoaderComposite;
	}
}
//------------------------------------------------------------
dsLoader_BaseInterface::traits::pen_cont_out dsLoaderAuto_CompositeDispatcher::getProcessElementNames()const
{
	shared_ptr_traits<traits::pen_cont>::type pnn( new traits::pen_cont );
	traits::child_ldr_cont::const_iterator iter = childLoaders.begin();
	traits::child_ldr_cont::const_iterator end_iter = childLoaders.end();
	for( ; iter != end_iter; ++iter ){
		pnn->insert( &(iter->first) );
	}
	return pnn;
}
//------------------------------------------------------------
}
}
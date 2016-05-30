#include "DemoStableHeaders.h"
#include "StringPath.h"

namespace Demo
{
namespace Core
{
	//------------------------------------------------------------
	StringPath::string_traits::cstr StringPath::msPathDelimiter = "/";
	//------------------------------------------------------------
	StringPath::StringPath( string_traits::in dirName )
		: name( dirName )
		, parentDir(0)
	{
	}
	//------------------------------------------------------------
	StringPath::~StringPath()
	{
		detachFromParent();
	}
	//------------------------------------------------------------
	StringPath::traits::child_dir_type StringPath::createChildDir( string_traits::in dirName )
	{
		traits::child_dir_type rv = getChildDir( dirName );
		if( rv != 0 )return rv;

		rv.bind( new StringPath( dirName ) );
		childContainer.insert( traits::child_container::value_type( dirName, rv ) );
		bindThisAsParent( rv );
		return rv;
	}
	//------------------------------------------------------------
	StringPath::traits::child_dir_type StringPath::removeChildDir( string_traits::in dirName )
	{
		traits::child_container::iterator iter = childContainer.find( dirName );
		if( iter == childContainer.end() )return traits::child_dir_type();
		traits::child_dir_type rv = iter->second;
		rv->detachingFromParent();
		childContainer.erase( iter );
		return rv;
	}
	//------------------------------------------------------------
	void StringPath::addDir( traits::child_container_in chContIn )
	{
		traits::child_oit iter( chContIn );
		while( iter.hasMoreElements() )
		{
			auto chDirIn = iter.getNextCRef();
			traits::child_dir_type chDirMy = getChildDir( chDirIn->name );
			if( chDirMy != 0 ) {
				chDirMy->addDir( chDirIn->childContainer );
				chDirIn = chDirMy;
			} else {
				createChildDir( chDirIn->name )->addDir( chDirIn->childContainer );
			}
		}
	}
	//------------------------------------------------------------
	void StringPath::addDir( traits::child_dir_in chDirIn )
	{
		traits::child_dir_type chDirMy = getChildDir( chDirIn->name );
		if( chDirMy == 0 ) {
			createChildDir( chDirIn->name )->addDir( chDirIn->childContainer );
		} else {
			chDirMy->addDir( chDirIn->childContainer );
			chDirIn = chDirMy;
		}
	}
	//------------------------------------------------------------
}
}
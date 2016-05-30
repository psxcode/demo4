#pragma once

#include <OgreIteratorWrapper.h>

namespace Demo
{
namespace Core
{
	//------------------------------------------------------------
	class _DemoExport StringPath
	{
	public:
		DEFINE_CLASS_SHARED_PTR(StringPath)
		//--------------------------------//
		typedef Demo::string_traits string_traits;
		//--------------------------------//
		class traits
		{
		public:
			typedef shared_ptr			child_dir_type;
			typedef const_shared_ptr	const_child_dir_type;
			typedef type_traits< child_dir_type >::ref child_dir_in;
			typedef shared_ptr			parent_dir_type;
			typedef const_shared_ptr	const_parent_dir_type;
			typedef type_traits< parent_dir_type >::ref parent_dir_in;

			typedef Demo::map< string_traits::str, child_dir_type >::type child_container;

			typedef type_traits< child_container >::ref child_container_in;
			typedef Ogre::MapIterator< child_container > child_oit;
			typedef Ogre::ConstMapIterator< child_container > child_ocit;
		};
		//--------------------------------//
		StringPath( string_traits::in dirName );
		~StringPath();

		traits::child_dir_type createChildDir( string_traits::in dirName );
		traits::child_dir_type removeChildDir( string_traits::in dirName );

		void addDir( traits::child_dir_in dir );

		string_traits::str getFullPath()const { return isRootDir() ? name + msPathDelimiter : parentDir->getFullPath() + name + msPathDelimiter; }

		string_traits::outref getName()const { return name; }

		traits::child_oit getChildDirIterator() { return traits::child_oit( childContainer ); }
		traits::child_ocit getChildDirIterator()const { return traits::child_ocit( childContainer ); }

		traits::parent_dir_type getParentDir() { return parentDir; }
		traits::const_parent_dir_type getParentDir()const { return parentDir; }

		//returns NULL if THIS is a Root
		traits::parent_dir_type getRootDir(){ return getRootDirImpl< traits::parent_dir_type >(); }
		traits::const_parent_dir_type getRootDir()const{ return getRootDirImpl< traits::const_parent_dir_type >(); }

		//returns NULL if child with that name do not exist
		traits::child_dir_type getChildDir( string_traits::in dirName ) { return getChildDirImpl< traits::child_dir_type >( dirName ); }
		traits::const_child_dir_type getChildDir( string_traits::in dirName )const { return getChildDirImpl< traits::const_child_dir_type >( dirName ); }

		bool isRootDir()const { return parentDir == 0; }
		unsigned int numChildDirs()const{ return childContainer.size(); }

		void detachFromParent() { if(parentDir != 0) parentDir->removeChildDir( name ); }

	private:
		//--------------------------------//
		static string_traits::cstr msPathDelimiter;
		//--------------------------------//
		StringPath();
		StringPath( const StringPath& );
		StringPath& operator=( const StringPath& );

		template< typename T >
		T getRootDirImpl()const {
			T pd = parentDir; T cd;
			while( pd != 0 ) { cd = pd; pd = pd->parentDir; }
			return cd;
		}

		template< typename T >
		T getChildDirImpl( string_traits::in dirName )const { 
			traits::child_container::const_iterator iter = childContainer.find( dirName );
			if( iter != childContainer.end() ) return iter->second;
			return T();
		}

		void addDir( traits::child_container_in chCont );

		void detachingFromParent() { parentDir.reset(); }

		void bindThisAsParent( traits::child_dir_in ch ) { 
			ch->detachFromParent();
			ch->parentDir.bind( this, Ogre::SPFM_NO_FREE );
		}

		string_traits::str name;
		traits::parent_dir_type parentDir;
		traits::child_container childContainer;
	};
	//------------------------------------------------------------
}
}
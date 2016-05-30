#include "DemoStableHeaders.h"
#include "DemoArchive.h"
#if DBS_LM_ARCHIVE

namespace bf = boost::filesystem;
namespace da = Demo::Archive;
namespace Demo
{
//------------------------------------------------------------
string_traits::cstr DemoArchiveFactory::ArchiveTypeName = "Demo";
string_traits::str DemoArchive::RootDirName = "root";
//------------------------------------------------------------
int DemoArchive::CreateArchive( string_traits::in volumeName )
{
	if( isOpen() )
		return OPR_FAIL;
	if( volumeName.empty() )
		return OPR_FILENAME_INCORRECT;
		
	da::Volume* vol = new da::Volume();
	VolumeMap::_Pairib vipair = volumeMap.insert( std::make_pair( volumeName, vol ) );
	if( !vipair.second ){ delete vol; return OPR_FAIL; }
	vol->pName = &vipair.first->first;

	rootDir = new da::Directory();
	rootDir->pVolume = vol;
	rootDir->pName = &RootDirName;

	_isOpen = true;
	_isCreated = true;
	_isModified = true;

	return OPR_OK;
}
//------------------------------------------------------------
int DemoArchive::OpenArchive( string_traits::in filePathName )
{
	return OpenArchive( ogre_path( filePathName ) );
}
//------------------------------------------------------------
int DemoArchive::OpenArchive( const ogre_path& filename )
{
	if( _isOpen )return OPR_FAIL;
	if( !bf::exists( filename ) || !bf::is_regular_file( filename ) )
		return OPR_FILE_NOT_FOUND;

	ogre_ifstream ifile( filename );
	if( !ifile )
		return OPR_FAIL;

	//loading

	//volume count
	VolumeMap::size_type volCount;
	ifile >> volCount;
	//loading volumes
	for( VolumeMap::size_type i = 0 ; i < volCount; ++i )
	{
		//volume name
		string_traits::str volName;
		ifile >> volName;

		da::Volume* vol = new da::Volume();
		vol->load( ifile );
		VolumeMap::_Pairib ipair = volumeMap.insert( std::make_pair( volName, vol ) );
		if( !ipair.second ){ delete vol; continue; }
		vol->pName = &ipair.first->first;
	}

	//load directories
	rootDir = new da::Directory();
	rootDir->load( ifile );
	rootDir->pName = &RootDirName;

	//loading
}
//------------------------------------------------------------
int DemoArchive::SaveArchive()
{
	if( !_isOpen || !_isModified )return;
	if( archivePath.empty() )return;
		
	if( _isCreated )
		return saveCreated();
	else
		return saveModified();
}
//------------------------------------------------------------
void da::Directory::save( ogre_ofstream &ostr )
{
	//volume name
	ostr << *(pVolume->pName);

	//save files
	{
		//file count
		FileMap::size_type count = fileMap.size();
		ostr << count;

		FIter iter = fileMap.begin();
		FIter end_iter = fileMap.end();
		for( ; iter != end_iter; ++iter )
		{
			//save filename
			ostr << iter->first;

			//save file
			ostr << iter->second->position;
			ostr << iter->second->size;
		}
	}

	//save directories
	{
		//dir count
		DirMap::size_type count = dirMap.size();
		ostr << count;

		DirMap::iterator iter = dirMap.begin();
		DirMap::iterator end_iter = dirMap.end();
		for( ; iter != end_iter; ++iter )
		{
			//save dirName
			ostr << iter->first;

			//save dir
			iter->second->save( ostr );
		}
	}
}
//------------------------------------------------------------
void da::Directory::load(ogre_ifstream &istr)
{
	istr >> volumeName;
	//load files
	{
		//file count
		FileMap::size_type count;
		istr >> count;

		for( FileMap::size_type i = 0; i < count; ++i )
		{
			//load filename
			string_traits::str filename;
			istr >> filename;

			//load file
			da::File* newFile = new da::File();
			istr >> newFile->position;
			istr >> newFile->size;
			FileInsertPair ipair = fileMap.insert( FilePair( filename, newFile ) );
			if( !ipair.second ){ delete newFile; continue; }
			newFile->pName = &ipair.first->first;
			newFile->pParentDir = this;
		}
	}

	//load directories
	{
		//dir count
		DirMap::size_type count;
		istr >> count;

		for( DirMap::size_type i = 0; i < count; ++i )
		{
			//dir name
			string_traits::str dirname;
			istr >> dirname;

			//load dir
			da::Directory* newDir = new da::Directory();
			newDir->load( istr );
			DirMap::_Pairib ipair = dirMap.insert( std::make_pair( dirname, newDir ) );
			if( !ipair.second ){ delete newDir; continue; }
			newDir->pName = &ipair.first->first;
			newDir->pParentDir = this;
		}
	}
}
//------------------------------------------------------------
int DemoArchive::saveCreated()
{
	//save volumes
}
//------------------------------------------------------------
int DemoArchive::saveModified()
{
}
//------------------------------------------------------------
int DemoArchive::saveVolumes()
{
		
}
//------------------------------------------------------------
}
#endif
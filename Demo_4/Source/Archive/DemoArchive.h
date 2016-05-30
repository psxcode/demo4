#pragma once
#if DBS_LM_ARCHIVE

#include <OgreArchive.h>
#include <OgreArchiveFactory.h>
#include <OgreIteratorWrappers.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace Demo
{
//--------------------------------//
#if OGRE_WCHAR_T_STRINGS
	typedef boost::filesystem::wpath ogre_path;
	typedef boost::filesystem::wfstream ogre_fstream;
	typedef boost::filesystem::wifstream ogre_ifstream;
	typedef boost::filesystem::wofstream ogre_ofstream;
#else
	typedef boost::filesystem::path ogre_path;
	typedef boost::filesystem::fstream ogre_fstream;
	typedef boost::filesystem::ifstream ogre_ifstream;
	typedef boost::filesystem::ofstream ogre_ofstream;
#endif
//--------------------------------//
namespace Archive
{
	//------------------------------------------------------------
	class Directory;
	//------------------------------------------------------------
	class File
	{
	public:
		File()
			: position(0)
			, size(0)
			, pParentDir(0)
			, pName(0)
		{
		}
		~File()
		{
		}
			
		//save/load data
		fpos_t position;
		fpos_t size;

		//link data
		string_traits::outptr pName;
		Directory* pParentDir;

		char* fileContentsBuffer;

		//Показывает путь при создании файла
		//ogre_path filePath;

		//flag data
		//bool isCreated;
		//bool isModified;
	};
	//------------------------------------------------------------
	//class Volume;
	//------------------------------------------------------------
	class Directory
	{
	public:
		class traits
		{
		public:
			typedef Demo::map< string_traits::str, Directory* >::type dir_container;
			typedef Ogre::MapIterator< dir_container > dir_oit;
			typedef Ogre::ConstMapIterator< dir_container > dir_ocit;

			typedef Demo::map< string_traits::str, File* >::type file_container;
			typedef Ogre::MapIterator< file_container > file_oit;
			typedef Ogre::ConstMapIterator< file_container > file_ocit;
		};
		//--------------------------------//
		Directory()
			: pName(0)
			, pParentDir(0)
		{
		}
		~Directory()
		{
		}
		void save( ogre_ofstream& ostr );
		void load( ogre_ifstream& istr );

		inline traits::dir_oit getDirIterator(){ return traits::dir_oit( dirMap.begin(), dirMap.end() ); }
		inline traits::file_oit getFileIterator(){ return traits::file_oit( fileMap.begin(), fileMap.end() ); }
		//inline traits::file_oit getAddFileIterator(){ return traits::file_oit( addFileMap.begin(), addFileMap.end() ); }
		//inline traits::file_oit getRemoveFileIterator(){ return traits::file_oit( removeFileMap.begin(), removeFileMap.end() ); }

		inline traits::dir_ocit getDirConstIterator()const{ return traits::dir_ocit( dirMap.begin(), dirMap.end() ); }
		inline traits::file_ocit getFileIterator()const{ return traits::file_ocit( fileMap.begin(), fileMap.end() ); }
		//inline traits::file_ocit getAddFileIterator()const{ return traits::file_ocit( addFileMap.begin(), addFileMap.end() ); }
		//inline traits::file_ocit getRemoveFileIterator()const{ return traits::file_ocit( removeFileMap.begin(), removeFileMap.end() ); }

		void setModifiedFlag(){ isModified = true; }

		//link data
		string_traits::outptr pName;
		string_traits::outptr pVolumeName;
		Directory* pParentDir;

		//save/load data
		traits::dir_container dirMap;
		traits::file_container fileMap;

		//add remove data
		//traits::file_container addFileMap;
		//traits::file_container removeFileMap;

		/*
		Это имя заполняется при загрузке
		Потом оно не нужно
		*/

		bool isModified;
	};
	//------------------------------------------------------------
	/*class Volume
	{
	public:
		//--------------------------------//
		class traits
		{
		public:
			typedef Demo::set< Demo::Archive::Directory* > dir_container;
		};
		//--------------------------------//
		Volume()
			: size(0)
			, occupiedSize(0)
			, pName(0)
			, isCreated( false )
			, wasRemoval( false )
			, isEmpty( false )
		{
		}
		~Volume()
		{
		}
		void save( ogre_ofstream& ostr );
		void load( ogre_ifstream& istr );
		void mergeEmptySpace();

		string_traits::outptr pName;
		fpos_t occupiedSize;
		fpos_t size;

		//FileMap emptyMap;
		//DirSet dirSet;

		bool isCreated;
		bool isEmpty;
		bool wasRemoval;
	};*/
	//------------------------------------------------------------
}//namespace Archive
//------------------------------------------------------------
class DemoArchive : public Ogre::Archive
{
public:
	//--------------------------------//
	enum OP_RESULT
	{
		OPR_FAIL = 0,
		OPR_OK = 0x1,
		OPR_FILE_NOT_FOUND = 0x2,
		OPR_FILE_ALREADY_EXIST = 0x4,
		OPR_FILENAME_INCORRECT = 0x8
	};
	//--------------------------------//
	virtual ~DemoArchive();

	//Ogre implementation--------------------------------------------------
	///////////////////////////////////////////////////////////////////////
	//Constructor for OGRE Archive System

	DemoArchive( string_traits::in name, string_traits::in archType );

	/// @copydoc Ogre::Archive::isCaseSensitive
	virtual bool isCaseSensitive(void)const{ return false; }

	/// @copydoc Ogre::Archive::load
    virtual void load();

	/// @copydoc Ogre::Archive::unload
    virtual void unload();

	/// @copydoc Ogre::Archive::open
    Ogre::DataStreamPtr open(string_traits::in filename) const;

    /// @copydoc Ogre::Archive::list
    Ogre::StringVectorPtr list(bool recursive = true, bool dirs = false);

    /// @copydoc Ogre::Archive::listFileInfo
    Ogre::FileInfoListPtr listFileInfo(bool recursive = true, bool dirs = false);

    /// @copydoc Ogre::Archive::find
    Ogre::StringVectorPtr find(string_traits::in pattern, bool recursive = true, bool dirs = false);

    /// @copydoc Ogre::Archive::findFileInfo
    Ogre::FileInfoListPtr findFileInfo(string_traits::in pattern, bool recursive = true, bool dirs = false);

    /// @copydoc Ogre::Archive::exists
    bool exists(string_traits::in filename);

	/// @copydoc Ogre::Archive::getModifiedTime
	time_t getModifiedTime(string_traits::in filename);

	///////////////////////////////////////////////////////////////////////
	//Ogre implementation--------------------------------------------------

	int CreateArchive( string_traits::in volumeName );
	int OpenArchive( string_traits::in filePathName );
	int OpenArchive( const ogre_path& filePathName );
	void CloseArchive();
	int SaveArchive();
	int AddFile();
	int AddDirectory();

	inline bool isCreated()const{ return _isCreated; }
	inline bool isModified()const{ return _isModified; }
	inline bool isOpen()const{ return _isOpen; }

protected:
	//--------------------------------//
	static string_traits::str RootDirName;
	//--------------------------------//
	int saveCreated();
	int saveModified();
	int saveVolumes();

	Demo::Archive::Directory* rootDir;
	ogre_path archivePath;

	//flags
	bool _isCreated;
	bool _isModified;
	bool _isOpen;
};
//------------------------------------------------------------
class DemoArchiveFactory : public Ogre::ArchiveFactory
{
public:
	//--------------------------------//
	static string_traits::cstr ArchiveTypeName;
	//--------------------------------//
	virtual ~DemoArchiveFactory()
	{
	}

	/// @copydoc Ogre::FactoryObj::getType
	virtual string_traits::outref getType()const{ return ArchiveTypeName; }

    /// @copydoc Ogre::FactoryObj::createInstance
	virtual Ogre::Archive* createInstance( string_traits::in name ){ return OGRE_NEW DemoArchive(name, ArchiveTypeName); }

    /// @copydoc Ogre::FactoryObj::destroyInstance
    virtual void destroyInstance( Ogre::Archive* arch){ OGRE_DELETE arch; }
};
//------------------------------------------------------------
}
#endif
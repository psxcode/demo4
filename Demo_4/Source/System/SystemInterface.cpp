#include "SystemInterface.h"
#include "System\MainController.h"
#include "System\InputController.h"
#include "System\OgreController.h"
#include "System\DistanceUnitInterface.h"
#include <OgreString.h>
#include <Windows.h>

namespace Demo
{
namespace System
{
	bool SystemInterface::_operate(false);
	bool SystemInterface::_has_focus(false);
	SystemInterface::String SystemInterface::defaultProfileName = "common";
	SystemInterface::String SystemInterface::profileName = "";
	//------------------------------------------------------------
	void SystemInterface::Run()
	{
		DistanceUnitInterface::SetSystemUnitScale( US_INCH );
		DistanceUnitInterface::SetDisplayUnitScale( US_CENTIMETER );
		_operate = true;
		if( !OgreController::GetSingleton().Initialize() )return;
		if( !InputController::GetSingleton().Initialize() )return;
		if( !MainController::GetSingleton().Initialize() )return;

		while( _operate )
		{
			InputController::GetSingleton().OperateController();
			MainController::GetSingleton().OperateController();
			OgreController::GetSingleton().OperateController();
		}
	}
	//------------------------------------------------------------
	void SystemInterface::Shutdown()
	{
		MainController::GetSingleton().CloseController();
		InputController::GetSingleton().CloseController();
		OgreController::GetSingleton().CloseController();
	}
	//------------------------------------------------------------
	bool SystemInterface::IsSeparator( char c )
	{
		return c == '\\' || c == '/';
	}
	//------------------------------------------------------------
	bool SystemInterface::IsFileReadable( const String &path )
	{
		return GetFileAttributesA( path.c_str() ) != INVALID_FILE_ATTRIBUTES;
	}
	//------------------------------------------------------------
	bool SystemInterface::IsAbsolutePath( const String &path )
	{
		return path[1] == ':';
	}
	//------------------------------------------------------------
	void SystemInterface::MakeFullPath( String& path )
	{
		DWORD maxLength = MAX_PATH + 1;
		std::vector< CHAR > fullPath;
		fullPath.resize( maxLength );

		LPSTR filePath;
		DWORD result = ::GetFullPathNameA( path.c_str(), maxLength, &fullPath[0], &filePath );
		if( result > maxLength )
		{
			fullPath.resize( result );
			result = ::GetFullPathNameA( path.c_str(), result, &fullPath[0], &filePath );
		}
		path = &fullPath[0];
	}
	//------------------------------------------------------------
	void SystemInterface::EnsureTrailingPathSeparator( String &path )
	{
		if( path.length() > 0 )
		{
			char lastChar = path[ path.size() - 1 ];
			if( !IsSeparator( lastChar ) )
				path += '/';
		}
	}
	//------------------------------------------------------------
	SystemInterface::String SystemInterface::GetApplicationDirectory()
	{
		String path, baseName;
		const DWORD maxLength = MAX_PATH + 1;
		CHAR pathChars[ maxLength ];
		GetModuleFileNameA( GetModuleHandle( 0 ), pathChars, maxLength );
		Ogre::StringUtil::splitFilename( String(pathChars), baseName, path );
		EnsureTrailingPathSeparator( path );
		return path;
	}
	//------------------------------------------------------------
}
}
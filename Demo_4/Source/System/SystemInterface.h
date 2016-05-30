#pragma once
#include "string_traits.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	class SystemInterface
	{
	public:
		//--------------------------------//
		typedef string_traits<>::str String;
		typedef string_traits<>::inpar string_traits::in;
		typedef string_traits<>::inoutpar String_InOutPar;
		typedef string_traits<>::outcopy String_OutCopy;
		//--------------------------------//
		static bool IsFileReadable( string_traits::in path );
		static bool IsSeparator( char );
		static bool IsAbsolutePath( string_traits::in path );
		static void MakeFullPath( String_InOutPar path );
		static void EnsureTrailingPathSeparator( String_InOutPar path );
		static String_OutCopy GetApplicationDirectory();
		//--------------------------------//
		static void Run();
		static void Shutdown();

		static bool _operate;
		static bool _has_focus;
		static String profileName;
		static String defaultProfileName;

	private:
		SystemInterface();
		SystemInterface( const SystemInterface& );
		SystemInterface& operator=( const SystemInterface& );
	};
	//------------------------------------------------------------
}
}
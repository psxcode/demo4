#pragma once

namespace Demo
{
namespace Core
{
	//------------------------------------------------------------
	template< typename T >
	class bitop
	{
	public:
		static inline void set_flag( T& value, T flag ){ value |= flag; }
		static inline void reset_flag( T& value, T flag ){ value &= ~ flag; }
		static inline void toggle_flag( T& value, T flag ){ value ^= flag; }
		static inline bool is_flag_set( T value, T flag ){ return (value & flag) != 0; }
		static inline T and_flag( T value, T flag ){ return value & flag; }
		//--------------------------------//
		enum { ZERO = 0x0 };
		//--------------------------------//

		bitop();
		explicit bitop( T );
		bitop( const bitop<T>& rhv );
		bitop<T>& operator=( const bitop<T>& rhv );

		inline void set( T flag ){ value |= flag; }
		inline void reset( T flag ){ value &= ~flag; }
		inline void toggle( T flag ){ value ^= flag; }
		inline bool is_set( T flag ){ return (value & flag) != 0; }
		inline T and( T flag ){ return value & flag; }

		T value;
	};
	//------------------------------------------------------------
	template< typename T >
	bitop<T>::bitop()
		: value( ZERO )
	{
	}
	//------------------------------------------------------------
	template< typename T >
	bitop<T>::bitop( T rhv )
		: value( rhv )
	{
	}
	//------------------------------------------------------------
	template< typename T >
	bitop<T>::bitop( const bitop<T>& rhv )
		: value( rhv.value )
	{
	}
	//------------------------------------------------------------
	template< typename T >
	bitop<T>& bitop<T>::operator=( const bitop<T>& rhv )
	{
		if( &rhv == this )
			return *this;

		value = rhv.value;
		return *this;
	}
	//------------------------------------------------------------
	template< typename T >
	inline bool operator==( const bitop<T>& a, const bitop<T>& b )
	{
		return a.value == b.value;
	}
	//------------------------------------------------------------
	template< typename T >
	inline bool operator!=( const bitop<T>& a, const bitop<T>& b )
	{
		return !( a == b );
	}
	//------------------------------------------------------------
}
}
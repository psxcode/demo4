#pragma once
/*Есть договоренность что OGRE является основной платформой
 *поэтому здесь происходит включение OgrePrerequisites.h
 */
#include <OgrePrerequisites.h>

/*Собственные настройки и утверждения
 *Имеет префикс DBS_ (DemoBuildSettings)
 */
#include "DemoBuildSettings.h"

#if DBS_ASSERTION_ENABLED
#endif

#if DBS_STRING_COMPARE_FUNC == DBS_STRING_COMPARE_BOOST
#	include <boost/algorithm/string.hpp>
#endif

#include "type_traits.h"

namespace Demo
{
	//------------------------------------------------------------
	//Ogre containers use custom allocator
	template< typename T >
	struct deque
	{
#if DBS_USE_OGRE_CONTAINERS
		typedef typename Ogre::deque< T >::type type;
#else
		typedef std::deque< T > type;
#endif
	};
	//------------------------------------------------------------
	template< typename T >
	struct vector
	{
#if DBS_USE_OGRE_CONTAINERS
		typedef typename Ogre::vector< T >::type type;
#else
		typedef std::vector< T > type;
#endif
	};
	//------------------------------------------------------------
	template< typename T >
	struct list
	{
#if DBS_USE_OGRE_CONTAINERS
		typedef typename Ogre::list< T >::type type;
#else
		typedef std::list< T > type;
#endif
	};
	//------------------------------------------------------------
	template< typename T, typename P = std::less<T> >
	struct set
	{
#if DBS_USE_OGRE_CONTAINERS
		typedef typename Ogre::set< T, P >::type type;
#else
		typedef std::set< T, P > type;
#endif
	};
	//------------------------------------------------------------
	template< typename K, typename V, typename P = std::less<K> >
	struct map
	{
#if DBS_USE_OGRE_CONTAINERS
		typedef typename Ogre::map< K, V, P >::type type;
#else
		typedef std::map< K, V, P > type;
#endif
	};
	//------------------------------------------------------------
	template< typename K, typename V, typename P = std::less<K> >
	struct multimap
	{
#if DBS_USE_OGRE_CONTAINERS
		typedef typename Ogre::multimap< K, V, P >::type type;
#else
		typedef std::multimap< K, V, P > type;
#endif
	};
	//------------------------------------------------------------
	/*
	Единый тип строк для всех.
	Также стандарт для параметров ввода вывода.
	*/
	template< typename str_type >
	class _StringTraits
	{
	public:
		typedef typename type_traits< str_type >::type str;
		typedef typename type_traits< str_type >::ctype cstr;
		typedef typename type_traits< str_type >::type outcopy;
		typedef typename type_traits< str_type >::cref outref;
		typedef typename type_traits< str_type >::cdata_ptr outptr;
		typedef typename type_traits< str_type >::cref in;
		typedef typename type_traits< str_type >::ref inout;
	};

#if DBS_STRING_TYPE == DBS_STRING_TYPE_OGRE
	typedef _StringTraits< Ogre::String > string_traits;
#else
#	error
#endif

	template<bool CaseSensitive>
	class string_equals;

	template<>
	class string_equals<true>
	{
	public:
#if DBS_STRING_COMPARE_FUNC == DBS_STRING_COMPARE_BOOST
		template<typename T1, typename T2>
		inline bool operator() ( T1 lhs, T2 rhs )const
		{
			return ::boost::equals( lhs, rhs );
		}
#endif
	};

	template<>
	class string_equals<false>
	{
	public:
#if DBS_STRING_COMPARE_FUNC == DBS_STRING_COMPARE_BOOST
		template<typename T1, typename T2>
		inline bool operator() ( T1 lhs, T2 rhs )const
		{
			return ::boost::iequals( lhs, rhs );
		}
#endif
	};
	//------------------------------------------------------------
}
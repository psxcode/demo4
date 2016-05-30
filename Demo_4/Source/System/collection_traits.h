#pragma once
#if DBS_LM_COLLECTIONS

namespace Demo
{
namespace System
{
//------------------------------------------------------------
/*
Класс представляет собой набор стандартных параметров ввода вывода
для интерфейсов коллекций
*/
//------------------------------------------------------------
enum CollectionObjectStoreType
{
	COST_TYPE,
	COST_PTR,
	COST_EXPLICIT_SHARED_PTR,
	COST_BUILTIN_SHARED_PTR,
};
//------------------------------------------------------------
template< typename T, int OBJTYPE = COST_TYPE >
class collection_traits
{
public:
	typedef typename type_traits<T>::type							store_type;
	typedef typename type_traits<T>::ctype							store_const_type;
	typedef typename type_traits<T>::cref							in;
	typedef typename type_traits<T>::ref							inout;
	typedef typename store_type										out;
	typedef typename store_type										outcopy;
	typedef typename type_traits<T>::cref							outref;
	typedef typename string_traits::str								key_type;
	typedef typename Demo::map< key_type, store_const_type >::type	container;
};
//------------------------------------------------------------
template< typename T >
class collection_traits< T, COST_PTR >
{
public:
	typedef typename type_traits<T>::ptr							store_type;
	typedef typename type_traits<T>::cdata_ptr						store_const_type;
	typedef store_const_type										in;
	typedef store_const_type										out;
	typedef typename type_traits<store_type>::ref					inout;
	typedef typename string_traits::str								key_type;
	typedef typename Demo::map< key_type, store_const_type >::type	container;
};
//------------------------------------------------------------
template< typename T >
class collection_traits< T, COST_BUILTIN_SHARED_PTR >
{
public:
	typedef typename T::shared_ptr										store_type;
	typedef typename T::const_shared_ptr								store_const_type;
	typedef typename type_traits<store_const_type>::cref				in;
	typedef store_const_type											out;
	typedef typename type_traits<store_type>::ref						inout;
	typedef typename string_traits::str									key_type;
	typedef typename Demo::map< key_type, store_const_type >::type		container;
};
//------------------------------------------------------------
template< typename T >
class collection_traits< T, COST_EXPLICIT_SHARED_PTR >
{
public:
	typedef typename shared_ptr_traits<T>::type											store_type;
	typedef typename shared_ptr_traits<typename type_traits<T>::ctype>::type			store_const_type;
	typedef typename type_traits<store_const_type>::cref								in;
	typedef store_const_type															out;
	typedef typename type_traits<store_type>::ref										inout;
	typedef typename string_traits::str													key_type;
	typedef typename Demo::map< key_type, store_const_type >::type						container;
};
//------------------------------------------------------------
}
}
#endif
#pragma once

#if DBS_SHARED_PTR_TYPE == DBS_SHARED_PTR_OGRE
#	include <OgreSharedPtr.h>
#elif DBS_SHARED_PTR_TYPE == DBS_SHARED_PTR_BOOST
#	include <boost/shared_ptr.hpp>
#else
#	error
#endif

namespace Demo
{
//------------------------------------------------------------
template< typename T >
class shared_ptr_traits
{
public:		
#if DBS_SHARED_PTR_TYPE == DBS_SHARED_PTR_OGRE
	typedef Ogre::SharedPtr< T > type;
#elif DBS_SHARED_PTR_TYPE == DBS_SHARED_PTR_BOOST
	typedef boost::shared_ptr< T > type;
#else
#error
#endif
};

template<typename T, typename SHPTR>
inline typename shared_ptr_traits<T>::type shared_ptr_dynamic_cast( SHPTR const &shptr )
{
#if DBS_SHARED_PTR_TYPE == DBS_SHARED_PTR_OGRE
	return typename shared_ptr_traits<T>::type(shptr, Ogre::shared_ptr_dynamic_cast_tag());
#elif DBS_SHARED_PTR_TYPE == DBS_SHARED_PTR_BOOST
	return boost::shared_dynamic_cast<T>(shptr);
#endif
}

template<typename T, typename SHPTR>
inline typename shared_ptr_traits<T>::type shared_ptr_static_cast( SHPTR const &shptr )
{
#if DBS_SHARED_PTR_TYPE == DBS_SHARED_PTR_OGRE
	return typename shared_ptr_traits<T>::type(shptr, Ogre::shared_ptr_static_cast_tag());
#elif DBS_SHARED_PTR_TYPE == DBS_SHARED_PTR_BOOST
	return boost::shared_static_cast<T>(shptr);
#endif
}
//------------------------------------------------------------
#define DEFINE_CLASS_SHARED_PTR(ClassName)											\
	typedef shared_ptr_traits<type_traits<ClassName>::type>::type shared_ptr;		\
	typedef shared_ptr_traits<type_traits<ClassName>::ctype>::type const_shared_ptr;

#define DEFINE_TCLASS_SHARED_PTR(ClassName)															\
	typedef typename shared_ptr_traits<typename type_traits<ClassName>::type>::type shared_ptr;		\
	typedef typename shared_ptr_traits<typename type_traits<ClassName>::ctype>::type const_shared_ptr;
//------------------------------------------------------------
}
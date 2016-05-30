#pragma once

namespace Demo
{
//------------------------------------------------------------
template< typename T >
class type_traits
{
public:
	typedef T					type;
	typedef T const				ctype;
	typedef T *					ptr;
	typedef T const *			cdata_ptr;
	typedef T * const			cptr_ptr;
	typedef T const * const		cdataptr_ptr;
	typedef T &					ref;
	typedef T const &			cref;
};
//------------------------------------------------------------
#define DEFINE_CLASS_BASIC_TYPES(ClassName)				\
	typedef type_traits<ClassName> _t;
//------------------------------------------------------------
}
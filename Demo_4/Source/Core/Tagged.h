#pragma once

namespace Demo {
namespace Core {
//------------------------------------------------------------
template< typename Base >
struct TagCounter
{
	static size_t s_counter; // declaration
};
//------------------------------------------------------------
template< typename Base >
size_t TagCounter< Base >::s_counter = 0; // definition: default 0
//------------------------------------------------------------
template< typename Derived, typename Base >
struct TagHolder
{
	static inline size_t get()
	{
		if(s_tag == 0) s_tag = ++TagCounter<Base>::s_counter;
		return s_tag;
	}
	static size_t s_tag;
};
//------------------------------------------------------------
template< typename Derived, typename Base >
size_t TagHolder< Derived, Base >::s_tag = 0;
//------------------------------------------------------------
// Base class must inherit from Tagged
template< typename Base >
struct Tagged
{
	template< typename Derived >
	static size_t sTag( const Derived* /* this */ )
	{
		return TagHolder< Derived, Base >::get();
	}
};
} //namespace Core
//------------------------------------------------------------
//A macro helps defining a virtual function in every Derived class to return its tag: 
#define DEFINE_TAGGED									\
virtual size_t Tag() const								\
{														\
	return this->sTag( this );							\
}
//------------------------------------------------------------
/* Base* ptr1 = new Derived1;
 * Base* ptr2 = new Derived2;
 * 
 * if(ptr1->Tag() == ptr2->Tag())
 * if(ptr1->Tag() == Base::sTag<Derived1>(0))
 **/
//------------------------------------------------------------
}

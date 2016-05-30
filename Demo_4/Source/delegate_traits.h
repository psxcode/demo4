#pragma once

#if DBS_DELEGATE_TYPE == DBS_DELEGATE_TYPE_DONS_FAST
#	include "Core/FastDelegate.h"
#	ifdef FASTDELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX
#		define DBS_DELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX
#	endif
#endif

#include <OgreIteratorWrappers.h>

namespace Demo
{
//------------------------------------------------------------
class delegate_traits
{
private:
	//--------------------------------//
	template< typename D >
	class container
	{
	public:
		//--------------------------------//
		class traits
		{
		public:
			typedef typename D::const_shared_ptr del_t;
			typedef typename type_traits< del_t >::cref del_in;
			typedef typename Demo::set< del_t >::type del_cont;
			typedef typename Ogre::SetIterator< del_cont > del_oit;
			typedef typename Ogre::ConstSetIterator< del_cont > del_ocit;
		};
		//--------------------------------//
		container() {}

		inline bool empty()const { return delContainer.empty(); }
		inline void add( typename traits::del_in del ) { delContainer.insert( del ); }
		inline void remove( typename traits::del_in del ) { delContainer.erase( del ); }
		inline void clear() { delContainer.clear(); }
		inline void merge( const container<D>& in ) {
			traits::del_cont::const_iterator iter = in.delContainer.begin();
			traits::del_cont::const_iterator end_iter = in.delContainer.end();
			for( ; iter != end_iter; ++iter )
				delContainer.insert( *iter );
		}
		inline typename traits::del_oit getDelIterator() { return typename traits::del_oit(delContainer); }
		inline typename traits::del_ocit getDelIterator()const { return typename traits::del_ocit(delContainer); }

	protected:
		~container() {}
		typename traits::del_cont delContainer;
	};
	//--------------------------------//

public:
	//--------------------------------//
	template<typename R>
	class idel0
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(idel0)
		virtual ~idel0() {}
		virtual R operator() ()const = 0;
	};
	//--------------------------------//
	template<typename Param1, typename R>
	class idel1
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(idel1)
		virtual ~idel1() {}
		virtual R operator() (Param1)const = 0;
	};
	//--------------------------------//
	template<typename Param1, typename Param2, typename R>
	class idel2
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(idel2)
		virtual ~idel2() {}
		virtual R operator() (Param1, Param2)const = 0;
	};
	//--------------------------------//
#ifdef DBS_DELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX
	//--------------------------------//
	template<typename Signature>
	class idel;
	//--------------------------------//
	template<typename R>
	class idel<R ()> : public idel0<R>
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(idel)
		virtual ~idel() {}
	};
	//--------------------------------//
	template<typename R, typename Param1>
	class idel<R (Param1)> : public idel1<Param1, R>
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(idel)
		virtual ~idel() {}
	};
	//--------------------------------//
	template<typename R, typename Param1, typename Param2>
	class idel<R (Param1, Param2)> : public idel2<Param1, Param2, R>
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(idel)
		virtual ~idel() {}
	};
	//--------------------------------//
#endif //DBS_DELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX
	//--------------------------------//
#if DBS_DELEGATE_TYPE == DBS_DELEGATE_TYPE_DONS_FAST
	//--------------------------------//
	template<typename R>
	class del0 : public fastdelegate::FastDelegate0<R>, public idel0<R>
	{
	public:
		/* Some Class may want to use fast baseType without interface and virtuals
		 * so just create delegate with del0::type line
		*/
		typedef fastdelegate::FastDelegate0<R> type;
		DEFINE_TCLASS_SHARED_PTR(del0)
		virtual ~del0() {}

		del0() : type() {}

		template < class X, class Y >
		del0(Y * pthis, 
			R (X::* function_to_bind)(  ))
			: type(pthis, function_to_bind)	{}

		template < class X, class Y >
		del0(const Y *pthis,
			R (X::* function_to_bind)(  ) const)
			: type(pthis, function_to_bind)	{}

		del0(R (*function_to_bind)(  ))
			: type(function_to_bind) {}

		virtual R operator() ()const { return type::operator() (); }
	};
	//--------------------------------//
	template<typename Param1, typename R>
	class del1 : public fastdelegate::FastDelegate1<Param1, R>, public idel1<Param1, R>
	{
	public:
		/* Some Class may want to use fast baseType without interface and virtuals
		 * so just create delegate with del0::type line
		*/
		typedef fastdelegate::FastDelegate1<Param1, R> type;
		DEFINE_TCLASS_SHARED_PTR(del1)
		virtual ~del1() {}

		del1() : type() {}

		template < class X, class Y >
		del1(Y * pthis, 
			R (X::* function_to_bind)( Param1 ))
			: type(pthis, function_to_bind)	{}

		template < class X, class Y >
		del1(const Y *pthis,
			R (X::* function_to_bind)( Param1 ) const)
			: type(pthis, function_to_bind)	{}

		del1(R (*function_to_bind)( Param1 ))
			: type(function_to_bind) {}

		virtual R operator() (Param1 p1)const { return type::operator() (p1); }
	};
	//--------------------------------//
	template<typename Param1, typename Param2, typename R>
	class del2 : public fastdelegate::FastDelegate2<Param1, Param2, R>
	{
	public:
		/* Some Class may want to use fast baseType without interface and virtuals
		 * so just create delegate with del0::type line
		*/
		typedef fastdelegate::FastDelegate2<Param1, Param2, R> type;
		DEFINE_TCLASS_SHARED_PTR(del2)
		virtual ~del2() {}

		del2() : type() {}

		template < class X, class Y >
		del2(Y * pthis, 
			R (X::* function_to_bind)( Param1, Param2 ))
			: type(pthis, function_to_bind)	{}

		template < class X, class Y >
		del2(const Y *pthis,
			R (X::* function_to_bind)( Param1, Param2 ) const)
			: type(pthis, function_to_bind)	{}

		del2(R (*function_to_bind)( Param1, Param2 ))
			: type(function_to_bind) {}

		virtual R operator() (Param1 p1, Param2 p2)const { return type::operator() (p1, p2); }
	};
	//--------------------------------//
#ifdef DBS_DELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX
	//--------------------------------//
	template<typename Signature>
	class del;
	//--------------------------------//
	template<typename R>
	class del<R ()> : public fastdelegate::FastDelegate0<R>, public idel<R ()>
	{
	public:
		/* Some Class may want to use fast baseType without interface and virtuals
		 * so just create delegate with del0::type line
		*/
		typedef fastdelegate::FastDelegate0<R> type;
		DEFINE_TCLASS_SHARED_PTR(del)
		virtual ~del() {}

		del() : type() {}

		template < class X, class Y >
		del(Y * pthis, 
			R (X::* function_to_bind)(  ))
			: type(pthis, function_to_bind)	{}

		template < class X, class Y >
		del(const Y *pthis,
			R (X::* function_to_bind)(  ) const)
			: type(pthis, function_to_bind)	{}

		del(R (*function_to_bind)(  ))
			: type(function_to_bind) {}

		virtual R operator() ()const { return type::operator() (); }
	};
	//--------------------------------//
	template<typename R, typename Param1>
	class del<R (Param1)> : public fastdelegate::FastDelegate1<Param1, R>, public idel<R (Param1)>
	{
	public:
		/* Some Class may want to use fast baseType without interface and virtuals
		 * so just create delegate with del0::type line
		*/
		typedef fastdelegate::FastDelegate1<Param1, R> type;
		DEFINE_TCLASS_SHARED_PTR(del)
		virtual ~del() {}

		del() : type() {}

		template < class X, class Y >
		del(Y * pthis, 
			R (X::* function_to_bind)( Param1 ))
			: type(pthis, function_to_bind)	{}

		template < class X, class Y >
		del(const Y *pthis,
			R (X::* function_to_bind)( Param1 ) const)
			: type(pthis, function_to_bind)	{}

		del(R (*function_to_bind)( Param1 ))
			: type(function_to_bind) {}

		virtual R operator() (Param1 p1)const { return type::operator() (p1); }
	};
	//--------------------------------//
	template<typename R, typename Param1, typename Param2>
	class del<R (Param1, Param2)> : public fastdelegate::FastDelegate2<Param1, Param2, R>, public idel<R (Param1, Param2)>
	{
	public:
		/* Some Class may want to use fast baseType without interface and virtuals
		 * so just create delegate with del0::type line
		*/
		typedef fastdelegate::FastDelegate2<Param1, Param2, R> type;
		DEFINE_TCLASS_SHARED_PTR(del)
		virtual ~del() {}

		del() : type() {}

		template < class X, class Y >
		del(Y * pthis, 
			R (X::* function_to_bind)( Param1, Param2 ))
			: type(pthis, function_to_bind)	{}

		template < class X, class Y >
		del(const Y *pthis,
			R (X::* function_to_bind)( Param1, Param2 ) const)
			: type(pthis, function_to_bind)	{}

		del(R (*function_to_bind)( Param1, Param2 ))
			: type(function_to_bind) {}

		virtual R operator() (Param1 p1, Param2 p2)const { return type::operator() (p1, p2); }
	};
	//--------------------------------//
#endif //DBS_DELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX
#endif //DBS_DELEGATE_TYPE == DBS_DELEGATE_TYPE_DONS_FAST
	//--------------------------------//
	class composite0 : public container< idel0<void> >, public idel0<void>
	{
	public:
		DEFINE_CLASS_SHARED_PTR(composite0)
		virtual ~composite0() {}
		virtual void operator() ()const {
			traits::del_ocit it = getDelIterator();
			while( it.hasMoreElements() )
				(*it.getNextCRef()) ();
		}
	};
	//--------------------------------//
	template<typename Param1>
	class composite1 : public container< idel1<Param1, void> >, public idel1<Param1, void>
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(composite1)
		virtual ~composite1() {}
		virtual void operator() (Param1 p1)const {
			traits::del_ocit it = getDelIterator();
			while( it.hasMoreElements() )
				(*it.getNextCRef()) (p1);
		}
	};
	//--------------------------------//
	template<typename Param1, typename Param2>
	class composite2 : public container< idel2<Param1, Param2, void> >, public idel2<Param1, Param2, void>
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(composite2)
		virtual ~composite2() {}
		virtual void operator() (Param1 p1, Param2 p2)const {
			traits::del_ocit it = getDelIterator();
			while( it.hasMoreElements() )
				(*it.getNextCRef()) (p1, p2);
		}
	};
	//--------------------------------//
#ifdef DBS_DELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX
	//--------------------------------//
	template<typename Signature>
	class composite;
	//--------------------------------//
	template<>
	class composite<void ()> : public container<idel<void ()>>, idel<void ()>
	{
	public:
		DEFINE_CLASS_SHARED_PTR(composite)
		virtual ~composite() {}
		virtual void operator() ()const {
			traits::del_ocit it = getDelIterator();
			while( it.hasMoreElements() )
				(*it.getNextCRef()) ();
		}
	};
	//--------------------------------//
	template<typename Param1>
	class composite<void (Param1)> : public container<idel<void (Param1)>>, idel<void (Param1)>
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(composite)
		virtual ~composite() {}
		virtual void operator() (Param1 p1)const {
			traits::del_ocit it = getDelIterator();
			while( it.hasMoreElements() )
				(*it.getNextCRef()) (p1);
		}
	};
	//--------------------------------//
	template<typename Param1, typename Param2>
	class composite<void (Param1, Param2)> : public container<idel<void (Param1, Param2)>>, idel<void (Param1, Param2)>
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(composite)
		virtual ~composite() {}
		virtual void operator() (Param1 p1, Param2 p2)const {
			traits::del_ocit it = getDelIterator();
			while( it.hasMoreElements() )
				(*it.getNextCRef()) (p1, p2);
		}
	};
	//--------------------------------//
#endif //DBS_DELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX
	//--------------------------------//
	template<typename R>
	class active_del0 : public del0<R>
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(active_del0)
		virtual ~active_del0() {}

		active_del0() : del0(), active(true) {}
		explicit active_del0(bool active) : del0(), active(active) {}

		template < class X, class Y >
		active_del0(Y * pthis, 
			R (X::* function_to_bind)(  ))
			: del0(pthis, function_to_bind)	{}

		template < class X, class Y >
		active_del0(const Y *pthis,
			R (X::* function_to_bind)(  ) const)
			: del0(pthis, function_to_bind)	{}

		active_del0(R (*function_to_bind)(  ))
			: del0(function_to_bind) {}

		virtual R operator() ()const { return type::operator() (); }

		bool active;
	};
	//--------------------------------//
	template<typename Param1, typename R>
	class active_del1 : public del1<Param1, R>
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(active_del1)
		virtual ~active_del1() {}

		active_del1() : del1(), active(true) {}
		explicit active_del1(bool active) : del1(), active(active) {}

		template < class X, class Y >
		active_del1(Y * pthis, 
			R (X::* function_to_bind)( Param1 ))
			: del1(pthis, function_to_bind)	{}

		template < class X, class Y >
		active_del1(const Y *pthis,
			R (X::* function_to_bind)( Param1 ) const)
			: del1(pthis, function_to_bind)	{}

		active_del1(R (*function_to_bind)( Param1 ))
			: del1(function_to_bind) {}

		virtual R operator() (Param1 p1)const { return type::operator() (p1); }

		bool active;
	};
	//--------------------------------//
	template<typename Param1, typename Param2, typename R>
	class active_del2 : public del2<Param1, Param2, R>
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(active_del2)
		virtual ~active_del2() {}

		active_del2() : del2(), active(true) {}
		explicit active_del2(bool active) : del2(), active(active) {}

		template < class X, class Y >
		active_del2(Y * pthis, 
			R (X::* function_to_bind)( Param1, Param2 ))
			: del2(pthis, function_to_bind)	{}

		template < class X, class Y >
		active_del2(const Y *pthis,
			R (X::* function_to_bind)( Param1, Param2 ) const)
			: del2(pthis, function_to_bind)	{}

		active_del2(R (*function_to_bind)( Param1, Param2 ))
			: del2(function_to_bind) {}

		virtual R operator() (Param1 p1, Param2 p2)const { return type::operator() (p1, p2); }

		bool active;
	};
	//--------------------------------//
#ifdef DBS_DELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX
	//--------------------------------//
	template<typename Signature>
	class active_del;
	//--------------------------------//
	template<typename R>
	class active_del<R ()> : public del<R ()>
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(active_del)
		virtual ~active_del() {}

		active_del() : del(), active(true) {}
		explicit active_del(bool active) : del(), active(active) {}

		template < class X, class Y >
		active_del(Y * pthis, 
			R (X::* function_to_bind)(  ))
			: del(pthis, function_to_bind)	{}

		template < class X, class Y >
		active_del(const Y *pthis,
			R (X::* function_to_bind)(  ) const)
			: del(pthis, function_to_bind)	{}

		active_del(R (*function_to_bind)(  ))
			: del(function_to_bind) {}

		virtual R operator() ()const { if(active) return type::operator() (); return R(); }

		bool active;
	};
	//--------------------------------//
	template<typename R, typename Param1>
	class active_del<R (Param1)> : public del<R (Param1)>
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(active_del)
		virtual ~active_del() {}

		active_del() : del(), active(true) {}
		explicit active_del(bool active) : del(), active(active) {}

		template < class X, class Y >
		active_del(Y * pthis, 
			R (X::* function_to_bind)( Param1 ))
			: del(pthis, function_to_bind)	{}

		template < class X, class Y >
		active_del(const Y *pthis,
			R (X::* function_to_bind)( Param1 ) const)
			: del(pthis, function_to_bind)	{}

		active_del(R (*function_to_bind)( Param1 ))
			: del(function_to_bind) {}

		virtual R operator() (Param1 p1)const { return type::operator() (p1); }

		bool active;
	};
	//--------------------------------//
	template<typename R, typename Param1, typename Param2>
	class active_del<R (Param1, Param2)> : public del<R (Param1, Param2)>
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(active_del)
		virtual ~active_del() {}

		active_del() : del(), active(true) {}
		explicit active_del(bool active) : del(), active(active) {}

		template < class X, class Y >
		active_del(Y * pthis, 
			R (X::* function_to_bind)( Param1, Param2 ))
			: del(pthis, function_to_bind)	{}

		template < class X, class Y >
		active_del(const Y *pthis,
			R (X::* function_to_bind)( Param1, Param2 ) const)
			: del(pthis, function_to_bind)	{}

		active_del(R (*function_to_bind)( Param1, Param2 ))
			: del(function_to_bind) {}

		virtual R operator() (Param1 p1, Param2 p2)const { return type::operator() (p1, p2); }

		bool active;
	};
	//--------------------------------//
#endif //DBS_DELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX
};
//------------------------------------------------------------
}
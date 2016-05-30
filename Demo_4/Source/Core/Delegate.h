#pragma once
#include "DemoPrerequisites.h"

#include <OgreIteratorWrapper.h>

#if DBS_DELEGATE_TYPE == DBS_DELEGATE_TYPE_DONS_FAST
#include "FastDelegate.h"
#endif

namespace Demo
{
namespace Core
{
	//------------------------------------------------------------
	/*	D - DelegateInterface
	 **/
	template< typename D >
	class DelegateContainer
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
		DelegateContainer() {}
		virtual ~DelegateContainer() {}

		inline bool empty()const { return delContainer.empty(); }
		inline void add( typename traits::del_in del ) { delContainer.insert( del ); }
		inline void remove( typename traits::del_in del ) { delContainer.erase( del ); }
		inline void clear() { delContainer.clear(); }
		inline void merge( const DelegateContainer<D>& in ) {
			traits::del_cont::const_iterator iter = in.delContainer.begin();
			traits::del_cont::const_iterator end_iter = in.delContainer.end();
			for( ; iter != end_iter; ++iter )
				delContainer.insert( *iter );
		}
		inline typename traits::del_oit getOgreIterator() { return typename traits::del_oit(delContainer); }
		inline typename traits::del_ocit getOgreIterator()const { return typename traits::del_ocit(delContainer); }
		
	protected:
		typename traits::del_cont delContainer;
	};
	//------------------------------------------------------------
	template< typename Signature >
	class DelegateComposite;
	//------------------------------------------------------------
	template<>
	class DelegateComposite< void () > : public DelegateInterface< void () >, public DelegateContainer< DelegateInterface< void () > >
	{
	public:
		//--------------------------------//
		typedef shared_ptr_traits< DelegateComposite >::shared_ptr shared_ptr;
		typedef shared_ptr_traits< DelegateComposite >::const_shared_ptr const_shared_ptr;
		//--------------------------------//
		virtual ~DelegateComposite() {}
		virtual void operator() () const
		{
			traits::del_ocit it = getOgreIterator();
			while( it.hasMoreElements() )
				(*it.getNext()) ();
		}
	};
	//------------------------------------------------------------
	template< typename Param1 >
	class DelegateComposite< void (Param1) > : public DelegateInterface< void (Param1) >, public DelegateContainer< DelegateInterface< void (Param1) > >
	{
	public:
		//--------------------------------//
		typedef typename shared_ptr_traits< DelegateComposite >::shared_ptr shared_ptr;
		typedef typename shared_ptr_traits< DelegateComposite >::const_shared_ptr const_shared_ptr;
		//--------------------------------//
		virtual ~DelegateComposite() {}
		virtual void operator() (Param1 p1) const
		{
			traits::del_ocit it = getOgreIterator();
			while( it.hasMoreElements() )
				(*it.getNext()) (p1);
		}
	};
	//------------------------------------------------------------
	template< typename Param1, typename Param2 >
	class DelegateComposite< void (Param1, Param2) > : public DelegateInterface< void (Param1, Param2) >, public DelegateContainer< DelegateInterface< void (Param1, Param2) > >
	{
	public:
		//--------------------------------//
		typedef typename shared_ptr_traits< DelegateComposite >::shared_ptr shared_ptr;
		typedef typename shared_ptr_traits< DelegateComposite >::const_shared_ptr const_shared_ptr;
		//--------------------------------//
		virtual ~DelegateComposite() {}
		virtual void operator() (Param1 p1, Param2 p2) const
		{
			traits::del_ocit it = getOgreIterator();
			while( it.hasMoreElements() )
				(*it.getNext()) (p1, p2);
		}
	};
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	template<typename Signature>
	class ActiveDelegate;
	//------------------------------------------------------------
	template<typename R>
	class ActiveDelegate< R() > : public Delegate< R() >
	{
	public:
		//--------------------------------//
		typedef typename shared_ptr_traits< ActiveDelegate >::shared_ptr shared_ptr;
		typedef typename shared_ptr_traits< ActiveDelegate >::const_shared_ptr const_shared_ptr;
		//--------------------------------//
		typedef Delegate< R() > base_type;
		typedef ActiveDelegate type;
		//--------------------------------//
		virtual ~ActiveDelegate() {}
		virtual R operator() () const { if(isActive) return invoke(); }

		template < class X, class Y >
		ActiveDelegate(Y * pthis, R (X::* function_to_bind)())
			: base_type(pthis, function_to_bind)
			, isActive(true) {}
		template < class X, class Y >
		ActiveDelegate(const Y *pthis, R (X::* function_to_bind)() const)
			: base_type(pthis, function_to_bind)
			, isActive(true) {}
		ActiveDelegate(R (*function_to_bind)())
			: base_type(function_to_bind)
			, isActive(true) {}
		ActiveDelegate( const ActiveDelegate &dg )
			: base_type(dg)
			, isActive(dg.isActive) {}
		inline void operator= ( const ActiveDelegate &dg ) {
			isActive = dg.isActive;
			*static_cast<base_type>(this) = dg;	}

		bool isActive;
	};
	//------------------------------------------------------------
} // namespace Core
} // namespace Demo
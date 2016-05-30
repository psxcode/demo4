#pragma once
#include "Core\Event.h"

namespace Demo
{
namespace Core
{
	//------------------------------------------------------------
	class ICallback
	{
	public:
		DEFINE_CLASS_SHARED_PTR(ICallback)
		//--------------------------------//
		class traits
		{
		public:
			typedef IEvent::traits::event_compare_in event_check_in;
		};
		//--------------------------------//
		virtual ~ICallback() {}
		virtual bool checkEvent( traits::event_check_in )const = 0;
		virtual bool checkEventNoCall( traits::event_check_in )const = 0;
	};
	//------------------------------------------------------------
	template<typename T>
	class Callback : public ICallback
	{
	public:
		DEFINE_CLASS_SHARED_PTR(Callback<T>)
		//--------------------------------//
		class traits : public ICallback::traits
		{
		public:
			typedef Event<T>::const_shared_ptr						event_t;
			typedef delegate_traits::composite0						comp0;
			typedef delegate_traits::composite1<event_t const &>	comp1;
			typedef comp0::traits::del_in							del0_in;
			typedef comp1::traits::del_in							del1_in;
		};
		//--------------------------------//
		explicit Callback( traits::event_t evt )
			: compareEvent(evt)
		{
		}
		virtual ~Callback() {}
		virtual bool checkEvent( traits::event_check_in evt )const {
			if( compareEvent->compare(evt) ) { _call(); return true; }
			else return false;
		}
		virtual bool checkEventNoCall( traits::event_check_in evt )const {
			return compareEvent->compare(evt);
		}

		bool checkEvent( const Event<T> &evt )const {
			if( (*compareEvent) == evt ) { _call(); return true; }
			else return false;
		}
		bool checkEventNoCall( const Event<T> &evt )const { 
			return (*compareEvent) == evt;
		}
		void addDelegate( traits::del0_in del ) { delComp0.add(del); }
		void addDelegate( traits::del1_in del ) { delComp1.add(del); }
		void removeDelegate( traits::del0_in del ) { delComp0.remove(del); }
		void removeDelegate( traits::del1_in del ) { delComp1.remove(del); }
		void clear(){ delComp0.clear(); delComp1.clear(); }
		void merge( Callback const &call ){ delComp0.merge(call->delComp0); delComp1.merge(call->delComp1); }		
		traits::event_t getCompareEvent()const { return compareEvent; }
		bool empty()const { return delComp0.empty() && delComp1.empty(); }
		void resetEvent( traits::event_t evt ) { compareEvent = evt; }

	protected:
		void _call()const {
			delComp0();
			delComp1( compareEvent );
		}
		traits::event_t compareEvent;
		traits::comp0 delComp0;
		traits::comp1 delComp1;

	private:
		Callback( const Callback& );
		Callback& operator=( const Callback& );
	};
	//------------------------------------------------------------
}
}
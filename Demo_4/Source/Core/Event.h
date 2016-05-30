#pragma once

#include "Tagged.h"

namespace Demo
{
namespace Core
{
	//------------------------------------------------------------
	class IEvent : public Tagged<IEvent>
	{
	public:
		DEFINE_CLASS_SHARED_PTR(IEvent)
		//--------------------------------//
		class traits
		{
		public:
			typedef type_traits<IEvent>::cdata_ptr event_compare_in;
		};
		//--------------------------------//
		DEFINE_TAGGED()
		virtual ~IEvent() {}
		virtual bool compare( traits::event_compare_in )const = 0;
	};
	//------------------------------------------------------------
	template<typename T>
	class _DemoExport Event : public IEvent
	{
	public:
		DEFINE_CLASS_SHARED_PTR(Event<T>)
		//--------------------------------//
		typedef Event<T> this_type;
		Event() {}
		DEFINE_TAGGED()
		virtual ~Event() {}
		virtual bool compare( traits::event_compare_in evt ) {
			_ASSERT( evt );
			if( this->Tag() != evt->Tag() ) return false;
			this_type *ptr = dynamic_cast<this_type>(evt);
			if( !ptr ) return false;
			return this->event_data == ptr->event_data;
		}
		T& getEventData() { return event_data; }
		const T& getEventData()const { return event_data; }

	protected:
		T event_data;

	private:
		Event();
		Event( const Event& );
		Event& operator=( const Event& );
	};
	//------------------------------------------------------------
}
}
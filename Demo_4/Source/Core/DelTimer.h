#pragma once
#include "Core\ValueFilter.h"
#include "delegate_traits.h"

namespace Demo
{
namespace Core
{
	//------------------------------------------------------------
	class DelTimer
	{
	public:
		//--------------------------------//
		class traits
		{
		public:
			typedef delegate_traits::composite0::traits::del_in del_in;
		};
		//--------------------------------//
		DelTimer()
			: work( false )
			, autoReset( false )
			, pause( false )
			, currentTime( 0.0f )
			, targetTime( 1.0f )
			, tickTime(0)
		{
			operateDel.bind( new delegate_traits::del0<void>( this, &DelTimer::operateNoWork ) );
		}
		~DelTimer(){ CloseDelTimer(); }

		void CloseDelTimer() {
			operateDel->bind( this, &DelTimer::operateNoWork );
			currentTime = 0.0f;
			targetTime = 1.0f;
			tickTime = 0;
			pause = false;
			autoReset = false;
			work = false;

			contCall.clear();
			onceCall.clear();
			tempCall.clear();
		}

		inline void Operate(){ Operate( TimeInterface::frameTime() ); }
		inline void Operate( float time ){	tickTime = time; (*operateDel)(); }
		inline void SetTargetTime( float time ){ targetTime = time; }
		inline void SetAutoReset( bool value = true ){ autoReset = value; }
		inline void SetPause( bool value = true ) {
			pause = value;
			operateDel->bind( this, pause ? &DelTimer::operatePause : work ? &DelTimer::operateWork : &DelTimer::operateNoWork );
		}
		inline void Stop() {
			currentTime = 0;
			pause = false;
			setWork( false );
			tempCall.clear();
		}
		inline void Reset( float time ){ SetTargetTime( time ); Reset(); }
		inline void Reset(){ currentTime = 0; pause = false; setWork(); }
		inline float GetCurrentTime()const{ return currentTime; }
		inline float GetCurrentTimePercentage()const{ return currentTime / targetTime; }
		inline float GetTargetTime()const{ return targetTime; }
		inline float GetTimeLeft()const{ return targetTime - currentTime; }
		inline bool IsWorking()const{ return work; }
		inline bool IsAutoReseting()const{ return autoReset; }

		inline void AddContDel( traits::del_in del ){ contCall.add( del ); }
		inline void AddOnceDel( traits::del_in del ){ onceCall.add( del ); }
		inline void AddTempDel( traits::del_in del ){ tempCall.add( del ); }

		inline void RemoveContDel( traits::del_in del ){ contCall.remove( del ); }
		inline void RemoveOnceDel( traits::del_in del ){ onceCall.remove( del ); }
		inline void RemoveTempDel( traits::del_in del ){ tempCall.remove( del ); }

		inline void RemoveAllContDels(){ contCall.clear(); }
		inline void RemoveAllOnceDels(){ onceCall.clear(); }
		inline void RemoveAllTempDels(){ tempCall.clear(); }

	protected:
		void operateWork() {
			currentTime += tickTime;
			if( currentTime > targetTime ) {
				setWork( false );
				if( autoReset )	Reset();
				contCall();
				onceCall();
				tempCall();
				tempCall.clear();
			}
		}
		void operateNoWork() {
			contCall();
			tempCall();
			tempCall.clear();
		}
		void operatePause(){ }
		void setWork( bool w = true ) {
			work = w;
			operateDel->bind( this, work ? &DelTimer::operateWork : &DelTimer::operateNoWork );
		}

		delegate_traits::del0<void>::shared_ptr operateDel;

		float currentTime;
		float targetTime;
		float tickTime;
		bool work;
		bool autoReset;
		bool pause;

		delegate_traits::composite0 contCall;
		delegate_traits::composite0 onceCall;
		delegate_traits::composite0 tempCall;

	private:
		DelTimer( const DelTimer& );
		DelTimer& operator=( const DelTimer& );
	};
	//------------------------------------------------------------
}
}
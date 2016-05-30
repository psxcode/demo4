#pragma once
#include "Core\RangeVar.h"
#include "Interface\UnitConverter.h"
#include "Interface\TimeInterface.h"

namespace Demo
{
namespace Core
{
	//------------------------------------------------------------
	template< class T >
	class _DemoExport ValueFilter
	{
	public:
		//--------------------------------//
		class traits
		{
		public:
			typedef typename delegate_traits::composite0::traits::del_in del_in;
		};
		//--------------------------------//
		ValueFilter()
			: work( false )
			, enableDelegates( true )
			, pause( false )
			, minValue( 0.0f )
			, maxValue( 1.0f )
			, targetValue( 0.0f )
			, currentValue( 0.0f )
			, speed( 1.0f )
		{
		}
		virtual ~ValueFilter()
		{
		}

		inline void Operate(){ Operate( System::TimeInterface::frameTime() ); }
		virtual void Operate( float time ) = 0;
		virtual void SetMinValue( T value )
		{
			minValue = value;
			work = false;
			if( minValue > currentValue )
				currentValue = minValue;

			if( minValue > maxValue )
				maxValue = minValue;

			if( minValue > targetValue )
				targetValue = minValue;
		}
		virtual void SetMaxValue( T value )
		{
			maxValue = value;
			work = false;
			if( maxValue < minValue )
				minValue = maxValue;

			if( currentValue > maxValue )
				currentValue = maxValue;

			if( targetValue > maxValue )
				targetValue = maxValue;
		}
		virtual void SetSpeed( T valuePerSecond )
		{
			if( valuePerSecond < 0 )
				valuePerSecond *= -1;
		}
		virtual void SetTargetValue( float value )
		{
			targetValue = value;
			Core::RangeVar<float>::SetToRange( minValue, maxValue, targetValue );
			work = true;
		}
		virtual void SetCurrentValue( float value )
		{
			currentValue = value;
			Core::RangeVar<float>::SetToRange( minValue, maxValue, currentValue );
			work = true;
		}

		inline T GetMinValue()const{ return minValue; }
		inline T GetMaxValue()const{ return maxValue; }
		inline T GetTargetValue()const{ return targetValue; }
		inline T GetCurrentValue()const{ return currentValue; }
		inline T GetSpeed()const{ return speed; }
		bool IsWorking()const{ return work; }
		bool IsIncreasing()const{ return work && targetValue > currentValue; }
		bool IsDecreasing()const{ return work && targetValue < currentValue; }

		//if paused, currentValue is not changing
		void SetPause( bool p = true ){ pause = p; }
		bool IsPaused()const{ return pause; }

		inline void AddContDel( typename traits::del_in del ){ contCall.addDelegate( del ); }
		inline void AddOnceDel( typename traits::del_in del ){ onceCall.addDelegate( del ); }
		inline void AddTempDel( typename traits::del_in del ){ tempCall.addDelegate( del ); }
		inline void RemoveContDel( typename traits::del_in del ){ contCall.removeDelegate( del ); }
		inline void RemoveOnceDel( typename traits::del_in del ){ onceCall.removeDelegate( del ); }
		inline void RemoveTempDel( typename traits::del_in del ){ tempCall.removeDelegate( del ); }
		inline void RemoveAllContDels(){ contCall.removeAllDelegates(); }
		inline void RemoveAllOnceDels(){ onceCall.removeAllDelegates(); }
		inline void RemoveAllTempDels(){ tempCall.removeAllDelegates(); }

		inline void EnableDelegates( bool enable = true ){ enableDelegates = enable; }

	protected:

		delegate_traits::composite0 contCall;
		delegate_traits::composite0 onceCall;
		delegate_traits::composite0 tempCall;

		T minValue;
		T maxValue;
		T currentValue;
		T targetValue;
		T speed;//value change per second
		bool work;
		bool enableDelegates;
		bool pause;

	private:
		ValueFilter( const ValueFilter& );
		ValueFilter& operator=( const ValueFilter& );
	};
	//------------------------------------------------------------
	typedef ValueFilter< float > FloatValueFilter;
	typedef ValueFilter< int > IntValueFilter;
	//------------------------------------------------------------
	class _DemoExport LinearFloatFilter : public FloatValueFilter
	{
	public:
		virtual ~LinearFloatFilter()
		{
		}

		virtual void Operate( float time )
		{
			if( !work )
			{
				if( enableDelegates )
				{
					contCall();
					if( !tempCall.empty() )
					{
						tempCall();
						tempCall.clear();
					}
				}
				return;
			}
			if( pause )return;

			if ( targetValue > currentValue )
			{
				currentValue = currentValue + speed * time;
				if ( currentValue > targetValue )
					TargetValueReached();
			}
			else 
			{
				currentValue = currentValue - speed * time;
				if ( currentValue < targetValue )
					TargetValueReached();
			}
		}
		
	protected:
		void TargetValueReached()
		{
			currentValue = targetValue;
			work = false;

			if( enableDelegates )
			{
				contCall();
				onceCall();
				if( !tempCall.empty() )
				{
					tempCall();
					tempCall.clear();
				}
			}
		}
	};
	//------------------------------------------------------------
}
}
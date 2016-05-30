#pragma once
#include "Core\RangeVar.h"
#include "IPedalControl.h"
#include "IPedalCallback.h"
#include "IPedalState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------	
	class PedalFilter : public Core::puFloat
	{
	public:
		//--------------------------------//
		DEFINE_CLASS_SHARED_PTR(PedalFilter)
		typedef delegate_traits::composite0::traits::del_in del_in;
		//--------------------------------//
		static const float timeLimit;
		static const float valueLimit;
		//--------------------------------//
		PedalFilter()
			: pressSpeed(0)
			, pressTime(0)
			, releaseSpeed(0)
			, releaseTime(0)
			, minLimitValue(minVal)
			, maxLimitValue(maxVal)
			, targetValue(0)
			, isRising( false )
			, isPressed( false )
			, isFullReleased( true )
			, isWorking( false )
		{
		}
		virtual ~PedalFilter()
		{
		}

		void Operate();
		IPedal::shared_ptr GetInterface( PEDAL_INTERFACE_TYPE );
		IPedalControl::shared_ptr GetControlInterface();
		IPedalCallback::shared_ptr GetCallbackInterface();
		IPedalState::shared_ptr GetStateInterface();

		//Интерфейс для IPedalControl
		void Press();
		void Press( const Core::puFloat& value );
		void Release();
		void SetPressTime( float time );
		void SetReleaseTime( float time );
		void SetMinLimit( float value );
		void SetMaxLimit( float value );
		//Интерфейс для IPedalControl

		//Интерфейс для IPedalState
		inline const Core::puFloat& Get()const{ return *this; }
		inline bool IsWorking()const{ return isWorking; }
		inline bool IsFullPressed()const{ return (maxVal - var) < valueLimit; }
		inline bool IsFullReleased()const{ return isFullReleased; }
		inline float GetPressSpeed()const{ return pressSpeed; }
		inline float GetReleaseSpeed()const{ return releaseSpeed; }
		inline float GetCurrentValue()const{ return var; }
		inline float GetTargetValue()const{ return targetValue; }
		//Интерфейс для IPedalState

		//Интерфейс для IPedalCallback
		inline void AddPressedOnceDel( del_in del ){ pressedOnceCall.add( del ); }
		inline void AddPressedTempDel( del_in del ){ pressedTempCall.add( del ); }
		inline void AddReleasedOnceDel( del_in del ){ releasedOnceCall.add( del ); }
		inline void AddReleasedTempDel( del_in del ){ releasedTempCall.add( del ); }
		inline void RemovePressedOnceDel( del_in del ){ pressedOnceCall.remove( del ); }
		inline void RemovePressedTempDel( del_in del ){ pressedTempCall.remove( del ); }
		inline void RemoveReleasedOnceCall( del_in del ){ releasedOnceCall.remove( del ); }
		inline void RemoveReleasedTempCall( del_in del ){ releasedTempCall.remove( del ); }
		//Интерфейс для IPedalCallback

		void Reset()
		{
			pressSpeed = 0;
			pressTime = 0;
			releaseSpeed = 0;
			releaseTime = 0;
			minLimitValue = minVal;
			maxLimitValue = maxVal;
			var = minVal;
			targetValue = 0;
			isRising = false;
			isPressed = false;
			isFullReleased = true;
			isWorking = false;

			pressedTempCall.clear();
			releasedTempCall.clear();
			pressedOnceCall.clear();
			releasedOnceCall.clear();
		}

	protected:
		void TargetValueReached();
		float calcSpeed( float time )const;
		void recalcSpeed();

		float pressSpeed;
		float pressTime;
		float releaseSpeed;
		float releaseTime;
		float targetValue;
		float minLimitValue;
		float maxLimitValue;
		bool isRising;
		bool isPressed;
		bool isFullReleased;
		bool isWorking;

		delegate_traits::composite0 pressedTempCall;
		delegate_traits::composite0 releasedTempCall;
		delegate_traits::composite0 pressedOnceCall;
		delegate_traits::composite0 releasedOnceCall;

		IPedalControl::shared_ptr iControl;
		IPedalCallback::shared_ptr iCallback;
		IPedalState::shared_ptr iState;

	private:
		PedalFilter( const PedalFilter& );
		PedalFilter& operator=( const PedalFilter& );
	};
	//------------------------------------------------------------
	class SteerFilter : public Core::npuFloat
	{
	public:
		DEFINE_CLASS_SHARED_PTR(SteerFilter)
		//--------------------------------//
		typedef delegate_traits::composite0::traits::del_in del_in;
		//--------------------------------//
		static const float timeLimit;
		static const float valueLimit;
		//--------------------------------//
		SteerFilter()
			: pressSpeed(0)
			, pressTime(0)
			, releaseSpeed(0)
			, releaseTime(0)
			, leftLimitValue(minVal)
			, rightLimitValue(maxVal)
			, targetValue(0)
			, isRising( false )
			, isPressed( false )
			, isFullReleased( true )
			, isWorking( false )
		{
		}
		virtual ~SteerFilter()
		{
		}

		void Operate();
		IPedal::shared_ptr GetInterface( PEDAL_INTERFACE_TYPE );
		ISteerControl::shared_ptr GetControlInterface();
		ISteerCallback::shared_ptr GetCallbackInterface();
		ISteerState::shared_ptr GetStateInterface();

		//Интерфейс для ISteerControl
		void Press( const Core::npuFloat& value );
		void PressRight();
		void PressLeft();
		void Release();
		void SetPressTime( float time );
		void SetReleaseTime( float time );
		void SetLimit( float value );
		//Интерфейс для ISteerControl
		
		//Интерфейс для ISteerState
		inline Core::npuFloat Get()const{ return *this; }
		inline float GetPressSpeed()const{ return pressSpeed; }
		inline float GetReleaseSpeed()const{ return releaseSpeed; }
		inline float GetCurrentValue()const{ return var; }
		inline float GetTargetValue()const{ return targetValue; }
		inline bool IsWorking()const{ return isWorking; }
		inline bool IsFullReleased()const{ return isFullReleased; };
		bool IsFullPressed()const;
		//Интерфейс для ISteerState

		//Интерфейс для ISteerCallback
		inline void AddPressedOnceDel( del_in del ){ pressedOnceCall.add( del ); }
		inline void AddPressedTempDel( del_in del ){ pressedTempCall.add( del ); }
		inline void AddReleasedOnceDel( del_in del ){ releasedOnceCall.add( del ); }
		inline void AddReleasedTempDel( del_in del ){ releasedTempCall.add( del ); }
		inline void RemovePressedOnceDel( del_in del ){ pressedOnceCall.remove( del ); }
		inline void RemovePressedTempDel( del_in del ){ pressedTempCall.remove( del ); }
		inline void RemoveReleasedOnceCall( del_in del ){ releasedOnceCall.remove( del ); }
		inline void RemoveReleasedTempCall( del_in del ){ releasedTempCall.remove( del ); }
		//Интерфейс для ISteerCallback

		void Reset()
		{
			pressSpeed = 0;
			pressTime = 0;
			releaseSpeed = 0;
			releaseTime = 0;
			var = 0;
			targetValue = 0;
			leftLimitValue = 0;
			rightLimitValue = 0;
			isRising = false;
			isPressed = false;
			isFullReleased = true;
			isWorking = false;

			pressedTempCall.clear();
			releasedTempCall.clear();
			pressedOnceCall.clear();
			releasedOnceCall.clear();
		}

	protected:
		void TargetValueReached();
		float calcSpeed( float time )const;
		void recalcSpeed();

		float pressSpeed;
		float pressTime;
		float releaseSpeed;
		float releaseTime;
		float targetValue;
		float leftLimitValue;
		float rightLimitValue;

		/*
		Показывает что var растёт в численном смысле
		Используется чтобы сразу использовать + или - для var
		*/
		bool isRising;
		/*
		Показывает что targetValue установлен через Press()
		*/
		bool isPressed;
		/*
		Показывает что var находится вблизи нуля, то усть полностью отпущен.
		Переменная станет true даже если ноль был достигнут через Press( 0 )
		*/
		bool isFullReleased;
		/*
		Показывает что var не достиг ещё targetValue, обозначает процесс работы
		*/
		bool isWorking;

		delegate_traits::composite0 pressedTempCall;
		delegate_traits::composite0 releasedTempCall;
		delegate_traits::composite0 pressedOnceCall;
		delegate_traits::composite0 releasedOnceCall;

		ISteerControl::shared_ptr iControl;
		ISteerCallback::shared_ptr iCallback;
		ISteerState::shared_ptr iState;

	private:
		SteerFilter( const SteerFilter& );
		SteerFilter& operator=( const SteerFilter& );
	};
	//------------------------------------------------------------
}//namespace Driver
}//namespace Demo
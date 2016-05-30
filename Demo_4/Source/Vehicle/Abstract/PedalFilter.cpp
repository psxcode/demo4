#include "DemoStableHeaders.h"
#if DBS_LM_ABSTRACT_VEHICLE

#include "PedalFilter.h"
#include "Interface/TimeInterface.h"

namespace Demo
{
	//------------------------------------------------------------
	const float Core::puFloat::minVal = 0.0f;
	const float Core::puFloat::maxVal = 1.0f;
	const float Core::npuFloat::minVal = -1.0f;
	const float Core::npuFloat::maxVal = 1.0f;
	//------------------------------------------------------------
namespace Vehicle
{
	//------------------------------------------------------------	
	const float PedalFilter::timeLimit = 0.001f;
	const float PedalFilter::valueLimit = 0.01f;
	const float SteerFilter::timeLimit = 0.001f;
	const float SteerFilter::valueLimit = 0.01f;
	//------------------------------------------------------------
	void PedalFilter::SetPressTime( float time )
	{
		pressTime = ((time < timeLimit) ? timeLimit : time);
		pressSpeed = calcSpeed( time );
	}
	//------------------------------------------------------------
	void PedalFilter::SetReleaseTime( float time )
	{
		releaseTime = ((time < timeLimit) ? timeLimit : time);
		releaseSpeed = calcSpeed( time );
	}
	//------------------------------------------------------------
	float PedalFilter::calcSpeed( float time )const
	{
		float range = maxLimitValue - minLimitValue;
		if( range < valueLimit )range = valueLimit;
		return range / ((time < timeLimit) ? timeLimit : time);
	}
	//------------------------------------------------------------
	void PedalFilter::recalcSpeed()
	{
		pressSpeed = calcSpeed( pressTime );
		releaseSpeed = calcSpeed( releaseTime );
	}
	//------------------------------------------------------------
	void PedalFilter::Operate()
	{
		if( isWorking )
		{
			if( isRising )
			{
				var += pressSpeed * TimeInterface::frameTime();
				if( var > targetValue )
					TargetValueReached();
			}
			else
			{
				var -= releaseSpeed * TimeInterface::frameTime();
				if( var < targetValue )
					TargetValueReached();
			}
		}
		else
		{
			if( isPressed && !pressedTempCall.empty() )
			{
				pressedTempCall();
				pressedTempCall.clear();
			}
			if( isFullReleased && !releasedTempCall.empty() )
			{
				releasedTempCall();
				releasedTempCall.clear();
			}
		}
	}
	//------------------------------------------------------------
	void PedalFilter::Press()
	{
		targetValue = maxLimitValue;
		isRising = true;
		isWorking = true;
		isPressed = true;
		isFullReleased = false;
	}
	//------------------------------------------------------------
	void PedalFilter::Press( const Core::puFloat &value )
	{
		targetValue = value.get();

		if( targetValue < minLimitValue )targetValue = minLimitValue;
		else if( targetValue > maxLimitValue )targetValue = maxLimitValue;

		if( targetValue > var )
			isRising = true;
		else isRising = false;
		isWorking = true;
		isPressed = true;
		isFullReleased = false;
	}
	//------------------------------------------------------------
	void PedalFilter::Release()
	{
		targetValue = minLimitValue;
		isRising = false;
		isWorking = true;
		isPressed = false;
	}
	//------------------------------------------------------------
	void PedalFilter::TargetValueReached()
	{
		var = targetValue;
		if( isPressed )
		{
			pressedOnceCall();
			pressedTempCall();
			pressedTempCall.clear();
		}
		if( var < valueLimit )
		{
			isFullReleased = true;
			releasedOnceCall();
			releasedTempCall();
			releasedTempCall.clear();
		}
		isWorking = false;
	}
	//------------------------------------------------------------
	void PedalFilter::SetMinLimit( float value )
	{
		if( value < minVal ) {
			minLimitValue = minVal;
		}
		else if( value > maxLimitValue - valueLimit ) {
			minLimitValue = maxLimitValue - valueLimit;
		}
		else {
			minLimitValue = value;
		}
		recalcSpeed();

		//update targetValue
		if( targetValue < minLimitValue )
			targetValue = minLimitValue;
		if( var < targetValue )
			var = targetValue;
	}
	//------------------------------------------------------------
	void PedalFilter::SetMaxLimit( float value )
	{
		if( value > maxVal ) {
			maxLimitValue = maxVal;
		}
		else if( value < minLimitValue + valueLimit ) {
			maxLimitValue = minLimitValue + valueLimit;
		}
		else {
			maxLimitValue = value;
		}
		recalcSpeed();

		//update targetValue
		if( targetValue > maxLimitValue )
			targetValue = maxLimitValue;
		if( var > targetValue )
			var = targetValue;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	void SteerFilter::Press( const Core::npuFloat &value )
	{
		targetValue = value.get();
		if( targetValue > var )
			isRising = true;
		else isRising = false;
		isPressed = true;
		isFullReleased = false;
		isWorking = true;
	}
	//------------------------------------------------------------
	void SteerFilter::PressRight()
	{
		targetValue = rightLimitValue;
		isPressed = true;
		isWorking = true;
		isRising = true;
		isFullReleased = false;
	}
	//------------------------------------------------------------
	void SteerFilter::PressLeft()
	{
		targetValue = leftLimitValue;
		isPressed = true;
		isWorking = true;
		isRising = false;
		isFullReleased = false;
	}
	//------------------------------------------------------------
	void SteerFilter::Release()
	{
		targetValue = 0.0f;
		isPressed = false;
		isWorking = true;
		var > 0 ? isRising = false : isRising = true;
	}
	//------------------------------------------------------------
	void SteerFilter::Operate()
	{
		//Если работаем
		if( isWorking )
		{
			float speed = TimeInterface::frameTime() * (isPressed ? pressSpeed : releaseSpeed);
			if( isRising )
			{
				var += speed;
				if( var > targetValue )
					TargetValueReached();
			}
			else
			{
				var -= speed;
				if( var < targetValue )
					TargetValueReached();
			}
		}
		else
		{
			if( isPressed && !pressedTempCall.empty() )
			{
				pressedTempCall();
				pressedTempCall.clear();
			}
			if( isFullReleased && !pressedTempCall.empty() )
			{
				releasedTempCall();
				releasedTempCall.clear();
			}
		}
	}
	//------------------------------------------------------------
	void SteerFilter::TargetValueReached()
	{
		var = targetValue;
		if( isPressed )
		{
			pressedOnceCall();
			pressedTempCall();
			pressedTempCall.clear();
		}

		if( (-valueLimit <= var && var <= 0.0f) || (0.0f <= var && var <= valueLimit)  )
			isFullReleased = true;

		if( isFullReleased )
		{
			releasedTempCall();
			releasedTempCall.clear();
		}

		isWorking = false;
	}
	//------------------------------------------------------------
	bool SteerFilter::IsFullPressed()const
	{
		return (minVal - var) > -valueLimit || (maxVal - var) < valueLimit;
	}
	//------------------------------------------------------------
	void SteerFilter::SetPressTime(float time)
	{
		pressTime = ((time < timeLimit) ? timeLimit : time);
		pressSpeed = calcSpeed( time );
	}
	//------------------------------------------------------------
	void SteerFilter::SetReleaseTime(float time)
	{
		releaseTime = ((time < timeLimit) ? timeLimit : time);
		releaseSpeed = calcSpeed( time );
	}
	//------------------------------------------------------------
	float SteerFilter::calcSpeed( float time )const
	{
		return rightLimitValue / ((time < timeLimit) ? timeLimit : time);
	}
	//------------------------------------------------------------
	void SteerFilter::recalcSpeed()
	{
		pressSpeed = calcSpeed( pressTime );
		releaseSpeed = calcSpeed( releaseTime );
	}
	//------------------------------------------------------------
	IPedal::shared_ptr PedalFilter::GetInterface( PEDAL_INTERFACE_TYPE type )
	{
		switch( type )
		{
		case PIT_PCONTROL: return GetControlInterface();
		case PIT_PCALLBACK: return GetCallbackInterface();
		case PIT_PSTATE: return GetStateInterface();
		default: return IPedal::shared_ptr();
		}
	}
	//------------------------------------------------------------
	IPedalControl::shared_ptr PedalFilter::GetControlInterface()
	{
		if( shared_ptr_null(iControl) ) iControl.bind( new IPedalControl( *this ) );
		return iControl;
	}
	//------------------------------------------------------------
	IPedalCallback::shared_ptr PedalFilter::GetCallbackInterface()
	{
		if( shared_ptr_null(iCallback) ) iCallback.bind( new IPedalCallback( *this ) );
		return iCallback;
	}
	//------------------------------------------------------------
	IPedalState::shared_ptr PedalFilter::GetStateInterface()
	{
		if( shared_ptr_null(iState) ) iState.bind( new IPedalState( *this ) );
		return iState;
	}
	//------------------------------------------------------------
	ISteerControl::shared_ptr SteerFilter::GetControlInterface()
	{
		if( shared_ptr_null(iControl) ) iControl.bind( new ISteerControl( *this ) );
		return iControl;
	}
	//------------------------------------------------------------
	ISteerCallback::shared_ptr SteerFilter::GetCallbackInterface()
	{
		if( shared_ptr_null(iCallback) ) iCallback.bind( new ISteerCallback( *this ) );
		return iCallback;
	}
	//------------------------------------------------------------
	ISteerState::shared_ptr SteerFilter::GetStateInterface()
	{
		if( shared_ptr_null(iState) ) iState.bind( new ISteerState( *this ) );
		return iState;
	}
	//------------------------------------------------------------
	IPedal::shared_ptr SteerFilter::GetInterface( PEDAL_INTERFACE_TYPE type )
	{
		switch( type )
		{
		case PIT_SCONTROL: return GetControlInterface();
		case PIT_SCALLBACK: return GetCallbackInterface();
		case PIT_SSTATE: return GetStateInterface();
		default: return IPedal::shared_ptr();
		}
	}
	//------------------------------------------------------------
	void SteerFilter::SetLimit(float value)
	{
		if( value > maxVal ) {
			value = maxVal;
		}
		else if( value < valueLimit ) {
			value = valueLimit;
		}
		rightLimitValue = value;
		leftLimitValue = -value;
		recalcSpeed();

		//update targetValue
		if( targetValue < leftLimitValue )
			targetValue = leftLimitValue;
		else if( targetValue > rightLimitValue )
			targetValue = rightLimitValue;
	}
	//------------------------------------------------------------
}//namespace Driver
}//namespace Demo
#endif //DBS_LM_ABSTRACT_VEHICLE
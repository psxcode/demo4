#include "MotorRPMZoneController.h"
#include "System\DistanceUnitInterface.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	typedef System::FrameTimeInterface FTI;
	typedef System::DistanceUnitInterface DUI;
	//------------------------------------------------------------
	const float MotorRPMZoneController::timeLimit = 0.001f;
	const float MotorRPMZoneController::rpmLimit = 10.0f;
	//------------------------------------------------------------
	bool RPMZoneTiming::isValid()const
	{
		return
			zoneReactionTime > MotorRPMZoneController::timeLimit &&
			speedReactionTime > MotorRPMZoneController::timeLimit;
	}
	//------------------------------------------------------------
	bool MotorRPMZoneController::Initialize( const InitData& init )
	{
		if( initializedController )return false;
		if( !init.isValid() )return false;
		rpmData = init.rpmData;
		timing = init.timing;
		motorState = init.motorState;
		
		//delegate creation
		pZoneTimerDel = new ThisVoidDelegate( this, &MotorRPMZoneController::OnZoneTimer );
		pSpeedTimerDel = new ThisVoidDelegate( this, &MotorRPMZoneController::OnSpeedTimer );
		zoneTimer.AddOnceDel( pZoneTimerDel );
		speedTimer.AddOnceDel( pSpeedTimerDel );

		zoneTimer.Reset( timing->GetRpmZoneReactionTime() );
		zoneTimer.SetAutoReset( false );
		speedTimer.SetAutoReset( false );

		return initializedController = true;
	}
	//------------------------------------------------------------
	void MotorRPMZoneController::CloseMotorRPMZoneController()
	{
		rpmSpeed = 0;
		spFirstRpm = 0;
		spSecondRpm = 0;
		motorState = 0;
		rpmData = 0;

		belowMin.removeAllDelegates();
		belowNorm.removeAllDelegates();
		norm.removeAllDelegates();
		aboveNorm.removeAllDelegates();
		aboveMax.removeAllDelegates();
		speed.removeAllDelegates();
		stall.removeAllDelegates();

		zoneTimer.RemoveAllOnceDels();
		speedTimer.RemoveAllOnceDels();
		if( pZoneTimerDel ){ delete pZoneTimerDel; pZoneTimerDel = 0; }
		if( pSpeedTimerDel ){ delete pSpeedTimerDel; pSpeedTimerDel = 0; }
		if( timing ){ delete timing; timing = 0; }
		
		Controller::CloseController();
	}
	//------------------------------------------------------------
	/*
	�������� �������� ��������� RPM
	��� ���� ������������ ����� �� ��������
	timeMultiplier ����������� ��� ��������� ������������ ������� �������
	*/
	void MotorRPMZoneController::MeasureSpeed( float timeMult )
	{
		//���� ����� ������������
		if( isMeasuringSpeed )
		{
			//���� ����� ����� ������ ������ �������
			if( speedTimer.GetTargetTime() > timing->GetRpmSpeedReactionTime() * timeMult )
			{
				//���������� ����� �����
				speedTimer.SetTargetTime( timing->GetRpmSpeedReactionTime() * timeMult );
			}
		}
		else
		{
			//����� �� ������������
			//���������� ������� �������� RPM
			spFirstRpm = motorState->getRadPS() * DUI::RadPStoRPM;
			//������������� ������
			speedTimer.Reset( timing->GetRpmSpeedReactionTime() * timeMult );
			//����
			isMeasuringSpeed = true;
		}
	}
	//------------------------------------------------------------
	void MotorRPMZoneController::MeasureSpeed( const Core::BaseDelegate* del, float timeMult )
	{
		//������� ��������
		speed.addDelegate( del );
		//������ �����
		MeasureSpeed( timeMult );
	}
	//------------------------------------------------------------
	void MotorRPMZoneController::MeasureSpeedInc( const Core::BaseDelegate* del, float timeMult )
	{
		speedInc.addDelegate( del );
		MeasureSpeed( timeMult );
	}
	//------------------------------------------------------------
	void MotorRPMZoneController::MeasureSpeedDec( const Core::BaseDelegate* del, float timeMult )
	{
		speedDec.addDelegate( del );
		MeasureSpeed( timeMult );
	}
	//------------------------------------------------------------
	void MotorRPMZoneController::OperateController()
	{
		zoneTimer.Operate( FTI::get() );
		speedTimer.Operate( FTI::get() );
	}
	//------------------------------------------------------------
	/*
	�������� ������ ��������� ��������
	*/
	void MotorRPMZoneController::OnSpeedTimer()
	{
		spSecondRpm = motorState->getRadPS() * DUI::RadPStoRPM;
		isMeasuringSpeed = false;
		rpmSpeed = ( spSecondRpm - spFirstRpm ) * ( 1.0 / speedTimer.GetTargetTime() );
		speed.Call();
		speed.removeAllDelegates();
		if( rpmSpeed < 0 )
		{
			speedDec.Call();
			speedDec.removeAllDelegates();
			speedInc.removeAllDelegates();
		}
		else
		{
			speedInc.Call();
			speedInc.removeAllDelegates();
			speedDec.removeAllDelegates();
		}
	}
	//------------------------------------------------------------
	void MotorRPMZoneController::RefreshZone()
	{
		float rpm = motorState->getRadPS() * DUI::RadPStoRPM;
		if( rpm > GetNormHighRpm() )
		{
			if( rpm > GetMaxRpm() )//ABOVE_MAX
			{
				lastZone = Z_ABOVE_MAX;
				aboveMax.Call();
			}
			else//ABOVE_NORM
			{
				lastZone = Z_ABOVE_NORM;
				aboveNorm.Call();
			}
		}
		else if( rpm < GetNormLowRpm() )
		{
			if( rpm < GetMinRpm() )//BELOW_MIN
			{
				lastZone = Z_BELOW_MIN;
				belowMin.Call();
			}
			else//BELOW_NORM
			{
				lastZone = Z_BELOW_NORM;
				belowNorm.Call();
			}
		}
		else//NORM
		{
			lastZone = Z_NORM;
			norm.Call();
		}
	}
	//------------------------------------------------------------
	void MotorRPMZoneController::OnZoneTimer()
	{
		float rpm = motorState->getRadPS() * DUI::RadPStoRPM;
		switch( lastZone )
		{
		case Z_BELOW_MIN:
			if( rpm > GetMinRpm() )
				RefreshZone();
			break;
		case Z_BELOW_NORM:
			if( rpm < GetMinRpm() || rpm > GetNormLowRpm() )
				RefreshZone();
			break;
		case Z_NORM:
			if( rpm < GetNormLowRpm() || rpm > GetNormHighRpm() )
				RefreshZone();
			break;
		case Z_ABOVE_NORM:
			if( rpm < GetNormHighRpm() || rpm > GetMaxRpm() )
				RefreshZone();
			break;
		case Z_ABOVE_MAX:
			if( rpm < GetMaxRpm() )
				RefreshZone();
			break;
		default:
			RefreshZone();
		}
		zoneTimer.Reset( timing->GetRpmZoneReactionTime() );
	}
	//------------------------------------------------------------
	}//namespace Vehicle
}//namespace Demo
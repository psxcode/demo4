#include "DemoStableHeaders.h"
#if DBS_LM_VEHICLE_DRIVER

#include "MechGearboxController.h"
#include "drive_options.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	const int MechGearboxController::gearboxRequestTimesLimit = 5;
	//------------------------------------------------------------
	bool MechGearboxController::Initialize( const MechGearboxControllerInitData& data )
	{
		if( !data.isValid() )return false;

		driveOptions = data.driveOptions;
		timing = data.timing;
		iGControl = data.iGearboxControl;
		iGCallback.downcast_assign( iGControl->GetInterface( GIT_CALLBACK ) );
		iGState.downcast_assign( iGControl->GetInterface( GIT_STATE ) );
		if( iGState == 0 || iGCallback == 0 )return false;

		if( (driveOptions & DO_CLUTCH_ASSIST) != 0 )
		{
			iClutchControl = data.iClutchControl;
			if( iClutchControl == 0 )return false;
			iClutchCallback.downcast_assign( iClutchControl->GetInterface( PIT_PCALLBACK ) );
			if( iClutchCallback == 0 )return false;

			//�������� ������� ��� ���������
			clutchPressedDel.bind( new delegate_traits::del0<void>( this, &MechGearboxController::OnClutchPressed ) );
			clutchReleasedDel.bind( new delegate_traits::del0<void>( this, &MechGearboxController::OnClutchReleased ) );

			//SHIFT_ASSIST ���������� ��� CLUTCH_ASSIST
			if( (driveOptions & DO_SHIFT_ASSIST) != 0 )
			{
				speedToGear = data.speedToGear;
				if( speedToGear == 0 )return false;

				radPSController = data.radPSController;
				if( radPSController == 0 )return false;

				//�������� ��������
				gearRpmBelowNormDel.bind( new delegate_traits::active_del0<void>( this, &MechGearboxController::OnRpmBelowNorm ) );
				gearRpmAboveNormDel.bind( new delegate_traits::active_del0<void>( this, &MechGearboxController::OnRpmAboveNorm ) );
				gearRpmSpeedIncDel.bind( new delegate_traits::active_del0<void>( this, &MechGearboxController::OnRpmSpeedInc ) );
				gearRpmSpeedDecDel.bind( new delegate_traits::active_del0<void>( this, &MechGearboxController::OnRpmSpeedDec ) );

				//������ �� ����
				radPSController->addNormLowBelowOnceDel( gearRpmBelowNormDel );
				radPSController->addNormHighAboveOnceDel( gearRpmAboveNormDel );
			}
		}

		//�������� ��������� ��������
		shiftDelayDel.bind( new delegate_traits::active_del0<void>( this, &MechGearboxController::OnShiftDelayTimer ) );
		shiftOpDel.bind( new delegate_traits::active_del0<void>( this, &MechGearboxController::OnShiftOpTimer ) );
		shiftToNeutralDel.bind( new delegate_traits::active_del0<void>( this, &MechGearboxController::OnShiftToNeutralTimer ) );
		shiftEndedDel.bind( new delegate_traits::del0<void>( this, &MechGearboxController::OnShiftEnded ) );

		shiftDelayDel->active = false;
		shiftDelayTimer.AddOnceDel( shiftDelayDel );
		shiftOpDel->active = false;
		shiftToNeutralDel->active = false;
		shiftOpTimer.AddOnceDel( shiftOpDel );
		shiftOpTimer.AddOnceDel( shiftToNeutralDel );

		setInitializeController();
		return true;//�� �.�.
	}
	//------------------------------------------------------------
	void MechGearboxController::CloseMechGearboxController()
	{
		if( gearRpmBelowNormDel != 0 )
		{
			if( radPSController != 0 )radPSController->removeNormLowBelowOnceDel( gearRpmBelowNormDel );
			gearRpmBelowNormDel = 0;
		}
		if( gearRpmAboveNormDel != 0 )
		{
			if( radPSController != 0 )radPSController->removeNormHighAboveOnceDel( gearRpmAboveNormDel );
			gearRpmAboveNormDel = 0;
		}
		if( gearRpmSpeedIncDel != 0 )
		{
			if( radPSController != 0 )radPSController->removeSpeedDel( gearRpmSpeedIncDel );
			gearRpmSpeedIncDel = 0;
		}
		if( gearRpmSpeedDecDel != 0 )
		{
			if( radPSController != 0 )radPSController->removeSpeedDel( gearRpmSpeedDecDel );
			gearRpmSpeedDecDel = 0;
		}
		if( clutchPressedDel != 0 )
		{
			if( iClutchCallback != 0 )iClutchCallback->RemovePressedTempDel( clutchPressedDel );
			clutchPressedDel = 0;
		}
		if( clutchReleasedDel != 0 )
		{
			if( iClutchCallback != 0 )iClutchCallback->RemoveReleasedTempCall( clutchReleasedDel );
			clutchReleasedDel = 0;
		}
		shiftDelayTimer.RemoveAllTempDels();
		shiftDelayTimer.RemoveAllContDels();
		shiftDelayTimer.RemoveAllOnceDels();
		shiftDelayDel = 0;
		shiftOpTimer.RemoveAllContDels();
		shiftOpTimer.RemoveAllOnceDels();
		shiftOpTimer.RemoveAllTempDels();
		shiftOpDel = 0;

		if( !shiftEndedDel.isNull() )
		{
			iGCallback->RemoveShiftCompletedOnceDel( shiftEndedDel );
			iGCallback->RemoveShiftFailedOnceDel( shiftEndedDel );
			shiftEndedDel = 0;
		}
		
		iGCallback.setNull();
		iGControl.setNull();
		iGState.setNull();
		iClutchControl.setNull();
		iClutchCallback.setNull();
		lastGear = targetGear = 0;
		isShiftInitiated = isShiftLastStage = false;

		GearboxController::CloseGearboxController();
	}
	//------------------------------------------------------------
	/*
	������ � ������ �����
	*/
	void MechGearboxController::OperateController()
	{
		//�������� �������,
		shiftDelayTimer.Operate();
		shiftOpTimer.Operate();
		Core::Controller::OperateController();
	}
	//------------------------------------------------------------
	/*
	���������� ����� VehicleDriver ��� user ������������ �������
	*/
	bool MechGearboxController::ShiftUp()
	{
		if( isShiftLastStage )
		{
			//������������ ������� � ��������� ������.
			//���������� true, �� ������ �� ������.
			//VehicleDriver ������ ��� ������� �������.
			return true;
		}
		
		//������������ �� � ��������� ������.
		//���� ������ �����
		//���� ��������� ������� � �������� ����������� �������

		lastGear = iGState->GetActiveGearIndex();//������ ������� ��������.
		if( !isShiftInitiated )targetGear = lastGear;
		if( !iGState->HasGear( targetGear + 1 ) )
		{
			//HasGear ������ false. �� ���� ������������� �������� �� ���.
			//VehicleDriver ����� ��� ������ ����������� ������.
			return false;
		}

		//HasGear ������ true
		//�������� ������������
		++targetGear;//++ �.�. ��� ShiftUp()
		shiftDelayTimer.Reset( timing->GetShiftDelayTime() );//������ ������� ���������� �������
		shiftDelayDel->active = true;//������� ������������ �������

		if( !isShiftInitiated )//���� ������������ ��� ������ �� ��������� ������
		{
			//������������ �� ������
			//�� ���� ��� ������ ����� ShiftUp()
			InitiateShift();//������
		}

		return true;
	}
	//------------------------------------------------------------
	/*
	��. ShiftUp()
	*/
	bool MechGearboxController::ShiftDown()
	{
		if( isShiftLastStage )
			return true;

		lastGear = iGState->GetActiveGearIndex();
		if( !isShiftInitiated )targetGear = lastGear;
		if( !iGState->HasGear( targetGear - 1 ) )
			return false;

		--targetGear;
		shiftDelayTimer.Reset( timing->GetShiftDelayTime() );
		shiftDelayDel->active = true;

		if( !isShiftInitiated )
			InitiateShift();

		return true;
	}
	//------------------------------------------------------------
	/*
	���������� ����� VehicleDriver ��� ������ SHIFT_ASSIST
	*/
	bool MechGearboxController::ShiftToDirect()
	{
		if( isShiftInitiated )return false;
		lastGear = iGState->GetActiveGearIndex();
		targetGear = GetDirectGearBySpeed();
		if( targetGear == lastGear )
			return true;

		ActivateRpmDels( false );
		shiftDelayTimer.Reset( timing->GetShiftDelayTime() );
		shiftDelayDel->active = true;//������� ������������ �������
		InitiateShift();
		return true;
	}
	//------------------------------------------------------------
	/*
	���������� ����� VehicleDriver ��� ������ SHIFT_ASSIST
	*/
	bool MechGearboxController::ShiftToNeutral()
	{
		if( isShiftInitiated )return false;
		lastGear = iGState->GetActiveGearIndex();
		targetGear = 0;
		if( targetGear == lastGear )
			return true;

		ActivateRpmDels( false );
		InitiateShift();
		return true;
	}
	//------------------------------------------------------------
	/*
	���������� ����� VehicleDriver ��� ������ SHIFT_ASSIST
	*/
	bool MechGearboxController::ShiftToReverse()
	{
		if( isShiftInitiated )return false;
		lastGear = iGState->GetActiveGearIndex();
		targetGear = GetReverseGearBySpeed();
		if( targetGear == lastGear )
			return true;

		ActivateRpmDels( false );
		shiftDelayTimer.Reset( timing->GetShiftDelayTime() );
		shiftDelayDel->active = true;//������� ������������ �������
		InitiateShift();
		return true;
	}
	//------------------------------------------------------------
	/*
	�������� ������������.
	�.�. ��� �������� ��� ����� �������.
	������� ��� ����������� �����������.
	*/
	void MechGearboxController::InitiateShift()
	{
		isShiftInitiated = true;//������
		shiftStartedDelArray();//������ � ������, VehicleDriver ��������� ���� ��� clutch

		//���� ������ � ���������
		if( (driveOptions & DO_CLUTCH_ASSIST) != 0 )
		{
			iClutchControl->Press();//���
			iClutchCallback->AddPressedTempDel( clutchPressedDel );//���
		}
		else
		{
			//�������. ������� ��� ��������� ������.
			OnClutchPressed();
		}
	}
	//------------------------------------------------------------
	/*
	������ ��������.
	����������� ������������ ������ ������.
	�������� ��������� ������
	*/
	void MechGearboxController::OnShiftDelayTimer()
	{
		//��� ������ �������� ����� �� ��������
		if( !shiftOpTimer.IsWorking() && iGState->GetActiveGearIndex() == 0 && iGState->GetTargetGearIndex() == 0 )
		{
			//�����!!
			isShiftLastStage = true;//��������� ������

			//�������� - � ���� ����� �� �������������
			if( targetGear > 0 )//������
				shiftOpTimer.Reset( timing->GetShiftToDirectTime() );//����� ���� ���� ������� �����
			else if( targetGear < 0 )//�����
				shiftOpTimer.Reset( timing->GetShiftToReverseTime() );//����� ���� ���� ������� �����
			else//��������
				shiftOpTimer.Reset( timing->GetShiftToNeutralTime() );//����� ���� ���� ������� �����

			shiftOpDel->active = true;//� ����� ��� ���� ��������
			shiftDelayDel->active = true;
			return;
		}
		else
		{
			++gearboxRequestTimes;
			if( gearboxRequestTimes >= gearboxRequestTimesLimit ){ 
				CancelShift(); return;
			}

			//���! ��� ��� ��������. �������� ���������.
			shiftDelayTimer.Reset(0.1f);//���. ����� �������.
			//� ������ - �����!! ����� ����-��.
			return;
		}
	}
	//------------------------------------------------------------
	/*
	������ ���������
	*/
	void MechGearboxController::OnClutchPressed()
	{
		if( !isShiftInitiated )return;//� �� ����� � ��� �����?? ����� �...

		//��� ��������� ������. ������ �� �������� �����.
		shiftOpTimer.Reset( timing->GetShiftToNeutralTime() );//������ ������� ����
		shiftToNeutralDel->active = true;//��� ������
	}
	//------------------------------------------------------------
	/*
	��������� ���� ��������
	*/
	void MechGearboxController::OnClutchReleased()
	{
		if( (driveOptions & DO_SHIFT_ASSIST) != 0 )
			ActivateRpmDels();
	}
	//------------------------------------------------------------
	/*
	���..
	������ ������� ��������.
	*/
	void MechGearboxController::OnShiftToNeutralTimer()
	{
		//���� ������������.
		//������ �������.
		if( !iGControl->ShiftToNeutral() )
		{
			++gearboxRequestTimes;
			if( gearboxRequestTimes >= gearboxRequestTimesLimit ){
				CancelShift(); return;
			}

			//�� ����� ������������.
			//��� ��������� �� ������, ��� ��� ���� ������������ �� ��������
			//������� ��� ��� ���������
			shiftOpTimer.Reset( 0.1f );//��������
			//���.. ����� ����-��
			return;
		}
		shiftToNeutralDel->active = false;
		//�� ��. ������� ����� ������� ��������.
		//����� ���������.......
	}
	//------------------------------------------------------------
	/*
	������ ��� ShiftOperation ��������.
	׸ ������??
	*/
	void MechGearboxController::OnShiftOpTimer()
	{
		//������� �� ������� �������� ��������
		if( !iGControl->ShiftTo( targetGear ) )
		{
			++gearboxRequestTimes;
			if( gearboxRequestTimes >= gearboxRequestTimesLimit ){
				CancelShift(); return; 
			}

			//���! ������?? ��������
			shiftOpTimer.Reset( 0.1f );//��������
			shiftOpTimer.AddTempDel( shiftOpDel );//���
			return;
		}
		shiftOpDel->active = false;
		//������� ����� �������� ��������
		iGCallback->AddShiftCompletedTempDel( shiftEndedDel );
		iGCallback->AddShiftFailedTempDel( shiftEndedDel );
	}
	//------------------------------------------------------------
	/*
	��������� �������� ���������( ����� � �������� )
	�� ���� �� �����.
	������ ���� ��������� �������� ������������
	*/
	void MechGearboxController::OnShiftEnded()
	{
		lastGear = 0;
		isShiftLastStage = false;//���������
		isShiftInitiated = false;//���������
		shiftEndedDelArray();//���������

		if( (driveOptions & DO_CLUTCH_ASSIST) != 0 )
		{
			iClutchCallback->AddReleasedTempDel( clutchReleasedDel );
			if( (driveOptions & DO_SHIFT_ASSIST) != 0 )
				ActivateRpmDels( true );
		}
	}
	//------------------------------------------------------------
	void MechGearboxController::CancelShift()
	{
		targetGear = lastGear;
		shiftDelayTimer.Stop();
		shiftOpTimer.Stop();
		gearboxRequestTimes = 0;
		OnShiftEnded();
	}
	//------------------------------------------------------------
	/*
	���\���� ��� ��������� ������� �� ����
	*/
	void MechGearboxController::ActivateRpmDels( bool activate )
	{
		gearRpmBelowNormDel->active = activate;
		gearRpmAboveNormDel->active = activate;
	}
	//------------------------------------------------------------
	/*
	� ���� �������!
	������� ��������� ���� �����
	*/
	void MechGearboxController::OnRpmBelowNorm()
	{
		ActivateRpmDels( false );//�������� ��������

		//��� ������!!
		//������� �������� �������� ��������� ��������( ����� ������ )
		//���������� ��������� ����.
		radPSController->measureSpeedDecrease( 1.0f, gearRpmSpeedDecDel );
		gearRpmSpeedDecDel->active = true;
	}
	//------------------------------------------------------------
	/*
	�������!!!
	������� ��������� ���� �����
	*/
	void MechGearboxController::OnRpmAboveNorm()
	{
		ActivateRpmDels( false );//��������
		radPSController->measureSpeedIncrease( 1.0f, gearRpmSpeedIncDel );//����������
		gearRpmSpeedIncDel->active = true;
	}
	//------------------------------------------------------------
	/*
	������� �����������
	����� ����������� - ����������� ����
	*/
	void MechGearboxController::OnRpmSpeedDec()
	{
		//��������� �������
		gearRpmSpeedDecDel->active = false;

		if( isShiftInitiated )//������������ ��� ���.. ��� ��� ������ ������
			return;

		//�������� ������������
		lastGear = iGState->GetActiveGearIndex();//����� ��� ��������
		
		if( lastGear > 0 )
		{
			//�������� Direct ��������
			targetGear = lastGear - 1;//��� ����
			if( targetGear > 0 && iGState->HasGear( targetGear ) )
			{
				//�������� Direct ���� ������� � ������ � ������
				shiftDelayTimer.Reset( timing->GetShiftDelayTime() );//�������� ����
				shiftDelayTimer.AddTempDel( shiftDelayDel );//����� �����
				InitiateShift();//� ���� ���� ������ Clutch � �� �������� ��������
				return;//�� �����
			}
			//else
			//�� �������� ��������� ������
			//���� ����� �������� ��� �� ���� �����
		}
		else if( lastGear < 0 )
		{
			//���. ��� �������� Reverse
			targetGear = lastGear + 1;//�������� Reverse
			if( targetGear < 0 && iGState->HasGear( targetGear ) )
			{
				//���� ����� ��������. ������ 0_+
				shiftDelayTimer.Reset( timing->GetShiftDelayTime() );//��������
				shiftDelayTimer.AddTempDel( shiftDelayDel );//�����
				InitiateShift();//����� ��� ���
				return;//����� ��������
			}
			//else
			//�� �������� ��������� ������
		}
		//������ ������� ���
		//�������� ����. ����� �������� �� ������
		ActivateRpmDels();
	}
	//------------------------------------------------------------
	/*
	������� �������������
	���� ����������� �����
	*/
	void MechGearboxController::OnRpmSpeedInc()
	{
		//��������� �������
		gearRpmSpeedIncDel->active = false;

		if( isShiftInitiated )//������������ ��� ���
			return;

		//��������
		lastGear = iGState->GetActiveGearIndex();

		if( lastGear > 0 )
		{
			//�������� Direct ��������
			targetGear = lastGear + 1;
			if( iGState->HasGear( targetGear ) )
			{
				//���� ����� ��������.. ��������
				shiftDelayTimer.Reset( timing->GetShiftDelayTime() );
				shiftDelayTimer.AddTempDel( shiftDelayDel );
				InitiateShift();
				return;
			}
			//��� �����..
		}
		else if( lastGear < 0 )
		{
			//�������� Reverse
			targetGear = lastGear - 1;
			if( iGState->HasGear( targetGear ) )
			{
				//���� ��������. �������..
				shiftDelayTimer.Reset( timing->GetShiftDelayTime() );
				shiftDelayTimer.AddTempDel( shiftDelayDel );
				InitiateShift();
				return;
			}
			//����� ���
			//������� �� �������
		}
		//�������� ����.
		ActivateRpmDels();
	}
	//------------------------------------------------------------
	/*
	����� Direct �������� ������������� ������ ��������
	*/
	int MechGearboxController::GetDirectGearBySpeed()
	{
		//�������� ���������� Direct �������
		int directCount = iGState->GetDirectGearsCount();
		//�������� NormHigh ������� ��� ������� driveStyle
		float normHighRadPS = radPSController->getNormHighRadPS();

		//��������� �� ���� ������������ ���������
		for( int i = 1; i <= directCount; i++ )
		{
			float engRpm = speedToGear->GetMotorRadPSAtGear( i );
			if( engRpm < normHighRadPS )
				return i;
		}

		//������ �� �������.
		//�������� ����� �������.
		//�������� ��������� ��������
		return directCount;
	}
	//------------------------------------------------------------
	/*
	���������� Reverse �������� �� ������� ��������
	�������� ��. � GetDirectGearsBySpeed()
	*/
	int MechGearboxController::GetReverseGearBySpeed()
	{
		int reverseCount = iGState->GetReverseGearsCount();
		float normHighRadPS = radPSController->getNormHighRadPS();
		for( int i = 1; i < reverseCount; i++ )
		{
			float engRpm = speedToGear->GetMotorRadPSAtGear( -i );
			if( engRpm < normHighRadPS )
				return -i;
		}
		return -reverseCount;
	}
	//------------------------------------------------------------
	bool MechGearboxController::IsDirect() const
	{
		return iGState->GetActiveGearIndex() > 0;
	}
	//------------------------------------------------------------
	bool MechGearboxController::IsNeutral() const
	{
		return iGState->GetActiveGearIndex() == 0;
	}
	//------------------------------------------------------------
	bool MechGearboxController::IsReverse() const
	{
		return iGState->GetActiveGearIndex() < 0;
	}
	//------------------------------------------------------------
	IGearboxController::shared_ptr MechGearboxController::GetInterface( GEARBOX_INTERFACE_TYPE type )
	{
		switch( type )
		{
		case GIT_STATE: return GetStateInterface();
		default: return IGearboxController::shared_ptr();
		};
	}
	//------------------------------------------------------------
	IMechGearboxControllerState::shared_ptr MechGearboxController::GetStateInterface()
	{
		if( iState == 0 )iState.bind( new IMechGearboxControllerState( *this ) );
		return iState;
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
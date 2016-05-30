#include "DemoStableHeaders.h"
#if DBS_LM_VEHICLE_DRIVER

#include "AutoGearboxController.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	bool AutoGearboxController::Initialize( const AutoGearboxControllerInitData& data )
	{
		if( !data.isValid() )return false;

		driveOptions = data.driveOptions;
		timing = data.timing;
		iControl = data.iGearboxControl;

		iCallback.downcast_assign( iControl->GetInterface( GIT_CALLBACK ) );
		if( iCallback == 0 )return false;

		iState.downcast_assign( iControl->GetInterface( GIT_STATE ) );
		if( iState == 0 )return false;

		switchModeUpDel.bind( new delegate_traits::del< void() >( this, &AutoGearboxController::OnSwitchModeUp ) );
		switchModeDownDel.bind( new delegate_traits::del< void() >( this, &AutoGearboxController::OnSwitchModeDown ) );

		switchModeEndedDel.bind( new delegate_traits::del< void() >( this, &AutoGearboxController::SwitchModeEnded ) );
		iCallback->AddSwitchCompletedOnceDel( switchModeEndedDel );
		iCallback->AddSwitchFailedOnceDel( switchModeEndedDel );

		setInitializeController();
		return true;
	}
	//------------------------------------------------------------
	void AutoGearboxController::CloseAutoGearboxController()
	{
		switchOpTimer.RemoveAllOnceDels();
		switchOpTimer.RemoveAllContDels();
		switchOpTimer.RemoveAllTempDels();
		switchModeUpDel = 0;
		switchModeDownDel = 0;
		switchModeOpDel = 0;

		if( switchModeEndedDel != 0 )
		{
			if( iCallback != 0 )
			{
				iCallback->RemoveSwitchCompletedOnceDel( switchModeEndedDel );
				iCallback->RemoveSwitchFailedOnceDel( switchModeEndedDel );
			}
			switchModeEndedDel = 0;
		}

		iControl = 0;
		iCallback = 0;
		iState = 0;
		GearboxController::CloseGearboxController();
	}
	//------------------------------------------------------------
	/*
	������������ ������� �� �������� ������
	��������������� ������� ������ ������ ShiftUp
	*/
	bool AutoGearboxController::ShiftUp()
	{
		//���� ��� ������������ - �������
		if( isSwitchInitiated )return false;

		if( iControl->PrepareModeUp() )
		{
			//������� ����� �������� ���� �����
			if( iState->GetActiveMode() == AGM_SEQ_EMU )
			{
				//��� �������� ������������� �����. ����� ����� ��� �������������� �����
				switchOpTimer.Reset( timing->GetSwitchSeqTime() );
			}
			else
			{
				//��� ������� �����. ����� ����� ��� �������� ������
				switchOpTimer.Reset( timing->GetSwitchModeTime() );
			}
			//��������� ������������ �������
			switchOpTimer.AddTempDel( switchModeUpDel );

			//����
			isSwitchInitiated = true;

			//������ � ������ ������
			shiftStartedDelArray();
		}
		else
		{
			//������� �� ����� ��� �������� ���� �����
			return false;
		}

		return true;
	}
	//------------------------------------------------------------
	/*
	������������ ������� �� �������� ������
	���� �������� ShiftDown
	�������� ��. � ShiftUp()
	*/
	bool AutoGearboxController::ShiftDown()
	{
		if( isSwitchInitiated )return false;

		if( iControl->PrepareModeDown() )
		{
			if( iState->GetActiveMode() == AGM_SEQ_EMU )
			{
				switchOpTimer.Reset( timing->GetSwitchSeqTime() );
			}
			else
			{
				switchOpTimer.Reset( timing->GetSwitchModeTime() );
			}
			switchOpTimer.AddTempDel( switchModeDownDel );
			isSwitchInitiated = true;
			shiftStartedDelArray();
		}
		else return false;

		return true;
	}
	//------------------------------------------------------------
	/*
	������������ ������� �� �������� ������
	�������� ��� GEAR_ASSIST
	*/
	bool AutoGearboxController::ShiftToDirect()
	{
		//���� ��� ������������ - �������
		if( isSwitchInitiated )return false;

		//����� �� ������� ���������� ������ �����
		if( iControl->PrepareMode( AGM_DIRECT ) )
		{
			//�������� ������
			switchOpTimer.Reset( timing->GetSwitchModeTime() );
			//��������� �������
			switchOpTimer.AddTempDel( switchModeOpDel );
			//��������� �������� �����
			targetMode = AGM_DIRECT;
			//����
			isSwitchInitiated = true;
			//������ � ������ �����
			shiftStartedDelArray();
			return true;
		}
		return false;//������ ����� ��� ����������
	}
	//------------------------------------------------------------
	/*
	������������ ������� �� �������� ������
	�������� ��� GEAR_ASSIST
	�������� � ShiftToDirect
	*/
	bool AutoGearboxController::ShiftToNeutral()
	{
		if( isSwitchInitiated )return false;

		if( iControl->PrepareMode( AGM_NEUTRAL ) )
		{
			switchOpTimer.Reset( timing->GetSwitchModeTime() );
			switchOpTimer.AddTempDel( switchModeOpDel );
			targetMode = AGM_NEUTRAL;
			isSwitchInitiated = true;
			shiftStartedDelArray();
			return true;
		}
		return false;
	}
	//------------------------------------------------------------
	/*
	������������ ������� �� �������� ������
	�������� ��� GEAR_ASSIST
	�������� � ShiftToDirect
	*/
	bool AutoGearboxController::ShiftToReverse()
	{
		if( isSwitchInitiated )return false;

		if( iControl->PrepareMode( AGM_REVERSE ) )
		{
			switchOpTimer.Reset( timing->GetSwitchModeTime() );
			switchOpTimer.AddTempDel( switchModeOpDel );
			targetMode = AGM_REVERSE;
			isSwitchInitiated = true;
			shiftStartedDelArray();
			return true;
		}
		return false;
	}
	//------------------------------------------------------------
	/*
	��� ������� ������ ��������� � ������ �����
	*/
	void AutoGearboxController::OperateController()
	{
		switchOpTimer.Operate();
		Core::Controller::OperateController();
	}
	//------------------------------------------------------------
	/*
	������� - ������� �� ������
	���������� � ������ ��� ShiftUp
	*/
	void AutoGearboxController::OnSwitchModeUp()
	{
		//���� ���� �� ������
		if( !isSwitchInitiated )return;

		if( !iControl->ModeUp() )
		{
			//������� ������������ ���������
			if( switchOpTimer.IsWorking() )
				switchOpTimer.Reset( 0.1f );
			switchOpTimer.AddTempDel( switchModeUpDel );
		}
	}
	//------------------------------------------------------------
	/*
	������� - ������� �� ������
	���������� � ������ ��� ShiftDown
	*/
	void AutoGearboxController::OnSwitchModeDown()
	{
		if( !isSwitchInitiated )return;
		if( !iControl->ModeDown() )
		{
			if( switchOpTimer.IsWorking() )
				switchOpTimer.Reset( 0.5f );
			switchOpTimer.AddTempDel( switchModeDownDel );
		}
	}
	//------------------------------------------------------------
	/*
	������� - ������� �� ������
	���������� � ������ ��� ShiftToDirect, ShiftToNeutral, ShiftToReverse
	*/
	void AutoGearboxController::OnSwitchOp()
	{
		if( !isSwitchInitiated )return;
		if( !iControl->SwitchMode( targetMode ) )
		{
			if( switchOpTimer.IsWorking() )
				switchOpTimer.Reset( 0.5f );
			switchOpTimer.AddTempDel( switchModeOpDel );
		}
	}
	//------------------------------------------------------------
	/*
	Callback �� �������
	�������� � ���������� ����� � ����� �������
	*/
	void AutoGearboxController::SwitchModeEnded()
	{
		//����� targetMode
		targetMode = AGM_NOT_DEFINED;
		//������� ����
		isSwitchInitiated = false;
		//������ � ����� �����
		shiftEndedDelArray();
	}
	//------------------------------------------------------------
	bool AutoGearboxController::IsDirect() const
	{
		return iState->GetActiveGearIndex() > 0;
	}
	//------------------------------------------------------------
	bool AutoGearboxController::IsNeutral() const
	{
		return iState->GetActiveGearIndex() == 0;
	}
	//------------------------------------------------------------
	bool AutoGearboxController::IsReverse() const
	{
		return iState->GetActiveGearIndex() < 0;
	}
	//------------------------------------------------------------
	IGearboxController::shared_ptr AutoGearboxController::GetInterface( GEARBOX_INTERFACE_TYPE type )
	{
		return IGearboxController::shared_ptr();
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_ABSTRACT_VEHICLE
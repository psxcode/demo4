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
	Интерфейсная функция из базового класса
	Подразумевается нажатие юзером кнопки ShiftUp
	*/
	bool AutoGearboxController::ShiftUp()
	{
		//Если идёт переключение - выходим
		if( isSwitchInitiated )return false;

		if( iControl->PrepareModeUp() )
		{
			//Коробка может включить этот режим
			if( iState->GetActiveMode() == AGM_SEQ_EMU )
			{
				//Щас работает Секвентальный режим. Берем время для секвентального шифта
				switchOpTimer.Reset( timing->GetSwitchSeqTime() );
			}
			else
			{
				//Щас обычный режим. Берем время для обычного режима
				switchOpTimer.Reset( timing->GetSwitchModeTime() );
			}
			//Добавляем срабатывание таймера
			switchOpTimer.AddTempDel( switchModeUpDel );

			//Флаг
			isSwitchInitiated = true;

			//Кричим о начале работы
			shiftStartedDelArray();
		}
		else
		{
			//Коробка не может щас включить этот режим
			return false;
		}

		return true;
	}
	//------------------------------------------------------------
	/*
	Интерфейсная функция из базового класса
	Юзер нажимает ShiftDown
	Комменты см. в ShiftUp()
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
	Интерфейсная функция из базового класса
	Работает при GEAR_ASSIST
	*/
	bool AutoGearboxController::ShiftToDirect()
	{
		//Если идёт переключение - выходим
		if( isSwitchInitiated )return false;

		//Может ли коробка обеспечить данный режим
		if( iControl->PrepareMode( AGM_DIRECT ) )
		{
			//Заряжаем таймер
			switchOpTimer.Reset( timing->GetSwitchModeTime() );
			//Добавляем реакцию
			switchOpTimer.AddTempDel( switchModeOpDel );
			//Сохраняем желаемый режим
			targetMode = AGM_DIRECT;
			//Флаг
			isSwitchInitiated = true;
			//Кричим о начале работ
			shiftStartedDelArray();
			return true;
		}
		return false;//Данный режим щас недоступен
	}
	//------------------------------------------------------------
	/*
	Интерфейсная функция из базового класса
	Работает при GEAR_ASSIST
	Комменты в ShiftToDirect
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
	Интерфейсная функция из базового класса
	Работает при GEAR_ASSIST
	Комменты в ShiftToDirect
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
	Эта функция должна вызыватся в каждом кадре
	*/
	void AutoGearboxController::OperateController()
	{
		switchOpTimer.Operate();
		Core::Controller::OperateController();
	}
	//------------------------------------------------------------
	/*
	Функция - реакция на таймер
	Заряжается в таймер при ShiftUp
	*/
	void AutoGearboxController::OnSwitchModeUp()
	{
		//Если Флаг не поднят
		if( !isSwitchInitiated )return;

		if( !iControl->ModeUp() )
		{
			//Коробка отказывается включатся
			if( switchOpTimer.IsWorking() )
				switchOpTimer.Reset( 0.1f );
			switchOpTimer.AddTempDel( switchModeUpDel );
		}
	}
	//------------------------------------------------------------
	/*
	Функция - реакция на таймер
	Заряжается в таймер при ShiftDown
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
	Функция - реакция на таймер
	Заряжается в таймер при ShiftToDirect, ShiftToNeutral, ShiftToReverse
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
	Callback из коробки
	Сообщает о завершении работ в самой коробке
	*/
	void AutoGearboxController::SwitchModeEnded()
	{
		//Сброс targetMode
		targetMode = AGM_NOT_DEFINED;
		//Снимаем флаг
		isSwitchInitiated = false;
		//Кричим о конце работ
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
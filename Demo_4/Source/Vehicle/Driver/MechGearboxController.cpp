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

			//Заряжаем делегат для сцепления
			clutchPressedDel.bind( new delegate_traits::del0<void>( this, &MechGearboxController::OnClutchPressed ) );
			clutchReleasedDel.bind( new delegate_traits::del0<void>( this, &MechGearboxController::OnClutchReleased ) );

			//SHIFT_ASSIST невозможен без CLUTCH_ASSIST
			if( (driveOptions & DO_SHIFT_ASSIST) != 0 )
			{
				speedToGear = data.speedToGear;
				if( speedToGear == 0 )return false;

				radPSController = data.radPSController;
				if( radPSController == 0 )return false;

				//Заряжаем зонщиков
				gearRpmBelowNormDel.bind( new delegate_traits::active_del0<void>( this, &MechGearboxController::OnRpmBelowNorm ) );
				gearRpmAboveNormDel.bind( new delegate_traits::active_del0<void>( this, &MechGearboxController::OnRpmAboveNorm ) );
				gearRpmSpeedIncDel.bind( new delegate_traits::active_del0<void>( this, &MechGearboxController::OnRpmSpeedInc ) );
				gearRpmSpeedDecDel.bind( new delegate_traits::active_del0<void>( this, &MechGearboxController::OnRpmSpeedDec ) );

				//Сажаем на зону
				radPSController->addNormLowBelowOnceDel( gearRpmBelowNormDel );
				radPSController->addNormHighAboveOnceDel( gearRpmAboveNormDel );
			}
		}

		//Заряжаем остальных деятелей
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
		return true;//Всё О.К.
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
	Работа в каждом кадре
	*/
	void MechGearboxController::OperateController()
	{
		//Работают таймеры,
		shiftDelayTimer.Operate();
		shiftOpTimer.Operate();
		Core::Controller::OperateController();
	}
	//------------------------------------------------------------
	/*
	Вызывается через VehicleDriver при user переключении передач
	*/
	bool MechGearboxController::ShiftUp()
	{
		if( isShiftLastStage )
		{
			//Переключение перешло в финальную стадию.
			//Возвращаем true, но ничего не делаем.
			//VehicleDriver думает что команда принята.
			return true;
		}
		
		//Переключение не в финальной стадии.
		//либо первый вызов
		//либо повторные нажатия в пределах допустимого времени

		lastGear = iGState->GetActiveGearIndex();//Взятие текущей передачи.
		if( !isShiftInitiated )targetGear = lastGear;
		if( !iGState->HasGear( targetGear + 1 ) )
		{
			//HasGear вернул false. То есть запрашиваемая передача не сущ.
			//VehicleDriver видит что дальше переключать нельзя.
			return false;
		}

		//HasGear вернул true
		//начинаем переключение
		++targetGear;//++ т.к. это ShiftUp()
		shiftDelayTimer.Reset( timing->GetShiftDelayTime() );//Таймер допуска повторного нажатия
		shiftDelayDel->active = true;//Слушаем срабатывание таймера

		if( !isShiftInitiated )//Если переключение уже начато не запускать заново
		{
			//Переключение не начато
			//То есть это первый вызов ShiftUp()
			InitiateShift();//Запуск
		}

		return true;
	}
	//------------------------------------------------------------
	/*
	см. ShiftUp()
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
	Вызывается через VehicleDriver при режиме SHIFT_ASSIST
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
		shiftDelayDel->active = true;//Слушаем срабатывание таймера
		InitiateShift();
		return true;
	}
	//------------------------------------------------------------
	/*
	Вызывается через VehicleDriver при режиме SHIFT_ASSIST
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
	Вызывается через VehicleDriver при режиме SHIFT_ASSIST
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
		shiftDelayDel->active = true;//Слушаем срабатывание таймера
		InitiateShift();
		return true;
	}
	//------------------------------------------------------------
	/*
	Начинаем переключение.
	Т.к. для механики это целый процесс.
	Функция для внутреннего пользования.
	*/
	void MechGearboxController::InitiateShift()
	{
		isShiftInitiated = true;//Начали
		shiftStartedDelArray();//Кричим о начале, VehicleDriver блокирует зоны для clutch

		//Если помощь в нажимании
		if( (driveOptions & DO_CLUTCH_ASSIST) != 0 )
		{
			iClutchControl->Press();//Жмём
			iClutchCallback->AddPressedTempDel( clutchPressedDel );//Ждём
		}
		else
		{
			//Пропуск. Считаем что сцепление нажато.
			OnClutchPressed();
		}
	}
	//------------------------------------------------------------
	/*
	Таймер сработал.
	Откладывать переключение больше нельзя.
	Начинаем последнюю стадию
	*/
	void MechGearboxController::OnShiftDelayTimer()
	{
		//Для начала проверим стоит ли нейтраль
		if( !shiftOpTimer.IsWorking() && iGState->GetActiveGearIndex() == 0 && iGState->GetTargetGearIndex() == 0 )
		{
			//Стоит!!
			isShiftLastStage = true;//Финальная стадия

			//проверка - а куда вааще мы переключаемся
			if( targetGear > 0 )//вперед
				shiftOpTimer.Reset( timing->GetShiftToDirectTime() );//время пока рука двигает рычаг
			else if( targetGear < 0 )//назад
				shiftOpTimer.Reset( timing->GetShiftToReverseTime() );//время пока рука двигает рычаг
			else//нейтраль
				shiftOpTimer.Reset( timing->GetShiftToNeutralTime() );//время пока рука двигает рычаг

			shiftOpDel->active = true;//И сидим ждём пока подвинет
			shiftDelayDel->active = true;
			return;
		}
		else
		{
			++gearboxRequestTimes;
			if( gearboxRequestTimes >= gearboxRequestTimesLimit ){ 
				CancelShift(); return;
			}

			//Фак! еще нет нейтрали. Придется подождать.
			shiftDelayTimer.Reset(0.1f);//Нет. Тогда зарядим.
			//И теперь - ЖДАТЬ!! Вызов сюда-же.
			return;
		}
	}
	//------------------------------------------------------------
	/*
	нажато сцепление
	*/
	void MechGearboxController::OnClutchPressed()
	{
		if( !isShiftInitiated )return;//А чё вааще я тут делаю?? Пойду я...

		//Так сцепление нажали. Теперь на нейтраль пойдём.
		shiftOpTimer.Reset( timing->GetShiftToNeutralTime() );//Таймер реакции руки
		shiftToNeutralDel->active = true;//Ждём таймер
	}
	//------------------------------------------------------------
	/*
	Сцепление было отпущено
	*/
	void MechGearboxController::OnClutchReleased()
	{
		if( (driveOptions & DO_SHIFT_ASSIST) != 0 )
			ActivateRpmDels();
	}
	//------------------------------------------------------------
	/*
	Опа..
	Таймер наконец сработал.
	*/
	void MechGearboxController::OnShiftToNeutralTimer()
	{
		//Рука среагировала.
		//Теперь коробка.
		if( !iGControl->ShiftToNeutral() )
		{
			++gearboxRequestTimes;
			if( gearboxRequestTimes >= gearboxRequestTimesLimit ){
				CancelShift(); return;
			}

			//Не хочет переключатся.
			//Или сцепление не нажато, или уже идет переключение на механике
			//Подождём еще раз попробуем
			shiftOpTimer.Reset( 0.1f );//Заряжаем
			//Ждём.. вызов сюда-же
			return;
		}
		shiftToNeutralDel->active = false;
		//Ну всё. коробка пошла ставить нейтраль.
		//Можно отдохнуть.......
	}
	//------------------------------------------------------------
	/*
	Таймер для ShiftOperation сработал.
	Чё теперь??
	*/
	void MechGearboxController::OnShiftOpTimer()
	{
		//Требуем от коробки включить передачу
		if( !iGControl->ShiftTo( targetGear ) )
		{
			++gearboxRequestTimes;
			if( gearboxRequestTimes >= gearboxRequestTimesLimit ){
				CancelShift(); return; 
			}

			//Фак! Почему?? Подождем
			shiftOpTimer.Reset( 0.1f );//Заряжаем
			shiftOpTimer.AddTempDel( shiftOpDel );//Ждём
			return;
		}
		shiftOpDel->active = false;
		//Коробка пошла включать передачу
		iGCallback->AddShiftCompletedTempDel( shiftEndedDel );
		iGCallback->AddShiftFailedTempDel( shiftEndedDel );
	}
	//------------------------------------------------------------
	/*
	Включение передачи завершено( может и неудачно )
	Но руке всё равно.
	Теперь надо завершить операцию переключения
	*/
	void MechGearboxController::OnShiftEnded()
	{
		lastGear = 0;
		isShiftLastStage = false;//закончили
		isShiftInitiated = false;//закончили
		shiftEndedDelArray();//закончили

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
	Вкл\Выкл для делегатов сидящих на зоне
	*/
	void MechGearboxController::ActivateRpmDels( bool activate )
	{
		gearRpmBelowNormDel->active = activate;
		gearRpmAboveNormDel->active = activate;
	}
	//------------------------------------------------------------
	/*
	С зоны новости!
	Обороты двигателя НИЖЕ нормы
	*/
	void MechGearboxController::OnRpmBelowNorm()
	{
		ActivateRpmDels( false );//вырубить зонщиков

		//Без паники!!
		//Вначале проверим скорость изменения оборотов( может растут )
		//Заказываем измерение вниз.
		radPSController->measureSpeedDecrease( 1.0f, gearRpmSpeedDecDel );
		gearRpmSpeedDecDel->active = true;
	}
	//------------------------------------------------------------
	/*
	Новости!!!
	Обороты двигателя ВЫШЕ нормы
	*/
	void MechGearboxController::OnRpmAboveNorm()
	{
		ActivateRpmDels( false );//вырубаем
		radPSController->measureSpeedIncrease( 1.0f, gearRpmSpeedIncDel );//заказываем
		gearRpmSpeedIncDel->active = true;
	}
	//------------------------------------------------------------
	/*
	Обороты уменьшаются
	Время действовать - переключать вниз
	*/
	void MechGearboxController::OnRpmSpeedDec()
	{
		//Отключаем делегат
		gearRpmSpeedDecDel->active = false;

		if( isShiftInitiated )//Переключение уже идёт.. Нам тут делать нечего
			return;

		//Начинаем переключение
		lastGear = iGState->GetActiveGearIndex();//Какая щас передача
		
		if( lastGear > 0 )
		{
			//Включена Direct передача
			targetGear = lastGear - 1;//Нам вниз
			if( targetGear > 0 && iGState->HasGear( targetGear ) )
			{
				//передача Direct ниже текущей и готова к работе
				shiftDelayTimer.Reset( timing->GetShiftDelayTime() );//заряжаем руку
				shiftDelayTimer.AddTempDel( shiftDelayDel );//Будем ждать
				InitiateShift();//А пока надо нажать Clutch и на нейтраль включить
				return;//ну пошли
			}
			//else
			//На нейтраль спускатся нельзя
			//Если такой передачи нет то тоже фигня
		}
		else if( lastGear < 0 )
		{
			//Так. Щас включена Reverse
			targetGear = lastGear + 1;//Понизить Reverse
			if( targetGear < 0 && iGState->HasGear( targetGear ) )
			{
				//Есть такая передача. Фигасе 0_+
				shiftDelayTimer.Reset( timing->GetShiftDelayTime() );//Заряжаем
				shiftDelayTimer.AddTempDel( shiftDelayDel );//Ждать
				InitiateShift();//Много дел ещё
				return;//Пошли работать
			}
			//else
			//На нейтраль спускатся нельзя
		}
		//Короче передач нет
		//Включаем зоны. Будем надеятся на лучшее
		ActivateRpmDels();
	}
	//------------------------------------------------------------
	/*
	Обороты увеличиваются
	Надо переключать вверх
	*/
	void MechGearboxController::OnRpmSpeedInc()
	{
		//Отключаем делегат
		gearRpmSpeedIncDel->active = false;

		if( isShiftInitiated )//Переключение уже идёт
			return;

		//Начинаем
		lastGear = iGState->GetActiveGearIndex();

		if( lastGear > 0 )
		{
			//Включена Direct передача
			targetGear = lastGear + 1;
			if( iGState->HasGear( targetGear ) )
			{
				//Есть такая передача.. работаем
				shiftDelayTimer.Reset( timing->GetShiftDelayTime() );
				shiftDelayTimer.AddTempDel( shiftDelayDel );
				InitiateShift();
				return;
			}
			//нет такой..
		}
		else if( lastGear < 0 )
		{
			//включена Reverse
			targetGear = lastGear - 1;
			if( iGState->HasGear( targetGear ) )
			{
				//Есть передача. Прикинь..
				shiftDelayTimer.Reset( timing->GetShiftDelayTime() );
				shiftDelayTimer.AddTempDel( shiftDelayDel );
				InitiateShift();
				return;
			}
			//такой нет
			//быстрее не поедешь
		}
		//Включаем зоны.
		ActivateRpmDels();
	}
	//------------------------------------------------------------
	/*
	Какая Direct передача соответствует данной скорости
	*/
	int MechGearboxController::GetDirectGearBySpeed()
	{
		//Получаем количество Direct передач
		int directCount = iGState->GetDirectGearsCount();
		//Получаем NormHigh границу для данного driveStyle
		float normHighRadPS = radPSController->getNormHighRadPS();

		//Пройдемся по всем существующим передачам
		for( int i = 1; i <= directCount; i++ )
		{
			float engRpm = speedToGear->GetMotorRadPSAtGear( i );
			if( engRpm < normHighRadPS )
				return i;
		}

		//Ничего не найдено.
		//Скорость очень большая.
		//Включаем последнюю передачу
		return directCount;
	}
	//------------------------------------------------------------
	/*
	Нахождение Reverse передачи по текущей скорости
	Комменты см. в GetDirectGearsBySpeed()
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
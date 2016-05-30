#pragma once
#include "IPedalControl.h"
#include "IMotorControl.h"
#include "IMotorState.h"
#include "IMotorCallback.h"
#include "IGearboxControl.h"
#include "SpeedToGearCalculator.h"
#include "ISpeedometerState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	/*
	Структура представляет данные и интерфейсы необходимиы водителю для управления
	*/
	struct VehicleToDriverData
	{
		DEFINE_CLASS_SHARED_PTR(VehicleToDriverData)
		//--------------------------------//
		VehicleToDriverData()
			: controlAxes(0)
			, controlSystems(0)
		{
		}
		bool isValid()const{
			return	controlAxes > 0	&& throttle != 0 && brake != 0 && steer != 0 && motorState != 0 && motorControl != 0 && motorCallback != 0 && gearboxControl != 0 && speedToGear != 0;
		}
		//Доступные оси управления
		int controlAxes;

		/*
		Установленные системы помощи при вождении
		Водитель знает о специальных системах, и не применяет свои навыки.
		*/
		int controlSystems;

		//Интерфейсы осей управления
		IPedalControl::shared_ptr throttle;
		IPedalControl::shared_ptr brake;
		IPedalControl::shared_ptr clutch;
		IPedalControl::shared_ptr handbrake;
		ISteerControl::shared_ptr steer;

		/*
		Интерфейс для управления двигателем.
		Используется водителем для запуска и остановки стартера
		Неправильно с точки зрения дизайна.
		Нужно сделать спец интерфейсы, а не предоставлять полный интерфейс двигателя
		*/
		IMotorControl::shared_ptr motorControl;
		IMotorState::shared_ptr motorState;
		IMotorCallback::shared_ptr motorCallback;

		/*
		Интерфейс контроля коробки передач( рычаг )
		Водитель сам определит тип коробки( авто или мануал )
		*/
		IGearboxControl::shared_ptr gearboxControl;

		//Индикатор спидометра
		ISpeedometerState::shared_ptr speedometer;

		//Какую передачу включить, чтобы обороты двигателя были в заданном диапазоне.
		//Этот объект расчитывает обороты оси "до коробки"
		SpeedToGearCalculator::const_shared_ptr speedToGear;
	};
	//------------------------------------------------------------
}
}
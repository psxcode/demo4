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
	��������� ������������ ������ � ���������� ����������� �������� ��� ����������
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
		//��������� ��� ����������
		int controlAxes;

		/*
		������������� ������� ������ ��� ��������
		�������� ����� � ����������� ��������, � �� ��������� ���� ������.
		*/
		int controlSystems;

		//���������� ���� ����������
		IPedalControl::shared_ptr throttle;
		IPedalControl::shared_ptr brake;
		IPedalControl::shared_ptr clutch;
		IPedalControl::shared_ptr handbrake;
		ISteerControl::shared_ptr steer;

		/*
		��������� ��� ���������� ����������.
		������������ ��������� ��� ������� � ��������� ��������
		����������� � ����� ������ �������.
		����� ������� ���� ����������, � �� ������������� ������ ��������� ���������
		*/
		IMotorControl::shared_ptr motorControl;
		IMotorState::shared_ptr motorState;
		IMotorCallback::shared_ptr motorCallback;

		/*
		��������� �������� ������� �������( ����� )
		�������� ��� ��������� ��� �������( ���� ��� ������ )
		*/
		IGearboxControl::shared_ptr gearboxControl;

		//��������� ����������
		ISpeedometerState::shared_ptr speedometer;

		//����� �������� ��������, ����� ������� ��������� ���� � �������� ���������.
		//���� ������ ����������� ������� ��� "�� �������"
		SpeedToGearCalculator::const_shared_ptr speedToGear;
	};
	//------------------------------------------------------------
}
}
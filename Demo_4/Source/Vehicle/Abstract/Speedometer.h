#pragma once
#include "IMechanicsState.h"
#include "ISpeedometerState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	/*
	����� ����������.
	��������� �������� � �/� ��� ��/�.
	��� ���������� ���������� ������������ ������ ������
	� ������ ��������, ������������ � ������ �� �����
	*/
	class Speedometer
	{
	public:
		DEFINE_CLASS_SHARED_PTR(Speedometer)
		//--------------------------------//
		/*��������� �������������*/
		struct InitData
		{
			InitData()
				: wheelRadius(0)
			{
			}
			bool isValid()const{ return wheelRadius > 0 && revSensor != 0; }

			float wheelRadius;
			IMechanicsState::shared_ptr revSensor;
		};
		//--------------------------------//
		virtual ~Speedometer()
		{
		}
		Speedometer()
			: wheelRadius(0)
			, revToMS(0)
			, revToKMH(0)
		{
		}

		bool Initialize( const Speedometer::InitData& );

		ISpeedometerState::shared_ptr GetStateInterface();

		inline float getWheelRadius()const{ return wheelRadius; }
		inline float getSpeedMS()const{ return revSensor->getRadPS() * revToMS; }
		inline float getSpeedKMH()const{ return revSensor->getRadPS() * revToKMH; }

		void setWheelRadius( float newRadius );

	protected:
		float wheelRadius;
		IMechanicsState::shared_ptr revSensor;

		/*
		��������� ��� �������� �������.
		���������: (������� �� radPS � RPS) � wheelRadius
		*/
		float revToMS;
		float revToKMH;

		ISpeedometerState::shared_ptr iState;

	private:
		Speedometer( const Speedometer& );
		Speedometer& operator=( const Speedometer& );
	};
	//------------------------------------------------------------
}
}
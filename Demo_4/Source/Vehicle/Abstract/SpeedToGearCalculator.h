#pragma once
#include "IGearboxState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	/*
	����� �������� ��������� ������� ��� "�� �������" �� �������� ��������.
	���� �� ����� IGearboxState �������� ������� ������� ��� "����� �������"( ������� GetOutRadPS() ).
	������ ����� ��������� ������� ��������� ��� ������ ��������� � �������� ���������
	*/
	class SpeedToGearCalculator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(SpeedToGearCalculator)
		//--------------------------------//
		struct InitData
		{
			IGearboxState::const_shared_ptr gearbox;
		};
		//--------------------------------//
		SpeedToGearCalculator()
		{
		}
		~SpeedToGearCalculator()
		{
		}
		bool Initialize( const InitData& data )
		{
			if( data.gearbox == 0 )return false;
			gearbox = data.gearbox;
			return true;
		}
		float GetMotorRadPSAtGear( int gear_index )const
		{
			float ratio = gearbox->GetRatioAtGear( gear_index );
			if( gear_index < 0 )ratio *= -1;
			return gearbox->GetOutRadPS() * ratio;
		}

	protected:
		IGearboxState::const_shared_ptr gearbox;
	};
	//------------------------------------------------------------
}
}
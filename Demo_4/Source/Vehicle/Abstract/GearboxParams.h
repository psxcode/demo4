#pragma once
#include "FlywheelParams.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class GearboxParams : public MechanicsParams
	{
	public:
		DEFINE_CLASS_SHARED_PTR(GearboxParams)
		//--------------------------------//
		GearboxParams()
			: MechanicsParams( MT_GEARBOX )
			, gearboxType( GT_NOT_DEFINED )
			, modes(0)
		{
		}
		GearboxParams( const GearboxParams& params )
			: MechanicsParams( MT_GEARBOX )
			, gearboxType( GT_NOT_DEFINED )
			, modes(0)
		{
			(*this) = params;
		}
		~GearboxParams()
		{
		}
		GearboxParams& operator=( const GearboxParams& p )
		{
			if( this == &p )
				return *this;

			CopyGearboxParams( p );
			return *this;
		}
		virtual bool isValid()const
		{
			return
				shared_ptr_valid(flywheel)		&&
				flywheel->isValid()				&&
				gearboxType != GT_NOT_DEFINED	&&
				!(modes < 0)					&&
				MechanicsParams::isValid();
		}
		virtual MechanicsParams::shared_ptr CreateCopyPtr()const{ return CreateGearboxCopyPtr(); }

		GearboxParams::shared_ptr CreateGearboxCopyPtr()const{ return GearboxParams::shared_ptr( new GearboxParams( *this ) ); }
		/*
		� ��������� � ������� ��������� �������.
		����� ������� ����� �� ��������,
		��� � ���������� ������� ����� ����������
		���� ������� ����� �������������� ��� ��������������
		����������� � ��������� torque � �������
		*/
		FlywheelParams::shared_ptr flywheel;
		/*
		��� �������
		��. gearbox_enums.h\ enum GEARBOX_TYPE
		*/
		GEARBOX_TYPE gearboxType;

		/*
		����� ����������� ��� ������ �������( ���� ��� ������� )
		��. gearbox_enums.h\ enum AUTO_GEARBOX_MODE
		*/
		int modes;

		/*
		����� �������� ������������ ��������� �������� ����� ����������� �������
		*/
		float mechShiftTime;
		
		/*
		������������ �������
		���������� ��������� ���������� ���������� �������
		������ 0 ��������� �� ������ �������� ��� direct ��� reverse
		����������� �������� ����� �����������
		*/
		Demo::vector< float >::type directRatios;
		Demo::vector< float >::type reverseRatios;

	protected:
		void CopyGearboxParams( const GearboxParams& p )
		{
			this->gearboxType		= p.gearboxType;
			this->modes				= p.modes;
			this->directRatios		= p.directRatios;
			this->reverseRatios		= p.reverseRatios;
			this->mechShiftTime		= p.mechShiftTime;
			flywheel.reset();
			if( shared_ptr_valid(p.flywheel) ) flywheel = p.flywheel->CreateFlywheelCopyPtr();
		}
	};
	//------------------------------------------------------------
}
}
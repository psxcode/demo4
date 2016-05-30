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
		Я использую в коробке маленький маховик.
		Когда коробка будет на нейтрали,
		или в переходные моменты между передачами
		этот маховик будет использоваться для преобразования
		подающегося с двигателя torque в обороты
		*/
		FlywheelParams::shared_ptr flywheel;
		/*
		Тип коробки
		см. gearbox_enums.h\ enum GEARBOX_TYPE
		*/
		GEARBOX_TYPE gearboxType;

		/*
		Здесь перечислены все режимы коробки( если это автомат )
		см. gearbox_enums.h\ enum AUTO_GEARBOX_MODE
		*/
		int modes;

		/*
		Время задержки срабатывания включения передачи после поступившей команды
		*/
		float mechShiftTime;
		
		/*
		Коэффициенты передач
		Количество элементов показывает количество передач
		Индекс 0 указывает на первую передачу для direct или reverse
		Нейтральная передача здесь отсутствует
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
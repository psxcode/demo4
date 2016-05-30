#pragma once

#include "Core\DelTimer.h"
#include "GearboxController.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class AutoGearboxController : public GearboxController
	{
	public:
		DEFINE_CLASS_SHARED_PTR(AutoGearboxController)
		//--------------------------------//
		struct AutoGearboxControllerInitData : public GearboxController::GearboxControllerInitData
		{
			AutoGearboxControllerInitData()
				: iGearboxControl(0)
			{
			}
			virtual ~AutoGearboxControllerInitData()
			{
			}
			virtual bool isValid()const{ return iGearboxControl != 0 && GearboxController::GearboxControllerInitData::isValid(); }

			IAutoGearboxControl::shared_ptr iGearboxControl;
		};
		//--------------------------------//
		AutoGearboxController()
			: iControl(0)
			, iCallback(0)
			, iState(0)
		{
		}
		virtual ~AutoGearboxController(){CloseAutoGearboxController();}

		//GearboxController interface
		virtual bool IsNeutral()const;
		virtual bool IsDirect()const;
		virtual bool IsReverse()const;

		virtual bool ShiftUp();
		virtual bool ShiftDown();
		virtual bool ShiftToDirect();
		virtual bool ShiftToNeutral();
		virtual bool ShiftToReverse();

		virtual IGearboxController::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE );

		virtual void CloseGearboxController(){ return CloseAutoGearboxController(); }
		//GearboxController interface

		//Controller interface
		virtual void OperateController();
		//Controller interface

		bool Initialize( const AutoGearboxControllerInitData& data );
		void CloseAutoGearboxController();

	protected:
		//Возвратные от коробки
		void SwitchModeEnded();

		//Срабатывают по своим делегатам, через таймер
		void OnSwitchModeUp();
		void OnSwitchModeDown();
		void OnSwitchOp();

		//Рычаг коробки
		IAutoGearboxControl::shared_ptr iControl;
		IAutoGearboxCallback::shared_ptr iCallback;
		IAutoGearboxState::shared_ptr iState;

		//Делегаты
		delegate_traits::del< void() >::shared_ptr switchModeUpDel;
		delegate_traits::del< void() >::shared_ptr switchModeDownDel;
		delegate_traits::del< void() >::shared_ptr switchModeOpDel;
		delegate_traits::del< void() >::shared_ptr switchModeEndedDel;
		
		//Таймер реакции руки
		Core::DelTimer switchOpTimer;

		AUTO_GEARBOX_MODE targetMode;

		//Флаг процесса переключения
		bool isSwitchInitiated;
	};
	//------------------------------------------------------------
}
}
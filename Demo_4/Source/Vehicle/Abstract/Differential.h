#pragma once
#include "Mechanics.h"
#include "DifferentialParams.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class VehicleAxle;
	//------------------------------------------------------------
	/*
	Это простой симметричный дифференциал без каких либо наворотов
	Здесь также не учитывается dragTorque ( момент сопротивления )
	Также для данного диффера невозможна работа без подключённых осей
	*/
	class Differential : public Mechanics
	{
	public:
		DEFINE_CLASS_SHARED_PTR(Differential)
		//--------------------------------//
		struct DifferentialInitData
		{
			DifferentialInitData()
			{
			}
			/*
			Отсюда надо скопировать только основные параметры.
			Не нужно использовать параметры цепи осей или копировать Params
			Цепью займётся класс Vehicle
			*/
			DifferentialParams::const_shared_ptr diffParams;
		};
		//--------------------------------//
		Differential()
			: Mechanics( MT_DIFFERENTIAL )
			, leftNode(0)
			, rightNode(0)
			, availableModes(0)
			, diffRatio(0)
			, firstNodeRatio(0.5f)
			, torqueMult(0)
		{
		}
		virtual ~Differential() {CloseDifferential();}

		//Mechanics interface
		virtual void CloseMechanics() { CloseDifferential(); }
		virtual float OperateMechanics( float, float );
		virtual MechanicsParams::shared_ptr CreateMechanicsParams()const { return CreateDifferentialParams(); }
		virtual bool isMechanicsChainValid()const;
		virtual IMechanicsState::shared_ptr GetMechanicsStateInterface();

		virtual bool ConnectChildMech( traits::child_mech_in child, int index = 0 );
		virtual void DisconnectChildMech( int index = 0 );
		virtual void DisconnectChildMech( traits::child_mech_in childMech );

		virtual int GetMechSlotCount()const{ return 2; }
		virtual traits::child_mech_type GetChildMech( int slotIndex = 0 );
		//Mechanics interface

		bool Initialize( const DifferentialInitData& );
		DifferentialParams::shared_ptr CreateDifferentialParams()const;
		void CloseDifferential();

	protected:
		void operateWithConnection();
		void operateNoConnection();

		delegate_traits::del0<void>::type operateDel;
		/*
		Сюда цепляем либо колёса, либо дифферы
		*/
		traits::child_mech_type leftNode;
		traits::child_mech_type rightNode;
		//Доступные режимы ( Используется сборка DIFFERENTIAL_MODE )
		int availableModes;
		//Передаточное число Главной пары
		float diffRatio;
		/*
		Передаточное число на Первый узел
		Значение от 0 до 1
		Остаток идёт на Второй узел
		*/
		Core::puFloat firstNodeRatio;
		/*
		Эта переменная используется в формуле для замены (torque * diffRatio * firstNodeRatio).
		Т.к. для симметричного дифференциала на обе оси подаётся одинаковый момент,
		то diffRatio * firstNodeRatio является константой, которую можно вычислить заранее.
		*/
		float torqueMult;

		IMechanicsState::shared_ptr iState;

	private:
		Differential( const Differential& );
		Differential& operator=( const Differential& );
	};
	//------------------------------------------------------------
	/*
	Этот дифференциал обладает полным набором управления процессом передачи мощности
	*/
	class ControlDifferential : public Differential
	{
	public:
		//--------------------------------//
		ControlDifferential()
			: differentialReactTime(0)
			, targetMode( DM_NOT_DEFINED )
			, activeMode( DM_NOT_DEFINED )
		{
		}
		virtual ~ControlDifferential(){Close();}

		virtual void Close();

		bool Initialize( const DifferentialInitData& );
		
		virtual bool SetLockMode( bool lock );
		virtual bool SetDisFirstMode( bool disFirst );
		virtual bool SetDisSecondMode( bool disSecond );

		bool IsLockedMode()const{ return (activeMode & DM_LOCK) != 0; }
		bool IsDisFirstMode()const{ return (activeMode & DM_DIS_FIRST) != 0; }
		bool IsDisSecondMode()const{ return (activeMode & DM_DIS_SECOND) != 0; }

	protected:
		//Целевой режим. Только во время переключения режимов.
		DIFFERENTIAL_MODE targetMode;
		//Активный режим.
		DIFFERENTIAL_MODE activeMode;

		/*
		Время реакции дифференциала
		Имеется ввиду реакция на переключение режимов
		*/
		float differentialReactTime;

	private:
		ControlDifferential( const ControlDifferential& );
		ControlDifferential& operator=( const ControlDifferential& );
	};
	//------------------------------------------------------------
}
}
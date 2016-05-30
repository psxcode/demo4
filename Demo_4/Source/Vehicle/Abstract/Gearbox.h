#pragma once
#include "Flywheel.h"
#include "IGearbox.h"
#include "GearboxParams.h"
#include "PedalFilter.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	/*
	Базовый класс Коробки Передач
	Данная реализация коробки обладает
	- встроеной Педалью Сцепления
	- встроеным Маховиком
	*/
	class Gearbox : public Mechanics
	{
	public:
		DEFINE_CLASS_SHARED_PTR(Gearbox)
		//--------------------------------//
		class traits : public Mechanics::traits
		{
		public:
			typedef Demo::vector< Ogre::Real >::type ratio_container;
		};
		//--------------------------------//
		struct GearboxInitData
		{
			GearboxParams::const_shared_ptr gearboxParams;
		};
		//--------------------------------//
		static const float timeLimit;
		//--------------------------------//
		virtual ~Gearbox() { /*CloseGearbox();*/ }

		//MechanicsInterface
		virtual void CloseMechanics();
		virtual MechanicsParams::shared_ptr CreateMechanicsParams()const{ return CreateGearboxParams(); }
		virtual bool isMechanicsChainValid()const;

		virtual bool ConnectChildMech( traits::child_mech_in child, int slotIndex = 0 );
		virtual void DisconnectChildMech( int slotIndex = 0 );
		virtual void DisconnectChildMech( traits::child_mech_in childMech );

		virtual int GetMechSlotCount()const{ return 1; }
		virtual traits::child_mech_type GetChildMech( int slotIndex = 0 ){ return childMechanics; }
		//MechanicsInterface

		virtual IGearbox::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE ) = 0;
		virtual IPedal::shared_ptr GetClutchInterface( PEDAL_INTERFACE_TYPE ) = 0;
		virtual GearboxParams::shared_ptr CreateGearboxParams()const;

		//интерфейс для IGearboxState
		inline int GetActiveGearIndex()const{ return activeGear; }
		inline int GetTargetGearIndex()const{ return targetGear; }
		inline int GetDirectGearsCount()const{ return int(directCount); };
		inline int GetReverseGearsCount()const{ return int(reverseCount); };
		inline float GetActiveGearRatioMod()const{ return activeRatioMod; }
		inline float GetActiveGearRatioSign()const{ return activeRatioSign; }
		inline float GetMechShiftTime()const{ return mechShiftTime; }
		inline float GetOutRadPS()const{ return outRadPS; }
		float GetRatioAtGear( int gear_index )const;
		bool HasGear( int gear_index )const;
		//интерфейс для IGearboxState

		//интерфейс для IGearboxCallback
		inline void AddShiftInitiatedOnceDel( traits::del_in del ){ shiftInitiatedOnceCall.add( del ); }
		inline void AddShiftInitiatedTempDel( traits::del_in del ){ shiftInitiatedTempCall.add( del ); }
		inline void AddShiftCompletedOnceDel( traits::del_in del ){ shiftCompletedOnceCall.add( del ); }
		inline void AddShiftCompletedTempDel( traits::del_in del ){ shiftCompletedTempCall.add( del ); }
		inline void AddShiftFailedOnceDel( traits::del_in del ){ shiftFailedOnceCall.add( del ); }
		inline void AddShiftFailedTempDel( traits::del_in del ){ shiftFailedTempCall.add( del ); }

		inline void RemoveShiftInitiatedOnceDel( traits::del_in del ){ shiftInitiatedOnceCall.remove( del ); }
		inline void RemoveShiftInitiatedTempDel( traits::del_in del ){ shiftInitiatedTempCall.remove( del ); }
		inline void RemoveShiftCompletedOnceDel( traits::del_in del ){ shiftCompletedOnceCall.remove( del ); }
		inline void RemoveShiftCompletedTempDel( traits::del_in del ){ shiftCompletedTempCall.remove( del ); }
		inline void RemoveShiftFailedOnceDel( traits::del_in del ){ shiftFailedOnceCall.remove( del ); }
		inline void RemoveShiftFailedTempDel( traits::del_in del ){ shiftFailedTempCall.remove( del ); }
		//интерфейс для IGearboxCallback

		inline GEARBOX_TYPE GetGearboxType()const{ return gType; }

	protected:
		Gearbox( GEARBOX_TYPE type )
			: Mechanics( MT_GEARBOX )
			, gType( type )
			, activeGear(0)
			, targetGear(0)
			, activeRatioMod(0)
			, activeRatioSign(0)
			, outRadPS(0)
			, motorDrag(0)
			, directCount(0)
			, reverseCount(0)
		{
		}

		//Параметры из GearboxParams
		/*
		Я использую в коробке маховик.
		Когда коробка будет на нейтрали, или в переходные моменты между передачами,
		этот маховик будет использоваться для преобразования подающегося с двигателя torque в обороты
		*/
		Flywheel::shared_ptr flywheel;
		/*
		Подключение к коробке
		*/
		traits::child_mech_type childMechanics;
		/*
		Коэффициенты передач.
		Количество элементов показывает количество передач.
		Индекс 0 указывает на первую передачу для direct или reverse.
		Нейтральная передача здесь отсутствует.
		*/
		traits::ratio_container directRatios;
		traits::ratio_container reverseRatios;
		traits::ratio_container::size_type directCount;
		traits::ratio_container::size_type reverseCount;

		int modes;
		float mechShiftTime;
		//Параметры из GearboxParams

		/*
		Я использую педаль сцепления ( вместе с самим механизмом )
		*/
		PedalFilter clutch;
		int activeGear;
		int targetGear;
		float activeRatioMod;
		float activeRatioSign;
		float outRadPS;
		float motorDrag;

		delegate_traits::composite0 shiftInitiatedOnceCall;
		delegate_traits::composite0 shiftInitiatedTempCall;
		delegate_traits::composite0 shiftCompletedOnceCall;
		delegate_traits::composite0 shiftCompletedTempCall;
		delegate_traits::composite0 shiftFailedOnceCall;
		delegate_traits::composite0 shiftFailedTempCall;

	private:
		Gearbox();
		Gearbox( const Gearbox& );
		Gearbox& operator=( const Gearbox& );

		GEARBOX_TYPE gType;
	};
	//------------------------------------------------------------
}
}

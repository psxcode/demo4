#pragma once
#include "DriveStyleCalculator.h"
#include "IGearboxCallback.h"
#include "IGearboxState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	/*
	Структура представляет собой отметки на тахометре, которые видит водитель
	Значения в RPM.
	*/
	struct MotorChart
	{
		MotorChart()
			: stallRPM(0)
			, idleRPM(0)
			, startRPM(0)
			, redRPM(0)
			, maxRPM(0)
		{
		}
		bool isValid()const{ return stallRPM > 0 &&	idleRPM > stallRPM && startRPM > idleRPM &&	redRPM > startRPM && maxRPM > redRPM ; }

		float stallRPM;
		float idleRPM;
		float startRPM;
		float redRPM;
		float maxRPM;
	};
	//------------------------------------------------------------
	/*
	Структура содержит отметки для тахометра
	Значения находятся в диапазоне от 0 до 1
	gearIndex показывает передачу для которой дейтсвительны отметки
	*/
	struct MotorChartMarks
	{
		DEFINE_CLASS_SHARED_PTR(MotorChartMarks)
		//--------------------------------//
		MotorChartMarks()
			: min( 0.0f )
			, normLow( 0.4f )
			, normHigh( 0.6f )
			, max( 1.0f )
			, gearIndex( 0 )
		{
		}
		bool isValid()const{ return min.get() < normLow.get() && normLow.get() < normHigh.get() && normHigh.get() < max.get(); }

		int gearIndex;
		Core::puFloat min;
		Core::puFloat normLow;
		Core::puFloat normHigh;
		Core::puFloat max;
	};
	//------------------------------------------------------------
	/*
	Структура содержит пакет отметок на тахометре для некоторых передач.
	С их помощью можно задать границы контроля за оборотами двигателя
	*/
	class MotorChartMarksPack
	{
	public:
		DEFINE_CLASS_SHARED_PTR(MotorChartMarksPack)
		//--------------------------------//
		typedef Demo::map< int, MotorChartMarks::shared_ptr >::type MarksMap;
		//--------------------------------//
		MotorChartMarksPack() {}
		~MotorChartMarksPack() {}
		
		bool isValid()const	{
			MarksMap::const_iterator iter = marksMap.begin();	MarksMap::const_iterator end_iter = marksMap.end();
			for( ; iter != end_iter; ++iter ) if( !iter->second->isValid() ) return false;
			return HasChartMarksForGear(0) && !name.empty();
		}

		//Есть ли отметки для данной передачи
		bool HasChartMarksForGear( int gear_index )const{ return marksMap.find( gear_index ) != marksMap.end(); }
		//Получить отметки для данной передачи
		MotorChartMarks::const_shared_ptr GetChartMarks( int gear_index )const {
			MarksMap::const_iterator iter = marksMap.find( gear_index );
			if( iter == marksMap.end() ) {
				iter = marksMap.find( 0 );
				if( iter == marksMap.end() )return MotorChartMarks::const_shared_ptr();
			}
			return iter->second;
		}

		//Добавить отметки
		bool AddChartMarks( MotorChartMarks::shared_ptr &marks, bool replaceDups = true ) {
			if( !marks->isValid() )return false;
			MarksMap::value_type pair( marks->gearIndex, marks );
			MarksMap::_Pairib ipair = marksMap.insert( pair );
			if( !ipair.second )	{
				if( !replaceDups )return false;
				marksMap.erase( ipair.first );
				marksMap.insert( pair );
			}
			return true;
		}
		
		string_traits::str name;

	protected:
		//Список отметок
		MarksMap marksMap;
	};
	//------------------------------------------------------------
	/*
	Этот класс помогает считать промежуточные значения
	между отметками на агрессивном и спокойном стиле вождения
	*/
	class MotorChartMarksCalculator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(MotorChartMarksCalculator)
		//--------------------------------//
		virtual ~MotorChartMarksCalculator()
		{
		}

		//Получить отметку границы для текущих передачи и стиля вождения
		virtual float GetMin()const = 0;
		virtual float GetNormLow()const = 0;
		virtual float GetNormHigh()const = 0;
		virtual float GetMax()const = 0;

	protected:
		MotorChartMarksCalculator()
		{
		}
		
		IGearboxCallback::shared_ptr iCallback;
		IGearboxState::shared_ptr iState;
	};
	//------------------------------------------------------------
	class ConstantMotorChartMarksCalculator : public MotorChartMarksCalculator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(ConstantMotorChartMarksCalculator)
		//--------------------------------//
		struct CMCMCInitData
		{
			bool isValid()const{ return iCallback != 0 && iState != 0 && mcmp != 0 && mcmp->isValid(); }
			IGearboxCallback::shared_ptr iCallback;
			IGearboxState::shared_ptr iState;
			MotorChartMarksPack::const_shared_ptr mcmp;
		};
		//--------------------------------//
		ConstantMotorChartMarksCalculator()
		{
		}
		virtual ~ConstantMotorChartMarksCalculator()
		{
		}

		//Получить отметку границы для текущих передачи и стиля вождения
		virtual float GetMin()const{ return currentMCM->min.get(); }
		virtual float GetNormLow()const{ return currentMCM->normLow.get(); }
		virtual float GetNormHigh()const{ return currentMCM->normHigh.get(); }
		virtual float GetMax()const{ return currentMCM->max.get(); }

		bool Initialize( const CMCMCInitData& data ) {
			if( !data.isValid() )return false;
			mcmPack = data.mcmp; iCallback = data.iCallback; iState = data.iState;
			onShiftCompleted();
			shiftInitiatedDel.bind( new delegate_traits::del0<void>( this, &ConstantMotorChartMarksCalculator::onShiftInitiated ) );
			shiftCompletedDel.bind( new delegate_traits::del0<void>( this, &ConstantMotorChartMarksCalculator::onShiftCompleted ) );
			iCallback->AddShiftInitiatedOnceDel( shiftInitiatedDel );
			iCallback->AddShiftCompletedOnceDel( shiftCompletedDel );
			iCallback->AddShiftFailedOnceDel( shiftCompletedDel );
			return true;
		}
		void Close() {
			currentMCM.reset();
			mcmPack.reset();
			iCallback->RemoveShiftCompletedOnceDel( shiftCompletedDel );
			iCallback->RemoveShiftFailedOnceDel( shiftCompletedDel );
			iCallback->RemoveShiftInitiatedOnceDel( shiftInitiatedDel );
			shiftCompletedDel.reset();
			shiftInitiatedDel.reset();
			iCallback.reset();
			iState.reset();
		}

	protected:
		void onShiftInitiated(){ currentMCM = mcmPack->GetChartMarks( 0 ); }
		void onShiftCompleted(){ currentMCM = mcmPack->GetChartMarks( iState->GetActiveGearIndex() ); }

		delegate_traits::del0<void>::shared_ptr shiftInitiatedDel;
		delegate_traits::del0<void>::shared_ptr shiftCompletedDel;

		//Установленный пакет
		MotorChartMarksPack::const_shared_ptr mcmPack;

		//Текущие отметки для данной передачи
		MotorChartMarks::const_shared_ptr currentMCM;
	};
	//------------------------------------------------------------
	class LinearMotorChartMarksCalculator : public MotorChartMarksCalculator, public DriveStyleCalculator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(LinearMotorChartMarksCalculator)
		//--------------------------------//
		struct LMCMCInitData : public DriveStyleCalculator::InitData
		{
			bool isValid()const{ return iCallback != 0 && iState != 0 && agr != 0 && agr->isValid() && calm != 0 && calm->isValid() && DriveStyleCalculator::InitData::isValid(); }

			IGearboxCallback::shared_ptr iCallback;
			IGearboxState::shared_ptr iState;
			MotorChartMarksPack::const_shared_ptr agr;
			MotorChartMarksPack::const_shared_ptr calm;
		};
		//--------------------------------//
		LinearMotorChartMarksCalculator()
		{
		}
		virtual ~LinearMotorChartMarksCalculator()
		{
		}

		//Получить отметку границы для текущих передачи и стиля вождения
		virtual float GetMin()const{ return Calculate( currCalm->min.get(), currAgr->min.get() ); }
		virtual float GetNormLow()const{ return Calculate( currCalm->normLow.get(), currAgr->normLow.get() ); }
		virtual float GetNormHigh()const{ return Calculate( currCalm->normHigh.get(), currAgr->normHigh.get() ); }
		virtual float GetMax()const{ return Calculate( currCalm->max.get(), currAgr->max.get() ); }

		bool Initialize( const LMCMCInitData& data ) {
			if( !data.isValid() )return false;
			iStyle = data.iStyle;
			iCallback = data.iCallback;
			iState = data.iState;
			agr = data.agr;
			calm = data.calm;
			onShiftCompleted();
			shiftInitiatedDel.bind( new delegate_traits::del0<void>( this, &LinearMotorChartMarksCalculator::onShiftInitiated ) );
			shiftCompletedDel.bind( new delegate_traits::del0<void>( this, &LinearMotorChartMarksCalculator::onShiftCompleted ) );
			iCallback->AddShiftInitiatedOnceDel( shiftInitiatedDel );
			iCallback->AddShiftCompletedOnceDel( shiftCompletedDel );
			iCallback->AddShiftFailedOnceDel( shiftCompletedDel );
			return true;
		}
		void CloseMotorChartMarksCalculator() {
			currAgr.reset();
			currCalm.reset();
			agr.reset();
			calm.reset();
			iCallback->RemoveShiftCompletedOnceDel( shiftCompletedDel );
			iCallback->RemoveShiftFailedOnceDel( shiftCompletedDel );
			iCallback->RemoveShiftInitiatedOnceDel( shiftInitiatedDel );
			shiftCompletedDel.reset();
			shiftInitiatedDel.reset();
			iCallback.reset();
			iState.reset();
		}

	protected:
		void onShiftInitiated() { currCalm = calm->GetChartMarks( 0 ); currAgr = agr->GetChartMarks( 0 ); }
		void onShiftCompleted() { currCalm = calm->GetChartMarks( iState->GetActiveGearIndex() ); currAgr = calm->GetChartMarks( iState->GetActiveGearIndex() ); }

		delegate_traits::del0<void>::shared_ptr shiftInitiatedDel;
		delegate_traits::del0<void>::shared_ptr shiftCompletedDel;

		//Установленный пакет
		MotorChartMarksPack::const_shared_ptr agr;
		MotorChartMarksPack::const_shared_ptr calm;

		//Текущие отметки для данной передачи
		MotorChartMarks::const_shared_ptr currAgr;
		MotorChartMarks::const_shared_ptr currCalm;
	};
	//------------------------------------------------------------
}
}
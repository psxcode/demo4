#pragma once
#include "Core\DelArray.h"
#include "Core\VoidDelegate.h"
#include "Core\Controller.h"
#include "Core\DelTimer.h"
#include "Vehicle\VehicleTypes.h"
#include "Vehicle\IMotorState.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class MotorRPMZoneController : Core::Controller
	{
	public:
		//--------------------------------//
		/*
		Именованые константы для идентификации зон для RPM
		*/
		enum ZONE
		{
			Z_NOT_DEFINED = 0,
			Z_BELOW_MIN   = 0x1,
			Z_BELOW_NORM  = 0x2,
			Z_NORM        = 0x4,
			Z_ABOVE_NORM  = 0x8,
			Z_ABOVE_MAX   = 0x10,
			Z_STALL       = 0x20,
			Z_START		  = 0x40
		};
		//--------------------------------//
		struct RPMZoneTiming
		{
			RPMZoneTiming()
				: zoneReactionTime(0)
				, speedReactionTime(0)
			{
			}
			bool isValid()const;

			float zoneReactionTime;
			float speedReactionTime;
		};
		//--------------------------------//
		/*
		Необходимые стартовые данные
		*/
		struct InitData
		{
			InitData()
				: motorState(0)
				, zonesToControl( Z_NOT_DEFINED )
			{
			}
			virtual bool isValid()const{ return motorState != 0 && motorState->isValid(); }

			//Сборка зон для контроля
			int zonesToControl;
			const IMotorState* motorState;
		};
		//--------------------------------//
		//Минимальное время
		static const float timeLimit;
		//Минимальная разница RPM
		static const float rpmLimit;
		//--------------------------------//
		MotorRPMZoneController()
			: Core::Controller( Core::CT_VD_RPMZONE_CONTROLLER )
			, pZoneTimerDel(0)
			, pSpeedTimerDel(0)
			, lastZone(Z_NOT_DEFINED)
			, rpmSpeed(0)
			, spFirstRpm(0)
			, spSecondRpm(0)
			, isMeasuringSpeed(false)
			, motorState(0)
		{
		}

		/*
		Замерить скорость изменения RPM
		При этом используется время из тайминга
		timeMultiplier используеся для изменения стандартного времени реакции
		*/
		virtual void MeasureSpeed( float timeMultiplier = 1.0f );
		/*
		Замерить скорость изменения RPM
		Добавить этот делегат в tempCall
		*/
		virtual void MeasureSpeed( const Core::BaseDelegate*, float timeMultiplier = 1.0f );
		/*
		Замерить скорость изменения RPM
		Вызвать делегаты только при росте RPM
		*/
		virtual void MeasureSpeedInc( const Core::BaseDelegate*, float timeMultiplier = 1.0f );
		/*
		Замерить скорость изменения RPM
		Вызвать делегаты только при уменьшении RPM
		*/
		virtual void MeasureSpeedDec( const Core::BaseDelegate*, float timeMultiplier = 1.0f );


		void RefreshZone();
		//Получить текущую скорость изменения RPM
		inline float GetSpeed()const{ return rpmSpeed; }
		//Получить текущую Зону RPM
		inline ZONE GetZone()const{ return lastZone; }
		//Происходит ли замер скорости изменения RPM
		inline bool IsMeasuringSpeed()const{ return isMeasuringSpeed; }
		//Получить значения RPM для отметки при заданных диапазоне на тахометре и стиле вождения
		float GetMinRpm()const{ return motorState->getMotorChart().idleRPM + (( motorState->getMotorChart().maxRPM - motorState->getMotorChart().idleRPM ) * rpmData->GetMin()); }
		float GetNormLowRpm()const{ return motorState->getMotorChart().idleRPM + ((motorState->getMotorChart().maxRPM - motorState->getMotorChart().idleRPM) * rpmData->GetNormLow()); }
		float GetNormHighRpm()const{ return motorState->getMotorChart().idleRPM + ((motorState->getMotorChart().maxRPM - motorState->getMotorChart().idleRPM) * rpmData->GetNormHigh()); }
		float GetMaxRpm()const{ return motorState->getMotorChart().idleRPM + ((motorState->getMotorChart().maxRPM - motorState->getMotorChart().idleRPM) * rpmData->GetMax()); }

		//Добавка - Удаление делегатов
		inline void AddBelowMinOnceDel( const Core::BaseDelegate* del ){ belowMin.addDelegate( del ); }
		inline void AddBelowNormOnceDel( const Core::BaseDelegate* del ){ belowNorm.addDelegate( del ); }
		inline void AddNormOnceDel( const Core::BaseDelegate* del ){ norm.addDelegate( del ); }
		inline void AddAboveNormOnceDel( const Core::BaseDelegate* del ){ aboveNorm.addDelegate( del ); }
		inline void AddAboveMaxOnceDel( const Core::BaseDelegate* del ){ aboveMax.addDelegate( del ); }
		inline void AddStallOnceDel( const Core::BaseDelegate* del ){ stall.addDelegate( del ); }

		inline void RemoveBelowMinOnceDel( const Core::BaseDelegate* del ){ belowMin.removeDelegate( del ); }
		inline void RemoveBelowNormOnceDel( const Core::BaseDelegate* del ){ belowNorm.removeDelegate( del ); }
		inline void RemoveNormOnceDel( const Core::BaseDelegate* del ){ norm.removeDelegate( del ); }
		inline void RemoveAboveNormOnceDel( const Core::BaseDelegate* del ){ aboveNorm.removeDelegate( del ); }
		inline void RemoveAboveMaxOnceDel( const Core::BaseDelegate* del ){ aboveMax.removeDelegate( del ); }
		inline void RemoveSpeedTempDel( const Core::BaseDelegate* del ){ speed.removeDelegate( del ); }
		inline void RemoveSpeedIncTempDel( const Core::BaseDelegate* del ){ speedInc.removeDelegate( del ); }
		inline void RemoveSpeedDecTempDel( const Core::BaseDelegate* del ){ speedDec.removeDelegate( del ); }
		inline void RemoveStallOnceDel( const Core::BaseDelegate* del ){ stall.removeDelegate( del ); }

	protected:
		typedef Core::VoidDelegate< MotorRPMZoneController > ThisVoidDelegate;

		//Код таймера для зон
		void OnZoneTimer();
		//Код таймера для скорости
		void OnSpeedTimer();

		ThisVoidDelegate* pZoneTimerDel;
		ThisVoidDelegate* pSpeedTimerDel;

		const IMotorState* motorState;
		
		Core::DelArray belowMin;
		Core::DelArray belowNorm;
		Core::DelArray norm;
		Core::DelArray aboveNorm;
		Core::DelArray aboveMax;
		Core::DelArray speed;
		Core::DelArray speedInc;
		Core::DelArray speedDec;
		Core::DelArray stall;

		//Таймер реакции на зоны RPM
		Core::DelTimer zoneTimer;
		//Текущая зона RPM
		ZONE lastZone;

		//Текущая скорость изменения RPM
		float rpmSpeed;
		//Значение RPM до замера скорости
		float spFirstRpm;
		//Значение RPM после замера скорости
		float spSecondRpm;
		//Таймер реакции на скорость изменения RPM
		Core::DelTimer speedTimer;
		//Флаг процесса измерения скорости
		bool isMeasuringSpeed;
	};
	//------------------------------------------------------------
}//namespace Vehicle
}//namespace Demo
#pragma once
#include "MotorRPMZoneController.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class RPMZoneTimingCalculator : public DriveStyleCalculator
	{
	public:
		//--------------------------------//
		struct ZTInitData : public DriveStyleCalculator::InitData
		{
			virtual bool isValid()const
			{
				return calmTiming.isValid() && agrTiming.isValid() && InitData::isValid();
			}

			MotorRPMZoneController::RPMZoneTiming calmTiming;
			MotorRPMZoneController::RPMZoneTiming agrTiming;
		};
		//--------------------------------//
		bool Initialize( const ZTInitData& );

		float GetRpmZoneReactionTime()const;
		float GetRpmSpeedReactionTime()const;

	protected:
		RPMZoneTiming calmTiming;
		RPMZoneTiming agrTiming;
	};
	//------------------------------------------------------------
	class LinearMotorRPMZoneController : public MotorRPMZoneController
	{
	public:
		//--------------------------------//
		struct LinearInitData : public MotorRPMZoneController::InitData
		{
			LinearInitData()
				: timing(0)
				, rpmData(0)
			{
			}
			virtual bool isValid()const
			{
				return
					InitData::isValid() &&
					timing != 0 &&
					rpmdata != 0 &&
			}
			RPMZoneTimingCalculator* timing;
			const RPMControlDataCalculator* rpmData;
		};
		//--------------------------------//

		//Интерфейс класса Controller
		virtual void OperateController();
		virtual void CloseController();

		//Собственный Initialize
		bool Initialize( const LinearInitData& init );

		/// @copydoc MotorRPMZoneController::MeasureSpeed
		virtual void MeasureSpeed( float timeMultiplier = 1.0f );
		/// @copydoc MotorRPMZoneController::MeasureSpeed
		virtual void MeasureSpeed( const Core::BaseDelegate*, float timeMultiplier = 1.0f );
		/// @copydoc MotorRPMZoneController::MeasureSpeedInc
		virtual void MeasureSpeedInc( const Core::BaseDelegate*, float timeMultiplier = 1.0f );
		/// @copydoc MotorRPMZoneController::MeasureSpeedDec
		virtual void MeasureSpeedDec( const Core::BaseDelegate*, float timeMultiplier = 1.0f );

	protected:
		RPMZoneTimingCalculator* timing;
		const RPMControlDataCalculator* rpmData;
	};
	//------------------------------------------------------------
}
}


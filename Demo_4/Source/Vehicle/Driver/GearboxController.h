#pragma once

#include "Core\Controller.h"
#include "Vehicle\Abstract\IGearboxControl.h"
#include "Vehicle\Abstract\IGearboxCallback.h"
#include "Vehicle\Abstract\IGearboxState.h"
#include "Vehicle\Abstract\DriverTiming.h"
#include "IGearboxController.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class GearboxController : public Core::Controller
	{
	public:
		DEFINE_CLASS_SHARED_PTR(GearboxController)
		//--------------------------------//
		class traits : public Controller::traits
		{
		public:
			typedef delegate_traits::composite0::traits::del_in del_in;
		};
		//--------------------------------//
		struct GearboxControllerInitData
		{
			GearboxControllerInitData()
				: driveOptions(0)
			{
			}
			virtual ~GearboxControllerInitData(){}
			virtual bool isValid()const{ return !timing.isNull(); }

			int driveOptions;
			DriverTimingCalculator::const_shared_ptr timing;
		};
		//--------------------------------//
		virtual ~GearboxController(){ CloseGearboxController(); }
		
		//GearboxController interface
		virtual bool IsNeutral()const = 0;
		virtual bool IsDirect()const = 0;
		virtual bool IsReverse()const = 0;

		virtual bool ShiftUp() = 0;
		virtual bool ShiftDown() = 0;
		virtual bool ShiftToDirect() = 0;
		virtual bool ShiftToNeutral() = 0;
		virtual bool ShiftToReverse() = 0;

		virtual IGearboxController::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE type ) = 0;

		virtual void CloseGearboxController();
		//GearboxController interface

		inline void AddShiftStartedOnceDel( traits::del_in del ){ shiftStartedDelArray.add( del ); }
		inline void AddShiftEndedOnceDel( traits::del_in del ){ shiftEndedDelArray.add( del ); }
		inline void RemoveShiftStartedOnceDel( traits::del_in del ){ shiftStartedDelArray.remove( del ); }
		inline void RemoveShiftEndedOnceDel( traits::del_in del ){ shiftEndedDelArray.remove( del ); }
		
	protected:
		GearboxController()
			: driveOptions(0)
		{
		}

		delegate_traits::composite0 shiftStartedDelArray;
		delegate_traits::composite0 shiftEndedDelArray;

		int driveOptions;
		DriverTimingCalculator::const_shared_ptr timing;

	private:
		GearboxController( const GearboxController& );
		GearboxController& operator=( const GearboxController& );
	};
	//------------------------------------------------------------
}//namespace Vehicle
}//namespace Demo
#pragma once
#include "IGearbox.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class Gearbox;
	class MechGearbox;
	class AutoGearbox;
	//------------------------------------------------------------
	class IGearboxCallback : public IGearbox
	{
	public:
		DEFINE_CLASS_SHARED_PTR(IGearboxCallback)
		//--------------------------------//
		typedef delegate_traits::composite0::traits::del_in del_in;
		//--------------------------------//
		virtual ~IGearboxCallback()
		{
		}
		//Èםעונפויס IGearbox
		virtual IGearbox::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE ) = 0;
		//Èםעונפויס IGearbox

		//טםעונפויס VehicleGearbox
		void AddShiftInitiatedOnceDel( del_in );
		void AddShiftInitiatedTempDel( del_in );
		void AddShiftCompletedOnceDel( del_in );
		void AddShiftCompletedTempDel( del_in );
		void AddShiftFailedOnceDel( del_in );
		void AddShiftFailedTempDel( del_in );

		void RemoveShiftInitiatedOnceDel( del_in );
		void RemoveShiftInitiatedTempDel( del_in );
		void RemoveShiftCompletedOnceDel( del_in );
		void RemoveShiftCompletedTempDel( del_in );
		void RemoveShiftFailedOnceDel( del_in );
		void RemoveShiftFailedTempDel( del_in );
		//טםעונפויס VehicleGearbox

		GEARBOX_TYPE GetGType()const{ return gType; }

	protected:
		IGearboxCallback( Gearbox& g );
		
	private:
		IGearboxCallback();
		
		GEARBOX_TYPE gType;
	};
	//------------------------------------------------------------
	class IMechGearboxCallback : public IGearboxCallback
	{
		friend class MechGearbox;
	public:
		DEFINE_CLASS_SHARED_PTR(IMechGearboxCallback)
		//--------------------------------//
		virtual ~IMechGearboxCallback()
		{
		}
		//Èםעונפויס IGearbox
		virtual IGearbox::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE );
		//Èםעונפויס IGearbox

	protected:
		IMechGearboxCallback( MechGearbox& mG );

		MechGearbox& mGearbox;
	};
	//------------------------------------------------------------
	class IAutoGearboxCallback : public IGearboxCallback
	{
		friend class AutoGearbox;
	public:
		DEFINE_CLASS_SHARED_PTR(IAutoGearboxCallback)
		//--------------------------------//
		virtual ~IAutoGearboxCallback()
		{
		}
		//Èםעונפויס IGearbox
		virtual IGearbox::shared_ptr GetInterface( GEARBOX_INTERFACE_TYPE );
		//Èםעונפויס IGearbox

		//טםעונפויס VehicleAutoGearbox
		void AddSwitchInitiatedOnceDel( del_in );
		void AddSwitchInitiatedTempDel( del_in );
		void AddSwitchCompletedOnceDel( del_in );
		void AddSwitchCompletedTempDel( del_in );
		void AddSwitchFailedOnceDel( del_in );
		void AddSwitchFailedTempDel( del_in );

		void RemoveSwitchInitiatedOnceDel( del_in );
		void RemoveSwitchInitiatedTempDel( del_in );
		void RemoveSwitchCompletedOnceDel( del_in );
		void RemoveSwitchCompletedTempDel( del_in );
		void RemoveSwitchFailedOnceDel( del_in );
		void RemoveSwitchFailedTempDel( del_in );
		//טםעונפויס VehicleAutoGearbox

	protected:
		IAutoGearboxCallback( AutoGearbox& aG );
					
		AutoGearbox& aGearbox;
	};
	//------------------------------------------------------------
}
}
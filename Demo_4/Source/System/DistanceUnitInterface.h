#pragma once

namespace Demo
{
//------------------------------------------------------------
class DistanceUnitInterface
{
	friend class SystemInterface;
	friend class UnitConverter;
public:
	//--------------------------------//
	enum UNIT_SCALE
	{
		US_UNIT = 0,
		US_INCH,
		US_METER,
		US_CENTIMETER
	};
	//--------------------------------//
	static inline UNIT_SCALE GetSystemUnitScale(){ return _system_unit; }
	static inline UNIT_SCALE GetDisplayUnitScale(){ return _display_unit; }

	//--------------------------------//

private:
	//--------------------------------//
	static void SetDisplayUnitScale( UNIT_SCALE );
	static void SetSystemUnitScale( UNIT_SCALE );
		
	static float _coeff;
	static float _coeffInv;
	static UNIT_SCALE _system_unit;
	static UNIT_SCALE _display_unit;
	//--------------------------------//
};
//------------------------------------------------------------
}
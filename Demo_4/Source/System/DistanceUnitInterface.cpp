#include "DistanceUnitInterface.h"

#define _USE_MATH_DEFINES
#include <cmath>

namespace Demo
{
//------------------------------------------------------------
float DistanceUnitInterface::_coeff( 1.0f );
float DistanceUnitInterface::_coeffInv( 1.0f );
DistanceUnitInterface::UNIT_SCALE DistanceUnitInterface::_system_unit( US_UNIT );
DistanceUnitInterface::UNIT_SCALE DistanceUnitInterface::_display_unit( US_UNIT );
//------------------------------------------------------------
void DistanceUnitInterface::SetSystemUnitScale( UNIT_SCALE us )
{
	_system_unit = us;
	if( us == US_UNIT || us == _display_unit || _display_unit == US_UNIT )
	{
		_coeff = 1.0f;
		_coeffInv = 1.0f;
		return;
	}
	switch( us )
	{
	case US_INCH:
		if( _display_unit == US_METER ) { _coeff = 39.37f; break; }
		if( _display_unit == US_CENTIMETER ) {_coeff = 0.3937f; break; }
	case US_METER:
		if( _display_unit == US_INCH ) { _coeff = 0.0254f; break; }
		if( _display_unit == US_CENTIMETER ) { _coeff = 0.01f; break; }
	case US_CENTIMETER:
		if( _display_unit == US_INCH ) { _coeff = 2.54f; break; }
		if( _display_unit == US_METER ) { _coeff = 100.0f; break; }
	default:
		_coeff = 1.0f;
		break;
	}
	_coeffInv = 1/_coeff; 
}
//------------------------------------------------------------
void DistanceUnitInterface::SetDisplayUnitScale( UNIT_SCALE us )
{
	_display_unit = us;
	if( us == US_UNIT || us == _system_unit || _system_unit == US_UNIT )
	{
		_coeff = 1.0f;
		_coeffInv = 1.0f;
		return;
	}
	switch( us )
	{
	case US_INCH:
		if( _system_unit == US_METER ) { _coeff = 0.0254f; break; }
		if( _system_unit == US_CENTIMETER ) { _coeff = 2.54f; break; }
	case US_METER:
		if( _system_unit == US_INCH ) { _coeff = 39.37f; break; }
		if( _system_unit == US_CENTIMETER ) { _coeff = 100.0f; break; }
	case US_CENTIMETER:
		if( _system_unit == US_INCH ) { _coeff = 0.3937f; break; }
		if( _system_unit == US_METER ) { _coeff = 0.01f; break; }
	default:
		_coeff = 1.0f;
		break;
	}
	_coeffInv = 1/_coeff; 
}
//------------------------------------------------------------
}
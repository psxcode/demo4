#pragma once
#include "mechanics_enums.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class Mechanics;
	//------------------------------------------------------------
	class IMechanicsState
	{
	public:
		DEFINE_CLASS_SHARED_PTR(IMechanicsState)
		//--------------------------------//
		IMechanicsState( Mechanics &m )
			: baseMechanics( m )
		{
		}
		virtual ~IMechanicsState()
		{
		}
		
		MECHANICS_TYPE getTypeMechanics()const;
		bool isInitializedMechanics()const;
		float getTorque()const;
		float getDrag()const;
		float getRadPS()const;

	protected:
		Mechanics &baseMechanics;

	private:
		IMechanicsState();
		IMechanicsState( const IMechanicsState& );
		IMechanicsState& operator=( const IMechanicsState& );
	};
	//------------------------------------------------------------
}
}
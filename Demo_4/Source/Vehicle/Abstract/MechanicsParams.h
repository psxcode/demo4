#pragma once
#include "mechanics_enums.h"
#include "Core\ValueByValue.h"

namespace Demo
{
namespace Core
{
	//------------------------------------------------------------
	typedef ValueByValue< Ogre::Real, Ogre::Real > OgreRealVBV;
	//------------------------------------------------------------
}
namespace Vehicle
{
	//------------------------------------------------------------
	class MechanicsParams
	{
	public:
		DEFINE_CLASS_SHARED_PTR(MechanicsParams)
		//--------------------------------//
		virtual ~MechanicsParams() {CloseMechanicsParams();}
		virtual shared_ptr CreateCopyPtr()const = 0;
		virtual bool isValid()const
		{
			return !nameMechanics.empty() && !(maxDrag < 0);
		}
		void CloseMechanicsParams()
		{
			nameMechanics.clear();
			maxDrag = 0;
			dragLookup.reset();
		}
		inline MECHANICS_TYPE getMechType()const { return mechType; }

		string_traits::str nameMechanics;
		float maxDrag;
		Core::OgreRealVBV::shared_ptr dragLookup;

	protected:
		MechanicsParams( MECHANICS_TYPE t )
			: mechType( t )
			, maxDrag(0)
		{
		}
		void CopyMechanicsParams( const MechanicsParams& base )
		{
			nameMechanics = base.nameMechanics;
			maxDrag = base.maxDrag;
			dragLookup.reset();
			if(shared_ptr_valid(base.dragLookup)) dragLookup = base.dragLookup->CreateCopyPtr();
		}

	private:
		MechanicsParams();
		MechanicsParams( const MechanicsParams& );

		MECHANICS_TYPE mechType;
	};
	//------------------------------------------------------------
}
}
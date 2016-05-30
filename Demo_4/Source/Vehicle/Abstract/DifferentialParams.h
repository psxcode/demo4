#pragma once
#include "Core\RangeVar.h"
#include "MechanicsParams.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	class DifferentialParams : public MechanicsParams
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DifferentialParams)
		//--------------------------------//
		DifferentialParams()
			: MechanicsParams( MT_DIFFERENTIAL )
			, availableModes(0)
			, diffRatio(0)
			, vehicleAxleIndex(-1)
			, firstNodeRatio(0)
		{
		}
		DifferentialParams( const DifferentialParams& diff )
			: MechanicsParams( MT_DIFFERENTIAL )
			, availableModes(0)
			, diffRatio(0)
			, vehicleAxleIndex(-1)
			, firstNodeRatio(0)
		{
			(*this) = diff;
		}
		virtual ~DifferentialParams() {CloseDifferentialParams();}
		DifferentialParams& operator=( const DifferentialParams& p )
		{
			if( this == &p )
				return *this;

			CopyDifferentialParams( p );
			return *this;
		}
		virtual MechanicsParams::shared_ptr CreateCopyPtr()const{ return CreateDifferentialCopyPtr(); }
		virtual bool isValid()const
		{
			return !(availableModes < 0) && diffRatio > 0 && MechanicsParams::isValid();
		}

		void CloseDifferentialParams()
		{
			availableModes = 0;
			diffRatio = 0;
			vehicleAxleIndex = -1;
			firstNodeRatio = 0;
			shared_ptr_reset(parentDiffParams);
			shared_ptr_reset(leftDiffParams);
			shared_ptr_reset(rightDiffParams);
			CloseMechanicsParams();
		}
		DifferentialParams::shared_ptr CreateDifferentialCopyPtr()const{ return DifferentialParams::shared_ptr( new DifferentialParams( *this ) ); }
		
		//Доступные режимы ( Используется сборка DIFFERENTIAL_MODE )
		int availableModes;
		//Передаточное число Главной пары
		float diffRatio;
		/*
		Индекс оси машины на которую предполагается присоединить дефференциал.(zeroBased)
		Как параметр DifferentialParamsRSD отсутствует.
		*/
		int vehicleAxleIndex;
		/*
		Передаточное число на Первый узел
		Значение от 0 до 1
		Остаток идёт на Второй узел
		*/
		Core::puFloat firstNodeRatio;
		
		/*
		DifferentialParams включает в себя возможно присоединенные child DifferentialParams,
		и если сам является child - parent DifferentialParams
		*/
		DifferentialParams::const_shared_ptr parentDiffParams;
		DifferentialParams::shared_ptr leftDiffParams;
		DifferentialParams::shared_ptr rightDiffParams;

	protected:
		void CopyDifferentialParams( const DifferentialParams& p )
		{
			availableModes = p.availableModes;
			diffRatio = p.diffRatio;
			firstNodeRatio = p.firstNodeRatio;
			vehicleAxleIndex = p.vehicleAxleIndex;
			shared_ptr_reset(leftDiffParams);
			if( shared_ptr_valid(p.leftDiffParams) )
			{
				leftDiffParams = p.leftDiffParams->CreateDifferentialCopyPtr();
				shared_ptr_reset(leftDiffParams->parentDiffParams);
				leftDiffParams->parentDiffParams.bind( this, Ogre::SPFM_NO_FREE );
			}
			shared_ptr_reset(rightDiffParams);
			if( shared_ptr_valid(p.rightDiffParams) )
			{
				rightDiffParams = p.rightDiffParams->CreateDifferentialCopyPtr();
				shared_ptr_reset(rightDiffParams->parentDiffParams);
				rightDiffParams->parentDiffParams.bind( this, Ogre::SPFM_NO_FREE );
			}
		}
	};
	//------------------------------------------------------------
}
}
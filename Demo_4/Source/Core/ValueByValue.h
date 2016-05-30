#pragma once

#include <typeinfo>

#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
#include "System/dsVehicleTypes.h"
#endif

namespace Demo
{
namespace Core
{
	//////////////////////////////////////////////////////////////
	//------------------------------------------------------------
	class BaseVBV
	{
	public:
		DEFINE_CLASS_SHARED_PTR(BaseVBV)
		//--------------------------------//
		enum VBV_TYPE
		{
			VBV_NOT_DEFINED,
			VBV_CONSTANT,
			VBV_LINEAR,
			VBV_TABLE
		};
		//--------------------------------//
		virtual ~BaseVBV()
		{
		}
		virtual const std::type_info& getInTypeid()const = 0;
		virtual const std::type_info& getOutTypeid()const = 0;
		virtual shared_ptr CreateCopyBaseVBVPtr()const = 0;
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
		virtual System::dsVBV CreateDotSceneVBV()const = 0;
#endif

		inline string_traits::outref getName()const{ return name; }

	protected:
		void CopyBaseVBV( const BaseVBV& in )
		{
			name = in.name;
		}

		string_traits::str name;
	};
	//////////////////////////////////////////////////////////////
	//------------------------------------------------------------
	template< class inT, class outT >
	class _DemoExport ValueByValue : public BaseVBV
	{
	public:
		DEFINE_TCLASS_SHARED_PTR(ValueByValue)
		//--------------------------------//
		virtual ~ValueByValue()
		{
		}

		//BaseVBV interface
		virtual const type_info& getInTypeid()const{ return typeid( inT ); }
		virtual const type_info& getOutTypeid()const{ return typeid( outT ); }
		virtual BaseVBV::shared_ptr CreateCopyBaseVBVPtr()const{ return CreateCopyPtr(); }
		//BaseVBV interface

		virtual shared_ptr CreateCopyPtr()const = 0;
		virtual outT GetValue( const inT& value )const = 0;
		virtual void Close() = 0;
		virtual void MultiplyVBV( const inT& in, const outT& out ) = 0;
		
		inline bool isInitialized()const{ return _isInitialized; }
		inline outT GetValueWithMult( const inT& value )const{ return outMult * GetValue( inMult * value ); }
		inline void SetMult( const inT& in, const outT& out ){ inMult = in; outMult = out; }
		inline VBV_TYPE getVBVType()const{ return vbvType; }
		inline inT getInMult()const{ return inMult; }
		inline outT getOutMult()const{ return outMult; }

	protected:
		ValueByValue( VBV_TYPE t )
			: vbvType( t )
			, inMult(0)
			, outMult(0)
			, _isInitialized( false )
		{
		}
		void CopyValueByValue( const ValueByValue< inT, outT > &in )
		{
			CopyBaseVBV( in );
			vbvType = in.vbvType;
			inMult = in.inMult;
			outMult = in.outMult;
			_isInitialized = in._isInitialized;
		}

		VBV_TYPE vbvType;
		inT inMult;
		outT outMult;
		bool _isInitialized;
	};
	//////////////////////////////////////////////////////////////
	//------------------------------------------------------------
	template< class inT, class outT >
	class _DemoExport ConstantVBV : public ValueByValue< inT, outT >
	{
	public:
		ConstantVBV()
			: ValueByValue< inT, outT >( VBV_CONSTANT )
			, constantValue(0)
		{
		}
		virtual ~ConstantVBV()
		{
		}
		//ValueByValue interface
		virtual shared_ptr CreateCopyPtr()const{ return shared_ptr( new ConstantVBV< inT, outT >(*this) ); }
		virtual outT GetValue( const inT& )const{ return constantValue; }
		virtual void Close(){ constantValue = 0; _isInitialized = false; }
		virtual void MultiplyVBV( const inT& in, const outT& out ){ constantValue *= out; }
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
		virtual System::dsVBV CreateDotSceneVBV()const {
			System::dsVBV rsd;
			rsd.name = name;
			rsd.startValue = constantValue;
			return rsd;
		}
#endif
		//ValueByValue interface

#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
		bool Initialize( const System::dsVBV& data ) {
			constantValue = data.startValue;
			name = data.name;
			if( name.empty() )return false;
			return _isInitialized = true;
		}
#endif
		outT getConstantValue()const{ return constantValue; }

	protected:

		outT constantValue;
	};
	//////////////////////////////////////////////////////////////
	//------------------------------------------------------------
	template< class inT, class outT >
	class _DemoExport LinearVBV : public ValueByValue< inT, outT >
	{
	public:
		LinearVBV()
			: ValueByValue< inT, outT >( VBV_LINEAR )
			, startValue(0)
			, multValue(0)
		{
		}
		virtual ~LinearVBV()
		{
		}
		//ValueByValue interface
		virtual shared_ptr CreateCopyPtr()const{ return shared_ptr( new LinearVBV< inT, outT >( *this ) ); }
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
		virtual System::dsVBV CreateDotSceneVBV()const {
			System::dsVBV rsd;
			rsd.name = name;
			rsd.startValue = startValue;
			rsd.multValue = multValue;
			return rsd;
		}
#endif
		virtual void Close(){ startValue = 0; multValue = 0; _isInitialized = false; }
		virtual void MultiplyVBV( const inT& in, const outT& out ){ startValue *= out; multValue *= out; }
		virtual outT GetValue( const inT& value )const{ return startValue + multValue * value; }
		//ValueByValue interface

#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
		bool Initialize( const System::dsVBV& data ) {
			if( data.name.empty() )return false;
			name = data.name;
			startValue = data.startValue;
			multValue = data.multValue;
			return _isInitialized = true;
		}
#endif
		inline outT getStartValue()const{ return startValue; }
		inline outT getMultValue()const{ return multValue; }

	protected:
		outT startValue;
		outT multValue;
	};
	//////////////////////////////////////////////////////////////
	//------------------------------------------------------------
	template< class inT, class outT >
	class _DemoExport SquaredLinearVBV : public LinearVBV< inT, outT >
	{
	public:
		virtual ~SquaredLinearVBV()
		{
		}
		//ValueByValue interface
		virtual shared_ptr CreateCopyPtr()const{ return shared_ptr( new SquaredLinearVBV< inT, outT >( *this ) ); }
		virtual outT GetValue( const inT& value )const{ return startValue + multValue * value * value; }
		//ValueByValue interface
	};
	//////////////////////////////////////////////////////////////
	//------------------------------------------------------------
	template< class inT, class outT >
	class CubedLinearVBV : public LinearVBV< inT, outT >
	{
	public:
		virtual ~CubedLinearVBV()
		{
		}
		//ValueByValue interface
		virtual shared_ptr CreateCopyPtr()const{ return shared_ptr( new CubedLinearVBV< inT, outT >( *this ) ); }
		virtual outT GetValue( const inT& value )const{ return startValue + multValue * value * value * value; }
		//ValueByValue interface
	};
	//////////////////////////////////////////////////////////////
	//------------------------------------------------------------
	template< class inT, class outT >
	class _DemoExport TableVBV : public ValueByValue< inT, outT >
	{
	public:
		TableVBV()
			: ValueByValue< inT, outT >( VBV_TABLE )
			, startValue(0)
			, endValue(0)
			, incValue(0)
			, outValuesCount(0)
		{
		}
		TableVBV( const TableVBV< inT, outT >& vbv )
			: ValueByValue< inT, outT >( VBV_TABLE )
			, startValue(0)
			, endValue(0)
			, incValue(0)
			, outValuesCount(0)
		{
			*this = vbv;
		}
		TableVBV< inT, outT >& operator=( const TableVBV< inT, outT >& vbv )
		{
			if( this == &vbv )
				return *this;
			startValue = vbv.startValue;
			endValue = vbv.endValue;
			incValue = vbv.incValue;
			outValues = vbv.outValues;
			outValuesCount = vbv.outValuesCount;
			name = vbv.name;
			return *this;
		}
		virtual ~TableVBV()
		{
		}
		//ValueByValue interface
		virtual shared_ptr CreateCopyPtr()const = 0;
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
		virtual System::dsVBV CreateDotSceneVBV()const {
			System::dsVBV rsd;
			rsd.name = name;
			rsd.startValue = startValue;
			rsd.incValue = incValue;

			std::copy(outValues.begin(), outValues.end(), std::back_inserter(rsd.entries));

			return rsd;
		}
#endif
		virtual void Close() {
			startValue = 0;
			incValue = 0;
			endValue = 0;
			outValuesCount = 0;
			outValues.clear();
			_isInitialized = false;
		}
		virtual void MultiplyVBV( const inT& in, const outT& out ) {
			startValue *= in;
			incValue *= in;
			endValue = startValue + incValue * inT( outValuesCount - 1 );

			std::for_each(outValues.begin(), outValues.end(), [out](outT& val) { val *= out; } );
		}
		virtual outT GetValue( const inT& value )const = 0;
		//ValueByValue interface
#if DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS
		bool Initialize( const System::dsVBV& data ) {
			startValue = data.startValue;
			incValue = data.incValue;

			std::copy(data.entries.begin(), data.entries,end(), std::back_inserter(outValues));

			outValuesCount = outValues.size();
			endValue = startValue + incValue * inT( outValuesCount - 1 );
			if( outValuesCount < 2 )return false;
			name = data.name;
			if( name.empty() )return false;
			return _isInitialized = true;
		}
#endif
		inline inT getStartValue()const{ return startValue; }
		inline inT getIncValue()const{ return incValue; }
		inline inT getEndValue()const{ return endValue; }
		inline const std::vector< outT >& getOutValues()const{ return outValues; }

	protected:
		inT startValue;
		inT endValue;
		inT incValue;
		typename Demo::vector< outT >::type outValues;
		typename Demo::vector< outT >::type::size_type outValuesCount;
	};
	//////////////////////////////////////////////////////////////
	//------------------------------------------------------------
	template< class inT, class outT >
	class _DemoExport InterTableVBV : public TableVBV< inT, outT >
	{
	public:
		virtual ~InterTableVBV()
		{
		}
		//ValueByValue interface
		virtual shared_ptr CreateCopyPtr()const{ return shared_ptr( new InterTableVBV< inT, outT >( *this ) ); }
		virtual outT GetValue( const inT& value )const
		{
			int element(0);
			outT valueDiff(0);
			float position = ( value - startValue ) / incValue;

			if( value < startValue ) { 
				valueDiff = outValues[1] - outValues[0];
			} else if( value > endValue ) {
				valueDiff = outValues[ outValuesCount - 1 ] - outValues[ outValuesCount - 2 ];
				element = outValuesCount - 1;
			} else {
				element = int( position );
				valueDiff = outValues[ element + 1 ] - outValues[ element ];
			}

			return outValues[ element ] + valueDiff * (position - element);
		}
		//ValueByValue interface
	};
	//////////////////////////////////////////////////////////////
	//------------------------------------------------------------
	template< class inT, class outT >
	class _DemoExport FloorTableVBV : public TableVBV< inT, outT >
	{
	public:
		virtual ~FloorTableVBV()
		{
		}
		virtual shared_ptr CreateCopyPtr()const{ return shared_ptr( new FloorTableVBV< inT, outT >( *this ) ); }
		virtual outT GetValue( const inT& value )const
		{
			if( value < startValue ) {
				return outValues[ 0 ];
			} else if( value > endValue ) {
				return outValues[ outValuesCount - 1 ];
			} else {
				return outValues[ int( ( value - startValue ) / incValue ) ];
			}
		}
	};
	//------------------------------------------------------------
}
}
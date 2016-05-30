#pragma once

namespace Demo
{
namespace Core
{
	//------------------------------------------------------------
	template< class T >
	class RangeVar
	{
	public:
		//--------------------------------//
		static void SetToRange( const T& minValue, const T& maxValue, T& value )
		{
			if( value < minValue )
				value = minValue;
			else if( value > maxValue )
				value = maxValue;
		}
		//--------------------------------//
		explicit RangeVar( const T& min, const T& max )
			: minVal( min )
			, maxVal( max )
			, var( min )
		{
		}
		explicit RangeVar( const T& min, const T& max, const T& value )
			: minVal( min )
			, maxVal( max )
			, var( value )
		{
			SetToRange( minVal, maxVal, var );
		}
		virtual ~RangeVar()
		{
		}

		inline T get()const{ return var; }
	protected:
		inline void CheckRange(){ SetToRange( minVal, maxVal, var ); }
		T minVal;
		T maxVal;
		T var;
	};
	//------------------------------------------------------------
	class puFloat
	{
	public:
		//--------------------------------//
		static const float minVal;
		static const float maxVal;
		//--------------------------------//
		explicit puFloat()
			: var( 0.0f )
		{
		}
		explicit puFloat( const float& value )
			: var( value )
		{
			CheckRange();
		}
		virtual ~puFloat()
		{
		}

		puFloat& operator=( const float& value )
		{
			var = value;
			CheckRange();
			return *this;
		}
		puFloat operator*( const puFloat& rv )const
		{
			puFloat value;
			value.var = this->var * rv.var;
			return value;
		}
		inline const float& get()const{ return var; }

	protected:
		inline void CheckRange(){ return RangeVar< float >::SetToRange( 0.0f, 1.0f, var ); }
		float var;
	};
	//------------------------------------------------------------
	class npuFloat
	{
	public:
		//--------------------------------//
		static const float minVal;
		static const float maxVal;
		//--------------------------------//
		explicit npuFloat()
			: var(0.0f)
		{
		}
		explicit npuFloat( const float& value )
			: var( value )
		{
			CheckRange();
		}
		virtual ~npuFloat()
		{
		}

		/*
		Оператор присваивания для float
		Приводит переданное значение к диапазону npuFloat
		*/
		npuFloat& operator=( const float& value )
		{
			var = value;
			CheckRange();
			return *this;
		}
		npuFloat operator*( const npuFloat& rv )const
		{
			npuFloat value;
			value.var = this->var * rv.var;
			return value;
		}
		inline const float& get()const{ return var; }

	protected:
		inline void CheckRange(){ return RangeVar< float >::SetToRange( -1.0f, 1.0f, var ); }
		float var;
	};
	//------------------------------------------------------------
}//namespace Core
}//namespace Demo
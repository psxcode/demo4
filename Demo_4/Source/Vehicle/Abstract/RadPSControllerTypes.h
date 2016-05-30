#pragma once
#include "Core\DelTimer.h"
#include "DriveStyleCalculator.h"

namespace Demo
{
namespace Vehicle
{
	class BaseRadPSEntry
	{
	public:
		DEFINE_CLASS_SHARED_PTR(BaseRadPSEntry)
		//--------------------------------//
		typedef delegate_traits::composite0::traits::del_in del_in;
		//--------------------------------//
		virtual ~BaseRadPSEntry()
		{
		}
		virtual void Operate( float radPS ) = 0;
		virtual bool IsEmpty()const{ return belowTDA.empty() && aboveTDA.empty() && belowODA.empty() && aboveODA.empty(); }
		
		virtual void MoveBelow( bool autoCall = true ){ _isBelow = true; _isAbove = false; if( autoCall && _isActive )callBelow(); }
		virtual void MoveAbove( bool autoCall = true ){ _isAbove = true; _isBelow = false; if( autoCall && _isActive )callAbove(); }
		virtual void RemoveDelegate( del_in  del ){ removeAboveOnceDel(del); removeAboveTempDel(del); removeBelowOnceDel(del); removeBelowTempDel(del); }
		virtual void ReCallDelegates(){ if( _isBelow )callBelow(); if( _isAbove )callAbove(); }
		
		inline void addAboveTempDel( del_in  del ){ _isAbove ? (*del)() : aboveTDA.add( del ); }
		inline void addBelowTempDel( del_in  del ){ _isBelow ? (*del)() : belowTDA.add( del ); }
		inline void addAboveOnceDel( del_in  del ){ if( _isAbove )(*del)(); aboveODA.add( del ); }
		inline void addBelowOnceDel( del_in  del ){ if( _isBelow )(*del)(); belowODA.add( del ); }
		
		inline void removeAboveTempDel( del_in  del ){ aboveTDA.remove( del ); }
		inline void removeBelowTempDel( del_in  del ){ belowTDA.remove( del ); }
		inline void removeAboveOnceDel( del_in  del ){ aboveODA.remove( del ); }
		inline void removeBelowOnceDel( del_in  del ){ belowODA.remove( del ); }
		
		void callBelow(){ belowODA(); belowTDA(); belowTDA.clear(); }
		void callAbove(){ aboveODA(); aboveTDA(); aboveTDA.clear(); }
		void setActive( bool active = true ){ _isActive = active; }
		inline bool isBelow()const{ return _isBelow; }
		inline bool isAbove()const{ return _isAbove; }
		inline bool isActive()const{ return _isActive; }

	protected:
		BaseRadPSEntry()
			: _isBelow( false )
			, _isAbove( false )
			, _isActive( true )
		{
		}
		//DA - DelArray
		//T - Temp
		//O - Once
		//C - Continious
		delegate_traits::composite0 belowTDA;
		delegate_traits::composite0 aboveTDA;
		delegate_traits::composite0 belowODA;
		delegate_traits::composite0 aboveODA;

		bool _isBelow;
		bool _isAbove;
		bool _isActive;

	private:
		BaseRadPSEntry( const BaseRadPSEntry& );
		BaseRadPSEntry& operator=( const BaseRadPSEntry& );
	};
	//------------------------------------------------------------
	class BaseRadPSMark : public BaseRadPSEntry
	{
	public:
		DEFINE_CLASS_SHARED_PTR(BaseRadPSMark)
		//--------------------------------//
		virtual ~BaseRadPSMark()
		{
		}
		virtual float GetMark()const = 0;
		virtual void Operate( float radPS );
		
	protected:
		BaseRadPSMark()
		{
		}

	private:
		BaseRadPSMark( const BaseRadPSMark& );
		BaseRadPSMark& operator=( const BaseRadPSMark& );
	};
	//------------------------------------------------------------
	class BaseRadPSZone : public BaseRadPSEntry
	{
	public:
		DEFINE_CLASS_SHARED_PTR(BaseRadPSZone)
		//--------------------------------//
		virtual ~BaseRadPSZone()
		{
		}
		virtual float GetLow()const = 0;
		virtual float GetHigh()const = 0;
		virtual Core::puFloat GetRangePos()const = 0;
		virtual void Operate( float radPS );
		virtual bool IsEmpty()const{ return normTDA.empty() && normODA.empty() && BaseRadPSEntry::IsEmpty(); }
		virtual void MoveAbove( bool autoCall = true ){ _isNorm = false; BaseRadPSEntry::MoveAbove( autoCall ); }
		virtual void MoveBelow( bool autoCall = true ){ _isNorm = false; BaseRadPSEntry::MoveBelow( autoCall ); }
		virtual void MoveNorm( bool autoCall = true ){ _isNorm = true; _isBelow = false; _isAbove = false; if( autoCall && _isActive )callNorm(); }
		virtual void RemoveDelegate( del_in  del ){ removeNormOnceDel(del); removeNormTempDel(del); BaseRadPSEntry::RemoveDelegate( del ); }
		virtual void ReCallDelegates(){ if( _isNorm )callNorm(); BaseRadPSEntry::ReCallDelegates(); }
				
		inline void addNormTempDel( del_in  del ){ _isNorm ? (*del)() : normTDA.add( del ); }
		inline void addNormOnceDel( del_in  del ){ if( _isNorm )(*del)(); normODA.add( del ); }
		
		inline void removeNormTempDel( del_in  del ){ normTDA.remove( del ); }
		inline void removeNormOnceDel( del_in  del ){ normODA.remove( del ); }
		
		void callNorm(){ normODA(); normTDA(); normTDA.clear(); }
		inline bool isNorm()const{ return _isNorm; }

	protected:
		BaseRadPSZone()
			: _isNorm( false )
			, lastRadPS(0)
		{
		}

		//DA - DelArray
		//T - Temp
		//O - Once
		//C - Continious
		delegate_traits::composite0 normTDA;
		delegate_traits::composite0 normODA;

		float lastRadPS;
		
		bool _isNorm;

	private:
		BaseRadPSZone( const BaseRadPSZone& );
		BaseRadPSZone& operator=( const BaseRadPSZone& );
	};
	//------------------------------------------------------------
	class ConstantRadPSMark : public BaseRadPSMark
	{
	public:
		ConstantRadPSMark()
			: mark(0)
		{
		}
		virtual ~ConstantRadPSMark()
		{
		}
		virtual float GetMark()const{ return mark; }

		bool Initialize( float markValue );

	protected:
		float mark;
	};
	//------------------------------------------------------------
	class ConstantRadPSZone : public BaseRadPSZone
	{
	public:
		//--------------------------------//
		typedef std::pair< float, float > ConstantZone;
		//--------------------------------//
		ConstantRadPSZone()
			: range( 0.0f )
			, zone( 0.0f, 0.0f )
		{
		}
		virtual ~ConstantRadPSZone()
		{
		}
		virtual float GetLow()const{ return zone.first; }
		virtual float GetHigh()const{ return zone.second; }
		virtual Core::puFloat GetRangePos()const{ return Core::puFloat( (lastRadPS - zone.first) / range ); }
		
		bool Initialize( const ConstantZone& constZone );

	protected:
		ConstantZone zone;
		float range;
	};
	//------------------------------------------------------------
	class LinearRadPSMark : public BaseRadPSMark, public DriveStyleCalculator
	{
	public:
		//--------------------------------//
		typedef std::pair< float, float > LinearMark;
		//--------------------------------//
		struct LinMarkInitData : DriveStyleCalculator::InitData
		{
			virtual bool isValid()const{ return InitData::isValid() && mark.first < mark.second; }
			LinearMark mark;
		};
		//--------------------------------//
		LinearRadPSMark()
		{
		}
		virtual ~LinearRadPSMark()
		{
		}
		virtual float GetMark()const{ return Calculate( mark.first, mark.second ); }
		
		bool Initialize( const LinMarkInitData& idata );

	protected:
		LinearMark mark;
	};
	//------------------------------------------------------------
	class LinearRadPSZone : public BaseRadPSZone, public DriveStyleCalculator
	{
	public:
		//--------------------------------//
		struct LinearZone
		{
			std::pair< float, float > low;
			std::pair< float, float > high;
		};
		//--------------------------------//
		struct LinZoneInitData : DriveStyleCalculator::InitData
		{
			virtual bool isValid()const{ return InitData::isValid(); }
			LinearZone zone;
		};
		//--------------------------------//
		virtual float GetLow()const{ return Calculate( zone.low.first, zone.low.second ); }
		virtual float GetHigh()const{ return Calculate( zone.high.first, zone.high.second ); }
		virtual Core::puFloat GetRangePos()const{ float lowVal = GetLow(); return Core::puFloat( (lastRadPS - lowVal) / (GetHigh() - lowVal) ); }
		
		bool Initialize( const LinZoneInitData& idata );

	protected:
		LinearZone zone;
	};
	//------------------------------------------------------------
	class ExtFeedRadPSMark : public BaseRadPSMark
	{
	public:
		//--------------------------------//
		typedef Ogre::SharedPtr< float > ExtMark;
		//--------------------------------//
		virtual ~ExtFeedRadPSMark()
		{
		}
		virtual float GetMark()const{ return *extMark; }
		
		ExtMark extMark;
	};
	//------------------------------------------------------------
	class ExtFeedRadPSZone : public BaseRadPSZone
	{
	public:
		//--------------------------------//
		typedef std::pair< Ogre::SharedPtr< float >, Ogre::SharedPtr< float > > ExtZone;
		//--------------------------------//
		virtual ~ExtFeedRadPSZone()
		{
		}
		virtual float GetLow()const{ return *(extZone.first); }
		virtual float GetHigh()const{ return *(extZone.second); }
		virtual Core::puFloat GetRangePos()const{ float lowVal = GetLow(); return Core::puFloat( (lastRadPS - lowVal) / (GetHigh() - lowVal) ); }
//		virtual void Operate( float radPS );

		ExtZone extZone;
	};
	//------------------------------------------------------------
}
}
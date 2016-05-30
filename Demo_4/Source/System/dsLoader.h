#pragma once
#include <OgreIteratorWrapper.h>
#include <OgreString.h>
#include <OgreStringConverter.h>

#include "tinyxml/tinyxml.h"

namespace Ogre
{
	class Vector3;
	class Quaternion;
	class ColourValue;
}

namespace Demo
{
namespace System
{
//------------------------------------------------------------
static const TiXmlElement* dsIterateChildElements( const TiXmlElement* &element, const TiXmlElement* &childElement ) {
	if( element ) {
		if( childElement == 0 ) childElement = element->FirstChildElement();
		else childElement = childElement->NextSiblingElement();
		return childElement;
	} else return 0;
}
//------------------------------------------------------------
template<bool CaseSensitive>
static size_t dsGetElementCount( const TiXmlElement *element, string_traits::in elementName ) {
	size_t count = 0;
	if( string_equals<CaseSensitive>()(elementName, element->ValueStr()) ) ++count;
	const TiXmlElement* childElement = 0;
	while( childElement = dsIterateChildElements( element, childElement ) )
		count += dsGetElementCount( childElement, elementName );
	return count;
}
//------------------------------------------------------------
template<bool CaseSensitive>
static size_t dsGetChildElementCount( const TiXmlElement *element, string_traits::in elementName ) {
	size_t count = 0;
	const TiXmlElement *childElement = 0;
	while( childElement = dsIterateChildElements( element, childElement ) )
		if( string_equals<CaseSensitive>()(elementName, childElement->ValueStr()) ) ++count;
	return count;
}
//------------------------------------------------------------
static void dsGetChildText( const TiXmlElement *element, string_traits::inout text ) {
	const TiXmlNode* childNode = element->FirstChild();
	while( childNode ) {
		if( childNode->Type() == TiXmlNode::TEXT ) {
			const TiXmlText* textNode = childNode->ToText();
			if( textNode ) {
				text = textNode->ValueStr();
				return;
			}
		}
		childNode = element->NextSibling();
	}
}
//------------------------------------------------------------
template<bool CaseSensitive>
static string_traits::outcopy dsGetString( const TiXmlElement *element, string_traits::in elementName, string_traits::in defaultValue = Ogre::StringUtil::BLANK ) {
	string_traits::outptr value = element->Attribute<CaseSensitive>(elementName);
	return value == 0 ? defaultValue : *value;
}
//------------------------------------------------------------
template<bool CaseSensitive>
static Ogre::Real dsGetReal( const TiXmlElement *element, string_traits::in elementName, Ogre::Real defaultValue = 0 ) {
	string_traits::outref value = dsGetString<CaseSensitive>(element, elementName);
	return value.empty() ? defaultValue : Ogre::StringConverter::parseReal(value);
}
//------------------------------------------------------------
template<bool CaseSensitive>
static int dsGetInt( const TiXmlElement *element, string_traits::in elementName, int defaultValue = 0 ) {
	string_traits::outref value = dsGetString<CaseSensitive>( element, elementName );
	return value.empty() ? defaultValue : Ogre::StringConverter::parseInt( value );
}
//------------------------------------------------------------
template<bool CaseSensitive>
static unsigned int dsGetUInt( const TiXmlElement *element, string_traits::in elementName, unsigned int defaultValue = 0 ) {
	string_traits::outref value = dsGetString<CaseSensitive>( element, elementName );
	return value.empty() ? defaultValue : Ogre::StringConverter::parseUnsignedInt( value );
}
//------------------------------------------------------------
template<bool CaseSensitive>
static bool dsGetBool( const TiXmlElement *element, string_traits::in elementName, bool defaultValue = false ) {
	string_traits::outref value = dsGetString<CaseSensitive>( element, elementName );
	return value.empty() ? defaultValue : Ogre::StringConverter::parseBool( value, defaultValue );
}
//------------------------------------------------------------
template<bool CaseSensitive>
static void dsGetVector3( const TiXmlElement *element, Ogre::Vector3 &outVal, const Ogre::Vector3 &defaultVal ) {
	outVal.x = dsGetReal<CaseSensitive>( element, "x", defaultVal.x );
	outVal.y = dsGetReal<CaseSensitive>( element, "y", defaultVal.y );
	outVal.z = dsGetReal<CaseSensitive>( element, "z", defaultVal.z );
}
//------------------------------------------------------------
template<bool CaseSensitive>
static void dsGetQuaternion( const TiXmlElement *element, Ogre::Quaternion &outQuat, const Ogre::Quaternion &defaultVal ) {
	if( element->Attribute<CaseSensitive>( "qx" ) )
	{
		//The rotation is specified as a quaternion
		outQuat.x = dsGetReal<CaseSensitive>( element, "qx", defaultVal.x );
		outQuat.y = dsGetReal<CaseSensitive>( element, "qy", defaultVal.y );
		outQuat.z = dsGetReal<CaseSensitive>( element, "qz", defaultVal.z );
		outQuat.w = dsGetReal<CaseSensitive>( element, "qw", defaultVal.w );
	}
	else if( element->Attribute<CaseSensitive>( "axisX" ) )
	{
		//The rotation is specified as an axis and angle
		Ogre::Real angle = dsGetReal<CaseSensitive>( element, "angle", defaultVal.w );
		Ogre::Vector3 vector3( 
			dsGetReal<CaseSensitive>( element, "axisX", defaultVal.x ),
			dsGetReal<CaseSensitive>( element, "axisY", defaultVal.y ),
			dsGetReal<CaseSensitive>( element, "axisZ", defaultVal.z ) );
		outQuat.FromAngleAxis( Ogre::Radian( angle ), vector3 );
	}
	else if( element->Attribute<CaseSensitive>( "angle" ) )
	{
		//Assume the rotation is specified as three Euler angles
		Ogre::Vector3 euler(
			dsGetReal<CaseSensitive>( element, "angleX", 0.0f ),
			dsGetReal<CaseSensitive>( element, "angleY", 0.0f ),
			dsGetReal<CaseSensitive>( element, "angleZ", 0.0f ) );
		string_traits::str order = dsGetString<CaseSensitive>( element, "order" );

		Ogre::Matrix3 rotMatrix;
		if( order.length() < 2 )
			rotMatrix.FromEulerAnglesXYZ( Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z) );
		else {
			if( string_equals<CaseSensitive>()(order.substr(0,1), "x") )
			{
				if( string_equals<CaseSensitive>()(order.substr(1,1), "y") )
					rotMatrix.FromEulerAnglesXYZ( Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z) );
				else
					rotMatrix.FromEulerAnglesXZY( Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z) );
			}
			else if( string_equals<CaseSensitive>()(order.substr(0,1), "y") )
			{
				if( string_equals<CaseSensitive>()(order.substr(1,1), "x") )
					rotMatrix.FromEulerAnglesYXZ( Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z) );
				else
					rotMatrix.FromEulerAnglesYZX( Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z) );
			}
			else
			{
				if( string_equals<CaseSensitive>()(order.substr(1,1), "x") )
					rotMatrix.FromEulerAnglesZXY( Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z) );
				else
					rotMatrix.FromEulerAnglesZYX( Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z) );
			}
		}
		outQuat.FromRotationMatrix( rotMatrix );
	} 
}
//------------------------------------------------------------
template<bool CaseSensitive>
static void dsGetColour( const TiXmlElement *element, Ogre::ColourValue &outVal, const Ogre::ColourValue &defaultVal ) {
	outVal.r = dsGetReal<CaseSensitive>( element, "r", defaultVal.r );
	outVal.g = dsGetReal<CaseSensitive>( element, "g", defaultVal.g );
	outVal.b = dsGetReal<CaseSensitive>( element, "b", defaultVal.b );
	outVal.a = dsGetReal<CaseSensitive>( element, "a", defaultVal.a );
}
//------------------------------------------------------------
class dsLoader_BaseInterface
{
public:
	//--------------------------------//
	DEFINE_CLASS_SHARED_PTR(dsLoader_BaseInterface)
	//--------------------------------//
	class traits
	{
	public:
		//ProcessElementNames
		typedef Demo::set< type_traits<string_traits::str>::cdata_ptr >::type pen_cont;
		typedef shared_ptr_traits< const pen_cont >::type pen_cont_out;
	};
	//--------------------------------//
	virtual ~dsLoader_BaseInterface() {}

	virtual traits::pen_cont_out getProcessElementNames()const = 0;
};
//------------------------------------------------------------
template< typename T >
class dsLoaderManual_ProcessInterface : public dsLoader_BaseInterface
{
public:
	//--------------------------------//
	DEFINE_TCLASS_SHARED_PTR(dsLoaderManual_ProcessInterface)
	//--------------------------------//
	class traits : public dsLoader_BaseInterface::traits
	{
	public:
		typedef typename type_traits<T>::ref process_inout;
	};
	//--------------------------------//
	virtual ~dsLoaderManual_ProcessInterface() {}

	virtual bool processElement( const TiXmlElement *element, typename traits::process_inout ) = 0;
};
//------------------------------------------------------------
class dsLoaderAuto_ProcessInterface : public dsLoader_BaseInterface
{
public:
	//--------------------------------//
	DEFINE_CLASS_SHARED_PTR(dsLoaderAuto_ProcessInterface)
	//--------------------------------//
	virtual ~dsLoaderAuto_ProcessInterface() {}

	virtual bool processElement( const TiXmlElement *element ) = 0;
};
//------------------------------------------------------------
class dsLoaderAuto_CompositeInterface : public dsLoaderAuto_ProcessInterface
{
public:
	//--------------------------------//
	DEFINE_CLASS_SHARED_PTR(dsLoaderAuto_CompositeInterface)
	//--------------------------------//
	class traits : public dsLoaderAuto_ProcessInterface::traits
	{
	public:
		typedef dsLoaderAuto_ProcessInterface::shared_ptr child_ldr_t;
		typedef type_traits< child_ldr_t >::cref child_ldr_in;
	};
	//--------------------------------//
	virtual ~dsLoaderAuto_CompositeInterface() {}

	virtual void addChildLoader( traits::child_ldr_in ldr ) = 0;
	virtual void removeChildLoader( traits::child_ldr_in ldr ) = 0;
};
//------------------------------------------------------------
template< typename T >
class dsLoaderManual_CompositeInterface : public dsLoaderManual_ProcessInterface<T>
{
public:
	//--------------------------------//
	DEFINE_TCLASS_SHARED_PTR(dsLoaderManual_CompositeInterface)
	//--------------------------------//
	class traits : public dsLoaderManual_ProcessInterface<T>::traits
	{
	public:
		typedef typename dsLoaderManual_ProcessInterface<T>::shared_ptr child_ldr_t;
		typedef typename type_traits< child_ldr_t >::cref child_ldr_in;
	};
	//--------------------------------//
	virtual ~dsLoaderManual_CompositeInterface() {}

	virtual void addChildLoader( typename traits::child_ldr_in ldr ) = 0;
	virtual void removeChildLoader( typename traits::child_ldr_in ldr ) = 0;
};
//------------------------------------------------------------
class dsLoaderAuto_CompositeSimple : public dsLoaderAuto_CompositeInterface
{
public:
	DEFINE_CLASS_SHARED_PTR(dsLoaderAuto_CompositeSimple)
	//--------------------------------//
	class traits : public dsLoaderAuto_CompositeInterface::traits
	{
	public:
		typedef Demo::set< child_ldr_t >::type child_ldr_cont;
		typedef Ogre::SetIterator< child_ldr_cont > child_ldr_oit;
		typedef Ogre::ConstSetIterator< child_ldr_cont > child_ldr_ocit;
	};
	//--------------------------------//
	dsLoaderAuto_CompositeSimple();
	virtual ~dsLoaderAuto_CompositeSimple();

	//dsLoader_BaseInterface
	virtual bool processElement( const TiXmlElement *element );
	virtual traits::pen_cont_out getProcessElementNames()const;

	//dsLoaderAuto_CompositeInterface
	virtual void addChildLoader( traits::child_ldr_in ldr ) { childLoaders.insert(ldr); }
	virtual void removeChildLoader( traits::child_ldr_in ldr ) { childLoaders.erase(ldr); }

protected:
	traits::child_ldr_cont childLoaders;
};
//------------------------------------------------------------
class dsLoaderAuto_CompositeDispatcher : public dsLoaderAuto_CompositeInterface
{
public:
	DEFINE_CLASS_SHARED_PTR(dsLoaderAuto_CompositeDispatcher)
	//--------------------------------//
	class traits : public dsLoaderAuto_CompositeInterface::traits
	{
	public:
		typedef Demo::map< string_traits::str, dsLoaderAuto_CompositeInterface::shared_ptr >::type child_ldr_cont;
		typedef Ogre::MapIterator< child_ldr_cont > child_ldr_oit;
		typedef Ogre::ConstMapIterator< child_ldr_cont > child_ldr_ocit;
	};
	//--------------------------------//
	dsLoaderAuto_CompositeDispatcher();
	virtual ~dsLoaderAuto_CompositeDispatcher();

	//dsLoader_BaseInterface
	virtual bool processElement( const TiXmlElement *element );
	virtual traits::pen_cont_out getProcessElementNames()const;

	//dsLoaderAuto_CompositeInterface
	virtual void addChildLoader( traits::child_ldr_in ldr );
	virtual void removeChildLoader( traits::child_ldr_in ldr );

protected:
	dsLoaderAuto_CompositeInterface::shared_ptr findCreateLoaderComposite( string_traits::in nodeName );

	traits::child_ldr_cont childLoaders;
};
//------------------------------------------------------------
}
}
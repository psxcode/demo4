#pragma once

#include <OgreStringConverter.h>
#include "dsSceneTypes.h"
#include "dsLoader.h"

namespace Demo
{
namespace System
{
//------------------------------------------------------------
static dsMovableObject::OBJECT_VISIBILITY ParseObjectVisibility( string_traits::in ovStr )
{
	if( ovStr.empty() ) return dsMovableObject::OV_DEFAULT;
	return Ogre::StringConverter::parseBool( ovStr, true ) ? dsMovableObject::OV_VISIBLE : dsMovableObject::OV_HIDDEN;
}
//------------------------------------------------------------
template<bool CaseSensitive>
static dsSceneNode::NODE_VISIBILITY ParseNodeVisibility( string_traits::in nvStr )
{
	typedef dsKeys::Node NK;
	if( string_equals<CaseSensitive>()(nvStr, NK::NODE_VISIBLE) ) return dsSceneNode::NV_VISIBLE;
	if( string_equals<CaseSensitive>()(nvStr, NK::NODE_HIDDEN) ) return dsSceneNode::NV_HIDDEN;
	if( string_equals<CaseSensitive>()(nvStr, NK::NODE_TREE_VISIBLE) ) return dsSceneNode::NV_TREE_VISIBLE;
	if( string_equals<CaseSensitive>()(nvStr, NK::NODE_TREE_HIDDEN) ) return dsSceneNode::NV_TREE_HIDDEN;
	return dsSceneNode::NV_DEFAULT;
}
//------------------------------------------------------------
template<bool CaseSensitive>
static Ogre::Light::LightTypes ParseLightType( string_traits::in typeStr, Ogre::Light::LightTypes defaultType )
{
	typedef dsKeys::Light LK;
	if( string_equals<CaseSensitive>()(typeStr, LK::TYPE_POINT) ) return Ogre::Light::LT_POINT;
	if( string_equals<CaseSensitive>()(typeStr, LK::TYPE_SPOT) ) return Ogre::Light::LT_SPOTLIGHT;
	if( string_equals<CaseSensitive>()(typeStr, LK::TYPE_DIRECT) ) return Ogre::Light::LT_DIRECTIONAL;
	return defaultType;
}
//------------------------------------------------------------
template<bool CaseSensitive>
static Ogre::ProjectionType ParseProjectionType( string_traits::in projType, Ogre::ProjectionType defaultValue )
{
	typedef dsKeys::Camera CK;
	if( string_equals<CaseSensitive>()(projType, CK::TYPE_PERSPECTIVE) ) return Ogre::PT_PERSPECTIVE;
	if( string_equals<CaseSensitive>()(projType, CK::TYPE_ORTHOGRAPHIC) ) return Ogre::PT_ORTHOGRAPHIC;
	return defaultValue;
}
//------------------------------------------------------------
template<bool CaseSensitive>
static Ogre::FogMode ParseFogMode( string_traits::in fogModeStr, Ogre::FogMode defaultMode )
{
	typedef dsKeys::Fog FK;
	if( string_equals<CaseSensitive>()(fogModeStr, FK::MODE_LINEAR) ) return Ogre::FOG_LINEAR;
	if( string_equals<CaseSensitive>()(fogModeStr, FK::MODE_EXP) ) return Ogre::FOG_EXP;
	if( string_equals<CaseSensitive>()(fogModeStr, FK::MODE_EXP2) ) return Ogre::FOG_EXP2;
	return defaultMode;
}
//------------------------------------------------------------
template<bool CaseSensitive>
static dsMovableObject::OBJECT_TYPE ParseObjectType( string_traits::in otStr )
{
	if( string_equals<CaseSensitive>()(otStr, dsKeys::SceneCommon::ENTITY_ELEMENT)) return dsMovableObject::OT_ENTITY;
	if( string_equals<CaseSensitive>()(otStr, dsKeys::SceneCommon::LIGHT_ELEMENT)) return dsMovableObject::OT_LIGHT;
	if( string_equals<CaseSensitive>()(otStr, dsKeys::SceneCommon::CAMERA_ELEMENT)) return dsMovableObject::OT_CAMERA;
	return dsMovableObject::OT_NONE;
}
//------------------------------------------------------------
template<bool CaseSensitive>
static Ogre::ShadowTechnique ParseShadowTechnique( string_traits::in stStr )
{
	typedef dsKeys::Shadow SK;
	if( string_equals<CaseSensitive>()(stStr, SK::TECH_STENCIL_ADDITIVE) ) return Ogre::SHADOWTYPE_STENCIL_ADDITIVE;
	if( string_equals<CaseSensitive>()(stStr, SK::TECH_STENCIL_MODULATIVE) ) return Ogre::SHADOWTYPE_STENCIL_MODULATIVE;
	if( string_equals<CaseSensitive>()(stStr, SK::TECH_TEXTURE_ADDITIVE_INT) ) return Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED;
	if( string_equals<CaseSensitive>()(stStr, SK::TECH_TEXTURE_ADDITIVE) ) return Ogre::SHADOWTYPE_TEXTURE_ADDITIVE;
	if( string_equals<CaseSensitive>()(stStr, SK::TECH_TEXTURE_MODULATIVE_INT) ) return Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED;
	if( string_equals<CaseSensitive>()(stStr, SK::TECH_TEXTURE_MODULATIVE) )	return Ogre::SHADOWTYPE_TEXTURE_MODULATIVE;
	return Ogre::SHADOWTYPE_NONE;
}
//------------------------------------------------------------
template<bool CaseSensitive>
static Ogre::HardwareBuffer::Usage ParseHardwareBufferUsage( string_traits::in HWBUsageStr, Ogre::HardwareBuffer::Usage defaultUsage )
{
	typedef dsKeys::HardwareBufferUsage UK;
	if( string_equals<CaseSensitive>()(HWBUsageStr, UK::STATIC) ) return Ogre::HardwareBuffer::HBU_STATIC;
	if( string_equals<CaseSensitive>()(HWBUsageStr, UK::DYNAMIC) ) return Ogre::HardwareBuffer::HBU_DYNAMIC;
	if( string_equals<CaseSensitive>()(HWBUsageStr, UK::STATIC_WRITE_ONLY) ) return Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY;
	if( string_equals<CaseSensitive>()(HWBUsageStr, UK::DYNAMIC_WRITE_ONLY) ) return Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY;
	if( string_equals<CaseSensitive>()(HWBUsageStr, UK::WRITE_ONLY) ) return Ogre::HardwareBuffer::HBU_WRITE_ONLY;
	return defaultUsage;
}
//------------------------------------------------------------
template<bool CaseSensitive>
static void LoadSubEntity( const TiXmlElement *element, dsEntity &ent )
{
	typedef dsKeys::Entity EK;
	const TiXmlElement *childElement(0);
	while( dsIterateChildElements( element, childElement ) )	{
		string_traits::outref childName = childElement->ValueStr();
		if( string_equals<CaseSensitive>()(childName, EK::SUBENTITY_ELEMENT) )
			ent.subMatNames.push_back( dsGetString<CaseSensitive>( childElement, EK::SUB_MATERIAL_NAME ) );
	}
}
//------------------------------------------------------------
template<bool CaseSensitive>
static void LoadLodBias( const TiXmlElement *element, dsEntity::LodBias &lod )
{
	lod.factor = dsGetReal<CaseSensitive>(element, dsKeys::SceneCommon::BIAS, 1.0f);
	lod.min = dsGetUInt<CaseSensitive>(element, dsKeys::SceneCommon::MIN, 255);
	lod.max = dsGetUInt<CaseSensitive>(element, dsKeys::SceneCommon::MAX, 0);
}
//------------------------------------------------------------
template<bool CaseSensitive>
static void LoadLightRange( const TiXmlElement *element, dsLight &lt )
{
	typedef dsKeys::Light LK;
	if( lt.lightType == Ogre::Light::LT_SPOTLIGHT )
	{
		string_traits::str value;
		value = dsGetString<CaseSensitive>( element, LK::RANGE_INNER );
		if( !value.empty() )
			lt.spotlightInnerAngle = Ogre::Radian( Ogre::StringConverter::parseReal( value ) );

		value = dsGetString<CaseSensitive>( element, LK::RANGE_OUTER );
		if( !value.empty() )
			lt.spotlightOuterAngle = Ogre::Radian( Ogre::StringConverter::parseReal( value ) );

		value = dsGetString<CaseSensitive>( element, LK::RANGE_FALLOFF );
		if( !value.empty() )
			lt.spotlightFalloff = Ogre::StringConverter::parseReal( value );
	}
}
//------------------------------------------------------------
template<bool CaseSensitive>
static void LoadLightAttenuation( const TiXmlElement *element, dsLight &lt )
{
	typedef dsKeys::Light LK;
	string_traits::str value;

	value = dsGetString<CaseSensitive>( element, LK::ATTENUATION_CONSTANT );
	if( !value.empty() )
		lt.attenuationConstant = Ogre::StringConverter::parseReal( value );

	value = dsGetString<CaseSensitive>( element, LK::ATTENUATION_LINEAR );
	if( !value.empty() )
		lt.attenuationLinear = Ogre::StringConverter::parseReal( value );

	value = dsGetString<CaseSensitive>( element, LK::ATTENUATION_QUADRIC );
	if( !value.empty() )
		lt.attenuationQuadric = Ogre::StringConverter::parseReal( value );

	value = dsGetString<CaseSensitive>( element, LK::ATTENUATION_RANGE );
	if( !value.empty() )
		lt.attenuationRange = Ogre::StringConverter::parseReal( value );
}
//------------------------------------------------------------
template<bool CaseSensitive>
static void LoadLightGeom( const TiXmlElement *element, dsLight::LightGeom &lg )
{
	lg.meshName = dsGetString<CaseSensitive>(element, dsKeys::Entity::MESH_FILE);
	const TiXmlElement *childElement = element->FirstChildElement<CaseSensitive>(dsKeys::Entity::MESH_LOD_BIAS);
	if(childElement)
		LoadLodBias<CaseSensitive>(childElement, lg.lodBias);
}
//------------------------------------------------------------
template<bool CaseSensitive>
static void LoadClippingParams( const TiXmlElement *element, Ogre::Real& outNearClip, Ogre::Real& outFarClip )
{
	typedef dsKeys::Clipping CP;
	outNearClip = dsGetReal<CaseSensitive>( element, CP::NEAR_CLIP, outNearClip );
	outFarClip = dsGetReal<CaseSensitive>( element, CP::FAR_CLIP, outFarClip );
}
//------------------------------------------------------------
template<bool CaseSensitive>
static void LoadEntity( const TiXmlElement *element, dsEntity &ent )
{
	typedef dsKeys::Entity EK;
	ent.name = dsGetString<CaseSensitive>( element, dsKeys::SceneCommon::NAME );
	ent.meshFilename = dsGetString<CaseSensitive>( element, EK::MESH_FILE );
	ent.castShadows = dsGetBool<CaseSensitive>( element, EK::CAST_SHADOWS, true );
	//ent->renderDistance = XI::GetRealAttribute( xmlElement, EK::RENDER_DISTANCE_KEY );

	const TiXmlElement *childElement(0);
	while( childElement = dsIterateChildElements( element, childElement ) )
	{
		string_traits::outref elementName = childElement->ValueStr();
		if( string_equals<CaseSensitive>()(elementName, EK::SUBENTITIES_FIELD) )
		{
			LoadSubEntity<CaseSensitive>( childElement, ent );
		}
		else if( string_equals<CaseSensitive>()(elementName, EK::MESH_LOD_BIAS) )
		{
			LoadLodBias<CaseSensitive>(childElement, ent.meshLodBias);
		}
		else if( string_equals<CaseSensitive>()(elementName, EK::MATERIAL_LOD_BIAS) )
		{
			LoadLodBias<CaseSensitive>(childElement, ent.materialLodBias);
		}
	}
}
//------------------------------------------------------------
template<bool CaseSensitive>
static void LoadLight( const TiXmlElement *element, dsLight &lt )
{
	typedef dsKeys::Light LK;
	lt.name = dsGetString<CaseSensitive>( element, dsKeys::SceneCommon::NAME );
	string_traits::str visStr = dsGetString<CaseSensitive>( element, LK::VISIBLE );
	string_traits::str typeStr = dsGetString<CaseSensitive>( element, dsKeys::SceneCommon::TYPE );

	lt.visibility = ParseObjectVisibility( visStr );
	lt.lightType = ParseLightType<CaseSensitive>( typeStr, Ogre::Light::LT_POINT );
	lt.castShadows = dsGetBool<CaseSensitive>( element, LK::CAST_SHADOWS );
	lt.power = dsGetReal<CaseSensitive>( element, LK::POWER, 1.0f );

	const TiXmlElement* childElement(0);
	while( dsIterateChildElements( element, childElement ) )
	{
		string_traits::outref elementName = childElement->ValueStr();
		if( string_equals<CaseSensitive>()(elementName, LK::COLOUR_DIFFUSE) )
		{
			dsGetColour<CaseSensitive>( childElement, lt.diffuseColour, Ogre::ColourValue::White );
		}
		else if( string_equals<CaseSensitive>()(elementName, LK::COLOUR_SPECULAR) )
		{
			dsGetColour<CaseSensitive>( childElement, lt.specularColour, Ogre::ColourValue::White );
		}
		else if( string_equals<CaseSensitive>()(elementName, LK::RANGE) )
		{
			LoadLightRange<CaseSensitive>( childElement, lt );
		}
		else if( string_equals<CaseSensitive>()(elementName, LK::ATTENUATION) )
		{
			LoadLightAttenuation<CaseSensitive>( childElement, lt );
		}
		else if( string_equals<CaseSensitive>()(elementName, LK::POSITION) )
		{
			dsGetVector3<CaseSensitive>( childElement, lt.position, Ogre::Vector3::ZERO );
		}
		else if( string_equals<CaseSensitive>()(elementName, LK::NORMAL) )
		{
			dsGetVector3<CaseSensitive>( childElement, lt.direction, Ogre::Vector3::ZERO );
		}
		else if( string_equals<CaseSensitive>()(elementName, LK::LIGHT_GEOM) )
		{
			LoadLightGeom<CaseSensitive>(childElement, lt.lightGeom);
		}
	}
}
//------------------------------------------------------------
template<bool CaseSensitive>
static void LoadCamera( const TiXmlElement *element, dsCamera &cam )
{
	typedef dsKeys::Camera CK;
	cam.name = dsGetString<CaseSensitive>( element, dsKeys::SceneCommon::NAME );
	string_traits::str camVisStr = dsGetString<CaseSensitive>( element, CK::VISIBLE );
	string_traits::str camTypeStr =dsGetString<CaseSensitive>( element, dsKeys::SceneCommon::TYPE );

	cam.visibility = ParseObjectVisibility( camVisStr );
	cam.projectionType = ParseProjectionType<CaseSensitive>( camTypeStr, Ogre::PT_PERSPECTIVE );
	cam.fov = dsGetReal<CaseSensitive>( element, CK::FOV, Ogre::Math::PI/2 );
	cam.aspectRatio = dsGetReal<CaseSensitive>( element, CK::ASPECT, 1.33f );

	const TiXmlElement *childElement(0);
	while( dsIterateChildElements( element, childElement ) )
	{
		string_traits::outref elementName = childElement->ValueStr();
		if( string_equals<CaseSensitive>()(elementName, CK::CLIPPING) )
		{
			LoadClippingParams<CaseSensitive>( childElement, cam.nearClip, cam.farClip ); 
		}
		else if( string_equals<CaseSensitive>()(elementName, CK::POSITION) )
		{
			dsGetVector3<CaseSensitive>( childElement, cam.position, Ogre::Vector3::ZERO );
		}
		else if( string_equals<CaseSensitive>()(elementName, CK::ROTATION) )
		{
			dsGetQuaternion<CaseSensitive>( childElement, cam.orientation, Ogre::Quaternion::IDENTITY );
		}
		else if( string_equals<CaseSensitive>()(elementName, CK::NORMAL) )
		{
			dsGetVector3<CaseSensitive>( childElement, cam.direction, Ogre::Vector3::ZERO );
		}
	}
}
//------------------------------------------------------------
template<bool CaseSensitive>
void LoadSceneNode( const TiXmlElement *element, dsSceneNode &node )
{
	typedef dsKeys::Node NK;
	node.name = dsGetString<CaseSensitive>( element, dsKeys::SceneCommon::NAME );

	const TiXmlElement *childElement(0);
	while( dsIterateChildElements( element, childElement ) )
	{
		string_traits::outref elementName = childElement->ValueStr();
		if( string_equals<CaseSensitive>()(elementName, NK::POSITION) )
			dsGetVector3<CaseSensitive>( childElement, node.position, Ogre::Vector3::ZERO );
		else if( string_equals<CaseSensitive>()(elementName, NK::ROTATION) )
			dsGetQuaternion<CaseSensitive>( childElement, node.orientation, Ogre::Quaternion::IDENTITY );
		else if( string_equals<CaseSensitive>()(elementName, NK::SCALE) )
			dsGetVector3<CaseSensitive>( childElement, node.scale, Ogre::Vector3::ZERO );
		else if( string_equals<CaseSensitive>()(elementName, dsKeys::SceneCommon::OBJECT_ELEMENT) )
		{
			dsSceneNode::traits::obj_store_t obj;
			obj.first = dsGetString<CaseSensitive>(childElement, dsKeys::SceneCommon::NAME);
			obj.second = ParseObjectType<CaseSensitive>(dsGetString<CaseSensitive>(childElement, dsKeys::SceneCommon::TYPE));
			node.addObject( obj );
		}
		else if( string_equals<CaseSensitive>()(elementName, dsKeys::SceneCommon::NODE_ELEMENT) )
		{
			dsSceneNode::traits::sn_store_t newSN( new dsSceneNode );
			LoadSceneNode<CaseSensitive>( childElement, *newSN );
			node.addSceneNode( newSN );
		}
	}
}
//------------------------------------------------------------
template<bool CaseSensitive>
static void LoadMeshLod( const TiXmlElement *element, dsMeshLod &ml )
{
	ml.meshName.name = dsGetString<CaseSensitive>(element, dsKeys::SceneCommon::NAME);
	ml.meshName.group = dsGetString<CaseSensitive>(element, dsKeys::SceneCommon::GROUP);
	ml.lodStrategy = dsGetString<CaseSensitive>(element, dsKeys::SceneCommon::LOD_STATEGY);

	const TiXmlElement *childElement(0);
	while( dsIterateChildElements( element, childElement) )
	{
		string_traits::outref childName = childElement->ValueStr();
		if( string_equals<CaseSensitive>()(childName, dsKeys::SceneCommon::LOD) )
		{
			Ogre::uint32 value = dsGetUInt<CaseSensitive>(childElement, dsKeys::SceneCommon::VALUE);
			dsMeshLod::MeshName meshName;
			meshName.name = dsGetString<CaseSensitive>(childElement, dsKeys::SceneCommon::NAME);
			meshName.group = dsGetString<CaseSensitive>(childElement, dsKeys::SceneCommon::GROUP);
			ml.lods.insert( dsMeshLod::lod_cont::value_type(value, meshName) );
		}
	}
}
//------------------------------------------------------------
class dsEntityLoader : public dsLoaderManual_ProcessInterface< dsEntity >
{
public:
	DEFINE_CLASS_SHARED_PTR(dsEntityLoader)
	//--------------------------------//
	dsEntityLoader()
	{
	}
	virtual ~dsEntityLoader()
	{
	}

	//dsLoaderManual_ProcessInterface
	virtual bool processElement( const TiXmlElement *element, traits::process_inout ent ) {
		LoadEntity<false>( element, ent );
		return true;
	}
	virtual traits::pen_cont_out getProcessElementNames()const {
		shared_ptr_traits< traits::pen_cont >::type pnn( new traits::pen_cont );
		pnn->insert( &dsKeys::SceneCommon::ENTITY_ELEMENT );
		return pnn;
	}
};
//------------------------------------------------------------
class dsLightLoader : public dsLoaderManual_ProcessInterface<dsLight>
{
public:
	DEFINE_CLASS_SHARED_PTR(dsLightLoader)
	//--------------------------------//
	dsLightLoader()
	{
	}
	virtual ~dsLightLoader()
	{
	}

	//dsLoaderManual_ProcessInterface
	virtual bool processElement( const TiXmlElement *element, traits::process_inout lit ) {
		LoadLight<false>(element, lit);
		return true;
	}
	virtual traits::pen_cont_out getProcessElementNames()const {
		shared_ptr_traits< traits::pen_cont >::type pnn( new traits::pen_cont );
		pnn->insert( &dsKeys::SceneCommon::LIGHT_ELEMENT );
		return pnn;
	}
};
//------------------------------------------------------------
class dsCameraLoader : public dsLoaderManual_ProcessInterface<dsCamera>
{
public:
	DEFINE_CLASS_SHARED_PTR(dsCameraLoader)
	//--------------------------------//
	dsCameraLoader() 
	{
	}
	virtual ~dsCameraLoader()
	{
	}

	//dsLoaderManual_ProcessInterface
	virtual bool processElement( const TiXmlElement *element, traits::process_inout cam ) {
		LoadCamera<false>(element, cam);
		return true;
	}
	virtual traits::pen_cont_out getProcessElementNames()const {
		shared_ptr_traits< traits::pen_cont >::type pnn( new traits::pen_cont );
		pnn->insert( &dsKeys::SceneCommon::CAMERA_ELEMENT );
		return pnn;
	}
};
//------------------------------------------------------------
class dsSceneNodeLoader : public dsLoaderManual_ProcessInterface< dsSceneNode >
{
public:
	DEFINE_CLASS_SHARED_PTR(dsSceneNodeLoader)
	//--------------------------------//
	dsSceneNodeLoader()
	{
	}
	virtual ~dsSceneNodeLoader()
	{
	}

	//dsLoaderManual_ProcessInterface
	virtual bool processElement( const TiXmlElement *element, traits::process_inout sn ) {
		LoadSceneNode<false>(element, sn );
		return true;
	}
	virtual traits::pen_cont_out getProcessElementNames()const {
		shared_ptr_traits< traits::pen_cont >::type pnn( new traits::pen_cont );
		pnn->insert( &dsKeys::SceneCommon::NODE_ELEMENT );
		return pnn;
	}
};
//------------------------------------------------------------
class dsMeshLodLoader : public dsLoaderManual_ProcessInterface< dsMeshLod >
{
public:
	DEFINE_CLASS_SHARED_PTR(dsMeshLodLoader)
	//--------------------------------//
	dsMeshLodLoader()
	{
	}
	virtual ~dsMeshLodLoader()
	{
	}

	//dsLoaderManual_ProcessInterface
	virtual bool processElement( const TiXmlElement *element, traits::process_inout ml ) {
		LoadMeshLod<false>(element, ml);
		return true;
	}
	virtual traits::pen_cont_out getProcessElementNames()const {
		shared_ptr_traits< traits::pen_cont >::type pnn( new traits::pen_cont );
		pnn->insert( &dsKeys::SceneCommon::MESHLOD_ELEMENT );
		return pnn;
	}
};
//------------------------------------------------------------
}
}
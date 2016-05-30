#include "SceneParamsCollection.h"

#if DBS_LM_COLLECTIONS

#include <OgreSubEntity.h>
#include <OgreSceneNode.h>
#include <OgreString.h>

#include "Interface\OgreRenderWindowInterface.h"
#include "Interface\OgreSceneManagerInterface.h"

namespace Ogre
{
	Demo::System::SceneParamsCollection* Ogre::Singleton< Demo::System::SceneParamsCollection >::ms_Singleton = 0;
}

namespace Demo
{
namespace System
{
	//--------------------------------//
	typedef SceneParamsCollection SPC;
	//--------------------------------//
#pragma region dsKeys
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	//------------------------------------------------------------
	struct NodeKeys
	{
		static SPC::string_traits::cstr NODES_FIELD_KEY;
		static SPC::string_traits::cstr NODE_ELEMENT_KEY;
		static SPC::string_traits::cstr POSITION_KEY;
		static SPC::string_traits::cstr ROTATION_KEY;
		static SPC::string_traits::cstr SCALE_KEY;
		static SPC::string_traits::cstr NAME_KEY;
		static SPC::string_traits::cstr ENTITY_ELEMENT_KEY;
		static SPC::string_traits::cstr CAMERA_ELEMENT_KEY;
		static SPC::string_traits::cstr LIGHT_ELEMENT_KEY;
		static SPC::string_traits::cstr PLANE_ELEMENT_KEY;
		static SPC::string_traits::cstr NODE_VISIBLE_KEY;
		static SPC::string_traits::cstr NODE_HIDDEN_KEY;
		static SPC::string_traits::cstr NODE_TREE_VISIBLE_KEY;
		static SPC::string_traits::cstr NODE_TREE_HIDDEN_KEY;
		static SPC::string_traits::cstr USER_DATA_KEY;
	};
	//--------------------------------//
	struct EntityKeys
	{
		static SPC::string_traits::cstr NAME_KEY;
		static SPC::string_traits::cstr MESH_FILE_KEY;
		static SPC::string_traits::cstr MATERIAL_FILE_KEY;
		static SPC::string_traits::cstr CAST_SHADOWS_KEY;
		static SPC::string_traits::cstr RECEIVE_SHADOWS_KEY;
		static SPC::string_traits::cstr RENDER_QUEUE_KEY;
		static SPC::string_traits::cstr RENDER_DISTANCE_KEY;
		static SPC::string_traits::cstr SUBENTITIES_FIELD_KEY;
		static SPC::string_traits::cstr SUBENTITY_ELEMENT_KEY;
		static SPC::string_traits::cstr VERTEX_BUFFER_KEY;
		static SPC::string_traits::cstr INDEX_BUFFER_KEY;
		static SPC::string_traits::cstr VISIBLE_KEY;
		static SPC::string_traits::cstr SUB_MATERIAL_NAME_KEY;
		static SPC::string_traits::cstr SUB_INDEX_KEY;
	};
	//--------------------------------//
	struct LightKeys
	{
		static SPC::string_traits::cstr TYPE_KEY;
		static SPC::string_traits::cstr TYPE_POINT_KEY;
		static SPC::string_traits::cstr TYPE_SPOT_KEY;
		static SPC::string_traits::cstr TYPE_DIRECT_KEY;
		static SPC::string_traits::cstr NAME_KEY;
		static SPC::string_traits::cstr COLOUR_DIFFUSE_KEY;
		static SPC::string_traits::cstr COLOUR_SPECULAR_KEY;
		static SPC::string_traits::cstr RANGE_KEY;
		static SPC::string_traits::cstr RANGE_INNER_KEY;
		static SPC::string_traits::cstr RANGE_OUTER_KEY;
		static SPC::string_traits::cstr RANGE_FALLOFF_KEY;
		static SPC::string_traits::cstr ATTENUATION_KEY;
		static SPC::string_traits::cstr ATTENUATION_RANGE_KEY;
		static SPC::string_traits::cstr ATTENUATION_CONSTANT_KEY;
		static SPC::string_traits::cstr ATTENUATION_LINEAR_KEY;
		static SPC::string_traits::cstr ATTENUATION_QUADRIC_KEY;
		static SPC::string_traits::cstr POSITION_KEY;
		static SPC::string_traits::cstr NORMAL_KEY;
		static SPC::string_traits::cstr VISIBLE_KEY;
		static SPC::string_traits::cstr CAST_SHADOWS_KEY;
		static SPC::string_traits::cstr POWER_KEY;
	};
	//--------------------------------//
	struct CameraKeys
	{
		static SPC::string_traits::cstr NAME_KEY;
		static SPC::string_traits::cstr VISIBLE_KEY;
		static SPC::string_traits::cstr FOV_KEY;
		static SPC::string_traits::cstr ASPECT_KEY;
		static SPC::string_traits::cstr TYPE_KEY;
		static SPC::string_traits::cstr CLIPPING_KEY;
		static SPC::string_traits::cstr POSITION_KEY;
		static SPC::string_traits::cstr ROTATION_KEY;
		static SPC::string_traits::cstr NORMAL_KEY;
		static SPC::string_traits::cstr TYPE_PERSPECTIVE;
		static SPC::string_traits::cstr TYPE_ORTHOGRAPHIC;
	};
	//--------------------------------//
	struct HardwareBufferUsageKeys
	{
		static SPC::string_traits::cstr STATIC_KEY;
		static SPC::string_traits::cstr DYNAMIC_KEY;
		static SPC::string_traits::cstr WRITE_ONLY_KEY;
		static SPC::string_traits::cstr STATIC_WRITE_ONLY;
		static SPC::string_traits::cstr DYNAMIC_WRITE_ONLY;
	};
	//--------------------------------//
	struct TextureTypeKeys
	{
		static SPC::string_traits::cstr TYPE_1D_KEY;
		static SPC::string_traits::cstr TYPE_2D_KEY;
		static SPC::string_traits::cstr TYPE_3D_KEY;
		static SPC::string_traits::cstr TYPE_CUBIC_KEY;
	};
	//--------------------------------//
	struct ShadowKeys
	{
		static SPC::string_traits::cstr SHADOW_TECH_KEY;
		static SPC::string_traits::cstr TECH_STENCIL_MODULATIVE_KEY;
		static SPC::string_traits::cstr TECH_STENCIL_ADDITIVE_KEY;
		static SPC::string_traits::cstr TECH_TEXTURE_MODULATIVE_KEY;
		static SPC::string_traits::cstr TECH_TEXTURE_ADDITIVE_KEY;
		static SPC::string_traits::cstr TECH_TEXTURE_MODULATIVE_INT_KEY;
		static SPC::string_traits::cstr TECH_TEXTURE_ADDITIVE_INT_KEY;
		static SPC::string_traits::cstr SHADOW_TEXTURES_KEY;
		static SPC::string_traits::cstr COLOUR_KEY;
		static SPC::string_traits::cstr FAR_DISTANCE_KEY;
		static SPC::string_traits::cstr SELF_SHADOW_KEY;
		static SPC::string_traits::cstr TEX_SIZE_KEY;
		static SPC::string_traits::cstr TEX_COUNT_KEY;
		static SPC::string_traits::cstr TEX_OFFSET_KEY;
		static SPC::string_traits::cstr TEX_FADE_START_KEY;
		static SPC::string_traits::cstr TEX_FADE_END_KEY;
		static SPC::string_traits::cstr TEX_CASTER_MATERIAL;
		static SPC::string_traits::cstr TEX_RECEIVER_MATERIAL;
		static SPC::string_traits::cstr CAMERA_SETUP_KEY;
	};
	//--------------------------------//
	struct ClippingKeys
	{
		static SPC::string_traits::cstr NEAR_KEY;
		static SPC::string_traits::cstr FAR_KEY;
	};
	//--------------------------------//
	struct PlaneKeys
	{
		static SPC::string_traits::cstr PLANE_X_KEY;
		static SPC::string_traits::cstr PLANE_Y_KEY;
		static SPC::string_traits::cstr PLANE_Z_KEY;
		static SPC::string_traits::cstr PLANE_D_KEY;
		static SPC::string_traits::cstr X_KEY;
		static SPC::string_traits::cstr Y_KEY;
		static SPC::string_traits::cstr Z_KEY;
		static SPC::string_traits::cstr D_KEY;
	};
	//--------------------------------//
	struct EnvironmentKeys
	{
		static SPC::string_traits::cstr ENVIRONMENT_FIELD_KEY;
		static SPC::string_traits::cstr FOG_KEY;
		static SPC::string_traits::cstr SKY_BOX_KEY;
		static SPC::string_traits::cstr SKY_DOME_KEY;
		static SPC::string_traits::cstr SKY_PLANE_KEY;
		static SPC::string_traits::cstr CLIPPING_KEY;
		static SPC::string_traits::cstr COLOUR_AMBIENT_KEY;
		static SPC::string_traits::cstr COLOUR_BACKGROUND_KEY;
		static SPC::string_traits::cstr SHADOWS_KEY;
	};
	//--------------------------------//
	struct FogKeys
	{
		static SPC::string_traits::cstr MODE_KEY;
		static SPC::string_traits::cstr MODE_EXP_KEY;
		static SPC::string_traits::cstr MODE_EXP2_KEY;
		static SPC::string_traits::cstr MODE_LINEAR_KEY;
		static SPC::string_traits::cstr EXP_DENSITY_KEY;
		static SPC::string_traits::cstr LINEAR_START_KEY;
		static SPC::string_traits::cstr LINEAR_END_KEY;
		static SPC::string_traits::cstr DIFFUSE_COLOUR_KEY;
	};
	//--------------------------------//
	struct SkyKeys
	{
		static SPC::string_traits::cstr ENABLE_KEY;
		static SPC::string_traits::cstr DRAW_FIRST_KEY;
		static SPC::string_traits::cstr DISTANCE_KEY;
		static SPC::string_traits::cstr ROTATION_KEY;
		static SPC::string_traits::cstr MATERIAL_KEY;
		static SPC::string_traits::cstr TILING_KEY;
		static SPC::string_traits::cstr CURVATURE_KEY;
		static SPC::string_traits::cstr X_SEGMENTS_KEY;
		static SPC::string_traits::cstr Y_SEGMENTS_KEY;
		static SPC::string_traits::cstr SCALE_KEY;
		static SPC::string_traits::cstr BOW_KEY;
	};
	//--------------------------------//
	struct MainKeys
	{
		static SPC::string_traits::cstr OGREMAX;
		static SPC::string_traits::cstr VEHICLES;
		static SPC::string_traits::cstr SCENE;
	};
	//--------------------------------//
	//------------------------------------------------------------
	SPC::string_traits::cstr NodeKeys::NODE_ELEMENT_KEY = "node";
	SPC::string_traits::cstr NodeKeys::NAME_KEY = "name";
	SPC::string_traits::cstr NodeKeys::NODES_FIELD_KEY = "nodes";
	SPC::string_traits::cstr NodeKeys::POSITION_KEY = "position";
	SPC::string_traits::cstr NodeKeys::ROTATION_KEY = "rotation";
	SPC::string_traits::cstr NodeKeys::SCALE_KEY = "scale";
	SPC::string_traits::cstr NodeKeys::ENTITY_ELEMENT_KEY = "entity";
	SPC::string_traits::cstr NodeKeys::CAMERA_ELEMENT_KEY = "camera";
	SPC::string_traits::cstr NodeKeys::LIGHT_ELEMENT_KEY = "light";
	SPC::string_traits::cstr NodeKeys::NODE_VISIBLE_KEY = "visible";
	SPC::string_traits::cstr NodeKeys::NODE_HIDDEN_KEY = "hidden";
	SPC::string_traits::cstr NodeKeys::NODE_TREE_VISIBLE_KEY = "treevisible";
	SPC::string_traits::cstr NodeKeys::NODE_TREE_HIDDEN_KEY = "treehidden";
	SPC::string_traits::cstr NodeKeys::PLANE_ELEMENT_KEY = "plane";
	SPC::string_traits::cstr NodeKeys::USER_DATA_KEY = "userData";
	//------------------------------------------------------------
	SPC::string_traits::cstr EntityKeys::MESH_FILE_KEY = "meshfile";
	SPC::string_traits::cstr EntityKeys::MATERIAL_FILE_KEY = "materialfile";
	SPC::string_traits::cstr EntityKeys::CAST_SHADOWS_KEY = "castshadows";
	SPC::string_traits::cstr EntityKeys::RECEIVE_SHADOWS_KEY = "receiveshadows";
	SPC::string_traits::cstr EntityKeys::RENDER_DISTANCE_KEY = "renderingdistance";
	SPC::string_traits::cstr EntityKeys::RENDER_QUEUE_KEY = "renderqueue";
	SPC::string_traits::cstr EntityKeys::SUBENTITIES_FIELD_KEY = "subentities";
	SPC::string_traits::cstr EntityKeys::SUBENTITY_ELEMENT_KEY = "subentity";
	SPC::string_traits::cstr EntityKeys::VERTEX_BUFFER_KEY = "vertexbuffer";
	SPC::string_traits::cstr EntityKeys::INDEX_BUFFER_KEY = "indexbuffer";
	SPC::string_traits::cstr EntityKeys::VISIBLE_KEY = "visible";
	SPC::string_traits::cstr EntityKeys::NAME_KEY = "name";
	SPC::string_traits::cstr EntityKeys::SUB_INDEX_KEY = "index";
	SPC::string_traits::cstr EntityKeys::SUB_MATERIAL_NAME_KEY = "materialname";
	//------------------------------------------------------------
	SPC::string_traits::cstr LightKeys::NAME_KEY = "name";
	SPC::string_traits::cstr LightKeys::TYPE_KEY = "type";
	SPC::string_traits::cstr LightKeys::CAST_SHADOWS_KEY = "castshadows";
	SPC::string_traits::cstr LightKeys::COLOUR_DIFFUSE_KEY = "colourdiffuse";
	SPC::string_traits::cstr LightKeys::COLOUR_SPECULAR_KEY = "colourspecular";
	SPC::string_traits::cstr LightKeys::POWER_KEY = "power";
	SPC::string_traits::cstr LightKeys::RANGE_INNER_KEY = "inner";
	SPC::string_traits::cstr LightKeys::RANGE_OUTER_KEY = "outer";
	SPC::string_traits::cstr LightKeys::RANGE_FALLOFF_KEY = "falloff";
	SPC::string_traits::cstr LightKeys::ATTENUATION_KEY = "lightattenuation";
	SPC::string_traits::cstr LightKeys::ATTENUATION_RANGE_KEY = "range";
	SPC::string_traits::cstr LightKeys::ATTENUATION_CONSTANT_KEY = "constant";
	SPC::string_traits::cstr LightKeys::ATTENUATION_LINEAR_KEY = "linear";
	SPC::string_traits::cstr LightKeys::ATTENUATION_QUADRIC_KEY = "quadric";
	SPC::string_traits::cstr LightKeys::TYPE_POINT_KEY = "point";
	SPC::string_traits::cstr LightKeys::TYPE_DIRECT_KEY = "directional";
	SPC::string_traits::cstr LightKeys::TYPE_SPOT_KEY = "spot";
	SPC::string_traits::cstr LightKeys::VISIBLE_KEY = "visible";
	SPC::string_traits::cstr LightKeys::POSITION_KEY = "position";
	SPC::string_traits::cstr LightKeys::RANGE_KEY = "lightrange";
	SPC::string_traits::cstr LightKeys::NORMAL_KEY = "normal";
	//------------------------------------------------------------
	SPC::string_traits::cstr CameraKeys::NAME_KEY = "name";
	SPC::string_traits::cstr CameraKeys::POSITION_KEY = "position";
	SPC::string_traits::cstr CameraKeys::ROTATION_KEY = "rotation";
	SPC::string_traits::cstr CameraKeys::NORMAL_KEY = "normal";
	SPC::string_traits::cstr CameraKeys::FOV_KEY = "fov";
	SPC::string_traits::cstr CameraKeys::ASPECT_KEY = "aspect";
	SPC::string_traits::cstr CameraKeys::TYPE_KEY = "type";
	SPC::string_traits::cstr CameraKeys::VISIBLE_KEY = "visible";
	SPC::string_traits::cstr CameraKeys::CLIPPING_KEY = "clipping";
	SPC::string_traits::cstr CameraKeys::TYPE_PERSPECTIVE = "perspective";
	SPC::string_traits::cstr CameraKeys::TYPE_ORTHOGRAPHIC = "orthographic";
	//------------------------------------------------------------
	SPC::string_traits::cstr HardwareBufferUsageKeys::STATIC_KEY = "static";
	SPC::string_traits::cstr HardwareBufferUsageKeys::DYNAMIC_KEY = "dynamic";
	SPC::string_traits::cstr HardwareBufferUsageKeys::STATIC_WRITE_ONLY = "staticwriteonly";
	SPC::string_traits::cstr HardwareBufferUsageKeys::DYNAMIC_WRITE_ONLY = "dynamicwriteonly";
	SPC::string_traits::cstr HardwareBufferUsageKeys::WRITE_ONLY_KEY = "writeonly";
	//------------------------------------------------------------
	SPC::string_traits::cstr TextureTypeKeys::TYPE_1D_KEY = "1d";
	SPC::string_traits::cstr TextureTypeKeys::TYPE_2D_KEY = "2d";
	SPC::string_traits::cstr TextureTypeKeys::TYPE_3D_KEY = "3d";
	SPC::string_traits::cstr TextureTypeKeys::TYPE_CUBIC_KEY = "cubic";
	//------------------------------------------------------------
	SPC::string_traits::cstr ShadowKeys::COLOUR_KEY = "colourshadow";
	SPC::string_traits::cstr ShadowKeys::FAR_DISTANCE_KEY = "fardistance";
	SPC::string_traits::cstr ShadowKeys::SELF_SHADOW_KEY = "selfshadow";
	SPC::string_traits::cstr ShadowKeys::SHADOW_TECH_KEY = "technique";
	SPC::string_traits::cstr ShadowKeys::SHADOW_TEXTURES_KEY = "shadowtextures";
	SPC::string_traits::cstr ShadowKeys::TECH_STENCIL_ADDITIVE_KEY = "stenciladditive";
	SPC::string_traits::cstr ShadowKeys::TECH_STENCIL_MODULATIVE_KEY = "stencilmodulative";
	SPC::string_traits::cstr ShadowKeys::TECH_TEXTURE_ADDITIVE_INT_KEY = "textureadditiveintegrated";
	SPC::string_traits::cstr ShadowKeys::TECH_TEXTURE_ADDITIVE_KEY = "textureadditive";
	SPC::string_traits::cstr ShadowKeys::TECH_TEXTURE_MODULATIVE_INT_KEY = "texturemodulativeintegrated";
	SPC::string_traits::cstr ShadowKeys::TECH_TEXTURE_MODULATIVE_KEY = "texturemodulative";
	SPC::string_traits::cstr ShadowKeys::TEX_CASTER_MATERIAL = "shadowCasterMaterial";
	SPC::string_traits::cstr ShadowKeys::TEX_RECEIVER_MATERIAL = "shadowReceiverMaterial";
	SPC::string_traits::cstr ShadowKeys::TEX_COUNT_KEY = "count";
	SPC::string_traits::cstr ShadowKeys::TEX_FADE_START_KEY = "fadestart";
	SPC::string_traits::cstr ShadowKeys::TEX_FADE_END_KEY = "fadeend";
	SPC::string_traits::cstr ShadowKeys::TEX_OFFSET_KEY = "offset";
	SPC::string_traits::cstr ShadowKeys::TEX_SIZE_KEY = "size";
	SPC::string_traits::cstr ShadowKeys::CAMERA_SETUP_KEY = "shadowcamerasetup";
	//------------------------------------------------------------
	SPC::string_traits::cstr ClippingKeys::NEAR_KEY = "near";
	SPC::string_traits::cstr ClippingKeys::FAR_KEY = "far";
	//------------------------------------------------------------
	SPC::string_traits::cstr PlaneKeys::PLANE_X_KEY = "planex";
	SPC::string_traits::cstr PlaneKeys::PLANE_Y_KEY = "planey";
	SPC::string_traits::cstr PlaneKeys::PLANE_Z_KEY = "planez";
	SPC::string_traits::cstr PlaneKeys::PLANE_D_KEY = "planed";
	SPC::string_traits::cstr PlaneKeys::X_KEY = "x";
	SPC::string_traits::cstr PlaneKeys::Y_KEY = "y";
	SPC::string_traits::cstr PlaneKeys::Z_KEY = "z";
	SPC::string_traits::cstr PlaneKeys::D_KEY = "d";
	//------------------------------------------------------------
	SPC::string_traits::cstr EnvironmentKeys::ENVIRONMENT_FIELD_KEY = "environment";
	SPC::string_traits::cstr EnvironmentKeys::FOG_KEY = "fog";
	SPC::string_traits::cstr EnvironmentKeys::SKY_BOX_KEY = "skybox";
	SPC::string_traits::cstr EnvironmentKeys::SKY_DOME_KEY = "skydome";
	SPC::string_traits::cstr EnvironmentKeys::SKY_PLANE_KEY = "skyplane";
	SPC::string_traits::cstr EnvironmentKeys::CLIPPING_KEY = "clipping";
	SPC::string_traits::cstr EnvironmentKeys::COLOUR_AMBIENT_KEY = "colourambient";
	SPC::string_traits::cstr EnvironmentKeys::COLOUR_BACKGROUND_KEY = "colourbackground";
	SPC::string_traits::cstr EnvironmentKeys::SHADOWS_KEY = "shadows";
	//------------------------------------------------------------
	SPC::string_traits::cstr SkyKeys::ENABLE_KEY = "enable";
	SPC::string_traits::cstr SkyKeys::MATERIAL_KEY = "material";
	SPC::string_traits::cstr SkyKeys::CURVATURE_KEY = "curvature";
	SPC::string_traits::cstr SkyKeys::DISTANCE_KEY = "distance";
	SPC::string_traits::cstr SkyKeys::DRAW_FIRST_KEY = "drawFirst";
	SPC::string_traits::cstr SkyKeys::ROTATION_KEY = "rotation";
	SPC::string_traits::cstr SkyKeys::TILING_KEY = "tiling";
	SPC::string_traits::cstr SkyKeys::BOW_KEY = "bow";
	SPC::string_traits::cstr SkyKeys::SCALE_KEY = "scale";
	SPC::string_traits::cstr SkyKeys::X_SEGMENTS_KEY = "xsegments";
	SPC::string_traits::cstr SkyKeys::Y_SEGMENTS_KEY = "ysegments";
	//------------------------------------------------------------
	SPC::string_traits::cstr FogKeys::EXP_DENSITY_KEY = "expdensity";
	SPC::string_traits::cstr FogKeys::LINEAR_START_KEY = "linearstart";
	SPC::string_traits::cstr FogKeys::LINEAR_END_KEY = "linearend";
	SPC::string_traits::cstr FogKeys::MODE_KEY = "mode";
	SPC::string_traits::cstr FogKeys::MODE_EXP_KEY = "exp";
	SPC::string_traits::cstr FogKeys::MODE_EXP2_KEY = "exp2";
	SPC::string_traits::cstr FogKeys::MODE_LINEAR_KEY = "linear";
	SPC::string_traits::cstr FogKeys::DIFFUSE_COLOUR_KEY = "colourdiffuse";
	//------------------------------------------------------------
	SPC::string_traits::cstr MainKeys::OGREMAX = "ogremax";
	SPC::string_traits::cstr MainKeys::VEHICLES = "vehicles";
	SPC::string_traits::cstr MainKeys::SCENE = "scene";
	//------------------------------------------------------------
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML
#pragma endregion
	//------------------------------------------------------------
	typedef OgreRenderWindowInterface ORWI;
	typedef OgreSceneManagerInterface OSMI;
	typedef OgremaxTinyXmlInterface XI;
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	SceneParamsCollection& SceneParamsCollection::getSingleton()
	{
		assert( ms_Singleton ); return *ms_Singleton;
	}
	//------------------------------------------------------------
	SceneParamsCollection* SceneParamsCollection::getSingletonPtr()
	{
		return ms_Singleton;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
#if DBS_LM_SCRIPT_OGREMAX_TIXML
	//------------------------------------------------------------
	/*static*/ Ogre::ProjectionType SceneParamsCollection::ParseProjectionType( string_traits::in projType, Ogre::ProjectionType defaultValue )
	{
		typedef CameraKeys CK;
		string_traits::str projLower = projType;
		Ogre::StringUtil::toLowerCase( projLower );
		if( projLower == CK::TYPE_PERSPECTIVE )
			return Ogre::PT_PERSPECTIVE;
		if( projLower == CK::TYPE_ORTHOGRAPHIC )
			return Ogre::PT_ORTHOGRAPHIC;
		return defaultValue;
	}
	//------------------------------------------------------------
	/*static*/ Ogre::FogMode SceneParamsCollection::ParseFogMode( string_traits::in fogModeStr, Ogre::FogMode defaultMode )
	{
		typedef FogKeys FK;
		string_traits::str fogModeLower = fogModeStr;
		Ogre::StringUtil::toLowerCase( fogModeLower );

		if( fogModeLower == FK::MODE_LINEAR_KEY )
			return Ogre::FOG_LINEAR;
		if( fogModeLower == FK::MODE_EXP_KEY )
			return Ogre::FOG_EXP;
		if( fogModeLower == FK::MODE_EXP2_KEY )
			return Ogre::FOG_EXP2;

		return defaultMode;
	}
	//------------------------------------------------------------
	/*static*/ Ogre::Light::LightTypes SceneParamsCollection::ParseLightType( string_traits::in typeStr, Ogre::Light::LightTypes defaultType )
	{
		typedef LightKeys LK;
		string_traits::str typeLower = typeStr;
		Ogre::StringUtil::toLowerCase( typeLower );
		if( typeLower == LK::TYPE_POINT_KEY )
			return Ogre::Light::LT_POINT;
		if( typeLower == LK::TYPE_SPOT_KEY )
			return Ogre::Light::LT_SPOTLIGHT;
		if( typeLower == LK::TYPE_DIRECT_KEY )
			return Ogre::Light::LT_DIRECTIONAL;
		return defaultType;
	}
	//------------------------------------------------------------
	/*static*/ Ogre::ShadowTechnique SceneParamsCollection::ParseShadowTechnique( string_traits::in stStr )
	{
		typedef ShadowKeys SK;
		string_traits::str ShadowTechStr = stStr;
		Ogre::StringUtil::toLowerCase( ShadowTechStr );
		if( ShadowTechStr == SK::TECH_STENCIL_ADDITIVE_KEY )
			return Ogre::SHADOWTYPE_STENCIL_ADDITIVE;
		else if( ShadowTechStr == SK::TECH_STENCIL_MODULATIVE_KEY )
			return Ogre::SHADOWTYPE_STENCIL_MODULATIVE;
		else if( ShadowTechStr == SK::TECH_TEXTURE_ADDITIVE_INT_KEY )
			return Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED;
		else if( ShadowTechStr == SK::TECH_TEXTURE_ADDITIVE_KEY )
			return Ogre::SHADOWTYPE_TEXTURE_ADDITIVE;
		else if( ShadowTechStr == SK::TECH_TEXTURE_MODULATIVE_INT_KEY )
			return Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED;
		else if( ShadowTechStr == SK::TECH_TEXTURE_MODULATIVE_KEY )
			return Ogre::SHADOWTYPE_TEXTURE_MODULATIVE;
		else
			return Ogre::SHADOWTYPE_NONE;
	}
	//------------------------------------------------------------
	/*static*/ Ogre::HardwareBuffer::Usage SceneParamsCollection::ParseHardwareBufferUsage( string_traits::in HWBUsageStr, Ogre::HardwareBuffer::Usage defaultUsage )
	{
		typedef HardwareBufferUsageKeys UK;
		string_traits::str usageLower = HWBUsageStr;
		Ogre::StringUtil::toLowerCase( usageLower );

		if( usageLower == UK::STATIC_KEY )return Ogre::HardwareBuffer::HBU_STATIC;
		if( usageLower == UK::DYNAMIC_KEY )return Ogre::HardwareBuffer::HBU_DYNAMIC;
		if( usageLower == UK::STATIC_WRITE_ONLY )return Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY;
		if( usageLower == UK::DYNAMIC_WRITE_ONLY )return Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY;
		if( usageLower == UK::WRITE_ONLY_KEY )return Ogre::HardwareBuffer::HBU_WRITE_ONLY;
		return defaultUsage;
	}
	//------------------------------------------------------------
	/*static*/ Core::ObjectParams::OBJECT_VISIBILITY SceneParamsCollection::ParseObjectVisibility( const string_traits::str &ovStr )
	{
		if( ovStr.empty() )return Core::ObjectParams::OV_DEFAULT;
		return XI::ParseBool( ovStr, true ) ? Core::ObjectParams::OV_VISIBLE : Core::ObjectParams::OV_HIDDEN;
	}
	//------------------------------------------------------------
	/*static*/ Core::SceneNodeParams::NODE_VISIBILITY SceneParamsCollection::ParseNodeVisibility( const string_traits::str& nvStr )
	{
		typedef NodeKeys NK;
		string_traits::str visLower = nvStr;
		Ogre::StringUtil::toLowerCase( visLower );
		if( visLower == NK::NODE_VISIBLE_KEY )
			return Core::SceneNodeParams::NV_VISIBLE;
		if( visLower == NK::NODE_HIDDEN_KEY )
			return Core::SceneNodeParams::NV_HIDDEN;
		if( visLower == NK::NODE_TREE_VISIBLE_KEY )
			return Core::SceneNodeParams::NV_TREE_VISIBLE;
		if( visLower == NK::NODE_TREE_HIDDEN_KEY )
			return Core::SceneNodeParams::NV_TREE_HIDDEN;
		return Core::SceneNodeParams::NV_DEFAULT;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	/*static*/ void SceneParamsCollection::SetSceneNodeVisibility( traits::OgreSceneNode_traits::inout node, Core::SceneNodeParams::NODE_VISIBILITY vis )
	{
		switch( vis )
		{
		case Core::SceneNodeParams::NV_VISIBLE:
			node->setVisible( true, false );
			break;
		case Core::SceneNodeParams::NV_HIDDEN:
			node->setVisible( false, false );
			break;
		case Core::SceneNodeParams::NV_TREE_VISIBLE:
			node->setVisible( true, true );
			break;
		case Core::SceneNodeParams::NV_TREE_HIDDEN:
			node->setVisible( false, true );
			break;
		};
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	SPC::traits::OgreSceneNode_traits::out SceneParamsCollection::CreateSceneNode( const Core::SceneNodeParams &snp, void* parentObject, int createOptions )
	{
		string_traits::str uniqueName = snp.name;
		Ogre::StringUtil::addNameUniquePart( uniqueName, parentObject );
		traits::OgreSceneNode_traits::out returnNode = OSMI::get()->createSceneNode( uniqueName );
		if( returnNode == 0 )return 0;
		returnNode->setPosition( snp.position );
		returnNode->setOrientation( snp.orientation );
		returnNode->setScale( snp.scale );

		{
			Core::SceneNodeParams::traits::obj_container::iterator iter = snp.objects.begin();
			Core::SceneNodeParams::traits::obj_container::iterator end_iter = snp.objects.end();
			for( ; iter != end_iter; ++iter )
			{
				switch( (*iter)->getType() )
				{
				case Core::ObjectParams::OT_ENTITY:
					if( (createOptions & CO_SKIP_ENTITIES) == 0 )
					{
						Core::EntityParams::const_shared_ptr entParams;
						entParams.downcast_assign( *iter );
						if( entParams.isNull() )break;
						traits::OgreEntity_traits::out ent = CreateEntity( *entParams, parentObject );
						if( ent != 0 )returnNode->attachObject( ent );
					}
					break;
				case Core::ObjectParams::OT_CAMERA:
					if( (createOptions & CO_SKIP_CAMERAS) == 0 )
					{
						Core::CameraParams::const_shared_ptr camParams;
						camParams.downcast_assign( *iter );
						traits::OgreCamera_traits::out camera = CreateCamera( *camParams, parentObject );
						if( camera != 0 )returnNode->attachObject( camera );
					}
					break;
				case Core::ObjectParams::OT_LIGHT:
					if( (createOptions & CO_SKIP_LIGHTS) == 0 )
					{
						Core::LightParams::const_shared_ptr lightParams;
						lightParams.downcast_assign( *iter );
						traits::OgreLight_traits::out light = CreateLight( *lightParams, parentObject );
						if( light != 0 )returnNode->attachObject( light );
					}
					break;
				}
			}
		}

		{
			Core::SceneNodeParams::traits::snp_container::iterator iter = snp.nodes.begin();
			Core::SceneNodeParams::traits::snp_container::iterator end_iter = snp.nodes.end();
			for( ; iter != end_iter; ++iter )
			{
				traits::OgreSceneNode_traits::out attachNode = CreateSceneNode( iter->getReference(), parentObject, createOptions );
				if( attachNode != 0 )returnNode->addChild( attachNode );
			}
		}

		SetSceneNodeVisibility( returnNode, snp.visibility );
		return returnNode;
	}
	//------------------------------------------------------------
	SPC::traits::OgreEntity_traits::out SceneParamsCollection::CreateEntity( const Core::EntityParams &entParams, void* parentObject )
	{
		string_traits::str uniqueName = entParams.name;
		Ogre::StringUtil::addNameUniquePart( uniqueName, parentObject );
		traits::OgreEntity_traits::out ent = OSMI::get()->createEntity( uniqueName, entParams.meshFilename );
		if( ent == 0 )return 0;
		ent->setMaterialName( entParams.materialName );
		unsigned int subEntitiesCount = ent->getNumSubEntities();
		for( unsigned int i = 0; i < subEntitiesCount; ++i )
		{
			Ogre::SubEntity* subEntity = ent->getSubEntity( i );
			if( !entParams.subMatNames[ i ].empty() )
				subEntity->setMaterialName( entParams.subMatNames[ i ] );
		}
		return ent;
	}
	//------------------------------------------------------------
	SPC::traits::OgreCamera_traits::out SceneParamsCollection::CreateCamera( const Core::CameraParams &camParams, void* parentObject )
	{
		string_traits::str uniqueName = camParams.name;
		Ogre::StringUtil::addNameUniquePart( uniqueName, parentObject );
		traits::OgreCamera_traits::out cam = OSMI::get()->createCamera( uniqueName );
		if( cam == 0 )return 0;
		cam->setFOVy( Ogre::Radian( camParams.fov ) );
		cam->setAspectRatio( camParams.aspectRatio );
		cam->setProjectionType( camParams.projectionType );
		cam->setNearClipDistance( camParams.nearClip );
		cam->setFarClipDistance( camParams.farClip );
		cam->setPosition( camParams.position );
		cam->setOrientation( camParams.orientation );
		cam->setDirection( camParams.direction );
		return cam;
	}
	//------------------------------------------------------------
	SPC::traits::OgreLight_traits::out SceneParamsCollection::CreateLight( const Core::LightParams &lightParams, void* parentObject )
	{
		string_traits::str uniqueName = lightParams.name;
		Ogre::StringUtil::addNameUniquePart( uniqueName, parentObject );
		traits::OgreLight_traits::out light = OSMI::get()->createLight( uniqueName );
		if( light == 0 )return 0;
		light->setType( lightParams.lightType );
		light->setCastShadows( lightParams.castShadows );
		light->setPowerScale( lightParams.power );
		light->setDiffuseColour( lightParams.diffuseColour );
		light->setSpecularColour( lightParams.specularColour );
		return light;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	SPC::traits::ObjectParams_traits::out SceneParamsCollection::CreateObjectParams( traits::OgreMovableObject_traits::in obj, int createOptions )
	{
		if( obj == 0 )return traits::ObjectParams_traits::out();

		if( (createOptions & CO_SKIP_ENTITIES) == 0 )
		{
			traits::OgreEntity_traits::in ent( dynamic_cast< traits::OgreEntity_traits::in >( obj ) );
			if( ent != 0 )return CreateEntityParams( ent );
		}

		if( (createOptions & CO_SKIP_CAMERAS) == 0 )
		{
			traits::OgreCamera_traits::in cam( dynamic_cast< traits::OgreCamera_traits::in >( obj ) );
			if( cam != 0 )return CreateCameraParams( cam );
		}

		if( (createOptions & CO_SKIP_LIGHTS) == 0 )
		{
			traits::OgreLight_traits::in light( dynamic_cast< traits::OgreLight_traits::in >( obj ) );
			if( light != 0 )return CreateLightParams( light );
		}

		return traits::ObjectParams_traits::out();
	}
	//------------------------------------------------------------
	SPC::traits::EntityParams_traits::out SceneParamsCollection::CreateEntityParams( traits::OgreEntity_traits::in ent )
	{
		if( ent == 0 )return traits::EntityParams_traits::out();

		string_traits::str baseName = ent->getName();
		Ogre::StringUtil::removeNameUniquePart( baseName );

		traits::EntityParams_traits::out newEnt( new Core::EntityParams() );
		//names
		Ogre::StringUtil::removeNameUniquePart( baseName );
		newEnt->name = baseName;
		newEnt->meshFilename = ent->getMesh()->getName();
		//materials
		unsigned int subEntCount = ent->getNumSubEntities();
		for( unsigned int i = 0; i < subEntCount; ++i )
			newEnt->subMatNames.push_back( ent->getSubEntity( i )->getMaterialName() );
		//cast shadows
		newEnt->castShadows = ent->getCastShadows();

		return newEnt;
	}
	//------------------------------------------------------------
	SPC::traits::LightParams_traits::out SceneParamsCollection::CreateLightParams( traits::OgreLight_traits::in light )
	{
		if( light == 0 )return traits::LightParams_traits::out();

		string_traits::str baseName = light->getName();
		Ogre::StringUtil::removeNameUniquePart( baseName );

		traits::LightParams_traits::out newLight( new Core::LightParams() );
		newLight->name = baseName;
		newLight->lightType = light->getType();
		newLight->castShadows = light->getCastShadows();
		newLight->power = light->getPowerScale();
		newLight->diffuseColour = light->getDiffuseColour();
		newLight->specularColour = light->getSpecularColour();
		newLight->spotlightInnerAngle = light->getSpotlightInnerAngle();
		newLight->spotlightOuterAngle = light->getSpotlightOuterAngle();
		newLight->spotlightFalloff = light->getSpotlightFalloff();
		newLight->attenuationRange = light->getAttenuationRange();
		newLight->attenuationConstant = light->getAttenuationConstant();
		newLight->attenuationLinear = light->getAttenuationLinear();
		newLight->attenuationQuadric = light->getAttenuationQuadric();
		newLight->position = light->getPosition();
		newLight->direction = light->getDirection();

		return newLight;
	}
	//------------------------------------------------------------
	SPC::traits::CameraParams_traits::out SceneParamsCollection::CreateCameraParams( traits::OgreCamera_traits::in cam )
	{
		if( cam == 0 )return traits::CameraParams_traits::out();

		string_traits::str baseName = cam->getName();
		Ogre::StringUtil::removeNameUniquePart( baseName );

		traits::CameraParams_traits::out newCam( new Core::CameraParams() );
		newCam->name = baseName;
		newCam->fov = cam->getFOVy();
		newCam->aspectRatio = cam->getAspectRatio();
		newCam->projectionType = cam->getProjectionType();
		newCam->nearClip = cam->getNearClipDistance();
		newCam->farClip = cam->getFarClipDistance();
		newCam->position = cam->getPosition();
		newCam->orientation = cam->getOrientation();
		newCam->direction = cam->getDirection();

		return newCam;
	}
	//------------------------------------------------------------
	SPC::traits::SceneNodeParams_traits::out SceneParamsCollection::CreateSceneNodeParams( traits::OgreSceneNode_traits::in node, int createOptions )
	{
		typedef Core::SceneNodeParams::traits::snp_elem sn_elem;
		typedef Core::SceneNodeParams::traits::obj_elem obj_elem;

		if( node == 0 )return traits::SceneNodeParams_traits::out();

		string_traits::str baseName = node->getName();
		Ogre::StringUtil::removeNameUniquePart( baseName );

		traits::SceneNodeParams_traits::out newNode( new Core::SceneNodeParams() );
		newNode->name = baseName;
		newNode->position = node->getPosition();
		newNode->orientation = node->getOrientation();
		newNode->scale = node->getScale();

		Ogre::SceneNode::ConstObjectIterator oiter = node->getAttachedObjectIterator();
		while( oiter.hasMoreElements() )
		{
			traits::OgreMovableObject_traits::in obj = oiter.getNextCRef();
			obj_elem objElem = CreateObjectParams( obj );
			if( objElem != 0 )newNode->objects.insert( objElem );
		}

		Ogre::SceneNode::ConstChildNodeIterator niter = node->getChildIterator();
		while( niter.hasMoreElements() )
		{
			traits::OgreSceneNode_traits::in snInPar( static_cast< traits::OgreSceneNode_traits::in >(niter.getNext()) );
			sn_elem node( CreateSceneNodeParams( snInPar, createOptions ) );
			if( node != 0 )newNode->nodes.insert( node );
		}

		return newNode;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	SPC::traits::EntityParams_traits::out SceneParamsCollection::LoadEntityParams( tixmlel_traits::in xmlElement )
	{
		typedef EntityKeys EK;
		traits::EntityParams_traits::store_type entity( new Core::EntityParams() );
		entity->name = XI::GetString( xmlElement, EK::NAME_KEY );
		entity->meshFilename = XI::GetString( xmlElement, EK::MESH_FILE_KEY );
		entity->materialName = XI::GetString( xmlElement, EK::MATERIAL_FILE_KEY );
		entity->castShadows = XI::GetBool( xmlElement, EK::CAST_SHADOWS_KEY, true );
		//entity->renderDistance = XI::GetRealAttribute( xmlElement, EK::RENDER_DISTANCE_KEY );

		string_traits::str elementName;
		tixmlel_traits::in childElement(0);
		while( childElement = XI::IterateChildElements( xmlElement, childElement ) )
		{
			elementName = childElement->Value();
			if( elementName == EK::SUBENTITIES_FIELD_KEY )
			{
				LoadSubEntityParams( childElement, *entity );
			}
		}
		return entity;
	}
	//------------------------------------------------------------
	void SceneParamsCollection::LoadSubEntityParams( tixmlel_traits::in element, Core::EntityParams &ent )
	{
		typedef EntityKeys EK;
		string_traits::str childName;
		tixmlel_traits::in childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == EK::SUBENTITY_ELEMENT_KEY )
			{
				ent.subMatNames.push_back( XI::GetString( childElement, EK::SUB_MATERIAL_NAME_KEY ) );
			}
		}
	}
	//------------------------------------------------------------
	SPC::traits::CameraParams_traits::out SceneParamsCollection::LoadCameraParams( tixmlel_traits::in xmlElement )
	{
		typedef CameraKeys CK;
		traits::CameraParams_traits::store_type camera( new Core::CameraParams() );
		camera->name = XI::GetString( xmlElement, CK::NAME_KEY );
		string_traits::str camVisStr = XI::GetString( xmlElement, CK::VISIBLE_KEY );
		string_traits::str camTypeStr = XI::GetString( xmlElement, CK::TYPE_KEY );

		camera->visibility = ParseObjectVisibility( camVisStr );
		camera->projectionType = ParseProjectionType( camTypeStr );
		camera->fov = XI::GetReal( xmlElement, CK::FOV_KEY, Ogre::Math::PI/2 );
		camera->aspectRatio = XI::GetReal( xmlElement, CK::ASPECT_KEY, 1.33f );

		string_traits::str elementName;
		tixmlel_traits::in childElement(0);
		while( childElement = XI::IterateChildElements( xmlElement, childElement ) )
		{
			elementName = childElement->Value();
			if( elementName == CK::CLIPPING_KEY )
			{
				LoadClippingParams( childElement, camera->nearClip, camera->farClip ); 
			}
			else if( elementName == CK::POSITION_KEY )
			{
				XI::GetVector3( childElement, camera->position, Ogre::Vector3::ZERO );
			}
			else if( elementName == CK::ROTATION_KEY )
			{
				XI::GetQuaternion( childElement, camera->orientation, Ogre::Quaternion::IDENTITY );
			}
			else if( elementName == CK::NORMAL_KEY )
			{
				XI::GetVector3( childElement, camera->direction, Ogre::Vector3::ZERO );
			}
		}
		return camera;
	}
	//------------------------------------------------------------
	SPC::traits::LightParams_traits::out SceneParamsCollection::LoadLightParams( tixmlel_traits::in element )
	{
		typedef LightKeys LK;
		traits::LightParams_traits::store_type light( new Core::LightParams() );
		light->name = XI::GetString( element, LK::NAME_KEY );
		string_traits::str visStr = XI::GetString( element, LK::VISIBLE_KEY );
		string_traits::str typeStr = XI::GetString( element, LK::TYPE_KEY );

		light->visibility = ParseObjectVisibility( visStr );
		light->lightType = ParseLightType( typeStr );
		light->castShadows = XI::GetBool( element, LK::CAST_SHADOWS_KEY );
		light->power = XI::GetReal( element, LK::POWER_KEY, 1.0f );

		string_traits::str elementName;
		const TiXmlElement* childElement(0);
		while( childElement = XI::IterateChildElements( element, childElement ) )
		{
			elementName = childElement->Value();
			if( elementName == LK::COLOUR_DIFFUSE_KEY )
			{
				XI::GetColour( childElement, light->diffuseColour, Ogre::ColourValue::White );
			}
			else if( elementName == LK::COLOUR_SPECULAR_KEY )
			{
				XI::GetColour( childElement, light->specularColour, Ogre::ColourValue::White );
			}
			else if( elementName == LK::RANGE_KEY )
			{
				LoadLightRangeParameters( childElement, *light );
			}
			else if( elementName == LK::ATTENUATION_KEY )
			{
				LoadLightAttenuationParameters( childElement, *light );
			}
			else if( elementName == LK::POSITION_KEY )
			{
				XI::GetVector3( childElement, light->position, Ogre::Vector3::ZERO );
			}
			else if( elementName == LK::NORMAL_KEY )
			{
				XI::GetVector3( childElement, light->direction, Ogre::Vector3::ZERO );
			}
		}
		return light;
	}
	//------------------------------------------------------------
	void SceneParamsCollection::LoadLightAttenuationParameters( tixmlel_traits::in element, Core::LightParams &outLightParams )
	{
		typedef LightKeys LK;
		string_traits::str value;

		value = XI::GetString( element, LK::ATTENUATION_CONSTANT_KEY );
		if( !value.empty() )
			outLightParams.attenuationConstant = Ogre::StringConverter::parseReal( value );

		value = XI::GetString( element, LK::ATTENUATION_LINEAR_KEY );
		if( !value.empty() )
			outLightParams.attenuationLinear = Ogre::StringConverter::parseReal( value );

		value = XI::GetString( element, LK::ATTENUATION_QUADRIC_KEY );
		if( !value.empty() )
			outLightParams.attenuationQuadric = Ogre::StringConverter::parseReal( value );

		value = XI::GetString( element, LK::ATTENUATION_RANGE_KEY );
		if( !value.empty() )
			outLightParams.attenuationRange = Ogre::StringConverter::parseReal( value );
	}
	//------------------------------------------------------------
	void SceneParamsCollection::LoadLightRangeParameters( tixmlel_traits::in element, Core::LightParams &outLightParams )
	{
		typedef LightKeys LK;
		if( outLightParams.lightType == Ogre::Light::LT_SPOTLIGHT )
		{
			string_traits::str value;
			value = XI::GetString( element, LK::RANGE_INNER_KEY );
			if( !value.empty() )
				outLightParams.spotlightInnerAngle = Ogre::Radian( Ogre::StringConverter::parseReal( value ) );

			value = XI::GetString( element, LK::RANGE_OUTER_KEY );
			if( !value.empty() )
				outLightParams.spotlightOuterAngle = Ogre::Radian( Ogre::StringConverter::parseReal( value ) );

			value = XI::GetString( element, LK::RANGE_FALLOFF_KEY );
			if( !value.empty() )
				outLightParams.spotlightFalloff = Ogre::StringConverter::parseReal( value );
		}
	}
	//------------------------------------------------------------
	void SceneParamsCollection::LoadClippingParams( tixmlel_traits::in objectElement, Ogre::Real& outNearClip, Ogre::Real& outFarClip )
	{
		outNearClip = XI::GetReal( objectElement, ClippingKeys::NEAR_KEY, envParams.nearClip );
		outFarClip = XI::GetReal( objectElement, ClippingKeys::FAR_KEY, envParams.farClip );
	}
	//------------------------------------------------------------
	void SceneParamsCollection::LoadPlaneParams( tixmlel_traits::in xmlElement, Ogre::Plane &outPlane )
	{
		typedef PlaneKeys PK;
		Ogre::Plane defaultPlane( Ogre::Vector3(0,1,0), 5000 );
		if( xmlElement->Attribute( PK::X_KEY ) )
		{
			outPlane.normal.x = XI::GetReal( xmlElement, PK::X_KEY, defaultPlane.normal.x );
			outPlane.normal.y = XI::GetReal( xmlElement, PK::Y_KEY, defaultPlane.normal.y );
			outPlane.normal.z = XI::GetReal( xmlElement, PK::Z_KEY, defaultPlane.normal.z );
			outPlane.d = XI::GetReal( xmlElement, PK::D_KEY, defaultPlane.d );
		}
		else if( xmlElement->Attribute( PK::PLANE_X_KEY ) )
		{
			outPlane.normal.x = XI::GetReal( xmlElement, PK::PLANE_X_KEY, defaultPlane.normal.x );
			outPlane.normal.y = XI::GetReal( xmlElement, PK::PLANE_Y_KEY, defaultPlane.normal.y );
			outPlane.normal.z = XI::GetReal( xmlElement, PK::PLANE_Z_KEY, defaultPlane.normal.z );
			outPlane.d = XI::GetReal( xmlElement, PK::PLANE_D_KEY, defaultPlane.d );
		}
	}
	//------------------------------------------------------------
	void SceneParamsCollection::LoadSkyBoxParams( tixmlel_traits::in objectElement )
	{
		envParams.skyBox.enabled = XI::GetBool( objectElement, SkyKeys::ENABLE_KEY );
		envParams.skyBox.drawFirst = XI::GetBool( objectElement, SkyKeys::DRAW_FIRST_KEY, true );
		envParams.skyBox.distance = XI::GetReal( objectElement, SkyKeys::DISTANCE_KEY, 5000.0f );
		envParams.skyBox.materialName = XI::GetString( objectElement, SkyKeys::MATERIAL_KEY );

		string_traits::str elementName;
		tixmlel_traits::in childElement(0);
		while( childElement = XI::IterateChildElements( objectElement, childElement ) )
		{
			elementName = childElement->Value();
			if( elementName == SkyKeys::ROTATION_KEY )
				XI::GetQuaternion( childElement, envParams.skyBox.rotation, Ogre::Quaternion::IDENTITY );
		}
		envParams.skyBoxLoaded = true;
	}
	//------------------------------------------------------------
	void SceneParamsCollection::LoadSkyDomeParams( tixmlel_traits::in objectElement )
	{
		envParams.skyDome.enabled = XI::GetBool( objectElement, SkyKeys::ENABLE_KEY );
		envParams.skyDome.drawFirst = XI::GetBool( objectElement, SkyKeys::DRAW_FIRST_KEY, true );
		envParams.skyDome.xSegments = XI::GetInt( objectElement, SkyKeys::X_SEGMENTS_KEY );
		envParams.skyDome.ySegments = XI::GetInt( objectElement, SkyKeys::Y_SEGMENTS_KEY );
		envParams.skyDome.tiling = XI::GetReal( objectElement, SkyKeys::TILING_KEY );
		envParams.skyDome.curvature = XI::GetReal( objectElement, SkyKeys::CURVATURE_KEY );
		envParams.skyDome.distance = XI::GetReal( objectElement, SkyKeys::DISTANCE_KEY );
		envParams.skyDome.materialName = XI::GetString( objectElement, SkyKeys::MATERIAL_KEY );

		string_traits::str elementName;
		tixmlel_traits::in childElement(0);
		while( childElement = XI::IterateChildElements( objectElement, childElement ) )
		{
			elementName = childElement->Value();
			if( elementName == SkyKeys::ROTATION_KEY )
				XI::GetQuaternion( childElement, envParams.skyDome.rotation, Ogre::Quaternion::IDENTITY );
		}
		envParams.skyDomeLoaded = true;
	}
	//------------------------------------------------------------
	void SceneParamsCollection::LoadSkyPlaneParams( tixmlel_traits::in objectElement )
	{
		envParams.skyPlane.enabled = XI::GetBool( objectElement, SkyKeys::ENABLE_KEY );
		envParams.skyPlane.drawFirst = XI::GetBool( objectElement, SkyKeys::DRAW_FIRST_KEY, true );
		envParams.skyPlane.scale = XI::GetReal( objectElement, SkyKeys::SCALE_KEY, envParams.skyPlane.scale );
		envParams.skyPlane.tiling = XI::GetReal( objectElement, SkyKeys::TILING_KEY, envParams.skyPlane.tiling );
		envParams.skyPlane.xSegments = XI::GetUInt( objectElement, SkyKeys::X_SEGMENTS_KEY, envParams.skyPlane.xSegments );
		envParams.skyPlane.ySegments = XI::GetUInt( objectElement, SkyKeys::Y_SEGMENTS_KEY, envParams.skyPlane.ySegments );
		envParams.skyPlane.materialName = XI::GetString( objectElement, SkyKeys::MATERIAL_KEY );
		LoadPlaneParams( objectElement, envParams.skyPlane.plane );

		string_traits::str elementName;
		tixmlel_traits::in childElement(0);
		while( childElement = XI::IterateChildElements( objectElement, childElement ) )
		{
			elementName = childElement->Value();
			if( elementName == SkyKeys::ROTATION_KEY )
				XI::GetQuaternion( childElement, envParams.skyPlane.rotation, Ogre::Quaternion::IDENTITY );
		}
		envParams.skyPlaneLoaded = true;
	}
	//------------------------------------------------------------
	void SceneParamsCollection::LoadFogParams( tixmlel_traits::in objectElement )
	{
		envParams.fog.expDensity = XI::GetReal( objectElement, FogKeys::EXP_DENSITY_KEY, envParams.fog.expDensity );
		envParams.fog.linearStart = XI::GetReal( objectElement, FogKeys::LINEAR_START_KEY, envParams.fog.linearStart );
		envParams.fog.linearEnd = XI::GetReal( objectElement, FogKeys::LINEAR_END_KEY, envParams.fog.linearEnd );

		const string_traits::str& fogModeStr = XI::GetString( objectElement, FogKeys::MODE_KEY );
		envParams.fog.mode = ParseFogMode( fogModeStr );

		tixmlel_traits::in colourElement = objectElement->FirstChildElement( FogKeys::DIFFUSE_COLOUR_KEY );
		if( colourElement != 0 )
			XI::GetColour( colourElement, envParams.fog.colour, Ogre::ColourValue::White );
	}
	//------------------------------------------------------------
	void SceneParamsCollection::LoadShadowParams( tixmlel_traits::in xmlElement )
	{
		typedef ShadowKeys SK;
		string_traits::str ShadowTechStr = XI::GetString( xmlElement, SK::SHADOW_TECH_KEY );
		envParams.shadow.technique = ParseShadowTechnique( ShadowTechStr );
		envParams.shadow.selfShadow = XI::GetBool( xmlElement, SK::SELF_SHADOW_KEY, envParams.shadow.selfShadow );
		envParams.shadow.farDistance = XI::GetReal( xmlElement, SK::FAR_DISTANCE_KEY, envParams.shadow.farDistance );

		string_traits::str elementName;
		tixmlel_traits::in childElement(0);
		while( childElement = XI::IterateChildElements( xmlElement, childElement ) )
		{
			elementName = childElement->Value();
			if( elementName == SK::SHADOW_TEXTURES_KEY )
			{
				envParams.shadow.texFadeStart = XI::GetReal( childElement, SK::TEX_FADE_START_KEY, envParams.shadow.texFadeStart );
				envParams.shadow.texFadeEnd = XI::GetReal( childElement, SK::TEX_FADE_END_KEY, envParams.shadow.texFadeEnd );
				envParams.shadow.texCount = XI::GetUInt( childElement, SK::TEX_COUNT_KEY, envParams.shadow.texCount );
				envParams.shadow.texOffset = XI::GetReal( childElement, SK::TEX_OFFSET_KEY, envParams.shadow.texOffset );
				envParams.shadow.texSize = XI::GetUInt( childElement, SK::TEX_SIZE_KEY, envParams.shadow.texSize );
				envParams.shadow.texShadowCasterMaterial = XI::GetString( childElement, SK::TEX_CASTER_MATERIAL );
				envParams.shadow.texShadowReceiverMaterial = XI::GetString( childElement, SK::TEX_RECEIVER_MATERIAL );
			}
			else if( elementName == SK::COLOUR_KEY )
			{
				XI::GetColour( childElement, envParams.shadow.colour, Ogre::ColourValue::White  );
			}
			else if( elementName == SK::CAMERA_SETUP_KEY )
			{
				envParams.shadow.cameraSetup = XI::GetString( childElement, string_traits::str( "type" ), string_traits::str( "lispsm" ) );
				LoadPlaneParams( childElement, envParams.shadow.optimalPlane );
			}
		}
	}
	//------------------------------------------------------------
	void SceneParamsCollection::LoadEnvironmentParams( tixmlel_traits::in xmlElement, int loadOptions )
	{
		tixmlel_traits::in childElement( 0 );
		string_traits::str elementName;
		while( childElement = XI::IterateChildElements( xmlElement, childElement ) )
		{
			elementName = childElement->Value();
			if( elementName == EnvironmentKeys::FOG_KEY )
				LoadFogParams( childElement );
			else if( elementName == EnvironmentKeys::SKY_BOX_KEY && !(loadOptions & LO_SKIP_SKY) )
				LoadSkyBoxParams( childElement );
			else if( elementName == EnvironmentKeys::SKY_DOME_KEY && !(loadOptions & LO_SKIP_SKY) )
				LoadSkyDomeParams( childElement );
			else if( elementName == EnvironmentKeys::SKY_PLANE_KEY && !(loadOptions & LO_SKIP_SKY) )
				LoadSkyPlaneParams( childElement );
			else if( elementName == EnvironmentKeys::CLIPPING_KEY )
				LoadClippingParams( childElement, envParams.nearClip, envParams.farClip );
			else if( elementName == EnvironmentKeys::COLOUR_AMBIENT_KEY )
				XI::GetColour( childElement, envParams.ambientColour, envParams.ambientColour );
			else if( elementName == EnvironmentKeys::COLOUR_BACKGROUND_KEY )
				XI::GetColour( childElement, envParams.backgroundColour, envParams.backgroundColour );
			else if( elementName == EnvironmentKeys::SHADOWS_KEY && !(loadOptions & LO_SKIP_SHADOWS) )
				LoadShadowParams( childElement );
		}
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	SPC::traits::SceneNodeParams_traits::out SceneParamsCollection::GetSceneSnp(string_traits::in name)const
	{
		if( name.empty() ) return traits::SceneNodeParams_traits::store_type();
		traits::SceneNodeParams_traits::container::const_iterator iter = sceneSnpContainer.find( name );
		if( iter == sceneSnpContainer.end() ) return traits::SceneNodeParams_traits::store_type();
		return iter->second->CreateCopyPtr();
	}
	//------------------------------------------------------------
	SPC::traits::SceneNodeParams_traits::out SceneParamsCollection::GetVehicleSnp(string_traits::in name)const
	{
		if( name.empty() )return traits::SceneNodeParams_traits::store_type();
		traits::SceneNodeParams_traits::container::const_iterator iter = vehicleSnpContainer.find( name );
		if( iter == vehicleSnpContainer.end() )return traits::SceneNodeParams_traits::store_type();
		return iter->second->CreateCopyPtr();
	}
	//------------------------------------------------------------
	void SceneParamsCollection::AddVehicleSnp( const traits::SceneNodeParams_traits::container::value_type &snpPair, bool replace )
	{
		if(replace) vehicleSnpContainer.erase( snpPair.first );
		vehicleSnpContainer.insert( snpPair );
	}
	//------------------------------------------------------------
	void SceneParamsCollection::AddSceneSnp( traits::SceneNodeParams_traits::in snp, bool replace /*= true */ )
	{
		if(replace) sceneSnpContainer.erase( snp->name );
		sceneSnpContainer.insert( traits::SceneNodeParams_traits::container::value_type( snp->name, snp ) );
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	void SceneParamsCollection::LoadScene( tixmlel_traits::in element, int loadOptions )
	{
		//Load Environment
		if( (loadOptions & LO_SKIP_ENVIRONMENT) == 0 )
		{
			tixmlel_traits::in envElement = element->FirstChildElement( EnvironmentKeys::ENVIRONMENT_FIELD_KEY );
			if( envElement != 0 )
			{
				LoadEnvironmentParams( envElement );
			}
		}

		//LoadScene
		if( (loadOptions & LO_SKIP_SCENENODES) == 0 )
		{
			tixmlel_traits::in nodesElement = element->FirstChildElement( NodeKeys::NODES_FIELD_KEY );
			if( nodesElement != 0 )
			{
				LoadSceneNodes( nodesElement, sceneSnpContainer, loadOptions );
			}
		}
	}
	//------------------------------------------------------------
	void SceneParamsCollection::LoadVehicles( tixmlel_traits::in element, int loadOptions )
	{
		if( (loadOptions & LO_SKIP_SCENENODES) == 0 )
		{
			tixmlel_traits::in nodesElement = element->FirstChildElement( NodeKeys::NODES_FIELD_KEY );
			if( nodesElement != 0 )
			{
				LoadSceneNodes( nodesElement, vehicleSnpContainer, loadOptions );
			}
		}
	}
	//------------------------------------------------------------
	SPC::traits::SceneNodeParams_traits::out SceneParamsCollection::LoadSceneNodeParams( tixmlel_traits::in xmlElement, int loadOptions )
	{
		typedef NodeKeys NK;
		
		traits::SceneNodeParams_traits::store_type params( new Core::SceneNodeParams() );
		params->name = XI::GetString( xmlElement, NK::NAME_KEY );

		string_traits::str elementName;
		tixmlel_traits::in childElement(0);
		while( childElement = XI::IterateChildElements( xmlElement, childElement ) )
		{
			elementName = childElement->Value();
			if( elementName == NK::POSITION_KEY )
				XI::GetVector3( childElement, params->position, Ogre::Vector3::ZERO );
			else if( elementName == NK::ROTATION_KEY )
				XI::GetQuaternion( childElement, params->orientation, Ogre::Quaternion::IDENTITY );
			else if( elementName == NK::SCALE_KEY )
				XI::GetVector3( childElement, params->scale, Ogre::Vector3::ZERO );
			else if( elementName == NK::ENTITY_ELEMENT_KEY )
			{
				traits::EntityParams_traits::store_type entity = LoadEntityParams( childElement );
				if( entity == 0 )continue;

				bool leaveEntity = listeners->AddingEntityParams(*entity);
				if( leaveEntity ) {
					if( (loadOptions & LO_REPLACE_DUPS) != 0 )
						params->objects.erase(entity);
					params->objects.insert( entity );
				}
			}
			else if( elementName == NK::NODE_ELEMENT_KEY )
			{
				bool leaveNode( true );
				traits::SceneNodeParams_traits::out node = LoadSceneNodeParams( childElement );
				if( node == 0 )continue;

				listener_traits::container::iterator iter = listeners.begin();
				listener_traits::container::iterator end_iter = listeners.end();
				for( ; iter != end_iter; ++iter )
				{
					if( (*iter)->AddingSceneNodeParams( *node ) == false )
						leaveNode = false;
				}

				if( leaveNode )params->nodes.insert( node );
			}
			else if( elementName == NK::CAMERA_ELEMENT_KEY )
			{
				bool leaveCamera( true );
				traits::CameraParams_traits::out camera = LoadCameraParams( childElement );
				if( camera == 0 )continue;

				listener_traits::container::iterator iter = listeners.begin();
				listener_traits::container::iterator end_iter = listeners.end();
				for( ; iter != end_iter; ++iter )
				{
					if( (*iter)->AddingCameraParams( *camera ) == false )
						leaveCamera = false;
				}

				if( leaveCamera )params->objects.insert( camera );
			}
			else if( elementName == NK::LIGHT_ELEMENT_KEY )
			{
				bool leaveLight( true );
				traits::LightParams_traits::out light = LoadLightParams( childElement );
				if( light == 0 )continue;

				listener_traits::container::iterator iter = listeners.begin();
				listener_traits::container::iterator end_iter = listeners.end();
				for( ; iter != end_iter; ++iter )
				{
					if( (*iter)->AddingLightParams( *light ) == false )
						leaveLight = false;
				}
				if( leaveLight )params->objects.insert( light );
			}
			/*
			else if( elementName == NK::PLANE_ELEMENT_KEY )
			{

			}
			*/
			/*
			else if( elementName == NK::USER_DATA_KEY )
			{
			}
			*/
		}
		return params;
	}
	//------------------------------------------------------------
	void SceneParamsCollection::LoadSceneNodes( tixmlel_traits::in xmlElement, traits::SceneNodeParams_traits::container &map, int loadOptions )
	{
		typedef NodeKeys NK;
		string_traits::str elementName;
		tixmlel_traits::in childElement(0);
		while( childElement = XI::IterateChildElements( xmlElement, childElement ) )
		{
			elementName = childElement->Value();
			if( elementName == NK::NODE_ELEMENT_KEY )
			{
				traits::SceneNodeParams_traits::out node = LoadSceneNodeParams( childElement );
				if( node == 0 )continue;

				bool leaveNode = listeners->AddingSceneNodeParams(*node);
				if( leaveNode )
				{
					if( (loadOptions & LO_REPLACE_DUPS) != 0 )
						map.erase( node->name );
					map.insert( traits::SceneNodeParams_traits::container::value_type( node->name, node ) );
				}
			}
		}
	}
	//------------------------------------------------------------
	void SceneParamsCollection::AddParamsFrom( tixmlel_traits::in element, int loadOptions )
	{
		typedef MainKeys MK;
		string_traits::str elementName;
		tixmlel_traits::in childElement(0);
		while( childElement = XI::IterateChildElements( element, childElement ) )
		{
			elementName = childElement->Value();
			if( elementName == MK::SCENE )
			{
				LoadScene( childElement, loadOptions );
			}
			else if( elementName == MK::VEHICLES )
			{
				LoadVehicles( childElement, loadOptions );
			}
		}
	}
	//------------------------------------------------------------
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML
}
}
#endif //DBS_LM_COLLECTIONS
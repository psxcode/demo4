#include "DemoStableHeaders.h"
#include "dsSceneTypes.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	string_traits::cstr dsKeys::SceneCommon::NODES_FIELD		= "nodes";
	string_traits::cstr dsKeys::SceneCommon::SCENE_FIELD		= "scene";
	string_traits::cstr dsKeys::SceneCommon::VEHICLES_FIELD		= "vehicles";
	string_traits::cstr dsKeys::SceneCommon::DOTSCENE_FIELD		= "dotscene";
	string_traits::cstr dsKeys::SceneCommon::ENTITIES_FIELD		= "entities";
	string_traits::cstr dsKeys::SceneCommon::LIGHTS_FIELD		= "lights";
	string_traits::cstr dsKeys::SceneCommon::CAMERAS_FIELD		= "cameras";
	string_traits::cstr dsKeys::SceneCommon::MESHLODS_FIELD		= "meshlods";
	string_traits::cstr dsKeys::SceneCommon::NODE_ELEMENT		= "node";
	string_traits::cstr dsKeys::SceneCommon::ENTITY_ELEMENT		= "entity";
	string_traits::cstr dsKeys::SceneCommon::CAMERA_ELEMENT		= "camera";
	string_traits::cstr dsKeys::SceneCommon::LIGHT_ELEMENT		= "light";
	string_traits::cstr dsKeys::SceneCommon::OBJECT_ELEMENT		= "object";
	string_traits::cstr dsKeys::SceneCommon::PLANE_ELEMENT		= "plane";
	string_traits::cstr dsKeys::SceneCommon::MESHLOD_ELEMENT	= "meshlod";
	string_traits::cstr dsKeys::SceneCommon::NAME				= "name";
	string_traits::cstr dsKeys::SceneCommon::TYPE				= "type";
	string_traits::cstr dsKeys::SceneCommon::MIN				= "min";
	string_traits::cstr dsKeys::SceneCommon::MAX				= "max";
	string_traits::cstr dsKeys::SceneCommon::BIAS				= "bias";
	string_traits::cstr dsKeys::SceneCommon::COPY				= "copy";
	string_traits::cstr dsKeys::SceneCommon::LOD				= "lod";
	string_traits::cstr dsKeys::SceneCommon::LOD_STATEGY		= "lodstrategy";
	string_traits::cstr dsKeys::SceneCommon::GROUP				= "group";
	string_traits::cstr dsKeys::SceneCommon::VALUE				= "value";
	//------------------------------------------------------------
	string_traits::cstr dsKeys::Node::POSITION				= "position";
	string_traits::cstr dsKeys::Node::ROTATION				= "rotation";
	string_traits::cstr dsKeys::Node::SCALE					= "scale";
	string_traits::cstr dsKeys::Node::NODE_VISIBLE			= "visible";
	string_traits::cstr dsKeys::Node::NODE_HIDDEN			= "hidden";
	string_traits::cstr dsKeys::Node::NODE_TREE_VISIBLE		= "treevisible";
	string_traits::cstr dsKeys::Node::NODE_TREE_HIDDEN		= "treehidden";
	string_traits::cstr dsKeys::Node::USER_DATA				= "userData";
	//------------------------------------------------------------
	string_traits::cstr dsKeys::Entity::MESH_FILE			= "meshfile";
	string_traits::cstr dsKeys::Entity::MATERIAL_FILE		= "materialfile";
	string_traits::cstr dsKeys::Entity::CAST_SHADOWS		= "castshadows";
	string_traits::cstr dsKeys::Entity::RECEIVE_SHADOWS		= "receiveshadows";
	string_traits::cstr dsKeys::Entity::RENDER_DISTANCE		= "renderingdistance";
	string_traits::cstr dsKeys::Entity::RENDER_QUEUE		= "renderqueue";
	string_traits::cstr dsKeys::Entity::SUBENTITIES_FIELD	= "subentities";
	string_traits::cstr dsKeys::Entity::SUBENTITY_ELEMENT	= "subentity";
	string_traits::cstr dsKeys::Entity::VERTEX_BUFFER		= "vertexbuffer";
	string_traits::cstr dsKeys::Entity::INDEX_BUFFER		= "indexbuffer";
	string_traits::cstr dsKeys::Entity::VISIBLE				= "visible";
	string_traits::cstr dsKeys::Entity::SUB_INDEX			= "index";
	string_traits::cstr dsKeys::Entity::SUB_MATERIAL_NAME	= "materialname";
	string_traits::cstr dsKeys::Entity::MESH_LOD_BIAS		= "meshlodbias";
	string_traits::cstr dsKeys::Entity::MATERIAL_LOD_BIAS	= "materiallodbias";
	//------------------------------------------------------------
	string_traits::cstr dsKeys::Light::CAST_SHADOWS			= "castshadows";
	string_traits::cstr dsKeys::Light::COLOUR_DIFFUSE		= "colourdiffuse";
	string_traits::cstr dsKeys::Light::COLOUR_SPECULAR		= "colourspecular";
	string_traits::cstr dsKeys::Light::POWER				= "power";
	string_traits::cstr dsKeys::Light::RANGE_INNER			= "inner";
	string_traits::cstr dsKeys::Light::RANGE_OUTER			= "outer";
	string_traits::cstr dsKeys::Light::RANGE_FALLOFF		= "falloff";
	string_traits::cstr dsKeys::Light::ATTENUATION			= "lightattenuation";
	string_traits::cstr dsKeys::Light::ATTENUATION_RANGE	= "range";
	string_traits::cstr dsKeys::Light::ATTENUATION_CONSTANT	= "constant";
	string_traits::cstr dsKeys::Light::ATTENUATION_LINEAR	= "linear";
	string_traits::cstr dsKeys::Light::ATTENUATION_QUADRIC	= "quadric";
	string_traits::cstr dsKeys::Light::TYPE_POINT			= "point";
	string_traits::cstr dsKeys::Light::TYPE_DIRECT			= "directional";
	string_traits::cstr dsKeys::Light::TYPE_SPOT			= "spot";
	string_traits::cstr dsKeys::Light::VISIBLE				= "visible";
	string_traits::cstr dsKeys::Light::POSITION				= "position";
	string_traits::cstr dsKeys::Light::RANGE				= "lightrange";
	string_traits::cstr dsKeys::Light::NORMAL				= "normal";
	string_traits::cstr dsKeys::Light::LIGHT_GEOM			= "lightgeom";
	//------------------------------------------------------------
	string_traits::cstr dsKeys::Camera::POSITION			= "position";
	string_traits::cstr dsKeys::Camera::ROTATION			= "rotation";
	string_traits::cstr dsKeys::Camera::NORMAL				= "normal";
	string_traits::cstr dsKeys::Camera::FOV					= "fov";
	string_traits::cstr dsKeys::Camera::ASPECT				= "aspect";
	string_traits::cstr dsKeys::Camera::VISIBLE				= "visible";
	string_traits::cstr dsKeys::Camera::CLIPPING			= "clipping";
	string_traits::cstr dsKeys::Camera::TYPE_PERSPECTIVE	= "perspective";
	string_traits::cstr dsKeys::Camera::TYPE_ORTHOGRAPHIC	= "orthographic";
	//------------------------------------------------------------
	string_traits::cstr dsKeys::HardwareBufferUsage::STATIC					= "static";
	string_traits::cstr dsKeys::HardwareBufferUsage::DYNAMIC				= "dynamic";
	string_traits::cstr dsKeys::HardwareBufferUsage::STATIC_WRITE_ONLY		= "staticwriteonly";
	string_traits::cstr dsKeys::HardwareBufferUsage::DYNAMIC_WRITE_ONLY		= "dynamicwriteonly";
	string_traits::cstr dsKeys::HardwareBufferUsage::WRITE_ONLY				= "writeonly";
	//------------------------------------------------------------
	string_traits::cstr dsKeys::TextureType::TYPE_1D		= "1d";
	string_traits::cstr dsKeys::TextureType::TYPE_2D		= "2d";
	string_traits::cstr dsKeys::TextureType::TYPE_3D		= "3d";
	string_traits::cstr dsKeys::TextureType::TYPE_CUBIC		= "cubic";
	//------------------------------------------------------------
	string_traits::cstr dsKeys::Shadow::COLOUR							= "colourshadow";
	string_traits::cstr dsKeys::Shadow::FAR_DISTANCE					= "fardistance";
	string_traits::cstr dsKeys::Shadow::SELF_SHADOW						= "selfshadow";
	string_traits::cstr dsKeys::Shadow::SHADOW_TECH						= "technique";
	string_traits::cstr dsKeys::Shadow::SHADOW_TEXTURES					= "shadowtextures";
	string_traits::cstr dsKeys::Shadow::TECH_STENCIL_ADDITIVE			= "stenciladditive";
	string_traits::cstr dsKeys::Shadow::TECH_STENCIL_MODULATIVE			= "stencilmodulative";
	string_traits::cstr dsKeys::Shadow::TECH_TEXTURE_ADDITIVE_INT		= "textureadditiveintegrated";
	string_traits::cstr dsKeys::Shadow::TECH_TEXTURE_ADDITIVE			= "textureadditive";
	string_traits::cstr dsKeys::Shadow::TECH_TEXTURE_MODULATIVE_INT		= "texturemodulativeintegrated";
	string_traits::cstr dsKeys::Shadow::TECH_TEXTURE_MODULATIVE			= "texturemodulative";
	string_traits::cstr dsKeys::Shadow::TEX_CASTER_MATERIAL				= "shadowCasterMaterial";
	string_traits::cstr dsKeys::Shadow::TEX_RECEIVER_MATERIAL			= "shadowReceiverMaterial";
	string_traits::cstr dsKeys::Shadow::TEX_COUNT						= "count";
	string_traits::cstr dsKeys::Shadow::TEX_FADE_START					= "fadestart";
	string_traits::cstr dsKeys::Shadow::TEX_FADE_END					= "fadeend";
	string_traits::cstr dsKeys::Shadow::TEX_OFFSET						= "offset";
	string_traits::cstr dsKeys::Shadow::TEX_SIZE						= "size";
	string_traits::cstr dsKeys::Shadow::CAMERA_SETUP					= "shadowcamerasetup";
	//------------------------------------------------------------
	string_traits::cstr dsKeys::Clipping::NEAR_CLIP	= "near";
	string_traits::cstr dsKeys::Clipping::FAR_CLIP	= "far";
	//------------------------------------------------------------
	string_traits::cstr dsKeys::Plane::PLANE_X = "planex";
	string_traits::cstr dsKeys::Plane::PLANE_Y = "planey";
	string_traits::cstr dsKeys::Plane::PLANE_Z = "planez";
	string_traits::cstr dsKeys::Plane::PLANE_D = "planed";
	string_traits::cstr dsKeys::Plane::X = "x";
	string_traits::cstr dsKeys::Plane::Y = "y";
	string_traits::cstr dsKeys::Plane::Z = "z";
	string_traits::cstr dsKeys::Plane::D = "d";
	//------------------------------------------------------------
	string_traits::cstr dsKeys::Environment::ENVIRONMENT_FIELD		= "environment";
	string_traits::cstr dsKeys::Environment::FOG					= "fog";
	string_traits::cstr dsKeys::Environment::SKY_BOX				= "skybox";
	string_traits::cstr dsKeys::Environment::SKY_DOME				= "skydome";
	string_traits::cstr dsKeys::Environment::SKY_PLANE				= "skyplane";
	string_traits::cstr dsKeys::Environment::CLIPPING				= "clipping";
	string_traits::cstr dsKeys::Environment::COLOUR_AMBIENT			= "colourambient";
	string_traits::cstr dsKeys::Environment::COLOUR_BACKGROUND		= "colourbackground";
	string_traits::cstr dsKeys::Environment::SHADOWS				= "shadows";
	//------------------------------------------------------------
	string_traits::cstr dsKeys::Sky::ENABLE			= "enable";
	string_traits::cstr dsKeys::Sky::MATERIAL		= "material";
	string_traits::cstr dsKeys::Sky::CURVATURE		= "curvature";
	string_traits::cstr dsKeys::Sky::DISTANCE		= "distance";
	string_traits::cstr dsKeys::Sky::DRAW_FIRST		= "drawFirst";
	string_traits::cstr dsKeys::Sky::ROTATION		= "rotation";
	string_traits::cstr dsKeys::Sky::TILING			= "tiling";
	string_traits::cstr dsKeys::Sky::BOW			= "bow";
	string_traits::cstr dsKeys::Sky::SCALE			= "scale";
	string_traits::cstr dsKeys::Sky::X_SEGMENTS		= "xsegments";
	string_traits::cstr dsKeys::Sky::Y_SEGMENTS		= "ysegments";
	//------------------------------------------------------------
	string_traits::cstr dsKeys::Fog::EXP_DENSITY		= "expdensity";
	string_traits::cstr dsKeys::Fog::LINEAR_START		= "linearstart";
	string_traits::cstr dsKeys::Fog::LINEAR_END			= "linearend";
	string_traits::cstr dsKeys::Fog::MODE				= "mode";
	string_traits::cstr dsKeys::Fog::MODE_EXP			= "exp";
	string_traits::cstr dsKeys::Fog::MODE_EXP2			= "exp2";
	string_traits::cstr dsKeys::Fog::MODE_LINEAR		= "linear";
	string_traits::cstr dsKeys::Fog::DIFFUSE_COLOUR		= "colourdiffuse";
	//------------------------------------------------------------
}
}
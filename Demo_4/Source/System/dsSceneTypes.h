#pragma once

#include <OgreVector3.h>
#include <OgreVector4.h>
#include <OgreLight.h>
#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgrePixelFormat.h>
#include <OgreResourceGroupManager.h>
#include <OgreTexture.h>

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	class _DemoExport dsMovableObject
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsMovableObject)
		//--------------------------------//
		enum OBJECT_TYPE
		{
			OT_NONE,
			OT_ENTITY,
			OT_LIGHT,
			OT_CAMERA,
			OT_PARTICLE_SYSTEM,
			OT_BILLBOARD_SET,
			OT_PLANE
		};
		//--------------------------------//
		enum OBJECT_VISIBILITY
		{
			OV_DEFAULT,
			OV_VISIBLE,
			OV_HIDDEN
		};
		//--------------------------------//
		virtual ~dsMovableObject()
		{
		}
		virtual dsMovableObject::shared_ptr CreateCopyPtr()const = 0;

		inline OBJECT_TYPE getType()const{return objectType;}

		OBJECT_VISIBILITY visibility;
		Ogre::uint8 renderQueue;
		Ogre::Real renderDistance;

		string_traits::str name;

	protected:
		explicit dsMovableObject( OBJECT_TYPE ot )
			: objectType( ot )
			, renderQueue( Ogre::RENDER_QUEUE_MAIN )
			, renderDistance( 0 )
			, visibility( OV_DEFAULT )
		{
		}

	private:
		OBJECT_TYPE objectType;
	};
	//------------------------------------------------------------
	class _DemoExport dsEntity : public dsMovableObject
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsEntity)
		//--------------------------------//
		typedef Demo::vector< string_traits::str >::type SubMatNameArray;
		//--------------------------------//
		struct LodBias
		{
			LodBias()
				: factor(1.0f)
				, min(99)
				, max(0)
			{
			}

			Ogre::uint8 max;
			Ogre::uint8 min;
			Ogre::Real factor;
		};
		//--------------------------------//
		dsEntity()
			: dsMovableObject( OT_ENTITY )
			, castShadows( true )
		{
		}
		virtual ~dsEntity()
		{
		}
		virtual dsMovableObject::shared_ptr CreateCopyPtr()const{ return CreateEntityCopyPtr(); }
		dsEntity::shared_ptr CreateEntityCopyPtr()const{ return dsEntity::shared_ptr( new dsEntity( *this ) ); }

		string_traits::str meshFilename;
		SubMatNameArray subMatNames;
		LodBias meshLodBias;
		LodBias materialLodBias;
		bool castShadows;
	};
	//------------------------------------------------------------
	class _DemoExport dsLight : public dsMovableObject
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsLight)
		//--------------------------------//
		struct LightGeom
		{
			string_traits::str meshName;
			dsEntity::LodBias lodBias;
		};
		//--------------------------------//
		dsLight()
			: dsMovableObject( OT_LIGHT )
			, lightType( Ogre::Light::LT_POINT )
			, castShadows( false )
			, power( 1.0f )
			, diffuseColour( Ogre::ColourValue::White )
			, specularColour( Ogre::ColourValue::White )
			, spotlightInnerAngle( Ogre::Degree( 40.0f ) )
			, spotlightOuterAngle( Ogre::Degree( 30.0f ) )
			, spotlightFalloff( 1.0f )
			, attenuationRange( 1000.0f )
			, attenuationConstant( 1.0f )
			, attenuationLinear( 0 )
			, attenuationQuadric( 0 )
			, position( Ogre::Vector3::ZERO )
			, direction( Ogre::Vector3::UNIT_Z )
		{
		}
		virtual ~dsLight()
		{
		}
		virtual dsMovableObject::shared_ptr CreateCopyPtr()const{ return CreateLightCopyPtr(); }
		dsLight::shared_ptr CreateLightCopyPtr()const{ return dsLight::shared_ptr( new dsLight( *this ) ); }

		Ogre::Light::LightTypes lightType;
		bool castShadows;
		Ogre::Real power;

		Ogre::ColourValue diffuseColour;
		Ogre::ColourValue specularColour;

		Ogre::Radian spotlightInnerAngle;
		Ogre::Radian spotlightOuterAngle;
		Ogre::Real spotlightFalloff;

		Ogre::Real attenuationRange;
		Ogre::Real attenuationConstant;
		Ogre::Real attenuationLinear;
		Ogre::Real attenuationQuadric;

		Ogre::Vector3 position;
		Ogre::Vector3 direction;

		LightGeom lightGeom;
	};
	//------------------------------------------------------------
	class _DemoExport dsCamera : public dsMovableObject
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsCamera)
		//--------------------------------//
		dsCamera()
			: dsMovableObject( OT_CAMERA )
			, fov( Ogre::Degree( 45.0f ) )
			, aspectRatio( 1.33f )
			, projectionType( Ogre::PT_PERSPECTIVE )
			, nearClip( 5.0f )
			, farClip( 10000.0f )
			, position( Ogre::Vector3::ZERO )
			, orientation( Ogre::Quaternion::IDENTITY )
			, direction( Ogre::Vector3::NEGATIVE_UNIT_Z )
		{
		}
		virtual ~dsCamera()
		{
		}
		virtual dsMovableObject::shared_ptr CreateCopyPtr()const{ return CreateCameraCopyPtr(); }
		dsCamera::shared_ptr CreateCameraCopyPtr()const{ return dsCamera::shared_ptr( new dsCamera( *this ) ); }

		Ogre::Radian fov;
		Ogre::Real aspectRatio;
		Ogre::ProjectionType projectionType;

		Ogre::Real nearClip;
		Ogre::Real farClip;

		Ogre::Vector3 position;
		Ogre::Quaternion orientation;
		Ogre::Vector3 direction;
	};
	//------------------------------------------------------------
	class _DemoExport dsSceneNode
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsSceneNode)
		//--------------------------------//
		enum NODE_VISIBILITY
		{
			NV_DEFAULT,
			NV_VISIBLE,
			NV_HIDDEN,
			NV_TREE_VISIBLE,
			NV_TREE_HIDDEN
		};
		//--------------------------------//
		class traits
		{
		public:
			typedef dsSceneNode::shared_ptr sn_store_t;
			typedef Demo::set< sn_store_t >::type sn_cont;
			typedef Ogre::SetIterator< sn_cont > sn_oit;
			typedef Ogre::ConstSetIterator< sn_cont > sn_ocit;
			typedef type_traits< sn_store_t >::cref sn_ar;

			typedef std::pair< string_traits::str, dsMovableObject::OBJECT_TYPE > obj_store_t;
			typedef Demo::set< obj_store_t >::type obj_cont;
			typedef Ogre::SetIterator< obj_cont > obj_oit;
			typedef Ogre::ConstSetIterator< obj_cont > obj_ocit;
			typedef type_traits< obj_store_t >::cref obj_ar;
		};
		//--------------------------------//
		~dsSceneNode()
		{
		}

		dsSceneNode()
			: position( Ogre::Vector3::ZERO )
			, orientation( Ogre::Quaternion::IDENTITY )
			, scale( Ogre::Vector3::UNIT_SCALE )
		{
		}

		dsSceneNode(const dsSceneNode &node)
		{
			*this = node;
		}

		dsSceneNode& operator=( const dsSceneNode& node )
		{
			if( this == &node )
				return *this;

			nodes.clear();
			objects.clear();

			name = node.name;
			position = node.position;
			orientation = node.orientation;
			scale = node.scale;

			{
				objects = node.objects;
			}
			{
				auto iter = node.getSceneNodeIterator();
				while(iter.hasMoreElements())
					nodes.insert( (iter.getNextCRef())->CreateCopyPtr() );
			}
			return *this;
		}

		dsSceneNode::shared_ptr CreateCopyPtr()const
		{
			return dsSceneNode::shared_ptr( new dsSceneNode(*this) );
		}

		inline void addSceneNode( traits::sn_ar snadd ) { nodes.insert( snadd ); }
		inline void addObject( traits::obj_ar objadd ) { objects.insert( objadd ); }

		inline traits::sn_oit getSceneNodeIterator() { return traits::sn_oit( nodes ); }
		inline traits::sn_ocit getSceneNodeIterator()const { return traits::sn_ocit( nodes ); }
		inline traits::obj_oit getObjectIterator() {return traits::obj_oit(objects);}
		inline traits::obj_ocit getObjectIterator()const {return traits::obj_ocit(objects);}

		traits::sn_cont nodes;
		traits::obj_cont objects;

		Ogre::Vector3 position;
		Ogre::Quaternion orientation;
		Ogre::Vector3 scale;

		string_traits::str name;
	};
	//------------------------------------------------------------
	class dsMeshLod
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsMeshLod)
		//--------------------------------//
		struct MeshName
		{
			string_traits::str name;
			string_traits::str group;
		};
		//--------------------------------//
		typedef Demo::map< Ogre::uint32, MeshName >::type lod_cont;
		//--------------------------------//
		dsMeshLod()
			: meshResourceState(0)
		{
		}

		dsMeshLod::shared_ptr CreateCopyPtr()const {return dsMeshLod::shared_ptr(new dsMeshLod(*this));}

		MeshName meshName;
		string_traits::str lodStrategy;

		lod_cont lods;

		//helper
		size_t meshResourceState;
	};
	//------------------------------------------------------------
	/*
	class _DemoExport dsSceneNode
	{
	public:
		DEFINE_CLASS_SHARED_PTR(dsSceneNode)
		//--------------------------------//
		enum NODE_VISIBILITY
		{
			NV_DEFAULT,
			NV_VISIBLE,
			NV_HIDDEN,
			NV_TREE_VISIBLE,
			NV_TREE_HIDDEN
		};
		enum COPY_OPTIONS
		{
			CO_NO_OPTIONS = 0x0,
			CO_SKIP_ENTITIES = 0x10,
			CO_SKIP_LIGHTS = 0x20,
			CO_SKIP_CAMERAS = 0x40
		};
		//--------------------------------//
		class traits
		{
		public:
			typedef dsSceneNode::shared_ptr sn_store_t;
			typedef dsMovableObject::shared_ptr obj_store_t;

			typedef Demo::set< sn_store_t >::type sn_cont;
			typedef Demo::set< obj_store_t >::type obj_cont;

			typedef Ogre::SetIterator< sn_cont > sn_oit;
			typedef Ogre::ConstSetIterator< sn_cont > sn_ocit;
			typedef Ogre::SetIterator< obj_cont > obj_oit;
			typedef Ogre::ConstSetIterator< obj_cont > obj_ocit;

			typedef type_traits< sn_store_t >::cref sn_ar;
			typedef type_traits< obj_store_t >::cref obj_ar;
		};
		//--------------------------------//
		~dsSceneNode()
		{
		}

		dsSceneNode()
			: visibility( NV_DEFAULT )
			, position( Ogre::Vector3::ZERO )
			, orientation( Ogre::Quaternion::IDENTITY )
			, scale( Ogre::Vector3::UNIT_SCALE )
		{
		}

		dsSceneNode( const dsSceneNode& node )
			: name( node.name )
			, visibility( node.visibility )
			, position( node.position )
			, orientation( node.orientation )
			, scale( node.scale )
		{
			traits::obj_ocit obj_ocit = node.getObjOgreIterator();
			while( obj_ocit.hasMoreElements() )
				objects.insert( obj_ocit.getNextCRef()->CreateCopyPtr() );

			traits::sn_ocit sn_ocit = node.getSceneNodeIterator();
			while( sn_ocit.hasMoreElements() )
				nodes.insert( sn_ocit.getNextCRef()->CreateCopyPtr() );
		}

		dsSceneNode( const dsSceneNode& node, int copyOptions )
			: name( node.name )
			, visibility( node.visibility )
			, position( node.position )
			, orientation( node.orientation )
			, scale( node.scale )
		{
			traits::obj_ocit obj_ocit = node.getObjOgreIterator();
			while( obj_ocit.hasMoreElements() ) {
				dsMovableObject::shared_ptr const &mo = obj_ocit.getNextCRef();
				switch( mo->getType() ) {
				case dsMovableObject::OT_ENTITY: if( (copyOptions & CO_SKIP_ENTITIES) != 0 )continue; break;
				case dsMovableObject::OT_LIGHT: if( (copyOptions & CO_SKIP_LIGHTS) != 0 )continue; break;
				case dsMovableObject::OT_CAMERA: if( (copyOptions & CO_SKIP_CAMERAS) != 0 )continue; break;
				}
				objects.insert( mo->CreateCopyPtr() );
			}

			traits::sn_ocit sn_ocit = node.getSceneNodeIterator();
			while( sn_ocit.hasMoreElements() )
				nodes.insert( sn_ocit.getNextCRef()->CreateCopyPtr(copyOptions) );
		}

		dsSceneNode& operator=( const dsSceneNode& node )
		{
			if( this == &node )
				return *this;

			nodes.clear();
			objects.clear();

			name = node.name;
			visibility = node.visibility;
			position = node.position;
			orientation = node.orientation;
			scale = node.scale;

			{
				traits::obj_cont::const_iterator iter = node.objects.begin();
				traits::obj_cont::const_iterator end_iter = node.objects.end();
				for( ; iter != end_iter; ++iter )
					objects.insert( (*iter)->CreateCopyPtr() );
			}
			{
				traits::sn_cont::const_iterator iter = node.nodes.begin();
				traits::sn_cont::const_iterator end_iter = node.nodes.end();
				for( ; iter != end_iter; ++iter )
					nodes.insert( (*iter)->CreateCopyPtr() );
			}
			return *this;
		}

		dsSceneNode::shared_ptr CreateCopyPtr( int copyOptions = CO_NO_OPTIONS )const{ return dsSceneNode::shared_ptr( new dsSceneNode( *this, copyOptions ) ); }

		//Name collisions are solved in collection
		inline void addSceneNode( traits::sn_ar snadd ) { nodes.insert( snadd ); }
		inline void addObject( traits::obj_ar objadd ) { objects.insert( objadd ); }

		inline traits::sn_oit getSceneNodeIterator() { return traits::sn_oit( nodes ); }
		inline traits::sn_ocit getSceneNodeIterator()const { return traits::sn_ocit( nodes ); }
		inline traits::obj_oit getObjOgreIterator() { return traits::obj_oit( objects ); }
		inline traits::obj_ocit getObjOgreIterator()const { return traits::obj_ocit( objects ); }

		traits::sn_cont nodes;
		traits::obj_cont objects;
		NODE_VISIBILITY visibility;

		Ogre::Vector3 position;
		Ogre::Quaternion orientation;
		Ogre::Vector3 scale;

		string_traits::str name;
	};
	*/
	//------------------------------------------------------------
	/*
	struct _DemoExport Fog
	{
		Fog()
			: mode( Ogre::FOG_NONE )
			, expDensity( 0.001f )
			, linearStart( 0.0f )
			, linearEnd( 1.0f )
			, colour( Ogre::ColourValue::White )
		{
		}
		Ogre::FogMode mode;
		Ogre::Real expDensity;
		Ogre::Real linearStart;
		Ogre::Real linearEnd;
		Ogre::ColourValue colour;
	};
	//------------------------------------------------------------
	struct _DemoExport Sky
	{
		//--------------------------------//
		Sky()
			: enabled( false )
			, drawFirst( true )
			, distance( 0.0f )
			, rotation( Ogre::Quaternion::IDENTITY )
			, resGroupName( Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME )
		{
		}

		bool enabled;
		bool drawFirst;
		Ogre::Real distance;
		Ogre::Quaternion rotation;
		string_traits::str materialName;
		string_traits::str resGroupName;
	};
	//------------------------------------------------------------
	typedef Sky SkyBox;
	//------------------------------------------------------------
	struct _DemoExport SkyDome : Sky
	{
		SkyDome()
			: curvature( 0.0f )
			, tiling( 0.0f )
			, xSegments( 0 )
			, ySegments( 0 )
		{
		}
		Ogre::Real curvature;
		Ogre::Real tiling;
		int xSegments;
		int ySegments;
	};
	//------------------------------------------------------------
	struct _DemoExport SkyPlane : Sky
	{
		SkyPlane()
			: xSegments( 1 )
			, ySegments( 1 )
			, scale( 1.0f )
			, bow( 0.0f )
			, tiling( 1.0f )
		{
		}
		Ogre::Plane plane;
		Ogre::Real scale;
		Ogre::Real bow;
		Ogre::Real tiling;
		int xSegments;
		int ySegments;
	};
	//------------------------------------------------------------
	struct _DemoExport Shadow
	{
		Shadow()
			: technique( Ogre::SHADOWTYPE_NONE )
			, selfShadow( false )
			, farDistance( 100.0f )
			, texSize( 128 )
			, texCount( 1 )
			, texOffset( 0.0f )
			, texFadeStart( 0.0f )
			, texFadeEnd( 1.0f )
			, pixelFormat( Ogre::PF_UNKNOWN )
			, colour( Ogre::ColourValue::Black )
		{
		}
		Ogre::ShadowTechnique technique;
		bool selfShadow;
		Ogre::Real farDistance;
		int texSize;
		int texCount;
		Ogre::Real texOffset;
		Ogre::Real texFadeStart;
		Ogre::Real texFadeEnd;
		string_traits::str texShadowCasterMaterial;
		string_traits::str texShadowReceiverMaterial;
		Ogre::PixelFormat pixelFormat;
		Ogre::ColourValue colour;
		string_traits::str cameraSetup;
		Ogre::Plane optimalPlane;
	};
	//------------------------------------------------------------
	struct _DemoExport Environment
	{
		Environment()
			: skyBoxLoaded( false )
			, skyDomeLoaded( false )
			, skyPlaneLoaded( false )
			, nearClip( 0 )
			, farClip( 100000 )
			, ambientColour( Ogre::ColourValue::Black )
			, backgroundColour( Ogre::ColourValue::Black )
		{
		}
		Fog fog;

		bool skyBoxLoaded;
		bool skyDomeLoaded;
		bool skyPlaneLoaded;
		SkyBox skyBox;
		SkyDome skyDome;
		SkyPlane skyPlane;

		Ogre::Real nearClip;
		Ogre::Real farClip;
		Ogre::ColourValue ambientColour;
		Ogre::ColourValue backgroundColour;

		Shadow shadow;
	};
	//------------------------------------------------------------
	*/
namespace dsKeys
{
	//------------------------------------------------------------
	struct Node
	{
		static string_traits::cstr POSITION;
		static string_traits::cstr ROTATION;
		static string_traits::cstr SCALE;
		static string_traits::cstr NODE_VISIBLE;
		static string_traits::cstr NODE_HIDDEN;
		static string_traits::cstr NODE_TREE_VISIBLE;
		static string_traits::cstr NODE_TREE_HIDDEN;
		static string_traits::cstr USER_DATA;
	};
	//------------------------------------------------------------
	struct Entity
	{
		static string_traits::cstr SUBENTITIES_FIELD;
		static string_traits::cstr SUBENTITY_ELEMENT;
		static string_traits::cstr MESH_FILE;
		static string_traits::cstr MATERIAL_FILE;
		static string_traits::cstr CAST_SHADOWS;
		static string_traits::cstr RECEIVE_SHADOWS;
		static string_traits::cstr RENDER_QUEUE;
		static string_traits::cstr RENDER_DISTANCE;
		static string_traits::cstr VERTEX_BUFFER;
		static string_traits::cstr INDEX_BUFFER;
		static string_traits::cstr VISIBLE;
		static string_traits::cstr SUB_MATERIAL_NAME;
		static string_traits::cstr SUB_INDEX;
		static string_traits::cstr MESH_LOD_BIAS;
		static string_traits::cstr MATERIAL_LOD_BIAS;
	};
	//------------------------------------------------------------
	struct Light
	{
		static string_traits::cstr TYPE_POINT;
		static string_traits::cstr TYPE_SPOT;
		static string_traits::cstr TYPE_DIRECT;
		static string_traits::cstr COLOUR_DIFFUSE;
		static string_traits::cstr COLOUR_SPECULAR;
		static string_traits::cstr RANGE;
		static string_traits::cstr RANGE_INNER;
		static string_traits::cstr RANGE_OUTER;
		static string_traits::cstr RANGE_FALLOFF;
		static string_traits::cstr ATTENUATION;
		static string_traits::cstr ATTENUATION_RANGE;
		static string_traits::cstr ATTENUATION_CONSTANT;
		static string_traits::cstr ATTENUATION_LINEAR;
		static string_traits::cstr ATTENUATION_QUADRIC;
		static string_traits::cstr POSITION;
		static string_traits::cstr NORMAL;
		static string_traits::cstr VISIBLE;
		static string_traits::cstr CAST_SHADOWS;
		static string_traits::cstr POWER;
		static string_traits::cstr LIGHT_GEOM;
	};
	//------------------------------------------------------------
	struct Camera
	{
		static string_traits::cstr VISIBLE;
		static string_traits::cstr FOV;
		static string_traits::cstr ASPECT;
		static string_traits::cstr TYPE_PERSPECTIVE;
		static string_traits::cstr TYPE_ORTHOGRAPHIC;
		static string_traits::cstr CLIPPING;
		static string_traits::cstr POSITION;
		static string_traits::cstr ROTATION;
		static string_traits::cstr NORMAL;
	};
	//------------------------------------------------------------
	struct HardwareBufferUsage
	{
		static string_traits::cstr STATIC;
		static string_traits::cstr DYNAMIC;
		static string_traits::cstr WRITE_ONLY;
		static string_traits::cstr STATIC_WRITE_ONLY;
		static string_traits::cstr DYNAMIC_WRITE_ONLY;
	};
	//------------------------------------------------------------
	struct TextureType
	{
		static string_traits::cstr TYPE_1D;
		static string_traits::cstr TYPE_2D;
		static string_traits::cstr TYPE_3D;
		static string_traits::cstr TYPE_CUBIC;
	};
	//------------------------------------------------------------
	struct Shadow
	{
		static string_traits::cstr SHADOW_TECH;
		static string_traits::cstr TECH_STENCIL_MODULATIVE;
		static string_traits::cstr TECH_STENCIL_ADDITIVE;
		static string_traits::cstr TECH_TEXTURE_MODULATIVE;
		static string_traits::cstr TECH_TEXTURE_ADDITIVE;
		static string_traits::cstr TECH_TEXTURE_MODULATIVE_INT;
		static string_traits::cstr TECH_TEXTURE_ADDITIVE_INT;
		static string_traits::cstr SHADOW_TEXTURES;
		static string_traits::cstr COLOUR;
		static string_traits::cstr FAR_DISTANCE;
		static string_traits::cstr SELF_SHADOW;
		static string_traits::cstr TEX_SIZE;
		static string_traits::cstr TEX_COUNT;
		static string_traits::cstr TEX_OFFSET;
		static string_traits::cstr TEX_FADE_START;
		static string_traits::cstr TEX_FADE_END;
		static string_traits::cstr TEX_CASTER_MATERIAL;
		static string_traits::cstr TEX_RECEIVER_MATERIAL;
		static string_traits::cstr CAMERA_SETUP;
	};
	//------------------------------------------------------------
	struct Clipping
	{
		static string_traits::cstr NEAR_CLIP;
		static string_traits::cstr FAR_CLIP;
	};
	//------------------------------------------------------------
	struct Plane
	{
		static string_traits::cstr PLANE_X;
		static string_traits::cstr PLANE_Y;
		static string_traits::cstr PLANE_Z;
		static string_traits::cstr PLANE_D;
		static string_traits::cstr X;
		static string_traits::cstr Y;
		static string_traits::cstr Z;
		static string_traits::cstr D;
	};
	//------------------------------------------------------------
	struct Environment
	{
		static string_traits::cstr ENVIRONMENT_FIELD;
		static string_traits::cstr FOG;
		static string_traits::cstr SKY_BOX;
		static string_traits::cstr SKY_DOME;
		static string_traits::cstr SKY_PLANE;
		static string_traits::cstr CLIPPING;
		static string_traits::cstr COLOUR_AMBIENT;
		static string_traits::cstr COLOUR_BACKGROUND;
		static string_traits::cstr SHADOWS;
	};
	//------------------------------------------------------------
	struct Fog
	{
		static string_traits::cstr MODE;
		static string_traits::cstr MODE_EXP;
		static string_traits::cstr MODE_EXP2;
		static string_traits::cstr MODE_LINEAR;
		static string_traits::cstr EXP_DENSITY;
		static string_traits::cstr LINEAR_START;
		static string_traits::cstr LINEAR_END;
		static string_traits::cstr DIFFUSE_COLOUR;
	};
	//------------------------------------------------------------
	struct Sky
	{
		static string_traits::cstr ENABLE;
		static string_traits::cstr DRAW_FIRST;
		static string_traits::cstr DISTANCE;
		static string_traits::cstr ROTATION;
		static string_traits::cstr MATERIAL;
		static string_traits::cstr TILING;
		static string_traits::cstr CURVATURE;
		static string_traits::cstr X_SEGMENTS;
		static string_traits::cstr Y_SEGMENTS;
		static string_traits::cstr SCALE;
		static string_traits::cstr BOW;
	};
	//------------------------------------------------------------
	struct SceneCommon
	{
		static string_traits::cstr NAME;
		static string_traits::cstr TYPE;
		static string_traits::cstr GROUP;
		static string_traits::cstr COPY;
		static string_traits::cstr MIN;
		static string_traits::cstr MAX;
		static string_traits::cstr BIAS;
		static string_traits::cstr LOD;
		static string_traits::cstr LOD_STATEGY;
		static string_traits::cstr VALUE;
		static string_traits::cstr NODES_FIELD;
		static string_traits::cstr SCENE_FIELD;
		static string_traits::cstr VEHICLES_FIELD;
		static string_traits::cstr DOTSCENE_FIELD;
		static string_traits::cstr ENTITIES_FIELD;
		static string_traits::cstr LIGHTS_FIELD;
		static string_traits::cstr CAMERAS_FIELD;
		static string_traits::cstr MESHLODS_FIELD;

		static string_traits::cstr NODE_ELEMENT;
		static string_traits::cstr ENTITY_ELEMENT;
		static string_traits::cstr CAMERA_ELEMENT;
		static string_traits::cstr LIGHT_ELEMENT;
		static string_traits::cstr PLANE_ELEMENT;
		static string_traits::cstr OBJECT_ELEMENT;
		static string_traits::cstr MESHLOD_ELEMENT;
	};
	//------------------------------------------------------------
}
}
}
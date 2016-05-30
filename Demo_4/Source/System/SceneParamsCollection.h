#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_COLLECTIONS

#include <OgreSingleton.h>

#if DBS_LM_SCRIPT_OGREMAX_TIXML
#	include "System\OgremaxTinyXmlInterface.h"
#	include "Core\OgremaxTypes.h"
#endif //DBS_LM_SCRIPT_OGREMAX_TIXML

#include "collection_traits.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	class SceneParamsCollection : public Ogre::Singleton< SceneParamsCollection >
	{
		friend class Ogre::Singleton< SceneParamsCollection >;
	public:
		//--------------------------------//
		enum CREATE_OPTIONS
		{
			CO_NO_OPTION = 0,
			CO_CACHE = 0x1,
			CO_CACHE_REPLACE = 0x2 | CO_CACHE,
			CO_SKIP_ENVIRONMENT = 0x10,
			CO_SKIP_SHADOWS = 0x20,
			CO_SKIP_SKY = 0x40,
			CO_SKIP_NODES = 0x100,
			CO_SKIP_LIGHTS = 0x200,
			CO_SKIP_CAMERAS = 0x400,
			CO_SKIP_ENTITIES = 0x800
		};
		//--------------------------------//
		typedef Demo::string_traits string_traits;
		//--------------------------------//
#	if DBS_LM_SCRIPT_OGREMAX_TIXML
		typedef OgremaxTinyXmlInterface::traits< TiXmlElement > tixmlel_traits;
		//--------------------------------//
		enum LOAD_OPTIONS
		{
			LO_NO_OPTION = 0x0,
			LO_REPLACE_DUPS = 0x1,
			LO_SKIP_VEHICLES = 0x10,
			LO_SKIP_SCENE = 0x20,

			LO_SKIP_SCENENODES = 0x100,
				LO_SKIP_ENTITIES = 0x200,
				LO_SKIP_CAMERAS = 0x400,
				LO_SKIP_LIGHTS = 0x800,
			
			LO_SKIP_ENVIRONMENT = 0x10000,
				LO_SKIP_SHADOWS = 0x20000,
				LO_SKIP_SKY = 0x40000
		};
#	endif //DBS_LM_SCRIPT_OGREMAX_TIXML
		//--------------------------------//
		class Listener
		{
		public:
			//--------------------------------//
			typedef shared_ptr_traits< Listener >::shared_ptr shared_ptr;
			typedef shared_ptr_traits< Listener >::const_shared_ptr const_shared_ptr;
			//--------------------------------//
#		if DBS_LM_SCRIPT_OGREMAX_TIXML
			virtual bool AddingEntityParams( Core::EntityParams& ){ return true; }
			virtual bool AddingCameraParams( Core::CameraParams& ){ return true; }
			virtual bool AddingLightParams( Core::LightParams& ){ return true; }
			virtual bool AddingSceneNodeParams( Core::SceneNodeParams& ){ return true; }
#		endif //DBS_LM_SCRIPT_OGREMAX_TIXML
		};
		//--------------------------------//
		class ListenerComposite : public Listener
		{
		public:
			//--------------------------------//
			typedef Listener::shared_ptr store_type;
			typedef Demo::set< store_type >::type container;
			typedef type_traits< store_type >::ref addremove;
			//--------------------------------//
			inline void addListener( addremove lis ) { listeners.insert(lis); }
			inline void removeListener( addremove lis ) { listeners.erase(lis); }

			virtual bool AddingEntityParams( Core::EntityParams &ent ) {
				Ogre::SetIterator<container> i(listeners);
				while(i.hasMoreElements())
					if(!i.getNext()->AddingEntityParams(ent))return false;
				return true;
			}
			virtual bool AddingCameraParams( Core::CameraParams &cam ) {
				Ogre::SetIterator<container> i(listeners);
				while(i.hasMoreElements())
					if(!i.getNext()->AddingCameraParams(cam))return false;
				return true;
			}
			virtual bool AddingLightParams( Core::LightParams& light ) {
				Ogre::SetIterator<container> i(listeners);
				while(i.hasMoreElements())
					if(!i.getNext()->AddingLightParams(light))return false;
				return true;
			}
			virtual bool AddingSceneNodeParams( Core::SceneNodeParams& snp ) {
				Ogre::SetIterator<container> i(listeners);
				while(i.hasMoreElements())
					if(!i.getNext()->AddingSceneNodeParams(snp))return false;
				return true;
			}

		protected:
			container listeners;
		};
		//--------------------------------//
		class traits
		{
		public:
			//--------------------------------//
			enum COST
			{
				COST_OGRE_SCENEOBJ = COST_PTR,
				COST_PARAMS = COST_BUILTIN_SHARED_PTR
			};
			//--------------------------------//
			typedef collection_traits< Ogre::SceneNode, COST_OGRE_SCENEOBJ > OgreSceneNode_traits;
			typedef collection_traits< Ogre::MovableObject, COST_OGRE_SCENEOBJ > OgreMovableObject_traits;
			typedef collection_traits< Ogre::Entity, COST_OGRE_SCENEOBJ > OgreEntity_traits;
			typedef collection_traits< Ogre::Camera, COST_OGRE_SCENEOBJ > OgreCamera_traits;
			typedef collection_traits< Ogre::Light, COST_OGRE_SCENEOBJ > OgreLight_traits;
			typedef collection_traits< Ogre::Plane, COST_OGRE_SCENEOBJ > OgrePlane_traits;
#		if DBS_LM_SCRIPT_OGREMAX_TIXML
			typedef collection_traits< Core::SceneNodeParams, COST_PARAMS > SceneNodeParams_traits;
			typedef collection_traits< Core::EntityParams, COST_PARAMS > EntityParams_traits;
			typedef collection_traits< Core::CameraParams, COST_PARAMS > CameraParams_traits;
			typedef collection_traits< Core::LightParams, COST_PARAMS > LightParams_traits;
			typedef collection_traits< Core::ObjectParams, COST_PARAMS > ObjectParams_traits;
#		endif //DBS_LM_SCRIPT_OGREMAX_TIXML
		};
		//--------------------------------//
		static SceneParamsCollection& getSingleton();
		static SceneParamsCollection* getSingletonPtr();
		//--------------------------------//
#	if DBS_LM_SCRIPT_OGREMAX_TIXML
		static traits::OgreSceneNode_traits::out	CreateSceneNode( const Core::SceneNodeParams &in, void* ownerObj, int createOptions = CO_NO_OPTION );
		static traits::OgreEntity_traits::out		CreateEntity( const Core::EntityParams &in, void* ownerObj );
		static traits::OgreCamera_traits::out		CreateCamera( const Core::CameraParams &in, void* ownerObj );
		static traits::OgreLight_traits::out		CreateLight( const Core::LightParams &in, void* ownerObj );

		static traits::SceneNodeParams_traits::out		CreateSceneNodeParams( traits::OgreSceneNode_traits::in, int createOptions = CO_NO_OPTION );
		static traits::EntityParams_traits::out			CreateEntityParams( traits::OgreEntity_traits::in );
		static traits::CameraParams_traits::out			CreateCameraParams( traits::OgreCamera_traits::in );
		static traits::LightParams_traits::out			CreateLightParams( traits::OgreLight_traits::in );
		static traits::ObjectParams_traits::out			CreateObjectParams( traits::OgreMovableObject_traits::in, int createOptions = CO_NO_OPTION );

		static Core::ObjectParams::OBJECT_VISIBILITY	ParseObjectVisibility( string_traits::in );
		static Core::SceneNodeParams::NODE_VISIBILITY	ParseNodeVisibility( string_traits::in );
		static Ogre::TextureType						ParseTextureType( string_traits::in texTypeStr, Ogre::TextureType defaultType = Ogre::TEX_TYPE_2D );
		static Ogre::PixelFormat						ParsePixelFormat( string_traits::in pixFormatStr, Ogre::PixelFormat defaultValue = Ogre::PF_A8R8G8B8 );
		static Ogre::ShadowTechnique					ParseShadowTechnique( string_traits::in stStr );
		static Ogre::Light::LightTypes					ParseLightType( string_traits::in typeStr, Ogre::Light::LightTypes defaultType = Ogre::Light::LT_POINT );
		static Ogre::ProjectionType						ParseProjectionType( string_traits::in projStr, Ogre::ProjectionType defaultValue = Ogre::PT_PERSPECTIVE );
		static Ogre::uint8								ParseRenderQueue( string_traits::in renderQueueStr, Ogre::uint8 defaultQueue = Ogre::RENDER_QUEUE_MAIN );
		static Ogre::FogMode							ParseFogMode( string_traits::in fogModeStr, Ogre::FogMode defaultMode = Ogre::FOG_NONE );
		static Ogre::HardwareBuffer::Usage				ParseHardwareBufferUsage( string_traits::in HWBUsageStr, Ogre::HardwareBuffer::Usage defaultUsage );

		static void SetSceneNodeVisibility( traits::OgreSceneNode_traits::inout, Core::SceneNodeParams::NODE_VISIBILITY );
#	endif //DBS_LM_SCRIPT_OGREMAX_TIXML
		//--------------------------------//

		inline void AddListener( ListenerComposite::addremove listener ) { listeners.addListener(listener); }
		inline void RemoveListener( ListenerComposite::addremove listener ) { listeners.removeListener(listener); }

#	if DBS_LM_SCRIPT_OGREMAX_TIXML
		void AddParamsFrom( tixmlel_traits::in, int loadOptions );
		
		traits::SceneNodeParams_traits::out GetVehicleSnp( string_traits::in name )const;
		traits::SceneNodeParams_traits::out GetSceneSnp( string_traits::in name )const;

		void AddSceneSnp( traits::SceneNodeParams_traits::in snp, bool replace = true );
		void AddVehicleSnp( const traits::SceneNodeParams_traits::container::value_type& snpPair, bool replace = true );
#	endif //DBS_LM_SCRIPT_OGREMAX_TIXML

	private:
#	if DBS_LM_SCRIPT_OGREMAX_TIXML
		void LoadFogParams( tixmlel_traits::in xmlElement );
		void LoadSkyBoxParams( tixmlel_traits::in xmlElement );
		void LoadSkyDomeParams( tixmlel_traits::in xmlElement );
		void LoadSkyPlaneParams( tixmlel_traits::in xmlElement );
		void LoadEnvironmentParams( tixmlel_traits::in xmlElement, int loadOptions = LO_NO_OPTION );
		void LoadShadowParams( tixmlel_traits::in xmlElement );
		void LoadVehicles( tixmlel_traits::in xmlElement, int loadOptions = LO_NO_OPTION );
		void LoadScene( tixmlel_traits::in xmlElement, int loadOptions = LO_NO_OPTION );
		void LoadSceneNodes( tixmlel_traits::in xmlElement, traits::SceneNodeParams_traits::container& map, int loadOptions = LO_NO_OPTION );

		traits::SceneNodeParams_traits::out LoadSceneNodeParams( tixmlel_traits::in xmlElement, int loadOptions = LO_NO_OPTION );
		traits::EntityParams_traits::out LoadEntityParams( tixmlel_traits::in xmlElement );
		traits::LightParams_traits::out LoadLightParams( tixmlel_traits::in xmlElement );
		traits::CameraParams_traits::out LoadCameraParams( tixmlel_traits::in xmlElement );
		void LoadSubEntityParams( tixmlel_traits::in element, Core::EntityParams &entity );
		void LoadClippingParams( tixmlel_traits::in xmlELement, Ogre::Real&, Ogre::Real& );
		void LoadLightAttenuationParameters( tixmlel_traits::in objectElement, Core::LightParams &outLightParams );
		void LoadLightRangeParameters( tixmlel_traits::in objectElement, Core::LightParams &outLightParams );
		void LoadPlaneParams( tixmlel_traits::in xmlElement, Ogre::Plane &outPlane );
#	endif //DBS_LM_SCRIPT_OGREMAX_TIXML

		ListenerComposite listeners;

#	if DBS_LM_SCRIPT_OGREMAX_TIXML
		Core::EnvironmentParams envParams;

		traits::SceneNodeParams_traits::container sceneSnpContainer;
		traits::SceneNodeParams_traits::container vehicleSnpContainer;
#	endif //DBS_LM_SCRIPT_OGREMAX_TIXML
	};
	//------------------------------------------------------------
} //namespace System
} //namespace Demo
#endif //DBS_LM_COLLECTIONS
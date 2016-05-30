#pragma once
#if DBS_LM_COLLECTIONS

#include "dsSceneTypes.h"
#include "dsLoader.h"
#include "collection_traits.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	class SceneCollection_LoaderInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(SceneCollection_LoaderInterface)
		//--------------------------------//
		enum ADD_OPTIONS
		{
			AO_NO_OPTIONS = 0x0,
			AO_DONT_REPLACE = 0x10
		};
		//--------------------------------//
		class traits
		{
		public:
			typedef collection_traits< dsSceneNode, COST_BUILTIN_SHARED_PTR >::in sn_in;
			typedef collection_traits< dsEntity, COST_BUILTIN_SHARED_PTR >::in ent_in;
			typedef collection_traits< dsLight, COST_BUILTIN_SHARED_PTR >::in light_in;
			typedef collection_traits< dsCamera, COST_BUILTIN_SHARED_PTR >::in cam_in;
			typedef collection_traits< dsMeshLod, COST_BUILTIN_SHARED_PTR >::in meshlod_in;
		};
		//--------------------------------//
		virtual ~SceneCollection_LoaderInterface() {}

		virtual void addSceneNode( traits::sn_in sn, int addOptions ) = 0;
		virtual void addEntity( traits::ent_in ent, int addOptions ) = 0;
		virtual void addLight( traits::light_in light, int addOptions ) = 0;
		virtual void addCamera( traits::cam_in cam, int addOptions ) = 0;
		virtual void addMeshLod( traits::meshlod_in ml, int addOptions ) = 0;
	};
	//------------------------------------------------------------
	class SceneCollection_FactoryInterface : public SceneCollection_LoaderInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(SceneCollection_FactoryInterface)
		//--------------------------------//
		class traits : public SceneCollection_LoaderInterface::traits
		{
		public:
			typedef collection_traits< dsSceneNode, COST_BUILTIN_SHARED_PTR >::container sn_cont;
			typedef collection_traits< dsSceneNode, COST_BUILTIN_SHARED_PTR >::out sn_out;
			typedef Ogre::MapIterator<sn_cont> sn_oit;
			typedef Ogre::ConstMapIterator<sn_cont> sn_ocit;

			typedef collection_traits< dsEntity, COST_BUILTIN_SHARED_PTR >::container ent_cont;
			typedef collection_traits< dsEntity, COST_BUILTIN_SHARED_PTR >::out ent_out;
			typedef Ogre::MapIterator< ent_cont > ent_oit;
			typedef Ogre::ConstMapIterator< ent_cont > ent_ocit;

			typedef collection_traits< dsLight, COST_BUILTIN_SHARED_PTR >::container light_cont;
			typedef collection_traits< dsLight, COST_BUILTIN_SHARED_PTR >::out light_out;
			typedef Ogre::MapIterator< light_cont > light_oit;
			typedef Ogre::ConstMapIterator< light_cont > light_ocit;

			typedef collection_traits< dsCamera, COST_BUILTIN_SHARED_PTR >::container cam_cont;
			typedef collection_traits< dsCamera, COST_BUILTIN_SHARED_PTR >::out cam_out;
			typedef Ogre::MapIterator< cam_cont > cam_oit;
			typedef Ogre::ConstMapIterator< cam_cont > cam_ocit;

			typedef collection_traits< dsMeshLod, COST_BUILTIN_SHARED_PTR >::container meshlod_cont;
			typedef collection_traits< dsMeshLod, COST_BUILTIN_SHARED_PTR >::out meshlod_out;
			typedef Ogre::MapIterator< meshlod_cont > meshlod_oit;
			typedef Ogre::ConstMapIterator< meshlod_cont > meshlod_ocit;
		};
		//--------------------------------//
		virtual ~SceneCollection_FactoryInterface() {}

		//Functions are not const, thus caching and other stuff can be made
		virtual traits::sn_out getSceneNode( string_traits::in snName ) = 0;
		virtual traits::ent_out getEntity( string_traits::in entName ) = 0;
		virtual traits::light_out getLight( string_traits::in lightName ) = 0;
		virtual traits::cam_out getCamera( string_traits::in camName ) = 0;
		virtual traits::meshlod_out getMeshLod( string_traits::in meshLodName ) = 0;

		virtual traits::sn_ocit getSceneNodeIterator()const = 0;
		virtual traits::ent_ocit getEntityIterator()const = 0;
		virtual traits::light_ocit getLightIterator()const = 0;
		virtual traits::cam_ocit getCameraIterator()const = 0;
		virtual traits::meshlod_ocit getMeshLodIterator()const = 0;
	};
	//------------------------------------------------------------
	class SceneLoader_CollectionInterface : public dsLoaderAuto_ProcessInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(SceneLoader_CollectionInterface)
		//--------------------------------//
		class traits : public dsLoaderAuto_ProcessInterface::traits
		{
		public:
			typedef type_traits< SceneCollection_FactoryInterface::shared_ptr >::cref sn_coll_in;
		};
		//--------------------------------//
		virtual ~SceneLoader_CollectionInterface() {}

		virtual void addCollection( traits::sn_coll_in snCollection, int collAddOptions ) = 0;
		virtual void removeCollection( traits::sn_coll_in snCollection ) = 0;
	};
	//------------------------------------------------------------
}
}
#endif //DBS_LM_COLLECTIONS
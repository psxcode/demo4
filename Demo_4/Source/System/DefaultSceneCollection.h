#pragma once
#if DBS_LM_COLLECTIONS

#include "SceneCollectionInterface.h"
#include "dsSceneTypesLoader.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	class DefaultSceneLoader : public SceneLoader_CollectionInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DefaultSceneLoader)
		//--------------------------------//
		class traits : public SceneLoader_CollectionInterface::traits
		{
		public:
			typedef Demo::map< SceneCollection_FactoryInterface::shared_ptr, int >::type sn_coll_cont;
		};
		//--------------------------------//
		DefaultSceneLoader();
		virtual ~DefaultSceneLoader();

		//LoaderAuto_ProcessInterface
		virtual traits::pen_cont_out getProcessElementNames()const;
		virtual bool processElement( const TiXmlElement *element );

		//SceneLoader_CollectionInterface
		virtual void addCollection( traits::sn_coll_in snCollection, int collAddOptions ) { m_collCont[ snCollection ] = collAddOptions; }
		virtual void removeCollection( traits::sn_coll_in snCollection ) { m_collCont.erase(snCollection); }

	protected:
		SceneCollection_FactoryInterface::shared_ptr prepareCopyCollection();

		traits::sn_coll_cont m_collCont;
	};
	//------------------------------------------------------------
	class DefaultSceneCollection : public SceneCollection_FactoryInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DefaultSceneCollection)
		//--------------------------------//
		DefaultSceneCollection();
		virtual ~DefaultSceneCollection();

		//SceneCollection_LoaderInterface
		virtual void addSceneNode( traits::sn_in sn, int addOptions );
		virtual void addEntity( traits::ent_in ent, int addOptions );
		virtual void addLight( traits::light_in light, int addOptions );
		virtual void addCamera( traits::cam_in cam, int addOptions );
		virtual void addMeshLod( traits::meshlod_in ml, int addOptions );
		
		//SceneCollection_FactoryInterface
		virtual traits::sn_out getSceneNode( string_traits::in snName );
		virtual traits::ent_out getEntity( string_traits::in entName );
		virtual traits::light_out getLight( string_traits::in lightName );
		virtual traits::cam_out getCamera( string_traits::in camName );
		virtual traits::meshlod_out getMeshLod( string_traits::in meshLodName );

		virtual traits::sn_ocit getSceneNodeIterator()const {return traits::sn_ocit(snContainer);}
		virtual traits::ent_ocit getEntityIterator()const {return traits::ent_ocit(entityContainer);}
		virtual traits::light_ocit getLightIterator()const {return traits::light_ocit(lightContainer);}
		virtual traits::cam_ocit getCameraIterator()const {return traits::cam_ocit(cameraContainer);}
		virtual traits::meshlod_ocit getMeshLodIterator()const {return traits::meshlod_ocit(meshlodContainer);}

	protected:
		traits::sn_out getSceneSnRecursive(string_traits::in snName);

		//containers
		traits::ent_cont entityContainer;
		traits::light_cont lightContainer;
		traits::cam_cont cameraContainer;
		traits::sn_cont snContainer;
		traits::meshlod_cont meshlodContainer;
	};
	//------------------------------------------------------------
}
}
#endif //DBS_LM_COLLECTIONS
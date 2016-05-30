#include "DemoStableHeaders.h"
#if DBS_LM_COLLECTIONS

#include "DefaultSceneCollection.h"


namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	static string_traits::cstr SCENE_LOADER_FIELD( "ogremax" );
	//------------------------------------------------------------
	DefaultSceneLoader::DefaultSceneLoader()
	{
	}
	//------------------------------------------------------------
	/*virtual*/ DefaultSceneLoader::~DefaultSceneLoader()
	{
	}
	//------------------------------------------------------------ 
	/*virtual*/ DefaultSceneLoader::traits::pen_cont_out DefaultSceneLoader::getProcessElementNames()const
	{
		shared_ptr_traits< traits::pen_cont >::type pnn( new traits::pen_cont );
		pnn->insert( &SCENE_LOADER_FIELD );
		return pnn;
	}
	//------------------------------------------------------------
	/*virtual*/ bool DefaultSceneLoader::processElement( const TiXmlElement *element )
	{
		if(element->ValueStr() != SCENE_LOADER_FIELD) return false;
		if(m_collCont.empty()) return false;

		SceneCollection_FactoryInterface::shared_ptr copyColl = prepareCopyCollection();

		dsSceneNodeLoader sn_ldr;
		dsEntityLoader ent_ldr;
		dsLightLoader light_ldr;
		dsCameraLoader cam_ldr;
		dsMeshLodLoader ml_ldr;

		const TiXmlElement *inOgremaxElem(0);
		while( dsIterateChildElements( element, inOgremaxElem ) )
		{
			//inside <ogremax>
			string_traits::outref inOgremaxName = inOgremaxElem->ValueStr();
			if( string_equals<false>()(inOgremaxName, dsKeys::SceneCommon::NODES_FIELD) )
			{
				string_traits::str nodesGroupName = dsGetString<false>(inOgremaxElem, dsKeys::SceneCommon::NAME);
				const TiXmlElement *inNodesElement(0);
				while( dsIterateChildElements( inOgremaxElem, inNodesElement ) )
				{
					//inside<nodes>
					string_traits::outref inNodesName = inNodesElement->ValueStr();
					if( string_equals<false>()(inNodesName, dsKeys::SceneCommon::NODE_ELEMENT) )
					{
						dsSceneNode::shared_ptr sn;

						//Find if a "copy" directive is present and get a copy
						string_traits::str copyName = dsGetString<false>(inNodesElement, dsKeys::SceneCommon::COPY);
						if(!copyName.empty() && copyColl)
						{
							auto copySn = copyColl->getSceneNode(copyName);
							if( copySn )
								sn = copySn->CreateCopyPtr();
						}

						//if still invalid - create
						if(!sn) sn.reset(new dsSceneNode);

						//process and add if succedeed
						if( sn_ldr.processElement(inNodesElement, *sn) )
							for( auto it = m_collCont.begin(), eit = m_collCont.end(); it != eit; ++it )
								it->first->addSceneNode( sn, it->second );
					}
				}
			}
			else if( string_equals<false>()(inOgremaxName, dsKeys::SceneCommon::ENTITIES_FIELD) )
			{
				const TiXmlElement *inEntitiesElem(0);
				while( dsIterateChildElements(inOgremaxElem, inEntitiesElem) )
				{
					//inside <entities>
					string_traits::outref inEntitiesName = inEntitiesElem->ValueStr();
					if(string_equals<false>()(inEntitiesName, dsKeys::SceneCommon::ENTITY_ELEMENT))
					{
						dsEntity::shared_ptr ent;

						//Find if a "copy" directive is present and get a copy
						string_traits::str copyName = dsGetString<false>(inEntitiesElem, dsKeys::SceneCommon::COPY);
						if(!copyName.empty() && copyColl)
						{
							auto copyEnt = copyColl->getEntity(copyName);
							if( copyEnt )
								ent = copyEnt->CreateEntityCopyPtr();
						}

						//if still invalid - create
						if(!ent) ent.reset(new dsEntity);

						//process and add if succedeed
						if( ent_ldr.processElement(inEntitiesElem, *ent) )
							for( auto it = m_collCont.begin(), eit = m_collCont.end(); it != eit; ++it )
								it->first->addEntity( ent, it->second );
					}
				}
			}
			else if( string_equals<false>()(inOgremaxName, dsKeys::SceneCommon::LIGHTS_FIELD) )
			{
				const TiXmlElement *inLightsElem(0);
				while( dsIterateChildElements(inOgremaxElem, inLightsElem) )
				{
					//inside <lights>
					string_traits::outref inLightsName = inLightsElem->ValueStr();
					if(string_equals<false>()(inLightsName, dsKeys::SceneCommon::LIGHT_ELEMENT))
					{
						dsLight::shared_ptr light;

						//Find if a "copy" directive is present and get a copy
						string_traits::str copyName = dsGetString<false>(inLightsElem, dsKeys::SceneCommon::COPY);
						if(!copyName.empty() && copyColl)
						{
							auto copyLight = copyColl->getLight(copyName);
							if( copyLight )
								light = copyLight->CreateLightCopyPtr();
						}

						//if still invalid - create
						if(!light) light.reset(new dsLight);

						//process and add if succedeed
						if( light_ldr.processElement(inLightsElem, *light) )
							for( auto it = m_collCont.begin(), eit = m_collCont.end(); it != eit; ++it )
								it->first->addLight( light, it->second );
					}
				}
			}
			else if( string_equals<false>()(inOgremaxName, dsKeys::SceneCommon::CAMERAS_FIELD) )
			{
				const TiXmlElement *inCamerasElem(0);
				while( dsIterateChildElements(inOgremaxElem, inCamerasElem) )
				{
					//inside <cameras>
					string_traits::outref inCamerasName = inCamerasElem->ValueStr();
					if(string_equals<false>()(inCamerasName, dsKeys::SceneCommon::CAMERA_ELEMENT))
					{
						dsCamera::shared_ptr cam;

						//Find if a "copy" directive is present and get a copy
						string_traits::str copyName = dsGetString<false>(inCamerasElem, dsKeys::SceneCommon::COPY);
						if(!copyName.empty() && copyColl)
						{
							auto copyCam = copyColl->getCamera(copyName);
							if( copyCam )
								cam = copyCam->CreateCameraCopyPtr();
						}

						//if still invalid - create
						if(!cam) cam.reset(new dsCamera);

						if( cam_ldr.processElement(inCamerasElem, *cam) )
							for( auto it = m_collCont.begin(), eit = m_collCont.end(); it != eit; ++it )
								it->first->addCamera( cam, it->second );
					}
				}
			}
			else if( string_equals<false>()(inOgremaxName, dsKeys::SceneCommon::MESHLODS_FIELD) )
			{
				const TiXmlElement *inMeshLodsElem(0);
				while( dsIterateChildElements(inOgremaxElem, inMeshLodsElem) )
				{
					//inside <meshLods>
					string_traits::outref inMeshLodsName = inMeshLodsElem->ValueStr();
					if( string_equals<false>()(inMeshLodsName, dsKeys::SceneCommon::MESHLOD_ELEMENT) )
					{
						dsMeshLod::shared_ptr ml;

						//Find if a "copy" directive is present and get a copy
						string_traits::str copyName = dsGetString<false>(inMeshLodsElem, dsKeys::SceneCommon::COPY);
						if(!copyName.empty() && copyColl)
						{
							auto copyML = copyColl->getMeshLod(copyName);
							if(copyML)
								ml = copyML->CreateCopyPtr();
						}

						//if still invalid - create
						if(!ml) ml.reset(new dsMeshLod);

						if( ml_ldr.processElement(inMeshLodsElem, *ml) )
							for( auto it = m_collCont.begin(), eit = m_collCont.end(); it != eit; ++it )
								it->first->addMeshLod( ml, it->second );
					}
				}
			}
			
		}

		return true;
	}
	//------------------------------------------------------------
	SceneCollection_FactoryInterface::shared_ptr DefaultSceneLoader::prepareCopyCollection()
	{
		if(m_collCont.empty())
			return SceneCollection_FactoryInterface::shared_ptr();

		for( auto it = m_collCont.begin(), eit = m_collCont.end(); it != eit; ++it )
		{
			if(it->second == SceneCollection_LoaderInterface::AO_NO_OPTIONS)
				return it->first;
		}

		return m_collCont.begin()->first;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	typedef DefaultSceneCollection DSC;
	//------------------------------------------------------------ 
	DefaultSceneCollection::DefaultSceneCollection()
	{
	}
	//------------------------------------------------------------
	DefaultSceneCollection::~DefaultSceneCollection()
	{
	}
	//------------------------------------------------------------
	/*virtual*/ DSC::traits::sn_out DefaultSceneCollection::getSceneNode( string_traits::in snName )
	{
		auto iter = snContainer.find(snName);
		return iter != snContainer.end() ? iter->second : traits::sn_out();
	}
	//------------------------------------------------------------
	/*virtual*/ DSC::traits::ent_out DefaultSceneCollection::getEntity( string_traits::in entName )
	{
		auto iter = entityContainer.find(entName);
		return iter != entityContainer.end() ? iter->second : traits::ent_out();
	}
	//------------------------------------------------------------
	/*virtual*/ DSC::traits::light_out DefaultSceneCollection::getLight( string_traits::in lightName )
	{
		auto iter = lightContainer.find(lightName);
		return iter != lightContainer.end() ? iter->second : traits::light_out();
	}
	//------------------------------------------------------------
	/*virtual*/ DSC::traits::cam_out DefaultSceneCollection::getCamera( string_traits::in camName )
	{
		auto iter = cameraContainer.find(camName);
		return iter != cameraContainer.end() ? iter->second : traits::cam_out();
	}
	//------------------------------------------------------------
	/*virtual*/ DSC::traits::meshlod_out DefaultSceneCollection::getMeshLod( string_traits::in meshLodName )
	{
		auto iter = meshlodContainer.find(meshLodName);
		return iter != meshlodContainer.end() ? iter->second : traits::meshlod_out();
	}
	//------------------------------------------------------------
	/*virtual*/ void DefaultSceneCollection::addSceneNode( traits::sn_in sn, int addOptions )
	{
		if( (addOptions & AO_DONT_REPLACE) == 0 ) snContainer.erase( sn->name );
		snContainer.insert( traits::sn_cont::value_type( sn->name, sn->CreateCopyPtr() ) );
	}
	//------------------------------------------------------------
	/*virtual*/ void DefaultSceneCollection::addEntity( traits::ent_in ent, int addOptions )
	{
		if( (addOptions & AO_DONT_REPLACE) == 0 ) entityContainer.erase( ent->name );
		entityContainer.insert( traits::ent_cont::value_type( ent->name, ent->CreateEntityCopyPtr() ) );
	}
	//------------------------------------------------------------
	/*virtual*/ void DefaultSceneCollection::addLight( traits::light_in light, int addOptions )
	{
		if( (addOptions & AO_DONT_REPLACE) == 0 ) lightContainer.erase( light->name );
		lightContainer.insert( traits::light_cont::value_type( light->name, light->CreateLightCopyPtr() ) );
	}
	//------------------------------------------------------------
	/*virtual*/ void DefaultSceneCollection::addCamera( traits::cam_in cam, int addOptions )
	{
		if( (addOptions & AO_DONT_REPLACE) == 0 ) cameraContainer.erase( cam->name );
		cameraContainer.insert( traits::cam_cont::value_type( cam->name, cam->CreateCameraCopyPtr() ) );
	}
	//------------------------------------------------------------
	/*virtual*/ void DefaultSceneCollection::addMeshLod( traits::meshlod_in ml, int addOptions )
	{
		if( (addOptions & AO_DONT_REPLACE) == 0 ) meshlodContainer.erase( ml->meshName.name );
		meshlodContainer.insert( traits::meshlod_cont::value_type( ml->meshName.name, ml->CreateCopyPtr() ) );
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
}
}
#endif //DBS_LM_COLLECTIONS
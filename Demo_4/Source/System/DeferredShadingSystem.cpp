#include "DemoStableHeaders.h"
#if DBS_LM_DEFERRED_RENDERING

#include <OgreCompositorManager.h>
#include <OgreCompositorChain.h>
#include <OgreCompositionTargetPass.h>
#include "MainController.h"

#include "DeferredShadingSystem.h"

namespace Ogre
{
	template<>
	Demo::System::DeferredShadingSystem* Singleton<Demo::System::DeferredShadingSystem>::ms_Singleton = 0;
}

namespace Demo {
namespace System {
//------------------------------------------------------------
/*static*/ DeferredShadingSystem* DeferredShadingSystem::getSingletonPtr()
{
	return ms_Singleton;
}
/*static*/ DeferredShadingSystem& DeferredShadingSystem::getSingleton()
{
	_ASSERT( ms_Singleton ); return (*ms_Singleton);
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
OgreMaterialManagerListener::OgreMaterialManagerListener()
	: createDummyTechniqueIfNotFound(true)
{
}
//------------------------------------------------------------
/*virtual*/ OgreMaterialManagerListener::~OgreMaterialManagerListener()
{
	for(auto iter = dfrgeom_mgs.begin(), end_iter = dfrgeom_mgs.end(); iter != end_iter; ++iter)
	{
		Ogre::MaterialManager::getSingleton().removeListener(this, iter->first);
	}
}
//------------------------------------------------------------
/*virtual*/ Ogre::Technique* OgreMaterialManagerListener::handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex, const Ogre::Renderable* rend)
{
	auto iter = dfrgeom_mgs.find(schemeName);
	if(iter != dfrgeom_mgs.end())
	{
		for(auto mg_iter = iter->second.begin(), mg_end_iter = iter->second.end(); mg_iter != mg_end_iter; ++mg_iter)
		{
			if((*mg_iter)->modifyMaterial(originalMaterial))
			{
				originalMaterial->compile();

				/* Here a dead cycle can occur
				 * because getBestTechnique calls handleSchemeNotFound
				 * To prevent this MaterialGenerator should not modify
				 * materials if already modified.
				 * MaterialGenerator should return FALSE if modification
				 * is requested again.
				 * By default DeferredGeometryMaterialGenerator behaves in this matter
				 **/
				return originalMaterial->getBestTechnique(lodIndex);
			}
		}
	}
	return 0;
}
//------------------------------------------------------------
void OgreMaterialManagerListener::addMaterialGenerator(DeferredGeometryMaterialGenerator::shared_ptr mg)
{
	string_traits::outref mgScheme = mg->getCreateSchemeName();
	auto iter = dfrgeom_mgs.find(mgScheme);
	if(iter != dfrgeom_mgs.end())
	{
		iter->second.insert(mg);
	}
	else
	{
		//new scheme
		dfrgeom_mgs[mgScheme].insert(mg);
		Ogre::MaterialManager::getSingleton().addListener(this, mgScheme);
	}
}
//------------------------------------------------------------
void OgreMaterialManagerListener::removeMaterialGenerator(DeferredGeometryMaterialGenerator::shared_ptr mg)
{
	string_traits::outref mgScheme = mg->getCreateSchemeName();
	auto iter = dfrgeom_mgs.find(mgScheme);
	if(iter != dfrgeom_mgs.end())
	{
		iter->second.erase(mg);
		if(iter->second.empty())
			Ogre::MaterialManager::getSingleton().removeListener(this, mgScheme);
	}
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
DeferredShadingSystem::GeomDebugListener::GeomDebugListener()
	: quadMode(false)
	, needUpdate(true)
	, gbufferFilter(0)
	, channelFilter(0)
{
}
//------------------------------------------------------------
/*virtual*/ DeferredShadingSystem::GeomDebugListener::~GeomDebugListener()
{
}
//------------------------------------------------------------
/*virtual*/ void DeferredShadingSystem::GeomDebugListener::notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
}
//------------------------------------------------------------
/*virtual*/ void DeferredShadingSystem::GeomDebugListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
	/*if(needUpdate)
	{
		Ogre::GpuProgramParametersSharedPtr fpParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
		fpParams->setNamedConstant("quad_mode", quadMode);
		//fpParams->setNamedConstant("gbuffer_filter", gbufferFilter);
		//fpParams->setNamedConstant("channel_filter", channelFilter);
		needUpdate = false;
	}*/
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
DFRViewport::DFRViewport()
	: m_pViewport(0)
	, mGBMode(GBM_NOT_DEFINED)
	, mRenderMode(RM_FINAL)
	, mSeparateLightTex(false)
{
	for(int i = 0; i < CI_NUM_INST; ++i)
		m_pCompInstances[i] = 0;
}
//------------------------------------------------------------
DFRViewport::~DFRViewport()
{
	if(m_pViewport)
		Ogre::CompositorManager::getSingleton().removeCompositorChain(m_pViewport);
}
//------------------------------------------------------------
/*static*/ DFRViewport::shared_ptr DFRViewport::getInstance(const InitData &idata)
{
	DFRViewport::shared_ptr spViewport;
	if(!idata.isValid())
		return spViewport;

	spViewport.reset(new DFRViewport);

	spViewport->m_pViewport = idata.pViewport;
	spViewport->mGBMode = idata.gbMode;
	spViewport->mSeparateLightTex = idata.separateLightTex;
	spViewport->m_spCompMG = idata.spCompMG;

	return spViewport;
}
//------------------------------------------------------------
/*static*/ string_traits::str DFRViewport::getCompositorSchemeName(Ogre::uint32 gbMode)
{
	return getGBSizeName(gbMode) + getGBLayoutName(gbMode);
}
//------------------------------------------------------------
/*static*/ string_traits::str DFRViewport::getGBLayoutName(Ogre::uint32 gbMode)
{
	return gbMode & GBM_LAYOUT_02 ? "Lay02" : gbMode & GBM_LAYOUT_01 ? "Lay01" : "";
}
//------------------------------------------------------------
/*static*/ string_traits::str DFRViewport::getGBSizeName(Ogre::uint32 gbMode)
{
	return gbMode & GBM_SIZE_FULL ? "FullSize"	: gbMode & GBM_SIZE_HALF ? "HalfSize" : "";
}
//------------------------------------------------------------
void DFRViewport::resetCompositorChain()
{
	Ogre::CompositorManager &compMan = Ogre::CompositorManager::getSingleton();
	compMan.removeCompositorChain(m_pViewport);
	Ogre::CompositorChain *pChain = compMan.getCompositorChain(m_pViewport);

	string_traits::str gbCompSchemeName = getCompositorSchemeName(mGBMode);
	Ogre::CompositorPtr gbComp = compMan.getByName("DFR/GBuffer");
	m_pCompInstances[CI_GBUFFER] = pChain->addCompositor(gbComp, Ogre::CompositorChain::LAST, gbCompSchemeName);
	m_pCompInstances[CI_GBUFFER]->setEnabled(true);

	string_traits::str lightCompSchemeName = getGBLayoutName(mGBMode);
	Ogre::CompositorPtr lightComp = mSeparateLightTex ?
		compMan.getByName("DFR/LightTex") : compMan.getByName("DFR/Light");
	m_pCompInstances[CI_LIGHT] = pChain->addCompositor(lightComp, Ogre::CompositorChain::LAST, lightCompSchemeName);
	m_pCompInstances[CI_LIGHT]->setEnabled(true);

	if(mRenderMode == RM_FINAL)
	{
	}
	else
	{
		Ogre::CompositorPtr postComp = mSeparateLightTex ?
			compMan.getByName("DFR/GeomDebugLightTex") : compMan.getByName("DFR/GeomDebug");
		//get material using permutation
		DeferredCompositorMaterialGenerator::perm_type perm;
		perm = mGBMode & GBM_LAYOUT_01 ? DeferredCompositorMaterialGenerator::DFR_GBLAYOUT_01
			: mGBMode & GBM_LAYOUT_02 ? DeferredCompositorMaterialGenerator::DFR_GBLAYOUT_02
			: 0;
		perm |= mSeparateLightTex ? DeferredCompositorMaterialGenerator::DFR_SEPARATE_LIGHT_REFL_TEX : 0;
		Ogre::MaterialPtr mat = m_spCompMG->getMaterial(perm);

		postComp->getTechnique(0)->getOutputTargetPass()->getPass(0)->setMaterial(mat);
		m_pCompInstances[CI_POST] = pChain->addCompositor(postComp);
		m_pCompInstances[CI_POST]->setEnabled(true);

		/*
		m_spGeomDebugListener.reset(new GeomDebugListener);
		m_pCompInstances[CI_POST]->addListener(m_spGeomDebugListener.get());
		updateGeomDebugData();
		*/
	}
}
//------------------------------------------------------------
void DFRViewport::setGeomMaterialScheme(string_traits::in schemeName)
{
	if(m_pCompInstances[CI_GBUFFER])
	{
		m_pCompInstances[CI_GBUFFER]->getTechnique()->getTargetPass(0)->setMaterialScheme(schemeName);
	}
}
//------------------------------------------------------------
string_traits::outref DFRViewport::getGeomMaterialScheme()const
{
	return m_pCompInstances[CI_GBUFFER] ?
		m_pCompInstances[CI_GBUFFER]->getTechnique()->getTargetPass(0)->getMaterialScheme() : Ogre::StringUtil::BLANK;
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
DFRLight::DFRLight()
{
}
//------------------------------------------------------------
/*virtual*/ DFRLight::~DFRLight()
{
}
//------------------------------------------------------------
/*static*/ DFRLight::shared_ptr DFRLight::getInstance(const LightData &ldata)
{
	/* This indexes are synced with LightMaskType
	 * The first one is Rect, but we create it procedurally
	 **/
	static string_traits::str meshNames[] = {
		"",
		"cone",
		"sphere",
		"cyl"
	};

	DFRLight::shared_ptr spLight;

	if(!ldata.isValid())
		return DFRLight::shared_ptr();

	Ogre::SceneNode *pLightSN = ldata.pLight->getParentSceneNode();
	Ogre::SceneManager *pManager = pLightSN->getCreator();

	SceneFactoryInterface::InitData factoryInitData;
	factoryInitData.pSceneManager = pManager;
	factoryInitData.objNamePrefix = "lg";
	SceneFactoryInterface::shared_ptr spSceneFactory = SceneFactoryInterface::getInstance(factoryInitData);

	LightMaskType maskType;
	if(ldata.useDefinedLightMask)
	{
		maskType = ldata.mask;
	}
	else
	{
		switch(ldata.pLight->getType())
		{
		case Ogre::Light::LT_DIRECTIONAL:
			maskType = LMT_RECT;
			break;
		case Ogre::Light::LT_SPOTLIGHT:
			maskType = LMT_CONE;
			break;
		case Ogre::Light::LT_POINT:
			maskType = LMT_SPHERE;
		}
	}

	spLight.reset(new DFRLight);
	spLight->m_pLight = ldata.pLight;
	spLight->mMaskType = maskType;

	if(maskType == LMT_RECT)
	{

	}
	else
	{
		dsEntity::shared_ptr dsEnt(new dsEntity);
		dsEnt->name = ldata.pLight->getName();
		dsEnt->castShadows = false;
		dsEnt->meshFilename = meshNames[maskType];
		dsEnt->meshFilename += "_lod" + Ogre::StringConverter::toString(ldata.maskStartLodIndex) + ".mesh";
		Ogre::Entity* pEnt = spSceneFactory->createEntity(dsEnt);

		if(!pEnt)
			return DFRLight::shared_ptr();

		Ogre::SceneNode *pLgSn = pLightSN->createChildSceneNode();
		pLgSn->attachObject(pEnt);

		auto calcAttRadius = [](const Ogre::Light* pLight)->Ogre::Real
		{

		};

		Ogre::Real attRange = ldata.pLight->getAttenuationRange();
		Ogre::Real attConst = ldata.pLight->getAttenuationConstant();
		Ogre::Real attLin = ldata.pLight->getAttenuationLinear();
		Ogre::Real attQuad = ldata.pLight->getAttenuationQuadric();

		if(!Ogre::Math::RealEqual(attConst, 1.0f) ||
			!Ogre::Math::RealEqual(attLin, 0.0f) ||
			!Ogre::Math::RealEqual(attQuad, 0.0f))
		{
			Ogre::Real threshold = 1.0f / (10.0f / 256.0f);
			attConst -= threshold;
			Ogre::Real d = 
		}
		else
		{

		}
	}


	

}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
DeferredShadingSystem::DeferredShadingSystem()
{
	m_spMaterialManagerListener.reset(new OgreMaterialManagerListener);

	Ogre::CompositorManager::getSingleton()
		.registerCustomCompositionPass("DeferredLight", new DLightCompositionPass(m_spMatGens[MG_LIGHT]));


}
//------------------------------------------------------------
DeferredShadingSystem::~DeferredShadingSystem()
{
}
//------------------------------------------------------------
void DeferredShadingSystem::resetMaterialGenerators()
{
	//DeferredGeometryMaterialGenerator
	{
		DeferredGeometryMaterialGenerator::InitData mat_idata;
		mat_idata.mgName = "dg01";
		mat_idata.mConvertSchemeName = "convert_dfrgeom01";
		mat_idata.mCreateSchemeName = "dfrgeom01";
		DeferredGeometryMaterialGenerator::shared_ptr mg01 = DeferredGeometryMaterialGenerator::getInstance(mat_idata);
		mg01->setGBufferLayout(DeferredGeometryMaterialGenerator::DFR_GBLAYOUT_01);

		mat_idata.mgName = "dg02";
		mat_idata.mConvertSchemeName = "convert_dfrgeom02";
		mat_idata.mCreateSchemeName = "dfrgeom02";
		DeferredGeometryMaterialGenerator::shared_ptr mg02 = DeferredGeometryMaterialGenerator::getInstance(mat_idata);
		mg02->setGBufferLayout(DeferredGeometryMaterialGenerator::DFR_GBLAYOUT_02);

		//this automatically registers Ogre::MaterialManager::Listener
		m_spMaterialManagerListener->addMaterialGenerator(mg01);
		m_spMaterialManagerListener->addMaterialGenerator(mg02);
	}

	//DeferredLightMaterialGenerator	
	{
		DeferredLightMaterialGenerator::InitData mat_idata;
		mat_idata.mgName = "dfrlight";
		m_spMatGens[MG_LIGHT] = DeferredLightMaterialGenerator::getInstance(mat_idata);
	}

	//DeferredCompositorMaterialGenerator	
	{
		DeferredCompositorMaterialGenerator::InitData mat_idata;
		mat_idata.mgName = "dfrcomp";
		m_spMatGens[MG_COMP] = DeferredCompositorMaterialGenerator::getInstance(mat_idata);
	}
}
//------------------------------------------------------------
/*static*/ void DeferredShadingSystem::modifySceneManagerForDS(Ogre::SceneManager *pSM)
{
	pSM->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
	pSM->setShadowTextureCasterMaterial("DFR/Shadows/Caster");
	pSM->setShadowFarDistance(150);
	pSM->setShadowTextureSize(512);
	pSM->setShadowTextureCount(1);
	pSM->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_R);
	pSM->setShadowDirectionalLightExtrusionDistance(75);
}
//------------------------------------------------------------
//------------------------------------------------------------
}
}
#endif
#pragma once
#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreViewport.h>
#include <OgreCompositorInstance.h>
#include <OgreCompositorLogic.h>
#include <OgreScriptCompiler.h>
#include "DeferredMaterialGenerator.h"
#include "Interface/FactoryInterface.h"

namespace Demo {
namespace System {
	//------------------------------------------------------------
	//The simple types of compositor logics will all do the same thing -
	//Attach a listener to the created compositor
	class CompositorListenerFactoryLogic : public Ogre::CompositorLogic
	{
	public:
		/** @copydoc CompositorLogic::compositorInstanceCreated */
		virtual void compositorInstanceCreated(Ogre::CompositorInstance* newInstance) 
		{
			Ogre::CompositorInstance::Listener* listener = createListener(newInstance);
			newInstance->addListener(listener);
			mListeners[newInstance] = listener;
		}

		/** @copydoc CompositorLogic::compositorInstanceDestroyed */
		virtual void compositorInstanceDestroyed(Ogre::CompositorInstance* destroyedInstance)
		{
			delete mListeners[destroyedInstance];
			mListeners.erase(destroyedInstance);
		}

	protected:
		//This is the method that implementations will need to override
		virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance) = 0;
	private:
		typedef std::map<Ogre::CompositorInstance*, Ogre::CompositorInstance::Listener*> ListenerMap;
		ListenerMap mListeners;
	};
	//------------------------------------------------------------
	/* This class captures materials with missing Material Schemes
	 * and uses Material Generators to fix that
	 **/
	class OgreMaterialManagerListener : public Ogre::MaterialManager::Listener
	{
	public:
		DEFINE_CLASS_SHARED_PTR(OgreMaterialManagerListener)
		//--------------------------------//
		OgreMaterialManagerListener();
		virtual ~OgreMaterialManagerListener();
		virtual Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex,
											const Ogre::String& schemeName,
											Ogre::Material* originalMaterial,
											unsigned short lodIndex,
											const Ogre::Renderable* rend);

		void addMaterialGenerator(DeferredGeometryMaterialGenerator::shared_ptr mg);
		void removeMaterialGenerator(DeferredGeometryMaterialGenerator::shared_ptr mg);

		bool createDummyTechniqueIfNotFound;

	protected:
		// (schemeName -> generator set) map
		typedef Demo::map<string_traits::str, Demo::set<DeferredGeometryMaterialGenerator::shared_ptr>::type>::type dfrgeom_mg_cont;

		dfrgeom_mg_cont dfrgeom_mgs;
	};
	//------------------------------------------------------------
	class DFRViewport
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DFRViewport)
		//--------------------------------//
		enum GBMode
		{
			GBM_NOT_DEFINED,

			GBM_LAYOUT_01 = 0x1,
			GBM_LAYOUT_02 = 0x2,
			GBM_LAYOUT_03 = 0x4,
			GBM_LAYOUT_MASK = 0x7,

			GBM_SIZE_HALF = 0x10,
			GBM_SIZE_FULL = 0x20,
			GBM_SIZE_MASK = 0x30
		};
		//--------------------------------//
		enum RenderMode
		{
			RM_GBUFFER = 0,
			RM_FINAL = RM_GBUFFER,

			RM_NORMAL = 0x1,
			RM_DIFFUSE = 0x2,
			RM_GEOMLIGHT = 0x4,
			RM_REFLECTION = 0x8,
			RM_MOTION = 0x10,
			RM_GEOM_3CHANNEL_MASK = 0xFF,

			RM_DEPTH = 0x100,
			RM_SPEC_INT = 0x200,
			RM_SPEC_POW = 0x400,
			RM_MAT_ID = 0x800,
			RM_4x1 = 0x1000,
			RM_GEOM_1CHANNEL_MASK = 0xFF00,

			RM_LIGHT = 0x10000,
			RM_DIFFUSExLIGHT = 0x20000,

			RM_GEOM_DEBUG_MASK = RM_GEOM_3CHANNEL_MASK | RM_GEOM_1CHANNEL_MASK,
			RM_LIGHT_DEBUG_MASK = 0x30000
		};
		//--------------------------------//
		enum ChannelFilter
		{
			CF_RGB0 = 0,
			CF_R = 1,
			CF_G = 2,
			CF_RG = 3,
			CF_B = 4,
			CF_RB = 5,
			CF_GB = 6,
			CF_RGB7 = 7,
			CF_ALPHA = 8
		};
		//--------------------------------//
		struct InitData
		{
			InitData()
				: pViewport(0)
				, gbMode(GBM_NOT_DEFINED)
				, renderMode(RM_FINAL)
				, separateLightTex(false)
			{
			}
			bool isValid()const {return pViewport != 0 && gbMode != GBM_NOT_DEFINED;}

			Ogre::Viewport *pViewport;
			Ogre::uint32 gbMode;
			RenderMode renderMode;
			bool separateLightTex;
			MaterialGenerator::shared_ptr spCompMG;
		};
		//--------------------------------//
		static DFRViewport::shared_ptr getInstance(const InitData &idata);
		static string_traits::str getCompositorSchemeName(Ogre::uint32 gbMode);
		static string_traits::str getGBSizeName(Ogre::uint32 gbMode);
		static string_traits::str getGBLayoutName(Ogre::uint32 gbMode);
		//--------------------------------//
		~DFRViewport();

		void setRenderMode(RenderMode mode);
		void setChannelFilter(ChannelFilter filter);

		Ogre::Viewport* getViewport()const {return m_pViewport;}
		Ogre::uint32 getGBMode()const {return mGBMode;}
		ChannelFilter getChannelFilter()const {return mChannelFilter;}
		RenderMode getRenderMode()const {return mRenderMode;}

		void setGeomMaterialScheme(string_traits::in schemeName);
		string_traits::outref getGeomMaterialScheme()const;

		/* WARNING
		* CustomCompositionPass "DeferredLight" should be registered within OGRE
		* before this call
		**/
		void resetCompositorChain();

		void setCompMG(MaterialGenerator::shared_ptr const &mg);
		MaterialGenerator::shared_ptr const & getCompMG()const {return m_spCompMG;}

	protected:
		//--------------------------------//
		enum CompInstances
		{
			CI_GBUFFER,
			CI_LIGHT,
			CI_POST,
			CI_NUM_INST
		};
		//--------------------------------//

		/* The CompositorChain is applied to this viewport
		 **/
		Ogre::Viewport *m_pViewport;

		/* GBuffer mode
		 * Size and Layout Modes are combined here
		 **/
		Ogre::uint32 mGBMode;

		/* Render modes are used for debug vis
		 **/
		RenderMode mRenderMode;

		/* Channel filter can be used to show specific channels
		 * For debug usage
		 **/
		ChannelFilter mChannelFilter;

		/* Should DSS use additional Light and Reflection texture
		 * during Light Render Pass
		 * If set to false - geometry Light and Reflection buffer will be used
		 * with additive blending
		 **/
		bool mSeparateLightTex;

		Ogre::CompositorInstance *m_pCompInstances[CI_NUM_INST];

		MaterialGenerator::shared_ptr m_spCompMG;

	private:
		DFRViewport();
		DFRViewport(const DFRViewport &);
	};
	//------------------------------------------------------------
	class DFRLight
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DFRLight)
		//--------------------------------//
		enum LightMaskType
		{
			LMT_RECT,
			LMT_SPHERE,
			LMT_CONE,
			LMT_CYL
		};
		//--------------------------------//
		struct LightData
		{
			LightData()
				: pLight(0)
				, useDefinedLightMask(false)
			{
			}
			bool isValid()const
			{
				return pLight && pLight->getParentNode() && pLight->getParentSceneNode()->getCreator();
			}

			/* Reference light
			 **/
			Ogre::Light *pLight;

			/* true - Light Geometry as defined in mask
			 * false - Light Geometry as Ref Light type
			 **/
			bool useDefinedLightMask;
			LightMaskType mask;

			Ogre::uint8 maskStartLodIndex;

			bool maskWithGeometryLods;


			bool maskWithMaterialLods;
			Ogre::Material::LodValueList materialLodValues;
			Ogre::LodStrategy* pMaterialLodStrategy;
		};
		//--------------------------------//
		static DFRLight::shared_ptr getInstance(const LightData &ldata);
		//--------------------------------//
		virtual ~DFRLight();

	protected:
		Ogre::Light *m_pLight;
		LightMaskType mMaskType;

		Ogre::Entity *m_pEntityMask;
		Ogre::SimpleRenderable *m_pSimpleRenderableMask;

	private:
		DFRLight();
		DFRLight(const DFRLight &);
	};
	//------------------------------------------------------------
	class DeferredShadingSystem : public Ogre::Singleton<DeferredShadingSystem>
	{
		friend class Ogre::Singleton<DeferredShadingSystem>;
	public:
		DEFINE_CLASS_SHARED_PTR(DeferredShadingSystem)
		//--------------------------------//
		/* Material generator types
		 **/
		enum MatGen
		{
			MG_COMP,		// Compositor MG
			MG_LIGHT,		// Light geometry MG
			MG_DFR_GEOM,	// Deferred geometry MG
			MG_NUM_GENS		// Number of MGs
		};
		//--------------------------------//
		static DeferredShadingSystem& getSingleton();
		static DeferredShadingSystem* getSingletonPtr();
		static void modifySceneManagerForDS(Ogre::SceneManager *pSM);
		//--------------------------------//
		~DeferredShadingSystem();

	protected:
		//--------------------------------//
		class GeomDebugListener : public Ogre::CompositorInstance::Listener
		{
		public:
			DEFINE_CLASS_SHARED_PTR(GeomDebugListener)
			//--------------------------------//
			GeomDebugListener();
			virtual ~GeomDebugListener();
			virtual void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
			virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);

			bool quadMode;
			int gbufferFilter;
			int channelFilter;
			bool needUpdate;
		};
		//--------------------------------//
		void resetMaterialGenerators();
		
		void updateGeomDebugData();

		SceneFactoryInterface::shared_ptr m_spSceneFactory;
		
		OgreMaterialManagerListener::shared_ptr m_spMaterialManagerListener;

		GeomDebugListener::shared_ptr m_spGeomDebugListener;

	private:
		DeferredShadingSystem();
		DeferredShadingSystem( const DeferredShadingSystem & );
	};
	//------------------------------------------------------------
}
}
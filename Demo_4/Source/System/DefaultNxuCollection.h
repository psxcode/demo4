#pragma once
#if DBS_LM_NXU_COLLECTIONS

#include <OgreIteratorWrappers.h>
#include "NxuCollectionInterface.h"

namespace Demo {
namespace System {
	//------------------------------------------------------------
	class DefaultNxuLoader : public NxuLoader_CollectionInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DefaultNxuLoader)
		//--------------------------------//
		class traits : public NxuLoader_CollectionInterface::traits
		{
		public:
			typedef NxuCollection_LoaderInterface nxu_coll_i;
			typedef Demo::map< nxu_coll_i::shared_ptr, int >::type nxu_coll_cont;
		};
		//--------------------------------//
		DefaultNxuLoader();
		virtual ~DefaultNxuLoader();

		//DotSceneLoader interface
		virtual const Ogre::StringVector& getScriptPatterns(void) const { return mScriptPatterns; }
		virtual void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
		virtual Ogre::Real getLoadingOrder(void) const { return 9999.0f; }

		//NxuLoader_CollectionInterface
		virtual void addCollection( traits::nxu_coll_in vehicleCollection, int collAddOptions ) { collectionCont[vehicleCollection] = collAddOptions; }
		virtual void removeCollection( traits::nxu_coll_in vehicleCollection ) { collectionCont.erase( vehicleCollection ); }

	protected:
		Ogre::StringVector mScriptPatterns;
		traits::nxu_coll_cont collectionCont;
	};
	//------------------------------------------------------------
	class DefaultNxuCollection : public NxuCollection_FactoryInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DefaultNxuCollection)
		//--------------------------------//
		DefaultNxuCollection();
		virtual ~DefaultNxuCollection();

		virtual void addNxuPhysicsCollection( NXU::NxuPhysicsCollection* coll, int addOptions );
		virtual void addNxParameterDesc( nxu_coll_traits<NXU::NxParameterDesc>::in desc, int addOptions );
		virtual void addNxConvexMeshDesc( nxu_coll_traits<NXU::NxConvexMeshDesc>::in desc, int addOptions );
		virtual void addNxTriangleMeshDesc( nxu_coll_traits<NXU::NxTriangleMeshDesc>::in desc, int addOptions );
		virtual void addNxHeightFieldDesc( nxu_coll_traits<NXU::NxHeightFieldDesc>::in desc, int addOptions );
		virtual void addNxCCDSceletonDesc( nxu_coll_traits<NXU::NxCCDSkeletonDesc>::in desc, int addOptions );
		virtual void addNxClothMeshDesc( nxu_coll_traits<NXU::NxClothMeshDesc>::in desc, int addOptions );
		virtual void addNxSceneDesc( nxu_coll_traits<NXU::NxSceneDesc>::in desc, int addOptions );
#if NX_USE_SOFTBODY_API
		virtual void addNxSoftBodyMeshDesc( nxu_coll_traits<NXU::NxSoftBodyMeshDesc>::in desc, int addOptions );
#endif

		virtual nxu_coll_traits<NXU::NxParameterDesc>::out getNxParameterDesc( string_traits::in name );
		virtual nxu_coll_traits<NXU::NxConvexMeshDesc>::out getNxConvexMeshDesc( string_traits::in name );
		virtual nxu_coll_traits<NXU::NxTriangleMeshDesc>::out getNxTriangleMeshDesc( string_traits::in name );
		virtual nxu_coll_traits<NXU::NxHeightFieldDesc>::out getNxHeightFieldDesc( string_traits::in name );
		virtual nxu_coll_traits<NXU::NxCCDSkeletonDesc>::out getNxCCDSkeletonDesc( string_traits::in name );
		virtual nxu_coll_traits<NXU::NxClothMeshDesc>::out getNxClothMeshDesc( string_traits::in name );
		virtual nxu_coll_traits<NXU::NxSceneDesc>::out getNxSceneDesc( string_traits::in name );
#if NX_USE_SOFTBODY_API
		virtual nxu_coll_traits<NXU::NxSoftBodyMeshDesc>::out getNxSoftBodyMeshDesc( string_traits::in name );
#endif

	protected:
		nxu_coll_traits<NXU::NxParameterDesc>::cont parameterDescCont;
		nxu_coll_traits<NXU::NxConvexMeshDesc>::cont convexMeshDescCont;
		nxu_coll_traits<NXU::NxTriangleMeshDesc>::cont triangleMeshDescCont;
		nxu_coll_traits<NXU::NxHeightFieldDesc>::cont heightFieldDescCont;
		nxu_coll_traits<NXU::NxCCDSkeletonDesc>::cont ccdSkeletonDescCont;
		nxu_coll_traits<NXU::NxClothMeshDesc>::cont clothMeshDescCont;
		nxu_coll_traits<NXU::NxSceneDesc>::cont sceneDescCont;
#if NX_USE_SOFTBODY_API
		nxu_coll_traits<NXU::NxSoftBodyMeshDesc>::cont softBodyMeshDesc;
#endif
	};
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
}
}
#endif //DBS_LM_NXU_COLLECTIONS
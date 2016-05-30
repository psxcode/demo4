#pragma once
#if DBS_LM_NXU_COLLECTIONS

#include <OgreScriptLoader.h>
#include "Nxu/NXU_schema.h"
#include "collection_traits.h"

namespace Demo {
namespace System {
	//------------------------------------------------------------
	template<typename T>
	class nxu_coll_traits
	{
	public:
		typedef typename collection_traits< T, COST_EXPLICIT_SHARED_PTR >::container cont;
		typedef typename collection_traits< T, COST_EXPLICIT_SHARED_PTR >::in in;
		typedef typename collection_traits< T, COST_EXPLICIT_SHARED_PTR >::out out;
		typedef typename collection_traits< T, COST_EXPLICIT_SHARED_PTR >::store_type store_t;
		typedef typename collection_traits< T, COST_EXPLICIT_SHARED_PTR >::store_const_type store_ct;
		static inline store_t Create(){ return store_t( new T ); }
		static inline store_t Create( T *obj ){ return store_t( new T(*obj) ); }
	};
	//------------------------------------------------------------
	class NxuCollection_LoaderInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(NxuCollection_LoaderInterface)
		//--------------------------------//
		enum ADD_OPTIONS
		{
			AO_NO_OPTIONS = 0x0,
			AO_DONT_REPLACE = 0x01,
			AO_SKIP_PARAMETER = 0x10,
			AO_SKIP_CONVEX_MESH = 0x20,
			AO_SKIP_TRIANGLE_MESH = 0x40,
			AO_SKIP_HEIGHT_FIELD = 0x80,
			AO_SKIP_CCDSKELETON = 0x100,
			AO_SKIP_CLOTH_MESH = 0x200,
			AO_SKIP_SCENE = 0x400,
			AO_SKIP_SOFTBODY_MESH = 0x800
		};
		//--------------------------------//
		virtual ~NxuCollection_LoaderInterface() {}

		virtual void addNxuPhysicsCollection( NXU::NxuPhysicsCollection* coll, int addOptions ) = 0;
		virtual void addNxParameterDesc( nxu_coll_traits<NXU::NxParameterDesc>::in desc, int addOptions ) = 0;
		virtual void addNxConvexMeshDesc( nxu_coll_traits<NXU::NxConvexMeshDesc>::in desc, int addOptions ) = 0;
		virtual void addNxTriangleMeshDesc( nxu_coll_traits<NXU::NxTriangleMeshDesc>::in desc, int addOptions ) = 0;
		virtual void addNxHeightFieldDesc( nxu_coll_traits<NXU::NxHeightFieldDesc>::in desc, int addOptions ) = 0;
		virtual void addNxCCDSceletonDesc( nxu_coll_traits<NXU::NxCCDSkeletonDesc>::in desc, int addOptions ) = 0;
		virtual void addNxClothMeshDesc( nxu_coll_traits<NXU::NxClothMeshDesc>::in desc, int addOptions ) = 0;
		virtual void addNxSceneDesc( nxu_coll_traits<NXU::NxSceneDesc>::in desc, int addOptions ) = 0;
#if NX_USE_SOFTBODY_API
		virtual void addNxSoftBodyMeshDesc( nxu_coll_traits<NXU::NxSoftBodyMeshDesc>::in desc, int addOptions ) = 0;
#endif
	};
	//------------------------------------------------------------
	class NxuCollection_FactoryInterface : public NxuCollection_LoaderInterface
	{
	public:
		DEFINE_CLASS_SHARED_PTR(NxuCollection_FactoryInterface)
		//--------------------------------//
		virtual ~NxuCollection_FactoryInterface() {}

		virtual nxu_coll_traits<NXU::NxParameterDesc>::out getNxParameterDesc( string_traits::in name ) = 0;
		virtual nxu_coll_traits<NXU::NxConvexMeshDesc>::out getNxConvexMeshDesc( string_traits::in name ) = 0;
		virtual nxu_coll_traits<NXU::NxTriangleMeshDesc>::out getNxTriangleMeshDesc( string_traits::in name ) = 0;
		virtual nxu_coll_traits<NXU::NxHeightFieldDesc>::out getNxHeightFieldDesc( string_traits::in name ) = 0;
		virtual nxu_coll_traits<NXU::NxCCDSkeletonDesc>::out getNxCCDSkeletonDesc( string_traits::in name ) = 0;
		virtual nxu_coll_traits<NXU::NxClothMeshDesc>::out getNxClothMeshDesc( string_traits::in name ) = 0;
		virtual nxu_coll_traits<NXU::NxSceneDesc>::out getNxSceneDesc( string_traits::in name ) = 0;
#if NX_USE_SOFTBODY_API
		virtual nxu_coll_traits<NXU::NxSoftBodyMeshDesc>::out getNxSoftBodyMeshDesc( string_traits::in name ) = 0;
#endif
	};
	//------------------------------------------------------------
	class NxuLoader_CollectionInterface : public Ogre::ScriptLoader
	{
	public:
		DEFINE_CLASS_SHARED_PTR(NxuLoader_CollectionInterface)
		//--------------------------------//
		class traits
		{
		public:
			typedef type_traits< NxuCollection_LoaderInterface::shared_ptr >::cref nxu_coll_in;
		};
		//--------------------------------//
		virtual ~NxuLoader_CollectionInterface() {}

		virtual void addCollection( traits::nxu_coll_in vehicleCollection, int collAddOptions ) = 0;
		virtual void removeCollection( traits::nxu_coll_in vehicleCollection ) = 0;
	};
	//------------------------------------------------------------
}
}
#endif //DBS_LM_NXU_COLLECTIONS
#include "DemoStableHeaders.h"
#if DBS_LM_NXU_COLLECTIONS

#include "DefaultNxuCollection.h"
#include "Nxu/NXU_helper.h"

namespace Demo {
namespace System {
//------------------------------------------------------------
DefaultNxuLoader::DefaultNxuLoader()
{
	mScriptPatterns.push_back("*.nxu");
}
//------------------------------------------------------------
/*virtual*/ DefaultNxuLoader::~DefaultNxuLoader()
{
}
//------------------------------------------------------------
/*virtual*/ void DefaultNxuLoader::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
{
	NXU::NxuPhysicsCollection* coll = NXU::loadCollection(stream, NXU::FT_XML);
	if( coll )
	{
		traits::nxu_coll_cont::iterator it = collectionCont.begin();
		traits::nxu_coll_cont::iterator eit = collectionCont.end();
		for( ; it != eit; ++it )
		{
			it->first->addNxuPhysicsCollection( coll, it->second );
		}
	}
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
DefaultNxuCollection::DefaultNxuCollection()
{
}
//------------------------------------------------------------
/*virtual*/ DefaultNxuCollection::~DefaultNxuCollection()
{
}
//------------------------------------------------------------
/*virtual*/ void DefaultNxuCollection::addNxuPhysicsCollection( NXU::NxuPhysicsCollection* coll, int addOptions )
{
	if( !coll ) return;

	if( (addOptions & AO_SKIP_CONVEX_MESH) == 0 )
	{
		NxArray< NXU::NxConvexMeshDesc* >::iterator it = coll->mConvexMeshes.begin();
		NxArray< NXU::NxConvexMeshDesc* >::iterator eit = coll->mConvexMeshes.end();
		for( ; it != eit; ++it )
			addNxConvexMeshDesc( nxu_coll_traits<NXU::NxConvexMeshDesc>::Create(*it), addOptions );
	}

	if( (addOptions & AO_SKIP_TRIANGLE_MESH) == 0 )
	{
		NxArray< NXU::NxTriangleMeshDesc* >::iterator it = coll->mTriangleMeshes.begin();
		NxArray< NXU::NxTriangleMeshDesc* >::iterator eit = coll->mTriangleMeshes.end();
		for( ; it != eit; ++it )
			addNxTriangleMeshDesc( nxu_coll_traits<NXU::NxTriangleMeshDesc>::Create(*it), addOptions );
	}

	if( (addOptions & AO_SKIP_HEIGHT_FIELD) == 0 )
	{
		NxArray< NXU::NxHeightFieldDesc* >::iterator it = coll->mHeightFields.begin();
		NxArray< NXU::NxHeightFieldDesc* >::iterator eit = coll->mHeightFields.end();
		for( ; it != eit; ++it )
			addNxHeightFieldDesc( nxu_coll_traits<NXU::NxHeightFieldDesc>::Create(*it), addOptions );
	}

	if( (addOptions & AO_SKIP_CLOTH_MESH) == 0 )
	{
		NxArray< NXU::NxClothMeshDesc* >::iterator it = coll->mClothMeshes.begin();
		NxArray< NXU::NxClothMeshDesc* >::iterator eit = coll->mClothMeshes.end();
		for( ; it != eit; ++it )
			addNxClothMeshDesc( nxu_coll_traits<NXU::NxClothMeshDesc>::Create(*it), addOptions );
	}

	if( (addOptions & AO_SKIP_CCDSKELETON) == 0 )
	{
		NxArray< NXU::NxCCDSkeletonDesc* >::iterator it = coll->mSkeletons.begin();
		NxArray< NXU::NxCCDSkeletonDesc* >::iterator eit = coll->mSkeletons.end();
		for( ; it != eit; ++it )
			addNxCCDSceletonDesc( nxu_coll_traits<NXU::NxCCDSkeletonDesc>::Create(*it), addOptions );
	}
#if NX_USE_SOFTBODY_API
	if( (addOptions & AO_SKIP_SOFTBODY_MESH) != 0 )
	{
		NxArray< NXU::NxSoftBodyMeshDesc* >::iterator it = coll->mSoftBodyMeshes.begin();
		NxArray< NXU::NxSoftBodyMeshDesc* >::iterator eit = coll->mSoftBodyMeshes.end();
		for( ; it != eit; ++it )
			addNxSoftBodyMeshDesc( nxu_coll_traits<NXU::NxSoftBodyMeshDesc>::Create(*it), addOptions );
	}
#endif
}
//------------------------------------------------------------
/*virtual*/ void DefaultNxuCollection::addNxParameterDesc( nxu_coll_traits<NXU::NxParameterDesc>::in desc, int addOptions )
{
}
//------------------------------------------------------------
/*virtual*/ void DefaultNxuCollection::addNxConvexMeshDesc( nxu_coll_traits<NXU::NxConvexMeshDesc>::in desc, int addOptions )
{
	string_traits::str name = desc->mId;
	if( (addOptions & AO_DONT_REPLACE) == 0 ) convexMeshDescCont.erase(name);
	convexMeshDescCont.insert(std::make_pair(name, desc));
}
//------------------------------------------------------------
/*virtual*/ void DefaultNxuCollection::addNxTriangleMeshDesc( nxu_coll_traits<NXU::NxTriangleMeshDesc>::in desc, int addOptions )
{
	string_traits::str name = desc->mId;
	if( (addOptions & AO_DONT_REPLACE) == 0 ) triangleMeshDescCont.erase(name);
	triangleMeshDescCont.insert(std::make_pair(name, desc));
}
//------------------------------------------------------------
/*virtual*/ void DefaultNxuCollection::addNxSceneDesc( nxu_coll_traits<NXU::NxSceneDesc>::in desc, int addOptions )
{
	string_traits::str name = desc->mId;
	if( (addOptions & AO_DONT_REPLACE) == 0 ) sceneDescCont.erase(name);
	sceneDescCont.insert(std::make_pair(name, desc));
}
//------------------------------------------------------------
/*virtual*/ void DefaultNxuCollection::addNxHeightFieldDesc( nxu_coll_traits<NXU::NxHeightFieldDesc>::in desc, int addOptions )
{
	string_traits::str name = desc->mId;
	if( (addOptions & AO_DONT_REPLACE) == 0 ) heightFieldDescCont.erase(name);
	heightFieldDescCont.insert(std::make_pair(name, desc));
}
//------------------------------------------------------------
/*virtual*/ void DefaultNxuCollection::addNxClothMeshDesc( nxu_coll_traits<NXU::NxClothMeshDesc>::in desc, int addOptions )
{
	string_traits::str name = desc->mId;
	if( (addOptions & AO_DONT_REPLACE) == 0 ) clothMeshDescCont.erase(name);
	clothMeshDescCont.insert(std::make_pair(name, desc));
}
//------------------------------------------------------------
/*virtual*/ void DefaultNxuCollection::addNxCCDSceletonDesc( nxu_coll_traits<NXU::NxCCDSkeletonDesc>::in desc, int addOptions )
{
	string_traits::str name = desc->mId;
	if( (addOptions & AO_DONT_REPLACE) == 0 ) ccdSkeletonDescCont.erase(name);
	ccdSkeletonDescCont.insert(std::make_pair(name, desc));
}
//------------------------------------------------------------
#if NX_USE_SOFTBODY_API
/*virtual*/ void DefaultNxuCollection::addNxSoftBodyMeshDesc( nxu_coll_traits<NXU::NxSoftBodyMeshDesc>::in desc, int addOptions )
{
	string_traits::str name = desc->mId;
	if( (addOptions & AO_DONT_REPLACE) == 0 ) softBodyMeshDesc.erase(name);
	softBodyMeshDesc.insert(std::make_pair(name, desc));
}
#endif
//------------------------------------------------------------
/*virtual*/ nxu_coll_traits<NXU::NxParameterDesc>::out DefaultNxuCollection::getNxParameterDesc( string_traits::in name )
{
	nxu_coll_traits<NXU::NxParameterDesc>::cont::iterator iter = parameterDescCont.find(name);
	if( iter != parameterDescCont.end() ) return iter->second;
	return nxu_coll_traits<NXU::NxParameterDesc>::out();
}
//------------------------------------------------------------
/*virtual*/ nxu_coll_traits<NXU::NxConvexMeshDesc>::out DefaultNxuCollection::getNxConvexMeshDesc( string_traits::in name )
{
	nxu_coll_traits<NXU::NxConvexMeshDesc>::cont::iterator iter = convexMeshDescCont.find(name);
	if( iter != convexMeshDescCont.end() ) return iter->second;
	return nxu_coll_traits<NXU::NxConvexMeshDesc>::out();
}
//------------------------------------------------------------
/*virtual*/ nxu_coll_traits<NXU::NxTriangleMeshDesc>::out DefaultNxuCollection::getNxTriangleMeshDesc( string_traits::in name )
{
	nxu_coll_traits<NXU::NxTriangleMeshDesc>::cont::iterator iter = triangleMeshDescCont.find(name);
	if( iter != triangleMeshDescCont.end() ) return iter->second;
	return nxu_coll_traits<NXU::NxTriangleMeshDesc>::out();
}
//------------------------------------------------------------
/*virtual*/ nxu_coll_traits<NXU::NxHeightFieldDesc>::out DefaultNxuCollection::getNxHeightFieldDesc( string_traits::in name )
{
	nxu_coll_traits<NXU::NxHeightFieldDesc>::cont::iterator iter = heightFieldDescCont.find(name);
	if( iter != heightFieldDescCont.end() ) return iter->second;
	return nxu_coll_traits<NXU::NxHeightFieldDesc>::out();
}
//------------------------------------------------------------
/*virtual*/ nxu_coll_traits<NXU::NxCCDSkeletonDesc>::out DefaultNxuCollection::getNxCCDSkeletonDesc( string_traits::in name )
{
	nxu_coll_traits<NXU::NxCCDSkeletonDesc>::cont::iterator iter = ccdSkeletonDescCont.find(name);
	if( iter != ccdSkeletonDescCont.end() ) return iter->second;
	return nxu_coll_traits<NXU::NxCCDSkeletonDesc>::out();
}
//------------------------------------------------------------
/*virtual*/ nxu_coll_traits<NXU::NxClothMeshDesc>::out DefaultNxuCollection::getNxClothMeshDesc( string_traits::in name )
{
	nxu_coll_traits<NXU::NxClothMeshDesc>::cont::iterator iter = clothMeshDescCont.find(name);
	if( iter != clothMeshDescCont.end() ) return iter->second;
	return nxu_coll_traits<NXU::NxClothMeshDesc>::out();
}
//------------------------------------------------------------
/*virtual*/ nxu_coll_traits<NXU::NxSceneDesc>::out DefaultNxuCollection::getNxSceneDesc( string_traits::in name )
{
	nxu_coll_traits<NXU::NxSceneDesc>::cont::iterator iter = sceneDescCont.find(name);
	if( iter != sceneDescCont.end() ) return iter->second;
	return nxu_coll_traits<NXU::NxSceneDesc>::out();
}
//------------------------------------------------------------
#if NX_USE_SOFTBODY_API
/*virtual*/ nxu_coll_traits<NXU::NxSoftBodyMeshDesc>::out DefaultNxuCollection::getNxSoftBodyMeshDesc( string_traits::in name )
{
	nxu_coll_traits<NXU::NxSoftBodyMeshDesc>::cont::iterator iter = softBodyMeshDesc.find(name);
	if( iter != softBodyMeshDesc.end() ) return iter->second;
	return nxu_coll_traits<NXU::NxSoftBodyMeshDesc>::out();
}
#endif
//------------------------------------------------------------
}
}
#endif //DBS_LM_NXU_COLLECTIONS
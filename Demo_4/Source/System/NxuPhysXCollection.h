#pragma once

#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX_NXU_COLLECTIONS

#include <NxTriangleMeshDesc.h>
#include <NxConvexMeshDesc.h>
#include <NxMaterialDesc.h>
#include <NxArray.h>

#include "string_traits.h"
#include "External\Code\PhysX\NXU_SchemaTypes.h"

class SchemaStream;
namespace Demo
{
namespace System
{
//------------------------------------------------------------
class NxuPhysXCollection
{
public:
	//--------------------------------//	
	typedef Demo::string_traits<Ogre::String>::str String;
	typedef Demo::string_traits<Ogre::String>::inpar string_traits::in;
	//--------------------------------//
	typedef Ogre::map< String, NxuTriangleMeshDesc::shared_ptr >::type TriMeshMap;
	typedef TriMeshMap::iterator TMIter;
	typedef TriMeshMap::const_iterator cTMIter;
	typedef std::pair< String, NxuTriangleMeshDesc::shared_ptr > TriMeshPair;
	typedef std::pair< TMIter, bool > TriMeshInsertPair;

	typedef Ogre::map< String, NxuConvexMeshDesc::shared_ptr >::type ConvMeshMap;
	typedef ConvMeshMap::iterator CMIter;
	typedef ConvMeshMap::const_iterator cCMIter;
	typedef std::pair< String, NxuConvexMeshDesc::shared_ptr > ConvMeshPair;
	typedef std::pair< CMIter, bool > ConvMeshInsertPair;
	//--------------------------------//
	bool loadCollectionFromOgre( string_traits::in filename );
	bool loadCollection( char* mem, int len, NXU_FileType );

	NxuConvexMeshDesc::shared_ptr GetConvexMeshDesc( string_traits::in name );
	NxuConvexMeshDesc::shared_ptr GetConvexMeshDesc( int index );
	NxuTriangleMeshDesc::shared_ptr GetTriangleMeshDesc( string_traits::in name );
	NxuTriangleMeshDesc::shared_ptr GetTriangleMeshDesc( int index );

private:
	NxuPhysXCollection();
	~NxuPhysXCollection();
	NxuPhysXCollection( const NxuPhysXCollection& );
	NxuPhysXCollection& operator=( const NxuPhysXCollection& );

	void load( SchemaStream &stream );

	TriMeshMap triMeshMap;
	ConvMeshMap convMeshMap;
};
//------------------------------------------------------------
}
}
#endif
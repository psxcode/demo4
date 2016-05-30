#include "NxuPhysXCollection.h"
#if DBS_LM_PHYSX_NXU_COLLECTIONS

#include <OgreDataStream.h>
#include <OgreResourceGroupManager.h>
#include "External\Code\PhysX\NXU_File.h"
#include "External\Code\PhysX\NXU_SchemaStream.h"

namespace Demo
{
namespace System
{

//------------------------------------------------------------
NxuConvexMeshDesc::NxuConvexMeshDesc()
	: mId(0)
	, mUserProperties(0)
	, mCookedDataSize(0)
{
	NxConvexMeshDesc def;
	flags = def.flags;
}
//------------------------------------------------------------
NxuConvexMeshDesc::~NxuConvexMeshDesc()
{
}
//------------------------------------------------------------
void NxuConvexMeshDesc::copyTo( NxConvexMeshDesc &desc )
{
	desc.numVertices			= mPoints.size();
	desc.numTriangles			= mTriangles.size();
	desc.pointStrideBytes		= sizeof( NxVec3 );
	desc.triangleStrideBytes	= sizeof( NxTri );

	if( desc.numVertices != 0 )
		desc.points = &mPoints[0].x;

	if( desc.numTriangles != 0 )
		desc.triangles = &mTriangles[0].a;

	desc.flags = flags;
}
//------------------------------------------------------------
void NxuConvexMeshDesc::load( SchemaStream &stream )
{
	if( stream.beginHeader( SC_NxConvexMeshDesc, 0 ) )
	{
		stream.load( mId, "mId", true );
		stream.load( mUserProperties, "mUserProperties", true );
		stream.load( mPoints, "mPoints", false );
		stream.load( mTriangles, "mTriangles", false );
		if( stream.isBinary() )
		{
			NxU32 v;
			stream.load( v, "flags", false );
			flags = v;
		}
		else
		{
			if( stream.beginHeader( SC_NxConvexFlags, "flags" ) )
			{
				/*
				flags = 0;
				for( int i = 0; i < 32; ++i )
				{
					int shift = (1<<i);
					const char *str = 
				}
				*/
				stream.endHeader();
			}
		}
		stream.load( mCookedDataSize, "mCookedDataSize", false );
		stream.load( mCookedData, "mCookedData", false );
		stream.endHeader();
	}
}
//------------------------------------------------------------
NxuSimpleTriangleMesh::NxuSimpleTriangleMesh()
{
}
//------------------------------------------------------------
NxuSimpleTriangleMesh::~NxuSimpleTriangleMesh()
{
}
//------------------------------------------------------------
void NxuSimpleTriangleMesh::copyTo( NxSimpleTriangleMesh &mesh )
{
	mesh.flags = flags;
	mesh.numVertices = mPoints.size();
	mesh.numTriangles = mTriangles.size();
	mesh.pointStrideBytes = sizeof(NxVec3);
	mesh.triangleStrideBytes = sizeof(NxTri);
	if( mesh.numVertices != 0 )
		mesh.points = &mPoints[0].x;
	if( mesh.numTriangles != 0 )
		mesh.triangles = &mTriangles[0].a;
}
//------------------------------------------------------------
void NxuSimpleTriangleMesh::load( SchemaStream &stream )
{
	if( stream.beginHeader( SC_NxSimpleTriangleMesh ) )
	{
		stream.load( mPoints, "mPoints", false );
		stream.load( mTriangles, "mTriangles", false );

		if( stream.isBinary() )
		{
			NxU32 v;
			stream.load( v, "flags", false );
			flags = v;
		}
		else
		{
			if( stream.beginHeader( SC_NxMeshFlags, "flags" ) )
			{
				flags = 0;
				for( int i = 0; i < 32; ++i )
				{
				}
				stream.endHeader();
			}
		}
		stream.endHeader();
	}
}
//------------------------------------------------------------
NxuTriangleMeshDesc::NxuTriangleMeshDesc()
	: mId(0)
	, mUserProperties(0)
	, mPmapSize(0)
	, mPmapDensity(0)
	, mCookedDataSize(0)
{
	NxTriangleMeshDesc def;
	heightFieldVerticalAxis = def.heightFieldVerticalAxis;
	heightFieldVerticalExtent = def.heightFieldVerticalExtent;
	convexEdgeThreshold = def.convexEdgeThreshold;
}
//------------------------------------------------------------
NxuTriangleMeshDesc::~NxuTriangleMeshDesc()
{
}
//------------------------------------------------------------
void NxuTriangleMeshDesc::copyTo( NxTriangleMeshDesc &desc )
{
	NxuSimpleTriangleMesh::copyTo( desc );

	desc.heightFieldVerticalAxis = heightFieldVerticalAxis;
	desc.heightFieldVerticalExtent = heightFieldVerticalExtent;
	desc.convexEdgeThreshold = convexEdgeThreshold;

	if( mCookedDataSize == 0 )
	{
		desc.materialIndexStride = sizeof(NxU32);
		desc.materialIndices = &mMaterialIndices[0];
	}
}
//------------------------------------------------------------
void NxuTriangleMeshDesc::load(SchemaStream &stream)
{
	if( stream.beginHeader( SC_NxTriangleMeshDesc ) )
	{
		stream.load( mId, "mId", true );
		stream.load( mUserProperties, "mUserProperties", true );
		stream.load( mMaterialIndices, "mMaterialIndices", false );
		if( stream.isBinary() )
		{
			NxU32 temp;
			stream.load( temp, "heightFieldVerticalAxis", false );
			heightFieldVerticalAxis = (NxHeightFieldAxis)temp;
		}
		else
		{
			const char *enumName(0);
			stream.load( enumName, "heightFieldVerticalAxis", false );
			StringToEnum( enumName, heightFieldVerticalAxis );
		}
		stream.load( heightFieldVerticalExtent, "heightFieldVerticalExtent", false );
		stream.load( mPmapSize, "mPmapSize", false );
		stream.load( mPmapDensity, "mPmapDensity", false );
		stream.load( mPmapData, "mPmapData", false );
		stream.load( convexEdgeThreshold, "convexEdgeThreshold", false );
		stream.load( mCookedDataSize, "mCookedDataSize", false );
		stream.load( mCookedData, "mCookedData", false );

		NxuSimpleTriangleMesh::load( stream );
		stream.endHeader();
	}
}
//------------------------------------------------------------
bool NxuPhysXCollection::loadCollectionFromOgre( string_traits::in filename )
{
	Ogre::DataStreamPtr stream;
	stream = Ogre::ResourceGroupManager::getSingleton().openResource( filename, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );
	if( stream.isNull() ) return false;
	String strData = stream->getAsString();
	const char* buffer = strData.c_str();
	return loadCollection( const_cast<char*>(buffer), strlen( buffer ), NXUFT_XML );
}
//------------------------------------------------------------
bool NxuPhysXCollection::loadCollection( char *mem, int len, NXU_FileType type )
{
	bool binary = (type == NXUFT_BINARY);

	SchemaStream ss( binary, "rb", mem, len );
	if( ss.isValid() )
	{
		load(ss);
		return true;
	}
	return false;
}
//------------------------------------------------------------
NxuPhysXCollection::NxuPhysXCollection()
{
}
//------------------------------------------------------------
NxuPhysXCollection::~NxuPhysXCollection()
{
}
//------------------------------------------------------------
void NxuPhysXCollection::load(SchemaStream &stream)
{
	stream.setCurrentCollection( this );
	if( stream.beginHeader( SC_NxuPhysicsCollection ) )
	{
		while( stream.peekHeader( SC_NxConvexMeshDesc ) )
		{
			NxuConvexMeshDesc::shared_ptr temp( new NxuConvexMeshDesc );
			temp->load( stream );
			String name( temp->mId );

			CMIter iter = convMeshMap.find( name );
			if( iter != convMeshMap.end() )
				convMeshMap.erase( iter );

			convMeshMap.insert( ConvMeshPair( name, temp ) );
		}
		while( stream.peekHeader( SC_NxTriangleMeshDesc ) )
		{
			NxuTriangleMeshDesc::shared_ptr temp( new NxuTriangleMeshDesc );
			temp->load( stream );
			String name( temp->mId );

			TMIter iter = triMeshMap.find( name );
			if( iter != triMeshMap.end() )
				triMeshMap.erase( iter );

			triMeshMap.insert( TriMeshPair( name, temp ) );
		}
		stream.endHeader();
	}
}
//------------------------------------------------------------
NxuConvexMeshDesc::shared_ptr NxuPhysXCollection::GetConvexMeshDesc( int index )
{
	if( index < 0 || index >= (int)convMeshMap.size() ) return NxuConvexMeshDesc::shared_ptr(0);

	CMIter iter = convMeshMap.begin();
	while( index-- > 0 ) ++iter;
	return iter->second;
}
//------------------------------------------------------------
NxuConvexMeshDesc::shared_ptr NxuPhysXCollection::GetConvexMeshDesc( string_traits::in name )
{
	if( name.empty() )return NxuConvexMeshDesc::shared_ptr(0);
	CMIter iter = convMeshMap.find( name );
	if( iter == convMeshMap.end() )return NxuConvexMeshDesc::shared_ptr(0);
	return iter->second;
}
//------------------------------------------------------------
NxuTriangleMeshDesc::shared_ptr NxuPhysXCollection::GetTriangleMeshDesc( int index )
{
	if( index < 0 || index >= (int)triMeshMap.size() ) return NxuTriangleMeshDesc::shared_ptr(0);

	TMIter iter = triMeshMap.begin();
	while( index-- > 0 ) ++iter;
	return iter->second;
}
//------------------------------------------------------------
NxuTriangleMeshDesc::shared_ptr NxuPhysXCollection::GetTriangleMeshDesc( string_traits::in name )
{
	if( name.empty() )return NxuTriangleMeshDesc::shared_ptr(0);
	TMIter iter = triMeshMap.find( name );
	if( iter == triMeshMap.end() )return NxuTriangleMeshDesc::shared_ptr(0);
	return iter->second;
}
//------------------------------------------------------------
}
}

#endif
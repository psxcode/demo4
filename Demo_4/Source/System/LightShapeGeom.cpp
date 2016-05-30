#include "DemoStableHeaders.h"
#if DBS_LM_DEFERRED_RENDERING

#include <OgreMeshManager.h>
#include <OgreSubMesh.h>
#include <OgreHardwareBufferManager.h>
#include "LightShapeGeom.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	void LightShapeGeom::createSphere( string_traits::in name
									, float radius
									, int rings
									, int segments
									, bool hasNormals
									, bool hasTexCoords )
	{
		Ogre::MeshPtr pSphere = Ogre::MeshManager::getSingleton().createManual( name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		Ogre::SubMesh *pSphereVertex = pSphere->createSubMesh();
		pSphere->sharedVertexData = new Ogre::VertexData();

		createSphere( pSphere->sharedVertexData, pSphereVertex->indexData, radius, rings, segments, hasNormals, hasTexCoords );

		pSphereVertex->useSharedVertices = true;

		pSphere->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3( -radius, -radius, -radius ), Ogre::Vector3( radius, radius, radius ) ), false );
		pSphere->_setBoundingSphereRadius( radius );

		// this line makes clear the mesh is loaded (avoids memory leaks)
		pSphere->load();
	}
	//------------------------------------------------------------
	void LightShapeGeom::createSphere(Ogre::VertexData *&vertexData, Ogre::IndexData *&indexData
									, float radius
									, int rings
									, int segments
									, bool hasNormals
									, bool hasTexCoords )
	{
		assert( vertexData && indexData );

		Ogre::VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
		std::size_t currOffset = 0;

		//positions
		vertexDecl->addElement( 0, currOffset, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
		currOffset += Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 );

		if( hasNormals )
		{
			vertexDecl->addElement( 0, currOffset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL );
			currOffset += Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 );
		}

		if( hasTexCoords )
		{
			vertexDecl->addElement( 0, currOffset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES );
			currOffset += Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT2 );
		}

		//allocate vertex buffer
		vertexData->vertexCount = ( rings + 1 ) * ( segments + 1 );
		Ogre::HardwareVertexBufferSharedPtr vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer( vertexDecl->getVertexSize(0), vertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false );
		Ogre::VertexBufferBinding *binding = vertexData->vertexBufferBinding;
		binding->setBinding( 0, vBuf );
		float *pVertex = static_cast< float* >( vBuf->lock( Ogre::HardwareBuffer::HBL_DISCARD ) );

		//allocate index buffer
		indexData->indexCount = 6 * rings * ( segments + 1 );
		indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer( Ogre::HardwareIndexBuffer::IT_16BIT, indexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false );
		Ogre::HardwareIndexBufferSharedPtr iBuf = indexData->indexBuffer;
		unsigned short *pIndices = static_cast< unsigned short* >( iBuf->lock( Ogre::HardwareBuffer::HBL_DISCARD ) );

		float deltaRingAngle = ( Ogre::Math::PI / rings );
		float deltaSegAngle = ( 2 * Ogre::Math::PI / segments );
		unsigned short wVerticeIndex = 0;

		//generate group of rings for sphere
		for( int ring = 0; ring <= rings; ++ring )
		{
			float r0 = radius * sinf( ring * deltaRingAngle );
			float y0 = radius * cosf( ring * deltaRingAngle );

			//generate group of segments for sphere
			for( int seg = 0; seg <= segments; ++seg )
			{
				float x0 = r0 * sinf( seg * deltaSegAngle );
				float z0 = r0 * cosf( seg * deltaSegAngle );

				//add one vertex to a strip which makes up the sphere
				*pVertex++ = x0;
				*pVertex++ = y0;
				*pVertex++ = z0;

				if( hasNormals )
				{
					Ogre::Vector3 vNormal = Ogre::Vector3( x0, y0, z0 ).normalisedCopy();
					*pVertex++ = vNormal.x;
					*pVertex++ = vNormal.y;
					*pVertex++ = vNormal.z;
				}

				if( hasTexCoords )
				{
					*pVertex++ = (float)seg / (float)segments;
					*pVertex++ = (float)ring / (float)rings;
				}

				if( ring != rings )
				{
					//each vertex (except the last) has six indices pointing to it
					*pIndices++ = wVerticeIndex + segments + 1;
					*pIndices++ = wVerticeIndex;
					*pIndices++ = wVerticeIndex + segments;
					*pIndices++ = wVerticeIndex + segments + 1;
					*pIndices++ = wVerticeIndex + 1;
					++wVerticeIndex;
				}
			}//end for seg
		}//end for ring

		//Unlock
		vBuf->unlock();
		iBuf->unlock();
	}
	//------------------------------------------------------------
	void LightShapeGeom::createCone( string_traits::in name, float radius, float height, int segments )
	{
		Ogre::MeshPtr pCone = Ogre::MeshManager::getSingleton().createManual( name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		Ogre::SubMesh *pConeVertex = pCone->createSubMesh();
		pCone->sharedVertexData = new Ogre::VertexData();

		createCone( pCone->sharedVertexData, pConeVertex->indexData, radius, height, segments );

		//generate face list
		pConeVertex->useSharedVertices = true;

		pCone->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3( -radius, 0, -radius ), Ogre::Vector3( radius, height, radius ) ), false );
		pCone->_setBoundingSphereRadius( Ogre::Math::Sqrt( height*height + radius*radius ) );

		pCone->load();
	}
	//------------------------------------------------------------
	void LightShapeGeom::createCone( Ogre::VertexData *&vertexData, Ogre::IndexData *&indexData, float radius, float height, int segments )
	{
		assert( vertexData && indexData );

		//define vertex format
		Ogre::VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;

		//positions
		vertexDecl->addElement( 0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );

		//allocate vertex buffer
		vertexData->vertexCount = segments + 1;
		Ogre::HardwareVertexBufferSharedPtr vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer( vertexDecl->getVertexSize(0), vertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false );
		Ogre::VertexBufferBinding *binding = vertexData->vertexBufferBinding;
		binding->setBinding( 0, vBuf );
		float *pVertex = static_cast< float* >( vBuf->lock( Ogre::HardwareBuffer::HBL_DISCARD ) );

		//allocate index buffer - cone and base
		indexData->indexCount = ( 3 * segments ) + ( 3 * ( segments - 2 ) );
		indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer( Ogre::HardwareIndexBuffer::IT_16BIT, indexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false );
		Ogre::HardwareIndexBufferSharedPtr iBuf = indexData->indexBuffer;
		unsigned short *pIndices = static_cast< unsigned short* >( iBuf->lock( Ogre::HardwareBuffer::HBL_DISCARD ) );

		//positions : cone head and base
		for( int i = 0; i < 3; ++i )
			*pVertex++ = 0.0f;

		//base
		float deltaBaseAngle = ( 2 * Ogre::Math::PI ) / segments;
		for( int i = 0; i < segments; ++i )
		{
			float angle = i * deltaBaseAngle;
			*pVertex++ = radius * cosf( angle );
			*pVertex++ = height;
			*pVertex++ = radius * sinf( angle );
		}

		//indices
		//cone head to vertices
		for( int i = 0; i < segments; ++i )
		{
			*pIndices++ = 0;
			*pIndices++ = ( i % segments ) + 1;
			*pIndices++ = ( (i+1) % segments ) + 1;
		}

		//cone base
		for( int i = 0; i < segments - 2; ++i )
		{
			*pIndices++ = 1;
			*pIndices++ = i + 3;
			*pIndices++ = i + 2;
		}

		//unlock
		vBuf->unlock();
		iBuf->unlock();
	}
	//------------------------------------------------------------
	void LightShapeGeom::createQuad( Ogre::VertexData*& vertexData )
	{
		assert( vertexData );

		vertexData->vertexCount = 4;
		vertexData->vertexStart = 0;

		Ogre::VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
		Ogre::VertexBufferBinding* binding = vertexData->vertexBufferBinding;

		vertexDecl->addElement( 0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );

		Ogre::HardwareVertexBufferSharedPtr vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer( vertexDecl->getVertexSize(0), vertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY );

		binding->setBinding( 0, vBuf );

		float data[]={
			-1,1,-1,
			-1,-1,-1,
			1,1,-1,
			1,-1,-1 };

		vBuf->writeData( 0, sizeof(data), data, true );
	}
	//------------------------------------------------------------
}
}
#endif
#include "OgrePhysXCooker.h"
#if DBS_LM_PHYSX

#include <Ogre.h>
#include <NxCooking.h>
#include "OgrePhysXStreams.h"
#include "OgrePhysXConvert.h"
#include "System/Nxu/NXU_Streaming.h"
#include "System/Nxu/NXU_Streaming.cpp"
#include "System/PhysXInterface.h"
//#include "NXU_File.cpp"

// if on the Windows platform and 2.5.0 or higher, use the versioned Cooking
// interface via PhysXLoader
#if	NX_SDK_VERSION_NUMBER	>= 250
#define	DBS_PHYSX_COOKING_INTERFACE	1
#endif

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
#ifdef DBS_PHYSX_COOKING_INTERFACE
	NxCookingInterface *gCooking = 0;
#endif
//------------------------------------------------------------
/*static*/ bool HasCookingLibrary()
{
	bool result = true;

#ifdef DBS_PHYSX_COOKING_INTERFACE
	if( gCooking == 0 )
	{
		gCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
		if( gCooking == 0 ) {
#if DBS_LOGGING_ENABLED
			Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: NxGetCookingLib failed.");
#endif
			result = false;
		}
	}
#endif

	return result;
}
//------------------------------------------------------------
/*static*/ bool InitCooking()
{
	bool result = false;
#ifdef DBS_PHYSX_COOKING_INTERFACE
	if( HasCookingLibrary() )
		result = gCooking->NxInitCooking();
#else
	result = NxInitCooking();
#endif

#if DBS_LOGGING_ENABLED
	if( !result )
		Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: Cooking initialization failed.");
#endif
	return result;
}
//------------------------------------------------------------
/*static*/ void CloseCooking()
{
#ifdef DBS_PHYSX_COOKING_INTERFACE
	if( gCooking == 0 )return;
	gCooking->NxCloseCooking();
#else
	return NxCloseCooking();
#endif
}
//------------------------------------------------------------
/*static*/ bool CookConvexMesh( const NxConvexMeshDesc& desc, NxStream& stream )
{
	bool result = false;
#ifdef DBS_PHYSX_COOKING_INTERFACE
	if( HasCookingLibrary() )
		result = gCooking->NxCookConvexMesh( desc, stream );
#else
	result = NxCookConvexMesh( desc, stream );
#endif

#if DBS_LOGGING_ENABLED
	if( !result )
		Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: CookConvexMesh failed.");
#endif
	return result;
}
//------------------------------------------------------------
/*static*/ bool CookTriangleMesh( const NxTriangleMeshDesc& desc, NxStream& stream )
{
	bool result = false;
#ifdef DBS_PHYSX_COOKING_INTERFACE
	if( HasCookingLibrary() )
		result = gCooking->NxCookTriangleMesh( desc, stream );
#else
	result = NxCookTriangleMesh( desc, stream );
#endif

#if DBS_LOGGING_ENABLED
	if( !result )
		Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: CookTriangleMesh failed.");
#endif
	return result;
}
//------------------------------------------------------------
/*static*/ bool Cooker::CookConvexMesh( traits::convex_mesh_desc_cont &mcont, traits::stream_cont &scont )
{
	_ASSERT( mcont.size() == scont.size() );
	auto mit = mcont.begin(); auto meit = mcont.end();
	auto sit = scont.begin(); auto seit = scont.end();
#ifdef DBS_PHYSX_COOKING_INTERFACE
	bool result = false;
	if( HasCookingLibrary() ) {
		result = true;
		for( ; mit != meit; ++mit, ++sit ) {
			if( !gCooking->NxCookConvexMesh(**mit, **sit) )
				result = false;
		}
	}
#else
	bool result = true;
	for( ; mit != meit; ++mit, ++sit ) {
		if( !NxCookConvexMesh(**mit, **sit) )
			result = false;
	}
#endif

#if DBS_LOGGING_ENABLED
	if( !result )
		Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: CookConvexMesh failed.");
#endif
	return result;
}
//------------------------------------------------------------
/*static*/ bool Cooker::CookTriangleMesh( traits::triangle_mesh_desc_cont &mcont, traits::stream_cont &scont )
{
	_ASSERT( mcont.size() == scont.size() );
	auto mit = mcont.begin(); auto meit = mcont.end();
	auto sit = scont.begin(); auto seit = scont.end();
#ifdef DBS_PHYSX_COOKING_INTERFACE
	bool result = false;
	if( HasCookingLibrary() ) {
		result = true;
		for( ; mit != meit; ++mit, ++sit ) {
			if( !gCooking->NxCookTriangleMesh(**mit, **sit) )
				result = false;
		}
	}
#else
	bool result = true;
	for( ; mit != meit; ++mit, ++sit ) {
		if( !NxCookTriangleMesh(**mit, **sit) )
			result = false;
	}
#endif

#if DBS_LOGGING_ENABLED
	if( !result )
		Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: CookTriangleMesh failed.");
#endif
	return result;
}
//------------------------------------------------------------
/*static*/ NxTriangleMesh* Cooker::loadNxMeshFromFile(string_traits::in nxsFile)
{
	Ogre::DataStreamPtr ds = Ogre::ResourceGroupManager::getSingleton().openResource(nxsFile);
	return System::PhysXInterface::getPhysicsSDK()->createTriangleMesh(OgreReadStream(ds));
}
//------------------------------------------------------------
/*static*/ void Cooker::cookNxMesh(Ogre::MeshPtr mesh, NxStream& outputStream, const Ogre::Vector3 &scale, traits::material_binding_cont &materialBindings)
{
	// Build the triangle mesh.
	NxTriangleMeshDesc meshDesc;
	meshDesc.numVertices                = 0;
	meshDesc.numTriangles               = 0;
	meshDesc.materialIndexStride		= sizeof(NxMaterialIndex);
	meshDesc.pointStrideBytes           = sizeof(NxVec3);
	meshDesc.triangleStrideBytes        = 3 * sizeof(NxU32);

	//First, we compute the total number of vertices and indices and create the buffers.
	if (mesh->sharedVertexData) meshDesc.numVertices += mesh->sharedVertexData->vertexCount;
	Ogre::Mesh::SubMeshIterator i = mesh->getSubMeshIterator();
	bool indices32 = true;
	while (i.hasMoreElements())
	{
		Ogre::SubMesh *subMesh = i.getNext();
		if (subMesh->vertexData) meshDesc.numVertices += subMesh->vertexData->vertexCount;
		meshDesc.numTriangles += (subMesh->indexData->indexCount / 3);
		//We assume, that every submesh uses the same index format
		indices32 = (subMesh->indexData->indexBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
	}

	NxArray<NxVec3> all_vertices;
	NxArray<NxU32> all_indices;
	NxArray<NxMaterialIndex> all_materialIndices;

	/*
	Read shared vertices
	*/
	unsigned int shared_index_offset = 0;
	Ogre::VertexData *shared_vertex_data = mesh->sharedVertexData;
	if (shared_vertex_data)
	{
        const Ogre::VertexElement* posElem =
            shared_vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
        Ogre::HardwareVertexBufferSharedPtr vbuf =
            shared_vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

		shared_index_offset = shared_vertex_data->vertexCount;

		unsigned char* pVertices = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		size_t added = all_vertices.size();
		all_vertices.resize(all_vertices.size()+shared_vertex_data->vertexCount);
		Ogre::Real* pReal;
		for (size_t i = added; i < all_vertices.size(); i++)
		{
			posElem->baseVertexPointerToElement(pVertices, &pReal);
			NxVec3 vec;
			vec.x = (*pReal++) * scale.x;
			vec.y = (*pReal++) * scale.y;
			vec.z = (*pReal++) * scale.z;
			all_vertices[i] = vec;
			pVertices += vbuf->getVertexSize();
		}
		vbuf->unlock();

	}

	unsigned int index_offset = 0;

	/*
	Read submeshes
	*/
	i = mesh->getSubMeshIterator();
	while (i.hasMoreElements())
	{
		Ogre::SubMesh *subMesh = i.getNext();

		//Read vertex data
		Ogre::VertexData *vertex_data = subMesh->vertexData;
		if (vertex_data)
		{
			const Ogre::VertexElement* posElem =
				vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			Ogre::HardwareVertexBufferSharedPtr vbuf =
				vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* pVertices = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			size_t added = all_vertices.size();
			all_vertices.resize(all_vertices.size()+vertex_data->vertexCount);
			Ogre::Real* pReal;
			for (size_t i = added; i < all_vertices.size(); i++)
			{
				posElem->baseVertexPointerToElement(pVertices, &pReal);
				NxVec3 vec;
				vec.x = (*pReal++) * scale.x;
				vec.y = (*pReal++) * scale.y;
				vec.z = (*pReal++) * scale.z;
				all_vertices[i] = vec;
				pVertices += vbuf->getVertexSize();
			}

			vbuf->unlock();
		}

		//Read index data
		Ogre::IndexData *index_data = subMesh->indexData;
		if (index_data)
		{
			Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

			NxU32 *pIndices = 0;
			if (indices32)
			{
				pIndices = static_cast<NxU32*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			}
			else
			{
				NxU16 *pShortIndices = static_cast<NxU16*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
				pIndices = new NxU32[index_data->indexCount];
				for (size_t k = 0; k < index_data->indexCount; k++) pIndices[k] = static_cast<NxU32>(pShortIndices[k]);
			}
			size_t added = all_indices.size();
			all_indices.resize(all_indices.size()+index_data->indexCount);
			unsigned int bufferIndex = 0;
			for (size_t i = added; i < all_indices.size(); i++)
			{
				if (subMesh->useSharedVertices)
				{
					if (pIndices[bufferIndex] > shared_index_offset) all_indices[i] = pIndices[bufferIndex] + index_offset;
					else all_indices[i] = pIndices[bufferIndex];
				}
				else all_indices[i] = pIndices[bufferIndex] + index_offset;
				bufferIndex++;
			}
			if (!indices32) delete pIndices;

			ibuf->unlock();

			//All triangles of a submesh have the same material.
			unsigned int numTris = index_data->indexCount / 3;
			added = all_materialIndices.size();
			all_materialIndices.resize(all_materialIndices.size()+numTris);
			auto i = materialBindings.find(subMesh->getMaterialName());
			NxMaterialIndex physXMat = (i != materialBindings.end()) ? i->second : 0;
			for (size_t i = added; i < all_materialIndices.size(); i++) all_materialIndices[i] = physXMat;
		}

		if (vertex_data) index_offset += vertex_data->vertexCount;

	}
		
	//dump the fucking buffers!
	/*for (unsigned int i = 0; i < meshDesc.numTriangles*3; i+=3)
		Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(all_indices[i]) + " " + Ogre::StringConverter::toString(all_indices[i+1]) + " " + Ogre::StringConverter::toString(all_indices[i+2]));

	for (unsigned int i = 0; i < meshDesc.numVertices; i++)
		Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(Convert::toOgre(all_vertices[i])));*/

	meshDesc.points = &all_vertices[0].x;
	meshDesc.triangles = &all_indices[0];
	meshDesc.materialIndices = &all_materialIndices[0];
	meshDesc.flags = 0;

	CookTriangleMesh(meshDesc, outputStream);
}
//------------------------------------------------------------
#if DBS_NXU_FILESYSTEM == DBS_NXU_FILESYSTEM_NATIVE
/*static*/ void Cooker::cookNxMeshToFile(Ogre::MeshPtr mesh, string_traits::in nxsOutputFile, const Ogre::Vector3 &scale, traits::material_binding_cont &materialBindings)
{
	cookNxMesh(mesh, NXU::UserStream(nxsOutputFile.c_str(), false), scale, materialBindings);
}
#endif
//------------------------------------------------------------
/*static*/ NxTriangleMesh* Cooker::getNxMesh(Ogre::MeshPtr mesh, const Ogre::Vector3 &scale, traits::material_binding_cont &materialBindings)
{
	NXU::MemoryWriteBuffer stream;
	cookNxMesh(mesh, stream, scale, materialBindings);
	return System::PhysXInterface::getPhysicsSDK()->createTriangleMesh(NXU::MemoryReadBuffer(stream.data));
}
//------------------------------------------------------------
}
}
#endif

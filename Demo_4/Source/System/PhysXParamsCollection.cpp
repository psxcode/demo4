#include "PhysXParamsCollection.h"

#if DBS_LM_PHYSX_COLLECTIONS

#include <OgreMeshManager.h>
#include <OgreSubmesh.h>
#include <NxTriangleMeshShapeDesc.h>
#include <NxConvexMeshDesc.h>
#include <NxCooking.h>

#include "Interface\DistanceUnitConverter.h"
#include "Interface\OgrePhysxConverter.h"

#include "External\Code\PhysX\Stream.h"

namespace Demo
{
	NxReal OgrePhysxConverter::torqueMechToPhysXMult(1.0f);
namespace System
{
	//------------------------------------------------------------
	typedef PhysXController PXC;
	typedef PhysXParamsCollection PPC;
	typedef OgrePhysxConverter OPC;
	typedef OgremaxTinyXmlInterface XI;
	typedef DistanceUnitConverter DUC;
	//------------------------------------------------------------
	struct PhysXKeys
	{
		static string_traits::cstr SDK;
		static string_traits::cstr SCENES;
		static string_traits::cstr SCENE;
		static string_traits::cstr NAME;
		static string_traits::cstr GRAVITY;
		static string_traits::cstr GROUND_PLANE;

		static string_traits::cstr MATERIALS;
		static string_traits::cstr MATERIAL;
		static string_traits::cstr MAT_STAT_FRIC;
		static string_traits::cstr MAT_DYN_FRIC;
		static string_traits::cstr MAT_RESTITUTION;

		static string_traits::cstr DEF_MAT_NAME;
		static string_traits::cstr DEF_SCENE_NAME;
		static string_traits::cstr DEFAULT_NAME;
		static string_traits::cstr TORQUE_MULT;

		static string_traits::cstr REMOTE_DEBUGGER;
		static string_traits::cstr RD_CONNECT;
		static string_traits::cstr RD_HOST_NAME;
		static string_traits::cstr RD_PORT;

		static string_traits::cstr NXU_FILE;
	};
	//------------------------------------------------------------
	typedef PhysXKeys PK;
	//------------------------------------------------------------
	string_traits::cstr PK::SDK = "sdk";
	string_traits::cstr PK::SCENE = "scene";
	string_traits::cstr PK::SCENES = "scenes";
	string_traits::cstr PK::MATERIAL = "material";
	string_traits::cstr PK::MATERIALS = "materials";
	string_traits::cstr PK::NAME = "name";
	string_traits::cstr PK::GRAVITY = "gravity";
	string_traits::cstr PK::GROUND_PLANE = "groundplane";
	string_traits::cstr PK::MAT_DYN_FRIC = "dynfric";
	string_traits::cstr PK::MAT_STAT_FRIC = "statfric";
	string_traits::cstr PK::MAT_RESTITUTION = "restitution";
	string_traits::cstr PK::DEF_MAT_NAME = "defmatname";
	string_traits::cstr PK::DEF_SCENE_NAME = "defscenename";
	string_traits::cstr PK::DEFAULT_NAME = "noname";
	string_traits::cstr PK::TORQUE_MULT = "torquemult";
	string_traits::cstr PK::REMOTE_DEBUGGER = "remotedebugger";
	string_traits::cstr PK::RD_CONNECT = "connect";
	string_traits::cstr PK::RD_HOST_NAME = "hostname";
	string_traits::cstr PK::RD_PORT = "port";
	string_traits::cstr PK::NXU_FILE = "nxufile";

	string_traits::str PPC::defaultMaterialName;
	string_traits::str PPC::defaultSceneName;
	//------------------------------------------------------------

#if DBS_LM_SCRIPT_OGREMAX_TIXML
	//------------------------------------------------------------
	void PPC::LoadMaterials( tixmlel_traits::in element, bool replace )
	{
		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == PK::MATERIAL )
				LoadMaterial( childElement, replace );
		}
	}
	//------------------------------------------------------------
	void PPC::LoadMaterial(tixmlel_traits::in element, bool replace)
	{
		string_traits::str name = XI::GetString( element, PK::NAME );
		if( name.empty() )return;

		NxMaterialDescPtr md( new NxMaterialDesc );
		md->staticFriction = XI::GetReal( element, PK::MAT_STAT_FRIC );
		md->dynamicFriction = XI::GetReal( element, PK::MAT_DYN_FRIC );
		md->restitution = XI::GetReal( element, PK::MAT_RESTITUTION );

		if( name == defaultMaterialName )
		{
			if( defaultMaterialDesc != 0 && !replace )return;
			defaultMaterialDesc = md;
		}
		else
		{
			MaterialContainer::iterator iter = materialContainer.find( name );
			if( iter != materialContainer.end() )
			{
				if( !replace )return;
				materialContainer.erase( iter );
			}
			materialContainer.insert( MaterialContainer::value_type( name, md ) );
		}
	}
	//------------------------------------------------------------
	void PPC::LoadScenes( tixmlel_traits::in element, bool replace )
	{
		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == PK::SCENE )
				LoadScene( childElement, replace );
		}
	}
	//------------------------------------------------------------
	void PPC::LoadScene(tixmlel_traits::in element, bool replace)
	{
		string_traits::str name = XI::GetString( element, PK::NAME );
		if( name.empty() )return;

		NxSceneDescPtr sd( new NxSceneDesc );
		sd->gravity.y = -DUC::DistanceToUnits( XI::GetReal( element, PK::GRAVITY ) );
		sd->groundPlane = XI::GetBool( element, PK::GROUND_PLANE );

		if( name == defaultSceneName )
		{
			if( defaultSceneDesc != 0 && !replace )return;
			defaultSceneDesc = sd;
		}
		else
		{
			SceneContainer::iterator iter = sceneContainer.find( name );
			if( iter != sceneContainer.end() )
			{
				if( !replace )return;
				sceneContainer.erase( iter );
			}
			sceneContainer.insert( SceneContainer::value_type( name, sd ) );
		}
	}
	//------------------------------------------------------------
	void PPC::LoadRDParams( tixmlel_traits::in element, bool replace )
	{
		if( !rdParams.hostName.empty() && !replace )return;
		rdParams.connect = XI::GetBool( element, PK::RD_CONNECT );
		rdParams.hostName = XI::GetString( element, PK::RD_HOST_NAME );
		rdParams.port = XI::GetUInt( element, PK::RD_PORT );
	}
	//------------------------------------------------------------
	void PPC::LoadSDK(tixmlel_traits::in element, bool replace)
	{
		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == PK::REMOTE_DEBUGGER )
				LoadRDParams( childElement, replace );
		}
	}
	//------------------------------------------------------------
	void PPC::AddParamsFrom(tixmlel_traits::in element)
	{
		defaultMaterialName = XI::GetString( element, PK::DEF_MAT_NAME );
		defaultSceneName = XI::GetString( element, PK::DEF_SCENE_NAME );
		if( defaultMaterialName.empty() )defaultMaterialName = PK::DEFAULT_NAME;
		if( defaultSceneName.empty() )defaultSceneName = PK::DEFAULT_NAME;
		OPC::torqueMechToPhysXMult = XI::GetReal( element, PK::TORQUE_MULT );
		nxuFilename = XI::GetString( element, PK::NXU_FILE );

		string_traits::str childName;
		const TiXmlElement* childElement(0);
		while( XI::IterateChildElements( element, childElement ) )
		{
			childName = childElement->Value();
			if( childName == PK::SDK )
			{
				LoadSDK( childElement );
			}
			else if( childName == PK::SCENES )
			{
				LoadScenes( childElement );
			}
			else if( childName == PK::MATERIALS )
			{
				LoadMaterials( childElement );
			}
		}
	}
	//------------------------------------------------------------
#endif

	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	PPC::NxMaterialDescPtr PPC::CreateMaterialDesc(string_traits::in name) const
	{
		if( name == defaultMaterialName )return defaultMaterialDesc;
		MaterialContainer::const_iterator iter = materialContainer.find( name );
		if( iter == materialContainer.end() )return PPC::NxMaterialDescPtr(0);
		return iter->second;
	}
	//------------------------------------------------------------
	PPC::NxSceneDescPtr PPC::CreateSceneDesc(string_traits::in name) const
	{
		if( name == defaultSceneName )return defaultSceneDesc;
		SceneContainer::const_iterator iter = sceneContainer.find( name );
		if( iter == sceneContainer.end() )return PPC::NxSceneDescPtr(0);
		return iter->second;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------

	//------------------------------------------------------------
	/*
	NxConvexMesh* PPC::PrepareConvexMesh(Ogre::MeshPtr mesh)
	{
		NxTriangleMeshShapeDesc shapeDesc;
		unsigned int vertexCount = 0;
		unsigned int indexCount = 0;
		bool added_shared = false;
		size_t current_offset = 0;
		size_t shared_offset = 0;
		size_t index_offset = 0;
		size_t next_offset = 0;

		//—читаем количество вершин и индексов
		for( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i )
		{
			Ogre::SubMesh* current_submesh = mesh->getSubMesh(i);
			if( current_submesh->useSharedVertices )
			{
				if ( !added_shared )
				{
					vertexCount += mesh->sharedVertexData->vertexCount;
					added_shared = true;
				}
			}
			else vertexCount += current_submesh->vertexData->vertexCount;
			indexCount += current_submesh->indexData->indexCount;
		}

		//—оздаЄм массивы
		Ogre::Vector3* vertices = new Ogre::Vector3[vertexCount];
		unsigned long* indices = new unsigned long[indexCount];
		NxVec3* meshVertices = new NxVec3[vertexCount];
		NxU32* meshFaces = new NxU32[indexCount];
		NxMaterialIndex* materials = new NxMaterialIndex[indexCount];

		NxMaterialIndex current_materialIndex = 0;
		added_shared = false;

		for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i )
		{
			Ogre::SubMesh* current_submesh = mesh->getSubMesh(i);
			Ogre::VertexData* current_vertex_data = current_submesh->useSharedVertices ? mesh->sharedVertexData : current_submesh->vertexData;
			if( !current_submesh->useSharedVertices || (current_submesh->useSharedVertices && !added_shared) )
			{
				if( current_submesh->useSharedVertices )
				{
					added_shared = true;
					shared_offset = current_offset;
				}
				const Ogre::VertexElement* posElem = current_vertex_data->vertexDeclaration->findElementBySemantic( Ogre::VES_POSITION );
				Ogre::HardwareVertexBufferSharedPtr vbuf = current_vertex_data->vertexBufferBinding->getBuffer( posElem->getSource() );
				unsigned char* vertex = static_cast< unsigned char* >( vbuf->lock( Ogre::HardwareBuffer::HBL_READ_ONLY ));

				float* temp_pos;

				for( size_t j = 0; j < current_vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize() )
				{
					posElem->baseVertexPointerToElement( vertex, &temp_pos );
					meshVertices[current_offset + j] = NxVec3( temp_pos[0], temp_pos[1], temp_pos[2] );
				}
				vbuf->unlock();
				next_offset += current_vertex_data->vertexCount;
			}
			Ogre::IndexData* current_index_data = current_submesh->indexData;
			size_t numTris = current_index_data->indexCount / 3;
			Ogre::HardwareIndexBufferSharedPtr ibuf = current_index_data->indexBuffer;

			bool use32bitindexes = ( ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT );

			if( use32bitindexes )
			{
				unsigned int* temp_index = static_cast< unsigned int* >( ibuf->lock( Ogre::HardwareBuffer::HBL_READ_ONLY ));
				size_t offset = current_submesh->useSharedVertices ? shared_offset : current_offset;

				for ( size_t k = 0; k < numTris*3; ++k )
				{
					meshFaces[index_offset] = temp_index[k] + static_cast< unsigned int >( offset );
					materials[index_offset++] = current_materialIndex;
				}
			}
			else
			{
				unsigned short* temp_index = reinterpret_cast< unsigned short* >( ibuf->lock( Ogre::HardwareBuffer::HBL_READ_ONLY ));
				size_t offset = current_submesh->useSharedVertices ? shared_offset : current_offset;

				for ( size_t k = 0; k < numTris*3; ++k )
				{
					meshFaces[index_offset] = static_cast< unsigned int >(temp_index[k] + static_cast< unsigned int >( offset ));
					materials[index_offset++] = current_materialIndex;
				}
			}
			ibuf->unlock();
			current_offset = next_offset;
		}

		NxConvexMeshDesc convexMeshDesc;
		convexMeshDesc.numVertices = vertexCount;
		convexMeshDesc.points = meshVertices;
		convexMeshDesc.pointStrideBytes = sizeof( NxVec3 );
		convexMeshDesc.numTriangles = indexCount / 3;
		convexMeshDesc.triangleStrideBytes = 3 * sizeof( NxU32 );
		convexMeshDesc.triangles = meshFaces;
		convexMeshDesc.flags = NX_CF_COMPUTE_CONVEX;

		MemoryWriteBuffer buf;

		bool resultOK = false;
		NxConvexMesh* convexMesh(0);
		
		resultOK = NxCookConvexMesh( convexMeshDesc, buf );
		if ( resultOK )
			convexMesh = PXC::GetSingleton().CreateConvexMesh( MemoryReadBuffer(buf.data) );

		delete [] vertices;
		delete [] indices;
		delete [] meshVertices;
		delete [] meshFaces;
		delete [] materials;

		return convexMesh;
	}
	*/
	//------------------------------------------------------------
}
}
#endif
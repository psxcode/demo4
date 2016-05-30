#pragma once
#include "DemoPrerequisites.h"
#if DBS_LM_PHYSX

#include <OgreMesh.h>
#include <NxMaterial.h>
#include <NxConvexMeshDesc.h>
#include <NxTriangleMeshDesc.h>
#include "shared_ptr_traits.h"

namespace Demo {
namespace OgrePhysX {
//------------------------------------------------------------
class Cooker
{
public:
	//--------------------------------//
	class traits
	{
	public:
		typedef Demo::map<string_traits::str, NxMaterialIndex>::type material_binding_cont;
		typedef Demo::vector< const NxConvexMeshDesc* >::type convex_mesh_desc_cont;
		typedef Demo::vector< const NxTriangleMeshDesc* >::type triangle_mesh_desc_cont;
		typedef Demo::vector< NxStream* >::type stream_cont;
	};
	//--------------------------------//
	static bool CookConvexMesh( const NxConvexMeshDesc&, NxStream& );
	static bool CookTriangleMesh( const NxTriangleMeshDesc&, NxStream& );
	static bool CookConvexMesh( traits::convex_mesh_desc_cont &mcont, traits::stream_cont &scont );
	static bool CookTriangleMesh( traits::triangle_mesh_desc_cont &mcont, traits::stream_cont &scont );
	
	/*
	loadNxMeshFromFile
	Loads a PhysX mesh from a nxs file. Throws an exception if the file is not found in the Ogre resource system.
	*/
	static NxTriangleMesh* loadNxMeshFromFile(string_traits::in nxsFile);

	/*
	cookNxMesh
	cooks an PhysX mesh from an Ogre mesh.
	*/
	static void cookNxMesh(
		Ogre::MeshPtr mesh,
		NxStream& outputStream,
		const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE,
		traits::material_binding_cont &materialBindings = traits::material_binding_cont()
		);
#if DBS_NXU_FILESYSTEM == DBS_NXU_FILESYSTEM_NATIVE
	static void cookNxMeshToFile(
		Ogre::MeshPtr mesh,
		string_traits::in nxsOutputFile,
		const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE,
		traits::material_binding_cont &materialBindings = traits::material_binding_cont()
		);
#endif

	/*
	getNxMesh
	Cooks an nx mesh from an ogre mesh and returns it, does not save to file.
	This is faster than getNxMesh(Ogre::MeshPtr, Ogre::String) if the mesh is not cooked yet, otherwise it is much slower.
	Example use case: This method is useful, if you want to rescale an actor with a triangle mesh shape in realtime
	*/
	static NxTriangleMesh* getNxMesh(
		Ogre::MeshPtr mesh,
		const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE,
		traits::material_binding_cont &materialBindings = traits::material_binding_cont()
		);
};
//------------------------------------------------------------
}
}
#endif
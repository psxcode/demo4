#pragma once
#if DBS_LM_PHYSX

#include <OgreMesh.h>
#include <NxMaterial.h>
#include <NxConvexMeshDesc.h>
#include <NxTriangleMeshDesc.h>

namespace Demo {
namespace System {
//------------------------------------------------------------
class PhysXCooker
{
public:
	//--------------------------------//
	class traits
	{
	public:
		typedef Demo::map<string_traits::str, NxMaterialIndex>::type material_binding_cont;
		typedef Demo::vector< shared_ptr_traits<const NxConvexMeshDesc>::type >::type convex_mesh_desc_cont;
		typedef Demo::vector< shared_ptr_traits<const NxTriangleMeshDesc>::type >::type triangle_mesh_desc_cont;
		typedef Demo::vector< shared_ptr_traits<NxStream>::type >::type stream_cont;
	};
	//--------------------------------//
	static bool CookConvexMesh( const NxConvexMeshDesc&, NxStream& );
	static bool CookTriangleMesh( const NxTriangleMeshDesc&, NxStream& );
	static bool CookConvexMesh( traits::convex_mesh_desc_cont &mcont, traits::stream_cont &scont );
	static bool CookTriangleMesh( traits::triangle_mesh_desc_cont &mcont, traits::stream_cont &scont );

	/*
	cookOgreMesh
	cooks an PhysX mesh from an Ogre mesh.
	*/
	static void cookOgreMesh(
		Ogre::MeshPtr mesh,
		NxStream& outputStream,
		const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE,
		traits::material_binding_cont &materialBindings = traits::material_binding_cont()
		);
};
//------------------------------------------------------------
}
}
#endif
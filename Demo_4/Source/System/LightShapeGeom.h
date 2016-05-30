#pragma once
#include <OgreVertexIndexData.h>

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	class LightShapeGeom
	{
	public:
		//--------------------------------//
		// Create a sphere Mesh with a given name, radius, number of rings and number of segments
		static void createSphere( string_traits::in name
								, float radius
								, int rings, int segments
								, bool hasNormals
								, bool hasTexCoords );

		// Fill up a fresh copy of VertexData and IndexData with a sphere's coords given the number of rings and the number of segments
		static void createSphere( Ogre::VertexData*& vertexData, Ogre::IndexData*& indexData
								, float radius
								, int rings, int segments
								, bool hasNormals
								, bool hasTexCoords );

		// Create a cone Mesh with a given name, radius and number of vertices in base
		// Created cone will have its head at 0,0,0, and will 'expand to' positive y
		static void createCone( string_traits::in name
								, float radius
								, float height
								, int segments );

		// Fill up a fresh copy of VertexData and IndexData with a cone's coords given the radius and number of vertices in base
		static void createCone( Ogre::VertexData*& vertexData, Ogre::IndexData*& indexData
								, float radius
								, float height
								, int segments );

		// Fill up a fresh copy of VertexData with a normalized quad
		static void createQuad( Ogre::VertexData*& vertexData );
	};
	//------------------------------------------------------------
}
}
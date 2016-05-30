#pragma once
#include <OgreMaterialManager.h>
#include "System\GBufferMaterialGenerator.h"
#include "string_traits.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	/** Class for handling materials who did not specify techniques for rendering
	 *  themselves into the GBuffer. This class allows deferred shading to be used,
	 *  without having to specify new techniques for all the objects in the scene.
	 *  @note This does not support all the possible rendering techniques out there.
	 *  in order to support more, either expand this class or specify the techniques
	 *  in the materials.
	 */
	class GBufferSchemeHandler : public Ogre::MaterialManager::Listener
	{
	public:
		//--------------------------------//
		typedef string_traits< Ogre::String >::str String;
		typedef string_traits< Ogre::String >::inpar string_traits::in;
		//--------------------------------//
		//The string that will be checked in textures to determine whether they are normal maps
		static const String NORMAL_MAP_PATTERN;
		//--------------------------------//
		/** @copydoc MaterialManager::Listener::handleSchemeNotFound */
		virtual Ogre::Technique* handleSchemeNotFound(
			unsigned short schemeIndex, 
			string_traits::in schemeName,
			Ogre::Material* originalMaterial,
			unsigned short lodIndex, 
			const Ogre::Renderable* rend );

	protected:
		//--------------------------------//
		//A structure for containing the properties of a material, relevant to GBuffer rendering
		//You might need to expand this class to support more options
		struct PassProperties 
		{
			PassProperties()
				: isDeferred(true)
				, normalMap(0)
				, isSkinned(false)
				, hasDiffuseColour(false)
			{
			}

			bool isDeferred;
			Ogre::vector< Ogre::TextureUnitState* >::type regularTextures;
			Ogre::TextureUnitState* normalMap;
			bool isSkinned;
			bool hasDiffuseColour;
			
			//Example of possible extension : vertex colours
			//Ogre::TrackVertexColourType vertexColourType;
		};
		//--------------------------------//
		//The material generator
		GBufferMaterialGenerator mMaterialGenerator;

		//Inspect a technique and return its relevant properties
		PassProperties inspectPass( Ogre::Pass* pass, unsigned short lodIndex, const Ogre::Renderable* rend );

		//Get the permutation of material flags that fit a certain property sheet
		MaterialGenerator::perm_type getPermutation( const PassProperties& props );

		//Fill a pass with the specific data from the pass it is based on
		void fillPass( Ogre::Pass* gBufferPass, Ogre::Pass* originalPass, const PassProperties& props );

		//Check if a texture is a normal map, and fill property sheet accordingly
		bool checkNormalMap( Ogre::TextureUnitState* tus, PassProperties& props );
	};
	//------------------------------------------------------------
}
}

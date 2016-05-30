#include "DemoStableHeaders.h"

#include <OgrePass.h>
#include <OgreTechnique.h>
#include <OgreHighLevelGpuProgram.h>
#include <OgreStringConverter.h>
#include <OgreLogManager.h>
#include "MaterialGenerator.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	Ogre::uint32 MaterialGenerator::msMatNameCounter = 0;
	//------------------------------------------------------------
	MaterialGenerator::MaterialGenerator()
		: vsMask(0)
		, fsMask(0)
		, matMask(0)
	{
	}
	//------------------------------------------------------------
	MaterialGenerator::~MaterialGenerator()
	{
#if DBS_LOGGING_ENABLED
		if(mpLog) Ogre::LogManager::getSingleton().destroyLog(mpLog);
#endif
	}
	//------------------------------------------------------------
	const Ogre::MaterialPtr & MaterialGenerator::getMaterial( perm_type permutation )
	{
		/// Check if material/shader permutation already was generated
		traits::material_container::iterator i = mMaterials.find(permutation);
		if( i != mMaterials.end() )
		{
			return i->second;
		}
		else
		{
			/// Create it
			Ogre::MaterialPtr tempMat = getTemplateMaterial(permutation);
			Ogre::GpuProgramPtr vp = getVertexProgram(permutation);
			Ogre::GpuProgramPtr fp = getFragmentProgram(permutation);
			
			/// Create material name
			string_traits::str name = tempMat->getName() + Ogre::StringConverter::toString( permutation );

			/// Create material from template, and set shaders
			Ogre::MaterialPtr mat = tempMat->clone( name );
			Ogre::Technique *tech = mat->getTechnique(0);
			Ogre::Pass *pass = tech->getPass(0);
			pass->setFragmentProgram( fp->getName() );
			pass->setVertexProgram( vp->getName() );
		
			/// And store it
			mMaterials[permutation] = mat;
			return mMaterials[permutation];
		}
	}
	//------------------------------------------------------------
	const Ogre::GpuProgramPtr & MaterialGenerator::getVertexProgram(perm_type permutation)
	{
		traits::program_container::iterator i = mVPs.find(permutation);
		if( i != mVPs.end() )
		{
			return i->second;
		}
		else
		{
			/// Create it
			Ogre::GpuProgramPtr progPtr = generateVertexProgram( permutation );
			mVPs[ permutation ] = progPtr;
			return mVPs[ permutation ];
		}
	}
	//------------------------------------------------------------
	const Ogre::GpuProgramPtr & MaterialGenerator::getFragmentProgram(perm_type permutation)
	{
		traits::program_container::iterator i = mFPs.find(permutation);
		if(i != mFPs.end())
		{
			return i->second;
		}
		else
		{
			/// Create it
			Ogre::GpuProgramPtr progPtr = generateFragmentProgram( permutation );
			mFPs[ permutation ] = progPtr;
			return mFPs[ permutation ];
		}
	}
	//------------------------------------------------------------
	const Ogre::MaterialPtr & MaterialGenerator::getTemplateMaterial(perm_type permutation)
	{
		traits::material_container::iterator i = mTemplateMats.find(permutation);
		if( i != mTemplateMats.end() )
		{
			return i->second;
		}
		else
		{
			/// Create it
			Ogre::MaterialPtr matPtr = generateTemplateMaterial( permutation );
			mTemplateMats[ permutation ] = matPtr;
			return mTemplateMats[ permutation ];
		}
	}
	//------------------------------------------------------------
	Ogre::MaterialPtr MaterialGenerator::getMaterialClone( perm_type permutation )
	{
		const Ogre::MaterialPtr & mat = getMaterial(permutation);
		return mat->clone(generateMatName(mat->getName()));
	}
	//------------------------------------------------------------
}
}
#pragma once
#include <OgreMaterial.h>

#if DBS_LOGGING_ENABLED
#include <OgreLog.h>
#endif

#include "Core/Tagged.h"

namespace Demo
{
namespace System
{
//------------------------------------------------------------
class MaterialGenerator : public Core::Tagged<MaterialGenerator>
{
public:
	DEFINE_CLASS_SHARED_PTR(MaterialGenerator)
	DEFINE_TAGGED
	//--------------------------------//
	typedef Ogre::uint32 perm_type;
	//--------------------------------//
	class traits
	{
	public:
		typedef Demo::map< perm_type, Ogre::GpuProgramPtr >::type program_container;
		typedef Demo::map< perm_type, Ogre::MaterialPtr >::type material_container;
	};
	//--------------------------------//
	struct InitData
	{
		virtual ~InitData(){}
		virtual bool isValid()const{ return !mgName.empty(); }

		string_traits::str mgName;
	};
	//--------------------------------//
	virtual ~MaterialGenerator();

	const Ogre::MaterialPtr & getMaterial( perm_type permutation );
	Ogre::MaterialPtr getMaterialClone( perm_type permutation );

	const Ogre::GpuProgramPtr & getVertexProgram( perm_type permutation );
	const Ogre::GpuProgramPtr & getFragmentProgram( perm_type permutation );
	const Ogre::MaterialPtr & getTemplateMaterial( perm_type permutation );

protected:
	virtual Ogre::GpuProgramPtr generateVertexProgram( perm_type permutation ) = 0;
	virtual Ogre::GpuProgramPtr generateFragmentProgram( perm_type permutation ) = 0;
	virtual Ogre::MaterialPtr generateTemplateMaterial( perm_type permutation ) = 0;

	string_traits::str generateMatName(string_traits::in prefix)
	{
		Ogre::StringStream ss;
		ss << prefix << "_" << msMatNameCounter++;
		return ss.str();
	}

	MaterialGenerator();

	/// Base name of materials generated by this
	string_traits::str mgName;

	/// Mask of permutation bits that influence vertex shader choice
	perm_type vsMask;

	/// Mask of permutation bits that influence fragment shader choice
	perm_type fsMask;

	/// Mask of permutation bits that influence template material choice
	perm_type matMask;

	traits::program_container mVPs;
	traits::program_container mFPs;
	traits::material_container mTemplateMats;
	traits::material_container mMaterials;

#if DBS_LOGGING_ENABLED
	Ogre::Log *mpLog;
#endif

private:
	MaterialGenerator( const MaterialGenerator& );
	MaterialGenerator& operator=( const MaterialGenerator& );

	static Ogre::uint32 msMatNameCounter;
};
//------------------------------------------------------------
}
}

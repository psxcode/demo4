#pragma once
#include "System\MaterialGenerator.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	class MaterialGeneratorManager
	{
		friend class OgreController;
	public:
		DEFINE_CLASS_SHARED_PTR(MaterialGeneratorManager)
		//--------------------------------//

		//Эта функция просматривает материал и модифицирует его
		//опираясь на специальные имена техник.
		//Возвращает true если материал был модифицирован.
		bool modifyMaterial( Ogre::Material& mat );

	protected:
		//--------------------------------//
		typedef Ogre::set< MaterialGenerator::shared_ptr >::type GeneratorSet;
		//--------------------------------//

		void initializeGenerators();

		GeneratorSet mGenerators;

	private:
		MaterialGeneratorManager();
		MaterialGeneratorManager( const MaterialGeneratorManager& );
		MaterialGeneratorManager& operator=( const MaterialGeneratorManager& );
	};
	//------------------------------------------------------------
}
}
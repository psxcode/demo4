#pragma once
#include <OgreRenderWindow.h>

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	class OgreRenderWindowInterface
	{
		friend class OgreController;
	public:
		static inline Ogre::RenderWindow* get(){ return _ptr; }
	private:
		static Ogre::RenderWindow* _ptr;
	};
	//------------------------------------------------------------
}
}
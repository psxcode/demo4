#pragma once
#include "Core\Controller.h"
#include <OgreCamera.h>

namespace Demo
{
	class BaseCameraController : public Core::Controller
	{
	public:
		virtual ~BaseCameraController()
		{
		}
		virtual void SetCamera( Ogre::Camera* pCam ) = 0;

	protected:
		BaseCameraController( Core::CONTROLLER_TYPE ct )
			: Core::Controller( ct )
			, _pCam(0)
		{
		}
		Ogre::Camera* _pCam;

	private:
		BaseCameraController( const BaseCameraController& );
		BaseCameraController& operator=( const BaseCameraController& );
	};
}
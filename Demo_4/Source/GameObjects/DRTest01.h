#pragma once
#include <OgreSceneManager.h>
#include <NxScene.h>
#include "Core/Controller.h"
#include "System/DeferredShadingSystem.h"
#include "SdkCameraMan.h"
#include "Interface/FactoryInterface.h"

namespace Demo
{
//------------------------------------------------------------
class DRTest01 : public Core::Controller
{
public:
	DEFINE_CLASS_SHARED_PTR(DRTest01)
	//--------------------------------//
	DRTest01();
	virtual ~DRTest01();
	virtual void CloseController();

	bool initialize();

protected:
	void createObjects();
	void createCam();
	void createDfrSystem();

	SdkCameraMan::shared_ptr mCam;
	System::DeferredShadingSystem::shared_ptr mDfr;
	Ogre::SceneManager *mpSceneManager;
	Ogre::Viewport *mpViewport;
};
//------------------------------------------------------------
}
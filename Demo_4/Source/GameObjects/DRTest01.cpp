#include "DemoStableHeaders.h"
#if DBS_LM_GAME_OBJECTS

#include <OgreRoot.h>
#include <NxMaterial.h>
#include "System/PhysXInterface.h"
#include "System/OgreInterface.h"
#include "Interface/FactoryInterface.h"
#include "Interface/OgreProcedural.h"
#include "DRTest01.h"

namespace Demo
{
//------------------------------------------------------------
DRTest01::DRTest01()
	: mpSceneManager(0)
	, mpViewport(0)
{
}
//------------------------------------------------------------
DRTest01::~DRTest01()
{
	if(isInitializedController())
		CloseController();
}
//------------------------------------------------------------
bool DRTest01::initialize()
{
	if(isInitializedController()) return false;

	mpSceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_EXTERIOR_CLOSE, "DRTestScene");

	createObjects();
	createCam();
	createDfrSystem();

	setInitializeController();
	return true;
}
//------------------------------------------------------------
void DRTest01::CloseController()
{
	Controller::CloseController();

	mCam->CloseController();

	if(mpViewport)
	{
		//viewport is deleted inside
		System::OgreInterface::getRenderWindow()->removeViewport(mpViewport->getZOrder());
		mpViewport = 0;
	}

	if(mpSceneManager)
	{
		Ogre::Root::getSingleton().destroySceneManager(mpSceneManager);
		mpSceneManager = 0;
	}
}
//------------------------------------------------------------
void DRTest01::createObjects()
{
	SceneFactoryInterface::shared_ptr sceneFactory = SceneFactoryInterface::getInstance(mpSceneManager);
	sceneFactory->createSceneOSNScene("test01", 0);

	Ogre::Light *sceneDirectLight = sceneFactory->createLight("direct");
	sceneDirectLight->setType(Ogre::Light::LT_DIRECTIONAL);
	sceneDirectLight->setPosition( 0, 1.0e3f, 1.5e3f );
	sceneDirectLight->setDirection( Ogre::Vector3::NEGATIVE_UNIT_Y );
	sceneDirectLight->setDiffuseColour(Ogre::ColourValue(0.5f, 1.0f, 0.5f));
	sceneDirectLight->setCastShadows(false);

	mpSceneManager->getRootSceneNode()->attachObject(sceneDirectLight);

	mpSceneManager->setAmbientLight(Ogre::ColourValue(0.1f, 0.1f, 0.1f));
}
//------------------------------------------------------------
void DRTest01::createCam()
{
	
	Ogre::Camera *cam = SceneFactoryInterface::getInstance(mpSceneManager)->createCamera( "DRCam" );
	mpViewport = System::OgreInterface::getRenderWindow()->addViewport( cam );
	mpViewport->setBackgroundColour( Ogre::ColourValue( 0.0f, 1.0f, 0.0f, 0.0f ) );
	cam->setAutoAspectRatio( true );
	cam->setNearClipDistance(0.1f);

	mCam.reset(new SdkCameraMan(mpSceneManager));
	mCam->initialize(cam);
	mCam->moveRelative(Ogre::Vector3(0,25,25));
	ConnectChildController(mCam.get());
}
//------------------------------------------------------------
void DRTest01::createDfrSystem()
{
	typedef System::DeferredShadingSystem DSS;

	if(mDfr) return;

	DSS::InitData idata(mpSceneManager, mpViewport);
	idata.gbMode = DSS::GBM_LAYOUT_HIGH | DSS::GBM_SIZE_HALF;

	mDfr = DSS::getInstance(idata);
	mDfr->registerOgreMaterialManagerListener();
}
//------------------------------------------------------------
}
#endif
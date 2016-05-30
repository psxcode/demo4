#include "DemoStableHeaders.h"
#if DBS_LM_GAME_OBJECTS

#include <limits>
#include <OgreControllerManager.h>
#include "Interface/FactoryInterface.h"
#include "Interface/UnitConverter.h"
#include "SdkCameraMan.h"

namespace Demo
{
//------------------------------------------------------------
SdkCameraMan::SdkCameraMan(Ogre::SceneManager *osm)
	: mOrbiting(false)
	, mZooming(false)
	, mGoingForward(false)
	, mGoingBack(false)
	, mGoingLeft(false)
	, mGoingRight(false)
	, mGoingUp(false)
	, mGoingDown(false)
	, mFastMove(false)
	, mVelocity( Ogre::Vector3::ZERO )
	, mTopSpeed( 150 )
	, mCamera(0)
	, mTarget(0)
	, mCamNode(0)
	, mDistToTarget(UnitConverter::DistanceToUnits(250))
	, mStyle( CS_NOT_DEFINED )
	, mpSceneManager(osm)
{
}
//------------------------------------------------------------
SdkCameraMan::~SdkCameraMan()
{
	CloseController();
}
//------------------------------------------------------------
InputRequest::container_t SdkCameraMan::getInputRequestContainer()
{
	InputRequest::container_t requestCont(ID_NUM_DELS);

	requestCont[ID_MOUSE_MOVED] = InputRequest(mInputDels[ID_MOUSE_MOVED], input_traits::IID_MOUSE_MOVED);
	requestCont[ID_FORWARD_PRESS] = InputRequest(mInputDels[ID_FORWARD_PRESS], input_traits::IID_MOVE_FORWARD, input_traits::IS_PRESSED);
	requestCont[ID_FORWARD_RELEASE] = InputRequest(mInputDels[ID_FORWARD_RELEASE], input_traits::IID_MOVE_FORWARD, input_traits::IS_RELEASED);
	requestCont[ID_BACK_PRESS] = InputRequest(mInputDels[ID_BACK_PRESS], input_traits::IID_MOVE_BACK, input_traits::IS_PRESSED);
	requestCont[ID_BACK_RELEASE] = InputRequest(mInputDels[ID_BACK_RELEASE], input_traits::IID_MOVE_BACK, input_traits::IS_RELEASED);
	requestCont[ID_LEFT_PRESS] = InputRequest(mInputDels[ID_LEFT_PRESS], input_traits::IID_MOVE_LEFT, input_traits::IS_PRESSED);
	requestCont[ID_LEFT_RELEASE] = InputRequest(mInputDels[ID_LEFT_RELEASE], input_traits::IID_MOVE_LEFT, input_traits::IS_RELEASED);
	requestCont[ID_RIGHT_PRESS] = InputRequest(mInputDels[ID_RIGHT_PRESS], input_traits::IID_MOVE_RIGHT, input_traits::IS_PRESSED);
	requestCont[ID_RIGHT_RELEASE] = InputRequest(mInputDels[ID_RIGHT_RELEASE], input_traits::IID_MOVE_RIGHT, input_traits::IS_RELEASED);

	return requestCont;
}
//------------------------------------------------------------
bool SdkCameraMan::initialize(Ogre::Camera *cam)
{
	if( isInitializedController() ) return false;

	mCamNode = SceneFactoryInterface::getInstance(mpSceneManager)->createSceneNode("camNode");
	setTarget(0);
	setStyle( CS_FREELOOK );
	setCamera(cam);
	mII = InputInterface::getInstance();
	if(mII)
	{
		for(int i = 0; i < ID_NUM_DELS; ++i)
			mInputDels[i].reset(new delegate_traits::del0<void>);

		mInputDels[ID_MOUSE_MOVED]->bind(this, &SdkCameraMan::inputMouseMoved);
		mInputDels[ID_FORWARD_PRESS]->bind(this, &SdkCameraMan::moveForwardPress);
		mInputDels[ID_FORWARD_RELEASE]->bind(this, &SdkCameraMan::moveForwardRelease);
		mInputDels[ID_BACK_PRESS]->bind(this, &SdkCameraMan::moveBackPress);
		mInputDels[ID_BACK_RELEASE]->bind(this, &SdkCameraMan::moveBackRelease);
		mInputDels[ID_LEFT_PRESS]->bind(this, &SdkCameraMan::moveLeftPress);
		mInputDels[ID_LEFT_RELEASE]->bind(this, &SdkCameraMan::moveLeftRelease);
		mInputDels[ID_RIGHT_PRESS]->bind(this, &SdkCameraMan::moveRightPress);
		mInputDels[ID_RIGHT_RELEASE]->bind(this, &SdkCameraMan::moveRightRelease);

		if( !mII->addInputDel(getInputRequestContainer()) )
			return false;
	}

	setInitializeController();
	return true;
}
//------------------------------------------------------------
void SdkCameraMan::CloseController()
{
	if(mII)
	{
		mII->removeInputDel(getInputRequestContainer());
	}
	mOrbiting = false;
	mZooming = false;
	mGoingForward = false;
	mGoingBack = false;
	mGoingLeft = false;
	mGoingRight = false;
	mGoingUp = false;
	mGoingDown = false;
	mFastMove = false;
	mVelocity = Ogre::Vector3::ZERO;
		
	setCamera( 0 );
	setTarget( 0 );
	SceneFactoryInterface::destroySceneNodeAndObjects(mCamNode);

	Core::Controller::CloseController();
}
//------------------------------------------------------------
void SdkCameraMan::OperateController()
{
	Ogre::Real frameTime = Ogre::ControllerManager::getSingleton().getFrameTimeSource()->getValue();
	if (mStyle == CS_FREELOOK)
	{
		// build our acceleration vector based on keyboard input composite
		Ogre::Vector3 accel = Ogre::Vector3::ZERO;
		if (mGoingForward) accel += getDirection();
		if (mGoingBack) accel -= getDirection();
		if (mGoingRight) accel += getRight();
		if (mGoingLeft) accel -= getRight();
		if (mGoingUp) accel += getUp();
		if (mGoingDown) accel -= getUp();

		// if accelerating, try to reach top speed in a certain time
		Ogre::Real topSpeed = mFastMove ? mTopSpeed * 20 : mTopSpeed;
		if (accel.squaredLength() != 0)
		{
			accel.normalise();
			mVelocity += accel * topSpeed * frameTime * 10;
		}
		// if not accelerating, try to stop in a certain time
		else mVelocity -= mVelocity * frameTime * 10;

		Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

		// keep camera velocity below top speed and above epsilon
		if (mVelocity.squaredLength() > topSpeed * topSpeed)
		{
			mVelocity.normalise();
			mVelocity *= topSpeed;
		}
		else if (mVelocity.squaredLength() < tooSmall * tooSmall)
			mVelocity = Ogre::Vector3::ZERO;

		if (mVelocity != Ogre::Vector3::ZERO) moveRelative(mVelocity * frameTime);
	}
}
//------------------------------------------------------------
void SdkCameraMan::setCamera(Ogre::Camera *cam)
{
	if( cam != 0 )
	{
		mCamNode->attachObject(cam);
		cam->setOrientation( Ogre::Quaternion::IDENTITY );
	}
	else
	{
		if(mCamera) mCamNode->detachObject(mCamera);
	}
	mCamera = cam;
}
//------------------------------------------------------------
void SdkCameraMan::setTarget(Ogre::SceneNode *target)
{
	/*
	if( mStyle == CS_ORBIT )
		{
			if( mTarget != target )
			{
				if( target != 0 )
				{
					mTarget = target;
					setYawPitchDist( Ogre::Degree(0), Ogre::Degree(-75), mDistToTarget );
					mCamNode->setAutoTracking( true, mTarget );
				}
				else
				{
					mTarget = OSMI::get()->getRootSceneNode();
					mCamNode->setAutoTracking( false );
				}
			}
		}*/
	
}
//------------------------------------------------------------
void SdkCameraMan::setStyle( CAMERA_STYLE style )
{
	if (mStyle != CS_ORBIT && style == CS_ORBIT)
	{
		setTarget(mTarget ? mTarget : mCamera->getSceneManager()->getRootSceneNode());
		mCamNode->setFixedYawAxis(true);
		manualStop();
		setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);
	}
	else if (mStyle != CS_FREELOOK && style == CS_FREELOOK)
	{
		mCamNode->setAutoTracking(false);
		mCamNode->setFixedYawAxis(true);
	}
	mStyle = style;
}
//------------------------------------------------------------
void SdkCameraMan::setYawPitchDist( Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist )
{
	mCamNode->setPosition( mTarget->_getDerivedPosition() );
	mCamNode->setOrientation( mTarget->_getDerivedOrientation() );
	mCamNode->yaw( yaw );
	mCamNode->pitch( pitch );
		
	moveRelative( Ogre::Vector3( 0,0,dist ) );
}
//------------------------------------------------------------
void SdkCameraMan::manualStop()
{
	if( mStyle == CS_FREELOOK )
	{
		mGoingForward = false;
		mGoingBack = false;
		mGoingLeft = false;
		mGoingRight = false;
		mGoingUp = false;
		mGoingDown = false;
		mVelocity = Ogre::Vector3::ZERO;
	}
}
//------------------------------------------------------------
void SdkCameraMan::inputMouseMoved()
{
	if (mStyle == CS_ORBIT)
	{
		Ogre::Real dist = (mCamNode->getPosition() - mTarget->_getDerivedPosition()).length();

		if (mOrbiting)   // yaw around the target, and pitch locally
		{
			mCamNode->setPosition(mTarget->_getDerivedPosition());

			mCamNode->yaw(Ogre::Degree(-mII->getMouseX().rel * 0.25f));
			mCamNode->pitch(Ogre::Degree(-mII->getMouseY().rel * 0.25f));

			moveRelative(Ogre::Vector3(0, 0, dist));

			// don't let the camera go over the top or around the bottom of the target
		}
		else if (mZooming)  // move the camera toward or away from the target
		{
			// the further the camera is, the faster it moves
			moveRelative(Ogre::Vector3(0, 0, mII->getMouseY().rel * 0.004f * dist));
		}
		else if (mII->getMouseZ().rel != 0)  // move the camera toward or away from the target
		{
			// the further the camera is, the faster it moves
			moveRelative(Ogre::Vector3(0, 0, -mII->getMouseZ().rel * 0.0008f * dist));
		}
	}
	else if (mStyle == CS_FREELOOK)
	{
		mCamNode->yaw(Ogre::Degree(-mII->getMouseX().rel * 0.15f), Ogre::Node::TS_WORLD);
		mCamNode->pitch(Ogre::Degree(-mII->getMouseY().rel * 0.15f));
	}
}
//------------------------------------------------------------
}
#endif
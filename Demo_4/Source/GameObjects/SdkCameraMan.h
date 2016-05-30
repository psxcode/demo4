#pragma once
#include <OgreSceneManager.h>
#include "Core/Controller.h"
#include "Interface/InputInterface.h"

namespace Demo
{
//------------------------------------------------------------
class SdkCameraMan : public Core::Controller
{
public:
	DEFINE_CLASS_SHARED_PTR(SdkCameraMan)
	//--------------------------------//
	enum CAMERA_STYLE
	{
		CS_NOT_DEFINED,
		CS_FREELOOK,
		CS_ORBIT
	};
	//--------------------------------//
	explicit SdkCameraMan(Ogre::SceneManager *osm);
	virtual ~SdkCameraMan();

	//Controller interface
	virtual void OperateController();
	virtual void CloseController();

	bool initialize(Ogre::Camera *cam);

	void setCamera( Ogre::Camera* cam );
	Ogre::Camera* getCamera()const{ return mCamera; }

	void setTarget( Ogre::SceneNode* target );
	Ogre::SceneNode* getTarget()const { return mTarget; }

	void setStyle( CAMERA_STYLE );
	CAMERA_STYLE getStyle()const{ return mStyle; }

	void setTopSpeed( Ogre::Real topSpeed ) {mTopSpeed = topSpeed;}
	Ogre::Real getTopSpeed()const { return mTopSpeed; }

	void moveRelative( const Ogre::Vector3& transVector ) {mCamNode->setPosition(mCamNode->getPosition() + transVector);}
	Ogre::Vector3 getDirection()const { return mCamNode->getOrientation() * -Ogre::Vector3::UNIT_Z; }
	Ogre::Vector3 getUp()const { return mCamNode->getOrientation() * Ogre::Vector3::UNIT_Y; }
	Ogre::Vector3 getRight()const { return mCamNode->getOrientation() * Ogre::Vector3::UNIT_X; }
	void setYawPitchDist( Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist );
	void manualStop();

protected:
	InputRequest::container_t getInputRequestContainer();
	void inputMouseMoved();
	void moveForwardPress() {mGoingForward = true;}
	void moveForwardRelease() {mGoingForward = false;}
	void moveBackPress() {mGoingBack = true;}
	void moveBackRelease() {mGoingBack = false;}
	void moveLeftPress() {mGoingLeft = true;}
	void moveLeftRelease() {mGoingLeft = false;}
	void moveRightPress() {mGoingRight = true;}
	void moveRightRelease() {mGoingRight = false;}
	//--------------------------------//
	enum InputDels
	{
		ID_MOUSE_MOVED,
		ID_FORWARD_PRESS,
		ID_FORWARD_RELEASE,
		ID_BACK_PRESS,
		ID_BACK_RELEASE,
		ID_LEFT_PRESS,
		ID_LEFT_RELEASE,
		ID_RIGHT_PRESS,
		ID_RIGHT_RELEASE,
		ID_NUM_DELS
	};
	//--------------------------------//
	delegate_traits::del0<void>::shared_ptr mInputDels[ID_NUM_DELS];

	InputInterface::shared_ptr mII;

	Ogre::SceneManager *mpSceneManager;
	Ogre::Camera* mCamera;
	Ogre::SceneNode* mTarget;
	Ogre::SceneNode* mCamNode;
	CAMERA_STYLE mStyle;
	Ogre::Real mTopSpeed;
	Ogre::Vector3 mVelocity;
	Ogre::Real mDistToTarget;

	bool mOrbiting;
	bool mZooming;
	bool mGoingForward;
	bool mGoingBack;
	bool mGoingLeft;
	bool mGoingRight;
	bool mGoingUp;
	bool mGoingDown;
	bool mFastMove;
};
//------------------------------------------------------------
}
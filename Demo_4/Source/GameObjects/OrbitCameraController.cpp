#include "DemoPrerequisites.h"
#if DBS_LM_GAME_OBJECTS

#include "OrbitCameraController.h"
#include "System\InputInterface.h"
#include "System\OgreSceneManagerInterface.h"
#include "System\FrameTimeInterface.h"

namespace Demo
{
	//------------------------------------------------------------
	typedef System::InputInterface II;
	typedef System::OgreSceneManagerInterface OSMI;
	typedef System::TimeInterface FTI;
	//------------------------------------------------------------
	OrbitCameraController::OrbitCameraController()
		: BaseCameraController( Core::CT_ORBIT_CAMERA_CONTROLLER )
		, _targetNode(0)
		, _camNode(0)
		, _moveDel(0)
		, _moveSpeed(0)
		, _rotSpeed(0)
		, _nearClipDistance(0)
		, _moveVec( Ogre::Vector3::ZERO )
		, _currPosition( Ogre::Vector3::ZERO )
	{
	}
	//------------------------------------------------------------
	OrbitCameraController::~OrbitCameraController()
	{
		CloseController();
	}
	//------------------------------------------------------------
	bool OrbitCameraController::Initialise( const OrbitCameraController::InitData& initData )
	{
		if( isInitializedController() ) return false;

		_moveDel.bind( new vdel( this, &OrbitCameraController::MouseMoved ) );
		if( !II::addInputDelivery( Core::IID_MOUSE_MOVED, II::bdel_elem(_moveDel) ) ){ CloseController(); return false; }
		_moveDel->SetActive( true );

		_currPosition = initData.position;
		_moveSpeed = initData.moveSpeed;
		_rotSpeed = initData.rotSpeed;
		_nearClipDistance = initData.nearClipDistance;

		_targetNode = OSMI::get()->getRootSceneNode()->createChildSceneNode();
		_camNode = _targetNode->createChildSceneNode();
		_camNode->setPosition( _currPosition );
	//	_camNode->setAutoTracking( true, _targetNode );
	//	_camNode->setInheritOrientation(false);

		setInitializeController();
		return true;
	};
	//------------------------------------------------------------
	void OrbitCameraController::CloseController()
	{
		BaseCameraController::CloseController();
		if( _camNode )
		{
			_camNode->getParentSceneNode()->removeChild( _camNode );
			OSMI::get()->destroySceneNode( _camNode );
			_camNode = 0;
		}
		if( _targetNode )
		{ 
			_targetNode->getParentSceneNode()->removeChild( _targetNode );
			OSMI::get()->destroySceneNode( _targetNode );
			_targetNode = 0;
		}
		if( _pCam ) 
			_pCam = 0;

		if( _moveDel != 0 ) 
		{
			II::removeInputDelivery( Core::IID_MOUSE_MOVED, II::bdel_elem(_moveDel) );
			_moveDel = 0;
		}

		_moveVec = Ogre::Vector3::ZERO;
		_currPosition = Ogre::Vector3::ZERO;
		_moveSpeed = 0;
		_rotSpeed = 0;
	}
	//------------------------------------------------------------
	void OrbitCameraController::SetCamera( Ogre::Camera* cam )
	{
		if( !mInitializedController ) return;

		if( cam && cam->isAttached() )
			cam->getParentSceneNode()->detachObject( cam );
		if( _pCam && _pCam->isAttached() )
			_pCam->getParentSceneNode()->detachObject( _pCam );

		_pCam = cam;
		if( _pCam )
		{
			_camNode->attachObject( _pCam );
		//	_pCam->lookAt(_targetNode->getPosition());
			_pCam->setAutoTracking( true, _targetNode );
		}
	}
	//------------------------------------------------------------
	bool OrbitCameraController::SetParentOSN( Ogre::SceneNode* parentOSN )
	{
		if( !mInitializedController ) return true;
		if( !parentOSN ) return false;
		_targetNode->getParentSceneNode()->removeChild( _targetNode );
		parentOSN->addChild( _targetNode ) ;
		return true;
	}
	//------------------------------------------------------------
	void OrbitCameraController::OperateController()
	{
		_moveVec *= _rotSpeed * FTI::frameTime();


		_targetNode->yaw( Ogre::Degree( _moveVec.x ) , Ogre::Node::TS_WORLD );
		_targetNode->pitch( Ogre::Degree( _moveVec.y ) ); //, Ogre::Node::TS_PARENT );
	//	_camNode->setOrientation( _targetNode->getOrientation());		
	//	_camNode->setPosition( _targetNode->getPosition() + _currPosition );


//		_currPosition = _camNode->getPosition();
//		_currPosition.z += _moveVec.z;
//		_camNode->setPosition( _currPosition );

		_moveVec = Ogre::Vector3::ZERO;
	}
	//------------------------------------------------------------
	void OrbitCameraController::MouseMoved()
	{
		_moveVec.x = Ogre::Real(II::getMouseX().rel);
		_moveVec.y = Ogre::Real(II::getMouseY().rel);
	}
	//------------------------------------------------------------
}
#endif
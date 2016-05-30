#pragma once
#include "GameObjects\BaseCameraController.h"
#include "Core\VoidDelegate.h"
#include <OgreSceneNode.h>
#include <OgreVector3.h>

namespace Demo
{
	class OrbitCameraController : public BaseCameraController
	{
	public:
		//--------------------------------//
		typedef void_delegate_traits< OrbitCameraController >::vdel vdel;
		typedef void_delegate_traits< OrbitCameraController >::vdel_shptr vdel_shptr;
		//--------------------------------//
		struct InitData
		{
			InitData()
				: moveSpeed(0)
				, rotSpeed(0)
				, nearClipDistance(0)
				, position( Ogre::Vector3::ZERO )
			{
			}

			Ogre::Vector3 position;
			float moveSpeed;
			float rotSpeed;
			int nearClipDistance;
		};
		//--------------------------------//
		OrbitCameraController();
		virtual ~OrbitCameraController();

		virtual void OperateController();
		virtual void CloseController();
		virtual bool ConnectChild( ctr_elem_addrem ){ return false; }
		virtual bool ConnectingToParent( ctr_elem_addrem ){ return false; }
	
		virtual void SetCamera( Ogre::Camera* pCam );

		bool Initialise( const OrbitCameraController::InitData& );
		bool SetParentOSN( Ogre::SceneNode* );

	protected:
		void MouseMoved();

		Ogre::SceneNode* _targetNode;
		Ogre::SceneNode* _camNode;
		Ogre::Vector3 _moveVec;
		Ogre::Vector3 _currPosition;
		int _nearClipDistance;
		float _moveSpeed;
		float _rotSpeed;
		vdel_shptr _moveDel;
	};
}
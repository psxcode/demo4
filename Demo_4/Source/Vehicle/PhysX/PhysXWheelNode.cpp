#include "DemoStableHeaders.h"
#if DBS_LM_PHYSX_VEHICLE

#include <NxMaterialDesc.h>
#include "PhysXWheelNode.h"
#include "Interface\OgrePhysxConverter.h"
#include "Interface\FrameTimeInterface.h"
#include "Vehicle\Abstract\VehicleTypes.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	typedef OgrePhysxConverter OPC;
	typedef UnitConverter UI;
	typedef TimeInterface FTI;
	typedef VehicleOSNNames VN;
	//------------------------------------------------------------
	PhysXWheelNode::~PhysXWheelNode()
	{
		 ClosePhysXWheelNode();
	}
	//------------------------------------------------------------
	bool PhysXWheelNode::Initialize( const PhysXWheelNodeInitData& initdata )
	{
		if( this->initializedMechanics ) return false;

		//Если нет SceneNode узла - выходим
		if( initdata.wheelNodeOSN == 0 )return false;

		//Копируем параметры полностью, так как надо будет модифицировать их немного
		PhysXWheelNodeParams::shared_ptr PWNparams( initdata.physxWheelNodeParams->CreatePhysXWheelNodeCopyPtr() );
		if ( PWNparams.isNull() || !PWNparams->isValid() )return false;

		//Проводим преобразование величин дистанций
		PWNparams->DistanceToUnits();

		if( PWNparams->ironWheelParams != 0 )ironWheel.bind( new PhysXWheelParams( *PWNparams->ironWheelParams ) );
		if( PWNparams->rubberWheelParams != 0 )rubberWheel.bind( new PhysXWheelParams( *PWNparams->rubberWheelParams ) );
		susDesc.bind( new PhysXSuspensionDesc( *PWNparams->susDesc ) );

		//Копируем смещения
		this->rubberWheelIncShift = PWNparams->rubberWheelIncShift;
		this->ironWheelIncShift = PWNparams->ironWheelIncShift;

		//mechanics
		if( !PWNparams->dragLookup.isNull() )dragLookup = PWNparams->dragLookup->CreateCopyPtr();
		nameMechanics = PWNparams->nameMechanics;
		maxDrag = PWNparams->maxDrag;
		//mechanics

		wheelNodeOSN = initdata.wheelNodeOSN;
		if( !ironWheel.isNull() )
		{
			ironWheelOSN = SPC::CreateSceneNode( *(ironWheel->wheelOSN), static_cast< void* >( this ) );
			if( ironWheelOSN == 0 ){ ClosePhysXWheelNode(); return false; }
			Ogre::Vector3 pos = this->ironWheelOSN->getPosition();

			pos.x += ironWheelIncShift;
			/*
			if( initdata.side == VS_LEFT ){ pos.x -= ironWheelIncShift;	}
			else if( initdata.side == VS_RIGHT ){ pos.x += ironWheelIncShift; }
			else{ ClosePhysXWheelNode(); return false; }
			*/

			this->ironWheelOSN->setPosition( pos );
			this->wheelNodeOSN->addChild( ironWheelOSN );

			if( !PWNparams->brakeBlockParams.isNull() )
			{
				BrakeBlock::BrakeBlockInitData bbIData;
				bbIData.blockParams = PWNparams->brakeBlockParams;
				brakeBlock.bind( new BrakeBlock() );
				if( !brakeBlock->Initialize( bbIData ) ){ ClosePhysXWheelNode(); return false; }
				if( brakeBlock->getOSN() != 0 )
				{
					pos = brakeBlock->getOSN()->getPosition();

					pos.x += ironWheelIncShift;

					/*
					if( initdata.side == VS_LEFT ){	pos.x -= ironWheelIncShift;	}
					else if( initdata.side == VS_RIGHT ){ pos.x += ironWheelIncShift; }
					else{ ClosePhysXWheelNode(); return false; }
					*/

					brakeBlock->getOSN()->setPosition( pos );
					if( !ConnectChildOsnc( brakeBlock.get() ) ){ ClosePhysXWheelNode(); return false; }
				}
			}
			if( !rubberWheel.isNull() )
			{
				rubberWheelOSN = SPC::CreateSceneNode( *(this->rubberWheel->wheelOSN), static_cast< void* >( this ) );
				if( rubberWheelOSN == 0 ){ ClosePhysXWheelNode(); return false; }
				pos = rubberWheelOSN->getPosition();

				pos.x += rubberWheelIncShift;

				/*
				if( initdata.side == VS_LEFT ){	pos.x -= rubberWheelIncShift; }
				else if( initdata.side == VS_RIGHT ){ pos.x += rubberWheelIncShift;	}
				else{ ClosePhysXWheelNode(); return false; }
				*/

				this->rubberWheelOSN->setPosition( pos );
				this->ironWheelOSN->addChild( rubberWheelOSN );
			}

			//wheelshapedesc
			PhysXWheelParams::shared_ptr wheelParams;
			if( !rubberWheel.isNull() )
				wheelParams = rubberWheel;
			else
				wheelParams = ironWheel;

			wheelShapeDesc.bind( new NxWheelShapeDesc() );
			wheelShapeDesc->mass = wheelParams->mass;
			wheelShapeDesc->inverseWheelMass = 1 / wheelParams->mass;
			wheelShapeDesc->radius = wheelParams->radius + wheelParams->tireIncRadius;
			wheelShapeDesc->lateralTireForceFunction = wheelParams->tireDesc->latFunction;
			wheelShapeDesc->longitudalTireForceFunction = wheelParams->tireDesc->longFunction;

			//suspension
			wheelShapeDesc->suspension.damper = susDesc->suspension.damper;
			wheelShapeDesc->suspension.spring = susDesc->suspension.spring;
			wheelShapeDesc->suspension.targetValue = susDesc->suspension.targetValue;
			wheelShapeDesc->suspensionTravel = susDesc->suspensionTravel;

			dragCalcDel.bind( this, brakeBlock != 0 ? &PhysXWheelNode::calcDragWithBrake : &PhysXWheelNode::calcDragNoBrake );
		}

		return initializedMechanics = true;
	}
	//------------------------------------------------------------
	void PhysXWheelNode::ClosePhysXWheelNode()
	{
		ironWheel.setNull();
		rubberWheel.setNull();
		if( wheelShape != 0 )wheelShape = 0;
		wheelShapeDesc.setNull();
		susDesc.setNull();
		dragCalcDel.clear();
		CloseWheelNode();
	}
	//------------------------------------------------------------
	PhysXWheelNodeParams::shared_ptr PhysXWheelNode::CreatePhysXWheelNodeParams() const
	{
		if ( !initializedMechanics ) return PhysXWheelNodeParams::shared_ptr();
		PhysXWheelNodeParams::shared_ptr params( new PhysXWheelNodeParams() );

		if( !ironWheel.isNull() ) params->ironWheelParams.bind( new PhysXWheelParams( *ironWheel ) );
		if( !rubberWheel.isNull() ) params->rubberWheelParams.bind( new PhysXWheelParams( *rubberWheel ) );
		if( !dragLookup.isNull() ) params->dragLookup = this->dragLookup->CreateCopyPtr();
		params->nameMechanics = nameMechanics;
		params->maxDrag = maxDrag;
		params->susDesc.bind( new PhysXSuspensionDesc( *susDesc ) );
		return params;
	}
	//------------------------------------------------------------
	float PhysXWheelNode::OperateMechanics( float t, float d )
	{
		NxWheelContactData wcd;
		NxShape* contactShape = wheelShape->getContact(wcd);
		Ogre::Vector3 physXPos = OPC::toOgre( wheelShape->getLocalPosition());
		Ogre::Vector3 ogrePos = wheelNodeOSN->getPosition();
		if( contactShape ) ogrePos.y = physXPos.y - wcd.contactPosition + wheelShapeDesc->radius;
		else ogrePos.y = physXPos.y - wheelShapeDesc->suspensionTravel;
		wheelNodeOSN->setPosition( ogrePos );

		wheelNodeOSN->setOrientation( Ogre::Quaternion::IDENTITY );
		wheelNodeOSN->yaw( Ogre::Radian( wheelShape->getSteerAngle() ), Ogre::Node::TS_LOCAL );
		ironWheelOSN->pitch( Ogre::Radian( -wheelShape->getAxleSpeed() * FTI::frameTime() ), Ogre::Node::TS_LOCAL );
		
		torque = t;
		drag = d;

		//Расчитать тормозное усилие. При этом модифицируется drag
		dragCalcDel();

		wheelShape->setMotorTorque( torque * OPC::torqueMechToPhysXMult );
		wheelShape->setBrakeTorque( drag * OPC::torqueMechToPhysXMult );

		return radPS = wheelShape->getAxleSpeed();
	}
	//------------------------------------------------------------
	bool PhysXWheelNode::isMechanicsChainValid() const
	{
		return isInitializedMechanics();
	}
	//------------------------------------------------------------
	void PhysXWheelNode::calcDragNoBrake()
	{
	}
	//------------------------------------------------------------
	void PhysXWheelNode::calcDragWithBrake()
	{
		drag += brakeBlock->getActiveBrakeTorque();
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_PHYSX_VEHICLE
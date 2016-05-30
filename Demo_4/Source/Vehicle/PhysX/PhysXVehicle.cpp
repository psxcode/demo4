#include "DemoStableHeaders.h"
#if DBS_LM_PHYSX_VEHICLE

#include <NxActor.h>
#include <NxConvexShapeDesc.h>
#include <NxBoxShapeDesc.h>
#include <NxMaterial.h>
#include <OgreStringConverter.h>
#include "PhysXVehicle.h"
#include "Vehicle\Abstract\MechGearbox.h"
#include "Vehicle\Abstract\AutoGearbox.h"
#include "Vehicle\Abstract\BrakeControl.h"
#include "Interface\OgrePhysxConverter.h"
#include "System\PhysXSceneInterface.h"
#include "System\PhysXController.h"
#include "Vehicle\Abstract\IGearboxControl.h"
#include "Vehicle\Abstract\IGearboxState.h"
#include "Vehicle\Abstract\IGearboxCallback.h"

namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	typedef OgrePhysxConverter OPC;
	typedef System::PhysXSceneInterface PSI;
	typedef VehicleOSNNames VN;
	//------------------------------------------------------------
	PhysXVehicle::shared_ptr PhysXVehicle::CreatePhysXVehicle( string_traits::in name )
	{
		PhysXVehicleBodyParams::shared_ptr bodyParams = VPC::GetSingleton().CreateVehicleBodyParams( name );
		if( bodyParams == 0 )return PhysXVehicle::shared_ptr();

		PhysXVehicle::PhysXVehicleInitData idata;
		idata.bodyParams = bodyParams;

		PhysXVehicle::shared_ptr vehicle( new PhysXVehicle() );
		if( !vehicle->Initialize( idata ) )return PhysXVehicle::shared_ptr();

		return vehicle;
	}
	//------------------------------------------------------------
	PhysXVehicle::PhysXVehicle()
		: solverCount(0)
		, addDownForce(0)
		, physxActor(0)
		, physxBodyMaterial(0)
	{
	}
	//------------------------------------------------------------
	bool PhysXVehicle::Initialize( const PhysXVehicle::PhysXVehicleInitData &data )
	{
		//Проверка
		if( this->mInitializedController || data.bodyParams.isNull() || !data.bodyParams->isValid() )return false;

		//Копируем указатель для удобства
		PhysXVehicleBodyParams::const_shared_ptr body = data.bodyParams;

		//делегат для OperateController
		operateDel.bind( this, &PhysXVehicle::OperateNoPhysx );

		//Создаём SceneNode для корпуса
		osn = SPC::CreateSceneNode( *body->bodyOSNParams, static_cast< void* >( this ) );
		if ( osn == 0 ){ ClosePhysXVehicle(); return false; }
		
		//копируем основные параметры
		this->vehicleBodyName = body->bodyName;
		this->massOffset = body->massOffset;
		this->boundBox = body->boundBox;
		this->solverCount = body->solverCount;
		this->boundMeshName = body->boundMeshName;
		this->physxMatName = body->physxMatName;
		this->mass = body->mass;
		this->addDownForce = body->addDownForce;
		this->vehicleSetName = body->setName;

		//Теперь колёса
		string_traits::str wheelNodeName;

		//Итераторы для перебора осей из PhysXVehicleBodyParams
		PhysXVehicleBodyParams::traits::axle_container::const_iterator iter = body->axles.begin();
		PhysXVehicleBodyParams::traits::axle_container::const_iterator end_iter = body->axles.end();

		//Структура для инициализации BrakeControl
		BrakeControl::BrakeControlInitData bcidata;
		//Структура для инициализации SteerControl
		SteerControl::SteerControlInitData scidata;

		//Перебираем оси из PhysXVehicleBodyParams
		for( ; iter != end_iter; ++iter )
		{
			//Создаём ось
			PhysXVehicleAxle::shared_ptr axle( new PhysXVehicleAxle() );
			this->axles.push_back( axle );

			//Копируем основные параметры
			axle->index = (*iter)->index;
			axle->position = (*iter)->position;
			axle->maxSteerAngle = (*iter)->maxSteerAngle;
			axle->brakeMaxPressValue = (*iter)->brakeMaxPressValue;

			//Работаем с WheelNodeOSN для узлов
			Ogre::SceneNode* leftWheelNodeOSN(0);
			Ogre::SceneNode* rightWheelNodeOSN(0);
			{
				//SceneNodeParams для узла колеса
				Core::SceneNodeParams snp;

				//Левый узел
				{
					//Имя узла проходит через модификацию
					wheelNodeName = VN::ComposeWheelNodeName( axle->index, VN::LEFT );
					snp.name = wheelNodeName;
					//Ставим позицию
					snp.position = axle->position;
					//Позицию по X инвертируем, так как это левый SceneNode
					snp.position.x = -axle->position.x;

					//Создаём SceneNode
					leftWheelNodeOSN = SPC::CreateSceneNode( snp, static_cast< void* >( this ) );

					//Так как SceneNode левый - ставим Scale в отрицательный по оси X
					//Все прикреплённые объекты унаследуют масштаб
					leftWheelNodeOSN->setScale( Ogre::Vector3(-1,1,1));
					//Добавляем SceneNode к корпусу
					osn->addChild( leftWheelNodeOSN );
				}

				//Тоже самое для правого узла
				{
					wheelNodeName = VN::ComposeWheelNodeName( axle->index, VN::RIGHT );
					snp.name = wheelNodeName;
					snp.position = axle->position;
					rightWheelNodeOSN = SPC::CreateSceneNode( snp, static_cast< void* >( this ) );
					osn->addChild( rightWheelNodeOSN );
				}
			}
			//закончили с WheelNodeOSN для этой оси

			//Теперь логика узлов
			{
				//Структура инициализации
				PhysXWheelNode::PhysXWheelNodeInitData wnData;

				//Левый узел
				{
					//основные параметры
					wnData.physxWheelNodeParams = (*iter)->leftWheelNodeParams;
					//сторона
					wnData.side = VS_LEFT;
					//Графический узел
					wnData.wheelNodeOSN = leftWheelNodeOSN;
					//Создаём
					axle->leftWheelNode.bind( new PhysXWheelNode() );
					//Инициализация
					if( !axle->leftWheelNode->Initialize( wnData ) ){ ClosePhysXVehicle(); return false; }
				}

				//Правый узел
				{
					wnData.physxWheelNodeParams = (*iter)->rightWheelNodeParams;
					wnData.side = VS_RIGHT;
					wnData.wheelNodeOSN = rightWheelNodeOSN;
					axle->rightWheelNode.bind( new PhysXWheelNode() );
					if( !axle->rightWheelNode->Initialize( wnData ) ){ ClosePhysXVehicle(); return false; }
				}
			}

			//Ось тормозной системы
			BrakeControl::Axle baxle;
			//Тормозные блоки
			baxle.leftBlock = axle->leftWheelNode->brakeBlock;
			baxle.rightBlock = axle->rightWheelNode->brakeBlock;
			//Макс давление на оси
			baxle.maxPressValue = axle->brakeMaxPressValue;
			//Кладём в InitData
			bcidata.brakeAxles.push_back( baxle );

			//Если ось поворотная - кладём в InitData для SteerControl
			if( axle->maxSteerAngle > 0 )scidata.steerAxle = axle;
		}
		//Закончили с осями

		//brakeControl
		this->brakeControl.bind( new BrakeControl() );
		if( !this->brakeControl->Initialize( bcidata ) ){ ClosePhysXVehicle(); return false; }
		//brakeControl

		//steerControl
		if( !scidata.isValid() ){ ClosePhysXVehicle(); return false; }
		this->steerControl.bind( new SteerControl() );
		if( !this->steerControl->Initialize( scidata ) ){ ClosePhysXVehicle(); return false; }
		//steerControl

		//differential chain
		this->differential = InitializeDifferential( body->diff );
		if( differential == 0 ){ ClosePhysXVehicle(); return false; }
		//differential chain

		//gearbox
		if( !InitializeGearbox( body->gearbox ) ){ ClosePhysXVehicle(); return false; }
		//gearbox

		//motor
		if( !InitializeMotor( body->motor ) ){ ClosePhysXVehicle(); return false; }
		//motor

		//fill non driven axles
		{
			AIter iter = axles.begin();
			AIter end_iter = axles.end();
			for( ; iter != end_iter; ++iter )
			{
				if( !(*iter)->isDriven() )nonDrivenAxles.push_back( (*iter) );
			}
		}

		//throttleControl
		switch( body->tcs->tcsType )
		{
		case TCS_CFEED:
			{
				ConstFeedThrottleControl::InitData tcsIData;
				tcsIData.motorControl = motor->GetControlInterface();
				tcsIData.tcParams = body->tcs;
				ConstFeedThrottleControl::shared_ptr tcs( new ConstFeedThrottleControl() );
				if( !tcs->Initialize( tcsIData ) ){ ClosePhysXVehicle(); return false; }
				throttleControl = tcs;
			}
			break;
		default:
			ClosePhysXVehicle();
			return false;
		};
		//throttleControl

		//speedToGear
		{
			SpeedToGearCalculator::InitData speedidata;
			speedidata.gearbox.downcast_assign( gearbox->GetInterface( GIT_STATE ) );
			speedToGear.bind( new SpeedToGearCalculator() );
			if( !speedToGear->Initialize( speedidata ) ){ ClosePhysXVehicle(); return false; }
		}
		//speedToGear

		//speedometer
		{
			Speedometer::InitData idata;
			idata.wheelRadius = body->speedometer.wheelRadius;

			if( body->speedometer.revSensorAxis < 0 || body->speedometer.revSensorAxis >= (int)axles.size() ){ ClosePhysXVehicle(); return false; }
			IMechanicsState::shared_ptr revSensor;
			switch( body->speedometer.revSensorSide )
			{
			case 0:
				revSensor = axles.at( body->speedometer.revSensorAxis )->GetLeftWheelNode()->GetMechanicsStateInterface();
				break;
			case 1:
				revSensor = axles.at( body->speedometer.revSensorAxis )->GetRightWheelNode()->GetMechanicsStateInterface();
				break;
			default:
				revSensor = axles.at( body->speedometer.revSensorAxis )->GetLeftWheelNode()->GetMechanicsStateInterface();
			}
			
			idata.revSensor = revSensor;

			if( !speedometer.Initialize( idata ) ){ ClosePhysXVehicle(); return false; }
		}
		//speedometer

		//vehicleToDriver
		vehicleToDriver.bind( new VehicleToDriverData() );
		vehicleToDriver->controlAxes |= VCA_BRAKE;
		vehicleToDriver->controlAxes |= VCA_STEER;
		vehicleToDriver->controlAxes |= VCA_THROTTLE;
		vehicleToDriver->brake.downcast_assign( getBrakeInterface( PIT_PCONTROL ) );
		vehicleToDriver->throttle.downcast_assign( getThrottleInterface( PIT_PCONTROL ) );
		vehicleToDriver->steer.downcast_assign( getSteerInterface( PIT_SCONTROL ) );
		vehicleToDriver->motorState = motor->GetStateInterface();
		vehicleToDriver->motorControl = motor->GetControlInterface();
		vehicleToDriver->motorCallback = motor->GetCallbackInterface();
		vehicleToDriver->speedToGear = speedToGear;
		vehicleToDriver->speedometer = GetSpeedometerStateInterface();
		switch( gearbox->GetGearboxType() )
		{
		case GT_MECHANICAL:
			vehicleToDriver->controlAxes |= VCA_CLUTCH;
			vehicleToDriver->clutch.downcast_assign( gearbox->GetClutchInterface( PIT_PCONTROL ) );
			vehicleToDriver->gearboxControl.downcast_assign( gearbox->GetInterface( GIT_CONTROL ) );
			break;
		case GT_AUTOMATIC:
			vehicleToDriver->gearboxControl.downcast_assign( gearbox->GetInterface( GIT_CONTROL ) );
			break;
		}
		//vehicleToDriver

		return mInitializedController = true;
	}
	//------------------------------------------------------------
	void PhysXVehicle::ClosePhysXVehicle()
	{
		StopPhysX();
		operateDel = 0;
		throttleControl = 0;
		brakeControl = 0;
		steerControl = 0;

		if( motor != 0 )motor = 0;
		if( gearbox != 0 )gearbox = 0;
		if( differential != 0 )differential = 0;

		nonDrivenAxles.clear();
		axles.clear();

		if( osn != 0 )SPC::DestroySceneNodeAndObjects( osn );

		if( physxBodyMaterial != 0 )
			PSI::get()->releaseMaterial( *physxBodyMaterial );

		Core::Controller::CloseController(); //mInitializedController = false
		Core::OSNController::CloseOSN();
	}
	//------------------------------------------------------------
	Differential::shared_ptr PhysXVehicle::InitializeDifferential( DifferentialParams::const_shared_ptr diffParams )
	{
		//Проверка готовности колёс
		if( axles.empty() )return Differential::shared_ptr();

		Differential::shared_ptr newDiff;
		//Если диффер простой - создаём простой
		if( diffParams->availableModes == 0 )
			newDiff.bind( new Differential() );

		//Если ничего не создано
		if( newDiff == 0 )return Differential::shared_ptr();

		//Инициализация
		Differential::DifferentialInitData diData;
		diData.diffParams = diffParams;
		if( !newDiff->Initialize( diData ) )return Differential::shared_ptr();

		//Если присутствуют прицепленные params
		if( diffParams->leftDiffParams != 0 || diffParams->rightDiffParams != 0 )
		{
			//Должны присутствовать сразу оба params'а
			if( !(diffParams->leftDiffParams != 0 && diffParams->rightDiffParams != 0) )return Differential::shared_ptr();

			//Инициализация левого диффера
			Differential::shared_ptr leftDiff = InitializeDifferential( diffParams->leftDiffParams );
			if( leftDiff == 0 )return Differential::shared_ptr();
			if( !newDiff->ConnectChildMech( leftDiff, 0 ) )return Differential::shared_ptr();

			//Инициализация правого диффера
			Differential::shared_ptr rightDiff = InitializeDifferential( diffParams->rightDiffParams );
			if( rightDiff == 0 )return Differential::shared_ptr();
			if( !newDiff->ConnectChildMech( rightDiff, 1 ) )return Differential::shared_ptr();
		}
		else
		{
			//Проверка индекса
			if( diffParams->vehicleAxleIndex < 0 || diffParams->vehicleAxleIndex >= int(axles.size()) )return Differential::shared_ptr();
			PhysXVehicleAxle::shared_ptr tempAxle = axles.at( diffParams->vehicleAxleIndex );

			//Подсоединяем левое колесо
			if( !newDiff->ConnectChildMech( tempAxle->GetLeftWheelNode(), 0 ) )return Differential::shared_ptr();

			//Подсоединяем правое колесо
			if( !newDiff->ConnectChildMech( tempAxle->GetRightWheelNode(), 1 ) )return Differential::shared_ptr();

			tempAxle->_isDriven = true;
		}
		return newDiff;
	}
	//------------------------------------------------------------
	bool PhysXVehicle::InitializeGearbox( GearboxParams::const_shared_ptr gearParams )
	{
		//Проверка готовности диффера
		if( differential == 0 )return false;

		Gearbox::GearboxInitData giData;
		giData.gearboxParams = gearParams;

		switch( gearParams->gearboxType )
		{
		case GT_MECHANICAL:
			{
				MechGearbox::shared_ptr mechG( new MechGearbox() );
				if( !mechG->Initialize( giData ) )return false;
				if( !mechG->ConnectChildMech( differential ) )return false;
				gearbox = mechG;
				return true;
			}
			break;
		case GT_AUTOMATIC:
			{
				AutoGearbox::shared_ptr autoG( new AutoGearbox() );
				if( !autoG->Initialize( giData ) )return false;
				if( !autoG->ConnectChildMech( differential ) )return false;
				gearbox = autoG;
				return true;
			}
			break;
		};
		return false;
	}
	//------------------------------------------------------------
	bool PhysXVehicle::InitializeMotor( MotorParams::const_shared_ptr motorParams )
	{
		if( gearbox == 0 )return false;

		IPedalState::shared_ptr state;
		state.downcast_assign( gearbox->GetClutchInterface( PIT_PSTATE ) );
		if( state == 0 )return false;
		Motor::MotorInitData miData;
		miData.clutchState = state;
		miData.motorParams = motorParams;

		switch( gearbox->GetGearboxType() )
		{
		case GT_MECHANICAL:
			{
				MechGearbox::shared_ptr mechG; mechG.downcast_assign( gearbox );
				if( mechG == 0 )return false;
				miData.gearboxState = mechG->GetStateInterface();
				miData.gearboxCallback = mechG->GetCallbackInterface();
			}
			break;
		case GT_AUTOMATIC:
			{
				AutoGearbox::shared_ptr autoG; autoG.downcast_assign( gearbox );
				if( autoG == 0 )return false;
				miData.gearboxCallback = autoG->GetCallbackInterface();
				miData.gearboxState = autoG->GetStateInterface();
			}
			break;
		default:
			return false;
		};

		motor.bind( new Motor() );
		if( !motor->Initialize( miData ) )return false;
		if( !motor->ConnectChildMech( gearbox ) )return false;

		return true;
	}
	//------------------------------------------------------------
	void PhysXVehicle::OperateController()
	{
		operateDel();
	}
	//------------------------------------------------------------
	void PhysXVehicle::OperatePhysx()
	{
		physxActor->addForce( NxVec3( 0, -addDownForce, 0 ) );
		brakeControl->OperateController();
		throttleControl->OperateController();
		steerControl->OperateController();
		motor->OperateMechanics( 0.0f, 0.0f );
		OperateNonDrivenAxles();
		osn->setPosition(OPC::toOgre(physxActor->getGlobalPosition()));
		osn->setOrientation(OPC::toOgre(physxActor->getGlobalOrientation()));
		Core::Controller::OperateController();
	}
	//------------------------------------------------------------
	void PhysXVehicle::OperateNoPhysx()
	{
		Core::Controller::OperateController();
	}
	//------------------------------------------------------------
	bool PhysXVehicle::StartPhysX( const PhysXVehicle::StartPhysXVehicleInitData &data )
	{
		if( isPhysXStarted() )return true;

		if( physxBodyMaterial == 0 )
		{
			PPC::NxMaterialDescPtr bodyMatDesc = PPC::GetSingleton().CreateMaterialDesc( physxMatName );
			if( bodyMatDesc != 0 )
			{
				physxBodyMaterial = PSI::get()->createMaterial( bodyMatDesc.getReference() );
			}
		}

		NxActorDesc actorDesc;
		NxConvexShapeDesc convexShapeDesc;
		NxBoxShapeDesc boxShapeDesc;
		convexShapeDesc.meshData = System::PhysXController::GetSingleton().CreateConvexMesh( this->boundMeshName );
		if( convexShapeDesc.meshData != 0 )
		{
			actorDesc.shapes.pushBack( &convexShapeDesc );
			if( physxBodyMaterial != 0 )
				convexShapeDesc.materialIndex = physxBodyMaterial->getMaterialIndex();
		}
		else 
		{
			boxShapeDesc.dimensions = OPC::toNx(this->boundBox);
			actorDesc.shapes.pushBack( &boxShapeDesc );
			if( physxBodyMaterial != 0 )
				boxShapeDesc.materialIndex = physxBodyMaterial->getMaterialIndex();
		}

		NxBodyDesc bodyDesc;
		bodyDesc.solverIterationCount = this->solverCount;
		bodyDesc.mass = this->mass;
		if( !bodyDesc.isValid()){ ClosePhysXVehicle(); return false; }
		actorDesc.body = &bodyDesc;
		actorDesc.globalPose.t = OPC::toNx( data.position ); 

		if( !actorDesc.isValid() ) return false;
		this->physxActor = PSI::get()->createActor( actorDesc );
		if( this->physxActor == 0 ) return false;
		physxActor->setCMassOffsetLocalPosition( OPC::toNx( DUI::DistanceToUnits( massOffset ) ) );

		NxMaterialDesc materialDesc;
		materialDesc.flags |= NX_MF_DISABLE_FRICTION; //materialDesc.flags |= NX_MF_DISABLE_STRONG_FRICTION;
		NxMaterialIndex matIndex = PSI::get()->createMaterial(materialDesc)->getMaterialIndex();
		NxQuat qOrientation;
		qOrientation.fromAngleAxis(180.f, NxVec3(0,-1,0));

		PhysXVehicle::AxleArray::const_iterator iter = this->axles.begin();
		PhysXVehicle::AxleArray::const_iterator end_iter = this->axles.end();
		for( ; iter != end_iter; ++iter )
		{
			NxWheelShapeDescPtr lWheel = (*iter)->leftWheelNode->wheelShapeDesc;
			NxWheelShapeDescPtr rWheel = (*iter)->rightWheelNode->wheelShapeDesc;

			NxReal wheelXShift(0);
			wheelXShift = (*iter)->position.x + (*iter)->leftWheelNode->ironWheelIncShift + (*iter)->leftWheelNode->rubberWheelIncShift;
			wheelXShift += (*iter)->leftWheelNode->rubberWheel != 0 ? (*iter)->leftWheelNode->rubberWheel->width / 2 : (*iter)->leftWheelNode->ironWheel->width / 2;
			lWheel->localPose.t = NxVec3( -wheelXShift, (*iter)->position.y, (*iter)->position.z );
			wheelXShift = (*iter)->position.x + (*iter)->rightWheelNode->ironWheelIncShift + (*iter)->rightWheelNode->rubberWheelIncShift;
			wheelXShift += (*iter)->rightWheelNode->rubberWheel != 0 ? (*iter)->rightWheelNode->rubberWheel->width / 2 : (*iter)->rightWheelNode->ironWheel->width / 2;
			rWheel->localPose.t = NxVec3( wheelXShift, (*iter)->position.y, (*iter)->position.z );

			lWheel->materialIndex = matIndex;
			rWheel->materialIndex = matIndex;
			lWheel->localPose.M.fromQuat( qOrientation );
			rWheel->localPose.M.fromQuat( qOrientation );
			
			if( !lWheel->isValid() || !rWheel->isValid() )	return false;
			NxShape* tempShape = 0;

			tempShape = this->physxActor->createShape( *lWheel );
			if( !tempShape ) return false;
			(*iter)->leftWheelNode->wheelShape = static_cast<NxWheelShape*>( tempShape );

			tempShape = this->physxActor->createShape( *rWheel );
			if( !tempShape ) return false;
			(*iter)->rightWheelNode->wheelShape = static_cast<NxWheelShape*>( tempShape );
		}

		operateDel.bind( this, &PhysXVehicle::OperatePhysx );

		return true;
	}
	//------------------------------------------------------------
	void PhysXVehicle::StopPhysX()
	{
		PhysXVehicle::AxleArray::const_iterator iter = this->axles.begin();
		PhysXVehicle::AxleArray::const_iterator end_iter = this->axles.end();
		for( ; iter != end_iter; ++iter )
		{
			if( (*iter) != 0 )
			{
				if( (*iter)->leftWheelNode != 0 )
					(*iter)->leftWheelNode->wheelShape = 0;
				if( (*iter)->rightWheelNode != 0 )
					(*iter)->rightWheelNode->wheelShape = 0;
			}
		}
		if( physxActor != 0 )
		{
			PSI::get()->releaseActor( *physxActor ); 
			physxActor = 0;
		}
		operateDel.bind( this, &PhysXVehicle::OperateNoPhysx );
	}
	//------------------------------------------------------------
	void PhysXVehicle::OperateNonDrivenAxles()
	{
		AIter iter = nonDrivenAxles.begin();
		AIter end_iter = nonDrivenAxles.end();
		for( ; iter != end_iter; ++iter )
		{
			(*iter)->GetLeftWheelNode()->OperateMechanics( 0, 0 );
			(*iter)->GetRightWheelNode()->OperateMechanics( 0, 0 );
		}
	}
	//------------------------------------------------------------
	IMechanicsState::shared_ptr PhysXVehicle::GetMechanicsStateInterface( MECHANICS_TYPE type, int axleIndex, VEHICLE_SIDE side )
	{
		switch( type )
		{
		case MT_MOTOR:
			if( motor != 0 )return motor->GetMechanicsStateInterface();
			break;
		case MT_GEARBOX:
			if( gearbox != 0 )return gearbox->GetMechanicsStateInterface();
			break;
		case MT_DIFFERENTIAL:
			if( differential != 0 )
			{
				if( axleIndex == -1 )return differential->GetMechanicsStateInterface();
			}
			break;
		case MT_WHEEL_NODE:
			if( axleIndex >= 0 && axleIndex < (int)axles.size() )
			{
				if( side == VS_LEFT )return axles.at( axleIndex )->leftWheelNode->GetMechanicsStateInterface();
				if( side == VS_RIGHT )return axles.at( axleIndex )->rightWheelNode->GetMechanicsStateInterface();
			}
			break;
		};
		return IMechanicsState::shared_ptr();
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_PHYSX_VEHICLE
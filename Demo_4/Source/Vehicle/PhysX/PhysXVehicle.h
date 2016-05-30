#pragma once
#include "Vehicle\Abstract\BaseVehicle.h"
#include "PhysXVehicleBodyParams.h"
#include "PhysXWheelNode.h"
#include "PhysXVehicleAxle.h"

class NxActor;
class NxMaterial;

namespace Demo
{
namespace Vehicle
{
//------------------------------------------------------------
class PhysXVehicle : public BaseVehicle
{
public:
	DEFINE_CLASS_SHARED_PTR(PhysXVehicle)
	//--------------------------------//
	typedef PhysXWheelNode::NxWheelShapeDescPtr NxWheelShapeDescPtr;
	//--------------------------------//
	struct PhysXVehicleInitData
	{
		PhysXVehicleBodyParams::const_shared_ptr bodyParams;
	};
	//--------------------------------//
	struct StartPhysXVehicleInitData
	{
		Ogre::Vector3 position;
		Ogre::Quaternion orientation;
	};
	//--------------------------------//
	typedef PhysXVehicleAxle::shared_ptr axle_elem;
	typedef std::vector< axle_elem > AxleArray;
	typedef AxleArray::iterator AIter;
	typedef AxleArray::const_iterator cAIter;
	//--------------------------------//
	static PhysXVehicle::shared_ptr CreatePhysXVehicle( string_traits::in name );
	//--------------------------------//
	PhysXVehicle();
	virtual ~PhysXVehicle(){if( isInitializedController() )ClosePhysXVehicle();}

	//Controller interface
	virtual void OperateController();
	//Controller interface

	virtual IMechanicsState::shared_ptr GetMechanicsStateInterface( MECHANICS_TYPE type, int axleIndex = -1, VEHICLE_SIDE side = VS_NOT_DEFINED );

	bool Initialize( const PhysXVehicleInitData& data );
	void ClosePhysXVehicle();

	bool StartPhysX( const StartPhysXVehicleInitData& data );
	void StopPhysX();

	inline bool isPhysXStarted()const{ return physxActor != 0; }

protected:
	/*
	�������������� ���� ��������������, ������������ �����
	��������! ����� ������� - axles ������ ���� ������
	�����������: ���������� �� bool, � Differential* - ��� ������������ ������ ������ ����� �������
	*/
	Differential::shared_ptr InitializeDifferential( DifferentialParams::const_shared_ptr );
	/*
	�������������� ������� �������, ������������ ������������
	��������! ����� ������� - differential ������ ���� �����
	*/
	bool InitializeGearbox( GearboxParams::const_shared_ptr );
	/*
	�������������� �����, ������������ �������
	��������! ����� ������� - gearbox ������ ���� �����
	*/
	bool InitializeMotor( MotorParams::const_shared_ptr );
	void OperatePhysx();
	void OperateNoPhysx();
	void OperateNonDrivenAxles();

	int solverCount;
	float addDownForce;
	string_traits::str boundMeshName;
	string_traits::str physxMatName;
	AxleArray axles;
	AxleArray nonDrivenAxles;
	NxActor* physxActor;
	NxMaterial* physxBodyMaterial;
	delegate_traits::del0<void>::type operateDel;

private:
	PhysXVehicle( const PhysXVehicle& );
	PhysXVehicle& operator=( const PhysXVehicle& );
};
//------------------------------------------------------------
}
}
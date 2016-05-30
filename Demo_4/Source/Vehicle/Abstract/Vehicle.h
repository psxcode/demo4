#pragma once
#include "Core\OgreSceneNodeController.h"
#include "Vehicle\VehicleSetParams.h"
#include "Vehicle\VehicleMotor.h"
#include "Vehicle\Axle\Gearbox\Gearbox.h"
#include "Vehicle\Axle\Differential\Differential.h"
#include "Vehicle\Axle\Wheel\Wheel.h"
#include "Vehicle\VehicleTypes.h"

namespace Demo
{
namespace Driver
{
}
namespace Vehicle
{
	//------------------------------------------------------------
	/*
	Базовый класс для реализации логики машины
	Для PhysX будет определён субкласс PhysxVehicle
	*/
	class BaseVehicle : public Core::OSNController
	{
	public:
		//--------------------------------//
		struct BaseVehicleInitData : public Controller::InitData
		{
			BaseVehicleInitData()
				: setParams(0)
			{
			}
			virtual ~BaseVehicleInitData()
			{
			}
			const VehicleSetParams* setParams;
		};
		//--------------------------------//
		virtual ~BaseVehicle(){	Close(); }

		//Controller interface
		virtual void Close();
		//Controller interface

	protected:
		//--------------------------------//
		typedef std::vector< VehicleWheelAxle* > WheelAxleArray;
		typedef WheelAxleArray::iterator AIter;
		typedef WheelAxleArray::const_iterator cAIter;
		//--------------------------------//
		//Оси
		WheelAxleArray wheelAxles;

		//Имена
		Ogre::String setName;
		Ogre::String vehicleName;

		//Двигатель
		VehicleMotor* motor;
		//Коробка передач
		Axle::Gearbox* gearbox;
	};
	//------------------------------------------------------------
}
}

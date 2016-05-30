#include "DemoStableHeaders.h"
#if DBS_LM_VEHICLE_DRIVER

#include "VehicleDriver.h"
#include "Interface\UnitConverter.h"
#include "Interface\InputInterface.h"
#include "drive_options.h"
#include "System\VehicleParamsCollection.h"
#include "Vehicle\Abstract\IGearboxControl.h"
#include "Vehicle\Abstract\IGearboxState.h"
#include "Vehicle\Abstract\IGearboxCallback.h"
#include "MechGearboxController.h"
#include "AutoGearboxController.h"
#include "Vehicle\Abstract\IMotorState.h"
#include "Vehicle\Abstract\VehicleTypes.h"


namespace Demo
{
namespace Vehicle
{
	//------------------------------------------------------------
	typedef TimeInterface FTI;
	typedef InputInterface II;
	typedef System::VehicleParamsCollection VPC;
	//------------------------------------------------------------
	VehicleDriver::shared_ptr VehicleDriver::CreateVehicleDriver( const CreateData& cdata )
	{
		if( !cdata.isValid() )return VehicleDriver::shared_ptr();

		System::VehicleDriverRSD driverRSD = VPC::GetSingleton().CreateVehicleDriverRSD();
		VehicleDriver::InitData idata;
		
		//Если не указан хоть один параметр из двух
		if( driverRSD.calmTimingName.empty() && driverRSD.agrTimingName.empty() )return VehicleDriver::shared_ptr();
		if( driverRSD.calmMCMPName.empty() && driverRSD.agrMCMPName.empty() )return VehicleDriver::shared_ptr();

#pragma region create driver timing
		//Если оба тайминга есть ( также должен присутствовать iDriveStyleState )
		if( !driverRSD.calmTimingName.empty() && !driverRSD.agrTimingName.empty() && cdata.iDriveStyleState != 0 )
		{
			LinearDriverTimingCalculator::LinearInitData ldtIData;

			ldtIData.calm = VPC::GetSingleton().CreateDriverTiming( driverRSD.calmTimingName );
			if( ldtIData.calm == 0 )return VehicleDriver::shared_ptr();

			ldtIData.agr = VPC::GetSingleton().CreateDriverTiming( driverRSD.agrTimingName );
			if( ldtIData.agr == 0 )return VehicleDriver::shared_ptr();

			ldtIData.iStyle = cdata.iDriveStyleState;

			LinearDriverTimingCalculator::shared_ptr ldtc( new LinearDriverTimingCalculator() );
			if( !ldtc->Initialize( ldtIData ) )return VehicleDriver::shared_ptr();

			idata.driverTimingCalc = ldtc;
		}
		else
		{
			//Если указан один тайминг
			DriverTiming::shared_ptr timing;
			if( !driverRSD.calmTimingName.empty() )	timing = VPC::GetSingleton().CreateDriverTiming( driverRSD.calmTimingName );
			else timing = VPC::GetSingleton().CreateDriverTiming( driverRSD.agrTimingName );
			if( timing == 0 )return VehicleDriver::shared_ptr();

			ConstantDriverTimingCalculator::ConstantInitData cdtIData;
			cdtIData.timing = timing;

			ConstantDriverTimingCalculator::shared_ptr cdtc( new ConstantDriverTimingCalculator() );
			if( !cdtc->Initialize( cdtIData ) )return VehicleDriver::shared_ptr();

			idata.driverTimingCalc = cdtc;
		}
#pragma endregion

#pragma region create MCMP
		//Если указаны оба MCMP ( также должен присутствовать iDriveStyleState )
		if( !driverRSD.calmMCMPName.empty() && !driverRSD.agrMCMPName.empty() && cdata.iDriveStyleState != 0 )
		{
			LinearMotorChartMarksCalculator::LMCMCInitData linIData;

			linIData.calm = VPC::GetSingleton().CreateMotorChartMarksPack( driverRSD.calmMCMPName );
			if( linIData.calm == 0 )return VehicleDriver::shared_ptr();

			linIData.agr = VPC::GetSingleton().CreateMotorChartMarksPack( driverRSD.agrMCMPName );
			if( linIData.agr == 0 )return VehicleDriver::shared_ptr(); 

			linIData.iStyle = cdata.iDriveStyleState;
			linIData.iState.downcast_assign( cdata.vtdData->gearboxControl->GetInterface( GIT_STATE ) );
			linIData.iCallback.downcast_assign( cdata.vtdData->gearboxControl->GetInterface( GIT_CALLBACK ) );

			LinearMotorChartMarksCalculator::shared_ptr lmcmc( new LinearMotorChartMarksCalculator() );
			if( !lmcmc->Initialize( linIData ) )return VehicleDriver::shared_ptr();

			idata.mcmCalc = lmcmc;
		}
		else
		{
			//Если указан один MCMP
			MotorChartMarksPack::shared_ptr mcmp;
			if( !driverRSD.calmMCMPName.empty() ) mcmp = VPC::GetSingleton().CreateMotorChartMarksPack( driverRSD.calmMCMPName );
			else mcmp = VPC::GetSingleton().CreateMotorChartMarksPack( driverRSD.agrMCMPName );
			if( mcmp == 0 )return VehicleDriver::shared_ptr();

			ConstantMotorChartMarksCalculator::CMCMCInitData constIData;
			constIData.mcmp = mcmp;
			constIData.iCallback.downcast_assign( cdata.vtdData->gearboxControl->GetInterface( GIT_CALLBACK ) );
			constIData.iState.downcast_assign( cdata.vtdData->gearboxControl->GetInterface( GIT_STATE ) );

			ConstantMotorChartMarksCalculator::shared_ptr cmcmc( new ConstantMotorChartMarksCalculator() );
			if( !cmcmc->Initialize( constIData ) )return VehicleDriver::shared_ptr();

			idata.mcmCalc = cmcmc;
		}
#pragma endregion

		idata.iDriveStyleState = cdata.iDriveStyleState;
		idata.vtdData = cdata.vtdData;

		VehicleDriver::shared_ptr driver( new VehicleDriver() );

		driver->ApplyDriveOptions( cdata.driveOptions );
		if( !driver->Initialize( idata ) )return VehicleDriver::shared_ptr();
		return driver;
	}
	//------------------------------------------------------------
#pragma region constructor
	VehicleDriver::VehicleDriver()
		: userGoPress( false )
		, userStopPress( false )
		
		, iThrottleCallback(0)
		, iThrottleControl(0)
		, iThrottleState(0)
		, throttlePressDel(0)
		, throttleReleaseDel(0)
		, throttleRpmAboveMaxDel(0)
		, throttleRpmAboveNormDel(0)
		, throttleRpmNormDel(0)
		, throttleRpmSpeedIncDel(0)
		, userThrottlePress( false )
		
		, iClutchCallback(0)
		, iClutchControl(0)
		, iClutchState(0)
		, clutchAxisDel(0)
		, clutchPressDel(0)
		, clutchReleaseDel(0)
		, clutchRpmBelowMinDel(0)
		, clutchRpmBelowNormDel(0)
		, clutchRpmNormDel(0)
		, userClutchPress( false )

		, iBrakeCallback(0)
		, iBrakeControl(0)
		, iBrakeState(0)
		, brakePressDel(0)
		, brakeReleaseDel(0)
		, userBrakePress( false )

		, iHandbrakeCallback(0)
		, iHandbrakeControl(0)
		, iHandbrakeState(0)
		, handbrakePressDel(0)
		, handbrakeReleaseDel(0)
		, userHandbrakePress( false )

		, iSteerCallback(0)
		, iSteerControl(0)
		, iSteerState(0)
		, steerPressLeftDel(0)
		, steerPressRightDel(0)
		, steerReleaseDel(0)
		, userSteerPress( false )

		, iGearboxControl(0)
		, gearbox(0)
		, shiftDownPressDel(0)
		, shiftEndedDel(0)
		, shiftStartedDel(0)
		, shiftUpPressDel(0)

		, driveOptions(0)
		, controlAxes(0)
		, controlSystems(0)
		, iMotorState(0)
		, speedToGear(0)
		, driverTimingCalc(0)
	{
	}
#pragma endregion
	//------------------------------------------------------------
	VehicleDriver::~VehicleDriver()
	{
		CloseVehicleDriver();
	}
	//------------------------------------------------------------
	bool VehicleDriver::Initialize( const InitData& data )
	{
		//early checks
		if( !data.isValid() )return false;
		
		iDriveStyleState = data.iDriveStyleState;
		
		driverTimingCalc = data.driverTimingCalc;
		mcmCalc = data.mcmCalc;

		//Создание radPSTimingCalculator
		switch( driverTimingCalc->getCalcType() )
		{
		case DTCT_CONSTANT:
			{
				ConstantDriverTimingCalculator::const_shared_ptr constDTC;
				constDTC.downcast_assign( driverTimingCalc );
				if( constDTC == 0 )return false;
				ConstantRadPSTimingCalculator::shared_ptr constTimingCalc( new ConstantRadPSTimingCalculator() );
				RadPSTiming radPSTiming;
				radPSTiming.speedReactionTime = constDTC->getTiming()->rpmSpeedReactionTime;
				radPSTiming.zoneReactionTime = constDTC->getTiming()->rpmZoneReactionTime;
				if( !constTimingCalc->Initialize( radPSTiming ) )return false;
				radPSTC = constTimingCalc;
			}
			break;
		case DTCT_LINEAR:
			{
				LinearDriverTimingCalculator::const_shared_ptr linDTC;
				linDTC.downcast_assign( driverTimingCalc );
				if( linDTC == 0 )return false;
				LinearRadPSTimingCalculator::LinearInitData idata;
				idata.iStyle = iDriveStyleState;
				idata.lowTiming.zoneReactionTime = linDTC->getCalmTiming()->rpmZoneReactionTime;
				idata.lowTiming.speedReactionTime = linDTC->getCalmTiming()->rpmSpeedReactionTime;
				idata.highTiming.zoneReactionTime = linDTC->getAgrTiming()->rpmZoneReactionTime;
				idata.highTiming.speedReactionTime = linDTC->getCalmTiming()->rpmSpeedReactionTime;
				LinearRadPSTimingCalculator::shared_ptr linTimingCalc( new LinearRadPSTimingCalculator() );
				if( !linTimingCalc->Initialize( idata ) )return false;
				radPSTC = linTimingCalc;
			}
			break;
		default:
			return false;
		};

		//Vehicle to driver data
		controlAxes = data.vtdData->controlAxes;
		controlSystems = data.vtdData->controlSystems;

		iMotorControl = data.vtdData->motorControl;
		iMotorState = data.vtdData->motorState;
		iMotorCallback = data.vtdData->motorCallback;

		motorStallDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnMotorStall ) );
		motorStartDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnMotorStart ) );
		iMotorCallback->AddStallRPMDel( motorStallDel );
		iMotorCallback->AddStartRPMDel( motorStartDel );

		iGearboxControl = data.vtdData->gearboxControl;
		speedToGear = data.vtdData->speedToGear;
		speedometer = data.vtdData->speedometer;

		//Создание radPSController
		{
			//создаём InitData для radPSController
			MotorChartRadPSController::MCInitData idata;
			idata.timing = radPSTC;
			idata.mechanicsState = iMotorState;
			idata.iMotorState = iMotorState;
			idata.mcmCalc = mcmCalc;
			//Инициализируем radPSController
			radPSController.bind( new MotorChartRadPSController() );
			if( !radPSController->Initialize( idata ) )return false;
			if( !this->ConnectChildController( radPSController.getPointer() ) )return false;

			//throttle marks & zones
			if( (driveOptions & DO_THROTTLE_ASSIST) != 0 )
			{
				if( !CreateThrottleAssist() )return false;
			}
		}

#pragma region control axes
		iThrottleControl = data.vtdData->throttle;
		if( iThrottleControl == 0 )return false;
		iThrottleState.downcast_assign( iThrottleControl->GetInterface( PIT_PSTATE ) );
		iThrottleCallback.downcast_assign( iThrottleControl->GetInterface( PIT_PCALLBACK ) );
		if( iThrottleCallback == 0 || iThrottleState == 0 )return false;
		iThrottleControl->SetPressTime( driverTimingCalc->GetThrottlePressTime() );
		iThrottleControl->SetReleaseTime( driverTimingCalc->GetThrottleReleaseTime() );

		iBrakeControl = data.vtdData->brake;
		if( iBrakeControl == 0 )return false;
		iBrakeCallback.downcast_assign( iBrakeControl->GetInterface( PIT_PCALLBACK ) );
		iBrakeState.downcast_assign( iBrakeControl->GetInterface( PIT_PSTATE ) );
		if( iBrakeCallback == 0 || iBrakeState == 0 )return false;
		iBrakeControl->SetPressTime( driverTimingCalc->GetBrakePressTime() );
		iBrakeControl->SetReleaseTime( driverTimingCalc->GetBrakeReleaseTime() );

		//Если есть педаль сцепления на машине
		if( (controlAxes & VCA_CLUTCH) != 0 )
		{
			iClutchControl = data.vtdData->clutch;
			if( iClutchControl == 0 )return false;
			iClutchCallback.downcast_assign( iClutchControl->GetInterface( PIT_PCALLBACK ) );
			iClutchState.downcast_assign( iClutchControl->GetInterface( PIT_PSTATE ) );
			if( iClutchCallback == 0 || iClutchState == 0 )return false;
			iClutchControl->SetPressTime( driverTimingCalc->GetClutchPressTime() );
			iClutchControl->SetReleaseTime( driverTimingCalc->GetClutchReleaseTime() );

			//Если указано помогать пользователю со сцеплением
			if( (driveOptions & DO_CLUTCH_ASSIST) != 0 )
			{
				if( !CreateClutchAssist() )return false;
			}
		}

		//Если есть ручной тормоз на машине
		if( (controlAxes & VCA_HANDBRAKE) != 0 )
		{
		}

		iSteerControl = data.vtdData->steer;
		if( iSteerControl == 0 )return false;
		iSteerCallback.downcast_assign( iSteerControl->GetInterface( PIT_SCALLBACK ) );
		iSteerState.downcast_assign( iSteerControl->GetInterface( PIT_SSTATE ) );
		if( iSteerCallback == 0 || iSteerState == 0 )return false;
		iSteerControl->SetPressTime( driverTimingCalc->GetSteerPressTime() );
		iSteerControl->SetReleaseTime( driverTimingCalc->GetSteerReleaseTime() );
#pragma endregion

		//create gearbox controller
		if( !CreateGearboxController() )return false;

		setInitializeController();
		return true;
	}
	//------------------------------------------------------------
	void VehicleDriver::CloseVehicleDriver()
	{
		RemoveUserInput();
		RemoveClutchAssist();
		RemoveThrottleAssist();
		CloseGearboxController();

		//radPSController закрывается после GearboxController
		//автоматически удаляются все добавленные делегаты
		if( radPSController != 0 ) {
			radPSController->CloseController();
			radPSController = 0;
		}

		iSteerCallback = 0;
		iSteerState = 0;
		iSteerControl = 0;

		//Закрытие Clutch
		iClutchCallback = 0;
		iClutchState = 0;
		iClutchControl = 0;
		//Закрытие Clutch

		iBrakeCallback = 0;
		iBrakeState = 0;
		iBrakeControl = 0;

		iThrottleCallback = 0;
		iThrottleState = 0;
		iThrottleControl = 0;

		speedToGear = 0;
		iMotorState = 0;
		iGearboxControl = 0;

		controlAxes = 0;
		controlSystems = 0;

		radPSTC = 0;
		iDriveStyleState = 0;
		driverTimingCalc = 0;
		mcmCalc = 0;
		
		Controller::CloseController();
	}
	//------------------------------------------------------------
	bool VehicleDriver::CreateGearboxController()
	{
		switch( iGearboxControl->GetGType() )
		{
		case GT_MECHANICAL:
			{
				IMechGearboxControl::shared_ptr imech;
				imech.downcast_assign( iGearboxControl );
				if( imech == 0 )return false;

				MechGearboxController::shared_ptr Mech( new MechGearboxController() );
				MechGearboxController::MechGearboxControllerInitData mechData;
				mechData.driveOptions = driveOptions;
				mechData.timing = driverTimingCalc;

				if( (driveOptions & DO_CLUTCH_ASSIST) != 0 )
				{
					mechData.iClutchControl = iClutchControl;
					mechData.radPSController = radPSController;
					mechData.speedToGear = speedToGear;
				}

				mechData.iGearboxControl = imech;
				if( !Mech->Initialize( mechData ) )return false;
				gearbox = Mech;
			}
			break;
		case GT_AUTOMATIC:
			{
				IAutoGearboxControl::shared_ptr iauto;
				iauto.downcast_assign( iGearboxControl );
				if( iauto == 0 )return false;
				
				AutoGearboxController::shared_ptr Auto( new AutoGearboxController() );
				AutoGearboxController::AutoGearboxControllerInitData autoData;
				autoData.driveOptions = driveOptions;
				autoData.timing = driverTimingCalc;
				if( !Auto->Initialize( autoData ) )return false;
				gearbox = Auto;
			}
			break;
		default:
			return false;
		};

		if( (driveOptions & DO_CLUTCH_ASSIST) != 0 )
		{
			shiftStartedDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnShiftStarted ) );
			shiftEndedDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnShiftEnded ) );
			gearbox->AddShiftStartedOnceDel( shiftStartedDel );
			gearbox->AddShiftEndedOnceDel( shiftEndedDel );
		}

		if( !this->ConnectChildController( gearbox.getPointer() ) )return false;

		return true;
	}
	//------------------------------------------------------------
	void VehicleDriver::CloseGearboxController()
	{
		if( gearbox != 0 )
		{
			this->DisconnectChildController( gearbox.getPointer() );
			gearbox->CloseController();
			gearbox = 0;
		}
	}
	//------------------------------------------------------------
	void VehicleDriver::ApplyDriveOptions(int options)
	{
		driveOptions = options;

		if( !mInitializedController )return;

		CloseGearboxController();
		CreateGearboxController();
		RemoveClutchAssist();
		if( (driveOptions & DO_CLUTCH_ASSIST) != 0 )
			CreateClutchAssist();
		RemoveUserInput();
		if( (driveOptions & DO_USER_CONTROLS) != 0 )
			CreateUserInput();
	}
	//------------------------------------------------------------
	bool VehicleDriver::CreateClutchAssist()
	{
		if( radPSController == 0 )return false;
		
		RemoveClutchAssist();

		//старая логика контроля сцепления
		//clutchRpmSpeedDecDel.bind( new vdel( this, &VehicleDriver::OnClutchRpmSpeedDec ) );

		//новая логика контроля сцепления
		clutchSmPressZoneTimerDel.bind( new delegate_traits::active_del0<void>( this, &VehicleDriver::OnClutchSmPressZoneTimer ) );
		clutchSmPressZoneTimer.AddOnceDel( clutchSmPressZoneTimerDel );

		clutchRpmBelowMinDel.bind( new delegate_traits::active_del0<void>( this, &VehicleDriver::OnClutchRpmBelowMin ) );
		clutchRpmBelowNormDel.bind( new delegate_traits::active_del0<void>( this, &VehicleDriver::OnClutchRpmBelowNorm ) );
		clutchRpmNormDel.bind( new delegate_traits::active_del0<void>( this, &VehicleDriver::OnClutchRpmNorm ) );
		
		radPSController->addMinBelowOnceDel( clutchRpmBelowMinDel );
		radPSController->addMinOnceDel( clutchRpmBelowNormDel );
		radPSController->addNormLowAboveOnceDel( clutchRpmNormDel );

		/*
		clutch пока не будет контролировать высокие обороты

		clutchRpmSpeedIncDel.bind( new vdel( this, &VehicleDriver::OnClutchRpmSpeedInc ) );
		clutchRpmAboveMaxDel.bind( new vdel( this, &VehicleDriver::OnClutchRpmAboveMax ) );
		radPSController->addMaxAboveOnceDel( clutchRpmAboveMaxDel );
		*/

		return true;
	}
	//------------------------------------------------------------
	bool VehicleDriver::CreateThrottleAssist()
	{
		if( radPSController == 0 )return false;

		RemoveThrottleAssist();

		throttleRpmNormDel.bind( new delegate_traits::active_del0<void>( this, &VehicleDriver::OnThrottleRpmNorm ) );
		throttleRpmAboveNormDel.bind( new delegate_traits::active_del0<void>( this, &VehicleDriver::OnThrottleRpmAboveNorm ) );
		throttleRpmAboveMaxDel.bind( new delegate_traits::active_del0<void>( this, &VehicleDriver::OnThrottleRpmAboveMax ) );
		throttleRpmSpeedIncDel.bind( new delegate_traits::active_del0<void>( this, &VehicleDriver::OnThrottleRpmSpeedInc ) );

		radPSController->addNormHighBelowOnceDel( throttleRpmNormDel );
		radPSController->addMaxOnceDel( throttleRpmAboveNormDel );
		radPSController->addMaxAboveOnceDel( throttleRpmAboveMaxDel );

		return true;
	}
	//------------------------------------------------------------
	void VehicleDriver::RemoveThrottleAssist()
	{
		if( radPSController != 0 )
		{
			if( throttleRpmNormDel != 0 )radPSController->removeNormHighBelowOnceDel( throttleRpmNormDel );
			if( throttleRpmAboveNormDel != 0 )radPSController->removeMaxOnceDel( throttleRpmAboveNormDel );
			if( throttleRpmAboveMaxDel != 0 )radPSController->removeMaxAboveOnceDel( throttleRpmAboveMaxDel );
			if( throttleRpmSpeedIncDel != 0 )radPSController->removeSpeedDel( throttleRpmSpeedIncDel );
		}
		throttleRpmNormDel = 0;
		throttleRpmAboveNormDel = 0;
		throttleRpmAboveMaxDel = 0;
		throttleRpmSpeedIncDel = 0;
	}
	//------------------------------------------------------------
	void VehicleDriver::RemoveClutchAssist()
	{
		if( radPSController != 0 )
		{
			//старая логика контроля сцепления
			//if( clutchRpmSpeedDecDel != 0 ) radPSController->removeSpeedDel( clutchRpmSpeedDecDel );

			if( clutchRpmBelowMinDel != 0 ) radPSController->removeMinBelowOnceDel( clutchRpmBelowMinDel );
			if( clutchRpmBelowNormDel != 0 ) radPSController->removeNormLowBelowOnceDel( clutchRpmBelowNormDel );
			if( clutchRpmNormDel != 0 ) radPSController->removeNormOnceDel( clutchRpmNormDel );
			//if( clutchRpmAboveMaxDel != 0 ) radPSController->removeMaxAboveOnceDel( clutchRpmAboveMaxDel );
		}
		clutchRpmBelowMinDel = 0;
		clutchRpmBelowNormDel = 0;
		clutchRpmNormDel = 0;
		//clutchRpmAboveMaxDel = 0;

		//новая логика контроля сцепления
		clutchSmPressZoneTimer.CloseDelTimer();
		clutchSmPressZoneTimerDel = 0;

		//старая логика контроля сцепления
		//clutchRpmSpeedDecDel = 0;
	}
	//------------------------------------------------------------
	bool VehicleDriver::CreateUserInput()
	{
		RemoveUserInput();

		if( (driveOptions & DO_SHIFT_ASSIST) != 0 )
		{
			goPressDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnGoPress ) );
			goStopReleaseDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnGoStopRelease ) );
			stopPressDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnStopPress ) );
			if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_THROTTLE_PRESS, II::bdel_elem( goPressDel ) ) )return false;
			if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_THROTTLE_RELEASE, II::bdel_elem( goStopReleaseDel ) ) )return false;
			if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_BRAKE_PRESS, II::bdel_elem( stopPressDel ) ) )return false;
			if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_BRAKE_RELEASE, II::bdel_elem( goStopReleaseDel ) ) )return false;
		}

		throttlePressDel.bind( new delegate_traits::active_del0<void>( this, iMotorState->IsStall() ? &VehicleDriver::OnThrottlePressEngineOff : (driveOptions & DO_THROTTLE_ASSIST) != 0 ? &VehicleDriver::OnThrottlePressEngineOnAssistOn : &VehicleDriver::OnThrottlePressEngineOnAssistOff ) );
		throttleReleaseDel.bind( new delegate_traits::active_del0<void>( this, iMotorState->IsStall() ? &VehicleDriver::OnThrottleReleaseEngineOff : (driveOptions & DO_THROTTLE_ASSIST) != 0 ? &VehicleDriver::OnThrottleReleaseEngineOnAssistOn : &VehicleDriver::OnThrottleReleaseEngineOnAssistOff ) );

		brakePressDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnBrakePress ) );
		brakeReleaseDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnBrakeRelease ) );

		if( (driveOptions & DO_SHIFT_ASSIST) == 0 )
		{
			if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_THROTTLE_PRESS, II::bdel_elem( throttlePressDel ) ) )return false;
			if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_THROTTLE_RELEASE, II::bdel_elem( throttleReleaseDel ) ) )return false;
			if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_BRAKE_PRESS, II::bdel_elem( brakePressDel ) ) )return false;
			if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_BRAKE_RELEASE, II::bdel_elem( brakeReleaseDel ) ) )return false;

			shiftUpPressDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnShiftUpPress ) );
			shiftDownPressDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnShiftDownPress ) );
			if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_SHIFT_UP_PRESS, II::bdel_elem( shiftUpPressDel ) ) )return false;
			if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_SHIFT_DOWN_PRESS, II::bdel_elem( shiftDownPressDel ) ) )return false;
		}

		if( (controlAxes & VCA_CLUTCH) != 0 && (driveOptions & DO_USER_CLUTCH) != 0 )
		{
			clutchPressDel.bind( new delegate_traits::active_del0<void>( this, &VehicleDriver::OnClutchPress ) );
			clutchReleaseDel.bind( new delegate_traits::active_del0<void>( this, (driveOptions & DO_CLUTCH_ASSIST) != 0 ? &VehicleDriver::OnClutchReleaseAssistOn : &VehicleDriver::OnClutchReleaseAssistOff ) );
			if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_CLUTCH_PRESS, II::bdel_elem( clutchPressDel ) ) )return false;
			if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_CLUTCH_RELEASE, II::bdel_elem( clutchReleaseDel ) ) )return false;
		}

		steerPressLeftDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnSteerPressLeft ) );
		steerPressRightDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnSteerPressRight ) );
		steerReleaseDel.bind( new delegate_traits::del0<void>( this, &VehicleDriver::OnSteerRelease ) );
		if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_STEER_LEFT_PRESS, II::bdel_elem( steerPressLeftDel ) ) )return false;
		if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_STEER_RIGHT_PRESS, II::bdel_elem( steerPressRightDel ) ) )return false;
		if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_STEER_LEFT_RELEASE, II::bdel_elem( steerReleaseDel ) ) )return false;
		if( !II::addInputDelivery( Core::IID_VEHICLE_DRIVER_STEER_RIGHT_RELEASE, II::bdel_elem( steerReleaseDel ) ) )return false;

		return true;
	}
	//------------------------------------------------------------
	void VehicleDriver::RemoveUserInput()
	{
		if( goPressDel != 0 ){ II::removeInputDelivery( II::bdel_elem( goPressDel ) ); goPressDel = 0; }
		if( goStopReleaseDel != 0 ){ II::removeInputDelivery( II::bdel_elem( goStopReleaseDel ) ); goStopReleaseDel = 0; }
		if( stopPressDel != 0 ){ II::removeInputDelivery( II::bdel_elem( stopPressDel ) ); stopPressDel = 0; }
		if( throttlePressDel != 0 ){ II::removeInputDelivery( II::bdel_elem( throttlePressDel ) ); throttlePressDel = 0; }
		if( throttleReleaseDel != 0 ){ II::removeInputDelivery( II::bdel_elem( throttleReleaseDel ) ); throttleReleaseDel = 0; }
		if( brakePressDel != 0 ){ II::removeInputDelivery( II::bdel_elem( brakePressDel ) ); brakePressDel = 0; }
		if( brakeReleaseDel != 0 ){ II::removeInputDelivery( II::bdel_elem( brakeReleaseDel ) ); brakeReleaseDel = 0; }
		if( clutchPressDel != 0 ){ II::removeInputDelivery( II::bdel_elem( clutchPressDel ) ); clutchPressDel = 0; }
		if( clutchReleaseDel != 0 ){ II::removeInputDelivery( II::bdel_elem( clutchReleaseDel ) ); clutchReleaseDel = 0; }
		if( steerPressLeftDel != 0 ){ II::removeInputDelivery( II::bdel_elem( steerPressLeftDel ) ); steerPressLeftDel = 0; }
		if( steerPressRightDel != 0 ){ II::removeInputDelivery( II::bdel_elem( steerPressRightDel ) ); steerPressRightDel = 0; }
		if( steerReleaseDel != 0 ){ II::removeInputDelivery( II::bdel_elem( steerReleaseDel ) ); steerReleaseDel = 0; }
		if( shiftUpPressDel != 0 ){ II::removeInputDelivery( II::bdel_elem( shiftUpPressDel ) ); shiftUpPressDel = 0; }
		if( shiftDownPressDel != 0 ){ II::removeInputDelivery( II::bdel_elem( shiftDownPressDel ) ); shiftDownPressDel = 0; }
	}
	//------------------------------------------------------------
	void VehicleDriver::OperateController()
	{
		clutchSmPressZoneTimer.Operate();
		Core::Controller::OperateController();
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//SimpleControls----------------------------------------------
	void VehicleDriver::OnGoPress()
	{
		if( speedometer->GetSpeedMS() > -0.1f )
		{
			//accel
			if( !gearbox->IsDirect() )
				gearbox->ShiftToDirect();

			(*throttlePressDel)();
		}
		else
		{
			//brake
			(*brakePressDel)();
		}
	}
	//------------------------------------------------------------
	void VehicleDriver::OnGoStopRelease()
	{
		(*throttleReleaseDel)();
		(*brakeReleaseDel)();
	}
	//------------------------------------------------------------
	void VehicleDriver::OnStopPress()
	{
		if( speedometer->GetSpeedMS() > 0.1f )
		{
			//brake
			(*brakePressDel)();
		}
		else
		{
			//accel backward
			if( !gearbox->IsReverse() )
				gearbox->ShiftToReverse();

			(*throttlePressDel)();
		}
	}
	//------------------------------------------------------------
	//SimpleControls----------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//iThrottleControl--------------------------------------------
	void VehicleDriver::OnThrottlePressEngineOnAssistOn()
	{
		//Флаг
		userThrottlePress = true;
		//Включаем делегаты
		ActivateThrottleRpmDels();
		radPSController->ReCallEntries();
	}
	//------------------------------------------------------------
	void VehicleDriver::OnThrottlePressEngineOnAssistOff()
	{
		userThrottlePress = true;
		iThrottleControl->Press();
	}
	//------------------------------------------------------------
	void VehicleDriver::OnThrottlePressEngineOff()
	{
		iThrottleControl->Press();
		iMotorControl->SetStarter();
	}
	//------------------------------------------------------------
	void VehicleDriver::OnThrottleReleaseEngineOnAssistOn()
	{
		//Флаг
		userThrottlePress = false;
		//Отключаем делегаты
		ActivateThrottleRpmDels( false );
		//Отпускаем педаль
		iThrottleControl->Release();
	}
	//------------------------------------------------------------
	void VehicleDriver::OnThrottleReleaseEngineOnAssistOff()
	{
		userThrottlePress = false;
		iThrottleControl->Release();
	}
	//------------------------------------------------------------
	void VehicleDriver::OnThrottleReleaseEngineOff()
	{
		iThrottleControl->Release();
		iMotorControl->SetStarter( false );
	}
	//------------------------------------------------------------
	//Обороты в Норме
	//вызов OnThrottleRpmNorm() происходит только при нажатой педали газа
	void VehicleDriver::OnThrottleRpmNorm()
	{
		iThrottleControl->Press();
	}
	//------------------------------------------------------------
	//Обороты выше нормы.
	//вызов OnThrottleRpmAboveNorm() происходит только при нажатой педали газа
	void VehicleDriver::OnThrottleRpmAboveNorm()
	{
		//Производим замер скорости изменения оборотов
		radPSController->measureSpeedIncrease( 1.0f, throttleRpmSpeedIncDel );
	}
	//------------------------------------------------------------
	//Обороты выше максимума
	//вызов OnThrottleRpmAboveMax() происходит только при нажатой педали газа
	void VehicleDriver::OnThrottleRpmAboveMax()
	{
		//Срочно отпускаем педаль
		iThrottleControl->Release();
	}
	//------------------------------------------------------------
	//Скорость выше 0. Обороты растут
	void VehicleDriver::OnThrottleRpmSpeedInc()
	{
		switch( radPSController->getZone() )
		{
		case MotorChartRadPSController::ZONE_MAX:
			iThrottleControl->Press( Core::puFloat( iThrottleState->GetCurrentValue() * 0.5f ) );
			radPSController->measureSpeedIncrease( 2.0f, throttleRpmSpeedIncDel );
			break;
		};
	}
	//------------------------------------------------------------
	void VehicleDriver::ActivateThrottleInputDels( bool activate )
	{
		throttlePressDel->isActive = activate;
		throttleReleaseDel->isActive = activate;
	}
	//------------------------------------------------------------
	void VehicleDriver::ActivateThrottleRpmDels( bool activate )
	{
		throttleRpmNormDel->isActive = activate;
		throttleRpmAboveNormDel->isActive = activate;
		throttleRpmAboveMaxDel->isActive = activate;
		throttleRpmSpeedIncDel->isActive = activate;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//iThrottleControl----------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//clutch------------------------------------------------------
	//Нажатие Clutch на клавиатуре пользователем
	void VehicleDriver::OnClutchPress()
	{
		userClutchPress = true;
		iClutchControl->Press();
	}
	//------------------------------------------------------------
	//Пользователь отпустил Clutch на клавиатуре
	//Также включена помощь ClutchAssist
	void VehicleDriver::OnClutchReleaseAssistOn()
	{
		userClutchPress = false;
		radPSController->ReCallEntries();
	}
	//------------------------------------------------------------
	//Пользователь отпустил Clutch на клавиатуре
	//помощь ClutchAssist отключена
	void VehicleDriver::OnClutchReleaseAssistOff()
	{
		userClutchPress = false;
		iClutchControl->Release();
	}
	//------------------------------------------------------------
	//Функция работает при включенной помощи ClutchAssist
	//Обороты двигателя ниже мин. отметки
	void VehicleDriver::OnClutchRpmBelowMin()
	{
		//Нажимаем педаль
		iClutchControl->Press();

		//старая логика
		//Возможно до этого был заказ на замер скорости
		//Нужно отменить вызов
		//clutchRpmSpeedDecDel->SetActive( false );

		//новая логика
		clutchSmPressZoneTimer.Stop();
	}
	//------------------------------------------------------------
	/*
	//Функция работает при включенной помощи ClutchAssist
	//( Временно отключено слежение Clucth за высокими оборотами )
	//Обороты двигателя выше макс. отметки
	void VehicleDriver::OnClutchRpmAboveMax()
	{
		if( !userThrottlePress )
		{
			radPSController->measureSpeedIncrease( clutchRpmSpeedIncDel, 0.5f );
			clutchRpmSpeedIncDel->SetActive( true );
			return;
		}
	}
	*/
	//------------------------------------------------------------
	//Функция работает при включенной помощи ClutchAssist
	//Обороты двигателя спустились ниже отметки NormLowMark
	void VehicleDriver::OnClutchRpmBelowNorm()
	{
		//старая логика
		//Замерим скорость изменения оборотов
		//radPSController->measureSpeedDecrease( clutchRpmSpeedDecDel );
		//clutchRpmSpeedDecDel->SetActive( true );

		//новая логика
		if( !clutchSmPressZoneTimer.IsWorking() )
			clutchSmPressZoneTimer.Reset( driverTimingCalc->GetRpmSpeedReactionTime() );
	}
	//------------------------------------------------------------
	//Функция работает при включенной помощи ClutchAssist
	//Обороты двигателя поднялись выше отметки NormLowMark
	void VehicleDriver::OnClutchRpmNorm()
	{
		//Проверяем нажатие пользователем
		if( !userClutchPress )
		{
			//Отпускаем сцепление
			iClutchControl->Release();
		}

		//новая логика
		clutchSmPressZoneTimer.Stop();
	}
	//------------------------------------------------------------
	/*
	//Функция работает при включенной помощи ClutchAssist
	//( Временно отключено слежение Clucth за высокими оборотами )
	//Обороты двигателя растут
	void VehicleDriver::OnClutchRpmSpeedInc()
	{
		//Находятся ли обороты в макс. зоне
		if( radPSController->getZone() == MotorChartRadPSController::ZONE_ABOVE_MAX )
		{
			//Нажимаем сцепление
			iClutchControl->Press();
		}
	}
	*/
	//------------------------------------------------------------
	/*
	//Функция работает при включенной помощи ClutchAssist
	//Обороты двигателя падают
	void VehicleDriver::OnClutchRpmSpeedDec()
	{
		//Находятся ли обороты в мин. зоне
		if( radPSController->getZone() == MotorChartRadPSController::ZONE_MIN )
		{
			//Чуть нажимаем сцепление
			iClutchControl->Press( Core::puFloat(iClutchState->GetCurrentValue() + 0.1f) );
			//Снова мерим скорость
			radPSController->measureSpeedDecrease( clutchRpmSpeedDecDel );
			return;
		}
		//отключаем доставку замера скорости
		clutchRpmSpeedDecDel->SetActive( false );
	}
	*/
	//------------------------------------------------------------
	void VehicleDriver::ActivateClutchInputDels( bool activate )
	{
		if( (driveOptions & DO_CLUTCH_AXIS) != 0 )
			clutchAxisDel->isActive = activate;
		clutchPressDel->isActive = activate;
		clutchReleaseDel->isActive = activate;
	}
	//------------------------------------------------------------
	void VehicleDriver::ActivateClutchRpmDels( bool activate )
	{
		clutchRpmBelowMinDel->isActive = activate;
		clutchRpmBelowNormDel->isActive = activate;
		clutchRpmNormDel->isActive = activate;

		//старая логика
		//clutchRpmSpeedDecDel->SetActive( activate );

		//новая логика
		clutchSmPressZoneTimerDel->isActive = activate;

		/*
		Временно отключены
		clutchRpmAboveMaxDel->SetActive( activate );
		clutchRpmSpeedIncDel->SetActive( activate );
		*/
	}
	//------------------------------------------------------------
	void VehicleDriver::OnClutchSmPressZoneTimer()
	{
		if( !userClutchPress )
		{
			Core::puFloat rangePos = radPSController->getMinZone()->GetRangePos();
			iClutchControl->Press( Core::puFloat( 1 - rangePos.get() ) );
		}
		clutchSmPressZoneTimer.Reset( driverTimingCalc->GetRpmSpeedReactionTime() );
	}
	//------------------------------------------------------------
	void VehicleDriver::OnClutchAxisMoved()
	{
	}
	//clutch------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//brake-------------------------------------------------------
	void VehicleDriver::OnBrakePress()
	{
		userBrakePress = true;
		iBrakeControl->Press( Core::puFloat(1.0f) );
	}
	//------------------------------------------------------------
	void VehicleDriver::OnBrakeRelease()
	{
		userBrakePress = false;
		iBrakeControl->Release();
	}
	//brake-------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//steer-------------------------------------------------------
	void VehicleDriver::OnSteerAxisMoved()
	{
	}
	//------------------------------------------------------------
	void VehicleDriver::OnSteerPressLeft()
	{
		userSteerPress = true;
		iSteerControl->PressLeft();
	}
	//------------------------------------------------------------
	void VehicleDriver::OnSteerPressRight()
	{
		userSteerPress = true;
		iSteerControl->PressRight();
	}
	//------------------------------------------------------------
	void VehicleDriver::OnSteerRelease()
	{
		userSteerPress = false;
		iSteerControl->Release();
	}
	//steer-------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//gearbox-----------------------------------------------------
	//------------------------------------------------------------
	//Срабатывает только при ClutchAssist
	void VehicleDriver::OnShiftStarted()
	{
		ActivateClutchRpmDels( false );
		if( (driveOptions & DO_USER_CLUTCH) != 0 )
			ActivateClutchInputDels( false );
	}
	//------------------------------------------------------------
	//Срабатывает только при ClutchAssist
	void VehicleDriver::OnShiftEnded()
	{
		if( (driveOptions & DO_USER_CLUTCH) != 0 )
			ActivateClutchInputDels();
		ActivateClutchRpmDels();
		radPSController->ReCallEntries();
	}
	//------------------------------------------------------------
	//Срабатывает только при отключенном ShiftAssist
	void VehicleDriver::OnShiftUpPress()
	{
		gearbox->ShiftUp();
	}
	//------------------------------------------------------------
	//Срабатывает только при отключенном ShiftAssist
	void VehicleDriver::OnShiftDownPress()
	{
		gearbox->ShiftDown();
	}
	//------------------------------------------------------------
	//gearbox-----------------------------------------------------
	//------------------------------------------------------------
	//motor-------------------------------------------------------
	//------------------------------------------------------------
	void VehicleDriver::OnMotorStall()
	{
		if( (driveOptions & DO_USER_CONTROLS) != 0 )
		{
			(driveOptions & DO_THROTTLE_ASSIST) != 0 ? OnThrottleReleaseEngineOnAssistOn() : OnThrottleReleaseEngineOnAssistOff();
			throttlePressDel->bind( this, &VehicleDriver::OnThrottlePressEngineOff );
			throttleReleaseDel->bind( this, &VehicleDriver::OnThrottleReleaseEngineOff );
		}
	}
	//------------------------------------------------------------
	void VehicleDriver::OnMotorStart()
	{
		if( (driveOptions & DO_USER_CONTROLS) != 0 )
		{
			OnThrottleReleaseEngineOff();
			throttlePressDel->bind( this, (driveOptions & DO_THROTTLE_ASSIST) != 0 ? &VehicleDriver::OnThrottlePressEngineOnAssistOn : &VehicleDriver::OnThrottlePressEngineOnAssistOff );
			throttleReleaseDel->bind( this, (driveOptions & DO_THROTTLE_ASSIST) != 0 ? &VehicleDriver::OnThrottleReleaseEngineOnAssistOn : &VehicleDriver::OnThrottleReleaseEngineOnAssistOff );
		}
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
}//namespace Vehicle
}//namespace Demo
#endif //DBS_LM_ABSTRACT_VEHICLE
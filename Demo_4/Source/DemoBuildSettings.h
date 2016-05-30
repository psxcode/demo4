#pragma once

//*****************************************************
// Options -------------------------------------------

// Rendering -------------------------------------------
/**
Materials will use Default Scheme
Render will use Fixed Function Pipeline D3D9 mode
*/
#define DBS_RS_OGRE_FORWARD_RENDERING	0
/**
Material will be converted with Material generators
*/
#define DBS_RS_DEFERRED_RENDERING		2
//------------------------------------------------------------

// Physics ---------------------------------------------------
#define DBS_PS_NO_PHYSICS	0
#define DBS_PS_PHYSX		2
//------------------------------------------------------------

// NXU Filesystem --------------------------------------------
#define DBS_NXU_FILESYSTEM_NATIVE	1
#define DBS_NXU_FILESYSTEM_OGRE		2
//------------------------------------------------------------

// String type -----------------------------------------------
#define DBS_STRING_TYPE_STL		1
#define DBS_STRING_TYPE_OGRE	2
//------------------------------------------------------------

// String compare --------------------------------------------
#define DBS_STRING_COMPARE_OPERATOR		0
#define DBS_STRING_COMPARE_BOOST		2
//------------------------------------------------------------

// SharedPtr type --------------------------------------------
#define DBS_SHARED_PTR_STL		1
#define DBS_SHARED_PTR_OGRE		2
#define DBS_SHARED_PTR_BOOST	3
//------------------------------------------------------------

// Delegate type ---------------------------------------------
#define DBS_DELEGATE_TYPE_DEMO			1
#define DBS_DELEGATE_TYPE_DONS_FAST		2 //by Don Clugston
//------------------------------------------------------------

// Options -------------------------------------------
//****************************************************


//****************************************************
// Pre Config ----------------------------------------

//render
#define DBS_RENDER_SYSTEM		DBS_RS_DEFERRED_RENDERING

//physics
#define DBS_PHYSICS_SYSTEM		DBS_PS_NO_PHYSICS

//NXU
#define DBS_NXU_FILESYSTEM		DBS_NXU_FILESYSTEM_OGRE

//string
#define DBS_STRING_TYPE			DBS_STRING_TYPE_OGRE

//string compare
#define DBS_STRING_COMPARE_FUNC	DBS_STRING_COMPARE_BOOST

//shared_ptr
#define DBS_SHARED_PTR_TYPE		DBS_SHARED_PTR_OGRE

//delegate
#define DBS_DELEGATE_TYPE		DBS_DELEGATE_TYPE_DONS_FAST

// Pre Config -----------------------------------------
//*****************************************************



//*****************************************************
// LOGIC MODULES 
/*
Material Generators
Lights
Compositors
Deferred Composition Pass
*/
#define DBS_LM_DEFERRED_RENDERING				DBS_RENDER_SYSTEM == DBS_RS_DEFERRED_RENDERING

/*
Mechanics -- Motor -- Gearbox -- Differential -- Flywheel
BaseVehicle
VehicleDriver
*/
#define DBS_LM_ABSTRACT_VEHICLE					0
#define DBS_LM_VEHICLE_DRIVER					0 && DBS_LM_ABSTRACT_VEHICLE

/*
Script Managers
*/
#define DBS_LM_SCRIPT							1
#define DBS_LM_SCRIPT_TIXML						1 && DBS_LM_SCRIPT
#define DBS_LM_SCRIPT_OGREMAX_TIXML				1 && DBS_LM_SCRIPT_TIXML

/*
Core Collections
Ogre Collections
*/
#define DBS_LM_COLLECTIONS						1

/*
Abstact Vehicle Params Collections
*/
#define DBS_LM_ABSTRACT_VEHICLE_COLLECTIONS		DBS_LM_ABSTRACT_VEHICLE && DBS_LM_COLLECTIONS

/*
PhysXController
*/
#define DBS_LM_PHYSX							DBS_PHYSICS_SYSTEM == DBS_PS_PHYSX

/*
PhysXVehicle over AbstactVehicle
*/
#define DBS_LM_PHYSX_VEHICLE					DBS_LM_ABSTRACT_VEHICLE && DBS_LM_PHYSX

/*
PhysX Params Collections
*/
#define DBS_LM_PHYSX_COLLECTIONS				DBS_LM_PHYSX && DBS_LM_COLLECTIONS

/*
PhysXVehicleCollections
*/
#define DBS_LM_PHYSX_VEHICLE_COLLECTIONS		DBS_LM_PHYSX_VEHICLE && DBS_LM_PHYSX_COLLECTIONS

/*
PhysX NXU Classes
*/
#define DBS_LM_NXU								1 && DBS_LM_PHYSX

/*
PhysX NXU Collections
*/
#define DBS_LM_NXU_COLLECTIONS					DBS_LM_NXU && DBS_LM_PHYSX_COLLECTIONS

/*
Archive
*/
#define DBS_LM_ARCHIVE							1

/*
Additionsl objects, not part of the Engine
*/
#define DBS_LM_GAME_OBJECTS						1

// LOGIC MODULES 
//*****************************************************



//****************************************************
// Post Config ---------------------------------------

#if defined DEMO_STATIC_BUILD || defined DEMO_EXE_BUILD
#	define _DemoExport
#else
#	if defined DEMO_NONCLIENT_BUILD
#		define _DemoExport __declspec( dllexport )
#	else
#		define _DemoExport __declspec( dllimport )
#	endif
#endif

#if DBS_LM_SCRIPT_TIXML
#	define TIXML_USE_STL
#endif

#define DBS_NXU_COLLADA							0 && DBS_LM_NXU

#define DBS_USE_OGRE_CONTAINERS					1

#define DBS_LOGGING_ENABLED						1

#define DBS_ASSERTION_ENABLED					1

// Post Config ---------------------------------------
//****************************************************
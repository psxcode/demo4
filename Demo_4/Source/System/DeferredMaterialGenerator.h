#pragma once
#include "System\MaterialGenerator.h"

namespace Demo
{
namespace System
{
	//------------------------------------------------------------
	/** Class for generating materials for objects to render themselves to the GBuffer
	 *  @note This does not support all the possible rendering techniques out there.
	 *  in order to support more, either expand this class or make sure that objects
	 *  that will not get treated correctly will not have materials generated for them.
	 */
	/*
			  Possible Texture Samplers

	                 PF_R8G8B8A8
	   ________________________________________
	S0 |   D.r   |   D.g   |   D.b   | alpha  |   ///Diffuse Map
	   ________________________________________
    S1 |   N.x   |   N.y   |   N.z   |        |   ///Normal Map
	   ________________________________________
    S2 |spec int |spec pow |refl int |emi int |   ///Effects Map -- Spec Intensity | Spec Power | Refl Intensity | Emissive Intensity

	               PF_FLOAT16_RGBA
	   ________________________________________
    S4 |  Ref.r  |  Ref.g  |  Ref.b  |        |   ///Reflection CubeMap
	   ________________________________________
	S5 |  Ref.r  |  Ref.g  |  Ref.b  |        |   ///Blurred Reflection CubeMap
	   ________________________________________
    S6 |  IBL.r  |  IBL.g  |  IBL.b  |        |   ///Image-based Lighting CubeMap
	*/

	/*
			  Possible GBuffrer Layout

				   PF_FLOAT16_RGBA
	   _________________________________________
   RT0 |   N.x   |   N.y   |   N.z   |  depth  |   /// Normal xyz | depth
	   _________________________________________
   RT1 |m.ID/D.r | D.g/D.b |sp.p/sp.i|         |   /// Material ID / Diffuse.r | Diffuse gb | Spec. Power / Spec. Intensity
	   _________________________________________
   RT2 |  Ref.r  |  Ref.g  |  Ref.b  |  Mo.x   |   /// Reflections rgb | Motion x
       _________________________________________
   RT3 |   L.r   |   L.g   |   L.b   |  Mo.y   |   /// Lighting rgb | Motion y


				    PF_R8G8B8A8
	   _________________________________________
   RT0 |     Normal.x      |     Normal.y      |   /// Normal xy ( z is encoded )
       _________________________________________
   RT1 |   D.r   |   D.g   |   D.b   |sp.p/sp.i|   /// Diffuse rgb | Spec.Power / Spec.Intensity
       _________________________________________
   RT2 |       depth       |  Ref.r  |  R.exp  |   /// Depth 16bit | Reflections luminance | Reflection exponent
       _________________________________________
   RT3 |   L.r   |   L.g   |   L.b   |  L.exp  |   /// Lighting rgb | Lighting exponent

	*/
	//------------------------------------------------------------
	class DeferredGeometryMaterialGenerator : public MaterialGenerator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DeferredGeometryMaterialGenerator)
		DEFINE_TAGGED
		//--------------------------------//
		enum SamplerPermutation
		{
			SMP_NOT_DEFINED					= 0x0,

			//Special Names
			SMP_DETAIL						= 0x01000000,

			SMP_SPECIAL_MASK				= 0xFF000000,

			//Main Samplers
			SMP_DIFFUSE						= 0x00000001,
			SMP_NORMAL						= 0x00000002,
			SMP_EFFECTS						= 0x00000004,
			SMP_REFLECTION					= 0x00000100,
			SMP_IBL							= 0x00000200,

			SMP_2D_SAMPLER_MASK				= 0x00000007 | SMP_DETAIL,
			SMP_CUBE_SAMPLER_MASK			= 0x00000300,
			SMP_LDR_SAMPLER_MASK			= 0x000000FF | SMP_DETAIL,
			SMP_HDR_SAMPLER_MASK			= 0x0000FF00,
			SMP_MAIN_SAMPLER_MASK			= 0x0000FFFF | SMP_DETAIL,

			//Effects Dummy Samplers
			SMP_SPECINT						= 0x00010000 | SMP_EFFECTS,
			SMP_SPECPOW						= 0x00020000 | SMP_EFFECTS,
			SMP_EMISSIVE					= 0x00040000 | SMP_EFFECTS,
			SMP_REFLINT						= 0x00080000 | SMP_EFFECTS,

			SMP_EFFECTS_SAMPLER_MASK		= 0x000F0000 | SMP_EFFECTS | SMP_DETAIL
		};
		//--------------------------------//
		enum GeometryMaterialPermutation
		{
			DFR_ONE_TEXCOORD =		0x00000001,
			DFR_TWO_TEXCOORDS =		0x00000002,
			DFR_TEXCOORDS_AMT =		0x00000003,   /// TexCoords Amount Mask
			DFR_TEXCOORD_MASK =		0x00000003,   /// TexCoords Section Mask

			DFR_ONE_DIFFUSE_MAP =	0x00000010,
			DFR_TWO_DIFFUSE_MAPS =	0x00000020,
			DFR_DIFFUSE_MAPS_AMT =	0x00000030,   /// Diffuse Maps Amount Mask
			DFR_HAS_DIFFUSE_COLOR =	0x00000040,   /// Has Diffuse Color ( finalValue = color * mapValue )
			DFR_HAS_ALPHA =			0x00000080,   /// Has Alpha ( discard pixel )
			DFR_DIFFUSE_MASK =		0x000000F0,   /// Diffuse Section Mask

			DFR_ONE_NORMAL_MAP =	0x00000100,
			DFR_TWO_NORMAL_MAPS =	0x00000200,
			DFR_NORMAL_MAPS_AMT =	0x00000300,   /// Normal Maps Amount Mask
			DFR_HAS_VERTEX_NORMALS=	0x00000400,   /// Material will use vertex normals
			DFR_NORMAL_MASK =		0x00000700,   /// Normal Map Section Mask

			DFR_ONE_SPECINT_MAP =	0x00001000,
			DFR_TWO_SPECINT_MAPS =	0x00002000,
			DFR_SPECINT_MAPS_AMT =	0x00003000,   /// Spec Intensity Maps Amount Mask
			DFR_HAS_SPECINT_CONST=	0x00004000,   /// Has Spec Intensity Constant ( finalValue = constant * mapValue )
			DFR_SPECINT_MASK =		0x00007000,   /// Spec Intensity Section Mask

			DFR_ONE_SPECPOW_MAP =	0x00010000,
			DFR_TWO_SPECPOW_MAPS =	0x00020000,
			DFR_SPECPOW_MAPS_AMT =	0x00030000,   /// Spec Power Maps Amount
			DFR_HAS_SPECPOW_CONST =	0x00040000,   /// Has Spec Power Constant ( finalValue = constant * mapValue )
			DFR_SPECPOW_MASK =		0x00070000,   /// Spec Power Section Mask

			DFR_SPECULAR_MASK =		DFR_SPECINT_MASK | DFR_SPECPOW_MASK,

			DFR_ONE_REFL_INT_MAP =	0x00100000,
			DFR_TWO_REFL_INT_MAPS =	0x00200000,
			DFR_REFL_INT_MAPS_AMT =	0x00300000,   /// Reflection Intensity Maps Amount Mask
			DFR_HAS_REFL_COLOR =	0x00400000,   /// Has Reflection tint color ( finalValue = color * mapValue )
			DFR_HAS_REFL_BLUR_MAP = 0x00800000,	  /// Additional Reflection Cube Map for blurred reflections
			DFR_REFL_MASK =			0x00F00000,   /// Reflection Section Mask

			DFR_ONE_EMISSIVE_MAP =	0x01000000,   /// Has Emissive Map
			DFR_TWO_EMISSIVE_MAPS =	0x02000000,   ///
			DFR_EMISSIVE_MAPS_AMT =	0x03000000,   ///
			DFR_HAS_EMISSIVE_COLOR=	0x04000000,   ///
			DFR_EMISSIVE_MASK =		0x07000000,   /// Lighting Section Mask

			DFR_EFFECTS_MAPS_AMT =	DFR_SPECINT_MAPS_AMT |
									DFR_SPECPOW_MAPS_AMT |
									DFR_REFL_INT_MAPS_AMT |
									DFR_EMISSIVE_MAPS_AMT,

			// GBuffer Layout
			DFR_GBLAYOUT_01 =		0x10000000,
			DFR_GBLAYOUT_02 =		0x20000000,
			DFR_GBLAYOUT_03 =		0x40000000,
			DFR_GBLAYOUT_MASK =		0x70000000,

			DFR_VERTEX_PROGRAM_MASK =					DFR_TEXCOORD_MASK |
														DFR_NORMAL_MASK,

			DFR_FRAGMENT_PROGRAM_MASK =					DFR_TEXCOORD_MASK |
														DFR_DIFFUSE_MASK |
														DFR_NORMAL_MASK |
														DFR_SPECULAR_MASK |
														DFR_REFL_MASK |
														DFR_EMISSIVE_MASK |
														DFR_GBLAYOUT_MASK,

			DFR_TEMPLATE_MATERIAL_MASK =				DFR_DIFFUSE_MAPS_AMT |
														DFR_NORMAL_MAPS_AMT |
														DFR_EFFECTS_MAPS_AMT,
		
			#pragma region diffuse operations
			/*--------------------------------
			Possible Diffuse Operations
			Diffuse pixel component is written to Diffuse storage(LDR)
			*/

			//No Diffuse
			//The output will be black
			DFR_DIFFOP_NO_DIFFUSE =						0x0,

			//Only Diffuse Color
			//DiffuseColor will be supplied as shader parameter by OGRE
			DFR_DIFFOP_DIFFCOL =						DFR_HAS_DIFFUSE_COLOR,

			//Using ordinary TextureMap to color object
			DFR_DIFFOP_DIFFMAP =						DFR_ONE_DIFFUSE_MAP | DFR_ONE_TEXCOORD,

			//Using Texture with Alpha channel
			DFR_DIFFOP_DIFFMAPxALPHA =					DFR_DIFFOP_DIFFMAP | DFR_HAS_ALPHA,

			//Using TextureMap with DiffuseColor
			//This may be used to tint or fade texture in runtime
			DFR_DIFFOP_DIFFCOLxMAP =					DFR_DIFFOP_DIFFCOL | DFR_DIFFOP_DIFFMAP,

			//Using TextureMap with alpha channel and DiffuseColor
			DFR_DIFFOP_DIFFCOLxMAPxALPHA =				DFR_DIFFOP_DIFFCOL | DFR_DIFFOP_DIFFMAPxALPHA,

			DFR_DIFFOP_MASK =							DFR_DIFFUSE_MASK | DFR_TEXCOORD_MASK,
			//--------------------------------
			#pragma endregion

			#pragma region normal operations
			/*--------------------------------
			Possible Normal Operations
			Normal pixel component is written to Normal Buffer(LDR)
			*/

			//All Normals will be facing the camera
			DFR_NORMOP_NO_NORMALS =						0x0,

			//Vertex Normals will be used in calculations
			DFR_NORMOP_VERTEX =							DFR_HAS_VERTEX_NORMALS,

			//NormalMap will be used to adjust normals
			DFR_NORMOP_MAP =							DFR_ONE_NORMAL_MAP | DFR_ONE_TEXCOORD,

			//Vertex Normals will be used to define normals
			//then the Map will rotate them
			DFR_NORMOP_VERTEXxMAP =						DFR_NORMOP_VERTEX | DFR_NORMOP_MAP,

			//All Operation included
			DFR_NORMOP_MASK =							DFR_NORMOP_VERTEXxMAP,
			//--------------------------------
			#pragma endregion

			#pragma region emissive operations
			/*--------------------------------
			Possible Emissive Operations
			Emissive pixel component is written to Light buffer(HDR) separately from Diffuse(LDR) pixel component.
			Then lights are calculated and additively blended to the Light buffer.
			*/

			//Emissive is off
			//No emissive operation performed
			DFR_EMIOP_NO_EMISSIVE =						0x0,

			//Emissive color [0,HDR)
			//This is multiplier and tinter.
			//Emissive operation writes this color to Light buffer as is.
			DFR_EMIOP_EMICOL =						DFR_HAS_EMISSIVE_COLOR,

			//Emissive Mask Map [0,1]
			//This could be used to mask out non-glowing areas.
			//Emissive operation writes sampled map value to Light buffer.
			DFR_EMIOP_EMIMAP =						DFR_ONE_EMISSIVE_MAP | DFR_ONE_TEXCOORD,

			//Emissive color and mask
			//Emissive operation multiplies color with sampled value and writes to buffer
			DFR_EMIOP_EMICOLxMAP =					DFR_EMIOP_EMICOL | DFR_EMIOP_EMIMAP,

			//All Operation included
			DFR_EMIOP_MASK =						DFR_EMIOP_EMICOLxMAP,
			//--------------------------------
			#pragma endregion

			#pragma region specular operations
			/*--------------------------------
			Possible Specular Intensity and Power Operations
			Specular intensity and Power are stored in GBuffer until lights are calculated.
			With lights, the pixel Specular Component is calculated, and additively blended to Reflection buffer(HDR)
			*/

			//No speculars for this material
			DFR_SPECOP_NO_SPECULARS =						0x0,

			DFR_SPECINTOP_SINTCON =							DFR_HAS_SPECINT_CONST,
			DFR_SPECINTOP_SINTMAP =							DFR_ONE_SPECINT_MAP | DFR_ONE_TEXCOORD,
			DFR_SPECINTOP_SINTCONxMAP =						DFR_SPECINTOP_SINTCON | DFR_SPECINTOP_SINTMAP,

			DFR_SPECPOWOP_SPOWCON =							DFR_HAS_SPECPOW_CONST,
			DFR_SPECPOWOP_SPOWMAP =							DFR_ONE_SPECPOW_MAP | DFR_ONE_TEXCOORD,
			DFR_SPECPOWOP_SPOWCONxMAP =						DFR_SPECPOWOP_SPOWCON | DFR_SPECPOWOP_SPOWMAP,

			//All Operation included
			DFR_SPECINTOP_MASK =							DFR_SPECINTOP_SINTCONxMAP,
			DFR_SPECPOWOP_MASK =							DFR_SPECPOWOP_SPOWCONxMAP,

			//Specular IntensityConstant [0,1]; Specular PowerConstant [0,1] - is upscaled later to [0,255]
			//SpecularColor parameter is passed to shader from OGRE.
			//Shininess parameter is passed to shader from OGRE.
			//SpecOp writes the SpecularColor.r component to specInt storage(LDR)
			//SpecOp writes the Shininess value to specPow storage(LDR)
			DFR_SPECOP_SINTCON_pow_SPOWCON =				DFR_SPECINTOP_SINTCON | DFR_SPECPOWOP_SPOWCON,

			//Specular IntensityConstant [0,1]; Specular PowerMap sampled value [0,1] - is upscaled later to [0,255]
			//SpecOp writes the SpecularColor.r component to specInt storage(LDR)
			//SpecOp writes the PowerMap sampled value to specPow storage(LDR)
			DFR_SPECOP_SINTCON_pow_SPOWMAP =				DFR_SPECINTOP_SINTCON | DFR_SPECPOWOP_SPOWMAP,

			//Specular IntensityConstant, PowerConstant and PowerMap
			//SpecOp writes the SpecularColor.r component to specInt storage(LDR)
			//SpecOp multiplies Shininess value and the PowerMap sampled value, then writes to specPow storage(LDR)
			DFR_SPECOP_SINTCON_pow_SPOWCONxMAP =			DFR_SPECINTOP_SINTCON | DFR_SPECPOWOP_SPOWCONxMAP,

			//Specular IntensityMap and PowerConstant
			//SpecOp samples SpecIntMap and stores it in specInt storage(LDR)
			//SpecOp writes the Shininess value to specPow storage(LDR)
			DFR_SPECOP_SINTMAP_pow_SPOWCON =				DFR_SPECINTOP_SINTMAP | DFR_SPECPOWOP_SPOWCON,

			//Specular IntensityMap and Power Map
			DFR_SPECOP_SINTMAP_pow_SPOWMAP =				DFR_SPECINTOP_SINTMAP | DFR_SPECPOWOP_SPOWMAP,

			//Specular IntensityMap, PowerConstant and PowerMap
			DFR_SPECOP_SINTMAP_pow_SPOWCONxMAP =			DFR_SPECINTOP_SINTMAP | DFR_SPECPOWOP_SPOWCONxMAP,

			//Specular IntensityConstant, IntensityMap and PowerConstant
			DFR_SPECOP_SINTCONxMAP_pow_SPOWCON =			DFR_SPECINTOP_SINTCONxMAP | DFR_SPECPOWOP_SPOWCON,

			//Specular IntensityConstant, IntensityMap and PowerMap
			DFR_SPECOP_SINTCONxMAP_pow_SPOWMAP =			DFR_SPECINTOP_SINTCONxMAP | DFR_SPECPOWOP_SPOWMAP,

			//Specular IntensityConstant, IntensityMap, PowerConstant and PowerMap
			DFR_SPECOP_SINTCONxMAP_pow_SPOWCONxMAP =		DFR_SPECINTOP_SINTCONxMAP | DFR_SPECPOWOP_SPOWCONxMAP,

			//All Operations included
			DFR_SPECOP_MASK =								DFR_SPECINTOP_MASK | DFR_SPECPOWOP_MASK,
			//--------------------------------
			#pragma endregion

			#pragma region reflection operations
			/*--------------------------------
			Possible Reflection operations
			Reflections are sampled from ReflectionCubeMaps during GeometryPass
			and are stored in Reflection buffer(HDR)
			Note: ReflectionCubeMap is required in all Reflection operation and thus is not presented in flags
			*/

			//No reflections for this material
			DFR_REFLOP_NO_REFLECTIONS =						0x0,

			//Reflection Intensity Constant is used to adjust reflection brightness
			DFR_REFLOP_REFLCOLOR =							DFR_HAS_REFL_COLOR,

			//Reflection Intensity Map is used to adjust reflection brightness
			DFR_REFLOP_REFLINTMAP =							DFR_ONE_REFL_INT_MAP | DFR_ONE_TEXCOORD,

			//Reflecftion Intensity Constant and Map are used to adjust reflection brightness
			DFR_REFLOP_REFLCOLORxMAP =						DFR_REFLOP_REFLCOLOR | DFR_REFLOP_REFLINTMAP,

			//All Operation included
			DFR_REFLOP_MASK =								DFR_REFLOP_REFLCOLORxMAP,
			//--------------------------------
			#pragma endregion

			#pragma region effects operations
			/*--------------------------------
			One effects texture contains 4 channel data:
			Specular Intensity
			Specular Power
			Reflection Intensity
			Emissive Mask
			*/

			//No effects textures needed
			DFR_EFFOP_NO_EFFECTS =							0x0,

			DFR_EFFOP_SPECINT =								DFR_SPECINTOP_SINTMAP,
			DFR_EFFOP_SPECPOW =								DFR_SPECPOWOP_SPOWMAP,
			DFR_EFFOP_REFLINT =								DFR_REFLOP_REFLINTMAP,
			DFR_EFFOP_EMI =									DFR_EMIOP_EMIMAP,

			//All Operations included
			DFR_EFFOP_MASK =								DFR_SPECINTOP_SINTMAP |
															DFR_SPECPOWOP_SPOWMAP |
															DFR_REFLOP_REFLINTMAP |
															DFR_EMIOP_EMIMAP,
			//--------------------------------
			#pragma endregion

			DFR_ALL_MASK =									0xFFFFFFFF
		};
		//--------------------------------//
		struct InitData : public MaterialGenerator::InitData
		{
			InitData() {}
			InitData(string_traits::in convertName, string_traits::in createName)
				: mConvertSchemeName(convertName) , mCreateSchemeName(createName)
			{
			}
			virtual bool isValid()const { return MaterialGenerator::InitData::isValid() &&
					!mConvertSchemeName.empty() && !mCreateSchemeName.empty(); }

			string_traits::str mConvertSchemeName;
			string_traits::str mCreateSchemeName;
		};
		//--------------------------------//
		typedef Ogre::uint32 SamplerPerm;
		//--------------------------------//
		static DeferredGeometryMaterialGenerator::shared_ptr getInstance( InitData const &idata );
		static SamplerPerm getSamplerPerm( string_traits::in samplerAliasName );
		static void removeDummyTUS(Ogre::Pass*);
		static perm_type getPassPermutation(Ogre::Pass*);
		//--------------------------------//
		virtual ~DeferredGeometryMaterialGenerator();

		//Эта функция просматривает материал и модифицирует его
		//опираясь на специальные имена техник.
		//Возвращает true если материал был модифицирован.
		bool modifyMaterial( Ogre::Material *mat );
		void setGBufferLayout(MaterialGenerator::perm_type layoutPerm) {gbufferLayout = layoutPerm;}
		string_traits::outref getConvertSchemeName()const {return mConvertSchemeName;}
		string_traits::outref getCreateSchemeName()const {return mCreateSchemeName;}

	protected:
		virtual Ogre::GpuProgramPtr generateVertexProgram(perm_type permutation);
		virtual Ogre::GpuProgramPtr generateFragmentProgram(perm_type permutation);
		virtual Ogre::MaterialPtr generateTemplateMaterial(perm_type permutation);

		//construct by getInstance()
		DeferredGeometryMaterialGenerator();

		bool initialize( InitData const &idata );
		void modifyTechnique( Ogre::Technique *tech );
		string_traits::str getPPDefines( perm_type permutation );

		string_traits::str mConvertSchemeName;
		string_traits::str mCreateSchemeName;
		string_traits::str mUtilsSource;

		MaterialGenerator::perm_type gbufferLayout;

	private:
		DeferredGeometryMaterialGenerator( const DeferredGeometryMaterialGenerator& );
		DeferredGeometryMaterialGenerator& operator=( const DeferredGeometryMaterialGenerator& );
	};
	//------------------------------------------------------------
	class DeferredLightMaterialGenerator: public MaterialGenerator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DeferredLightMaterialGenerator)
		DEFINE_TAGGED
		//--------------------------------//
		enum LightMaterialPermutation
		{
			LIT_TYPE_SPOT				= 0x00000002,
			LIT_TYPE_POINT				= 0x00000001,
			LIT_TYPE_DIRECT				= 0x00000004,
			LIT_TYPE_MASK				= 0x00000007,

			LIT_HAS_SPECULAR			= 0x00000010,
			LIT_HAS_SHADOW				= 0x00000020,

			/* Vertex program has two permutations
			 * Quad or LightGeometry
			 */
			LIT_VERTEX_PROGRAM_MASK			= LIT_TYPE_DIRECT,

			/* Fragment program is affected by every permutation parameter
			 */
			LIT_FRAGMENT_PROGRAM_MASK		= 0x00000037,

			/* Template material has 4 permutations
			 * and is affected by light type (Quad or Geometry)
			 * and has/no shadow
			 **/
			LIT_TEMPLATE_MATERIAL_MASK		= LIT_TYPE_DIRECT | LIT_HAS_SHADOW
		};
		//--------------------------------//
		static DeferredLightMaterialGenerator::shared_ptr getInstance(InitData const &);
		static perm_type getPermutationLod( perm_type permutation );
		//--------------------------------//
		virtual ~DeferredLightMaterialGenerator();

	protected:
		virtual Ogre::GpuProgramPtr generateVertexProgram(perm_type permutation);
		virtual Ogre::GpuProgramPtr generateFragmentProgram(perm_type permutation);
		virtual Ogre::MaterialPtr generateTemplateMaterial(perm_type permutation);

		//construct by getInstance
		DeferredLightMaterialGenerator();

		bool initialize(InitData const &);

		string_traits::str getPPDefines( perm_type permutation );
		void setUpBaseParameters( const Ogre::GpuProgramParametersSharedPtr& params );

		string_traits::str mMasterSource;
	};
	//------------------------------------------------------------
	class DeferredCompositorMaterialGenerator : public MaterialGenerator
	{
	public:
		DEFINE_CLASS_SHARED_PTR(DeferredCompositorMaterialGenerator)
		DEFINE_TAGGED
		//--------------------------------//
		enum CompositorMaterialPermutation
		{
			// GBuffer Layout (matches with geometry permutation)
			DFR_GBLAYOUT_01 =		0x10000000,
			DFR_GBLAYOUT_02 =		0x20000000,
			DFR_GBLAYOUT_03 =		0x40000000,
			DFR_GBLAYOUT_MASK =		0x70000000,

			DFR_SEPARATE_LIGHT_REFL_TEX =	0x00000001,

			// Permutation does not affect vertex program
			DFR_VERTEX_PROGRAM_MASK =		0,

			DFR_FRAGMENT_PROGRAM_MASK =		DFR_GBLAYOUT_MASK |
											DFR_SEPARATE_LIGHT_REFL_TEX,

			DFR_TEMPLATE_MATERIAL_MASK =	DFR_SEPARATE_LIGHT_REFL_TEX
		};
		//--------------------------------//
		static DeferredCompositorMaterialGenerator::shared_ptr getInstance(InitData const &);
		//--------------------------------//
		virtual ~DeferredCompositorMaterialGenerator();

	protected:
		virtual Ogre::GpuProgramPtr generateVertexProgram(perm_type permutation);
		virtual Ogre::GpuProgramPtr generateFragmentProgram(perm_type permutation);
		virtual Ogre::MaterialPtr generateTemplateMaterial(perm_type permutation);

		DeferredCompositorMaterialGenerator();

		bool initialize(InitData const &);

		string_traits::str getPPDefines( perm_type permutation );
	};
	//------------------------------------------------------------
}
}
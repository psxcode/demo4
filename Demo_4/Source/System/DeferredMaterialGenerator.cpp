#include "DemoStableHeaders.h"
#if DBS_LM_DEFERRED_RENDERING

#include <OgreMaterialManager.h>
#include <OgreGpuProgramManager.h>
#include <OgreStringConverter.h>
#include <OgreHighLevelGpuProgramManager.h>
#include <OgreTechnique.h>
#include "Core\bitop.h"
#include "DeferredMaterialGenerator.h"

namespace Demo
{
namespace System
{

#pragma region utility_functions

	typedef DeferredGeometryMaterialGenerator DGMG;
	/*
	Эти функции-утилиты помогают работать с флагами permutation
	>> ВАЖНО <<: Предполагается что входной параметр permutation ОЧИЩЕН от флага категории
	*/
	//------------------------------------------------------------
	/*static*/ bool hasAlpha( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_HAS_ALPHA) != 0;
	}
	//------------------------------------------------------------
	/*static*/ bool hasDiffuseColor( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_HAS_DIFFUSE_COLOR) != 0;
	}
	//------------------------------------------------------------
	/*static*/ bool hasSpecIntConstant( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_HAS_SPECINT_CONST) != 0;
	}
	//------------------------------------------------------------
	/*static*/ bool hasSpecPowConstant( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_HAS_SPECPOW_CONST) != 0;
	}
	//------------------------------------------------------------
	/*static*/ bool hasReflIntColor( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_HAS_REFL_COLOR) != 0;
	}
	//------------------------------------------------------------
	/*static*/ Ogre::uint32 getTexCoordsAmount( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_TEXCOORDS_AMT);
	}
	//------------------------------------------------------------
	/*static*/ Ogre::uint32 getDiffuseMapsAmount( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_DIFFUSE_MAPS_AMT) >> 4;
	}
	//------------------------------------------------------------
	/*static*/ Ogre::uint32 getNormalMapsAmount( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_NORMAL_MAPS_AMT) >> 8;
	}
	//------------------------------------------------------------
	/*static*/ Ogre::uint32 getSpecIntMapsAmount( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_SPECINT_MAPS_AMT) >> 12;
	}
	//------------------------------------------------------------
	/*static*/ Ogre::uint32 getSpecPowMapsAmount( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_SPECPOW_MAPS_AMT) >> 16;
	}
	//------------------------------------------------------------
	/*static*/ Ogre::uint32 getReflIntMapsAmount( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_REFL_INT_MAPS_AMT) >> 20;
	}
	//------------------------------------------------------------
	/*static*/ Ogre::uint32 getEmissiveMapsAmount( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_EMISSIVE_MAPS_AMT) >> 24;
	}
	//------------------------------------------------------------
	/*static*/ Ogre::uint32 getEffectsMapsAmount( MaterialGenerator::perm_type permutation )
	{
		Ogre::uint32 numSpecIntMaps = getSpecIntMapsAmount( permutation );
		Ogre::uint32 numSpecPowMaps = getSpecPowMapsAmount( permutation );
		Ogre::uint32 numReflIntMaps = getReflIntMapsAmount( permutation );
		Ogre::uint32 numEmissiveMaps = getEmissiveMapsAmount( permutation );

		return
			std::max< Ogre::uint32 >( numSpecIntMaps,
			std::max< Ogre::uint32 >( numSpecPowMaps,
			std::max< Ogre::uint32 >( numReflIntMaps, numEmissiveMaps ) ) );
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	/*static*/ MaterialGenerator::perm_type getDiffuseOperation( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_DIFFOP_MASK);
	}
	//------------------------------------------------------------
	/*static*/ MaterialGenerator::perm_type getSpecIntOperation( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_SPECINTOP_MASK);
	}
	//------------------------------------------------------------
	/*static*/ MaterialGenerator::perm_type getSpecPowOperation( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_SPECPOWOP_MASK);
	}
	//------------------------------------------------------------
	/*static*/ MaterialGenerator::perm_type getSpecularOperation( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_SPECOP_MASK);
	}
	//------------------------------------------------------------
	/*static*/ MaterialGenerator::perm_type getEmissiveOperation( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_EMIOP_MASK);
	}
	//------------------------------------------------------------
	/*static*/ MaterialGenerator::perm_type getNormalOperation( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_NORMOP_MASK);
	}
	//------------------------------------------------------------
	/*static*/ MaterialGenerator::perm_type getEffectsOperation( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_EFFOP_MASK);
	}
	//------------------------------------------------------------
	/*static*/ bool hasSubOperation( MaterialGenerator::perm_type operation, MaterialGenerator::perm_type subOperation )
	{
		return (operation & subOperation) == subOperation;
	}
	//------------------------------------------------------------
	/*static*/ bool hasSpecularOperation( MaterialGenerator::perm_type permutation )
	{
		return (permutation & DGMG::DFR_SPECOP_MASK) != 0;
	}
	//------------------------------------------------------------
	/*static*/ bool hasEffectsOperation( MaterialGenerator::perm_type permutation )
	{
		MaterialGenerator::perm_type effectPerm = getEffectsOperation(permutation);
		return hasSubOperation(effectPerm, DGMG::DFR_EFFOP_SPECINT) ||
			hasSubOperation(effectPerm, DGMG::DFR_EFFOP_SPECPOW) ||
			hasSubOperation(effectPerm, DGMG::DFR_EFFOP_REFLINT) ||
			hasSubOperation(effectPerm, DGMG::DFR_EFFOP_EMI);
	}
	//------------------------------------------------------------
#pragma endregion

#pragma region code_gen_functions

	//Use this directive to control whether you are writing projective (regular) or linear depth.
	#define WRITE_LINEAR_DEPTH
	//------------------------------------------------------------
	/*
	Упаковка двух чисел в одно:
		Первое число - целое без дробной части ( используется явный вызов floor )
		Второе число - нормализованное [0,1] ( преобразуется к [0, 0.999] )

	Результат сохраняется в формате не меньше FLOAT16

	Например SpecPower - целое, SpecInt - [0,1].
			MaterialID - целое, RGB компонента - [0,1].
	*/
	/*static*/ void add_pack_intnorm_16f( Ogre::StringStream &ss )
	{
		ss << "half pack_intnorm_16f( half2 intNorm )" << std::endl;
		ss << "{" << std::endl;
		ss << "intNorm.x = floor( intNorm.x );" << std::endl;
		ss << "return intNorm.x + clamp( intNorm.y, 0.0h, 0.999h );" << std::endl;
		ss << "}" << std::endl;
	}
	/*static*/ void add_unpack_intnorm_16f( Ogre::StringStream &ss )
	{
		ss << "half2 unpack_intnorm_16f( half intNorm )" << std::endl;
		ss << "{" << std::endl;
		ss << "half2 rv;" << std::endl;
		ss << "rv.x = floor( intNorm );" << std::endl;
		ss << "rv.y = intNorm - rv.x;" << std::endl;
		ss << "return rv;" << std::endl;
		ss << "}" << std::endl;
	}
	//------------------------------------------------------------
	/*
	Упаковка двух чисел в одно:
		Оба числа нормализованные [0,1].
		Первое преобразуется к [0, 255.0]
		Второе преобразуется к [0, 0.999]

	Результат сохраняется в формате не меньше FLOAT16

	Например 2 RGB компоненты
	*/
	/*static*/ void add_pack_2norm_16f( Ogre::StringStream &ss )
	{
		ss << "half pack_2norm_16f( half2 normNorm )" << std::endl;
		ss << "{" << std::endl;
		ss << "normNorm.x = normNorm.x * 255.0h;" <<std::endl; //необходимая операция floor выполняется в функции pack_intnorm_16f
		ss << "return pack_intnorm_16f( normNorm );" << std::endl;
		ss << "}" <<std::endl;
	}
	/*static*/ void add_unpack_2norm_16f( Ogre::StringStream &ss )
	{
		ss << "half2 unpack_2norm_16f( half normNorm )" << std::endl;
		ss << "{" << std::endl;
		ss << "half2 rv = unpack_intnorm_16f( normNorm );" << std::endl;
		ss << "rv.x /= 255.0h;" << std::endl; //умножение на инверсию костанты
		ss << "return rv;" << std::endl;
		ss << "}" << std::endl;
	}
	//------------------------------------------------------------
	/*
	Упаковка одного FLOAT16 нормализованного числа
	в две color ячейки ( нормализованные 8bit )

	Результат: 2 color ячейки ( нормализованные 8bit )

	Например: компонента нормали пишется в 2 color 8bit ячейки
	*/
	/*static*/ void add_pack_norm16f_2norm8( Ogre::StringStream &ss )
	{
		ss << "half2 pack_norm16f_2norm8( half pv )" << std::endl;
		ss << "{" << std::endl;
		ss << "pv *= 256.0h;" << std::endl;
		ss << "half floorPV = floor(pv);" << std::endl;
		ss << "return half2( floorPV/256.0h, pv - floorPV );" << std::endl;
		ss << "}" << std::endl;
	}
	/*static*/ void add_unpack_norm16f_2norm8( Ogre::StringStream &ss )
	{
		ss << "half unpack_norm16f_2norm8( half2 upv )" << std::endl;
		ss << "{" << std::endl;
		ss << "const half2 mult( 1.0h, 1.0h/256.0h );" << std::endl;
		ss << "return dot( upv, mult );" << std::endl;
		ss << "}" << std::endl;
	}
	//------------------------------------------------------------
	/*
	Упаковка двух чисел в одно:
		Первое число - целое без дробной части в диапазоне [0, 255.0]( используется явный вызов floor для очистки хвоста )
		Второе число - нормализованное [0,1]

	Результат: 1 color ячейка ( нормализованная 8bit )

	Например: SpecPower - целое, SpecInt - [0,1].
	*/
	/*static*/ void add_pack_int0255norm_norm8( Ogre::StringStream &ss )
	{
		ss << "half pack_int0255norm_norm8( half2 pv )" << std::endl;
		ss << "{" << std::endl;
		ss << "pv.x = floor(pv.x/16.0h);" << std::endl;
		ss << "pv.y = pv.y*15.0h;" << std::endl;
		ss << "return floor(pv.x*16.0h + pv.y) / 255.0h;" << std::endl;
		ss << "}" << std::endl;
	}
	/*static*/ void add_unpack_int0255norm_norm8( Ogre::StringStream &ss )
	{
		ss << "half2 unpack_int0255norm_norm8( half upv )" << std::endl;
		ss << "{" << std::endl;
		ss << "upv *= 255.0h;" << std::endl;
		ss << "half2 rv;" << std::endl;
		ss << "rv.x = floor(upv / 16.0h) * 16.0h;" << std::endl;
		ss << "rv.y = (upv - rv.x) / 15.0h;" << std::endl;
		ss << "return rv;" << std::endl;
		ss << "}" << std::endl;
	}
	//------------------------------------------------------------
	/*
	Упаковка двух чисел в одно:
		Оба числа - нормализованные [0,1]

	Результат: 1 color ячейка ( нормализованная 8bit )
	*/
	/*static*/ void add_pack_2norm_norm8( Ogre::StringStream &ss )
	{
		ss << "half pack_2norm_norm8( half2 pv )" << std::endl;
		ss << "{" << std::endl;
		ss << "pv.x *= 255.0h;" << std::endl;
		ss << "return pack_int0255norm_norm8( pv );" << std::endl;
		ss << "}" << std::endl;
	}
	/*static*/ void add_unpack_2norm_norm8( Ogre::StringStream &ss )
	{
		ss << "half2 unpack_2norm_norm8( half upv )" << std::endl;
		ss << "{" << std::endl;
		ss << "half2 rv = unpack_int0255norm_norm8( upv );" << std::endl;
		ss << "rv.x /= 255.0h;" << std::endl;
		ss << "}" << std::endl;
	}
	//------------------------------------------------------------
	/*
	Упаковка трёх float компонент нормали в две
	*/
	/*static*/ void add_encodeNormal( Ogre::StringStream &ss )
	{
		ss << "half2 encode_normal( half3 n ){ " << std::endl;
		ss << "half p = sqrt( n.z*8 + 8 );" << std::endl;
		ss << "return half2( n.xy/p + 0.5h ); }" << std::endl;
	}
	/*static*/ void add_decodeNormal( Ogre::StringStream &ss )
	{
		ss << "half3 decode_normal( half2 enc ){ " << std::endl;
		ss << "half2 fenc = enc*4 - 2;" << std::endl;
		ss << "half f = dot( fenc, fenc );" << std::endl;
		ss << "half g = sqrt( 1 - f/4 );" << std::endl;
		ss << "return half3( fenc*g, (1 - f/2) ); }" << std::endl;
	}
	//------------------------------------------------------------
	/*
	Все глобальные константы
	*/
	/*static*/ void add_constants( Ogre::StringStream &ss )
	{
		//Вектор для вычисления интенсивности цвета
		ss << "static const float3 LUMINANCE_VECTOR = {0.2125f, 0.7154f, 0.0721f};" << std::endl;
	}
	//------------------------------------------------------------
	/*
	Функция writeGB
	*/
	/*static*/ void add_writeGB( Ogre::StringStream &ss, MaterialGenerator::perm_type permutation )
	{
		// utility functions
		if( (permutation & DGMG::DFR_GBLAYOUT_01) != 0 )
		{
			add_pack_intnorm_16f( ss );
			add_pack_2norm_16f( ss );
		}
		else if( (permutation & DGMG::DFR_GBLAYOUT_02) != 0 )
		{
			add_encodeNormal( ss );
			add_pack_norm16f_2norm8( ss );
			add_pack_int0255norm_norm8( ss );
		}

		// writeGB function begin --------------------
		ss << "void writeGB (" << std::endl;

		// input params
		ss << " float depth" << std::endl;
		ss << ", half3 normal" << std::endl;
		ss << ", half3 color" << std::endl;
		ss << ", half spec_int" << std::endl;
		ss << ", half3 light_color" << std::endl;
		ss << ", half3 refl_color" << std::endl;
		ss << ", half spec_power" << std::endl;

		if( (permutation & DGMG::DFR_GBLAYOUT_01) != 0 )
		{
			ss << ", half matID" << std::endl;
			ss << ", half2 motion" << std::endl;
		}

		// output params
		ss << ", out half4 oColor0" << std::endl;
		ss << ", out half4 oColor1" << std::endl;
		ss << ", out half4 oColor2" << std::endl;
		ss << ", out half4 oColor3" << std::endl;

		ss << ") { " << std::endl;

		// code begin ---------------------
		if( (permutation & DGMG::DFR_GBLAYOUT_01) != 0 )
		{
			ss << "oColor0 = half4( normal, depth );" << std::endl;
			
			ss << "oColor1.r = pack_intnorm_16f( matID, color.r );" << std::endl;
			ss << "oColor1.g = pack_2norm_16f( color.g, color.b );" << std::endl;
			ss << "oColor1.b = pack_intnorm_16f( spec_power, spec_int );" << std::endl;

			ss << "oColor2 = half4( refl_color, motion.x );" << std::endl;
			ss << "oColor3 = half4( light_color, motion.y );" << std::endl;
		}
		else if( (permutation & DGMG::DFR_GBLAYOUT_02) != 0 )
		{
			ss << "const float3 LUMINANCE_VECTOR = float3(0.2125f, 0.7154f, 0.0721f);" << std::endl;

			ss << "half2 encNormal = encodeNormal( normal );" << std::endl;
			ss << "oColor0 = half4( pack_norm16f_2norm8(encNormal.x), pack_norm16f_2norm8(encNormal.y) );" << std::endl;

			ss << "half spp_spi = pack_int0255norm_norm8( spec_power, spec_int );" << std::endl;
			ss << "oColor1 = half4( color, spp_spi );" << std::endl;

			ss << "half refl_lum = dot( refl_color, LUMINANCE_VECTOR );" << std::endl;
			ss << "half maxComponent = max( refl_lum, max( light_color.r, max( light_color.g, light_color.b ) ) );" << std::endl;
			ss << "half Exp = ceil( log2(maxComponent) );" << std::endl;
			ss << "half Intensity = exp2(Exp);" << std::endl;
			ss << "oColor2 = half4( pack_norm16f_2norm8(depth), refl_lum/Intensity, 0.0h );" << std::endl;
			ss << "oColor3 = half4( light_color/Intensity, (Exp+128.0h)/255.0h );" << std::endl;
		}
		// code end ---------------------
		ss << "}" << std::endl;
		// writeGB function end --------------------
	}
	//------------------------------------------------------------
	/*
	Функция readGB
	*/
	/*static*/ void add_readGB( Ogre::StringStream &ss, MaterialGenerator::perm_type permutation )
	{
		// utility functions -------------------
		if( (permutation & DGMG::DFR_GBLAYOUT_01) != 0 )
		{
			add_unpack_intnorm_16f( ss );
			add_unpack_2norm_16f( ss );
		}
		else if( (permutation & DGMG::DFR_GBLAYOUT_02) != 0 )
		{
			add_decodeNormal( ss );
			add_unpack_norm16f_2norm8( ss );
			add_unpack_int0255norm_norm8( ss );
		}

		// readGB function begin ---------------
		ss << "void readGB (" << std::endl;

		//input params
		ss << " half4 iColor0" << std::endl;
		ss << ", half4 iColor1" << std::endl;
		ss << ", half4 iColor2" << std::endl;
		ss << ", half4 iColor3" << std::endl;

		//output params
		ss << ", out float depth" << std::endl;
		ss << ", out half3 normal" << std::endl;
		ss << ", out half3 color" << std::endl;
		ss << ", out half spec_int" << std::endl;
		ss << ", out half3 light_color" << std::endl;
		ss << ", out half3 refl_color" << std::endl;
		ss << ", out half spec_pow" << std::endl;

		if( (permutation & DGMG::DFR_GBLAYOUT_01) != 0 )
		{
			ss << ", out half matID" << std::endl;
			ss << ", out half2 motion" << std::endl;
		}

		ss << ") { " << std::endl;

		//code begin --------------------------
		if( (permutation & DGMG::DFR_GBLAYOUT_01) != 0 )
		{
			//RT0
			ss << "depth = iColor0.w;" << std::endl;
			ss << "normal = iColor0.xyz;" << std::endl;

			//unpacking RT1 color cells
			ss << "half2 upkColor;" << std::endl;
			ss << "upkColor = unpack_intnorm_16f( iColor1.r );" << std::endl;
			ss << "matID = upkColor.x;" << std::endl;
			ss << "color.r = upkColor.y;" << std::endl;

			ss << "upkColor = unpack_2norm_16f( iColor1.g );" << std::endl;
			ss << "color.gb = upkColor;" << std::endl;

			ss << "upkColor = unpack_intnorm_16f( iColor1.b );" << std::endl;
			ss << "spec_power = upkColor.x;" << std::endl;
			ss << "spec_int = upkColor.y;" << std::endl;

			//RT2 and RT3
			ss << "motion = half2( iColor2.a, iColor3.a );" << std::endl;
			ss << "refl_color = iColor2.rgb;" << std::endl;
			ss << "light_color = iColor3.rgb;" << std::endl;
		}
		else if( (permutation & DGMG::DFR_GBLAYOUT_02) != 0 )
		{
			//RT0
			ss << "half2 encNormal = half2( unpack_norm16f_2norm8( iColor0.rg ), unpack_norm16f_2norm8( iColor0.ba ) );" << std::endl;
			ss << "normal = decode_normal( encNormal );" << std::endl;

			//RT1
			ss << "color = iColor1.rgb;" << std::endl;
			ss << "half2 spp_spi = unpack_int0255norm_norm8( iColor1.a );" << std::endl;
			ss << "spec_pow = spp_spi.x;" << std::endl;
			ss << "spec_int = spp_spi.y;" << std::endl;

			//RT2 and RT3
			ss << "depth = unpack_norm16f_2norm8( iColor2.rg );" << std::endl;

			ss << "half Intensity = exp2( (iColor3.a*255.0h) - 128.0h);" << std::endl;
			ss << "refl_color = ( iColor2.b * Intensity ).xxx;" << std::endl;
			ss << "light_color = iColor3.rgb * Intensity;" << std::endl;
		}
		//code end --------------------------
		ss << "}" << std::endl;
		//readGB function end ---------------
	}
	//------------------------------------------------------------
	/*
	void geomGBuffer_vp (
		float4 iPos : POSITION,
		float3 iNormal : NORMAL,
		float2 iUV0 : TEXCOORD0,
		float3 iTangent : TANGENT0,
		
		out float4 oProjPos : POSITION,
		out float3 oViewPos : TEXCOORD0,
		out float3 oViewNormal : TEXCOORD1,
		out float3 oViewTangent : TEXCOORD2,
		out float3 oViewBinormal : TEXCOORD3,
		out float2 oUV0 : TEXCOORD4,
		
		uniform float4x4 worldViewProj,
		uniform float4x4 worldView
		)
	{
		oProjPos = mul( worldViewProj, iPos );
		oViewPos = mul( worldView, iPos ).xyz;
		oViewNormal = mul( worldView, float4(iNormal,0) ).xyz;
		oViewTangent = mul( worldView, float4(iTangent,0) ).xyz;
		oViewBinormal = cross( oViewNormal, oViewTangent );
		oUV0 = iUV0;
	}
	*/
	//------------------------------------------------------------
	/*static*/ void add_geomGBuffer_vp( Ogre::StringStream &ss, MaterialGenerator::perm_type permutation )
	{
		//No additional utility functions needed

		//entry function -------------------------------
		ss << "void geomGBuffer_vp(" << std::endl;

		//input params -----------------------------------
		ss << " float4 iPos : POSITION" << std::endl;
		
		Ogre::uint32 numTexCoords = getTexCoordsAmount( permutation );
		if( numTexCoords > 0 )
		{
			ss << ", float2 iUV0" << " : TEXCOORD0" << std::endl;
			if( numTexCoords > 1 ) {
				ss << ", float2 iUV1" << " : TEXCOORD1" << std::endl;
			}
		}

		MaterialGenerator::perm_type normOp = getNormalOperation( permutation );
		bool useVertexNormals = hasSubOperation( normOp, DGMG::DFR_NORMOP_VERTEX );
		bool useNormalMap = hasSubOperation( normOp, DGMG::DFR_NORMOP_MAP );
		if( useVertexNormals )
		{
			//Если будем работать с Вершинными Нормалями
			ss << ", float3 iNormal : NORMAL" << std::endl;

			//Без Вершинных нормалей в VertexProgram не нужно думать о Нормалях вообще
			if( useNormalMap )
			{
				//Для работы с Картой Нормалей поверх Вершинных нормалей
				ss << ", float3 iTangent : TANGENT0" << std::endl;
			}
		}

		ss << std::endl;

		//output params -----------------------------
		ss << ", out float4 oProjPos : POSITION" << std::endl;

	#ifdef WRITE_LINEAR_DEPTH
		ss << ", out float3 oViewPos : TEXCOORD0" << std::endl;
	#else
		ss << ", out float oDepth : TEXCOORD0" << std::endl;
	#endif

		int texCoordSemantic = 1;

		if( useVertexNormals )
		{
			ss << ", out float3 oViewNormal : TEXCOORD" << texCoordSemantic++ << std::endl;
			if( useNormalMap )
			{
				ss << ", out float3 oViewTangent : TEXCOORD" << texCoordSemantic++ << std::endl;
				ss << ", out float3 oViewBinormal : TEXCOORD" << texCoordSemantic++ << std::endl;
			}
		}

		if( numTexCoords > 0 )
		{
			//Как минимум один набор Координат
			ss << ", out float2 oUV0" << " : TEXCOORD" << texCoordSemantic++ << std::endl;
			if( numTexCoords > 1 ) {
				//Как минимум два набора Координат
				ss << ", out float2 oUV1" << " : TEXCOORD" << texCoordSemantic++ << std::endl;
			}
		}

		ss << std::endl;

		//uniform params -----------------------------
		ss << ", uniform float4x4 mWorldViewProj" << std::endl;
		ss << ", uniform float4x4 mWorldView" << std::endl;

		ss << "	)" << std::endl;
		
		//code begin geomGBuffer_vp -------------------------------
		ss << "{" << std::endl;
		ss << "	oProjPos = mul( mWorldViewProj, iPos );" << std::endl;

		if( useVertexNormals )
		{
			ss << "	oViewNormal = mul( mWorldView, float4( iNormal, 0 ) ).xyz;" << std::endl;
			if( useNormalMap )
			{
				ss << "	oViewTangent = mul( mWorldView, float4( iTangent, 0 ) ).xyz;" << std::endl;
				ss << "	oViewBinormal = cross( oViewNormal, oViewTangent );" << std::endl;
			}
		}

	#ifdef WRITE_LINEAR_DEPTH
		ss << "	oViewPos = mul( mWorldView, iPos ).xyz;" << std::endl;
	#else
		ss << "	oDepth = oPosition.w;" << std::endl;
	#endif

		//Здесь можно оставить цикл. Проверки условий выше.
		for( Ogre::uint32 i = 0; i < numTexCoords; ++i )
		{
			ss << "	oUV" << i << " = iUV" << i << ';' << std::endl;
		}

		ss << "}" << std::endl;
		//code end geomGBuffer_vp -------------------------------
	}
	//------------------------------------------------------------
	/*
	*/
	//------------------------------------------------------------
	/*static*/ void add_geomGBuffer_fp( Ogre::StringStream &ss, MaterialGenerator::perm_type permutation )
	{
		//utility functions-----------------------
		//will add it through cg #include directive
		//add_writeGB( ss, permutation );

		//entry function --------------------------
		ss << "void geomGBuffer_fp(" << std::endl;

		//input params -----------------------
	#ifdef WRITE_LINEAR_DEPTH
		ss << " float3 iViewPos : TEXCOORD0" << std::endl;
	#else
		ss << ", float1 iDepth : TEXCOORD0" << std::endl;
	#endif

		int texCoordSemantic = 1;

		MaterialGenerator::perm_type normOp = getNormalOperation( permutation );
		bool useVertexNormals = hasSubOperation( normOp, DGMG::DFR_NORMOP_VERTEX );
		bool useNormalMap = hasSubOperation( normOp, DGMG::DFR_NORMOP_MAP );
		if( useVertexNormals )
		{
			ss << ", float3 iViewNormal : TEXCOORD" << texCoordSemantic++ << std::endl;
			if( useNormalMap )
			{
				ss << ", float3 iViewTangent : TEXCOORD" << texCoordSemantic++ << std::endl;
				ss << ", float3 iViewBinormal : TEXCOORD" << texCoordSemantic++ << std::endl;
			}
		};

		Ogre::uint32 numTexCoords = getTexCoordsAmount( permutation );
		if( numTexCoords > 0 )
		{
			ss << ", float2 iUV0" << " : TEXCOORD" << texCoordSemantic++ << std::endl;
			if( numTexCoords > 1 ) {
				ss << ", float2 iUV1" << " : TEXCOORD" << texCoordSemantic++ << std::endl;
			}
		}

		ss << std::endl;

		//output params --------------------------------
		ss << ", out float4 oColor0 : COLOR0" << std::endl;
		ss << ", out float4 oColor1 : COLOR1" << std::endl;
		ss << ", out float4 oColor2 : COLOR2" << std::endl;
		ss << ", out float4 oColor3 : COLOR3" << std::endl;

		ss << std::endl;

		//uniform params ---------------------------------
		int samplerSemantic = 0;

		//diffuse sampler
		MaterialGenerator::perm_type diffOp = getDiffuseOperation( permutation );
		bool useDiffuseColor = hasSubOperation( diffOp, DGMG::DFR_DIFFOP_DIFFCOL );
		bool useDiffuseMap = hasSubOperation( diffOp, DGMG::DFR_DIFFOP_DIFFMAP );
		bool useDiffuseMapAlpha = hasSubOperation( diffOp, DGMG::DFR_DIFFOP_DIFFMAPxALPHA );
		if( useDiffuseMap )
		{
			ss << ", uniform sampler2D sDiffuseMap : register(s" << samplerSemantic++ << ")" << std::endl;
		}
		
		//normal sampler
		if( useNormalMap )
		{
			ss << ", uniform sampler2D sNormalMap : register(s" << samplerSemantic++ << ")" << std::endl;
		}

		//effects sampler
		bool useEffectMap = hasEffectsOperation( permutation );
		if( useEffectMap )
		{
			ss << ", uniform sampler2D sEffectsMap : register(s" << samplerSemantic++ << ")" << std::endl;
		}

	#ifdef WRITE_LINEAR_DEPTH
		ss << ", uniform float cFarDistance" << std::endl;
	#endif

		//Diffuse color
		if( useDiffuseColor )
		{
			ss << ", uniform float4 cDiffuseColor" << std::endl;
		}

		//Emissive color
		MaterialGenerator::perm_type emiOp = getEmissiveOperation( permutation );
		bool useEmissiveColor = hasSubOperation( emiOp, DGMG::DFR_EMIOP_EMICOL );
		bool useEmissiveMap = hasSubOperation( emiOp, DGMG::DFR_EMIOP_EMIMAP );
		if( useEmissiveColor )
		{
			ss << ", uniform float4 cEmissiveColor" << std::endl;
		}

		//Specular constants
		MaterialGenerator::perm_type specIntOp = getSpecIntOperation( permutation );
		MaterialGenerator::perm_type specPowOp = getSpecPowOperation( permutation );
		bool useSpecIntConstant = hasSubOperation( specIntOp, DGMG::DFR_SPECINTOP_SINTCON );
		bool useSpecPowConstant = hasSubOperation( specPowOp, DGMG::DFR_SPECPOWOP_SPOWCON );
		if( useSpecIntConstant )
		{
			//С OGRE можно получить только ACT_SPECULAR_COLOR
			//Он имеет тип float4, но нам нужна только одна компонента
			//для простоты вычислений будем брать r компоненту
			ss << ", uniform float4 cSpecColor" << std::endl;
		}
		if( useSpecPowConstant )
		{
			ss << ", uniform float cSpecPower" << std::endl;
		}

		//TODO: add reflections
		
		ss << "	)" << std::endl;
		
		ss << "{" << std::endl;

		//code begin geomGBuffer_fp -------------------------------

		// color ----------------------------------
		ss << " half4 wrtColor;" << std::endl;
		if( useDiffuseMap )
		{
			ss << " wrtColor = tex2D( sDiffuseMap, iUV0 );" << std::endl;
			if( useDiffuseMapAlpha ) {
				ss << " clip( 0.5h - wrtColor.a );" << std::endl;
			}
		}
		if( useDiffuseColor )
		{
			if( useDiffuseMap ) {
				ss << " wrtColor *= cDiffuseColor;" << std::endl;
			} else {
				ss << " wrtColor = cDiffuseColor;" << std::endl;
			}
		}
		// color ----------------------------------

		// normal ----------------------------------
		ss << " half3 wrtNormal;" << std::endl;
		if( useVertexNormals && useNormalMap ) {
			ss << " half3 texNormal = (tex2D( sNormalMap, iUV0 ) -0.5) * 2.0;" << std::endl;
			ss << " half3x3 normalRotation = half3x3( iViewTangent, iViewBinormal, iViewNormal);" << std::endl;
			ss << " wrtNormal = normalize( mul( texNormal, normalRotation ) );" << std::endl;
		} else if( useVertexNormals ) {
			ss << " wrtNormal = normalize( iViewNormal );" << std::endl;
		} else if( useNormalMap ) {
			ss << " wrtNormal = (tex2D( sNormalMap, iUV0 ) -0.5) * 2.0;" << std::endl;
		}
		// normal ----------------------------------

		// depth ---------------------
	#ifdef WRITE_LINEAR_DEPTH
		ss << " float wrtDepth = length( iViewPos ) / cFarDistance;" << std::endl;
	#else
		ss << " float wrtDepth = iDepth;" << std::endl;
	#endif
		// depth ---------------------

		// effects -----------------------------------------
		ss << " half4 effectSample = {0,0,0,0};" << std::endl;
		ss << " half3 wrtLight = {0,0,0};" << std::endl;
		if( useEffectMap )
		{
			//With one texture sampling we sample all effects values
			ss << " effectSample = tex2D( sEffectsMap, iUV0 );" << std::endl;
			//TODO Effect DetailMap
		}

		// emissive
		if( useEmissiveColor && useEmissiveMap ) {
			ss << " wrtLight = cEmissiveColor.rgb * effectSample.a;" << std::endl;
		} else if( useEmissiveColor ) {
			ss << " wrtLight = cEmissiveColor.rgb;" << std::endl;
		} else if( useEmissiveMap ) {
			ss << " wrtLight = effectSample.a;" << std::endl;
		}

		//TODO: IBL

		// speculars
		bool useSpecIntMap = hasSubOperation( specIntOp, DGMG::DFR_SPECINTOP_SINTMAP );
		bool useSpecPowMap = hasSubOperation( specPowOp, DGMG::DFR_SPECPOWOP_SPOWMAP );
		if( useSpecIntConstant )
		{
			if( useSpecIntMap ) {
				ss << " effectSample.r *= cSpecColor.r;" << std::endl;
			} else {
				ss << " effectSample.r = cSpecColor.r;" << std::endl;
			}
		}
		if( useSpecPowConstant )
		{
			if( useSpecPowMap ) {
				ss << " effectSample.g *= cSpecPower;" << std::endl;
			} else {
				ss << " effectSample.g = cSpecPower;" << std::endl;
			}
		}
		//TODO Reflection

		// effects end -----------------------------------------

		//TODO Material ID

		//TODO Motion Vector
		
		// writeGB call ------------------------
		ss << " writeGB(" << std::endl;
		ss << " wrtDepth" << std::endl;
		ss << ", wrtNormal" << std::endl;
		ss << ", wrtColor.rgb" << std::endl;
		ss << ", effectSample.r" << std::endl;
		ss << ", wrtLight" << std::endl;
		ss << ", half3(0,0,0)" << std::endl;
		ss << ", effectSample.g" << std::endl;

		if( (permutation & DGMG::DFR_GBLAYOUT_01) != 0 )
		{
			ss << ", half(0)" << std::endl;
			ss << ", half2(0,0)" << std::endl;
		}

		ss << ", oColor0" << std::endl;
		ss << ", oColor1" << std::endl;
		ss << ", oColor2" << std::endl;
		ss << ", oColor3" << std::endl;

		ss << ");" << std::endl;
		// writeGB call end------------------------

		ss << "}" << std::endl;
		//code end geomGBuffer_fp -------------------------------
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
#pragma endregion utility_functions

	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	/*static*/ DeferredGeometryMaterialGenerator::shared_ptr DeferredGeometryMaterialGenerator::getInstance( InitData const &idata )
	{
		if(!idata.isValid()) return shared_ptr();
		shared_ptr mg( new DeferredGeometryMaterialGenerator() );
		return mg->initialize( idata ) ? mg : shared_ptr();
	}
	//------------------------------------------------------------
	/*static*/ string_traits::str getSamplerAliasName( DeferredGeometryMaterialGenerator::SamplerPerm perm )
	{
		typedef DeferredGeometryMaterialGenerator MG;

		string_traits::str name = perm & MG::SMP_DETAIL ? "Detail" : "";

		switch( perm )
		{
		case MG::SMP_DIFFUSE:			name += "Diffuse"; break;
		case MG::SMP_NORMAL:			name += "Normal"; break;
		case MG::SMP_EFFECTS:			name += "Effects"; break;
		case MG::SMP_REFLECTION:		name += "Reflection"; break;
		case MG::SMP_IBL:				name += "IBL"; break;
		case MG::SMP_SPECINT:			name += "SpecInt"; break;
		case MG::SMP_SPECPOW:			name += "SpecPow"; break;
		case MG::SMP_EMISSIVE:			name += "Emissive"; break;
		case MG::SMP_REFLINT:			name += "ReflInt"; break;
		};

		return name;
	}
	//------------------------------------------------------------
	/*static*/ DeferredGeometryMaterialGenerator::SamplerPerm DeferredGeometryMaterialGenerator::getSamplerPerm( string_traits::in name )
	{
		static SamplerPerm PERMS[] = {
			SMP_DIFFUSE,
			SMP_NORMAL,
			SMP_EFFECTS,
			SMP_REFLECTION,
			SMP_IBL,
			SMP_SPECINT,
			SMP_SPECPOW,
			SMP_EMISSIVE,
			SMP_REFLINT
		};
		static int numPERMS = sizeof(PERMS) / sizeof(SamplerPerm);

		//try to find main sampler
		for( int i = 0; i < numPERMS; ++i )
		{
			if(string_equals<false>()(name, getSamplerAliasName(PERMS[i])))
				return PERMS[i];
		}

		//try to find detail sampler
		for( int i = 0; i < numPERMS; ++i )
		{
			SamplerPerm detailPerm = PERMS[i] | SMP_DETAIL;
			if(string_equals<false>()(name, getSamplerAliasName(detailPerm)))
				return PERMS[i];
		}

		return 0;
	}
	//------------------------------------------------------------
	/*static*/ void DeferredGeometryMaterialGenerator::removeDummyTUS(Ogre::Pass *pass)
	{
		//samplers to remove
		static SamplerPerm DUMMY_PERMS[] = {
			SMP_SPECINT,
			SMP_SPECPOW,
			SMP_EMISSIVE,
			SMP_REFLINT,
			SMP_SPECINT | SMP_DETAIL,
			SMP_SPECPOW | SMP_DETAIL,
			SMP_EMISSIVE | SMP_DETAIL,
			SMP_REFLINT | SMP_DETAIL
		};
		static int numPerms = sizeof(DUMMY_PERMS) / sizeof(SamplerPerm);

		//remove pass func
		auto removeFunc = [&](SamplerPerm perm)->void
		{
			auto numTUS = pass->getNumTextureUnitStates();
			for(decltype(numTUS) i = 0; i < numTUS; ++i)
			{
				if(string_equals<false>()(getSamplerAliasName(perm), pass->getTextureUnitState(i)->getName()))
				{
					pass->removeTextureUnitState(i);
					return;
				}
			}
		};

		for(int i = 0; i < numPerms; ++i)
		{
			removeFunc(DUMMY_PERMS[i]);
		}
	}
	//------------------------------------------------------------
	/*static*/ MaterialGenerator::perm_type DeferredGeometryMaterialGenerator::getPassPermutation(Ogre::Pass *pass)
	{
		if( pass == 0 ) return 0;

		MaterialGenerator::perm_type perm = 0;
		Demo::vector<SamplerPerm>::type present_samplers;

		//Gather information
		unsigned short numTUS = pass->getNumTextureUnitStates();
		for( unsigned short i = 0; i < numTUS; ++i ) {
			const Ogre::TextureUnitState* tempTUS = pass->getTextureUnitState(i);
			SamplerPerm tempPerm = getSamplerPerm( tempTUS->getName() );
			present_samplers.push_back(tempPerm);
		}

		auto checkSamplerPerm = [&](SamplerPerm sp)->bool
		{
			for(auto iter = present_samplers.begin(), end_iter = present_samplers.end(); iter != end_iter; ++iter)
			{
				if(*iter == sp) return true;
			}
			return false;
		};

		bool hasDiffuseMap = checkSamplerPerm( SMP_DIFFUSE );
		bool hasNormalMap = checkSamplerPerm( SMP_NORMAL );
		bool hasEffectsMap = checkSamplerPerm( SMP_EFFECTS );
		//TODO:add Reflection
		bool hasReflection = false;
		//TODO:add IBL
		bool hasIBL = false;
		bool hasSpecIntMap = checkSamplerPerm( SMP_SPECINT );
		bool hasSpecPowMap = checkSamplerPerm( SMP_SPECPOW );
		bool hasReflIntMap = checkSamplerPerm( SMP_REFLINT );
		bool hasEmissiveMap = checkSamplerPerm( SMP_EMISSIVE );

		bool hasDiffuseConst = pass->getDiffuse()		!= Ogre::ColourValue::White;
		bool hasSpecular = pass->getSpecular()			!= Ogre::ColourValue::Black;
		bool hasSpecIntConst = pass->getSpecular()		!= Ogre::ColourValue::White;
		bool hasSpecPowConst = pass->getShininess()		!= 0;
		bool hasReflConst = pass->getAmbient()			!= Ogre::ColourValue::White;
		bool hasEmissive = pass->getSelfIllumination()	!= Ogre::ColourValue::Black;
		bool hasEmissiveColor = pass->getSelfIllumination() != Ogre::ColourValue::White;

		//generate Permutation
		if(hasDiffuseMap) perm |= DFR_DIFFOP_DIFFMAP;
		if(hasDiffuseConst || !hasDiffuseMap) perm |= DFR_DIFFOP_DIFFCOL;

		perm |= DFR_NORMOP_VERTEX;
		if(hasNormalMap) perm |= DFR_NORMOP_MAP;

		if(hasSpecular)
		{
			if(hasSpecIntConst) perm |= DFR_SPECINTOP_SINTCON;
			if(hasSpecIntMap) perm |= DFR_SPECINTOP_SINTMAP;
			if(hasSpecPowConst) perm |= DFR_SPECPOWOP_SPOWCON;
			if(hasSpecPowMap) perm |= DFR_SPECPOWOP_SPOWMAP;
		}
		if(hasEmissive)
		{
			if(hasEmissiveColor) perm |= DFR_EMIOP_EMICOL;
			if(hasEmissiveMap) perm |= DFR_EMIOP_EMIMAP;
		}
		if(hasReflection)
		{
			if(hasReflConst) perm |= DFR_REFLOP_REFLCOLOR;
			if(hasReflIntMap)perm |= DFR_REFLOP_REFLINTMAP;
		}
		return perm;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	DeferredGeometryMaterialGenerator::DeferredGeometryMaterialGenerator()
	{
		vsMask = DFR_VERTEX_PROGRAM_MASK;
		fsMask = DFR_FRAGMENT_PROGRAM_MASK;
		matMask = DFR_TEMPLATE_MATERIAL_MASK;
		gbufferLayout = DFR_GBLAYOUT_01;
	}
	//------------------------------------------------------------
	DeferredGeometryMaterialGenerator::~DeferredGeometryMaterialGenerator()
	{
	}
	//------------------------------------------------------------
	Ogre::GpuProgramPtr DeferredGeometryMaterialGenerator::generateVertexProgram( MaterialGenerator::perm_type permutation )
	{
		Ogre::StringStream ss;
		
		//add vertex program code
		add_geomGBuffer_vp( ss, permutation );
		
		Ogre::String programSource = ss.str();
		Ogre::String programName = mgName + "VP_" + Ogre::StringConverter::toString(permutation,0,8,std::ios::fmtflags(std::ios::hex));

	#if DBS_LOGGING_ENABLED
		mpLog->logMessage(programName);
		mpLog->logMessage(programSource);
	#endif

		// Create shader object
		Ogre::HighLevelGpuProgramPtr ptrProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
			programName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "cg", Ogre::GPT_VERTEX_PROGRAM );

		ptrProgram->setSource(programSource);
		ptrProgram->setParameter("entry_point","geomGBuffer_vp");
		ptrProgram->setParameter("profiles","vs_1_1 arbvp1");

		//create uniform params associations
		const Ogre::GpuProgramParametersSharedPtr& params = ptrProgram->getDefaultParameters();
		params->setNamedAutoConstant("mWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
		params->setNamedAutoConstant("mWorldView", Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX);

		ptrProgram->load();

		return Ogre::GpuProgramPtr( ptrProgram );
	}
	//------------------------------------------------------------
	Ogre::GpuProgramPtr DeferredGeometryMaterialGenerator::generateFragmentProgram( MaterialGenerator::perm_type permutation )
	{
		if(mUtilsSource.empty())
		{
			Ogre::DataStreamPtr utilsSource = Ogre::ResourceGroupManager::getSingleton().openResource(
				"gbuffer_utils.cg", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
			mUtilsSource = utilsSource->getAsString();

#if DBS_LOGGING_ENABLED
			mpLog->logMessage("UTILS SOURCE");
			mpLog->logMessage(mUtilsSource);
#endif
		}

		Ogre::StringStream ss;

		//add fragment program code
		add_geomGBuffer_fp( ss, permutation );
		
		Ogre::String programSource = ss.str();
		Ogre::String programName = mgName + "FP_" + Ogre::StringConverter::toString(permutation,0,8,std::ios::fmtflags(std::ios::hex));

	#if DBS_LOGGING_ENABLED
		mpLog->logMessage(programName);
		mpLog->logMessage(programSource);
	#endif

		// Create shader object
		Ogre::HighLevelGpuProgramPtr ptrProgram =
			Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
					programName,
					Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
					"cg",
					Ogre::GPT_FRAGMENT_PROGRAM );

		string_traits::str finalSource = mUtilsSource + "\n" + programSource;
		ptrProgram->setSource(finalSource);
		ptrProgram->setParameter("entry_point", "geomGBuffer_fp");
		ptrProgram->setParameter("profiles", "ps_3_0 fp30 arbfp1");

		// set up the preprocessor defines
		// Important to do this before any call to get parameters, i.e. before the program gets loaded
		ptrProgram->setParameter("compile_arguments", getPPDefines(permutation));

		//create uniform params associations
		const Ogre::GpuProgramParametersSharedPtr& params = ptrProgram->getDefaultParameters();
		if( hasSpecIntConstant( permutation ) )
		{
			params->setNamedAutoConstant("cSpecColor", Ogre::GpuProgramParameters::ACT_SURFACE_SPECULAR_COLOUR);
		}
		if( getSpecPowMapsAmount( permutation ) == 0 )
		{
			params->setNamedAutoConstant("cSpecPower", Ogre::GpuProgramParameters::ACT_SURFACE_SHININESS);
		}
		if( hasDiffuseColor( permutation ) != 0 || getDiffuseMapsAmount( permutation ) == 0 )
		{
			params->setNamedAutoConstant("cDiffuseColor", Ogre::GpuProgramParameters::ACT_SURFACE_DIFFUSE_COLOUR);
		}
	#ifdef WRITE_LINEAR_DEPTH
		//TODO : Should this be the distance to the far corner, not the far clip distance?
		params->setNamedAutoConstant("cFarDistance", Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE);
	#endif

		ptrProgram->load();

		return Ogre::GpuProgramPtr(ptrProgram);
	}
	//------------------------------------------------------------
	Ogre::MaterialPtr DeferredGeometryMaterialGenerator::generateTemplateMaterial( MaterialGenerator::perm_type permutation )
	{
		string_traits::str matName = mgName + "Geom_" + Ogre::StringConverter::toString(permutation,0,8,std::ios::fmtflags(std::ios::hex));

		Ogre::MaterialPtr matPtr = Ogre::MaterialManager::getSingleton().create( matName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

		Ogre::Pass* pass = matPtr->getTechnique(0)->getPass(0);
		pass->setLightingEnabled( false );

		Ogre::uint32 numDiffuseMaps = getDiffuseMapsAmount( permutation );
		for( Ogre::uint32 i = 0; i < numDiffuseMaps; ++i ) {
			Ogre::TextureUnitState *tus = pass->createTextureUnitState();
			string_traits::str tusName = getSamplerAliasName(SMP_DIFFUSE);
			if(i > 0) tusName += getSamplerAliasName(SMP_DETAIL);
			tus->setName(tusName);
		}

		Ogre::uint32 numNormalMaps = getNormalMapsAmount( permutation );
		for( Ogre::uint32 i = 0; i < numNormalMaps; ++i ) {
			Ogre::TextureUnitState *tus = pass->createTextureUnitState();
			string_traits::str tusName = getSamplerAliasName(SMP_NORMAL);
			if(i > 0) tusName += getSamplerAliasName(SMP_DETAIL);
			tus->setName(tusName);
		}

		Ogre::uint32 numEffectsMaps = getEffectsMapsAmount( permutation );
		for( Ogre::uint32 i = 0; i < numEffectsMaps; ++i ) {
			Ogre::TextureUnitState *tus = pass->createTextureUnitState();
			string_traits::str tusName = getSamplerAliasName(SMP_EFFECTS);
			if(i > 0) tusName += getSamplerAliasName(SMP_DETAIL);
			tus->setName(tusName);
		}

		return matPtr;
	}
	//------------------------------------------------------------
	void DeferredGeometryMaterialGenerator::modifyTechnique(Ogre::Technique *tech)
	{
		//leave only Pass 0
		{
			if( tech->getNumPasses() == 0 ) {
				tech->createPass();
			}
			else while( tech->getNumPasses() != 1 ) {
				tech->removePass( 1 );
			}
		}

		Ogre::Pass* pass = tech->getPass(0);
		MaterialGenerator::perm_type perm = getPassPermutation(pass);
		removeDummyTUS(pass);

		//apply gbuffer layout defined by DeferredSystem
		perm |= gbufferLayout;

		Ogre::GpuProgramPtr vp = getVertexProgram( perm & vsMask );
		Ogre::GpuProgramPtr fp = getFragmentProgram( perm & fsMask );
		pass->setVertexProgram( vp->getName() );
		pass->setFragmentProgram( fp->getName() );
	}
	//------------------------------------------------------------
	bool DeferredGeometryMaterialGenerator::modifyMaterial(Ogre::Material *mat)
	{
		unsigned short techCount = mat->getNumTechniques();

		//Check if material was already modified
		for(unsigned short i = 0; i < techCount; ++i)
		{
			Ogre::Technique* pTech = mat->getTechnique(i);
			if(string_equals<false>()(pTech->getSchemeName(), mCreateSchemeName))
			{
				return true;
			}
		}

		Demo::list<Ogre::Technique*>::type modifyTechList;
		for(unsigned short i = 0; i < techCount; ++i)
		{
			Ogre::Technique* pTech = mat->getTechnique(i);
			if(string_equals<false>()(pTech->getSchemeName(), mConvertSchemeName))
			{
				modifyTechList.push_back(pTech);
			}
		}

		for(auto i = modifyTechList.begin(), iend = modifyTechList.end(); i != iend; ++i)
		{
			//create copy new technique
			Ogre::Technique* pNewTech = mat->createTechnique(**i);
			modifyTechnique(pNewTech);
			pNewTech->setSchemeName(mCreateSchemeName);
		}
		
		return !modifyTechList.empty();
	}
	//------------------------------------------------------------
	bool DeferredGeometryMaterialGenerator::initialize( InitData const &idata )
	{
		if(!idata.isValid()) return false;
		this->mgName = idata.mgName;
		this->mCreateSchemeName = idata.mCreateSchemeName;
		this->mConvertSchemeName = idata.mConvertSchemeName;

#if DBS_LOGGING_ENABLED
		mpLog = Ogre::LogManager::getSingleton().createLog("mg_" + mgName + ".log", false, false);
		mpLog->setTimeStampEnabled(false);
#endif

		return true;
	}
	//------------------------------------------------------------
	string_traits::str DeferredGeometryMaterialGenerator::getPPDefines( perm_type permutation )
	{
		string_traits::str strPPD;

		string_traits::str gbufferLayout;
		if(permutation & DFR_GBLAYOUT_01)
		{
			gbufferLayout = "GBLAYOUT_01";
		}
		else if(permutation & DFR_GBLAYOUT_02)
		{
			gbufferLayout = "GBLAYOUT_02";
		}

		strPPD += " -DGBLAYOUT=" + gbufferLayout;

		return strPPD;
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	/*static*/ DeferredLightMaterialGenerator::shared_ptr DeferredLightMaterialGenerator::getInstance(InitData const &idata)
	{
		if(!idata.isValid()) return shared_ptr();
		shared_ptr mg( new DeferredLightMaterialGenerator() );
		return mg->initialize( idata ) ? mg : shared_ptr();
	}
	//------------------------------------------------------------
	/*static*/ MaterialGenerator::perm_type DeferredLightMaterialGenerator::getPermutationLod( perm_type permutation )
	{
		//TODO implement
		return permutation;
	}
	//------------------------------------------------------------
	DeferredLightMaterialGenerator::DeferredLightMaterialGenerator()
	{
		vsMask = LIT_VERTEX_PROGRAM_MASK;
		fsMask = LIT_FRAGMENT_PROGRAM_MASK;
		matMask = LIT_TEMPLATE_MATERIAL_MASK;
	}
	//------------------------------------------------------------
	DeferredLightMaterialGenerator::~DeferredLightMaterialGenerator()
	{
	}
	//------------------------------------------------------------
	bool DeferredLightMaterialGenerator::initialize(InitData const &idata)
	{
		if(!idata.isValid()) return false;
		this->mgName = idata.mgName;

#if DBS_LOGGING_ENABLED
		mpLog = Ogre::LogManager::getSingleton().createLog("mg_" + mgName + ".log", false, false);
		mpLog->setTimeStampEnabled(false);
#endif

		return true;
	}
	//------------------------------------------------------------
	Ogre::GpuProgramPtr DeferredLightMaterialGenerator::generateVertexProgram( perm_type permutation )
	{
		string_traits::str programName = "DFR/";
		if (permutation & DeferredLightMaterialGenerator::LIT_TYPE_DIRECT)
		{
			programName += "fsquad_vs";
		}
		else
		{
			programName += "LightGeometry_vs";
		}
		Ogre::GpuProgramPtr ptr = Ogre::HighLevelGpuProgramManager::getSingleton().getByName(programName);
		assert(!ptr.isNull());
		return ptr;
	}
	//------------------------------------------------------------
	Ogre::GpuProgramPtr DeferredLightMaterialGenerator::generateFragmentProgram( perm_type permutation )
	{
		if(mMasterSource.empty())
		{
			Ogre::DataStreamPtr utilSourceCode = Ogre::ResourceGroupManager::getSingleton().openResource(
				"gbuffer_utils.cg", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

			Ogre::DataStreamPtr mainCodeSource = Ogre::ResourceGroupManager::getSingleton().openResource(
				"LightGeometry_ps.cg", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

			if(!utilSourceCode || !mainCodeSource)
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, Ogre::StringUtil::BLANK,
					"DeferredLightMaterialGenerator::generateFragmentProgram");
			}

			mMasterSource.append(utilSourceCode->getAsString());
			mMasterSource.append(mainCodeSource->getAsString());
		}

		// Create name
		string_traits::str name = mgName + "FP_" + Ogre::StringConverter::toString(permutation);

		// Create shader object
		Ogre::HighLevelGpuProgramPtr ptrProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
			name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "cg", Ogre::GPT_FRAGMENT_PROGRAM);

		ptrProgram->setSource(mMasterSource);
		ptrProgram->setParameter("entry_point", "main");
		ptrProgram->setParameter("profiles", "ps_3_0 arbfp1");

		// set up the preprocessor defines
		// Important to do this before any call to get parameters, i.e. before the program gets loaded
		ptrProgram->setParameter("compile_arguments", getPPDefines(permutation));

		setUpBaseParameters(ptrProgram->getDefaultParameters());

		return Ogre::GpuProgramPtr(ptrProgram);
	}
	//------------------------------------------------------------
	Ogre::MaterialPtr DeferredLightMaterialGenerator::generateTemplateMaterial( perm_type permutation )
	{
		string_traits::str materialName = "DFR/Light/";

		materialName += (permutation & DeferredLightMaterialGenerator::LIT_TYPE_DIRECT) != 0 ?
			"Quad" : "Geometry";

		if((permutation & DeferredLightMaterialGenerator::LIT_HAS_SHADOW) != 0)
			materialName += "Shadow";

		return Ogre::MaterialManager::getSingleton().getByName(materialName);
	}
	//------------------------------------------------------------
	string_traits::str DeferredLightMaterialGenerator::getPPDefines( perm_type permutation )
	{
		string_traits::str strPPD;

		//Get the type of light
		{
			string_traits::str lightType;
			if((permutation & DeferredLightMaterialGenerator::LIT_TYPE_POINT) != 0)
			{
				lightType = "POINT";
			}
			else if((permutation & DeferredLightMaterialGenerator::LIT_TYPE_SPOT) != 0)
			{
				lightType = "SPOT";
			}
			else if((permutation & DeferredLightMaterialGenerator::LIT_TYPE_DIRECT) != 0)
			{
				lightType = "DIRECT";
			}
			else
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Permutation must have a light type",
					"DeferredLightMaterialGenerator::getPPDefines");
			}
			strPPD += " -DLIGHT_TYPE=LIGHT_" + lightType;
		}

		//Optional parameters
		if (permutation & DeferredLightMaterialGenerator::LIT_HAS_SPECULAR)
		{
			strPPD += " -DIS_SPECULAR";
		}
		if (permutation & DeferredLightMaterialGenerator::LIT_HAS_SHADOW)
		{
			strPPD += " -DIS_SHADOW_CASTER";
		}
		return strPPD;
	}
	//------------------------------------------------------------
	void DeferredLightMaterialGenerator::setUpBaseParameters( const Ogre::GpuProgramParametersSharedPtr& params )
	{
		if(!params)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, Ogre::StringUtil::BLANK,
				"DeferredLightMaterialGenerator::setUpBaseParameters");
		}

		struct AutoParamPair { string_traits::str name; Ogre::GpuProgramParameters::AutoConstantType type; };	

		//A list of auto params that might be present in the shaders generated
		static const AutoParamPair AUTO_PARAMS[] = {
			{ "vpWidth",			Ogre::GpuProgramParameters::ACT_VIEWPORT_WIDTH },
			{ "vpHeight",			Ogre::GpuProgramParameters::ACT_VIEWPORT_HEIGHT },
			{ "worldView",			Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX },
			{ "invProj",			Ogre::GpuProgramParameters::ACT_INVERSE_PROJECTION_MATRIX },
			{ "invView",			Ogre::GpuProgramParameters::ACT_INVERSE_VIEW_MATRIX },
			{ "flip",				Ogre::GpuProgramParameters::ACT_RENDER_TARGET_FLIPPING },
			{ "lightDiffuseColor",	Ogre::GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR_POWER_SCALED },
			{ "lightSpecularColor", Ogre::GpuProgramParameters::ACT_LIGHT_SPECULAR_COLOUR_POWER_SCALED },
			{ "lightFalloff",		Ogre::GpuProgramParameters::ACT_LIGHT_ATTENUATION },
			{ "lightPos",			Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_VIEW_SPACE },
			{ "lightDir",			Ogre::GpuProgramParameters::ACT_LIGHT_DIRECTION_VIEW_SPACE },
			{ "spotParams",			Ogre::GpuProgramParameters::ACT_SPOTLIGHT_PARAMS },
			{ "farClipDistance",	Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE },
			{ "shadowViewProjMat",	Ogre::GpuProgramParameters::ACT_TEXTURE_VIEWPROJ_MATRIX }
		};
		int numParams = sizeof(AUTO_PARAMS) / sizeof(AutoParamPair);

		params->setIgnoreMissingParams(true);
		for (int i=0; i<numParams; i++)
		{
			params->setNamedAutoConstant(AUTO_PARAMS[i].name, AUTO_PARAMS[i].type);
		}
	}
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	//------------------------------------------------------------
	/*static*/ DeferredCompositorMaterialGenerator::shared_ptr DeferredCompositorMaterialGenerator::getInstance(InitData const &idata)
	{
		if(!idata.isValid()) return shared_ptr();
		shared_ptr mg( new DeferredCompositorMaterialGenerator() );
		return mg->initialize( idata ) ? mg : shared_ptr();
	}
	//------------------------------------------------------------
	DeferredCompositorMaterialGenerator::DeferredCompositorMaterialGenerator()
	{
		vsMask = DFR_VERTEX_PROGRAM_MASK;
		fsMask = DFR_FRAGMENT_PROGRAM_MASK;
		matMask = DFR_TEMPLATE_MATERIAL_MASK;
	}
	//------------------------------------------------------------
	/*virtual*/ DeferredCompositorMaterialGenerator::~DeferredCompositorMaterialGenerator()
	{
	}
	//------------------------------------------------------------
	bool DeferredCompositorMaterialGenerator::initialize(InitData const &idata)
	{
		if(!idata.isValid()) return false;
		this->mgName = idata.mgName;

#if DBS_LOGGING_ENABLED
		mpLog = Ogre::LogManager::getSingleton().createLog("mg_" + mgName + ".log", false, false);
		mpLog->setTimeStampEnabled(false);
#endif

		return true;
	}
	//------------------------------------------------------------
	/*virtual*/ Ogre::MaterialPtr DeferredCompositorMaterialGenerator::generateTemplateMaterial(perm_type permutation)
	{
		string_traits::str matName = mgName + "Comp_" + Ogre::StringConverter::toString(permutation,0,8,std::ios::fmtflags(std::ios::hex));
		Ogre::MaterialPtr matPtr = Ogre::MaterialManager::getSingleton().create(matName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		Ogre::Pass* pass = matPtr->getTechnique(0)->getPass(0);
		pass->setLightingEnabled( false );
		pass->setDepthCheckEnabled(false);
		pass->setDepthWriteEnabled(false);
		pass->setDepthFunction(Ogre::CMPF_ALWAYS_PASS);
		pass->setCullingMode(Ogre::CULL_NONE);

		int numTUS = 4;
		if(permutation & DFR_SEPARATE_LIGHT_REFL_TEX)
			numTUS += 2;

		for(int i = 0; i < numTUS; ++i)
		{
			Ogre::TextureUnitState *tus = pass->createTextureUnitState();
			tus->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
			tus->setTextureFiltering(Ogre::TFO_NONE);
		}

		return matPtr;
	}
	//------------------------------------------------------------
	/*virtual*/ Ogre::GpuProgramPtr DeferredCompositorMaterialGenerator::generateVertexProgram(perm_type permutation)
	{
		string_traits::str programName = "DFR/fsquad_vs";
		Ogre::GpuProgramPtr ptr = Ogre::HighLevelGpuProgramManager::getSingleton().getByName(programName);
		assert(!ptr.isNull());
		return ptr;
	}
	//------------------------------------------------------------
	/*virtual*/ Ogre::GpuProgramPtr DeferredCompositorMaterialGenerator::generateFragmentProgram(perm_type permutation)
	{
		Ogre::DataStreamPtr ptrMasterSource = Ogre::ResourceGroupManager::getSingleton().openResource(
			"gbuffer_utils.cg", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

		assert(ptrMasterSource.isNull()==false);

		Ogre::String programSource = ptrMasterSource->getAsString();
		Ogre::String programName = mgName + "FP_" + Ogre::StringConverter::toString(permutation,0,8,std::ios::fmtflags(std::ios::hex));

#if DBS_LOGGING_ENABLED
		mpLog->logMessage(programName);
		mpLog->logMessage(programSource);
#endif

		// Create shader object
		Ogre::HighLevelGpuProgramPtr ptrProgram =
			Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
			programName,
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"cg",
			Ogre::GPT_FRAGMENT_PROGRAM );

		ptrProgram->setSource(programSource);
		ptrProgram->setParameter("entry_point", "testQuad_fp");
		ptrProgram->setParameter("profiles", "ps_3_0 fp30 arbfp1");

		// set up the preprocessor defines
		// Important to do this before any call to get parameters, i.e. before the program gets loaded
		ptrProgram->setParameter("compile_arguments", getPPDefines(permutation));

		Ogre::GpuProgramParametersSharedPtr params = ptrProgram->getDefaultParameters();
		/*params->setNamedConstant("channel_filter", 0);
		params->setNamedConstant("gbuffer_filter", 11);
		params->setNamedConstant("quad_mode", 0);*/

		ptrProgram->load();

		return Ogre::GpuProgramPtr(ptrProgram);
	}
	//------------------------------------------------------------
	string_traits::str DeferredCompositorMaterialGenerator::getPPDefines( perm_type permutation )
	{
		string_traits::str strPPD;

		string_traits::str gbufferLayout;
		if(permutation & DFR_GBLAYOUT_01)
		{
			gbufferLayout = "GBLAYOUT_01";
		}
		else if(permutation & DFR_GBLAYOUT_02)
		{
			gbufferLayout = "GBLAYOUT_02";
		}

		strPPD += " -DGBLAYOUT=" + gbufferLayout;

		if(permutation & DFR_SEPARATE_LIGHT_REFL_TEX)
		{
			strPPD += " -DSEPARATE_LIGHT_REFL_TEX";
		}

		return strPPD;
	}
	//------------------------------------------------------------
}
}
#endif //DBS_LM_DEFERRED_RENDERING
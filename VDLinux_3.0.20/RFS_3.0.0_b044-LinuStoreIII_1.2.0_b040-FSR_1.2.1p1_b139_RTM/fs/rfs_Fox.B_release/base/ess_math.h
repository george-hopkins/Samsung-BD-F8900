/**
 *     @mainpage   Nestle Layer
 *
 *     @section Intro
 *       TFS5 File system's VFS framework
 *   
 *     @MULTI_BEGIN@ @COPYRIGHT_DEFAULT
 *     @section Copyright COPYRIGHT_DEFAULT
 *            COPYRIGHT. SAMSUNG ELECTRONICS CO., LTD.
 *                                    ALL RIGHTS RESERVED
 *     Permission is hereby granted to licensees of Samsung Electronics Co., Ltd. products
 *     to use this computer program only in accordance 
 *     with the terms of the SAMSUNG FLASH MEMORY DRIVER SOFTWARE LICENSE AGREEMENT.
 *     @MULTI_END@
 *
 */

/** 
 * @file		ess_math.h
 * @brief		math operations
 * @author		DongYoung Seo(dy76.seo@samsung.com)
 * @version		JUL-17-2006 [DongYoung Seo] First writing
 * @see			None
 */

#ifndef _FFAT_MATH_H_
#define _FFAT_MATH_H_

// includes
#include "ess_types.h"

#ifdef ESS_DEBUG
//	#define ESS_MATH_DEBUG
#endif

// defines
#define ESS_GET_MIN(a, b)		((a) < (b) ? (a) : (b))
#define ESS_GET_MAX(a, b)		((a) < (b) ? (b) : (a))

// ceiling device with bits
// _n : numerator
// _d : denominator
// _b : bit count
#define	ESS_MATH_CDB(_n, _d, _b)		EssMath_CeilingDivideWithBit(_n, _d, _b)
#define	ESS_MATH_CD(_n, _d)				EssMath_CeilingDivide(_n, _d)

#ifndef ESS_MATH_DEBUG
	// modified below macros because unsigned integer type overflow
	#define EssMath_CeilingDivideWithBit(_dwA, _dwB, _dwC)	(((_dwA) & ((_dwB) - 1)) ? (((_dwA) >> (_dwC)) + 1) : (_dwA) >> (_dwC))
	#define EssMath_CeilingDivide(_dwA, _dwB)				(((_dwA) % (_dwB)) ? (((_dwA) / (_dwB)) + 1) : ((_dwA) / (_dwB)))
#endif

// enum

// typedefs

// constant definitions

// external variable declarations


// function declarations

#ifdef __cplusplus
	extern "C" {
#endif

	extern t_boolean	EssMath_IsPowerOfTwo(t_uint32 dwValue);
	extern t_int32		EssMath_Log2(t_uint32 dwValue);

#ifdef ESS_MATH_DEBUG
	extern t_uint32	EssMath_CeilingDivideWithBit(t_uint32 dwA, t_uint32 dwB, t_uint32 dwC);
	extern t_uint32	EssMath_CeilingDivide(t_uint32 dwA, t_uint32 dwB);
#endif

	extern t_int32		EssMath_Rand(void);
	extern void			EssMath_Srand(t_int32 dwNew);

#ifdef __cplusplus
	};
#endif


#endif	/* #ifndef _FFAT_MATH_H_ */




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
 * @file		ess_bitmap.h
 * @brief		module for bitmap operation 
 * @author		DongYoung Seo(dy76.seo@samsung.com)
 * @version	  	JUL-18-2006 [DongYoung Seo] First writing
 * @see			None
 */

// includes
#include "ess_types.h"

#ifndef _ESS_BITMAP_H_
#define _ESS_BITMAP_H_

// defines

#ifdef ESS_STRICT_CHECK
	#define ESS_BITMAP_STRICT_CHECK
#endif

#define ESS_BITMAP_SET(pBitmap, _bit)	(((t_uint8*)pBitmap)[(_bit) >> 3] |= (0x01U << ((_bit) & 0x07)))
#define ESS_BITMAP_CLEAR(pBitmap, _bit)	(((t_uint8*)pBitmap)[(_bit) >> 3] &= ~(0x01U << ((_bit) & 0x07)))
#define ESS_BITMAP_CHANGE(pBitmap, _bit)	(((t_uint8*)pBitmap)[(_bit) >> 3] ^= ~(0x01U << ((_bit) & 0x07)))

#define ESS_BITMAP_IS_SET(pBitmap, _bit)		((((t_uint8*)pBitmap)[(_bit) >> 3] & (0x01U << ((_bit) & 0x07))) ? 1 : 0)
#define ESS_BITMAP_IS_CLEAR(pBitmap, _bit)	((((t_uint8*)pBitmap)[(_bit) >> 3] & (0x01U << ((_bit) & 0x07))) ? 0 : 1)


// enum

// typedefs

// constant definitions

// external variable declarations


// function declarations

#ifdef __cplusplus
	extern "C" {
#endif

	extern t_int32	EssBitmap_GetLowestBitZero(t_uint8* pBitmap, t_int32 dwSize);
	extern t_int32	EssBitmap_GetLowestBitOne(t_uint8* pBitmap, t_int32 dwSize);
	extern t_int32	EssBitmap_GetCountOfBitOne(t_uint8* pBitmap, t_int32 dwSize);

#ifdef __cplusplus
	};
#endif


#endif	/* #ifndef _ESS_BITMAP_H_ */




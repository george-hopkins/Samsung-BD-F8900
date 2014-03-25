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
 * @file		ess_ctype.h
 * @brief		migration from FSM/LIB/util.h
 * @author		Soojeong kim(sj003.kim@samsung.com)
 * @version	  	JEN-08-2007 [Soojeong Kim] First writing
 * @see			None
 */



#ifndef _ESS_CTPYE_H_
#define _ESS_CTPYE_H_

// includes
#include "ess_types.h"
#include "ess_unicode.h"

// defines
#define ESS_IsSpace(x)	(((x == ' ') || (x == '\t') || (x == '\r') || (x == '\n') || (x == '\v')) ? true : false)

#if (!defined(ESS_DUL))
	#define	ESS_STRICMP(a, b)				ESS_Stricmp(a, b)
#else
	#define	ESS_STRICMP(a, b, vol)			ESS_Stricmp(a, b, vol)	// Volume level NLS support
#endif

#ifdef __cplusplus
	extern "C" {
#endif

	extern void		ESS_Bincpy(void* pDst, const void* pSrc, t_uint32 dwLen);
	extern t_int32	ESS_Atoi(t_int8* str);
	extern t_int8*	ESS_Itoa(t_int32 nVal, t_int8* str);

	extern t_int32	ESS_Stricmp(const t_int8* s1, const t_int8* s2);

	extern t_int32	ESS_Isalnum(t_int8 c);

#ifdef __cplusplus
	};
#endif


#endif	/* #ifndef _ESS_CTPYE_H_ */




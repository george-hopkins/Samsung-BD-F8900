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
 * @file		ess_pstack.h
 * @brief		The file defines EssPStack (Personal stack module)
 * @author		DongYoung Seo(dy76.seo@samsung.com)
 * @version		JUL-08-2006 [DongYoung Seo] First writing
 * @see			None
 */

#ifndef _ESS_PSTACK_H_
#define _ESS_PSTACK_H_

//=============================================================================
//
//		NOTIFICATION & CAUTION !!!
//
//	1. PStack is not a reentrant module
//		Implement a synchronization control routine.
//
//=============================================================================


// includes
#include "ess_types.h"
#include "ess_dlist.h"

#ifdef ESS_DEBUG
	#define ESS_PSTACK_DEBUG
#endif

// just for test
#ifndef ESS_PSTACK_DEBUG
	#define ESS_PSTACK_DEBUG
#endif

// defines
#define ESS_PSTACK_DEFAULT_SIZE		24*1024		//!< default PSTACK size
#define ESS_PSTACK_META_SIZE		sizeof(EssPStack)		//!< sizeof (EssPStack)

// enum

// typedefs
typedef struct _EssPStack
{
	t_uint32	dwMaxDepth;			//!< PSTACK max depth
	t_uint32	dwDepth;			//!< current PSTACK depth
	t_int8*		pHeapData;			//!< data pointer
	EssDList	stList;				//!< list for PSTACK list
} EssPStack;

typedef struct
{
	EssDList	stListFree;			//!< free PSTACK
	EssDList	stListBusy;			//!< occupied PSTACK list
	EssPStack*	pDefaultPStack;		//!< pointer of default PSTACK
} EssPStackMain;

// constant definitions

// external variable declarations

// function declarations

#ifdef __cplusplus
	extern "C" {
#endif

	extern t_int32		EssPStack_Init(void);
	extern t_int32		EssPStack_InitMain(EssPStackMain* pPM);
	extern t_int32		ESSPStack_initPStack(EssPStack** ppPS, t_int8* pBuff, t_int32 dwSize);

	extern t_int32		EssPStack_Add(EssPStackMain* pPM, t_int8* pBuff, t_int32 dwSize);
	extern t_int32		EssPStack_Remove(EssPStack* pPStack);

	extern t_int32		EssPStack_AddDefault(EssPStackMain* pPM, t_int8* pBuff, t_int32 dwSize);
	extern EssPStack*	EssPStack_GetDefault(EssPStackMain* pPM);
	extern t_int32		EssPStack_RemoveDefault(EssPStackMain* pPM);

	extern EssPStack*	EssPStack_GetFree(EssPStackMain* pPM, t_uint32 dwSize);
	extern t_int32		EssPStack_Release(EssPStackMain* pPM, EssPStack* pPStack);

	extern void*		EssPStack_Alloc(EssPStackMain* pPM, EssPStack* pPStack, t_int32 dwSize);
	extern void			EssPStack_Free(EssPStackMain* pPM, EssPStack* pPStack, void* pPtr, t_int32 dwSize);

#ifdef __cplusplus
	};
#endif


#endif	/* #ifndef _ESS_PSTACK_H_ */




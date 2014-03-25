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
* @file			ess_rbtree2.h
* @brief		The file defines for ESS_RBTree2 module
* @brief		This module comes from ESS_RBTree module for static memory allocation.
* @author		Zhang Qing(qing1.zhang@samsung.com)
* @version		JUL-26-2006 [Zhang Qing] First writing
* @see			None
*/

#ifndef _ESS_RBTREE2_H_
#define _ESS_RBTREE2_H_

// includes
#include "ess_types.h"

// defines
#ifdef ESS_STRICT_CHECK
	#define ESS_RBT2_STRICT_CHECK		// CHECK VARIABLES 
#endif

#ifdef ESS_DEBUG
	#define ESS_RBT2_DEBUG				// enable debug
#endif

#define ESS_RBTREE2_NODE_KEY(pRBNode)	((pRBNode)->dwRBNodeKey)

#define ESS_RBTREE2_LEFT_MOST(pRBTree)	(pRBTree->pstRBNodeLeft)
#define ESS_RBTREE2_RIGHT_MOST(pRBTree)	(pRBTree->pstRBNodeRight)



// typedefs
typedef struct _EssRBNode2
{
	t_uint32			dwRBNodeKey;		//!< key of node
	t_uint32			dwRBNodeColor;		//!< 0: red, 1: black
	struct _EssRBNode2*	pstRBNodeLeft;		//!< left node
	struct _EssRBNode2*	pstRBNodeRight;		//!< right node
	struct _EssRBNode2*	pstRBNodeParent;	//!< parent node
#ifdef ESS_RBT2_DEBUG
	t_int32				dwBlackNum;
#endif	
} EssRBNode2;

typedef EssRBNode2 EssRBTree2;


// constant definitions

// external variable declarations


// function declarations

#ifdef __cplusplus
	extern "C" {
#endif

	extern t_int32		EssRBTree2_Init(EssRBTree2* pRBTree);
	extern t_int32		EssRBTree2_IsEmpty(EssRBTree2* pRBTree);

	extern t_int32		EssRBTree2_Delete(EssRBTree2* pRBTree, EssRBNode2* pRBNode);
	extern t_int32		EssRBTree2_Insert(EssRBTree2* pRBTree, EssRBNode2* pRBNode);
	extern EssRBNode2*	EssRBTree2_Lookup(EssRBTree2* pRBTree, t_uint32 dwRBNodeKey);
	extern EssRBNode2*	EssRBTree2_LookupBiggerApproximate(EssRBTree2* pRBTree, t_uint32 dwRBNodeKey);
	extern EssRBNode2*	EssRBTree2_LookupSmallerApproximate(EssRBTree2* pRBTree, t_uint32 dwRBNodeKey);

	extern t_int32		EssRBTree2_Count(EssRBTree2* pRBTree);

#ifdef ESS_RBT2_DEBUG
	extern t_int32 EssRBTree2_Print(EssRBTree2* pRBTree);
	extern t_int32 EssRBTree2_Check(EssRBTree2* pRBTree);
#endif

#ifdef __cplusplus
	};
#endif


#endif	/* #ifndef _ESS_RBTREE_H_ */




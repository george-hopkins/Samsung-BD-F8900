/*************************************************************************/
/*                                                                       */
/*               Copyright Mentor Graphics Corporation 2004              */
/*                         All Rights Reserved.                          */
/*                                                                       */
/* THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS  */
/* THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS   */
/* SUBJECT TO LICENSE TERMS.                                             */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                               VERSION       */
/*                                                                       */
/*      cs_extr.h                                      Nucleus PLUS 1.15 */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CS -    Common Services                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains function prototypes of all functions          */
/*      accessible to other components.                                  */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      cs_defs.h                           Common service definitions   */
/*                                                                       */
/*************************************************************************/
/* Check to see if the file has been included already.  */

#ifndef CS_EXTR
#define CS_EXTR

#include "cs_defs.h"        /* Include CS definitions    */



#ifndef NU_INLINE
VOID            CSC_Place_On_List(CS_NODE **head, CS_NODE *new_node);
VOID            CSC_Priority_Place_On_List(CS_NODE **head, CS_NODE *new_node);
VOID            CSC_Remove_From_List(CS_NODE **head, CS_NODE *node);
#else
#define         CSC_Place_On_List(head, new_node);                           \
	if (*((CS_NODE **) (head)))                                  \
	{                                                            \
		((CS_NODE *) (new_node)) -> cs_previous=                 \
		        (*((CS_NODE **) (head))) -> cs_previous;         \
		(((CS_NODE *) (new_node)) -> cs_previous) -> cs_next =   \
		        (CS_NODE *) (new_node);                          \
		((CS_NODE *) (new_node)) -> cs_next =                    \
		        (*((CS_NODE **) (head)));                        \
		(((CS_NODE *) (new_node)) -> cs_next) -> cs_previous =   \
		        ((CS_NODE *) (new_node));                        \
	}                                                            \
	else                                                         \
	{                                                            \
		(*((CS_NODE **) (head))) = ((CS_NODE *) (new_node));     \
		((CS_NODE *) (new_node)) -> cs_previous =                \
		        ((CS_NODE *) (new_node));                        \
		((CS_NODE *) (new_node)) -> cs_next =                    \
		        ((CS_NODE *) (new_node));                        \
	}

VOID            CSC_Priority_Place_On_List(CS_NODE **head, CS_NODE *new_node);

#define         CSC_Remove_From_List(head, node);                            \
	if (((CS_NODE *) (node)) -> cs_previous ==                   \
	        ((CS_NODE *) (node)))            \
	{                                                            \
		(*((CS_NODE **) (head))) =  NU_NULL;                     \
	}                                                            \
	else                                                         \
	{                                                            \
		(((CS_NODE *) (node)) -> cs_previous) -> cs_next =       \
		        ((CS_NODE *) (node)) -> cs_next;    \
		(((CS_NODE *) (node)) -> cs_next) -> cs_previous =       \
		        ((CS_NODE *) (node)) -> cs_previous;    \
		if (((CS_NODE *) (node)) == *((CS_NODE **) (head)))      \
			*((CS_NODE **) (head)) =                             \
			        ((CS_NODE *) (node)) -> cs_next;                 \
	}
#endif

#endif






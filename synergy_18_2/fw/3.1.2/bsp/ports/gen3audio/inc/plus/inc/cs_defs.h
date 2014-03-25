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
/*      cs_defs.h                                      Nucleus PLUS 1.15 */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CS -    Common Services                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions used in the common */
/*      service linked list routines.                                    */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      CS_NODE                             Link node structure          */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      nucleus.h                           Nucleus PLUS constants       */
/*                                                                       */
/*************************************************************************/
/* Check to see if the file has been included already.  */
#ifndef CS_DEFS
#define CS_DEFS

#if 0
#include "Kernel/plus/nucleus.h"            /* Include Nucleus constants */
#endif



/* Define a common node data structure that can be included inside of
   other system data structures.  */

typedef struct  CS_NODE_STRUCT
{
	struct CS_NODE_STRUCT  *cs_previous;
	struct CS_NODE_STRUCT  *cs_next;
	DATA_ELEMENT            cs_priority;

#if     PAD_1
	DATA_ELEMENT            cs_padding[PAD_1];
#endif

}  CS_NODE;

#endif /* CS_DEFS */




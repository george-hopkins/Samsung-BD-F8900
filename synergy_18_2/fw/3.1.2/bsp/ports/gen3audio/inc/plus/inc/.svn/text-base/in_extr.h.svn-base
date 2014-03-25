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
/*      in_extr.h                                      Nucleus PLUS 1.15 */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      IN -    Initialization                                           */
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
/*      nucleus.h                           Nucleus PLUS definitions     */
/*                                                                       */
/*************************************************************************/
#ifndef __IN_EXTR_H__
#define __IN_EXTR_H__
#include "Kernel/plus/nucleus.h"            /* Include system constants  */


/* Check to see if the file has been included already.  */

#ifndef IN_EXTR
#define IN_EXTR


VOID            INT_Initialize(VOID);
VOID            INC_Initialize(VOID *first_available_memory);

/* Depending on the target processor, these routines may only be callable
   successfully from Supervisor mode in Supervisor/User mode switching
   kernels.  If this is the case, SUC_ versions of these routines will be
   available for calling from User mode.
*/
VOID           *INT_Retrieve_Shell(INT vector);
#if 0
VOID           *INT_Setup_Vector(INT vector, VOID *pointer);
#endif
INT             INT_Vectors_Loaded(VOID);

#endif /* IN_EXTR */


#endif




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
/*      pi_extr.h                                      Nucleus PLUS 1.15 */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PI - Pipe Management                                             */
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
/*      pi_defs.h                           Pipe Management constants    */
/*                                                                       */
/*************************************************************************/
/* Check to see if the file has been included already.  */

#ifndef PI_EXTR
#define PI_EXTR

#include "pi_defs.h"        /* Include Pipe constants    */




/*  Initialization functions.  */

VOID            PII_Initialize(VOID);


/* Error checking core functions.  */

STATUS          PICE_Create_Pipe(NU_PIPE *pipe_ptr, CHAR *name,
                                 VOID *start_address, UNSIGNED pipe_size,
                                 OPTION message_type, UNSIGNED message_size,
                                 OPTION suspend_type);
STATUS          PICE_Delete_Pipe(NU_PIPE *pipe_ptr);
STATUS          PICE_Send_To_Pipe(NU_PIPE *pipe_ptr, VOID *message,
                                  UNSIGNED size, UNSIGNED suspend);
STATUS          PICE_Receive_From_Pipe(NU_PIPE *pipe_ptr, VOID *message,
                                       UNSIGNED size, UNSIGNED *actual_size, UNSIGNED suspend);


/* Error checking supplemental functions.  */

STATUS          PISE_Reset_Pipe(NU_PIPE *pipe_ptr);
STATUS          PISE_Send_To_Front_Of_Pipe(NU_PIPE *pipe_ptr, VOID *message,
        UNSIGNED size, UNSIGNED suspend);
STATUS          PISE_Broadcast_To_Pipe(NU_PIPE *pipe_ptr, VOID *message,
                                       UNSIGNED size, UNSIGNED suspend);


/* Core processing functions.  */

STATUS          PIC_Create_Pipe(NU_PIPE *pipe_ptr, CHAR *name,
                                VOID *start_address, UNSIGNED pipe_size,
                                OPTION message_type, UNSIGNED message_size,
                                OPTION suspend_type);
STATUS          PIC_Delete_Pipe(NU_PIPE *pipe_ptr);
STATUS          PIC_Send_To_Pipe(NU_PIPE *pipe_ptr, VOID *message,
                                 UNSIGNED size, UNSIGNED suspend);
STATUS          PIC_Receive_From_Pipe(NU_PIPE *pipe_ptr, VOID *message,
                                      UNSIGNED size, UNSIGNED *actual_size, UNSIGNED suspend);


/* Supplemental processing functions.  */

STATUS          PIS_Reset_Pipe(NU_PIPE *pipe_ptr);
STATUS          PIS_Send_To_Front_Of_Pipe(NU_PIPE *pipe_ptr, VOID *message,
        UNSIGNED size, UNSIGNED suspend);
STATUS          PIS_Broadcast_To_Pipe(NU_PIPE *pipe_ptr, VOID *message,
                                      UNSIGNED size, UNSIGNED suspend);


/* Information retrieval functions.  */

UNSIGNED        PIF_Established_Pipes(VOID);
STATUS          PIF_Pipe_Information(NU_PIPE *pipe_ptr, CHAR *name,
                                     VOID **start_address, UNSIGNED *pipe_size,
                                     UNSIGNED *available, UNSIGNED *messages,
                                     OPTION *message_type, UNSIGNED *message_size,
                                     OPTION *suspend_type, UNSIGNED *tasks_waiting,
                                     NU_TASK **first_task);
UNSIGNED        PIF_Pipe_Pointers(NU_PIPE **pointer_list,
                                  UNSIGNED maximum_pointers);

#endif






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
/*      qu_extr.h                                      Nucleus PLUS 1.15 */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      QU - Queue Management                                            */
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
/*      qu_defs.h                           Queue Management constants   */
/*                                                                       */
/*************************************************************************/
/* Check to see if the file has been included already.  */

#ifndef QU_EXTR
#define QU_EXTR

#include "qu_defs.h"        /* Include QU constants      */




/*  Initialization functions.  */

VOID            QUI_Initialize(VOID);


/* Core error checking functions.  */

STATUS          QUCE_Create_Queue(NU_QUEUE *queue_ptr, CHAR *name,
                                  VOID *start_address, UNSIGNED queue_size,
                                  OPTION message_type, UNSIGNED message_size,
                                  OPTION suspend_type);
STATUS          QUCE_Delete_Queue(NU_QUEUE *queue_ptr);
STATUS          QUCE_Send_To_Queue(NU_QUEUE *queue_ptr, VOID *message,
                                   UNSIGNED size, UNSIGNED suspend);
STATUS          QUCE_Receive_From_Queue(NU_QUEUE *queue_ptr, VOID *message,
                                        UNSIGNED size, UNSIGNED *actual_size, UNSIGNED suspend);

/* Supplemental error checking functions.  */

STATUS          QUSE_Reset_Queue(NU_QUEUE *queue_ptr);
STATUS          QUSE_Send_To_Front_Of_Queue(NU_QUEUE *queue_ptr, VOID *message,
        UNSIGNED size, UNSIGNED suspend);
STATUS          QUSE_Broadcast_To_Queue(NU_QUEUE *queue_ptr, VOID *message,
                                        UNSIGNED size, UNSIGNED suspend);

/* Core processing functions.  */

STATUS          QUC_Create_Queue(NU_QUEUE *queue_ptr, CHAR *name,
                                 VOID *start_address, UNSIGNED queue_size,
                                 OPTION message_type, UNSIGNED message_size,
                                 OPTION suspend_type);
STATUS          QUC_Delete_Queue(NU_QUEUE *queue_ptr);
STATUS          QUC_Send_To_Queue(NU_QUEUE *queue_ptr, VOID *message,
                                  UNSIGNED size, UNSIGNED suspend);
STATUS          QUC_Receive_From_Queue(NU_QUEUE *queue_ptr, VOID *message,
                                       UNSIGNED size, UNSIGNED *actual_size, UNSIGNED suspend);

/* Supplemental processing functions.  */

STATUS          QUS_Reset_Queue(NU_QUEUE *queue_ptr);
STATUS          QUS_Send_To_Front_Of_Queue(NU_QUEUE *queue_ptr, VOID *message,
        UNSIGNED size, UNSIGNED suspend);
STATUS          QUS_Broadcast_To_Queue(NU_QUEUE *queue_ptr, VOID *message,
                                       UNSIGNED size, UNSIGNED suspend);

/* Information gathering functions.  */


UNSIGNED        QUF_Established_Queues(VOID);
STATUS          QUF_Queue_Information(NU_QUEUE *queue_ptr, CHAR *name,
                                      VOID **start_address, UNSIGNED *queue_size,
                                      UNSIGNED *available, UNSIGNED *messages,
                                      OPTION *message_type, UNSIGNED *message_size,
                                      OPTION *suspend_type, UNSIGNED *tasks_waiting,
                                      NU_TASK **first_task);
UNSIGNED        QUF_Queue_Pointers(NU_QUEUE **pointer_list,
                                   UNSIGNED maximum_pointers);

#endif





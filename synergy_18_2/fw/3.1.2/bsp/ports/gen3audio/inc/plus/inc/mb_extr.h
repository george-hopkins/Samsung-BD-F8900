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
/*      mb_extr.h                                      Nucleus PLUS 1.15 */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      MB - Mailbox Management                                          */
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
/*      mb_defs.h                           Mailbox Management constants */
/*                                                                       */
/*************************************************************************/
/* Check to see if the file has been included already.  */

#ifndef MB_EXTR
#define MB_EXTR

#include "mb_defs.h"        /* Include MB constants      */




/*  Initialization functions.  */

VOID            MBI_Initialize(VOID);


/* Core error checking functions.  */

STATUS          MBCE_Create_Mailbox(NU_MAILBOX *mailbox_ptr, CHAR *name,
                                    OPTION suspend_type);
STATUS          MBCE_Delete_Mailbox(NU_MAILBOX *mailbox_ptr);
STATUS          MBCE_Send_To_Mailbox(NU_MAILBOX *mailbox_ptr, VOID *message,
                                     UNSIGNED suspend);
STATUS          MBCE_Receive_From_Mailbox(NU_MAILBOX *mailbox_ptr,
        VOID *message, UNSIGNED suspend);

/* Supplemental error checking functions.  */

STATUS          MBSE_Reset_Mailbox(NU_MAILBOX *mailbox_ptr);
STATUS          MBSE_Broadcast_To_Mailbox(NU_MAILBOX *mailbox_ptr,
        VOID *message, UNSIGNED suspend);

/* Core processing functions.  */

STATUS          MBC_Create_Mailbox(NU_MAILBOX *mailbox_ptr, CHAR *name,
                                   OPTION suspend_type);
STATUS          MBC_Delete_Mailbox(NU_MAILBOX *mailbox_ptr);
STATUS          MBC_Send_To_Mailbox(NU_MAILBOX *mailbox_ptr, VOID *message,
                                    UNSIGNED suspend);
STATUS          MBC_Receive_From_Mailbox(NU_MAILBOX *mailbox_ptr,
        VOID *message, UNSIGNED suspend);

/* Supplemental mailbox functions.  */

STATUS          MBS_Reset_Mailbox(NU_MAILBOX *mailbox_ptr);
STATUS          MBS_Broadcast_To_Mailbox(NU_MAILBOX *mailbox_ptr,
        VOID *message, UNSIGNED suspend);

/* Mailbox fact retrieval functions.  */

UNSIGNED        MBF_Established_Mailboxes(VOID);
STATUS          MBF_Mailbox_Information(NU_MAILBOX *mailbox_ptr, CHAR *name,
                                        OPTION *suspend_type, DATA_ELEMENT *message_present,
                                        UNSIGNED *tasks_waiting, NU_TASK **first_task);
UNSIGNED        MBF_Mailbox_Pointers(NU_MAILBOX **pointer_list,
                                     UNSIGNED maximum_pointers);

#endif






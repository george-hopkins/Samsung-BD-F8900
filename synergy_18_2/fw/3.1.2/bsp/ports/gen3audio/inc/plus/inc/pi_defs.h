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
/*      pi_defs.h                                      Nucleus PLUS 1.15 */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PI - Pipe Management                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions and constants for  */
/*      the message Pipe component.                                      */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      PI_PCB                              Pipe control block           */
/*      PI_SUSPEND                          Pipe suspension block        */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      cs_defs.h                           Common service definitions   */
/*      tc_defs.h                           Thread Control definitions   */
/*                                                                       */
/*************************************************************************/
/* Check to see if the file has been included already.  */

#ifndef PI_DEFS
#define PI_DEFS

#if 0
#include "cs_defs.h"        /* Common service constants  */
#include "tc_defs.h"        /* Thread control constants  */
#endif



/* Define constants local to this component.  */

#define         PI_PIPE_ID              0x50495045UL


/* Define the Pipe Control Block data type.  */

typedef struct PI_PCB_STRUCT
{
	CS_NODE             pi_created;            /* Node for linking to    */
	/*   created pipe list    */
	UNSIGNED            pi_id;                 /* Internal PCB ID        */
	CHAR                pi_name[NU_MAX_NAME];  /* Pipe name              */
	BOOLEAN             pi_fixed_size;         /* Fixed-size messages?   */
	BOOLEAN             pi_fifo_suspend;       /* Suspension type flag   */
#if     PAD_2
	DATA_ELEMENT        pi_padding[PAD_2];
#endif
	UNSIGNED            pi_pipe_size;          /* Total size of pipe     */
	UNSIGNED            pi_messages;           /* Messages in pipe       */
	UNSIGNED            pi_message_size;       /* Size of each message   */
	UNSIGNED            pi_available;          /* Available bytes        */
	BYTE_PTR            pi_start;              /* Start of pipe area     */
	BYTE_PTR            pi_end;                /* End of pipe area + 1   */
	BYTE_PTR            pi_read;               /* Read pointer           */
	BYTE_PTR            pi_write;              /* Write pointer          */
	UNSIGNED            pi_tasks_waiting;      /* Number of waiting tasks*/
	struct PI_SUSPEND_STRUCT
				*pi_urgent_list;        /* Urgent message suspend */
	struct PI_SUSPEND_STRUCT
				*pi_suspension_list;    /* Suspension list        */
} PI_PCB;


/* Define the Pipe suspension structure.  This structure is allocated off of
   the caller's stack.  */

typedef struct PI_SUSPEND_STRUCT
{
	CS_NODE             pi_suspend_link;       /* Link to suspend blocks */
	PI_PCB             *pi_pipe;               /* Pointer to the pipe    */
	TC_TCB             *pi_suspended_task;     /* Task suspended         */
	BYTE_PTR            pi_message_area;       /* Pointer to message area*/
	UNSIGNED            pi_message_size;       /* Message size requested */
	UNSIGNED            pi_actual_size;        /* Actual size of message */
	STATUS              pi_return_status;      /* Return status          */
} PI_SUSPEND;

#endif






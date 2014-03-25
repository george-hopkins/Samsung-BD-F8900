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
/*      hi_defs.h                                      Nucleus PLUS 1.15 */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      HI - History Management                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions and constants for  */
/*      the History Management component.                                */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      HI_HISTORY_ENTRY                    Entry in the history table   */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      tc_defs.h                           Thread Control definitions   */
/*                                                                       */
/*************************************************************************/
/* Check to see if the file has been included already.  */

#ifndef HI_DEFS
#define HI_DEFS

#include "tc_defs.h"        /* Thread control constants  */




/* Define constants local to this component.  */

#define         HI_MAX_ENTRIES          30
#define         HI_TASK                 1
#define         HI_HISR                 2
#define         HI_INITIALIZE           3


/* Define the History Entry data type.  */

typedef struct HI_HISTORY_ENTRY_STRUCT
{
	DATA_ELEMENT        hi_id;              /* ID of the history entry  */
	DATA_ELEMENT        hi_caller;          /* Task, HISR, or Initialize*/
	UNSIGNED            hi_param1;          /* First parameter          */
	UNSIGNED            hi_param2;          /* Second parameter         */
	UNSIGNED            hi_param3;          /* Third parameter          */
	UNSIGNED            hi_time;            /* Clock tick time for entry*/
	VOID               *hi_thread;          /* Calling thread's pointer */
} HI_HISTORY_ENTRY;

#endif






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
/* FILE NAME                                       VERSION               */
/*                                                                       */
/*      nucleus_specific.h          Nucleus PLUS\Integrator\RVCT 1.15.11 */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      System Constants                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains system constants common to both the           */
/*      application and the actual Nucleus PLUS components.  This file   */
/*      also contains data structure definitions that hide internal      */
/*      information from the application.                                */
/*                                                                       */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/*************************************************************************/

/* Check to see if this file has been included already.  */

#ifndef         NUCLEUS_SPECIFIC
#define         NUCLEUS_SPECIFIC

#define NU_PLUS_RELEASE_STRING "Copyright MGC 2004 - Nucleus PLUS - Integrator RVCT v. 1.15"

#define         NU_SUPERV_USER_MODE   0
#define         NU_MODULE_SUPPORT     0
#define         NU_MMU_MODE           0


/* MMU mode requires the module support */
#if (NU_MMU_MODE == 1)

#undef          NU_MODULE_SUPPORT
#define         NU_MODULE_SUPPORT       1

#endif

/* Module support requires mode switching */
#if (NU_MODULE_SUPPORT == 1)

#undef          NU_SUPERV_USER_MODE
#define         NU_SUPERV_USER_MODE   1

#endif

/* Describe the static base register, needed by DDL, for calling
** services. This also avoids allocating SB (=v6) to any other purpose. */
#if 0
__global_reg(6) void *sb;
#endif

/* Define the clock frequency that is used by the timer control unit (must be defined in hertz */
#define NU_System_Clock_Frequency       24000000

/* Define the number of Nucleus PLUS ticks that will occur every second.
   By default, the PLUS timer goes off every 10ms, so the PLUS System Clock
   ticks 100 times in a second. */
#define NU_PLUS_Ticks_Per_Second        100

/* Define the number of timer ticks that will occur every second.  This is
   usually the frequency of the timer used by the Nucleus clock.  It may
   be modified by a prescalar.  See the INT assembler file for more
   information about the clock */
#define NU_HW_Ticks_Per_Second          (NU_System_Clock_Frequency / 16)

/* The number of timer ticks between Nucleus timer interrupts that increment
   TMD_System_Clock.  See timer initialization code for details about how to
   determine this number. */
#define NU_HW_Ticks_Per_SW_Tick  (NU_HW_Ticks_Per_Second / NU_PLUS_Ticks_Per_Second)

/* Define NU_COUNT_DOWN if the system timer counts down to 0.  Undefine it if
   the system timer counts up from 0. This macro is used to switch between
   formulas to calculate the number of ticks since the systems started in
   NU_Get_Clock */
#define NU_COUNT_DOWN

/* This macro returns the value of the Nucleus timer (not the system clock)
   in the 32-bit variable, ticks.   The ARM Integrator Timer is a 16-bit timer */
#define NU_Retrieve_Hardware_Clock(hw_tick) hw_tick = (0x0000FFFF & (*(UINT32 *)(0x13000104)))

/* Toggle support for Nucleus POSIX */

#define         INCLUDE_NU_POSIX                0

/* Define the number of accesses required to read or write a pointer data
   type.  This value is used to make optimizations in some ports of Nucleus
   PLUS.  */

#define         NU_POINTER_ACCESS               1

/* Define the padding required for usage of the DATA_ELEMENT type within
   Nucleus PLUS structures.  These values insure proper alignment for the
   next structure member.  */

#define         PAD_1                           3
#define         PAD_2                           2
#define         PAD_3                           1

/* Define interrupt lockout and enable constants.  */

#define         NU_DISABLE_INTERRUPTS           0xC0
#define         NU_ENABLE_INTERRUPTS            0x00

/* Define constants that are target dependent and/or are used for internal
   purposes.  */

#define         NU_MIN_STACK_SIZE               240
#define         NU_MAX_NAME                     8
#define         NU_MAX_VECTORS                  22
#define         NU_MAX_LISRS                    8

#endif  /* !NUCLEUS */

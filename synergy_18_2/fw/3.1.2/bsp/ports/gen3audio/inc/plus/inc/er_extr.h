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
/*      er_extr.h                                      Nucleus PLUS 1.15 */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      ER - Error Management                                            */
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
/* MACROS                                                                */
/*                                                                       */
/*      NU_CHECK                                                         */
/*      NU_ASSERT                                                        */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      nucleus.h                           System definitions           */
/*                                                                       */
/*************************************************************************/
/* Check to see if the file has been included already.  */

#ifndef ER_EXTR
#define ER_EXTR

#include "Kernel/plus/nucleus.h"           /* Include system definitions */




/*  Initialization function.  */

VOID            ERI_Initialize(VOID);


/* System error handling function definition.  */

VOID            ERC_System_Error(INT error_code);

#ifdef NU_DEBUG

void ERC_Assert(CHAR *test, CHAR *name, UNSIGNED line);

#endif


#ifdef NU_ASSERT
#undef NU_ASSERT
#endif


#ifdef NU_CHECK
#undef NU_CHECK
#endif


#ifdef NU_DEBUG
#define NU_ASSERT( test ) \
	if ( !(test) ) ERC_Assert( #test, __FILE__, __LINE__ );  ((void) 0)
#else
#define NU_ASSERT( test ) ((void) 0)
#endif /* NU_DEBUG */


#ifdef NU_DEBUG
#define NU_ASSERT2( test ) \
	if ( !(test) ) ERC_Assert( #test, __FILE__, __LINE__ );  ((void) 0)
#else
#define NU_ASSERT2( test ) ((void) 0)
#endif /* NU_DEBUG */


#ifndef NU_NO_ERROR_CHECKING
#define NU_CHECK( test, statement ) \
	NU_ASSERT2( test );  if ( !(test) ) { statement; }  ((void) 0)
#else
#define NU_CHECK( test, statement ) NU_ASSERT2( test )
#endif /* NU_NO_ERROR_CHECKING */



#endif






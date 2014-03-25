#ifndef CSR_BT_TYPES_H__
#define CSR_BT_TYPES_H__
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 

   REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
    extern "C" {
#endif


/* Set to the most optimal type on the given platform that is 
   at least 8, 16 and 32 bit wide.
   e.g. using 32 bit types on a 32 bit processor will decrese 
   the code-size and increase the execution speed. */

#ifndef CsrUintFast8
#define CsrUintFast8 CsrUint32 
#endif
#ifndef CsrUintFast16
#define CsrUintFast16 CsrUint32 
#endif
#ifndef CsrUintFast32
#define CsrUintFast32 CsrUint32 
#endif
#ifndef CsrIntFast8
#define CsrIntFast8 CsrInt32 
#endif
#ifndef CsrIntFast16
#define CsrIntFast16 CsrInt32 
#endif
#ifndef CsrIntFast32
#define CsrIntFast32 CsrInt32 
#endif

#ifdef __cplusplus
}
#endif


#endif


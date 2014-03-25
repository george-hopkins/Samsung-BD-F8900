#ifndef APP_HANDER_H__
#define APP_HANDER_H__


/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
/* Definiton of the struct type for the CM application event functions  */
extern void CsrBtAppEventHandler(CsrUint16 id,  CsrUint16 cmd,CsrUint16 payload_length, void* data);



#ifdef __cplusplus
}
#endif

#endif//APP_HANDER_H__
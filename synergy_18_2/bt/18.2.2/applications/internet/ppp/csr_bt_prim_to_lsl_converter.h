#ifndef CSR_BT_PRIM_TO_LSL_CONVERTER_H__
#define CSR_BT_PRIM_TO_LSL_CONVERTER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void * ConvertPrimToLsl(void *message, CsrUint16 event,CsrUint8 state);

#ifdef __cplusplus
}
#endif

#endif

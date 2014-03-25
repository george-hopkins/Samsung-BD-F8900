#ifndef CSR_BT_OPS_HANDLER_H__
#define CSR_BT_OPS_HANDLER_H__

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_OPS_MODULE
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_ops_prim.h"
#include "csr_bt_obex_util.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CsrUint8               localServerCh;
    void                  *obexInst;
    dm_security_level_t    secIncoming;
    CsrSchedQid            appHandle;
    CsrBool                finalFlag;
    CsrBool                typeHeaderFlag;
    CsrUint8               cardType;
#ifdef CSR_BT_INSTALL_OPS_GET
    CsrBool                getFinalFlag;
#endif
    CsrUint16              supportedFormats;
    CsrBool                localSrmEnable;
    CsrUint16              localMaxPacketSize;
    CsrUint16              windowSize;
} OpsInstanceDataType;

typedef CsrUint8 (*OpsStateHandlerType)(OpsInstanceDataType * opsInstanceData, void *msg);

#ifdef __cplusplus
}
#endif

#endif
#endif

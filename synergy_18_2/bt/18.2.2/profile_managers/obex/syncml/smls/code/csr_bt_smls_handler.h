#ifndef CSR_BT_SMLS_HANDLER_H__
#define CSR_BT_SMLS_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_smls_prim.h"
#include "csr_bt_obex_util.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CsrUint8       localServerCh;
    CsrBtDeviceAddr     deviceAddr;
    void                *obexInst;
    CsrSchedQid              appHandle;
    dm_security_level_t secIncoming; 
    CsrUint8            obexCommand;
    CsrUint8            mimeType;
    CsrUint8            targetService;
    CsrBool             finalFlag;
    CsrUint16             localMaxPacketSize;
    CsrUint16             windowSize;
    CsrBool               localSrmEnable;
} SmlsInstanceDataType;

typedef CsrUint8 (* SmlsStateHandlerType)(SmlsInstanceDataType * smlsInstanceData, void *msg);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_SMLS_HANDLER_H__ */


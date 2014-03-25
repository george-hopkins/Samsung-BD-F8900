#ifndef CSR_BT_SMLC_HANDLER_H__
#define CSR_BT_SMLC_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_smlc_prim.h"
#include "csr_bt_obex_util.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CsrSchedQid           appHandle;
    CsrUint8              finalFlag;
    CsrUint16             bodyLength;
    CsrUint16             availablePutBodySize;
    CsrUint8             *body;
    CsrBool               headerCfm;
    dm_security_level_t   secOutgoing;
    CsrBool               cliBusy;
    CsrBool               activated;
    void                 *obexCliInst;
    CsrUint16             localMaxPacketSize;
    CsrUint16             windowSize;
    CsrBool               localSrmEnable;
} SmlcInstanceData;

typedef CsrUint8 (* SmlcStateHandlerType)(SmlcInstanceData * smlcInstanceData, void *msg);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_SMLC_HANDLER_H__ */

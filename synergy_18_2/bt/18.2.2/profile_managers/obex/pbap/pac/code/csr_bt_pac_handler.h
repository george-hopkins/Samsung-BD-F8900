#ifndef CSR_BT_PAC_HANDLER_H__
#define CSR_BT_PAC_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_pac_prim.h"
#include "csr_bt_obex_util.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CsrSchedQid              appHandle;
    CsrBtPacPrim             type;
    CsrBool                  isFinal;
    CsrUint16                pbSize;
    CsrUint8                 newMissedCall;
    dm_security_level_t      secOutgoing;
    void                    *obexInst;
    CsrUint16                localMaxPacketSize;
    CsrUint16                windowSize;
    CsrBool                  localSrmEnable;
    
} PacInst;

typedef CsrUint8 (*PacStateHandlerType)(PacInst *pacInstanceData, void *msg);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_PAC_HANDLER_H__ */

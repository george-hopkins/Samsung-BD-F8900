#ifndef CSR_BT_SYNCC_HANDLER_H__
#define CSR_BT_SYNCC_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_syncc_prim.h"
#include "csr_bt_obex_util.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define CSR_BT_SYNCC_CLIENT_INST_ID     0x01
#define CSR_BT_SYNCC_SERVER_INST_ID     0x02

typedef struct
{
    CsrSchedQid              appHandle;
    dm_security_level_t      secIncoming; 
    dm_security_level_t      secOutgoing; 
    void                    *obexClientInst;
    CsrUint16                maxFrameSize;
    CsrUint16                windowSize;
    CsrBool                  localSrmEnable;

#ifdef CSR_AMP_ENABLE
    CsrBtConnId              masterBtConnId;
#endif /* CSR_AMP_ENABLE */

    /* OBEX Server part of SYNC Client */
    void                    *obexServerInst;
    CsrBool                  isServerObexConnected;
    CsrBool                  putFinalFlag;
    CsrUint8                 serverChannel;
    CsrBool                  activated;
#ifdef CSR_AMP_ENABLE
    CsrBtConnId              slaveBtConnId;
#endif /* CSR_AMP_ENABLE */
} SynccInstanceData;

typedef CsrUint8 (* SynccStateHandlerType)(SynccInstanceData * synccInstanceData, void *msg);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_SYNCC_HANDLER_H__ */


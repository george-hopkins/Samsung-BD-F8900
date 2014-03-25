#ifndef CSR_BT_SYNCS_HANDLER_H__
#define CSR_BT_SYNCS_HANDLER_H__

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_SYNCS_MODULE
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_syncs_prim.h"
#include "csr_bt_obex_auth.h"
#include "csr_message_queue.h"
#include "csr_bt_obex_common.h"
#include "csr_bt_obex_util.h"


#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_SYNCS_CLIENT_INST_ID     0x01
#define CSR_BT_SYNCS_SERVER_INST_ID     0x02


typedef struct
{
    CsrUint32             connectionId;
    CsrUint16             nameOffset;
    CsrUint8             *payload;
    CsrUint16             payloadLength;
    CsrUint16             nAuthChallenge;
    CsrUint8              authChallenge[CSR_BT_OBEX_MAX_AUTHENTICATE_LENGTH];
} SyncsGetHandler;

typedef struct
{
    CsrUint16             supportedFormats;
    CsrUint8              putFinalFlag;
    CsrSchedQid           appHandle;
    dm_security_level_t   secIncoming; /* Incoming security level */
    void                 *obexServerInst;
    CsrUint8              serverChannel;
    CsrBool               activated;
    CsrUint8             *serviceRecordPtr;
    CsrUint16             serviceRecordLength;
    CsrUint8              obexCommand;        
    CsrUint16             localMaxPacketSize;
    CsrUint16             windowSize;
    CsrBool               localSrmEnable;
} SyncsInstanceDataType;

typedef CsrUint8 (* SyncsStateHandlerType)(SyncsInstanceDataType *syncsInstanceData, void *msg);

#ifdef __cplusplus
}
#endif

#endif /* EXCLUDE_CSR_BT_SYNCS_MODULE*/
#endif

#ifndef CSR_BT_BPPC_HANDLER_H__
#define CSR_BT_BPPC_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_bppc_prim.h"
#include "csr_bt_obex_util.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_BPPC_JOB_CHANNEL_IDLE_STATE              0x00
#define CSR_BT_BPPC_JOB_CHANNEL_CONNECTING_STATE        0x01
#define CSR_BT_BPPC_JOB_CHANNEL_CONNECTED_STATE         0x02
#define CSR_BT_BPPC_JOB_CHANNEL_DISCONNECTING_STATE     0x03
#define CSR_BT_BPPC_JOB_CHANNEL_CANCEL_CONNECT_STATE    0x04

#define CSR_BT_BPPC_STATUS_CHANNEL_IDLE_STATE           0x00
#define CSR_BT_BPPC_STATUS_CHANNEL_CONNECTING_STATE     0x01
#define CSR_BT_BPPC_STATUS_CHANNEL_CONNECTED_STATE      0x02
#define CSR_BT_BPPC_STATUS_CHANNEL_DISCONNECTING_STATE  0x03

#define CSR_BT_BPPC_OBJECT_CHANNEL_IDLE_STATE               0x00
#define CSR_BT_BPPC_OBJECT_CHANNEL_CONNECTABLE_STATE        0x01
#define CSR_BT_BPPC_OBJECT_CHANNEL_NON_CONNECTABLE_STATE    0x02

typedef struct
{
    CsrSchedQid                     appHandle;
    CsrBtDeviceAddr                 deviceAddr;
    CsrUint8                        objectChLocalServerCh;
    CsrUint32                       jobId;
    CsrUint16                       jobChPeerMaxPacketLength;
    CsrUint16                       objectChPeerMaxPacketLength;
    CsrUint16                       localMaxPacketLength;
    CsrUint16                       windowSize;
    CsrBool                         localSrmEnable;
    CsrUint16                       soapBodyLength;
    CsrUint16                       soapBodyIndex;
    CsrUint8                       *soapBody;
    dm_security_level_t             secOutgoing;
    CsrUint8                        jobChState;
    CsrUint8                        statusChState;
    CsrBool                         getEventActivated;
    CsrInt32                        objectChCount;
    CsrUint32                       objectChOffset;
    CsrBool                         objectChAppParamsFound;
    CsrUint32                       objectChBytesSent;
    CsrUtf8String                  *objectChName;
    CsrBtObexOperationStatusCode    operationStatus;
    void                           *obexJobChannelInst;
    void                           *obexStatusChannelInst;
    void                           *obexObjectChannelInst;
    CsrUint8                        objectChState;
#ifdef CSR_AMP_ENABLE
    CsrBtConnId                     statusChConnId;
    CsrBtConnId                     jobChConnId;
    CsrBtConnId                     objectChConnId;
#endif /* CSR_AMP_ENABLE */
} BppcInstanceData;
typedef CsrUint8 (*BppcStateHandlerType)(BppcInstanceData *pInst, void *msg);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_BPPC_HANDLER_H__ */

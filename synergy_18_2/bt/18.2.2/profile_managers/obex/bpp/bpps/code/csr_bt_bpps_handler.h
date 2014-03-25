#ifndef CSR_BT_BPPS_HANDLER_H__
#define CSR_BT_BPPS_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_bpps_prim.h"
#include "csr_bt_obex_util.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_BPPS_JOB_CHANNEL_INST_ID                         0x01
#define CSR_BT_BPPS_STATUS_CHANNEL_INST_ID                      0x02
#define CSR_BT_BPPS_OBJECT_CHANNEL_INST_ID                      0x03
                                                                
#define CSR_BT_BPPS_INVALID_TARGET                              0x00
#define CSR_BT_BPPS_JOB_CHANNEL_TARGET                          0x01
#define CSR_BT_BPPS_JOB_CHANNEL_SHORT_TARGET                    0x02
#define CSR_BT_BPPS_STATUS_CHANNEL_TARGET                       0x03
#define CSR_BT_BPPS_STATUS_CHANNEL_SHORT_TARGET                 0x04


#define CSR_BT_BPPS_JOB_CHANNEL_IDLE_STATE                      0x00
#define CSR_BT_BPPS_JOB_CHANNEL_ACTIVATED_STATE                 0x01
#define CSR_BT_BPPS_JOB_CHANNEL_CONNECTED_STATE                 0x02

#define CSR_BT_BPPS_STATUS_CHANNEL_IDLE_STATE                   0x00
#define CSR_BT_BPPS_STATUS_CHANNEL_CONNECTED_STATE              0x01
#define CSR_BT_BPPS_STATUS_CHANNEL_DEACTIVATING                 0x02

#define CSR_BT_BPPS_OBJECT_CHANNEL_IDLE_STATE                   0x00
#define CSR_BT_BPPS_OBJECT_CHANNEL_CONNECTED_STATE              0x01
#define CSR_BT_BPPS_OBJECT_CHANNEL_SENDING_OBJECT_STATE         0x02

typedef struct
{
    CsrSchedQid          appHandle;
    dm_security_level_t  secIncoming;
    CsrUint8             jobChLocalServerCh;
    CsrUint8             statusChLocalServerCh;
    CsrUint8             jobChState;
    CsrBool              jobChFinalBitSet;
    CsrBtReasonCode      jobChReasonCode;
    CsrBtSupplier        jobChReasonSupplier;
    CsrUint8             statusChState;
    CsrUint8             objectChState;
    CsrBool              objectChHeadersFound;
    CsrUint8             jobChObexCommand;
    CsrBool              getPrinterAttrOperationCh;
    CsrBool              getJobAttrOperationCh;
    CsrBool              cancelOperationCh;
    CsrUint8             targetId;
    CsrUint16            maxLocalObexPacketLength;
    CsrUint16            windowSize;
    CsrBool              localSrmEnable;
    CsrUint16            serviceRecordLength;  
    CsrUint16            savedStatusChSoapActionLength;
    CsrUint16            savedStatusChSoapActionIndex;
    CsrUint16            savedJobChSoapActionLength;
    CsrUint16            savedJobChSoapActionIndex;
    CsrUint16            jobChResponsePacketLength;
    CsrUint16            statusChResponsePacketLength;
    CsrBool              statusChGetEventActive;
    CsrBool              statusChGetReadyToSend;
    CsrUint32            statusChGetEventJobId;
    CsrBtDeviceAddr      deviceAddr;
    CsrUint8             numOfConn;   
    CsrBool              jobCompletion;   
    CsrUint8            *serviceRecord;
    CsrUint8            *savedStatusChSoapAction;
    CsrUint8            *savedJobChSoapAction;
    void                *savedObjectChDownStreamMsg;
    void                *savedObjectChUpStreamMsg;
    void                *savedJobChMsg;
    void                *obexJobChannelInst;
    void                *obexStatusChannelInst;
    void                *obexObjectChannelInst;
    void                *recvMsgP;
#ifdef CSR_AMP_ENABLE
    CsrBtConnId          statusChConnId;
    CsrBtConnId          jobChConnId;
    CsrBtConnId          objectChConnId;
#endif /* CSR_AMP_ENABLE */

} BppsInstanceData;

typedef CsrUint8 (*BppsStateHandlerType)(BppsInstanceData *pInst);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_BPPS_HANDLER_H__ */

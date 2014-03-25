#ifndef CSR_BT_PPP_HANDLER_H__
#define CSR_BT_PPP_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_ppp_prim.h"
#include "csr_bt_ppp_fcs.h"
#include "csr_bt_util.h"
#include "csr_message_queue.h"
#include "csr_bt_internet_primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_PPP_DATA_REQ_QUEUE_SIZE     10

#define PPP_STATE_INITIAL           0
#define PPP_STATE_STARTING          1
#define PPP_STATE_CLOSED            2
#define PPP_STATE_STOPPED           3
#define PPP_STATE_CLOSING           4
#define PPP_STATE_STOPPING          5
#define PPP_STATE_REQ_SENT          6
#define PPP_STATE_ACK_RCVD          7
#define PPP_STATE_ACK_SENT          8
#define PPP_STATE_OPENED            9

typedef struct PppRegisteredNetworkProtocolTag
{
    CsrUint16 ncpProtocol;    /* Network order */
    CsrSchedQid dataQueue;
    CsrUint16 dataProtocol;    /* Network order */
    struct PppRegisteredNetworkProtocolTag * next;
} PppRegisteredNetworkProtocol;

typedef struct
{
    CsrUint16 event;
    void * receivedMessage;
    CsrSchedQid applicationQueue;
    CsrUint8 pppState;
    CsrBtDeviceAddr deviceAddr;
    CsrUint16 maxPayloadLength;
    CsrUint16 currentPayloadIndex;
    CsrUint8 hdlcAndPppHeader[4];
    CsrBtPppDataInd * pppDataInd;
    CsrUint16 mtu;
    CsrUint16 mru;
    CsrBool rxAsynchronousControlCharacterMap[0x20];
    CsrBool txAsynchronousControlCharacterMap[0x20];
    CsrUint8 rxAccmConfiguration[4];
    CsrUint8 rxHdlcState;
    CsrBool rxHdlcEscapeFound;
    CsrBool transmissionAllowed;
    CsrBtPppDataReq * pppDataReq;
    CsrMessageQueueType * dataReqQueue;
    CsrUint16 numberOnQueue;
    CsrUint16 queueTail;
    CsrUint16 queueHead;
    CsrBtPppDataReq * pppDataReqQueue[CSR_BT_PPP_DATA_REQ_QUEUE_SIZE];
    CsrUint8 txHdlcState;
    CsrUint16 txCurrentPayloadIndex;
    CsrBool txHdlcEscapeSent;
    CsrUint8 txFcs[2];
    CsrUint8 txIdentifier;
    CsrBool configureMru;
    CsrBool configureAccm;
    CsrBool configureAp;
    CsrBool usePap;
    CsrCharString * loginName;
    CsrCharString * password;
    CsrSchedTid papTimer;
    CsrUint8 papIdentifier;
    CsrSchedTid lcpTimer;
    PppRegisteredNetworkProtocol * registeredNetworkProtocolList;
} PppInstanceData;

#ifdef __cplusplus
}
#endif

#endif


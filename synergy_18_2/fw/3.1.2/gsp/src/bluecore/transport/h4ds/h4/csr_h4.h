#ifndef CSR_H4_H__
#define CSR_H4_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_panic.h"
#include "csr_tm_bluecore_transport.h"
#include "csr_util.h"
#include "csr_serial_com.h"
#include "csr_queue_lib.h"

#include "csr_h4_events.h"


#ifdef __cplusplus
extern "C" {
#endif

#define QMSG_STORE(q, m)                         CsrQueueMsgStore((TXMSG **) (q), (TXMSG *) (m))

#define EVENT_BUFFER_SIZE                       (10)

#define HCI_MAX_HEADER_SIZE                     (5)
#define H4_PACKET_HEADER_SIZE                   (1)

/* Define used in in- and decoding of tunneled HCI messages.
   An unfragmented package is both start and end */
#define UNFRAGMENTED                            (FRAGMENT_START | FRAGMENT_END)

/* States */
#define H4_UNINIT_STATE                         (0x00)
#define H4_INIT_STATE                           (0x01)

/* H4 RX states */
#define H4_TX_HDR_STATE                         (0x00)
#define H4_TX_BODY_STATE                        (0x01)

typedef struct
{
    CsrUint8 *msg;
    CsrUint8  len;
} eventBufElement;

/* Instance data for H4 */
typedef struct
{
    CsrUint8        state;
    TXMSG          *sendQ;
    TXMSG          *currMsg;
    CsrUint8       *scoData[2];
    CsrUint8        scoIndex;
    CsrUint8       *eventMsg;
    CsrUint16       eventMsgOffset;
    CsrUint8        eventChannel;
    eventBufElement eventBuf[EVENT_BUFFER_SIZE];
    CsrUint8        eventBufCurr;
    CsrUint8        pumpRequest;
    CsrUint8        txState;
    CsrTime         txBufFullWaitTime;
    CsrSchedTid     txBufFullTid;
    CsrUint16       txBufSize;
} H4InstanceData;

extern H4InstanceData CsrH4Inst;

/* HCI Package Headers */
#define HCI_HDR_CMD             0x01
#define HCI_HDR_ACL             0x02
#define HCI_HDR_SCO             0x03
#define HCI_HDR_EVT             0x04

/* Packet sizes */
#define HCI_EVENT_HDR_SIZE      2 /* event=1, length=1 */
#define HCI_COMMAND_HDR_SIZE    3 /* opcode=2, length=1 */
#define HCI_ACL_HDR_SIZE        4 /* handle+flags=2, length=2 */
#define HCI_SCO_HDR_SIZE        3 /* handle=2, length=1 */

#define HCI_EXT_PAYLOAD_DSC_SIZE 1;

#define HCI_COMMAND_DATA_SIZE   255
#define HCI_EVENT_DATA_SIZE     255
#define HCI_ACL_DATA_SIZE       1000

#define MAX_HCI_COMMAND_SIZE    (HCI_COMMAND_HDR_SIZE + HCI_COMMAND_DATA_SIZE)
#define MAX_HCI_EVENT_SIZE      (HCI_EVENT_HDR_SIZE + HCI_EVENT_DATA_SIZE)
#define MAX_HCI_ACL_SIZE        (HCI_ACL_HDR_SIZE + HCI_ACL_DATA_SIZE)

#ifdef __cplusplus
}
#endif

#endif

#ifndef CSR_TYPE_A_PRIVATE_H__
#define CSR_TYPE_A_PRIVATE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_sched.h"
#include "csr_sdio.h"
#include "csr_aclbuf_lower.h"
#include "csr_membuf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Type-A service IDs */
#define SDIO_CHANNEL_HCI (1)
#define SDIO_CHANNEL_ACL (2)
#define SDIO_CHANNEL_SCO (3)
#define SDIO_CHANNEL_HCI_EVT (4)

/* main states */
typedef enum
{
    WaitForInsertS = 0,
    InitS,
    IdleS,
#if CSR_TYPE_A_SLEEP_TIMEOUT > 0
    SleepS,
    WakingS,
#endif
    RstS,
    TxS,
    LastOfStates
} TypeAState;

typedef struct
{
    /* kept like the BT transport abstraction - see transport_abstraction.h */
    CsrUint8 *buf;
    CsrUint32 bufLen;
} SdioATxData;

/* Define TYPEA_PEEK_BYTES unless defined. */
#ifndef TYPEA_PEEK_BYTES
#ifdef CSR_ACLBUF_ENABLE
#define TYPEA_PEEK_BYTES 16
#else
#define TYPEA_PEEK_BYTES 0
#endif
#endif

typedef struct
{
    CsrUint8 *buf;          /* packet received from chip - SDIO header */
    void     *bufCtx;       /* Buffer context (buffer supplier private data) */
    CsrUint16 bufLen;       /* len of packet */
#ifdef CSR_ACLBUF_ENABLE
    CsrBool aclBuf;
#endif
    CsrUint8  *hdr;         /* Type-A and HCI headers + peek */
    CsrMemBuf *hdrBuf;

    CsrUint8 cha;           /* extracted values from header */
} SdioARxMsg;

typedef struct
{
    CsrSdioFunction *sdioHandle;              /* handle to SDIO device */

    SdioARxMsg   rxMsg;
    void        *rxPool;
    SdioATxData *txMsg;

    void      *txPool;
    CsrUint8  *txBuffer;
    CsrMemBuf *txBuf;

    TypeAState  state;
    CsrUint8    chipState;
    CsrSchedTid timer;

    CsrSchedBgint bgint_cardevent;
    CsrSchedBgint bgint_intr;
    CsrSchedBgint bgint_txdone;
    CsrSchedBgint bgint_rxdone;
    CsrSchedBgint bgint_panic;
#ifdef CSR_ACLBUF_ENABLE
    CsrAclBufCb aclbufcb;
#endif
    CsrUint32 pendingTxPackets;                  /* number of pending packets to Tx */
    CsrUint8  curChipSleepState;                 /* current sleep state of chip */
    CsrUint8  eventAndIrq;
    CsrUint8  readBuf8;
    CsrUint8  readyState;

    CsrUint8 readNeedAck;
    CsrUint8 rxTries;
    CsrUint8 maxRxTries;
    CsrUint8 txTries;
    CsrUint8 maxTxTries;
} CsrSdioAInstanceData;

#ifdef __cplusplus
}
#endif

#endif

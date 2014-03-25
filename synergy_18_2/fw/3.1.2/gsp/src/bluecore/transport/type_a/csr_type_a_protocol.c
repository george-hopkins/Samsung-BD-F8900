/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"

#include "csr_membuf.h"

#include "csr_sdio.h"
#include "csr_type_a_sdio.h"

#include "csr_type_a_private.h"
#include "csr_type_a_protocol.h"
#include "csr_type_a_quirks.h"

#include "csr_transport.h"

#ifdef CSR_ACLBUF_ENABLE
#include "csr_aclbuf.h"
#include "csr_aclbuf_lower.h"
#endif

#include "csr_log_gsp.h"
#include "csr_tm_bluecore_private_lib.h"

/* Wakeup event bits */
#define TYPEA_WUE_INTR  (1 << 0)
#define TYPEA_WUE_CMPL  (1 << 1)

/* Chip init states */
#define CHST_UNK        0
#define CHST_SCRATCH    1
#define CHST_FUNCEN     2
#define CHST_WAIT       3
#define CHST_RUN        4
#define CHST_UP         5
#define CHST_DSWAIT     6
#define CHST_DS         7

/* specify descriptors to support */
static CsrSdioFunctionId sdioADescriptors[] =
{
    /* CSR, *, *, BLUETOOTH */
    {0x032A, CSR_SDIO_ANY_CARD_ID, CSR_SDIO_ANY_SDIO_FUNCTION, 2}, /* all CSR bluetooth devices */
};

/* call back function given to CsrSdio */
static void sdioInsertCbf(CsrSdioFunction *f);
static void sdioRemoveCbf(CsrSdioFunction *f);
static CsrSdioInterruptDsrCallback sdioInterruptCbf(CsrSdioFunction *f);

static void sdioInterruptHighLevel(CsrSdioFunction *f);

static CsrSdioFunctionDriver sdioAFunctionDriver =
{
    sdioInsertCbf,
    sdioRemoveCbf,
    sdioInterruptCbf,
    NULL /* typeaSuspendCbf */,
    NULL /* typeaResumeCbf */,
    sdioADescriptors,
    sizeof(sdioADescriptors) / sizeof(sdioADescriptors[0]),
    NULL
};

static const CsrUint8 gWarmResetCmd[26] = {0x1a, 0x00, 0x00, 0x01, 0x00, 0xfc, 0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* local defines */
#define MAX_RETRIES 3

/* defines for sleep state */
#define SDIO_REG_IO_READY 0x03
#define SDIO_REG_CSR_SLEEP_STATE 0xf0
#define SDIO_REG_CSR_SLEEP_STATE_FUNC(f) (((f)->sdioId.sdioFunction) << 4)
#define SDIO_REG_CSR_SLEEP_STATE_RDY_INT_EN  (1 << 1)
#define SDIO_REG_CSR_SLEEP_STATE_WAKE_REQ    (1 << 0)

/* Wake up, prohibit sleeping, normal interrupt operation. */
#define SLEEP_STATE_AWAKE   (SDIO_REG_CSR_SLEEP_STATE_WAKE_REQ)
/* Wake up and signal interrupt for verification. */
#define SLEEP_STATE_DROWSY  (SDIO_REG_CSR_SLEEP_STATE_WAKE_REQ | SDIO_REG_CSR_SLEEP_STATE_RDY_INT_EN)
/* Permit sleeping, normal interrupt operation. */
#define SLEEP_STATE_SLEEP   (0x0)

#define SDIO_REG_CSR_HOST_INT 0xf1
#define SDIO_REG_CSR_HOST_INT_CL 0x01

#define SDIO_REG_CSR_TO_HOST_SCRATCH0 0xf4
#define SDIO_REG_CSR_TO_HOST_SCRATCH0_INITED 0x1

#define SDIO_REG_CSR_EXT_IO_EN 0xf6

#define SDIO_REG_BT_TYPE_A_RD 0x00
#define SDIO_REG_BT_TYPE_A_TD 0x00

#define SDIO_REG_BT_TYPE_A_RX_PKT_CTRL 0x10
#define PC_RRT_NEXT     0x00
#define PC_RRT_RETRY    0x01

#define SDIO_REG_BT_TYPE_A_TX_PKT_CTRL 0x11
#define PC_WRT_RETRY    0x01

#define SDIO_REG_BT_TYPE_A_INTRD       0x13
#define INTRD 0x01
#define CL_INTRD 0x01

#define SDIO_REG_BT_TYPE_A_INT_EN      0x14
#define EN_INTRD 0x01

#define SDIO_BT_TYPE_A_HEADER_LEN         4
#define SDIO_BT_TYPE_A_PACKET_LEN_MAX (4096) /* BC6 got 4k as max MMU buffer */

/* Background interrupt handlers */
static void sdioABgintCardEvent(void *arg);
static void sdioABgintIntr(void *arg);


/* SDIO completion callback function prototypes */


/* TX */
static void sdioATxHighLvlComplete(CsrSdioFunction *f, CsrResult result);

static CsrSdioAsyncDsrCallback sdioATxComplete(
    CsrSdioFunction *f,
    CsrResult result);

static CsrSdioAsyncDsrCallback sdioATxRetryComplete(
    CsrSdioFunction *f,
    CsrResult result);

static void sdioATxWarmResetWakeHighLvlComplete(CsrSdioFunction *f,
    CsrResult result);

static CsrSdioAsyncDsrCallback sdioATxWarmResetWakeComplete(
    CsrSdioFunction *f,
    CsrResult result);

static CsrSdioAsyncDsrCallback sdioATxWarmResetComplete(
    CsrSdioFunction *f,
    CsrResult result);


/* RX */

static CsrSdioAsyncDsrCallback sdioARxBufCompletion(CsrSdioAInstanceData *instData);

static void sdioARxBufCompleteHighLvl(CsrSdioFunction *f,
    CsrResult result);

static CsrSdioAsyncDsrCallback sdioARxHdrComplete(
    CsrSdioFunction *f,
    CsrResult result);

static CsrSdioAsyncDsrCallback sdioARxRetryComplete(
    CsrSdioFunction *f,
    CsrResult result);

static void sdioARxClrNoIntrHighLvlComplete(CsrSdioFunction *f,
    CsrResult result);

static CsrSdioAsyncDsrCallback sdioARxClrNoIntrComplete(CsrSdioFunction *f,
    CsrResult result);

static CsrSdioAsyncDsrCallback sdioARxClrIntrComplete(CsrSdioFunction *f,
    CsrResult result);

static CsrSdioAsyncDsrCallback sdioARxRdIntrComplete(CsrSdioFunction *f,
    CsrResult result);

static CsrSdioAsyncDsrCallback sdioARxAckComplete(CsrSdioFunction *f,
    CsrResult result);

static CsrSdioAsyncDsrCallback sdioARxBufComplete(CsrSdioFunction *f,
    CsrResult result);


/* Chip init */

static void sdioAChipStartHighLvlComplete(CsrSdioFunction *f,
    CsrResult result);

static CsrSdioAsyncDsrCallback sdioAChipStartComplete(CsrSdioFunction *f,
    CsrResult result);


/* Deep sleep */
#if CSR_TYPE_A_SLEEP_TIMEOUT > 0
static void sdioASleepSetCompleteHighLvl(CsrSdioFunction *f,
    CsrResult result);

static CsrSdioAsyncDsrCallback sdioASleepSetComplete(CsrSdioFunction *f,
    CsrResult result);
#endif

/* Misc */

static void sdioAEnterTxPacketS(CsrSdioAInstanceData *instData);
static void sdioAEnterRxPacketS(CsrSdioAInstanceData *instData);

static void sdioABgintPanic(void *arg)
{
    CSR_UNUSED(arg);

    CsrTmBluecoreResetIndSend();
}

static void sdioAPanicHighLvl(CsrSdioFunction *f, CsrResult result)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) f->driverData;

    CsrSchedBgintSet(instData->bgint_panic);
}

static CsrSdioAsyncDsrCallback sdioAPanic(CsrSdioAInstanceData *instData)
{
    CsrSdioAsyncDsrCallback cb;

    cb = sdioAPanicHighLvl;

    return cb;
}

static CsrUint32 sdioBtTypeAPacketLen(const CsrUint8 *p)
{
    CsrUint32 ret;

    ret = p[2];
    ret <<= 8;
    ret |= p[1];
    ret <<= 8;
    ret |= p[0];

    return ret;
}

static CsrInt32 sdioAValidRxLen(const CsrUint32 len)
{
    CsrInt32 retVal = 0;

    if ((len >= SDIO_BT_TYPE_A_HEADER_LEN)
        && (len < SDIO_BT_TYPE_A_PACKET_LEN_MAX))
    {
        retVal = 1;
    }
    return retVal;
}

/*
 * SDIO function event handlers
 */

static void sdioInsertCbf(CsrSdioFunction *f)
{
    CsrSdioBtACardEvent(f, TRUE);
}

static void sdioRemoveCbf(CsrSdioFunction *f)
{
    CsrSdioBtACardEvent(f, FALSE);
}

static CsrSdioInterruptDsrCallback sdioInterruptCbf(CsrSdioFunction *f)
{
    CsrSdioAInstanceData *instData;
    CsrSdioInterruptDsrCallback cb;

    instData = (CsrSdioAInstanceData *) f->driverData;

    if ((InitS == instData->state) || (RstS == instData->state))
    {
        cb = sdioInterruptHighLevel;
    }
    else
    {
        cb = NULL;
        /* Schedule read sequence. */
        sdioAEnterRxPacketS(instData);
    }

    return cb;
}

static void sdioInterruptHighLevel(CsrSdioFunction *f)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) f->driverData;

    CsrSchedBgintSet(instData->bgint_intr);
}

/* -------------------------------------------------------------------- */

static void sdioADelayedIntrEnable(CsrUint16 unused, void *ptr)
{
    CsrSdioAInstanceData *instData;
    CsrResult retVal;

    instData = (CsrSdioAInstanceData *) ptr;

    instData->timer = CSR_SCHED_TID_INVALID;

    CsrSdioInterruptDisable(instData->sdioHandle);
    retVal = CsrSdioInterruptEnable(instData->sdioHandle);
    if (retVal == CSR_RESULT_SUCCESS)
    {
        instData->curChipSleepState = SLEEP_STATE_AWAKE;
        instData->chipState = CHST_FUNCEN;

        CsrTypeAF0Write8(instData->sdioHandle,
            SDIO_REG_CSR_SLEEP_STATE,
            (CsrUint8) (SDIO_REG_CSR_SLEEP_STATE_FUNC(instData->sdioHandle)
                        | instData->curChipSleepState),
            sdioAChipStartComplete);
    }
    else
    {
        CsrSchedBgintSet(instData->bgint_panic);
    }
}

/*
 * This is called when a packet read has been fully completed
 * and, if required, acknowledged.  It passes the packet to the
 * stack, resets state, and checks for more work.
 */
static void sdioARxBufCompleteHighLvl(CsrSdioFunction *f, CsrResult result)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) f->driverData;

    CsrSchedBgintSet(instData->bgint_rxdone);
}

/*
 * This is called when a packet read acknowledgement has completed.
 * Dispatch handling to the high level RX handler.
 */
static CsrSdioAsyncDsrCallback sdioARxAckComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAsyncDsrCallback cb;
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) f->driverData;

    cb = sdioARxBufCompletion(instData);

    return cb;
}

/*
 * This is called when a read acknowledgement for an empty
 * read buffer completes.  It deliberately does nothing but
 * acknowledge the interrupt.
 */
static CsrSdioAsyncDsrCallback sdioARxAckEmptyComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioInterruptAcknowledge(f);

    return NULL;
}

static CsrSdioAsyncDsrCallback sdioARxBufComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAInstanceData *instData;
    CsrSdioAsyncDsrCallback cb;

    instData = (CsrSdioAInstanceData *) f->driverData;

    cb = NULL;

    if (CSR_RESULT_SUCCESS == result)
    {
        if (instData->readNeedAck)
        {
            instData->rxTries = 1;

            CsrTypeAWrite8(instData->sdioHandle,
                SDIO_REG_BT_TYPE_A_RX_PKT_CTRL,
                PC_RRT_NEXT,
                sdioARxAckComplete);
        }
        else
        {
            cb = sdioARxBufCompletion(instData);
        }
    }
    else /* failed in reading data */
    {
        if (instData->rxTries < instData->maxRxTries)
        {
            instData->rxTries++;

            CsrTypeAWrite8(instData->sdioHandle,
                SDIO_REG_BT_TYPE_A_RX_PKT_CTRL,
                PC_RRT_RETRY,
                sdioARxRetryComplete);
        }
        else
        {
            cb = sdioAPanic(instData);
        }
    }
    return cb;
}

static CsrSdioAsyncDsrCallback sdioARxHdrComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAInstanceData *instData;
    CsrSdioAsyncDsrCallback cb;

    instData = (CsrSdioAInstanceData *) f->driverData;
    cb = NULL;

    if (CSR_RESULT_SUCCESS == result)
    {
        CsrUint32 sdioALen;

        instData->rxTries = 1;

        /* header has been read */
        sdioALen = sdioBtTypeAPacketLen(instData->rxMsg.hdr);

        if (0 != sdioALen)
        {
            /* We've got data */
            if (sdioAValidRxLen(sdioALen))
            {
                CsrUint16 payloadLen;
                CsrInt16 payloadRemainder;

                /* Convert Type-A service ID to Synergy channel ID */
                if ((instData->rxMsg.hdr[3] == SDIO_CHANNEL_HCI) ||
                    (instData->rxMsg.hdr[3] == SDIO_CHANNEL_HCI_EVT))
                {
                    instData->rxMsg.cha = TRANSPORT_CHANNEL_HCI;
                }
                else if (instData->rxMsg.hdr[3] == SDIO_CHANNEL_ACL)
                {
                    instData->rxMsg.cha = TRANSPORT_CHANNEL_ACL;
                }
                else if (instData->rxMsg.hdr[3] == SDIO_CHANNEL_SCO)
                {
                    instData->rxMsg.cha = TRANSPORT_CHANNEL_SCO;
                }
                else
                {
                    TYPE_A_PANIC(CSR_PANIC_FW_UNEXPECTED_VALUE,
                        "type a invalid channel");
                }

                /* Subtract Type-A header size */
                instData->rxMsg.bufLen = (CsrUint16) (sdioALen - 4);

                /* We already read the HCI/ACL header so subtract hdrsz. */
                payloadLen = instData->rxMsg.bufLen - 4;

                payloadRemainder = payloadLen - TYPEA_PEEK_BYTES;

#ifdef CSR_ACLBUF_ENABLE
                if (TRANSPORT_CHANNEL_ACL == instData->rxMsg.cha)
                {
                    CsrUint16 aclHdlFlags;
                    CsrUint8 rv;

                    aclHdlFlags = instData->rxMsg.hdr[5] << 8;
                    aclHdlFlags |= instData->rxMsg.hdr[4];

                    rv = CsrAclBufAlloc(aclHdlFlags, payloadLen,
                        instData->rxMsg.hdr + 8,
                        TYPEA_PEEK_BYTES,
                        &instData->rxMsg.bufCtx,
                        &instData->rxMsg.buf);
                    if (CSR_ACLBUF_SUCCESS == rv)
                    {
                        CsrInt32 i;

                        instData->rxMsg.aclBuf = TRUE;

                        instData->rxMsg.bufLen = payloadLen;

                        /* Copy back any peeked bytes. */
                        for (i = 0;
                             i < CSRMIN(TYPEA_PEEK_BYTES, payloadLen);
                             i++)
                        {
                            instData->rxMsg.buf[i] =
                                instData->rxMsg.hdr[8 + i];
                        }

                        if (payloadRemainder > 0)
                        {
                            CsrTypeARead(instData->sdioHandle,
                                SDIO_REG_BT_TYPE_A_RD,
                                instData->rxMsg.buf + TYPEA_PEEK_BYTES,
                                /* peek already read so only read remainder */
                                payloadRemainder,
                                sdioARxBufComplete);
                        }
                        else
                        {
                            if (instData->readNeedAck)
                            {
                                CsrTypeAWrite8(
                                    instData->sdioHandle,
                                    SDIO_REG_BT_TYPE_A_RX_PKT_CTRL,
                                    PC_RRT_NEXT,
                                    sdioARxAckComplete);
                            }
                            else
                            {
                                /*
                                 * No ack needed -- fake it, and return a
                                 * callback.
                                 */
                                cb = sdioARxBufCompletion(instData);
                            }
                        }
                    }
                    else
                    {
                        instData->rxMsg.aclBuf = FALSE;

                        if (CSR_ACLBUF_NONE == rv)
                        {
                            /*
                             * No one subscribed to this channel, so do a
                             * regular allocation and copy in header.
                             */
                            CsrInt32 i;

                            instData->rxMsg.buf = CsrMemBufAlloc(
                                instData->rxPool,
                                NULL,
                                &instData->rxMsg.bufCtx);

                            instData->rxMsg.buf[0] = instData->rxMsg.hdr[4];
                            instData->rxMsg.buf[1] = instData->rxMsg.hdr[5];
                            instData->rxMsg.buf[2] = instData->rxMsg.hdr[6];
                            instData->rxMsg.buf[3] = instData->rxMsg.hdr[7];

                            for (i = 0;
                                 i < CSRMIN(TYPEA_PEEK_BYTES, payloadLen);
                                 i++)
                            {
                                instData->rxMsg.buf[4 + i] =
                                    instData->rxMsg.hdr[8 + i];
                            }

                            if (payloadRemainder > 0)
                            {
                                CsrTypeARead(instData->sdioHandle,
                                    SDIO_REG_BT_TYPE_A_RD,
                                    instData->rxMsg.buf + 4 + TYPEA_PEEK_BYTES,
                                    payloadRemainder, /* peek already read */
                                    sdioARxBufComplete);
                            }
                            else
                            {
                                if (instData->readNeedAck)
                                {
                                    CsrTypeAWrite8(
                                        instData->sdioHandle,
                                        SDIO_REG_BT_TYPE_A_RX_PKT_CTRL,
                                        PC_RRT_NEXT,
                                        sdioARxAckComplete);
                                }
                                else
                                {
                                    /*
                                     * No ack needed -- fake it, and return a
                                     * callback.
                                     */
                                    cb = sdioARxBufCompletion(instData);
                                }
                            }
                        }
                        else if (CSR_ACLBUF_NOMEM == rv)
                        {
                            TYPE_A_PANIC(CSR_PANIC_FW_HEAP_EXHAUSTION,
                                "ACL-buffering allocation failed");
                        }
                    }
                }
                else
#endif
                {
                    /* Not ACL data */
                    CsrInt32 i;

#ifdef CSR_ACLBUF_ENABLE
                    instData->rxMsg.aclBuf = FALSE;
#endif

                    instData->rxMsg.buf = CsrMemBufAlloc(instData->rxPool,
                        NULL,
                        &instData->rxMsg.bufCtx);

                    /* Copy in header + peek */
                    instData->rxMsg.buf[0] = instData->rxMsg.hdr[4];
                    instData->rxMsg.buf[1] = instData->rxMsg.hdr[5];
                    instData->rxMsg.buf[2] = instData->rxMsg.hdr[6];
                    instData->rxMsg.buf[3] = instData->rxMsg.hdr[7];

                    for (i = 0;
                         i < CSRMIN(TYPEA_PEEK_BYTES, payloadLen);
                         i++)
                    {
                        instData->rxMsg.buf[4 + i] =
                            instData->rxMsg.hdr[8 + i];
                    }

                    if (payloadRemainder > 0)
                    {
                        CsrTypeARead(instData->sdioHandle,
                            SDIO_REG_BT_TYPE_A_RD,
                            instData->rxMsg.buf + 4 + TYPEA_PEEK_BYTES,
                            payloadRemainder, /* HCI hdr etc. already read */
                            sdioARxBufComplete);
                    }
                    else
                    {
                        if (instData->readNeedAck)
                        {
                            CsrTypeAWrite8(
                                instData->sdioHandle,
                                SDIO_REG_BT_TYPE_A_RX_PKT_CTRL,
                                PC_RRT_NEXT,
                                sdioARxAckComplete);
                        }
                        else
                        {
                            /*
                             * No ack needed -- fake it, and return a
                             * callback.
                             */
                            cb = sdioARxBufCompletion(instData);
                        }
                    }
                }
            }
            else if (instData->rxTries < instData->maxRxTries)
            {
                instData->rxTries++;

                /* try again */
                CsrTypeAWrite8(instData->sdioHandle,
                    SDIO_REG_BT_TYPE_A_RX_PKT_CTRL,
                    PC_RRT_RETRY,
                    sdioARxRetryComplete);
            }
            else
            {
                cb = sdioAPanic(instData);
            }
        }
        else
        {
            /* No actual data for us */
            if (instData->readNeedAck)
            {
                instData->rxTries = 1;

                CsrTypeAWrite8(instData->sdioHandle,
                    SDIO_REG_BT_TYPE_A_RX_PKT_CTRL,
                    PC_RRT_NEXT,
                    sdioARxAckEmptyComplete);
            }
            else
            {
                /* Nothing more to do -- just unmask interrupt. */
                CsrSdioInterruptAcknowledge(instData->sdioHandle);
            }
        }
    }
    /* Request failed -- check retry count */
    else if (instData->rxTries < instData->maxRxTries)
    {
        instData->rxTries++;

        CsrTypeARead(instData->sdioHandle,
            SDIO_REG_BT_TYPE_A_RD,
            instData->rxMsg.hdr,
            SDIO_BT_TYPE_A_HEADER_LEN,
            sdioARxHdrComplete);
    }
    else
    {
        cb = sdioAPanic(instData);
    }

    return cb;
}

static CsrSdioAsyncDsrCallback sdioARxRetryComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAInstanceData *instData;
    CsrSdioAsyncDsrCallback cb;

    instData = (CsrSdioAInstanceData *) f->driverData;

    cb = NULL;

    if (CSR_RESULT_SUCCESS == result)
    {
        instData->rxTries++;

        /* Retry request completed -- read header again */
        CsrTypeARead(instData->sdioHandle,
            SDIO_REG_BT_TYPE_A_RD,
            instData->rxMsg.hdr,
            SDIO_BT_TYPE_A_HEADER_LEN,
            sdioARxHdrComplete);
    }
    else
    {
        if (instData->rxTries < instData->maxRxTries)
        {
            instData->rxTries++;

            /* Retry write failed - try again */
            CsrTypeAWrite8(instData->sdioHandle,
                SDIO_REG_BT_TYPE_A_RX_PKT_CTRL,
                PC_RRT_RETRY,
                sdioARxRetryComplete);
        }
        else
        {
            cb = sdioAPanic(instData);
        }
    }

    return cb;
}

/* High level completion handler for bogus interrupts. */
static void sdioARxClrNoIntrHighLvlComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioInterruptAcknowledge(f);
}

/* Called when clearing a seemingly bogus interrupt. */
static CsrSdioAsyncDsrCallback sdioARxClrNoIntrComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAsyncDsrCallback cb;

    cb = sdioARxClrNoIntrHighLvlComplete;

    return cb;
}

/* Called when clearing the packet read ready flag */
static CsrSdioAsyncDsrCallback sdioARxClrIntrComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAInstanceData *instData;
    CsrSdioAsyncDsrCallback cb;

    instData = (CsrSdioAInstanceData *) f->driverData;

    cb = NULL;

    if (CSR_RESULT_SUCCESS == result)
    {
        /* We're handling one now.*/
        instData->rxTries = 1;

        CsrTypeARead(instData->sdioHandle,
            SDIO_REG_BT_TYPE_A_RD,
            instData->rxMsg.hdr,
            /* Read out SDIO and HCI headers + TYPEA_PEEK_BYTES bytes */
            8 + TYPEA_PEEK_BYTES,
            sdioARxHdrComplete);
    }
    else if (instData->rxTries < instData->maxRxTries)
    {
        instData->rxTries++;

        CsrTypeAWrite8(instData->sdioHandle,
            SDIO_REG_BT_TYPE_A_INTRD,
            CL_INTRD,
            sdioARxClrIntrComplete);
    }
    else
    {
        cb = sdioAPanic(instData);
    }


    return cb;
}

/* Called when reading INTRD completes. */
static CsrSdioAsyncDsrCallback sdioARxRdIntrComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAInstanceData *instData;
    CsrSdioAsyncDsrCallback cb;

    instData = (CsrSdioAInstanceData *) f->driverData;

    cb = NULL;

    if (CSR_RESULT_SUCCESS == result)
    {
        if (instData->readBuf8 == INTRD)
        {
            /* BT Type-A interrupt */
            CsrTypeAWrite8(instData->sdioHandle,
                SDIO_REG_BT_TYPE_A_INTRD,
                CL_INTRD,
                sdioARxClrIntrComplete);
        }
        else
        {
#if 1
            /* unexpected interrupt - clear it */
            CsrTypeAF0Write8(instData->sdioHandle,
                SDIO_REG_CSR_HOST_INT,
                SDIO_REG_CSR_HOST_INT_CL,
                sdioARxClrNoIntrComplete);
#else
            return &sdioARxClrNoIntrHighLvlComplete;
#endif
        }
    }
    else if (instData->rxTries < instData->maxRxTries)
    {
        instData->rxTries++;

        CsrTypeARead8(instData->sdioHandle,
            SDIO_REG_BT_TYPE_A_INTRD,
            &instData->readBuf8,
            sdioARxRdIntrComplete);
    }
    else
    {
#if 0
        cb = sdioAPanic(instData);
#endif
    }

    return cb;
}

static CsrSdioAsyncDsrCallback sdioARxBufCompletion(CsrSdioAInstanceData *instData)
{
    CsrSdioAsyncDsrCallback cb;
#ifdef CSR_ACLBUF_ENABLE
    CsrUint16 aclHdlFlags;

    aclHdlFlags = instData->rxMsg.hdr[5] << 8;
    aclHdlFlags |= instData->rxMsg.hdr[4];

    if (instData->rxMsg.aclBuf == TRUE)
    {
        CsrAclBufCb aclcb;
        aclcb = CsrAclBufDeliver(aclHdlFlags, instData->rxMsg.bufCtx,
            instData->rxMsg.bufLen);

        if (NULL != aclcb)
        {
            instData->aclbufcb = aclcb;
            cb = sdioARxBufCompleteHighLvl;
        }
        else
        {
            cb = NULL;
        }
        /* Unmask the interrupt now that we've serviced it. */
        CsrSdioInterruptAcknowledge(instData->sdioHandle);
    }
    else
#endif
    {
        /* rx - done */
        CsrMemBufQueue(instData->rxPool, instData->rxMsg.bufCtx,
            instData->rxMsg.bufLen, instData->rxMsg.cha);

        cb = sdioARxBufCompleteHighLvl;
        /* Unmask intr later. */
    }

    /* Invalidate these so we don't mess up during deinit */
    instData->rxMsg.buf = NULL;
    instData->rxMsg.bufCtx = NULL;

    return cb;
}

/*
 * Called from sdioInterruptCbf and as
 * the last step of chip initialization.
 */
static void sdioAEnterRxPacketS(CsrSdioAInstanceData *instData)
{
    instData->rxTries = 1;
    CsrTypeARead8(instData->sdioHandle,
        SDIO_REG_BT_TYPE_A_INTRD,
        &instData->readBuf8,
        sdioARxRdIntrComplete);
}

static void sdioAEnterTxPacketS(CsrSdioAInstanceData *instData)
{
    CsrInt32 retVal;
    CsrInt32 normalPkt = 0;
    CsrUint32 i;

    retVal = CsrTypeATxQueueMessageGet(&instData->txMsg);

    if (0 == retVal)
    {
        /* Log the outgoing packet. */
        CSR_LOG_TRANSPORT(TRANSPORT_TYPE_TYPE_A,
            CSR_LOG_TRANSPORT_OUTGOING,
            instData->txMsg->bufLen,
            instData->txMsg->bufLen,
            instData->txMsg->buf);

        instData->pendingTxPackets--;

        if (instData->txMsg->bufLen > SDIO_BT_TYPE_A_HEADER_LEN)
        {
            /* check for cold and warm reset - msg */
            for (i = 0; i < sizeof(gWarmResetCmd); i++)
            {
                /* don't check byte 12 and 13 - seq no */
                if ((i != 12) && (i != 13) && (instData->txMsg->buf[i] != gWarmResetCmd[i]))
                {
                    normalPkt = 1; /* diff found */
                    break;
                }
            }

            if (normalPkt)
            {
                instData->txTries = 0;

                instData->state = TxS;

                instData->txTries = 1;

                CsrTypeAWrite(instData->sdioHandle,
                    SDIO_REG_BT_TYPE_A_TD,
                    (CsrUint8 *) instData->txMsg->buf,
                    instData->txMsg->bufLen,
                    sdioATxComplete);
            }
            else
            {
                instData->state = RstS;
                instData->txTries = 1;
                instData->eventAndIrq = 0x0;

                CsrTypeAWrite(instData->sdioHandle,
                    SDIO_REG_BT_TYPE_A_TD,
                    (CsrUint8 *) instData->txMsg->buf,
                    instData->txMsg->bufLen,
                    sdioATxWarmResetComplete);
            }
        }
        else
        {
            /*
             * We're being fed broken data.  Just handle it
             * gracefully by tossing the packet.
             */
            CsrTypeATxQueueMessageRemove(&instData->txMsg);

            /*
             * In principle, we're done now so check for
             * more work.
             */
            sdioAEnterTxPacketS(instData);
        }
    }
    else
    {
        TYPE_A_PANIC(CSR_PANIC_FW_UNEXPECTED_VALUE,
            "tx queue unexpectedly empty");
    }
}

/*
 * This is called when a write retry request completes.
 */
static CsrSdioAsyncDsrCallback sdioATxRetryComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAInstanceData *instData;
    CsrSdioAsyncDsrCallback cb;

    instData = (CsrSdioAInstanceData *) f->driverData;

    cb = NULL;

    if (CSR_RESULT_SUCCESS == result)
    {
        instData->txTries++;

        CsrTypeAWrite(instData->sdioHandle,
            SDIO_REG_BT_TYPE_A_TD,
            (CsrUint8 *) instData->txMsg->buf,
            instData->txMsg->bufLen, sdioATxComplete);
    }
    else
    {
        if (instData->txTries < instData->maxTxTries)
        {
            instData->txTries++;

            /* Tx failed - let's prepare for another try */
            CsrTypeAWrite8(instData->sdioHandle,
                SDIO_REG_BT_TYPE_A_TX_PKT_CTRL,
                PC_WRT_RETRY,
                sdioATxRetryComplete);
        }
        else
        {
            cb = sdioAPanic(instData);
        }
    }

    return cb;
}

/*
 * This is called when a wake command write request following
 * a warm reset completes.
 */
static void sdioATxWarmResetWakeHighLvlComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) f->driverData;

    if (CSR_RESULT_SUCCESS == result)
    {
        instData->eventAndIrq |= TYPEA_WUE_CMPL;

        /* warm reset - free msg */
        CsrTypeATxQueueMessageRemove(&instData->txMsg);

        /* Nothing more to do right now -- wait for reinit. */
    }
    else
    {
        /* chip is in deep sleep */
        instData->eventAndIrq = 0x0;
    }
}

static CsrSdioAsyncDsrCallback sdioATxWarmResetWakeComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAsyncDsrCallback cb;

    cb = sdioATxWarmResetWakeHighLvlComplete;

    return cb;
}

/*
 * This is called when the warm reset command write request completes.
 */
static CsrSdioAsyncDsrCallback sdioATxWarmResetComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) f->driverData;

    /*
     * Ensure we are listening to interrupts.  The reset may do
     * strange things beneath our feet.
     */
    CsrSdioInterruptAcknowledge(instData->sdioHandle);

    instData->curChipSleepState = SLEEP_STATE_AWAKE;
    CsrTypeAF0Write8(instData->sdioHandle,
        SDIO_REG_CSR_SLEEP_STATE,
        (CsrUint8) (SDIO_REG_CSR_SLEEP_STATE_FUNC(instData->sdioHandle)
                    | instData->curChipSleepState),
        sdioATxWarmResetWakeComplete);

    return NULL;
}

static void sdioATxHighLvlComplete(CsrSdioFunction *f, CsrResult result)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) f->driverData;

    /* Complete the TX */
    CsrSchedBgintSet(instData->bgint_txdone);
}

/*
 * This is called when a write request completes.
 */
static CsrSdioAsyncDsrCallback sdioATxComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAsyncDsrCallback cb;
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) f->driverData;

    if (CSR_RESULT_SUCCESS != result)
    {
        if (instData->txTries < instData->maxTxTries)
        {
            instData->txTries++;

            /* TX failed - let's prepare for another try */
            CsrTypeAWrite8(instData->sdioHandle,
                SDIO_REG_BT_TYPE_A_TX_PKT_CTRL,
                PC_WRT_RETRY,
                sdioATxRetryComplete);

            cb = NULL;
        }
        else
        {
            cb = sdioAPanic(instData);
        }
    }
    else
    {
        cb = sdioATxHighLvlComplete;
    }

    return cb;
}

static void sdioAChipStartHighLvlComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) f->driverData;

    instData->eventAndIrq |= TYPEA_WUE_CMPL;

    CsrSchedBgintSet(instData->bgint_cardevent);
}

static CsrSdioAsyncDsrCallback sdioAChipStartComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAsyncDsrCallback cb;

    cb = sdioAChipStartHighLvlComplete;

    return cb;
}

#if CSR_TYPE_A_SLEEP_TIMEOUT > 0
static void sdioASleepSetCompleteHighLvl(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) f->driverData;

    if (WakingS == instData->state)
    {
        CsrSchedBgintSet(instData->bgint_txdone);
    }
    else
    {
        /* A completion for a sleep permit command. */
    }
}

static CsrSdioAsyncDsrCallback sdioASleepSetComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAsyncDsrCallback cb;

    cb = sdioASleepSetCompleteHighLvl;

    return cb;
}

static void sdioAWakePollCompleteHighLvl(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) f->driverData;

    /* If the command fails, have another go. */
    if (CSR_RESULT_SUCCESS != result)
    {
        instData->readyState = 0;
    }

    CsrSchedBgintSet(instData->bgint_txdone);
}

static CsrSdioAsyncDsrCallback sdioAWakePollComplete(CsrSdioFunction *f,
    CsrResult result)
{
    CsrSdioAsyncDsrCallback cb;

    cb = sdioAWakePollCompleteHighLvl;

    return cb;
}

#endif

/*
 * If this timer triggers, the chip is probably in
 * deep sleep.  Attempt to wake it up.
 */
static void sdioATimerWAIT(CsrUint16 unused, void *ptr)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) ptr;

    instData->chipState = CHST_DSWAIT;
    instData->eventAndIrq = 0x0;

    instData->curChipSleepState = SLEEP_STATE_DROWSY;
    CsrTypeAF0Write8(instData->sdioHandle,
        SDIO_REG_CSR_SLEEP_STATE,
        (CsrUint8) (SDIO_REG_CSR_SLEEP_STATE_FUNC(instData->sdioHandle)
                    | instData->curChipSleepState),
        sdioAChipStartComplete);

    CsrSdioInterruptAcknowledge(instData->sdioHandle);
}

/*
 * If this timer triggers, the chip is probably in
 * deep sleep.  Attempt to wake it up.
 */
static void sdioATimerDSWAIT(CsrUint16 unused, void *ptr)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) ptr;

    instData->chipState = CHST_DS;
    instData->eventAndIrq = 0x0;

    instData->curChipSleepState = SLEEP_STATE_DROWSY;
    CsrTypeAF0Write8(instData->sdioHandle,
        SDIO_REG_CSR_SLEEP_STATE,
        (CsrUint8) (SDIO_REG_CSR_SLEEP_STATE_FUNC(instData->sdioHandle)
                    | instData->curChipSleepState),
        sdioAChipStartComplete);
}

/*
 * If this timer triggers, communication with the chip
 * has failed completely.  Panic.
 */
static void sdioATimerDS(CsrUint16 unused1, void *unused2)
{
    CsrTmBluecoreResetIndSend();
}

#if CSR_TYPE_A_SLEEP_TIMEOUT > 0
/*
 * This timer is used to permit the chip to
 * enter deep sleep.  It is set whenever the
 * TX part of the driver becomes idle.
 */
static void sdioATimerSleepEnable(CsrUint16 unused, void *arg)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) arg;

    instData->state = SleepS;
    instData->timer = CSR_SCHED_TID_INVALID;

    CsrTypeAF0Write8(instData->sdioHandle,
        SDIO_REG_CSR_SLEEP_STATE,
        (CsrUint8) (SDIO_REG_CSR_SLEEP_STATE_FUNC(instData->sdioHandle)
                    | SLEEP_STATE_SLEEP),
        sdioASleepSetComplete);
}

static void sdioATimerWakeup(CsrUint16 unused, void *arg)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) arg;

    instData->state = IdleS;

    CsrSchedBgintSet(instData->bgint_txdone);
}

#endif

/* May only be called from bgint ctx. */

static void sdioABgintIntr(void *arg)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) arg;

    /* Dispatch interrupt handling based on state */
    switch (instData->state)
    {
        case TxS:
            /*
             * We're busy doing other things now but
             * when they finish, they will check for
             * more work, so don't do anything other
             * than count this intr.
             */
            break;

        case RstS:
            instData->eventAndIrq |= TYPEA_WUE_INTR;
            break;

        case InitS:
            /*
             * In InitS all we do is flag the intr and
             * set the bgint.
             */
            instData->eventAndIrq |= TYPEA_WUE_INTR;
            sdioABgintCardEvent(instData);
            break;
        default:
            /* Nothing. */
            break;
    }
}

/*
 * Bgint handlers
 */
static void sdioABgintRxDone(void *arg)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) arg;

    /* copy rxMsg, start new operation, and pass to Rx */

#ifdef CSR_ACLBUF_ENABLE
    if (NULL == instData->aclbufcb)
    {
#endif
    CsrTypeARxQueueMsgPut(instData->rxPool);
    /* Now we can unmask the intr. */
    CsrSdioInterruptAcknowledge(instData->sdioHandle);
#ifdef CSR_ACLBUF_ENABLE
}
else
{
    CsrAclBufCb aclcb;
    aclcb = instData->aclbufcb;
    instData->aclbufcb = NULL;
    aclcb();
}
#endif
}

/*
 * This bgint is used to handle the completion
 * of a packet TX.
 * May only be called from bgint ctx.
 */
static void sdioABgintTxDone(void *arg)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) arg;

    if (TxS == instData->state)
    {
        /* done - TX succes -- free msg */
        CsrTypeATxQueueMessageRemove(&instData->txMsg);
        instData->state = IdleS;
    }
#if CSR_TYPE_A_SLEEP_TIMEOUT > 0
    else if (WakingS == instData->state)
    {
        if (instData->readyState & (1 << instData->sdioHandle->sdioId.sdioFunction))
        {
            instData->timer = CsrSchedTimerSet(CSR_TYPE_A_WAKEUP_TIMEOUT *
                TYPE_A_TIME_MS,
                sdioATimerWakeup, 0, instData);
        }
        else
        {
            /* Poll IO Ready until we're alive. */
            CsrTypeAF0Read8(instData->sdioHandle,
                SDIO_REG_IO_READY,
                &instData->readyState,
                sdioAWakePollComplete);
        }
    }
#endif


    /* Check for more data to send. */
    if (IdleS == instData->state)
    {
        if (instData->pendingTxPackets)
        {
            sdioAEnterTxPacketS(instData);
        }
        else
        {
            /* Nothing else to do so idle */

#if CSR_TYPE_A_SLEEP_TIMEOUT > 0
            instData->timer = CsrSchedTimerSet(CSR_TYPE_A_SLEEP_TIMEOUT *
                TYPE_A_TIME_MS,
                sdioATimerSleepEnable, 0, instData);
#endif
        }
    }
}

/* May only be called from bgint ctx. */
static void sdioABgintCardEvent(void *arg)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) arg;

    if (WaitForInsertS == instData->state)
    {
        if (CsrSdioAProtSetChipQuirks(instData) == TRUE)
        {
            CsrSdioInsertedAcknowledge(instData->sdioHandle, CSR_RESULT_SUCCESS);

            /*
             * SDIO will do N block transfers plus a number
             * of byte mode transfers to transfer a given
             * amount of data on the bus.  A byte mode transfer
             * has a maximum length of 512 bytes.  Thus, a
             * block size of 512 ensures that we limit the bus
             * operations to N * block mode + 1 * byte mode ops.
             */
            CsrSdioBlockSizeSet(instData->sdioHandle, 512);

            /* If this fails, we limp on. */
            CsrSdioMaxBusClockFrequencySet(instData->sdioHandle,
                CSR_TYPE_A_BUSSPEED_AWAKE);
        }
        else
        {
            CsrSdioInsertedAcknowledge(instData->sdioHandle, CSR_RESULT_FAILURE);

            return;
        }

        instData->chipState = CHST_UNK;
        instData->state = InitS;
    }

    if (InitS == instData->state)
    {
        switch (instData->chipState)
        {
            case CHST_UNK:
            {
                /* Enable interrupts in a short while */
                instData->timer = CsrSchedTimerSet(CSR_TYPE_A_ENABLE_DELAY *
                    TYPE_A_TIME_MS,
                    sdioADelayedIntrEnable,
                    0, instData);
                break;
            }

            case CHST_FUNCEN:

                CsrSdioFunctionDisable(instData->sdioHandle);
                CsrSdioFunctionEnable(instData->sdioHandle);

                CsrTypeAF0Read8(instData->sdioHandle,
                SDIO_REG_CSR_TO_HOST_SCRATCH0,
                &instData->readBuf8,
                sdioAChipStartComplete);

                instData->chipState = CHST_SCRATCH;

                break;

            case CHST_SCRATCH:
                /* Make sure IRQ state is well-defined at this point */
                instData->eventAndIrq = 0x0;

                instData->timer = CsrSchedTimerSet(CSR_TYPE_A_WAKEUP_DELAY *
                TYPE_A_TIME_MS,
                sdioATimerWAIT, 0, instData);

                instData->chipState = CHST_WAIT;

                CsrTypeAWrite8(instData->sdioHandle,
                SDIO_REG_BT_TYPE_A_INT_EN,
                EN_INTRD,
                sdioAChipStartComplete);
                break;

            case CHST_WAIT:
                if ((TYPEA_WUE_CMPL | TYPEA_WUE_INTR) == instData->eventAndIrq)
                {
                    /* Both events received so the chip is running */
                    CsrSchedTimerCancel(instData->timer, NULL, NULL);
                    instData->timer = CSR_SCHED_TID_INVALID;

                    instData->chipState = CHST_RUN;

                    /* We could call directly but this is consistent. */
                    CsrSchedBgintSet(instData->bgint_cardevent);
                }
                break;

            case CHST_UP:
                if ((TYPEA_WUE_CMPL | TYPEA_WUE_INTR) == instData->eventAndIrq)
                {
                    /* Both events received so we're up */
                    CsrSchedTimerCancel(instData->timer, NULL, NULL);
                    instData->timer = CSR_SCHED_TID_INVALID;

                    instData->curChipSleepState = SLEEP_STATE_AWAKE;
                    CsrTypeAF0Write8(instData->sdioHandle,
                        SDIO_REG_CSR_SLEEP_STATE,
                        (CsrUint8) (SDIO_REG_CSR_SLEEP_STATE_FUNC(instData->sdioHandle)
                                    | instData->curChipSleepState),
                        sdioAChipStartComplete);
                    instData->chipState = CHST_RUN;
                }
                break;

            case CHST_RUN:
                if (TYPEA_WUE_CMPL & instData->eventAndIrq)
                {
                    instData->chipState = CHST_RUN;
                    instData->state = IdleS;

                    /* NOP is waiting */
                    sdioAEnterRxPacketS(instData);
                }
                break;

            case CHST_DSWAIT:
                if ((TYPEA_WUE_CMPL | TYPEA_WUE_INTR) == instData->eventAndIrq)
                {
                    /* Both events received so we're up */
                    instData->timer = CsrSchedTimerSet(CSR_TYPE_A_WAKEUP_DELAY *
                        TYPE_A_TIME_MS,
                        sdioATimerDSWAIT, 0, instData);
                    instData->chipState = CHST_UP;

                    instData->curChipSleepState = SLEEP_STATE_AWAKE;
                    CsrTypeAF0Write8(instData->sdioHandle,
                        SDIO_REG_CSR_SLEEP_STATE,
                        (CsrUint8) (SDIO_REG_CSR_SLEEP_STATE_FUNC(instData->sdioHandle)
                                    | instData->curChipSleepState),
                        sdioAChipStartComplete);
                }
                break;

            case CHST_DS:
                if ((TYPEA_WUE_CMPL | TYPEA_WUE_INTR) == instData->eventAndIrq)
                {
                    /* Both events received so we're up */
                    instData->timer = CsrSchedTimerSet(CSR_TYPE_A_WAKEUP_DELAY *
                        TYPE_A_TIME_MS,
                        sdioATimerDS, 0, instData);
                    instData->chipState = CHST_UP;

                    instData->curChipSleepState = SLEEP_STATE_AWAKE;
                    CsrTypeAF0Write8(instData->sdioHandle,
                        SDIO_REG_CSR_SLEEP_STATE,
                        (CsrUint8) (SDIO_REG_CSR_SLEEP_STATE_FUNC(instData->sdioHandle)
                                    | instData->curChipSleepState),
                        sdioAChipStartComplete);
                }
                break;
        }
    }
}

/*
 * Signals that a packet has been added to the tx
 * queue.
 */
void CsrSdioATxQueueSignal(void *ptr)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) ptr;

    instData->pendingTxPackets++;

    /* Find out if we can start right away. */
    if (IdleS == instData->state)
    {
        /* Set bgint so we get to work. */
#if CSR_TYPE_A_SLEEP_TIMEOUT > 0
        if (CSR_SCHED_TID_INVALID != instData->timer)
        {
            CsrSchedTimerCancel(instData->timer, NULL, NULL);
            instData->timer = CSR_SCHED_TID_INVALID;
        }
#endif

        sdioAEnterTxPacketS(instData);
    }
#if CSR_TYPE_A_SLEEP_TIMEOUT > 0
    else if (SleepS == instData->state)
    {
        /*
         * The chip is idle but may be sleeping
         * so we need to force it to stay awake
         * from now on.
         * This causes sdioASleepSetComplete to
         * set the TX done bgint which starts
         * the TX operation.
         */

        instData->state = WakingS;
        instData->readyState = 0;
        CsrTypeAF0Write8(instData->sdioHandle,
            SDIO_REG_CSR_SLEEP_STATE,
            (CsrUint8) (SDIO_REG_CSR_SLEEP_STATE_FUNC(instData->sdioHandle)
                        | SLEEP_STATE_AWAKE),
            sdioASleepSetComplete);
    }
#endif
    else
    {
        /*
         * We're busy doing something else right now,
         * but that will invoke sdioABgintTxComplete()
         * when it's done, so we're done now.
         */
    }
}

void CsrSdioAProtReinit(void **gash)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) *gash;

    /* Bring chip up from scratch again. */
    instData->state = InitS;
    instData->chipState = CHST_UNK;

    CsrSchedBgintSet(instData->bgint_cardevent);
}

void CsrSdioAProtInit(void **gash)
{
    CsrInt32 retVal;
    CsrSdioAInstanceData *instData;

    /* allocate and initialise instance data space */
    *gash = (void *) CsrPmemAlloc(sizeof(CsrSdioAInstanceData));
    instData = (CsrSdioAInstanceData *) *gash;
    CsrMemSet(instData, 0, sizeof(CsrSdioAInstanceData));
    instData->maxRxTries = MAX_RETRIES;
    instData->maxTxTries = MAX_RETRIES;
    instData->readNeedAck = 1;

    instData->state = WaitForInsertS;
    instData->chipState = CHST_UNK;


    instData->timer = CSR_SCHED_TID_INVALID;

    instData->bgint_cardevent = CsrSchedBgintReg(sdioABgintCardEvent,
        instData, "sdioa_cardevent");
    instData->bgint_intr = CsrSchedBgintReg(sdioABgintIntr, instData,
        "sdioa_intr");
    instData->bgint_txdone = CsrSchedBgintReg(sdioABgintTxDone, instData,
        "sdioa_txdone");
    instData->bgint_rxdone = CsrSchedBgintReg(sdioABgintRxDone, instData,
        "sdioa_rxdone");
    instData->bgint_panic = CsrSchedBgintReg(sdioABgintPanic, instData,
        "sdioa_panic");

#ifdef CSR_ACLBUF_ENABLE
    instData->aclbufcb = NULL;
#endif

    /* register function driver */
    retVal = CsrTypeAFunctionDriverRegister(&sdioAFunctionDriver);
    if (retVal != CSR_RESULT_SUCCESS)
    {
        TYPE_A_PANIC(CSR_PANIC_FW_UNEXPECTED_VALUE, "type a function driver register");
    }
}

void CsrSdioACardEvent(void *p, CsrSdioFunction *f, CsrBool inserted)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) p;

    if (TRUE == inserted)
    {
        instData->sdioHandle = f;
        f->driverData = instData;
    }

    CsrSchedBgintSet(instData->bgint_cardevent);
}

void CsrSdioAProtDeinit(void **gash)
{
    CsrSdioAInstanceData *instData;

    instData = (CsrSdioAInstanceData *) (*gash);

    CsrTypeAFunctionDriverUnregister(&sdioAFunctionDriver);

    if (NULL != instData->rxMsg.buf)
    {
        if (instData->rxMsg.bufCtx != NULL)
        {
#ifdef CSR_ACLBUF_ENABLE
            if (instData->rxMsg.aclBuf == TRUE)
            {
                /* Pass back to AB, indicating there's no data. */
                CsrAclBufDeliver(0, instData->rxMsg.bufCtx, 0);
            }
            else
#endif
            {
                CsrMemBufQueue(instData->rxPool, instData->rxMsg.bufCtx, 0, 0);
                CsrMemBufFree(instData->rxPool, instData->rxMsg.hdrBuf);
            }
        }
    }

    CsrSchedBgintUnreg(instData->bgint_cardevent);
    CsrSchedBgintUnreg(instData->bgint_intr);
    CsrSchedBgintUnreg(instData->bgint_rxdone);
    CsrSchedBgintUnreg(instData->bgint_txdone);

    CsrMemBufFree(instData->rxPool, instData->rxMsg.hdrBuf);
    CsrMemBufFree(instData->txPool, instData->txBuf);

    CsrMemBufDeinit(instData->rxPool);
    CsrMemBufDeinit(instData->txPool);

    /* clear instance data to make it hard to reuse in case of call to handler */
    CsrMemSet(instData, 0, sizeof(CsrSdioAInstanceData));

    instData->bgint_cardevent = CSR_SCHED_BGINT_INVALID;
    instData->bgint_intr = CSR_SCHED_BGINT_INVALID;
    instData->bgint_rxdone = CSR_SCHED_BGINT_INVALID;
    instData->bgint_txdone = CSR_SCHED_BGINT_INVALID;
    instData->bgint_panic = CSR_SCHED_BGINT_INVALID;

    instData->timer = CSR_SCHED_TID_INVALID;

    CsrPmemFree(instData);
    *gash = NULL;
}

/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_panic.h"
#include "csr_pmem.h"
#include "csr_membuf.h"
#include "csr_tm_bluecore_transport.h"
#include "csr_tm_bluecore_private_lib.h"
#include "csr_type_a_com.h"
#include "csr_util.h"
#include "csr_transport.h"
#include "csr_tm_bluecore_transport_descriptor.h"

/* SDIO defines */
#define BT_TYPE_A_HEADER_LEN    (4)

/* large enough for any CSRMAX L2CAP or RFCOMM packets - value size is taken from USB */
#define PACKET_SIZE_L2CAP_OR_RFCOMM_MAX (776)

/* HCI defines */
#define FRAGMENT_END            (0x80)
#define FRAGMENT_START          (0x40)
#define UNFRAGMENTED            0xC0
#define MAX_HCI_PAYLOAD_SIZE    254
#define HCI_EXTENSION_HEADER_LEN  4

/* Macros */
/* Macro for inserting size in SDIO Bluetooth Type-A header */
#define TYPE_A_SET_PACKET_SIZE(sdioMsg, sdioSize) {(sdioMsg)[0] = (CsrUint8) (0xff & (sdioSize));(sdioMsg)[1] = (CsrUint8) (0xff & ((sdioSize) >> 8));(sdioMsg)[2] = (CsrUint8) (0xff & ((sdioSize) >> 16));}

/* Local funcs */
static void send2Sdio(CsrUint8 channel, CsrUint8 *msg, CsrUint16 size);

/* Function TransportStart
 *  RETURN: TRUE on succes else FALSE
 */
static CsrBool CsrTransportTypeAStart(void)
{
    return CsrSdioBtADrvStart(0);
}

static CsrBool CsrTransportTypeAStop(void)
{
    CsrBool res = CsrSdioBtADrvStop();
#ifdef __linux__
#ifdef __KERNEL__
    /* Must stop USB driver like this in Linux kernel */
    CsrSdioBtAExit();
#endif
#endif
    return res;
}

static CsrUint16 CsrTransportTypeAQuery(void)
{
    return TRANSPORT_TYPE_TYPE_A;
}

static void CsrTransportTypeAMsgTx(void *arg)
{
    MessageStructure *txMsg;
    TXMSG *msg;
    CsrUint8 channel;

    msg = (TXMSG *) arg;

    txMsg = &msg->m;
    channel = msg->chan;
    if (((channel >= TRANSPORT_CHANNEL_HCI) && (channel <= TRANSPORT_CHANNEL_SCO)) || (channel == TRANSPORT_CHANNEL_BCCMD) || (channel == TRANSPORT_CHANNEL_HQ) || (channel == TRANSPORT_CHANNEL_VM))
    {
        send2Sdio(channel, txMsg->buf, (CsrUint16) txMsg->buflen);
    }
    else
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE,
            "type a invalid channel");
    }
    CsrPmemFree(msg);
}

static void CsrTransportTypeAScoTx(void *scoData)
{
    send2Sdio(TRANSPORT_CHANNEL_SCO, (CsrUint8 *) scoData,
        (CsrUint16) (((CsrUint8 *) scoData)[2] + 3));
}

static void CsrTransportTypeAMsgClean(void *msg)
{
    /* Things are cleaned up by the SdioBtADrvRx() function. */
}

void CsrTmBlueCoreTypeADeliverMsg(void *msg, CsrUint8 rel)
{
    MessageStructure *theMsg;

    theMsg = (MessageStructure *) msg;

    CsrTmBlueCoreTransportMsgRx(CsrBluecoreTransportTypeA, theMsg);
}

/*---   Static functions   ---*/

/***********************************************************************
        Handle messages tunneled over HCI
***********************************************************************/
static void sdioASendHciExtCmd(CsrUint8 ch, CsrUint8 *cmd, CsrUint16 len)
{
    CsrUint8 *sdioHciExtCmd;
    CsrUint16 count = 0;
    CsrUint32 sdioSize;

    if (len <= MAX_HCI_PAYLOAD_SIZE) /* CSRMAX HCI cmd packet size */
    {
        sdioSize = BT_TYPE_A_HEADER_LEN + HCI_EXTENSION_HEADER_LEN + len;
        sdioHciExtCmd = (CsrUint8 *) CsrPmemAlloc(sdioSize);
        TYPE_A_SET_PACKET_SIZE(sdioHciExtCmd, sdioSize);
        sdioHciExtCmd[3] = SDIO_CHANNEL_HCI;
        sdioHciExtCmd[4] = 0x00;    /* 2 bytes HCI ext. opcode corresponding to OGF=0x3f, OCF=0x00 */
        sdioHciExtCmd[5] = 0xFC;
        sdioHciExtCmd[6] = len + 1;
        sdioHciExtCmd[7] = UNFRAGMENTED | ch; /* unfragmented packet, tunneled channel id */
        CsrMemCpy(&sdioHciExtCmd[8], cmd, len);

        CsrSdioBtADrvTx(sdioHciExtCmd, sdioSize);
    }
    else    /* fragmentation is required, packet too big */
    {
        CsrBool firstFragment = TRUE;

        while (len > 0)
        {
            if (len > MAX_HCI_PAYLOAD_SIZE)
            {
                sdioSize = BT_TYPE_A_HEADER_LEN + HCI_EXTENSION_HEADER_LEN + MAX_HCI_PAYLOAD_SIZE;
                sdioHciExtCmd = (CsrUint8 *) CsrPmemAlloc(sdioSize);
                TYPE_A_SET_PACKET_SIZE(sdioHciExtCmd, sdioSize);
                sdioHciExtCmd[3] = SDIO_CHANNEL_HCI;
                sdioHciExtCmd[4] = 0x00;
                sdioHciExtCmd[5] = 0xFC;
                sdioHciExtCmd[6] = MAX_HCI_PAYLOAD_SIZE + 1;

                if (firstFragment)
                {
                    sdioHciExtCmd[7] = FRAGMENT_START | ch; /* first fragment packet, tunneled channel id */
                    firstFragment = FALSE;
                    count = 0;
                }
                else
                {
                    sdioHciExtCmd[7] = ch; /* continuation packet, tunneled channel id */
                }
                CsrMemCpy(&sdioHciExtCmd[8], cmd + count, MAX_HCI_PAYLOAD_SIZE);

                len -= MAX_HCI_PAYLOAD_SIZE;
                count += MAX_HCI_PAYLOAD_SIZE;
            }
            else
            {
                sdioSize = BT_TYPE_A_HEADER_LEN + HCI_EXTENSION_HEADER_LEN + len;
                sdioHciExtCmd = (CsrUint8 *) CsrPmemAlloc(sdioSize);
                TYPE_A_SET_PACKET_SIZE(sdioHciExtCmd, sdioSize);
                sdioHciExtCmd[3] = SDIO_CHANNEL_HCI;
                sdioHciExtCmd[4] = 0x00;
                sdioHciExtCmd[5] = 0xFC;
                sdioHciExtCmd[6] = len + 1;
                sdioHciExtCmd[7] = FRAGMENT_END | ch; /*  last fragmented packet, tunneled channel id */
                CsrMemCpy(&sdioHciExtCmd[8], cmd + count, len);

                len = 0;
            }
            CsrSdioBtADrvTx(sdioHciExtCmd, sdioSize);
        }
    }
    CsrPmemFree(cmd);
}

/*
   DESC: Build a SDIO Bluetooth Type-A message and send it to driver

*/
static void send2Sdio(CsrUint8 channel, CsrUint8 *msg, CsrUint16 size)
{
    CsrUint8 *sdioMsg = NULL;
    CsrUint32 sdioSize;
    CsrUint8 sdioChannel;

    if ((channel >= TRANSPORT_CHANNEL_HCI) && (channel <= TRANSPORT_CHANNEL_SCO))
    {
        /* regular HCI, ACL or SCO messages */
        sdioSize = BT_TYPE_A_HEADER_LEN + size;
        sdioMsg = (CsrUint8 *) CsrPmemAlloc(sdioSize);
        /* Build a msg according to spec */
        /* SDIO Card Type-A Spedification for Bluetooth Ver 1.00 */
        TYPE_A_SET_PACKET_SIZE(sdioMsg, sdioSize);
        if (channel == TRANSPORT_CHANNEL_HCI)
        {
            sdioChannel = SDIO_CHANNEL_HCI;
        }
        else if (channel == TRANSPORT_CHANNEL_ACL)
        {
            sdioChannel = SDIO_CHANNEL_ACL;
        }
        else
        {
            sdioChannel = SDIO_CHANNEL_SCO;
        }
        sdioMsg[3] = sdioChannel;
        CsrMemCpy(&sdioMsg[4], msg, size);
        CsrSdioBtADrvTx(sdioMsg, sdioSize);
        CsrPmemFree(msg);
    }
    else
    {
        /* do HCI tunneling */
        sdioASendHciExtCmd(channel, msg, size);
    }
}

static CsrBool CsrTransportTypeADriverRestart(CsrUint8 reset)
{
    CsrSdioBtADrvRestart();

    return TRUE;
}

static void CsrTransportTypeARestart(void)
{
}

static void CsrTransportTypeAClose(void)
{
    CsrTmBluecoreResetIndSend();
}

static struct CsrTmBlueCoreTransport _CsrTransportTypeAType =
{
    CsrTransportTypeAStart,
    CsrTransportTypeAStop,
    CsrTransportTypeAQuery,
    CsrTransportTypeAMsgTx,
    NULL, /* msgrx */
    CsrTransportTypeAMsgClean,
    CsrTransportTypeAScoTx,
    CsrTransportTypeADriverRestart,
    CsrTransportTypeARestart,
    CsrTransportTypeAClose,
    FALSE,
};
void *CsrBluecoreTransportTypeA = &_CsrTransportTypeAType;

void CsrTmBlueCoreTypeAInit(void **gash)
{
    CsrTmBlueCoreTransportInit(gash, CsrBluecoreTransportTypeA);
}

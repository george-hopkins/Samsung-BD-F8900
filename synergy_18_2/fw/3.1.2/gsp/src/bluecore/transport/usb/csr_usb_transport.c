/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_panic.h"
#include "csr_pmem.h"
#include "csr_usb_com.h"
#include "csr_util.h"
#include "csr_transport.h"
#include "csr_tm_bluecore_transport_descriptor.h"
#include "csr_tm_bluecore_transport.h"
#include "csr_tm_bluecore_private_lib.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log_gsp.h"
#endif

static void Send2Usb(CsrUint8 channel, CsrUint8 *msg, CsrUint32 size)
{
    if ((channel > TRANSPORT_CHANNEL_DM) && (channel < TRANSPORT_CHANNEL_L2CAP))
    {
        /* regular HCI, ACL or SCO messages */
        if (CsrUsbDrvTx(channel, msg, size) == FALSE)
        {
            CsrTmBluecoreResetIndSend();
        }
    }
}

static CsrBool CsrTransportUsbStart(void)
{
    return CsrUsbDrvStart(0);
}

static CsrBool CsrTransportUsbStop(void)
{
    CsrBool res = CsrUsbDrvStop();
    return res;
}

static CsrUint16 CsrTransportUsbQuery(void)
{
    return TRANSPORT_TYPE_USB;
}

static void CsrTransportUsbMsgTx(void *arg)
{
    MessageStructure *txMsg;
    TXMSG *msg;

    msg = (TXMSG *) arg;

    txMsg = &msg->m;
    Send2Usb(msg->chan, txMsg->buf, txMsg->buflen);
    CsrPmemFree(msg);
}

static void CsrTransportUsbScoTx(void *scoData)
{
    Send2Usb(TRANSPORT_CHANNEL_SCO, (CsrUint8 *) scoData, (CsrUint32) (((CsrUint8 *) scoData)[2] + 3));
}

static void CsrTransportUsbMsgClean(void *msg)
{
    CsrPmemFree(((MessageStructure *) msg)->buf);
}

static CsrBool CsrTransportUsbDriverRestart(CsrUint8 reset)
{
    if (CsrUsbDrvStop())
    {
        if (CsrUsbDrvStart(reset))
        {
            return TRUE;
        }
    }
    return FALSE;
}

static void CsrTransportUsbRestart(void)
{
}

static void CsrTransportUsbClose(void)
{
    CsrTmBluecoreResetIndSend();
}

static struct CsrTmBlueCoreTransport _CsrTransportUsbType =
{
    CsrTransportUsbStart,
    CsrTransportUsbStop,
    CsrTransportUsbQuery,
    CsrTransportUsbMsgTx,
    NULL, /* msgrx */
    CsrTransportUsbMsgClean,
    CsrTransportUsbScoTx,
    CsrTransportUsbDriverRestart,
    CsrTransportUsbRestart,
    CsrTransportUsbClose,
    FALSE,
};
void *CsrBluecoreTransportUsb = &_CsrTransportUsbType;

/* For backwards compatibility with the USB driver */
void CsrTmBlueCoreUsbDeliverMsg(void *msg, CsrUint8 rel)
{
    MessageStructure *theMsg;

    theMsg = (MessageStructure *) msg;

    CsrTmBlueCoreTransportMsgRx(CsrBluecoreTransportUsb, theMsg);
}

void CsrTmBlueCoreUsbInit(void **gash)
{
    CsrTmBlueCoreTransportInit(gash, CsrBluecoreTransportUsb);
}

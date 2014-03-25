/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_h4.h"
#include "csr_h4_rx.h"
#include "csr_h4_tx.h"
#include "csr_h4ds_initialize.h"
#include "csr_types.h"
#include "csr_serial_com.h"
#include "csr_tm_bluecore_transport.h"
#include "csr_tm_bluecore_transport_descriptor.h"
#include "csr_h4ds_inst.h"
#ifdef CSR_LOG_ENABLE
#include "csr_log_gsp.h"
#endif

static CsrBool CsrTransportH4dsStart(void)
{
    CsrBool returnValue;
    CsrUint32 baudRate = 0;

    baudRate = CsrUartDrvGetBaudrate(CsrH4DSInst.uartHandle);
    if (baudRate != 0)
    {
        /* Initialize H4DS */
        CsrH4DSInitialize();
    }

    returnValue = CsrUartDrvStart(CsrH4DSInst.uartHandle, 0);

    if (baudRate != 0)
    {
        /* Calculate how long it takes to transmit half of the TX buffer */
        CsrH4Inst.txBufSize = (CsrUint16) CsrUartDrvGetTxSpace(CsrH4DSInst.uartHandle);
        CsrH4Inst.txBufFullWaitTime = (8 * CsrH4Inst.txBufSize) / (baudRate / (CSR_SCHED_MILLISECOND / 2)) * CSR_SCHED_MILLISECOND;
    }
    return returnValue;
}

static CsrBool CsrTransportH4dsStop(void)
{
    CsrBool res = CsrUartDrvStop(CsrH4DSInst.uartHandle);
    CsrH4DSDeinitialize();
    return res;
}

static CsrUint16 CsrTransportH4dsQuery(void)
{
    return TRANSPORT_TYPE_H4_DS;
}

static void CsrTransportH4dsMsgTx(void *arg)
{
    TXMSG *msg;

    msg = (TXMSG *) arg;

    CsrH4MsgSend(&CsrH4Inst, msg, msg->chan);
}

static void CsrTransportH4dsScoTx(void *arg)
{
    CsrH4MsgSend(&CsrH4Inst, arg, TRANSPORT_CHANNEL_SCO);
}

static void CsrTransportH4dsMsgClean(void *msg)
{
    CsrH4RxMsgFree(msg);
}

static CsrBool CsrTransportH4dsDriverRestart(CsrUint8 reset)
{
    if (CsrUartDrvStop(CsrH4DSInst.uartHandle))
    {
        return CsrUartDrvStart(CsrH4DSInst.uartHandle, reset);
    }
    return FALSE;
}

static void CsrTransportH4dsRestart(void)
{
    CsrH4DSDeinitialize();
    CsrH4DSInitialize();
}

static void CsrTransportH4dsClose(void)
{
}

static struct CsrTmBlueCoreTransport _CsrTransportH4dsType =
{
    CsrTransportH4dsStart,
    CsrTransportH4dsStop,
    CsrTransportH4dsQuery,
    CsrTransportH4dsMsgTx,
    NULL, /* msgrx */
    CsrTransportH4dsMsgClean,
    CsrTransportH4dsScoTx,
    CsrTransportH4dsDriverRestart,
    CsrTransportH4dsRestart,
    CsrTransportH4dsClose,
    FALSE,
};
void *CsrBluecoreTransportH4DS = &_CsrTransportH4dsType;

void CsrTmBlueCoreH4dsInit(void **gash)
{
    CsrTmBlueCoreTransportInit(gash, CsrBluecoreTransportH4DS);
}

void CsrTmBlueCoreRegisterUartHandleH4ds(void *handle)
{
    CsrH4dsRegisterUartHandle(handle);
}

/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_abcsp.h"
#include "csr_queue_lib.h"
#include "csr_serial_com.h"
#include "csr_abcsp_config_rxmsg.h"
#include "csr_util.h"
#include "csr_tm_bluecore_transport.h"
#include "csr_transport.h"
#include "csr_tm_bluecore_transport_descriptor.h"
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
#include "csr_ips.h"
#include "csr_sched_ips.h"
#endif

extern TXMSG *CsrAbcspQStart;
extern void CsrAbcspBgintSend(void);

#ifndef CSR_BCSP_RETRANSMISSION_TIMER
#define BCSP_HOST_LATENCY   3 /* ms */
#define MAX_PACKET_LENGTH   1025 /* bytes */
#define UART_BITS_PER_BYTE  11 /* 8 data + start + stop + even parity */

#define BCSP_REXMIT_PERIOD()  (((2 * 2 * UART_BITS_PER_BYTE * MAX_PACKET_LENGTH * 1000) / \
                                (CsrUartDrvGetBaudrate(CsrAbcspInstanceData.uartHandle) * 8) + \
                                2 * BCSP_HOST_LATENCY) * 1000)
#endif

static CsrBool CsrTransportBcspStart(void)
{
    CsrBool rv;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrUint32 *previousMeasurements;
    previousMeasurements = CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_CURRENT, CsrAbcspMeasurements);
#endif

    CsrAbcspInit();
    rv = CsrUartDrvStart(CsrAbcspInstanceData.uartHandle, 0);

#ifndef CSR_BCSP_RETRANSMISSION_TIMER
    CsrAbcspInstanceData.retransmitTimerPeriod = BCSP_REXMIT_PERIOD();
#endif

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_CURRENT, previousMeasurements);
#endif
    return rv;
}

static CsrBool CsrTransportBcspStop(void)
{
    CsrBool res = CsrUartDrvStop(CsrAbcspInstanceData.uartHandle);
    CsrAbcspDeinit();
    return res;
}

static CsrUint16 CsrTransportBcspQuery(void)
{
    return TRANSPORT_TYPE_BCSP;
}

static void CsrTransportBcspMsgTx(void *arg)
{
    TXMSG *msg;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrUint32 *previousMeasurements;
    previousMeasurements = CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, CsrAbcspMeasurements);
#endif

    msg = (TXMSG *) arg;

    CsrQueueMsgStore(&CsrAbcspQStart, msg);
    CsrAbcspBgintSend();

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, previousMeasurements);
#endif
}

static void CsrTransportBcspMsgClean(void *arg)
{
    TXMSG *msg;

    msg = (TXMSG *) arg;

    ABCSP_RXMSG_DESTROY(msg);
}

static void CsrTransportBcspScoTx(void *scoData)
{
    MessageStructure msg;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrUint32 *previousMeasurements;
    previousMeasurements = CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, CsrAbcspMeasurements);
#endif

    msg.buf = scoData;
    msg.buflen = (((CsrUint8 *) scoData)[2] + 3);  /* 3 = HCI_DATA_HEADER_SIZE + HCI_SCO_DATA_LEN_SIZE */

    CsrAbcspMsgSend(&msg, 7, TRANSPORT_CHANNEL_SCO);

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, previousMeasurements);
#endif
}

static CsrBool CsrTransportBcspDriverRestart(CsrUint8 reset)
{
    CsrBool rv;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrUint32 *previousMeasurements;
    previousMeasurements = CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_CURRENT, CsrAbcspMeasurements);
#endif

    rv = FALSE;

    if (CsrUartDrvStop(CsrAbcspInstanceData.uartHandle))
    {
        rv = CsrUartDrvStart(CsrAbcspInstanceData.uartHandle, reset);

#ifndef CSR_BCSP_RETRANSMISSION_TIMER
        CsrAbcspInstanceData.retransmitTimerPeriod = BCSP_REXMIT_PERIOD();
#endif
    }

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_CURRENT, previousMeasurements);
#endif
    return rv;
}

static void CsrTransportBcspRestart(void)
{
    CsrAbcspDeinit();
    CsrAbcspInit();
}

static void CsrTransportBcspClose(void)
{
}

static struct CsrTmBlueCoreTransport _CsrTransportBcspType =
{
    CsrTransportBcspStart,
    CsrTransportBcspStop,
    CsrTransportBcspQuery,
    CsrTransportBcspMsgTx,
    NULL, /* msgrx */
    CsrTransportBcspMsgClean,
    CsrTransportBcspScoTx,
    CsrTransportBcspDriverRestart,
    CsrTransportBcspRestart,
    CsrTransportBcspClose,
    FALSE,
};
void *CsrBluecoreTransportBcsp = &_CsrTransportBcspType;

void CsrTmBlueCoreBcspInit(void **gash)
{
    CsrTmBlueCoreTransportInit(gash, CsrBluecoreTransportBcsp);
}

void CsrTmBlueCoreRegisterUartHandleBcsp(void *handle)
{
    CsrAbcspRegisterUartHandle(handle);
}

/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_random.h"

#include "csr_abcsp.h"
#include "csr_abcsp_prototype.h"
#include "csr_abcsp_events.h"
#include "csr_abcsp_config_event.h"
#include "csr_abcsp_panics.h"
#include "csr_abcsp_log_text.h"

#include "csr_abcsp_intf_hl.h"
#include "csr_serial_com.h"
#include "csr_tm_bluecore_bcsp.h"

#include "csr_log_text_2.h"

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
#include "csr_sched_ips.h"
CsrUint32 *CsrAbcspMeasurements = NULL;
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrBcspLto);

extern abcsp CsrAbcspInstanceData;

CsrUint32 CsrAbcspTxMsgQueueClean(void); /* Remove queues from layer just above abcsp */

/**/
void CsrAbcspRegisterUartHandle(void *handle)
{
    CsrAbcspInstanceData.uartHandle = handle;
}

/****************************************************************************
NAME
        CsrAbcspInit  -  initialise the abcsp library
*/

void CsrAbcspInit(void)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    /* _this->retransmitTimer is calculated elsewhere */

#if CSR_BCSP_RETRANSMISSION_JITTER > 0
    _this->randomState = CsrRandomSeed();
#endif

    CSR_LOG_TEXT_REGISTER(&CsrBcspLto, "BCSP", 0, NULL);

    ABCSP_EVENT(ABCSP_EVT_START);
    /* The bgint needed for abcsp pumps */
    _this->bgint_pumptxmsg = CsrSchedBgintReg((CsrSchedBgintHandler) CsrAbcspPumpTxMsgsOut, NULL, "CsrAbcspPumpTxMsgs");

    /* The bgint for rx msg delivery */
    _this->bgint_rx = CsrSchedBgintReg(CsrUartDrvRx, _this->uartHandle, "CsrUartDrvRx");
    CsrUartDrvRegister(_this->uartHandle, CsrTmBlueCoreBcspDeliverMsg, _this->bgint_rx);

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_BGINT(_this->bgint_pumptxmsg), CsrAbcspMeasurements);
    CsrSchedIpsInsert(CSR_SCHED_IPS_CONTEXT_BGINT(_this->bgint_rx), CsrAbcspMeasurements);
#endif

    /* BCSP Link Establishment engine. */
    CsrAbcspBcspLeInit();

    /* The slip reception code block. */
    CsrAbcspUartInit();

    /* Transmit message generator. */
    CsrAbcspTxMsgInit();

    /* Transmit message slip encoder. */
    CsrAbcspTxSlipInit();

    /* We done */
    ABCSP_EVENT(ABCSP_EVT_INITIATED);
}

/****************************************************************************
NAME
        CsrAbcspDeinit  -  de-initialise the abcsp library
*/

void CsrAbcspDeinit(void)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    /*        BT_DBG("this %p", _this); */
    CsrSchedBgintUnreg(_this->bgint_pumptxmsg);
    _this->bgint_pumptxmsg = CSR_SCHED_BGINT_INVALID;

    CsrSchedBgintUnreg(_this->bgint_rx);
    _this->bgint_rx = CSR_SCHED_BGINT_INVALID;

    /* BCSP Link Establishment engine. */
    CsrAbcspBcspLeDeinit();

    CsrAbcspTxMsgQueueClean(); /* Remove queues from layer just above abcsp */

    /* Transmit message generator. */
    CsrAbcspTxMsgDeinit();

#if CSR_BCSP_RETRANSMISSION_JITTER > 0
    CsrPmemFree(_this->randomState);
    _this->randomState = NULL;
#endif
}

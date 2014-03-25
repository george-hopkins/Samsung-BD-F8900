/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_h4_events.h"
#include "csr_panic.h"

#include "csr_h4ds_log_text.h"
#include "csr_log_text_2.h"

void CsrH4Event(CsrUint8 e)
{
    if (e == H4_EVT_H4_SYNC_LOST)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_H4_SYNC_LOST,
            "h4ds sync lost");
    }

#ifdef CSR_LOG_ENABLE
    switch (e)
    {
        /* Initialisation of the h4 transport protocol has started. */
        case H4_EVT_START:
            CSR_LOG_TEXT_DEBUG((CsrH4dsLto, CSR_H4DS_LTSO_H4, "EVT_START"));
            break;
        /* Initialisation of the h4 transport protocol has been completed. */
        case H4_EVT_INITIATED:
            CSR_LOG_TEXT_INFO((CsrH4dsLto, CSR_H4DS_LTSO_H4, "EVT_INITIATED"));
            break;
        /* Deinitialisation of the h4 transport protocol has been completed. */
        case H4_EVT_DEINITIATED:
            CSR_LOG_TEXT_INFO((CsrH4dsLto, CSR_H4DS_LTSO_H4, "EVT_DEINITIATED"));
            break;
        /* The function h4_txmsg() has been called with dubious parameters. */
        case H4_EVT_DUFF_TX_PARAMS:
            CSR_LOG_TEXT_WARNING((CsrH4dsLto, CSR_H4DS_LTSO_H4, "EVT_DUFF_TX_PARAMS"));
            break;
        /* An attempt to allocate memory has failed.  (Using the local malloc(),
           alloc(), CsrPmemAlloc(), calloc(), etc.)  The h4 code will normally handle
           this gracefully, but this event indicates the system is ill, and will
           probably croak soon. */
        case H4_EVT_MALLOC:
            CSR_LOG_TEXT_WARNING((CsrH4dsLto, CSR_H4DS_LTSO_H4, "EVT_MALLOC"));
            break;
        /* The H4 driver has lost sync with the incoming byte stream */
        case H4_EVT_H4_SYNC_LOST:
            CSR_LOG_TEXT_WARNING((CsrH4dsLto, CSR_H4DS_LTSO_H4, "EVT_H4_SYNC_LOST"));
            break;
        /* An H4 message was received that's payload was too large. */
        case H4_EVT_OVERSIZE_RX_PAYLOAD:
            CSR_LOG_TEXT_WARNING((CsrH4dsLto, CSR_H4DS_LTSO_H4, "EVT_OVERSIZE_RX_PAYLOAD"));
            break;
        /* The H4 driver has sent more data then the UART can handle */
        case H4_EVT_OVERRUN:
            CSR_LOG_TEXT_WARNING((CsrH4dsLto, CSR_H4DS_LTSO_H4, "EVT_OVERRUN"));
            break;
        /* The H4 driver has sent more data than instructed to */
        case H4_EVT_TOO_MUCH_TX:
            CSR_LOG_TEXT_WARNING((CsrH4dsLto, CSR_H4DS_LTSO_H4, "EVT_TOO_MUCH_TX"));
            break;
        /* Received unknown parameter in RX message */
        case H4_EVT_DUFF_RX_PARAMS:
            CSR_LOG_TEXT_WARNING((CsrH4dsLto, CSR_H4DS_LTSO_H4, "EVT_DUFF_RX_PARAMS"));
            break;
        /* Something else is received */
        default:
            CSR_LOG_TEXT_WARNING((CsrH4dsLto, CSR_H4DS_LTSO_H4, "Unknown event"));
            break;
    }
#endif
}

/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_abcsp_events.h"
#include "csr_abcsp.h"
#include "csr_pmem.h"
#include "csr_panic.h"
#include "csr_tm_bluecore_private_lib.h"

#include "csr_abcsp_log_text.h"
#include "csr_log_text_2.h"
#include <stdio.h>
#ifdef CSR_LOG_ENABLE
#include "csr_log_gsp.h"
#endif

void CsrAbcspEvent(CsrUint8 e)
{
    switch (e)
    {
        case ABCSP_EVT_LE_SYNC_LOST:
        {
            CsrTmBluecoreResetIndSend();
            break;
        }
        default:
        {
            break;
        }
    }

#if defined(CSR_LOG_ENABLE)
    switch (e)
    {
        /* Initialisation of the abcsp library has started. */
        case ABCSP_EVT_START:
            CSR_LOG_TEXT_DEBUG((CsrBcspLto, 0, "EVT_START"));
            break;

        /* Initialisation of the abcsp library has been completed. */
        case ABCSP_EVT_INITIATED:
            CSR_LOG_TEXT_INFO((CsrBcspLto, 0, "EVT_INITIATED"));
            break;

        /* The BCSP Link Establishment engine has established sync with its peer
           and the choke has been removed locally.  Messages will now be accepted
           by CsrAbcspMsgSend() and csrAbcspUartBytesDeliver(). */
        case ABCSP_EVT_LE_SYNC:
            CSR_LOG_TEXT_INFO((CsrBcspLto, 0, "EVT_LE_SYNC"));
            break;

        /* The BCSP Link Establishment engine has received confirmation from the peer
           that it's in its "garrulous" state, so allowing generation of the sync-lost
           event below. */
        case ABCSP_EVT_LE_CONF:
            CSR_LOG_TEXT_INFO((CsrBcspLto, 0, "EVT_LE_CONF"));
            break;

        /* The BCSP Link Establishment engine has detected that the peer BCSP-LE
           engine has restarted.  This presumably means that the peer BCSP stack
           (or system) has been restarted.  A common local response would be to
           restart (CsrAbcspInit()) the local BCSP stack. */
        case ABCSP_EVT_LE_SYNC_LOST:
            CSR_LOG_TEXT_WARNING((CsrBcspLto, 0, "EVT_LE_SYNC_LOST"));
            break;

        /* An attempt has been made to use an uninitialised block of code.  This
           should only mean that a call has been made to the abcsp library before
           CsrAbcspInit() has been called. */
        case ABCSP_EVT_UNINITIATED:
            CSR_LOG_TEXT_INFO((CsrBcspLto, 0, "EVT_UNINITIATED"));
            break;

        /* The rx SLIP engine has established sync on the received byte stream. */
        case ABCSP_EVT_SLIP_SYNC:
            CSR_LOG_TEXT_INFO((CsrBcspLto, 0, "EVT_SLIP_SYNC"));
            break;

        /* The rx SLIP engine has lost sync on the received byte stream. */
        case ABCSP_EVT_SLIP_SYNC_LOST:
            CSR_LOG_TEXT_WARNING((CsrBcspLto, 0, "EVT_SLIP_SYNC_LOST"));
            break;

        /* A message received by the BCSP stack has been discarded because the local
           choke is applied.  This is a reasonable occurrence where the peer BCSP stack
           has removed its choke (shortly) before the local choke is removed. */
        case ABCSP_EVT_RX_CHOKE_DISCARD:
            CSR_LOG_TEXT_INFO((CsrBcspLto, 0, "EVT_RX_CHOKE_DISCARD"));
            break;

        /* A message submitted to CsrAbcspMsgSend() has been rejected because the
           local choke is applied. */
        case ABCSP_EVT_TX_CHOKE_DISCARD:
            CSR_LOG_TEXT_INFO((CsrBcspLto, 0, "EVT_RX_CHOKE_DISCARD"));
            break;

        /* A message submitted to CsrAbcspMsgSend() has been rejected because the
           BCSP transmit window is full. */
        case ABCSP_EVT_TX_WINDOW_FULL_DISCARD:
            CSR_LOG_TEXT_INFO((CsrBcspLto, 0, "EVT_TX_WINDOW_FULL_DISCARD"));
            break;

        /* The header of a received message has declared its payload to be
           longer than the limit set by the #define ABCSP_RXMSG_MAX_PAYLOAD_LEN
           and the message is being discarded.  (See the #define's description
           in abcsp.h ) */
        case ABCSP_EVT_OVERSIZE_DISCARD:
            CSR_LOG_TEXT_WARNING((CsrBcspLto, 0, "EVT_OVERSIZE_DISCARD"));
            break;

        /* A received reliable message has been rejected because its sequence
           number is not that expected by the local BCSP stack. */
        case ABCSP_EVT_MISSEQ_DISCARD:
            CSR_LOG_TEXT_WARNING((CsrBcspLto, 0, "EVT_MISSEQ_DISCARD"));
            break;

        /* A received message's header checksum has failed and the message is
           being discarded. */
        case ABCSP_EVT_CHECKSUM:
            CSR_LOG_TEXT_WARNING((CsrBcspLto, 0, "EVT_CHECKSUM"));
            break;

        /* A received message held fewer bytes than its header declared. */
        case ABCSP_EVT_SHORT_PAYLOAD:
            CSR_LOG_TEXT_WARNING((CsrBcspLto, 0, "EVT_SHORT_PAYLOAD"));
            break;

        /* A received message is being discarded because its CRC check failed. */
        case ABCSP_EVT_CRC_FAIL:
            CSR_LOG_TEXT_WARNING((CsrBcspLto, 0, "EVT_CRC_FAIL"));
            break;

        default:
            CSR_LOG_TEXT_INFO((CsrBcspLto, 0, "Unknown event"));
            break;
    }
#endif
}

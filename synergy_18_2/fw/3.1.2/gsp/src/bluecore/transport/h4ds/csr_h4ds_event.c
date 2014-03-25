/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_panic.h"
#include "csr_h4ds_events.h"
#include "csr_h4ds_panics.h"
#include "csr_h4.h"
#include "csr_tm_bluecore_private_lib.h"

#include "csr_h4ds_log_text.h"
#include "csr_log_text_2.h"

extern H4InstanceData CsrH4Inst;

void CsrH4DSEvent(CsrUint8 e)
{
    switch (e)
    {
        case H4DS_EVT_PEER_RESTART:
        case H4DS_EVT_SYNC_LOSS:
        {
            CsrTmBluecoreResetIndSend();
            break;
        }
        default:
        {
            break;
        }
    }

#ifdef CSR_LOG_ENABLE
    switch (e)
    {
        case H4DS_EVT_START:
            CSR_LOG_TEXT_INFO((CsrH4dsLto, CSR_H4DS_LTSO_H4DS, "EVT_START"));
            break;
        case H4DS_EVT_INITIATED:
            CSR_LOG_TEXT_INFO((CsrH4dsLto, CSR_H4DS_LTSO_H4DS, "EVT_INITIATED"));
            break;
        case H4DS_EVT_SYNC_LOSS:
            CSR_LOG_TEXT_WARNING((CsrH4dsLto, CSR_H4DS_LTSO_H4DS, "EVT_SYNC_LOSS"));
            break;
        case H4DS_EVT_PEER_MAY_SLEEP:
            CSR_LOG_TEXT_INFO((CsrH4dsLto, CSR_H4DS_LTSO_H4DS, "EVT_PEER_MAY_SLEEP"));
            break;
        case H4DS_EVT_PEER_AWAKE:
            CSR_LOG_TEXT_INFO((CsrH4dsLto, CSR_H4DS_LTSO_H4DS, "EVT_PEER_AWAKE"));
            break;
        case H4DS_EVT_LOCAL_DEVICE_MAY_SLEEP:
            CSR_LOG_TEXT_INFO((CsrH4dsLto, CSR_H4DS_LTSO_H4DS, "EVT_LOCAL_DEVICE_MAY_SLEEP"));
            break;
        case H4DS_EVT_LOCAL_DEVICE_MAY_NOT_SLEEP:
            CSR_LOG_TEXT_INFO((CsrH4dsLto, CSR_H4DS_LTSO_H4DS, "EVT_DEVICE_MAY_NOT_SLEEP"));
            break;
        case H4DS_EVT_LE_SYNC:
            CSR_LOG_TEXT_INFO((CsrH4dsLto, CSR_H4DS_LTSO_H4DS, "EVT_LE_SYNC"));
            break;
        case H4DS_EVT_LE_CONF:
            CSR_LOG_TEXT_INFO((CsrH4dsLto, CSR_H4DS_LTSO_H4DS, "EVT_LE_CONF"));
            break;
        case H4DS_EVT_PEER_RESTART:
            CSR_LOG_TEXT_INFO((CsrH4dsLto, CSR_H4DS_LTSO_H4DS, "EVT_PEER_RESTART"));
            break;

        /* Something else is received */
        default:
            CSR_LOG_TEXT_WARNING((CsrH4dsLto, CSR_H4DS_LTSO_H4DS, "Unknown event"));
            break;
    }
#endif
}

void CsrH4DSPanic(CsrUint32 p)
{
    /* H4DS engine has paniced. Do something dramatic */
    CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_H4_CORRUPTION, "h4ds panic");
}

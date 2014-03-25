/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_macro.h"
#include "csr_h4ds_private.h"

#include "csr_h4ds_log_text.h"
#include "csr_log_text_2.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrH4dsLto);

#ifdef CSR_LOG_ENABLE
static const CsrCharString *subOrigins[] =
{
    "H4",
    "H4DS",
};
#endif

/****************************************************************************
NAME
        CsrH4DSInit  -  initialise the h4ds library
*/

void CsrH4DSInit(void)
{
    CSR_LOG_TEXT_REGISTER(&CsrH4dsLto, "H4DS", CSR_ARRAY_SIZE(subOrigins), subOrigins);

    H4DS_EVENT(H4DS_EVT_START);

    /* Receive path.  Must occur before CsrH4DSFsmInit(). */
    CsrH4DSRxInit();

    /* Transmit path. */
    CsrH4DSTxInit();

    /* Deep sleep state machine. */
    CsrH4DSFsmInit();

    /* Link establishment state machine. */
    CsrH4DSLeInit();

    /* h4ds transmit path animation. */
    CsrH4DSPumpInit();

    H4DS_EVENT(H4DS_EVT_INITIATED);
}

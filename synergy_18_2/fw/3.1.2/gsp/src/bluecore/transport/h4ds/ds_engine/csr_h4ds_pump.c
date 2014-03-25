/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_h4ds_private.h"


/* Have we made a request to the external code to call CsrH4DSPump(),
which has not yet been serviced? */

static CsrBool req_pump_tx;


/****************************************************************************
NAME
        CsrH4DSPumpInit  -  initialise the message pump
*/

void CsrH4DSPumpInit(void)
{
    req_pump_tx = FALSE;
}

/****************************************************************************
NAME
        CsrH4DSPump  -  transmit an h4ds message to the peer
*/

void CsrH4DSPump(void)
{
    req_pump_tx = FALSE;
    CsrH4DSTxPump();
}

/****************************************************************************
NAME
        CsrH4DSPumpTxReq  -  request a call to pump the h4ds tx code
*/

void CsrH4DSPumpTxReq(void)
{
    if (req_pump_tx)
    {
        return;
    }
    req_pump_tx = TRUE;
    H4DS_PUMP_REQ();
}

/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_abcsp_private.h"
#include "csr_queue_lib.h"

extern abcsp CsrAbcspInstanceData;

TXMSG *CsrAbcspQStart = NULL;

/****************************************************************************
*
* FUNCTION
*   Clean the TXMSG queue when deregister
*
*  RETURNS
*        number of messages removed from queue
*/

CsrUint32 CsrAbcspTxMsgQueueClean(void)
{
    TXMSG *m;
    CsrUint32 numTaken = 0;
    while (CsrAbcspQStart != NULL)
    {
        m = CsrQueueMsgTake(&CsrAbcspQStart);

        DELETE(m->m.buf);
        DELETE(m);
        numTaken++;
    }
    return numTaken;
}

/****************************************************************************
*
* FUNCTION
*   Is called from the pump bg interrupt. It takes a message in the abcsp
*    queue and send the message.
*   If there is no room in the abcsp the message is not deleted from the
*    abcsp queue.
*
*  RETURNS
*
*/

void CsrAbcspBgintSend(void)
{
    CsrUint8 chan;
    CsrUint32 rel;
    if (CsrAbcspQStart != NULL)
    {
        TXMSG *m;
        ABCSP_TXMSG *msg;

        rel = CsrAbcspQStart->seq;
        chan = CsrAbcspQStart->chan;
        msg = &CsrAbcspQStart->m;

        if (CsrAbcspMsgSend(msg, chan, rel)) /*Put a new message into the abcsp queue*/
        {
            m = CsrQueueMsgTake(&CsrAbcspQStart); /* remove message for linked queue*/
            DELETE(m);                /*CsrPmemFree the message holding structure */
        }
    }
}

/****************************************************************************
NAME
        CsrAbcspPumpTxMsgs  -  send message to the uart
*/
void CsrAbcspPumpTxMsgsOut(void)
{
    CsrAbcspPumpTxMsgs();

    CsrAbcspBgintSend();                /*Check abcsp queue and transmit if necessary */
}

/****************************************************************************
*    NAME
*        CsrAbcspReqPumpTxMsgs  -  push messages down the abcsp stack
*/

void CsrAbcspReqPumpTxMsgs(void)
{
    CsrSchedBgintSet(CsrAbcspInstanceData.bgint_pumptxmsg);   /* Set the bg_interrupt flag true*/
    /*Verify that there is no reentry problems.*/
}

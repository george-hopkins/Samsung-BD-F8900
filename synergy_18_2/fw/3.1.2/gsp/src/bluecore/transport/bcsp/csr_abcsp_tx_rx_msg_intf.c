/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_abcsp.h"
#include "csr_abcsp_config_event.h"
#include "csr_abcsp_config_panic.h"
#include "csr_abcsp_config_rxmsg.h"
#include "csr_abcsp_config_timer.h"
#include "csr_abcsp_config_txmsg.h"
#include "csr_abcsp_config_le.h"
#include "csr_pmem.h"
#include "csr_abcsp_private.h"
#include "csr_queue_lib.h"

/****************************************************************************
 *NAME
 *        void temp_abcsp_rxmsg_destroy(ABCSP_RXMSG *m)
 *FUNCTION
 *        Destroy the Rx message buffer and structure
 *RETURNS
 *        NON
 */


void CsrAbcspRxMsgDestroy(ABCSP_RXMSG *m)
{
}

/****************************************************************************
 *NAME
 *        void CsrAbcspTxMsgInitRead(ABCSP_TXMSG *msg)
 *FUNCTION
 *        Set the index pointer to start
 *RETURNS
 *        NON
 */

void CsrAbcspTxMsgInitRead(void *msg)
{
    MessageStructure *mm = (MessageStructure *) (msg);
    mm->dex = 0;
}

/****************************************************************************
 *NAME
 *        CsrUint32 CsrAbcspTxMsgLength(ABCSP_TXMSG *msg)
 *FUNCTION
 *
 *RETURNS
 *        how many payload bytes are in a message.
 */
CsrUint32 CsrAbcspTxMsgLength(ABCSP_TXMSG *msg)
{
    MessageStructure *mm = (MessageStructure *) (msg);

    return mm->buflen;
}

/****************************************************************************
 *NAME
 *        CsrUint8 *CsrAbcspTxMsgBufGet(ABCSP_TXMSG *msg, CsrUint32 *buflen)
 *FUNCTION
 *
 *RETURNS
 *        The remaining buffer space
 *        A pointer to the free space, if no more bytes return NULL
 */
CsrUint8 *CsrAbcspTxMsgBufGet(ABCSP_TXMSG *msg, CsrUint32 *buflen)
{
    MessageStructure *mm = (MessageStructure *) (msg);
    *buflen = mm->buflen - mm->dex;

    if (*buflen == 0)
    {
        return NULL;
    }
    else
    {
        return &(mm->buf[mm->dex]);
    }
}

/****************************************************************************
 *NAME
 *        void CsrAbcspTxMsgTaken(ABCSP_TXMSG *msg, CsrUint32 ntaken)
 *FUNCTION
 *        Update the number o bytes been consumed from the buffer obtained
 *        by the preceding call to CsrAbcspTxMsgBufGet.
 *
 *RETURNS
 *        NON
 */
void CsrAbcspTxMsgTaken(ABCSP_TXMSG *msg, CsrUint32 ntaken)
{
    MessageStructure *mm = (MessageStructure *) (msg);
    mm->dex += ntaken;
}

/****************************************************************************
 *NAME
 *        void CsrAbcspTxMsgDone(ABCSP_TXMSG *msg)
 *FUNCTION
 *        For an unreliable BCSP message (SCO) this means that the message has been send.
 *        For a reliable BCSP message this means that the message has been send to the peer
 *        and that the peer has returned acknowledgement of its reception. This can
 *        be used as the basics of a "recorded delivery" mechanism.
 *
 *RETURNS
 *        NON
 */
void CsrAbcspTxMsgDone(ABCSP_TXMSG *msg)
{
    MessageStructure *mm = (MessageStructure *) (msg);

    CsrPmemFree((void *) (mm->buf));
}

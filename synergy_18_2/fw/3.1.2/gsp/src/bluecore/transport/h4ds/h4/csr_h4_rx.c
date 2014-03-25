/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_queue_lib.h"
#include "csr_h4.h"
#include "csr_transport.h"
#include "csr_tm_bluecore_transport_descriptor.h"

/***********************************************************************
    Receives a full RX package from the lowest part of the H4
    driver. At this point the package is assembled and the channel
    is decoded. If the package is not a HCI-type this function
    must further decode the message to determine the channel and
    assemble fragmented messages (e.g. BCCMD messages).
***********************************************************************/
CsrUint16 CsrH4RxMsgPut(CsrUint8 *msg, CsrUint8 chan, CsrUint16 len, CsrBool *flag)
{
    MessageStructure msgStr;
    CsrBool err = 0;

    *flag = TRUE;

    switch (chan)
    {
        case (HCI_HDR_SCO):
            msgStr.chan = TRANSPORT_CHANNEL_SCO;
            break;
        case (HCI_HDR_ACL):
        {
            msgStr.chan = TRANSPORT_CHANNEL_ACL;
            break;
        }
        case (HCI_HDR_EVT):
        {
            msgStr.chan = TRANSPORT_CHANNEL_HCI;
            break;
        }
        default:
            /* This should never happen so do something dramatic */
            H4_EVENT(H4_EVT_DUFF_RX_PARAMS);
            err = 1;
            break;
    } /* end switch */

    if (err == 0)
    {
        msgStr.buf = msg;
        msgStr.buflen = len;
        msgStr.dex = 0;
        msgStr.chan = TRANSPORT_CHANNEL_ACL;

        CsrTmBlueCoreTransportMsgRx(CsrBluecoreTransportH4DS, &msgStr);
    }

    return len;
}

/***********************************************************************
    Used to free the message structure when it has been used.
***********************************************************************/
void CsrH4RxMsgFree(MessageStructure *msg)
{
    if (msg != NULL)
    {
        if (msg->buf != NULL)
        {
            CsrPmemFree(msg->buf);
        }
        msg = NULL;
    }
}

/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_h4ds_private.h"


/* Sleep rx message scanner state machine states. */
#define scst_lock           (0x0)             /* Matching "lock" part of a message. */
#define scst_body_start     (0x1)             /* Seeking the start of a message body. */
#define scst_in_body        (0x2)             /* Matching "body" part of a message. */


/* Associate a message's (fixed) byte sequence with an identifier. */
typedef struct
{
    const CsrUint8 *msg;           /* Message (body) byte sequence. */
    ds_msgid        id;            /* Corresponding message identifier. */
} MSGINFO;


/* The complete set of h4ds messages (and message components).  (A Wake-Up
message is composed of 1 or more CsrSchedMsgId_pre_wu sequences, one CsrSchedMsgId_prez_wu
sequence and one CsrSchedMsgId_wu_sequence.  As far as this file is concerned, these
components are treated as distinct messages.) */

static const MSGINFO h4ds_msgs[] =
{
    {CsrH4DSPreWuMsgfrag, CsrSchedMsgId_pre_wu},
    {CsrH4DSPrezWuMsgfrag, CsrSchedMsgId_prez_wu},
    {CsrH4DSWuMsgfrag, CsrSchedMsgId_wu},
    {CsrH4DSIaMsgfrag, CsrSchedMsgId_ia},
    {CsrH4DSYmsMsgfrag, CsrSchedMsgId_yms},
    {CsrH4DSSyncMsgfrag, CsrSchedMsgId_sync},
    {CsrH4DSSackMsgfrag, CsrSchedMsgId_sack},
    {CsrH4DSConfMsgfrag, CsrSchedMsgId_conf},
    {CsrH4DSCackMsgfrag, CsrSchedMsgId_cack},
    {(const CsrUint8 *) (NULL), CsrSchedMsgId_none}
};


/* State information for the message scanner. */
typedef struct
{
    CsrUint8       ix;              /* Match point. */
    CsrUint8       st;              /* State. */
    const MSGINFO *mi;             /* Message being matched. */
} SCANNER_INFO;
static SCANNER_INFO sc_info;


/* Forward reference. */
static const MSGINFO *match_msg_start(CsrUint8 b);


/****************************************************************************
NAME
        CsrH4DSRxInitMatchMsg  -  initialise h4ds message scanner
*/

void CsrH4DSRxInitMatchMsg(void)
{
    sc_info.st = scst_lock;
    sc_info.ix = 0;
}

/****************************************************************************
NAME
        CsrH4DSRxMatchMsg  -  h4ds message scanner
*/

ds_msgid CsrH4DSRxMatchMsg(const CsrUint8 *buf, CsrUint16 len, CsrUint16 *ntaken)
{
    CsrUint16 i;

    for (i = 0; i < len; i++, buf++)
    {
        switch (sc_info.st)
        {
            case scst_lock:
                /* Match the "lock" - the common byte sequence that
                starts all h4ds layer messages. */

                if (*buf == CsrH4DSLockMsgfrag[sc_info.ix++])
                {
                    if (sc_info.ix >= H4DS_MSGFRAG_LOCK_LEN)
                    {
                        sc_info.st = scst_body_start;
                    }
                }
                else
                {
                    /* The ill-matching byte may be the start
                    of a good message, so don't consume it. */

                    CsrH4DSRxInitMatchMsg();
                    *ntaken = i;
                    return CsrSchedMsgId_duff;
                }
                break;
            case scst_body_start:
                if ((sc_info.mi = match_msg_start(*buf))
                    != (MSGINFO *) (NULL))
                {
                    sc_info.ix = 1;
                    sc_info.st = scst_in_body;
                }
                else
                {
                    /* The ill-matching byte may be the start
                    of a good message, so don't consume it. */

                    CsrH4DSRxInitMatchMsg();
                    *ntaken = i;
                    return CsrSchedMsgId_duff;
                }
                break;
            case scst_in_body:
                if (*buf == sc_info.mi->msg[sc_info.ix++])
                {
                    if (sc_info.ix >= H4DS_MSGFRAG_BODY_LEN)
                    {
                        /* Must consume final msg byte. */

                        *ntaken = i + 1;
                        CsrH4DSRxInitMatchMsg();
                        return sc_info.mi->id;
                    }
                }
                else
                {
                    /* The ill-matching byte may be the start
                    of a good message, so don't consume it. */

                    CsrH4DSRxInitMatchMsg();
                    *ntaken = i;
                    return CsrSchedMsgId_duff;
                }
                break;
            default:
                H4DS_PANIC(H4DS_PANIC_INT_CORRUPTION);
                break;
        }
    }

    /* We've run out of input bytes. */
    *ntaken = len;
    return CsrSchedMsgId_none;
}

/****************************************************************************
NAME
        match_msg_start  -  identify an h4ds message from its first byte

FUNCTION
        Each h4ds message body starts with a different byte.  This function
        obtains information for the h4ds message body which starts with "b".

RETURNS
        A MSGINFO for which the first byte of the corresponding message is
        "b", or NULL if there is no such message.
*/

static const MSGINFO *match_msg_start(CsrUint8 b)
{
    const MSGINFO *mi;

    for (mi = h4ds_msgs; mi->msg; mi++)
    {
        if (b == mi->msg[0])
        {
            return mi;
        }
    }

    return (const MSGINFO *) (NULL);
}

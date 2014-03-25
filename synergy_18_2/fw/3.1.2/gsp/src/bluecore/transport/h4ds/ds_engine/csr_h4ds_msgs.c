/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_h4ds_private.h"


/* h4ds messages' fixed "sync" preamble. */
const CsrUint8 CsrH4DSLockMsgfrag[H4DS_MSGFRAG_LOCK_LEN] =
{0x40, 0x0a, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0x50};


/* h4ds "You May Sleep" message, equivalent to {'y', 'm', 's', '\0'}. */
const CsrUint8 CsrH4DSYmsMsgfrag[H4DS_MSGFRAG_BODY_LEN] =
{0x79, 0x6d, 0x73, 0x00};

/* h4ds "Wake Up" message, equivalent to {'w', 'u', '\0', '\0'}. */
const CsrUint8 CsrH4DSWuMsgfrag[H4DS_MSGFRAG_BODY_LEN] =
{0x77, 0x75, 0x00, 0x00};

/* Preamble to an h4ds "Wake Up" msg, equivalent to {'t', 'i', 'c', 'k'}. */
const CsrUint8 CsrH4DSPreWuMsgfrag[H4DS_MSGFRAG_BODY_LEN] =
{0x74, 0x69, 0x63, 0x6b};

/* Preamble to an h4ds "Wake Up" msg. */
const CsrUint8 CsrH4DSPrezWuMsgfrag[H4DS_MSGFRAG_BODY_LEN] =
{0x00, 0x00, 0x00, 0x00};

/* h4ds "I'm Awake" message, equivalent to {'i', 'a', '\0', '\0'}. */
const CsrUint8 CsrH4DSIaMsgfrag[H4DS_MSGFRAG_BODY_LEN] =
{0x69, 0x61, 0x00, 0x00};

/* h4ds link establishment "Sync" message, equiv to {'s', 'y', 'n', 'c'}. */
const CsrUint8 CsrH4DSSyncMsgfrag[H4DS_MSGFRAG_BODY_LEN] =
{0x73, 0x79, 0x6e, 0x63};

/* h4ds link establishment "Sync Ack" msg, equiv to {'S', 'a', 'c', 'k'}. */
const CsrUint8 CsrH4DSSackMsgfrag[H4DS_MSGFRAG_BODY_LEN] =
{0x53, 0x61, 0x63, 0x6b};

/* h4ds link establishment "Conf" message, equiv to {'c', 'o', 'n', 'f'}. */
const CsrUint8 CsrH4DSConfMsgfrag[H4DS_MSGFRAG_BODY_LEN] =
{0x63, 0x6f, 0x6e, 0x66};

/* h4ds link establishment "Conf Ack" msg, equiv to {'C', 'a', 'c', 'k'}. */
const CsrUint8 CsrH4DSCackMsgfrag[H4DS_MSGFRAG_BODY_LEN] =
{0x43, 0x61, 0x63, 0x6b};

#ifndef CSR_H4DS_MSGS_H__
#define CSR_H4DS_MSGS_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_h4ds_private.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Message identifiers. */
enum CsrSchedMsgId_enum
{
    CsrSchedMsgId_none,               /* No message. */
    CsrSchedMsgId_duff,               /* Unknown or corrupt message detected. */
    CsrSchedMsgId_wu,                 /* Wake-Up. */
    CsrSchedMsgId_pre_wu,             /* Preamble to Wake-Up. */
    CsrSchedMsgId_prez_wu,            /* Zero preamble to Wake-Up. */
    CsrSchedMsgId_ia,                 /* I'm-Awake. */
    CsrSchedMsgId_yms,                /* You-May-Sleep. */
    CsrSchedMsgId_sync,               /* Link establishment "Sync". */
    CsrSchedMsgId_sack,               /* Link establishment "Sync Ack". */
    CsrSchedMsgId_conf,               /* Link establishment "Conf". */
    CsrSchedMsgId_cack,               /* Link establishment "Conf Ack". */
    CsrSchedMsgId_h4                  /* h4 layer: cmd/evt, ACL or SCO. */
};
typedef enum CsrSchedMsgId_enum ds_msgid;


/* Message layer identifiers. */
enum msglayer_enum
{
    msglayer_h4ds,            /* h4ds layer. */
    msglayer_h4,              /* h4 layer. */
    msglayer_none             /* Unknown layer. */
};
typedef enum msglayer_enum msglayer;


/* All h4ds messages are fixed sequences of 12 bytes, consisting of an 8
byte LOCK preamble plus 4 bytes for the message body.  The LOCK preamble
is identical for all 7 message types.  (This description ignores the
variable length byte train that forms the start of a Wake-Up message.) */

#define H4DS_MSGFRAG_LOCK_LEN  (8)
#define H4DS_MSGFRAG_BODY_LEN  (4)
#define H4DS_MSG_LEN           (H4DS_MSGFRAG_LOCK_LEN + H4DS_MSGFRAG_BODY_LEN)

/* h4ds messages' fixed "lock" preamble. */
extern const CsrUint8 CsrH4DSLockMsgfrag[H4DS_MSGFRAG_LOCK_LEN];

/* h4ds messages' fixed bodies. */
extern const CsrUint8 CsrH4DSYmsMsgfrag[H4DS_MSGFRAG_BODY_LEN];
extern const CsrUint8 CsrH4DSWuMsgfrag[H4DS_MSGFRAG_BODY_LEN];
extern const CsrUint8 CsrH4DSPreWuMsgfrag[H4DS_MSGFRAG_BODY_LEN];
extern const CsrUint8 CsrH4DSPrezWuMsgfrag[H4DS_MSGFRAG_BODY_LEN];
extern const CsrUint8 CsrH4DSIaMsgfrag[H4DS_MSGFRAG_BODY_LEN];
extern const CsrUint8 CsrH4DSSyncMsgfrag[H4DS_MSGFRAG_BODY_LEN];
extern const CsrUint8 CsrH4DSSackMsgfrag[H4DS_MSGFRAG_BODY_LEN];
extern const CsrUint8 CsrH4DSConfMsgfrag[H4DS_MSGFRAG_BODY_LEN];
extern const CsrUint8 CsrH4DSCackMsgfrag[H4DS_MSGFRAG_BODY_LEN];

#ifdef __cplusplus
}
#endif

#endif

#ifndef CSR_ABCSP_LE_H__
#define CSR_ABCSP_LE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* BCSP-LE state machine states. */
enum bcsp_le_state_enum
{
    state_shy,
    state_curious,
    state_garrulous
};
typedef enum bcsp_le_state_enum bcsp_le_state;


/* LE message identifiers - signals sent to/from bcsp-le state machines. */
enum lemsgid_enum
{
    lemsgid_none,               /* no message */
    lemsgid_sync,               /* sync message */
    lemsgid_sync_resp,          /* sync-resp message */
    lemsgid_conf,               /* conf message */
    lemsgid_conf_resp,          /* conf-resp message */
    lemsgid_tshy_timeout,       /* message indicating Tshy timeout */
    lemsgid_tconf_timeout       /* message indicating Tconf timeout */
};
typedef enum lemsgid_enum lemsgid;

/* Messages sent to/from the peer bcsp-le state machine.  All messages
are the same length, simplifying some of the code.   Messages are global
as they are used by the receive and transmit paths*/

#define BCSPLE_MSGLEN           (4)

/* BCSP-LE messages are all fixed-format. */
extern const CsrUint8 CsrAbcspLeSyncMsg[];
extern const CsrUint8 CsrAbcspLeSyncrespMsg[];
extern const CsrUint8 CsrAbcspLeConfMsg[];
extern const CsrUint8 CsrAbcspLeConfrespMsg[];

struct le_state
{
    bcsp_le_state state;

#ifdef ABCSP_USE_OLD_BCSP_LE
    CsrUint8 conf_cnt;
#endif
};

#ifdef __cplusplus
}
#endif

#endif

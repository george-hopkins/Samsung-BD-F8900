#ifndef _DM_SM_TYPES_H_
#define _DM_SM_TYPES_H_

#include "csr_synergy.h"
/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              dm_sm_types.h

DESCRIPTION:       Types exported by the security manager.



****************************************************************************/

#include "csr_sched.h"
#include "dm_prim.h"
#include "sm.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
 * PURPOSE
 *     The Security Manager must keep track of simple pairing parameters
 *     while it is in progress. This cannot be part of DM_SM_REQUEST_T
 *     because it is possible for the pairing process to start without
 *     a request from the application or L2CAP.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    CsrUint8 io_cap_local;
    CsrUint8 io_cap_remote;
    CsrUint8 mitm_local;
    CsrUint8 mitm_remote;
    CsrUint8 *oob_hash_c;
    CsrUint8 *oob_rand_r;
} DM_SM_SSP_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      The Security Manager queues up requests relating to each ACL, and
 *      processes them one at a time.
 *
 *----------------------------------------------------------------------------*/
typedef struct DM_SM_REQUEST_T_tag
{
    /* Pointer to next queued request */
    struct DM_SM_REQUEST_T_tag *p_next;

    /* A copy of the request primitive */
    union
    {
        dm_prim_t type;
        DM_SM_ACCESS_REQ_T access_req;
        DM_SM_AUTHENTICATE_REQ_T auth_req;
        DM_SM_ENCRYPT_REQ_T enc_req;
        DM_SM_BONDING_REQ_T bond_req;
        DM_SM_SECURITY_REQ_T security_req;
    } u;
    /* Security timers */

    /* Delay to allow for late reception of HCI encryption event, before
       disconnecting after receiving a connection request on an unencrypted
       link when both local and remote devices are in SM4. */
    CsrSchedTid detach_timer;

    /* Reauthentication delay to avoid the multiple authentication
       attempt error. We will sometimes try to reauthenticate after
       a KEY_MISSING error or a transaction collision. Also we wait
       before retrying encryption requests. */
    CsrSchedTid transaction_holdoff_timer;

    /* Keep track of how many times we've had collisions. */
#define DM_SM_AUTH_RETRIES_MAX 0x03
#define DM_SM_ENC_RETRIES_MAX 0x10
    CsrUint8 transaction_retries;

    /* For dedicated bonding, pairing is essential. It is not enough
       to authenticate existing link keys. Mostly we can avoid that,
       but if the application is not using security mode 4 and is
       determined to do bonding with an existing ACL connection the
       LM might use its locally stored link key to authenticate. If
       this happens then we should indicate failure to the application.
       See B-52426 */
    hci_error_t bonding_status;

    /* Security requirements needed for an access request
     * This bitfield should be tested with the combined incoming/outgoing
     * #defines in dm_prim.h. When it is set, the settings inappropriate
     * to this sort of connection will already have been masked out.
     */
    dm_security_level_t seclevel;
} DM_SM_REQUEST_T;

/* Security Flags */
#define DM_SM_FLAG_RSF_KNOWN                ((CsrUint16) 0x0001) /* Remote supported features known. */
#define DM_SM_FLAG_RSF_SSP                  ((CsrUint16) 0x0002) /* Remote device has controller support for Secure Simple Pairing. */
#define DM_SM_FLAG_RHSF_KNOWN               ((CsrUint16) 0x0004) /* Remote host supported features known. */
#define DM_SM_FLAG_RHSF_SSP                 ((CsrUint16) 0x0008) /* Remote device has full support for Secure Simple Pairing. */
#define DM_SM_FLAG_KEY_MISSING              ((CsrUint16) 0x0010) /* A link key has gone missing so we are re-pairing. */
#define DM_SM_FLAG_NEED_ENCRYPTION_REFRESH  ((CsrUint16) 0x0020) /* Encryption refresh required on this link. */
#define DM_SM_FLAG_KEY_PENDING              ((CsrUint16) 0x0040) /* We are at the link-key part of authentication. */

#define DM_SM_FLAG_RSF_RHSF_KNOWN (DM_SM_FLAG_RSF_KNOWN | DM_SM_FLAG_RHSF_KNOWN)
#define DM_SM_FLAG_RSF_RHSF_SSP   (DM_SM_FLAG_RSF_SSP | DM_SM_FLAG_RHSF_SSP)


/* SM Data in the DM ACL structure. */
typedef struct
{
    CsrUint16 flags;
    fsm_state_t state;
#ifdef BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE
    CsrBool encrypting;
#endif /* BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE */
    CsrBool encrypted;
    DM_SM_REQUEST_T *p_requests;
    union
    {
        DM_SM_SSP_T *p_ssp;
        SM_PAIRING_T *p_sm;
    } u;
} DM_SM_T;

#ifdef __cplusplus
}
#endif

#endif


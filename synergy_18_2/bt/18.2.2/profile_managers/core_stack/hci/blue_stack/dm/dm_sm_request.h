#ifndef _DM_SM_REQUEST_H_
#define _DM_SM_REQUEST_H_

#include "csr_synergy.h"
/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              dm_sm_request.h

DESCRIPTION:       The security management entity of the BlueStack Device Manager.



****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
    Public Defines
 *============================================================================*/
/* None */

/*============================================================================*
    Public Data Types
 *============================================================================*/

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
    CsrSchedTid retry_timer;

    /* Keep track of how many times we've had collisions. */
#define DM_SM_AUTH_RETRIES_MAX 0x03
#define DM_SM_ENC_RETRIES_MAX 0x10
    CsrUint8 transaction_retries;

    /* Security requirements needed for an access request
     * This bitfield should be tested with the combined incoming/outgoing
     * #defines in dm_prim.h. When it is set, the settings inappropriate
     * to this sort of connection will already have been masked out.
     */
    dm_security_level_t seclevel;
} DM_SM_REQUEST_T;

/*============================================================================*
    Public Data
 *============================================================================*/
/* None */

/*============================================================================*
    Public Functions
 *============================================================================*/
/* None */

#ifdef __cplusplus
}
#endif

#endif


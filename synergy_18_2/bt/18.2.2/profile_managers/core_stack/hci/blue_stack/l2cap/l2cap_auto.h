/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_auto.h

\brief  Header file for the L2CAP auto-connect system

        L2CAP auto-connect is a helper system that can do connect and
        configuration in a single step as seen from the upper
        layer. The upper layer simply provides a set of
        key,value-pairs with the acceptable parameters and L2CAP will
        then do the full connect and configuration negotiation -- and
        hopefully end up with a usable channel.
*/
#ifndef _L2CAP_AUTO_H
#define _L2CAP_AUTO_H

#include "csr_synergy.h"

#include "csr_bt_core_stack_fsm.h"
#include "csr_bt_common.h"
#include "csr_mblk.h"
#include "l2cap_types.h"
#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "l2cap_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/*! \brief Handler function return codes */
typedef enum {
    L2RS_UNHANDLED = 0,                        /*!< Primitive not handled */
    L2RS_HANDLED,                              /*!< Primitive handled and pointer-members consumed */
    L2RS_READY,                                /*!< Primitive handled and channel ready */
    L2RS_FAILED                                /*!< Primitive handled, but channel could not be opened */
} L2CAUTO_RESULT_T;

/*! \brief State bitmasks
 */
#define L2CAUTO_AMP                         0x0001 /*!< AMP enabled */
#define L2CAUTO_OUTGOING                    0x0002 /*!< Outgoing connection */
#define L2CAUTO_CONNECTION_OK               0x0004 /*!< Connection/channel up */
#define L2CAUTO_FEATS_REQUESTED             0x0008 /*!< GetInfo extended features sent */
#define L2CAUTO_FEATS_OK                    0x0010 /*!< GetInfo response received */
#define L2CAUTO_CONFIG_REQUESTED            0x0020 /*!< Configuration request sent */
#define L2CAUTO_CONFIG_LOCAL_OK             0x0040 /*!< Peer has replied success to our config */
#define L2CAUTO_CONFIG_REMOTE_OK            0x0080 /*!< Application has replied success to peer's config */
#define L2CAUTO_ACTIVE_OK                   0x0100 /*!< Link is now fully active (other OKs can be cleared) */
#define L2CAUTO_RECONFIG_IND                0x0200 /*!< Reconfig ind received, results in "backoff" buffer */
#define L2CAUTO_DYING                       0x0400 /*!< Disconnecting due to error - app already notified */
#define L2CAUTO_NO_FLOWSPEC_RECONF          0x0800 /*!< Don't use flowspecs in ongoing reconfiguration */
#define L2CAUTO_AWAITING_AUTO_CONNECT_RSP   0x1000 /*!< Host must send us L2CA_AUTO_CONNECT_RSP */
#define L2CAUTO_DESTRUCTION_ATTEMPTED       0x2000 /*!< We've already tried to kill this once */

/*! \brief Auto-connect instance data structure

    We need to store a few bits of information while the
    auto-connection is in progress. As soon as the channel enters the
    open state, we can free this data
*/
typedef struct l2cautotag
{
    struct l2cautotag      *next;              /*!< Pointer to next block */
    CsrUint16                state;             /*!< State bitmask */
    CsrSchedQid               phandle;           /*!< Application handle */
    l2ca_cid_t              cid;               /*!< L2CAP assigned CID */
    psm_t                   remote_psm;        /*!< Remote PSM number */
    psm_t                   local_psm;         /*!< Local PSM number */
    BD_ADDR_T               bd_addr;           /*!< Remote BT address */
    CsrUint16                con_ctx;           /*!< Connection context */
    CsrUint16                reg_ctx;           /*!< Registration context */

    l2ca_ext_feats_t        ext_feats;         /*!< Remote get-info extended features response */
    DM_SM_SERVICE_T        *dm_sm_service;     /*!< Replacement Security Service Description */
    l2ca_options_t          unknown;           /*!< Peer doesn't know about these options */
    L2CA_CONFIG_T          *backoff;           /*!< Unacceptable acceptable/backoff parameters */
    l2ca_options_t          disable_reconf;    /*!< Which options to not allow reconf for */
    L2CA_CONFIG_T          *results;           /*!< Resulting config parameters for peer */

    CsrUint16               *conftab;           /*!< Key,value configuration table */
    CsrUint16                length;            /*!< Number of CsrUint16's in the conftab */
    CsrUint16                num_blocks;        /*!< Number of reachable blocks */
    CsrUint16                curr_block;        /*!< Current block index */
    CsrUint8                 retry;             /*!< Retry counter to detect misbehaving peers */
    CsrUint16                peer_modes_left;   /*!< Modes peer is allowed to ask for */
    CsrUint16                channel_priority;  /*!< L2CAP CID priority */
    L2CA_AUTO_CONNECT_CFM_T *queued_confirm;   /*!< We have a CFM ready to send upwards */
    CsrUint16               *saved_conftab;     /*!< Saved Key,value configuration table */
    CsrUint16                saved_length;      /*!< Saved number of CsrUint16's in the conftab */
} L2CAUTO_INSTANCE_T;

/* Main functions */
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
extern void L2CAUTO_Free(L2CAUTO_INSTANCE_T *instance,
                         l2ca_cid_t cid);
extern void L2CAUTO_Deinit(void);
extern L2CAUTO_RESULT_T L2CAUTO_HandleL2ca(l2ca_cid_t cid,
                                           const BD_ADDR_T *bd_addr,
                                           psm_t local_psm,
                                           L2CAUTO_INSTANCE_T *user_inst,
                                           L2CA_UPRIM_T *prim);
#endif


#ifdef __cplusplus
}
#endif
#endif


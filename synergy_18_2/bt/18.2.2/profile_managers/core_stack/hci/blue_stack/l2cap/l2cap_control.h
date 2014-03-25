/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_control.h

\brief  Miscellenous control functions for handling PSMs, fixed
        channels, connectionless rx/tx setup, etc.  We also control
        creation of CIDCBs for both incoming and outgoing connections.
*/

#ifndef _L2CAP_CONTROL_H    /* Once is enough */
#define _L2CAP_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_bt_common.h"
#include "l2cap_config.h"
#include "l2cap_con_handle.h"
#include "l2cap_cid.h"
#include "dm_acl_core.h"

/*! \brief Connectionless receive status */
typedef enum
{
    CL_RX_ENABLE_NONE = 0,
    CL_RX_ENABLE_ALL,
    CL_RX_ENABLE_INDIVIDUAL
} cl_rx_status_t;

/*! \brief PSM register structure

    This structure contains the registration information for a PSM, for each
    PSM registered a instance of this structure is created
*/
typedef struct psm_tag_t
{
    struct psm_tag_t    *next_ptr;                      /*!< Pointer to next instance or NULL if last in list */
    CsrSchedQid            p_handle;                      /*!< Protocol handle to owner who registered this PSM */
    CsrUint16             psm;                           /*!< PSM value */

    CsrUint16             regflags;                      /*!< Registration config */
    CsrUint16             reg_ctx;                       /*!< Registration context */

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    CsrUint16             mode_mask;                     /*!< Modes supported by this application */
#endif
} PSM_T;

/*! \brief CID fifo for L2CAP connect retry.

    When L2CAP decides to retry a failed connect attempt, it adds the local CID
    that it was alloctated to a fifo. This reserves the CID. It is taken off
    again when the connect retry is processed.
*/
typedef struct l2cap_cid_list_tag_t
{
    struct l2cap_cid_list_tag_t *next_ptr;
    l2ca_cid_t                  cid;
} L2CAP_CID_LIST_T;

/*! \brief Fixed channel register structure

    This structure holds registrations for fixed channels needed by
    AMP. It also holds a configuration, which shall always be used for
    the particular channel.
*/
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
typedef struct cid_tag_t
{
     struct cid_tag_t   *next_ptr;                      /*!< Pointer to next instance or NULL if last in list */
    CsrSchedQid            p_handle;                      /*!< Protocol handle to owner who registered this CID */
    l2ca_cid_t           cid;                           /*!< CID */
    L2CA_CONFIG_T        config;                        /*!< Channel config */
} FIXCID_T;
#endif


/*! \brief Logical channel to queue mappings

    When we receive NCPs back from AMP queues, we need to quickly look
    up which queue and CHCB the NCP was for, given the logical
    handle. This structure provides the lookup cache
*/
#ifdef INSTALL_AMP_SUPPORT
typedef struct logiq_tag_t
{
    struct logiq_tag_t  *next;
    amp_link_id_t        logical_handle;
    TXQUEUE_T           *queue;
    L2CAP_CHCB_T        *chcb;
} LOGIC_Q_LOOKUP_T;
#endif

/*! \brief L2CAP Control Block.

    The L2CAP Control Block holds the parameters needed to manage
    connections, channels and registered PSMs.  It also contains parameters
    to manage data credits.
*/
typedef struct
{
    PSM_T               *psm_list;                      /*!< Pointer to first PSM structure in list */
    CIDCB_T             *cid_table[L2CAP_MAX_NUM_CIDS]; /*!< Table of pointers to dynamic CIDCB instances */
    L2CAP_CID_LIST_T    *retry_cid_fifo;                /*!< CID fifo. Preserves CID during connect retry */

#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
    FIXCID_T            *fixcid_list;                   /*!< Fixed channel registrations */
#endif
#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
    L2CAP_CHCB_T        *connectionless;                /*!< Pointer to common CHCB for connectionless TX */
#endif

    struct l2cautotag   *autoconnect;                   /*!< Auto-connect instances */

#ifdef INSTALL_AMP_SUPPORT
    LOGIC_Q_LOOKUP_T    *logical_lookup;                /*!< Queue/CHCB lookup table for logical channel IDs */
#endif

    CsrUint16            cid_counter;                    /*!< Incrementing counter for top 10 bits of CID */
} MCB_T;

/* The static placeholder for the L2CAP master control block */
extern MCB_T mcb;

/* Startup and shutdown */
extern void MCB_Init(void);
#ifdef ENABLE_SHUTDOWN
extern void MCB_DeInit(void);
#endif

/* Standard L2CAP control functions */
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
extern void MCB_RegisterProtocol(L2CA_REGISTER_REQ_T *req);
extern void MCB_UnregisterProtocol(L2CA_UNREGISTER_REQ_T *req);
extern void MCB_ConnectReq(L2CA_CONNECT_REQ_T *req);
extern CsrUint16 MCB_ConnectInd(L2CAP_CHCB_T *chcb, CID_CONNECT_REQ_T *sig_args);
extern void MCB_DeviceReady(void);
extern void MCB_PingReq(L2CA_PING_REQ_T *prim);
extern void MCB_GetInfoReq(L2CA_GETINFO_REQ_T *prim);
#else
#define MCB_DeviceReady();
#endif

extern CsrSchedQid MCB_GetPhandle(psm_t psm);
extern PSM_T *MCB_FindProtocol(psm_t psm);
extern void MCB_SetRawMode(L2CA_RAW_MODE_REQ_T *req);

/* Fixed channels */
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
extern CsrBool MCB_ReceiveConnectionlessAllowed(psm_t psm, CsrBool bc);
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
extern void MCB_RegisterFixedCid(L2CA_REGISTER_FIXED_CID_REQ_T *req);
extern CsrSchedQid MCB_GetPhandleFromFixedCid(l2ca_cid_t cid);
extern L2CA_CONFIG_T *MCB_GetConfigFromFixedCid(l2ca_cid_t cid);
#else
#define MCB_GetPhandleFromFixedCid(fixed_cid) CSR_SCHED_QID_INVALID
#define MCB_GetConfigFromFixedCid(fixed_cid) NULL
#endif
extern void MCB_MapFixedCidReq(L2CA_MAP_FIXED_CID_REQ_T *req);
extern CIDCB_T *MCB_MapFixedCidInd(L2CAP_CHCB_T *chcb, l2ca_cid_t fixed_cid, psm_t cl_psm);
extern void MCB_MapFixedCidRsp(L2CA_MAP_FIXED_CID_RSP_T *rsp);
extern void MCB_UnmapFixedCidReq(L2CA_UNMAP_FIXED_CID_REQ_T *req);
#endif

/* CID fifo */
extern void MCB_RetryCidFifoPush(l2ca_cid_t cid);
extern l2ca_cid_t MCB_RetryCidFifoPop(void);
extern CsrBool MCB_CidIsAvailable(l2ca_cid_t cid);

/* AMP extensions */
#ifdef INSTALL_AMP_SUPPORT
extern void MCB_AmpCreateReq(L2CA_CREATE_CHANNEL_REQ_T *req);
extern CsrUint16 MCB_AmpCreateInd(L2CAP_CHCB_T *chcb, CID_CREATE_CHAN_REQ_T *sig_args);
#endif

#ifdef INSTALL_ULP
extern void L2CA_ConnUpdateReq(CsrSchedQid phandle,
                        const TYPED_BD_ADDR_T *addrt,
                        CsrUint16 min_interval,
                        CsrUint16 max_interval,
                        CsrUint16 latency,
                        CsrUint16 timeout
                        );

extern void L2CA_AcceptConnUpdateRsp(CsrSchedQid phandle,
                              const TYPED_BD_ADDR_T *addrt,
                              CsrUint16 result,
                              l2ca_identifier_t signal_id);

#endif

/* Callback function used by LC/DM/etc on-chip */
#ifndef BUILD_FOR_HOST
extern void l2cap_low_memory(void);
#endif

#ifdef __cplusplus
}
#endif

#endif

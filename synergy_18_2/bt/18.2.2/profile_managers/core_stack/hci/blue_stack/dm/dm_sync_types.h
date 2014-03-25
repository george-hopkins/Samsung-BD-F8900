#ifdef INSTALL_DM_SYNC_MODULE
/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_sync_types.h

\brief  Sync Manager definitions required for the ACL record structure.
*/

#ifndef _DM_SYNC_CLIENT_H
#define _DM_SYNC_CLIENT_H

#include "csr_synergy.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef CsrUint8 sync_initiator_t;

/* Single linked list for registered handlers of incoming connections. */
typedef struct DM_INCOMING_SYNC_HANDLE_T_tag
{
    struct DM_INCOMING_SYNC_HANDLE_T_tag *p_next;

    CsrSchedQid phandle;      /* Registered task handle */
    CsrUint16  pv_cbarg;     /* Callback argument supplied by registered task */
} DM_INCOMING_SYNC_HANDLER_T;

/* Try to resolve connect collisions by introducing a small delay. */
typedef struct
{
    HCI_EV_CONN_REQUEST_T *prim;
    CsrSchedTid delay;
} DM_SYNC_CONNECT_COLLISION_T;

typedef struct DM_SYNC_T_tag
{
    struct DM_SYNC_T_tag *p_next;

    hci_connection_handle_t handle;     /* HCI connection handle */
    DM_SYNC_CONFIG_T config;

    BITFIELD(CsrBool, rejected, 1);
    BITFIELD(CsrBool, peer_connection_accepted, 1);
    BITFIELD(sync_initiator_t, initiator, 2);   /* Peer- or locally-initiated */
    BITFIELD(CsrUint8, req_count, 5);

    BITFIELD(CsrBool, initiated_disconnect, 1);  /* Locally initiated disconnection */
    BITFIELD(CsrBool, initiated_renegotiate, 1); /* Locally initiated renegotiation */
    BITFIELD(CsrBool, requested, 1);
    BITFIELD(CsrBool, assigned, 1);
    BITFIELD(CsrBool, is_esco, 1);

    /* Task handle and callback argument for the 'owner' of this SCO connection.
     * The 'assigned' flag indicates for incoming SCO connections whether the
     * connection has been successfully assigned to an owner - if not, then the
     * phandle and pv_cbarg identify the current position in the list of
     * registered handlers while the DM is trying to assign an owner.
     */
    DM_INCOMING_SYNC_HANDLER_T *handler;

    /* Try to resolve connect collisions by introducing a small delay. */
    DM_SYNC_CONNECT_COLLISION_T *connection_collision;
} DM_SYNC_T;

#ifdef __cplusplus
}
#endif

#endif
#endif

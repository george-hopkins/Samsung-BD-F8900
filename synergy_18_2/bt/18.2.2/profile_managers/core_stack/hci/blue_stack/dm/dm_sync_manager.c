/****************************************************************************

                (c) CSR plc 2010

                All rights reserved

FILE:           dm_sync_manager.h

DESCRIPTION:    DM synchroneous connection manager.



****************************************************************************/

#include "csr_synergy.h"

/*=============================================================================*
    ANSI C & System-wide Header Files
 *============================================================================*/

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "csr_bt_common.h"
#include "error.h"
#include "csr_bt_core_stack_fsm.h"
#include "tbdaddr.h"

#ifndef BUILD_FOR_HOST
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "io_defs.h"
#include "hostio.h"
#include "psbc.h"
#include "buffer.h"
#else
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_usr_config.h"
#endif

#include "csr_util.h"

#ifdef INSTALL_DM_SYNC_MODULE

/*============================================================================*
    Interface Header Files
 *============================================================================*/

#include "hci_prim.h"
#include "l2cap_prim.h"
#include "dm_prim.h"

/*============================================================================*
    Local Header File
 *============================================================================*/

#include "dm_acl_core.h"
#include "dm_acl_manager.h"
#include "dm_acl_policy_manager.h"
#include "dm_security_manager.h"
#include "dm_sync_manager.h"
#include "dm_layer_manager.h"
#include "dm_hci_interface.h"
#include "dm_mgt_interface.h"

/*============================================================================*
    Private Defines
 *============================================================================*/

#define GET_SYNC_DATA(p_acl) ((DM_SYNC_T **)DM_ACL_CLIENT_GET_DATA((p_acl), dm_acl_client_sync))

#define SYNC_DISPATCH_RESTART_DELAY (100 * CSR_SCHED_MILLISECOND)
#define SYNC_CONNECT_COLLISION_DELAY (500 * CSR_SCHED_MILLISECOND)

/*============================================================================*
    Private Data Types
 *============================================================================*/

typedef struct
{
    CsrUint8 link_type;
    CsrUint8 tx_interval;
    CsrUint8 wesco;
    CsrUint16 rx_packet_length;
    CsrUint16 tx_packet_length;
    CsrUint8 air_mode;
} DM_SYNC_FROM_AIR_CONFIG_T;

/*============================================================================*
    Private Data
 *============================================================================*/

/* Linked list of registered incoming Sync handlers */
static DM_INCOMING_SYNC_HANDLER_T *p_handler_list = NULL;


/*============================================================================*
    Private Function Prototypes
 *============================================================================*/

/* DM primitive issuing functions */
static void send_dm_sync_connect_complete_from_sync(
    DM_SYNC_T *p_sync,
    hci_return_t status,
    BD_ADDR_T *bd_addr,
    HCI_EV_SYNC_CONN_COMPLETE_T *hci_prim,
    DM_SYNC_CONNECT_COMPLETE_T **dm_prim);
static void send_dm_sync_connect_complete(
    sync_initiator_t initiator,
    hci_return_t status,
    hci_connection_handle_t handle,
    BD_ADDR_T *bd_addr,
    CsrSchedQid phandle,
    CsrUint16 pv_cbarg,
    HCI_EV_SYNC_CONN_COMPLETE_T *hci_prim,
    DM_SYNC_CONNECT_COMPLETE_T **dm_prim
    );
static void send_dm_sync_connect_ind(DM_SYNC_T *p_sync, DM_ACL_T *p_acl);
static void send_dm_sync_renegotiate_indcfm(
    dm_prim_t type,
    hci_return_t status,
    DM_SYNC_T *p_sync,
    HCI_EV_SYNC_CONN_CHANGED_T *p_hci_prim
    );
static void send_dm_sync_disconnect_ind(
    DM_SYNC_T *p_sync,
    hci_reason_t reason,
    BD_ADDR_T *p_bd_addr);
static void send_dm_sync_disconnect_cfm(
    DM_SYNC_T *p_sync,
    hci_return_t status,
    BD_ADDR_T *bd_addr);
static void send_dm_sync_register_cfm(
    CsrSchedQid phandle,
    CsrUint16 pv_cbarg
    );
static void send_dm_sync_unregister_cfm(
    CsrSchedQid phandle,
    CsrUint16 pv_cbarg,
    CsrUint8 status
    );
static void Send_HCI_Setup_Synchronous_Connection(
        hci_connection_handle_t handle,
        DM_SYNC_CONFIG_T *config
        );
static void Send_HCI_Accept_Synchronous_Connection(
        BD_ADDR_T *bd_addr,
        DM_SYNC_CONFIG_T *config
        );
static void Send_HCI_Change_Connection_Packet_Type(
        hci_connection_handle_t handle,
        packet_type_t packet_type
        );
static void send_sync_to_hci(HCI_UPRIM_T *hci_uprim, hci_op_code_t op_code);

static void handle_dm_sync_connect_req(DM_SYNC_CONNECT_REQ_T *p_prim);
static void handle_dm_sync_connect_res(DM_SYNC_CONNECT_RSP_T *p_prim);
static void handle_dm_sync_renegotiate_req(DM_SYNC_RENEGOTIATE_REQ_T *p_prim);
static void handle_dm_sync_disconnect_req(DM_SYNC_DISCONNECT_REQ_T *p_prim);

static void add_new_handler(CsrSchedQid phandle, CsrUint16 pv_cbarg);
static void remove_handler(CsrSchedQid phandle, CsrUint16 pv_cbarg);
static DM_INCOMING_SYNC_HANDLER_T *find_handler(CsrSchedQid phandle, CsrUint16 pv_cbarg);
static hci_error_t new_sync(DM_INCOMING_SYNC_HANDLER_T *handler, BD_ADDR_T *p_bd_addr, sync_initiator_t initiator, CsrUint24 dev_class, DM_SYNC_T **pp_sync, DM_ACL_T **pp_acl);
static void free_sync(DM_SYNC_T *p_sync);
static DM_SYNC_T *dm_sync_find_by_acl_invalid_sync_handle(
        hci_connection_handle_t acl_handle, DM_ACL_T **pp_acl);
static DM_SYNC_T *dm_sync_find_by_bd_addr_invalid_handle(
        BD_ADDR_T *p_bd_addr, DM_ACL_T **pp_acl);
static void kick_sync_dispatch_process(DM_SYNC_T *p_sync, DM_ACL_T *p_acl, CsrUint8 response);

/*============================================================================*
    Public Function Implementations
 *============================================================================*/

/* The maximum number of SCO connections that are allowed.
   This is now controlled by the MAX_SCOS pskey
 */

CsrUint16  max_sync_connections ;

void dm_sync_init(void)
{
#ifdef BUILD_FOR_HOST
    max_sync_connections = CSR_BT_MAX_NUM_SCO_CONNS;
#else
    psget(PSKEY_MAX_SCOS, &max_sync_connections, sizeof(max_sync_connections));
#endif
}

/*! \brief Handler for Synchronous primitives coming into DM.

    \param p_prim Pointer to DM primitive
*/
void dm_sync_handler(
    DM_UPRIM_T *p_prim
    )
{
    switch (p_prim->type)
    {
        case DM_SYNC_REGISTER_REQ:
            /* Register the new handler into the handler list */
            add_new_handler(p_prim->dm_sync_register_req.phandle,
                p_prim->dm_sync_register_req.pv_cbarg);
            break;

        case DM_SYNC_UNREGISTER_REQ:
            /* Remove the handler from the list */
            remove_handler(p_prim->dm_sync_unregister_req.phandle,
                p_prim->dm_sync_unregister_req.pv_cbarg);
            break;

        case DM_SYNC_CONNECT_REQ:
            handle_dm_sync_connect_req(&p_prim->dm_sync_connect_req);
            CsrPmemFree(p_prim->dm_sync_connect_req.u.config);
            break;

         case DM_SYNC_CONNECT_RSP:
            handle_dm_sync_connect_res(&p_prim->dm_sync_connect_rsp);
            break;

        case DM_SYNC_RENEGOTIATE_REQ:
            handle_dm_sync_renegotiate_req(&p_prim->dm_sync_renegotiate_req);
            CsrPmemFree(p_prim->dm_sync_renegotiate_req.u.config);
            break;

        case DM_SYNC_DISCONNECT_REQ:
            handle_dm_sync_disconnect_req(&p_prim->dm_sync_disconnect_req);
            break;
    }
}

/*! \brief Destroy any delayed connect request and associated data.
    \param p_sync Pointer to Sync record structure.
*/
static void dm_sync_delayed_connection_destroy(DM_SYNC_T *p_sync)
{
    DM_SYNC_CONNECT_COLLISION_T *collision;

    if ((collision = p_sync->connection_collision) != NULL)
    {
        timer_cancel(&collision->delay);
        CsrPmemFree(collision->prim);
        CsrPmemFree(collision);
        p_sync->connection_collision = NULL;
    }
}

/*! \brief We've waited for the connection collision to resolve itself.
           Now we retry the connect request.

    \param unused1 unused
    \param p_sync Pointer to Sync record structure cast to (void*).
*/
static void dm_sync_hci_conn_request_retry(CsrUint16 unused1, void *p_sync)
{
    TIMER_EXPIRED(((DM_SYNC_T*)p_sync)->connection_collision->delay);

    /* Retry connect request before CsrSchedTidying up p_sync so that we can be
       sure that we won't retry yet again. */
    dm_sync_hci_conn_request(((DM_SYNC_T*)p_sync)->connection_collision->prim);
    dm_sync_delayed_connection_destroy(((DM_SYNC_T*)p_sync));
}

/*! \brief Handle a connection request event from HCI, indicating an incoming
           peer-initiated connection.

    \param p_prim Pointer to HCI_EV_CONN_REQUEST primitive.
*/
void dm_sync_hci_conn_request(
    HCI_EV_CONN_REQUEST_T *p_prim
    )
{
    DM_SYNC_T *p_sync;
    DM_ACL_T *p_acl;

    if (new_sync(NULL,
                 &p_prim->bd_addr,
                 SYNC_INIT_PEER,
                 p_prim->dev_class,
                 &p_sync,
                 &p_acl) == HCI_SUCCESS)
    {
        /* Start informing the registered handlers one by one of the new
         * connection, until one of them accepts it.
         */
        p_sync->is_esco = p_prim->link_type == HCI_LINK_TYPE_ESCO;
        kick_sync_dispatch_process(p_sync, p_acl, HCI_ERROR_REJ_BY_REMOTE_NO_RES);
    }
    else if (p_sync != NULL && p_sync->connection_collision == NULL)
    {
        /* Hang on for a short while before trying once again.
           We're effectively delaying the HCI event, so undo any changes
           to p_sync and wait to be called again. */
        --p_sync->req_count;
        p_sync->connection_collision = pnew(DM_SYNC_CONNECT_COLLISION_T);
        p_sync->connection_collision->prim = pcopy(p_prim,
                sizeof(HCI_EV_CONN_REQUEST_T));
        timer_start(&p_sync->connection_collision->delay,
                    SYNC_CONNECT_COLLISION_DELAY,
                    dm_sync_hci_conn_request_retry,
                    0,
                    p_sync);
    }
    else
    {
        Send_HCI_Reject_Synchronous_Connection(HCI_ERROR_REJ_BY_REMOTE_NO_RES,
                &p_prim->bd_addr, DM_HCI_SENDER_SYNC_MANAGER);
    }
}

/*! \brief Updates the policy manager, which handles the powerstates

    \param p_sync pointer to synchronous connection data structure instance.
    \param p_acl pointer to the ACL structure instance

    \return None
*/
static void dm_sync_update_policy_manager(DM_SYNC_T *p_sync, DM_ACL_T *p_acl)
{
    if (p_sync->initiator == SYNC_INIT_LOCAL)
    {
        p_acl->flags &= ~DM_ACL_SCO_PENDING;

        /* Fix a potential issue where the policy manager is putting the
         * link back to sniff immediatly after SYNC FAIL/SUCCESS.
         * Cache the current time in last_acl_time to restrict the policy
         * manager to out the link in sniff mode immediatly.
         */
#ifdef INSTALL_DM_POWERSTATES_MODULE
        DM_ACL_POLICY_UPDATE_LAST_ACL_TIME(p_acl);
#endif

        dm_acl_clear_active_mode(p_acl, dm_acl_client_sync);
    }
}


/*! \brief Notification from HCI interface that a connection setup has completed

    \param p_prim Pointer to HCI_EV_SYNC_CONN_COMPLETE primitive.
*/
void dm_sync_hci_connect_complete(
    HCI_EV_SYNC_CONN_COMPLETE_T *p_prim
    )
{
    DM_SYNC_T *p_sync;
    DM_ACL_T *p_acl;

    /* Get the Sync instance */
    p_sync = dm_sync_find_by_bd_addr_invalid_handle(&p_prim->bd_addr, &p_acl);
    if (!p_sync)
    {
        /* We can ignore failed connections that we know nothing about. */
        if (p_prim->status != HCI_SUCCESS)
            return;

        /* Our host may have set an auto-accept event filter. */
        if (p_handler_list == NULL || new_sync(p_handler_list,
                                               &p_prim->bd_addr,
                                               SYNC_INIT_PEER,
                                               0,
                                               &p_sync,
                                               &p_acl) != HCI_SUCCESS)
        {
            /* We have no room to store the Sync record structure. */
            Send_HCI_Disconnect(p_prim->handle, HCI_ERROR_OETC_LOW_RESOURCE,
                    DM_HCI_SENDER_SYNC_MANAGER);
            return;
        }

        p_sync->assigned = TRUE;
    }

    /* Allow Policy Manager to return to normal operation */
    dm_sync_update_policy_manager(p_sync, p_acl);

    if (p_prim->status == HCI_SUCCESS)
    {
        /* Set the connection handle */
        p_sync->handle = p_prim->handle;

        p_sync->is_esco = FALSE;
        if (p_prim->link_type == HCI_LINK_TYPE_ESCO)
        {
            /* Set flag if link is eSCO */
            p_sync->is_esco = TRUE;

            /* It is possible that the responder may have set dont care parameters
               for tx_bdw and rx_bdw when sending the DM_SYNC_CONNECT_RSP prim. If
               that was the case then these fields need updating here. Calculate
               them from the supplied primitive.
               Calculation is:
                  (packet len * esco slots per sec) / tx interval
            */

            /* For an eSCO link the tx_interval should never be 0. If it is then
               something has gone seriously wrong in the hci layer.
            */
            if (p_prim->tx_interval == 0)
                BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_ESCO_TX_INTERVAL);

                p_sync->config.tx_bdw =
                    (p_prim->tx_packet_length *
                     (CsrUint32)DM_ESCO_SLOTS_PER_SECOND)/ p_prim->tx_interval;

                p_sync->config.rx_bdw =
                    (p_prim->rx_packet_length *
                     (CsrUint32)DM_ESCO_SLOTS_PER_SECOND)/ p_prim->tx_interval;
        }

        /* enable buffer */
#ifndef BUILD_FOR_HOST
        hostio_enable_tohost_data(p_prim->handle);
#endif

        if (p_sync->connection_collision != NULL)
        {
            /* Our connection has succeeded, so reject remote request. */
            dm_sync_delayed_connection_destroy(p_sync);
            Send_HCI_Reject_Synchronous_Connection(
                    HCI_ERROR_REJ_BY_REMOTE_NO_RES,
                    &p_prim->bd_addr,
                    DM_HCI_SENDER_SYNC_MANAGER);
        }

        send_dm_sync_connect_complete_from_sync(p_sync,
                                                p_prim->status,
                                                &p_prim->bd_addr,
                                                p_prim,
                                                NULL);

        if (p_sync->req_count > 1)
        {
            /* There are outstanding doomed incoming connect requests.
               Create a copy of the Sync structure to deal with
               these doomed requests. */
            DM_SYNC_T *doomed = pcopy(p_sync, sizeof(DM_SYNC_T));

            doomed->p_next = p_acl->dm_acl_client_sync;
            p_acl->dm_acl_client_sync = doomed;

            p_sync->req_count = 1;
            doomed->handle = HCI_HANDLE_INVALID;
            --doomed->req_count;
            doomed->initiator = SYNC_INIT_PEER;
            doomed->requested = FALSE;
            doomed->rejected = TRUE;
        }
    }
    else
    {
        /* Can't set rejected flag here because this connection request may
           already have been doomed. Only when a non-doomed request fails can
           we set the flag. See B-56315. */
        dm_sync_hci_connect_failed(p_acl->handle, p_prim->status, FALSE);
    }
}


/*! \brief Notification from HCI interface that eSCO connection parameters have
           changed.

    \param p_prim Pointer to HCI_EV_SYNC_CONN_CHANGED primitive.
*/
void dm_sync_hci_connection_changed(
    HCI_EV_SYNC_CONN_CHANGED_T *p_prim
    )
{
    DM_SYNC_T *p_sync;

    p_sync = dm_sync_find_by_handle(p_prim->handle, NULL);
    if (p_sync && p_sync->assigned && p_sync->is_esco)
    {
        send_dm_sync_renegotiate_indcfm((dm_prim_t)(p_sync->initiated_renegotiate
                                                    ? DM_SYNC_RENEGOTIATE_CFM
                                                    : DM_SYNC_RENEGOTIATE_IND),
                                        p_prim->status,
                                        p_sync,
                                        p_prim);
        p_sync->initiated_renegotiate = FALSE;
    }
}


/*! \brief Notification from HCI interface that SCO packet type has changed.
           attempt has failed.

    Note: For SCO this only happens locally so there is no need to have a case
    for sending a renegotiate IND.

    \param p_prim Pointer to HCI_EV_CONN_PACKET_TYPE_CHANGED primitive
*/
void dm_sync_hci_connection_packet_type_changed(
    HCI_EV_CONN_PACKET_TYPE_CHANGED_T *p_prim
    )
{
    DM_SYNC_T *p_sync;
    DM_ACL_T *p_acl;

    p_sync = dm_sync_find_by_handle(p_prim->handle, &p_acl);
    if (p_sync && p_sync->assigned && !p_sync->is_esco)
    {
        if (p_sync->initiated_renegotiate)
        {
            p_sync->initiated_renegotiate = FALSE;
            send_dm_sync_renegotiate_indcfm(
                    DM_SYNC_RENEGOTIATE_CFM,
                    p_prim->status,
                    p_sync,
                    NULL);
        }
        else
        {
            /* A packet type change event has been received for a
               command that was not initiated by the sync manager.
               Thus forward the event straight up.
            */
            DM_SendBDADDRCommandComplete(DM_HCI_CONN_PACKET_TYPE_CHANGED_IND,
                    p_prim->status, &TBDADDR_ADDR(p_acl->addrt));
        }
    }
}

/*! \brief HCI_ACCEPT/REJECT_SYNCHRONOUS_CONN_REQ has failed at command
           status. This should only happen if the connection request event
           has timed out before we were able to get our response through.

    \param bd_addr Pointer to Bluetooth address of remote device.
    \param status HCI error.
    \param set_rejected Set the rejected flag for this SYNC instance.
*/
void dm_sync_hci_connect_response_failed(BD_ADDR_T *bd_addr,
                                         hci_return_t status,
                                         CsrBool set_rejected)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_connected_by_bd_addr(bd_addr)) != NULL)
    {
        /* Command disallowed really means that the connection accept timeout
           has been exceeded. The controller doesn't tell us about that. */
        if (status == HCI_ERROR_COMMAND_DISALLOWED)
            status = HCI_ERROR_HOST_TIMEOUT;

        dm_sync_hci_connect_failed(p_acl->handle, status, set_rejected);
    }
}

/*! \brief Notification from the HCI interface that an outgoing connection setup
           attempt has failed.

    This can be due to a Command Status or a Connection Complete event with an
    unsuccessful status.

    \param acl_handle ACL handle
    \param status HCI status code
    \param set_rejected Set the rejected flag for this SYNC instance.
*/
void dm_sync_hci_connect_failed(
    hci_connection_handle_t acl_handle,
    hci_return_t status,
    CsrBool set_rejected
    )
{
    DM_SYNC_T *p_sync;
    DM_ACL_T *p_acl;

    /* Search for the Sync instance, note that its handle will be invalid */
    p_sync = dm_sync_find_by_acl_invalid_sync_handle(acl_handle, &p_acl);

    /* We have to handle a case we landed up here because of a sync re-negotiation
         failure (a Command has been sent on an existing SCO/eSCO link to change the
         params), in that case the handle to search would not be an INVALID_HANDLE
    */
    if(!p_sync)
    {
        p_sync = dm_sync_find_by_handle(acl_handle, &p_acl);
    }

    if (p_sync)
    {
        /* This could have resulted from a renegotiation attempt */
        if (p_sync->initiated_renegotiate)
        {
            send_dm_sync_renegotiate_indcfm(
                    DM_SYNC_RENEGOTIATE_CFM,
                    status,
                    p_sync,
                    NULL);
            p_sync->initiated_renegotiate = FALSE;
        }
        /* If not renegotiating, must be from a connection request */
        else
        {
            DM_SYNC_CONNECT_COMPLETE_T *conn_complete_prim = NULL;
            HCI_EV_CONN_REQUEST_T *conn_request_prim = NULL;

            /* Allow Policy Manager to return to normal operation */
            dm_sync_update_policy_manager(p_sync, p_acl);

            /* Issue a negative confirmation for locally-initiated
             * connections and for peer-initiated connections we've
             * already tried to accept. We construct the primitive here
             * (because p_sync might be about to disappear). We send it
             * later so that we can retry any delayed incoming connections
             * first and so give the application a clue what's going on.
             * We only want to send this once, so wait until any colliding
             * incoming requests have been dealt with.
             */
            if ((p_sync->initiator == SYNC_INIT_LOCAL || p_sync->assigned)
                    && p_sync->req_count == 1)
                send_dm_sync_connect_complete_from_sync(
                        p_sync,
                        status,
                        &TBDADDR_ADDR(p_acl->addrt),
                        NULL,
                        &conn_complete_prim);

            /* A non-doomed request has just failed. This means that all other
               requests are already doomed. So we set the rejected flag to
               allow more (less doomed) connect requests. See B-56315. */
            if (set_rejected)
                p_sync->rejected = TRUE;

            /* Deal with retries now but don't send the primitive until
               we have released the SCO instance. */
            if (p_sync->connection_collision != NULL)
            {
                conn_request_prim = p_sync->connection_collision->prim;
                p_sync->connection_collision->prim = NULL;
                dm_sync_delayed_connection_destroy(p_sync);
            }

            /* Release the SCO instance */
            free_sync(p_sync);

            /* Now the SCO instance is gone, we can deal with retries. */
            if (conn_request_prim != NULL)
                dm_sync_hci_conn_request(conn_request_prim);

            /* Now that we've dealt with retries, we can send the
               connect complete primitive to the application. */
            if (conn_complete_prim != NULL)
                DM_SendMessage(conn_complete_prim);
        }
    }
}


/*! \brief Notification from the HCI interface that disconnection has failed.

    \param handle HCI handle of the synchronous connection
    \param status HCI status code
    \param reason HCI reason code
*/
void dm_sync_hci_disconnect_failure(
    hci_connection_handle_t handle,
    hci_return_t status,
    hci_error_t reason
    )
{
    DM_ACL_T *p_acl;
    DM_SYNC_T *p_sync;

    if ((p_sync = dm_sync_find_by_handle(handle, &p_acl)) != NULL
            && p_sync->assigned)
    {
        send_dm_sync_disconnect_cfm(p_sync,
                                    status,
                                    &TBDADDR_ADDR(p_acl->addrt));
    }
}


/*! \brief Notification from the HCI interface that disconnection has completed
           successfully.

    \param handle HCI handle of the synchronous connection
    \param reason HCI reason code
*/
void dm_sync_hci_disconnect_success(
    hci_connection_handle_t handle,
    hci_error_t reason
    )
{
    DM_SYNC_T *p_sync;
    DM_ACL_T *p_acl;

    p_sync = dm_sync_find_by_handle(handle, &p_acl);
    if (p_sync)
    {
        if (p_sync->assigned)
        {
            if (p_sync->initiated_disconnect)
            {
                send_dm_sync_disconnect_cfm(p_sync,
                                            HCI_SUCCESS,
                                            &TBDADDR_ADDR(p_acl->addrt));
            }
            else
            {
                send_dm_sync_disconnect_ind(p_sync,
                                            reason,
                                            &TBDADDR_ADDR(p_acl->addrt));
            }
        }

        /* Release the Sync instance and inform the ACL manager */
        free_sync(p_sync);
        dm_acl_policy_manager_reset(p_acl);
    }
}

/*! \brief Searches for a Sync instance, attached to the specified ACL,
           that has the specified HCI connection handle.

    \param p_acl Pointer to ACL record structure.
    \param handle SCO/eSCO connection handle.
    \returns Pointer to Sync instance or NULL if not found.
*/
static DM_SYNC_T *dm_sync_find_by_acl_and_handle(
        DM_ACL_T *p_acl,
        hci_connection_handle_t handle)
{
    DM_SYNC_T *p_sync = NULL;

    if (p_acl != NULL)
        for (p_sync = *(GET_SYNC_DATA(p_acl));
                p_sync != NULL && p_sync->handle != handle;
                p_sync = p_sync->p_next)
            ;

    return p_sync;
}

/*! \brief Searches for a Sync instance with the specified BD address and an
           invalid HCI connection handle.

    \param p_bd_addr Pointer to Bluetooth address
    \returns Pointer to Sync instance or NULL if not found
*/
static DM_SYNC_T *dm_sync_find_by_bd_addr_invalid_handle(
    BD_ADDR_T *p_bd_addr,
    DM_ACL_T **pp_acl
    )
{
    DM_ACL_T *p_acl = dm_acl_find_by_bd_addr(p_bd_addr);

    if (pp_acl != NULL)
        *pp_acl = p_acl;

    return dm_sync_find_by_acl_and_handle(p_acl, HCI_HANDLE_INVALID);
}


/*! \brief Searches for a Sync instance associated with the specified acl
           handle and with an invalid HCI connection handle.

    \param acl_handle ACL handle
    \returns Pointer to Sync instance or NULL if not found
*/
static DM_SYNC_T *dm_sync_find_by_acl_invalid_sync_handle(
        hci_connection_handle_t acl_handle,
        DM_ACL_T **pp_acl
        )
{
    DM_ACL_T *p_acl = dm_acl_find_by_handle(acl_handle);

    if (pp_acl != NULL)
        *pp_acl = p_acl;

    return dm_sync_find_by_acl_and_handle(p_acl, HCI_HANDLE_INVALID);
}


/*! \brief Searches for a Sync instance with the specified HCI connection handle.

    \param handle HCI connection handle
    \returns Pointer to Sync instance or NULL if not found
*/
DM_SYNC_T *dm_sync_find_by_handle(
    hci_connection_handle_t handle,
    DM_ACL_T **pp_acl
    )
{
    DM_ACL_T *p_acl;
    DM_SYNC_T *p_sync = NULL;

    for (p_acl = DM_ACL_FIRST();
            p_acl != NULL &&
            (p_sync = dm_sync_find_by_acl_and_handle(p_acl, handle)) == NULL;
            p_acl = p_acl->p_next)
        ;

    if (pp_acl != NULL)
        *pp_acl = p_acl;

    return p_sync;
}


/*! \brief Returns the current number of Sync connection instances.

    \returns Number of connections
*/
CsrUint16 dm_sync_num_connections(void)
{
    DM_ACL_T *p_acl;
    DM_SYNC_T *p_sync;
    CsrUint16 num = 0;

    for (p_acl = DM_ACL_FIRST(); p_acl != NULL; p_acl = p_acl->p_next)
    {
        for (p_sync = *(GET_SYNC_DATA(p_acl)); p_sync != NULL; p_sync = p_sync->p_next)
        {
            ++num;
        }
    }

    return num;
}


/*! \brief Returns the number of Sync connections associated with the ACL.

    \param acl_handle ACL handle
    \returns Number of connections
*/
CsrUint16 dm_sync_num_connections_acl(
    hci_connection_handle_t acl_handle
    )
{
    DM_ACL_T *p_acl;
    DM_SYNC_T *p_sync;
    CsrUint16 num = 0;

    if ((p_acl = dm_acl_find_by_handle(acl_handle)) != NULL)
        for (p_sync = *(GET_SYNC_DATA(p_acl)); p_sync != NULL;
                p_sync = p_sync->p_next)
            ++num;

    return num;
}


/*! \brief Indication from ACL manager that ACL has opened.

    \param p_acl Pointer to ACL structure.
    \param status HCI status (HCI_SUCCESS if opened successfully).
*/
void dm_sync_acl_opened(
    DM_ACL_T *p_acl,
    CsrUint8 status
    )
{
    if (status == HCI_SUCCESS)
    {
        /* Since the ACL has only just opened, there should be at most one
           sync structure on the list. If it's there and has not yet been
           requested, kick the policy manager to get things going. */
        if (*GET_SYNC_DATA(p_acl) != NULL && !(*GET_SYNC_DATA(p_acl))->requested)
        {
            p_acl->flags |= DM_ACL_SCO_PENDING;
            dm_acl_set_active_mode(p_acl, dm_acl_client_sync);
        }
    }
    else
    {
        /* Failed to open ACL. So clean up. */
        (void)dm_sync_acl_closed(p_acl, status);
    }
}


/*! \brief Indication from ACL manager that ACL has closed.

    \param chcb Pointer to ACL structure.
    \param status Reason for disconnect (HCI error code).
*/
CsrUint16 dm_sync_acl_closed(
    DM_ACL_T *p_acl,
    CsrUint16 reason
    )
{
    DM_SYNC_T *p_sync;

    /* Link loss, so clean up */
    while ((p_sync = *GET_SYNC_DATA(p_acl)) != NULL)
    {
        if (p_sync->handle != HCI_HANDLE_INVALID)
        {
            /* Issue a disconnect indication for established connections. */
            send_dm_sync_disconnect_ind(p_sync, (CsrUint8)(reason & 0xFF),
                    &TBDADDR_ADDR(p_acl->addrt));
        }
        else if (p_sync->initiator == SYNC_INIT_LOCAL || p_sync->assigned)
        {
            /* Issue a negative confirmation for locally-initiated connections
             * and for peer-initiated connections we've already tried to accept
             */
            send_dm_sync_connect_complete_from_sync(
                    p_sync,
                    (CsrUint8)(reason & 0xFF),
                    &TBDADDR_ADDR(p_acl->addrt),
                    NULL,
                    NULL);
        }

        free_sync(p_sync);
    }

    return 0;
}


/*! \brief The ACL is ready so we can safely initiate a synchronous connection
           request through HCI.

    \param p_acl Pointer to ACL structure
*/
void dm_sync_acl_ready(
    DM_ACL_T *p_acl
    )
{
    DM_SYNC_T *p_sync;

    /* Search for any outgoing Sync connections that have not been requested yet */
    for (p_sync = *(GET_SYNC_DATA(p_acl)); p_sync != NULL; p_sync = p_sync->p_next)
    {
        if ((p_sync->initiator == SYNC_INIT_LOCAL) && (!p_sync->requested))
        {
            /* Send the HCI connect request */
            Send_HCI_Setup_Synchronous_Connection(p_acl->handle,
                                                  &p_sync->config);
            p_sync->requested = TRUE;
        }
    }
}


/*============================================================================*
    Private Function Implementations
 *============================================================================*/
/*! \brief Handle a request to create a locally-initiated Synchronous connection.

    \param p_prim Pointer to DM_SYNC_CONNECT_REQ primitive
*/
static void handle_dm_sync_connect_req(
    DM_SYNC_CONNECT_REQ_T *p_prim
    )
{
    DM_ACL_T *p_acl;
    DM_SYNC_T *p_sync;
    DM_INCOMING_SYNC_HANDLER_T *handler;
    BD_ADDR_T *p_bd_addr = &p_prim->bd_addr;
    hci_error_t error = HCI_ERROR_UNSPECIFIED;

    if (p_prim->length == 0 &&
            (handler = find_handler(p_prim->phandle, p_prim->pv_cbarg)) != NULL
            && (error = new_sync(handler,
                                 p_bd_addr,
                                 SYNC_INIT_LOCAL,
                                 0,
                                 &p_sync,
                                 &p_acl)) == HCI_SUCCESS)
    {
        /* Store the details of the owning task */
        p_sync->assigned = TRUE;
        p_sync->requested = FALSE;

        CsrMemCpy(&p_sync->config,
               p_prim->u.config,
               sizeof(DM_SYNC_CONFIG_T));

        if (dm_acl_is_connected(p_acl))
        {
            p_acl->flags |= DM_ACL_SCO_PENDING;
            dm_acl_set_active_mode(p_acl, dm_acl_client_sync);
        }

        return;
    }

    /* Report failure */
    send_dm_sync_connect_complete(SYNC_INIT_LOCAL, error,
            HCI_HANDLE_INVALID, p_bd_addr, p_prim->phandle,
            p_prim->pv_cbarg, NULL, NULL);
}


/*! \brief Handles a response from a handler indicating whether it has
           accepted or rejected the incoming connection.

    \param p_prim Pointer to DM_SYNC_CONNECT_RSP primitive
*/
static void handle_dm_sync_connect_res(
    DM_SYNC_CONNECT_RSP_T *p_prim
    )
{
    DM_SYNC_T *p_sync;
    DM_ACL_T *p_acl;

    /* Find the connection */
    p_sync = dm_sync_find_by_bd_addr_invalid_handle(&p_prim->bd_addr, &p_acl);
    if (p_sync)
    {
        if (p_prim->response == HCI_SUCCESS)
        {
            p_sync->peer_connection_accepted = TRUE;
            p_sync->assigned = TRUE;

            /* Update with new connection paramters. */
            CsrMemCpy(&p_sync->config, &p_prim->config, sizeof(DM_SYNC_CONFIG_T));

            /* Accept the connection as slave */
            Send_HCI_Accept_Synchronous_Connection(
                    &p_prim->bd_addr,
                    &p_sync->config);
        }
        else
        {
            /* Not accepted, continue trying to find an owner for it */
            kick_sync_dispatch_process(p_sync, p_acl, p_prim->response);
        }
    }
}


/*! \brief Handle a request to renegotiate connection parameters.

    \param p_prim Pointer to DM_SYNC_RENEGOTIATE_REQ primitive
*/
static void handle_dm_sync_renegotiate_req(
    DM_SYNC_RENEGOTIATE_REQ_T *p_prim
    )
{
    DM_SYNC_T *p_sync;

    p_sync = dm_sync_find_by_handle(p_prim->handle, NULL);
    if (p_sync)
    {
        if (p_prim->length != 0)
        {
            send_dm_sync_renegotiate_indcfm(DM_SYNC_RENEGOTIATE_CFM,
                                            HCI_ERROR_UNSPECIFIED,
                                            p_sync,
                                            NULL);
            return;
        }

        p_sync->config.packet_type = p_prim->u.config->packet_type;
        p_sync->initiated_renegotiate = TRUE;

        if (p_sync->is_esco)
        {
            p_sync->config.max_latency = p_prim->u.config->max_latency;
            p_sync->config.retx_effort = p_prim->u.config->retx_effort;
            Send_HCI_Setup_Synchronous_Connection(p_prim->handle,
                                                  &p_sync->config);
        }
        else
        {
            /* Need to convert the packet type from esco format to
               sco/acl format.
            */
            p_sync->config.packet_type &= (HCI_ESCO_PKT_HV1 | HCI_ESCO_PKT_HV2 | HCI_ESCO_PKT_HV3);
            p_sync->config.packet_type <<= ESCO_TO_HCI_PKT_BIT_SHIFT;

            Send_HCI_Change_Connection_Packet_Type(p_prim->handle,
                p_sync->config.packet_type);
        }
    }
}


/*! \brief Handle a request to disconnect a connection.

    \param p_prim Pointer to DM_SYNC_DISCONNECT_REQ primitive
*/
static void handle_dm_sync_disconnect_req(
    DM_SYNC_DISCONNECT_REQ_T *p_prim
    )
{
    DM_SYNC_T *p_sync;

    if ((p_sync = dm_sync_find_by_handle(p_prim->handle, NULL)) != NULL)
    {
        p_sync->initiated_disconnect = TRUE;
        Send_HCI_Disconnect(p_prim->handle, p_prim->reason,
                                                DM_HCI_SENDER_SYNC_MANAGER);
    }
}


/*! \brief Issue a DM_SYNC_CONNECT_CFM or DM_SYNC_CONNECT_COMPLETE_IND primitive.

    DM_SYNC_CONNECT_CFM will be issued for a locally initiated connection,
    while DM_SYNC_CONNECT_COMPLETE_IND is used for an incoming connection.

    \param initiator Locally- or peer-initaited (SYNC_INIT_LOCAL/SYNC_INIT_PEER)
    \param status HCI status code
    \param handle HCI handle of the synchronous connection
    \param lm_sco_handle
    \param link_type
    \param tx_interval
    \param wesco
    \param rx_packet_length Receive packet length
    \param tx_packet_length Transmit packet length
    \param air_mode
    \param bd_addr Pointer to Bluetooth address
    \param phandle The phandle value of the destination task
    \param pv_cbarg The pv_cbarg value of the destination task
*/
static void send_dm_sync_connect_complete(
    sync_initiator_t initiator,
    hci_return_t status,
    hci_connection_handle_t handle,
    BD_ADDR_T *bd_addr,
    CsrSchedQid phandle,
    CsrUint16 pv_cbarg,
    HCI_EV_SYNC_CONN_COMPLETE_T *hci_prim,
    DM_SYNC_CONNECT_COMPLETE_T **dm_prim
    )
{
    DM_SYNC_CONNECT_COMPLETE_T *prim;

    prim = zpnew(DM_SYNC_CONNECT_COMPLETE_T);

    prim->type = initiator == SYNC_INIT_LOCAL ?
        DM_SYNC_CONNECT_CFM : DM_SYNC_CONNECT_COMPLETE_IND;
    prim->phandle = phandle;
    prim->pv_cbarg = pv_cbarg;
    prim->status = status;
    prim->handle = handle;
    bd_addr_copy(&prim->bd_addr, bd_addr);

    if (hci_prim != NULL)
    {
        prim->link_type = hci_prim->link_type;
        prim->tx_interval = hci_prim->tx_interval;
        prim->wesco = hci_prim->wesco;
        prim->rx_packet_length = hci_prim->rx_packet_length;
        prim->tx_packet_length = hci_prim->tx_packet_length;
        prim->air_mode = hci_prim->air_mode;
    }

    if (dm_prim == NULL)
        DM_SendMessage(prim);
    else
        *dm_prim = prim;
}

static void send_dm_sync_connect_complete_from_sync(
        DM_SYNC_T *p_sync,
        hci_return_t status,
        BD_ADDR_T *bd_addr,
        HCI_EV_SYNC_CONN_COMPLETE_T *hci_prim,
        DM_SYNC_CONNECT_COMPLETE_T **dm_prim)
{
    sync_initiator_t initiator = p_sync->initiator;

    /* Only take handle from SYNC record structure for successful connections */
    hci_connection_handle_t handle = HCI_HANDLE_INVALID;
    if (status == HCI_SUCCESS)
        handle = p_sync->handle;

    /* Deal with ambiguities arising from simultaneous connection requests.
       Note that there will only ever be one outstanding SYNC_INIT_LOCAL
       connect request - the rest will be SYNC_INIT_PEER. */
    if (status != HCI_SUCCESS && p_sync->req_count > 1)
    {
        /* Deal with outstanding failed peer-requests first. */
        if (!p_sync->peer_connection_accepted)
            initiator = SYNC_INIT_PEER;

        /* If our only SYNC_INIT_LOCAL connection has just failed then the
          rest must be SYNC_INIT_PEER. */
        if (initiator == SYNC_INIT_LOCAL)
            p_sync->initiator = SYNC_INIT_PEER;
    }

    send_dm_sync_connect_complete(p_sync->initiator,
                                  status,
                                  handle,
                                  bd_addr,
                                  p_sync->handler->phandle,
                                  p_sync->handler->pv_cbarg,
                                  hci_prim,
                                  dm_prim);
}

/*! \brief Issue a DM_SYNC_CONNECT_IND primitive.

    \param Pointer to SYNC record structure.
*/
static void send_dm_sync_connect_ind(DM_SYNC_T *p_sync, DM_ACL_T *p_acl)
{
    DM_SYNC_CONNECT_IND_T *dm_prim = pnew(DM_SYNC_CONNECT_IND_T);

    dm_prim->type = DM_SYNC_CONNECT_IND;
    dm_prim->phandle = p_sync->handler->phandle;
    dm_prim->pv_cbarg = p_sync->handler->pv_cbarg;
    dm_prim->dev_class = p_acl->dev_class;
    dm_prim->link_type = p_sync->is_esco ?
        HCI_LINK_TYPE_ESCO : HCI_LINK_TYPE_SCO;
    bd_addr_copy(&dm_prim->bd_addr, &TBDADDR_ADDR(p_acl->addrt));

    DM_SendMessage(dm_prim);
}


/*! \brief Issue a DM_SYNC_DISCONNECT_IND primitive.

    \param status HCI status code
    \param handle HCI handle of the synchronous connection
    \param reason HCI reason code
    \param bd_addr Pointer to Bluetooth address
    \param phandle The phandle value of the destination task
    \param pv_cbarg The pv_cbarg value of the destination task
*/
static void send_dm_sync_disconnect_ind(
        DM_SYNC_T *p_sync,
        hci_reason_t reason,
        BD_ADDR_T *p_bd_addr)
{
    /* Sending DM_SYNC_DISCONNECT_IND with HCI_HANDLE_INVALID causes a
       panic in the stream subsystem, so we double check here. */
    if (p_sync->handle != HCI_HANDLE_INVALID)
    {
        DM_SYNC_DISCONNECT_IND_T *dm_prim = pnew(DM_SYNC_DISCONNECT_IND_T);

        dm_prim->type = DM_SYNC_DISCONNECT_IND;
        dm_prim->phandle = p_sync->handler->phandle;
        dm_prim->pv_cbarg = p_sync->handler->pv_cbarg;
        dm_prim->reason = reason;
        dm_prim->handle = p_sync->handle;
        bd_addr_copy(&dm_prim->bd_addr, p_bd_addr);

        DM_SendMessage(dm_prim);
    }
}


/*! \brief Issue a DM_SYNC_DISCONNECT_CFM primitive.

    \param status HCI status code
    \param handle HCI handle of the synchronous connection
    \param bd_addr Pointer to Bluetooth address
    \param phandle The phandle value of the destination task
    \param pv_cbarg The pv_cbarg value of the destination task
*/
static void send_dm_sync_disconnect_cfm(
        DM_SYNC_T *p_sync,
        hci_return_t status,
        BD_ADDR_T *bd_addr)
{
    DM_SYNC_DISCONNECT_CFM_T *dm_prim = pnew(DM_SYNC_DISCONNECT_CFM_T);

    dm_prim->type = DM_SYNC_DISCONNECT_CFM;
    dm_prim->phandle = p_sync->handler->phandle;
    dm_prim->pv_cbarg = p_sync->handler->pv_cbarg;
    dm_prim->status = status;
    dm_prim->handle = p_sync->handle;
    bd_addr_copy(&dm_prim->bd_addr, bd_addr);

    DM_SendMessage(dm_prim);
}


/*! \brief Issue a DM_SYNC_RENEGOTIATE_IND/CFM primitive.

    \param type DM_SYNC_RENEGOTIATE_IND/CFM
    \param status HCI status code
    \param p_sync Pointer to sync record structure.
    \param p_hci_prim Pointer to HCI_EV_SYNC_CONN_CHANGED_T primitive, or NULL.
*/
static void send_dm_sync_renegotiate_indcfm(
    dm_prim_t type,
    hci_return_t status,
    DM_SYNC_T *p_sync,
    HCI_EV_SYNC_CONN_CHANGED_T *p_hci_prim
    )
{
    DM_SYNC_RENEGOTIATE_CFM_T *dm_prim;

    dm_prim = zpnew(DM_SYNC_RENEGOTIATE_CFM_T);

    dm_prim->type = type;
    dm_prim->phandle = p_sync->handler->phandle;
    dm_prim->pv_cbarg = p_sync->handler->pv_cbarg;
    dm_prim->status = status;
    dm_prim->handle = p_sync->handle;

    if (p_hci_prim != NULL)
    {
        dm_prim->tx_interval = p_hci_prim->tx_interval;
        dm_prim->wesco = p_hci_prim->wesco;
        dm_prim->rx_packet_length = p_hci_prim->rx_packet_length;
        dm_prim->tx_packet_length = p_hci_prim->tx_packet_length;
    }

    DM_SendMessage(dm_prim);
}


/*! \brief Issue a DM_SYNC_REGISTER_CFM primitive.

    \param phandle The phandle value of the destination task
    \param pv_cbarg The pv_cbarg value of the destination task
*/
static void send_dm_sync_register_cfm(
    CsrSchedQid phandle,
    CsrUint16 pv_cbarg
    )
{
    DM_SYNC_REGISTER_CFM_T *dm_prim;

    dm_prim = pnew(DM_SYNC_REGISTER_CFM_T);

    dm_prim->type = DM_SYNC_REGISTER_CFM;
    dm_prim->phandle = phandle;
    dm_prim->pv_cbarg = pv_cbarg;

    DM_SendMessage(dm_prim);
}


/*! \brief Issue a DM_SYNC_UNREGISTER_CFM primitive.

    \param phandle The phandle value of the destination task
    \param pv_cbarg The pv_cbarg value of the destination task
*/
static void send_dm_sync_unregister_cfm(
    CsrSchedQid phandle,
    CsrUint16 pv_cbarg,
    CsrUint8 status
    )
{
    DM_SYNC_UNREGISTER_CFM_T *dm_prim;

    dm_prim = pnew(DM_SYNC_UNREGISTER_CFM_T);

    dm_prim->type = DM_SYNC_UNREGISTER_CFM;
    dm_prim->phandle = phandle;
    dm_prim->pv_cbarg = pv_cbarg;
    dm_prim->status = status;

    DM_SendMessage(dm_prim);

}


/*! \brief Add a new incoming Sync handler to the linked list.

    Handlers are added to the tail of the list, just in case order of
    registration is significant to the application.

    \param phandle The phandle value of the new handler
    \param pv_cbarg The pv_cbarg value of the new handler
*/
static void add_new_handler(
    CsrSchedQid phandle,
    CsrUint16 pv_cbarg
    )
{
    DM_INCOMING_SYNC_HANDLER_T *p_new_handler;
    DM_INCOMING_SYNC_HANDLER_T **pp_tail;

    /* Find the tail of the handler list */
    pp_tail = &p_handler_list;
    while (*pp_tail)
    {
        pp_tail = &((*pp_tail)->p_next);
    }

    /* Create a new handler */
    p_new_handler = pnew(DM_INCOMING_SYNC_HANDLER_T);
    p_new_handler->phandle = phandle;
    p_new_handler->pv_cbarg = pv_cbarg;
    p_new_handler->p_next = NULL;

    /* Add the handler to the list tail */
    *pp_tail = p_new_handler;

    send_dm_sync_register_cfm(phandle, pv_cbarg);
}


/*! \brief Remove a registered handler from the list.

    \param phandle The phandle value to search for
    \param pv_cbarg The pv_cbarg value to search for
*/
static void remove_handler(CsrSchedQid phandle, CsrUint16 pv_cbarg)
{
    DM_INCOMING_SYNC_HANDLER_T *p, **pp;
    CsrUint8 status = DM_SYNC_UNREGISTER_NOT_FOUND;

    for (pp = &p_handler_list; (p = *pp) != NULL; pp = &p->p_next)
    {
        if (p->phandle == phandle && p->pv_cbarg == pv_cbarg)
        {
            DM_ACL_T *a;

            status = DM_SYNC_UNREGISTER_SUCCESS;

            /* Make sure not currently in use */
            for (a = DM_ACL_FIRST();
                    status == DM_SYNC_UNREGISTER_SUCCESS && a != NULL;
                    a = a->p_next)
            {
                DM_SYNC_T *s;

                for (s = *GET_SYNC_DATA(a); s != NULL; s = s->p_next)
                {
                    if (s->handler == p)
                    {
                        /* Still in use. */
                        status = DM_SYNC_UNREGISTER_BUSY;
                        break;
                    }
                }
            }

            if (status == DM_SYNC_UNREGISTER_SUCCESS)
            {
                *pp = p->p_next;
                CsrPmemFree(p);
            }

            break;
        }
    }

    send_dm_sync_unregister_cfm(phandle, pv_cbarg, status);
}


/*! \brief Search for a registered handler matching 'phandle' and 'pv_cbarg'.

    \param phandle The phandle value to search for
    \param pv_cbarg The pv_cbarg value to search for

    \returns Pointer to handler
*/
static DM_INCOMING_SYNC_HANDLER_T *find_handler(
    CsrSchedQid phandle,
    CsrUint16 pv_cbarg
    )
{
    DM_INCOMING_SYNC_HANDLER_T *p_handler;

    p_handler = p_handler_list;
    while (p_handler)
    {
        if ((p_handler->phandle == phandle) && (p_handler->pv_cbarg == pv_cbarg))
        {
            break;
        }

        p_handler = p_handler->p_next;
    }

    return p_handler;
}

/*! \brief Creates a new Sync connection instance using the supplied BD address.

    \param p_bd_addr Pointer to Bluetooth address
    \param initiator Locally- or peer-initaited (SYNC_INIT_LOCAL/SYNC_INIT_PEER)

    \returns Pointer to Sync instance, or NULL on failure
*/
static hci_error_t new_sync(
    DM_INCOMING_SYNC_HANDLER_T *handler,
    BD_ADDR_T *p_bd_addr,
    sync_initiator_t initiator,
    CsrUint24 dev_class,
    DM_SYNC_T **pp_sync,
    DM_ACL_T **pp_acl
    )
{
    TYPED_BD_ADDR_T addrt;
    DM_ACL_T *p_acl;
    DM_SYNC_T **pp_sync_list;
    DM_SYNC_T *p_new_sync = NULL;
    hci_error_t error = (hci_error_t)HCI_SUCCESS;

    tbdaddr_copy_from_public_bd_addr(&addrt, p_bd_addr);

    if (pp_acl == NULL)
        pp_acl = &p_acl;

    if (dm_acl_open(&addrt,
                    NULL,
                    dm_acl_client_sync,
                    DM_ACL_INIT_LOCAL | DM_ACL_CONNECTION_BR_EDR,
                    pp_acl) != DM_ACL_FAILED)
    {
        p_acl = *pp_acl;

        if (dev_class != 0)
            p_acl->dev_class = dev_class;

        /* Check there is not already a pending connection to this device */
        if ((*pp_sync = p_new_sync = dm_sync_find_by_bd_addr_invalid_handle(
                        p_bd_addr, NULL)) != NULL)
        {
            /* There is already a pending connection, thus increment a counter
               to show that another request to the same bd_addr has occurred.
               If we're about to fail a locally-initiated request then we don't
               increment because this is the last we'll hear about it.
            */
            if (p_new_sync->rejected || initiator == SYNC_INIT_PEER)
                ++p_new_sync->req_count;

            /* The pending connection has been marked as rejected. This should only
               happen in the case where there is a crossover between local and peer
               requested connections. Allow the current requester to takeover the
               pending connection.
               NOTE: As we have still incremented the req_count to show there was
               more than one request to this bd_addr, when the HCI_EV_SYNC_CONN_COMPLETE
               arrives for the reject response the free_sync() call will not remove this
               connection as the count once decremented will still be greater than 0.
            */
            if (p_new_sync->rejected)
            {
                p_new_sync->rejected = FALSE;
                p_new_sync->initiator = initiator;

                return (hci_error_t)HCI_SUCCESS;
            }

            error = HCI_ERROR_LMP_TRANSACTION_COLLISION;
        }

        if (error == HCI_SUCCESS)
        {
            if (dm_sync_num_connections() == max_sync_connections)
                error = HCI_ERROR_MAX_NR_OF_SCO;
            else if ((*pp_sync = p_new_sync = xzpnew(DM_SYNC_T)) == NULL)
                error = HCI_ERROR_MEMORY_FULL;
            else
            {
                /* Initialise the non-zero connection details */
                p_new_sync->handle = HCI_HANDLE_INVALID;
                p_new_sync->initiator = initiator;
                p_new_sync->handler = handler;
                p_new_sync->req_count = 1;

                /* Add to list */
                pp_sync_list = GET_SYNC_DATA(p_acl);
                p_new_sync->p_next = *pp_sync_list;
                *pp_sync_list = p_new_sync;

                return (hci_error_t)HCI_SUCCESS;
            }
        }

        dm_acl_close(p_acl,
                     dm_acl_client_sync,
                     HCI_ERROR_OETC_USER,
                     ACL_IDLE_TIMEOUT_AT_LEAST_NONE);
    }

    return error;
}


/*! \brief Release the specified Sync connection data.

    \param p_sync Pointer to Sync data structure
*/
static void free_sync(
    DM_SYNC_T *p_sync
    )
{
    DM_ACL_T *p_acl;
    DM_SYNC_T *p_sync_data, **pp_sync_data;

    for (p_acl = DM_ACL_FIRST(); p_acl != NULL; p_acl = p_acl->p_next)
    {
        for (pp_sync_data = GET_SYNC_DATA(p_acl);
             (p_sync_data = *pp_sync_data) != NULL;
             pp_sync_data = &(p_sync_data->p_next))
        {
            if (p_sync_data == p_sync)
            {
                if (--p_sync->req_count == 0)
                {
                    /* Remove entry from list */
                    *pp_sync_data = p_sync_data->p_next;

                    /* Release any resources used by the Sync connection */
                    dm_sync_delayed_connection_destroy(p_sync_data);

                    /* Release it */
                    CsrPmemFree(p_sync_data);
                }

                /* Inform ACL manager so it can decrement number of locks on this ACL */
                dm_acl_close(p_acl, dm_acl_client_sync, HCI_ERROR_OETC_USER, ACL_IDLE_TIMEOUT_AT_LEAST_NONE);

                /* Exit as we're done and don't want to dereference p_sync_data again. */
                return;
            }
        }
    }
}


/*! \brief Find an owner for incoming synchronous connection

    Called to start the SCO dispatch process, or to move it along if the
    last dispatch attempt was rejected. For an unassigned incoming SCO
    connection, cycles through the registered incoming SCO handlers one at
    a time, issuing a DM_SYNC_CONNECT_IND primitive to each one. Handlers
    respond with a DM_SYNC_CONNECT_RSP primitive indicating acceptance or
    rejection of the connection - on rejection, the connection is offered
    to the next registered handler.

    If all handlers have rejected the connection, the process is repeated
    after a short delay. The process will continue until either a handler
    accepts the connection, or the connection is released by the peer
    device.

    \param p_sync Pointer to data structure for the incoming connection
*/
static void kick_sync_dispatch_process(
    DM_SYNC_T *p_sync,
    DM_ACL_T *p_acl,
    CsrUint8 response
    )
{
    /* Do nothing if connection has already been assigned. */
    if (p_sync->assigned)
        return;

    /* Start from first registered handler, or move to next one. */
    p_sync->handler = p_sync->handler == NULL ?
        p_handler_list : p_sync->handler->p_next;

    if (p_sync->handler != NULL)
    {
        /* Try to dispatch the Sync connection to this handler */
        send_dm_sync_connect_ind(p_sync, p_acl);
    }
    else
    {
        /* Got to the end of the list and no task accepted the connection,
           so send a reject message */
        Send_HCI_Reject_Synchronous_Connection(
                response,
                &TBDADDR_ADDR(p_acl->addrt),
                DM_HCI_SENDER_SYNC_MANAGER);

        /* Mark the initiator field as REJECTED to allow this p_sync structure
           to be re-used in the case where there is a crossover of Sync
           connection requests.
        */
        p_sync->rejected = TRUE;
    }
}

/*! \param handle Connection handle of link.
    \param packet_type Packet types for link.
*/
static void Send_HCI_Change_Connection_Packet_Type(
        hci_connection_handle_t handle, packet_type_t packet_type)
{
    HCI_CHANGE_CONN_PKT_TYPE_T *p_prim;

    p_prim = pnew(HCI_CHANGE_CONN_PKT_TYPE_T);
    p_prim->common.op_code = HCI_CHANGE_CONN_PKT_TYPE;
    p_prim->handle = handle;
    p_prim->pkt_type = packet_type;

    send_sync_to_hci((HCI_UPRIM_T*)p_prim, HCI_CHANGE_CONN_PKT_TYPE);
}

/*! \param handle ACL handle for existing connection to remote device.
    \param fixed Pointer to fixed config.
    \param dynamic Pointer to renegotiable config.
*/
static void Send_HCI_Setup_Synchronous_Connection(
        hci_connection_handle_t handle,
        DM_SYNC_CONFIG_T *config
        )
{
    HCI_SETUP_SYNCHRONOUS_CONN_T *p_prim = NULL;

    p_prim = pnew(HCI_SETUP_SYNCHRONOUS_CONN_T);
    p_prim->handle = handle;
    p_prim->voice_settings = config->voice_settings;

    /*
        To interop with devices with a mis-matched baseband
        and stack (e.g. HCI 1.2 and Bluestack 1.1), we need
        to guarantee that when our client only specifies HV
        packets, the baseband really does use the SCO LMP
        settings and not upgrade it to eSCO.

        In theory, the baseband should do this, but the SIG
        chose not to address the issue.
    */
    if (HCI_ALL_MR_ESCO == (config->packet_type &
        (HCI_ALL_MR_ESCO|HCI_ESCO_PKT_EV3|HCI_ESCO_PKT_EV4|HCI_ESCO_PKT_EV5)))
    {
        /*
            Override fields to match SCO.
         */
        p_prim->tx_bdw = 8000;
        p_prim->rx_bdw = 8000;
        p_prim->max_latency = 5;
        p_prim->retx_effort = HCI_ESCO_NO_RETX;

    }
    else
    {
        p_prim->tx_bdw = config->tx_bdw;
        p_prim->rx_bdw = config->rx_bdw;
        p_prim->max_latency = config->max_latency;
        p_prim->retx_effort = (CsrUint8)config->retx_effort;
    }

    p_prim->pkt_type = config->packet_type;

    send_sync_to_hci((HCI_UPRIM_T*)p_prim, HCI_SETUP_SYNCHRONOUS_CONN);
}

/*! \param bd_addr Pointer to Bluetooth address of remote device.
    \param tx_bdw Transmit bandwidth
    \param rx_bdw Receive bandwidth
    \param max_latency
    \param voice_settings
    \param retx_effort
    \param packet_type Permitted packet types for this connection.
*/
static void Send_HCI_Accept_Synchronous_Connection(
        BD_ADDR_T *bd_addr,
        DM_SYNC_CONFIG_T *config
        )
{
    HCI_ACCEPT_SYNCHRONOUS_CONN_REQ_T *p_prim;

    p_prim = pnew(HCI_ACCEPT_SYNCHRONOUS_CONN_REQ_T);
    bd_addr_copy(&p_prim->bd_addr, bd_addr);
    p_prim->tx_bdw = config->tx_bdw;
    p_prim->rx_bdw = config->rx_bdw;
    p_prim->max_latency = config->max_latency;
    p_prim->voice_settings = config->voice_settings;
    p_prim->retx_effort = (CsrUint8)config->retx_effort;
    p_prim->pkt_type = config->packet_type;

    send_sync_to_hci((HCI_UPRIM_T*)p_prim, HCI_ACCEPT_SYNCHRONOUS_CONN_REQ);
}

static void send_sync_to_hci(HCI_UPRIM_T *hci_uprim, hci_op_code_t op_code)
{
    hci_uprim->hci_cmd.op_code = op_code;
    hci_uprim->hci_cmd.length = DM_HCI_SENDER_SYNC_MANAGER;

    send_to_hci((DM_UPRIM_T*)hci_uprim);
}

#ifdef ENABLE_SHUTDOWN
void dm_sync_deinit(void)
{
    DM_INCOMING_SYNC_HANDLER_T *p_handler_next;

    /* Destroy the handler list */
    while (p_handler_list)
    {
        p_handler_next = p_handler_list->p_next;
        CsrPmemFree(p_handler_list);
        p_handler_list = p_handler_next;
    }

    dm_sync_init();
}

void dm_acl_client_deinit_sync(DM_SYNC_T** pp_sync)
{
    DM_SYNC_T *p_sync = *pp_sync;

    while (p_sync != NULL)
    {
        DM_SYNC_T *tmp = p_sync;

        dm_sync_delayed_connection_destroy(p_sync);
        p_sync = p_sync->p_next;
        CsrPmemFree(tmp);
    }

    *pp_sync = NULL;
}
#endif /* ENABLE_SHUTDOWN */

#endif /* ifdef INSTALL_DM_SYNC_MODULE */

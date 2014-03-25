#ifdef INSTALL_DM_SYNC_MODULE

#ifndef _DM_SYNC_MANAGER_H
#define _DM_SYNC_MANAGER_H

#include "csr_synergy.h"
/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_sync_manager.h

\brief  DM synchroneous connection manager.
*/

#include "dm_acl_core.h"
#include "dm_sync_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYNC_INIT_LOCAL  ((sync_initiator_t) 0x00)
#define SYNC_INIT_PEER   ((sync_initiator_t) 0x01)

/* Number of 625us in a second NB This is a local duplicate of the LM define */
#define   DM_ESCO_SLOTS_PER_SECOND 1600

void dm_sync_handler(DM_UPRIM_T *p_prim);
void dm_sync_hci_conn_request(HCI_EV_CONN_REQUEST_T *p_prim);
void dm_sync_hci_connect_failed(hci_connection_handle_t acl_handle, hci_return_t status, CsrBool set_rejected);
void dm_sync_hci_connect_response_failed(BD_ADDR_T *bd_addr, hci_return_t status, CsrBool set_rejected);
void dm_sync_hci_connect_complete(HCI_EV_SYNC_CONN_COMPLETE_T *p_prim);
void dm_sync_hci_connection_changed(HCI_EV_SYNC_CONN_CHANGED_T *p_prim);
void dm_sync_hci_connection_packet_type_changed(HCI_EV_CONN_PACKET_TYPE_CHANGED_T *p_prim);
void dm_sync_hci_disconnect_failure(hci_connection_handle_t handle, hci_return_t status, hci_error_t reason);
void dm_sync_hci_disconnect_success(hci_connection_handle_t handle, hci_error_t reason);
DM_SYNC_T *dm_sync_find_by_handle(hci_connection_handle_t handle, DM_ACL_T **pp_acl);
CsrUint16 dm_sync_num_connections(void);
CsrUint16 dm_sync_num_connections_acl(hci_connection_handle_t acl_handle);
void dm_sync_acl_ready(DM_ACL_T *p_acl);

/* ACL manager callback functions */
void dm_sync_acl_opened(DM_ACL_T *p_acl, CsrUint8 status);
CsrUint16 dm_sync_acl_closed(DM_ACL_T *p_acl, CsrUint16 reason);

void dm_sync_init(void);

#ifdef ENABLE_SHUTDOWN
void dm_acl_client_deinit_sync(DM_SYNC_T** pp_sync);
void dm_sync_deinit(void);
#else
#define dm_sync_deinit() /* Nothing to do */
#endif

#ifdef __cplusplus
}
#endif

#endif /* header protection */

#else  /* sync module */

#define dm_sync_num_connections_acl(x) (0)
#define dm_sync_init() /* Nothing to do */
#define dm_sync_deinit() /* Nothing to do */

#endif /* sync module */

#ifndef _DM_HCI_INTERFACE_H_
#define _DM_HCI_INTERFACE_H_

#include "csr_synergy.h"
/*!

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

\file              dm_hci_interface.h

\brief             This file contains DM HCI Interface declarations.
*/

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"

/* Entities responsible for sending HCI commands. Replies should find them. */
#define DM_HCI_SENDER_NONE              0x00
#define DM_HCI_SENDER_APPLICATION       0x01
#define DM_HCI_SENDER_BLUESTACK         0xFE
#define DM_HCI_SENDER_ACL_MANAGER       0x02
#define DM_HCI_SENDER_SECURITY_MANAGER  0x04
#define DM_HCI_SENDER_SYNC_MANAGER      0x08
#define DM_HCI_SENDER_LAYER_MANAGER     0x10
#define DM_HCI_SENDER_AMPM              0x20
#define DM_HCI_SENDER_DM_AMP            0x40

CsrBool dm_hci_handle_and_tbdaddr(dm_prim_t type);
void handle_hci_cc_application(HCI_EV_COMMAND_COMPLETE_T *prim, HCI_UPRIM_T *ret_prim);
void handle_hci_cc_bluestack(HCI_EV_COMMAND_COMPLETE_T *prim, HCI_UPRIM_T *ret_prim);
void handle_hci_cs_fail(HCI_EV_COMMAND_STATUS_T *prim, HCI_UPRIM_T *ret_prim);
void DM_HCI_Initialise(void);
void dm_hci_event_callback(HCI_UPRIM_T *hci_prim);
void send_to_hci(DM_UPRIM_T *pv_hci_uprim);
void send_to_hci_with_handle(DM_UPRIM_T *pv_hci_uprim);

#ifndef DISABLE_DM_BREDR
void send_rnr_to_hci_with_conn_params(const DM_UPRIM_T *const p_uprim);
void Send_HCI_Reject_Synchronous_Connection(const hci_reason_t reason, const BD_ADDR_T *const bd_addr, const CsrUint8 sender);
void send_hci_write_auth_enable(const CsrUint8 auth_enable, const CsrUint8 sender);
void Send_HCI_Delete_Stored_Link_Key(const BD_ADDR_T *const p_bd_addr, const CsrUint8 sender);
#else
#define send_rnr_to_hci_with_conn_params  NULL
#endif

void Send_HCI_Reject_Connection(const BD_ADDR_T *const bd_addr, const CsrUint8 sender);
void Send_HCI_Disconnect(const hci_connection_handle_t handle, const hci_error_t reason, const CsrUint8 sender);
void dm_hci_handle_completed_packets(CsrUint16 handle, CsrUint16 completed_packets);

#ifdef INSTALL_AMP_SUPPORT
#define DM_HCI_L2CAP_DATA(controller, physical_link_id, logical_link_id, mblk) \
        dm_hci_l2cap_data((controller), (physical_link_id), (logical_link_id), (mblk))
#else
#define DM_HCI_L2CAP_DATA(controller, physical_link_id, logical_link_id, mblk) \
        dm_hci_l2cap_data((logical_link_id), (mblk))
#endif

#ifdef BUILD_FOR_HOST
void dm_hci_event_handler(HCI_UPRIM_T* hci_prim);
CsrBool dm_hci_l2cap_data(
#ifdef INSTALL_AMP_SUPPORT
        l2ca_controller_t controller,
        CsrUint8 physical_link_id,
#endif /* INSTALL_AMP_SUPPORT */
        CsrUint16 logical_link_id,
        CsrMblk *mblk);
#endif /* BUILD_FOR_HOST */

#ifdef L2CAP_HCI_DATA_CREDIT_SLOW_CHECKS
/*! \brief Ensure DM and L2CAP agree on how many credits are outstanding. */
void dm_hci_data_credit_audit(void);
#else
#define dm_hci_data_credit_audit()
#endif

#ifdef ENABLE_SHUTDOWN
void dm_hci_interface_deinit(void);
#else
#define dm_hci_interface_deinit() /* Nothing to do */
#endif

#ifdef BUILD_FOR_HOST
void CsrBtDmHciInit(void **gash);
void handle_cc_autogen(HCI_UPRIM_T *hci_prim, HCI_UPRIM_T *ret_prim);
#else
#define CsrBtDmHciInit(gash) /* Nothing to do */
#endif

#ifdef __cplusplus
}
#endif

#endif


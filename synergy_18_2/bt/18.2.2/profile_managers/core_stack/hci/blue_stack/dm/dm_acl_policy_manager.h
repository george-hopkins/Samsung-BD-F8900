#ifndef _DM_ACL_POLICY_MANAGER_H_
#define _DM_ACL_POLICY_MANAGER_H_

#include "csr_synergy.h"
/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              dm_acl_policy_manager.h

DESCRIPTION:       The ACL policy management entity of the
                   BlueStack Device Manager.



****************************************************************************/

#include "dm_acl_core.h"
#include "dm_acl_manager.h"

#define DM_ACL_LP_MASK \
    ((link_policy_settings_t)(ENABLE_SNIFF | ENABLE_PARK | ENABLE_HOLD))
#define DM_ACL_NO_LP_MASK \
    ((link_policy_settings_t)(~DM_ACL_LP_MASK))

#ifdef __cplusplus
extern "C" {
#endif
void dm_policy_acl_opened(DM_ACL_T *p_acl, CsrUint8 status);
CsrUint16 dm_policy_acl_closed(DM_ACL_T *p_acl, CsrUint16 reason);
void dm_acl_policy_reset(DM_ACL_T *p_acl);

#ifndef DISABLE_DM_BREDR
void dm_acl_set_default_link_policy(const DM_UPRIM_T *const p_uprim);
void dm_acl_switch_role(const DM_UPRIM_T *const p_uprim);
void dm_acl_write_lp_settings(const DM_UPRIM_T *const p_uprim);
void dm_acl_sniff_mode(const DM_UPRIM_T *const p_uprim);
void dm_acl_park_mode(const DM_UPRIM_T *const p_uprim);
void dm_acl_lp_write_roleswitch_policy_req(const DM_UPRIM_T *const p_uprim);
void dm_acl_lp_write_always_master_devices_req(const DM_UPRIM_T *const p_uprim);
void dm_acl_exit_sniff_mode(const DM_UPRIM_T *const p_uprim);
void dm_acl_exit_park_mode(const DM_UPRIM_T *const p_uprim);
void dm_acl_change_conn_pkt_type_req(const DM_UPRIM_T *const p_uprim);
#else
#define dm_acl_switch_role  NULL
#define dm_acl_write_lp_settings  NULL
#define dm_acl_set_default_link_policy  NULL
#define dm_acl_sniff_mode  NULL
#define dm_acl_park_mode  NULL
#define dm_acl_lp_write_roleswitch_policy_req  NULL
#define dm_acl_lp_write_always_master_devices_req  NULL
#define dm_acl_exit_sniff_mode  NULL
#define dm_acl_exit_park_mode  NULL
#define dm_acl_change_conn_pkt_type_req NULL
#endif

#ifndef INSTALL_BD_ADDR_BLACK_LIST_SUPPORT
#define dm_handle_bdaddr_black_list_req NULL
#else
void dm_handle_bdaddr_black_list_req(const DM_UPRIM_T *const p_uprim);
CsrBool dm_is_device_in_black_list(const BD_ADDR_T *p_bd_addr, CsrUint8 * no_of_devices);
void dm_remove_all_devices_from_black_list(void);
#endif



void dm_acl_hci_role_change(const BD_ADDR_T *const p_bd_addr, const hci_return_t status, hci_role_t role);
void dm_acl_hci_mode_change(DM_ACL_T *const p_acl, const hci_return_t status, const hci_bt_mode_t current_mode, const CsrUint16 interval);
CsrUint8 dm_acl_role_policy(CsrBool incoming, const BD_ADDR_T *const p_bd_addr);
void dm_acl_set_link_policy(DM_ACL_T *const p_acl, const link_policy_settings_t link_policy);
void dm_acl_set_active_mode(DM_ACL_T *const p_acl, const DM_ACL_CLIENT_T client);
void dm_acl_clear_active_mode(DM_ACL_T *const p_acl, const DM_ACL_CLIENT_T client);
void acl_activity(DM_ACL_T *p_acl);
void dm_acl_policy_manager_reset(DM_ACL_T *p_acl);

CsrBool check_mode_change(DM_ACL_T *const p_acl, const hci_bt_mode_t mode, const LP_POWERSTATE_T *const state);
void kick_policy_manager(DM_ACL_T *const p_acl);
void send_hci_sniff_mode(const BD_ADDR_T *const p_bd_addr, const CsrUint16 max_interval, const CsrUint16 min_interval, const CsrUint16 attempt, const CsrUint16 timeout);

#ifdef BUILD_FOR_HOST
void dm_acl_policy_manager_init(void);
#else
#define dm_acl_policy_manager_init() /* Nothing to do */
#endif

#ifdef ENABLE_SHUTDOWN
void dm_acl_policy_manager_deinit(void);
#else
#define dm_acl_policy_manager_deinit() /* Nothing to do */
#endif

#ifdef __cplusplus
}
#endif

#endif


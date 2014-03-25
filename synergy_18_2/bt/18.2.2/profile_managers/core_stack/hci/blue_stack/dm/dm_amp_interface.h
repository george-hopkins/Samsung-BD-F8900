#ifndef _DM_AMP_INTERFACE_H_
#define _DM_AMP_INTERFACE_H_

#include "csr_synergy.h"
/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_amp_interface.h

\brief  Bluestack interface to the AMP manager.
*/

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "dm_prim.h"

#include "dm_acl_amp_types.h"
#include "dm_acl_core.h"
#include "l2cap_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Generic flow control/data credit structure for BR/EDR buffers*/
typedef struct
{
    CsrUint16 max_acl_data_packet_length;
    CsrUint16 data_block_length;
    CsrUint16 total_num_data_blocks;
    CsrUint16 used_data_blocks;
#ifdef INSTALL_AMP_SUPPORT
    CsrBool fragmentable;
#endif
} DM_HCI_FLOW_CONTROL_T;

/* enum for the BR/EDR or, LE type device */
typedef enum
{
    DM_AMP_ACL_TYPE_BR_EDR = 0,
    DM_AMP_ACL_TYPE_LE
}DM_AMP_ACL_TYPE_T;

#ifdef SUPPORT_SEPARATE_LE_BUFFERS
#define DM_GET_FC_TYPE(type) (((type) == DM_AMP_ACL_TYPE_BR_EDR) \
                               ? (&LYMCB.fc) : (&LYMCB.le_fc))
#else
#define DM_GET_FC_TYPE(type) (&LYMCB.fc)
#endif

/* Data handling */
CsrUint16 dm_amp_getdatacredits(TXQUEUE_T *queue, L2CAP_CH_DATA_SIZES_T *sizes,
                               DM_AMP_ACL_TYPE_T type);
void dm_amp_reserve_credit(TXQUEUE_T *queue, DM_AMP_ACL_TYPE_T type);

#ifdef SUPPORT_SEPARATE_LE_BUFFERS
DM_HCI_FLOW_CONTROL_T * dm_amp_get_fc_type( DM_ACL_T *p_acl);
#else
#define dm_amp_get_fc_type(p_acl)  (&LYMCB.fc)
#endif


/* Everything from this point onwards is specific for AMP */
#ifdef INSTALL_AMP_SUPPORT

/* Some link keys we get are active, others still need to go to the LM. */
typedef enum
{
    DM_AMP_LINK_KEY_ACTIVE,
    DM_AMP_LINK_KEY_PENDING
} DM_AMP_LINK_KEY_STATE_T;

typedef struct DM_AMP_CHANNEL_T_tag
{
    struct DM_AMP_CHANNEL_T_tag *p_next;
    l2ca_controller_t amp_id;
    CsrUint16 l2cap_max_pdu_length;
    DM_HCI_FLOW_CONTROL_T fc;

#ifdef SUPPORT_SEPARATE_LE_BUFFERS
    /* LE flow control data structure */
    DM_HCI_FLOW_CONTROL_T le_fc;
#endif
} DM_AMP_CHANNEL_T;

#ifdef BUILD_FOR_HOST
void dm_amp_init(void);
#else
#define dm_amp_init() /* Nothing to do */
#endif

void dm_amp_device_ready(void);

#ifdef ENABLE_SHUTDOWN
void dm_acl_client_deinit_amp(DM_AMP_T *p_amp);
void dm_amp_deinit(void);
#else
#define dm_amp_deinit() /* Nothing to do */
#endif

DM_AMP_CHANNEL_T *dm_amp_find(const l2ca_controller_t amp_id);
DM_AMP_CHANNEL_T *dm_amp_create(const l2ca_controller_t amp_id,
                                const CsrBool fragmentable,
                                const CsrUint16 l2cap_max_pdu_length,
                                const CsrUint16 max_acl_data_packet_length,
                                const CsrUint16 data_block_length,
                                const CsrUint16 total_num_data_blocks);
void dm_amp_free(const l2ca_controller_t amp_id);

/* HCI_READ_BD_ADDR command complete. */
void dm_amp_read_bd_addr_cc(const CsrUint8 sender, const hci_return_t status,
                                const BD_ADDR_T *const p_bd_addr);

/* Handle AMPM's message-based interface. */
void dm_amp_handler(DM_UPRIM_T *const p_uprim);


/* ACL Manager callbacks */
void dm_amp_acl_opened(DM_ACL_T *p_acl, CsrUint8 status);
CsrUint16 dm_amp_acl_closed(DM_ACL_T *p_acl, CsrUint16 status);
void dm_amp_acl_mode_changed(DM_ACL_T *p_acl, CsrUint8 status, CsrUint16 length);

/* L2CAP's function interface */
void dm_amp_physical_connect_req(const DM_ACL_T *const p_acl,
                                 const l2ca_cid_t cid,
                                 const l2ca_controller_t local_amp_id,
                                 const l2ca_controller_t remote_amp_id);
void dm_amp_logical_connect_req(const DM_ACL_T *const p_acl,
                                const l2ca_cid_t cid,
                                const l2ca_controller_t local_amp_id,
                                const L2CA_FLOWSPEC_T *flow_spec_tx,
                                const L2CA_FLOWSPEC_T *flow_spec_rx);
void dm_amp_disconnect_req(const DM_ACL_T *const p_acl,
                           const l2ca_cid_t cid,
                           const l2ca_controller_t local_amp_id);
void dm_amp_verify_physical_link_req(const DM_ACL_T *const p_acl,
                                     const CsrUint16 identifier,
                                     const l2ca_controller_t local_amp_id);
CsrBool dm_amp_reject_remote_request(const DM_ACL_T *const p_acl);

/* Security manager's function-based interface. */
void dm_amp_link_key_notification_ind(DM_ACL_T *p_acl,
                                      const CsrUint8 link_key_type,
                                      const CsrUint8 *const link_key,
                                      const DM_AMP_LINK_KEY_STATE_T state);
void dm_amp_link_key_active_ind(DM_ACL_T *p_acl, CsrUint8 status);
void dm_amp_simple_pairing_enabled(void);
void dm_amp_remote_features(const TYPED_BD_ADDR_T *addrt, CsrUint16 acl_flags);
#else
#define dm_amp_simple_pairing_enabled() /* Nothing to do */
#define dm_amp_remote_features(bd_addr, acl_flags) /* Nothing to do */
#define dm_amp_init() /* Nothing to do */
#define dm_amp_deinit() /* Nothing to do */
#define dm_amp_device_ready() /* Nothing to do */
#endif /* INSTALL_AMP_SUPPORT */

#ifdef __cplusplus
}
#endif

#endif /* _DM_AMP_INTERFACE_H_ */

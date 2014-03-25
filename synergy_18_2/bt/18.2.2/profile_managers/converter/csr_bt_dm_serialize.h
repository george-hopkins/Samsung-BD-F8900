#ifndef CSR_BT_DM_SERIALIZE_H__
#define CSR_BT_DM_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#include "csr_synergy.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtDmPfree(void *ptr);

#ifndef EXCLUDE_DM_AMPM_DISCONNECT_REQ
CsrSize DM_AMPM_DISCONNECT_REQ_TSizeof(void *msg);
CsrUint8 *DM_AMPM_DISCONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_DISCONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_DISCONNECT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_DISCONNECT_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_REQ
CsrSize DM_HCI_ULP_TRANSMITTER_TEST_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_TRANSMITTER_TEST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_TRANSMITTER_TEST_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_TRANSMITTER_TEST_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_CFM
CsrSize DM_HCI_ULP_SET_EVENT_MASK_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_EVENT_MASK_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_EVENT_MASK_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_EVENT_MASK_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND
CsrSize DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_TSizeof(void *msg);
CsrUint8 *DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND */

#ifndef EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM
CsrSize DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_REQ
CsrSize DM_HCI_READ_LOCAL_VER_INFO_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_LOCAL_VER_INFO_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_LOCAL_VER_INFO_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_LOCAL_VER_INFO_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_REQ */

#ifndef EXCLUDE_DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM
CsrSize DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_HOST_NUM_COMPLETED_PACKETS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_VOICE_SETTING_CFM
CsrSize DM_HCI_READ_VOICE_SETTING_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_VOICE_SETTING_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_VOICE_SETTING_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_VOICE_SETTING_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_VOICE_SETTING_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_MODE_CFM
CsrSize DM_HCI_READ_INQUIRY_MODE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_INQUIRY_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_INQUIRY_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_INQUIRY_MODE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM
CsrSize DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ
CsrSize DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ
CsrSize DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM
CsrSize DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_CFM
CsrSize DM_HCI_READ_CLOCK_OFFSET_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_CLOCK_OFFSET_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_CLOCK_OFFSET_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_CLOCK_OFFSET_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_CFM */

#ifndef EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ
CsrSize DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM
CsrSize DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_CFM
CsrSize DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ
CsrSize DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_ENCRYPT_CFM
CsrSize DM_HCI_ULP_ENCRYPT_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_ENCRYPT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_ENCRYPT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_ENCRYPT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_ENCRYPT_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ
CsrSize DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_RSP
CsrSize DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_TSizeof(void *msg);
CsrUint8 *DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_RSP */

#ifndef EXCLUDE_DM_HCI_READ_CURRENT_IAC_LAP_REQ
CsrSize DM_HCI_READ_CURRENT_IAC_LAP_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_CURRENT_IAC_LAP_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_CURRENT_IAC_LAP_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_CURRENT_IAC_LAP_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_CURRENT_IAC_LAP_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ
CsrSize DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_CFM
CsrSize DM_HCI_ULP_RECEIVER_TEST_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_RECEIVER_TEST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_RECEIVER_TEST_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_RECEIVER_TEST_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_RAND_CFM
CsrSize DM_HCI_ULP_RAND_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_RAND_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_RAND_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_RAND_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_RAND_CFM */

#ifndef EXCLUDE_DM_SYNC_CONNECT_RSP
CsrSize DM_SYNC_CONNECT_RSP_TSizeof(void *msg);
CsrUint8 *DM_SYNC_CONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SYNC_CONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SYNC_CONNECT_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SYNC_CONNECT_RSP */

#ifndef EXCLUDE_DM_HCI_SET_EVENT_MASK_CFM
CsrSize DM_HCI_SET_EVENT_MASK_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_SET_EVENT_MASK_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_SET_EVENT_MASK_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_SET_EVENT_MASK_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_REQ
CsrSize DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ
CsrSize DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_REQ */

#ifndef EXCLUDE_DM_AM_REGISTER_WRONG_API_REQ
CsrSize DM_AM_REGISTER_WRONG_API_REQ_TSizeof(void *msg);
CsrUint8 *DM_AM_REGISTER_WRONG_API_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AM_REGISTER_WRONG_API_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AM_REGISTER_WRONG_API_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AM_REGISTER_WRONG_API_REQ */

#ifndef EXCLUDE_DM_SM_AUTHORISE_RSP
CsrSize DM_SM_AUTHORISE_RSP_TSizeof(void *msg);
CsrUint8 *DM_SM_AUTHORISE_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_AUTHORISE_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_AUTHORISE_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_AUTHORISE_RSP */

#ifndef EXCLUDE_DM_SM_AUTHORISE_IND
CsrSize DM_SM_AUTHORISE_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_AUTHORISE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_AUTHORISE_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_AUTHORISE_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_AUTHORISE_IND */

#ifndef EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM
CsrSize DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_CFM
CsrSize DM_HCI_READ_PAGESCAN_ACTIVITY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_PAGESCAN_ACTIVITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_PAGESCAN_ACTIVITY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_PAGESCAN_ACTIVITY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_SNIFF_SUB_RATE_CFM
CsrSize DM_HCI_SNIFF_SUB_RATE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_SNIFF_SUB_RATE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_SNIFF_SUB_RATE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_SNIFF_SUB_RATE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_SNIFF_SUB_RATE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_BD_ADDR_REQ
CsrSize DM_HCI_READ_BD_ADDR_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_BD_ADDR_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_BD_ADDR_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_BD_ADDR_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_BD_ADDR_REQ */

#ifndef EXCLUDE_DM_SM_REGISTER_REQ
CsrSize DM_SM_REGISTER_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_REGISTER_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_REGISTER_REQ */

#ifndef EXCLUDE_DM_SYNC_DISCONNECT_CFM
CsrSize DM_SYNC_DISCONNECT_CFM_TSizeof(void *msg);
CsrUint8 *DM_SYNC_DISCONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SYNC_DISCONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SYNC_DISCONNECT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SYNC_DISCONNECT_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM
CsrSize DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_NOTIFICATION_IND
CsrSize DM_SM_USER_PASSKEY_NOTIFICATION_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_USER_PASSKEY_NOTIFICATION_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_USER_PASSKEY_NOTIFICATION_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_USER_PASSKEY_NOTIFICATION_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_NOTIFICATION_IND */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ
CsrSize DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_SYNC_CONNECT_COMPLETE_IND
CsrSize DM_SYNC_CONNECT_COMPLETE_IND_TSizeof(void *msg);
CsrUint8 *DM_SYNC_CONNECT_COMPLETE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SYNC_CONNECT_COMPLETE_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SYNC_CONNECT_COMPLETE_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SYNC_CONNECT_COMPLETE_IND */

#ifndef EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM
CsrSize DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_CFM */

#ifndef EXCLUDE_DM_SM_BONDING_REQ
CsrSize DM_SM_BONDING_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_BONDING_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_BONDING_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_BONDING_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_BONDING_REQ */

#ifndef EXCLUDE_DM_SM_REMOVE_DEVICE_REQ
CsrSize DM_SM_REMOVE_DEVICE_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_REMOVE_DEVICE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_REMOVE_DEVICE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_REMOVE_DEVICE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_REMOVE_DEVICE_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_REQ
CsrSize DM_LP_WRITE_ROLESWITCH_POLICY_REQ_TSizeof(void *msg);
CsrUint8 *DM_LP_WRITE_ROLESWITCH_POLICY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_LP_WRITE_ROLESWITCH_POLICY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void DM_LP_WRITE_ROLESWITCH_POLICY_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_REQ */

#ifndef EXCLUDE_DM_SM_SIMPLE_PAIRING_COMPLETE_IND
CsrSize DM_SM_SIMPLE_PAIRING_COMPLETE_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_SIMPLE_PAIRING_COMPLETE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_SIMPLE_PAIRING_COMPLETE_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_SIMPLE_PAIRING_COMPLETE_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_SIMPLE_PAIRING_COMPLETE_IND */

#ifndef EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_REQ
CsrSize DM_HCI_READ_STORED_LINK_KEY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_STORED_LINK_KEY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_STORED_LINK_KEY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_STORED_LINK_KEY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_REQ
CsrSize DM_HCI_REFRESH_ENCRYPTION_KEY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_REFRESH_ENCRYPTION_KEY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_REFRESH_ENCRYPTION_KEY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_REFRESH_ENCRYPTION_KEY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_REQ */

#ifndef EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_EXIT_IND
CsrSize DM_BLUESTACK_STATE_TRANSITION_EXIT_IND_TSizeof(void *msg);
CsrUint8 *DM_BLUESTACK_STATE_TRANSITION_EXIT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_BLUESTACK_STATE_TRANSITION_EXIT_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_BLUESTACK_STATE_TRANSITION_EXIT_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_EXIT_IND */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ
CsrSize DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_QOS_SETUP_CFM
CsrSize DM_HCI_QOS_SETUP_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_QOS_SETUP_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_QOS_SETUP_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_QOS_SETUP_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_QOS_SETUP_CFM */

#ifndef EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_CFM
CsrSize DM_HANDLE_DEVICE_BLACK_LIST_CFM_TSizeof(void *msg);
CsrUint8 *DM_HANDLE_DEVICE_BLACK_LIST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HANDLE_DEVICE_BLACK_LIST_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HANDLE_DEVICE_BLACK_LIST_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_REQ
CsrSize DM_HCI_WRITE_INQUIRY_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_INQUIRY_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_INQUIRY_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_INQUIRY_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_AMPM_REGISTER_CFM
CsrSize DM_AMPM_REGISTER_CFM_TSizeof(void *msg);
CsrUint8 *DM_AMPM_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_REGISTER_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_REGISTER_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_REQ
CsrSize DM_HCI_WRITE_SCAN_ENABLE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_SCAN_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_SCAN_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_SCAN_ENABLE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_INQUIRY_REQ
CsrSize DM_HCI_INQUIRY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_INQUIRY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_INQUIRY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_INQUIRY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_INQUIRY_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ
CsrSize DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM
CsrSize DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_RSP
CsrSize DM_SM_USER_PASSKEY_REQUEST_RSP_TSizeof(void *msg);
CsrUint8 *DM_SM_USER_PASSKEY_REQUEST_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_USER_PASSKEY_REQUEST_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_USER_PASSKEY_REQUEST_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_RSP */

#ifndef EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM
CsrSize DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_TSizeof(void *msg);
CsrUint8 *DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_CFM
CsrSize DM_SET_BLE_CONNECTION_PARAMETERS_CFM_TSizeof(void *msg);
CsrUint8 *DM_SET_BLE_CONNECTION_PARAMETERS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SET_BLE_CONNECTION_PARAMETERS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SET_BLE_CONNECTION_PARAMETERS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_CLEAR_PARAM_CACHE_REQ
CsrSize DM_CLEAR_PARAM_CACHE_REQ_TSizeof(void *msg);
CsrUint8 *DM_CLEAR_PARAM_CACHE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_CLEAR_PARAM_CACHE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_CLEAR_PARAM_CACHE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_CLEAR_PARAM_CACHE_REQ */

#ifndef EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_RSP
CsrSize DM_SM_USER_CONFIRMATION_REQUEST_RSP_TSizeof(void *msg);
CsrUint8 *DM_SM_USER_CONFIRMATION_REQUEST_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_USER_CONFIRMATION_REQUEST_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_USER_CONFIRMATION_REQUEST_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_RSP */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_NAME_REQ
CsrSize DM_HCI_READ_LOCAL_NAME_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_LOCAL_NAME_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_LOCAL_NAME_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_LOCAL_NAME_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_NAME_REQ */

#ifndef EXCLUDE_DM_AM_REGISTER_REQ
CsrSize DM_AM_REGISTER_REQ_TSizeof(void *msg);
CsrUint8 *DM_AM_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AM_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AM_REGISTER_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AM_REGISTER_REQ */

#ifndef EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_CFM
CsrSize DM_HCI_CREATE_NEW_UNIT_KEY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_CREATE_NEW_UNIT_KEY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_CREATE_NEW_UNIT_KEY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_CREATE_NEW_UNIT_KEY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_CFM
CsrSize DM_HCI_CREATE_CONNECTION_CANCEL_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_CREATE_CONNECTION_CANCEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_CREATE_CONNECTION_CANCEL_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_CREATE_CONNECTION_CANCEL_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_READ_SCAN_ENABLE_REQ
CsrSize DM_HCI_READ_SCAN_ENABLE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_SCAN_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_SCAN_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_SCAN_ENABLE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM
CsrSize DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_SM_REGISTER_CFM
CsrSize DM_SM_REGISTER_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_REGISTER_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_REGISTER_CFM */

#ifndef EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM
CsrSize DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM */

#ifndef EXCLUDE_DM_AMPM_READ_BD_ADDR_REQ
CsrSize DM_AMPM_READ_BD_ADDR_REQ_TSizeof(void *msg);
CsrUint8 *DM_AMPM_READ_BD_ADDR_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_READ_BD_ADDR_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_READ_BD_ADDR_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_READ_BD_ADDR_REQ */

#ifndef EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_IND
CsrSize DM_SM_IO_CAPABILITY_REQUEST_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_IO_CAPABILITY_REQUEST_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_IO_CAPABILITY_REQUEST_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_IO_CAPABILITY_REQUEST_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_IND */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ
CsrSize DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_ENCRYPT_REQ
CsrSize DM_HCI_ULP_ENCRYPT_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_ENCRYPT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_ENCRYPT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_ENCRYPT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_ENCRYPT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_REQ
CsrSize DM_HCI_READ_CLOCK_OFFSET_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_CLOCK_OFFSET_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_CLOCK_OFFSET_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_CLOCK_OFFSET_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_OFFSET_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_CFM
CsrSize DM_HCI_READ_LOCAL_EXT_FEATURES_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_LOCAL_EXT_FEATURES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_LOCAL_EXT_FEATURES_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_LOCAL_EXT_FEATURES_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM
CsrSize DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ
CsrSize DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM
CsrSize DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_CFM */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_NEG_RSP
CsrSize DM_SM_USER_PASSKEY_REQUEST_NEG_RSP_TSizeof(void *msg);
CsrUint8 *DM_SM_USER_PASSKEY_REQUEST_NEG_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_USER_PASSKEY_REQUEST_NEG_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_USER_PASSKEY_REQUEST_NEG_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_SYNC_RENEGOTIATE_IND
CsrSize DM_SYNC_RENEGOTIATE_IND_TSizeof(void *msg);
CsrUint8 *DM_SYNC_RENEGOTIATE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SYNC_RENEGOTIATE_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SYNC_RENEGOTIATE_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SYNC_RENEGOTIATE_IND */

#ifndef EXCLUDE_DM_SM_SECURITY_REQ
CsrSize DM_SM_SECURITY_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_SECURITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_SECURITY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_SECURITY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_SECURITY_REQ */

#ifndef EXCLUDE_DM_SM_SECURITY_CFM
CsrSize DM_SM_SECURITY_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_SECURITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_SECURITY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_SECURITY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_SECURITY_CFM */

#ifndef EXCLUDE_DM_SET_DEFAULT_LINK_POLICY_REQ
CsrSize DM_SET_DEFAULT_LINK_POLICY_REQ_TSizeof(void *msg);
CsrUint8 *DM_SET_DEFAULT_LINK_POLICY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SET_DEFAULT_LINK_POLICY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SET_DEFAULT_LINK_POLICY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SET_DEFAULT_LINK_POLICY_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ
CsrSize DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_FLUSH_REQ
CsrSize DM_HCI_FLUSH_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_FLUSH_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_FLUSH_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_FLUSH_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_FLUSH_REQ */

#ifndef EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_REQ
CsrSize DM_HCI_CREATE_NEW_UNIT_KEY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_CREATE_NEW_UNIT_KEY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_CREATE_NEW_UNIT_KEY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_CREATE_NEW_UNIT_KEY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_CREATE_NEW_UNIT_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_ENHANCED_FLUSH_REQ
CsrSize DM_HCI_ENHANCED_FLUSH_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ENHANCED_FLUSH_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ENHANCED_FLUSH_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ENHANCED_FLUSH_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ENHANCED_FLUSH_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ
CsrSize DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_REQ */

#ifndef EXCLUDE_DM_SM_ACCESS_CFM
CsrSize DM_SM_ACCESS_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_ACCESS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_ACCESS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_ACCESS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_ACCESS_CFM */

#ifndef EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ
CsrSize DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM
CsrSize DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_REQ
CsrSize DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ
CsrSize DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_READ_COUNTRY_CODE_REQ
CsrSize DM_HCI_READ_COUNTRY_CODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_COUNTRY_CODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_COUNTRY_CODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_COUNTRY_CODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_COUNTRY_CODE_REQ */

#ifndef EXCLUDE_DM_SM_PIN_REQUEST_RSP
CsrSize DM_SM_PIN_REQUEST_RSP_TSizeof(void *msg);
CsrUint8 *DM_SM_PIN_REQUEST_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_PIN_REQUEST_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_PIN_REQUEST_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_PIN_REQUEST_RSP */

#ifndef EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_CFM
CsrSize DM_HCI_WRITE_NUM_BCAST_RETXS_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_NUM_BCAST_RETXS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_NUM_BCAST_RETXS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_NUM_BCAST_RETXS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_CFM */

#ifndef EXCLUDE_DM_HCI_SNIFF_SUB_RATING_IND
CsrSize DM_HCI_SNIFF_SUB_RATING_IND_TSizeof(void *msg);
CsrUint8 *DM_HCI_SNIFF_SUB_RATING_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_SNIFF_SUB_RATING_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_SNIFF_SUB_RATING_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_SNIFF_SUB_RATING_IND */

#ifndef EXCLUDE_DM_HCI_SWITCH_ROLE_CFM
CsrSize DM_HCI_SWITCH_ROLE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_SWITCH_ROLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_SWITCH_ROLE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_SWITCH_ROLE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_SWITCH_ROLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ
CsrSize DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_REQ
CsrSize DM_HCI_WRITE_PAGESCAN_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_PAGESCAN_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_PAGESCAN_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_PAGESCAN_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM
CsrSize DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_REQ
CsrSize DM_HCI_READ_PAGESCAN_ACTIVITY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_PAGESCAN_ACTIVITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_PAGESCAN_ACTIVITY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_PAGESCAN_ACTIVITY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ
CsrSize DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ */

#ifndef EXCLUDE_DM_HCI_LINK_SUPERV_TIMEOUT_IND
CsrSize DM_HCI_LINK_SUPERV_TIMEOUT_IND_TSizeof(void *msg);
CsrUint8 *DM_HCI_LINK_SUPERV_TIMEOUT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_LINK_SUPERV_TIMEOUT_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_LINK_SUPERV_TIMEOUT_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_LINK_SUPERV_TIMEOUT_IND */

#ifndef EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_RSP
CsrSize DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_TSizeof(void *msg);
CsrUint8 *DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_RSP */

#ifndef EXCLUDE_DM_SM_IO_CAPABILITY_RESPONSE_IND
CsrSize DM_SM_IO_CAPABILITY_RESPONSE_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_IO_CAPABILITY_RESPONSE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_IO_CAPABILITY_RESPONSE_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_IO_CAPABILITY_RESPONSE_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_IO_CAPABILITY_RESPONSE_IND */

#ifndef EXCLUDE_DM_HCI_READ_PIN_TYPE_REQ
CsrSize DM_HCI_READ_PIN_TYPE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_PIN_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_PIN_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_PIN_TYPE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_PIN_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_CFM
CsrSize DM_HCI_READ_PAGE_SCAN_TYPE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_PAGE_SCAN_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_PAGE_SCAN_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_PAGE_SCAN_TYPE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_AM_REGISTER_CFM
CsrSize DM_AM_REGISTER_CFM_TSizeof(void *msg);
CsrUint8 *DM_AM_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AM_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AM_REGISTER_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AM_REGISTER_CFM */

#ifndef EXCLUDE_DM_SET_BT_VERSION_REQ
CsrSize DM_SET_BT_VERSION_REQ_TSizeof(void *msg);
CsrUint8 *DM_SET_BT_VERSION_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SET_BT_VERSION_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SET_BT_VERSION_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SET_BT_VERSION_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_CFM
CsrSize DM_HCI_READ_PAGESCAN_MODE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_PAGESCAN_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_PAGESCAN_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_PAGESCAN_MODE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_REQ
CsrSize DM_HCI_CREATE_CONNECTION_CANCEL_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_CREATE_CONNECTION_CANCEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_CREATE_CONNECTION_CANCEL_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_CREATE_CONNECTION_CANCEL_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_CREATE_CONNECTION_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM
CsrSize DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_HCI_EXIT_SNIFF_MODE_REQ
CsrSize DM_HCI_EXIT_SNIFF_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_EXIT_SNIFF_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_EXIT_SNIFF_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_EXIT_SNIFF_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_EXIT_SNIFF_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_IND
CsrSize DM_HCI_REFRESH_ENCRYPTION_KEY_IND_TSizeof(void *msg);
CsrUint8 *DM_HCI_REFRESH_ENCRYPTION_KEY_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_REFRESH_ENCRYPTION_KEY_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_REFRESH_ENCRYPTION_KEY_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_REFRESH_ENCRYPTION_KEY_IND */

#ifndef EXCLUDE_DM_HCI_INQUIRY_CANCEL_REQ
CsrSize DM_HCI_INQUIRY_CANCEL_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_INQUIRY_CANCEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_INQUIRY_CANCEL_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_INQUIRY_CANCEL_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_INQUIRY_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_READ_SCAN_ENABLE_CFM
CsrSize DM_HCI_READ_SCAN_ENABLE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_SCAN_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_SCAN_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_SCAN_ENABLE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_DM_SM_SECURITY_IND
CsrSize DM_SM_SECURITY_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_SECURITY_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_SECURITY_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_SECURITY_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_SECURITY_IND */

#ifndef EXCLUDE_DM_SM_LINK_KEY_REQUEST_IND
CsrSize DM_SM_LINK_KEY_REQUEST_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_LINK_KEY_REQUEST_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_LINK_KEY_REQUEST_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_LINK_KEY_REQUEST_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_LINK_KEY_REQUEST_IND */

#ifndef EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP
CsrSize DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP_TSizeof(void *msg);
CsrUint8 *DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_REQ
CsrSize DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_CFM
CsrSize DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_CFM */

#ifndef EXCLUDE_DM_SM_ACCESS_IND
CsrSize DM_SM_ACCESS_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_ACCESS_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_ACCESS_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_ACCESS_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_ACCESS_IND */

#ifndef EXCLUDE_DM_HCI_RESET_CFM
CsrSize DM_HCI_RESET_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_RESET_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_RESET_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_RESET_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_RESET_CFM */

#ifndef EXCLUDE_DM_SM_READ_DEVICE_REQ
CsrSize DM_SM_READ_DEVICE_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_READ_DEVICE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_READ_DEVICE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_READ_DEVICE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_READ_DEVICE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_CURRENT_IAC_LAP_CFM
CsrSize DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_CURRENT_IAC_LAP_CFM */

#ifndef EXCLUDE_DM_SM_KEYPRESS_NOTIFICATION_IND
CsrSize DM_SM_KEYPRESS_NOTIFICATION_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_KEYPRESS_NOTIFICATION_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_KEYPRESS_NOTIFICATION_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_KEYPRESS_NOTIFICATION_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_KEYPRESS_NOTIFICATION_IND */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_REQ
CsrSize DM_HCI_READ_PAGE_SCAN_TYPE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_PAGE_SCAN_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_PAGE_SCAN_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_PAGE_SCAN_TYPE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_PAGE_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_POWERSTATES_CFM
CsrSize DM_LP_WRITE_POWERSTATES_CFM_TSizeof(void *msg);
CsrUint8 *DM_LP_WRITE_POWERSTATES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_LP_WRITE_POWERSTATES_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_LP_WRITE_POWERSTATES_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_LP_WRITE_POWERSTATES_CFM */

#ifndef EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_IND
CsrSize DM_AMPM_VERIFY_PHYSICAL_LINK_IND_TSizeof(void *msg);
CsrUint8 *DM_AMPM_VERIFY_PHYSICAL_LINK_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_VERIFY_PHYSICAL_LINK_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_VERIFY_PHYSICAL_LINK_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_VERIFY_PHYSICAL_LINK_IND */

#ifndef EXCLUDE_DM_HCI_SET_EVENT_MASK_REQ
CsrSize DM_HCI_SET_EVENT_MASK_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_SET_EVENT_MASK_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_SET_EVENT_MASK_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_SET_EVENT_MASK_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_DM_SYNC_CONNECT_IND
CsrSize DM_SYNC_CONNECT_IND_TSizeof(void *msg);
CsrUint8 *DM_SYNC_CONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SYNC_CONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SYNC_CONNECT_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SYNC_CONNECT_IND */

#ifndef EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ
CsrSize DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_ENC_MODE_REQ
CsrSize DM_HCI_READ_ENC_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_ENC_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_ENC_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_ENC_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_ENC_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PIN_TYPE_CFM
CsrSize DM_HCI_READ_PIN_TYPE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_PIN_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_PIN_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_PIN_TYPE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_PIN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ
CsrSize DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_REQ */

#ifndef EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ
CsrSize DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_CFM
CsrSize DM_HCI_READ_CLOCK_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_CLOCK_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_CLOCK_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_CLOCK_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_CFM */

#ifndef EXCLUDE_DM_AMPM_REMOTE_FEATURES_IND
CsrSize DM_AMPM_REMOTE_FEATURES_IND_TSizeof(void *msg);
CsrUint8 *DM_AMPM_REMOTE_FEATURES_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_REMOTE_FEATURES_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_REMOTE_FEATURES_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_REMOTE_FEATURES_IND */

#ifndef EXCLUDE_DM_SM_ADD_DEVICE_CFM
CsrSize DM_SM_ADD_DEVICE_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_ADD_DEVICE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_ADD_DEVICE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_ADD_DEVICE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_ADD_DEVICE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_RSSI_CFM
CsrSize DM_HCI_READ_RSSI_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_RSSI_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_RSSI_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_RSSI_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_RSSI_CFM */

#ifndef EXCLUDE_DM_DATA_FROM_HCI_REQ
CsrSize DM_DATA_FROM_HCI_REQ_TSizeof(void *msg);
CsrUint8 *DM_DATA_FROM_HCI_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_DATA_FROM_HCI_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void DM_DATA_FROM_HCI_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_DM_DATA_FROM_HCI_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_CFM
CsrSize DM_LP_WRITE_ROLESWITCH_POLICY_CFM_TSizeof(void *msg);
CsrUint8 *DM_LP_WRITE_ROLESWITCH_POLICY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_LP_WRITE_ROLESWITCH_POLICY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_LP_WRITE_ROLESWITCH_POLICY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_LP_WRITE_ROLESWITCH_POLICY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM
CsrSize DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_AMPM_CONNECT_CHANNEL_RSP
CsrSize DM_AMPM_CONNECT_CHANNEL_RSP_TSizeof(void *msg);
CsrUint8 *DM_AMPM_CONNECT_CHANNEL_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_CONNECT_CHANNEL_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_CONNECT_CHANNEL_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_CONNECT_CHANNEL_RSP */

#ifndef EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_CFM
CsrSize DM_WRITE_CACHED_PAGE_MODE_CFM_TSizeof(void *msg);
CsrUint8 *DM_WRITE_CACHED_PAGE_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_WRITE_CACHED_PAGE_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_WRITE_CACHED_PAGE_MODE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_CFM */

#ifndef EXCLUDE_DM_SM_READ_LOCAL_OOB_DATA_REQ
CsrSize DM_SM_READ_LOCAL_OOB_DATA_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_READ_LOCAL_OOB_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_READ_LOCAL_OOB_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_READ_LOCAL_OOB_DATA_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_READ_LOCAL_OOB_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_TEST_END_CFM
CsrSize DM_HCI_ULP_TEST_END_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_TEST_END_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_TEST_END_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_TEST_END_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_TEST_END_CFM */

#ifndef EXCLUDE_DM_LP_WRITE_POWERSTATES_REQ
CsrSize DM_LP_WRITE_POWERSTATES_REQ_TSizeof(void *msg);
CsrUint8 *DM_LP_WRITE_POWERSTATES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_LP_WRITE_POWERSTATES_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void DM_LP_WRITE_POWERSTATES_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_DM_LP_WRITE_POWERSTATES_REQ */

#ifndef EXCLUDE_DM_SM_ENCRYPT_REQ
CsrSize DM_SM_ENCRYPT_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_ENCRYPT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_ENCRYPT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_ENCRYPT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_ENCRYPT_REQ */

#ifndef EXCLUDE_DM_SM_KEY_REQUEST_NEG_RSP
CsrSize DM_SM_KEY_REQUEST_NEG_RSP_TSizeof(void *msg);
CsrUint8 *DM_SM_KEY_REQUEST_NEG_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_KEY_REQUEST_NEG_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_KEY_REQUEST_NEG_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_KEY_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ
CsrSize DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_REQ */

#ifndef EXCLUDE_DM_HCI_CONN_PACKET_TYPE_CHANGED_IND
CsrSize DM_HCI_CONN_PACKET_TYPE_CHANGED_IND_TSizeof(void *msg);
CsrUint8 *DM_HCI_CONN_PACKET_TYPE_CHANGED_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_CONN_PACKET_TYPE_CHANGED_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_CONN_PACKET_TYPE_CHANGED_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_CONN_PACKET_TYPE_CHANGED_IND */

#ifndef EXCLUDE_DM_HCI_SET_EVENT_FILTER_CFM
CsrSize DM_HCI_SET_EVENT_FILTER_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_SET_EVENT_FILTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_SET_EVENT_FILTER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_SET_EVENT_FILTER_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_SET_EVENT_FILTER_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_MODE_REQ
CsrSize DM_HCI_READ_INQUIRY_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_INQUIRY_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_INQUIRY_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_INQUIRY_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_SM_AUTHENTICATE_REQ
CsrSize DM_SM_AUTHENTICATE_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_AUTHENTICATE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_AUTHENTICATE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_AUTHENTICATE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_AUTHENTICATE_REQ */

#ifndef EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ
CsrSize DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_TSizeof(void *msg);
CsrUint8 *DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ
CsrSize DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_TSizeof(void *msg);
CsrUint8 *DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM
CsrSize DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ
CsrSize DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ */

#ifndef EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_REQ
CsrSize DM_HCI_CHANGE_CONN_LINK_KEY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_CHANGE_CONN_LINK_KEY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_CHANGE_CONN_LINK_KEY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_CHANGE_CONN_LINK_KEY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_SYNC_UNREGISTER_CFM
CsrSize DM_SYNC_UNREGISTER_CFM_TSizeof(void *msg);
CsrUint8 *DM_SYNC_UNREGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SYNC_UNREGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SYNC_UNREGISTER_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SYNC_UNREGISTER_CFM */

#ifndef EXCLUDE_DM_HCI_ROLE_DISCOVERY_REQ
CsrSize DM_HCI_ROLE_DISCOVERY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ROLE_DISCOVERY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ROLE_DISCOVERY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ROLE_DISCOVERY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ROLE_DISCOVERY_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM
CsrSize DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_AMPM_REGISTER_REQ
CsrSize DM_AMPM_REGISTER_REQ_TSizeof(void *msg);
CsrUint8 *DM_AMPM_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_REGISTER_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_REGISTER_REQ */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_CFM
CsrSize DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_CFM
CsrSize DM_HCI_READ_STORED_LINK_KEY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_STORED_LINK_KEY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_STORED_LINK_KEY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_STORED_LINK_KEY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_STORED_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_REQ
CsrSize DM_HANDLE_DEVICE_BLACK_LIST_REQ_TSizeof(void *msg);
CsrUint8 *DM_HANDLE_DEVICE_BLACK_LIST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HANDLE_DEVICE_BLACK_LIST_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HANDLE_DEVICE_BLACK_LIST_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HANDLE_DEVICE_BLACK_LIST_REQ */

#ifndef EXCLUDE_DM_SM_UNREGISTER_REQ
CsrSize DM_SM_UNREGISTER_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_UNREGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_UNREGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_UNREGISTER_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_UNREGISTER_REQ */

#ifndef EXCLUDE_DM_SM_DATA_SIGN_REQ
CsrSize DM_SM_DATA_SIGN_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_DATA_SIGN_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_DATA_SIGN_REQ_TDes(CsrUint8 *buffer, CsrSize length);
void DM_SM_DATA_SIGN_REQ_TSerFree(void *msg);
#endif /* EXCLUDE_DM_SM_DATA_SIGN_REQ */

#ifndef EXCLUDE_DM_HCI_MODE_CHANGE_EVENT_IND
CsrSize DM_HCI_MODE_CHANGE_EVENT_IND_TSizeof(void *msg);
CsrUint8 *DM_HCI_MODE_CHANGE_EVENT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_MODE_CHANGE_EVENT_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_MODE_CHANGE_EVENT_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_MODE_CHANGE_EVENT_IND */

#ifndef EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP
CsrSize DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_TSizeof(void *msg);
CsrUint8 *DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP */

#ifndef EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM
CsrSize DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM */

#ifndef EXCLUDE_DM_HCI_PARK_MODE_REQ
CsrSize DM_HCI_PARK_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_PARK_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_PARK_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_PARK_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_PARK_MODE_REQ */

#ifndef EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND
CsrSize DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND_TSizeof(void *msg);
CsrUint8 *DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_BLUESTACK_STATE_TRANSITION_ENTRY_IND */

#ifndef EXCLUDE_DM_AMPM_LINK_KEY_NOTIFICATION_IND
CsrSize DM_AMPM_LINK_KEY_NOTIFICATION_IND_TSizeof(void *msg);
CsrUint8 *DM_AMPM_LINK_KEY_NOTIFICATION_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_LINK_KEY_NOTIFICATION_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_LINK_KEY_NOTIFICATION_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_LINK_KEY_NOTIFICATION_IND */

#ifndef EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_CFM
CsrSize DM_HCI_SET_AFH_CHANNEL_CLASS_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_SET_AFH_CHANNEL_CLASS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_SET_AFH_CHANNEL_CLASS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_SET_AFH_CHANNEL_CLASS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_SET_AFH_CHANNEL_CLASS_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PIN_TYPE_REQ
CsrSize DM_HCI_WRITE_PIN_TYPE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_PIN_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_PIN_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_PIN_TYPE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_PIN_TYPE_REQ */

#ifndef EXCLUDE_DM_SM_INIT_REQ
CsrSize DM_SM_INIT_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_INIT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_INIT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_INIT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_INIT_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM
CsrSize DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM */

#ifndef EXCLUDE_DM_CLEAR_PARAM_CACHE_CFM
CsrSize DM_CLEAR_PARAM_CACHE_CFM_TSizeof(void *msg);
CsrUint8 *DM_CLEAR_PARAM_CACHE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_CLEAR_PARAM_CACHE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_CLEAR_PARAM_CACHE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_CLEAR_PARAM_CACHE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM
CsrSize DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_AMPM_CONNECT_IND
CsrSize DM_AMPM_CONNECT_IND_TSizeof(void *msg);
CsrUint8 *DM_AMPM_CONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_CONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_CONNECT_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_CONNECT_IND */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_REQ
CsrSize DM_HCI_READ_PAGESCAN_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_PAGESCAN_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_PAGESCAN_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_PAGESCAN_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_RESET_REQ
CsrSize DM_HCI_RESET_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_RESET_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_RESET_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_RESET_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_RESET_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ
CsrSize DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_REQ
CsrSize DM_HCI_ULP_CLEAR_WHITE_LIST_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_CLEAR_WHITE_LIST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_CLEAR_WHITE_LIST_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_CLEAR_WHITE_LIST_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_CLEAR_WHITE_LIST_REQ */

#ifndef EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_CFM
CsrSize DM_HCI_READ_NUM_BCAST_RETXS_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_NUM_BCAST_RETXS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_NUM_BCAST_RETXS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_NUM_BCAST_RETXS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_CFM
CsrSize DM_HCI_READ_LOCAL_VER_INFO_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_LOCAL_VER_INFO_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_LOCAL_VER_INFO_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_LOCAL_VER_INFO_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_VER_INFO_CFM */

#ifndef EXCLUDE_DM_HCI_QOS_SETUP_REQ
CsrSize DM_HCI_QOS_SETUP_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_QOS_SETUP_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_QOS_SETUP_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_QOS_SETUP_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_QOS_SETUP_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_CFM
CsrSize DM_HCI_READ_AFH_CHANNEL_MAP_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_AFH_CHANNEL_MAP_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_AFH_CHANNEL_MAP_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_AFH_CHANNEL_MAP_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_CFM */

#ifndef EXCLUDE_DM_SM_CSRK_COUNTER_CHANGE_IND
CsrSize DM_SM_CSRK_COUNTER_CHANGE_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_CSRK_COUNTER_CHANGE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_CSRK_COUNTER_CHANGE_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_CSRK_COUNTER_CHANGE_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_CSRK_COUNTER_CHANGE_IND */

#ifndef EXCLUDE_DM_HCI_INQUIRY_CFM
CsrSize DM_HCI_INQUIRY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_INQUIRY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_INQUIRY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_INQUIRY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_INQUIRY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AUTH_ENABLE_CFM
CsrSize DM_HCI_READ_AUTH_ENABLE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_AUTH_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_AUTH_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_AUTH_ENABLE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_AUTH_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_CFM
CsrSize DM_HCI_READ_CLASS_OF_DEVICE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_CLASS_OF_DEVICE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_CLASS_OF_DEVICE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_CLASS_OF_DEVICE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_REQ
CsrSize DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_REQ */

#ifndef EXCLUDE_DM_HCI_HOLD_MODE_REQ
CsrSize DM_HCI_HOLD_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_HOLD_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_HOLD_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_HOLD_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_HOLD_MODE_REQ */

#ifndef EXCLUDE_DM_SET_BT_VERSION_CFM
CsrSize DM_SET_BT_VERSION_CFM_TSizeof(void *msg);
CsrUint8 *DM_SET_BT_VERSION_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SET_BT_VERSION_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SET_BT_VERSION_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SET_BT_VERSION_CFM */

#ifndef EXCLUDE_DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ
CsrSize DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ */

#ifndef EXCLUDE_DM_ACL_CLOSE_REQ
CsrSize DM_ACL_CLOSE_REQ_TSizeof(void *msg);
CsrUint8 *DM_ACL_CLOSE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_ACL_CLOSE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_ACL_CLOSE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_ACL_CLOSE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM
CsrSize DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM */

#ifndef EXCLUDE_DM_SM_KEY_REQUEST_IND
CsrSize DM_SM_KEY_REQUEST_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_KEY_REQUEST_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_KEY_REQUEST_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_KEY_REQUEST_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_KEY_REQUEST_IND */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_REQ
CsrSize DM_HCI_WRITE_PAGE_TIMEOUT_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_PAGE_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_PAGE_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_PAGE_TIMEOUT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_REQ
CsrSize DM_HCI_ULP_READ_CHANNEL_MAP_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_READ_CHANNEL_MAP_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_READ_CHANNEL_MAP_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_READ_CHANNEL_MAP_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ
CsrSize DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_REQ
CsrSize DM_HCI_ULP_SET_SCAN_ENABLE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_SCAN_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_SCAN_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_SCAN_ENABLE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_REQ
CsrSize DM_HCI_READ_REMOTE_VER_INFO_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_REMOTE_VER_INFO_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_REMOTE_VER_INFO_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_REMOTE_VER_INFO_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ
CsrSize DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM
CsrSize DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_RANDOM_ADDRESS_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM
CsrSize DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_CFM
CsrSize DM_WRITE_CACHED_CLOCK_OFFSET_CFM_TSizeof(void *msg);
CsrUint8 *DM_WRITE_CACHED_CLOCK_OFFSET_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_WRITE_CACHED_CLOCK_OFFSET_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_WRITE_CACHED_CLOCK_OFFSET_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_CFM */

#ifndef EXCLUDE_DM_SM_REMOVE_DEVICE_CFM
CsrSize DM_SM_REMOVE_DEVICE_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_REMOVE_DEVICE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_REMOVE_DEVICE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_REMOVE_DEVICE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_REMOVE_DEVICE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM
CsrSize DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_CHANGE_LOCAL_NAME_CFM
CsrSize DM_HCI_CHANGE_LOCAL_NAME_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_CHANGE_LOCAL_NAME_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_CHANGE_LOCAL_NAME_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_CHANGE_LOCAL_NAME_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_CHANGE_LOCAL_NAME_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ
CsrSize DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM
CsrSize DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_INQUIRY_CANCEL_CFM
CsrSize DM_HCI_INQUIRY_CANCEL_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_INQUIRY_CANCEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_INQUIRY_CANCEL_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_INQUIRY_CANCEL_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_INQUIRY_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_CFM
CsrSize DM_HCI_ULP_READ_SUPPORTED_STATES_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_READ_SUPPORTED_STATES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_READ_SUPPORTED_STATES_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_READ_SUPPORTED_STATES_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_CFM
CsrSize DM_HCI_ULP_READ_CHANNEL_MAP_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_READ_CHANNEL_MAP_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_READ_CHANNEL_MAP_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_READ_CHANNEL_MAP_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_READ_CHANNEL_MAP_CFM */

#ifndef EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM
CsrSize DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_TSizeof(void *msg);
CsrUint8 *DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_REQ
CsrSize DM_HCI_READ_TX_POWER_LEVEL_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_TX_POWER_LEVEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_TX_POWER_LEVEL_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_TX_POWER_LEVEL_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_RAND_REQ
CsrSize DM_HCI_ULP_RAND_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_RAND_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_RAND_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_RAND_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_RAND_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_REQ
CsrSize DM_HCI_READ_AFH_CHANNEL_MAP_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_AFH_CHANNEL_MAP_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_AFH_CHANNEL_MAP_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_AFH_CHANNEL_MAP_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_MAP_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_CFM
CsrSize DM_HCI_WRITE_SCAN_ENABLE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_SCAN_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_SCAN_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_SCAN_ENABLE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_REQ
CsrSize DM_HCI_READ_LINK_POLICY_SETTINGS_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_LINK_POLICY_SETTINGS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_LINK_POLICY_SETTINGS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_LINK_POLICY_SETTINGS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_REQ
CsrSize DM_HCI_PERIODIC_INQUIRY_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_PERIODIC_INQUIRY_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_PERIODIC_INQUIRY_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_PERIODIC_INQUIRY_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_REQ */

#ifndef EXCLUDE_DM_SYNC_CONNECT_CFM
CsrSize DM_SYNC_CONNECT_CFM_TSizeof(void *msg);
CsrUint8 *DM_SYNC_CONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SYNC_CONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SYNC_CONNECT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SYNC_CONNECT_CFM */

#ifndef EXCLUDE_DM_SM_INIT_CFM
CsrSize DM_SM_INIT_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_INIT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_INIT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_INIT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_INIT_CFM */

#ifndef EXCLUDE_DM_SM_PIN_REQUEST_IND
CsrSize DM_SM_PIN_REQUEST_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_PIN_REQUEST_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_PIN_REQUEST_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_PIN_REQUEST_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_PIN_REQUEST_IND */

#ifndef EXCLUDE_DM_HCI_READ_RSSI_REQ
CsrSize DM_HCI_READ_RSSI_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_RSSI_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_RSSI_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_RSSI_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_RSSI_REQ */

#ifndef EXCLUDE_DM_AM_REGISTER_WRONG_API_CFM
CsrSize DM_AM_REGISTER_WRONG_API_CFM_TSizeof(void *msg);
CsrUint8 *DM_AM_REGISTER_WRONG_API_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AM_REGISTER_WRONG_API_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AM_REGISTER_WRONG_API_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AM_REGISTER_WRONG_API_CFM */

#ifndef EXCLUDE_DM_ACL_CONN_HANDLE_IND
CsrSize DM_ACL_CONN_HANDLE_IND_TSizeof(void *msg);
CsrUint8 *DM_ACL_CONN_HANDLE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_ACL_CONN_HANDLE_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_ACL_CONN_HANDLE_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_ACL_CONN_HANDLE_IND */

#ifndef EXCLUDE_DM_SYNC_DISCONNECT_REQ
CsrSize DM_SYNC_DISCONNECT_REQ_TSizeof(void *msg);
CsrUint8 *DM_SYNC_DISCONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SYNC_DISCONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SYNC_DISCONNECT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SYNC_DISCONNECT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ
CsrSize DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_SYNC_REGISTER_REQ
CsrSize DM_SYNC_REGISTER_REQ_TSizeof(void *msg);
CsrUint8 *DM_SYNC_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SYNC_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SYNC_REGISTER_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SYNC_REGISTER_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM
CsrSize DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ
CsrSize DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ
CsrSize DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_REQ */

#ifndef EXCLUDE_DM_HCI_SNIFF_SUB_RATE_REQ
CsrSize DM_HCI_SNIFF_SUB_RATE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_SNIFF_SUB_RATE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_SNIFF_SUB_RATE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_SNIFF_SUB_RATE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_SNIFF_SUB_RATE_REQ */

#ifndef EXCLUDE_DM_SM_AUTHENTICATE_CFM
CsrSize DM_SM_AUTHENTICATE_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_AUTHENTICATE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_AUTHENTICATE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_AUTHENTICATE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_AUTHENTICATE_CFM */

#ifndef EXCLUDE_DM_HCI_FLUSH_CFM
CsrSize DM_HCI_FLUSH_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_FLUSH_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_FLUSH_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_FLUSH_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_FLUSH_CFM */

#ifndef EXCLUDE_DM_AMPM_READ_BD_ADDR_CFM
CsrSize DM_AMPM_READ_BD_ADDR_CFM_TSizeof(void *msg);
CsrUint8 *DM_AMPM_READ_BD_ADDR_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_READ_BD_ADDR_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_READ_BD_ADDR_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_READ_BD_ADDR_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_CFM
CsrSize DM_HCI_ULP_SET_SCAN_ENABLE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_SCAN_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_SCAN_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_SCAN_ENABLE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_DM_HCI_MASTER_LINK_KEY_CFM
CsrSize DM_HCI_MASTER_LINK_KEY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_MASTER_LINK_KEY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_MASTER_LINK_KEY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_MASTER_LINK_KEY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_MASTER_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM
CsrSize DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_LINK_SUPERV_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_ACL_OPEN_CFM
CsrSize DM_ACL_OPEN_CFM_TSizeof(void *msg);
CsrUint8 *DM_ACL_OPEN_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_ACL_OPEN_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_ACL_OPEN_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_ACL_OPEN_CFM */

#ifndef EXCLUDE_DM_HCI_GET_LINK_QUALITY_REQ
CsrSize DM_HCI_GET_LINK_QUALITY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_GET_LINK_QUALITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_GET_LINK_QUALITY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_GET_LINK_QUALITY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_GET_LINK_QUALITY_REQ */

#ifndef EXCLUDE_DM_SYNC_DISCONNECT_IND
CsrSize DM_SYNC_DISCONNECT_IND_TSizeof(void *msg);
CsrUint8 *DM_SYNC_DISCONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SYNC_DISCONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SYNC_DISCONNECT_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SYNC_DISCONNECT_IND */

#ifndef EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ
CsrSize DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ
CsrSize DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_CFM
CsrSize DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_DATA_CFM */

#ifndef EXCLUDE_DM_AMPM_CONNECT_RSP
CsrSize DM_AMPM_CONNECT_RSP_TSizeof(void *msg);
CsrUint8 *DM_AMPM_CONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_CONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_CONNECT_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_CONNECT_RSP */

#ifndef EXCLUDE_DM_AMPM_CONNECT_CHANNEL_IND
CsrSize DM_AMPM_CONNECT_CHANNEL_IND_TSizeof(void *msg);
CsrUint8 *DM_AMPM_CONNECT_CHANNEL_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_CONNECT_CHANNEL_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_CONNECT_CHANNEL_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_CONNECT_CHANNEL_IND */

#ifndef EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_CFM
CsrSize DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_NUM_SUPPORTED_IAC_CFM */

#ifndef EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND
CsrSize DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_TSizeof(void *msg);
CsrUint8 *DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND */

#ifndef EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_CFM
CsrSize DM_HCI_CHANGE_CONN_LINK_KEY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_CHANGE_CONN_LINK_KEY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_CHANGE_CONN_LINK_KEY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_CHANGE_CONN_LINK_KEY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_CHANGE_CONN_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND
CsrSize DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_REQ
CsrSize DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_EXT_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AUTH_ENABLE_REQ
CsrSize DM_HCI_READ_AUTH_ENABLE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_AUTH_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_AUTH_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_AUTH_ENABLE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_AUTH_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_CFM
CsrSize DM_HCI_READ_TX_POWER_LEVEL_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_TX_POWER_LEVEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_TX_POWER_LEVEL_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_TX_POWER_LEVEL_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_TX_POWER_LEVEL_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_REQ
CsrSize DM_HCI_ULP_SET_EVENT_MASK_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_EVENT_MASK_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_EVENT_MASK_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_EVENT_MASK_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ
CsrSize DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_CFM
CsrSize DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_CLASS_OF_DEVICE_CFM */

#ifndef EXCLUDE_DM_HCI_ROLE_DISCOVERY_CFM
CsrSize DM_HCI_ROLE_DISCOVERY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ROLE_DISCOVERY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ROLE_DISCOVERY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ROLE_DISCOVERY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ROLE_DISCOVERY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_REQ
CsrSize DM_HCI_READ_CLASS_OF_DEVICE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_CLASS_OF_DEVICE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_CLASS_OF_DEVICE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_CLASS_OF_DEVICE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_CLASS_OF_DEVICE_REQ */

#ifndef EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_IND
CsrSize DM_AMPM_READ_DATA_BLOCK_SIZE_IND_TSizeof(void *msg);
CsrUint8 *DM_AMPM_READ_DATA_BLOCK_SIZE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_READ_DATA_BLOCK_SIZE_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_READ_DATA_BLOCK_SIZE_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_READ_DATA_BLOCK_SIZE_IND */

#ifndef EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP
CsrSize DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP_TSizeof(void *msg);
CsrUint8 *DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ
CsrSize DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM
CsrSize DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM */

#ifndef EXCLUDE_DM_HCI_ENHANCED_FLUSH_CFM
CsrSize DM_HCI_ENHANCED_FLUSH_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ENHANCED_FLUSH_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ENHANCED_FLUSH_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ENHANCED_FLUSH_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ENHANCED_FLUSH_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_CFM
CsrSize DM_HCI_WRITE_PAGESCAN_MODE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_PAGESCAN_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_PAGESCAN_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_PAGESCAN_MODE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_MODE_CFM */

#ifndef EXCLUDE_DM_SM_LINK_KEY_IND
CsrSize DM_SM_LINK_KEY_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_LINK_KEY_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_LINK_KEY_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_LINK_KEY_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_LINK_KEY_IND */

#ifndef EXCLUDE_DM_SM_UNREGISTER_OUTGOING_CFM
CsrSize DM_SM_UNREGISTER_OUTGOING_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_UNREGISTER_OUTGOING_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_UNREGISTER_OUTGOING_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_UNREGISTER_OUTGOING_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_UNREGISTER_OUTGOING_CFM */

#ifndef EXCLUDE_DM_HCI_READ_COUNTRY_CODE_CFM
CsrSize DM_HCI_READ_COUNTRY_CODE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_COUNTRY_CODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_COUNTRY_CODE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_COUNTRY_CODE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_COUNTRY_CODE_CFM */

#ifndef EXCLUDE_DM_SM_REGISTER_OUTGOING_REQ
CsrSize DM_SM_REGISTER_OUTGOING_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_REGISTER_OUTGOING_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_REGISTER_OUTGOING_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_REGISTER_OUTGOING_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_REGISTER_OUTGOING_REQ */

#ifndef EXCLUDE_DM_HCI_GET_LINK_QUALITY_CFM
CsrSize DM_HCI_GET_LINK_QUALITY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_GET_LINK_QUALITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_GET_LINK_QUALITY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_GET_LINK_QUALITY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_GET_LINK_QUALITY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LMP_HANDLE_REQ
CsrSize DM_HCI_READ_LMP_HANDLE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_LMP_HANDLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_LMP_HANDLE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_LMP_HANDLE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_LMP_HANDLE_REQ */

#ifndef EXCLUDE_DM_AMPM_SIMPLE_PAIRING_ENABLED_IND
CsrSize DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_TSizeof(void *msg);
CsrUint8 *DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_SIMPLE_PAIRING_ENABLED_IND */

#ifndef EXCLUDE_DM_BAD_MESSAGE_IND
CsrSize DM_BAD_MESSAGE_IND_TSizeof(void *msg);
CsrUint8 *DM_BAD_MESSAGE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_BAD_MESSAGE_IND_TDes(CsrUint8 *buffer, CsrSize length);
void DM_BAD_MESSAGE_IND_TSerFree(void *msg);
#endif /* EXCLUDE_DM_BAD_MESSAGE_IND */

#ifndef EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_REQ
CsrSize DM_HCI_ULP_RECEIVER_TEST_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_RECEIVER_TEST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_RECEIVER_TEST_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_RECEIVER_TEST_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_RECEIVER_TEST_REQ */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ
CsrSize DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_CFM
CsrSize DM_HCI_WRITE_PAGE_TIMEOUT_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_PAGE_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_PAGE_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_PAGE_TIMEOUT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_PAGE_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_CLOCK_REQ
CsrSize DM_HCI_READ_CLOCK_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_CLOCK_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_CLOCK_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_CLOCK_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_CLOCK_REQ */

#ifndef EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_REQ
CsrSize DM_WRITE_CACHED_CLOCK_OFFSET_REQ_TSizeof(void *msg);
CsrUint8 *DM_WRITE_CACHED_CLOCK_OFFSET_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_WRITE_CACHED_CLOCK_OFFSET_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_WRITE_CACHED_CLOCK_OFFSET_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_WRITE_CACHED_CLOCK_OFFSET_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_REQ
CsrSize DM_HCI_WRITE_VOICE_SETTING_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_VOICE_SETTING_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_VOICE_SETTING_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_VOICE_SETTING_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM
CsrSize DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ
CsrSize DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ */

#ifndef EXCLUDE_DM_SM_ENCRYPTION_CHANGE_IND
CsrSize DM_SM_ENCRYPTION_CHANGE_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_ENCRYPTION_CHANGE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_ENCRYPTION_CHANGE_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_ENCRYPTION_CHANGE_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_ENCRYPTION_CHANGE_IND */

#ifndef EXCLUDE_DM_SM_DATA_SIGN_CFM
CsrSize DM_SM_DATA_SIGN_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_DATA_SIGN_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_DATA_SIGN_CFM_TDes(CsrUint8 *buffer, CsrSize length);
void DM_SM_DATA_SIGN_CFM_TSerFree(void *msg);
#endif /* EXCLUDE_DM_SM_DATA_SIGN_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_CFM
CsrSize DM_HCI_ULP_TRANSMITTER_TEST_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_TRANSMITTER_TEST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_TRANSMITTER_TEST_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_TRANSMITTER_TEST_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_TRANSMITTER_TEST_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM
CsrSize DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_CONN_ACCEPT_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_REQ
CsrSize DM_HCI_READ_FAILED_CONTACT_COUNT_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_FAILED_CONTACT_COUNT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_FAILED_CONTACT_COUNT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_FAILED_CONTACT_COUNT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_FAILED_CONTACT_COUNT_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_TEST_END_REQ
CsrSize DM_HCI_ULP_TEST_END_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_TEST_END_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_TEST_END_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_TEST_END_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_TEST_END_REQ */

#ifndef EXCLUDE_DM_DATA_TO_HCI_IND
CsrSize DM_DATA_TO_HCI_IND_TSizeof(void *msg);
CsrUint8 *DM_DATA_TO_HCI_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_DATA_TO_HCI_IND_TDes(CsrUint8 *buffer, CsrSize length);
void DM_DATA_TO_HCI_IND_TSerFree(void *msg);
#endif /* EXCLUDE_DM_DATA_TO_HCI_IND */

#ifndef EXCLUDE_DM_HCI_SWITCH_ROLE_REQ
CsrSize DM_HCI_SWITCH_ROLE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_SWITCH_ROLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_SWITCH_ROLE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_SWITCH_ROLE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PIN_TYPE_CFM
CsrSize DM_HCI_WRITE_PIN_TYPE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_PIN_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_PIN_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_PIN_TYPE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_PIN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ
CsrSize DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_REQ */

#ifndef EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ
CsrSize DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_REQ
CsrSize DM_HCI_READ_LOCAL_EXT_FEATURES_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_LOCAL_EXT_FEATURES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_LOCAL_EXT_FEATURES_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_LOCAL_EXT_FEATURES_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_LOCAL_EXT_FEATURES_REQ */

#ifndef EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_CFM
CsrSize DM_HCI_PERIODIC_INQUIRY_MODE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_PERIODIC_INQUIRY_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_PERIODIC_INQUIRY_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_PERIODIC_INQUIRY_MODE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_PERIODIC_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_READ_VOICE_SETTING_REQ
CsrSize DM_HCI_READ_VOICE_SETTING_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_VOICE_SETTING_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_VOICE_SETTING_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_VOICE_SETTING_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_VOICE_SETTING_REQ */

#ifndef EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM
CsrSize DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_CONN_ACCEPT_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ
CsrSize DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_RESET_FAILED_CONTACT_COUNT_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_REQ
CsrSize DM_HCI_ULP_READ_SUPPORTED_STATES_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_READ_SUPPORTED_STATES_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_READ_SUPPORTED_STATES_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_READ_SUPPORTED_STATES_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_READ_SUPPORTED_STATES_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ
CsrSize DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_CHANGE_CONN_PKT_TYPE_REQ
CsrSize DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_CHANGE_CONN_PKT_TYPE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM
CsrSize DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ
CsrSize DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_SCO_FLOW_CON_ENABLE_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_REQ
CsrSize DM_HCI_WRITE_NUM_BCAST_RETXS_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_NUM_BCAST_RETXS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_NUM_BCAST_RETXS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_NUM_BCAST_RETXS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_NUM_BCAST_RETXS_REQ */

#ifndef EXCLUDE_DM_SYNC_RENEGOTIATE_CFM
CsrSize DM_SYNC_RENEGOTIATE_CFM_TSizeof(void *msg);
CsrUint8 *DM_SYNC_RENEGOTIATE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SYNC_RENEGOTIATE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SYNC_RENEGOTIATE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SYNC_RENEGOTIATE_CFM */

#ifndef EXCLUDE_DM_SM_UNREGISTER_CFM
CsrSize DM_SM_UNREGISTER_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_UNREGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_UNREGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_UNREGISTER_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_UNREGISTER_CFM */

#ifndef EXCLUDE_DM_HCI_MASTER_LINK_KEY_REQ
CsrSize DM_HCI_MASTER_LINK_KEY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_MASTER_LINK_KEY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_MASTER_LINK_KEY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_MASTER_LINK_KEY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_MASTER_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_CFM
CsrSize DM_HCI_READ_PAGE_TIMEOUT_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_PAGE_TIMEOUT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_PAGE_TIMEOUT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_PAGE_TIMEOUT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_CFM */

#ifndef EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_IND
CsrSize DM_AMPM_DISCONNECT_CHANNEL_IND_TSizeof(void *msg);
CsrUint8 *DM_AMPM_DISCONNECT_CHANNEL_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_DISCONNECT_CHANNEL_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_DISCONNECT_CHANNEL_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_IND */

#ifndef EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_CFM
CsrSize DM_HCI_READ_LINK_POLICY_SETTINGS_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_LINK_POLICY_SETTINGS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_LINK_POLICY_SETTINGS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_LINK_POLICY_SETTINGS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_LINK_POLICY_SETTINGS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_REQ
CsrSize DM_HCI_READ_PAGE_TIMEOUT_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_PAGE_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_PAGE_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_PAGE_TIMEOUT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_PAGE_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_CFM
CsrSize DM_HCI_WRITE_INQUIRY_MODE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_INQUIRY_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_INQUIRY_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_INQUIRY_MODE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_SNIFF_MODE_REQ
CsrSize DM_HCI_SNIFF_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_SNIFF_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_SNIFF_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_SNIFF_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_SNIFF_MODE_REQ */

#ifndef EXCLUDE_DM_SYNC_REGISTER_CFM
CsrSize DM_SYNC_REGISTER_CFM_TSizeof(void *msg);
CsrUint8 *DM_SYNC_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SYNC_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SYNC_REGISTER_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SYNC_REGISTER_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM
CsrSize DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM */

#ifndef EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_IND
CsrSize DM_SM_USER_PASSKEY_REQUEST_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_USER_PASSKEY_REQUEST_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_USER_PASSKEY_REQUEST_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_USER_PASSKEY_REQUEST_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_USER_PASSKEY_REQUEST_IND */

#ifndef EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ
CsrSize DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM
CsrSize DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM */

#ifndef EXCLUDE_DM_SYNC_UNREGISTER_REQ
CsrSize DM_SYNC_UNREGISTER_REQ_TSizeof(void *msg);
CsrUint8 *DM_SYNC_UNREGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SYNC_UNREGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SYNC_UNREGISTER_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SYNC_UNREGISTER_REQ */

#ifndef EXCLUDE_DM_HCI_READ_ENC_MODE_CFM
CsrSize DM_HCI_READ_ENC_MODE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_ENC_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_ENC_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_ENC_MODE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_ENC_MODE_CFM */

#ifndef EXCLUDE_DM_SM_ACCESS_REQ
CsrSize DM_SM_ACCESS_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_ACCESS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_ACCESS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_ACCESS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_ACCESS_REQ */

#ifndef EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_CFM
CsrSize DM_HCI_DELETE_STORED_LINK_KEY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_DELETE_STORED_LINK_KEY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_DELETE_STORED_LINK_KEY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_DELETE_STORED_LINK_KEY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_ENABLE_DUT_MODE_CFM
CsrSize DM_HCI_ENABLE_DUT_MODE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ENABLE_DUT_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ENABLE_DUT_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ENABLE_DUT_MODE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ENABLE_DUT_MODE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM
CsrSize DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_CFM */

#ifndef EXCLUDE_DM_HCI_READ_BD_ADDR_CFM
CsrSize DM_HCI_READ_BD_ADDR_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_BD_ADDR_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_BD_ADDR_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_BD_ADDR_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_BD_ADDR_CFM */

#ifndef EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_REQ
CsrSize DM_HCI_READ_NUM_BCAST_RETXS_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_NUM_BCAST_RETXS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_NUM_BCAST_RETXS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_NUM_BCAST_RETXS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_NUM_BCAST_RETXS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_CFM
CsrSize DM_HCI_WRITE_VOICE_SETTING_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_VOICE_SETTING_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_VOICE_SETTING_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_VOICE_SETTING_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_VOICE_SETTING_CFM */

#ifndef EXCLUDE_DM_SM_UNREGISTER_OUTGOING_REQ
CsrSize DM_SM_UNREGISTER_OUTGOING_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_UNREGISTER_OUTGOING_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_UNREGISTER_OUTGOING_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_UNREGISTER_OUTGOING_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_UNREGISTER_OUTGOING_REQ */

#ifndef EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_IND
CsrSize DM_SM_USER_CONFIRMATION_REQUEST_IND_TSizeof(void *msg);
CsrUint8 *DM_SM_USER_CONFIRMATION_REQUEST_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_USER_CONFIRMATION_REQUEST_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_USER_CONFIRMATION_REQUEST_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_USER_CONFIRMATION_REQUEST_IND */

#ifndef EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM
CsrSize DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_EXIT_PERIODIC_INQUIRY_MODE_CFM */

#ifndef EXCLUDE_DM_ACL_CLOSED_IND
CsrSize DM_ACL_CLOSED_IND_TSizeof(void *msg);
CsrUint8 *DM_ACL_CLOSED_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_ACL_CLOSED_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_ACL_CLOSED_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_ACL_CLOSED_IND */

#ifndef EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_RSP
CsrSize DM_AMPM_DISCONNECT_CHANNEL_RSP_TSizeof(void *msg);
CsrUint8 *DM_AMPM_DISCONNECT_CHANNEL_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_AMPM_DISCONNECT_CHANNEL_RSP_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_AMPM_DISCONNECT_CHANNEL_RSP_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_AMPM_DISCONNECT_CHANNEL_RSP */

#ifndef EXCLUDE_DM_HCI_WRITE_STORED_LINK_KEY_CFM
CsrSize DM_HCI_WRITE_STORED_LINK_KEY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_STORED_LINK_KEY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_STORED_LINK_KEY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_STORED_LINK_KEY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_STORED_LINK_KEY_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM
CsrSize DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_SCO_FLOW_CON_ENABLE_CFM */

#ifndef EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_REQ
CsrSize DM_SET_BLE_CONNECTION_PARAMETERS_REQ_TSizeof(void *msg);
CsrUint8 *DM_SET_BLE_CONNECTION_PARAMETERS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SET_BLE_CONNECTION_PARAMETERS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SET_BLE_CONNECTION_PARAMETERS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SET_BLE_CONNECTION_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ
CsrSize DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_REQ */

#ifndef EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_REQ
CsrSize DM_WRITE_CACHED_PAGE_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_WRITE_CACHED_PAGE_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_WRITE_CACHED_PAGE_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_WRITE_CACHED_PAGE_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_WRITE_CACHED_PAGE_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_EXIT_PARK_MODE_REQ
CsrSize DM_HCI_EXIT_PARK_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_EXIT_PARK_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_EXIT_PARK_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_EXIT_PARK_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_EXIT_PARK_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_REMOTE_NAME_REQUEST_REQ
CsrSize DM_HCI_REMOTE_NAME_REQUEST_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_REMOTE_NAME_REQUEST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_REMOTE_NAME_REQUEST_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_REMOTE_NAME_REQUEST_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_REMOTE_NAME_REQUEST_REQ */

#ifndef EXCLUDE_DM_SM_REGISTER_OUTGOING_CFM
CsrSize DM_SM_REGISTER_OUTGOING_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_REGISTER_OUTGOING_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_REGISTER_OUTGOING_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_REGISTER_OUTGOING_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_REGISTER_OUTGOING_CFM */

#ifndef EXCLUDE_DM_SM_ENCRYPT_CFM
CsrSize DM_SM_ENCRYPT_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_ENCRYPT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_ENCRYPT_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_ENCRYPT_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_ENCRYPT_CFM */

#ifndef EXCLUDE_DM_ACL_OPENED_IND
CsrSize DM_ACL_OPENED_IND_TSizeof(void *msg);
CsrUint8 *DM_ACL_OPENED_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_ACL_OPENED_IND_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_ACL_OPENED_IND_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_ACL_OPENED_IND */

#ifndef EXCLUDE_DM_HCI_FLOW_SPEC_CFM
CsrSize DM_HCI_FLOW_SPEC_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_FLOW_SPEC_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_FLOW_SPEC_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_FLOW_SPEC_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_FLOW_SPEC_CFM */

#ifndef EXCLUDE_DM_HCI_ENABLE_DUT_MODE_REQ
CsrSize DM_HCI_ENABLE_DUT_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ENABLE_DUT_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ENABLE_DUT_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ENABLE_DUT_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ENABLE_DUT_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM
CsrSize DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_CREATE_CONNECTION_CANCEL_CFM */

#ifndef EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM
CsrSize DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_INQUIRY_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ
CsrSize DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM
CsrSize DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_CFM
CsrSize DM_HCI_READ_REMOTE_VER_INFO_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_REMOTE_VER_INFO_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_REMOTE_VER_INFO_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_REMOTE_VER_INFO_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_REMOTE_VER_INFO_CFM */

#ifndef EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM
CsrSize DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_SM_BONDING_CFM
CsrSize DM_SM_BONDING_CFM_TSizeof(void *msg);
CsrUint8 *DM_SM_BONDING_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_BONDING_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_BONDING_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_BONDING_CFM */

#ifndef EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ
CsrSize DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM
CsrSize DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_READ_WHITE_LIST_SIZE_CFM */

#ifndef EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ
CsrSize DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ */

#ifndef EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM
CsrSize DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM */

#ifndef EXCLUDE_DM_SM_BONDING_CANCEL_REQ
CsrSize DM_SM_BONDING_CANCEL_REQ_TSizeof(void *msg);
CsrUint8 *DM_SM_BONDING_CANCEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_SM_BONDING_CANCEL_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_SM_BONDING_CANCEL_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_SM_BONDING_CANCEL_REQ */

#ifndef EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_REQ
CsrSize DM_HCI_DELETE_STORED_LINK_KEY_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_DELETE_STORED_LINK_KEY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_DELETE_STORED_LINK_KEY_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_DELETE_STORED_LINK_KEY_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_DELETE_STORED_LINK_KEY_REQ */

#ifndef EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ
CsrSize DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM
CsrSize DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_READ_REMOTE_USED_FEATURES_CFM */

#ifndef EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM
CsrSize DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_HOLD_MODE_ACTIVITY_CFM */

#ifndef EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ
CsrSize DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_PAGESCAN_PERIOD_MODE_REQ */

#ifndef EXCLUDE_DM_HCI_FLOW_SPEC_REQ
CsrSize DM_HCI_FLOW_SPEC_REQ_TSizeof(void *msg);
CsrUint8 *DM_HCI_FLOW_SPEC_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_FLOW_SPEC_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_FLOW_SPEC_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_FLOW_SPEC_REQ */

#ifndef EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM
CsrSize DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM */

#ifndef EXCLUDE_DM_ACL_OPEN_REQ
CsrSize DM_ACL_OPEN_REQ_TSizeof(void *msg);
CsrUint8 *DM_ACL_OPEN_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_ACL_OPEN_REQ_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_ACL_OPEN_REQ_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_ACL_OPEN_REQ */

#ifndef EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM
CsrSize DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM */

#ifndef EXCLUDE_DM_HCI_READ_LMP_HANDLE_CFM
CsrSize DM_HCI_READ_LMP_HANDLE_CFM_TSizeof(void *msg);
CsrUint8 *DM_HCI_READ_LMP_HANDLE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *DM_HCI_READ_LMP_HANDLE_CFM_TDes(CsrUint8 *buffer, CsrSize length);
#define DM_HCI_READ_LMP_HANDLE_CFM_TSerFree CsrBtDmPfree
#endif /* EXCLUDE_DM_HCI_READ_LMP_HANDLE_CFM */

#ifdef __cplusplus
}
#endif

#endif

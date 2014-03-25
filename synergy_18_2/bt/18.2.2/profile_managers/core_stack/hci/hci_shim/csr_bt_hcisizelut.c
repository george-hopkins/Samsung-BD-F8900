/*******************************************************************************

                (c) CSR plc 2010

                All rights reserved

*******************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_common.h"
#include "hci_prim.h"

static const CsrUint8 csrBtHcishimLinkLengths[HCI_MAX_LINK_OCF & HCI_OPCODE_MASK] =
{
    0,                                                        /* 0x0000 -- not used */
    HCI_INQUIRY_PARAM_LEN,                                    /* 0x0001 HCI_INQUIRY */
    HCI_INQUIRY_CANCEL_PARAM_LEN,                             /* 0x0002 HCI_INQUIRY_CANCEL */
    HCI_PERIODIC_INQUIRY_MODE_PARAM_LEN,                      /* 0x0003 HCI_PERIODIC_INQUIRY_MODE */
    HCI_EXIT_PERIODIC_INQUIRY_MODE_PARAM_LEN,                 /* 0x0004 HCI_EXIT_PERIODIC_INQUIRY_MODE */
    HCI_CREATE_CONNECTION_PARAM_LEN,                          /* 0x0005 HCI_CREATE_CONNECTION */
    HCI_DISCONNECT_PARAM_LEN,                                 /* 0x0006 HCI_DISCONNECT */
    HCI_ADD_SCO_CONNECTION_PARAM_LEN,                         /* 0x0007 HCI_ADD_SCO_CONNECTION */
    HCI_CREATE_CONNECTION_CANCEL_PARAM_LEN,                   /* 0x0008 HCI_CREATE_CONNECTION_CANCEL */
    HCI_ACCEPT_CONNECTION_REQ_PARAM_LEN,                      /* 0x0009 HCI_ACCEPT_CONNECTION_REQ */
    HCI_REJECT_CONNECTION_REQ_PARAM_LEN,                      /* 0x000A HCI_REJECT_CONNECTION_REQ */
    HCI_LINK_KEY_REQ_REPLY_PARAM_LEN,                         /* 0x000B HCI_LINK_KEY_REQ_REPLY */
    HCI_LINK_KEY_REQ_NEG_REPLY_PARAM_LEN,                     /* 0x000C HCI_LINK_KEY_REQ_NEG_REPLY */
    HCI_PIN_CODE_REQ_REPLY_PARAM_LEN,                         /* 0x000D HCI_PIN_CODE_REQ_REPLY */
    HCI_PIN_CODE_REQ_NEG_REPLY_PARAM_LEN,                     /* 0x000E HCI_PIN_CODE_REQ_NEG_REPLY */
    HCI_CHANGE_CONN_PKT_TYPE_PARAM_LEN,                       /* 0x000F HCI_CHANGE_CONN_PKT_TYPE */
    0,                                                        /* 0x0010 -- not used */
    HCI_AUTH_REQ_PARAM_LEN,                                   /* 0x0011 HCI_AUTH_REQ */
    0,                                                        /* 0x0012 -- not used */
    HCI_SET_CONN_ENCRYPTION_PARAM_LEN,                        /* 0x0013 HCI_SET_CONN_ENCRYPTION */
    0,                                                        /* 0x0014 -- not used */
    HCI_CHANGE_CONN_LINK_KEY_PARAM_LEN,                       /* 0x0015 HCI_CHANGE_CONN_LINK_KEY */
    0,                                                        /* 0x0016 -- not used */
    HCI_MASTER_LINK_KEY_PARAM_LEN,                            /* 0x0017 HCI_MASTER_LINK_KEY */
    0,                                                        /* 0x0018 -- not used */
    HCI_REMOTE_NAME_REQ_PARAM_LEN,                            /* 0x0019 HCI_REMOTE_NAME_REQ */
    HCI_REMOTE_NAME_REQ_CANCEL_PARAM_LEN,                     /* 0x001A HCI_REMOTE_NAME_REQ_CANCEL */
    HCI_READ_REMOTE_SUPP_FEATURES_PARAM_LEN,                  /* 0x001B HCI_READ_REMOTE_SUPP_FEATURES */
    HCI_READ_REMOTE_EXT_FEATURES_PARAM_LEN,                   /* 0x001C HCI_READ_REMOTE_EXT_FEATURES */
    HCI_READ_REMOTE_VER_INFO_PARAM_LEN,                       /* 0x001D HCI_READ_REMOTE_VER_INFO */
    0,                                                        /* 0x001E -- not used */
    HCI_READ_CLOCK_OFFSET_PARAM_LEN,                          /* 0x001F HCI_READ_CLOCK_OFFSET */
    HCI_READ_LMP_HANDLE_PARAM_LEN,                            /* 0x0020 HCI_READ_LMP_HANDLE_PARAM */
    HCI_EXCHANGE_FIXED_INFO_PARAM_LEN,                        /* 0x0021 HCI_EXCHANGE_FIXED_INFO */
    HCI_EXCHANGE_ALIAS_INFO_PARAM_LEN,                        /* 0x0022 HCI_EXCHANGE_ALIAS_INFO */
    HCI_PRIVATE_PAIRING_REQ_REPLY_PARAM_LEN,                  /* 0x0023 HCI_PRIVATE_PAIRING_REQ_REPLY */
    HCI_PRIVATE_PAIRING_REQ_NEG_REPLY_PARAM_LEN,              /* 0x0024 HCI_PRIVATE_PAIRING_REQ_NEG_REPLY */
    HCI_GENERATED_ALIAS_PARAM_LEN,                            /* 0x0025 HCI_GENERATED_ALIAS */
    HCI_ALIAS_ADDRESS_REQ_REPLY_PARAM_LEN,                    /* 0x0026 HCI_ALIAS_ADDRESS_REQ_REPLY */
    HCI_ALIAS_ADDRESS_REQ_NEG_REPLY_PARAM_LEN,                /* 0x0027 HCI_ALIAS_ADDRESS_REQ_NEG_REPLY */
    HCI_SETUP_SYNCHRONOUS_CONN_PARAM_LEN,                     /* 0x0028 HCI_SETUP_SYNCHRONOUS_CONN */
    HCI_ACCEPT_SYNCHRONOUS_CONN_REQ_PARAM_LEN,                /* 0x0029 HCI_ACCEPT_SYNCHRONOUS_CONN_REQ */
    HCI_REJECT_SYNCHRONOUS_CONN_REQ_PARAM_LEN,                /* 0x002A HCI_REJECT_SYNCHRONOUS_CONN_REQ */
    HCI_IO_CAPABILITY_RESPONSE_LEN,                           /* 0x002B HCI_IO_CAPABILITY_RESPONSE */
    HCI_USER_CONFIRMATION_REQUEST_REPLY_LEN,                  /* 0x002C HCI_USER_CONFIRMATION_REQUEST_REPLY */
    HCI_USER_CONFIRMATION_REQEST_NEG_REPLY_LEN,               /* 0x002D HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY */
    HCI_USER_PASSKEY_REQUEST_REPLY_LEN,                       /* 0x002E HCI_USER_PASSKEY_REQUEST_REPLY */
    HCI_USER_PASSKEY_REQUEST_NEG_REPLY_LEN,                   /* 0x002F HCI_USER_PASSKEY_REQUEST_NEG_REPLY */
    HCI_REMOTE_OOB_DATA_REQUEST_REPLY_LEN,                    /* 0x0030 HCI_REMOTE_OOB_DATA_REQUEST_REPLY */
    0,                                                        /* 0x0031 -- not used */
    0,                                                        /* 0x0032 -- not used */
    HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY_LEN,                /* 0x0033 HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY */
    HCI_IO_CAPABILITY_REQUEST_NEG_REPLY_LEN,                  /* 0x0034 HCI_IO_CAPABILITY_REQUEST_NEG_REPLY */
};

static const CsrUint8 csrBtHcishimPolicyLengths[HCI_MAX_POLICY_OCF & HCI_OPCODE_MASK] =
{
    0,                                                        /* 0x0000 -- not used */
    HCI_HOLD_MODE_PARAM_LEN,                                  /* 0x0001 HCI_HOLD_MODE */
    0,                                                        /* 0x0002 -- not used */
    HCI_SNIFF_MODE_PARAM_LEN,                                 /* 0x0003 HCI_SNIFF_MODE */
    HCI_EXIT_SNIFF_MODE_PARAM_LEN,                            /* 0x0004 HCI_EXIT_SNIFF_MODE */
    HCI_PARK_MODE_PARAM_LEN,                                  /* 0x0005 HCI_PARK_MODE */
    HCI_EXIT_PARK_MODE_PARAM_LEN,                             /* 0x0006 HCI_EXIT_PARK_MODE */
    HCI_QOS_SETUP_PARAM_LEN,                                  /* 0x0007 HCI_QOS_SETUP */
    0,                                                        /* 0x0008 -- not used */
    HCI_ROLE_DISCOVERY_PARAM_LEN,                             /* 0x0009 HCI_ROLE_DISCOVERY */
    0,                                                        /* 0x000A -- not used */
    HCI_SWITCH_ROLE_PARAM_LEN,                                /* 0x000B HCI_SWITCH_ROLE */
    HCI_READ_LINK_POLICY_SETTINGS_PARAM_LEN,                  /* 0x000C HCI_READ_LINK_POLICY_SETTINGS */
    HCI_WRITE_LINK_POLICY_SETTINGS_PARAM_LEN,                 /* 0x000D HCI_WRITE_LINK_POLICY_SETTINGS */
    HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_PARAM_LEN,          /* 0x000E HCI_READ_DEFAULT_LINK_POLICY_SETTINGS */
    HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_PARAM_LEN,         /* 0x000F HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS */
    HCI_FLOW_SPEC_PARAM_LEN,                                  /* 0x0010 HCI_FLOW_SPEC */
    HCI_SNIFF_SUB_RATE_PARAM_LEN                              /* 0x0011 HCI_SNIFF_SUB_RATE */
};

static const CsrUint8 csrBtHcishimHostBbLengths[HCI_MAX_HOST_BB_OCF & HCI_OPCODE_MASK] =
{
    0,                                                        /* 0x0000 -- not used */
    HCI_SET_EVENT_MASK_PARAM_LEN,                             /* 0x0001 HCI_SET_EVENT_MASK */
    0,                                                        /* 0x0002 -- not used */
    HCI_RESET_PARAM_LEN,                                      /* 0x0003 HCI_RESET */
    0,                                                        /* 0x0004 -- not used */
    HCI_SET_EVENT_FILTER_PARAM_LEN,                           /* 0x0005 HCI_SET_EVENT_FILTER -- variable length */
    0,                                                        /* 0x0006 -- not used */
    0,                                                        /* 0x0007 -- not used */
    HCI_FLUSH_PARAM_LEN,                                      /* 0x0008 HCI_FLUSH */
    HCI_READ_PIN_TYPE_PARAM_LEN,                              /* 0x0009 HCI_READ_PIN_TYPE */
    HCI_WRITE_PIN_TYPE_PARAM_LEN,                             /* 0x000A HCI_WRITE_PIN_TYPE */
    0,                                                        /* 0x000B HCI_CREATE_NEW_UNIT_KEY */
    0,                                                        /* 0x000C -- not used */
    HCI_READ_STORED_LINK_KEY_PARAM_LEN,                       /* 0x000D HCI_READ_STORED_LINK_KEY */
    0,                                                        /* 0x000E -- not used */
    0,                                                        /* 0x000F -- not used */
    0,                                                        /* 0x0010 -- not used */
    HCI_WRITE_STORED_LINK_KEY_PARAM_LEN,                      /* 0x0011 HCI_WRITE_STORED_LINK_KEY -- variable length */
    HCI_DELETE_STORED_LINK_KEY_PARAM_LEN,                     /* 0x0012 HCI_DELETE_STORED_LINK_KEY */
    HCI_CHANGE_LOCAL_NAME_PARAM_LEN,                          /* 0x0013 HCI_CHANGE_LOCAL_NAME */
    HCI_READ_LOCAL_NAME_PARAM_LEN,                            /* 0x0014 HCI_READ_LOCAL_NAME */
    HCI_READ_CONN_ACCEPT_TIMEOUT_PARAM_LEN,                   /* 0x0015 HCI_READ_CONN_ACCEPT_TIMEOUT */
    HCI_WRITE_CONN_ACCEPT_TIMEOUT_PARAM_LEN,                  /* 0x0016 HCI_WRITE_CONN_ACCEPT_TIMEOUT */
    HCI_READ_PAGE_TIMEOUT_PARAM_LEN,                          /* 0x0017 HCI_READ_PAGE_TIMEOUT */
    HCI_WRITE_PAGE_TIMEOUT_PARAM_LEN,                         /* 0x0018 HCI_WRITE_PAGE_TIMEOUT */
    HCI_READ_SCAN_ENABLE_PARAM_LEN,                           /* 0x0019 HCI_READ_SCAN_ENABLE */
    HCI_WRITE_SCAN_ENABLE_PARAM_LEN,                          /* 0x001A HCI_WRITE_SCAN_ENABLE */
    HCI_READ_PAGESCAN_ACTIVITY_PARAM_LEN,                     /* 0x001B HCI_READ_PAGESCAN_ACTIVITY */
    HCI_WRITE_PAGESCAN_ACTIVITY_PARAM_LEN,                    /* 0x001C HCI_WRITE_PAGESCAN_ACTIVITY */
    HCI_READ_INQUIRYSCAN_ACTIVITY_PARAM_LEN,                  /* 0x001D HCI_READ_INQUIRYSCAN_ACTIVITY */
    HCI_WRITE_INQUIRYSCAN_ACTIVITY_PARAM_LEN,                 /* 0x001E HCI_WRITE_INQUIRYSCAN_ACTIVITY */
    HCI_READ_AUTH_ENABLE_PARAM_LEN,                           /* 0x001F HCI_READ_AUTH_ENABLE */
    HCI_WRITE_AUTH_ENABLE_PARAM_LEN,                          /* 0x0020 HCI_WRITE_AUTH_ENABLE */
    HCI_READ_ENC_MODE_PARAM_LEN,                              /* 0x0021 HCI_READ_ENC_MODE */
    HCI_WRITE_ENC_MODE_PARAM_LEN,                             /* 0x0022 HCI_WRITE_ENC_MODE */
    HCI_READ_CLASS_OF_DEVICE_PARAM_LEN,                       /* 0x0023 HCI_READ_CLASS_OF_DEVICE */
    HCI_WRITE_CLASS_OF_DEVICE_PARAM_LEN,                      /* 0x0024 HCI_WRITE_CLASS_OF_DEVICE */
    HCI_READ_VOICE_SETTING_PARAM_LEN,                         /* 0x0025 HCI_READ_VOICE_SETTING */
    HCI_WRITE_VOICE_SETTING_PARAM_LEN,                        /* 0x0026 HCI_WRITE_VOICE_SETTING */
    HCI_READ_AUTO_FLUSH_TIMEOUT_PARAM_LEN,                    /* 0x0027 HCI_READ_AUTO_FLUSH_TIMEOUT */
    HCI_WRITE_AUTO_FLUSH_TIMEOUT_PARAM_LEN,                   /* 0x0028 HCI_WRITE_AUTO_FLUSH_TIMEOUT */
    HCI_READ_NUM_BCAST_RETXS_PARAM_LEN,                       /* 0x0029 HCI_READ_NUM_BCAST_RETXS */
    HCI_WRITE_NUM_BCAST_RETXS_PARAM_LEN,                      /* 0x002A HCI_WRITE_NUM_BCAST_RETXS */
    HCI_READ_HOLD_MODE_ACTIVITY_PARAM_LEN,                    /* 0x002B HCI_READ_HOLD_MODE_ACTIVITY */
    HCI_WRITE_HOLD_MODE_ACTIVITY_PARAM_LEN,                   /* 0x002C HCI_WRITE_HOLD_MODE_ACTIVITY */
    HCI_READ_TX_POWER_LEVEL_PARAM_LEN,                        /* 0x002D HCI_READ_TX_POWER_LEVEL */
    0,                                                        /* 0x002E HCI_READ_SCO_FLOW_CON_ENABLE */
    HCI_WRITE_SCO_FLOW_CON_ENABLE_PARAM_LEN,                  /* 0x002F HCI_WRITE_SCO_FLOW_CON_ENABLE */
    0,                                                        /* 0x0030 -- not used */
    HCI_SET_HCTOHOST_FLOW_CONTROL_PARAM_LEN,                  /* 0x0031 HCI_SET_HCTOHOST_FLOW_CONTROL */
    0,                                                        /* 0x0032 -- not used */
    HCI_HOST_BUFFER_SIZE_PARAM_LEN,                           /* 0x0033 HCI_HOST_BUFFER_SIZE */
    0,                                                        /* 0x0034 -- not used */
    HCI_HOST_NUM_COMPLETED_PACKETS_PARAM_LEN,                 /* 0x0035 HCI_HOST_NUM_COMPLETED_PACKETS -- variable length */
    HCI_READ_LINK_SUPERV_TIMEOUT_PARAM_LEN,                   /* 0x0036 HCI_READ_LINK_SUPERV_TIMEOUT */
    HCI_WRITE_LINK_SUPERV_TIMEOUT_PARAM_LEN,                  /* 0x0037 HCI_WRITE_LINK_SUPERV_TIMEOUT */
    HCI_READ_NUM_SUPPORTED_IAC_PARAM_LEN,                     /* 0x0038 HCI_READ_NUM_SUPPORTED_IAC */
    HCI_READ_CURRENT_IAC_LAP_PARAM_LEN,                       /* 0x0039 HCI_READ_CURRENT_IAC_LAP */
    HCI_WRITE_CURRENT_IAC_LAP_PARAM_LEN,                      /* 0x003A HCI_WRITE_CURRENT_IAC_LAP -- variable length */
    HCI_READ_PAGESCAN_PERIOD_MODE_PARAM_LEN,                  /* 0x003B HCI_READ_PAGESCAN_PERIOD_MODE */
    HCI_WRITE_PAGESCAN_PERIOD_MODE_PARAM_LEN,                 /* 0x003C HCI_WRITE_PAGESCAN_PERIOD_MODE */
    HCI_READ_PAGESCAN_MODE_PARAM_LEN,                         /* 0x003D HCI_READ_PAGESCAN_MODE */
    HCI_WRITE_PAGESCAN_MODE_PARAM_LEN,                        /* 0x003E HCI_WRITE_PAGESCAN_MODE */
    HCI_SET_AFH_CHANNEL_CLASS_PARAM_LEN,                      /* 0x003F HCI_SET_AFH_CHANNEL_CLASS */
    0,                                                        /* 0x0040 -- not used */
    0,                                                        /* 0x0041 -- not used */
    HCI_READ_INQUIRY_SCAN_TYPE_PARAM_LEN,                     /* 0x0042 HCI_READ_INQUIRY_SCAN_TYPE */
    HCI_WRITE_INQUIRY_SCAN_TYPE_PARAM_LEN,                    /* 0x0043 HCI_WRITE_INQUIRY_SCAN_TYPE */
    HCI_READ_INQUIRY_MODE_PARAM_LEN,                          /* 0x0044 HCI_READ_INQUIRY_MODE */
    HCI_WRITE_INQUIRY_MODE_PARAM_LEN,                         /* 0x0045 HCI_WRITE_INQUIRY_MODE */
    HCI_READ_PAGE_SCAN_TYPE_PARAM_LEN,                        /* 0x0046 HCI_READ_PAGE_SCAN_TYPE */
    HCI_WRITE_PAGE_SCAN_TYPE_PARAM_LEN,                       /* 0x0047 HCI_WRITE_PAGE_SCAN_TYPE */
    HCI_READ_AFH_CHANNEL_CLASS_M_PARAM_LEN,                   /* 0x0048 HCI_READ_AFH_CHANNEL_CLASS_M */
    HCI_WRITE_AFH_CHANNEL_CLASS_M_PARAM_LEN,                  /* 0x0049 HCI_WRITE_AFH_CHANNEL_CLASS_M */
    HCI_READ_ANON_MODE_PARAM_LEN,                             /* 0x004A HCI_READ_ANON_MODE */
    HCI_WRITE_ANON_MODE_PARAM_LEN,                            /* 0x004B HCI_WRITE_ANON_MODE */
    HCI_READ_ALIAS_AUTH_ENABLE_PARAM_LEN,                     /* 0x004C HCI_READ_ALIAS_AUTH_ENABLE */
    HCI_WRITE_ALIAS_AUTH_ENABLE_PARAM_LEN,                    /* 0x004D HCI_WRITE_ALIAS_AUTH_ENABLE */
    HCI_READ_ANON_ADDR_CHANGE_PARAMS_PARAM_LEN,               /* 0x004E HCI_READ_ANON_ADDR_CHANGE_PARAMS */
    HCI_WRITE_ANON_ADDR_CHANGE_PARAMS_PARAM_LEN,              /* 0x004F HCI_WRITE_ANON_ADDR_CHANGE_PARAMS */
    HCI_RESET_FIXED_ADDRESS_ATTEMPTS_COUNTER_PARAM_LEN,       /* 0x0050 HCI_RESET_FIXED_ADDRESS_ATTEMPTS_COUNTER */
    HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_PARAM_LEN,        /* 0x0051 HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA */
    HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_PARAM_LEN,       /* 0x0052 HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA */
    HCI_REFRESH_ENCRYPTION_KEY_PARAM_LEN,                     /* 0x0053 HCI_REFRESH_ENCRYPTION_KEY */
    0,                                                        /* 0x0054 -- not used */
    HCI_READ_SIMPLE_PAIRING_MODE_PARAM_LEN,                   /* 0x0055 HCI_READ_SIMPLE_PAIRING_MODE */
    HCI_WRITE_SIMPLE_PAIRING_MODE_PARAM_LEN,                  /* 0x0056 HCI_WRITE_SIMPLE_PAIRING_MODE */
    HCI_READ_LOCAL_OOB_DATA_PARAM_LEN,                        /* 0x0057 HCI_READ_LOCAL_OOB_DATA */
    HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_PARAM_LEN,       /* 0x0058 HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL */
    HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_PARAM_LEN,         /* 0x0059 HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL */
    0,                                                        /* 0x005A -- not used */
    0,                                                        /* 0x005B -- not used */
    0,                                                        /* 0x005C -- not used */
    0,                                                        /* 0x005D -- not used */
    0,                                                        /* 0x005E -- not used */
    HCI_ENHANCED_FLUSH_PARAM_LEN,                             /* 0x005F HCI_ENHANCED_FLUSH */
    HCI_SEND_KEYPRESS_NOTIFICATION_PARAM_LEN,                 /* 0x0060 HCI_SEND_KEYPRESS_NOTIFICATION */
    0,                                                        /* 0x0061 -- not used */
    0,                                                        /* 0x0062 -- not used */
    0,                                                        /* 0x0063 -- not used */
    0,                                                        /* 0x0064 -- not used */
    0,                                                        /* 0x0065 -- not used */
    0,                                                        /* 0x0066 -- not used */
    0,                                                        /* 0x0067 -- not used */
    HCI_READ_ENH_TX_POWER_LEVEL_PARAM_LEN,                    /* 0x0068 HCI_READ_ENH_TX_POWER_LEVEL */
    0,                                                        /* 0x0069 -- not used */
    0,                                                        /* 0x006A -- not used */
    0,                                                        /* 0x006B -- not used */
    HCI_READ_LE_HOST_SUPPORT_PARAM_LEN,                       /* 0x006C HCI_READ_LE_HOST_SUPPORT */
    HCI_WRITE_LE_HOST_SUPPORT_PARAM_LEN,                      /* 0x006D HCI_WRITE_LE_HOST_SUPPORT */
};

static const CsrUint8 csrBtHcishimInfoLengths[HCI_MAX_INFO_OCF & HCI_OPCODE_MASK] =
{
    0,                                                        /* 0x0000 -- not used */
    HCI_READ_LOCAL_VER_INFO_PARAM_LEN,                        /* 0x0001 HCI_READ_LOCAL_VER_INFO */
    HCI_READ_LOCAL_SUPP_COMMANDS_PARAM_LEN,                   /* 0x0002 HCI_READ_LOCAL_SUPP_COMMANDS */
    HCI_READ_LOCAL_SUPP_FEATURES_PARAM_LEN,                   /* 0x0003 HCI_READ_LOCAL_SUPP_FEATURES */
    HCI_READ_LOCAL_EXT_FEATURES_PARAM_LEN,                    /* 0x0004 HCI_READ_LOCAL_EXT_FEATURES */
    HCI_READ_BUFFER_SIZE_PARAM_LEN,                           /* 0x0005 HCI_READ_BUFFER_SIZE */
    0,                                                        /* 0x0006 -- not used */
    HCI_READ_COUNTRY_CODE_PARAM_LEN,                          /* 0x0007 HCI_READ_COUNTRY_CODE */
    0,                                                        /* 0x0008 -- not used */
    HCI_READ_BD_ADDR_PARAM_LEN                                /* 0x0009 HCI_READ_BD_ADDR */
};

static const CsrUint8 csrBtHcishimStatusLengths[HCI_MAX_STATUS_OCF & HCI_OPCODE_MASK] =
{
    0,                                                        /* 0x0000 -- not used */
    HCI_READ_FAILED_CONTACT_COUNT_PARAM_LEN,                  /* 0x0001 HCI_READ_FAILED_CONTACT_COUNT */
    HCI_RESET_FAILED_CONTACT_COUNT_PARAM_LEN,                 /* 0x0002 HCI_RESET_FAILED_CONTACT_COUNT */
    HCI_GET_LINK_QUALITY_PARAM_LEN,                           /* 0x0003 HCI_GET_LINK_QUALITY */
    0,                                                        /* 0x0004 -- not used */
    HCI_READ_RSSI_PARAM_LEN,                                  /* 0x0005 HCI_READ_RSSI */
    HCI_READ_AFH_CHANNEL_MAP_PARAM_LEN,                       /* 0x0006 HCI_READ_AFH_CHANNEL_MAP */
    HCI_READ_CLOCK_PARAM_LEN,                                 /* 0x0007 HCI_READ_CLOCK */
    HCI_READ_ENCRYPTION_KEY_SIZE_PARAM_LEN                    /* 0x0008 HCI_READ_ENCRYPTION_KEY_SIZE */
};

static const CsrUint8 csrBtHcishimTestLengths[HCI_MAX_TEST_OCF & HCI_OPCODE_MASK] =
{
    0,                                                        /* 0x0000 -- not used */
    HCI_READ_LOOPBACK_MODE_PARAM_LEN,                         /* 0x0001 HCI_READ_LOOPBACK_MODE */
    HCI_WRITE_LOOPBACK_MODE_PARAM_LEN,                        /* 0x0002 HCI_WRITE_LOOPBACK_MODE */
    HCI_ENABLE_DUT_MODE_PARAM_LEN,                            /* 0x0003 HCI_ENABLE_DUT_MODE */
    HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE_PARAM_LEN             /* 0x0004 HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE */
};

#ifdef CSR_BT_LE_ENABLE
static const CsrUint8 csrBtHcishimUlpLengths[HCI_MAX_ULP_OCF & HCI_OPCODE_MASK] =
{
    0,                                                        /* 0x0000 -- not used */
    HCI_ULP_SET_EVENT_MASK_PARAM_LEN,                         /* 0x0001 HCI_ULP_SET_EVENT_MASK */
    HCI_ULP_READ_BUFFER_SIZE_PARAM_LEN,                       /* 0x0002 HCI_ULP_READ_BUFFER_SIZE */
    HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_PARAM_LEN,          /* 0x0003 HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES */
    0,                                                        /* 0x0004 HCI_ULP_SET_LOCAL_USED_FEATURES -- not used  */
    HCI_ULP_SET_RANDOM_ADDRESS_PARAM_LEN,                     /* 0x0005 HCI_ULP_SET_RANDOM_ADDRESS */
    HCI_ULP_SET_ADVERTISING_PARAMETERS_PARAM_LEN,             /* 0x0006 HCI_ULP_SET_ADVERTISING_PARAMETERS */
    HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_PARAM_LEN,      /* 0x0007 HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER */
    HCI_ULP_SET_ADVERTISING_DATA_PARAM_LEN,                   /* 0x0008 HCI_ULP_SET_ADVERTISING_DATA */
    HCI_ULP_SET_SCAN_RESPONSE_DATA_PARAM_LEN,                 /* 0x0009 HCI_ULP_SET_SCAN_RESPONSE_DATA */ 
    HCI_ULP_SET_ADVERTISE_ENABLE_PARAM_LEN,                   /* 0x000A HCI_ULP_SET_ADVERTISE_ENABLE */
    HCI_ULP_SET_SCAN_PARAMETERS_PARAM_LEN,                    /* 0x000B HCI_ULP_SET_SCAN_PARAMETERS */
    HCI_ULP_SET_SCAN_ENABLE_PARAM_LEN,                        /* 0x000C HCI_ULP_SET_SCAN_ENABLE */
    HCI_ULP_CREATE_CONNECTION_PARAM_LEN,                      /* 0x000D HCI_ULP_CREATE_CONNECTION */ 
    HCI_ULP_CREATE_CONNECTION_CANCEL_PARAM_LEN,               /* 0x000E HCI_ULP_CREATE_CONNECTION_CANCEL */
    HCI_ULP_READ_WHITE_LIST_SIZE_PARAM_LEN,                   /* 0x000F HCI_ULP_READ_WHITE_LIST_SIZE */
    HCI_ULP_CLEAR_WHITE_LIST_PARAM_LEN,                       /* 0x0010 HCI_ULP_CLEAR_WHITE_LIST */
    HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_PARAM_LEN,               /* 0x0011 HCI_ULP_ADD_DEVICE_TO_WHITE_LIST */
    HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LIST_PARAM_LEN,          /* 0x0012 HCI_ULP_REMOVE_DEVICE_FROM_WHITE_LISTLIST */
    HCI_ULP_CONNECTION_UPDATE_PARAM_LEN,                      /* 0x0013 HCI_ULP_CONNECTION_UPDATELIST */
    HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_PARAM_LEN,        /* 0x0014 HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATIONLIST */
    HCI_ULP_READ_CHANNEL_MAP_PARAM_LEN,                       /* 0x0015 HCI_ULP_READ_CHANNEL_MAPLIST */
    HCI_ULP_READ_REMOTE_USED_FEATURES_PARAM_LEN,              /* 0x0016 HCI_ULP_READ_REMOTE_USED_FEATURES */
    HCI_ULP_ENCRYPT_PARAM_LEN,                                /* 0x0017 HCI_ULP_ENCRYPT */
    HCI_ULP_RAND_PARAM_LEN,                                   /* 0x0018 HCI_ULP_RAND */
    HCI_ULP_START_ENCRYPTION_PARAM_LEN,                       /* 0x0019 HCI_ULP_START_ENCRYPTION */
    HCI_ULP_LONG_TERM_KEY_REQUESTED_REPLY_PARAM_LEN,          /* 0x001A HCI_ULP_LONG_TERM_KEY_REQUESTED_REPLY */
    HCI_ULP_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_PARAM_LEN, /* 0x001B HCI_ULP_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY */
    HCI_ULP_READ_SUPPORTED_STATES_PARAM_LEN,                  /* 0x001C HCI_ULP_READ_SUPPORTED_STATES */
    HCI_ULP_RECEIVER_TEST_PARAM_LEN,                          /* 0x001D HCI_ULP_RECEIVER_TEST */
    HCI_ULP_TRANSMITTER_TEST_PARAM_LEN,                       /* 0x001E HCI_ULP_TRANSMITTER_TEST */
    HCI_ULP_TEST_END_PARAM_LEN,                               /* 0x001F HCI_ULP_TEST_END */
};
#endif

void CsrBtHcishimInsertLengthByOpcode(HCI_UPRIM_T *pv_hci_uprim)
{
    HCI_UPRIM_T *p_prim = (HCI_UPRIM_T *) pv_hci_uprim;
    hci_op_code_t opcode;

    opcode = p_prim->op_code & HCI_OPCODE_MASK;
    switch (p_prim->op_code & HCI_OPCODE_GROUP_MASK)
    {
        case HCI_LINK:
            p_prim->hci_cmd.length = csrBtHcishimLinkLengths[opcode];
            break;

        case HCI_POLICY:
            p_prim->hci_cmd.length = csrBtHcishimPolicyLengths[opcode];
            break;

        case HCI_HOST_BB:
            p_prim->hci_cmd.length = csrBtHcishimHostBbLengths[opcode];
            break;

        case HCI_INFO:
            p_prim->hci_cmd.length = csrBtHcishimInfoLengths[opcode];
            break;

        case HCI_STATUS:
            p_prim->hci_cmd.length = csrBtHcishimStatusLengths[opcode];
            break;

        case HCI_TEST:
            p_prim->hci_cmd.length = csrBtHcishimTestLengths[opcode];
            break;

#ifdef CSR_BT_LE_ENABLE
        case HCI_ULP:
            p_prim->hci_cmd.length = csrBtHcishimUlpLengths[opcode];
            break;
#endif

        default:
            /* This is probably a variable-length command, or
             * something went wrong */
            break;
    }

    /* Some commands have variable length, handle these separately */
    switch (p_prim->op_code)
    {
        case HCI_SET_EVENT_FILTER:
            if (p_prim->hci_set_event_filter.filter_type == 0)
            {
                p_prim->hci_cmd.length = 1;
            }
            else
            {
                if (p_prim->hci_set_event_filter.filter_condition_type == 0)
                {
                    p_prim->hci_cmd.length = 2;
                }

                if (p_prim->hci_set_event_filter.filter_type == 2)
                {
                    p_prim->hci_cmd.length++;
                }
            } /* options for length 1, 2, 3, 8(default), 9 */
            break;

#ifndef CSR_BT_EXCLUDE_HCI_WRITE_CURRENT_IAC_LAP
        case HCI_WRITE_CURRENT_IAC_LAP:
            p_prim->hci_cmd.length = 1 + 3 *
                p_prim->hci_write_curr_iac_lap.num_current_iac;
            break;
#endif
    }
}

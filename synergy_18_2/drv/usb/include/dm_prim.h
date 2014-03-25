/*******************************************************************************
 
    (C) COPYRIGHT Cambridge Consultants Ltd 1998























































*******************************************************************************/
#ifndef BLUESTACK__DM_PRIM_H     /* Once is enough */
#define BLUESTACK__DM_PRIM_H





#ifdef BUILD_FOR_HOST



#endif










#include "hci.h"

#ifdef __cplusplus
extern "C" {
#endif

#define Sink uint16_t

/* search_string="dm_prim_t" */
/* conversion_rule="ADD_UNDERSCORE_AND_UPPERCASE_T" */

/*============================================================================*
                    Header files on which this one depends
 *============================================================================*/

/* None */

/*============================================================================*
                       Interface #defined parameters
 *============================================================================*/

#define DEFAULT_ACL_PACKET_TYPE	(HCI_PKT_DM1 | HCI_PKT_DH1 | HCI_PKT_DM3 | HCI_PKT_DH3 | HCI_PKT_DM5 | HCI_PKT_DH5)
#define DEFAULT_SCO_PACKET_TYPE	(HCI_PKT_HV3)

#define DM_PRIM_BASE        0x0000  
#define DM_ACL_PRIM         0x2000
#define DM_SCO_PRIM         0x2400
#define DM_PRIV_PRIM        0x2800      /* Commands specific to this Device Manager */
#define DM_SM_PRIM          0x2C00
#define DM_SM_PRIM_UP       0x2C11
#define DM_SM_PRIM_DOWN2    0x2E10
#define DM_EN_PRIM          0x3000

#ifdef INSTALL_DYNAMIC_TOKEN_HANDLING
#define DM_MC_PRIM          0x4000
#endif 

#define DM_SYNC_PRIM        0x7C00
#define DM_LC_PRIM          HCI_LINK
#define DM_LP_PRIM          HCI_POLICY
#define DM_BB_PRIM          HCI_HOST_BB
#define DM_INF_PRIM         HCI_INFO
#define DM_STATUS_PRIM      HCI_STATUS
#define DM_TEST_PRIM        HCI_TEST

#define DM_1P2_PRIM	    0x7800
#define DM_2P1_PRIM         0x7840
#define DM_OGF_MASK         0xFC00
#define DM_OPCODE_MASK      0x03FF
#define DM_SM_OPCODE_MASK   0x01FF      /* Recombines downstream prim blocks */


/* First add your primitive here... */
typedef enum dm_prim_tag
{
    /*-------------------------------------------------------------
      Registration Primitives (applicable to AM Interface only, SCO and
      ACL registration primitives are supplied by the SCO and ACL
      interfaces)
      ---------------------------------------------------------------*/
    ENUM_DM_AM_REGISTER_REQ= DM_PRIM_BASE,
    ENUM_DM_AM_REGISTER_CFM,
    ENUM_DM_SET_BT_VERSION_REQ,
    ENUM_DM_SET_BT_VERSION_CFM,

    /* DM_AM_REGISTER_REQ for wrong API.
      
       This is just to help catch an application sending DM_AM_REGISTER_REQ
       using the wrong API. If you do this from the VM (which should be
       quite difficult) then the firmware will panic. If you do it via an
       external physical interface (setting the top bit of the phandle) then
       the firmware will reply with INVALID_PHANDLE.*/
    ENUM_DM_AM_REGISTER_WRONG_API_REQ = 0x300,
    ENUM_DM_AM_REGISTER_WRONG_API_CFM = 0x340,

    /*-------------------------------------------------------------
      ACL Connection Interface Primitives - for L2CAP use only
      ---------------------------------------------------------------*/
    ENUM_SEP_DM_ACL_FIRST= DM_ACL_PRIM,                 /* not a primitive */
    ENUM_DM_ACL_REGISTER_REQ,
    ENUM_DM_ACL_REGISTER_CFM,
    ENUM_DM_ACL_CONNECT_REQ,
    ENUM_DM_ACL_CONNECT_CFM,
    ENUM_DM_ACL_CONNECT_IND,
    ENUM_DM_ACL_DISCONNECT_REQ,
    ENUM_DM_ACL_DISCONNECT_IND,
    ENUM_DM_ACL_BUFFER_SIZE_IND,
    ENUM_DM_ACL_DATA_SENT_IND,
    ENUM_OBSOLETE_DM_CONNECTIONLESS_CH_REG_REQ,
    ENUM_SEP_DM_ACL_LAST,                               /* not a primitive */

    /*-------------------------------------------------------------
      SCO Connection Interface Primitives
      ---------------------------------------------------------------*/
    ENUM_SEP_DM_SCO_FIRST= DM_SCO_PRIM,                 /* not a primitive */
    ENUM_OBSOLETE_DM_SCO_INCOMING_REGISTER_REQ,
    ENUM_OBSOLETE_DM_SCO_INCOMING_UNREGISTER_REQ,
    ENUM_OBSOLETE_DM_SCO_CONNECT_REQ,
    ENUM_OBSOLETE_DM_SCO_CONNECT_CFM,
    ENUM_OBSOLETE_DM_SCO_CONNECT_IND,
    ENUM_OBSOLETE_DM_SCO_CONNECT_RES,
    ENUM_OBSOLETE_DM_SCO_DISCONNECT_REQ,
    ENUM_OBSOLETE_DM_SCO_DISCONNECT_IND,
    ENUM_OBSOLETE_DM_SCO_BUFFER_SIZE_IND,
    ENUM_OBSOLETE_DM_SCO_DATA_SENT_IND,
    ENUM_SEP_DM_SCO_LAST,                               /* not a primitive */

    /*-------------------------------------------------------------
      Security Management Primitives

      Put new upstream primitives just before ENUM_SEP_DM_SM_UP_LAST
      Put new downstream primitives just before ENUM_SEP_DM_SM_DOWN2_LAST
      ---------------------------------------------------------------*/
    ENUM_SEP_DM_SM_DOWN_FIRST= DM_SM_PRIM,              /* not a primitive */
    ENUM_DM_SM_SSP_LINK_KEY_REQUEST_RES = DM_SM_PRIM,
    ENUM_DM_SM_SET_DEFAULT_SECURITY_REQ,
    ENUM_DM_SM_REGISTER_REQ,
    ENUM_DM_SM_UNREGISTER_REQ,
    ENUM_DM_SM_REGISTER_OUTGOING_REQ,
    ENUM_DM_SM_UNREGISTER_OUTGOING_REQ,
    ENUM_DM_SM_ACCESS_REQ,
    ENUM_DM_SM_SET_SEC_MODE_REQ,
    ENUM_DM_SM_ADD_DEVICE_REQ,
    ENUM_DM_SM_REMOVE_DEVICE_REQ,
    ENUM_DM_SM_LINK_KEY_REQUEST_RES,
    ENUM_DM_SM_PIN_REQUEST_RES,
    ENUM_DM_SM_AUTHORISE_RES,
    ENUM_DM_SM_AUTHENTICATE_REQ,
    ENUM_DM_SM_ENCRYPT_REQ,
    ENUM_DM_SM_L2CAP_REGISTER_REQ,
    /*
     * Don't add more primitives here. This block is now full.
     * New downstream security manager primitives should be added to
     * the second downstream block, just before ENUM_SEP_DM_SM_DOWN2_LAST.
     * New upstream primitives should be added to the upstream block,
     * just before ENUM_SEP_DM_SM_UP_LAST.
     */
    ENUM_SEP_DM_SM_DOWN_LAST,                           /* not a primitive */

    ENUM_SEP_DM_SM_UP_FIRST = DM_SM_PRIM_UP,            /* not a primitive */
    ENUM_DM_SM_ACCESS_CFM,
    ENUM_DM_SM_SET_SEC_MODE_CFM,
    ENUM_DM_SM_ADD_DEVICE_CFM,
    ENUM_DM_SM_REMOVE_DEVICE_CFM,
    ENUM_DM_SM_LINK_KEY_REQUEST_IND,
    ENUM_DM_SM_PIN_REQUEST_IND,
    ENUM_DM_SM_LINK_KEY_IND,
    ENUM_DM_SM_AUTHORISE_IND,
    ENUM_DM_SM_AUTHENTICATE_CFM,
    ENUM_DM_SM_ENCRYPT_CFM,
    ENUM_DM_SM_ENCRYPTION_CHANGE,
    ENUM_DM_SM_L2CAP_CLRX_ENABLE_IND,
    ENUM_DM_SM_SEC_MODE_CONFIG_CFM,
    ENUM_DM_SM_READ_LOCAL_OOB_DATA_CFM,
    ENUM_DM_SM_SSP_ADD_DEVICE_CFM,
    ENUM_DM_SM_IO_CAPABILITY_RESPONSE_IND,
    ENUM_DM_SM_IO_CAPABILITY_REQUEST_IND,
    ENUM_DM_SM_SIMPLE_PAIRING_COMPLETE_IND,
    ENUM_DM_SM_USER_CONFIRMATION_REQUEST_IND,
    ENUM_DM_SM_USER_PASSKEY_REQUEST_IND,
    ENUM_DM_SM_SSP_LINK_KEY_REQUEST_IND,
    ENUM_DM_SM_USER_PASSKEY_NOTIFICATION_IND,
    ENUM_DM_SM_KEYPRESS_NOTIFICATION_IND,
    ENUM_DM_SM_BONDING_CFM,
    ENUM_DM_SM_READ_DEVICE_CFM,
    ENUM_DM_SM_ACCESS_IND,
    ENUM_DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND,
    ENUM_SEP_DM_SM_UP_LAST,                             /* not a primitive */

    /* This is a trick so that both primitives can be directed to the same
     * function pointer. (ENUM_DM_SM_ADD_DEVICE_REQ = 0x2C08 and 0x0200 is
     * masked out.) We save a couple of words of const space.
     */
    ENUM_DM_SM_SSP_ADD_DEVICE_REQ = 0x2E08,

    ENUM_SEP_DM_SM_DOWN2_FIRST = DM_SM_PRIM_DOWN2,      /* not a primitive */
    ENUM_DM_SM_SEC_MODE_CONFIG_REQ = DM_SM_PRIM_DOWN2,
    ENUM_DM_SM_IO_CAPABILITY_REQUEST_RES,
    ENUM_DM_SM_BONDING_REQ,
    ENUM_DM_SM_BONDING_CANCEL_REQ,
    ENUM_DM_SM_READ_DEVICE_REQ,
    ENUM_DM_SM_IO_CAPABILITY_REQUEST_NEG_RES,
    ENUM_SEP_DM_SM_DOWN2_LAST,                          /* not a primitive */

    /**************************************************************************
        Device Manager private primitives, not HCI commands
     **************************************************************************/
    ENUM_SEP_DM_PRIV_DOWN_FIRST = DM_PRIV_PRIM,         /* not a primitive */
    ENUM_DM_WRITE_CACHED_PAGE_MODE_REQ,
    ENUM_DM_WRITE_CACHED_CLOCK_OFFSET_REQ,
    ENUM_DM_CLEAR_PARAM_CACHE_REQ,
    ENUM_DM_SET_DEFAULT_LINK_POLICY,
    ENUM_DM_ACL_OPEN_REQ,
    ENUM_DM_ACL_CLOSE_REQ,
    ENUM_SEP_DM_PRIV_DOWN_LAST,                         /* not a primitive */

    ENUM_DM_WRITE_CACHED_PAGE_MODE_CFM,
    ENUM_DM_WRITE_CACHED_CLOCK_OFFSET_CFM,
    ENUM_DM_CLEAR_PARAM_CACHE_CFM,
    ENUM_DM_HC_TO_HOST_FLOW_CONTROL,
    ENUM_DM_ACL_OPEN_CFM,
    ENUM_DM_ACL_OPENED_IND,
    ENUM_DM_ACL_CLOSED_IND,
    ENUM_SEP_DM_PRIV_UP_LAST,                           /* not a primitive */

    /*-------------------------------------------------------------
      HCI API primitives
      ---------------------------------------------------------------*/

    /******************************************************************************
      Link Control 
      *****************************************************************************/

    /*-------------------------------------------------------------
      Downstream primitives
      ---------------------------------------------------------------*/
    ENUM_SEP_DM_LC_DOWN_FIRST = HCI_LINK,               /* not a primitive */

	ENUM_DM_HCI_INQUIRY = HCI_INQUIRY,
	ENUM_DM_HCI_INQUIRY_CANCEL = HCI_INQUIRY_CANCEL,
	ENUM_DM_HCI_PERIODIC_INQUIRY = HCI_PERIODIC_INQUIRY_MODE,
	ENUM_DM_HCI_EXIT_PERIODIC_INQUIRY = HCI_EXIT_PERIODIC_INQUIRY_MODE,
	ENUM_DM_HCI_CHANGE_PACKET_TYPE = HCI_CHANGE_CONN_PKT_TYPE,
	ENUM_DM_HCI_CHANGE_LINK_KEY = HCI_CHANGE_CONN_LINK_KEY,
	ENUM_DM_HCI_MASTER_LINK_KEY = HCI_MASTER_LINK_KEY,
	ENUM_DM_HCI_REMOTE_NAME_REQUEST = HCI_REMOTE_NAME_REQ,
	ENUM_DM_HCI_READ_REMOTE_FEATURES = HCI_READ_REMOTE_SUPP_FEATURES,
	ENUM_DM_HCI_READ_REMOTE_VERSION = HCI_READ_REMOTE_VER_INFO,
	ENUM_DM_HCI_READ_CLOCK_OFFSET = HCI_READ_CLOCK_OFFSET,

    ENUM_SEP_DM_LC_DOWN_LAST = HCI_LINK | HCI_MAX_LINK_OCF_V1_1,        /* not a primitive */

    /*-------------------------------------------------------------
      Upstream primitives
      ---------------------------------------------------------------*/
    ENUM_DM_HCI_INQUIRY_CANCEL_COMPLETE,
    ENUM_DM_HCI_PERIODIC_INQUIRY_COMPLETE,
    ENUM_DM_HCI_EXIT_PERIODIC_INQUIRY_COMPLETE,
    ENUM_DM_HCI_INQUIRY_RESULT,
    ENUM_DM_HCI_INQUIRY_COMPLETE,
    ENUM_DM_HCI_LINK_KEY_CHANGE_COMPLETE,
    ENUM_DM_HCI_RETURN_LINK_KEYS,
    ENUM_DM_HCI_MASTER_LINK_KEY_COMPLETE,
    ENUM_DM_HCI_REMOTE_NAME_COMPLETE,
    ENUM_DM_HCI_READ_REMOTE_FEATURES_COMPLETE,
    ENUM_DM_HCI_READ_REMOTE_VERSION_COMPLETE,
    ENUM_DM_HCI_READ_CLOCK_OFFSET_COMPLETE,
    ENUM_SEP_DM_LC_UP_LAST,                            /* not a primitive */

    /******************************************************************************
      Link Policy 
      *****************************************************************************/
    /*-------------------------------------------------------------
      Downstream primitives
      ---------------------------------------------------------------*/
    ENUM_SEP_DM_LP_DOWN_FIRST = HCI_POLICY,    /* not a primitive */

	ENUM_DM_HCI_HOLD_MODE = HCI_HOLD_MODE,
	ENUM_DM_HCI_SNIFF_MODE = HCI_SNIFF_MODE,
	ENUM_DM_HCI_EXIT_SNIFF_MODE = HCI_EXIT_SNIFF_MODE,
	ENUM_DM_HCI_PARK_MODE = HCI_PARK_MODE,
	ENUM_DM_HCI_EXIT_PARK_MODE = HCI_EXIT_PARK_MODE,
	ENUM_DM_HCI_QOS_SETUP_REQ = HCI_QOS_SETUP,
	ENUM_DM_HCI_ROLE_DISCOVERY = HCI_ROLE_DISCOVERY,
	ENUM_DM_HCI_SWITCH_ROLE = HCI_SWITCH_ROLE,
	ENUM_DM_HCI_READ_LP_SETTINGS = HCI_READ_LINK_POLICY_SETTINGS,
	ENUM_DM_HCI_WRITE_LP_SETTINGS = HCI_WRITE_LINK_POLICY_SETTINGS,
  
    ENUM_SEP_DM_LP_DOWN_LAST = HCI_POLICY | HCI_MAX_POLICY_OCF_V1_1,      /* not a primitive */

    /*-------------------------------------------------------------
      Upstream primitives
      ---------------------------------------------------------------*/
    ENUM_DM_HCI_MODE_CHANGE_EVENT,
    ENUM_DM_HCI_QOS_SETUP_CFM,
    ENUM_DM_HCI_QOS_VIOLATION_IND,
    ENUM_DM_HCI_ROLE_DISCOVERY_COMPLETE,
    ENUM_DM_HCI_SWITCH_ROLE_COMPLETE,
    ENUM_DM_HCI_READ_LP_SETTINGS_COMPLETE,
    ENUM_DM_HCI_WRITE_LP_SETTINGS_COMPLETE,
    ENUM_SEP_DM_LP_UP_LAST,                             /* not a primitive */

    /******************************************************************************
      Host Controller and Baseband
      *****************************************************************************/
    /*-------------------------------------------------------------
      Downstream primitives
      ---------------------------------------------------------------*/
    ENUM_SEP_DM_BB_DOWN_FIRST = HCI_HOST_BB,            /* not a primitive */

	ENUM_DM_HCI_SET_EVENT_MASK = HCI_SET_EVENT_MASK,
	ENUM_DM_HCI_RESET = HCI_RESET,
	ENUM_DM_HCI_SET_EVENT_FILTER = HCI_SET_EVENT_FILTER,
	ENUM_DM_HCI_FLUSH = HCI_FLUSH,
	ENUM_DM_HCI_READ_PIN_TYPE = HCI_READ_PIN_TYPE,
	ENUM_DM_HCI_WRITE_PIN_TYPE = HCI_WRITE_PIN_TYPE,
	ENUM_DM_HCI_CREATE_NEW_UNIT_KEY = HCI_CREATE_NEW_UNIT_KEY,
	ENUM_DM_HCI_READ_STORED_LINK_KEY = HCI_READ_STORED_LINK_KEY,
	ENUM_DM_HCI_WRITE_STORED_LINK_KEY = HCI_WRITE_STORED_LINK_KEY,
	ENUM_DM_HCI_DELETE_STORED_LINK_KEY = HCI_DELETE_STORED_LINK_KEY,
	ENUM_DM_HCI_CHANGE_LOCAL_NAME = HCI_CHANGE_LOCAL_NAME,
	ENUM_DM_HCI_READ_LOCAL_NAME	= HCI_READ_LOCAL_NAME,
	ENUM_DM_HCI_READ_CONN_ACCEPT_TO = HCI_READ_CONN_ACCEPT_TIMEOUT,
	ENUM_DM_HCI_WRITE_CONN_ACCEPT_TO = HCI_WRITE_CONN_ACCEPT_TIMEOUT,
	ENUM_DM_HCI_READ_PAGE_TO = HCI_READ_PAGE_TIMEOUT,
	ENUM_DM_HCI_WRITE_PAGE_TO = HCI_WRITE_PAGE_TIMEOUT,
	ENUM_DM_HCI_READ_SCAN_ENABLE = HCI_READ_SCAN_ENABLE,
	ENUM_DM_HCI_WRITE_SCAN_ENABLE = HCI_WRITE_SCAN_ENABLE,
	ENUM_DM_HCI_READ_PAGESCAN_ACTIVITY = HCI_READ_PAGESCAN_ACTIVITY,
	ENUM_DM_HCI_WRITE_PAGESCAN_ACTIVITY = HCI_WRITE_PAGESCAN_ACTIVITY,
	ENUM_DM_HCI_READ_INQUIRYSCAN_ACTIVITY = HCI_READ_INQUIRYSCAN_ACTIVITY,
	ENUM_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY = HCI_WRITE_INQUIRYSCAN_ACTIVITY,
	ENUM_DM_HCI_READ_AUTH_ENABLE = HCI_READ_AUTH_ENABLE,
	ENUM_DM_HCI_READ_ENCRYPTION_MODE = HCI_READ_ENC_MODE,
	ENUM_DM_HCI_READ_CLASS_OF_DEVICE = HCI_READ_CLASS_OF_DEVICE,
	ENUM_DM_HCI_WRITE_CLASS_OF_DEVICE = HCI_WRITE_CLASS_OF_DEVICE,
	ENUM_DM_HCI_READ_VOICE_SETTING = HCI_READ_VOICE_SETTING,
	ENUM_DM_HCI_WRITE_VOICE_SETTING = HCI_WRITE_VOICE_SETTING,
	ENUM_DM_HCI_READ_AUTO_FLUSH_TIMEOUT = HCI_READ_AUTO_FLUSH_TIMEOUT,
	ENUM_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT = HCI_WRITE_AUTO_FLUSH_TIMEOUT,
	ENUM_DM_HCI_READ_NUM_BCAST_TXS = HCI_READ_NUM_BCAST_RETXS,
	ENUM_DM_HCI_WRITE_NUM_BCAST_TXS = HCI_WRITE_NUM_BCAST_RETXS,
	ENUM_DM_HCI_READ_HOLD_MODE_ACTIVITY = HCI_READ_HOLD_MODE_ACTIVITY,
	ENUM_DM_HCI_WRITE_HOLD_MODE_ACTIVITY = HCI_WRITE_HOLD_MODE_ACTIVITY,
	ENUM_DM_HCI_READ_TX_POWER_LEVEL = HCI_READ_TX_POWER_LEVEL,
	ENUM_DM_HCI_READ_SCO_FLOW_CONTROL_ENABLE = HCI_READ_SCO_FLOW_CON_ENABLE,
	ENUM_DM_HCI_WRITE_SCO_FLOW_CONTROL_ENABLE = HCI_WRITE_SCO_FLOW_CON_ENABLE,
	ENUM_DM_HCI_SET_HC_TO_HOST_FLOW = HCI_SET_HCTOHOST_FLOW_CONTROL,
	ENUM_DM_HCI_HOST_NUM_COMPLETED_PACKETS = HCI_HOST_NUM_COMPLETED_PACKETS,
	ENUM_DM_HCI_READ_LINK_SUPERV_TIMEOUT = HCI_READ_LINK_SUPERV_TIMEOUT,
	ENUM_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT = HCI_WRITE_LINK_SUPERV_TIMEOUT,
	ENUM_DM_HCI_READ_NUM_SUPPORTED_IAC = HCI_READ_NUM_SUPPORTED_IAC,
	ENUM_DM_HCI_READ_CURRENT_IAC_LAP = HCI_READ_CURRENT_IAC_LAP,
	ENUM_DM_HCI_WRITE_CURRENT_IAC_LAP = HCI_WRITE_CURRENT_IAC_LAP,
	ENUM_DM_HCI_READ_PAGESCAN_PERIOD_MODE = HCI_READ_PAGESCAN_PERIOD_MODE,
	ENUM_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE = HCI_WRITE_PAGESCAN_PERIOD_MODE,
	ENUM_DM_HCI_READ_PAGESCAN_MODE = HCI_READ_PAGESCAN_MODE,
	ENUM_DM_HCI_WRITE_PAGESCAN_MODE = HCI_WRITE_PAGESCAN_MODE,

    ENUM_SEP_DM_BB_DOWN_LAST = HCI_HOST_BB | HCI_MAX_HOST_BB_OCF_V1_1,     /* not a primitive */

    /*-------------------------------------------------------------
      Upstream primitives
      ---------------------------------------------------------------*/
    ENUM_DM_HCI_SET_EVENT_MASK_COMPLETE,
    ENUM_DM_HCI_RESET_COMPLETE,
    ENUM_DM_HCI_SET_EVENT_FILTER_COMPLETE,
    ENUM_DM_HCI_FLUSH_COMPLETE,
    ENUM_DM_HCI_READ_PIN_TYPE_COMPLETE,
    ENUM_DM_HCI_WRITE_PIN_TYPE_COMPLETE,
    ENUM_DM_HCI_CREATE_NEW_UNIT_KEY_COMPLETE,
    ENUM_DM_HCI_READ_STORED_LINK_KEY_COMPLETE,
    ENUM_DM_HCI_WRITE_STORED_LINK_KEY_COMPLETE,
    ENUM_DM_HCI_DELETE_STORED_LINK_KEY_COMPLETE,
    ENUM_DM_HCI_CHANGE_LOCAL_NAME_COMPLETE,
    ENUM_DM_HCI_READ_LOCAL_NAME_COMPLETE,
    ENUM_DM_HCI_READ_CONN_ACCEPT_TO_COMPLETE,
    ENUM_DM_HCI_WRITE_CONN_ACCEPT_TO_COMPLETE,
    ENUM_DM_HCI_READ_PAGE_TO_COMPLETE,
    ENUM_DM_HCI_WRITE_PAGE_TO_COMPLETE,
    ENUM_DM_HCI_READ_SCAN_ENABLE_COMPLETE,
    ENUM_DM_HCI_WRITE_SCAN_ENABLE_COMPLETE,
    ENUM_DM_HCI_READ_PAGESCAN_ACTIVITY_COMPLETE,
    ENUM_DM_HCI_WRITE_PAGESCAN_ACTIVITY_COMPLETE,
    ENUM_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_COMPLETE,
    ENUM_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_COMPLETE,
    ENUM_DM_HCI_READ_AUTH_ENABLE_COMPLETE,
    ENUM_DM_HCI_READ_ENCRYPTION_MODE_COMPLETE,
    ENUM_DM_HCI_READ_CLASS_OF_DEVICE_COMPLETE,
    ENUM_DM_HCI_WRITE_CLASS_OF_DEVICE_COMPLETE,
    ENUM_DM_HCI_READ_VOICE_SETTING_COMPLETE,
    ENUM_DM_HCI_WRITE_VOICE_SETTING_COMPLETE,
    ENUM_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_COMPLETE,
    ENUM_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_COMPLETE,
    ENUM_DM_HCI_READ_NUM_BCAST_TXS_COMPLETE,
    ENUM_DM_HCI_WRITE_NUM_BCAST_TXS_COMPLETE,
    ENUM_DM_HCI_READ_HOLD_MODE_ACTIVITY_COMPLETE,
    ENUM_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_COMPLETE,
    ENUM_DM_HCI_READ_TX_POWER_LEVEL_COMPLETE,
    ENUM_DM_HCI_SET_HC_TO_HOST_FLOW_COMPLETE,
    ENUM_DM_HCI_HOST_NUM_COMPLETED_PACKETS_COMPLETE,
    ENUM_DM_HCI_READ_LINK_SUPERV_TIMEOUT_COMPLETE,   
    ENUM_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_COMPLETE,  
    ENUM_DM_HCI_READ_NUM_SUPPORTED_IAC_COMPLETE,
    ENUM_DM_HCI_READ_CURRENT_IAC_LAP_COMPLETE,    
    ENUM_DM_HCI_WRITE_CURRENT_IAC_LAP_COMPLETE,      
    ENUM_DM_HCI_READ_PAGESCAN_PERIOD_MODE_COMPLETE,
    ENUM_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_COMPLETE, 
    ENUM_DM_HCI_READ_PAGESCAN_MODE_COMPLETE, 
    ENUM_DM_HCI_WRITE_PAGESCAN_MODE_COMPLETE,        
    ENUM_DM_HCI_HARDWARE_ERROR,
    ENUM_DM_HCI_DATA_BUFFER_OVERFLOW,
    ENUM_DM_HCI_MAX_SLOTS_CHANGE,
    ENUM_SEP_DM_BB_UP_LAST,                            /* not a primitive */

    /******************************************************************************
      Informational parameters
      *****************************************************************************/
    /*-------------------------------------------------------------
      Downstream primitives
      ---------------------------------------------------------------*/
    ENUM_SEP_DM_INF_DOWN_FIRST = HCI_INFO,              /* not a primitive */

	ENUM_DM_HCI_READ_LOCAL_VERSION = HCI_READ_LOCAL_VER_INFO,
	ENUM_DM_HCI_READ_LOCAL_FEATURES = HCI_READ_LOCAL_SUPP_FEATURES,
	ENUM_DM_HCI_READ_COUNTRY_CODE = HCI_READ_COUNTRY_CODE,
	ENUM_DM_HCI_READ_BD_ADDR = HCI_READ_BD_ADDR,

	ENUM_SEP_DM_INF_DOWN_LAST = HCI_INFO | HCI_MAX_INFO_OCF_V1_1,       /* not a primitive */

    /*-------------------------------------------------------------
      Upstream primitives
      ---------------------------------------------------------------*/
    ENUM_DM_HCI_READ_LOCAL_VERSION_COMPLETE,
    ENUM_DM_HCI_READ_LOCAL_FEATURES_COMPLETE,
    ENUM_DM_HCI_READ_COUNTRY_CODE_COMPLETE,
    ENUM_DM_HCI_READ_BD_ADDR_COMPLETE,
    ENUM_SEP_DM_INF_UP_LAST,                            /* not a primitive */

    /******************************************************************************
      Status parameters
      *****************************************************************************/
    /*-------------------------------------------------------------
      Downstream primitives
      ---------------------------------------------------------------*/
    ENUM_SEP_DM_STATUS_DOWN_FIRST = HCI_STATUS,         /* not a primitive */

	ENUM_DM_HCI_FAILED_CONTACT_COUNTER = HCI_READ_FAILED_CONTACT_COUNT,
	ENUM_DM_HCI_RESET_CONTACT_COUNTER = HCI_RESET_FAILED_CONTACT_COUNT,
	ENUM_DM_HCI_GET_LINK_QUALITY = HCI_GET_LINK_QUALITY,
	ENUM_DM_HCI_READ_RSSI = HCI_READ_RSSI,
 
    ENUM_SEP_DM_STATUS_DOWN_LAST = HCI_STATUS | HCI_MAX_STATUS_OCF_V1_1,  /* not a primitive */

    /*-------------------------------------------------------------
      Upstream primitives
      ---------------------------------------------------------------*/
    ENUM_DM_HCI_FAILED_CONTACT_COUNTER_COMPLETE,
    ENUM_DM_HCI_RESET_CONTACT_COUNTER_COMPLETE,
    ENUM_DM_HCI_GET_LINK_QUALITY_COMPLETE,
    ENUM_DM_HCI_READ_RSSI_COMPLETE,
    ENUM_SEP_DM_STATUS_UP_LAST,                         /* not a primitive */

    /******************************************************************************
      Testing primitives
      *****************************************************************************/
    /*-------------------------------------------------------------
      Downstream primitives
      ---------------------------------------------------------------*/
    ENUM_SEP_DM_TEST_DOWN_FIRST = HCI_TEST,             /* not a primitive */

	ENUM_OBSOLETE_DM_HCI_READ_LOOPBACK_MODE = HCI_READ_LOOPBACK_MODE,
	ENUM_OBSOLETE_DM_HCI_WRITE_LOOPBACK_MODE = HCI_WRITE_LOOPBACK_MODE,
	ENUM_DM_HCI_ENABLE_DEVICE_UT_MODE = HCI_ENABLE_DUT_MODE,

    ENUM_SEP_DM_TEST_DOWN_LAST = HCI_TEST | HCI_MAX_TEST_OCF_V1_1,      /* not a primitive */

    /*-------------------------------------------------------------
      Upstream primitives
      ---------------------------------------------------------------*/
    ENUM_OBSOLETE_DM_HCI_READ_LOOPBACK_MODE_COMPLETE,
    ENUM_OBSOLETE_DM_HCI_WRITE_LOOPBACK_MODE_COMPLETE,
    ENUM_OBSOLETE_DM_HCI_LOOPBACK_EVENT,
    ENUM_DM_HCI_ENABLE_DEVICE_UT_MODE_COMPLETE,
    ENUM_SEP_DM_TEST_UP_LAST,                         /* not a primitive */

    /* add everything above here */
    ENUM_SEP_DM_LAST_PRIM,
   
    /* Downstream */
    ENUM_DM_HCI_CREATE_CONNECTION_CANCEL=0x7000,
    ENUM_DM_HCI_REMOTE_NAME_REQ_CANCEL,
    ENUM_DM_HCI_READ_REMOTE_EXT_FEATURES,
    ENUM_DM_HCI_READ_LMP_HANDLE,

    ENUM_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS,
    ENUM_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS,
    ENUM_DM_HCI_FLOW_SPEC,

    ENUM_DM_HCI_SET_AFH_CHANNEL_CLASS,
    ENUM_DM_HCI_READ_INQUIRY_SCAN_TYPE,
    ENUM_DM_HCI_WRITE_INQUIRY_SCAN_TYPE,
    ENUM_DM_HCI_READ_INQUIRY_MODE,
    ENUM_DM_HCI_WRITE_INQUIRY_MODE,
    ENUM_DM_HCI_READ_PAGE_SCAN_TYPE,
    ENUM_DM_HCI_WRITE_PAGE_SCAN_TYPE,
    ENUM_DM_HCI_READ_AFH_CHANNEL_CLASS_M,
    ENUM_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M,
    
    ENUM_DM_HCI_READ_LOCAL_EXT_FEATURES,
    ENUM_DM_HCI_READ_AFH_CHANNEL_MAP,
    ENUM_DM_HCI_READ_CLOCK,

    ENUM_OBSOLETE_DM_HCI_SETUP_SYNCHRONOUS_CONN,
    ENUM_OBSOLETE_DM_HCI_ACCEPT_SYNCHRONOUS_CONN_REQ,
    ENUM_OBSOLETE_DM_HCI_REJECT_SYNCHRONOUS_CONN,

    /* Sniff sub rate and enhanced flush are 2.1 features but due to
     * the fact the whole numbering of the enums is broken they have
     * had to be placed here for now!
    */
    ENUM_DM_HCI_SNIFF_SUB_RATE,
    ENUM_DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA,
    ENUM_DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA,
    ENUM_DM_HCI_ENHANCED_FLUSH,
    ENUM_DM_SM_USER_CONFIRMATION_REQUEST_RES,
    ENUM_DM_SM_USER_CONFIRMATION_REQUEST_NEG_RES,
    ENUM_DM_SM_USER_PASSKEY_REQUEST_RES,
    ENUM_DM_SM_USER_PASSKEY_REQUEST_NEG_RES,
    ENUM_DM_SM_READ_LOCAL_OOB_DATA_REQ,
    ENUM_DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ,
    ENUM_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ,
    ENUM_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ,
    ENUM_DM_HCI_REFRESH_ENCRYPTION_KEY,

    /* Upstream */
    ENUM_DM_HCI_CREATE_CONNECTION_CANCEL_COMPLETE = DM_1P2_PRIM,
    ENUM_DM_HCI_REMOTE_NAME_REQ_CANCEL_COMPLETE,
    ENUM_DM_HCI_READ_REMOTE_EXT_FEATURES_COMPLETE,
    ENUM_DM_HCI_READ_LMP_HANDLE_COMPLETE,
    ENUM_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_COMPLETE,
    ENUM_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_COMPLETE,    
    ENUM_DM_HCI_FLOW_SPEC_COMPLETE,

    ENUM_DM_HCI_SET_AFH_CHANNEL_CLASS_COMPLETE,
    ENUM_DM_HCI_READ_INQUIRY_SCAN_TYPE_COMPLETE,
    ENUM_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_COMPLETE,
    ENUM_DM_HCI_READ_INQUIRY_MODE_COMPLETE,
    ENUM_DM_HCI_WRITE_INQUIRY_MODE_COMPLETE,
    ENUM_DM_HCI_READ_PAGE_SCAN_TYPE_COMPLETE,
    ENUM_DM_HCI_WRITE_PAGE_SCAN_TYPE_COMPLETE,
    ENUM_DM_HCI_READ_AFH_CHANNEL_CLASS_M_COMPLETE,
    ENUM_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_COMPLETE,
    
    ENUM_DM_HCI_READ_LOCAL_EXT_FEATURES_COMPLETE,
    ENUM_DM_HCI_READ_AFH_CHANNEL_MAP_COMPLETE,
    ENUM_DM_HCI_READ_CLOCK_COMPLETE,

    ENUM_OBSOLETE_DM_HCI_SETUP_SYNCHRONOUS_CONN_COMPLETE,
    ENUM_OBSOLETE_DM_HCI_ACCEPT_SYNCHRONOUS_CONN_REQ_COMPLETE,
    ENUM_OBSOLETE_DM_HCI_REJECT_SYNCHRONOUS_CONN_COMPLETE,
    
    ENUM_DM_HCI_INQUIRY_RESULT_WITH_RSSI,
















    /* Upstream */
    ENUM_DM_HCI_SNIFF_SUB_RATING_IND    = DM_2P1_PRIM,
    ENUM_DM_HCI_SNIFF_SUB_RATE_COMPLETE,
    ENUM_DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_COMPLETE,
    ENUM_DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_COMPLETE,
    ENUM_DM_HCI_EXTENDED_INQUIRY_RESULT_IND,
    ENUM_DM_HCI_ENHANCED_FLUSH_COMPLETE,
    ENUM_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_COMPLETE,
    ENUM_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_COMPLETE,
    ENUM_DM_HCI_LINK_SUPERV_TIMEOUT_IND,
    ENUM_DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND,
    ENUM_DM_HCI_ENCRYPTION_KEY_REFRESH_IND,

#ifndef EXCLUDE_BLUESTACK_ENHANCEMENTS
    /*-------------------------------------------------------------
      Device Manager Enhancements
    ---------------------------------------------------------------*/
    ENUM_DM_EN_ENABLE_ENHANCEMENTS_REQ = DM_EN_PRIM,
    ENUM_DM_EN_ENABLE_ENHANCEMENTS_CFM,
    ENUM_DM_EN_READ_ENHANCEMENTS_REQ,
    ENUM_DM_EN_READ_ENHANCEMENTS_CFM,
    ENUM_DM_EN_ACL_OPENED_IND,
    ENUM_DM_EN_ACL_CLOSED_IND,
    ENUM_DM_EN_ACL_DETACH_REQ,
    ENUM_DM_EN_BLUESTACK_STATE_TRANSITION_ENTRY_IND,
    ENUM_DM_EN_BLUESTACK_STATE_TRANSITION_EXIT_IND,
#endif

#ifdef INSTALL_DYNAMIC_TOKEN_HANDLING

    ENUM_DM_MC_READ_CHANNEL_INFO_REQ = DM_MC_PRIM,
    ENUM_DM_MC_READ_CHANNEL_INFO_CFM,
    ENUM_DM_MC_RESERVE_TOKENS_REQ,
    ENUM_DM_MC_RESERVE_TOKENS_CFM,
    ENUM_DM_MC_RETURN_TOKENS_REQ,
    ENUM_DM_MC_RETURN_TOKENS_CFM,

#endif 

    /*
      Link Policy Power State primitives
    */
    ENUM_DM_LP_WRITE_POWERSTATES_REQ = 0x7900,
    ENUM_DM_LP_WRITE_POWERSTATES_CFM,
    ENUM_DM_LP_WRITE_ROLESWITCH_POLICY_REQ,
    ENUM_DM_LP_WRITE_ROLESWITCH_POLICY_CFM,
    ENUM_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ,
    ENUM_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM,

    /*-------------------------------------------------------------
      Synchronous Connection Interface Primitives
      ---------------------------------------------------------------*/
    ENUM_SEP_DM_SYNC_FIRST= DM_SYNC_PRIM,               /* not a primitive */
    ENUM_DM_SYNC_REGISTER_REQ,
    ENUM_DM_SYNC_UNREGISTER_REQ,
    ENUM_DM_SYNC_CONNECT_REQ,
    ENUM_DM_SYNC_CONNECT_CFM,
    ENUM_DM_SYNC_CONNECT_COMPLETE_IND,
    ENUM_DM_SYNC_CONNECT_IND,
    ENUM_DM_SYNC_CONNECT_RES,
    ENUM_DM_SYNC_DISCONNECT_REQ,
    ENUM_DM_SYNC_DISCONNECT_IND,
    ENUM_DM_SYNC_DISCONNECT_CFM,
    ENUM_DM_SYNC_REGISTER_CFM,
    ENUM_DM_SYNC_UNREGISTER_CFM,
    ENUM_DM_SYNC_RENEGOTIATE_REQ,
    ENUM_DM_SYNC_RENEGOTIATE_IND,
    ENUM_DM_SYNC_RENEGOTIATE_CFM,
    ENUM_SEP_DM_SYNC_LAST                               /* not a primitive */













} DM_PRIM_T;

/* ... and then reflect it here */
#define DM_AM_REGISTER_REQ              ((dm_prim_t)ENUM_DM_AM_REGISTER_REQ)
#define DM_AM_REGISTER_CFM              ((dm_prim_t)ENUM_DM_AM_REGISTER_CFM)
#define DM_SET_BT_VERSION_REQ           ((dm_prim_t)ENUM_DM_SET_BT_VERSION_REQ)
#define DM_SET_BT_VERSION_CFM           ((dm_prim_t)ENUM_DM_SET_BT_VERSION_CFM)
#define DM_AM_REGISTER_WRONG_API_REQ    ((dm_prim_t)ENUM_DM_AM_REGISTER_WRONG_API_REQ)
#define DM_AM_REGISTER_WRONG_API_CFM    ((dm_prim_t)ENUM_DM_AM_REGISTER_WRONG_API_CFM)

/*-------------------------------------------------------------
  ACL Connection Interface Primitives
  ---------------------------------------------------------------*/

#define DM_ACL_REGISTER_REQ             ((dm_prim_t)(ENUM_DM_ACL_REGISTER_REQ))
#define DM_ACL_REGISTER_CFM             ((dm_prim_t)(ENUM_DM_ACL_REGISTER_CFM))
#define DM_ACL_CONNECT_REQ              ((dm_prim_t)(ENUM_DM_ACL_CONNECT_REQ))
#define DM_ACL_CONNECT_CFM              ((dm_prim_t)(ENUM_DM_ACL_CONNECT_CFM))
#define DM_ACL_CONNECT_IND              ((dm_prim_t)(ENUM_DM_ACL_CONNECT_IND))
#define DM_ACL_DISCONNECT_REQ           ((dm_prim_t)(ENUM_DM_ACL_DISCONNECT_REQ))
#define DM_ACL_DISCONNECT_IND           ((dm_prim_t)(ENUM_DM_ACL_DISCONNECT_IND))
#define DM_ACL_BUFFER_SIZE_IND          ((dm_prim_t)(ENUM_DM_ACL_BUFFER_SIZE_IND))
#define DM_ACL_DATA_SENT_IND            ((dm_prim_t)(ENUM_DM_ACL_DATA_SENT_IND))

/*-------------------------------------------------------------
  Synchronous Connection Interface Primitives
  ---------------------------------------------------------------*/
#define DM_SYNC_REGISTER_REQ            ((dm_prim_t)(ENUM_DM_SYNC_REGISTER_REQ))
#define DM_SYNC_REGISTER_CFM            ((dm_prim_t)(ENUM_DM_SYNC_REGISTER_CFM))
#define DM_SYNC_UNREGISTER_REQ          ((dm_prim_t)(ENUM_DM_SYNC_UNREGISTER_REQ))
#define DM_SYNC_UNREGISTER_CFM          ((dm_prim_t)(ENUM_DM_SYNC_UNREGISTER_CFM))
#define DM_SYNC_CONNECT_REQ             ((dm_prim_t)(ENUM_DM_SYNC_CONNECT_REQ))
#define DM_SYNC_CONNECT_CFM             ((dm_prim_t)(ENUM_DM_SYNC_CONNECT_CFM))
#define DM_SYNC_CONNECT_COMPLETE_IND    ((dm_prim_t)(ENUM_DM_SYNC_CONNECT_COMPLETE_IND))
#define DM_SYNC_CONNECT_IND             ((dm_prim_t)(ENUM_DM_SYNC_CONNECT_IND))
#define DM_SYNC_CONNECT_RES             ((dm_prim_t)(ENUM_DM_SYNC_CONNECT_RES))
#define DM_SYNC_RENEGOTIATE_REQ         ((dm_prim_t)(ENUM_DM_SYNC_RENEGOTIATE_REQ))
#define DM_SYNC_RENEGOTIATE_IND         ((dm_prim_t)(ENUM_DM_SYNC_RENEGOTIATE_IND))
#define DM_SYNC_RENEGOTIATE_CFM         ((dm_prim_t)(ENUM_DM_SYNC_RENEGOTIATE_CFM))
#define DM_SYNC_DISCONNECT_REQ          ((dm_prim_t)(ENUM_DM_SYNC_DISCONNECT_REQ))
#define DM_SYNC_DISCONNECT_IND          ((dm_prim_t)(ENUM_DM_SYNC_DISCONNECT_IND))
#define DM_SYNC_DISCONNECT_CFM          ((dm_prim_t)(ENUM_DM_SYNC_DISCONNECT_CFM))












/*-------------------------------------------------------------
  Security Management Primitives
  ---------------------------------------------------------------*/

#define DM_SM_SET_DEFAULT_SECURITY_REQ  ((dm_prim_t)(ENUM_DM_SM_SET_DEFAULT_SECURITY_REQ))
#define DM_SM_REGISTER_REQ              ((dm_prim_t)(ENUM_DM_SM_REGISTER_REQ))
#define DM_SM_UNREGISTER_REQ            ((dm_prim_t)(ENUM_DM_SM_UNREGISTER_REQ))
#define DM_SM_REGISTER_OUTGOING_REQ     ((dm_prim_t)(ENUM_DM_SM_REGISTER_OUTGOING_REQ))
#define DM_SM_UNREGISTER_OUTGOING_REQ   ((dm_prim_t)(ENUM_DM_SM_UNREGISTER_OUTGOING_REQ))
#define DM_SM_ACCESS_REQ                ((dm_prim_t)(ENUM_DM_SM_ACCESS_REQ))
#define DM_SM_ACCESS_CFM                ((dm_prim_t)(ENUM_DM_SM_ACCESS_CFM))
#define DM_SM_ACCESS_IND                ((dm_prim_t)(ENUM_DM_SM_ACCESS_IND))
#define DM_SM_SET_SEC_MODE_REQ          ((dm_prim_t)(ENUM_DM_SM_SET_SEC_MODE_REQ))
#define DM_SM_SET_SEC_MODE_CFM          ((dm_prim_t)(ENUM_DM_SM_SET_SEC_MODE_CFM))
#define DM_SM_ADD_DEVICE_REQ            ((dm_prim_t)(ENUM_DM_SM_ADD_DEVICE_REQ))
#define DM_SM_ADD_DEVICE_CFM            ((dm_prim_t)(ENUM_DM_SM_ADD_DEVICE_CFM))
#define DM_SM_REMOVE_DEVICE_REQ         ((dm_prim_t)(ENUM_DM_SM_REMOVE_DEVICE_REQ))
#define DM_SM_REMOVE_DEVICE_CFM         ((dm_prim_t)(ENUM_DM_SM_REMOVE_DEVICE_CFM))
#define DM_SM_LINK_KEY_REQUEST_IND      ((dm_prim_t)(ENUM_DM_SM_LINK_KEY_REQUEST_IND))
#define DM_SM_LINK_KEY_REQUEST_RES      ((dm_prim_t)(ENUM_DM_SM_LINK_KEY_REQUEST_RES))
#define DM_SM_PIN_REQUEST_IND           ((dm_prim_t)(ENUM_DM_SM_PIN_REQUEST_IND))
#define DM_SM_PIN_REQUEST_RES           ((dm_prim_t)(ENUM_DM_SM_PIN_REQUEST_RES))
#define DM_SM_LINK_KEY_IND              ((dm_prim_t)(ENUM_DM_SM_LINK_KEY_IND))
#define DM_SM_AUTHORISE_IND             ((dm_prim_t)(ENUM_DM_SM_AUTHORISE_IND))
#define DM_SM_AUTHORISE_RES             ((dm_prim_t)(ENUM_DM_SM_AUTHORISE_RES))
#define DM_SM_AUTHENTICATE_REQ          ((dm_prim_t)(ENUM_DM_SM_AUTHENTICATE_REQ))
#define DM_SM_AUTHENTICATE_CFM          ((dm_prim_t)(ENUM_DM_SM_AUTHENTICATE_CFM))
#define DM_SM_ENCRYPT_REQ               ((dm_prim_t)(ENUM_DM_SM_ENCRYPT_REQ))
#define DM_SM_ENCRYPT_CFM               ((dm_prim_t)(ENUM_DM_SM_ENCRYPT_CFM))
#define DM_SM_ENCRYPTION_CHANGE         ((dm_prim_t)(ENUM_DM_SM_ENCRYPTION_CHANGE))
#define DM_SM_ENCRYPTION_CHANGE_IND     DM_SM_ENCRYPTION_CHANGE
#define DM_SM_L2CAP_REGISTER_REQ        ((dm_prim_t)(ENUM_DM_SM_L2CAP_REGISTER_REQ))
#define DM_SM_L2CAP_CLRX_ENABLE_IND     ((dm_prim_t)(ENUM_DM_SM_L2CAP_CLRX_ENABLE_IND))
#define DM_SM_SEC_MODE_CONFIG_REQ                 \
                ((dm_prim_t)(ENUM_DM_SM_SEC_MODE_CONFIG_REQ))
#define DM_SM_SEC_MODE_CONFIG_CFM                 \
                ((dm_prim_t)(ENUM_DM_SM_SEC_MODE_CONFIG_CFM))
#define DM_SM_IO_CAPABILITY_RESPONSE_IND        \
                ((dm_prim_t)(ENUM_DM_SM_IO_CAPABILITY_RESPONSE_IND))
#define DM_SM_IO_CAPABILITY_REQUEST_IND         \
                ((dm_prim_t)(ENUM_DM_SM_IO_CAPABILITY_REQUEST_IND))
#define DM_SM_IO_CAPABILITY_REQUEST_RES         \
                ((dm_prim_t)(ENUM_DM_SM_IO_CAPABILITY_REQUEST_RES))
#define DM_SM_IO_CAPABILITY_REQUEST_NEG_RES     \
                ((dm_prim_t)(ENUM_DM_SM_IO_CAPABILITY_REQUEST_NEG_RES))
#define DM_SM_SSP_ADD_DEVICE_REQ           \
                ((dm_prim_t)(ENUM_DM_SM_SSP_ADD_DEVICE_REQ))
#define DM_SM_SSP_ADD_DEVICE_CFM           \
                ((dm_prim_t)(ENUM_DM_SM_SSP_ADD_DEVICE_CFM))
#define DM_SM_USER_CONFIRMATION_REQUEST_IND     \
                ((dm_prim_t)(ENUM_DM_SM_USER_CONFIRMATION_REQUEST_IND))
#define DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND  \
                ((dm_prim_t)(ENUM_DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND))
#define DM_SM_USER_CONFIRMATION_REQUEST_RES     \
                ((dm_prim_t)(ENUM_DM_SM_USER_CONFIRMATION_REQUEST_RES))
#define DM_SM_USER_CONFIRMATION_REQUEST_NEG_RES \
                ((dm_prim_t)(ENUM_DM_SM_USER_CONFIRMATION_REQUEST_NEG_RES))
#define DM_SM_USER_PASSKEY_REQUEST_IND          \
                ((dm_prim_t)(ENUM_DM_SM_USER_PASSKEY_REQUEST_IND))
#define DM_SM_USER_PASSKEY_REQUEST_RES          \
                ((dm_prim_t)(ENUM_DM_SM_USER_PASSKEY_REQUEST_RES))
#define DM_SM_USER_PASSKEY_REQUEST_NEG_RES      \
                ((dm_prim_t)(ENUM_DM_SM_USER_PASSKEY_REQUEST_NEG_RES))
#define DM_SM_SIMPLE_PAIRING_COMPLETE_IND       \
                ((dm_prim_t)(ENUM_DM_SM_SIMPLE_PAIRING_COMPLETE_IND))
#define DM_SM_READ_LOCAL_OOB_DATA_REQ           \
                ((dm_prim_t)(ENUM_DM_SM_READ_LOCAL_OOB_DATA_REQ))
#define DM_SM_READ_LOCAL_OOB_DATA_CFM           \
                ((dm_prim_t)(ENUM_DM_SM_READ_LOCAL_OOB_DATA_CFM))
#define DM_SM_SSP_LINK_KEY_REQUEST_IND          \
                ((dm_prim_t)(ENUM_DM_SM_SSP_LINK_KEY_REQUEST_IND))
#define DM_SM_SSP_LINK_KEY_REQUEST_RES          \
                ((dm_prim_t)(ENUM_DM_SM_SSP_LINK_KEY_REQUEST_RES))
#define DM_SM_USER_PASSKEY_NOTIFICATION_IND     \
                ((dm_prim_t)(ENUM_DM_SM_USER_PASSKEY_NOTIFICATION_IND))
#define DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ    \
                ((dm_prim_t)(ENUM_DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ))
#define DM_SM_KEYPRESS_NOTIFICATION_IND         \
                ((dm_prim_t)(ENUM_DM_SM_KEYPRESS_NOTIFICATION_IND))
#define DM_SM_BONDING_REQ           ((dm_prim_t)(ENUM_DM_SM_BONDING_REQ))
#define DM_SM_BONDING_CFM           ((dm_prim_t)(ENUM_DM_SM_BONDING_CFM))
#define DM_SM_BONDING_CANCEL_REQ    ((dm_prim_t)(ENUM_DM_SM_BONDING_CANCEL_REQ))
#define DM_SM_READ_DEVICE_REQ       ((dm_prim_t)(ENUM_DM_SM_READ_DEVICE_REQ))
#define DM_SM_READ_DEVICE_CFM       ((dm_prim_t)(ENUM_DM_SM_READ_DEVICE_CFM))

#define DM_SM_BASE                      (ENUM_SEP_DM_SM_DOWN_FIRST + 1)
#define DM_SM_DOWN_MAX                  (ENUM_SEP_DM_SM_DOWN2_LAST - 1)
#define DM_SM_MAX                       (ENUM_SEP_DM_SM_UP_LAST - 1)

/*-------------------------------------------------------------
  DM private primitives
  ---------------------------------------------------------------*/

#define DM_WRITE_CACHED_PAGE_MODE_REQ	 ((dm_prim_t)(ENUM_DM_WRITE_CACHED_PAGE_MODE_REQ))
#define DM_WRITE_CACHED_PAGE_MODE_CFM	 ((dm_prim_t)(ENUM_DM_WRITE_CACHED_PAGE_MODE_CFM))
#define DM_WRITE_CACHED_CLOCK_OFFSET_REQ ((dm_prim_t)(ENUM_DM_WRITE_CACHED_CLOCK_OFFSET_REQ))
#define DM_WRITE_CACHED_CLOCK_OFFSET_CFM ((dm_prim_t)(ENUM_DM_WRITE_CACHED_CLOCK_OFFSET_CFM))
#define DM_CLEAR_PARAM_CACHE_REQ		 ((dm_prim_t)(ENUM_DM_CLEAR_PARAM_CACHE_REQ))
#define DM_CLEAR_PARAM_CACHE_CFM		 ((dm_prim_t)(ENUM_DM_CLEAR_PARAM_CACHE_CFM))
#define DM_HC_TO_HOST_FLOW_CONTROL       ((dm_prim_t)(ENUM_DM_HC_TO_HOST_FLOW_CONTROL))
#define DM_ACL_OPEN_REQ                  ((dm_prim_t)(ENUM_DM_ACL_OPEN_REQ))
#define DM_ACL_OPEN_CFM                  ((dm_prim_t)(ENUM_DM_ACL_OPEN_CFM))
#define DM_ACL_CLOSE_REQ                 ((dm_prim_t)(ENUM_DM_ACL_CLOSE_REQ))
#define DM_ACL_OPENED_IND                ((dm_prim_t)(ENUM_DM_ACL_OPENED_IND))
#define DM_ACL_CLOSED_IND                ((dm_prim_t)(ENUM_DM_ACL_CLOSED_IND))
#define DM_SET_DEFAULT_LINK_POLICY       ((dm_prim_t)(ENUM_DM_SET_DEFAULT_LINK_POLICY))

#define DM_LP_WRITE_POWERSTATES_REQ      ((dm_prim_t)(ENUM_DM_LP_WRITE_POWERSTATES_REQ))
#define DM_LP_WRITE_POWERSTATES_CFM      ((dm_prim_t)(ENUM_DM_LP_WRITE_POWERSTATES_CFM))
#define DM_LP_WRITE_ROLESWITCH_POLICY_REQ   ((dm_prim_t)(ENUM_DM_LP_WRITE_ROLESWITCH_POLICY_REQ))
#define DM_LP_WRITE_ROLESWITCH_POLICY_CFM   ((dm_prim_t)(ENUM_DM_LP_WRITE_ROLESWITCH_POLICY_CFM))
#define DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ ((dm_prim_t)(ENUM_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ))
#define DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM ((dm_prim_t)(ENUM_DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM))


#ifndef EXCLUDE_BLUESTACK_ENHANCEMENTS
#define DM_EN_ENABLE_ENHANCEMENTS_REQ    ((dm_prim_t)(ENUM_DM_EN_ENABLE_ENHANCEMENTS_REQ))
#define DM_EN_ENABLE_ENHANCEMENTS_CFM    ((dm_prim_t)(ENUM_DM_EN_ENABLE_ENHANCEMENTS_CFM))
#define DM_EN_READ_ENHANCEMENTS_REQ      ((dm_prim_t)(ENUM_DM_EN_READ_ENHANCEMENTS_REQ))
#define DM_EN_READ_ENHANCEMENTS_CFM      ((dm_prim_t)(ENUM_DM_EN_READ_ENHANCEMENTS_CFM))
#define DM_EN_ACL_OPENED_IND             ((dm_prim_t)(ENUM_DM_EN_ACL_OPENED_IND))
#define DM_EN_ACL_CLOSED_IND             ((dm_prim_t)(ENUM_DM_EN_ACL_CLOSED_IND))
#define DM_EN_ACL_DETACH_REQ             ((dm_prim_t)(ENUM_DM_EN_ACL_DETACH_REQ))

#define DM_EN_BLUESTACK_STATE_TRANSITION_ENTRY_IND ((dm_prim_t)(ENUM_DM_EN_BLUESTACK_STATE_TRANSITION_ENTRY_IND))
#define DM_EN_BLUESTACK_STATE_TRANSITION_EXIT_IND ((dm_prim_t)(ENUM_DM_EN_BLUESTACK_STATE_TRANSITION_EXIT_IND))
#endif

#define DM_PRIV_BASE	                    (ENUM_SEP_DM_PRIV_DOWN_FIRST + 1)
#define DM_PRIV_DOWN_MAX                    (ENUM_SEP_DM_PRIV_DOWN_LAST - 1)
#define DM_PRIV_MAX                         (ENUM_SEP_DM_PRIV_UP_LAST - 1)

/*-------------------------------------------------------------
SCO Connection Interface (None - no special SCO primitives defined.
                Just use Connection Interface primitives)
---------------------------------------------------------------*/

#define DM_SCO_BASE	                 DM_CONN_BASE
#define DM_SCO_MAX                   DM_CONN_MAX

/*-------------------------------------------------------------
MCDSP primitives
---------------------------------------------------------------*/
#ifdef INSTALL_DYNAMIC_TOKEN_HANDLING

#define DM_MC_READ_CHANNEL_INFO_REQ ((dm_prim_t)(ENUM_DM_MC_READ_CHANNEL_INFO_REQ))
#define DM_MC_READ_CHANNEL_INFO_CFM ((dm_prim_t)(ENUM_DM_MC_READ_CHANNEL_INFO_CFM))
#define DM_MC_RESERVE_TOKENS_REQ    ((dm_prim_t)(ENUM_DM_MC_RESERVE_TOKENS_REQ))
#define DM_MC_RESERVE_TOKENS_CFM    ((dm_prim_t)(ENUM_DM_MC_RESERVE_TOKENS_CFM))
#define DM_MC_RETURN_TOKENS_REQ     ((dm_prim_t)(ENUM_DM_MC_RETURN_TOKENS_REQ))
#define DM_MC_RETURN_TOKENS_CFM     ((dm_prim_t)(ENUM_DM_MC_RETURN_TOKENS_CFM))

#endif 


/*-------------------------------------------------------------
HCI API primitives
---------------------------------------------------------------*/

/******************************************************************************
 Link Control 
 *****************************************************************************/

/*-------------------------------------------------------------
Downstream primitives
---------------------------------------------------------------*/
#define DM_HCI_INQUIRY                    ((dm_prim_t) ENUM_DM_HCI_INQUIRY)
#define DM_HCI_INQUIRY_CANCEL             ((dm_prim_t) ENUM_DM_HCI_INQUIRY_CANCEL)
#define DM_HCI_PERIODIC_INQUIRY           ((dm_prim_t) ENUM_DM_HCI_PERIODIC_INQUIRY)
#define DM_HCI_EXIT_PERIODIC_INQUIRY      ((dm_prim_t) ENUM_DM_HCI_EXIT_PERIODIC_INQUIRY)
#define DM_HCI_CHANGE_PACKET_TYPE		  ((dm_prim_t) ENUM_DM_HCI_CHANGE_PACKET_TYPE)



#define DM_HCI_CHANGE_LINK_KEY            ((dm_prim_t) ENUM_DM_HCI_CHANGE_LINK_KEY)
#define DM_HCI_MASTER_LINK_KEY            ((dm_prim_t) ENUM_DM_HCI_MASTER_LINK_KEY)
#define DM_HCI_REMOTE_NAME_REQUEST        ((dm_prim_t) ENUM_DM_HCI_REMOTE_NAME_REQUEST)
#define DM_HCI_READ_REMOTE_FEATURES       ((dm_prim_t) ENUM_DM_HCI_READ_REMOTE_FEATURES)
#define DM_HCI_READ_REMOTE_VERSION        ((dm_prim_t) ENUM_DM_HCI_READ_REMOTE_VERSION)
#define DM_HCI_READ_CLOCK_OFFSET          ((dm_prim_t) ENUM_DM_HCI_READ_CLOCK_OFFSET)

/*-------------------------------------------------------------
Upstream primitives
---------------------------------------------------------------*/
#define DM_HCI_INQUIRY_CANCEL_COMPLETE        ((dm_prim_t)ENUM_DM_HCI_INQUIRY_CANCEL_COMPLETE)
#define DM_HCI_PERIODIC_INQUIRY_COMPLETE      ((dm_prim_t)ENUM_DM_HCI_PERIODIC_INQUIRY_COMPLETE)
#define DM_HCI_EXIT_PERIODIC_INQUIRY_COMPLETE ((dm_prim_t)ENUM_DM_HCI_EXIT_PERIODIC_INQUIRY_COMPLETE)
#define DM_HCI_INQUIRY_RESULT                 ((dm_prim_t)ENUM_DM_HCI_INQUIRY_RESULT)
#define DM_HCI_INQUIRY_COMPLETE               ((dm_prim_t)ENUM_DM_HCI_INQUIRY_COMPLETE)
#define DM_HCI_LINK_KEY_CHANGE_COMPLETE       ((dm_prim_t)ENUM_DM_HCI_LINK_KEY_CHANGE_COMPLETE)
#define DM_HCI_RETURN_LINK_KEYS               ((dm_prim_t)ENUM_DM_HCI_RETURN_LINK_KEYS)
#define DM_HCI_MASTER_LINK_KEY_COMPLETE       ((dm_prim_t)ENUM_DM_HCI_MASTER_LINK_KEY_COMPLETE)
#define DM_HCI_REMOTE_NAME_COMPLETE           ((dm_prim_t)ENUM_DM_HCI_REMOTE_NAME_COMPLETE)
#define DM_HCI_READ_REMOTE_FEATURES_COMPLETE  ((dm_prim_t)ENUM_DM_HCI_READ_REMOTE_FEATURES_COMPLETE)
#define DM_HCI_READ_REMOTE_VERSION_COMPLETE   ((dm_prim_t)ENUM_DM_HCI_READ_REMOTE_VERSION_COMPLETE)
#define DM_HCI_READ_CLOCK_OFFSET_COMPLETE     ((dm_prim_t)ENUM_DM_HCI_READ_CLOCK_OFFSET_COMPLETE)

#define DM_LC_BASE	                    (ENUM_SEP_DM_LC_DOWN_FIRST + 1)
#define DM_LC_DOWN_MAX                  (ENUM_SEP_DM_LC_DOWN_LAST - 1)
#define DM_LC_MAX                       (ENUM_SEP_DM_LC_UP_LAST - 1)

/******************************************************************************
   Link Policy 
 *****************************************************************************/
/*-------------------------------------------------------------
Downstream primitives
---------------------------------------------------------------*/
#define DM_HCI_HOLD_MODE           ((dm_prim_t) ENUM_DM_HCI_HOLD_MODE)
#define DM_HCI_SNIFF_MODE          ((dm_prim_t) ENUM_DM_HCI_SNIFF_MODE)
#define DM_HCI_EXIT_SNIFF_MODE     ((dm_prim_t) ENUM_DM_HCI_EXIT_SNIFF_MODE)
#define DM_HCI_PARK_MODE           ((dm_prim_t) ENUM_DM_HCI_PARK_MODE)
#define DM_HCI_EXIT_PARK_MODE      ((dm_prim_t) ENUM_DM_HCI_EXIT_PARK_MODE)
#define DM_HCI_QOS_SETUP_REQ       ((dm_prim_t) ENUM_DM_HCI_QOS_SETUP_REQ)
#define DM_HCI_ROLE_DISCOVERY      ((dm_prim_t) ENUM_DM_HCI_ROLE_DISCOVERY)
#define DM_HCI_SWITCH_ROLE         ((dm_prim_t) ENUM_DM_HCI_SWITCH_ROLE)
#define DM_HCI_READ_LP_SETTINGS    ((dm_prim_t) ENUM_DM_HCI_READ_LP_SETTINGS)
#define DM_HCI_WRITE_LP_SETTINGS   ((dm_prim_t) ENUM_DM_HCI_WRITE_LP_SETTINGS)

/*-------------------------------------------------------------
Upstream primitives
---------------------------------------------------------------*/
#define DM_HCI_MODE_CHANGE_EVENT          ((dm_prim_t)ENUM_DM_HCI_MODE_CHANGE_EVENT)
#define DM_HCI_QOS_SETUP_CFM              ((dm_prim_t)ENUM_DM_HCI_QOS_SETUP_CFM)
#define DM_HCI_QOS_VIOLATION_IND          ((dm_prim_t)ENUM_DM_HCI_QOS_VIOLATION_IND)
#define DM_HCI_ROLE_DISCOVERY_COMPLETE    ((dm_prim_t)ENUM_DM_HCI_ROLE_DISCOVERY_COMPLETE)
#define DM_HCI_SWITCH_ROLE_COMPLETE       ((dm_prim_t)ENUM_DM_HCI_SWITCH_ROLE_COMPLETE)
#define DM_HCI_READ_LP_SETTINGS_COMPLETE  ((dm_prim_t)ENUM_DM_HCI_READ_LP_SETTINGS_COMPLETE)
#define DM_HCI_WRITE_LP_SETTINGS_COMPLETE ((dm_prim_t)ENUM_DM_HCI_WRITE_LP_SETTINGS_COMPLETE)

#define DM_LP_BASE						  (ENUM_SEP_DM_LP_DOWN_FIRST + 1)
#define DM_LP_DOWN_MAX                    (ENUM_SEP_DM_LP_DOWN_LAST - 1)
#define DM_LP_MAX                         (ENUM_SEP_DM_LP_DOWN_LAST - 1)

/******************************************************************************
   Host Controller and Baseband
 *****************************************************************************/
/*-------------------------------------------------------------
Downstream primitives
---------------------------------------------------------------*/
#define DM_HCI_SET_EVENT_MASK                   ((dm_prim_t) ENUM_DM_HCI_SET_EVENT_MASK)
#define DM_HCI_RESET                            ((dm_prim_t) ENUM_DM_HCI_RESET)
#define DM_HCI_SET_EVENT_FILTER                 ((dm_prim_t) ENUM_DM_HCI_SET_EVENT_FILTER)
#define DM_HCI_FLUSH                            ((dm_prim_t) ENUM_DM_HCI_FLUSH)
#define DM_HCI_READ_PIN_TYPE                    ((dm_prim_t) ENUM_DM_HCI_READ_PIN_TYPE)
#define DM_HCI_WRITE_PIN_TYPE                   ((dm_prim_t) ENUM_DM_HCI_WRITE_PIN_TYPE)
#define DM_HCI_CREATE_NEW_UNIT_KEY              ((dm_prim_t) ENUM_DM_HCI_CREATE_NEW_UNIT_KEY)
#define DM_HCI_READ_STORED_LINK_KEY             ((dm_prim_t) ENUM_DM_HCI_READ_STORED_LINK_KEY)
#define DM_HCI_WRITE_STORED_LINK_KEY            ((dm_prim_t) ENUM_DM_HCI_WRITE_STORED_LINK_KEY)
#define DM_HCI_DELETE_STORED_LINK_KEY           ((dm_prim_t) ENUM_DM_HCI_DELETE_STORED_LINK_KEY)
#define DM_HCI_CHANGE_LOCAL_NAME                ((dm_prim_t) ENUM_DM_HCI_CHANGE_LOCAL_NAME)
#define DM_HCI_READ_LOCAL_NAME			((dm_prim_t) ENUM_DM_HCI_READ_LOCAL_NAME)
#define DM_HCI_READ_CONN_ACCEPT_TO              ((dm_prim_t) ENUM_DM_HCI_READ_CONN_ACCEPT_TO)
#define DM_HCI_WRITE_CONN_ACCEPT_TO             ((dm_prim_t) ENUM_DM_HCI_WRITE_CONN_ACCEPT_TO)
#define DM_HCI_READ_PAGE_TO                     ((dm_prim_t) ENUM_DM_HCI_READ_PAGE_TO)
#define DM_HCI_WRITE_PAGE_TO                    ((dm_prim_t) ENUM_DM_HCI_WRITE_PAGE_TO)
#define DM_HCI_READ_SCAN_ENABLE                 ((dm_prim_t) ENUM_DM_HCI_READ_SCAN_ENABLE)
#define DM_HCI_WRITE_SCAN_ENABLE                ((dm_prim_t) ENUM_DM_HCI_WRITE_SCAN_ENABLE)
#define DM_HCI_READ_PAGESCAN_ACTIVITY           ((dm_prim_t) ENUM_DM_HCI_READ_PAGESCAN_ACTIVITY)
#define DM_HCI_WRITE_PAGESCAN_ACTIVITY          ((dm_prim_t) ENUM_DM_HCI_WRITE_PAGESCAN_ACTIVITY)
#define DM_HCI_READ_INQUIRYSCAN_ACTIVITY        ((dm_prim_t) ENUM_DM_HCI_READ_INQUIRYSCAN_ACTIVITY)
#define DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY       ((dm_prim_t) ENUM_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY)
#define DM_HCI_READ_AUTH_ENABLE                 ((dm_prim_t) ENUM_DM_HCI_READ_AUTH_ENABLE)
#define DM_HCI_READ_ENCRYPTION_MODE             ((dm_prim_t) ENUM_DM_HCI_READ_ENCRYPTION_MODE)
#define DM_HCI_READ_CLASS_OF_DEVICE             ((dm_prim_t) ENUM_DM_HCI_READ_CLASS_OF_DEVICE)
#define DM_HCI_WRITE_CLASS_OF_DEVICE            ((dm_prim_t) ENUM_DM_HCI_WRITE_CLASS_OF_DEVICE)
#define DM_HCI_READ_VOICE_SETTING               ((dm_prim_t) ENUM_DM_HCI_READ_VOICE_SETTING)
#define DM_HCI_WRITE_VOICE_SETTING              ((dm_prim_t) ENUM_DM_HCI_WRITE_VOICE_SETTING)
#define DM_HCI_READ_AUTO_FLUSH_TIMEOUT          ((dm_prim_t) ENUM_DM_HCI_READ_AUTO_FLUSH_TIMEOUT)
#define DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT         ((dm_prim_t) ENUM_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT)
#define DM_HCI_READ_NUM_BCAST_TXS               ((dm_prim_t) ENUM_DM_HCI_READ_NUM_BCAST_TXS)
#define DM_HCI_WRITE_NUM_BCAST_TXS              ((dm_prim_t) ENUM_DM_HCI_WRITE_NUM_BCAST_TXS)
#define DM_HCI_READ_HOLD_MODE_ACTIVITY          ((dm_prim_t) ENUM_DM_HCI_READ_HOLD_MODE_ACTIVITY)
#define DM_HCI_WRITE_HOLD_MODE_ACTIVITY         ((dm_prim_t) ENUM_DM_HCI_WRITE_HOLD_MODE_ACTIVITY)
#define DM_HCI_READ_TX_POWER_LEVEL              ((dm_prim_t) ENUM_DM_HCI_READ_TX_POWER_LEVEL)
#define DM_HCI_READ_SCO_FLOW_CONTROL_ENABLE     ((dm_prim_t) ENUM_DM_HCI_READ_SCO_FLOW_CONTROL_ENABLE)
#define DM_HCI_WRITE_SCO_FLOW_CONTROL_ENABLE    ((dm_prim_t) ENUM_DM_HCI_WRITE_SCO_FLOW_CONTROL_ENABLE)
#define DM_HCI_SET_HC_TO_HOST_FLOW              ((dm_prim_t) ENUM_DM_HCI_SET_HC_TO_HOST_FLOW)
#define DM_HCI_HOST_NUM_COMPLETED_PACKETS       ((dm_prim_t) ENUM_DM_HCI_HOST_NUM_COMPLETED_PACKETS)
#define DM_HCI_READ_LINK_SUPERV_TIMEOUT         ((dm_prim_t) ENUM_DM_HCI_READ_LINK_SUPERV_TIMEOUT)
#define DM_HCI_WRITE_LINK_SUPERV_TIMEOUT        ((dm_prim_t) ENUM_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT)
#define DM_HCI_READ_NUM_SUPPORTED_IAC           ((dm_prim_t) ENUM_DM_HCI_READ_NUM_SUPPORTED_IAC)
#define DM_HCI_READ_CURRENT_IAC_LAP             ((dm_prim_t) ENUM_DM_HCI_READ_CURRENT_IAC_LAP)
#define DM_HCI_WRITE_CURRENT_IAC_LAP            ((dm_prim_t) ENUM_DM_HCI_WRITE_CURRENT_IAC_LAP)
#define DM_HCI_READ_PAGESCAN_PERIOD_MODE        ((dm_prim_t) ENUM_DM_HCI_READ_PAGESCAN_PERIOD_MODE)
#define DM_HCI_WRITE_PAGESCAN_PERIOD_MODE       ((dm_prim_t) ENUM_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE)
#define DM_HCI_READ_PAGESCAN_MODE               ((dm_prim_t) ENUM_DM_HCI_READ_PAGESCAN_MODE)
#define DM_HCI_WRITE_PAGESCAN_MODE              ((dm_prim_t) ENUM_DM_HCI_WRITE_PAGESCAN_MODE)

/*-------------------------------------------------------------
Upstream primitives
---------------------------------------------------------------*/
#define DM_HCI_SET_EVENT_MASK_COMPLETE          ((dm_prim_t)ENUM_DM_HCI_SET_EVENT_MASK_COMPLETE)
#define DM_HCI_RESET_COMPLETE                   ((dm_prim_t)ENUM_DM_HCI_RESET_COMPLETE)
#define DM_HCI_SET_EVENT_FILTER_COMPLETE        ((dm_prim_t)ENUM_DM_HCI_SET_EVENT_FILTER_COMPLETE)
#define DM_HCI_FLUSH_COMPLETE                   ((dm_prim_t)ENUM_DM_HCI_FLUSH_COMPLETE)
#define DM_HCI_READ_PIN_TYPE_COMPLETE           ((dm_prim_t)ENUM_DM_HCI_READ_PIN_TYPE_COMPLETE)
#define DM_HCI_WRITE_PIN_TYPE_COMPLETE          ((dm_prim_t)ENUM_DM_HCI_WRITE_PIN_TYPE_COMPLETE)
#define DM_HCI_CREATE_NEW_UNIT_KEY_COMPLETE     ((dm_prim_t)ENUM_DM_HCI_CREATE_NEW_UNIT_KEY_COMPLETE)
#define DM_HCI_READ_STORED_LINK_KEY_COMPLETE    ((dm_prim_t)ENUM_DM_HCI_READ_STORED_LINK_KEY_COMPLETE)
#define DM_HCI_WRITE_STORED_LINK_KEY_COMPLETE   ((dm_prim_t)ENUM_DM_HCI_WRITE_STORED_LINK_KEY_COMPLETE)
#define DM_HCI_DELETE_STORED_LINK_KEY_COMPLETE  ((dm_prim_t)ENUM_DM_HCI_DELETE_STORED_LINK_KEY_COMPLETE)
#define DM_HCI_CHANGE_LOCAL_NAME_COMPLETE       ((dm_prim_t)ENUM_DM_HCI_CHANGE_LOCAL_NAME_COMPLETE)
#define DM_HCI_READ_LOCAL_NAME_COMPLETE			((dm_prim_t)ENUM_DM_HCI_READ_LOCAL_NAME_COMPLETE)
#define DM_HCI_READ_CONN_ACCEPT_TO_COMPLETE     ((dm_prim_t)ENUM_DM_HCI_READ_CONN_ACCEPT_TO_COMPLETE)
#define DM_HCI_WRITE_CONN_ACCEPT_TO_COMPLETE    ((dm_prim_t)ENUM_DM_HCI_WRITE_CONN_ACCEPT_TO_COMPLETE)
#define DM_HCI_READ_PAGE_TO_COMPLETE            ((dm_prim_t)ENUM_DM_HCI_READ_PAGE_TO_COMPLETE)
#define DM_HCI_WRITE_PAGE_TO_COMPLETE           ((dm_prim_t)ENUM_DM_HCI_WRITE_PAGE_TO_COMPLETE)
#define DM_HCI_READ_SCAN_ENABLE_COMPLETE        ((dm_prim_t)ENUM_DM_HCI_READ_SCAN_ENABLE_COMPLETE)
#define DM_HCI_WRITE_SCAN_ENABLE_COMPLETE               ((dm_prim_t)ENUM_DM_HCI_WRITE_SCAN_ENABLE_COMPLETE)
#define DM_HCI_READ_PAGESCAN_ACTIVITY_COMPLETE          ((dm_prim_t)ENUM_DM_HCI_READ_PAGESCAN_ACTIVITY_COMPLETE)
#define DM_HCI_WRITE_PAGESCAN_ACTIVITY_COMPLETE         ((dm_prim_t)ENUM_DM_HCI_WRITE_PAGESCAN_ACTIVITY_COMPLETE)
#define DM_HCI_READ_INQUIRYSCAN_ACTIVITY_COMPLETE       ((dm_prim_t)ENUM_DM_HCI_READ_INQUIRYSCAN_ACTIVITY_COMPLETE)
#define DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_COMPLETE      ((dm_prim_t)ENUM_DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_COMPLETE)
#define DM_HCI_READ_AUTH_ENABLE_COMPLETE                ((dm_prim_t)ENUM_DM_HCI_READ_AUTH_ENABLE_COMPLETE)
#define DM_HCI_READ_ENCRYPTION_MODE_COMPLETE            ((dm_prim_t)ENUM_DM_HCI_READ_ENCRYPTION_MODE_COMPLETE)
#define DM_HCI_READ_CLASS_OF_DEVICE_COMPLETE            ((dm_prim_t)ENUM_DM_HCI_READ_CLASS_OF_DEVICE_COMPLETE)
#define DM_HCI_WRITE_CLASS_OF_DEVICE_COMPLETE           ((dm_prim_t)ENUM_DM_HCI_WRITE_CLASS_OF_DEVICE_COMPLETE)
#define DM_HCI_READ_VOICE_SETTING_COMPLETE              ((dm_prim_t)ENUM_DM_HCI_READ_VOICE_SETTING_COMPLETE)
#define DM_HCI_WRITE_VOICE_SETTING_COMPLETE             ((dm_prim_t)ENUM_DM_HCI_WRITE_VOICE_SETTING_COMPLETE)
#define DM_HCI_READ_AUTO_FLUSH_TIMEOUT_COMPLETE         ((dm_prim_t)ENUM_DM_HCI_READ_AUTO_FLUSH_TIMEOUT_COMPLETE)
#define DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_COMPLETE        ((dm_prim_t)ENUM_DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_COMPLETE)
#define DM_HCI_READ_NUM_BCAST_TXS_COMPLETE              ((dm_prim_t)ENUM_DM_HCI_READ_NUM_BCAST_TXS_COMPLETE)
#define DM_HCI_WRITE_NUM_BCAST_TXS_COMPLETE             ((dm_prim_t)ENUM_DM_HCI_WRITE_NUM_BCAST_TXS_COMPLETE)
#define DM_HCI_READ_HOLD_MODE_ACTIVITY_COMPLETE         ((dm_prim_t)ENUM_DM_HCI_READ_HOLD_MODE_ACTIVITY_COMPLETE)
#define DM_HCI_WRITE_HOLD_MODE_ACTIVITY_COMPLETE        ((dm_prim_t)ENUM_DM_HCI_WRITE_HOLD_MODE_ACTIVITY_COMPLETE)
#define DM_HCI_READ_TX_POWER_LEVEL_COMPLETE             ((dm_prim_t)ENUM_DM_HCI_READ_TX_POWER_LEVEL_COMPLETE)
#define DM_HCI_SET_HC_TO_HOST_FLOW_COMPLETE             ((dm_prim_t)ENUM_DM_HCI_SET_HC_TO_HOST_FLOW_COMPLETE)
#define DM_HCI_HOST_NUM_COMPLETED_PACKETS_COMPLETE      ((dm_prim_t)ENUM_DM_HCI_HOST_NUM_COMPLETED_PACKETS_COMPLETE)
#define DM_HCI_READ_LINK_SUPERV_TIMEOUT_COMPLETE        ((dm_prim_t)ENUM_DM_HCI_READ_LINK_SUPERV_TIMEOUT_COMPLETE)
#define DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_COMPLETE       ((dm_prim_t)ENUM_DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_COMPLETE)
#define DM_HCI_READ_NUM_SUPPORTED_IAC_COMPLETE          ((dm_prim_t)ENUM_DM_HCI_READ_NUM_SUPPORTED_IAC_COMPLETE)
#define DM_HCI_READ_CURRENT_IAC_LAP_COMPLETE            ((dm_prim_t)ENUM_DM_HCI_READ_CURRENT_IAC_LAP_COMPLETE)
#define DM_HCI_WRITE_CURRENT_IAC_LAP_COMPLETE           ((dm_prim_t)ENUM_DM_HCI_WRITE_CURRENT_IAC_LAP_COMPLETE)
#define DM_HCI_READ_PAGESCAN_PERIOD_MODE_COMPLETE       ((dm_prim_t)ENUM_DM_HCI_READ_PAGESCAN_PERIOD_MODE_COMPLETE)
#define DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_COMPLETE      ((dm_prim_t)ENUM_DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_COMPLETE)
#define DM_HCI_READ_PAGESCAN_MODE_COMPLETE              ((dm_prim_t)ENUM_DM_HCI_READ_PAGESCAN_MODE_COMPLETE)
#define DM_HCI_WRITE_PAGESCAN_MODE_COMPLETE             ((dm_prim_t)ENUM_DM_HCI_WRITE_PAGESCAN_MODE_COMPLETE)
#define DM_HCI_HARDWARE_ERROR                           ((dm_prim_t)ENUM_DM_HCI_HARDWARE_ERROR)
#define DM_HCI_DATA_BUFFER_OVERFLOW                     ((dm_prim_t)ENUM_DM_HCI_DATA_BUFFER_OVERFLOW)
#define DM_HCI_MAX_SLOTS_CHANGE                         ((dm_prim_t)ENUM_DM_HCI_MAX_SLOTS_CHANGE)

#define DM_BB_BASE                              (ENUM_SEP_DM_BB_DOWN_FIRST + 1)
#define DM_BB_DOWN_MAX                          (ENUM_SEP_DM_BB_DOWN_LAST - 1)
#define DM_BB_MAX                               (ENUM_SEP_DM_BB_UP_LAST - 1)

/******************************************************************************
   Informational parameters
 *****************************************************************************/
/*-------------------------------------------------------------
Downstream primitives
---------------------------------------------------------------*/
#define DM_HCI_READ_LOCAL_VERSION               ((dm_prim_t) ENUM_DM_HCI_READ_LOCAL_VERSION)
#define DM_HCI_READ_LOCAL_FEATURES              ((dm_prim_t) ENUM_DM_HCI_READ_LOCAL_FEATURES)
#define DM_HCI_READ_COUNTRY_CODE                ((dm_prim_t) ENUM_DM_HCI_READ_COUNTRY_CODE)
#define DM_HCI_READ_BD_ADDR                     ((dm_prim_t) ENUM_DM_HCI_READ_BD_ADDR)

/*-------------------------------------------------------------
Upstream primitives
---------------------------------------------------------------*/
#define DM_HCI_READ_LOCAL_VERSION_COMPLETE      ((dm_prim_t)ENUM_DM_HCI_READ_LOCAL_VERSION_COMPLETE)
#define DM_HCI_READ_LOCAL_FEATURES_COMPLETE     ((dm_prim_t)ENUM_DM_HCI_READ_LOCAL_FEATURES_COMPLETE)
#define DM_HCI_READ_COUNTRY_CODE_COMPLETE       ((dm_prim_t)ENUM_DM_HCI_READ_COUNTRY_CODE_COMPLETE)
#define DM_HCI_READ_BD_ADDR_COMPLETE            ((dm_prim_t)ENUM_DM_HCI_READ_BD_ADDR_COMPLETE)

#define DM_IP_BASE	                            (ENUM_SEP_DM_INF_DOWN_FIRST + 1)
#define	DM_IP_DOWN_BASE							DM_IP_BASE
#define DM_IP_DOWN_MAX                          (ENUM_SEP_DM_INF_DOWN_LAST - 1)
#define DM_IP_UP_BASE							(ENUM_SEP_DM_INF_DOWN_LAST + 1)
#define DM_IP_MAX                               (ENUM_SEP_DM_INF_UP_LAST - 1)
#define DM_IP_UP_MAX							DM_IP_MAX

/******************************************************************************
   Status parameters
 *****************************************************************************/
/*-------------------------------------------------------------
Downstream primitives
---------------------------------------------------------------*/
#define DM_HCI_FAILED_CONTACT_COUNTER           ((dm_prim_t) ENUM_DM_HCI_FAILED_CONTACT_COUNTER)
#define DM_HCI_RESET_CONTACT_COUNTER            ((dm_prim_t) ENUM_DM_HCI_RESET_CONTACT_COUNTER)
#define DM_HCI_GET_LINK_QUALITY                 ((dm_prim_t) ENUM_DM_HCI_GET_LINK_QUALITY)
#define DM_HCI_READ_RSSI                        ((dm_prim_t) ENUM_DM_HCI_READ_RSSI)

/*-------------------------------------------------------------
Upstream primitives
---------------------------------------------------------------*/
#define DM_HCI_FAILED_CONTACT_COUNTER_COMPLETE  ((dm_prim_t)ENUM_DM_HCI_FAILED_CONTACT_COUNTER_COMPLETE)
#define DM_HCI_RESET_CONTACT_COUNTER_COMPLETE   ((dm_prim_t)ENUM_DM_HCI_RESET_CONTACT_COUNTER_COMPLETE)
#define DM_HCI_GET_LINK_QUALITY_COMPLETE        ((dm_prim_t)ENUM_DM_HCI_GET_LINK_QUALITY_COMPLETE)
#define DM_HCI_READ_RSSI_COMPLETE               ((dm_prim_t)ENUM_DM_HCI_READ_RSSI_COMPLETE)

#define DM_STATUS_BASE	                        (ENUM_SEP_DM_STATUS_DOWN_FIRST + 1)
#define DM_STATUS_DOWN_MAX                      (ENUM_SEP_DM_STATUS_DOWN_LAST - 1)
#define DM_STATUS_MAX                           (ENUM_SEP_DM_STATUS_UP_LAST - 1)

/******************************************************************************
   Testing primitives
 *****************************************************************************/
/*-------------------------------------------------------------
Downstream primitives
---------------------------------------------------------------*/
#define DM_HCI_ENABLE_DEVICE_UT_MODE            ((dm_prim_t) ENUM_DM_HCI_ENABLE_DEVICE_UT_MODE)

/*-------------------------------------------------------------
Upstream primitives
---------------------------------------------------------------*/
#define DM_HCI_ENABLE_DEVICE_UT_MODE_COMPLETE ((dm_prim_t)ENUM_DM_HCI_ENABLE_DEVICE_UT_MODE_COMPLETE)

#define DM_TEST_BASE	                      (ENUM_SEP_DM_TEST_DOWN_FIRST + 1)
#define DM_TEST_DOWN_MAX                      (ENUM_SEP_DM_TEST_DOWN_LAST - 1)
#define DM_TEST_MAX                           (ENUM_SEP_DM_TEST_UP_LAST - 1)

/* end of #define of primitives */

/* primitive type for dm */
typedef uint16_t                 dm_prim_t;

/*------------------------------------------------------------------------
 *
 *      REGISTER PRIMITIVES
 *
 *-----------------------------------------------------------------------*/

/* Note1:

	A value of 0 in the 'status' field of returned primitives indicates 
	success. All other values indicate failure. 
	The error codes are defined in the Bluetooth HCI specification.
	(www.bluetooth.com para. 6.1 in v1.0b)."
*/

 /*----------------------------------------------------------------------------*
 * PURPOSE
 *      Standard command complete
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t           type;               /* Prim ID */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* Success or failure - See Note1 */

} DM_HCI_STANDARD_COMMAND_COMPLETE_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Register request to AM Interface, registering a destination phandle
 *      for upstream application primitives.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_AM_REGISTER_REQ */
    phandle_t               phandle;        /* protocol handle */
} DM_AM_REGISTER_REQ_T;

typedef DM_AM_REGISTER_REQ_T DM_AM_REGISTER_WRONG_API_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Confirmation that the registration request has been received.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_AM_REGISTER_CFM */
    phandle_t               phandle;        /* destination phandle */
} DM_AM_REGISTER_CFM_T;

typedef DM_AM_REGISTER_CFM_T DM_AM_REGISTER_WRONG_API_CFM_T;

/*---------------------------------------------------------------------------*
 *  PURPOSE
 *      Set bluetooth version. On successful completion, status will
 *      be set to HCI_SUCCESS == 0 and version will be set to the
 *      BT version requested. On unsuccessful complettion, status will
 *      be set to an HCI error code and version will be set to the
 *      current version.
 *
 *      You can use this command to read the current BT version by
 *      setting version to BT_VERSION_CURRENT in the command primitive.
 *      (In this case a failure indication in the status parameter would
 *      be unusual and would indicate that the current version settings
 *      are no longer supported by the device.)
 *---------------------------------------------------------------------------*/

#define BT_VERSION_CURRENT          0x00
#define BT_VERSION_2p0              0x01
#define BT_VERSION_2p1              0x02
#define BT_VERSION_MAX              BT_VERSION_2p1

typedef struct
{
    dm_prim_t               type;           /* Always DM_SET_BT_VERSION_REQ */
    uint8_t                 version;        /* Requested version */
} DM_SET_BT_VERSION_REQ_T;

typedef struct
{
    dm_prim_t               type;           /* Always DM_SET_BT_VERSION_CFM */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 version;        /* Requested version if success, */
    uint8_t                 status;         /* or current version if failure */
} DM_SET_BT_VERSION_CFM_T;

/*------------------------------------------------------------------------
 *
 *      ACL CONNECTION INTERFACE PRIMITIVES
 *      These primitives are all sent to the DM_ACLQUEUE input queue, and
 *      are intended for use only by L2CAP.
 *
 *-----------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Register request to ACL interface, registering a destination phandle
 *      for all upstream ACL connection-related primitives (expected to be
 *      L2CAP).
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t               type;           /* Always DM_ACL_REGISTER_REQ */
    phandle_t               phandle;        /* protocol handle */
} DM_ACL_REGISTER_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Confirmation that the registration request has been received.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_ACL_REGISTER_CFM */
    phandle_t               phandle;        /* destination phandle */
} DM_ACL_REGISTER_CFM_T;

/* TODO Find somewhere to put this... */
/*---------------------------------------------------------------------------*
 * PURPOSE
 *      Service structure.
 *
 *---------------------------------------------------------------------------*/
typedef uint32_t dm_protocol_id_t;
typedef struct
{
    dm_protocol_id_t protocol_id;
    uint32_t channel;
} DM_SM_SERVICE_T; /* autogen_makefile_ignore_this */


/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request from L2CAP to create ACL connection ro remote Bluetooth device.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_ACL_CONNECT_REQ */
    BD_ADDR_T               bd_addr;        /* Bluetooth device address */
    hci_pkt_type_t          packet_type;    /* Set to zero for default */

    /* Protocol and channel for the protocol layer requesting access */
    DM_SM_SERVICE_T         service;
} DM_ACL_CONNECT_REQ_T; /* autogen_makefile_ignore_this */

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      ACL connect confirmation to L2CAP for connection to remote bluetooth
 *      device.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t               type;           /* Always DM_ACL_CONNECT_CFM */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    hci_connection_handle_t handle;         /* Connection handle */
    BD_ADDR_T               bd_addr;        /* Bluetooth device address */
	uint8_t                 enc_mode;       /* Encryption mode (point to point, etc) */
} DM_ACL_CONNECT_CFM_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      ACL connect indication to L2CAP from remote bluetooth device.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t               type;           /* Always DM_ACL_CONNECT_IND */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    hci_connection_handle_t handle;         /* Connection handle */
    BD_ADDR_T               bd_addr;        /* Bluetooth device address */
	uint8_t                 enc_mode;       /* Encryption mode (point to point, etc) */
} DM_ACL_CONNECT_IND_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Disconnect request from L2CAP for a particular ACL connection handle.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t                   type;           /* Always DM_ACL_DISCONNECT_REQ */
    hci_connection_handle_t     handle;         /* HCI Connection Handle */
    hci_reason_t                reason;         /* reason for disconnection */
} DM_ACL_DISCONNECT_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Disconnect indicate to L2CAP for a particular ACL connection handle.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t                   type;           /* Always DM_ACL_DISCONNECT_IND */
    phandle_t                   phandle;        /* destination phandle */
    bool_t                      reconnecting;   /* TRUE if we're trying to reconnect. */
    BD_ADDR_T                   bd_addr;        /* Bluetooth device address */
    hci_reason_t                reason;         /* reason for disconnection */
} DM_ACL_DISCONNECT_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      An indication of Host Controller buffer sizes.
 *      This is sent once only on start-up to each ACL interface
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t                   type;           /* Always DM_ACL_BUFFER_SIZE_IND */
    phandle_t                   phandle;        /* destination phandle */
    uint16_t                    size;           /* Max size of data packet */
    uint16_t                    max_num_packets;/* Max num packets on host */
} DM_ACL_BUFFER_SIZE_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Data sent indication for each ACL Interface 
 *      Sent upon receiving hci_number_completed_packets event
 *
 *		See defn of HCI_EV_NUMBER_COMPLETED_PKTS_T for details.
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t                   type;               /* Always DM_ACL_DATA_SENT_IND */
    phandle_t                   phandle;            /* destination phandle */
    uint8_t                     num_handles;		/* Number of handles */
	/* handle plus number of packets */
    HANDLE_COMPLETE_T *			handle_complete_ptr[HCI_EV_NUM_HANDLE_COMPLETE_PACKET_PTRS];
} DM_ACL_DATA_SENT_IND_T;

/*------------------------------------------------------------------------
 *
 *      SYNCHRONOUS CONNECTION INTERFACE PRIMITIVES
 *      These primitives are all sent to the DM_IFACEQUEUE input queue.
 *
 *-----------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t               type;           /* Always DM_SYNC_REGISTER_REQ */
    phandle_t               phandle;        /* protocol handle */

    /* Opaque argument to be returned in all upstream SCO primitives, can be
     * used by the destination task for discrimination purposes.
     */
    uint32_t                pv_cbarg;
} DM_SYNC_REGISTER_REQ_T;

typedef struct
{
    dm_prim_t               type;           /* Always DM_SYNC_REGISTER_CFM */
    phandle_t               phandle;        /* protocol handle */
    uint32_t                pv_cbarg;
} DM_SYNC_REGISTER_CFM_T;

typedef struct
{
    dm_prim_t               type;           /* Always DM_SYNC_UNREGISTER_REQ */
    phandle_t               phandle;        /* client phandle */
    uint32_t                pv_cbarg;
} DM_SYNC_UNREGISTER_REQ_T;

typedef struct
{
    dm_prim_t               type;           /* Always DM_SYNC_UNREGISTER_CFM */
    phandle_t               phandle;        /* client phandle */
    uint32_t                pv_cbarg;
} DM_SYNC_UNREGISTER_CFM_T;

typedef struct
{
    dm_prim_t               type;           /* Always DM_SYNC_CONNECT_REQ */
    phandle_t               phandle;        /* client phandle */
    uint32_t                pv_cbarg;
    BD_ADDR_T               bd_addr;        /* Bluetooth device address */
    uint32_t                tx_bdw;
    uint32_t                rx_bdw;
    uint16_t                max_latency;
    uint16_t                voice_settings;
    uint8_t                 retx_effort;
    hci_pkt_type_t          packet_type;    /* Set to zero for default */
} DM_SYNC_CONNECT_REQ_T;

typedef struct
{
    dm_prim_t               type;           /* Always DM_SYNC_CONNECT_CFM */
    phandle_t               phandle;        /* client phandle */
    uint32_t                pv_cbarg;
    uint8_t                 status;         /* Success or failure - See Note1 */
    hci_connection_handle_t handle;         /* Connection handle */
    uint8_t                 lm_sco_handle;  /* LM SCO handle, or HCI_LM_SCO_HANDLE_INVALID if unknown */
    BD_ADDR_T               bd_addr;        /* Bluetooth device address */
    uint8_t                 link_type;
    uint8_t                 tx_interval;
    uint8_t                 wesco;
    uint16_t                rx_packet_length;
    uint16_t                tx_packet_length;
    uint8_t                 air_mode;
} DM_SYNC_CONNECT_COMPLETE_T; /* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */
typedef DM_SYNC_CONNECT_COMPLETE_T DM_SYNC_CONNECT_CFM_T;
typedef DM_SYNC_CONNECT_COMPLETE_T DM_SYNC_CONNECT_COMPLETE_IND_T;

typedef struct
{
    dm_prim_t               type;           /* Always DM_SYNC_CONNECT_IND */
    phandle_t               phandle;        /* client phandle */
    uint32_t                pv_cbarg;
    BD_ADDR_T               bd_addr;        /* Bluetooth device address */
} DM_SYNC_CONNECT_IND_T;

typedef struct
{
    dm_prim_t               type;           /* Always DM_SYNC_CONNECT_RES */
    BD_ADDR_T               bd_addr;        /* Bluetooth device address */
    bool_t                  accept;         /* TRUE to accept the connection */
    uint32_t                tx_bdw;
    uint32_t                rx_bdw;
    uint16_t                max_latency;
    uint16_t                voice_settings;
    uint8_t                 retx_effort;
    hci_pkt_type_t          packet_type;    /* Set to zero for default */
    hci_error_t             reason;         /* Reason for reject */
} DM_SYNC_CONNECT_RES_T;

typedef struct
{
    dm_prim_t                   type;           /* Always DM_SYNC_RENEGOTIATE_REQ */
    hci_connection_handle_t     handle;
    uint16_t                    max_latency;
    uint8_t                     retx_effort;
    hci_pkt_type_t              packet_type;
} DM_SYNC_RENEGOTIATE_REQ_T;

typedef struct
{
    dm_prim_t                   type;           /* Always DM_SYNC_RENEGOTIATE_IND */
    phandle_t                   phandle;        /* destination phandle */
    uint32_t                    pv_cbarg;
    hci_return_t                status;
    hci_connection_handle_t     handle;
} DM_SYNC_RENEGOTIATE_IND_T;

typedef struct
{
    dm_prim_t                   type;           /* Always DM_SYNC_RENEGOTIATE_CFM */
    phandle_t                   phandle;        /* destination phandle */
    uint32_t                    pv_cbarg;
    hci_return_t                status;
    hci_connection_handle_t     handle;
} DM_SYNC_RENEGOTIATE_CFM_T;

typedef struct
{
    dm_prim_t                   type;           /* Always DM_SYNC_DISCONNECT_REQ */
    hci_connection_handle_t     handle;         /* HCI Connection Handle */
    hci_reason_t                reason;         /* reason for disconnection */
} DM_SYNC_DISCONNECT_REQ_T;

typedef struct
{
    dm_prim_t                   type;           /* Always DM_SYNC_DISCONNECT_IND */
    phandle_t                   phandle;        /* destination phandle */
    uint32_t                    pv_cbarg;
    uint8_t                     status;         /* Success or failure - See Note1 */
    hci_connection_handle_t     handle;         /* HCI Connection Handle */
    hci_reason_t                reason;         /* reason for disconnection */
    BD_ADDR_T                   bd_addr;        /* Bluetooth device address */
} DM_SYNC_DISCONNECT_IND_T;

typedef struct
{
    dm_prim_t                   type;           /* Always DM_SYNC_DISCONNECT_CFM */
    phandle_t                   phandle;        /* destination phandle */
    uint32_t                    pv_cbarg;
    uint8_t                     status;         /* Success or failure - See Note1 */
    hci_connection_handle_t     handle;         /* HCI Connection Handle */
    BD_ADDR_T                   bd_addr;        /* Bluetooth device address */
} DM_SYNC_DISCONNECT_CFM_T;





































































































/*----------------------------------------------------------------------------*
 * MusiCore (MCDSP) primitive definitions
 *----------------------------------------------------------------------------*/
 
#ifdef INSTALL_DYNAMIC_TOKEN_HANDLING

typedef struct
{
    dm_prim_t type;     /* Always DM_MC_READ_CHANNEL_INFO_REQ */
    uint16_t  host_id;
    BD_ADDR_T bd_addr;  /* Bluetooth device address */

} DM_MC_READ_CHANNEL_INFO_REQ_T;

typedef struct
{
    dm_prim_t type;            /* Always DM_MC_READ_CHANNEL_INFO_CFM */
    uint16_t  host_id;
    BD_ADDR_T bd_addr;               /* Bluetooth device address */
    hci_connection_handle_t handle;  /* ACL connection handle */
    uint16_t  max_acl_pkt_size;
    uint16_t  max_avail_tokens;  

} DM_MC_READ_CHANNEL_INFO_CFM_T;

typedef struct
{
    dm_prim_t type;       /* Always DM_MC_RESERVE_TOKENS_REQ */
    uint16_t  host_id;
    uint16_t  num_tokens;

} DM_MC_RESERVE_TOKENS_REQ_T;

typedef struct
{
    dm_prim_t type;    /* Always DM_MC_RESERVE_TOKENS_CFM */
    uint16_t  host_id;
    bool_t    status;  /* True if tokens were reserved, False otherwise */

} DM_MC_RESERVE_TOKENS_CFM_T;

typedef struct
{
    dm_prim_t type;     /* Always DM_MC_RETURN_TOKENS_REQ */
    uint16_t  host_id;
    uint16_t  num_tokens;

} DM_MC_RETURN_TOKENS_REQ_T;

typedef struct
{
    dm_prim_t type;     /* Always DM_MC_RETURN_TOKENS_CFM */
    uint16_t  host_id;

} DM_MC_RETURN_TOKENS_CFM_T;

#endif 



/*----------------------------------------------------------------------------*
 *
 *  SECURITY MANAGEMENT INTERFACE
 *
 *  Downstream primitives are sent to the DM_IFACEQUEUE, upstream primitives
 *  are sent either to the registered AM queue (see DM_AM_REGISTER_REQ), or to
 *  the queue of the protocol layer requesting access (for DM_SM_ACCESS_REQ).
 *  There is a private L2CAP/SM interface for controlling security of received
 *  connectionless packets.
 *
 *----------------------------------------------------------------------------*/

typedef uint8_t dm_security_mode_t;
#define SEC_MODE0_OFF                   ((dm_security_mode_t) 0x00)
#define SEC_MODE1_NON_SECURE            ((dm_security_mode_t) 0x01)
#define SEC_MODE2_SERVICE               ((dm_security_mode_t) 0x02)
#define SEC_MODE3_LINK                  ((dm_security_mode_t) 0x03)
#define SEC_MODE4_SSP                   ((dm_security_mode_t) 0x04)
/*
   SEC_MODE2a_DEVICE is a custom security mode that authenticates on a per
   device basis using the Security Manager's device database.
   On starting a new service (incoming and outgoing), the device will be
   authenticated unless it is marked as 'trusted' in the device database
   (see DM_SM_ADD_DEVICE_REQ).
*/
#define SEC_MODE2a_DEVICE		((dm_security_mode_t) 0x80)

/* Security level settings
 *
 * SECL_(IN|OUT)_AUTHORISATION (both mode 4 and legacy)
 * Results in a DM_SM_AUTHORISE_IND on a connection attempt from a
 * device that is not marked as 'trusted' in the device database.
 *
 * SECL_(IN|OUT)_AUTHENTICATION (legacy)
 * For connections where at least one of the devices is not in mode 4
 * (i.e. it's a legacy device) then the legacy pairing procedure
 * should be used. The users may have to enter PINs.
 *
 * SECL_(IN|OUT)_ENCRYPTION (legacy)
 * This setting indicates that the link should be encrypted after legacy
 * authentication.
 *
 * SECL4_(IN|OUT)_SSP (mode 4)
 * For connections where both devices are in mode 4, Secure Simple Pairing
 * is used to generate a shared link key and the link is then encrypted. Only
 * SDP is allowed not to have these bits set. All other services must set
 * them to achieve compliance.
 *
 * SECL4_(IN|OUT)_MITM (mode 4)
 * For connections where both devices are in mode 4, Secure Simple Pairing
 * is used to generate a shared authenticated link key and the link is
 * then encrypted. If you set this then you should also set
 * SECL4_(IN|OUT)_SSP as it is implied.
 *
 * SECL4_(IN|OUT)_NO_LEGACY
 * Don't support legacy security procedures. This is a mode4-only setting.
 * By setting this, you are preventing the device from using legacy pairing
 * procedures to authenticate with remote devices that do not support
 * simple pairing. If you set this then the device will be
 * incapable of pairing using legacy procedures and so will reject all
 * connection attempts with legacy devices where security is required.
 * If you set this then you may severely reduce interopability.
 *
 */

typedef uint16_t dm_security_level_t;
#define SECL_NONE                           ((dm_security_level_t) 0x0000)
#define SECL_IN_AUTHORISATION               ((dm_security_level_t) 0x0001)
#define SECL_IN_AUTHENTICATION              ((dm_security_level_t) 0x0002)
#define SECL_IN_ENCRYPTION                  ((dm_security_level_t) 0x0004)
#define SECL_OUT_AUTHORISATION              ((dm_security_level_t) 0x0008)
#define SECL_OUT_AUTHENTICATION             ((dm_security_level_t) 0x0010)
#define SECL_OUT_ENCRYPTION                 ((dm_security_level_t) 0x0020)
#define SECL_IN_CONNECTIONLESS              ((dm_security_level_t) 0x0040)

#define SECL4_IN_SSP                        ((dm_security_level_t) 0x0100)
#define SECL4_IN_MITM                       ((dm_security_level_t) 0x0200)
#define SECL4_IN_NO_LEGACY                  ((dm_security_level_t) 0x0400)
#define SECL4_OUT_SSP                       ((dm_security_level_t) 0x0800)
#define SECL4_OUT_MITM                      ((dm_security_level_t) 0x1000)
#define SECL4_OUT_NO_LEGACY                 ((dm_security_level_t) 0x2000)

/* 
 * Security Levels defined in Vol 3, Part C, Section 5.2.2.5 of BT2.1 Spec
 *
 * N.B. These only affect authentication and encryption. Authorisation is
 *      separate.
 */
/* No security - permitted for SDP only */
#define SECL4_IN_LEVEL_0    SECL_NONE
#define SECL4_OUT_LEVEL_0   SECL_NONE
#define SECL4_LEVEL_0       (SECL4_IN_LEVEL_0 | SECL4_OUT_LEVEL_0)
/* Low security */
#define SECL4_IN_LEVEL_1    SECL4_IN_SSP
#define SECL4_OUT_LEVEL_1   SECL4_OUT_SSP
#define SECL4_LEVEL_1       (SECL4_IN_LEVEL_1 | SECL4_OUT_LEVEL_1)
/* Medium security */
#define SECL4_IN_LEVEL_2    (SECL4_IN_SSP | SECL_IN_AUTHENTICATION \
                                                        | SECL_IN_ENCRYPTION)
#define SECL4_OUT_LEVEL_2   (SECL4_OUT_SSP | SECL_OUT_AUTHENTICATION \
                                                        | SECL_OUT_ENCRYPTION)
#define SECL4_LEVEL_2       (SECL4_IN_LEVEL_2 | SECL4_OUT_LEVEL_2)
/* High security */
#define SECL4_IN_LEVEL_3    (SECL4_IN_SSP | SECL4_IN_MITM \
                               | SECL_IN_AUTHENTICATION | SECL_IN_ENCRYPTION)
#define SECL4_OUT_LEVEL_3    (SECL4_OUT_SSP | SECL4_OUT_MITM \
                               | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION)
#define SECL4_LEVEL_3       (SECL4_IN_LEVEL_3 | SECL4_OUT_LEVEL_3)
/* 
 * Security level combinations useful internally
 */
/* Incoming security settings */
#define SECL_IN    (SECL_IN_AUTHORISATION | SECL_IN_AUTHENTICATION      \
        | SECL_IN_CONNECTIONLESS |SECL_IN_ENCRYPTION | SECL4_IN_SSP     \
        | SECL4_IN_MITM | SECL4_IN_NO_LEGACY)

/* Outgoing security settings */
#define SECL_OUT   (SECL_OUT_AUTHORISATION | SECL_OUT_AUTHENTICATION    \
        | SECL_OUT_ENCRYPTION | SECL4_OUT_SSP | SECL4_OUT_MITM          \
        | SECL4_OUT_NO_LEGACY )

/* Authorisation required (either in or out) */
#define SECL_AUTHORISATION (SECL_IN_AUTHORISATION | SECL_OUT_AUTHORISATION)

/* Authentication required (legacy) */
#define SECL_AUTHENTICATION (SECL_IN_AUTHENTICATION \
        | SECL_OUT_AUTHENTICATION | SECL_IN_ENCRYPTION | SECL_OUT_ENCRYPTION)

/* Encryption required (legacy) */
#define SECL_ENCRYPTION (SECL_IN_ENCRYPTION | SECL_OUT_ENCRYPTION)

/* Simple Secure Pairing required (either in or out) */
#define SECL4_SSP (SECL4_IN_SSP | SECL4_OUT_SSP | SECL4_IN_MITM \
                                                            | SECL4_OUT_MITM)

/* MITM protection required in mode 4 (either in or out) */
#define SECL4_MITM (SECL4_IN_MITM | SECL4_OUT_MITM)

/* Legacy pairing prohibited in mode 4 (either in or out) */
#define SECL4_NO_LEGACY (SECL4_IN_NO_LEGACY | SECL4_OUT_NO_LEGACY)

/* Mask of legacy mode settings */
#define SECL_MODE_LEGACY                ((dm_security_level_t) 0x007F)

/* The default security level, applied in security modes 2, 3 and 4 when no
 * specific security level has been registered for a service. This is set to
 * require authentication and encryption of both incoming and outgoing
 * connections. It also requires authorisation of incoming connections.
 * It may be overridden at runtime using the DM_SET_DEFAULT_SECURITY_REQ
 * primitive.
 */
#define SECL_DEFAULT (SECL_IN_AUTHORISATION | SECL4_LEVEL_2)

/* Standard security protocol identifiers provided by BlueStack. Higher
 * multiplexing protocols must define their own identifiers, from the range
 * SEC_PROTOCOL_USER and above.
 */
#define SEC_PROTOCOL_L2CAP              ((dm_protocol_id_t) 0x0000)
#define SEC_PROTOCOL_RFCOMM             ((dm_protocol_id_t) 0x0001)
/* Higher layers base their protocol IDs from here */
#define SEC_PROTOCOL_USER               ((dm_protocol_id_t) 0x8000)

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Sets the default security level to be applied in security modes 2, 3
 *      and 4 in cases where a service has not been registered with the
 *      Security Manager. The "default" default security level is defined by
 *      SECL_DEFAULT, and using this primitive overrides SECL_DEFAULT.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_REGISTER_REQ */

    dm_security_level_t secl_default;       /* New default security level */
} DM_SM_SET_DEFAULT_SECURITY_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Registers the security requirements for access to a service or a
 *      multiplexing protocol layer when the Security Manager is in security
 *      mode 2 or 3. The registered security level is applied to all incoming
 *      connections on the specified 'channel', and optionally to all outgoing
 *      connections on that channel if 'outgoing_ok' is TRUE.
 *
 *      It does not always make sense to apply security to outgoing connections,
 *      for example a local RFCOMM server channel is only valid for incoming
 *      connections. An L2CAP PSM, however, may be valid in both directions.
 *      Therefore use the 'outgoing_ok' flag to specify whether security
 *      should be applied to outgoing connections as well as incoming.
 *      When the local and remote L2CAP PSMs are different, the local PSM should
 *      be passed to the Security Manager.  For outgoing connections where the
 *      channel may be different for each remote device, security requirements
 *      may be registered using the DM_SM_REGISTER_OUTGOING_REQ primitive.
 *
 *      NOTE
 *          The Security Manager enforces a default security level in modes 2,
 *          3 and 4 given by the SECL_DEFAULT definition. Any access request
 *          for which there is no registered security level will be subject to
 *          the default security. Therefore it is recommended that certain
 *          services and protocols are always registered with no security,
 *          for example, SDP and RFCOMM. In Security mode 4, no service, other
 *          than SDP, should be registered with no security.
 *
 *      Some security examples are presented:
 *
 *      Eg 1 - A TCS Terminal registers with 'protocol_id' = SEC_PROTOCOL_L2CAP,
 *      'channel' = 7 (cordless telephony PSM), and 'outgoing_ok' = TRUE. Then
 *      L2CAP will request access from the Security Manager for all connections,
 *      incoming and outgoing, on PSM 7.
 *
 *      Eg 2 - A headset profile might register with 'protocol_id' =
 *      SEC_PROTOCOL_RFCOMM, 'channel' = the RFCOMM server channel number of the
 *      headset service, and 'outgoing_ok' = FALSE. Then RFCOMM will request
 *      access from the Security Manager for all incoming connections on that
 *      server channel. Access requests for outgoing connections will be
 *      subjected to the default security level.
 *
 *      Eg 3 - As for (Eg 2), but whenever the profile makes an outgoing
 *      connection to an Audio Gateway it first performs service discovery to
 *      obtain the server channel number on the AG device, then registers using
 *      the DM_SM_REGISTER_OUTGOING_REQ primitive with 'protocol_id' =
 *      SEC_PROTOCOL_RFCOMM, 'channel' = remote server channel, 'bd_addr' =
 *      device address of the AG. When the outgoing connection is started,
 *      RFCOMM will request access from the Security Manager which will apply
 *      the security level registered for the outgoing connection. Incoming
 *      connections proceed as in (Eg 2).
 *
 *      Eg 4 - An application can specify the security requirements for access
 *      to RFCOMM by sending DM_SM_REGISTER_REQ with 'protocol_id' =
 *      SEC_PROTOCOL_L2CAP, 'channel' = 3, and 'outgoing_ok' = TRUE. Then L2CAP
 *      will request access from the Security Manager for all new RFCOMM
 *      connections.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_REGISTER_REQ */

    /* Protocol at which security is to be applied */
    dm_protocol_id_t    protocol_id;

    /* Channel for that protocol (e.g. RFCOMM server channel number) */
    uint32_t            channel;

    /* TRUE if this registration applies to outgoing connections in addition
     * to incoming connections.
     */
    bool_t              outgoing_ok;

    /* Level of security to be applied */
    dm_security_level_t security_level;

    /* The L2CAP PSM number is required if connectionless security is used. If
     * there is a conflict, where multiple services based on the same PSM have
     * different connectionless security requirements, then connectionless
     * reception will be disabled for that PSM.
     *
     * If connectionless security does not matter, set 'psm' to zero.
     */
    psm_t               psm;
} DM_SM_REGISTER_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Registers the security requirements for outgoing connections to the
 *      specified protocol and channel on the specified remote device. This is
 *      typically used to control security when connecting to a remote RFCOMM
 *      server channel.
 *
 *      See the description of DM_SM_REGISTER_REQ (above) for more details.
 /
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_REGISTER_OUTGOING_REQ */

    /* The address of the device to which the given 'channel' applies. For
     * example, RFCOMM server channels may be different on each device.
     */
    BD_ADDR_T           bd_addr;

    /* Protocol at which security is to be applied */
    dm_protocol_id_t    protocol_id;

    /* Remote channel for that protocol (e.g. RFCOMM server channel number) */
    uint32_t            remote_channel;

    /* Level of security to be applied - outgoing and connectionless only */
    dm_security_level_t outgoing_security_level;

    /* The L2CAP PSM number is required if connectionless security is used. If
     * there is a conflict, where multiple services based on the same PSM have
     * different connectionless security requirements, then connectionless
     * reception will be disabled for that PSM.
     *
     * If connectionless security does not matter, set 'psm' to zero.
     */
    psm_t               psm;
} DM_SM_REGISTER_OUTGOING_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Unregisters the security requirements for a service previously
 *      registered with DM_SM_REGISTER_REQ. 'protocol_id' and 'channel' are the
 *      same as in the register request.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_UNREGISTER_REQ */

    /* Protocol and channel numbers previously registered */
    dm_protocol_id_t    protocol_id;
    uint32_t            channel;
} DM_SM_UNREGISTER_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Unregisters the security requirements for a service previously
 *      registered with DM_SM_REGISTER_OUTGOING_REQ. 'bd_addr', 'protocol_id'
 *      and 'remote_channel' are the same as in the register request.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_UNREGISTER_OUTGOING_REQ */

    /* Protocol and channel numbers previously registered */
    BD_ADDR_T           bd_addr;
    dm_protocol_id_t    protocol_id;
    uint32_t            remote_channel;
} DM_SM_UNREGISTER_OUTGOING_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request from a protocol layer (e.g. L2CAP, RFCOMM, or a higher layer) to
 *      determine if the remote device 'bd_addr' is to be granted access to that
 *      protocol layer's channel.
 *      The Security Manager will search its service and device databases, may
 *      perform some security procedures, and may request authorisation from
 *      the application. The result is returned in a DM_SM_ACCESS_CFM_T
 *      primitive.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_ACCESS_REQ */
    phandle_t           phandle;            /* phandle for the response */

    /* BD address of the remote device */
    BD_ADDR_T           bd_addr;

    /* Protocol and channel for the protocol layer requesting access */
    dm_protocol_id_t    protocol_id;
    uint32_t            channel;

    /* TRUE for incoming connection, FALSE for outgoing connection */
    bool_t              incoming;

    /* An opaque value supplied by the requesting entity, and returned in the
     * access confirm primitive. It may be used to match a confirm to the
     * correct request.
     */
    uint32_t            pv_context;
    uint32_t            n_context;
} DM_SM_ACCESS_REQ_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Response to an access request from a protocol layer. This is sent to
 *      the phandle that was sent in the DM_SM_ACCESS_REQ primitive.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_ACCESS_CFM */
    phandle_t           phandle;            /* Destination phandle */

    /* The parameters from the access request */
    BD_ADDR_T           bd_addr;
    dm_protocol_id_t    protocol_id;
    uint32_t            channel;
    bool_t              incoming;

    /* Anything other than HCI_SUCCESS is a failure */
    hci_return_t        status;

    /* Context value supplied in the request */
    uint32_t            pv_context;
    uint32_t            n_context;
} DM_SM_ACCESS_CFM_T;

typedef DM_SM_ACCESS_CFM_T DM_SM_ACCESS_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request to place the security manager into the specified security mode.
 *      The result is returned in a DM_SM_SET_SEC_MODE_CFM primitive. It is
 *      recommended that the security mode is changed only when the system is
 *      not in the process of creating or accepting any new connections.
 *
 *      Mode 0 is for use by applications which perform their own security
 *      procedures. The service database is unused in this mode, whereas the
 *      device database is used only for link keys. Unknown link keys result in
 *      a link key request being forwarded to the application. This is the
 *      default mode at startup.
 *
 *      In mode 1, the Security Manager automatically accepts all access
 *      requests without initiating authentication, encryption, or
 *      authorisation procedures. Any attempt by an application to perform
 *      authentication or encryption procedures will be rejected. The Security
 *      Manager will respond to peer-invoked security procedures as for mode 2.
 *
 *      In mode 2, access requests are subject to the security requirements
 *      registered in the service and device databases. The Security Manager
 *      will respond to link key requests using the device database - if a link
 *      key is unknown, the request is forwarded to the application. PIN code
 *      requests are forwarded to the application. The application may invoke
 *      security procedures in this mode.
 *
 *      In mode 3, the host controller is configured to perform authentication
 *      and possibly encryption at LMP link setup. The Security Manager still
 *      uses the service database to control authorisation and encryption.
 *      The encryption level for mode 3 is specified in 'mode3_enc'. If the host
 *      controller will not enter authentication mode, then the request fails
 *      (DM_SM_SET_SEC_MODE_CFM has a negative response). If the host controller
 *      does enter authentication mode, then the request succeeds. Note that the
 *      requested encryption level may not be supported - in this case the
 *      request still succeeds and the actual encryption level is returned in
 *      DM_SM_SET_SEC_MODE_CFM.
 *
 *      In mode 4, security is service-based, like in mode 2. Secure simple
 *      pairing is turned on in mode 4 and we now do authorisation before
 *      authentication. A device must be put into mode 4 to be V2.1 compliant
 *      Once in mode 4, you cannot change to any other mode.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_SET_SEC_MODE_REQ */

    /* Which security mode to use */
    dm_security_mode_t  mode;

    /* Encryption level for mode 3. One of:
     *  HCI_ENC_MODE_OFF
     *  HCI_ENC_MODE_PT_TO_PT
     *  HCI_ENC_MODE_PT_TO_PT_AND_BCAST
     */
    uint8_t         mode3_enc;
} DM_SM_SET_SEC_MODE_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Response to a security mode change request. A mode change may fail if
 *      an unknown mode was requested, or if the host controller refused to
 *      enable/disable authentication at LMP link setup.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_SET_SEC_MODE_CFM */
    phandle_t           phandle;            /* Destination phandle */

    /* The current security mode and the actual encryption level
     * in use for mode 3.
     */
    dm_security_mode_t  mode;
    uint8_t mode3_enc;

    /* TRUE if the requested security mode has been activated */
    bool_t success;
} DM_SM_SET_SEC_MODE_CFM_T;

/*---------------------------------------------------------------------------*
 * PURPOSE
 *      Set security mode configuration parameters.
 *
 *      write_auth_enable
 *          This controls when the security manager issues WRITE_AUTH_ENABLE
 *          to do mode3-style pairing with legacy devices.
 *          See DM_SM_BONDING_REQ for a description of this parameter.
 *
 *      config
 *          This is a bitfield for general config.
 *
 *          bit 0 - DM_SM_SEC_MODE_CONFIG_SEND_ACCESS_IND
 *          If set then the security manager sends DM_SM_ACCESS_IND
 *          primitives to the application whenever it sends DM_SM_ACCESS_CFM
 *          to L2CAP or some higher level protocol. An application might
 *          want to receive these as an indication that service-based pairing
 *          has finished. Set to TRUE to receive these primitives.
 *
 *          bit 1 - DM_SM_SEC_MODE_CONFIG_DEBUG_MODE
 *          If set then the security manager will be put into debug mode.
 *          HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE will be issued to the
 *          controller and the security manager will start accepting
 *          link keys with debug type. You can exit debug mode by
 *          resending this primitive with this bit unset.
 *
 *          bit 2 - DM_SM_SEC_MODE_CONFIG_LEGACY_AUTO_PAIR_MISSING_LINK_KEY
 *          If set then the security manager will automatically start
 *          pairing if authentication of a legacy device fails with 
 *          'missing link key'.
 *
 *          bit 3 - DM_SM_SEC_MODE_CONFIG_DONT_ABORT_BONDING_FOR_MITM 
 *          If set then the device manager will not abort dedicated bonding
 *          just because our requirements for an authenticated link key are 
 *          not possible to achieve, given the results of the IO capabilities
 *          exchange. You probably want to set this or else you might not be
 *          able to bond with some devices if you have any services that
 *          require man-in-the-middle protection. If you set this then you
 *          will be able to bond with remote devices irrespective of their
 *          IO capabilites, but an unauthenticated link key will be produced
 *          if it's not possible to get an authenticated one. Any subsequent
 *          attempt to access a service requiring an authenticated link key
 *          will trigger re-pairing, which will fail if the IO capabilities
 *          remain unchanged.
 *
 *          bit 4 - DM_SM_SEC_MODE_CONFIG_LOCAL_USER_CONFIRMATION
 *          If set then the device manager will issue a
 *          DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND primitive when the
 *          local device has DISPLAY_YES_NO IO capabilities and the remote
 *          device has DISPLAY_ONLY or NO_INPUT_NO_OUTPUT. If this bit is
 *          not set then the device manager will automatically accept. This
 *          bit should be set to achieve full BT2.1 compliance.
 *
 *          bit 5 - DM_SM_SEC_MODE_CONFIG_NEVER_AUTO_ACCEPT
 *          Setting this bit is not recommended and should not be necessary.
 *          If set then the device manager will always issue a 
 *          DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND primitive instead of
 *          auto-accepting, irrespective of the local IO capabilities or
 *          the authentication requirements of either device. If you set
 *          this bit then you must also set 
 *          DM_SM_SEC_MODE_CONFIG_LOCAL_USER_CONFIRMATION.
 *
 *          bit 6 - DM_SM_SEC_MODE_CONFIG_HCI_ERRORS_FOR_BONDING
 *          Setting this bit will force the Security Manager to use HCI
 *          error codes in DM_SM_BONDING_CFM. An additional error code is
 *          defined for the case where bonding is cancelled locally.
 *          See DM_SM_BONDING_CFM.
 *
 *  The security manager will respond with DM_SM_SEC_MODE_CONFIG_CFM. If
 *  status != HCI_SUCCESS then some part failed. At present, the only part
 *  that can fail is entering or leaving debug mode. DM_SM_SEC_MODE_CONFIG_CFM
 *  will return the new values of write_auth_enable and config.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;           /* Always DM_SM_SEC_MODE_CONFIG_REQ */
    uint16_t            write_auth_enable; /* See DM_SM_BONDING_REQ */
    uint16_t            config;
} DM_SM_SEC_MODE_CONFIG_REQ_T;

/*
 * write_auth_enable (bitfield)
 */

/* Never pair during link setup. Use this if you know that the remote host
 * supports SSP and SM4 or if you plan to avoid timeouts some other way
 */
#define DM_SM_WAE_NEVER             0x0000

/* Pair during link setup if an ACL does not exist */
#define DM_SM_WAE_ACL_NONE          0x0001

/* Pair during link setup, first tearing down any existing ownerless ACL
 * This implies DM_SM_WAE_ACL_NONE
 */
#define DM_SM_WAE_ACL_OWNER_NONE    0x0002

/* Pair during link setup, first tearing down any existing ownerless or
 * APP-owned ACL.
 * This implies DM_SM_WAE_ACL_NONE and DM_SM_WAE_ACL_OWNER_NONE 
 */
#define DM_SM_WAE_ACL_OWNER_APP     0x0004

/* Pair during link setup, first tearing down any existing ownerless or
 * L2CAP-owned ACL. This will of course close the l2cap connection(s).
 * This implies DM_SM_WAE_ACL_NONE and DM_SM_WAE_ACL_OWNER_NONE
 */
#define DM_SM_WAE_ACL_OWNER_L2CAP   0x0008

/* Pair during link setup, first tearing down any existing ACL */
#define DM_SM_WAE_ALWAYS ( DM_SM_WAE_ACL_NONE \
                | DM_SM_WAE_ACL_OWNER_NONE | DM_SM_WAE_ACL_OWNER_APP \
                | DM_SM_WAE_ACL_OWNER_L2CAP)

/*
 * config (bitfield)
 */
#define DM_SM_SEC_MODE_CONFIG_NONE                              0x0000
#define DM_SM_SEC_MODE_CONFIG_SEND_ACCESS_IND                   0x0001
#define DM_SM_SEC_MODE_CONFIG_DEBUG_MODE                        0x0002
#define DM_SM_SEC_MODE_CONFIG_LEGACY_AUTO_PAIR_MISSING_LINK_KEY 0x0004
#define DM_SM_SEC_MODE_CONFIG_DONT_ABORT_BONDING_FOR_MITM       0x0008
#define DM_SM_SEC_MODE_CONFIG_LOCAL_USER_CONFIRMATION           0x0010
#define DM_SM_SEC_MODE_CONFIG_NEVER_AUTO_ACCEPT                 0x0020
#define DM_SM_SEC_MODE_CONFIG_HCI_ERRORS_FOR_BONDING            0x0040

typedef struct
{
    dm_prim_t           type;           /* Always DM_SM_SEC_MODE_CONFIG_CFM */
    phandle_t           phandle;
    uint8_t             status;
    uint16_t            write_auth_enable; /* See DM_SM_BONDING_REQ */
    uint16_t            config;
} DM_SM_SEC_MODE_CONFIG_CFM_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request to add a device to the Security Manager's device database, or
 *      to modify the record for an existing entry. The operation will be
 *      confirmed in a DM_SM_ADD_DEVICE_CFM primitive.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_ADD_DEVICE_REQ */

    /* The address of the remote device */
    BD_ADDR_T           bd_addr;

    /* The trust level of the remote device, TRUE=trusted, FALSE=untrusted.
     * Trusted devices are automatically granted access by the Security Manager.
     * Untrusted devices result in a DM_SM_AUTHORISE_IND for access to protocols
     * that require authorisation.
     * In SEC_MODE2a_DEVICE, a device with 'trusted' set to TRUE will not be
     * authenticated when a new service is started.
     * The trust level is changed to 'trusted' if 'update_trust_level' is TRUE.
     */
    bool_t              update_trust_level;
    bool_t              trusted;

    /* The link key for the remote device. The Security Manager expects to have
     * a copy of the link key for each device. The trust level may be changed
     * for an existing device record by setting 'update_link_key' to FALSE, in
     * which case the Security Manager will not overwrite its copy of the link
     * key.
     */
    bool_t              update_link_key;
    uint8_t             link_key[SIZE_LINK_KEY];
} DM_SM_ADD_DEVICE_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Confirmation of adding a device to the device database.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_ADD_DEVICE_CFM */
    phandle_t           phandle;            /* Destination phandle */

    /* The address of the remote device */
    BD_ADDR_T           bd_addr;

    /* TRUE if added, FALSE if not added */
    bool_t              success;
} DM_SM_ADD_DEVICE_CFM_T;

/*---------------------------------------------------------------------------*
 * PURPOSE
 *      Replacement for DM_SM_ADD_DEVICE_REQ that adds support for
 *      link key type to the device database.
 *---------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;           /* DM_SM_SSP_ADD_DEVICE_REQ */
    BD_ADDR_T           bd_addr;        /* Bluetooth address */
    bool_t              update_trust_level;
    bool_t              trusted;
    bool_t              update_link_key;
    uint8_t             link_key[SIZE_LINK_KEY];
    uint8_t             link_key_type;
} DM_SM_SSP_ADD_DEVICE_REQ_T;

typedef DM_SM_ADD_DEVICE_CFM_T DM_SM_SSP_ADD_DEVICE_CFM_T;

/*---------------------------------------------------------------------------*
 *  PURPOSE
 *      Read from the Security Manager's device database
 *---------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t           type;           /* DM_SM_READ_DEVICE_REQ */
    BD_ADDR_T           bd_addr;        /* Bluetooth address */
} DM_SM_READ_DEVICE_REQ_T;

typedef struct
{
    dm_prim_t           type;           /* DM_SM_READ_DEVICE_CFM */
    phandle_t           phandle;        /* Destination phandle */
    BD_ADDR_T           bd_addr;        /* Bluetooth address */
    bool_t              success;
    bool_t              trusted;
    uint8_t             link_key_type;
    uint8_t             link_key[SIZE_LINK_KEY];
} DM_SM_READ_DEVICE_CFM_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request to remove a device from the Security Manager's device database.
 *      The operation will be confirmed in a DM_SM_REMOVE_DEVICE_CFM primitive.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_REMOVE_DEVICE_REQ */

    /* The address of the remote device to remove */
    BD_ADDR_T           bd_addr;
} DM_SM_REMOVE_DEVICE_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Confirmation of removing a device from the device database.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_REMOVE_DEVICE_CFM */
    phandle_t           phandle;            /* Destination phandle */

    /* The address of the remote device that was removed */
    BD_ADDR_T           bd_addr;

    /* TRUE if the device was removed, FALSE if it did not exist */
    bool_t              success;
} DM_SM_REMOVE_DEVICE_CFM_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      A request from the Security Manager for a link key required for
 *      authentication with the specified remote device. The application must
 *      respond with a DM_SM_LINK_KEY_REQUEST_RES primitive containing the link
 *      key, or with 'valid' set to FALSE to reject the request.
 *
 *      The Security Manager will only issue a link key request if it does not
 *      already have a valid link key for that device in the device database.
 *
 *      This primitive is not used by the Security Manager in mode 4. See
 *      DM_SM_SSP_LINK_KEY_REQUEST_IND_T below.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_LINK_KEY_REQUEST_IND */
    phandle_t           phandle;            /* Destination phandle */

    /* The address of the remote device for which a link key is required */
    BD_ADDR_T           bd_addr;
} DM_SM_LINK_KEY_REQUEST_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      A response to the Security Manager containing the link key for remote
 *      device 'bd_addr'. If no link key is known, then set 'valid' to FALSE to
 *      reject the request.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;           /* Always DM_SM_LINK_KEY_REQUEST_RES */

    /* The address of the remote device for which a link key was requested */
    BD_ADDR_T           bd_addr;

    /* The link key - set 'valid' to FALSE to reject */
    uint8_t             key[SIZE_LINK_KEY]; /* The link key value */
    bool_t              valid;  /* TRUE if link key valid, FALSE to reject */
} DM_SM_LINK_KEY_REQUEST_RES_T;


/*---------------------------------------------------------------------------*
 * PURPOSE
 *      A request from the Security Manager for a link key required for
 *      authentication with the specified remote device. The application must
 *      respond with a DM_SM_SSP_LINK_KEY_REQUEST_RES primitive containing
 *      the link and type, or with no link key and 'key_type' set to 
 *      DM_SM_KEY_TYPE_NONE to reject the request. 'authenticated' will be
 *      set to TRUE if an authenticated link key is required.
 *
 *      The Security Manager will only issue a link key request if it does
 *      not already have a suitable link key for that device in the device
 *      database.
 *
 *      This primitive is only sent when the Security Manager is in mode 4.
 *
 *      If an ACL exists then rhsf will provide the first 15 bits of the
 *      remote host's supported features. Currently only one bit is defined
 *      (bit 0), which is 1 if the remote host supports SSP and 0 if it
 *      doesn't (legacy pre-2.1 host). If no ACL exists then we return 0xFFFF.
 *
 *      If this parameter is set to 0 and initiated_outgoing is TRUE then sending
 *      a negative response will tell the security manager to initiate legacy
 *      pairing on an existing ACL. This can potentially lead to timeouts on
 *      some devices. To try to avoid timing out, you may want to prefetch
 *      the PIN from the user, set up an SDP 'ping' to keep the ACL alive, or
 *      whatever you did to avoid these problems in SM2.
 *
 *---------------------------------------------------------------------------*/

/* rhsf parameter */
#define DM_SM_RHSF_NO_ACL   0xFFFF
#define DM_SM_RHSF_NONE     0x0000
#define DM_SM_RHSF_SSP      0x0001

typedef struct
{
    dm_prim_t           type;           /* DM_SM_SSP_LINK_KEY_REQUEST_IND */
    phandle_t           phandle;        /* Destination phandle */
    BD_ADDR_T           bd_addr;        /* address of remote device */

    /* Class of device for peer, valid for incoming connections only else 0 */
    uint24_t            dev_class;

    bool_t              authenticated;  /* TRUE if we need authenticated key */
    /* TRUE if this is security for an outgoing connection that we've initiated */
    bool_t              initiated_outgoing;
    uint16_t            rhsf;           /* Remote host's supported features */
} DM_SM_SSP_LINK_KEY_REQUEST_IND_T;

/*---------------------------------------------------------------------------*
 * PURPOSE
 *      A response to DM_SM_SSP_LINK_KEY_REQUEST_IND
 *
 *      This is similar to DM_SM_LINK_KEY_REQUEST_IND; however, a 'key_type'
 *      parameter has been added and 'key' has been changed from an array
 *      to a pointer, which should always be set to NULL.
 *
 *      To respond with a suitable link key, set 'key_type' according to
 *      'HCI Link Key Notification Event Key Type', in hci.h, and then put
 *      the link key at the end of the primitive as normal.
 *
 *      To reject the request, set 'key_type' to DM_SM_KEY_TYPE_NONE and
 *      add nothing to the end of the primitive.
 *
 *---------------------------------------------------------------------------*/

/* key_type */
#define DM_SM_LINK_KEY_NONE                 ((uint8_t) 0xFE)
#define DM_SM_LINK_KEY_LEGACY               HCI_COMBINATION_KEY
#define DM_SM_LINK_KEY_DEBUG                HCI_DEBUG_COMBINATION_KEY
#define DM_SM_LINK_KEY_UNAUTHENTICATED      HCI_UNAUTHENTICATED_COMBINATION_KEY
#define DM_SM_LINK_KEY_AUTHENTICATED        HCI_AUTHENTICATED_COMBINATION_KEY
#define DM_SM_LINK_KEY_CHANGED              HCI_CHANGED_COMBINATION_KEY

typedef struct
{
    dm_prim_t           type;           /* DM_SM_LINK_KEY_REQUEST_EX_RES */

    BD_ADDR_T           bd_addr;        /* Remote device address */

    /* See HCI Link Key Notification Event Key Type in hci.h */
    uint8_t            key_type;

    uint8_t             *key;
} DM_SM_SSP_LINK_KEY_REQUEST_RES_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      A request from the Security Manager for a PIN code required for
 *      pairing with the specified remote device. The application must respond
 *      with a DM_SM_PIN_REQUEST_RES primitive containing the PIN code, or
 *      with 'pin_length' set to 0 to reject the request.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_PIN_REQUEST_IND */
    phandle_t           phandle;            /* Destination phandle */

    /* The address of the remote device for which a PIN is required */
    BD_ADDR_T           bd_addr;
    /* Class of device for peer, valid for incoming connections only else 0 */
    uint24_t            dev_class;
} DM_SM_PIN_REQUEST_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      A response to the Security Manager containing the PIN for remote device
 *      'bd_addr'. If no PIN is known, then set 'pin_length' to zero to reject
 *      the request.
 *
 *      If pairing succeeds as a result of the PIN, then the Security Manager
 *      will issue a DM_SM_LINK_KEY_IND primitive, containing the new link key.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_PIN_REQUEST_RES */

    /* The address of the remote device for which a PIN was requested */
    BD_ADDR_T           bd_addr;

    /* The PIN code - set 'pin_length' to zero to reject */
    uint8_t             pin_length;
    uint8_t             pin[HCI_MAX_PIN_LENGTH];
} DM_SM_PIN_REQUEST_RES_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      A notification from the Security Manager that a new link key is now
 *      in force for the remote device 'bd_addr'. This can be as a result of
 *      pairing, or as a result of one of the devices requesting a change of
 *      link key with the Change_Connection_Link_Key HCI command.
 *
 *      The Security Manager keeps a copy of the new link key, and passes it up
 *      to the application for non-volatile storage.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_LINK_KEY_IND */
    phandle_t           phandle;            /* Destination phandle */

    /* The address of the remote device with which pairing completed */
    BD_ADDR_T           bd_addr;

    /* The link key type and value. The type can be:
    *   HCI_COMBINATION_KEY
    *   HCI_LOCAL_UNIT_KEY
    *   HCI_REMOTE_UNIT_KEY
    */
    uint8_t             key_type;
    uint8_t             key[SIZE_LINK_KEY];
} DM_SM_LINK_KEY_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      A request for authorisation from the Security Manager to the
 *      application. This is only performed when an untrusted or unknown device
 *      is attempting to access a service that requires authorisation in
 *      security mode 2.
 *
 *      The application must make a decision and then respond with a
 *      DM_SM_AUTHORISE_RES primitive containing the result. Devices can be
 *      marked as trusted using the DM_SM_ADD_DEVICE_REQ primitive.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_AUTHORISE_IND */
    phandle_t           phandle;            /* Destination phandle */

    /* Device address, and the protocol+channel of the service being accessed */
    BD_ADDR_T           bd_addr;
    dm_protocol_id_t    protocol_id;
    uint32_t            channel;

    /* TRUE for incoming connection, FALSE for outgoing connection */
    bool_t              incoming;
} DM_SM_AUTHORISE_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Authorisation response from the application to the Security Manager.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_AUTHORISE_RES */

    /* The parameters from the authorisation request */
    BD_ADDR_T           bd_addr;
    dm_protocol_id_t    protocol_id;
    uint32_t            channel;
    bool_t              incoming;

    /* TRUE if authorisation granted, FALSE to reject */
    bool_t              authorised;
} DM_SM_AUTHORISE_RES_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Applications may request authentication of a link at any time, using
 *      the DM_SM_AUTHENTICATE_REQ primitive. If the link has already been
 *      authenticated, the Security Manager responds immediately, otherwise it
 *      performs the HCI authentication procedure, which may involve pairing.
 *
 *      This request will be rejected in security mode 1.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_AUTHENTICATE_REQ */

    /* Address of device to authenticate */
    BD_ADDR_T           bd_addr;
} DM_SM_AUTHENTICATE_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Result of an authentication request.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_AUTHENTICATE_CFM */
    phandle_t           phandle;            /* Destination phandle */

    /* Address of device being authenticated */
    BD_ADDR_T           bd_addr;

    /* TRUE if successful */
    bool_t              success;
} DM_SM_AUTHENTICATE_CFM_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Applications may request encryption activation or de-activation of a
 *      link at any time, using the DM_SM_ENCRYPT_REQ primitive. If the link
 *      encryption is already in the requested state, the Security Manager
 *      responds immediately, otherwise it performs the relevant HCI procedure.
 *      The link must have been authenticated before encryption is allowed.
 *
 *      This request will be rejected in security mode 1.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_ENCRYPT_REQ */

    /* Link to encrypt */
    BD_ADDR_T           bd_addr;

    /* TRUE to activate encryption, FALSE to stop encryption */
    bool_t              encrypt;
} DM_SM_ENCRYPT_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Result of a locally-initiated encryption request (DM_SM_ENCRYPT_REQ).
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_ENCRYPT_CFM */
    phandle_t           phandle;            /* Destination phandle */

    /* Link being encrypted */
    BD_ADDR_T           bd_addr;

    /* TRUE if the command was successful, FALSE if it failed. The actual
     * encryption state of the link is in 'encrypted'.
     */
    bool_t              success;

    /* TRUE if encrypted, FALSE if not encrypted */
    bool_t              encrypted;
} DM_SM_ENCRYPT_CFM_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of a possible change in the encryption status of a link
 *      due to a peer-initiated encryption procedure.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_ENCRYPTION_CHANGE */
    phandle_t           phandle;            /* Destination phandle */

    /* Link being encrypted */
    BD_ADDR_T           bd_addr;

    /* TRUE if encrypted, FALSE if not encrypted */
    bool_t              encrypted;
} DM_SM_ENCRYPTION_CHANGE_T; /* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */

/* This definition is provided for naming consistency */
typedef DM_SM_ENCRYPTION_CHANGE_T DM_SM_ENCRYPTION_CHANGE_IND_T;

typedef HCI_REFRESH_ENCRYPTION_KEY_T DM_HCI_REFRESH_ENCRYPTION_KEY_T;
typedef struct
{
    dm_prim_t               type;   /* DM_HCI_ENCRYPTION_KEY_REFRESH_IND */
    uint16_t                phandle;
    uint8_t                 status;
    BD_ADDR_T               bd_addr;
} DM_HCI_ENCRYPTION_KEY_REFRESH_IND_T;

#define DM_HCI_REFRESH_ENCRYPTION_KEY  ((dm_prim_t)(ENUM_DM_HCI_REFRESH_ENCRYPTION_KEY))
#define DM_HCI_ENCRYPTION_KEY_REFRESH_IND  ((dm_prim_t)(ENUM_DM_HCI_ENCRYPTION_KEY_REFRESH_IND))

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Internal to BlueStack.
 *
 *      L2CAP needs to register with the Security Manager in order to receive
 *      notifications of which PSMs should allow connectionless reception.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_L2CAP_REGISTER_REQ */

    /* phandle to receive connectionless security information */
    phandle_t           phandle;
} DM_SM_L2CAP_REGISTER_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Internal to BlueStack.
 *
 *      The Security Manager informs L2CAP of which PSMs should allow
 *      connectionless reception. In case of conflict with connectionless
 *      settings defined by the L2CA_ENABLE_CONNECTIONLESS_REQ and
 *      L2CA_DISABLE_CONNECTIONLESS_REQ primitives, any global enable
 *      takes priority over Security Manager enable, and Security
 *      Manager enable takes priority over individual L2CAP
 *      enable.
 *
 *----------------------------------------------------------------------------*/
#define MAX_CLRX_PSMS 10
typedef struct
{
    dm_prim_t           type;               /* Always DM_SM_L2CAP_CLRX_ENABLE_IND */

    /* phandle to receive connectionless security information */
    phandle_t           phandle;

    /* Array of PSMs for which connectionless reception is allowed. All other
     * PSMs should disallow it.
     */
    psm_t               psm_array[MAX_CLRX_PSMS];
} DM_SM_L2CAP_CLRX_ENABLE_IND_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

/*---------------------------------------------------------------------------*
 *  PURPOSE
 *      Common RES and IND primitive for Secure Simple Pairing
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;
    phandle_t               phandle;    /* Destination phandle */
    BD_ADDR_T               bd_addr;

    /* Class of device for peer, valid for incoming connections only else 0 */
    uint24_t                dev_class;
} DM_COMMON_IND_T; /* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */

typedef struct
{
    dm_prim_t               type;
    uint8_t                 unused;     /* Always set to 0 */
    BD_ADDR_T               bd_addr;
} DM_COMMON_RES_T; /* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */

/*---------------------------------------------------------------------------*
 *  DM_SM_IO_CAPABILITY_RESPONSE_IND
 *
 *      Reports parameters given by the other device.
 *      It is purely informative and you should not respond to it.
 *--------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;       /* DM_SM_IO_CAPABILITY_RESPONSE_IND */
    phandle_t               phandle;    /* Destination phandle */
    BD_ADDR_T               bd_addr;
    uint8_t                 io_capability;
    uint8_t                 oob_data_present;
    uint8_t                 authentication_requirements;
} DM_SM_IO_CAPABILITY_RESPONSE_IND_T;

/*---------------------------------------------------------------------------*
 *  DM_SM_IO_CAPABILITY_REQUEST_IND
 *
 *  The Security Manager asks the application for IO capability information
 *
 *  authentication_requirements is present so that the application can
 *  request General Bonding during service-based security and so that
 *  the application can insist on MITM protection, overiding the
 *  security manager. Effectively, the application can upgrade the security
 *  requirements. Setting authentication_requirements for dedicated bonding
 *  will always be ignored by the security manager as this will only be done
 *  when you issue a DM_SM_BONDING_REQ or when you are the responder and the
 *  initiator has set its authentication_requirements for dedicated bonding.
 *
 *  Note:
 *      1. An Application cannot downgrade the security requirements. If the
 *      security manager believes that MITM protection is required then
 *      it will request an authenticated link key, irrespective of the
 *      authentication_requirements set in this primitive.
 *
 *      2. If you are doing dedicated bonding then the security manager will
 *      send appropriate authentication_requirements (i.e. it will send either
 *      HCI_MITM_NOT_REQUIRED_DEDICATED_BONDING or 
 *      HCI_MITM_REQUIRED_DECICATED_BONDING) to the remote device.
 *
 *      3. If you are doing service-based security then the security manager
 *      will send appropriate authentication_requirements (i.e. it will send
 *      HCI_MITM_NOT_REQUIRED_NO_BONDING, HCI_MITM_REQUIRED_NO_BONDING,
 *      HCI_MITM_NOT_REQUIRED_GENERAL_BONDING or 
 *      HCI_MITM_REQUIRED_GENERAL_BONDING) to the remote device.
 *
 *      4. You should be careful about upgrading the security requirements
 *      to MITM protection required. It is probably only worth doing if
 *      you know that MITM protection is going to be required anyway. This
 *      might be the case when you are opening the RFCOMM MUX (which might
 *      not require MITM protection) in order to open an RFCOMM channel
 *      that does require MITM protection. You might be able to save some
 *      time by upgrading to MITM_REQUIRED when opening the MUX as this
 *      will avoid pairing twice.
 *
 *      5. Remember that if you insist on MITM protection and the
 *      io_capabilites of the two devices prevent the creation of
 *      an authenticated link key then pairing will fail.
 *
 *  If oob_data_present == 0 then both oob_hash_c and oob_rand_r shall be
 *  NULL and there shall be no associated data with the primitive.
 *
 *  If oob_data_present == 1 then oob_hash_c and oob_rand_r shall each point
 *  to SIZE_OOB_DATA words of OOB data. If you only have oob_hash_c then
 *  zero the oob_rand_r data.
 *
 *---------------------------------------------------------------------------*/
typedef DM_COMMON_IND_T DM_SM_IO_CAPABILITY_REQUEST_IND_T;

typedef struct
{
    dm_prim_t               type;       /* DM_SM_IO_CAPABILITY_REQUEST_RES */
    BD_ADDR_T               bd_addr;
    uint8_t                 io_capability;
    uint8_t                 oob_data_present;
    uint8_t                 authentication_requirements;
    uint8_t                 *oob_hash_c;
    uint8_t                 *oob_rand_r;
} DM_SM_IO_CAPABILITY_REQUEST_RES_T;

typedef struct
{
    dm_prim_t               type;     /* DM_SM_IO_CAPABILITY_REQUEST_NEG_RES */
    BD_ADDR_T               bd_addr;
    hci_reason_t            reason;
} DM_SM_IO_CAPABILITY_REQUEST_NEG_RES_T;

/*---------------------------------------------------------------------------*
 *  DM_SM_USER_CONFIRMATION_REQUEST_IND
 *
 *  The user must confirm that the displayed numeric values are the same
 *  There is a separate primitive to reject the request
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;   /* DM_SM_USER_CONFIRMATION_REQUEST_IND */
    phandle_t               phandle;        /* Destination phandle */
    BD_ADDR_T               bd_addr;

    /* Class of device for peer, valid for incoming connections only else 0 */
    uint24_t                dev_class;

    uint32_t                numeric_value;  /* Value to compare */
} DM_SM_USER_CONFIRMATION_REQUEST_IND_T;

/*---------------------------------------------------------------------------*
 *  DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND
 *
 *  The remote device cannot disaply the numeric value, but the local
 *  application/user might still want to confirm that the pairing continues.
 *
 *  Reply with DM_SM_USER_CONFIRMATION_REQUEST(_NEG)_RES
 *
 *---------------------------------------------------------------------------*/
typedef DM_SM_USER_CONFIRMATION_REQUEST_IND_T DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND_T;
typedef DM_COMMON_RES_T DM_SM_USER_CONFIRMATION_REQUEST_RES_T;
typedef DM_COMMON_RES_T DM_SM_USER_CONFIRMATION_REQUEST_NEG_RES_T;

/*---------------------------------------------------------------------------*
 *  DM_SM_USER_PASSKEY_REQUEST
 *
 *  The user must enter a six digit passkey.
 *  There is a separate primitive to reject the request
 *
 *---------------------------------------------------------------------------*/
typedef DM_COMMON_IND_T DM_SM_USER_PASSKEY_REQUEST_IND_T;

typedef struct
{
    dm_prim_t               type;       /* DM_SM_USER_PASSKEY_REQUEST_RES */
    uint8_t                 unused;     /* Always 0 */
    BD_ADDR_T               bd_addr;
    uint32_t                numeric_value;
} DM_SM_USER_PASSKEY_REQUEST_RES_T;

typedef DM_COMMON_RES_T DM_SM_USER_PASSKEY_REQUEST_NEG_RES_T;

/*---------------------------------------------------------------------------*
 *  DM_SM_USER_PASSKEY_NOTIFICATION_IND
 *
 *  The passkey should be displayed on the local device so that it may be
 *  entered on the remote device.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;     /* DM_SM_USER_PASSKEY_NOTIFICATION_IND */
    phandle_t               phandle;  /* destination phandle */
    BD_ADDR_T               bd_addr;
    uint32_t                passkey;

    /* Class of device for peer, valid for incoming connections only */
    uint24_t                dev_class;
} DM_SM_USER_PASSKEY_NOTIFICATION_IND_T;

/*---------------------------------------------------------------------------*
 *  DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ
 *
 *  Tell the remote device about keypresses during local passkey entry.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;    /* DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ */
    uint8_t                 unused;  /* Always 0 */
    BD_ADDR_T               bd_addr;
    uint8_t                 notification_type;
} DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_T;

/*---------------------------------------------------------------------------*
 *  DM_SM_KEYPRESS_NOTIFICATION_IND
 *
 *  Notification of keypresses during passkey entry on the remote device.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;      /* DM_SM_KEYPRESS_NOTIFICATION_IND */
    phandle_t               phandle;   /* destination phandle */
    BD_ADDR_T               bd_addr;
    uint8_t                 notification_type;
} DM_SM_KEYPRESS_NOTIFICATION_IND_T;

/*---------------------------------------------------------------------------*
 *  DM_SM_SIMPLE_PAIRING_COMPLETE_IND
 *
 *  Indication that authentication by SSP has finished
 *
 *--------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;      /* DM_SM_SIMPLE_PAIRING_COMPLETE_IND */
    phandle_t               phandle;   /* destination phandle */
    BD_ADDR_T               bd_addr;
    uint8_t                 status;
} DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T;

/*---------------------------------------------------------------------------*
 *  DM_SM_READ_LOCAL_OOB_DATA_REQ
 *
 *  The application may request local OOB data from the controller. This
 *  should then be transfered to the remote device via an OOB mechanism.
 *  Only the most recent set of OOB data retrieved may be used by a
 *  remote device for authentication
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;      /* DM_SM_READ_LOCAL_OOB_DATA_REQ */
    uint8_t                 unused;    /* Always 0 */
} DM_SM_READ_LOCAL_OOB_DATA_REQ_T;

typedef struct
{
    dm_prim_t               type;       /* DM_SM_READ_LOCAL_OOB_DATA_CFM */
    phandle_t               phandle;    /* destination phandle */
    uint8_t                 status;
    uint8_t                 *oob_hash_c;
    uint8_t                 *oob_rand_r;
} DM_SM_READ_LOCAL_OOB_DATA_CFM_T;

/*---------------------------------------------------------------------------*
 * DM_SM_BONDING_REQ/CFM
 *
 * The application requests bonding with a remote device. First we delete
 * any link key we have in the device database. 
 *
 * If the remote device is in SM4 then we make sure an ACL exists before
 * authenticating it. 
 *
 * If the remote device is legacy then the write_auth_enable parameter
 * determines in what circumstances we try to pair during link setup
 * (just like mode 3).
 *---------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;       /* DM_SM_BONDING_REQ */
    BD_ADDR_T               bd_addr;    /* Remote device address */
} DM_SM_BONDING_REQ_T;

typedef struct
{
    dm_prim_t               type;       /* DM_SM_BONDING_CFM */
    phandle_t               phandle;    /* destination phandle */
    BD_ADDR_T               bd_addr;    /* Remote device address */
    uint8_t                 status;
} DM_SM_BONDING_CFM_T;


/* If DM_SM_SEC_MODE_CONFIG_HCI_ERRORS_FOR_BONDING is not set in the config
   then the following error codes are used. */
#define DM_SM_BONDING_SUCCESS               0x00
#define DM_SM_BONDING_FAILURE               0x01
#define DM_SM_BONDING_CANCELLED             0x02
/* Otherwise we use the HCI error codes plus the following */
#define DM_SM_HCI_ERROR_BONDING_CANCELLED   0xFF

/* TODO Provide better documentation for this */
/* Try to cancel a bonding request. If you set force = TRUE then, if all
 * else fails, the DM will cancel bonding by bringing down the ACL, even
 * if there are l2cap connections open.
 */
typedef struct
{
    dm_prim_t               type;       /* DM_SM_BONDING_CANCEL_REQ */
    BD_ADDR_T               bd_addr;    /* Remote device address */
    bool_t                  force;
} DM_SM_BONDING_CANCEL_REQ_T;

/*-------------------------------------------------------------
 *
 *   CONNECTION PARAMETER CACHE INTERFACE
 *
 *---------------------------------------------------------------*/

/* Cache the page mode parameters for a given bd_addr */
typedef struct
{
    dm_prim_t            type;              /* Always DM_WRITE_CACHED_PAGE_MODE_REQ */
    BD_ADDR_T            bd_addr;           /* Bluetooth device address */
    page_scan_mode_t	 page_scan_mode;
    page_scan_rep_mode_t page_scan_rep_mode;
} DM_WRITE_CACHED_PAGE_MODE_REQ_T;

        /* Always DM_WRITE_CACHED_PAGE_MODE_CFM */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_WRITE_CACHED_PAGE_MODE_CFM_T;

/* Cache the clock offset parameter for a given bd_addr */
typedef struct
{
    dm_prim_t           type;               /* Always DM_WRITE_CACHED_CLOCK_OFFSET_REQ */
    BD_ADDR_T           bd_addr;            /* Bluetooth device address */
    uint16_t            clock_offset;       /* the clock offset */
} DM_WRITE_CACHED_CLOCK_OFFSET_REQ_T;

        /* Always DM_WRITE_CACHED_CLOCK_OFFSET_CFM */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_WRITE_CACHED_CLOCK_OFFSET_CFM_T;

/* Clear all cached parameters for a given bd_addr */
typedef struct
{
    dm_prim_t           type;               /* Always DM_CLEAR_PARAM_CACHE_REQ */
    BD_ADDR_T           bd_addr;            /* Bluetooth device address */
} DM_CLEAR_PARAM_CACHE_REQ_T;

        /* Always DM_CLEAR_PARAM_CACHE_CFM */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_CLEAR_PARAM_CACHE_CFM_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Letting higher layers know about the flow control status.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_REGISTER_CFM */
    phandle_t               phandle;        /* destination phandle */
    bool_t                  Active;         /* TRUE = ON */
    
} DM_HC_TO_HOST_FLOW_CONTROL_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request from Application task for the DM to create an ACL to the
 *      specified remote device. This might be used for pairing, for example.
 *      The DM allows both L2CAP and the application to request ACLs. L2CAP
 *      uses the DM_ACLQUEUE interface primitives (DM_ACL_CONNECT_REQ/CFM),
 *      and the application uses the DM_IFACEQUEUE primitives
 *      (DM_ACL_OPEN_REQ/CFM). The DM keeps track of who has an interest in
 *      each ACL, and will only disconnect the ACL when all interested parties
 *      have released the ACL (by DM_ACL_DISCONNECT_REQ and DM_ACL_CLOSE_REQ).
 *
 *      DM_ACL_OPEN_REQ registers the application's interest in the ACL. DM will
 *      create the link if it does not already exist. DM will always respond
 *      with DM_ACL_OPEN_CFM, indicating success or failure. If successful, the
 *      application may subsequently call DM_ACL_CLOSE_REQ to relinquish its
 *      interest in the ACL - DM will then release the link if L2CAP is not
 *      using it.
 *
 *      The DM keeps the application informed of the state of all ACLs via the
 *      DM_ACL_OPENED_IND and DM_ACL_CLOSED_IND primitives. Note that there is
 *      no specific response to DM_ACL_CLOSE_REQ, as the DM_ACL_CLOSED_IND is
 *      only issued when all users of the ACL have released it.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_ACL_OPEN_REQ */
    BD_ADDR_T               bd_addr;        /* Remote device address */
} DM_ACL_OPEN_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Confirmation of an application request to create an ACL. See text for
 *      DM_ACL_OPEN_REQ for more details.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_ACL_OPEN_CFM */
    phandle_t               phandle;        /* Destination phandle */
    BD_ADDR_T               bd_addr;        /* Remote device address */
    bool_t                  success;        /* TRUE if open, FALSE if not */
} DM_ACL_OPEN_CFM_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request from the application to close the ACL to the specified device.
 *      An application may send this request only if it previously requested
 *      ACL creation via the DM_ACL_OPEN_REQ primitive.
 *
 *      There is no specific response to this primitive - DM_ACL_CLOSED_IND is
 *      issued when the ACL is closed, but the DM may keep the ACL open if L2CAP
 *      is still using it.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_ACL_CLOSE_REQ */
    BD_ADDR_T               bd_addr;        /* Remote device address */
} DM_ACL_CLOSE_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Letting higher layers know about ACL comings and goings. This primitive
 *      is issued to the application interface whenever an ACL is successfully
 *      established (incoming or outgoing).
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_ACL_OPENED_IND */
    phandle_t               phandle;        /* Destination phandle */
    BD_ADDR_T               bd_addr;        /* Peer device address */

    /* Flag indicating peer-initiated ACL (TRUE) or locally-initiated (FALSE) */
    bool_t                  incoming;
    /* Class of device for peer, valid for incoming connections only */
    uint24_t                dev_class;
} DM_ACL_OPENED_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Letting higher layers know about ACL comings and goings. This primitive
 *      is issued to the application interface whenever a successfully
 *      established ACL is disconnected.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_ACL_CLOSED_IND */
    phandle_t               phandle;        /* Destination phandle */
    BD_ADDR_T               bd_addr;        /* Peer device address */
} DM_ACL_CLOSED_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Define the default link policy to be applied to all new connections.
 *      This does not affect existing connections.
 *
 *      For valid policy settings see 'HCI Link Policy Settings' in hci.h.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_SET_DEFAULT_LINK_POLICY */

    /* Link Manager policy settings for incoming and outgoing connections */
    link_policy_settings_t  link_policy_settings_in;
    link_policy_settings_t  link_policy_settings_out;
} DM_SET_DEFAULT_LINK_POLICY_T;

/*------------------------------------------------------------------------
 *
 *      HCI API PRIMITIVES
 *
 *-----------------------------------------------------------------------*/

/******************************************************************************
 Link Control 
 *****************************************************************************/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Inquiry command 
 *
 *----------------------------------------------------------------------------*/

typedef HCI_INQUIRY_T DM_HCI_INQUIRY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Inquiry cancel command 
 *
 *----------------------------------------------------------------------------*/

typedef HCI_INQUIRY_CANCEL_T DM_HCI_INQUIRY_CANCEL_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of Inquiry cancel command complete
 *
 *----------------------------------------------------------------------------*/
        /* Always DM_HCI_INQUIRY_CANCEL_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_INQUIRY_CANCEL_COMPLETE_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Enter periodic inquiry mode
 *
 *----------------------------------------------------------------------------*/

typedef HCI_PERIODIC_INQUIRY_MODE_T DM_HCI_PERIODIC_INQUIRY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of entered periodic inquiry mode
 *
 *----------------------------------------------------------------------------*/
        /* Always DM_HCI_PERIODIC_INQUIRY_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_PERIODIC_INQUIRY_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Exit periodic inquiry mode
 *
 *----------------------------------------------------------------------------*/

typedef HCI_EXIT_PERIODIC_INQUIRY_MODE_T DM_HCI_EXIT_PERIODIC_INQUIRY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of exited periodic inquiry mode
 *
 *----------------------------------------------------------------------------*/
        /* Always DM_HCI_EXIT_PERIODIC_INQUIRY_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_EXIT_PERIODIC_INQUIRY_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of inquiry results
 *
 *		See HCI_EV_INQUIRY_RESULT_T.
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t               type;           /* Always DM_HCI_INQUIRY_RESULT_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 num_responses;

    HCI_INQ_RESULT_T *		result[HCI_MAX_INQ_RESULT_PTRS];

} DM_HCI_INQUIRY_RESULT_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of inquiry results with rssi information
 *
 *		See HCI_EV_INQUIRY_RESULT_WITH_RSSI_T.
 *----------------------------------------------------------------------------*/

#define DM_HCI_INQUIRY_RESULT_WITH_RSSI ((dm_prim_t)(ENUM_DM_HCI_INQUIRY_RESULT_WITH_RSSI))

typedef struct
{
    dm_prim_t               type;           /* Always DM_HCI_INQUIRY_RESULT_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 num_responses;

    HCI_INQ_RESULT_WITH_RSSI_T *		result[HCI_MAX_INQ_RESULT_PTRS];

} DM_HCI_INQUIRY_RESULT_WITH_RSSI_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of inquiry complete
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t               type;           /* Always DM_HCI_INQUIRY_COMPLETE_T */
    phandle_t               phandle;            /* destination phandle */
    hci_return_t            status;             /* cast to error if error */

} DM_HCI_INQUIRY_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request change of packet type on a connection.
 *
 *----------------------------------------------------------------------------*/
typedef HCI_CHANGE_CONN_PKT_TYPE_T DM_HCI_CHANGE_PACKET_TYPE_T;










/*----------------------------------------------------------------------------*
 * PURPOSE
 *      A request to start a synchronous connection has been made by a remote host
 *
 *----------------------------------------------------------------------------*/












/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Change link key for a device
 *
 *----------------------------------------------------------------------------*/

typedef HCI_CHANGE_CONN_LINK_KEY_T DM_HCI_CHANGE_LINK_KEY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of change of link key for a device
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_LINK_KEY_CHANGE_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* Success or failure - See Note1 */
	BD_ADDR_T           bd_addr;            /* Bluetooth device address */

} DM_HCI_LINK_KEY_CHANGE_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of link keys for devices
 *
 *		See HCI_EV_RETURN_LINK_KEYS_T for details.
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_RETURN_LINK_KEYS_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             num_keys;           /* Number of link keys */

    LINK_KEY_BD_ADDR_T  *link_key_bd_addr[HCI_STORED_LINK_KEY_MAX];

} DM_HCI_RETURN_LINK_KEYS_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Use the master device link keys
 *
 *----------------------------------------------------------------------------*/

typedef HCI_MASTER_LINK_KEY_T DM_HCI_MASTER_LINK_KEY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of use of the master device link keys
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_MASTER_LINK_KEY_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* Success or failure - See Note1 */
    uint8_t             key_flag;           /* Regular/Temporary link key */

} DM_HCI_MASTER_LINK_KEY_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Remote name request
 *
 *----------------------------------------------------------------------------*/

typedef HCI_REMOTE_NAME_REQ_T DM_HCI_REMOTE_NAME_REQUEST_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of remote name
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_REMOTE_NAME_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* Success or failure - See Note1 */
	BD_ADDR_T           bd_addr;            /* Bluetooth device address */
	/* User friendly name */
    uint8_t             *name_part[HCI_LOCAL_NAME_BYTE_PACKET_PTRS]; 

} DM_HCI_REMOTE_NAME_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read remote features
 *
 *----------------------------------------------------------------------------*/

typedef HCI_READ_REMOTE_SUPP_FEATURES_T DM_HCI_READ_REMOTE_FEATURES_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of remote features
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_REMOTE_FEATURES_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* Success or failure - See Note1 */
	BD_ADDR_T           bd_addr;            /* Bluetooth device address */
    uint16_t            features[4];        /* LMP features */

} DM_HCI_READ_REMOTE_FEATURES_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read remote version
 *
 *----------------------------------------------------------------------------*/

typedef HCI_READ_REMOTE_VER_INFO_T DM_HCI_READ_REMOTE_VERSION_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of remote version information
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_REMOTE_VERSION_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* Success or failure - See Note1 */
	BD_ADDR_T           bd_addr;            /* Bluetooth device address */
    uint8_t             LMP_version;        /* Version of LMP */
    uint16_t            manufacturer_name;  /* Name of manufacturer */
    uint16_t            LMP_subversion;     /* Subversion of LMP */

} DM_HCI_READ_REMOTE_VERSION_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read clock offset
 *
 *----------------------------------------------------------------------------*/

typedef HCI_READ_CLOCK_OFFSET_T DM_HCI_READ_CLOCK_OFFSET_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read clock offset
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_CLOCK_OFFSET_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* Success or failure - See Note1 */
	BD_ADDR_T           bd_addr;            /* Bluetooth device address */
    uint16_t            clock_offset;       /* Device's clock offset */

} DM_HCI_READ_CLOCK_OFFSET_COMPLETE_T;

/******************************************************************************
   Link Policy 
 *****************************************************************************/

 /*----------------------------------------------------------------------------*
 * PURPOSE
 *      Put local or remote device into hold mode
 *      Use the remote bluetooth device address to map onto  
 *      connection handle
 *
 *----------------------------------------------------------------------------*/

typedef HCI_HOLD_MODE_T DM_HCI_HOLD_MODE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Put local device (for a particular connection) into sniff mode
 *      Note, SCO connections cannot be put into sniff.
 *
 *----------------------------------------------------------------------------*/

typedef HCI_SNIFF_MODE_T DM_HCI_SNIFF_MODE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      End local device (for a particular connection) in sniff mode
 *      Note, SCO connections cannot be put into sniff.
 *
 *----------------------------------------------------------------------------*/

typedef HCI_EXIT_SNIFF_MODE_T DM_HCI_EXIT_SNIFF_MODE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Put local or remote device into park mode
 *
 *----------------------------------------------------------------------------*/

typedef HCI_PARK_MODE_T DM_HCI_PARK_MODE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Exit local or remote device in park mode
 *
 *----------------------------------------------------------------------------*/

typedef HCI_EXIT_PARK_MODE_T DM_HCI_EXIT_PARK_MODE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Mode change event
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_MODE_CHANGE_EVENT_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* Success or failure - See Note1 */
	BD_ADDR_T           bd_addr;            /* Bluetooth device address */
    uint8_t             mode;               /* Current mode */
    uint16_t            length;             /* Length of mode (or similar) */

} DM_HCI_MODE_CHANGE_EVENT_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Quality of service setup from L2CAP
 *
 *----------------------------------------------------------------------------*/

typedef HCI_QOS_SETUP_T DM_HCI_QOS_SETUP_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Completion of quality of service setup 
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
    dm_prim_t                type;    /* always DM_HCI_QOS_SETUP_CFM */             
    phandle_t                phandle; /* destination phandle */
    hci_return_t             status;  /* cast to error if error */
	BD_ADDR_T				 bd_addr;            /* Bluetooth device address */
    uint8_t                  flags;              /* reserved */
    hci_qos_type_t           service_type;
    uint32_t                 token_rate;
    uint32_t                 peak_bandwidth;
    uint32_t                 latency;
    uint32_t                 delay_variation;
} DM_HCI_QOS_SETUP_CFM_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Quality of service violation
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
    dm_prim_t                type;    /* always DM_HCI_QOS_VIOLATION_IND */           
    phandle_t                phandle; /* destination phandle */           
} DM_HCI_QOS_VIOLATION_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Discover role
 *      Use the bd_addr to identify the (ACL) link at this interface
 *
 *----------------------------------------------------------------------------*/

typedef HCI_ROLE_DISCOVERY_T DM_HCI_ROLE_DISCOVERY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Discover role complete
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_ROLE_DISCOVERY_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    BD_ADDR_T           bd_addr;            /* BD_ADDR of link */
    uint8_t             status;             /* Success or failure - See Note1 */
    uint8_t             role;               /* Master or slave */

} DM_HCI_ROLE_DISCOVERY_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Switch role
 *      Use the bd_addr to identify the (ACL) link at this interface
 *
 *----------------------------------------------------------------------------*/

typedef HCI_SWITCH_ROLE_T DM_HCI_SWITCH_ROLE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Switch role complete
 *      The current role is contained in 'role', even in case of failure.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_SWITCH_ROLE_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    BD_ADDR_T           bd_addr;            /* BD_ADDR of link */
    uint8_t             status;             /* Success or failure - See Note1 */
    uint8_t             role;               /* Master or slave */

} DM_HCI_SWITCH_ROLE_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read link policy settings from LM
 *      Use the bd_addr to identify the (ACL) link at this interface
 *
 *----------------------------------------------------------------------------*/

typedef HCI_READ_LINK_POLICY_SETTINGS_T DM_HCI_READ_LP_SETTINGS_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read link policy settings complete from LM
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_LP_SETTINGS_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    BD_ADDR_T           bd_addr;            /* bd_addr of link */
    uint16_t            link_policy;        /* link policy settings */

} DM_HCI_READ_LP_SETTINGS_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write link policy settings.
 *      Use the bd_addr to identify the ACL link at this interface.
 *
 *      There are two settings - the Link Manager settings, which define the
 *      connection modes that are allowed (hold/sniff/park); and the Device
 *      Manager settings, which define how the Device Manager tries to control
 *      the connection mode. It is the responsibility of the caller to specify
 *      compatible LM and DM link policies.
 *
 *      The DM policies are as follows:
 *          - DM_LINK_POLICY_DEFAULT - DM does not manage the link in any way.
 *              It is left to the higher layers to control the park and sniff
 *              modes. This is the default mode if no DM policy is specified.
 *          - DM_LINK_POLICY_NO_CHANGE - Not really a mode, this simply means
 *              that the current DM link policy is not changed. This is used to
 *              change the LM link policy settings without affecting the DM link
 *              policy settings.
 *          - DM_LINK_POLICY_TRANSPARENT
 *          - DM_LINK_POLICY_KEEP_ACTIVE
 *          - DM_LINK_POLICY_KEEP_SNIFF
 *          - DM_LINK_POLICY_KEEP_PARKED
 *          - DM_LINK_POLICY_TWO_STAGE_SNIFF
 *               These modes have all been depreciated by LP_POWERSTATES. Note
 *               that when using PowerStates, the user should always pass
 *               DM_LINK_POLICY_NO_CHANGE when trying to just update the
 *               link_policy_setings, such as disabling role-switch.
 *
 *      The DM will accept mode change requests from the application (i.e.
 *      DM_HCI_PARK_MODE, DM_HCI_EXIT_SNIFF_MODE etc) only when the ACL connection is
 *      in the DM_LINK_POLICY_DEFAULT mode. When it is in any of the other
 *      modes, any mode change requests will just be answered with a mode change
 *      event, DM_HCI_MODE_CHANGE_EVENT, indicating the current mode.
 *
 *      In all modes, the DM will keep the application informed of any changes
 *      of mode by issuing DM_HCI_MODE_CHANGE_EVENT events.
 *
 *----------------------------------------------------------------------------*/

typedef uint8_t dm_link_policy_t;
#define DM_LINK_POLICY_DEFAULT      ((dm_link_policy_t) 0x00)
#define DM_LINK_POLICY_NO_CHANGE    ((dm_link_policy_t) 0x05)

#define HCI_LINK_POLICY_NO_CHANGE   ((link_policy_settings_t) 0xffff)

/* Do NOT use */
typedef struct 
{
	uint16_t max_interval;          /* Max sniff interval */
	uint16_t min_interval;          /* Min sniff interval */
	uint16_t attempt;               /* Sniff attempt */
	uint16_t timeout;               /* Sniff timeout */
} SNIFF_SETTINGS_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

/* Do NOT use */
typedef struct 
{
	uint16_t max_interval;          /* Max park interval */
	uint16_t min_interval;          /* Min park interval */

	/* The amount of time for which the ACL link should be idle before
	 * being parked. DM will try to keep the link active until there has
	 * been no ACL data in either direction for this amount of time.
	*/
	uint32_t park_idle_time;        /* Microseconds */
} PARK_SETTINGS_T ;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

typedef union
{
	SNIFF_SETTINGS_T sniff_settings ;
	PARK_SETTINGS_T  park_settings;
} U_SNIFF_PARK_T ;

typedef struct
{
    dm_prim_t               type;           /* Always DM_ACL_SET_LINK_POLICY */
    BD_ADDR_T               bd_addr;        /* Bluetooth device address */

    /* Link Manager link policy settings - i.e. hold/sniff/park allowed, sent
     * to Host Controller via HCI_Write_Link_Policy_Settings command.
     * Can be set to HCI_LINK_POLICY_NO_CHANGE to not send the HCI command.
     */
    link_policy_settings_t  link_policy_settings;

    /* Device Manager link policy */
    dm_link_policy_t        dm_policy;

    /* This is no longer used. */
	U_SNIFF_PARK_T          u;


} DM_HCI_WRITE_LP_SETTINGS_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Defines the powerstate(s) to be applied to the specified ACL. It
 *      takes the form of a list of successive power modes which the ACL moves
 *      through over periods of increasing inactivity.  If at anytime there
 *      is activity on the ACL, control will reset to the first powerstate.
 *      
 *      Each powerstate is defined using the LP_POWERSTATE_T structure.
 *      The 'mode' entry is used to define the Bluetooth power mode.
 *      The 'min_interval' etc fields define the parameters of the Bluetooth
 *          power mode - see the HCI spec.
 *      The 'duration' is the length of time, in seconds, that the ACL will
 *          remain in this state.  When this period expires, control moves
 *          to the next state.  The last powerstate therefore MUST have an
 *          infinite duration (zero).
 *      
 *----------------------------------------------------------------------------*/
typedef uint8_t lp_powermode_t;
#define LP_POWERMODE_ACTIVE ((lp_powermode_t)0x00)
#define LP_POWERMODE_SNIFF  ((lp_powermode_t)0x01)
/* Passive mode is a "don't care" setting where the local device will not
   attempt to alter the power mode. */
#define LP_POWERMODE_PASSIVE ((lp_powermode_t)0xff)

typedef struct
{
    lp_powermode_t mode;
    uint16_t min_interval;
    uint16_t max_interval;
    uint16_t attempt;
    uint16_t timeout;
    uint16_t duration; /* time to spend in this mode */
} LP_POWERSTATE_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

typedef struct
{
    dm_prim_t       type;
    BD_ADDR_T       bd_addr;    /* Bluetooth Device Address */
    uint16_t        num_states; /* Number of states in list */
    LP_POWERSTATE_T *states;    /* Table of power states */
} DM_LP_WRITE_POWERSTATES_REQ_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Generated in response to DM_LP_WRITE_POWERSTATES_REQ.
 *
 *----------------------------------------------------------------------------*/
#define LP_POWERSTATES_SUCCESS          ((uint8_t)0) /* powerstates set successfully */
#define LP_POWERSTATES_UNSUPPORTED_MODE ((uint8_t)1) /* unrecognised LP_POWERMODE */
#define LP_POWERSTATES_UNKNOWN_DEVICE   ((uint8_t)2) /* unknown bluetooth device */
#define LP_POWERSTATES_BAD_TERMINATION  ((uint8_t)3) /* last state duration not zero */
#define LP_POWERSTATES_ERROR            ((uint8_t)4) /* error described above */
typedef struct
{
    dm_prim_t   type;
    BD_ADDR_T   bd_addr;
    uint8_t     result;
} DM_LP_WRITE_POWERSTATES_CFM_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Allows the application to modify role-switch policy for ACL
 *      connection setup.
 *
 *      The policy takes the form of a table of conditions and actions.
 *      When the device manager is about to initiate a connection to a
 *      remote device, or is about to accept an incoming connection
 *      request, it walks down the table until it reaches an entry for
 *      which all conditions are met. Role-switch policy for that ACL
 *      connection is then determined by the actions specified.
 *
 *      There are four condition criteria:
 *
 *      1) Minimum number of connections to remote slaves.
 *      2) Minimum number of connections to remote masters.
 *      3) Minimum number of connections to remote unsniffed masters.
 *      4) Minimum number of connections.
 *
 *      Each entry in the table specifies separately the minimum number of 
 *      existing connections for each individual condition to be met. So the
 *      resolution operator for each condition is always >= 'greater than
 *      or equal to'.
 *
 *      There are two actions:
 *
 *      1) Accept incoming connection with role-switch (becoming master).
 *      2) Request outgoing connection, prohibiting role-switch (staying master).
 *
 *      Either, both, or neither action can be set for any specified 
 *      combination of conditions. If an action isn't specified, then the
 *      reverse happens, i.e. outgoing connections are requested allowing
 *      role-switch and incoming connections are accepted without requesting
 *      role-switch.
 *
 *      Encoding
 *      Each 'row' in the table is encoded into a single uint16_t.
 *      Each condition is encoded into 3 bits, giving a range of 0-7.
 *      Each action is encoded into 1 bit - setting the bit turns on the action.
 *
 *      Bits
 *        0-2  Condition - minimum number of remote slaves.
 *        3-5  Condition - minimum number of remote masters.
 *        6-8  Condition - minimum number of remote unsniffed masters.
 *        9-11 Condition - minimum number of connections.
 *       12    Action - Accept incoming connection with role-switch.
 *       13    Action - Request outgoing connection, prohibiting role-switch.
 *       15-16 Reserved - must be set to zero.
 *
 *       There are macros defined below to make packing the condtions easier.
 *
 *      Restrictions
 *      1) The final table entry must be 0 - i.e. 'always do nothing'.
 *      2) The reserved bits must all be set to 0.
 *      3) All table entries (except the last) must have at least one
 *         non-zero condtion.
 *
 *      Operation
 *      Subsequent valid tables sent will replace any existing table.
 *      Sending a zero-length table will delete any existing table.
 *      If the device manager does not have a table then it will default
 *      to its legacy behaviour, which was to never prohibit role-switch
 *      when requesting connection creation and only to request a role-switch
 *      when accepting remote connection requests if there is already an
 *      existing connection to some other device.
 *
 *      Notes
 *      The purpose of this feature is to help manage and hopefully simplify
 *      scatternet topology. Using it for any other purpose is likely to
 *      severely reduce device interoperability. In particular, prohibiting 
 *      role-switch on outgoing connections should be used sparingly, as it
 *      may be that the topology that the remote device finds itself in is
 *      even more complicated than that for the local device and the only way
 *      for the connection to succeed is with a role-switch.
 *
 *--------------------------------------------------------------------------*/

/* Bits reserved per condition */
#define DM_LP_ROLESWITCH_CONDITION_BITS                     3

/* Conditions */
#define DM_ACL_NUM_SLAVES                                   0
#define DM_ACL_NUM_MASTERS                                  1
#define DM_ACL_NUM_UNSNIFFED_MASTERS                        2
#define DM_ACL_NUM_CONNECTIONS                              3
#define DM_ACL_NUM_END                                      4

/* Actions */
#define DM_LP_ROLESWITCH_ACTION_INCOMING_RS                 0x1000
#define DM_LP_ROLESWITCH_ACTION_OUTGOING_NO_RS              0x2000

 /* Reserved (must not be set) */
#define DM_LP_ROLESWITCH_RESERVED_1                         0x4000
#define DM_LP_ROLESWITCH_RESERVED_2                         0x8000

/* Macro to encode conditions */
#define DM_LP_ROLESWITCH_WRITE_CONDITION(value, condition)  \
    ((value) << ((condition) * DM_LP_ROLESWITCH_CONDITION_BITS))

/* Macro to decode conditions */
#define DM_LP_ROLESWITCH_READ_CONDITION(policy, condition)  \
    (((policy) >> ((condition) * DM_LP_ROLESWITCH_CONDITION_BITS)) & 7)


/* Macro to encode complete table entry
 *
 * slv, mst, usm, con are the four conditions - enter a number 0-7 for each
 * rs_in - enter 0 or DM_LP_ROLESWITCH_ACTION_INCOMING_RS
 * no_rs_out - enter 0 or DM_LP_ROLESWITCH_ACTION_OUTGOING_NO_RS
 */
#define DM_LP_ROLESWITCH_TABLE_ENTRY(slv, mst, usm, con, rs_in, no_rs_out) \
     (DM_LP_ROLESWITCH_WRITE_CONDITION((slv), DM_ACL_NUM_SLAVES) \
    | DM_LP_ROLESWITCH_WRITE_CONDITION((mst), DM_ACL_NUM_MASTERS) \
    | DM_LP_ROLESWITCH_WRITE_CONDITION((usm), DM_ACL_NUM_UNSNIFFED_MASTERS) \
    | DM_LP_ROLESWITCH_WRITE_CONDITION((con), DM_ACL_NUM_CONNECTIONS) \
    | rs_in \
    | no_rs_out)

/* Masks */
#define DM_LP_ROLESWITCH_CONDITION_MASK                     0x0FFF
#define DM_LP_ROLESWITCH_ACTION_MASK                        0x3000
#define DM_LP_ROLESWITCH_RESERVED_MASK                      0xC000

typedef struct
{
    dm_prim_t           type;               /* Always DM_LP_WRITE_ROLESWITCH_POLICY_REQ */
    uint16_t            version;            /* Must be set to 0 */
    uint16_t            length;             /* Number of uint16s in table */
    uint16_t            *rs_table;          /* Pointer to role-switch table */
} DM_LP_WRITE_ROLESWITCH_POLICY_REQ_T;

/*---------------------------------------------------------------------------*
 * PURPOSE
 *      Response to DM_LP_WRITE_ROLESWITCH_POLICY_REQ.
 *
 *---------------------------------------------------------------------------*/ 


#define DM_LP_WRITE_ROLESWITCH_POLICY_SUCCESS               ((uint8_t)0)
#define DM_LP_WRITE_ROLESWITCH_POLICY_ILLEGAL               ((uint8_t)1)
#define DM_LP_WRITE_ROLESWITCH_POLICY_UNSUPPORTED           ((uint8_t)2)
#define DM_LP_WRITE_ROLESWITCH_POLICY_NOT_ZERO_TERMINATED   ((uint8_t)3)
typedef struct
{
    dm_prim_t           type;               /* Always DM_LP_WRITE_ROLESWITCH_POLICY_CFM */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* Indication of success or failure */
} DM_LP_WRITE_ROLESWITCH_POLICY_CFM_T;

/*---------------------------------------------------------------------------*
 * PURPOSE
 *      Maintain list of remote devices for which the Device Manager will
 *      always try to become master during any ACL connection creation,
 *      even if there are no existing ACLs connected.
 *
 *      For locally-initited connection requests to devices in the list,
 *      the Device Manager will prohibit role-switch, thus ensuring that
 *      the local device becomes master.
 *
 *      For remotely-initiated connection requests to devices in the list,
 *      the Device Manager will request a role-switch during connection
 *      creation. This may or may not be accepted by the remote device.
 *
 *      This primitive may be used to:
 *
 *      1) ADD a new device to the list
 *      2) DELETE a device from the list
 *      3) CLEAR the entire list
 *
 *      This primitive should be used only to work around problems with 
 *      severely misbehaving remote devices. Any other use is likely to
 *      produce a severely misbehaving local device and lead to major
 *      interoperability problems.
 *
 *      Roleswitches should only be used to simplify topology. In most
 *      cases, DM_LP_WRITE_ROLESWITCH_POLICY_REQ/CFM should be sufficient.
 *      DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ/CFM should only be used
 *      when it is necessary to become master, even when there are no
 *      existing connections, because the remote device is badly behaved
 *      and will not roleswitch after connection creation and it is likely
 *      that further ACLs will soon be connected. (If it turns out that
 *      no further ACLs materialise after a connection affected by the
 *      device list is created then the local device might want to
 *      consider initiating a role-switch to become slave.)
 *---------------------------------------------------------------------------*/
#define DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CLEAR     0x0000
#define DM_LP_WRITE_ALWAYS_MASTER_DEVICES_ADD       0x0001
#define DM_LP_WRITE_ALWAYS_MASTER_DEVICES_DELETE    0x0002
typedef struct
{
    dm_prim_t           type;           /* Always DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ */
    uint16_t            operation;      /* ADD, DELETE, CLEAR. */
    BD_ADDR_T           bd_addr;        /* Pointer to array of Bluetooth addresses. */
} DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_T;

#define DM_LP_WRITE_ALWAYS_MASTER_DEVICES_SUCCESS   0x00
#define DM_LP_WRITE_ALWAYS_MASTER_DEVICES_ILLEGAL   0x01
typedef struct
{
    dm_prim_t           type;           /* Always DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM */
    phandle_t           phandle;        /* destination phandle */
    uint8_t             status;         /* Indication of success or failure */
} DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_T;
/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write link policy settings complete from LM
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_WRITE_LP_SETTINGS_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    BD_ADDR_T           bd_addr;            /* bd_addr of link */

} DM_HCI_WRITE_LP_SETTINGS_COMPLETE_T;


#ifndef EXCLUDE_BLUESTACK_ENHANCEMENTS
/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Enabled/disables the Enhanced features of Bluestack.
 *      Bluestack will respond with DM_EN_ENABLE_ENHANCEMENTS_CFM.
 *      NB: It may be dangerous to enable/disable particular enhancements
 *          during particular operations.  For this reason, it is recommended
 *          that all modifications are carried out during initialisation.
 *----------------------------------------------------------------------------*/
#define ENHANCEMENT_ACL_INDICATION  ((uint32_t)0x00000001)
#define ENHANCEMENT_ACL_DETACH      ((uint32_t)0x00000002)
#define ENHANCEMENT_SDC_ERROR_CODES ((uint32_t)0x00000004) /* see sdc_prim.h */
#define ENHANCEMENT_FSM_DEBUG       ((uint32_t)0x00000008)

typedef struct
{
    dm_prim_t       type;
    uint32_t        enhancements;
} DM_EN_ENABLE_ENHANCEMENTS_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Response to a DM_EN_ENABLE_ENHANCEMENTS_REQ.
 *      'enabled_enhancements' contains the currently enabled enhancements.
 *      (Bluestack will only allow supported enhancements to be enabled!)
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t       type;
    uint32_t        enabled_enhancements;
} DM_EN_ENABLE_ENHANCEMENTS_CFM_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read the currently enabled enhancements.
 *      Bluestack will respond with DM_EN_READ_ENHANCEMENTS_CFM
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t       type;
} DM_EN_READ_ENHANCEMENTS_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Generated in response to a DM_EN_READ_ENHANCEMENTS_REQ.
 *      'enabled_enhancements' indicates which enhancements are enabled.
 *      'supported_enhancements' indicates the enhancements supported.
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t       type;
    uint32_t        enabled_enhancements;
    uint32_t        supported_enhancements;
} DM_EN_READ_ENHANCEMENTS_CFM_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *      An enhanced version of DM_ACL_OPENED_IND (see original).
 *      This replaces the old event and contains the HCI status and is
 *      sent on connection failure as well as success.
 *
 *      Enabled by ENHANCEMENT_ACL_INDICATION.
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_ACL_OPENED_IND */
    phandle_t               phandle;        /* Destination phandle */
    BD_ADDR_T               bd_addr;        /* Peer device address */

    /* Flag indicating peer-initiated ACL (TRUE) or locally-initiated (FALSE) */
    bool_t                  incoming;
    /* Class of device for peer, valid for incoming connections only */
    uint24_t                dev_class;

    uint8_t                 status;         /* HCI status */
} DM_EN_ACL_OPENED_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      An enhanced version of DM_ACL_CLOSED_IND (see original).
 *      This replaces the original event and contains the HCI reason for
 *      the ACL being closed.
 *
 *      Enabled by ENHANCEMENT_ACL_INDICATION.
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_ACL_CLOSED_IND */
    phandle_t               phandle;        /* Destination phandle */
    BD_ADDR_T               bd_addr;        /* Peer device address */
    uint8_t                 reason;          /* HCI reason code */
} DM_EN_ACL_CLOSED_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request to immediately disconnect an ACL, even if L2CAP/RFCOMM/SDP
 *      is active, and so this command should only be used in extreme cases.
 *      There is no explicit confirmation, however the closing of the ACL
 *      should cause other indications to occur.
 *
 *      Enabled by ENHANCEMENT_ACL_DETACH.
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;           /* Always DM_EN_ACL_DETACH_REQ */
    phandle_t               phandle;        /* Destination phandle */
    BD_ADDR_T               bd_addr;        /* Peer device address */
    hci_reason_t            reason;         /* HCI reason for the disconnect */
} DM_EN_ACL_DETACH_REQ_T;

/*---------------------------------------------------------------------------*
 * PURPOSE
 *      FSM Debugging primitives. Provides details of Bluestack state 
 *      machine transitions.
 *      Will not be compiled into production firmware.
 *---------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t       type;
    phandle_t       phandle;
    uint16_t        state_machine;
    uint32_t        event;
    uint32_t        old_state;
    uint32_t        new_state;
    uint32_t        action;
} DM_EN_BLUESTACK_STATE_TRANSITION_ENTRY_IND_T;

typedef struct
{
    dm_prim_t       type;
    phandle_t       phandle;
    uint16_t        state_machine;
    uint32_t        event;
} DM_EN_BLUESTACK_STATE_TRANSITION_EXIT_IND_T;

#endif /* ifndef EXCLUDE_BLUESTACK_ENHANCEMENTS */

/******************************************************************************
   Host Controller and Baseband
 *****************************************************************************/
/*
 * These primitives will map directly onto HCI primitives
 *
 */
typedef HCI_SET_EVENT_MASK_T DM_HCI_SET_EVENT_MASK_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of set event mask
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_SET_EVENT_MASK_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */

} DM_HCI_SET_EVENT_MASK_COMPLETE_T;

typedef HCI_RESET_T DM_HCI_RESET_T;
/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of reset command
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_RESET_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_RESET_COMPLETE_T;

typedef HCI_SET_EVENT_FILTER_T DM_HCI_SET_EVENT_FILTER_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request for flush
 *
 *----------------------------------------------------------------------------*/
typedef HCI_FLUSH_T DM_HCI_FLUSH_T;

        /* Always DM_HCI_SET_EVENT_FILTER_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_SET_EVENT_FILTER_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of flush complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_FLUSH_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    BD_ADDR_T           bd_addr;            /* bd_addr of link */

} DM_HCI_FLUSH_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Notification of set event filter command
 *
 *----------------------------------------------------------------------------*/
        
typedef HCI_READ_PIN_TYPE_T DM_HCI_READ_PIN_TYPE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read pin type complete event
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_PIN_TYPE_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    pin_type_t          pin_type;           /* the pin type */

} DM_HCI_READ_PIN_TYPE_COMPLETE_T;

typedef HCI_WRITE_PIN_TYPE_T DM_HCI_WRITE_PIN_TYPE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write pin type complete event
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_WRITE_PIN_TYPE_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_WRITE_PIN_TYPE_COMPLETE_T;

typedef HCI_CREATE_NEW_UNIT_KEY_T DM_HCI_CREATE_NEW_UNIT_KEY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Create new unit key complete event
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_CREATE_NEW_UNIT_KEY_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_CREATE_NEW_UNIT_KEY_COMPLETE_T;

typedef HCI_READ_STORED_LINK_KEY_T DM_HCI_READ_STORED_LINK_KEY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read stored link key complete event
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_STORED_LINK_KEY_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint16_t            max_num_keys;
    uint16_t            num_keys_read;

} DM_HCI_READ_STORED_LINK_KEY_COMPLETE_T;

typedef HCI_WRITE_STORED_LINK_KEY_T DM_HCI_WRITE_STORED_LINK_KEY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write stored link key complete event
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_WRITE_STORED_LINK_KEY_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint8_t             num_keys_written;

} DM_HCI_WRITE_STORED_LINK_KEY_COMPLETE_T;

typedef HCI_DELETE_STORED_LINK_KEY_T DM_HCI_DELETE_STORED_LINK_KEY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Delete stored link keys complete event
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_DELETE_STORED_LINK_KEY_COMPLETE */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint16_t            num_keys_deleted;

} DM_HCI_DELETE_STORED_LINK_KEY_COMPLETE_T;

typedef HCI_CHANGE_LOCAL_NAME_T DM_HCI_CHANGE_LOCAL_NAME_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Change local name complete event
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_CHANGE_LOCAL_NAME_COMPLETE */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_CHANGE_LOCAL_NAME_COMPLETE_T;

typedef HCI_READ_CONN_ACCEPT_TIMEOUT_T DM_HCI_READ_CONN_ACCEPT_TO_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      read local name 
 *
 *----------------------------------------------------------------------------*/

typedef HCI_READ_LOCAL_NAME_T DM_HCI_READ_LOCAL_NAME_T;

typedef struct 
{
    dm_prim_t           type;               /* Always DM_HCI_READ_LOCAL_NAME_COMPLETE */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    /* See HCI_READ_LOCAL_NAME_RET_T */
    uint8_t            *name_part[HCI_LOCAL_NAME_BYTE_PACKET_PTRS]; 
} DM_HCI_READ_LOCAL_NAME_COMPLETE_T;

typedef HCI_WRITE_CONN_ACCEPT_TIMEOUT_T DM_HCI_WRITE_CONN_ACCEPT_TO_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read connection accept timeout complete event
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_CONN_ACCEPT_TO_COMPLETE */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint16_t            conn_accept_timeout;/* the timeout value */

} DM_HCI_READ_CONN_ACCEPT_TO_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write connection accept timeout complete event
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_WRITE_CONN_ACCEPT_TO_COMPLETE */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_WRITE_CONN_ACCEPT_TO_COMPLETE_T;

typedef HCI_READ_PAGE_TIMEOUT_T DM_HCI_READ_PAGE_TO_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read page timeout complete event
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_PAGE_TO_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint16_t            page_timeout;       /* the timeout value */

} DM_HCI_READ_PAGE_TO_COMPLETE_T;

typedef HCI_WRITE_PAGE_TIMEOUT_T DM_HCI_WRITE_PAGE_TO_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write page timeout complete event
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_WRITE_PAGE_TO_COMPLETE */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_WRITE_PAGE_TO_COMPLETE_T;

typedef HCI_READ_SCAN_ENABLE_T DM_HCI_READ_SCAN_ENABLE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read scan enable complete event
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_SCAN_ENABLE_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint8_t             scan_enable;        /* return parameter */

} DM_HCI_READ_SCAN_ENABLE_COMPLETE_T;

typedef HCI_WRITE_SCAN_ENABLE_T DM_HCI_WRITE_SCAN_ENABLE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write scan enable complete event
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_READ_SCAN_ENABLE_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_WRITE_SCAN_ENABLE_COMPLETE_T;

typedef HCI_READ_PAGESCAN_ACTIVITY_T DM_HCI_READ_PAGESCAN_ACTIVITY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read pagescan activity complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_PAGESCAN_ACTIVITY_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint16_t            pagescan_interval;
    uint16_t            pagescan_window;

} DM_HCI_READ_PAGESCAN_ACTIVITY_COMPLETE_T;

typedef HCI_WRITE_PAGESCAN_ACTIVITY_T DM_HCI_WRITE_PAGESCAN_ACTIVITY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write pagescan activity complete
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_WRITE_PAGESCAN_ACTIVITY_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_WRITE_PAGESCAN_ACTIVITY_COMPLETE_T;

typedef HCI_READ_INQUIRYSCAN_ACTIVITY_T DM_HCI_READ_INQUIRYSCAN_ACTIVITY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read inquiryscan activity complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_INQUIRYSCAN_ACTIVITY_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint16_t            inqscan_interval;
    uint16_t            inqscan_window;

} DM_HCI_READ_INQUIRYSCAN_ACTIVITY_COMPLETE_T;

typedef HCI_WRITE_INQUIRYSCAN_ACTIVITY_T DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write inquiryscan activity complete
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_COMPLETE_T;

typedef HCI_READ_AUTH_ENABLE_T DM_HCI_READ_AUTH_ENABLE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read authentication enable complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_AUTH_ENABLE_COMPLETE */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint8_t             auth_enable;

} DM_HCI_READ_AUTH_ENABLE_COMPLETE_T;

typedef HCI_READ_ENC_MODE_T DM_HCI_READ_ENCRYPTION_MODE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read encryption mode complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_ENCRYPTION_MODE_COMPLETE */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint8_t             mode;

} DM_HCI_READ_ENCRYPTION_MODE_COMPLETE_T;

typedef HCI_READ_CLASS_OF_DEVICE_T DM_HCI_READ_CLASS_OF_DEVICE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read class of device complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_CLASS_OF_DEVICE_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint32_t            dev_class;

} DM_HCI_READ_CLASS_OF_DEVICE_COMPLETE_T;

typedef HCI_WRITE_CLASS_OF_DEVICE_T DM_HCI_WRITE_CLASS_OF_DEVICE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write class of device complete
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_WRITE_CLASS_OF_DEVICE_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_WRITE_CLASS_OF_DEVICE_COMPLETE_T;

typedef HCI_READ_VOICE_SETTING_T DM_HCI_READ_VOICE_SETTING_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read voice setting complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_VOICE_SETTING_COMPLETE */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint16_t            voice_setting;

} DM_HCI_READ_VOICE_SETTING_COMPLETE_T;

typedef HCI_WRITE_VOICE_SETTING_T DM_HCI_WRITE_VOICE_SETTING_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write voice setting complete
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_WRITE_VOICE_SETTING_COMPLETE */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_WRITE_VOICE_SETTING_COMPLETE_T;

typedef HCI_READ_NUM_BCAST_RETXS_T DM_HCI_READ_NUM_BCAST_TXS_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request for Reading the automatic flush timeout
 *
 *----------------------------------------------------------------------------*/
typedef HCI_READ_AUTO_FLUSH_TIMEOUT_T DM_HCI_READ_AUTO_FLUSH_TIMEOUT_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read Auto Flush Timeout complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_AUTO_FLUSH_COMPLETE */
    phandle_t           phandle;            /* destination phandle */
    BD_ADDR_T           bd_addr;            /* bd_addr of link (Allways ACL)*/
    uint8_t             status;             /* status */
    uint16_t            FlushTimeout;       /* 11 bits of this number * 0.625ms */

} DM_HCI_READ_AUTO_FLUSH_COMPLETE_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

/* This definition is provided for naming consistency */
typedef DM_HCI_READ_AUTO_FLUSH_COMPLETE_T DM_HCI_READ_AUTO_FLUSH_TIMEOUT_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request for Writing the automatic flush timeout
 *
 *----------------------------------------------------------------------------*/
typedef HCI_WRITE_AUTO_FLUSH_TIMEOUT_T DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write Auto Flush Timeout complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_WRITE_AUTO_FLUSH_TIMEOUT */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    BD_ADDR_T           bd_addr;            /* bd_addr of link (Allways ACL)*/

} DM_HCI_WRITE_AUTO_FLUSH_COMPLETE_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

/* This definition is provided for naming consistency */
typedef DM_HCI_WRITE_AUTO_FLUSH_COMPLETE_T DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request for Reading SCO Flow Control Settings
 *
 *----------------------------------------------------------------------------*/
typedef HCI_READ_SCO_FLOW_CON_ENABLE_T DM_HCI_READ_SCO_FLOW_CONTROL_ENABLE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write Auto Flush Timeout complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_NUM_BCAST_TXS_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint8_t             FlowControlEnabled; 

} DM_HCI_READ_SCO_FLOW_CONTROL_COMPLETE_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Request for Writing SCO Flow Control Settings
 *
 *----------------------------------------------------------------------------*/
typedef HCI_WRITE_SCO_FLOW_CON_ENABLE_T DM_HCI_WRITE_SCO_FLOW_CONTROL_ENABLE_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write SCO Flow Control complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_WRITE_SCO_FLOW_CONTROL_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */

} DM_HCI_WRITE_SCO_FLOW_CONTROL_COMPLETE_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read number of broadcast transmissions complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_NUM_BCAST_TXS_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint8_t             num;

} DM_HCI_READ_NUM_BCAST_TXS_COMPLETE_T;

typedef HCI_WRITE_NUM_BCAST_RETXS_T DM_HCI_WRITE_NUM_BCAST_TXS_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write number of broadcast transmissions complete
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_WRITE_NUM_BCAST_TXS_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_WRITE_NUM_BCAST_TXS_COMPLETE_T;

typedef HCI_READ_HOLD_MODE_ACTIVITY_T DM_HCI_READ_HOLD_MODE_ACTIVITY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write number of broadcast transmissions complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_HOLD_MODE_ACTIVITY_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint8_t             activity;

} DM_HCI_READ_HOLD_MODE_ACTIVITY_COMPLETE_T;

typedef HCI_WRITE_HOLD_MODE_ACTIVITY_T DM_HCI_WRITE_HOLD_MODE_ACTIVITY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write hold mode activity complete
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_WRITE_HOLD_MODE_ACTIVITY_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_WRITE_HOLD_MODE_ACTIVITY_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read transmit power level - use the bd_addr to reference the link
 *
 *----------------------------------------------------------------------------*/
typedef HCI_READ_TX_POWER_LEVEL_T DM_HCI_READ_TX_POWER_LEVEL_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read transmit power level complete - use the bd_addr to reference the link
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_TX_POWER_LEVEL_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    BD_ADDR_T           bd_addr;            /* bd_addr of the link */
    uint8_t             pwr_level;

} DM_HCI_READ_TX_POWER_LEVEL_COMPLETE_T;

typedef HCI_SET_HCTOHOST_FLOW_CONTROL_T DM_HCI_SET_HC_TO_HOST_FLOW_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Set hc to host flow complete
 *
 *----------------------------------------------------------------------------*/

        /* DM_HCI_SET_HC_TO_HOST_FLOW_COMPLETE */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_SET_FC_HC_TO_HOST_COMPLETE_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 
/* Required by parseprim so that primitive name can be matched to primitive structure! */
typedef DM_HCI_SET_FC_HC_TO_HOST_COMPLETE_T DM_HCI_SET_HC_TO_HOST_FLOW_COMPLETE_T ;

typedef HCI_HOST_NUM_COMPLETED_PACKETS_T DM_HCI_HOST_NUM_COMPLETED_PACKETS_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Num completed packets complete
 *      Only used if there is an error
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_HOST_NUM_COMPLETED_PACKETS_COMPLETE */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_HOST_NUM_COMPLETED_PACKETS_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      read link supervision timeout 
 *
 *----------------------------------------------------------------------------*/

typedef HCI_READ_LINK_SUPERV_TIMEOUT_T DM_HCI_READ_LINK_SUPERV_TIMEOUT_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      read link supervision timeout complete
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
    dm_prim_t           type;               /* Always DM_HCI_READ_LINK_SUPERV_TIMEOUT_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    BD_ADDR_T           bd_addr;
    uint16_t            timeout;

} DM_HCI_READ_LINK_SUPERV_TIMEOUT_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Write link supervision timeout
 *
 *----------------------------------------------------------------------------*/

typedef HCI_WRITE_LINK_SUPERV_TIMEOUT_T DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      write link supervision timeout complete
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
    dm_prim_t           type;               /* Always DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    BD_ADDR_T           bd_addr;

} DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_COMPLETE_T;

typedef HCI_READ_NUM_SUPPORTED_IAC_T DM_HCI_READ_NUM_SUPPORTED_IAC_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      read num suppport iac complete
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
    dm_prim_t           type;               /* Always DM_HCI_READ_NUM_SUPPORTED_IAC_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint8_t             num;

} DM_HCI_READ_NUM_SUPPORTED_IAC_COMPLETE_T;

typedef HCI_READ_CURRENT_IAC_LAP_T DM_HCI_READ_CURRENT_IAC_LAP_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      read current iac lap complete
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
    dm_prim_t           type;               /* Always DM_HCI_READ_CURRENT_IAC_LAP_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint8_t             num_current_iac;

	/* Array of HCI_IAC_LAP_PTRS pointers to HCI_IAC_LAP_PER_PTR iac */
    uint24_t *			iac_lap[HCI_IAC_LAP_PTRS];

} DM_HCI_READ_CURRENT_IAC_LAP_COMPLETE_T;

typedef HCI_WRITE_CURRENT_IAC_LAP_T DM_HCI_WRITE_CURRENT_IAC_LAP_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      write current iac lap complete
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_WRITE_CURRENT_IAC_LAP_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_WRITE_CURRENT_IAC_LAP_COMPLETE_T;
  
typedef HCI_READ_PAGESCAN_PERIOD_MODE_T DM_HCI_READ_PAGESCAN_PERIOD_MODE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      read pagescan period mode complete
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
    dm_prim_t           type;               /* Always DM_HCI_READ_PAGESCAN_PERIOD_MODE_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint8_t             mode;

} DM_HCI_READ_PAGESCAN_PERIOD_MODE_COMPLETE_T;

typedef HCI_WRITE_PAGESCAN_PERIOD_MODE_T DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      write pagescan period mode complete
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_COMPLETE_T;

typedef HCI_READ_PAGESCAN_MODE_T DM_HCI_READ_PAGESCAN_MODE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      read pagescan mode complete
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
    dm_prim_t           type;               /* Always DM_HCI_READ_PAGESCAN_MODE_COMPLETE_T */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint8_t             mode;

} DM_HCI_READ_PAGESCAN_MODE_COMPLETE_T;

typedef HCI_WRITE_PAGESCAN_MODE_T DM_HCI_WRITE_PAGESCAN_MODE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      write pagescan mode complete
 *
 *----------------------------------------------------------------------------*/

        /* Always DM_HCI_WRITE_PAGESCAN_MODE_COMPLETE_T */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_WRITE_PAGESCAN_MODE_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      notification of hardware error
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;               /* always DM_HCI_HARDWARE_ERROR */
    phandle_t               phandle;            /* destination phandle */
    uint8_t                 hw_error;           /* TBD */
} DM_HCI_HARDWARE_ERROR_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      notification of data buffer overflow 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;               /* always DM_HCI_DATA_BUFFER_OVERFLOW */
    phandle_t               phandle;            /* destination phandle */
} DM_HCI_DATA_BUFFER_OVERFLOW_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      max slots change event
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;               /* always DM_HCI_MAX_SLOTS_CHANGE */
    phandle_t               phandle;            /* destination phandle */
    uint8_t                 lmp_max_slots;
} DM_HCI_MAX_SLOTS_CHANGE_T;

/******************************************************************************
   Informational parameters
 *****************************************************************************/

typedef HCI_READ_LOCAL_VER_INFO_T DM_HCI_READ_LOCAL_VERSION_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read local version complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_LOCAL_VERSION_COMPLETE */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    hci_version_t       hci_version;
    uint16_t            hci_revision;
    uint8_t             lmp_version;        /* defined in LMP */
    uint16_t            manuf_name;         /* defined in LMP */
    uint16_t            lmp_subversion;

} DM_HCI_READ_LOCAL_VERSION_COMPLETE_T;

typedef HCI_READ_LOCAL_SUPP_FEATURES_T DM_HCI_READ_LOCAL_FEATURES_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read local features complete
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
    dm_prim_t           type;               /* Always DM_HCI_READ_LOCAL_FEATURES_COMPLETE */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint8_t             lmp_supp_features[8]; /* TODO: bit masks! */

} DM_HCI_READ_LOCAL_FEATURES_COMPLETE_T;

typedef HCI_READ_COUNTRY_CODE_T DM_HCI_READ_COUNTRY_CODE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Read country code complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t           type;               /* Always DM_HCI_READ_COUNTRY_CODE_COMPLETE */
    phandle_t           phandle;            /* destination phandle */
    uint8_t             status;             /* status */
    uint8_t             country_code;

} DM_HCI_READ_COUNTRY_CODE_COMPLETE_T;

typedef HCI_READ_BD_ADDR_T DM_HCI_READ_BD_ADDR_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      read bd_addr return primitive
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;
    phandle_t               phandle;            /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    BD_ADDR_T				bd_addr;

} DM_HCI_READ_BD_ADDR_COMPLETE_T;

/******************************************************************************
   Status parameters
 *****************************************************************************/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      read failed contact counter
 *
 *----------------------------------------------------------------------------*/
typedef HCI_READ_FAILED_CONTACT_COUNT_T DM_HCI_FAILED_CONTACT_COUNTER_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      read failed contact counter complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;    /* always DM_HCI_FAILED_CONTACT_COUNTER_COMPLETE */
    phandle_t               phandle; /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    BD_ADDR_T               bd_addr;
    uint16_t                failed_contact_count;

} DM_HCI_FAILED_CONTACT_COUNTER_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      reset contact counter request
 *
 *----------------------------------------------------------------------------*/
typedef HCI_RESET_FAILED_CONTACT_COUNT_T DM_HCI_RESET_CONTACT_COUNTER_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      reset contact counter complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;    /* always DM_HCI_RESET_CONTACT_COUNTER_COMPLETE */
    phandle_t               phandle; /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    BD_ADDR_T               bd_addr;

} DM_HCI_RESET_CONTACT_COUNTER_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      get link quality
 *
 *----------------------------------------------------------------------------*/
typedef HCI_GET_LINK_QUALITY_T DM_HCI_GET_LINK_QUALITY_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      get link quality complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;    /* always DM_HCI_GET_LINK_QUALITY_COMPLETE */
    phandle_t               phandle; /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    BD_ADDR_T               bd_addr;
    uint8_t                 link_quality;

} DM_HCI_GET_LINK_QUALITY_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      read RSSI
 *
 *----------------------------------------------------------------------------*/
typedef HCI_READ_RSSI_T DM_HCI_READ_RSSI_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      read RSSI complete
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;    /* always DM_HCI_READ_RSSI_COMPLETE */
    phandle_t               phandle; /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    BD_ADDR_T               bd_addr;
    int8_t                  rssi;
} DM_HCI_READ_RSSI_COMPLETE_T;

/******************************************************************************
   Testing primitives
 *****************************************************************************/
/*
 * These primitives will map directly onto HCI primitives
 *
 */

typedef HCI_ENABLE_DUT_MODE_T DM_HCI_ENABLE_DEVICE_UT_MODE_T;            

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Enable device under test complete
 *
 *----------------------------------------------------------------------------*/

        /* DM_HCI_ENABLE_DEVICE_UT_MODE_COMPLETE */
typedef DM_HCI_STANDARD_COMMAND_COMPLETE_T DM_HCI_ENABLE_DEVICE_UT_MODE_COMPLETE_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Common elements of an "upstream" primitive
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type; 
    phandle_t               phandle;
} DM_UPSTREAM_COMMON_T; /* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

typedef struct
{
    dm_prim_t               type;
    phandle_t               phandle;
    BD_ADDR_T               bd_addr;
} DM_UPSTREAM_BD_ADDR_COMMON_T; /* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */

typedef HCI_CREATE_CONNECTION_CANCEL_T DM_HCI_CREATE_CONNECTION_CANCEL_T;
#define DM_HCI_CREATE_CONNECTION_CANCEL ((dm_prim_t)(ENUM_DM_HCI_CREATE_CONNECTION_CANCEL)) 
#define DM_HCI_CREATE_CONNECTION_CANCEL_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_CREATE_CONNECTION_CANCEL_COMPLETE)) 

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_CREATE_CONNECTION_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */     BD_ADDR_T              bd_addr ;

} DM_HCI_CREATE_CONNECTION_CANCEL_COMPLETE_T;

typedef HCI_REMOTE_NAME_REQ_CANCEL_T DM_HCI_REMOTE_NAME_REQ_CANCEL_T;
#define DM_HCI_REMOTE_NAME_REQ_CANCEL ((dm_prim_t)(ENUM_DM_HCI_REMOTE_NAME_REQ_CANCEL))
#define DM_HCI_REMOTE_NAME_REQ_CANCEL_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_REMOTE_NAME_REQ_CANCEL_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_REMOTE_NAME_REQ_CANCEL_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
     BD_ADDR_T              bd_addr ;

} DM_HCI_REMOTE_NAME_REQ_CANCEL_COMPLETE_T;


typedef HCI_READ_REMOTE_EXT_FEATURES_T DM_HCI_READ_REMOTE_EXT_FEATURES_T;
#define DM_HCI_READ_REMOTE_EXT_FEATURES ((dm_prim_t)(ENUM_DM_HCI_READ_REMOTE_EXT_FEATURES))
#define DM_HCI_READ_REMOTE_EXT_FEATURES_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_READ_REMOTE_EXT_FEATURES_COMPLETE))

typedef struct
{
     dm_prim_t               type;           /* DM_HCI_READ_REMOTE_EXT_FEATURES_COMPLETE_T */
     phandle_t               phandle;        /* destination phandle */
     uint8_t                 status;         /* Success or failure - See Note1 */
     BD_ADDR_T               bd_addr ;
     uint8_t                 page_num;
     uint8_t                 max_page_num;
     uint16_t                lmp_ext_features[4];

} DM_HCI_READ_REMOTE_EXT_FEATURES_COMPLETE_T;


typedef HCI_READ_LMP_HANDLE_T DM_HCI_READ_LMP_HANDLE_T;
#define DM_HCI_READ_LMP_HANDLE ((dm_prim_t)(ENUM_DM_HCI_READ_LMP_HANDLE))
#define DM_HCI_READ_LMP_HANDLE_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_READ_LMP_HANDLE_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_READ_LMP_HANDLE_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    uint8_t                 lmp_handle;
    uint32_t                reserved;

} DM_HCI_READ_LMP_HANDLE_COMPLETE_T;


typedef HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_T DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_T;
#define DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS ((dm_prim_t)(ENUM_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS))
#define DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    link_policy_settings_t  default_lps;   

} DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_COMPLETE_T;


typedef HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_T DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_T;
#define DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS ((dm_prim_t)(ENUM_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS))
#define DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */

} DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_COMPLETE_T;


typedef HCI_FLOW_SPEC_T DM_HCI_FLOW_SPEC_T;
#define DM_HCI_FLOW_SPEC ((dm_prim_t)(ENUM_DM_HCI_FLOW_SPEC))
#define DM_HCI_FLOW_SPEC_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_FLOW_SPEC_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_FLOW_SPEC_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    BD_ADDR_T              bd_addr ;
    uint8_t		    flags;
    uint8_t		    flow_direction; /* 0=out (to air), 1=in (from air) */
    uint8_t		    service_type;
    uint32_t		    token_rate;
    uint32_t		    token_bucket_size;
    uint32_t		    peak_bandwidth;
    uint32_t		    access_latency;    
} DM_HCI_FLOW_SPEC_COMPLETE_T;

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_SNIFF_SUB_RATING_IND */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;
    BD_ADDR_T               bd_addr;
    uint16_t                transmit_latency;
    uint16_t                receive_latency;
    uint16_t                remote_timeout;
    uint16_t                local_timeout;
} DM_HCI_SNIFF_SUB_RATING_IND_T;
typedef HCI_SNIFF_SUB_RATE_T DM_HCI_SNIFF_SUB_RATE_T;
#define DM_HCI_SNIFF_SUB_RATE ((dm_prim_t)(ENUM_DM_HCI_SNIFF_SUB_RATE))
#define DM_HCI_SNIFF_SUB_RATE_COMPLETE \
                            ((dm_prim_t)(ENUM_DM_HCI_SNIFF_SUB_RATE_COMPLETE))
#define DM_HCI_SNIFF_SUB_RATING_IND \
                            ((dm_prim_t)(ENUM_DM_HCI_SNIFF_SUB_RATING_IND))
typedef struct
{
    dm_prim_t               type;   /* DM_HCI_SNIFF_SUB_RATE_COMPLETE */
    phandle_t               phandle;
    uint8_t                 status;
    BD_ADDR_T              bd_addr;
} DM_HCI_SNIFF_SUB_RATE_COMPLETE_T;

typedef HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_T DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_T;
#define DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA \
                ((dm_prim_t)(ENUM_DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA))
typedef HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_T DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_T;
#define DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA \
                ((dm_prim_t)(ENUM_DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA))

typedef struct
{
    /* DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_COMPLETE */
    dm_prim_t               type;
    uint16_t                phandle;
    uint8_t                 status;
    uint8_t                 fec_required;
    uint8_t                 *eir_data_part[HCI_EIR_DATA_PACKET_PTRS];
} DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_COMPLETE_T;

#define DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_COMPLETE \
    ((dm_prim_t)(ENUM_DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_COMPLETE))

typedef struct
{
    /* DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_COMPLETE */
    dm_prim_t               type;
    uint16_t                phandle;
    uint8_t                 status;
} DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_COMPLETE_T;

#define DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_COMPLETE \
    ((dm_prim_t)(ENUM_DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_COMPLETE))

typedef struct
{
    dm_prim_t               type;   /* DM_HCI_EXTENDED_INQUIRY_RESULT_IND */
    uint16_t                phandle;
    uint8_t                 num_responses;
    HCI_INQ_RESULT_WITH_RSSI_T  result;
    uint8_t                 *eir_data_part[HCI_EIR_DATA_PACKET_PTRS];
} DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T;

#define DM_HCI_EXTENDED_INQUIRY_RESULT_IND \
    ((dm_prim_t)(ENUM_DM_HCI_EXTENDED_INQUIRY_RESULT_IND))

typedef struct
{
    dm_prim_t               type;   /* DM_HCI_ENHANCED_FLUSH_COMPLETE */
    uint16_t                phandle;
    uint8_t                 status;
    BD_ADDR_T               bd_addr;
} DM_HCI_ENHANCED_FLUSH_COMPLETE_T;

typedef HCI_ENHANCED_FLUSH_T DM_HCI_ENHANCED_FLUSH_T;
#define DM_HCI_ENHANCED_FLUSH  ((dm_prim_t)(ENUM_DM_HCI_ENHANCED_FLUSH))

#define DM_HCI_ENHANCED_FLUSH_COMPLETE \
                            ((dm_prim_t)(ENUM_DM_HCI_ENHANCED_FLUSH_COMPLETE))

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Event generated when the link supervision timeout for an ACL changes.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    dm_prim_t               type;   /* DM_HCI_LINK_SUPERV_TIMEOUT_IND */
    uint16_t                phandle;
    BD_ADDR_T               bd_addr;
    uint16_t                timeout;
} DM_HCI_LINK_SUPERV_TIMEOUT_IND_T;
#define DM_HCI_LINK_SUPERV_TIMEOUT_IND ((dm_prim_t)(ENUM_DM_HCI_LINK_SUPERV_TIMEOUT_IND))

typedef HCI_SET_AFH_CHANNEL_CLASS_T DM_HCI_SET_AFH_CHANNEL_CLASS_T;
#define DM_HCI_SET_AFH_CHANNEL_CLASS ((dm_prim_t)(ENUM_DM_HCI_SET_AFH_CHANNEL_CLASS))
#define DM_HCI_SET_AFH_CHANNEL_CLASS_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_SET_AFH_CHANNEL_CLASS_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_SET_AFH_CHANNEL_CLASS_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
} DM_HCI_SET_AFH_CHANNEL_CLASS_COMPLETE_T;


typedef HCI_READ_INQUIRY_SCAN_TYPE_T DM_HCI_READ_INQUIRY_SCAN_TYPE_T;
#define DM_HCI_READ_INQUIRY_SCAN_TYPE ((dm_prim_t)(ENUM_DM_HCI_READ_INQUIRY_SCAN_TYPE))
#define DM_HCI_READ_INQUIRY_SCAN_TYPE_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_READ_INQUIRY_SCAN_TYPE_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_READ_INQUIRY_SCAN_TYPE_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
     uint8_t                mode ;
} DM_HCI_READ_INQUIRY_SCAN_TYPE_COMPLETE_T;


typedef HCI_WRITE_INQUIRY_SCAN_TYPE_T DM_HCI_WRITE_INQUIRY_SCAN_TYPE_T;
#define DM_HCI_WRITE_INQUIRY_SCAN_TYPE ((dm_prim_t)(ENUM_DM_HCI_WRITE_INQUIRY_SCAN_TYPE))
#define DM_HCI_WRITE_INQUIRY_SCAN_TYPE_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_WRITE_INQUIRY_SCAN_TYPE_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_WRITE_INQUIRY_SCAN_TYPE_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
} DM_HCI_WRITE_INQUIRY_SCAN_TYPE_COMPLETE_T;


typedef HCI_READ_INQUIRY_MODE_T DM_HCI_READ_INQUIRY_MODE_T;
#define DM_HCI_READ_INQUIRY_MODE ((dm_prim_t)(ENUM_DM_HCI_READ_INQUIRY_MODE))
#define DM_HCI_READ_INQUIRY_MODE_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_READ_INQUIRY_MODE_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_READ_INQUIRY_MODE_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    uint8_t                  mode;
} DM_HCI_READ_INQUIRY_MODE_COMPLETE_T;


typedef HCI_WRITE_INQUIRY_MODE_T DM_HCI_WRITE_INQUIRY_MODE_T;
#define DM_HCI_WRITE_INQUIRY_MODE ((dm_prim_t)(ENUM_DM_HCI_WRITE_INQUIRY_MODE))
#define DM_HCI_WRITE_INQUIRY_MODE_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_WRITE_INQUIRY_MODE_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_WRITE_INQUIRY_MODE_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
} DM_HCI_WRITE_INQUIRY_MODE_COMPLETE_T;


typedef HCI_READ_PAGE_SCAN_TYPE_T DM_HCI_READ_PAGE_SCAN_TYPE_T;
#define DM_HCI_READ_PAGE_SCAN_TYPE ((dm_prim_t)(ENUM_DM_HCI_READ_PAGE_SCAN_TYPE))
#define DM_HCI_READ_PAGE_SCAN_TYPE_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_READ_PAGE_SCAN_TYPE_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_READ_PAGE_SCAN_TYPE_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    uint8_t                  mode;
} DM_HCI_READ_PAGE_SCAN_TYPE_COMPLETE_T;


typedef HCI_WRITE_PAGE_SCAN_TYPE_T DM_HCI_WRITE_PAGE_SCAN_TYPE_T;
#define DM_HCI_WRITE_PAGE_SCAN_TYPE ((dm_prim_t)(ENUM_DM_HCI_WRITE_PAGE_SCAN_TYPE))
#define DM_HCI_WRITE_PAGE_SCAN_TYPE_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_WRITE_PAGE_SCAN_TYPE_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_WRITE_PAGE_SCAN_TYPE_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */

} DM_HCI_WRITE_PAGE_SCAN_TYPE_COMPLETE_T;


typedef HCI_READ_AFH_CHANNEL_CLASS_M_T DM_HCI_READ_AFH_CHANNEL_CLASS_M_T;
#define DM_HCI_READ_AFH_CHANNEL_CLASS_M ((dm_prim_t)(ENUM_DM_HCI_READ_AFH_CHANNEL_CLASS_M))
#define DM_HCI_READ_AFH_CHANNEL_CLASS_M_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_READ_AFH_CHANNEL_CLASS_M_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_READ_AFH_CHANNEL_CLASS_M_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    uint8_t                  class_mode;

} DM_HCI_READ_AFH_CHANNEL_CLASS_M_COMPLETE_T;


typedef HCI_WRITE_AFH_CHANNEL_CLASS_M_T DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_T;
#define DM_HCI_WRITE_AFH_CHANNEL_CLASS_M ((dm_prim_t)(ENUM_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M))
#define DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
} DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_COMPLETE_T;


typedef HCI_READ_LOCAL_EXT_FEATURES_T DM_HCI_READ_LOCAL_EXT_FEATURES_T;
#define DM_HCI_READ_LOCAL_EXT_FEATURES ((dm_prim_t)(ENUM_DM_HCI_READ_LOCAL_EXT_FEATURES))
#define DM_HCI_READ_LOCAL_EXT_FEATURES_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_READ_LOCAL_EXT_FEATURES_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_READ_LOCAL_EXT_FEATURES_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
    uint8_t                  page_num;
    uint8_t                  max_page_num;
    uint8_t                  lmp_ext_features[8]; /* TODO: bit masks! */
  

} DM_HCI_READ_LOCAL_EXT_FEATURES_COMPLETE_T;


typedef HCI_READ_AFH_CHANNEL_MAP_T DM_HCI_READ_AFH_CHANNEL_MAP_T;
#define DM_HCI_READ_AFH_CHANNEL_MAP ((dm_prim_t)(ENUM_DM_HCI_READ_AFH_CHANNEL_MAP))
#define DM_HCI_READ_AFH_CHANNEL_MAP_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_READ_AFH_CHANNEL_MAP_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_READ_AFH_CHANNEL_MAP_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
     BD_ADDR_T              bd_addr ;
    uint8_t                  mode;
    uint8_t                  map[10];
} DM_HCI_READ_AFH_CHANNEL_MAP_COMPLETE_T;


typedef HCI_READ_CLOCK_T DM_HCI_READ_CLOCK_T;
#define DM_HCI_READ_CLOCK ((dm_prim_t)(ENUM_DM_HCI_READ_CLOCK))
#define DM_HCI_READ_CLOCK_COMPLETE ((dm_prim_t)(ENUM_DM_HCI_READ_CLOCK_COMPLETE))

typedef struct
{
    dm_prim_t               type;           /* DM_HCI_READ_CLOCK_COMPLETE_T */
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;         /* Success or failure - See Note1 */
     BD_ADDR_T              bd_addr ;
    uint32_t                 clock;
    uint16_t                 accuracy;

} DM_HCI_READ_CLOCK_COMPLETE_T;

#define DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ \
    ((dm_prim_t)(ENUM_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ))
#define DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ \
    ((dm_prim_t)(ENUM_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ))
#define DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_COMPLETE \
    ((dm_prim_t)(ENUM_DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_COMPLETE))
#define DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_COMPLETE \
    ((dm_prim_t)(ENUM_DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_COMPLETE))

typedef HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_T DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_T;
typedef HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_T DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_T;

typedef struct
{
    /* Always DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_COMPLETE */
    dm_prim_t               type;
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;
    uint8_t                 tx_power;
} DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_COMPLETE_T;

typedef struct
{
    /* Always DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_COMPLETE */
    dm_prim_t               type;
    phandle_t               phandle;        /* destination phandle */
    uint8_t                 status;
} DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_COMPLETE_T;

#define DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND \
    ((dm_prim_t)(ENUM_DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND))
typedef struct
{
    /* Always DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND */
    dm_prim_t               type;
    phandle_t               phandle;        /* destination phandle */
    BD_ADDR_T               bd_addr;
    uint16_t                host_features[4];
} DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_T;

/*------------------------------------------------------------------------
 *
 *      UNION OF       PRIMITIVES
 *
 *-----------------------------------------------------------------------*/
typedef union
{
    dm_prim_t                               type;     
    DM_UPSTREAM_COMMON_T                    dm_upstream_common ;
    DM_UPSTREAM_BD_ADDR_COMMON_T            dm_upstream_bd_addr_common;

    DM_AM_REGISTER_REQ_T                    dm_am_register_req;
    DM_AM_REGISTER_CFM_T                    dm_am_register_cfm;
    DM_AM_REGISTER_WRONG_API_CFM_T          dm_am_register_wrong_api_cfm;
    DM_AM_REGISTER_WRONG_API_REQ_T          dm_am_register_wrong_api_req;
    DM_SET_BT_VERSION_REQ_T                 dm_set_bt_version_req;
    DM_SET_BT_VERSION_CFM_T                 dm_set_bt_version_cfm;
    DM_HC_TO_HOST_FLOW_CONTROL_T            dm_hc_to_host_fc;
    DM_ACL_REGISTER_REQ_T                   dm_acl_register_req;
    DM_ACL_REGISTER_CFM_T                   dm_acl_register_cfm;
    DM_ACL_CONNECT_REQ_T                    dm_acl_connect_req;
    DM_ACL_CONNECT_CFM_T                    dm_acl_connect_cfm;
    DM_ACL_CONNECT_IND_T                    dm_acl_connect_ind;
    DM_ACL_DISCONNECT_REQ_T                 dm_acl_disconnect_req;
    DM_ACL_DISCONNECT_IND_T                 dm_acl_disconnect_ind;
    DM_ACL_BUFFER_SIZE_IND_T                dm_acl_buffer_size_ind;
    DM_ACL_DATA_SENT_IND_T                  dm_acl_data_sent_ind;
    DM_HCI_REMOTE_NAME_COMPLETE_T           dm_remote_name_complete;

    DM_HCI_INQUIRY_RESULT_T                 dm_inquiry_result;
    DM_HCI_INQUIRY_RESULT_WITH_RSSI_T       dm_inquiry_result_with_rssi;
    DM_HCI_INQUIRY_COMPLETE_T               dm_inquiry_complete;
    DM_HCI_INQUIRY_CANCEL_COMPLETE_T        dm_inquiry_cancel_complete;
    DM_HCI_PERIODIC_INQUIRY_COMPLETE_T      dm_periodic_inquiry_complete;
    DM_HCI_EXIT_PERIODIC_INQUIRY_COMPLETE_T dm_exit_periodic_inquiry_complete;
    DM_HCI_CHANGE_LINK_KEY_T                dm_change_link_key;
    DM_HCI_LINK_KEY_CHANGE_COMPLETE_T	    dm_link_key_change_complete;
    DM_HCI_RETURN_LINK_KEYS_T		    dm_return_link_keys;
    DM_HCI_MASTER_LINK_KEY_COMPLETE_T       dm_master_link_key_complete;
    DM_HCI_REMOTE_NAME_REQUEST_T            dm_remote_name_request;
    DM_HCI_READ_REMOTE_FEATURES_T           dm_read_remote_features;
    DM_HCI_READ_REMOTE_FEATURES_COMPLETE_T  dm_read_remote_features_complete;
    DM_HCI_READ_REMOTE_VERSION_T            dm_read_remote_version;
    DM_HCI_READ_REMOTE_VERSION_COMPLETE_T   dm_read_remote_version_complete;
    DM_HCI_READ_CLOCK_OFFSET_T              dm_read_clock_offset;
    DM_HCI_READ_CLOCK_OFFSET_COMPLETE_T     dm_read_clock_offset_complete;

    DM_HCI_WRITE_LP_SETTINGS_T              dm_write_lp_settings;
    DM_HCI_WRITE_LP_SETTINGS_COMPLETE_T     dm_write_lp_settings_complete;
    DM_HCI_READ_LP_SETTINGS_T               dm_read_lp_settings;
    DM_HCI_READ_LP_SETTINGS_COMPLETE_T      dm_read_lp_settings_complete;
    DM_HCI_ROLE_DISCOVERY_T                 dm_role_discovery;
    DM_HCI_ROLE_DISCOVERY_COMPLETE_T        dm_role_discovery_complete;
    DM_HCI_SWITCH_ROLE_COMPLETE_T	    dm_switch_role_complete;
    DM_HCI_MODE_CHANGE_EVENT_T              dm_mode_change_event;
    DM_HCI_SET_EVENT_MASK_COMPLETE_T        dm_set_event_mask_complete;
    DM_HCI_RESET_COMPLETE_T                 dm_reset_complete;
    DM_HCI_SET_EVENT_FILTER_COMPLETE_T      dm_set_event_filter_complete;
    DM_HCI_FLUSH_T                          dm_flush;
    DM_HCI_FLUSH_COMPLETE_T                 dm_flush_complete;
    DM_HCI_READ_PIN_TYPE_COMPLETE_T         dm_read_pin_type_complete;
    DM_HCI_WRITE_PIN_TYPE_COMPLETE_T        dm_write_pin_type_complete;
    DM_HCI_CREATE_NEW_UNIT_KEY_COMPLETE_T   dm_create_new_unit_key_complete;
    DM_HCI_READ_STORED_LINK_KEY_COMPLETE_T  dm_read_stored_link_key_complete;
    DM_HCI_WRITE_STORED_LINK_KEY_COMPLETE_T dm_write_stored_link_key_complete;
    DM_HCI_DELETE_STORED_LINK_KEY_COMPLETE_T        dm_delete_stored_link_key_complete;
    DM_HCI_CHANGE_LOCAL_NAME_COMPLETE_T     dm_change_local_name_complete;
    DM_HCI_READ_LOCAL_NAME_COMPLETE_T       dm_read_local_name_complete;
    DM_HCI_READ_CONN_ACCEPT_TO_COMPLETE_T   dm_read_conn_accept_to_complete;
    DM_HCI_WRITE_CONN_ACCEPT_TO_COMPLETE_T  dm_write_conn_accept_to_complete;
    DM_HCI_READ_PAGE_TO_COMPLETE_T          dm_read_page_to_complete;
    DM_HCI_WRITE_PAGE_TO_COMPLETE_T         dm_write_page_to_complete;
    DM_HCI_READ_SCAN_ENABLE_COMPLETE_T      dm_read_scan_enable_complete;
    DM_HCI_WRITE_SCAN_ENABLE_COMPLETE_T     dm_write_scan_enable_complete;
    DM_HCI_READ_PAGESCAN_ACTIVITY_COMPLETE_T        dm_read_pagescan_activity_complete;
    DM_HCI_WRITE_PAGESCAN_ACTIVITY_COMPLETE_T       dm_write_pagescan_activity_complete;
    DM_HCI_READ_INQUIRYSCAN_ACTIVITY_COMPLETE_T     dm_read_inquiryscan_activity_complete;
    DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_COMPLETE_T    dm_write_inquiryscan_activity_complete;
    DM_HCI_READ_AUTH_ENABLE_COMPLETE_T              dm_read_auth_enable_complete;
    DM_HCI_READ_ENCRYPTION_MODE_COMPLETE_T          dm_read_encryption_mode_complete;
    DM_HCI_READ_CLASS_OF_DEVICE_COMPLETE_T          dm_read_class_of_device_complete;
    DM_HCI_WRITE_CLASS_OF_DEVICE_COMPLETE_T         dm_write_class_of_device_complete;
    DM_HCI_READ_VOICE_SETTING_COMPLETE_T            dm_read_voice_setting_complete;
    DM_HCI_WRITE_VOICE_SETTING_COMPLETE_T           dm_write_voice_setting_complete;
    DM_HCI_READ_AUTO_FLUSH_TIMEOUT_T                dm_read_auto_flush_timeout;
    DM_HCI_READ_AUTO_FLUSH_COMPLETE_T               dm_read_auto_flush_timeout_complete;
    DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_T               dm_write_auto_flush_timeout;
    DM_HCI_WRITE_AUTO_FLUSH_COMPLETE_T              dm_write_auto_flush_timeout_complete;
    DM_HCI_READ_SCO_FLOW_CONTROL_COMPLETE_T         dm_read_sco_flow_control_complete;
    DM_HCI_WRITE_SCO_FLOW_CONTROL_COMPLETE_T        dm_write_sco_flow_control_complete;
    DM_HCI_READ_NUM_BCAST_TXS_COMPLETE_T            dm_read_num_bcast_txs_complete;
    DM_HCI_WRITE_NUM_BCAST_TXS_COMPLETE_T           dm_write_num_bcast_txs_complete;
    DM_HCI_READ_HOLD_MODE_ACTIVITY_COMPLETE_T       dm_read_hold_mode_activity_complete;
    DM_HCI_WRITE_HOLD_MODE_ACTIVITY_COMPLETE_T      dm_write_hold_mode_activity_complete;
    DM_HCI_READ_TX_POWER_LEVEL_T                    dm_read_tx_power_level;
    DM_HCI_READ_TX_POWER_LEVEL_COMPLETE_T           dm_read_tx_power_level_complete;
    DM_HCI_HOST_NUM_COMPLETED_PACKETS_T	            dm_host_num_completed_packets;
    DM_HCI_HOST_NUM_COMPLETED_PACKETS_COMPLETE_T    dm_host_num_completed_pkts_complete;
    DM_HCI_READ_LINK_SUPERV_TIMEOUT_T               dm_read_link_superv_timeout;
    DM_HCI_READ_LINK_SUPERV_TIMEOUT_COMPLETE_T      dm_read_link_superv_timeout_complete;
    DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_T              dm_write_link_superv_timeout;
    DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_COMPLETE_T     dm_write_link_superv_timeout_complete;
    DM_HCI_READ_NUM_SUPPORTED_IAC_COMPLETE_T        dm_read_num_supported_iac_complete;
    DM_HCI_READ_CURRENT_IAC_LAP_COMPLETE_T          dm_read_current_iac_lap_complete;
    DM_HCI_WRITE_CURRENT_IAC_LAP_COMPLETE_T         dm_write_current_iac_lap_complete;
    DM_HCI_READ_PAGESCAN_PERIOD_MODE_COMPLETE_T     dm_read_pagescan_period_mode;
    DM_HCI_WRITE_PAGESCAN_PERIOD_MODE_COMPLETE_T    dm_write_pagescan_period_mode;
    DM_HCI_READ_PAGESCAN_MODE_COMPLETE_T            dm_read_pagescan_mode;
    DM_HCI_WRITE_PAGESCAN_MODE_COMPLETE_T           dm_write_pagescan_mode;
    DM_HCI_HARDWARE_ERROR_T			    dm_hardware_error;
    DM_HCI_DATA_BUFFER_OVERFLOW_T		    dm_data_buffer_overflow;
    DM_HCI_MAX_SLOTS_CHANGE_T			    dm_max_slots_change;

    DM_HCI_READ_LOCAL_VERSION_COMPLETE_T            dm_read_local_version_complete;
    DM_HCI_READ_LOCAL_FEATURES_COMPLETE_T           dm_read_local_features_complete;
    DM_HCI_READ_COUNTRY_CODE_COMPLETE_T             dm_read_country_code_complete;
    DM_HCI_READ_BD_ADDR_COMPLETE_T                  dm_read_bd_addr_complete;

    DM_HCI_QOS_SETUP_REQ_T			    dm_qos_setup;
    DM_HCI_QOS_SETUP_CFM_T			    dm_qos_setup_complete;
    DM_HCI_QOS_VIOLATION_IND_T                      dm_qos_violation_indication;
    DM_HCI_FAILED_CONTACT_COUNTER_T                 dm_failed_contact_counter;
    DM_HCI_FAILED_CONTACT_COUNTER_COMPLETE_T	    dm_failed_contact_counter_complete;
    DM_HCI_RESET_CONTACT_COUNTER_T                  dm_reset_contact_counter;
    DM_HCI_RESET_CONTACT_COUNTER_COMPLETE_T	    dm_reset_contact_counter_compete;
    DM_HCI_GET_LINK_QUALITY_T                       dm_get_link_quality;
    DM_HCI_GET_LINK_QUALITY_COMPLETE_T		    dm_get_link_quality_complete;
    DM_HCI_READ_RSSI_T                              dm_read_rssi;
    DM_HCI_READ_RSSI_COMPLETE_T			    dm_read_rssi_complete;

    DM_LP_WRITE_ROLESWITCH_POLICY_REQ_T                     dm_lp_write_roleswitch_policy_req;
    DM_LP_WRITE_ROLESWITCH_POLICY_CFM_T                     dm_lp_write_roleswitch_policy_cfm;
    DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ_T                 dm_lp_write_always_master_devices_req;
    DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_T                 dm_lp_write_always_master_devices_cfm;

    DM_HCI_ENABLE_DEVICE_UT_MODE_COMPLETE_T dm_enable_device_ut_mode_complete;
    DM_HCI_SET_FC_HC_TO_HOST_COMPLETE_T		dm_flow_control_hc_to_host_complete;
     
    DM_HCI_CREATE_CONNECTION_CANCEL_COMPLETE_T    dm_create_connection_cancel_complete;
    DM_HCI_REMOTE_NAME_REQ_CANCEL_COMPLETE_T      dm_remote_name_req_cancel_complete;
    DM_HCI_READ_PAGE_SCAN_TYPE_COMPLETE_T         dm_read_page_scan_type_complete;
    DM_HCI_WRITE_PAGE_SCAN_TYPE_COMPLETE_T        dm_write_page_scan_type_complete;
    DM_HCI_READ_INQUIRY_SCAN_TYPE_COMPLETE_T      dm_read_inquiry_scan_type_complete;
    DM_HCI_WRITE_INQUIRY_SCAN_TYPE_COMPLETE_T     dm_write_inquiry_scan_type_complete;
    DM_HCI_READ_INQUIRY_MODE_COMPLETE_T           dm_read_inquiry_mode_complete;
    DM_HCI_WRITE_INQUIRY_MODE_COMPLETE_T          dm_write_inquiry_mode_complete;
    DM_HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_COMPLETE_T   dm_read_default_link_policy_settings_complete;
    DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_COMPLETE_T  dm_write_default_link_policy_settings_complete;
    DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_COMPLETE_T   dm_write_afh_channel_class_m_complete;
    DM_HCI_SET_AFH_CHANNEL_CLASS_COMPLETE_T       dm_set_afh_channel_class_complete;

    DM_HCI_READ_LMP_HANDLE_COMPLETE_T             dm_read_lmp_handle_complete;
    DM_HCI_READ_AFH_CHANNEL_CLASS_M_COMPLETE_T    dm_read_afh_channel_class_m_complete;
    DM_HCI_READ_CLOCK_COMPLETE_T                  dm_read_clock_complete;
    DM_HCI_READ_AFH_CHANNEL_MAP_COMPLETE_T        dm_read_afh_channel_map_complete;
    DM_HCI_READ_LOCAL_EXT_FEATURES_COMPLETE_T     dm_read_local_ext_features_complete;
    DM_HCI_SNIFF_SUB_RATE_T                     dm_sniff_sub_rate;
    DM_HCI_SNIFF_SUB_RATE_COMPLETE_T            dm_sniff_sub_rate_complete;
    DM_HCI_SNIFF_SUB_RATING_IND_T               dm_sniff_sub_rating_ind;
    DM_HCI_LINK_SUPERV_TIMEOUT_IND_T            dm_link_superv_timeout_ind;

    DM_WRITE_CACHED_PAGE_MODE_REQ_T		dm_write_cached_page_mode_req;
    DM_WRITE_CACHED_PAGE_MODE_CFM_T		dm_write_cached_page_mode_cfm;
    DM_WRITE_CACHED_CLOCK_OFFSET_REQ_T		dm_write_cached_clock_offset_req;
    DM_WRITE_CACHED_CLOCK_OFFSET_CFM_T		dm_write_cached_clock_offset_cfm;
    DM_CLEAR_PARAM_CACHE_REQ_T			dm_clear_param_cache_req;
    DM_CLEAR_PARAM_CACHE_CFM_T			dm_clear_param_cache_cfm;
    DM_SET_DEFAULT_LINK_POLICY_T            dm_set_default_link_policy;
    DM_ACL_OPEN_REQ_T                       dm_acl_open_req;
    DM_ACL_OPEN_CFM_T                       dm_acl_open_cfm;
    DM_ACL_CLOSE_REQ_T                      dm_acl_close_req;
    DM_ACL_OPENED_IND_T                     dm_acl_opened_ind;
    DM_ACL_CLOSED_IND_T                     dm_acl_closed_ind;

    DM_SM_SET_DEFAULT_SECURITY_REQ_T        dm_sm_set_default_security_req;
    DM_SM_REGISTER_REQ_T                    dm_sm_register_req;
    DM_SM_UNREGISTER_REQ_T                  dm_sm_unregister_req;
    DM_SM_ACCESS_REQ_T                      dm_sm_access_req;
    DM_SM_ACCESS_CFM_T                      dm_sm_access_cfm;
    DM_SM_SET_SEC_MODE_REQ_T                dm_sm_set_sec_mode_req;
    DM_SM_SET_SEC_MODE_CFM_T                dm_sm_set_sec_mode_cfm;
    DM_SM_ADD_DEVICE_REQ_T                  dm_sm_add_device_req;
    DM_SM_ADD_DEVICE_CFM_T                  dm_sm_add_device_cfm;
    DM_SM_REMOVE_DEVICE_REQ_T               dm_sm_remove_device_req;
    DM_SM_REMOVE_DEVICE_CFM_T               dm_sm_remove_device_cfm;
    DM_SM_LINK_KEY_REQUEST_IND_T            dm_sm_link_key_request_ind;
    DM_SM_LINK_KEY_REQUEST_RES_T            dm_sm_link_key_request_res;
    DM_SM_PIN_REQUEST_IND_T                 dm_sm_pin_request_ind;
    DM_SM_PIN_REQUEST_RES_T                 dm_sm_pin_request_res;
    DM_SM_LINK_KEY_IND_T                    dm_sm_link_key_ind;
    DM_SM_AUTHORISE_IND_T                   dm_sm_authorise_ind;
    DM_SM_AUTHORISE_RES_T                   dm_sm_authorise_res;
    DM_SM_AUTHENTICATE_REQ_T                dm_sm_authenticate_req;
    DM_SM_AUTHENTICATE_CFM_T                dm_sm_authenticate_cfm;
    DM_SM_ENCRYPT_REQ_T                     dm_sm_encrypt_req;
    DM_SM_ENCRYPT_CFM_T                     dm_sm_encrypt_cfm;
    DM_SM_ENCRYPTION_CHANGE_T               dm_sm_encryption_change;
    DM_SM_L2CAP_REGISTER_REQ_T              dm_sm_l2cap_register_req;
    DM_SM_L2CAP_CLRX_ENABLE_IND_T           dm_sm_l2cap_clrx_enable_ind;
    DM_SM_IO_CAPABILITY_RESPONSE_IND_T      dm_sm_io_capability_response_ind;
    DM_SM_IO_CAPABILITY_REQUEST_IND_T       dm_sm_io_capability_request_ind;
    DM_SM_IO_CAPABILITY_REQUEST_RES_T       dm_sm_io_capability_request_res;
    DM_SM_IO_CAPABILITY_REQUEST_NEG_RES_T   dm_sm_io_capability_request_neg_res;
    DM_SM_SSP_ADD_DEVICE_REQ_T              dm_sm_ssp_add_device_req;
    DM_SM_SSP_ADD_DEVICE_CFM_T              dm_sm_ssp_add_device_cfm;
    DM_SM_USER_CONFIRMATION_REQUEST_IND_T   dm_sm_user_confirmation_request_ind;
    DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND_T dm_sm_local_user_confirmation_request_ind;
    DM_SM_USER_CONFIRMATION_REQUEST_RES_T   dm_sm_user_confirmation_request_res;
    DM_SM_USER_CONFIRMATION_REQUEST_NEG_RES_T   dm_sm_user_confirmation_request_neg_res;
    DM_SM_USER_PASSKEY_REQUEST_IND_T        dm_sm_user_passkey_request_ind;
    DM_SM_USER_PASSKEY_REQUEST_RES_T        dm_sm_user_passkey_request_res;
    DM_SM_USER_PASSKEY_REQUEST_NEG_RES_T    dm_sm_user_passkey_request_neg_res;
    DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T     dm_sm_simple_pairing_complete_ind;
    DM_SM_READ_LOCAL_OOB_DATA_REQ_T         dm_sm_read_local_oob_data_req;
    DM_SM_READ_LOCAL_OOB_DATA_CFM_T         dm_sm_read_local_oob_data_cfm;
    DM_SM_SEC_MODE_CONFIG_REQ_T               dm_sm_set_sec_mode4_req;
    DM_SM_SEC_MODE_CONFIG_CFM_T               dm_sm_set_sec_mode4_cfm;
    DM_SM_SSP_LINK_KEY_REQUEST_IND_T        dm_sm_ssp_link_key_request_ind;
    DM_SM_SSP_LINK_KEY_REQUEST_RES_T        dm_sm_ssp_link_key_request_res;
    DM_SM_USER_PASSKEY_NOTIFICATION_IND_T   dm_sm_user_passkey_notification_ind;
    DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ_T  dm_sm_send_keypress_notification_req;
    DM_SM_KEYPRESS_NOTIFICATION_IND_T       dm_sm_keypress_notification_req;
    DM_SM_BONDING_REQ_T                     dm_sm_bonding_req;
    DM_SM_BONDING_CFM_T                     dm_sm_bonding_cfm;
    DM_SM_BONDING_CANCEL_REQ_T              dm_sm_bonding_cancel_req;
    DM_SM_READ_DEVICE_REQ_T                 dm_sm_read_device_req;
    DM_SM_READ_DEVICE_CFM_T                 dm_sm_read_device_cfm;
    DM_SM_ACCESS_IND_T                      dm_sm_access_ind;

    DM_SYNC_REGISTER_REQ_T                  dm_sync_register_req;
    DM_SYNC_REGISTER_CFM_T                  dm_sync_register_cfm;
    DM_SYNC_UNREGISTER_REQ_T                dm_sync_unregister_req;
    DM_SYNC_UNREGISTER_CFM_T                dm_sync_unregister_cfm;
    DM_SYNC_CONNECT_REQ_T                   dm_sync_connect_req;
    DM_SYNC_CONNECT_CFM_T                   dm_sync_connect_cfm;
    DM_SYNC_CONNECT_COMPLETE_IND_T          dm_sync_connect_complete_ind;
    DM_SYNC_CONNECT_IND_T                   dm_sync_connect_ind;
    DM_SYNC_CONNECT_RES_T                   dm_sync_connect_res;
    DM_SYNC_RENEGOTIATE_REQ_T               dm_sync_renegotiate_req;
    DM_SYNC_RENEGOTIATE_IND_T               dm_sync_renegotiate_ind;
    DM_SYNC_RENEGOTIATE_CFM_T               dm_sync_renegotiate_cfm;
    DM_SYNC_DISCONNECT_REQ_T                dm_sync_disconnect_req;
    DM_SYNC_DISCONNECT_IND_T                dm_sync_disconnect_ind;
    DM_SYNC_DISCONNECT_CFM_T                dm_sync_disconnect_cfm;
    DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_T dm_read_extended_inquiry_response_data;
    DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_T dm_write_extended_inquiry_response_data;
    DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_COMPLETE_T dm_read_extended_inquiry_response_data_complete;
    DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_COMPLETE_T dm_write_extended_inquiry_response_data_complete;
    DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T    dm_extended_inquiry_result_ind;
    DM_HCI_ENHANCED_FLUSH_COMPLETE_T        dm_enhanced_flush_complete;
    DM_HCI_ENHANCED_FLUSH_T                 dm_enhanced_flush;
    DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_REQ_T       dm_read_inquiry_response_tx_power_level_req;
    DM_HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL_COMPLETE_T  dm_read_inquiry_response_tx_power_level_cfm;
    DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_T      dm_write_inquiry_transmit_power_level_req;
    DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_COMPLETE_T dm_write_inquiry_transmit_power_level_cfm;
    DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_T dm_rem_host_supported_features_ind;
    DM_HCI_REFRESH_ENCRYPTION_KEY_T          dm_refresh_encryption_key;
    DM_HCI_ENCRYPTION_KEY_REFRESH_IND_T      dm_encryption_key_refresh_ind;
    DM_HCI_WRITE_STORED_LINK_KEY_T           dm_write_stored_link_key;
    DM_HCI_CHANGE_LOCAL_NAME_T               dm_change_local_name;
    DM_HCI_WRITE_CURRENT_IAC_LAP_T           dm_write_current_iac_lap;

    DM_EN_ACL_OPENED_IND_T                   dm_en_acl_opened_ind;
    DM_EN_ACL_CLOSED_IND_T                   dm_en_acl_closed_ind;

#ifdef INSTALL_DYNAMIC_TOKEN_HANDLING
    DM_MC_READ_CHANNEL_INFO_REQ_T            dm_mc_read_channel_info_req;
    DM_MC_READ_CHANNEL_INFO_CFM_T            dm_mc_read_channel_info_cfm;
    DM_MC_RESERVE_TOKENS_REQ_T               dm_mc_reserve_tokens_req;
    DM_MC_RESERVE_TOKENS_CFM_T               dm_mc_reserve_tokens_cfm;
    DM_MC_RETURN_TOKENS_REQ_T                dm_mc_return_tokens_req;
    DM_MC_RETURN_TOKENS_CFM_T                dm_mc_return_tokens_cfm;
#endif 


} DM_UPRIM_T;



/*============================================================================*
 *                               PUBLIC FUNCTIONS
 *============================================================================*/
/* None */

#ifdef __cplusplus
}
#endif 

/*=============================== END OF FILE ================================*/
#endif /* ndef _DM_PRIM_H */

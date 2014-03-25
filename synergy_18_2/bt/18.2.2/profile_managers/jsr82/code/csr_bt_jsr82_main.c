/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_jsr82_main.h"
#include "csr_bt_jsr82_prim.h"
#include "csr_types.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_profiles.h"
#include "csr_pmem.h"
#include "csr_bt_jsr82_discovery.h"
#include "csr_bt_jsr82_local_device.h"
#include "csr_bt_jsr82_maintenance_handler.h"
#include "csr_bt_jsr82_service_record.h"
#include "csr_bt_jsr82_remote_device.h"
#include "csr_bt_jsr82_l2cap.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_jsr82_rfcomm.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_private_prim.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_util.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

void CsrBtJsr82CleanupDisconnect(Jsr82MainInstanceData *instData);
static void cleanupUnregisterHandler(Jsr82MainInstanceData *instData);
static void cleanupWriteCodHandler(Jsr82MainInstanceData *instData);

static const jsr82HandlerType jsr82MsgHandler[NUM_OF_JSR82_STATES][CSR_BT_JSR82_PRIM_DOWNSTREAM_COUNT] =
{
    {   /* IDLE_S */
        CsrBtJsr82DaStartInquiry,            /* CSR_BT_JSR82_DA_START_INQUIRY_REQ */
        NULL,                                /* CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */
        CsrBtJsr82AddSearchReq,              /* CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */
        CsrBtJsr82DaCancelServiceSearch,     /* CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */
        CsrBtJsr82DaSelectServiceReqHandler, /* CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */
        CsrBtJsr82DaRetrieveDevicesReqHandler,/* CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */
        CsrBtJsr82LdGetBluetoothAddress,     /* CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */
        CsrBtJsr82StartLdUpdateRecord,       /* CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */
        CsrBtJsr82LdGetFriendlyName,         /* CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82LdGetDiscoverableReqHandler,/* CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */
        CsrBtJsr82LdSetDiscoverableReqHandler,/* CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */
        CsrBtJsr82LdGetProperty,             /* CSR_BT_JSR82_LD_GET_PROPERTY_REQ */
        CsrBtJsr82LdGetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdSetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdGetSecurityLevelReqHandler,/* CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */
        CsrBtJsr82LdIsMasterReqHandler,      /* CSR_BT_JSR82_LD_IS_MASTER_REQ */
        CsrBtJsr82SrCreateRecordHandle,      /* CSR_BT_JSR82_SR_CREATE_RECORD_REQ */
        CsrBtJsr82SrRegisterReq,             /* CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */
        CsrBtJsr82SrUnRegisterReq,           /* CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */
        CsrBtJsr82SrPopulateRecordReqHandler,/* CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */
        CsrBtJsr82RdGetFriendlyNameReqHandler,/* CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82RdAuthenticate,            /* CSR_BT_JSR82_RD_AUTHENTICATE_REQ */
        CsrBtJsr82RdIsAuthenticatedReqHandler,/* CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */
        CsrBtJsr82RdEncrypt,                 /* CSR_BT_JSR82_RD_ENCRYPT_REQ */
        CsrBtJsr82RdIsEncryptedReqHandler,   /* CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */
        CsrBtJsr82RdIsTrustedReqHandler,     /* CSR_BT_JSR82_RD_IS_TRUSTED_REQ */
        CsrBtJsr82RdIsConnected,             /* JSR82_RD_IS_CONNECTED */
        CsrBtJsr82L2caGetPsmReqHandler,      /* CSR_BT_JSR82_L2CA_GET_PSM_REQ */
        CsrBtJsr82L2caAcceptReqHandler,      /* CSR_BT_JSR82_L2CA_ACCEPT_REQ */
        CsrBtJsr82L2caOpenReqHandler,        /* CSR_BT_JSR82_L2CA_OPEN_REQ */
        CsrBtJsr82L2caDisconnectReqHandler,  /* CSR_BT_JSR82_L2CA_DISCONNECT_REQ */
        CsrBtJsr82L2caTxDataReqHandler,      /* CSR_BT_JSR82_L2CA_TX_DATA_REQ */
        CsrBtJsr82L2caRxDataReqHandler,      /* CSR_BT_JSR82_L2CA_RX_DATA_REQ */
        CsrBtJsr82L2caRxReadyReqHandler,     /* CSR_BT_JSR82_L2CA_RX_READY_REQ */
        CsrBtJsr82L2caGetConfigReqHandler,   /* CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */
        CsrBtJsr82L2caCloseReqHandler,       /* CSR_BT_JSR82_L2CA_CLOSE_REQ */
        CsrBtJsr82RfcGetServerChannelReqHandler,/* CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */
        CsrBtJsr82RfcAcceptAndOpenReqHandler,/* CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */
        CsrBtJsr82RfcConnectReqHandler,      /* CSR_BT_JSR82_RFC_CONNECT_REQ */
        CsrBtJsr82RfcDisconnectReqHandler,   /* CSR_BT_JSR82_RFC_DISCONNECT_REQ */
        CsrBtJsr82RfcCloseReqHandler,        /* CSR_BT_JSR82_RFC_CLOSE_REQ */
        CsrBtJsr82StartRfcSendData,          /* CSR_BT_JSR82_RFC_SEND_DATA_REQ */
        CsrBtJsr82RfcReceiveData,            /* CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */
        CsrBtJsr82RfcGetAvailable,           /* CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */
        CsrBtJsr82CleanupReqHandler,         /* CSR_BT_JSR82_CLEANUP_REQ */
        CsrBtJsr82SetEventMaskReqHandler,    /* CSR_BT_JSR82_SET_EVENT_MASK_REQ */
    },
    {   /* BUSY_S */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_START_INQUIRY_REQ */
        NULL,                                /* CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */
        CsrBtJsr82AddSearchReq,              /* CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */
        CsrBtJsr82DaCancelServiceSearch,     /* CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */
        CsrBtJsr82LdGetBluetoothAddress,     /* CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */
        CsrBtJsr82LdGetFriendlyName,         /* CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82LdGetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */
        CsrBtJsr82LdSetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */
        CsrBtJsr82LdGetProperty,             /* CSR_BT_JSR82_LD_GET_PROPERTY_REQ */
        CsrBtJsr82LdGetDeviceClassReqHandler,  /* CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdSetDeviceClassReqHandler,  /* CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdGetSecurityLevelReqHandler,/* CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */
        CsrBtJsr82LdIsMasterReqHandler,      /* CSR_BT_JSR82_LD_IS_MASTER_REQ */
        CsrBtJsr82SrCreateRecordHandle,      /* CSR_BT_JSR82_SR_CREATE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_AUTHENTICATE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_ENCRYPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_TRUSTED_REQ */
        CsrBtJsr82RdIsConnected,             /* JSR82_RD_IS_CONNECTED */
        CsrBtJsr82L2caGetPsmReqHandler,      /* CSR_BT_JSR82_L2CA_GET_PSM_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_ACCEPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_TX_DATA_REQ */
        CsrBtJsr82L2caRxDataReqHandler,      /* CSR_BT_JSR82_L2CA_RX_DATA_REQ */
        CsrBtJsr82L2caRxReadyReqHandler,     /* CSR_BT_JSR82_L2CA_RX_READY_REQ */
        CsrBtJsr82L2caGetConfigReqHandler,   /* CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_CLOSE_REQ */
        CsrBtJsr82RfcGetServerChannelReqHandler,/* CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CLOSE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_SEND_DATA_REQ */
        CsrBtJsr82RfcReceiveData,            /* CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */
        CsrBtJsr82RfcGetAvailable,           /* CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */
        CsrBtJsr82CleanupReqHandler,         /* CSR_BT_JSR82_CLEANUP_REQ */
        CsrBtJsr82SetEventMaskReqHandler,    /* CSR_BT_JSR82_SET_EVENT_MASK_REQ */
    },
    {   /* DISCOVERY_S */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_START_INQUIRY_REQ */
        CsrBtJsr82DaCancelInquiry,           /* CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */
        CsrBtJsr82AddSearchReq,              /* CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */
        CsrBtJsr82DaCancelServiceSearch,     /* CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */
        CsrBtJsr82LdGetBluetoothAddress,     /* CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */
        CsrBtJsr82LdGetFriendlyName,         /* CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82LdGetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */
        CsrBtJsr82LdSetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */
        CsrBtJsr82LdGetProperty,             /* CSR_BT_JSR82_LD_GET_PROPERTY_REQ */
        CsrBtJsr82LdGetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdSetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdGetSecurityLevelReqHandler,/* CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */
        CsrBtJsr82LdIsMasterReqHandler,      /* CSR_BT_JSR82_LD_IS_MASTER_REQ */
        CsrBtJsr82SrCreateRecordHandle,      /* CSR_BT_JSR82_SR_CREATE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */
        CsrBtJsr82DaInquiryGetFriendlyName,  /* CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_AUTHENTICATE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_ENCRYPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_TRUSTED_REQ */
        CsrBtJsr82RdIsConnected,             /* JSR82_RD_IS_CONNECTED */
        CsrBtJsr82L2caGetPsmReqHandler,      /* CSR_BT_JSR82_L2CA_GET_PSM_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_ACCEPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_TX_DATA_REQ */
        CsrBtJsr82L2caRxDataReqHandler,      /* CSR_BT_JSR82_L2CA_RX_DATA_REQ */
        CsrBtJsr82L2caRxReadyReqHandler,     /* CSR_BT_JSR82_L2CA_RX_READY_REQ */
        CsrBtJsr82L2caGetConfigReqHandler,   /* CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_CLOSE_REQ */
        CsrBtJsr82RfcGetServerChannelReqHandler,/* CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CLOSE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_SEND_DATA_REQ */
        CsrBtJsr82RfcReceiveData,            /* CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */
        CsrBtJsr82RfcGetAvailable,           /* CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */
        CsrBtJsr82CleanupReqHandler,         /* CSR_BT_JSR82_CLEANUP_REQ */
        CsrBtJsr82SetEventMaskReqHandler,    /* CSR_BT_JSR82_SET_EVENT_MASK_REQ */
    },
    {   /* SERV_SEARCH_S */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_START_INQUIRY_REQ */
        NULL,                                /* CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */
        CsrBtJsr82AddSearchReq,              /* CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */
        CsrBtJsr82DaCancelServiceSearch,     /* CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */
        CsrBtJsr82LdGetBluetoothAddress,     /* CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */
        CsrBtJsr82LdGetFriendlyName,         /* CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82LdGetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */
        CsrBtJsr82LdSetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */
        CsrBtJsr82LdGetProperty,             /* CSR_BT_JSR82_LD_GET_PROPERTY_REQ */
        CsrBtJsr82LdGetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdSetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdGetSecurityLevelReqHandler,/* CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */
        CsrBtJsr82LdIsMasterReqHandler,      /* CSR_BT_JSR82_LD_IS_MASTER_REQ */
        CsrBtJsr82SrCreateRecordHandle,      /* CSR_BT_JSR82_SR_CREATE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_AUTHENTICATE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_ENCRYPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_TRUSTED_REQ */
        CsrBtJsr82RdIsConnected,             /* JSR82_RD_IS_CONNECTED */
        CsrBtJsr82L2caGetPsmReqHandler,      /* CSR_BT_JSR82_L2CA_GET_PSM_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_ACCEPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_TX_DATA_REQ */
        CsrBtJsr82L2caRxDataReqHandler,      /* CSR_BT_JSR82_L2CA_RX_DATA_REQ */
        CsrBtJsr82L2caRxReadyReqHandler,     /* CSR_BT_JSR82_L2CA_RX_READY_REQ */
        CsrBtJsr82L2caGetConfigReqHandler,   /* CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_CLOSE_REQ */
        CsrBtJsr82RfcGetServerChannelReqHandler,/* CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CLOSE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_SEND_DATA_REQ */
        CsrBtJsr82RfcReceiveData,            /* CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */
        CsrBtJsr82RfcGetAvailable,           /* CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */
        CsrBtJsr82CleanupReqHandler,         /* CSR_BT_JSR82_CLEANUP_REQ */
        CsrBtJsr82SetEventMaskReqHandler,    /* CSR_BT_JSR82_SET_EVENT_MASK_REQ */
    },
    {   /* UPDATING_RECORD_S */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_START_INQUIRY_REQ */
        NULL,                                /* CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */
        CsrBtJsr82AddSearchReq,              /* CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */
        CsrBtJsr82DaCancelServiceSearch,     /* CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */
        CsrBtJsr82LdGetBluetoothAddress,     /* CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */
        CsrBtJsr82PendingMessage,            /* CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */
        CsrBtJsr82LdGetFriendlyName,         /* CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82LdGetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */
        CsrBtJsr82LdSetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */
        CsrBtJsr82LdGetProperty,             /* CSR_BT_JSR82_LD_GET_PROPERTY_REQ */
        CsrBtJsr82LdGetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdSetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdGetSecurityLevelReqHandler,/* CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */
        CsrBtJsr82LdIsMasterReqHandler,      /* CSR_BT_JSR82_LD_IS_MASTER_REQ */
        CsrBtJsr82SrCreateRecordHandle,      /* CSR_BT_JSR82_SR_CREATE_RECORD_REQ */
        CsrBtJsr82SrUpdateRegisterReq,       /* CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */
        CsrBtJsr82SrUpdateUnRegisterReq,     /* CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_AUTHENTICATE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_ENCRYPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_TRUSTED_REQ */
        CsrBtJsr82RdIsConnected,             /* JSR82_RD_IS_CONNECTED */
        CsrBtJsr82L2caGetPsmReqHandler,      /* CSR_BT_JSR82_L2CA_GET_PSM_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_ACCEPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_TX_DATA_REQ */
        CsrBtJsr82L2caRxDataReqHandler,      /* CSR_BT_JSR82_L2CA_RX_DATA_REQ */
        CsrBtJsr82L2caRxReadyReqHandler,     /* CSR_BT_JSR82_L2CA_RX_READY_REQ */
        CsrBtJsr82L2caGetConfigReqHandler,   /* CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_CLOSE_REQ */
        CsrBtJsr82RfcGetServerChannelReqHandler,/* CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CLOSE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_SEND_DATA_REQ */
        CsrBtJsr82RfcReceiveData,            /* CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */
        CsrBtJsr82RfcGetAvailable,           /* CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */
        CsrBtJsr82CleanupReqHandler,         /* CSR_BT_JSR82_CLEANUP_REQ */
        CsrBtJsr82SetEventMaskReqHandler,    /* CSR_BT_JSR82_SET_EVENT_MASK_REQ */
    },
    {   /* WRITE_COD_S */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_START_INQUIRY_REQ */
        NULL,                                /* CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */
        CsrBtJsr82AddSearchReq,              /* CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */
        CsrBtJsr82DaCancelServiceSearch,     /* CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */
        CsrBtJsr82LdGetBluetoothAddress,     /* CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */
        CsrBtJsr82LdGetFriendlyName,         /* CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82LdGetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */
        CsrBtJsr82LdSetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */
        CsrBtJsr82LdGetProperty,             /* CSR_BT_JSR82_LD_GET_PROPERTY_REQ */
        CsrBtJsr82LdGetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdSetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdGetSecurityLevelReqHandler,/* CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */
        CsrBtJsr82LdIsMasterReqHandler,      /* CSR_BT_JSR82_LD_IS_MASTER_REQ */
        CsrBtJsr82SrCreateRecordHandle,      /* CSR_BT_JSR82_SR_CREATE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_AUTHENTICATE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_ENCRYPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_TRUSTED_REQ */
        CsrBtJsr82RdIsConnected,             /* JSR82_RD_IS_CONNECTED */
        CsrBtJsr82L2caGetPsmReqHandler,      /* CSR_BT_JSR82_L2CA_GET_PSM_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_ACCEPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_TX_DATA_REQ */
        CsrBtJsr82L2caRxDataReqHandler,      /* CSR_BT_JSR82_L2CA_RX_DATA_REQ */
        CsrBtJsr82L2caRxReadyReqHandler,     /* CSR_BT_JSR82_L2CA_RX_READY_REQ */
        CsrBtJsr82L2caGetConfigReqHandler,   /* CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_CLOSE_REQ */
        CsrBtJsr82RfcGetServerChannelReqHandler,/* CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CLOSE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_SEND_DATA_REQ */
        CsrBtJsr82RfcReceiveData,            /* CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */
        CsrBtJsr82RfcGetAvailable,           /* CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */
        CsrBtJsr82CleanupReqHandler,         /* CSR_BT_JSR82_CLEANUP_REQ */
        CsrBtJsr82SetEventMaskReqHandler,    /* CSR_BT_JSR82_SET_EVENT_MASK_REQ */
    },
    {   /* SELECT_SERVICE_S */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_START_INQUIRY_REQ */
        NULL,                                /* CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */
        CsrBtJsr82AddSearchReq,              /* CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */
        CsrBtJsr82DaCancelServiceSearch,     /* CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */
        CsrBtJsr82LdGetBluetoothAddress,     /* CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */
        CsrBtJsr82LdGetFriendlyName,         /* CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82LdGetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */
        CsrBtJsr82LdSetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */
        CsrBtJsr82LdGetProperty,             /* CSR_BT_JSR82_LD_GET_PROPERTY_REQ */
        CsrBtJsr82LdGetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdSetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdGetSecurityLevelReqHandler,/* CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */
        CsrBtJsr82LdIsMasterReqHandler,      /* CSR_BT_JSR82_LD_IS_MASTER_REQ */
        CsrBtJsr82SrCreateRecordHandle,      /* CSR_BT_JSR82_SR_CREATE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_AUTHENTICATE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_ENCRYPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_TRUSTED_REQ */
        CsrBtJsr82RdIsConnected,             /* JSR82_RD_IS_CONNECTED */
        CsrBtJsr82L2caGetPsmReqHandler,      /* CSR_BT_JSR82_L2CA_GET_PSM_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_ACCEPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_TX_DATA_REQ */
        CsrBtJsr82L2caRxDataReqHandler,      /* CSR_BT_JSR82_L2CA_RX_DATA_REQ */
        CsrBtJsr82L2caRxReadyReqHandler,     /* CSR_BT_JSR82_L2CA_RX_READY_REQ */
        CsrBtJsr82L2caGetConfigReqHandler,   /* CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_CLOSE_REQ */
        CsrBtJsr82RfcGetServerChannelReqHandler,/* CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CLOSE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_SEND_DATA_REQ */
        CsrBtJsr82RfcReceiveData,            /* CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */
        CsrBtJsr82RfcGetAvailable,           /* CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */
        CsrBtJsr82CleanupReqHandler,         /* CSR_BT_JSR82_CLEANUP_REQ */
        CsrBtJsr82SetEventMaskReqHandler,    /* CSR_BT_JSR82_SET_EVENT_MASK_REQ */
    },
        {   /* RD_NAME_S */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_START_INQUIRY_REQ */
        NULL,                                /* CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */
        CsrBtJsr82AddSearchReq,              /* CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */
        CsrBtJsr82DaCancelServiceSearch,     /* CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */
        CsrBtJsr82LdGetBluetoothAddress,     /* CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */
        CsrBtJsr82LdGetFriendlyName,         /* CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82LdGetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */
        CsrBtJsr82LdSetDiscoverableReqHandler, /* CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */
        CsrBtJsr82LdGetProperty,             /* CSR_BT_JSR82_LD_GET_PROPERTY_REQ */
        CsrBtJsr82LdGetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdSetDeviceClassReqHandler,/* CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */
        CsrBtJsr82LdGetSecurityLevelReqHandler,/* CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */
        CsrBtJsr82LdIsMasterReqHandler,      /* CSR_BT_JSR82_LD_IS_MASTER_REQ */
        CsrBtJsr82SrCreateRecordHandle,      /* CSR_BT_JSR82_SR_CREATE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */
        CsrBtJsr82RdGetFriendlyNameReqHandler,  /* CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_AUTHENTICATE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_ENCRYPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RD_IS_TRUSTED_REQ */
        CsrBtJsr82RdIsConnected,             /* JSR82_RD_IS_CONNECTED */
        CsrBtJsr82L2caGetPsmReqHandler,      /* CSR_BT_JSR82_L2CA_GET_PSM_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_ACCEPT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_TX_DATA_REQ */
        CsrBtJsr82L2caRxDataReqHandler,      /* CSR_BT_JSR82_L2CA_RX_DATA_REQ */
        CsrBtJsr82L2caRxReadyReqHandler,     /* CSR_BT_JSR82_L2CA_RX_READY_REQ */
        CsrBtJsr82L2caGetConfigReqHandler,   /* CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_L2CA_CLOSE_REQ */
        CsrBtJsr82RfcGetServerChannelReqHandler,/* CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_DISCONNECT_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_CLOSE_REQ */
        CsrBtJsr82SaveMessage,               /* CSR_BT_JSR82_RFC_SEND_DATA_REQ */
        CsrBtJsr82RfcReceiveData,            /* CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */
        CsrBtJsr82RfcGetAvailable,           /* CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */
        CsrBtJsr82CleanupReqHandler,         /* CSR_BT_JSR82_CLEANUP_REQ */
        CsrBtJsr82SetEventMaskReqHandler,    /* CSR_BT_JSR82_SET_EVENT_MASK_REQ */
    },
    {   /* CLEANUP_S */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_DA_START_INQUIRY_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_LD_GET_PROPERTY_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_LD_IS_MASTER_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_SR_CREATE_RECORD_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RD_AUTHENTICATE_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RD_ENCRYPT_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RD_IS_TRUSTED_REQ */
        CsrBtJsr82LeaveCleanup,              /* JSR82_RD_IS_CONNECTED */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_L2CA_GET_PSM_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_L2CA_ACCEPT_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_L2CA_OPEN_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_L2CA_DISCONNECT_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_L2CA_TX_DATA_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_L2CA_RX_DATA_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_L2CA_RX_READY_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_L2CA_CLOSE_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RFC_CONNECT_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RFC_DISCONNECT_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RFC_CLOSE_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RFC_SEND_DATA_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */
        CsrBtJsr82LeaveCleanup,              /* CSR_BT_JSR82_CLEANUP_REQ */
        CsrBtJsr82SetEventMaskReqHandler,    /* CSR_BT_JSR82_SET_EVENT_MASK_REQ */
    }
};

static const jsr82HandlerType cmRfcConnMsgHandler[NUM_OF_JSR82_STATES][CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT] =
{
    {   /* IDLE_S */
        CsrBtJsr82CmCancelAcceptConnectCfmHandler,   /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        CsrBtJsr82CmConnectCfmHandler,               /* CSR_BT_CM_CONNECT_CFM */
        CsrBtJsr82CmConnectAcceptCfmHandler,         /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_REGISTER_CFM */
        CsrBtJsr82CmDisconnectIndHandler,            /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                        /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtJsr82RfcHandleDataInd,                  /* CSR_BT_CM_DATA_IND */
        CsrBtJsr82RfcHandleDataCfm,                  /* CSR_BT_CM_DATA_CFM */
        CsrBtJsr82CmControlIndHandler,               /* CSR_BT_CM_CONTROL_IND */
        CsrBtJsr82CmModeChangeIndHandler,            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        CsrBtJsr82CmPortNegIndHandler,               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                        /* CSR_BT_CM_PORTNEG_CFM */
    },
    {   /* BUSY_S */
        CsrBtJsr82CmCancelAcceptConnectCfmHandler,   /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        CsrBtJsr82CmConnectCfmHandler,               /* CSR_BT_CM_CONNECT_CFM */
        CsrBtJsr82CmConnectAcceptCfmHandler,         /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_REGISTER_CFM */
        CsrBtJsr82CmDisconnectIndHandler,            /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                        /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtJsr82RfcHandleDataInd,                  /* CSR_BT_CM_DATA_IND */
        CsrBtJsr82RfcHandleDataCfm,                  /* CSR_BT_CM_DATA_CFM */
        CsrBtJsr82CmControlIndHandler,               /* CSR_BT_CM_CONTROL_IND */
        CsrBtJsr82CmModeChangeIndHandler,            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        CsrBtJsr82CmPortNegIndHandler,               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                        /* CSR_BT_CM_PORTNEG_CFM */
    },
    {   /* DISCOVERY_S */
        CsrBtJsr82CmCancelAcceptConnectCfmHandler,   /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        CsrBtJsr82CmConnectCfmHandler,               /* CSR_BT_CM_CONNECT_CFM */
        CsrBtJsr82CmConnectAcceptCfmHandler,         /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_REGISTER_CFM */
        CsrBtJsr82CmDisconnectIndHandler,            /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                        /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtJsr82RfcHandleDataInd,                  /* CSR_BT_CM_DATA_IND */
        CsrBtJsr82RfcHandleDataCfm,                  /* CSR_BT_CM_DATA_CFM */
        CsrBtJsr82CmControlIndHandler,               /* CSR_BT_CM_CONTROL_IND */
        CsrBtJsr82CmModeChangeIndHandler,            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        CsrBtJsr82CmPortNegIndHandler,               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                        /* CSR_BT_CM_PORTNEG_CFM */
    },
    {   /* SERV_SEARCH_S */
        CsrBtJsr82CmCancelAcceptConnectCfmHandler,   /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        CsrBtJsr82CmConnectCfmHandler,               /* CSR_BT_CM_CONNECT_CFM */
        CsrBtJsr82CmConnectAcceptCfmHandler,         /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_REGISTER_CFM */
        CsrBtJsr82CmDisconnectIndHandler,            /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                        /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtJsr82RfcHandleDataInd,                  /* CSR_BT_CM_DATA_IND */
        CsrBtJsr82RfcHandleDataCfm,                  /* CSR_BT_CM_DATA_CFM */
        CsrBtJsr82CmControlIndHandler,               /* CSR_BT_CM_CONTROL_IND */
        CsrBtJsr82CmModeChangeIndHandler,            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        CsrBtJsr82CmPortNegIndHandler,               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                        /* CSR_BT_CM_PORTNEG_CFM */
    },
    { /* UPDATING_RECORD_S */
        CsrBtJsr82CmCancelAcceptConnectCfmHandler,   /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        CsrBtJsr82CmConnectCfmHandler,               /* CSR_BT_CM_CONNECT_CFM */
        CsrBtJsr82CmConnectAcceptCfmHandler,         /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_REGISTER_CFM */
        CsrBtJsr82CmDisconnectIndHandler,            /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                        /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtJsr82RfcHandleDataInd,                  /* CSR_BT_CM_DATA_IND */
        CsrBtJsr82RfcHandleDataCfm,                  /* CSR_BT_CM_DATA_CFM */
        CsrBtJsr82CmControlIndHandler,               /* CSR_BT_CM_CONTROL_IND */
        CsrBtJsr82CmModeChangeIndHandler,            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        CsrBtJsr82CmPortNegIndHandler,               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                        /* CSR_BT_CM_PORTNEG_CFM */
    },
    {   /* WRITE_COD_S */
        CsrBtJsr82CmCancelAcceptConnectCfmHandler,   /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        CsrBtJsr82CmConnectCfmHandler,               /* CSR_BT_CM_CONNECT_CFM */
        CsrBtJsr82CmConnectAcceptCfmHandler,         /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_REGISTER_CFM */
        CsrBtJsr82CmDisconnectIndHandler,            /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                        /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtJsr82RfcHandleDataInd,                  /* CSR_BT_CM_DATA_IND */
        CsrBtJsr82RfcHandleDataCfm,                  /* CSR_BT_CM_DATA_CFM */
        CsrBtJsr82CmControlIndHandler,               /* CSR_BT_CM_CONTROL_IND */
        CsrBtJsr82CmModeChangeIndHandler,            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        CsrBtJsr82CmPortNegIndHandler,               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                        /* CSR_BT_CM_PORTNEG_CFM */
    },
    {   /* SELECT_SERVICE_S */
        CsrBtJsr82CmCancelAcceptConnectCfmHandler,   /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        CsrBtJsr82CmConnectCfmHandler,               /* CSR_BT_CM_CONNECT_CFM */
        CsrBtJsr82CmConnectAcceptCfmHandler,         /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_REGISTER_CFM */
        CsrBtJsr82CmDisconnectIndHandler,            /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                        /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtJsr82RfcHandleDataInd,                  /* CSR_BT_CM_DATA_IND */
        CsrBtJsr82RfcHandleDataCfm,                  /* CSR_BT_CM_DATA_CFM */
        CsrBtJsr82CmControlIndHandler,               /* CSR_BT_CM_CONTROL_IND */
        CsrBtJsr82CmModeChangeIndHandler,            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        CsrBtJsr82CmPortNegIndHandler,               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                        /* CSR_BT_CM_PORTNEG_CFM */
    },
    {   /* RD_NAME_S */
        CsrBtJsr82CmCancelAcceptConnectCfmHandler,   /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        CsrBtJsr82CmConnectCfmHandler,               /* CSR_BT_CM_CONNECT_CFM */
        CsrBtJsr82CmConnectAcceptCfmHandler,         /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_REGISTER_CFM */
        CsrBtJsr82CmDisconnectIndHandler,            /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                        /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                        /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtJsr82RfcHandleDataInd,                  /* CSR_BT_CM_DATA_IND */
        CsrBtJsr82RfcHandleDataCfm,                  /* CSR_BT_CM_DATA_CFM */
        CsrBtJsr82CmControlIndHandler,               /* CSR_BT_CM_CONTROL_IND */
        CsrBtJsr82CmModeChangeIndHandler,            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        CsrBtJsr82CmPortNegIndHandler,               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                        /* CSR_BT_CM_PORTNEG_CFM */
    },
    {   /* CLEANUP_S */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        CsrBtJsr82CleanupDisconnect,                /* CSR_BT_CM_CONNECT_CFM */
        CsrBtJsr82CmConnectAcceptCfmHandler,        /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_REGISTER_CFM */
        CsrBtJsr82CleanupDisconnect,                /* CSR_BT_CM_DISCONNECT_IND */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_SCO_CONNECT_CFM */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_SCO_DISCONNECT_IND */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_DATA_IND */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_DATA_CFM */
        CsrBtJsr82CmControlIndHandler,              /* CSR_BT_CM_CONTROL_IND */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        CsrBtJsr82CmPortNegIndHandler,              /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                       /* CSR_BT_CM_PORTNEG_CFM */
    }
};

static const jsr82HandlerType cmSdcConnMsgHandler[NUM_OF_JSR82_STATES][CSR_BT_CM_SDC_PRIM_UPSTREAM_COUNT] =
{
    {   /* IDLE_S */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                   /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        CsrBtJsr82SendSrRegisterRecordCfm,           /* CSR_BT_CM_SDS_REGISTER_CFM */
        CsrBtJsr82SendSrRemoveRecordCfm,             /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    {   /* BUSY_S */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        CsrBtJsr82CmSdcOpenCfmHandler,               /* CSR_BT_CM_SDC_OPEN_CFM */
        CsrBtJsr82GetAttrSet,                        /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        CsrBtJsr82PopulateCloseHandler,              /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                   /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        CsrBtJsr82SendSrRegisterRecordCfm,           /* CSR_BT_CM_SDS_REGISTER_CFM */
        CsrBtJsr82SendSrRemoveRecordCfm,             /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    {   /* DISCOVERY_S */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                   /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        CsrBtJsr82SendSrRegisterRecordCfm,           /* CSR_BT_CM_SDS_REGISTER_CFM */
        CsrBtJsr82SendSrRemoveRecordCfm,             /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    {   /* SERV_SEARCH_S */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_CFM */
        CsrBtJsr82DlServicesDiscovered,              /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_OPEN_CFM */
        CsrBtJsr82GetAttrSet,                        /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        CsrBtJsr82ClosedServiceSearch,               /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                   /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        CsrBtJsr82SendSrRegisterRecordCfm,           /* CSR_BT_CM_SDS_REGISTER_CFM */
        CsrBtJsr82SendSrRemoveRecordCfm,             /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    { /* UPDATING_RECORD_S */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                   /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        CsrBtJsr82SendSrRegisterRecordCfm,           /* CSR_BT_CM_SDS_REGISTER_CFM */
        CsrBtJsr82SendSrRemoveRecordCfm,             /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    {   /* WRITE_COD_S */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                   /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        CsrBtJsr82SendSrRegisterRecordCfm,           /* CSR_BT_CM_SDS_REGISTER_CFM */
        CsrBtJsr82SendSrRemoveRecordCfm,             /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    {   /* SELECT_SERVICE_S */
        CsrBtJsr82CmSelServiceSdcSearchIndHandler,   /* CSR_BT_CM_SDC_SEARCH_IND */
        CsrBtJsr82CmSelServiceSdcSearchCfmHandler,   /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_OPEN_CFM */
        CsrBtJsr82CmSelServiceSdcAttributeCfmHandler,/* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        CsrBtJsr82CmSelServiceSdcCloseIndHandler,    /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                   /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        CsrBtJsr82SendSrRegisterRecordCfm,           /* CSR_BT_CM_SDS_REGISTER_CFM */
        CsrBtJsr82SendSrRemoveRecordCfm,             /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    {   /* RD_NAME_S */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                   /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                   /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                   /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        CsrBtJsr82SendSrRegisterRecordCfm,           /* CSR_BT_CM_SDS_REGISTER_CFM */
        CsrBtJsr82SendSrRemoveRecordCfm,             /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    {   /* CLEANUP_S */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_SDC_SEARCH_IND */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_SDC_SEARCH_CFM */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_SDC_OPEN_CFM */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_SDC_CLOSE_IND */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_SDS_REGISTER_CFM */
        cleanupUnregisterHandler,              /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    }
};

static const jsr82HandlerType cmL2caConnMsgHandler[NUM_OF_JSR82_STATES][CSR_BT_CM_L2CA_PRIM_UPSTREAM_COUNT] =
{
    {   /* IDLE_S */

        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtJsr82CmL2caConnectCfmHandler,           /* CSR_BT_CM_L2CA_CONNECT_CFM */
        CsrBtJsr82CmL2caConnectAcceptCfmHandler,     /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caCancelAcceptConnectCfmHandler,/* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caDataCfmHandler,              /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtJsr82CmL2caDataIndHandler,              /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtJsr82CmL2caDisconnectIndHandler,        /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtJsr82CmL2caModeChangeIndHandler,   /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    {   /* BUSY_S */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtJsr82CmL2caConnectCfmHandler,           /* CSR_BT_CM_L2CA_CONNECT_CFM */
        CsrBtJsr82CmL2caConnectAcceptCfmHandler,     /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caCancelAcceptConnectCfmHandler,/* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caDataCfmHandler,              /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtJsr82CmL2caDataIndHandler,              /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtJsr82CmL2caDisconnectIndHandler,        /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtJsr82CmL2caModeChangeIndHandler,   /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    {   /* DISCOVERY_S */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtJsr82CmL2caConnectCfmHandler,           /* CSR_BT_CM_L2CA_CONNECT_CFM */
        CsrBtJsr82CmL2caConnectAcceptCfmHandler,     /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caCancelAcceptConnectCfmHandler,/* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caDataCfmHandler,              /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtJsr82CmL2caDataIndHandler,              /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtJsr82CmL2caDisconnectIndHandler,        /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtJsr82CmL2caModeChangeIndHandler,   /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    {   /* SERV_SEARCH_S */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtJsr82CmL2caConnectCfmHandler,           /* CSR_BT_CM_L2CA_CONNECT_CFM */
        CsrBtJsr82CmL2caConnectAcceptCfmHandler,     /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caCancelAcceptConnectCfmHandler,/* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caDataCfmHandler,              /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtJsr82CmL2caDataIndHandler,              /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtJsr82CmL2caDisconnectIndHandler,        /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtJsr82CmL2caModeChangeIndHandler,   /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    { /* UPDATING_RECORD_S */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtJsr82CmL2caConnectCfmHandler,           /* CSR_BT_CM_L2CA_CONNECT_CFM */
        CsrBtJsr82CmL2caConnectAcceptCfmHandler,     /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caCancelAcceptConnectCfmHandler,/* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caDataCfmHandler,              /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtJsr82CmL2caDataIndHandler,              /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtJsr82CmL2caDisconnectIndHandler,        /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtJsr82CmL2caModeChangeIndHandler,   /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    {   /* WRITE_COD_S */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtJsr82CmL2caConnectCfmHandler,           /* CSR_BT_CM_L2CA_CONNECT_CFM */
        CsrBtJsr82CmL2caConnectAcceptCfmHandler,     /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caCancelAcceptConnectCfmHandler,/* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caDataCfmHandler,              /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtJsr82CmL2caDataIndHandler,              /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtJsr82CmL2caDisconnectIndHandler,        /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtJsr82CmL2caModeChangeIndHandler,   /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    {   /* SELECT_SERVICE_S */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtJsr82CmL2caConnectCfmHandler,           /* CSR_BT_CM_L2CA_CONNECT_CFM */
        CsrBtJsr82CmL2caConnectAcceptCfmHandler,     /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caCancelAcceptConnectCfmHandler,/* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caDataCfmHandler,              /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtJsr82CmL2caDataIndHandler,              /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtJsr82CmL2caDisconnectIndHandler,        /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtJsr82CmL2caModeChangeIndHandler,   /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    {   /* RD_NAME_S */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtJsr82CmL2caConnectCfmHandler,           /* CSR_BT_CM_L2CA_CONNECT_CFM */
        CsrBtJsr82CmL2caConnectAcceptCfmHandler,     /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caCancelAcceptConnectCfmHandler,/* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtJsr82CmL2caDataCfmHandler,              /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtJsr82CmL2caDataIndHandler,              /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtJsr82CmL2caDisconnectIndHandler,        /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                   /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtJsr82CmL2caModeChangeIndHandler,   /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    {   /* CLEANUP_S */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtJsr82CleanupDisconnect,                /* CSR_BT_CM_L2CA_CONNECT_CFM */
        CsrBtJsr82CmL2caConnectAcceptCfmHandler,    /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtJsr82CleanupDisconnect,                /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtJsr82FreeMessage,                      /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    }
};

static const jsr82HandlerType cmInquiryDiscMsgHandler[NUM_OF_JSR82_STATES][CSR_BT_CM_INQUIRY_PRIM_UPSTREAM_COUNT] =
{
    {   /* IDLE_S */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_INQUIRY_RESULT_IND */
        NULL,                                   /* CSR_BT_CM_INQUIRY_CFM */
        CsrBtJsr82RemoteNameCfmHandler,              /* CSR_BT_CM_READ_REMOTE_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_PAGE_TO_CFM */
    },
    {   /* BUSY_S */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_INQUIRY_RESULT_IND */
        NULL,                                   /* CSR_BT_CM_INQUIRY_CFM */
        CsrBtJsr82RemoteNameCfmHandler,              /* CSR_BT_CM_READ_REMOTE_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_PAGE_TO_CFM */
    },
    {   /* DISCOVERY_S */
        CsrBtJsr82DlDeviceDiscovered,                /* CSR_BT_CM_INQUIRY_RESULT_IND */
        CsrBtJsr82DaSdCloseIndHandler,               /* CSR_BT_CM_INQUIRY_CFM */
        CsrBtJsr82RemoteNameCfmHandler,              /* CSR_BT_CM_READ_REMOTE_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_PAGE_TO_CFM */
    },
    {   /* SERV_SEARCH_S */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_INQUIRY_RESULT_IND */
        NULL,                                   /* CSR_BT_CM_INQUIRY_CFM */
        NULL,                                   /* CSR_BT_CM_READ_REMOTE_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_PAGE_TO_CFM */
    },
    {   /* UPDATING_RECORD_S */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_INQUIRY_RESULT_IND */
        NULL,                                   /* CSR_BT_CM_INQUIRY_CFM */
        NULL,                                   /* CSR_BT_CM_READ_REMOTE_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_PAGE_TO_CFM */
    },
    {   /* WRITE_COD_S */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_INQUIRY_RESULT_IND */
        NULL,                                   /* CSR_BT_CM_INQUIRY_CFM */
        NULL,                                   /* CSR_BT_CM_READ_REMOTE_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_PAGE_TO_CFM */
    },
    {   /* SELECT_SERVICE_S */
        CsrBtJsr82CmSelServiceDiscResIndHandler,     /* CSR_BT_CM_INQUIRY_RESULT_IND */
        CsrBtJsr82CmSelServiceDiscCfmHandler,        /* CSR_BT_CM_INQUIRY_CFM */
        NULL,                                   /* CSR_BT_CM_READ_REMOTE_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_PAGE_TO_CFM */
    },
    {   /* RD_NAME_S */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_INQUIRY_RESULT_IND */
        NULL,                                   /* CSR_BT_CM_INQUIRY_CFM */
        CsrBtJsr82RemoteNameCfmHandler,              /* CSR_BT_CM_READ_REMOTE_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_PAGE_TO_CFM */
    },
    {   /* CLEANUP_S */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_INQUIRY_RESULT_IND */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_INQUIRY_CFM */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_READ_REMOTE_NAME_CFM */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_WRITE_PAGE_TO_CFM */
    }
};
static const jsr82HandlerType cmDmDiscMsgHandler[NUM_OF_JSR82_STATES][CSR_BT_CM_DM_PRIM_UPSTREAM_COUNT] =
{
    {   /* IDLE_S */
        NULL,                                   /* CSR_BT_CM_SET_LOCAL_NAME_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */
        NULL,                                   /* CSR_BT_CM_GET_LINK_QUALITY_CFM */
        NULL,                                   /* CSR_BT_CM_READ_RSSI_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_WRITE_COD_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_COD_CFM */
        NULL,                                   /* CSR_BT_CM_READ_LOCAL_VERSION_CFM */
        NULL,                                   /* CSR_BT_CM_READ_REMOTE_VERSION_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_SCAN_ENABLE_CFM */
        CsrBtJsr82CmDynQueueHandler                  /* CSR_BT_CM_WRITE_SCAN_ENABLE_CFM */
    },
    {   /* BUSY_S */
        NULL,                                   /* CSR_BT_CM_SET_LOCAL_NAME_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */
        NULL,                                   /* CSR_BT_CM_GET_LINK_QUALITY_CFM */
        NULL,                                   /* CSR_BT_CM_READ_RSSI_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_WRITE_COD_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_COD_CFM */
        NULL,                                   /* CSR_BT_CM_READ_LOCAL_VERSION_CFM */
        NULL,                                   /* CSR_BT_CM_READ_REMOTE_VERSION_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_SCAN_ENABLE_CFM */
        CsrBtJsr82CmDynQueueHandler                  /* CM_WRITE_SCAN_ENABLE_CFM */
    },
    {   /* DISCOVERY_S */
        NULL,                                   /* CSR_BT_CM_SET_LOCAL_NAME_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */
        NULL,                                   /* CSR_BT_CM_GET_LINK_QUALITY_CFM */
        NULL,                                   /* CSR_BT_CM_READ_RSSI_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_WRITE_COD_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_COD_CFM */
        NULL,                                   /* CSR_BT_CM_READ_LOCAL_VERSION_CFM */
        NULL,                                   /* CSR_BT_CM_READ_REMOTE_VERSION_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_SCAN_ENABLE_CFM */
        CsrBtJsr82CmDynQueueHandler                  /* CM_WRITE_SCAN_ENABLE_CFM */
    },
    {   /* SERV_SEARCH_S */
        NULL,                                   /* CSR_BT_CM_SET_LOCAL_NAME_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */
        NULL,                                   /* CSR_BT_CM_GET_LINK_QUALITY_CFM */
        NULL,                                   /* CSR_BT_CM_READ_RSSI_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_WRITE_COD_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_COD_CFM */
        NULL,                                   /* CSR_BT_CM_READ_LOCAL_VERSION_CFM */
        NULL,                                   /* CSR_BT_CM_READ_REMOTE_VERSION_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_SCAN_ENABLE_CFM */
        CsrBtJsr82CmDynQueueHandler                  /* CM_WRITE_SCAN_ENABLE_CFM */
    },
    {   /* UPDATING_RECORD_S */
        NULL,                                   /* CSR_BT_CM_SET_LOCAL_NAME_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */
        NULL,                                   /* CSR_BT_CM_GET_LINK_QUALITY_CFM */
        NULL,                                   /* CSR_BT_CM_READ_RSSI_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_WRITE_COD_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_COD_CFM */
        NULL,                                   /* CSR_BT_CM_READ_LOCAL_VERSION_CFM */
        NULL,                                   /* CSR_BT_CM_READ_REMOTE_VERSION_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_SCAN_ENABLE_CFM */
        CsrBtJsr82CmDynQueueHandler                  /* CM_WRITE_SCAN_ENABLE_CFM */
    },
    {   /* WRITE_COD_S */
        NULL,                                   /* CSR_BT_CM_SET_LOCAL_NAME_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */
        NULL,                                   /* CSR_BT_CM_GET_LINK_QUALITY_CFM */
        NULL,                                   /* CSR_BT_CM_READ_RSSI_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_WRITE_COD_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_COD_CFM */
        NULL,                                   /* CSR_BT_CM_READ_LOCAL_VERSION_CFM */
        NULL,                                   /* CSR_BT_CM_READ_REMOTE_VERSION_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_SCAN_ENABLE_CFM */
        CsrBtJsr82CmDynQueueHandler                  /* CM_WRITE_SCAN_ENABLE_CFM */
    },
    {   /* SELECT_SERVICE_S */
        NULL,                                   /* CSR_BT_CM_SET_LOCAL_NAME_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */
        NULL,                                   /* CSR_BT_CM_GET_LINK_QUALITY_CFM */
        NULL,                                   /* CSR_BT_CM_READ_RSSI_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_WRITE_COD_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_COD_CFM */
        NULL,                                   /* CSR_BT_CM_READ_LOCAL_VERSION_CFM */
        NULL,                                   /* CSR_BT_CM_READ_REMOTE_VERSION_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_SCAN_ENABLE_CFM */
        CsrBtJsr82CmDynQueueHandler                  /* CM_WRITE_SCAN_ENABLE_CFM */
    },
    {   /* RD_NAME_S */
        NULL,                                   /* CSR_BT_CM_SET_LOCAL_NAME_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */
        NULL,                                   /* CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_LOCAL_NAME_CFM */
        NULL,                                   /* CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */
        NULL,                                   /* CSR_BT_CM_GET_LINK_QUALITY_CFM */
        NULL,                                   /* CSR_BT_CM_READ_RSSI_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_WRITE_COD_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_COD_CFM */
        NULL,                                   /* CSR_BT_CM_READ_LOCAL_VERSION_CFM */
        NULL,                                   /* CSR_BT_CM_READ_REMOTE_VERSION_CFM */
        CsrBtJsr82CmDynQueueHandler,                 /* CSR_BT_CM_READ_SCAN_ENABLE_CFM */
        CsrBtJsr82CmDynQueueHandler                  /* CM_WRITE_SCAN_ENABLE_CFM */
    },
    {   /* CLEANUP_S */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_SET_LOCAL_NAME_CFM */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_READ_LOCAL_NAME_CFM */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_GET_LINK_QUALITY_CFM */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_READ_RSSI_CFM */
        cleanupWriteCodHandler,                 /* CSR_BT_CM_WRITE_COD_CFM */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_READ_COD_CFM */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_READ_LOCAL_VERSION_CFM */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_READ_REMOTE_VERSION_CFM */
        CsrBtJsr82FreeMessage,                       /* CSR_BT_CM_READ_SCAN_ENABLE_CFM */
        CsrBtJsr82FreeMessage                        /* CSR_BT_CM_WRITE_SCAN_ENABLE_CFM */
    }
};

static const jsr82HandlerType sdMsgHandler[NUM_OF_JSR82_STATES][CSR_BT_SD_PRIM_UPSTREAM_COUNT] =
{
    {   /* IDLE_S */
        CsrBtJsr82RemoteNameCfmHandler,            /* CSR_BT_SD_READ_DEVICE_INFO_CFM */
        NULL,                                 /* CSR_BT_SD_SEARCH_RESULT_IND */
        NULL,                                 /* CSR_BT_SD_CLOSE_SEARCH_IND */
        NULL,                                 /* CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_IND */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_CFM */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_IND */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_MEMORY_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_SEARCH_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_CLEANUP_CFM */
    },
    {   /* BUSY_S */
        CsrBtJsr82SdReadDeviceInfoCfmHandler,      /* CSR_BT_SD_READ_DEVICE_INFO_CFM */
        NULL,                                 /* CSR_BT_SD_SEARCH_RESULT_IND */
        NULL,                                 /* CSR_BT_SD_CLOSE_SEARCH_IND */
        NULL,                                 /* CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */
        CsrBtJsr82SdReadDeviceListIndHandler,      /* CSR_BT_SD_READ_DEVICE_LIST_IND */
        CsrBtJsr82SdReadDeviceListCfmHandler,      /* CSR_BT_SD_READ_DEVICE_LIST_CFM */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_IND */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_MEMORY_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_SEARCH_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_CLEANUP_CFM */
    },
    {   /* DISCOVERY_S */
        CsrBtJsr82RemoteNameCfmHandler,            /* CSR_BT_SD_READ_DEVICE_INFO_CFM */
        NULL,                                 /* CSR_BT_SD_SEARCH_RESULT_IND */
        NULL,                                 /* CSR_BT_SD_CLOSE_SEARCH_IND */
        NULL,                                 /* CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_IND */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_CFM */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_IND */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_MEMORY_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_SEARCH_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_CLEANUP_CFM */
    },
    {   /* SERV_SEARCH_S */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_INFO_CFM */
        NULL,                                 /* CSR_BT_SD_SEARCH_RESULT_IND */
        NULL,                                 /* CSR_BT_SD_CLOSE_SEARCH_IND */
        NULL,                                 /* CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_IND */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_CFM */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_IND */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_MEMORY_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_SEARCH_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_CLEANUP_CFM */
    },
    {   /* UPDATING_RECORD_S */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_INFO_CFM */
        NULL,                                 /* CSR_BT_SD_SEARCH_RESULT_IND */
        NULL,                                 /* CSR_BT_SD_CLOSE_SEARCH_IND */
        NULL,                                 /* CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_IND */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_CFM */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_IND */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_MEMORY_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_SEARCH_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_CLEANUP_CFM */
    },
    {   /* WRITE_COD_S */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_INFO_CFM */
        NULL,                                 /* CSR_BT_SD_SEARCH_RESULT_IND */
        NULL,                                 /* CSR_BT_SD_CLOSE_SEARCH_IND */
        NULL,                                 /* CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_IND */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_CFM */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_IND */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_MEMORY_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_SEARCH_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_CLEANUP_CFM */
    },
    {   /* SELECT_SERVICE_S */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_INFO_CFM */
        NULL,                                 /* CSR_BT_SD_SEARCH_RESULT_IND */
        NULL,                                 /* CSR_BT_SD_CLOSE_SEARCH_IND */
        NULL,                                 /* CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_IND */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_CFM */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_IND */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_MEMORY_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_SEARCH_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_CLEANUP_CFM */
    },
    {   /* RD_NAME_S */
        CsrBtJsr82RemoteNameCfmHandler,            /* CSR_BT_SD_READ_DEVICE_INFO_CFM */
        NULL,                                 /* CSR_BT_SD_SEARCH_RESULT_IND */
        NULL,                                 /* CSR_BT_SD_CLOSE_SEARCH_IND */
        NULL,                                 /* CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_IND */
        NULL,                                 /* CSR_BT_SD_READ_DEVICE_LIST_CFM */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_IND */
        NULL,                                 /* CSR_BT_SD_READ_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */
        NULL,                                 /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_MEMORY_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_SEARCH_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_CLEANUP_CFM */
    },
    {   /* CLEANUP_S */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_READ_DEVICE_INFO_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_SEARCH_RESULT_IND */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_CLOSE_SEARCH_IND */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_READ_DEVICE_LIST_IND */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_READ_DEVICE_LIST_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_READ_SERVICE_RECORD_IND */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_READ_SERVICE_RECORD_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_MEMORY_CONFIG_CFM */
        CsrBtJsr82FreeMessage,                     /* CSR_BT_SD_SEARCH_CONFIG_CFM */
        CsrBtJsr82FreeMessage                      /* CSR_BT_SD_CLEANUP_CFM */
    }
};

void CsrBtJsr82Init(void **gash)
{
    Jsr82MainInstanceData *instData;
    *gash                               = (void *)CsrPmemZalloc(sizeof(Jsr82MainInstanceData));
    instData                            = (Jsr82MainInstanceData *) (*gash);

    CsrBtJsr82SetIdle(instData);
}

static void freeMessages(Jsr82MainInstanceData *instData)
{
    CsrPmemFree(instData->recvMsg);
    while (CsrMessageQueuePop(&instData->saveQueue, &instData->eventClass, &instData->recvMsg)
            || CsrMessageQueuePop(&instData->pendingQueue, &instData->eventClass, &instData->recvMsg)
            || CsrSchedMessageGet(&instData->eventClass, &instData->recvMsg))
    {
        CsrBtJsr82FreeMessage(instData);
    }
    instData->recvMsg = NULL;
}

static void freeLinkedList(void **listStart)
{
    jsr82LinkedListStruct *element;
    jsr82LinkedListStruct *prevElement;

    element  = (jsr82LinkedListStruct *)(*listStart);

    while(element)
    {
        prevElement = element;
        element = element->next;
        CsrPmemFree(prevElement);
    }
    *listStart = NULL;
}


static void cleanupDiscovery(Jsr82MainInstanceData *instData)
{
    if(instData->getAttr)
    {
        CsrPmemFree(instData->getAttr->serviceRecHandles);
        CsrPmemFree(instData->getAttr->attributes);
        CsrPmemFree(instData->getAttr->attrSet);
        CsrPmemFree(instData->getAttr);
        instData->getAttr = NULL;
    }
    if(instData->servSearch)
    {
        CsrPmemFree(instData->servSearch->attrSet);
        CsrPmemFree(instData->servSearch);
        instData->servSearch = NULL;
    }
    freeLinkedList((void **)&instData->validSearches);

    if (instData->selectService)
    {
        freeLinkedList((void **)&instData->selectService->list);
        CsrPmemFree(instData->selectService->uuid);
        CsrPmemFree(instData->selectService->handleList);
        CsrPmemFree(instData->selectService);
        instData->selectService = NULL;
    }

    freeLinkedList((void **)&instData->requestedNames);

    if (instData->populate)
    {
        CsrPmemFree(instData->populate->attrSet);
        CsrPmemFree(instData->populate);
        instData->populate = NULL;
    }
}


static void l2capReleaseBuffers(Jsr82MainInstanceData *instData)
{
    /* limited cleanup for JvmDown signal */
    psmValue *psmPtr = instData->psmList;

    while(psmPtr)
    {
        CsrCmnListDeinit(&(psmPtr->cidList));
        psmPtr->assigned        = FALSE;
        psmPtr->client          = FALSE;
        psmPtr->destroy         = FALSE;
        psmPtr->returnFunction  = NULL;
        psmPtr                  = psmPtr->next;
    }
}

static CsrBool csrBtJsr82FindConnectedElement(CsrCmnListElm_t *elem, void *value)
{ /* This function will return TRUE if the given btConnId match               */
    jsr82CidElement *theElement = (jsr82CidElement *) elem;
    CSR_UNUSED(value);

    if (theElement->cidInst)
    {
        if (theElement->cidInst->connected)
        {
            return (TRUE);
        }
    }
    return (FALSE);
}

static CsrBool cleanupSendDisconnect(Jsr82MainInstanceData *instData)
{
    jsr82CidElement * cidElement;
    rfcConnectionPoolStruct *rfcPool;
    psmValue *psmPtr;
    CsrUint16 i;
    rfcConnectionStruct rfcConnection;
    rfcPool = (rfcConnectionPoolStruct *)instData->rfcConnectionPool;

    while(rfcPool)
    {
        for(i=0; i<RFC_CONNECTION_POOL_SIZE; i++)
        {
            rfcConnection = rfcPool->connectionList[i];
            if(rfcConnection.connected)
            {
                CsrBtCmDisconnectReqSend(rfcConnection.jsr82ConnId);
                return TRUE;
            }
        }
        rfcPool = rfcPool->next;
    }

    psmPtr = instData->psmList;

    while(psmPtr)
    {
        cidElement = JSR82_FIND_CID_ELEMENT(csrBtJsr82FindConnectedElement, NULL);

        if (cidElement)
        {
            CsrBtCml2caDisconnectReqSend(cidElement->cidInst->cid);
            return TRUE;
        }
        psmPtr = psmPtr->next;
    }
    return FALSE;
}

void CsrBtJsr82CleanupDisconnect(Jsr82MainInstanceData *instData)
{
    /* Disconnects any active L2CAP and RFCOMM connections for cleanup/deinit */
    CsrBtCmPrim *primType;
    primType = (CsrBtCmPrim *)instData->recvMsg;

    switch(*primType)
    {
    case CSR_BT_CM_CONNECT_CFM:
        {
            CsrBtCmConnectCfm *prim;
            prim = (CsrBtCmConnectCfm *)instData->recvMsg;

            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL,
                    prim->deviceAddr,prim->btConnId);

            CsrBtCmDisconnectReqSend(prim->btConnId);
            break;
        }
    case CSR_BT_CM_L2CA_CONNECT_CFM:
        {
            CsrBtCmL2caConnectCfm *prim;
            prim = (CsrBtCmL2caConnectCfm *)instData->recvMsg;

            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL, prim->deviceAddr, prim->btConnId);

            CsrBtCml2caDisconnectReqSend(prim->btConnId);
            break;
        }
    case CSR_BT_CM_DISCONNECT_IND:
        {
            CsrBtCmDisconnectInd *prim = (CsrBtCmDisconnectInd *)instData->recvMsg;

            if (prim->status == TRUE)
            {
                rfcConnectionStruct *rfcConnection;

                rfcConnection = getRfcConnectionFromConnId(prim->btConnId, instData->rfcConnectionPool);
                if (rfcConnection)
                {
                    if (prim->localTerminated == TRUE)
                    {
                        CsrBtJsr82PropgateRfcDisconnectIndEvent(instData, rfcConnection->serverChannel,
                            CSR_BT_RESULT_CODE_JSR82_SUCCESS, TRUE);
                    }
                    else
                    {
                        CsrBtJsr82PropgateRfcDisconnectIndEvent(instData, rfcConnection->serverChannel,
                            CSR_BT_RESULT_CODE_JSR82_SUCCESS, FALSE);
                    }
                }
                CsrBtJsr82RfcCleanupConnection(instData);
                CsrBtJsr82CleanupReqHandler(instData);
            }
            else
            {
                CsrBtCmDisconnectReqSend(prim->btConnId);
            }
            break;
        }
    case CSR_BT_CM_L2CA_DISCONNECT_IND:
        {
            CsrBtCmL2caDisconnectInd *prim = (CsrBtCmL2caDisconnectInd *)instData->recvMsg;

            if (prim->reasonSupplier == CSR_BT_SUPPLIER_CM &&
                prim->reasonCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
            {

                if (prim->localTerminated == TRUE)
                {
                    CsrBtJsr82PropgateL2caDisconnectEvent(instData, prim->btConnId,
                        CSR_BT_RESULT_CODE_JSR82_SUCCESS, TRUE);
                }
                else
                {
                    CsrBtJsr82PropgateL2caDisconnectEvent(instData, prim->btConnId,
                        CSR_BT_RESULT_CODE_JSR82_SUCCESS, FALSE);
                }
            }
            else
            {
                /* General link loss error */
                CsrBtJsr82PropgateL2caDisconnectEvent(instData, prim->btConnId,
                    CSR_BT_RESULT_CODE_JSR82_LINK_LOST, FALSE);
            }

            CsrBtJsr82RemoveL2capConnection(prim->btConnId, instData);
            CsrBtJsr82CleanupReqHandler(instData);
            break;
        }
    default:
        break;
    }
}


static void cleanupRfc(Jsr82MainInstanceData *instData)
{
    rfcConnectionPoolStruct *pool;
    rfcConnectionPoolStruct *prevPool;

    rfcConnectionStruct     connection;
    CsrUint16 i;
    pool = (rfcConnectionPoolStruct *)instData->rfcConnectionPool;

    while(pool)
    {
        for(i=0; i<RFC_CONNECTION_POOL_SIZE; i++)
        {
            connection = pool->connectionList[i];
            CsrPmemFree(connection.receiveInfo.receiveBuffer);
            connection.receiveInfo.receiveBuffer = NULL;
            if (connection.sendInfo)
            {
                CsrPmemFree(connection.sendInfo->buffer);
                CsrPmemFree(connection.sendInfo);
                connection.sendInfo = NULL;
            }
        }
        prevPool = pool;
        pool = pool->next;
        CsrPmemFree(prevPool);
    }
    instData->rfcConnectionPool = NULL;
}

static CsrBool cleanupSendUnregister(Jsr82MainInstanceData *instData)
{
    serviceHandlePoolStruct *pool;
    serviceHandleMapping    handle;
    CsrUint16 i;
    pool = instData->serviceRecHandlePool;

    while(pool)
    {
        for(i=0; i<SERVICE_REC_HANDLE_POOL_SIZE; i++)
        {
            handle = pool->mapList[i];
            if(handle.allocated)
            {
                CsrBtCmSdsUnRegisterReqSend(CSR_BT_JSR82_IFACEQUEUE, handle.downstreamHandle);
                return TRUE;
            }
        }
        pool = pool->next;
    }

    return FALSE;
}

static void cleanupUnregisterHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82SrCleanupUnregisterCfm(instData);
    CsrBtJsr82CleanupReqHandler(instData);
}


static void cleanupServiceRecord(Jsr82MainInstanceData *instData)
{
    if(instData->populate)
    {
        CsrPmemFree(instData->populate->attrSet);
        CsrPmemFree(instData->populate);
        instData->populate = NULL;

    }

    freeLinkedList((void **)&instData->serviceRecHandlePool);
}

static void cleanupRemoteDevice(Jsr82MainInstanceData *instData)
{
    CsrMemSet(&(instData->aclConnections), 0, MAX_ACL_LINKS*sizeof(aclConnStruct));
}

static void cleanupLocalDevice(Jsr82MainInstanceData *instData)
{
    CsrPmemFree(instData->updateMessage);
    instData->updateMessage = NULL;
}

static void cleanupWriteCodHandler(Jsr82MainInstanceData *instData)
{
    instData->javaDeviceClassBits = 0x000000;
    CsrBtJsr82CleanupReqHandler(instData);
}

static CsrBool cleanupCod(Jsr82MainInstanceData *instData)
{
    if (instData->javaDeviceClassBits != 0x000000)
    {
        CsrBtCmWriteServiceCodReqSend(CSR_BT_JSR82_IFACEQUEUE, 0x000000);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void CsrBtJsr82CleanupReqHandler(Jsr82MainInstanceData *instData)
{
    freeMessages(instData);
    CsrBtJsr82SetCleanup(instData);
    instData->doingCleanup = TRUE;

    if (instData->inquiry)
    {
        if (instData->inquiry->state == INQUIRY_STATE)
        {
            /*SdCancelSearchReqSend(CSR_BT_JSR82_IFACEQUEUE);*/
            CsrBtCmCancelInquiryReqSend(CSR_BT_JSR82_IFACEQUEUE);
        }
    }
    if (!cleanupCod(instData))
    {
        if(!cleanupSendDisconnect(instData))
        {
            if(!cleanupSendUnregister(instData))
            {
                cleanupDiscovery(instData);
                cleanupServiceRecord(instData);
                l2capReleaseBuffers(instData);
                cleanupRfc(instData);
                cleanupRemoteDevice(instData);
                cleanupLocalDevice(instData);
                instData->doingCleanup = FALSE;
            }
        }
    }
}

#ifdef ENABLE_SHUTDOWN
/****************************************************************************
This function is called by the scheduler to perform a graceful shutdown
of a scheduler task.
This function must:
1)  empty the input message queue and free any allocated memory in the
messages.
2)  free any instance data that may be allocated.
****************************************************************************/
static void cleanupL2cap(Jsr82MainInstanceData *instData)
{
    /* run ReleaseBuffers function before this one */
    freeLinkedList((void **)&instData->psmList);
}

static void freeJsr82QElement(Jsr82MainInstanceData *instData, Jsr82QElement *qElement)
{
    if (qElement->orgMsg)
    {
        instData->eventClass = CSR_BT_JSR82_PRIM;
        instData->recvMsg = qElement->orgMsg;
        CsrBtJsr82FreeMessage(instData);
    }
    if (qElement->reqMsg)
    {
        instData->eventClass = qElement->event;
        instData->recvMsg = qElement->reqMsg;
        CsrBtJsr82FreeMessage(instData);
    }
    instData->recvMsg = NULL;
}

static void freeDynQueue(Jsr82MainInstanceData *instData, Jsr82QElement **dynQueue, CsrUint16 dynQueueLength)
{
    CsrUint16 i;
    for (i = 0; i < dynQueueLength; i++)
    {
        while (dynQueue[i])
        {
            Jsr82QElement *qElement;

            qElement = dynQueue[i];
            dynQueue[i] = dynQueue[i]->next;
            freeJsr82QElement(instData, qElement);
            CsrPmemFree(qElement);
        }
    }
}

static void freeDynQueues(Jsr82MainInstanceData *instData)
{
    freeDynQueue(instData, (Jsr82QElement **)instData->cmRfcDynJumpTable, CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT);
    freeDynQueue(instData, (Jsr82QElement **)instData->cmSdcDynJumpTable, CSR_BT_CM_SDC_PRIM_UPSTREAM_COUNT);
    freeDynQueue(instData, (Jsr82QElement **)instData->cmL2caDynJumpTable, CSR_BT_CM_L2CA_PRIM_UPSTREAM_COUNT);
    freeDynQueue(instData, (Jsr82QElement **)instData->cmBnepDynJumpTable, CSR_BT_CM_BNEP_PRIM_UPSTREAM_COUNT);
    freeDynQueue(instData, (Jsr82QElement **)instData->cmInquiryDynJumpTable, CSR_BT_CM_INQUIRY_PRIM_UPSTREAM_COUNT);
    freeDynQueue(instData, (Jsr82QElement **)instData->cmDmDynJumpTable, CSR_BT_CM_DM_PRIM_UPSTREAM_COUNT);
    freeDynQueue(instData, (Jsr82QElement **)instData->cmDm1P2DynJumpTable, CSR_BT_CM_DM_1P2_PRIM_UPSTREAM_COUNT);
    freeDynQueue(instData, (Jsr82QElement **)instData->cmSendDynJumpTable, CSR_BT_CM_SEND_PRIM_UPSTREAM_COUNT);
    freeDynQueue(instData, (Jsr82QElement **)instData->sdDynJumpTable, CSR_BT_SD_PRIM_UPSTREAM_COUNT);
    freeDynQueue(instData, (Jsr82QElement **)instData->scDynJumpTable, CSR_BT_SC_PRIM_UPSTREAM_COUNT);
}

void CsrBtJsr82Deinit(void **gash)
{
    Jsr82MainInstanceData   *instData;

    instData = (Jsr82MainInstanceData *) (*gash);

    instData->deinit = TRUE;

    freeMessages(instData);
    freeDynQueues(instData);
    cleanupDiscovery(instData);
    cleanupServiceRecord(instData);
    l2capReleaseBuffers(instData);
    cleanupL2cap(instData);
    cleanupRfc(instData);
    cleanupRemoteDevice(instData);
    cleanupLocalDevice(instData);
    CsrPmemFree(instData->inqDeviceList);
    CsrPmemFree(instData);

}

#endif

void CsrBtJsr82Handler(void **gash)
{
    Jsr82MainInstanceData   *instData;
    void                    *msg=NULL;
    Jsr82State              state;

    instData = (Jsr82MainInstanceData *) (*gash);

    if(!instData->restoreFlag)
    {
        CsrSchedMessageGet(&(instData->eventClass), &msg);
    }
    else
    {
        if(!CsrMessageQueuePop( &(instData->saveQueue), &(instData->eventClass), &msg))
        {
            instData->restoreFlag = FALSE;
            CsrSchedMessageGet(&(instData->eventClass), &msg);
        }
    }
    instData->recvMsg = msg;

    state = instData->state;
    switch(instData->eventClass)
    {
    case CSR_BT_JSR82_PRIM:
        {
            CsrBtJsr82Prim      *primType;

            primType = (CsrBtJsr82Prim *)msg;

            if ((*primType - CSR_BT_JSR82_PRIM_DOWNSTREAM_LOWEST < CSR_BT_JSR82_PRIM_DOWNSTREAM_COUNT) && (jsr82MsgHandler[state][*primType - CSR_BT_JSR82_PRIM_DOWNSTREAM_LOWEST] != NULL))
            {
                jsr82MsgHandler[state][*primType - CSR_BT_JSR82_PRIM_DOWNSTREAM_LOWEST](instData);
            }
            else if ((state == UPDATING_RECORD_S) && ((*primType == CSR_BT_JSR82_SR_REGISTER_RECORD_CFM) || (*primType == CSR_BT_JSR82_SR_REMOVE_RECORD_CFM)))
            {
                if (*primType == CSR_BT_JSR82_SR_REGISTER_RECORD_CFM)
                {
                    CsrBtJsr82EndLdUpdateRecord(instData);
                }
                else if (*primType == CSR_BT_JSR82_SR_REMOVE_RECORD_CFM)
                {
                    CsrBtJsr82MidLdUpdateRecord(instData);
                }
                else
                {
                    CsrBtJsr82FreeMessage(instData);
                }
            }
            else if (*primType == CSR_BT_JSR82_CONTINUE_RFC_SEND_DATA)
            {
                switch (state)
                {
                    case IDLE_S:
                        {
                            CsrBtJsr82ContinueRfcSendDataHandler(instData);
                            break;
                        }
                    case CLEANUP_S:
                        {
                            CsrBtJsr82LeaveCleanup(instData);
                            break;
                        }
                    default:
                        {
                            CsrBtJsr82SaveMessage(instData);
                            break;
                        }
                }
            }

            else if(*primType != CSR_BT_JSR82_HOUSE_CLEANING)
            {

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("JSR82",
                                        instData->eventClass,
                                        *primType,
                                        state);
#endif
                CsrBtJsr82FreeMessage(instData);
            }

            break;
        }
    case CSR_BT_SC_PRIM:
        {
            CsrBtScPrim *primType;
            primType = (CsrBtScPrim *)msg;
            switch(*primType)
            {
            case CSR_BT_SC_AUTHENTICATE_CFM:
                {
                    CsrBtJsr82RdAuthenticate(instData);
                    break;
                }
            case CSR_BT_SC_ENCRYPTION_CFM:
                {
                    CsrBtJsr82RdEncrypt(instData);
                    break;
                }
            default:
                {
                    CsrBtJsr82RdForwardScPrim(instData);
                    break;
                }
            }

            break;
        }
    case CSR_BT_SD_PRIM:
        {
            CsrBtSdPrim *primType;
            primType = (CsrBtSdPrim *)msg;
            if((*primType >= CSR_BT_SD_PRIM_UPSTREAM_LOWEST) &&
               (*primType <= CSR_BT_SD_PRIM_UPSTREAM_HIGHEST) &&
               (sdMsgHandler[state][(CsrUint16)(*primType - CSR_BT_SD_PRIM_UPSTREAM_LOWEST)] != NULL))
            {
                sdMsgHandler[state][(CsrUint16)(*primType - CSR_BT_SD_PRIM_UPSTREAM_LOWEST)](instData);
            }

            else
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("JSR82",
                                        instData->eventClass,
                                        *primType,
                                        state);
#endif
                CsrBtSdFreeUpstreamMessageContents(instData->eventClass,
                                                   instData->recvMsg);
                CsrBtSdPrivateFreeUpstreamMessageContents(instData->eventClass,
                                                          instData->recvMsg);

            }

          break;
        }
    case CSR_BT_CM_PRIM:
        {
            CsrPrim      *primType;
            primType = (CsrPrim *)msg;

            if ((*primType >= CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST) &&
                (*primType <= CSR_BT_CM_RFC_PRIM_UPSTREAM_HIGHEST) &&
                (cmRfcConnMsgHandler[state][(CsrUint16)(*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)] != NULL))
            {
                cmRfcConnMsgHandler[state][(CsrUint16)(*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)](instData);
            }
            else if ((*primType >= CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST) &&
                     (*primType <= CSR_BT_CM_SDC_PRIM_UPSTREAM_HIGHEST) &&
                     (cmSdcConnMsgHandler[state][(CsrUint16)(*primType - CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST)] != NULL))
            {
                cmSdcConnMsgHandler[state][(CsrUint16)(*primType - CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST)](instData);
            }
            else if ((*primType >= CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST) &&
                     (*primType <= CSR_BT_CM_L2CA_PRIM_UPSTREAM_HIGHEST) &&
                     (cmL2caConnMsgHandler[state][(CsrUint16)(*primType - CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST)] != NULL))
            {
                cmL2caConnMsgHandler[state][(CsrUint16)(*primType - CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST)](instData);
            }
            else if ((*primType >= CSR_BT_CM_INQUIRY_PRIM_UPSTREAM_LOWEST) &&
                     (*primType <= CSR_BT_CM_INQUIRY_PRIM_UPSTREAM_HIGHEST) &&
                     (cmInquiryDiscMsgHandler[state][(CsrUint16)(*primType - CSR_BT_CM_INQUIRY_PRIM_UPSTREAM_LOWEST)] != NULL))
            {
                cmInquiryDiscMsgHandler[state][(CsrUint16)(*primType - CSR_BT_CM_INQUIRY_PRIM_UPSTREAM_LOWEST)](instData);
            }
            else if ((*primType >= CSR_BT_CM_DM_PRIM_UPSTREAM_LOWEST) &&
                     (*primType <= CSR_BT_CM_DM_PRIM_UPSTREAM_HIGHEST) &&
                     (cmDmDiscMsgHandler[state][(CsrUint16)(*primType - CSR_BT_CM_DM_PRIM_UPSTREAM_LOWEST)] != NULL))
            {
                cmDmDiscMsgHandler[state][(CsrUint16)(*primType - CSR_BT_CM_DM_PRIM_UPSTREAM_LOWEST)](instData);
            }
            else if (*primType == CSR_BT_CM_ROLE_DISCOVERY_CFM)
            {
                CsrBtJsr82CmDynQueueHandler(instData);
            }
            else if (*primType == CSR_BT_CM_READ_IAC_CFM)
            {
                CsrBtJsr82CmDynQueueHandler(instData);
            }
            else if (*primType == CSR_BT_CM_WRITE_IAC_IND)
            {
                CsrBtJsr82CmDynQueueHandler(instData);
            }
            else if (*primType == CSR_BT_CM_READ_ENCRYPTION_STATUS_CFM)
            {
                CsrBtJsr82CmReadEncryptionStatusCfmHandler(instData);
            }
            else if ((*primType == CSR_BT_CM_SDC_UUID128_SEARCH_IND) && (state == SELECT_SERVICE_S))
            {
                CsrBtJsr82CmSelServiceSdc128searchIndHandler(instData);
            }
            else
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("JSR82", instData->eventClass, *primType, state);
#endif
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, instData->recvMsg);
            }
            break;
        }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
    default:
        {
            CsrStateEventException("JSR82",
                                    instData->eventClass,
                                    0,
                                    state);
            break;
        }
#endif
    }
    CsrPmemFree(instData->recvMsg);
    instData->recvMsg = NULL;
}

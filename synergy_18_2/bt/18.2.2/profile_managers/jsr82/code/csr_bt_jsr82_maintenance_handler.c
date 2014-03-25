/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_bt_result.h"
#include "csr_bt_jsr82_main.h"
#include "csr_bt_jsr82_maintenance_handler.h"
#include "csr_bt_jsr82_rfcomm.h"
#include "csr_bt_jsr82_prim.h"
#include "csr_pmem.h"
#include "csr_message_queue.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_private_prim.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_private_lib.h"

typedef void (* jsr82StateFunction)(Jsr82MainInstanceData *instData);

static const jsr82StateFunction jsr82State[NUM_OF_JSR82_STATES] =
{
        CsrBtJsr82SetIdle,
        CsrBtJsr82SetBusy,
        CsrBtJsr82SetDiscovery,
        CsrBtJsr82SetServSearch,
        CsrBtJsr82SetUpdatingRecord,
        CsrBtJsr82SetWriteCod,
        CsrBtJsr82SetSelectService,
        CsrBtJsr82SetRdName,
        CsrBtJsr82SetCleanup
};

void CsrBtJsr82MessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_JSR82_PRIM, msg);
}

void CsrBtJsr82SaveMessage(Jsr82MainInstanceData *instData)
{
    CsrMessageQueuePush(&instData->saveQueue, instData->eventClass, instData->recvMsg);
    instData->recvMsg = NULL;
}

void CsrBtJsr82PendingMessage(Jsr82MainInstanceData *instData)
{
    CsrMessageQueuePush(&instData->pendingQueue, instData->eventClass, instData->recvMsg);
    instData->recvMsg = NULL;
}
    
void CsrBtJsr82LeaveCleanup(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82SaveMessage(instData);
    if (!instData->doingCleanup)
    {
        CsrBtJsr82SetIdle(instData);
    }
}

void CsrBtJsr82RestoreSavedMessages(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82HouseCleaning  *prim;

    instData->restoreFlag = TRUE;
    prim = CsrPmemAlloc(sizeof(CsrBtJsr82HouseCleaning));
    prim->type = CSR_BT_JSR82_HOUSE_CLEANING;
    CsrBtJsr82MessagePut(CSR_BT_JSR82_IFACEQUEUE, prim);
}

void CsrBtJsr82SetBusy(Jsr82MainInstanceData *instData)
{
    instData->restoreFlag = FALSE;
    instData->state = BUSY_S;
}


void CsrBtJsr82SetDiscovery(Jsr82MainInstanceData *instData)
{
    instData->restoreFlag = FALSE;
    instData->state = DISCOVERY_S;
}


void CsrBtJsr82SetIdle(Jsr82MainInstanceData *instData)
{
    instData->state=IDLE_S;
    CsrBtJsr82RestoreSavedMessages(instData);
    CsrMessageQueueRestore(&instData->pendingQueue, CSR_BT_JSR82_IFACEQUEUE);
}

void CsrBtJsr82SetServSearch(Jsr82MainInstanceData *instData)
{
    instData->restoreFlag = FALSE;
    instData->state=SERV_SEARCH_S;
}

void CsrBtJsr82SetUpdatingRecord(Jsr82MainInstanceData *instData)
{
    instData->state = UPDATING_RECORD_S;
    CsrBtJsr82RestoreSavedMessages(instData);
}

void CsrBtJsr82SetWriteCod(Jsr82MainInstanceData *instData)
{
    instData->restoreFlag = FALSE;
    instData->state=WRITE_COD_S;
}

void CsrBtJsr82SetSelectService(Jsr82MainInstanceData *instData)
{
    instData->restoreFlag = FALSE;
    instData->state = SELECT_SERVICE_S;
}

void CsrBtJsr82SetRdName(Jsr82MainInstanceData *instData)
{
    instData->restoreFlag = FALSE;
    instData->state = RD_NAME_S;
}

void CsrBtJsr82SetCleanup(Jsr82MainInstanceData *instData)
{
    instData->restoreFlag = FALSE;
    instData->state=CLEANUP_S;
}

void CsrBtJsr82SetState(Jsr82MainInstanceData *instData, Jsr82State state)
{
    if (jsr82State[state])
    {
        jsr82State[state](instData);
    }
}

CsrUint16 CsrBtJsr82SetSecurity(CsrUint32 attrs, CsrBool incoming)
{
    CsrUint16 secLevel = 0;;

    if (attrs & CSR_BT_JSR82_AUTHENTICATE)
    {
        secLevel |= CSR_BT_SEC_AUTHENTICATION;
    }
    if (attrs & CSR_BT_JSR82_ENCRYPT)
    {
        secLevel |= CSR_BT_SEC_AUTHENTICATION;
        secLevel |= CSR_BT_SEC_ENCRYPTION;
    }

    secLevel |= CSR_BT_SEC_SPECIFY;

    if (incoming)
    {
        if(attrs & CSR_BT_JSR82_AUTHORIZE)
        { /* Authorization implies authentication */
            secLevel |= CSR_BT_SEC_AUTHENTICATION;
            secLevel |= CSR_BT_SEC_AUTHORISATION;
        }
        CsrBtScSetSecInLevel(&secLevel, secLevel, secLevel, secLevel,
            CSR_BT_RESULT_CODE_JSR82_SUCCESS,
            CSR_BT_RESULT_CODE_JSR82_UNACCEPTABLE_PARAMETER);
    }
    else
    {
        CsrBtScSetSecOutLevel(&secLevel, secLevel, secLevel, secLevel,
            CSR_BT_RESULT_CODE_JSR82_SUCCESS,
            CSR_BT_RESULT_CODE_JSR82_UNACCEPTABLE_PARAMETER);
    }
    return secLevel;
}

static CsrUint16 cmWriteIacRspMsgs[] = {CSR_BT_CM_WRITE_IAC_IND};
static void csrBtJsr82CmWriteIacReqSender(void *reqMsg)
{
    CsrBtCmWriteIacReq *msg;

    msg = (CsrBtCmWriteIacReq *)reqMsg;
    CsrBtCmWriteIacReqSend(msg->appHandle, msg->iac);
}

static CsrUint16 cmReadLocalBdAddrRspMsgs[] = {CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM};
static void csrBtJsr82CmReadLocalBdAddrReqSender(void *reqMsg)
{
    CsrBtCmReadLocalBdAddrReq *msg;

    msg = (CsrBtCmReadLocalBdAddrReq *)reqMsg;
    CsrBtCmReadLocalBdAddrReqSend(msg->phandle);
}

static CsrUint16 cmReadLocalNameRspMsgs[] = {CSR_BT_CM_READ_LOCAL_NAME_CFM};
static void csrBtJsr82CmReadLocalNameReqSender(void *reqMsg)
{
    CsrBtCmReadLocalNameReq *msg;

    msg = (CsrBtCmReadLocalNameReq *)reqMsg;
    CsrBtCmReadLocalNameReqSend(msg->phandle);
}

static CsrUint16 cmReadScanEnableRspMsgs[] = {CSR_BT_CM_READ_SCAN_ENABLE_CFM};
static void csrBtJsr82CmReadScanEnableReqSender(void *reqMsg)
{
    CsrBtCmReadScanEnableReq *msg;

    msg = (CsrBtCmReadScanEnableReq *)reqMsg;
    CsrBtCmReadScanEnableReqSend(msg->appHandle);
}

static CsrUint16 cmWriteScanEnableRspMsgs[] = {CSR_BT_CM_WRITE_SCAN_ENABLE_CFM};
static void csrBtJsr82CmWriteScanEnableReqSender(void *reqMsg)
{
    CsrBtCmWriteScanEnableReq *msg;

    msg = (CsrBtCmWriteScanEnableReq *)reqMsg;
    CsrBtCmWriteScanEnableReqSend(msg->appHandle, msg->disableInquiryScan, msg->disablePageScan);
}

static CsrUint16 cmReadIacRspMsgs[] = {CSR_BT_CM_READ_IAC_CFM};
static void csrBtJsr82CmReadIacReqSender(void *reqMsg)
{
    CsrBtCmReadIacReq *msg;

    msg = (CsrBtCmReadIacReq *)reqMsg;
    CsrBtCmReadIacReqSend(msg->appHandle);
}

static CsrUint16 cmReadCodRspMsgs[] = {CSR_BT_CM_READ_COD_CFM};
static void csrBtJsr82CmReadCodReqSender(void *reqMsg)
{
    CsrBtCmReadCodReq *msg;

    msg = (CsrBtCmReadCodReq *)reqMsg;
    CsrBtCmReadCodReqSend(msg->appHandle);
}

static CsrUint16 cmWriteCodRspMsgs[] = {CSR_BT_CM_WRITE_COD_CFM};
static void csrBtJsr82CmWriteCodReqSender(void *reqMsg)
{
    CsrBtCmWriteCodReq *msg;

    msg = (CsrBtCmWriteCodReq *)reqMsg;
    CsrBtCmWriteServiceCodReqSend(msg->appHandle, msg->serviceClassOfDevice);
}

static CsrUint16 cmRoleDiscoveryRspMsgs[] = {CSR_BT_CM_ROLE_DISCOVERY_CFM};
static void csrBtJsr82CmRoleDiscoveryReqSender(void *reqMsg)
{
    CsrBtCmRoleDiscoveryReq *msg;

    msg = (CsrBtCmRoleDiscoveryReq *)reqMsg;
    CsrBtCmRoleDiscoveryReqSend(msg->phandle, msg->deviceAddr);
}

static CsrUint16 cmL2caRegisterRspMsgs[] = {CSR_BT_CM_L2CA_REGISTER_CFM};
static void csrBtJsr82CmL2caRegisterReqSender(void *reqMsg)
{
    CsrBtCmL2caRegisterReq *msg;

    msg = (CsrBtCmL2caRegisterReq *)reqMsg;
    CsrBtCml2caRegisterReqSend(msg->phandle, msg->localPsm);
}

static CsrUint16 cmRegisterRspMsgs[] = {CSR_BT_CM_REGISTER_CFM};
static void csrBtJsr82CmRegisterReqSender(void *reqMsg)
{
    CsrBtCmRegisterReq *msg;

    msg = (CsrBtCmRegisterReq *)reqMsg;
    CsrBtCmRegisterReqSend(msg->phandle);
}

static const jsr82PrimMapping jsr82CmDmPrimMapping[CSR_BT_CM_DM_PRIM_DOWNSTREAM_COUNT] =
{
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SET_LOCAL_NAME_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_REMOTE_NAME_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SCO_CONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SCO_RENEGOTIATE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SCO_DISCONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DM_MODE_CHANGE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_DELETE_STORE_LINK_KEY_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_REMOVE_DEVICE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_SET_SEC_MODE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_ACL_OPEN_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_AUTHENTICATE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_ENCRYPTION_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_ACL_CLOSE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_SET_DEFAULT_SEC_LEVEL_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_UNREGISTER_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_REGISTER_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DM_SWITCH_ROLE_REQ */
    {csrBtJsr82CmRoleDiscoveryReqSender, cmRoleDiscoveryRspMsgs, sizeof(cmRoleDiscoveryRspMsgs)/sizeof(CsrUint16)},        /* CSR_BT_CM_ROLE_DISCOVERY_REQ */
    {csrBtJsr82CmReadLocalBdAddrReqSender, cmReadLocalBdAddrRspMsgs, sizeof(cmReadLocalBdAddrRspMsgs)/sizeof(CsrUint16)},  /* CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ */
    {csrBtJsr82CmReadLocalNameReqSender, cmReadLocalNameRspMsgs, sizeof(cmReadLocalNameRspMsgs)/sizeof(CsrUint16)},        /* CSR_BT_CM_READ_LOCAL_NAME_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_ENABLE_DUT_MODE_REQ */
    {csrBtJsr82CmWriteScanEnableReqSender, cmWriteScanEnableRspMsgs, sizeof(cmWriteScanEnableRspMsgs)/sizeof(CsrUint16)},     /* CSR_BT_CM_WRITE_SCAN_ENABLE_REQ */
    {csrBtJsr82CmReadScanEnableReqSender, cmReadScanEnableRspMsgs, sizeof(cmReadScanEnableRspMsgs)/sizeof(CsrUint16)},     /* CSR_BT_CM_READ_SCAN_ENABLE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_CONNECTABLE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_WRITE_PAGE_TO_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_TX_POWER_LEVEL_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_GET_LINK_QUALITY_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_RSSI_REQ */
    {csrBtJsr82CmWriteCodReqSender, cmWriteCodRspMsgs, sizeof(cmWriteCodRspMsgs)/sizeof(CsrUint16)},                       /* CSR_BT_CM_WRITE_COD_REQ */
    {csrBtJsr82CmReadCodReqSender, cmReadCodRspMsgs, sizeof(cmReadCodRspMsgs)/sizeof(CsrUint16)},                          /* CSR_BT_CM_READ_COD_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_REMOTE_EXT_FEATURES_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SET_AFH_CHANNEL_CLASS_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_AFH_CHANNEL_ASSESMENT_MODE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESMENT_MODE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_LOCAL_EXT_FEATURES_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_AFH_CHANNEL_MAP_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_CLOCK_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_LOCAL_VERSION_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SET_EVENT_FILTER_COD_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_CLEAR_EVENT_FILTER_REQ */
    {csrBtJsr82CmReadIacReqSender, cmReadIacRspMsgs, sizeof(cmReadIacRspMsgs)/sizeof(CsrUint16)},                          /* CSR_BT_CM_READ_IAC_REQ */
    {csrBtJsr82CmWriteIacReqSender, cmWriteIacRspMsgs, sizeof(cmWriteIacRspMsgs)/sizeof(CsrUint16)},                       /* CSR_BT_CM_WRITE_IAC_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ               */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_MAP_SCO_PCM_RES                          */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_WRITE_PAGESCAN_TYPE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DM_MODE_SETTINGS_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DM_CHECK_SSR_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_BONDING_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_READ_DEVICE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_REMOTE_FEATURES_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_ACCESS_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_ACL_DETACH_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_ALWAYS_MASTER_DEVICES_REQ */
};

static const jsr82PrimMapping jsr82CmSmPrimMapping[CSR_BT_CM_SM_PRIM_DOWNSTREAM_COUNT] =
{
    {csrBtJsr82CmRegisterReqSender, cmRegisterRspMsgs, sizeof(cmRegisterRspMsgs)/sizeof(CsrUint16)}, /* CSR_BT_CM_REGISTER_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_CONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_CONNECT_ACCEPT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DISCONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_RFC_MODE_CHANGE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_CONNECT_EXT_REQ */
    {csrBtJsr82CmL2caRegisterReqSender, cmL2caRegisterRspMsgs, sizeof(cmL2caRegisterRspMsgs)/sizeof(CsrUint16)}, /* CSR_BT_CM_L2CA_REGISTER_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_L2CA_CONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_L2CA_DISCONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_L2CA_MODE_CHANGE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_BNEP_REGISTER_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_BNEP_CONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_BNEP_CONNECT_ACCEPT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_BNEP_MODE_CHANGE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_BNEP_SWITCH_ROLE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_BNEP_DISCONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SDC_SEARCH_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SDC_SERVICE_SEARCH_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SDC_UUID128_SEARCH_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SDC_OPEN_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SDC_RFC_SEARCH_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SDC_RELEASE_RESOURCES_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SDS_REGISTER_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SDS_UNREGISTER_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_PRIVATE_SWITCH_ROLE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_WRITE_LINK_POLICY_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_LINK_POLICY_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SWITCH_ROLE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_MODE_CHANGE_REQ */
};

static const jsr82PrimMapping jsr82CmPrimMapping[CSR_BT_CM_PRIM_DOWNSTREAM_COUNT] =
{
    {NULL, NULL, 0},                                       /* CSR_BT_CM_INQUIRY_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_CANCEL_INQUIRY_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_L2CA_DATA_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_LINK_KEY_REQUEST_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_AUTHORISE_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_PIN_REQUEST_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_UNREGISTER_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DATA_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DATA_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_CONTROL_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_PORTNEG_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_BNEP_EXTENDED_DATA_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_BNEP_DISCONNECT_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SDC_CANCEL_SEARCH_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SDC_ATTRIBUTE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SDC_CLOSE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_CANCEL_CONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_CANCEL_CONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_BONDING_CANCEL_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_REPAIR_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SET_EVENT_MASK_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_MODE_CHANGE_CONFIG_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_L2CA_UNREGISTER_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_MOVE_CHANNEL_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_MOVE_CHANNEL_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_PORTNEG_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_READ_REMOTE_VERSION_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_AMP_REGISTER_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_L2CA_DATA_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_L2CA_DATA_ABORT_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_A2DP_BIT_RATE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_GET_SECURITY_CONF_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DATA_BUFFER_EMPTY_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_SM_HOUSE_CLEANING */
    {NULL, NULL, 0},                                       /* CSR_BT_CM_DM_HOUSE_CLEANING */
};

static const jsr82PrimMapping jsr82ScPrimMapping[CSR_BT_SC_PRIM_DOWNSTREAM_COUNT] =
{
    {NULL, NULL, 0},                                       /* CSR_BT_SC_REGISTER_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_BOND_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_DEBOND_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_PASSKEY_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_SET_SECURITY_MODE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_ENCRYPTION_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_AUTHORISE_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_DEREGISTER_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_ACTIVATE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_SET_TRUST_LEVEL_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_READ_DEVICE_RECORD_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_UPDATE_DEVICE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_CANCEL_BOND_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_AUTHENTICATE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_SET_IO_CAPABILITY_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_SSP_PASSKEY_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_SSP_NOTIFICATION_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_SSP_COMPARE_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_SSP_JUSTWORKS_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_MODE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_REBOND_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_SET_EVENT_MASK_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_SSP_PAIRING_RES */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_DEBUG_MODE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_CONFIG_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_DATABASE_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_HOUSE_CLEANING */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_ACCEPT_BOND_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ */
};

static const jsr82PrimMapping jsr82SdPrimMapping[CSR_BT_SD_PRIM_DOWNSTREAM_COUNT] =
{
    {NULL, NULL, 0},                                       /* CSR_BT_SD_MEMORY_CONFIG_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SD_SEARCH_CONFIG_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SD_READ_DEVICE_INFO_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SD_SEARCH_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SD_CANCEL_SEARCH_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SD_READ_DEVICE_LIST_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SD_READ_SERVICE_RECORD_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SD_CLEANUP_REQ */
    {NULL, NULL, 0},                                       /* CSR_BT_SD_SYNCHRONIZE_REQ */
};

static void dispatchMessage(Jsr82QElement *qElement, const jsr82PrimMapping *primMapping, CsrUint16 downLowest)
{
    primMapping[(CsrUint16) qElement->reqType - downLowest].msgSender(qElement->reqMsg);
    CsrPmemFree(qElement->reqMsg);
    qElement->reqMsg = NULL;
}

static Jsr82QElement *createQElement(CsrBtJsr82Prim *orgMsg,
                                     CsrUint16 eventClass,
                                     void *reqMsg,
                                     CsrUint16 reqType,
                                     const jsr82DynSignalHandler *signalHandlerList,
                                     CsrUint16 index)
{
    Jsr82QElement   *qElement;

    qElement = (Jsr82QElement *)CsrPmemAlloc(sizeof(Jsr82QElement));
    qElement->next = NULL;
    qElement->orgMsg = orgMsg;
    qElement->event = eventClass;
    qElement->reqMsg = reqMsg;
    qElement->reqType = reqType;
    qElement->signalHandler = signalHandlerList[index];
    return qElement;
}

static const jsr82PrimMapping *csrBtJsr82GetPrimMapping(CsrUint16 eventClass, CsrUint16 reqType)
{
    switch (eventClass)
    {
        case CSR_BT_SD_PRIM:
        {
            return jsr82SdPrimMapping;
        }
        case CSR_BT_SC_PRIM:
        {
            return jsr82ScPrimMapping;
        }
        case CSR_BT_CM_PRIM:
        {
            if (reqType <= CSR_BT_CM_DM_PRIM_DOWNSTREAM_HIGHEST)
            {
                return jsr82CmDmPrimMapping;
            }
            else if (reqType >= CSR_BT_CM_SM_PRIM_DOWNSTREAM_LOWEST && reqType <= CSR_BT_CM_SM_PRIM_DOWNSTREAM_HIGHEST)
            {
                return jsr82CmSmPrimMapping;
            }
            else if (reqType >= CSR_BT_CM_PRIM_DOWNSTREAM_LOWEST && reqType <= CSR_BT_CM_PRIM_DOWNSTREAM_HIGHEST)
            {
                return jsr82CmPrimMapping;
            }

            break;
        }
        default:
        {
            break;
        }
    }

    return NULL;
}

static CsrUint16 csrBtJsr82GetUpLowest(CsrUint16 eventClass, CsrUint16 type)
{
    switch (eventClass)
    {
        case CSR_BT_SD_PRIM:
        {
            return CSR_BT_SD_PRIM_UPSTREAM_LOWEST;
        }
        case CSR_BT_SC_PRIM:
        {
            return CSR_BT_SC_PRIM_UPSTREAM_LOWEST;
        }
        case CSR_BT_CM_PRIM:
        {
            if (type >= CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST && type <= CSR_BT_CM_RFC_PRIM_UPSTREAM_HIGHEST)
            {
                return CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST;
            }
            else if (type >= CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST && type <= CSR_BT_CM_SDC_PRIM_UPSTREAM_HIGHEST)
            {
                return CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST;
            }
            else if (type >= CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST && type <= CSR_BT_CM_L2CA_PRIM_UPSTREAM_HIGHEST)
            {
                return CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST;
            }
            else if (type >= CSR_BT_CM_BNEP_PRIM_UPSTREAM_LOWEST && type <= CSR_BT_CM_BNEP_PRIM_UPSTREAM_HIGHEST)
            {
                return CSR_BT_CM_BNEP_PRIM_UPSTREAM_LOWEST;
            }
            else if (type >= CSR_BT_CM_INQUIRY_PRIM_UPSTREAM_LOWEST && type <= CSR_BT_CM_INQUIRY_PRIM_UPSTREAM_HIGHEST)
            {
                return CSR_BT_CM_INQUIRY_PRIM_UPSTREAM_LOWEST;
            }
            else if (type >= CSR_BT_CM_DM_PRIM_UPSTREAM_LOWEST && type <= CSR_BT_CM_DM_PRIM_UPSTREAM_HIGHEST)
            {
                return CSR_BT_CM_DM_PRIM_UPSTREAM_LOWEST;
            }
            else if (type >= CSR_BT_CM_DM_1P2_PRIM_UPSTREAM_LOWEST && type <= CSR_BT_CM_DM_1P2_PRIM_UPSTREAM_HIGHEST)
            {
                return CSR_BT_CM_DM_1P2_PRIM_UPSTREAM_LOWEST;
            }
            else if (type >= CSR_BT_CM_SEND_PRIM_UPSTREAM_LOWEST && type <= CSR_BT_CM_SEND_PRIM_UPSTREAM_HIGHEST)
            {
                return CSR_BT_CM_SEND_PRIM_UPSTREAM_LOWEST;
            }
            else
            {
                return 0xFFFF; /* Need to return something! */
            }
        }
        default:
        {
            return 0xFFFF; /* Need to return something! */
        }
    }
}

static CsrUint16 csrBtJsr82GetDownLowest(CsrUint16 eventClass, CsrUint16 type)
{
    switch (eventClass)
    {
        case CSR_BT_SD_PRIM:
        {
            return CSR_BT_SD_PRIM_DOWNSTREAM_LOWEST;
        }
        case CSR_BT_SC_PRIM:
        {
            return CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST;
        }
        case CSR_BT_CM_PRIM:
        {
            if (type <= CSR_BT_CM_DM_PRIM_DOWNSTREAM_HIGHEST)
            {
                return CSR_BT_CM_DM_PRIM_DOWNSTREAM_LOWEST;
            }
            else if (type >= CSR_BT_CM_SM_PRIM_DOWNSTREAM_LOWEST && type <= CSR_BT_CM_SM_PRIM_DOWNSTREAM_HIGHEST)
            {
                return CSR_BT_CM_SM_PRIM_DOWNSTREAM_LOWEST;
            }
            else if (type >= CSR_BT_CM_PRIM_DOWNSTREAM_LOWEST && type <= CSR_BT_CM_PRIM_DOWNSTREAM_HIGHEST)
            {
                return CSR_BT_CM_PRIM_DOWNSTREAM_LOWEST;
            }
            else
            {
                return 0xFFFF; /* Need to return something! */
            }
        }
        default:
        {
            return 0xFFFF; /* Need to return something! */
        }
    }
}

static Jsr82QElement **csrBtJsr82GetDynTable(Jsr82MainInstanceData *instData,
                                             CsrUint16 eventClass,
                                             CsrUint16 prim)
{
    switch (eventClass)
    {
        case CSR_BT_SD_PRIM:
        {
            return (Jsr82QElement **) instData->sdDynJumpTable;
        }
        case CSR_BT_SC_PRIM:
        {
            return (Jsr82QElement **) instData->scDynJumpTable;
        }
        case CSR_BT_CM_PRIM:
        {
            if (prim >= CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST && prim <= CSR_BT_CM_RFC_PRIM_UPSTREAM_HIGHEST)
            {
                return (Jsr82QElement **) instData->cmRfcDynJumpTable;
            }
            else if (prim >= CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST && prim <= CSR_BT_CM_SDC_PRIM_UPSTREAM_HIGHEST)
            {
                return (Jsr82QElement **) instData->cmSdcDynJumpTable;
            }
            else if (prim >= CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST && prim <= CSR_BT_CM_L2CA_PRIM_UPSTREAM_HIGHEST)
            {
                return (Jsr82QElement **) instData->cmL2caDynJumpTable;
            }
            else if (prim >= CSR_BT_CM_BNEP_PRIM_UPSTREAM_LOWEST && prim <= CSR_BT_CM_BNEP_PRIM_UPSTREAM_HIGHEST)
            {
                return (Jsr82QElement **) instData->cmBnepDynJumpTable;
            }
            else if (prim >= CSR_BT_CM_INQUIRY_PRIM_UPSTREAM_LOWEST && prim <= CSR_BT_CM_INQUIRY_PRIM_UPSTREAM_HIGHEST)
            {
                return (Jsr82QElement **) instData->cmInquiryDynJumpTable;
            }
            else if (prim >= CSR_BT_CM_DM_PRIM_UPSTREAM_LOWEST && prim <= CSR_BT_CM_DM_PRIM_UPSTREAM_HIGHEST)
            {
                return (Jsr82QElement **) instData->cmDmDynJumpTable;
            }
            else if (prim >= CSR_BT_CM_DM_1P2_PRIM_UPSTREAM_LOWEST && prim <= CSR_BT_CM_DM_1P2_PRIM_UPSTREAM_HIGHEST)
            {
                return (Jsr82QElement **) instData->cmDm1P2DynJumpTable;
            }
            else if (prim >= CSR_BT_CM_SEND_PRIM_UPSTREAM_LOWEST && prim <= CSR_BT_CM_SEND_PRIM_UPSTREAM_HIGHEST)
            {
                return (Jsr82QElement **) instData->cmSendDynJumpTable;
            }
        }
    }

    return NULL;
}

static void queueOrSendMsg(Jsr82MainInstanceData *instData,
                           CsrBtJsr82Prim **orgMsg,
                           CsrUint16 eventClass,
                           void *reqMsg,
                           const jsr82DynSignalHandler *signalHandlerList)
{
    CsrUint16 i;
    Jsr82QElement *qElement;
    Jsr82QElement *qPtr;
    CsrUint16 *primList;
    CsrUint16 primListLength;
    CsrUint16 reqType;
    CsrUint16 type;
    Jsr82QElement **dynQueue;
    const jsr82PrimMapping *primMapping;

    type        = *((CsrUint16 *) reqMsg);
    reqType     = type - csrBtJsr82GetDownLowest(eventClass, type);
    primMapping = csrBtJsr82GetPrimMapping(eventClass, type);

    if (primMapping == NULL)
    {
        /* Signal out of bounds */
        return;
    }

    primList = primMapping[reqType].primList;
    primListLength = primMapping[reqType].primListLength;

    for (i = 0; i < primListLength; i++)
    {
        CsrUint16 primType;
        dynQueue = csrBtJsr82GetDynTable(instData, eventClass, primList[i]);
        primType = primList[i] - csrBtJsr82GetUpLowest(eventClass, primList[i]);
        qElement = createQElement(*orgMsg, eventClass, reqMsg, type, signalHandlerList, i);
        if (dynQueue[primType] == NULL)
        {
            /* Message is first, store first in queue */
            if (reqMsg)
            {
                dispatchMessage(qElement, primMapping, csrBtJsr82GetDownLowest(eventClass, type));
                reqMsg = NULL;
            }
            dynQueue[primType] = (void *)qElement;
        }
        else
        {
            /* Queue message at the end of the queue */
            for (qPtr = (Jsr82QElement *)dynQueue[primType]; qPtr && qPtr->next; qPtr = qPtr->next)
            {
                ;
            }
            if (qPtr)
            {
                qPtr->next = qElement;
            }
            else
            {
                CsrPmemFree(qElement);
            }
        }
    }
    *orgMsg = NULL;
}

void CsrBtJsr82SendCmMsgDown(Jsr82MainInstanceData *instData,
                             CsrBtJsr82Prim **orgMsg,
                             void *reqMsg,
                             const jsr82DynSignalHandler *signalHandlerList)
{
    queueOrSendMsg(instData, orgMsg, CSR_BT_CM_PRIM, reqMsg, signalHandlerList);
}

void CsrBtJsr82SendSdMsgDown(Jsr82MainInstanceData *instData,
                        CsrBtJsr82Prim **orgMsg,
                        void *reqMsg,
                        const jsr82DynSignalHandler *signalHandlerList)
{
    queueOrSendMsg(instData, orgMsg, CSR_BT_SD_PRIM, reqMsg, signalHandlerList);
}

void CsrBtJsr82SendScMsgDown(Jsr82MainInstanceData *instData,
                        CsrBtJsr82Prim **orgMsg,
                        void *reqMsg,
                        const jsr82DynSignalHandler *signalHandlerList)
{
    queueOrSendMsg(instData, orgMsg, CSR_BT_SC_PRIM, reqMsg, signalHandlerList);
}

static void csrBtJsr82DynQueueHandler(Jsr82MainInstanceData *instData,
                                      CsrUint16 eventClass)
{
    CsrUint16 reqType;
    CsrUint16 primType;
    Jsr82QElement **dynQueue;
    const jsr82PrimMapping *primMapping;

    reqType  = *((CsrUint16 *) instData->recvMsg);
    primType = reqType - csrBtJsr82GetUpLowest(eventClass, reqType);
    dynQueue = csrBtJsr82GetDynTable(instData, eventClass, reqType);

    if (dynQueue && dynQueue[primType] != NULL)
    {
        if (dynQueue[primType]->signalHandler(instData, &dynQueue[primType]->orgMsg))
        {
            /* This signal is done processing, now we must clean the queue */
            CsrUint16 i;
            CsrBool sentMsg = FALSE;
            CsrUint16 primListLength;
            CsrUint16 *primList;
            CsrUint16 downLowest = csrBtJsr82GetDownLowest(eventClass, dynQueue[primType]->reqType);

            primMapping    = csrBtJsr82GetPrimMapping(eventClass, dynQueue[primType]->reqType);
            primListLength = primMapping[dynQueue[primType]->reqType - downLowest].primListLength;
            primList       = primMapping[dynQueue[primType]->reqType - downLowest].primList;

            CsrPmemFree(dynQueue[primType]->orgMsg);
            dynQueue[primType]->orgMsg = NULL;
            for (i = 0; i < primListLength; i++)
            {
                CsrUint16 upPrimType;
                Jsr82QElement *nextElement;

                upPrimType = (CsrUint16) (primList[i] - csrBtJsr82GetUpLowest(eventClass, primList[i]));
                nextElement = dynQueue[upPrimType]->next;
                CsrPmemFree(dynQueue[upPrimType]);
                dynQueue[upPrimType] = nextElement;
                if (dynQueue[upPrimType] != NULL)
                {
                    if (!sentMsg)
                    {
                        dispatchMessage(dynQueue[upPrimType], primMapping, downLowest);
                        CsrPmemFree(dynQueue[upPrimType]->reqMsg);
                        sentMsg = TRUE;
                    }
                    dynQueue[upPrimType]->reqMsg = NULL;
                }
            }
        }
    }
    else
    {
        /* State event error */
    }
}

void CsrBtJsr82CmDynQueueHandler(Jsr82MainInstanceData *instData)
{
    csrBtJsr82DynQueueHandler(instData, CSR_BT_CM_PRIM);
}

void CsrBtJsr82SdDynQueueHandler(Jsr82MainInstanceData *instData)
{
    csrBtJsr82DynQueueHandler(instData, CSR_BT_SD_PRIM);
}

void CsrBtJsr82ScDynQueueHandler(Jsr82MainInstanceData *instData)
{
    csrBtJsr82DynQueueHandler(instData, CSR_BT_SC_PRIM);
}

/* subscribe signal handling    */
static CsrBool csrBtJsr82PropgateEvent(Jsr82MainInstanceData *instData, CsrBtResultCode resultCode, CsrBtJsr82EventMask event)
{
    if ((instData->subscribeInfo.eventMask & event) == event)
    {
        if (instData->subscribeInfo.conditionMask == CSR_BT_JSR82_EVENT_MASK_COND_ALL)
        {
            return (TRUE);
        }
    }
    return (FALSE);
}

static void csrBtJsr82RfcCloseIndMsgSend(CsrSchedQid         pHandle,
                                    CsrUint8    localServerChannel,
                                    CsrBtResultCode  result,
                                    CsrBool           localTerminated)
{
    CsrBtJsr82RfcCloseInd *prim      = (CsrBtJsr82RfcCloseInd *)CsrPmemAlloc(sizeof(CsrBtJsr82RfcCloseInd));
    prim->type                  = CSR_BT_JSR82_RFC_CLOSE_IND;
    prim->localServerChannel    = localServerChannel;
    prim->resultCode            = result;
    prim->localTerminated       = localTerminated;

    CsrBtJsr82MessagePut(pHandle, prim);
}

static void csrBtJsr82L2caCloseIndMsgSend(CsrSchedQid         pHandle,
                                     CsrBtConnId btConnId,
                                     CsrBtResultCode  result,
                                     CsrBool localTerminated)
{
    CsrBtJsr82L2caCloseInd *prim = (CsrBtJsr82L2caCloseInd *)CsrPmemAlloc(sizeof(CsrBtJsr82L2caCloseInd));
    prim->type              = CSR_BT_JSR82_L2CA_CLOSE_IND;
    prim->btConnId          = btConnId;
    prim->resultCode        = result;
    prim->localTerminated   = localTerminated;

    CsrBtJsr82MessagePut(pHandle, prim);
}

void CsrBtJsr82PropgateRfcDisconnectIndEvent(Jsr82MainInstanceData *instData,
    CsrUint8 serverChannel, CsrBtResultCode resultCode,
    CsrBool localTerminated)
{
    if (csrBtJsr82PropgateEvent(instData, resultCode, CSR_BT_JSR82_EVENT_MASK_SUBSCRIBE_RFC_CLOSE_IND))
    {
        csrBtJsr82RfcCloseIndMsgSend(instData->subscribeInfo.appHandle,
            serverChannel, resultCode, localTerminated);
    }
    else
    { /* The application has not subscribed for this event, ignore it    */
        ;
    }
}

void CsrBtJsr82PropgateL2caDisconnectEvent(Jsr82MainInstanceData *instData,
    CsrBtConnId btConnId, CsrBtResultCode resultCode,
    CsrBool localTerminated)
{
    if (csrBtJsr82PropgateEvent(instData, resultCode, CSR_BT_JSR82_EVENT_MASK_SUBSCRIBE_L2CA_CLOSE_IND))
    {
        csrBtJsr82L2caCloseIndMsgSend(instData->subscribeInfo.appHandle,
            btConnId, resultCode, localTerminated);
    }
    else
    { /* The application has not subscribed for this event, ignore it    */
        ;
    }
}

void CsrBtJsr82SetEventMaskReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82SetEventMaskReq *prim              = (CsrBtJsr82SetEventMaskReq *)instData->recvMsg;
    CsrBtJsr82SetEventMaskCfm *cfmPrim           = (CsrBtJsr82SetEventMaskCfm *) CsrPmemAlloc(sizeof(CsrBtJsr82SetEventMaskCfm));
    instData->subscribeInfo.appHandle       = prim->phandle;
    instData->subscribeInfo.eventMask       = (prim->eventMask & JSR82_EVENT_MASK_RESERVER_VALUES_MASK);
    instData->subscribeInfo.conditionMask   = prim->conditionMask;
    cfmPrim->type                           = CSR_BT_JSR82_SET_EVENT_MASK_CFM;
    cfmPrim->eventMask                      = instData->subscribeInfo.eventMask;
    CsrBtJsr82MessagePut(prim->phandle, cfmPrim);
}

/* clean up */
void CsrBtJsr82FreeMessage(Jsr82MainInstanceData *instData)
{
    switch(instData->eventClass)
    {
        case CSR_BT_JSR82_PRIM:
        {
            CsrBtJsr82FreeDownstreamMessageContents(instData->eventClass,
                                               instData->recvMsg);
            break;
        }
    case CSR_BT_CM_PRIM:
        {
            if((*(CsrUint16*)instData->recvMsg == CSR_BT_CM_L2CA_DATA_IND) &&
               !instData->deinit)
            {
                /* Send data response if we're not in deinit */
                CsrBtCmL2caDataInd *prim;
                prim = (CsrBtCmL2caDataInd*)instData->recvMsg;
                CsrBtCmL2caDataResSend(prim->btConnId);
            }

            CsrBtCmFreeUpstreamMessageContents(instData->eventClass,
                                          instData->recvMsg);
            break;
        }
    case CSR_BT_SD_PRIM:
        {
            CsrBtSdFreeUpstreamMessageContents(instData->eventClass,
                                          instData->recvMsg);
            CsrBtSdPrivateFreeUpstreamMessageContents(instData->eventClass,
                                          instData->recvMsg);

            break;
        }
    default:
        {
            break;
        }
    } /* end switch(msg_type) */
    CsrPmemFree(instData->recvMsg);
    instData->recvMsg = NULL;
}


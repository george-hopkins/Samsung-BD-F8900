/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_jsr82_prim.h"
#include "csr_bt_jsr82_main.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_pmem.h"
#include "csr_bt_usr_config.h"
#include "csr_bt_jsr82_maintenance_handler.h"
#include "csr_bt_util.h"
#include "csr_bt_jsr82_local_device.h"

void CsrBtJsr82LdGetBluetoothAddress(Jsr82MainInstanceData *instData)
{
    const jsr82DynSignalHandler cfmHandler[] = {CsrBtJsr82CmGetBdAddrCfmHandler};
    CsrBtJsr82SendCmMsgDown(instData, (CsrBtJsr82Prim **)&instData->recvMsg, CsrBtCmReadLocalBdAddrReq_struct(CSR_BT_JSR82_IFACEQUEUE), cfmHandler);
}

CsrBool CsrBtJsr82CmGetBdAddrCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg)
{
    CsrBtCmReadLocalBdAddrCfm *prim;
    CsrBtJsr82LdGetBluetoothAddressCfm *msg;
    CsrBtJsr82LdGetBluetoothAddressReq *orgPrim;
    orgPrim = (CsrBtJsr82LdGetBluetoothAddressReq *) *orgMsg;

    prim = (CsrBtCmReadLocalBdAddrCfm *)instData->recvMsg;
    msg = CsrPmemAlloc(sizeof(CsrBtJsr82LdGetBluetoothAddressCfm));
    msg->type = (CsrBtJsr82Prim)CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_CFM;
    msg->reqID = orgPrim->reqID;
    msg->deviceAddr = prim->deviceAddr;
    CsrBtJsr82MessagePut(orgPrim->appHandle, msg);
    return TRUE;
}

void CsrBtJsr82LdGetFriendlyName(Jsr82MainInstanceData *instData)
{
    const jsr82DynSignalHandler cfmHandler[] = {CsrBtJsr82CmReadLocalNameCfmHandler};
    CsrBtJsr82SendCmMsgDown(instData, (CsrBtJsr82Prim **)&instData->recvMsg, CsrBtCmReadLocalNameReq_struct(CSR_BT_JSR82_IFACEQUEUE), cfmHandler);
}

CsrBool CsrBtJsr82CmReadLocalNameCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg)
{
    CsrBtJsr82LdGetFriendlyNameCfm *msg;
    CsrBtJsr82LdGetFriendlyNameReq *orgPrim;
    CsrBtCmReadLocalNameCfm *prim;

    orgPrim = (CsrBtJsr82LdGetFriendlyNameReq *) *orgMsg;

    prim = (CsrBtCmReadLocalNameCfm *)instData->recvMsg;
    msg = (CsrBtJsr82LdGetFriendlyNameCfm *)CsrPmemAlloc(sizeof(CsrBtJsr82LdGetFriendlyNameCfm));
    msg->type = (CsrBtJsr82Prim)CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM;
    msg->reqID = orgPrim->reqID;

    msg->localName = prim->localName;
    prim->localName = NULL;

    CsrBtJsr82MessagePut(orgPrim->appHandle, msg);
    return TRUE;
}

static void sendJsr82L2GetDiscoverableCfm(CsrSchedQid appHandle, CsrUint32 reqId, CsrUint24 mode)
{
    CsrBtJsr82LdGetDiscoverableCfm *msg;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82LdGetDiscoverableCfm));
    msg->type = (CsrBtJsr82Prim)CSR_BT_JSR82_LD_GET_DISCOVERABLE_CFM;
    msg->reqID = reqId;
    msg->mode = mode;
    CsrBtJsr82MessagePut(appHandle, msg);
}

void CsrBtJsr82LdGetDiscoverableReqHandler(Jsr82MainInstanceData *instData)
{
    const jsr82DynSignalHandler cfmHandler[] = {CsrBtJsr82CmReadScanEnableCfmHandler};

    CsrBtCmReadScanEnableReq *msg = (CsrBtCmReadScanEnableReq*) CsrPmemAlloc(sizeof(CsrBtCmReadScanEnableReq));
    msg->type = CSR_BT_CM_READ_SCAN_ENABLE_REQ;
    msg->appHandle = CSR_BT_JSR82_IFACEQUEUE;

    CsrBtJsr82SendCmMsgDown(instData, (CsrBtJsr82Prim **)&instData->recvMsg, msg, cfmHandler);
}

CsrBool CsrBtJsr82CmReadScanEnableCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg)
{
    CsrBtCmReadScanEnableCfm *prim;
    CsrBtJsr82LdGetDiscoverableReq *orgPrim;

    orgPrim = (CsrBtJsr82LdGetDiscoverableReq *) *orgMsg;
    prim = (CsrBtCmReadScanEnableCfm *) instData->recvMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        if ((prim->scanEnable == HCI_SCAN_ENABLE_INQ) || (prim->scanEnable == HCI_SCAN_ENABLE_INQ_AND_PAGE))
        {
            const jsr82DynSignalHandler cfmHandler[] = {CsrBtJsr82CmReadIacCfmHandler};
            CsrBtJsr82SendCmMsgDown(instData, orgMsg, CsrBtCmReadIacReq_struct(CSR_BT_JSR82_IFACEQUEUE), cfmHandler);
        }
        else if ((prim->scanEnable == HCI_SCAN_ENABLE_OFF) || (prim->scanEnable == HCI_SCAN_ENABLE_PAGE))
        {
            sendJsr82L2GetDiscoverableCfm(orgPrim->appHandle, orgPrim->reqID, CSR_BT_DISCOVERY_AGENT_NOT_DISCOVERABLE);
        }
        else
        {
            sendJsr82L2GetDiscoverableCfm(orgPrim->appHandle, orgPrim->reqID, CSR_BT_DISCOVERY_AGENT_ERROR_CODE);
        }
    }
    else
    {
        sendJsr82L2GetDiscoverableCfm(orgPrim->appHandle, orgPrim->reqID, CSR_BT_DISCOVERY_AGENT_ERROR_CODE);
    }
    return TRUE;
}

CsrBool CsrBtJsr82CmReadIacCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg)
{
    CsrBtCmReadIacCfm *prim;
    CsrBtJsr82LdGetDiscoverableReq *orgPrim;

    prim = (CsrBtCmReadIacCfm *)instData->recvMsg;
    orgPrim = (CsrBtJsr82LdGetDiscoverableReq *) *orgMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        sendJsr82L2GetDiscoverableCfm(orgPrim->appHandle, orgPrim->reqID, prim->iac);
    }
    else
    {
        sendJsr82L2GetDiscoverableCfm(orgPrim->appHandle, orgPrim->reqID, CSR_BT_DISCOVERY_AGENT_ERROR_CODE);
    }
    return TRUE;
}

static void sendCsrBtJsr82LdSetDiscoverableCfm(CsrSchedQid appHandle,
    CsrUint32 reqId, CsrBtResultCode resultCode)
{
    CsrBtJsr82LdSetDiscoverableCfm *msg;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82LdSetDiscoverableCfm));
    msg->type=(CsrBtJsr82Prim)CSR_BT_JSR82_LD_SET_DISCOVERABLE_CFM;
    msg->reqID = reqId;
    msg->resultCode = resultCode;

    CsrBtJsr82MessagePut(appHandle, msg);
}

CsrBool CsrBtJsr82CmWriteScanEnableCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg)
{
    CsrBtCmWriteScanEnableCfm *prim;
    CsrBtJsr82LdSetDiscoverableReq *orgPrim;
    CsrUint24 modeNumber;

    prim = (CsrBtCmWriteScanEnableCfm*)instData->recvMsg;
    orgPrim = (CsrBtJsr82LdSetDiscoverableReq *) *orgMsg;
    modeNumber = orgPrim->mode;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        if ((CSR_BT_DISCOVERY_AGENT_DISC_INTERVAL_START <= modeNumber) && (modeNumber <= CSR_BT_DISCOVERY_AGENT_DISC_INTERVAL_END))
        {
            const jsr82DynSignalHandler cfmHandler[] = {CsrBtJsr82CmWriteIacIndHandler};
            CsrBtJsr82SendCmMsgDown(instData, orgMsg, CsrBtCmWriteIacReq_struct(CSR_BT_JSR82_IFACEQUEUE, modeNumber), cfmHandler);
        }
        else
        {
            sendCsrBtJsr82LdSetDiscoverableCfm(orgPrim->appHandle,
                orgPrim->reqID, CSR_BT_RESULT_CODE_JSR82_SUCCESS);
        }
    }
    else
    {
        sendCsrBtJsr82LdSetDiscoverableCfm(orgPrim->appHandle, orgPrim->reqID,
            CSR_BT_RESULT_CODE_JSR82_COMMAND_DISALLOWED);
    }
    return TRUE;
}

void CsrBtJsr82LdSetDiscoverableReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82LdSetDiscoverableReq *prim;
    CsrUint24 modeNumber;

    const jsr82DynSignalHandler cfmHandler[] = {CsrBtJsr82CmWriteScanEnableCfmHandler};
    prim = (CsrBtJsr82LdSetDiscoverableReq *) instData->recvMsg;
    modeNumber = prim->mode;

    if ((CSR_BT_DISCOVERY_AGENT_DISC_INTERVAL_START <= modeNumber) && (modeNumber <= CSR_BT_DISCOVERY_AGENT_DISC_INTERVAL_END))
    {
        CsrBtCmWriteScanEnableReq *msg = (CsrBtCmWriteScanEnableReq*) CsrPmemAlloc(sizeof(CsrBtCmWriteScanEnableReq));
        msg->type = CSR_BT_CM_WRITE_SCAN_ENABLE_REQ;
        msg->appHandle = CSR_BT_JSR82_IFACEQUEUE;
        msg->disableInquiryScan = FALSE;
        msg->disablePageScan = FALSE;

        CsrBtJsr82SendCmMsgDown(instData,
                           (CsrBtJsr82Prim **)&instData->recvMsg,
                           msg,
                           cfmHandler);
    }
    else if(modeNumber == CSR_BT_DISCOVERY_AGENT_NOT_DISCOVERABLE)
    {
        CsrBtCmWriteScanEnableReq *msg = (CsrBtCmWriteScanEnableReq*) CsrPmemAlloc(sizeof(CsrBtCmWriteScanEnableReq));
        msg->type = CSR_BT_CM_WRITE_SCAN_ENABLE_REQ;
        msg->appHandle = CSR_BT_JSR82_IFACEQUEUE;
        msg->disableInquiryScan = TRUE;
        msg->disablePageScan = FALSE;

        CsrBtJsr82SendCmMsgDown(instData,
                           (CsrBtJsr82Prim **)&instData->recvMsg,
                           msg,
                           cfmHandler);
    }
    else /* modeNumber is out of legal range */
    {
        sendCsrBtJsr82LdSetDiscoverableCfm(prim->appHandle, prim->reqID,
            CSR_BT_RESULT_CODE_JSR82_COMMAND_DISALLOWED);
    }
}

CsrBool CsrBtJsr82CmWriteIacIndHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg)
{
    CsrBtCmWriteIacInd *prim;
    CsrBtJsr82LdSetDiscoverableReq *orgPrim;

    prim = (CsrBtCmWriteIacInd *)instData->recvMsg;
    orgPrim = (CsrBtJsr82LdSetDiscoverableReq *) *orgMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        if (prim->iac == orgPrim->mode)
        {
            sendCsrBtJsr82LdSetDiscoverableCfm(orgPrim->appHandle,
                orgPrim->reqID, CSR_BT_RESULT_CODE_JSR82_SUCCESS);
        }
    }
    else
    {
        sendCsrBtJsr82LdSetDiscoverableCfm(orgPrim->appHandle, orgPrim->reqID,
            CSR_BT_RESULT_CODE_JSR82_COMMAND_DISALLOWED);
    }
    return TRUE;
}

void CsrBtJsr82LdGetProperty(Jsr82MainInstanceData *instData)
{

    /* Property values to be reported back to caller  */
    static const CsrUint16 propertyValues[CSR_BT_JSR82_LD_NUMBER_OF_PROPERTIES] =
    {
        0x00,  /* Illegal property  */
        0x0101, /* Report API version 1.1  */
        0x0, /* Switch allowed is false  */
        0xFFFF, /* Allow 0xFFFF concurrent fetches of service attributes - handled one at a time in the Jsr82 task  */
        MAX_ACL_LINKS, /* see define in csr_bt_jsr82_main.h */
        CSR_BT_JSR82_L2CAP_PROFILE_DEFAULT_MTU_SIZE,
        0x1,  /* One concurrent SD transaction */
        0x1,  /* Inquiry scanning during connection is allowed */
        0x1,  /* Page scanning is allowed during connection */
        0x1,  /* Inquiry is allowed during a connection (but may fail!) */
        0x1   /* Paging is allowed during a connection */
    };

    CsrBtJsr82LdGetPropertyCfm *msg;
    CsrBtJsr82LdGetPropertyReq *prim;
    prim = (CsrBtJsr82LdGetPropertyReq *)instData->recvMsg;
    msg = CsrPmemAlloc(sizeof(CsrBtJsr82LdGetPropertyCfm));
    msg->type = (CsrBtJsr82Prim)CSR_BT_JSR82_LD_GET_PROPERTY_CFM;
    msg->reqID = prim->reqID;

    if(prim->propertyName < CSR_BT_JSR82_LD_NUMBER_OF_PROPERTIES)
    {
        msg->propertyName = prim->propertyName;
        msg->propertyValue=propertyValues[prim->propertyName];
    }
    else
    {
        msg->propertyName = CSR_BT_JSR82_ILLEGAL_PROPERTY_NAME;
        msg->propertyValue=propertyValues[CSR_BT_JSR82_ILLEGAL_PROPERTY_NAME];

    }
    CsrBtJsr82MessagePut(prim->appHandle, msg);
}

void CsrBtJsr82LdGetDeviceClassReqHandler(Jsr82MainInstanceData *instData)
{
    const jsr82DynSignalHandler cfmHandler[] = {CsrBtJsr82CmReadCodCfmHandler};

    CsrBtCmReadCodReq *msg = (CsrBtCmReadCodReq*) CsrPmemAlloc(sizeof(CsrBtCmReadCodReq));
    msg->type = CSR_BT_CM_READ_COD_REQ;
    msg->appHandle = CSR_BT_JSR82_IFACEQUEUE;

    CsrBtJsr82SendCmMsgDown(instData, (CsrBtJsr82Prim **)&instData->recvMsg, msg, cfmHandler);
}

#define COD_MAJOR_BIT_MASK              0x00001F00
#define COD_MINOR_BIT_MASK              0x000000FC
#define COD_SERVICE_CLASSES_BIT_MASK    0x00FFE000

CsrBool CsrBtJsr82CmReadCodCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg)
{
    CsrBtCmReadCodCfm           *prim;
    CsrBtJsr82LdGetDeviceClassCfm    *msg;
    CsrUint32                    codMask = 0;
    CsrBtJsr82LdGetDeviceClassReq    *orgPrim;

    prim = (CsrBtCmReadCodCfm *)instData->recvMsg;
    orgPrim = (CsrBtJsr82LdGetDeviceClassReq *) *orgMsg;
    if (orgPrim->fieldsMask & CSR_BT_JSR82_COD_MAJOR)
    {
        codMask |= COD_MAJOR_BIT_MASK;
    }
    if (orgPrim->fieldsMask & CSR_BT_JSR82_COD_MINOR)
    {
        codMask |= COD_MINOR_BIT_MASK;
    }
    if (orgPrim->fieldsMask & CSR_BT_JSR82_COD_SERVICES)
    {
        codMask |= COD_SERVICE_CLASSES_BIT_MASK;
    }

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82LdGetDeviceClassCfm));
    msg->type = CSR_BT_JSR82_LD_GET_DEVICE_CLASS_CFM;
    msg->reqID = orgPrim->reqID;
    msg->deviceClass = prim->classOfDevice & codMask;

    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        msg->resultCode     = CSR_BT_RESULT_CODE_JSR82_SUCCESS;
    }
    else
    {
        msg->resultCode     = CSR_BT_RESULT_CODE_JSR82_UNSPECIFIED_ERROR;
    }

    CsrBtJsr82MessagePut(orgPrim->appHandle, msg);

    return TRUE;
}

void CsrBtJsr82LdSetDeviceClassReqHandler(Jsr82MainInstanceData *instData)
{
    const jsr82DynSignalHandler cfmHandler[] = {CsrBtJsr82SetCodCmReadCodCfmHandler};

    CsrBtCmReadCodReq *msg = (CsrBtCmReadCodReq*) CsrPmemAlloc(sizeof(CsrBtCmReadCodReq));
    msg->type = CSR_BT_CM_READ_COD_REQ;
    msg->appHandle = CSR_BT_JSR82_IFACEQUEUE;

    CsrBtJsr82SendCmMsgDown(instData, (CsrBtJsr82Prim **)&instData->recvMsg, msg, cfmHandler);
}

static void sendCsrBtJsr82LdSetDeviceClassCfm(CsrSchedQid appHandle, CsrUint32 reqID, CsrBtResultCode resultCode)
{
    CsrBtJsr82LdSetDeviceClassCfm    *msg;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82LdSetDeviceClassCfm));
    msg->type = CSR_BT_JSR82_LD_SET_DEVICE_CLASS_CFM;
    msg->reqID = reqID;
    msg->resultCode = resultCode;

    CsrBtJsr82MessagePut(appHandle, msg);
}

CsrBool CsrBtJsr82SetCodCmReadCodCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg)
{
    CsrBtCmReadCodCfm   *prim;
    CsrBtJsr82LdSetDeviceClassReq    *orgPrim;

    prim = (CsrBtCmReadCodCfm *)instData->recvMsg;
    orgPrim = (CsrBtJsr82LdSetDeviceClassReq *) *orgMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        device_class_t  serviceClasses;
        device_class_t  nativeBits;
        device_class_t  newDeviceClass;
        const jsr82DynSignalHandler cfmHandler[] = {CsrBtJsr82CmWriteCodCfmHandler};

        serviceClasses  = (device_class_t)(prim->classOfDevice & COD_SERVICE_CLASSES_BIT_MASK);
        nativeBits      = (device_class_t)(serviceClasses & (~instData->javaDeviceClassBits));
        newDeviceClass  = (device_class_t)((orgPrim->deviceClass & COD_SERVICE_CLASSES_BIT_MASK) | nativeBits);
        CsrBtCmWriteServiceCodReqSend(CSR_BT_JSR82_IFACEQUEUE, newDeviceClass);
        CsrBtJsr82SendCmMsgDown(instData, orgMsg, CsrBtCmWriteCodReq_struct(CSR_BT_JSR82_IFACEQUEUE, CSR_BT_CM_WRITE_COD_UPDATE_FLAG_SERVICE_CLASS, newDeviceClass, 0, 0), cfmHandler);
        *orgMsg = NULL;
    }
    else
    {
        sendCsrBtJsr82LdSetDeviceClassCfm(orgPrim->appHandle, orgPrim->reqID,
            CSR_BT_RESULT_CODE_JSR82_UNSPECIFIED_ERROR);
    }
    return TRUE;
}

CsrBool CsrBtJsr82CmWriteCodCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg)
{
    CsrBtCmWriteCodCfm  *prim;
    CsrBtJsr82LdSetDeviceClassReq    *orgPrim;

    prim = (CsrBtCmWriteCodCfm *)instData->recvMsg;
    orgPrim = (CsrBtJsr82LdSetDeviceClassReq *) *orgMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        instData->javaDeviceClassBits = (orgPrim->deviceClass & COD_SERVICE_CLASSES_BIT_MASK);
        sendCsrBtJsr82LdSetDeviceClassCfm(orgPrim->appHandle, orgPrim->reqID,
            CSR_BT_RESULT_CODE_JSR82_SUCCESS);
    }
    else
    {
        sendCsrBtJsr82LdSetDeviceClassCfm(orgPrim->appHandle, orgPrim->reqID,
            CSR_BT_RESULT_CODE_JSR82_UNSPECIFIED_ERROR);
    }
    return TRUE;
}

void CsrBtJsr82LdGetSecurityLevelReqHandler(Jsr82MainInstanceData * instData)
{
    CsrBtJsr82LdGetSecurityLevelReq *prim;
    CsrBtJsr82LdGetSecurityLevelCfm *msg;

    prim = (CsrBtJsr82LdGetSecurityLevelReq *)instData->recvMsg;
    msg = (CsrBtJsr82LdGetSecurityLevelCfm *)CsrPmemAlloc(sizeof(CsrBtJsr82LdGetSecurityLevelCfm));
    msg->type = CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_CFM;
    msg->reqID = prim->reqID;
    msg->secLevel = CSR_BT_DEFAULT_SEC_MODE;
    CsrBtJsr82MessagePut(prim->appHandle,msg);
}

void CsrBtJsr82LdIsMasterReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82LdIsMasterReq  *prim;
    const jsr82DynSignalHandler cfmHandler[] = {CsrBtJsr82CmRoleDiscoveryCfmHandler};
    CsrBtCmRoleDiscoveryReq *msg = (CsrBtCmRoleDiscoveryReq*) CsrPmemAlloc(sizeof(CsrBtCmRoleDiscoveryReq));

    prim = (CsrBtJsr82LdIsMasterReq *)instData->recvMsg;

    msg->type = CSR_BT_CM_ROLE_DISCOVERY_REQ;
    msg->phandle = CSR_BT_JSR82_IFACEQUEUE;
    msg->deviceAddr = prim->deviceAddr;

    CsrBtJsr82SendCmMsgDown(instData, (CsrBtJsr82Prim **)&instData->recvMsg, msg, cfmHandler);
}

CsrBool CsrBtJsr82CmRoleDiscoveryCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg)
{
    CsrBtJsr82LdIsMasterCfm *msg;
    CsrBtCmRoleDiscoveryCfm *prim;
    CsrBtJsr82LdIsMasterReq    *orgPrim;

    prim = (CsrBtCmRoleDiscoveryCfm *)instData->recvMsg;
    orgPrim = (CsrBtJsr82LdIsMasterReq *) *orgMsg;
    msg = CsrPmemAlloc(sizeof(CsrBtJsr82LdIsMasterCfm));
    msg->type = CSR_BT_JSR82_LD_IS_MASTER_CFM;
    msg->reqID = orgPrim->reqID;
    msg->role = prim->role;
    CsrBtJsr82MessagePut(orgPrim->appHandle, msg);
    return TRUE;
}

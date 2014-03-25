/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE

#include "csr_bt_avrcp_main.h"
#include "csr_bt_avrcp_lib.h"

/* Macros */
#define AVRCP_INSERT_UINT16(dest_addr, uint)     *(dest_addr) = (CsrUint8)((uint)>>8); \
                                                 *((dest_addr) + 1) = (CsrUint8)((uint) & 0x00FF)

/* Service record sizes and indexes */
#define AVRCP_SDP_AVCTP_VERSION_INDEX           (27)
#define AVRCP_SDP_AVRCP_UUID_INDEX              (6)
#define AVRCP_SDP_AVRCP_VERSION_INDEX           (40)
#define AVRCP_SDP_FEATURES_INDEX                (46)
#define AVRCP_SDP_MINIMUM_RECORD_LENGTH         (56)
#define AVRCP_SDP_OPTIONAL_PREFIX_SIZE          (5)
#define AVRCP_SDP_OPTIONAL_PREFIX_SIZE_OFFSET   (4)
#define AVRCP_SDP_CT_EXTRA_OFFSET               (3)

/* Incomplete AVRCP record */
#define AVRCP_SDP_HEADER_SIZE                   (29)
static const CsrUint8 avrcpServiceRecordHeader[] = /* AVRCP_SDP_HEADER_SIZE */
{
    /* Service class ID list */
    0x09, 0x00, 0x01,      /* AttrId = ServiceClassIdList */
    0x35, 0x03,            /* Data element seq. 3 bytes */
    0x19, 0x00, 0x00,      /* UUID(2 bytes) */                              /* To be modified */

    /* Protocol descriptor list */
    0x09, 0x00, 0x04,      /* AttrId = ProtocolDescriptorList */
    0x35, 0x10,            /* Data element seq. 16 bytes */
    /* L2CAP */
    0x35, 0x06,            /* Data element seq. 6 bytes */
    0x19, 0x01, 0x00,      /* UUID(2 bytes), Protocol = L2CAP */
    0x09, 0x00, CSR_BT_AVCTP_PSM, /* PSM value = AVCTP = 0x0017 */
    /* AVCTP */
    0x35, 0x06,            /* Data element seq. 6 bytes */
    0x19, 0x00, 0x17,      /* UUID(2 bytes), Protocol = AVCTP */
    0x09, 0x00, 0x00       /* AVCTP Version */                              /* To be modified */
};

#ifdef CSR_BT_INSTALL_AVRCP_13_14
#define AVRCP_SDP_CT_HEADER_SIZE                   (32)
static const CsrUint8 avrcpCtServiceRecordHeader[] = /* AVRCP_SDP_CT_HEADER_SIZE */
{
    /* Service class ID list */
    0x09, 0x00, 0x01,      /* AttrId = ServiceClassIdList */
    0x35, 0x06,            /* Data element seq. 6 bytes */
    0x19, 0x00, 0x00,      /* UUID(2 bytes) */                              /* To be modified */
    0x19, 0x00, 0x00,      /* UUID(2 bytes) */                              /* To be modified */

    /* Protocol descriptor list */
    0x09, 0x00, 0x04,      /* AttrId = ProtocolDescriptorList */
    0x35, 0x10,            /* Data element seq. 16 bytes */
    /* L2CAP */
    0x35, 0x06,            /* Data element seq. 6 bytes */
    0x19, 0x01, 0x00,      /* UUID(2 bytes), Protocol = L2CAP */
    0x09, 0x00, CSR_BT_AVCTP_PSM, /* PSM value = AVCTP = 0x0017 */
    /* AVCTP */
    0x35, 0x06,            /* Data element seq. 6 bytes */
    0x19, 0x00, 0x17,      /* UUID(2 bytes), Protocol = AVCTP */
    0x09, 0x00, 0x00       /* AVCTP Version */                              /* To be modified */
};

#define AVRCP_SDP_BROWSING_SIZE                 (23)
static const CsrUint8 avrcpServiceRecordBrowsing[] = /* AVRCP_SDP_BROWSING_SIZE */
{
    /* Additional protocol descriptor list */
    0x09, 0x00, 0x0D,      /* AttrId = AdditionalProtocolDescriptorList */
    0x35, 0x12,            /* Data element seq. 21 bytes */

    /* Protocol descriptor list */
    0x35, 0x10,            /* Data element seq. 16 bytes */
    /* L2CAP */
    0x35, 0x06,            /* Data element seq. 6 bytes */
    0x19, 0x01, 0x00,      /* UUID(2 bytes), Protocol = L2CAP */
    0x09, 0x00, CSR_BT_AVCTP_BROWSING_PSM,/* PSM value = AVCTP_BROWSING = 0x001B */
    /* AVCTP */
    0x35, 0x06,            /* Data element seq. 6 bytes */
    0x19, 0x00, 0x17,      /* UUID(2 bytes), Protocol = AVCTP */
    0x09, 0x01, 0x03,      /* AVCTP Version */
};
#endif
#define AVRCP_SDP_FOOTER_SIZE                   (27)
static const CsrUint8 avrcpServiceRecordFooter[] = /* AVRCP_SDP_FOOTER_SIZE */
{
    /* Bluetooth profile descriptor list */
    0x09, 0x00, 0x09,      /* AttrId = BluetoothProfileDescriptorList */
    0x35, 0x08,            /* Data element seq. 8 bytes */
    0x35, 0x06,            /* Data element seq. 6 bytes */
    0x19, 0x11, 0x0E,      /* UUID(2 bytes), Profile = A/V remote Control */
    0x09, 0x00, 0x00,      /* AVRCP Version */                              /* To be modified */

    /* Supported features */
    0x09, 0x03, 0x11,      /* AttrId = Supported features */
    0x09, 0x00, 0x00,      /* Supported features */                         /* To be modified */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,    /* AttrId = BrowseGroupList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,    /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};


static CsrBool csrBtAvrcpSdpGetBluetoothProfileDescriptorList(CmnCsrBtLinkedListStruct *bll_p,
                                                        CsrUint16 serviceHandleIndex,
                                                        CsrUint16 *version)
{
    CsrBool    retBool = FALSE;
    CsrUint8  *att_p;
    CsrUint16  attDataLen, nofBytesToAttribute, emptyAttSize, consumedBytes, totalConsumedBytes = 0, tempVar;
    CsrUintFast16 nofAttributes, x;
    CsrUint32  returnValue, protocolValue;

    if (TRUE == CsrBtUtilSdrGetNofAttributes(bll_p, serviceHandleIndex, &nofAttributes))
    {
        for (x=0; x<nofAttributes; x++)
        {
            att_p = CsrBtUtilSdrGetAttributePointer(bll_p, serviceHandleIndex, x, &nofBytesToAttribute);
            /* Check if the UUID in the 'outer' attribute struct is correct */
            CsrMemCpy(&tempVar, att_p + SDR_ENTRY_INDEX_ATTRIBUTE_UUID, SDR_ENTRY_SIZE_SERVICE_UINT16);

            if (CSR_BT_BLUETOOTH_PROFILE_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER == tempVar)
            {
                CsrBtUtilSdrGetEmptyAttributeSize(&emptyAttSize);
                CsrMemCpy(&tempVar, att_p, SDR_ENTRY_SIZE_SERVICE_UINT16);
                attDataLen = tempVar - emptyAttSize + SDR_ENTRY_SIZE_TAG_LENGTH;

                /* First extract the attribute uuid from the attribute SDP data */
                if (TRUE == CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA,
                                              attDataLen,
                                              &returnValue,
                                              &consumedBytes,
                                              FALSE))
                {
                    /* Check if the UUID in the 'inner' attribute sdp data struct is correct */
                    if (CSR_BT_BLUETOOTH_PROFILE_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER == returnValue)
                    {
                        attDataLen = attDataLen - consumedBytes;
                        totalConsumedBytes += consumedBytes;
                        /* first find the protocol UUID */
                        if (TRUE == CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes,
                                                      attDataLen,
                                                      &protocolValue,
                                                      &consumedBytes,
                                                      TRUE))
                        {
                            attDataLen = attDataLen - consumedBytes;
                            totalConsumedBytes += consumedBytes;
                            /* Now find the value */
                            if (TRUE == CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes,
                                                          attDataLen,
                                                          &returnValue,
                                                          &consumedBytes,
                                                          TRUE))
                            {
                                attDataLen = attDataLen - consumedBytes;
                                totalConsumedBytes += consumedBytes;

                                if (CSR_BT_AV_REMOTE_CONTROL_UUID == protocolValue)
                                {
                                    *version = (CsrUint16)returnValue;
                                    retBool = TRUE;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return retBool;
}

static void csrBtAvrcpSdpExtractServiceRecordV13Data(CmnCsrBtLinkedListStruct *bll_p,
                                           CsrUint16                 serviceHandleIndex,
                                           CsrUint16                 *avrcpVersion,
                                           CsrUint16                 *supportedFeatures,
                                           CsrCharString             **providerName,
                                           CsrCharString             **serviceName)
{
    CsrUint8 *string;
    CsrUint16 stringLen;
    CsrUint32 features;


    if (FALSE == csrBtAvrcpSdpGetBluetoothProfileDescriptorList(bll_p, serviceHandleIndex, avrcpVersion))
    {
        *avrcpVersion = CSR_BT_AVRCP_CONFIG_SR_VERSION_10;
    }

    if (TRUE == CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(bll_p, serviceHandleIndex, CSR_BT_SUPPORTED_FEATURES_ATTRIBUTE_IDENTIFIER, &features))
    {
        *supportedFeatures = (CsrUint16)features;
    }
    else
    {
        *supportedFeatures = 0;
    }

    if (TRUE == CsrBtUtilSdrGetStringAttributeFromAttributeUuid(bll_p, serviceHandleIndex, CSR_BT_SERVICE_PROVIDER_NAME, &string, &stringLen))
    {
        *providerName = CsrPmemAlloc(stringLen + 1);
        CsrMemCpy(*providerName, string, stringLen);
        (*providerName)[stringLen] = 0;
    }
    else
    {
        *providerName = NULL;
    }

    if (TRUE == CsrBtUtilSdrGetStringAttributeFromAttributeUuid(bll_p, serviceHandleIndex, CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER, &string, &stringLen))
    {
        *serviceName = CsrPmemAlloc(stringLen + 1);
        CsrMemCpy(*serviceName, string, stringLen);
        (*serviceName)[stringLen] = 0;
    }
    else
    {
        *providerName = NULL;
    }
}


CsrUint16 CsrBtAvrcpSdpGenerateServiceRecord(CsrBtAvrcpRoleDetails *roleDetails, CsrUint8 **sr, CsrUint8 role)
{
    CsrUint8     providerNameLen = (CsrUint8)CsrStrLen(roleDetails->providerName), serviceNameLen = (CsrUint8)CsrStrLen(roleDetails->serviceName);
    CsrUint16    avctpVersion, srLength = AVRCP_SDP_MINIMUM_RECORD_LENGTH;
    CsrUint8     srOffset = 0;
    CsrUint8     srCtExtraOffset = 0;
    CsrUint8     *tmpSr;
    CsrUint16    mask;

#ifdef CSR_BT_INSTALL_AVRCP_13_14
    /* Determine AVCTP version */
    if (roleDetails->srAvrcpVersion == CSR_BT_AVRCP_CONFIG_SR_VERSION_14)
    {
        avctpVersion    = AVRCP_VERSION_AVCTP_13;
        srOffset        = AVRCP_SDP_BROWSING_SIZE;
        srLength        += AVRCP_SDP_BROWSING_SIZE;
        if (role == AVRCP_ROLE_CONTROLLER)
        {
            srCtExtraOffset = AVRCP_SDP_CT_EXTRA_OFFSET;
            srLength   += AVRCP_SDP_CT_EXTRA_OFFSET;
        }
    }
    else if (roleDetails->srAvrcpVersion == CSR_BT_AVRCP_CONFIG_SR_VERSION_13)
    {
        avctpVersion = AVRCP_VERSION_AVCTP_12;
    }
    else
#endif        
    {/* Only AVRCP 1.0 features are supported */
        avctpVersion = AVRCP_VERSION_AVCTP_10;
    }

    /* Determine totalt length and allocate memory */
    *sr = CsrPmemAlloc(srLength +
                (roleDetails->providerName ? providerNameLen + AVRCP_SDP_OPTIONAL_PREFIX_SIZE : 0) +
                (roleDetails->serviceName ? serviceNameLen + AVRCP_SDP_OPTIONAL_PREFIX_SIZE : 0));
    tmpSr = *sr;

#ifdef CSR_BT_INSTALL_AVRCP_13_14
    /* Insert required elements in service record */
    if ((role == AVRCP_ROLE_CONTROLLER) && (roleDetails->srAvrcpVersion == CSR_BT_AVRCP_CONFIG_SR_VERSION_14))
    {
        CsrMemCpy(tmpSr, avrcpCtServiceRecordHeader, sizeof(avrcpCtServiceRecordHeader));
        tmpSr += sizeof(avrcpCtServiceRecordHeader);
    }
    else
#endif
    {
        CsrMemCpy(tmpSr, avrcpServiceRecordHeader, sizeof(avrcpServiceRecordHeader));
        tmpSr += sizeof(avrcpServiceRecordHeader);
    }

#ifdef CSR_BT_INSTALL_AVRCP_13_14
    if (srOffset)
    {
        CsrMemCpy(tmpSr, avrcpServiceRecordBrowsing, sizeof(avrcpServiceRecordBrowsing));
    }
    tmpSr += srOffset;
#endif
    CsrMemCpy(tmpSr, avrcpServiceRecordFooter, sizeof(avrcpServiceRecordFooter));

    mask = role == AVRCP_ROLE_TARGET ? CSR_BT_AV_REMOTE_CONTROL_TARGET_UUID : CSR_BT_AV_REMOTE_CONTROL_UUID;

    AVRCP_INSERT_UINT16(*sr + AVRCP_SDP_AVCTP_VERSION_INDEX + srCtExtraOffset, avctpVersion);
    AVRCP_INSERT_UINT16(*sr + AVRCP_SDP_AVRCP_UUID_INDEX, mask);
    AVRCP_INSERT_UINT16(*sr + AVRCP_SDP_AVRCP_VERSION_INDEX + srOffset + srCtExtraOffset, roleDetails->srAvrcpVersion);
    if(role == AVRCP_ROLE_CONTROLLER)
    {/* bit 4,5 and 7-15 are RFA: must not be used! */
        mask = roleDetails->srFeatures & AVRCP_CT_SDP_FEATURES_MASK;

        if (roleDetails->srAvrcpVersion == CSR_BT_AVRCP_CONFIG_SR_VERSION_14)
        {
            AVRCP_INSERT_UINT16(*sr + AVRCP_SDP_AVRCP_UUID_INDEX + srCtExtraOffset, CSR_BT_AV_REMOTE_CONTROL_CONTROLLER_UUID);
        }
        AVRCP_INSERT_UINT16(*sr + AVRCP_SDP_FEATURES_INDEX + srOffset + srCtExtraOffset, mask);
    }
    else
    { /* role is AVRCP_ROLE_TARGET */
        mask = roleDetails->srFeatures & AVRCP_TG_SDP_FEATURES_MASK;

        AVRCP_INSERT_UINT16(*sr + AVRCP_SDP_FEATURES_INDEX + srOffset, mask);
    }


    /* Insert optional elements in service record */
    if (roleDetails->providerName)
    {/* Provider Name */
        const CsrUint8 providerPrefix[AVRCP_SDP_OPTIONAL_PREFIX_SIZE] = {0x09, 0x01, 0x02, 0x25, 0x00};

        CsrMemCpy(*sr + srLength, providerPrefix, AVRCP_SDP_OPTIONAL_PREFIX_SIZE);
        *(*sr + srLength + AVRCP_SDP_OPTIONAL_PREFIX_SIZE_OFFSET) = providerNameLen;
        srLength += AVRCP_SDP_OPTIONAL_PREFIX_SIZE;

        CsrMemCpy(*sr + srLength, roleDetails->providerName, providerNameLen);
        srLength += providerNameLen;
    }

    if (roleDetails->serviceName)
    {/* Service Name */
        const CsrUint8 servicePrefix[AVRCP_SDP_OPTIONAL_PREFIX_SIZE] = {0x09, 0x01, 0x00, 0x25, 0x00};

        CsrMemCpy(*sr + srLength, servicePrefix, AVRCP_SDP_OPTIONAL_PREFIX_SIZE);
        *(*sr + srLength + AVRCP_SDP_OPTIONAL_PREFIX_SIZE_OFFSET) = serviceNameLen;
        srLength += AVRCP_SDP_OPTIONAL_PREFIX_SIZE;

        CsrMemCpy(*sr + srLength, roleDetails->serviceName, serviceNameLen);
        srLength += serviceNameLen;
    }

    return srLength;
}

/* Determines which SR to register */
CsrBool CsrBtAvrcpSdpRegisterSR(AvrcpInstanceData_t *instData)
{
    if (instData->srPending)
    {
        CsrUint8 *sr;
        CsrUint16 srLength;

        if (instData->srPending->tgDetails.roleSupported)
        {
            srLength = CsrBtAvrcpSdpGenerateServiceRecord(&instData->srPending->tgDetails, &sr, AVRCP_ROLE_TARGET);
            CsrBtCmSdsRegisterReqSend(CSR_BT_AVRCP_IFACEQUEUE, sr, srLength);
            AVRCP_CHANGE_STATE(instData->srActiveRole, AVRCP_ROLE_TARGET);
            CsrBtAvrcpUtilFreeRoleDetails(&instData->srPending->tgDetails);
            return TRUE;
        }
        else if (instData->srPending->ctDetails.roleSupported)
        {
            srLength = CsrBtAvrcpSdpGenerateServiceRecord(&instData->srPending->ctDetails, &sr, AVRCP_ROLE_CONTROLLER);
            CsrBtCmSdsRegisterReqSend(CSR_BT_AVRCP_IFACEQUEUE, sr, srLength);
            AVRCP_CHANGE_STATE(instData->srActiveRole, AVRCP_ROLE_CONTROLLER);
            CsrBtAvrcpUtilFreeConfigReq(&instData->srPending);
            return TRUE;
        }
        else
        {
            CsrBtAvrcpUtilFreeConfigReq(&instData->srPending); /* Make sure everything is released */
        }
    }

    return FALSE;
}

void CsrBtAvrcpSdpSearchStart(AvrcpInstanceData_t *instData, AvrcpConnInstance_t *connInst)
{
    CmnCsrBtLinkedListStruct *sdpTagList = NULL;
    CsrUint16                shIndex;

    if (AVRCP_STATE_SDP_IDLE == instData->sdpState)
    {
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
        if (instData->tgLocal.srHandle != AVRCP_SDP_INVALID_SR_HANDLE)
        {/* Search for the corresponding controller */
            sdpTagList = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTagList, CSR_BT_AV_REMOTE_CONTROL_UUID, &shIndex);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_BLUETOOTH_PROFILE_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SUPPORTED_FEATURES_ATTRIBUTE_IDENTIFIER, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SERVICE_PROVIDER_NAME, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER, NULL, 0);
        }
#endif
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
        if (instData->ctLocal.srHandle != AVRCP_SDP_INVALID_SR_HANDLE)
        {/* Search for the corresponding target */
            sdpTagList = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTagList, CSR_BT_AV_REMOTE_CONTROL_TARGET_UUID, &shIndex);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_BLUETOOTH_PROFILE_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SUPPORTED_FEATURES_ATTRIBUTE_IDENTIFIER, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SERVICE_PROVIDER_NAME, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER, NULL, 0);
        }
#endif
        AVRCP_CHANGE_STATE(instData->sdpState, AVRCP_STATE_SDP_ACTIVE);
        AVRCP_CHANGE_STATE_INDEX(connInst->sdpState, AVRCP_STATE_SDP_ACTIVE, connInst->appConnId);

        /* Start the SDP search */
        CsrBtUtilSdcSearchStart((void *)instData, instData->sdpSearchData, sdpTagList, connInst->address);
    }
}

void CsrBtAvrcpSdpRestartSearch(AvrcpInstanceData_t *instData)
{
    AvrcpConnInstance_t *connInst;
    /* Determine if SDP search should be started for other connections */
    connInst = AVRCP_LIST_CONN_GET_SDP_ST(&instData->connList, AVRCP_STATE_SDP_PENDING);

    if (connInst && (AVRCP_STATE_CONN_CONNECTED == connInst->control.state))
    {
        /* Start SDP search for pending connection */
        CsrBtAvrcpSdpSearchStart(instData, connInst);
    }
}

void CsrBtAvrcpSdpSearchCancel(AvrcpConnInstance_t *connInst)
{
    AvrcpInstanceData_t *instData = connInst->instData;
    AVRCP_CHANGE_STATE_INDEX(connInst->sdpState, AVRCP_STATE_SDP_DONE, connInst->appConnId);
    CsrBtUtilSdcSearchCancel((void *)instData, instData->sdpSearchData);
}

void CsrBtAvrcpSdpResultHandler(void             *inst,
                        CmnCsrBtLinkedListStruct *sdpTagList,
                        CsrBtDeviceAddr             deviceAddr,
                        CsrBtResultCode          resultCode,
                        CsrBtSupplier      resultSupplier)
{
    AvrcpInstanceData_t *instData = (AvrcpInstanceData_t *) inst;
    AvrcpConnInstance_t *connInst = AVRCP_LIST_CONN_GET_ADDR(&instData->connList, &deviceAddr);

    AVRCP_CHANGE_STATE(instData->sdpState, AVRCP_STATE_SDP_IDLE);

    if (connInst)
    {
        if (connInst->sdpState == AVRCP_STATE_SDP_ACTIVE)
        {/* Device is known, connected and doing SDP search */
            CsrBtAvrcpRoleDetails ctFeatures;
            CsrBtAvrcpRoleDetails tgFeatures;

            CsrBtAvrcpConfigRoleNoSupport(&tgFeatures);
            CsrBtAvrcpConfigRoleNoSupport(&ctFeatures);

            if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                resultSupplier == CSR_BT_SUPPLIER_CM &&
                sdpTagList)
            {
                CsrUintFast16    numOfSdpRecords = CsrBtUtilBllGetNofEntriesEx(sdpTagList);
                CsrUintFast16    sdpRecordIndex;
                CsrBtUuid32 tmpUuid;
                CsrUint16    tmpResult;
                CsrUint16    dummy1, dummy2; /* Currently CSR_UNUSED */

                for (sdpRecordIndex = 0; sdpRecordIndex < numOfSdpRecords; sdpRecordIndex++)
                {/* Handle each service record */
                    if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList,
                                                    sdpRecordIndex,
                                                    &tmpUuid,
                                                    &tmpResult,
                                                    &dummy1,
                                                    &dummy2))
                    {
                        if (tmpResult == SDR_SDC_SEARCH_SUCCESS)
                        {
                            if (tmpUuid == CSR_BT_AV_REMOTE_CONTROL_TARGET_UUID)
                            {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
                                csrBtAvrcpSdpExtractServiceRecordV13Data(sdpTagList,
                                                                    (CsrUint16)sdpRecordIndex,
                                                                    &connInst->ctLocal.tgSdpAvrcpVersion,
                                                                    &connInst->ctLocal.tgSdpSupportedFeatures,
                                                                    &tgFeatures.providerName,
                                                                    &tgFeatures.serviceName);

                                tgFeatures.srAvrcpVersion   = connInst->ctLocal.tgSdpAvrcpVersion;
                                tgFeatures.srFeatures       = connInst->ctLocal.tgSdpSupportedFeatures;
                                
                                /* If 
                                   The local device support the CT Role and
                                   The local device support AVRCP 1.4 or higher and
                                   The peer device support the TG Role and
                                   The peer device support AVRCP 1.4 or higher and
                                   The peer device either support 
                                   Category 1 or Category 3 or Browsing or Multiple Media Players     
               
                                   Then
                                   The local device should be able initiate a browsing channel
                                
                                   Note the local device will always accept and incoming browsing channel
                                   if it support AVRCP 1.4 or higher */ 
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
                                if ( (instData->srAvrcpVersionHighest >= CSR_BT_AVRCP_CONFIG_SR_VERSION_14)     && 
                                     (tgFeatures.srAvrcpVersion >= CSR_BT_AVRCP_CONFIG_SR_VERSION_14)           && 
                                     ((CSR_MASK_IS_SET(tgFeatures.srFeatures, CSR_BT_AVRCP_CONFIG_SR_FEAT_CAT1_PLAY_REC)) || 
                                      (CSR_MASK_IS_SET(tgFeatures.srFeatures, CSR_BT_AVRCP_CONFIG_SR_FEAT_CAT3_TUNER))    || 
                                      (CSR_MASK_IS_SET(tgFeatures.srFeatures, CSR_BT_AVRCP_CONFIG_SR_FEAT_BROWSING))      || 
                                      (CSR_MASK_IS_SET(tgFeatures.srFeatures, CSR_BT_AVRCP_CONFIG_SR_FEAT_MULTIPLE_MP))))    
                                {
                                    CSR_MASK_SET(connInst->remoteFeatures, CSR_BT_AVRCP_CONFIG_SR_FEAT_BROWSING);
                                }
#endif                                
#endif
                            tgFeatures.roleSupported = TRUE;
                            }
                            else if (tmpUuid == CSR_BT_AV_REMOTE_CONTROL_UUID)
                            {
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
                                csrBtAvrcpSdpExtractServiceRecordV13Data(sdpTagList,
                                                                    (CsrUint16) sdpRecordIndex,
                                                                    &connInst->tgLocal.ctSdpAvrcpVersion,
                                                                    &connInst->tgLocal.ctSdpSupportedFeatures,
                                                                    &ctFeatures.providerName,
                                                                    &ctFeatures.serviceName);

                                ctFeatures.srAvrcpVersion   = connInst->tgLocal.ctSdpAvrcpVersion;
                                ctFeatures.srFeatures       = connInst->tgLocal.ctSdpSupportedFeatures;
#endif
                                ctFeatures.roleSupported    = TRUE;
                            }
                        }
                    }
                }
            }

            if (connInst->control.state == AVRCP_STATE_CONN_PENDING)
            {/* Proceed by connecting to the remote device */
                instData->tgDetails = tgFeatures.roleSupported ? CsrMemDup(&tgFeatures, sizeof(CsrBtAvrcpRoleDetails)) : NULL;
                instData->ctDetails = ctFeatures.roleSupported ? CsrMemDup(&ctFeatures, sizeof(CsrBtAvrcpRoleDetails)) : NULL;
                CsrBtAvrcpUtilConnect(connInst);
            }
            else if (connInst->control.state == AVRCP_STATE_CONN_CONNECTED)
            {/* The remote features has been retrieved due to an incoming connection */
                CsrBtAvrcpRemoteFeaturesIndSend(instData->ctrlHandle,
                                           connInst->appConnId,
                                           connInst->address,
                                           &tgFeatures,
                                           &ctFeatures);
            }

            AVRCP_CHANGE_STATE_INDEX(connInst->sdpState, AVRCP_STATE_SDP_DONE, connInst->appConnId);
        }
        else if (connInst->sdpState == AVRCP_STATE_SDP_DONE)
        {/* Send confirmation to app; this is a cancel operation! */
            if (connInst->control.state != AVRCP_STATE_CONN_DISCONNECTING)
            {
                CsrBtAvrcpConnectCfmSend(instData->ctrlHandle,
                                         &connInst->address,
                                         AVRCP_MTU_INVALID,
                                         CSR_BT_AVRCP_CONNECTION_ID_INVALID,
                                         NULL,
                                         NULL,
                                         CSR_BT_RESULT_CODE_AVRCP_CONNECT_ATTEMPT_CANCELLED,
                                         CSR_BT_SUPPLIER_AVRCP,
                                         CSR_BT_CONN_ID_INVALID);
                AVRCP_LIST_CONN_REMOVE(&instData->connList, connInst);
                CsrBtAvrcpUtilGo2Idle(instData);
            }
            /* else wait for disconnect indication */
        }
    }
    else if((resultSupplier == CSR_BT_SUPPLIER_CM ) && (resultCode == CSR_BT_RESULT_CODE_CM_CANCELLED))
    {
        CsrBtAvrcpUtilGo2Idle(instData);
    }
    CsrBtUtilBllFreeLinkedList(&sdpTagList, CsrBtUtilBllPfreeWrapper);
}

#endif


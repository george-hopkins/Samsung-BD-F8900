/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_jsr82_prim.h"
#include "csr_bt_jsr82_main.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_jsr82_maintenance_handler.h"
#include "csr_bt_jsr82_discovery.h"
#include "csr_types.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_profiles.h"
#include "csr_bt_jsr82_main.h"
#include "csr_bt_jsr82_remote_device.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_usr_config.h"
#include "sdc_prim.h"

#define SERVICE_DB_HANDLE 0x00000000
#define SERVICE_DB_STATE_ID 0x0201
#define MAX_SERVICE_SEARCH_UUIDS 12

#define CONVERT_TO_SECONDS 1000000

#define JSR82_SEL_SERVICE_INQ_TIME 11

static void removeDuplicateAttr(CsrUint16 *reducedAttrSetLength, CsrUint16 **reducedAttrSet, CsrUint16 attrSetLength, CsrUint16 *attrSet);

static const CsrUint16 defaultAttributes[] =
{
    CSR_BT_SERVICE_RECORD_HANDLE_ATTRIBUTE_IDENTIFIER,
    CSR_BT_SERVICE_CLASS_ID_LIST,
    CSR_BT_SERVICE_RECORD_STATE_ATTRIBUTE_IDENTIFIER,
    CSR_BT_SERVICE_ID_ATTRIBUTE_IDENTIFIER,
    CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER
};

#define NUM_OF_DEFAULT_ATTR         ((CsrUint16)(sizeof(defaultAttributes)/sizeof(CsrUint16)))
#define JSR82_ATTRIBUTE_MAX_BYTES   0xFFFF

static CsrBool searchIsValid(CsrUint32 reqID,Jsr82MainInstanceData *instData);
static void cancelSearchReq(CsrUint32 reqID, Jsr82MainInstanceData *instData);
static void removeSearchReq(CsrUint32 reqID,Jsr82MainInstanceData *instData);

static void sendInquiryCompletedInd(CsrSchedQid appHandle, CsrUint32 reqID,
    CsrUint8 resultCode)
{
    CsrBtJsr82DlInquiryCompletedInd *msg;

    msg = (CsrBtJsr82DlInquiryCompletedInd *)CsrPmemAlloc(sizeof(CsrBtJsr82DlInquiryCompletedInd));
    msg->type = CSR_BT_JSR82_DL_INQUIRY_COMPLETED_IND;
    msg->respCode = resultCode;
    msg->reqID = reqID;

    CsrBtJsr82MessagePut(appHandle, msg);
}

static void inquiryTimeout(CsrUint16 dummy, void *argv)
{
    Jsr82MainInstanceData *instData;
    CSR_UNUSED(dummy);
    instData = (Jsr82MainInstanceData *)argv;
    instData->inquiry->timedOut = TRUE;
    instData->inquiry->timer = 0;
    if (instData->inquiry->state == INQUIRY_STATE)
    {
        /*CsrBtSdCancelSearchReqSend(CSR_BT_JSR82_IFACEQUEUE);*/
        CsrBtCmCancelInquiryReqSend(CSR_BT_JSR82_IFACEQUEUE);
        instData->inquiry->state = CANCEL_STATE;

        CsrBtJsr82DaSdCloseIndHandler(instData);
    }
    else if (instData->inquiry->state == CANCEL_STATE)
    {
        /* The cancel came first, give it higher priority */
        instData->inquiry->timedOut = FALSE;
    }
    else if (!instData->inquiry->indSent)
    {
        sendInquiryCompletedInd(instData->inquiry->appHandle,
            instData->inquiry->reqID, CSR_BT_JSR82_INQUIRY_COMPLETED);
        instData->inquiry->indSent = TRUE;
    }
}

static void startInquiry(Jsr82MainInstanceData *instData, CsrSchedQid appHandle, CsrUint32 reqID, CsrUint24 iac)
{
    instData->appHandle = appHandle;
    instData->reqID = reqID;

    instData->inqDeviceListMax = (CSR_BT_JSR82_MAX_MEMORY_BLOCK / (sizeof(CsrBtDeviceAddr)));
    if ((CSR_BT_JSR82_INQ_MAX_RESULTS < instData->inqDeviceListMax) && (CSR_BT_JSR82_INQ_MAX_RESULTS != 0))
    {
        instData->inqDeviceListMax = CSR_BT_JSR82_INQ_MAX_RESULTS;
    }

    if (instData->inqDeviceList != NULL)
    {
        CsrPmemFree(instData->inqDeviceList);
        instData->inqDeviceList = NULL;
    }
    instData->inqDeviceList = (CsrBtDeviceAddr *)CsrPmemZalloc((sizeof(CsrBtDeviceAddr) * instData->inqDeviceListMax));
    instData->inqDeviceListCount = 0;

    /*CsrBtSdSearchReqSend(CSR_BT_JSR82_IFACEQUEUE, 0, TRUE, CSR_BT_JSR82_INQ_TIME, 0, FALSE, TRUE, TRUE);*/
    CsrBtCmInquiryReqSend(CSR_BT_JSR82_IFACEQUEUE, iac, CSR_BT_SD_DEFAULT_INQUIRY_TX_POWER_LEVEL, FALSE);
    instData->inquiry = (inquiryStruct *)CsrPmemZalloc(sizeof(inquiryStruct));
    instData->inquiry->appHandle = appHandle;
    instData->inquiry->reqID = reqID;
    instData->inquiry->timer = CsrSchedTimerSet(CSR_BT_JSR82_INQ_TIME*CONVERT_TO_SECONDS, inquiryTimeout, 0, (void *)instData);
    instData->inquiry->state = INQUIRY_STATE;
    instData->inquiry->indSent = FALSE;
    instData->inquiry->nameReqSent = FALSE;
    instData->inquiry->iac = iac;
    instData->inquiry->resultCount = 0;
    CsrBtJsr82SetDiscovery(instData);
}

void CsrBtJsr82DaStartInquiry(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82DaStartInquiryReq *msg;

    msg = (CsrBtJsr82DaStartInquiryReq *) instData->recvMsg;
    startInquiry(instData, msg->appHandle, msg->reqID, msg->iac);
}

void CsrBtJsr82DaCancelInquiry(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82DaCancelInquiryReq *msg;

    msg = (CsrBtJsr82DaCancelInquiryReq *) instData->recvMsg;
    if((msg->reqID == instData->inquiry->reqID) && !instData->inquiry->timedOut)
    {
        CsrSchedTimerCancel(instData->inquiry->timer, NULL, NULL);
        instData->inquiry->timer    = 0;
        instData->inquiry->cancelled = TRUE;

        if (instData->inquiry->state == INQUIRY_STATE)
        {
            /*CsrBtSdCancelSearchReqSend(CSR_BT_JSR82_IFACEQUEUE);*/
            CsrBtCmCancelInquiryReqSend(CSR_BT_JSR82_IFACEQUEUE);
            instData->inquiry->state = CANCEL_STATE;

            CsrBtJsr82DaSdCloseIndHandler(instData);
        }
        else if (instData->inquiry->state == CANCEL_STATE)
        {
            /* Wait for the cancel to go through before sending the response */
        }
        else if (!instData->inquiry->indSent)
        {
            sendInquiryCompletedInd(instData->inquiry->appHandle,
                instData->inquiry->reqID, CSR_BT_JSR82_INQUIRY_TERMINATED);
            instData->inquiry->indSent = TRUE;
        }
    }
}

void CsrBtJsr82DlDeviceDiscovered(Jsr82MainInstanceData *instData)
{
    /*CsrBtSdSearchResultInd *prim;*/
    CsrBtCmInquiryResultInd *prim;
    CsrBtJsr82DlDeviceDiscoveredInd *msg;
    CsrUint16 i;
    CsrBool foundBefore = FALSE;

    msg = (CsrBtJsr82DlDeviceDiscoveredInd *)CsrPmemAlloc(sizeof(CsrBtJsr82DlDeviceDiscoveredInd));
    /*prim = (CsrBtSdSearchResultInd *)instData->recvMsg;*/
    prim = (CsrBtCmInquiryResultInd *)instData->recvMsg;

    msg->type = (CsrBtJsr82Prim)CSR_BT_JSR82_DL_DEVICE_DISCOVERED_IND;
    msg->reqID = instData->inquiry->reqID;
    /*msg->deviceAddr = prim->deviceProperties.deviceAddr;
    msg->classOfDevice = prim->deviceProperties.classOfDevice;*/
    msg->deviceAddr = prim->deviceAddr;
    msg->classOfDevice = prim->classOfDevice;
    msg->rssi = prim->rssi;

    CsrBtJsr82MessagePut(instData->inquiry->appHandle, msg);

    for (i = 0; i < instData->inqDeviceListCount; i++)
    {
        if (bd_addr_eq(&prim->deviceAddr, &instData->inqDeviceList[i]))
        {
            foundBefore = TRUE;
        }
    }
    if (foundBefore == FALSE)
    {
        if (instData->inqDeviceListCount < instData->inqDeviceListMax)
        {
            instData->inqDeviceList[instData->inqDeviceListCount++] = msg->deviceAddr;
        }
    }

#if CSR_BT_JSR82_INQ_MAX_RESULTS > 0
    if (++instData->inquiry->resultCount == CSR_BT_JSR82_INQ_MAX_RESULTS)
    {
        CsrSchedTimerCancel(instData->inquiry->timer, NULL, NULL);
        inquiryTimeout(0, (void *)instData);
    }
#endif

    CsrPmemFree(prim->eirData);
}

void CsrBtJsr82DlDeviceDiscoveredAndIgnored(Jsr82MainInstanceData *instData)
{
    CsrBtCmInquiryResultInd *prim;
    prim = (CsrBtCmInquiryResultInd *)instData->recvMsg;

    /* Just free any EIR-data and ignore the message */
    CsrPmemFree(prim->eirData);
}

void CsrBtJsr82DaInquiryGetFriendlyName(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82RdGetFriendlyNameReq *prim;
    requestedNamesStruct *element;

    prim = (CsrBtJsr82RdGetFriendlyNameReq *)instData->recvMsg;

    if(!instData->requestedNames)
    {
        instData->requestedNames = (requestedNamesStruct *)CsrPmemZalloc(sizeof(requestedNamesStruct));
        element = instData->requestedNames;
    }
    else
    {
        element = instData->requestedNames;
        while(element->next)
        {
            element = element->next;
        }
        element->next = (requestedNamesStruct *)CsrPmemZalloc(sizeof(requestedNamesStruct));
        element = element->next;
    }
    element->address = prim->address;
    element->alwaysAsk = prim->alwaysAsk;
    element->appHandle = prim->appHandle;
    element->reqID = prim->reqID;

    if(!instData->gettingName)
    {
        instData->gettingName = TRUE;
        if(instData->requestedNames->alwaysAsk)
        {
            /*CsrBtSdCancelSearchReqSend(CSR_BT_JSR82_IFACEQUEUE); */
            if (instData->inquiry->state == INQUIRY_STATE)
            {
                CsrBtCmCancelInquiryReqSend(CSR_BT_JSR82_IFACEQUEUE);
                instData->inquiry->state = CANCEL_STATE;

                CsrBtJsr82DaSdCloseIndHandler(instData);
            }
        }
        else
        {
            CsrBtSdReadDeviceInfoReqSend(CSR_BT_JSR82_IFACEQUEUE, instData->requestedNames->address); /* confirm is taken by jsr82RemoteNameCfm */
            instData->inquiry->nameReqSent = TRUE;
        }
    }
}

void CsrBtJsr82DaContinueInquiry(Jsr82MainInstanceData *instData)
{
    CsrBtCmInquiryReqSend(CSR_BT_JSR82_IFACEQUEUE, instData->inquiry->iac, CSR_BT_SD_DEFAULT_INQUIRY_TX_POWER_LEVEL, FALSE);
    instData->inquiry->state = INQUIRY_STATE;
}


void CsrBtJsr82DaSdCloseIndHandler(Jsr82MainInstanceData *instData)
{
    if(instData->gettingName)
    {
        if (instData->inquiry->timedOut)
        {
            if (!instData->inquiry->indSent)
            {
                sendInquiryCompletedInd(instData->inquiry->appHandle,
                    instData->inquiry->reqID, CSR_BT_JSR82_INQUIRY_COMPLETED);
                instData->inquiry->indSent = TRUE;
            }
            instData->inquiry->state = NAME_STATE;
            CsrBtCmReadRemoteNameReqSend(CSR_BT_JSR82_IFACEQUEUE, instData->requestedNames->address);
            instData->inquiry->nameReqSent = TRUE;
        }
        else if (instData->inquiry->cancelled)
        {
            if (!instData->inquiry->indSent)
            {
                sendInquiryCompletedInd(instData->inquiry->appHandle,
                    instData->inquiry->reqID, CSR_BT_JSR82_INQUIRY_TERMINATED);
                instData->inquiry->indSent = TRUE;
            }
            instData->inquiry->state = NAME_STATE;
            CsrBtCmReadRemoteNameReqSend(CSR_BT_JSR82_IFACEQUEUE, instData->requestedNames->address);
            instData->inquiry->nameReqSent = TRUE;
        }
        else if (instData->inquiry->state == CANCEL_STATE)
        {
            instData->inquiry->state = NAME_STATE;
            CsrBtCmReadRemoteNameReqSend(CSR_BT_JSR82_IFACEQUEUE, instData->requestedNames->address);
            instData->inquiry->nameReqSent = TRUE;
        }
        else if (!instData->inquiry->indSent)
        {
            sendInquiryCompletedInd(instData->inquiry->appHandle,
                instData->inquiry->reqID, CSR_BT_JSR82_INQUIRY_ERROR);
            instData->inquiry->indSent = TRUE;
        }
    }
    else
    {
        if (!instData->inquiry->indSent)
        {
            if (instData->inquiry->cancelled)
            {
                sendInquiryCompletedInd(instData->inquiry->appHandle,
                    instData->inquiry->reqID, CSR_BT_JSR82_INQUIRY_TERMINATED);
            }
            else if (instData->inquiry->timedOut)
            {
                sendInquiryCompletedInd(instData->inquiry->appHandle,
                    instData->inquiry->reqID, CSR_BT_JSR82_INQUIRY_COMPLETED);
            }
            else
            {
                sendInquiryCompletedInd(instData->inquiry->appHandle,
                    instData->inquiry->reqID, CSR_BT_JSR82_INQUIRY_ERROR);
                CsrSchedTimerCancel(instData->inquiry->timer,NULL,NULL);
            }
        }
        CsrPmemFree(instData->inquiry);
        instData->inquiry = NULL;
        CsrBtJsr82SetIdle(instData);
    }
}

static void getTheAttribute(getAttrStruct *getAttr)
{
    CsrBtCmSdcAttributeReqSend(getAttr->serviceRecHandles[getAttr->serviceIndex], getAttr->attrSet[getAttr->attrCounter], JSR82_ATTRIBUTE_MAX_BYTES);
}

static void saveAttr(CsrUint16 *outAttributesLength, CsrUint8 **outAttributes, CsrUint16 attributesLength, CsrUint8 *attributes)
{
    CsrUint8 *tempAttributes;
    CsrUint16 tempAttributesLength;

    tempAttributesLength = (CsrUint16)(*outAttributesLength + attributesLength);
    tempAttributes = (CsrUint8 *)CsrPmemAlloc(tempAttributesLength*sizeof(CsrUint8));

    CsrMemCpy(tempAttributes, *outAttributes, *outAttributesLength);
    CsrMemCpy(tempAttributes + *outAttributesLength, attributes, attributesLength);
    *outAttributesLength = tempAttributesLength;
    CsrPmemFree(*outAttributes);
    *outAttributes = tempAttributes;
}

static void addHeader(CsrUint16 *attributesLength, CsrUint8 **attributes)
{
    CsrUint16    tempAttributesLength = 0;
    CsrUint8     *tempAttributes = NULL;

    if(*attributesLength > 0)
    {
        CsrUint8     header = 0x35;
        CsrUint16    headerSize = 2;
        CsrUint8     i = 0;

        if (*attributesLength > 255)
        {
            header = 0x36;
            headerSize = 3;
        }

        tempAttributesLength = (CsrUint16)(*attributesLength + headerSize);
        tempAttributes = (CsrUint8 *)CsrPmemAlloc(tempAttributesLength * sizeof(CsrUint8));
        tempAttributes[0] = header;
        for(i=1; i < (headerSize); i++)
        {
            tempAttributes[i] = (CsrUint8)((*attributesLength >> (8 * (headerSize - i - 1))) & 0xFF);
        }
        CsrMemCpy(tempAttributes + headerSize, *attributes, *attributesLength);
        CsrPmemFree(*attributes);
        *attributes = tempAttributes;
        *attributesLength = tempAttributesLength;
    }
}

static void sendCsrBtJsr82DlServicesDiscoveredInd(CsrSchedQid appHandle, CsrUint32 reqID, CsrUint16 attributesLength, CsrUint8 *attributes)
{
    CsrBtJsr82DlServicesDiscoveredInd *msg;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82DlServicesDiscoveredInd));
    msg->type = CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND;
    msg->reqID = reqID;
    msg->attributesLength = attributesLength;
    msg->attributes = attributes;
    CsrBtJsr82MessagePut(appHandle, msg);
}

static void sendCsrBtJsr82SrPopulateRecordCfm(CsrSchedQid appHandle, CsrUint32 reqID,
    CsrUint16 attributesLength, CsrUint8 *attributes, CsrBtResultCode result)
{
    CsrBtJsr82SrPopulateRecordCfm *msg;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82SrPopulateRecordCfm));
    msg->type = CSR_BT_JSR82_SR_POPULATE_RECORD_CFM;
    msg->reqID = reqID;
    msg->attributesLength = attributesLength;
    msg->attributes = attributes;
    msg->resultCode = result;

    CsrBtJsr82MessagePut(appHandle, msg);
}

static CsrBool csrBtJsr82SdcSdpResultCodeHandler(CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    if (resultSupplier == CSR_BT_SUPPLIER_SDP_SDC)
    {
        switch  (resultCode)
        {
            case SDC_NO_RESPONSE_DATA:
            case SDC_RESPONSE_PDU_HEADER_ERROR:
            case SDC_RESPONSE_PDU_SIZE_ERROR:
            case SDC_RESPONSE_TIMEOUT_ERROR:
            case SDC_DATA_CFM_ERROR:
            case SDC_SEARCH_DATA_ERROR:
            case SDC_RESPONSE_OUT_OF_MEMORY:
            case SDC_SEARCH_SIZE_TO_BIG:
            case SDC_ERROR_RESPONSE_PDU:
            case SDC_SEARCH_BUSY:
                return(FALSE);
            default:
                return(TRUE);
        }
    }
    return(FALSE);
}

void CsrBtJsr82GetAttrSet(Jsr82MainInstanceData *instData)
{

    getAttrStruct *getAttr;
    CsrBtCmSdcAttributeCfm *prim;
    prim = (CsrBtCmSdcAttributeCfm *)instData->recvMsg;

    getAttr = instData->getAttr;

    if (getAttr->gettingDBState)
    {
        CsrUint32 servDBState;
        getAttr->gettingDBState = FALSE;
        if ((prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS) &&
            (prim->resultSupplier == CSR_BT_SUPPLIER_CM) &&
            prim->attributeListSize &&
            CsrBtSdcGetUint32Direct(prim->attributeListSize, prim->attributeList, SERVICE_DB_STATE_ID, &servDBState))
        {
            getAttr->servDBstate = servDBState;
            getAttr->servDBvalid = TRUE;
        }
        else
        {
            getAttr->servDBvalid = FALSE;
        }

        if (instData->state == SERV_SEARCH_S)
        {
            getTheAttribute(getAttr);
        }
        else
        {
            if (getAttr->servDBvalid && (getAttr->servDBstate == instData->populate->serviceDataBaseState))
            {
                getTheAttribute(getAttr);
            }
            else
            {
                CsrBtCmSdcCloseReqSend(CSR_BT_JSR82_IFACEQUEUE);
                instData->populate->result = CSR_BT_RESULT_CODE_JSR82_SDC_DATABASE_STATE_CHANGED;
            }
        }
    }
    else
    {
        if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            saveAttr(&getAttr->attributesLength, &getAttr->attributes, prim->attributeListSize, prim->attributeList);
            getAttr->attrCounter++;
            getAttr->attrRetries = 0;
        }
        else if (prim->resultCode == SDC_SEARCH_BUSY && prim->resultSupplier == CSR_BT_SUPPLIER_SDP_SDC)
        {
            if(getAttr->attrRetries < ATTR_NUM_OF_RETRIES)
            {
                getAttr->attrRetries++;
            }
            else
            {
                getAttr->attrRetries = 0;
                getAttr->attrCounter++;
            }
        }
        else
        { /* skip failed attribute */
            getAttr->attrCounter++;
        }

        if((getAttr->attrCounter < getAttr->attrSetLength) && !csrBtJsr82SdcSdpResultCodeHandler(prim->resultCode, prim->resultSupplier))
        {
            getTheAttribute(getAttr);
        }
        else
        {
            addHeader(&getAttr->attributesLength, &getAttr->attributes);
            if (instData->state == SERV_SEARCH_S)
            {
                if (!csrBtJsr82SdcSdpResultCodeHandler(prim->resultCode, prim->resultSupplier))
                {
                    sendCsrBtJsr82DlServicesDiscoveredInd(instData->appHandle, instData->reqID, getAttr->attributesLength, getAttr->attributes);
                    getAttr->attributes = NULL;
                    getAttr->attributesLength = 0;
                }
            }
            else
            {

                instData->populate->done = TRUE;
                if (getAttr->attributesLength == 0)
                {
                    instData->populate->result = CSR_BT_RESULT_CODE_JSR82_CSR_BT_SDC_EMPTY_RESPONSE;
                }
                else
                {
                    instData->populate->result = CSR_BT_RESULT_CODE_JSR82_SUCCESS;
                }
            }

            if (csrBtJsr82SdcSdpResultCodeHandler(prim->resultCode, prim->resultSupplier))
            {
                getAttr->attrResult = CSR_BT_RESULT_CODE_JSR82_SDC_CONNECTION_RESPONSE_ERROR;
                CsrBtCmSdcCloseReqSend(CSR_BT_JSR82_IFACEQUEUE);
            }
            else
            {
                getAttr->serviceIndex++;
                getAttr->attrCounter = 0;
                getAttr->serviceCount++;
                if (getAttr->serviceIndex < getAttr->serviceHandleListCount)
                {
                    getTheAttribute(getAttr);
                }
                else
                {
                    CsrBtCmSdcCloseReqSend(CSR_BT_JSR82_IFACEQUEUE);
                }
            }
        }
    }
    CsrPmemFree(prim->attributeList);
}

void CsrBtJsr82StartGetAttrSet(Jsr82MainInstanceData *instData,
                          CsrUint32 *serviceRecHandles, CsrUint16 serviceRecHandlesLength,
                          CsrUint16 *attrSet, CsrUint16 attrSetLength)
{
    instData->getAttr = CsrPmemZalloc(sizeof(getAttrStruct));
    removeDuplicateAttr(&instData->getAttr->attrSetLength, &instData->getAttr->attrSet, attrSetLength, attrSet);

    instData->getAttr->serviceRecHandles = serviceRecHandles;
    instData->getAttr->serviceHandleListCount = serviceRecHandlesLength;
    instData->getAttr->attrResult = CSR_BT_RESULT_CODE_JSR82_SUCCESS;

    instData->getAttr->gettingDBState = TRUE;
    CsrBtCmSdcAttributeReqSend(SERVICE_DB_HANDLE, SERVICE_DB_STATE_ID, JSR82_ATTRIBUTE_MAX_BYTES);
}

static CsrBool searchIsValid(CsrUint32 reqID, Jsr82MainInstanceData *instData)
{
    if(instData->validSearches)
    {
        validSearchStruct *element;
        element = instData->validSearches;
        while(element)
        {
            if(element->search.reqID == reqID && element->search.valid == TRUE)
            {
                return TRUE;
            }

            element=element->next;

        }
    }
    return FALSE;
}

void CsrBtJsr82AddSearchReq(Jsr82MainInstanceData *instData)
{

    CsrBtJsr82DaSearchServicesReq *prim;
    prim = (CsrBtJsr82DaSearchServicesReq *)instData->recvMsg;

    if(!instData->validSearches)
    {
        instData->validSearches = CsrPmemZalloc(sizeof(validSearchStruct));
        instData->validSearches->search.reqID = prim->reqID;
        instData->validSearches->search.valid = TRUE;
    }
    else
    {
        CsrBool found = FALSE;
        validSearchStruct *element;
        validSearchStruct *prevElement = NULL;
        element = instData->validSearches;

        while(element)
        {
            if(element->search.reqID == prim->reqID)
            {
                found = TRUE;
                break;
            }
            prevElement=element;
            element=element->next;
        }
        if(!found)
        {
            prevElement->next = CsrPmemZalloc(sizeof(validSearchStruct));
            prevElement->next->search.reqID = prim->reqID;
            prevElement->next->search.valid = TRUE;

        }
    }

    if(instData->state != IDLE_S)
    {
        CsrBtJsr82SaveMessage(instData);
    }
    else
    {
        CsrBtJsr82DaSearchServices(instData);
    }
}


static void removeSearchReq(CsrUint32 reqID,Jsr82MainInstanceData *instData)
{
    if(instData->validSearches)
    {
        validSearchStruct *element;
        validSearchStruct *prevElement = NULL;
        element = instData->validSearches;
        while(element)
        {
            if(element->search.reqID == reqID)
            {
                if(prevElement)
                {
                    prevElement->next = element->next;
                }
                else
                {
                    instData->validSearches = element->next;
                }
                CsrPmemFree(element);
                return;
            }
            prevElement=element;
            element=element->next;
        }
    }
}


static void cancelSearchReq(CsrUint32 reqID,Jsr82MainInstanceData *instData)
{
    if(instData->validSearches)
    {
        validSearchStruct *element;
        element = instData->validSearches;
        while(element)
        {
            if(element->search.reqID == reqID)
            {
                element->search.valid = FALSE;
                break;
            }
            element=element->next;
        }
    }
    CsrBtCmSdcCancelServiceSearchReqSend(instData->appHandle, instData->serviceSearchDeviceAddr);
}



static void removeDuplicateAttr(CsrUint16 *reducedAttrSetLength, CsrUint16 **reducedAttrSet, CsrUint16 attrSetLength, CsrUint16 *attrSet)
{
    CsrUint16 i,j;
    CsrBool present=FALSE;

    *reducedAttrSetLength = 0;
    *reducedAttrSet = (CsrUint16 *)CsrPmemAlloc((attrSetLength)*sizeof(CsrUint16));

    for(i=0; i<attrSetLength; i++)
    {
        present=FALSE;
        for(j=0; j<(*reducedAttrSetLength); j++)
        {
            if((*reducedAttrSet)[j] == attrSet[i])
            {
                present=TRUE;
            }
        }
        if(!present)
        {
            (*reducedAttrSet)[(*reducedAttrSetLength)] = attrSet[i];
            (*reducedAttrSetLength)++;
        }
    }
    CsrPmemFree(attrSet);
}

void CsrBtJsr82DaSearchServices(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82DaSearchServicesReq *prim;
    prim = (CsrBtJsr82DaSearchServicesReq *)instData->recvMsg;
    instData->appHandle = prim->appHandle;
    instData->reqID = prim->reqID;
    instData->serviceSearchDeviceAddr = prim->deviceAddr;

    if(!searchIsValid(prim->reqID, instData)) /* check if search has been cancelled */
    {
        CsrBtJsr82DlServiceSearchCompletedInd *ind;

        ind = (CsrBtJsr82DlServiceSearchCompletedInd *)CsrPmemAlloc(sizeof(CsrBtJsr82DlServiceSearchCompletedInd));
        ind->type = CSR_BT_JSR82_DL_SERVICE_SEARCH_COMPLETED_IND;
        ind->reqID = instData->reqID;
        ind->serviceDataBaseState = 0;
        ind->serviceDBStateValid = FALSE;
        ind->respCode = CSR_BT_JSR82_SERVICE_SEARCH_TERMINATED;
        CsrBtJsr82MessagePut(instData->appHandle, ind);
        removeSearchReq(instData->reqID, instData);
        CsrPmemFree(prim->uuidSet);
    }
    else
    {
            CsrUint16    *tempAttrSet;

            CsrBtCmSdcServiceSearchReqSend(CSR_BT_JSR82_IFACEQUEUE, instData->serviceSearchDeviceAddr, prim->uuidSetLength, prim->uuidSet);

            instData->servSearch = CsrPmemZalloc(sizeof(servSearchStruct));

            /* prepend default attributes to the attrSet */
            tempAttrSet = CsrPmemAlloc((prim->attrSetLength + NUM_OF_DEFAULT_ATTR)*sizeof(CsrUint16));
            CsrMemCpy(tempAttrSet, defaultAttributes, NUM_OF_DEFAULT_ATTR*sizeof(CsrUint16));
            CsrMemCpy(tempAttrSet+NUM_OF_DEFAULT_ATTR, prim->attrSet, prim->attrSetLength*sizeof(CsrUint16));
            CsrPmemFree(prim->attrSet);

            instData->servSearch->attrSetLength = (CsrUint16)(prim->attrSetLength+NUM_OF_DEFAULT_ATTR); /* allow for default attributes */
            instData->servSearch->attrSet       = tempAttrSet;

            /* Set the Jsr82 task to SERV_SEARCH_S state */
            CsrBtJsr82SetServSearch(instData);
    }
}

void CsrBtJsr82DlServicesDiscovered(Jsr82MainInstanceData *instData)
{
    CsrBtCmSdcServiceSearchCfm *prim;
    prim = (CsrBtCmSdcServiceSearchCfm *)instData->recvMsg;

    if(searchIsValid(instData->reqID, instData))
    {
        if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            CsrBtJsr82StartGetAttrSet(instData, prim->recList, prim->recListLength, instData->servSearch->attrSet, instData->servSearch->attrSetLength);
        }
        else
        {
            CsrPmemFree(instData->servSearch->attrSet);
            instData->servSearch->attrSet = NULL;
        }
    }
    else
    {
        CsrBtCmSdcCloseReqSend(CSR_BT_JSR82_IFACEQUEUE);
        CsrPmemFree(instData->servSearch->attrSet);
        CsrPmemFree(prim->recList);
        instData->servSearch->attrSet = NULL;
    }
    CsrPmemFree(instData->servSearch);
    instData->servSearch = NULL;
}

void CsrBtJsr82DaCancelServiceSearch(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82DaCancelServiceSearchReq *prim;
    prim = (CsrBtJsr82DaCancelServiceSearchReq *)instData->recvMsg;
    cancelSearchReq(prim->reqID, instData);
}

void CsrBtJsr82ClosedServiceSearch(Jsr82MainInstanceData *instData)
{
    CsrBtCmSdcCloseInd *prim;
    CsrBtJsr82DlServiceSearchCompletedInd *msg;

    prim = (CsrBtCmSdcCloseInd *)instData->recvMsg;

    msg = (CsrBtJsr82DlServiceSearchCompletedInd *)CsrPmemZalloc(sizeof(CsrBtJsr82DlServiceSearchCompletedInd));
    msg->type = CSR_BT_JSR82_DL_SERVICE_SEARCH_COMPLETED_IND;
    msg->reqID = instData->reqID;
    msg->serviceDataBaseState = 0;
    msg->serviceDBStateValid = FALSE;

    if(!searchIsValid(instData->reqID, instData))
    {
        /* search has been cancelled */
        msg->respCode = CSR_BT_JSR82_SERVICE_SEARCH_TERMINATED;
    }
    else
    {
        removeSearchReq(instData->reqID, instData); /* Remove completed search from list of valid requests */

        if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
            prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
        {
            if (instData->getAttr)
            {
                if (instData->getAttr->serviceCount > 0)
                {
                    msg->serviceDataBaseState = instData->getAttr->servDBstate;
                    msg->serviceDBStateValid = instData->getAttr->servDBvalid;
                    msg->respCode = CSR_BT_JSR82_SERVICE_SEARCH_COMPLETED;
                }
                else
                {
                    if (instData->getAttr->attrResult == CSR_BT_RESULT_CODE_JSR82_SDC_CONNECTION_RESPONSE_ERROR)
                    {
                        msg->respCode = CSR_BT_JSR82_SERVICE_SEARCH_DEVICE_NOT_REACHABLE;
                    }
                    else
                    {
                        msg->respCode = CSR_BT_JSR82_SERVICE_SEARCH_NO_RECORDS;
                    }
                }
            }
            else
            {
                msg->respCode = CSR_BT_JSR82_SERVICE_SEARCH_ERROR;
            }
        }
        else
        {
            msg->respCode = CSR_BT_JSR82_SERVICE_SEARCH_ERROR;
        }
    }
    CsrBtJsr82MessagePut(instData->appHandle, msg);
    if (instData->getAttr)
    {
        CsrPmemFree(instData->getAttr->serviceRecHandles);
        CsrPmemFree(instData->getAttr->attrSet);
        CsrPmemFree(instData->getAttr->attributes);
        CsrPmemFree(instData->getAttr);
        instData->getAttr = NULL;
    }
    if (instData->servSearch)
    {
        CsrPmemFree(instData->servSearch->attrSet);
        CsrPmemFree(instData->servSearch);
        instData->servSearch = NULL;
    }
    CsrBtJsr82SetIdle(instData);
}

void CsrBtJsr82SrPopulateRecordReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82SrPopulateRecordReq *prim;
    prim = (CsrBtJsr82SrPopulateRecordReq *)instData->recvMsg;

    instData->appHandle = prim->appHandle;
    instData->reqID = prim->reqID;

    /* Save attrSet to a temporary struct, and open connection to check serviceDataBaseState */
    instData->populate = (populateStruct *)CsrPmemZalloc(sizeof(populateStruct));
    instData->populate->serviceDataBaseState = prim->serviceDataBaseState;
    instData->populate->attrSet = prim->attrSet;
    instData->populate->attrSetLength = prim->attrSetLength;
    instData->populate->serviceRecHandle = prim->serviceRecHandle;
    instData->populate->done = FALSE;

    /* Open SDC connection to remote device */
    CsrBtCmSdcOpenReqSend(CSR_BT_JSR82_IFACEQUEUE, prim->deviceAddr);
    CsrBtJsr82SetBusy(instData);
}

void CsrBtJsr82CmSdcOpenCfmHandler(Jsr82MainInstanceData *instData)
{
    /* Get serviceDataBaseState attribute */
    CsrUint32 * serviceRecHandle = (CsrUint32 *) CsrPmemAlloc(sizeof(CsrUint32));
    *serviceRecHandle           = instData->populate->serviceRecHandle;

    CsrBtJsr82StartGetAttrSet(instData, serviceRecHandle, 1, instData->populate->attrSet, instData->populate->attrSetLength);
    instData->populate->attrSet = NULL;  /* Ownership has been transferred */
    instData->populate->attrSetLength = 0;
}

void CsrBtJsr82PopulateCloseHandler(Jsr82MainInstanceData *instData)
{
    if(instData->populate->done)
    {
        sendCsrBtJsr82SrPopulateRecordCfm(   instData->appHandle,
                                        instData->reqID,
                                        instData->getAttr->attributesLength,
                                        instData->getAttr->attributes,
                                        instData->populate->result);
        instData->getAttr->attributes = NULL;
    }
    else
    {
        sendCsrBtJsr82SrPopulateRecordCfm(   instData->appHandle,
                                        instData->reqID,
                                        0,
                                        NULL,
                                        instData->populate->result);
    }
    CsrPmemFree(instData->populate);
    instData->populate = NULL;

    if (instData->getAttr)
    {
        CsrPmemFree(instData->getAttr->serviceRecHandles);
        CsrPmemFree(instData->getAttr->attributes);
        CsrPmemFree(instData->getAttr->attrSet);
        CsrPmemFree(instData->getAttr);
        instData->getAttr = NULL;
    }

    CsrBtJsr82SetIdle(instData);
}

static CsrBool deviceExists(CsrBtDeviceAddr address, selectServiceListStruct *list)
{
    selectServiceListStruct *listPtr;

    for (listPtr = list; listPtr; listPtr = listPtr->next)
    {
        if (bd_addr_eq(&address, &(listPtr->deviceAddr)))
        {
            return TRUE;
        }
    }
    return FALSE;
}


void CsrBtJsr82DaRetrieveDevicesReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82DaRetrieveDevicesReq *prim;
    CsrBtJsr82DaRetrieveDevicesCfm *msg;

    prim = (CsrBtJsr82DaRetrieveDevicesReq *)instData->recvMsg;
    instData->appHandle = prim->appHandle;
    instData->reqID = prim->reqID;

    if (prim->option == CSR_BT_JSR82_DISCOVERY_AGENT_CACHED)
    {
        CsrBtDeviceAddr *deviceListToSend;

        deviceListToSend = (CsrBtDeviceAddr *)CsrPmemZalloc((sizeof(CsrBtDeviceAddr) * instData->inqDeviceListMax));
        CsrMemCpy(deviceListToSend, instData->inqDeviceList, (sizeof(CsrBtDeviceAddr) * instData->inqDeviceListMax));

        msg = CsrPmemAlloc(sizeof(CsrBtJsr82DaRetrieveDevicesCfm));
        msg->type = CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM;
        msg->reqID = instData->reqID;
        msg->devicesCount = instData->inqDeviceListCount;
        msg->devices = deviceListToSend;
        CsrBtJsr82MessagePut(instData->appHandle, msg);

        CsrBtJsr82SetIdle(instData);
    }
    else if (prim->option == CSR_BT_JSR82_DISCOVERY_AGENT_PAIRED)
    {
        CsrBtSdReadDeviceListReqSend(CSR_BT_JSR82_IFACEQUEUE, CSR_BT_JSR82_MAX_MEMORY_BLOCK, CSR_BT_SD_DEVICE_EXCLUDE_NON_PAIRED_DEVICES);
        CsrBtJsr82SetBusy(instData);
    }
    else
    {
        msg = CsrPmemAlloc(sizeof(CsrBtJsr82DaRetrieveDevicesCfm));
        msg->type = CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM;
        msg->reqID = prim->reqID;
        msg->devicesCount = 0;
        msg->devices = NULL;
        CsrBtJsr82MessagePut(prim->appHandle, msg);
    }
}

void CsrBtJsr82SdReadDeviceListIndHandler(Jsr82MainInstanceData *instData)
{
    CsrUint16 i=0;
    CsrUint16 maxDevices = CSR_BT_JSR82_MAX_MEMORY_BLOCK/sizeof(CsrBtDeviceAddr);
    CsrBtSdReadDeviceListInd *prim;
    prim = (CsrBtSdReadDeviceListInd *)instData->recvMsg;

    if(!instData->trustedDevicesTempStorage)
    {
        instData->trustedDevicesTempStorage = (CsrBtDeviceAddr *)CsrPmemAlloc(sizeof(CsrBtDeviceAddr)*maxDevices);
        instData->trustedDevicesTempNumber = 0;
    }

    while((instData->trustedDevicesTempNumber < maxDevices) && (i < prim->deviceInfoCount))
    {
        CsrPmemFree(prim->deviceInfo[i].info);
        instData->trustedDevicesTempStorage[instData->trustedDevicesTempNumber++] = prim->deviceInfo[i++].deviceAddr;
    }

    CsrPmemFree(prim->deviceInfo);
}

void CsrBtJsr82SdReadDeviceListCfmHandler(Jsr82MainInstanceData *instData)
{
    CsrBtSdReadDeviceListCfm         *prim;
    CsrBtDeviceAddr                *devices;
    CsrUint16                    devicesCount;
    CsrUint32                    index = 0;
    CsrBtJsr82DaRetrieveDevicesCfm   *msg;
    CsrUint16 maxDevices = CSR_BT_JSR82_MAX_MEMORY_BLOCK/sizeof(CsrBtDeviceAddr);
    prim = (CsrBtSdReadDeviceListCfm *)instData->recvMsg;

    devicesCount = (CsrUint16)(((instData->trustedDevicesTempNumber + prim->deviceInfoCount) < maxDevices) ? (instData->trustedDevicesTempNumber + prim->deviceInfoCount) : maxDevices);
    devices = (CsrBtDeviceAddr *)CsrPmemAlloc(sizeof(CsrBtDeviceAddr)*devicesCount);

    if(instData->trustedDevicesTempStorage)
    {
        CsrMemCpy(devices, instData->trustedDevicesTempStorage, instData->trustedDevicesTempNumber*sizeof(CsrBtDeviceAddr));
        CsrPmemFree(instData->trustedDevicesTempStorage);
        instData->trustedDevicesTempStorage = NULL;
    }
    else
    {
        instData->trustedDevicesTempNumber = 0;
    }

    while (instData->trustedDevicesTempNumber < devicesCount)
    {
        CsrPmemFree(prim->deviceInfo[index].info);
        devices[instData->trustedDevicesTempNumber++] = prim->deviceInfo[index++].deviceAddr;
    }
    instData->trustedDevicesTempNumber = 0;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82DaRetrieveDevicesCfm));
    msg->type = CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM;
    msg->reqID = instData->reqID;
    msg->devicesCount = devicesCount;
    msg->devices = devices;
    CsrBtJsr82MessagePut(instData->appHandle, msg);

    CsrPmemFree(prim->deviceInfo);
    CsrBtJsr82SetIdle(instData);
}


void CsrBtJsr82DaSelectServiceReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82DaSelectServiceReq *prim;

    prim = (CsrBtJsr82DaSelectServiceReq *)instData->recvMsg;
    instData->appHandle = prim->appHandle;
    instData->reqID = prim->reqID;
    instData->selectService = CsrPmemZalloc(sizeof(selectServiceStruct));
    instData->selectService->uuid       = prim->requestedUuid;
    instData->selectService->uuidLength = prim->requestedUuidLength;
    instData->selectService->handleList = NULL;

    CsrBtCmInquiryReqSend(CSR_BT_JSR82_IFACEQUEUE, CSR_BT_CM_ACCESS_CODE_GIAC, CSR_BT_SD_DEFAULT_INQUIRY_TX_POWER_LEVEL, FALSE);
    instData->selectService->inquiryTimer = CsrSchedTimerSet(JSR82_SEL_SERVICE_INQ_TIME * 1000000, CsrBtJsr82CmSelServiceDiscTimeout, 0, (void *) instData);

    CsrBtJsr82SetSelectService(instData);
}

void CsrBtJsr82CmSelServiceDiscResIndHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmInquiryResultInd *prim;

    prim = (CsrBtCmInquiryResultInd *)instData->recvMsg;

    if (!deviceExists(prim->deviceAddr, instData->selectService->list))
    {
        selectServiceListStruct *newElement;
        newElement = CsrPmemZalloc(sizeof(selectServiceListStruct));
        newElement->deviceAddr = prim->deviceAddr;
        if (instData->selectService->listPtr)
        {
            instData->selectService->listPtr->next = newElement;
        }
        if (!instData->selectService->list)
        {
            instData->selectService->list = newElement;
        }
        instData->selectService->listPtr = newElement;
    }

    CsrPmemFree(prim->eirData);
}

static void sendCsrBtJsr82DaSelectServiceCfm(CsrSchedQid appHandle, CsrUint32 reqID,
    CsrBtDeviceAddr deviceAddr, CsrUint8 protocol, CsrUint16 chanPsm,
    CsrBtResultCode resultCode)
{
    CsrBtJsr82DaSelectServiceCfm *msg;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82DaSelectServiceCfm));
    msg->type = CSR_BT_JSR82_DA_SELECT_SERVICE_CFM;
    msg->reqID = reqID;
    msg->deviceAddr = deviceAddr;
    msg->protocol = protocol;
    msg->chanPsm = chanPsm;
    msg->resultCode = resultCode;

    CsrBtJsr82MessagePut(appHandle, msg);
}

void CsrBtJsr82CleanupSelectService(Jsr82MainInstanceData *instData)
{
    if (instData->selectService)
    {
        selectServiceListStruct *listPtr;
        for (listPtr = instData->selectService->list; listPtr;)
        {
            selectServiceListStruct *nextPtr;
            nextPtr = listPtr->next;
            CsrPmemFree(listPtr);
            listPtr = nextPtr;
        }

        CsrPmemFree(instData->selectService->handleList);
        CsrPmemFree(instData->selectService->uuid);
        CsrPmemFree(instData->selectService);
        instData->selectService = NULL;
    }
}

static void sendSearchRequest(Jsr82MainInstanceData *instData)
{
    selectServiceStruct *sel;

    sel = instData->selectService;

    if (sel->handleList)
    {
        CsrPmemFree(sel->handleList);
        sel->handleList = NULL;
    }

    switch (sel->uuidLength)
    {
        case 2:
        case 4:
        {
            CsrUint8 index, offset;
            CsrBtUuid32 *uuid = (CsrBtUuid32 *)CsrPmemAlloc(sizeof(CsrBtUuid32));
            *uuid = 0;

            for (index = 0, offset=(CsrUint8)(8*(sel->uuidLength - 1)); index < sel->uuidLength; index++, offset -= 8)
            {
                *uuid |= ((sel->uuid[index] & 0xFF) << offset);
            }
            CsrBtCmSdcSearchReqSend(CSR_BT_JSR82_IFACEQUEUE, sel->listPtr->deviceAddr, uuid, 1);
            break;
        }
        case 16:
        {
            CsrBtUuid128 *uuid128 = (CsrBtUuid128 *)CsrPmemAlloc(sizeof(CsrBtUuid128));
            CsrMemCpy(uuid128, sel->uuid, 16 * sizeof(CsrUint8));
            CsrBtCmSdcUuid128SearchReqSend(CSR_BT_JSR82_IFACEQUEUE, sel->listPtr->deviceAddr, uuid128, 1);
            break;
        }
        default:
        {
            CsrBtDeviceAddr dummyAddr = {0,0,0};
            sendCsrBtJsr82DaSelectServiceCfm(instData->appHandle,
                instData->reqID, dummyAddr, 0,0,
                CSR_BT_RESULT_CODE_JSR82_UNACCEPTABLE_PARAMETER);

            CsrBtJsr82CleanupSelectService(instData);
            CsrBtJsr82SetIdle(instData);
            return;
        }
    }
}

static void sendNextSearchRequest(Jsr82MainInstanceData *instData)
{
    selectServiceStruct *sel;
    sel = instData->selectService;
    sel->listPtr = sel->listPtr->next;
    if (sel->listPtr)
    {
        sendSearchRequest(instData);
    }
    else
    {
        CsrBtDeviceAddr dummyAddr = {0,0,0};
        sendCsrBtJsr82DaSelectServiceCfm(instData->appHandle, instData->reqID,
            dummyAddr, 0,0, CSR_BT_RESULT_CODE_JSR82_DEVICE_NOT_FOUND);
        CsrBtJsr82CleanupSelectService(instData);
        CsrBtJsr82SetIdle(instData);
    }
}

void CsrBtJsr82CmSelServiceDiscTimeout(CsrUint16 dummy, void *data)
{
    Jsr82MainInstanceData * instData = (Jsr82MainInstanceData *) data;

    instData->selectService->inquiryTimer = 0;

    CsrBtCmCancelInquiryReqSend(CSR_BT_JSR82_IFACEQUEUE);

    instData->selectService->listPtr = instData->selectService->list;
    if (instData->selectService->listPtr)
    {
        sendSearchRequest(instData);
    }
}

void CsrBtJsr82CmSelServiceDiscCfmHandler(Jsr82MainInstanceData *instData)
{
    /* CSR_BT_CM_INQUIRY_CFM will always indicate an error */
    CsrBtDeviceAddr dummyAddr = {0,0,0};
    sendCsrBtJsr82DaSelectServiceCfm(instData->appHandle, instData->reqID,
        dummyAddr, 0,0, CSR_BT_RESULT_CODE_JSR82_COMMAND_DISALLOWED);
    CsrBtJsr82CleanupSelectService(instData);
    CsrBtJsr82SetIdle(instData);
}

void CsrBtJsr82CmSelServiceSdc128searchIndHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmSdcUuid128SearchInd *prim;

    prim = (CsrBtCmSdcUuid128SearchInd *)instData->recvMsg;
    instData->selectService->handleList         = prim->serviceHandleList;
    instData->selectService->handleListLength   = prim->serviceHandleListCount;
    instData->selectService->handleListIndex    = 0;
}

void CsrBtJsr82CmSelServiceSdcSearchIndHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmSdcSearchInd *prim;

    prim = (CsrBtCmSdcSearchInd *)instData->recvMsg;
    instData->selectService->handleList         = prim->serviceHandleList;
    instData->selectService->handleListLength   = prim->serviceHandleListCount;
    instData->selectService->handleListIndex    = 0;
}

void CsrBtJsr82CmSelServiceSdcSearchCfmHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmSdcAttributeReqSend(  instData->selectService->handleList[instData->selectService->handleListIndex],
                            CSR_BT_SERVICE_AVAILABILITY_ATTRIBUTE_IDENTIFIER,
                            JSR82_ATTRIBUTE_MAX_BYTES);
    instData->selectService->currentAttribute = CSR_BT_SERVICE_AVAILABILITY_ATTRIBUTE_IDENTIFIER;
}

static void checkNextService(Jsr82MainInstanceData *instData)
{
    instData->selectService->handleListIndex++;
    if (instData->selectService->handleListIndex < instData->selectService->handleListLength)
    {
        CsrBtCmSdcAttributeReqSend(  instData->selectService->handleList[instData->selectService->handleListIndex],
                                CSR_BT_SERVICE_AVAILABILITY_ATTRIBUTE_IDENTIFIER,
                                JSR82_ATTRIBUTE_MAX_BYTES);
        instData->selectService->currentAttribute = CSR_BT_SERVICE_AVAILABILITY_ATTRIBUTE_IDENTIFIER;
    }
    else
    {
        if (instData->selectService->handleList)
        {
            CsrPmemFree(instData->selectService->handleList);
            instData->selectService->handleList     = NULL;
        }
        instData->selectService->handleListLength   = 0;
        instData->selectService->handleListIndex    = 0;
        CsrBtCmSdcCloseReqSend(CSR_BT_JSR82_IFACEQUEUE);
    }
}

void CsrBtJsr82CmSelServiceSdcAttributeCfmHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmSdcAttributeCfm *prim;

    prim = (CsrBtCmSdcAttributeCfm *)instData->recvMsg;

    switch (instData->selectService->currentAttribute)
    {
    case CSR_BT_SERVICE_AVAILABILITY_ATTRIBUTE_IDENTIFIER:
        {
            if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
            {
                CsrUint8 availability = 0xFF;

                if (CsrBtSdcGetUint8Direct(prim->attributeListSize, prim->attributeList, CSR_BT_SERVICE_AVAILABILITY_ATTRIBUTE_IDENTIFIER, &availability) &&
                    (availability == 0x00))
                {
                    checkNextService(instData);
                }
                else
                {
                    CsrBtCmSdcAttributeReqSend(  instData->selectService->handleList[instData->selectService->handleListIndex],
                                            CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER,
                                            JSR82_ATTRIBUTE_MAX_BYTES);
                    instData->selectService->currentAttribute = CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER;
                }
            }
            else if (prim->resultCode == SDC_NO_RESPONSE_DATA && prim->resultSupplier == CSR_BT_SUPPLIER_SDP_SDC)
            {
                    CsrBtCmSdcAttributeReqSend(instData->selectService->handleList[instData->selectService->handleListIndex],
                                            CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER,
                                            JSR82_ATTRIBUTE_MAX_BYTES);
                    instData->selectService->currentAttribute = CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER;
            }
            else
            {
                checkNextService(instData);
            }
            break;
        }
        case CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER:
        {
            if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
            {
                CsrUint16 chanPsm;
                CsrUint8 protocol = 0;

                if ((chanPsm = CsrBtSdcGetRfcommServerChannel(prim->attributeListSize, prim->attributeList)) != 0xff)
                {
                    protocol = CSR_BT_JSR82_PROTOCOL_RFCOMM;
                }
                else if ((chanPsm = CsrBtSdcGetL2capPsm(prim->attributeListSize, prim->attributeList)) != 0x0000)
                {
                    protocol = CSR_BT_JSR82_PROTOCOL_L2CAP;
                }

                if (protocol != 0)
                {
                    sendCsrBtJsr82DaSelectServiceCfm(instData->appHandle,
                        instData->reqID, prim->deviceAddr, protocol, chanPsm,
                        CSR_BT_RESULT_CODE_JSR82_SUCCESS);
                    CsrBtJsr82CleanupSelectService(instData);
                    CsrBtCmSdcCloseReqSend(CSR_BT_JSR82_IFACEQUEUE);
                        CsrBtCmSdcCloseReqSend(CSR_BT_JSR82_IFACEQUEUE);
                }
                else
                {
                    checkNextService(instData);
                }
            }
            else
            {
                checkNextService(instData);
            }
            break;
        }
        default:
        {
            break;
        }
    }
    CsrPmemFree(prim->attributeList);
}

void CsrBtJsr82CmSelServiceSdcCloseIndHandler(Jsr82MainInstanceData *instData)
{
    if (instData->selectService)
    {
        sendNextSearchRequest(instData);
    }
    else
    {
        CsrBtJsr82SetIdle(instData);
    }
}


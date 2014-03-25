/****************************************************************************

                (c) CSR plc 2008

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_sdc.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_sdc_support.h"

#define MAX_RECORDS_TO_RETURN       0x0040
#define MAX_BYTES_TO_RETURN         0x0070

static void delayedMessage(CsrUint16 appHdl, void *msg)
{
    CsrBtCmPutMessage(appHdl, msg);
}

static void CsrBtCmSdcSearchIndMsgSend(cmInstanceData_t *cmData, sdcSearchElement * theElement, CsrUint16 size)
{
    CsrBtUuid32  * serviceList;
    CsrUint32  allocatedSize = (sizeof(CsrBtUuid32) * size);


    cmData->sdcVar.validSearchResult = TRUE;

    serviceList   = theElement->searchPtrArray[cmData->sdcVar.currentIndex]->serviceHandle;

    if (theElement->serviceList)
    {
        CsrBtCmSdcSearchInd * prim;

        prim                        = (CsrBtCmSdcSearchInd *) CsrPmemAlloc(sizeof(CsrBtCmSdcSearchInd));
        prim->type                  = CSR_BT_CM_SDC_SEARCH_IND;
        prim->service               = theElement->serviceList[cmData->sdcVar.requestedServiceListSize];
        prim->serviceHandleListCount         = size;
        prim->localServerChannel    = cmData->sdcVar.localServer;
        prim->deviceAddr            = theElement->deviceAddr;
        prim->serviceHandleList     = (CsrBtUuid32 *) CsrPmemAlloc(allocatedSize);
        CsrMemCpy(prim->serviceHandleList, serviceList, allocatedSize);
        CsrBtCmPutMessage(theElement->appHandle, prim);
    }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
    else
    {
        CsrUintFast8                 t;
        CsrBtCmSdcUuid128SearchInd * prim;

        prim                    = (CsrBtCmSdcUuid128SearchInd *) CsrPmemAlloc(sizeof(CsrBtCmSdcUuid128SearchInd));
        prim->type              = CSR_BT_CM_SDC_UUID128_SEARCH_IND;

        for (t = 0; t < 16; t++)
        {
            prim->service[t]    = theElement->uuid128ServiceList[cmData->sdcVar.requestedServiceListSize][t];
        }
        prim->serviceHandleListCount         = size;
        prim->localServerChannel    = cmData->sdcVar.localServer;
        prim->deviceAddr            = theElement->deviceAddr;
        prim->serviceHandleList     = (CsrBtUuid32 *) CsrPmemAlloc(allocatedSize);
        CsrMemCpy(prim->serviceHandleList, serviceList, allocatedSize);
        CsrBtCmPutMessage(theElement->appHandle, prim);
    }
#endif
}

static void csrBtCmSdcServiceSearchCfmMsgSend(cmInstanceData_t *cmData, CsrBtUuid32 *recList, CsrUint16 recListLength,
                                              CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtCmSdcServiceSearchCfm *prim;

    prim        = (CsrBtCmSdcServiceSearchCfm *)CsrPmemAlloc(sizeof(CsrBtCmSdcServiceSearchCfm));
    prim->type  = CSR_BT_CM_SDC_SERVICE_SEARCH_CFM;

    if (recList)
    {
        CsrUint32  allocatedSize = (sizeof(CsrBtUuid32) * recListLength);
        prim->recList           = (CsrBtUuid32 *) CsrPmemAlloc(allocatedSize);
        CsrMemCpy(prim->recList, recList, allocatedSize);
    }
    else
    {
        prim->recList = NULL;
    }
    prim->recListLength             = recListLength;
    prim->resultCode                = resultCode;
    prim->resultSupplier            = resultSupplier;
    CsrBtCmPutMessage(cmData->sdcVar.currentElement->appHandle, prim);
}

static void CsrBtCmSdcSearchCfmMsgSend(CsrSchedQid appHandle, CsrUint8 server, CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmSdcSearchCfm  * prim;

    prim                        = (CsrBtCmSdcSearchCfm *) CsrPmemAlloc(sizeof(CsrBtCmSdcSearchCfm));
    prim->type                  = CSR_BT_CM_SDC_SEARCH_CFM;
    prim->localServerChannel    = server;
    prim->deviceAddr            = deviceAddr;
    CsrBtCmPutMessage(appHandle, prim);
}

static void csrBtCmSdcAttrCfmMsgSend(cmInstanceData_t *cmData)
{
    CsrBtCmSdcAttributeCfm          * prim;
    SDC_SERVICE_ATTRIBUTE_CFM_T     * sdcPrim;
    sdcSearchElement                * theElement = cmData->sdcVar.currentElement;

    sdcPrim                     = (SDC_SERVICE_ATTRIBUTE_CFM_T *) cmData->recvMsgP;
    prim                        = (CsrBtCmSdcAttributeCfm *) CsrPmemAlloc(sizeof(CsrBtCmSdcAttributeCfm));
    prim->type                  = CSR_BT_CM_SDC_ATTRIBUTE_CFM;
    prim->attributeList         = sdcPrim->attr_list;
    prim->attributeListSize     = sdcPrim->size_attr_list;
    prim->localServerChannel    = cmData->sdcVar.localServer;
    prim->deviceAddr            = theElement->deviceAddr;

    if (sdcPrim->response == SDC_RESPONSE_SUCCESS)
    {
        prim->resultCode     = CSR_BT_RESULT_CODE_CM_SUCCESS;
        prim->resultSupplier = CSR_BT_SUPPLIER_CM;
    }
    else
    {
        prim->resultCode     = (CsrBtResultCode) sdcPrim->response;
        prim->resultSupplier = CSR_BT_SUPPLIER_SDP_SDC;
    }
    CsrBtCmPutMessage(theElement->appHandle, prim);
}

void CsrBtCmSdcCloseIndMsgSend(CsrSchedQid appHandle, CsrUint8 server, CsrBtDeviceAddr deviceAddr,
                               CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtCmSdcCloseInd  * prim;
    prim                        = (CsrBtCmSdcCloseInd *) CsrPmemAlloc(sizeof(CsrBtCmSdcCloseInd));
    prim->type                  = CSR_BT_CM_SDC_CLOSE_IND;
    prim->resultCode            = resultCode;
    prim->resultSupplier        = resultSupplier;
    prim->localServerChannel    = server;
    prim->deviceAddr            = deviceAddr;
    CsrBtCmPutMessage(appHandle, prim);
}

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
static CsrBool getSdcElement(cmInstanceData_t *cmData, CsrSchedQid appHandle, CsrBtDeviceAddr deviceAddr)
{
    if (cmData->sdcVar.sdcSearchList)
    {
        sdcSearchElement    * theElement;
        theElement  = cmData->sdcVar.sdcSearchList;
        do
        {
            if (theElement->appHandle == appHandle && bd_addr_eq(&(theElement->deviceAddr), &deviceAddr))
            {
                cmData->sdcVar.currentElement = theElement;
                return TRUE;
            }
            theElement = theElement->nextSdcElement;
        } while (theElement);
    }
    return FALSE;
}

static void csrBtCmSdcReleaseResourcesCfmMsgSend(CsrSchedQid appHandle, CsrUint8 server, CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmSdcReleaseResourcesCfm  * prim;
    prim                        = (CsrBtCmSdcReleaseResourcesCfm *) CsrPmemAlloc(sizeof(CsrBtCmSdcReleaseResourcesCfm));
    prim->type                  = CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM;
    prim->localServerChannel    = server;
    prim->deviceAddr            = deviceAddr;
    CsrBtCmPutMessage(appHandle, prim);
}
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

static void csrBtCmSdcServiceSearchHandler(cmInstanceData_t *cmData)
{
    CsrUint16            i = 0;
    CsrUint8             * searchPtr;
    sdcSearchElement    * theElement;

    theElement      = cmData->sdcVar.currentElement;

    if (theElement->serviceList)
    {
        CsrBtUuid32              service;

        service         = theElement->serviceList[cmData->sdcVar.requestedServiceListSize];

        searchPtr       = (CsrUint8 *) CsrPmemAlloc(7);
        searchPtr[i++]  = 0x35;
        searchPtr[i++]  = 0x05;
        searchPtr[i++]  = 0x1a;
        searchPtr[i++]  = (CsrUint8) ((service    >> 24) & 0xff);
        searchPtr[i++]  = (CsrUint8) ((service    >> 16) & 0xff);
        searchPtr[i++]  = (CsrUint8) ((service    >> 8 ) & 0xff);
        searchPtr[i++]  = (CsrUint8) ((service         ) & 0xff);
    }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
    else if (theElement->uuid128ServiceList)
    {
        CsrUintFast8     t;

        searchPtr       = (CsrUint8 *) CsrPmemAlloc(19);
        searchPtr[i++]  = 0x35;
        searchPtr[i++]  = 0x11;
        searchPtr[i++]  = 0x1c;

        for (t = 0; t < 16; t++)
        {
            searchPtr[i++]  = (CsrUint8) theElement->uuid128ServiceList[cmData->sdcVar.requestedServiceListSize][t];
        }
    }
#endif
    else /* if (theElement->uuidSet) */
    {
        searchPtr = CsrPmemAlloc(sizeof(CsrUint8)*theElement->uuidSetLength);
        CsrMemCpy(searchPtr,theElement->uuidSet,sizeof(CsrUint8)*theElement->uuidSetLength);
        i = theElement->uuidSetLength;
    }
    sdc_service_search_req(CSR_BT_CM_IFACEQUEUE, &(theElement->deviceAddr), i,
                           searchPtr, MAX_RECORDS_TO_RETURN);
}

static void csrBtCmServiceSearchContinueOrDoneHandler(cmInstanceData_t *cmData, sdcSearchElement * theElement)
{
    if (cmData->sdcVar.requestedServiceListSize > 0)
    {
        cmData->sdcVar.requestedServiceListSize--;
        csrBtCmSdcServiceSearchHandler(cmData);
    }
    else
    {
        if (theElement->searchPtrArray[0] && cmData->sdcVar.validSearchResult)
        {
            CSR_BT_CM_STATE_CHANGE(cmData->sdcVar.state, sdcAttr_s);
            CsrBtCmSdcSearchCfmMsgSend(theElement->appHandle, cmData->sdcVar.localServer, theElement->deviceAddr);
        }
        else
        { /* The none of UUID's did match the UUID's defined under the Service
             Class ID List attribute                                            */
            cmData->smVar.resultCode     = (CsrBtResultCode) SDC_NO_RESPONSE_DATA;
            cmData->smVar.resultSupplier = CSR_BT_SUPPLIER_SDP_SDC;
            sdc_close_search_req(CSR_BT_CM_IFACEQUEUE);
        }
    }
}

static void csrBtCmSdcOpenCfmMsgSend(CsrSchedQid appHandle, CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmSdcOpenCfm   * prim;

    prim                        = (CsrBtCmSdcOpenCfm *) CsrPmemAlloc(sizeof(CsrBtCmSdcOpenCfm));
    prim->type                  = CSR_BT_CM_SDC_OPEN_CFM;
    prim->deviceAddr            = deviceAddr;
    CsrBtCmPutMessage(appHandle, prim);
}


static void csrBtCmSdcAttrRequestInSearchState(CsrBtDeviceAddr addr, CsrUint16 attrId, CsrBtUuid32 serviceHandle)
{
    CsrUint8 * attrList;

    attrList        = (CsrUint8 *) CsrPmemAlloc(5);
    attrList[0]     = 0x35;
    attrList[1]     = 0x03;
    attrList[2]     = 0x09;
    attrList[3]     = (CsrUint8) ((attrId & 0xff00) >> 8);
    attrList[4]     = (CsrUint8) (attrId & 0x00ff);
    sdc_service_attribute_req(CSR_BT_CM_IFACEQUEUE, &addr, serviceHandle, 5, attrList, MAX_BYTES_TO_RETURN);
}

static void csrBtCmSdcSearchResultHandler(cmInstanceData_t *cmData, sdcSearchElement * theElement)
{
    CsrUintFast16 i;
    sdcList_t   * searchList;

    CsrUint16    validResults    = 0;
    searchList                  = theElement->searchPtrArray[cmData->sdcVar.currentIndex];

    for ( i = 0; i < searchList->resultListSize; i++)
    {
        if (searchList->validResult[i])
        {
            if (i != validResults)
            {
                searchList->serviceHandle[validResults]     = searchList->serviceHandle[i];
                searchList->serverChannelList[validResults] = searchList->serverChannelList[i];
                searchList->validResult[validResults]       = TRUE;
            }
            validResults++;
        }
    }
    searchList->resultListSize  = validResults;

    if (validResults > 0)
    {
        CsrBtCmSdcSearchIndMsgSend(cmData, theElement, validResults);
    }
}


static void csrBtCmRetrieveAttrContinueOrDoneSearchState(cmInstanceData_t *cmData, sdcSearchElement  * theElement)
{
    if ((cmData->sdcVar.currentServiceIndex + 1) < theElement->searchPtrArray[cmData->sdcVar.currentIndex]->resultListSize)
    {
        CsrBtUuid32              serviceHandle;

        cmData->sdcVar.currentServiceIndex++;

        serviceHandle           = theElement->searchPtrArray[cmData->sdcVar.currentIndex]->serviceHandle[cmData->sdcVar.currentServiceIndex];
        cmData->sdcVar.attrId   = CSR_BT_SERVICE_CLASS_ID_LIST;
        csrBtCmSdcAttrRequestInSearchState(theElement->deviceAddr, cmData->sdcVar.attrId, serviceHandle);
    }
    else
    {
        csrBtCmSdcSearchResultHandler(cmData, theElement);
        cmData->sdcVar.currentIndex++;
        csrBtCmServiceSearchContinueOrDoneHandler(cmData, theElement);
    }
}

static void csrBtCmAttrCfmSuccessHandlerInSdcSearchState(cmInstanceData_t * cmData, SDC_SERVICE_ATTRIBUTE_CFM_T * prim)
{
    sdcSearchElement *theElement = cmData->sdcVar.currentElement;
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    CsrBtUuid32 serviceHandle = theElement->searchPtrArray[cmData->sdcVar.currentIndex]->serviceHandle[cmData->sdcVar.currentServiceIndex];
#endif

    if (cmData->sdcVar.attrId == CSR_BT_SERVICE_CLASS_ID_LIST)
    {
        CsrBool result = FALSE;
        if (theElement->serviceList)
        {
            result = CsrBtSdcFindServiceClassUuid(prim->size_attr_list, prim->attr_list, theElement->serviceList[cmData->sdcVar.requestedServiceListSize]);
        }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
        else
        {
            result = CsrBtSdcFindServiceClass128Uuid(prim->size_attr_list, prim->attr_list, theElement->uuid128ServiceList[cmData->sdcVar.requestedServiceListSize]);
        }
#endif
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
        if (result && theElement->obtainServerChannels)
        {
            if (theElement->extendedProtocolDescriptorList)
            {
                cmData->sdcVar.attrId   = CSR_BT_ADDITIONAL_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER;
            }
            else
            {
                cmData->sdcVar.attrId   = CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER;
            }
            csrBtCmSdcAttrRequestInSearchState(theElement->deviceAddr, cmData->sdcVar.attrId, serviceHandle);
        }
        else
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
        {
            theElement->searchPtrArray[cmData->sdcVar.currentIndex]->validResult[cmData->sdcVar.currentServiceIndex] = result;
            csrBtCmRetrieveAttrContinueOrDoneSearchState(cmData, theElement);
        }
    }
    else
    {
        CsrUint8   server;

        if (theElement->extendedProtocolDescriptorList)
        {
            server          = CsrBtSdcGetExtendedRfcommServerChannel(prim->size_attr_list, prim->attr_list);
        }
        else
        {
            server          = CsrBtSdcGetRfcommServerChannel(prim->size_attr_list, prim->attr_list);
        }
        if (server != CM_ERROR)
        {
            theElement->searchPtrArray[cmData->sdcVar.currentIndex]->serverChannelList[cmData->sdcVar.currentServiceIndex]  = server;
            theElement->searchPtrArray[cmData->sdcVar.currentIndex]->validResult[cmData->sdcVar.currentServiceIndex]        = TRUE;
        }
        csrBtCmRetrieveAttrContinueOrDoneSearchState(cmData, theElement);
    }
}

static void csrBtCmSdcAttrCfmInSdcSearchState(cmInstanceData_t * cmData, SDC_SERVICE_ATTRIBUTE_CFM_T * prim)
{
    cmData->smVar.resultCode     = (CsrBtResultCode) prim->response;
    cmData->smVar.resultSupplier = CSR_BT_SUPPLIER_SDP_SDC;

    switch (prim->response)
    {
        case SDC_RESPONSE_SUCCESS:
        {
            csrBtCmAttrCfmSuccessHandlerInSdcSearchState(cmData, prim);
            break;
        }
        case SDC_CON_DISCONNECTED:
        case SDC_CONNECTION_ERROR_UNKNOWN:
        case SDC_CONFIGURE_ERROR:
        case SDC_RESPONSE_TIMEOUT_ERROR:
        {
            sdc_close_search_req(CSR_BT_CM_IFACEQUEUE);
            break;
        }
        default :
        {
            csrBtCmRetrieveAttrContinueOrDoneSearchState(cmData, cmData->sdcVar.currentElement);
            break;
        }
    }
}

static CsrBool insertSdcElement(cmInstanceData_t *cmData, CsrSchedQid appHandle, CsrBtDeviceAddr deviceAddr,
                               CsrBtUuid32 * serviceList, CsrUint8 serviceListSize, CsrBool obtainServerChannel, CsrBtUuid128 * uuid128ServiceList,
                               CsrUint8 *uuidSet, CsrUint16 uuidSetLength, CsrBool readAttrDirect, CsrBool extendedProtocolList, CsrBool extendedUuidSearch)
{
    sdcSearchElement    * theElement;

    if (cmData->sdcVar.sdcSearchList)
    {
        sdcSearchElement    * previousElement;

        theElement      = cmData->sdcVar.sdcSearchList;
        do
        {
            if (theElement->appHandle == appHandle && bd_addr_eq(&(theElement->deviceAddr), &deviceAddr))
            {
                return FALSE;
            }
            previousElement = theElement;
            theElement      = theElement->nextSdcElement;
        } while (theElement);
        theElement                      = (sdcSearchElement *) CsrPmemAlloc(sizeof(sdcSearchElement));
        previousElement->nextSdcElement = theElement;
    }
    else
    {
        cmData->sdcVar.sdcSearchList = (sdcSearchElement *) CsrPmemAlloc(sizeof(sdcSearchElement));
        theElement                   = cmData->sdcVar.sdcSearchList;
    }

    theElement->searchPtrArray              = (sdcList_t **) CsrPmemAlloc(serviceListSize * sizeof(sdcList_t *));
    theElement->numOfSearchPtr              = serviceListSize;
    CsrMemSet(theElement->searchPtrArray, 0, sizeof(sdcList_t *) * serviceListSize);
    theElement->readAttrDirect              = readAttrDirect;
    theElement->nextSdcElement              = NULL;
    theElement->dmOpenResult                = FALSE;
    theElement->serviceList                 = serviceList;
    theElement->uuid128ServiceList          = uuid128ServiceList;
    theElement->uuidSet                     = uuidSet;
    theElement->uuidSetLength               = uuidSetLength;
    theElement->obtainServerChannels        = obtainServerChannel;
    theElement->appHandle                   = appHandle;
    theElement->deviceAddr                  = deviceAddr;
    theElement->extendedProtocolDescriptorList = extendedProtocolList;
    theElement->extendedUuidSearch          = extendedUuidSearch;
    cmData->sdcVar.requestedServiceListSize = (CsrUint8)(serviceListSize - 1);
    cmData->sdcVar.currentElement           = theElement;
    cmData->sdcVar.currentIndex             = 0;
    return TRUE;
}

static void searchRequestHandler(cmInstanceData_t *cmData, CsrSchedQid appHandle, CsrBtDeviceAddr deviceAddr,
                                 CsrBtUuid32 * serviceList, CsrUint8 serviceListSize, CsrBool obtainServerChannel, CsrBtUuid128 * uuid128ServiceList,
                                 CsrUint8 *uuidSet, CsrUint16 uuidSetLength, CsrBool readAttrDirect, CsrBool extendedProtocolList, CsrBool extendedUuidSearch)
{
    if (insertSdcElement(cmData, appHandle, deviceAddr, serviceList, serviceListSize,
        obtainServerChannel, uuid128ServiceList, uuidSet, uuidSetLength, readAttrDirect, extendedProtocolList, extendedUuidSearch))
    {
        CsrUint8  numOfConnection         = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, deviceAddr);
        cmData->sdcVar.validSearchResult = FALSE;
        cmData->sdcVar.cancelPending     = FALSE;

        if(numOfConnection == 0)
        { /* Currently there is no other connections attach to the given Bluetooth
             address. Start open the SDC channel */
            CsrUint16 clockOffset                    = 0;
            page_scan_mode_t pageScanMode           = 0;
            page_scan_rep_mode_t pageScanRepMode    = 0;

            if(isCacheParamsKnown(cmData, deviceAddr, &clockOffset, &pageScanMode, &pageScanRepMode))
            {
                CSR_BT_CM_STATE_CHANGE(cmData->sdcVar.state, sdcSearchInit_s);
                /* start writing cache params. via DM queue */
                CsrBtCmDmWriteCacheParamsReqSend(SDC_PLAYER, deviceAddr, clockOffset, pageScanMode, pageScanRepMode);
            }
            else
            {
                CsrBtCmSdcStartHandler(cmData, deviceAddr);
            }
        }
        else
        { /* Currently there is at least one more connection attach to the given
             device address. Request Active mode. */
            CSR_BT_CM_STATE_CHANGE(cmData->sdcVar.state, sdcSearchInit_s);
            cmData->sdcVar.sdcMsgPending    = FALSE;
            CsrBtCmDmModeChangeReqMsgSend(SDC_PLAYER, 0, CSR_BT_ACTIVE_MODE, deviceAddr);
        }
    }
    else
    {
        CsrPmemFree(serviceList);
        CsrPmemFree(uuid128ServiceList);
        CsrPmemFree(uuidSet);
        CSR_BT_CM_STATE_CHANGE(cmData->sdcVar.state, idle_s);
        CsrBtCmSdcCloseIndMsgSend(appHandle,
                                  cmData->sdcVar.localServer,
                                  deviceAddr,
                                  CSR_BT_RESULT_CODE_CM_COMMAND_DISALLOWED,
                                  CSR_BT_SUPPLIER_CM);

        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

static void csrBtCmServiceSearchSuccessHandler(cmInstanceData_t *cmData, SDC_SERVICE_SEARCH_CFM_T * prim)
{
    sdcSearchElement * theElement = cmData->sdcVar.currentElement;
    if (cmData->sdcVar.currentIndex < theElement->numOfSearchPtr)
    {
        CsrUintFast16         i, offset = 0;
        theElement->searchPtrArray[cmData->sdcVar.currentIndex]                     = (sdcList_t *) CsrPmemAlloc(sizeof(sdcList_t));
        theElement->searchPtrArray[cmData->sdcVar.currentIndex]->serviceHandle      = (CsrBtUuid32 *) CsrPmemAlloc((sizeof(CsrBtUuid32) * prim->num_recs_ret));
        theElement->searchPtrArray[cmData->sdcVar.currentIndex]->serverChannelList  = (CsrUint8 *) CsrPmemAlloc((sizeof(CsrUint8) * prim->num_recs_ret));
        theElement->searchPtrArray[cmData->sdcVar.currentIndex]->validResult        = (CsrBool *) CsrPmemAlloc((sizeof(CsrBool) * prim->num_recs_ret));
        theElement->searchPtrArray[cmData->sdcVar.currentIndex]->resultListSize     = prim->num_recs_ret;

        for(i = 0; i < prim->num_recs_ret; i++)
        {
            theElement->searchPtrArray[cmData->sdcVar.currentIndex]->serviceHandle[i] = CSR_GET_UINT32_FROM_BIG_ENDIAN(&prim->rec_list[offset]);
            offset += 4;
            theElement->searchPtrArray[cmData->sdcVar.currentIndex]->validResult[i]         = theElement->extendedUuidSearch;
            theElement->searchPtrArray[cmData->sdcVar.currentIndex]->serverChannelList[i]   = CSR_BT_NO_SERVER;
        }
        if (theElement->uuidSet)
        {
            csrBtCmSdcServiceSearchCfmMsgSend(cmData, theElement->searchPtrArray[cmData->sdcVar.currentIndex]->serviceHandle, prim->num_recs_ret, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
            cmData->sdcVar.currentIndex++;
            CSR_BT_CM_STATE_CHANGE(cmData->sdcVar.state, sdcAttr_s);
        }
        else
        {
            if (!theElement->extendedUuidSearch)
            { /* The UUID must only be consider for valid, if and only if the
                 UUID is contained under the Service Class ID List attribute    */
                cmData->sdcVar.currentServiceIndex  = 0;
                cmData->sdcVar.attrId               = CSR_BT_SERVICE_CLASS_ID_LIST;
                csrBtCmSdcAttrRequestInSearchState(theElement->deviceAddr, cmData->sdcVar.attrId,
                                                   theElement->searchPtrArray[cmData->sdcVar.currentIndex]->serviceHandle[0]);
            }
            else
            { /* The UUID must be consider valid, if the UUID is contained
                 within any of the service record's attribute values            */
                csrBtCmSdcSearchResultHandler(cmData, theElement);
                cmData->sdcVar.currentIndex++;
                csrBtCmServiceSearchContinueOrDoneHandler(cmData, theElement);
            }
        }
    }
    else
    {
        sdc_close_search_req(CSR_BT_CM_IFACEQUEUE);
    }
}

static void csrBtCmSdcCancelledHandler(cmInstanceData_t *cmData)
{
    CsrBtDeviceAddr deviceAddr    = cmData->sdcVar.currentElement->deviceAddr;
    cmData->sdcVar.cancelPending  = FALSE;
    CsrBtCmSdcCloseIndMsgSend(cmData->sdcVar.currentElement->appHandle,
                              cmData->sdcVar.localServer,
                              deviceAddr,
                              CSR_BT_RESULT_CODE_CM_CANCELLED,
                              CSR_BT_SUPPLIER_CM);

    removeElementFromSdcList(cmData);
    CsrBtCmModeChangeReqMsgSend(cmData, deviceAddr);
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
}
static void freeSdcSearchElement(sdcSearchElement    * theElement)
{
   CsrUintFast16 i;
   for (i = 0; i < theElement->numOfSearchPtr; i++)
    {
        if (theElement->searchPtrArray[i])
        {
            CsrPmemFree(theElement->searchPtrArray[i]->serviceHandle);
            CsrPmemFree(theElement->searchPtrArray[i]->serverChannelList);
            CsrPmemFree(theElement->searchPtrArray[i]->validResult);
            CsrPmemFree(theElement->searchPtrArray[i]);
        }
    }
    CsrPmemFree(theElement->searchPtrArray);
    CsrPmemFree(theElement->serviceList);
    CsrPmemFree(theElement->uuidSet);
    CsrPmemFree(theElement->uuid128ServiceList);
    CsrPmemFree(theElement);
}
void removeElementFromSdcList(cmInstanceData_t * cmData)
{
    sdcSearchElement    * tempPtr;
    sdcSearchElement    * theElement;

    theElement  = cmData->sdcVar.sdcSearchList;

    if (theElement == cmData->sdcVar.currentElement)
    {
        tempPtr = theElement->nextSdcElement;
        freeSdcSearchElement(theElement);
        cmData->sdcVar.sdcSearchList = tempPtr;
    }
    else
    {
        tempPtr = cmData->sdcVar.currentElement;
        do
        {
            if (theElement->nextSdcElement == tempPtr)
            {
                theElement->nextSdcElement = tempPtr->nextSdcElement;
                freeSdcSearchElement(tempPtr);
                return;
            }
            theElement = theElement->nextSdcElement;
        }while (theElement);
    }
}

void CsrBtCmSdcStartHandler(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr)
{
    if (!cmData->sdcVar.cancelPending)
    {
        CSR_BT_CM_STATE_CHANGE(cmData->sdcVar.state, sdcSearch_s);
        cmData->sdcVar.sdcMsgPending    = TRUE;
        sdc_open_search_req(CSR_BT_CM_IFACEQUEUE, &deviceAddr);
    }
    else
    {
        csrBtCmSdcCancelledHandler(cmData);
    }
}



void CsrBtCmSdcSearchReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSdcSearchReq * prim;

    prim = (CsrBtCmSdcSearchReq *) cmData->recvMsgP;
    cmData->sdcVar.localServer  = CSR_BT_NO_SERVER;

    searchRequestHandler(cmData, prim->appHandle, prim->deviceAddr, prim->serviceList,
                         prim->serviceListSize, FALSE, NULL, NULL, 0, FALSE, FALSE, prim->extendedUuidSearch);
}

#ifdef CSR_BT_INSTALL_CM_PRI_SDC
void CsrBtCmSdcServiceSearchReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSdcServiceSearchReq * prim;

    prim = (CsrBtCmSdcServiceSearchReq *) cmData->recvMsgP;
    cmData->sdcVar.localServer  = CSR_BT_NO_SERVER;

    searchRequestHandler(cmData, prim->appHandle, prim->deviceAddr, NULL,
                         1, FALSE, NULL, prim->uuidSet, prim->uuidSetLength, FALSE, FALSE, FALSE);
}
#endif

#ifdef CSR_BT_INSTALL_CM_PRI_SDC
void CsrBtCmSdcOpenReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSdcOpenReq   * prim;

    prim = (CsrBtCmSdcOpenReq *) cmData->recvMsgP;

    cmData->sdcVar.localServer  = CSR_BT_NO_SERVER;
    searchRequestHandler(cmData, prim->appHandle, prim->deviceAddr, NULL,
                         1, FALSE, NULL, NULL, 0, TRUE, FALSE, FALSE);

}
#endif
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
void CsrBtCmSdcUuid128SearchReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSdcUuid128SearchReq * prim;

    prim = (CsrBtCmSdcUuid128SearchReq *) cmData->recvMsgP;
    cmData->sdcVar.localServer  = CSR_BT_NO_SERVER;
    searchRequestHandler(cmData, prim->appHandle, prim->deviceAddr, NULL,
                         prim->serviceListSize, FALSE, prim->serviceList, NULL, 0, FALSE, FALSE, FALSE);
}
#endif
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
void CsrBtCmSdcRfcSearchReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSdcRfcSearchReq * prim;

    prim = (CsrBtCmSdcRfcSearchReq *) cmData->recvMsgP;
    cmData->sdcVar.localServer  = prim->localServerChannel;
    searchRequestHandler(cmData, prim->appHandle, prim->deviceAddr, prim->serviceList,
                         prim->serviceListSize, TRUE, NULL, NULL, 0, FALSE, FALSE, FALSE);
}

#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
void CsrBtCmSdcUuid128RfcSearchReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSdcUuid128RfcSearchReq * prim;

    prim = (CsrBtCmSdcUuid128RfcSearchReq *) cmData->recvMsgP;
    cmData->sdcVar.localServer  = prim->localServerChannel;
    searchRequestHandler(cmData, prim->appHandle, prim->deviceAddr, NULL,
                         prim->serviceListSize, TRUE, prim->serviceList, NULL, 0, FALSE, FALSE, FALSE);
}
#endif
void CsrBtCmSdcRfcExtendedSearchReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSdcRfcExtendedSearchReq    * prim;

    prim = (CsrBtCmSdcRfcExtendedSearchReq *) cmData->recvMsgP;
    cmData->sdcVar.localServer  = prim->localServerChannel;
    searchRequestHandler(cmData, prim->appHandle, prim->deviceAddr, prim->serviceList,
                         prim->serviceListSize, TRUE, NULL, NULL, 0, FALSE, TRUE, FALSE);
}


void CsrBtCmSdcReleaseResourcesReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSdcReleaseResourcesReq  * prim;

    prim                        = (CsrBtCmSdcReleaseResourcesReq *) cmData->recvMsgP;

    if (getSdcElement(cmData, prim->appHandle, prim->deviceAddr))
    {
        if (cmData->sdcVar.currentElement->dmOpenResult)
        {
            TYPED_BD_ADDR_T ad;
            ad.addr = prim->deviceAddr;
            ad.type = CSR_BT_ADDR_PUBLIC;
            dm_acl_close_req(&ad,
                             0, /* flags */
                             0, /* reason */
                             NULL);
        }
        removeElementFromSdcList(cmData);
        CsrBtCmModeChangeReqMsgSend(cmData, prim->deviceAddr);
    }
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
    csrBtCmSdcReleaseResourcesCfmMsgSend(prim->appHandle, prim->localServerChannel, prim->deviceAddr);
}
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

void CsrBtCmSdcCancelSearchReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSdcCancelSearchReq  * prim;

    prim                        = (CsrBtCmSdcCancelSearchReq *) cmData->recvMsgP;

    if (cmData->sdcVar.sdcSearchList && cmData->smVar.smInProgress && cmData->smVar.smMsgTypeInProgress == prim->typeToCancel)
    {
        if (cmData->sdcVar.state != idle_s)
        { /* CM is in the middle of a sdc search procedure */
            sdcSearchElement        * theElement;

            theElement  = cmData->sdcVar.currentElement;

            if (theElement->appHandle == prim->appHandle && bd_addr_eq(&(theElement->deviceAddr), &prim->deviceAddr))
            { /* The the ongoing sdc search procedure is the one the
                 application trying to cancel */
                cmData->sdcVar.cancelPending     = TRUE;
                CsrBtCmScRejectedForSecurityReasonMsgSend(cmData,
                                                          prim->deviceAddr, TRUE);

                switch (cmData->sdcVar.state)
                {
                    case sdcSearch_s:
                    {
                        if (cmData->sdcVar.sdcMsgPending)
                        { /* Must be a SDC open req */
                            CsrBtCmDmCancelPageOrDetach(cmData, prim->deviceAddr);
                        }
                        else
                        { /* A ACL connection is establish, must wait until CM
                             receives a SDC_X_CFM */
                            ;
                        }
                        break;
                    }
                    case sdcAttr_s:
                    {
                        if (!cmData->sdcVar.sdcMsgPending)
                        {
                            sdc_close_search_req(CSR_BT_CM_IFACEQUEUE);
                        }
                        else
                        { /* A ACL connection is establish, must wait until CM
                             receives a SDC_ATTRIBUTE_CFM */
                            ;
                        }
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            else
            { /* The the ongoing sdc search procedure is not the one the
                 application trying to cancel */
                CsrUint8 localServer;
                CsrUint16     dummy;
                if(cancelServiceManagerMsg(cmData, prim->typeToCancel, prim->appHandle, prim->deviceAddr, CSR_BT_NO_SERVER, NO_LOCAL_PSM, &localServer, &dummy))
                { /* The SDC search request msg is found and removed from the
                     local SM queue */
                    CsrBtCmSdcCloseIndMsgSend(prim->appHandle,
                                              localServer,
                                              prim->deviceAddr,
                                              CSR_BT_RESULT_CODE_CM_CANCELLED,
                                              CSR_BT_SUPPLIER_CM);
                }
                else
                { /* Nothing to cancel just ignore */
                    ;
                }
            }
        }
        else
        { /* Nothing to cancel just ignore */
            ;
        }
    }
    else
    {
        CsrUint8 localServer;
        CsrUint16     dummy;
        if(cancelServiceManagerMsg(cmData, prim->typeToCancel, prim->appHandle, prim->deviceAddr, CSR_BT_NO_SERVER, NO_LOCAL_PSM, &localServer, &dummy))
        { /* The SDC search request msg is found and removed from the local
             SM queue */
            CsrBtCmSdcCloseIndMsgSend(prim->appHandle,
                                      localServer,
                                      prim->deviceAddr,
                                      CSR_BT_RESULT_CODE_CM_CANCELLED,
                                      CSR_BT_SUPPLIER_CM);
        }
        else
        { /* Nothing to cancel just ignore */
            ;
        }
    }
}

void CsrBtCmSdcAttrReqHandle(cmInstanceData_t *cmData)
{
    if (cmData->sdcVar.sdcSearchList && cmData->sdcVar.state == sdcAttr_s && !cmData->sdcVar.cancelPending)
    {
        CsrUint8                 attrListSize;
        CsrUint8                 *attrList;
        CsrUint8                 dataElementSize;
        CsrBtCmSdcAttributeReq   *prim       = (CsrBtCmSdcAttributeReq *) cmData->recvMsgP;
        sdcSearchElement         *theElement = cmData->sdcVar.currentElement;

        if (prim->upperRangeAttributeIdentifier == 0)
        {
            attrListSize    = 5;
            dataElementSize = CSR_BT_DATA_ELEMENT_SIZE_2_BYTES;
        }
        else
        {
            attrListSize    = 7;
            dataElementSize = CSR_BT_DATA_ELEMENT_SIZE_4_BYTES;
        }


        cmData->sdcVar.sdcMsgPending   = TRUE;

        attrList    = (CsrUint8 *) CsrPmemAlloc(attrListSize);
        attrList[0] = 0x35;
        attrList[1] = (CsrUint8)(attrListSize - 2);
        attrList[2] = (CsrUint8)(CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE + dataElementSize);
        attrList[3] = (CsrUint8) ((prim->attributeIdentifier & 0xff00) >> 8);
        attrList[4] = (CsrUint8) (prim->attributeIdentifier & 0x00ff);
        if (attrListSize == 7)
        {
            attrList[5] = (CsrUint8) ((prim->upperRangeAttributeIdentifier & 0xff00) >> 8);
            attrList[6] = (CsrUint8) (prim->upperRangeAttributeIdentifier & 0x00ff);
        }
        sdc_service_attribute_req(CSR_BT_CM_IFACEQUEUE,
                                  &theElement->deviceAddr,
                                  prim->serviceHandle,
                                  attrListSize,
                                  attrList,
                                  (CsrUint16) (cmData->rfcBuild ? MAX_BYTES_TO_RETURN : prim->maxBytesToReturn));/* Limit size if we are running on an RFC build */
    }
}

void CsrBtCmSdcCloseReqHandler(cmInstanceData_t *cmData)
{
    if (cmData->sdcVar.sdcSearchList && cmData->sdcVar.state == sdcAttr_s && !cmData->sdcVar.cancelPending)
    {
        CSR_BT_CM_STATE_CHANGE(cmData->sdcVar.state, sdcClose_s);
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
        if (cmData->sdcVar.currentElement->obtainServerChannels)
        {
            cmData->sdcVar.aclOpenRequested = TRUE;
            CsrBtCmScDmAclOpenReq(CSR_BT_CM_IFACEQUEUE, cmData->sdcVar.currentElement->deviceAddr);
        }
        else
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
        {
            sdc_close_search_req(CSR_BT_CM_IFACEQUEUE);
        }
    }
}

void CsrBtCmSdcOpenSearchCfmHandler(cmInstanceData_t *cmData)
{
    SDC_OPEN_SEARCH_CFM_T * prim;

    prim        = (SDC_OPEN_SEARCH_CFM_T *) cmData->recvMsgP;

    cmData->sdcVar.sdcMsgPending   = FALSE;

    CsrBtCmDmUpdateAndClearCachedParamReqSend(cmData->sdcVar.currentElement->deviceAddr);
    if (prim->result == SDC_OPEN_SEARCH_OK)
    {
        if (cmData->sdcVar.state == sdcSearch_s && !cmData->sdcVar.cancelPending)
        {
            sdcSearchElement    * theElement;

            theElement = cmData->sdcVar.currentElement;

            if (theElement->readAttrDirect)
            {
                CSR_BT_CM_STATE_CHANGE(cmData->sdcVar.state, sdcAttr_s);
                csrBtCmSdcOpenCfmMsgSend(theElement->appHandle, theElement->deviceAddr);
            }
            else
            {
                csrBtCmSdcServiceSearchHandler(cmData);
            }
        }
        else
        {
            sdc_close_search_req(CSR_BT_CM_IFACEQUEUE);
        }
    }
    else
    {
        CsrBtDeviceAddr deviceAddr       = cmData->sdcVar.currentElement->deviceAddr;
        CSR_BT_CM_STATE_CHANGE(cmData->sdcVar.state, idle_s);
        if (cmData->sdcVar.cancelPending)
        {
            cmData->sdcVar.cancelPending  = FALSE;
            CsrBtCmSdcCloseIndMsgSend(cmData->sdcVar.currentElement->appHandle,
                                      cmData->sdcVar.localServer,
                                      deviceAddr,
                                      CSR_BT_RESULT_CODE_CM_CANCELLED,
                                      CSR_BT_SUPPLIER_CM);
        }
        else
        {
            CsrBtCmSdcCloseIndMsgSend(cmData->sdcVar.currentElement->appHandle,
                                      cmData->sdcVar.localServer,
                                      deviceAddr,
                                      (CsrBtResultCode) prim->result,
                                      CSR_BT_SUPPLIER_SDP_SDC_OPEN_SEARCH);
        }
        removeElementFromSdcList(cmData);
        CsrBtCmModeChangeReqMsgSend(cmData, deviceAddr);
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

void CsrBtCmSdcServiceSearchCfmHandler(cmInstanceData_t *cmData)
{ /* This event returns the results of a service search request */
    SDC_SERVICE_SEARCH_CFM_T    * prim;

    prim        = (SDC_SERVICE_SEARCH_CFM_T *) cmData->recvMsgP;

    if (cmData->sdcVar.sdcSearchList)
    {
        if (cmData->sdcVar.cancelPending)
        {
            sdc_close_search_req(CSR_BT_CM_IFACEQUEUE);
        }
        else
        {
        switch (cmData->sdcVar.state)
        {
            case sdcSearch_s:
            {
                cmData->smVar.resultCode     = (CsrBtResultCode) prim->response;
                cmData->smVar.resultSupplier = CSR_BT_SUPPLIER_SDP_SDC;

                switch (prim->response)
                {
                    case SDC_RESPONSE_SUCCESS :
                    {
                        csrBtCmServiceSearchSuccessHandler(cmData, prim);
                        break;
                    }
                    case SDC_CON_DISCONNECTED:
                    case SDC_CONNECTION_ERROR_UNKNOWN:
                    case SDC_CONFIGURE_ERROR:
                    case SDC_RESPONSE_TIMEOUT_ERROR:
                    {
                        if (cmData->sdcVar.currentElement->uuidSet)
                        {
                            csrBtCmSdcServiceSearchCfmMsgSend(cmData, NULL, 0, (CsrBtResultCode) prim->response, CSR_BT_SUPPLIER_SDP_SDC);
                        }
                        sdc_close_search_req(CSR_BT_CM_IFACEQUEUE);
                        break;
                    }
                    default:
                    {
                        if (cmData->sdcVar.currentElement->uuidSet)
                        {
                            csrBtCmSdcServiceSearchCfmMsgSend(cmData, NULL, 0, (CsrBtResultCode) prim->response, CSR_BT_SUPPLIER_SDP_SDC);
                            sdc_close_search_req(CSR_BT_CM_IFACEQUEUE);
                        }
                        else
                        {
                            csrBtCmServiceSearchContinueOrDoneHandler(cmData, cmData->sdcVar.currentElement);
                        }
                    }
                }
                break;
            }
            default :
            {
                sdc_close_search_req(CSR_BT_CM_IFACEQUEUE);
                break;
            }
        }
    }
    }
    CsrPmemFree(prim->rec_list);
    CsrPmemFree (prim->err_info);
}

void CsrBtCmSdcServiceAttributeCfmHandler(cmInstanceData_t * cmData)
{/* Returns the results of a service attribute request */
    SDC_SERVICE_ATTRIBUTE_CFM_T * prim;

    prim    = (SDC_SERVICE_ATTRIBUTE_CFM_T *) cmData->recvMsgP;

    cmData->sdcVar.sdcMsgPending   = FALSE;
    if (cmData->sdcVar.sdcSearchList)
    {
        if (cmData->sdcVar.cancelPending)
        {
            sdc_close_search_req(CSR_BT_CM_IFACEQUEUE);
            CsrPmemFree(prim->attr_list);
        }
        else
        {
        switch (cmData->sdcVar.state)
        {
            case sdcSearch_s :
            {
                csrBtCmSdcAttrCfmInSdcSearchState(cmData, prim);
                CsrPmemFree(prim->attr_list);
                break;
            }
            case sdcAttr_s :
            {
                csrBtCmSdcAttrCfmMsgSend(cmData);
                break;
            }
                default :
            {
                CsrPmemFree(prim->attr_list);
                break;
            }
            }
        }
    }
    else
    {
        CsrPmemFree(prim->attr_list);
    }
    CsrPmemFree(prim->err_info);
}

void CsrBtCmSdcCloseSearchIndHandler(cmInstanceData_t * cmData)
{
    if (cmData->sdcVar.sdcSearchList)
    {
        sdcSearchElement * theElement = cmData->sdcVar.currentElement;
        CsrBtDeviceAddr deviceAddr         = theElement->deviceAddr;

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
        if (theElement->obtainServerChannels && cmData->sdcVar.state == sdcClose_s)
        {
            if (cmData->sdcVar.cancelPending)
            {
                if (theElement->dmOpenResult)
                {
                    TYPED_BD_ADDR_T ad;
                    ad.addr = deviceAddr;
                    ad.type = CSR_BT_ADDR_PUBLIC;
                    dm_acl_close_req(&ad,
                                     0, /* flags */
                                     0, /* reason */
                                     NULL);
                }
                cmData->sdcVar.cancelPending = FALSE;
                CsrBtCmSdcCloseIndMsgSend(theElement->appHandle,
                                          cmData->sdcVar.localServer,
                                          deviceAddr,
                                          CSR_BT_RESULT_CODE_CM_CANCELLED,
                                          CSR_BT_SUPPLIER_CM);

                removeElementFromSdcList(cmData);
                CsrBtCmModeChangeReqMsgSend(cmData, deviceAddr);
            }
            else
            {
                CsrBtCmSdcCloseIndMsgSend(theElement->appHandle,
                                          cmData->sdcVar.localServer,
                                          deviceAddr,
                                          CSR_BT_RESULT_CODE_CM_SUCCESS,
                                          CSR_BT_SUPPLIER_CM);
            }
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
        else
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
        {
            CsrBtResultCode     resultCode;
            CsrBtSupplier resultSupplier;

            if (cmData->sdcVar.cancelPending)
            {
                cmData->sdcVar.cancelPending = FALSE;
                resultCode                   = CSR_BT_RESULT_CODE_CM_CANCELLED;
                resultSupplier               = CSR_BT_SUPPLIER_CM;
            }
            else
            {
                if (cmData->sdcVar.state == sdcClose_s)
                {
                    resultCode     = CSR_BT_RESULT_CODE_CM_SUCCESS;
                    resultSupplier = CSR_BT_SUPPLIER_CM;
                }
                else if (cmData->sdcVar.state == sdcSearch_s)
                {
                    if (cmData->smVar.resultCode == SDC_RESPONSE_SUCCESS &&
                        cmData->smVar.resultSupplier == CSR_BT_SUPPLIER_SDP_SDC)
                    {
                        resultCode     = CSR_BT_RESULT_CODE_CM_SUCCESS;
                        resultSupplier = CSR_BT_SUPPLIER_CM;
                    }
                    else
                    {
                        resultCode      = cmData->smVar.resultCode;
                        resultSupplier  = cmData->smVar.resultSupplier;
                    }
                }
                else
                {
                    resultCode     = CSR_BT_RESULT_CODE_CM_UNSPECIFIED_ERROR;
                    resultSupplier = CSR_BT_SUPPLIER_CM;
                }
            }
            CsrBtCmSdcCloseIndMsgSend(theElement->appHandle,
                                      cmData->sdcVar.localServer,
                                      deviceAddr,
                                      resultCode,
                                      resultSupplier);

            removeElementFromSdcList(cmData);
            CsrBtCmModeChangeReqMsgSend(cmData, deviceAddr);
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
        CSR_BT_CM_STATE_CHANGE(cmData->sdcVar.state, idle_s);
    }
}

void CsrBtCmSdcDecAclRefCountTo(cmInstanceData_t * cmData, CsrBtDeviceAddr deviceAddr)
{
    if (cmData->sdcVar.sdcSearchList)
    {
        sdcSearchElement    *theElement;
        theElement = cmData->sdcVar.sdcSearchList;
        do
        {
            if (CsrBtBdAddrEq(&theElement->deviceAddr, &deviceAddr))
            {
                theElement->dmOpenResult = FALSE;
            }
            theElement = theElement->nextSdcElement;
        } while (theElement);
    }
}

void CsrBtCmDmAclOpenCfmInSdcCloseStateHandler(cmInstanceData_t * cmData)
{
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    DM_ACL_OPEN_CFM_T * prim;

    prim    = (DM_ACL_OPEN_CFM_T *) cmData->recvMsgP;
    if (cmData->sdcVar.aclOpenRequested &&
        cmData->sdcVar.sdcSearchList &&
        bd_addr_eq(&(cmData->sdcVar.currentElement->deviceAddr),
                   &prim->addrt.addr))
    {
        cmData->sdcVar.aclOpenRequested = FALSE;
        if (prim->success == TRUE)
        {
            cmData->sdcVar.currentElement->dmOpenResult = TRUE;
        }
        sdc_close_search_req(CSR_BT_CM_IFACEQUEUE);
        CsrBtCmDmLocalQueueHandler();
        CsrPmemFree(cmData->recvMsgP);
        cmData->recvMsgP = NULL;
    }
#else
    CSR_UNUSED(cmData);
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
}

void CsrBtCmSdsRegisterReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSdsRegisterReq   * prim = (CsrBtCmSdsRegisterReq *) cmData->recvMsgP;

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    if (cmData->dmVar.localEirData && prim->serviceRecord)
    {
        /* Extract service class UUIDs from service record for use with EIR */
        CsrBtCmEirExtractServicesFromRecord(cmData, prim->serviceRecordSize, prim->serviceRecord);
    }
#endif

    cmData->smVar.appHandle     = prim->appHandle;
    cmData->smVar.context       = prim->context;
    sds_register_req(CSR_BT_CM_IFACEQUEUE, prim->serviceRecord, prim->serviceRecordSize);
}

void CsrBtCmSdsUnRegisterReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSdsUnregisterReq * prim;

    prim                    = (CsrBtCmSdsUnregisterReq *) cmData->recvMsgP;
    cmData->smVar.appHandle = prim->appHandle;
    cmData->smVar.context   = prim->context;
    sds_unregister_req(CSR_BT_CM_IFACEQUEUE, prim->serviceRecHandle);
}

void CsrBtCmSdsRegisterCfmHandler(cmInstanceData_t *cmData)
{
    void                    *message = NULL;
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
    SDS_REGISTER_CFM_T      * receivePrim = (SDS_REGISTER_CFM_T *) cmData->recvMsgP;

    if (receivePrim->result == SDS_SUCCESS)
    {
        resultCode     = CSR_BT_RESULT_CODE_CM_SUCCESS;
        resultSupplier = CSR_BT_SUPPLIER_CM;

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
        if (cmData->dmVar.localEirData)
        {
            /* Registration was a success - update local EIR */
            CsrBtCmEirAddServiceRecord(cmData, receivePrim->svc_rec_hndl);
        }
#endif
    }
    else
    {
        resultCode     = (CsrBtResultCode) receivePrim->result;
        resultSupplier = CSR_BT_SUPPLIER_SDP_SDS;
    }


    if (cmData->smVar.context == CSR_BT_CM_CONTEXT_UNUSED)
    {
        CsrBtCmSdsRegisterCfm   * prim = (CsrBtCmSdsRegisterCfm *) CsrPmemAlloc(sizeof(CsrBtCmSdsRegisterCfm));
        prim->type              = CSR_BT_CM_SDS_REGISTER_CFM;
        prim->serviceRecHandle  = receivePrim->svc_rec_hndl;
        prim->resultCode        = resultCode;
        prim->resultSupplier    = resultSupplier;
        message                 = prim;
    }
    else
    {
        CsrBtCmSdsExtRegisterCfm * prim = (CsrBtCmSdsExtRegisterCfm *) CsrPmemAlloc(sizeof(CsrBtCmSdsExtRegisterCfm));
        prim->type              = CSR_BT_CM_SDS_EXT_REGISTER_CFM;
        prim->serviceRecHandle  = receivePrim->svc_rec_hndl;
        prim->resultCode        = resultCode;
        prim->resultSupplier    = resultSupplier;
        prim->context           = cmData->smVar.context;
        message                 = prim;
    }

    if (receivePrim->result == SDS_SUCCESS)
    {
        CsrBtCmPutMessage(cmData->smVar.appHandle, message);
    }
    else if (receivePrim->result == SDS_PENDING)
    {
        /* Ignore */
        CsrPmemFree(message);
    }
    else
    {
        CsrSchedTimerSet(300000, delayedMessage,cmData->smVar.appHandle, message);
    }
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
}

void CsrBtCmSdsUnRegisterCfmHandler(cmInstanceData_t *cmData)
{
    SDS_UNREGISTER_CFM_T    * receivePrim = (SDS_UNREGISTER_CFM_T *) cmData->recvMsgP;

    if (receivePrim->result == SDS_PENDING)
    {/* Ignore */
        ;
    }
    else
    {
        void                    *message;
        CsrBtResultCode         resultCode;
        CsrBtSupplier           resultSupplier;
        if (receivePrim->result == SDS_SUCCESS)
        {
            resultCode     = CSR_BT_RESULT_CODE_CM_SUCCESS;
            resultSupplier = CSR_BT_SUPPLIER_CM;

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
            if (cmData->dmVar.localEirData)
            {
                /* Unregistration was a success - update local EIR */
                CsrBtCmEirRemoveServiceRecord(cmData, receivePrim->svc_rec_hndl);
            }
#endif
        }
        else
        {
            resultCode     = (CsrBtResultCode) receivePrim->result;
            resultSupplier = CSR_BT_SUPPLIER_SDP_SDS;
        }

        if (cmData->smVar.context == CSR_BT_CM_CONTEXT_UNUSED)
        {
            CsrBtCmSdsUnregisterCfm * prim  = (CsrBtCmSdsUnregisterCfm *) 
                                               CsrPmemAlloc(sizeof(CsrBtCmSdsUnregisterCfm));

            prim->type                      = CSR_BT_CM_SDS_UNREGISTER_CFM;
            prim->serviceRecHandle          = receivePrim->svc_rec_hndl;
            prim->resultCode                = resultCode;
            prim->resultSupplier            = resultSupplier;
            message                         = prim;
        }
        else
        {
            CsrBtCmSdsExtUnregisterCfm * prim = (CsrBtCmSdsExtUnregisterCfm *) 
                                                 CsrPmemAlloc(sizeof(CsrBtCmSdsExtUnregisterCfm));

            prim->type                        = CSR_BT_CM_SDS_EXT_UNREGISTER_CFM;
            prim->serviceRecHandle            = receivePrim->svc_rec_hndl;
            prim->resultCode                  = resultCode;
            prim->resultSupplier              = resultSupplier;
            prim->context                     = cmData->smVar.context;
            message                           = prim;
        }

        if (receivePrim->result == SDS_SUCCESS)
        {
            CsrBtCmPutMessage(cmData->smVar.appHandle, message);
        }
        else
        {
            CsrSchedTimerSet(300000, delayedMessage,cmData->smVar.appHandle, message);
        }
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}


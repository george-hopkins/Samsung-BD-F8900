/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_cmn_sdp_lib.h"
#include "sdc_prim.h"

#define CSR_BT_SDC_RFC_UTIL_INST_ID_NOT_USED 0x00

/* Static util functions    
   priInst MUST be Zalloc'ed before calling this function */
static void cmnSdcRfcInitPrivateInst(CmnSdcRfcPriInstType * priInst)
{ /* Initialize the private instance data used by the this library  */
/*    priInst->numOfSdrAttr               = 0;
    priInst->numOfSdrOutEntries         = 0;
    priInst->sdpInTagList               = NULL;
    priInst->sdpOutTagList              = NULL;
    priInst->sdrAttrIndex               = 0;
    priInst->sdrEntryIndex              = 0;
*/    
    priInst->uuidType                   = SDP_DATA_ELEMENT_SIZE_1_BYTE;
    priInst->serviceHandle              = CMN_SDR_INVALID_SERVICE_HANDLE;
    priInst->state                      = CMN_SDC_RFC_IDLE_STATE;
    priInst->secLevel                   = CSR_BT_SEC_DEFAULT;
    priInst->localServerCh              = CSR_BT_NO_SERVER;
    priInst->btConnId                   = CSR_BT_CONN_ID_INVALID;
/*
    priInst->reqPortPar                 = FALSE;
    priInst->validPortPar               = FALSE;
    priInst->obtainServer               = FALSE;
    priInst->maxFrameSize               = 0;
    priInst->numOfServiceHandleIndicis  = 0;
    priInst->serviceHandleIndexList     = NULL;
    CsrBtBdAddrZero(&priInst->deviceAddr);
*/
    CsrBtPortParDefault(&(priInst->portPar));
    priInst->modemStatus                = CSR_BT_MODEM_SEND_CTRL_DTE_DEFAULT;
    priInst->breakSignal                = CSR_BT_DEFAULT_BREAK_SIGNAL;
    priInst->scTimeout                  = CSR_BT_DEFAULT_MSC_TIMEOUT;
}

static void cmnSdcRfcCleanupPrivateInst(CmnSdcRfcInstType    * inst,
                                        CmnSdcRfcPriInstType * priInst)
{ /* Pfree all pointers used by the library                         */
    if (priInst)
    {
        CsrBtUtilBllFreeLinkedList(&priInst->sdpInTagList, CsrBtUtilBllPfreeWrapper);
        CsrBtUtilBllFreeLinkedList(&priInst->sdpOutTagList, CsrBtUtilBllPfreeWrapper);
        CsrPmemFree(priInst->serviceHandleIndexList);
        CsrPmemFree(inst->privateInst);
        inst->privateInst = NULL;
    }
    else
    { /* No data CsrPmemFree, just ignore                                 */
        ;
    }
}

static CsrBool cmnSdcRfcStartSearch(CmnSdcRfcInstType    * inst,
                                   CmnSdcRfcPriInstType * priInst)
{ /* Start an 32 or 128 bit CM SDP search if the given tag list,
     sdpInTagList, is correct                                       */

    /* Return the number of entries in the linked list, e.g. how
       many different UUID (32 or 128 bit) define in ServiceClass
       ID List                                                      */
    CsrUintFast16 entries = CsrBtUtilBllGetNofEntriesEx(priInst->sdpInTagList);

    if (entries > CMN_SDC_MAX_NUM_OF_UUID || entries == 0)
    { /* Either there is more different uuid's than the CM library
         can handle or there was not a single uuid. Return FALSE
         to indicate an error                                       */
        return FALSE;
    }
    else
    { /* The number of different entries can be handle by the CM
         library                                                    */
        CsrUintFast16    i;
        CsrBtUuid32    *uuid32List  = NULL;
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
        CsrBtUuid128   *uuid128List = NULL;
#endif
        CsrBool      getServiceSuccess = FALSE;

        /* Retrieves the first UUID-type                            */
        getServiceSuccess = CsrBtUtilSdrGetServiceUuidType(priInst->sdpInTagList, 0, &priInst->uuidType);


        /* Find out if a UUID 32bit or a UUID 128 bit CM SDP search
           must be started. Note that it is up to the calling
           process to ensure that only one type of UUID's, 32 or
           128 bit, can be present in the sdpInTagList list         */
        if ((priInst->uuidType == SDP_DATA_ELEMENT_SIZE_4_BYTES) && getServiceSuccess)
        { /* A 32bit CM SDP search must be started.                 */
            uuid32List  = (CsrBtUuid32 *) CsrPmemAlloc(sizeof(CsrBtUuid32) * entries);
        }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
        else
        { /* A 128bit CM SDP search must be started.                */
            uuid128List = (CsrBtUuid128 *) CsrPmemAlloc(sizeof(CsrBtUuid128) * entries);
        }
#endif
        for (i = 0; i < entries; i++)
        { /* Take a copy of the sdpInTagList and the it in the
             sdpOutTagList                                          */
            CsrUint16 newIndex;
            priInst->sdpOutTagList =
                CsrBtUtilSdrCreateServiceHandleEntryFromTemplate(priInst->sdpOutTagList, &newIndex,
                                                       priInst->sdpInTagList, i);

            if ((priInst->uuidType == SDP_DATA_ELEMENT_SIZE_4_BYTES) && getServiceSuccess)
            { /* The first UUID-type were a 32 bit. Extract the
                 UUID and save it                                   */
               if (!CsrBtUtilSdrGetServiceUuid32(priInst->sdpInTagList, i, &(uuid32List[i])))
               { /* Could not find a 32bit UUID-type. Pfree the
                    uuid32List to prevent a memleak and return
                    FALSE to indicate an error                      */
                   CsrPmemFree(uuid32List);
                   return FALSE;
               }
            }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
            else
            { /* The first UUID-type were a 32 bit. Extract the
                 UUID and save it                                   */
               CsrBtUuid128 *uuid128;

               if (!CsrBtUtilSdrGetServiceUuid128(priInst->sdpInTagList, i, &uuid128))
               { /* Could not find a 128bit UUID-type. Pfree the
                    uuid128List to prevent a memleak and return
                    FALSE to indicate an error                      */
                   CsrPmemFree(uuid128List);
                   return FALSE;
               }
               else
               { /* CsrMemCpy uuid128 to the uuid128List               */
                    CsrMemCpy((uuid128List[i]), uuid128, sizeof(CsrBtUuid128));
               }
            }
#endif
        }

        if ((priInst->uuidType == SDP_DATA_ELEMENT_SIZE_4_BYTES) && getServiceSuccess)
        { /* The Cm SDP 32bit search procedure must be called.
             The uuid32List must not be freed because the function
             CsrBtCmSdcSearchReqSend/CsrBtCmSdcRfcSearchReqSend just passes
             it on */
#if !defined(EXCLUDE_CSR_BT_RFC_MODULE)
            if (inst->connectResultFunc)
            { /* Start the Cm Rfc Search procedure                  */
                CsrUint8 localServerChannel;

                if (inst->instId == CSR_BT_SDC_RFC_UTIL_INST_ID_NOT_USED)
                {
                    localServerChannel = priInst->localServerCh;
                }
                else
                { /* The localServerCh is used as a context
                     identifier instead                             */
                    localServerChannel = (CsrUint8)inst->instId;
                }

                if (!CsrBtUtilSdrPerformExtendedSearch(priInst->sdpInTagList, 0))
                {
                    CsrBtCmSdcRfcSearchReqSend(inst->appHandle, priInst->deviceAddr,
                                      uuid32List, (CsrUint8) entries, localServerChannel);
                }
                else
                {
                    CsrBtCmSdcRfcExtendedSearchReqSend(inst->appHandle, priInst->deviceAddr,
                                      uuid32List, (CsrUint8) entries, localServerChannel);
                }
            }
            else
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
            {
                /* Start the Cm Search procedure                      */
                if (!CsrBtUtilSdrPerformExtendedSearch(priInst->sdpInTagList, 0))
                {
                    CsrBtCmSdcSearchReqSend(inst->appHandle, priInst->deviceAddr,
                                            uuid32List, (CsrUint8) entries);
                }
                else
                {
                    CsrBtCmSdcSearchExtReqSend(inst->appHandle, priInst->deviceAddr,
                                            uuid32List, (CsrUint8) entries);
                }
            }
        }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
        else
        { /* The Cm SDP 128bit search procedure must be called.
             The uuid128List must not be CsrPmemFreed, because the functions
             CsrBtCmSdcUuid128SearchReqSend/CsrBtCmSdcUuid128RfcSearchReqSend
             just passes uuid128List on                             */
#if !defined(EXCLUDE_CSR_BT_RFC_MODULE)
            if (inst->connectResultFunc)
            { /* Start the Cm Rfc uuid128 Search procedure          */
                if (inst->instId == CSR_BT_SDC_RFC_UTIL_INST_ID_NOT_USED)
                {
                    CsrBtCmSdcUuid128RfcSearchReqSend(inst->appHandle, priInst->deviceAddr,
                                      uuid128List, (CsrUint8) entries, priInst->localServerCh);
                }
                else
                { /* The localServerCh is used as a context
                     identifier instead. CM just this value up      */
                    CsrBtCmSdcUuid128RfcSearchReqSend(inst->appHandle, priInst->deviceAddr,
                                      uuid128List, (CsrUint8) entries, (CsrUint8)inst->instId);
                }
            }
            else
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
            { /* Start the Cm uuid128 Search procedure              */
                CsrBtCmSdcUuid128SearchReqSend(inst->appHandle, priInst->deviceAddr,
                                                 uuid128List, (CsrUint8) entries);
            }
        }
#endif
        /* Return TRUE to indicate that the CM SDP search is started*/
        return TRUE;
    }
}

static void cmnSdcRfcInsertSearchIndResult(CmnSdcRfcPriInstType    * priInst,
                                           CsrBtUuid32                * serviceHandleList,
                                           CsrUint16                index,
                                           CsrUint16                serviceHandleListCount,
                                           CsrBool                  servicePresent,
                                           CsrBtUuid32                * service32,
                                           CsrBtUuid128               * service128)
{
    if (servicePresent)
    { /* The read service is present in the tag structure           */
        CsrUint16 i;

        for (i = 0; i < (serviceHandleListCount - 1); i++)
        { /* If the remote device support more than one instance
             of this service, make sure that sdpOutTagList contain
             this information                                       */
            CsrUint16 newIndex;
            priInst->sdpOutTagList =
                CsrBtUtilSdrCreateServiceHandleEntryFromTemplate(priInst->sdpOutTagList, &newIndex,
                                                       priInst->sdpOutTagList, index);
        }

        for (i = 0; i < serviceHandleListCount; i++)
        { /* Insert the service handle in the list                  */
            if (service32)
            { /* The service is 32bit UUID-type                     */
                CsrBtUtilSdrInsertServiceHandleAtUuid32(priInst->sdpOutTagList,
                                *service32, serviceHandleList[i]);
            }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
            else
            { /* The service is 128bit UUID-type                    */
                CsrBtUtilSdrInsertServiceHandleAtUuid128(priInst->sdpOutTagList,
                                service128, serviceHandleList[i]);
            }
#else
        CSR_UNUSED(service128);
#endif
        }
    }
    else
    { /* The read service is not present in the tag structure       */
        ;
    }
}

#if !defined(EXCLUDE_CSR_BT_RFC_MODULE)
static CsrUint8 cmnGetLocalServerChannel(CmnCsrBtLinkedListStruct * sdpTag)
{
    CsrUint16 serverCh   = CSR_BT_NO_SERVER;
    CsrUintFast16 i;
    CsrUintFast16 entries    = CsrBtUtilBllGetNofEntriesEx(sdpTag);

    for (i = 0; i < entries; i++)
    {
        if (CsrBtUtilSdrGetLocalServerChannel(sdpTag, i, &serverCh))
        {
            if (serverCh != CSR_BT_NO_SERVER)
            {
                return ((CsrUint8) serverCh);
            }
        }
    }
    return ((CsrUint8) serverCh);
}

static CsrBool cmnSdcRfcGetSelectedServiceHandle(CmnSdcRfcPriInstType * priInst)
{
    CsrUintFast16 i;

    for (i = priInst->sdrEntryIndex; i < priInst->numOfServiceHandleIndicis; i++)
    {
        CsrUintFast16 index = priInst->serviceHandleIndexList[i];

        if (index < priInst->numOfSdrOutEntries)
        { /* The selected entry number is valid, try to retrieve the
             service handle. If this func returns TRUE a valid
             service record is found, other not                     */
            if (CsrBtUtilSdrGetServiceHandle(priInst->sdpOutTagList, index,
                                                &priInst->serviceHandle))
            {
                CsrUint16 serverCh   = CSR_BT_NO_SERVER;
                if (CsrBtUtilSdrGetLocalServerChannel(priInst->sdpOutTagList, index, &serverCh))
                {
                    if (serverCh != CSR_BT_NO_SERVER)
                    {
                        priInst->localServerCh  = (CsrUint8) serverCh;
                        priInst->sdrEntryIndex  = i;
                        return TRUE;
                    }
                }
            }
        }
        else
        { /* The selected entry number is invalid. Just ignore
             the given index                                        */
            ;
        }
    }

    /* Return FALSE to indicate that the connect procedue failed    */
    return FALSE;
}

static CsrBool cmnSdcRfcGetServiceHandle(CmnSdcRfcPriInstType * priInst)
{
    CsrUintFast16 i;

    for (i = priInst->sdrEntryIndex; i < priInst->numOfSdrOutEntries; i++)
    {
        if (CsrBtUtilSdrGetServiceHandle(priInst->sdpOutTagList, i, &priInst->serviceHandle))
        {
            priInst->sdrEntryIndex  = i;
            return TRUE;
        }
        else
        { /* Could no find any valid serviceHandle in the
             sdpOutTagList structure. Just ignore this entry        */
            ;
        }
    }
    /* Return FALSE to indicate all service handle has been read    */
    priInst->sdrEntryIndex = i;
    return FALSE;
}
#endif /* EXCLUDE_CSR_BT_RFC_MODULE*/

static CsrBool cmnSdcRfcGetAttributeValue(CmnSdcRfcPriInstType * priInst,
                                         CsrBtUuid32             * serviceHandle,
                                         CsrUint16             * attrId)
{ /* Search for a service handle which was a SDP attributes to read */
    CsrBtUuid32 serviceHd;
    CsrUintFast16 i;

    for (i = priInst->sdrEntryIndex; i < priInst->numOfSdrOutEntries; i++)
    {
        if (CsrBtUtilSdrGetServiceHandle(priInst->sdpOutTagList, i, &serviceHd))
        { /* Find out how many SDP attributes must be read from the
             serviceHandle                                          */
            priInst->sdrAttrIndex = 0;

            if ((CsrBtUtilSdrGetNofAttributes(priInst->sdpOutTagList, i, &(priInst->numOfSdrAttr))) &&
                priInst->numOfSdrAttr > 0)
            { /* At least one SDP attribute must be read from this
                 serviceHandle                                      */
                CsrUint16 attrValue;
                if (CsrBtUtilSdrGetAttributeUuid(priInst->sdpOutTagList, i, priInst->sdrAttrIndex, &attrValue))
                { /* Found the SDP attribute value. Return TRUE
                     to indicate that an attribute must be read
                     from the servicehandle                         */
                    *serviceHandle          = serviceHd;
                    *attrId                 = attrValue;
                    priInst->sdrEntryIndex  = i;
                    return TRUE;
                }
            }
            else
            { /* No attribute, try next entry                       */
                ;
            }
        }
        else
        { /* Could no find any valid serviceHandle in the
             sdpOutTagList structure. Just ignore this entry        */
            ;
        }
    }
    /* Return FALSE to indicate all attributes has been read        */
    priInst->sdrEntryIndex = i;
    return FALSE;
}

static void cmnSdcRfcReadAttrOrClose(CmnSdcRfcInstType    * inst,
                                     CmnSdcRfcPriInstType * priInst)
{
    CsrBtUuid32 serviceHandle;
    CsrUint16 attrId;

    if (cmnSdcRfcGetAttributeValue(priInst, &serviceHandle, &attrId))
    { /* Need to read an attribute                                  */
        priInst->state          = CMN_SDC_RFC_ATTR_STATE;
        priInst->serviceHandle  = serviceHandle;
        CsrBtCmSdcAttributeReqSend(serviceHandle, attrId, L2CA_MTU_DEFAULT);
    }
    else
    { /* No attributes to read, close the SDP channel               */
        priInst->state = CMN_SDC_RFC_CLOSE_SEARCH_STATE;
        CsrBtCmSdcCloseReqSend(inst->appHandle);
    }
}

#if !defined(EXCLUDE_CSR_BT_RFC_MODULE)
static CsrBool cmnSdcRfcErrorResultHandler(CsrBtResultCode      resultCode,
                                           CsrBtSupplier  resultSupplier)
{
    CsrBool initiateConnect = FALSE;

    /* Initiate connect if supplier of error is rfcomm only
       and are not one of the following errors */
    if ( (resultSupplier == CSR_BT_SUPPLIER_RFCOMM) && 
         (resultCode != RFC_CONNECTION_REJ_SECURITY) )
    {
        initiateConnect = TRUE;
    }

    return initiateConnect;
}
#endif

static void cmnSdcRfcCallResultFunc(void                 * instData,
                                    CmnSdcRfcInstType    * inst,
                                    CmnSdcRfcPriInstType * priInst,
                                    CsrBtResultCode      resultCode,
                                    CsrBtSupplier  resultSupplier,
                                    CsrBool               releaseResources)
{
    if (inst->searchResultFunc)
    { /* Call the function which indicates that Cm SDP search
         procedure is finish                                        */
        if (resultCode      == CSR_BT_RESULT_CODE_CM_SUCCESS &&
            resultSupplier  == CSR_BT_SUPPLIER_CM)
        { /* The search procedure is finish with SUCCESS. Set
             priInst->sdpOutTagList NULL because the profiles will
             be the owner of the pointer                            */
            inst->searchResultFunc(instData, priInst->sdpOutTagList,
                priInst->deviceAddr, resultCode, resultSupplier);

            priInst->sdpOutTagList = NULL;
        }
        else
        { /* The search procedure had failed                        */
            inst->searchResultFunc(instData, NULL,
                priInst->deviceAddr, resultCode, resultSupplier);
        }
    }
#if !defined(EXCLUDE_CSR_BT_RFC_MODULE)
    else
    { /* Call the function which indicates that connect procedure
         is finish                                                  */
        if (resultCode      == CSR_BT_RESULT_CODE_CM_SUCCESS &&
            resultSupplier  == CSR_BT_SUPPLIER_CM)
        { /* The connect procedure is finish with success. Copy one
             ready, and copy one sdpTag out of the sdpTagList. The
             owner of the sdpTag                                    */
            CmnCsrBtLinkedListStruct * sdpTag;
            CsrUint8                 * data;
            CsrUint16                dataLen;

            data    = CsrBtUtilBllGetDataPointerEx(priInst->sdpOutTagList, priInst->sdrEntryIndex, &dataLen);
            sdpTag  = CsrBtUtilBllCreateNewEntry(NULL, data, dataLen);
            CsrBtUtilBllSetDataPointerEx(priInst->sdpOutTagList, priInst->sdrEntryIndex, NULL, 0);

            inst->connectResultFunc(instData, priInst->localServerCh,
                                    priInst->btConnId, priInst->deviceAddr,
                                    priInst->maxFrameSize, priInst->validPortPar,
                                    priInst->portPar, resultCode, resultSupplier, sdpTag);
        }
        else
        { /* The connect procedure had failed                       */
            if (priInst->localServerCh != CSR_BT_NO_SERVER &&
                priInst->obtainServer                   )
            { /* Need to unregister the local serverCh again        */
                CsrBtCmUnRegisterReqSend(priInst->localServerCh);
            }
            else
            { /* Did not get any local serverChannel, e.g.
                 nothing to unregister                              */
                ;
            }
            inst->connectResultFunc(instData, CSR_BT_NO_SERVER, CSR_BT_CONN_ID_INVALID,
                                    priInst->deviceAddr, 0,
                                    FALSE, priInst->portPar,
                                    resultCode, resultSupplier, NULL);
        }

        if (releaseResources)
        { /* Request the CM to release its resources                */
            if (inst->instId == CSR_BT_SDC_RFC_UTIL_INST_ID_NOT_USED)
            {
                CsrBtCmSdcReleaseResourcesReqSend(inst->appHandle,
                                                  priInst->deviceAddr,
                                                  priInst->localServerCh);
            }
            else
            {
                CsrBtCmSdcReleaseResourcesReqSend(inst->appHandle,
                                                  priInst->deviceAddr,
                                                  (CsrUint8)inst->instId);
            }
        }
        else
        { /* No resources to release                                */
            ;
        }
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

    /* The Search or connect procedure is finished, free
       all data used by intern by the util lib                      */
    cmnSdcRfcCleanupPrivateInst(inst, priInst);
}

static CsrBool cmnReleaseResources(CmnSdcRfcInstType     * inst,
                                  CsrBtResultCode       resultCode,
                                  CsrBtSupplier   resultSupplier)
{
    if (inst->connectResultFunc                     &&
        resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        resultSupplier  == CSR_BT_SUPPLIER_CM)
    { /* The cm resources must be free                              */
        return TRUE;
    }
    else
    { /* No cm resources are allocated                              */
        return FALSE;
    }
}


static CsrBool cmnSdcRfcStartConnect(void                 * instData,
                                    CmnSdcRfcInstType    * inst,
                                    CmnSdcRfcPriInstType * priInst)
{
    if (priInst->numOfSdrOutEntries > 0)
    { /* At least one entry is valid                                */
        priInst->sdrEntryIndex  = 0;

        if (inst->selectServiceHandleFunc)
        { /* The profile has requested to select a service record
             handle index, call the function which allow the
             profile to do this                                     */
            priInst->state = CMN_SDC_RFC_SELECT_SERVICE_RECORD_HANDLE_STATE;

            inst->selectServiceHandleFunc(instData, inst, priInst->deviceAddr,
                       priInst->localServerCh, (CsrUint16)priInst->numOfSdrOutEntries, priInst->sdpOutTagList);

            return TRUE;
        }
        else
        { /* The service record handle must be auto selected by the
             CM RFC connect procedure                               */
#if !defined(EXCLUDE_CSR_BT_RFC_MODULE)
            if (cmnSdcRfcGetServiceHandle(priInst))
            { /* A service record handle has been found. Request CM
                 to initiate an RFC connection                      */
                priInst->state          = CMN_SDC_RFC_CONNECT_STATE;

                CsrBtCmContextConnectReqSend(inst->appHandle, priInst->localServerCh,
                                             priInst->serviceHandle, priInst->maxFrameSize,
                                             priInst->reqPortPar, priInst->validPortPar,
                                             priInst->portPar, priInst->secLevel, priInst->deviceAddr,
                                             inst->instId, priInst->modemStatus, priInst->breakSignal, priInst->scTimeout);

                return TRUE;
            }
            else
#endif /*EXCLUDE_CSR_BT_RFC_MODULE */
            { /* No service record handle has been found, the
                 connect procedure failed                           */
                return FALSE;
            }
        }
    }
    else
    { /* Not a single Sdr entry is valid, the connect
         procedure failed                                           */
        return FALSE;
    }
}

/* Static CM upstream messages handler function                     */
static CsrBool cmnSdcRfcSearchIndHandler(void                 * instData,
                                        CmnSdcRfcInstType    * inst,
                                        CmnSdcRfcPriInstType * priInst,
                                        void                 * msg)
{ /* Has read a uuid32 service, save info and wait for a
     CSR_BT_CM_SDC_SEARCH_CFM message                                      */
    CsrUint16 index;
    CsrBtCmSdcSearchInd * prim = (CsrBtCmSdcSearchInd *) msg;

    /* Find the return index to where the return service is place
       in the priInst->sdpOutTagList                                */
    CsrBool servicePresent = CsrBtUtilSdrGetIndexForServiceUuid32
                                (priInst->sdpOutTagList, prim->service, &index);

    /* Save the return service record handle and if the remote
       device support support more than one instance of the found
       service this information the sdpOutTagList is updated        */
    cmnSdcRfcInsertSearchIndResult(priInst, prim->serviceHandleList,
                                   index, prim->serviceHandleListCount,
                                   servicePresent, &prim->service, NULL);

    CsrPmemFree(prim->serviceHandleList);
    prim->serviceHandleList = NULL;
    CSR_UNUSED(instData);
    return FALSE;
}

#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
static CsrBool cmnSdcRfcUuid128SearchIndHandler(void                 * instData,
                                               CmnSdcRfcInstType    * inst,
                                               CmnSdcRfcPriInstType * priInst,
                                               void                 * msg)
{ /* Has read a uuid128 service, save info and wait for a
     CSR_BT_CM_SDC_SEARCH_CFM message                                      */
    CsrUint16 index;
    CsrBtCmSdcUuid128SearchInd *prim  = (CsrBtCmSdcUuid128SearchInd *) msg;

    /* Find the return index to where the return service is place
       in the priInst->sdpOutTagList                                */
    CsrBool servicePresent = CsrBtUtilSdrGetIndexForServiceUuid128
                              (priInst->sdpOutTagList, &prim->service, &index);

    /* Save the return service record handle and if the remote
       device support support more than one instance of the found
       service this information the sdpOutTagList is updated        */
    cmnSdcRfcInsertSearchIndResult(priInst, prim->serviceHandleList,
                                   index, prim->serviceHandleListCount,
                                   servicePresent, NULL, &prim->service);

    CsrPmemFree(prim->serviceHandleList);
    prim->serviceHandleList = NULL;
    CSR_UNUSED(instData);
    return FALSE;
}
#endif
static CsrBool cmnSdcRfcSearchCfmHandler(void                 * instData,
                                        CmnSdcRfcInstType    * inst,
                                        CmnSdcRfcPriInstType * priInst,
                                        void                 * msg)
{ /* The CM has found all of the requested service that the remote
     device support                                                 */
    if (priInst->state == CMN_SDC_RFC_CANCEL_STATE)
    { /* The SDP search has been cancelled, Close the SDP channel   */
        CsrBtCmSdcCloseReqSend(inst->appHandle);
    }
    else
    { /* Remove all none succesful entries. At least one entry must
         be vaild otherwise the CSR_BT_CM_SDC_SEARCH_CFM message is never
         received. Start determined if any of these entries has some
         attributes that must be read. If not the SDP channel is
         closed                                                     */
        priInst->sdrEntryIndex      = 0;
        priInst->sdpOutTagList      = CsrBtUtilSdrRemoveNonSuccessStatusEntries(priInst->sdpOutTagList);
        priInst->numOfSdrOutEntries = CsrBtUtilBllGetNofEntriesEx(priInst->sdpOutTagList);
        cmnSdcRfcReadAttrOrClose(inst, priInst);
    }
    CSR_UNUSED(instData);
    CSR_UNUSED(msg);
    return FALSE;
}

static CsrBool cmnSdcRfcAttributeCfmHandler(void                 * instData,
                                           CmnSdcRfcInstType    * inst,
                                           CmnSdcRfcPriInstType * priInst,
                                           void                 * msg)
{
    CsrBtCmSdcAttributeCfm * prim    = (CsrBtCmSdcAttributeCfm *) msg;

    if (priInst->state == CMN_SDC_RFC_CANCEL_STATE)
    { /* The SDP search has been  cancelled, Close the SDP channel  */
        CsrBtCmSdcCloseReqSend(inst->appHandle);
    }
    else
    { /* Insert the retrieve attribute information                  */
         CsrBtUtilSdrInsertAttributeDataAtAttributeUuid(priInst->sdpOutTagList, priInst->sdrEntryIndex,
                                              priInst->sdrAttrIndex, prim->attributeListSize, prim->attributeList);

       /* Increment the sdrAttrIndex and check if more attribute
          needs to be read from the servicehandle                   */
        priInst->sdrAttrIndex++;

        if (priInst->sdrAttrIndex < priInst->numOfSdrAttr)
        { /* Need to read more attribute from this entry, e.g
             from this serviceHandle                                */
            CsrUint16 attrId;

            if (CsrBtUtilSdrGetAttributeUuid(priInst->sdpOutTagList,
                        priInst->sdrEntryIndex, priInst->sdrAttrIndex, &attrId))
            { /* Read the SDP attribute                             */
                CsrBtCmSdcAttributeReqSend(priInst->serviceHandle,
                                        attrId,
                                        L2CA_MTU_DEFAULT);
            }
            else
            { /* Could not find any attribute value. Determined if
                 any attributes must be read from another
                 serviceHandle. If not the SDP channel is closed    */
                priInst->sdrEntryIndex++;
                cmnSdcRfcReadAttrOrClose(inst, priInst);
            }
        }
        else
        { /* All atribute has been read from this servicehandle.
             Determined if any attributes must be read from
             another serviceHandle. If not the SDP channel
             is closed                                              */
            priInst->sdrEntryIndex++;
            cmnSdcRfcReadAttrOrClose(inst, priInst);
        }
    }
    CsrPmemFree(prim->attributeList);
    prim->attributeList = NULL;
    CSR_UNUSED(instData);
    return FALSE;
}

static CsrBool cmnSdcRfcCloseIndHandler(void                 * instData,
                                       CmnSdcRfcInstType    * inst,
                                       CmnSdcRfcPriInstType * priInst,
                                       void                 * msg)
{ /* Indicates that the SDC connection has now closed.              */
    CsrBool returnValue          = TRUE;
    CsrBtCmSdcCloseInd * prim   = (CsrBtCmSdcCloseInd *) msg;
    CsrBool releaseResources     = cmnReleaseResources(inst, prim->resultCode, prim->resultSupplier);

    switch (priInst->state)
    {
        case CMN_SDC_RFC_CANCEL_STATE:
        { /* The procedure has been cancelled.                      */
            cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                    CSR_BT_RESULT_CODE_CM_CANCELLED,
                                    CSR_BT_SUPPLIER_CM, releaseResources);
            break;
        }
        case CMN_SDC_RFC_CLOSE_SEARCH_STATE:
        { /* The SDP search is closed because the search procedure
             is finish                                              */
            if (inst->searchResultFunc)
            { /* Call the function which indicates that search
                 procedure is finish with SUCCESS                   */
                cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                        CSR_BT_RESULT_CODE_CM_SUCCESS,
                                        CSR_BT_SUPPLIER_CM, releaseResources);
            }
            else
            { /* The connect procedure is running, check if at least
                 one SDP result is valid                            */
                if (cmnSdcRfcStartConnect(instData, inst, priInst))
                { /* Continue connecting. Set the returnValue to
                     FALSE in order to indicate that the connect
                     procedure is not finish yet                    */
                    returnValue = FALSE;
                }
                else
                { /* The sdc search fail, e.g the connect procedure
                     failed                                         */
                    cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                            (CsrBtResultCode) SDC_NO_RESPONSE_DATA,
                                            CSR_BT_SUPPLIER_SDP_SDC, releaseResources);
                }
            }
            break;
        }
        default:
        { /* The SDP channel were close abnormal                    */
            cmnSdcRfcCallResultFunc(instData, inst, priInst, prim->resultCode, prim->resultSupplier, releaseResources);
            break;
        }
    }
    return (returnValue);
}

#if !defined(EXCLUDE_CSR_BT_RFC_MODULE)
static CsrBool cmnSdcRfcRegisterCfm(void                 * instData,
                                   CmnSdcRfcInstType    * inst,
                                   CmnSdcRfcPriInstType * priInst,
                                   void                 * msg)
{ /* This event will be used to indicate to the higher layer that
     its previous registration of a protocol handle with an
     CSR_BT_CM_REGISTER_REQ event had been accepted . The server
     channel number assigned is also returned.                      */

    CsrBtCmRegisterCfm * prim    = (CsrBtCmRegisterCfm *) msg;
    priInst->localServerCh       = prim->serverChannel;

    CsrBtUtilSdrInsertLocalServerChannel(priInst->sdpInTagList, 0, prim->serverChannel);

    if (priInst->state == CMN_SDC_RFC_CANCEL_STATE)
    { /* The RFC connect procedure has been cancelled,              */
        cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                CSR_BT_RESULT_CODE_CM_CANCELLED,
                                CSR_BT_SUPPLIER_CM, FALSE);

        return TRUE;
    }
    else
    { /* priInst->state must be CMN_SDC_RFC_REGISTER_STATE          */
        if (prim->resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS &&
            prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        { /* A local server channel has been obtained               */
            if (cmnSdcRfcStartSearch(inst, priInst))
            { /* Start the rfc search procedure                     */
                priInst->state          = CMN_SDC_RFC_SEARCH_STATE;
                return FALSE;
            }
            else
            { /* The sdpTag could not be interpret correct          */
                cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                        CSR_BT_RESULT_CODE_CM_INTERNAL_ERROR,
                                        CSR_BT_SUPPLIER_CM, FALSE);
            }
        }
        else
        { /* A local server channel could not be obtained           */
            cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                    prim->resultCode, prim->resultSupplier, FALSE);
        }
        return TRUE;
    }
}

static CsrBool cmnSdcRfcConnectCfm(void                    * instData,
                                     CmnSdcRfcInstType    * inst,
                                     CmnSdcRfcPriInstType * priInst,
                                     void                 * msg)
{ /* This event is the confirmation of the opening of a RFC
     connection. If the connection has been established the
     result is a success otherwise not                              */
    CsrBtCmConnectCfm * prim    = (CsrBtCmConnectCfm *) msg;

    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The connection has been established                        */
        if (priInst->state == CMN_SDC_RFC_CANCEL_STATE)
        { /* The Rfc connect procedure has been cancelled. Release
             the connection again                                   */
            CsrBtCmContextDisconnectReqSend(prim->btConnId, inst->instId);
            return FALSE;
        }
        else
        { /* Call the function which indicates that connect
             procedure is finish with SUCCESS                       */
            priInst->btConnId       = prim->btConnId;
            priInst->maxFrameSize   = prim->profileMaxFrameSize;
            priInst->portPar        = prim->portPar;
            priInst->validPortPar   = prim->validPortPar;
            cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                    CSR_BT_RESULT_CODE_CM_SUCCESS,
                                    CSR_BT_SUPPLIER_CM, TRUE);

            return TRUE;
        }
    }
    else
    { /* The connection could not be establish                      */
        if (priInst->state == CMN_SDC_RFC_CANCEL_STATE)
        { /* The Rfc connect procedure has been cancelled. Call the
             function which indicates that connect procedure is
             finish                                                 */
            cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                    CSR_BT_RESULT_CODE_CM_CANCELLED,
                                    CSR_BT_SUPPLIER_CM, TRUE);

            return TRUE;
        }
        else
        { /* The profile don't care which of the service handle is
             being used. Check if it is possible to connect to
             another SDP service handle                             */
            CsrBool initiateConnect = cmnSdcRfcErrorResultHandler(prim->resultCode, prim->resultSupplier);
            
            if (initiateConnect)
            { 
                priInst->sdrEntryIndex++;

                if (inst->selectServiceHandleFunc)
                { /* The profile has selected a specific service record
                     handle list. Check if it is possible to connect to
                     another SDP service handle                         */
                    initiateConnect = cmnSdcRfcGetSelectedServiceHandle(priInst);
                }
                else
                { /* The profile don't care which of the service handle
                     is being used. Check if it is possible to connect
                     to another SDP service handle                      */
                    initiateConnect = cmnSdcRfcGetServiceHandle(priInst);
                }
            }

            if (initiateConnect)
            { /* A service record handle has been found. Request CM
                 to initiate an RFC connection                      */
                priInst->state          = CMN_SDC_RFC_CONNECT_STATE;

                CsrBtCmContextConnectReqSend(inst->appHandle, priInst->localServerCh,
                                             priInst->serviceHandle, (CsrUint16)priInst->btConnId,
                                             priInst->reqPortPar, priInst->validPortPar,
                                             priInst->portPar, priInst->secLevel, priInst->deviceAddr,
                                             inst->instId, priInst->modemStatus, priInst->breakSignal, priInst->scTimeout);

                return FALSE;
            }
            else
            { /* Could not connect to any of the obtain service
                 record handles. Call the function which indicates
                 that the connect procedure failed                  */
                cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                    prim->resultCode,
                                    prim->resultSupplier, TRUE);

                return TRUE;
            }
        }
    }
}

static CsrBool cmnSdcRfcDisconnectInd(void                 * instData,
                                     CmnSdcRfcInstType    * inst,
                                     CmnSdcRfcPriInstType * priInst,
                                     void                 * msg)
{ /* The connection is release because the request to cancel this
     procedure were crossing with a successful CSR_BT_CM_CONNECT_CFM
     message                                                        */
    CsrBtCmDisconnectInd *prim = (CsrBtCmDisconnectInd *) msg;

    if (prim->status)
    { /* The connection is release meaning that this procedure is
         cancelled, call the function which indicates that this
         procedure is finish                                        */
        cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                CSR_BT_RESULT_CODE_CM_CANCELLED,
                                CSR_BT_SUPPLIER_CM, TRUE);

        return TRUE;
    }
    else
    { /* Failed to release the connection. Try again                */
        CsrBtCmContextDisconnectReqSend(prim->btConnId, inst->instId);
        return FALSE;
    }
}

static CsrBool cmnSdcRfcPortNegInd(void                 * instData,
                                  CmnSdcRfcInstType    * inst,
                                  CmnSdcRfcPriInstType * priInst,
                                  void                 * msg)
{ /* This event is indicates to the higher port entity that the
     remote port entity requests it to set port parameters for the
     DLCI as given in the Port Parameters. The port entity should
     respond with RFC_PORTNEG_RES.                                  */
    CsrBtCmPortnegInd  * prim    = (CsrBtCmPortnegInd *) msg;

    if (inst->setPortParFunc)
    { /* The profile has requested to handle this message, call the
         function which allow the profile to do this                */
        priInst->btConnId = prim->btConnId;
        priInst->state = CMN_SDC_RFC_PORTNEG_PENDING_STATE;

        inst->setPortParFunc(instData, priInst->deviceAddr,
                    priInst->localServerCh, prim->portPar, prim->request);
    }
    else
    { /* The message must be handle automatic by this lib           */
        if (prim->request)
        {
            RFC_PORTNEG_VALUES_T portPar;

            CsrBtPortParDefault(&portPar);
            CsrBtCmPortnegResSend(prim->btConnId, &(portPar));
        }
        else
        {
            CsrBtCmPortnegResSend(prim->btConnId, &(prim->portPar));
        }
    }
    return FALSE;
}

static CsrBool cmnSdcRfcMicsMsgCfm(void                 * instData,
                                  CmnSdcRfcInstType    * inst,
                                  CmnSdcRfcPriInstType * priInst,
                                  void                 * msg)
{ /* Received a CM upstream that must be ignore. Make sure that
     the message contents is CsrPmemFree to prevent a memory leak         */
    CSR_UNUSED(instData);
    CSR_UNUSED(inst);
    CSR_UNUSED(priInst);
    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg);
    return FALSE;
}

static const CmnSdcRfcType cmnRfcCmMsgHandlers[CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT] =
{
    NULL,                           /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
    cmnSdcRfcConnectCfm,            /* CSR_BT_CM_CONNECT_CFM               */
    NULL,                           /* CSR_BT_CM_CONNECT_ACCEPT_CFM        */
    cmnSdcRfcRegisterCfm,           /* CSR_BT_CM_REGISTER_CFM              */
    cmnSdcRfcDisconnectInd,         /* CSR_BT_CM_DISCONNECT_IND            */
    NULL,                           /* CSR_BT_CM_SCO_CONNECT_CFM           */
    NULL,                           /* CSR_BT_CM_SCO_DISCONNECT_IND        */
    NULL,                           /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM    */
    cmnSdcRfcMicsMsgCfm,            /* CSR_BT_CM_DATA_IND                  */
    NULL,                           /* CSR_BT_CM_DATA_CFM                  */
    cmnSdcRfcMicsMsgCfm,            /* CSR_BT_CM_CONTROL_IND               */
    cmnSdcRfcMicsMsgCfm,            /* CSR_BT_CM_RFC_MODE_CHANGE_IND       */
    cmnSdcRfcPortNegInd,            /* CSR_BT_CM_PORTNEG_IND               */
    NULL,                           /* CSR_BT_CM_PORTNEG_CFM               */
};
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

static const CmnSdcRfcType cmnSdcCmMsgHandlers[CSR_BT_CM_SDC_PRIM_UPSTREAM_COUNT] =
{
    cmnSdcRfcSearchIndHandler,      /* CSR_BT_CM_SDC_SEARCH_IND            */
    cmnSdcRfcSearchCfmHandler,      /* CSR_BT_CM_SDC_SEARCH_CFM            */
    NULL,                           /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM    */
    NULL,                           /* CSR_BT_CM_SDC_OPEN_CFM              */
    cmnSdcRfcAttributeCfmHandler,   /* CSR_BT_CM_SDC_ATTRIBUTE_CFM         */
    cmnSdcRfcCloseIndHandler,       /* CSR_BT_CM_SDC_CLOSE_IND             */
    NULL,                           /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
    NULL,                           /* CSR_BT_CM_SDS_REGISTER_CFM          */
    NULL,                           /* CSR_BT_CM_SDS_UNREGISTER_CFM        */
};

static CsrBool cmnSdcRfcVerifyInstData(void *cmSdcRfcInstData)
{
    if (cmSdcRfcInstData)
    { /* inst data is present                                       */
        CmnSdcRfcInstType * inst = cmSdcRfcInstData;

        if (inst->privateInst)
        { /* All inst data is available, return TRUE                */
            return TRUE;
        }
        else
        { /* No private inst data, return FALSE                     */
            return FALSE;
        }
    }
    else
    { /* No inst data is available, return FALSE                    */
        return FALSE;
    }
}

static CsrBool cmnCmMsgHandler(void *instData, void *cmSdcRfcInstData,
                              void *msg, CsrBool rfcConnect)
{
    if (cmnSdcRfcVerifyInstData(cmSdcRfcInstData))
    {
        CsrBtCmPrim *primType         = (CsrPrim *) msg;
        CmnSdcRfcInstType * inst    = cmSdcRfcInstData;

        CsrUint16  sdcIndex = (CsrUint16) (*primType - CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST);

        if (*primType >= CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST &&
            *primType <= CSR_BT_CM_SDC_PRIM_UPSTREAM_HIGHEST &&
            cmnSdcCmMsgHandlers[sdcIndex] != NULL)
        { /* This library function is able to handle the incoming CM
             message. Call the handler function to the incoming
             message                                                */
            return (cmnSdcCmMsgHandlers[sdcIndex] (instData, inst,
                                             inst->privateInst, msg));
        }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
        else if (*primType == CSR_BT_CM_SDC_UUID128_SEARCH_IND)
        { /* This library function is able to handle the incoming CM
             message. Call the handler function to this incoming
             message                                                */
            return (cmnSdcRfcUuid128SearchIndHandler(instData, inst,
                                                     inst->privateInst, msg));
        }
#endif
        else
        { /* Check if it the Sdc search or the Rfc connect procedure
             that is running                                        */
#if !defined(EXCLUDE_CSR_BT_RFC_MODULE)
            if (rfcConnect)
            { /* It is the Cmn Rfc Connect procedure that is running*/
                CsrUint16  rfcIndex = (CsrUint16) (*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST);
                if (*primType >= CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST &&
                    *primType <= CSR_BT_CM_RFC_PRIM_UPSTREAM_HIGHEST &&
                    cmnRfcCmMsgHandlers[rfcIndex] != NULL)
                {
                    return (cmnRfcCmMsgHandlers[rfcIndex] (instData, inst,
                                             inst->privateInst, msg));
                }
                else
                { /* This library function is not able to handle the
                     incoming CM message. Make sure that the data
                     pointer in this message is CsrPmemFree as it is
                     consider as a state event error has occurred   */
                    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg);
                    return FALSE;
                }
            }
            else
#endif /* EXCLUDE_CSR_BT_RFC_MODULE*/
            { /* This library function is not able to handle the
                 incoming CM message. Make sure that the data pointer
                 in this message is CsrPmemFree as it is consider as a
                 state event error has occurred                     */
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg);
                return FALSE;
            }
        }
    }
    else
    {  /* No inst data is allocated this function called is not
         allowed.                                                   */
        CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg);
        return TRUE;
    }
}

void * CsrBtUtilSdpRfcInit(CmnRfcConSelectServiceHandleFuncType selectServiceHandler,
                                  CsrBtUtilRfcConSetPortParFuncType    setPortParHandler,
                                  CmnRfcConResultFuncType              rfcConResultHandler,
                                  CsrSchedQid                               appHandle,
                                  CsrUint8                             instIdentifier)
{
    CmnSdcRfcInstType * inst = (CmnSdcRfcInstType *) CsrPmemAlloc(sizeof(CmnSdcRfcInstType));

    inst->appHandle                 = appHandle;
    inst->connectResultFunc         = rfcConResultHandler;
    inst->selectServiceHandleFunc   = selectServiceHandler;
    inst->setPortParFunc            = setPortParHandler;
    inst->instId                    = instIdentifier;
    inst->privateInst               = NULL;
    inst->searchResultFunc          = NULL;
    return (inst);
}

/* public function                                                  */
void * CsrBtUtilSdcInit(CmnSdcResultFuncType sdcResultHandler,
                        CsrSchedQid               appHandle)
{
    CmnSdcRfcInstType * inst = (CmnSdcRfcInstType *) CsrPmemZalloc(sizeof(CmnSdcRfcInstType));

    inst->appHandle                 = appHandle;
    inst->searchResultFunc          = sdcResultHandler;
    return (inst);
}

void CsrBtUtilSdcRfcDeinit(void ** cmSdcRfcInstData)
{ /* Deregister the callback function register by the CmnSdpInit    */
    CmnSdcRfcInstType * inst = *cmSdcRfcInstData;

    if (inst)
    { /* Pfree priInst and inst                                     */
        cmnSdcRfcCleanupPrivateInst(inst, inst->privateInst);
        CsrPmemFree(inst);
        *cmSdcRfcInstData = NULL;
    }
    else
    { /* Nothing to free                                            */
        ;
    }
}

CsrBool CsrBtUtilSdcSearchStart(void                    * instData,
                         void                    * cmSdcRfcInstData,
                         CmnCsrBtLinkedListStruct * sdpTag,
                         CsrBtDeviceAddr            deviceAddr)
{
    if (cmSdcRfcInstData)
    { /* inst data is present                                       */
        CmnSdcRfcInstType * inst = cmSdcRfcInstData;

        if (!inst->privateInst && inst->searchResultFunc)
        { /* Start the CM SDP search procedure                      */
            CmnSdcRfcPriInstType * priInst;

            inst->privateInst = (CmnSdcRfcPriInstType *) CsrPmemZalloc(sizeof(CmnSdcRfcPriInstType));

            priInst               = inst->privateInst;
            cmnSdcRfcInitPrivateInst(priInst);
            priInst->deviceAddr   = deviceAddr;
            priInst->sdpInTagList = sdpTag;

            if (cmnSdcRfcStartSearch(inst, priInst))
            { /* The CM SDP search is started                       */
                priInst->state    = CMN_SDC_RFC_SEARCH_STATE;
            }
            else
            { /* The sdpTag could not be interpret correct. Call
                 the function which indicates that this procedure
                 is finish                                          */
                cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                        CSR_BT_RESULT_CODE_CM_INTERNAL_ERROR,
                                        CSR_BT_SUPPLIER_CM, FALSE);

                return TRUE;
            }
        }
        else
        { /* The procedure is already running. Just ignore this
             request, but make sure that the pointer *sdpTag
             is CsrPmemFree to prevent a memoryleak                       */
            CsrBtUtilBllFreeLinkedList(&sdpTag, CsrBtUtilBllPfreeWrapper);
        }
        return FALSE;
    }
    else
    { /* No cmSdcRfcInstData, make sure that the pointer *sdpTag
         is CsrPmemFree to prevent a memoryleak                           */
        CsrBtUtilBllFreeLinkedList(&sdpTag, CsrBtUtilBllPfreeWrapper);
        return TRUE;
    }
}

CsrBool CsrBtUtilSdcVerifyCmMsg(void *msg)
{
    CsrBtCmPrim *primType = (CsrPrim *) msg;
    CsrUint16  index     = (CsrUint16) (*primType - CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST);

    if (*primType >= CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST &&
        *primType <= CSR_BT_CM_SDC_PRIM_UPSTREAM_HIGHEST &&
        cmnSdcCmMsgHandlers[index] != NULL)
    { /* This library is able to handle this message                */
        return TRUE;
    }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
    else if (*primType == CSR_BT_CM_SDC_UUID128_SEARCH_IND)
    { /* This library is able to handle this message                */
        return TRUE;
    }
#endif
    else
    { /* This library is not able to handle this message            */
        return FALSE;
    }
}

CsrBool CsrBtUtilSdcCmMsgHandler(void *instData, void *cmSdcRfcInstData, void *msg)
{ /* This function handles incoming CM messages                     */
    return (cmnCmMsgHandler(instData, cmSdcRfcInstData, msg, FALSE));
}

CsrBool CsrBtUtilSdcSearchCancel(void *instData, void *cmSdcRfcInstData)
{
    if (cmnSdcRfcVerifyInstData(cmSdcRfcInstData))
    {
        CmnSdcRfcInstType    * inst    = cmSdcRfcInstData;
        CmnSdcRfcPriInstType * priInst = inst->privateInst;

        switch(priInst->state)
        {
            case CMN_SDC_RFC_CLOSE_SEARCH_STATE:
            { /* The SDP channel is allready being closed. Change
                 state to indicated that the SDP search is
                 cancelled                                          */
                priInst->state = CMN_SDC_RFC_CANCEL_STATE;
                return FALSE;
            }
            case CMN_SDC_RFC_ATTR_STATE:
            case CMN_SDC_RFC_SEARCH_STATE:
            { /* The SDP search is running, cancel it               */
                priInst->state = CMN_SDC_RFC_CANCEL_STATE;

                if (priInst->uuidType == SDP_DATA_ELEMENT_SIZE_4_BYTES)
                { /* A 32bit CM SDP search must be cancel           */
 #if !defined(EXCLUDE_CSR_BT_RFC_MODULE)
                    if (inst->connectResultFunc)
                    { /* Cancel the Cm Rfc Search procedure         */
                        CsrBtCmSdcCancelRfcSearchReqSend(inst->appHandle, priInst->deviceAddr);
                    }
                    else
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
                    { /* Cancel the CM search procedure             */
                        CsrBtCmSdcCancelSearchReqSend(inst->appHandle, priInst->deviceAddr);
                    }
                }
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
                else
                { /* A 128bit CM SDP search must be cancel          */
#if !defined(EXCLUDE_CSR_BT_RFC_MODULE)
                    if (inst->connectResultFunc)
                    { /* Cancel the Cm Rfc uuid128 Search procedure */
                        CsrBtCmSdcCancelUuid128RfcSearchReqSend(inst->appHandle, priInst->deviceAddr);
                    }
                    else
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
                    { /* Cancel the Cm uuid128 Search procedure */
                        CsrBtCmSdcCancelUuid128SearchReqSend(inst->appHandle, priInst->deviceAddr);
                    }
                }
#endif                
                return FALSE;
            }
            case CMN_SDC_RFC_CANCEL_STATE:
            { /* The process is allready being cancelled            */
                return FALSE;
            }
            default:
            { /* CMN_SDC_RFC_IDLE_STATE, nothing to cancel          */
                return TRUE;
            }
        }
    }
    else
    { /* No inst data is allocated nothing to cancel                */
        return TRUE;
    }
}

#if !defined(EXCLUDE_CSR_BT_RFC_MODULE)
void * CsrBtUtilSdpRfcConInit(CmnRfcConSelectServiceHandleFuncType selectServiceHandler,
                              CsrBtUtilRfcConSetPortParFuncType    setPortParHandler,
                              CmnRfcConResultFuncType              rfcConResultHandler,
                              CsrSchedQid                               appHandle)
{
    return (CsrBtUtilSdpRfcInit(selectServiceHandler,
                                setPortParHandler,
                                rfcConResultHandler,
                                appHandle,
                                CSR_BT_SDC_RFC_UTIL_INST_ID_NOT_USED));
}

CsrBool CsrBtUtilRfcConStart(void                    * instData,
                      void                    * cmSdcRfcInstData,
                      CmnCsrBtLinkedListStruct * sdpTag,
                      CsrBtDeviceAddr            deviceAddr,
                      dm_security_level_t     secLevel,
                      CsrBool                  requestPortPar,
                      RFC_PORTNEG_VALUES_T     * portPar,
                      CsrUint16                mtu,
                      CsrUint8                 modemStatus,
                      CsrUint8                 mscTimeout)
{
    if (cmSdcRfcInstData)
    { /* inst data is present                                       */
        CmnSdcRfcInstType * inst = cmSdcRfcInstData;

        if (!inst->privateInst && inst->connectResultFunc)
        { /* Request the CM to register a protocol handle with
             RFCOMM. CM will return a CSR_BT_CM_REGISTER_CFM primitive
             with an assigned server channel number.                */
            CmnSdcRfcPriInstType * priInst;

            inst->privateInst     = (CmnSdcRfcPriInstType *) CsrPmemZalloc(sizeof(CmnSdcRfcPriInstType));
            priInst               = inst->privateInst;
            cmnSdcRfcInitPrivateInst(priInst);

            priInst->deviceAddr   = deviceAddr;
            priInst->sdpInTagList = sdpTag;
            priInst->secLevel     = secLevel;
            priInst->reqPortPar   = requestPortPar;
            if (modemStatus != 0xFF)
            {
                priInst->modemStatus  = modemStatus;
            }
            if (mscTimeout != 0)
            {
                priInst->scTimeout    = mscTimeout;
            }
            priInst->maxFrameSize = mtu;

            if (portPar)
            {
                priInst->validPortPar = TRUE;
                priInst->portPar      = *portPar;
            }

            priInst->localServerCh = cmnGetLocalServerChannel(priInst->sdpInTagList);

            if (priInst->localServerCh == CSR_BT_NO_SERVER)
            { /* Must obtain a local server channel                 */
                priInst->state          = CMN_SDC_RFC_REGISTER_STATE;
                priInst->obtainServer   = TRUE;
                CsrBtCmContextRegisterReqSend(inst->appHandle, inst->instId);
            }
            else
            { /* A local server channel is already present. Start
                 the CmRfcSearch                                    */
                if (cmnSdcRfcStartSearch(inst, priInst))
                { /* The CmRfcSearch is started                     */
                    priInst->state = CMN_SDC_RFC_SEARCH_STATE;
                    return FALSE;
                }
                else
                { /* The sdpTag could not be interpret correct.     */
                    cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                        CSR_BT_RESULT_CODE_CM_INTERNAL_ERROR,
                                        CSR_BT_SUPPLIER_CM, FALSE);

                    return TRUE;
                }
            }
        }
        else
        { /* The procedure is allready running. Just ignore this
             request, but make sure that the pointer *sdpTag
             is CsrPmemFree to prevent a memoryleak                       */
            CsrBtUtilBllFreeLinkedList(&sdpTag, CsrBtUtilBllPfreeWrapper);
        }
        return FALSE;
    }
    else
    { /* No cmSdcRfcInstData, make sure that the pointer *sdpTag
         is CsrPmemFree to prevent a memoryleak                           */
        CsrBtUtilBllFreeLinkedList(&sdpTag, CsrBtUtilBllPfreeWrapper);
        return TRUE;
    }
}

CsrBool CsrBtUtilRfcConVerifyCmMsg(void *msg)
{
    if (CsrBtUtilSdcVerifyCmMsg(msg))
    {
        return TRUE;
    }
    else
    {
        CsrBtCmPrim *primType = (CsrPrim *) msg;
        CsrUint16  index     = (CsrUint16) (*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST);

        if (*primType >= CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST &&
            *primType <= CSR_BT_CM_RFC_PRIM_UPSTREAM_HIGHEST &&
            cmnRfcCmMsgHandlers[index] != NULL)
        { /* This library is able to handle this message            */
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

CsrBool CsrBtUtilRfcConCmMsgHandler(void *instData, void *cmSdcRfcInstData, void *msg)
{
    return (cmnCmMsgHandler(instData, cmSdcRfcInstData, msg, TRUE));
}

CsrBool CsrBtUtilRfcConCancel(void *instData, void *cmSdcRfcInstData)
{
    if (cmnSdcRfcVerifyInstData(cmSdcRfcInstData))
    {
        if (!CsrBtUtilSdcSearchCancel(instData, cmSdcRfcInstData))
        { /* The cancel procedure has been initiated by this
             function. Return FALSE to indicate that the cancelled
             but the connect procedure is not finished yet          */
            return FALSE;
        }
        else
        {
            CmnSdcRfcInstType    * inst    = cmSdcRfcInstData;
            CmnSdcRfcPriInstType * priInst = inst->privateInst;

            switch(priInst->state)
            {
                case CMN_SDC_RFC_REGISTER_STATE:
                { /* The CM has been requested to register a protocol
                     handle with RFCOMM. Change the state to indicate
                     that the connect procedure has been cancelled
                     and wait for the CSR_BT_CM_REGISTER_CFM message       */
                    priInst->state = CMN_SDC_RFC_CANCEL_STATE;
                    return FALSE;
                }
                case CMN_SDC_RFC_SELECT_SERVICE_RECORD_HANDLE_STATE:
                { /* This procedure has been cancelled while waiting
                     for the profile to select a service record
                     handle index. Right now the SDP connection is
                     closed which means that the function which
                     indicates that the connect procedure has been
                     cancelled can be called direct                 */
                    cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                            CSR_BT_RESULT_CODE_CM_CANCELLED,
                                            CSR_BT_SUPPLIER_CM, TRUE);

                    return TRUE;
                }
                case CMN_SDC_RFC_CONNECT_STATE:
                case CMN_SDC_RFC_PORTNEG_PENDING_STATE:
                { /* The CM has been requested to initiate an RFCOMM
                     connection, or is waiting for the profile to
                     response the CSR_BT_CM_PORTNEG_IND message. Request
                     CM to cancel this connect procedure, and
                     change the state to indicate that the connect
                     procedure has been cancelled. Wait for the
                     CSR_BT_CM_CONNECT_CFM message                         */
                    priInst->state = CMN_SDC_RFC_CANCEL_STATE;
                    CsrBtCmCancelConnectReqSend(inst->appHandle,
                                priInst->localServerCh, priInst->deviceAddr);

                    return FALSE;
                }
                default:
                { /* CMN_SDC_RFC_IDLE_STATE, nothing to cancel      */
                    return TRUE;
                }
            }
        }
    }
    else
    { /* No inst data is allocated nothing to cancel                */
        return TRUE;
    }
}

CsrBool CsrBtUtilRfcConSetServiceHandleIndexList(void      * instData,
                                                void      * cmSdcRfcInstData,
                                                CsrUint16  * serviceHandleIndexList,
                                                CsrUint16  nofServiceHandleIndicis)
{
    if (cmnSdcRfcVerifyInstData(cmSdcRfcInstData))
    {
        CmnSdcRfcInstType    * inst         = cmSdcRfcInstData;
        CmnSdcRfcPriInstType * priInst      = inst->privateInst;

        priInst->numOfServiceHandleIndicis  = nofServiceHandleIndicis;
        priInst->serviceHandleIndexList     = serviceHandleIndexList;
        priInst->sdrEntryIndex              = 0;

        if (cmnSdcRfcGetSelectedServiceHandle(priInst))
        { /* A service record handle has been found. Request CM
             to initiate an RFC connection                          */
            priInst->state          = CMN_SDC_RFC_CONNECT_STATE;

            CsrBtCmContextConnectReqSend(inst->appHandle, priInst->localServerCh,
                                         priInst->serviceHandle, priInst->maxFrameSize,
                                         priInst->reqPortPar, priInst->validPortPar,
                                         priInst->portPar, priInst->secLevel, priInst->deviceAddr,
                                         inst->instId, priInst->modemStatus, priInst->breakSignal, priInst->scTimeout);

            return FALSE;
        }
        else
        { /* No service record handle match the index given from the
             profile, call the function which indicates that connect
             procedure failed                                       */
            cmnSdcRfcCallResultFunc(instData, inst, priInst,
                                    (CsrBtResultCode) SDC_NO_RESPONSE_DATA,
                                    CSR_BT_SUPPLIER_SDP_SDC, TRUE);
            return TRUE;
        }
    }
    else
    { /* No inst data is allocated.                                 */
         return TRUE;
    }
}

#ifdef CSR_BT_INSTALL_SDC_SET_PORT_PAR
CsrBool CsrBtUtilRfcConSetPortPar(void * cmSdcRfcInstData, RFC_PORTNEG_VALUES_T portPar)
{
    if (cmnSdcRfcVerifyInstData(cmSdcRfcInstData))
    {
        CmnSdcRfcInstType    * inst    = cmSdcRfcInstData;
        CmnSdcRfcPriInstType * priInst = inst->privateInst;

        if (priInst->state == CMN_SDC_RFC_PORTNEG_PENDING_STATE)
        { /* Response the portPar given from the profiles and goto
             CMN_SDC_RFC_CONNECT_STATE                              */
            priInst->state = CMN_SDC_RFC_CONNECT_STATE;
            CsrBtCmPortnegResSend(priInst->btConnId, &(portPar));
        }
        else
        { /* This function is called in a state where this util
             function is not ready to received it. Ignore it        */
            ;
        }
        return FALSE;
    }
    else
    { /* No inst data is allocated.                                 */
         return TRUE;
    }
}
#endif
#endif /*EXCLUDE_CSR_BT_RFC_MODULE */


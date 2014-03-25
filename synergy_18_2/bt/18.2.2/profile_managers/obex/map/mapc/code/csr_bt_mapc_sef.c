/****************************************************************************

               Copyright Cambridge Silicon Radio Limited and its licensors 2012. All rights reserved.


REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_mapc_handler.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sdc_support.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_assert.h"
#ifdef CSR_AMP_ENABLE
#include "csr_bt_ampm_private_lib.h"
#endif /* CSR_AMP_ENABLE */

void CsrBtMapcMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_MAPC_PRIM, msg);
}

/* MAPC OBEX server needed stuff begin */
static void csrBtObexMapcServerStart(MapcInstanceData *pInst);
static void csrBtMapcObexServerMakeNotConnectable(MapcInstanceData *pInst, void *msg);
/* MAPC OBEX server needed stuff end */


#define MAPC_MAS_TARGET {0xbb,0x58,0x2b,0x40,0x42,0x0c,0x11,0xdb,0xb0,0xde,0x08,0x00,0x20,0x0c,0x9a,0x66}
#define MAPC_MNS_TARGET {0xbb,0x58,0x2b,0x41,0x42,0x0c,0x11,0xdb,0xb0,0xde,0x08,0x00,0x20,0x0c,0x9a,0x66}
#define MAPC_TARGET_LENGTH 0x10

static CsrBool csrBtMapcExtractMasSdpParameters(MapcInstanceData *pInst,
                                                CmnCsrBtLinkedListStruct *sdpTagList, 
                                                CsrUint16 sdpRecordIndex,
                                                CsrBtDeviceName *serviceName, 
                                                CsrUint8 *masInstanceId, 
                                                CsrBtMapMesSupport *supportedMessages)
{
    CsrBool      gotValidSdpRecord = FALSE;
    CsrBtUuid32  tmpUuid = 0;
    CsrUint16    tmpResult;
    CsrUint16    dummy1,dummy2;
    CsrUint8     *string;
    CsrUint16    stringLen;
    CsrUint32    returnValue;

    CsrMemSet(*serviceName, 0, sizeof(*serviceName));
    *masInstanceId = 0;
    *supportedMessages = CSR_BT_MAP_NO_TYPE_SUPPORT;

    if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList,sdpRecordIndex, &tmpUuid, &tmpResult, &dummy1, &dummy2))
    {
        if (tmpResult == SDR_SDC_SEARCH_SUCCESS)
        {
            if (tmpUuid == CSR_BT_OBEX_MESSAGE_ACCESS_SERVER_UUID)
            {
                gotValidSdpRecord = TRUE;
                if (TRUE == CsrBtUtilSdrGetStringAttributeFromAttributeUuid(sdpTagList, sdpRecordIndex, CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER, &string, &stringLen))
                {
                    if (stringLen > CSR_BT_MAX_FRIENDLY_NAME_LEN)
                    {
                        stringLen = CSR_BT_MAX_FRIENDLY_NAME_LEN;
                    }
                    CsrMemCpy((CsrUint8 *) *serviceName, string, stringLen);
                    CsrUtf8StrTruncate(*serviceName, stringLen);
                }

                if (TRUE == CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(sdpTagList, sdpRecordIndex,
                                            CSR_BT_MAS_INSTANCE_ID_ATTRIBUTE_IDENTIFIER, &returnValue))
                {
                    *masInstanceId = (CsrUint8) returnValue;
                }

                if (TRUE == CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(sdpTagList, sdpRecordIndex,
                                            CSR_BT_SUPPORTED_MESSAGE_TYPES_ATTRIBUTE_IDENTIFIER, &returnValue))
                {
                    *supportedMessages = (CsrUint8) returnValue;
                }
            }
        }
    }
    return gotValidSdpRecord;
}

static void csrBtMapcConnectResultHandler(void                       *instData,
                                            CsrBtResultCode          resultCode,
                                            CsrBtSupplier            resultSupplier,
                                            CsrBtDeviceAddr          deviceAddr,
                                            CsrBtConnId              cid,
                                            CsrUint16                maxPeerObexPacketLength,
                                            CmnCsrBtLinkedListStruct *sdpTag,
                                            CsrUint16                obexPacketLength,
                                            CsrUint8                 *obexPacket)
{
    MapcInstanceData * pInst    = instData;

    CsrBtMapcConnectCfm *pMsg   = (CsrBtMapcConnectCfm *) CsrPmemAlloc(sizeof(CsrBtMapcConnectCfm));
    pMsg->type                  = CSR_BT_MAPC_CONNECT_CFM;
    pMsg->instanceId            = pInst->mapcInstanceId;
    csrBtMapcExtractMasSdpParameters(pInst,sdpTag,0,&pMsg->serviceName,&pMsg->masInstanceId,&pMsg->supportedMessages);
    pMsg->masInstanceId         = pInst->selectedMasInstanceId;
    pMsg->obexPeerMaxPacketSize = maxPeerObexPacketLength;
    pMsg->resultCode            = resultCode;
    pMsg->resultSupplier        = resultSupplier;
    pMsg->btConnId              = cid;

    CsrBtMapcMessagePut(pInst->appHandle, pMsg);

    if((resultCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE) && (resultSupplier == CSR_BT_SUPPLIER_IRDA_OBEX))
    {
        pInst->isMasConnected = TRUE;
#ifdef CSR_AMP_ENABLE
        pInst->masterBtConnId = cid;
#endif /* CSR_AMP_ENABLE */
    }
    else
    {
        pInst->isMasConnected = FALSE;
    }
    /* MNS registration is always OFF by default */
    pInst->notificationRegistrationOn = FALSE;

    CsrBtUtilBllFreeLinkedList(&(sdpTag), CsrBtUtilBllPfreeWrapper);

    CsrPmemFree(pInst->masInstToServiceMap);
    pInst->masInstToServiceMap = NULL;

    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(deviceAddr);
    CsrPmemFree(obexPacket);
}

static void csrBtMapcDisconnectResultHandler(void           *instData,
                                            CsrBtReasonCode reasonCode,
                                            CsrBtSupplier   reasonSupplier,
                                            CsrUint8        *obexPacket,
                                            CsrUint16       obexPacketLength)
{
    MapcInstanceData * pInst        = instData;
    CsrBtMapcDisconnectInd *pMsg    = (CsrBtMapcDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtMapcDisconnectInd));
    pMsg->type                      = CSR_BT_MAPC_DISCONNECT_IND;
    pMsg->instanceId                = pInst->mapcInstanceId;
    pMsg->reasonCode                = reasonCode; 
    pMsg->reasonSupplier            = reasonSupplier;
    CsrBtMapcMessagePut(pInst->appHandle, pMsg);

    pInst->isMasConnected = FALSE;

    if (pInst->activated)
    {
        csrBtMapcObexServerMakeNotConnectable(pInst, NULL);
    }
#ifdef CSR_AMP_ENABLE
    pInst->masterBtConnId = 0;
#endif /* CSR_AMP_ENABLE */
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtMapcSendSelectMasInstanceInd(MapcInstanceData *pInst,
                                              CsrBtMapcMasInstance *masInstanceList,
                                              CsrUint16           masInstanceListSize)
{
    CsrBtMapcSelectMasInstanceInd *pMsg   = (CsrBtMapcSelectMasInstanceInd *) CsrPmemAlloc(sizeof(CsrBtMapcSelectMasInstanceInd));
    pMsg->type                  = CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND;
    pMsg->instanceId            = pInst->mapcInstanceId;
    pMsg->masInstanceList       = masInstanceList;
    pMsg->masInstanceListSize   = masInstanceListSize;

    CsrBtMapcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtMapcSelectMasInstanceIdHandler(void                     *instData,
                                               CsrUint16                entriesInSdpTaglist,
                                               CmnCsrBtLinkedListStruct *sdpTagList)
{
    MapcInstanceData * pInst    = instData;
    CsrUint16 sdpRecordIndex;
    CsrUint16    idx = 0;
    CsrBool      gotValidSdpRecord = FALSE;
    CsrBtMapcMasInstance *masInstanceList =  (CsrBtMapcMasInstance *) CsrPmemAlloc(sizeof(CsrBtMapcMasInstance) * entriesInSdpTaglist);

    pInst->masInstToServiceMapLength = entriesInSdpTaglist;
    pInst->masInstToServiceMap =   (CsrBtMapcMasInstIdMap *)CsrPmemAlloc(sizeof(CsrBtMapcMasInstIdMap) * entriesInSdpTaglist);

    for (sdpRecordIndex=0; sdpRecordIndex < entriesInSdpTaglist; sdpRecordIndex++ )
    {
        if (csrBtMapcExtractMasSdpParameters(pInst,sdpTagList,sdpRecordIndex,&masInstanceList[idx].serviceName,&masInstanceList[idx].masInstanceId,&masInstanceList[idx].supportedMessages))
        {
            gotValidSdpRecord = TRUE;
            pInst->masInstToServiceMap[idx].serviceIdx = sdpRecordIndex;
            pInst->masInstToServiceMap[idx].masInstanceId = masInstanceList[idx].masInstanceId;

            idx++;
        }
    }

    if(gotValidSdpRecord)
    {
        csrBtMapcSendSelectMasInstanceInd(pInst, masInstanceList, entriesInSdpTaglist);
    }
    else
    {
        ObexUtilSetServiceHandleIndexListResponse(pInst->obexClientInst, NULL, 0);
        CsrPmemFree(masInstanceList);
    }
}

static void csrBtMapcSetFolderResultHandler(void  *    instData,
                                      CsrUint8    responseCode,
                                      CsrUint8 * obexPacket,
                                      CsrUint16   obexPacketLength)
{
    MapcInstanceData * pInst = instData;
    CsrBtMapcSetFolderCfm *pMsg   = (CsrBtMapcSetFolderCfm *) CsrPmemAlloc(sizeof(CsrBtMapcSetFolderCfm));
    pMsg->type              = CSR_BT_MAPC_SET_FOLDER_CFM;
    pMsg->instanceId            = pInst->mapcInstanceId;
    pMsg->result            = responseCode;
    CsrBtMapcMessagePut(pInst->appHandle, pMsg);

    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtMapcSetBackFolderResultHandler(void  *    instData,
                                          CsrUint8    responseCode,
                                          CsrUint8 * obexPacket,
                                          CsrUint16   obexPacketLength)
{
    MapcInstanceData * pInst     = instData;
    CsrBtMapcSetBackFolderCfm *pMsg   = CsrPmemAlloc(sizeof(CsrBtMapcSetBackFolderCfm));
    pMsg->type                  = CSR_BT_MAPC_SET_BACK_FOLDER_CFM;
    pMsg->instanceId            = pInst->mapcInstanceId;
    pMsg->result                = responseCode;
    CsrBtMapcMessagePut(pInst->appHandle, pMsg);

    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtMapcSetRootFolderResultHandler(void  *    instData,
                                          CsrUint8    responseCode,
                                          CsrUint8 * obexPacket,
                                          CsrUint16   obexPacketLength)
{
    MapcInstanceData * pInst     = instData;
    CsrBtMapcSetRootFolderCfm *pMsg   = (CsrBtMapcSetRootFolderCfm *) CsrPmemAlloc(sizeof(CsrBtMapcSetRootFolderCfm));
    pMsg->type                  = CSR_BT_MAPC_SET_ROOT_FOLDER_CFM;
    pMsg->instanceId            = pInst->mapcInstanceId;
    pMsg->result                = responseCode;
    CsrBtMapcMessagePut(pInst->appHandle, pMsg);

    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

CsrUint16 CsrBtObexUtilAddApplicationParametersHeaderField(CsrUint8 **appHeader, CsrUint16 offset, CsrUint8 tagId, CsrUint8 tagLength, CsrUint8 *tagValue)
{
    CsrUint16 retLength = 0;
    
    if(*appHeader == NULL)
    { /* First entry in the header */
        offset = 3; 
        retLength = 3 + 2 + tagLength;
        *appHeader = CsrPmemAlloc(retLength);
        (*appHeader)[0] = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
    }
    else
    { /* Not the first entry in the header, we'll append this new tag to the end of the existing header */
        CsrUint8 *tempAppHeader = *appHeader;
        retLength = 2 + tagLength + offset;
        *appHeader = CsrPmemAlloc(retLength);
        CsrMemCpy(*appHeader, tempAppHeader, offset);
        CsrPmemFree(tempAppHeader);
    }

    /* Update full header length */
    CSR_BT_OBEX_SET_WORD_AT(*appHeader, 1, retLength);

    /* Insert tagId */
    (*appHeader)[offset] = tagId;

    /* Insert tagLength */
    (*appHeader)[offset + 1] = tagLength;

    if(tagLength > 0)
    {
        /* Insert tagValue */
        CsrMemCpy(&(*appHeader)[offset + 2], tagValue, tagLength);
    }
    
    return retLength;
}

CsrBool CsrBtObexUtilExtractApplicationParametersHeaderValueField(CsrUint8 *obexPacket, CsrUint16 packetLength, CsrUint8 tagId, CsrUint8 *tagLength, CsrUint8 **tagValue)
{
    CsrBool found = FALSE;
    *tagLength = 0;
    *tagValue = NULL;

    if (obexPacket)
    {
        CsrUint16 index = 3; /* start possition of a normal OBEX response header */
        CsrUint16 headerLength = 0;

        while(index < packetLength)
        {
            if(obexPacket[index++] == CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER)
            {
                headerLength = (CsrUint16)CSR_BT_OBEX_GET_WORD_AT(obexPacket, index);
                break;
            }
        }

        if(headerLength)
        {
            for(index += 2; ((index <packetLength) && (found == FALSE)); index++)
            {            
                if(obexPacket[index] == tagId)
                { 
                    *tagLength = obexPacket[index + 1];
                    *tagValue = &obexPacket[index + 2];
                    found = TRUE;
                }
                else
                {
                    index += obexPacket[index + 1] + 1; /* skip to next tag */
                }
            }
        }
        else
        {
            CSR_BT_GENERAL_WARNING("MAPC", 0, 0, "Application response header is not found\n");
        }
    }
    return found;
}


static void csrBtMapcGetFolderListingResultHandler(void        *instData,
                                                   CsrUint8    responseCode,
                                                   CsrBool     bodyHeader,
                                                   CsrUint16   bodyLength,
                                                   CsrUint16   bodyOffset,
                                                   CsrUint8  *obexPacket,
                                                   CsrUint16   obexPacketLength)
{
    MapcInstanceData * pInst = instData;

    if(pInst->fullSize == 0)
    {
        CsrUint8 tagLength;
        CsrUint8 *tagValue;
        
        if(CsrBtObexUtilExtractApplicationParametersHeaderValueField(obexPacket, obexPacketLength,
            CSR_BT_MAP_TAG_ID_FOLDER_LISTING_SIZE, &tagLength, &tagValue))
        {
            if(tagLength == CSR_BT_MAP_TAG_ID_LENGTH_FOLDER_LISTING_SIZE && tagValue)
            {
                pInst->fullSize = CSR_BT_OBEX_GET_WORD_AT(tagValue, 0);
            }
            else
            {
                CSR_BT_GENERAL_WARNING("MAPC", 0, 0, "FOLDER_LISTING_SIZE tag is found but length field is wrong\n");
            }
        }
        else
        {
            CSR_BT_GENERAL_WARNING("MAPC", 0, 0, "Application response header is not found\n");
        }
    }
    
    pInst->partialSize += bodyLength;

    if (responseCode != CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        CsrBtMapcGetFolderListingCfm *pMsg = (CsrBtMapcGetFolderListingCfm *) CsrPmemAlloc(sizeof(CsrBtMapcGetFolderListingCfm));

        pMsg->type                    = CSR_BT_MAPC_GET_FOLDER_LISTING_CFM;
        pMsg->instanceId              = pInst->mapcInstanceId;
        pMsg->result                  = responseCode;
        pMsg->fullFolderListingSize   = pInst->partialSize; 
        pMsg->bodyLength              = bodyLength; 
        pMsg->bodyOffset              = bodyOffset;  
        pMsg->payload                 = obexPacket;
        pMsg->payloadLength           = obexPacketLength;
        CsrBtMapcMessagePut(pInst->appHandle, pMsg);
    }
    else
    {
        CsrBtMapcGetFolderListingInd *pMsg = (CsrBtMapcGetFolderListingInd *) CsrPmemAlloc(sizeof(CsrBtMapcGetFolderListingInd));

        pMsg->type                    = CSR_BT_MAPC_GET_FOLDER_LISTING_IND;
        pMsg->instanceId              = pInst->mapcInstanceId;
        pMsg->fullFolderListingSize   = pInst->fullSize;
        pMsg->folderListingRetrieved  = pInst->partialSize;
        pMsg->bodyLength              = bodyLength; 
        pMsg->bodyOffset              = bodyOffset;  
        pMsg->payload                 = obexPacket;
        pMsg->payloadLength           = obexPacketLength;
        CsrBtMapcMessagePut(pInst->appHandle, pMsg);
    }
}

static void csrBtMapcGetMessageListingResultHandler(void        *instData,
                                                    CsrUint8    responseCode,
                                                    CsrBool     bodyHeader,
                                                    CsrUint16   bodyLength,
                                                    CsrUint16   bodyOffset,
                                                    CsrUint8  *obexPacket,
                                                    CsrUint16   obexPacketLength)
{
    MapcInstanceData * pInst = instData;
    CsrUint16 size = 0;
    if(pInst->fullSize == 0)
    {
        CsrUint8 tagLength;
        CsrUint8 *tagValue;
        
        if(CsrBtObexUtilExtractApplicationParametersHeaderValueField(obexPacket, obexPacketLength,
            CSR_BT_MAP_TAG_ID_MESSAGES_LISTING_SIZE, &tagLength, &tagValue))
        {
            if(tagLength == CSR_BT_MAP_TAG_ID_LENGTH_MESSAGES_LISTING_SIZE && tagValue)
            {
                pInst->fullSize = CSR_BT_OBEX_GET_WORD_AT(tagValue, 0);
            }
            else
            {
                CSR_BT_GENERAL_WARNING("MAPC", 0, 0, "MESSAGES_LISTING_SIZE tag is found but length field is wrong\n");
            }
        }
        else
        {
            CSR_BT_GENERAL_WARNING("MAPC", 0, 0, "Application response header is not found\n");
        }
    }

    if(pInst->newMessage == 0xFF)
    {
        CsrUint8 tagLength;
        CsrUint8 *tagValue;
        
        if(CsrBtObexUtilExtractApplicationParametersHeaderValueField(obexPacket, obexPacketLength,
            CSR_BT_MAP_TAG_ID_NEW_MESSAGE, &tagLength, &tagValue))
        {
            if(tagLength == CSR_BT_MAP_TAG_ID_LENGTH_NEW_MESSAGE && tagValue)
            {
                pInst->newMessage = CSR_BT_OBEX_GET_BYTE_AT(tagValue, 0);
            }
            else
            {
                CSR_BT_GENERAL_WARNING("MAPC", 0, 0, "NEW_MESSAGE tag is found but length field is wrong\n");
            }
        }
        else
        {
            CSR_BT_GENERAL_WARNING("MAPC", 0, 0, "Application response header is not found\n");
        }
    }

    if (CsrStrCmp((char*)pInst->mseTime,"") == 0) /* If 0 is returned -> First time the obex packet contain MSETime */
    { 
        CsrUint8 tagLength = 0;
        CsrUint8 *tagValue = NULL;

        if(CsrBtObexUtilExtractApplicationParametersHeaderValueField(obexPacket, obexPacketLength,
            CSR_BT_MAP_TAG_ID_MSE_TIME, &tagLength, &tagValue))
        {
            if(tagLength != 0 && tagValue)
            {
                CsrPmemFree(pInst->mseTime);
                pInst->mseTime = CsrPmemAlloc(tagLength+1);
                CsrMemCpy(pInst->mseTime,tagValue,tagLength);
                pInst->mseTime[tagLength]=0;
            }
            else
            {
                CSR_BT_GENERAL_WARNING("MAPC", 0, 0, "MSE tag is found but length field is wrong\n");
            }
        }
        else
        {
            CSR_BT_GENERAL_WARNING("MAPC", 0, 0, "Application response header is not found\n");
        }
    }
    
    pInst->partialSize += bodyLength;

    if (responseCode != CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {       
        CsrBtMapcGetMessageListingCfm *pMsg = (CsrBtMapcGetMessageListingCfm *) CsrPmemAlloc(sizeof(CsrBtMapcGetMessageListingCfm));

        if(pInst->partialSize == 0)
        {
            size = pInst->fullSize;
        }
        else
        {
           size = pInst->partialSize;
        }

        pMsg->type                    = CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM;
        pMsg->instanceId              = pInst->mapcInstanceId;
        pMsg->result                  = responseCode;
        pMsg->newMessages             = pInst->newMessage;
        pMsg->fullMessageListingSize  = size; 
        pMsg->mseTime                 = CsrUtf8StrDup((CsrUtf8String*)pInst->mseTime);
        pMsg->bodyLength              = bodyLength; 
        pMsg->bodyOffset              = bodyOffset;  
        pMsg->payload                 = obexPacket;
        pMsg->payloadLength           = obexPacketLength;
        CsrBtMapcMessagePut(pInst->appHandle, pMsg);

        CsrPmemFree(pInst->mseTime);
        pInst->mseTime = NULL;
    }
    else
    {
        CsrBtMapcGetMessageListingInd *pMsg = (CsrBtMapcGetMessageListingInd *) CsrPmemAlloc(sizeof(CsrBtMapcGetMessageListingInd));

        pMsg->type                    = CSR_BT_MAPC_GET_MESSAGE_LISTING_IND;
        pMsg->instanceId              = pInst->mapcInstanceId;
        pMsg->newMessages             = pInst->newMessage;
        pMsg->fullMessageListingSize  = pInst->fullSize;
        pMsg->messageListingRetrieved = pInst->partialSize;
        pMsg->mseTime                 = CsrUtf8StrDup(pInst->mseTime);
        pMsg->bodyLength              = bodyLength; 
        pMsg->bodyOffset              = bodyOffset;  
        pMsg->payload                 = obexPacket;
        pMsg->payloadLength           = obexPacketLength;
        CsrBtMapcMessagePut(pInst->appHandle, pMsg);
    }
}

static void csrBtMapcGetMessageResultHandler(void       *instData,
                                             CsrUint8   responseCode,
                                             CsrBool    bodyHeader,
                                             CsrUint16  bodyLength,
                                             CsrUint16  bodyOffset,
                                             CsrUint8 *obexPacket,
                                             CsrUint16  obexPacketLength)
{
    MapcInstanceData * pInst = instData;

    if(pInst->fractionDeliver == 0xFE)
    {
        CsrUint8 tagLength;
        CsrUint8 *tagValue;
        
        if(CsrBtObexUtilExtractApplicationParametersHeaderValueField(obexPacket, obexPacketLength,
            CSR_BT_MAP_TAG_ID_FRACTION_DELIVER, &tagLength, &tagValue))
        {
            if(tagLength == CSR_BT_MAP_TAG_ID_LENGTH_FRACTION_DELIVER && tagValue)
            {
                pInst->fractionDeliver = CSR_BT_OBEX_GET_BYTE_AT(tagValue, 0);
            }
            else
            {                
                CSR_BT_GENERAL_WARNING("MAPC", 0, 0, "FRACTION_DELIVER tag is found but length field is wrong so we now set it as unspecified \n");
                pInst->fractionDeliver = CSR_BT_MAP_FRACTION_DEL_NOT_SPECIFIED;
            }
        }
        else
        {
            pInst->fractionDeliver = CSR_BT_MAP_FRACTION_DEL_NOT_SPECIFIED;
        }
    }


    if (responseCode != CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        CsrBtMapcGetMessageCfm *pMsg      = (CsrBtMapcGetMessageCfm *) CsrPmemAlloc(sizeof(CsrBtMapcGetMessageCfm));
        pMsg->type              = CSR_BT_MAPC_GET_MESSAGE_CFM;
        pMsg->instanceId        = pInst->mapcInstanceId;
        pMsg->result            = responseCode;
        pMsg->fractionDeliver   = pInst->fractionDeliver;
        pMsg->bodyLength        = bodyLength; 
        pMsg->bodyOffset        = bodyOffset;  
        pMsg->payload           = obexPacket;
        pMsg->payloadLength     = obexPacketLength;
        CsrBtMapcMessagePut(pInst->appHandle, pMsg);
        
    }
    else
    {
        CsrBtMapcGetMessageInd *pMsg  = (CsrBtMapcGetMessageInd *) CsrPmemAlloc(sizeof(CsrBtMapcGetMessageInd));
        pMsg->type              = CSR_BT_MAPC_GET_MESSAGE_IND;
        pMsg->instanceId        = pInst->mapcInstanceId;
        pMsg->bodyLength        = bodyLength; 
        pMsg->bodyOffset        = bodyOffset;  
        pMsg->payload           = obexPacket;
        pMsg->payloadLength     = obexPacketLength;
        CsrBtMapcMessagePut(pInst->appHandle, pMsg);
    }
}

static void csrBtMapcSetMessageStatusResultHandler(void  *    instData,
                                        CsrUint8    responseCode,
                                        CsrUint8 * obexPacket,
                                        CsrUint16   obexPacketLength)
{
    MapcInstanceData *pInst    = instData;
    CsrBtMapcSetMessageStatusCfm *pMsg     = (CsrBtMapcSetMessageStatusCfm *) CsrPmemAlloc(sizeof(CsrBtMapcSetMessageStatusCfm));

    pMsg->type                  = CSR_BT_MAPC_SET_MESSAGE_STATUS_CFM;
    pMsg->instanceId            = pInst->mapcInstanceId;
    pMsg->result                = responseCode;

    CsrBtMapcMessagePut(pInst->appHandle, pMsg);
    CsrPmemFree(obexPacket);
    CSR_UNUSED(obexPacketLength);
}

static void csrBtMapcSetMessageStatusContinueHandler(void  *    instData,
                                             CsrUint16   obexPacketLength)
{
    MapcInstanceData *pInst    = instData;
    CsrUint8 *payload = CsrPmemAlloc(1);
    payload[0] = 0x30; /* Filler byte */

    ObexUtilPutContinueRequest(pInst->obexClientInst, TRUE, &payload, 1);
}

static void csrBtMapcPushMessageResultHandler(void  *    instData,
                                           CsrUint8    responseCode,
                                           CsrUint8 * obexPacket,
                                           CsrUint16   obexPacketLength)
{
    MapcInstanceData *pInst    = instData;
    CsrBtMapcPushMessageCfm *pMsg  = (CsrBtMapcPushMessageCfm *) CsrPmemAlloc(sizeof(CsrBtMapcPushMessageCfm));
    CsrUint16  nameOffset;

    pMsg->type                  = CSR_BT_MAPC_PUSH_MESSAGE_CFM;
    pMsg->instanceId            = pInst->mapcInstanceId;
    pMsg->result                = responseCode;

    if(CsrBtObexGetNameOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &nameOffset))
    {
        pMsg->messageHandle = (CsrUtf8String*) CsrUcs2ByteString2Utf8((const CsrUcs2String *) &obexPacket[nameOffset]);
    }
    else
    {
        pMsg->messageHandle = NULL;
    }

    CsrBtMapcMessagePut(pInst->appHandle, pMsg);
    CsrPmemFree(obexPacket);
    CSR_UNUSED(obexPacketLength);
}

static void csrBtMapcPushMessageContinueHandler(void  *    instData,
                                             CsrUint16   obexPacketLength)
{
    MapcInstanceData *pInst    = instData;
    CsrBtMapcPushMessageInd *pMsg  = (CsrBtMapcPushMessageInd *) CsrPmemAlloc(sizeof(CsrBtMapcPushMessageInd));

    pMsg->type                  = CSR_BT_MAPC_PUSH_MESSAGE_IND;
    pMsg->instanceId            = pInst->mapcInstanceId;
    pMsg->maxAllowedPayloadSize = obexPacketLength;

    CsrBtMapcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtMapcUpdateInboxResultHandler(void  *    instData,
                                           CsrUint8    responseCode,
                                           CsrUint8 * obexPacket,
                                           CsrUint16   obexPacketLength)
{
    MapcInstanceData * pInst   = instData;
    CsrBtMapcUpdateInboxCfm *pMsg  = (CsrBtMapcUpdateInboxCfm *) CsrPmemAlloc(sizeof(CsrBtMapcUpdateInboxCfm));
    pMsg->type                  = CSR_BT_MAPC_UPDATE_INBOX_CFM;
    pMsg->instanceId            = pInst->mapcInstanceId;
    pMsg->result                = responseCode;

    CsrBtMapcMessagePut(pInst->appHandle, pMsg);
    CsrPmemFree(obexPacket);
    CSR_UNUSED(obexPacketLength);
}

static void csrBtMapcUpdateInboxContinueHandler(void  *    instData,
                                             CsrUint16   obexPacketLength)
{
    MapcInstanceData *pInst    = instData;
    CsrUint8 *payload = CsrPmemAlloc(1);
    payload[0] = 0x30; /* Filler byte */

    ObexUtilPutContinueRequest(pInst->obexClientInst, TRUE, &payload, 1);
}

static void csrBtMapcNotificationRegistrationResultHandler(void  *    instData,
                                           CsrUint8    responseCode,
                                           CsrUint8 * obexPacket,
                                           CsrUint16   obexPacketLength)
{
    MapcInstanceData * pInst   = instData;
    CsrBtMapcNotificationRegistrationCfm *pMsg  = (CsrBtMapcNotificationRegistrationCfm *) CsrPmemAlloc(sizeof(CsrBtMapcNotificationRegistrationCfm));
    pMsg->type                  = CSR_BT_MAPC_NOTIFICATION_REGISTRATION_CFM;
    pMsg->instanceId            = pInst->mapcInstanceId;
    pMsg->result                = responseCode;

    if(responseCode != CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        csrBtMapcObexServerMakeNotConnectable(pInst, NULL);
    }

    CsrBtMapcMessagePut(pInst->appHandle, pMsg);
    CsrPmemFree(obexPacket);
    CSR_UNUSED(obexPacketLength);
}

static void csrBtMapcNotificationRegistrationContinueHandler(void  *    instData,
                                             CsrUint16   obexPacketLength)
{
    MapcInstanceData *pInst    = instData;
    CsrUint8 *payload = CsrPmemAlloc(1);
    payload[0] = 0x30; /* Filler byte */

    ObexUtilPutContinueRequest(pInst->obexClientInst, TRUE, &payload, 1);
}

static void csrBtMapcAbortResultHandler(void  *    instData,
                                    CsrUint8    responseCode,
                                    CsrUint8 * obexPacket,
                                    CsrUint16   obexPacketLength)
{
    MapcInstanceData * pInst     = instData;
    CsrBtMapcAbortCfm *pMsg      = (CsrBtMapcAbortCfm*) CsrPmemAlloc(sizeof(CsrBtMapcAbortCfm));
    pMsg->type                   = CSR_BT_MAPC_ABORT_CFM;
    pMsg->instanceId             = pInst->mapcInstanceId;
    CsrBtMapcMessagePut(pInst->appHandle, pMsg);
    
    CSR_UNUSED(responseCode);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtMapcEventNotificationResultHandler(void  *    instData,
                                          CsrUint32   lengthOfObject,
                                          CsrUint16   ucs2nameOffset,
                                          CsrUint16   bodyOffset,
                                          CsrUint16   bodyLength,
                                          CsrUint8 *payload,
                                          CsrUint16   payloadLength)
{
    MapcInstanceData * pInst = instData;
    CsrBtMapcEventNotificationInd *pMsg = pnew(CsrBtMapcEventNotificationInd);

    pMsg->type              = CSR_BT_MAPC_EVENT_NOTIFICATION_IND;
    pMsg->instanceId        = pInst->mapcInstanceId;
    pMsg->finalFlag         = pInst->putFinalFlag;
    pMsg->bodyOffset        = bodyOffset;  
    pMsg->bodyLength        = bodyLength; 
    pMsg->payload           = payload;
    pMsg->payloadLength     = payloadLength;
    
    CsrBtMapcMessagePut(pInst->appHandle, pMsg);
}


static void csrBtMapcSecurityInCfmSend(CsrSchedQid appHandle, CsrSchedQid instanceId, 
    CsrBtResultCode resultCode,  CsrBtSupplier resultSupplier)
{
    CsrBtMapcSecurityInCfm *pMsg    = (CsrBtMapcSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtMapcSecurityInCfm));
    pMsg->type                  = CSR_BT_MAPC_SECURITY_IN_CFM;
    pMsg->instanceId            = instanceId;
    pMsg->resultCode            = resultCode;
    pMsg->resultSupplier        = resultSupplier;
    CsrBtMapcMessagePut(appHandle, pMsg);
}

static void csrBtMapcSecurityOutCfmSend(CsrSchedQid appHandle, CsrSchedQid instanceId, 
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtMapcSecurityOutCfm *pMsg   = (CsrBtMapcSecurityOutCfm*)CsrPmemAlloc(sizeof(CsrBtMapcSecurityOutCfm));
    pMsg->type                  = CSR_BT_MAPC_SECURITY_OUT_CFM;
    pMsg->instanceId            = instanceId;
    pMsg->resultCode            = resultCode;
    pMsg->resultSupplier        = resultSupplier;
    CsrBtMapcMessagePut(appHandle, pMsg);
}

void CsrBtMapcGetInstanceIdsReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrBtMapcGetInstanceIdsReq *prim_req;
    CsrBtMapcGetInstanceIdsCfm *prim_cfm;
    CsrBtMapcInstancePool         *ptr;
    CsrUint8                    offset;

    if (pInst->mapcInstanceId == CSR_BT_MAPC_IFACEQUEUE)
    {
        prim_req=(CsrBtMapcGetInstanceIdsReq *) msg;

        prim_cfm = CsrPmemAlloc(sizeof(CsrBtMapcGetInstanceIdsCfm));

        prim_cfm->type = CSR_BT_MAPC_GET_INSTANCE_IDS_CFM;
        prim_cfm->instanceIdsListSize = pInst->numberOfMapcInstances;

        if (pInst->numberOfMapcInstances == 0)
        {
            prim_cfm->instanceIdsList = NULL;
        }
        else
        {
            prim_cfm->instanceIdsList = CsrPmemAlloc(sizeof(CsrSchedQid) *
                                              pInst->numberOfMapcInstances);

            ptr = pInst->mapcInstances;
            offset = 0;

            while(ptr)
            {
                CsrMemCpy(&prim_cfm->instanceIdsList[offset],
                       ptr->phandles,
                       sizeof(CsrSchedQid) * ptr->numberInPool);

                offset += ptr->numberInPool;
                ptr = ptr->next;
            }
        }

        CsrBtMapcMessagePut(prim_req->appHandle,prim_cfm);
    }
    else
    {
        CSR_BT_GENERAL_EXCEPTION("MAPC",0,CSR_BT_MAPC_GET_INSTANCE_IDS_REQ,"Task not MAPC-manager");
    }
}


CsrUint8 CsrBtMapcConnectReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrUint16  index;
    CsrUint8   headerPriorityTable[2]       = {CSR_BT_OBEX_UTIL_LENGTH_HEADER, CSR_BT_OBEX_UTIL_COUNT_HEADER};
    CsrUint8 tempTarget[MAPC_TARGET_LENGTH] = MAPC_MAS_TARGET;
    CsrUint8   numOfHeaders                 = 0;
    CsrBtMapcConnectReq *pMsg               = (CsrBtMapcConnectReq *) msg;
    CmnCsrBtLinkedListStruct * sdpTag       = NULL;
    CsrUint8 *target                        = (CsrUint8 *) CsrPmemAlloc(MAPC_TARGET_LENGTH);

    pInst->appHandle                        = pMsg->appHandle;
    pInst->windowSize = pMsg->windowSize;
    pInst->localSrmEnable = pMsg->srmEnable;

    CsrMemCpy(target, tempTarget, MAPC_TARGET_LENGTH);
    sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, CSR_BT_OBEX_MESSAGE_ACCESS_SERVER_UUID, &index);
    CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER, NULL, 0);
    CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_MAS_INSTANCE_ID_ATTRIBUTE_IDENTIFIER, NULL, 0);
    CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_SUPPORTED_MESSAGE_TYPES_ATTRIBUTE_IDENTIFIER, NULL, 0);
    CsrBtUtilSdrInsertLocalServerChannel(sdpTag, index, CSR_BT_NO_SERVER);

    if (pMsg->length > 0)
    {
        numOfHeaders = (CsrUint8)(numOfHeaders + 1); 
    }

    if (pMsg->count > 0)
    {
        numOfHeaders = (CsrUint8)(numOfHeaders + 1);
    }

    /* Only for the OBEX server begin */
    pInst->maxFrameSize = pMsg->maxPacketSize;

    /* Only for the OBEX server end*/
    CsrPmemFree(pInst->masInstToServiceMap);
    pInst->masInstToServiceMap = NULL;

    return(ObexUtilConnectRequest(pInst->obexClientInst, 
                                  &sdpTag, 
                                  pMsg->deviceAddr, 
                                  pInst->secOutgoing, 
                                  pMsg->maxPacketSize, 
                                  CSR_BT_MAPC_PROFILE_DEFAULT_MTU_SIZE, 
                                  &target, 
                                  MAPC_TARGET_LENGTH, 
                                  numOfHeaders, 
                                  headerPriorityTable, 
                                  CSR_BT_MAPC_LP_SUPERVISION_TIMEOUT, 
                                  NULL, 
                                  pMsg->length, 
                                  pMsg->count, 
                                  NULL,
                                  0,
                                  NULL,
                                  csrBtMapcConnectResultHandler, 
                                  NULL, 
                                  csrBtMapcDisconnectResultHandler, 
                                  csrBtMapcSelectMasInstanceIdHandler,
                                  pInst->windowSize,
                                  pInst->localSrmEnable));
}

CsrUint8 CsrBtMapcCancelConnectReqHandler(MapcInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return (ObexUtilDisconnectRequest(pInst->obexClientInst, FALSE, NULL));
}

CsrUint8 CsrBtMapcDisconnectReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrBtMapcDisconnectReq *pMsg = (CsrBtMapcDisconnectReq *) msg;

    return (ObexUtilDisconnectRequest(pInst->obexClientInst, pMsg->normalObexDisconnect, NULL));
}

static CsrUint16 csrBtMapcFindServiceHandleIndexFromMasInstanceId(MapcInstanceData *pInst,
                                                                  CsrUint16 masInstanceId)
{
    CsrUint16 i;
    CsrUint16 retVal = 0xFFFF;
    if(pInst->masInstToServiceMap)
    {
        for(i=0;i<pInst->masInstToServiceMapLength;i++)
        {
            if(pInst->masInstToServiceMap[i].masInstanceId == masInstanceId)
            {
                retVal = pInst->masInstToServiceMap[i].serviceIdx;
                break;
            }
        }

        CsrPmemFree(pInst->masInstToServiceMap);
        pInst->masInstToServiceMap = NULL;
    }

    return retVal; 
}

CsrUint8 CsrBtMapcSelectMasInstanceResHandler(MapcInstanceData *pInst, void *msg)
{
    CsrBtMapcSelectMasInstanceRes *pMsg = (CsrBtMapcSelectMasInstanceRes *) msg;
    CsrUint16 *serviceHandleIndexList = NULL;
    CsrUint16 nofServiceHandleIndicis = 0;

    if(pMsg->proceedWithConnection)
    {
        CsrUint16 tempServiceHandle = csrBtMapcFindServiceHandleIndexFromMasInstanceId(pInst, pMsg->masInstanceId);
        if(tempServiceHandle != 0xFFFF)
        {
            nofServiceHandleIndicis = 1;
            serviceHandleIndexList = CsrPmemAlloc(sizeof(CsrUint16));
            *serviceHandleIndexList = tempServiceHandle;
            pInst->selectedMasInstanceId = pMsg->masInstanceId;
        }
    }

    return (ObexUtilSetServiceHandleIndexListResponse(pInst->obexClientInst, &serviceHandleIndexList, nofServiceHandleIndicis));
}

/* Browsing */
CsrUint8 CsrBtMapcSetFolderReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[3]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtMapcSetFolderReq *pMsg           = (CsrBtMapcSetFolderReq *) msg;
    CsrUcs2String *folderName = CsrUtf82Ucs2ByteString((CsrUint8*)pMsg->folderName);

    CsrPmemFree(pMsg->folderName);
    return (ObexUtilSetPathRequest(pInst->obexClientInst, 0x02, headerPriorityTable, &folderName, NULL, 0, NULL, csrBtMapcSetFolderResultHandler));
}

CsrUint8 CsrBtMapcSetBackFolderReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[3]  = {CSR_BT_OBEX_UTIL_EMPTY_NAME_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CSR_UNUSED(msg);
    return (ObexUtilSetPathRequest(pInst->obexClientInst, 0x03, headerPriorityTable, NULL, NULL, 0, NULL, csrBtMapcSetBackFolderResultHandler));
}

CsrUint8 CsrBtMapcSetRootFolderReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[3]  = {CSR_BT_OBEX_UTIL_EMPTY_NAME_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CSR_UNUSED(msg);
    return (ObexUtilSetPathRequest(pInst->obexClientInst, 0x02, headerPriorityTable, NULL, NULL, 0, NULL, csrBtMapcSetRootFolderResultHandler));
}

CsrUint8 CsrBtMapcGetFolderListingReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER, CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtMapcGetFolderListingReq *pMsg       = (CsrBtMapcGetFolderListingReq *) msg;
    CsrUint8 *appHeader = NULL;
    CsrUint16 appHeaderLength = 0;
    CsrUint8 temp[2];

    /* Build applications parameter header */
    CSR_BT_OBEX_SET_WORD_AT(temp,0,pMsg->maxListCount);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(&appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_MAX_LIST_COUNT, CSR_BT_MAP_TAG_ID_LENGTH_MAX_LIST_COUNT, temp);

    CSR_BT_OBEX_SET_WORD_AT(temp,0,pMsg->listStartOffset);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(&appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_START_OFFSET, CSR_BT_MAP_TAG_ID_LENGTH_START_OFFSET, temp);

    pInst->fullSize = 0;
    pInst->partialSize = 0;
        
    return(ObexUtilGetRequest(pInst->obexClientInst, 
                              headerPriorityTable, 
                              (const CsrUint8*)("x-obex/folder-listing"), 
                              NULL, 
                              NULL, 
                              appHeaderLength, 
                              &appHeader, 
                              0, 
                              NULL, 
                              CSR_BT_OBEX_SRMP_INVALID,
                              csrBtMapcGetFolderListingResultHandler, 
                              NULL));
}

CsrUint8 CsrBtMapcGetFolderListingResHandler(MapcInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return(ObexUtilGetContinueRequest(pInst->obexClientInst, CSR_BT_OBEX_SRMP_INVALID));
}

CsrUint8 CsrBtMapcGetMessageListingReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER, CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtMapcGetMessageListingReq *pMsg       = (CsrBtMapcGetMessageListingReq *) msg;
    CsrUint8 *appHeader = NULL;
    CsrUint16 appHeaderLength = 0;
    CsrUint8 temp[4];
    CsrUcs2String *folderName = CsrUtf82Ucs2ByteString((CsrUint8*)pMsg->folderName);

    /* Build applications parameter header */
    CSR_BT_OBEX_SET_WORD_AT(temp,0,pMsg->maxListCount);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(&appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_MAX_LIST_COUNT, CSR_BT_MAP_TAG_ID_LENGTH_MAX_LIST_COUNT, temp);

    CSR_BT_OBEX_SET_WORD_AT(temp,0,pMsg->listStartOffset);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(&appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_START_OFFSET, CSR_BT_MAP_TAG_ID_LENGTH_START_OFFSET, temp);

    if(pMsg->maxSubjectLength > 0)
    {
        CSR_BT_OBEX_SET_BYTE_AT(temp,0,pMsg->maxSubjectLength);
        appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(&appHeader, appHeaderLength,
            CSR_BT_MAP_TAG_ID_SUBJECT_LENGTH, CSR_BT_MAP_TAG_ID_LENGTH_SUBJECT_LENGTH, temp);
    }
    
    CSR_BT_OBEX_SET_D_WORD_AT(temp,0,pMsg->parameterMask);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(&appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_PARAMETER_MASK, CSR_BT_MAP_TAG_ID_LENGTH_PARAMETER_MASK, temp);

    CSR_BT_OBEX_SET_BYTE_AT(temp,0,pMsg->filterMessageType);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(&appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_FILTER_MESSAGE_TYPE, CSR_BT_MAP_TAG_ID_LENGTH_FILTER_MESSAGE_TYPE, temp);

    if(pMsg->filterPeriodBegin)
    {
        appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(
            &appHeader, 
            appHeaderLength,
            CSR_BT_MAP_TAG_ID_FILTER_PERIOD_BEGIN,
            (CsrUint8) CsrStrLen((const char *)pMsg->filterPeriodBegin), 
            (CsrUint8*)pMsg->filterPeriodBegin);
    }
    
    if(pMsg->filterPeriodEnd)
    {
        appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(
            &appHeader, 
            appHeaderLength,
            CSR_BT_MAP_TAG_ID_FILTER_PERIOD_END,
            (CsrUint8) CsrStrLen((const char *)pMsg->filterPeriodEnd), 
            (CsrUint8*)pMsg->filterPeriodEnd);
    }

    CSR_BT_OBEX_SET_BYTE_AT(temp,0,pMsg->filterReadStatus);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(&appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_FILTER_READ_STATUS, CSR_BT_MAP_TAG_ID_LENGTH_FILTER_READ_STATUS, temp);

    if(pMsg->filterRecipient)
    {
        appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(
            &appHeader, appHeaderLength,
            CSR_BT_MAP_TAG_ID_FILTER_RECIPIENT, 
            (CsrUint8) CsrStrLen((const char *)pMsg->filterRecipient),
            (CsrUint8*)pMsg->filterRecipient);
    }
    
    if(pMsg->filterOriginator)
    {
        appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(
            &appHeader, appHeaderLength,
            CSR_BT_MAP_TAG_ID_FILTER_ORIGINATOR, 
            (CsrUint8) CsrStrLen((const char *)pMsg->filterOriginator), 
            (CsrUint8*)pMsg->filterOriginator);
    }

    CSR_BT_OBEX_SET_BYTE_AT(temp,0,pMsg->filterPriority);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(
        &appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_FILTER_PRIORITY, 
        CSR_BT_MAP_TAG_ID_LENGTH_FILTER_PRIORITY, 
        (CsrUint8*)temp);

    /* Check values used in the result handler */
    pInst->newMessage = 0xFF;
    pInst->fullSize = 0;
    pInst->partialSize = 0;
    pInst->mseTime = (CsrUtf8String*)CsrPmemAlloc(sizeof(CsrCharString));    
    *pInst->mseTime = '\0';        
    
    CsrPmemFree(pMsg->folderName);
    CsrPmemFree(pMsg->filterPeriodBegin);
    CsrPmemFree(pMsg->filterPeriodEnd);
    CsrPmemFree(pMsg->filterRecipient);
    CsrPmemFree(pMsg->filterOriginator);
    
    return(ObexUtilGetRequest(pInst->obexClientInst, 
                              headerPriorityTable, 
                              (const CsrUint8*)("x-bt/MAP-msg-listing"), 
                              &folderName, 
                              NULL, 
                              appHeaderLength, 
                              &appHeader, 
                              0, 
                              NULL,
                              CSR_BT_OBEX_SRMP_INVALID,
                              csrBtMapcGetMessageListingResultHandler, 
                              NULL));
}

CsrUint8 CsrBtMapcGetMessageListingResHandler(MapcInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return(ObexUtilGetContinueRequest(pInst->obexClientInst, CSR_BT_OBEX_SRMP_INVALID));
}


CsrUint8 CsrBtMapcGetMessageReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_TYPE_HEADER, CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtMapcGetMessageReq *pMsg    = (CsrBtMapcGetMessageReq *) msg;
    CsrUint8 *appHeader = NULL;
    CsrUint16 appHeaderLength = 0;
    CsrUint8 temp[1];
    CsrUcs2String *messageHandle = CsrUtf82Ucs2ByteString((CsrUint8*)pMsg->messageHandle);

    CSR_BT_OBEX_SET_BYTE_AT(temp,0,pMsg->attachment);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(
        &appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_ATTACHMENT, 
        CSR_BT_MAP_TAG_ID_LENGTH_ATTACHMENT, 
        temp);

    CSR_BT_OBEX_SET_BYTE_AT(temp,0,pMsg->charset);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(
        &appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_CHARSET, 
        CSR_BT_MAP_TAG_ID_LENGTH_CHARSET, 
        temp);

    if(pMsg->fractionRequest != CSR_BT_MAP_FRACTION_REQ_NOT_USED)
    {
        CSR_BT_OBEX_SET_BYTE_AT(temp,0,pMsg->fractionRequest);
        appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(
            &appHeader, appHeaderLength,
            CSR_BT_MAP_TAG_ID_FRACTION_REQUEST, 
            CSR_BT_MAP_TAG_ID_LENGTH_FRACTION_REQUEST, 
            temp);
    }

    pInst->fractionDeliver = 0xFE;   
    CsrPmemFree(pMsg->messageHandle);
    return(ObexUtilGetRequest(pInst->obexClientInst, 
                              headerPriorityTable, 
                              (const CsrUint8*)("x-bt/message"), 
                              &messageHandle, 
                              NULL, 
                              appHeaderLength, 
                              &appHeader, 
                              0, 
                              NULL,
                              CSR_BT_OBEX_SRMP_INVALID,
                              csrBtMapcGetMessageResultHandler, 
                              NULL));
}

CsrUint8 CsrBtMapcGetMessageResHandler(MapcInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return(ObexUtilGetContinueRequest(pInst->obexClientInst, CSR_BT_OBEX_SRMP_INVALID));
}

CsrUint8 CsrBtMapcSetMessageStatusReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[6]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_TYPE_HEADER, CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtMapcSetMessageStatusReq *pMsg         = (CsrBtMapcSetMessageStatusReq *) msg;
    CsrUint8 *appHeader = NULL;
    CsrUint16 appHeaderLength = 0;
    CsrUint8 temp[1];
    CsrUcs2String *messageHandle = CsrUtf82Ucs2ByteString((CsrUint8*)pMsg->messageHandle);

    CSR_BT_OBEX_SET_BYTE_AT(temp,0,pMsg->statusIndicator);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(
        &appHeader, 
        appHeaderLength,
        CSR_BT_MAP_TAG_ID_STATUS_INDICATOR, 
        CSR_BT_MAP_TAG_ID_LENGTH_STATUS_INDICATOR, 
        temp);

    CSR_BT_OBEX_SET_BYTE_AT(temp,0,pMsg->statusValue);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(
        &appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_STATUS_VALUE, 
        CSR_BT_MAP_TAG_ID_LENGTH_STATUS_VALUE, 
        temp);

    CsrPmemFree(pMsg->messageHandle);
    return(ObexUtilPutRequest(pInst->obexClientInst, 
                              headerPriorityTable, 0,
                              (CsrUint8*)("x-bt/messageStatus"), 
                              &messageHandle, NULL, 
                              appHeaderLength, &appHeader, 0, 
                              NULL,
                              csrBtMapcSetMessageStatusContinueHandler, 
                              csrBtMapcSetMessageStatusResultHandler));
}

CsrUint8 CsrBtMapcPushMessageReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[6]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER, CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtMapcPushMessageReq *pMsg      = (CsrBtMapcPushMessageReq *) msg;
    CsrUint8 *appHeader = NULL;
    CsrUint16 appHeaderLength = 0;
    CsrUint8 temp[1];
    CsrUcs2String *folderName = CsrUtf82Ucs2ByteString((CsrUint8*)pMsg->folderName);

    CSR_BT_OBEX_SET_BYTE_AT(temp,0,pMsg->transparent);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(&appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_TRANSPARENT, CSR_BT_MAP_TAG_ID_LENGTH_TRANSPARENT, temp);

    CSR_BT_OBEX_SET_BYTE_AT(temp,0,pMsg->retry);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(&appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_RETRY, CSR_BT_MAP_TAG_ID_LENGTH_RETRY, temp);

    CSR_BT_OBEX_SET_BYTE_AT(temp,0,pMsg->charset);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(&appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_CHARSET, CSR_BT_MAP_TAG_ID_LENGTH_CHARSET, temp);

    if(pMsg->lengthOfObject > 0)
    {
        headerPriorityTable[3] = CSR_BT_OBEX_UTIL_LENGTH_HEADER;
    }
    
    CsrPmemFree(pMsg->folderName);
    return(ObexUtilPutRequest(pInst->obexClientInst, headerPriorityTable, pMsg->lengthOfObject, (CsrUint8*)("x-bt/message"), &folderName, NULL, appHeaderLength, &appHeader, 0, NULL, csrBtMapcPushMessageContinueHandler, csrBtMapcPushMessageResultHandler));
}

CsrUint8 CsrBtMapcPushMessageResHandler(MapcInstanceData *pInst, void *msg)
{
    CsrBtMapcPushMessageRes *pMsg      = (CsrBtMapcPushMessageRes *) msg;

    return(ObexUtilPutContinueRequest(pInst->obexClientInst, pMsg->finalFlag, &pMsg->payload, pMsg->payloadLength));
}

CsrUint8 CsrBtMapcUpdateInboxReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[6]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CSR_UNUSED(msg);

    return(ObexUtilPutRequest(pInst->obexClientInst, headerPriorityTable, 0, (CsrUint8*)("x-bt/MAP-messageUpdate"), NULL, NULL, 0, NULL, 0, NULL, csrBtMapcUpdateInboxContinueHandler, csrBtMapcUpdateInboxResultHandler));
}

CsrUint8 CsrBtMapcAbortReqHandler(MapcInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);

    return(ObexUtilAbortRequest(pInst->obexClientInst, NULL, csrBtMapcAbortResultHandler));
}


/* OBEX Server part of MAPC */
#define RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX    24
static const CsrUint8 mapcObexMapcMNSServiceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,        /* AttrID , ServiceClassIDList */
    0x35,0x03,             /* 3 bytes in total DataElSeq */
    0x19,0x11,0x33,        /* 2 byte UUID, Service class = Message Notification Server */

    /* protocol descriptor list */
    0x09,0x00,0x04,        /* AttrId ProtocolDescriptorList */
    0x35,0x11,             /* 17 bytes in total DataElSeq */
    0x35,0x03,             /* 3 bytes in DataElSeq */
    0x19,0x01,0x00,        /* 2 byte UUID, Protocol = L2CAP */

    0x35,0x05,             /* 5 bytes in DataElSeq */
    0x19,0x00,0x03,        /* 2 byte UUID Protocol = RFCOMM */
    0x08,0x00,             /* 1 byte UINT - server channel template value 0 - to be filled in by app (index:27) */

    0x35,0x03,             /* 3 bytes in DataElSeq */
    0x19, 0x00, 0x08,      /* 2 byte UUID, Protocol = OBEX */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,      /* AttrId = BrowseGroupList */
    0x35, 0x03,            /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,      /* 2 byte UUID, PublicBrowseGroup = 0x1002 */

    /* profile descriptor list */
    0x09,0x00,0x09,        /* AttrId, ProfileDescriptorList */
    0x35,0x08,
    0x35,0x06,             /* 6 bytes in total DataElSeq */
    0x19,0x11,0x34,        /* 2 byte UUID, Service class = Message Access Profile */
    0x09,0x01,0x00,        /* 2 byte uint, version = 100 */

    /* service name */
    0x09,0x01,0x00,        /* AttrId - Service Name */
    0x25,0x0C,             /* 12 bytes string */
    'M','A','P',' ','M','N','S','-','n','a','m','e'
};


static CsrUint8 csrBtMapcObexServerResponseSend(MapcInstanceData *pInst, CsrUint8 responseCode)
{

    return (ObexUtilServPutResponse(pInst->obexServerInst,
                                    (CsrBtObexResponseCode) responseCode,
                                    CSR_BT_OBEX_SRMP_INVALID)); 
}

static void csrBtMapcObexServerSendNotificationOffInd(MapcInstanceData *pInst)
{
    if(pInst->isMasConnected && pInst->notificationRegistrationOn)
    { /* We only send the CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND if MAS is still connected and if the notification registration is not void */
        CsrBtMapcNotificationRegistrationOffInd *pMsg;
        
        pMsg = pnew(CsrBtMapcNotificationRegistrationOffInd);
        pMsg->type = CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND;
        pMsg->instanceId = pInst->mapcInstanceId;
    
        CsrBtMapcMessagePut(pInst->appHandle, pMsg);
    }

    /* MNS service registration always becomes void when disconnected 
       NB: This also ensures that we only send the CSR_BT_MAPC_NOTIFICATION_REGISTRATION_OFF_IND once 
       which will either be on the OBEX Disconnect _or_ if the MSE kills the RFCOMM channel directly */
    pInst->notificationRegistrationOn = FALSE;
    csrBtMapcObexServerMakeNotConnectable(pInst, NULL);
}

static CsrBool csrBtMapcCheckTargetHeader(CsrUint8   *target, 
                                          CsrUint16  targetHeaderLength)
{
    if (target && (targetHeaderLength == MAPC_TARGET_LENGTH))
    {
        CsrUint8 tempTarget[MAPC_TARGET_LENGTH] = MAPC_MNS_TARGET;
    
        if (!CsrMemCmp(target, tempTarget, MAPC_TARGET_LENGTH))
        {
            return (TRUE);
        }
    }
    return (FALSE);       
}

static CsrUint8 csrBtMapcServerConnectResponseSend(MapcInstanceData             *pInst,
                                                   CsrBtObexResponseCode        responseCode, 
                                                   CsrUcs2String                *description)
{
    CsrUint8  numOfHeaders           = 0;  
    CsrUint8  headerPriorityTable[1] = {CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER};
    CsrUint16 whoHeaderLength        = MAPC_TARGET_LENGTH;
    CsrUint8  *who                   = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
    CsrUint8  tempTarget[MAPC_TARGET_LENGTH] = MAPC_MNS_TARGET;
    responseCode                     = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);
    CsrMemCpy(who, tempTarget, whoHeaderLength);

    if (description)
    {
        numOfHeaders                 = 1;
    }
    
    return (ObexUtilServConnectResponse(pInst->obexServerInst, 
                                        responseCode, 
                                        whoHeaderLength, 
                                        &who, 
                                        (CsrUint32) pInst->serverChannel, 
                                        numOfHeaders, 
                                        headerPriorityTable, 
                                        NULL, 
                                        &description, 
                                        0, 
                                        NULL,
                                        FALSE));
}

static void csrBtMapcServerConnectIndHandler(void                    *instData,
                                             CsrBtDeviceAddr         deviceAddr,
                                             CsrBtConnId             cid,
                                             CsrUint16               maxPeerObexPacketLength,
                                             CsrUint16               obexResponsePacketLength,
                                             CsrUint32               length,
                                             CsrUint32               count,
                                             CsrUint16               targetHeaderLength,
                                             CsrUint8                *target,
                                             CsrBool                 authenticated,
                                             CsrBool                 challenged,
                                             CsrUint16               obexPacketLength,
                                             CsrUint8                *obexPacket)
{
    MapcInstanceData *pInst = (MapcInstanceData *)instData;
    
    if (csrBtMapcCheckTargetHeader(target, targetHeaderLength))
    {   
        csrBtMapcServerConnectResponseSend(pInst, 
                                            CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, 
                                            NULL);
        pInst->isServerObexConnected = TRUE;
#ifdef CSR_AMP_ENABLE
        /* Tell the AMPM that the slave channel is connected */
        pInst->slaveBtConnId = cid;
        CsrBtAmpmPrfMultiConnReqSend(pInst->masterBtConnId, 1, &pInst->slaveBtConnId);
#endif /* CSR_AMP_ENABLE */
    }
    else
    {   
        CsrUcs2String *description = CsrUtf82Ucs2ByteString((CsrUint8 *) "Target missing");
        csrBtMapcServerConnectResponseSend(pInst, CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE,
                                           description);
    }
    CSR_UNUSED(length);
    CSR_UNUSED(count);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(challenged);
    CsrPmemFree(obexPacket);
    CsrPmemFree(target);
}

static void csrBtMapcServerPutNextIndHandler(void          *instData,
                                             CsrUint16     obexResponsePacketLength,
                                             CsrBool       authenticated,
                                             CsrBool       finalBitSet,
                                             CsrUint16     bodyLength,
                                             CsrUint16     bodyOffset,
                                             CsrUint16     obexPacketLength,
                                             CsrUint8    *obexPacket)
{ /* The next Put Request packet is received */
    MapcInstanceData   *pInst = (MapcInstanceData *)instData;
    
    pInst->putFinalFlag = finalBitSet;

    csrBtMapcEventNotificationResultHandler(pInst,
                                            0,
                                            0,
                                            bodyOffset,
                                            bodyLength,
                                            obexPacket,
                                            obexPacketLength);    
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexResponsePacketLength);
}

static void csrBtMapcServerPutIndHandler(void          *instData,
                                          CsrUint16     obexResponsePacketLength,
                                          CsrBool       authenticated,
                                          CsrBool       finalBitSet,
                                          CsrUint16     bodyLength,
                                          CsrUint16     bodyOffset,
                                          CsrUint16     obexPacketLength,
                                          CsrUint8    *obexPacket)
{ /* The first Put Request packet is received */
    MapcInstanceData   *pInst = (MapcInstanceData *)instData;
    CsrUint16           nameOffset;
    CsrUint32           totalObjectSize;

    CsrBtObexGetNameOffset(CSR_BT_OBEX_REQUEST, obexPacket, &nameOffset);
    CsrBtObexObjectLengthHeaderIndex(CSR_BT_OBEX_REQUEST, obexPacket, &totalObjectSize);

    pInst->putFinalFlag = finalBitSet;

    csrBtMapcEventNotificationResultHandler(pInst,
                                  totalObjectSize,
                                  nameOffset,
                                  bodyOffset,
                                  bodyLength,
                                  obexPacket,
                                  obexPacketLength);
    
    if (!finalBitSet && nameOffset && totalObjectSize)
    { /* More OBEX PUT request packets to come and all headers found  */
        ObexUtilServSetPutContinueFunc(pInst->obexServerInst, csrBtMapcServerPutNextIndHandler);
    }
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexResponsePacketLength);
}

static void csrBtMapcServerAbortIndHandler(void                    *instData,
                                           CsrUint16               descriptionOffset,
                                           CsrUint16               descriptionLength,
                                           CsrUint16               obexPacketLength,
                                           CsrUint8              *obexPacket)
{
    MapcInstanceData *pInst = (MapcInstanceData *)instData;
    CsrBtMapcEventNotificationAbortInd *pMsg;    
    
    if (descriptionOffset == 0)
    { /* An OBEX Descriptor header is not included, CsrPmemFree obexPacket         */
        CsrPmemFree(obexPacket);
        obexPacket          = NULL;
        obexPacketLength    = 0;     
    }
    pMsg = pnew(CsrBtMapcEventNotificationAbortInd);
    pMsg->type = CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND;
    pMsg->descriptionOffset   = descriptionOffset;
    pMsg->descriptionLength   = descriptionLength;
    pMsg->payload             = obexPacket;
    pMsg->payloadLength       = obexPacketLength;
    CsrBtMapcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtMapcServerDisconnectIndHandler(void                *instData,
                                                CsrBtDeviceAddr     deviceAddr,
                                                CsrBtReasonCode     reasonCode,
                                                CsrBtSupplier       reasonSupplier,
                                                CsrUint16           obexPacketLength,
                                                CsrUint8            *obexPacket)
{
    MapcInstanceData *pInst = (MapcInstanceData *)instData;
    
    pInst->isServerObexConnected = FALSE;
    csrBtMapcObexServerSendNotificationOffInd(pInst);

#ifdef CSR_AMP_ENABLE
    /* Tell the AMPM that the slave channel is disconnected */
    pInst->slaveBtConnId = 0;
    CsrBtAmpmPrfMultiConnReqSend(pInst->masterBtConnId, 0, NULL);
#endif /* CSR_AMP_ENABLE */    
    CSR_UNUSED(instData);
    CSR_UNUSED(deviceAddr);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(reasonCode);
    CSR_UNUSED(reasonSupplier);
    CsrPmemFree(obexPacket);
}

static void csrBtObexMapcNotificationPutReqSend(MapcInstanceData *pInst)
{
    CsrUint8 headerPriorityTable[6]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER, CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrUint8 *appHeader = NULL;
    CsrUint16 appHeaderLength = 0;
    CsrUint8 temp[1];
    
    CSR_BT_OBEX_SET_BYTE_AT(temp, 0, pInst->notificationRegistrationOn);
    appHeaderLength = CsrBtObexUtilAddApplicationParametersHeaderField(&appHeader, appHeaderLength,
        CSR_BT_MAP_TAG_ID_NOTIFICATION_STATUS, CSR_BT_MAP_TAG_ID_LENGTH_NOTIFICATION_STATUS, temp);
    
    ObexUtilPutRequest(pInst->obexClientInst, 
                              headerPriorityTable, 
                              0, 
                              (CsrUint8*)("x-bt/MAP-NotificationRegistration"), 
                              NULL, 
                              NULL, 
                              appHeaderLength, 
                              &appHeader, 
                              0, 
                              NULL, 
                              csrBtMapcNotificationRegistrationContinueHandler, 
                              csrBtMapcNotificationRegistrationResultHandler);

}

static void csrBtMapcServerActivateCfmHandler(void            *instData,
                                              CsrUint8   serverChannel,
                                              psm_t           psm,
                                              CsrBtResultCode resultCode,
                                              CsrBtSupplier   resultSupplier)
{
    MapcInstanceData *pInst = (MapcInstanceData *) instData;
    
    if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS && 
        resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* Server is activated */
        pInst->serverChannel    = serverChannel;
        ObexUtilServConnectableStart(pInst->obexServerInst, 
                                     CSR_BT_MAPC_PROFILE_DEFAULT_MTU_SIZE,
                                     pInst->maxFrameSize);

        csrBtObexMapcNotificationPutReqSend(pInst);
    }
    else
    { /* Try to activate again */
        csrBtObexMapcServerStart(pInst);
    }
    CSR_UNUSED(psm);
}

static void csrBtObexMapcServerStart(MapcInstanceData *pInst)
{
    CsrUint8 *record, recordLength = (CsrUint8) sizeof(mapcObexMapcMNSServiceRecord);
    
    /* Prepare to register Service Record */
    record = CsrPmemAlloc(recordLength);
    CsrMemCpy(record, mapcObexMapcMNSServiceRecord, recordLength);

    /* Activate server */
    if (!pInst->activated)
    {
        pInst->obexServerInst = ObexUtilInit(pInst->mapcInstanceId, pInst, CSR_BT_MAPC_SERVER_INST_ID);
    }
    pInst->activated = TRUE;
    ObexUtilServActivateStart(pInst->obexServerInst,
                              pInst->secIncoming,
                              CSR_BT_OBEX_MESSAGE_NOTIFICATION_SERVER_UUID,
                              CSR_BT_MAPC_LP_SUPERVISION_TIMEOUT,
                              pInst->serverChannel,
                              L2CA_PSM_INVALID,
                              recordLength,
                              RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX,
                              CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                              CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                              &record,
                              CSR_BT_OBJECT_TRANSFER_COD,
                              csrBtMapcServerConnectIndHandler,
                              csrBtMapcServerPutIndHandler,
                              NULL,
                              NULL,
                              csrBtMapcServerAbortIndHandler,
                              csrBtMapcServerDisconnectIndHandler,
                              NULL,
                              csrBtMapcServerActivateCfmHandler,
                              NULL,
                              pInst->maxFrameSize,
                              pInst->windowSize,
                              pInst->localSrmEnable);
}


CsrUint8 CsrBtMapcNotificationRegistrationReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrBtMapcNotificationRegistrationReq *pMsg      = (CsrBtMapcNotificationRegistrationReq *) msg;

    if(pInst->isMasConnected)
    {
        if(pMsg->mnsController)
        {
            pInst->notificationRegistrationOn = pMsg->enableNotifications;

            if(pMsg->enableNotifications && !pInst->activated)
            {
                csrBtObexMapcServerStart(pInst);
            }
            else
            {
                csrBtObexMapcNotificationPutReqSend(pInst);
            }
        }
        else
        { /* Even though notifications might be turned on the MNS will be handled by another task so we treath it as not registred in this task */
            pInst->notificationRegistrationOn = FALSE;
            csrBtObexMapcNotificationPutReqSend(pInst);
        }

        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    {
        return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
    }
}


CsrUint8 CsrBtMapcEventNotificationResHandler(MapcInstanceData *pInst, void *msg)
{
    CsrBtMapcEventNotificationRes *pMsg = msg;
    CsrUint8 result;

    if (pInst->putFinalFlag)
    {
        if (pMsg->response != CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && pMsg->response != CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
        {
            result = pMsg->response;
        }
        else
        {
            result = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
        }
    }
    else
    {
        if (pMsg->response != CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && pMsg->response != CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
        {
            result = pMsg->response;
        }
        else
        {
            result = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
        }
    }

    switch (result & ~CSR_BT_OBEX_FINAL_BIT_MASK)
    {
        case CSR_BT_OBEX_SUCCESS_RESPONSE_CODE:
            if (pInst->putFinalFlag)
            {
                csrBtMapcObexServerResponseSend(pInst,CSR_BT_OBEX_SUCCESS_RESPONSE_CODE | CSR_BT_OBEX_FINAL_BIT_MASK);
            }
            else
            {
                csrBtMapcObexServerResponseSend(pInst,CSR_BT_OBEX_CONTINUE_RESPONSE_CODE | CSR_BT_OBEX_FINAL_BIT_MASK);
            }
            break;
        case CSR_BT_OBEX_CONTINUE_RESPONSE_CODE:
            csrBtMapcObexServerResponseSend(pInst,CSR_BT_OBEX_CONTINUE_RESPONSE_CODE | CSR_BT_OBEX_FINAL_BIT_MASK);
            break;
        default:
            csrBtMapcObexServerResponseSend(pInst, (CsrUint8)(result | (CsrUint8)CSR_BT_OBEX_FINAL_BIT_MASK));
            break;
    }

    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtMapcSecurityInReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrBtMapcSecurityInReq *pMsg    = (CsrBtMapcSecurityInReq*)msg;

    csrBtMapcSecurityInCfmSend(pMsg->appHandle, pInst->mapcInstanceId,
                            CsrBtScSetSecInLevel(&pInst->secIncoming, pMsg->secLevel,
                                            CSR_BT_OBEX_MESSAGE_ACCESS_MANDATORY_SECURITY_INCOMING,
                                            CSR_BT_OBEX_MESSAGE_ACCESS_DEFAULT_SECURITY_INCOMING,
                                            CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                            CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER),
                            CSR_BT_SUPPLIER_OBEX_PROFILES);
    ObexUtilServChangeIncomingSecurity(pInst->obexServerInst, pInst->secIncoming);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtMapcSecurityOutReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrBtMapcSecurityOutReq *pMsg = (CsrBtMapcSecurityOutReq*)msg;

    csrBtMapcSecurityOutCfmSend(pMsg->appHandle, pInst->mapcInstanceId,
                            CsrBtScSetSecOutLevel(&pInst->secOutgoing, pMsg->secLevel,
                                             CSR_BT_OBEX_MESSAGE_ACCESS_MANDATORY_SECURITY_OUTGOING,
                                             CSR_BT_OBEX_MESSAGE_ACCESS_DEFAULT_SECURITY_OUTGOING,
                                            CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                            CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER),
                            CSR_BT_SUPPLIER_OBEX_PROFILES);

    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtMapcRegisterQIDReqHandler(MapcInstanceData *pInst, void *msg)
{
    CsrBtMapcRegisterQidReq *prim;
    CsrBtMapcInstancePool *ptr;
    CsrBtMapcInstancePool *prev;

    prim = (CsrBtMapcRegisterQidReq *) msg;

    if (pInst->mapcInstanceId == CSR_BT_MAPC_IFACEQUEUE)
    {
        ptr = pInst->mapcInstances;
        prev = NULL;

        while((ptr) && (ptr->numberInPool == MAPC_INSTANCES_POOL_SIZE))
        {
            prev = ptr;
            ptr = ptr->next;
        }

        if (ptr)
        {
            /* Do nothing */
        }
        else
        {
            ptr = CsrPmemZalloc(sizeof(CsrBtMapcInstancePool));
            ptr->numberInPool = 0;
            ptr->next=NULL;

            if (prev)
            {
                prev->next = ptr;
            }
            else
            {
                pInst->mapcInstances = ptr;
            }
        }

        ptr->phandles[ptr->numberInPool++] = prim->mapcInstanceId;
        pInst->numberOfMapcInstances++;
    }
    else
    {
        CSR_BT_GENERAL_EXCEPTION("MAPC",0,CSR_BT_MAPC_REGISTER_QID_REQ,"Task not MAPC-manager");
    }

    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

static void CsrBtObexMapcDeactivateCfmHandler(void          *instData, 
                                              CsrUint8 serverChannel,
                                              psm_t         psm)
{
    MapcInstanceData *pInst = (MapcInstanceData *) instData;
    pInst->activated = FALSE;
    pInst->serverChannel = serverChannel;
    ObexUtilDeinit(&pInst->obexServerInst);
    CSR_UNUSED(psm);
}


static void csrBtMapcObexServerMakeNotConnectable(MapcInstanceData *pInst, void *msg)
{

    CSR_UNUSED(msg);
    
    if (pInst->activated)
    {
        ObexUtilServDeactivateStart(pInst->obexServerInst,
                                    CSR_BT_NO_SERVER,
                                    L2CA_PSM_INVALID,
                                    CsrBtObexMapcDeactivateCfmHandler);
    }
    else
    {
        CsrBtObexMapcDeactivateCfmHandler(pInst, pInst->serverChannel, L2CA_PSM_INVALID);
    }

}


/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR




REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_HDP_MODULE

#include "csr_bt_hdp_util.h"
#include "csr_pmem.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_cmn_sdp_lib.h"
#include "csr_bt_hdp_prim_out.h"



/* Template Hdp Service Record  for
    protocol descriptor,
    BrowseGroupList,
    bluetooth profile descriptor,
    additional protocol descriptor */

/* Omron blood presure device
36 00 6d 
09 00 04 
35 10 
35 06 
19 01 00 
09 10 01 
35 06 
19 00 1e 
09 01 00 

09 00 09 
35 08 
35 06 
19 14 00 
09 01 00 

09 00 0d 
35 0f 
35 0d 
35 06 
19 01 00 
09 10 03 
35 03 
19 00 1f 

09 02 00 Supported features
35 09 
35 07 
    08 01 
09 10 07 
    08 00 

09 03 01 MCAP data exchange specification
    08 01 
09 03 02 MCAP operation types supported
    08 06

09 00 06 language
35 09 
09 65 6e 
09 00 6a 
09 01 00 

09 01 00 Service name
25 0c 48 45 4d 2d 37 30 38 31 2d 49 54 00
*/

static CsrUint8 sdsHdpServiceRecord_PD_BP_APD[] =
{
   /* protocol descriptor list */
   0x09, 0x00, 0x04,       /* Attribute 0x0004 = Protocol Descriptor List */
   0x35, 0x10,             /* sequence, length 16 */
   0x35, 0x06,          /* sequence, length 6 (First Protocol) */
   0x19, 0x01, 0x00, /* UUID, 0x0100 (Protocol ID: L2CAP) */
   0x09, 0x10, 0x01, /* (Protocol Parameter 0 [PSM] = 0x1001) - Fill this later */
   0x35, 0x06,          /* sequence, length 6 (Second Protocol) */
   0x19, 0x00, 0x1E, /* UUID, 0x001E (Protocol ID: MCAP Control Channel) */
   0x09, 0x01, 0x00, /* (Protocol Parameter 0 [version] = 0x0100) */

   /* BrowseGroupList    */
    0x09, 0x00, 0x05,    /* AttrId = BrowseGroupList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,    /* 2 byte UUID, PublicBrowseGroup = 0x1002 */

   /* bluetooth profile descriptor list */
   0x09, 0x00, 0x09,       /* (Attribute 0x0009 = Profile Descriptor List) */
   0x35, 0x08,          /* sequence, length 8 */
   0x35, 0x06,       /* sequence, length 6 (First Profile) */
   0x19, 0x14, 0x00, /* UUID, 0x1400 (Profile ID: HDP) */
   0x09, 0x01, 0x00, /* integer (Profile Parameter 0 [version] = 0x0100 */

   /* additional protocol descriptor list */
   0x09, 0x00, 0x0D,       /* Attribute 0x000D = Additional Protocol Descriptor List */
   0x35, 0x0F,             /* sequence, length 15 */
   0x35, 0x0D,          /* sequence, length 13 (First Additional List) */
   0x35, 0x06,       /* sequence, length 6 (First Protocol) */
   0x19, 0x01, 0x00, /* UUID, 0x0100 (Protocol ID: L2CAP) */
   0x09, 0x10, 0x03, /* integer (Protocol Parameter 0 [PSM] = 0x1003)-  - Fill this later */
   0x35, 0x03,          /*  sequence, length 3 (Second Protocol) */
   0x19, 0x00, 0x1F,    /*  UUID, 0x001F (Protocol ID: MCAP Data Channel) */
};


#define MASK 1

CsrUint16 mpeddataTypeList[CSR_BT_TOTAL_DEVICE_TYPES_SUPPORTED] =
{
   CSR_BT_HDP_DEVICE_TYPE_BP,
   CSR_BT_HDP_DEVICE_TYPE_SCALE,
   CSR_BT_HDP_DEVICE_TYPE_OXIM,
   CSR_BT_HDP_DEVICE_TYPE_GLUCOSE,
   CSR_BT_HDP_DEVICE_TYPE_TEMP
};

static CsrBool csrBtHdpBdAddrCmp(BD_ADDR_T *p_bd_addr_dest,BD_ADDR_T *p_bd_addr_src
    )
{
    if ( ( p_bd_addr_dest->lap == p_bd_addr_src->lap ) &&
         ( p_bd_addr_dest->uap == p_bd_addr_src->uap )&&
         ( p_bd_addr_dest->nap == p_bd_addr_src->nap ) )
         return TRUE;
    else
        return FALSE;
}

CsrUint32 HdpStrLen(const char *str)
{
    CsrUint32 len = 0;

    if ( IS_VALID_PTR(str))
    {
        len = CsrStrLen(str);
    }

    return len;

}

void CsrBtHdpAssignDefaultValues(HdpInstanceData  *pInstance)
{
    CsrMemSet(pInstance, 0, sizeof(HdpInstanceData));

    pInstance->state = HDP_IDLE_STATE;
    pInstance->prev_state = HDP_IDLE_STATE;
    pInstance->aux_state = HDP_IDLE_STATE;
    pInstance->roleMask = CSR_BT_HDP_ROLE_INVALID;

    pInstance->mdepDataTypeMask = 0;
    pInstance->sdpState = HDP_SDP_IDLE_STATE;
    pInstance->state = HDP_IDLE_STATE;
    pInstance->numOfMdep = 0;
    pInstance->currentMdepId = 0;
    pInstance->countNumOfMdep = 0;

    pInstance->psm.control = HDP_PSM_INVALID;
    pInstance->psm.data = HDP_PSM_INVALID;

    pInstance->serviceName = NULL;
    pInstance->serviceDescription = NULL;
    pInstance->providerName = NULL;

    pInstance->hdpInstanceId = 0;
    pInstance->mdlIdList = 1;
    pInstance->getCapab = 0;
    pInstance->isActivated = FALSE;
    pInstance->disconnectAssociate = FALSE;
    pInstance->mdlDeleted = TRUE;
    pInstance->echoTestInit = FALSE;
    pInstance->connectAssociate = FALSE;
    pInstance->dataExchangeSpecification = 0x01;

    pInstance->hServiceRecord = HDP_SDP_INVALID_SR_HANDLE;
}

void CsrBtHdpCopyRegisterReqAttributes(HdpInstanceData *pInstance,CsrBtHdpRegisterReq *pMsg)
{
   pInstance->appHandle = pMsg->appHandle;
   pInstance->secLevel = pMsg->secLevel;
   pInstance->serviceName = pMsg->serviceName;
   pInstance->serviceDescription = pMsg->serviceDescription;
   pInstance->providerName = pMsg->providerName;
   pInstance->sniffTimeOut = pMsg->sniffTimeOut;
   pInstance->numOfMdep = pMsg->numOfMdep;
   pInstance->supportedProcedures = pMsg->supportedProcedures;
}


/* hdp service record builder functions */


static CsrUint16 hdpServiceRecordLen(HdpInstanceData  *pInstance,CsrBool *IsSource, CsrBool *IsSink)
{
   CsrUint16 i = 0;

   HdpMdepListEntry *pNode = HDP_MDEP_GET_FIRST(pInstance->mdepList);

   /* protocol descriptor list */
   /* profile descriptor list*/
   /* additional protocol descriptor*/
   i+= sizeof(sdsHdpServiceRecord_PD_BP_APD);
   i+= (3+2+(CsrUint16)HdpStrLen((char *)pInstance->serviceName));
   i+= (3+2+(CsrUint16)HdpStrLen((char *)pInstance->serviceDescription));
   i+= (3+2+(CsrUint16)HdpStrLen((char *)pInstance->providerName));

   /* hdp supported features */
   i+= 5;
   while ( pNode != NULL )
   {
      i += 2+2+3+2+(2+(CsrUint16)HdpStrLen((char *)pNode->mdep.description));

      if ( pNode->mdep.role == CSR_BT_HDP_MDEP_SOURCE )
      {
         *IsSource = TRUE;
      }
      if ( pNode->mdep.role == CSR_BT_HDP_MDEP_SINK )
      {
         *IsSink = TRUE;
      }

      HDP_MDEP_GET_NEXT(pNode);
   }
   /*  data exchange specfication  */
   i+= (3+2);
   /*  MCAP supported features  */
   i+= (3+2);

   /* service class */
   i+= (3+2);
   if (*IsSource == TRUE )
   {
      i += 3;
   }

   if (*IsSink == TRUE )
   {
      i += 3;
   }

   return i;
}

static CsrUint16 addHdpServiceClass(CsrBool IsSource, CsrBool IsSink,CsrUint8 *data)
{
   CsrUint8 attrId[] = { 0x09,0x00, 0x01, 0x35 ,0x00 };
   CsrUint16 i=0;

   i = sizeof(attrId);
   CsrMemCpy(data, attrId, i);

   data[i-1] = ( ( IsSource == TRUE ) && ( IsSink   == TRUE ) ) ? 6 : 3;

   if ( IsSource == TRUE )
   {
      data[i++] = 0x19;
      data[i++] = 0x14;
      data[i++] = 0x01;
   }

   if ( IsSink == TRUE )
   {
      data[i++] = 0x19;
      data[i++] = 0x14;
      data[i++] = 0x02;
   }

   return i;

}

static CsrUint16 addHdpServiceName(HdpInstanceData  *pInstance, CsrUint8 *data)
{
   CsrUint8 attrId[] = {0x09,0x01,0x00,0x25,0x00},len;
   CsrUint16 i = 0 ;

   if ( pInstance->serviceName != NULL )
   {
      len = sizeof ( attrId);
      CsrMemCpy(data, attrId,len);
      i = len;

      len = (CsrUint8)HdpStrLen((char *)pInstance->serviceName);
      data[i-1] = len;
      CsrMemCpy(&data[i], pInstance->serviceName, len );
      i += len;
   }
   return i;
}

static CsrUint16 addHdpServiceDescription(HdpInstanceData  *pInstance, CsrUint8 *data)
{
   CsrUint8 attrId[] = {0x09,0x01,0x01,0x25,0x00},len;
   CsrUint16 i = 0 ;

   if ( pInstance->serviceDescription != NULL )
   {
      len = sizeof ( attrId);
      CsrMemCpy(data, attrId,len);
      i = len;

      len = (CsrUint8) HdpStrLen((char *)pInstance->serviceDescription);
      data[i-1] = len;
      CsrMemCpy(&data[i], pInstance->serviceDescription, len );
      i += len;
   }
   return i;
}

static CsrUint16 addHdpProviderName(HdpInstanceData  *pInstance, CsrUint8 *data)
{
   CsrUint8 attrId[] = {0x09,0x01,0x02,0x25,0x00},len;
   CsrUint16 i = 0 ;

   if ( pInstance->providerName != NULL )
   {
      len = sizeof ( attrId);
      CsrMemCpy(data, attrId,len);
      i = len;

      len = (CsrUint8) HdpStrLen((char *)pInstance->providerName);
      data[i-1] = len;
      CsrMemCpy(&data[i], pInstance->providerName, len );
      i += len;
   }
   return i;
}


static CsrUint16 addHdpMdep(HdpMdepListEntry *pNode, CsrUint8 *data)
   {
    CsrUint16 i = 0;
      CsrUint16 offsetMdepListLen;
    CsrUint8 encodedTagLen = 0;

      /* sequence length */
      data[i++]   = 0x35;
      offsetMdepListLen = i++;


      /* 1 byte MDEP id */
      data[i++] = 0x08;
    encodedTagLen++;

      data[i++] = pNode->mdep.id;
    encodedTagLen++;


      /* 2 byte Specialization */
      data[i++] = 0x09;
    encodedTagLen++;

      data[i++] = pNode->mdep.dataType >> 8; /*high byte */
    encodedTagLen++;

      data[i++] = pNode->mdep.dataType & 0x00FF; /* low byte */
    encodedTagLen++;


      /* 1 byte role */
      data[i++] = 0x08;
    encodedTagLen++;

    data[i++] = pNode->mdep.role;
    encodedTagLen++;


      /*  text string */
    if ( ( pNode->mdep.description[0] != '\0' )  )
      {
         data[i++] = 0x25;
        encodedTagLen++;

        data[i++] = (CsrUint8) CsrStrLen((char *)pNode->mdep.description);
        encodedTagLen++;

        CsrStrNCpy((char *)(&data[i]), (char *)(pNode->mdep.description),data[i-1]);

        encodedTagLen+=data[i-1];
        i = i + data[i-1];

      }
    data[offsetMdepListLen ] = encodedTagLen;

    return i;

}
static CsrUint16  addHdpSupportedFeatures(HdpInstanceData  *pInstance, CsrUint8 *data )
{
    CsrUint16 i = 0;
    CsrUint16 offsetSupportedFeaturesLen;
    CsrUint16 encodedTagLen = 0;
    HdpMdepListEntry *pNode = HDP_MDEP_GET_FIRST(pInstance->mdepList);

    /* hdp supported features */
    data[i++] = 0x09;
    data[i++] = 0x02;
    data[i++] = 0x00;

    /* sequence length */
    data[i++] = 0x35;
    offsetSupportedFeaturesLen = i++; /* @note what if the length is more then 512*/

    /* mdep list */
    while ( ( pNode != NULL ) )
    {
       encodedTagLen = addHdpMdep(pNode, &data[i]);
       i += encodedTagLen;
      HDP_MDEP_GET_NEXT(pNode);
    }
    pNode = HDP_MDEP_GET_FIRST(pInstance->mdepList);
    if ( encodedTagLen <= 0xFF)
    {
        data[offsetSupportedFeaturesLen] = (CsrUint8)i-offsetSupportedFeaturesLen-1;
    }
    else
    {
        i = i-encodedTagLen-1;

        data[i] = 0x36;
        data[i++] = encodedTagLen >> 8; /*high byte */
        data[i++] = encodedTagLen & 0x00FF; /* low byte */

        encodedTagLen = 0;

        while(pNode != NULL)
        {
           encodedTagLen += addHdpMdep(pNode, &data[i]);
           i += encodedTagLen;
           HDP_MDEP_GET_NEXT(pNode);
        }
    }

   return i;

}


CsrUint16 buildHdpServiceRecord(HdpInstanceData  *pInstance, CsrUint8 **record)
{
   CsrUint16 serviceRecordLen,encodedLen = 0;
   CsrBool IsSource = FALSE, IsSink = FALSE;
   CsrUint8 *data;;

   serviceRecordLen = hdpServiceRecordLen(pInstance, &IsSource, &IsSink);

   data = (CsrUint8 *)CsrPmemAlloc(serviceRecordLen);

   *record = data;

   encodedLen += addHdpServiceClass( IsSource, IsSink, data);

   /* Update the tempplate for Control and Data PSM
        the control and data PSM offsets are magic numbers */
   sdsHdpServiceRecord_PD_BP_APD[11] = HDP_GET_HB(pInstance->psm.control);
   sdsHdpServiceRecord_PD_BP_APD[12] = HDP_GET_LB(pInstance->psm.control);

   sdsHdpServiceRecord_PD_BP_APD[55] = HDP_GET_HB(pInstance->psm.data);
   sdsHdpServiceRecord_PD_BP_APD[56] = HDP_GET_LB(pInstance->psm.data);

   CsrMemCpy(&data[encodedLen],sdsHdpServiceRecord_PD_BP_APD ,sizeof(sdsHdpServiceRecord_PD_BP_APD));
   encodedLen += sizeof(sdsHdpServiceRecord_PD_BP_APD);

   encodedLen += addHdpServiceName(pInstance,&data[encodedLen]);
   encodedLen += addHdpServiceDescription(pInstance,&data[encodedLen]);
   encodedLen += addHdpProviderName(pInstance,&data[encodedLen]);

   encodedLen += addHdpSupportedFeatures(pInstance,&data[encodedLen]);

   /*  data exchange specification*/
   data[encodedLen++] = 0x09;
   data[encodedLen++] = 0x03;
   data[encodedLen++] = 0x01;

   data[encodedLen++] = 0x08;
   data[encodedLen++] = pInstance->dataExchangeSpecification;

   /* mcap supported features*/
   data[encodedLen++] = 0x09;
   data[encodedLen++] = 0x03;
   data[encodedLen++] = 0x02;

   data[encodedLen++] = 0x08;
   data[encodedLen++] = pInstance->supportedProcedures;

   return encodedLen;

}

/* hdp mdep link list functions*/

CsrBool CsrBtHdpUtilCmnListIsMdepBusy(CsrUint16 mdepId,CsrCmnList_t mdepList)
{
   HdpMdepListEntry *pNode = HDP_MDEP_GET_FIRST(mdepList);

   while(pNode != NULL)
   {
      if( ( pNode->mdep.id == mdepId )  &&
          ( pNode->busy == TRUE) )
      {
        return TRUE;
      }
      pNode = pNode->next;
   }
   return FALSE;
}

HdpMdepListEntry * CsrBtHdpUtilCmnListMdepMatchingMdepFree(CsrCmnList_t mdepList, CsrBtMdepId mdepId)
{
   HdpMdepListEntry *pNode = HDP_MDEP_GET_FIRST(mdepList);

   while(pNode != NULL)
   {
      if( ( pNode->mdep.id == mdepId )  &&
          ( pNode->busy == FALSE) )
      {
        return pNode;
      }
      pNode = pNode->next;
   }
   return NULL;
}

HdpMdepListEntry * CsrBtHdpUtilCmnListMdepMatchingMdepBusy(CsrCmnList_t mdepList, CsrBtMdepId mdepId)
{
   HdpMdepListEntry *pNode = HDP_MDEP_GET_FIRST(mdepList);

   while(pNode != NULL)
   {
      if( ( pNode->mdep.id == mdepId )  &&
          ( pNode->busy == TRUE) )
      {
        return pNode;
      }
      pNode = pNode->next;
   }
   return NULL;
}

HdpMdepListEntry * CsrBtHdpUtilCmnListMdepMatchingMdep(CsrCmnList_t mdepList, CsrBtMdepId mdepId)
{
   HdpMdepListEntry *pNode = HDP_MDEP_GET_FIRST(mdepList);

   while(pNode != NULL)
   {
      if( pNode->mdep.id == mdepId )
      {
        return pNode;
      }
      pNode = pNode->next;
   }
   return NULL;
}

/*--------hdp get capabability util functions ------*/


void CsrBtHdpSdpInitServiceRecordStruct(CsrBtHdpSdpRecord *rec)
{

    rec->role                      = CSR_BT_HDP_ROLE_INVALID;           /* Set to AVRCP_ROLE_TARGET or AVRCP_ROLE_CONTROLLER */
    rec->dataExchangeSpecification = CSR_BT_HDP_DATA_EXC_PROTO_INVALID; /* Specifies data exchange protocol */
    rec->mcapFeatures              = CSR_BT_HDP_SF_NONE;                /* MCAP Features*/
    rec->supportedFeatureList      = NULL;                              /* Supported features sequence */
    rec->supportedFeatureListLen   = 0;
    rec->providerName              = NULL;                              /* NULL-terminated string */
    rec->serviceName               = NULL;                              /* NULL-terminated string */
    rec->serviceDescription        = NULL;                              /* NULL-terminated string */
}

void CsrBtHdpSdcStartSearch(HdpInstanceData * pInstance,CsrBtDeviceAddr addr)
{
    CmnCsrBtLinkedListStruct *sdpTagList = NULL;
    CsrUint16 shIndex;
    if (HDP_SDP_IDLE_STATE == pInstance->sdpState || HDP_SDP_PENDING_STATE == pInstance->sdpState)
    {
        if((pInstance->roleMask & CSR_BT_HDP_SOURCE_ROLE) || (pInstance->roleMask & CSR_BT_HDP_ROLE_ANY))
        {
            sdpTagList = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTagList, CSR_BT_HDP_SOURCE_UUID, &shIndex);
        }
        else if(pInstance->roleMask & CSR_BT_HDP_SINK_ROLE)
        {
            sdpTagList = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTagList, CSR_BT_HDP_SINK_UUID, &shIndex);
        }
        if (NULL != sdpTagList)
        {
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_ADDITIONAL_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SERVICE_PROVIDER_NAME, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_DATA_EXCHANGE_SPECIFICATION, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_MCAP_SUPPORTED_FEATURES, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SUPPORTED_FEATURES, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SERVICE_DESCRIPTION_ATTRIBUTE_IDENTIFIER, NULL, 0);

            if((pInstance->roleMask & CSR_BT_HDP_ROLE_ANY))
            {
               sdpTagList = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTagList, CSR_BT_HDP_SINK_UUID, &shIndex);
               CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, NULL, 0);
               CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_ADDITIONAL_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, NULL, 0);
               CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER, NULL, 0);
               CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SERVICE_PROVIDER_NAME, NULL, 0);
               CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_DATA_EXCHANGE_SPECIFICATION, NULL, 0);
               CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_MCAP_SUPPORTED_FEATURES, NULL, 0);
               CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SUPPORTED_FEATURES, NULL, 0);
               CsrBtUtilSdrCreateAndInsertAttribute(sdpTagList, shIndex, CSR_BT_SERVICE_DESCRIPTION_ATTRIBUTE_IDENTIFIER, NULL, 0);
            }

            pInstance->sdpState = HDP_SDP_ACTIVE_STATE;
            /* Start the SDP search */
            CsrBtUtilSdcSearchStart((void *)pInstance, pInstance->sdpSearchData, sdpTagList, addr);
        }
    }
    else
    {
        CsrBtHdpGetCapabCfmSend(pInstance->appHandle, &addr, CSR_BT_RESULT_CODE_HDP_SEARCH_ACTIVE, CSR_BT_SUPPLIER_HDP);
    }
}

static CsrBool csrBtHdpSdpGetSupportedFeatures(CmnCsrBtLinkedListStruct *bll_p,
                                                CsrUint16 serviceHandleIndex,CsrBtHdpMdepList **supportedFeatures,CsrUint8 mdepDataTypemask,CsrUint8 role_mask,CsrUint8 *length)
{
    CsrBool    retBool = FALSE,found = FALSE;
    CsrUint8  *att_p,mask =0,role =0,j=0;
    CsrUintFast16  nofAttributes, x, y;
    CsrUint16 attDataLen, nofBytesToAttribute, emptyAttSize, consumedBytes, totalConsumedBytes = 0, tempVar,i=0,len=0;
    CsrUint32  mpedId,returnValue;
    CsrBtMdepDataType type;
    CsrBtHdpMdep *rec = NULL;
    CsrBtHdpMdepList *first=NULL,*temp = NULL;


    if (TRUE == CsrBtUtilSdrGetNofAttributes(bll_p, serviceHandleIndex, &nofAttributes))
    {
        for (x=0; x<nofAttributes; x++)
        {
            att_p = CsrBtUtilSdrGetAttributePointer(bll_p, serviceHandleIndex, x, &nofBytesToAttribute);
            /* Check if the UUID in the 'outer' attribute struct is correct */
            CsrMemCpy(&tempVar, att_p + SDR_ENTRY_INDEX_ATTRIBUTE_UUID, SDR_ENTRY_SIZE_SERVICE_UINT16);
            if (CSR_BT_SUPPORTED_FEATURES == tempVar)
            {
                CsrBtUtilSdrGetEmptyAttributeSize(&emptyAttSize);

                CsrMemCpy(&tempVar, att_p, SDR_ENTRY_SIZE_SERVICE_UINT16);
                attDataLen = tempVar - emptyAttSize + SDR_ENTRY_SIZE_TAG_LENGTH;

                /* First extract the attribute uuid from the attribute SDP data */
                if (CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA,
                                              attDataLen,
                                              &returnValue,
                                              &consumedBytes,
                                              FALSE))
                {
                    /* Check if the UUID in the 'inner' attribute sdp data struct is correct */
                    if (CSR_BT_SUPPORTED_FEATURES == returnValue)
                    {
                        attDataLen = attDataLen - consumedBytes;
                        totalConsumedBytes += consumedBytes;
                        while((attDataLen > 0) && (CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes,
                                                          attDataLen,
                                                          &mpedId,
                                                          &consumedBytes,
                                                          TRUE)))
                        {
                           attDataLen = attDataLen - consumedBytes;
                           totalConsumedBytes += consumedBytes;
                           for(y=0;y<2;y++)
                           {
                             if(CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes,
                                                          attDataLen,
                                                          &returnValue,
                                                          &consumedBytes,
                                                          FALSE))
                             {
                                if(y==0) /*&& returnValue == Any of the types*/
                                {
                                  type = (CsrUint16)returnValue;
                                  mask = mdepDataTypemask;
                                  while(i< CSR_BT_TOTAL_DEVICE_TYPES_SUPPORTED)
                                  {
                                    if(mask & MASK)
                                    {
                                        break;
                                    }
                                    mask = mask >> 1;
                                    i++;
                                  }
                                  if( ((i< CSR_BT_TOTAL_DEVICE_TYPES_SUPPORTED) && (type == mpeddataTypeList[i])) || 
                                       (mdepDataTypemask == 0xFF))
                                  {
                                     rec = (CsrBtHdpMdep *)CsrPmemAlloc(sizeof(CsrBtHdpMdep));
                                     rec->id= (CsrUint8)mpedId;
                                     rec->dataType = type;
                                     found = TRUE;
                                     i = 0;
                                  }


                                  attDataLen = attDataLen - consumedBytes;
                                  totalConsumedBytes += consumedBytes;
                                }
                                else if(y==1 && found == TRUE)
                                {
                                  role = (CsrUint8)returnValue;
                                  if((role_mask == CSR_BT_HDP_SOURCE_ROLE && role == 0x00) || (role_mask == CSR_BT_HDP_SINK_ROLE && role == 0x01) || (role_mask == CSR_BT_HDP_ROLE_ANY))
                                  {
                                     if(first == NULL)
                                     {
                                        first = (CsrBtHdpMdepList *)CsrPmemAlloc(sizeof(CsrBtHdpMdepList));
                                        first->next = NULL;
                                        temp = first;
                                     }
                                     else
                                     {
                                       temp = first;
                                       while(temp->next != NULL)
                                       {
                                           temp = temp->next;
                                       }
                                       temp->next = (CsrBtHdpMdepList *)CsrPmemAlloc(sizeof(CsrBtHdpMdepList));
                                       temp = temp->next;
                                       temp->next = NULL;
                                     }
                                     CsrMemSet(temp->node.description,0,CSR_BT_MAX_FRIENDLY_NAME_LEN + 1);
                                     temp->node.id = rec->id;
                                     temp->node.role = role;
                                     temp->node.dataType = rec->dataType;
                                     found = FALSE;
                                  }
                                  else
                                  {
                                     CsrPmemFree(rec);
                                     rec = NULL;
                                     break;
                                  }
                                  CsrPmemFree(rec);
                                  rec = NULL;
                                  attDataLen = attDataLen - consumedBytes;
                                  totalConsumedBytes += consumedBytes;
                                  if(*(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes) == (CSR_BT_DATA_ELEMENT_TEXT_STRING_TYPE + CSR_BT_DATA_ELEMENT_SIZE_NEXT_1_BYTE))
                                  {
                                    len = *(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes + 1);
                                    CsrMemCpy(temp->node.description, att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes + 2, len);
                                    totalConsumedBytes += 2;
                                  }
                                  else if(*(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes) == (CSR_BT_DATA_ELEMENT_TEXT_STRING_TYPE + CSR_BT_DATA_ELEMENT_SIZE_NEXT_2_BYTES))
                                  {
                                    len = CSR_GET_UINT16_FROM_BIG_ENDIAN(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes + 1);
                                    CsrMemCpy(temp->node.description, att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes + 3, len);
                                    totalConsumedBytes += 3;
                                  }
                                  
                                  if(len)
                                  {
                                    temp->node.description[len] = '\0';
                                    attDataLen = attDataLen - len;
                                    totalConsumedBytes += len;
                                  }
                                  else
                                  {
                                    temp->node.description[0] = '\0';
                                  }
                                  j++;
                               }

                             }
                             else if(y==1 && found == TRUE)
                             {/* Remember to free the allocated "rec" value which is not going to be used after all...*/
                                 CsrPmemFree(rec);
                                 rec = NULL;
                             }
                           }
                        }
                    }
                }
            }
        }
    }
   if(j)
   {
      *length = j;
      *(supportedFeatures) = first;
      retBool = TRUE;
   }
   return retBool;
}



static CsrBool csrBtHdpSdpGetProtocolDescriptorList(CmnCsrBtLinkedListStruct *bll_p, CsrUint16 serviceHandleIndex, CsrUint16  *psm)
{
    CsrBool    retBool = FALSE;
    CsrUint8  *att_p, foundCounter = 0;
    CsrUintFast16 nofAttributes, x, y;
    CsrUint16 attDataLen, nofBytesToAttribute, emptyAttSize, consumedBytes, totalConsumedBytes = 0, tempVar;
    CsrUint32  returnValue, protocolValue;

    if (TRUE == CsrBtUtilSdrGetNofAttributes(bll_p, serviceHandleIndex, &nofAttributes))
    {
        for (x=0; x<nofAttributes; x++)
        {
            att_p = CsrBtUtilSdrGetAttributePointer(bll_p, serviceHandleIndex, x, &nofBytesToAttribute);
            /* Check if the UUID in the 'outer' attribute struct is correct */
            CsrMemCpy(&tempVar, att_p + SDR_ENTRY_INDEX_ATTRIBUTE_UUID, SDR_ENTRY_SIZE_SERVICE_UINT16);
            if (CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER == tempVar)
            {
                CsrBtUtilSdrGetEmptyAttributeSize(&emptyAttSize);

                CsrMemCpy(&tempVar, att_p, SDR_ENTRY_SIZE_SERVICE_UINT16);
                attDataLen = tempVar - emptyAttSize + SDR_ENTRY_SIZE_TAG_LENGTH;

                /* First extract the attribute uuid from the attribute SDP data */
                if (CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA,
                                              attDataLen,
                                              &returnValue,
                                              &consumedBytes,
                                              FALSE))
                {
                    /* Check if the UUID in the 'inner' attribute sdp data struct is correct */
                    if (CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER == returnValue)
                    {
                        attDataLen = attDataLen - consumedBytes;
                        totalConsumedBytes += consumedBytes;
                        /* Then find the UUID type */
                        for (y=0; y<2; y++)
                        {
                            /* first find the protocol UUID */
                            if (CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes,
                                                          attDataLen,
                                                          &protocolValue,
                                                          &consumedBytes,
                                                          TRUE))
                            {
                                attDataLen = attDataLen - consumedBytes;
                                totalConsumedBytes += consumedBytes;
                                /* Now find the value */
                                if (CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes,
                                                              attDataLen,
                                                              &returnValue,
                                                              &consumedBytes,
                                                              TRUE))
                                {
                                    attDataLen = attDataLen - consumedBytes;
                                    totalConsumedBytes += consumedBytes;

                                    if (CSR_BT_L2CAP_PROTOCOL_UUID16_IDENTIFIER == protocolValue)
                                    {
                                        foundCounter++;
                                        *psm = (CsrUint16)returnValue;
                                    }
                                }
                            }
                        }
                        if (foundCounter == 1)
                        {
                            retBool = TRUE;
                        }
                    }
                }
            }
        }
    }
    return retBool;
}

static CsrBool csrBtHdpSdpGetAdditionalProtocolDescriptorList(CmnCsrBtLinkedListStruct *bll_p,
                                                CsrUint16 serviceHandleIndex,
                                                CsrUint16  *dataPsm)
{
    CsrBool    retBool = FALSE;
    CsrUint8  *att_p, foundCounter = 0;
    CsrUintFast16 nofAttributes, x, y;
    CsrUint16 attDataLen, nofBytesToAttribute, emptyAttSize, consumedBytes, totalConsumedBytes = 0, tempVar;
    CsrUint32  returnValue, protocolValue;

    if (TRUE == CsrBtUtilSdrGetNofAttributes(bll_p, serviceHandleIndex, &nofAttributes))
    {
        for (x=0; x<nofAttributes; x++)
        {
            att_p = CsrBtUtilSdrGetAttributePointer(bll_p, serviceHandleIndex, x, &nofBytesToAttribute);
            /* Check if the UUID in the 'outer' attribute struct is correct */
            CsrMemCpy(&tempVar, att_p + SDR_ENTRY_INDEX_ATTRIBUTE_UUID, SDR_ENTRY_SIZE_SERVICE_UINT16);
            if (CSR_BT_ADDITIONAL_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER == tempVar)
            {
                CsrBtUtilSdrGetEmptyAttributeSize(&emptyAttSize);

                CsrMemCpy(&tempVar, att_p, SDR_ENTRY_SIZE_SERVICE_UINT16);
                attDataLen = tempVar - emptyAttSize + SDR_ENTRY_SIZE_TAG_LENGTH;

                /* First extract the attribute uuid from the attribute SDP data */
                if (CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA,
                                              attDataLen,
                                              &returnValue,
                                              &consumedBytes,
                                              FALSE))
                {
                    /* Check if the UUID in the 'inner' attribute sdp data struct is correct */
                    if (CSR_BT_ADDITIONAL_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER == returnValue)
                    {
                        attDataLen = attDataLen - consumedBytes;
                        totalConsumedBytes += consumedBytes;

                           for (y=0; y<2; y++)
                           {
                               /* first find the protocol UUID */
                               if (CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes,
                                                             attDataLen,
                                                             &protocolValue,
                                                             &consumedBytes,
                                                             TRUE))
                               {
                                   attDataLen = attDataLen - consumedBytes;
                                   totalConsumedBytes += consumedBytes;
                                   /* Now find the value */
                                   if (CsrBtUtilSdpExtractUint(att_p + SDR_ENTRY_INDEX_ATTRIBUTE_DATA + totalConsumedBytes,
                                                                 attDataLen,
                                                                 &returnValue,
                                                                 &consumedBytes,
                                                                 TRUE))
                                   {
                                       attDataLen = attDataLen - consumedBytes;
                                       totalConsumedBytes += consumedBytes;

                                       if (CSR_BT_L2CAP_PROTOCOL_UUID16_IDENTIFIER == protocolValue)
                                       {
                                           foundCounter++;
                                           *dataPsm = (CsrUint16)returnValue;
                                       }
                                   }
                               }
                           }

                        if (foundCounter == 1)
                        {
                            retBool = TRUE;
                        }
                    }
                }
            }
        }
    }
    return retBool;
}



CsrBool CsrBtHdpSdpExtractServiceRecordData(CmnCsrBtLinkedListStruct *bll_p,
                                                CsrUint16  serviceHandleIndex,
                                                CsrBtHdpSdpRecord *sdpRecord,
                                                CsrUint32 *handle,
                                                CsrUint8 dataTypeMask,
                                                CsrUint8 roleMask)
{
   CsrUint32 returnValue;
   CsrUint16 stringLen,ctrlPsm = 0,dataPsm = 0;
   CsrUint8 *string,i =0;
   CsrBool retBool = FALSE;
   CsrBtHdpMdepList *suppFeatures = NULL,*temp1 = NULL,*temp2 = NULL;
   CsrBtHdpMdep *temp = NULL;

   if(csrBtHdpSdpGetSupportedFeatures(bll_p,serviceHandleIndex,&suppFeatures,dataTypeMask,roleMask,&(sdpRecord->supportedFeatureListLen)))
   {
       sdpRecord->supportedFeatureList = (CsrBtHdpMdep *)CsrPmemAlloc(sizeof(CsrBtHdpMdep) * sdpRecord->supportedFeatureListLen);
       temp1 = suppFeatures;
       for(i =0;i<sdpRecord->supportedFeatureListLen;i++)
       {
          temp = (sdpRecord->supportedFeatureList + i);
          CsrMemSet(temp, 0, sizeof(CsrBtHdpMdep));
          temp->id = suppFeatures->node.id;
          temp->role = suppFeatures->node.role;
          temp->dataType = suppFeatures->node.dataType;
          CsrMemCpy(temp->description, suppFeatures->node.description, CsrUtf8StringLengthInBytes(suppFeatures->node.description));
          suppFeatures = suppFeatures->next;
       }
       while(temp1)
       {
          temp2 = temp1->next;
          CsrPmemFree(temp1);
          temp1 = temp2;
       }
       csrBtHdpSdpGetProtocolDescriptorList(bll_p,serviceHandleIndex,&ctrlPsm);
       *handle = ctrlPsm << 16;
       csrBtHdpSdpGetAdditionalProtocolDescriptorList(bll_p,serviceHandleIndex,&dataPsm);
       *handle = *handle | dataPsm;
       if (TRUE == CsrBtUtilSdrGetStringAttributeFromAttributeUuid(bll_p, serviceHandleIndex, CSR_BT_SERVICE_PROVIDER_NAME, &string, &stringLen))
       {
            sdpRecord->providerName    = CsrPmemAlloc(stringLen+1);
            CsrMemCpy(sdpRecord->providerName, string, stringLen);
            sdpRecord->providerName[stringLen] = 0x00;
       }
       if (TRUE == CsrBtUtilSdrGetStringAttributeFromAttributeUuid(bll_p, serviceHandleIndex, CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER, &string, &stringLen))
       {
            sdpRecord->serviceName    = CsrPmemAlloc(stringLen+1);
            CsrMemCpy(sdpRecord->serviceName, string, stringLen);
            sdpRecord->serviceName[stringLen] = 0x00;
       }
       if (TRUE == CsrBtUtilSdrGetStringAttributeFromAttributeUuid(bll_p, serviceHandleIndex, CSR_BT_SERVICE_DESCRIPTION_ATTRIBUTE_IDENTIFIER, &string, &stringLen))
       {
            sdpRecord->serviceDescription = CsrPmemAlloc(stringLen+1);
            CsrMemCpy(sdpRecord->serviceDescription, string, stringLen);
            sdpRecord->serviceDescription[stringLen] = 0x00;
       }
       if (TRUE == CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(bll_p, serviceHandleIndex, CSR_BT_DATA_EXCHANGE_SPECIFICATION, &returnValue))
       {
            sdpRecord->dataExchangeSpecification = (CsrUint8) returnValue;
       }
       if (TRUE == CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(bll_p, serviceHandleIndex, CSR_BT_MCAP_SUPPORTED_FEATURES, &returnValue))
       {
            sdpRecord->mcapFeatures = (CsrUint8) returnValue;
       }
       retBool = TRUE;
   }
   return retBool;
}


/*--------hdp get capabability util functions ------*/

/* hdp mdep link list functions*/

HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMatchingAssocChId(HdpConnectionListEntry *pList, CsrUint32 assocChId)
{
    CsrUint32 chId;

    while (pList != NULL )
    {
        chId = HDP_FORM_ASSOC_CH_ID(pList->mclId,pList->mdlId);

        if ( chId == assocChId)
        {
            return pList;
        }

        pList = pList->next;
    }

    return NULL;
}


void CsrBtHdpUtilMdepIdUnCheckBusy(CsrUint16 mdepId,CsrCmnList_t mdepList)
{
    HdpMdepListEntry *pNode = HDP_MDEP_GET_FIRST(mdepList);
    while(pNode != NULL)
    {
      if(pNode->mdep.id == mdepId)
      {
        pNode->busy = FALSE;
        return;
      }
      pNode = pNode->next;
    }
}


HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryGetActiveMdlInAssocChId(HdpConnectionListEntry *pList, CsrUint32 assocChId)
{
    while (pList != NULL )
    {
        if ( ( pList->assocChId == assocChId ) && ( pList->active == TRUE ))
        {
            return pList;
        }

        pList = pList->next;
    }

    return NULL;
}

HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryGetInActiveMdlInAssocChId(HdpConnectionListEntry *pList, CsrUint32 assocChId)
{
    while (pList != NULL )
    {
        if ( ( pList->assocChId == assocChId ) && ( pList->active == FALSE ))
        {
            return pList;
        }

        pList = pList->next;
    }

    return NULL;
}

HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryGetActiveMdepInMcl(HdpConnectionListEntry *pList, CsrUint16 mclId)
{
    while (pList != NULL )
    {
        if ( ( pList->mclId == mclId ) && ( pList->active == TRUE ))
        {
            return pList;
        }

        pList = pList->next;
    }

    return NULL;
}

HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMclActiveOnAssocChId(HdpConnectionListEntry *pList, CsrUint32 assocChId)
{
    HdpConnectionListEntry *pNode = pList;
    while (pList != NULL )
    {
        if ( ( pList->assocChId == assocChId ) && (pList->active == FALSE) )
        {
            break;
        }

        pList = pList->next;
    }

    if(pList == NULL)
    {
       return NULL;
    }

    while ( pNode != NULL )
    {
        if (csrBtHdpBdAddrCmp(&pList->bd_addr, &pNode->bd_addr) && (pNode->active == TRUE))
        {
            return pList;
        }
        pNode = pNode->next;
    }
    return NULL;
}


HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMatchingMclMdl(HdpConnectionListEntry *pList, CsrUint16 mclId,CsrUint16 mdlId)
{
    while (pList != NULL )
    {
        if (  ( pList->mdlId == mdlId ) && ( pList->mclId == mclId ))
        {
            return pList;
        }

        pList = pList->next;
    }

    return NULL;
}

HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMatchingMcl(HdpConnectionListEntry *pList, CsrUint16 mclId)
{
    while (pList != NULL )
    {
        if (   pList->mclId == mclId  )
        {
            return pList;
        }

        pList = pList->next;
    }

    return NULL;
}

HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMatchingMclMdlBdPsm(HdpConnectionListEntry *pList,
                                                               CsrUint16 mclId,
                                                               CsrUint16 mdlId,
                                                               CsrBtDeviceAddr  *bd_addr,
                                                               CsrUint16 psm)
{
    while (pList != NULL )
    {
        if (  ( pList->mclId == mclId ) &&
              ( pList->mdlId == mdlId ) &&
              ( ( pList->remotecontrolPsm == psm ) || ( pList->remotedataPsm == psm ) ) &&
              csrBtHdpBdAddrCmp(&pList->bd_addr, bd_addr))
        {
            return pList;
        }

        pList = pList->next;
    }

    return NULL;
}

HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryGetMatchingBdMcl(HdpConnectionListEntry *pList,
                                                                  CsrBtDeviceAddr  *bd_addr,
                                                                  CsrUint16 mclId)
{
    while (pList != NULL )
    {
        if ( csrBtHdpBdAddrCmp(&pList->bd_addr, bd_addr) &&
             (pList->mclId == mclId))
        {
            return pList;
        }

        pList = pList->next;
    }

    return NULL;
}

CsrUint16 CsrBtHdpUtilFindNumberofEntries(HdpConnectionListEntry *pList, CsrUint16 mclId)
{
   HdpConnectionListEntry * temp;
   CsrUint16 mdlCount =0;
   while(pList != NULL)
   {
      temp = pList->next;
      if(pList->mclId == mclId)
      {
         mdlCount++;
      }
      pList = temp;
   }
   return mdlCount;
}

CsrUint16 CsrBtHdpUtilFindNumberofMdepEntries(HdpConnectionListEntry *pList, CsrBtMdepId mdepId)
{
   HdpConnectionListEntry * temp;
   CsrUint16 mdlCount =0;
   while(pList != NULL)
   {
      temp = pList->next;
      if(pList->mdepId == mdepId)
      {
         mdlCount++;
      }
      pList = temp;
   }
   return mdlCount;
}



HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMatchingBdAddr(HdpConnectionListEntry *pList, CsrBtDeviceAddr *bd_addr)
{
    while (pList != NULL )
    {
        if ( csrBtHdpBdAddrCmp(&pList->bd_addr, bd_addr) )
        {
            return pList;
        }

        pList = pList->next;
    }

    return NULL;
}




void CsrBtHdpUtilConnectionEntryUpdateStatusMcl(HdpConnectionListEntry *pList, CsrUint16 mclId,CsrBool status)
{
   while ( pList != NULL )
   {
       if(pList->mclId == mclId)
       {
          pList->active = status;
       }
       pList = pList->next;
   }
}

CsrBool CsrBtHdpUtilConnectionEntryDeleteMdl(HdpConnectionListEntry *pList, CsrUint32 assocId)
{
   HdpConnectionListEntry * temp;

   while(pList != NULL)
   {
      temp = pList->next;
      if(pList->assocChId == assocId)
      {
         CsrBtMcapDeleteMdlReqSend(pList->mclId, pList->mdlId);
         return TRUE;
      }
      pList = temp;
   }
   return FALSE;
}


CsrBool CsrBtHdpUtilConnectionEntryFindOtherConnection(HdpConnectionListEntry *pList,CsrUint32 assocChId)
{
   while(pList != NULL)
   {
      if (pList->assocChId != assocChId)
      {
         return FALSE;
      }
      pList = pList->next;
   }
   return TRUE;
}


CsrBool CsrBtHdpUtilFindActiveMdl(HdpConnectionListEntry *pList)
{
   while(pList != NULL)
   {
      if(pList->active == TRUE)
      {
         return TRUE;
      }
      pList = pList->next;
   }
   return FALSE;
}


HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMatchingMclMdep(HdpConnectionListEntry *pList, CsrUint16 mclId,CsrBtMdepId mdep)
{
    while ( pList != NULL && ( pList->mclId != mclId ) &&  ( pList->mdepId != mdep ))
    {
        pList = pList->next;
    }

    return pList;
}

void CsrBtHdpUtilUpdateConnectionListForReconnect(HdpInstanceData *instData,CsrBtHdpSdpRecord *record,CsrUint32 *handle,CsrBtDeviceAddr * bd_addr)
{
    HdpConnectionListEntry *node = HDP_CONNECTION_GET_FIRST(instData->connectionList),*temp;
    CsrBtHdpMdep *mdepNode;
    CsrUint32 i,suppFeatureLen = record->supportedFeatureListLen,assocId;
    CsrUint16 localCtrlPsm = instData->psm.control;

    temp = CsrBtHdpUtilConnectionEntryMatchingBdAddr(HDP_CONNECTION_GET_FIRST(instData->connectionList), bd_addr);
    assocId = temp->assocChId;

    while(node != NULL)
    {
       if(node->assocChId == assocId)
       {
         for(i = 0;i < suppFeatureLen; i++)
         {
           mdepNode = (record->supportedFeatureList + i);
           if(node->mdepId == mdepNode->id && node->mdepDataType == mdepNode->dataType)
           {
              node->remotecontrolPsm = (CsrUint16)(*handle >> 16);
              node->remotedataPsm = (CsrUint16)(*handle & CSR_BT_HDP_INSTANCE_MDL_MASK);
              CsrBtMcapConnectMclReqSend(node->bd_addr,localCtrlPsm ,node->remotecontrolPsm , CSR_BT_HDP_IFACEQUEUE, (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) , CSR_BT_SNIFF_TIMEOUT_MCL);
              return;
           }
         }
         CsrBtHdpResumeCfmSend(instData->appHandle,&node->bd_addr, assocId, CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR, CSR_BT_SUPPLIER_HDP);
       }
       node = node->next;
    }
}

CsrUint16 CsrBtHdpUtilGetNextFreeMdl( CsrUint16 *mdlList)
{
    CsrInt16 i = 0 ;

    while ( (i < HDP_MAX_MDL_ID) && ( ( ( *mdlList >>  i ) & 0x1 )  !=  FALSE ))
    {
        i++;
    }
    *mdlList = *mdlList | (0x0001 << i);
    return i;
}

L2CA_FLOW_T * CsrBtHdpFillFlowParameters(l2ca_flow_mode_t mode, CsrUint16 mtu)
{
   L2CA_FLOW_T *flow;
   flow                   = CsrPmemAlloc(sizeof(L2CA_FLOW_T));
   flow->mode             = mode;
   flow->tx_window        = HDP_DATA_TX_WINDOW_SIZE;
   flow->max_retransmit   = MCAP_CONTROL_CHANNEL_MAX_RETRANS;
   flow->retrans_timeout  = MCAP_CONTROL_CHANNEL_RETRANS_TIMEOUT;
   flow->monitor_timeout  = MCAP_CONTROL_CHANNEL_MONITOR_TIMEOUT;
   flow->maximum_pdu      = mtu;
   return flow;
}

void CsrBtHdpDeInitInstance(HdpInstanceData *pInstance)
{
    pInstance->state = HDP_IDLE_STATE;

    pInstance->mdepDataTypeMask = 0;
    pInstance->sdpState = HDP_SDP_IDLE_STATE;
    pInstance->sdpSearchData = CsrBtUtilSdcInit(CsrBtHdpSdcResultHandler, CSR_BT_HDP_IFACEQUEUE);
    pInstance->state = HDP_IDLE_STATE;
    pInstance->currentMdepId = 0;
    pInstance->countNumOfMdep = 0;
    pInstance->psm.control = HDP_PSM_INVALID;
    pInstance->psm.data = HDP_PSM_INVALID;

    pInstance->serviceName = NULL;
    pInstance->serviceDescription = NULL;
    pInstance->hdpInstanceId = 0;
    pInstance->mdlIdList = 1;
    pInstance->getCapab = 0;

    pInstance->providerName = NULL;

    pInstance->hServiceRecord = HDP_SDP_INVALID_SR_HANDLE;

}

#endif


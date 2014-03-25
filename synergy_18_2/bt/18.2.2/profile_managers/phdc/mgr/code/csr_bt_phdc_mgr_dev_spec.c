
/****************************************************************************

Copyright Cambridge Silicon Radio Limited and its licensors 2009-2011. All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_MODULE


#include "csr_bt_phdc_mgr_dev_spec.h"
#include "csr_bt_phdc_mgr_prim.h"
#include "csr_util.h"
#include "csr_bt_phdc_mgr_util.h"
#include "csr_bt_phdc_mgr_out.h"
#include "csr_pmem.h"

#define PHDC_MGR_BPM_OBJ_HDL_BP          (1)
#define PHDC_MGR_BPM_OBJ_HDL_PUL         (2)
#define PHDC_MGR_WS_OBJ_HDL_WEIGHT       (1)


static CsrUint16 getWord(CsrUint8  *pString,CsrUint16 *pDecodedBytes)
{
    CsrUint16 value;

    value = pString[0];
    value = value << 8 | pString[1] ;

    *pDecodedBytes +=2;

    return value;
}

static CsrBool csrBtPhdcMgrCanWsWeightIndBeUsed(AttrValMap *pAttrValMap)
{
    CsrUint16 i;
    AttrValMapEntry *pAttrValMapEntry = pAttrValMap->value;
    CsrBool use = TRUE;

    for(i = 0; (i < pAttrValMap->count ) &&
               (use == TRUE ); i++,pAttrValMapEntry++)
    {
        switch (pAttrValMapEntry->attribute_id )
        {
            case MDC_ATTR_NU_VAL_OBS_SIMP :
            case MDC_ATTR_TIME_STAMP_ABS  :
            case MDC_ATTR_UNIT_CODE :
            {
                use = TRUE;
                break;
            }
            default :
            {
                use = FALSE;
                break;
            }
        }
    }

    return use;
}

static CsrBool csrBtPhdcMgrCanBpmBpIndBeUsed(AttrValMap *pAttrValMap)
{
    CsrUint16 i;
    AttrValMapEntry *pAttrValMapEntry = pAttrValMap->value;
    CsrBool use = TRUE;

    for(i = 0; (i < pAttrValMap->count ) &&
               (use == TRUE ); i++,pAttrValMapEntry++)
    {
        switch (pAttrValMapEntry->attribute_id )
        {
            case MDC_ATTR_NU_CMPD_VAL_OBS_BASIC :
            case MDC_ATTR_TIME_STAMP_ABS  :
            case MDC_ATTR_UNIT_CODE :
            {
                use = TRUE;
                break;
            }
            default :
            {
                use = FALSE;
                break;
            }
        }
    }

    return use;
}

static CsrBool csrBtPhdcMgrCanBpmPulseIndBeUsed(AttrValMap *pAttrValMap)
{
    CsrUint16 i;
    AttrValMapEntry *pAttrValMapEntry = pAttrValMap->value;
    CsrBool use = TRUE;

    for(i = 0; (i < pAttrValMap->count ) &&
               (use == TRUE ); i++,pAttrValMapEntry++)
    {
        switch (pAttrValMapEntry->attribute_id )
        {
            case MDC_ATTR_NU_VAL_OBS_BASIC :
            case MDC_ATTR_TIME_STAMP_ABS  :
            case MDC_ATTR_UNIT_CODE :
            {
                use = TRUE;
                break;
            }
            default :
            {
                use = FALSE;
                break;
            }
        }
    }

    return use;
}


static CsrBool decodeObsScanFixStdWt(octet_string               *pString,
                                     AttrValMap                 *pAttrValMap,
                                     CsrBtPhdcMgrAbsoluteTim    *pTime,
                                     CsrUint32                  *pWeight,
                                     CsrUint16                  *pUnitCode )
{
    CsrUint16  bytesToDecode =  pString->length, decodedBytes = 0;
    CsrUint16 i;
    AttrValMapEntry *pAttrValMapEntry = pAttrValMap->value;    

    for(i = 0; i < pAttrValMap->count; i++,pAttrValMapEntry++)
    {
        switch (pAttrValMapEntry->attribute_id)
        {
            case MDC_ATTR_NU_VAL_OBS_SIMP:
            {
                if ( decodedBytes < bytesToDecode )
                {
                    *pWeight = ((pString->value[decodedBytes+0]  << 24 ) |
                                (pString->value[decodedBytes+1]  << 16 ) |
                                (pString->value[decodedBytes+2]  << 8 ) |
                                (pString->value[decodedBytes+3]));
                                decodedBytes +=4;
                }
                else
                {
                    return FALSE;                
                }
                

                break;
            }
            case MDC_ATTR_TIME_STAMP_ABS:
            {
                if ( ( decodedBytes < bytesToDecode ) && 
                     ( pAttrValMapEntry->attribute_len < bytesToDecode) )
                {
                    CsrMemCpy(pTime,&pString->value[decodedBytes],pAttrValMapEntry->attribute_len);
                    decodedBytes += pAttrValMapEntry->attribute_len;
                }
                else
                {
                    return FALSE;                
                }
                break;
            }
            case MDC_ATTR_UNIT_CODE:
            {
                if ( ( decodedBytes < bytesToDecode ) && 
                     ( pAttrValMapEntry->attribute_len < bytesToDecode) )
                {
                    *pUnitCode = getWord(&pString->value[decodedBytes], &decodedBytes);
                }
                else
                {
                    return FALSE;
                }

                break;
            }            
            default:
                break;
        }
    }           

    return TRUE;
}

CsrUint16 decodeObsScanFixStdBpmBpMeaurement(CsrUint16     bytesToDecode,
                                             CsrUint8     *pString,
                                             MetricIdList *pMetricIdList,
                                             CsrUint16    *pSys,
                                             CsrUint16    *pDis,
                                             CsrUint16    *pMap)
{
    CsrUint16  i,decodedBytes = 0;
    OID_Type  *pOID_Type;

    *pSys = CSR_BT_PHDC_MGR_INVALID_VAL;
    *pDis = CSR_BT_PHDC_MGR_INVALID_VAL;    
    *pMap = CSR_BT_PHDC_MGR_INVALID_VAL;  

    pOID_Type = pMetricIdList->value;
    
    for( i = 0; (i < pMetricIdList->count ) &&
                (decodedBytes < bytesToDecode ); i++,pOID_Type++)
    {
        if (*pOID_Type == MDC_PRESS_BLD_NONINV_SYS)
        {
            *pSys = getWord(&pString[decodedBytes], &decodedBytes);
        }
        else if (*pOID_Type == MDC_PRESS_BLD_NONINV_DIA)
        {
            *pDis = getWord(&pString[decodedBytes], &decodedBytes);
        }            
        else if (*pOID_Type == MDC_PRESS_BLD_NONINV_MEAN)
        {
            *pMap = getWord(&pString[decodedBytes], &decodedBytes);
        }
    }

    return decodedBytes;
}

static CsrBool decodeObsScanFixStdBpmBp(octet_string            *pString,
                                        AttrValMap              *pAttrValMap,
                                        MetricIdList            *pMetricIdList,
                                        CsrBtPhdcMgrAbsoluteTim *pTime,
                                        CsrUint16               *pSys,
                                        CsrUint16               *pDis,
                                        CsrUint16               *pMap,
                                        CsrUint16               *pUnitCode)
{
    CsrUint16  bytesToDecode =  pString->length, decodedBytes = 0;
    CsrUint16   noObjects, lenNumericObject;
    CsrUint16 i;
    AttrValMapEntry *pAttrValMapEntry; 

    /* Decode compound numeric object number of objects and length */
    noObjects = getWord(&pString->value[decodedBytes], &decodedBytes);
    lenNumericObject = getWord(&pString->value[decodedBytes], &decodedBytes);

    if ( noObjects == 0 &&
         lenNumericObject == 0 )
    {
         return FALSE;
    }

    pAttrValMapEntry = pAttrValMap->value ;    

    for(i = 0; i < pAttrValMap->count; i++,pAttrValMapEntry++)
    {
        switch (pAttrValMapEntry->attribute_id)
        {
            case MDC_ATTR_NU_CMPD_VAL_OBS_BASIC:
            {
                if (decodedBytes < bytesToDecode)
                {
                    decodedBytes += decodeObsScanFixStdBpmBpMeaurement((CsrUint16)(bytesToDecode - decodedBytes), 
                                                                       &pString->value[decodedBytes], 
                                                                       pMetricIdList,
                                                                       pSys,
                                                                       pDis, 
                                                                       pMap);
                }
                else
                {
                    return FALSE;                
                }
                
                break;
            }
            case MDC_ATTR_TIME_STAMP_ABS:
            {
                if ( ( decodedBytes < bytesToDecode ) && 
                    ( pAttrValMapEntry->attribute_len < bytesToDecode) )
                {
                    CsrMemCpy(pTime,&pString->value[decodedBytes],pAttrValMapEntry->attribute_len);
                    decodedBytes += pAttrValMapEntry->attribute_len;
                }
                else
                {
                    return FALSE;                
                }
                break;
            }
            case MDC_ATTR_UNIT_CODE:
            {
                if ( ( decodedBytes < bytesToDecode ) && 
                     ( pAttrValMapEntry->attribute_len < bytesToDecode) )
                {
                    *pUnitCode = getWord(&pString->value[decodedBytes], &decodedBytes);
                }
                else
                {
                    return FALSE;
                }
                break;
            }
            default:
                break;
        }
    }
    return TRUE;
}

static CsrBool decodeObsScanFixStdBpmPulse(octet_string *pString,
                                           AttrValMap *pAttrValMap,
                                           CsrBtPhdcMgrAbsoluteTim  *pTime,
                                           CsrUint16       *pPulse,
                                           CsrUint16       *pUnitCode)
{
    CsrUint16 bytesToDecode =  pString->length,decodedBytes = 0;
    CsrUint16 i;
    AttrValMapEntry *pAttrValMapEntry;     

    pAttrValMapEntry = pAttrValMap->value;    

    for(i = 0; i < pAttrValMap->count; i++,pAttrValMapEntry++)
    {
        switch (pAttrValMapEntry->attribute_id)
        {
            case MDC_ATTR_NU_VAL_OBS_BASIC:
            {
                if ( decodedBytes < bytesToDecode )
                {
                    *pPulse = getWord(&pString->value[decodedBytes], &decodedBytes);
                }
                else
                {
                    return FALSE;
                }
                break;
            }
            case MDC_ATTR_TIME_STAMP_ABS:
            {
                if ( ( decodedBytes < bytesToDecode ) && 
                     ( pAttrValMapEntry->attribute_len < bytesToDecode) )
                {
                    CsrMemCpy(pTime,&pString->value[decodedBytes],pAttrValMapEntry->attribute_len);
                    decodedBytes += pAttrValMapEntry->attribute_len;
                }
                else
                {
                    return FALSE;
                }

                break;
            }
            case MDC_ATTR_UNIT_CODE:
            {
                if ( ( decodedBytes < bytesToDecode ) && 
                     ( pAttrValMapEntry->attribute_len < bytesToDecode) )
                {
                    *pUnitCode = getWord(&pString->value[decodedBytes], &decodedBytes);
                }
                else
                {
                    return FALSE;
                }

                break;
            }             
            default:
                break;
        }
    }      

    return TRUE;
}


static CsrBool decodeObsScanVarBpmBp(BasicNuObsValueCmp *pCmpVal,
                                     MetricIdList *pMetricIdList,
                                     CsrUint16       *pSys,
                                     CsrUint16       *pDis,
                                     CsrUint16       *pMap)
{
    CsrUint16 i;
    OID_Type  *pOID_Type = pMetricIdList->value;
    BasicNuObsValue *pNuValue = pCmpVal->value;

    *pSys = CSR_BT_PHDC_MGR_INVALID_VAL;
    *pDis = CSR_BT_PHDC_MGR_INVALID_VAL;    
    *pMap = CSR_BT_PHDC_MGR_INVALID_VAL;    

    if ( pMetricIdList->count != pCmpVal->count)
    {
        return FALSE;
    }

    
    for( i = 0; (i < pMetricIdList->count ) ; i++,pOID_Type++,pNuValue++)
    {
        if (*pOID_Type == MDC_PRESS_BLD_NONINV_SYS)
        {
            *pSys = *pNuValue;
        }
        else if (*pOID_Type == MDC_PRESS_BLD_NONINV_DIA)
        {
            *pDis = *pNuValue;
        }
        else if (*pOID_Type == MDC_PRESS_BLD_NONINV_MEAN)
        {
            *pMap = *pNuValue;
        }
    }

    return TRUE;
}

void DecodeObsevationScanFixed(CsrSchedQid              qId,
                               PhdcMgrDeviceListEntry   *pDevice,
                               OID_Type                 eventType,
                               PersonId                 personId, 
                               CsrUint16                objHandle,
                               ObservationScanFixed     *pScan)
{
    CsrUint8 i = 0;
    TypeVer  *pDevSpec ;

    pDevSpec = pDevice->pMds->pSpec->value;

    for  ( i=0 ; i < pDevice->pMds->pSpec->count; i++,pDevSpec++  )
    {
        switch ( pDevSpec->type)
        {
            case MDC_DEV_SPEC_PROFILE_BP :
            {
                PhdcMgrConfigObjectEntry *pObject;
                
                pObject= PHDC_MGR_OBJECT_LIST_FIND_FROM_HANDLE(pDevice->pMds->configObjList,&objHandle);
                
                if ( pObject != NULL && 
                     pObject->useAVM == TRUE)
                {
                    /* standard configuration */
                    if (objHandle== PHDC_MGR_BPM_OBJ_HDL_BP )
                    {
                        CsrBtPhdcMgrAbsoluteTim     time;
                        CsrUint16                   systolicPressure = 0;
                        CsrUint16                   diastolicPressure = 0;
                        CsrUint16                   meanArterialPressure = 0;
                        CsrUint16                   unitCode = pObject->unitCode;

                        if (decodeObsScanFixStdBpmBp(&pScan->obs_val_data,
                                                     pObject->pAttrValMap,
                                                     pObject->pMetricIdList,
                                                     &time, 
                                                     &systolicPressure, 
                                                     &diastolicPressure,
                                                     &meanArterialPressure,
                                                     &unitCode) )
                        {
                            CsrBtPhdcMgrBpmBpIndSend(qId, 
                                                     pDevice->dataChannelId, 
                                                     personId,
                                                     unitCode, 
                                                     &time,
                                                     systolicPressure, 
                                                     diastolicPressure, 
                                                     meanArterialPressure);
                            
                        }
                        else
                        {
                            /* ignore */
                        }
                                
                    }
                    else if (objHandle == PHDC_MGR_BPM_OBJ_HDL_PUL)
                    {
                        CsrBtPhdcMgrAbsoluteTim     time;
                        CsrUint16                   Pulse = 0;
                        CsrUint16                   unitCode = pObject->unitCode;

                        if (decodeObsScanFixStdBpmPulse(&pScan->obs_val_data,
                                                        pObject->pAttrValMap,
                                                        &time, 
                                                        &Pulse,
                                                        &unitCode))
                        {
                            CsrBtPhdcMgrBpmPulseIndSend(qId, 
                                                        pDevice->dataChannelId,
                                                        personId,
                                                        unitCode, 
                                                        &time,
                                                        Pulse);
                        }
                        else
                        {
                            /* ignore */
                        }
                                                  
                        
                    }
                    else
                    {
                        /* vendor specific */
                        CsrBtPhdcScanReportType scanReportType = ( personId == CSR_BT_PHDC_MGR_PERSON_ID_NA ) ? 
                                                                    CSR_BT_PHDC_MGR_SCAN_REPORT_TYPE_FIXED:
                                                                    CSR_BT_PHDC_MGR_SCAN_REPORT_TYPE_MP_FIXED;
                        
                        CsrUint8 *pOctetString  = CsrPmemAlloc(pScan->obs_val_data.length);
                        CsrMemCpy(pOctetString, pScan->obs_val_data.value, pScan->obs_val_data.length);
                        
                        CsrBtPhdcMgrDsIndSend(qId, 
                                              pDevice->dataChannelId,
                                              objHandle, 
                                              personId,
                                              scanReportType,
                                              pScan->obs_val_data.length,
                                              pOctetString);
                    }

                }
                else if (pObject != NULL)
                {
                    /* vendor specific or a Attribute Value Map Cannot be used */
                    CsrBtPhdcScanReportType scanReportType = ( personId == CSR_BT_PHDC_MGR_PERSON_ID_NA ) ? 
                                                                CSR_BT_PHDC_MGR_SCAN_REPORT_TYPE_FIXED:
                                                                CSR_BT_PHDC_MGR_SCAN_REPORT_TYPE_MP_FIXED;

                    CsrUint8 *pOctetString  = CsrPmemAlloc(pScan->obs_val_data.length);
                    CsrMemCpy(pOctetString, pScan->obs_val_data.value, pScan->obs_val_data.length);
                    
                    CsrBtPhdcMgrDsIndSend(qId, 
                                          pDevice->dataChannelId,
                                          objHandle, 
                                          personId,
                                          scanReportType,
                                          pScan->obs_val_data.length,
                                          pOctetString);
                }
                else
                {
                    /* object handle is not indicated in configuration ignore the data */
                }

                break;
            }
            
            case MDC_DEV_SPEC_PROFILE_SCALE :
            {
                PhdcMgrConfigObjectEntry *pObject;
                
                pObject= PHDC_MGR_OBJECT_LIST_FIND_FROM_HANDLE(pDevice->pMds->configObjList,&objHandle);
                
                if ( pObject != NULL && 
                        pDevice->pMds->devConfigId == CSR_BT_PHDC_MGR_DEVICE_CONFIG_ID_WS &&
                        pObject->useAVM == TRUE )
                {
                    /* standard configuration */
                    if (objHandle== PHDC_MGR_WS_OBJ_HDL_WEIGHT)
                    {
                        CsrBtPhdcMgrAbsoluteTim     time;
                        CsrUint32                   weight=0;
                        CsrUint16                   unitCode = pObject->unitCode;

                        if (decodeObsScanFixStdWt(&pScan->obs_val_data,
                                                  pObject->pAttrValMap,  
                                                  &time, 
                                                  &weight,
                                                  &unitCode) )
                        {
                            CsrBtPhdcMgrWsWeightIndSend(qId, 
                                                        pDevice->dataChannelId, 
                                                        personId,
                                                        unitCode, 
                                                        &time,
                                                        weight);
                            
                        }
                        else
                        {
                            /* ignore */
                        }
                                
                    }                    
                    else
                    {
                        /* ignore */
                    }

                }
                else if (pObject != NULL)
                {
                    /* vendor specific */
                    CsrBtPhdcScanReportType scanReportType = ( personId == CSR_BT_PHDC_MGR_PERSON_ID_NA ) ? 
                                                               CSR_BT_PHDC_MGR_SCAN_REPORT_TYPE_FIXED:
                                                               CSR_BT_PHDC_MGR_SCAN_REPORT_TYPE_MP_FIXED;
                    CsrUint8 *pOctetString  = CsrPmemAlloc(pScan->obs_val_data.length);
                    CsrMemCpy(pOctetString, pScan->obs_val_data.value, pScan->obs_val_data.length);
                    
                    CsrBtPhdcMgrDsIndSend(qId, 
                                          pDevice->dataChannelId,
                                          objHandle, 
                                          personId,
                                          scanReportType,
                                          pScan->obs_val_data.length,
                                          pOctetString);
                }
                else
                {
                    /* object handle is not indicated in configuration ignore the data */
                }

                break;
            }            
            default :
                break;
            
        }
    }
}

CsrBool DecodeObsevationScanVar(CsrSchedQid            qId,
                                PhdcMgrDeviceListEntry *pDevice,
                                OID_Type               eventType,
                                PersonId               personId, 
                                CsrUint16              objHandle,
                                ObservationScan        *pScan)
{
    /* @todo - Move this template to a separate for file
       @brief - Implementation specific handling      
       @note - if there is a change of unit code, update MDS info */
    CsrUint8 i = 0;
    TypeVer  *pDevSpec;
    CsrBool obsScanHandled = FALSE;
    AttributeList *pAttributes;
    CsrUint16 decodedBytes = 0;
    
    CSR_UNUSED(eventType);
    pDevSpec = pDevice->pMds->pSpec->value;
    pAttributes = &pScan->attributes;

    for  ( i=0 ; i < pDevice->pMds->pSpec->count; i++,pDevSpec++  )
    {
        switch ( pDevSpec->type)
        {
            case MDC_DEV_SPEC_PROFILE_BP :
            {
                PhdcMgrConfigObjectEntry *pObject;
                
                pObject = PHDC_MGR_OBJECT_LIST_FIND_FROM_HANDLE(pDevice->pMds->configObjList,&objHandle);
                
                if ( pObject != NULL )
                {
                    CsrUint16 c = pAttributes->count, i = 0;
                    AVA_Type *pAttribute = &pAttributes->value[0];

                    if (pObject->objType == ( PHDC_MGR_MOVE_LF_16(MDC_PART_SCADA) | MDC_PRESS_BLD_NONINV ) )
                    {
                        CsrBtPhdcMgrAbsoluteTim        time = {CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL};

                        CsrUint16 systolicPressure = CSR_BT_PHDC_MGR_INVALID_VAL;
                        CsrUint16 diastolicPressure = CSR_BT_PHDC_MGR_INVALID_VAL;
                        CsrUint16 meanArterialPressure = CSR_BT_PHDC_MGR_INVALID_VAL;
                        CsrBool bMeas = FALSE;

                        obsScanHandled = TRUE;
                        
                        for ( i =0 ; i < c; i++, pAttribute++)
                        {
                            if (pAttribute->attribute_id == MDC_ATTR_UNIT_CODE )
                            {
                                pObject->unitCode = getWord((CsrUint8 * )pAttribute->attribute_value.value, &decodedBytes);
                            }
                            else if (pAttribute->attribute_id == MDC_ATTR_ATTRIBUTE_VAL_MAP )
                            {
                                CsrPmemFree(pObject->pAttrValMap->value);
                                CsrPmemFree(pObject->pAttrValMap);
                                (void)CsrBtPhdcMgrDecodeAttrValMap(pAttribute->attribute_value.value,&pObject->pAttrValMap);
                                pObject->useAVM = csrBtPhdcMgrCanBpmBpIndBeUsed(pObject->pAttrValMap);
                                if (pObject->useAVM == FALSE)
                                {
                                    /* The AVM cannot be used with CSR template.Send it to the application
                                       so that it can use this template to read DsDataInd*/
                                
                                       AVA_Type *pUpdatedAttribute ;
                                       CsrUint16 dataLen;

                                       pUpdatedAttribute = CsrPmemAlloc(sizeof (AVA_Type));
                                       *pUpdatedAttribute = *pAttribute;
                                       pUpdatedAttribute->attribute_value.value = CsrPmemAlloc(pAttribute->attribute_value.length);
                                       CsrMemCpy(pUpdatedAttribute->attribute_value.value, 
                                                 pAttribute->attribute_value.value, 
                                                 pAttribute->attribute_value.length);
                                
                                       dataLen = sizeof(AVA_Type);
                                        
                                       CsrBtPhdcMgrExtConfigObjIndSend(qId, 
                                                                   pDevice->pMds->devConfigId,
                                                                   &pDevice->bdAddr, 
                                                                   objHandle, 
                                                                   dataLen,
                                                                   (CsrUint8 *)pUpdatedAttribute,
                                                                   CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_OPCODE_UPDATE);                                   
                            }
                            }
                            else if (pAttribute->attribute_id == MDC_ATTR_NU_CMPD_VAL_OBS_BASIC)
                            {
                                BasicNuObsValueCmp  *pBasicNuObsValueCmp = NULL;
                                
                                CsrBtPhdcMgrDecodeBasicNuObsValueCmp(pAttribute->attribute_value.value,
                                                                     &pBasicNuObsValueCmp);
                                decodeObsScanVarBpmBp( pBasicNuObsValueCmp,
                                                       pObject->pMetricIdList,
                                                       &systolicPressure, 
                                                       &diastolicPressure,
                                                       &meanArterialPressure);
                                CsrPmemFree(pBasicNuObsValueCmp->value);
                                CsrPmemFree(pBasicNuObsValueCmp);
                                bMeas = TRUE;
                            }
                            else if (pAttribute->attribute_id == MDC_ATTR_TIME_STAMP_ABS)
                            {
                                CsrMemCpy(&time,pAttribute->attribute_value.value,8);
                            }
                            else if (pAttribute->attribute_id == MDC_ATTR_ID_PHYSIO_LIST)
                            {
                                if ( pObject->pMetricIdList != NULL )
                                {
                                    CsrPmemFree(pObject->pMetricIdList->value);
                                    CsrPmemFree(pObject->pMetricIdList);
                                }
                                (void)CsrBtPhdcMgrDecodeMetricIdList(pAttribute->attribute_value.value, &pObject->pMetricIdList);
                            }
                            else
                            {
                                /* The attribute id is either a standard or vendor specific.
                                   In either case, it is an object attibute which is being updated.
                                   Send it to the application */

                                   AVA_Type *pUpdatedAttribute ;
                                   CsrUint16 dataLen;

                                   pUpdatedAttribute = CsrPmemAlloc(sizeof (AVA_Type));
                                   *pUpdatedAttribute = *pAttribute;
                                   pUpdatedAttribute->attribute_value.value = CsrPmemAlloc(pAttribute->attribute_value.length);
                                   CsrMemCpy(pUpdatedAttribute->attribute_value.value, 
                                             pAttribute->attribute_value.value, 
                                             pAttribute->attribute_value.length);

                                   dataLen = sizeof(AVA_Type);
                                    
                                   CsrBtPhdcMgrExtConfigObjIndSend(qId, 
                                                               pDevice->pMds->devConfigId,
                                                               &pDevice->bdAddr, 
                                                               objHandle, 
                                                               dataLen,
                                                               (CsrUint8 *)pUpdatedAttribute,
                                                               CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_OPCODE_UPDATE);                                   
                            }
                        }
                        if ( bMeas == TRUE )
                        {
                            CsrBtPhdcMgrBpmBpIndSend(qId, pDevice->dataChannelId, 
                                                    personId,
                                                    pObject->unitCode, 
                                                    &time,
                                                    systolicPressure, 
                                                    diastolicPressure, 
                                                    meanArterialPressure);
                        }
                    }
                    else if ( pObject->objType == ( PHDC_MGR_MOVE_LF_16(MDC_PART_SCADA) | MDC_PULS_RATE_NON_INV) )
                    {
                        CsrBtPhdcMgrAbsoluteTim time = {CSR_BT_PHDC_MGR_INVALID_VAL,
                                                        CSR_BT_PHDC_MGR_INVALID_VAL,
                                                        CSR_BT_PHDC_MGR_INVALID_VAL,
                                                        CSR_BT_PHDC_MGR_INVALID_VAL,
                                                        CSR_BT_PHDC_MGR_INVALID_VAL,
                                                        CSR_BT_PHDC_MGR_INVALID_VAL,
                                                        CSR_BT_PHDC_MGR_INVALID_VAL,
                                                        CSR_BT_PHDC_MGR_INVALID_VAL};
                        
                        CsrUint16 Pulse = CSR_BT_PHDC_MGR_INVALID_VAL;               
                        CsrBool bMeas = FALSE;

                        obsScanHandled = TRUE;

                        for ( i =0 ; i < c; i++, pAttribute++)
                        {                            
                            if (pAttribute->attribute_id == MDC_ATTR_UNIT_CODE)
                            {
                                pObject->unitCode = getWord((CsrUint8 * )pAttribute->attribute_value.value, &decodedBytes);

                            }
                            else if (pAttribute->attribute_id == MDC_ATTR_ATTRIBUTE_VAL_MAP )
                            {
                                CsrPmemFree(pObject->pAttrValMap->value);
                                CsrPmemFree(pObject->pAttrValMap);
                                (void)CsrBtPhdcMgrDecodeAttrValMap(pAttribute->attribute_value.value,&pObject->pAttrValMap);
                                pObject->useAVM = csrBtPhdcMgrCanBpmPulseIndBeUsed(pObject->pAttrValMap);

                                if (pObject->useAVM == FALSE)
                                {
                                    /* The AVM cannot be used with CSR template.Send it to the application
                                       so that it can use this template to read DsDataInd*/
                                
                                       AVA_Type *pUpdatedAttribute ;
                                       CsrUint16 dataLen;
                                
                                       pUpdatedAttribute = CsrPmemAlloc(sizeof (AVA_Type));
                                       *pUpdatedAttribute = *pAttribute;
                                       pUpdatedAttribute->attribute_value.value = CsrPmemAlloc(pAttribute->attribute_value.length);
                                       CsrMemCpy(pUpdatedAttribute->attribute_value.value, 
                                                 pAttribute->attribute_value.value, 
                                                 pAttribute->attribute_value.length);
                                
                                       dataLen = sizeof(AVA_Type);
                                        
                                       CsrBtPhdcMgrExtConfigObjIndSend(qId, 
                                                                   pDevice->pMds->devConfigId,
                                                                   &pDevice->bdAddr, 
                                                                   objHandle, 
                                                                   dataLen,
                                                                   (CsrUint8 *)pUpdatedAttribute,
                                                                   CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_OPCODE_UPDATE);                                   
                            }                            
                            }                            
                            else if (pAttribute->attribute_id == MDC_ATTR_NU_VAL_OBS_BASIC)
                            {
                                Pulse = getWord((CsrUint8 * )pAttribute->attribute_value.value, &decodedBytes);
                                bMeas = TRUE;
                            }
                            else if (pAttribute->attribute_id == MDC_ATTR_TIME_STAMP_ABS)
                            {
                                CsrMemCpy(&time,pAttribute->attribute_value.value,8);
                            }
                            else
                            {
                                /* The attribute id is either a standard or vendor specific.
                                   In either case, it is an object attibute which is being updated.
                                   Send it to the application */

                                   AVA_Type *pUpdatedAttribute ;
                                   CsrUint16 dataLen;

                                   pUpdatedAttribute = CsrPmemAlloc(sizeof (AVA_Type));
                                   *pUpdatedAttribute = *pAttribute;
                                   pUpdatedAttribute->attribute_value.value = CsrPmemAlloc(pAttribute->attribute_value.length);
                                   CsrMemCpy(pUpdatedAttribute->attribute_value.value, 
                                             pAttribute->attribute_value.value, 
                                             pAttribute->attribute_value.length);

                                   dataLen = sizeof(AVA_Type);
                                    
                                   CsrBtPhdcMgrExtConfigObjIndSend(qId, 
                                                               pDevice->pMds->devConfigId,
                                                               &pDevice->bdAddr, 
                                                               objHandle, 
                                                               dataLen,
                                                               (CsrUint8 *)pUpdatedAttribute,
                                                               CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_OPCODE_UPDATE);                                   
                            }                            
                        }
                        if ( bMeas == TRUE )
                        {
                            CsrBtPhdcMgrBpmPulseIndSend(qId, pDevice->dataChannelId, 
                                                        personId,
                                                        pObject->unitCode, 
                                                        &time,Pulse);

                        }                        
                    }
                    else
                    {
                        /* Handled after return directly on recieved data string */ 
                    }
                }
                else
                {
                    /* object handle is not indicated in configuration ignore the data */
                    obsScanHandled = TRUE; 
                }

                break;
            }
            
            case MDC_DEV_SPEC_PROFILE_SCALE :
            {
                PhdcMgrConfigObjectEntry *pObject;
                
                pObject = PHDC_MGR_OBJECT_LIST_FIND_FROM_HANDLE(pDevice->pMds->configObjList,&objHandle);
                
                if ( pObject != NULL )
                {
                    CsrUint16 c = pAttributes->count, i = 0;
                    AVA_Type *pAttribute = &pAttributes->value[0];

                    if (pObject->objType == ( PHDC_MGR_MOVE_LF_16(MDC_PART_SCADA) | MDC_MASS_BODY_ACTUAL ) )
                    {
                        CsrBtPhdcMgrAbsoluteTim time = {CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL,
                                                             CSR_BT_PHDC_MGR_INVALID_VAL};
                        CsrUint32       weight = CSR_BT_PHDC_MGR_INVALID_VAL;
                        CsrBool bMeas = FALSE;
                        
                        obsScanHandled = TRUE;

                        for ( i =0 ; i < c; i++, pAttribute++)
                        {
                            if (pAttribute->attribute_id == MDC_ATTR_UNIT_CODE)
                            {
                                pObject->unitCode = getWord((CsrUint8 * )pAttribute->attribute_value.value, &decodedBytes);
                            }
                            else if (pAttribute->attribute_id == MDC_ATTR_ATTRIBUTE_VAL_MAP )
                            {
                                CsrPmemFree(pObject->pAttrValMap->value);
                                CsrPmemFree(pObject->pAttrValMap);
                                (void)CsrBtPhdcMgrDecodeAttrValMap(pAttribute->attribute_value.value,&pObject->pAttrValMap);
                                pObject->useAVM = csrBtPhdcMgrCanWsWeightIndBeUsed(pObject->pAttrValMap);
                                if (pObject->useAVM == FALSE)
                                {
                                    /* The AVM cannot be used with CSR template.Send it to the application
                                       so that it can use this template to read DsDataInd*/

                                       AVA_Type *pUpdatedAttribute ;
                                       CsrUint16 dataLen;
                                
                                       pUpdatedAttribute = CsrPmemAlloc(sizeof (AVA_Type));
                                       *pUpdatedAttribute = *pAttribute;
                                       pUpdatedAttribute->attribute_value.value = CsrPmemAlloc(pAttribute->attribute_value.length);
                                       CsrMemCpy(pUpdatedAttribute->attribute_value.value, 
                                                 pAttribute->attribute_value.value, 
                                                 pAttribute->attribute_value.length);
                                
                                       dataLen = sizeof(AVA_Type);
                                        
                                       CsrBtPhdcMgrExtConfigObjIndSend(qId, 
                                                                   pDevice->pMds->devConfigId,
                                                                   &pDevice->bdAddr, 
                                                                   objHandle, 
                                                                   dataLen,
                                                                   (CsrUint8 *)pUpdatedAttribute,
                                                                   CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_OPCODE_UPDATE);                                   
                                }                                  
                            }                            
                            else if (pAttribute->attribute_id == MDC_ATTR_NU_VAL_OBS_SIMP)
                            {
                                weight = ((pAttribute->attribute_value.value[0] << 24) |
                                        (pAttribute->attribute_value.value[1] << 16) |
                                        (pAttribute->attribute_value.value[2] << 8) |
                                        (pAttribute->attribute_value.value[3] ) );

                                bMeas = TRUE;
                            }
                            else if (pAttribute->attribute_id == MDC_ATTR_TIME_STAMP_ABS)
                            {
                                CsrMemCpy(&time,pAttribute->attribute_value.value,8);
                            }
                            else
                            {
                                /* The attribute id is either a standard or vendor specific.
                                   In either case, it is an object attibute which is being updated.
                                   Send it to the application */

                                   AVA_Type *pUpdatedAttribute ;
                                   CsrUint16 dataLen;

                                   pUpdatedAttribute = CsrPmemAlloc(sizeof (AVA_Type));
                                   *pUpdatedAttribute = *pAttribute;
                                   pUpdatedAttribute->attribute_value.value = CsrPmemAlloc(pAttribute->attribute_value.length);
                                   CsrMemCpy(pUpdatedAttribute->attribute_value.value, 
                                             pAttribute->attribute_value.value, 
                                             pAttribute->attribute_value.length);

                                   dataLen = sizeof(AVA_Type) ;
                                    
                                   CsrBtPhdcMgrExtConfigObjIndSend(qId, 
                                                               pDevice->pMds->devConfigId,
                                                               &pDevice->bdAddr, 
                                                               objHandle, 
                                                               dataLen,
                                                               (CsrUint8 *)pUpdatedAttribute,
                                                               CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_OPCODE_UPDATE);                                   
                            }                            
                        }

                        if ( bMeas == TRUE )
                        {
                            CsrBtPhdcMgrWsWeightIndSend(qId, 
                                                        pDevice->dataChannelId, 
                                                        personId,
                                                        pObject->unitCode, 
                                                        &time,
                                                        weight);
                        }
                    }
                    else
                    {
                        /* Handled after return directly on recieved data string */ 
                    }
                }
                else
                {
                    /* object handle is not indicated in configuration ignore the data */
                    obsScanHandled = TRUE; 
                }

                break;
            }            
            default :
                break;
            
        }
    }       
 
    return obsScanHandled;
}

#endif

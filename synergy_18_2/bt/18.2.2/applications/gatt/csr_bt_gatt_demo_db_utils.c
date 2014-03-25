/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 


REVISION:      $Revision: #8 $

 ****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_gatt_demo_db_utils.h"
#include "csr_bt_gatt_prim.h"
#include "csr_bt_gatt_lib.h"
#include "csr_list.h"
#include "csr_pmem.h"
#include "csr_bt_gatt_demo_defines.h"


void CsrUint8ArrayToHex(CsrUint8* value, CsrCharString *str, CsrUint16 length)
{
    CsrUint8 count;
    CsrUint8 index = 0;
    CsrUint8 currentValue, temp;
    if(str == NULL)
    {
        str = (CsrCharString*)CsrPmemZalloc(sizeof(CsrUint8)*length*2 + 1);
    }

    for(count = 0; count < length; count++)
    {
        temp = value[count];
        currentValue = (CsrUint8)(temp & 0x0F);

        str[index + 1] = (char)(currentValue > 9 ? currentValue + 55 : currentValue + '0');

        temp >>= 4;        
        currentValue = (CsrUint8)(temp & 0x0F);

        str[index] = (char)(currentValue > 9 ? currentValue + 55 : currentValue + '0');

        index += 2;
        if(index > length * 2)
        {
            break;
        }
    }
}

void CsrBtGattAppInitDbCharacDescrList(CsrCmnListElm_t *elem)
{
    /* Initialize a CsrBtGattDbCharacDescrElement. This function is called every
     * time a new entry is made on the queue list */
    CsrBtGattAppDbCharacDescrElement *dElem = (CsrBtGattAppDbCharacDescrElement *) elem;
    dElem->descriptorHandle                 = CSR_BT_GATT_ATTR_HANDLE_INVALID;
    dElem->value       = NULL;
    dElem->valueLength = 0;
}

void CsrBtGattAppFreeDbCharacDescrList(CsrCmnListElm_t *elem)
{
    /* CsrPmemFree local pointers in the CsrBtGattAppDbCharacDescrElement. 
     * This function is called every time a element is removed from the
     * Characteristics list */
    CsrBtGattAppDbCharacDescrElement *dElem = (CsrBtGattAppDbCharacDescrElement *) elem;

    CsrPmemFree(dElem->value);
}
void CsrBtGattAppInitDbCharacList(CsrCmnListElm_t *elem)
{
    /* Initialize a CsrBtGattDbCharacElement. This function is called every
     * time a new entry is made on the queue list */
    CsrBtGattAppDbCharacElement *cElem = (CsrBtGattAppDbCharacElement *) elem;
    cElem->valueHandle = CSR_BT_GATT_ATTR_HANDLE_INVALID;
    cElem->value       = NULL;
    cElem->valueLength = 0;
    CsrCmnListInit(&cElem->descrList, 0, CsrBtGattAppInitDbCharacDescrList, CsrBtGattAppFreeDbCharacDescrList);
}

void CsrBtGattAppFreeDbCharacList(CsrCmnListElm_t *elem)
{
    /* CsrPmemFree local pointers in the CsrBtGattAppDbCharacElement. 
     * This function is called every time a element is removed from the
     * Characteristics list */
    CsrBtGattAppDbCharacElement *cElem = (CsrBtGattAppDbCharacElement *) elem;
    CsrCmnListDeinit(&(cElem->descrList));
    CsrPmemFree(cElem->value);
}


void CsrBtGattAppInitDbPrimList(CsrCmnListElm_t *elem)
{
    /* Initialize a CsrBtGattDbPrimElement. This function is called every
     * time a new entry is made on the queue list */
    CsrBtGattAppDbPrimServiceElement *pElem = (CsrBtGattAppDbPrimServiceElement *) elem;
    pElem->startHandle    = CSR_BT_GATT_ATTR_HANDLE_INVALID;
    pElem->endHandle      = CSR_BT_GATT_ATTR_HANDLE_INVALID;
    CsrCmnListInit(&pElem->characList, 0, CsrBtGattAppInitDbCharacList, CsrBtGattAppFreeDbCharacList);

}

void CsrBtGattAppFreeDbPrimList(CsrCmnListElm_t *elem)
{
    /* CsrPmemFree local pointers in the CsrBtGattAppDbPrimElement.  
     * This function is called every time a element is removed from the
     * Primary Service List list */
    CsrBtGattAppDbPrimServiceElement *pElem = (CsrBtGattAppDbPrimServiceElement *) elem;
    CsrCmnListDeinit(&(pElem->characList));
}


CsrBool CsrBtGattAppDbFindPrimServiceByUuid(CsrCmnListElm_t *elem, void *value)
{ /* Return TRUE if uuid matches*/
    CsrBtUuid uuid             = *(CsrBtUuid *) value;
    CsrUint8 i;
    CsrBtGattAppDbPrimServiceElement *element = (CsrBtGattAppDbPrimServiceElement *)elem;
    if(element->uuid.length != uuid.length)
    {
        return FALSE;
    }
    for(i = 0; i< element->uuid.length; i++)
    {
        if(element->uuid.uuid[i] != uuid.uuid[i])
        { 
            return FALSE;
        }
    }

    /*the uuid is the same */
    return TRUE;
}


CsrBool CsrBtGattAppDbFindCharacByUuid(CsrCmnListElm_t *elem, void *value)
{ /* Return TRUE if uuid matches*/
    CsrBtUuid uuid             = *(CsrBtUuid *) value;
    CsrUint8 i;
    CsrBtGattAppDbCharacElement *element = (CsrBtGattAppDbCharacElement *)elem;
    if(element->uuid.length != uuid.length)
    {
        return FALSE;
    }
    for(i = 0; i< element->uuid.length; i++)
    {
        if(element->uuid.uuid[i] != uuid.uuid[i])
        { 
            return FALSE;
        }
    }
    /*the uuid is the same */
    return TRUE;
}
CsrBool CsrBtGattAppTestValueHandleInterval(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattAppDbPrimServiceElement *element = (CsrBtGattAppDbPrimServiceElement *)elem;
    CsrBtGattHandle valueHandle = *(CsrBtGattHandle *) value;

    if((element->startHandle < valueHandle) && (element->endHandle > valueHandle))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
CsrBool CsrBtGattAppTestValueHandle(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattAppDbCharacElement *element = (CsrBtGattAppDbCharacElement *) elem;
    CsrBtGattHandle valueHandle = *(CsrBtGattHandle *) value;

    if(element->valueHandle == valueHandle)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
CsrBtGattAppDbCharacElement *CsrBtGattAppDbFindCharacByValueHandle(CsrCmnList_t *cmnList, void *value)
{ /* Return TRUE if uuid matches*/
    CsrBtGattHandle valueHandle = *(CsrBtGattHandle *) value;
    
    CsrBtGattAppDbPrimServiceElement *element = (CsrBtGattAppDbPrimServiceElement *)CsrCmnListSearch(cmnList, CsrBtGattAppTestValueHandleInterval, &valueHandle);
    
    return (CsrBtGattAppDbCharacElement *)CsrCmnListSearch(&(element->characList), CsrBtGattAppTestValueHandle, &valueHandle);

}

CsrBool CsrBtGattAppDbFindCharacDescrByUuid(CsrCmnListElm_t *elem, void *value)
{ /* Return TRUE if uuid matches*/
    
    CsrBtUuid uuid             = *(CsrBtUuid *) value;
    CsrUint8 i;
    CsrBtGattAppDbCharacDescrElement *element = (CsrBtGattAppDbCharacDescrElement *)elem;
    if(element->uuid.length != uuid.length)
    {
        return FALSE;
    }
    for(i = 0; i< element->uuid.length; i++)
    {
        if(element->uuid.uuid[i] != uuid.uuid[i])
        { 
            return FALSE;
        }
    }
    /*the uuid is the same */
    return TRUE;
}
/* Find next readable characteristic element 
 * pElem    : Current primary Service Element
 * cElem    : Current characteristic Element, if NULL it will start with first charac in pElem.
 * property : Property that a charac value needs to obey (can be a mask of multiple properties)
 * The elements will get updated when using this function so use temp pointers if you do not want to get them updated
 */
CsrBool CsrBtGattAppDbFindNextCharacWithProperty(CsrBtGattAppDbPrimServiceElement **pElem, CsrBtGattAppDbCharacElement **cElem, CsrBtGattPropertiesBits property)
{
    CsrBtGattAppDbPrimServiceElement *primElem = (CsrBtGattAppDbPrimServiceElement *)*pElem;
    CsrBtGattAppDbCharacElement *characElem    = (CsrBtGattAppDbCharacElement *)*cElem;
    if (characElem == NULL)
    {
        /* */
        characElem = CSR_BT_GATT_APP_DB_GET_FIRST_CHARAC(primElem->characList);
    }
    else
    {
        /* move to next charac element in list */
        characElem = characElem->next;
        
    }
    while (primElem != NULL)  
    {
        while (characElem != NULL) 
        {
            if (((characElem->property & property) == property) || property == 0 /* passthrough if property is 0*/) 
            { 
                /* found a usable element  update pointers and return */
                *pElem = primElem;
                *cElem = characElem;
                return TRUE;
            }
            else
            {
                /* move to next charac element in list */
                characElem = characElem->next;
            }

        }
        /* no more readable charac elements in this */
        primElem = primElem->next;
        /* reset CharacElem - it will get updated as the first thing in the next loop */
        if(primElem != NULL)
        {
            characElem = CSR_BT_GATT_APP_DB_GET_FIRST_CHARAC(primElem->characList);
        }
    }
    /* could not find any element that mached the property in all of the elements */
    *pElem = NULL;
    *cElem = NULL;
    return FALSE;
}

/* Find next readable characteristic element 
 * cElem    : Current characteristic Element
 * dElem    : Current characteristic descriptor Element, if NULL it will start with first descr in cElem.
 * The elements will get updated when using this function so use temp pointers if you do not want to get them updated
 */
CsrBool CsrBtGattAppDbFindNextCharacDescriptor(CsrBtGattAppDbCharacElement **cElem, CsrBtGattAppDbCharacDescrElement **dElem)
{
    CsrBtGattAppDbCharacDescrElement *descrElem = (CsrBtGattAppDbCharacDescrElement *)*dElem;
    CsrBtGattAppDbCharacElement *characElem     = (CsrBtGattAppDbCharacElement *)*cElem;
    if (descrElem == NULL)
    {
        /* */
        descrElem = CSR_BT_GATT_APP_DB_GET_FIRST_CHARAC_DESCR(characElem->descrList);
    }
    while (characElem != NULL)  
    {
        while (descrElem->next != NULL) 
        {
            /* move to next charac element in list */
             descrElem = descrElem->next;
            *cElem = characElem;
            *dElem = descrElem;
            return TRUE;
        }
        /* no more readable charac elements in this */
        characElem = characElem->next;
        /* reset descrElem - it will get updated as the first thing in the next loop */
        if(characElem != NULL)
        {
            descrElem = CSR_BT_GATT_APP_DB_GET_FIRST_CHARAC_DESCR(characElem->descrList);
        }
    }
    /* could not find any element that mached the property in all of the elements */
    *dElem = NULL;
    *cElem = NULL;
    return FALSE;
}

CsrBtUuid* CsrBtGatt16BitTo128BitUuid(CsrUint16 uuid)
{
    CsrBtUuid* longUuid = (CsrBtUuid *)CsrPmemAlloc(sizeof(CsrBtUuid));
    longUuid->length = 2;
    longUuid->uuid[0] = uuid & 0x00FF;
    longUuid->uuid[1] = (uuid >> 8) & 0x00FF;

    return longUuid;
}

CsrUint16 CsrBtGattDiscoverService(CsrBtGattDbElement *dbElement, void* recvMsgP, CsrBtGattId gattId, CsrBtConnId btConnId)
{
    CsrPrim *primType;

    CsrInt32 retVal = CSR_BT_GATT_PRIM_HANDLED;

    primType = (CsrPrim *)recvMsgP;
    switch (*primType)
    {
        case CSR_BT_GATT_DISCOVER_SERVICES_IND:
            {
                CsrBtGattDiscoverServicesInd *ind;
                ind = (CsrBtGattDiscoverServicesInd*)recvMsgP;
                /*check to see if we already have this prim service*/
                if(!CSR_BT_GATT_APP_DB_FIND_PRIM_SERVICE_BY_UUID(dbElement->dbPrimServices, &ind->uuid))
                {
                    CsrBtGattAppDbPrimServiceElement *elem = CSR_BT_GATT_APP_DB_ADD_PRIM_SERVICE(dbElement->dbPrimServices);
                    elem->uuid        = ind->uuid;
                    elem->startHandle = ind->startHandle;
                    elem->endHandle   = ind->endHandle;
                    elem->id          = dbElement->dbPrimServicesCount++;
                }
                break;
            }
        case CSR_BT_GATT_DISCOVER_SERVICES_CFM:
            {
                CsrBtGattDiscoverServicesCfm *cfm;
                cfm = (CsrBtGattDiscoverServicesCfm*)recvMsgP;

                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    /* Start with first primary service element and find all characteristics */
                    dbElement->currentPrimService = CSR_BT_GATT_APP_DB_GET_FIRST_PRIM_SERVICE(dbElement->dbPrimServices);
                    CsrBtGattDiscoverAllCharacOfAServiceReqSend(gattId,
                                                                btConnId,
                                                                dbElement->currentPrimService->startHandle,
                                                                dbElement->currentPrimService->endHandle);
                    retVal |= CSR_BT_GATT_SERVICES_CFM_SUCCESS;
                }
                else
                {
                    retVal |= CSR_BT_GATT_SERVICES_CFM_ERROR;
                }
                break;
            }
        case CSR_BT_GATT_DISCOVER_CHARAC_IND:
            {
                CsrBtGattDiscoverCharacInd *ind;
                CsrBtGattAppDbCharacElement      *cElem = CSR_BT_GATT_APP_DB_ADD_CHARAC(dbElement->currentPrimService->characList);
                ind = (CsrBtGattDiscoverCharacInd*) recvMsgP;
                cElem->valueHandle       = ind->valueHandle;
                cElem->declarationHandle = ind->declarationHandle;
                cElem->property          = ind->property;
                cElem->uuid              = ind->uuid;
                break;
            }
        case CSR_BT_GATT_DISCOVER_CHARAC_CFM:
            {
                CsrBtGattDiscoverCharacCfm *cfm;
                cfm = (CsrBtGattDiscoverCharacCfm*)recvMsgP;
                if( (cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS && cfm->resultSupplier == CSR_BT_SUPPLIER_GATT) || 
                    /* Returning NOT FOUND if no element found in recived interval, 
                       but should be handles as a cfm success recieved after an indication of found charac */
                    (cfm->resultCode == ATT_RESULT_ATTR_NOT_FOUND && cfm->resultSupplier == CSR_BT_SUPPLIER_ATT))
                {
                    /*move on to next prim service (if any and find all of its charac*/
                    dbElement->currentPrimService = dbElement->currentPrimService->next;
                    if(dbElement->currentPrimService)
                    {
                        CsrBtGattDiscoverAllCharacOfAServiceReqSend(gattId,
                                                                    btConnId,
                                                                    dbElement->currentPrimService->startHandle,
                                                                    dbElement->currentPrimService->endHandle);
                    }
                    else
                    {
                        /* we go back to the beginning and start over */
                        dbElement->currentPrimService = CSR_BT_GATT_APP_DB_GET_FIRST_PRIM_SERVICE(dbElement->dbPrimServices);
                        
                        dbElement->currentCharac  = NULL;
                        
                        if(CsrBtGattAppDbFindNextCharacWithProperty(&dbElement->currentPrimService,
                                                                                       &dbElement->currentCharac,
                                                                                       CSR_BT_GATT_CHARAC_PROPERTIES_READ))
                        {
                            CsrBtGattReadReqSend(gattId, btConnId, dbElement->currentCharac->valueHandle,0);
                        }
                        else
                        {
                            CsrUint16 startHandle = 0;
                            CsrUint16 endHandle   = 0;
                            /* Done Reading!
                            * we need to reset the "current" pointers and start working on the descriptors*/
                            dbElement->currentPrimService  = CSR_BT_GATT_APP_DB_GET_FIRST_PRIM_SERVICE(dbElement->dbPrimServices);
                            /*start from the first charac */
                            dbElement->currentCharac  = NULL;
                            if(CsrBtGattAppDbFindNextCharacWithProperty(&dbElement->currentPrimService, &dbElement->currentCharac, 0/*any property*/))
                            {
                               startHandle = dbElement->currentCharac->declarationHandle + 1;
                               if(dbElement->currentCharac->next)
                               {
                                   /* there are more characs in this prim service so we use the start of the next one as end handle*/
                                   endHandle = ((CsrBtGattAppDbCharacElement*)dbElement->currentCharac->next)->declarationHandle - 1;
                               }
                               else
                               {
                                   /* We are at the last charac in the current prim service so endhandle
                                    * is start of next prim service (NOTE: we expect them to be sorted) */
                                   endHandle = ((CsrBtGattAppDbPrimServiceElement*)dbElement->currentPrimService->next)->startHandle - 1;
                               }
                               CsrBtGattDiscoverAllCharacDescriptorsReqSend(gattId, btConnId, startHandle, endHandle);
                            }
                            else
                            {
                               /* no characs and hence no descriptors
                                 we are done traversing the DB */
                               dbElement->traversingDb = FALSE;

                            }
                        }
                    }
                }
                else
                {
                    retVal |= CSR_BT_GATT_CHARAC_CFM_ERROR;
                }

                break;
            }
        case CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND:
            {
                /* add new element to currentCharac descrList */
                CsrBtGattDiscoverCharacDescriptorsInd *ind;
                CsrUint16 currentUuid;
                dbElement->currentDescr = CSR_BT_GATT_APP_DB_ADD_CHARAC_DESCR(dbElement->currentCharac->descrList);
                
                ind = (CsrBtGattDiscoverCharacDescriptorsInd*) recvMsgP;
                
                dbElement->currentDescr->descriptorHandle                      = ind->descriptorHandle;
                dbElement->currentDescr->uuid                                  = ind->uuid;

                /*FIXME: Find a different order of reading descriptor values 
                         Do it after recieving CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM*/

                currentUuid = dbElement->currentDescr->uuid.uuid[1];
                currentUuid = (currentUuid << 8) + dbElement->currentDescr->uuid.uuid[0];
                if(currentUuid == CSR_BT_GATT_APP_EXTENDED_PROP_DESCR_UUID)
                {
                    CsrBtGattReadExtendedPropertiesReqSend(gattId, btConnId, dbElement->currentDescr->descriptorHandle);
                }
                else if(currentUuid == CSR_BT_GATT_APP_USER_DESCRIPTION_DESCR_UUID)
                {
                    CsrBtGattReadUserDescriptionReqSend(gattId, btConnId, dbElement->currentDescr->descriptorHandle);
                    dbElement->tempCharac = dbElement->currentCharac;
                }
                break;
            }
        case CSR_BT_GATT_READ_EXTENDED_PROPERTIES_CFM:
            {
                CsrBtGattReadExtendedPropertiesCfm *ind;
                
                ind = (CsrBtGattReadExtendedPropertiesCfm*) recvMsgP;
                if(ind->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    dbElement->currentDescr->value = (CsrUint8*)CsrPmemAlloc(sizeof(CsrBtGattExtPropertiesBits));
                    CsrMemCpy(dbElement->currentDescr->value, &ind->extProperties, sizeof(CsrBtGattExtPropertiesBits));
                }
                break;
            }
        case CSR_BT_GATT_READ_USER_DESCRIPTION_CFM:
            {
                CsrBtGattReadUserDescriptionCfm *ind = (CsrBtGattReadUserDescriptionCfm *)recvMsgP;
                if(ind->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    CsrSize strLength = CsrStrLen((CsrCharString*)ind->usrDescription);
                    CsrBtGattAppDbCharacDescrElement *dElem = CSR_BT_GATT_APP_DB_FIND_CHARAC_DESCR_BY_UUID(dbElement->tempCharac->descrList, 
                                                                                                           CsrBtGatt16BitTo128BitUuid(CSR_BT_GATT_APP_USER_DESCRIPTION_DESCR_UUID));
                    dElem->value = (CsrUint8 *)CsrPmemZalloc(strLength + 10);
                    CsrMemCpy(dElem->value, ind->usrDescription, strLength);
                    dElem->valueLength = strLength + 1;
                }
                break;
            }
        case CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM:
            {
                /* Move on to next Charac or if at last one, then move to next Prim Service first charac */
                CsrBtGattDiscoverCharacDescriptorsCfm *cfm;
                CsrUint16 startHandle = 0;
                CsrUint16 endHandle   = 0;
                cfm                   = (CsrBtGattDiscoverCharacDescriptorsCfm*)recvMsgP;

                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS || cfm->resultCode == CSR_BT_GATT_RESULT_INVALID_HANDLE_RANGE )

                {
                    if(CsrBtGattAppDbFindNextCharacWithProperty(&dbElement->currentPrimService, &dbElement->currentCharac, 0))
                    {
                        startHandle  =  dbElement->currentCharac->declarationHandle + 1;
                        if(dbElement->currentCharac->next)
                        {
                            /*still not the last charac in prim service so get endhandle from next charac*/
                            endHandle =  ((CsrBtGattAppDbCharacElement*)dbElement->currentCharac->next)->declarationHandle - 1;
                        }
                        else
                        {              
                            /*get endhandle from prim service */
                            if(dbElement->currentPrimService)
                            {
                                endHandle = dbElement->currentPrimService->endHandle; 
                            }
                            else 
                            {
                                endHandle = 0xFFFF;
                            }
                            /* move to next prim service*/
                        }
                        CsrBtGattDiscoverAllCharacDescriptorsReqSend(gattId, btConnId, startHandle, endHandle);

                    }
                    else 
                    {
                        /*we are done traversing the DB*/
                        dbElement->traversingDb = FALSE;
                    }
                } 
                else 
                {
                    retVal |= CSR_BT_GATT_CHARAC_DESCRIPTOR_CFM_ERROR;
                }
                break;
            }
        case CSR_BT_GATT_READ_CFM:
            {
                CsrUint16 startHandle, endHandle  = 0;
                CsrBtGattReadCfm *cfm;
                cfm = (CsrBtGattReadCfm*)recvMsgP;

                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    /*store the values */
                    dbElement->currentCharac->value       = cfm->value;
                    dbElement->currentCharac->valueLength = cfm->valueLength;
   
                    if(CsrBtGattAppDbFindNextCharacWithProperty(&dbElement->currentPrimService, 
                                                                &dbElement->currentCharac, 
                                                                CSR_BT_GATT_CHARAC_PROPERTIES_READ))
                    {
                        CsrBtGattReadReqSend(gattId, btConnId, dbElement->currentCharac->valueHandle,0);
                    }
                    else 
                    {
                        /* Done Reading!
                         * we need to reset the "current" pointers and start working on the descriptors*/
                        dbElement->currentPrimService  = CSR_BT_GATT_APP_DB_GET_FIRST_PRIM_SERVICE(dbElement->dbPrimServices);
                        /*start from the first charac (we know that there is at least one) */
                        dbElement->currentCharac  = CSR_BT_GATT_APP_DB_GET_FIRST_CHARAC(dbElement->currentPrimService->characList);
                        startHandle = dbElement->currentCharac->declarationHandle + 1;
                        if(dbElement->currentCharac->next)
                        {
                            /* there are more characs in this prim service so we use the start of the next one as end handle*/
                            endHandle = ((CsrBtGattAppDbCharacElement*)dbElement->currentCharac->next)->declarationHandle - 1; 
                        }
                        else
                        {
                            /* We are at the last charac in the current prim service so endhandle
                             * is start of next prim service (NOTE: we expect them to be sorted) */
                            endHandle = ((CsrBtGattAppDbPrimServiceElement*)dbElement->currentPrimService->next)->startHandle - 1;
                        }
                        CsrBtGattDiscoverAllCharacDescriptorsReqSend(gattId, btConnId, startHandle, endHandle);

                    }
                    retVal |= CSR_BT_GATT_READ_CFM_SUCCESS;
                }
                else
                {

                    retVal |= CSR_BT_GATT_READ_CFM_ERROR;
                }
                break;
            }

        default:
            /* If the switch parameter is not recognized. */
            retVal = CSR_BT_GATT_PRIM_NOT_RECOGNIZED;
    }

    return retVal;
}

CsrUtf16String *CsrBt16BitUuidToName(CsrUint16* Uuid)
{
    CsrUtf16String *ret16  =  NULL;

    ret16 = CsrUtf82Utf16String((CsrUtf8String *)CsrBt16BitUuidToCharName(*Uuid));
    return ret16;
}

CsrCharString *CsrBt16BitUuidToCharName(CsrUint16 Uuid)
{
    switch(Uuid)
        {   /* Primary Services */
            case CSR_BT_GATT_APP_GAP_UUID:
                {
                    return "GAP";
                }
            case CSR_BT_GATT_APP_GATT_UUID:
                {
                    return "GATT";
                }
            case CSR_BT_GATT_APP_LINKLOSS_UUID:
                {
                    return "Link Loss";
                }
            case CSR_BT_GATT_APP_IALERT_UUID:
                {
                    return "Immediate Alert";
                }
            case CSR_BT_GATT_APP_TX_POWER_UUID:
                {
                    return "Tx Power";
                }
            case CSR_BT_GATT_APP_CURRENT_TIME_UUID:
                {
                    return "Current Time";
                }
            case CSR_BT_GATT_APP_BATTERY_UUID:
                {
                    return "Battery";
                }
            case CSR_BT_GATT_APP_THERMOMETER_UUID:
                {
                    return "Health Thermometer";
                }
            case CSR_BT_GATT_APP_DEVICE_INFO_UUID:
                {
                    return "Device Information";
                }
                /* Characteristics */
            case CSR_BT_GATT_APP_ALERT_LEVEL_CHARAC_UUID:
                {
                    return "Alert Level";
                    
                }
            case CSR_BT_GATT_APP_APPEARANCE_CHARAC_UUID:
                {
                    return "Appearance";
                    
                }
            case CSR_BT_GATT_APP_DATE_TIME_CHARAC_UUID:
                {
                    return "Date Time";
                    
                }
            case CSR_BT_GATT_APP_DEVICE_NAME_CHARAC_UUID:
                {
                    return "Device Name";
                    
                }
            case CSR_BT_GATT_APP_FIRMWARE_REVISION_CHARAC_UUID:
                {
                    return "Firmware Revision";
                    
                }
            case CSR_BT_GATT_APP_HARDWARE_REVISION_CHARAC_UUID:
                {
                    return "Hardware Revision";
                    
                }
            case CSR_BT_GATT_APP_CERT_DATA_CHARAC_UUID:
                {
                    return "Certification Data";
                    
                }
            case CSR_BT_GATT_APP_INTER_TEMP_CHARAC_UUID:
                {
                    return "Intermediate Temperature";
                    
                }
            case CSR_BT_GATT_APP_MANUFACTURER_CHARAC_UUID:
                {
                    return "Manufacturer Name";
                    
                }
            case CSR_BT_GATT_APP_MEAS_INTERVAL_CHARAC_UUID:
                {
                    return "Measurement Interval";
                    
                }
            case CSR_BT_GATT_APP_MODEL_NUMBER_CHARAC_UUID:
                {
                    return "Model Number";
                    
                }
            case CSR_BT_GATT_APP_PREFER_CONN_CHARAC_UUID:
                {
                    return "Preferred Connection";
                    
                }
            case CSR_BT_GATT_APP_PERIPH_PRIVACY_CHARAC_UUID:
                {
                    return "Peripheral Privacy";
                    
                }
            case CSR_BT_GATT_APP_RECONN_ADDR_CHARAC_UUID:
                {
                    return "Reconnection Address";
                    
                }
            case CSR_BT_GATT_APP_SERIAL_NUMBER_CHARAC_UUID:
                {
                    return "Serial Number";
                    
                }
            case CSR_BT_GATT_APP_SERVICE_CHANGED_CHARAC_UUID:
                {
                    return "Service Changed";
                    
                }
            case CSR_BT_GATT_APP_SOFTWARE_REVISION_CHARAC_UUID:
                {
                    return "Software Revision";
                    
                }
            case CSR_BT_GATT_APP_SYSTEM_ID_CHARAC_UUID:
                {
                    return "System ID";
                    
                }
            case CSR_BT_GATT_APP_TEMP_MEASUREMENT_CHARAC_UUID:
                {
                    return "Temp Measurement"; 
                    
                }
            case CSR_BT_GATT_APP_TEMP_TYPE_CHARAC_UUID:
                {
                    return "Temperature Type";
                    
                }
            case CSR_BT_GATT_APP_TX_POWER_CHARAC_UUID:
                {
                    return "Tx Power Level";
                    
                }
            case CSR_BT_GATT_APP_BATT_MASK_CHARAC_UUID:
                {
                    return "Battery Properties";
                    
                }
            case CSR_BT_GATT_APP_BATT_LEVEL_CHARAC_UUID:
                {
                    return "Battery Level";
                    
                }
            case CSR_BT_GATT_APP_BATT_SERVICE_REQUIRED_CHARAC_UUID:
                {
                    return "Service Required";
                    
                }
               /* Non-adopted Descriptors */
            case CSR_BT_GATT_APP_HID_REPORT_DESCRIPTOR_CHARAC_UUID:
                {
                    return "HID Report";
                    
                }
            case CSR_BT_GATT_APP_HID_CONTROL_REPORT_CHARAC_UUID:
                {
                    return "HID Control";
                    
                }
            case CSR_BT_GATT_APP_HID_INTERUPT_REPORT_CHARAC_UUID:
                {
                    return "HID Interrupt";
                    
                } /* Descriptors */
            case CSR_BT_GATT_APP_AGGREGATE_FORMAT_DESCR_UUID:
                {
                    return "Aggregate Format";
                    
                }
            case CSR_BT_GATT_APP_EXTENDED_PROP_DESCR_UUID:
                {
                    return "Ext. Properties";
                    
                }
            case CSR_BT_GATT_APP_PRESENTATION_FORMAT_DESCR_UUID:
                {
                    return "Presentation Format";
                    
                }
            case CSR_BT_GATT_APP_USER_DESCRIPTION_DESCR_UUID:
                {
                    return "User Description";
                    
                }
            case CSR_BT_GATT_APP_SERVER_CONFIG_DESCR_UUID:
                {
                    return "Server Config";
                    
                }
            case CSR_BT_GATT_CLIENT_CHARACTERISTIC_CONFIGURATION:
                {
                    return "Client Config";
                    
                }
            default:
                {
                    return "Unknown UUID";
                    
                }
        }
}


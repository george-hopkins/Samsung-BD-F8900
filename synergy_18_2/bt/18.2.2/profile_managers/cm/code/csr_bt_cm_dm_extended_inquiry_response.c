/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_cm_events_handler.h"
#include "csr_bt_cm_util.h"

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1 /* FIXME: Remove this to the makefile */

#define EIR_SERVICE_CHUNK_SIZE (64) /* Minimum size of an entry in 'services' is 2+4+1+2 octets */

/* ACL packet sizes */
#define PACKET_SIZE_DM1         (17)
#define PACKET_SIZE_DH1         (27)
#define PACKET_SIZE_DM3         (121)
#define PACKET_SIZE_DH3         (183)
#define PACKET_SIZE_DM5         (224)

/* Defines for use with localEirData->services */
#define EIR_SR_UUID_SIZE        (1)
#define EIR_SR_SIZE_FIELD       (2)
#define EIR_SR_HANDLE_FIELD     (4)
#define EIR_SR_HEADER_SIZE      (6)

/* Prioritise packet size over FEC - but still use FEC whenever possible */
#define USE_FEC(size)   (CsrUint8)(size > 0 ? (((size) <= PACKET_SIZE_DM1) || ((size) > PACKET_SIZE_DH1 && (size) <= PACKET_SIZE_DM3) || \
                                               ((size) > PACKET_SIZE_DH3 && (size) <= PACKET_SIZE_DM5)) : 0)

#define EIR_GET_SERVICE_SIZE(octetsRequired) (((octetsRequired) / EIR_SERVICE_CHUNK_SIZE + ((octetsRequired) % \
                                                                                            EIR_SERVICE_CHUNK_SIZE ? 1 : 0)) * EIR_SERVICE_CHUNK_SIZE)
/*
*************************************** localEirData->services format ***************************************

Description:
Used for holding all service record handles and related UUIDs. Will be updated each time a successful
SDS_REGISTER_CFM or SDS_UNREGISTER_CFM is received by the CM.

|<-EIR_SR_SIZE_FIELD->|<-EIR_SR_HANDLE_FIELD->|                    |<-- service_size -->|
|=====================|=======================|====================|====================|====================|====================||============
|---- SR_size (2) ----|---- SR_handle (4) ----|- service_size (1) -|- service (2/4/16) -|- service_size (1) -|- service (2/4/16) -||- next SR...
|=====================|=======================|====================|====================|====================|====================||============
|<---------------------------------------------------------------- SR_size ------------------------------------------------------>|
|<----------- EIR_SR_HEADER_SIZE ------------>|<-EIR_SR_UUID_SIZE->|

A variable number of (at least one) services ('service_size' + 'service') can be available for each service record entry.

Example with two service record entries. One with a UUID16 and a UUID32 and one with two UUID16:
|======|==========|====|======|====|==========||======|==========|====|======|====|======|
|0x000E|0x89ABCDEF|0x02|0x1011|0x04|0x01234567||0x000C|0x12345678|0x02|0x1234|0x02|0x2345|
|======|==========|====|======|====|==========||======|==========|====|======|====|======|

*************************************** localEirData->requestedServices format ***************************************

Description:
Used for temporarily storing service UUIDs between a service registration request and a successful confirm.
localEirData->requestedServices will be NULL in all other states/situations. Services are extracted by
cmEirExtractServices when a CSR_BT_CM_SDS_REGISTER_REQ is received and inserted into localEirData->services by
cmEirAddServiceRecord when a successful CSR_BT_CM_SDS_REGISTER_CFM is received.

|<-EIR_SR_UUID_SIZE->|<-- service_size -->|
|====================|====================|====================|====================||==================|
|- service_size (1) -|- service (2/4/16) -|- service_size (1) -|- service (2/4/16) -||- next services...|
|====================|====================|====================|====================||==================|
|<--------------------------------- localEirData->requestedServicesLength -------------------------------->|

************************************************ EIR-tag format ******************************************

Description:
The EIR-message format as defined by the specification. The EIR-message is created by csrBtCmEirBuildMessage
and csrBtCmEirInsertServices.

|<-- CSR_BT_EIR_TAG_HEADER_SIZE -->|
|<------------------ a complete tag ------------->|
|==============|============|=====================||=============
|- length (1) -|- type (1) -|- data (length - 1) -||- next tag...
|==============|============|=====================||=============
|<---------- length -------------->|

'data' contains one or more elements of the type defined by the preceding 'type' field.
*/

#ifdef CSR_BT_INSTALL_CM_EIR_FLAGS
static void csrBtCmEirFlagsCfmSend(CsrSchedQid appHandle,
                                   CsrBtResultCode resultCode,
                                   CsrBtSupplier resultSupplier)
{
    CsrBtCmEirFlagsCfm *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtCmEirFlagsCfm));
    prim->type = CSR_BT_CM_EIR_FLAGS_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtCmPutMessage(appHandle, prim);
}
#endif

/* Static functions related to EIR */
static void csrBtCmEirInsertServices(cmInstanceData_t *cmData, CsrUint8 *eirMessage,
                                     CsrUint8 *octetsAvailable, CsrUint8 *octetsUsed)
{
    localEirData_t *localEirData;
    CsrUint8 *srStart;
    CsrUint8 *eirTagPtr;
    CsrIntFast16 eirUuidLoop, eirCurrentTagSize;
    CsrIntFast16 srLoop, srUuidLoop, eirUuidSizeLoop;
    CsrBool uuidFound, tagComplete;

    localEirData = cmData->dmVar.localEirData;
    srStart = localEirData->services;
    eirTagPtr = eirMessage + *octetsUsed;
    tagComplete = TRUE;

    /* Loop through the different sizes of UUIDs */
    for (eirUuidSizeLoop = CSR_BT_EIR_UUID16_SIZE;
         (tagComplete
          && (*octetsAvailable >= eirUuidSizeLoop + CSR_BT_EIR_TAG_HEADER_SIZE)
          && (eirUuidSizeLoop <= CSR_BT_EIR_UUID128_SIZE));
         eirUuidSizeLoop *= eirUuidSizeLoop) /* 2->4->16 (UUID16->UUID32->UUID128) */
    {
        eirCurrentTagSize = 0;

        /* Loop through service record entries - increment srLoop by the value of two octets */
        for (srLoop = 0;
             tagComplete && srLoop < localEirData->servicesOctetsUsed;
             srLoop += CSR_BT_EIR_GET_UINT16_FROM_PTR(srStart + srLoop))
        {
            /* Loop through UUIDs in a service record entry */
            for (srUuidLoop = EIR_SR_HEADER_SIZE;
                 tagComplete && srUuidLoop < CSR_BT_EIR_GET_UINT16_FROM_PTR(srStart + srLoop);
                 srUuidLoop += *(srStart + srLoop + srUuidLoop) + 1)
            {
                /* Compare UUID type */
                if (eirUuidSizeLoop == *(srStart + srLoop + srUuidLoop))
                {
                    uuidFound = FALSE;

                    /* UUID type matches - check if specific UUID is already inserted in EIR-message */
                    for (eirUuidLoop = 0; eirUuidLoop < eirCurrentTagSize; eirUuidLoop += eirUuidSizeLoop)
                    {
                        if (!CsrMemCmp(eirTagPtr + CSR_BT_EIR_TAG_HEADER_SIZE + eirUuidLoop,
                                       srStart + srLoop + srUuidLoop + EIR_SR_UUID_SIZE,
                                       eirUuidSizeLoop))
                        {
                            /* UUID found - set flag and break loop */
                            uuidFound = TRUE;
                            break;
                        }
                    }

                    /* Insert if new */
                    if (!uuidFound)
                    {
                        /* Is space available in EIR-message? */
                        if (*octetsAvailable >= (eirUuidSizeLoop + CSR_BT_EIR_TAG_HEADER_SIZE))
                        {
                            /* Insert UUID in EIR-message */
                            CsrMemCpy(eirTagPtr + CSR_BT_EIR_TAG_HEADER_SIZE + eirCurrentTagSize,
                                      srStart + srLoop + srUuidLoop + EIR_SR_UUID_SIZE,
                                      eirUuidSizeLoop);

                            /* Update counters */
                            eirCurrentTagSize += eirUuidSizeLoop;
                            *octetsAvailable  -= (CsrUint8)eirUuidSizeLoop;
                        }
                        else
                        {
                            /* EIR-message is full */
                            tagComplete = FALSE;
                        }
                    }
                }
            } /* Skip to next UUID in service record entry */
        } /* Skip to next service record entry */

        /* Write EIR tag header and counters */
        if (eirCurrentTagSize > 0)
        {
            /* Update header of EIR tag */
            *eirTagPtr = eirCurrentTagSize + 1; /* 'length' field */
            *(eirTagPtr + 1) = CSR_BT_EIR_CONV_UUID_SIZE_TO_TYPE(eirUuidSizeLoop, tagComplete); /* 'type' field */

            /* Update pointers and counters */
            eirTagPtr += eirCurrentTagSize + CSR_BT_EIR_TAG_HEADER_SIZE; /* Point to beginning of next tag */
            *octetsUsed += eirCurrentTagSize + CSR_BT_EIR_TAG_HEADER_SIZE;
            *octetsAvailable -= CSR_BT_EIR_TAG_HEADER_SIZE;
        }
    } /* Skip to next UUID size */
}

static void csrBtCmEirInsertTag(CsrUint8 ** const eirMessage,
                                CsrUint8 * const octetsUsed,
                                CsrUint8 * const octetsAvailable,
                                CsrUint8 const tagType,
                                CsrUint8 const dataLength,
                                CsrUint8 const * const data)
{
    CsrUint8 *eirPtr = *eirMessage + *octetsUsed;

    if (*octetsAvailable >= (CSR_BT_EIR_TAG_HEADER_SIZE + dataLength))
    {
        /* Insert tag header */
        *eirPtr         = dataLength + 1;
        *(eirPtr + 1)   = tagType;

        /* Insert data */
        CsrMemCpy(eirPtr + CSR_BT_EIR_TAG_HEADER_SIZE, data, dataLength);

        /* Update counters */
        *octetsAvailable    -= (dataLength + CSR_BT_EIR_TAG_HEADER_SIZE);
        *octetsUsed         += (dataLength + CSR_BT_EIR_TAG_HEADER_SIZE);
    }
    else
    {
        /* No space available in EIR-message */
    }

}

static void csrBtCmEirBuildMessage(cmInstanceData_t *cmData)
{
    localEirData_t  *localEirData = cmData->dmVar.localEirData;
    CsrUint8         *eirMessage;
    CsrUint8         octetsUsed = 0;
    CsrUint8         octetsAvailable = CSR_BT_EIR_DATA_MAX_SIZE;
    CsrUint8         octetsToCopy;
    CsrUint16        nameLength;
    CsrUint8 flags = 0;

    eirMessage = CsrPmemAlloc(CSR_BT_EIR_DATA_MAX_SIZE);

    /* Set BT4 LE controller flag */
#ifdef CSR_BT_LE_ENABLE
    if (cmData->dmVar.lmpVersion >= CSR_BT_BLUETOOTH_VERSION_4P0)
    {
        flags |= CSR_BT_EIR_FLAG_SIMUL_BREDR_LE_CONTROLLER;
    }
#endif
    
    /* Get discoverable mode flag from app */
    if(cmData->dmVar.localEirData != NULL)
    {
        flags |= (cmData->dmVar.localEirData->flags &
                  (CSR_BT_EIR_FLAG_LE_LIMITED_DISCOVERABLE |
                   CSR_BT_EIR_FLAG_LE_GENERAL_DISCOVERABLE));
    }
    
    if (flags != 0)
    {
        csrBtCmEirInsertTag(&eirMessage, &octetsUsed, &octetsAvailable,
                            CSR_BT_EIR_DATA_TYPE_FLAGS,
                            CSR_BT_EIR_FLAG_DATA_LEN, &flags);
    }

    /* Insert manufacturer specific EIR data if high priority - only look at two LSBs */
    if (((localEirData->manufacturerDataSettings & 0x0003) == CSR_BT_EIR_MANUFACTURER_PRIORITY_HIGH) &&
        (localEirData->manufacturerData != NULL))
    {
        csrBtCmEirInsertTag(&eirMessage, &octetsUsed, &octetsAvailable,
                            CSR_BT_EIR_DATA_TYPE_MANUFACTURER_SPECIFIC,
                            localEirData->manufacturerDataLength,
                            localEirData->manufacturerData);
    }

    /* Insert services - separate function to minimise complexity */
    csrBtCmEirInsertServices(cmData, eirMessage, &octetsAvailable, &octetsUsed);

    /* Insert name */
    if (octetsAvailable > CSR_BT_EIR_TAG_HEADER_SIZE)
    {
        nameLength = (CsrUint16)CsrStrLen((char *) localEirData->currentName);

        if (octetsAvailable >= (nameLength + CSR_BT_EIR_TAG_HEADER_SIZE))
        {
            /* Space is available for entire name and header */
            csrBtCmEirInsertTag(&eirMessage, &octetsUsed, &octetsAvailable,
                                CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME,
                                (CsrUint8)nameLength, localEirData->currentName);
        }
        else
        {
            /* Space is not available for the entire name and header */
            octetsToCopy = octetsAvailable - CSR_BT_EIR_TAG_HEADER_SIZE;
            csrBtCmEirInsertTag(&eirMessage, &octetsUsed, &octetsAvailable,
                                CSR_BT_EIR_DATA_TYPE_SHORT_LOCAL_NAME,
                                octetsToCopy, localEirData->currentName);
        }
    }

    /* Insert manufacturer specific EIR data if low priority */
    if (((localEirData->manufacturerDataSettings & 0x03) == CSR_BT_EIR_MANUFACTURER_PRIORITY_LOW) &&
        (localEirData->manufacturerData != NULL )&&
        (octetsAvailable > CSR_BT_EIR_TAG_HEADER_SIZE))
    {
        octetsToCopy = (localEirData->manufacturerDataLength + CSR_BT_EIR_TAG_HEADER_SIZE) <= octetsAvailable ?
            localEirData->manufacturerDataLength : octetsAvailable - CSR_BT_EIR_TAG_HEADER_SIZE;

        csrBtCmEirInsertTag(&eirMessage, &octetsUsed, &octetsAvailable, CSR_BT_EIR_DATA_TYPE_MANUFACTURER_SPECIFIC,
                            octetsToCopy, localEirData->manufacturerData);
    }

    /* Write EIR message */
    dm_hci_write_extended_inquiry_response_data(USE_FEC(octetsUsed), octetsUsed, eirMessage, NULL);
    CsrPmemFree(eirMessage); /* Message has been sent - free the temporary EIR-data */
}

static CsrUint8 * csrBtCmEirFindSrHandle(cmInstanceData_t *cmData, CsrUint32 serviceRecordHandle)
{
    localEirData_t *localEirData = cmData->dmVar.localEirData;

    if (localEirData)
    {
        CsrUint8 *srPtr = localEirData->services;

        for (srPtr = localEirData->services; srPtr < localEirData->services + localEirData->servicesOctetsUsed;
             srPtr += CSR_BT_EIR_GET_UINT16_FROM_PTR(srPtr))
        {
            if (serviceRecordHandle == CSR_BT_EIR_GET_UINT32_FROM_PTR(srPtr + EIR_SR_SIZE_FIELD))
            {
                /* Service record handle was found */
                return srPtr;
            }
        }
    }

    /* Service record handle was not found */
    return NULL;
}

static CsrBool csrBtCmEirCheckIfUuidExists(cmInstanceData_t *cmData, CsrUint8 uuidSize, CsrUint8 *uuid)
{
    localEirData_t  *localEirData = cmData->dmVar.localEirData;
    CsrIntFast16    srLoop;     /* Loop through the service records */
    CsrIntFast16    srUuidLoop; /* Loop through the UUIDs in a service record */
    CsrUint8         srUuidSize; /* Size in octets of the current UUID */
    CsrUint8         *services = localEirData->services;

    /* Skip through UUIDs in old service record entries */
    for (srLoop = 0; srLoop < localEirData->servicesOctetsUsed; srLoop += CSR_BT_EIR_GET_UINT16_FROM_PTR(services + srLoop))
    {
        for (srUuidLoop = EIR_SR_HEADER_SIZE; srUuidLoop < CSR_BT_EIR_GET_UINT16_FROM_PTR(services + srLoop); srUuidLoop +=  srUuidSize + 1)
        {
            srUuidSize = *(services + srLoop + srUuidLoop);

            if (uuidSize == srUuidSize)
            {
                /* UUID size matches - compare UUID */
                if (!CsrMemCmp(uuid, services + srLoop + srUuidLoop, uuidSize))
                {
                    /* Matching service found */
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

/* Public functions */
void CsrBtCmEirInitData(cmInstanceData_t *cmData)
{
    if (!cmData->dmVar.localEirData)
    {
        localEirData_t *localEirData;

        localEirData = CsrPmemAlloc(sizeof(localEirData_t));

        localEirData->currentName               = NULL;
        localEirData->requestedName             = NULL;
        localEirData->manufacturerDataSettings  = CSR_BT_EIR_MANUFACTURER_NOT_AVAILABLE;
        localEirData->manufacturerDataLength    = 0;
        localEirData->manufacturerData          = NULL;
        localEirData->servicesOctetsUsed        = 0;
        localEirData->services                  = NULL;
        localEirData->requestedServicesLength   = 0;
        localEirData->requestedServices         = NULL;
        localEirData->requestedServicesNew      = FALSE;
        localEirData->flags                     = 0;

        cmData->dmVar.localEirData = localEirData;
    }
}

void CsrBtCmEirExtractServicesFromRecord(cmInstanceData_t *cmData, CsrUint16 serviceRecordLen, CsrUint8 *serviceRecord)
{
    localEirData_t *localEirData = cmData->dmVar.localEirData;

    if (localEirData && serviceRecord)
    {
        CsrUint8     type, uuid32Converted = 0;
        CsrUint32    index, size;

        /* Find the Service Class ID List in the service record */
        if (CsrBtSdcGetAttribute(serviceRecordLen, serviceRecord, CSR_BT_SERVICE_CLASS_ID_LIST, &type, &size, &index))
        {
            CsrUint32 listLength = size, readBytes = 0, read = 0;
            CsrUint16 requestedServiceIndex = 0;

            /* Allocate enough memory to hold all UUIDs */
            localEirData->requestedServices = CsrPmemAlloc(size);

            while (read < listLength)
            {
                if (CsrBtSdcGetTypeDescriptorInsideList(serviceRecordLen, serviceRecord, index, &type, &size, &readBytes))
                {
                    if (type != CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
                    {
                        readBytes += size;
                    }

                    if (type == CSR_BT_DATA_ELEMENT_UUID_TYPE)
                    {
                        CsrUintFast8 i;

                        if (size == CSR_BT_EIR_UUID32_SIZE && CSR_BT_EIR_GET_UINT16_FROM_PTR(serviceRecord + index + 1) == 0)
                        {
                            /* UUID32 can be converted to a UUID16 - skip the first two octets */
                            size            = CSR_BT_EIR_UUID16_SIZE;
                            uuid32Converted = CSR_BT_EIR_UUID16_SIZE;
                        }
                        else
                        {
                            uuid32Converted = 0;
                        }

                        /* Check if UUID is already known - EIR will only be updated if new UUIDs are registered */
                        if (!localEirData->requestedServicesNew && !csrBtCmEirCheckIfUuidExists(cmData, (CsrUint8)size, serviceRecord + readBytes))
                        {
                            localEirData->requestedServicesNew = TRUE;
                        }

                        /* Copy UUID to requestedServices */
                        *(localEirData->requestedServices + requestedServiceIndex) = (CsrUint8)size; /* Set type field */
                        requestedServiceIndex++;

                        for (i = 0; i < size; i++)
                        {
                            /* Little endianess is required */
                            *(localEirData->requestedServices + requestedServiceIndex + i) = *(serviceRecord + index + uuid32Converted + (size - i));
                        }
                        requestedServiceIndex += (CsrUint8)size;
                    }
                    index   += readBytes;
                    read    += readBytes;
                }
            }

            localEirData->requestedServicesLength = requestedServiceIndex;
        }
    }
}

void CsrBtCmEirAddServiceRecord(cmInstanceData_t *cmData, CsrUint32 serviceRecordHandle)
{
    localEirData_t  *localEirData = cmData->dmVar.localEirData;
    CsrUint16        octetsUsed, serviceRecordSize;
    CsrUint8         octetsAvailableInChunk;

    if (localEirData->requestedServices)
    {
        octetsUsed              = localEirData->servicesOctetsUsed;
        octetsAvailableInChunk = octetsUsed % EIR_SERVICE_CHUNK_SIZE > 0 ? (EIR_SERVICE_CHUNK_SIZE - octetsUsed % EIR_SERVICE_CHUNK_SIZE) : 0;
        serviceRecordSize       = localEirData->requestedServicesLength + EIR_SR_HEADER_SIZE;

        /* Check if more space must be allocated */
        if (octetsAvailableInChunk < serviceRecordSize)
        {
            /* Chunk is full - allocate more data */
            CsrUint8 *localServices;

            localServices = CsrPmemAlloc(EIR_GET_SERVICE_SIZE(octetsUsed + serviceRecordSize));

            if (localEirData->services)
            {
                /* Copy existing entries */
                CsrMemCpy(localServices, localEirData->services, octetsUsed);
                CsrPmemFree(localEirData->services);
            }

            localEirData->services = localServices;
        }

        /* Add service */
        CSR_BT_EIR_COPY_UINT16_TO_PTR(serviceRecordSize, localEirData->services + octetsUsed);
        octetsUsed += EIR_SR_SIZE_FIELD;

        /* Insert service record handle */
        CSR_BT_EIR_COPY_UINT32_TO_PTR(serviceRecordHandle, localEirData->services + octetsUsed);
        octetsUsed += EIR_SR_HANDLE_FIELD;

        /* Copy requested services */
        CsrMemCpy(localEirData->services + octetsUsed, localEirData->requestedServices, localEirData->requestedServicesLength);

        localEirData->servicesOctetsUsed += serviceRecordSize;

        /* Free temporary storage for UUIDs */
        CsrPmemFree(localEirData->requestedServices);
        localEirData->requestedServices        = NULL;
        localEirData->requestedServicesLength  = 0;

        /* Build EIR-message */
        if (localEirData->requestedServicesNew)
        {
            /* Send internal message to the CM to acquire the DM-queue */
            CsrBtCmEirUpdateManufacturerDataReqSend(CSR_BT_CM_IFACEQUEUE, CSR_BT_EIR_MANUFACTURER_NOT_AVAILABLE, 0, NULL);
            localEirData->requestedServicesNew = FALSE;
        }
    }
}

void CsrBtCmEirRemoveServiceRecord(cmInstanceData_t *cmData, CsrUint32 serviceRecordHandle)
{
    localEirData_t  *localEirData = cmData->dmVar.localEirData;

    if (localEirData)
    {
        CsrUint16    octetsToRemove;
        CsrUint8     *moveFrom, octetsUsedInLastChunk, remainder;

        if ((moveFrom = csrBtCmEirFindSrHandle(cmData, serviceRecordHandle)) != NULL)
        {
            /* Service record entry was found */
            octetsToRemove = CSR_BT_EIR_GET_UINT16_FROM_PTR(moveFrom); /* Point to 'size' field */

            if (octetsToRemove >= localEirData->servicesOctetsUsed)
            {
                /* The entry to remove is the only one available */
                CsrPmemFree(localEirData->services);
                localEirData->services              = NULL;
                localEirData->servicesOctetsUsed    = 0;
            }
            else
            {
                /* More service record entries were available */
                remainder = localEirData->servicesOctetsUsed % EIR_SERVICE_CHUNK_SIZE;
                octetsUsedInLastChunk = remainder > 0 ? remainder : EIR_SERVICE_CHUNK_SIZE;

                /* Determine if one or more chunks can be freed */
                if (octetsUsedInLastChunk <= octetsToRemove)
                {
                    CsrUint16    preBlockSize, postBlockSize;
                    CsrUint8     *localServices;

                    /* Determine sizes of blocks before and after the block to remove */
                    preBlockSize    = (CsrUint16) (moveFrom - localEirData->services);
                    postBlockSize   = (CsrUint16) ((localEirData->services + localEirData->servicesOctetsUsed) - (moveFrom + octetsToRemove));

                    /* A chunk can be freed - reallocate */
                    localServices = CsrPmemAlloc(EIR_GET_SERVICE_SIZE(localEirData->servicesOctetsUsed - octetsToRemove));

                    /* Copy remaining services to newly allocated memory */
                    CsrMemCpy(localServices, localEirData->services, preBlockSize);
                    CsrMemCpy(localServices + preBlockSize, moveFrom + octetsToRemove, postBlockSize);

                    /* Relink service data*/
                    CsrPmemFree(localEirData->services);
                    localEirData->services = localServices;
                }
                else
                {
                    CsrUintFast16 i;
                    CsrUint16 octetsToCopy = (CsrUint16) (localEirData->servicesOctetsUsed - octetsToRemove - (moveFrom - localEirData->services));

                    /* A chunk can not be freed - just move the remaining data - CsrMemCpy can not be used for overlapping src and dst */
                    for (i = 0; i < octetsToCopy; i++)
                    {
                        *(moveFrom + i) = *(moveFrom + octetsToRemove + i);
                    }
                }

                localEirData->servicesOctetsUsed -= octetsToRemove;
            }

            /* Send internal message to the CM to acquire the DM-queue */
            CsrBtCmEirUpdateManufacturerDataReqSend(CSR_BT_CM_IFACEQUEUE, CSR_BT_EIR_MANUFACTURER_NOT_AVAILABLE, 0, NULL);
        }
        else
        {
            CsrBtCmGeneralWarning(CSR_BT_CM_PRIM, 0, "Service record not found");
        }
    }
}

void CsrBtCmEirUpdateName(cmInstanceData_t *cmData)
{
    localEirData_t *localEirData = cmData->dmVar.localEirData;

    if (localEirData)
    {
        /* The local name was successfully updated - copy requested name to current name */
        CsrPmemFree(localEirData->currentName);
        localEirData->currentName = localEirData->requestedName;
        localEirData->requestedName = NULL;

        /* Make sure a CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_CFM is not sent to the app */
        cmData->dmVar.appHandle = CSR_BT_CM_IFACEQUEUE;

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOCAL_NAME_CHANGE
        /* Propagate name change to subscribers */
        CsrBtCmPropgateEvent(cmData,
                             CsrBtCmPropagateLocalNameChangeEvent,
                             CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOCAL_NAME_CHANGE,
                             HCI_SUCCESS,
                             localEirData->currentName, /* not modified */
                             NULL);
#endif

        /* Build and send EIR */
        csrBtCmEirBuildMessage(cmData);
    }
}

void CsrBtCmEirUpdateManufacturerReqHandler(cmInstanceData_t *cmData)
{
    localEirData_t *localEirData;
    CsrBtCmEirUpdateManufacturerDataReq *cmPrim;

    localEirData = cmData->dmVar.localEirData;
    cmPrim = (CsrBtCmEirUpdateManufacturerDataReq *)cmData->recvMsgP;

    cmData->dmVar.appHandle = cmPrim->appHandle;

    if (cmPrim->appHandle == CSR_BT_CM_IFACEQUEUE)
    {
        /* DM-queue acquired - build and write EIR */
        csrBtCmEirBuildMessage(cmData);
    }
    else if (localEirData)
    {
        /* 2.1 is supported - verify if format of message is ok */
        if (((cmPrim->manufacturerDataLength <= CSR_BT_EIR_MANUFACTURER_DATA_MAX_SIZE)) &&
            (((cmPrim->manufacturerDataSettings & 0x03) == CSR_BT_EIR_MANUFACTURER_PRIORITY_LOW) ||
             ((cmPrim->manufacturerDataSettings & 0x03) == CSR_BT_EIR_MANUFACTURER_PRIORITY_HIGH) ||
             ((cmPrim->manufacturerDataSettings & 0x03) == CSR_BT_EIR_MANUFACTURER_NOT_AVAILABLE)))
        {
            if (localEirData->manufacturerData)
            {
                /* Free previously allocated manufacturer specific data */
                CsrPmemFree(localEirData->manufacturerData);
                localEirData->manufacturerData = NULL;
            }

            /* Parameters are valid */
            localEirData->manufacturerDataLength    = cmPrim->manufacturerDataLength;
            localEirData->manufacturerData          = cmPrim->manufacturerData;
            localEirData->manufacturerDataSettings  = cmPrim->manufacturerDataSettings;
            cmPrim->manufacturerData = NULL;
            csrBtCmEirBuildMessage(cmData);
        }
        else
        {
            /* Parameters are invalid */
            CsrBtCmEirSendUpdateManufacturerCfm(cmPrim->appHandle, CSR_BT_RESULT_CODE_CM_UNACCEPTABLE_PARAMETER, CSR_BT_SUPPLIER_CM);
            CsrBtCmDmLocalQueueHandler();
        }
    }
    else
    {
        CsrBtCmEirSendUpdateManufacturerCfm(cmPrim->appHandle, CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE, CSR_BT_SUPPLIER_CM);
        CsrBtCmDmLocalQueueHandler();
    }

    CsrPmemFree(cmPrim->manufacturerData);
}

void CsrBtCmDmHciWriteExtendedInquiryResponseDataCompleteHandler(cmInstanceData_t *cmData)
{
    if (cmData->dmVar.appHandle != CSR_BT_CM_IFACEQUEUE)
    {
        DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T  *dmPrim;
        dmPrim = (DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T *)cmData->recvMsgP;

        if (dmPrim->status == HCI_SUCCESS)
        {
            CsrBtCmEirSendUpdateManufacturerCfm(cmData->dmVar.appHandle, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
        }
        else
        {
            CsrBtCmEirSendUpdateManufacturerCfm(cmData->dmVar.appHandle, (CsrBtResultCode) dmPrim->status, CSR_BT_SUPPLIER_HCI);
        }
    }

    if (cmData->globalState == notReady_s)
    {
        /* We are currently in the CM initialization process, we must
         * continue setting up the chip */
        dm_hci_write_inquiryscan_activity(cmData->dmVar.inquiryscanInterval,
                                          cmData->dmVar.inquiryscanWindow,
                                          NULL);
    }
    else
    {
        CsrBtCmDmLocalQueueHandler();
    }
}

#ifdef CSR_BT_INSTALL_CM_EIR_FLAGS
void CsrBtCmEirFlagsReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmEirFlagsReq *prim;
    prim = (CsrBtCmEirFlagsReq*)cmData->recvMsgP;

    if(cmData->dmVar.localEirData != NULL)
    {
        cmData->dmVar.localEirData->flags = prim->eirFlags;
        csrBtCmEirFlagsCfmSend(prim->appHandle,
                               CSR_BT_RESULT_CODE_CM_SUCCESS,
                               CSR_BT_SUPPLIER_CM);
    }
    else
    {
        csrBtCmEirFlagsCfmSend(prim->appHandle,
                               CSR_BT_RESULT_CODE_CM_COMMAND_DISALLOWED,
                               CSR_BT_SUPPLIER_CM);
    }
}
#endif
#else /* CSR_BT_BT_VERSION < BT2.1 */

void CsrBtCmEirUpdateManufacturerReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmEirUpdateManufacturerDataReq   *cmPrim;
    cmPrim = (CsrBtCmEirUpdateManufacturerDataReq *)cmData->recvMsgP;

    CsrBtCmEirSendUpdateManufacturerCfm(cmPrim->appHandle, CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE, CSR_BT_SUPPLIER_CM);
    CsrBtCmDmLocalQueueHandler();

    CsrPmemFree(cmPrim->manufacturerData);
}

#ifdef CSR_BT_INSTALL_CM_EIR_FLAGS
void CsrBtCmEirFlagsReqHandler(cmInstanceData_t *cmData)
{
    csrBtCmEirFlagsCfmSend(prim->appHandle,
                           CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE,
                           CSR_BT_SUPPLIER_CM);
}
#endif
#endif


void CsrBtCmEirSendUpdateManufacturerCfm(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtCmEirUpdateManufacturerDataCfm *cmPrim;

    cmPrim                  = CsrPmemAlloc(sizeof(CsrBtCmEirUpdateManufacturerDataCfm));
    cmPrim->type            = CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_CFM;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    CsrBtCmPutMessage(appHandle, cmPrim);
}

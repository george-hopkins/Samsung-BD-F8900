/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_bsl_main.h"
#include "csr_bt_bsl_util.h"
#include "csr_bt_tasks.h"

/* must use index values as defined in BslSdcAttrSearchIndex_t */
static const CsrUint16 sdcSearchAttrList[] =
{
    CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER,
    CSR_BT_LANGUAGE_BASE_ATTRIBUTE_ID_LIST_ATTRIBUTE_IDENTIFIER,
    CSR_BT_SERVICE_AVAILABILITY_ATTRIBUTE_IDENTIFIER,
    CSR_BT_BLUETOOTH_PROFILE_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER,
    CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER,
    CSR_BT_SERVICE_DESCRIPTION_ATTRIBUTE_IDENTIFIER,
    CSR_BT_SECURITY_DESCRIPTION_ATTRIBUTE_IDENTIFIER,
    CSR_BT_NET_ACCESS_TYPE_ATTRIBUTE_IDENTIFIER,
    CSR_BT_MAX_NET_ACCESS_RATE_ATTRIBUTE_IDENTIFIER,
    CSR_BT_IPV4_SUBNET_ATTRIBUTE_IDENTIFIER,
    CSR_BT_IPV6_SUBNET_ATTRIBUTE_IDENTIFIER,
};

static const CsrUint8 sdsPanPanuServiceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,       /* AttrID , ServiceClassIDList */
    0x35,0x03,            /* DataElSeq 3 bytes */
    /* UUID for PANU */
    0x19,0x11,0x15,       /* 2 byte UUID, Service class = PANU = 0x1115 */

    /* protocol descriptor list */
    0x09,0x00,0x04,       /* AttrId ProtocolDescriptorList */
    0x35,0x1B,            /* DataElSeq 27 bytes */

    /* L2CAP */
    0x35,0x06,            /* 6 bytes in DataElSeq */
    0x19,0x01,0x00,       /* 2 byte UUID, Protocol = L2CAP */
    0x09,0x00,0x0F,
    0x35,0x11,            /* DataElSeq 17 bytes */
    0x19,0x00,0x0F,       /* uuid16_t BNEP(0x000f) */
    0x09,0x01,0x00,       /* CsrUint16 0x0100 */
    0x35,0x09,            /* DataElSeq 9 bytes
                             This is a list of supported network packet types */
    0x09,0x08,0x00,       /* CsrUint16 0x0800 */
    0x09,0x08,0x06,       /* CsrUint16 0x0806 */
    0x09,0x86,0xdd,       /* CsrUint16 0x86dd */
    0x09,0x00,0x06,       /* LanguageBaseAttributeIDList(0x0006) */
    0x35,0x09,            /* DataElSeq 9 bytes */
    0x09,0x65,0x6e,       /* CsrUint16 0x656e */
    0x09,0x00,0x6a,       /* CsrUint16 0x006a */
    0x09,0x01,0x00,       /* CsrUint16 0x0100 */
    0x09,0x00,0x09,       /* BluetoothProfileDescriptorList(0x0009) */
    0x35,0x08,
    0x35,0x06,            /* DataElSeq 6 bytes */
    0x19,0x11,0x15,       /* uuid16_t PANU(0x1115) */
    0x09,0x01,0x00,       /* CsrUint16 0x0100 */
    0x09,0x01,0x00,       /* ServiceName(0x0100) */
    0x25,0x04,            /* String length 9 */
    'P','A','N','U',
    0x09,0x01,0x01,       /* ServiceDescription(0x0101) */
    0x25,0x04,            /* String length 16 */
    'B','C','H','S',
    0x09,0x03,0x0a,       /* SecurityDescription(0x030a) */
    0x09,0x00,0x00,       /* CsrUint16 0x0000 */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,      /* AttrId = BrowseGroupList */
    0x35, 0x03,            /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,      /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

static const CsrUint8 sdsPanGnServiceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,       /* AttrID , ServiceClassIDList */
    0x35,0x03,            /* DataElSeq 3 bytes */
    /* UUID for GN */
    0x19,0x11,0x17,       /* 2 byte UUID, Service class = GN = 0x1117 */

    /* protocol descriptor list */
    0x09,0x00,0x04,       /* AttrId ProtocolDescriptorList */
    0x35,0x1B,            /* DataElSeq 27 bytes */

    /* L2CAP */
    0x35,0x06,            /* 6 bytes in DataElSeq */
    0x19,0x01,0x00,       /* 2 byte UUID, Protocol = L2CAP */
    0x09,0x00,0x0F,
    0x35,0x11,            /* DataElSeq 17 bytes */
    0x19,0x00,0x0F,       /* uuid16_t BNEP(0x000f) */
    0x09,0x01,0x00,       /* CsrUint16 0x0100 */
    0x35,0x09,            /* DataElSeq 9 bytes
                           * List of supported network packet types */
    0x09,0x08,0x00,       /* CsrUint16 0x0800 */
    0x09,0x08,0x06,       /* CsrUint16 0x0806 */
    0x09,0x86,0xdd,       /* CsrUint16 0x86dd */
    0x09,0x00,0x06,       /* LanguageBaseAttributeIDList(0x0006) */
    0x35,0x09,            /* DataElSeq 9 bytes */
    0x09,0x65,0x6e,       /* CsrUint16 0x656e */
    0x09,0x00,0x6a,       /* CsrUint16 0x006a */
    0x09,0x01,0x00,       /* CsrUint16 0x0100 */
    0x09,0x00,0x09,       /* BluetoothProfileDescriptorList(0x0009) */
    0x35,0x08,
    0x35,0x06,            /* DataElSeq 6 bytes */
    0x19,0x11,0x17,       /* uuid16_t GN(0x1117) */
    0x09,0x01,0x00,       /* CsrUint16 0x0100 */
    0x09,0x01,0x00,       /* ServiceName(0x0100) */
    0x25,0x02,            /* String length 7 */
    'G','N',
    0x09,0x01,0x01,       /* ServiceDescription(0x0101) */
    0x25,0x4,             /* String length 16 */
    'B','C','H','S',
    0x09,0x03,0x0a,       /* SecurityDescription(0x030a) */
    0x09,0x00,0x00,       /* CsrUint16 0x0000 */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,      /* AttrId = BrowseGroupList */
    0x35, 0x03,            /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,      /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

static const CsrUint8 sdsPanNapServiceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,       /* AttrID , ServiceClassIDList */
    0x35,0x03,            /* DataElSeq 3 bytes */
    /* UUID for NAP */
    0x19,0x11,0x16,       /* 2 byte UUID, Service class = NAP = 0x1116 */

    /* protocol descriptor list */
    0x09,0x00,0x04,       /* AttrId ProtocolDescriptorList */
    0x35,0x1B,            /* DataElSeq 27 bytes */

    /* L2CAP */
    0x35,0x06,            /* 6 bytes in DataElSeq */
    0x19,0x01,0x00,       /* 2 byte UUID, Protocol = L2CAP */
    0x09,0x00,0x0F,
    0x35,0x11,            /* DataElSeq 17 bytes */
    0x19,0x00,0x0F,       /* uuid16_t BNEP(0x000f) */
    0x09,0x01,0x00,       /* CsrUint16 0x0100 */
    0x35,0x09,            /* DataElSeq 9 bytes */
    0x09,0x08,0x00,       /* CsrUint16 0x0800 */
    0x09,0x08,0x06,       /* CsrUint16 0x0806 */
    0x09,0x86,0xdd,       /* CsrUint16 0x86dd */
    0x09,0x00,0x06,       /* LanguageBaseAttributeIDList(0x0006) */
    0x35,0x09,            /* DataElSeq 9 bytes */
    0x09,0x65,0x6e,       /* CsrUint16 0x656e */
    0x09,0x00,0x6a,       /* CsrUint16 0x006a */
    0x09,0x01,0x00,       /* CsrUint16 0x0100 */
    0x09,0x00,0x09,       /* BluetoothProfileDescriptorList(0x0009) */
    0x35,0x08,
    0x35,0x06,            /* DataElSeq 6 bytes */
    0x19,0x11,0x16,       /* uuid16_t NAP(0x1116) */
    0x09,0x01,0x00,       /* CsrUint16 0x0100 */
    0x09,0x01,0x00,       /* ServiceName(0x0100) */
    0x25,0x03,            /* String length 8 */
    'N','A','P',
    0x09,0x01,0x01,       /* ServiceDescription(0x0101) */
    0x25,0x04,            /* String length 20 */
    'B','C','H','S',
    0x09,0x03,0x0a,       /* SecurityDescription(0x030a) */
    0x09,0x00,0x00,       /* CsrUint16 0x0000 */
    0x09,0x03,0x0b,       /* NetAccessType(0x030b) */
    0x09,0xff,0xfe,       /* CsrUint16 0xfffe */
    0x09,0x03,0x0c,       /* MaxNetAccessRate(0x030c) */
    0x0a,0x00,0x00,0x00,0x01,  /* CsrUint32 0x00000001 */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,      /* AttrId = BrowseGroupList */
    0x35, 0x03,            /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,      /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

void CsrBtPanMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_BSL_PRIM, msg);
}

void StateChange(BslInstanceData_t *instData, BslState_t newState)
{
    instData->state = newState;
}


/***********************************************************************************
    Empty own save queue.
***********************************************************************************/
void SendBslHouseCleaning(BslInstanceData_t *instData)
{
    CsrBtBslHouseCleaning    *prim;

    prim = (CsrBtBslHouseCleaning *) CsrPmemAlloc(sizeof(CsrBtBslHouseCleaning));
    prim->type = CSR_BT_BSL_HOUSE_CLEANING;
    instData->restoreFlag = TRUE;
    CsrBtPanMessagePut(CSR_BT_BSL_IFACEQUEUE, prim);
}

void Go2Idle(BslInstanceData_t *instData)
{
    SendBslHouseCleaning(instData);
    StateChange(instData, IdleS);
}

void CsrBtBslSendSdsRegisterReq(CsrBtBslPanRole role)
{
    CsrUint8 * record = NULL;
    CsrUint16 num_rec_bytes = 0;

    switch (role)
    {
        case CSR_BT_BSL_PANU_ROLE:
            {
                num_rec_bytes = sizeof(sdsPanPanuServiceRecord);
                record = (CsrUint8 *) CsrPmemAlloc(num_rec_bytes );
                CsrMemCpy(record, sdsPanPanuServiceRecord, num_rec_bytes);
                break;
            }
        case CSR_BT_BSL_GN_ROLE:
            {
                num_rec_bytes = sizeof(sdsPanGnServiceRecord);
                record = (CsrUint8 *) CsrPmemAlloc(num_rec_bytes );
                CsrMemCpy(record, sdsPanGnServiceRecord, num_rec_bytes);
                break;
            }
        case CSR_BT_BSL_NAP_ROLE:
            {
                num_rec_bytes = sizeof(sdsPanNapServiceRecord);
                record = (CsrUint8 *) CsrPmemAlloc(num_rec_bytes );
                CsrMemCpy(record, sdsPanNapServiceRecord, num_rec_bytes);
                break;
            }
        default:
            {
                break;
            }
    }

    CsrBtCmSdsRegisterReqSend(CSR_BT_BSL_IFACEQUEUE, record, num_rec_bytes);
}

/*-----------------------------------------------------------------------------

returns TRUE if a register req has been sent
------------------------------------------------------------------------------*/
CsrBool SdsRoleRegister(BslInstanceData_t *instData)
{
    CsrBool                registerSent;

    registerSent = FALSE;
    /* Register only the wanted roles if they are not already registered */
    if (instData->wantedRole & CSR_BT_BSL_NAP_ROLE)
    {
        if ( (instData->localRole & CSR_BT_BSL_NAP_ROLE) == CSR_BT_BSL_NO_ROLE )
        {
            CsrBtBslSendSdsRegisterReq(CSR_BT_BSL_NAP_ROLE);
            registerSent = TRUE;
            instData->currentRoleRegister = CSR_BT_BSL_NAP_ROLE;
        }
    }
    if ( (instData->wantedRole & CSR_BT_BSL_GN_ROLE) && !registerSent )
    {
        if ( (instData->localRole & CSR_BT_BSL_GN_ROLE) == CSR_BT_BSL_NO_ROLE )
        {
            CsrBtBslSendSdsRegisterReq(CSR_BT_BSL_GN_ROLE);
            registerSent = TRUE;
            instData->currentRoleRegister = CSR_BT_BSL_GN_ROLE;
        }
    }
    if ( (instData->wantedRole & CSR_BT_BSL_PANU_ROLE) && !registerSent )
    {
        if ( (instData->localRole & CSR_BT_BSL_PANU_ROLE) == CSR_BT_BSL_NO_ROLE )
        {
            CsrBtBslSendSdsRegisterReq(CSR_BT_BSL_PANU_ROLE);
            registerSent = TRUE;
            instData->currentRoleRegister = CSR_BT_BSL_PANU_ROLE;
        }
    }
    return registerSent;
}


/*-----------------------------------------------------------------------------

returns TRUE if a de register req has been sent
------------------------------------------------------------------------------*/
CsrBool SdsRoleDeregister(BslInstanceData_t *instData)
{
    CsrBool    deregisterSent;

    deregisterSent = FALSE;
    /* See if any roles should be deregistered */
    if (instData->localRole & CSR_BT_BSL_NAP_ROLE)
    {
        if ( (instData->wantedRole & CSR_BT_BSL_NAP_ROLE) == CSR_BT_BSL_NO_ROLE )
        {
            CsrBtCmSdsUnRegisterReqSend(CSR_BT_BSL_IFACEQUEUE, instData->napSdsRecHandle);
            deregisterSent = TRUE;
            instData->currentRoleRegister = CSR_BT_BSL_NAP_ROLE;
        }
    }
    if ( (instData->localRole & CSR_BT_BSL_GN_ROLE) && !deregisterSent )
    {
        if ( (instData->wantedRole & CSR_BT_BSL_GN_ROLE) == CSR_BT_BSL_NO_ROLE )
        {
            CsrBtCmSdsUnRegisterReqSend(CSR_BT_BSL_IFACEQUEUE, instData->gnSdsRecHandle);
            deregisterSent = TRUE;
            instData->currentRoleRegister = CSR_BT_BSL_GN_ROLE;
        }
    }
    if ( (instData->localRole & CSR_BT_BSL_PANU_ROLE) && !deregisterSent )
    {
        if ( (instData->wantedRole & CSR_BT_BSL_PANU_ROLE) == CSR_BT_BSL_NO_ROLE )
        {
            CsrBtCmSdsUnRegisterReqSend(CSR_BT_BSL_IFACEQUEUE, instData->panuSdsRecHandle);
            deregisterSent = TRUE;
            instData->currentRoleRegister = CSR_BT_BSL_PANU_ROLE;
        }
    }
    return deregisterSent;
}

void CsrBtBslSendCmSdcAttributeReq(BslInstanceData_t *instData)
{
    /* search for the next attribute */
    CsrBtCmSdcAttributeReqSend(instData->sdcSearchHandle,
                          sdcSearchAttrList[instData->sdcAttrSearchIndex],
                          SDC_MAX_RETURN_BYTES);
}

CsrBtBslPanRole CsrBtBslGetRoleFromUuid(CsrUint16 uuid)
{
    if (uuid == CSR_BT_PAN_PANU_PROFILE_UUID)
    {
        return CSR_BT_BSL_PANU_ROLE;
    }
    else if (uuid == CSR_BT_PAN_NAP_PROFILE_UUID)
    {
        return CSR_BT_BSL_NAP_ROLE;
    }
    else
    {
        return CSR_BT_BSL_GN_ROLE;
    }
}

CsrUint16 CsrBtBslGetUuidFromRole(CsrBtBslPanRole role)
{
    if (role == CSR_BT_BSL_PANU_ROLE)
    {
        return CSR_BT_PAN_PANU_PROFILE_UUID;
    }
    else if (role == CSR_BT_BSL_NAP_ROLE)
    {
        return CSR_BT_PAN_NAP_PROFILE_UUID;
    }
    else
    {
        return CSR_BT_PAN_GN_PROFILE_UUID;
    }
}

void CsrBtBslSendBslDisconnectInd(CsrSchedQid appHandle, CsrUint16 id,
    CsrBtReasonCode reasonCode, CsrBtSupplier reasonSupplier)
{
    CsrBtBslDisconnectInd *prim;

    prim = (CsrBtBslDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtBslDisconnectInd));
    prim->reasonCode = reasonCode;
    prim->reasonSupplier = reasonSupplier;
    prim->id = id;
    prim->type = CSR_BT_BSL_DISCONNECT_IND;
    CsrBtPanMessagePut(appHandle, prim);
}

void CsrBtBslSendBslConnectInd(CsrSchedQid phandle,
                          CsrUint16 id,
                          BD_ADDR_T *remAddr,
                          CsrBtBslPanRole remoteRole,
                          CsrBtBslPanRole localRole,
                          CsrBtResultCode resultCode,
                          CsrBtSupplier resultSupplier)
{
    CsrBtBslConnectInd *prim;

    prim = (CsrBtBslConnectInd *) CsrPmemAlloc(sizeof(CsrBtBslConnectInd));
    prim->type = CSR_BT_BSL_CONNECT_IND;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    prim->id = id;
    prim->bdAddr = *remAddr;
    prim->remoteRole = remoteRole;
    prim->localRole = localRole;
    CsrBtPanMessagePut(phandle, prim);
}

void CsrBtBslSendBslStatusInd(BslInstanceData_t *instData, CsrUint16 id, CsrBtBslStatus event, CsrUint8 status)
{
    CsrBtBslStatusInd *prim;

    prim = (CsrBtBslStatusInd *) CsrPmemAlloc(sizeof(CsrBtBslStatusInd));
    prim->type = CSR_BT_BSL_STATUS_IND;
    prim->event = event;
    prim->status = status;
    prim->id = id;
    CsrBtPanMessagePut(instData->appHandle, prim);
}

static CsrBool getProtocolDescriptorListNumOfSupportedNetworkPacketTypeValues(CsrUint16 attributeListLength,
                                                                             CsrUint8 * attributeList,
                                                                             CsrUint32 specificParameter1Index,
                                                                             CsrUint16 *numOfTypes)
{
    *numOfTypes = 0;

    if (specificParameter1Index < attributeListLength)
    {
        CsrUint32    size;
        CsrUint8        type;

        type    = (attributeList[specificParameter1Index] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK);
        size    = CsrBtReturnSizeDescriptorValue(attributeList[specificParameter1Index]);

        if (type == CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
        {
            if (size == 1)
            {
                if ((specificParameter1Index + 1) < attributeListLength)
                {
                    size        = attributeList[specificParameter1Index + 1];
                }
                else
                    return FALSE;
            }
            *numOfTypes = (CsrUint16)((size - (size / 3)) / 2);
            if (*numOfTypes > 0)
                return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

static CsrBool getBluetoothProfileDescriptorListVersionParameter(CsrUint16 attributeListLength,
                                                                CsrUint8 * attributeList,
                                                                CsrUint16 *version)
{
    CsrUint8        type;
    CsrUint32    size, index;

    if(CsrBtSdcGetAttribute(attributeListLength,
                       attributeList,
                       CSR_BT_BLUETOOTH_PROFILE_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER,
                       &type,
                       &size,
                       &index))
    {
        CsrUint32 listLength, nextindexToRead = 0, readBytes = 0, read = 0;

        listLength        = size;
        nextindexToRead = index;

        while (read < listLength)
        {
            if(CsrBtSdcGetTypeDescriptorInsideList(attributeListLength,
                                              attributeList,
                                              nextindexToRead,
                                              &type,
                                              &size,
                                              &readBytes))
            {
                CsrUint16 value;
                if (type != CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
                    readBytes +=  size;

                if (type == CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE)
                {
                    if (size == 2)
                    {
                        if(CsrBtSdcGetUint16Value(attributeListLength, attributeList, nextindexToRead, &value))
                        {
                            *version = value;
                            return TRUE;
                        }
                        return FALSE;
                    }
                }

                nextindexToRead += readBytes;
                read            += readBytes;
            }
            else
                return FALSE;
        }
    }
    return FALSE;
}

static CsrBool getProtocolDescriptorListVersionParameter(CsrUint16 attributeListLength,
                                                        CsrUint8 * attributeList,
                                                        CsrUint16 *version,
                                                        CsrUint32 *specificParameter1Index)
{
    CsrUint8        type;
    CsrUint32    size, index;

    if(CsrBtSdcGetAttribute(attributeListLength, attributeList, CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, &type, &size, &index))
    {
        CsrBool        foundVersion    = FALSE;
        CsrUint32    listLength, nextindexToRead = 0, readBytes = 0, read = 0;

        listLength        = size;
        nextindexToRead = index;

        while (read < listLength)
        {
            if(CsrBtSdcGetTypeDescriptorInsideList(attributeListLength, attributeList, nextindexToRead, &type, &size, &readBytes))
            {
                CsrUint16 value;
                if (type != CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
                    readBytes +=  size;


                if (type == CSR_BT_DATA_ELEMENT_UUID_TYPE)
                {
                    CsrUint32 newIndex = nextindexToRead;

                    if (size != 2)
                        newIndex += 2;

                    if (CsrBtSdcGetUint16Value(attributeListLength, attributeList, newIndex, &value))
                    {
                        if (value == CSR_BT_BNEP_PROTOCOL_UUID16_IDENTIFIER)
                            foundVersion = TRUE;
                    }
                }
                if (foundVersion)
                {
                    if (type == CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE)
                    {
                        if (size == 2)
                        {
                            if(CsrBtSdcGetUint16Value(attributeListLength, attributeList, nextindexToRead, &value))
                            {
                                *version                    = value;
                                *specificParameter1Index    = nextindexToRead + 3;
                                return TRUE;
                            }
                            return FALSE;
                        }
                    }
                }
                nextindexToRead += readBytes;
                read            += readBytes;
            }
            else
                return FALSE;
        }
    }
    return FALSE;
}

static void languageBaseAttributeHandler(CsrBtBslSearchRecord * bslSearchRecord, CsrUint16 attributeListLength, CsrUint8 * attributeList)
{
    CsrUint32 i, languageBaseIndex = 0;
    if (CsrBtGetNumOfLanguageBaseAttributeId(attributeListLength,
                                        attributeList,
                                        &languageBaseIndex,
                                        &bslSearchRecord->languageElementsCount))
    {
        if (bslSearchRecord->languageElementsCount > CSR_BT_MAX_NUM_OF_LANGUAGE_ELEMENTS)
        {
            bslSearchRecord->languageElementsCount = CSR_BT_MAX_NUM_OF_LANGUAGE_ELEMENTS;
        }

        bslSearchRecord->languageElements = CsrPmemAlloc(bslSearchRecord->languageElementsCount * sizeof(CsrBtLanguageElement));

        for (i = 0; i <  bslSearchRecord->languageElementsCount; i++)
        {
            CsrUint32 nextIndex;
            if (!CsrBtGetLanguageBaseAttributeIdElement(attributeListLength, attributeList, languageBaseIndex,
                                                   &(bslSearchRecord->languageElements[i]), &nextIndex))
            {
                bslSearchRecord->languageElementsCount = 0;
                return;
            }
            languageBaseIndex = nextIndex;
        }
    }
}

static void getTextStringHandler(CsrCharString **dest, CsrUint16 attributeListLength, CsrUint8 * attributeList, CsrUint16 attributeIdentifier)
{
    CsrUint32 dummySize, dummyIndex;

    *dest = CsrPmemAlloc(CSR_BT_MAX_FRIENDLY_NAME_LEN + 1);
    CsrBtSdcGetTextStringDirect((CsrUint8 *)*dest, (CSR_BT_MAX_FRIENDLY_NAME_LEN),
                           attributeListLength, attributeList, attributeIdentifier, &dummySize, &dummyIndex);
}

static void getProtocolDescriptorListHandler(CsrBtBslSearchRecord * bslSearchRecord, CsrUint16 attributeListLength, CsrUint8 * attributeList)
{
    CsrUint32 i, protocolVersionIndex = 0;
    if (getProtocolDescriptorListVersionParameter(attributeListLength,
                                                  attributeList,
                                                  &bslSearchRecord->protocolVersion,
                                                  &protocolVersionIndex))
    {
        if (getProtocolDescriptorListNumOfSupportedNetworkPacketTypeValues(attributeListLength,
                                                                           attributeList,
                                                                           protocolVersionIndex,
                                                                           &bslSearchRecord->numOfSupportedNetwPacketTypes))
        {
            if (bslSearchRecord->numOfSupportedNetwPacketTypes > CSR_BT_MAX_NUM_OF_NETW_PACKET_TYPE)
            {
                bslSearchRecord->numOfSupportedNetwPacketTypes = CSR_BT_MAX_NUM_OF_NETW_PACKET_TYPE;
            }
            for (i = 0; i < bslSearchRecord->numOfSupportedNetwPacketTypes; i++)
            {
                if (CsrBtSdcGetUint16Value(attributeListLength, attributeList, protocolVersionIndex, &(bslSearchRecord->supportedNetwPacketType[i])))
                    protocolVersionIndex += 3;
                else
                {
                    bslSearchRecord->numOfSupportedNetwPacketTypes = 0;
                    return;
                }
            }
        }
    }
}

void PutSdsVariables(BslInstanceData_t *instData, CsrUint16 attributeListLength, CsrUint8 * attributeList)
{
    CsrUint16          attributeIdentifier;
    CsrBtBslSearchRecord *bslSearchRecord;

    attributeIdentifier = sdcSearchAttrList[instData->sdcAttrSearchIndex];
    bslSearchRecord = instData->sdcSearchRec;
    switch(attributeIdentifier)
    {
        case CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER:
            getProtocolDescriptorListHandler(bslSearchRecord,
                                             attributeListLength,
                                             attributeList);
            break;
        case CSR_BT_LANGUAGE_BASE_ATTRIBUTE_ID_LIST_ATTRIBUTE_IDENTIFIER:
            languageBaseAttributeHandler(bslSearchRecord,
                                         attributeListLength,
                                         attributeList);
            break;
        case CSR_BT_SERVICE_AVAILABILITY_ATTRIBUTE_IDENTIFIER:
            bslSearchRecord->loadFactorIncluded = CsrBtSdcGetUint8Direct(attributeListLength,
                                                                    attributeList,
                                                                    attributeIdentifier,
                                                                    &bslSearchRecord->loadFactor);
            break;
        case CSR_BT_BLUETOOTH_PROFILE_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER:
            getBluetoothProfileDescriptorListVersionParameter(attributeListLength,
                                                              attributeList,
                                                              &bslSearchRecord->bluetoothProfileVersion);
            break;
        case CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER:
            getTextStringHandler(&bslSearchRecord->serviceName,
                                attributeListLength,
                                attributeList,
                                attributeIdentifier);
            break;
        case CSR_BT_SERVICE_DESCRIPTION_ATTRIBUTE_IDENTIFIER:
            getTextStringHandler(&bslSearchRecord->serviceDescription,
                                attributeListLength,
                                attributeList,
                                attributeIdentifier);
            break;
        case CSR_BT_SECURITY_DESCRIPTION_ATTRIBUTE_IDENTIFIER:
            CsrBtSdcGetUint16Direct(attributeListLength,
                               attributeList, attributeIdentifier,
                               &bslSearchRecord->securityDescription);
            break;
        case CSR_BT_NET_ACCESS_TYPE_ATTRIBUTE_IDENTIFIER:
            bslSearchRecord->netAccessTypeIncluded = CsrBtSdcGetUint16Direct(attributeListLength,
                                                                        attributeList,
                                                                        attributeIdentifier,
                                                                        &bslSearchRecord->netAccessType);
            break;
        case CSR_BT_MAX_NET_ACCESS_RATE_ATTRIBUTE_IDENTIFIER:
            bslSearchRecord->maxNetAccessRateIncluded = CsrBtSdcGetUint32Direct(attributeListLength,
                                                                           attributeList,
                                                                           attributeIdentifier,
                                                                           &bslSearchRecord->maxNetAccessRate);
            break;
        case CSR_BT_IPV4_SUBNET_ATTRIBUTE_IDENTIFIER:
            getTextStringHandler(&bslSearchRecord->ipv4Subnet,
                                attributeListLength,
                                attributeList, attributeIdentifier);
            break;
        case CSR_BT_IPV6_SUBNET_ATTRIBUTE_IDENTIFIER:
            getTextStringHandler(&bslSearchRecord->ipv6Subnet,
                                attributeListLength,
                                attributeList,
                                attributeIdentifier);
            break;
    }
}

void CsrBtBslSendBslServiceSearchResultInd(BslInstanceData_t *instData)
{
    CsrBtBslServiceSearchResultInd *prim;

    prim = CsrPmemAlloc(sizeof(CsrBtBslServiceSearchResultInd));
    prim->type = CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND;
    prim->bd_addr = instData->currentConn2Addr;
    prim->searchRole = instData->sdcSearchRole;
    prim->moreResults = FALSE;

    if (instData->sdcSearchRec)
    {
        CsrMemCpy( &(prim->searchResult), instData->sdcSearchRec, sizeof(CsrBtBslSearchRecord));
        /* Copy external data */
        prim->searchResult.serviceName = (CsrCharString*)CsrStrDup((char*)instData->sdcSearchRec->serviceName);
        prim->searchResult.serviceDescription = (CsrCharString*)CsrStrDup((char*)instData->sdcSearchRec->serviceDescription);
        prim->searchResult.ipv4Subnet = (CsrCharString*)CsrStrDup((char*)instData->sdcSearchRec->ipv4Subnet);
        prim->searchResult.ipv6Subnet = (CsrCharString*)CsrStrDup((char*)instData->sdcSearchRec->ipv6Subnet);
    }
    else
    {
        CsrMemSet(&(prim->searchResult),0,sizeof(CsrBtBslSearchRecord));
    }

    CsrBtPanMessagePut(instData->sdpHandle, prim);
}

void CsrBtBslSendBslServiceSearchCfm(BslInstanceData_t *instData,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtBslServiceSearchCfm *prim;

    prim = (CsrBtBslServiceSearchCfm *) CsrPmemAlloc(sizeof(CsrBtBslServiceSearchCfm));
    prim->type = CSR_BT_BSL_SERVICE_SEARCH_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    prim->bd_addr = instData->currentConn2Addr;
    CsrBtPanMessagePut(instData->sdpHandle, prim);
}

/******************************************************************************

    function BslConvEtherNative2Big  --
    convert a ETHER_ADDR to big endian

******************************************************************************/
ETHER_ADDR CsrBtBslConvNativeEther2EtherBig (ETHER_ADDR *nativeAddr)
{
    ETHER_ADDR    etherAddr;

    etherAddr = CsrBtEtherAddrWordSwap(nativeAddr);
    return etherAddr;
}

/******************************************************************************

    function BslConvEtherNative2Big  --
    convert a ETHER_ADDR to big endian

******************************************************************************/
ETHER_ADDR CsrBtBslConvEtherBig2Native (ETHER_ADDR *etherAddr)
{
    ETHER_ADDR    nativeAddr;

    nativeAddr = CsrBtEtherAddrWordSwap(etherAddr);
    return nativeAddr;
}

CsrUint8 CsrBtBslFindEmptyIndex(BslInstanceData_t *instData)
{
    CsrUint8 result = BSL_INVALID_CONNECTION;
    CsrUint8 i;

    for (i = 0; i < CSR_BT_BSL_MAX_NUM_OF_CONNECTIONS; i++)
    {
        if (CsrBtBdAddrEqZero(&instData->connectionList[i].bdAddr))
        {
            result = i;
            break;
        }
    }

    return result;
}

CsrUint8 CsrBtBslFindBdAddrIndex(BslInstanceData_t *instData, BD_ADDR_T *bdAddr)
{
    CsrUint8 result = BSL_INVALID_CONNECTION;
    CsrUint8 i;

    for (i = 0; i < CSR_BT_BSL_MAX_NUM_OF_CONNECTIONS; i++)
    {
        if (bd_addr_eq(&instData->connectionList[i].bdAddr, bdAddr))
        {
            result = i;
            break;
        }
    }

    return result;
}

CsrUint8 CsrBtBslFindIdIndex(BslInstanceData_t *instData, CsrUint16 id)
{
    CsrUint8 result = BSL_INVALID_CONNECTION;
    CsrUint8 i;

    for (i = 0; i < CSR_BT_BSL_MAX_NUM_OF_CONNECTIONS; i++)
    {
        if (instData->connectionList[i].id == id)
        {
            result = i;
            break;
        }
    }

    return result;
}

void CsrBtBslClearConnection(BslConnection_t *conn)
{
    CsrMemSet(conn, 0, sizeof(BslConnection_t));
}


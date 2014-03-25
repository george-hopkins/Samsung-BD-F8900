/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

#define CSR_BT_AMPM_ENCODE_U8_IE(source, dest, offset, bufferSize) \
    CSR_BT_AMPM_ASSERT(((bufferSize)-(offset)) >= 1);\
    (dest)[offset++] = (CsrUint8)(source)

#define CSR_BT_AMPM_ENCODE_U16_IE(source, dest, offset, bufferSize) \
    CSR_BT_AMPM_ASSERT(((bufferSize)-(offset)) >= 2);\
    (dest)[offset++] = (CsrUint8)((source) & 0xFF);\
    (dest)[offset++] = (CsrUint8)(((source) >> 8) & 0xFF);

#define CSR_BT_AMPM_ENCODE_U32_IE(source, dest, offset, bufferSize) \
    CSR_BT_AMPM_ASSERT(((bufferSize)-(offset)) >= 4);\
    (dest)[offset++] = (CsrUint8)((source) & 0xFF);\
    (dest)[offset++] = (CsrUint8)(((source) >> 8) & 0xFF); \
    (dest)[offset++] = (CsrUint8)(((source) >> 16) & 0xFF); \
    (dest)[offset++] = (CsrUint8)(((source) >> 24) & 0xFF);

#define CSR_BT_AMPM_ENCODE_DATA_IE(source, dest, offset, bufferSize, dataSize) \
    CSR_BT_AMPM_ASSERT(((bufferSize)-(offset)) >= dataSize);\
    (void)CsrMemCpy((void *)(dest + offset), (const void *)(source), (CsrUint32)(dataSize));\
    offset+=dataSize;

/* Populates the supplied pointer with the local AMP List */
static void csrBtAmpmSduFillLocalAmpList(CsrBtAmpmInstance * inst,
                                         CsrUint8 * pData,
                                         CsrUint16 offset,
                                         CsrUint16 bufferSize)
{
    CsrBtAmpmLocalAmp *pLocalAmp = (CsrBtAmpmLocalAmp*)CsrCmnListGetFirst(&inst->localAmpList);

    CSR_BT_AMPM_ASSERT(pData != NULL);

    /* The list always contains the BR/EDR controller. */
    CSR_BT_AMPM_ENCODE_U8_IE(0, pData, offset, bufferSize); /* Id is always 0 */
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMP_AMP_CONTROLLER_TYPE_BR_EDR,
                             pData, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMP_AMP_STATUS_ONLY_USED_BY_BLUETOOTH_TECHNOLOGY,
                             pData, offset, bufferSize);

    while (pLocalAmp)
    {
        /* Only inform other AMP managers of properly initialised AMPs */
        if (pLocalAmp->initialised)
        {
            CSR_BT_AMPM_ENCODE_U8_IE(CsrBtAmpmLocalGetId(pLocalAmp), pData,
                offset, bufferSize);
            CSR_BT_AMPM_ENCODE_U8_IE(CsrBtAmpmLocalGetType(pLocalAmp), pData,
                offset, bufferSize);
            CSR_BT_AMPM_ENCODE_U8_IE(CsrBtAmpmLocalGetStatus(pLocalAmp),
                pData, offset, bufferSize);
        }
        pLocalAmp = (CsrBtAmpmLocalAmp*)pLocalAmp->next;
    }
}

/* Returns the number of initialised local AMPs */
static CsrUint16 csrBtAmpmSduNumOfInitialisedAmps(
    CsrBtAmpmInstance * inst)
{
    CsrUint16 initialisedAmps = 0;
    CsrBtAmpmLocalAmp *pLocalAmp = (CsrBtAmpmLocalAmp*)CsrCmnListGetFirst(&inst->localAmpList);

    while (pLocalAmp)
    {
        /* Only inform other AMP managers of properly initialised AMPs */
        if (pLocalAmp->initialised)
        {
            initialisedAmps++;
        }
        pLocalAmp = (CsrBtAmpmLocalAmp*)pLocalAmp->next;
    }
    return initialisedAmps;
}

/* Checks that each amp id in the list is unique if not then false is
 * returned. */
static CsrBool csrBtAmpmIsAmpListValid(
    const CsrUint8 * pAmpList,
    const CsrUint16 length)
{
    CsrBool IsAmpListValid = TRUE;

    CSR_BT_AMPM_ASSERT(pAmpList != NULL);

    /* Must the list at least have the Br/Edr controller? */
    if (CSR_BT_AMPM_SIZEOF_AMP_LIST_ENTRY < length)
    {
        /* Each Amp id should only appear in the list once */
        CsrBtAmpmId *pIdToCheck = (CsrBtAmpmId *) pAmpList; /* First Id in the list */
        CsrBtAmpmId *pIdFromList = (CsrBtAmpmId *) (pAmpList + CSR_BT_AMPM_SIZEOF_AMP_LIST_ENTRY);  /* Next Id in the list */
        const CsrUint8 *pLasCsrBtAmpmIdInList = (pAmpList + (length - 1)) - 2;

        /* Iterate over the whole list checking that Ids only appear once */
        while (IsAmpListValid && pIdToCheck < pLasCsrBtAmpmIdInList)
        {
            if (*pIdToCheck == *pIdFromList)
            {
                IsAmpListValid = FALSE;
            }

            pIdFromList += CSR_BT_AMPM_SIZEOF_AMP_LIST_ENTRY;

            /* If we've iterated over the list move onto the next id */
            if (pIdFromList > pLasCsrBtAmpmIdInList)
            {
                pIdToCheck += CSR_BT_AMPM_SIZEOF_AMP_LIST_ENTRY;
                pIdFromList = pIdToCheck + CSR_BT_AMPM_SIZEOF_AMP_LIST_ENTRY;
            }
        }
    }
    return (IsAmpListValid);
}

/* Returns the length of the extended features mask supplied as
 * input */
static CsrUint16 csrBtAmpmGetByteSizeOfExtFeaturesMask(
    const CsrUint8 * pExtFeatureMask,
    const CsrUint16 maxLength)
{
    CsrUint16 extFeatureMaskField;

    /* The size will always be at least one 2 byte entry */
    CsrUint16 size = CSR_BT_AMPM_SIZEOF_EXT_FEATURE_MASK;
    CsrUint8 offset = 0;
    CsrBool moreToFollow = FALSE;

    CSR_BT_AMPM_ASSERT(pExtFeatureMask != NULL);

    extFeatureMaskField = CSR_BT_AMPM_DECODE_U16_IE(pExtFeatureMask, offset);
    offset += CSR_BT_AMPM_SIZEOF_EXT_FEATURE_MASK;
    /* Each entry is a minimum of 2 bytes that will be extended by another
     * 2 byte entry if the top most bit is set.
     */
    if (CSR_BT_AMPM_MORE_EXT_FEATURE_MASKS ==
        (extFeatureMaskField & CSR_BT_AMPM_MORE_EXT_FEATURE_MASKS))
    {
        size += CSR_BT_AMPM_SIZEOF_EXT_FEATURE_MASK;
        moreToFollow = TRUE;
    }
    while (moreToFollow && offset < maxLength)
    {
        extFeatureMaskField =
            CSR_BT_AMPM_DECODE_U16_IE(pExtFeatureMask, offset);
        offset += CSR_BT_AMPM_SIZEOF_EXT_FEATURE_MASK;
        if (CSR_BT_AMPM_MORE_EXT_FEATURE_MASKS ==
            (extFeatureMaskField & CSR_BT_AMPM_MORE_EXT_FEATURE_MASKS))
        {
            size += CSR_BT_AMPM_SIZEOF_EXT_FEATURE_MASK;
            moreToFollow = TRUE;
        }
        else
        {
            moreToFollow = FALSE;
        }
    }
    return (size);
}

CsrUint8 CsrBtAmpmSduAllocIdentifier(
    CsrBtAmpmInstance * inst)
{
    if (0xFF == inst->sduIdentifier)
    {
        inst->sduIdentifier = 1;
    }
    else
    {
        inst->sduIdentifier++;
    }
    return (inst->sduIdentifier);
}

CsrMblk *CsrBtAmpmSduCreateAmpCommandReject(
    CsrUint8 identifier)
{
    CsrMblk *pMsgBlk;

    /* buffer size is always 6 bytes */
    const CsrUint16 bufferSize = CSR_BT_AMPM_MIN_COMMAND_REJECT_SDU_SIZE;
    CsrUint16 offset = 0;
    CsrUint8 *pSDU;

    /* Length is fixed at 2 bytes, as there is no optional data field */
    CsrUint16 length = CSR_BT_AMPM_MIN_COMMAND_REJECT_LENGTH_VALUE;
    CsrUint16 Reason = CSR_BT_AMPM_REJECT_REASON_NOT_RECOGNISED;

    pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU), bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMPM_COMMAND_REJECT, pSDU, offset,
        bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(identifier, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(length, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(Reason, pSDU, offset, bufferSize);

    return (pMsgBlk);
}


CsrBool CsrBtAmpmSduValidateAmpCommandReject(
    const CsrUint16 length,
    const CsrUint8 * pSdu)
{
    CsrBool isValid = FALSE;

    CSR_BT_AMPM_ASSERT(pSdu != NULL);

    /* The SDU has to be a minimum of 6 bytes if there is no data field */
    if (CSR_BT_AMPM_MIN_COMMAND_REJECT_SDU_SIZE <= length)
    {
        CsrUint16 msgDataLength =
            CSR_BT_AMPM_DECODE_U16_IE(pSdu, CSR_BT_AMPM_SDU_LENGTH_OFFSET);

        /* The data portion of the SDU has to be at least 2 byte in length
         * to account for the Reason field.
         */
        if (CSR_BT_AMPM_MIN_COMMAND_REJECT_LENGTH_VALUE <= msgDataLength)
        {
            isValid = TRUE;
        }
    }
    return (isValid);
}

CsrMblk *CsrBtAmpmSduCreateAmpDiscoverRequest(
    CsrUint8 identifier,
    CsrUint16 supportedMTUSize)
{
    CsrMblk *pMsgBlk = NULL;

    const CsrUint16 bufferSize = CSR_BT_AMPM_MIN_DISCOVER_REQUEST_SDU_SIZE;
    CsrUint16 offset = 0;
    CsrUint8 *pSDU;

    CsrUint16 length = CSR_BT_AMPM_MIN_DISCOVER_REQUEST_LENGTH_VALUE;

    pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU), bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMPM_DISCOVER_REQUEST, pSDU, offset,
        bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(identifier, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(length, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(supportedMTUSize, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(CSR_BT_AMPM_MANAGER_FEATURES, pSDU, offset,
        bufferSize);

    return (pMsgBlk);
}


CsrBool CsrBtAmpmSduValidateAmpDiscoverRequest(
    const CsrUint16 length,
    const CsrUint8 * pSdu)
{
    CsrBool isValid = FALSE;

    CSR_BT_AMPM_ASSERT(pSdu != NULL);

    if (CSR_BT_AMPM_MIN_DISCOVER_REQUEST_SDU_SIZE <= length)
    {
        CsrUint16 msgDataLength =
            CSR_BT_AMPM_DECODE_U16_IE(pSdu, CSR_BT_AMPM_SDU_LENGTH_OFFSET);

        if (CSR_BT_AMPM_MIN_DISCOVER_REQUEST_LENGTH_VALUE <= msgDataLength)
        {
            CsrUint16 extFeatureMaskSize =
                csrBtAmpmGetByteSizeOfExtFeaturesMask(pSdu +
                CSR_BT_AMPM_DISCOVER_EXT_FEATURE_MASK_OFFSET,
                (CsrUint16) (msgDataLength -
                    CSR_BT_AMPM_SIZEOF_MTU_MPS_SIZE));
            if (extFeatureMaskSize + CSR_BT_AMPM_SIZEOF_MTU_MPS_SIZE ==
                msgDataLength)
            {
                isValid = TRUE;
            }
        }
    }
    return (isValid);
}

CsrMblk *CsrBtAmpmSduCreateAmpDiscoverResponse(
    CsrBtAmpmInstance * inst,
    CsrUint8 identifier,
    CsrUint16 supportedMTUSize)
{
    CsrMblk *pMsgBlk = NULL;

    /* The AMP List always contains the BR/EDR controller, but this is not stored
     * as a local AMP instance, hence the + 1
     */
    const CsrUint16 ampListLength =
        (CsrUint16) ((csrBtAmpmSduNumOfInitialisedAmps(inst) +
            1) * CSR_BT_AMPM_SIZEOF_AMP_LIST_ENTRY);
    const CsrUint16 bufferSize =
        CSR_BT_AMPM_MIN_DISCOVER_RESPONSE_SDU_SIZE + ampListLength;
    CsrUint16 offset = 0;
    CsrUint8 *pSDU;
    CsrUint16 length =
        CSR_BT_AMPM_MIN_DISCOVER_RESPONSE_LENGTH_VALUE + ampListLength;

    pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU), bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMPM_DISCOVER_RESPONSE, pSDU, offset,
        bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(identifier, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(length, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(supportedMTUSize, pSDU, offset, bufferSize);
    /* At the moment the AMP features don't require the addition
     * of extended feature mask bytes fields ie. just a single
     * 2byte field will do at the moment.
     */
    CSR_BT_AMPM_ENCODE_U16_IE(CSR_BT_AMPM_MANAGER_FEATURES, pSDU, offset,
        bufferSize);

    csrBtAmpmSduFillLocalAmpList(inst, pSDU, offset, bufferSize);

    return (pMsgBlk);
}


CsrBool CsrBtAmpmSduValidateAmpDiscoverResponse(
    const CsrUint16 length,
    const CsrUint8 * pSdu)
{
    CsrBool isValid = FALSE;

    CSR_BT_AMPM_ASSERT(pSdu != NULL);

    /* The SDU has to be a minimum of 8 bytes + a minimum of one amp list
     * entry for the BR/EDR controller.
     */
    if (CSR_BT_AMPM_MIN_DISCOVER_RESPONSE_SDU_SIZE +
        CSR_BT_AMPM_SIZEOF_AMP_LIST_ENTRY <= length)
    {
        CsrUint16 extFeatureMaskSize;
        CsrUint16 msgDataLength =
            CSR_BT_AMPM_DECODE_U16_IE(pSdu, CSR_BT_AMPM_SDU_LENGTH_OFFSET);

        /* One or both of the extended features mask or the AMP list is
         * bigger than the minimum requred for the SDU to be valid so we
         * need to check both.
         */
        extFeatureMaskSize = csrBtAmpmGetByteSizeOfExtFeaturesMask(
            (const CsrUint8 *) (pSdu + CSR_BT_AMPM_DISCOVER_EXT_FEATURE_MASK_OFFSET),
            (CsrUint16) ((msgDataLength - CSR_BT_AMPM_SIZEOF_MTU_MPS_SIZE) - CSR_BT_AMPM_SIZEOF_AMP_LIST_ENTRY));

        if ((CSR_BT_AMPM_MIN_DISCOVER_RESPONSE_SDU_SIZE +
             (extFeatureMaskSize - CSR_BT_AMPM_SIZEOF_EXT_FEATURE_MASK) +  /* One entry already included in the MIN SDU SIZE */
             CSR_BT_AMPM_SIZEOF_AMP_LIST_ENTRY) == length)
        {
            isValid = TRUE;
        }
        else
        {
            isValid = csrBtAmpmIsAmpListValid(
                (const CsrUint8 *) (pSdu + CSR_BT_AMPM_DISCOVER_EXT_FEATURE_MASK_OFFSET + extFeatureMaskSize),
                (CsrUint16) (msgDataLength - CSR_BT_AMPM_MIN_DISCOVER_RESPONSE_LENGTH_VALUE));
        }
    }
    return (isValid);
}


CsrMblk *CsrBtAmpmSduCreateAmpChangeNotify(
    CsrBtAmpmInstance * inst,
    CsrUint8 identifier)
{
    CsrMblk *pMsgBlk = NULL;
    const CsrUint16 ampListLength =
        (CsrUint16) ((csrBtAmpmSduNumOfInitialisedAmps(inst) +
            1) * CSR_BT_AMPM_SIZEOF_AMP_LIST_ENTRY);
    const CsrUint16 bufferSize =
        CSR_BT_AMPM_MIN_CHANGE_NOTIFY_SDU_SIZE + ampListLength;
    CsrUint16 offset = 0;
    CsrUint8 *pSDU;

    pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU), bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMPM_CHANGE_NOTIFY, pSDU, offset,
        bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(identifier, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(ampListLength, pSDU, offset, bufferSize);

    csrBtAmpmSduFillLocalAmpList(inst, pSDU, offset, bufferSize);

    return (pMsgBlk);
}


CsrBool CsrBtAmpmSduValidateAmpChangeNotify(
    const CsrUint16 length,
    const CsrUint8 * pSdu)
{
    CsrBool isValid = FALSE;

    CSR_BT_AMPM_ASSERT(pSdu != NULL);

    /* The SDU has to be a minimum of 4 bytes + a minimum of one amp list
     * entry for the BR/EDR controller.
     */
    if (CSR_BT_AMPM_MIN_CHANGE_NOTIFY_SDU_SIZE +
        CSR_BT_AMPM_SIZEOF_AMP_LIST_ENTRY <= length)
    {
        CsrUint16 msgDataLength =
            CSR_BT_AMPM_DECODE_U16_IE(pSdu, CSR_BT_AMPM_SDU_LENGTH_OFFSET);

        /* The data portion of the SDU has to be 0 bytes + a minimum of
         * one amp list entry for the BR/EDR controller in length
         */
        if (CSR_BT_AMPM_MIN_CHANGE_NOTIFY_LENGTH_VALUE +
            CSR_BT_AMPM_SIZEOF_AMP_LIST_ENTRY <= msgDataLength)
        {
            isValid =
                csrBtAmpmIsAmpListValid((const CsrUint8 *) (pSdu +
                    CSR_BT_AMPM_LIST_OFFSET_IN_CHANGE_IND),
                    (CsrUint16) (msgDataLength -
                    CSR_BT_AMPM_MIN_CHANGE_NOTIFY_LENGTH_VALUE));
        }
    }
    return (isValid);
}

CsrMblk *CsrBtAmpmSduCreateAmpChangeResponse(
    CsrUint8 identifier)
{
    CsrMblk *pMsgBlk = NULL;

    /* buffer size is always 4 bytes */
    const CsrUint16 bufferSize = CSR_BT_AMPM_MIN_CHANGE_RESPONSE_SDU_SIZE;
    CsrUint16 offset = 0;
    CsrUint8 *pSDU;
    CsrUint16 length = CSR_BT_AMPM_MIN_CHANGE_RESPONSE_LENGTH_VALUE;

    pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU), bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMPM_CHANGE_RESPONSE, pSDU, offset,
        bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(identifier, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(length, pSDU, offset, bufferSize);

    return (pMsgBlk);
}


CsrBool CsrBtAmpmSduValidateAmpChangeResponse(
    const CsrUint16 length,
    const CsrUint8 * pSdu)
{
    CsrBool isValid = FALSE;

    CSR_BT_AMPM_ASSERT(pSdu != NULL);

    /* The SDU has to be a minimum of 4 bytes */
    if (CSR_BT_AMPM_MIN_CHANGE_RESPONSE_SDU_SIZE <= length)
    {
        CsrUint16 msgDataLength =
            CSR_BT_AMPM_DECODE_U16_IE(pSdu, CSR_BT_AMPM_SDU_LENGTH_OFFSET);

        /* The data portion of the SDU has to be 0 bytes */
        if (CSR_BT_AMPM_MIN_CHANGE_RESPONSE_LENGTH_VALUE == msgDataLength)
        {
            isValid = TRUE;
        }
    }
    return (isValid);
}


CsrMblk *CsrBtAmpmSduCreateAmpGetInfoRequest(
    CsrUint8 identifier,
    CsrBtAmpmId AMPID)
{
    CsrMblk *pMsgBlk = NULL;

    /* buffer size is always 5 bytes */
    const CsrUint16 bufferSize = CSR_BT_AMPM_MIN_GET_INFO_REQUEST_SDU_SIZE;
    CsrUint16 offset = 0;
    CsrUint8 *pSDU;
    CsrUint16 length = CSR_BT_AMPM_MIN_GET_INFO_REQUEST_LENGTH_VALUE;

    pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU), bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMPM_GET_INFO_REQUEST, pSDU, offset,
        bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(identifier, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(length, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(AMPID, pSDU, offset, bufferSize);

    return (pMsgBlk);
}


CsrBool CsrBtAmpmSduValidateAmpGetInfoRequest(
    const CsrUint16 length,
    const CsrUint8 * pSdu)
{
    CsrBool isValid = FALSE;

    CSR_BT_AMPM_ASSERT(pSdu != NULL);

    /* The SDU has to be 5 bytes */
    if (CSR_BT_AMPM_MIN_GET_INFO_REQUEST_SDU_SIZE == length)
    {
        CsrUint16 msgDataLength =
            CSR_BT_AMPM_DECODE_U16_IE(pSdu, CSR_BT_AMPM_SDU_LENGTH_OFFSET);

        /* The data portion of the SDU has to be 1 byte */
        if (CSR_BT_AMPM_MIN_GET_INFO_REQUEST_LENGTH_VALUE == msgDataLength)
        {
            isValid = TRUE;
        }
    }
    return (isValid);
}


CsrMblk *CsrBtAmpmSduCreateAmpGetInfoResponse(
    CsrUint8 identifier,
    CsrUint8 Status,
    const CsrBtAmpmDataInfo * pAMPInfo)
{
    CsrMblk *pMsgBlk = NULL;
    CsrUint16 length = CSR_BT_AMPM_MIN_GET_INFO_RESPONSE_LENGTH_VALUE;
    CsrUint16 bufferSize = CSR_BT_AMPM_MIN_GET_INFO_RESPONSE_SDU_SIZE;
    CsrUint16 offset = 0;
    CsrUint8 *pSDU;

    CSR_BT_AMPM_ASSERT(pAMPInfo != NULL);

    pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU), bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMPM_GET_INFO_RESPONSE, pSDU, offset,
        bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(identifier, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(length, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(pAMPInfo->id, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(Status, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U32_IE(pAMPInfo->totalBandwidth, pSDU, offset,
        bufferSize);
    CSR_BT_AMPM_ENCODE_U32_IE(pAMPInfo->guaranteedBandwidth, pSDU, offset,
        bufferSize);
    CSR_BT_AMPM_ENCODE_U32_IE(pAMPInfo->minLatency, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(pAMPInfo->palCapabilities, pSDU, offset,
        bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(pAMPInfo->maxHciAssocSize, pSDU, offset,
        bufferSize);

    return (pMsgBlk);
}

CsrBool CsrBtAmpmSduValidateAmpGetInfoResponse(
    const CsrUint16 length,
    const CsrUint8 * pSdu)
{
    CsrBool isValid = FALSE;

    CSR_BT_AMPM_ASSERT(pSdu != NULL);

    /* The SDU has to be 22 bytes bytes */
    if (CSR_BT_AMPM_MIN_GET_INFO_RESPONSE_SDU_SIZE == length)
    {
        CsrUint16 msgDataLength =
            CSR_BT_AMPM_DECODE_U16_IE(pSdu, CSR_BT_AMPM_SDU_LENGTH_OFFSET);

        /* The data portion of the SDU has to be 18 byte */
        if (CSR_BT_AMPM_MIN_GET_INFO_RESPONSE_LENGTH_VALUE == msgDataLength)
        {
            isValid = TRUE;
        }
    }
    return (isValid);
}


CsrMblk *CsrBtAmpmSduCreateAmpGetAmpAssocRequest(
    CsrUint8 identifier,
    CsrBtAmpmId AMPID)
{
    CsrMblk *pMsgBlk = NULL;

    /* buffer size is always 5 bytes */
    const CsrUint16 bufferSize = CSR_BT_AMPM_MIN_GET_INFO_REQUEST_SDU_SIZE;
    CsrUint16 offset = 0;
    CsrUint8 *pSDU;
    CsrUint16 length = CSR_BT_AMPM_MIN_GET_AMP_ASSOC_REQUEST_LENGTH_VALUE;

    pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU), bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMPM_GET_AMP_ASSOC_REQUEST, pSDU, offset,
        bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(identifier, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(length, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(AMPID, pSDU, offset, bufferSize);

    return (pMsgBlk);
}


CsrBool CsrBtAmpmSduValidateAmpGetAmpAssocRequest(
    const CsrUint16 length,
    const CsrUint8 * pSdu)
{
    CsrBool isValid = FALSE;

    CSR_BT_AMPM_ASSERT(pSdu != NULL);

    /* The SDU has to be 5 bytes */
    if (CSR_BT_AMPM_MIN_GET_AMP_ASSOC_REQUEST_SDU_SIZE == length)
    {
        CsrUint16 msgDataLength =
            CSR_BT_AMPM_DECODE_U16_IE(pSdu, CSR_BT_AMPM_SDU_LENGTH_OFFSET);

        /* The data portion of the SDU has to be 1 byte */
        if (CSR_BT_AMPM_MIN_GET_AMP_ASSOC_REQUEST_LENGTH_VALUE ==
            msgDataLength)
        {
            isValid = TRUE;
        }
    }
    return (isValid);
}


CsrMblk *CsrBtAmpmSduCreateAmpGetAmpAssocResponse(
    CsrUint8 identifier,
    CsrUint8 Status,
    CsrBtAmpmId ampId,
    CsrBtAmpmAmpAssoc * pAssoc)
{
    CsrMblk *pMsgBlk = NULL;

    /* Minimum if status == Invalid AMP no AMP_Assoc */
    CsrUint16 length = CSR_BT_AMPM_MIN_GET_AMP_ASSOC_RESPONSE_LENGTH_VALUE;

    /* Miminum of 6 bytes need to determine correct length */
    CsrUint16 bufferSize = CSR_BT_AMPM_MIN_GET_AMP_ASSOC_RESPONSE_SDU_SIZE;
    CsrUint16 offset = 0;
    CsrUint8 *pSDU;

    if (CSR_BT_AMPM_GET_ASSOC_RSP_STATUS_SUCCESS == Status)
    {
        CSR_BT_AMPM_ASSERT(pAssoc != NULL);
        CSR_BT_AMPM_ASSERT(pAssoc->data != NULL);
        CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_MAX_AMP_ASSOC_SIZE >= pAssoc->length);

        /* Need to add in the length of the amp assoc */
        length += pAssoc->length;
        bufferSize += pAssoc->length;
    }

    pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU), bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMPM_GET_AMP_ASSOC_RESPONSE, pSDU, offset,
        bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(identifier, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(length, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(ampId, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(Status, pSDU, offset, bufferSize);
    if (CSR_BT_AMPM_GET_ASSOC_RSP_STATUS_SUCCESS == Status)
    {
        CSR_BT_AMPM_ENCODE_DATA_IE(pAssoc->data, pSDU, offset, bufferSize, pAssoc->length);
    }

    return (pMsgBlk);
}


CsrBool CsrBtAmpmSduValidateAmpGetAmpAssocResponse(
    const CsrUint16 length,
    const CsrUint8 * pSdu)
{
    CsrBool isValid = FALSE;

    CSR_BT_AMPM_ASSERT(pSdu != NULL);

    /* The SDU has to be a minimum of 6 bytes */
    if ((CSR_BT_AMPM_MIN_GET_AMP_ASSOC_RESPONSE_SDU_SIZE) <= length)
    {
        CsrUint16 msgDataLength =
            CSR_BT_AMPM_DECODE_U16_IE(pSdu, CSR_BT_AMPM_SDU_LENGTH_OFFSET);

        /* The data portion of the SDU has to be a minimum 2 bytes and
         * AMP Assoc can not be bigger than 664 bytes
         */
        if (CSR_BT_AMPM_MIN_GET_AMP_ASSOC_REQUEST_LENGTH_VALUE <=
            msgDataLength &&
            (CSR_BT_AMPM_MIN_GET_AMP_ASSOC_REQUEST_LENGTH_VALUE +
                CSR_BT_AMPM_MAX_AMP_ASSOC_SIZE) >= msgDataLength)
        {
            isValid = TRUE;
        }
    }
    return (isValid);
}


CsrMblk *CsrBtAmpmSduCreateAmpCreatePhysicalLinkRequest(
    CsrUint8 identifier,
    CsrBtAmpmId localAmpId,
    CsrBtAmpmId remoteAmpId,
    const CsrBtAmpmAmpAssoc * pAMPAssoc)
{
    CsrMblk *pMsgBlk = NULL;

    /* Minimum need to add AMP_Assoc */
    CsrUint16 length =
        CSR_BT_AMPM_MIN_CREATE_PHYSICAL_LINK_REQUEST_LENGTH_VALUE;
    /* Miminum of 6 bytes plus AMP Assoc */
    CsrUint16 bufferSize =
        CSR_BT_AMPM_MIN_CREATE_PHYSICAL_LINK_REQUEST_SDU_SIZE;
    CsrUint16 offset = 0;
    CsrUint8 *pSDU;

    CSR_BT_AMPM_ASSERT(pAMPAssoc != NULL);
    CSR_BT_AMPM_ASSERT(pAMPAssoc->data != NULL);
    CSR_BT_AMPM_ASSERT(CSR_BT_AMPM_MAX_AMP_ASSOC_SIZE >= pAMPAssoc->length);

    length += pAMPAssoc->length;
    bufferSize += pAMPAssoc->length;

    pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU), bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMPM_CREATE_PHYSICAL_LINK_REQUEST, pSDU,
                             offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(identifier, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(length, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(localAmpId, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(remoteAmpId, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_DATA_IE(pAMPAssoc->data, pSDU, offset, bufferSize, pAMPAssoc->length);

    return (pMsgBlk);
}

CsrBool CsrBtAmpmSduValidateAmpCreatePhysicalLinkRequest(
    const CsrUint16 length,
    const CsrUint8 * pSdu)
{
    CsrBool isValid = FALSE;

    CSR_BT_AMPM_ASSERT(pSdu != NULL);

    /* The SDU has to be a minimum of 6 bytes */
    if ((CSR_BT_AMPM_MIN_CREATE_PHYSICAL_LINK_REQUEST_SDU_SIZE) <= length)
    {
        CsrUint16 msgDataLength =
            CSR_BT_AMPM_DECODE_U16_IE(pSdu, CSR_BT_AMPM_SDU_LENGTH_OFFSET);

        /* The data portion of the SDU has to be a minimum 2 byte and
         * AMP Assoc can not be bigger than 664 bytes
         */
        if (CSR_BT_AMPM_MIN_CREATE_PHYSICAL_LINK_REQUEST_LENGTH_VALUE <=
            msgDataLength &&
            (CSR_BT_AMPM_MIN_CREATE_PHYSICAL_LINK_REQUEST_LENGTH_VALUE +
                CSR_BT_AMPM_MAX_AMP_ASSOC_SIZE) >= msgDataLength)
        {
            isValid = TRUE;
        }
    }
    return (isValid);
}

CsrMblk *CsrBtAmpmSduCreateAmpCreatePhysicalLinkResponse(
    CsrUint8 identifier,
    CsrBtAmpmId localAmpId,
    CsrBtAmpmId remoteAmpId,
    CsrUint8 Status)
{
    CsrMblk *pMsgBlk = NULL;

    /* Complete SDU is 6 bytes */
    const CsrUint16 bufferSize =
        CSR_BT_AMPM_MIN_CREATE_PHYSICAL_LINK_RESPONSE_SDU_SIZE;
    /* 2 bytes */
    CsrUint16 length =
        CSR_BT_AMPM_MIN_CREATE_PHYSICAL_LINK_RESPONSE_LENGTH_VALUE;
    CsrUint16 offset = 0;
    CsrUint8 *pSDU;

    pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU), bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMPM_CREATE_PHYSICAL_LINK_RESPONSE, pSDU,
        offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(identifier, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(length, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(localAmpId, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(remoteAmpId, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(Status, pSDU, offset, bufferSize);

    return (pMsgBlk);
}


CsrBool CsrBtAmpmSduValidateAmpCreatePhysicalLinkResponse(
    const CsrUint16 length,
    const CsrUint8 * pSdu)
{
    CsrBool isValid = FALSE;

    CSR_BT_AMPM_ASSERT(pSdu != NULL);

    /* The SDU has to be 7 bytes */
    if (CSR_BT_AMPM_MIN_CREATE_PHYSICAL_LINK_RESPONSE_SDU_SIZE == length)
    {
        CsrUint16 msgDataLength =
            CSR_BT_AMPM_DECODE_U16_IE(pSdu, CSR_BT_AMPM_SDU_LENGTH_OFFSET);

        /* The data portion of the SDU has to be 3 bytes */
        if (CSR_BT_AMPM_MIN_CREATE_PHYSICAL_LINK_RESPONSE_LENGTH_VALUE ==
            msgDataLength)
        {
            isValid = TRUE;
        }
    }
    return (isValid);
}


CsrMblk *CsrBtAmpmSduCreateAmpDisconnectPhysicalLinkRequest(
    CsrUint8 identifier,
    CsrBtAmpmId localAmpId,
    CsrBtAmpmId remoteAmpId)
{
    CsrMblk *pMsgBlk = NULL;

    /* SDU Size is 5 bytes */
    const CsrUint16 bufferSize =
        CSR_BT_AMPM_MIN_DISCONNECT_PHYSICAL_LINK_REQUEST_SDU_SIZE;
    CsrUint16 length =
        CSR_BT_AMPM_MIN_DISCONNECT_PHYSICAL_LINK_REQUEST_LENGTH_VALUE;
    CsrUint16 offset = 0;
    CsrUint8 *pSDU;

    pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU), bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMPM_DISCONNECT_PHYSICAL_LINK_REQUEST,
        pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(identifier, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(length, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(localAmpId, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(remoteAmpId, pSDU, offset, bufferSize);

    return (pMsgBlk);
}

CsrBool CsrBtAmpmSduValidateAmpDisconnectPhysicalLinkRequest(
    const CsrUint16 length,
    const CsrUint8 * pSdu)
{
    CsrBool isValid = FALSE;

    CSR_BT_AMPM_ASSERT(pSdu != NULL);

    /* The SDU has to be 6 bytes */
    if (CSR_BT_AMPM_MIN_DISCONNECT_PHYSICAL_LINK_REQUEST_SDU_SIZE == length)
    {
        CsrUint16 msgDataLength =
            CSR_BT_AMPM_DECODE_U16_IE(pSdu, CSR_BT_AMPM_SDU_LENGTH_OFFSET);

        /* The data portion of the SDU has to be 2 byte */
        if (CSR_BT_AMPM_MIN_DISCONNECT_PHYSICAL_LINK_REQUEST_LENGTH_VALUE ==
            msgDataLength)
        {
            isValid = TRUE;
        }
    }
    return (isValid);
}

CsrMblk *CsrBtAmpmSduCreateAmpDisconnectPhysicalLinkResponse(
    CsrUint8 identifier,
    CsrBtAmpmId localAmpId,
    CsrBtAmpmId remoteAmpId,
    CsrUint8 Status)
{
    CsrMblk *pMsgBlk = NULL;

    /* SDU Size is 6 bytes */
    const CsrUint16 bufferSize =
        CSR_BT_AMPM_MIN_DISCONNECT_PHYSICAL_LINK_RESPONSE_SDU_SIZE;
    CsrUint16 length =
        CSR_BT_AMPM_MIN_DISCONNECT_PHYSICAL_LINK_RESPONSE_LENGTH_VALUE;
    CsrUint16 offset = 0;
    CsrUint8 *pSDU;

    pMsgBlk = CsrMblkMallocCreate((void *) &(pSDU), bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(CSR_BT_AMPM_DISCONNECT_PHYSICAL_LINK_RESPONSE,
        pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(identifier, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U16_IE(length, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(localAmpId, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(remoteAmpId, pSDU, offset, bufferSize);
    CSR_BT_AMPM_ENCODE_U8_IE(Status, pSDU, offset, bufferSize);

    return (pMsgBlk);
}


CsrBool CsrBtAmpmSduValidateAmpDisconnectPhysicalLinkResponse(
    const CsrUint16 length,
    const CsrUint8 * pSdu)
{
    CsrBool isValid = FALSE;

    CSR_BT_AMPM_ASSERT(pSdu != NULL);

    /* The SDU has to be 7 bytes */
    if (CSR_BT_AMPM_MIN_DISCONNECT_PHYSICAL_LINK_RESPONSE_SDU_SIZE == length)
    {
        CsrUint16 msgDataLength =
            CSR_BT_AMPM_DECODE_U16_IE(pSdu, CSR_BT_AMPM_SDU_LENGTH_OFFSET);

        /* The data portion of the SDU has to be 3 bytes */
        if (CSR_BT_AMPM_MIN_DISCONNECT_PHYSICAL_LINK_RESPONSE_LENGTH_VALUE ==
            msgDataLength)
        {
            isValid = TRUE;
        }
    }
    return (isValid);
}

void CsrBtAmpmSduDecodeAmpGetInfoResponse(
    const CsrUint8 * pAmpGetInfoRsp,
    CsrUint8 * pSduIdentifier,
    CsrUint8 * pStatus,
    CsrBtAmpmDataInfo * pAMPInfo)
{
    CSR_BT_AMPM_ASSERT(pAmpGetInfoRsp != NULL);
    CSR_BT_AMPM_ASSERT(pSduIdentifier != NULL);
    CSR_BT_AMPM_ASSERT(pStatus != NULL);
    CSR_BT_AMPM_ASSERT(pAMPInfo != NULL);

    *pSduIdentifier =
        CSR_BT_AMPM_DECODE_U8_IE(pAmpGetInfoRsp,
        CSR_BT_AMPM_SDU_IDENTIFIER_OFFSET);
    *pStatus =
        CSR_BT_AMPM_DECODE_U8_IE(pAmpGetInfoRsp,
        CSR_BT_AMPM_GET_INFO_RSP_STATUS_OFFSET);
    pAMPInfo->id =
        CSR_BT_AMPM_DECODE_U8_IE(pAmpGetInfoRsp,
        CSR_BT_AMPM_GET_INFO_RSP_AMP_ID_OFFSET);

    if (CSR_BT_AMPM_GET_INFO_RSP_STATUS_SUCCESS == *pStatus)
    {
        pAMPInfo->totalBandwidth =
            CSR_BT_AMPM_DECODE_U32_IE(pAmpGetInfoRsp,
            CSR_BT_AMPM_GET_INFO_RSP_TOTAL_BW_OFFSET);
        pAMPInfo->guaranteedBandwidth =
            CSR_BT_AMPM_DECODE_U32_IE(pAmpGetInfoRsp,
            CSR_BT_AMPM_GET_INFO_RSP_GUARANTEED_BW_OFFSET);
        pAMPInfo->minLatency =
            CSR_BT_AMPM_DECODE_U32_IE(pAmpGetInfoRsp,
            CSR_BT_AMPM_GET_INFO_RSP_MIN_LATENCY_OFFSET);
        pAMPInfo->palCapabilities =
            CSR_BT_AMPM_DECODE_U16_IE(pAmpGetInfoRsp,
            CSR_BT_AMPM_GET_INFO_RSP_PAL_CAPABILITIES_OFFSET);
        pAMPInfo->maxHciAssocSize =
            CSR_BT_AMPM_DECODE_U16_IE(pAmpGetInfoRsp,
            CSR_BT_AMPM_GET_INFO_RSP_AMP_ASSOC_LEN_OFFSET);
    }
}

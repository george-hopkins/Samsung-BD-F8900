/****************************************************************************

        Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
                All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_hand_coded.h"
#include "csr_bt_profiles.h"
#include "csr_mblk.h"
#include "csr_bt_log_version.h"
#include "csr_bt_converter_utils.h"

/* ------------------------------ Helpers ------------------------------ */

/* CsrBtSdDeviceInfoType length counter */
static CsrUint32 countInfoBytes(CsrUint32 devCount,
                                CsrBtSdDeviceInfoType *dev)
{
    CsrUint16 i;
    CsrUint32 infoLen = 0;
    for (i = 0; i < devCount; i++)
    {
        infoLen += dev[i].infoLen;
    }
    return infoLen;
}

/* HCI name part length counter */
static CsrUint16 namePartBytes(CsrUint8 **part)
{
    CsrUint16 i;

    for(i=0; i<HCI_NAME_LENGTH; i++)
    {
        if(part[i/HCI_LOCAL_NAME_BYTES_PER_PTR] == NULL)
        {
            i++;
            break;
        }
    }
    return i;
}

#ifdef CSR_BT_LE_ENABLE
/* CsrBtGattAttrWritePairs length counter */
static CsrUint32 countAttrValueBytes(CsrUint16               devCount,
                                     CsrBtGattAttrWritePairs *dev)
{
    CsrUint16 i;
    CsrUint32 attrValueLen = 0;

    for (i = 0; i < devCount; i++)
    {
        attrValueLen += dev[i].valueLength;
    }
    return attrValueLen;
}
#endif

void namePartSerialise(CsrUint8 *ptr,
                       CsrSize *length,
                       CsrUint8 **part)
{
    CsrUint8 i;

    CsrUint16Ser(ptr, length, (CsrUint16) namePartBytes(part));
    
    for(i=0; i<HCI_NAME_LENGTH; i++)
    {
        if(part[i/HCI_LOCAL_NAME_BYTES_PER_PTR] == NULL)
        {
            break;
        }
        else
        {
            ptr[*length] = *part[i/HCI_LOCAL_NAME_BYTES_PER_PTR];
            (*length)++;
        }
    }
}

void namePartDeserialise(CsrUint8 *buffer,
                         CsrSize *offset,
                         CsrUint8 **part)
{
    CsrUint16 partLength;
    CsrUint8 i, j;

    CsrUint16Des((CsrUint16 *) &partLength, buffer, offset);
    
    for(i=0, j=0; i<partLength; i++)
    {
        if(i % HCI_LOCAL_NAME_BYTES_PER_PTR == 0)
        {
            part[i/HCI_LOCAL_NAME_BYTES_PER_PTR] = CsrPmemZalloc(HCI_LOCAL_NAME_BYTES_PER_PTR);
            j++;
        }
        part[i/HCI_LOCAL_NAME_BYTES_PER_PTR][i%HCI_LOCAL_NAME_BYTES_PER_PTR] = buffer[*offset];
        (*offset)++;
    }
    for(; j<HCI_LOCAL_NAME_BYTE_PACKET_PTRS; j++)
    {
        part[j] = NULL;
    }
}

void namePartFree(CsrUint8 **part)
{
    CsrUint8 i;
    for(i=0; i<HCI_LOCAL_NAME_BYTE_PACKET_PTRS && part[i] != NULL; i++)
    {
        CsrPmemFree(part[i]);
    }
}

void iacPartSerialise(CsrUint8 *ptr,
                      CsrSize *length,
                      CsrUint24 **part,
                      CsrUint8 numIac)
{
    CsrUint8 i;
    for(i=0; i<numIac; i++)
    {
        CsrUint32Ser(ptr, length, (CsrUint24)part[i/HCI_IAC_LAP_PER_PTR][i%HCI_IAC_LAP_PER_PTR]);
    }
}

void iacPartDeserialise(CsrUint8 *buffer,
                        CsrSize *offset,
                        CsrUint24 **part,
                        CsrUint8 numIac)
{
    CsrUint8 i, j;

    for(i=0, j=0; i<numIac; i++)
    {
        if(i % HCI_IAC_LAP_PER_PTR == 0)
        {
            part[i/HCI_IAC_LAP_PER_PTR] = CsrPmemZalloc(HCI_IAC_LAP_PER_PTR * sizeof(CsrUint24));
            j++;
        }
        part[i/HCI_IAC_LAP_PER_PTR][i%HCI_IAC_LAP_PER_PTR] = (CsrUint24)buffer[*offset];
        *offset += sizeof(CsrUint24);
    }
    for(; j<HCI_IAC_LAP_PTRS; j++)
    {
        part[j] = NULL;
    }
}

void iacPartFree(CsrUint24 **part,
                 CsrUint8 numIac)
{
    CsrUint8 i;
    for(i=0; i<numIac; i++)
    {
        CsrPmemFree(part[i]);
    }
}

static void eirPartSerialise(CsrUint8 *buffer,
                             CsrSize *offset,
                             CsrUint8 **part)
{
    CsrUint8 i;
    for(i=0; i<HCI_EIR_DATA_LENGTH; i++)
    {
        if (part[i/HCI_EIR_DATA_BYTES_PER_PTR] != NULL)
        {
            buffer[*offset] = part[i/HCI_EIR_DATA_BYTES_PER_PTR][i%HCI_EIR_DATA_BYTES_PER_PTR];
        }
        else
        {
            buffer[*offset] = 0;
        }
        (*offset)++;
    }
}

static void eirPartDeserialise(CsrUint8 *buffer,
                               CsrSize *offset,
                               CsrUint8 **part)
{
    CsrUint8 i;
    for(i=0; i<HCI_EIR_DATA_LENGTH; i++)
    {
        if(i%HCI_EIR_DATA_BYTES_PER_PTR == 0)
        {
            part[i/HCI_EIR_DATA_BYTES_PER_PTR] = CsrPmemZalloc(HCI_EIR_DATA_BYTES_PER_PTR);
        }
        part[i/HCI_EIR_DATA_BYTES_PER_PTR][i%HCI_EIR_DATA_BYTES_PER_PTR] = buffer[*offset];
        (*offset)++;
    }
}

static void eirPartFree(CsrUint8 **part)
{
    CsrUint8 i;
    for(i=0; i<HCI_EIR_DATA_PACKET_PTRS && part[i] != NULL; i++)
    {
        CsrPmemFree(part[i]);
    }
}

/* ------------------------------ CM-BNEP ------------------------------ */

/* CsrBtCmBnepConnectReq */
CsrSize CsrBtCmBnepConnectReqSizeof(void *voidPrimitivePointer)
{
    CsrBtCmBnepConnectReq *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
void CsrBtCmBnepConnectReqSerFree(void *voidPrimitivePointer)
{
    CsrBtCmBnepConnectReq *primitive;
    primitive = (CsrBtCmBnepConnectReq *) voidPrimitivePointer;

    CsrPmemFree(primitive);
}

CsrUint8 *CsrBtCmBnepConnectReqSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    CsrBtCmBnepConnectReq *primitive;

    primitive = (CsrBtCmBnepConnectReq *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrMemCpySer(ptr, length, ((const void *) &primitive->flags), sizeof(primitive->flags));
    CsrMemCpySer(ptr, length, ((const void *) primitive->rem_addr.w), ((CsrUint32) 2 * (3)));
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->profileMaxFrameSize);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->secLevel);

    return ptr;
}
void *CsrBtCmBnepConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepConnectReq *) CsrPmemZalloc(sizeof(CsrBtCmBnepConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->flags), buffer, &offset, sizeof(primitive->flags));
    CsrMemCpyDes(((void *) primitive->rem_addr.w), buffer, &offset, ((CsrUint32) 2 * (3)));
    CsrUint16Des((CsrUint16 *) &primitive->profileMaxFrameSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}

/* CsrBtCmBnepConnectAcceptReq */
CsrSize CsrBtCmBnepConnectAcceptReqSizeof(void *voidPrimitivePointer)
{
    CsrBtCmBnepConnectAcceptReq *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
void CsrBtCmBnepConnectAcceptReqSerFree(void *voidPrimitivePointer)
{
    CsrBtCmBnepConnectAcceptReq *primitive;
    primitive = (CsrBtCmBnepConnectAcceptReq *) voidPrimitivePointer;

    CsrPmemFree(primitive);
}
CsrUint8 *CsrBtCmBnepConnectAcceptReqSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    CsrBtCmBnepConnectAcceptReq *primitive;

    primitive = (CsrBtCmBnepConnectAcceptReq *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrMemCpySer(ptr, length, ((const void *) &primitive->flags), sizeof(primitive->flags));
    CsrMemCpySer(ptr, length, ((const void *) primitive->rem_addr.w), ((CsrUint32) 2 * (3)));
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->secLevel);
    CsrUint32Ser(ptr, length, (CsrUint32) primitive->classOfDevice);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->profileMaxFrameSize);

    return ptr;
}
void *CsrBtCmBnepConnectAcceptReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepConnectAcceptReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepConnectAcceptReq *) CsrPmemZalloc(sizeof(CsrBtCmBnepConnectAcceptReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->flags), buffer, &offset, sizeof(primitive->flags));
    CsrMemCpyDes(((void *) primitive->rem_addr.w), buffer, &offset, ((CsrUint32) 2 * (3)));
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->classOfDevice, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileMaxFrameSize, buffer, &offset);

    return primitive;
}

/* ------------------------------ SD ------------------------------ */

CsrSize CsrBtSdReadDeviceListIndSizeof(void *voidPrimitivePointer)
{
    CsrBtSdReadDeviceListInd *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(CsrBtSdReadDeviceListInd)
                + primitive->deviceInfoCount*sizeof(CsrBtSdDeviceInfoType)
                + countInfoBytes(primitive->deviceInfoCount,
                                 primitive->deviceInfo));
    }
    else
    {
        return 0;
    }
}

CsrUint8 *CsrBtSdReadDeviceListIndSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    CsrUint32 i;
    CsrBtSdReadDeviceListInd * primitive;

    primitive = (CsrBtSdReadDeviceListInd *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16)primitive->type);
    CsrUint32Ser(ptr, length, (CsrUint32)primitive->deviceInfoCount);

    if (primitive->deviceInfoCount > 0)
    {
        for (i=0;i<primitive->deviceInfoCount;i++)
        {
            CsrUint16Ser(ptr, length, (CsrUint16)primitive->deviceInfo[i].deviceAddr.nap);
            CsrUint8Ser(ptr, length, (CsrUint8)primitive->deviceInfo[i].deviceAddr.uap);
            CsrUint32Ser(ptr, length, (CsrUint32)primitive->deviceInfo[i].deviceAddr.lap);
            CsrUint32Ser(ptr, length, (CsrUint32)primitive->deviceInfo[i].deviceClass);
            CsrUint16Ser(ptr, length, (CsrUint16)primitive->deviceInfo[i].infoLen);
            CsrMemCpySer(ptr, length,
                         (const void *) primitive->deviceInfo[i].info,
                         primitive->deviceInfo[i].infoLen);
            CsrUint32Ser(ptr, length, (CsrUint32)primitive->deviceInfo[i].deviceStatus);
        }
    }
    return ptr;
}

void *CsrBtSdReadDeviceListIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadDeviceListInd * primitive;
    CsrUint32 i;
    CsrSize offset;

    length = length;
    offset = 0;
    primitive = (CsrBtSdReadDeviceListInd *) CsrPmemAlloc(sizeof(CsrBtSdReadDeviceListInd));

    CsrUint16Des((CsrUint16 *)&primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *)&primitive->deviceInfoCount, buffer, &offset);

    if (primitive->deviceInfoCount > 0)
    {
        primitive->deviceInfo = CsrPmemAlloc((CsrUint32) primitive->deviceInfoCount*sizeof(CsrBtSdDeviceInfoType));

        for (i=0;i<primitive->deviceInfoCount;i++)
        {
            CsrUint16Des((CsrUint16 *)&primitive->deviceInfo[i].deviceAddr.nap, buffer, &offset);
            CsrUint8Des((CsrUint8 *)&primitive->deviceInfo[i].deviceAddr.uap, buffer, &offset);
            CsrUint32Des((CsrUint32 *)&primitive->deviceInfo[i].deviceAddr.lap, buffer, &offset);
            CsrUint32Des((CsrUint32 *)&primitive->deviceInfo[i].deviceClass, buffer, &offset);
            CsrUint16Des((CsrUint16 *)&primitive->deviceInfo[i].infoLen, buffer, &offset);
            primitive->deviceInfo[i].info = CsrPmemAlloc(primitive->deviceInfo[i].infoLen);
            CsrMemCpyDes((void *) primitive->deviceInfo[i].info, buffer, &offset, 
                         primitive->deviceInfo[i].infoLen);
            CsrUint32Des((CsrUint32 *)&primitive->deviceInfo[i].deviceStatus, buffer, &offset);
        }
    }
    else
    {
        primitive->deviceInfo = NULL;
    }
    return ((void *)primitive);
}

void CsrBtSdReadDeviceListIndSerFree(void *voidPrimitivePointer)
{
    CsrBtSdReadDeviceListInd * primitive;
    primitive = (CsrBtSdReadDeviceListInd *)voidPrimitivePointer;

    if (primitive && primitive->deviceInfoCount > 0)
    {
        CsrUint32 i;
        for (i = 0; i < primitive->deviceInfoCount; i++)
        {
            if (primitive->deviceInfo[i].infoLen > 0)
            {
                CsrPmemFree(primitive->deviceInfo[i].info);
            }
        }
        CsrPmemFree(primitive->deviceInfo);
    }

    CsrPmemFree(primitive);
}


CsrSize CsrBtSdReadDeviceListCfmSizeof(void *voidPrimitivePointer)
{
    CsrBtSdReadDeviceListCfm *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(CsrBtSdReadDeviceListCfm)
                + primitive->deviceInfoCount*sizeof(CsrBtSdDeviceInfoType)
                + countInfoBytes(primitive->deviceInfoCount,
                                 primitive->deviceInfo));
    }
    else
    {
        return 0;
    }
}

CsrUint8 *CsrBtSdReadDeviceListCfmSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    CsrUint32 i;
    CsrBtSdReadDeviceListCfm * primitive;

    primitive = (CsrBtSdReadDeviceListCfm *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16)primitive->type);
    CsrUint32Ser(ptr, length, (CsrUint32)primitive->deviceInfoCount);
    CsrUint32Ser(ptr, length, (CsrUint32)primitive->totalNumOfDevices);

    if (primitive->deviceInfoCount > 0)
    {
        for (i=0;i<primitive->deviceInfoCount;i++)
        {
            CsrUint16Ser(ptr, length, (CsrUint16)primitive->deviceInfo[i].deviceAddr.nap);
            CsrUint8Ser(ptr, length, (CsrUint8)primitive->deviceInfo[i].deviceAddr.uap);
            CsrUint32Ser(ptr, length, (CsrUint32)primitive->deviceInfo[i].deviceAddr.lap);
            CsrUint32Ser(ptr, length, (CsrUint32)primitive->deviceInfo[i].deviceClass);
            CsrUint16Ser(ptr, length, (CsrUint16)primitive->deviceInfo[i].infoLen);
            CsrMemCpySer(ptr, length,
                         (const void *) primitive->deviceInfo[i].info,
                         primitive->deviceInfo[i].infoLen);
            CsrUint32Ser(ptr, length, (CsrUint32)primitive->deviceInfo[i].deviceStatus);
        }
    }
    return ptr;
}

void *CsrBtSdReadDeviceListCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadDeviceListCfm * primitive;
    CsrUint32 i;
    CsrSize offset;

    length = length;

    offset = 0;
    primitive = (CsrBtSdReadDeviceListCfm *) CsrPmemAlloc(sizeof(CsrBtSdReadDeviceListCfm));

    CsrUint16Des((CsrUint16 *)&primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *)&primitive->deviceInfoCount, buffer, &offset);
    CsrUint32Des((CsrUint32 *)&primitive->totalNumOfDevices, buffer, &offset);

    if (primitive->deviceInfoCount > 0)
    {
        primitive->deviceInfo = CsrPmemAlloc((CsrUint32) primitive->deviceInfoCount*sizeof(CsrBtSdDeviceInfoType));

        for (i=0;i<primitive->deviceInfoCount;i++)
        {
            CsrUint16Des((CsrUint16 *)&primitive->deviceInfo[i].deviceAddr.nap, buffer, &offset);
            CsrUint8Des((CsrUint8 *)&primitive->deviceInfo[i].deviceAddr.uap, buffer, &offset);
            CsrUint32Des((CsrUint32 *)&primitive->deviceInfo[i].deviceAddr.lap, buffer, &offset);
            CsrUint32Des((CsrUint32 *)&primitive->deviceInfo[i].deviceClass, buffer, &offset);
            CsrUint16Des((CsrUint16 *)&primitive->deviceInfo[i].infoLen, buffer, &offset);
            primitive->deviceInfo[i].info = CsrPmemAlloc(primitive->deviceInfo[i].infoLen);
            CsrMemCpyDes((void *) primitive->deviceInfo[i].info, buffer, &offset, 
                         primitive->deviceInfo[i].infoLen);
            CsrUint32Des((CsrUint32 *)&primitive->deviceInfo[i].deviceStatus, buffer, &offset);
        }
    }
    else
    {
        primitive->deviceInfo = NULL;
    }
    return ((void *)primitive);
}

void CsrBtSdReadDeviceListCfmSerFree(void *voidPrimitivePointer)
{
    CsrBtSdReadDeviceListCfm * primitive;
    primitive = (CsrBtSdReadDeviceListCfm *)voidPrimitivePointer;

    if (primitive && primitive->deviceInfoCount > 0)
    {
        CsrUint32 i;
        for (i = 0; i < primitive->deviceInfoCount; i++)
        {
            if (primitive->deviceInfo[i].infoLen > 0)
            {
                CsrPmemFree(primitive->deviceInfo[i].info);
            }
        }
        CsrPmemFree(primitive->deviceInfo);
    }

    CsrPmemFree(primitive);
}

/* ------------------------------ DM ------------------------------ */

/* DM_SM_LINK_KEY_REQUEST_RSP */
CsrUint8 *DM_SM_LINK_KEY_REQUEST_RSP_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_SM_LINK_KEY_REQUEST_RSP_T *primitive;

    primitive = (DM_SM_LINK_KEY_REQUEST_RSP_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint32Ser(ptr, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->key_type);
    if(primitive->key_type != DM_SM_LINK_KEY_NONE)
    {
        CsrMemCpySer(ptr, length, ((const void *) primitive->key), SIZE_LINK_KEY);
    }

    return ptr;
}
void *DM_SM_LINK_KEY_REQUEST_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_LINK_KEY_REQUEST_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_LINK_KEY_REQUEST_RSP_T*) CsrPmemZalloc(sizeof(DM_SM_LINK_KEY_REQUEST_RSP_T));
    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->key_type, buffer, &offset);
    if(primitive->key_type != DM_SM_LINK_KEY_NONE)
    {
        CsrMemCpyDes(((void *) primitive->key), buffer, &offset, SIZE_LINK_KEY);
    }
    else
    {
        primitive->key = NULL;
    }

    return primitive;
}
CsrSize DM_SM_LINK_KEY_REQUEST_RSP_TSizeof(void *voidPrimitivePointer)
{
    DM_SM_LINK_KEY_REQUEST_RSP_T *primitive = voidPrimitivePointer;
    if (primitive)
    {
        return (sizeof(DM_SM_LINK_KEY_REQUEST_RSP_T) +
                ((primitive->key_type != DM_SM_LINK_KEY_NONE) ? SIZE_LINK_KEY : 0));
    }
    else
    {
        return 0;
    }
}
void DM_SM_LINK_KEY_REQUEST_RSP_TSerFree(void *voidPrimitivePointer)
{
    DM_SM_LINK_KEY_REQUEST_RSP_T *primitive;
    primitive = (DM_SM_LINK_KEY_REQUEST_RSP_T*)voidPrimitivePointer;

    if(primitive && primitive->key_type != DM_SM_LINK_KEY_NONE)
    {
        CsrPmemFree(primitive->key);
    }
    CsrPmemFree(primitive);
}

/* DM_SM_IO_CAPABILITY_REQUEST_RSP */
CsrSize DM_SM_IO_CAPABILITY_REQUEST_RSP_TSizeof(void *voidPrimitivePointer)
{
    DM_SM_IO_CAPABILITY_REQUEST_RSP_T *primitive = voidPrimitivePointer;
    if (primitive)
    {
        return (sizeof(DM_SM_IO_CAPABILITY_REQUEST_RSP_T) +
                (primitive->oob_data_present ? 2 * SIZE_OOB_DATA : 0));
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_SM_IO_CAPABILITY_REQUEST_RSP_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_SM_IO_CAPABILITY_REQUEST_RSP_T *primitive;

    primitive = (DM_SM_IO_CAPABILITY_REQUEST_RSP_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(ptr, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->io_capability);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->oob_data_present);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->authentication_requirements);
    if(primitive->oob_data_present)
    {
        CsrMemCpySer(ptr, length, ((const void *) primitive->oob_hash_c), SIZE_OOB_DATA);
        CsrMemCpySer(ptr, length, ((const void *) primitive->oob_rand_r), SIZE_OOB_DATA);
        
    }
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->key_distribution);

    return ptr;
}
void *DM_SM_IO_CAPABILITY_REQUEST_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_IO_CAPABILITY_REQUEST_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_IO_CAPABILITY_REQUEST_RSP_T*) CsrPmemZalloc(sizeof(DM_SM_IO_CAPABILITY_REQUEST_RSP_T));
    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->io_capability, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->oob_data_present, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authentication_requirements, buffer, &offset);
    if(primitive->oob_data_present)
    {
        primitive->oob_hash_c = CsrPmemAlloc(SIZE_OOB_DATA);
        primitive->oob_rand_r = CsrPmemAlloc(SIZE_OOB_DATA);
        CsrMemCpyDes(((void *) primitive->oob_hash_c), buffer, &offset, SIZE_OOB_DATA);
        CsrMemCpyDes(((void *) primitive->oob_rand_r), buffer, &offset, SIZE_OOB_DATA);
    }
    else
    {
        primitive->oob_hash_c = NULL;
        primitive->oob_rand_r = NULL;
    }

    CsrUint8Des((CsrUint8 *) &primitive->key_distribution, buffer, &offset);

    return primitive;
}
void DM_SM_IO_CAPABILITY_REQUEST_RSP_TSerFree(void *voidPrimitivePointer)
{
    DM_SM_IO_CAPABILITY_REQUEST_RSP_T *primitive;
    primitive = (DM_SM_IO_CAPABILITY_REQUEST_RSP_T *) voidPrimitivePointer;

    if(primitive && primitive->oob_data_present)
    {
        CsrPmemFree(primitive->oob_hash_c);
        CsrPmemFree(primitive->oob_rand_r);
    }
    CsrPmemFree(primitive);
}

/* DM_SM_READ_LOCAL_OOB_DATA_CFM */
CsrSize DM_SM_READ_LOCAL_OOB_DATA_CFM_TSizeof(void *voidPrimitivePointer)
{
    DM_SM_READ_LOCAL_OOB_DATA_CFM_T *primitive = voidPrimitivePointer;
    if (primitive)
    {
        return (sizeof(DM_SM_READ_LOCAL_OOB_DATA_CFM_T) +
                2 * SIZE_OOB_DATA);
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_SM_READ_LOCAL_OOB_DATA_CFM_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_SM_READ_LOCAL_OOB_DATA_CFM_T *primitive;

    primitive = (DM_SM_READ_LOCAL_OOB_DATA_CFM_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->status);
    if (primitive->status == HCI_SUCCESS)
    {
        CsrMemCpySer(ptr, length, ((const void *) primitive->oob_hash_c), SIZE_OOB_DATA);
        CsrMemCpySer(ptr, length, ((const void *) primitive->oob_rand_r), SIZE_OOB_DATA);
    }
    else
    {
        CsrMemSet(&ptr[*length], 0, SIZE_OOB_DATA * 2);
        *length += SIZE_OOB_DATA * 2;
    }

    return ptr;
}

void *DM_SM_READ_LOCAL_OOB_DATA_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_READ_LOCAL_OOB_DATA_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_SM_READ_LOCAL_OOB_DATA_CFM_T*) CsrPmemZalloc(sizeof(DM_SM_READ_LOCAL_OOB_DATA_CFM_T));
    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    if (primitive->status == HCI_SUCCESS)
    {
        primitive->oob_hash_c = CsrPmemAlloc(SIZE_OOB_DATA);
        primitive->oob_rand_r = CsrPmemAlloc(SIZE_OOB_DATA);
        CsrMemCpyDes(((void *) primitive->oob_hash_c), buffer, &offset, SIZE_OOB_DATA);
        CsrMemCpyDes(((void *) primitive->oob_rand_r), buffer, &offset, SIZE_OOB_DATA);
    }
    else
    {
        primitive->oob_hash_c = NULL;
        primitive->oob_rand_r = NULL;
    }

    return primitive;
}

void DM_SM_READ_LOCAL_OOB_DATA_CFM_TSerFree(void *voidPrimitivePointer)
{
    DM_SM_READ_LOCAL_OOB_DATA_CFM_T *primitive;
    primitive = (DM_SM_READ_LOCAL_OOB_DATA_CFM_T *) voidPrimitivePointer;

    if(primitive)
    {
        CsrPmemFree(primitive->oob_hash_c);
        CsrPmemFree(primitive->oob_rand_r);
    }
    CsrPmemFree(primitive);
}

/* DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ */
CsrSize DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_TSizeof(void *voidPrimitivePointer)
{
    DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_T) +
                primitive->number_of_handles * sizeof(DM_AMPM_NCB_T));
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_T *primitive;
    CsrUint8 i;

    primitive = (DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->local_amp_id);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->total_num_data_blocks);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->number_of_handles);
    for(i=0; i<primitive->number_of_handles; i++)
    {
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->num_completed_blks_ptr[i]->logical_link_handle);
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->num_completed_blks_ptr[i]->num_of_completed_packets);
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->num_completed_blks_ptr[i]->num_of_completed_blocks);
    }

    return ptr;
}

void *DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_T *primitive;
    CsrSize offset;
    CsrUint8 i;

    offset = 0;
    primitive = (DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_T *) CsrPmemZalloc(sizeof(DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_amp_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->total_num_data_blocks, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->number_of_handles, buffer, &offset);
    for(i=0; i<primitive->number_of_handles; i++)
    {
        primitive->num_completed_blks_ptr[i] = CsrPmemAlloc(sizeof(DM_AMPM_NCB_T));
        CsrUint16Des((CsrUint16 *) &primitive->num_completed_blks_ptr[i]->logical_link_handle, buffer, &offset);
        CsrUint16Des((CsrUint16 *) &primitive->num_completed_blks_ptr[i]->num_of_completed_packets, buffer, &offset);
        CsrUint16Des((CsrUint16 *) &primitive->num_completed_blks_ptr[i]->num_of_completed_blocks, buffer, &offset);
    }

    return primitive;
}

void DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_TSerFree(void *voidPrimitivePointer)
{
    DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_T *primitive;
    CsrUint8 i;

    primitive = (DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_T *) voidPrimitivePointer;
    for(i=0; i<primitive->number_of_handles; i++)
    {
        CsrPmemFree(primitive->num_completed_blks_ptr[i]);
    }
    CsrPmemFree(primitive);
}


/* ------------------------------ SM ------------------------------ */

CsrUint8 *DM_SM_KEYS_IND_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_SM_KEYS_IND_T *primitive;
    CsrUint8 i;
    CsrUint16 present;

    primitive = (DM_SM_KEYS_IND_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(ptr, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->keys.security_requirements);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->keys.encryption_key_size);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->keys.present);

    for(i=0, present = primitive->keys.present;
        i<DM_SM_MAX_NUM_KEYS;
        i++, present >>= DM_SM_NUM_KEY_BITS)
    {
        switch(present & 0x7)
        {
            case DM_SM_KEY_NONE:
                /* no key, ignore */
                break;

            case DM_SM_KEY_ENC_BREDR:
                CsrUint16Ser(ptr, length, (CsrUint16) primitive->keys.u[i].enc_bredr->link_key_type);
                CsrMemCpySer(ptr, length, ((const void *) primitive->keys.u[i].enc_bredr->link_key), sizeof(primitive->keys.u[i].enc_bredr->link_key));
                break;

            case DM_SM_KEY_ENC_CENTRAL:
                CsrUint16Ser(ptr, length, (CsrUint16) primitive->keys.u[i].enc_central->ediv);
                CsrMemCpySer(ptr, length, ((const void *) primitive->keys.u[i].enc_central->rand), sizeof(primitive->keys.u[i].enc_central->rand));
                CsrMemCpySer(ptr, length, ((const void *) primitive->keys.u[i].enc_central->ltk), sizeof(primitive->keys.u[i].enc_central->ltk));
                break;

            case DM_SM_KEY_DIV:
                CsrUint16Ser(ptr, length, (CsrUint16) primitive->keys.u[i].div);
                break;

            case DM_SM_KEY_SIGN:
                CsrMemCpySer(ptr, length, ((const void *) primitive->keys.u[i].sign->csrk), sizeof(primitive->keys.u[i].sign->csrk));
                CsrUint32Ser(ptr, length, (CsrUint32) primitive->keys.u[i].sign->counter);
                break;

            case DM_SM_KEY_ID:
                CsrMemCpySer(ptr, length, ((const void *) primitive->keys.u[i].id->irk), sizeof(primitive->keys.u[i].id->irk));
                break;

            default:
                break;
        }
    }

    CsrUint8Ser(ptr, length, (CsrUint8) primitive->id_addrt.type);
    CsrUint32Ser(ptr, length, (CsrUint32) primitive->id_addrt.addr.lap);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->id_addrt.addr.uap);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->id_addrt.addr.nap);

    return ptr;
}

void *DM_SM_KEYS_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_KEYS_IND_T *primitive;
    CsrSize offset;
    CsrUint8 i;
    CsrUint16 present;

    offset = 0;
    primitive = (DM_SM_KEYS_IND_T *) CsrPmemZalloc(sizeof(DM_SM_KEYS_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);

    for(i=0, present = primitive->keys.present;
        i<DM_SM_MAX_NUM_KEYS;
        i++, present >>= DM_SM_NUM_KEY_BITS)
    {
        switch(present & 0x7)
        {
            case DM_SM_KEY_NONE:
                /* no key, ignore */
                break;

            case DM_SM_KEY_ENC_BREDR:
                primitive->keys.u[i].enc_bredr = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->keys.u[i].enc_bredr)));
                CsrUint16Des((CsrUint16 *) &primitive->keys.u[i].enc_bredr->link_key_type, buffer, &offset);
                CsrMemCpyDes(((void *) primitive->keys.u[i].enc_bredr->link_key), buffer, &offset, sizeof(primitive->keys.u[i].enc_bredr->link_key));
                break;

            case DM_SM_KEY_ENC_CENTRAL:
                primitive->keys.u[i].enc_central = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->keys.u[i].enc_central)));
                CsrUint16Des((CsrUint16 *) &primitive->keys.u[i].enc_central->ediv, buffer, &offset);
                CsrMemCpyDes(((void *) primitive->keys.u[i].enc_central->rand), buffer, &offset, sizeof(primitive->keys.u[i].enc_central->rand));
                CsrMemCpyDes(((void *) primitive->keys.u[i].enc_central->ltk), buffer, &offset, sizeof(primitive->keys.u[i].enc_central->ltk));
                break;

            case DM_SM_KEY_DIV:
                CsrUint16Des((CsrUint16 *) &primitive->keys.u[i].div, buffer, &offset);
                break;

            case DM_SM_KEY_SIGN:
                primitive->keys.u[i].sign = CsrPmemZalloc((CsrUint32) (sizeof(primitive->keys.u[i].sign->csrk)));
                CsrMemCpyDes(((void *) primitive->keys.u[i].sign->csrk), buffer, &offset, sizeof(primitive->keys.u[i].sign->csrk));
                CsrUint32Des((CsrUint32 *) &primitive->keys.u[i].sign->counter, buffer, &offset);
                break;

            case DM_SM_KEY_ID:
                primitive->keys.u[i].id = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->keys.u[i].id)));
                CsrMemCpyDes(((void *) primitive->keys.u[i].id->irk), buffer, &offset, sizeof(primitive->keys.u[i].id->irk));
                break;

            default:
                break;
        }
    }

    CsrUint8Des((CsrUint8 *) &primitive->id_addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->id_addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->id_addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id_addrt.addr.nap, buffer, &offset);

    return primitive;
}

void DM_SM_KEYS_IND_TSerFree(void *voidPrimitivePointer)
{
    DM_SM_KEYS_IND_T *primitive;
    CsrUint8 i;
    CsrUint16 present;

    primitive = (DM_SM_KEYS_IND_T *) voidPrimitivePointer;

    for(i=0, present = primitive->keys.present;
        i<DM_SM_MAX_NUM_KEYS;
        i++, present >>= DM_SM_NUM_KEY_BITS)
    {
        switch(present & 0x7)
        {
            case DM_SM_KEY_NONE:
                /* no key, ignore */
                break;

            case DM_SM_KEY_ENC_BREDR:
                CsrPmemFree(primitive->keys.u[i].enc_bredr);
                break;

            case DM_SM_KEY_ENC_CENTRAL:
                CsrPmemFree(primitive->keys.u[i].enc_central);
                break;

            case DM_SM_KEY_DIV:
                break;

            case DM_SM_KEY_SIGN:
                break;

            case DM_SM_KEY_ID:
                CsrPmemFree(primitive->keys.u[i].id);
                break;

            default:
                break;
        }
    }

    CsrPmemFree(primitive);
}

CsrSize DM_SM_KEYS_IND_TSizeof(void *voidPrimitivePointer)
{
    DM_SM_KEYS_IND_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_SM_KEYS_IND_T) +
                DM_SM_MAX_NUM_KEYS * sizeof(DM_SM_KEY_ENC_CENTRAL_T));
    }
    else
    {
        return 0;
    }
}

CsrUint8 *DM_SM_KEY_REQUEST_RSP_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_SM_KEY_REQUEST_RSP_T *primitive;

    primitive = (DM_SM_KEY_REQUEST_RSP_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(ptr, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->security_requirements);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->key_type);

    switch(primitive->key_type)
    {
        case DM_SM_KEY_NONE:
            /* no key, ignore */
            break;

        case DM_SM_KEY_ENC_BREDR:
            CsrUint16Ser(ptr, length, (CsrUint16) primitive->key.enc_bredr->link_key_type);
            CsrMemCpySer(ptr, length, ((const void *) primitive->key.enc_bredr->link_key), sizeof(primitive->key.enc_bredr->link_key));
            break;

        case DM_SM_KEY_ENC_CENTRAL:
            CsrUint16Ser(ptr, length, (CsrUint16) primitive->key.enc_central->ediv);
            CsrMemCpySer(ptr, length, ((const void *) primitive->key.enc_central->rand), sizeof(primitive->key.enc_central->rand));
            CsrMemCpySer(ptr, length, ((const void *) primitive->key.enc_central->ltk), sizeof(primitive->key.enc_central->ltk));
            break;

        case DM_SM_KEY_DIV:
            CsrUint16Ser(ptr, length, (CsrUint16) primitive->key.div);
            break;

        case DM_SM_KEY_SIGN:
            CsrMemCpySer(ptr, length, ((const void *) primitive->key.sign->csrk), sizeof(primitive->key.sign->csrk));
            CsrUint32Ser(ptr, length, (CsrUint32) primitive->key.sign->counter);
            break;

        case DM_SM_KEY_ID:
            CsrMemCpySer(ptr, length, ((const void *) primitive->key.id->irk), sizeof(primitive->key.id->irk));
            break;

        default:
            break;
    }

    return ptr;
}

void *DM_SM_KEY_REQUEST_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SM_KEY_REQUEST_RSP_T *primitive;
    CsrSize offset;

    offset = 0;
    primitive = (DM_SM_KEY_REQUEST_RSP_T *) CsrPmemZalloc(sizeof(DM_SM_KEY_REQUEST_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->security_requirements, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->key_type, buffer, &offset);

    switch(primitive->key_type)
    {
        case DM_SM_KEY_NONE:
            /* no key, ignore */
            break;

        case DM_SM_KEY_ENC_BREDR:
            primitive->key.enc_bredr = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->key.enc_bredr)));
            CsrUint16Des((CsrUint16 *) &primitive->key.enc_bredr->link_key_type, buffer, &offset);
            CsrMemCpyDes(((void *) primitive->key.enc_bredr->link_key), buffer, &offset, sizeof(primitive->key.enc_bredr->link_key));
            break;

        case DM_SM_KEY_ENC_CENTRAL:
            primitive->key.enc_central = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->key.enc_central)));
            CsrUint16Des((CsrUint16 *) &primitive->key.enc_central->ediv, buffer, &offset);
            CsrMemCpyDes(((void *) primitive->key.enc_central->rand), buffer, &offset, sizeof(primitive->key.enc_central->rand));
            CsrMemCpyDes(((void *) primitive->key.enc_central->ltk), buffer, &offset, sizeof(primitive->key.enc_central->ltk));
            break;

        case DM_SM_KEY_DIV:
            CsrUint16Des((CsrUint16 *) &primitive->key.div, buffer, &offset);
            break;

        case DM_SM_KEY_SIGN:
            primitive->key.sign = CsrPmemZalloc((CsrUint32) (sizeof(primitive->key.sign->csrk)));
            CsrMemCpyDes(((void *) primitive->key.sign->csrk), buffer, &offset, sizeof(primitive->key.sign->csrk));
            CsrUint32Des((CsrUint32 *) &primitive->key.sign->counter, buffer, &offset);
            break;

        case DM_SM_KEY_ID:
            primitive->key.id = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->key.id)));
            CsrMemCpyDes(((void *) primitive->key.id->irk), buffer, &offset, sizeof(primitive->key.id->irk));
            break;

        default:
            break;
    }

    return primitive;
}

void DM_SM_KEY_REQUEST_RSP_TSerFree(void *voidPrimitivePointer)
{
    DM_SM_KEY_REQUEST_RSP_T *primitive;

    primitive = (DM_SM_KEY_REQUEST_RSP_T *) voidPrimitivePointer;

    switch(primitive->key_type)
    {
        case DM_SM_KEY_NONE:
            /* no key, ignore */
            break;

        case DM_SM_KEY_ENC_BREDR:
            CsrPmemFree(primitive->key.enc_bredr);
            break;

        case DM_SM_KEY_ENC_CENTRAL:
            CsrPmemFree(primitive->key.enc_central);
            break;

        case DM_SM_KEY_DIV:
            break;

        case DM_SM_KEY_SIGN:
            break;

        case DM_SM_KEY_ID:
            CsrPmemFree(primitive->key.id);
            break;

        default:
            break;
    }

    CsrPmemFree(primitive);
}

CsrSize DM_SM_KEY_REQUEST_RSP_TSizeof(void *voidPrimitivePointer)
{
    DM_SM_KEY_REQUEST_RSP_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_SM_KEY_REQUEST_RSP_T) +
                sizeof(DM_SM_KEY_ENC_CENTRAL_T));
    }
    else
    {
        return 0;
    }
}

CsrUint8 *DM_SM_ADD_DEVICE_REQ_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    /* Not used on-host */
    CSR_UNUSED(ptr);
    CSR_UNUSED(length);
    CSR_UNUSED(voidPrimitivePointer);
    return NULL;
}

void *DM_SM_ADD_DEVICE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    /* Not used on-host */
    CSR_UNUSED(buffer);
    CSR_UNUSED(length);
    return NULL;
}

void DM_SM_ADD_DEVICE_REQ_TSerFree(void *voidPrimitivePointer)
{
    /* Not used on-host */
    CSR_UNUSED(voidPrimitivePointer);
}

CsrSize DM_SM_ADD_DEVICE_REQ_TSizeof(void *voidPrimitivePointer)
{
    /* Not used on-host */
    CSR_UNUSED(voidPrimitivePointer);
    return 0;
}

CsrUint8 *DM_SM_READ_DEVICE_CFM_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    /* Not used on-host */
    CSR_UNUSED(ptr);
    CSR_UNUSED(length);
    CSR_UNUSED(voidPrimitivePointer);
    return NULL;
}

void *DM_SM_READ_DEVICE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    /* Not used on-host */
    CSR_UNUSED(buffer);
    CSR_UNUSED(length);
    return NULL;
}

void DM_SM_READ_DEVICE_CFM_TSerFree(void *voidPrimitivePointer)
{
    /* Not used on-host */
    CSR_UNUSED(voidPrimitivePointer);
}

CsrSize DM_SM_READ_DEVICE_CFM_TSizeof(void *voidPrimitivePointer)
{
    /* Not used on-host */
    CSR_UNUSED(voidPrimitivePointer);
    return 0;
}

/* ------------------------------ DM-HCI ------------------------------ */

/* DM_HCI_INQUIRY_RESULT_IND */
CsrSize DM_HCI_INQUIRY_RESULT_IND_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_INQUIRY_RESULT_IND_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_HCI_INQUIRY_RESULT_IND_T) +
                primitive->num_responses * sizeof(HCI_INQ_RESULT_T));
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_HCI_INQUIRY_RESULT_IND_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_INQUIRY_RESULT_IND_T *primitive;
    CsrUint8 i;

    primitive = (DM_HCI_INQUIRY_RESULT_IND_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->num_responses);
    for(i=0; i<primitive->num_responses; i++)
    {
        CsrUint32Ser(ptr, length, (CsrUint32) primitive->result[i]->bd_addr.lap);
        CsrUint8Ser(ptr, length, (CsrUint8) primitive->result[i]->bd_addr.uap);
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->result[i]->bd_addr.nap);
        CsrUint8Ser(ptr, length, (CsrUint8) primitive->result[i]->page_scan_rep_mode);
        CsrUint8Ser(ptr, length, (CsrUint8) primitive->result[i]->page_scan_period_mode);
        CsrUint8Ser(ptr, length, (CsrUint8) primitive->result[i]->page_scan_mode);
        CsrUint32Ser(ptr, length, (CsrUint32) primitive->result[i]->dev_class);
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->result[i]->clock_offset);
    }

    return ptr;
}

void *DM_HCI_INQUIRY_RESULT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_INQUIRY_RESULT_IND_T *primitive;
    CsrSize offset;
    CsrUint8 i;

    offset = 0;
    primitive = (DM_HCI_INQUIRY_RESULT_IND_T *) CsrPmemZalloc(sizeof(DM_HCI_INQUIRY_RESULT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num_responses, buffer, &offset);
    for(i=0; i<primitive->num_responses; i++)
    {
        primitive->result[i] = CsrPmemAlloc(sizeof(HCI_INQ_RESULT_T));
        CsrUint32Des((CsrUint32 *) &primitive->result[i]->bd_addr.lap, buffer, &offset);
        CsrUint8Des((CsrUint8 *) &primitive->result[i]->bd_addr.uap, buffer, &offset);
        CsrUint16Des((CsrUint16 *) &primitive->result[i]->bd_addr.nap, buffer, &offset);
        CsrUint8Des((CsrUint8 *) &primitive->result[i]->page_scan_rep_mode, buffer, &offset);
        CsrUint8Des((CsrUint8 *) &primitive->result[i]->page_scan_period_mode, buffer, &offset);
        CsrUint8Des((CsrUint8 *) &primitive->result[i]->page_scan_mode, buffer, &offset);
        CsrUint32Des((CsrUint32 *) &primitive->result[i]->dev_class, buffer, &offset);
        CsrUint16Des((CsrUint16 *) &primitive->result[i]->clock_offset, buffer, &offset);
    }

    return primitive;
}

void DM_HCI_INQUIRY_RESULT_IND_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_INQUIRY_RESULT_IND_T *primitive;
    CsrUint8 i;

    primitive = (DM_HCI_INQUIRY_RESULT_IND_T *) voidPrimitivePointer;
    for(i=0; i<primitive->num_responses; i++)
    {
        CsrPmemFree(primitive->result[i]);
    }
    CsrPmemFree(primitive);
}

/* DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND */
CsrSize DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T) +
                primitive->num_responses * sizeof(HCI_INQ_RESULT_WITH_RSSI_T));
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T *primitive;
    CsrUint8 i;

    primitive = (DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->num_responses);
    for(i=0; i<primitive->num_responses; i++)
    {
        CsrUint32Ser(ptr, length, (CsrUint32) primitive->result[i]->bd_addr.lap);
        CsrUint8Ser(ptr, length, (CsrUint8) primitive->result[i]->bd_addr.uap);
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->result[i]->bd_addr.nap);
        CsrUint8Ser(ptr, length, (CsrUint8) primitive->result[i]->page_scan_rep_mode);
        CsrUint8Ser(ptr, length, (CsrUint8) primitive->result[i]->page_scan_period_mode);
        CsrUint32Ser(ptr, length, (CsrUint32) primitive->result[i]->dev_class);
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->result[i]->clock_offset);
        CsrUint8Ser(ptr, length, (CsrUint8) primitive->result[i]->rssi);
    }

    return ptr;
}

void *DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T *primitive;
    CsrSize offset;
    CsrUint8 i;

    offset = 0;
    primitive = (DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T *) CsrPmemZalloc(sizeof(DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num_responses, buffer, &offset);
    for(i=0; i<primitive->num_responses; i++)
    {
        primitive->result[i] = CsrPmemAlloc(sizeof(HCI_INQ_RESULT_WITH_RSSI_T));
        CsrUint32Des((CsrUint32 *) &primitive->result[i]->bd_addr.lap, buffer, &offset);
        CsrUint8Des((CsrUint8 *) &primitive->result[i]->bd_addr.uap, buffer, &offset);
        CsrUint16Des((CsrUint16 *) &primitive->result[i]->bd_addr.nap, buffer, &offset);
        CsrUint8Des((CsrUint8 *) &primitive->result[i]->page_scan_rep_mode, buffer, &offset);
        CsrUint8Des((CsrUint8 *) &primitive->result[i]->page_scan_period_mode, buffer, &offset);
        CsrUint32Des((CsrUint32 *) &primitive->result[i]->dev_class, buffer, &offset);
        CsrUint16Des((CsrUint16 *) &primitive->result[i]->clock_offset, buffer, &offset);
        CsrUint8Des((CsrUint8 *) &primitive->result[i]->rssi, buffer, &offset);
    }

    return primitive;
}

void DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T *primitive;
    CsrUint8 i;

    primitive = (DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T *) voidPrimitivePointer;
    for(i=0; i<primitive->num_responses; i++)
    {
        CsrPmemFree(primitive->result[i]);
    }
    CsrPmemFree(primitive);
}

/* DM_HCI_REMOTE_NAME_CFM */
CsrSize DM_HCI_REMOTE_NAME_CFM_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_REMOTE_NAME_CFM_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_HCI_REMOTE_NAME_CFM_T) + 
                namePartBytes(primitive->name_part));
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_HCI_REMOTE_NAME_CFM_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_REMOTE_NAME_CFM_T *primitive;
    primitive = (DM_HCI_REMOTE_NAME_CFM_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->status);
    CsrUint32Ser(ptr, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->bd_addr.nap);

    namePartSerialise(ptr, length, primitive->name_part);

    return ptr;
}

void *DM_HCI_REMOTE_NAME_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_REMOTE_NAME_CFM_T *primitive;
    CsrSize offset;

    offset = 0;
    primitive = (DM_HCI_REMOTE_NAME_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_REMOTE_NAME_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    namePartDeserialise(buffer, &offset, primitive->name_part);

    return primitive;
}

void DM_HCI_REMOTE_NAME_CFM_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_REMOTE_NAME_CFM_T *primitive;
    primitive = (DM_HCI_REMOTE_NAME_CFM_T *) voidPrimitivePointer;

    if(primitive)
    {
        namePartFree(primitive->name_part);
    }
    CsrPmemFree(primitive);
}

/* DM_HCI_READ_LOCAL_NAME_CFM */
CsrSize DM_HCI_READ_LOCAL_NAME_CFM_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_READ_LOCAL_NAME_CFM_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_HCI_READ_LOCAL_NAME_CFM_T) +
                namePartBytes(primitive->name_part));
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_HCI_READ_LOCAL_NAME_CFM_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_READ_LOCAL_NAME_CFM_T *primitive;

    primitive = (DM_HCI_READ_LOCAL_NAME_CFM_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->status);

    namePartSerialise(ptr, length, primitive->name_part);

    return ptr;
}

void *DM_HCI_READ_LOCAL_NAME_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_LOCAL_NAME_CFM_T *primitive;
    CsrSize offset;

    offset = 0;
    primitive = (DM_HCI_READ_LOCAL_NAME_CFM_T *) CsrPmemZalloc(sizeof(DM_HCI_READ_LOCAL_NAME_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    namePartDeserialise(buffer, &offset, primitive->name_part);

    return primitive;
}

void DM_HCI_READ_LOCAL_NAME_CFM_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_READ_LOCAL_NAME_CFM_T *primitive;
    primitive = (DM_HCI_READ_LOCAL_NAME_CFM_T *) voidPrimitivePointer;

    if(primitive)
    {
        namePartFree(primitive->name_part);
    }
    CsrPmemFree(primitive);
}

/* DM_HCI_READ_CURRENT_IAC_LAP_CFM */
CsrSize DM_HCI_READ_CURRENT_IAC_LAP_CFM_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_READ_CURRENT_IAC_LAP_CFM_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_HCI_READ_CURRENT_IAC_LAP_CFM_T) +
                primitive->num_current_iac * sizeof(CsrUint24));
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_HCI_READ_CURRENT_IAC_LAP_CFM_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_READ_CURRENT_IAC_LAP_CFM_T *primitive;

    primitive = (DM_HCI_READ_CURRENT_IAC_LAP_CFM_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->status);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->num_current_iac);

    iacPartSerialise(ptr, length,
                     primitive->iac_lap,
                     primitive->num_current_iac);

    return ptr;
}

void *DM_HCI_READ_CURRENT_IAC_LAP_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_CURRENT_IAC_LAP_CFM_T *primitive;
    CsrSize offset;

    offset = 0;
    primitive = (DM_HCI_READ_CURRENT_IAC_LAP_CFM_T*) CsrPmemZalloc(sizeof(DM_HCI_READ_CURRENT_IAC_LAP_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num_current_iac, buffer, &offset);

    iacPartDeserialise(buffer, &offset,
                       primitive->iac_lap,
                       primitive->num_current_iac);

    return primitive;
}

void DM_HCI_READ_CURRENT_IAC_LAP_CFM_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_READ_CURRENT_IAC_LAP_CFM_T *primitive;
    primitive = (DM_HCI_READ_CURRENT_IAC_LAP_CFM_T*)voidPrimitivePointer;
    if(primitive)
    {
        iacPartFree(primitive->iac_lap,
                    primitive->num_current_iac);
    }
    CsrPmemFree(primitive);
}

/* DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM */
CsrSize DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T) +
                HCI_EIR_DATA_LENGTH);
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T *primitive;

    primitive = (DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->status);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->fec_required);

    eirPartSerialise(ptr, length, primitive->eir_data_part);

    return ptr;
}

void *DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T *primitive;
    CsrSize offset;

    offset = 0;
    primitive = (DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T*) CsrPmemZalloc(sizeof(DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fec_required, buffer, &offset);
    
    eirPartDeserialise(buffer, &offset, primitive->eir_data_part);

    return primitive;
}

void DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T *primitive;
    primitive = (DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T*)voidPrimitivePointer;
    if(primitive)
    {
        eirPartFree(primitive->eir_data_part);
    }
    CsrPmemFree(primitive);
}

/* DM_HCI_EXTENDED_INQUIRY_RESULT_IND */
CsrSize DM_HCI_EXTENDED_INQUIRY_RESULT_IND_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T) +
                HCI_EIR_DATA_LENGTH);
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_HCI_EXTENDED_INQUIRY_RESULT_IND_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T *primitive;

    primitive = (DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->num_responses);
    CsrUint32Ser(ptr, length, (CsrUint32) primitive->result.bd_addr.lap);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->result.bd_addr.uap);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->result.bd_addr.nap);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->result.page_scan_rep_mode);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->result.page_scan_period_mode);
    CsrUint32Ser(ptr, length, (CsrUint32) primitive->result.dev_class);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->result.clock_offset);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->result.rssi);

    eirPartSerialise(ptr, length, primitive->eir_data_part);

    return ptr;
}

void *DM_HCI_EXTENDED_INQUIRY_RESULT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T *primitive;
    CsrSize offset;

    offset = 0;
    primitive = (DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T*) CsrPmemZalloc(sizeof(DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num_responses, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->result.bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result.bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result.bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result.page_scan_rep_mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result.page_scan_period_mode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->result.dev_class, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result.clock_offset, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result.rssi, buffer, &offset);

    eirPartDeserialise(buffer, &offset, primitive->eir_data_part);

    return primitive;
}

void DM_HCI_EXTENDED_INQUIRY_RESULT_IND_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T *primitive;
    primitive = (DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T*)voidPrimitivePointer;
    if(primitive)
    {
        eirPartFree(primitive->eir_data_part);
    }
    CsrPmemFree(primitive);
}

/* DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ */
CsrSize DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T) +
                HCI_EIR_DATA_LENGTH);
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->fec_required);

    eirPartSerialise(ptr, length, primitive->eir_data_part);

    return ptr;
}

void *DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T*) CsrPmemZalloc(sizeof(DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T));
    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fec_required, buffer, &offset);

    eirPartDeserialise(buffer, &offset, primitive->eir_data_part);

    return primitive;
}

void DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T *primitive;
    primitive = (DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T*)voidPrimitivePointer;
    if(primitive)
    {
        eirPartFree(primitive->eir_data_part);
    }
    CsrPmemFree(primitive);
}

/* DM_HCI_WRITE_CURRENT_IAC_LAP_REQ */
CsrSize DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_T) +
                primitive->num_current_iac * sizeof(CsrUint24));
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->num_current_iac);

    iacPartSerialise(ptr, length,
                     primitive->iac_lap,
                     primitive->num_current_iac);

    return ptr;
}

void *DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_T*) CsrPmemZalloc(sizeof(DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_T));
    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num_current_iac, buffer, &offset);

    iacPartDeserialise(buffer, &offset, primitive->iac_lap, primitive->num_current_iac);

    return primitive;
}

void DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_T *primitive;
    primitive = (DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_T*)voidPrimitivePointer;
    if(primitive)
    {
        iacPartFree(primitive->iac_lap,
                    primitive->num_current_iac);
    }
    CsrPmemFree(primitive);
}

/* DM_HCI_CHANGE_LOCAL_NAME_REQ */
CsrSize DM_HCI_CHANGE_LOCAL_NAME_REQ_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_CHANGE_LOCAL_NAME_REQ_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_HCI_CHANGE_LOCAL_NAME_REQ_T) +
                namePartBytes(primitive->name_part));
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_HCI_CHANGE_LOCAL_NAME_REQ_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_CHANGE_LOCAL_NAME_REQ_T *primitive;

    primitive = (DM_HCI_CHANGE_LOCAL_NAME_REQ_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->common.length);

    namePartSerialise(ptr, length, primitive->name_part);

    return ptr;
}

void *DM_HCI_CHANGE_LOCAL_NAME_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_CHANGE_LOCAL_NAME_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_CHANGE_LOCAL_NAME_REQ_T*) CsrPmemZalloc(sizeof(DM_HCI_CHANGE_LOCAL_NAME_REQ_T));
    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);

    namePartDeserialise(buffer, &offset, primitive->name_part);

    return primitive;
}

void DM_HCI_CHANGE_LOCAL_NAME_REQ_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_CHANGE_LOCAL_NAME_REQ_T *primitive;
    primitive = (DM_HCI_CHANGE_LOCAL_NAME_REQ_T*)voidPrimitivePointer;
    if(primitive)
    {
        namePartFree(primitive->name_part);
    }
    CsrPmemFree(primitive);
}

/* DM_HCI_SET_EVENT_FILTER_REQ */
CsrSize DM_HCI_SET_EVENT_FILTER_REQ_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_SET_EVENT_FILTER_REQ_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(DM_HCI_SET_EVENT_FILTER_REQ_T));
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_HCI_SET_EVENT_FILTER_REQ_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_SET_EVENT_FILTER_REQ_T *primitive;

    primitive = (DM_HCI_SET_EVENT_FILTER_REQ_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->filter_type);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->filter_condition_type);
    CsrMemCpySer(ptr, length, ((const void *) &primitive->condition), sizeof(primitive->condition));

    return ptr;
}
void *DM_HCI_SET_EVENT_FILTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_SET_EVENT_FILTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_SET_EVENT_FILTER_REQ_T*) CsrPmemZalloc(sizeof(DM_HCI_SET_EVENT_FILTER_REQ_T));
    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->filter_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->filter_condition_type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->condition), buffer, &offset, sizeof(primitive->condition));

    return primitive;
}

void DM_HCI_SET_EVENT_FILTER_REQ_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_SET_EVENT_FILTER_REQ_T *primitive;
    primitive = (DM_HCI_SET_EVENT_FILTER_REQ_T*)voidPrimitivePointer;
    CsrPmemFree(primitive);
}

/* DM_HCI_RETURN_LINK_KEYS_IND */
CsrSize DM_HCI_RETURN_LINK_KEYS_IND_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_RETURN_LINK_KEYS_IND_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(*primitive) + (10 * sizeof(*primitive->link_key_bd_addr[0])));
    }
    else
    {
        return (0);
    }
}

CsrUint8 *DM_HCI_RETURN_LINK_KEYS_IND_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_RETURN_LINK_KEYS_IND_T *primitive;

    primitive = (DM_HCI_RETURN_LINK_KEYS_IND_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->num_keys);
    CsrMemCpySer(ptr, length, primitive->link_key_bd_addr, sizeof(primitive->link_key_bd_addr));
    {
        CsrUint32 size = 10;
        CsrUint32 idx;
        for (idx=0; idx < size; ++idx)
        {
            if(primitive->link_key_bd_addr[idx])
            {
                CsrUint32Ser(ptr, length, (CsrUint32) primitive->link_key_bd_addr[idx]->bd_addr.lap);
                CsrUint8Ser(ptr, length, (CsrUint8) primitive->link_key_bd_addr[idx]->bd_addr.uap);
                CsrUint16Ser(ptr, length, (CsrUint16) primitive->link_key_bd_addr[idx]->bd_addr.nap);
                CsrMemCpySer(ptr, length, ((const void *) primitive->link_key_bd_addr[idx]->link_key), ((CsrUint32) (16)));
            }
        }
    }

    return ptr;
}

void *DM_HCI_RETURN_LINK_KEYS_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_RETURN_LINK_KEYS_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_RETURN_LINK_KEYS_IND_T *) CsrPmemZalloc(sizeof(DM_HCI_RETURN_LINK_KEYS_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num_keys, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->link_key_bd_addr), buffer, &offset, ((CsrUint16) sizeof(primitive->link_key_bd_addr)));
    {
        CsrUint32 size = 10;
        CsrUint32 idx;
        for (idx=0; idx < size; ++idx)
        {
            if(primitive->link_key_bd_addr[idx])
            {
                primitive->link_key_bd_addr[idx] = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->link_key_bd_addr[idx])));
                CsrUint32Des(((CsrUint32 *) &primitive->link_key_bd_addr[idx]->bd_addr.lap), buffer, &offset);
                CsrUint8Des(((CsrUint8 *) &primitive->link_key_bd_addr[idx]->bd_addr.uap), buffer, &offset);
                CsrUint16Des(((CsrUint16 *) &primitive->link_key_bd_addr[idx]->bd_addr.nap), buffer, &offset);
                CsrMemCpyDes(((void *) primitive->link_key_bd_addr[idx]->link_key), buffer, &offset, ((CsrUint16) (16)));
            }
        }
    }

    return primitive;
}

void DM_HCI_RETURN_LINK_KEYS_IND_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_RETURN_LINK_KEYS_IND_T *primitive;
    CsrUint32 idx;
    
    primitive = (DM_HCI_RETURN_LINK_KEYS_IND_T *) voidPrimitivePointer;

    for (idx=10; idx > 0; idx--)
    {
        CsrPmemFree(primitive->link_key_bd_addr[idx - 1]);
    }

    CsrPmemFree(primitive);
}

/* DM_HCI_WRITE_STORED_LINK_KEY_REQ */
CsrSize DM_HCI_WRITE_STORED_LINK_KEY_REQ_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_WRITE_STORED_LINK_KEY_REQ_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(*primitive) + (10 * sizeof(*primitive->link_key_bd_addr[0])));
    }
    else
    {
        return (0);
    }
}

CsrUint8 *DM_HCI_WRITE_STORED_LINK_KEY_REQ_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_WRITE_STORED_LINK_KEY_REQ_T *primitive;

    primitive = (DM_HCI_WRITE_STORED_LINK_KEY_REQ_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->number_keys);
    CsrMemCpySer(ptr, length, primitive->link_key_bd_addr, sizeof(primitive->link_key_bd_addr));
    {
        CsrUint32 size = 10;
        CsrUint32 idx;
        for (idx=0; idx < size; ++idx)
        {
            if(primitive->link_key_bd_addr[idx])
            {
                CsrUint32Ser(ptr, length, (CsrUint32) primitive->link_key_bd_addr[idx]->bd_addr.lap);
                CsrUint8Ser(ptr, length, (CsrUint8) primitive->link_key_bd_addr[idx]->bd_addr.uap);
                CsrUint16Ser(ptr, length, (CsrUint16) primitive->link_key_bd_addr[idx]->bd_addr.nap);
                CsrMemCpySer(ptr, length, ((const void *) primitive->link_key_bd_addr[idx]->link_key), ((CsrUint32) (16)));
            }
        }
    }

    return ptr;
}

void *DM_HCI_WRITE_STORED_LINK_KEY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_WRITE_STORED_LINK_KEY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_WRITE_STORED_LINK_KEY_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_WRITE_STORED_LINK_KEY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->number_keys, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->link_key_bd_addr), buffer, &offset, ((CsrUint16) sizeof(primitive->link_key_bd_addr)));
    {
        CsrUint32 size = 10;
        CsrUint32 idx;
        for (idx=0; idx < size; ++idx)
        {
            if(primitive->link_key_bd_addr[idx])
            {
                primitive->link_key_bd_addr[idx] = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->link_key_bd_addr[idx])));
                CsrUint32Des(((CsrUint32 *) &primitive->link_key_bd_addr[idx]->bd_addr.lap), buffer, &offset);
                CsrUint8Des(((CsrUint8 *) &primitive->link_key_bd_addr[idx]->bd_addr.uap), buffer, &offset);
                CsrUint16Des(((CsrUint16 *) &primitive->link_key_bd_addr[idx]->bd_addr.nap), buffer, &offset);
                CsrMemCpyDes(((void *) primitive->link_key_bd_addr[idx]->link_key), buffer, &offset, ((CsrUint16) (16)));
            }
        }
    }

    return primitive;
}

void DM_HCI_WRITE_STORED_LINK_KEY_REQ_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_WRITE_STORED_LINK_KEY_REQ_T *primitive;
    CsrUint32 idx;
        
    primitive = (DM_HCI_WRITE_STORED_LINK_KEY_REQ_T *) voidPrimitivePointer;

    for (idx=10; idx > 0; idx--)
    {
        CsrPmemFree(primitive->link_key_bd_addr[idx - 1]);
    }
    
    CsrPmemFree(primitive);
}

/* DM_HCI_HOST_NUM_COMPLETED_PACKETS_REQ */
CsrSize DM_HCI_HOST_NUM_COMPLETED_PACKETS_REQ_TSizeof(void *voidPrimitivePointer)
{
    DM_HCI_HOST_NUM_COMPLETED_PACKETS_REQ_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(*primitive) + (4 * sizeof(*primitive->num_completed_pkts_ptr[0])));
    }
    else
    {
        return (0);
    }
}

CsrUint8 *DM_HCI_HOST_NUM_COMPLETED_PACKETS_REQ_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_HCI_HOST_NUM_COMPLETED_PACKETS_REQ_T *primitive;

    primitive = (DM_HCI_HOST_NUM_COMPLETED_PACKETS_REQ_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->common.op_code);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->common.length);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->num_handles);
    CsrMemCpySer(ptr, length, primitive->num_completed_pkts_ptr, sizeof(primitive->num_completed_pkts_ptr));
    {
        CsrUint32 size = 4;
        CsrUint32 idx;
        for (idx=0; idx < size; ++idx)
        {
            if(primitive->num_completed_pkts_ptr[idx])
            {
                CsrUint16Ser(ptr, length, (CsrUint16) primitive->num_completed_pkts_ptr[idx]->handle);
                CsrUint16Ser(ptr, length, (CsrUint16) primitive->num_completed_pkts_ptr[idx]->num_completed);
            }
        }
    }

    return ptr;
}

void *DM_HCI_HOST_NUM_COMPLETED_PACKETS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_HOST_NUM_COMPLETED_PACKETS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_HOST_NUM_COMPLETED_PACKETS_REQ_T *) CsrPmemZalloc(sizeof(DM_HCI_HOST_NUM_COMPLETED_PACKETS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->common.op_code, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->common.length, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num_handles, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->num_completed_pkts_ptr), buffer, &offset, ((CsrUint16) sizeof(primitive->num_completed_pkts_ptr)));
    {
        CsrUint32 size = 4;
        CsrUint32 idx;
        for (idx=0; idx < size; ++idx)
        {
            if(primitive->num_completed_pkts_ptr[idx])
            {
                primitive->num_completed_pkts_ptr[idx] = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->num_completed_pkts_ptr[idx])));
                CsrUint16Des((CsrUint16 *) &primitive->num_completed_pkts_ptr[idx]->handle, buffer, &offset);
                CsrUint16Des((CsrUint16 *) &primitive->num_completed_pkts_ptr[idx]->num_completed, buffer, &offset);
            }
        }
    }

    return primitive;
}

void DM_HCI_HOST_NUM_COMPLETED_PACKETS_REQ_TSerFree(void *voidPrimitivePointer)
{
    DM_HCI_HOST_NUM_COMPLETED_PACKETS_REQ_T *primitive;
    
    CsrUint32 idx;
    
    primitive = (DM_HCI_HOST_NUM_COMPLETED_PACKETS_REQ_T *) voidPrimitivePointer;

    for (idx=4; idx > 0; idx--)
    {
        CsrPmemFree(primitive->num_completed_pkts_ptr[idx - 1]);
    }

    CsrPmemFree(primitive);
}


/* DM_HCI_ULP_ADVERTISING_REPORT_IND_T */
CsrSize DM_HCI_ULP_ADVERTISING_REPORT_IND_TSizeof(void *msg)
{
    DM_HCI_ULP_ADVERTISING_REPORT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + primitive->length_data);
    }
    else
    {
        return (0);
    }
}

CsrUint8 *DM_HCI_ULP_ADVERTISING_REPORT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    DM_HCI_ULP_ADVERTISING_REPORT_IND_T *primitive;

    primitive = (DM_HCI_ULP_ADVERTISING_REPORT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->num_reports);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->event_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->current_addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->current_addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->current_addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->current_addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->permanent_addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->permanent_addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->permanent_addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->permanent_addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->length_data);
    CsrMemCpySer(buffer, length, primitive->data, primitive->length_data);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rssi);

    return buffer;
}

void *DM_HCI_ULP_ADVERTISING_REPORT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_HCI_ULP_ADVERTISING_REPORT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (DM_HCI_ULP_ADVERTISING_REPORT_IND_T *) CsrPmemZalloc(sizeof(DM_HCI_ULP_ADVERTISING_REPORT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->num_reports, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->event_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->current_addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->current_addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->current_addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->current_addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->permanent_addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->permanent_addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->permanent_addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->permanent_addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->length_data, buffer, &offset);
    if(primitive->length_data > 0)
    {
        primitive->data = CsrPmemAlloc((CsrSize)primitive->length_data);
        CsrMemCpyDes(((void*)primitive->data), buffer, &offset, (CsrSize)primitive->length_data);
    }
    else
    {
        primitive->data = NULL;
    }
    CsrUint8Des((CsrUint8 *) &primitive->rssi, buffer, &offset);

    return primitive;
}

void DM_HCI_ULP_ADVERTISING_REPORT_IND_TSerFree(void *msg)
{
    DM_HCI_ULP_ADVERTISING_REPORT_IND_T *primitive;

    primitive = (DM_HCI_ULP_ADVERTISING_REPORT_IND_T *) msg;

    CsrPmemFree(primitive->data);
    CsrPmemFree(primitive);
}

/* DM_SYNC_CONNECT_REQ */
CsrSize DM_SYNC_CONNECT_REQ_TSizeof(void *voidPrimitivePointer)
{
    DM_SYNC_CONNECT_REQ_T *primitive = voidPrimitivePointer;
    if(primitive)
    {
        return (sizeof(DM_SYNC_CONNECT_REQ_T) +
                (CsrSize)(primitive->length == 0
                          ? sizeof(*(primitive->u.config))
                          : primitive->length * sizeof(CsrUint16)));
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_SYNC_CONNECT_REQ_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_SYNC_CONNECT_REQ_T *primitive;

    primitive = (DM_SYNC_CONNECT_REQ_T *) voidPrimitivePointer;

    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->pv_cbarg);
    CsrUint32Ser(ptr, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->length);

    if(primitive->length == 0)
    {
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->u.config->max_latency);
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->u.config->retx_effort);
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->u.config->packet_type);
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->u.config->voice_settings);
        CsrUint32Ser(ptr, length, (CsrUint32) primitive->u.config->tx_bdw);
        CsrUint32Ser(ptr, length, (CsrUint32) primitive->u.config->rx_bdw);
    }
    else
    {
        CsrMemCpySer(ptr, length, ((const void *) primitive->u.conftab), ((CsrUint16) (primitive->length * sizeof(CsrUint16))));
    }

    return ptr;
}

void *DM_SYNC_CONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_CONNECT_REQ_T *primitive;
    CsrSize offset;

    offset = 0;
    primitive = (DM_SYNC_CONNECT_REQ_T *) CsrPmemAlloc(sizeof(DM_SYNC_CONNECT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pv_cbarg, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);

    if (primitive->length == 0)
    {
        primitive->u.config = CsrPmemAlloc((CsrUint32) (sizeof(*primitive->u.config)));
        CsrUint16Des((CsrUint16 *) &primitive->u.config->max_latency, buffer, &offset);
        CsrUint16Des((CsrUint16 *) &primitive->u.config->retx_effort, buffer, &offset);
        CsrUint16Des((CsrUint16 *) &primitive->u.config->packet_type, buffer, &offset);
        CsrUint16Des((CsrUint16 *) &primitive->u.config->voice_settings, buffer, &offset);
        CsrUint32Des((CsrUint32 *) &primitive->u.config->tx_bdw, buffer, &offset);
        CsrUint32Des((CsrUint32 *) &primitive->u.config->rx_bdw, buffer, &offset);
    }
    else
    {
        primitive->u.conftab = CsrPmemAlloc(((CsrUint32) (primitive->length * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->u.conftab), buffer, &offset, ((CsrUint16) (primitive->length * sizeof(CsrUint16))));
    }

    return ((void *) primitive);
}

void DM_SYNC_CONNECT_REQ_TSerFree(void *voidPrimitivePointer)
{
    DM_SYNC_CONNECT_REQ_T *primitive;

    primitive = (DM_SYNC_CONNECT_REQ_T *) voidPrimitivePointer;

    /* Note that u is a union, so only free one of the members */
    CsrPmemFree(primitive->u.config);
    CsrPmemFree(primitive);
}

/* DM_SYNC_RENEGOTIATE_REQ */
CsrSize DM_SYNC_RENEGOTIATE_REQ_TSizeof(void *voidPrimitivePointer)
{
    DM_SYNC_RENEGOTIATE_REQ_T *primitive = voidPrimitivePointer;
    if(primitive)
    {
        return (sizeof(DM_SYNC_RENEGOTIATE_REQ_T) +
                (CsrSize)(primitive->length == 0
                          ? sizeof(*(primitive->u.config))
                          : primitive->length * sizeof(CsrUint16)));
    }
    else
    {
        return 0;
    }
}
CsrUint8 *DM_SYNC_RENEGOTIATE_REQ_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    DM_SYNC_RENEGOTIATE_REQ_T *primitive;

    primitive = (DM_SYNC_RENEGOTIATE_REQ_T *) voidPrimitivePointer;

    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->length);

    if(primitive->length == 0)
    {
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->u.config->max_latency);
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->u.config->retx_effort);
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->u.config->packet_type);
        CsrUint16Ser(ptr, length, (CsrUint16) primitive->u.config->voice_settings);
        CsrUint32Ser(ptr, length, (CsrUint32) primitive->u.config->tx_bdw);
        CsrUint32Ser(ptr, length, (CsrUint32) primitive->u.config->rx_bdw);
    }
    else
    {
        CsrMemCpySer(ptr, length, ((const void *) primitive->u.conftab), ((CsrUint16) (primitive->length * sizeof(CsrUint16))));
    }

    return ptr;
}

void *DM_SYNC_RENEGOTIATE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    DM_SYNC_RENEGOTIATE_REQ_T *primitive;
    CsrSize offset;

    offset = 0;
    primitive = (DM_SYNC_RENEGOTIATE_REQ_T *) CsrPmemAlloc(sizeof(DM_SYNC_RENEGOTIATE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);

    if (primitive->length == 0)
    {
        primitive->u.config = CsrPmemAlloc((CsrUint32) (sizeof(*primitive->u.config)));
        CsrUint16Des((CsrUint16 *) &primitive->u.config->max_latency, buffer, &offset);
        CsrUint16Des((CsrUint16 *) &primitive->u.config->retx_effort, buffer, &offset);
        CsrUint16Des((CsrUint16 *) &primitive->u.config->packet_type, buffer, &offset);
        CsrUint16Des((CsrUint16 *) &primitive->u.config->voice_settings, buffer, &offset);
        CsrUint32Des((CsrUint32 *) &primitive->u.config->tx_bdw, buffer, &offset);
        CsrUint32Des((CsrUint32 *) &primitive->u.config->rx_bdw, buffer, &offset);
    }
    else
    {
        primitive->u.conftab = CsrPmemAlloc(((CsrUint32) (primitive->length * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->u.conftab), buffer, &offset, ((CsrUint16) (primitive->length * sizeof(CsrUint16))));
    }

    return ((void *) primitive);
}

void DM_SYNC_RENEGOTIATE_REQ_TSerFree(void *voidPrimitivePointer)
{
    DM_SYNC_RENEGOTIATE_REQ_T *primitive;

    primitive = (DM_SYNC_RENEGOTIATE_REQ_T *) voidPrimitivePointer;

    /* Note that u is a union, so only free one of the members */
    CsrPmemFree(primitive->u.config);
    CsrPmemFree(primitive);
}


/* ------------------------------ ATT ------------------------------ */
CsrSize ATT_ADD_DB_REQ_TSizeof(void *voidPrimitivePointer)
{
    /* Not used on-host */
    CSR_UNUSED(voidPrimitivePointer);
    return 0;
}

CsrUint8 *ATT_ADD_DB_REQ_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
    /* Not used on-host */
    CSR_UNUSED(ptr);
    CSR_UNUSED(length);
    CSR_UNUSED(voidPrimitivePointer);
    return NULL;
}

void *ATT_ADD_DB_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    /* Not used on-host */
    CSR_UNUSED(buffer);
    CSR_UNUSED(length);
    return NULL;
}

void ATT_ADD_DB_REQ_TSerFree(void *voidPrimitivePointer)
{
    /* Not used on-host */
    CSR_UNUSED(voidPrimitivePointer);
}

CsrSize ATT_ADD_REQ_TSizeof(void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    ATT_ADD_REQ_T *primitive = voidPrimitivePointer;

    if (primitive)
    {
        /* sizeof(CsrUint32) is needed due to housekeeping/count of primitive->attrs */
        CsrSize res = sizeof(*primitive) + sizeof(CsrUint32);
        att_attr_t *db = primitive->attrs;

        while (db != NULL)
        {
            res += sizeof(*db) + db->size_value;
            db = db->next;
        }
        return res;
    }
    else
#endif
    {
        return 0;
    }
}

CsrUint8 *ATT_ADD_REQ_TSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    CsrUint32 count = 0;
    ATT_ADD_REQ_T * primitive;
    att_attr_t *db;

    primitive = (ATT_ADD_REQ_T *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16)primitive->type);
    CsrUint16Ser(ptr, length, (CsrUint16)primitive->phandle);

    db = primitive->attrs;
    while (db != NULL)
    {
        ++count;
        db = db->next;
    }
    CsrUint32Ser(ptr, length, (CsrUint32)count);

    db = primitive->attrs;
    while (db != NULL)
    {
        CsrUint16Ser(ptr, length, (CsrUint16)db->handle);
        CsrUint16Ser(ptr, length, (CsrUint16)db->perm);
        CsrUint16Ser(ptr, length, (CsrUint16)db->flags);
        CsrUint32Ser(ptr, length, (CsrUint32)db->uuid[0]);
        CsrUint32Ser(ptr, length, (CsrUint32)db->uuid[1]);
        CsrUint32Ser(ptr, length, (CsrUint32)db->uuid[2]);
        CsrUint32Ser(ptr, length, (CsrUint32)db->uuid[3]);
        CsrUint16Ser(ptr, length, (CsrUint16)db->size_value);
        CsrMemCpySer(ptr, length, (const void *) db->value, db->size_value);
        db = db->next;
    }
#endif

    return ptr;
}

void *ATT_ADD_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
#ifdef CSR_BT_LE_ENABLE
    CsrUint32 count;
    ATT_ADD_REQ_T *primitive;
    CsrSize offset = 0;
    att_attr_t *db;
    CSR_UNUSED(length);

    primitive = (ATT_ADD_REQ_T *) CsrPmemAlloc(sizeof(ATT_ADD_REQ_T));

    CsrUint16Des((CsrUint16 *)&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *)&primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *)&count, buffer, &offset);

    if (count > 0)
    {
        primitive->attrs = CsrPmemAlloc(sizeof(*primitive->attrs));
        db = primitive->attrs;

        while(1)
        {
            CsrUint16Des((CsrUint16 *)&db->handle, buffer, &offset);
            CsrUint16Des((CsrUint16 *)&db->perm, buffer, &offset);
            CsrUint16Des((CsrUint16 *)&db->flags, buffer, &offset);
            CsrUint32Des((CsrUint32 *)&db->uuid[0], buffer, &offset);
            CsrUint32Des((CsrUint32 *)&db->uuid[1], buffer, &offset);
            CsrUint32Des((CsrUint32 *)&db->uuid[2], buffer, &offset);
            CsrUint32Des((CsrUint32 *)&db->uuid[3], buffer, &offset);
            CsrUint16Des((CsrUint16 *)&db->size_value, buffer, &offset);

            db->value = CsrPmemAlloc(db->size_value);
            CsrMemCpyDes((void *) db->value, buffer, &offset, db->size_value);

            if (--count > 0)
            {
                db->next = CsrPmemAlloc(sizeof(*primitive->attrs));
                db = db->next;
            }
            else
            {
                db->next = NULL;
                break;
            }
        }
    }
    else
    {
        primitive->attrs = NULL;
    }
    return ((void *)primitive);
#else
    return NULL;
#endif
}

void ATT_ADD_REQ_TSerFree(void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    ATT_ADD_REQ_T *primitive;
    att_attr_t *db;
    att_attr_t *dbNext;

    primitive = (ATT_ADD_REQ_T*) voidPrimitivePointer;
    db = primitive->attrs;

    while (db != NULL)
    {
        dbNext = db->next;
        CsrPmemFree(db->value);
        db->value = NULL;
        db->next = NULL;
        CsrPmemFree(db);
        db = dbNext;
    }

    CsrPmemFree(primitive);
#endif
}

/* ------------------------------ GATT ------------------------------ */
CsrSize CsrBtGattDbAddReqSizeof(void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    CsrBtGattDbAddReq *primitive = voidPrimitivePointer;

    if (primitive)
    {
        /* sizeof(CsrUint32) is needed due to housekeeping/count of primitive->db */
        CsrSize res = sizeof(*primitive) + sizeof(CsrUint32);
        CsrBtGattDb *db = primitive->db;

        while (db != NULL)
        {
            res += sizeof(*db) + db->size_value;
            db = db->next;
        }
        return res;
    }
    else
#endif
    {
        return 0;
    }
}

CsrUint8 *CsrBtGattDbAddReqSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    CsrUint32 count = 0;
    CsrBtGattDbAddReq * primitive;
    CsrBtGattDb *db;

    primitive = (CsrBtGattDbAddReq *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16)primitive->type);
    CsrUint32Ser(ptr, length, (CsrUint32)primitive->gattId);

    db = primitive->db;
    while (db != NULL)
    {
        ++count;
        db = db->next;
    }
    CsrUint32Ser(ptr, length, (CsrUint32)count);

    db = primitive->db;
    while (db != NULL)
    {
        CsrUint16Ser(ptr, length, (CsrUint16)db->handle);
        CsrUint16Ser(ptr, length, (CsrUint16)db->perm);
        CsrUint16Ser(ptr, length, (CsrUint16)db->flags);
        CsrUint32Ser(ptr, length, (CsrUint32)db->uuid[0]);
        CsrUint32Ser(ptr, length, (CsrUint32)db->uuid[1]);
        CsrUint32Ser(ptr, length, (CsrUint32)db->uuid[2]);
        CsrUint32Ser(ptr, length, (CsrUint32)db->uuid[3]);
        CsrUint16Ser(ptr, length, (CsrUint16)db->size_value);
        CsrMemCpySer(ptr, length, (const void *) db->value, db->size_value);
        db = db->next;
    }
#endif

    return ptr;
}

void *CsrBtGattDbAddReqDes(CsrUint8 *buffer, CsrSize length)
{
#ifdef CSR_BT_LE_ENABLE
    CsrUint32 count;
    CsrBtGattDbAddReq *primitive;
    CsrSize offset = 0;
    CsrBtGattDb *db;
    CSR_UNUSED(length);

    primitive = (CsrBtGattDbAddReq *) CsrPmemAlloc(sizeof(CsrBtGattDbAddReq));

    CsrUint16Des((CsrUint16 *)&primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *)&primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *)&count, buffer, &offset);

    if (count > 0)
    {
        primitive->db = CsrPmemAlloc(sizeof(*primitive->db));
        db = primitive->db;

        while(1)
        {
            CsrUint16Des((CsrUint16 *)&db->handle, buffer, &offset);
            CsrUint16Des((CsrUint16 *)&db->perm, buffer, &offset);
            CsrUint16Des((CsrUint16 *)&db->flags, buffer, &offset);
            CsrUint32Des((CsrUint32 *)&db->uuid[0], buffer, &offset);
            CsrUint32Des((CsrUint32 *)&db->uuid[1], buffer, &offset);
            CsrUint32Des((CsrUint32 *)&db->uuid[2], buffer, &offset);
            CsrUint32Des((CsrUint32 *)&db->uuid[3], buffer, &offset);
            CsrUint16Des((CsrUint16 *)&db->size_value, buffer, &offset);

            db->value = CsrPmemAlloc(db->size_value);
            CsrMemCpyDes((void *) db->value, buffer, &offset, db->size_value);

            if (--count > 0)
            {
                db->next = CsrPmemAlloc(sizeof(*primitive->db));
                db = db->next;
            }
            else
            {
                db->next = NULL;
                break;
            }
        }
    }
    else
    {
        primitive->db = NULL;
    }
    return ((void *)primitive);
#else
    return NULL;
#endif
}

void CsrBtGattDbAddReqSerFree(void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    CsrBtGattDbAddReq *primitive;
    CsrBtGattDb *db;
    CsrBtGattDb *dbNext;

    primitive = (CsrBtGattDbAddReq*) voidPrimitivePointer;
    db = primitive->db;

    while (db != NULL)
    {
        dbNext = db->next;
        CsrPmemFree(db->value);
        db->value = NULL;
        db->next = NULL;
        CsrPmemFree(db);
        db = dbNext;
    }

    CsrPmemFree(primitive);
#endif
}

CsrSize CsrBtGattWriteReqSizeof(void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    CsrBtGattWriteReq *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(CsrBtGattWriteReq)
                + primitive->attrWritePairsCount*sizeof(CsrBtGattAttrWritePairs)
                + countAttrValueBytes(primitive->attrWritePairsCount,
                                      primitive->attrWritePairs));
    }
    else
#endif
    {
        return 0;
    }
}

CsrUint8 *CsrBtGattWriteReqSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    CsrUint16 i;
    CsrBtGattWriteReq * primitive;

    primitive = (CsrBtGattWriteReq *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16)primitive->type);
    CsrUint32Ser(ptr, length, (CsrUint32)primitive->gattId);
    CsrUint32Ser(ptr, length, (CsrUint32)primitive->btConnId);
    CsrUint16Ser(ptr, length, (CsrUint16)primitive->flags);
    CsrUint16Ser(ptr, length, (CsrUint16)primitive->attrWritePairsCount);

    if (primitive->attrWritePairsCount > 0)
    {
        for (i = 0; i<primitive->attrWritePairsCount; i++)
        {
            CsrUint16Ser(ptr, length, (CsrUint16)primitive->attrWritePairs[i].attrHandle);
            CsrUint16Ser(ptr, length, (CsrUint16)primitive->attrWritePairs[i].offset);
            CsrUint16Ser(ptr, length, (CsrUint16)primitive->attrWritePairs[i].valueLength);
            CsrMemCpySer(ptr, length, (const void *) primitive->attrWritePairs[i].value, primitive->attrWritePairs[i].valueLength);
        }
    }
#endif
    return ptr;
}

void *CsrBtGattWriteReqDes(CsrUint8 *buffer, CsrSize length)
{
#ifdef CSR_BT_LE_ENABLE
    CsrUint16 i;
    CsrBtGattWriteReq *primitive;
    CsrSize offset = 0;
    CSR_UNUSED(length);
    
    primitive = (CsrBtGattWriteReq *) CsrPmemAlloc(sizeof(CsrBtGattWriteReq));

    CsrUint16Des((CsrUint16 *)&primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *)&primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *)&primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *)&primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *)&primitive->attrWritePairsCount, buffer, &offset);

    if (primitive->attrWritePairsCount > 0)
    {
        primitive->attrWritePairs = CsrPmemAlloc((CsrUint32) primitive->attrWritePairsCount*sizeof(CsrBtGattAttrWritePairs));

        for (i = 0; i< primitive->attrWritePairsCount; i++)
        {
            CsrUint16Des((CsrUint16 *)&primitive->attrWritePairs[i].attrHandle, buffer, &offset);
            CsrUint16Des((CsrUint16 *)&primitive->attrWritePairs[i].offset, buffer, &offset);
            CsrUint16Des((CsrUint16 *)&primitive->attrWritePairs[i].valueLength, buffer, &offset);

            primitive->attrWritePairs[i].value = CsrPmemAlloc(primitive->attrWritePairs[i].valueLength);
            CsrMemCpyDes((void *) primitive->attrWritePairs[i].value, buffer, &offset, primitive->attrWritePairs[i].valueLength);
        }
    }
    else
    {
        primitive->attrWritePairs = NULL;
    }
    return ((void *)primitive);
#else
    return NULL;
#endif
}

void CsrBtGattWriteReqSerFree(void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    CsrBtGattWriteReq *primitive;
    CsrUint32 i;

    primitive = (CsrBtGattWriteReq*) voidPrimitivePointer;

    for (i=0;i<primitive->attrWritePairsCount;i++)
    {
        CsrPmemFree(primitive->attrWritePairs[i].value);
        primitive->attrWritePairs[i].value=NULL;
    }

    CsrPmemFree(primitive->attrWritePairs);
    primitive->attrWritePairs = NULL;

    CsrPmemFree(primitive);
#endif
}

CsrSize CsrBtGattDbAccessWriteIndSizeof(void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    CsrBtGattDbAccessWriteInd *primitive = voidPrimitivePointer;

    if (primitive)
    {
        return (sizeof(CsrBtGattDbAccessWriteInd)
                + primitive->writeUnitCount*sizeof(CsrBtGattAttrWritePairs)
                + countAttrValueBytes(primitive->writeUnitCount,
                                      primitive->writeUnit));
    }
    else
#endif
    {
        return 0;
    }
}

CsrUint8 *CsrBtGattDbAccessWriteIndSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    CsrUint16 i;
    CsrBtGattDbAccessWriteInd * primitive;

    primitive = (CsrBtGattDbAccessWriteInd *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16)primitive->type);
    CsrUint32Ser(ptr, length, (CsrUint32)primitive->gattId);
    CsrUint32Ser(ptr, length, (CsrUint32)primitive->btConnId);
    CsrUint16Ser(ptr, length, (CsrUint16)primitive->check);
    CsrUint32Ser(ptr, length, (CsrUint16)primitive->connInfo);
    CsrBtConverterSerialiseBtTypedAddr(ptr, length, &(primitive->address));
    CsrUint16Ser(ptr, length, (CsrUint16)primitive->writeUnitCount);

    if (primitive->writeUnitCount > 0)
    {
        for (i = 0; i<primitive->writeUnitCount; i++)
        {
            CsrUint16Ser(ptr, length, (CsrUint16)primitive->writeUnit[i].attrHandle);
            CsrUint16Ser(ptr, length, (CsrUint16)primitive->writeUnit[i].offset);
            CsrUint16Ser(ptr, length, (CsrUint16)primitive->writeUnit[i].valueLength);
            CsrMemCpySer(ptr, length, (const void *) primitive->writeUnit[i].value, primitive->writeUnit[i].valueLength);
        }
    }
#endif
    return ptr;
}

void *CsrBtGattDbAccessWriteIndDes(CsrUint8 *buffer, CsrSize length)
{
#ifdef CSR_BT_LE_ENABLE
    CsrUint16 i;
    CsrBtGattDbAccessWriteInd *primitive;
    CsrSize offset = 0;
    CSR_UNUSED(length);
    
    primitive = (CsrBtGattDbAccessWriteInd *) CsrPmemAlloc(sizeof(CsrBtGattDbAccessWriteInd));

    CsrUint16Des((CsrUint16 *)&primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *)&primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *)&primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *)&primitive->check, buffer, &offset);
    CsrUint32Des((CsrUint32 *)&primitive->connInfo, buffer, &offset);
    CsrBtConverterDeserialiseBtTypedAddr(buffer, &offset, &(primitive->address));
    CsrUint32Des((CsrUint32 *)&primitive->writeUnitCount, buffer, &offset);

    if (primitive->writeUnitCount > 0)
    {
        primitive->writeUnit = CsrPmemAlloc((CsrUint32) primitive->writeUnitCount*sizeof(CsrBtGattAttrWritePairs));

        for (i = 0; i< primitive->writeUnitCount; i++)
        {
            CsrUint16Des((CsrUint16 *)&primitive->writeUnit[i].attrHandle, buffer, &offset);
            CsrUint16Des((CsrUint16 *)&primitive->writeUnit[i].offset, buffer, &offset);
            CsrUint16Des((CsrUint16 *)&primitive->writeUnit[i].valueLength, buffer, &offset);

            primitive->writeUnit[i].value = CsrPmemAlloc(primitive->writeUnit[i].valueLength);
            CsrMemCpyDes((void *) primitive->writeUnit[i].value, buffer, &offset, primitive->writeUnit[i].valueLength);
        }
    }
    else
    {
        primitive->writeUnit = NULL;
    }
    return ((void *)primitive);
#else
    return NULL;
#endif
}

void CsrBtGattDbAccessWriteIndSerFree(void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    CsrBtGattDbAccessWriteInd *primitive;
    CsrUint32 i;

    primitive = (CsrBtGattDbAccessWriteInd*) voidPrimitivePointer;

    for (i=0;i<primitive->writeUnitCount;i++)
    {
        CsrPmemFree(primitive->writeUnit[i].value);
        primitive->writeUnit[i].value=NULL;
    }

    CsrPmemFree(primitive->writeUnit);
    primitive->writeUnit = NULL;

    CsrPmemFree(primitive);
#endif
}

CsrSize CsrBtGattScanReqSizeof(void *msg)
{
#ifdef CSR_BT_LE_ENABLE
    CsrBtGattScanReq *primitive = msg;

    if (primitive)
    {
        CsrSize size;
        CsrUint8 i;
        size = sizeof(*primitive) + (primitive->filterAddrCount * sizeof(CsrBtTypedAddr));

        for(i=0; i<primitive->filterDataCount; i++)
        {
            size += (sizeof(CsrBtGattDataFilter)
                     + primitive->filterData[i].dataLength);
        }

        return size;
    }
    else
#endif
    {
        return (0);
    }
}

CsrUint8 *CsrBtGattScanReqSer(CsrUint8 *ptr, CsrSize *length, void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    CsrBtGattScanReq *primitive;

    primitive = (CsrBtGattScanReq *) voidPrimitivePointer;
    *length = 0;
    CsrUint16Ser(ptr, length, (CsrUint16) primitive->type);
    CsrUint32Ser(ptr, length, (CsrUint32) primitive->gattId);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->enable);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->scanFlags);
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->filterAddrCount);
    CsrMemCpySer(ptr, length, &primitive->filterAddr, sizeof(primitive->filterAddr));
    CsrUint8Ser(ptr, length, (CsrUint8) primitive->filterDataCount);
    CsrMemCpySer(ptr, length, &primitive->filterData, sizeof(primitive->filterData));

    if(primitive->filterAddr)
    {
        CsrMemCpySer(ptr, length,
                     ((const void *) primitive->filterAddr),
                     ((CsrSize) (primitive->filterAddrCount * sizeof(CsrBtTypedAddr))));
    }
    if(primitive->filterData)
    {
        CsrUint8 i;
        for(i=0; i<primitive->filterDataCount; i++)
        {
            CsrUint8Ser(ptr, length, (CsrUint8) primitive->filterData[i].adType);
            CsrUint8Ser(ptr, length, (CsrUint8) primitive->filterData[i].interval);
            CsrUint8Ser(ptr, length, (CsrUint8) primitive->filterData[i].dataLength);
            CsrMemCpySer(ptr, length,
                         ((const void *) primitive->filterData[i].data),
                         (primitive->filterData[i].dataLength * sizeof(CsrUint8)));
        }
    }
#endif
    return ptr;
}

void *CsrBtGattScanReqDes(CsrUint8 *buffer, CsrSize length)
{
#ifdef CSR_BT_LE_ENABLE
    CsrBtGattScanReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattScanReq *) CsrPmemZalloc(sizeof(CsrBtGattScanReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enable, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scanFlags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->filterAddrCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->filterAddr), buffer, &offset, ((CsrSize) sizeof(primitive->filterAddr)));
    CsrUint8Des((CsrUint8 *) &primitive->filterDataCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->filterData), buffer, &offset, ((CsrSize) sizeof(primitive->filterData)));

    if (primitive->filterAddrCount)
    {
        primitive->filterAddr = CsrPmemZalloc(((CsrUint32) (primitive->filterAddrCount * sizeof(CsrBtTypedAddr))));
        CsrMemCpyDes(((void *) primitive->filterAddr), buffer, &offset,
                     (primitive->filterAddrCount * sizeof(CsrBtTypedAddr)));
    }
    else
    {
        primitive->filterAddr = NULL;
    }

    if (primitive->filterDataCount)
    {
        CsrUint8 i;

        primitive->filterData = CsrPmemZalloc(((CsrUint32) (primitive->filterDataCount * sizeof(CsrBtGattDataFilter))));

        for(i=0; i<primitive->filterDataCount; i++)
        {
            CsrUint8Des((CsrUint8 *) &primitive->filterData[i].adType, buffer, &offset);
            CsrUint8Des((CsrUint8 *) &primitive->filterData[i].interval, buffer, &offset);
            CsrUint8Des((CsrUint8 *) &primitive->filterData[i].dataLength, buffer, &offset);
            if (primitive->filterData[i].dataLength)
            {
                primitive->filterData[i].data = CsrPmemAlloc(((CsrUint32)(primitive->filterData[i].dataLength * sizeof(CsrUint8))));
                CsrMemCpyDes(((void *) primitive->filterData[i].data),
                             buffer, &offset,
                             (primitive->filterData[i].dataLength * sizeof(CsrUint8)));
            }
        }
    }
    else
    {
        primitive->filterData = NULL;
    }
    return primitive;
#else
    return NULL;
#endif
}

void CsrBtGattScanReqSerFree(void *voidPrimitivePointer)
{
#ifdef CSR_BT_LE_ENABLE
    CsrBtGattScanReq *primitive;
    primitive = (CsrBtGattScanReq*) voidPrimitivePointer;

    if(primitive->filterAddr != NULL)
    {
        CsrPmemFree(primitive->filterAddr);
        primitive->filterAddr = NULL;
    }
    if(primitive->filterData != NULL)
    {
        CsrUint8 i;
        for(i=0; i<primitive->filterDataCount; i++)
        {
            CsrPmemFree(primitive->filterData[i].data);
        }
        CsrPmemFree(primitive->filterData);
        primitive->filterData = NULL;
    }

    CsrPmemFree(primitive);
#endif
}

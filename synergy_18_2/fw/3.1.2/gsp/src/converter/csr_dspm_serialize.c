/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_autogen.h"
#include "csr_unicode.h"
#include "csr_dspm_prim.h"

#ifndef EXCLUDE_CSR_DSPM_MODULE

void CsrDspmPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

CsrSize CsrDspmStreamCpOpenReqSizeof(void *msg)
{
    CsrDspmStreamCpOpenReq *primitive = (CsrDspmStreamCpOpenReq *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 14) */
    bufferSize += 2; /* CsrSchedQid primitive->qid */
    bufferSize += 1; /* CsrUint8 primitive->cpInfoCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cpInfoCount; i1++)
        {
            bufferSize += 2; /* CsrDspmCpid primitive->cpInfo[i1].cpid */
            bufferSize += 2; /* CsrDspmCpType primitive->cpInfo[i1].cpType */
            bufferSize += 2; /* CsrUint16 primitive->cpInfo[i1].instance */
            bufferSize += 2; /* CsrUint16 primitive->cpInfo[i1].channel */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmStreamCpOpenReqSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmStreamCpOpenReq *primitive = (CsrDspmStreamCpOpenReq *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->qid);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->cpInfoCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cpInfoCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->cpInfo[i1].cpid);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->cpInfo[i1].cpType);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->cpInfo[i1].instance);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->cpInfo[i1].channel);
        }
    }
    return ptr;
}

void *CsrDspmStreamCpOpenReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmStreamCpOpenReq *primitive = (CsrDspmStreamCpOpenReq *) CsrPmemAlloc(sizeof(CsrDspmStreamCpOpenReq));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cpInfoCount, buffer, &offset);
    primitive->cpInfo = NULL;
    if (primitive->cpInfoCount)
    {
        primitive->cpInfo = (CsrDspmCpInfo *) CsrPmemAlloc(sizeof(CsrDspmCpInfo) * primitive->cpInfoCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cpInfoCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->cpInfo[i1].cpid, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->cpInfo[i1].cpType, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->cpInfo[i1].instance, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->cpInfo[i1].channel, buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmStreamCpOpenReqSerFree(void *voidPrimitivePointer)
{
    CsrDspmStreamCpOpenReq *primitive = (CsrDspmStreamCpOpenReq *) voidPrimitivePointer;
    CsrPmemFree(primitive->cpInfo);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmStreamCpCloseReqSizeof(void *msg)
{
    CsrDspmStreamCpCloseReq *primitive = (CsrDspmStreamCpCloseReq *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 8) */
    bufferSize += 2; /* CsrSchedQid primitive->qid */
    bufferSize += 1; /* CsrUint8 primitive->cpidCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cpidCount; i1++)
        {
            bufferSize += 2; /* CsrDspmCpid primitive->cpid */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmStreamCpCloseReqSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmStreamCpCloseReq *primitive = (CsrDspmStreamCpCloseReq *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->qid);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->cpidCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cpidCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->cpid[i1]);
        }
    }
    return ptr;
}

void *CsrDspmStreamCpCloseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmStreamCpCloseReq *primitive = (CsrDspmStreamCpCloseReq *) CsrPmemAlloc(sizeof(CsrDspmStreamCpCloseReq));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cpidCount, buffer, &offset);
    primitive->cpid = NULL;
    if (primitive->cpidCount)
    {
        primitive->cpid = (CsrDspmCpid *) CsrPmemAlloc(sizeof(CsrDspmCpid) * primitive->cpidCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cpidCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->cpid[i1], buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmStreamCpCloseReqSerFree(void *voidPrimitivePointer)
{
    CsrDspmStreamCpCloseReq *primitive = (CsrDspmStreamCpCloseReq *) voidPrimitivePointer;
    CsrPmemFree(primitive->cpid);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmStreamConfigureReqSizeof(void *msg)
{
    CsrDspmStreamConfigureReq *primitive = (CsrDspmStreamConfigureReq *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 14) */
    bufferSize += 2; /* CsrSchedQid primitive->qid */
    bufferSize += 1; /* CsrUint8 primitive->configInfoCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->configInfoCount; i1++)
        {
            bufferSize += 2; /* CsrDspmCpid primitive->configInfo[i1].cpid */
            bufferSize += 2; /* CsrUint16 primitive->configInfo[i1].feature */
            bufferSize += 4; /* CsrUint32 primitive->configInfo[i1].value */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmStreamConfigureReqSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmStreamConfigureReq *primitive = (CsrDspmStreamConfigureReq *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->qid);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->configInfoCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->configInfoCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->configInfo[i1].cpid);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->configInfo[i1].feature);
            CsrUint32Ser(ptr, len, (CsrUint32) primitive->configInfo[i1].value);
        }
    }
    return ptr;
}

void *CsrDspmStreamConfigureReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmStreamConfigureReq *primitive = (CsrDspmStreamConfigureReq *) CsrPmemAlloc(sizeof(CsrDspmStreamConfigureReq));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->configInfoCount, buffer, &offset);
    primitive->configInfo = NULL;
    if (primitive->configInfoCount)
    {
        primitive->configInfo = (CsrDspmConfigInfo *) CsrPmemAlloc(sizeof(CsrDspmConfigInfo) * primitive->configInfoCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->configInfoCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->configInfo[i1].cpid, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->configInfo[i1].feature, buffer, &offset);
            CsrUint32Des((CsrUint32 *) &primitive->configInfo[i1].value, buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmStreamConfigureReqSerFree(void *voidPrimitivePointer)
{
    CsrDspmStreamConfigureReq *primitive = (CsrDspmStreamConfigureReq *) voidPrimitivePointer;
    CsrPmemFree(primitive->configInfo);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmStreamSyncReqSizeof(void *msg)
{
    CsrDspmStreamSyncReq *primitive = (CsrDspmStreamSyncReq *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 10) */
    bufferSize += 2; /* CsrSchedQid primitive->qid */
    bufferSize += 1; /* CsrUint8 primitive->syncInfoCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->syncInfoCount; i1++)
        {
            bufferSize += 2; /* CsrDspmCpid primitive->syncInfo[i1].cpid1 */
            bufferSize += 2; /* CsrDspmCpid primitive->syncInfo[i1].cpid2 */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmStreamSyncReqSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmStreamSyncReq *primitive = (CsrDspmStreamSyncReq *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->qid);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->syncInfoCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->syncInfoCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->syncInfo[i1].cpid1);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->syncInfo[i1].cpid2);
        }
    }
    return ptr;
}

void *CsrDspmStreamSyncReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmStreamSyncReq *primitive = (CsrDspmStreamSyncReq *) CsrPmemAlloc(sizeof(CsrDspmStreamSyncReq));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->syncInfoCount, buffer, &offset);
    primitive->syncInfo = NULL;
    if (primitive->syncInfoCount)
    {
        primitive->syncInfo = (CsrDspmSyncInfo *) CsrPmemAlloc(sizeof(CsrDspmSyncInfo) * primitive->syncInfoCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->syncInfoCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->syncInfo[i1].cpid1, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->syncInfo[i1].cpid2, buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmStreamSyncReqSerFree(void *voidPrimitivePointer)
{
    CsrDspmStreamSyncReq *primitive = (CsrDspmStreamSyncReq *) voidPrimitivePointer;
    CsrPmemFree(primitive->syncInfo);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmOperatorCreateReqSizeof(void *msg)
{
    CsrDspmOperatorCreateReq *primitive = (CsrDspmOperatorCreateReq *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 10) */
    bufferSize += 2; /* CsrSchedQid primitive->qid */
    bufferSize += 1; /* CsrUint8 primitive->operatorInfoCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorInfoCount; i1++)
        {
            bufferSize += 2; /* CsrDspmOpid primitive->operatorInfo[i1].opid */
            bufferSize += 2; /* CsrDspmCapability primitive->operatorInfo[i1].capability */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmOperatorCreateReqSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmOperatorCreateReq *primitive = (CsrDspmOperatorCreateReq *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->qid);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->operatorInfoCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorInfoCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->operatorInfo[i1].opid);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->operatorInfo[i1].capability);
        }
    }
    return ptr;
}

void *CsrDspmOperatorCreateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmOperatorCreateReq *primitive = (CsrDspmOperatorCreateReq *) CsrPmemAlloc(sizeof(CsrDspmOperatorCreateReq));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->operatorInfoCount, buffer, &offset);
    primitive->operatorInfo = NULL;
    if (primitive->operatorInfoCount)
    {
        primitive->operatorInfo = (CsrDspmOperatorInfo *) CsrPmemAlloc(sizeof(CsrDspmOperatorInfo) * primitive->operatorInfoCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorInfoCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->operatorInfo[i1].opid, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->operatorInfo[i1].capability, buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmOperatorCreateReqSerFree(void *voidPrimitivePointer)
{
    CsrDspmOperatorCreateReq *primitive = (CsrDspmOperatorCreateReq *) voidPrimitivePointer;
    CsrPmemFree(primitive->operatorInfo);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmOperatorDestroyReqSizeof(void *msg)
{
    CsrDspmOperatorDestroyReq *primitive = (CsrDspmOperatorDestroyReq *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 8) */
    bufferSize += 2; /* CsrSchedQid primitive->qid */
    bufferSize += 1; /* CsrUint8 primitive->opidCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            bufferSize += 2; /* CsrDspmOpid primitive->opid */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmOperatorDestroyReqSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmOperatorDestroyReq *primitive = (CsrDspmOperatorDestroyReq *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->qid);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->opidCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->opid[i1]);
        }
    }
    return ptr;
}

void *CsrDspmOperatorDestroyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmOperatorDestroyReq *primitive = (CsrDspmOperatorDestroyReq *) CsrPmemAlloc(sizeof(CsrDspmOperatorDestroyReq));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->opidCount, buffer, &offset);
    primitive->opid = NULL;
    if (primitive->opidCount)
    {
        primitive->opid = (CsrDspmOpid *) CsrPmemAlloc(sizeof(CsrDspmOpid) * primitive->opidCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->opid[i1], buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmOperatorDestroyReqSerFree(void *voidPrimitivePointer)
{
    CsrDspmOperatorDestroyReq *primitive = (CsrDspmOperatorDestroyReq *) voidPrimitivePointer;
    CsrPmemFree(primitive->opid);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmOperatorMessageReqSizeof(void *msg)
{
    CsrDspmOperatorMessageReq *primitive = (CsrDspmOperatorMessageReq *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 12) */
    bufferSize += 2; /* CsrSchedQid primitive->qid */
    bufferSize += 1; /* CsrUint8 primitive->operatorMessageInfoCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorMessageInfoCount; i1++)
        {
            bufferSize += 2; /* CsrDspmOpid primitive->operatorMessageInfo[i1].opid */
            bufferSize += 2; /* CsrUint16 primitive->operatorMessageInfo[i1].messageLength */
            {
                CsrUint16 i2;
                for (i2 = 0; i2 < primitive->operatorMessageInfo[i1].messageLength; i2++)
                {
                    bufferSize += 2; /* CsrUint16 primitive->operatorMessageInfo[i1].message */
                }
            }
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmOperatorMessageReqSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmOperatorMessageReq *primitive = (CsrDspmOperatorMessageReq *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->qid);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->operatorMessageInfoCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorMessageInfoCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->operatorMessageInfo[i1].opid);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->operatorMessageInfo[i1].messageLength);
            {
                CsrUint16 i2;
                for (i2 = 0; i2 < primitive->operatorMessageInfo[i1].messageLength; i2++)
                {
                    CsrUint16Ser(ptr, len, (CsrUint16) primitive->operatorMessageInfo[i1].message[i2]);
                }
            }
        }
    }
    return ptr;
}

void *CsrDspmOperatorMessageReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmOperatorMessageReq *primitive = (CsrDspmOperatorMessageReq *) CsrPmemAlloc(sizeof(CsrDspmOperatorMessageReq));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->operatorMessageInfoCount, buffer, &offset);
    primitive->operatorMessageInfo = NULL;
    if (primitive->operatorMessageInfoCount)
    {
        primitive->operatorMessageInfo = (CsrDspmOperatorMessageInfo *) CsrPmemAlloc(sizeof(CsrDspmOperatorMessageInfo) * primitive->operatorMessageInfoCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorMessageInfoCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->operatorMessageInfo[i1].opid, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->operatorMessageInfo[i1].messageLength, buffer, &offset);
            primitive->operatorMessageInfo[i1].message = NULL;
            if (primitive->operatorMessageInfo[i1].messageLength)
            {
                primitive->operatorMessageInfo[i1].message = (CsrUint16 *) CsrPmemAlloc(sizeof(CsrUint16) * primitive->operatorMessageInfo[i1].messageLength);
            }
            {
                CsrUint16 i2;
                for (i2 = 0; i2 < primitive->operatorMessageInfo[i1].messageLength; i2++)
                {
                    CsrUint16Des((CsrUint16 *) &primitive->operatorMessageInfo[i1].message[i2], buffer, &offset);
                }
            }
        }
    }

    return primitive;
}

void CsrDspmOperatorMessageReqSerFree(void *voidPrimitivePointer)
{
    CsrDspmOperatorMessageReq *primitive = (CsrDspmOperatorMessageReq *) voidPrimitivePointer;
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorMessageInfoCount; i1++)
        {
            CsrPmemFree(primitive->operatorMessageInfo[i1].message);
        }
    }
    CsrPmemFree(primitive->operatorMessageInfo);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmOperatorStartReqSizeof(void *msg)
{
    CsrDspmOperatorStartReq *primitive = (CsrDspmOperatorStartReq *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 8) */
    bufferSize += 2; /* CsrSchedQid primitive->qid */
    bufferSize += 1; /* CsrUint8 primitive->opidCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            bufferSize += 2; /* CsrDspmOpid primitive->opid */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmOperatorStartReqSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmOperatorStartReq *primitive = (CsrDspmOperatorStartReq *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->qid);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->opidCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->opid[i1]);
        }
    }
    return ptr;
}

void *CsrDspmOperatorStartReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmOperatorStartReq *primitive = (CsrDspmOperatorStartReq *) CsrPmemAlloc(sizeof(CsrDspmOperatorStartReq));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->opidCount, buffer, &offset);
    primitive->opid = NULL;
    if (primitive->opidCount)
    {
        primitive->opid = (CsrDspmOpid *) CsrPmemAlloc(sizeof(CsrDspmOpid) * primitive->opidCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->opid[i1], buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmOperatorStartReqSerFree(void *voidPrimitivePointer)
{
    CsrDspmOperatorStartReq *primitive = (CsrDspmOperatorStartReq *) voidPrimitivePointer;
    CsrPmemFree(primitive->opid);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmOperatorStopReqSizeof(void *msg)
{
    CsrDspmOperatorStopReq *primitive = (CsrDspmOperatorStopReq *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 8) */
    bufferSize += 2; /* CsrSchedQid primitive->qid */
    bufferSize += 1; /* CsrUint8 primitive->opidCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            bufferSize += 2; /* CsrDspmOpid primitive->opid */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmOperatorStopReqSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmOperatorStopReq *primitive = (CsrDspmOperatorStopReq *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->qid);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->opidCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->opid[i1]);
        }
    }
    return ptr;
}

void *CsrDspmOperatorStopReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmOperatorStopReq *primitive = (CsrDspmOperatorStopReq *) CsrPmemAlloc(sizeof(CsrDspmOperatorStopReq));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->opidCount, buffer, &offset);
    primitive->opid = NULL;
    if (primitive->opidCount)
    {
        primitive->opid = (CsrDspmOpid *) CsrPmemAlloc(sizeof(CsrDspmOpid) * primitive->opidCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->opid[i1], buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmOperatorStopReqSerFree(void *voidPrimitivePointer)
{
    CsrDspmOperatorStopReq *primitive = (CsrDspmOperatorStopReq *) voidPrimitivePointer;
    CsrPmemFree(primitive->opid);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmOperatorResetReqSizeof(void *msg)
{
    CsrDspmOperatorResetReq *primitive = (CsrDspmOperatorResetReq *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 8) */
    bufferSize += 2; /* CsrSchedQid primitive->qid */
    bufferSize += 1; /* CsrUint8 primitive->opidCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            bufferSize += 2; /* CsrDspmOpid primitive->opid */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmOperatorResetReqSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmOperatorResetReq *primitive = (CsrDspmOperatorResetReq *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->qid);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->opidCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->opid[i1]);
        }
    }
    return ptr;
}

void *CsrDspmOperatorResetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmOperatorResetReq *primitive = (CsrDspmOperatorResetReq *) CsrPmemAlloc(sizeof(CsrDspmOperatorResetReq));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->opidCount, buffer, &offset);
    primitive->opid = NULL;
    if (primitive->opidCount)
    {
        primitive->opid = (CsrDspmOpid *) CsrPmemAlloc(sizeof(CsrDspmOpid) * primitive->opidCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->opid[i1], buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmOperatorResetReqSerFree(void *voidPrimitivePointer)
{
    CsrDspmOperatorResetReq *primitive = (CsrDspmOperatorResetReq *) voidPrimitivePointer;
    CsrPmemFree(primitive->opid);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmConnectionCreateReqSizeof(void *msg)
{
    CsrDspmConnectionCreateReq *primitive = (CsrDspmConnectionCreateReq *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 12) */
    bufferSize += 2; /* CsrSchedQid primitive->qid */
    bufferSize += 1; /* CsrUint8 primitive->connectionInfoCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->connectionInfoCount; i1++)
        {
            bufferSize += 2; /* CsrDspmCid primitive->connectionInfo[i1].cid */
            bufferSize += 2; /* CsrDspmCpid primitive->connectionInfo[i1].cpidSource */
            bufferSize += 2; /* CsrDspmCpid primitive->connectionInfo[i1].cpidSink */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmConnectionCreateReqSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmConnectionCreateReq *primitive = (CsrDspmConnectionCreateReq *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->qid);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->connectionInfoCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->connectionInfoCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->connectionInfo[i1].cid);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->connectionInfo[i1].cpidSource);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->connectionInfo[i1].cpidSink);
        }
    }
    return ptr;
}

void *CsrDspmConnectionCreateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmConnectionCreateReq *primitive = (CsrDspmConnectionCreateReq *) CsrPmemAlloc(sizeof(CsrDspmConnectionCreateReq));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionInfoCount, buffer, &offset);
    primitive->connectionInfo = NULL;
    if (primitive->connectionInfoCount)
    {
        primitive->connectionInfo = (CsrDspmConnectionInfo *) CsrPmemAlloc(sizeof(CsrDspmConnectionInfo) * primitive->connectionInfoCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->connectionInfoCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->connectionInfo[i1].cid, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->connectionInfo[i1].cpidSource, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->connectionInfo[i1].cpidSink, buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmConnectionCreateReqSerFree(void *voidPrimitivePointer)
{
    CsrDspmConnectionCreateReq *primitive = (CsrDspmConnectionCreateReq *) voidPrimitivePointer;
    CsrPmemFree(primitive->connectionInfo);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmConnectionDestroyReqSizeof(void *msg)
{
    CsrDspmConnectionDestroyReq *primitive = (CsrDspmConnectionDestroyReq *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 8) */
    bufferSize += 2; /* CsrSchedQid primitive->qid */
    bufferSize += 1; /* CsrUint8 primitive->cidCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cidCount; i1++)
        {
            bufferSize += 2; /* CsrDspmCid primitive->cid */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmConnectionDestroyReqSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmConnectionDestroyReq *primitive = (CsrDspmConnectionDestroyReq *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->qid);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->cidCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cidCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->cid[i1]);
        }
    }
    return ptr;
}

void *CsrDspmConnectionDestroyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmConnectionDestroyReq *primitive = (CsrDspmConnectionDestroyReq *) CsrPmemAlloc(sizeof(CsrDspmConnectionDestroyReq));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cidCount, buffer, &offset);
    primitive->cid = NULL;
    if (primitive->cidCount)
    {
        primitive->cid = (CsrDspmCid *) CsrPmemAlloc(sizeof(CsrDspmCid) * primitive->cidCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cidCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->cid[i1], buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmConnectionDestroyReqSerFree(void *voidPrimitivePointer)
{
    CsrDspmConnectionDestroyReq *primitive = (CsrDspmConnectionDestroyReq *) voidPrimitivePointer;
    CsrPmemFree(primitive->cid);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmCapabilityDownloadReqSizeof(void *msg)
{
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 15) */
    bufferSize += 2; /* CsrSchedQid primitive->qid */
    bufferSize += 2; /* CsrDspmCapability primitive->capability */
    bufferSize += 2; /* CsrUint16 primitive->versionMajor */
    bufferSize += 2; /* CsrUint16 primitive->versionMinor */
    bufferSize += 4;
    return bufferSize;
}

CsrUint8 *CsrDspmCapabilityDownloadReqSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmCapabilityDownloadReq *primitive = (CsrDspmCapabilityDownloadReq *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->qid);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->capability);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->versionMajor);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->versionMinor);
    CsrVoidPtrSer(ptr, len, (void *) primitive->data);
    return ptr;
}

void *CsrDspmCapabilityDownloadReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmCapabilityDownloadReq *primitive = (CsrDspmCapabilityDownloadReq *) CsrPmemAlloc(sizeof(CsrDspmCapabilityDownloadReq));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->capability, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->versionMajor, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->versionMinor, buffer, &offset);
    CsrVoidPtrDes((void **) &primitive->data, buffer, &offset);

    return primitive;
}

CsrSize CsrDspmStreamCpOpenCfmSizeof(void *msg)
{
    CsrDspmStreamCpOpenCfm *primitive = (CsrDspmStreamCpOpenCfm *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 15) */
    bufferSize += 2; /* CsrResult primitive->result */
    bufferSize += 1; /* CsrUint8 primitive->successes */
    bufferSize += 1; /* CsrUint8 primitive->cpInfoCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cpInfoCount; i1++)
        {
            bufferSize += 2; /* CsrDspmCpid primitive->cpInfo[i1].cpid */
            bufferSize += 2; /* CsrDspmCpType primitive->cpInfo[i1].cpType */
            bufferSize += 2; /* CsrUint16 primitive->cpInfo[i1].instance */
            bufferSize += 2; /* CsrUint16 primitive->cpInfo[i1].channel */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmStreamCpOpenCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmStreamCpOpenCfm *primitive = (CsrDspmStreamCpOpenCfm *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->result);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->successes);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->cpInfoCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cpInfoCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->cpInfo[i1].cpid);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->cpInfo[i1].cpType);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->cpInfo[i1].instance);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->cpInfo[i1].channel);
        }
    }
    return ptr;
}

void *CsrDspmStreamCpOpenCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmStreamCpOpenCfm *primitive = (CsrDspmStreamCpOpenCfm *) CsrPmemAlloc(sizeof(CsrDspmStreamCpOpenCfm));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->successes, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cpInfoCount, buffer, &offset);
    primitive->cpInfo = NULL;
    if (primitive->cpInfoCount)
    {
        primitive->cpInfo = (CsrDspmCpInfo *) CsrPmemAlloc(sizeof(CsrDspmCpInfo) * primitive->cpInfoCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cpInfoCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->cpInfo[i1].cpid, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->cpInfo[i1].cpType, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->cpInfo[i1].instance, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->cpInfo[i1].channel, buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmStreamCpOpenCfmSerFree(void *voidPrimitivePointer)
{
    CsrDspmStreamCpOpenCfm *primitive = (CsrDspmStreamCpOpenCfm *) voidPrimitivePointer;
    CsrPmemFree(primitive->cpInfo);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmStreamCpCloseCfmSizeof(void *msg)
{
    CsrDspmStreamCpCloseCfm *primitive = (CsrDspmStreamCpCloseCfm *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 6) */
    bufferSize += 1; /* CsrUint8 primitive->cpidCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cpidCount; i1++)
        {
            bufferSize += 2; /* CsrDspmCpid primitive->cpid */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmStreamCpCloseCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmStreamCpCloseCfm *primitive = (CsrDspmStreamCpCloseCfm *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->cpidCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cpidCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->cpid[i1]);
        }
    }
    return ptr;
}

void *CsrDspmStreamCpCloseCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmStreamCpCloseCfm *primitive = (CsrDspmStreamCpCloseCfm *) CsrPmemAlloc(sizeof(CsrDspmStreamCpCloseCfm));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cpidCount, buffer, &offset);
    primitive->cpid = NULL;
    if (primitive->cpidCount)
    {
        primitive->cpid = (CsrDspmCpid *) CsrPmemAlloc(sizeof(CsrDspmCpid) * primitive->cpidCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cpidCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->cpid[i1], buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmStreamCpCloseCfmSerFree(void *voidPrimitivePointer)
{
    CsrDspmStreamCpCloseCfm *primitive = (CsrDspmStreamCpCloseCfm *) voidPrimitivePointer;
    CsrPmemFree(primitive->cpid);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmStreamConfigureCfmSizeof(void *msg)
{
    CsrDspmStreamConfigureCfm *primitive = (CsrDspmStreamConfigureCfm *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 15) */
    bufferSize += 2; /* CsrResult primitive->result */
    bufferSize += 1; /* CsrUint8 primitive->successes */
    bufferSize += 1; /* CsrUint8 primitive->configInfoCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->configInfoCount; i1++)
        {
            bufferSize += 2; /* CsrDspmCpid primitive->configInfo[i1].cpid */
            bufferSize += 2; /* CsrUint16 primitive->configInfo[i1].feature */
            bufferSize += 4; /* CsrUint32 primitive->configInfo[i1].value */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmStreamConfigureCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmStreamConfigureCfm *primitive = (CsrDspmStreamConfigureCfm *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->result);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->successes);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->configInfoCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->configInfoCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->configInfo[i1].cpid);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->configInfo[i1].feature);
            CsrUint32Ser(ptr, len, (CsrUint32) primitive->configInfo[i1].value);
        }
    }
    return ptr;
}

void *CsrDspmStreamConfigureCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmStreamConfigureCfm *primitive = (CsrDspmStreamConfigureCfm *) CsrPmemAlloc(sizeof(CsrDspmStreamConfigureCfm));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->successes, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->configInfoCount, buffer, &offset);
    primitive->configInfo = NULL;
    if (primitive->configInfoCount)
    {
        primitive->configInfo = (CsrDspmConfigInfo *) CsrPmemAlloc(sizeof(CsrDspmConfigInfo) * primitive->configInfoCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->configInfoCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->configInfo[i1].cpid, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->configInfo[i1].feature, buffer, &offset);
            CsrUint32Des((CsrUint32 *) &primitive->configInfo[i1].value, buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmStreamConfigureCfmSerFree(void *voidPrimitivePointer)
{
    CsrDspmStreamConfigureCfm *primitive = (CsrDspmStreamConfigureCfm *) voidPrimitivePointer;
    CsrPmemFree(primitive->configInfo);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmStreamSyncCfmSizeof(void *msg)
{
    CsrDspmStreamSyncCfm *primitive = (CsrDspmStreamSyncCfm *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 11) */
    bufferSize += 2; /* CsrResult primitive->result */
    bufferSize += 1; /* CsrUint8 primitive->successes */
    bufferSize += 1; /* CsrUint8 primitive->syncInfoCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->syncInfoCount; i1++)
        {
            bufferSize += 2; /* CsrDspmCpid primitive->syncInfo[i1].cpid1 */
            bufferSize += 2; /* CsrDspmCpid primitive->syncInfo[i1].cpid2 */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmStreamSyncCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmStreamSyncCfm *primitive = (CsrDspmStreamSyncCfm *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->result);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->successes);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->syncInfoCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->syncInfoCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->syncInfo[i1].cpid1);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->syncInfo[i1].cpid2);
        }
    }
    return ptr;
}

void *CsrDspmStreamSyncCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmStreamSyncCfm *primitive = (CsrDspmStreamSyncCfm *) CsrPmemAlloc(sizeof(CsrDspmStreamSyncCfm));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->successes, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->syncInfoCount, buffer, &offset);
    primitive->syncInfo = NULL;
    if (primitive->syncInfoCount)
    {
        primitive->syncInfo = (CsrDspmSyncInfo *) CsrPmemAlloc(sizeof(CsrDspmSyncInfo) * primitive->syncInfoCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->syncInfoCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->syncInfo[i1].cpid1, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->syncInfo[i1].cpid2, buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmStreamSyncCfmSerFree(void *voidPrimitivePointer)
{
    CsrDspmStreamSyncCfm *primitive = (CsrDspmStreamSyncCfm *) voidPrimitivePointer;
    CsrPmemFree(primitive->syncInfo);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmOperatorCreateCfmSizeof(void *msg)
{
    CsrDspmOperatorCreateCfm *primitive = (CsrDspmOperatorCreateCfm *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 11) */
    bufferSize += 2; /* CsrResult primitive->result */
    bufferSize += 1; /* CsrUint8 primitive->successes */
    bufferSize += 1; /* CsrUint8 primitive->operatorInfoCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorInfoCount; i1++)
        {
            bufferSize += 2; /* CsrDspmOpid primitive->operatorInfo[i1].opid */
            bufferSize += 2; /* CsrDspmCapability primitive->operatorInfo[i1].capability */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmOperatorCreateCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmOperatorCreateCfm *primitive = (CsrDspmOperatorCreateCfm *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->result);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->successes);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->operatorInfoCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorInfoCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->operatorInfo[i1].opid);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->operatorInfo[i1].capability);
        }
    }
    return ptr;
}

void *CsrDspmOperatorCreateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmOperatorCreateCfm *primitive = (CsrDspmOperatorCreateCfm *) CsrPmemAlloc(sizeof(CsrDspmOperatorCreateCfm));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->successes, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->operatorInfoCount, buffer, &offset);
    primitive->operatorInfo = NULL;
    if (primitive->operatorInfoCount)
    {
        primitive->operatorInfo = (CsrDspmOperatorInfo *) CsrPmemAlloc(sizeof(CsrDspmOperatorInfo) * primitive->operatorInfoCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorInfoCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->operatorInfo[i1].opid, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->operatorInfo[i1].capability, buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmOperatorCreateCfmSerFree(void *voidPrimitivePointer)
{
    CsrDspmOperatorCreateCfm *primitive = (CsrDspmOperatorCreateCfm *) voidPrimitivePointer;
    CsrPmemFree(primitive->operatorInfo);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmOperatorDestroyCfmSizeof(void *msg)
{
    CsrDspmOperatorDestroyCfm *primitive = (CsrDspmOperatorDestroyCfm *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 9) */
    bufferSize += 2; /* CsrResult primitive->result */
    bufferSize += 1; /* CsrUint8 primitive->successes */
    bufferSize += 1; /* CsrUint8 primitive->opidCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            bufferSize += 2; /* CsrDspmOpid primitive->opid */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmOperatorDestroyCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmOperatorDestroyCfm *primitive = (CsrDspmOperatorDestroyCfm *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->result);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->successes);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->opidCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->opid[i1]);
        }
    }
    return ptr;
}

void *CsrDspmOperatorDestroyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmOperatorDestroyCfm *primitive = (CsrDspmOperatorDestroyCfm *) CsrPmemAlloc(sizeof(CsrDspmOperatorDestroyCfm));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->successes, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->opidCount, buffer, &offset);
    primitive->opid = NULL;
    if (primitive->opidCount)
    {
        primitive->opid = (CsrDspmOpid *) CsrPmemAlloc(sizeof(CsrDspmOpid) * primitive->opidCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->opid[i1], buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmOperatorDestroyCfmSerFree(void *voidPrimitivePointer)
{
    CsrDspmOperatorDestroyCfm *primitive = (CsrDspmOperatorDestroyCfm *) voidPrimitivePointer;
    CsrPmemFree(primitive->opid);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmOperatorMessageCfmSizeof(void *msg)
{
    CsrDspmOperatorMessageCfm *primitive = (CsrDspmOperatorMessageCfm *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 13) */
    bufferSize += 2; /* CsrResult primitive->result */
    bufferSize += 1; /* CsrUint8 primitive->successes */
    bufferSize += 1; /* CsrUint8 primitive->operatorMessageInfoCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorMessageInfoCount; i1++)
        {
            bufferSize += 2; /* CsrDspmOpid primitive->operatorMessageInfo[i1].opid */
            bufferSize += 2; /* CsrUint16 primitive->operatorMessageInfo[i1].messageLength */
            {
                CsrUint16 i2;
                for (i2 = 0; i2 < primitive->operatorMessageInfo[i1].messageLength; i2++)
                {
                    bufferSize += 2; /* CsrUint16 primitive->operatorMessageInfo[i1].message */
                }
            }
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmOperatorMessageCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmOperatorMessageCfm *primitive = (CsrDspmOperatorMessageCfm *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->result);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->successes);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->operatorMessageInfoCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorMessageInfoCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->operatorMessageInfo[i1].opid);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->operatorMessageInfo[i1].messageLength);
            {
                CsrUint16 i2;
                for (i2 = 0; i2 < primitive->operatorMessageInfo[i1].messageLength; i2++)
                {
                    CsrUint16Ser(ptr, len, (CsrUint16) primitive->operatorMessageInfo[i1].message[i2]);
                }
            }
        }
    }
    return ptr;
}

void *CsrDspmOperatorMessageCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmOperatorMessageCfm *primitive = (CsrDspmOperatorMessageCfm *) CsrPmemAlloc(sizeof(CsrDspmOperatorMessageCfm));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->successes, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->operatorMessageInfoCount, buffer, &offset);
    primitive->operatorMessageInfo = NULL;
    if (primitive->operatorMessageInfoCount)
    {
        primitive->operatorMessageInfo = (CsrDspmOperatorMessageInfo *) CsrPmemAlloc(sizeof(CsrDspmOperatorMessageInfo) * primitive->operatorMessageInfoCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorMessageInfoCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->operatorMessageInfo[i1].opid, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->operatorMessageInfo[i1].messageLength, buffer, &offset);
            primitive->operatorMessageInfo[i1].message = NULL;
            if (primitive->operatorMessageInfo[i1].messageLength)
            {
                primitive->operatorMessageInfo[i1].message = (CsrUint16 *) CsrPmemAlloc(sizeof(CsrUint16) * primitive->operatorMessageInfo[i1].messageLength);
            }
            {
                CsrUint16 i2;
                for (i2 = 0; i2 < primitive->operatorMessageInfo[i1].messageLength; i2++)
                {
                    CsrUint16Des((CsrUint16 *) &primitive->operatorMessageInfo[i1].message[i2], buffer, &offset);
                }
            }
        }
    }

    return primitive;
}

void CsrDspmOperatorMessageCfmSerFree(void *voidPrimitivePointer)
{
    CsrDspmOperatorMessageCfm *primitive = (CsrDspmOperatorMessageCfm *) voidPrimitivePointer;
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->operatorMessageInfoCount; i1++)
        {
            CsrPmemFree(primitive->operatorMessageInfo[i1].message);
        }
    }
    CsrPmemFree(primitive->operatorMessageInfo);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmOperatorMessageIndSizeof(void *msg)
{
    CsrDspmOperatorMessageInd *primitive = (CsrDspmOperatorMessageInd *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 9) */
    bufferSize += 2; /* CsrDspmOpid primitive->opid */
    bufferSize += 2; /* CsrUint16 primitive->messageLength */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->messageLength; i1++)
        {
            bufferSize += 2; /* CsrUint16 primitive->message */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmOperatorMessageIndSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmOperatorMessageInd *primitive = (CsrDspmOperatorMessageInd *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->opid);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->messageLength);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->messageLength; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->message[i1]);
        }
    }
    return ptr;
}

void *CsrDspmOperatorMessageIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmOperatorMessageInd *primitive = (CsrDspmOperatorMessageInd *) CsrPmemAlloc(sizeof(CsrDspmOperatorMessageInd));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->opid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->messageLength, buffer, &offset);
    primitive->message = NULL;
    if (primitive->messageLength)
    {
        primitive->message = (CsrUint16 *) CsrPmemAlloc(sizeof(CsrUint16) * primitive->messageLength);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->messageLength; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->message[i1], buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmOperatorMessageIndSerFree(void *voidPrimitivePointer)
{
    CsrDspmOperatorMessageInd *primitive = (CsrDspmOperatorMessageInd *) voidPrimitivePointer;
    CsrPmemFree(primitive->message);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmOperatorStartCfmSizeof(void *msg)
{
    CsrDspmOperatorStartCfm *primitive = (CsrDspmOperatorStartCfm *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 9) */
    bufferSize += 2; /* CsrResult primitive->result */
    bufferSize += 1; /* CsrUint8 primitive->successes */
    bufferSize += 1; /* CsrUint8 primitive->opidCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            bufferSize += 2; /* CsrDspmOpid primitive->opid */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmOperatorStartCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmOperatorStartCfm *primitive = (CsrDspmOperatorStartCfm *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->result);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->successes);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->opidCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->opid[i1]);
        }
    }
    return ptr;
}

void *CsrDspmOperatorStartCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmOperatorStartCfm *primitive = (CsrDspmOperatorStartCfm *) CsrPmemAlloc(sizeof(CsrDspmOperatorStartCfm));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->successes, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->opidCount, buffer, &offset);
    primitive->opid = NULL;
    if (primitive->opidCount)
    {
        primitive->opid = (CsrDspmOpid *) CsrPmemAlloc(sizeof(CsrDspmOpid) * primitive->opidCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->opid[i1], buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmOperatorStartCfmSerFree(void *voidPrimitivePointer)
{
    CsrDspmOperatorStartCfm *primitive = (CsrDspmOperatorStartCfm *) voidPrimitivePointer;
    CsrPmemFree(primitive->opid);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmOperatorStopCfmSizeof(void *msg)
{
    CsrDspmOperatorStopCfm *primitive = (CsrDspmOperatorStopCfm *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 9) */
    bufferSize += 2; /* CsrResult primitive->result */
    bufferSize += 1; /* CsrUint8 primitive->successes */
    bufferSize += 1; /* CsrUint8 primitive->opidCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            bufferSize += 2; /* CsrDspmOpid primitive->opid */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmOperatorStopCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmOperatorStopCfm *primitive = (CsrDspmOperatorStopCfm *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->result);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->successes);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->opidCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->opid[i1]);
        }
    }
    return ptr;
}

void *CsrDspmOperatorStopCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmOperatorStopCfm *primitive = (CsrDspmOperatorStopCfm *) CsrPmemAlloc(sizeof(CsrDspmOperatorStopCfm));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->successes, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->opidCount, buffer, &offset);
    primitive->opid = NULL;
    if (primitive->opidCount)
    {
        primitive->opid = (CsrDspmOpid *) CsrPmemAlloc(sizeof(CsrDspmOpid) * primitive->opidCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->opid[i1], buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmOperatorStopCfmSerFree(void *voidPrimitivePointer)
{
    CsrDspmOperatorStopCfm *primitive = (CsrDspmOperatorStopCfm *) voidPrimitivePointer;
    CsrPmemFree(primitive->opid);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmOperatorResetCfmSizeof(void *msg)
{
    CsrDspmOperatorResetCfm *primitive = (CsrDspmOperatorResetCfm *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 9) */
    bufferSize += 2; /* CsrResult primitive->result */
    bufferSize += 1; /* CsrUint8 primitive->successes */
    bufferSize += 1; /* CsrUint8 primitive->opidCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            bufferSize += 2; /* CsrDspmOpid primitive->opid */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmOperatorResetCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmOperatorResetCfm *primitive = (CsrDspmOperatorResetCfm *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->result);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->successes);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->opidCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->opid[i1]);
        }
    }
    return ptr;
}

void *CsrDspmOperatorResetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmOperatorResetCfm *primitive = (CsrDspmOperatorResetCfm *) CsrPmemAlloc(sizeof(CsrDspmOperatorResetCfm));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->successes, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->opidCount, buffer, &offset);
    primitive->opid = NULL;
    if (primitive->opidCount)
    {
        primitive->opid = (CsrDspmOpid *) CsrPmemAlloc(sizeof(CsrDspmOpid) * primitive->opidCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->opidCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->opid[i1], buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmOperatorResetCfmSerFree(void *voidPrimitivePointer)
{
    CsrDspmOperatorResetCfm *primitive = (CsrDspmOperatorResetCfm *) voidPrimitivePointer;
    CsrPmemFree(primitive->opid);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmConnectionCreateCfmSizeof(void *msg)
{
    CsrDspmConnectionCreateCfm *primitive = (CsrDspmConnectionCreateCfm *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 13) */
    bufferSize += 2; /* CsrResult primitive->result */
    bufferSize += 1; /* CsrUint8 primitive->successes */
    bufferSize += 1; /* CsrUint8 primitive->connectionInfoCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->connectionInfoCount; i1++)
        {
            bufferSize += 2; /* CsrDspmCid primitive->connectionInfo[i1].cid */
            bufferSize += 2; /* CsrDspmCpid primitive->connectionInfo[i1].cpidSource */
            bufferSize += 2; /* CsrDspmCpid primitive->connectionInfo[i1].cpidSink */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmConnectionCreateCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmConnectionCreateCfm *primitive = (CsrDspmConnectionCreateCfm *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->result);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->successes);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->connectionInfoCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->connectionInfoCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->connectionInfo[i1].cid);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->connectionInfo[i1].cpidSource);
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->connectionInfo[i1].cpidSink);
        }
    }
    return ptr;
}

void *CsrDspmConnectionCreateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmConnectionCreateCfm *primitive = (CsrDspmConnectionCreateCfm *) CsrPmemAlloc(sizeof(CsrDspmConnectionCreateCfm));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->successes, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionInfoCount, buffer, &offset);
    primitive->connectionInfo = NULL;
    if (primitive->connectionInfoCount)
    {
        primitive->connectionInfo = (CsrDspmConnectionInfo *) CsrPmemAlloc(sizeof(CsrDspmConnectionInfo) * primitive->connectionInfoCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->connectionInfoCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->connectionInfo[i1].cid, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->connectionInfo[i1].cpidSource, buffer, &offset);
            CsrUint16Des((CsrUint16 *) &primitive->connectionInfo[i1].cpidSink, buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmConnectionCreateCfmSerFree(void *voidPrimitivePointer)
{
    CsrDspmConnectionCreateCfm *primitive = (CsrDspmConnectionCreateCfm *) voidPrimitivePointer;
    CsrPmemFree(primitive->connectionInfo);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmConnectionDestroyCfmSizeof(void *msg)
{
    CsrDspmConnectionDestroyCfm *primitive = (CsrDspmConnectionDestroyCfm *) msg;
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 6) */
    bufferSize += 1; /* CsrUint8 primitive->cidCount */
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cidCount; i1++)
        {
            bufferSize += 2; /* CsrDspmCid primitive->cid */
        }
    }
    return bufferSize;
}

CsrUint8 *CsrDspmConnectionDestroyCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmConnectionDestroyCfm *primitive = (CsrDspmConnectionDestroyCfm *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->cidCount);
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cidCount; i1++)
        {
            CsrUint16Ser(ptr, len, (CsrUint16) primitive->cid[i1]);
        }
    }
    return ptr;
}

void *CsrDspmConnectionDestroyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmConnectionDestroyCfm *primitive = (CsrDspmConnectionDestroyCfm *) CsrPmemAlloc(sizeof(CsrDspmConnectionDestroyCfm));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cidCount, buffer, &offset);
    primitive->cid = NULL;
    if (primitive->cidCount)
    {
        primitive->cid = (CsrDspmCid *) CsrPmemAlloc(sizeof(CsrDspmCid) * primitive->cidCount);
    }
    {
        CsrUint16 i1;
        for (i1 = 0; i1 < primitive->cidCount; i1++)
        {
            CsrUint16Des((CsrUint16 *) &primitive->cid[i1], buffer, &offset);
        }
    }

    return primitive;
}

void CsrDspmConnectionDestroyCfmSerFree(void *voidPrimitivePointer)
{
    CsrDspmConnectionDestroyCfm *primitive = (CsrDspmConnectionDestroyCfm *) voidPrimitivePointer;
    CsrPmemFree(primitive->cid);
    CsrPmemFree(primitive);
}

CsrSize CsrDspmCapabilityDownloadCfmSizeof(void *msg)
{
    CsrSize bufferSize = 2;

    /* Calculate the Size of the Serialised Data. Could be more efficient (Try 6) */
    bufferSize += 1; /* CsrUint8 primitive->result */
    bufferSize += 2; /* CsrDspmCapability primitive->capability */
    return bufferSize;
}

CsrUint8 *CsrDspmCapabilityDownloadCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg)
{
    CsrDspmCapabilityDownloadCfm *primitive = (CsrDspmCapabilityDownloadCfm *) msg;
    *len = 0;
    CsrUint16Ser(ptr, len, primitive->type);
    CsrUint8Ser(ptr, len, (CsrUint8) primitive->result);
    CsrUint16Ser(ptr, len, (CsrUint16) primitive->capability);
    return ptr;
}

void *CsrDspmCapabilityDownloadCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDspmCapabilityDownloadCfm *primitive = (CsrDspmCapabilityDownloadCfm *) CsrPmemAlloc(sizeof(CsrDspmCapabilityDownloadCfm));
    CsrSize offset;
    offset = 0;

    CsrUint16Des(&primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->capability, buffer, &offset);

    return primitive;
}

#endif /* EXCLUDE_CSR_DSPM_MODULE */

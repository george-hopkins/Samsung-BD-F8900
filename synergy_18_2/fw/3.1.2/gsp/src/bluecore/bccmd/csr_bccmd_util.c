/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_bccmd_handler.h"
#include "csr_macro.h"
#include "csr_util.h"
#include "csr_log_text_2.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log_gsp.h"
#endif

#define CSR_PERSISTENT_STORE_HEADER_SIZE    ((CsrUint8) 0x06)
#define CSR_VARID_PERSISTENT_STORE          ((CsrUint16) 0x7003)

CsrUint8 *CsrBccmdBccmdCommandBuild(CsrUint16 type,
    CsrUint16 varId,
    CsrUint16 seqNo,
    CsrUint16 payloadLength,
    CsrUint8 *payload,
    CsrUint16 *bccmdLength)
{
    CsrUint8 *bccmd, *tmpPayload;
    CsrUint16 bccmdLen = CSR_BCCMD_STD_HEADER_SIZE;

    if (payload && (payloadLength > 0))
    {
        bccmdLen = (CsrUint16) (bccmdLen + payloadLength);
    }
    else
    {
        payloadLength = 0;
    }

    if (bccmdLen < CSR_BCCMD_MIN_SIZE)
    {
        bccmdLen = CSR_BCCMD_MIN_SIZE;
    }
    else
    {
        if ((bccmdLen & 0x0001) == 0x0001)
        {
            bccmdLen++;
        }
    }

    bccmd = (CsrUint8 *) CsrPmemZalloc(bccmdLen);
    tmpPayload = bccmd;

    CSR_ADD_UINT16_TO_XAP(tmpPayload, type);
    CSR_ADD_UINT16_TO_XAP(tmpPayload, (bccmdLen / 2));
    CSR_ADD_UINT16_TO_XAP(tmpPayload, seqNo);
    CSR_ADD_UINT16_TO_XAP(tmpPayload, varId);
    CSR_ADD_UINT16_TO_XAP(tmpPayload, CSR_RESULT_SUCCESS);

    if (payloadLength > 0)
    {
        CsrMemCpy(&(bccmd[CSR_BCCMD_STD_HEADER_SIZE]), payload, payloadLength);
    }
    CsrPmemFree(payload);
    *bccmdLength = bccmdLen;
    return bccmd;
}

CsrUint8 *CsrBccmdPsKeyCommandBuild(CsrUint16 type,
    CsrUint16 seqNo,
    CsrUint16 key,
    CsrBccmdStoresType stores,
    CsrUint16 psValueLength,
    CsrUint8 *psValue,
    CsrUint16 *bccmdLength)
{
    CsrUint16 payloadLength = (CsrUint16) (CSR_PERSISTENT_STORE_HEADER_SIZE + psValueLength);
    CsrUint8 *payload = (CsrUint8 *) CsrPmemZalloc(payloadLength);
    CsrUint8 *tmpPayload = payload;

    CSR_ADD_UINT16_TO_XAP(tmpPayload, key);
    CSR_ADD_UINT16_TO_XAP(tmpPayload, (CsrUint16) (psValueLength / 2));
    CSR_ADD_UINT16_TO_XAP(tmpPayload, stores);

    if (psValue)
    {
        CsrMemCpy(&(payload[CSR_PERSISTENT_STORE_HEADER_SIZE]),
            psValue,
            psValueLength);
    }
    CsrPmemFree(psValue);
    return CsrBccmdBccmdCommandBuild(type, CSR_VARID_PERSISTENT_STORE, seqNo, payloadLength, payload, bccmdLength);
}

#ifdef CSR_BLUECORE_ONOFF
void CsrBccmdDiscardSavedMessages(CsrBccmdInstanceData *bccmdInst)
{
    void *message;
    CsrUint16 event;
    while (CsrMessageQueuePop(&bccmdInst->messageQueue, &event, &message))
    {
        CSR_LOG_TEXT_ASSERT(CsrBccmdLto, 0, event == CSR_BCCMD_PRIM);
        CsrBccmdFreeDownstreamMessageContents(CSR_BCCMD_PRIM, message);
        CsrPmemFree(message);
    }
}
#endif

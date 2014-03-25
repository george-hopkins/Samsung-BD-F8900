/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_common.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_tpt_cmd.h"

/* Handle the primitive types. */

void tptUint16Write(CsrUint8 *ptr, CsrUint16 *offset, CsrUint16 value)
{
    ptr[0+(*offset)] = (CsrUint8)(value & 0x00FF);
    ptr[1+(*offset)] = (CsrUint8)(value >> 8);
    *offset += 2;
}

void tptUint32Write(CsrUint8 *ptr, CsrUint16 *offset, CsrUint32 value)
{
    ptr[0+(*offset)] = (CsrUint8)(value & 0x00FF);
    ptr[1+(*offset)] = (CsrUint8)((value >> 8) & 0x000000FF);
    ptr[2+(*offset)] = (CsrUint8)((value >> 16) & 0x000000FF);
    ptr[3+(*offset)] = (CsrUint8)((value >> 24) & 0x000000FF);
    *offset += 4;
}

/*
 * Command payload stuff.
 */

void serializeTptSppTestParams(CsrUint8 *buf, CsrUint16 *offset, TptCommand_t *cmd)
{
    tptSppTestParams *param = cmd->paramPtr;

    tptUint32Write(buf, offset, param->packetCount);
    tptUint16Write(buf, offset, param->packetSize);
    tptUint16Write(buf, offset, param->sppFlags);

    return;
}

void deserializeTptSppTestParams(TptCommand_t *prim, CsrUint16 *offset, CsrUint8 *buf)
{
    tptSppTestParams *pa = CsrPmemAlloc(sizeof(*pa));

    pa->packetCount = UINT32_R(buf, *offset);
    *offset += sizeof(pa->packetCount);

    pa->packetSize = UINT16_R(buf, *offset);
    *offset += sizeof(pa->packetSize);

    pa->sppFlags = UINT16_R(buf, *offset);
    *offset += sizeof(pa->sppFlags);

    prim->paramPtr = pa;

    return;
}

void serializeTptAvTestParams(CsrUint8 *buf, CsrUint16 *offset, TptCommand_t *cmd)
{
    tptAvTestParams *param = cmd->paramPtr;

    tptUint32Write(buf, offset, param->packetCount);
    tptUint16Write(buf, offset, param->packetSize);
    tptUint16Write(buf, offset, param->avFlags);

    return;
}

void deserializeTptAvTestParams(TptCommand_t *prim, CsrUint16 *offset, CsrUint8 *buf)
{
    tptAvTestParams *pa = CsrPmemAlloc(sizeof(*pa));

    pa->packetCount = UINT32_R(buf, *offset);
    *offset += sizeof(pa->packetCount);

    pa->packetSize = UINT16_R(buf, *offset);
    *offset += sizeof(pa->packetSize);

    pa->avFlags = UINT16_R(buf, *offset);
    *offset += sizeof(pa->avFlags);

    prim->paramPtr = pa;

    return;
}

void serializeTptReportResult(CsrUint8 *buf, CsrUint16 *offset, TptCommand_t *cmd)
{
    tptReportResult *res = cmd->paramPtr;
    CsrUint32        result = 0;
    CsrUint32        numOfResults = 0;

    numOfResults = (CsrUint32)(cmd->paramLen / (sizeof(res->timeStamp[0]) + sizeof(res->seqno[0])));
    tptUint32Write(buf, offset, numOfResults);

    while (result < numOfResults)
    {
        tptUint32Write(buf, offset, res->timeStamp[result]);
        tptUint32Write(buf, offset, res->seqno[result]);
        result++;
    }

    return;
}

void deserializeTptReportResult(TptCommand_t *prim, CsrUint16 *offset, CsrUint8 *buf)
{
    tptReportResult *res;
    CsrUint32        resultCount;
    CsrUint32        idx = 0;

    resultCount = UINT32_R(buf, *offset);
    *offset += sizeof(resultCount);

    res = CsrPmemAlloc(sizeof(tptReportResult));
    res->resultCount = resultCount;
    res->timeStamp = CsrPmemAlloc(resultCount * sizeof(res->timeStamp[0]));
    res->seqno = CsrPmemAlloc(resultCount * sizeof(res->seqno[0]));

    while (idx < resultCount)
    {
        res->timeStamp[idx] = UINT32_R(buf, *offset);
        *offset += sizeof(res->timeStamp[0]);
        res->seqno[idx] = UINT32_R(buf, *offset);
        *offset += sizeof(res->seqno[0]);
        idx++;
    }

    prim->paramPtr = res;
}

/*
 * Payload arbiters.
 */

void serializeTptCommandPayload(CsrUint8 *buf, CsrUint16 *offset, TptCommand_t *cmd)
{
    switch (cmd->cmdOpcode)
    {
        case TPT_CMD_SETUP_REQ:
            {
                if (cmd->testType & TPT_TEST_SPP)
                {
                    serializeTptSppTestParams(buf, offset, cmd);
                }
                else if (cmd->testType & TPT_TEST_AV)
                {
                    serializeTptAvTestParams(buf, offset, cmd);
                }
                break;
            }
        case TPT_CMD_RESULT_IND:
            {
                serializeTptReportResult(buf, offset, cmd);
                break;
            }
        default:
            {
                /* Error.  This message is not supposed to have any payload. */
                break;
            }
    }

    return;
}

void deserializeTptCommandPayload(TptCommand_t *prim, CsrUint16 *offset, CsrUint8 *buf)
{
    switch (prim->cmdOpcode)
    {
        case TPT_CMD_SETUP_REQ:
            {
                if (prim->testType & TPT_TEST_SPP)
                {
                    deserializeTptSppTestParams(prim, offset, buf);
                }
                else if (prim->testType & TPT_TEST_AV)
                {
                    deserializeTptAvTestParams(prim, offset, buf);
                }
                break;
            }
        case TPT_CMD_RESULT_IND:
            {
                deserializeTptReportResult(prim, offset, buf);
                break;
            }
        default:
            {
                /* Error.  This message is not supposed to have any payload. */
                break;
            }
    }

    return;
}


/*
 * Command serialization.
 */

CsrUint8 *serializeTptCommand(TptCommand_t *cmd, CsrUint16 *len)
{
    CsrUint8 *primBuf = CsrPmemAlloc(sizeof(*cmd) + cmd->paramLen);
    CsrUint16 offset = 0;

    tptUint32Write(primBuf, &offset, cmd->cmdOpcode);
    tptUint32Write(primBuf, &offset, cmd->testType);
    tptUint32Write(primBuf, &offset, cmd->paramLen);

    if (cmd->paramLen)
    {
        serializeTptCommandPayload(primBuf, &offset, cmd);
    }

    *len = offset;

    return primBuf;
}

TptCommand_t *deserializeTptCommand(CsrUint8 *buf)
{
    TptCommand_t *prim;
    CsrUint16 offset = 0;

    prim = (TptCommand_t *) CsrPmemAlloc(sizeof(*prim));

    prim->paramLen = 0;
    prim->paramPtr = NULL;

    prim->cmdOpcode = UINT32_R(buf, offset);
    offset += sizeof(prim->cmdOpcode);

    prim->testType = UINT32_R(buf, offset);
    offset += sizeof(prim->testType);

    prim->paramLen = UINT32_R(buf, offset);
    offset += sizeof(prim->paramLen);

    if (prim->paramLen)
    {
        deserializeTptCommandPayload(prim, &offset, buf);
    }

    return prim;
}

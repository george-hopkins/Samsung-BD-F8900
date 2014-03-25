/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_panic.h"
#include "csr_log_pcap_common.h"
#include "csr_log_gsp.h"
#include "csr_log_private.h"
#include "csr_log_pcap.h"
#include "csr_usr_config.h"
#include "csr_msgconv.h"

#ifdef CSR_LOG_ENABLE

#define MEMCPY_U32_ITER_BE(buf, offset, max_len, val)           \
    do {                                                        \
        CSR_COPY_UINT32_TO_BIG_ENDIAN(val, (buf) + *(offset)); \
        *(offset) += sizeof(val);                               \
        CSR_LOG_ASSERT(*(offset) <= max_len);                   \
    } while (0)

void CsrLogPcapSerializePrim(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrMsgConvMsgEntry *msgEntry,
    CsrBool onlyPrimType,
    CsrBool limitPrim,
    const void *msg,
    CsrSize *actualOffset)
{
    const CsrUint8 serializerType = SYNERGY_SERIALIZER_TYPE_SER;

    if (onlyPrimType || !msgEntry || !msgEntry->serFunc)
    {
        if (msg)
        {
            CsrSize offsetNew;
            CsrUint16 msgType = *(CsrUint16 *) msg;

            MEMCPY_ITER(buf, offset, max_len, serializerType);

            offsetNew = *offset;
            CsrUint16Ser(buf, &offsetNew, msgType);

            *offset = offsetNew;
        }
        else
        {
            /* Do nothing */
        }
        *actualOffset = *offset;
    }
    else
    {
        CsrSize offsetClamped;
        CsrSize serializedLength;
        MEMCPY_ITER(buf, offset, max_len, serializerType);

        offsetClamped = *offset;
        msgEntry->serFunc(&buf[*offset], &serializedLength, (void *) msg);
        *offset += serializedLength;
        *actualOffset = *offset;

        if (limitPrim)
        {
            *offset = offsetClamped + CSRMIN((*offset - offsetClamped), CSR_LOG_PRIM_SIZE_UPPER_LIMIT);
        }
    }
}

CsrSize CsrLogPcapSizeofPrim(CsrMsgConvMsgEntry *msgEntry,
    CsrBool onlyPrimType,
    const void *msg)
{
    if (onlyPrimType || !msgEntry || !msgEntry->sizeofFunc)
    {
        if (msg)
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return msgEntry->sizeofFunc((void *) msg);
    }
}

/****************************************************************************
 * Helper methods
 ***************************************************************************/
static void csrLogPcapGetUtcTime(CsrUint32 *time_sec, CsrUint32 *time_usec)
{
#ifdef CSR_HAVE_64_BIT_INTEGERS
    CsrTime low;
    CsrTime high;
    CsrUint64 msecs;

    low = CsrTimeGet(&high);

    msecs = ((CsrUint64) high) << 32 | low;

    *time_sec = (CsrUint32) (msecs / 1000000);
    *time_usec = (CsrUint32) (msecs % 1000000);
#else
    CsrTimeUtc tod = {0};

    CsrTimeUtcGet(&tod, NULL, NULL);
    *time_sec = tod.sec;
    *time_usec = tod.msec * 1000;
#endif
}

CsrSize CsrLogPcapStrLen(const CsrCharString *str)
{
    if (str)
    {
        return CsrStrLen((CsrCharString *) str);
    }
    return 0;
}

/****************************************************************************
 * Write a PCAP global header
 ***************************************************************************/
void CsrLogPcapSerializeHdr(CsrUint8 *buf, CsrSize *offset, CsrSize max_len)
{
    CsrUint32 magicNumber = 0xa1b2c3d4;              /* magic number */
    CsrUint16 versionMajor = 2;                      /* major version number */
    CsrUint16 versionMinor = 4;                      /* minor version number */
    CsrInt32 thisZone = 0;                           /* GMT to local correction */
    CsrUint32 sigFigs = 0;                           /* accuracy of timestamps */
    CsrUint32 snapLen = PCAP_MAX_REC_SIZE;           /* CSRMAX length of captured packets, in octets */
    CsrUint32 network = 0xC9;                        /* data link type */

    MEMCPY_ITER(buf, offset, max_len, magicNumber);
    MEMCPY_ITER(buf, offset, max_len, versionMajor);
    MEMCPY_ITER(buf, offset, max_len, versionMinor);
    MEMCPY_ITER(buf, offset, max_len, thisZone);
    MEMCPY_ITER(buf, offset, max_len, sigFigs);
    MEMCPY_ITER(buf, offset, max_len, snapLen);
    MEMCPY_ITER(buf, offset, max_len, network);
}

CsrUint32 CsrLogPcapSizeofHdr(void)
{
    PcapHdr *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->magic_number) +
           sizeof(msg->version_major) +
           sizeof(msg->version_minor) +
           sizeof(msg->thiszone) +
           sizeof(msg->sigfigs) +
           sizeof(msg->snaplen) +
           sizeof(msg->network);
}

/****************************************************************************
 * Write a PCAP record header
 ***************************************************************************/
CsrSize CsrLogPcapSizeofRecHdr(void)
{
    PcapRecHdr *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->ts_sec) +
           sizeof(msg->ts_usec) +
           sizeof(msg->incl_len) +
           sizeof(msg->orig_len) +
           sizeof(msg->received);
}

void CsrLogPcapSerializeRecHdr(CsrUint8 *buf, CsrSize *offset, CsrUint32 max_len,
    CsrUint32 incl_len, CsrUint32 orig_len, CsrUint32 received)
{
    CsrUint32 time_sec;
    CsrUint32 time_usec;
    CsrUint32 recHdrSize = (CsrUint32) (CsrLogPcapSizeofRecHdr() - sizeof(received));

    /* PCAP hdr should not be accounted for as payload but direction should */
    orig_len -= recHdrSize;
    incl_len -= recHdrSize;

    /* Don't exceed PCAP_MAX_REC_SIZE */
    CSR_LOG_ASSERT(incl_len <= PCAP_MAX_REC_SIZE);
    CSR_LOG_ASSERT(orig_len <= PCAP_MAX_REC_SIZE);
    CSR_LOG_ASSERT(received <= 1);

    csrLogPcapGetUtcTime(&time_sec, &time_usec);

    MEMCPY_ITER(buf, offset, max_len, time_sec);
    MEMCPY_ITER(buf, offset, max_len, time_usec);
    MEMCPY_ITER(buf, offset, max_len, incl_len);
    MEMCPY_ITER(buf, offset, max_len, orig_len);
    MEMCPY_U32_ITER_BE(buf, offset, max_len, received);
}

void CsrLogPcapSerializeRecHdrNoDirection(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrSize incl_len, CsrSize orig_len)
{
    CsrLogPcapSerializeRecHdr(buf, offset, (CsrUint32) max_len, (CsrUint32) incl_len, (CsrUint32) orig_len, 0);
}

/****************************************************************************
 * Synergy format
 ***************************************************************************/
void CsrLogPcapSerializeSynergyVerHeader(CsrUint8 *buf, CsrSize *offset, CsrSize max_len)
{
    CsrUint8 hci_type = HCI_H4_TYPE_SYNERGY_V3;

    MEMCPY_ITER(buf, offset, max_len, hci_type);
}

CsrSize CsrLogPcapSizeofSynergyVerHeader(void)
{
    SynergyVerHeader *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->hci_type);
}

#endif /* CSR_LOG_ENABLE */

#ifdef CSR_LOG_ENABLE
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_logtransport.h"
#include "csr_log_btsnoop.h"
#include "csr_panic.h"
#include "csr_log_formats.h"
#include "csr_log_gsp.h"
#include "csr_log_private.h"

#define MEMCPY_ITER(buf, offset, max_len, val)          \
    do {                                                    \
        CsrMemCpy((buf) + *(offset), &(val), sizeof(val));  \
        *(offset) += sizeof(val);                           \
        CSR_LOG_ASSERT(*(offset) <= max_len);               \
    } while (0)

#define MEMCPY_PTR_NO_LEN_ITER(buf, offset, max_len, ptr_length, ptr)      \
    do {                                                                \
        if (!ptr || !ptr_length) {                                         \
            break;}                                                       \
                                                                    \
        CsrMemCpy((buf) + *(offset), ptr, ptr_length);                  \
        *(offset) += ptr_length;                                        \
        CSR_LOG_ASSERT(*(offset) <= max_len);                           \
    } while (0)

#define MEMCPY_U32_ITER_BE(buf, offset, max_len, val)           \
    do {                                                        \
        CSR_COPY_UINT32_TO_BIG_ENDIAN(val, (buf) + *(offset)); \
        *(offset) += sizeof(val);                               \
        CSR_LOG_ASSERT(*(offset) <= max_len);                   \
    } while (0)

#define CSR_LOG_BTSNOOP_TRANSPORT_ALLOC(buf, handle, length) \
    buf = CsrLogTransportAlloc(handle, length);  \
    if (!buf)                                   \
    {                                           \
        return;                                 \
    }

/* HCI packet types */
#define HCI_H4_TYPE_CMD       0x01
#define HCI_H4_TYPE_ACL       0x02
#define HCI_H4_TYPE_SCO       0x03
#define HCI_H4_TYPE_EVT       0x04

static const CsrUint8 btsnoop_magic[] = {0x62, 0x74, 0x73, 0x6E, 0x6F, 0x6F, 0x70, 0x00};

typedef struct
{
    CsrUint8  magic[sizeof(btsnoop_magic)];
    CsrUint32 ver_no;
    CsrUint32 dlt;
} btsnoopHdr;

typedef struct
{
    CsrUint32 originalLength;
    CsrUint32 includedLength;
    CsrUint32 packetFlags;
    CsrUint32 cumulativeDrops;
    CsrUint32 timestampHigh;
    CsrUint32 timestampLow;
} btsnoopRecHdr;

/****************************************************************************
 * Helper methods
 ***************************************************************************/
static void csrLogBtsnoopGetUtcTime(CsrUint32 *high, CsrUint32 *low)
{
#ifdef CSR_HAVE_64_BIT_INTEGERS
    CsrUint64 msecs = 0;
    /* Make it relative to midnight, 1st Jan 2000 */
    msecs |= ((CsrUint64) 0x00dcddb3) << 32;
    msecs |= 0x0f2f8000;

    *low = CsrTimeGet(high);

    msecs += ((CsrUint64) * high) << 32 | *low;

    *high = (CsrUint32) (msecs >> 32);
    *low = (CsrUint32) (msecs);
#else
    *high = 0;
    *low = 0;
#endif
}

static CsrUint32 csrLogBtsnoopConvertFlags(CsrUint8 channel, CsrBool received)
{
    CsrUint32 flags = 0x00;

    if (channel == SYNERGY_BTI_TRANS_TYPE_HCI)
    {
        flags |= 0x02;
    }

    if (received)
    {
        flags |= 0x01;
    }

    return flags;
}

/****************************************************************************
 * Write a BTSNOOP record header
 ***************************************************************************/
static CsrUint32 csrLogBtsnoopSizeofRecHdr(CsrSize payload_length)
{
    btsnoopRecHdr *msg = NULL;
    CSR_UNUSED(msg);
    return (CsrUint32) (
               sizeof(msg->originalLength) +
               sizeof(msg->includedLength) +
               sizeof(msg->packetFlags) +
               sizeof(msg->cumulativeDrops) +
               sizeof(msg->timestampHigh) +
               sizeof(msg->timestampLow) +
               payload_length);
}

static void csrLogBtsnoopSerializeRecHdr(CsrUint8 *buf, CsrUint32 *offset, CsrUint32 max_len,
    CsrUint32 type, CsrUint32 orig_len,
    const void *payload)
{
    CsrUint32 time_high;
    CsrUint32 time_low;
    CsrUint32 incl_len;
    CsrUint32 cum_drops;

    incl_len = orig_len;
    cum_drops = 0;

    csrLogBtsnoopGetUtcTime(&time_high, &time_low);

    MEMCPY_U32_ITER_BE(buf, offset, max_len, orig_len);
    MEMCPY_U32_ITER_BE(buf, offset, max_len, incl_len);
    MEMCPY_U32_ITER_BE(buf, offset, max_len, type);
    MEMCPY_U32_ITER_BE(buf, offset, max_len, cum_drops);
    MEMCPY_U32_ITER_BE(buf, offset, max_len, time_high);
    MEMCPY_U32_ITER_BE(buf, offset, max_len, time_low);
    MEMCPY_PTR_NO_LEN_ITER(buf, offset, max_len, orig_len, payload);
}

/****************************************************************************
 * Write a BTSNOOP global header
 ***************************************************************************/
static void csrLogBtsnoopSerializeHdr(CsrUint8 *buf, CsrUint32 *offset, CsrUint32 max_len)
{
    CsrUint32 ver_no = 1;        /* version number */
    CsrUint32 dlt = 1001;        /* data link type */

    /*lint -save -e545 */
    MEMCPY_ITER(buf, offset, max_len, btsnoop_magic);
    /*lint -restore */
    MEMCPY_U32_ITER_BE(buf, offset, max_len, ver_no);
    MEMCPY_U32_ITER_BE(buf, offset, max_len, dlt);
}

static CsrUint32 csrLogBtsnoopSizeofHdr(void)
{
    btsnoopHdr *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->magic) +
           sizeof(msg->ver_no) +
           sizeof(msg->dlt);
}

/****************************************************************************
 * Log a BlueCore transport channel message
 ***************************************************************************/
static void csrLogBtsnoopBlueCoreTransport(void *ltHdl,
    CsrUint8 channel,
    CsrBool received,
    CsrSize payload_length,
    const void *payload)
{
    CsrUint32 flags = csrLogBtsnoopConvertFlags(channel, received);

    CsrUint8 *buf;
    CsrUint32 offset = 0;
    CsrUint32 length = csrLogBtsnoopSizeofRecHdr(payload_length);

    if ((channel != SYNERGY_BTI_TRANS_TYPE_HCI) &&
        (channel != SYNERGY_BTI_TRANS_TYPE_ACL) &&
        (channel != SYNERGY_BTI_TRANS_TYPE_SCO))
    {
        return;
    }

    CSR_LOG_BTSNOOP_TRANSPORT_ALLOC(buf, ltHdl, length);

    csrLogBtsnoopSerializeRecHdr(buf, &offset, length, flags, (CsrUint32) payload_length, payload);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log Btsnoop header
 ***************************************************************************/
static void csrLogBtsnoopHdr(void *ltHdl)
{
    CsrUint8 *buf;
    CsrUint32 offset = 0;
    CsrUint32 length = csrLogBtsnoopSizeofHdr();

    CSR_LOG_BTSNOOP_TRANSPORT_ALLOC(buf, ltHdl, length);

    csrLogBtsnoopSerializeHdr(buf, &offset, length);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
}

/****************************************************************************
 * External interface
 ***************************************************************************/
CsrLog *CsrLogBtsnoopCreate(void *ltHdl)
{
    CsrLog *l;

    l = CsrPmemZalloc(sizeof(*l));
    l->lbci = csrLogBtsnoopBlueCoreTransport;
    l->ltHdl = ltHdl;

    csrLogBtsnoopHdr(ltHdl);

    return l;
}

void CsrLogBtsnoopDestroy(CsrLog *l)
{
    CsrPmemFree(l);
}

#else
void CsrLogBtsnoopSilenceCompiler(void);
#endif

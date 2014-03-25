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
#include "csr_log_fts.h"
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

typedef struct
{
    CsrUint32 flags;
} ftsRecHdr;

/****************************************************************************
 * Helper methods
 ***************************************************************************/
static CsrUint32 csrLogFtsConvertFlags(CsrUint8 channel, CsrBool received)
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
 * Write a FTS header
 ***************************************************************************/
CsrUint32 CsrLogFtsSizeofRecHdr(CsrUint32 payload_length)
{
    ftsRecHdr *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->flags) +
           payload_length;
}

static void CsrLogFtsSerializeRecHdr(CsrUint8 *buf, CsrUint32 *offset, CsrUint32 max_len,
    CsrUint32 type, CsrUint32 orig_len,
    const void *payload)
{
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_PTR_NO_LEN_ITER(buf, offset, max_len, orig_len, payload);
}

/****************************************************************************
 * Log a BlueCore transport channel message
 ***************************************************************************/
static void csrLogFtsBlueCoreTransport(void *ltHdl,
    CsrUint8 channel,
    CsrBool received,
    CsrSize payload_length,
    const void *payload)
{
    CsrUint32 flags;
    CsrUint8 *buf;
    CsrUint32 offset = 0;
    CsrUint32 length;

    if ((channel != SYNERGY_BTI_TRANS_TYPE_HCI) &&
        (channel != SYNERGY_BTI_TRANS_TYPE_ACL) &&
        (channel != SYNERGY_BTI_TRANS_TYPE_SCO))
    {
        return;
    }

    flags = csrLogFtsConvertFlags(channel, received);
    length = CsrLogFtsSizeofRecHdr((CsrUint32) payload_length);

    buf = CsrLogTransportAlloc(ltHdl, length);

    CsrLogFtsSerializeRecHdr(buf, &offset, length, flags, (CsrUint32) payload_length, payload);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * External interface
 ***************************************************************************/
CsrLog *CsrLogFtsCreate(void *ltHdl)
{
    CsrLog *l;

    l = CsrPmemZalloc(sizeof(*l));

    l->lbci = csrLogFtsBlueCoreTransport;
    l->ltHdl = ltHdl;

    return l;
}

void CsrLogFtsDestroy(CsrLog *l)
{
    CsrPmemFree(l);
}

#else
void CsrLogFtsSilenceCompiler(void);
#endif

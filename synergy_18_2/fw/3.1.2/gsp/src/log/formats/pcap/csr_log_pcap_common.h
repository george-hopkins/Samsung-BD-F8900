#ifndef CSR_LOG_PCAP_COMMON_H__
#define CSR_LOG_PCAP_COMMON_H__
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
#include "csr_panic.h"
#include "csr_log_gsp.h"
#include "csr_log_private.h"
#include "csr_log_pcap.h"
#include "csr_usr_config.h"
#include "csr_msgconv.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MEMCPY_ITER(buf, offset, max_len, val)          \
    do {                                                    \
        CsrMemCpy((buf) + *(offset), &(val), sizeof(val));  \
        *(offset) += sizeof(val);                           \
        CSR_LOG_ASSERT(*(offset) <= max_len);               \
    } while (0)

#define MEMCPY_STR_ITER(buf, offset, max_len, str)      \
    do {                                                    \
        if (!str)                                            \
        {                                                   \
            CsrStrCpy((CsrCharString *) ((buf) + *(offset)), "");    \
            *(offset) += 1;                                 \
        }                                                   \
        else                                                \
        {                                                   \
            CsrStrCpy((CsrCharString *) ((buf) + *(offset)), (CsrCharString *) str);  \
            *(offset) += CsrLogPcapStrLen(str) + 1;         \
        }                                                   \
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

#define MEMCPY_PTR_ITER(buf, offset, max_len, ptr_length, ptr)      \
    do {                                                                \
        CsrMemCpy((buf) + *(offset), &(ptr_length), sizeof(ptr_length)); \
        *(offset) += sizeof(ptr_length);                                \
        CSR_LOG_ASSERT(*(offset) <= max_len);                           \
                                                                    \
        MEMCPY_PTR_NO_LEN_ITER(buf, offset, max_len, ptr_length, ptr);  \
    } while (0)

#define CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, handle, length) \
    buf = CsrLogTransportAlloc(handle, length);  \
    if (!buf)                                   \
    {                                           \
        return;                                 \
    }

/* Max PCAP payload length */
#define PCAP_MAX_REC_SIZE    ((CsrUint16) 0xFFFE)

void CsrLogPcapSerializePrim(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrMsgConvMsgEntry *msgEntry,
    CsrBool onlyPrimType,
    CsrBool limitPrim,
    const void *msg,
    CsrSize *actualOffset);

CsrSize CsrLogPcapSizeofPrim(CsrMsgConvMsgEntry *msgEntry,
    CsrBool onlyPrimType,
    const void *msg);

/****************************************************************************
 * Helper methods
 ***************************************************************************/
CsrSize CsrLogPcapStrLen(const CsrCharString *str);

/****************************************************************************
 * Write a PCAP global header
 ***************************************************************************/
void CsrLogPcapSerializeHdr(CsrUint8 *buf, CsrSize *offset, CsrSize max_len);

CsrUint32 CsrLogPcapSizeofHdr(void);

/****************************************************************************
 * Write a PCAP record header
 ***************************************************************************/
CsrSize CsrLogPcapSizeofRecHdr(void);

void CsrLogPcapSerializeRecHdr(CsrUint8 *buf, CsrSize *offset, CsrUint32 max_len,
    CsrUint32 incl_len, CsrUint32 orig_len, CsrUint32 received);

void CsrLogPcapSerializeRecHdrNoDirection(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrSize incl_len, CsrSize orig_len);

/****************************************************************************
 * Synergy format
 ***************************************************************************/
void CsrLogPcapSerializeSynergyVerHeader(CsrUint8 *buf, CsrSize *offset, CsrSize max_len);

CsrSize CsrLogPcapSizeofSynergyVerHeader(void);

#ifdef __cplusplus
}
#endif

#endif /* CSR_LOG_PCAP_COMMON_H__ */

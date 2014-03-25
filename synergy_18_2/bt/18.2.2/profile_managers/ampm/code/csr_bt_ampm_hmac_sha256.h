#ifndef CSR_BT_AMPM_HMAC_SHA256_H
#define CSR_BT_AMPM_HMAC_SHA256_H

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_AMPM_SHA256_DIGEST_LENGTH 32

typedef struct
{
    CsrUint32 H[8];
    CsrUint8 plain[64];
    CsrUint32 remainder;
    CsrUint32 length;
} CsrBtAmpmSha256ctx;

typedef struct
{
    CsrBtAmpmSha256ctx ctx;
    CsrUint8 opad[64];
} CsrBtAmpmHmacSha256ctx;

void CsrBtAmpmSha256Init(CsrBtAmpmSha256ctx *pCtx);
void CsrBtAmpmSha256Update(CsrBtAmpmSha256ctx *pCtx,
                           const void *pPlain,
                           CsrUint32 length);
void CsrBtAmpmSha256Final(CsrUint8 *pDigest,
                          CsrBtAmpmSha256ctx *pCtx);
void CsrBtAmpmHmacSha256Init(CsrBtAmpmHmacSha256ctx *pCtx,
                             CsrUint8 *pKey,
                             CsrUint32 length);
void CsrBtAmpmHmacSha256Update(CsrBtAmpmHmacSha256ctx *pCtx,
                               const void *pPlain,
                               CsrUint32 length);
void CsrBtAmpmHmacSha256Final(CsrUint8 *pDigest,
                              CsrBtAmpmHmacSha256ctx *pCtx);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_AMPM_HMAC_SHA256_H */

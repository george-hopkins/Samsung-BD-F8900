/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

static const CsrUint32 K[64] =
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#define CSR_BT_AMPM_ROTR( x, n ) (( (x) >> (n) ) | ( (x) << (32 - (n)) ))
#define CSR_BT_AMPM_SHR( x, n ) ( (x) >> (n) )
#define CSR_BT_AMPM_CH( x, y, z ) (( (x) & (y) ) ^ ( ~(x) & (z) ))
#define CSR_BT_AMPM_MAJ( x, y, z) (( (x) & (y) ) ^ ( (x) & (z) ) ^ ( (y) & (z) ))

static void csrBtAmpmSha256Process(CsrBtAmpmSha256ctx *pCtx,
                                   CsrUint32 M[16])
{
    CsrUint32 W[64];
    int t;
    CsrUint32 a, b, c, d, e, f, g, h;
    CsrUint32 T1, T2;

    for (t = 0; t < 16; t++)
    {
        W[t] = M[t];
    }

    for (t = 16; t < 64; t++)
    {
        W[t] =  (CSR_BT_AMPM_ROTR(W[t - 2], 17) ^
                 CSR_BT_AMPM_ROTR(W[t - 2], 19) ^
                 CSR_BT_AMPM_SHR(W[t - 2], 10));
        W[t] += W[t - 7];
        W[t] += (CSR_BT_AMPM_ROTR(W[t - 15], 7) ^
                 CSR_BT_AMPM_ROTR(W[t - 15], 18) ^
                 CSR_BT_AMPM_SHR(W[t - 15], 3));
        W[t] += W[t - 16];
    }

    a = pCtx->H[0];
    b = pCtx->H[1];
    c = pCtx->H[2];
    d = pCtx->H[3];
    e = pCtx->H[4];
    f = pCtx->H[5];
    g = pCtx->H[6];
    h = pCtx->H[7];

    for (t = 0; t < 64; t++)
    {
        T1 =  h;
        T1 += (CSR_BT_AMPM_ROTR(e, 6) ^
               CSR_BT_AMPM_ROTR(e, 11) ^
               CSR_BT_AMPM_ROTR(e, 25));
        T1 += CSR_BT_AMPM_CH(e, f, g);
        T1 += K[t];
        T1 += W[t];
        T2 =  (CSR_BT_AMPM_ROTR(a, 2) ^
               CSR_BT_AMPM_ROTR(a, 13) ^
               CSR_BT_AMPM_ROTR(a, 22));
        T2 += CSR_BT_AMPM_MAJ(a, b, c);

        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    pCtx->H[0] += a;
    pCtx->H[1] += b;
    pCtx->H[2] += c;
    pCtx->H[3] += d;
    pCtx->H[4] += e;
    pCtx->H[5] += f;
    pCtx->H[6] += g;
    pCtx->H[7] += h;
}

void CsrBtAmpmSha256Init(CsrBtAmpmSha256ctx * pCtx)
{
    pCtx->H[0] = 0x6a09e667;
    pCtx->H[1] = 0xbb67ae85;
    pCtx->H[2] = 0x3c6ef372;
    pCtx->H[3] = 0xa54ff53a;
    pCtx->H[4] = 0x510e527f;
    pCtx->H[5] = 0x9b05688c;
    pCtx->H[6] = 0x1f83d9ab;
    pCtx->H[7] = 0x5be0cd19;

    pCtx->remainder = 0;
    pCtx->length = 0;
}

void CsrBtAmpmSha256Update(CsrBtAmpmSha256ctx *pCtx,
                           const void *pPlain,
                           CsrUint32 length)
{
    CsrUint32 M[16];
    int i;
    int offset = 0;

    while (pCtx->remainder + length >= 64)
    {
        CsrMemCpy((void *) &(pCtx->plain[pCtx->remainder]),
                  (const void *) ((CsrUint8 *) pPlain + offset),
                  (64 - pCtx->remainder));
        offset += 64 - pCtx->remainder;
        length -= 64 - pCtx->remainder;
        for (i = 0; i < 16; i++)
        {
            M[i] = pCtx->plain[i * 4] << 24;
            M[i] |= pCtx->plain[(i * 4) + 1] << 16;
            M[i] |= pCtx->plain[(i * 4) + 2] << 8;
            M[i] |= pCtx->plain[(i * 4) + 3];
        }
        csrBtAmpmSha256Process(pCtx, M);
        pCtx->length += 64 - pCtx->remainder;
        pCtx->remainder = 0;
    }

    if (pCtx->remainder + length > 0)
    {
        /* Less that one full block - just add to remainder buffer */
        CsrMemCpy((void *) &(pCtx->plain[pCtx->remainder]),
                  (const void *) ((CsrUint8 *) pPlain + offset), length);
        pCtx->remainder += length;
        pCtx->length += length;
    }
}

void CsrBtAmpmSha256Final(CsrUint8 *pDigest,
                          CsrBtAmpmSha256ctx *pCtx)
{
    int i;
    CsrUint32 M[16];

    pCtx->plain[pCtx->remainder++] = 0x80;

    if (pCtx->remainder > 56)
    {
        for (i = pCtx->remainder; i < 64; i++)
        {
            pCtx->plain[i] = 0x00;
        }

        for (i = 0; i < 16; i++)
        {
            M[i] =  pCtx->plain[i * 4] << 24;
            M[i] |= pCtx->plain[(i * 4) + 1] << 16;
            M[i] |= pCtx->plain[(i * 4) + 2] << 8;
            M[i] |= pCtx->plain[(i * 4) + 3];
        }
        csrBtAmpmSha256Process(pCtx, M);
        pCtx->remainder = 0;
    }

    for (i = pCtx->remainder; i < 56; i++)
    {
        pCtx->plain[i] = 0x00;
    }

    for (i = 0; i < 14; i++)
    {
        M[i] = pCtx->plain[i * 4] << 24;
        M[i] |= pCtx->plain[(i * 4) + 1] << 16;
        M[i] |= pCtx->plain[(i * 4) + 2] << 8;
        M[i] |= pCtx->plain[(i * 4) + 3];
    }
    M[14] = 0;
    M[15] = pCtx->length * 8;

    csrBtAmpmSha256Process(pCtx, M);

    for (i = 0; i < 8; i++)
    {
        pDigest[(i * 4) + 0] = (CsrUint8)(pCtx->H[i] >> 24 & 0xff);
        pDigest[(i * 4) + 1] = (CsrUint8)(pCtx->H[i] >> 16 & 0xff);
        pDigest[(i * 4) + 2] = (CsrUint8)(pCtx->H[i] >> 8 & 0xff);
        pDigest[(i * 4) + 3] = (CsrUint8)(pCtx->H[i] & 0xff);
    }
}

void CsrBtAmpmHmacSha256Init(CsrBtAmpmHmacSha256ctx *pCtx,
                             CsrUint8 *pKey,
                             CsrUint32 length)
{
    CsrUint8 ipad[64];
    CsrUint32 i;

    for (i = 0; i < 64; i++)
    {
        ipad[i] = 0x36;
        pCtx->opad[i] = 0x5c;
    }

    if (length > 64)
    {
        CsrBtAmpmSha256Init(&pCtx->ctx);
        CsrBtAmpmSha256Update(&pCtx->ctx, pKey, length);
        CsrBtAmpmSha256Final(pKey, &pCtx->ctx);
        length = CSR_BT_AMPM_SHA256_DIGEST_LENGTH;
    }

    for (i = 0; i < length; i++)
    {
        ipad[i] ^= pKey[i];
        pCtx->opad[i] ^= pKey[i];
    }

    CsrBtAmpmSha256Init(&pCtx->ctx);
    CsrBtAmpmSha256Update(&pCtx->ctx, ipad, 64);
}

void CsrBtAmpmHmacSha256Update(CsrBtAmpmHmacSha256ctx *pCtx,
                               const void *pPlain,
                               CsrUint32 length)
{
    CsrBtAmpmSha256Update(&pCtx->ctx, pPlain, length);
}

void CsrBtAmpmHmacSha256Final(CsrUint8 *pDigest,
                              CsrBtAmpmHmacSha256ctx *pCtx)
{
    CsrBtAmpmSha256Final(pDigest, &pCtx->ctx);
    CsrBtAmpmSha256Init(&pCtx->ctx);
    CsrBtAmpmSha256Update(&pCtx->ctx, pCtx->opad, 64);
    CsrBtAmpmSha256Update(&pCtx->ctx, pDigest,
                          CSR_BT_AMPM_SHA256_DIGEST_LENGTH);
    CsrBtAmpmSha256Final(pDigest, &pCtx->ctx);
}

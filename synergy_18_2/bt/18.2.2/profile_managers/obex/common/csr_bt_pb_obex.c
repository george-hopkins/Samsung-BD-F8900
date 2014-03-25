/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_obex_extraction_lib.h"
#include "csr_bt_obex.h"
#include "csr_bt_obex_auth.h"
#include "csr_bt_pb_obex.h"
#include "csr_bt_util.h"

/* == LOCAL DEFINES ========================================================= */
/* == LOCAL CONSTS ========================================================== */
/* == LOCAL PROTOTYPES ======================================================= */
/* == GLOBAL FUNCTIONS ====================================================== */
CsrUint16 PbRetAppParaHeaderLen(PbAppParFlag flag)
{
    CsrUint16 nLen = CSR_BT_OBEX_APP_PAR_HEADER_SIZE;

    nLen += (CsrUint16)(flag & CSR_BT_PB_OBEX_FILTER_FLAG)? CSR_BT_OBEX_APP_PAR_FILTER_SIZE : 0;
    nLen += (CsrUint16)(flag & CSR_BT_PB_OBEX_FORMAT_FLAG)? CSR_BT_OBEX_APP_PAR_FORMAT_SIZE : 0;
    nLen += (CsrUint16)(flag & CSR_BT_PB_OBEX_MAX_LIST_CNT_FLAG) ? CSR_BT_OBEX_APP_PAR_MAXLISTCNT_SIZE : 0;
    nLen += (CsrUint16)(flag & CSR_BT_PB_OBEX_ORDER_FLAG) ? CSR_BT_OBEX_APP_PAR_ORDER_SIZE : 0;
    nLen += (CsrUint16)(flag & CSR_BT_PB_OBEX_LISTSTARTOFFSET_FLAG) ? CSR_BT_OBEX_APP_PAR_LISTSTARTOFFSET_SIZE : 0;
    nLen += (CsrUint16)(flag & CSR_BT_PB_OBEX_PBSIZE_FLAG) ? CSR_BT_OBEX_APP_PAR_PB_SIZE : 0;
    nLen += (CsrUint16)(flag & CSR_BT_PB_OBEX_NEWMISSEDCALL_FLAG) ? CSR_BT_OBEX_APP_PAR_NEWMISSEDCALL_SIZE : 0;

    return nLen;
}

void PbBuildAppHeader(CsrUint8 **ppBuf, CsrUint8 **ppAppTotalLen)
{
    CsrUint8 *pBuf = *ppBuf;

    *pBuf = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER; /* write App par Tag */
    pBuf++;
    *pBuf = 0x00;
    pBuf++;
    *ppAppTotalLen = pBuf;    /* store offset to length */
    **ppAppTotalLen = CSR_BT_OBEX_APP_PAR_HEADER_SIZE;   /* write current length */
    pBuf++;
    *ppBuf = pBuf;
}

CsrUint32 CsrBtObexWcsLen(const CsrUint8 *string)
{
    CsrUintFast32 nLen = 0;

    if (NULL != string)
    {
        for ( ; string[nLen] || string[nLen+1] ; nLen += 2 )
        {
        }
    }
    return nLen;
}

CsrUint8 *CsrBtObexWcsCpy(CsrUint8 *dest, const CsrUint8 *src)
{
    CsrUint8 *tmp = dest;

    for ( ; src[0] || src[1]; *tmp++ = *src++, *tmp++ = *src++)
    {
    }
    tmp[0] = 0;
    tmp[1] = 0;
    return dest;
}

CsrUint8 *CsrBtObexWcsCat(CsrUint8 *dest, const CsrUint8 *src)
{
    CsrUint8 *tmp = dest + CsrBtObexWcsLen(dest);

    CsrBtObexWcsCpy(tmp, src);
    return dest;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtObexRevIWcsNMatch
 *
 *  DESCRIPTION
 *      Compare string with pattern from right to left and
 *      return the beginning of the first appearance of pattern.
 *      It will only return success if it find a complete match.
 *
 *  INPUT:
 *      src: usc-2 string to to perform the search in
 *      patter: usc-2 char you which to find
 *      cnt: the position you want to start searching from, specified from left to right
 *
 * EXAMPLE:
 *  CsrUint8 src[] = L"The quick brown dog jumps over the lazy fox";
 *  CsrUint8 pattern = L"jumps";
 *  CsrUint32 cnt = CsrBtObexWcsLen(src);
 *
 *  prntf("s is in position: %i\n", RevWcsNCmp(src, pattern, cnt));
 *
 *  result:
 *      s is in position: 24
 *
 *      The quick brown dog jumps over the lazy fox
 *      ------------------------^
 *  RETURNS
 *      CsrInt32position of pattern in src, negative value if pattern is not found
 *
 *----------------------------------------------------------------------------*/
CsrInt32 CsrBtObexRevIWcsNMatch(const CsrUint8 *src, const CsrUint8 *pattern, CsrInt32 cnt)
{
    CsrInt32 iCnt = (CsrInt32)cnt, iPatternLen = CsrBtObexWcsLen(pattern);

    if (NULL == src || NULL == pattern || iPatternLen <= 0)
    {
        return -1;
    }
    iPatternLen -= 2;
    for ( ; iCnt >= 0; iCnt -= 2 )
    {
        CsrInt32 i = iPatternLen, j = iCnt;

        for ( ; j >= 0 && i >= 0; j -= 2, i -= 2 )
        {
            if (src[j] != pattern[i] || src[j+1] != pattern[i+1])
            {
                i = -4;
                break; /* no match */
            }
        }
        if ( -2 == i)
        {
            break; /* success we found a match */
        }
    }
    return iCnt;
}

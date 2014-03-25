#ifndef CSR_UI_UTIL_H__
#define CSR_UI_UTIL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrUint32 CsrUiUtilStringLength(CsrUint16 *string);
CsrUint16 CsrUiUtilStringLines(CsrUint16 *string);
CsrUint16 *CsrUiUtilStringCopy(CsrUint16 *string);
CsrUint16 CsrUiUtilWordLength(CsrUint16 *string);
CsrUint16 *CsrUiUtilWordWrap(CsrUint16 columns, CsrUint16 rows, CsrUint16 *string);
CsrUint16 *CsrUiUtilStringCat(CsrUint16 *dest, const CsrUint16 *src);
CsrUint16 *CsrUiUtilStringCatCap(CsrUint16 *dest, const CsrUint16 *src, CsrUint32 cap);

#ifdef __cplusplus
}
#endif

#endif /* CSR_UI_UTIL_H__ */

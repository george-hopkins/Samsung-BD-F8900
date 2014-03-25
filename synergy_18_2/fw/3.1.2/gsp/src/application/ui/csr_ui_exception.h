#ifndef CSR_UI_EXCEPTION_H__
#define CSR_UI_EXCEPTION_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Only use exceptions if debug is on */
#ifdef _DEBUG
#define CSR_UIEXCEPTION(text) CsrUiException(text)
#else
#define CSR_UIEXCEPTION(text)
#endif

void *CsrUiException(const CsrCharString *exception);

#ifdef __cplusplus
}
#endif

#endif

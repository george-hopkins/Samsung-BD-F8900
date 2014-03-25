#ifndef CSR_TYPE_A_SDIO_H__
#define CSR_TYPE_A_SDIO_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sdio.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrResult CsrTypeAFunctionDriverRegister(CsrSdioFunctionDriver *driver);
void CsrTypeAFunctionDriverUnregister(CsrSdioFunctionDriver *driver);

void CsrTypeARead8(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 *data,
    CsrSdioAsyncCallback cb);
void CsrTypeAF0Read8(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 *data,
    CsrSdioAsyncCallback cb);
void CsrTypeARead(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 *data,
    CsrUint32 bytes, CsrSdioAsyncCallback cb);

void CsrTypeAWrite8(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 data,
    CsrSdioAsyncCallback cb);
void CsrTypeAF0Write8(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 data,
    CsrSdioAsyncCallback cb);
void CsrTypeAWrite(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 *data,
    CsrUint32 bytes, CsrSdioAsyncCallback cb);

#ifdef __cplusplus
}
#endif

#endif

/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sdio.h"

#include "csr_type_a_sdio.h"

CsrResult CsrTypeAFunctionDriverRegister(CsrSdioFunctionDriver *driver)
{
    return CsrSdioFunctionDriverRegister(driver);
}

void CsrTypeAFunctionDriverUnregister(CsrSdioFunctionDriver *driver)
{
    CsrSdioFunctionDriverUnregister(driver);
}

void CsrTypeARead8(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 *data,
    CsrSdioAsyncCallback cb)
{
    CsrSdioRead8Async(f, addr, data, cb);
}

void CsrTypeAF0Read8(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 *data,
    CsrSdioAsyncCallback cb)
{
    CsrSdioF0Read8Async(f, addr, data, cb);
}

void CsrTypeARead(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 *data,
    CsrUint32 bytes, CsrSdioAsyncCallback cb)
{
    CsrSdioReadAsync(f, addr, data, bytes, cb);
}

void CsrTypeAWrite8(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 data,
    CsrSdioAsyncCallback cb)
{
    CsrSdioWrite8Async(f, addr, data, cb);
}

void CsrTypeAF0Write8(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 data,
    CsrSdioAsyncCallback cb)
{
    CsrSdioF0Write8Async(f, addr, data, cb);
}

void CsrTypeAWrite(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 *data,
    CsrUint32 bytes, CsrSdioAsyncCallback cb)
{
    CsrSdioWriteAsync(f, addr, data, bytes, cb);
}

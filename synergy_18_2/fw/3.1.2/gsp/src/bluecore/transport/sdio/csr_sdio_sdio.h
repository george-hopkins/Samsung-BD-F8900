#ifndef CSR_SDIO_SDIO_H__
#define CSR_SDIO_SDIO_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_result.h"
#include "csr_sdio_master.h"
#include "csr_sdio_common.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrResult CsrSdioSdioDeviceInitialise(CsrSdioDevice *sdioDevice, CsrUint16 *vendorId, CsrUint16 *deviceId, CsrUint8 standardInterface[8]);

CsrResult CsrSdioSdioRead16(CsrSdioMasterDevice *sdioMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint16 *data);
CsrResult CsrSdioSdioWrite16(CsrSdioMasterDevice *sdioMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint16 data);
CsrResult CsrSdioSdioRead(CsrSdioMasterDevice *sdioMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint32 length);
CsrResult CsrSdioSdioWrite(CsrSdioMasterDevice *sdioMasterDevice, CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint32 length);

void CsrSdioSdioReadAsync(CsrSdioMasterDevice *sdioMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint32 length, CsrSdioMasterCallback operationCallback);
void CsrSdioSdioWriteAsync(CsrSdioMasterDevice *sdioMasterDevice, CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint32 length, CsrSdioMasterCallback operationCallback);

#ifdef __cplusplus
}
#endif

#endif

#ifndef CSR_SDIO_CSPI_H__
#define CSR_SDIO_CSPI_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_result.h"
#include "csr_spi_master.h"
#include "csr_sdio_common.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrResult CsrSdioCspiDeviceInitialise(CsrSdioDevice *sdioDevice, CsrUint16 *vendorId, CsrUint16 *deviceId, CsrUint8 standardInterface[8]);

void CsrSdioCspiRead8(CsrSpiMasterDevice *spiMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint8 *data);
void CsrSdioCspiWrite8(CsrSpiMasterDevice *spiMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint8 data);
void CsrSdioCspiRead16(CsrSpiMasterDevice *spiMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint16 *data);
void CsrSdioCspiWrite16(CsrSpiMasterDevice *spiMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint16 data);
void CsrSdioCspiRead(CsrSpiMasterDevice *spiMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint32 length);
void CsrSdioCspiWrite(CsrSpiMasterDevice *spiMasterDevice, CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint32 length);

void CsrSdioCspiRead8Async(CsrSpiMasterDevice *spiMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrSpiMasterCallback operationCallback);
void CsrSdioCspiWrite8Async(CsrSpiMasterDevice *spiMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint8 data, CsrSpiMasterCallback operationCallback);
void CsrSdioCspiRead16Async(CsrSpiMasterDevice *spiMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint16 *data, CsrSpiMasterCallback operationCallback);
void CsrSdioCspiWrite16Async(CsrSpiMasterDevice *spiMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint16 data, CsrSpiMasterCallback operationCallback);
void CsrSdioCspiReadAsync(CsrSpiMasterDevice *spiMasterDevice, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint32 length, CsrSpiMasterCallback operationCallback);
void CsrSdioCspiWriteAsync(CsrSpiMasterDevice *spiMasterDevice, CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint32 length, CsrSpiMasterCallback operationCallback);

#ifdef __cplusplus
}
#endif

#endif

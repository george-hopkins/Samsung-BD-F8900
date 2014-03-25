#ifndef CSR_SDIO_PROBE_TEST_H__
#define CSR_SDIO_PROBE_TEST_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ui_prim.h"
#include "csr_sdio.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrResult CsrSdioProbeBlockSizeSet(CsrSdioFunction *function);

CsrSdioInterruptDsrCallback CsrSdioProbeInterruptCallback(CsrSdioFunction *function);

void CsrSdioProbeStartStopTest(CsrSdioProbeInstanceData *instanceData, CsrUint16 key, CsrSdioProbeTestType testType);

#ifdef __cplusplus
}
#endif

#endif

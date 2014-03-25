#ifndef CSR_SDIO_PROBE_TASK_H__
#define CSR_SDIO_PROBE_TASK_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"

#ifdef __cplusplus
extern "C" {
#endif

extern CsrSchedQid CSR_SDIO_PROBE_IFACEQUEUE;
void CsrSdioProbeInit(void **gash);
void CsrSdioProbeDeinit(void **gash);
void CsrSdioProbeHandler(void **gash);

#define CSR_SDIO_PROBE_INIT      CsrSdioProbeInit
#define CSR_SDIO_PROBE_HANDLER   CsrSdioProbeHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_SDIO_PROBE_DEINIT    CsrSdioProbeDeinit
#else
#define CSR_SDIO_PROBE_DEINIT    NULL
#endif

#ifdef __cplusplus
}
#endif

#endif

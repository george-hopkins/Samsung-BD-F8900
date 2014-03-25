#ifndef CSR_SDIO_PROBE_INTERNAL_LIB_H__
#define CSR_SDIO_PROBE_INTERNAL_LIB_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_msg_transport.h"
#include "csr_sdio.h"
#include "csr_sdio_probe_task.h"
#include "csr_sdio_probe_internal_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CsrSdioProbeInternalInsertedIndSend(function) \
    { \
        CsrSdioProbeInternalInsertedInd *indication = CsrPmemAlloc(sizeof(CsrSdioProbeInternalInsertedInd)); \
        indication->type = CSR_SDIO_PROBE_INTERNAL_INSERTED_IND; \
        indication->function = function; \
        CsrMsgTransport(CSR_SDIO_PROBE_IFACEQUEUE, CSR_SDIO_PROBE_PRIM, indication); \
    }

#define CsrSdioProbeInternalRemovedIndSend(function) \
    { \
        CsrSdioProbeInternalRemovedInd *indication = CsrPmemAlloc(sizeof(CsrSdioProbeInternalRemovedInd)); \
        indication->type = CSR_SDIO_PROBE_INTERNAL_REMOVED_IND; \
        indication->function = function; \
        CsrMsgTransport(CSR_SDIO_PROBE_IFACEQUEUE, CSR_SDIO_PROBE_PRIM, indication); \
    }

#ifdef __cplusplus
}
#endif

#endif

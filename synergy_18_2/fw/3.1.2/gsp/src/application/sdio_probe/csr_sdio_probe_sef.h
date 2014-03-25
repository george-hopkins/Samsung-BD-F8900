#ifndef CSR_SDIO_PROBE_SEF_H__
#define CSR_SDIO_PROBE_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sdio_probe_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrSdioProbeInternalInsertedIndHandler(CsrSdioProbeInstanceData *instanceData);
void CsrSdioProbeInternalRemovedIndHandler(CsrSdioProbeInstanceData *instanceData);

void CsrSdioProbeAppRegisterCfmHandler(CsrSdioProbeInstanceData *instanceData);
void CsrSdioProbeAppTakeControlIndHandler(CsrSdioProbeInstanceData *instanceData);

void CsrSdioProbeUiUieCreateCfmHandler(CsrSdioProbeInstanceData *instanceData);
void CsrSdioProbeUiEventIndHandler(CsrSdioProbeInstanceData *instanceData);

/* UI related */
void CsrSdioProbeEventNotification(CsrSdioProbeInstanceData *instanceData, CsrBool backlog, CsrBool popup, const CsrCharString *fmt, ...);
void CsrSdioProbeTestMenuUpdate(CsrSdioProbeInstanceData *instanceData, CsrUint16 key);

#ifdef __cplusplus
}
#endif

#endif

#ifndef CSR_DSPM_SEF_H__
#define CSR_DSPM_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_dspm_handler.h"

#include "csr_bccmd_prim.h"
#include "csr_hq_prim.h"

#ifdef CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD
#include "csr_fp_prim.h"
#endif

#ifdef CSR_BLUECORE_ONOFF
#include "csr_tm_bluecore_prim.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* DSPM */
CsrBool CsrDspmRequestHandler(CsrDspmInstanceData *instanceData);

/* BCCMD */
void CsrDspmBccmdCfmHandler(CsrDspmInstanceData *instanceData, CsrBccmdCfm *confirm);

/* HQ */
void CsrDspmHqMsgIndHandler(CsrDspmInstanceData *instanceData, CsrHqMsgInd *indication);
void CsrDspmHqRegister(void);

#ifdef CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD
/* FP */
void CsrDspmFpCreateCfmHandler(CsrDspmInstanceData *instanceData, CsrFpCreateCfm *confirm);
void CsrDspmFpWriteCfmHandler(CsrDspmInstanceData *instanceData, CsrFpWriteCfm *confirm);
void CsrDspmFpDestroyCfmHandler(CsrDspmInstanceData *instanceData, CsrFpDestroyCfm *confirm);
#endif

#ifdef CSR_BLUECORE_ONOFF
/* TM */
void CsrDspmTmBluecoreActivateTransportIndHandler(CsrDspmInstanceData *instanceData, CsrTmBluecoreActivateTransportInd *indication);
void CsrDspmTmBluecoreDeactivateTransportIndHandler(CsrDspmInstanceData *instanceData, CsrTmBluecoreDeactivateTransportInd *indication);
#endif

#ifdef __cplusplus
}
#endif

#endif

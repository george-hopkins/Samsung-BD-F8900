#ifndef CSR_FP_SEF_H__
#define CSR_FP_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_sched.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_fp_handler.h"

/************************************************************************/
/* Functions Prototypes                                                 */
/************************************************************************/

void CsrFpCreateReqHandler(CsrFpInstType *inst);
void CsrFpWriteReqHandler(CsrFpInstType *inst);
void CsrFpReadResHandler(CsrFpInstType *inst);
void CsrFpClearReqHandler(CsrFpInstType *inst);
void CsrFpDestroyReqHandler(CsrFpInstType *inst);

void CsrFpHciAclDataIndHandler(CsrFpInstType *inst);
void CsrFpBccmdCfmHandler(CsrFpInstType *inst);

void CsrFpActivate(CsrFpInstType *inst);
void CsrFpContainerDestroy(CsrFpInstType *inst, CsrUint8 pipeId);
void CsrFpContainerDestroyAll(CsrFpInstType *inst);

/* Prototypes from csr_fp_free_down.c */
void CsrFpFreeDownstreamMessageContents(CsrUint16 eventClass, void *message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_FP_SEF_H__ */

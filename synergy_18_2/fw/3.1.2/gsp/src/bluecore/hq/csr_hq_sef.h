#ifndef CSR_HQ_SEF_H__
#define CSR_HQ_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_hq_handler.h"
#include "csr_hci_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*  State event functions for CSR_HQ Prim */
void CsrHqRegisterReqHandler(CsrHqInstanceData *hqInst, CsrHqRegisterReq *request);
void CsrHqDeregisterReqHandler(CsrHqInstanceData *hqInst, CsrHqDeregisterReq *request);
void CsrHqMsgResHandler(CsrHqInstanceData *hqInst, CsrHqMsgRes *response);

/*  State event functions for received CMDIF Prim */
void CsrHqHciVendorSpecificEventIndHandler(CsrHqInstanceData *hqInst, CsrHciVendorSpecificEventInd *indication);

#ifdef __cplusplus
}
#endif

#endif

#ifndef CSR_BT_CM_BCCMD_H__
#define CSR_BT_CM_BCCMD_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_cm_main.h"
#include "csr_bccmd_prim.h"
#include "csr_bccmd_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BCCMD_VARID_INQUIRY_PRIORITY        0x685C
#define BCCMD_VARID_MAP_SCO_PCM             0x481C
#define BCCMD_VARID_PSVALUE                 0x7003
#define BCCMD_VARID_GET_SCO_PARAMETERS      0x304d
#define PSKEY_LM_MAX_EVENT_FILTERS          0x00F4
#define PSKEY_ONCHIP_HCI_CLIENT             0x03CC

void CsrBtCmBccmdArrivalHandler(cmInstanceData_t *cmData);
void CsrBtCmBccmdGetScoParametersReqSend(CsrUint16 scoHandle, CsrUint16 seqNo);

#ifdef __cplusplus
}
#endif

#endif /* ndef _CM_BCCMD_H */


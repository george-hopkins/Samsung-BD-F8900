#ifndef CSR_BT_HF_AT_INTER_H__
#define CSR_BT_HF_AT_INTER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_hf_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtHfHandleMultipleAtCommand(HfMainInstanceData_t *instData, CsrUint32 theConnectionId);

#ifdef __cplusplus
}
#endif

#endif

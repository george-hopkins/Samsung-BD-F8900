#ifndef CSR_BT_CM_COMMON_AMP_H__
#define CSR_BT_CM_COMMON_AMP_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef CSR_AMP_ENABLE

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Common upstream senders */
void CsrBtCmAmpMoveChannelCfmSend(cmInstanceData_t *inst,
                                  CsrBtConnId btConnId,
                                  CsrBtAmpController localControl,
                                  CsrBtDeviceAddr addr,
                                  CsrBtResultCode resultCode,
                                  CsrBtSupplier resultSupplier);
void CsrBtCmAmpMoveChannelCmpIndSend(cmInstanceData_t *inst,
                                     CsrBtConnId btConnId,
                                     CsrBtAmpController localControl,
                                     CsrBtDeviceAddr addr,
                                     CsrBtResultCode resultCode,
                                     CsrBtSupplier resultSupplier);
void CsrBtCmAmpMoveChannelIndSend(cmInstanceData_t *inst,
                                  CsrBtConnId btConnId,
                                  CsrBtAmpController localControl,
                                  CsrBtDeviceAddr addr);

/* Common downstream handlers */
void CsrBtCmAmpMoveChannelReqHandler(cmInstanceData_t *cmData);
void CsrBtCmAmpMoveChannelResHandler(cmInstanceData_t *cmData);

/* Utility functions */
void CsrBtCmAmpForceModeChange(cmInstanceData_t *cmData,
                               CsrBtDeviceAddr *addr);

#ifdef __cplusplus
}
#endif
#endif /* CSR_AMP_ENABLE */
#endif /* CSR_BT_CM_COMMON_AMP_H__ */

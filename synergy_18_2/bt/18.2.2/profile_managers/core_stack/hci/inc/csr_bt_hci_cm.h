#ifndef CSR_BT_HCI_CM_H__
#define CSR_BT_HCI_CM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_types.h"
#include "csr_sched.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_HCI_CM_INVALID_HCI_CMD_IF 0xFFFFFFFF

typedef void (*CsrBtHciCmEventStatusType)(CsrUint8 numHciCommandPackets, hci_return_t hciStatus);
typedef void (*CsrBtHciCmEventCompleteType)(void *cmdArg, CsrUint8 numHciCommandPackets);
typedef void (*CsrBtHciCmFreeType)(void *cmdArg);
typedef CsrBool (*CsrBtHciCmMatchCompareType)(void *cmdArg, const void *eventArg);

#ifdef CSR_CHIP_MANAGER_ENABLE
void CsrBtHciCmInit(CsrSchedQid appHandle);
void CsrBtHciCmDeinit(void);
void CsrBtHciCmResendStart(void);
CsrBool CsrBtHciCmCmdResendTypeAlwaysToggleHandler(void *cmdStream, CsrUint32 cmdId, CsrBool remove);
CsrBool CsrBtHciCmCmdResendTypeAlwaysHandler(void *cmdStream, CsrUint32 cmdId);
CsrBool CsrBtHciCmCmdResendTypePendingHandler(void *cmdStream, 
                                              void *cmdArg,
                                              CsrUint32 cmdId,
                                              CsrUint32 secondaryCmdId,
                                              CsrBtHciCmFreeType cmdArgFreeFunc,
                                              CsrBtHciCmMatchCompareType cmdMatchFunc);
CsrBool CsrBtHciCmCmdResendTypeNeverHandler(void *cmdArg,
                                            CsrUint32 cmdId, 
                                            CsrUint32 secondaryCmdId,
                                            CsrBtHciCmEventStatusType cmdStatusEventHandler,
                                            CsrBtHciCmEventCompleteType cmdCompleteEventHandler,
                                            CsrBtHciCmFreeType cmdArgFreeFunc,
                                            CsrBtHciCmMatchCompareType cmdMatchFunc);

CsrBool CsrBtHciCmEventHandler(CsrUint32 cmdId, 
                               hci_return_t hciStatus, 
                               CsrBool cmdComplete,
                               CsrUint8 *numHciCommandPackets,
                               const void *eventArg);

#ifdef CSR_BT_LE_ENABLE
CsrBool CsrBtHciCmEventUlpConnectionCompleteHandler(hci_return_t status,
                                                    hci_connection_handle_t handle,
                                                    const void *eventArg);
#endif
CsrBool CsrBtHciCmEventConnectionCompleteHandler(hci_return_t status,
                                                 hci_connection_handle_t handle,
                                                 CsrUint8 linkType,
                                                 const void *eventArg);
CsrBool CsrBtHciCmEventDisconnectionCompleteHandler(hci_return_t status,
                                                    hci_connection_handle_t handle);
CsrBool CsrBtHciCmSendRequest(void);
void CsrBtHciCmResetDetectedRequest(void);
void CsrBtHciCmRestartDetectedRequest(void);
#endif

#ifdef __cplusplus
}
#endif

#endif


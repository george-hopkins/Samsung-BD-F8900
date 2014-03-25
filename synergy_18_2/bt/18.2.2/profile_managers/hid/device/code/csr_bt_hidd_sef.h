#ifndef CSR_BT_HIDD_SEF_H__
#define CSR_BT_HIDD_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_hidd_main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* HIDD handler functions */
extern void CsrBtHiddActivateIdleHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddReactivateIdleHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddDeactivateHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddControlRespConnectedHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddDataConnectedHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddUnplugConnectedHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddChangeModeConnectedHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddChangeModeNotConnectedHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddSecurityInHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddSecurityOutHandler(HiddInstanceDataType *instData);

/* CM handler functions */
extern void CsrBtHiddCmRegisterInitHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddCmRegisterSdpRegisteringHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddCmUnregisterSdpRegisteringHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddCmConnectAcceptConnectingHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddCmCancelConnectAcceptConnectingHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddCmConnectConnectingHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddCmModeChangeHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddCmUnregisterSdpUnregisteringHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddCmDataCfmConnectedHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddCmDataIndConnectedHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddCmDisconnectHandler(HiddInstanceDataType *instData);
extern void CsrBtHiddCmDisconnectIgnoreHandler(HiddInstanceDataType *instData);

/* Prototypes from hidd_free_down.c */
void CsrBtHiddFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif
#endif /* CSR_BT_HIDD_SEF_H__ */


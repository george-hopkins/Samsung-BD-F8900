#ifndef CSR_BT_MCAP_SEF_H__
#define CSR_BT_MCAP_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_bt_cmn_linked_list.h"
#include "csr_list.h"

#ifdef __cplusplus
extern "C" {
#endif

/* CSR_BT_MCAP_PRIM event functions */
extern void CsrBtMcapActivateReqHandler(McapInstanceData *pInst);
extern void mcapActivateCfmHandler(McapInstanceData *pInst);
extern void CsrBtMcapDeactivateReqHandler(McapInstanceData *pInst);
extern void mcapDeactivateCfmHandler(McapInstanceData *pInst);
extern void CsrBtMcapConnectMclReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapDisconnectMclReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidDisconnectMclReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapConnectMdlReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidConnectMdlReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapCreateMdlResHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidCreateMdlResHandler(McapInstanceData *pInst);
extern void CsrBtMcapDataSendReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidDataSendReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapDeleteMdlReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidDeleteMdlReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapDeleteMdlResHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidDeleteMdlResHandler(McapInstanceData *pInst);
extern void CsrBtMcapAbortMdlResHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidAbortMdlResHandler(McapInstanceData *pInst);
extern void CsrBtMcapDisconnectMdlReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidDisconnectMdlReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapReconnectMdlReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidReconnectMdlReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapReconnectMdlResHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidReconnectMdlResHandler(McapInstanceData *pInst);
extern void CsrBtMcapSyncCapReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidSyncCapReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapSyncCapResHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidSyncCapResHandler(McapInstanceData *pInst);
extern void CsrBtMcapSyncSetReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidSyncSetReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapSyncSetResHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidSyncSetResHandler(McapInstanceData *pInst);
extern void CsrBtMcapSyncInfoReqHandler(McapInstanceData *pInst);
extern void CsrBtMcapInvalidSyncInfoReqHandler(McapInstanceData *pInst);

extern void CsrBtMcapCmL2caConnectCfmConnectingMclHandler(McapInstanceData *pInst);
extern void CsrBtMcapCmL2caConnectCfmDisconnectingMclHandler(McapInstanceData *pInst);
extern void CsrBtMcapCmL2caConnectCfmHandler(McapInstanceData *pInst);
extern void CsrBtMcapCmL2caCancelConnectAcceptCfmHandler(McapInstanceData *pInst);
extern void CsrBtMcapCmL2caDisconnectIndHandler(McapInstanceData *pInst);
extern void CsrBtMcapCmL2caDataCfmHandler(McapInstanceData *pInst);
extern void CsrBtMcapCmL2caDataIndHandler(McapInstanceData *pInst);
extern void CsrBtMcapCmL2caDataIndIgnore(McapInstanceData *pInst);
extern void CsrBtMcapCmL2caModeChangeIndHandler(McapInstanceData *pInst);
extern void CsrBtMcapCmIgnoreMsg(McapInstanceData *pInst);

/* CSR_BT_CM_PRIM event functions */
extern void CsrBtMcapCmL2caConnectAcceptCfmHandler(McapInstanceData *pInst);

/* Internal functions */
CsrUint8 getMclState(McapInstanceData *pInst, CsrUint16 primType);

#ifdef CSR_LOG_ENABLE
#define CSR_BT_MCAP_STATE_CHANGE(var, val) CsrBtMcapChangeState((CsrCharString*) __FILE__, __LINE__, (CsrCharString*) #var, &(var), val, (CsrCharString*) #val)

void CsrBtMcapChangeState(const CsrCharString *file, CsrUint32 line,
                              const CsrCharString *fsmName,
                              CsrUint8 *state, CsrUint8 newState, const CsrCharString *newStateName);
#else
#define CSR_BT_MCAP_STATE_CHANGE(var, val) (var = val)
#endif

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_MCAP_SEF_H__ */

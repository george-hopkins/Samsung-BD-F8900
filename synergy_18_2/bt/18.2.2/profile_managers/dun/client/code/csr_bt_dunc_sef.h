#ifndef CSR_BT_DUNC_SEF_H__     /* Once is enough */
#define CSR_BT_DUNC_SEF_H__
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_result.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FIND_DATA_PATH_QUEUE(x) \
((CsrSchedQid)( x->dataHandle == CSR_SCHED_QID_INVALID ? x->ctrlHandle : x->dataHandle ))

/* define DUNC handler functions */
extern void CsrBtDuncIdleStateConnectReqHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncConnectStateCancelConnectReqHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncConnectStatePortnegResHandler(DuncInstanceDataType * instData);
extern void CsrBtDuncConnectedStateDisconnectReqHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncConnectedStateDataReqHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncConnectedStateDataResHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncConnectedStateControlReqHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncXstateRegisterDataPathHandleReqHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncXStateChangeDataPathStatusReqHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncConnectedStatePortnegResHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncConnectedStatePortnegReqHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncXSecurityOutReqHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncSendDataPathStatusInd(DuncInstanceDataType * instData, CsrUint8 status, CsrSchedQid handle,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

/* define CM handler functions */
extern void CsrBtDuncConnectedStateCmDisconnectIndHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncDisconnectingStateCmDisconnectIndHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncConnectedStateCmDataCfmHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncConnectedStateCmDataIndHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncConnectedStateCmControlIndHandler(DuncInstanceDataType *instData);

extern void CsrBtDuncConnectedStateCmPortNegIndHandler(DuncInstanceDataType *instData);
extern void CsrBtDuncConnectedStateCmPortNegCfmHandler(DuncInstanceDataType *instData);
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
extern void CsrBtDuncConnectedStateCmModeChangeIndHandler(DuncInstanceDataType *instData);
#else
#define CsrBtDuncConnectedStateCmModeChangeIndHandler NULL
#endif
extern void CsrBtDuncXStateIgnoreCmMsgHandler(DuncInstanceDataType * instData);


/* common functions */
extern void CsrBtDuncIgnoreMsg(DuncInstanceDataType *instData);

extern void CsrBtDuncConnectResultHandler(void *instData,
    CsrUint8           localServerCh,
    CsrUint32               btConnId,
    CsrBtDeviceAddr         deviceAddr,
    CsrUint16                maxFrameSize,
    CsrBool                  validPortPar,
    RFC_PORTNEG_VALUES_T    portPar,
    CsrBtResultCode         resultCode,
    CsrBtSupplier     resultSupplier,
    CmnCsrBtLinkedListStruct * sdpTag);

extern void CsrBtDuncPortNegIndHandler(void                       * instData,
                                  CsrBtDeviceAddr               deviceAddr,
                                  CsrUint8              serverChannel,
                                  RFC_PORTNEG_VALUES_T       portPar,
                                  CsrBool                     request);

extern void CsrBtDuncInitInstanceData(DuncInstanceDataType *instData);
extern void CsrBtDuncCleanupQueue(void);
extern void CsrBtDuncReleaseMessage(CsrUint16 msgType, void * msgData);

#ifdef __cplusplus
}
#endif

#endif /* _DUNC_SEF_H_ */

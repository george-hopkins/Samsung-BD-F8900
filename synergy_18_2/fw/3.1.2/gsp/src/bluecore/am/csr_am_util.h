#ifndef CSR_AM_UTIL_H__
#define CSR_AM_UTIL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_am_handler.h"
#include "csr_list.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Prototypes */
CsrUint16 CsrAmGetBcValue(CsrBccmdCfm *bcCmdMsg);
CsrUint32 CsrAmGetBcValue32bit(CsrBccmdCfm *bcCmdMsg, CsrUint8 offset);
void CsrAmFreeOperationMessage(AmInstanceDataType *inst);
void CsrAmFreeOperationMessageAndQueuedMessages(AmInstanceDataType *inst);
void CsrAmRestoreSavedMessages(AmInstanceDataType *inst);

void CsrAmCreateNewStreamElement(AmInstanceDataType *inst, CsrAmAudioPathConnectReq *prim);

StreamType *CsrAmFindStreamByAppConnId(AmInstanceDataType *inst, CsrUint16 connId);
StreamType *CsrAmFindStreamByAppSrcType(AmInstanceDataType *inst, CsrEndpointType sourceType);
StreamType *CsrAmFindStreamByAppSinkType(AmInstanceDataType *inst, CsrAmEndPointDefType sinkType);
void CsrAmFindAndRemoveSinkFromSinkList(StreamType *stream, CsrAmEndPointDefType sinkType);

CsrBool CsrAmIsSrcInUse(AmInstanceDataType *inst, CsrEndpointType source, StreamType **srcStream);
CsrBool CsrAmIsSinkInUse(AmInstanceDataType *inst, CsrAmEndPointDefType sink, StreamType **sinkStream);

EndpointType *CsrAmFindEndPointBySinkId(CsrCmnList_t *list, CsrUint16 sinkId);
EndpointType *CsrAmStoreNewSinkId(AmInstanceDataType *inst, CsrUint16 sinkId);
EndpointType *CsrAmFindAndRemoveConfigItemFromSinkList(AmInstanceDataType *inst, CsrUint32 key, CsrUint16 sid);
CsrAmEndPointDefType *CsrAmGetNewSinkToAlloc(AmInstanceDataType *inst);

void CsrAmCreateLinkOfSinksToAdd(AmInstanceDataType *instData);
void CsrAmDestroyLinkOfSinksToAdd(AmInstanceDataType *instData);
void CsrAmAddNewSinkToStreamSinkList(AmInstanceDataType *inst);
CsrBool CsrAmNeedToSyncEndpoints(AmInstanceDataType *inst, EndpointType **retEp);
CsrBool CsrAmFindAndDoSinkToAlias(AmInstanceDataType *inst);
CsrBool CsrAmNonAudioGainConfigPresent(CsrUint16 configLen, CsrUint8 *configData);
EndpointType *CsrAmGetEndpointToConfig(StreamType *str, CsrUint16 *sid, CsrAmEndpointType epType, CsrBool removeItem);
EndpointType *CsrAmGetEndpointToConfigBySinkType(CsrCmnList_t *list, CsrUint16 epType);
CsrBool CsrAmConfigDone(StreamType *str);

void CsrAmFreeStreamListAndNewSinkList(AmInstanceDataType *inst);

/* Call back functions */
void CsrAmGetChipVersionCfm(void *inst);
void CsrAmGetBuildIdCfm(void *inst);

void CsrAmSrcAllocCfm(void *inst);
void CsrAmSinkAllocCfm(void *inst);

void CsrAmStreamTransStartCfm(void *inst);
void CsrAmStreamTransCompleteCfm(void *inst);

void CsrAmStreamConnectCfm(void *inst);

void CsrAmStreamSyncSidCfm(void *inst);

void CsrAmStreamAliasAddCfm(void *inst);
void CsrAmStreamConfigCfm(void *inst);

void CsrAmSrcDeallocCfm(void *inst);
void CsrAmSinkDeallocCfm(void *inst);
void CsrAmStreamDisconnectCfm(void *inst);

#ifdef __cplusplus
}
#endif

#endif /* CSR_AM_UTIL_H__ */

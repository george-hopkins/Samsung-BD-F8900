#ifndef CSR_DSPM_SERIALIZE_H__
#define CSR_DSPM_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"
#include "csr_msgconv.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void CsrDspmPfree(void *ptr);

#define CsrDspmActivateReqSer CsrEventCsrUint16Ser
#define CsrDspmActivateReqDes CsrEventCsrUint16Des
#define CsrDspmActivateReqSizeof CsrEventCsrUint16Sizeof
#define CsrDspmActivateReqSerFree CsrDspmPfree

#define CsrDspmDeactivateReqSer CsrEventCsrUint16Ser
#define CsrDspmDeactivateReqDes CsrEventCsrUint16Des
#define CsrDspmDeactivateReqSizeof CsrEventCsrUint16Sizeof
#define CsrDspmDeactivateReqSerFree CsrDspmPfree

extern CsrUint8 *CsrDspmStreamCpOpenReqSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmStreamCpOpenReqDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmStreamCpOpenReqSizeof(void *msg);
extern void CsrDspmStreamCpOpenReqSerFree(void *msg);

extern CsrUint8 *CsrDspmStreamCpCloseReqSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmStreamCpCloseReqDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmStreamCpCloseReqSizeof(void *msg);
extern void CsrDspmStreamCpCloseReqSerFree(void *msg);

#define CsrDspmStreamScoEnableReqSer CsrEventCsrUint16CsrUint8Ser
#define CsrDspmStreamScoEnableReqDes CsrEventCsrUint16CsrUint8Des
#define CsrDspmStreamScoEnableReqSizeof CsrEventCsrUint16CsrUint8Sizeof
#define CsrDspmStreamScoEnableReqSerFree CsrDspmPfree

extern CsrUint8 *CsrDspmStreamConfigureReqSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmStreamConfigureReqDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmStreamConfigureReqSizeof(void *msg);
extern void CsrDspmStreamConfigureReqSerFree(void *msg);

extern CsrUint8 *CsrDspmStreamSyncReqSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmStreamSyncReqDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmStreamSyncReqSizeof(void *msg);
extern void CsrDspmStreamSyncReqSerFree(void *msg);

extern CsrUint8 *CsrDspmOperatorCreateReqSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmOperatorCreateReqDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmOperatorCreateReqSizeof(void *msg);
extern void CsrDspmOperatorCreateReqSerFree(void *msg);

extern CsrUint8 *CsrDspmOperatorDestroyReqSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmOperatorDestroyReqDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmOperatorDestroyReqSizeof(void *msg);
extern void CsrDspmOperatorDestroyReqSerFree(void *msg);

extern CsrUint8 *CsrDspmOperatorMessageReqSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmOperatorMessageReqDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmOperatorMessageReqSizeof(void *msg);
extern void CsrDspmOperatorMessageReqSerFree(void *msg);

extern CsrUint8 *CsrDspmOperatorStartReqSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmOperatorStartReqDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmOperatorStartReqSizeof(void *msg);
extern void CsrDspmOperatorStartReqSerFree(void *msg);

extern CsrUint8 *CsrDspmOperatorStopReqSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmOperatorStopReqDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmOperatorStopReqSizeof(void *msg);
extern void CsrDspmOperatorStopReqSerFree(void *msg);

extern CsrUint8 *CsrDspmOperatorResetReqSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmOperatorResetReqDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmOperatorResetReqSizeof(void *msg);
extern void CsrDspmOperatorResetReqSerFree(void *msg);

extern CsrUint8 *CsrDspmConnectionCreateReqSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmConnectionCreateReqDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmConnectionCreateReqSizeof(void *msg);
extern void CsrDspmConnectionCreateReqSerFree(void *msg);

extern CsrUint8 *CsrDspmConnectionDestroyReqSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmConnectionDestroyReqDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmConnectionDestroyReqSizeof(void *msg);
extern void CsrDspmConnectionDestroyReqSerFree(void *msg);

extern CsrUint8 *CsrDspmCapabilityDownloadReqSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmCapabilityDownloadReqDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmCapabilityDownloadReqSizeof(void *msg);
#define CsrDspmCapabilityDownloadReqSerFree CsrDspmPfree

#define CsrDspmCapabilityRemoveReqSer CsrEventCsrUint16CsrUint16Ser
#define CsrDspmCapabilityRemoveReqDes CsrEventCsrUint16CsrUint16Des
#define CsrDspmCapabilityRemoveReqSizeof CsrEventCsrUint16CsrUint16Sizeof
#define CsrDspmCapabilityRemoveReqSerFree CsrDspmPfree

#define CsrDspmActivateCfmSer CsrEventSer
#define CsrDspmActivateCfmDes CsrEventDes
#define CsrDspmActivateCfmSizeof CsrEventSizeof
#define CsrDspmActivateCfmSerFree CsrDspmPfree

#define CsrDspmDeactivateCfmSer CsrEventSer
#define CsrDspmDeactivateCfmDes CsrEventDes
#define CsrDspmDeactivateCfmSizeof CsrEventSizeof
#define CsrDspmDeactivateCfmSerFree CsrDspmPfree

#define CsrDspmDeactivateIndSer CsrEventSer
#define CsrDspmDeactivateIndDes CsrEventDes
#define CsrDspmDeactivateIndSizeof CsrEventSizeof
#define CsrDspmDeactivateIndSerFree CsrDspmPfree

extern CsrUint8 *CsrDspmStreamCpOpenCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmStreamCpOpenCfmDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmStreamCpOpenCfmSizeof(void *msg);
extern void CsrDspmStreamCpOpenCfmSerFree(void *msg);

extern CsrUint8 *CsrDspmStreamCpCloseCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmStreamCpCloseCfmDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmStreamCpCloseCfmSizeof(void *msg);
extern void CsrDspmStreamCpCloseCfmSerFree(void *msg);

#define CsrDspmStreamScoEnableCfmSer CsrEventCsrUint16Ser
#define CsrDspmStreamScoEnableCfmDes CsrEventCsrUint16Des
#define CsrDspmStreamScoEnableCfmSizeof CsrEventCsrUint16Sizeof
#define CsrDspmStreamScoEnableCfmSerFree CsrDspmPfree

extern CsrUint8 *CsrDspmStreamConfigureCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmStreamConfigureCfmDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmStreamConfigureCfmSizeof(void *msg);
extern void CsrDspmStreamConfigureCfmSerFree(void *msg);

extern CsrUint8 *CsrDspmStreamSyncCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmStreamSyncCfmDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmStreamSyncCfmSizeof(void *msg);
extern void CsrDspmStreamSyncCfmSerFree(void *msg);

extern CsrUint8 *CsrDspmOperatorCreateCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmOperatorCreateCfmDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmOperatorCreateCfmSizeof(void *msg);
extern void CsrDspmOperatorCreateCfmSerFree(void *msg);

extern CsrUint8 *CsrDspmOperatorDestroyCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmOperatorDestroyCfmDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmOperatorDestroyCfmSizeof(void *msg);
extern void CsrDspmOperatorDestroyCfmSerFree(void *msg);

extern CsrUint8 *CsrDspmOperatorMessageCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmOperatorMessageCfmDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmOperatorMessageCfmSizeof(void *msg);
extern void CsrDspmOperatorMessageCfmSerFree(void *msg);

extern CsrUint8 *CsrDspmOperatorMessageIndSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmOperatorMessageIndDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmOperatorMessageIndSizeof(void *msg);
extern void CsrDspmOperatorMessageIndSerFree(void *msg);

extern CsrUint8 *CsrDspmOperatorStartCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmOperatorStartCfmDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmOperatorStartCfmSizeof(void *msg);
extern void CsrDspmOperatorStartCfmSerFree(void *msg);

extern CsrUint8 *CsrDspmOperatorStopCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmOperatorStopCfmDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmOperatorStopCfmSizeof(void *msg);
extern void CsrDspmOperatorStopCfmSerFree(void *msg);

extern CsrUint8 *CsrDspmOperatorResetCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmOperatorResetCfmDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmOperatorResetCfmSizeof(void *msg);
extern void CsrDspmOperatorResetCfmSerFree(void *msg);

extern CsrUint8 *CsrDspmConnectionCreateCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmConnectionCreateCfmDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmConnectionCreateCfmSizeof(void *msg);
extern void CsrDspmConnectionCreateCfmSerFree(void *msg);

extern CsrUint8 *CsrDspmConnectionDestroyCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmConnectionDestroyCfmDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmConnectionDestroyCfmSizeof(void *msg);
extern void CsrDspmConnectionDestroyCfmSerFree(void *msg);

extern CsrUint8 *CsrDspmCapabilityDownloadCfmSer(CsrUint8 *ptr, CsrSize *len, void *msg);
extern void *CsrDspmCapabilityDownloadCfmDes(CsrUint8 *buffer, CsrSize len);
extern CsrSize CsrDspmCapabilityDownloadCfmSizeof(void *msg);
#define CsrDspmCapabilityDownloadCfmSerFree CsrDspmPfree

#define CsrDspmCapabilityRemoveCfmSer CsrEventCsrUint16CsrUint16Ser
#define CsrDspmCapabilityRemoveCfmDes CsrEventCsrUint16CsrUint16Des
#define CsrDspmCapabilityRemoveCfmSizeof CsrEventCsrUint16CsrUint16Sizeof
#define CsrDspmCapabilityRemoveCfmSerFree CsrDspmPfree

#ifdef __cplusplus
}
#endif

#endif

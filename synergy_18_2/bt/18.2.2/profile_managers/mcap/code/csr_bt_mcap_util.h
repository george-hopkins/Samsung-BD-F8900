#ifndef CSR_BT_MCAP_UTIL_H__
#define CSR_BT_MCAP_UTIL_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_cmn_linked_list.h"
#include "csr_list.h"
#include "csr_bt_mcap_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void CsrBtMcapUtilCmnListPsmAdd(CsrCmnListElm_t *elem);
extern void CsrBtMcapUtilCmnListPsmRemove(CsrCmnListElm_t *elem);
extern void CsrBtMcapUtilCmnListMclAdd(CsrCmnListElm_t *elem);
extern void CsrBtMcapUtilCmnListMclRemove(CsrCmnListElm_t *elem);
extern void CsrBtMcapUtilCmnListMdlAdd(CsrCmnListElm_t *elem);
extern void CsrBtMcapUtilCmnListMdlRemove(CsrCmnListElm_t *elem);

extern CsrBool CsrBtMcapUtilComparePsm(CsrCmnListElm_t *elem, void *data);
extern CsrBool CsrBtMcapUtilCompareBdAddr(CsrCmnListElm_t *elem, void *data);
extern CsrBool CsrBtMcapUtilCompareMdlId(CsrCmnListElm_t *elem, void *data);
extern CsrBool CsrBtMcapUtilCompareLocalControlPsm(CsrCmnListElm_t *elem, void *data);
extern CsrBool CsrBtMcapUtilCompareLocalControlPsmAndMclState(CsrCmnListElm_t *elem, void *data);
extern CsrBool CsrBtMcapUtilCompareLocalControlPsmAndBdAddr(CsrCmnListElm_t *elem, void *data);
extern CsrBool CsrBtMcapUtilCompareLocalDataPsm(CsrCmnListElm_t *elem, void *data);
extern CsrBool CsrBtMcapUtilCompareLocalDataPsmAndBdAddr(CsrCmnListElm_t *elem, void *data);
extern CsrBool CsrBtMcapUtilCompareRemoteControlPsm(CsrCmnListElm_t *elem, void *data);
extern CsrBool mcapUtilCompareRemote_DataPsm(CsrCmnListElm_t *elem, void *data);
extern CsrBool CsrBtMcapUtilCompareMclState(CsrCmnListElm_t *elem, void *data);
extern CsrBool CsrBtMcapUtilCompareMclId(CsrCmnListElm_t *elem, void *data);
extern CsrBool CsrBtMcapUtilCompareMdlCid(CsrCmnListElm_t *elem, void *data);
extern CsrBool CsrBtMcapUtilCompareMdlState(CsrCmnListElm_t *elem, void *data);

extern void CsrBtMcapFreeUpstreamMessageContents(void *msg_data);
extern CsrBool CsrBtMcapIsDisconnectAbnormal(CsrBtResultCode resultCode, CsrBtSupplier supplier);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_MCAP_UTIL_H__ */

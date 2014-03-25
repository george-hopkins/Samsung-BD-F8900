#ifndef CSR_BT_HDP_UTIL_H__
#define CSR_BT_HDP_UTIL_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_hdp_handler.h"
#include "csr_bt_hdp_prim.h"
#include "csr_list.h"
#include "csr_bt_mcap_private_lib.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct CsrBtHdpMdepListTag
{
    CsrBtHdpMdep node;
    struct CsrBtHdpMdepListTag *next;
}CsrBtHdpMdepList;

/* move to a separate file */
#define IN_RANGE(primType, lo, up)  ((primType >= lo) && (primType < up))
#define IS_VALID_PTR(p)             ((p) != NULL)


#define HDP_INVALID_MDLID       (00)
#define HDP_MAX_MDL_ID          (16)
#define HDP_MCLID_OFFSET        (16)
#define HDP_CTRL_PSM_OFFSET     (16)
#define HDP_MDLID_MASK          (0xFFFF)
#define HDP_DATA_PSM_MASK       (0xFFFF)
#define HDP_WILDCARD_ID         (0xFFFF)
#define HDP_FORM_ASSOC_CH_ID(mclId, mdlId) ( ((CsrUint32)mclId << HDP_MCLID_OFFSET ) | mdlId)
#define HDP_GET_MCLID(assocChId)  (CsrUint16)((assocChId) >> HDP_MCLID_OFFSET)
#define HDP_GET_MDLID(assocChId)  (CsrUint16)((assocChId) & HDP_MDLID_MASK )
#define HDP_GET_CTRL_PSM(instanceId)    (CsrUint16)(instanceId >> HDP_CTRL_PSM_OFFSET)
#define HDP_GET_DATA_PSM(instanceId)    (CsrUint16)(instanceId & HDP_DATA_PSM_MASK)
#define HDP_GET_LB(uint16)    ((uint16) & 0x00FF)
#define HDP_GET_HB(uint16)    ((uint16) >> 8)


#define HDP_MDEP_ADD_ENTRY(list)  (HdpMdepListEntry *)CsrCmnListElementAddLast((CsrCmnList_t *)&(list),sizeof(HdpMdepListEntry))
#define HDP_MDEP_DEL_ENTRY(CsrCmnList_t,pCsrCmnListElm_t)  CsrCmnListElementRemove(&CsrCmnList_t ,(CsrCmnListElm_t *)pCsrCmnListElm_t)
#define HDP_MDEP_GET_FIRST(CsrCmnList_t)   (HdpMdepListEntry *)CsrCmnListGetFirst(&(CsrCmnList_t))
#define HDP_MDEP_GET_NEXT(pCsrCmnListElm_t) pCsrCmnListElm_t = ((pCsrCmnListElm_t)->next)
CsrBool CsrBtHdpUtilCmnListIsMdepBusy(CsrUint16 mdepId,CsrCmnList_t mdepList);
HdpMdepListEntry * CsrBtHdpUtilCmnListMdepMatchingMdepFree(CsrCmnList_t mdepList, CsrBtMdepId mdepId);
HdpMdepListEntry * CsrBtHdpUtilCmnListMdepMatchingMdepBusy(CsrCmnList_t mdepList, CsrBtMdepId mdepId);
HdpMdepListEntry * CsrBtHdpUtilCmnListMdepMatchingMdep(CsrCmnList_t mdepList, CsrBtMdepId mdepId);


#define HDP_CONNECTION_ADD_ENTRY(connectionList)  (HdpConnectionListEntry *)CsrCmnListElementAddLast((CsrCmnList_t *)&(connectionList),sizeof(HdpConnectionListEntry))
#define HDP_CONNECTION_DEL_ENTRY(CsrCmnList_t,pCsrCmnListElm_t)  CsrCmnListElementRemove(&CsrCmnList_t ,(CsrCmnListElm_t *)pCsrCmnListElm_t)
#define HDP_CONNECTION_GET_FIRST(CsrCmnList_t)   (HdpConnectionListEntry *)CsrCmnListGetFirst(&(CsrCmnList_t))
#define HDP_CONNECTION_GET_NEXT(pCsrCmnListElm_t) pCsrCmnListElm_t = ((pCsrCmnListElm_t)->next)
HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMatchingAssocChId(HdpConnectionListEntry *pList, CsrUint32 assocChId);
HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMatchingMclMdl(HdpConnectionListEntry *pList, CsrUint16 mclId,CsrUint16 mdlId);
HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMatchingMcl(HdpConnectionListEntry *pList, CsrUint16 mclId);
HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMatchingMclMdep(HdpConnectionListEntry *pList, CsrUint16 mclId,CsrBtMdepId mdep);
HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMatchingBdAddr(HdpConnectionListEntry * pList, CsrBtDeviceAddr * bd_addr);
HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryGetActiveMdlInAssocChId(HdpConnectionListEntry *pList, CsrUint32 assocChId);
HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryGetInActiveMdlInAssocChId(HdpConnectionListEntry *pList, CsrUint32 assocChId);
HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryGetActiveMdepInMcl(HdpConnectionListEntry *pList, CsrUint16 mclId);
HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMclActiveOnAssocChId(HdpConnectionListEntry *pList, CsrUint32 assocChId);
HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryMatchingMclMdlBdPsm(HdpConnectionListEntry *pList,
                                                               CsrUint16 mclId,
                                                               CsrUint16 mdlId,
                                                               CsrBtDeviceAddr  *bd_addr,
                                                               CsrUint16 psm);
HdpConnectionListEntry *CsrBtHdpUtilConnectionEntryGetMatchingBdMcl(HdpConnectionListEntry *pList,
                                                                  CsrBtDeviceAddr  *bd_addr,
                                                                  CsrUint16 mclId);
CsrUint16 CsrBtHdpUtilFindNumberofMdepEntries(HdpConnectionListEntry *pList, CsrBtMdepId mdepId);





void CsrBtHdpUtilConnectionEntryUpdateStatusMcl(HdpConnectionListEntry *pList, CsrUint16 mclId,CsrBool status);
/*void hdpUtilConnectionEntryDeleteMclEntry(CsrCmnList_t connectionList, CsrUint16 mclId);*/
CsrBool CsrBtHdpUtilConnectionEntryFindOtherConnection(HdpConnectionListEntry *pList,CsrUint32 assocChId);
CsrUint16 CsrBtHdpUtilFindNumberofEntries(HdpConnectionListEntry *pList, CsrUint16 mclId);
CsrBool CsrBtHdpUtilConnectionEntryDeleteMdl(HdpConnectionListEntry *pList, CsrUint32 assocId);
CsrBool CsrBtHdpUtilFindActiveMdl(HdpConnectionListEntry *pList);

void CsrBtHdpSdcStartSearch(HdpInstanceData * pInstance,CsrBtDeviceAddr addr);


void CsrBtHdpAssignDefaultValues(HdpInstanceData  *pInstance);
CsrUint16 buildHdpServiceRecord(HdpInstanceData  *pInstance, CsrUint8 **record);
void CsrBtHdpCopyRegisterReqAttributes(HdpInstanceData *pInstance,CsrBtHdpRegisterReq *pMsg);

CsrUint16 CsrBtHdpUtilGetNextFreeMdl( CsrUint16 *mdlList);
void CsrBtHdpUtilMdepIdUnCheckBusy(CsrUint16 mdepId, CsrCmnList_t mdepList);

void CsrBtHdpSdpInitServiceRecordStruct(CsrBtHdpSdpRecord *rec);
CsrBool CsrBtHdpSdpExtractServiceRecordData(CmnCsrBtLinkedListStruct *bll_p,
                                                CsrUint16  serviceHandleIndex,
                                                CsrBtHdpSdpRecord *sdpRecord,
                                                CsrUint32 *handle,
                                                CsrUint8 dataTypeMask,
                                                CsrUint8 roleMask);


L2CA_FLOW_T * CsrBtHdpFillFlowParameters(l2ca_flow_mode_t mode, CsrUint16 mtu);

void CsrBtHdpDeInitInstance(HdpInstanceData *pInstance);
CsrUint32 HdpStrLen(const char *str);
void CsrBtHdpUtilUpdateConnectionListForReconnect(HdpInstanceData *instData,CsrBtHdpSdpRecord *record,CsrUint32 *handle,CsrBtDeviceAddr * bd_addr);




#ifdef __cplusplus
   }
#endif

#endif


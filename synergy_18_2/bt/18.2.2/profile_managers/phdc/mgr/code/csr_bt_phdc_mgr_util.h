#ifndef CSR_BT_PHDC_MGR_UTIL_H__
#define CSR_BT_PHDC_MGR_UTIL_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_util.h"
#include "csr_bt_phdc_mgr_main.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PHDC_MGR_TIMEOUT_ASSOCIATION            (10 * CSR_SCHED_SECOND)
#define PHDC_MGR_TIMEOUT_CONFIGURATION          (10 * CSR_SCHED_SECOND)
#define PHDC_MGR_TIMEOUT_GET                     (3 * CSR_SCHED_SECOND)
#define PHDC_MGR_TIMEOUT_ASSOCIATION_RELEASE     (3 * CSR_SCHED_SECOND) 

#define PHDC_MGR_TIMEOUT_INACTIVTY              (20 * CSR_SCHED_SECOND)


#define PHDC_MGR_MOVE_LF_16(x)     (((CsrUint32)(x)) << 16 )

#define PHDC_MGR_DEVICE_LIST_ADD_DEVICE(list)                           ((PhdcMgrDeviceListEntry *)CsrCmnListElementAddFirst((CsrCmnList_t *)&(list), sizeof(PhdcMgrDeviceListEntry)))
#define PHDC_MGR_DEVICE_LIST_REMOVE_DEVICE(list, elementPtr)            (CsrCmnListElementRemove((CsrCmnList_t *)&(list), (CsrCmnListElm_t *)(elementPtr)))
#define PHDC_MGR_DEVICE_LIST_FIND_FROM_BD(list, pBd)                    ((PhdcMgrDeviceListEntry *)CsrCmnListSearch(&(list), CsrBtPhdcMgrCompareRemoteBdAddr, (void *)(pBd)))
#define PHDC_MGR_DEVICE_LIST_FIND_FROM_DATA_CH_ID(list, chId)           ((PhdcMgrDeviceListEntry *)CsrCmnListSearch(&(list), CsrBtPhdcMgrCompareDataChannelId,(chId)))

#define PHDC_MGR_OBJECT_LIST_ADD_OBJECT(list)                           ((PhdcMgrConfigObjectEntry *)CsrCmnListElementAddFirst((CsrCmnList_t *)&(list), sizeof(PhdcMgrConfigObjectEntry)))
#define PHDC_MGR_OBJECT_LIST_REMOVE_OBJECT(list, elementPtr)            (CsrCmnListElementRemove((CsrCmnList_t *)&(list), (CsrCmnListElm_t *)(elementPtr)))
#define PHDC_MGR_OBJECT_LIST_FIND_FROM_HANDLE(list, objHdl)             ((PhdcMgrConfigObjectEntry *)CsrCmnListSearch(&(list), CsrBtPhdcMgrCompareObjectHandle, (void *)(objHdl)))
#define PHDC_MGR_OBJECT_LIST_FIND_FROM_OBJ_TYPE(list,objType)           ((PhdcMgrConfigObjectEntry *)CsrCmnListSearch(&(list), CsrBtPhdcMgrCompareObjectType, (void *)(objType)))

#define PHDC_MGR_TYPE_SPEC_ADD_TYPE(list)                               ((PhdcMgrDeviceDataTypeEntry *)CsrCmnListElementAddFirst((CsrCmnList_t *)&(list), sizeof(PhdcMgrDeviceDataTypeEntry)))
#define PHDC_MGR_TYPE_SPEC_REMOVE_TYPE(list, elementPtr)                (CsrCmnListElementRemove((CsrCmnList_t *)&(list), (CsrCmnListElm_t *)(elementPtr)))
#define PHDC_MGR_TYPE_SPEC_FIND_FROM_DATA_TYPE(list, dataType)          ((PhdcMgrDeviceDataTypeEntry *)CsrCmnListSearch(&(list), CsrBtPhdcMgrCompareMdepDataType, (void *)(dataType)))
#define PHDC_MGR_TYPE_SPEC_FIND_FROM_TYPE_SPEC(list, typeSpec)          ((PhdcMgrDeviceDataTypeEntry *)CsrCmnListSearch(&(list), CsrBtPhdcMgrCompareTypeSpec, (void *)(typeSpec)))

CsrBool     CsrBtPhdcMgrCompareRemoteBdAddr(CsrCmnListElm_t *elem, void *data);
CsrBool     CsrBtPhdcMgrCompareDataChannelId(CsrCmnListElm_t *elem, void *data);

void        CsrBtPhdcMgrCmnListObjectAdd(CsrCmnListElm_t *elem);
void        CsrBtPhdcMgrCmnListObjectRemove(CsrCmnListElm_t *elem);
CsrBool     CsrBtPhdcMgrCompareObjectHandle(CsrCmnListElm_t *elem, void *data);
CsrBool     CsrBtPhdcMgrCompareMdepDataType(CsrCmnListElm_t *elem, void *data);
CsrBool     CsrBtPhdcMgrCompareTypeSpec(CsrCmnListElm_t *elem, void *data);

void        CsrBtPhdcMgrCmnListDeviceAdd(CsrCmnListElm_t *elem);
void        CsrBtPhdcMgrCmnListDeviceRemove(CsrCmnListElm_t *elem);

void        CsrBtPhdcMgrCmnListTypeSpecAdd(CsrCmnListElm_t *elem);
void        CsrBtPhdcMgrCmnListTypeSpecRemove(CsrCmnListElm_t *elem);

CsrBool     CsrBtPhdcMgrCanTrasmit(PhdcMgrDeviceListEntry *pDevice);
CsrUint16   CsrBtPhdcMgrGetInvokeIdTx(PhdcMgrDeviceListEntry *pDevice);
void        CsrBtPhdcMgrUpdateInvokeIdRcv(PhdcMgrDeviceListEntry *pDevice);
void        CsrBtPhdcMgrUpdateInvokeIdAck(APDU *pApdu,PhdcMgrDeviceListEntry *pDevice );
CsrUint16   PhdcMgrMapDataTypeToIEEE(CsrBtMdepDataType   datatype);

Associate_result CsrBtPhdcMgrIsAssociationComptabile(AARQ_apdu *pAarq,PhdcMgrMDSInfo *pMds );

CsrBool     CsrBtPhdcMgrIsMDC_NOTI_CONFIG(APDU *pApdu );
CsrBool     CsrBtPhdcMgrIsGetRsp(APDU *pApdu );

CsrBool     CsrBtPhdcMgrUpdateDevConfig(APDU *pApdu, PhdcMgrMDSInfo *pMds );
AttrValMap* CsrBtPhdcMgrGetAttrValMap(ConfigObject *pConfigObj);
MetricIdList* CsrBtPhdcMgrGetMetricIdList(ConfigObject *pConfigObj);
CsrUint16   CsrBtPhdcMgrDecodeAttrValMap(CsrUint8 *msgPtr,AttrValMap **ppAttrValMap);
CsrUint16   CsrBtPhdcMgrDecodeMetricIdList(CsrUint8 *msgPtr,MetricIdList **ppMetricIdList);
CsrUint16   CsrBtPhdcMgrDecodeBasicNuObsValueCmp(CsrUint8 *msgPtr,BasicNuObsValueCmp **ppBasicNuObsValueCmp);
CsrUint16   CsrBtPhdcMgrGetNoOfConfigObj(APDU *pApdu );
ConfigObject* CsrBtPhdcMgrGetConfigObject(APDU *pApdu,CsrUint8 idx );
CsrUint16   CsrBtPhdcMgrGetUnitCode(ConfigObject *pConfigObj);
CsrBool     CsrBtPhdcMgrIsGetRegulatoryParam(APDU *pApdu, RegCertDataList **ppRegCertDataList);
CsrBool     CsrBtPhdcMgrIsContinua(RegCertDataList *pRegCertDataList);
ContinuaBodyStruct * CsrBtPhdcMgrGetContinuaStruct(RegCertDataList *pRegCertDataList);
CsrBool     CsrBtPhdcMgrIsContinuaCertifiedCompatible(PhdcMgrDeviceDataTypeEntry *pDataTypeEntry,ContinuaBodyStruct *pContinuaInfo);
ConfigResult CsrBtPhdcMgrIsSupportedStandardConfig(CsrUint32 devConfigId);
CsrBool     CsrBtPhdcMgrIsSupportedDataType(CsrBtMdepDataType   dataType,PhdcMgrDeviceListEntry *pDevice );
CsrBool     CsrBtPhdcMgrGetMDC_ATTR_SYS_TYPE_SPEC_LIST (APDU *pApdu,TypeVerList **ppSpec );
OID_Type    CsrBtPhdcMgrIsData(APDU *pApdu );
CsrBool     CsrBtPhdcMgrIsConfirmed(APDU *pApdu);
void        CsrBtPhdcMgrHandleObsScanFixed(CsrSchedQid qId,PhdcMgrDeviceListEntry *pDevice,APDU *pApdu );
CsrBool     CsrBtPhdcMgrHandleObsScanVar(CsrSchedQid qId,PhdcMgrDeviceListEntry *pDevice,APDU *pApdu );

CsrUint32   CsrBtPhdcMgrGetObjectType(ConfigObject *pConfigObj);

/* Free function's for PDU decode */
void        CsrBtPhdcMgrFreeDecodeAarq(APDU *pApdu);
void        CsrBtPhdcMgrFreeDecodePrstNotifyConfig(APDU *pApdu);
void        CsrBtPhdcMgrFreeDecodePrstGetMds(APDU *pApdu);
void        CsrBtPhdcMgrFreeDecodePrstGetMdsRsp(APDU * pApdu);
void        CsrBtPhdcMgrFreeRegCertDataList(RegCertDataList * pRegCertDataList);
void        CsrBtPhdcMgrFreeDecodePrstObsScanFixed(APDU *pApdu);
void        CsrBtPhdcMgrFreeDecodePrstObsScanVar(APDU *pApdu);
CsrBool     CsrBtPhdcMgrFreeDecodeAnyApdu(APDU *pApdu,CsrUint16 *pChoice);
void        CsrBtPhdcMderFreeDecodePrst(APDU *pApdu);
void        CsrBtPhdcMgrFreeDataProtoContents(DataProto  *pData);

/* Assertion helpers */
#if !defined(__KERNEL__) && defined(_DEBUG)
#include <assert.h>
#define CSR_BT_PHDC_M_ASSERT(x) assert(x)
#endif
#ifndef CSR_BT_PHDC_M_ASSERT
#define CSR_BT_PHDC_M_ASSERT(x)
#endif


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /*  CSR_BT_PHDC_MGR_UTIL_H*/


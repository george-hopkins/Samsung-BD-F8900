#ifndef CSR_BT_PHDC_IEEE_UTILS_H__
#define CSR_BT_PHDC_IEEE_UTILS_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


/* IEEE Headers */
#include "csr_bt_phdc_ieee_asn.h"
#include "csr_bt_phdc_ieee.h"


/* Relative time resolution - 125 Micro-secs -> LSB. Relative time period spans upto
6.2 days.*/
#define GET_RELATIVE_TIME_FROM_MS(x) ((x * 1000)/125)

/* Global variable Prototypes */
extern const AttributeIdList ieeeSupportedMdsAttrList;

/* Function Prototypes */
extern void updatepAssoRspData(CsrUint8 *systemId);

extern CsrUint16 checkAnyDataChannelActive(void);

extern CsrUint16 checkOnlyLEConnected(ieeeLogEntityType logEntity);

extern CsrUint16 generateIeeeMdlId(CsrUint16 mgrIdx);

extern ieeeLogEntityType getLogEntityFromMdlId(CsrUint16 mdl);

extern ieeeLogEntityType clearManagerDataSink(Sink dataSink);

extern void clearIeeeContext(void);

extern void fillConfigObjectInfo(ConfigObject *pConfObj, const ObjectInfo *pObjInfo);

extern tBool updateConfigObj(ObservationScan *pObsScan, const ObjectInfo *pConfObj);

extern CsrUint16 getDevConfigId(void);

extern void getAttrbDataInfo(HANDLE objHandle, AttrDataFormat **pAttrDataFrmt);

extern tBool getConfigInfo(const ConfigTable *pConfigInfo);

extern void fillSysTypeSpecList(TypeVerList *typeList);

extern void fillAttrIdModel(SystemModel *sysModel);

extern void fillSystemId(octet_string *systemId);

extern CsrUint16 getMdsAttrbCount(const AttributeIdList *pAttrbIdList, AttributeInfoList *pAttrList);

extern void fillIeeeSupportedMDSAttributeInfo(AVA_Type *pAttributeData, CsrUint16 *pAttrCnt);

extern void fillApplicationSupportedMDSAttributeInfo(AVA_Type *pAttributeData, AttributeInfo *pAttrbInfo, CsrUint16 *pAttrCnt);

#if 0
void fillAbsoluteTimer(AbsoluteTime *absTime, tIeeeManufactureInfo *pManuInfo);
#endif

#ifdef __cplusplus
extern "C" }
#endif

#endif

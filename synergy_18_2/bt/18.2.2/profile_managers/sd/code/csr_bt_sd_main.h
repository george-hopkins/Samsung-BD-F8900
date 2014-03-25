#ifndef CSR_BT_SD_MAIN_H__
#define CSR_BT_SD_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_message_queue.h"
#include "csr_env_prim.h"
#include "csr_list.h"

#include "csr_bt_result.h"
#include "csr_bt_util.h"
#include "csr_bt_profiles.h"
#include "csr_bt_uuids.h"
#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_cmn_sdr_tagbased_lib.h"
#include "csr_bt_cmn_sdp_lib.h"
#include "sdc_prim.h"
#include "sds_prim.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_sd_private_prim.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sc_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif
#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif
#ifdef CSR_BT_LE_ENABLE
#include "csr_bt_gatt_prim.h"
#include "csr_bt_gatt_lib.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SD_SDP_ATTEMPT                          (100)

/* Process states */
#define SD_STATE_PROC_IDLE                      (0)
#define SD_STATE_PROC_INQ                       (1)
#define SD_STATE_PROC_INQ_WITH_LE               (2)
#define SD_STATE_PROC_RRN                       (3)
#define SD_STATE_PROC_CRRN                      (4)
#define SD_STATE_PROC_CRRN_TO_STOP              (5)
#define SD_STATE_PROC_SDC_SEARCH                (6)
#define SD_STATE_PROC_SDC_SEARCH_CANCEL         (7)
#define SD_STATE_PROC_SDS_REGISTER              (8)
#define SD_STATE_PROC_SDS_UNREGISTER            (9)
#define SD_STATE_PROC_SET_EVENT_TO_START        (10)
#define SD_STATE_PROC_SET_EVENT_TO_STOP         (11)
#define SD_STATE_PROC_SC_READ_DEVICE_RECORD     (12)

/* Basebands used for scan/inquiry */
#define SD_BASEBAND_NONE                        (0x00)
#define SD_BASEBAND_BREDR                       (0x01)
#define SD_BASEBAND_LE                          (0x02)

/* App states */
#define SD_STATE_APP_IDLE                       (0)
#define SD_STATE_APP_INQ                        (1)
#define SD_STATE_APP_OTHER                      (2)
#define SD_STATE_APP_INIT                       (3)

#define NUM_OF_DOWNSTREAM_APP_STATES            (4)
#define NUM_OF_UPSTREAM_APP_STATES              (3)

/* Main search states */
#define SD_STATE_SEARCH_IDLE                    (0)
#define SD_STATE_SEARCH_NOT_BUFFERING           (1)
#define SD_STATE_SEARCH_BUFFERING               (2)
#define SD_STATE_SEARCH_BUFFER_PROC             (3)

/* Search instance states */
#define SD_STATE_SEARCH_INST_IDLE               (0)
#define SD_STATE_SEARCH_INST_SEARCH             (1)

#define SD_DI_INST_IDX_IMM                      (0)             /* Immediate result has been sent */
#define SD_DI_INST_IDX_NORMAL                   (1)             /* Normal result has been sent */

#ifdef CSR_BT_LE_ENABLE
#define SD_DI_INST_IDX_ADVERT                   (2)             /* Immediate LE advertise result has been sent */
#define SD_DI_INST_IDX_SCANRSP                  (3)             /* Immediate LE scan-response result has been sent */
#define SD_DI_INST_IDX_COUNT                    (4)
#else
#define SD_DI_INST_IDX_COUNT                    (2)
#endif

#define SD_DI_DEV_FLAG_FOUND                    (0x00000001)    /* The device was found during the current search */
#define SD_DI_DEV_FLAG_PAIRED                   (0x00000002)    /* The device is paired */
#define SD_DI_DEV_FLAG_TRUSTED                  (0x00000004)    /* The device is trusted */
#define SD_DI_DEV_FLAG_EIR                      (0x00000008)    /* The device supports EIR */
#define SD_DI_DEV_FLAG_NAME_UPDATED             (0x00000010)    /* The name has been updated in the current search */
#define SD_DI_DEV_FLAG_NAME_ATTEMPT             (0x00000020)    /* Reading device name over BR/EDR has/is being attempted */
#define SD_DI_DEV_FLAG_DONE                     (0x00000040)    /* Processing for the current device is done for the current search */
#define SD_DI_DEV_FLAG_ADVERT                   (0x00000080)    /* Advertise report received */
#define SD_DI_DEV_FLAG_SCANRSP                  (0x00000100)    /* Scan Response report received */
#define SD_DI_DEV_FLAG_NO_CLASSIC               (0x00000200)    /* LE report claism NO BR/EDR support */
#define SD_DI_DEV_FLAG_GATT_NAME_ATTEMPT        (0x00000400)    /* Reading device name over LE has/is being attempted */
#define SD_DI_DEV_FLAG_LE_PRIVATE_ADDR          (0x00000800)    /* LE private/random address */
#define SD_DI_DEV_FLAG_SEEN_BREDR               (0x00001000)    /* We have seen the BR/EDR radio... */

/* Misc */
#define SD_SEARCH_INSTANCES_MAX                 (16)            /* Do not change this - defined in API doc */
#define SD_ALL_INSTANCE_BITS                    (0x0000FFFF)    /* All 16 bits set for 'activeInstFlags' */
#define SD_SC_READ_DEVICE_RECORD_BYTES_MAX      (512)           /* Maximum number of bytes per CSR_BT_SC_READ_DEVICE_RECORD_IND */
#define SD_POOL_SIZE_FOR_SERVICE_RECORD_HANDLES (5)
#define SD_POOL_SIZE_FOR_UUID32_LIST            (5)

#define SD_LE_REPORT_LENGTH                     (2*CSR_BT_CM_LE_MAX_REPORT_LENGTH) /* because of our extended tag,length,value system */

#define SD_SS_LIST_INCREMENT                    (10) /* increment/decrement device info list by this many elements */

#define SD_CANCEL_TIMER(CsrSchedTid)                            (CsrSchedTimerCancel((CsrSchedTid), NULL, NULL)); ((CsrSchedTid) = 0)
#define SD_COMPARE_COD(dev_cod, inst_cod, inst_cod_mask) \
    (inst_cod_mask == 0) ?  ((inst_cod == 0) ? 1 : (dev_cod & inst_cod) > 0) : ((dev_cod & inst_cod_mask) == (inst_cod & inst_cod_mask))
#define SD_CHANGE_STATE(var, state)                     ((var) = (state))

/* Device List specific macros */
#define SD_DL_ADD_DEVICE(list, infoLen)                 ((SdDeviceInfo_t *)CsrCmnListElementAddFirst((CsrCmnList_t *)&(list), sizeof(SdDeviceInfo_t) + (infoLen)))
#define SD_DL_REMOVE_DEVICE(list, elementPtr)           (CsrCmnListElementRemove((CsrCmnList_t *)&(list), (CsrCmnListElm_t *)(elementPtr)))
#define SD_DL_GET_FIRST(list)                           ((SdDeviceInfo_t *)CsrCmnListGetFirst(&(list)))
#define SD_DL_GET_LAST(list)                            ((SdDeviceInfo_t *)CsrCmnListGetLast(&(list)))
#define SD_DL_FIND_FROM_ADDR(list, addrPtr)             ((SdDeviceInfo_t *)CsrCmnListSearch(&(list), CsrBtSdUtilCompareBdaddr, (void *)(addrPtr)))
#define SD_DL_ITERATE(list, func, dataPtr)              (CsrCmnListIterate(&(list), (func), (void *)(dataPtr)))
#define SD_DL_COUNT(list)                               (CsrCmnListGetCount(&(list)))

/* Search Instance specific macros */
#define SD_SI_ADD_INST(list)                            ((SdSearchInstance_t *)CsrCmnListElementAddFirst((CsrCmnList_t *)&(list), sizeof(SdSearchInstance_t)))
#define SD_SI_GET_FIRST(list)                           ((SdSearchInstance_t *)CsrCmnListGetFirst(&(list)))
#define SD_SI_REMOVE_INST(list, instPtr)                (CsrCmnListElementRemove((CsrCmnList_t *)&(list), (CsrCmnListElm_t *)(instPtr)))
#define SD_SI_ITERATE(list, func, dataPtr)              (CsrCmnListIterate(&(list), (func), (void *)(dataPtr)))
#define SD_SI_COUNT(list)                               (CsrCmnListGetCount(&(list)))
#define SD_SI_FIND_FROM_PHANDLE(list, handlePtr)        ((SdSearchInstance_t *)CsrCmnListSearch(&(list), CsrBtSdUtilSearchInstComparePhandle, (void *)(handlePtr)))

typedef struct SdDeviceInfoTag
{
    struct SdDeviceInfoTag      *next;          /* Do not move - used for common linked list handling */
    struct SdDeviceInfoTag      *prev;          /* Do not move - used for common linked list handling */
    CsrBtDeviceAddr              addr;
    CsrBtClassOfDevice           cod;
    CsrInt8                      rssi;
    CsrUint32                    devFlags;       /* Found in current search?, paired?, EIR?, name updated? (SD_DEVICE_INFO_DEV_FLAG_X) */
    CsrUint32                    instFlags[SD_DI_INST_IDX_COUNT]; /* Has instance been notified about an event */
    CsrUint32                    age;            /* An "ever" increasing number used for determining which entry to overwrite*/

#ifdef CSR_BT_LE_ENABLE
    CsrUint16                    advertLen;
    CsrUint8                     advert[SD_LE_REPORT_LENGTH];
    CsrUint16                    scanrspLen;
    CsrUint8                     scanrsp[SD_LE_REPORT_LENGTH];
    CsrInt8                      leRssi;
    CsrBtConnId                  btConnId;
    CsrUint32                    serviceChangedHandle;
#endif
    CsrUint16                    listCount;
    CsrBtUuid                   *list;

    /* These two must be last */
    CsrUint16                    infoLen;
    CsrUint8                     *info;          /* The start of the info data */
} SdDeviceInfo_t;

typedef struct SdSearchInstanceTag
{
    struct SdSearchInstanceTag  *next;              /* Do not move - used for common linked list handling */
    struct SdSearchInstanceTag  *prev;              /* Do not move - used for common linked list handling */
    CsrUint8                     state;
    CsrUint8                     searchInstIdx;      /* Index starting from 0 - used for instFlags in SdDeviceInfo_t */
    CsrSchedTid                  searchTid;
    CsrSchedQid                  phandle;
    CsrUint32                    searchResultsCount;

    /* Parameters from CsrBtSdSearchConfigReq */
    CsrUint32                    searchConfig;
    CsrUint32                    searchResultsMax;
    CsrUint32                    readNameTimeout;

    /* Parameters from CsrBtSdSearchReq */
    CsrBtClassOfDevice           cod;
    CsrBtClassOfDevice           codMask;
    CsrUint24                    iac;
    CsrUint32                    rssiBufferTime;
    CsrUint32                    totalSearchTime;
    CsrUint32                    activeSearchConfig;
    CsrInt8                      rssiThreshold;

#ifdef CSR_BT_LE_ENABLE
    CsrInt8                      leRssiThreshold;
#endif

    struct SdMainInstanceTag    *mainInst;
} SdSearchInstance_t;

typedef struct SdMainInstanceTag
{
    void                        *recvMsgP;
    CsrSchedQid                  phandle;
    CsrUint8                     processState;
    CsrUint8                     baseband;          /* What baseband is used actively for inquiry/scan */
    CsrUint8                     appState;
    CsrUint8                     searchState;
    CsrBool                      restoreMsgFlag;
    CsrMessageQueueType         *saveQueue;
    SdDeviceInfo_t              *currentDevice;     /* Reference to the currently active device (RRN, SDP stuff etc..) */

    /* Device List & Memory related */
    CsrCmnList_t                 dl;                /* The DeviceList. Count indicates to TOTAL number of devices included paired */
    CsrUint32                    dlMax;              /* Total maximum numbers of devices allowed in the DL. Paired devices not included. */
    CsrUint32                    dlInfoCount;        /* Current number of non-paired devices with info */
    CsrUint32                    dlInfoMax;          /* Max number of devices for which to store additional info besides address and CoD - paired device excluded */
    CsrUint32                    dlPairedCount;      /* Total number of paired devices */
    CsrUint32                    dlPairedInfoCount;  /* Total number of paired device with info */
    CsrUint32                    dlCurrentAge;
    CsrUint32                    memoryConfig;

    /* Search (inquiry) related */
    CsrCmnList_t                searchInst;
    CsrUint32                   activeInstFlags[SD_DI_INST_IDX_COUNT]; /* array of bitmasks; what instance is using what feature */
    CsrBool                     nameUpdateRequired; /* One or more search instances requires updated names */
    CsrSchedTid                 searchBufferTid;
    CsrInt8                     inquiryTxPowerLevel;

    /* RRN specific */
    CsrSchedTid                  rrnTid;
    CsrUint32                    rrnActiveTime;

    /* SDP related */
    CsrBtUuid32                 *uuid32List;
    CsrUint16                    uuid32ListCount;
    CsrUint16                    uuid32ListSize;
    CsrUint32                    serviceConfig;
    void                        *sdpSearchData;
    CsrBtSdPrim                  sdpMsgType;
    CsrUint16                    sdpRecordLength;

    /* SR specific */
    CsrUint8                    *sdpRecord;
    CsrUint8                     sdpRetryCounter;
    CsrUint16                    serviceHandleListCount;
    CsrUint32                   *serviceHandleList;

    /* Low energy */
#ifdef CSR_BT_LE_ENABLE
    CsrBtGattId                  gattId; /* GATT registration ID */
#endif /* CSR_BT_LE_ENABLE */
} SdMainInstance_t;

/***** Prototypes *****/
typedef void (* SdStateHandlerType)(SdMainInstance_t *mainInst);

/* SDP search result call-back function */
/* XXX: remove task*? */
extern void CsrBtSdReadSdpResultHandler(void                       *inst,
                                        CmnCsrBtLinkedListStruct   *sdpTagList,
                                        CsrBtDeviceAddr            deviceAddr,
                                        CsrBtResultCode            resultCode,
                                        CsrBtSupplier        resultSupplier);

/* csr_bt_sd_cmsc_handlers.c */
void CsrBtSdEnvCleanupHandler(SdMainInstance_t *mainInst);
void CsrBtSdCmInquiryResultIndHandler(SdMainInstance_t *mainInst);
void CsrBtSdCmInquiryCfmHandler(SdMainInstance_t *mainInst);
void CsrBtSdCmReadRemoteNameCfmHandler(SdMainInstance_t *mainInst);
void CsrBtSdCmReadRemoteNameIndHandler(SdMainInstance_t *mainInst);

#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
void CsrBtSdCmSdsRegisterCfmHandler(SdMainInstance_t * instData);
void CsrBtSdCmSdsUnregisterCfmHandler(SdMainInstance_t * instData);
#endif
void CsrBtSdCmSetEventFilterCodCfm(SdMainInstance_t *mainInst);
void CsrBtSdCmClearEventFilterCfm(SdMainInstance_t *mainInst);

void CsrBtSdScReadDeviceRecordIndHandler(SdMainInstance_t *mainInst);
void CsrBtSdScReadDeviceRecordCfmHandler(SdMainInstance_t *mainInst);
void CsrBtSdScUpdateTrustLevelIndHandler(SdMainInstance_t *mainInst);

/* csr_bt_sd_app_handlers.c */
#ifdef CSR_BT_INSTALL_SD_MEMORY_CONFIG
void CsrBtSdMemoryConfigReqIdleState(SdMainInstance_t *mainInst);
#endif /* CSR_BT_INSTALL_SD_MEMORY_CONFIG */
#ifdef CSR_BT_INSTALL_SD_SEARCH_CONFIG
void CsrBtSdSearchConfigReqIdleState(SdMainInstance_t *mainInst);
#endif /* CSR_BT_INSTALL_SD_SEARCH_CONFIG */
void CsrBtSdReadDeviceInfoReqAllState(SdMainInstance_t *mainInst);
void CsrBtSdReadDeviceListReqAllState(SdMainInstance_t *mainInst);
void CsrBtSdSearchReqIdleState(SdMainInstance_t *mainInst);
void CsrBtSdSearchReqInqState(SdMainInstance_t *mainInst);
void CsrBtSdCancelSearchReqAllState(SdMainInstance_t *mainInst);
void CsrBtSdReadAvailableServicesReqIdleState(SdMainInstance_t * instData);
void CsrBtSdSaveReadAvailableServicesReqHandler(SdMainInstance_t *mainInst);
void CsrBtSdCancelReadAvailableServicesReqAllState(SdMainInstance_t *mainInst);
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
void CsrBtSdReadServiceRecordReqInqSave(SdMainInstance_t *mainInst);
void CsrBtSdCancelReadServiceRecordReqAllState(SdMainInstance_t *mainInst);
void CsrBtSdRegisterServiceRecordReqSave(SdMainInstance_t *mainInst);
void CsrBtSdUnregisterServiceRecordReqSave(SdMainInstance_t *mainInst);
void CsrBtSdReadServiceRecordReqIdleState(SdMainInstance_t *mainInst);
void CsrBtSdRegisterServiceRecordReqIdleState(SdMainInstance_t *mainInst);
void CsrBtSdUnregisterServiceRecordReqIdleState(SdMainInstance_t *mainInst);
void CsrBtSdReadServicesReqIdleState(SdMainInstance_t *mainInst);
void CsrBtSdReadServicesReqInqSave(SdMainInstance_t *mainInst);
void CsrBtSdReadServicesCancelReqAllState(SdMainInstance_t *mainInst);
#endif /*EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */
void CsrBtSdCleanupReqIdleState(SdMainInstance_t *mainInst);
void CsrBtSdRegisterAvailableServiceReqAllState(SdMainInstance_t *mainInst);
void CsrBtSdUpdateDeviceReqAllState(SdMainInstance_t *mainInst);
void CsrBtSdSynchronizeReqIdleState(SdMainInstance_t *mainInst);


void CsrBtSdSendReadDeviceInfoCfm(CsrSchedQid phandle, CsrBtDeviceAddr deviceAddr, SdDeviceInfo_t *deviceInfo,
                                  CsrBtResultCode resultCode, CsrBtSupplier resultSupplier,
                                  CsrBtAddressType addressType, CsrUint16 context);
void CsrBtSdSendSearchResultInd(CsrSchedQid phandle, CsrBtSdPrim type, SdDeviceInfo_t *deviceInfo);
void CsrBtSdSendCloseSearchInd(CsrSchedQid phandle, CsrBtResultCode resultCode,
                               CsrBtSupplier resultSupplier);
void CsrBtSdSendReadServiceRecordInd(CsrSchedQid phandle, CsrUint16 dataLen, CsrUint8 *data);
void CsrBtSdSendReadServiceRecordCfm(CsrSchedQid phandle, CsrBtResultCode resultCode,
                                     CsrBtSupplier resultSupplier);
void CsrBtSdSendRegisterServiceRecordCfm(CsrSchedQid phandle, CsrUint32 serviceHandle,
                                         CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtSdSendUnregisterServiceRecordCfm(CsrSchedQid phandle, CsrUint32 serviceHandle,
                                           CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtSdSendReadAvailableServicesCfmWithError(CsrSchedQid phandle, CsrBtDeviceAddr deviceAddr);
void CsrBtSdSendReadAvailableServicesCfm(SdMainInstance_t * mainInst, SdDeviceInfo_t *deviceInfo,
                                         CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtSdSendCancelReadAvailableServicesCfm(CsrSchedQid phandle);
void CsrBtSdSendReadServicesCfm(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo,
                                CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
CsrBtSdReadDeviceListInd *CsrBtSdBuildReadDeviceListInd(CsrUint32 count);
CsrBtSdReadDeviceListCfm *CsrBtSdBuildReadDeviceListCfm(CsrUint32 count, CsrUint32 total);
void CsrBtSdSendSynchronizeCfm(SdMainInstance_t *mainInst, CsrUint32 totalNumOfDevices);
#ifdef CSR_BT_INSTALL_SD_MEMORY_CONFIG
void CsrBtSdSendSdMemoryConfigCfm(SdMainInstance_t *mainInst, CsrSchedQid phandle, CsrBtResultCode resultCode,
                                  CsrBtSupplier resultSupplier);
#endif /* CSR_BT_INSTALL_SD_MEMORY_CONFIG */
#ifdef CSR_BT_INSTALL_SD_SEARCH_CONFIG
void CsrBtSdSendSdSearchConfigCfm(SdSearchInstance_t *searchInst, CsrSchedQid phandle, CsrBtResultCode resultCode,
                                  CsrBtSupplier resultSupplier);
#endif /* CSR_BT_INSTALL_SD_SEARCH_CONFIG */
void CsrBtSdSendSdCleanUpCfm(CsrSchedQid phandle, CsrUint32 mode, CsrBtResultCode resultCode,
                             CsrBtSupplier resultSupplier);

/* csr_bt_sd_util.c */
void CsrBtSdMessagePut(CsrSchedQid phandle, void *msg);
void CsrBtSdUtilReadRemoteNameTimeout(CsrUint16 dummy, void *ptr);
CsrBool CsrBtSdUtilReadRemoteNameStart(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo);
void CsrBtSdUtilReadRemoteNameStop(SdMainInstance_t *mainInst);
void CsrBtSdUtilReadRemoteNameTimeUpdate(SdMainInstance_t *mainInst);
void CsrBtSdUtilGo2Idle(SdMainInstance_t *mainInst);
void CsrBtSdUtilSaveMessage(SdMainInstance_t *mainInst);
CsrBool CsrBtSdUtilCheckSearchInstConfig(SdMainInstance_t *mainInst, CsrUint32 mask, CsrBool set, CsrBool all);
SdDeviceInfo_t *CsrBtSdUtilDLAdd(SdMainInstance_t *mainInst, CsrBtDeviceAddr *addr, CsrUint16 infoLen, CsrBool maintainOld,
                                 CsrBool *infoUpdated, CsrBool paired);
SdDeviceInfo_t *CsrBtSdUtilDLAddCmInqRes(SdMainInstance_t *mainInst, CsrBtCmInquiryResultInd *prim);

void CsrBtSdUtilDLAddDevicePropertiesType(SdMainInstance_t          *mainInst, 
                                          CsrBtDevicePropertiesType *deviceProperties, 
                                          CsrUint32                 serviceChangedHandle);

SdDeviceInfo_t *SdUtilDeviceListGetFromAddr(SdMainInstance_t *mainInst, CsrBtDeviceAddr *addr);

CsrBool CsrBtSdUtilDLGetCountFromSize(SdMainInstance_t *mainInst, SdDeviceInfo_t *currentDevice, CsrUint32 size,
                                      CsrUint32 config, CsrUint32 *count);
void CsrBtSdUtilDLCleanAll(SdMainInstance_t *mainInst);
void CsrBtSdUtilDLCleanNonPaired(SdMainInstance_t *mainInst);
void CsrBtSdUtilDLCleanPairedList(SdMainInstance_t *mainInst);
SdDeviceInfo_t *CsrBtSdUtilDLFindOldest(SdMainInstance_t *mainInst, CsrBool withInfo);
SdSearchInstance_t *CsrBtSdUtilSearchInstAdd(SdMainInstance_t *mainInst, CsrSchedQid phandle);
CsrUint32 CsrBtSdUtilGetDeviceStatus(SdDeviceInfo_t *deviceInfo);
CsrUint16 CsrBtSdUtilInfoRemoveTag(SdMainInstance_t *mainInst, SdDeviceInfo_t **deviceInfo, CsrUint16 tag);
void CsrBtSdUtilInfoUpdateTag(SdMainInstance_t *mainInst, SdDeviceInfo_t **deviceInfo, CsrUint16 tag, CsrUint16 valueLen, CsrUint8 *value);
void CsrBtSdUtilConvertEir2Info(CsrUint8 eirDataLen, CsrUint8 *eirData, CsrUint16 *infoLen, CsrUint8 *info);
CsrBool CsrBtSdUtilEirValidate(CsrUint8 *data, CsrUint16 length);
CsrUint8 CsrBtSdUtilEirCountTags(CsrUint8 eirDataLen, CsrUint8 *eirData);
CsrBool CsrBtSdUtilUuid32Check(SdMainInstance_t * mainInst, CsrBtUuid32 uuid32);
SdDeviceInfo_t *CsrBtSdUtilDLAddFromAddrOnly(SdMainInstance_t *mainInst, CsrBtDeviceAddr *addr);
void CsrBtSdUtilDICopy2DIType(SdDeviceInfo_t *deviceInfo, CsrBtSdDeviceInfoType *infoType);
void CsrBtSdUtilGenerateReadDLInd(SdMainInstance_t *mainInst, CsrUint32 size, CsrUint32 config, CsrSchedQid phandle);
void CsrBtSdUtilDLTrim(SdMainInstance_t *mainInst);
CsrBool CsrBtSdUtilSearchInstComparePhandle(CsrCmnListElm_t *elem, void *data);
CsrInt32 CsrBtSdUtilSortRssi(CsrCmnListElm_t *elem1, CsrCmnListElm_t *elem2);
CsrBool CsrBtSdUtilCompareBdaddr(CsrCmnListElm_t *elem, void *value);
void CsrBtSdUtilSearchInstDeinit(CsrCmnListElm_t *elem);
void CsrBtSdUtilDeviceInfoDeinit(CsrCmnListElm_t *elem);
void CsrBtSdUtilSearchInstUnsetInst(CsrCmnListElm_t *elem, void *data);
SdSearchInstance_t *CsrBtSdUtilSearchInstGetFromHandle(SdMainInstance_t *mainInst, CsrSchedQid phandle);
CsrBool CsrBtSdUtilIsUpdatedNameRequired(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo, CsrBool *nameAvailable);
void CsrBtSdUtilUpdateTrustedDeviceList(SdDeviceInfo_t *deviceInfo);
void CsrBtSdCmReadRemoteNameContinue(SdMainInstance_t *mainInst);
#ifdef CSR_BT_LE_ENABLE
SdDeviceInfo_t *CsrBtSdUtilDLAddCmReportRes(SdMainInstance_t *mainInst, CsrBtGattReportInd *prim);
void CsrBtSdUtilCombineTags(SdDeviceInfo_t *devInfo, CsrUint32 *length, CsrUint8 **data);
#endif
void CsrBtSdResizeDiList(SdDeviceInfo_t *devInfo, CsrUint16 newCount);
/* Generic UUID (16,32,128 bit) support */
CsrBool CsrBtSdUuidEqual(CsrBtUuid *a, CsrBtUuid *b);
void CsrBtSdUuidExpand128(CsrBtUuid *in, CsrBtUuid *out);

#ifndef SD_DONT_UPDATE_SC_TRUSTED_DEVICE_LIST
void CsrBtSdUtilInfoConvertToDeviceProperties(SdDeviceInfo_t *deviceInfo, CsrBtDevicePropertiesType *deviceProperties);
#endif

/* csr_bt_sd_sdp.c */
void CsrBtSdInsertServiceRecordHandle(SdMainInstance_t * mainInst, CsrUint32 serviceRecHandle);
CsrBool CsrBtSdGetServiceRecordHandleIndex(SdMainInstance_t * mainInst, CsrUint32 serviceRecHandle, CsrUint16 * index);
void CsrBtSdRestartServiceSearch(SdMainInstance_t * mainInst);
void CsrBtSdRestartServiceSearchFull(SdMainInstance_t * mainInst);
CsrBool CsrBtSdSdpQueueHandler(SdMainInstance_t * mainInst, CsrBtSdPrim type, CsrSchedQid phandle, CsrBool save);

/* sd_inquiry.c */
void CsrBtSdSearchTimeoutHandler(CsrUint16 dummy, void *ptr);
void CsrBtSdSearchRssiBufferTimeoutHandler(CsrUint16 dummy, void *ptr);
void CsrBtSdSearchInquiryStartFirst(SdMainInstance_t *mainInst);
void CsrBtSdSearchInquiryStart(SdMainInstance_t *mainInst);
void CsrBtSdSearchInquiryStop(SdMainInstance_t *mainInst);
void CsrBtSdSearchCompleteHandler(SdSearchInstance_t *searchInst, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);
void CsrBtSdSearchStopProcess(SdMainInstance_t * instData);
void CsrBtSdSearchBufferProcess(SdMainInstance_t *mainInst);
void CsrBtSdSearchValidateResult(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo);
CsrBool CsrBtSdSearchValidateNormalResult(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo);
void CsrBtSdSearchValidateImmResult(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo);
void CsrBtSdSearchBufferStart(SdMainInstance_t *mainInst, CsrUint32 rssiBufferTime);
CsrBool CsrBtSdSearchSetupParameters(SdMainInstance_t *mainInst, SdSearchInstance_t *searchInst, CsrBtSdSearchReq *prim);
void CsrBtSdSearchClearCodFilter(SdMainInstance_t *mainInst);
void CsrBtSdSearchSetCodFilter(SdSearchInstance_t *searchInst);
void CsrBtSdSearchReqCommonHandler(SdMainInstance_t *mainInst, CsrBtSdSearchReq *searchReq);
CsrBool CsrBtSdSearchQueueHandler(SdMainInstance_t *mainInst, CsrSchedQid phandle, CsrBool cancel);
void CsrBtSdSearchStopAllSearches(SdMainInstance_t *mainInst, CsrBtResultCode resultCode,
                                  CsrBtSupplier resultSupplier);

CsrBool CsrBtSdSearchIsGiacPresent(CsrCmnListElm_t *elem, void *data);
void CsrBtSdSearchSendAllResultsToInst(CsrCmnListElm_t *elem, void *data);

/* sd_free_down.c */
void CsrBtSdFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

/* GATT support - from csr_bt_sd_gatt.c */
#ifdef CSR_BT_LE_ENABLE
void CsrBtSdGattHandler(SdMainInstance_t *mainInst);
void CsrBtSdGattGetNameStart(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo);
void CsrBtSdGattGetNameStop(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo);
CsrBool CsrBtSdGattServiceSearchStart(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo);

#endif

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_SD_MAIN_H__ */


#ifndef CSR_BT_PHDC_MGR_MAIN_H__
#define CSR_BT_PHDC_MGR_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_types.h"
#include "csr_list.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_bt_hdp_lib.h"
#include "csr_bt_phdc_mgr_prim.h"
#include "csr_bt_phdc_mgr_genfsm.h"
#include "csr_bt_phdc_mgr_device_genfsm.h"
#include "csr_bt_phdc_ieee_asn.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AARQ_ONGOING_FALSE      0
#define AARQ_ONGOING_STARTED    1
#define AARQ_ONGOING_FAILED     2

typedef struct
{
    void            *pRecvMsg;      /* Receive message */
    CsrUint8        state;          /* Phdc mgr State */
    CsrUint16       eventClass;     /* Message class */
    CsrCmnList_t    deviceList;
    CsrCmnList_t    typeSpec;       /* Store Mdep data types during Register */
    CsrUint8        noActiveConnections;
    CsrBool         bActivated;
    CsrBool         bGetCapab;      /* When user intiated search is triggered */
    CsrSchedQid     qId;            /* Qid of the App */
}CsrBtPhdcMgrInstance;

typedef struct
{
    CsrUint8        *systemId;
    CsrUint16       devConfigId;
    CsrUint16       configStatus;
    APDU            *pSavedApdu;
    DataProto       *pDataProto; /* ptr to the received data_proto  */
    TypeVerList     *pSpec;
    CsrCmnList_t    configObjList;
}PhdcMgrMDSInfo;

typedef struct PhdcMgrDeviceListEntryTag
{
    struct PhdcMgrDeviceListEntryTag  *next;          /* Do not move - used for common linked list handling */
    struct PhdcMgrDeviceListEntryTag  *prev;          /* Do not move - used for common linked list handling */

    CsrUint8        state;                  /* Phdc mgr device state */
    BD_ADDR_T       bdAddr;                 /* Device bt address */
    CsrUint32       dataChannelId;          /* of the reliable channel */
    CsrBtMdepId     mdepId;                 /* @decide - do need to store this ?*/
    CsrUint16       maxPacketLength;        /* packet length accepted by peer */
    PhdcMgrMDSInfo  *pMds;                  /* MDS info of the peer device */
    CsrUint16       invokeIdTx;             /* */
    CsrUint16       invokeIdAck;
    CsrUint16       invokeIdRcv;
    CsrUint32       psmIdentifier;          /* of the remote service record */
    CsrBtResultCode reason;
    CsrSchedTid     timerId;                /* Timer used by 20601 */
    void            *fsmParam;              /* pointer to FsmParam copied when starting a timer.
                                               Useful for clearing memory incase the timer
                                               is cancelled */
    CsrBool         isDisUserIntiated;      /* To distinguish between user intiated or peer 
                                               intiated disconnect */
    CsrUint8        aarqOngoing;            /* Flag to insure correct response sent to app */ 
} PhdcMgrDeviceListEntry;

typedef struct PhdcMgrConfigObjectEntryTag
{
    struct PhdcMgrDeviceListEntryTag  *next;          /* Do not move - used for common linked list handling */
    struct PhdcMgrDeviceListEntryTag  *prev;          /* Do not move - used for common linked list handling */

    HANDLE          objHandle;
    CsrUint16       unitCode;
    CsrUint32       objType;
    CsrBool         useAVM;                                                    
    AttrValMap      *pAttrValMap;
    MetricIdList    *pMetricIdList;         /* NULL or set to valid pointer for object
                                               type MDC_PRESS_BLD_NONINV */

} PhdcMgrConfigObjectEntry;

typedef struct PhdcMgrDeviceDataTypeEntryTag
{
    struct PhdcMgrDeviceListEntryTag  *next;          /* Do not move - used for common linked list handling */
    struct PhdcMgrDeviceListEntryTag  *prev;          /* Do not move - used for common linked list handling */

    CsrBtMdepDataType   dataType;
    CsrUint16           deviceSpec;


} PhdcMgrDeviceDataTypeEntry;

void CsrBtPhdcMgrInstanceInit(CsrBtPhdcMgrInstance *pInst);

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /*  CSR_BT_PHDC_MGR_MAIN_H*/


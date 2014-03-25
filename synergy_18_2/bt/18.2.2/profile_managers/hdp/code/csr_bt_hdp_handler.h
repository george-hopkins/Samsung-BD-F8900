#ifndef CSR_BT_HDP_HANDLER_H__
#define CSR_BT_HDP_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_hdp_prim.h"
#include "csr_message_queue.h"
#include "csr_list.h"
#include "csr_bt_cmn_sdc_rfc_util.h"


#ifdef __cplusplus
extern "C" {
#endif

#define HDP_NUM_OF_INSTANCES        (2)


#define HDP_IDLE_STATE              (0)
#define HDP_CONNECTING_STATE        (1)
#define HDP_CONNECTED_STATE         (2)
#define HDP_DATA_STATE              (3)
#define HDP_SUSPEND_STATE           (4)
#define HDP_NUMBER_OF_STATES        (5)


#define HDP_SDP_IDLE_STATE          (0)
#define HDP_SDP_ACTIVE_STATE        (1)
#define HDP_SDP_PENDING_STATE       (2)

#define HDP_IDLE_W4_PSM             (1)
#define HDP_IDLE_W4_MDEP            (2)
#define HDP_IDLE_W4_SERVICE_HANDLE  (3)
#define HDP_IDLE_CONNECTABLE        (4)

#define HDP_PSM_INVALID             0x00FF
#define HDP_INVALID_ASSOC_CH_ID     0x0000
#define HDP_INVALID_VIRTUAL_CH_ID   0x0000
#define HDP_INVALID_INSTANCE_ID     0x0000
#define HDP_SDP_INVALID_SR_HANDLE   0x0000
#define HDP_SDP_USER_INITIATED      0x01
#define HDP_SDP_CONNECT_INITIATED   0x02

#define HDP_CTRL_TX_WINDOW_SIZE     1
#define HDP_DATA_TX_WINDOW_SIZE     5

typedef struct
{
     psm_t control;
     psm_t data;
} hdp_psm_t;

typedef struct
{
    CsrUint16              eventClass;
    CsrUint8               state;
    CsrUint8               prev_state;
    CsrUint8               aux_state;
    phandle_t              appHandle;
    CsrUint8               mdepDataTypeMask;
    CsrUint8               roleMask;
    CsrUint32              hServiceRecord;              /* Service record handler*/
    CsrUint8               sdpState;                    /* Specifies whether a SDP search is in progress or not */
    dm_security_level_t    secLevel;                    /**< security level of the device being activated */
    CsrCharString          *serviceName;                /**< name of the service ...upto 200 characters */
    CsrCharString          *serviceDescription;         /**< brief description of the service ...
                                                            upto 200 characters */
    CsrCharString          *providerName;               /**< organsiation providing the service */

    CsrUint8               dataExchangeSpecification;   /**< byte mask - Data Exchange Protocol */
    CsrTime                sniffTimeOut;                /**< Time (in ms) the MCL must be idle before requesting sniff */
    CsrUint8               numOfMdep;                   /**< number of End Points */
    CsrUint8               currentMdepId;
    CsrUint8               countNumOfMdep;
    CsrUint8               supportedProcedures;         /* byte mask - MCAP procedures supported by HDP */
    CsrCmnList_t           mdepList;
    CsrBool                connectAssociate;
    CsrBool                disconnectAssociate;
    CsrUint8               getCapab;
    CsrBool                mdlDeleted;
    CsrUint16              mdlIdList;
    CsrCmnList_t           connectionList;
    CsrUint32              hdpInstanceId;
    CsrBool                isActivated;
    CsrBool                echoTestInit;
    hdp_psm_t              psm;
    void                  *recvMsgP;
    void                  *savedMsg;
    void                  *sdpSearchData;             /* Instance data for the common SDP search lib */
}HdpInstanceData;


typedef struct
{
    void                    *recvMsgP;
    CsrUint16               eventClass;
    dm_security_level_t     secIncoming;
    dm_security_level_t     secOutgoing;
    CsrCmnList_t            IdList;
    HdpInstanceData         *instance[HDP_NUM_OF_INSTANCES];
}HdpDb;

typedef struct HdpInstanceIdListEntryTag
{
   struct HdpInstanceIdListEntryTag *next;
   struct HdpInstanceIdListEntryTag *prev;
   CsrUint16                        ctrlPsm;
   CsrUint16                        mclId;
   CsrBtDeviceAddr                  bd_addr;
   CsrUint8                         IsManager;
   CsrUint8                         instanceId;
}HdpInstanceIdListEntry;

typedef struct HdpMdepListEntryTag
{
    struct HdpMdepListEntryTag  *next;
    struct HdpMdepListEntryTag  *prev;
    CsrBtHdpMdep                 mdep;
    CsrBool                      busy;
} HdpMdepListEntry;

typedef struct HdpConnectionListEntryTag
{
    struct HdpConnectionListEntryTag  *next;
    struct HdpConnectionListEntryTag  *prev;
    CsrUint16                mclId;
    CsrUint16                mdlId;
    CsrBtMdepId              mdepId;
    CsrBtMdepDataType        mdepDataType;
    CsrBtMdepRole            mdepRole;
    CsrUint32                assocChId;
    CsrBtDeviceAddr          bd_addr;
    CsrBool                  active;
    CsrUint16                remotecontrolPsm;
    CsrUint16                remotedataPsm;
    CsrUint16                localMtu;
    CsrUint16                remoteMtu;
    CsrUint16                localDataPsm;
} HdpConnectionListEntry;

typedef void (*HdpStateHandlerType)(HdpInstanceData *pInst);

void CsrBtHdpSdcResultHandler(void                      *inst,
                              CmnCsrBtLinkedListStruct  *sdpTagList,
                              CsrBtDeviceAddr           deviceAddr,
                              CsrBtResultCode           resultCode,
                              CsrBtSupplier       resultSupplier);

void CsrBtHdpSdpRestartSearch(HdpInstanceData *pInst);

#define HDP_INSTANCE_ADD_ENTRY(IdList)  (HdpInstanceIdListEntry *)CsrCmnListElementAddLast((CsrCmnList_t *)&(IdList),sizeof(HdpInstanceIdListEntry))
#define HDP_INSTANCE_DEL_ENTRY(CsrCmnList_t,pBchsCmnListElm_t)  CsrCmnListElementRemove(&CsrCmnList_t ,(CsrCmnListElm_t *)pBchsCmnListElm_t)
#define HDP_INSTANCE_GET_FIRST(CsrCmnList_t)   (HdpInstanceIdListEntry *)CsrCmnListGetFirst(&(CsrCmnList_t))
#define HDP_INSTANCE_GET_NEXT(pCsrCmnListElm_t) pCsrCmnListElm_t = (HdpInstanceIdListEntry *)((pCsrCmnListElm_t)->next)

void hdpHandlerCmnListIdAdd(CsrCmnListElm_t *elem);
void hdpHandlerCmnListIdRemove(CsrCmnListElm_t *elem);


#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_HDP_HANDLER_H__ */

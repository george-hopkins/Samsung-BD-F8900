#ifndef CSR_BT_MCAP_HANDLER_H__
#define CSR_BT_MCAP_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_cm_prim.h"
#include "csr_message_queue.h"
#include "csr_bt_mcap_private_prim.h"
#include "csr_bt_mcap_util.h"
#include "csr_bt_cmn_linked_list.h"
#include "csr_list.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MCAP_MODE_MASK                         (L2CA_EXT_FEAT_FLOW_CONTROL + L2CA_EXT_FEAT_BIDIR_QOS + L2CA_EXT_FEAT_ENH_RETRANS + L2CA_EXT_FEAT_STREAMING + L2CA_EXT_FEAT_OPT_FCS)

#define MCAP_SERVER                             FALSE
#define MCAP_CLIENT                             TRUE

#define MCAP_REQUESTER                          FALSE
#define MCAP_RESPONDER                          TRUE

#define MCAP_NULL_STATE                         (0)
#define MCAP_IDLE_STATE                         (1)
#define MCAP_ACTIVE_STATE                       (2)
#define MCAP_DEACTIVATING_STATE                 (3)
#define MCAP_NUMBER_OF_MCAP_STATES              (4)

#define MCAP_MCL_NULL_STATE                     (100)
#define MCAP_MCL_CONNECTING_STATE               (101)
#define MCAP_MCL_CONNECTED_STATE                (102)
#define MCAP_MCL_ACTIVE_STATE                   (103)
#define MCAP_MCL_DISCONNECTING_STATE            (104)
#define MCAP_MCL_DEACTIVATING_STATE             (105)
#define MCAP_NUMBER_OF_MCL_STATES               (6)

#define MCAP_MDL_NULL_STATE                     (200)
#define MCAP_MDL_CREATING_STATE                 (201)
#define MCAP_MDL_CONNECTING_STATE               (202)
#define MCAP_MDL_ACTIVE_STATE                   (203)
#define MCAP_MDL_DELETING_STATE                 (204)
#define MCAP_MDL_RECONNECTING_STATE             (205)
#define MCAP_MDL_DISCONNECTING_STATE            (206)
#define MCAP_MDL_ABORTING_STATE                 (207)
#define MCAP_MDL_DEACTIVATING_STATE             (208)
#define MCAP_NUMBER_OF_MDL_STATES               (8)

#define MCAP_UNDEFINED_STATE                    (255) /* 0xFF */

#define MCAP_CONTROL_MTU                        L2CA_MTU_MINIMUM
/* Op codes */
#define MD_ERROR_RSP                            (0x00)
#define MD_CREATE_MDL_REQ                       (0x01)
#define MD_CREATE_MDL_RSP                       (0x02)
#define MD_RECONNECT_MDL_REQ                    (0x03)
#define MD_RECONNECT_MDL_RSP                    (0x04)
#define MD_ABORT_MDL_REQ                        (0x05)
#define MD_ABORT_MDL_RSP                        (0x06)
#define MD_DELETE_MDL_REQ                       (0x07)
#define MD_DELETE_MDL_RSP                       (0x08)
/* 0x09 - 0x10 reserved */
#define MD_SYNC_CAP_REQ                         (0x11)
#define MD_SYNC_CAP_RSP                         (0x12)
#define MD_SYNC_SET_REQ                         (0x13)
#define MD_SYNC_SET_RSP                         (0x14)
#define MD_SYNC_INFO_IND                        (0x15)
/* 0x16 - 0x20 reserved */
/* 0x21 - 0xFF reserved */

/* Op code lengths in bytes */
#define MD_ERROR_RSP_LEN                        (4)
#define MD_CREATE_MDL_REQ_LEN                   (5)
#define MD_CREATE_MDL_RSP_LEN                   (5)
#define MD_RECONNECT_MDL_REQ_LEN                (3)
#define MD_RECONNECT_MDL_RSP_LEN                (4)
#define MD_ABORT_MDL_REQ_LEN                    (3)
#define MD_ABORT_MDL_RSP_LEN                    (4)
#define MD_DELETE_MDL_REQ_LEN                   (3)
#define MD_DELETE_MDL_RSP_LEN                   (4)
#define MD_SYNC_CAP_REQ_LEN                     (3)
#define MD_SYNC_CAP_RSP_LEN                     (9)
#define MD_SYNC_SET_REQ_LEN                     (14)
#define MD_SYNC_SET_RSP_LEN                     (16)
#define MD_SYNC_INFO_IND_LEN                    (15)


typedef struct
{
    void                *recvMsgP;
    dm_security_level_t secIncoming;
    dm_security_level_t secOutgoing;
    CsrCmnList_t        mclList;                /* "Real" type is McapMclListEntry */
    CsrCmnList_t        psmList;                /* "Real" type is McapPsmListEntry */
    CsrTime            sniffTimeout;
    CsrUint8            currentLocalMode;
    CsrUint8            requestedMode;
    CsrSchedTid              timerId;
} McapInstanceData;

typedef struct McapPsmListEntryTag
{
    struct McapPsmListEntryTag  *next;          /* Do not move - used for common linked list handling */
    struct McapPsmListEntryTag  *prev;          /* Do not move - used for common linked list handling */
    psm_t                       psm;
    CsrUint8                    allowedMcls;
    CsrUint8                    currentMcls;
} McapPsmListEntry;

typedef struct McapMdlListEntryTag
{
    struct McapMdlListEntryTag  *next;          /* Do not move - used for common linked list handling */
    struct McapMdlListEntryTag  *prev;          /* Do not move - used for common linked list handling */
    CsrSchedQid                 phandle;
    CsrUint16                   mdlId;
    CsrUint8                    mdlState;
    CsrUint8                    mdepId;
    CsrUint8                    configuration;
    dm_security_level_t         secLevel;
    l2ca_mtu_t                  localMtu;
    l2ca_mtu_t                  remoteMtu;
    CsrBtConnId                 btConnId;
    L2CA_FLOW_T                 *flow;
    CsrBool                     createRole;
    CsrBool                     reconnectRole;
    CsrBool                     disconnectRole;
} McapMdlListEntry;

typedef struct McapMclListEntryTag
{
    struct McapMclListEntryTag  *next;          /* Do not move - used for common linked list handling */
    struct McapMclListEntryTag  *prev;          /* Do not move - used for common linked list handling */
    CsrCmnList_t                mdlList;        /* "Real" type is McapMdlListEntry */
    CsrBtDeviceAddr             bd_addr;
    CsrSchedQid                 phandle;
    CsrUint16                   mclId;
    CsrBtConnId                 btConnId;
    CsrUint8                    mclState;
    CsrUint8                    mclSubState;
    CsrTime                     sniffTimeout;
    psm_t                       localControlPsm;
    psm_t                       localDataPsm;
    psm_t                       remoteControlPsm;
    psm_t                       remoteDataPsm;
    dm_security_level_t         secLevel;
    l2ca_mtu_t                  mtu;
    CsrUint24                   classOfDevice;
    uuid16_t                    uuid;
    L2CA_FLOW_T                 *flow;
    CsrBool                     mclRole;
    CsrBool                     deleteRole;
    CsrUint16                   deletingMdlId;
    CsrBool                     awaitingRes;
    CsrBool                     awaitingCfm;
    CsrBool                     awaitingSyncRes;
    CsrUint8                    reqOpCode;
    CsrUint16                   reqMdlId;
} McapMclListEntry;

typedef void (*McapMclStateHandlerType)(McapInstanceData *pInst);

typedef struct
{
    McapMclListEntry    *mclPtr;
    McapMdlListEntry    *mdlPtr;
} MclMdlPtr;

typedef struct
{
    psm_t       psm;
    BD_ADDR_T   *bdAddr;
} PsmBdAddrPtr;

typedef struct
{
    psm_t   psm;
    CsrUint8 state;
} PsmStatePtr;

/* MCL List specific macros */
#define MCAP_PSM_ADD_ENTRY(list)                           ((McapPsmListEntry *)CsrCmnListElementAddFirst((CsrCmnList_t *)&(list), sizeof(McapPsmListEntry)))
#define MCAP_PSM_REMOVE_ENTRY(list, elementPtr)            (CsrCmnListElementRemove((CsrCmnList_t *)&(list), (CsrCmnListElm_t *)(elementPtr)))
#define MCAP_PSM_FIND(list, psmPtr)                        ((McapPsmListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilComparePsm, (void *)(psmPtr)))
#define MCAP_PSM_GET_FIRST(list)                           ((McapPsmListEntry *)CsrCmnListGetFirst(&(list)))

/* MCL List specific macros */
#define MCAP_MCL_ADD_ENTRY(list)                           ((McapMclListEntry *)CsrCmnListElementAddFirst((CsrCmnList_t *)&(list), sizeof(McapMclListEntry)))
#define MCAP_MCL_REMOVE_ENTRY(list, elementPtr)            (CsrCmnListElementRemove((CsrCmnList_t *)&(list), (CsrCmnListElm_t *)(elementPtr)))
#define MCAP_MCL_GET_FIRST(list)                           ((McapMclListEntry *)CsrCmnListGetFirst(&(list)))
#define MCAP_MCL_GET_LAST(list)                            ((McapMclListEntry *)CsrCmnListGetLast(&(list)))
#define MCAP_MCL_FIND_FROM_LOCAL_CONTROL_PSM(list, psmPtr) ((McapMclListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilCompareLocalControlPsm, (void *)(psmPtr)))
#define MCAP_MCL_FIND_FROM_LOCAL_CONTROL_PSM_AND_MCL_STATE(list, dataPtr) ((McapMclListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilCompareLocalControlPsmAndMclState, (void *)(dataPtr)))
#define MCAP_MCL_FIND_FROM_LOCAL_CONTROL_PSM_AND_BDADDR(list, searchDataPtr)    ((McapMclListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilCompareLocalControlPsmAndBdAddr, (void *)(searchDataPtr)))
#define MCAP_MCL_FIND_FROM_LOCAL_DATA_PSM(list, psmPtr)    ((McapMclListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilCompareLocalDataPsm, (void *)(psmPtr)))
#define MCAP_MCL_FIND_FROM_LOCAL_DATA_PSM_AND_BDADDR(list, dataPtr)    ((McapMclListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilCompareLocalDataPsmAndBdAddr, (void *)(dataPtr)))
#define MCAP_MCL_FIND_FROM_REMOTE_CONTROL_PSM(list, psmPtr)((McapMclListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilCompareRemoteControlPsm, (void *)(psmPtr)))
#define MCAP_MCL_FIND_FROM_REMOTE_DATA_PSM(list, psmPtr)   ((McapMclListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilCompareRemoteDataPsm, (void *)(psmPtr)))
#define MCAP_MCL_FIND_FROM_MCL_STATE(list, mclStatePtr)    ((McapMclListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilCompareMclState, (void *)(mclStatePtr)))
#define MCAP_MCL_FIND_FROM_BDADDR(list, bdAddrPtr)         ((McapMclListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilCompareBdAddr, (void *)(bdAddrPtr)))
#define MCAP_MCL_FIND_FROM_MCLID(list, mclIdPtr)           ((McapMclListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilCompareMclId, (void *)(mclIdPtr)))
#define MCAP_MCL_COUNT(list)                               (CsrCmnListGetCount(&(list)))

/* MDL List specific macros */
#define MCAP_MDL_ADD_ENTRY(list)                           ((McapMdlListEntry *)CsrCmnListElementAddFirst((CsrCmnList_t *)&(list), sizeof(McapMdlListEntry)))
#define MCAP_MDL_REMOVE_ENTRY(list, instPtr)               (CsrCmnListElementRemove((CsrCmnList_t *)&(list), (CsrCmnListElm_t *)(instPtr)))
#define MCAP_MDL_GET_FIRST(list)                           ((McapMdlListEntry *)CsrCmnListGetFirst(&(list)))
#define MCAP_MDL_GET_LAST(list)                            ((McapMdlListEntry *)CsrCmnListGetLast(&(list)))
#define MCAP_MDL_FIND_FROM_MDLID(list, mdlIdPtr)           ((McapMdlListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilCompareMdlId, (void *)(mdlIdPtr)))
#define MCAP_MDL_FIND_FROM_MDL_STATE(list, statePtr)       ((McapMdlListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilCompareMdlState, (void *)(statePtr)))
#define MCAP_MDL_FIND_FROM_CID(list, cidPtr)               ((McapMdlListEntry *)CsrCmnListSearch(&(list), CsrBtMcapUtilCompareMdlCid, (void *)(cidPtr)))
#define MCAP_MDL_ITERATE(list, func, dataPtr)              (CsrCmnListIterate(&(list), (func), (void *)(dataPtr)))
#define MCAP_MDL_COUNT(list)                               (CsrCmnListGetCount(&(list)))


#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_MCAP_HANDLER_H__ */

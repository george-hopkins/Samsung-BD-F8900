#ifndef CSR_BT_HCI_CM_PRIVATE_H__
#define CSR_BT_HCI_CM_PRIVATE_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_types.h"
#include "csr_sched.h"
#include "csr_list.h"
#include "csr_bt_hci_cm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_HCI_CM_CMD_TYPE_PENDING              0x00
#define CSR_BT_HCI_CM_CMD_TYPE_ALWAYS_ADD           0x01
#define CSR_BT_HCI_CM_CMD_TYPE_ALWAYS_REMOVE        0x02
#define CSR_BT_HCI_CM_CMD_TYPE_NEVER                0x03
#define CSR_BT_HCI_CM_INVAID_HCI_CONNECTION_HANDLE  0xFFFF

#define CSR_BT_HCI_CM_STATE_NORMAL                  0x00
#define CSR_BT_HCI_CM_STATE_RESETTING               0x01
#define CSR_BT_HCI_CM_REPLAYING                     0x02

#define CSR_BT_HCI_CM_CMD_STATE_IDLE                0x00
#define CSR_BT_HCI_CM_CMD_STATE_PENDING             0x01

typedef struct CsrBtHciCmCommonHciCmdListTypeTag
{
    struct CsrBtHciCmCommonHciCmdListTypeTag *next; /* must be 1st */
    struct CsrBtHciCmCommonHciCmdListTypeTag *prev; /* must be 2nd */
    CsrUint32 cmdId; 
    CsrUint32 secondaryCmdId; 
    CsrUint8 cmdType;
    CsrUint16 cmdSize;
    void *cmdMsg;
    void *cmdArg;
    CsrUint8 cmdState;    
    CsrBtHciCmEventStatusType cmdStatusEventHandler;
    CsrBtHciCmEventCompleteType cmdCompleteEventHandler;
    CsrBtHciCmFreeType cmdArgFreeFunc;
    CsrBtHciCmMatchCompareType cmdMatchFunc;
} CsrBtHciCmCommonHciCmdListType;

typedef struct CsrBtHciCmAclConnHandleListTypeTag
{
    struct CsrBtHciCmAclConnHandleListTypeTag *next; /* must be 1st */
    struct CsrBtHciCmAclConnHandleListTypeTag *prev; /* must be 2nd */
    hci_connection_handle_t connHandle;  
} CsrBtHciCmAclConnHandleListType;

typedef struct
{
    CsrCmnList_t hciCmdPendingList;
    CsrCmnList_t hciDeviceSetupList;
    CsrCmnList_t aclConnHandleList;
    CsrUint8 state;
    CsrUint8 numHciCommandPackets;
    CsrBool readingFromHciDeviceSetupList;
    CsrUint16 hciDeviceSetupListIndex;  
    CsrSchedQid appHandle;  
} CsrBtHciCmInstDataType;

typedef struct
{
    CsrUint32 cmdId;
    const void *eventArg;
    CsrBool cmdComplete;
} CsrBtHciCmCompareType;


/* aclConnHandleList specific macros        */
#define CSR_BT_HCI_CM_ADD_CONN_HANDLE_LIST() (CsrBtHciCmAclConnHandleListType *)CsrCmnListElementAddLast(&bcCmInstData->aclConnHandleList, sizeof(CsrBtHciCmAclConnHandleListType))
#define CSR_BT_HCI_CM_SEARCH_CONN_HANDLE_LIST(connHandlePtr) ((CsrBtHciCmAclConnHandleListType *)CsrCmnListSearch(&(bcCmInstData->aclConnHandleList), csrBtHciCmCompareHciConnHandle, (void *)(connHandlePtr)))
#define CSR_BT_HCI_CM_REMOVE_CONN_HANDLE_LIST(elementPtr) (CsrCmnListElementRemove((CsrCmnList_t *)&(bcCmInstData->aclConnHandleList), (CsrCmnListElm_t *)(elementPtr)))
#define CSR_BT_HCI_CM_ITERATE_CONN_HANDLE_LIST() (CsrCmnListIterate(&(bcCmInstData->aclConnHandleList), csrBtHciCmEventDisconnectionCompleteSend, NULL))

/* hciCmdPendingList specific macros        */
#define CSR_BT_HCI_CM_GET_FIRST_CMD_FROM_PENDING_LIST() (CsrBtHciCmCommonHciCmdListType *) CsrCmnListElementGetFirst(&bcCmInstData->hciCmdPendingList)
#define CSR_BT_HCI_CM_ADD_CMD_TO_PENDING_LIST() (CsrBtHciCmCommonHciCmdListType *)CsrCmnListElementAddLast(&bcCmInstData->hciCmdPendingList, sizeof(CsrBtHciCmCommonHciCmdListType))
#define CSR_BT_HCI_CM_REMOVE_CMD_FROM_PENDING_LIST(elementPtr) (CsrCmnListElementRemove((CsrCmnList_t *)&(bcCmInstData->hciCmdPendingList), (CsrCmnListElm_t *)(elementPtr)))
#define CSR_BT_HCI_CM_FIND_CMD_ON_PENDING_LIST(cmdIdPtr) ((CsrBtHciCmCommonHciCmdListType *)CsrCmnListSearch(&(bcCmInstData->hciCmdPendingList), csrBtHciCmCompareHciCmdId, (void *)(cmdIdPtr)))
#define CSR_BT_HCI_CM_FREE_ALL_CMD_ON_PENDING_LIST() (CsrCmnListIterate(&(bcCmInstData->hciCmdPendingList), csrBtHciCmFreeCmdResendElementExt, NULL))
#define CSR_BT_HCI_CM_FIND_SECONDARY_CMD_ON_PENDING_LIST() ((CsrBtHciCmCommonHciCmdListType *)CsrCmnListSearch(&(bcCmInstData->hciCmdPendingList), csrBtHciCmFindSecondaryHciCmdId, NULL))
#define CSR_BT_HCI_CM_FIND_AND_MATCH_CMD_ON_PENDING_LIST(cmdIdPtr) ((CsrBtHciCmCommonHciCmdListType *)CsrCmnListSearch(&(bcCmInstData->hciCmdPendingList), csrBtHciCmCompareHciCmdIdExt, (void *)(cmdIdPtr)))
#define CSR_BT_HCI_CM_SET_CMD_STATE_TO_IDLE_ON_PENDING_LIST() (CsrCmnListIterate(&(bcCmInstData->hciCmdPendingList), csrBtHciCmSetHciCmdStateIdle, NULL))
#define CSR_BT_HCI_CM_FIND_CMD_IN_IDLE_STATE_ON_PENDING_LIST() ((CsrBtHciCmCommonHciCmdListType *)CsrCmnListSearch(&(bcCmInstData->hciCmdPendingList), csrBtHciCmFindHciCmdInStateIdle, NULL))

/* hciDeviceSetupList specific macros       */
#define CSR_BT_HCI_CM_GET_CMD_FROM_INDEX_DEVICE_SETUP_LIST(index) (CsrBtHciCmCommonHciCmdListType *) CsrCmnListGetFromIndex(&bcCmInstData->hciDeviceSetupList, index)
#define CSR_BT_HCI_CM_ADD_CMD_TO_DEVICE_SETUP_LIST() (CsrBtHciCmCommonHciCmdListType *)CsrCmnListElementAddLast(&bcCmInstData->hciDeviceSetupList, sizeof(CsrBtHciCmCommonHciCmdListType))
#define CSR_BT_HCI_CM_REMOVE_CMD_FROM_DEVICE_SETUP_LIST(elementPtr) (CsrCmnListElementRemove((CsrCmnList_t *)&(bcCmInstData->hciDeviceSetupList), (CsrCmnListElm_t *)(elementPtr)))
#define CSR_BT_HCI_CM_FIND_CMD_ON_DEVICE_SETUP_LIST(cmdIdPtr) ((CsrBtHciCmCommonHciCmdListType *)CsrCmnListSearch(&(bcCmInstData->hciDeviceSetupList), csrBtHciCmCompareHciCmdId, (void *)(cmdIdPtr)))
#define CSR_BT_HCI_CM_FREE_ALL_CMD_ON_DEVICE_SETUP_LIST() (CsrCmnListIterate(&(bcCmInstData->hciDeviceSetupList), csrBtHciCmFreeCmdResendElementExt, NULL))

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_HCI_CM_PRIVATE__H__ */

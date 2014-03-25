/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_hci_cm_private.h"
#include "csr_bt_hcishim_private.h"
#include "csr_pmem.h"
#include "csr_hci_lib.h"
#include "dm_prim.h"
#include "dm_hci_interface.h"
#include "csr_tm_bluecore_lib.h"

#ifdef CSR_CHIP_MANAGER_ENABLE
#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
#include "csr_bc_chip_manager_test.h"
#include "csr_transport.h"
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */

static CsrBtHciCmInstDataType *bcCmInstData = NULL;

static void csrBtHciCmEventDisconnectionCompleteSend(CsrCmnListElm_t *elem, void *data);
static CsrBool csrBtHciCmCompareHciCmdId(CsrCmnListElm_t *elem, void *value);
static CsrBool csrBtHciCmCompareHciCmdIdExt(CsrCmnListElm_t *elem, void *value);
static CsrBool csrBtHciCmFindSecondaryHciCmdId(CsrCmnListElm_t *elem, void *value);
static CsrBool csrBtHciCmFindHciCmdInStateIdle(CsrCmnListElm_t *elem, void *value);

static void csrBtHciCmFreeCmdResendElement(CsrCmnListElm_t *elem)
{ /* Deallocate private pointer before the element is removed from either the 
     hciCmdPendingList */
    CsrBtHciCmCommonHciCmdListType *element = (CsrBtHciCmCommonHciCmdListType *)elem;
    
    if (element->cmdMsg)
    {
        CsrPmemFree(element->cmdMsg);
        element->cmdMsg = NULL;
    }

    if (element->cmdArgFreeFunc)
    {
        element->cmdArgFreeFunc(element->cmdArg);      
        element->cmdArgFreeFunc = NULL;
    }
}

static void csrBtHciCmFreeCmdResendElementExt(CsrCmnListElm_t *elem, void *data)
{ /* Deallocate private pointer before the element is removed from either the 
     hciCmdPendingList */
    csrBtHciCmFreeCmdResendElement(elem);
    CSR_UNUSED(data);
}


static void csrBtHciCmHciEventDisconnectionCompleteSendHandler(void)
{ /* The BlueCore Chip has been reset. Iterate thourgh the ACL connection 
     handle list in order to make sure that the DM receives a 
     HCI_EV_DISCONNECT_COMPLETE event for each ACL handle */  
    if (bcCmInstData)
    {
        CSR_BT_HCI_CM_ITERATE_CONN_HANDLE_LIST();
        CsrCmnListDeinit(&bcCmInstData->aclConnHandleList);
    }
}

static CsrBool csrBtHciCmHciCmdHandler(void *cmdMsg,
                                       CsrUint16 cmdSize,
                                       void *cmdArg,
                                       CsrUint8 cmdType,
                                       CsrUint32 cmdId, 
                                       CsrUint32 secodaryCmdId, 
                                       CsrBtHciCmEventStatusType cmdStatusEventHandler,
                                       CsrBtHciCmEventCompleteType cmdCompleteEventHandler,
                                       CsrBtHciCmFreeType cmdArgFreeFunc,
                                       CsrBtHciCmMatchCompareType cmdMatchFunc)
{ /* A HCI command is send from the host to the BlueCore Chip. Add this 
     command to the hciCmdPendingList */
    if (bcCmInstData)
    {
        CsrBtHciCmCommonHciCmdListType *element = CSR_BT_HCI_CM_ADD_CMD_TO_PENDING_LIST();
        element->cmdType = cmdType;
        element->cmdId = cmdId;
        element->secondaryCmdId = secodaryCmdId;
        element->cmdSize = cmdSize;
        element->cmdArg = cmdArg;
        element->cmdStatusEventHandler = cmdStatusEventHandler;
        element->cmdCompleteEventHandler = cmdCompleteEventHandler;
        element->cmdArgFreeFunc = cmdArgFreeFunc;
        element->cmdMatchFunc = cmdMatchFunc;
        element->cmdMsg = NULL;
        element->cmdState = CSR_BT_HCI_CM_CMD_STATE_PENDING;
    
        if (cmdMsg)
        { /* Take a copy of the HCI command as the chip manager just be able to 
             resend it */
            element->cmdMsg = CsrPmemAlloc(cmdSize);
            CsrMemCpy(element->cmdMsg, cmdMsg, cmdSize);
        }

        if (bcCmInstData->state != CSR_BT_HCI_CM_STATE_NORMAL)
        { /* The Controller has been reset. Return FALSE to indicate that this
             HCI command shall not be send to the Controller */
            element->cmdState = CSR_BT_HCI_CM_CMD_STATE_IDLE;
            return (FALSE);
        }
    }

#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
    CsrBcCmTestCmdActivityHandler(TRANSPORT_CHANNEL_HCI, 
                                  cmdId, 
                                  CSR_BC_CM_TEST_TYPE_CMD, 
                                  (void*)NULL);
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */

    /* Return TRUE to indicate that this HCI command must be send to the 
       Controller */
    return (TRUE);
}

static void csrBtHciCmSendHciCommand(CsrBtHciCmCommonHciCmdListType *element)
{ /* Sends a HCI command message which has been restored from the 
     hciDeviceSetupList or the hciCmdPendingList during CSR_BT_HCI_CM_REPLAYING 
     state */
    HCI_COMMAND_COMMON_T *convPrim  = (HCI_COMMAND_COMMON_T *)element->cmdMsg;
    CsrUint16 cmdLen                = convPrim->length + HCI_COMMAND_HEADER_SIZE;
    void *data                      = CsrPmemAlloc(element->cmdSize);
    CsrMemCpy(data, element->cmdMsg, element->cmdSize);
    CsrHciCommandReqSend(cmdLen, (CsrUint8*)data);
}

static void csrBtHciCmRemoveHciSecondaryCmd(CsrUint32 secondaryCmdId)
{ /* The HCI event which remove the current HCI command must also remove 
     another HCI command. Check if this 'secondary' HCI command still is 
     placed on the hciCmdPendingList */
    if (bcCmInstData)
    {

        CsrBtHciCmCommonHciCmdListType *tmpElement = CSR_BT_HCI_CM_FIND_CMD_ON_PENDING_LIST(&secondaryCmdId);

        if (tmpElement)
        { /* The 'secondary' HCI command is found on the hciCmdPendingList, 
            remove it */ 
            CSR_BT_HCI_CM_REMOVE_CMD_FROM_PENDING_LIST(tmpElement);
        }
    }
}

static void csrBtHciCmRemoveHciCmdFromResendPendingList(CsrBtHciCmCommonHciCmdListType *element)
{ /* Remove the HCI command from the hciCmdPendingList */
    if (element->secondaryCmdId != CSR_BT_HCI_CM_INVALID_HCI_CMD_IF)
    { /* The HCI event which remove the current HCI command must also remove 
         another HCI command. Check if this 'secondary' HCI command still is 
         placed on the hciCmdPendingList */
        csrBtHciCmRemoveHciSecondaryCmd(element->secondaryCmdId);
    }
    if (bcCmInstData)
    {
        CSR_BT_HCI_CM_REMOVE_CMD_FROM_PENDING_LIST(element);
    }
}

static void csrBtHciCmRemoveSecondaryBeforeReplayPendingList(void)
{
    if (bcCmInstData)
    {
        CsrBtHciCmCommonHciCmdListType *element = CSR_BT_HCI_CM_FIND_SECONDARY_CMD_ON_PENDING_LIST();

        while (element)
        {
            csrBtHciCmRemoveHciSecondaryCmd(element->secondaryCmdId);
            element->secondaryCmdId = CSR_BT_HCI_CM_INVALID_HCI_CMD_IF;
            element = CSR_BT_HCI_CM_FIND_SECONDARY_CMD_ON_PENDING_LIST();
        }
    }
}

static void csrBtHciCmReplayOrEmulateHciCmd(CsrBtHciCmCommonHciCmdListType *element)
{ /* A HCI command is found on the hciDeviceSetupList or on the 
     hciPendingList. Check if this HCI command must be replayed or 
     emulated */
    if (element->cmdType == CSR_BT_HCI_CM_CMD_TYPE_NEVER)
    { /* The type of the HCI command is CSR_BT_HCI_CM_CMD_TYPE_NEVER.
         E.g. this type must not be resend to the Controller, instead
         the HCI event messages that were missing is emulate by calling 
         the registered callback function. Note this message can only be
         restored from the hciCmdPendingList */

        if (element->cmdStatusEventHandler)
        { /* The host have not received a HCI status event for the given
             HCI command. Emulate the HCI status event */               
            element->cmdStatusEventHandler(0, HCI_SUCCESS);
#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
            CsrBcCmTestCmdActivityHandler(TRANSPORT_CHANNEL_HCI, 
                                          element->cmdId, 
                                          CSR_BC_CM_TEST_TYPE_FAKE_STATUS, 
                                          (void*) NULL);
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */
        }

        if (element->cmdCompleteEventHandler)
        { /* The host have not received a HCI complete event for the 
             given HCI command. Emulate the HCI complete event */
            element->cmdCompleteEventHandler(element->cmdArg, 0);
#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
            CsrBcCmTestCmdActivityHandler(TRANSPORT_CHANNEL_HCI, 
                                          element->cmdId, 
                                          CSR_BC_CM_TEST_TYPE_FAKE_COMPLETE, 
                                          (void*) NULL);
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */
        }

        /* Remove the HCI command from the hciCmdPendingList */
        csrBtHciCmRemoveHciCmdFromResendPendingList(element);
    }
    else
    { /* Send the HCI command to the controller, as the command type must be 
         CSR_BT_HCI_CM_CMD_TYPE_PENDING or CSR_BT_HCI_CM_CMD_TYPE_ALWAYS */ 
        if (bcCmInstData)
        {
            bcCmInstData->numHciCommandPackets--;
        }
        element->cmdState = CSR_BT_HCI_CM_CMD_STATE_PENDING;
        csrBtHciCmSendHciCommand(element);
#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
        CsrBcCmTestCmdActivityHandler(TRANSPORT_CHANNEL_HCI, 
                                      element->cmdId, 
                                      CSR_BC_CM_TEST_TYPE_CMD, 
                                      (void*) NULL);
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */
    }
}

static CsrBool csrBtHciCmReplayHciCmdHandler(void)
{ 
    if (bcCmInstData)
    {
        while (bcCmInstData->numHciCommandPackets > 0)
        { /* The host is allow to send a HCI command to the controller */
            CsrBtHciCmCommonHciCmdListType *element;

            if (bcCmInstData->readingFromHciDeviceSetupList)
            { /* Check if there is more HCI commands on the hciDeviceSetupList that 
                 needs to be replayed */
                element = CSR_BT_HCI_CM_GET_CMD_FROM_INDEX_DEVICE_SETUP_LIST(bcCmInstData->hciDeviceSetupListIndex);
            
                if (element)
                { /* There are still HCI commands on the hciDeviceSetupList that
                     needs to be replayed */
                    bcCmInstData->hciDeviceSetupListIndex++;
                    csrBtHciCmReplayOrEmulateHciCmd(element);
                }
                else
                { /* All HCI commands on the hciDeviceSetupList has been replayed. 
                     Check the hciCmdPendingList as the hciDeviceSetupList is 
                     always replayed first */
                    bcCmInstData->readingFromHciDeviceSetupList = FALSE;
                    csrBtHciCmRemoveSecondaryBeforeReplayPendingList();
                }
            }
            else
            { /* Check if there is more HCI commands on the hciCmdPendingList that 
                 needs to be replayed */
                element = CSR_BT_HCI_CM_FIND_CMD_IN_IDLE_STATE_ON_PENDING_LIST();

                if (element)
                { /* There are still HCI commands on the hciCmdPendingList that
                     needs to be replayed or emulated */
                    csrBtHciCmReplayOrEmulateHciCmd(element);
                }
                else
                { /* All the HCI commands on the hciCmdPendingList has been or is 
                     being played. This function must only return TRUE when 
                     when the list is empty. e.g no HCI commands is being replayed/
                     pending */
                    if (CSR_BT_HCI_CM_GET_FIRST_CMD_FROM_PENDING_LIST() == NULL)
                    { /* There are no more HCI commands on the hciCmdPendingList 
                        return TRUE to indicate that all HCI commands has been 
                        replayed or emulated */
                        return (TRUE);
                    }
                    else
                    { /* There are still HCI commands on the hciCmdPendingList 
                         which is being replayed. Return FALSE to go out of 
                         while loop and to indicate that the replaying process
                         is still running */
                        return (FALSE);
                    }
                }
            }
        }
    }
    
    /* Return FALSE to indicate that there are still some HCI commands on the 
       hciDeviceSetupList or the hciCmdPendingList that either must be replayed 
       or emulated */      
    return (FALSE);
}

static void csrBtHciCmSendHciNop(void)
{ /* Generate a HCI NOP (No OPeration) in order to tell the DM how many HCI 
     command packets it is allow to send to the BlueCore Chip */
    HCI_EV_COMMAND_STATUS_T evt;
    evt.event.event_code = HCI_EV_COMMAND_STATUS; 
    evt.event.length = HCI_EV_COMMAND_STATUS_PARAM_LEN;
    evt.status = HCI_SUCCESS;
    if (bcCmInstData)
    {
        evt.num_hci_command_pkts = bcCmInstData->numHciCommandPackets;
    }
    else
    {
        evt.num_hci_command_pkts = 0;
    }
    evt.op_code = HCI_NOP;  

    dm_hci_event_handler((HCI_UPRIM_T*)&evt);
}

static void csrBtHciCmResetToNormalHandler(void)
{ /* Checks if a HCI NOP (No OPeration) event must be send to the DM and go to 
     CSR_BT_HCI_CM_STATE_NORMAL state as all commands (HCI and Bccmd) has been 
     replayed or emulated */
    if (bcCmInstData)
    {
        bcCmInstData->state = CSR_BT_HCI_CM_STATE_NORMAL;

        if (bcCmInstData->numHciCommandPackets > 0)
        { /* Generate a NOP (No OPeration) */
            csrBtHciCmSendHciNop();
            bcCmInstData->numHciCommandPackets = 0;
        }
        CsrTmBlueCoreCmReplayStartResSend(bcCmInstData->appHandle, CSR_RESULT_SUCCESS);
    }
}

static void csrBtHciCmMoveHciCmdToResendAlwaysList(CsrBtHciCmCommonHciCmdListType *cmd)
{ /* Move the HCI command from the hciCmdPendingList to the hciDeviceSetupList
     and check for duplicates. E.g. if a HCI command og type 
     CSR_BT_HCI_CM_CMD_TYPE_ALWAYS is send twice only the latest is stored */
    if (bcCmInstData)
    {
        CsrBtHciCmCommonHciCmdListType *existingCmd, *newCmd, tmp;

        existingCmd = CSR_BT_HCI_CM_FIND_CMD_ON_DEVICE_SETUP_LIST(&(cmd->cmdId));
        if (existingCmd)
        { /* A duplicate is found remove it */
            CSR_BT_HCI_CM_REMOVE_CMD_FROM_DEVICE_SETUP_LIST(existingCmd);
        }
    
        /* Move the HCI command from hciCmdPendingList to the hciDeviceSetupList */
        newCmd       = CSR_BT_HCI_CM_ADD_CMD_TO_DEVICE_SETUP_LIST();
        tmp          = *newCmd;
        *newCmd      = *cmd; 
        newCmd->next = tmp.next;
        newCmd->prev = tmp.prev;

        /* Make sure that cmd->cmdMsg is not deallocated when the command is remove 
           from the hciCmdPendingList */     
        cmd->cmdMsg  = NULL;
    
        CSR_BT_HCI_CM_REMOVE_CMD_FROM_PENDING_LIST(cmd);
    }
    
}

static void csrBtHciCmRemoveHciCmdFromResendAlwaysList(CsrBtHciCmCommonHciCmdListType *cmd)
{ /* Move the HCI command from the hciCmdPendingList and the hciDeviceSetupList */
    if (bcCmInstData)
    {
        CsrBtHciCmCommonHciCmdListType *existingCmd;

        existingCmd = CSR_BT_HCI_CM_FIND_CMD_ON_DEVICE_SETUP_LIST(&(cmd->cmdId));
        if (existingCmd)
        { /* A duplicate is found remove it */
            CSR_BT_HCI_CM_REMOVE_CMD_FROM_DEVICE_SETUP_LIST(existingCmd);
        }

        CSR_BT_HCI_CM_REMOVE_CMD_FROM_PENDING_LIST(cmd);
    }

}

#ifdef CSR_BT_LE_ENABLE
static void csrBtHciCmRemoveHciCmdIdFromResendAlwaysList(CsrUint32 cmdId)
{ /* Move the HCI command from the hciCmdPendingList and the hciDeviceSetupList */
    if (bcCmInstData)
    {
        CsrBtHciCmCommonHciCmdListType *existingCmd;

        existingCmd = CSR_BT_HCI_CM_FIND_CMD_ON_DEVICE_SETUP_LIST(&cmdId);
        if (existingCmd)
        { /* A duplicate is found remove it */
            CSR_BT_HCI_CM_REMOVE_CMD_FROM_DEVICE_SETUP_LIST(existingCmd);
        }
    }

}
#endif

static void csrBtHciCmSetHciCmdStatusReceived(CsrBtHciCmCommonHciCmdListType *cmd)
{ /* A successful Hci Status Event is received. Set the status callback funtion 
     to NULL in order to ensure that this Event is not emulate later */
    cmd->cmdStatusEventHandler = NULL;
}

static void csrBtHciCmSetHciCmdState(CsrBtHciCmCommonHciCmdListType *cmd,
                                  CsrUint32                    cmdId, 
                                  hci_return_t                hciStatus, 
                                  CsrBool                      cmdComplete)
{ /* A Hci event is received and found on the hciCmdPendingList */
    if (cmdComplete && hciStatus == HCI_SUCCESS &&
         (cmd->cmdType == CSR_BT_HCI_CM_CMD_TYPE_ALWAYS_ADD ||
          cmd->cmdType == CSR_BT_HCI_CM_CMD_TYPE_ALWAYS_REMOVE))
    {
        if (cmd->cmdType == CSR_BT_HCI_CM_CMD_TYPE_ALWAYS_ADD)
        {
            /* The HCI command is complete with success. Move the Hci command to the
             hciDeviceSetupList because the cmdType is
             CSR_BT_HCI_CM_CMD_TYPE_ALWAYS*/
            csrBtHciCmMoveHciCmdToResendAlwaysList(cmd);
        }
        else if (cmd->cmdType == CSR_BT_HCI_CM_CMD_TYPE_ALWAYS_REMOVE)
        {
            csrBtHciCmRemoveHciCmdFromResendAlwaysList(cmd);
        }
    }    
    else if (!cmdComplete && hciStatus == HCI_SUCCESS)
    { /* A Hci Status Event is received with success */
        csrBtHciCmSetHciCmdStatusReceived(cmd);
    }
    else
    { /* A Hci Status or Complete Event is received eith Error. Remove the HCI 
         command from the hciCmdPendingList */
        csrBtHciCmRemoveHciCmdFromResendPendingList(cmd);
    }
}

static CsrBtHciCmCommonHciCmdListType *csrBtHciCmFindMatchingHciCmdEvent(CsrUint32 cmdId,
                                                                         const void *eventArg,
                                                                         CsrBool cmdComplete)
{ /* A Hci event is received. Check if this event match one of the stored 
     HCI commands */
    CsrBtHciCmCommonHciCmdListType *cmd = NULL;    
    if (bcCmInstData)
    {
        if (bcCmInstData->state == CSR_BT_HCI_CM_REPLAYING && 
            bcCmInstData->readingFromHciDeviceSetupList)
        { /* The Hci command which trigger the incoming Hci event were 
             replayed from the hciDeviceSetupList */
            cmd = CSR_BT_HCI_CM_FIND_CMD_ON_DEVICE_SETUP_LIST(&(cmdId));
        }
        else
        {/* Check if the received Hci event match one of the Hci command stored 
            on the  hciCmdPendingList */
            CsrBtHciCmCompareType compareVar;
            compareVar.cmdId = cmdId;
            compareVar.eventArg = eventArg;
            compareVar.cmdComplete = cmdComplete;  
            cmd = CSR_BT_HCI_CM_FIND_AND_MATCH_CMD_ON_PENDING_LIST(&(compareVar));
        }
    }
    return (cmd);
}

static void csrBtHciCmUpdateHciCredits(CsrUint8 *numHciCommandPackets)
{ /* Check if the HCI credits need to be updated */
    if ((numHciCommandPackets) && (bcCmInstData != NULL))
    { /* The Hci event includes Hci credits. Update credits */
        bcCmInstData->numHciCommandPackets = *numHciCommandPackets;
    }
}

static CsrBool csrBtHciCmCompareHciConnHandle(CsrCmnListElm_t *elem, void *value)
{ /* This check if the connection handle receive in a Hci 
     DisconnectionComplete Event match one of the connection handles
     store in the aclConnHandleList */
    hci_connection_handle_t connHandle = *(hci_connection_handle_t *)value;
    CsrBtHciCmAclConnHandleListType *element = (CsrBtHciCmAclConnHandleListType *)elem;

    if (element->connHandle == connHandle)
    { /* Return True to indicate that a matching connection handle is found */
        return (TRUE);
    }
    return (FALSE);
}

static void csrBtHciCmEventDisconnectionCompleteSend(CsrCmnListElm_t *elem, void *data)
{ /* The BlueCore Chip has been reset check if the DM had any ACL connection 
     before the reset occured */
    CsrBtHciCmAclConnHandleListType *element = (CsrBtHciCmAclConnHandleListType *)elem;
    
    if (element->connHandle != CSR_BT_HCI_CM_INVAID_HCI_CONNECTION_HANDLE)
    { /* An ACL were connected, send a Hci Disconnection Complete Event to the 
         DM */
        HCI_EV_DISCONNECT_COMPLETE_T *prim = CsrPmemAlloc(sizeof(HCI_EV_DISCONNECT_COMPLETE_T));
        prim->event.event_code = HCI_EV_DISCONNECT_COMPLETE; 
        prim->event.length = HCI_EV_DISCONNECT_COMPLETE_PARAM_LEN;
        prim->status = HCI_SUCCESS;
        prim->handle = element->connHandle;
        prim->reason = HCI_ERROR_CONN_TIMEOUT;
        dm_hci_event_handler((HCI_UPRIM_T*)prim);
        CsrPmemFree(prim);
    }
    CSR_UNUSED(data);
}

static void csrBtHciCmSetHciCmdStateIdle(CsrCmnListElm_t *elem, void *data)
{
    CsrBtHciCmCommonHciCmdListType *element = (CsrBtHciCmCommonHciCmdListType*)elem;
    element->cmdState = CSR_BT_HCI_CM_CMD_STATE_IDLE;
    CSR_UNUSED(data);
}

static CsrBool csrBtHciCmCompareHciCmdIdExt(CsrCmnListElm_t *elem, void *value)
{
    CsrBtHciCmCompareType compareVar = *(CsrBtHciCmCompareType *)value;
    CsrBtHciCmCommonHciCmdListType *element = (CsrBtHciCmCommonHciCmdListType *)elem;

    if (element->cmdId == compareVar.cmdId)
    {
        if (element->cmdMatchFunc && compareVar.cmdComplete)
        { 
            return (element->cmdMatchFunc(element->cmdArg, compareVar.eventArg));
        }
        else
        { /* Note that the match function is only valid if it is a HCI complete 
             event. A HCI status event don't have any eventArg */
            return (TRUE);
        }
    }
    return (FALSE);
}

static CsrBool csrBtHciCmFindSecondaryHciCmdId(CsrCmnListElm_t *elem, void *value)
{
    CsrBtHciCmCommonHciCmdListType *element = (CsrBtHciCmCommonHciCmdListType *)elem;
    CSR_UNUSED(value);

    if (element->secondaryCmdId != CSR_BT_HCI_CM_INVALID_HCI_CMD_IF)
    {
        return (TRUE);
    }
    return (FALSE);
}

static CsrBool csrBtHciCmFindHciCmdInStateIdle(CsrCmnListElm_t *elem, void *value)
{
    CsrBtHciCmCommonHciCmdListType *element = (CsrBtHciCmCommonHciCmdListType *)elem;
    CSR_UNUSED(value);

    if (element->cmdState == CSR_BT_HCI_CM_CMD_STATE_IDLE)
    {
        return (TRUE);
    }
    return (FALSE);
}

static CsrBool csrBtHciCmCompareHciCmdId(CsrCmnListElm_t *elem, void *value)
{
    CsrUint32 cmdId = *(CsrUint32 *)value;
    CsrBtHciCmCommonHciCmdListType *element = (CsrBtHciCmCommonHciCmdListType *)elem;

    if (element->cmdId == cmdId)
    {
        return (TRUE);
    }
    return (FALSE);
}

/* Public functions */
void CsrBtHciCmInit(CsrSchedQid appHandle)
{ /* Initialize the Chip Managers instance data */
    bcCmInstData = CsrPmemAlloc(sizeof(CsrBtHciCmInstDataType));
    bcCmInstData->state = CSR_BT_HCI_CM_STATE_NORMAL;
    bcCmInstData->numHciCommandPackets = 0;
    bcCmInstData->readingFromHciDeviceSetupList = FALSE;
    bcCmInstData->appHandle = appHandle;
    CsrCmnListInit(&(bcCmInstData->hciCmdPendingList), 0, NULL, (CsrCmnListRemoveFunc_t) csrBtHciCmFreeCmdResendElement);
    CsrCmnListInit(&(bcCmInstData->hciDeviceSetupList), 0, NULL, (CsrCmnListRemoveFunc_t) csrBtHciCmFreeCmdResendElement);
    CsrCmnListInit(&(bcCmInstData->aclConnHandleList), 0, NULL, NULL);
}

void CsrBtHciCmDeinit(void)
{ /* De-initialize the Chip Managers instance data */
    if (bcCmInstData)
    {
        CSR_BT_HCI_CM_FREE_ALL_CMD_ON_PENDING_LIST();
        CsrCmnListDeinit(&(bcCmInstData->hciCmdPendingList));

        CSR_BT_HCI_CM_FREE_ALL_CMD_ON_DEVICE_SETUP_LIST();
        CsrCmnListDeinit(&(bcCmInstData->hciDeviceSetupList));

        CsrCmnListDeinit(&(bcCmInstData->aclConnHandleList));

        CsrPmemFree(bcCmInstData);
        bcCmInstData = NULL;
    }
}

CsrBool CsrBtHciCmCmdResendTypeAlwaysToggleHandler(void *cmdStream, CsrUint32 cmdId, CsrBool remove)
{ 
    return (csrBtHciCmHciCmdHandler(cmdStream,
                                    HCI_COMMAND_DATA_SIZE + HCI_COMMAND_HEADER_SIZE,
                                    NULL,
                                    (CsrUint8)(remove
                                               ? CSR_BT_HCI_CM_CMD_TYPE_ALWAYS_REMOVE
                                               : CSR_BT_HCI_CM_CMD_TYPE_ALWAYS_ADD),
                                    cmdId,
                                    CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL));
}

CsrBool CsrBtHciCmCmdResendTypeAlwaysHandler(void *cmdStream, CsrUint32 cmdId)
{
    return (csrBtHciCmHciCmdHandler(cmdStream,
                                    HCI_COMMAND_DATA_SIZE + HCI_COMMAND_HEADER_SIZE,
                                    NULL,
                                    CSR_BT_HCI_CM_CMD_TYPE_ALWAYS_ADD,
                                    cmdId,
                                    CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL));
}


CsrBool CsrBtHciCmCmdResendTypePendingHandler(void *cmdStream, 
                                              void *cmdArg,
                                              CsrUint32 cmdId,
                                              CsrUint32 secondaryCmdId,
                                              CsrBtHciCmFreeType cmdArgFreeFunc,
                                              CsrBtHciCmMatchCompareType cmdMatchFunc)
{
    return (csrBtHciCmHciCmdHandler(cmdStream,
                                    HCI_COMMAND_DATA_SIZE + HCI_COMMAND_HEADER_SIZE,
                                    cmdArg,
                                    CSR_BT_HCI_CM_CMD_TYPE_PENDING,
                                    cmdId,              
                                    secondaryCmdId,
                                    NULL,
                                    NULL,
                                    cmdArgFreeFunc,
                                    cmdMatchFunc));
}

CsrBool CsrBtHciCmCmdResendTypeNeverHandler(void *cmdArg,
                                           CsrUint32 cmdId, 
                                           CsrUint32 secondaryCmdId,
                                           CsrBtHciCmEventStatusType cmdStatusEventHandler,
                                           CsrBtHciCmEventCompleteType cmdCompleteEventHandler,
                                           CsrBtHciCmFreeType cmdArgFreeFunc,
                                           CsrBtHciCmMatchCompareType cmdMatchFunc)
{
    return (csrBtHciCmHciCmdHandler(NULL,
                                    0,
                                    cmdArg,
                                    CSR_BT_HCI_CM_CMD_TYPE_NEVER,
                                    cmdId, 
                                    secondaryCmdId,
                                    cmdStatusEventHandler,
                                    cmdCompleteEventHandler,
                                    cmdArgFreeFunc,
                                    cmdMatchFunc));
}

CsrBool CsrBtHciCmEventHandler(CsrUint32 cmdId, 
                               hci_return_t hciStatus, 
                               CsrBool cmdComplete,
                               CsrUint8 *numHciCommandPackets,
                               const void *eventArg)
{
    CsrBool sendEventToDm = TRUE;
    CsrBtHciCmCommonHciCmdListType *cmd;
#ifdef CSR_CHIP_MANAGER_TEST_ENABLE

    if (cmdId != HCI_DISCONNECT &&
        cmdId != HCI_CREATE_CONNECTION &&
        CsrBcCmTestCmdActivityEventHandler(TRANSPORT_CHANNEL_HCI, cmdId, 
                                              (void*)NULL, cmdComplete))
    { /* A reset is in progress and we need to block the event. (We want to 
         test if we are able to recover from the case wrere the incomming 
         event has never been received.) */
        return (FALSE);
    }
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */

    /* Check if the HCI credits need to be updated  */
    csrBtHciCmUpdateHciCredits(numHciCommandPackets);

    /* Find out if the received Hci event has a matcing Hci command */
    cmd = csrBtHciCmFindMatchingHciCmdEvent(cmdId, eventArg, cmdComplete);
    
    if (cmd == NULL)
    { /* No match were found. Need to decide if Hci events shall be sent to DM 
         or not */
        return (CsrBtHciCmSendRequest());
    }

    if (bcCmInstData)
    {/* Set internal chip manager state based on received event */
        if (!bcCmInstData->readingFromHciDeviceSetupList)
        {
            csrBtHciCmSetHciCmdState(cmd, cmdId, hciStatus, cmdComplete);
        }

        if (bcCmInstData->state == CSR_BT_HCI_CM_REPLAYING)
        {
            if (bcCmInstData->readingFromHciDeviceSetupList)
            {
                sendEventToDm = FALSE;
                if (hciStatus != HCI_SUCCESS)
                {
#ifdef CSR_CHIP_MANAGER_ENABLE
                    CsrTmBlueCoreCmReplayErrorReqSend((CsrUint16)cmdId);
#endif
                    CSR_BT_HCI_CM_REMOVE_CMD_FROM_DEVICE_SETUP_LIST(cmd);
                    bcCmInstData->hciDeviceSetupListIndex--;
                }
            }
            else
            {
                if (numHciCommandPackets)
                { /* This event must be a Command Complete Event (0x0E) or a Command 
                     Status Event (0x0F). Set Num_HCI_Command_Packets to 0 in order 
                     to ensure that the DM don't start to send HCI messages */    
                    *numHciCommandPackets = 0;
                }
            }
        
            /* Must only called csrBtHciCmReplayHciCmdHandler if the Hci command 
               which is being replayed is completed */
            if (csrBtHciCmReplayHciCmdHandler())
            { /* No More messages on the Pending queue.
                 Set normal state and possibly send NOP
                 with credits */   
                if (sendEventToDm && numHciCommandPackets)
                {
                    *numHciCommandPackets = bcCmInstData->numHciCommandPackets;
                    bcCmInstData->numHciCommandPackets = 0;
                }
                csrBtHciCmResetToNormalHandler();
            }
        }
    }
    return (sendEventToDm);
}

CsrBool CsrBtHciCmEventConnectionCompleteHandler(hci_return_t status,
                                                 hci_connection_handle_t handle,
                                                 CsrUint8 linkType,
                                                 const void *eventArg)
{ /* A Connection Complete Event (0x03) is received */

#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
    if (CsrBcCmTestCmdActivityEventHandler(TRANSPORT_CHANNEL_HCI, 
                                              HCI_CREATE_CONNECTION, 
                                              (void*)NULL, 
                                              TRUE))
    { /* A reset is in progress and we need to block the event. (We want to 
         test if we are able to recover from the case wrere the incomming 
         event has never been received.) */
        return (FALSE);
    }
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */    
        
    if (linkType == HCI_LINK_TYPE_ACL && status == HCI_SUCCESS && bcCmInstData)
    { /* An ACL connection is establish, save the connection handle */ 
        CsrBtHciCmAclConnHandleListType *element = CSR_BT_HCI_CM_ADD_CONN_HANDLE_LIST();
        element->connHandle = handle;
    }
    return(CsrBtHciCmEventHandler(HCI_CREATE_CONNECTION, 
                                  status, 
                                  TRUE, 
                                  NULL, 
                                  eventArg));
}

#ifdef CSR_BT_LE_ENABLE
CsrBool CsrBtHciCmEventUlpConnectionCompleteHandler(hci_return_t status,
                                                    hci_connection_handle_t handle,
                                                    const void *eventArg)
{ /* A LE Connection Complete Event (0x01) is received */

#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
    if (CsrBcCmTestCmdActivityEventHandler(TRANSPORT_CHANNEL_HCI,
                                              HCI_ULP_CREATE_CONNECTION,
                                              (void*)NULL,
                                              TRUE))
    { /* A reset is in progress and we need to block the event. (We want to
         test if we are able to recover from the case where the incoming
         event has never been received.) */
        return (FALSE);
    }
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */

    switch(status)
    {
        case HCI_ERROR_DIRECTED_ADVERTISING_TIMEOUT:
        {
            csrBtHciCmRemoveHciCmdIdFromResendAlwaysList(HCI_ULP_SET_ADVERTISE_ENABLE);
            break;
        }
        case HCI_SUCCESS:
        {
            if (bcCmInstData)
            { /* An ACL connection is establish, save the connection handle */
                CsrBtHciCmAclConnHandleListType *element = CSR_BT_HCI_CM_ADD_CONN_HANDLE_LIST();
                element->connHandle = handle;
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return(CsrBtHciCmEventHandler(HCI_ULP_CREATE_CONNECTION,
                                  status,
                                  TRUE,
                                  NULL,
                                  eventArg));
}
#endif

CsrBool CsrBtHciCmEventDisconnectionCompleteHandler(hci_return_t status,
                                                    hci_connection_handle_t handle)
{ /* A Disconnection Complete Event (0x05) is received */
#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
    if (CsrBcCmTestCmdActivityEventHandler(TRANSPORT_CHANNEL_HCI, 
                                              HCI_DISCONNECT, 
                                              (void*)NULL, 
                                              TRUE))
    { /* A reset is in progress and we need to block the event. (We want to 
         test if we are able to recover from the case wrere the incomming 
         event has never been received.) */
        return (FALSE);
    }
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */    
        
    if (status == HCI_SUCCESS && bcCmInstData)
    { /* A connection is release, try to find a connection handle 
         and remove it */  
        CsrBtHciCmAclConnHandleListType *element = CSR_BT_HCI_CM_SEARCH_CONN_HANDLE_LIST(&(handle));
        if (element)
        { /* A matching connection handle is found, remove it */
            CSR_BT_HCI_CM_REMOVE_CONN_HANDLE_LIST(element);
        }
    }
    return(CsrBtHciCmEventHandler(HCI_DISCONNECT, status, TRUE, NULL, NULL));
}

CsrBool CsrBtHciCmSendRequest(void)
{ /* Check if the command must be send to the BlueCore Chip or not */
    if (bcCmInstData)
    {
        if (bcCmInstData->state == CSR_BT_HCI_CM_REPLAYING)
        { /* Return FALSE to indicate that the command must not be send to 
             the BlueCore Chip */
            return (FALSE);
        }
    }
    /* Return TRUE to indicate that a command can be send to the BlueCore 
       Chip */
    return (TRUE);
}

void CsrBtHciCmResendStart(void)
{ /* Begin the seamless reset procedure */
    if (bcCmInstData)
    {
        bcCmInstData->state = CSR_BT_HCI_CM_REPLAYING;
        bcCmInstData->readingFromHciDeviceSetupList = TRUE;
        bcCmInstData->hciDeviceSetupListIndex = 0;
        csrBtHciCmHciEventDisconnectionCompleteSendHandler();
        CSR_BT_HCI_CM_SET_CMD_STATE_TO_IDLE_ON_PENDING_LIST();

#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
        CsrBcCmTestResendStartedHandler();
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */

        if (csrBtHciCmReplayHciCmdHandler())
        { /* Either the Input queue were empty or only commands of the type 
             CSR_BT_HCI_CM_CMD_TYPE_NEVER were place on it. E.g. ignore
            return value of csrBtHciCmResetToNormalHandler */
            bcCmInstData->readingFromHciDeviceSetupList  = FALSE;
            csrBtHciCmResetToNormalHandler();
        }
    }
}

void CsrBtHciCmResetDetectedRequest(void)
{ /* The BlueCore chip is reset */
    if (bcCmInstData)
    {
        bcCmInstData->state = CSR_BT_HCI_CM_STATE_RESETTING;
    }
}

void CsrBtHciCmRestartDetectedRequest(void)
{ /* The BlueCore chip is restarted (NOP) after reset */
}
#endif /* CSR_CHIP_MANAGER_ENABLE */


/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sd_prim.h"
#include "csr_bccmd_lib.h"
#include "csr_bt_cm_bccmd.h"
#include "csr_bt_cm_events_handler.h"
#include "csr_bt_cm_util.h"

#define CSR_BT_CM_INVALID_INQUIRY_TX_POWER_LEVEL    (21)

/*--------------------------------------------------------------------------
 * Defines for inquiry and scan to be allowed simultaneously
 *--------------------------------------------------------------------------*/
#define CM_DM_HCI_PAGESCAN_INTERVAL         0x0800
#define CM_DM_HCI_PAGESCAN_WINDOW           0x0800
#define CM_DM_HCI_INQUIRYSCAN_INTERVAL      0x0800
#define CM_DM_HCI_INQUIRYSCAN_WINDOW        0x0800
#define CM_DM_HCI_INQUIRYSCAN_TYPE          HCI_SCAN_TYPE_LEGACY
#define CM_DM_HCI_PAGESCAN_TYPE             HCI_SCAN_TYPE_LEGACY


/* Static functions */
static void csrBtCmInquiryCfmSend(CsrSchedQid phandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtCmInquiryCfm *prim = (CsrBtCmInquiryCfm *) CsrPmemAlloc(sizeof(CsrBtCmInquiryCfm));
    prim->type              = CSR_BT_CM_INQUIRY_CFM;
    prim->resultCode        = resultCode;
    prim->resultSupplier    = resultSupplier;
    CsrBtCmPutMessage(phandle, prim);
}

static void csrBtCmSendInquiryResultInd(CsrSchedQid appHandle, CsrUint32 status, CsrBtClassOfDevice classOfDevice, CsrInt8 rssi, CsrBtDeviceAddr *deviceAddr, CsrUint8 eirDataLength, CsrUint8 *eirData)
{
    CsrBtCmInquiryResultInd *cmPrim;

    cmPrim                  = CsrPmemAlloc(sizeof(CsrBtCmInquiryResultInd));
    cmPrim->type            = CSR_BT_CM_INQUIRY_RESULT_IND;
    cmPrim->classOfDevice   = classOfDevice;
    cmPrim->rssi            = rssi;
    cmPrim->eirDataLength   = eirDataLength;
    cmPrim->eirData         = eirData;
    cmPrim->deviceAddr      = *deviceAddr;
    cmPrim->status          = status;
    CsrBtCmPutMessage(appHandle, cmPrim);
}

static void csrBtCmStartHciInquiry(cmInstanceData_t *cmData)
{
    /* Start inquiry                                              */
    CsrUint8 inqTime = 0;
    CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryDmState, CM_INQUIRY_DM_STATE_INQUIRING);
    if(cmData->dmVar.scanEnabled && (cmData->dmVar.currentChipScanMode & CSR_BT_HCI_SCAN_ENABLE_INQ))
    {
        inqTime = HCI_INQUIRY_LENGTH_SHORTENED;
    }
    else
    {
        inqTime = HCI_INQUIRY_LENGTH_MAX;
    }
    dm_hci_inquiry(cmData->dmVar.inquiryAccessCode, inqTime, 0x00, NULL);

}

static void csrBtCmStopInquiry(cmInstanceData_t *cmData)
{
    CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryDmState, CM_INQUIRY_DM_STATE_CANCELLING);
    dm_hci_inquiry_cancel(NULL);

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
    /* Raise event that inquiry operation is stopped */
    CsrBtCmPropgateEvent(cmData,
                        CsrBtCmPropagateInquiryPageEvents,
                        CSR_BT_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE,
                        HCI_SUCCESS,
                        NULL,
                        NULL);
#endif
}

static void csrBtCmSetupInquiryPriorityLevel(cmInstanceData_t *cmData)
{
    if ((cmData->dmVar.inquiryMask & CSR_BT_CM_INQUIRY_MASK_SET_PRIORITY_LEVEL) == CSR_BT_CM_INQUIRY_MASK_SET_PRIORITY_LEVEL)
    {
        CsrUint8 *payload      = (CsrUint8 *) CsrPmemAlloc(sizeof(CsrUint16));
        CsrUint8 *tmpPayload   = payload;
        CSR_ADD_UINT8_TO_XAP(tmpPayload, cmData->dmVar.lowInquiryPriorityLevel);
        CsrBccmdWriteReqSend(CSR_BT_CM_IFACEQUEUE, BCCMD_VARID_INQUIRY_PRIORITY, 0, sizeof(CsrUint16), payload);
        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryDmState, CM_INQUIRY_DM_STATE_SET_LOW_PRIORITY);
    }
    else
    {  /* Start inquiry                                              */
        csrBtCmStartHciInquiry(cmData);
    }
}

static void csrBtCmScanAllowedClearSettings(cmInstanceData_t *cmData)
{
    cmData->dmVar.scanEnabled = FALSE;
    cmData->dmVar.scanTime    = SCAN_TIMER_DEFAULT;
}

static void csrBtCmScanAllowedStopTimer(cmInstanceData_t *cmData)
{
    if (cmData->dmVar.scanTimerId != 0)
    {/* Cancel the timer if it is already running */
        CsrSchedTimerCancel(cmData->dmVar.scanTimerId, NULL, NULL);
        cmData->dmVar.scanTimerId = 0;
    }

    if(cmData->dmVar.scanIntervalChanged)
    {
        CsrBtCmWriteInquiryScanSettingsReqSend(CSR_BT_CM_IFACEQUEUE, cmData->dmVar.origInqScanInterval, cmData->dmVar.origInqScanWindow );
        cmData->dmVar.scanIntervalChanged = FALSE;
    }
}

static void csrBtCmScanAllowedTimeout(CsrUint8 dummy, cmInstanceData_t *cmData)
{
    CSR_UNUSED(dummy);
    cmData->dmVar.scanTimerId = 0; /* Timer was fired */

    if(cmData->dmVar.scanIntervalChanged)
    {
        CsrBtCmWriteInquiryScanSettingsReqSend(CSR_BT_CM_IFACEQUEUE, cmData->dmVar.origInqScanInterval, cmData->dmVar.origInqScanWindow );
        cmData->dmVar.scanIntervalChanged = FALSE;
    }

    if (cmData->dmVar.inquiryAppState == CM_INQUIRY_APP_STATE_INQUIRING)
    { /* Start inquiry                                              */
        csrBtCmStartHciInquiry(cmData);
    }
    else if (cmData->dmVar.inquiryAppState == CM_INQUIRY_APP_STATE_RESTARTING)
    {
        CsrBtCmStartInquiry(cmData);
    }
    else
    {   /* cmData->dmVar.inquiryAppState = CM_INQUIRY_APP_STATE_IDLE */
        /* No action required in this case */
        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryDmState, CM_INQUIRY_DM_STATE_IDLE);
        csrBtCmScanAllowedClearSettings(cmData);
    }

}

static void csrBtCmScanAllowedStartTimer(cmInstanceData_t *cmData)
{
    /* Stop the timer if it is already running - should not be running at this point */
    csrBtCmScanAllowedStopTimer(cmData);

    if(!cmData->dmVar.scanIntervalChanged)
    {
        cmData->dmVar.origInqScanInterval = cmData->dmVar.inquiryscanInterval;
        cmData->dmVar.origInqScanWindow = cmData->dmVar.inquiryscanWindow;
        CsrBtCmWriteInquiryScanSettingsReqSend(CSR_BT_CM_IFACEQUEUE, CM_DM_HCI_INQUIRYSCAN_INTERVAL, CM_DM_HCI_INQUIRYSCAN_WINDOW);
        cmData->dmVar.scanIntervalChanged = TRUE;
    }

    if(cmData->dmVar.scanTime >= 10 * SCAN_TIMER_DEFAULT)
    {
        cmData->dmVar.scanTime = SCAN_TIMER_DEFAULT;
    }
   
    /* Start the timer */
    cmData->dmVar.scanTimerId = CsrSchedTimerSet(cmData->dmVar.scanTime*1000, (void (*) (CsrUint16, void *)) csrBtCmScanAllowedTimeout, 0, (void *)  cmData);
    cmData->dmVar.scanTime += SCAN_TIMER_DEFAULT;
}


/* Global app handler functions*/
void CsrBtCmStartInquiry(cmInstanceData_t *cmData)
{
    /* Make sure the flush cache timer is not running */
    CsrBtCmFlushCmCacheStopTimer(cmData);
    CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryAppState, CM_INQUIRY_APP_STATE_INQUIRING);

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
    /* Raise event that inquiry operation is started */
    CsrBtCmPropgateEvent(cmData,
                        CsrBtCmPropagateInquiryPageEvents,
                        CSR_BT_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE,
                        HCI_SUCCESS,
                        NULL,
                        NULL);
#endif

    if ((cmData->dmVar.lmpVersion >= CSR_BT_BLUETOOTH_VERSION_2P1) && 
        (cmData->dmVar.inquiryMask & CSR_BT_CM_INQUIRY_MASK_SET_TRANSMIT_POWER) == CSR_BT_CM_INQUIRY_MASK_SET_TRANSMIT_POWER)
    {
        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryDmState, CM_INQUIRY_DM_STATE_SETTING_INQUIRY_POWER_LEVEL);
        dm_hci_write_inquiry_transmit_power_level_req(cmData->dmVar.inquiryTxPowerLevel, NULL);
    }
    else
    { /* dm_hci_write_inquiry_transmit_power_level not supported                    */
        csrBtCmSetupInquiryPriorityLevel(cmData);
    }
}

void CsrBtCmInquiryReqHandler(cmInstanceData_t *cmData)
{
    CsrInt8           inqPriorityLevel;

    CsrBtCmInquiryReq *cmPrim = (CsrBtCmInquiryReq *)cmData->recvMsgP;
    cmData->dmVar.inquiryMask = CSR_BT_CM_INQUIRY_MASK_SET_NONE;

    if (cmData->dmVar.inquiryAppState == CM_INQUIRY_APP_STATE_IDLE)
    {/* Only start if not already running                                           */
  
     /* Only supported from CSR_BT_BLUETOOTH_VERSION_2P1 
         - Here we store the value, but it is only used if cmData->dmVar.lmpVersion >= CSR_BT_BLUETOOTH_VERSION_2P1 */
        CsrInt8  inqTxPowerLevel;

        if (cmPrim->inquiryTxPowerLevel < -70 || cmPrim->inquiryTxPowerLevel > 20)
        {
            inqTxPowerLevel = CSR_BT_SD_DEFAULT_INQUIRY_TX_POWER_LEVEL;
        }
        else
        {
            inqTxPowerLevel = cmPrim->inquiryTxPowerLevel;
        }

        if (inqTxPowerLevel != cmData->dmVar.inquiryTxPowerLevel)
        {
            cmData->dmVar.inquiryTxPowerLevel     = inqTxPowerLevel;
            cmData->dmVar.inquiryMask             = CSR_BT_CM_INQUIRY_MASK_SET_TRANSMIT_POWER;  
        }
        else
        { /* No need to Set transmit power                                          */
            ;
        }

        if (cmPrim->configMask & CSR_BT_SD_SEARCH_LOW_INQUIRY_PRIORITY_DURING_ACL)
        { /* Request to set inquiry to low priority level                               */
            inqPriorityLevel = CSR_BT_CM_LOW_PRIORITY_INQUIRY_LEVEL;
        }
        else
        { /* Request to set inquiry to default level                                    */
            inqPriorityLevel = CSR_BT_CM_DEFAULT_INQUIRY_LEVEL;
        }

        if (inqPriorityLevel != cmData->dmVar.lowInquiryPriorityLevel)
        {
            if (inqPriorityLevel == CSR_BT_CM_DEFAULT_INQUIRY_LEVEL)
            { /* Must set inquiry to default level                                      */
                cmData->dmVar.inquiryMask               = (cmData->dmVar.inquiryMask | CSR_BT_CM_INQUIRY_MASK_SET_PRIORITY_LEVEL); 
                cmData->dmVar.lowInquiryPriorityLevel   = inqPriorityLevel;
            }
            else 
            { /* Requested low inquiry priority level                                   */
                if (returnNumOfAclConnection(cmData) > 0)
                { /* Set inquiry to low priority level                                  */
                    cmData->dmVar.inquiryMask               = (cmData->dmVar.inquiryMask | CSR_BT_CM_INQUIRY_MASK_SET_PRIORITY_LEVEL); 
                    cmData->dmVar.lowInquiryPriorityLevel   = inqPriorityLevel;       
                }
                else
                { /* Requested low inquiry level, but no ACL, 
                    keep current setting this time                                     */ 
                    ;
                }
            }
        }
        else 
        { /* No changes in the inquiry priority level                                   */
            ;
        }

        /* if configured to ensure inquiry scan is available during inquiry */
        if (cmPrim->configMask & CSR_BT_SD_SEARCH_ENABLE_SCAN_DURING_INQUIRY)
        { /* Request that scan get a time slot                                  */
            cmData->dmVar.scanEnabled = TRUE;
            cmData->dmVar.scanTime    = SCAN_TIMER_DEFAULT;
        }
    

        cmData->dmVar.inquiryAppHandle  = cmPrim->appHandle;
        cmData->dmVar.inquiryAccessCode = cmPrim->inquiryAccessCode;

        if ((cmData->globalState == notReady_s) ||
            (cmData->dmVar.inquiryDmState == CM_INQUIRY_DM_STATE_CANCELLING) ||
            (cmData->dmVar.inquiryDmState == CM_INQUIRY_DM_STATE_SETTING_INQUIRY_POWER_LEVEL) ||
            (cmData->dmVar.inquiryDmState == CM_INQUIRY_DM_STATE_SET_LOW_PRIORITY))
        {/* Don't start new inquiry before the init procedure is finished,
            if the inquiry power level is being set or if inquiry is being cancelled */
            CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryAppState, CM_INQUIRY_APP_STATE_RESTARTING);
        }
        else
        {/* Start a new inquiry */
            CsrBtCmStartInquiry(cmData);
        }
    }
    else
    {/* Inquiry is already running */
        csrBtCmInquiryCfmSend(cmPrim->appHandle, CSR_BT_RESULT_CODE_CM_COMMAND_DISALLOWED, CSR_BT_SUPPLIER_CM);
    }
}

void CsrBtCmCancelInquiryReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmCancelInquiryReq *cmPrim = (CsrBtCmCancelInquiryReq *)cmData->recvMsgP;

    if (cmPrim->phandle == cmData->dmVar.inquiryAppHandle)
    {/* Only allow apps that started the inquiry to cancel it */
        if (cmData->dmVar.inquiryAppState != CM_INQUIRY_APP_STATE_IDLE)
        {/* Do nothing if the app is already in idle state */
            CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryAppState, CM_INQUIRY_APP_STATE_IDLE);

            if (cmData->dmVar.inquiryDmState == CM_INQUIRY_DM_STATE_INQUIRING)
            {/* Inquiry is running, cancel the inquiry process */
                csrBtCmStopInquiry(cmData);
            }
            else
            { /* Make sure that the inquiry Txc Power level and inquiry level is
                 set correct the next time inquiry is started                                       */
                cmData->dmVar.inquiryTxPowerLevel     = CSR_BT_CM_INVALID_INQUIRY_TX_POWER_LEVEL;
                cmData->dmVar.lowInquiryPriorityLevel = CSR_BT_CM_UNDEFINED_PRIORITY_INQUIRY_LEVEL;
                csrBtCmScanAllowedClearSettings(cmData);
            }
        }
    }
}

/* Global DM handler functions */
void CsrBtCmDmHciWriteInquiryTransmitPowerLevelCompleteHandler(cmInstanceData_t *cmData)
{
    if (cmData->dmVar.inquiryAppState == CM_INQUIRY_APP_STATE_INQUIRING)
    {
        csrBtCmSetupInquiryPriorityLevel(cmData);
    }
    else if (cmData->dmVar.inquiryAppState == CM_INQUIRY_APP_STATE_RESTARTING)
    {
        CsrBtCmStartInquiry(cmData);
    }
    else
    {   /* cmData->dmVar.inquiryAppState = CM_INQUIRY_APP_STATE_IDLE */
        /* No action required in this case */
        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryDmState, CM_INQUIRY_DM_STATE_IDLE);
        csrBtCmScanAllowedClearSettings(cmData);
    }    
}
void CsrBtCmBccmdInquiryPriorityCfmHandler(cmInstanceData_t *cmData)
{
    CsrBccmdCfm * prim =  (CsrBccmdCfm *) cmData->recvMsgP;

    if (cmData->dmVar.inquiryAppState == CM_INQUIRY_APP_STATE_INQUIRING)
    { /* Start inquiry                                              */
        csrBtCmStartHciInquiry(cmData);
    }
    else if (cmData->dmVar.inquiryAppState == CM_INQUIRY_APP_STATE_RESTARTING)
    {
        CsrBtCmStartInquiry(cmData);
    }
    else
    {   /* cmData->dmVar.inquiryAppState = CM_INQUIRY_APP_STATE_IDLE */
        /* No action required in this case */
        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryDmState, CM_INQUIRY_DM_STATE_IDLE);
        csrBtCmScanAllowedClearSettings(cmData);
    }    
    CsrPmemFree(prim->payload);
}

void CsrBtCmDmHciInquiryCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_INQUIRY_CFM_T *dmPrim;

    dmPrim = (DM_HCI_INQUIRY_CFM_T *)cmData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS)
    {
        if (cmData->dmVar.inquiryAppState != CM_INQUIRY_APP_STATE_IDLE)
        {
            if(cmData->dmVar.scanEnabled && (cmData->dmVar.currentChipScanMode & CSR_BT_HCI_SCAN_ENABLE_INQ))
            {
                CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryDmState, CM_INQUIRY_DM_STATE_SCAN_ALLOWED);
                csrBtCmScanAllowedStartTimer(cmData);                
            }
            else
            {/* Restart the inquiry unless it was intentionally cancelled */
                CsrBtCmStartInquiry(cmData);
            }
        }
        else
        {/* Ignore the request as inquiry is already cancelled */
            CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryDmState, CM_INQUIRY_DM_STATE_IDLE);
            CsrBtCmFlushCmCacheStartTimer(cmData);
        }
    }
    else
    {/* Inquiry failed */
        CsrBtCmFlushCmCacheStartTimer(cmData);

        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryDmState, CM_INQUIRY_DM_STATE_IDLE);

        if (cmData->dmVar.inquiryAppState != CM_INQUIRY_APP_STATE_IDLE)
        {/* Inquiry is requested by the app */
            if (cmData->smVar.smInProgress)
            {/* Don't start inquiry before the serviceManagerProvider or the
                sdcServiceManagerProvider is finish */
                CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryAppState, CM_INQUIRY_APP_STATE_RESTARTING);
            }
            else
            {/* Inquiry is not allowed */
                CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryAppState, CM_INQUIRY_APP_STATE_IDLE);
                csrBtCmInquiryCfmSend(cmData->dmVar.inquiryAppHandle, (CsrBtResultCode) dmPrim->status, CSR_BT_SUPPLIER_HCI);
                csrBtCmScanAllowedClearSettings(cmData);
            }
        } /* if AppState == IDLE --> ignore */
    }
}

void CsrBtCmDmHciInquiryCancelCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_INQUIRY_CANCEL_CFM_T *dmPrim;

    dmPrim = (DM_HCI_INQUIRY_CANCEL_CFM_T *)cmData->recvMsgP;

    if(dmPrim->status == HCI_SUCCESS)
    {
        if (cmData->dmVar.inquiryAppState != CM_INQUIRY_APP_STATE_IDLE)
        {/* New inquiry request has been received while cancelling - restart inquiry */
            CsrBtCmStartInquiry(cmData);
        }
        else
        {/* The app is in idle state */
            CsrBtCmFlushCmCacheStartTimer(cmData);
            CSR_BT_CM_STATE_CHANGE(cmData->dmVar.inquiryDmState, CM_INQUIRY_DM_STATE_IDLE);
            csrBtCmScanAllowedClearSettings(cmData);
        }

    }
    else if (cmData->dmVar.inquiryDmState == CM_INQUIRY_DM_STATE_CANCELLING)
    {/* Only try again if DM-state is still cancelling */
        csrBtCmStopInquiry(cmData);
    }
}

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
void CsrBtCmDmHciExtendedInquiryResultIndHandler(cmInstanceData_t *cmData)
{
    DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T    *dmPrim;
    HCI_INQ_RESULT_WITH_RSSI_T              *resultPtr;
    CsrUint8 eirDataLength = 0, *eirData;
    CsrIntFast8 i;

    dmPrim      = (DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T *)cmData->recvMsgP;
    resultPtr  = &dmPrim->result;

    /* Insert dm cache params in list */
    CsrBtCmDmStoreCacheParams(cmData,
                         resultPtr->bd_addr,
                         resultPtr->clock_offset,
                         0,
                         resultPtr->page_scan_rep_mode);

    if (cmData->dmVar.inquiryAppState == CM_INQUIRY_APP_STATE_INQUIRING)
    {
        /* Build EIR data */
        if (dmPrim->eir_data_part[0] != NULL)
        {
            CsrUint8 eirDataLengthAdd;

            /* Determine the length of the EIR - invalid tags were removed in the converter */
            while (dmPrim->eir_data_part[eirDataLength / HCI_EIR_DATA_BYTES_PER_PTR] != NULL &&
                   (eirDataLengthAdd = (dmPrim->eir_data_part[eirDataLength / HCI_EIR_DATA_BYTES_PER_PTR][eirDataLength % HCI_EIR_DATA_BYTES_PER_PTR])) != 0)
            {
                if ((eirDataLength + eirDataLengthAdd + 1) <= HCI_EIR_DATA_LENGTH)
                {
                    eirDataLength += (eirDataLengthAdd + 1);
                }
                else
                {/* Invalid tag length - should not happen */
                    break;
                }
            }
        }

        if (eirDataLength > 0)
        {
            /* Allocate space for the EIR-data */
            eirData = CsrPmemAlloc(eirDataLength);

            /* Copy full blocks */
            for (i = 0; i < eirDataLength / HCI_EIR_DATA_BYTES_PER_PTR; i++)
            {
                CsrMemCpy(eirData + i * HCI_EIR_DATA_BYTES_PER_PTR, dmPrim->eir_data_part[i], HCI_EIR_DATA_BYTES_PER_PTR);
            }

            /* Copy rest of data */
            CsrMemCpy(eirData + i * HCI_EIR_DATA_BYTES_PER_PTR, dmPrim->eir_data_part[i], eirDataLength % HCI_EIR_DATA_BYTES_PER_PTR);
        }
        else
        {/* No valid Extended Inquiry Response data was available */
            eirDataLength   = 0;
            eirData         = NULL;
        }

        /* Send inquiry result */
        csrBtCmSendInquiryResultInd(cmData->dmVar.inquiryAppHandle,
                               CSR_BT_CM_INQUIRY_STATUS_EIR,
                               resultPtr->dev_class,
                               resultPtr->rssi,
                               &resultPtr->bd_addr,
                               eirDataLength,
                               eirData);
    }

    /* Free EIR pointers */
    for (i = 0; i < HCI_EIR_DATA_PACKET_PTRS; i++ )
    {
        if (!dmPrim->eir_data_part[i])
        {
            break;
        }
        CsrPmemFree(dmPrim->eir_data_part[i]);
    }
}
#endif /* CSR_BT_BT_VERSION */

void CsrBtCmDmHciInquiryResultWithRssiHandler(cmInstanceData_t *cmData)
{
    CsrUintFast16                        i;
    CsrUint16                            res_index;
    CsrUint16                            inq_index;
    CsrUint8                             nResultPtrs;
    DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T   *dmPrim;
    HCI_INQ_RESULT_WITH_RSSI_T          *resultPtr;

    dmPrim = (DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T *)cmData->recvMsgP;

    for (i = 0; i<dmPrim->num_responses; i++)
    {/* There may be several responses in one message */
        res_index       = (CsrUint16)(i / HCI_MAX_INQ_RESULT_PER_PTR);
        inq_index       = (CsrUint16)(i - (res_index * HCI_MAX_INQ_RESULT_PER_PTR));
        resultPtr      = &dmPrim->result[res_index][inq_index];

        /* Insert dm cache params in list */
        CsrBtCmDmStoreCacheParams(cmData,
                             resultPtr->bd_addr,
                             resultPtr->clock_offset,
                             0,
                             resultPtr->page_scan_rep_mode);

        if (cmData->dmVar.inquiryAppState == CM_INQUIRY_APP_STATE_INQUIRING)
        {
            /* Send inquiry result */
            csrBtCmSendInquiryResultInd(cmData->dmVar.inquiryAppHandle,
                                   CSR_BT_CM_INQUIRY_STATUS_NONE,
                                   resultPtr->dev_class,
                                   resultPtr->rssi,
                                   &resultPtr->bd_addr,
                                   0,
                                   NULL);
        }
    }

    /* Free the individual results */
    nResultPtrs = (dmPrim->num_responses + HCI_MAX_INQ_RESULT_PER_PTR - 1 ) / HCI_MAX_INQ_RESULT_PER_PTR;

    for (i = 0; i < nResultPtrs; i++)
    {
        CsrPmemFree(dmPrim->result[i]);
    }
}

void CsrBtCmDmHciInquiryResultHandler(cmInstanceData_t *cmData)
{
    CsrUintFast8                 i;
    CsrUint16                    res_index;
    CsrUint16                    inq_index;
    CsrUint8                     nResultPtrs;
    DM_HCI_INQUIRY_RESULT_IND_T     *dmPrim;
    HCI_INQ_RESULT_T            *resultPtr;

    dmPrim = (DM_HCI_INQUIRY_RESULT_IND_T *)cmData->recvMsgP;

    for (i = 0; i < dmPrim->num_responses; i++)
    {/*  There may be several responses in one message */
        res_index       = (CsrUint16)(i / HCI_MAX_INQ_RESULT_PER_PTR);
        inq_index       = (CsrUint16)(i - (res_index * HCI_MAX_INQ_RESULT_PER_PTR));
        resultPtr      = &dmPrim->result[res_index][inq_index];

        /* Insert dm cache params in list */
        CsrBtCmDmStoreCacheParams(cmData,
                             resultPtr->bd_addr,
                             resultPtr->clock_offset,
                             resultPtr->page_scan_mode,
                             resultPtr->page_scan_rep_mode);

        if (cmData->dmVar.inquiryAppState == CM_INQUIRY_APP_STATE_INQUIRING)
        {
            /* Send inquiry result */
            csrBtCmSendInquiryResultInd(cmData->dmVar.inquiryAppHandle,
                                   CSR_BT_CM_INQUIRY_STATUS_NONE,
                                   resultPtr->dev_class,
                                   CSR_BT_RSSI_INVALID,
                                   &resultPtr->bd_addr,
                                   0,
                                   NULL);
        }
    }

    /* Free the individual results */
    nResultPtrs = (dmPrim->num_responses + HCI_MAX_INQ_RESULT_PER_PTR - 1 ) / HCI_MAX_INQ_RESULT_PER_PTR;

    for (i = 0; i < nResultPtrs; i++)
    {
        CsrPmemFree(dmPrim->result[i]);
    }
}


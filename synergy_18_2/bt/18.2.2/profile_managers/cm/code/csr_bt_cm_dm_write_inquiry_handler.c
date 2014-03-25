/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"

/* Handle inquiry scan setting request */
void CsrBtCmWriteInquiryScanSettingsReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmWriteInquiryscanSettingsReq *prim;
    prim = (CsrBtCmWriteInquiryscanSettingsReq*)cmData->recvMsgP;

    /* Store new settings */
    cmData->dmVar.appHandle           = prim->appHandle;
    cmData->dmVar.inquiryscanInterval = prim->interval;
    cmData->dmVar.inquiryscanWindow   = prim->window;

    /* Send request to core stack */
    dm_hci_write_inquiryscan_activity(cmData->dmVar.inquiryscanInterval,
                                      cmData->dmVar.inquiryscanWindow,
                                      NULL);
}

#ifdef CSR_BT_INSTALL_CM_WRITE_INQUIRY_SCAN_TYPE
/* Handle inquiry scan type request */
void CsrBtCmWriteInquiryScanTypeReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmWriteInquiryscanTypeReq *prim;
    prim = (CsrBtCmWriteInquiryscanTypeReq*)cmData->recvMsgP;

    /* Store new settings */
    cmData->dmVar.appHandle       = prim->appHandle;
    cmData->dmVar.inquiryscanType = prim->scanType;

    /* Send request to core stack */
    dm_hci_write_inquiry_scan_type(prim->scanType,
                                   NULL);
}
#endif

/*************************************************************************************
CsrBtCmDmHciWriteInquiryScanTypeCompleteHandler
*************************************************************************************/
void CsrBtCmDmHciWriteInquiryScanTypeCompleteHandler(cmInstanceData_t * cmData)
{
    DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_T    *dmPrim;
    dmPrim = (DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM_T *)cmData->recvMsgP;

    if(cmData->globalState == notReady_s)
    {
        /* We are currently in the CM initialization process, we must
         * continue setting up the chip */

        if(dmPrim->status == HCI_SUCCESS)
        {
            cmData->dmVar.fallbackPerformed = FALSE;

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
            if (cmData->dmVar.lmpVersion >= CSR_BT_BLUETOOTH_VERSION_2P1)
            {
                dm_hci_write_inquiry_mode(HCI_INQUIRY_MODE_WITH_EIR,
                                          NULL);
            }
            else
#endif
            {
                dm_hci_write_inquiry_mode(HCI_INQUIRY_MODE_WITH_RSSI,
                                          NULL);
            }
        }
        else
        {
            /* The host controller did not accept Interlaced inquiry
             * mode. Try again with Normal mode */
            if (!cmData->dmVar.fallbackPerformed)
            {
                cmData->dmVar.fallbackPerformed = TRUE;
                dm_hci_write_inquiry_scan_type(HCI_SCAN_TYPE_LEGACY,
                                               NULL);

            }
            else
            {
                cmData->dmVar.fallbackPerformed = FALSE;
                dm_hci_write_inquiryscan_activity(cmData->dmVar.inquiryscanInterval,
                                                  cmData->dmVar.inquiryscanWindow,
                                                  NULL);
            }
        }
    }
#ifdef CSR_BT_INSTALL_CM_WRITE_INQUIRY_SCAN_TYPE
    else
    {
        /* We are in normal CM operation mode, so handle the confirm from the chip,
         * tell the user app and finally restore the queue */
        CsrBtCmWriteInquiryscanTypeCfm *prim = CsrPmemAlloc(sizeof(CsrBtCmWriteInquiryscanTypeCfm));
        prim->type = CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_CFM;
        if (dmPrim->status == HCI_SUCCESS)
        {
            prim->resultCode     = CSR_BT_RESULT_CODE_CM_SUCCESS;
            prim->resultSupplier = CSR_BT_SUPPLIER_CM;
        }
        else
        {
            prim->resultCode     = (CsrBtResultCode) dmPrim->status;
            prim->resultSupplier = CSR_BT_SUPPLIER_HCI;
        }
        CsrBtCmPutMessage(cmData->dmVar.appHandle, prim);
        CsrBtCmDmLocalQueueHandler();
    }
#endif
}

/*************************************************************************************
CsrBtCmDmHciWriteInquiryScanModeCompleteHandler
*************************************************************************************/
void CsrBtCmDmHciWriteInquiryModeCompleteHandler(cmInstanceData_t * cmData)
{
    DM_HCI_WRITE_INQUIRY_MODE_CFM_T *dmPrim;
    dmPrim = (DM_HCI_WRITE_INQUIRY_MODE_CFM_T *)cmData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS)
    {
        cmData->dmVar.fallbackPerformed = FALSE;

        if (cmData->dmVar.lmpVersion >= CSR_BT_BLUETOOTH_VERSION_2P1)
        {
            CsrUint8 version;

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
            /* LMP version is 2.1 or newer - init EIR-data and use other 2.1 features */
            CsrBtCmEirInitData(cmData);
#endif

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_4P0
            version = BT_VERSION_4p0;
#elif CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_3P0
            version = BT_VERSION_3p0;
#elif CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
            version = BT_VERSION_2p1;
#else
            version = BT_VERSION_2p0;
#endif

            dm_set_bt_version(version, NULL);
        }
        else
        {
            dm_hci_write_page_scan_type(cmData->dmVar.pagescanType,
                                    NULL);
        }
    }
    else
    {
        /* The host controller did not accept Interlaced page scan
         * mode. Try again with Normal mode */
        if (!cmData->dmVar.fallbackPerformed)
        {
            cmData->dmVar.fallbackPerformed = TRUE;
            dm_hci_write_inquiry_mode(HCI_INQUIRY_MODE_STANDARD,
                                      NULL);
        }
        else
        {
            cmData->dmVar.fallbackPerformed = FALSE;
            dm_hci_write_page_scan_type(cmData->dmVar.pagescanType,
                                    NULL);
        }
    }
}

/*************************************************************************************
 CsrBtCmDmHciWriteInquiryScanActivityCompleteHandler:
************************************************************************************/
void CsrBtCmDmHciWriteInquiryScanActivityCompleteHandler(cmInstanceData_t *cmData)
{
    /* This event is the confirmation from the host controller
     * following a DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ */
    DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_T *dmPrim;
    dmPrim = (DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM_T *)cmData->recvMsgP;

    if(cmData->globalState == notReady_s)
    {
        /* We are currently in the CM initialization process, we must
         * continue setting up the chip */

        if(dmPrim->status == HCI_SUCCESS)
        {
            /* Setup the voice setting */
            dm_hci_write_voice_setting(CSR_BT_VOICE_SETTING,
                                       NULL);
        }
        else
        {
            /* The hostcontroller did not accept the given inquiry scan
             * window. Try again with BT-specification default values */
            if (!cmData->dmVar.fallbackPerformed)
            {
                cmData->dmVar.fallbackPerformed = TRUE;
                dm_hci_write_inquiryscan_activity(HCI_INQUIRYSCAN_INTERVAL_DEFAULT,
                                                  HCI_INQUIRYSCAN_WINDOW_DEFAULT,
                                                  NULL);
            }
            else
            {
                dm_hci_write_voice_setting(CSR_BT_VOICE_SETTING,
                                           NULL);
            }
        }
    }
    else
    {
        /* We are in normal CM operation mode, so handle the confirm from the chip,
         * tell the user app and finally restore the queue */
        if(cmData->dmVar.appHandle != CSR_BT_CM_IFACEQUEUE)
        {
            CsrBtCmWriteInquiryscanSettingsCfm *prim = CsrPmemAlloc(sizeof(CsrBtCmWriteInquiryscanSettingsCfm));
            prim->type = CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_CFM;

            if (dmPrim->status == HCI_SUCCESS)
            {
                prim->resultCode     = CSR_BT_RESULT_CODE_CM_SUCCESS;
                prim->resultSupplier = CSR_BT_SUPPLIER_CM;
            }
            else
            {
                prim->resultCode     = (CsrBtResultCode) dmPrim->status;
                prim->resultSupplier = CSR_BT_SUPPLIER_HCI;
            }
            
            CsrBtCmPutMessage(cmData->dmVar.appHandle, prim);
        }
        CsrBtCmDmLocalQueueHandler();

    }
}


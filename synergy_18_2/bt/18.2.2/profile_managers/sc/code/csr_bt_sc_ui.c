/****************************************************************************

Copyright Cambridge Silicon Radio Limited and its licensors 2011. All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_panic.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_sc_bond_unified.h"
#include "dm_prim.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_main.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_sc_register.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_sc_bondable_mode.h"
#include "csr_bt_sc_oob.h"
#include "csr_bt_sc_authorise.h"
#include "csr_bt_sc_bond_legacy.h"
#include "csr_bt_sc_le.h"

void CsrBtScForwardPendingInd(ScInstanceData_t * scData, CsrBtDeviceName friendlyName)
{
    SC_CHANGE_STATE(&scData->bondData.appState, APP_RESPOND);
    SC_CHANGE_STATE(&scData->bondData.uiState, UI_COMPLETE);

    switch(scData->pendingMsgType)
    {
        case MSG_TYPE_REBOND:
            CsrBtScRebondIndSend(scData->appHandle, scData->cod,
                                 &scData->deviceAddr, friendlyName,
                                 scData->addressType);
            break;

        case MSG_TYPE_IO_CAPS:
            CsrBtScSspPairingIndSend(scData->appHandle, scData->cod,
                                     &scData->deviceAddr, friendlyName,
                                     CsrBtScRemoteAuthValid(scData->bondData.remoteAuth), 
                                     scData->bondData.remoteAuth,
                                     scData->paired,
                                     scData->bondData.localInitiator,
                                     scData->addressType,
                                     CsrBtScGetHighestAuthRequirement(scData));
            break;

        case MSG_TYPE_COMPARE:
            CsrBtScSspCompareIndSend(scData->appHandle, scData->cod,
                                     &scData->deviceAddr, friendlyName,
                                     scData->paired,
                                     scData->bondData.numValue,
                                     scData->bondData.remoteAuth,
                                     scData->bondData.localInitiator,
                                     scData->addressType,
                                     CsrBtScGetHighestAuthRequirement(scData));
            break;
        case MSG_TYPE_PASSKEY:
            CsrBtScSspPasskeyIndSend(scData->appHandle, scData->cod,
                                     &scData->deviceAddr, friendlyName,
                                     scData->paired,
                                     scData->bondData.remoteAuth,
                                     scData->bondData.localInitiator,
                                     scData->addressType,
                                     CsrBtScGetHighestAuthRequirement(scData));
            break;

        case MSG_TYPE_JUSTWORKS:
            CsrBtScSspJustWorksIndSend(scData->appHandle, scData->cod,
                                       &scData->deviceAddr, friendlyName,
                                       scData->paired,
                                       scData->bondData.remoteAuth,
                                       scData->bondData.localInitiator,
                                       scData->addressType,
                                       CsrBtScGetHighestAuthRequirement(scData));
            break;

        case MSG_TYPE_NOTIFICATION:
            CsrBtScSspNotificationIndSend(scData->appHandle, scData->cod,
                                          &scData->deviceAddr, friendlyName,
                                          scData->paired,
                                          scData->bondData.numValue,
                                          scData->bondData.remoteAuth,
                                          scData->bondData.localInitiator,
                                          scData->addressType,
                                          CsrBtScGetHighestAuthRequirement(scData));
            break;

        case MSG_TYPE_LEGACY:
            CsrBtScPasskeyIndSend(scData->mainAppHandle, scData->cod,
                                  &scData->deviceAddr, friendlyName,
                                  scData->paired,
                                  scData->bondData.localInitiator,
                                  scData->addressType);
            break;

        case MSG_TYPE_AUTHORISE:
        {
            uuid16_t serviceId;
            getUuidOfRegisteredChannel(scData, &serviceId);

            if (CsrBtScEventIsEnabled(scData->eventMask, CSR_BT_SC_EVENT_MASK_AUTHORISE_EXTENDED))
            {
                CsrBtScAuthoriseExtIndSend(scData->mainAppHandle, serviceId,
                                           scData->deviceAddr, friendlyName,
                                           scData->protocolId, scData->channel,
                                           scData->incoming,
                                           scData->addressType);
            }
            else
            {
                CsrBtScAuthoriseIndSend(scData->mainAppHandle, serviceId,
                                        scData->deviceAddr, friendlyName,
                                        scData->addressType);
            }
            break;
        }

        case MSG_TYPE_OOB:
            /* Not forwarded to the app */
        default:
            CsrBtScStateEventException(CSR_BT_SC_PRIM, scData->pendingMsgType, scData->bondData.uiState);
            break;
    }
}

static CsrBool csrBtScUiNeedsRestart(ScInstanceData_t * scData)
{
    return !CsrBtScFriendlyNameValid(scData) || scData->cod == 0;
}

static void csrBtScGetCodFromAclList(ScInstanceData_t * scData)
{
    if (scData->cod == 0)
    {
        CsrUintFast8 index;

        if (CsrBtScAclPresent(scData, scData->deviceAddr, &index))
        {
            scData->cod = scData->aclList[index].cod;
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScStartUi
*
*  DESCRIPTION
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScStartUi(ScInstanceData_t * scData)
{
    /* Restart our watchdog timer */
    CsrBtScStartPairingWatchdog(scData);
    csrBtScGetCodFromAclList(scData);

    if (csrBtScUiNeedsRestart(scData))
    {
        /* Remote device unknown */
        SC_CHANGE_STATE(&scData->bondData.uiState, UI_READ_DEV);
        scData->paired = CsrBtScLookupPairedType(scData);

        CsrBtScAddPendingMessage(&scData->ignoreQueue, CSR_BT_SD_PRIM, CSR_BT_SD_READ_DEVICE_INFO_CFM);
        CsrBtSdReadDeviceInfoReqSendEx(CSR_BT_SC_IFACEQUEUE, scData->deviceAddr, scData->addressType, 0);
    }
    else
    {
        /* Don't kick UI state machine. We already know everything */
        CsrBtScForwardPendingInd(scData, scData->friendlyName);
    }
}

static void csrBtScSetUpdateCod(ScInstanceData_t * scData)
{
    CsrBtSdReadDeviceInfoCfm *sdPrim;

    sdPrim = (CsrBtSdReadDeviceInfoCfm *) scData->recvMsgP;

    if (scData->cod == 0)
    {
        if (sdPrim->deviceClass == 0)
        {
            CsrBtScDbDeviceRecord deviceRecord;

            if(CsrBtScDbRead(&sdPrim->deviceAddr, &deviceRecord))
            {
                scData->cod = deviceRecord.classOfDevice;
            }
        }
        else
        {
            scData->cod = sdPrim->deviceClass;
        }
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSdReadDeviceInfoCfmRnrUiS
*
*  DESCRIPTION
*      Check and see if SD had the name in the cache or request it from the CM.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSdReadDeviceInfoCfmRnrUiS(ScInstanceData_t * scData)
{
    CsrBtSdReadDeviceInfoCfm *sdPrim;
    CsrUint8 *name;

    sdPrim = (CsrBtSdReadDeviceInfoCfm *) scData->recvMsgP;
    name = NULL;
    csrBtScSetUpdateCod(scData);

    if (sdPrim->resultCode == CSR_BT_RESULT_CODE_SD_SUCCESS &&
        sdPrim->resultSupplier == CSR_BT_SUPPLIER_SD &&
#ifdef CSR_BT_INSTALL_SC_CONFIG_MASK
        ((scData->configMask & CSR_BT_SC_CONFIG_MASK_FORCE_NAME_UPDATE) != CSR_BT_SC_CONFIG_MASK_FORCE_NAME_UPDATE) &&
#endif
        CsrBtSdDecodeFriendlyName(sdPrim->info, sdPrim->infoLen, &name))
    {
        CsrBtScCopyFriendlyName(scData->friendlyName, (CsrUtf8String*)name);
    }
    CsrPmemFree(name);

    if (CsrBtScFriendlyNameValid(scData))
    {
        CsrBtScRemoveLastPendingMessage(&scData->ignoreQueue);
        CsrBtScForwardPendingInd(scData, scData->friendlyName);
    }
    else
    {
        CsrBtScReplaceLastPendingMessage(&scData->ignoreQueue, CSR_BT_CM_PRIM, CSR_BT_CM_READ_REMOTE_NAME_CFM);

        /* With LE support, we can read the remote name in two
         * different ways */
#ifdef CSR_BT_LE_ENABLE
        CsrBtScLeReadRemoteName(scData);
#else
        CsrBtCmReadRemoteNameReqSend(CSR_BT_SC_IFACEQUEUE, scData->deviceAddr);
#endif
        SC_CHANGE_STATE(&scData->bondData.uiState, UI_RNR);
    }
}


/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSdReadDeviceInfoCfmUiS
*
*  DESCRIPTION
*      Dispatch to the correct handler according to state.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSdReadDeviceInfoCfmUiS(ScInstanceData_t * scData)
{
    CsrBtSdReadDeviceInfoCfm *sdPrim;
    sdPrim = (CsrBtSdReadDeviceInfoCfm *) scData->recvMsgP;

    switch(scData->bondData.uiState)
    {
        case UI_READ_DEV:
            CsrBtScSdReadDeviceInfoCfmRnrUiS(scData);
            break;

        case UI_CANCELLED:
            CsrBtScRemoveLastPendingMessage(&scData->ignoreQueue);
            break;

        case UI_COMPLETE:
        case UI_RNR:
        default:
            CsrBtScStateEventException(CSR_BT_SD_PRIM, VOIDP_TO_MSG_TYPE(scData), scData->bondData.uiState);
            break;
    }

    CSR_UNUSED(sdPrim);
}

/*----------------------------------------------------------------------------*
*  NAME
*      csrBtScCmReadRemoteNameCfmRnrUiS
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
static void csrBtScCmReadRemoteNameCfmRnrUiS(ScInstanceData_t * scData)
{
    CsrBtCmReadRemoteNameCfm *cmPrim;

    cmPrim = (CsrBtCmReadRemoteNameCfm *) scData->recvMsgP;

    if (cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtScCopyFriendlyName(scData->friendlyName, cmPrim->friendlyName);
    }

    CsrBtScRemoveLastPendingMessage(&scData->ignoreQueue);
    CsrBtScForwardPendingInd(scData, scData->friendlyName);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScCmReadRemoteNameCfmUiS
*
*  DESCRIPTION
*      Dispatch to the correct handler according to state.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCmReadRemoteNameCfmUiS(ScInstanceData_t * scData)
{
    switch(scData->bondData.uiState)
    {
        case UI_RNR:
            csrBtScCmReadRemoteNameCfmRnrUiS(scData);
            SC_CHANGE_STATE(&scData->bondData.uiState, UI_COMPLETE);
            break;

        case UI_CANCELLED:
            CsrBtScRemoveLastPendingMessage(&scData->ignoreQueue);
            break;

        case UI_COMPLETE:
        case UI_READ_DEV:
        default:
            CsrBtScStateEventException(CSR_BT_CM_PRIM, VOIDP_TO_MSG_TYPE(scData), scData->bondData.uiState);
            break;
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScCancelUiS
*
*  DESCRIPTION
*      Cancel the UI statemachine.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCancelUiS(ScInstanceData_t * scData)
{
    switch(scData->bondData.uiState)
    {
        case UI_IDLE:
            /* UI statemachine not running. Ignore it */
            break;

        case UI_RNR:
#ifdef CSR_BT_LE_ENABLE
            CsrBtScLeReadRemoteNameCancel(scData);
#else
            CsrBtCmCancelReadRemoteNameReqSend(CSR_BT_SC_IFACEQUEUE, scData->deviceAddr);
#endif
            /* !! FALL-THROUGH !! */

        case UI_READ_DEV:
            SC_CHANGE_STATE(&scData->bondData.uiState, UI_CANCELLED);
            break;

        case UI_COMPLETE:
            /* Crossover. Ignore it */
            break;

        case UI_CANCELLED:
        default:
            CsrBtScStateEventException(CSR_BT_SC_PRIM, VOIDP_TO_MSG_TYPE(scData), scData->bondData.uiState);
            break;
    }
}


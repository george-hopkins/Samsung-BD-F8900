/****************************************************************************

Copyright Cambridge Silicon Radio Limited and its licensors 2011. All rights reserved.

REVISION:      $Revision: #6 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_bccmd.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_cm_common_amp.h"

#ifdef CSR_BT_LE_ENABLE
#include "csr_bt_cm_le.h"
#endif

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

void CsrBtCmPutMessage(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_CM_PRIM, msg);
}

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
static void csrBtCmCbInitRfcConnElement(CsrCmnListElm_t *elem)
{
    cmRfcConnInstType *connInst   = (cmRfcConnInstType *) CsrPmemZalloc(sizeof(cmRfcConnInstType));
    cmRfcConnElement  *theElement = (cmRfcConnElement *) elem;

    connInst->actualMode                      = CSR_BT_ACTIVE_MODE;
    connInst->appHandle                       = CSR_SCHED_QID_INVALID;
    connInst->lowPowerPriority                = DEFAULT_PRIORITY;
    connInst->btConnId                        = CSR_BT_CONN_ID_INVALID;
    connInst->remoteServerChan                = CSR_BT_NO_SERVER;
    connInst->requestedMode                   = CSR_BT_ACTIVE_MODE;
    connInst->serverChannel                   = CSR_BT_NO_SERVER;
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    connInst->ssrSettings.maxRemoteLatency    = CM_SSR_DISABLED;
    connInst->ssrSettings.minLocalTimeout     = CM_SSR_DISABLED;
    connInst->ssrSettings.minRemoteTimeout    = CM_SSR_DISABLED;
#endif /* CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1 */
    connInst->state                           = idle_s;
    connInst->logicalChannelTypeMask          = CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL;
#ifdef CSR_AMP_ENABLE
    connInst->controller                      = CSR_BT_AMP_CONTROLLER_BREDR;
    connInst->ampProcessState                 = CSR_BT_CM_AMP_PROCESS_IDLE;
#endif
    connInst->dataControl.dataResReceived     = TRUE;
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
    connInst->eScoParms.handle                = NO_SCO;
#endif
    connInst->pendingBtConnId                 = CSR_BT_CONN_ID_INVALID;
    connInst->modemStatusState                = CSR_BT_CM_MODEM_STATUS_IDLE;
    CsrBtPortParDefault(&connInst->portPar);

    theElement->cmRfcConnInst                 = connInst;
}

static void csrBtCmCbFreeRfcConnElementPointers(CsrCmnListElm_t *elem)
{
    cmRfcConnElement * theElement = (cmRfcConnElement *) elem;
    cleanUpConnectionTable(&(theElement->cmRfcConnInst));
}
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
static void csrBtCmCbInitL2caConnElement(CsrCmnListElm_t *elem)
{
    cmL2caConnInstType *connInst   = (cmL2caConnInstType *) CsrPmemZalloc(sizeof(cmL2caConnInstType));
    cmL2caConnElement  *theElement = (cmL2caConnElement *) elem;

    connInst->rxQueueMax                    = MAX_L2CAP_DATA_QUEUE_LENGTH;;

    connInst->txPendingContext              = 0xFFFF;

    connInst->actualMode                    = CSR_BT_ACTIVE_MODE;
    connInst->authorised                    = FALSE;
    connInst->btConnId                      = BTCONN_ID_EMPTY;
    connInst->outgoingFlush                 = L2CA_FLUSH_TO_DEFAULT;
    connInst->state                         = idle_s;
    connInst->psm                           = NO_LOCAL_PSM;
    connInst->remotePsm                     = NO_REMOTE_PSM;
    connInst->requestedMode                 = CSR_BT_ACTIVE_MODE;
#ifdef CSR_AMP_ENABLE
    connInst->controller                    = CSR_BT_AMP_CONTROLLER_BREDR;
    connInst->ampProcessState               = CSR_BT_CM_AMP_PROCESS_IDLE;
#endif
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    connInst->ssrSettings.maxRemoteLatency  = CM_SSR_DISABLED;
    connInst->ssrSettings.minLocalTimeout   = CM_SSR_DISABLED;
    connInst->ssrSettings.minRemoteTimeout  = CM_SSR_DISABLED;
#endif /* CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1 */
    connInst->logicalChannelTypeMask        = CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL;
    connInst->secLevel                      = SECL_NONE;
    connInst->dataPriority                  = CSR_BT_CM_PRIORITY_NORMAL;

    theElement->cmL2caConnInst = connInst;
}

static void csrBtCmCbFreeL2caConnElementPointers(CsrCmnListElm_t *elem)
{
    cmL2caConnElement * theElement = (cmL2caConnElement *) elem;
    CsrBtCleanUpCmL2caConnInst(&(theElement->cmL2caConnInst));
}
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

void CsrBtCmInitInstData(cmInstanceData_t *cmData)
{
    /* Initial the CM instance data */
    CsrUintFast16 i;

    cmData->globalState                     = notReady_s;

    /* Special upper-layer handlers */
    cmData->sdHandle                        = CSR_SCHED_QID_INVALID;
    cmData->scHandle                        = CSR_SCHED_QID_INVALID;
#ifdef CSR_AMP_ENABLE
    cmData->ampmHandle                      = CSR_SCHED_QID_INVALID;
#endif
#ifdef CSR_BT_LE_ENABLE
    cmData->gattHandle                      = CSR_SCHED_QID_INVALID;
#endif

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
    cmData->roleVar.doMssBeforeScoSetup     = TRUE;
#endif

    for ( i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        cmData->roleVar.aclVar[i].l2capExtendedFeatures = CM_INVALID_L2CAP_EXT_FEAT;
        cmData->roleVar.aclVar[i].logicalChannelTypeMask = CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL;
    }

    cmData->dmVar.state                                 = scNull_s;
    cmData->dmVar.dmInProgress                          = TRUE;
    cmData->dmVar.contableIndex                         = CM_ERROR;
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
    cmData->dmVar.closeScoHandle                        = NO_SCO;
#endif
    cmData->dmVar.connectAblePhandle                    = CSR_SCHED_QID_INVALID;
    cmData->dmVar.majorCod                              = CSR_BT_MAJOR_MINOR_DEVICE_CLASS & CSR_BT_CM_MAJOR_DEVICE_CLASS_FILTER;
    cmData->dmVar.minorCod                              = CSR_BT_MAJOR_MINOR_DEVICE_CLASS & CSR_BT_CM_MINOR_DEVICE_CLASS_FILTER;
    cmData->dmVar.codWrittenToChip                      = 0xffffff;
    cmData->dmVar.pendingCod                            = 0xffffff;

    /* Link policy */
    cmData->dmVar.defaultLinkPolicySettings          = (link_policy_settings_t) CSR_BT_DEFAULT_LOW_POWER_MODES;
    cmData->dmVar.defaultSniffSettings.max_interval  = CSR_BT_SNIFF_MAX_TIME_INTERVAL;
    cmData->dmVar.defaultSniffSettings.min_interval  = CSR_BT_SNIFF_MIN_TIME_INTERVAL;
    cmData->dmVar.defaultSniffSettings.attempt       = CSR_BT_SNIFF_ATTEMPT;
    cmData->dmVar.defaultSniffSettings.timeout       = CSR_BT_SNIFF_TIMEOUT;
    cmData->dmVar.defaultParkSettings.max_interval   = CSR_BT_PARK_MAX_TIME_INTERVAL;
    cmData->dmVar.defaultParkSettings.min_interval   = CSR_BT_PARK_MIN_TIME_INTERVAL;
    cmData->dmVar.inquiryTxPowerLevel                = CSR_BT_SD_DEFAULT_INQUIRY_TX_POWER_LEVEL;
    cmData->dmVar.lowInquiryPriorityLevel            = CSR_BT_CM_DEFAULT_INQUIRY_LEVEL;
    CsrBtCmDmSmClearRebondData(cmData);

    for (i = 0; i < MAX_PCM_SLOTS; i++)
    {
        cmData->dmVar.pcmAllocationTable[i] = NO_SCO;
    }

    /* Inquiry */
    cmData->dmVar.inquiryTxPowerLevel       = CSR_BT_SD_DEFAULT_INQUIRY_TX_POWER_LEVEL;
    cmData->dmVar.inquiryAccessCode         = CSR_BT_CM_ACCESS_CODE_GIAC;

    /* Set default page and inquiry scan settings and type */
    cmData->dmVar.pagescanInterval          = CSR_BT_PAGE_SCAN_INTERVAL_DEFAULT;   /* Should be 0x0800 according to spec 1.2 */
    cmData->dmVar.pagescanWindow            = CSR_BT_PAGE_SCAN_WINDOW_DEFAULT;     /* Should be 0x0012 according to spec 1.2 */
    cmData->dmVar.pagescanType              = CSR_BT_PAGE_SCAN_TYPE_DEFAULT;
    cmData->dmVar.inquiryscanInterval       = HCI_INQUIRYSCAN_INTERVAL_DEFAULT;   /* Should be 0x1000 according to spec 1.2 */
    cmData->dmVar.inquiryscanWindow         = HCI_INQUIRYSCAN_WINDOW_DEFAULT;     /* Should be 0x0012 according to spec 1.2 */
    cmData->dmVar.inquiryscanType           = CSR_BT_INQUIRY_SCAN_TYPE_DEFAULT;

    /* Assume that all features are supported, but set version
     * conservatively */
    CsrMemSet(&cmData->dmVar.lmpSuppFeatures, 0xFF, 8);

    cmData->dmVar.scanTime                  = SCAN_TIMER_DEFAULT;

    cmData->smVar.smInProgress              = TRUE;
    cmData->smVar.registeringSrvChannel     = CSR_BT_CM_SERVER_CHANNEL_DONT_CARE;

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    cmData->rfcVar.btConnId                     = CSR_BT_CONN_ID_INVALID;
    CsrCmnListInit(&(cmData->rfcVar.connList), 0, csrBtCmCbInitRfcConnElement, csrBtCmCbFreeRfcConnElementPointers);

#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    CsrCmnListInit(&(cmData->l2caVar.connList), 0, csrBtCmCbInitL2caConnElement, csrBtCmCbFreeL2caConnElementPointers);
#ifdef CSR_BT_INSTALL_L2CAP_CONNLESS_SUPPORT
    CsrBtCmL2caConnlessInit(cmData);
#endif
    CsrBtCmScCleanupVar(cmData);
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

    CsrCmnListInit(&cmData->remoteVersion, 0, NULL, NULL);

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    cmData->bnepVar.roleSwitchReqIndex          = CM_ERROR;

    for ( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++)
    {
        cmData->bnepVar.connectVar[i].actualMode        = CSR_BT_ACTIVE_MODE;
        cmData->bnepVar.connectVar[i].id                = ID_EMPTY;
        cmData->bnepVar.connectVar[i].requestedMode     = CSR_BT_ACTIVE_MODE;

        cmData->bnepVar.connectVar[i].logicalChannelTypeMask  = CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL;
    }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

}

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
static CsrUint8 numOfConnectionToPeerDeviceRfc(cmInstanceData_t *cmData, CsrBtDeviceAddr *theAddr)
{
    cmRfcConnElement *currentElem;
    CsrUint8 count   = 0;

    for (currentElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentElem; currentElem = currentElem->next)
    { /* Search through the RFC connection table */
        if (currentElem->cmRfcConnInst)
        {
            if(currentElem->cmRfcConnInst->state == connected_s)
            {
                if (bd_addr_eq(theAddr, &(currentElem->cmRfcConnInst->deviceAddr)))
                {
                    count++;
                }
            }
        }
    }
    return count;
}
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
static CsrUint8 numOfConnectionToPeerDeviceL2cap(cmInstanceData_t *cmData, CsrBtDeviceAddr *theAddr)
{
    cmL2caConnElement *currentElem;
    CsrUint8           count = 0;

    for (currentElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList); currentElem; currentElem = currentElem->next)
    { /* Search through the L2CAP connection list */
        if (currentElem->cmL2caConnInst)
        {
            if (currentElem->cmL2caConnInst->state == l2capConnected_s)
            {
                if (bd_addr_eq(theAddr, &(currentElem->cmL2caConnInst->deviceAddr)))
                {
                    count++;
                }
            }
        }
    }
    return count;
}
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
static CsrUint8 numOfConnectionToPeerDeviceBnep(cmInstanceData_t *cmData, CsrBtDeviceAddr *theAddr)
{
    CsrUintFast8 i     = 0;
    CsrUint8 count = 0;

    for (i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++)
    {
        if ((cmData->bnepVar.connectVar[i].state == bnepConnected_s))
        {
            if (bd_addr_eq(theAddr, &(cmData->bnepVar.connectVar[i].deviceAddr)))
            {
                count++;
            }
        }
    }
    return count;
}
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

CsrUint8 CsrBtCmReturnNumOfConnectionToPeerDeviceFlags(cmInstanceData_t *cmData, CsrBtDeviceAddr *theAddr, CsrUint32 flags)
{
    CsrUint8 count = 0;

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    if (flags & CSR_BT_CM_ACL_DETACH_EXCLUDE_RFC)
    {
        count += numOfConnectionToPeerDeviceRfc(cmData, theAddr);
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    if (flags & CSR_BT_CM_ACL_DETACH_EXCLUDE_L2CAP)
    {
        count += numOfConnectionToPeerDeviceL2cap(cmData, theAddr);
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    if (flags & CSR_BT_CM_ACL_DETACH_EXCLUDE_BNEP)
    {
        count += numOfConnectionToPeerDeviceBnep(cmData, theAddr);
    }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

#ifdef CSR_BT_LE_ENABLE
    if (flags & CSR_BT_CM_ACL_DETACH_EXCLUDE_LE)
    {
        aclTable *acl = NULL;
        if (returnAclConnectionElement(cmData, *theAddr, &acl) != CM_ERROR)
        {
            if (acl->gattConnectionActive)
            {
                count += 1;
            }
        }
    }
#endif

    return count;
}

CsrUint8 CsrBtCmReturnNumOfConnectionsToPeerDevice(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr)
{
    return CsrBtCmReturnNumOfConnectionToPeerDeviceFlags(cmData, &theAddr, CSR_BT_CM_ACL_DETACH_EXCLUDE_ALL);
}

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
static CsrUint8 returnIndexPlayer(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr, CsrUint8 requestedMode)
{
    CsrUintFast8 i =0;

    for ( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    {
        if(cmData->bnepVar.connectVar[i].state == bnepConnected_s)
        {
            if( bd_addr_eq( &(theAddr), &(cmData->bnepVar.connectVar[i].deviceAddr) ) )
            {
                if(cmData->bnepVar.connectVar[i].requestedMode == requestedMode)
                {
                    return (CsrUint8)i;
                }
            }
        }
    }
    return CM_ERROR;
}
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

#ifdef CSR_AMP_ENABLE
CsrUint8 CsrBtCmBtControllerActive(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr)
{
    CsrUint8 result;
    result = CTRL_ACTIVE_INACTIVE;

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    if (numOfConnectionToPeerDeviceBnep(cmData, &deviceAddr) > 0)
    {
        /* At least one bnep connection is connected. BNEP can not use
         * AMP at the moment, so this means BR/EDR is active */
        return CTRL_ACTIVE_BREDR_ACTIVE;
    }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */


#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    {
        cmRfcConnElement *rfcElm;
        for(rfcElm = CM_RFC_GET_FIRST(cmData->rfcVar.connList);
            rfcElm;
            rfcElm = rfcElm->next)
        {
            if(rfcElm->cmRfcConnInst)
            {
                cmRfcConnInstType *conn;
                conn = rfcElm->cmRfcConnInst;
                if(CsrBtBdAddrEq(&conn->deviceAddr, &deviceAddr))
                {
                    if(conn->state == connected_s)
                    {
                        if(conn->controller == CSR_BT_AMP_CONTROLLER_BREDR)
                        {
                            /* At least one RFCOMM connection using the BT controller */
                            return CTRL_ACTIVE_BREDR_ACTIVE;
                        }
                        else
                        {
                            /* We should not really have to continue
                             * scanning as RFCOMM uses just one L2CAP
                             * channel. Oh well... */
                            result = CTRL_ACTIVE_AMP_ONLY;
                        }
                    }
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    {
        cmL2caConnElement *currentElem;
        for (currentElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList);
             currentElem;
             currentElem = currentElem->next)
        {
            /* Search through the L2CAP connection list */
            if (currentElem->cmL2caConnInst)
            {
                cmL2caConnInstType *conn = currentElem->cmL2caConnInst;
                
                if(bd_addr_eq(&(deviceAddr), &(conn->deviceAddr)))
                {
                    if(conn->state == l2capConnected_s)
                    {
                        if (conn->controller == CSR_BT_AMP_CONTROLLER_BREDR)
                        {
                            /* At least one L2CAP connection using the BT controller */
                            return CTRL_ACTIVE_BREDR_ACTIVE;
                        }
                        else
                        {
                            /* AMP is active, but keep scanning list
                             * to see if there is any BR/EDR
                             * activity */
                            result = CTRL_ACTIVE_AMP_ONLY;
                        }
                    }
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

    return result;
}
#endif /* CSR_AMP_ENABLE */

void CsrBtCmModeChangeReqMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr)
{
    aclTable *aclConnectionElement;

    returnAclConnectionElement(cmData, theAddr, &aclConnectionElement);

    if (aclConnectionElement && !aclConnectionElement->appControlsLowPower)
    {
#ifdef CSR_AMP_ENABLE
        if (CsrBtCmBtControllerActive(cmData, theAddr) == CTRL_ACTIVE_AMP_ONLY)
        {
            /* All connections are using AMP. Try to force it into
             * sniff */
            CsrBtCmAmpForceModeChange(cmData, &theAddr);
        }
        else
#endif /* CSR_AMP_ENABLE */
        {
            CsrUint8 requestedMode = CsrBtCmDmReturnRequestedMode(cmData, theAddr);
            if(requestedMode != CSR_BT_ACTIVE_MODE)
            { /* The requested link policy in different from active. Change the
                 link policy to the requested mode */
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
                cmRfcConnElement *theElement;

                theElement = CsrBtCmRfcFindRfcConnElementFromDeviceAddrStateRequestedMode(cmData, &theAddr, 
                                                                                          connected_s, requestedMode);

                if (theElement)
                { /* An application that is attach with the RFC module demand a
                     new link policy, after the current link is released */
                    cmRfcConnInstType * theLogicalLink = theElement->cmRfcConnInst;
                    CsrBtCmRfcModeChangeReqSend(theLogicalLink->btConnId, requestedMode);
                }
                else
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
                {
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
                    cmL2caConnElement *theElement;

                    theElement = CsrBtCmRfcFindRfcConnElementFromDeviceAddrConnectedSRequestedMode(cmData, 
                                                                                                   &theAddr, 
                                                                                                   requestedMode);

                    if (theElement)
                    { /* An application that is attach with the L2CA module demand a
                        new link policy, after the current link is released */
                        CsrBtCml2caModeChangeReqSend(theElement->cmL2caConnInst->btConnId, requestedMode);
                    }
                    else
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */
                    {
#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
                        CsrUint8 theIndex = returnIndexPlayer(cmData, theAddr, requestedMode);

                        if(theIndex != CM_ERROR)
                        { /* An application that is attach with the BNEP module demand a
                             new link policy, after the current link is released */
                            CsrBtCmBnepModeChangeReqSend(cmData->bnepVar.connectVar[theIndex].id, requestedMode);
                        }
                        else
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */
                        {
#ifdef CSR_BT_LE_ENABLE
                            if (aclConnectionElement->gattConnectionActive)
                            {
                                CsrBtCmDmModeChangeReqMsgSend(LE_PLAYER,
                                                          0, /* index, not used */
                                                          CSR_BT_SNIFF_MODE,
                                                          theAddr);
                            }
#endif
                        }

                    }
                }
            }
        }
    }
    else
    { /* The application is controlling low power, just ignore this message */
        ;
    }
}

CsrUint24 CsrBtCmReturnClassOfdevice(cmInstanceData_t *cmData)
{ /* This function return the Class of device */
    CsrUint24    classOfDevice = 0x000000;

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    cmL2caConnElement *currentL2caElem;
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    cmRfcConnElement *currentRfcElem;

    for (currentRfcElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList);
         currentRfcElem; 
         currentRfcElem = currentRfcElem->next)
    { /* Goes through RFC connection table to find all CSR_BT_CONN_ID_INVALID and adds COD for them */
        if (currentRfcElem->cmRfcConnInst)
        {
            if(currentRfcElem->cmRfcConnInst->btConnId == CSR_BT_CONN_ID_INVALID)
            {
                classOfDevice |= currentRfcElem->cmRfcConnInst->classOfDevice;
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    for (currentL2caElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList); 
         currentL2caElem; 
         currentL2caElem = currentL2caElem->next)
    { /* Search through the L2CAP connection list to find all BTCONN_ID_RESERVED and
         adds COD for them */
        if (currentL2caElem->cmL2caConnInst)
        {
            if(currentL2caElem->cmL2caConnInst->btConnId == BTCONN_ID_RESERVED)
            {
                classOfDevice |= currentL2caElem->cmL2caConnInst->classOfDevice;
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    classOfDevice |= cmData->bnepVar.classOfDevice;
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */
    classOfDevice |= cmData->dmVar.serviceCod;
    classOfDevice |= cmData->dmVar.liacCod;
    classOfDevice  = (classOfDevice & CSR_BT_CM_SERVICE_CLASS_FILTER);
    classOfDevice  = (classOfDevice | (cmData->dmVar.majorCod & CSR_BT_CM_MAJOR_DEVICE_CLASS_FILTER));
    classOfDevice  = (classOfDevice | (cmData->dmVar.minorCod & CSR_BT_CM_MINOR_DEVICE_CLASS_FILTER));
    return classOfDevice;
}

link_policy_settings_t CsrBtCmEnableMSSwitchParameter(cmInstanceData_t *cmData)
{
    if (cmData->roleVar.alwaysSupportMasterRole)
    {
        return (0);
    }
    else
    {
        CsrUint8 numOfAcl = returnNumOfAclConnection(cmData);

        if (numOfAcl > 1)
        {
            return (0);
        }
        else
        {
            return (ENABLE_MS_SWITCH);
        }
    }
}


#ifdef CSR_LOG_ENABLE
void CsrBtCmChangePlayer(char *file, CsrUint32 lineno, CsrUint8 *player, 
                         char *playerName, CsrUint8 newPlayer, char *newPlayerName)
{
    CsrLogStateTransition((CSR_LOG_STATE_TRANSITION_MASK_FSM_NAME |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE_STR |
                           CSR_LOG_STATE_TRANSITION_MASK_PREV_STATE),
                          0,
                          playerName,
                          *player,
                          NULL,
                          0,
                          NULL,
                          newPlayer,
                          newPlayerName,
                          lineno,
                          file);

    *player = newPlayer;
}
#endif

void CsrBtCmDmLockQueue(cmInstanceData_t *cmData)
{
    CsrPrim         *primPtr;

    primPtr = (CsrPrim *) cmData->recvMsgP;

    cmData->dmVar.dmInProgress    = TRUE;
    cmData->dmVar.dmMsgInProgress = *primPtr;

#ifdef CSR_LOG_ENABLE
    CSR_LOG_TEXT("Lock DM");
#endif
}

void CsrBtCmDmUnlockQueue(cmInstanceData_t *cmData)
{
    cmData->dmVar.dmInProgress = FALSE;

#ifdef CSR_LOG_ENABLE
    CSR_LOG_TEXT("Unlock DM");
#endif
}

void CsrBtCmSmLockQueue(cmInstanceData_t *cmData)
{
    CsrPrim         *primPtr;

    primPtr = (CsrPrim *) cmData->recvMsgP;

    cmData->smVar.smInProgress        = TRUE;
    cmData->smVar.smMsgTypeInProgress = *primPtr;

#ifdef CSR_LOG_ENABLE
    CSR_LOG_TEXT("Lock SM");
#endif
}

void CsrBtCmSmUnlockQueue(cmInstanceData_t *cmData)
{
    cmData->smVar.smInProgress = FALSE;

#ifdef CSR_LOG_ENABLE
    CSR_LOG_TEXT("Unlock SM");
#endif
}

CsrBool updateLogicalChannelTypeMaskAndNumber(cmInstanceData_t *cmData, CsrBtDeviceAddr *theAddr)
{
    CsrUint8    count = 0;
    CsrUintFast8    i;
    aclTable    *aclConnectionElement = NULL;
    CsrBtLogicalChannelType    logicalChannelTypeMaskSum = CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL;
    CsrBool     needUpdate = FALSE;
#if !defined(EXCLUDE_CSR_BT_RFC_MODULE) || !defined(EXCLUDE_CSR_BT_BNEP_MODULE)
    CsrBtLogicalChannelType    checkMask = CSR_BT_ACTIVE_CONTROL_CHANNEL | CSR_BT_ACTIVE_DATA_CHANNEL;
    CsrBool     stopSearch = FALSE;
#endif

    /* Start by finding the proper ACL link table */
    for ( i = 0; ((i < NUM_OF_ACL_CONNECTION)  && (aclConnectionElement == NULL)); i++)
    {
        if( bd_addr_eq(theAddr, &(cmData->roleVar.aclVar[i].deviceAddr)))
        {
            /* Found */
            aclConnectionElement = &(cmData->roleVar.aclVar[i]);
        }
    }

    if (aclConnectionElement)
    {
#ifndef  EXCLUDE_CSR_BT_L2CA_MODULE
        /* Now check all the L2CAP connections to the given bd address */
        cmL2caConnElement *currentElem;

        for (currentElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList);
             currentElem; 
             currentElem = currentElem->next)
        { /* Search through the L2CAP connection list */
            if (currentElem->cmL2caConnInst)
            {
                if ((currentElem->cmL2caConnInst->state == l2capConnected_s) &&
                    (bd_addr_eq(theAddr, &(currentElem->cmL2caConnInst->deviceAddr))) )
                {
                    logicalChannelTypeMaskSum |= currentElem->cmL2caConnInst->logicalChannelTypeMask;

                    if (currentElem->cmL2caConnInst->logicalChannelTypeMask == CSR_BT_ACTIVE_STREAM_CHANNEL)
                    {
                        /* A2DP stream channels are only possible in L2CAP connections */
                        count++;
                    }
                }
            }
        }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_RFC_MODULE

        {
            /* Check all the RFCOMM connections to the given bd address unless control and data channels already found */
            cmRfcConnElement *currentElement;
            stopSearch = ((logicalChannelTypeMaskSum & checkMask) == checkMask);

            for (currentElement = CM_RFC_GET_FIRST(cmData->rfcVar.connList);
                 (currentElement && !stopSearch);
                 currentElement = currentElement->next)
            {
                if (currentElement->cmRfcConnInst)
                {
                    if((connected_s == currentElement->cmRfcConnInst->state) &&
                       (bd_addr_eq(theAddr, &(currentElement->cmRfcConnInst->deviceAddr))))
                    {
                        logicalChannelTypeMaskSum |= currentElement->cmRfcConnInst->logicalChannelTypeMask;
                        stopSearch = ((logicalChannelTypeMaskSum & checkMask) == checkMask);
                    }
                }
            }
        }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE

        {/* If data channel type already active, don't bother running through the BNEP table... */
            bnepTable *bnepEntry;
            stopSearch = ((logicalChannelTypeMaskSum & checkMask) == CSR_BT_ACTIVE_DATA_CHANNEL);
            for ( i = 0; ((i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS) && !stopSearch); i++)
            {
                bnepEntry = &cmData->bnepVar.connectVar[i];
                if (bnepEntry->state == bnepConnected_s &&
                     bd_addr_eq(theAddr, &(bnepEntry->deviceAddr)))
                {
                    logicalChannelTypeMaskSum |= bnepEntry->logicalChannelTypeMask;
                    stopSearch = ((logicalChannelTypeMaskSum & checkMask) == CSR_BT_ACTIVE_DATA_CHANNEL);
                }
            }

        }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

        if ((aclConnectionElement->logicalChannelTypeMask != logicalChannelTypeMaskSum) ||
            (aclConnectionElement->noOfGuaranteedLogicalChannels != count))
        {
            /* Need to update table and send event */
            aclConnectionElement->logicalChannelTypeMask        = logicalChannelTypeMaskSum;
            aclConnectionElement->noOfGuaranteedLogicalChannels = count;
            needUpdate = TRUE;
        }
    }
    /* else..... not possible at all; there must be an ACL connection
     * to the device address if there is an L2CAP or RFCOMM
     * connection! */

    return needUpdate;
}

void CsrBtCmDataBufferEmptyCfmSend(CsrSchedQid appHandle, CsrUint16 context)
{
    CsrBtCmDataBufferEmptyCfm *prim = (CsrBtCmDataBufferEmptyCfm *)
                                       CsrPmemAlloc(sizeof(CsrBtCmDataBufferEmptyCfm));


    prim->type      = CSR_BT_CM_DATA_BUFFER_EMPTY_CFM;
    prim->context   = context;
    CsrBtCmPutMessage(appHandle, prim);
}

void CsrBtCmDataBufferEmptyReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmDataBufferEmptyReq *prim = (CsrBtCmDataBufferEmptyReq *) cmData->recvMsgP;

    if (CSR_BT_CONN_ID_IS_L2CA(prim->btConnId))
    {
#ifndef  EXCLUDE_CSR_BT_L2CA_MODULE
        cmL2caConnElement *connElem = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &prim->btConnId);

        if (connElem)
        {
            cmL2caConnInstType *l2capConn = connElem->cmL2caConnInst;

            if (l2capConn->txCount > 0)
            { /* There are still data elements pending in the L2CAP buffer */
                l2capConn->pendingBufferStatus = TRUE;
            }
            else
            { /* There are no data elements in the L2CAP buffer */
                CsrBtCmDataBufferEmptyCfmSend(l2capConn->appHandle, l2capConn->context);
            }
        }
#else
        /* Could not identify the btConnId, just ignore */
        ;
#endif
    }
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    else if (CSR_BT_CONN_ID_IS_RFC(prim->btConnId))
    {
        cmRfcConnElement *connElem  = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(prim->btConnId));
        if (connElem)
        {
            cmRfcConnInstType *rfcConn = connElem->cmRfcConnInst;

            if (rfcConn->dataControl.txCount > 0)
            { /* There are still data elements pending in the L2CAP buffer */
                rfcConn->dataControl.pendingBufferStatus = TRUE;
            }
            else
            { /* There are no data elements in the L2CAP buffer */
                CsrBtCmDataBufferEmptyCfmSend(rfcConn->appHandle, rfcConn->context);
            }
        }
    }
#endif
    else
    { /* Could not identify the btConnId, just ignore */
        ;
    }
}

/* Register special upper-layer handler */
void CsrBtCmRegisterHandlerReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmRegisterHandlerReq *prim;
    prim = (CsrBtCmRegisterHandlerReq*)cmData->recvMsgP;
    
    switch(prim->handlerType)
    {
        case CSR_BT_CM_HANDLER_TYPE_SD:
            cmData->sdHandle = prim->handle;
            break;

        case CSR_BT_CM_HANDLER_TYPE_SC:
            cmData->scHandle = prim->handle;
            break;

#ifdef CSR_AMP_ENABLE
        case CSR_BT_CM_HANDLER_TYPE_AMP:
            cmData->ampmHandle = prim->handle;
            break;
#endif

#ifdef CSR_BT_LE_ENABLE
        case CSR_BT_CM_HANDLER_TYPE_LE:
            cmData->gattHandle = prim->handle;
            break;
#endif

        default:
            /* invalid handler type - ignore */
            break;
    }
}

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
#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_dm.h"

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1 

static void CsrBtCmCheckSsrReq(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr)
{
    CsrBool       enterSsr                    = TRUE;
    CsrUint16     currentMaxRemoteLatency     = SSR_MAX_LATENCY_DONT_CARE;
    CsrUint16     currentMinRemoteTimeout     = 0;
    CsrUint16     currentMinLocalTimeout      = 0;
    CsrBtDeviceAddr *ssrDeviceAddr              = NULL;
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    cmL2caConnElement *currentL2caElem;
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    CsrUintFast8 i;
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    cmRfcConnElement *currentRfcElem;

    for (currentRfcElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentRfcElem; currentRfcElem = currentRfcElem->next)
    { /* Search through the RFC connection list                     */
        if (currentRfcElem->cmRfcConnInst)
        {
            if(currentRfcElem->cmRfcConnInst->state == connected_s)
            { /* The application has a RFCOMM connection */
                if(bd_addr_eq(&(deviceAddr), &(currentRfcElem->cmRfcConnInst->deviceAddr)))
                { /* The given device address is right. */
                    cmRfcConnInstType *theLogicalLink   = currentRfcElem->cmRfcConnInst;

                    if(theLogicalLink->actualMode == CSR_BT_SNIFF_MODE)
                    {
                        if(theLogicalLink->ssrAccepted == TRUE)
                        {
                            /* SSR settings registered, find the set of parameters that will satisfy the */
                            /* requirements for all connections on between these two devices */
                            if(theLogicalLink->ssrSettings.maxRemoteLatency < currentMaxRemoteLatency)
                            {
                                currentMaxRemoteLatency = theLogicalLink->ssrSettings.maxRemoteLatency;
                            }

                            if(theLogicalLink->ssrSettings.minRemoteTimeout > currentMinRemoteTimeout &&
                               theLogicalLink->ssrSettings.minRemoteTimeout < SSR_MIN_REMOTE_TIMEOUT_DONT_CARE)
                            {
                                currentMinRemoteTimeout = theLogicalLink->ssrSettings.minRemoteTimeout;
                            }

                            if(theLogicalLink->ssrSettings.minLocalTimeout > currentMinLocalTimeout &&
                               theLogicalLink->ssrSettings.minLocalTimeout < SSR_MIN_LOCAL_TIMEOUT_DONT_CARE)
                            {
                                currentMinLocalTimeout = theLogicalLink->ssrSettings.minLocalTimeout;
                            }
                            ssrDeviceAddr = &(theLogicalLink->deviceAddr);
                        }
                        else
                        {
                            /* connection which does not want SSR exist */
                            enterSsr = FALSE;
                        }
                    }
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    for (currentL2caElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList); currentL2caElem; currentL2caElem = currentL2caElem->next)
    { /* Search through the L2CAP connection list                     */
        if (currentL2caElem->cmL2caConnInst)
        {
            if(currentL2caElem->cmL2caConnInst->state == l2capConnected_s)
            {/* The application has a valid L2CAP connection */
                if(bd_addr_eq(&(deviceAddr), &(currentL2caElem->cmL2caConnInst->deviceAddr)))
                { /* The given device address is right. */
                    cmL2caConnInstType *l2CaConnection = currentL2caElem->cmL2caConnInst;

                    if(l2CaConnection->actualMode == CSR_BT_SNIFF_MODE)
                    {
                        if(l2CaConnection->ssrAccepted == TRUE)
                        {
                            /* SSR desired find the set of parameters that will satisfy the */
                            /* requirements for all connections on between these two devices */
                            if(l2CaConnection->ssrSettings.maxRemoteLatency < currentMaxRemoteLatency)
                            {
                                currentMaxRemoteLatency = l2CaConnection->ssrSettings.maxRemoteLatency;
                            }

                            if(l2CaConnection->ssrSettings.minRemoteTimeout > currentMinRemoteTimeout)
                            {
                                currentMinRemoteTimeout = l2CaConnection->ssrSettings.minRemoteTimeout;
                            }

                            if(l2CaConnection->ssrSettings.minLocalTimeout > currentMinLocalTimeout)
                            {
                                currentMinLocalTimeout = l2CaConnection->ssrSettings.minLocalTimeout;
                            }
                            ssrDeviceAddr = &(l2CaConnection->deviceAddr);
                        }
                        else
                        {
                            /* connection which does not want SSR exist */
                            enterSsr = FALSE;
                        }
                    }
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
   for( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    { /* Search through the BNEP connection table */

       if( cmData->bnepVar.connectVar[i].state == bnepConnected_s)
        {
            if(bd_addr_eq(&(deviceAddr), &(cmData->bnepVar.connectVar[i].deviceAddr)))
            { /* Thie given address is right. Store the actual link policy in the connection table */
                bnepTable *bnepConnection;

                bnepConnection    = &(cmData->bnepVar.connectVar[i]);

                if(bnepConnection->actualMode == CSR_BT_SNIFF_MODE)
                {
                    if(bnepConnection->ssrAccepted == TRUE)
                    {
                        /* SSR desired find the set of parameters that will satisfy the */
                        /* requirements for all connections on between these two devices */
                        if(bnepConnection->ssrSettings.maxRemoteLatency < currentMaxRemoteLatency)
                        {
                            currentMaxRemoteLatency = bnepConnection->ssrSettings.maxRemoteLatency;
                        }

                        if(bnepConnection->ssrSettings.minRemoteTimeout > currentMinRemoteTimeout)
                        {
                            currentMinRemoteTimeout = bnepConnection->ssrSettings.minRemoteTimeout;
                        }

                        if(bnepConnection->ssrSettings.minLocalTimeout > currentMinLocalTimeout)
                        {
                            currentMinLocalTimeout = bnepConnection->ssrSettings.minLocalTimeout;
                        }
                        ssrDeviceAddr = &(bnepConnection->deviceAddr);
                    }
                    else
                    {
                        /* connection which does not want SSR exist */
                        enterSsr = FALSE;
                    }
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

    if (enterSsr == TRUE && currentMaxRemoteLatency < SSR_MAX_LATENCY_DONT_CARE)
    {
        dm_hci_sniff_sub_rate(ssrDeviceAddr,
                              currentMaxRemoteLatency,
                              currentMinRemoteTimeout,
                              currentMinLocalTimeout,
                              NULL);
    }
    else
    {
        dm_hci_sniff_sub_rate(&deviceAddr,
                              CM_SSR_DISABLED,
                              CM_SSR_DISABLED,
                              CM_SSR_DISABLED,
                              NULL);
    }
}




void CsrBtCmDmCheckSsrReqSend(CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmDmCheckSsrReq        *prim;

    prim    = (CsrBtCmDmCheckSsrReq *)CsrPmemAlloc(sizeof(CsrBtCmDmCheckSsrReq));

    prim->type         = CSR_BT_CM_DM_CHECK_SSR_REQ;
    prim->deviceAddr   = deviceAddr;

    CsrBtCmPutMessage(CSR_BT_CM_IFACEQUEUE, prim);
}

void CsrBtCmDmCheckSsrReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmDmCheckSsrReq        *prim;

    prim                        = (CsrBtCmDmCheckSsrReq *) cmData->recvMsgP;

    CsrBtCmCheckSsrReq(cmData, prim->deviceAddr);
}

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
#ifdef CSR_BT_INSTALL_CM_PRI_MODE_SETTINGS
void CsrBtCmDmModeSettingsReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmDmModeSettingsReq * cmPrim = (CsrBtCmDmModeSettingsReq *) cmData->recvMsgP;
    CsrBool  ssrRequested               = FALSE;
    cmRfcConnElement * theElement    = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(cmPrim->btConnId));

    if(theElement)
    {
        cmRfcConnInstType *theLogicalLink = theElement->cmRfcConnInst;

        if(theLogicalLink->state == connected_s)
        { /* The link is in a state where it is allowed to proceed with the
             request. The settings are stored in the l2ca connection table  */
            theLogicalLink->lowPowerPriority = cmPrim->lowPowerPriority;

            if (cmPrim->sniffSettings)
            { /* Change the sniff settings                                  */
                theLogicalLink->sniffSettings = *cmPrim->sniffSettings;
            }
            else
            { /* Keep the sniff settings                                    */
                ;
            }

            if (cmPrim->parkSettings)
            { /* Change the park settings                                   */
                theLogicalLink->parkSettings = *cmPrim->parkSettings;
            }
            else
            { /* Keep the park settings                                     */
                ;
            }

            if (cmData->dmVar.lmpVersion >= CSR_BT_BLUETOOTH_VERSION_2P1)
            {
                if (cmPrim->ssrSettings)
                {
                    theLogicalLink->ssrSettings = *cmPrim->ssrSettings;
                    theLogicalLink->ssrAccepted = TRUE;

                    if(theLogicalLink->actualMode == CSR_BT_SNIFF_MODE)
                    {
                        ssrRequested = TRUE;
                        CsrBtCmCheckSsrReq(cmData, theLogicalLink->deviceAddr);
                    }
                }
                else
                {
                    theLogicalLink->ssrAccepted = FALSE;
                    theLogicalLink->ssrSettings.maxRemoteLatency = CM_SSR_DISABLED;
                    theLogicalLink->ssrSettings.minRemoteTimeout = CM_SSR_DISABLED;
                    theLogicalLink->ssrSettings.minLocalTimeout = CM_SSR_DISABLED;
                }
            }
            else
            {
                theLogicalLink->ssrAccepted = FALSE;
                theLogicalLink->ssrSettings.maxRemoteLatency = CM_SSR_DISABLED;
                theLogicalLink->ssrSettings.minRemoteTimeout = CM_SSR_DISABLED;
                theLogicalLink->ssrSettings.minLocalTimeout = CM_SSR_DISABLED;
            }
        }
        else
        { /* The application is not allowed to change the settings without a connection*/
            ;
        }
    }
    else
    { /* This signal does not reserve any queues hence no restoring is needed */
        ;
    }

    if (ssrRequested == FALSE)
    { /* Restore the DM queue */
        CsrBtCmDmLocalQueueHandler();
    }

    CsrPmemFree(cmPrim->parkSettings);
    CsrPmemFree(cmPrim->sniffSettings);
    CsrPmemFree(cmPrim->ssrSettings);
}
#endif
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
#ifdef CSR_BT_INSTALL_CM_PRI_MODE_SETTINGS
void CsrBtCmDmL2caModeSettingsReqHandler(cmInstanceData_t *cmData)
{
    CsrBool  ssrRequested                 = FALSE;
    CsrBtCmDmL2caModeSettingsReq *cmPrim = (CsrBtCmDmL2caModeSettingsReq *) cmData->recvMsgP;
    cmL2caConnElement * theElement       = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &(cmPrim->btConnId));

    if (theElement)
    {
        cmL2caConnInstType *l2CaConnection = theElement->cmL2caConnInst;

        if(l2CaConnection->state == l2capConnected_s)
        { /* The link is in a state where it is allowed to proceed with the
             request. The settings are stored in the l2ca connection table  */
            l2CaConnection->lowPowerPriority = cmPrim->lowPowerPriority;

            if (cmPrim->sniffSettings)
            { /* Change the sniff settings                                  */
                l2CaConnection->sniffSettings = *cmPrim->sniffSettings;
            }
            else
            { /* Keep the sniff settings                                    */
                ;
            }

            if (cmPrim->parkSettings)
            { /* Change the park settings                                   */
                l2CaConnection->parkSettings = *cmPrim->parkSettings;
            }
            else
            { /* Keep the park settings                                     */
                ;
            }

            if (cmData->dmVar.lmpVersion >= CSR_BT_BLUETOOTH_VERSION_2P1)
            {
                if (cmPrim->ssrSettings)
                {
                    l2CaConnection->ssrSettings = *cmPrim->ssrSettings;
                    l2CaConnection->ssrAccepted = TRUE;
                    if(l2CaConnection->actualMode == CSR_BT_SNIFF_MODE)
                    {
                        ssrRequested = TRUE;
                        CsrBtCmCheckSsrReq(cmData, l2CaConnection->deviceAddr);
                    }
                }
                else
                {
                    l2CaConnection->ssrAccepted = FALSE;
                    l2CaConnection->ssrSettings.maxRemoteLatency = CM_SSR_DISABLED;
                    l2CaConnection->ssrSettings.minRemoteTimeout = CM_SSR_DISABLED;
                    l2CaConnection->ssrSettings.minLocalTimeout = CM_SSR_DISABLED;
                }
            }
            else
            {
                l2CaConnection->ssrAccepted = FALSE;
                l2CaConnection->ssrSettings.maxRemoteLatency = CM_SSR_DISABLED;
                l2CaConnection->ssrSettings.minRemoteTimeout = CM_SSR_DISABLED;
                l2CaConnection->ssrSettings.minLocalTimeout = CM_SSR_DISABLED;
            }
        }
        else
        {
        /* The application is not allowed to change the settings without a connection*/

        }
    }
    else
    { /* This signal does not reserve any queues hence no restoring is needed */

    }
    if (ssrRequested == FALSE)
    {
        /* Restore the DM queue */
        CsrBtCmDmLocalQueueHandler();
    }
    CsrPmemFree(cmPrim->parkSettings);
    CsrPmemFree(cmPrim->sniffSettings);
    CsrPmemFree(cmPrim->ssrSettings);
}
#endif
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
#ifdef CSR_BT_INSTALL_CM_PRI_MODE_SETTINGS
void CsrBtCmDmBnepModeSettingsReqHandler(cmInstanceData_t *cmData)
{
    CsrUint8 theIndex;
    CsrBool  ssrRequested = FALSE;
    CsrBtCmDmBnepModeSettingsReq    *cmPrim;

    cmPrim      = (CsrBtCmDmBnepModeSettingsReq *) cmData->recvMsgP;
    theIndex    = returnBnepIdIndex(cmData, cmPrim->id);

    if(theIndex != CM_ERROR)
    {
        bnepTable *bnepConnection;

        bnepConnection        = &(cmData->bnepVar.connectVar[theIndex]);

        if(bnepConnection->state == bnepConnected_s)
        { /* The link is in a state where it is allowed to proceed with the
             request. */
            bnepConnection->lowPowerPriority = cmPrim->lowPowerPriority;

            if (cmPrim->sniffSettings)
            { /* Change the sniff settings                                  */
                bnepConnection->sniffSettings = *cmPrim->sniffSettings;
            }
            else
            { /* Keep the sniff settings                                    */
                ;
            }

            if (cmPrim->parkSettings)
            { /* Change the park settings                                   */
                bnepConnection->parkSettings = *cmPrim->parkSettings;
            }
            else
            { /* Keep the park settings                                     */
                ;
            }

            if (cmData->dmVar.lmpVersion >= CSR_BT_BLUETOOTH_VERSION_2P1)
            {
                if (cmPrim->ssrSettings)
                {
                    bnepConnection->ssrSettings = *cmPrim->ssrSettings;
                    bnepConnection->ssrAccepted = TRUE;
                    if(bnepConnection->actualMode == CSR_BT_SNIFF_MODE)
                    {
                        ssrRequested = TRUE;
                        CsrBtCmCheckSsrReq(cmData, bnepConnection->deviceAddr);
                    }
                }
                else
                {
                    bnepConnection->ssrAccepted = FALSE;
                    bnepConnection->ssrSettings.maxRemoteLatency = CM_SSR_DISABLED;
                    bnepConnection->ssrSettings.minRemoteTimeout = CM_SSR_DISABLED;
                    bnepConnection->ssrSettings.minLocalTimeout = CM_SSR_DISABLED;
                }
            }
            else
            {
                bnepConnection->ssrAccepted = FALSE;
                bnepConnection->ssrSettings.maxRemoteLatency = CM_SSR_DISABLED;
                bnepConnection->ssrSettings.minRemoteTimeout = CM_SSR_DISABLED;
                bnepConnection->ssrSettings.minLocalTimeout = CM_SSR_DISABLED;
            }
        }
        else
        { /* The application is not allowed to change the settings without a connection */

        }
    }
    if (ssrRequested == FALSE)
    {
        /* Restore the DM queue */
        CsrBtCmDmLocalQueueHandler();
    }
    CsrPmemFree(cmPrim->parkSettings);
    CsrPmemFree(cmPrim->sniffSettings);
    CsrPmemFree(cmPrim->ssrSettings);
}
#endif
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

void CsrBtCmDmHciSniffSubRatingIndHandler(cmInstanceData_t *cmData)
{
    DM_HCI_SNIFF_SUB_RATING_IND_T *dmPrim;
    aclTable *aclConnectionElement;

    dmPrim    = (DM_HCI_SNIFF_SUB_RATING_IND_T *) cmData->recvMsgP;

    returnAclConnectionElement(cmData, dmPrim->bd_addr, &aclConnectionElement);

    if (aclConnectionElement && dmPrim->status == HCI_SUCCESS)
    {
        aclConnectionElement->curSsrSettings.valid            = TRUE;
        aclConnectionElement->curSsrSettings.maxTxLatency     = dmPrim->transmit_latency;
        aclConnectionElement->curSsrSettings.maxRxLatency     = dmPrim->receive_latency;
        aclConnectionElement->curSsrSettings.minRemoteTimeout = dmPrim->remote_timeout;
        aclConnectionElement->curSsrSettings.minLocalTimeout  = dmPrim->local_timeout;
    }
}

#ifdef CSR_BT_INSTALL_CM_PRI_MODE_SETTINGS
void CsrBtCmDmHciSniffSubRateCompleteHandler(cmInstanceData_t *cmData)
{
    /* Restore the DM queue */
    CsrBtCmDmLocalQueueHandler();
}
#endif
#endif


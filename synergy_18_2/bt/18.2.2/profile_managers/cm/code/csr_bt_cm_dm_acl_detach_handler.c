/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_l2cap.h"

static void csrBtCmAclDetachComplete(cmInstanceData_t *cmData,
                                     CsrSchedQid phandle,
                                     CsrBtResultCode resultCode,
                                     CsrBtSupplier resultSupplier)
{
    if((phandle != CSR_BT_CM_IFACEQUEUE) &&
       (phandle != 0))
    {
        /* Send confirm if handle is valid */
        CsrBtCmAclDetachCfm * prim;
        prim                    = (CsrBtCmAclDetachCfm *)CsrPmemAlloc(sizeof(CsrBtCmAclDetachCfm));
        prim->type              = CSR_BT_CM_ACL_DETACH_CFM;
        prim->resultCode        = resultCode;
        prim->resultSupplier    = resultSupplier;
        CsrBtCmPutMessage(phandle, prim);
    }

    /* Clean up instance data and restore queue */
    cmData->dmVar.dmMsgInProgress = 0;
    cmData->dmVar.appHandle = 0;
    CsrMemSet(&cmData->dmVar.detachAddr, 0, sizeof(cmData->dmVar.detachAddr));
    CsrBtCmDmRestoreQueueHandler(cmData);
}

static void csrBtCmAclMarkAllDetach(cmInstanceData_t *cmData, CsrUint32 flags)
{
    CsrUintFast8 i;

    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if (!CsrBtBdAddrEqZero(&(cmData->roleVar.aclVar[i].deviceAddr)))
        {
            if (CsrBtCmReturnNumOfConnectionToPeerDeviceFlags(cmData, &cmData->roleVar.aclVar[i].deviceAddr, flags) == 0)
            {
                cmData->roleVar.aclVar[i].detachRequested = TRUE;
            }
        }
    }
}

static CsrUint8 csrBtCmAclGetNextDetach(cmInstanceData_t *cmData, aclTable **aclConnectionElement)
{
    CsrUintFast8 i;

    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if (!CsrBtBdAddrEqZero(&(cmData->roleVar.aclVar[i].deviceAddr)) &&
            cmData->roleVar.aclVar[i].detachRequested)
        {
            *aclConnectionElement = &(cmData->roleVar.aclVar[i]);
            return (CsrUint8)i;
        }
    }
    *aclConnectionElement = NULL;
    return CM_ERROR;
}

void CsrBtCmAclDetachReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmAclDetachReq *cmPrim;
    cmPrim = (CsrBtCmAclDetachReq *) cmData->recvMsgP;

    cmData->dmVar.appHandle = cmPrim->phandle;

    if(CsrBtBdAddrEqZero(&cmPrim->deviceAddr))
    {
        /* Detach all currently connected ACLs */
        aclTable *aclConnectionElement;

        csrBtCmAclMarkAllDetach(cmData, cmPrim->flags);

        if(csrBtCmAclGetNextDetach(cmData, &aclConnectionElement) != CM_ERROR)
        {
            TYPED_BD_ADDR_T ad;

            /* Target ACL found, send the detach */
            cmData->dmVar.dmMsgInProgress = CSR_BT_CM_ACL_DETACH_REQ;
            CsrBtBdAddrCopy(&cmData->dmVar.detachAddr, &aclConnectionElement->deviceAddr);

            ad.addr = aclConnectionElement->deviceAddr;
            ad.type = CSR_BT_ADDR_PUBLIC;
            dm_acl_close_req(&ad,
                             DM_ACL_FLAG_FORCE,
                             HCI_ERROR_OETC_USER,
                             NULL);
        }
        else
        {
            /* Operation completed */
            csrBtCmAclDetachComplete(cmData,
                                     cmPrim->phandle,
                                     CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER,
                                     CSR_BT_SUPPLIER_CM);
        }
    }
    else
    {
        /* Detach a specific ACL */
        aclTable *aclConnectionElement;

        returnAclConnectionElement(cmData, cmPrim->deviceAddr, &aclConnectionElement);

        if(aclConnectionElement &&
           CsrBtCmReturnNumOfConnectionToPeerDeviceFlags(cmData, &aclConnectionElement->deviceAddr, cmPrim->flags) == 0)
        {
            TYPED_BD_ADDR_T ad;

            /* Requested device address is connected and can hence be
             * disconnected */
            cmData->dmVar.dmMsgInProgress = CSR_BT_CM_ACL_DETACH_REQ;
            CsrBtBdAddrCopy(&cmData->dmVar.detachAddr, &cmPrim->deviceAddr);
            aclConnectionElement->detachRequested = TRUE;

            ad.addr = cmPrim->deviceAddr;
            ad.type = cmPrim->addressType;
            dm_acl_close_req(&ad,
                             DM_ACL_FLAG_FORCE,
                             HCI_ERROR_OETC_USER,
                             NULL);
        }
        else
        {
            /* Operation completed */
            csrBtCmAclDetachComplete(cmData,
                                     cmPrim->phandle,
                                     CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER,
                                     CSR_BT_SUPPLIER_CM);
        }
    }
}

/* The DM reports that the ACL has been closed, either via a
 * DM_ACL_CLOSED_IND or through a failed DM_ACL_CONN_HANDLE_IND */
void CsrBtCmDmAclDetachAclCloseHandler(cmInstanceData_t *cmData, aclTable *aclConnectionElement,
                                       CsrBtDeviceAddr deviceAddr, CsrUint8 reason)
{
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    /* Is the pending L2CAP connection waiting for the detach (legacy
     * pairing work around)? */
    if((cmData->l2caVar.connElement != NULL) &&
       (cmData->l2caVar.connElement->cmL2caConnInst->state == l2capLegacyDetach_s) &&
       CsrBtBdAddrEq(&cmData->dmVar.detachAddr, &cmData->l2caVar.connElement->cmL2caConnInst->deviceAddr))
    {
        if (!cmData->l2caVar.cancelConnect)
        {
            /* Connection was waiting for channel to get detached
             * - start the L2CAP connection */
            CsrBtCml2caAutoConnectSetup(cmData, cmData->l2caVar.connElement->cmL2caConnInst);
        }
        else
        {
            CsrBtCmL2caConnectCancelCleanup(cmData, cmData->l2caVar.connElement, TRUE);
        }
    }
#endif

    /* Only act on this if it matches the request we sent */
    if((cmData->dmVar.dmMsgInProgress == CSR_BT_CM_ACL_DETACH_REQ) &&
       CsrBtBdAddrEq(&cmData->dmVar.detachAddr, &deviceAddr))
    {
        aclConnectionElement->detachRequested = FALSE;

        if(csrBtCmAclGetNextDetach(cmData, &aclConnectionElement) == CM_ERROR)
        {
            /* Operation completed*/
            csrBtCmAclDetachComplete(cmData,
                                     cmData->dmVar.appHandle,
                                     CSR_BT_RESULT_CODE_CM_SUCCESS,
                                     CSR_BT_SUPPLIER_CM);
        }
        else
        {
            TYPED_BD_ADDR_T ad;

            /* Detach next in list */
            cmData->dmVar.dmMsgInProgress = CSR_BT_CM_ACL_DETACH_REQ;
            CsrBtBdAddrCopy(&cmData->dmVar.detachAddr, &aclConnectionElement->deviceAddr);

            ad.addr = aclConnectionElement->deviceAddr;
            ad.type = CSR_BT_ADDR_PUBLIC;
            dm_acl_close_req(&ad,
                             DM_ACL_FLAG_FORCE,
                             HCI_ERROR_OETC_USER,
                             NULL);
        }
    }
}

CsrBool CsrBtCmDmCancelPageOrDetach(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr)
{
    /* Return TRUE if the ACL connection is being detach otherwise FALSE */         
    aclTable * aclConnectionElement;

    returnAclConnectionElement(cmData, deviceAddr, &aclConnectionElement);

    if (aclConnectionElement == NULL)
    { /* No knowledge about any ACL connections. Try to cancel page scan */
        dm_hci_create_connection_cancel(&(deviceAddr), NULL);
    }
    else
    {
        /* An ACL is present  */
        if (CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, deviceAddr) == 0 &&
            !CsrBtCmCheckSavedIncomingConnectMessages(cmData, deviceAddr))
        {
            /* The ACL is only used by the outgoing connection which
             * is being setup. Called detach in order to cancel the
             * outgoing connection */
            TYPED_BD_ADDR_T ad;

            ad.addr = deviceAddr;
            ad.type = CSR_BT_ADDR_PUBLIC;
            dm_acl_close_req(&ad,
                             DM_ACL_FLAG_FORCE,
                             HCI_ERROR_OETC_USER,
                             NULL);
            return (TRUE);
        }
    }
    return (FALSE);
}


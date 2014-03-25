/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"

#define CSR_BT_CM_FIRST_ENTRY (0)


static void csrBtCmWriteLinkSuperVisionTimeoutCfmMsgSend(CsrSchedQid phandle, CsrBtResultCode resultCode,
                                           CsrBtSupplier resultSupplier, CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmWriteLinkSupervTimeoutCfm            *prim;

    prim                    = (CsrBtCmWriteLinkSupervTimeoutCfm *)CsrPmemAlloc(sizeof(CsrBtCmWriteLinkSupervTimeoutCfm));
    prim->type              = CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM;
    prim->resultCode        = resultCode;
    prim->resultSupplier    = resultSupplier;
    prim->deviceAddr        = deviceAddr;
    CsrBtCmPutMessage(phandle, prim);
}

#if defined(CSR_BT_INSTALL_CM_WRITE_LINK_SUPERVISION_TIMEOUT) || (CSR_BT_DEFAULT_LINK_SUPERVISION_TIMEOUT != CSR_BT_HCI_DEFAULT_LSTO)
void CsrBtCmWriteDmLinkSuperVisionTimeoutHandler(cmInstanceData_t   *cmData,
                                                 CsrSchedQid             phandle,
                                                 CsrUint16          timeout,
                                                 CsrBtDeviceAddr    deviceAddr)
{
    CsrBool         ready = TRUE;
    cmLstoParms_t   *lsto = pnew(cmLstoParms_t);
    lsto->appHandle       = phandle;
    lsto->timeout         = timeout;
    lsto->deviceAddr      = deviceAddr;

    if (cmData->lstoList)
    { /* Another DM_HCI_WRITE_LINK_SUPERV_TIMEOUT is pending. Wait for a
         DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_COMPLETE                          */
        ready = FALSE;
    }
    cmData->lstoList      = CsrBtUtilBllCreateNewEntry(cmData->lstoList, lsto, sizeof(cmLstoParms_t));

    if (ready)
    {
        dm_hci_write_link_superv_timeout(&deviceAddr, timeout, NULL);
    }
}
#endif

static void csrBtCmRemoveDuplicatedCmCommand(cmInstanceData_t *cmData, 
                                             aclTable         *aclConnectionElement)
{ /* This function remove duplicate commands initiated by the CM ONLY       */
    if (aclConnectionElement)
    {
        CsrIntFast16      i;
        cmLstoParms_t *lsto;
        CsrUint16     dataSize;
        CsrUintFast16     nofEntries = CsrBtUtilBllGetNofEntriesEx(cmData->lstoList);

        for (i = 0; (CsrUintFast16)i < nofEntries; i++)
        {
            lsto = (cmLstoParms_t *) CsrBtUtilBllGetDataPointerEx(cmData->lstoList, i, &dataSize);
            if (lsto)
            {
                if (lsto->appHandle == CSR_BT_CM_IFACEQUEUE     &&
                    lsto->timeout == aclConnectionElement->lsto &&
                    bd_addr_eq(&(lsto->deviceAddr), &(aclConnectionElement->deviceAddr)))
                {
                    cmData->lstoList = CsrBtUtilBllFreeLinkedListEntryEx(cmData->lstoList, i, CsrBtUtilBllPfreeWrapper);
                    nofEntries = CsrBtUtilBllGetNofEntriesEx(cmData->lstoList);
                    i  = -1; /* Will be increased by 1 when restarting the loop */     
                }
            }
        }
    }
}

void CsrBtCmDmHciWriteLinkSuperVisionTimeoutCompleteHandler(cmInstanceData_t *cmData)
{ /* This event is the confirmation that the host controller was write
     (or try to) the new supervision timeout value to the given device address.
     Build and send CM_LINK_WRITE_SUPERV_TIMEOUT_CFM to the application */
    CsrUint16 dataSize;
    DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_T    *dmPrim = (DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM_T *)cmData->recvMsgP;
    cmLstoParms_t *lsto = (cmLstoParms_t *) CsrBtUtilBllGetDataPointerEx(cmData->lstoList, CSR_BT_CM_FIRST_ENTRY, &dataSize);

    if (lsto)
    {
        aclTable *aclConnectionElement = NULL;

        if (dmPrim->status == HCI_SUCCESS)
        {
            returnAclConnectionElement(cmData, dmPrim->bd_addr, &aclConnectionElement);

            if (aclConnectionElement)
            {
                aclConnectionElement->lsto = lsto->timeout;
            }
        }

        if (lsto->appHandle != CSR_BT_CM_IFACEQUEUE)
        {
            aclConnectionElement = NULL;
            if (dmPrim->status == HCI_SUCCESS)
            {
                csrBtCmWriteLinkSuperVisionTimeoutCfmMsgSend(lsto->appHandle,
                                                             CSR_BT_RESULT_CODE_CM_SUCCESS,
                                                             CSR_BT_SUPPLIER_CM,
                                                             dmPrim->bd_addr);
            }
            else
            {
                csrBtCmWriteLinkSuperVisionTimeoutCfmMsgSend(lsto->appHandle,
                                                             (CsrBtResultCode) dmPrim->status,
                                                             CSR_BT_SUPPLIER_HCI,
                                                             dmPrim->bd_addr);
            }
        }
        else
        {
            ;
        }

        cmData->lstoList = CsrBtUtilBllFreeLinkedListEntryEx(cmData->lstoList, CSR_BT_CM_FIRST_ENTRY, CsrBtUtilBllPfreeWrapper);
        csrBtCmRemoveDuplicatedCmCommand(cmData, aclConnectionElement);

        if (cmData->lstoList)
        { /* Send the next DM_HCI_WRITE_LINK_SUPERV_TIMEOUT                         */
            lsto = (cmLstoParms_t *) CsrBtUtilBllGetDataPointerEx(cmData->lstoList, CSR_BT_CM_FIRST_ENTRY, &dataSize);
            if (lsto)
            {
                dm_hci_write_link_superv_timeout(&(lsto->deviceAddr), lsto->timeout, NULL);
            }
            else
            { /* No more messages to send                                               */
                ;
            }
        }
        else
        { /* No more messages to send                                                   */
            ;
        }
    }
    else
    {
        ;
    }
}

#ifdef CSR_BT_INSTALL_CM_WRITE_LINK_SUPERVISION_TIMEOUT
void CsrBtCmWriteLinkSuperVTimeoutReqHandler(cmInstanceData_t *cmData)
{ /* This event indicates that the application desired to change link supervision
     timeout setting */
    aclTable           *aclConnectionElement;
    CsrBtCmWriteLinkSupervTimeoutReq *cmPrim = (CsrBtCmWriteLinkSupervTimeoutReq *) cmData->recvMsgP;
    returnAclConnectionElement(cmData, cmPrim->deviceAddr, &aclConnectionElement);

    if (aclConnectionElement)
    {
        CsrBtCmWriteDmLinkSuperVisionTimeoutHandler(cmData,
                                                    cmPrim->phandle,
                                                    cmPrim->timeout,
                                                    cmPrim->deviceAddr);
    }
    else
    {
        csrBtCmWriteLinkSuperVisionTimeoutCfmMsgSend(cmPrim->phandle,
                                                     CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER,
                                                     CSR_BT_SUPPLIER_CM,
                                                     cmPrim->deviceAddr);
    }
}
#endif

void CsrBtCmDmHciLinkSupervisionTimeoutIndHandler(cmInstanceData_t *cmData)
{ /* This event indicates that the application desired to change link supervision
     timeout setting */
    DM_HCI_LINK_SUPERV_TIMEOUT_IND_T *dmPrim;
    aclTable *aclConnectionElement;

    dmPrim    = (DM_HCI_LINK_SUPERV_TIMEOUT_IND_T *) cmData->recvMsgP;

    returnAclConnectionElement(cmData, dmPrim->bd_addr, &aclConnectionElement);

    if (aclConnectionElement)
    {
        aclConnectionElement->lsto = dmPrim->timeout;
    }
}


/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"

static void csrBtCmCopyTheRemoteName(cmInstanceData_t *cmData, CsrBtDeviceName theName)
{
    CsrUintFast16                      i,j;
    CsrUint8                          *ptr;
    DM_HCI_REMOTE_NAME_CFM_T          *dmPrim;

    CsrUint16 nNameLen   = 0;
    CsrBool  bNullFound  = FALSE;
    dmPrim              = (DM_HCI_REMOTE_NAME_CFM_T *)cmData->recvMsgP;

    for ( i = 0 ; ( ( i < HCI_LOCAL_NAME_BYTE_PACKET_PTRS ) && ( nNameLen < CSR_BT_MAX_FRIENDLY_NAME_LEN ) ) ; i++ )
    {
        ptr = dmPrim->name_part[ i ];
        for ( j = 0 ; ( ( j < HCI_LOCAL_NAME_BYTES_PER_PTR ) && ( nNameLen < CSR_BT_MAX_FRIENDLY_NAME_LEN ) ) ; j++ )
        {
            if ( *ptr == '\0' )
            {
                bNullFound = TRUE;
                break;
            }
            theName[nNameLen++] = *ptr++;
        } /*  end for (j ...) */
        /* quit loop if we got to the end of the string */
        if ( bNullFound )
        {
            break;
        }
    } /*  end for (i = .... */
    /* terminate the string */
    CsrUtf8StrTruncate(theName, nNameLen);
}

void CsrBtCmReadRemoteNameCfmSend(cmInstanceData_t *cmData, CsrSchedQid phandle,
             CsrBtDeviceAddr deviceAddr, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtCmReadRemoteNameCfm *cmPrim = (CsrBtCmReadRemoteNameCfm *)CsrPmemAlloc(sizeof(CsrBtCmReadRemoteNameCfm));
    cmPrim->type                     = CSR_BT_CM_READ_REMOTE_NAME_CFM;
    cmPrim->deviceAddr               = deviceAddr;
    cmPrim->resultCode               = resultCode;
    cmPrim->resultSupplier           = resultSupplier;

    if (resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtDeviceName tmpnam;
        csrBtCmCopyTheRemoteName(cmData, tmpnam);
        cmPrim->friendlyName = CsrUtf8StrDup((CsrUtf8String*)&tmpnam);
    }
    else
    {
        cmPrim->friendlyName         = CsrUtf8StrDup((CsrUtf8String*)"");
    }
    CsrBtCmPutMessage(phandle, cmPrim);
}

void CsrBtCmReadRemoteNameSetup(cmInstanceData_t *cmData)
{ /* Request the DM module to read remote name */
    CsrUint16 clockOffset                    = 0;
    page_scan_mode_t pageScanMode           = 0;
    page_scan_rep_mode_t pageScanRepMode    = 0;

    if(isCacheParamsKnown(cmData, cmData->dmVar.readNameDeviceAddr, &clockOffset, &pageScanMode, &pageScanRepMode))
    {
        CsrBtCmDmWriteCacheParamsDirect(cmData,
                                   cmData->dmVar.readNameDeviceAddr,
                                   clockOffset,
                                   pageScanMode,
                                   pageScanRepMode,
                                   CM_PLAYER,
                                   CSR_BT_CM_READ_REMOTE_NAME_REQ);
    }
    else
    {
        cmData->dmVar.readingName = TRUE;
        dm_hci_remote_name_request(&cmData->dmVar.readNameDeviceAddr, NULL);
    }
}

void CsrBtCmReadRemoteNameReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadRemoteNameReq    *cmPrim;

    cmPrim = (CsrBtCmReadRemoteNameReq *) cmData->recvMsgP;

    cmData->dmVar.appHandle         = cmPrim->phandle;
    cmData->dmVar.remoteNamePending = TRUE;
    bd_addr_copy(&cmData->dmVar.readNameDeviceAddr, &cmPrim->deviceAddr);

    if (cmData->roleVar.doMssBeforeRnr &&
        CsrBtCmDmAclMarkForMss(cmData, &cmData->dmVar.readNameDeviceAddr) != CM_ERROR)
    {
        CsrBtCmDmAclProcessNextMarkedForMss(cmData);
    }
    else
    {
        CsrBtCmReadRemoteNameSetup(cmData);
    }
}

void CsrBtCmCancelReadRemoteNameReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmCancelReadRemoteNameReq *prim;

    prim = (CsrBtCmCancelReadRemoteNameReq *)cmData->recvMsgP;

    if (cmData->dmVar.remoteNamePending &&
        bd_addr_eq(&cmData->dmVar.readNameDeviceAddr,&prim->deviceAddr) &&
        (cmData->dmVar.appHandle == prim->appHandle))
    {
        if (cmData->dmVar.readingName)
        {
            dm_hci_remote_name_req_cancel(&prim->deviceAddr, NULL);
        }
        else
        {
            /* Wait for finish writing of cache parameters */
            cmData->dmVar.cancelRemoteName = TRUE;
        }
    }
    else
    {
        if (cancelDmMsg(cmData, CSR_BT_CM_READ_REMOTE_NAME_REQ, prim->appHandle, prim->deviceAddr))
        {
            CsrBtCmReadRemoteNameCfmSend(cmData, prim->appHandle, prim->deviceAddr, CSR_BT_RESULT_CODE_CM_CANCELLED, CSR_BT_SUPPLIER_CM);
        }
        /* Else, the CFM has already been sent, or cancel is sent without name request. Ignore in both cases. */
    }
}

void CsrBtCmDmHciRemoteNameCompleteHandler(cmInstanceData_t *cmData)
{ /* Notification of remote name is received. Read the name and restore the
     local DM queue */
    CsrUintFast16 i;
    DM_HCI_REMOTE_NAME_CFM_T   *dmPrim;

    dmPrim = (DM_HCI_REMOTE_NAME_CFM_T *)cmData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS && 
        cmData->dmVar.appHandle != cmData->sdHandle &&
        cmData->sdHandle != CSR_SCHED_QID_INVALID)
    { /* The attempt to find a remote device is a success, and it is not
         initiated by the SD module */
        CsrBtCmReadRemoteNameInd       * sdPrim;
        CsrBtDeviceName tmpnam;

        sdPrim  = (CsrBtCmReadRemoteNameInd *) CsrPmemAlloc(sizeof(CsrBtCmReadRemoteNameInd));
        csrBtCmCopyTheRemoteName(cmData, tmpnam);
        sdPrim->friendlyName = CsrUtf8StrDup(tmpnam);

        if (sdPrim->friendlyName[0] != '\0')
        { /* A valid name is found, pass this information to the SD module */
            sdPrim->type        = CSR_BT_CM_READ_REMOTE_NAME_IND;
            sdPrim->deviceAddr  = dmPrim->bd_addr;
            CsrBtCmPutMessage(cmData->sdHandle, sdPrim);
        }
        else
        { /* The name is not valid just CsrPmemFree the message again to prevent a
             memory leak */
            CsrPmemFree(sdPrim);
        }
    }

    if (cmData->dmVar.readingName == TRUE)
    { /* The remote name has been requested from one of the profiles or the SC */
        if (dmPrim->status == HCI_SUCCESS)
        {
            CsrBtCmReadRemoteNameCfmSend(cmData, cmData->dmVar.appHandle, dmPrim->bd_addr,
                                        CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
        }
        else
        {
            CsrBtCmReadRemoteNameCfmSend(cmData, cmData->dmVar.appHandle, dmPrim->bd_addr,
                                    (CsrBtResultCode) dmPrim->status, CSR_BT_SUPPLIER_HCI);
        }

        for ( i = 0 ; i < HCI_LOCAL_NAME_BYTE_PACKET_PTRS ; i++ )
        {/* In order to prevent a memory leak, the memory associated with pointers
            are CsrPmemFree */
            CsrPmemFree(dmPrim->name_part[i]);
        }
        cmData->dmVar.readingName = FALSE;
        CsrBtCmDmUpdateAndClearCachedParamDirect(cmData,dmPrim->bd_addr);
    }
}

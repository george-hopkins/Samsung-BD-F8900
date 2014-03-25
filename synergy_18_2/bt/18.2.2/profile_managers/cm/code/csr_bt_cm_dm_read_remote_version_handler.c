/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"

static void csrBtCmSaveRemoteVersionHandler(cmInstanceData_t *cmData,
                                            CsrBtDeviceAddr  deviceAddr,
                                            CsrUint8         lmpVersion,
                                            CsrUint16        manufacturerName,
                                            CsrUint16        lmpSubversion,
                                            CsrUint8         hciStatus)
{
    if (hciStatus == HCI_SUCCESS)
    {
        aclTable *aclConnectionElement;
        returnAclConnectionElement(cmData, deviceAddr, &aclConnectionElement);

        if (aclConnectionElement)
        {
            aclConnectionElement->lmpVersion        = lmpVersion;
            aclConnectionElement->manufacturerName  = manufacturerName;
            aclConnectionElement->lmpSubversion     = lmpSubversion;
        }
    }
}

static void csrBtCmReadRemoteVersionReqKick(cmInstanceData_t *cmData)
{
    remoteVersionReq *req;

    req = (remoteVersionReq*)CsrCmnListGetFirst(&cmData->remoteVersion);
    if(req)
    {
        TYPED_BD_ADDR_T ad;

        /* Send next request if list isn't empty */
        ad.addr = req->addr;
        ad.type = CSR_BT_ADDR_PUBLIC;
        dm_hci_read_remote_version(&ad, NULL);
    }
}

void CsrBtCmDmHciReadRemoteVersionCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_READ_REMOTE_VER_INFO_CFM_T *dmPrim = (DM_HCI_READ_REMOTE_VER_INFO_CFM_T *) cmData->recvMsgP;
    remoteVersionReq *req;

    req = (remoteVersionReq*)CsrCmnListGetFirst(&cmData->remoteVersion);
    if(req != NULL)
    {
        if(req->appHandle != CSR_BT_CM_IFACEQUEUE)
        {
            CsrBtCmReadRemoteVersionCfm *cmPrim;

            cmPrim = (CsrBtCmReadRemoteVersionCfm *)CsrPmemAlloc(sizeof(CsrBtCmReadRemoteVersionCfm));
            cmPrim->type                = CSR_BT_CM_READ_REMOTE_VERSION_CFM;
            cmPrim->deviceAddr          = dmPrim->addrt.addr;
            cmPrim->lmpSubversion       = dmPrim->LMP_subversion;
            cmPrim->lmpVersion          = dmPrim->LMP_version;
            cmPrim->manufacturerName    = dmPrim->manufacturer_name;

            if (dmPrim->status == HCI_SUCCESS)
            {
                cmPrim->resultCode      = CSR_BT_RESULT_CODE_CM_SUCCESS;
                cmPrim->resultSupplier  = CSR_BT_SUPPLIER_CM;
            }
            else
            {
                cmPrim->resultCode      = (CsrBtResultCode) dmPrim->status;
                cmPrim->resultSupplier  = CSR_BT_SUPPLIER_HCI;
            }

            CsrBtCmPutMessage(req->appHandle, cmPrim);
        }

        CsrCmnListElementRemove(&cmData->remoteVersion, (CsrCmnListElm_t*)req);
    }
    else
    {
        /* The Cm has issue this request. */
        ;
    }

    csrBtCmSaveRemoteVersionHandler(cmData,
                                    dmPrim->addrt.addr,
                                    dmPrim->LMP_version,
                                    dmPrim->manufacturer_name,
                                    dmPrim->LMP_subversion,
                                    dmPrim->status);

    /* Fire off next pending request */
    csrBtCmReadRemoteVersionReqKick(cmData);
}

void CsrBtCmReadRemoteVersionReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadRemoteVersionReq *cmPrim;
    remoteVersionReq *qe;

    cmPrim = (CsrBtCmReadRemoteVersionReq *) cmData->recvMsgP;

    /* Add to queue. Kicker will ensure next pending request is
     * sent */
    qe = (remoteVersionReq*)CsrCmnListElementAddLast(&cmData->remoteVersion,
                                                     sizeof(remoteVersionReq));
    qe->appHandle = cmPrim->appHandle;
    qe->addr = cmPrim->deviceAddr;

    csrBtCmReadRemoteVersionReqKick(cmData);
}


/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#ifdef CSR_BT_INSTALL_CM_PRI_IAC

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_prim.h"
#include "dm_prim.h"
#include "dmlib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_util.h"

#define HCI_INQ_CODE_LIAC   ((CsrUint24) 0x9E8B00)
#define LIAC_PERIOD         60000000
#define LIAC_RETRY_DELAY    1000000
#define LIAC_RETRY_COUNT    10

void CsrBtCmReadIacReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadIacReq *cmPrim;

    cmPrim = (CsrBtCmReadIacReq *)cmData->recvMsgP;

    cmData->dmVar.appHandle = cmPrim->appHandle;
    dm_hci_read_current_iac_lap(NULL);
}

static void sendCmReadIacCfm(CsrSchedQid appHandle, CsrUint24 iac, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtCmReadIacCfm *msg  = (CsrBtCmReadIacCfm *)CsrPmemAlloc(sizeof(CsrBtCmReadIacCfm));
    msg->type               = CSR_BT_CM_READ_IAC_CFM;
    msg->iac                = iac;
    msg->resultCode         = resultCode;
    msg->resultSupplier     = resultSupplier;
    CsrBtCmPutMessage(appHandle, msg);
}

void CsrBtCmDmHciReadIacCompleteHandler(cmInstanceData_t *cmData)
{
    CsrUintFast8                    i;
    CsrUintFast8                    numIac;
    DM_HCI_READ_CURRENT_IAC_LAP_CFM_T *dmPrim;
    CsrUint24 iac = 0;

    dmPrim = (DM_HCI_READ_CURRENT_IAC_LAP_CFM_T *)cmData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS)
    {
        if (dmPrim->num_current_iac <= 2)
        {
            numIac = dmPrim->num_current_iac;
            i=0;
            while (numIac)
            {
                CsrUintFast16 structIndex;
                CsrUintFast16 numToDo = HCI_IAC_LAP_PER_PTR;

                if (numToDo > numIac)
                {
                    numToDo = numIac;
                }
                for ( structIndex = 0; structIndex < numToDo; structIndex++)
                {
                    if  ((iac == 0) || ((dmPrim->iac_lap[i][structIndex] != HCI_INQ_CODE_GIAC) && (iac == HCI_INQ_CODE_GIAC)))
                    {
                        iac = dmPrim->iac_lap[i][structIndex];
                    }
                }
                CsrPmemFree(dmPrim->iac_lap[i]);
                numIac = (CsrUintFast8)(numIac - numToDo);
                i++;
            }
            if (iac != 0)
            {
                sendCmReadIacCfm(cmData->dmVar.appHandle, iac, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
            }
            else
            {
                sendCmReadIacCfm(cmData->dmVar.appHandle, iac, CSR_BT_RESULT_CODE_CM_UNSPECIFIED_ERROR, CSR_BT_SUPPLIER_CM);
            }
        }
        else
        {
            sendCmReadIacCfm(cmData->dmVar.appHandle, iac, CSR_BT_RESULT_CODE_CM_UNSPECIFIED_ERROR, CSR_BT_SUPPLIER_CM);
        }
    }
    else
    {
        sendCmReadIacCfm(cmData->dmVar.appHandle, iac, (CsrBtResultCode) dmPrim->status, CSR_BT_SUPPLIER_HCI);
    }
    CsrBtCmDmLocalQueueHandler();
}

static void sendCmWriteIacInd(CsrSchedQid appHandle, CsrUint24 iac, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtCmWriteIacInd *msg = (CsrBtCmWriteIacInd *)CsrPmemAlloc(sizeof(CsrBtCmWriteIacInd));
    msg->type               = CSR_BT_CM_WRITE_IAC_IND;
    msg->iac                = iac;
    msg->resultCode         = resultCode;
    msg->resultSupplier     = resultSupplier;
    CsrBtCmPutMessage(appHandle, msg);
}

static void restoreGiacAfterLiac(CsrUint16 dummy, void* argv)
{
    cmInstanceData_t *cmData;

    cmData = (cmInstanceData_t *)argv;

    CSR_UNUSED(dummy);
    CsrBtCmWriteIacReqSend(CSR_BT_CM_IFACEQUEUE, HCI_INQ_CODE_GIAC);
    cmData->dmVar.liacRestoreTimer = 0;
}

void CsrBtCmWriteIacIndHandler(cmInstanceData_t *cmData)
{
    CsrBtCmWriteIacInd *cmPrim = (CsrBtCmWriteIacInd *)cmData->recvMsgP;

    if (((cmPrim->resultCode != CSR_BT_RESULT_CODE_CM_SUCCESS) || (cmPrim->resultSupplier != CSR_BT_SUPPLIER_CM)) && (cmData->dmVar.liacRetryCounter++ < LIAC_RETRY_COUNT))
    {
        cmData->dmVar.liacRestoreTimer = CsrSchedTimerSet(LIAC_RETRY_DELAY, restoreGiacAfterLiac, 0, (void *)cmData);
    }
    else
    {
        sendCmWriteIacInd(cmData->dmVar.liacRestoreAppHandle, HCI_INQ_CODE_GIAC, cmPrim->resultCode, cmPrim->resultSupplier);
    }
}

static void writePendingIac(cmInstanceData_t *cmData)
{
    CsrUint24 *iac_list;
    CsrUint8 num_iac;

    if ((cmData->dmVar.pendingIac == HCI_INQ_CODE_GIAC) && (cmData->dmVar.numberOfSupportedIac >= 1))
    {
        iac_list = (CsrUint24 *)CsrPmemAlloc(sizeof(CsrUint24));
        iac_list[0] = HCI_INQ_CODE_GIAC;
        num_iac = 1;
        CsrSchedTimerCancel(cmData->dmVar.liacRestoreTimer,NULL,NULL);
    }
    else if (cmData->dmVar.numberOfSupportedIac >= 2)
    {
        iac_list = (CsrUint24 *)CsrPmemAlloc(2*sizeof(CsrUint24));
        iac_list[0] = HCI_INQ_CODE_GIAC;
        iac_list[1] = cmData->dmVar.pendingIac;
        num_iac = 2;
        CsrSchedTimerCancel(cmData->dmVar.liacRestoreTimer,NULL,NULL);
        if (cmData->dmVar.pendingIac == HCI_INQ_CODE_LIAC)
        {
            cmData->dmVar.liacRestoreTimer = CsrSchedTimerSet(LIAC_PERIOD, restoreGiacAfterLiac, 0, (void *)cmData);
            cmData->dmVar.liacRestoreAppHandle = cmData->dmVar.appHandle;
            cmData->dmVar.liacRetryCounter = 0;
        }
    }
    else
    {
        sendCmWriteIacInd(cmData->dmVar.appHandle, cmData->dmVar.pendingIac, CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE, CSR_BT_SUPPLIER_CM);
        CsrBtCmDmLocalQueueHandler();
        cmData->dmVar.pendingIac = 0;
        return;
    }
    dm_hci_write_current_iac_lap(num_iac, iac_list, NULL);
    CsrPmemFree(iac_list);
}

void CsrBtCmWriteIacReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmWriteIacReq *cmPrim;

    cmPrim = (CsrBtCmWriteIacReq *)cmData->recvMsgP;

    cmData->dmVar.appHandle = cmPrim->appHandle;
    cmData->dmVar.pendingIac = cmPrim->iac;

    if ((cmPrim->iac >= HCI_IAC_LAP_MIN) == (cmPrim->iac <= HCI_IAC_LAP_MAX))
    {
        if (cmData->dmVar.numberOfSupportedIac == 0)
        {
            dm_hci_read_num_supported_iac(NULL);
            cmData->dmVar.liacRestoreTimer = 0;
        }
        else
        {
            writePendingIac(cmData);
        }
    }
    else
    {
        sendCmWriteIacInd(cmData->dmVar.appHandle, cmPrim->iac, CSR_BT_RESULT_CODE_CM_UNACCEPTABLE_PARAMETER, CSR_BT_SUPPLIER_CM);
        CsrBtCmDmLocalQueueHandler();
        cmData->dmVar.pendingIac = 0;
    }
}

void CsrBtCmDmHciWriteIacCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_T *dmPrim;

    dmPrim = (DM_HCI_WRITE_CURRENT_IAC_LAP_CFM_T *)cmData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS)
    {
        CsrUint24    classOfDevice;
        if (cmData->dmVar.pendingIac == HCI_INQ_CODE_LIAC)
        {
            cmData->dmVar.liacCod |= CSR_BT_LIMITED_DISCOVERABLE_MODE_MAJOR_SERVICE_MASK;
        }
        else
        {
            cmData->dmVar.liacCod &= ~CSR_BT_LIMITED_DISCOVERABLE_MODE_MAJOR_SERVICE_MASK;
        }
        classOfDevice = CsrBtCmReturnClassOfdevice(cmData);

        if (classOfDevice != cmData->dmVar.codWrittenToChip)
        {
            cmData->dmVar.pendingCod = classOfDevice;
            dm_hci_write_class_of_device(classOfDevice, NULL);
        }
        else
        {
            CsrBtCmFinishWriteIac(cmData, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
        }
    }
    else
    {
        sendCmWriteIacInd(cmData->dmVar.appHandle, cmData->dmVar.pendingIac, (CsrBtResultCode)dmPrim->status, CSR_BT_SUPPLIER_HCI);
        CsrBtCmDmLocalQueueHandler();
        cmData->dmVar.pendingIac = 0;
    }
}

void CsrBtCmFinishWriteIac(cmInstanceData_t *cmData, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    sendCmWriteIacInd(cmData->dmVar.appHandle, cmData->dmVar.pendingIac, resultCode, resultSupplier);
    CsrBtCmDmLocalQueueHandler();
    cmData->dmVar.pendingIac = 0;
}

void CsrBtCmDmHciReadNumSupportedIacCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_T *dmPrim;

    dmPrim = (DM_HCI_READ_NUM_SUPPORTED_IAC_CFM_T *)cmData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS)
    {
        cmData->dmVar.numberOfSupportedIac = dmPrim->num;
        writePendingIac(cmData);
    }
    else
    {
        sendCmWriteIacInd(cmData->dmVar.appHandle, cmData->dmVar.pendingIac, (CsrBtResultCode)dmPrim->status, CSR_BT_SUPPLIER_HCI);
        CsrBtCmDmLocalQueueHandler();
        cmData->dmVar.pendingIac = 0;
    }
}
#endif

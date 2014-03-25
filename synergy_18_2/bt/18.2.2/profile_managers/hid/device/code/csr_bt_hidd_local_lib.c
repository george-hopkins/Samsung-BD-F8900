/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_pmem.h"
#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "csr_bt_hidd_prim.h"
#include "csr_bt_hidd_main.h"
#include "csr_bt_hidd_local_lib.h"
#include "csr_bt_util.h"

void CsrBtHiddMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_HIDD_PRIM, msg);
}

void CsrBtHiddActivateCfmSend(HiddInstanceDataType *instData,
                                    CsrBtResultCode resultCode,
                                    CsrBtSupplier resultSupplier)
{
    CsrBtHiddActivateCfm* prim;

    prim = (CsrBtHiddActivateCfm *) CsrPmemAlloc(sizeof(CsrBtHiddActivateCfm));

    prim->type = CSR_BT_HIDD_ACTIVATE_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtHiddMessagePut(instData->appHandle, prim);
}

void CsrBtHiddDeactivateCfmSend(HiddInstanceDataType *instData,
                                       CsrBtResultCode resultCode,
                                       CsrBtSupplier resultSupplier)
{
    CsrBtHiddDeactivateCfm* prim;

    prim = (CsrBtHiddDeactivateCfm *) CsrPmemAlloc(sizeof(CsrBtHiddDeactivateCfm));

    prim->type = CSR_BT_HIDD_DEACTIVATE_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtHiddMessagePut(instData->appHandle, prim);
}

void CsrBtHiddStatusIndSend(HiddInstanceDataType *instData,
                            CsrBtHiddStatusType status,
                            CsrBtConnId btConnId)
{
    CsrBtHiddStatusInd* prim;

    prim = (CsrBtHiddStatusInd *) CsrPmemAlloc(sizeof(CsrBtHiddStatusInd));

    prim->type = CSR_BT_HIDD_STATUS_IND;
    CsrBtBdAddrCopy(&(prim->deviceAddr),&(instData->bdAddr));
    prim->status = status;
    prim->btConnId = btConnId;

    CsrBtHiddMessagePut(instData->appHandle, prim);

}

void CsrBtHiddControlIndSend(HiddInstanceDataType *instData, CsrBtHiddTransactionType transactionType, CsrBtHiddParameterType parameter, CsrUint16 dataLen, CsrUint8* data)
{
    CsrBtHiddControlInd* prim;

    prim = (CsrBtHiddControlInd *) CsrPmemAlloc(sizeof(CsrBtHiddControlInd));

    prim->type = CSR_BT_HIDD_CONTROL_IND;
    prim->transactionType = transactionType;
    prim->parameter = parameter;
    prim->dataLen = dataLen;
    prim->data = data;

    CsrBtHiddMessagePut(instData->appHandle, prim);
}

void CsrBtHiddDataCfmSend(HiddInstanceDataType *instData,
                                CsrBtResultCode resultCode,
                                CsrBtSupplier resultSupplier)
{
    CsrBtHiddDataCfm* prim;

    prim = (CsrBtHiddDataCfm *) CsrPmemAlloc(sizeof(CsrBtHiddDataCfm));

    prim->type = CSR_BT_HIDD_DATA_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtHiddMessagePut(instData->appHandle, prim);
}

void CsrBtHiddDataIndSend(HiddInstanceDataType *instData, CsrBtHiddReportType reportType, CsrUint16 reportLen, CsrUint8 *report)
{
    CsrBtHiddDataInd* prim;

    prim = (CsrBtHiddDataInd *) CsrPmemAlloc(sizeof(CsrBtHiddDataInd));

    prim->type = CSR_BT_HIDD_DATA_IND;
    prim->reportType = reportType;
    prim->reportLen = reportLen;
    prim->report = report;

    CsrBtHiddMessagePut(instData->appHandle, prim);
}

void CsrBtHiddUnplugCfmSend(HiddInstanceDataType *instData,
                                   CsrBtResultCode resultCode,
                                   CsrBtSupplier resultSupplier)
{
    CsrBtHiddUnplugCfm* prim;

    prim = (CsrBtHiddUnplugCfm *) CsrPmemAlloc(sizeof(CsrBtHiddUnplugCfm));

    prim->type = CSR_BT_HIDD_UNPLUG_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtHiddMessagePut(instData->appHandle, prim);
}

void CsrBtHiddUnplugIndSend(HiddInstanceDataType *instData,
                                  CsrBtDeviceAddr deviceAddr,
                                  CsrBtResultCode resultCode,
                                  CsrBtSupplier resultSupplier)
{
    CsrBtHiddUnplugInd* prim;

    prim = (CsrBtHiddUnplugInd *) CsrPmemAlloc(sizeof(CsrBtHiddUnplugInd));

    prim->type = CSR_BT_HIDD_UNPLUG_IND;
    CsrBtBdAddrCopy(&(prim->deviceAddr),&deviceAddr);
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtHiddMessagePut(instData->appHandle, prim);
}

void CsrBtHiddModeChangeIndSend(HiddInstanceDataType *instData,
                                         CsrBtHiddPowerModeType mode,
                                         CsrBtResultCode resultCode,
                                         CsrBtSupplier resultSupplier)
{
    CsrBtHiddModeChangeInd* prim;

    prim = (CsrBtHiddModeChangeInd *) CsrPmemAlloc(sizeof(CsrBtHiddModeChangeInd));

    prim->type = CSR_BT_HIDD_MODE_CHANGE_IND;
    prim->mode = mode;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtHiddMessagePut(instData->appHandle, prim);
}

/* message used locally to reactivate if connection is lost for an unknown reason */
void CsrBtHiddReactivateIndSend(HiddInstanceDataType *instData)
{
    CsrBtHiddReactivateInd *prim;

    prim = (CsrBtHiddReactivateInd *) CsrPmemAlloc(sizeof(CsrBtHiddReactivateInd));

    prim->type = CSR_BT_HIDD_REACTIVATE_IND;

    CsrBtHiddMessagePut(instData->myAppHandle, prim);
}

void CsrBtHiddSecurityInCfmSend(CsrSchedQid appHandle,
                                      CsrBtResultCode resultCode,
                                      CsrBtSupplier resultSupplier)
{
    CsrBtHiddSecurityInCfm *prim;
    prim = (CsrBtHiddSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtHiddSecurityInCfm));
    prim->type = CSR_BT_HIDD_SECURITY_IN_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtHiddMessagePut(appHandle, prim);
}

void CsrBtHiddSecurityOutCfmSend(CsrSchedQid appHandle,
                                        CsrBtResultCode resultCode,
                                        CsrBtSupplier resultSupplier)
{
    CsrBtHiddSecurityOutCfm *prim;
    prim = (CsrBtHiddSecurityOutCfm*)CsrPmemAlloc(sizeof(CsrBtHiddSecurityOutCfm));
    prim->type = CSR_BT_HIDD_SECURITY_OUT_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtHiddMessagePut(appHandle, prim);
}


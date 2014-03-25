/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "csr_bt_util.h"
#include "csr_bt_hidh_prim.h"
#include "csr_bt_hidh_main.h"

static void CsrBtHidhMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_HIDH_PRIM, msg);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhConnectCfmSend
 *
 *  DESCRIPTION
 *      Send a CSR_BT_HIDH_CONNECT_CFM primitive
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhConnectCfmSend(CsrSchedQid appHandle,
                             CsrBtResultCode resultCode,
                             CsrBtSupplier resultSupplier,
                             CsrBtDeviceAddr bdAddr,
                             CsrUint8 deviceId,
                             hidh_subInstData_t *subInst,
                             CsrBtConnId btConnId)
{
    CsrBtHidhConnectCfm    *prim;

    /* Bail out if invalid handle */
    if(appHandle == CSR_SCHED_QID_INVALID)
    {
        return;
    }

    prim = (CsrBtHidhConnectCfm *) CsrPmemAlloc(sizeof(CsrBtHidhConnectCfm));

    prim->type             = CSR_BT_HIDH_CONNECT_CFM;
    prim->resultCode       = resultCode;
    prim->resultSupplier   = resultSupplier;
    prim->deviceId         = deviceId;
    prim->deviceAddr       = bdAddr;

    prim->sdpInfo          = NULL;
    prim->sdpInfoCount     = 0;
    prim->serviceName      = NULL;
    prim->descriptor       = NULL;
    prim->descriptorLength = 0;

    prim->btConnId = btConnId;

    if(subInst != NULL)
    {
        if(subInst->sdpInfo.isValid)
        {
            prim->sdpInfo = (CsrBtHidhSdpInfo *)CsrPmemAlloc(sizeof(CsrBtHidhSdpInfo));
            CsrMemCpy(prim->sdpInfo, &subInst->sdpInfo, sizeof(CsrBtHidhSdpInfo));
            prim->sdpInfoCount = 1; /* Due to serialization */
        }

        if(subInst->descriptor != NULL)
        {
            prim->descriptorLength = subInst->descriptorLength;
            prim->descriptor = (CsrUint8 *)CsrPmemAlloc(subInst->descriptorLength);
            CsrMemCpy(prim->descriptor, subInst->descriptor, subInst->descriptorLength);
        }

        if(subInst->serviceName != NULL)
        {
            prim->serviceName = (CsrCharString*)CsrPmemAlloc(CsrStrLen((char*)subInst->serviceName)+1);
            CsrStrCpy((char*)prim->serviceName, (char*)subInst->serviceName);
        }
    }

    CsrBtHidhMessagePut(appHandle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhConnectAcceptCfmSend
 *
 *  DESCRIPTION
 *      Send a CSR_BT_HIDH_CONNECT_ACCEPT_CFM primitive
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhConnectAcceptCfmSend(CsrSchedQid appHandle,
                                   CsrBtResultCode resultCode,
                                   CsrBtSupplier resultSupplier,
                                   CsrBtDeviceAddr bdAddr,
                                   CsrUint8 deviceId,
                                   CsrBtConnId btConnId)
{
    CsrBtHidhConnectAcceptCfm    *prim;

    /* Bail out if invalid handle */
    if(appHandle == CSR_SCHED_QID_INVALID)
    {
        return;
    }

    prim = (CsrBtHidhConnectAcceptCfm *) CsrPmemAlloc(sizeof(CsrBtHidhConnectAcceptCfm));

    prim->type              = CSR_BT_HIDH_CONNECT_ACCEPT_CFM;
    prim->resultCode        = resultCode;
    prim->resultSupplier    = resultSupplier;
    prim->deviceId          = deviceId;
    prim->deviceAddr        = bdAddr;
    prim->btConnId = btConnId;

    CsrBtHidhMessagePut(appHandle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhConnectAcceptIndSend
 *
 *  DESCRIPTION
 *      Send a CSR_BT_HIDH_CONNECT_ACCEPT_IND primitive
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhConnectAcceptIndSend(CsrSchedQid appHandle,
                                            CsrBtResultCode resultCode,
                                            CsrBtSupplier resultSupplier,
                                            CsrUint8 deviceId)
{
    CsrBtHidhConnectAcceptInd    *prim;

    /* Bail out if invalid handle */
    if(appHandle == CSR_SCHED_QID_INVALID)
    {
        return;
    }

    prim = (CsrBtHidhConnectAcceptInd *) CsrPmemAlloc(sizeof(CsrBtHidhConnectAcceptInd));

    prim->type            = CSR_BT_HIDH_CONNECT_ACCEPT_IND;
    prim->resultCode      = resultCode;
    prim->resultSupplier  = resultSupplier;
    prim->deviceId        = deviceId;

    CsrBtHidhMessagePut(appHandle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhCancelConnectAcceptCfmSend
 *
 *  DESCRIPTION
 *      Send a CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM primitive
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhCancelConnectAcceptCfmSend(CsrSchedQid appHandle,
                                         CsrUint8 deviceId,
                                         CsrBtResultCode resultCode,
                                         CsrBtSupplier resultSupplier)
{
    CsrBtHidhCancelConnectAcceptCfm    *prim;

    /* Bail out if invalid handle */
    if(appHandle == CSR_SCHED_QID_INVALID)
    {
        return;
    }

    prim = (CsrBtHidhCancelConnectAcceptCfm *) CsrPmemAlloc(sizeof(CsrBtHidhCancelConnectAcceptCfm));

    prim->type            = CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM;
    prim->resultCode      = resultCode;
    prim->resultSupplier  = resultSupplier;
    prim->deviceId        = deviceId;

    CsrBtHidhMessagePut(appHandle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhDisconnectCfmSend
 *
 *  DESCRIPTION
 *      Send a CSR_BT_HIDH_DISCONNECT_CFM primitive
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhDisconnectCfmSend(CsrSchedQid appHandle,
                                        CsrBtResultCode resultCode,
                                        CsrBtSupplier resultSupplier,
                                        CsrUint8 deviceId)
{
    CsrBtHidhDisconnectCfm    *prim;

    /* Bail out if invalid handle */
    if(appHandle == CSR_SCHED_QID_INVALID)
    {
        return;
    }

    prim = (CsrBtHidhDisconnectCfm *) CsrPmemAlloc(sizeof(CsrBtHidhDisconnectCfm));

    prim->type            = CSR_BT_HIDH_DISCONNECT_CFM;
    prim->resultCode      = resultCode;
    prim->resultSupplier  = resultSupplier;
    prim->deviceId        = deviceId;

    CsrBtHidhMessagePut(appHandle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhDisconnectIndSend
 *
 *  DESCRIPTION
 *      Send a CSR_BT_HIDH_DISCONNECT_IND primitive
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhDisconnectIndSend(CsrSchedQid appHandle,
                                       CsrBtReasonCode reasonCode,
                                       CsrBtSupplier reasonSupplier,
                                       CsrUint8 deviceId)
{
    CsrBtHidhDisconnectInd    *prim;

    /* Bail out if invalid handle */
    if(appHandle == CSR_SCHED_QID_INVALID)
    {
        return;
    }

    prim = (CsrBtHidhDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtHidhDisconnectInd));

    prim->type            = CSR_BT_HIDH_DISCONNECT_IND;
    prim->reasonCode      = reasonCode;
    prim->reasonSupplier  = reasonSupplier;
    prim->deviceId        = deviceId;

    CsrBtHidhMessagePut(appHandle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhControlIndSend
 *
 *  DESCRIPTION
 *      Send a CSR_BT_HIDH_CONTROL_IND primitive
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhControlIndSend(CsrSchedQid appHandle,
                                   CsrUint8 deviceId,
                                   CsrBtHidhCtrl operation)
{
    CsrBtHidhControlInd    *prim;

    /* Bail out if invalid handle */
    if(appHandle == CSR_SCHED_QID_INVALID)
    {
        return;
    }

    prim = (CsrBtHidhControlInd *) CsrPmemAlloc(sizeof(CsrBtHidhControlInd));

    prim->type            = CSR_BT_HIDH_CONTROL_IND;
    prim->deviceId        = deviceId;
    prim->operation    = operation;

    CsrBtHidhMessagePut(appHandle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhHandshakeIndSend
 *
 *  DESCRIPTION
 *      Send a CSR_BT_HIDH_HANDSHAKE_IND primitive
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhHandshakeIndSend(CsrSchedQid appHandle,
                                       CsrUint8 deviceId,
                                       CsrBtResultCode resultCode)
{
    CsrBtHidhHandshakeInd    *prim;

    /* Bail out if invalid handle */
    if(appHandle == CSR_SCHED_QID_INVALID)
    {
        return;
    }

    prim = (CsrBtHidhHandshakeInd *) CsrPmemAlloc(sizeof(CsrBtHidhHandshakeInd));

    prim->type          = CSR_BT_HIDH_HANDSHAKE_IND;
    prim->deviceId      = deviceId;
    prim->resultCode    = resultCode;

    CsrBtHidhMessagePut(appHandle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhStatusIndSend
 *
 *  DESCRIPTION
 *      Send a CSR_BT_HIDH_STATUS_IND primitive
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhStatusIndSend(CsrSchedQid appHandle,
                       CsrBtHidhStatus status,
                       CsrUint8 deviceId,
                       hidh_subInstData_t *subInst)
{
    CsrBtHidhStatusInd    *prim;

    /* Bail out if invalid handle */
    if(appHandle == CSR_SCHED_QID_INVALID)
    {
        return;
    }

    prim = (CsrBtHidhStatusInd *) CsrPmemAlloc(sizeof(CsrBtHidhStatusInd));

    prim->type             = CSR_BT_HIDH_STATUS_IND;
    prim->deviceId         = deviceId;
    prim->status           = status;

    prim->sdpInfo          = NULL;
    prim->sdpInfoCount     = 0;
    prim->serviceName      = NULL;
    prim->descriptor       = NULL;
    prim->descriptorLength = 0;

    if(subInst != NULL)
    {
        if(subInst->sdpInfo.isValid)
        {
            prim->sdpInfo = (CsrBtHidhSdpInfo *)CsrPmemAlloc(sizeof(CsrBtHidhSdpInfo));
            CsrMemCpy(prim->sdpInfo, &subInst->sdpInfo, sizeof(CsrBtHidhSdpInfo));
            prim->sdpInfoCount = 1; /* Due to serialization */
        }

        if(subInst->descriptor != NULL)
        {
            prim->descriptorLength = subInst->descriptorLength;
            prim->descriptor = (CsrUint8 *)CsrPmemAlloc(subInst->descriptorLength);
            CsrMemCpy(prim->descriptor, subInst->descriptor, subInst->descriptorLength);
        }

        if(subInst->serviceName != NULL)
        {
            prim->serviceName = (CsrCharString*)CsrPmemAlloc(CsrStrLen((char*)subInst->serviceName)+1);
            CsrStrCpy((char*)prim->serviceName, (char*)subInst->serviceName);
        }
    }

    CsrBtHidhMessagePut(appHandle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhRegisterUserCfmSend
 *
 *  DESCRIPTION
 *      Send a CSR_BT_HIDH_REGISTER_USER_CFM
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhRegisterUserCfmSend(CsrSchedQid appHandle,
                                          CsrUint8 slotId,
                                          CsrBtResultCode resultCode,
                                          CsrBtSupplier resultSupplier)
{
    CsrBtHidhRegisterUserCfm    *prim;

    /* Bail out if invalid handle */
    if(appHandle == CSR_SCHED_QID_INVALID)
    {
        return;
    }

    prim = (CsrBtHidhRegisterUserCfm *) CsrPmemAlloc(sizeof(CsrBtHidhRegisterUserCfm));

    prim->type            = CSR_BT_HIDH_REGISTER_USER_CFM;
    prim->slotId          = slotId;
    prim->resultCode      = resultCode;
    prim->resultSupplier  = resultSupplier;

    CsrBtHidhMessagePut(appHandle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhDataIndSend
 *
 *  DESCRIPTION
 *      Send a CSR_BT_HIDH_DATA_IND primitive
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhDataIndSend(CsrSchedQid appHandle, CsrUint8 deviceId, CsrUint16 dataLen, CsrUint8 *data)
{
    CsrBtHidhDataInd    *prim;

    /* Bail out if invalid handle */
    if(appHandle == CSR_SCHED_QID_INVALID)
    {
        return;
    }

    prim = (CsrBtHidhDataInd *) CsrPmemAlloc(sizeof(CsrBtHidhDataInd));

    prim->type            = CSR_BT_HIDH_DATA_IND;
    prim->deviceId        = deviceId;
    prim->reportType    = data[0] & 0x03;
    prim->dataLen        = dataLen;
    prim->data            = data;

    CsrBtHidhMessagePut(appHandle, prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSecurityInCfmSend
 *
 *  DESCRIPTION
 *      Send confirm on security changes
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtHidhSecurityInCfm *prim;
    prim = (CsrBtHidhSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtHidhSecurityInCfm));
    prim->type = CSR_BT_HIDH_SECURITY_IN_CFM;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtHidhMessagePut(appHandle, prim);
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSecurityOutCfmSend
 *
 *  DESCRIPTION
 *      Send confirm on security changes
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSecurityOutCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtHidhSecurityOutCfm *prim;
    prim = (CsrBtHidhSecurityOutCfm*)CsrPmemAlloc(sizeof(CsrBtHidhSecurityOutCfm));
    prim->type = CSR_BT_HIDH_SECURITY_OUT_CFM;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtHidhMessagePut(appHandle, prim);
}


/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ppp_lib.h"
#include "csr_bt_util.h"

void PppConnectReqSend(CsrSchedQid applicationQueue, CsrBtDeviceAddr deviceAddr)
{
    CsrBtPppConnectReq * connectReq;

    connectReq = (CsrBtPppConnectReq *) CsrPmemAlloc(sizeof(CsrBtPppConnectReq));
    connectReq->type = CSR_BT_PPP_CONNECT_REQ;
    connectReq->applicationQueue = applicationQueue;
    connectReq->deviceAddr = deviceAddr;

    CsrSchedMessagePut(CSR_BT_PPP_IFACEQUEUE, CSR_BT_PPP_PRIM, connectReq);
}

void PppRegisterNetworkProtocolReqSend(CsrSchedQid responseQueue, CsrUint16 ncpProtocol, CsrSchedQid dataQueue, CsrUint16 dataProtocol)
{
    CsrBtPppRegisterNetworkProtocolReq * rnpReq;

    rnpReq = (CsrBtPppRegisterNetworkProtocolReq *) CsrPmemAlloc(sizeof(CsrBtPppRegisterNetworkProtocolReq));
    rnpReq->type = CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_REQ;
    rnpReq->responseQueue = responseQueue;
    rnpReq->ncpProtocol = ncpProtocol;
    rnpReq->dataQueue = dataQueue;
    rnpReq->dataProtocol = dataProtocol;

    CsrSchedMessagePut(CSR_BT_PPP_IFACEQUEUE, CSR_BT_PPP_PRIM, rnpReq);
}

void PppNcpReqSend(CsrUint16 ncpProtocol, CsrUint16 length, CsrUint8 * payload)
{
    CsrBtPppNcpReq * ncpReq;

    ncpReq = (CsrBtPppNcpReq *) CsrPmemAlloc(sizeof(CsrBtPppNcpReq));
    ncpReq->type = CSR_BT_PPP_NCP_REQ;
    ncpReq->protocol = ncpProtocol;
    ncpReq->payloadLength = length;
    ncpReq->payload = payload;

    CsrSchedMessagePut(CSR_BT_PPP_IFACEQUEUE, CSR_BT_PPP_PRIM, ncpReq);
}

void PppDisconnectReqSend(void)
{
    CsrBtPppDisconnectReq * disconnectReq;

    disconnectReq = (CsrBtPppDisconnectReq *) CsrPmemAlloc(sizeof(CsrBtPppDisconnectReq));
    disconnectReq->type = CSR_BT_PPP_DISCONNECT_REQ;

    CsrSchedMessagePut(CSR_BT_PPP_IFACEQUEUE, CSR_BT_PPP_PRIM, disconnectReq);
}

void PppAuthenticateResSend(CsrUint8 * loginName, CsrUint8 * password)
{
    CsrBtPppAuthenticateRes * authenticateRes;

    authenticateRes = (CsrBtPppAuthenticateRes *) CsrPmemAlloc(sizeof(CsrBtPppAuthenticateRes));
    authenticateRes->type = CSR_BT_PPP_AUTHENTICATE_RES;
    if (loginName)
    {
        authenticateRes->loginName = CsrPmemAlloc(CsrStrLen((char *) loginName) + 1);
        CsrStrCpy((char *) authenticateRes->loginName, (char *) loginName);
    }
    else
    {
        authenticateRes->loginName = NULL;
    }
    if (password)
    {
        authenticateRes->password = CsrPmemAlloc(CsrStrLen((char *) password) + 1);
        CsrStrCpy((char *) authenticateRes->password, (char *) password);
    }
    else
    {
        authenticateRes->password = NULL;
    }

    CsrSchedMessagePut(CSR_BT_PPP_IFACEQUEUE, CSR_BT_PPP_PRIM, authenticateRes);
}


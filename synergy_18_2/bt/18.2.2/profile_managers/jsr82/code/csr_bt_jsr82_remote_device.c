/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_jsr82_prim.h"
#include "csr_bt_util.h"
#include "csr_pmem.h"
#include "csr_types.h"
#include "csr_bt_jsr82_main.h"
#include "csr_bt_jsr82_maintenance_handler.h"
#include "csr_bt_jsr82_discovery.h"
#include "csr_bt_usr_config.h"
#include "bluetooth.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_jsr82_remote_device.h"


void CsrBtJsr82RdGetFriendlyNameReqHandler(Jsr82MainInstanceData *instData)
{
    requestedNamesStruct *element;
    CsrBtJsr82RdGetFriendlyNameReq *prim;
    prim = (CsrBtJsr82RdGetFriendlyNameReq *)instData->recvMsg;

    if(!instData->requestedNames)
    {
        instData->requestedNames = (requestedNamesStruct *)CsrPmemZalloc(sizeof(requestedNamesStruct));
        element = instData->requestedNames;
    }
    else
    {
        element = instData->requestedNames;
        while(element->next)
        {
            element = element->next;
        }
        element->next = (requestedNamesStruct *)CsrPmemZalloc(sizeof(requestedNamesStruct));
        element = element->next;
    }
    element->address = prim->address;
    element->alwaysAsk = prim->alwaysAsk;
    element->appHandle = prim->appHandle;
    element->reqID = prim->reqID;


    if(prim->alwaysAsk)
    {
        CsrBtCmReadRemoteNameReqSend(CSR_BT_JSR82_IFACEQUEUE, prim->address);
    }
    else
    {
        CsrBtSdReadDeviceInfoReqSend(CSR_BT_JSR82_IFACEQUEUE, prim->address); /* confirm is taken by jsr82RemoteNameCfm */
    }
    CsrBtJsr82SetRdName(instData);
}


static void sendCsrBtJsr82RdGetFriendlyNameCfm(CsrSchedQid appHandle, CsrUint32 reqID, CsrUtf8String *remoteName, CsrBtResultCode result)
{
    CsrBtJsr82RdGetFriendlyNameCfm *msg;

    msg = (CsrBtJsr82RdGetFriendlyNameCfm *)CsrPmemZalloc(sizeof(CsrBtJsr82RdGetFriendlyNameCfm));
    msg->type = CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM;
    msg->reqID = reqID;
    msg->remoteName = remoteName;
    msg->resultCode = result;
    CsrBtJsr82MessagePut(appHandle, msg);
}


void CsrBtJsr82RemoteNameCfmHandler(Jsr82MainInstanceData *instData)
{
    requestedNamesStruct *element;
    CsrUint16 event;
    event = instData->eventClass;

    if (instData->inquiry)
    {
        instData->inquiry->nameReqSent = FALSE;
    }

    if(event == CSR_BT_CM_PRIM)
    {
        CsrBtCmReadRemoteNameCfm *prim;
        prim = (CsrBtCmReadRemoteNameCfm *)instData->recvMsg;

        if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            sendCsrBtJsr82RdGetFriendlyNameCfm(instData->requestedNames->appHandle,
                                               instData->requestedNames->reqID, prim->friendlyName,
                                               CSR_BT_RESULT_CODE_JSR82_SUCCESS);
        }
        else
        {
            sendCsrBtJsr82RdGetFriendlyNameCfm(instData->requestedNames->appHandle,
                                               instData->requestedNames->reqID, prim->friendlyName,
                                               CSR_BT_RESULT_CODE_JSR82_UNSPECIFIED_ERROR);
        }
        prim->friendlyName = NULL;
    }
    else if(event == CSR_BT_SD_PRIM)
    {
        CsrBtSdReadDeviceInfoCfm    *prim;
        CsrUint16 nameLen;
        CsrUint8 *name = NULL;
        prim = (CsrBtSdReadDeviceInfoCfm *)instData->recvMsg;
        nameLen = CsrBtSdDecodeFriendlyName(prim->info, prim->infoLen, &name);
        if (prim->resultSupplier == CSR_BT_SUPPLIER_SD &&
            prim->resultCode == CSR_BT_RESULT_CODE_SD_SUCCESS &&
            (nameLen > 0))
        {
            sendCsrBtJsr82RdGetFriendlyNameCfm(instData->requestedNames->appHandle,
                                               instData->requestedNames->reqID,
                                               (CsrUtf8String*)name,
                                               CSR_BT_RESULT_CODE_JSR82_SUCCESS);
        }
        else
        {
            if ((instData->inquiry) && (instData->state == DISCOVERY_S) && (instData->inquiry->state == INQUIRY_STATE))
            {
                instData->requestedNames->alwaysAsk = TRUE; /* make close_search_ind handler get the name through the CM */
                /*CsrBtSdCancelSearchReqSend(CSR_BT_JSR82_IFACEQUEUE);*/
                CsrBtCmCancelInquiryReqSend(CSR_BT_JSR82_IFACEQUEUE);
                instData->inquiry->state = CANCEL_STATE;
                CsrPmemFree(name);
                CsrPmemFree(prim->info);
                CsrBtJsr82DaSdCloseIndHandler(instData);
                return;
            }
            else
            {
                CsrBtCmReadRemoteNameReqSend(CSR_BT_JSR82_IFACEQUEUE, instData->requestedNames->address);
                if (instData->inquiry)
                {
                    instData->inquiry->nameReqSent = TRUE;
                }
                CsrPmemFree(name);
                CsrPmemFree(prim->info);
                return;
            }
        }
        CsrPmemFree(prim->info);
    }

    element = instData->requestedNames->next;
    CsrPmemFree(instData->requestedNames);
    instData->requestedNames = element;


    if(instData->requestedNames)
    {
        if(instData->requestedNames->alwaysAsk)
        {
            if ((instData->inquiry) && (instData->state == DISCOVERY_S) && (instData->inquiry->state == INQUIRY_STATE))
            {
                /*CsrBtSdCancelSearchReqSend(CSR_BT_JSR82_IFACEQUEUE);*/
                CsrBtCmCancelInquiryReqSend(CSR_BT_JSR82_IFACEQUEUE);
                instData->inquiry->state = CANCEL_STATE;
                CsrBtJsr82DaSdCloseIndHandler(instData);
            }
            else
            {
                CsrBtCmReadRemoteNameReqSend(CSR_BT_JSR82_IFACEQUEUE, instData->requestedNames->address);
                if (instData->inquiry)
                {
                    instData->inquiry->nameReqSent = TRUE;
                }
                /* Already in name state */
            }
        }
        else
        {
            if ((instData->inquiry) && (instData->state == DISCOVERY_S) && (instData->inquiry->state != INQUIRY_STATE))
            {
                if (instData->inquiry->timedOut || instData->inquiry->cancelled)
                {
                    instData->inquiry->state = NAME_STATE;
                }
                else
                {
                    CsrBtJsr82DaContinueInquiry(instData);
                }
            }
            CsrBtSdReadDeviceInfoReqSend(CSR_BT_JSR82_IFACEQUEUE, instData->requestedNames->address); /* confirm is taken by jsr82RemoteNameCfm */
            if (instData->inquiry)
            {
                instData->inquiry->nameReqSent = TRUE;
            }
        }
    }
    else
    {
        if(instData->state == RD_NAME_S)
        {
            CsrBtJsr82SetIdle(instData);
        }
        else if(instData->state == DISCOVERY_S )
        {
            instData->gettingName = FALSE;
            if (instData->inquiry)
            {
                if (instData->inquiry->timedOut || instData->inquiry->cancelled)
                {
                    CsrPmemFree(instData->inquiry);
                    instData->inquiry = NULL;
                    CsrBtJsr82SetIdle(instData);
                }
                else if (instData->inquiry->state != INQUIRY_STATE)
                {
                    CsrBtJsr82DaContinueInquiry(instData);
                }
            }
        }
        else
        {
            /* This may never happen */
            instData->gettingName = FALSE;
        }
    }
}


void CsrBtJsr82RdAuthenticate(Jsr82MainInstanceData *instData)
{
    switch(instData->eventClass)
    {
        case CSR_BT_JSR82_PRIM:
            {
                CsrBtJsr82RdAuthenticateReq *prim;
                prim = (CsrBtJsr82RdAuthenticateReq *)instData->recvMsg;
                instData->appHandle = prim->appHandle;
                instData->reqID = prim->reqID;

                CsrBtScAuthenticateReqSend(CSR_BT_JSR82_IFACEQUEUE, prim->address);
                CsrBtJsr82SetBusy(instData);
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                CsrBtScPrim *primType;
                primType = (CsrBtScPrim *)instData->recvMsg;

                switch(*primType)
                {
                    case CSR_BT_SC_AUTHENTICATE_CFM:
                        {
                            CsrBtScAuthenticateCfm *prim;
                            CsrBtJsr82RdAuthenticateCfm *msg;
                            prim = (CsrBtScAuthenticateCfm *)instData->recvMsg;
                            msg = (CsrBtJsr82RdAuthenticateCfm *)CsrPmemAlloc(sizeof(CsrBtJsr82RdAuthenticateCfm));
                            msg->type = CSR_BT_JSR82_RD_AUTHENTICATE_CFM;

                            if (prim->resultSupplier == CSR_BT_SUPPLIER_SC &&
                                prim->resultCode == CSR_BT_RESULT_CODE_SC_SUCCESS)
                            {
                                msg->resultCode = CSR_BT_RESULT_CODE_JSR82_SUCCESS;
                            }
                            else
                            {
                                msg->resultCode = CSR_BT_RESULT_CODE_JSR82_SECURITY_FAIL;
                            }
                            msg->reqID = instData->reqID;

                            CsrBtJsr82MessagePut(instData->appHandle, msg);
                            CsrBtJsr82SetIdle(instData);
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
                break;
            }
        default:
            {
                break;
            }
    }

}

void CsrBtJsr82RdForwardScPrim(Jsr82MainInstanceData *instData)
{
    CsrSchedMessagePut(CSR_BT_APPLICATION_SECURITY_HANDLER, CSR_BT_SC_PRIM, instData->recvMsg); /* App SC handler is defined in csr_bt_usr_config.h */
    instData->recvMsg = NULL;
}

static CsrUint8 csrBtJsr82IsConnected(Jsr82MainInstanceData *instData, CsrBtDeviceAddr address)
{
    /* Check if there's a Java initiated L2CAP or RFCOMM connection active on the given address */
    CsrUint8 i;
    for(i=0; i<MAX_ACL_LINKS; i++)
    {
        if(CsrBtBdAddrEq(&instData->aclConnections[i].deviceAddr, &address))
        {
            return (CsrUint8)(i+1);
        }
    }
    return 0;
}

void CsrBtJsr82RdEncrypt(Jsr82MainInstanceData *instData)
{

    switch(instData->eventClass)
    {
        case CSR_BT_JSR82_PRIM:
            {
                CsrBtJsr82RdEncryptReq *prim;
                prim = (CsrBtJsr82RdEncryptReq *)instData->recvMsg;
                instData->appHandle = prim->appHandle;
                instData->reqID = prim->reqID;

                if(prim->encrypt && csrBtJsr82IsConnected(instData, prim->address))
                {
                    CsrBtScEncryptionReqSend(CSR_BT_JSR82_IFACEQUEUE, prim->address, prim->encrypt);
                    CsrBtJsr82SetBusy(instData);
                }
                else
                {
                    CsrBtJsr82RdEncryptCfm *msg;
                    msg = (CsrBtJsr82RdEncryptCfm *)CsrPmemAlloc(sizeof(CsrBtJsr82RdEncryptCfm));
                    msg->type = CSR_BT_JSR82_RD_ENCRYPT_CFM;

                    if (!prim->encrypt)
                    {
                        msg->resultCode = CSR_BT_RESULT_CODE_JSR82_COMMAND_DISALLOWED;
                    }
                    else
                    {
                        msg->resultCode = CSR_BT_RESULT_CODE_JSR82_DEVICE_NOT_CONNECTED;
                    }
                    msg->reqID = instData->reqID;

                    CsrBtJsr82MessagePut(instData->appHandle, msg);
                }
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                CsrBtJsr82RdEncryptCfm *msg;

                CsrBtScEncryptionCfm *prim;
                prim = (CsrBtScEncryptionCfm *)instData->recvMsg;

                msg = (CsrBtJsr82RdEncryptCfm *)CsrPmemAlloc(sizeof(CsrBtJsr82RdEncryptCfm));
                msg->type = CSR_BT_JSR82_RD_ENCRYPT_CFM;
                msg->reqID = instData->reqID;

                if (prim->resultCode == CSR_BT_RESULT_CODE_SC_SUCCESS &&
                    prim->resultSupplier == CSR_BT_SUPPLIER_SC)
                {
                    msg->resultCode = CSR_BT_RESULT_CODE_JSR82_SUCCESS;
                }
                else
                {
                    msg->resultCode = CSR_BT_RESULT_CODE_JSR82_UNSPECIFIED_ERROR;
                }

                CsrBtJsr82MessagePut(instData->appHandle, msg);
                CsrBtJsr82SetIdle(instData);
                break;
            }
        default:
            break;
    }

}

void CsrBtJsr82RdIsAuthenticatedReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82RdIsAuthenticatedReq *prim;
    CsrUint8 connections = 0;

    prim = (CsrBtJsr82RdIsAuthenticatedReq *)instData->recvMsg;

    connections = csrBtJsr82IsConnected(instData, prim->deviceAddr);
    if(!connections)
    {
        CsrBtJsr82RdIsAuthenticatedCfm *msg;


        msg = CsrPmemAlloc(sizeof(CsrBtJsr82RdIsAuthenticatedCfm));
        msg->type = CSR_BT_JSR82_RD_IS_AUTHENTICATED_CFM;
        msg->reqID = prim->reqID;
        msg->authenticated = FALSE;
        CsrBtJsr82MessagePut(prim->appHandle, msg);
    }
    else
    {
        instData->reqID = prim->reqID;
        instData->appHandle = prim->appHandle;
        instData->trustedCheck = FALSE;
        CsrBtSdReadDeviceInfoReqSend(CSR_BT_JSR82_IFACEQUEUE, prim->deviceAddr);
        CsrBtJsr82SetBusy(instData);
    }
}

void CsrBtJsr82RdIsEncryptedReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82RdIsEncryptedReq *prim;

    prim = (CsrBtJsr82RdIsEncryptedReq *)instData->recvMsg;

    instData->reqID = prim->reqID;
    instData->appHandle = prim->appHandle;
    CsrBtCmReadEncryptionStatusReqSend(CSR_BT_JSR82_IFACEQUEUE, prim->deviceAddr);
    CsrBtJsr82SetBusy(instData);
}

void CsrBtJsr82CmReadEncryptionStatusCfmHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmReadEncryptionStatusCfm *prim;
    CsrBtJsr82RdIsEncryptedCfm *msg;

    prim = (CsrBtCmReadEncryptionStatusCfm *)instData->recvMsg;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82RdIsEncryptedCfm));
    msg->type = CSR_BT_JSR82_RD_IS_ENCRYPTED_CFM;
    msg->reqID = instData->reqID;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        msg->encrypted = prim->encrypted;
    }
    else
    {
        msg->encrypted = FALSE;
    }

    CsrBtJsr82MessagePut(instData->appHandle, msg);
    CsrBtJsr82SetIdle(instData);
}

void CsrBtJsr82RdIsTrustedReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82RdIsTrustedReq *prim;

    prim = (CsrBtJsr82RdIsTrustedReq *)instData->recvMsg;
    instData->appHandle = prim->appHandle;
    instData->reqID = prim->reqID;

    instData->trustedCheck = TRUE;
    CsrBtSdReadDeviceInfoReqSend(CSR_BT_JSR82_IFACEQUEUE, prim->deviceAddr);
    CsrBtJsr82SetBusy(instData);
}

void CsrBtJsr82SdReadDeviceInfoCfmHandler(Jsr82MainInstanceData *instData)
{
    CsrBtSdReadDeviceInfoCfm    *prim;
    prim = (CsrBtSdReadDeviceInfoCfm *)instData->recvMsg;

    if(instData->trustedCheck) /* We're checking the trusted (what the SD calls "authorised") status of this device */
    {
        CsrBtJsr82RdIsTrustedCfm                 *msg;

        msg = CsrPmemAlloc(sizeof(CsrBtJsr82RdIsTrustedCfm));
        msg->type = CSR_BT_JSR82_RD_IS_TRUSTED_CFM;
        msg->reqID = instData->reqID;
        if (prim->resultSupplier == CSR_BT_SUPPLIER_SD && prim->resultCode == CSR_BT_RESULT_CODE_SD_SUCCESS &&
            ((prim->deviceStatus & CSR_BT_SD_STATUS_PAIRED) == CSR_BT_SD_STATUS_PAIRED) && ((prim->deviceStatus & CSR_BT_SD_STATUS_TRUSTED) == CSR_BT_SD_STATUS_TRUSTED))
        {
            msg->trusted = TRUE;
        }
        else
        {
            msg->trusted = FALSE;
        }
        CsrBtJsr82MessagePut(instData->appHandle, msg);
    }
    else /* not checking trusted status - i.e. we're checking authentication status */
    {
        CsrBtJsr82RdIsAuthenticatedCfm                 *msg;

        msg = CsrPmemAlloc(sizeof(CsrBtJsr82RdIsAuthenticatedCfm));
        msg->type = CSR_BT_JSR82_RD_IS_AUTHENTICATED_CFM;
        msg->reqID = instData->reqID;
        if (prim->resultSupplier == CSR_BT_SUPPLIER_SD && prim->resultCode == CSR_BT_RESULT_CODE_SD_SUCCESS &&
            ((prim->deviceStatus & CSR_BT_SD_STATUS_PAIRED) == CSR_BT_SD_STATUS_PAIRED))
        {
            msg->authenticated = TRUE;
        }
        else
        {
            msg->authenticated = FALSE;
        }
        CsrBtJsr82MessagePut(instData->appHandle, msg);

    }
    CsrPmemFree(prim->info);
    CsrBtJsr82SetIdle(instData);
}

void CsrBtJsr82RdIsConnected(Jsr82MainInstanceData * instData)
{
    CsrBtJsr82RdIsConnectedCfm *msg;
    CsrBtJsr82RdIsConnectedReq *prim;
    prim = (CsrBtJsr82RdIsConnectedReq *)instData->recvMsg;

    msg = (CsrBtJsr82RdIsConnectedCfm *)CsrPmemAlloc(sizeof(CsrBtJsr82RdIsConnectedCfm));
    msg->type = CSR_BT_JSR82_RD_IS_CONNECTED_CFM;
    msg->handle = csrBtJsr82IsConnected(instData, prim->deviceAddr);
    msg->reqID = prim->reqID;
    CsrBtJsr82MessagePut(prim->appHandle, msg);
}

void CsrBtJsr82AddConnection(Jsr82MainInstanceData *instData, CsrBtDeviceAddr deviceAddr)
{
    CsrUint8 connectionHandle;
    connectionHandle = csrBtJsr82IsConnected(instData, deviceAddr);
    if(connectionHandle)
    {
        connectionHandle--; /* handles are offset by one compared to the array index */
        instData->aclConnections[connectionHandle].activeConnections++;
    }
    else
    {
        CsrUint8 i;
        for(i=0; i<MAX_ACL_LINKS; i++)
        {
            if(instData->aclConnections[i].activeConnections == 0)
            {
                instData->aclConnections[i].deviceAddr = deviceAddr;
                instData->aclConnections[i].activeConnections = 1;
                break;
            }
        }
    }
}

void CsrBtJsr82RemoveConnection(Jsr82MainInstanceData *instData, CsrBtDeviceAddr deviceAddr)
{
    CsrUint8 connectionHandle;
    connectionHandle = csrBtJsr82IsConnected(instData, deviceAddr);
    connectionHandle--; /* handles are offset by one compared to the array index */
    instData->aclConnections[connectionHandle].activeConnections--;
    if(!instData->aclConnections[connectionHandle].activeConnections)
    {
        instData->aclConnections[connectionHandle].deviceAddr.nap = 0;
        instData->aclConnections[connectionHandle].deviceAddr.uap = 0;
        instData->aclConnections[connectionHandle].deviceAddr.lap = 0;
    }
}

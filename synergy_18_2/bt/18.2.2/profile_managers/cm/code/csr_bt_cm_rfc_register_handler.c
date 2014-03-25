/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_RFC_MODULE

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_util.h"

#define CM_POOL_SIZE_FOR_LOCAL_SERVER_CHANNELS  (30) /* This is the number of server channels that
                                                        currently can be assigned by RFCOMM */

static CsrBool csrBtCmFindFreeLocalServerCh(cmInstanceData_t *cmData, CsrUint8 *server)
{
    CsrUintFast8 i;

    for (i = 0; i < cmData->rfcVar.localServerChListSize; i++)
    {
        if (cmData->rfcVar.localServerChList[i].serverChannel != CSR_BT_NO_SERVER && !cmData->rfcVar.localServerChList[i].inUse)
        {
            cmData->rfcVar.localServerChList[i].inUse   = TRUE;
            *server = cmData->rfcVar.localServerChList[i].serverChannel;
            return TRUE;
        }
    }
    return FALSE;
}

static CsrUint8 csrBtCmGetLocalServerChFromList(cmInstanceData_t *cmData, CsrUint8 server)
{
    CsrUintFast8 i;

    for (i = 0; i < cmData->rfcVar.localServerChListSize; i++)
    {
        if (cmData->rfcVar.localServerChList[i].serverChannel == server)
        {
            return (CsrUint8)i;
        }
    }
    return (CsrUint8)i;
}

static void csrBtCmInitializeLocalServerChList(serverChanTable *serverList, CsrUint8 listSize)
{
    CsrUintFast8   i;

    for ( i = 0; i < listSize; i++)
    {
         serverList[i].serverChannel = CSR_BT_NO_SERVER;
         serverList[i].inUse         = FALSE;
    }
}

static CsrBool csrBtCmFoundEmptySpace(serverChanTable *serverList, CsrUint8 listSize, CsrUint8 *index)
{
    CsrUintFast8 i;

    for (i = 0; i < listSize; i++)
    {
        if (serverList[i].serverChannel == CSR_BT_NO_SERVER)
        {
            *index = (CsrUint8)i;
            return TRUE;
        }
    }
    return FALSE;
}

static void csrBtCmInsertLocalServerChInList(cmInstanceData_t *cmData, CsrUint8 theServer)
{
    CsrUint8    index;

    if (cmData->rfcVar.localServerChList)
    {
        if (csrBtCmFoundEmptySpace(cmData->rfcVar.localServerChList, cmData->rfcVar.localServerChListSize, &index))
        {
            ;
        }
        else
        {
            serverChanTable *tempList;

            CsrUint8 t;

            index                                   = cmData->rfcVar.localServerChListSize;
            cmData->rfcVar.localServerChListSize    = (CsrUint8)(index + CM_POOL_SIZE_FOR_LOCAL_SERVER_CHANNELS);
            tempList                                = (serverChanTable *) CsrPmemAlloc(sizeof(serverChanTable) * cmData->rfcVar.localServerChListSize);
            csrBtCmInitializeLocalServerChList(tempList, cmData->rfcVar.localServerChListSize);
            for (t = 0; t < index; t++)
            {
                tempList[t] = cmData->rfcVar.localServerChList[t];
            }
            CsrPmemFree(cmData->rfcVar.localServerChList);
            cmData->rfcVar.localServerChList = tempList;

        }
    }
    else
    {
        index                                  = 0;
        cmData->rfcVar.localServerChListSize   = CM_POOL_SIZE_FOR_LOCAL_SERVER_CHANNELS;
        cmData->rfcVar.localServerChList       = (serverChanTable *) CsrPmemAlloc(sizeof(serverChanTable) * cmData->rfcVar.localServerChListSize);
        csrBtCmInitializeLocalServerChList(cmData->rfcVar.localServerChList, cmData->rfcVar.localServerChListSize);
    }

    cmData->rfcVar.localServerChList[index].inUse           = TRUE;
    cmData->rfcVar.localServerChList[index].serverChannel   = theServer;
}

static void csrBtCmRfcRegisterCfmMsgSend(CsrSchedQid theAppHandle, CsrUint8 theServer,
                            CsrBtResultCode resultCode, CsrBtSupplier resultSupplier,
                            CsrUint16 context)
{/* Send a CSR_BT_CM_REGISTER_CFM signal to the application */
    CsrBtCmRegisterCfm *cmPrim;

    cmPrim    = (CsrBtCmRegisterCfm *)CsrPmemAlloc(sizeof(CsrBtCmRegisterCfm));
    cmPrim->type                = CSR_BT_CM_REGISTER_CFM;
    cmPrim->resultCode          = resultCode;
    cmPrim->resultSupplier      = resultSupplier;
    cmPrim->serverChannel       = theServer;
    cmPrim->context             = context;
    CsrBtCmPutMessage(theAppHandle, cmPrim);
}

void CsrBtCmRfcRegisterReqHandler(cmInstanceData_t *cmData)
{/* This event allows the above profile to register a protocol handle with
    RFCOMM. RFCOMM will return a RFC_REGISTER_CFM primitive with an
    assigned server channel number. The registered protocol handle will be
    used for notifying the profile of the arrival of events for the given server
    channel. */
    CsrBtCmRegisterReq  * cmPrim;

    cmPrim                      = (CsrBtCmRegisterReq *) cmData->recvMsgP;
    cmData->smVar.appHandle     = cmPrim->phandle;
    cmData->smVar.context       = cmPrim->context;
    
    if (cmPrim->serverChannel != CSR_BT_CM_SERVER_CHANNEL_DONT_CARE)
    {/* If the requested server channel is already in use send error confirmation straight away;
        otherwise, register towards the rfcomm */
        
        if (cmData->rfcVar.localServerChList)
        {
            CsrUint8 idx = csrBtCmGetLocalServerChFromList(cmData, cmPrim->serverChannel);
            if (idx < cmData->rfcVar.localServerChListSize)
            {/* Server channel found in local list: is it in use? */
                if (cmData->rfcVar.localServerChList[idx].inUse)
                {
                    csrBtCmRfcRegisterCfmMsgSend(cmData->smVar.appHandle, cmPrim->serverChannel, CSR_BT_RESULT_CODE_CM_SERVER_CHANNEL_ALREADY_USED, CSR_BT_SUPPLIER_CM, cmPrim->context);
                }
                else
                {/* Just mark it as being in use */
                    csrBtCmRfcRegisterCfmMsgSend(cmData->smVar.appHandle, cmPrim->serverChannel, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM, cmPrim->context);
                    cmData->rfcVar.localServerChList[idx].inUse = TRUE;
                }
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
            }
            else
            {
                cmData->smVar.registeringSrvChannel = cmPrim->serverChannel;
                RfcRegisterReqSend(CSR_BT_CM_IFACEQUEUE, 0, cmPrim->context, cmPrim->serverChannel);
            }
        }
        else
        {
            cmData->smVar.registeringSrvChannel = cmPrim->serverChannel;
            RfcRegisterReqSend(CSR_BT_CM_IFACEQUEUE, 0, cmPrim->context, cmPrim->serverChannel);
        }
    }
    else if (cmData->rfcVar.localServerChList)
    {
        CsrUint8 server;

        if (csrBtCmFindFreeLocalServerCh(cmData, &server))
        {
            csrBtCmRfcRegisterCfmMsgSend(cmData->smVar.appHandle, server, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM, cmPrim->context);
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
        else
        {
            RfcRegisterReqSend(CSR_BT_CM_IFACEQUEUE, 0, cmPrim->context, RFC_INVALID_SERV_CHANNEL);
        }
    }
    else
    {
        RfcRegisterReqSend(CSR_BT_CM_IFACEQUEUE, 0, cmPrim->context, RFC_INVALID_SERV_CHANNEL);
    }
}

void CsrBtCmRfcUnRegisterReqMsgSend(cmInstanceData_t *cmData)
{
    CsrUintFast8               i;
    CsrBtCmUnregisterReq     * cmPrim;

    cmPrim        = (CsrBtCmUnregisterReq *) cmData->recvMsgP;

    for (i = 0; i < cmData->rfcVar.localServerChListSize; i++)
    {
        if (cmData->rfcVar.localServerChList[i].serverChannel == cmPrim->serverChannel)
        {
            cmData->rfcVar.localServerChList[i].inUse           = FALSE;
            return;
        }
    }
}
void CsrBtCmRfcRegisterCfmHandler(cmInstanceData_t *cmData)
{ /* This event will be used to indicate to the higher layer that its
     previous registration of a protocol handle with an RFC_REGISTER_REQ
     event had been accepted (if accept is set to True) or denied (if accept
     is set to false). */
    RFC_REGISTER_CFM_T    * rfcPrim;

    rfcPrim        = (RFC_REGISTER_CFM_T *) cmData->recvMsgP;

    if(rfcPrim->accept)
    { /* RFC_REGISTER_REQ event had been accepted. The received
         serverchannel number is send to the profile. */
        if ((cmData->smVar.registeringSrvChannel == CSR_BT_CM_SERVER_CHANNEL_DONT_CARE) || (cmData->smVar.registeringSrvChannel == rfcPrim->loc_serv_chan))
        {/* Either the desired server channel is used, or the application did not request a particular server channel */
            csrBtCmRfcRegisterCfmMsgSend(cmData->smVar.appHandle, rfcPrim->loc_serv_chan, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM, cmData->smVar.context);
            csrBtCmInsertLocalServerChInList(cmData, rfcPrim->loc_serv_chan);
        }
        else
        {/* We did not get the requested server channel. A different one was registered: un-register it and indicate the error to the application */
            RfcUnregisterReqSend(rfcPrim->loc_serv_chan);
            csrBtCmRfcRegisterCfmMsgSend(cmData->smVar.appHandle, CSR_BT_NO_SERVER, (CsrBtResultCode)HCI_ERROR_SCM_INSUFFICIENT_RESOURCES, CSR_BT_SUPPLIER_RFCOMM, cmData->smVar.context);
        }
    }
    else
    { /* RFC_REGISTER_REQ event had been denied */
        csrBtCmRfcRegisterCfmMsgSend(cmData->smVar.appHandle, CSR_BT_NO_SERVER, (CsrBtResultCode)HCI_ERROR_SCM_INSUFFICIENT_RESOURCES, CSR_BT_SUPPLIER_RFCOMM, cmData->smVar.context);
    }
    cmData->smVar.registeringSrvChannel = CSR_BT_CM_SERVER_CHANNEL_DONT_CARE;
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
}

#endif /* #ifndef EXCLUDE_CSR_BT_RFC_MODULE */

/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_hci_sef.h"
#include "csr_hci_prim.h"
#include "csr_hci_task.h"
#include "csr_hci_util.h"
#include "csr_hci_handler.h"
#include "csr_pmem.h"
#include "csr_transport.h"
#include "csr_tm_bluecore_lib.h"
#include "csr_tm_bluecore_private_lib.h"
#include "csr_tm_bluecore_transport.h"
#include "csr_hci_sco.h"
#include "csr_log_gsp.h"
#include "csr_log_text_2.h"
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
#include "csr_ips.h"
#include "csr_sched_ips.h"
#endif

/* CSR_HCI internal transport manager interface functions */
static void *thdl = NULL;
static CsrHciInstData *globalInst;

static void csrHciPrepareTransport(CsrUint8 ch, CsrUint8 type, CsrUint8 *msg, CsrUint32 msg_len)
{
    TXMSG *txQMsg;

    txQMsg = CsrPmemAlloc(sizeof(TXMSG));
    txQMsg->chan = ch;
    txQMsg->seq = type;

    txQMsg->m.buf = msg;
    txQMsg->m.buflen = msg_len;
    txQMsg->m.dex = 0;

    CSR_LOG_BCI(ch, FALSE, msg_len, msg);

    CsrTmBlueCoreTransportMsgTx(thdl, txQMsg);        /*    transport is always able to send or queue the message */
}

static void csrHciIncomingDataProcess(CsrHciInstData *inst, CsrUint8 chan,
    CsrMblk *mblk)
{
    CsrUint8 data[4];

    switch (chan)
    {
        case TRANSPORT_CHANNEL_ACL:
        {
            if (CsrMblkReadHead(&mblk, data, CSR_HCI_ACL_HDR_SIZE) == CSR_HCI_ACL_HDR_SIZE)
            {
                CsrUint16 handlePlusFlags, handle, length;
                CsrSchedQid queueId;

                CsrHciExtractAclHeader(data, &handlePlusFlags, &length);

                handle = handlePlusFlags & CSR_HCI_ACL_HANDLE_MASK;

                if ((queueId = CsrHciCheckHandlers(inst->hciAclHandler, handle)) != CSR_SCHED_QID_INVALID) /* This specifical aclhandle was registred for by a higher layer */
                {
#ifdef CSR_BLUECORE_ONOFF
                    if (inst->state != CSR_HCI_STATE_DEACTIVATING)
#endif
                    {
                        CsrHciSendAclDataInd(handlePlusFlags, queueId, mblk, FALSE);
                        mblk = NULL;
                    }
                }
                else if (inst->hciAclMainHandler != CSR_SCHED_QID_INVALID) /* This is not registred for specificly but instead generally by a higher layer using aclHandle==0xFFFF in CSR_HCI_ACL_REGISTER_HANDLER_REQ */
                {
#ifdef CSR_BLUECORE_ONOFF
                    if (inst->state != CSR_HCI_STATE_DEACTIVATING)
#endif
                    {
                        CsrHciSendAclDataInd(handlePlusFlags, inst->hciAclMainHandler, mblk, TRUE);
                        mblk = NULL;
                    }
                }
                else
                {
                    CSR_LOG_TEXT_WARNING((CsrHciLto, 0,
                                          "csrHciIncomingDataProcess we received ACL data but have nowhere to send it"));
                }
            }
            else
            {
                CSR_LOG_TEXT_WARNING((CsrHciLto, 0,
                                      "csrHciIncomingDataProcess failed to read ACL header out of mblk"));
            }

            break;
        }
        case TRANSPORT_CHANNEL_SCO:
        {
            if (CsrMblkReadHead(&mblk, data, CSR_HCI_SCO_HDR_SIZE) == CSR_HCI_SCO_HDR_SIZE)
            {
                CsrUint16 handlePlusFlags, handle, length;
                CsrSchedQid queueId;

                CsrHciExtractScoHeader(data, &handlePlusFlags, &length);

                handle = handlePlusFlags & CSR_HCI_SCO_HANDLE_MASK;

                if ((queueId = CsrHciCheckHandlers(inst->hciScoHandler, handle)) != CSR_SCHED_QID_INVALID) /* This specifical aclhandle was registred for by a higher layer */
                {
#ifdef CSR_BLUECORE_ONOFF
                    if (inst->state != CSR_HCI_STATE_DEACTIVATING)
#endif
                    {
                        CsrHciSendScoDataInd(handlePlusFlags, queueId, mblk);
                        mblk = NULL;
                    }
                }
                else
                {
                    CSR_LOG_TEXT_WARNING((CsrHciLto, 0,
                                          "csrHciIncomingDataProcess we received SCO data but have nowhere to send it"));
                }
            }
            else
            {
                CSR_LOG_TEXT_WARNING((CsrHciLto, 0,
                                      "csrHciIncomingDataProcess failed to read SCO header out of mblk"));
            }
            break;
        }
        case TRANSPORT_CHANNEL_HCI:
        {
            if (CsrMblkReadHead(&mblk, data, CSR_HCI_EVENT_HDR_SIZE) == CSR_HCI_EVENT_HDR_SIZE)
            {
                if (*data == 0xFF) /* Vendor specific event */
                {
                    if (CsrMblkReadHead(&mblk, data, 2) == 2)
                    {
                        CsrUint8 channel, fragment;
                        channel = data[1] & CHANNEL_ID_MASK;
                        fragment = data[1] & ~CHANNEL_ID_MASK;

                        CsrHciReassembleVendorSpecificEvents(inst, channel, fragment, mblk);
                        mblk = NULL;
                    }
                    else
                    {
                        CSR_LOG_TEXT_WARNING((CsrHciLto, 0,
                                              "csrHciIncomingDataProcess failed to read HCI vendor specific event header out of mblk"));
                    }
                }
                else /* Normal HCI event forward to hciEventHandler (unless its a NOP event in which case it goes to the TM */
                {
                    CsrUint16 length;
                    CsrUint8 *payload;

                    length = CsrMblkGetLength(mblk);
                    payload = CsrPmemAlloc(length + 1);
                    /* Reinsert the event code before forwarding it all upwards */
                    payload[0] = data[0];

                    CsrMblkCopyToMemory(mblk, 0, length, &payload[1]);

                    if (((payload[0] == CSR_HCI_EV_COMMAND_STATUS) && ((length + 1) >= 6) && (payload[4] == 0x00) && (payload[5] == 0x00)) ||
                        ((payload[0] == CSR_HCI_EV_COMMAND_COMPLETE) && ((length + 1) >= 5) && (payload[3] == 0x00) && (payload[4] == 0x00)))
                    { /* A HCI NOP message is received - Note: length check uses (length+1) since length is computed from data excluding the event code */
                        CsrHciSendTmNopInd(inst->hciEventHandler, payload, (CsrUint16) (length + 1));
                    }
                    else
                    {
                        if (inst->hciEventHandler != CSR_SCHED_QID_INVALID)
                        {
#ifdef CSR_BLUECORE_ONOFF
                            if (inst->state != CSR_HCI_STATE_DEACTIVATING)
#endif
                            {
                                CsrHciSendEventInd(inst->hciEventHandler, payload, (CsrUint16) (length + 1));
                                payload = NULL;
                            }
                        }
                        else
                        {
                            CSR_LOG_TEXT_WARNING((CsrHciLto, 0,
                                                  "csrHciIncomingDataProcess we received an HCI event but have nowhere to send it"));
                        }
                        CsrPmemFree(payload);
                    }
                }
            }
            else
            {
                CSR_LOG_TEXT_WARNING((CsrHciLto, 0,
                                      "csrHciIncomingDataProcess failed to read HCI event header out of mblk"));
            }
            break;
        }
        case TRANSPORT_CHANNEL_BCCMD:
        case TRANSPORT_CHANNEL_HQ:
        case TRANSPORT_CHANNEL_DM:
        case TRANSPORT_CHANNEL_L2CAP:
        case TRANSPORT_CHANNEL_RFCOMM:
        case TRANSPORT_CHANNEL_SDP:
        case TRANSPORT_CHANNEL_DFU:
        case TRANSPORT_CHANNEL_VM:
        { /* Can only be sent here by BCSP */
            CsrSchedQid queueId = CsrHciCheckHandlers(inst->hciVendorEventHandler, chan);

            if (queueId != CSR_SCHED_QID_INVALID) /* This specific channel was registred to receive data on by a higher layer */
            {
#ifdef CSR_BLUECORE_ONOFF
                if (inst->state != CSR_HCI_STATE_DEACTIVATING)
#endif
                {
                    CsrHciSendVendorSpecificEventInd(queueId, chan, mblk);
                    mblk = NULL;
                }
            }
            else
            {
                CSR_LOG_TEXT_WARNING((CsrHciLto, 0,
                                      "csrHciIncomingDataProcess we received incoming data on a BCSP channel but we have nowhere to send it"));
            }
            break;
        }
        default:
        {
            CSR_LOG_TEXT_WARNING((CsrHciLto, 0,
                                  "unexpected data received on an invalid transport channel"));
            break;
        }
    }

    if (mblk != NULL)
    {
        CsrMblkDestroy(mblk);
    }
}

/***********************************************************************
CsrHciSendScoData
***********************************************************************/
void CsrHciSendScoData(CsrUint8 *theData)
{
    CSR_LOG_BCI(TRANSPORT_CHANNEL_SCO, FALSE, ((CsrUint8) theData[2]) + 3, theData);

    CsrTmBlueCoreTransportScoTx(thdl, theData);
}

static void csrHciTransportMsgRx(void *arg)
{
    MessageStructure *src;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    /* The purpose of this instrumentation is to attribute the time spent in
       this function to the HCI task rather than the calling context (usually
       the transport protocol driver). */
    CsrUint32 *previousMeasurements;
    previousMeasurements = CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, CsrSchedIpsMeasurementsGet(CSR_SCHED_IPS_CONTEXT_TASK(CSR_HCI_IFACEQUEUE)));
#endif

    src = (MessageStructure *) arg;

    CSR_LOG_BCI(src->chan, TRUE, src->buflen, src->buf);

    if (src->buflen > 0)
    {
        void *data;
        CsrMblk *mblk;

        if (src->chan == TRANSPORT_CHANNEL_SCO)
        {
            if (CsrHciLookForScoHandle(src->buf)) /* We still support function based SCO */
            {
                CsrTmBlueCoreTransportMsgClean(thdl, src);
                return;
            }
        }

        mblk = CsrMblkMallocCreate(&data, (CsrMblkSize) src->buflen);
        CsrMemCpy(data, src->buf, (CsrMblkSize) src->buflen);

        csrHciIncomingDataProcess(globalInst, src->chan, mblk);
    }

    CsrTmBlueCoreTransportMsgClean(thdl, src);

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, previousMeasurements);
#endif
}

void *CsrHciTransportCreate(void *type)
{
    thdl = type;

    CsrTmBlueCoreTransportRegisterMsgRx(type, csrHciTransportMsgRx);

    return type;
}

void CsrHciInstanceRegister(CsrHciInstData *inst)
{
    globalInst = inst;
}

/* CSR_HCI downstream handler functions                                                */
void CsrHciRegisterEventHandlerReqHandler(CsrHciInstData *inst)
{
    CsrHciRegisterEventHandlerReq *prim = inst->recvMsgP;

    inst->hciEventHandler = prim->queueId;

    CsrHciSendRegisterEventHandlerCfm(prim->queueId);
}

void CsrHciRegisterAclHandlerReqHandler(CsrHciInstData *inst)
{
    CsrHciRegisterAclHandlerReq *prim = inst->recvMsgP;

    if (prim->aclHandle == 0xFFFF)
    {
        inst->hciAclMainHandler = prim->queueId;
    }
    else
    {
        CsrHciRegisterHandle(&(inst->hciAclHandler), prim->aclHandle, prim->queueId);
    }

    CsrHciSendRegisterAclHandlerCfm(prim->queueId, prim->aclHandle);
}

void CsrHciUnregisterAclHandlerReqHandler(CsrHciInstData *inst)
{
    CsrHciUnregisterAclHandlerReq *prim = inst->recvMsgP;

    if (prim->aclHandle == 0xFFFF)
    {
        inst->hciAclMainHandler = CSR_SCHED_QID_INVALID;
    }
    else
    {
        CsrHciUnregisterHandle(&(inst->hciAclHandler), prim->aclHandle, prim->queueId);
    }

    CsrHciSendUnregisterAclHandlerCfm(prim->queueId, prim->aclHandle);
}

void CsrHciRegisterScoHandlerReqHandler(CsrHciInstData *inst)
{
    CsrHciRegisterScoHandlerReq *prim = inst->recvMsgP;

    CsrHciRegisterHandle(&(inst->hciScoHandler), prim->scoHandle, prim->queueId);

    CsrHciSendRegisterScoHandlerCfm(prim->queueId, prim->scoHandle);
}

void CsrHciUnregisterScoHandlerReqHandler(CsrHciInstData *inst)
{
    CsrHciUnregisterScoHandlerReq *prim = inst->recvMsgP;

    CsrHciUnregisterHandle(&(inst->hciScoHandler), prim->scoHandle, prim->queueId);

    CsrHciSendUnregisterScoHandlerCfm(prim->queueId, prim->scoHandle);
}

void CsrHciRegisterVendorSpecificEventHandlerReqHandler(CsrHciInstData *inst)
{
    CsrHciRegisterVendorSpecificEventHandlerReq *prim = inst->recvMsgP;

#ifndef CSR_BLUECORE_ONOFF
    if (inst->state == CSR_HCI_STATE_IDLE) /* In idle state we only allow BCCMD and HQ to register because they need to bootstrap and receive error events from chip */
    {
        if (!((prim->channel == TRANSPORT_CHANNEL_BCCMD) || (prim->channel == TRANSPORT_CHANNEL_HQ)))
        {
            CsrHciSaveMessage(inst);
            return;
        }
    }
#endif
    CsrHciRegisterHandle(&(inst->hciVendorEventHandler), prim->channel, prim->queueId);

    CsrHciSendRegisterVendorSpecificEventHandlerCfm(prim->queueId, prim->channel);
}

void CsrHciUnregisterVendorSpecificEventHandlerReqHandler(CsrHciInstData *inst)
{
    CsrHciUnregisterVendorSpecificEventHandlerReq *prim = inst->recvMsgP;

    CsrHciUnregisterHandle(&(inst->hciVendorEventHandler), prim->channel, prim->queueId);

    CsrHciSendUnregisterVendorSpecificEventHandlerCfm(prim->queueId, prim->channel);
}

void CsrHciCommandReqHandler(CsrHciInstData *inst)
{
    CsrHciCommandReq *prim = inst->recvMsgP;

#ifdef CSR_BLUECORE_ONOFF
    if (CSR_TM_BLUECORE_TRANSPORT_STARTED(thdl))
#endif
    {
        csrHciPrepareTransport(TRANSPORT_CHANNEL_HCI, TRANSPORT_RELIABLE_CHANNEL, prim->payload, prim->payloadLength);
    }
#ifdef CSR_BLUECORE_ONOFF
    else
    {
        CsrPmemFree(prim->payload);
    }
#endif
}

void CsrHciAclDataReqHandler(CsrHciInstData *inst)
{
    CsrHciAclDataReq *prim = inst->recvMsgP;

#ifdef CSR_BLUECORE_ONOFF
    if (CSR_TM_BLUECORE_TRANSPORT_STARTED(thdl))
#endif
    {
        CsrUint16 length = CsrMblkGetLength(prim->data);
        CsrUint8 *payload = CsrPmemAlloc(length + CSR_HCI_ACL_HDR_SIZE);
        (void) CsrMblkCopyToMemory(prim->data, 0, length, &(payload[CSR_HCI_ACL_HDR_SIZE]));
        CsrHciInsertAclHeader(payload, prim->handlePlusFlags, length);
        csrHciPrepareTransport(TRANSPORT_CHANNEL_ACL, TRANSPORT_RELIABLE_CHANNEL, payload, length + CSR_HCI_ACL_HDR_SIZE);
    }

    CsrMblkDestroy(prim->data);
    prim->data = NULL;
}

void CsrHciScoDataReqHandler(CsrHciInstData *inst)
{
    CsrHciScoDataReq *prim = inst->recvMsgP;

#ifdef CSR_BLUECORE_ONOFF
    if (CSR_TM_BLUECORE_TRANSPORT_STARTED(thdl))
#endif
    {
        CsrUint16 length = CsrMblkGetLength(prim->data);
        CsrUint8 *payload = CsrPmemAlloc(length + CSR_HCI_SCO_HDR_SIZE);
        (void) CsrMblkCopyToMemory(prim->data, 0, length, &(payload[CSR_HCI_SCO_HDR_SIZE]));
        CsrHciInsertScoHeader(payload, prim->handlePlusFlags, length);
        csrHciPrepareTransport(TRANSPORT_CHANNEL_SCO, TRANSPORT_RELIABLE_CHANNEL, payload, length + CSR_HCI_SCO_HDR_SIZE);
    }

    CsrMblkDestroy(prim->data);
    prim->data = NULL;
}

void CsrHciVendorSpecificCommandReqHandler(CsrHciInstData *inst)
{
    CsrHciVendorSpecificCommandReq *prim = inst->recvMsgP;
    CsrUint16 length;

#ifdef CSR_BLUECORE_ONOFF
    if (!CSR_TM_BLUECORE_TRANSPORT_STARTED(thdl))
    {
        /* Discard everything when transport is not started */
        CsrMblkDestroy(prim->data);
        prim->data = NULL;
        return;
    }
#else
    if (inst->state == CSR_HCI_STATE_IDLE) /* In idle state we only allow BCCMD and HQ to send data because they need to bootstrap and receive error events from chip */
    {
        if (!((prim->channel == TRANSPORT_CHANNEL_BCCMD) || (prim->channel == TRANSPORT_CHANNEL_HQ)))
        {
            CsrHciSaveMessage(inst);
            return;
        }
    }
#endif

    length = CsrMblkGetLength(prim->data);

    if ((prim->channel < 16) && (CsrTmBlueCoreTransportQuery(thdl) == TRANSPORT_TYPE_BCSP))
    {
        CsrUint8 *payload;
        /* On BCSP channels 0-15 are mapped as logical channels in the transport protocol so we do not send those as vendor specific HCI cmds */
        payload = CsrPmemAlloc(length);

        (void) CsrMblkCopyToMemory(prim->data, 0, length, payload);

        csrHciPrepareTransport(prim->channel, TRANSPORT_RELIABLE_CHANNEL, payload, length);
    }
    else if (prim->channel <= 63) /* On all other transports than BCSP, the channels 0-15 are sent as vendor specific HCI commands,
         and for all transports incl BCSP the channels 16-63 are always routed as vendor specific HCI commands */
    {
        CsrUint8 *hciExtCmd;
        CsrUint16 count = 0;

        if (length <= CSR_HCI_MAX_HCI_CMD_PAYLOAD_SIZE) /* MAX HCI cmd packet size */
        {
            hciExtCmd = (CsrUint8 *) CsrPmemAlloc(length + CSR_HCI_COMMAND_VENDOR_HDR_SIZE);
            hciExtCmd[0] = 0x00;    /* 2 bytes HCI ext. opcode corresponding to OGF=0x3f, OCF=0x00 */
            hciExtCmd[1] = 0xFC;
            hciExtCmd[2] = length + 1;
            hciExtCmd[3] = CSR_HCI_UNFRAGMENTED | prim->channel; /* unfragmented packet, tunneled channel id */

            (void) CsrMblkCopyToMemory(prim->data, 0, length, &hciExtCmd[CSR_HCI_COMMAND_VENDOR_HDR_SIZE]);

#ifdef CSR_LOG_ENABLE
            if ((prim->channel == TRANSPORT_CHANNEL_BCCMD) || (prim->channel == TRANSPORT_CHANNEL_HQ) || (prim->channel == TRANSPORT_CHANNEL_VM))
            {
                CSR_LOG_BCI(prim->channel, FALSE, length, &hciExtCmd[CSR_HCI_COMMAND_VENDOR_HDR_SIZE]);
            }
#endif

            csrHciPrepareTransport(TRANSPORT_CHANNEL_HCI, TRANSPORT_RELIABLE_CHANNEL, hciExtCmd, length + CSR_HCI_COMMAND_VENDOR_HDR_SIZE);
        }
        else    /* fragmentation is required, packet too big */
        {
            CsrBool firstFragment = TRUE;

            while (length > 0)
            {
                if (length > CSR_HCI_MAX_HCI_CMD_PAYLOAD_SIZE)
                {
                    hciExtCmd = (CsrUint8 *) CsrPmemAlloc(CSR_HCI_MAX_HCI_CMD_PAYLOAD_SIZE + CSR_HCI_COMMAND_VENDOR_HDR_SIZE);
                    hciExtCmd[0] = 0x00;
                    hciExtCmd[1] = 0xFC;
                    hciExtCmd[2] = CSR_HCI_MAX_HCI_CMD_PAYLOAD_SIZE + 1;

                    if (firstFragment)
                    {
                        hciExtCmd[3] = CSR_HCI_FRAGMENT_START | prim->channel; /* first fragment packet, tunneled channel id */
                        firstFragment = FALSE;
                        count = 0;
                    }
                    else
                    {
                        hciExtCmd[3] = prim->channel; /* continuation packet, tunneled channel id */
                    }

                    (void) CsrMblkCopyToMemory(prim->data, count, length, &hciExtCmd[CSR_HCI_COMMAND_VENDOR_HDR_SIZE]);

#ifdef CSR_LOG_ENABLE
                    if ((prim->channel == TRANSPORT_CHANNEL_BCCMD) || (prim->channel == TRANSPORT_CHANNEL_HQ) || (prim->channel == TRANSPORT_CHANNEL_VM))
                    {
                        CSR_LOG_BCI(prim->channel, FALSE, length, &hciExtCmd[CSR_HCI_COMMAND_VENDOR_HDR_SIZE]);
                    }
#endif

                    csrHciPrepareTransport(TRANSPORT_CHANNEL_HCI, TRANSPORT_RELIABLE_CHANNEL, hciExtCmd, length + CSR_HCI_COMMAND_VENDOR_HDR_SIZE);

                    length -= CSR_HCI_MAX_HCI_CMD_PAYLOAD_SIZE;
                    count += CSR_HCI_MAX_HCI_CMD_PAYLOAD_SIZE;
                }
                else
                {
                    hciExtCmd = (CsrUint8 *) CsrPmemAlloc(length + CSR_HCI_COMMAND_VENDOR_HDR_SIZE);
                    hciExtCmd[0] = 0x00;
                    hciExtCmd[1] = 0xFC;
                    hciExtCmd[2] = length + 1;
                    hciExtCmd[3] = CSR_HCI_FRAGMENT_END | prim->channel; /*  last fragmented packet, tunneled channel id */

                    (void) CsrMblkCopyToMemory(prim->data, count, length, &hciExtCmd[CSR_HCI_COMMAND_VENDOR_HDR_SIZE]);

#ifdef CSR_LOG_ENABLE
                    if ((prim->channel == TRANSPORT_CHANNEL_BCCMD) || (prim->channel == TRANSPORT_CHANNEL_HQ) || (prim->channel == TRANSPORT_CHANNEL_VM))
                    {
                        CSR_LOG_BCI(prim->channel, FALSE, length, &hciExtCmd[CSR_HCI_COMMAND_VENDOR_HDR_SIZE]);
                    }
#endif

                    csrHciPrepareTransport(TRANSPORT_CHANNEL_HCI, TRANSPORT_RELIABLE_CHANNEL, hciExtCmd, length + CSR_HCI_COMMAND_VENDOR_HDR_SIZE);

                    length = 0;
                }
            }
        }
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrHciLto, 0,
                              "CsrHciVendorSpecificCommandReqHandler somebody tried to send data to a channel number higher than 63 which is illegal"));
    }

    CsrMblkDestroy(prim->data);
    prim->data = NULL;
}

#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
/* This function is needed to send a COLD RESET without using the queues in the bccmd task,
   when testing the chip manager. (implemented in csr_hci_sef.c) */
void CsrBcCmTestHciDriverSendBcCommand(CsrUint8 *msg, CsrUint32 msg_len)
{
    csrHciPrepareTransport(TRANSPORT_CHANNEL_BCCMD, TRANSPORT_RELIABLE_CHANNEL, msg, msg_len);
}

#endif

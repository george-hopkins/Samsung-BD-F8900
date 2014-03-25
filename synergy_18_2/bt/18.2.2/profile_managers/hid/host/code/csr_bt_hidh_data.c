/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_tasks.h"
#include "bluetooth.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_hidh_main.h"
#include "csr_bt_hidh_prim.h"
#include "csr_bt_hidh_util.h"
#include "csr_bt_hidh_sigup.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      csrBtHidhCml2caDataSendAck (internal fct.)
 *
 *  DESCRIPTION
 *      Checks downlink data queue for additional transmission upon data confirm.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
static void csrBtHidhCml2caDataSendAck(l2ca_data_ch_t *ch)
{
    if(ch->qFirst != NULL)
    {
        q_element_t *tmp = ch->qFirst;
        ch->qFirst = ch->qFirst->next;

        CsrBtCml2caDataReqSend(ch->cid, tmp->length, tmp->data);
        ch->sending = TRUE;

        CsrPmemFree(tmp);

        if( ch->qFirst == NULL)
        {
            ch->qLast = NULL;
        }
    }
    else
    {
        ch->sending = FALSE;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      csrBtHidhCml2caDataSend (internal fct.)
 *
 *  DESCRIPTION
 *      Send a downlink data message or queue message if already data on queue
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
static void csrBtHidhCml2caDataSend(l2ca_data_ch_t *ch, CsrUint16 dataLen, void *data)
{
    if(( ch->sending) || (ch->qFirst != NULL))
    {
        /* q the data */
        q_element_t    *tmp;

        tmp = CsrPmemAlloc(sizeof(q_element_t));
        tmp->length = dataLen;
        tmp->data = data;
        tmp->next = NULL;

        if( ch->qFirst == NULL)
        {
            ch->qFirst = ch->qLast = tmp;
        }
        else
        {
            ch->qLast->next = tmp;
            ch->qLast = tmp;
        }
    }
    else
    {
        CsrBtCml2caDataReqSend(ch->cid, dataLen, data);
        ch->sending = TRUE;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhCmL2caDataCfm
 *
 *  DESCRIPTION
 *      Handle a confirm of downlink data transmission
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhCmL2caDataCfm(hidh_instData_t *instData)
{
    CsrUint8                 deviceId;
    CsrBtCmL2caDataCfm    *prim;

    prim = (CsrBtCmL2caDataCfm *) instData->recvMsgP;

    if( (deviceId = getDeviceIdFromCid(prim->btConnId, instData)) != NOT_FOUND)
    {
        l2ca_data_ch_t        *ch;
        hidh_subInstData_t    *subInstData = &instData->subInstData[deviceId];

        if( subInstData->intr_ch.cid == prim->btConnId)
        {
            ch = &subInstData->intr_ch;
        }
        else
        {
            ch = &subInstData->ctrl_ch;
        }

        ch->sending = FALSE;

        csrBtHidhCml2caDataSendAck(ch);
    }
}

#define HIDH_SUCCESS                 (0x00)
#define HIDH_NOT_READY               (0x01)
#define HIDH_ERR_INVALID_REPORT_ID   (0x02)
#define HIDH_ERR_UNSUPPORTED         (0x03)
#define HIDH_ERR_INVALID_PARAMETER   (0x04)
#define HIDH_ERR_UNKNOWN             (0x0E)
#define HIDH_ERR_FATAL               (0x0F)

static CsrBtResultCode csrBtHidhMapToResultCode(CsrUint8 handshakeResult)
{
    switch(handshakeResult & 0x0F)
    {
        case HIDH_SUCCESS:
            return CSR_BT_RESULT_CODE_HIDH_SUCCESS;
        case HIDH_NOT_READY:
            return CSR_BT_RESULT_CODE_HIDH_NOT_READY;
        case HIDH_ERR_INVALID_REPORT_ID:
            return CSR_BT_RESULT_CODE_HIDH_INVALID_REPORT_ID;
        case HIDH_ERR_UNSUPPORTED:
            return CSR_BT_RESULT_CODE_HIDH_UNSUPPORTED;
        case HIDH_ERR_INVALID_PARAMETER:
            return CSR_BT_RESULT_CODE_HIDH_INVALID_PARAMETER;
        case HIDH_ERR_FATAL:
            return CSR_BT_RESULT_CODE_HIDH_FATAL;
        default:
            return CSR_BT_RESULT_CODE_HIDH_UNKNOWN;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhCmL2caDataInd
 *
 *  DESCRIPTION
 *      Handle an uplink data message indication
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhCmL2caDataInd(hidh_instData_t *instData)
{
    CsrUint8                 deviceId;
    CsrBtCmL2caDataInd    *prim;

    prim = (CsrBtCmL2caDataInd *) instData->recvMsgP;

    CsrBtCmL2caDataResSend(prim->btConnId);
    if( (deviceId = getDeviceIdFromCid(prim->btConnId, instData)) != NOT_FOUND)
    {
        l2ca_data_ch_t        *ch;
        CsrSchedQid            appHandle;

        hidh_subInstData_t *subInstData = &instData->subInstData[deviceId];

        if( subInstData->intr_ch.cid == prim->btConnId)
        {
            ch = &subInstData->intr_ch;
        }
        else
        {
            ch = &subInstData->ctrl_ch;
        }

        if( ch->rxMsg != NULL )
        {
            appHandle = subInstData->userHandle;

            if( prim->length < ch->localMtu)
            {
                CsrMemCpy(&(ch->rxMsg[ch->rxMsgCount]), prim->payload + 1, prim->length - 1);
                ch->rxMsgCount += prim->length - 1;
                CsrPmemFree(prim->payload);

                CsrBtHidhDataIndSend(appHandle, deviceId, ch->rxMsgCount, ch->rxMsg);
                ch->rxMsg = NULL;
            }
            else if( prim->length == 1)
            {
                CsrPmemFree(prim->payload);
                CsrBtHidhDataIndSend(appHandle, deviceId, ch->rxMsgCount, ch->rxMsg);
                ch->rxMsg = NULL;
            }
            else
            {
                CsrUint8 *tmp;

                ch->rxMsgAllocCount += ch->localMtu;
                tmp = CsrPmemAlloc(ch->rxMsgAllocCount);
                CsrMemCpy(tmp, ch->rxMsg, ch->rxMsgCount);
                CsrPmemFree(ch->rxMsg);
                ch->rxMsg = tmp;

                CsrMemCpy(&(ch->rxMsg[ch->rxMsgCount]), prim->payload + 1, prim->length - 1);
                ch->rxMsgCount += prim->length - 1;
                CsrPmemFree(prim->payload);
            }
        }
        else
        {
            if( prim->length < ch->localMtu )
            {
                CsrUint8    transactionType = prim->payload[0] & 0xF0;

                if( transactionType > CSR_BT_HIDH_SET_IDLE )
                {
                    appHandle = subInstData->userHandle;
                    CsrBtHidhDataIndSend(appHandle, deviceId, prim->length, prim->payload);
                }
                else if( transactionType == CSR_BT_HIDH_CONTROL)
                {
                    appHandle = instData->ctrlHandle;
                    CsrBtHidhControlIndSend(appHandle, deviceId, (CsrBtHidhCtrl)(prim->payload[0] & 0x0F));
                    CsrPmemFree(prim->payload);
                }
                else if( transactionType == CSR_BT_HIDH_HANDSHAKE)
                {
                    appHandle = subInstData->userHandle;
                    CsrBtHidhHandshakeIndSend(appHandle, deviceId, csrBtHidhMapToResultCode(prim->payload[0]));
                    CsrPmemFree(prim->payload);
                }
                else
                {
                    /* unexpected transaction type */
                    CsrPmemFree(prim->payload);
                }
            }
            else
            {
                ch->rxMsgAllocCount = ch->localMtu<<1;
                ch->rxMsg = CsrPmemAlloc(ch->rxMsgAllocCount);

                CsrMemCpy(ch->rxMsg, prim->payload, prim->length);
                ch->rxMsgCount = prim->length;
                CsrPmemFree(prim->payload);
            }
        }
    }
    else
    {
        CsrPmemFree(prim->payload);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhControlReqHandler
 *
 *  DESCRIPTION
 *      Handle a control request
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhControlReqHandler(hidh_instData_t *instData)
{
    CsrBtHidhControlReq    *prim;

    prim = (CsrBtHidhControlReq *) instData->recvMsgP;

    if( prim->deviceId != NOT_FOUND)
    {
        hidh_subInstData_t *subInstData = &instData->subInstData[prim->deviceId];

        if(subInstData->subState == CONNECTED)
        {
            CsrUint8    *control = CsrPmemAlloc(1);

            *control = prim->operation & 0x0F;

            if( *control ==  CSR_BT_HIDH_VC_UNPLUG )
            {
                CSR_BT_HIDH_SUB_STATE_CHANGE(prim->deviceId, UNPLUGGING);
            }

            *control |= CSR_BT_HIDH_CONTROL;

            csrBtHidhCml2caDataSend(&subInstData->ctrl_ch, 1, control);
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhGetReportReqHandler
 *
 *  DESCRIPTION
 *      Handle a get report request
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhGetReportReqHandler(hidh_instData_t *instData)
{
    CsrBtHidhGetReportReq    *prim;

    prim = (CsrBtHidhGetReportReq *) instData->recvMsgP;

    if( prim->deviceId != NOT_FOUND)
    {
        hidh_subInstData_t *subInstData = &instData->subInstData[prim->deviceId];

        if(subInstData->subState == CONNECTED)
        {
            CsrUint8    *get_report, i, size = 1;

            if( prim->reportId != 0)
                size++;
            if( prim->bufferSize != 0 )
                size += 2;

            get_report = CsrPmemAlloc(size);

            i = 0;
            get_report[i] = prim->reportType & 0x0F;

            if( prim->bufferSize != 0)
            {
                get_report[i] |= 0x08;
            }
            get_report[i++] |= CSR_BT_HIDH_GET_REPORT;

            if( prim->reportId != 0)
            {
                get_report[i++] = prim->reportId;
            }

            if( prim->bufferSize != 0 )
            {
                get_report[i++] = (CsrUint8) (prim->bufferSize & 0x00FF);
                get_report[i++] = prim->bufferSize >> 8;
            }

            csrBtHidhCml2caDataSend(&subInstData->ctrl_ch, size, get_report);
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSetReportReqHandler
 *
 *  DESCRIPTION
 *      Handle a set report request
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSetReportReqHandler(hidh_instData_t *instData)
{
    CsrBtHidhSetReportReq    *prim;

    prim = (CsrBtHidhSetReportReq *) instData->recvMsgP;

    if( prim->deviceId != NOT_FOUND)
    {
        hidh_subInstData_t *subInstData = &instData->subInstData[prim->deviceId];

        if(subInstData->subState == CONNECTED)
        {
            if( prim->reportLen < subInstData->ctrl_ch.mtu )
            {
                prim->report[0] = CSR_BT_HIDH_SET_REPORT | prim->reportType;

                csrBtHidhCml2caDataSend(&subInstData->ctrl_ch, prim->reportLen, prim->report);
            }
            else
            {
                /* fragmentation required */
                CsrUint8       *fragment;
                CsrUint16    count = 1;
                short        size = prim->reportLen - 1;
                CsrBool        firstFrag = TRUE;

                while( size >= 0)
                {
                    if( size >= subInstData->ctrl_ch.mtu - 1)
                    {
                        fragment = (CsrUint8 *) CsrPmemAlloc(subInstData->ctrl_ch.mtu);

                        if( firstFrag )
                        {
                            firstFrag = FALSE;
                            fragment[0] = CSR_BT_HIDH_SET_REPORT | prim->reportType;
                        }
                        else
                        {
                            fragment[0] = CSR_BT_HIDH_DATC | prim->reportType;
                        }

                        CsrMemCpy(&fragment[1], &prim->report[count], subInstData->ctrl_ch.mtu - 1);

                        csrBtHidhCml2caDataSend(&subInstData->ctrl_ch, subInstData->ctrl_ch.mtu, fragment);

                        count += subInstData->ctrl_ch.mtu - 1;
                        size -= subInstData->ctrl_ch.mtu - 1;
                    }
                    else
                    {
                        fragment = (CsrUint8 *) CsrPmemAlloc(size+1);

                        fragment[0] = CSR_BT_HIDH_DATC | prim->reportType;

                        CsrMemCpy(&fragment[1], &prim->report[count], size);

                        csrBtHidhCml2caDataSend(&subInstData->ctrl_ch, (CsrUint16) (size+1), fragment);
                        break;
                    }
                }
                CsrPmemFree (prim->report);
            }
        }
        else
        {
            CsrPmemFree(prim->report);
        }
    }
    else
    {
        CsrPmemFree(prim->report);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhGetProtocolReqHandler
 *
 *  DESCRIPTION
 *      Handle a get protocol request
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhGetProtocolReqHandler(hidh_instData_t *instData)
{
    CsrBtHidhGetProtocolReq    *prim;

    prim = (CsrBtHidhGetProtocolReq *) instData->recvMsgP;

    if( prim->deviceId != NOT_FOUND)
    {
        hidh_subInstData_t *subInstData = &instData->subInstData[prim->deviceId];

        if(subInstData->subState == CONNECTED)
        {
            CsrUint8    *get_protocol = CsrPmemAlloc(1);

            *get_protocol = CSR_BT_HIDH_GET_PROTOCOL;

            csrBtHidhCml2caDataSend(&subInstData->ctrl_ch, 1, get_protocol);
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSetProtocolReqHandler
 *
 *  DESCRIPTION
 *      Handle a set protocol request
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSetProtocolReqHandler(hidh_instData_t *instData)
{
    CsrBtHidhSetProtocolReq    *prim;

    prim = (CsrBtHidhSetProtocolReq *) instData->recvMsgP;

    if( prim->deviceId != NOT_FOUND)
    {
        hidh_subInstData_t *subInstData = &instData->subInstData[prim->deviceId];

        if(subInstData->subState == CONNECTED)
        {
            CsrUint8    *set_protocol = CsrPmemAlloc(1);

            *set_protocol = prim->protocol & 0x01;
            *set_protocol |= CSR_BT_HIDH_SET_PROTOCOL;

            csrBtHidhCml2caDataSend(&subInstData->ctrl_ch, 1, set_protocol);
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhGetIdleReqHandler
 *
 *  DESCRIPTION
 *      Handle a get idle request
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhGetIdleReqHandler(hidh_instData_t *instData)
{
    CsrBtHidhGetIdleReq    *prim;

    prim = (CsrBtHidhGetIdleReq *) instData->recvMsgP;

    if( prim->deviceId != NOT_FOUND)
    {
        hidh_subInstData_t *subInstData = &instData->subInstData[prim->deviceId];

        if(subInstData->subState == CONNECTED)
        {
            CsrUint8    *get_idle = CsrPmemAlloc(1);

            *get_idle = CSR_BT_HIDH_GET_IDLE;

            csrBtHidhCml2caDataSend(&subInstData->ctrl_ch, 1, get_idle);
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSetIdleReqHandler
 *
 *  DESCRIPTION
 *      Handle a set idle request
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSetIdleReqHandler(hidh_instData_t *instData)
{
    CsrBtHidhSetIdleReq    *prim;

    prim = (CsrBtHidhSetIdleReq *) instData->recvMsgP;

    if( prim->deviceId != NOT_FOUND)
    {
        hidh_subInstData_t *subInstData = &instData->subInstData[prim->deviceId];

        if(subInstData->subState == CONNECTED)
        {
            CsrUint8    *set_idle = CsrPmemAlloc(2);

            set_idle[0] = CSR_BT_HIDH_SET_IDLE;
            set_idle[1] = prim->idleRate;

            csrBtHidhCml2caDataSend(&subInstData->ctrl_ch, 2, set_idle);
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhDataReqHandler
 *
 *  DESCRIPTION
 *      Handle a data request
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhDataReqHandler(hidh_instData_t *instData)
{
    CsrBtHidhDataReq    *prim;

    prim = (CsrBtHidhDataReq *) instData->recvMsgP;

    if( prim->deviceId != NOT_FOUND)
    {
        hidh_subInstData_t *subInstData = &instData->subInstData[prim->deviceId];

        if(subInstData->subState == CONNECTED)
        {
            if( prim->dataLen < subInstData->intr_ch.mtu )
            {
                prim->data[0] = CSR_BT_HIDH_DATA | prim->reportType;

                if( subInstData->useExtDataPath && instData->extSendMsgFct)
                {
                    instData->extSendMsgFct(subInstData->intr_ch.cid, prim->data, prim->dataLen);
                }
                else
                {
                    csrBtHidhCml2caDataSend(&subInstData->intr_ch, prim->dataLen, prim->data);
                }
            }
            else
            {
                /* fragmentation required */
                CsrUint8       *fragment;
                CsrUint16    count = 1;
                short        size = prim->dataLen - 1;
                CsrBool        firstFrag = TRUE;

                while( size >= 0)
                {
                    if( size >= subInstData->intr_ch.mtu - 1)
                    {
                        fragment = (CsrUint8 *) CsrPmemAlloc(subInstData->intr_ch.mtu);

                        if( firstFrag )
                        {
                            firstFrag = FALSE;
                            fragment[0] = CSR_BT_HIDH_DATA | prim->reportType;
                        }
                        else
                        {
                            fragment[0] = CSR_BT_HIDH_DATC | prim->reportType;
                        }

                        CsrMemCpy(&fragment[1], &prim->data[count], subInstData->intr_ch.mtu - 1);

                        csrBtHidhCml2caDataSend(&subInstData->intr_ch, subInstData->intr_ch.mtu, fragment);

                        count += subInstData->intr_ch.mtu - 1;
                        size -= subInstData->intr_ch.mtu - 1;
                    }
                    else
                    {
                        fragment = (CsrUint8 *) CsrPmemAlloc(size+1);

                        fragment[0] = CSR_BT_HIDH_DATC | prim->reportType;

                        CsrMemCpy(&fragment[1], &prim->data[count], size);

                        csrBtHidhCml2caDataSend(&subInstData->intr_ch, (CsrUint16) (size+1), fragment);
                        break;
                    }
                }
            }
        }
        else
        {
            CsrPmemFree(prim->data);
        }
    }
    else
    {
        CsrPmemFree(prim->data);
    }
}


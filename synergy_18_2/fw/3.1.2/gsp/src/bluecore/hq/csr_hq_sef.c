/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_hq_handler.h"
#include "csr_panic.h"
#include "csr_hq_prim.h"
#include "csr_macro.h"
#include "csr_hq_subscriber.h"
#include "csr_hci_lib.h"
#include "csr_util.h"
#include "csr_transport.h"
#ifdef CSR_CHIP_MANAGER_ENABLE
#include "csr_bccmd_lib.h"
#include "csr_hq_task.h"
#endif

#ifdef CSR_LOG_ENABLE
#include "csr_log_gsp.h"
#endif

#define HQ_COMMAND_HEADER_LENGTH   10                    /* Length of HQ command header in bytes */
#define HQ_COMMAND_MINIMUM_LENGTH  24

#define HQ_GET_RESP ((CsrUint8) 0x01)

static void csrSendHqData(CsrUint8 *payload, CsrUint16 payloadLength)
{
    CsrMblk *data = CsrMblkDataCreate(payload, payloadLength, TRUE);

    CsrHciVendorSpecificCommandReqSend(TRANSPORT_CHANNEL_HQ, data);
}

#ifdef CSR_CHIP_MANAGER_ENABLE
#define CSR_BCCMD_COLD_RESET_VARID         (0x4001)
static void csrHqColdResetMsgSend(void)
{
    CsrBccmdReadReqSend(CSR_HQ_IFACEQUEUE, CSR_BCCMD_COLD_RESET_VARID, 0, 0, NULL);
}

#endif

void CsrHqRegisterReqHandler(CsrHqInstanceData *hqInst, CsrHqRegisterReq *request)
{
    CsrHqRegisterCfm *confirm;

    CsrHqInsertSubscriber(hqInst, request->phandle, request->varId, request->response);

    confirm = CsrPmemAlloc(sizeof(CsrHqRegisterCfm));
    confirm->type = CSR_HQ_REGISTER_CFM;
    confirm->varId = request->varId;
    confirm->result = CSR_RESULT_SUCCESS;
    CsrSchedMessagePut(request->phandle, CSR_HQ_PRIM, confirm);
}

void CsrHqDeregisterReqHandler(CsrHqInstanceData *hqInst, CsrHqDeregisterReq *request)
{
    CsrHqDeregisterCfm *confirm;

    confirm = CsrPmemAlloc(sizeof(CsrHqDeregisterCfm));
    confirm->type = CSR_HQ_DEREGISTER_CFM;
    confirm->varId = request->varId;
    confirm->result = CsrHqRemoveSubscriberSpecific(hqInst, request);
    CsrSchedMessagePut(request->phandle, CSR_HQ_PRIM, confirm);
}

void CsrHqMsgResHandler(CsrHqInstanceData *hqInst, CsrHqMsgRes *response)
{
    CsrUint16 dataLength;
    CsrUint8 *data, *tmpData;

#ifdef CSR_BLUECORE_ONOFF
    if (!hqInst->deactivating) /* Force drop messages when deactivating */
#endif
    {
        dataLength = CSRMAX(response->payloadLength * sizeof(CsrUint16) + HQ_COMMAND_HEADER_LENGTH,
            HQ_COMMAND_MINIMUM_LENGTH);
        tmpData = data = CsrPmemZalloc(dataLength);

        CSR_ADD_UINT16_TO_XAP(tmpData, response->cmdType);
        CSR_ADD_UINT16_TO_XAP(tmpData, dataLength / 2);
        CSR_ADD_UINT16_TO_XAP(tmpData, response->seqNo);
        CSR_ADD_UINT16_TO_XAP(tmpData, response->varId);
        CSR_ADD_UINT16_TO_XAP(tmpData, response->status);
        CsrMemCpy(tmpData, response->payload, response->payloadLength);
        csrSendHqData(data, dataLength);
    }

    CsrPmemFree(response->payload);
    response->payload = NULL;
}
#include <stdio.h>
void CsrHqHciVendorSpecificEventIndHandler(CsrHqInstanceData *hqInst, CsrHciVendorSpecificEventInd *indication)
{
    CsrHqVarIdType varId;
    CsrUint16 dataLen;
    CsrUint8 *msg;
    CsrBool response = FALSE;
    CsrHqSubscription *element;
#ifndef CSR_BLUECORE_ONOFF
    CsrBool handled = FALSE;
#endif

#ifdef CSR_BLUECORE_ONOFF
    if (hqInst->deactivating) /* Force drop messages when deactivating */
    {
        CsrMblkDestroy(indication->data);
        indication->data = NULL;
        return;
    }
#endif

    dataLen = CsrMblkGetLength(indication->data);
    msg = CsrMblkMap(indication->data, 0, dataLen);

    varId = (((CsrHqVarIdType) msg[7]) << 8) | ((CsrHqVarIdType) msg[6]);

    for (element = (CsrHqSubscription *) CsrCmnListGetFirst(&hqInst->subscribers);
         element != NULL;
         element = element->next)
    {
        if (element->varId == varId)
        {
            CsrHqMsgInd *newPrim;
            CsrUint8 *tmpMsg = msg;

            newPrim = CsrPmemAlloc(sizeof(CsrHqMsgInd));
            newPrim->type = CSR_HQ_MSG_IND;
            CSR_CONVERT_16_FROM_XAP(newPrim->cmdType, tmpMsg);
            CSR_CONVERT_16_FROM_XAP(newPrim->payloadLength, tmpMsg);
            newPrim->payloadLength = (newPrim->payloadLength << 1) - HQ_COMMAND_HEADER_LENGTH;
            CSR_CONVERT_16_FROM_XAP(newPrim->seqNo, tmpMsg);
            newPrim->varId = varId;

            newPrim->payload = CsrPmemAlloc(newPrim->payloadLength);
            CsrMemCpy(newPrim->payload, (msg + HQ_COMMAND_HEADER_LENGTH), newPrim->payloadLength);

            /* Forward copies of the HQ event to subscriber */
            CsrSchedMessagePut(element->appHandle, CSR_HQ_PRIM, newPrim);

            if (element->response)
            {
                response = TRUE;
            }
#ifndef CSR_BLUECORE_ONOFF
            handled = TRUE;
#endif
        }
    }

#ifndef CSR_BLUECORE_ONOFF
    if (!handled)
    {
        switch (varId)
        {
            case CSR_HQVARID_DELAYED_PANIC:
            case CSR_HQVARID_FAULT:
            {
				int i;
				extern CsrBool isCSRChipInit;
#ifdef __CSR_COUNT_WD__
				extern void CsrAppBtGapChipWdCount(CsrUint16 );
				CsrAppBtGapChipWdCount(varId);
#endif				

				printf("varId %x\n",varId);
				printf("payload \n");
				for(i=0; i<dataLen; i++)
					printf("%x ",msg[i]);
				
#ifdef CSR_CHIP_MANAGER_ENABLE
                csrHqColdResetMsgSend();
#else
#ifdef CSR_BT_APP_SAMSUNG
				if(isCSRChipInit)
#endif					
				{
	                CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE,
	                    "unexpected value");
				}
#endif
                break;
            }
            default:
            {
                /* we choose to not save unreceived messages by the HQ */
                break;  /* silently discard this HQ event */
            }
        }
    }
#endif /* !CSR_BLUECORE_ONOFF */

    if (!response)
    {
        /* Send BC_HQ_MSG_Response */
        CsrUint8 *respMsg = CsrPmemAlloc(dataLen);
        CsrMemCpy(respMsg, msg, dataLen);
        respMsg[0] = HQ_GET_RESP; /* type */
        respMsg[1] = 0x00;
        respMsg[8] = 0x00; /* status */
        respMsg[9] = 0x00; /* status */
        csrSendHqData(respMsg, dataLen);
    }

    CsrMblkUnmap(indication->data, msg);
    CsrMblkDestroy(indication->data);
}

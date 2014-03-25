/****************************************************************************

       Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
      
                    All rights reserved.

FILE:            csr_bt_hci_convert_lmdownstream.c

DESCRIPTION:            converter for _hci _lm commands and events

REVISION:        $Revision: #3 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_lmdownstream.h"
#include "csr_bt_util.h"

#ifdef CSR_CHIP_MANAGER_ENABLE
#include "csr_pmem.h"
#include "csr_hci_task.h"
#include "csr_bt_hci_cm.h"
#include "dm_hci_interface.h"
#endif

#ifdef CSR_CHIP_MANAGER_ENABLE
void csrBcCmHciInquiryStatusEventHandler(CsrUint8 numHciCommandPackets, hci_return_t hciStatus)
{
    CsrBtHciCmHciCommonStatusEventHandler(numHciCommandPackets, hciStatus, HCI_INQUIRY);
}

static void csrBcCmHciInquiryCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_INQUIRY_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_INQUIRY_COMPLETE; 
    evt.event.length              = HCI_EV_INQUIRY_COMPLETE_PARAM_LEN;
    evt.status                    = HCI_ERROR_UNSPECIFIED;
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
    CSR_UNUSED(cmdArg);
    CSR_UNUSED(numHciCommandPackets);
}

void csrBcCmHciInquiryCancelCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_COMMAND_COMPLETE; 
    evt.event.length              = HCI_INQUIRY_CANCEL_ARG_LEN;
    evt.num_hci_command_pkts      = numHciCommandPackets;
    evt.op_code                   = HCI_INQUIRY_CANCEL;
    evt.status                    = HCI_SUCCESS;
    evt.argument_ptr              = NULL;
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
    CSR_UNUSED(cmdArg);
    CSR_UNUSED(numHciCommandPackets);
}

void csrBcCmHciCreateConStatusEventHandler(CsrUint8 numHciCommandPackets, hci_return_t hciStatus)
{
    CsrBtHciCmHciCommonStatusEventHandler(numHciCommandPackets, hciStatus, HCI_CREATE_CONNECTION);
}

void csrBcCmHciConnectionCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_CONN_COMPLETE_T evt;
    evt.event.event_code      = HCI_EV_CONN_COMPLETE; 
    evt.event.length          = HCI_EV_CONN_COMPLETE_PARAM_LEN;
    evt.status                = HCI_ERROR_CONN_TIMEOUT;
    evt.handle                = 0;
    CsrBtBdAddrCopy(&evt.bd_addr, (BD_ADDR_T *) cmdArg);
    evt.link_type             = 1; /*ACL */
    evt.enc_mode              = 0; /* disabled */
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
    CSR_UNUSED(numHciCommandPackets);
}

void csrBcCmHciAcceptConStatusEventHandler(CsrUint8 numHciCommandPackets, hci_return_t hciStatus)
{
    CsrBtHciCmHciCommonStatusEventHandler(numHciCommandPackets, hciStatus, HCI_ACCEPT_CONNECTION_REQ);
}

void csrBcCmHciRejectConStatusEventHandler(CsrUint8 numHciCommandPackets, hci_return_t hciStatus)
{
    CsrBtHciCmHciCommonStatusEventHandler(numHciCommandPackets, hciStatus, HCI_REJECT_CONNECTION_REQ);
}

void csrBcCmHciLinkKeyReplyCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_COMMAND_COMPLETE; 
    evt.event.length              = HCI_LINK_KEY_REQ_REPLY_ARG_LEN;
    evt.num_hci_command_pkts      = numHciCommandPackets;
    evt.op_code                   = HCI_LINK_KEY_REQ_REPLY;
    evt.status                    = HCI_SUCCESS;
    evt.argument_ptr              = CsrPmemAlloc(sizeof(*evt.argument_ptr));
    CsrBtBdAddrCopy(&(evt.argument_ptr->link_key_req_reply_args.bd_addr), (BD_ADDR_T *) cmdArg);
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
}

void csrBcCmHciLinkKeyNegReplyCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_COMMAND_COMPLETE; 
    evt.event.length              = HCI_LINK_KEY_REQ_NEG_REPLY_ARG_LEN;
    evt.num_hci_command_pkts      = numHciCommandPackets;
    evt.op_code                   = HCI_LINK_KEY_REQ_NEG_REPLY;
    evt.status                    = HCI_SUCCESS;
    evt.argument_ptr              = CsrPmemAlloc(sizeof(*evt.argument_ptr));
    CsrBtBdAddrCopy(&(evt.argument_ptr->link_key_reqneg_reply_args.bd_addr), (BD_ADDR_T *) cmdArg);
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
}

void csrBcCmHciPinCodeReplyCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_COMMAND_COMPLETE; 
    evt.event.length              = HCI_PIN_CODE_REQ_REPLY_ARG_LEN;
    evt.num_hci_command_pkts      = numHciCommandPackets;
    evt.op_code                   = HCI_PIN_CODE_REQ_REPLY;
    evt.status                    = HCI_SUCCESS;
    evt.argument_ptr              = CsrPmemAlloc(sizeof(*evt.argument_ptr));
    CsrBtBdAddrCopy(&(evt.argument_ptr->pin_code_req_reply_args.bd_addr), (BD_ADDR_T *) cmdArg);
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
}

void csrBcCmHciPinCodeNegReplyCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_COMMAND_COMPLETE; 
    evt.event.length              = HCI_PIN_CODE_REQ_NEG_REPLY_ARG_LEN;
    evt.num_hci_command_pkts      = numHciCommandPackets;
    evt.op_code                   = HCI_PIN_CODE_REQ_NEG_REPLY;
    evt.status                    = HCI_SUCCESS;
    evt.argument_ptr              = CsrPmemAlloc(sizeof(*evt.argument_ptr));
    CsrBtBdAddrCopy(&(evt.argument_ptr->pin_code_reqneg_reply_args.bd_addr), (BD_ADDR_T *) cmdArg);
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
}

void csrBcCmHciIoCapRespCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_COMMAND_COMPLETE; 
    evt.event.length              = HCI_IO_CAPABILITY_RESPONSE_ARG_LEN;
    evt.num_hci_command_pkts      = numHciCommandPackets;
    evt.op_code                   = HCI_IO_CAPABILITY_RESPONSE;
    evt.status                    = HCI_ERROR_UNSPECIFIED;
    evt.argument_ptr              = CsrPmemAlloc(sizeof(*evt.argument_ptr));
    CsrBtBdAddrCopy(&(evt.argument_ptr->io_capability_response_args.bd_addr), (BD_ADDR_T *) cmdArg);
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
}

void csrBcCmHciIoCapReqNegReplyCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_COMMAND_COMPLETE; 
    evt.event.length              = HCI_IO_CAPABILITY_REQUEST_NEG_REPLY_ARG_LEN;
    evt.num_hci_command_pkts      = numHciCommandPackets;
    evt.op_code                   = HCI_IO_CAPABILITY_REQUEST_NEG_REPLY;
    evt.status                    = HCI_SUCCESS;
    evt.argument_ptr              = CsrPmemAlloc(sizeof(*evt.argument_ptr));
    CsrBtBdAddrCopy(&(evt.argument_ptr->io_capability_request_neg_reply_args.bd_addr), (BD_ADDR_T *) cmdArg);
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
}

void csrBcCmHciUserConfReqReplyCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_COMMAND_COMPLETE; 
    evt.event.length              = HCI_USER_CONFIRMATION_REQUEST_REPLY_ARG_LEN;
    evt.num_hci_command_pkts      = numHciCommandPackets;
    evt.op_code                   = HCI_USER_CONFIRMATION_REQUEST_REPLY;
    evt.status                    = HCI_SUCCESS;
    evt.argument_ptr              = CsrPmemAlloc(sizeof(*evt.argument_ptr));
    CsrBtBdAddrCopy(&(evt.argument_ptr->user_confirmation_request_reply_args.bd_addr), (BD_ADDR_T *) cmdArg);
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
}

void csrBcCmHciUserConfReqNegReplyCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_COMMAND_COMPLETE; 
    evt.event.length              = HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY_ARG_LEN;
    evt.num_hci_command_pkts      = numHciCommandPackets;
    evt.op_code                   = HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY;
    evt.status                    = HCI_SUCCESS;
    evt.argument_ptr              = CsrPmemAlloc(sizeof(*evt.argument_ptr));
    CsrBtBdAddrCopy(&(evt.argument_ptr->user_confirmation_request_neg_reply_args.bd_addr), (BD_ADDR_T *) cmdArg);
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
}

void csrBcCmHciUserPasskeyReqReplyCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_COMMAND_COMPLETE; 
    evt.event.length              = HCI_USER_PASSKEY_REQUEST_REPLY_ARG_LEN;
    evt.num_hci_command_pkts      = numHciCommandPackets;
    evt.op_code                   = HCI_USER_PASSKEY_REQUEST_REPLY;
    evt.status                    = HCI_SUCCESS;
    evt.argument_ptr              = CsrPmemAlloc(sizeof(*evt.argument_ptr));
    CsrBtBdAddrCopy(&(evt.argument_ptr->user_passkey_request_reply_args.bd_addr), (BD_ADDR_T *) cmdArg);
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
}
void csrBcCmHciUserPasskeyReqNegReplyCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_COMMAND_COMPLETE; 
    evt.event.length              = HCI_USER_PASSKEY_REQUEST_NEG_REPLY_ARG_LEN;
    evt.num_hci_command_pkts      = numHciCommandPackets;
    evt.op_code                   = HCI_USER_PASSKEY_REQUEST_NEG_REPLY;
    evt.status                    = HCI_SUCCESS;
    evt.argument_ptr              = CsrPmemAlloc(sizeof(*evt.argument_ptr));
    CsrBtBdAddrCopy(&(evt.argument_ptr->user_passkey_request_neg_reply_args.bd_addr), (BD_ADDR_T *) cmdArg);
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
}

void csrBcCmHciOobDataReqReplyCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_COMMAND_COMPLETE; 
    evt.event.length              = HCI_REMOTE_OOB_DATA_REQUEST_REPLY_ARG_LEN;
    evt.num_hci_command_pkts      = numHciCommandPackets;
    evt.op_code                   = HCI_REMOTE_OOB_DATA_REQUEST_REPLY;
    evt.status                    = HCI_SUCCESS;
    evt.argument_ptr              = CsrPmemAlloc(sizeof(*evt.argument_ptr));
    CsrBtBdAddrCopy(&(evt.argument_ptr->remote_oob_data_request_reply_args.bd_addr), (BD_ADDR_T *) cmdArg);
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
}

void csrBcCmHciOobDataReqNegReplyCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt;
    evt.event.event_code          = HCI_EV_COMMAND_COMPLETE; 
    evt.event.length              = HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY_ARG_LEN;
    evt.num_hci_command_pkts      = numHciCommandPackets;
    evt.op_code                   = HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY;
    evt.status                    = HCI_SUCCESS;
    evt.argument_ptr              = CsrPmemAlloc(sizeof(*evt.argument_ptr));
    CsrBtBdAddrCopy(&(evt.argument_ptr->remote_oob_data_request_neg_reply_args.bd_addr), (BD_ADDR_T *) cmdArg);
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
}
#endif /* CSR_CHIP_MANAGER_ENABLE */

CsrBool ConvertHciInquiry(CONVERT_PARAMS)
{
    HCI_INQUIRY_T * msg;

    msg = (HCI_INQUIRY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->inquiry_length, &buffer[6]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->num_responses, &buffer[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeNeverHandler(NULL, 
                                                msg->common.op_code,
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                csrBcCmHciInquiryStatusEventHandler,
                                                csrBcCmHciInquiryCompleteEventHandler,
                                                NULL,
                                                NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 
}

CsrBool ConvertHciInquiryCancel(CONVERT_PARAMS)
{
    HCI_INQUIRY_CANCEL_T * msg;

    msg = (HCI_INQUIRY_CANCEL_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return(CsrBtHciCmCmdResendTypeNeverHandler( NULL,
                                                msg->common.op_code,
                                                HCI_INQUIRY,
                                                NULL,
                                                csrBcCmHciInquiryCancelCompleteEventHandler,
                                                NULL,
                                                NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciCreateConnection(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif
    HCI_CREATE_CONNECTION_T * msg;

    msg = (HCI_CREATE_CONNECTION_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->pkt_type, &buffer[9]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->page_scan_rep_mode, &buffer[11]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->page_scan_mode, &buffer[12]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->clock_offset, &buffer[13]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->allow_role_switch, &buffer[15]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr,
                                                msg->common.op_code,
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                csrBcCmHciCreateConStatusEventHandler,
                                                csrBcCmHciConnectionCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciDisconnect(CONVERT_PARAMS)
{
    HCI_DISCONNECT_T * msg;

    msg = (HCI_DISCONNECT_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->reason, &buffer[5]);
    
#ifdef CSR_CHIP_MANAGER_ENABLE
    return CsrBtHciCmSendRequest();
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */

}

CsrBool ConvertHciCreateConnectionCancel(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif
    HCI_CREATE_CONNECTION_CANCEL_T * msg;

    msg = (HCI_CREATE_CONNECTION_CANCEL_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  bdaddr,  
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciAcceptConnectionReq(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif
    HCI_ACCEPT_CONNECTION_REQ_T * msg;

    msg = (HCI_ACCEPT_CONNECTION_REQ_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->role, &buffer[9]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                HCI_CREATE_CONNECTION, 
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                csrBcCmHciAcceptConStatusEventHandler,
                                                csrBcCmHciConnectionCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciRejectConnectionReq(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif
    HCI_REJECT_CONNECTION_REQ_T * msg;

    msg = (HCI_REJECT_CONNECTION_REQ_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->reason, &buffer[9]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                HCI_CREATE_CONNECTION,
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                csrBcCmHciRejectConStatusEventHandler,
                                                csrBcCmHciConnectionCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciLinkKeyReqReply(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif
    CsrUint8                         i;
    HCI_LINK_KEY_REQ_REPLY_T * msg;

    msg = (HCI_LINK_KEY_REQ_REPLY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);

    for(i = 0; i < SIZE_LINK_KEY; i++)
    {
        CSR_BT_CONVERT_8_TO_STREAM(msg->key_val[i], &buffer[(9 + i)]);
    }
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                msg->common.op_code, 
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                NULL,
                                                csrBcCmHciLinkKeyReplyCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciLinkKeyReqNegReply(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif

    HCI_LINK_KEY_REQ_NEG_REPLY_T * msg;

    msg = (HCI_LINK_KEY_REQ_NEG_REPLY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                msg->common.op_code, 
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                NULL,
                                                csrBcCmHciLinkKeyNegReplyCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciPinCodeReqReply(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif

    CsrUint8                         i;
    HCI_PIN_CODE_REQ_REPLY_T * msg;

    msg = (HCI_PIN_CODE_REQ_REPLY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->pin_code_length, &buffer[9]);

    for(i = 0; i < CSR_ARRAY_SIZE(msg->pin); i++)
    {
        CSR_BT_CONVERT_16_TO_STREAM(msg->pin[i], &buffer[(10 + i * 2)]);
    }
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                msg->common.op_code, 
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                NULL,
                                                csrBcCmHciPinCodeReplyCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciPinCodeReqNegReply(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif

    HCI_PIN_CODE_REQ_NEG_REPLY_T * msg;

    msg = (HCI_PIN_CODE_REQ_NEG_REPLY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                msg->common.op_code, 
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                NULL,
                                                csrBcCmHciPinCodeNegReplyCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

#ifndef CSR_BT_EXCLUDE_HCI_CHANGE_ACL_CONN_PKT_TYPE
CsrBool ConvertHciChangeConnPktType(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_CHANGE_CONN_PKT_TYPE_T * msg;

    msg = (HCI_CHANGE_CONN_PKT_TYPE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->pkt_type, &buffer[5]);

#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = (hci_connection_handle_t *) CsrPmemAlloc(sizeof(hci_connection_handle_t));
    *handle = msg->handle;

    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,  
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif
}
#endif

CsrBool ConvertHciAuthReq(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_AUTH_REQ_T * msg;

    msg = (HCI_AUTH_REQ_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,  
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciSetConnEncryption(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_SET_CONN_ENCRYPTION_T * msg;

    msg = (HCI_SET_CONN_ENCRYPTION_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->enc_enable, &buffer[5]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,  
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif
}

CsrBool ConvertHciRemoteNameReq(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif
    HCI_REMOTE_NAME_REQ_T * msg;

    msg = (HCI_REMOTE_NAME_REQ_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->page_scan_rep_mode, &buffer[9]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->page_scan_mode, &buffer[10]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->clock_offset, &buffer[11]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  bdaddr,  
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciRemoteNameReqCancel(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif

    HCI_REMOTE_NAME_REQ_CANCEL_T * msg;

    msg = (HCI_REMOTE_NAME_REQ_CANCEL_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  bdaddr,  
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciReadRemoteSuppFeatures(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_READ_REMOTE_SUPP_FEATURES_T * msg;

    msg = (HCI_READ_REMOTE_SUPP_FEATURES_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,  
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciReadRemoteExtFeatures(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_READ_REMOTE_EXT_FEATURES_T * msg;

    msg = (HCI_READ_REMOTE_EXT_FEATURES_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->page_num, &buffer[5]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,  
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciReadRemoteVerInfo(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif

    HCI_READ_REMOTE_VER_INFO_T * msg;

    msg = (HCI_READ_REMOTE_VER_INFO_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,  
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciReadClockOffset(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_READ_CLOCK_OFFSET_T * msg;

    msg = (HCI_READ_CLOCK_OFFSET_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    handle = CsrBtHciConnectionHandleDup(msg->handle);
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  handle,  
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  CsrPmemFree,
                                                  CsrBtHciCmCompareConnectionHandles));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciSetupSynchronousConn(CONVERT_PARAMS)
{
    HCI_SETUP_SYNCHRONOUS_CONN_T * msg;

    msg = (HCI_SETUP_SYNCHRONOUS_CONN_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_32_TO_STREAM(msg->tx_bdw, &buffer[5]);
    CSR_BT_CONVERT_32_TO_STREAM(msg->rx_bdw, &buffer[9]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->max_latency, &buffer[13]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->voice_settings, &buffer[15]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->retx_effort, &buffer[17]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->pkt_type, &buffer[18]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,  
                                                  msg->common.op_code,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciAcceptSynchronousConnReq(CONVERT_PARAMS)
{
    HCI_ACCEPT_SYNCHRONOUS_CONN_REQ_T * msg;

    msg = (HCI_ACCEPT_SYNCHRONOUS_CONN_REQ_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    CSR_BT_CONVERT_32_TO_STREAM(msg->tx_bdw, &buffer[9]);
    CSR_BT_CONVERT_32_TO_STREAM(msg->rx_bdw, &buffer[13]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->max_latency, &buffer[17]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->voice_settings, &buffer[19]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->retx_effort, &buffer[21]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->pkt_type, &buffer[22]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,  
                                                  HCI_SETUP_SYNCHRONOUS_CONN,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */

}

CsrBool ConvertHciRejectSynchronousConn(CONVERT_PARAMS)
{
    HCI_REJECT_SYNCHRONOUS_CONN_REQ_T * msg;

    msg = (HCI_REJECT_SYNCHRONOUS_CONN_REQ_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->reason, &buffer[9]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypePendingHandler(buffer,
                                                  NULL,  
                                                  HCI_SETUP_SYNCHRONOUS_CONN,
                                                  CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                  NULL,
                                                  NULL));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciIoCapabilityResponse(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif
    HCI_IO_CAPABILITY_RESPONSE_T * msg;

    msg = (HCI_IO_CAPABILITY_RESPONSE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->io_capability, &buffer[9]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->oob_data_present, &buffer[10]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->authentication_requirements, &buffer[11]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                msg->common.op_code,
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                NULL,
                                                csrBcCmHciIoCapRespCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 
}

CsrBool ConvertHciUserConfirmationRequestReply(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif

    HCI_USER_CONFIRMATION_REQUEST_REPLY_T * msg;

    msg = (HCI_USER_CONFIRMATION_REQUEST_REPLY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                msg->common.op_code,
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                NULL,
                                                csrBcCmHciUserConfReqReplyCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 
}

CsrBool ConvertHciUserConfirmationRequestNegReply(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif
    HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY_T * msg;

    msg = (HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                msg->common.op_code,
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                NULL,
                                                csrBcCmHciUserConfReqNegReplyCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */ 
}

CsrBool ConvertHciUserPasskeyRequestReply(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif
    HCI_USER_PASSKEY_REQUEST_REPLY_T * msg;

    msg = (HCI_USER_PASSKEY_REQUEST_REPLY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    CSR_BT_CONVERT_32_TO_STREAM(msg->numeric_value, &buffer[9]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                msg->common.op_code,
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                NULL,
                                                csrBcCmHciUserPasskeyReqReplyCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciUserPasskeyRequestNegReply(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif
    HCI_USER_PASSKEY_REQUEST_NEG_REPLY_T * msg;

    msg = (HCI_USER_PASSKEY_REQUEST_NEG_REPLY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                msg->common.op_code,
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                NULL,
                                                csrBcCmHciUserPasskeyReqNegReplyCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciRemoteOobDataRequestReply(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif

    CsrUint8 index = 0;
    HCI_REMOTE_OOB_DATA_REQUEST_REPLY_T * msg;

    msg = (HCI_REMOTE_OOB_DATA_REQUEST_REPLY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    for (index = 0; index < SIZE_OOB_DATA; index++)
    {
        CSR_BT_CONVERT_8_TO_STREAM(msg->c[index], &buffer[9+index]);
    }

    for (index = 0; index < SIZE_OOB_DATA; index++)
    {
        CSR_BT_CONVERT_8_TO_STREAM(msg->r[index], &buffer[25+index]);
    }
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                msg->common.op_code,
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                NULL,
                                                csrBcCmHciOobDataReqReplyCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciRemoteOobDataRequestNegReply(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif

    HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY_T * msg;

    msg = (HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                msg->common.op_code,
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                NULL,
                                                csrBcCmHciOobDataReqNegReplyCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciIoCapabilityRequestNegReply(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif

    HCI_IO_CAPABILITY_REQUEST_NEG_REPLY_T * msg;

    msg = (HCI_IO_CAPABILITY_REQUEST_NEG_REPLY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->reason, &buffer[9]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                msg->common.op_code,
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                NULL,
                                                csrBcCmHciIoCapReqNegReplyCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

/****************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:           csr_bt_hci_convert_lmupstream.c

DESCRIPTION:    Converter for HCI LM commands and events

REVISION:       $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "csr_pmem.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_lmupstream.h"
#include "csr_bt_hci_cm.h"

CsrBool ConvertHciCreateConnectionCancelRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    BD_ADDR_T bd_addr;

    bd_addr.lap = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    bd_addr.uap = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    bd_addr.nap = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&bd_addr))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->create_connection_cancel_args.bd_addr.lap = bd_addr.lap; 
        msg->argument_ptr->create_connection_cancel_args.bd_addr.uap = bd_addr.uap;
        msg->argument_ptr->create_connection_cancel_args.bd_addr.nap = bd_addr.nap;
        return TRUE;
    }
}

CsrBool ConvertHciRemoteNameReqCancelRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    BD_ADDR_T bd_addr;

    bd_addr.lap = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    bd_addr.uap = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    bd_addr.nap = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&bd_addr))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->remote_name_req_cancel_args.bd_addr.lap = bd_addr.lap; 
        msg->argument_ptr->remote_name_req_cancel_args.bd_addr.uap = bd_addr.uap;
        msg->argument_ptr->remote_name_req_cancel_args.bd_addr.nap = bd_addr.nap;
        return TRUE;
    }
}

CsrBool ConvertHciLinkKeyReqRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
    BD_ADDR_T bd_addr;

    bd_addr.lap = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
    bd_addr.uap = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
    bd_addr.nap = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
    
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,&bd_addr))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->link_key_req_reply_args.bd_addr.lap = bd_addr.lap; 
        msg->argument_ptr->link_key_req_reply_args.bd_addr.uap = bd_addr.uap;
        msg->argument_ptr->link_key_req_reply_args.bd_addr.nap = bd_addr.nap;
        return TRUE;
    }
}


/****************************************************************************

                (c) CSR plc 2009

                All rights reserved

FILE:           csr_bt_hci_convert_hcdownstream.c

DESCRIPTION:            converter for _hci _host _controller and _baseband
                        commands and events

REVISION:       $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_hcdownstream.h"

#ifdef CSR_CHIP_MANAGER_ENABLE
#include "csr_bt_hci_cm.h"
#include "csr_bt_util.h"
#include "csr_hci_task.h"
#include "dm_hci_interface.h"

void csrBcCmHciSendKeyPressNotifyCompleteEventHandler(void *cmdArg, CsrUint8 numHciCommandPackets)
{
    HCI_EV_COMMAND_COMPLETE_T evt; /*   = (HCI_EV_COMMAND_COMPLETE_T *) CsrPmemAlloc(sizeof(HCI_EV_COMMAND_COMPLETE_T));*/
    evt.event.event_code    = HCI_EV_COMMAND_COMPLETE;         
    evt.event.length        = HCI_SEND_KEYPRESS_NOTIFICATION_ARG_LEN;        
    evt.op_code             = HCI_SEND_KEYPRESS_NOTIFICATION;        
    evt.status              = HCI_SUCCESS;        
    evt.argument_ptr        = CsrPmemAlloc(sizeof(*evt.argument_ptr));        
    CsrBtBdAddrCopy(&(evt.argument_ptr->send_keypress_notification_args.bd_addr), (BD_ADDR_T *) cmdArg);
    dm_hci_event_handler((HCI_UPRIM_T *) &evt);
}
#endif /* CSR_CHIP_MANAGER_ENABLE */

CsrBool ConvertHciReset(CONVERT_PARAMS)
{
    HCI_RESET_T * msg;
    msg = (HCI_RESET_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
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

CsrBool ConvertHciDeleteStoredLinkKey(CONVERT_PARAMS)
{
    HCI_DELETE_STORED_LINK_KEY_T * msg;

    msg = (HCI_DELETE_STORED_LINK_KEY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->flag, &buffer[9]);
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

CsrBool ConvertHciChangeLocalName(CONVERT_PARAMS)
{
    CsrUint8                     i;
    HCI_CHANGE_LOCAL_NAME_T * msg;

    msg = (HCI_CHANGE_LOCAL_NAME_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);

    i = 0;
    while(i < HCI_NAME_LENGTH && (msg->name_part[i / HCI_LOCAL_NAME_BYTES_PER_PTR] != NULL) && (msg->name_part[i / HCI_LOCAL_NAME_BYTES_PER_PTR][i % HCI_LOCAL_NAME_BYTES_PER_PTR] != '\0'))
    {
        CSR_BT_CONVERT_8_TO_STREAM(msg->name_part[i / HCI_LOCAL_NAME_BYTES_PER_PTR][i % HCI_LOCAL_NAME_BYTES_PER_PTR], &buffer[3 + i]);
        ++i;
    }

    for (i = 0; i < HCI_LOCAL_NAME_BYTE_PACKET_PTRS; i++)
    {
        CsrPmemFree(msg->name_part[i]);
        msg->name_part[i] = NULL;
    }
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

#ifndef CSR_BT_EXCLUDE_HCI_READ_LOCAL_NAME
CsrBool ConvertHciReadLocalName(CONVERT_PARAMS)
{
    HCI_READ_LOCAL_NAME_T * msg;
    msg = (HCI_READ_LOCAL_NAME_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
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
#endif

CsrBool ConvertHciWriteScanEnable(CONVERT_PARAMS)
{
    HCI_WRITE_SCAN_ENABLE_T * msg;

    msg = (HCI_WRITE_SCAN_ENABLE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->scan_enable, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

#ifndef CSR_BT_EXCLUDE_HCI_READ_SCAN_ENABLE
CsrBool ConvertHciReadScanEnable(CONVERT_PARAMS)
{
    HCI_READ_SCAN_ENABLE_T * msg;

    msg = (HCI_READ_SCAN_ENABLE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
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
#endif

CsrBool ConvertHciWritePagescanActivity(CONVERT_PARAMS)
{
    HCI_WRITE_PAGESCAN_ACTIVITY_T * msg;

    msg = (HCI_WRITE_PAGESCAN_ACTIVITY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->pagescan_interval, &buffer[3]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->pagescan_window, &buffer[5]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciWriteInquiryscanActivity(CONVERT_PARAMS)
{
    HCI_WRITE_INQUIRYSCAN_ACTIVITY_T * msg;

    msg = (HCI_WRITE_INQUIRYSCAN_ACTIVITY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->inqscan_interval, &buffer[3]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->inqscan_window, &buffer[5]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciWriteAuthEnable(CONVERT_PARAMS)
{
    HCI_WRITE_AUTH_ENABLE_T * msg;

    msg = (HCI_WRITE_AUTH_ENABLE_T *) prim;
    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->auth_enable, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciWriteEncMode(CONVERT_PARAMS)
{
    HCI_WRITE_ENC_MODE_T * msg;

    msg = (HCI_WRITE_ENC_MODE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->mode, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

#ifndef CSR_BT_EXCLUDE_HCI_READ_CLASS_OF_DEVICE
CsrBool ConvertHciReadClassOfDevice(CONVERT_PARAMS)
{
    HCI_READ_CLASS_OF_DEVICE_T * msg;

    msg = (HCI_READ_CLASS_OF_DEVICE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
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
#endif

CsrBool ConvertHciWriteClassOfDevice(CONVERT_PARAMS)
{
    HCI_WRITE_CLASS_OF_DEVICE_T * msg;

    msg = (HCI_WRITE_CLASS_OF_DEVICE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->dev_class, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

#ifndef CSR_BT_EXCLUDE_HCI_WRITE_VOICE_SETTING
CsrBool ConvertHciWriteVoiceSetting(CONVERT_PARAMS)
{
    HCI_WRITE_VOICE_SETTING_T * msg;

    msg = (HCI_WRITE_VOICE_SETTING_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->voice_setting, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}
#endif

CsrBool ConvertHciHostBufferSize(CONVERT_PARAMS)
{
    HCI_HOST_BUFFER_SIZE_T * msg;

    msg = (HCI_HOST_BUFFER_SIZE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->acl_packet_len, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->sco_packet_len, &buffer[5]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->acl_total_packets, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->sco_total_packets, &buffer[8]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciWritePageTimeout(CONVERT_PARAMS)
{
    HCI_WRITE_PAGE_TIMEOUT_T * msg;

    msg = (HCI_WRITE_PAGE_TIMEOUT_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->page_timeout, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciWriteLinkSupervTimeout(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_WRITE_LINK_SUPERV_TIMEOUT_T * msg;

    msg = (HCI_WRITE_LINK_SUPERV_TIMEOUT_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->timeout, &buffer[5]);
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

#ifndef CSR_BT_EXCLUDE_HCI_READ_TX_POWER_LEVEL
CsrBool ConvertHciReadTxPowerLevel(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif

    HCI_READ_TX_POWER_LEVEL_T * msg;

    msg = (HCI_READ_TX_POWER_LEVEL_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->type, &buffer[5]);
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
#endif
/**********************************************************************
NOTE:   the rfc firmware requires that the not used bytes of the
        should be set to all 0's. Use the macros to fill in the zero's.
***********************************************************************/
CsrBool ConvertHciSetEventFilter(CONVERT_PARAMS)
{
    HCI_SET_EVENT_FILTER_T  *msg;

    msg = (HCI_SET_EVENT_FILTER_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->filter_type, &buffer[3]);

    switch (msg->filter_type)
    {
        case CLEAR_ALL_FILTERS:
        {
            /*  filter type is the only parameter */
            break;
        }
        case INQUIRY_RESULT_FILTER:
        {
            CSR_BT_CONVERT_8_TO_STREAM(msg->filter_condition_type, &buffer[4]);
            switch ( msg->filter_condition_type )
            {
                case NEW_DEVICE_RESPONDED:
                {
                    /*  condition parameter is not used */
                    break;
                }
                case  CLASS_OF_DEVICE_RESPONDED:
                {
                    CSR_BT_CONVERT_24_TO_STREAM(msg->condition.class_mask.class_of_device, &buffer[5]);
                    CSR_BT_CONVERT_24_TO_STREAM(msg->condition.class_mask.mask, &buffer[8]);
                    break;
                }
                case  ADDRESSED_DEVICE_RESPONDED:
                {
                    CSR_BT_CONVERT_24_TO_STREAM(msg->condition.bd_addr.lap, &buffer[5]);
                    CSR_BT_CONVERT_8_TO_STREAM(msg->condition.bd_addr.uap, &buffer[8]);
                    CSR_BT_CONVERT_16_TO_STREAM(msg->condition.bd_addr.nap, &buffer[9]);
                    break;
                }
                default:
                {
                    /* reserved for future use */
                }
            }
            break;
        }
        case CONNECTION_FILTER:
        {
            CSR_BT_CONVERT_8_TO_STREAM(msg->filter_condition_type, &buffer[4]);
            switch ( msg->filter_condition_type )
            {
                case  ALL_CONNECTION:
                {
                    CSR_BT_CONVERT_8_TO_STREAM(msg->condition.auto_accept, &buffer[5]);
                    break;
                }
                case  CLASS_OF_DEVICE_RESPONDED:
                {
                    CSR_BT_CONVERT_24_TO_STREAM(msg->condition.cma.class_mask.class_of_device, &buffer[5]);
                    CSR_BT_CONVERT_24_TO_STREAM(msg->condition.cma.class_mask.mask, &buffer[8]);
                    CSR_BT_CONVERT_8_TO_STREAM(msg->condition.auto_accept, &buffer[11]);
                    break;
                }
                case  ADDRESSED_DEVICE_RESPONDED:
                {
                    CSR_BT_CONVERT_24_TO_STREAM(msg->condition.addr_auto.bd_addr.lap, &buffer[5]);
                    CSR_BT_CONVERT_8_TO_STREAM(msg->condition.addr_auto.bd_addr.uap, &buffer[8]);
                    CSR_BT_CONVERT_16_TO_STREAM(msg->condition.addr_auto.bd_addr.nap, &buffer[9]);
                    CSR_BT_CONVERT_8_TO_STREAM(msg->condition.addr_auto.auto_accept, &buffer[11]);
                    break;
                }
                default:
                {
                    /* reserved for future use */
                }
            }
            break;
        }
        default:
        {
            /*  reserved for future use */
        }
    }
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

#ifndef CSR_BT_EXCLUDE_HCI_SET_AFH_CHANNEL_CLASS
CsrBool ConvertHciSetAfhChannelClass(CONVERT_PARAMS)
{
    HCI_SET_AFH_CHANNEL_CLASS_T * msg;
    msg = (HCI_SET_AFH_CHANNEL_CLASS_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CsrMemCpy(&buffer[3], msg->map, CSR_ARRAY_SIZE(msg->map));

#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}
#endif

CsrBool ConvertHciWriteInquiryScanType(CONVERT_PARAMS)
{
    HCI_WRITE_INQUIRY_SCAN_TYPE_T * msg;

    msg = (HCI_WRITE_INQUIRY_SCAN_TYPE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->mode, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciWriteInquiryMode(CONVERT_PARAMS)
{
    HCI_WRITE_INQUIRY_MODE_T * msg;

    msg = (HCI_WRITE_INQUIRY_MODE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->mode, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

CsrBool ConvertHciWritePageScanType(CONVERT_PARAMS)
{
    HCI_WRITE_PAGE_SCAN_TYPE_T * msg;

    msg = (HCI_WRITE_PAGE_SCAN_TYPE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->mode, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

#ifndef CSR_BT_EXCLUDE_HCI_READ_AFH_CHANNEL_CLASS_M
CsrBool ConvertHciReadAfhChannelClassM(CONVERT_PARAMS)
{
    HCI_READ_AFH_CHANNEL_CLASS_M_T * msg;

    msg = (HCI_READ_AFH_CHANNEL_CLASS_M_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
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
#endif

#ifndef CSR_BT_EXCLUDE_HCI_WRITE_AFH_CHANNEL_CLASS_M
CsrBool ConvertHciWriteAfhChannelClassM(CONVERT_PARAMS)
{
    HCI_WRITE_AFH_CHANNEL_CLASS_M_T * msg;

    msg = (HCI_WRITE_AFH_CHANNEL_CLASS_M_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->class_mode, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_NUM_SUPPORTED_IAC
CsrBool ConvertHciReadNumSupportedIac(CONVERT_PARAMS)
{
    HCI_READ_NUM_SUPPORTED_IAC_T *msg;

    msg = (HCI_READ_NUM_SUPPORTED_IAC_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
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
#endif

#ifndef CSR_BT_EXCLUDE_HCI_READ_CURRENT_IAC_LAP
CsrBool ConvertHciReadCurrentIacLap(CONVERT_PARAMS)
{
    HCI_READ_CURRENT_IAC_LAP_T *msg;

    msg = (HCI_READ_CURRENT_IAC_LAP_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
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
#endif

#ifndef CSR_BT_EXCLUDE_HCI_WRITE_CURRENT_IAC_LAP
CsrBool ConvertHciWriteCurrentIacLap(CONVERT_PARAMS)
{
    CsrUint8                     i;
    CsrUint8                     numIac;
    HCI_WRITE_CURRENT_IAC_LAP_T *msg;

    msg = (HCI_WRITE_CURRENT_IAC_LAP_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->num_current_iac, &buffer[3]);

    numIac = msg->num_current_iac;

    for (i = 0; i < numIac; i++)
    {
        CSR_BT_CONVERT_24_TO_STREAM(msg->iac_lap[i / HCI_IAC_LAP_PER_PTR][i % HCI_IAC_LAP_PER_PTR], &buffer[(4 + i * 3)]);
    }

    for (i = 0; i < HCI_IAC_LAP_PTRS; i++)
    {
        CsrPmemFree(msg->iac_lap[i]);
        msg->iac_lap[i] = NULL;
    }
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}
#endif

/* [*] 2.1 primitives */
CsrBool ConvertHciRefreshEncryptionKey(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    hci_connection_handle_t *handle;
#endif
    HCI_REFRESH_ENCRYPTION_KEY_T *msg;

    msg = (HCI_REFRESH_ENCRYPTION_KEY_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->handle, &buffer[3]);
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


/* [*] 2.1 primitives */
CsrBool ConvertHciWriteSimplePairingMode(CONVERT_PARAMS)
{
    HCI_WRITE_SIMPLE_PAIRING_MODE_T *msg;

    msg = (HCI_WRITE_SIMPLE_PAIRING_MODE_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->simple_pairing_mode, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

#ifndef CSR_BT_EXCLUDE_SM_READ_LOCAL_OOB_DATA
CsrBool ConvertHciReadLocalOobData(CONVERT_PARAMS)
{
    HCI_READ_LOCAL_OOB_DATA_T *msg;

    msg = (HCI_READ_LOCAL_OOB_DATA_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
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
#endif

CsrBool ConvertHciWriteInquiryTransmitPowerLevel(CONVERT_PARAMS)
{
    HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_T *msg;

    msg = (HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->tx_power, &buffer[3]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

#ifndef CSR_BT_EXCLUDE_SM_SEND_KEYPRESS_NOTIFICATION
CsrBool ConvertHciSendKeypressNotification(CONVERT_PARAMS)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    BD_ADDR_T *bdaddr;
#endif
    HCI_SEND_KEYPRESS_NOTIFICATION_T *msg;

    msg = (HCI_SEND_KEYPRESS_NOTIFICATION_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_24_TO_STREAM(msg->bd_addr.lap, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->bd_addr.uap, &buffer[6]);
    CSR_BT_CONVERT_16_TO_STREAM(msg->bd_addr.nap, &buffer[7]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->notification_type, &buffer[9]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    bdaddr = CsrBtBdAddrDup(&msg->bd_addr);
    return (CsrBtHciCmCmdResendTypeNeverHandler(bdaddr, 
                                                msg->common.op_code, 
                                                CSR_BT_HCI_CM_INVALID_HCI_CMD_IF,
                                                NULL,
                                                csrBcCmHciSendKeyPressNotifyCompleteEventHandler,
                                                CsrPmemFree,
                                                CsrBtHciCmCompareBdaddr));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}
#endif
    
#ifndef CSR_BT_EXCLUDE_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA
CsrBool ConvertHciWriteExtendedInquiryResponseData(CONVERT_PARAMS)
{
    HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_T *msg;
    CsrUint8 i;

    msg = (HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_T *) prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->fec_required, &buffer[3]);

    for (i = 0; msg->eir_data_part[i / HCI_EIR_DATA_BYTES_PER_PTR] != NULL && i < HCI_EIR_DATA_LENGTH; i++)
    {
        CSR_BT_CONVERT_8_TO_STREAM(msg->eir_data_part[i / HCI_EIR_DATA_BYTES_PER_PTR][i % HCI_EIR_DATA_BYTES_PER_PTR], &buffer[4 + i]);
    }

    for (; i < HCI_EIR_DATA_LENGTH; i++)
    {
        CSR_BT_CONVERT_8_TO_STREAM(0, &buffer[4 + i]);
    }

    /* Insert length */
    CSR_BT_CONVERT_8_TO_STREAM(i + 1, &buffer[2]);

    for (i = 0; i < HCI_EIR_DATA_PACKET_PTRS; i++)
    {
        CsrPmemFree(msg->eir_data_part[i]);
        msg->eir_data_part[i] = NULL;
    }
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}
#endif

CsrBool ConvertHciSetEventMask(CONVERT_PARAMS)
{
    HCI_SET_EVENT_MASK_T *msg;

    msg = (HCI_SET_EVENT_MASK_T *) prim;
    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_32_TO_STREAM(msg->event_mask[0], &buffer[3]);
    CSR_BT_CONVERT_32_TO_STREAM(msg->event_mask[1], &buffer[7]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}

/* 4.0 BLE controller features */
#ifdef CSR_BT_LE_ENABLE
CsrBool ConvertHciWriteLeHostSupport(CONVERT_PARAMS)
{
    HCI_WRITE_LE_HOST_SUPPORT_T *msg;

    msg = (HCI_WRITE_LE_HOST_SUPPORT_T*)prim;

    CSR_BT_CONVERT_16_TO_STREAM(msg->common.op_code, buffer);
    CSR_BT_CONVERT_8_TO_STREAM(msg->common.length, &buffer[2]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->le_supported_host, &buffer[3]);
    CSR_BT_CONVERT_8_TO_STREAM(msg->simultaneous_le_host, &buffer[4]);
#ifdef CSR_CHIP_MANAGER_ENABLE
    return (CsrBtHciCmCmdResendTypeAlwaysHandler(buffer, msg->common.op_code));
#else
    return TRUE;
#endif /* CSR_CHIP_MANAGER_ENABLE */
}
#endif


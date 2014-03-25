/****************************************************************************

                  (c) CSR plc 2008

                All rights reserved

FILE:            csr_bt_hci_convert_info_upstream.c

DESCRIPTION:    Converter for HCI Converter Informational Parameters

REVISION:        $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "csr_pmem.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_info_upstream.h"
#include "csr_bt_hci_cm.h"

CsrBool ConvertHciReadLocalSuppFeaturesRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        CsrMemCpy(msg->argument_ptr->read_local_supp_features_args.lmp_supp_features, &hciData[0], CSR_ARRAY_SIZE(msg->argument_ptr->read_local_supp_features_args.lmp_supp_features));

        return TRUE;
    }
}

CsrBool ConvertHciReadBdAddrRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_bd_addr_args.bd_addr.lap = CSR_BT_CONVERT_24_FROM_STREAM(hciData);
        msg->argument_ptr->read_bd_addr_args.bd_addr.uap = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
        msg->argument_ptr->read_bd_addr_args.bd_addr.nap = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
        return TRUE;
    }
}

CsrBool ConvertHciReadBufferSizeRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_buffer_size_args.acl_data_pkt_length = CSR_BT_CONVERT_16_FROM_STREAM(hciData);
        msg->argument_ptr->read_buffer_size_args.sco_data_pkt_length = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[2]);
        msg->argument_ptr->read_buffer_size_args.total_acl_data_pkts = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[3]);
        msg->argument_ptr->read_buffer_size_args.total_sco_data_pkts = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[5]);
        return TRUE;
    }
}

CsrBool ConvertHciReadLocalVersionInfoRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_local_ver_info_args.hci_version = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
        msg->argument_ptr->read_local_ver_info_args.hci_revision = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[1]);
        msg->argument_ptr->read_local_ver_info_args.lmp_version = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[3]);
        msg->argument_ptr->read_local_ver_info_args.manuf_name = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[4]);
        msg->argument_ptr->read_local_ver_info_args.lmp_subversion = CSR_BT_CONVERT_16_FROM_STREAM(&hciData[6]);
        return TRUE;
    }
}

#ifndef CSR_BT_EXCLUDE_HCI_READ_LOCAL_EXT_FEATURES
CsrBool ConvertHciReadLocalExtFeaturesRet(CsrUint8 *hciData, HCI_EV_COMMAND_COMPLETE_T *msg)
{
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBtHciCmEventHandler(msg->op_code,msg->status,TRUE,&msg->num_hci_command_pkts,NULL))
    {
        return FALSE;
    }
#endif
    {
        msg->argument_ptr->read_local_ext_features_args.page_num = CSR_BT_CONVERT_8_FROM_STREAM(hciData);
        msg->argument_ptr->read_local_ext_features_args.max_page_num = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[1]);
        CsrMemCpy(msg->argument_ptr->read_local_ext_features_args.lmp_ext_features, &hciData[2], CSR_ARRAY_SIZE(msg->argument_ptr->read_local_ext_features_args.lmp_ext_features));

        return TRUE;
    }
}
#endif


/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_deinit.c

\brief  Shutdown DM.
*/

#include "csr_synergy.h"
#ifdef ENABLE_SHUTDOWN

#include "csr_bt_bluestack_types.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_sched.h"

#include "csr_env_prim.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_fsm.h"

#include "hci_prim.h"
#include "dm_prim.h"
#include "dmlib.h"
#include "bluestacklib.h"

#include "dm_sm_types.h"
#include "dm_acl_core.h"
#include "dm_acl_policy_manager.h"
#include "dm_security_manager.h"
#include "dm_mgt_interface.h"
#include "dm_hci_interface.h"
#include "dm_sync_manager.h"
#ifdef INSTALL_AMP_SUPPORT
#include "dm_amp_interface.h"
#endif

#ifdef BUILD_FOR_HOST
#include "csr_bt_hcishim.h"
#endif

static void dm_hci_free_upstream_primitive(HCI_UPRIM_T *prim)
{
    switch (prim->event_code)
    {
        case HCI_EV_INQUIRY_RESULT:
            {
                pdestroy_array((void **) prim->hci_inquiry_result_event.result,
                               HCI_MAX_INQ_RESULT_PTRS);
                break;
            }

        case HCI_EV_INQUIRY_RESULT_WITH_RSSI:
            {
                pdestroy_array((void **) prim->hci_inquiry_result_with_rssi_event.result,
                               HCI_MAX_INQ_RESULT_PTRS);
                break;
            }

        case HCI_EV_EXTENDED_INQUIRY_RESULT:
            {
                pdestroy_array((void **) prim->hci_extended_inquiry_result_event.eir_data_part,
                               HCI_EIR_DATA_PACKET_PTRS);
                break;
            }

        case HCI_EV_REMOTE_NAME_REQ_COMPLETE:
            {
                pdestroy_array((void **) prim->hci_remote_name_request_complete.name_part,
                               HCI_LOCAL_NAME_BYTE_PACKET_PTRS);
                break;
            }

        case HCI_EV_LOOPBACK_COMMAND:
            {
                pdestroy_array((void **) prim->hci_loopback_command_event.loopback_part_ptr,
                               HCI_LOOPBACK_BYTE_PACKET_PTRS);
                break;
            }

        case HCI_EV_RETURN_LINK_KEYS:
            {
                pdestroy_array((void **) prim->hci_ret_linkkeys_event.link_key_bd_addr,
                               HCI_STORED_LINK_KEY_MAX);
                break;
            }

        case HCI_EV_NUMBER_COMPLETED_PKTS:
            {
                pdestroy_array((void **) prim->hci_num_compl_pkts_event.num_completed_pkts_ptr,
                               HCI_EV_NUM_HANDLE_COMPLETE_PACKET_PTRS);
                break;
            }

        case HCI_EV_COMMAND_COMPLETE:
            {
                switch (prim->hci_command_complete_event.op_code)
                {
                    case HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA:
                        {
                            pdestroy_array((void **) prim->hci_command_complete_event.argument_ptr->read_extended_inquiry_response_data_args.eir_data_part,
                                           HCI_EIR_DATA_PACKET_PTRS);
                            break;
                        }

                    case HCI_READ_LOCAL_NAME:
                        {
                            pdestroy_array((void **) prim->hci_command_complete_event.argument_ptr->read_local_name_args.name_part,
                                           HCI_LOCAL_NAME_BYTE_PACKET_PTRS);
                            break;
                        }

                    case HCI_READ_LOCAL_SUPP_COMMANDS:
                        {
                            pdestroy_array((void **) prim->hci_command_complete_event.argument_ptr->read_local_supp_commands_args.supp_commands,
                                           HCI_READ_SUPP_COMMANDS_PACKET_PTRS);
                            break;
                        }

                    case HCI_READ_CURRENT_IAC_LAP:
                        {
                            pdestroy_array((void **) prim->hci_command_complete_event.argument_ptr->read_current_iac_lap_args.iac_lap,
                                           HCI_IAC_LAP_PTRS);
                            break;
                        }

                    default:
                        {
                            break;
                        }
                }

                CsrPmemFree(prim->hci_command_complete_event.argument_ptr);
                break;
            }

        default:
            {
                break;
            }
    }

    CsrPmemFree(prim);
}

static void dm_mgt_queue_deinit(void)
{
    CsrUint16 msg_type;
    void *msg_data;

    /* Empty message queue. */
    while (CsrSchedMessageGet(&msg_type, &msg_data))
    {
        if (msg_type == CSR_SCHED_PRIM)
            CsrPmemFree(msg_data);
        else
            dm_free_downstream_primitive((DM_UPRIM_T*)msg_data);
    }
}

void CsrBtDmHciDeinit(void **gash)
{
    CsrUint16 msg_type;
    void *msg_data;

    while (CsrSchedMessageGet(&msg_type, &msg_data))
    {
        if (msg_type == DM_PRIM)
        {
            CsrMblkDestroy(((DM_DATA_FROM_HCI_REQ_T*)msg_data)->data);
            CsrPmemFree(msg_data);
        }
        else if (msg_type == HCI_PRIM)
        {
            dm_hci_free_upstream_primitive((HCI_UPRIM_T *) msg_data);
        }
        else
        {
            CsrPmemFree(msg_data);
        }
    }
}

void CsrBtDmDeinit(void **gash)
{
    dm_mgt_queue_deinit();
    dm_acl_deinit();
    dm_sm_deinit();
    dm_acl_policy_manager_deinit();
    dm_hci_interface_deinit();
#ifdef INSTALL_DM_SYNC_MODULE
    dm_sync_deinit();
#endif
#ifdef INSTALL_AMP_SUPPORT
    dm_amp_deinit();
#endif
#ifdef BUILD_FOR_HOST
    CsrBtHcishimDeinit();
#endif
}

#endif


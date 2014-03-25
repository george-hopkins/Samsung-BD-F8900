/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>

#include "csr_types.h"
#include "csr_result.h"

#include "csr_sched_init.h"

#include "csr_tm_bluecore_task.h"
#include "csr_bccmd_task.h"
#include "csr_fp_task.h"
#include "csr_hq_task.h"
#include "csr_vm_task.h"
#include "csr_hci_task.h"
#include "csr_ui_task.h"
#include "csr_app_task.h"
#include "csr_ip_task.h"
#include "csr_am_task.h"
#include "csr_bluecore_manager_task.h"
#include "csr_ethernet_manager_task.h"
#include "csr_sdio_probe_task.h"
#include "csr_tcp_transceiver_task.h"
#include "csr_dhcp_server_manager_task.h"
#include "csr_dhcp_server_task.h"
#include "csr_tftp_task.h"
#include "csr_tftp_file_task.h"
#include "csr_fsal_task.h"

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
#include "csr_ips_manager_task.h"
#endif

#ifdef DEMO_USE_ETHERNET_TASK
#include "platform/csr_ethernet_task.h"
#endif

#include "csr_app_main.h"
#include "csr_transport.h"

#include "csr_bootstrap_generic.h"

static schedEntryFunction_t tmEntryFunction = NULL;

void CsrAppMainUsage(void)
{
}

CsrResult CsrAppMain(void)
{
    CsrUint8 address[6];

    /* Transfer settings to bootstrap */
    CsrBootstrapSetInterface(NULL, CsrAppMainBluecoreTransportGet());
    CsrBootstrapSetBitRate(NULL, CsrAppMainBluecorePostBootstrapBaudrateGet());
    CsrBootstrapSetFrequency(NULL, (CsrUint16) CsrAppMainBluecoreXtalFrequencyGet());
    CsrAppMainBluecoreLocalDeviceBluetoothAddressGet(address);
    CsrBootstrapSetLocalBtAddress(NULL, address);
    CsrBootstrapSetFriendlyName(NULL, (const CsrUtf8String *) CsrAppMainBluecoreLocalDeviceNameGet());
    CsrBootstrapParsePsrFile(NULL, CsrAppMainBluecorePsrFileGet());
    CsrBootstrapParsePsrConstant(NULL, CsrAppMainBluecorePsrStringGet());

    switch (CsrAppMainBluecoreTransportGet())
    {
        case TRANSPORT_TYPE_BCSP:
            tmEntryFunction = CSR_TM_BLUECORE_BCSP_INIT;
            break;
        case TRANSPORT_TYPE_H4_DS:
            tmEntryFunction = CSR_TM_BLUECORE_H4DS_INIT;
            break;
#ifdef DEMO_USE_USB
        case TRANSPORT_TYPE_USB:
#ifdef CSR_HCI_SOCKET_TRANSPORT
            tmEntryFunction = CSR_TM_BLUECORE_HCI_SOCKET_INIT;
#else
            tmEntryFunction = CSR_TM_BLUECORE_USB_INIT;
#endif
            break;
#endif
#ifdef DEMO_USE_SDIO
        case TRANSPORT_TYPE_TYPE_A:
            tmEntryFunction = CSR_TM_BLUECORE_TYPE_A_INIT;
            break;
#endif
        default:
            printf("Unsupported Bluecore transport selected\n");
            return CSR_RESULT_FAILURE;
    }

    return CSR_RESULT_SUCCESS;
}

CsrSchedQid CSR_HCI_IFACEQUEUE;
CsrSchedQid CSR_BCCMD_IFACEQUEUE;
CsrSchedQid CSR_FP_IFACEQUEUE;
CsrSchedQid CSR_VM_IFACEQUEUE;
CsrSchedQid CSR_HQ_IFACEQUEUE;
CsrSchedQid CSR_TM_BLUECORE_IFACEQUEUE;
CsrSchedQid CSR_APP_IFACEQUEUE;
CsrSchedQid CSR_UI_IFACEQUEUE;
CsrSchedQid CSR_IP_IFACEQUEUE;
CsrSchedQid CSR_AM_IFACEQUEUE;
CsrSchedQid CSR_ETHERNET_MANAGER_IFACEQUEUE;
CsrSchedQid CSR_BLUECORE_MANAGER_IFACEQUEUE;
CsrSchedQid CSR_TCP_TRANSCEIVER_IFACEQUEUE;
CsrSchedQid CSR_DHCP_SERVER_MANAGER_IFACEQUEUE;
CsrSchedQid CSR_DHCP_SERVER_IFACEQUEUE;
CsrSchedQid CSR_TFTP_IFACEQUEUE;
CsrSchedQid CSR_TFTP_FILE_IFACEQUEUE;
CsrSchedQid CSR_FSAL_IFACEQUEUE;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
CsrSchedQid CSR_IPS_MANAGER_IFACEQUEUE;
#endif

#ifdef DEMO_USE_SDIO
CsrSchedQid CSR_SDIO_PROBE_IFACEQUEUE;
#endif

#ifdef DEMO_USE_ETHERNET_TASK
CsrSchedQid CSR_ETHERNET_IFACEQUEUE;
#endif

/* CSR scheduler task initialisation */
void CsrSchedTaskInit(void *data)
{
#ifdef DEMO_USE_SDIO
    CsrSchedRegisterTask(&CSR_SDIO_PROBE_IFACEQUEUE,
        CSR_SDIO_PROBE_INIT,
        CSR_SDIO_PROBE_DEINIT,
        CSR_SDIO_PROBE_HANDLER,
        "SDIO_PROBE", data, 0);
#endif

#ifdef DEMO_USE_ETHERNET_TASK
    CsrSchedRegisterTask(&CSR_ETHERNET_IFACEQUEUE,
        CSR_ETHERNET_INIT,
        CSR_ETHERNET_DEINIT,
        CSR_ETHERNET_HANDLER,
        "ETHERNET", data, 0);
#endif

    CsrSchedRegisterTask(&CSR_DHCP_SERVER_IFACEQUEUE,
        CSR_DHCP_SERVER_INIT,
        CSR_DHCP_SERVER_DEINIT,
        CSR_DHCP_SERVER_HANDLER,
        "DHCP_SERVER", data, 0);

    CsrSchedRegisterTask(&CSR_TFTP_IFACEQUEUE,
        CSR_TFTP_INIT,
        CSR_TFTP_DEINIT,
        CSR_TFTP_HANDLER,
        "TFTP", data, 0);

    CsrSchedRegisterTask(&CSR_TFTP_FILE_IFACEQUEUE,
        CSR_TFTP_FILE_INIT,
        CSR_TFTP_FILE_DEINIT,
        CSR_TFTP_FILE_HANDLER,
        "TFTP_FILE", data, 0);

    CsrSchedRegisterTask(&CSR_BLUECORE_MANAGER_IFACEQUEUE,
        CSR_BLUECORE_MANAGER_INIT,
        CSR_BLUECORE_MANAGER_DEINIT,
        CSR_BLUECORE_MANAGER_HANDLER,
        "BLUECORE_MANAGER", data, 0);

    CsrSchedRegisterTask(&CSR_ETHERNET_MANAGER_IFACEQUEUE,
        CSR_ETHERNET_MANAGER_INIT,
        CSR_ETHERNET_MANAGER_DEINIT,
        CSR_ETHERNET_MANAGER_HANDLER,
        "ETHERNET_MANAGER", data, 0);

    CsrSchedRegisterTask(&CSR_TCP_TRANSCEIVER_IFACEQUEUE,
        CSR_TCP_TRANSCEIVER_INIT,
        CSR_TCP_TRANSCEIVER_DEINIT,
        CSR_TCP_TRANSCEIVER_HANDLER,
        "TCP_TRANSCEIVER", data, 0);

    CsrSchedRegisterTask(&CSR_HCI_IFACEQUEUE,
        CSR_HCI_INIT,
        CSR_HCI_DEINIT,
        CSR_HCI_HANDLER,
        "HCI", data, 0);

    CsrSchedRegisterTask(&CSR_BCCMD_IFACEQUEUE,
        CSR_BCCMD_INIT,
        CSR_BCCMD_DEINIT,
        CSR_BCCMD_HANDLER,
        "BCCMD", data, 0);

    CsrSchedRegisterTask(&CSR_FP_IFACEQUEUE,
        CSR_FP_INIT,
        CSR_FP_DEINIT,
        CSR_FP_HANDLER,
        "FP", data, 0);

    CsrSchedRegisterTask(&CSR_VM_IFACEQUEUE,
        CSR_VM_INIT,
        CSR_VM_DEINIT,
        CSR_VM_HANDLER,
        "VM", data, 0);

    CsrSchedRegisterTask(&CSR_HQ_IFACEQUEUE,
        CSR_HQ_INIT,
        CSR_HQ_DEINIT,
        CSR_HQ_HANDLER,
        "HQ", data, 0);

    CsrSchedRegisterTask(&CSR_TM_BLUECORE_IFACEQUEUE,
        tmEntryFunction,
        CSR_TM_BLUECORE_DEINIT,
        CSR_TM_BLUECORE_HANDLER,
        "TM_BLUECORE", data, 0);

    CsrSchedRegisterTask(&CSR_APP_IFACEQUEUE,
        CSR_APP_INIT,
        CSR_APP_DEINIT,
        CSR_APP_HANDLER,
        "APP", data, 0);

    CsrSchedRegisterTask(&CSR_IP_IFACEQUEUE,
        CSR_IP_INIT,
        CSR_IP_DEINIT,
        CSR_IP_HANDLER,
        "IP", data, 0);

    CsrSchedRegisterTask(&CSR_UI_IFACEQUEUE,
        CSR_UI_INIT,
        CSR_UI_DEINIT,
        CSR_UI_HANDLER,
        "UI", data, 1);

    CsrSchedRegisterTask(&CSR_AM_IFACEQUEUE,
        CSR_AM_INIT,
        CSR_AM_DEINIT,
        CSR_AM_HANDLER,
        "AM", data, 0);

    CsrSchedRegisterTask(&CSR_DHCP_SERVER_MANAGER_IFACEQUEUE,
        CSR_DHCP_SERVER_MANAGER_INIT,
        CSR_DHCP_SERVER_MANAGER_DEINIT,
        CSR_DHCP_SERVER_MANAGER_HANDLER,
        "DHCP_SERVER_MANAGER", data, 0);

    CsrSchedRegisterTask(&CSR_FSAL_IFACEQUEUE,
        CSR_FSAL_INIT,
        CSR_FSAL_DEINIT,
        CSR_FSAL_HANDLER,
        "FSAL", data, 2);

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrSchedRegisterTask(&CSR_IPS_MANAGER_IFACEQUEUE,
        CSR_IPS_MANAGER_INIT,
        CSR_IPS_MANAGER_DEINIT,
        CSR_IPS_MANAGER_HANDLER,
        "IPS_MANAGER", data, 0);
#endif
}

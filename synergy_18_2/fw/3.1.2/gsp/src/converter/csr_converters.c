/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_msgconv.h"
#ifndef EXCLUDE_CSR_DATA_STORE_MODULE
#include "csr_data_store_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_ENV_MODULE
#include "csr_env_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_FSAL_MODULE
#include "csr_fsal_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_IP_ETHER_MODULE
#include "csr_ip_ether_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_IP_IFCONFIG_MODULE
#include "csr_ip_ifconfig_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_IP_SOCKET_MODULE
#include "csr_ip_socket_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_TLS_MODULE
#include "csr_tls_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_AM_MODULE
#include "csr_am_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_APP_MODULE
#include "csr_app_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BCCMD_MODULE
#include "csr_bccmd_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_DHCP_SERVER_MODULE
#include "csr_dhcp_server_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_FP_MODULE
#include "csr_fp_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_HCI_MODULE
#include "csr_hci_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_HQ_MODULE
#include "csr_hq_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_TFTP_MODULE
#include "csr_tftp_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_TM_BLUECORE_MODULE
#include "csr_tm_bluecore_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_UI_MODULE
#include "csr_ui_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_VM_MODULE
#include "csr_vm_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_DSPM_MODULE
#include "csr_dspm_converter_init.h"
#endif

void CsrConverterInit(void)
{
    CsrMsgConvInit();

#ifndef EXCLUDE_CSR_DATA_STORE_MODULE
    CsrDataStoreConverterInit();
#endif
#ifndef EXCLUDE_CSR_ENV_MODULE
    CsrEnvConverterInit();
#endif
#ifndef EXCLUDE_CSR_FSAL_MODULE
    CsrFsalConverterInit();
#endif
#ifndef EXCLUDE_CSR_IP_ETHER_MODULE
    CsrIpEtherConverterInit();
#endif
#ifndef EXCLUDE_CSR_IP_IFCONFIG_MODULE
    CsrIpIfconfigConverterInit();
#endif
#ifndef EXCLUDE_CSR_IP_SOCKET_MODULE
    CsrIpSocketConverterInit();
#endif
#ifndef EXCLUDE_CSR_TLS_MODULE
    CsrTlsConverterInit();
#endif
#ifndef EXCLUDE_CSR_AM_MODULE
    CsrAmConverterInit();
#endif
#ifndef EXCLUDE_CSR_APP_MODULE
    CsrAppConverterInit();
#endif
#ifndef EXCLUDE_CSR_BCCMD_MODULE
    CsrBccmdConverterInit();
#endif
#ifndef EXCLUDE_CSR_DHCP_SERVER_MODULE
    CsrDhcpServerConverterInit();
#endif
#ifndef EXCLUDE_CSR_FP_MODULE
    CsrFpConverterInit();
#endif
#ifndef EXCLUDE_CSR_HCI_MODULE
    CsrHciConverterInit();
#endif
#ifndef EXCLUDE_CSR_HQ_MODULE
    CsrHqConverterInit();
#endif
#ifndef EXCLUDE_CSR_TFTP_MODULE
    CsrTftpConverterInit();
#endif
#ifndef EXCLUDE_CSR_TM_BLUECORE_MODULE
    CsrTmBluecoreConverterInit();
#endif
#ifndef EXCLUDE_CSR_UI_MODULE
    CsrUiConverterInit();
#endif
#ifndef EXCLUDE_CSR_VM_MODULE
    CsrVmConverterInit();
#endif
#ifndef EXCLUDE_CSR_DSPM_MODULE
    CsrDspmConverterInit();
#endif
}

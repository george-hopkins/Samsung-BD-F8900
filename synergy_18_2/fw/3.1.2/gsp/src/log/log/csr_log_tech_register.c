/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#include "csr_log_version.h"
#include "csr_log_configure.h"
#include "csr_log_register.h"

#include "csr_am_converter_init.h"
#include "csr_app_converter_init.h"
#include "csr_bccmd_converter_init.h"
#include "csr_dhcp_server_converter_init.h"
#include "csr_dspm_converter_init.h"
#include "csr_fp_converter_init.h"
#include "csr_hci_converter_init.h"
#include "csr_hq_converter_init.h"
#include "csr_tftp_converter_init.h"
#include "csr_tm_bluecore_converter_init.h"
#include "csr_ui_converter_init.h"
#include "csr_vm_converter_init.h"
#include "csr_data_store_converter_init.h"
#include "csr_env_converter_init.h"
#include "csr_fsal_converter_init.h"
#include "csr_ip_ether_converter_init.h"
#include "csr_ip_ifconfig_converter_init.h"
#include "csr_ip_socket_converter_init.h"
#include "csr_tls_converter_init.h"

#define NUMBER_OF_CSR_PRIMITIVES_INFOS 19

void CsrLogTechInfoRegister(void)
{
    static CsrLogPrimitiveInformation primitiveInfo[NUMBER_OF_CSR_PRIMITIVES_INFOS];
    static CsrLogTechInformation techInfo;
    CsrUint32 counter = 0;

#ifndef EXCLUDE_CSR_AM_MODULE
    primitiveInfo[counter++] = *CsrAmTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_APP_MODULE
    primitiveInfo[counter++] = *CsrAppTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BCCMD_MODULE
    primitiveInfo[counter++] = *CsrBccmdTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_DHCP_SERVER_MODULE
    primitiveInfo[counter++] = *CsrDhcpServerTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_DSPM_MODULE
    primitiveInfo[counter++] = *CsrDspmTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_FP_MODULE
    primitiveInfo[counter++] = *CsrFpTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_HCI_MODULE
    primitiveInfo[counter++] = *CsrHciTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_HQ_MODULE
    primitiveInfo[counter++] = *CsrHqTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_TFTP_MODULE
    primitiveInfo[counter++] = *CsrTftpTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_TM_BLUECORE_MODULE
    primitiveInfo[counter++] = *CsrTmBluecoreTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_UI_MODULE
    primitiveInfo[counter++] = *CsrUiTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_VM_MODULE
    primitiveInfo[counter++] = *CsrVmTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_DATA_STORE_MODULE
    primitiveInfo[counter++] = *CsrDataStoreTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_ENV_MODULE
    primitiveInfo[counter++] = *CsrEnvTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_FSAL_MODULE
    primitiveInfo[counter++] = *CsrFsalTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_IP_ETHER_MODULE
    primitiveInfo[counter++] = *CsrIpEtherTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_IP_IFCONFIG_MODULE
    primitiveInfo[counter++] = *CsrIpIfconfigTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_IP_SOCKET_MODULE
    primitiveInfo[counter++] = *CsrIpSocketTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_TLS_MODULE
    primitiveInfo[counter++] = *CsrTlsTechInfoGet();
#endif

    techInfo.techVer = CSR_LOG_VERSION;
    techInfo.primitiveInfoCount = counter;
    techInfo.primitiveInfo = primitiveInfo;

    CsrLogRegisterTechnology(&techInfo);
}
#else
void CsrLogTechInfoRegister(void)
{
}
#endif /* CSR_LOG_ENABLE */


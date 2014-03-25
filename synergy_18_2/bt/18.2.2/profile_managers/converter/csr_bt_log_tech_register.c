/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#include "csr_bt_log_version.h"
#include "csr_log_configure.h"
#include "csr_log_register.h"

#include "csr_bt_att_converter_init.h"
#include "csr_bt_ampm_converter_init.h"
#include "csr_bt_at_converter_init.h"
#include "csr_bt_av_converter_init.h"
#include "csr_bt_avrcp_converter_init.h"
#include "csr_bt_bipc_converter_init.h"
#include "csr_bt_bips_converter_init.h"
#include "csr_bt_bppc_converter_init.h"
#include "csr_bt_bpps_converter_init.h"
#include "csr_bt_bsl_converter_init.h"
#include "csr_bt_cm_converter_init.h"
#include "csr_bt_dg_converter_init.h"
#include "csr_bt_dhcp_converter_init.h"
#include "csr_bt_dunc_converter_init.h"
#include "csr_bt_ftc_converter_init.h"
#include "csr_bt_fts_converter_init.h"
#include "csr_bt_gatt_converter_init.h"
#include "csr_bt_hcrp_converter_init.h"
#include "csr_bt_hdp_converter_init.h"
#include "csr_bt_hf_converter_init.h"
#include "csr_bt_hfg_converter_init.h"
#include "csr_bt_hidd_converter_init.h"
#include "csr_bt_hidh_converter_init.h"
#include "csr_bt_icmp_converter_init.h"
#include "csr_bt_ip_converter_init.h"
#include "csr_bt_jsr82_converter_init.h"
#include "csr_bt_lsl_converter_init.h"
#include "csr_bt_mapc_converter_init.h"
#include "csr_bt_maps_converter_init.h"
#include "csr_bt_opc_converter_init.h"
#include "csr_bt_ops_converter_init.h"
#include "csr_bt_pac_converter_init.h"
#include "csr_bt_pas_converter_init.h"
#include "csr_bt_phdc_ag_converter_init.h"
#include "csr_bt_phdc_mgr_converter_init.h"
#include "csr_bt_ppp_converter_init.h"
#include "csr_bt_prox_srv_converter_init.h"
#include "csr_bt_sapc_converter_init.h"
#include "csr_bt_saps_converter_init.h"
#include "csr_bt_sc_converter_init.h"
#include "csr_bt_sd_converter_init.h"
#include "csr_bt_smlc_converter_init.h"
#include "csr_bt_smls_converter_init.h"
#include "csr_bt_spp_converter_init.h"
#include "csr_bt_syncc_converter_init.h"
#include "csr_bt_syncs_converter_init.h"
#include "csr_bt_therm_srv_converter_init.h"
#include "csr_bt_udp_converter_init.h"
#include "csr_bt_dm_converter_init.h"
#include "csr_bt_l2cap_converter_init.h"
#include "csr_bt_rfcomm_converter_init.h"
#include "csr_bt_sdc_converter_init.h"
#include "csr_bt_sds_converter_init.h"

#define NUMBER_OF_CSR_BT_PRIMITIVES_INFOS 53

void CsrBtLogTechInfoRegister(void)
{
    static CsrLogPrimitiveInformation primitiveInfo[NUMBER_OF_CSR_BT_PRIMITIVES_INFOS];
    static CsrLogTechInformation techInfo;
    CsrUint32 counter = 0;

#ifndef EXCLUDE_CSR_BT_ATT_MODULE
    primitiveInfo[counter++] = *CsrBtAttTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_AMPM_MODULE
    primitiveInfo[counter++] = *CsrBtAmpmTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_AT_MODULE
    primitiveInfo[counter++] = *CsrBtAtTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_AV_MODULE
    primitiveInfo[counter++] = *CsrBtAvTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE
    primitiveInfo[counter++] = *CsrBtAvrcpTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_BIPC_MODULE
    primitiveInfo[counter++] = *CsrBtBipcTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_BIPS_MODULE
    primitiveInfo[counter++] = *CsrBtBipsTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_BPPC_MODULE
    primitiveInfo[counter++] = *CsrBtBppcTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_BPPS_MODULE
    primitiveInfo[counter++] = *CsrBtBppsTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_BSL_MODULE
    primitiveInfo[counter++] = *CsrBtBslTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_CM_MODULE
    primitiveInfo[counter++] = *CsrBtCmTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_DG_MODULE
    primitiveInfo[counter++] = *CsrBtDgTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_DHCP_MODULE
    primitiveInfo[counter++] = *CsrBtDhcpTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_DUNC_MODULE
    primitiveInfo[counter++] = *CsrBtDuncTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_FTC_MODULE
    primitiveInfo[counter++] = *CsrBtFtcTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_FTS_MODULE
    primitiveInfo[counter++] = *CsrBtFtsTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_GATT_MODULE
    primitiveInfo[counter++] = *CsrBtGattTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_HCRP_MODULE
    primitiveInfo[counter++] = *CsrBtHcrpTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_HDP_MODULE
    primitiveInfo[counter++] = *CsrBtHdpTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_HF_MODULE
    primitiveInfo[counter++] = *CsrBtHfTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_HFG_MODULE
    primitiveInfo[counter++] = *CsrBtHfgTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_HIDD_MODULE
    primitiveInfo[counter++] = *CsrBtHiddTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_HIDH_MODULE
    primitiveInfo[counter++] = *CsrBtHidhTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_ICMP_MODULE
    primitiveInfo[counter++] = *CsrBtIcmpTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_IP_MODULE
    primitiveInfo[counter++] = *CsrBtIpTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_JSR82_MODULE
    primitiveInfo[counter++] = *CsrBtJsr82TechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_LSL_MODULE
    primitiveInfo[counter++] = *CsrBtLslTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_MAPC_MODULE
    primitiveInfo[counter++] = *CsrBtMapcTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_MAPS_MODULE
    primitiveInfo[counter++] = *CsrBtMapsTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_OPC_MODULE
    primitiveInfo[counter++] = *CsrBtOpcTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_OPS_MODULE
    primitiveInfo[counter++] = *CsrBtOpsTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_PAC_MODULE
    primitiveInfo[counter++] = *CsrBtPacTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_PAS_MODULE
    primitiveInfo[counter++] = *CsrBtPasTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_PHDC_AG_MODULE
    primitiveInfo[counter++] = *CsrBtPhdcAgTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_MODULE
    primitiveInfo[counter++] = *CsrBtPhdcMgrTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_PPP_MODULE
    primitiveInfo[counter++] = *CsrBtPppTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_PROX_SRV_MODULE
    primitiveInfo[counter++] = *CsrBtProxSrvTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_SAPC_MODULE
    primitiveInfo[counter++] = *CsrBtSapcTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_SAPS_MODULE
    primitiveInfo[counter++] = *CsrBtSapsTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_SC_MODULE
    primitiveInfo[counter++] = *CsrBtScTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_SD_MODULE
    primitiveInfo[counter++] = *CsrBtSdTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_SMLC_MODULE
    primitiveInfo[counter++] = *CsrBtSmlcTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_SMLS_MODULE
    primitiveInfo[counter++] = *CsrBtSmlsTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_SPP_MODULE
    primitiveInfo[counter++] = *CsrBtSppTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_SYNCC_MODULE
    primitiveInfo[counter++] = *CsrBtSynccTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_SYNCS_MODULE
    primitiveInfo[counter++] = *CsrBtSyncsTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_THERM_SRV_MODULE
    primitiveInfo[counter++] = *CsrBtThermSrvTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_UDP_MODULE
    primitiveInfo[counter++] = *CsrBtUdpTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_DM_MODULE
    primitiveInfo[counter++] = *CsrBtDmTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_L2CAP_MODULE
    primitiveInfo[counter++] = *CsrBtL2capTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_RFCOMM_MODULE
    primitiveInfo[counter++] = *CsrBtRfcommTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_SDC_MODULE
    primitiveInfo[counter++] = *CsrBtSdcTechInfoGet();
#endif
#ifndef EXCLUDE_CSR_BT_SDS_MODULE
    primitiveInfo[counter++] = *CsrBtSdsTechInfoGet();
#endif

    techInfo.techVer = CSR_BT_LOG_VERSION;
    techInfo.primitiveInfoCount = counter;
    techInfo.primitiveInfo = primitiveInfo;

    CsrLogRegisterTechnology(&techInfo);
}
#else
void CsrBtLogTechInfoRegister(void)
{
}
#endif /* CSR_LOG_ENABLE */


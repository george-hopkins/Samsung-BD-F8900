/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_msgconv.h"
#ifndef EXCLUDE_CSR_BT_ATT_MODULE
#include "csr_bt_att_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_AMPM_MODULE
#include "csr_bt_ampm_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_AT_MODULE
#include "csr_bt_at_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_AV_MODULE
#include "csr_bt_av_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE
#include "csr_bt_avrcp_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_BIPC_MODULE
#include "csr_bt_bipc_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_BIPS_MODULE
#include "csr_bt_bips_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_BPPC_MODULE
#include "csr_bt_bppc_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_BPPS_MODULE
#include "csr_bt_bpps_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_BSL_MODULE
#include "csr_bt_bsl_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_CM_MODULE
#include "csr_bt_cm_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_DG_MODULE
#include "csr_bt_dg_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_DHCP_MODULE
#include "csr_bt_dhcp_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_DUNC_MODULE
#include "csr_bt_dunc_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_FTC_MODULE
#include "csr_bt_ftc_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_FTS_MODULE
#include "csr_bt_fts_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_GATT_MODULE
#include "csr_bt_gatt_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_HCRP_MODULE
#include "csr_bt_hcrp_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_HDP_MODULE
#include "csr_bt_hdp_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_HF_MODULE
#include "csr_bt_hf_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_HFG_MODULE
#include "csr_bt_hfg_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_HIDD_MODULE
#include "csr_bt_hidd_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_HIDH_MODULE
#include "csr_bt_hidh_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_ICMP_MODULE
#include "csr_bt_icmp_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_IP_MODULE
#include "csr_bt_ip_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_JSR82_MODULE
#include "csr_bt_jsr82_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_LSL_MODULE
#include "csr_bt_lsl_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_MAPC_MODULE
#include "csr_bt_mapc_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_MAPS_MODULE
#include "csr_bt_maps_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_OPC_MODULE
#include "csr_bt_opc_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_OPS_MODULE
#include "csr_bt_ops_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_PAC_MODULE
#include "csr_bt_pac_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_PAS_MODULE
#include "csr_bt_pas_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_PHDC_AG_MODULE
#include "csr_bt_phdc_ag_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_MODULE
#include "csr_bt_phdc_mgr_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_PPP_MODULE
#include "csr_bt_ppp_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_PROX_SRV_MODULE
#include "csr_bt_prox_srv_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_SAPC_MODULE
#include "csr_bt_sapc_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_SAPS_MODULE
#include "csr_bt_saps_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_SC_MODULE
#include "csr_bt_sc_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_SD_MODULE
#include "csr_bt_sd_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_SMLC_MODULE
#include "csr_bt_smlc_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_SMLS_MODULE
#include "csr_bt_smls_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_SPP_MODULE
#include "csr_bt_spp_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_SYNCC_MODULE
#include "csr_bt_syncc_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_SYNCS_MODULE
#include "csr_bt_syncs_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_THERM_SRV_MODULE
#include "csr_bt_therm_srv_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_UDP_MODULE
#include "csr_bt_udp_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_DM_MODULE
#include "csr_bt_dm_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_L2CAP_MODULE
#include "csr_bt_l2cap_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_RFCOMM_MODULE
#include "csr_bt_rfcomm_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_SDC_MODULE
#include "csr_bt_sdc_converter_init.h"
#endif
#ifndef EXCLUDE_CSR_BT_SDS_MODULE
#include "csr_bt_sds_converter_init.h"
#endif

void CsrBtConverterInit(void)
{ 
    CsrMsgConvInit();

#ifndef EXCLUDE_CSR_BT_ATT_MODULE
    CsrBtAttConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_AMPM_MODULE
    CsrBtAmpmConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_AT_MODULE
    CsrBtAtConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_AV_MODULE
    CsrBtAvConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE
    CsrBtAvrcpConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_BIPC_MODULE
    CsrBtBipcConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_BIPS_MODULE
    CsrBtBipsConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_BPPC_MODULE
    CsrBtBppcConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_BPPS_MODULE
    CsrBtBppsConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_BSL_MODULE
    CsrBtBslConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_CM_MODULE
    CsrBtCmConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_DG_MODULE
    CsrBtDgConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_DHCP_MODULE
    CsrBtDhcpConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_DUNC_MODULE
    CsrBtDuncConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_FTC_MODULE
    CsrBtFtcConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_FTS_MODULE
    CsrBtFtsConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_GATT_MODULE
    CsrBtGattConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_HCRP_MODULE
    CsrBtHcrpConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_HDP_MODULE
    CsrBtHdpConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_HF_MODULE
    CsrBtHfConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_HFG_MODULE
    CsrBtHfgConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_HIDD_MODULE
    CsrBtHiddConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_HIDH_MODULE
    CsrBtHidhConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_ICMP_MODULE
    CsrBtIcmpConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_IP_MODULE
    CsrBtIpConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_JSR82_MODULE
    CsrBtJsr82ConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_LSL_MODULE
    CsrBtLslConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_MAPC_MODULE
    CsrBtMapcConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_MAPS_MODULE
    CsrBtMapsConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_OPC_MODULE
    CsrBtOpcConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_OPS_MODULE
    CsrBtOpsConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_PAC_MODULE
    CsrBtPacConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_PAS_MODULE
    CsrBtPasConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_PHDC_AG_MODULE
    CsrBtPhdcAgConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_MODULE
    CsrBtPhdcMgrConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_PPP_MODULE
    CsrBtPppConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_PROX_SRV_MODULE
    CsrBtProxSrvConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_SAPC_MODULE
    CsrBtSapcConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_SAPS_MODULE
    CsrBtSapsConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_SC_MODULE
    CsrBtScConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_SD_MODULE
    CsrBtSdConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_SMLC_MODULE
    CsrBtSmlcConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_SMLS_MODULE
    CsrBtSmlsConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_SPP_MODULE
    CsrBtSppConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_SYNCC_MODULE
    CsrBtSynccConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_SYNCS_MODULE
    CsrBtSyncsConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_THERM_SRV_MODULE
    CsrBtThermSrvConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_UDP_MODULE
    CsrBtUdpConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_DM_MODULE
    CsrBtDmConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_L2CAP_MODULE
    CsrBtL2capConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_RFCOMM_MODULE
    CsrBtRfcommConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_SDC_MODULE
    CsrBtSdcConverterInit();
#endif
#ifndef EXCLUDE_CSR_BT_SDS_MODULE
    CsrBtSdsConverterInit();
#endif
}

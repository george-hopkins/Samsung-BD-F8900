/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>

#include "csr_types.h"
#include "csr_sched_init.h"

#include "csr_bt_declare_tasks.h"

#include "csr_ui_task.h"
#include "csr_app_task.h"
#include "csr_app_main.h"
#include "csr_bt_gap_app_task.h"
#include "csr_bt_spp_app_task.h"
#include "csr_bt_gatt_app_task.h"
#include "csr_bt_hfg_app_task.h"
#include "csr_bt_hf_app_task.h"
#include "csr_bt_av_app_task.h"
#include "csr_bt_ftc_app_task.h"
#include "csr_bt_avrcp_app_task.h"
#include "csr_bt_fts_app_task.h"
#include "csr_bt_phdc_ag_app_task.h"
#include "csr_bt_phdc_mgr_app_task.h"
#include "csr_bt_ampwifi_app_task.h"
#include "csr_bt_demoapp.h"
#include "csr_bt_bootstrap_generic.h"
#include "csr_bt_profiles.h"
#include "csr_tm_bluecore_task.h"
#include "csr_arg_search.h"
#include "csr_bt_log_tech_info_register.h"

#ifdef CSR_BT_APP_AMP_WIFI
#include "csr_wifi_log_tech_info_register.h"
#endif

void CsrAppMainUsage(void)
{
    CsrBtGapUsage();

#if NUM_AV_INST >= 1
    CsrBtAvAppUsage();
#endif


#if NUM_AMPWIFI_INST >= 1
    CsrBtAmpWiFiAppUsage();
#endif
}
        
CsrResult CsrAppMain(void)
{
    return CSR_RESULT_SUCCESS;
}

/* CSR scheduler task initialisation */
void CsrSchedTaskInit(void *data)
{
    CsrUint8 address[6];
    CsrBtDeviceAddr localAddress;
#if (NUM_SPP_INST > 1) || (NUM_GATT_PROXC_INST >= 1||NUM_GATT_THERMC_INST >=1)
    CsrUint16 i = 0; /*used for SPP and Proximity demo */
#endif
#if ((FW_MAJOR_VERSION >= 3) && (FW_MINOR_VERSION >= 0) && (FW_FIXLEVEL_VERSION >= 1)) || ((FW_MAJOR_VERSION >= 3) && (FW_MINOR_VERSION > 0)) || (FW_MAJOR_VERSION > 3)
    CsrUtf8String   *localDeviceName;
    if ((localDeviceName = (CsrUtf8String *)CsrAppMainBluecoreLocalDeviceNameGet()) != NULL) 
    {
        CsrBtBootstrapSetFriendlyName(NULL,localDeviceName);
    }
#endif
    CsrBtBootstrapSetInterface(NULL, CsrAppMainBluecoreTransportGet());
    CsrBtBootstrapSetBitRate(NULL, CsrAppMainBluecorePostBootstrapBaudrateGet());
    CsrBtBootstrapSetFrequency(NULL, (CsrUint16) (CsrAppMainBluecoreXtalFrequencyGet() & 0xFFFF));
    CsrAppMainBluecoreLocalDeviceBluetoothAddressGet(address);
    localAddress.nap = (address[0] << 8) | address[1];
    localAddress.uap = address[2];
    localAddress.lap = (address[3] << 16) | (address[4] << 8) | address[5];
    CsrBtBootstrapSetLocalBtAddress(NULL, &localAddress);

    switch (CsrAppMainBluecoreTransportGet())
    {
#ifdef CSR_BT_APP_USE_SERIAL_COM
        case TRANSPORT_TYPE_BCSP:
            CSR_TM_BLUECORE_TRANSPORT_INIT = CSR_TM_BLUECORE_BCSP_INIT;
            break;
#endif
#ifdef CSR_BT_APP_USE_USB
        case TRANSPORT_TYPE_USB:
            CSR_TM_BLUECORE_TRANSPORT_INIT = CSR_TM_BLUECORE_USB_INIT;
            break;
#endif
#ifdef CSR_BT_APP_USE_SERIAL_COM
        case TRANSPORT_TYPE_H4_DS:
            CSR_TM_BLUECORE_TRANSPORT_INIT = CSR_TM_BLUECORE_H4DS_INIT;
            break;

#ifdef CSR_IBFP_H4I_SUPPORT
        case TRANSPORT_TYPE_H4_I:
            CSR_TM_BLUECORE_TRANSPORT_INIT = CSR_TM_BLUECORE_H4I_INIT;
            break;
#endif

#endif
#ifdef CSR_BT_APP_USE_TYPE_A
        case TRANSPORT_TYPE_TYPE_A:
            CSR_TM_BLUECORE_TRANSPORT_INIT = CSR_TM_BLUECORE_TYPE_A_INIT;
            break;
#endif
        default:
            break;
    }

    /* Initialise standard tasks (see csr_bt_declare_tasks.c) */
    CsrBtAppTaskInitFw(data);
    CsrBtAppTaskInitBt(data);
    CsrBtAppTaskInitWifi(data);

    /* Generic access profile - keep first to display at the top in the UI */
    CsrSchedRegisterTask(&CSR_BT_GAP_APP_IFACEQUEUE, CsrBtGapInit, CsrBtGapDeinit, CsrBtGapHandler, "CSR_BT_APP_GAP", data, ID_STACK);

    /* FW application*/
    CsrSchedRegisterTask(&CSR_APP_IFACEQUEUE, CSR_APP_INIT, CSR_APP_DEINIT, CSR_APP_HANDLER, "CSR_APP", data, ID_APP);
    CsrSchedRegisterTask(&CSR_UI_IFACEQUEUE, CSR_UI_INIT, CSR_UI_DEINIT, CSR_UI_HANDLER, "CSR_UI", data, ID_APP);

#if (NUM_PHDC_AG_INST >= 1) || (NUM_PHDC_MGR_INST >= 1)
    /* We need MCAP and HDP if running with PHDC */
    CsrSchedRegisterTask(&CSR_BT_MCAP_IFACEQUEUE, CSR_BT_MCAP_INIT, CSR_BT_MCAP_DEINIT, CSR_BT_MCAP_HANDLER, "CSR_BT_MCAP", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_HDP_IFACEQUEUE, CSR_BT_HDP_INIT, CSR_BT_HDP_DEINIT, CSR_BT_HDP_HANDLER, "CSR_BT_HDP", data, ID_STACK);
#endif
#if NUM_PHDC_AG_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_PHDC_AG_IFACEQUEUE, CSR_BT_PHDC_AG_INIT, CSR_BT_PHDC_AG_DEINIT, CSR_BT_PHDC_AG_HANDLER, "CSR_BT_PHDC_AG", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_PHDC_AG_APP_IFACEQUEUE, CsrBtPhdcAgAppInit, CsrBtPhdcAgAppDeinit, CsrBtPhdcAgAppHandler, "CSR_BT_APP_PHDC_AG", data, ID_STACK);
#endif
#if NUM_PHDC_MGR_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_PHDC_MGR_IFACEQUEUE, CSR_BT_PHDC_MGR_INIT, CSR_BT_PHDC_MGR_DEINIT, CSR_BT_PHDC_MGR_HANDLER,"CSR_BT_PHDC_MGR", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_PHDC_MGR_APP_IFACEQUEUE, CsrBtPhdcMgrAppInit, CsrBtPhdcMgrAppDeinit, CsrBtPhdcMgrAppHandler, "CSR_BT_APP_PHDC_MGR", data, ID_STACK);
#endif
#if NUM_SPP_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_SPP_IFACEQUEUE, CSR_BT_SPP_INIT, CSR_BT_SPP_DEINIT, CSR_BT_SPP_HANDLER, "CSR_BT_SPP", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_SPP_APP_IFACEQUEUE, CsrBtSppAppInit, CsrBtSppAppDeinit, CsrBtSppAppHandler, "CSR_BT_APP_SPP", data, ID_STACK);
#if NUM_SPP_INST > 1
    for(i=1;i<NUM_SPP_INST;i++)
    {
        CsrSchedRegisterTask(&CSR_BT_SPP_EXTRA_IFACEQUEUE, CSR_BT_SPP_INIT, CSR_BT_SPP_DEINIT, CSR_BT_SPP_HANDLER, "CSR_BT_SPP_EXTRA", data, ID_STACK);
        CsrSchedRegisterTask(&CSR_BT_SPP_APP_EXTRA_IFACEQUEUE, CsrBtSppAppInit, CsrBtSppAppDeinit, CsrBtSppAppHandler, "CSR_BT_APP_SPP_EXTRA", data, ID_STACK);
    }
#endif
#endif
#if NUM_GATT_HIDC_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_HIDC_APP_IFACEQUEUE, CsrBtHidcAppInit, CsrBtHidcAppDeinit, CsrBtHidcAppHandler, "CSR_BT_APP_HIDC", data, ID_STACK);
#endif

#if NUM_GATT_THERMS_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_THERM_SRV_IFACEQUEUE, CSR_BT_THERM_SRV_INIT, CSR_BT_THERM_SRV_DEINIT, CSR_BT_THERM_SRV_HANDLER, "CSR_BT_THERM_SRV", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_THERMS_APP_IFACEQUEUE, CsrBtThermsAppInit, CsrBtThermsAppDeinit, CsrBtThermsAppHandler, "CSR_BT_APP_THERMS", data, ID_STACK);
#endif

#if NUM_GATT_LE_BROWSER_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_LE_BROWSER_APP_IFACEQUEUE, CsrBtLeBrowserAppInit, CsrBtLeBrowserAppDeinit, CsrBtLeBrowserAppHandler, "CSR_BT_APP_LE_BROWSER", data, ID_STACK);
#if NUM_GATT_LE_BROWSER_INST > 1
    for(i=1;i<NUM_GATT_LE_BROWSER_INST;i++)
    {
        CsrSchedRegisterTask(&CSR_BT_LE_BROWSER_APP_EXTRA_IFACEQUEUE, CsrBtLeBrowserAppInit, CsrBtLeBrowserAppDeinit, CsrBtLeBrowserAppHandler, "CSR_BT_APP_LE_BROWSER_EXTRA", data, ID_STACK);
    }
#endif
#endif

#if NUM_GATT_THERMC_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_THERMC_APP_IFACEQUEUE, CsrBtThermcAppInit, CsrBtThermcAppDeinit, CsrBtThermcAppHandler, "CSR_BT_APP_THERMC", data, ID_STACK);
#if NUM_GATT_THERMC_INST > 1
    for(i=1;i<NUM_GATT_THERMC_INST;i++)
    {
        CsrSchedRegisterTask(&CSR_BT_THERMC_APP_EXTRA_IFACEQUEUE, CsrBtThermcAppInit, CsrBtThermcAppDeinit, CsrBtThermcAppHandler, "CSR_BT_APP_THERMC_EXTRA", data, ID_STACK);
    }
#endif
#endif

#if NUM_GATT_GENERIC_SERVER_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_GENERIC_SERVER_APP_IFACEQUEUE, CsrBtGenericServerAppInit, CsrBtGenericServerAppDeinit, CsrBtGenericServerAppHandler, "CSR_BT_APP_GENERIC_SERVER", data, ID_STACK);
#endif

#if NUM_GATT_PROXS_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_PROX_SRV_IFACEQUEUE, CSR_BT_PROX_SRV_INIT, CSR_BT_PROX_SRV_DEINIT, CSR_BT_PROX_SRV_HANDLER, "CSR_BT_PROX_SRV", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_PROXS_APP_IFACEQUEUE, CsrBtProxsAppInit, CsrBtProxsAppDeinit, CsrBtProxsAppHandler, "CSR_BT_APP_PROXS", data, ID_STACK);
#endif

#if NUM_GATT_PROXC_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_PROXC_APP_IFACEQUEUE, CsrBtProxcAppInit, CsrBtProxcAppDeinit, CsrBtProxcAppHandler, "CSR_BT_APP_PROXC", data, ID_STACK);
#if NUM_GATT_PROXC_INST > 1
    for(i=1;i<NUM_GATT_PROXC_INST;i++)
    {
        CsrSchedRegisterTask(&CSR_BT_PROXC_APP_EXTRA_IFACEQUEUE, CsrBtProxcAppInit, CsrBtProxcAppDeinit, CsrBtProxcAppHandler, "CSR_BT_APP_PROXC_EXTRA", data, ID_STACK);
    }
#endif
#endif


#if NUM_FTS_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_FTS_IFACEQUEUE, CSR_BT_FTS_INIT, CSR_BT_FTS_DEINIT, CSR_BT_FTS_HANDLER, "CSR_BT_FTS", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_FTS_APP_IFACEQUEUE, CsrBtFtsAppInit, CsrBtFtsAppDeinit, CsrBtFtsAppHandler, "CSR_BT_APP_FTS", data, ID_STACK);
#endif
#if NUM_FTC_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_FTC_IFACEQUEUE, CSR_BT_FTC_INIT, CSR_BT_FTC_DEINIT, CSR_BT_FTC_HANDLER, "CSR_BT_FTC", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_FTC_APP_IFACEQUEUE, CsrBtFtcAppInit, CsrBtFtcAppDeinit, CsrBtFtcAppHandler, "CSR_BT_APP_FTC", data, ID_STACK);
#endif
#if NUM_HFG_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_HFG_IFACEQUEUE, CSR_BT_HFG_INIT, CSR_BT_HFG_DEINIT, CSR_BT_HFG_HANDLER, "CSR_BT_HFG", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_HFG_APP_IFACEQUEUE, CsrBtHfgAppInit, CsrBtHfgAppDeinit, CsrBtHfgAppHandler, "CSR_BT_APP_HFG", data, ID_STACK);
#endif
#if NUM_HF_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_HF_IFACEQUEUE, CSR_BT_HF_INIT, CSR_BT_HF_DEINIT, CSR_BT_HF_HANDLER, "CSR_BT_HF", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_HF_APP_IFACEQUEUE, CsrBtHfAppInit, CsrBtHfAppDeinit, CsrBtHfAppHandler, "CSR_BT_APP_HF", data, ID_STACK);
#endif
#if NUM_AVRCP_INST >=1
#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE
    CsrSchedRegisterTask(&CSR_BT_AVRCP_IFACEQUEUE, CSR_BT_AVRCP_INIT, CSR_BT_AVRCP_DEINIT, CSR_BT_AVRCP_HANDLER, "CSR_BT_AVRCP", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_AVRCP_APP_IFACEQUEUE, CsrBtAvrcpAppInit, CsrBtAvrcpAppDeinit, CsrBtAvrcpAppHandler, "CSR_BT_APP_AVRCP", data, ID_STACK);
#endif
#endif
#if NUM_AV_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_AV_IFACEQUEUE, CSR_BT_AV_INIT, CSR_BT_AV_DEINIT, CSR_BT_AV_HANDLER, "CSR_BT_AV", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_AV_APP_IFACEQUEUE, CsrBtAvAppInit, CsrBtAvAppDeinit, CsrBtAvAppHandler, "CSR_BT_APP_AV", data, ID_STACK);
#endif
#if NUM_AMPWIFI_INST >= 1
    CsrSchedRegisterTask(&CSR_BT_AMPWIFI_APP_IFACEQUEUE, CsrBtAmpWifiAppInit, CsrBtAmpWifiAppDeinit, CsrBtAmpWifiAppHandler, "CSR_BT_APP_AMPWIFI", data, ID_STACK);
#endif

/* Enable TFTP access - Runs in the APP sched instance to reduce the load on the ID_STACK sched instance*/
#ifdef __arm
    /* ARM needs the IP stack */
    CsrSchedRegisterTask(&CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_INIT, CSR_BT_IP_DEINIT, CSR_BT_IP_TASK, "CSR_BT_IP", data, ID_APP);
    CsrSchedRegisterTask(&CSR_BT_ICMP_IFACEQUEUE, CSR_BT_ICMP_INIT, CSR_BT_ICMP_DEINIT, CSR_BT_ICMP_TASK, "CSR_BT_ICMP", data, ID_APP);
    CsrSchedRegisterTask(&CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_INIT, CSR_BT_UDP_DEINIT, CSR_BT_UDP_HANDLER, "CSR_BT_UDP", data, ID_APP);
    CsrSchedRegisterTask(&CSR_BT_DHCP_IFACEQUEUE, CSR_BT_DHCP_INIT, CSR_BT_DHCP_DEINIT, CSR_BT_DHCP_TASK, "CSR_BT_DHCP", data, ID_APP);
    CsrSchedRegisterTask(&CSR_BT_TFTP_IFACEQUEUE, CSR_BT_TFTP_INIT, CSR_BT_TFTP_DEINIT, CSR_BT_TFTP_HANDLER, "CSR_BT_TFTP", data, ID_APP);
    CsrSchedRegisterTask(&CSR_BT_CTRL_IFACEQUEUE, CSR_BT_CTRL_INIT, CSR_BT_CTRL_DEINIT, CSR_BT_CTRL_TASK, "CSR_BT_CTRL", data, ID_APP);
#endif /* __arm */

    CsrSchedRegisterTask(&CSR_BT_OPC_IFACEQUEUE, CSR_BT_OPC_INIT, CSR_BT_OPC_DEINIT, CSR_BT_OPC_HANDLER, "CSR_BT_OPC", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_OPS_IFACEQUEUE, CSR_BT_OPS_INIT, CSR_BT_OPS_DEINIT, CSR_BT_OPS_HANDLER, "CSR_BT_OPC", data, ID_STACK);


#ifdef CSR_LOG_ENABLE
    CsrBtLogTechInfoRegister();
#ifdef CSR_BT_APP_AMP_WIFI
    CsrWifiLogTechInfoRegister();
#if WIFI_MAJOR_VERSION == 4
    {
        extern void CsrWifiLogTextRegister(void);
        CsrWifiLogTextRegister();
    }
#endif
#endif
#endif
}

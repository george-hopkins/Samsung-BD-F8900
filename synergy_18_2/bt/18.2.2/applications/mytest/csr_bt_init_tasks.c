/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_declare_tasks.h"

/* Transport handler function. Set by the cmdline parser depending on
 * what transport the user selects */
schedEntryFunction_t CSR_TM_BLUECORE_TRANSPORT_INIT = NULL;

/* Initialise standard, mandatory Bluetooth tasks */
void CsrBtAppTaskInitBt(void *data)
{
    CsrSchedRegisterTask(&DM_IFACEQUEUE, DM_INIT, DM_DEINIT, DM_TASK, "CSR_BT_DM", data, ID_STACK);
    CsrSchedRegisterTask(&DM_HCI_IFACEQUEUE, DM_HCI_INIT, DM_HCI_DEINIT, DM_HCI_TASK, "CSR_BT_DM_HCI", data, ID_STACK);
    CsrSchedRegisterTask(&L2CAP_IFACEQUEUE, L2CAP_INIT, L2CAP_DEINIT, L2CAP_TASK, "CSR_BT_L2CAP", data, ID_STACK);
    CsrSchedRegisterTask(&RFCOMM_IFACEQUEUE, RFCOMM_INIT, RFCOMM_DEINIT, RFCOMM_TASK, "CSR_BT_RFCOMM", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_BNEP_IFACEQUEUE, CSR_BT_BNEP_INIT, CSR_BT_BNEP_DEINIT, CSR_BT_BNEP_HANDLER, "CSR_BT_BNEP", data, ID_STACK);
    CsrSchedRegisterTask(&SDP_L2CAP_IFACEQUEUE, SDP_L2CAP_INIT, SDP_L2CAP_DEINIT, SDP_L2CAP_TASK, "CSR_BT_SDP_L2CAP", data, ID_STACK);
    CsrSchedRegisterTask(&SDP_IFACEQUEUE, SDP_INIT, SDP_DEINIT, SDP_TASK, "CSR_BT_SDP", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_CM_IFACEQUEUE, CSR_BT_CM_INIT, CSR_BT_CM_DEINIT, CSR_BT_CM_HANDLER, "CSR_BT_CM", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_SC_IFACEQUEUE, CSR_BT_SC_INIT, CSR_BT_SC_DEINIT, CSR_BT_SC_HANDLER, "CSR_BT_SC", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_SD_IFACEQUEUE, CSR_BT_SD_INIT, CSR_BT_SD_DEINIT, CSR_BT_SD_HANDLER, "CSR_BT_SD", data, ID_STACK);

#ifdef CSR_AMP_ENABLE
    CsrSchedRegisterTask(&CSR_BT_AMPM_IFACEQUEUE, CSR_BT_AMPM_INIT, CSR_BT_AMPM_DEINIT, CSR_BT_AMPM_HANDLER, "CSR_BT_AMPM", data, ID_STACK);
#endif /* CSR_AMP_ENABLE */

#ifdef CSR_BT_LE_ENABLE
    CsrSchedRegisterTask(&ATT_IFACEQUEUE, ATT_INIT, ATT_DEINIT, ATT_TASK, "CSR_BT_ATT", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_GATT_IFACEQUEUE, CSR_BT_GATT_INIT, CSR_BT_GATT_DEINIT, CSR_BT_GATT_HANDLER, "CSR_BT_GATT", data, ID_STACK);
#endif /* CSR_BT_APP_AMP_FAKE */
}

/* Initialise standard (mandatory) Framework tasks */
void CsrBtAppTaskInitFw(void *data)
{
    CsrSchedRegisterTask(&CSR_BCCMD_IFACEQUEUE, CSR_BCCMD_INIT, CSR_BCCMD_DEINIT, CSR_BCCMD_HANDLER, "CSR_BCCMD", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_TM_BLUECORE_IFACEQUEUE, CSR_TM_BLUECORE_TRANSPORT_INIT, CSR_TM_BLUECORE_DEINIT, CSR_TM_BLUECORE_HANDLER, "CSR_TM_BLUECORE", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_HQ_IFACEQUEUE, CSR_HQ_INIT, CSR_HQ_DEINIT, CSR_HQ_HANDLER, "CSR_HQ", data, ID_STACK);
//    CsrSchedRegisterTask(&CSR_AM_IFACEQUEUE, CSR_AM_INIT, CSR_AM_DEINIT, CSR_AM_HANDLER, "CSR_AM", data, ID_STACK);
#ifdef CSR_USE_DSPM
    CsrSchedRegisterTask(&CSR_DSPM_IFACEQUEUE, CsrDspmInit, CsrDspmDeinit, CsrDspmHandler, "CSR_DSPM", data, ID_STACK);
#endif
    CsrSchedRegisterTask(&CSR_HCI_IFACEQUEUE, CSR_HCI_INIT, CSR_HCI_DEINIT, CSR_HCI_HANDLER, "CSR_HCI", data, ID_STACK);
}

/* Initialise standard WiFi tasks for AMP/COEX */
void CsrBtAppTaskInitWifi(void *data)
{
    /* WiFi tasks */
#if defined(CSR_BT_APP_AMP_WIFI) 
    CsrSchedRegisterTask(&CSR_WIFI_SME_IFACEQUEUE, CsrWifiSmeInit, CsrWifiSmeDeinit, CsrWifiSmeHandler, "CSR_WIFI_SME", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_WIFI_ROUTER_IFACEQUEUE, CsrWifiRouterInit, CsrWifiRouterDeinit, CsrWifiRouterHandler, "CSR_WIFI_ROUTER", data, ID_STACK);
#endif /* CSR_BT_APP_AMP_WIFI */

#if defined(CSR_BT_APP_AMP_WIFI) && defined(CSR_AMP_ENABLE)
    CsrSchedRegisterTask(&CSR_WIFI_PAL_DATA_IFACEQUEUE, CsrWifiPalDataInit, CsrWifiPalDataDeinit, CsrWifiPalDataHandler, "CSR_WIFI_PAL_DATA", data, ID_STACK);
#endif /* CSR_BT_APP_AMP_WIFI && CSR_AMP_ENABLE */
}

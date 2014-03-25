/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_sched_init.h"
#include "csr_bt_declare_tasks.h"
#include "csr_result.h"
//#include "csr_ip_task.h"
#include "csr_bt_demoapp.h"
#include "csr_bt_bootstrap_generic.h"
#include "csr_bt_profiles.h"
#include "csr_tm_bluecore_task.h"
#include "csr_arg_search.h"
#include "csr_app_main.h"

//
/* CSR scheduler task initialisation */


void CsrAppMainUsage(void)
{

}
        
CsrResult CsrAppMain(void)
{
    return CSR_RESULT_SUCCESS;
}


void CsrSchedTaskInit(void *data)
{
		CsrUint8 address[6];
		CsrBtDeviceAddr localAddress;
#if (NUM_SPP_INST > 1) || (NUM_GATT_PROXC_INST >= 1||NUM_GATT_THERMC_INST >=1)
		CsrUint16 i = 0; /*used for SPP and Proximity demo */
#endif
#if ((FW_MAJOR_VERSION >= 3) && (FW_MINOR_VERSION >= 0) && (FW_FIXLEVEL_VERSION >= 1)) || ((FW_MAJOR_VERSION >= 3) && (FW_MINOR_VERSION > 0)) || (FW_MAJOR_VERSION > 3)
		CsrUtf8String	*localDeviceName;
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
    /* Synergy BT high-level protocols */
//	CsrSchedRegisterTask(&CSR_IP_IFACEQUEUE, CSR_IP_INIT, CSR_IP_DEINIT, CSR_IP_HANDLER, "CSR_IP", data, ID_STACK);
	CsrSchedRegisterTask(&CSR_BT_HIDH_IFACEQUEUE, CSR_BT_HIDH_INIT, CSR_BT_HIDH_DEINIT, CSR_BT_HIDH_HANDLER, "CSR_BT_HIDH", data, ID_STACK);
	CsrSchedRegisterTask(&CSR_BT_AV_IFACEQUEUE, CSR_BT_AV_INIT, CSR_BT_AV_DEINIT, CSR_BT_AV_HANDLER, "CSR_BT_AV", data, ID_STACK);
	CsrSchedRegisterTask(&CSR_BT_AVRCP_IFACEQUEUE, CSR_BT_AVRCP_INIT, CSR_BT_AVRCP_DEINIT, CSR_BT_AVRCP_HANDLER, "CSR_BT_AVRCP", data, ID_STACK);
    CsrSchedRegisterTask(&TESTQUEUE, CSR_BT_TEST_INIT, CSR_BT_TEST_DEINIT, CSR_BT_TEST_HANDLER, "APP_BT_MAIN", data, ID_STACK);
}

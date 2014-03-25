#ifndef APP_HIDH_HANDER_H__
#define APP_HIDH_HANDER_H__


/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
#include "csr_bt_hidh_prim.h"
#include "AppHIDHidhData.h"
/* Definiton of the struct type for the CM application event functions  */



struct CsrBtAppHIDHEventHandlerStruct
{  
	/* HIDH_ACTIVATATE_CFM                */
	void (*csrBtHidhAtivateCfmFunc)(void);
	/* HIDH_DEACTIVATATE_CFM                */
	void (*csrBtHidhDeActivateCfmFunc)(void);
	/*HIDH_CONNECT_IND*/
	void (*csrBtHidhConnectIndFunc)(CsrBtDeviceAddr bdAddr);                
	/*   HIDH_CONNECT_CFM,*/
	void (*csrBtHidhConnectCfmFunc)( CsrBtResultCode       resultCode,CsrBtSupplier   resultSupplier, CsrAppBtHidhSdpInfoData  * infodata);
	/*HIDH_DISCONNECT_IND*/
	void (*csrBtHidhDisconnectIndFunc) (CsrBtDeviceAddr bdAddr,CsrBtResultCode reasonCode,CsrBtSupplier reasonSupplier);  
	/*HIDH_DATA_IND*/
	void (*csrBtHidhDataIndFunc) (CsrUint16              dataLen, CsrUint8               *data);
	/*HIDH_STATUS_IND*/
	void (*csrBtHidhStatusIndFunc)(CsrBtHidhStatus         status);
    /*HIDH_CONNECT_ACCEPT_CFM*/
	void (*csrBtHidhConnectAcceptIndHandler)(BD_ADDR_T addr,CsrBtResultCode        avResultCode, CsrBtSupplier          avResultSupplier);
    /*HIDH_DISCONNECT_CFM*/
	void (*csrBtHidhDisconnectCfmFunc) (CsrBtDeviceAddr bdAddr,CsrBtResultCode reasonCode,CsrBtSupplier reasonSupplier);
};

typedef struct CsrBtAppHIDHEventHandlerStruct CsrBtAppHIDHEventHandlerStruct;


//extern void CsrBtAppHIDHEventHandler(  CsrUint16 message, void* data, CsrUint16 length);



extern CsrInt16 csr_hidh_active_cmd(CsrBtAppHIDHEventHandlerStruct * btAvrcCallbackFunctionsPtr );
extern CsrInt16 csr_hidh_deactive_cmd(void);
extern CsrInt16 csr_hidh_deactive_cmd(void);
extern CsrInt16 csr_hidh_connect_cmd(CsrAppBtHidhSdpInfoData *pHidData );
extern CsrInt16 csr_hidh_connect_accecpt_cmd(CsrAppBtHidhSdpInfoData *infodata );
extern CsrInt16 csr_hidh_disconnect_cmd(BD_ADDR_T bdaddr );
extern CsrInt16 csr_hidh_control_cmd(BD_ADDR_T bdaddr,CsrBtHidhCtrl operation );
extern CsrInt16 csr_hidh_data_cmd(CsrBtHidhReport         reportType,CsrUint16    reportLen,CsrUint8     *reportData);

#ifdef __cplusplus
}
#endif

#endif//APP_HIDH_HANDER_H__
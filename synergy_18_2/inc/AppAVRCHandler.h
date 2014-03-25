#ifndef APP_AVRC_HANDER_H__
#define APP_AVRC_HANDER_H__


/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
#include "csr_bt_avrcp_prim.h"
/* Definiton of the struct type for the CM application event functions  */
struct CsrBtAppAVRCEventHandlerStruct
{
   
   /* AVRC_ACTIVATATE_CFM                */
    void (*csrBtAVRCAtivateCfmFunc)(void);
    /* AVRC_DEACTIVATATE_CFM                */
    void (*csrBtAVRCDeActivateCfmFunc)(void);
	 /*AVRC_CONNECT_IND*/
    void (*csrBtAVRCConnectIndFunc)(CsrBtDeviceAddr bdAddr);
	 /*AVRC_REMOTE_FEATURES_IND*/
    void (*csrBtAVRCFeaturesIndFunc) (CsrBtAvrcpConfigSrFeatureMask   srFeatures);
	 /*AVRC_CT_PASS_THROUGH_CFM*/
	void (*csrBtAVRCPassThroughCfmFunc)(CsrBtAvrcpPTOpId operationId, CsrBtResultCode             resultCode,CsrBtSupplier          avResultSupplier);                   
	  /*AVRC_CLOSE_IND*/
    void (*csrBtAVRCCloseIndFunc) (void);
	  /*   AVRC_CONNECT_CFM,*/
    void (*csrBtAVRCConnectCfmFunc)(BD_ADDR_T addr,CsrBtResultCode        avResultCode, CsrBtSupplier          avResultSupplier);
		/*AVRC_DISCONNECT_IND*/
    void (*csrBtAVRCDisconnectIndFunc) (BD_ADDR_T addr,CsrUint16 reasonCode,CsrUint16 reasonSupplier);  
		 /*AVRC_CT_PASS_THROUGH_IND*/
	void (*csrBtAVRCPassThroughIndFunc)(CsrBtAvrcpPTOpId operationId, CsrBtAvrcpPTState             state);  
			/*AVRC_DISCONNECT_CFM*/
    void (*csrBtAVRCDisconnectCfmFunc) (BD_ADDR_T addr,CsrUint16 reasonCode,CsrUint16 reasonSupplier); 	 
};
typedef struct CsrBtAppAVRCEventHandlerStruct CsrBtAppAVRCEventHandlerStruct;


//extern void CsrBtAppAVRCEventHandler(  CsrUint16 message, void* data, CsrUint16 length);



extern CsrInt16 csr_avrc_active_cmd(CsrBtAppAVRCEventHandlerStruct * btAvrcCallbackFunctionsPtr );
extern CsrInt16 csr_avrc_deactive_cmd(void);
extern CsrInt16 csr_avrc_ct_pass_through_cmd(CsrUint8   opId, CsrUint8   state);
extern CsrInt16 csr_avrc_deactive_cmd(void);
extern CsrInt16 csr_avrc_connect_cmd(BD_ADDR_T bdaddr );
extern CsrInt16 csr_avrc_disconnect_cmd(BD_ADDR_T bdaddr );
extern CsrInt16 csr_avrc_ct_set_volume_cmd(CsrUint8   volume, CsrUint8   isMute);
extern CsrInt16 csr_avrc_connecting_cancel_cmd(BD_ADDR_T bdaddr );


#ifdef __cplusplus
}
#endif

#endif//APP_AVRC_HANDER_H__
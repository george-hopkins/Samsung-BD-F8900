#ifndef APP_AV_HANDER_H__
#define APP_AV_HANDER_H__


/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
#include "csr_bt_result.h"


/* Definiton of the struct type for the CM application event functions  */
struct CsrBtAppAVEventHandlerStruct
{
   
   /* AV_ACTIVATATE_CFM                */
    void (*csrBtAvAtivateCfmFunc)(void);
      /* AV_DEACTIVATATE_CFM                */
    void (*csrBtAvDeActivateCfmFunc)(void);
	  /*AV_CONNECT_IND*/
    void (*csrBtAvConnectIndFunc)(BD_ADDR_T addr);
	  /*AV_CONFIG_IND*/
    void (*csrBtAvConfigIndFunc) (CsrUint16 pcm_sampling_rate, CsrUint8  pcm_no_channels);
	  /*AV_PCM_STREAM_DATA_IND*/
    void (*csrBtAvPcmStreamDataIndFunc)(CsrUint8 *p_data, CsrUint16 len, CsrUint32 sampling_rate, CsrUint8 number_of_channelss);
	  /*AV_CLOSE_IND*/
    void (*csrBtAvCloseIndFunc) (void);
	  /*AV_CLOSE_CFM*/
	void (*csrBtAvCloseCfmFunc) (CsrBool result);  
	  /*AV_CONNECT_CFM*/
	void (*csrBtAvConnectCfmFunc)(BD_ADDR_T addr,CsrBtResultCode        avResultCode, CsrBtSupplier          avResultSupplier);
      /*AV_DISCONNECT_IND*/
	 void (*csrBtAvDisconectIndFunc) (BD_ADDR_T addr,CsrBtResultCode reasonCode,CsrBtSupplier reasonSupplier);  //reasonSupplier  csr_bt_result.h   reasonCode: csr_bt_**_prim.h
		  /*AV_OPEN_IND*/
    void (*csrBtAvOpenIndFunc) (CsrBool  ss_filter); 
		  /*AV_CONFIG_CFM*/
    void (*csrBtAvConfigCfmFunc) (CsrUint16 pcm_sampling_rate, CsrUint8  pcm_no_channels);
		  /*AV_OPEN_CFM*/
    void (*csrBtAvOpenCfmFunc) (CsrBool  ss_filter); 
          /*AV_DISCONNECT_CFM*/
	 void (*csrBtAvDisconectCfmFunc) (BD_ADDR_T addr,CsrBtResultCode reasonCode,CsrBtSupplier reasonSupplier); 
		  /*AV_START_IND*/
    void (*csrBtAvStartIndFunc) (void); 
		  /*AV_SUSPEND_IND*/
	 void (*csrBtAvSuspendIndFunc) (void); 	  
};
typedef struct CsrBtAppAVEventHandlerStruct CsrBtAppAVEventHandlerStruct;



extern CsrInt16 csr_av_active_cmd(CsrBtAppAVEventHandlerStruct * BtAvCallbackFunctionsPtr );

extern CsrInt16 csr_av_deactvie_cmd(void );

extern CsrInt16 csr_av_connect_cmd(BD_ADDR_T bdaddr );

extern CsrInt16 csr_av_disconnect_cmd(BD_ADDR_T bdaddr );

extern CsrInt16 csr_av_conneting_cancel_cmd(BD_ADDR_T bdaddr );


#ifdef __cplusplus
}
#endif

#endif//APP_AV_CMD_HANDER_H__

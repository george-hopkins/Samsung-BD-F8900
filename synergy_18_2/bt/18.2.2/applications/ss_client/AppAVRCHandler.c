/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "bluetooth.h"
#include "csr_bt_ss_prim.h"
#include "csr_util.h"
#include "csr_pmem.h"
#include "peer_com_linux_u2u.h"
#include "AppAVRCHandler.h"
#include "csr_samsung_log.h"





typedef void (* csrBtAVRCAppStateHandlerType)(csr_api_avrc_prim_type message, void* data, CsrUint16 length);


static CsrBtAppAVRCEventHandlerStruct * CsrBtAVRCAppEventFunctionsPtr=NULL;   
static BD_ADDR_T 		 deviceAvrcAddrNull; 



//////////////AVRC CMD
CsrInt16 csr_avrc_active_cmd(CsrBtAppAVRCEventHandlerStruct * btAvrcCallbackFunctionsPtr )
{
  DBG_SS_PRINT("csr_av_actvie_cmd\n");

   if(btAvrcCallbackFunctionsPtr == NULL)
	{
		return FALSE;
	}
    CsrBtAVRCAppEventFunctionsPtr = btAvrcCallbackFunctionsPtr;
   

   
  return csr_bt_client_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID,AVRC_ACTIVATATE_REQ,NULL, 0 );
}

CsrInt16 csr_avrc_deactive_cmd(void)
{
  DBG_SS_PRINT("csr_av_deactvie_cmd\n");

  return csr_bt_client_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID,AVRC_DEACTIVATATE_REQ,NULL, 0 );
}

CsrInt16 csr_avrc_ct_pass_through_cmd(CsrUint8   opId, CsrUint8   state)
{
  void *data;
  CsrUint16 data_len;
  data_len=sizeof(opId)+sizeof(state);
  DBG_SS_PRINT("csr_avrc_ct_pass_through_cmd\n");
  

  data=CsrPmemAlloc(data_len);
  CsrMemCpy(data,&opId,sizeof(opId));
  CsrMemCpy(data+sizeof(opId),&state,sizeof(state));

  
  return csr_bt_client_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID,AVRC_CT_PASS_THROUGH_REQ,data, data_len );
}
CsrInt16 csr_avrc_connect_cmd(BD_ADDR_T bdaddr )
{
   void * data= CsrPmemAlloc(sizeof(BD_ADDR_T));

   CsrMemCpy(data,&bdaddr,sizeof(BD_ADDR_T));
   
   DBG_SS_PRINT("csr_avrc_connect_cmd\n");

      if(CsrMemCmp(&(bdaddr), &(deviceAvrcAddrNull),sizeof(BD_ADDR_T))==0)
		return CSR_ERR_CLI_BAD_PARAM;
  return  csr_bt_client_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID,AVRC_CONNECT_REQ,data, sizeof(BD_ADDR_T) );
}

CsrInt16 csr_avrc_disconnect_cmd(BD_ADDR_T bdaddr )
{
   DBG_SS_PRINT("csr_avrc_disconnect_cmd\n");

   
  return csr_bt_client_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID,AVRC_DISCONNECT_REQ,NULL, 0 );
}

CsrInt16 csr_avrc_ct_set_volume_cmd(CsrUint8   volume, CsrUint8   isMute)
{
  void *data;
  CsrUint16 data_len;
  data_len=sizeof(volume)+sizeof(isMute);
  DBG_SS_PRINT("csr_avrc_ct_set_volume_cmd\n");
  

  data=CsrPmemAlloc(data_len);
  CsrMemCpy(data,&volume,sizeof(volume));
  CsrMemCpy(data+sizeof(volume),&isMute,sizeof(isMute));

  
  return csr_bt_client_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID,AVRC_CT_SET_VOLUME_REQ,data, data_len );

}


CsrInt16 csr_avrc_connecting_cancel_cmd(BD_ADDR_T bdaddr )
{
   void * data= CsrPmemAlloc(sizeof(BD_ADDR_T));

   CsrMemCpy(data,&bdaddr,sizeof(BD_ADDR_T));
   
   DBG_SS_PRINT("csr_avrc_connecting_cancel_cmd\n");

      if(CsrMemCmp(&(bdaddr), &(deviceAvrcAddrNull),sizeof(BD_ADDR_T))==0)
		return CSR_ERR_CLI_BAD_PARAM;
  return  csr_bt_client_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID,AVRC_CONNECTING_CANCEL_REQ,data, sizeof(BD_ADDR_T) );
}



//////////////AVRC EVENT
static void csrBtAVRCActivateCfmHandler(csr_api_avrc_prim_type message, void* data, CsrUint16 length)
{
    if(CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCAtivateCfmFunc == NULL)
    {
        DBG_SS_PRINT("csrBtAVRCActivateCfmHandler Event function not defined\n");
    }
    else
    {
        CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCAtivateCfmFunc();
    }
}

static void csrBtAVRCDeActivateCfmHandler(csr_api_avrc_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCDeActivateCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAVRCDeActivateCfmHandler Event function not defined\n");
    }
    else
    {
        CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCDeActivateCfmFunc();
    }
}

static void csrBtAVRCConnectIndHandler(csr_api_avrc_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCConnectIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAVRCConnectIndHandler Event function not defined\n");
    }
    else
    {
        CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCConnectIndFunc(*((CsrBtDeviceAddr*) data));
    }
}
                                            

static void csrBtAVRCRemoteFeatureIndHandler(csr_api_avrc_prim_type message, void* data, CsrUint16 length)
{
   

    if(CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCFeaturesIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAVRCConfigIndFunc Event function not defined\n");
    }
    else
    {
      
		CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCFeaturesIndFunc(*((CsrBtAvrcpConfigSrFeatureMask *)data));
	}
}
static void csrBtAVRCPassThroughCfmHandler(csr_api_avrc_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCPassThroughCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtCmReadTxPowerLevelCfmFunc Event function not defined\n");
    }
    else
    {
        CsrBtAvrcpPTOpId operationId;
		CsrBtResultCode             resultCode;
		CsrBtSupplier          avResultSupplier;
		CsrMemCpy(&operationId,data,sizeof(operationId));
		CsrMemCpy(&resultCode,data+sizeof(operationId),sizeof(resultCode));	
		CsrMemCpy(&avResultSupplier,data+sizeof(operationId)+sizeof(resultCode),sizeof(avResultSupplier));	
        CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCPassThroughCfmFunc(operationId,resultCode,avResultSupplier);
    }
}

static void csrBtAVRCCloseIndHandler(csr_api_avrc_prim_type message, void* data, CsrUint16 length)
{
    if(CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCCloseIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtCmReadTxPowerLevelCfmFunc Event function not defined\n");
    }
    else
    {
        CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCCloseIndFunc();
    }
}

static void csrBtAVRConectCfmHandler(csr_api_avrc_prim_type message, void* data, CsrUint16 length)
{
    if(CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCConnectCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAVRConectCfmHandler Event function not defined\n");
    }
    else
    {
		BD_ADDR_T Bdaddr;
        CsrBtResultCode 	   avResultCode;
		CsrBtSupplier			avResultSupplier;

		CsrMemCpy(&Bdaddr,data,sizeof(BD_ADDR_T));
		CsrMemCpy(&avResultCode,data+sizeof(BD_ADDR_T),sizeof(CsrBtResultCode));
		CsrMemCpy(&avResultSupplier,data+sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode),sizeof(CsrBtSupplier));

        CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCConnectCfmFunc(Bdaddr,avResultCode,avResultSupplier);
    }
}


static void csrBtAVRCDisconectIndHandler(csr_api_avrc_prim_type message, void* data, CsrUint16 length)
{
    if(CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCDisconnectIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAVRCDisconectIndHandler Event function not defined\n");
    }
    else
    {

	    BD_ADDR_T  Bdaddr;
	    CsrUint16 reasonCode;
	    CsrUint16 reasonSupplier;
#ifdef EXTERNAL_DEBUG_PCM      
        fclose(avInstnceData.localPCMFile);
#endif
        CsrMemCpy(&Bdaddr,data,sizeof(BD_ADDR_T));
		CsrMemCpy(&reasonCode,data+sizeof(BD_ADDR_T),sizeof(CsrUint16));
		CsrMemCpy(&reasonSupplier,data+sizeof(BD_ADDR_T)+sizeof(CsrUint16),sizeof(CsrUint16));

		
        CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCDisconnectIndFunc(Bdaddr,reasonCode,reasonSupplier);
    }
}


static void csrBtAVRCDisconectCfmHandler(csr_api_avrc_prim_type message, void* data, CsrUint16 length)
{
    if(CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCDisconnectCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAVRCDisconectIndHandler Event function not defined\n");
    }
    else
    {

	    BD_ADDR_T  Bdaddr;
	    CsrUint16 reasonCode;
	    CsrUint16 reasonSupplier;
#ifdef EXTERNAL_DEBUG_PCM      
        fclose(avInstnceData.localPCMFile);
#endif
        CsrMemCpy(&Bdaddr,data,sizeof(BD_ADDR_T));
		CsrMemCpy(&reasonCode,data+sizeof(BD_ADDR_T),sizeof(CsrUint16));
		CsrMemCpy(&reasonSupplier,data+sizeof(BD_ADDR_T)+sizeof(CsrUint16),sizeof(CsrUint16));

		
        CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCDisconnectCfmFunc(Bdaddr,reasonCode,reasonSupplier);
    }
}


static void csrBtAVRCPassThroughIndHandler(csr_api_avrc_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCPassThroughIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtCmReadTxPowerLevelCfmFunc Event function not defined\n");
    }
    else
    {
        CsrBtAvrcpPTOpId operationId;
		CsrBtAvrcpPTState             state;
		CsrMemCpy(&operationId,data,sizeof(operationId));
		CsrMemCpy(&state,data+sizeof(operationId),sizeof(state));	
        CsrBtAVRCAppEventFunctionsPtr->csrBtAVRCPassThroughIndFunc(operationId,state);
    }
}


static const csrBtAVRCAppStateHandlerType csrBtAVRCAppStateHandler[AVRC_UPSTREAM_CNT] =
{
    csrBtAVRCActivateCfmHandler,                      /* AVRC_ACTIVATATE_CFM                	*/
    csrBtAVRCDeActivateCfmHandler,                    /* AVRC_DEACTIVATATE_CFM            	*/
    csrBtAVRCConnectIndHandler,                       /* AVRC_CONNECT_IND                      	*/
    csrBtAVRCRemoteFeatureIndHandler,                 /*AVRC_REMOTE_FEATURES_IND		*/
    csrBtAVRCPassThroughCfmHandler,                   /*AVRC_CT_PASS_THROUGH_CFM*/
    csrBtAVRCCloseIndHandler,                         /*AVRC_CLOSE_IND		*/
    csrBtAVRConectCfmHandler,					  	  /*AVRC_CONNECT_CFM*/
    csrBtAVRCDisconectIndHandler,                     /* AVRC_DISCONNECT_IND*/
    csrBtAVRCPassThroughIndHandler,                   /*AVRC_TG_PASS_THROUGH_IND*/
    csrBtAVRCDisconectCfmHandler						/*AVRC_DISCONNECT_CFM*/
};


void CsrBtAVRCAppEventHandler(  CsrUint16 message, void* data, CsrUint16 length)
{
    if((message >= AVRC_UPSTREAM_BASE) && (message< AVRC_UPSTREM_MAX) && (csrBtAVRCAppStateHandler[(CsrUint16)(message- AVRC_UPSTREAM_BASE)] != NULL)&&(CsrBtAVRCAppEventFunctionsPtr !=  NULL))
    {
        csrBtAVRCAppStateHandler[(CsrUint16)(message - AVRC_UPSTREAM_BASE)]( (csr_api_avrc_prim_type)message, data,length);
    }
    else
    {
       DBG_SS_PRINT("Received CSR_AVRC primitive is unknown or private\n");
    }
}


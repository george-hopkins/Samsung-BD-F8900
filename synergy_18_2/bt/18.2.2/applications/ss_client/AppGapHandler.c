/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include "bluetooth.h"
#include "csr_bt_profiles.h"
#include "csr_bt_ss_prim.h"
#include "csr_util.h"
#include "csr_pmem.h"
#include "peer_com_linux_u2u.h"
#include "AppGapHandler.h"
#include "csr_samsung_log.h"

#include <unistd.h>
#include <dirent.h>
#include <sys/types.h> // for opendir(), readdir(), closedir()
#include <sys/stat.h> // for stat()

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


typedef struct CsrBtAppAddListStructType
{
  CsrBtDeviceAddr          deviceAddr; 
}CsrBtAppAddList; 

typedef struct CsrBtAppGapDataStructType
{
  BluetoothCsrDeviceInfo deviceInfo;
  CsrBtAppAddList          deviceAddList[MAX_APP_REGISTER_DEV];
 // CsrBool 				 isAppCSRErrorState;

}CsrBtAppGapDataType; 
static CsrBtDeviceAddr 		 deviceAddrNull; 

typedef void (* csrBtGapAppStateHandlerType)(csr_api_gap_prim_type message, void* data, CsrUint16 length);
static CsrBtAppGapEventHandlerStruct * CsrBtGapAppEventFunctionsPtr=NULL;  
static CsrBtAppGapDataType gapInstnceData;


#define PROC_DIRECTORY "/proc/"
#define CASE_SENSITIVE    1
#define CASE_INSENSITIVE  0
#define EXACT_MATCH       1
#define INEXACT_MATCH     0

////CMD

#ifdef __CSR_COUNT_WD__
typedef struct csr_hq_fault_map_struct
{
	CsrUint16 csr_wd_count;
	CsrUint16 csr_fq_fault_map[256];
}csr_hq_fault_map_t;

static csr_hq_fault_map_t g_csr_wd;

void csr_gap_init_wd_count(void)
{
	DBG_SS_PRINT("____csr_gap_init_wd_count____");

	CsrMemSet((void *)&g_csr_wd, 0x00, sizeof(g_csr_wd));
}

unsigned short csr_gap_get_wd_count(void)
{
	DBG_SS_PRINT_INT1("csr_gap_get_wd_count : csr_wd_count ", g_csr_wd.csr_wd_count);

	return ((unsigned short)(g_csr_wd.csr_wd_count));
}

#else
void csr_gap_init_wd_count(void)
{
	DBG_SS_PRINT("csr_gap_init_wd_count __CSR_COUNT_WD__ macro is not defined!!!!!");
}

unsigned short csr_gap_get_wd_count(void)
{
	DBG_SS_PRINT("csr_gap_get_wd_count __CSR_COUNT_WD__ macro is not defined!!!!!");

	return 0;
}
#endif
CsrInt16 csr_client_mgt_open(csrBtMgtErrInd ClsoeCB,CsrBtAppGapEventHandlerStruct * btGapCallbackFunctionsPtr)
{
   CsrInt16 result;

   DBG_SS_PRINT("csr_client_mgt_open");

   if(ClsoeCB && btGapCallbackFunctionsPtr)
   {
	 CsrBtGapAppEventFunctionsPtr=btGapCallbackFunctionsPtr;
	 gapInstnceData.deviceInfo.classOfDevice=CSR_BT_MAJOR_MINOR_DEVICE_CLASS;
	 gapInstnceData.deviceInfo.discoverable=FALSE;
	 gapInstnceData.deviceInfo.classOfDevice=FALSE;
	 	//return peer_client_com_init(DEFAULT_EXTERNAL_SRV_PORT,(csrBtClientCB)ClsoeCB);
    if(peer_client_com_init(DEFAULT_EXTERNAL_SRV_PORT,(csrBtClientCB)ClsoeCB))
    {
        result=csr_bt_client_app_put_message_to_tx_thread(CSR_CM_SS_ID,GAP_SS_INIT_REQ,NULL, 0 );
    }
	else
	{
	    result=CSR_ERR_CLI_NOT_CONNECT;
	}
   }
   else
   	 result=CSR_ERR_CLI_BAD_PARAM;
   
	return result;
    
}

CsrInt16 csr_client_mgt_close(void)
{
    CsrInt16 result=CSR_ERR_CLI_NOT_CONNECT;
   DBG_SS_PRINT("csr_client_mgt_close");
	if( peer_client_com_deinit())
		result=CSR_ERR_CLI_SUCESS;
	return result;
}

void csr_client_mgt_connect(void)
{
    DBG_SS_PRINT("csr_client_mgt_connect");
	peer_get_external_conect();

}


CsrInt16 csr_gap_set_scan_enable_cmd(CsrBool     disableInquiryScan, CsrBool     disablePageScand )
{
  void *data;
  CsrUint16 data_len;

  data_len=sizeof(disableInquiryScan)+sizeof(disablePageScand);
  DBG_SS_PRINT("csr_gap_scan_eable_cmd \n");
  

  data=CsrPmemAlloc(data_len);
  CsrMemCpy(data,&disableInquiryScan,sizeof(disableInquiryScan));
  CsrMemCpy(data+sizeof(disableInquiryScan),&disablePageScand,sizeof(disablePageScand));
  if(disableInquiryScan)
       gapInstnceData.deviceInfo.discoverable=FALSE;
  else
  	gapInstnceData.deviceInfo.discoverable=TRUE;
  if(disablePageScand)  	
	gapInstnceData.deviceInfo.connectable=FALSE;
  else
  	gapInstnceData.deviceInfo.connectable=TRUE;
  
  return csr_bt_client_app_put_message_to_tx_thread(CSR_CM_SS_ID,GAP_SET_SCAN_ENABLE_REQ,data, data_len );
}

CsrInt16 csr_gap_connectable_cmd(CsrBool connectAble )
{
   void *data=CsrPmemAlloc(sizeof(connectAble));
   DBG_SS_PRINT("csr_gap_connectable_cmd __\n");
   
  CsrMemCpy(data,&connectAble,sizeof(connectAble));
  if(connectAble)  	
	gapInstnceData.deviceInfo.connectable=TRUE;
  else
  	gapInstnceData.deviceInfo.connectable=FALSE;
   
  return csr_bt_client_app_put_message_to_tx_thread(CSR_CM_SS_ID,GAP_CONNECTABLE_REQ,data, sizeof(connectAble) );
}
CsrInt16 csr_gap_local_bdaddr_cmd(void )
{
   DBG_SS_PRINT("csr_gap_local_bdaddr_cmd ___\n");


  
  return  csr_bt_client_app_put_message_to_tx_thread(CSR_CM_SS_ID,GAP_LOCAL_BDADDR_REQ,NULL, 0 );
}

CsrInt16 csr_gap_local_name_cmd(void )
{
   DBG_SS_PRINT("csr_gap_local_name_cmd ___\n");

   
  return csr_bt_client_app_put_message_to_tx_thread(CSR_CM_SS_ID,GAP_LOCAL_NAME_REQ,NULL, 0 );
}


CsrInt16 csr_gap_sc_register_scdb_cmd(CsrBtScDbDeviceRecord * pRecord)
{
    CsrUint8 i;
	CsrBtDeviceLinkkey   Linkkey;
	CsrBtScDbDeviceRecord * send_data;
	CsrBool isFound=FALSE;

   DBG_SS_PRINT_ADDR("csr_gap_sc_register_scdb_cmd",pRecord->deviceAddr.nap, pRecord->deviceAddr.uap, pRecord->deviceAddr.lap);

   if(pRecord ==NULL)
   	return CSR_ERR_CLI_BAD_PARAM;
   
   if(pRecord->linkkeyValid ==FALSE)
   	return CSR_ERR_CLI_BAD_PARAM;
   

   for(i=0; i<MAX_APP_REGISTER_DEV; i++)
   {
       DBG_SS_PRINT_ADDR("csr_gap_sc_register_scdb_cmd_1", gapInstnceData.deviceAddList[i].deviceAddr.nap, gapInstnceData.deviceAddList[i].deviceAddr.uap, gapInstnceData.deviceAddList[i].deviceAddr.lap);
       if(CsrMemCmp(&(gapInstnceData.deviceAddList[i].deviceAddr), &(pRecord->deviceAddr), sizeof(CsrBtDeviceAddr))==0)
       	{
	   		isFound =TRUE;
			DBG_SS_PRINT_INT1("csr_gap_sc_register_scdb_cmd found",i);
	    	break;
       	}
   }

   if(isFound==FALSE)
   {
       for(i=0; i<MAX_APP_REGISTER_DEV; i++)
	   {
	       DBG_SS_PRINT_ADDR("csr_gap_sc_register_scdb_cmd_2", gapInstnceData.deviceAddList[i].deviceAddr.nap, gapInstnceData.deviceAddList[i].deviceAddr.uap, gapInstnceData.deviceAddList[i].deviceAddr.lap);
	       if(CsrMemCmp(&(gapInstnceData.deviceAddList[i].deviceAddr), &(deviceAddrNull),sizeof(CsrBtDeviceAddr))==0)
	       	{
		   		isFound =TRUE;
				CsrMemCpy(&(gapInstnceData.deviceAddList[i].deviceAddr), &(pRecord->deviceAddr), sizeof(CsrBtDeviceAddr));
				DBG_SS_PRINT_INT1("csr_gap_sc_register_scdb_cmd found ",i);
		    	break;
	       	}
	   }
   }
   
   if(isFound==FALSE)
   {
      	DBG_SS_PRINT("csr_gap_sc_register_scdb_cmd err");
   		return CSR_ERR_CLI_REGISTER_ERR;
   }

   	send_data=CsrPmemAlloc(sizeof(CsrBtScDbDeviceRecord));
	CsrMemCpy(send_data,pRecord,sizeof(CsrBtScDbDeviceRecord));
	
#if 1 /*convert link key*/
	for(i=0;i<SIZE_LINK_KEY; i++)
	   Linkkey[i]=send_data->Linkkey[SIZE_LINK_KEY-1-i];

	for(i=0;i<SIZE_LINK_KEY; i++)
	{
	   send_data->Linkkey[i]=Linkkey[i];
	}
#endif
	
	return csr_bt_client_app_put_message_to_tx_thread(CSR_SD_SS_ID,GAP_SD_REGISTER_SCDB_REQ,send_data, sizeof(CsrBtScDbDeviceRecord) );
}


CsrInt16 csr_gap_gap_set_local_name_cmd(   CsrBtDeviceName            localName            /* the  name of the local Bluetooth device in utf8                      */)
{
	void *data;
	DBG_SS_PRINT("csr_gap_gap_set_local_name_cmd __");
	
	data=CsrPmemAlloc(sizeof(CsrBtDeviceName));
	CsrStrCpy((CsrCharString *)data,(const CsrCharString *)localName);
    CsrStrCpy((CsrCharString *)gapInstnceData.deviceInfo.friendlyName, (const CsrCharString *)localName);
	
	return csr_bt_client_app_put_message_to_tx_thread(CSR_CM_SS_ID,GAP_SET_LOCAL_NAME_REQ,data,sizeof(CsrBtDeviceName));
}


CsrInt16 csr_gap_sc_deregister_scdb_cmd(   BD_ADDR_T            bd_addr)
{
	void *data;
	CsrUint8 i;
	CsrBool isFound=FALSE;
	DBG_SS_PRINT_ADDR("csr_gap_sc_deregister_scdb_cmd ", bd_addr.nap, bd_addr.uap, bd_addr.lap);

    for(i=0; i<MAX_APP_REGISTER_DEV; i++)
    {
       DBG_SS_PRINT_ADDR("csr_gap_sc_deregister_scdb_cmd_1", gapInstnceData.deviceAddList[i].deviceAddr.nap, gapInstnceData.deviceAddList[i].deviceAddr.uap, gapInstnceData.deviceAddList[i].deviceAddr.lap);
       if(CsrMemCmp(&(gapInstnceData.deviceAddList[i].deviceAddr), &(bd_addr), sizeof(CsrBtDeviceAddr))==0)
       	{
	   		isFound =TRUE;
			CsrMemCpy(&(gapInstnceData.deviceAddList[i].deviceAddr), &(deviceAddrNull), sizeof(CsrBtDeviceAddr));
	    	break;
       	}
    }
	
	if(isFound==FALSE)
	{
		DBG_SS_PRINT("csr_gap_sc_deregister_scdb_cmd not found");
	 return CSR_ERR_CLI_SUCESS;
	}
	data=CsrPmemAlloc(sizeof(BD_ADDR_T));
	CsrMemCpy(data,&bd_addr,sizeof(BD_ADDR_T));

	
	return csr_bt_client_app_put_message_to_tx_thread(CSR_SC_SS_ID,GAP_SC_DEREGISGER_SCDB_REQ,data,sizeof(BD_ADDR_T));
}



CsrInt16 csr_gap_sc_passkey_res_cmd(  CsrBool          accept,CsrBtDeviceAddr    theDeviceAddr,
                                        CsrUint8         thePasskeyLength,
                                        CsrUint8 *        thePasskey,
                                        CsrBool          authorised)
{
	void *data;
	
	CsrUint8 Passkey[CSR_BT_PASSKEY_MAX_LEN];

	if(thePasskeyLength>CSR_BT_PASSKEY_MAX_LEN)
		thePasskeyLength=CSR_BT_PASSKEY_MAX_LEN;
	
	if(thePasskeyLength)
		CsrMemCpy(Passkey,thePasskey,thePasskeyLength);
	
	DBG_SS_PRINT("csr_gap_sc_passkey_res_cmd __\n");

	data=CsrPmemAlloc(sizeof(accept)+sizeof(theDeviceAddr)+sizeof(thePasskeyLength)+sizeof(Passkey)+sizeof(authorised));
	CsrMemCpy(data,&accept,sizeof(accept));
	CsrMemCpy(data+sizeof(accept),&theDeviceAddr,sizeof(theDeviceAddr));
	CsrMemCpy(data+sizeof(accept)+sizeof(theDeviceAddr),&thePasskeyLength,sizeof(thePasskeyLength));
	CsrMemCpy(data+sizeof(accept)+sizeof(theDeviceAddr)+sizeof(thePasskeyLength),&Passkey,sizeof(Passkey));
	CsrMemCpy(data+sizeof(accept)+sizeof(theDeviceAddr)+sizeof(thePasskeyLength)+sizeof(Passkey),&authorised,sizeof(authorised));
	
	return csr_bt_client_app_put_message_to_tx_thread(CSR_SC_SS_ID,GAP_SC_PASSKEY_RES,data, sizeof(accept)+sizeof(theDeviceAddr)+sizeof(thePasskeyLength)+sizeof(Passkey)+sizeof(authorised));
}

CsrInt16 csr_gap_sc_justworks_res_cmd(  CsrBool  accept, CsrBtDeviceAddr deviceAddr, CsrBool authorised)
{
	void *data;
	DBG_SS_PRINT("csr_gap_sc_justworks_res_cmd __\n");

	data=CsrPmemAlloc(sizeof(accept)+sizeof(deviceAddr)+sizeof(authorised));
	CsrMemCpy(data,&accept,sizeof(accept));
	CsrMemCpy(data+sizeof(accept),&deviceAddr,sizeof(deviceAddr));
	CsrMemCpy(data+sizeof(accept)+sizeof(deviceAddr),&authorised,sizeof(authorised));
	
	return csr_bt_client_app_put_message_to_tx_thread(CSR_SC_SS_ID,GAP_SC_SSP_JUSTWORKS_RES,data, sizeof(accept)+sizeof(deviceAddr)+sizeof(authorised));
}


CsrInt16 csr_gap_sd_search_cmd( CsrUint16 maxnum, CsrUint32 searchTime , CsrUint32 bufferTime, CsrBtClassOfDevice Cod, CsrBool skipNameRequest )
{

	void *data;
	CsrUint16 data_len;
	data_len=sizeof(maxnum)+sizeof(searchTime)+sizeof(Cod)+sizeof(bufferTime)+sizeof(skipNameRequest);
	DBG_SS_PRINT("csr_gap_sd_search_cmd ");

	data=CsrPmemAlloc(data_len);
	CsrMemCpy(data,&maxnum,sizeof(maxnum));
	CsrMemCpy(data+sizeof(maxnum),&searchTime,sizeof(searchTime));
	CsrMemCpy(data+sizeof(maxnum)+sizeof(searchTime),&bufferTime,sizeof(bufferTime));
	CsrMemCpy(data+sizeof(maxnum)+sizeof(searchTime)+sizeof(bufferTime),&Cod,sizeof(Cod));
	CsrMemCpy(data+sizeof(maxnum)+sizeof(searchTime)+sizeof(bufferTime)+sizeof(Cod),&skipNameRequest,sizeof(skipNameRequest));
	
	return csr_bt_client_app_put_message_to_tx_thread(CSR_SD_SS_ID,GAP_SD_SEARCH_REQ,data, data_len );
}


CsrInt16 csr_gap_sd_search_cancel_cmd( void )
{

	DBG_SS_PRINT("csr_gap_sd_search_cancel_cmd ");
	
	
	return csr_bt_client_app_put_message_to_tx_thread(CSR_SD_SS_ID,GAP_SD_SEARCH_CANCEL_REQ,NULL, 0 );
}


CsrInt16 csr_gap_get_local_device_info(BluetoothCsrDeviceInfo  * pDeviceInfo )
{

   DBG_SS_PRINT("csr_gap_local_name_cmd ___");
   CsrMemCpy(pDeviceInfo,&( gapInstnceData.deviceInfo),sizeof(BluetoothCsrDeviceInfo));
    return CSR_ERR_CLI_SUCESS;
}


CsrInt16 csr_gap_set_dbg_level(CsrUint32 level)
{
    void *data;

    data=CsrPmemAlloc(sizeof(CsrUint32));
	CsrMemCpy(data,&level,sizeof(level));
	DBG_SS_PRINT("csr_gap_set_dbg_level __");
	csr_ss_log_set_level(level);
	
    return csr_bt_client_app_put_message_to_tx_thread(CSR_CM_SS_ID,GAP_SET_DBG_LEVEL_REQ,data, sizeof(level) );
}

CsrInt16 csr_gap_ss_reset_cmd( void )
{
	DBG_SS_PRINT("csr_gap_ss_reset_cmd __");	
	return csr_bt_client_app_put_message_to_tx_thread(CSR_CM_SS_ID,GAP_SS_RESET_REQ,NULL,0);
}

CsrInt16 csr_gap_sc_authorize_rsp_cmd( CsrBool isAuthozed , BD_ADDR_T            bd_addr)
{
	void *data;
	CsrUint16 data_len;

	data_len=sizeof(isAuthozed)+sizeof(bd_addr);
	DBG_SS_PRINT("csr_gap_sc_authorize_rsp_cmd \n");


	data=CsrPmemAlloc(data_len);
	CsrMemCpy(data,&isAuthozed,sizeof(isAuthozed));
	CsrMemCpy(data+sizeof(isAuthozed),&bd_addr,sizeof(bd_addr));

	return csr_bt_client_app_put_message_to_tx_thread(CSR_SC_SS_ID,GAP_SC_AUTHRIZE_RSP,data,data_len);
}

CsrInt16 csr_gap_write_page_timeout_cmd( CsrUint16        timeout )
{
	void *data;
	CsrUint16 data_len;

	data_len=sizeof(CsrUint16);
	DBG_SS_PRINT("csr_gap_write_page_timeout_cmd \n");


	data=CsrPmemAlloc(data_len);
	CsrMemCpy(data,&timeout,sizeof(CsrUint16));

	return csr_bt_client_app_put_message_to_tx_thread(CSR_CM_SS_ID,GAP_CM_WRITE_PAGE_TO_REQ,data,data_len);
}

CsrInt16 csr_gap_sc_encryption_req_cmd( CsrBool isEncryption , BD_ADDR_T            bd_addr)
{
	void *data;
	CsrUint16 data_len;

	data_len=sizeof(isEncryption)+sizeof(bd_addr);
	DBG_SS_PRINT("csr_gap_sc_encryption_req_cmd \n");


	data=CsrPmemAlloc(data_len);
	CsrMemCpy(data,&isEncryption,sizeof(isEncryption));
	CsrMemCpy(data+sizeof(isEncryption),&bd_addr,sizeof(bd_addr));

	return csr_bt_client_app_put_message_to_tx_thread(CSR_SC_SS_ID,GAP_SC_ENCRYPTION_REQ,data,data_len);
}

///EVENT
static void csrBtGapScPassKeyIndHandler(  csr_api_gap_prim_type message, void* data, CsrUint16 length)
{
    if(CsrBtGapAppEventFunctionsPtr->csrBtGapScPassKeyIndFunc == NULL)
    {
        DBG_SS_PRINT("csrBtGapScPassKeyIndHandler Event function not defined\n");
    }
    else
    {
       CsrBtDeviceAddr  DeviceAddr;
	   CsrBtDeviceName  DeviceName;
	    CsrUint8   Paired;
		CsrUint32   cod;
	   DBG_SS_PRINT("csrBtGapScPassKeyIndHandler __");
        CsrMemCpy(&(DeviceAddr), data, sizeof(DeviceAddr));
		CsrMemCpy(&(DeviceName), data + sizeof(DeviceAddr), sizeof(DeviceName));
		CsrMemCpy(&(Paired), data + sizeof(DeviceAddr)+ sizeof(DeviceName), sizeof(Paired));
		CsrMemCpy(&(cod), data + sizeof(DeviceAddr)+ sizeof(DeviceName)+sizeof(Paired), sizeof(cod));
        CsrBtGapAppEventFunctionsPtr->csrBtGapScPassKeyIndFunc(  DeviceAddr,  DeviceName,     Paired,cod);
    }
}

static void csrBtGapScSspJustworksIndHandler(  csr_api_gap_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtGapAppEventFunctionsPtr->csrBtGapScSspJustworksIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtGapScSspJustworksIndFunc Event function not defined\n");
    }
    else
    {
        CsrBtDeviceAddr  DeviceAddr;
	    CsrBtDeviceName  DeviceName;
	    CsrUint8   Paired;
		CsrUint32    cod;
		
	    DBG_SS_PRINT("csrBtGapScSspJustworksIndHandler __");
        CsrMemCpy(&(DeviceAddr), data, sizeof(DeviceAddr));
		CsrMemCpy(&(DeviceName), data + sizeof(DeviceAddr), sizeof(DeviceName));
		CsrMemCpy(&(Paired), data + sizeof(DeviceAddr)+ sizeof(DeviceName), sizeof(Paired));
		CsrMemCpy(&(cod), data + sizeof(DeviceAddr)+ sizeof(DeviceName)+sizeof(Paired), sizeof(cod));
        CsrBtGapAppEventFunctionsPtr->csrBtGapScSspJustworksIndFunc(  DeviceAddr,  DeviceName,     Paired,cod);
    }
}

static void csrBtGapScBondIndHandler(  csr_api_gap_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtGapAppEventFunctionsPtr->csrBtGapScBondIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtGapScBondIndHandler Event function not defined\n");
    }
    else
    {
		CsrBool				 addedToScDbList;
		CsrBtDeviceAddr		 deviceAddr;
		CsrBtResultCode   resultCode;
		CsrBtSupplier    resultSupplier;
		CsrBool isFound=FALSE;
		CsrBtScDbDeviceRecord record;
		CsrBtScDbDeviceRecord * send_record=NULL;
        CsrUint8 i;
		CsrBtDeviceLinkkey   Linkkey;
		DBG_SS_PRINT("csrBtGapScBondIndHandler");
		
		CsrMemCpy(&(addedToScDbList), data, sizeof(addedToScDbList));
		CsrMemCpy(&(deviceAddr), data + sizeof(addedToScDbList), sizeof(deviceAddr));
		CsrMemCpy(&resultCode, data + sizeof(addedToScDbList)+ sizeof(deviceAddr), sizeof(resultCode));
		CsrMemCpy(&resultSupplier, data + sizeof(addedToScDbList)+ sizeof(deviceAddr)+sizeof(resultCode), sizeof(resultSupplier));
	
		if(addedToScDbList)
		{
		
		   CsrMemCpy(&record, data + sizeof(addedToScDbList)+ sizeof(deviceAddr)+sizeof(resultCode)+sizeof(resultSupplier), sizeof(record));
#if 1			/*convert link key*/	
		   for(i=0;i<SIZE_LINK_KEY; i++)
		     Linkkey[i]=record.Linkkey[SIZE_LINK_KEY-1-i];

	       for(i=0;i<SIZE_LINK_KEY; i++)
	       	{
		      record.Linkkey[i]=Linkkey[i];
	       	}
#endif	
		   for(i=0; i<MAX_APP_REGISTER_DEV; i++)
		   {
		      
			   if(CsrMemCmp(&(gapInstnceData.deviceAddList[i].deviceAddr), &(record.deviceAddr), sizeof(CsrBtDeviceAddr))==0)
				{
					isFound =TRUE;
					DBG_SS_PRINT_ADDR("csrBtGapScBondIndHandler addr found\n", gapInstnceData.deviceAddList[i].deviceAddr.nap, gapInstnceData.deviceAddList[i].deviceAddr.uap, gapInstnceData.deviceAddList[i].deviceAddr.lap);
					break;
				}
			}
		   
		    if(isFound==FALSE)
		    {
		       	for(i=0; i<MAX_APP_REGISTER_DEV; i++)
				{
				    
				   if(CsrMemCmp(&(gapInstnceData.deviceAddList[i].deviceAddr), &(deviceAddrNull), sizeof(CsrBtDeviceAddr))==0)
					{
				
                        CsrMemCpy(&(gapInstnceData.deviceAddList[i].deviceAddr),&(deviceAddr),sizeof(deviceAddr));
						DBG_SS_PRINT_INT1("csrBtGapScBondIndHandler null addr found idx:\n",i);
						isFound=TRUE;
						break;
					}
				}
		       
		    }

			if(isFound==FALSE)
			{
			    DBG_SS_PRINT("csrBtGapScBondIndHandler not found\n");
				CsrMemCpy(&(gapInstnceData.deviceAddList[0].deviceAddr),&(deviceAddr),sizeof(deviceAddr));
			}
			
			send_record=&record;
				

		}
        CsrBtGapAppEventFunctionsPtr->csrBtGapScBondIndFunc(addedToScDbList,deviceAddr,resultCode,resultSupplier,send_record);
    }
}

static void csrBtGapScAuthorizeIndHandler(  csr_api_gap_prim_type message, void* data, CsrUint16 length)
{

	if(CsrBtGapAppEventFunctionsPtr->csrBtGapScAuthorizeIndFunc == NULL)
	{
		DBG_SS_PRINT( "csrBtGapScAuthorizeIndHandler Event function not defined\n");
	}
	else
	{
	    CsrBtDeviceAddr  DeviceAddr;
	    CsrBtDeviceName  DeviceName;
	    CsrUint16   Service;
	    DBG_SS_PRINT("csrBtGapScAuthorizeIndHandler __\n");
        CsrMemCpy(&(DeviceAddr), data, sizeof(DeviceAddr));
		CsrMemCpy(&(DeviceName), data + sizeof(DeviceAddr), sizeof(DeviceName));
		CsrMemCpy(&(Service), data + sizeof(DeviceAddr)+ sizeof(DeviceName), sizeof(Service));

		CsrBtGapAppEventFunctionsPtr->csrBtGapScAuthorizeIndFunc(  DeviceAddr,  DeviceName,     Service);
	}
}

static void csrBtGapCmLocalBdaddrCfmHandler(  csr_api_gap_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtGapAppEventFunctionsPtr->csrBtGapCmLocalBdaddrCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtGapScBondIndHandler Event function not defined\n");
    }
    else
    {
        DBG_SS_PRINT("csrBtGapCmLocalBdaddrCfmHandler __");
        CsrBtGapAppEventFunctionsPtr->csrBtGapCmLocalBdaddrCfmFunc(*((CsrBtDeviceAddr *)data));
    }
}

static void csrBtGapCmLocalNameCfmHandler(  csr_api_gap_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtGapAppEventFunctionsPtr->csrBtGapCmLocalNameCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtGapScBondIndHandler Event function not defined\n");
    }
    else
    {
         DBG_SS_PRINT("csrBtGapCmLocalNameCfmHandler __");
        CsrStrCpy((CsrCharString *)gapInstnceData.deviceInfo.friendlyName, ( const CsrCharString *)data);
        CsrBtGapAppEventFunctionsPtr->csrBtGapCmLocalNameCfmFunc( *((CsrBtDeviceName*) data));
    }
}

static void csrBtGapSDSearchIndHandler(  csr_api_gap_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtGapAppEventFunctionsPtr->csrBtGapSdSearchIndFunc == NULL)
    {
        DBG_SS_PRINT("csrBtGapSDSearchIndHandler Event function not defined");
    }
    else
    {
        GAP_SD_SEARCH_IND_T searchdata;
		CsrUint32  vidpid;

		DBG_SS_PRINT("csrBtGapSDSearchIndHandler __");

		CsrMemCpy(&(searchdata.deviceAddr), data, sizeof(CsrBtDeviceAddr));
		CsrMemCpy(&(searchdata.deviceClass), data + sizeof(CsrBtDeviceAddr), sizeof(CsrBtClassOfDevice));
		CsrMemCpy(&(searchdata.friendlyName), data + sizeof(CsrBtDeviceAddr)+ sizeof(CsrBtClassOfDevice), sizeof(CsrBtDeviceName));
		CsrMemCpy(&(vidpid), data + sizeof(CsrBtDeviceAddr)+ sizeof(CsrBtClassOfDevice)+sizeof(CsrBtDeviceName), sizeof(CsrUint32));
		CsrMemCpy(&(searchdata.rssi), data + sizeof(CsrBtDeviceAddr)+ sizeof(CsrBtClassOfDevice)+sizeof(CsrBtDeviceName)+sizeof(CsrUint32), sizeof(CsrInt8));
		searchdata.pid=(vidpid&0xffff);
		searchdata.vid=((vidpid>>16) &0xffff);
        CsrBtGapAppEventFunctionsPtr->csrBtGapSdSearchIndFunc(&searchdata);
    }
}

static void csrBtGapSDSearchCloseHandler(  csr_api_gap_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtGapAppEventFunctionsPtr->csrBtGapSdSearchCloseFunc == NULL)
    {
        DBG_SS_PRINT("csrBtGapSDSearchCloseHandler Event function not defined");
    }
    else
    {
        DBG_SS_PRINT("csrBtGapSDSearchCloseHandler __");
        CsrBtGapAppEventFunctionsPtr->csrBtGapSdSearchCloseFunc();
    }
}

static void csrBtGapChiWdCountHandler(csr_api_gap_prim_type msg, void *p_data, CsrUint16 len)
{
	CsrUint16 varId = 0;

	if(p_data && len > 0)
	{
		CsrMemCpy((void *)&varId, p_data, len);
	}

#ifdef __CSR_COUNT_WD__
	 DBG_SS_PRINT_INT1(" csrBtGapChiWdCountHandler : varId ", varId);
	 DBG_SS_PRINT_INT1(" csrBtGapChiWdCountHandler : wd_count ", g_csr_wd.csr_wd_count);

	if(g_csr_wd.csr_wd_count >= 256)
	{
		g_csr_wd.csr_wd_count = 0;
	}

	g_csr_wd.csr_fq_fault_map[g_csr_wd.csr_wd_count++] = varId;
#endif	
	if(CsrBtGapAppEventFunctionsPtr->csrBtGapChiWdCountFunc == NULL)
    {
        DBG_SS_PRINT("csrBtGapChiWdCountHandler Event function not defined");
    }
    else
    {
         DBG_SS_PRINT("csrBtGapChiWdCountHandler __");
        CsrBtGapAppEventFunctionsPtr->csrBtGapChiWdCountFunc(varId);
    }
}

static int IsNumeric(const char* ccharptr_CharacterList)
{
    for ( ; *ccharptr_CharacterList; ccharptr_CharacterList++)
        if (*ccharptr_CharacterList < '0' || *ccharptr_CharacterList > '9')
            return 0; // false
    return 1; // true
}


static int strcmp_Wrapper(const char *s1, const char *s2, int intCaseSensitive)
{
    if (intCaseSensitive)
        return !strcmp(s1, s2);
    else
        return !strcasecmp(s1, s2);
}

static int strstr_Wrapper(const char* haystack, const char* needle, int intCaseSensitive)
{
    if (intCaseSensitive)
        return (int) strstr(haystack, needle);
    else
        return (int) strcasestr(haystack, needle);
}


static pid_t GetPIDbyName_implements(const char* cchrptr_ProcessName, int intCaseSensitiveness, int intExactMatch)
{
    char chrarry_CommandLinePath[100]  ;
    char chrarry_NameOfProcess[300]  ;
    char* chrptr_StringToCompare = NULL ;
    pid_t pid_ProcessIdentifier = (pid_t) -1 ;
    struct dirent* de_DirEntity = NULL ;
    DIR* dir_proc = NULL ;

    int (*CompareFunction) (const char*, const char*, int) ;

    if (intExactMatch)
        CompareFunction = &strcmp_Wrapper;
    else
        CompareFunction = &strstr_Wrapper;


    dir_proc = opendir(PROC_DIRECTORY) ;
    if (dir_proc == NULL)
    {
        DBG_SS_PRINT("Couldn't open the " PROC_DIRECTORY " directory") ;
        return (pid_t) -2 ;
    }

    // Loop while not NULL
    while ( (de_DirEntity = readdir(dir_proc)) )
    {
        if (de_DirEntity->d_type == DT_DIR)
        {
            if (IsNumeric(de_DirEntity->d_name))
            {
                FILE* fd_CmdLineFile=NULL;
                CsrStrCpy(chrarry_CommandLinePath, PROC_DIRECTORY) ;
                CsrStrCat(chrarry_CommandLinePath, de_DirEntity->d_name) ;
                CsrStrCat(chrarry_CommandLinePath, "/cmdline") ;
                fd_CmdLineFile = fopen (chrarry_CommandLinePath, "rt") ;  // open the file for reading text
                if (fd_CmdLineFile)
                {
                    fscanf(fd_CmdLineFile, "%s", chrarry_NameOfProcess) ; // read from /proc/<NR>/cmdline
                    fclose(fd_CmdLineFile);  // close the file prior to exiting the routine

                    if (strrchr(chrarry_NameOfProcess, '/'))
                        chrptr_StringToCompare = strrchr(chrarry_NameOfProcess, '/') +1 ;
                    else
                        chrptr_StringToCompare = chrarry_NameOfProcess ;

                    if ( CompareFunction(chrptr_StringToCompare, cchrptr_ProcessName, intCaseSensitiveness) )
                    {
                        pid_ProcessIdentifier = (pid_t) atoi(de_DirEntity->d_name) ;
                        closedir(dir_proc) ;
                        return pid_ProcessIdentifier ;
                    }
                }
            }
        }
    }
    closedir(dir_proc) ;
    return pid_ProcessIdentifier ;
}

static pid_t GetPIDbyName_Wrapper(const char* cchrptr_ProcessName, ... )
{
    int intTempArgument ;
    int intInputArguments[2] ;
	
    int intInputIndex ;
    va_list argptr;
    // intInputArguments[0] = 0 ;
    // intInputArguments[1] = 0 ;
    CsrMemSet(intInputArguments, 0, sizeof(intInputArguments) ) ;

    va_start( argptr, cchrptr_ProcessName );
    for (intInputIndex = 0;  (intTempArgument = va_arg( argptr, int )) != 15; ++intInputIndex)
    {
        intInputArguments[intInputIndex] = intTempArgument ;
    }
    va_end( argptr );
    return GetPIDbyName_implements(cchrptr_ProcessName, intInputArguments[0], intInputArguments[1]);
}

#define GetPIDbyName(ProcessName,...) GetPIDbyName_Wrapper(ProcessName, ##__VA_ARGS__, (int) 15)

CsrInt16 csr_get_pid_by_name(const char* processName)
{
	CsrInt16 pid = -1;

	pid = GetPIDbyName(processName);
	DBG_SS_PRINT_INT1("____csr_get_pid_by_name____  pid ", pid);
	return pid;
}
static void csrBtGapDbgLevelHandler(  csr_api_gap_prim_type message, void* data, CsrUint16 length)
{
    CsrUint32 dbgLevel;
	
   	if(data && length > 0)
	{
	 
		CsrMemCpy((void *)&dbgLevel, data, sizeof(dbgLevel));
		DBG_SS_PRINT_INT1(" csrBtGapDbgLevelHandler",dbgLevel);
		csr_ss_log_set_level(dbgLevel);
	}


	if(CsrBtGapAppEventFunctionsPtr->csrBtGapGetDbgLevelIndFunc == NULL)
    {
        DBG_SS_PRINT("csrBtGapDbgLevelHandler Event function not defined");
    }
    else
    {
        DBG_SS_PRINT("csrBtGapDbgLevelHandler __");
        CsrBtGapAppEventFunctionsPtr->csrBtGapGetDbgLevelIndFunc(dbgLevel);
    }
}

static void csrBtGapErrorHandler(  csr_api_gap_prim_type message, void* data, CsrUint16 length)
{
	csr_bt_client_app_set_error_state(TRUE);
    if(CsrBtGapAppEventFunctionsPtr->csrBtGapGetErrorIndFunc == NULL)
    {
        DBG_SS_PRINT("csrBtGapErrorHandler Event function not defined");
    }
    else
    {
         DBG_SS_PRINT("csrBtGapErrorHandler __");
        CsrBtGapAppEventFunctionsPtr->csrBtGapGetErrorIndFunc();
    }
}


static void csrBtGapBtInitIndHandler(  csr_api_gap_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtGapAppEventFunctionsPtr->csrBtGapBtInitIndFunc == NULL)
    {
        DBG_SS_PRINT("csrBtGapBtInitIndHandler Event function not defined");
    }
    else
    {
         DBG_SS_PRINT("csrBtGapBtInitIndHandler __");
        CsrBtGapAppEventFunctionsPtr->csrBtGapBtInitIndFunc();
    }
}

static void csrBtGapScDeregisterScdbCfmHandler(  csr_api_gap_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtGapAppEventFunctionsPtr->csrBtGapScDeregisterScdbCfmFunc == NULL)
    {
        DBG_SS_PRINT("csrBtGapScDeregisterScdbCfmHandler Event function not defined");
    }
    else
    {
		CsrBtDeviceAddr 		deviceAddr;
		CsrBtResultCode   resultCode;
		CsrBtSupplier    resultSupplier;

	    DBG_SS_PRINT("csrBtGapScDeregisterScdbCfmHandler __\n");
        CsrMemCpy(&(deviceAddr), data, sizeof(CsrBtDeviceAddr));
		CsrMemCpy(&(resultCode), data + sizeof(CsrBtDeviceAddr), sizeof(CsrBtResultCode));
		CsrMemCpy(&(resultSupplier), data + sizeof(CsrBtDeviceAddr)+sizeof(CsrBtResultCode), sizeof(CsrBtSupplier));


        CsrBtGapAppEventFunctionsPtr->csrBtGapScDeregisterScdbCfmFunc(deviceAddr,resultCode,resultSupplier);
    }
}

static void csrBtGapScEncryptionCfmHandler(  csr_api_gap_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtGapAppEventFunctionsPtr->csrBtScEncryptionCfmFunc == NULL)
    {
        DBG_SS_PRINT("csrBtGapScEncryptionCfmHandler Event function not defined");
    }
    else
    {
		CsrBtDeviceAddr		   deviceAddr;
		CsrBtResultCode		  resultCode;
		CsrBtSupplier			 resultSupplier;

	    DBG_SS_PRINT("csrBtGapScEncryptionCfmHandler __\n");
        CsrMemCpy(&(deviceAddr), data, sizeof(CsrBtDeviceAddr));
		CsrMemCpy(&(resultCode), data + sizeof(CsrBtDeviceAddr), sizeof(CsrBtResultCode));
		CsrMemCpy(&(resultSupplier), data + sizeof(CsrBtDeviceAddr)+sizeof(CsrBtResultCode), sizeof(CsrBtSupplier));


        CsrBtGapAppEventFunctionsPtr->csrBtScEncryptionCfmFunc(deviceAddr,resultCode,resultSupplier);
    }
}


static const csrBtGapAppStateHandlerType csrBtGapAppStateHandler[GAP_UPSTREAM_CNT] =
{
    csrBtGapScPassKeyIndHandler,                      /* GAP_SC_PASSKEY_IND */
    csrBtGapScSspJustworksIndHandler,                 /* GAP_SC_SSP_JUSTWORS_IND */
    csrBtGapScBondIndHandler,                         /*GAP_SC_BOND_IND*/
    csrBtGapScAuthorizeIndHandler,                    /*GAP_SC_ATHORIZE_IND*/
    csrBtGapCmLocalBdaddrCfmHandler,                   /*GAP_LOCAL_BDADDR_CFM*/
    csrBtGapCmLocalNameCfmHandler,                     /*GAP_LOCAL_NAME_CFM*/
    csrBtGapSDSearchIndHandler,                       /*GAP_SD_SEARCH_IND*/
	csrBtGapSDSearchCloseHandler,                     /*GAP_SD_SEARCH_CLOSE*/
    csrBtGapChiWdCountHandler,				           /*GAP_CHIP_WD_COUNT */
    csrBtGapDbgLevelHandler,                           /*GAP_GET_DBG_LEVEL_IND*/
    csrBtGapErrorHandler,                              /*GAP_ERROR_IND*/
    csrBtGapBtInitIndHandler,                           /*GAP_BT_INIT_IND*/
    csrBtGapScDeregisterScdbCfmHandler,                 /*GAP_SC_DEREGISGER_SCDB_CFM*/
    csrBtGapScEncryptionCfmHandler						/*GAP_SC_ENCRYPTION_CFM*/
};

void CsrBtGapAppEventHandler(  CsrUint16 message, void* data, CsrUint16 length)
{
    if((message >= GAP_UPSTREAM_BASE) && (message< GAP_UPSTREM_MAX) && (csrBtGapAppStateHandler[(CsrUint16)(message- GAP_UPSTREAM_BASE)] != NULL)&&(CsrBtGapAppEventFunctionsPtr !=NULL))
    {
        csrBtGapAppStateHandler[(CsrUint16)(message - GAP_UPSTREAM_BASE)]( (csr_api_gap_prim_type)message, data,length);
    }
    else
    {
       DBG_SS_PRINT("Received CSR_AV primitive is unknown or private\n");
    }
}


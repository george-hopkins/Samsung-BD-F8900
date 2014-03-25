/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include <stdio.h>
#include "csr_types.h"
#include "csr_bt_hidh_prim.h"
#include "csr_bt_hidh_lib.h"
#include "AppHIDHlib.h" 
#include "csr_bt_ss_prim.h"
#include "peer_com_linux_u2u.h"
#include "csr_pmem.h"


void CsrAppServerHidhConnetInd(CsrBtDeviceAddr * pBdaddr)
{
    CsrUint8 *data = (CsrUint8 *)CsrPmemAlloc(sizeof(CsrBtDeviceAddr));
	CsrMemCpy(data, pBdaddr,  sizeof(CsrBtDeviceAddr));
	csr_bt_server_app_put_message_to_tx_thread(CSR_HIDH_SS_ID, HIDH_CONNECT_IND,data,sizeof(CsrBtDeviceAddr));
}


void CsrAppServerHidhActivateCfm(void)
{
   csr_bt_server_app_put_message_to_tx_thread(CSR_HIDH_SS_ID, HIDH_ACTIVATATE_CFM,NULL,0);
}

void CsrAppServerHidhDeActivateCfm(void)
{
   csr_bt_server_app_put_message_to_tx_thread(CSR_HIDH_SS_ID, HIDH_DEACTIVATATE_CFM,NULL,0);
}

void CsrAppServerHidhConnetCfm( CsrBtResultCode       resultCode, CsrBtSupplier   resultSupplier , CsrAppBtHidhSdpInfoData * infodata)
{
	void *server_data;
	void *server_data_prt;
	CsrUint16 server_data_len;
	CsrUint16 serviceName_len=0;
	CsrUint8 charData='\0'; //null char
	if(infodata->serviceName)
	   serviceName_len=CsrStrLen(infodata->serviceName);
	
	server_data_len=sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier)+sizeof(BD_ADDR_T)+sizeof(CsrBtHidhSdpInfo)+serviceName_len+ sizeof(CsrUint8)+sizeof(CsrUint16)+infodata->descriptorLength;
	server_data=	CsrPmemAlloc(server_data_len);
	server_data_prt=server_data;
	CsrMemCpy(server_data_prt, &resultCode, sizeof(CsrBtResultCode));
	server_data_prt+=sizeof(CsrBtResultCode);
	CsrMemCpy(server_data_prt, &resultSupplier, sizeof(CsrBtSupplier));
	server_data_prt+=sizeof(CsrBtSupplier);
	CsrMemCpy(server_data_prt, &infodata->deviceAddr, sizeof(CsrBtDeviceAddr));
	server_data_prt+=sizeof(CsrBtDeviceAddr);
	if(infodata->sdpInfo)	
	{
	    CsrMemCpy(server_data_prt, infodata->sdpInfo, sizeof(CsrBtHidhSdpInfo));
	}
	else
		CsrMemSet(server_data_prt, 0x00, sizeof(CsrBtHidhSdpInfo));
	server_data_prt+=sizeof(CsrBtHidhSdpInfo);
	
	if(serviceName_len)
		CsrMemCpy(server_data_prt, infodata->serviceName, serviceName_len);
	server_data_prt+=serviceName_len;

	
	CsrMemCpy(server_data_prt, &charData, sizeof(CsrUint8));
	server_data_prt+=sizeof(CsrUint8);

	CsrMemCpy(server_data_prt, &infodata->descriptorLength, sizeof(CsrUint16));

	server_data_prt+=sizeof(CsrUint16);
	
	if(infodata->descriptorLength)
		CsrMemCpy(server_data_prt, infodata->descriptor, infodata->descriptorLength);
	
	
	csr_bt_server_app_put_message_to_tx_thread(CSR_HIDH_SS_ID, HIDH_CONNECT_CFM,server_data,server_data_len);
}


void CsrAppServerHidhConectAccectCfm( BD_ADDR_T * pBdaddr,    CsrBtResultCode        avResultCode, CsrBtSupplier          avResultSupplier)
{
	void *server_data = CsrPmemAlloc(sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier));
	CsrMemCpy(server_data, pBdaddr, sizeof(BD_ADDR_T));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T), &avResultCode, sizeof(CsrBtResultCode));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode), &avResultSupplier, sizeof(CsrBtSupplier));
	
	csr_bt_server_app_put_message_to_tx_thread(CSR_HIDH_SS_ID, HIDH_CONNECT_ACCEPT_CFM,server_data,sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier));
}

	


void CsrAppServerHidhDisconnectInd(BD_ADDR_T * pBdaddr,CsrBtResultCode reasonCode,CsrBtSupplier reasonSupplier)
{

	void *server_data = CsrPmemAlloc(sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier));
	CsrMemCpy(server_data, pBdaddr, sizeof(BD_ADDR_T));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T), &reasonCode, sizeof(CsrBtResultCode));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode), &reasonSupplier, sizeof(CsrBtSupplier));
	csr_bt_server_app_put_message_to_tx_thread(CSR_HIDH_SS_ID, HIDH_DISCONNECT_IND,server_data,sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier));

}


void CsrAppServerHidhDisconnectCfm(BD_ADDR_T * pBdaddr,CsrBtResultCode reasonCode,CsrBtSupplier reasonSupplier)
{

	void *server_data = CsrPmemAlloc(sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier));
	CsrMemCpy(server_data, pBdaddr, sizeof(BD_ADDR_T));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T), &reasonCode, sizeof(CsrBtResultCode));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode), &reasonSupplier, sizeof(CsrBtSupplier));
	csr_bt_server_app_put_message_to_tx_thread(CSR_HIDH_SS_ID, HIDH_DISCONNECT_CFM,server_data,sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier));

}
void CsrAppServerHidhDataInd(CsrUint16              dataLen, CsrUint8               *data)
{
   void *server_data=NULL;
   
   if(dataLen)
   {
   	 server_data = CsrPmemAlloc(dataLen);
     CsrMemCpy(server_data, data, dataLen);
   }
   csr_bt_server_app_put_message_to_tx_thread(CSR_HIDH_SS_ID, HIDH_DATA_IND,server_data,dataLen);
}

void CsrAppServerHidhStatusInd(CsrBtHidhStatus         status)
{
	CsrUint8 *data = (CsrUint8 *)CsrPmemAlloc(sizeof(CsrBtHidhStatus));
	CsrMemCpy(data, &status,  sizeof(CsrBtHidhStatus));
	csr_bt_server_app_put_message_to_tx_thread(CSR_HIDH_SS_ID, HIDH_STATUS_IND,data,sizeof(CsrBtHidhStatus));
	
}



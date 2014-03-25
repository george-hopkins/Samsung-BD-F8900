/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include <stdio.h>
#include "csr_types.h"
#include "csr_bt_avrcp_prim.h"
#include "csr_bt_avrcp_lib.h"
#include "csr_bt_ss_prim.h"
#include "peer_com_linux_u2u.h"
#include "csr_pmem.h"



void CsrAppServerAVRCCloseInd(void)
{
   csr_bt_server_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID, AVRC_CLOSE_IND,NULL,0);
}

void CsrAppServerAVRCConnetInd(CsrBtDeviceAddr * pBdaddr)
{
    CsrUint8 *data = (CsrUint8 *)CsrPmemAlloc(sizeof(CsrBtDeviceAddr));
	CsrMemCpy(data, pBdaddr,  sizeof(CsrBtDeviceAddr));
	csr_bt_server_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID, AVRC_CONNECT_IND,data,sizeof(CsrBtDeviceAddr));
}

void CsrAppServerAVRCRemoteFeatureInd(CsrUint16  feature)
{
    CsrUint8 *data = (CsrUint8 *)CsrPmemAlloc(sizeof(feature));
	CsrMemCpy(data, &feature,  sizeof(feature));
	csr_bt_server_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID, AVRC_REMOTE_FEATURES_IND,data,sizeof(feature));
}

void CsrAppServerAVRCActivateCfm(void)
{
   csr_bt_server_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID, AVRC_ACTIVATATE_CFM,NULL,0);
}

void CsrAppServerAVRCDeActivateCfm(void)
{
   csr_bt_server_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID, AVRC_DEACTIVATATE_CFM,NULL,0);
}


void CsrAppServerAVRCCtPassThroughCfm(CsrBtAvrcpPTOpId operationId, CsrBtResultCode             resultCode,CsrBtSupplier          avResultSupplier)
{
   void *data = (CsrUint8 *)CsrPmemAlloc(sizeof(operationId)+sizeof(resultCode)+sizeof(avResultSupplier));
   CsrMemCpy(data,&operationId,sizeof(operationId));
   CsrMemCpy(data+sizeof(operationId), &resultCode, sizeof(resultCode));
   CsrMemCpy(data+sizeof(operationId)+sizeof(resultCode), &avResultSupplier, sizeof(avResultSupplier));
   csr_bt_server_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID, AVRC_CT_PASS_THROUGH_CFM,data,sizeof(operationId)+sizeof(resultCode)+sizeof(avResultSupplier));
}

void CsrAppServerAVRCTgPassThroughInd(CsrBtAvrcpPTOpId operationId, CsrBtAvrcpPTState             state)
{
   void *data = (CsrUint8 *)CsrPmemAlloc(sizeof(operationId)+sizeof(state));
   CsrMemCpy(data,&operationId,sizeof(operationId));
   CsrMemCpy(data+sizeof(operationId), &state, sizeof(state));
   csr_bt_server_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID, AVRC_TG_PASS_THROUGH_IND,data,sizeof(operationId)+sizeof(state));
}

void CsrAppServerAVRCConnetCfm( BD_ADDR_T * pBdaddr,    CsrBtResultCode        avResultCode, CsrBtSupplier          avResultSupplier)
{
	void *server_data = CsrPmemAlloc(sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier));
	CsrMemCpy(server_data, pBdaddr, sizeof(BD_ADDR_T));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T), &avResultCode, sizeof(CsrBtResultCode));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode), &avResultSupplier, sizeof(CsrBtSupplier));

	
	csr_bt_server_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID, AVRC_CONNECT_CFM,server_data,sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier));
}


void CsrAppServerAVRCDisconectInd(BD_ADDR_T * pBdaddr,CsrBtReasonCode reasonCode,CsrBtSupplier reasonSupplier)
{

	void *server_data = CsrPmemAlloc(sizeof(BD_ADDR_T)+sizeof(CsrBtReasonCode)+sizeof(CsrBtSupplier));
	CsrMemCpy(server_data, pBdaddr, sizeof(BD_ADDR_T));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T), &reasonCode, sizeof(CsrUint16));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T)+sizeof(CsrUint16), &reasonSupplier, sizeof(CsrUint16));
//	CsrMemCpy(server_data+sizeof(BD_ADDR_T)+sizeof(CsrUint16)+sizeof(CsrUint16), &localTerminated, sizeof(CsrBool));
	csr_bt_server_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID, AVRC_DISCONNECT_IND,server_data,sizeof(BD_ADDR_T)+sizeof(CsrBtReasonCode)+sizeof(CsrBtSupplier));

}

void CsrAppServerAVRCDisconectCfm(BD_ADDR_T * pBdaddr,CsrUint16 reasonCode,CsrUint16 reasonSupplier)
{

	void *server_data = CsrPmemAlloc(sizeof(BD_ADDR_T)+sizeof(CsrBtReasonCode)+sizeof(CsrBtSupplier));
	CsrMemCpy(server_data, pBdaddr, sizeof(BD_ADDR_T));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T), &reasonCode, sizeof(CsrUint16));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T)+sizeof(CsrUint16), &reasonSupplier, sizeof(CsrUint16));
//	CsrMemCpy(server_data+sizeof(BD_ADDR_T)+sizeof(CsrUint16)+sizeof(CsrUint16), &localTerminated, sizeof(CsrBool));
	csr_bt_server_app_put_message_to_tx_thread(CSR_AVRCP_SS_ID, AVRC_DISCONNECT_CFM,server_data,sizeof(BD_ADDR_T)+sizeof(CsrBtReasonCode)+sizeof(CsrBtSupplier));

}




/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include <stdio.h>
#include "csr_types.h"
#include "csr_bt_av_prim.h"
#include "csr_bt_av_lib.h"
#include "csr_bt_ss_prim.h"
#include "peer_com_linux_u2u.h"
#include "csr_pmem.h"




void CsrAppServerAvConnetInd(BD_ADDR_T * pBdaddr)
{
	CsrUint8 *server_data = (CsrUint8 *)CsrPmemAlloc(sizeof(BD_ADDR_T));
	CsrMemCpy(server_data,pBdaddr,sizeof(BD_ADDR_T));
	csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_CONNECT_IND,server_data,sizeof(BD_ADDR_T));
}

void CsrAppServerAvConfigInd(CsrUint16 samplerate, CsrUint8 pcm_channels)
{
	CsrUint8 *server_data_pcm_format = (CsrUint8 *)CsrPmemAlloc(sizeof(samplerate) + sizeof(pcm_channels) );
	CsrMemCpy(server_data_pcm_format, &(samplerate), sizeof(samplerate));
	CsrMemCpy((server_data_pcm_format + sizeof(samplerate)), &(pcm_channels), sizeof(pcm_channels));

	csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_CONFIG_IND, (void *)server_data_pcm_format, sizeof(samplerate) + sizeof(pcm_channels));

}

void CsrAppServerAvConfigCfm(CsrUint16 samplerate, CsrUint8 pcm_channels)
{
	CsrUint8 *server_data_pcm_format = (CsrUint8 *)CsrPmemAlloc(sizeof(samplerate) + sizeof(pcm_channels) );
	CsrMemCpy(server_data_pcm_format, &(samplerate), sizeof(samplerate));
	CsrMemCpy((server_data_pcm_format + sizeof(samplerate)), &(pcm_channels), sizeof(pcm_channels));

	csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_CONFIG_CFM, (void *)server_data_pcm_format, sizeof(samplerate) + sizeof(pcm_channels));

}


void CsrAppServerAvActivateCfm(void)
{
   csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_ACTIVATATE_CFM,NULL,0);
}

void CsrAppServerAvDeactivateCfm(void)
{
   csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_DEACTIVATATE_CFM,NULL,0);
}


void CsrAppServerAvStreamDataInd(void *data,CsrUint16 length )
{
   csr_bt_server_app_put_message_to_tx_thread((CsrUint16)CSR_AV_SS_ID, (CsrUint16)AV_PCM_STREAM_DATA_IND, data,length);
}

void CsrAppServerCloseInd(void)
{
 
   csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_CLOSE_IND,NULL,0);
}

void CsrAppServerCloseCfm(CsrBool result)
{
	void *server_data = (void *)CsrPmemAlloc(sizeof(result) );
	CsrMemCpy(server_data,&result,sizeof(result));
	csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_CLOSE_CFM,server_data,sizeof(result));
}


void CsrAppServerOpenInd( CsrBool ss_filter_support)
{
	CsrUint8 *server_data_ss_filter_support = (CsrBool *)CsrPmemAlloc(sizeof(ss_filter_support) );
	CsrMemCpy(server_data_ss_filter_support, &(ss_filter_support), sizeof(ss_filter_support));
	csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_OPEN_IND,server_data_ss_filter_support,sizeof(ss_filter_support));

}

void CsrAppServerOpenCfm( CsrBool ss_filter_support)
{
	CsrUint8 *server_data_ss_filter_support = (CsrBool *)CsrPmemAlloc(sizeof(ss_filter_support) );
	CsrMemCpy(server_data_ss_filter_support, &(ss_filter_support), sizeof(ss_filter_support));
	csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_OPEN_CFM,server_data_ss_filter_support,sizeof(ss_filter_support));

}



void CsrAppServerAvConnetCfm( BD_ADDR_T * pBdaddr,    CsrBtResultCode        avResultCode, CsrBtSupplier          avResultSupplier)
{
	void *server_data = CsrPmemAlloc(sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier));
	CsrMemCpy(server_data, pBdaddr, sizeof(BD_ADDR_T));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T), &avResultCode, sizeof(CsrBtResultCode));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode), &avResultSupplier, sizeof(CsrBtSupplier));
	
	csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_CONNECT_CFM,server_data,sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier));
}


void CsrAppServerAvDisconnectInd(BD_ADDR_T * pBdaddr,CsrUint16 reasonCode,CsrUint16 reasonSupplier)
{

	void *server_data = CsrPmemAlloc(sizeof(BD_ADDR_T)+sizeof(CsrUint16)+sizeof(CsrUint16));
	CsrMemCpy(server_data, pBdaddr, sizeof(BD_ADDR_T));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T), &reasonCode, sizeof(CsrUint16));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T)+sizeof(CsrUint16), &reasonSupplier, sizeof(CsrUint16));
	csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_DISCONNECT_IND,server_data,sizeof(BD_ADDR_T)+sizeof(CsrUint16)+sizeof(CsrUint16));

}

void CsrAppServerAvDisconnectCfm(BD_ADDR_T * pBdaddr,CsrUint16 reasonCode,CsrUint16 reasonSupplier)
{

	void *server_data = CsrPmemAlloc(sizeof(BD_ADDR_T)+sizeof(CsrUint16)+sizeof(CsrUint16));
	CsrMemCpy(server_data, pBdaddr, sizeof(BD_ADDR_T));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T), &reasonCode, sizeof(CsrUint16));
	CsrMemCpy(server_data+sizeof(BD_ADDR_T)+sizeof(CsrUint16), &reasonSupplier, sizeof(CsrUint16));
	csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_DISCONNECT_CFM,server_data,sizeof(BD_ADDR_T)+sizeof(CsrUint16)+sizeof(CsrUint16));

}


void CsrAppServerAvStartInd(void)
{
    csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_START_IND,NULL,0);
}

void CsrAppServerAvSuspendInd(void)
{
    csr_bt_server_app_put_message_to_tx_thread(CSR_AV_SS_ID, AV_SUSPEND_IND,NULL,0);
}




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
#include "csr_bt_sc_db.h"
#include "csr_samsung_log.h"



void CsrAppBtScPassKeyInd(CsrBtDeviceAddr *      pDeviceAddr, CsrBtDeviceName   *   pDeviceName,  CsrUint8    *   pPaired, CsrUint32   cod)
{
    void * data;
   	CsrUint16 datalen=sizeof(CsrBtDeviceAddr)+sizeof(CsrBtDeviceName)+sizeof(CsrUint8)+sizeof(CsrUint32);
   	
    data = (CsrUint8 *)CsrPmemAlloc(datalen );
	CsrMemCpy(data, pDeviceAddr, sizeof(CsrBtDeviceAddr));
	CsrMemCpy((data + sizeof(CsrBtDeviceAddr)), pDeviceName, sizeof(CsrBtDeviceName));
	CsrMemCpy((data + sizeof(CsrBtDeviceAddr)+sizeof(CsrBtDeviceName)), pPaired, sizeof(CsrUint8));
	CsrMemCpy((data + sizeof(CsrBtDeviceAddr)+sizeof(CsrBtDeviceName)+ sizeof(CsrUint8)), &cod, sizeof(CsrUint32));
	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "CsrAppBtScPassKeyInd "));
	csr_bt_server_app_put_message_to_tx_thread(CSR_SC_SS_ID, GAP_SC_PASSKEY_IND,data,datalen);
}

void CsrAppBtScAuthorizeInd(CsrBtDeviceAddr *      pDeviceAddr, CsrBtDeviceName   *   pDeviceName,  CsrUint16    *   pService)
{
    void * data;
	CsrUint16 datalen=sizeof(CsrBtDeviceAddr)+sizeof(CsrBtDeviceName)+sizeof(CsrUint16);
	
	data = (CsrUint8 *)CsrPmemAlloc(datalen );
	CsrMemCpy(data, pDeviceAddr, sizeof(CsrBtDeviceAddr));
	CsrMemCpy((data + sizeof(CsrBtDeviceAddr)), pDeviceName, sizeof(CsrBtDeviceName));
	CsrMemCpy((data + sizeof(CsrBtDeviceAddr)+sizeof(CsrBtDeviceName)), pService, sizeof(CsrUint16));
	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "CsrAppBtScAuthorizeInd "));
	csr_bt_server_app_put_message_to_tx_thread(CSR_SC_SS_ID, GAP_SC_ATHORIZE_IND,data,datalen);
}



void CsrAppBtScSspJustworksInd(CsrBtDeviceAddr *      pDeviceAddr, CsrBtDeviceName   *   pDeviceName,  CsrUint8    *   pPaired, CsrUint32    cod)
{
    void * data;
	CsrUint16 datalen=sizeof(CsrBtDeviceAddr)+sizeof(CsrBtDeviceName)+sizeof(CsrUint8)+sizeof(CsrUint32);
	
	data = (CsrUint8 *)CsrPmemAlloc(datalen );
	CsrMemCpy(data, pDeviceAddr, sizeof(CsrBtDeviceAddr));
	CsrMemCpy((data + sizeof(CsrBtDeviceAddr)), pDeviceName, sizeof(CsrBtDeviceName));
	CsrMemCpy((data + sizeof(CsrBtDeviceAddr)+sizeof(CsrBtDeviceName)), pPaired, sizeof(CsrUint8));
	CsrMemCpy((data + sizeof(CsrBtDeviceAddr)+sizeof(CsrBtDeviceName))+sizeof(CsrUint8), &cod, sizeof(CsrUint32));
	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "CsrAppBtScSspJustworksInd "));
	csr_bt_server_app_put_message_to_tx_thread(CSR_SC_SS_ID, GAP_SC_SSP_JUSTWORS_IND,data,datalen);
}


void CsrAppBtScBondInd(CsrBool * pAddedToScDbList,  CsrBtDeviceAddr * pDeviceAddr,  CsrBtResultCode   resultCode,CsrBtSupplier    resultSupplier,CsrBtScDbDeviceRecord *pTheRecord)
{
    void * data;
	CsrUint16 datalen=sizeof(CsrBool)+sizeof(CsrBtDeviceAddr)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier);

	if(*pAddedToScDbList)
		datalen+=sizeof(CsrBtScDbDeviceRecord);

	data = (CsrUint8 *)CsrPmemAlloc(datalen );
	CsrMemCpy(data, pAddedToScDbList, sizeof(CsrBool));
	CsrMemCpy((data + sizeof(CsrBool)), pDeviceAddr, sizeof(CsrBtDeviceAddr));
	CsrMemCpy((data + sizeof(CsrBool)+sizeof(CsrBtDeviceAddr)), &resultCode, sizeof(CsrBtResultCode));
	CsrMemCpy((data + sizeof(CsrBool)+sizeof(CsrBtDeviceAddr)+sizeof(CsrBtResultCode)), &resultSupplier, sizeof(CsrBtSupplier));
	
	if(*pAddedToScDbList)
		CsrMemCpy((data + sizeof(CsrBool)+sizeof(CsrBtDeviceAddr)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier)), pTheRecord, sizeof(CsrBtScDbDeviceRecord));
	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "CsrAppBtScBondInd "));
	csr_bt_server_app_put_message_to_tx_thread(CSR_SC_SS_ID, GAP_SC_BOND_IND,data,datalen);
}


void CsrAppBtScDeBondCfm(CsrBtDeviceAddr         deviceAddr,CsrBtResultCode   resultCode,CsrBtSupplier    resultSupplier)
{
    void * data;
	CsrUint16 datalen=sizeof(CsrBtDeviceAddr)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier);

	data = (CsrUint8 *)CsrPmemAlloc(datalen );
	CsrMemCpy(data, &deviceAddr, sizeof(CsrBtDeviceAddr));
	CsrMemCpy((data + sizeof(CsrBtDeviceAddr)), &resultCode, sizeof(CsrBtResultCode));
	CsrMemCpy((data + sizeof(CsrBtDeviceAddr))+sizeof(CsrBtResultCode), &resultSupplier, sizeof(CsrBtSupplier));
	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "CsrAppBtScDeBondCfm "));

	csr_bt_server_app_put_message_to_tx_thread(CSR_SC_SS_ID, GAP_SC_DEREGISGER_SCDB_CFM,data,datalen);
}


void CsrAppBtGapCmLocalBdaddrCfm(CsrBtDeviceAddr * pDeviceAddr)
{
    void * data;
	CsrUint16 datalen=sizeof(CsrBtDeviceAddr);
	
	data = (CsrUint8 *)CsrPmemAlloc(datalen );
	CsrMemCpy(data , pDeviceAddr, sizeof(CsrBtDeviceAddr));
	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "CsrAppBtGapCmLocalBdaddrCfm "));
	csr_bt_server_app_put_message_to_tx_thread(CSR_CM_SS_ID, GAP_LOCAL_BDADDR_CFM,data,datalen);
}


void CsrAppBtGapCmLocalNameCfm(CsrBtDeviceName * pname)
{
    void * data;
	
	data = (CsrUint8 *)CsrPmemAlloc(sizeof(CsrBtDeviceName) );
	CsrMemCpy(data , pname, sizeof(CsrBtDeviceName));
	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "CsrAppBtGapCmLocalNameCfm "));
	csr_bt_server_app_put_message_to_tx_thread(CSR_CM_SS_ID, GAP_LOCAL_NAME_CFM,data,sizeof(CsrBtDeviceName));
}

void CsrAppBtScEncryptionCfm(CsrBtDeviceAddr         deviceAddr,    CsrBtResultCode         resultCode,CsrBtSupplier           resultSupplier)
{
    void * data;
	CsrUint16 datalen=sizeof(CsrBtDeviceAddr)+sizeof(CsrBtResultCode)+sizeof(CsrBtSupplier);

	data = (CsrUint8 *)CsrPmemAlloc(datalen );
	CsrMemCpy(data, &deviceAddr, sizeof(CsrBtDeviceAddr));
	CsrMemCpy((data + sizeof(CsrBtDeviceAddr)), &resultCode, sizeof(CsrBtResultCode));
	CsrMemCpy((data + sizeof(CsrBtDeviceAddr))+sizeof(CsrBtResultCode), &resultSupplier, sizeof(CsrBtSupplier));
	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "CsrAppBtScEncryptionCfm "));

	csr_bt_server_app_put_message_to_tx_thread(CSR_SC_SS_ID, GAP_SC_ENCRYPTION_CFM,data,datalen);
}



void CsrAppBtGapSdSearchInd(CsrBtDeviceAddr * pdeviceAddr,CsrBtClassOfDevice deviceClass, CsrBtDeviceName * friendlyName, CsrUint32 vidpid,CsrInt8 rssi)
{
    void * data;
	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "CsrAppBtGapSdSearchInd"));

	data = (CsrUint8 *)CsrPmemAlloc(sizeof(CsrBtDeviceAddr)+sizeof(CsrBtClassOfDevice)+sizeof(CsrBtDeviceName)+sizeof(CsrUint32) +sizeof(CsrInt8));

	CsrMemCpy(data , pdeviceAddr, sizeof(CsrBtDeviceAddr));
	CsrMemCpy(data+sizeof(CsrBtDeviceAddr) , &deviceClass, sizeof(CsrBtClassOfDevice));
	CsrMemCpy((data+sizeof(CsrBtDeviceAddr)+sizeof(CsrBtClassOfDevice)) , friendlyName, sizeof(CsrBtDeviceName));
	CsrMemCpy((data+sizeof(CsrBtDeviceAddr)+sizeof(CsrBtClassOfDevice)+sizeof(CsrBtDeviceName)) , &vidpid, sizeof(CsrUint32));
	CsrMemCpy((data+sizeof(CsrBtDeviceAddr)+sizeof(CsrBtClassOfDevice)+sizeof(CsrBtDeviceName)+sizeof(CsrUint32)) , &rssi, sizeof(CsrInt8));
	csr_bt_server_app_put_message_to_tx_thread(CSR_SD_SS_ID, GAP_SD_SEARCH_IND,data,sizeof(CsrBtDeviceAddr)+sizeof(CsrBtClassOfDevice)+sizeof(CsrBtDeviceName)+sizeof(CsrUint32) +sizeof(CsrInt8));
}


void CsrAppBtGapSdSearchClose()
{
   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "CsrAppBtGapSdSearchClose "));
	csr_bt_server_app_put_message_to_tx_thread(CSR_SD_SS_ID, GAP_SD_SEARCH_CLOSE,NULL,0);
}


#ifdef __CSR_COUNT_WD__
void CsrAppBtGapChipWdCount(CsrUint16 varId)
{
	CsrUint16 *p_data = (CsrUint16 *)CsrPmemAlloc(sizeof(CsrUint16));

	*p_data = varId;

	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "CsrAppBtGapChipWdCount : varId<0x%04x>\n", varId));
	csr_bt_server_app_put_message_to_tx_thread(CSR_CM_SS_ID, GAP_CHIP_WD_COUNT, p_data, sizeof(CsrUint16));
}
#endif



void CsrAppBtGapGetDbgLevel(CsrUint32 logLevel)
{
   	CsrUint16 *p_data = (CsrUint16 *)CsrPmemAlloc(sizeof(CsrUint32));

	CsrMemCpy(p_data , &logLevel, sizeof(CsrUint32));

	csr_bt_server_app_put_message_to_tx_thread(CSR_CM_SS_ID, GAP_GET_DBG_LEVEL_IND,p_data,sizeof(CsrUint32));
}



void CsrAppBtGapError(void)
{

	csr_bt_server_app_put_message_to_tx_thread(CSR_CM_SS_ID, GAP_ERROR_IND,NULL,0);
}


void CsrAppBtGapInitInd()
{
	csr_bt_server_app_put_message_to_tx_thread(CSR_CM_SS_ID, GAP_BT_INIT_IND,NULL,0);
}

#ifndef APP_GAP_HANDER_H__
#define APP_GAP_HANDER_H__


/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
#include "csr_bt_sc_db.h"
#include "csr_bt_result.h"

typedef struct
{
	CsrBtDeviceAddr deviceAddr;
	CsrBtClassOfDevice deviceClass;
	CsrBtDeviceName friendlyName;
	CsrUint16  pid;
	CsrUint16  vid;
	CsrInt8 rssi;

} GAP_SD_SEARCH_IND_T;


/* Definiton of the struct type for the Samsung application event functions  */
struct CsrBtAppGapEventHandlerStruct
{
   
   /* GAP_SC_PASSKEY_IND 	*/
    void (*csrBtGapScPassKeyIndFunc)
    	(CsrBtDeviceAddr  DeviceAddr, 	/* address of device for which a passkey is requested                           */
    	CsrBtDeviceName  DeviceName,  /* name of device for which a passkey is requested                              */
    	CsrUint8   Paired,              /* actual pair status of the device                                             */
    	CsrUint32   cod
    	);
      /* GAP_SC_SSP_JUSTWORS_IND	*/
    void (*csrBtGapScSspJustworksIndFunc)
    	(CsrBtDeviceAddr  DeviceAddr, /* address of device for which a passkey is requested                           */
    	CsrBtDeviceName  DeviceName,  /* name of device for which a passkey is requested                              */
    	CsrUint8   Paired,             /* actual pair status of the device                                             */
    	CsrUint32    cod
    	);
	  /* GAP_SC_BOND_IND	*/
	 void (*csrBtGapScBondIndFunc)
		  (CsrBool				 addedToScDbList,	 /* If TRUE the device is added to the Security Manager's device database		 */
		  CsrBtDeviceAddr		 deviceAddr,		 /* address of device that has been bonded										 */
    	  CsrBtResultCode   resultCode,               /* the result of the outgoing bonding procedure                                 */
		  CsrBtSupplier    resultSupplier,                             
    	  CsrBtScDbDeviceRecord * pRecord
    	); 
	     /* GAP_SC_ATHORIZE_IND 	*/
  void (*csrBtGapScAuthorizeIndFunc)
 	(CsrBtDeviceAddr  DeviceAddr, 	/* address of device for which a passkey is requested                           */
  	CsrBtDeviceName  DeviceName,  /* name of device for which a passkey is requested                              */
  	CsrUint16   Service              /* servie status of the device                                             */
  	);
	  /*GAP_LOCAL_BDADDR_CFM*/
	void (*csrBtGapCmLocalBdaddrCfmFunc)(CsrBtDeviceAddr		 deviceAddr);
	  /*GAP_LOCAL_NAME_CFM*/
	void (*csrBtGapCmLocalNameCfmFunc)(CsrBtDeviceName		 name);

		  /*GAP_SD_SEARCH_IND*/
	void (*csrBtGapSdSearchIndFunc)(GAP_SD_SEARCH_IND_T * searchdata);
    	  /*GAP_SD_SEARCH_CLOSE*/
	void (*csrBtGapSdSearchCloseFunc)();	  
	    	/*GAP_CHIP_WD_COUNT*/
	void (*csrBtGapChiWdCountFunc)(CsrUint16 varId);
		   /*GAP_GET_DBG_LEVEL_IND*/
	void (*csrBtGapGetDbgLevelIndFunc)(CsrUint32 dbgLevel);
			/*GAP_ERROR_IND*/
    void (*csrBtGapGetErrorIndFunc)(void);
           /*GAP_BT_INIT_IND*/
	void (*csrBtGapBtInitIndFunc)(void);
		  /*GAP_SC_DEREGISGER_SCDB_CFM*/
	void (*csrBtGapScDeregisterScdbCfmFunc)(CsrBtDeviceAddr         deviceAddr,CsrBtResultCode   resultCode,CsrBtSupplier    resultSupplier);
		  /*GAP_SC_ENCRYPTION_CFM*/
    void (*csrBtScEncryptionCfmFunc)(CsrBtDeviceAddr		   deviceAddr,	  CsrBtResultCode		  resultCode,CsrBtSupplier			 resultSupplier);

};
typedef struct CsrBtAppGapEventHandlerStruct CsrBtAppGapEventHandlerStruct;




/* MGT_SERVER_PROCESS_CLOSE  */
typedef  void (*csrBtMgtErrInd)(void);

 typedef struct
 {
   CsrUint32                classOfDevice;                        /* device class of peer device                                              */
   CsrBtDeviceAddr          deviceAddr;                 /* address of device for which a passkey is requested                       */
  CsrBtDeviceName          friendlyName;                 /* name of device for which a passkey is requested                          */
  CsrBool enable;
  CsrUint8 discoverable;
  CsrUint8 connectable;
  CsrUint8 szPincode[CSR_BT_PASSKEY_MAX_LEN]; // For null charater
  unsigned int iPinLength;
 } BluetoothCsrDeviceInfo;

#define CSR_SERVER_NAME "csr_bt_app_hci"

extern CsrInt16 csr_get_pid_by_name(const char* processName);

extern CsrInt16 csr_client_mgt_open(csrBtMgtErrInd ClsoeCB,CsrBtAppGapEventHandlerStruct * btGapCallbackFunctionsPtr);

extern CsrInt16 csr_client_mgt_close(void);

//extern void csr_client_mgt_connect(void);

extern CsrInt16 csr_gap_set_scan_enable_cmd(CsrBool     disableInquiryScan, CsrBool     disablePageScand );

extern CsrInt16 csr_gap_connectable_cmd(CsrBool connectAble );

extern CsrInt16 csr_gap_local_bdaddr_cmd(void );

extern CsrInt16 csr_gap_local_name_cmd(void );

extern CsrInt16 csr_gap_sd_search_cmd( CsrUint16 maxnum, CsrUint32 searchTime , CsrUint32 bufferTime, CsrBtClassOfDevice Cod, CsrBool skipNameRequest );

extern CsrInt16 csr_gap_sd_search_cancel_cmd( void );

extern CsrInt16 csr_gap_get_local_device_info(BluetoothCsrDeviceInfo  * pDeviceInfo );

extern CsrInt16 csr_gap_set_dbg_level(CsrUint32 level);

extern CsrInt16 csr_gap_ss_reset_cmd(void);

extern CsrInt16 csr_gap_sc_authorize_rsp_cmd( CsrBool isAuthozed , BD_ADDR_T            bd_addr);

extern CsrInt16 csr_gap_write_page_timeout_cmd(CsrUint16        timeout );

extern CsrInt16 csr_gap_sc_encryption_req_cmd( CsrBool isEncryption , BD_ADDR_T            bd_addr);

extern CsrInt16 csr_gap_sc_register_scdb_cmd(CsrBtScDbDeviceRecord * pRecord);

extern CsrInt16 csr_gap_gap_set_local_name_cmd(   CsrBtDeviceName            localName            /* the  name of the local Bluetooth device in utf8                      */);

extern CsrInt16 csr_gap_sc_deregister_scdb_cmd(   BD_ADDR_T            bd_addr);

extern CsrInt16 csr_gap_sc_passkey_res_cmd(  CsrBool          accept,CsrBtDeviceAddr    theDeviceAddr,
                                        CsrUint8         thePasskeyLength,
                                        CsrUint8         * thePasskey,
                                        CsrBool          authorised);	

extern CsrInt16 csr_gap_sc_justworks_res_cmd(  CsrBool  accept, CsrBtDeviceAddr deviceAddr, CsrBool authorised);


extern void csr_gap_init_wd_count(void);

extern unsigned short csr_gap_get_wd_count(void);
#ifdef __cplusplus
}
#endif

#endif//APP_GapEvent_CMD_HANDER_H__

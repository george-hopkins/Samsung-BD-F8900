/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "csr_bt_platform.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_hidh_lib.h"
#include "csr_bt_hidh_prim.h"
//#include "csr_bt_cm_lib.h"
#include "Bluetooth_Common.h"
#include "csr_bt_hid_parser.h"
//#include "csr_bt_hidh_demo_app.h"
//#include "csr_bt_nvs_hid.h"
//#include "csr_bt_cm_private_lib.h"
//#include "csr_bccmd_lib.h"
//#include "csr_sched_init.h"
//#include "csr_bt_sdc_support.h"
#include "Bluetooth_HIDH.h"
#include "AppHIDHlib.h"
#include "csr_bt_ss_prim.h"


//#include "../../csrhid/csrhid_loopback.h"
#define HIDH_DEBUG

#define HID_MINOR_DEV_CLASS_POINTER     0x80
#define HID_MINOR_DEV_CLASS_KEYBOARD    0x40

#define HIDH_MAX_SDP_ATTRIBUTE    300 /* maximum SDP attribute size to read */

#define CHANGE_STATE(s) (instData->state=s)

/* Defines for PTS RF-shielding */
#define ENABLE_TX_VARID  0x4007
#define DISABLE_TX_VARID 0x4008

typedef enum
{
    HID_PARSER_VERSION            = 0x0201,
    HID_DEVICE_SUBCLASS,
    HID_COUNTRY_CODE,
    HID_VIRTUAL_CABLE,
    HID_RECONNECT_INITIATE,
    HID_DESCRIPTOR_LIST,
    HID_LANGID_BASE_LIST,
    HID_SDP_DISABLE,
    HID_BATTERY_POWER,
    HID_REMOTE_WAKE,
    HID_PROFILE_VERSION,
    HID_SUPERVISION_TIMEOUT,
    HID_NORMALLY_CONNECTABLE,
    HID_BOOT_DEVICE,
    ENUM_HID_ATTRIBUTE_END
} hidh_sdp_attrib_t;


typedef enum  {
	CSRHID_DRV_CONNECT = 0,
	CSRHID_DRV_DISCONNECT,
	CSRHID_DRV_DATA,
	CSRHID_DRV_ACTIVATE,
	CSRHID_DRV_DEACTIVATE,
} opcodes;

typedef struct
{
	CsrUint8 opcode;
	CsrUint8 deviceIndex;
	CsrUint16 messageSize;
	CsrUint16 vendorId;
	CsrUint16 productId;
	CsrUint16 deviceVersion;
    	CsrUint8 *report;
}csrhid_message;

const char *state_str[HIDH_NOF_STATES] =
{
    "IDLE",
    "CONNECTED",
    "AWAIT_CONNECT",
    "UNPLUG",
    "RECONNECTING",
    "AWAIT_USER_REG"
};
#if 0
static void hidhSendReport(CsrUint8 reportId, CsrUint8 data)
{
    CsrUint16    reportLen;
    CsrUint8     *reportData;

    reportLen = 3;
    reportData = CsrPmemAlloc(reportLen);

    reportData[0] = 0; /* Reserved */
    reportData[1] = reportId;
    reportData[2] = data;

    CsrBtHidhDataReqSend(0, CSR_BT_HIDH_OUTPUT_REPORT,
                    reportLen, reportData);
}

#endif
#ifndef EXCLUDE_CSR_BT_HID_DRIVER


/* HID Driver Interface Definitions */
CsrUint32 HidDriver_open()
{
    CsrInt32 fd = -1;
    
    fd = open("/dev/bluetoothinput", O_WRONLY);
    
    if (fd < 0) {
        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"<%s (Line: %i)> Failed opening /dev/bluetoothinput",
                                     __FUNCTION__, __LINE__));
    }    
#ifdef HIDH_DEBUG    
    else
    {
            CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH: HidDriver_open Success \n"));
    }
#endif       
    return fd;
}

CsrUint16 HidDriver_write(CsrUint32 fd, CsrUint8 *hidReport,CsrUint8 reportLen)
{
    return write(fd, hidReport,  reportLen);    
}

int HidDriver_close(int fd)
{
    int result = -1;
    
    result = close(fd);
    
    if (result < 0) {
        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"<%s (Line: %i)> Failed Closing /dev/bluetoothinput", __FUNCTION__, __LINE__));
        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH: HidDriver_close failed with %d \n", fd));   
    }
#ifdef HIDH_DEBUG    
    else
    {
       CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH: HidDriver_close Success \n"));
    }
#endif    
    
    return result;
}
#endif
#if 0
/*************************************************************************************
  startConnecting:
************************************************************************************/
void startConnecting(HidhAppInst *instData)
{
    CsrBtHidhSdpInfo *sdpInfo = NULL;
    CsrCharString       *serviceName;
    CsrUint16        descLen;
    CsrUint8        *desc;

    if( instData->selectedDevice == CSR_BT_UNDEFINED)
    {
        instData->selectedDevice = 0;
    }


    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Connecting  - please wait \n\n"));

    Nvs_GetHidInfo(&instData->bdAddrList[instData->selectedDevice],
                   &sdpInfo,
                   &serviceName,
                   &descLen,
                   &desc);

    if( (sdpInfo != NULL) && sdpInfo->isValid )
    {
        CsrBtHidhConnectReqSend( instData->qid,
                            instData->selectedDevice,
                            instData->bdAddrList[instData->selectedDevice],
                            L2CA_FLUSH_TO_DEFAULT,
                            NULL,
                            NULL,
                            sdpInfo,
                            serviceName,
                            descLen,
                            desc);
    }
    else
    {
        CsrBtHidhConnectReqSend( instData->qid, instData->selectedDevice,
                            instData->bdAddrList[instData->selectedDevice],
                            L2CA_FLUSH_TO_DEFAULT,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            0,
                            NULL);
    }
}

/*************************************************************************************
  startConnectAccept:
************************************************************************************/
void startConnectAccept(HidhAppInst *instData)
{
    CsrBtHidhSdpInfo *sdpInfo = NULL;
    CsrCharString       *serviceName = NULL;
    CsrUint16        descLen = 0;
    CsrUint8        *desc = NULL;

    if( instData->selectedDevice == CSR_BT_UNDEFINED)
    {
        instData->selectedDevice = 0;
    }

    Nvs_GetHidInfo(&instData->bdAddrList[instData->selectedDevice],
                   &sdpInfo,
                   &serviceName,
                   &descLen,
                   &desc);

    if( (sdpInfo != NULL) && sdpInfo->isValid )
    {
        CsrBtHidhConnectAcceptReqSend( instData->qid, 0,
                                  instData->bdAddrList[instData->selectedDevice],
                                  L2CA_FLUSH_TO_DEFAULT,
                                  NULL,
                                  NULL,
                                  sdpInfo,
                                  serviceName,
                                  descLen,
                                  desc);

        CHANGE_STATE(HIDH_STATE_AWAIT_CONNECT);
    }
    else
    {
        CsrPmemFree(sdpInfo);
        CsrPmemFree(serviceName);
        CsrPmemFree(desc);
        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Device information not available, unable to accept connection\n"));
    }
}


#endif

/*************************************************************************************
  initInstanceData:
************************************************************************************/
void initInstanceData(HidhAppInst *instData)
{
//    CsrUint8    i;
 
    instData->state                 = HIDH_STATE_IDLE;
 //   instData->numInList             = 0;
    instData->selectedDevice        = CSR_BT_UNDEFINED;
    instData->action                = NULL_ACTION;
 //   instData->device                = NULL;
   // instData->nonZeroIdleRate       = FALSE;
//    instData->isEncrypted           = FALSE;
 //   instData->showPtsExtras         = FALSE;
    instData->protocol              = CSR_BT_HIDH_REPORT_PROTOCOL;
//    instData->scMsgPending          = FALSE;
#ifndef EXCLUDE_CSR_BT_HID_DRIVER	
	instData->fd =-1;
#endif
#if 0
    for (i = 0; i < MAX_DISCOVERY_RESULTS_IN_LIST; i++)
    {
        instData->bdAddrList[i].lap = 0;
        instData->bdAddrList[i].nap = 0;
        instData->bdAddrList[i].uap = 0;
    }
    instData->selectedDevice = 0;
#endif

    instData->hidServiceRecordHandle = 0;;
}


/**************************************************************************************************
 *
 *    init function called by the scheduler upon initialisation. This function is used to boot
 *    the demo application.
 *
 **************************************************************************************************/
void init_hidh(void **gash)
{
    HidhAppInst *instData;

    /*  allocate and initialise instance data space  */
    *gash = (void *) CsrPmemAlloc(sizeof(HidhAppInst));
    instData = (HidhAppInst *) *gash;

    initInstanceData(instData);
    //Nvs_Init();

    /* Get control application handle */
    instData->qid = TESTQUEUE;


}



#ifdef ENABLE_SHUTDOWN
void deinit_hidh(void * *gash)
{

	HidhAppInst *instData;

	instData = *gash;

//    CsrPmemFree(instData->device);
    CsrPmemFree(instData);
}
#else
void deinit_hidh(void **gash)
{
	CSR_UNUSED(gash);
}
#endif



void printOutHidData(CsrUint8 *data,
                     CsrUint16 dataLen)
{
    CsrUint16 i, startDataIndex = 0;

    if (1 == data[1])
    {
        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Keyboard(%02x): ", data[1]));
        startDataIndex = 2;
    }
    else if (2 == data[1])
    {
        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Mouse(%02x): buttons(%02x), X(%i), Y(%i) ", data[1], data[2], (CsrInt8)data[3], (CsrInt8)data[4]));
        startDataIndex = 5;
    }
    for(i=startDataIndex; i<dataLen;i++)
    {
        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"%02x ", data[i]));
    }
    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"\n%d bytes received\n", dataLen));

}

void printOutHandshakeResultCode(CsrBtResultCode resultCode)
{
    switch (resultCode)
    {
        case CSR_BT_RESULT_CODE_HIDH_SUCCESS:
            {
                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_RESULT_CODE_HIDH_SUCCESS"));
                break;
            }
        case CSR_BT_RESULT_CODE_HIDH_NOT_READY:
            {
                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_RESULT_CODE_HIDH_NOT_READY"));
                break;
            }
        case CSR_BT_RESULT_CODE_HIDH_INVALID_REPORT_ID:
            {
                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_RESULT_CODE_HIDH_INVALID_REPORT_ID"));
                break;
            }
        case CSR_BT_RESULT_CODE_HIDH_UNSUPPORTED:
            {
                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_RESULT_CODE_HIDH_UNSUPPORTED"));
                break;
            }
        case CSR_BT_RESULT_CODE_HIDH_INVALID_PARAMETER:
            {
                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_RESULT_CODE_HIDH_INVALID_PARAMETER"));
                break;
            }
        case CSR_BT_RESULT_CODE_HIDH_UNKNOWN:
            {
                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_RESULT_CODE_HIDH_UNKNOWN"));
                break;
            }
        case CSR_BT_RESULT_CODE_HIDH_FATAL:
            {
                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_RESULT_CODE_HIDH_FATAL"));
                break;
            }
        default:
            {
                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Unknown result code"));
                break;
            }
    }
}

/**************************************************************************************************
Function to handle all hidh-primitives.
**************************************************************************************************/
void handleHidhPrim(HidhAppInst *instData, CsrPrim    *primType)
{
  //  primType = (CsrBtHidhPrim *) instData->recvMsgP;
 //   printf("handleHidhPrim %x\n",*primType);
    switch (*primType)
    {
        case CSR_BT_HIDH_DATA_IND:
            {
                CsrBtHidhDataInd        *prim = (CsrBtHidhDataInd *) primType;


                if( prim->reportType != CSR_BT_HIDH_OTHER)
                {
#ifndef EXCLUDE_CSR_BT_HID_DRIVER                
					CsrInt32 driverResultCode = 0; 
					csrhid_message *send_data;
					static CsrUint16 report_data_len = 0;
                    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Report(%i): ", prim->reportType));
                    printOutHidData(prim->data, prim->dataLen);
					


					send_data = (csrhid_message*) CsrPmemAlloc(sizeof(csrhid_message)) ;					 
					if (send_data == NULL)
					{
					   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_HIDH_DATA_IND FAIL: report_desc CsrPmemAlloc Fail"));
					   return;
					}

					/* Report Descriptor holder memory block */
					send_data->report = (CsrUint8*) CsrPmemAlloc(sizeof(CsrUint8) * prim->dataLen);					
					if (send_data->report == NULL)
					{

					   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_HIDH_DATA_IND FAIL: report_desc CsrPmemAlloc Fail"));
					   return;
					}
					report_data_len = prim->dataLen;

					send_data->messageSize = report_data_len;
					send_data->opcode = CSRHID_DRV_DATA;
					send_data->deviceIndex = 0;/*Nvs_FindInList(&instData->bdAddrList[instData->selectedDevice]);*/
					CsrMemCpy(send_data->report, (CsrUint8*)(prim->data+1), prim->dataLen );

					/* Open the HID Loopback Driver Character Interface*/
					if (instData->fd < 0)
					instData->fd = HidDriver_open();

					if(instData->fd>0)
					{
					driverResultCode = HidDriver_write(instData->fd, (CsrUint8*)send_data,	\
														sizeof(csrhid_message)); 
							if (driverResultCode < 0)
					{
					          CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"wrinting err...\n"));
					}
					}
					CsrPmemFree(send_data->report);                     
					CsrPmemFree(send_data);
 
#else
                    CsrAppServerHidhDataInd(prim->dataLen,prim->data);

#endif					
                }
                else
                {
                    if( instData->action == GET_PROTOCOL_ACTION)
                    {
                        instData->protocol = prim->data[1];
                        if( instData->protocol )
                        {
                            CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Get protocol returned: Report\n"));
                        }
                        else
                        {
                            CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Get protocol returned: Boot"));
                        }
                    }
                    else if( instData->action == GET_IDLE_ACTION)
                    {
                        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Get idle returned: %i", prim->data[1]));
                    }
                    instData->action = NULL_ACTION;

                }
				
				CsrPmemFree(prim->data);
                break;
            }
        case CSR_BT_HIDH_REGISTER_USER_CFM:
            {
                CsrBtHidhRegisterUserCfm *prim = (CsrBtHidhRegisterUserCfm*)primType;

                if(prim->resultCode == CSR_BT_RESULT_CODE_HIDH_SUCCESS &&
                   prim->resultSupplier == CSR_BT_SUPPLIER_HIDH)
                {
                    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Received a successful REGISTER_USER confirmation for deviceId %i\n",
                           prim->slotId));
                    CHANGE_STATE(HIDH_STATE_IDLE);
					CsrAppServerHidhActivateCfm();
                }
                else
                {
                    CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,"Received a REGISTER_USER error with code 0x%02X (supplier 0x%02X)\n", prim->resultCode, prim->resultSupplier));
                }
                break;
            }

        case CSR_BT_HIDH_CONNECT_ACCEPT_IND:
            {
#if 0				
                CsrBtHidhSdpInfo *sdpInfo = NULL;
                CsrCharString *serviceName = NULL;
                CsrUint16 descLen = 0;
                CsrUint8 *desc = NULL;

                Nvs_GetHidInfo(&instData->bdAddrList[instData->selectedDevice],
                               &sdpInfo,
                               &serviceName,
                               &descLen,
                               &desc);
#endif
#if 0

//                if( (sdpInfo != NULL) && (sdpInfo->deviceSubclass & HID_MINOR_DEV_CLASS_KEYBOARD))
                {
                    /* the device is a keyboard => encrypt connection */
                    CsrBtScEncryptionReqSend(instData->qid,
                                        instData->bdAddrList[instData->selectedDevice], TRUE);
                    instData->scMsgPending = TRUE;
                }

                CsrPmemFree(sdpInfo);
                CsrPmemFree(serviceName);
                CsrPmemFree(desc);
#endif
                CHANGE_STATE(HIDH_STATE_CONNECTED);

                
                break;
            }
        case CSR_BT_HIDH_CONNECT_CFM:
            {
                CsrBtHidhConnectCfm        *prim = (CsrBtHidhConnectCfm *) primType;
 
				//CsrInt32 deviceIndex;
				//CsrBtHidhSdpInfo *sdpInfo = NULL;
				//CsrCharString		*serviceName;
				//CsrUint16		 descLen;
//				CsrUint8		*desc;
				CsrAppBtHidhSdpInfoData sdpInfoData;


				sdpInfoData.deviceAddr=prim->deviceAddr;
				sdpInfoData.sdpInfo=prim->sdpInfo;
				sdpInfoData.serviceName=prim->serviceName;
				sdpInfoData.descriptorLength=prim->descriptorLength;
				sdpInfoData.descriptor=prim->descriptor;
				 CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_HIDH_CONNECT_CFM btConnId[%d] deviceId[%d]",prim->btConnId,prim->deviceId));

                if (prim->resultCode == CSR_BT_RESULT_CODE_HIDH_SUCCESS &&
                    prim->resultSupplier == CSR_BT_SUPPLIER_HIDH)
                {
#ifndef EXCLUDE_CSR_BT_HID_DRIVER
                    csrhid_message *send_desc = NULL;
                    CsrUint16 report_desc_len = 0;
                	CsrInt32 driverResultCode = 0;
#endif					
#if 0                
                    if( (prim->sdpInfo != NULL) && (prim->sdpInfo->deviceSubclass & HID_MINOR_DEV_CLASS_KEYBOARD))
                    {
                        /* the device is a keyboard => encrypt connection */
                        CsrBtScEncryptionReqSend(instData->qid,
                                            instData->bdAddrList[instData->selectedDevice],
                                            TRUE);
                        instData->scMsgPending = TRUE;
                    }
#endif					
					
#ifndef EXCLUDE_CSR_BT_HID_DRIVER					 
					                    /* Open the HID Loopback Driver Character Interface*/					
					if (instData->fd < 0)
					instData->fd = HidDriver_open();
					
									/* Get the Length of the HID Report Descriptor alone */
					report_desc_len = prim->descriptorLength - 8 ;
					send_desc = (csrhid_message*) malloc(sizeof(csrhid_message)) ; 
					send_desc->messageSize = report_desc_len;	
									 /* Report Descriptor holder memory block */
					send_desc->report = (CsrUint8*) malloc(sizeof(CsrUint8) * report_desc_len);
					send_desc->opcode = CSRHID_DRV_CONNECT;
					send_desc->deviceIndex = instData->selectedDevice;
					send_desc->vendorId = prim->sdpInfo->vendorId;
					send_desc->productId = prim->sdpInfo->productId;
					send_desc->deviceVersion = prim->sdpInfo->deviceVersion ;			 
					CsrMemCpy(send_desc->report, (CsrUint8*)(prim->descriptor + 8), report_desc_len );
					result=TRUE;
					if(instData->fd>0)
					{
						driverResultCode = HidDriver_write(instData->fd, (CsrUint8*)send_desc, sizeof(csrhid_message)); 
						if (driverResultCode < 0)
						{
									  CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"wrinting err.."));
						}
						else
						{
						  
						}
					}
					else
					{
					   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_HIDH_CONNECT_CFM err..."));
					}
					CsrPmemFree(send_desc->report); 					
					CsrPmemFree(send_desc); 
#endif
                    instData->connectedAddr=prim->deviceAddr;
				

                    CHANGE_STATE(HIDH_STATE_CONNECTED);
                }
                else
                {
                    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Establishment failed, remote device not responding...resultCode[%x] resultSupplier[%x]\n",prim->resultCode,prim->resultSupplier));
                }
                

                if(prim->sdpInfo)
                {

                    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"\nHID device plugged in: %s\n", prim->serviceName));
                    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Vendor ID: %X, Product ID: %X, Version: %i.%i.%i\n",
                           prim->sdpInfo->vendorId,
                           prim->sdpInfo->productId,
                           prim->sdpInfo->deviceVersion >> 8,
                           (prim->sdpInfo->deviceVersion & 0x00F0) >> 4,
                           prim->sdpInfo->deviceVersion & 0x000F ));
#if 0
                    Nvs_PutHidInfo(&prim->deviceAddr,
                                   prim->sdpInfo,
                                   prim->serviceName,
                                   prim->descriptorLength,
                                   prim->descriptor);
#endif

                }

				CsrAppServerHidhConnetCfm(prim->resultCode,prim->resultSupplier,&sdpInfoData);


                CsrPmemFree(prim->descriptor);
                CsrPmemFree(prim->serviceName);
                CsrPmemFree(prim->sdpInfo);

                break;
            }
        case CSR_BT_HIDH_CONNECT_ACCEPT_CFM:
            {
                CsrBtHidhConnectAcceptCfm        *prim = (CsrBtHidhConnectAcceptCfm *) primType;

                if(prim->resultCode != CSR_BT_RESULT_CODE_HIDH_SUCCESS ||
                   prim->resultSupplier != CSR_BT_SUPPLIER_HIDH)
                {
                    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_HIDH_CONNECT_ACCEPT_CFM Connect accept call rejected with result: 0x%02X (supplier 0x%02X)\n", prim->resultCode, prim->resultSupplier));
                }
                else
                {
                    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, " CSR_BT_HIDH_CONNECT_ACCEPT_CFM Waiting for HID device (%04X:%02X:%06X) to connect...  \n\n",
                            prim->deviceAddr.nap,
                            prim->deviceAddr.uap,
                            prim->deviceAddr.lap ));
                }
				CsrAppServerHidhConectAccectCfm(&(prim->deviceAddr),prim->resultCode,prim->resultSupplier);
                
                break;
            }
        case CSR_BT_HIDH_DISCONNECT_CFM:
        case CSR_BT_HIDH_DISCONNECT_IND:
            {
				CsrBtHidhDisconnectInd        *prim = (CsrBtHidhDisconnectInd *) primType;
                CHANGE_STATE(HIDH_STATE_IDLE);
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_HIDH_DISCONNECT_IND deviceId[%d], reasonCode[%d] reasonSupplier[%d]",prim->deviceId, prim->reasonCode,prim->reasonSupplier));
				if(*primType==CSR_BT_HIDH_DISCONNECT_CFM)
					CsrAppServerHidhDisconnectCfm(& instData->connectedAddr,prim->reasonCode,prim->reasonSupplier);
				else					
					CsrAppServerHidhDisconnectInd(& instData->connectedAddr,prim->reasonCode,prim->reasonSupplier);
#ifndef EXCLUDE_CSR_BT_HID_DRIVER				
				if (instData->fd >= 0)
				{
					HidDriver_close(instData->fd);
					instData->fd = -1;							  
				}
#endif
                
                break;
            }
        case CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM:
            {
				CsrBtHidhCancelConnectAcceptCfm    *prim = (CsrBtHidhCancelConnectAcceptCfm *) primType;
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM resultCode[%x] resultSupplier[%x]",prim->resultCode,prim->resultSupplier));
				 
                if( instData->state == HIDH_STATE_AWAIT_CONNECT )
                {
                    if(prim->resultCode == CSR_BT_RESULT_CODE_HIDH_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_HIDH )
                    {
                        CHANGE_STATE(HIDH_STATE_IDLE);
                    }
					
					
                }
                break;
            }
        case CSR_BT_HIDH_CONTROL_IND:
            {
                CsrBtHidhControlInd    *prim = (CsrBtHidhControlInd *) primType;
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_HIDH_CONTROL_IND operation %x deviceId %d", prim->operation,prim->deviceId));

                if( prim->operation == CSR_BT_HIDH_VC_UNPLUG)
                {
                    CsrBtHidhDisconnectReqSend(0);
#if 0					
                    //Nvs_RemoveHidInfo(&instData->bdAddrList[instData->selectedDevice]);
#endif
                }
                break;
            }
        case CSR_BT_HIDH_HANDSHAKE_IND:
            {
				
                CsrBtHidhHandshakeInd    *prim = (CsrBtHidhHandshakeInd *) primType;

                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Received HANDSHAKE, result code:[0x%02X] deviceId[%d]", prim->resultCode,prim->deviceId));
                printOutHandshakeResultCode(prim->resultCode);

                if(prim->resultCode != CSR_BT_RESULT_CODE_HIDH_SUCCESS)
                {
                    instData->action = NULL_ACTION;
                }

                break;
            }
        case CSR_BT_HIDH_STATUS_IND:
            {
                CsrBtHidhStatusInd    *prim = (CsrBtHidhStatusInd *) primType;
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_HIDH_STATUS_IND deviceId %d",prim->deviceId));

                if( prim->status == CSR_BT_HIDH_STATUS_CONNECTED )
                {
                    if(instData->state == HIDH_STATE_RECONNECTING)
                    {
                        CHANGE_STATE(HIDH_STATE_CONNECTED);
#if 0                        
                        if( instData->isEncrypted )
                        {
                            /* the connection was encrypted before the drop-out, start encryption again */
                            CsrBtScEncryptionReqSend(instData->qid, instData->bdAddrList[instData->selectedDevice], TRUE);
                            instData->scMsgPending = TRUE;
                        }
#endif						
                    }
#if 0
                    if( prim->descriptor != NULL)
                    {
                        instData->device = CsrBtHidParse( prim->descriptor,
                                                     prim->descriptorLength);

                    }
#endif					
                }
                else if(prim->status == CSR_BT_HIDH_STATUS_RECONNECTING)
                {
                    CHANGE_STATE(HIDH_STATE_RECONNECTING);
                    
                }
				CsrAppServerHidhStatusInd(prim->status);

                CsrPmemFree(prim->descriptor);
                CsrPmemFree(prim->serviceName);
                CsrPmemFree(prim->sdpInfo);

                break;
            }

        case CSR_BT_HIDH_SECURITY_IN_CFM:
            {
				CsrBtHidhSecurityInCfm *prim = (CsrBtHidhSecurityInCfm *) primType;
				 CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_HIDH_SECURITY_IN_CFM resultCode[%x] resultSupplier[%x]",prim->resultCode, prim->resultSupplier));
            //    handleScSecLevelPrim(&instData->scInstData);
                break;
            }
		 case CSR_BT_HIDH_SECURITY_OUT_CFM:
		 	{
				CsrBtHidhSecurityOutCfm *prim = (CsrBtHidhSecurityOutCfm *) primType;
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_HIDH_SECURITY_OUT_CFM resultCode[%x] resultSupplier[%x]",prim->resultCode , prim->resultSupplier));
				break;
		 	}
        default:
            {
                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"## HID host app. unhandled HIDH prim 0x%04x,\n", *primType));
                CsrBtHidhFreeUpstreamMessageContents(CSR_BT_HIDH_PRIM, primType);
            }
    }

}

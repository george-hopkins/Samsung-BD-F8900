/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_sc_db.h"
#include "csr_bt_util.h"
#include "csr_bt_common.h"
#include "csr_samsung_log.h"
#include "csr_framework_ext.h"
#include "csr_bt_ss_prim.h"


typedef struct CsrBtScDbListStructType
{
  CsrBtScDbDeviceRecord deviceVariable[MAX_APP_REGISTER_DEV];
}CsrBtScDbList;

static CsrBtScDbList         devicelist;
//static CsrBtScDbDeviceRecord deviceVariable;
static CsrBtScDbDeviceRecord null_deviceVariable;


static CsrUint16  deviceIdx=0;

#if 1
static CsrMutexHandle db_mutex;

static void CsrDbLock(void)
{
   static CsrBool initialise=FALSE;

   CsrGlobalMutexLock();
   if(!initialise)
   {
      CsrMutexCreate(&db_mutex);
	  initialise=TRUE;
   }
   CsrMutexLock(&db_mutex);
   CsrGlobalMutexUnlock();
}

static void CsrDbUnlock(void)
{
   CsrMutexUnlock(&db_mutex);
}
#endif

CsrBool CsrBtScDbRead(CsrBtDeviceAddr *theAddr, CsrBtScDbDeviceRecord *theRecord)
{ /* Should return TRUE and the information about the requested device, if the device is not present in the data base return FALSE */
    CsrBool result=FALSE;
	CsrUint16  i;
	
 	CsrDbLock();
    for(i=0;i<MAX_APP_REGISTER_DEV;i++)
    {
	    if(bd_addr_eq(theAddr, &devicelist.deviceVariable[i].deviceAddr) )
	    {
	         CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbRead idx[%d] \n",i));
	         *theRecord = devicelist.deviceVariable[i];
	         result=TRUE;
			 break;
	    }
    }

	if(result==FALSE)
    {
         CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbRead not foud"));
         null_deviceVariable.linkkeyValid = FALSE;
        *theRecord = null_deviceVariable;
    }
	CsrDbUnlock();
	
	 return (result);
}

void CsrBtScDbWrite(CsrBtDeviceAddr *theAddr, CsrBtScDbDeviceRecord *theRecord)
{ /* Should write information about into the data base, if the device already exist, the present entry should be updated */
    CsrBool result=FALSE;
	CsrUint16  i;


	CsrDbLock();
    for(i=0;i<MAX_APP_REGISTER_DEV;i++)   ///
    {
	    if(bd_addr_eq(theAddr, &devicelist.deviceVariable[i].deviceAddr) )   //update
	    {
	         CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbWrite idx[%d] \n",i));
	          devicelist.deviceVariable[i]= *theRecord;
	         result=TRUE;
			 break;
	    }
    }
	
    if(result ==FALSE)
    {
		for(i=0;i<MAX_APP_REGISTER_DEV;i++)                             //
	    {
		    if( bd_addr_eq(&devicelist.deviceVariable[i].deviceAddr, &null_deviceVariable.deviceAddr) )
		    {
		         CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbWrite idx[%d] \n",i));
		          devicelist.deviceVariable[i]= *theRecord;
		         result=TRUE;
				 break;
		    }
	    }
    }

	if(result ==FALSE)
	{
    	devicelist.deviceVariable[0] = *theRecord;   //error
    	    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbWrite error"));
	}
	CsrDbUnlock();
}

void CsrBtScDbRemove(CsrBtDeviceAddr *theAddr)
{ /* SHould remove the device from the data base */
    CsrBool result=FALSE;
	CsrUint16  i;
	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbRemove"));

//    CSR_UNUSED(theAddr);
	CsrDbLock();
    for(i=0;i<MAX_APP_REGISTER_DEV;i++)
    {
	    if(bd_addr_eq(theAddr, &(devicelist.deviceVariable[i].deviceAddr)) )
	    {
	         CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbRemove [%d] \n",i));
	         // devicelist.deviceVariable[i]= *theRecord;
			//  bd_addr_zero( &(deviceVariable.deviceAddr) );
			  CsrMemSet(&(devicelist.deviceVariable[i]),0x00,sizeof(CsrBtScDbDeviceRecord));
	         result=TRUE;
			 break;
	    }
    }
	CsrDbUnlock();
	if(result ==FALSE)
	{

    	    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbRemove error"));
	}
	
}

CsrBool CsrBtScDbReadFirst(CsrBtScDbDeviceRecord *theRecord)
{ /* Should return TRUE and the first valid device if it exist, is no device exist it return FALSE */
 //   CSR_UNUSED(theRecord);
   CsrBool result=FALSE;
   CsrDbLock();
 
   deviceIdx=0;
   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbReadFirst"));

   for(deviceIdx=0;deviceIdx<MAX_APP_REGISTER_DEV;deviceIdx++)
   {
	    if(bd_addr_eq(&(null_deviceVariable.deviceAddr), &(devicelist.deviceVariable[deviceIdx].deviceAddr))==FALSE )
	    {
	         CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbReadFirst deviceIdx [%d] \n",deviceIdx));
	         // devicelist.deviceVariable[i]= *theRecord;
			//  bd_addr_zero( &(deviceVariable.deviceAddr) );

			 *theRecord= devicelist.deviceVariable[deviceIdx];
	         result=TRUE;
			 break;
	    }
    }
    CsrDbUnlock();
   if(result ==FALSE)
   {
   
		   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbReadFirst  not found"));
   }


  return (result);
}

CsrBool CsrBtScDbReadNext(CsrBtScDbDeviceRecord *theRecord)
{ /* Should return TRUE and the next valid device in line if it exist, is no device exist it return FALSE */
   	CsrUint16  i;
	CsrBool result=FALSE;
	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbReadNext"));
	
	 CsrDbLock();
	deviceIdx++;  //next index 

	for(i=deviceIdx;i<MAX_APP_REGISTER_DEV;i++)
    {
	    if(bd_addr_eq(&(null_deviceVariable.deviceAddr), &(devicelist.deviceVariable[i].deviceAddr)) ==FALSE )
	    {
	         CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbReadNext deviceIdx[%d] \n",i));
	         // devicelist.deviceVariable[i]= *theRecord;
			//  bd_addr_zero( &(deviceVariable.deviceAddr) );
			 *theRecord= devicelist.deviceVariable[i];
			 deviceIdx=i;
	         result=TRUE;
			 break;
	    }
    }
	CsrDbUnlock();
   if(result ==FALSE)
   {
   
		   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "[simple mem]CsrBtScDbReadNext not found"));
   }	
	
    return(result);
}

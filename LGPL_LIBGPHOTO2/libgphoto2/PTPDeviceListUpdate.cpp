/**
* @file		CPTPDeviceListUpdate
* @brief		USB device manager
* @author	    Sheetal(SISC) and Niraj(SISC)
* @author	Sandeep Kumar(SISC) modified for PTP Thread Safe Driver
* @Date	    	August, 2008
*
* Copyright 2010 by Samsung Electronics, Inc.,
* 
* This software is the confidential and proprietary information
* of Samsung Electronics, Inc. ("Confidential Information").  You
* shall not disclose such Confidential Information and shall use
* it only in accordance with the terms of the license agreement
* you entered into with Samsung.
*

*/

/*New Code Implementation is under PTP_THREAD_SAFE Macro */

#ifdef PTP_THREAD_SAFE
#include "PtpTypes.h"
#include "PtpDeviceAccess.h"
#include "PTPDeviceListUpdate.h"
#include "PtpCallback.h"

#ifdef CSP_LAYER_USAGE
#include "PCThread.h"
#endif
#else
#include "PTPDeviceListUpdate.h"
#include "usb.h"
#endif

#include "ptpapi.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
using namespace std;

#ifndef CSP_LAYER_USAGE

static pthread_mutex_t __m_mutex = PTHREAD_MUTEX_INITIALIZER;    /* Mutex variable */
#define MUTEX_BEGIN		pthread_mutex_lock(&__m_mutex)
#define MUTEX_END		pthread_mutex_unlock(&__m_mutex)
/*CSP Layer Implementation To be used in Samsung Platform i.e. Valencia or Trident */
#else
static PCMutex			__m_mutex = PTHREAD_MUTEX_INITIALIZER;  /* Mutex variable */
#define MUTEX_CREATE	__m_mutex.Create();
#define MUTEX_DESTROY	__m_mutex.Destroy();
#define MUTEX_BEGIN		__m_mutex.Lock();
#define MUTEX_END		&__m_mutex.UnLock();
#endif

/**
* @fn         		CPTPDeviceListUpdate::CPTPDeviceListUpdate(void)
* @brief         		CPTPDeviceListUpdate - Constructor.
* @remarks       
* @param         void
* @return        none.
* @see           none
*/

CPTPDeviceListUpdate::CPTPDeviceListUpdate(void)
{
    for (int i=0; i <16;i++)
	{
		m_available_ids[i] = 0;
	}
}


/**
* @fn         	CPTPDeviceListUpdate::~CPTPDeviceListUpdate(void)
* @brief        CPTPDeviceListUpdate - Destructor.
* @remarks       
* @param      void
* @return      none
* @see         none
*/

CPTPDeviceListUpdate::~CPTPDeviceListUpdate(void)
{

}

/**
* @fn         	int CPTPDeviceListUpdate::Initialize(void)
* @brief        This function initialises the PTP device.
* @remarks       
* @param      void
* @return      error code
*			-1 : in case of error.
*			0 : in case of success.
* @see         none
*/

int CPTPDeviceListUpdate::Initialize(void)
{
    //set callback to receive device events
#ifdef PTP_THREAD_SAFE
    PTPDevUsbRegisterCbFn(callback_on_device_change);
#else
    usb_set_callback(callback_on_device_change);	
#endif
    return 0;    
}


/**
* @fn         	int CPTPDeviceListUpdate::Finalize(void)
* @brief        This function stops the polling thread.
* @remarks       
* @param      void
* @return      error code
*			-1 : in case of error.
*			0 : in case of success.
* @see         none
*/

int CPTPDeviceListUpdate::Finalize(void)
{
    //remove the callback
#ifdef PTP_THREAD_SAFE
    PTPDevUsbRegisterCbFn(NULL);
#else
    usb_set_callback(NULL);
#endif
    return 0;    
}


/**
* @fn         	int CPTPDeviceListUpdate::GetPTPDeviceList( uint32_t a_Size, UsbDev* a_pDevpathList[],uint32_t *a_Dev_cnt)
* @brief        This function fetches the current Device List in the map.
* @param	unit32_t a_size:	maximum number of devices in the list
* 			UsbDev* 			a_pDevpathList[]:		Device List
* 			uint32_t *		a_Dev_cnt:		number of Devices in the list
* @return        error code
*					-1 : in case of error.
*					 0 : in case of success.
* @see           none*/

int CPTPDeviceListUpdate::GetPTPDeviceList( uint32_t a_Size, UsbDev* a_pDevpathList[],uint32_t *a_Dev_cnt)
{
    int result = 0;
    uint32_t maxSize = a_Size;
    uint32_t count = 0;

    if((a_Size == 0) || (a_pDevpathList == 0))
	{
		return -1;
	}		

    MUTEX_BEGIN;   

    for(devpath_iter MapItor = m_pPTPDevPathCtxMap.begin(); MapItor != m_pPTPDevPathCtxMap.end(); ++MapItor)
    {
        if(count == maxSize) 
        {
            result = -1; 
            break;
        }

        a_pDevpathList[count]=(*MapItor).first;	
        count++;
    }

    *a_Dev_cnt = count;

    MUTEX_END;

    return result;
}
#ifdef PTP_THREAD_SAFE

/**
* @fn         	void CPTPDeviceListUpdate::callback_on_device_change(struct stUsbDevice* device, int isRemoved)
* @brief        callback_on_device_change.
* @remarks       
* @param      stUsbDevice, device being connected/disconnected
*			isRemoved - 1 for removed; 0 for attached
* @return      void.
* @see         none
*/

void CPTPDeviceListUpdate::callback_on_device_change(struct stUsbDevice* device, int isRemoved)
{
    CPTPDeviceListUpdate::Get().OnDeviceChange(device,(isRemoved == 1));
}       

/**
* @fn         	void CPTPDeviceListUpdate::OnDeviceChange(struct stUsbDevice* device, bool isRemoved)
* @brief        This function is called if there is any change in number of connected devices in libusb.
* @remarks       
* @param      stUsbDevice* - 	device being attached or removed
*			isRemoved - 1 	for removed; 0 for attached
* @return        void
* @see           none
*/

void CPTPDeviceListUpdate::OnDeviceChange(struct stUsbDevice* device, bool isRemoved)
{

    if((device == NULL) || (device->iLastSelectedConfig == -1)||(NULL == device->ppConfigDesc))
    {
        PTP_DRV_WARN("USB Device Configuration  is not selected \n");
        return;
    }


    if(device->ppConfigDesc[device->iLastSelectedConfig] == NULL 
        || device->ppConfigDesc[device->iLastSelectedConfig]->interface == NULL 
        || device->ppConfigDesc[device->iLastSelectedConfig]->interface->altsetting == NULL )
    {
        PTP_DRV_WARN(" USB Device Address is NULL \n");
        return;
    }
    if(!isRemoved)
    {
        device->iAltSettingNum = -1;

        for(int iConfig = 0; iConfig < device->deviceDesc.bNumConfigurations; iConfig++)
        {
            for(int iIntf = 0; iIntf < device->ppConfigDesc[iConfig]->bNumInterfaces; iIntf++)
            {
                for(int iAlt = 0; iAlt < device->ppConfigDesc[iConfig]->interface[iIntf].num_altsetting ; iAlt++)
                {
                    if ( device->ppConfigDesc[iConfig]->interface[iIntf].altsetting[iAlt].bInterfaceClass == LIBUSB_CLASS_PTP)
                    {
                        device->iAltSettingNum = iAlt;
                        device->iLastClaimedInfterface = iIntf;
                        device->iLastSelectedConfig = iConfig;
                        goto FOUND;
                    }
                }		
            }						
        }

    }	

FOUND:	
    if(-1 != device->iAltSettingNum)

    {

        if(isRemoved == 1) 
        {        
            // extract the dc giving the dev & update both maps deleting corresponding dc.
            UsbDev ptpDevice;
            UsbDev* TempptpDevice = NULL;
            ptpDevice.ptp_path.bus_num=atoi(device->dirPath);
            ptpDevice.ptp_path.dev_num=(int)device->uiDevNum;

            MUTEX_BEGIN;
            for(devpath_iter MapItor = m_pPTPDevPathCtxMap.begin(); MapItor != m_pPTPDevPathCtxMap.end(); ++MapItor)
            {

                UsbDev* ptpDevice_map = (*MapItor).first;//to check whether there is a first field
                if ((ptpDevice_map->ptp_path.dev_num == ptpDevice.ptp_path.dev_num) && (ptpDevice_map->ptp_path.bus_num == ptpDevice.ptp_path.bus_num))
                {
                    m_available_ids[ptpDevice_map->DeviceId - 16] = 0;
                    TempptpDevice =	ptpDevice_map;
	            m_pPTPDevPathCtxMap.erase (MapItor);	
                    break;
                }
            }

            /*Device DisConnect Status Updation*/
            if(NULL != TempptpDevice)
            {
			PtpCallbackUpdateDevInfo(0,TempptpDevice, NULL);
			if(NULL != TempptpDevice)
			{
					delete TempptpDevice;
			}
            }

             MUTEX_END;
        }

        /*A new device has arrived*/
        else 
        {
            /*Make a new dc giving the dev & update both maps adding corresponding dc*/
            PTPDevContext* pNewDC = new PTPDevContext;
            pNewDC->dev = device;
            UsbDev *ptpDevice=new UsbDev;
            ptpDevice->ptp_path.bus_num=atoi(device->dirPath);
            ptpDevice->ptp_path.dev_num=(int)device->uiDevNum;

            for (int i=0; i <16;i++)
            {
                if (m_available_ids[i] == 0)
                {
                    ptpDevice->DeviceId = i+16;
                    m_available_ids[i] = 1;
                    break;
                }
            }

            MUTEX_BEGIN;
            /* Assign DevPath value...*/
            m_pPTPDevPathCtxMap[ptpDevice]= pNewDC;
            /*Update fields of UsbDev *ptpdevice*/
            if(0 == MakeUsbDevStr(ptpDevice))
            {
                /*Device Connect Status Updation*/
                PtpCallbackUpdateDevInfo(1,ptpDevice, pNewDC);
            }
		MUTEX_END;		

        }			

        PrintDeviceList();	/* Checking list of connected devices; can be removed if not required*/
    }
    else
    {
        PTP_DRV_WARN("\n!!!!!!!!!NOT PTP CLASS!!!!!!!!!!!!\n");
    }
}



/**
* @fn         	int CPTPDeviceListUpdate::MakeUsbDevStr(UsbDev *ptpDevice)
* @brief         This function updates the fields of UsbDev structure in the map
* @remarks       
* @param         UsbDev *ptpDevice - pointer to the UsbDev structure which is being updated

* @return        void
* @see           none
*/

int CPTPDeviceListUpdate::MakeUsbDevStr(UsbDev *ptpDevice)
{
#define PTP_DEFAULT_MODEL_NAME		"Digital Camera"	
#define PTP_DEFAULT_VENDOR_NAME		"PTP Device"
#define PTP_DEFAULT_SERIAL_NAME		"123456789012345678901234567890"
    PTPDeviceInfo deviceinfo;
    PTPStorageIDs storageids;
    uint16 nResult;


    if ((nResult = PtpCallbackInitComm (ptpDevice->ptp_path)) != PTP_RC_OK)
    {       
        //error handling		
        if (nResult==0X2ff || nResult== 0x2fd || nResult ==0x2fe)
		{
			PTP_DRV_ERR("\nCamera could not be intialised due to IO error Please restart the camera\n");
		}
		else 
		{
			PTP_DRV_ERR("\nPtpCallbackInitComm failed\n");
		}

        /*erasing from map*/

        for(devpath_iter MapItor = m_pPTPDevPathCtxMap.begin(); MapItor != m_pPTPDevPathCtxMap.end(); ++MapItor)
        {

            UsbDev* ptpDevice_map = (*MapItor).first;//to check whether there is a first field
            if(ptpDevice_map == NULL)
			{
				continue;
			}

            if (ptpDevice_map->ptp_path.dev_num==ptpDevice->ptp_path.dev_num && ptpDevice_map->ptp_path.bus_num==ptpDevice->ptp_path.bus_num)
            {
                m_available_ids[ptpDevice_map->DeviceId - 16] = 0;
                m_pPTPDevPathCtxMap.erase (MapItor);
                break;
            }
        }

        PTP_DRV_ERR("\nMakeUsbDevStr ERROR\n");
        return -1; 			
    }
    PTP_API_Init_DeviceInfo( &deviceinfo );
    nResult = PtpCallbackGetDevInfo( &deviceinfo, (ptpDevice->ptp_path));

    if(nResult == PTP_RC_OK)
    {
        if(deviceinfo.Manufacturer)
        {
            strncpy(ptpDevice->vendor,deviceinfo.Manufacturer,(80-1));	
        }
        else	
        {
            strncpy (ptpDevice->vendor, PTP_DEFAULT_VENDOR_NAME,(80-1));			
        }

        if(deviceinfo.Model)
        {
            strncpy(ptpDevice->model, deviceinfo.Model,(80-1));
        }
        else	
        {
            //Panasonic DMC-FX12ŽÂ PTP DeviceInfo·Î  model(product)ží readœÃ NULL ž®ÅÏÇÏ¹Ç·Î ¿¹¿ÜÃ³ž®, by kks
            strncpy (ptpDevice->model, PTP_DEFAULT_MODEL_NAME,(80-1));			
        }

        if(deviceinfo.SerialNumber)
        {
            strncpy(ptpDevice->SerialNumber, deviceinfo.SerialNumber,(80-1));
        }
        else
        {
            strncpy (ptpDevice->SerialNumber,PTP_DEFAULT_SERIAL_NAME,(80-1));
        }


    }
    else
    {
        PTP_DRV_ERR("\nCamera connection error Please restart the camera\n"); 

        /*Erase from map*/
        for(devpath_iter MapItor = m_pPTPDevPathCtxMap.begin(); MapItor != m_pPTPDevPathCtxMap.end(); ++MapItor)
        {

            UsbDev* ptpDevice_map = (*MapItor).first;
		if(ptpDevice_map == NULL)
		{
			continue;
		}
            if (ptpDevice_map->ptp_path.dev_num==ptpDevice->ptp_path.dev_num && ptpDevice_map->ptp_path.bus_num==ptpDevice->ptp_path.bus_num)
            {
                m_available_ids[ptpDevice_map->DeviceId - 16] = 0;
                m_pPTPDevPathCtxMap.erase (MapItor);
                break;
            }
        }
        return -1;
    }
    PTP_API_Clear_DeviceInfo(&deviceinfo);
    PTP_API_Init_StorageIDs(&storageids);
    if( (PtpCallbackGetStorageIDs((ptpDevice->ptp_path),&storageids)) == PTP_RC_OK)
    {
        for(uint32_t i=0;i<storageids.n && i<2;i++)
        {	
            ptpDevice->storage_id[i]=storageids.Storage[i];

        }
        ptpDevice->sids = storageids;
        PTPStorageInfo 	storageinfo[2];

        for(uint32_t index=0;index<storageids.n && index<2;index++)
        {
            PTP_API_Init_PTPStorageInfo(&(storageinfo[index]));

            if(PtpCallbackGetStorageInfos(ptpDevice->ptp_path,storageids.Storage[index],&(storageinfo[index])) == PTP_RC_OK)
            {
                ptpDevice->storageinfo[index]=storageinfo[index];				
            }
            else
            {
                PTP_DRV_WARN("\nPtpCallbackGetStorageInfos failed\n");
            }		

            PTP_API_Clear_PTPStorageInfo(&storageinfo[index]);
        }		
    }
    else
    {

        PTP_DRV_ERR("\nMakeUsbDevStr ERROR\n");
        PTP_API_Clear_StorageIDs(&storageids);
        return -1;
    }
    PTP_API_Clear_StorageIDs(&storageids);
    return 0;	
}

/**
* @fn         	void CPTPDeviceListUpdate::GetPTPDeviceContext(PtpDevHndl ptpHandle, PTPDevContext** pDC)
* @brief         This function maps the ptpHandle of a ptp device to its DeviceContext
* @remarks       pDC is set to NULL if the device is not in the list
* @param         PtpDevHndl ptpHandle - ptpHandle of the camera  
* 				 PTPDevContext** pDC  -  Device Context of the camera
* @return        void
* @see           
*/

void CPTPDeviceListUpdate::GetPTPDeviceContext(PtpDevHndl ptpHandle, PTPDevContext** pDC)
{
    PTPDevContext *dc = NULL;
    for(devpath_iter MapItor = m_pPTPDevPathCtxMap.begin(); MapItor != m_pPTPDevPathCtxMap.end(); ++MapItor)
    {
        UsbDev* ptpDevice = (*MapItor).first;

        if ((ptpDevice->ptp_path.dev_num == ptpHandle.dev_num) && (ptpDevice->ptp_path.bus_num==ptpHandle.bus_num))
        {
            dc=(*MapItor).second;
            break;
        }	
    }
    *pDC=dc;

    return;
}

/**
* @fn         	void CPTPDeviceListUpdate :: FreeDeviceInstances(void)
* @brief         This function frees the device instances
* @param        void
* @return        void
* @see           
*/
void CPTPDeviceListUpdate :: FreeDeviceInstances(void)
{
    MUTEX_BEGIN;

    for(devpath_iter MapItor = m_pPTPDevPathCtxMap.begin(); MapItor != m_pPTPDevPathCtxMap.end();)
    {				
        UsbDev* ptpDevice_map = (*MapItor).first;//to check whether there is a first field
	/*Check for Valid Device Instance*/
	if((NULL != ptpDevice_map)&&(ptpDevice_map->DeviceId >= 16)&&(ptpDevice_map->DeviceId <= 31))
	{
		m_available_ids[ptpDevice_map->DeviceId - 16] = 0;
	}
        m_pPTPDevPathCtxMap.erase (MapItor);	

	  if(NULL != ptpDevice_map)
	  {
        	delete ptpDevice_map;					
	  }
	  MapItor = m_pPTPDevPathCtxMap.begin();
    }

    MUTEX_END;

}

/**
* @fn         CPTPDeviceListUpdate::PrintDeviceList( void )
* @brief         This function prints the device list in OnDeviceChange().
* @remarks       This function is used for checking connection & disconnection of PTP devices
* @param         void
* @return        void
* @see           
*/
void CPTPDeviceListUpdate::PrintDeviceList( void )
{
    uint32_t cnt;
    devpath_iter MapItor; 
  
    MUTEX_BEGIN;   
    cnt = 0; 
    printf("\n============ DEV LIST =================");

    for(MapItor = m_pPTPDevPathCtxMap.begin(); MapItor != m_pPTPDevPathCtxMap.end(); ++MapItor)
    {
        UsbDev *ptpDevice=(*MapItor).first;

        if (ptpDevice)
        {
            
	     printf("\n#%d. Device Man. %s \t" "Device Model. %s\t" " BusNum.%d  DevNum.%d", cnt++,  ptpDevice->vendor, ptpDevice->model,ptpDevice->ptp_path.bus_num,ptpDevice->ptp_path.dev_num );
        }
    }

    printf("\n======================================\n");
    MUTEX_END; 
    return;
}



#else
/**
* @fn         	void CPTPDeviceListUpdate::callback_on_device_change(struct usb_device* device, int isRemoved)
* @brief         callback_on_device_change.
* @remarks       
* @param         usb_device, device being connected/disconnected
*					isRemoved - 1 for removed; 0 for attached
* @return        void.
* @see           none
*/

void CPTPDeviceListUpdate::callback_on_device_change(struct usb_device* device, int isRemoved)
{
    CPTPDeviceListUpdate::Get().OnDeviceChange(device,(isRemoved == 1));
}                                                 


/**
* @fn         	void CPTPDeviceListUpdate::OnDeviceChange(struct usb_device* device, bool isRemoved)
* @brief         This function is called if there is any change in number of connected devices in libusb.
* @remarks       
* @param         usb_device* - device being attached or removed
*					isRemoved - 1 for removed; 0 for attached
* @return        void
* @see           none
*/

void CPTPDeviceListUpdate::OnDeviceChange(struct usb_device* device, bool isRemoved)
{

    // NULL check, because somtimes usbfs was crashed by on-chip usb device 
    if(device == NULL ||device->config == NULL 
        || device->config->interface == NULL 
        || device->config->interface->altsetting == NULL )
    {
        printf(" usb_device's address is NULL \n");
        return;
    }

    //A device is removed
    if (device->config->interface->altsetting->bInterfaceClass == USB_CLASS_PTP)
    {
        if(isRemoved == 1) 
        {        
            // extract the dc giving the dev & update both maps deleting corresponding dc.
            UsbDev ptpDevice;

            ptpDevice.ptp_path.bus_num=atoi(device->bus->dirname);
            ptpDevice.ptp_path.dev_num=(int)device->devnum;

            MUTEX_BEGIN;

            for(devpath_iter MapItor = m_pPTPDevPathCtxMap.begin(); MapItor != m_pPTPDevPathCtxMap.end(); ++MapItor)
            {

                UsbDev* ptpDevice_map = (*MapItor).first;//to check whether there is a first field

                if ((ptpDevice_map->ptp_path.dev_num == ptpDevice.ptp_path.dev_num) && (ptpDevice_map->ptp_path.bus_num == ptpDevice.ptp_path.bus_num))
                {
                    m_available_ids[ptpDevice_map->DeviceId - 16] = 0;
                    m_pPTPDevPathCtxMap.erase (MapItor);	
                    //PTP_API_Clear_PTPStorageInfo(&(ptpDevice_map->storageinfo[0]));
                    //PTP_API_Clear_StorageIDs(&(ptpDevice_map->sids));
                    delete ptpDevice_map;

                    break;
                }
            }
            MUTEX_END;

            ///////////////////////////////////////////////////
        }

        //A new device has arrived
        else 
        {
            // make a new dc giving the dev & update both maps adding corresponding dc.
            PTPDevContext* pNewDC = new PTPDevContext;

            pNewDC->dev = device;
            pNewDC->bus = device->bus;
            UsbDev *ptpDevice=new UsbDev;
            ptpDevice->ptp_path.bus_num=atoi(device->bus->dirname);
            ptpDevice->ptp_path.dev_num=(int)device->devnum;

            for (int i=0; i <16;i++)
            {
                if (m_available_ids[i] == 0)
                {
                    ptpDevice->DeviceId = i+16;
                    m_available_ids[i] = 1;
                    break;
                }
            }

            MUTEX_BEGIN;
            // Assign DevPath value...
            m_pPTPDevPathCtxMap[ptpDevice]= pNewDC;
            MUTEX_END;

            MakeUsbDevStr(ptpDevice);	//updete the fields of UsbDev *ptpdevice

        }			
        PrintDeviceList();	// for checking list of connected devices; can be removed if not required
    }
}

/**
* @fn         	void CPTPDeviceListUpdate::MakeUsbDevStr(UsbDev *ptpDevice)
* @brief         This function updates the fields of UsbDev structure in the map
* @remarks       
* @param         UsbDev *ptpDevice - pointer to the UsbDev structure which is being updated

* @return        void
* @see           none
*/

void CPTPDeviceListUpdate::MakeUsbDevStr(UsbDev *ptpDevice)
{
#define PTP_DEFAULT_MODEL_NAME		"Digital Camera"	
#define PTP_DEFAULT_VENDOR_NAME		"PTP Device"
#define PTP_DEFAULT_SERIAL_NAME		"123456789012345678901234567890"
    PTPDeviceInfo deviceinfo;
    PTPStorageIDs storageids;
    uint16 nResult;


    if ((nResult = PTP_API_INIT_COMM_MPTP (ptpDevice->ptp_path)) != PTP_RC_OK)
    {       
        //error handling		
        if (nResult==0X2ff || nResult== 0x2fd || nResult ==0x2fe)
        {
            printf("\ncamera could not be intialised due to IO error\nPlease restart the camera");
        }
        else 
        {
            printf("\nPTP_API_INIT_COMM_MPTP failed\n");
        }

        //erasing from map
        MUTEX_BEGIN;
        for(devpath_iter MapItor = m_pPTPDevPathCtxMap.begin(); MapItor != m_pPTPDevPathCtxMap.end(); ++MapItor)
        {

            UsbDev* ptpDevice_map = (*MapItor).first;//to check whether there is a first field
            if(ptpDevice_map == NULL)
            {
                continue;
            }

            if (ptpDevice_map->ptp_path.dev_num==ptpDevice->ptp_path.dev_num && ptpDevice_map->ptp_path.bus_num==ptpDevice->ptp_path.bus_num)
            {
                m_available_ids[ptpDevice_map->DeviceId - 16] = 0;
                m_pPTPDevPathCtxMap.erase (MapItor);
                break;
            }
        }
        MUTEX_END;
        return; 			
    }
    ////
    PTP_API_Init_DeviceInfo( &deviceinfo );
    nResult = PTP_API_Get_DeviceInfo_MPTP( &deviceinfo, (ptpDevice->ptp_path));

    if(nResult == PTP_RC_OK)
    {
        //printf( "Vendor = %s\n",deviceinfo.Manufacturer);
        if(deviceinfo.Manufacturer)
        {
            strcpy(ptpDevice->vendor,deviceinfo.Manufacturer);			
        }
        else	
        {
            strcpy (ptpDevice->vendor, PTP_DEFAULT_VENDOR_NAME);			
        }

        if(deviceinfo.Model)
        {
            strcpy(ptpDevice->model, deviceinfo.Model);
        }
        else	
        {
            //Panasonic DMC-FX12ŽÂ PTP DeviceInfo·Î  model(product)ží readœÃ NULL ž®ÅÏÇÏ¹Ç·Î ¿¹¿ÜÃ³ž®, by kks
            strcpy (ptpDevice->model, PTP_DEFAULT_MODEL_NAME);			
        }

        if(deviceinfo.SerialNumber)
        {
            strcpy(ptpDevice->SerialNumber, deviceinfo.SerialNumber);
        }
        else
        {
            strcpy (ptpDevice->SerialNumber,PTP_DEFAULT_SERIAL_NAME);
        }


    }
    else
    {
        printf("\n	PTP_API_Get_DeviceInfo_MPTP Failed\r\n");
        printf("\ncamera connection error \nplease restart the camera\n"); 

        // erase from map
        MUTEX_BEGIN;
        for(devpath_iter MapItor = m_pPTPDevPathCtxMap.begin(); MapItor != m_pPTPDevPathCtxMap.end(); ++MapItor)
        {

            UsbDev* ptpDevice_map = (*MapItor).first;
            if(ptpDevice_map == NULL)
            {
                continue;
            }

            if (ptpDevice_map->ptp_path.dev_num==ptpDevice->ptp_path.dev_num && ptpDevice_map->ptp_path.bus_num==ptpDevice->ptp_path.bus_num)
            {
                m_available_ids[ptpDevice_map->DeviceId - 16] = 0;
                m_pPTPDevPathCtxMap.erase (MapItor);
                break;
            }
        }
        MUTEX_END;
        return;
    }
    PTP_API_Clear_DeviceInfo(&deviceinfo);
    ////

    PTP_API_Init_StorageIDs(&storageids);

    if( (PTP_API_Get_StorageIDs_MPTP((ptpDevice->ptp_path),&storageids)) == PTP_RC_OK)
    {
        for(uint32_t i=0;i<storageids.n && i<2;i++)
        {	
            ptpDevice->storage_id[i]=storageids.Storage[i];

        }
        ptpDevice->sids = storageids;
        PTPStorageInfo 	storageinfo[2];

        for(uint32_t index=0;index<storageids.n && index<2;index++)
        {
            PTP_API_Init_PTPStorageInfo(&(storageinfo[index]));

            if(PTP_API_Get_StorageInfos_MPTP(ptpDevice->ptp_path,storageids.Storage[index],&(storageinfo[index])) == PTP_RC_OK)
            {
                ptpDevice->storageinfo[index]=storageinfo[index];				
            }
            else
            {
                printf("\n	PTP_API_Get_StorageInfos_MPTP failed for %x\r\n",storageids.Storage[index]);
            }		
        }		
    }
    else
    {
        printf("\n	PTP_API_Get_StorageIDs_MPTP Failed\r\n");
    }

    ////		
    return;	
}


/**
* @fn         	 void CPTPDeviceListUpdate::PrintDeviceList( void )
* @brief         This function prints the device list in OnDeviceChange().
* @remarks       This function is used for checking connection & disconnection of PTP devices
* @param         void
* @return        void
* @see           
*/

void CPTPDeviceListUpdate::PrintDeviceList( void )
{
	uint32_t cnt = 0;
	devpath_iter MapItor; 

	MUTEX_BEGIN;   

	printf("\n ================= DEV LIST =====================\n");

	for(MapItor = m_pPTPDevPathCtxMap.begin(); MapItor != m_pPTPDevPathCtxMap.end(); ++MapItor)
	{
		UsbDev *ptpDevice=(*MapItor).first;

		if (ptpDevice)
		{
			if(ptpDevice->vendor)
			{
				printf("#%d. Device Man. %s \t", cnt++,  ptpDevice->vendor );
			}

			if(ptpDevice->model)
			{
				printf("Device Model. %s", ptpDevice->model);
			}

			printf(" \tBusNum.%d  DevNum.%d",ptpDevice->ptp_path.bus_num,ptpDevice->ptp_path.dev_num);
			printf("\n");	
		}
	}

	MUTEX_END; 

	printf("======================================\n");
	return;
}

/**
* @fn         	 void CPTPDeviceListUpdate::GetPTPDeviceContext(PTPDeviceHandle ptpHandle, PTPDevContext** pDC)
* @brief         This function maps the ptpHandle of a ptp device to its DeviceContext
* @remarks       pDC is set to NULL if the device is not in the list
* @param         PtpDevHndl ptpHandle - ptpHandle of the camera  
* 				 PTPDevContext** pDC  -  Device Context of the camera
* @return        void
* @see           
*/

void CPTPDeviceListUpdate::GetPTPDeviceContext(PTPDeviceHandle ptpHandle, PTPDevContext** pDC)
{
    PTPDevContext *dc = NULL;

    MUTEX_BEGIN;

    for(devpath_iter MapItor = m_pPTPDevPathCtxMap.begin(); MapItor != m_pPTPDevPathCtxMap.end(); ++MapItor)
    {
        UsbDev* ptpDevice = (*MapItor).first;

        if ((ptpDevice->ptp_path.dev_num == ptpHandle.dev_num) && (ptpDevice->ptp_path.bus_num==ptpHandle.bus_num))
        {
            dc=(*MapItor).second;
            break;
        }	
    }

    MUTEX_END;

    *pDC=dc;

    return;
}

#endif

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <string.h>

#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_hidh_prim.h"
#include "csr_bt_platform.h"
#include "csr_bt_demoapp.h"

#define NOT_FOUND        0xFF
#define NVS_LIST_SIZE    10

typedef struct
{
    CsrBool          inUse;
    CsrBtDeviceAddr    bdAddr;
} deviceElement_t;

static deviceElement_t deviceList[NVS_LIST_SIZE];

/*************************************************************************************
  Nvs_FindInList:
************************************************************************************/
CsrUint8 Nvs_FindInList(CsrBtDeviceAddr *bdAddr)
{
    CsrUint8    i;

    for(i=0;i<NVS_LIST_SIZE;i++)
    {
        if( deviceList[i].inUse && CsrBtBdAddrEq(bdAddr, &deviceList[i].bdAddr) )
        {
            return i;
        }
    }
    return NOT_FOUND;
}

/*************************************************************************************
  Nvs_GetFreeListEntry:
************************************************************************************/
CsrUint8 Nvs_GetFreeListEntry(CsrBtDeviceAddr *bdAddr)
{
    CsrUint8    i;

    for(i=0;i<NVS_LIST_SIZE;i++)
    {
        if( deviceList[i].inUse == FALSE )
        {
            return i;
        }
    }
    return NOT_FOUND;
}

/*************************************************************************************
  Nvs_Init:
************************************************************************************/
void Nvs_Init(void)
{
    CsrFileHandle    *fp = NULL;

    CsrMemSet(&deviceList, 0, sizeof(deviceList));

    fp = CsrBtFopen("dev_list.hid", "rb");

    if( fp != NULL)
    {
        CsrBtFread(&deviceList, 1, sizeof(deviceList), fp);
        CsrBtFclose(fp);
    }
    else
    {
        CsrUint8    i;

        for(i=0;i<NVS_LIST_SIZE;i++)
        {
            deviceList[i].inUse = FALSE;
        }
    }
}

/*************************************************************************************
  Nvs_Flush:
************************************************************************************/
CsrBool Nvs_Flush(void)
{
    CsrFileHandle    *fp = NULL;

    fp = CsrBtFopen("dev_list.hid", "wb");

    if( fp != NULL)
    {
        CsrBtFwrite(&deviceList, 1, sizeof(deviceList), fp);
        CsrBtFclose(fp);
        return TRUE;
    }

    return FALSE;
}

/*************************************************************************************
  SaveHidDeviceInfo:
************************************************************************************/
CsrBool Nvs_PutHidInfo(CsrBtDeviceAddr *bdAddr,
                      CsrBtHidhSdpInfo *sdpInfo,
                      CsrCharString *serviceName,
                      CsrUint16 descLen,
                      CsrUint8 *desc)
{
    char    filename[20];
    CsrUint8    entry;
    CsrFileHandle    *fp = NULL;

    entry = Nvs_FindInList(bdAddr);
    if( entry == NOT_FOUND)
    {
        entry = Nvs_GetFreeListEntry(bdAddr);
    }

    if( entry != NOT_FOUND )
    {
        sprintf(filename, "dev%04x%02x%06x.hid", bdAddr->nap, bdAddr->uap, bdAddr->lap);
        fp = CsrBtFopen(filename, "wb");

        if(fp != NULL)
        {
            CsrBtFwrite( sdpInfo, 1, sizeof(CsrBtHidhSdpInfo), fp);

            CsrBtFwrite( &descLen, 1, sizeof(CsrUint16), fp );

            if((descLen != 0) && (desc != NULL) )
            {
                CsrBtFwrite( desc, 1, (size_t) descLen, fp);
            }

            if(serviceName != NULL)
            {
                CsrUint32 len = CsrStrLen((char *) serviceName);

                CsrBtFwrite( &len, 1, sizeof(CsrUint32), fp);
                CsrBtFwrite( serviceName, 1, len+1, fp);
            }

            CsrBtFclose(fp);

            CsrMemCpy(&deviceList[entry].bdAddr, bdAddr, sizeof(CsrBtDeviceAddr));
            deviceList[entry].inUse = TRUE;

            return TRUE;
        }
    }

    return FALSE;
}

/*************************************************************************************
  OpenHidDeviceInfo:
************************************************************************************/
CsrBool Nvs_GetHidInfo(CsrBtDeviceAddr *bdAddr,
                      CsrBtHidhSdpInfo **sdpInfo,
                      CsrCharString **serviceName,
                      CsrUint16 *descLen,
                      CsrUint8 **desc)
{
    char    filename[20];
    CsrInt16    read;
    CsrFileHandle    *fp = NULL;

    sprintf(filename, "dev%04x%02x%06x.hid", bdAddr->nap, bdAddr->uap, bdAddr->lap);

    fp = CsrBtFopen(filename, "rb");

    if(fp != NULL)
    {
        CsrUint32 len;

        *sdpInfo = (CsrBtHidhSdpInfo *) CsrPmemAlloc(sizeof(CsrBtHidhSdpInfo));
        CsrBtFread(*sdpInfo, 1, sizeof(CsrBtHidhSdpInfo), fp);
        if (((CsrBtHidhSdpInfo *)*sdpInfo)->isValid == FALSE)
        {
            CsrMemSet(*sdpInfo, 0, sizeof(CsrBtHidhSdpInfo));
        }

        CsrBtFread(descLen, 1, sizeof(CsrUint16), fp);
        if(*descLen != 0)
        {
            *desc = (CsrUint8*)CsrPmemAlloc(*descLen);
            read = CsrBtFread(*desc, 1, (size_t)(*descLen), fp);

            if( read != *descLen)
            {
                CsrPmemFree(*desc);
                CsrPmemFree(*sdpInfo);
                CsrBtFclose(fp);
                return FALSE;
            }
        }
        else
        {
            *descLen = 0;
            *desc = NULL;
        }

        read = CsrBtFread(&len, 1, sizeof(CsrUint32), fp);
        if(len != 0 && read != 0)
        {
            *serviceName = (CsrCharString *) CsrPmemAlloc(len+1);
            read = CsrBtFread(*serviceName, 1, (size_t) len+1, fp);
        }
        else
        {
            *serviceName = NULL;
        }

        CsrBtFclose(fp);

        return TRUE;
    }

    return FALSE;
}

/*************************************************************************************
  RemoveHidDeviceInfo:
************************************************************************************/
CsrBool Nvs_RemoveHidInfo(CsrBtDeviceAddr *bdAddr)
{
    char    filename[20];
    CsrUint8    entry;

    if( (entry = Nvs_FindInList(bdAddr)) != NOT_FOUND )
    {
        deviceList[entry].inUse = FALSE;
    }

    sprintf(filename, "dev%04x%02x%06x.hid", bdAddr->nap, bdAddr->uap, bdAddr->lap);

    if( CsrBtRemove(filename) != -1 )
    {
        return TRUE;
    }

    return FALSE;
}

/*************************************************************************************
  FindHidDeviceInfo:
************************************************************************************/
CsrUint8 Nvs_GetHidDeviceList(CsrUint8 maxDevices, CsrBtDeviceAddr *bdAddrList)
{
    CsrUint8    i, foundDevices = 0;

    for (i=0; (i<NVS_LIST_SIZE) && (foundDevices < maxDevices); i++)
    {
        if( deviceList[i].inUse)
        {
            CsrMemCpy(bdAddrList, &deviceList[i].bdAddr, sizeof(CsrBtDeviceAddr));
            bdAddrList++;
            foundDevices++;
        }
    }
    return foundDevices;
}


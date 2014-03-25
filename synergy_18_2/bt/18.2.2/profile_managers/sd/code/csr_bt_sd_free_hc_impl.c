/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

                All rights reserved and confidential information of CSR

REVISION:       $Revision: #2 $
***************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_SD_MODULE

#include "csr_pmem.h"
#include "csr_bt_sd_prim.h"

void CsrBtSdReadDeviceListCfmPrimFree(CsrBtSdPrim *prim)
{
    CsrUintFast32 i;
    CsrBtSdDeviceInfoType *t = NULL;
    CsrBtSdReadDeviceListCfm *p;

    p = (CsrBtSdReadDeviceListCfm*) prim;
    t = p->deviceInfo;
   
    for (i=0;i<p->deviceInfoCount;i++)
    {
        CsrPmemFree(t[i].info);
        t[i].info=NULL;
    }

    CsrPmemFree(p->deviceInfo);
    p->deviceInfo = NULL;
}

void CsrBtSdReadDeviceListIndPrimFree(CsrBtSdPrim *prim)
{
    CsrUintFast32 i;
    CsrBtSdDeviceInfoType *t = NULL;
    CsrBtSdReadDeviceListInd *p;

    p = (CsrBtSdReadDeviceListInd*) prim;

    t = p->deviceInfo;

    for (i=0;i<p->deviceInfoCount;i++)
    {
        CsrPmemFree(t[i].info);
        t[i].info=NULL;
    }

    CsrPmemFree(p->deviceInfo);
    p->deviceInfo = NULL;
}

#endif

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

                All rights reserved and confidential information of CSR

REVISION:       $Revision: #2 $
***************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_GATT_MODULE

#include "csr_pmem.h"
#include "csr_bt_gatt_prim.h"

void CsrBtGattDbAddReqPrimFree(CsrBtGattPrim *prim)
{
    CsrBtGattDbAddReq *p;
    CsrBtGattDb *db;
    CsrBtGattDb *dbNext;

    p = (CsrBtGattDbAddReq*) prim;
    db = p->db;

    while (db != NULL)
    {
        dbNext = db->next;
        CsrPmemFree(db->value);
        db->value = NULL;
        db->next = NULL;
        CsrPmemFree(db);
        db = dbNext;
    }
}

void CsrBtGattWriteReqPrimFree(CsrBtGattPrim *prim)
{
    CsrBtGattWriteReq *p;
    CsrUint32 i;

    p = (CsrBtGattWriteReq*) prim;

    for (i=0;i<p->attrWritePairsCount;i++)
    {
        CsrPmemFree(p->attrWritePairs[i].value);
        p->attrWritePairs[i].value=NULL;
    }

    CsrPmemFree(p->attrWritePairs);
    p->attrWritePairs = NULL;
}

void CsrBtGattScanReqPrimFree(CsrBtGattPrim *prim)
{
    CsrBtGattScanReq *p;
    p = (CsrBtGattScanReq*)prim;

    if(p->filterAddr != NULL)
    {
        CsrPmemFree(p->filterAddr);
        p->filterAddr = NULL;
    }
    if(p->filterData != NULL)
    {
        CsrUint8 i;
        for(i=0; i<p->filterDataCount; i++)
        {
            CsrPmemFree(p->filterData[i].data);
        }
        CsrPmemFree(p->filterData);
        p->filterData = NULL;
    }
}

void CsrBtGattDbAccessWriteIndPrimFree(CsrBtGattPrim *prim)
{
    CsrBtGattDbAccessWriteInd *p;
    CsrUint32 i;
    p = (CsrBtGattDbAccessWriteInd*)prim;

    for (i=0;i<p->writeUnitCount;i++)
    {
        CsrPmemFree(p->writeUnit[i].value);
        p->writeUnit[i].value=NULL;
    }

    CsrPmemFree(p->writeUnit);
    p->writeUnit = NULL;
}

#endif

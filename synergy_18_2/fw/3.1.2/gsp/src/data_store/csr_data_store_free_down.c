/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_DATA_STORE_MODULE
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_autogen.h"
#include "csr_data_store_lib.h"
#include "csr_data_store_prim.h"

void CsrDataStoreFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_DATA_STORE_PRIM)
    {
        CsrDataStorePrim *prim = (CsrDataStorePrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_REQ
            case CSR_DATA_STORE_DELETE_REQ:
            {
                CsrDataStoreDeleteReq *p = message;
                CsrPmemFree(p->storeName);
                p->storeName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_REQ */
#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ
            case CSR_DATA_STORE_RECORD_DELETE_REQ:
            {
                CsrDataStoreRecordDeleteReq *p = message;
                CsrPmemFree(p->key);
                p->key = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ */
#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_REQ
            case CSR_DATA_STORE_OPEN_REQ:
            {
                CsrDataStoreOpenReq *p = message;
                CsrPmemFree(p->storeName);
                p->storeName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_REQ */
#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_REQ
            case CSR_DATA_STORE_CREATE_REQ:
            {
                CsrDataStoreCreateReq *p = message;
                CsrPmemFree(p->storeName);
                p->storeName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_REQ */
#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ
            case CSR_DATA_STORE_RECORD_WRITE_REQ:
            {
                CsrDataStoreRecordWriteReq *p = message;
                CsrPmemFree(p->key);
                p->key = NULL;
                CsrPmemFree(p->record);
                p->record = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ */
#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ
            case CSR_DATA_STORE_RECORD_READ_REQ:
            {
                CsrDataStoreRecordReadReq *p = message;
                CsrPmemFree(p->key);
                p->key = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ */
            default:
            {
                break;
            }
        } /* End switch */
    } /* End if */
    else
    {
        /* Unknown primitive type, exception handling */
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_MODULE */

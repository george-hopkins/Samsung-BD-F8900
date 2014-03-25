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

void CsrDataStoreFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_DATA_STORE_PRIM)
    {
        CsrDataStorePrim *prim = (CsrDataStorePrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM
            case CSR_DATA_STORE_RECORD_READ_CFM:
            {
                CsrDataStoreRecordReadCfm *p = message;
                CsrPmemFree(p->key);
                p->key = NULL;
                CsrPmemFree(p->record);
                p->record = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM */
#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_CFM
            case CSR_DATA_STORE_DELETE_CFM:
            {
                CsrDataStoreDeleteCfm *p = message;
                CsrPmemFree(p->storeName);
                p->storeName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_CFM */
#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_CFM
            case CSR_DATA_STORE_CREATE_CFM:
            {
                CsrDataStoreCreateCfm *p = message;
                CsrPmemFree(p->storeName);
                p->storeName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_CFM */
#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM
            case CSR_DATA_STORE_RECORD_WRITE_CFM:
            {
                CsrDataStoreRecordWriteCfm *p = message;
                CsrPmemFree(p->key);
                p->key = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM */
#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_CFM
            case CSR_DATA_STORE_OPEN_CFM:
            {
                CsrDataStoreOpenCfm *p = message;
                CsrPmemFree(p->storeName);
                p->storeName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_CFM */
#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM
            case CSR_DATA_STORE_RECORD_DELETE_CFM:
            {
                CsrDataStoreRecordDeleteCfm *p = message;
                CsrPmemFree(p->key);
                p->key = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM */
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

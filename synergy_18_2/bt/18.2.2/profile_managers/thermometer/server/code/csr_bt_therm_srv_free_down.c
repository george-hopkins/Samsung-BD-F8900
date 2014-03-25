/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_therm_srv_lib.h"
#include "csr_bt_therm_srv_prim.h"

void CsrBtThermSrvFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_THERM_SRV_PRIM)
    {
        CsrBtThermSrvPrim *prim = (CsrBtThermSrvPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ
            case CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ:
            {
                CsrBtThermSrvUpdateTemperatureReq *p = message;
                CsrPmemFree(p->tempData);
                p->tempData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ
            case CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ:
            {
                CsrBtThermSrvUpdateHealthInfoReq *p = message;
                CsrPmemFree(p->info);
                p->info = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ
            case CSR_BT_THERM_SRV_ACTIVATE_REQ:
            {
                CsrBtThermSrvActivateReq *p = message;
                CsrPmemFree(p->clientConfig);
                p->clientConfig = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ */
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
#endif /* EXCLUDE_CSR_BT_THERM_SRV_MODULE */

/****************************************************************************

                (c) CSR plc 2010

                All rights reserved

FILE:           sdp_free_upstream.c

DESCRIPTION:    Helper function for freeing SDP primitives.



****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "csr_sched.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_tasks.h"
#include "sds_prim.h"
#include "bluestacklib.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      sdp_free_upstream_primitive
 *
 *  DESCRIPTION
 *      Free upstream SDP primitives, including any referenced memory.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void sdp_free_upstream_primitive(
    SDS_UPRIM_T *p_uprim
    )
{
    if (!p_uprim)
        return;

    switch (p_uprim->type)
    {
        case SDC_SERVICE_SEARCH_CFM:
            {
                SDC_SERVICE_SEARCH_CFM_T *prim;

                prim = (SDC_SERVICE_SEARCH_CFM_T *) p_uprim;
                CsrPmemFree(prim->rec_list);
                CsrPmemFree(prim->err_info);
                break;
            }
        case SDC_SERVICE_ATTRIBUTE_CFM:
            {
                SDC_SERVICE_ATTRIBUTE_CFM_T *prim;

                prim = (SDC_SERVICE_ATTRIBUTE_CFM_T *) p_uprim;
                CsrPmemFree(prim->attr_list);
                CsrPmemFree(prim->err_info);
                break;
            }
        case SDC_SERVICE_SEARCH_ATTRIBUTE_CFM:
            {
                SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_T *prim;

                prim = (SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_T *) p_uprim;
                CsrPmemFree(prim->attr_list);
                CsrPmemFree(prim->err_info);
                break;
            }
        default:
            {
                break;
            }
    }

    CsrPmemFree(p_uprim);
}


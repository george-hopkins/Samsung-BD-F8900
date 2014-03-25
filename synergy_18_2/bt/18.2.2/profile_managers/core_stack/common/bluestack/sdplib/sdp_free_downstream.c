/****************************************************************************

                (c) CSR plc 2010

                All rights reserved

FILE:           sdp_free_downstream.c

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
 *      sdp_free_downstream_primitive
 *
 *  DESCRIPTION
 *      Free downstream SDP primitives, including any referenced memory.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void sdp_free_downstream_primitive(SDS_UPRIM_T *p_uprim)
{
    if (!p_uprim)
        return;

    switch (p_uprim->type)
    {
        case SDC_SERVICE_SEARCH_REQ:
            {
                SDC_SERVICE_SEARCH_REQ_T *prim;

                prim = (SDC_SERVICE_SEARCH_REQ_T *) p_uprim;
                CsrPmemFree(prim->srch_pttrn);
                break;
            }
        case SDC_SERVICE_ATTRIBUTE_REQ:
            {
                SDC_SERVICE_ATTRIBUTE_REQ_T *prim;

                prim = (SDC_SERVICE_ATTRIBUTE_REQ_T *) p_uprim;
                CsrPmemFree(prim->attr_list);
                break;
            }
        case SDC_SERVICE_SEARCH_ATTRIBUTE_REQ:
            {
                SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *prim;

                prim = (SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *) p_uprim;
                CsrPmemFree(prim->attr_list);
                CsrPmemFree(prim->srch_pttrn);
                break;
            }
        case SDS_REGISTER_REQ:
            {
                SDS_REGISTER_REQ_T *prim;

                prim = (SDS_REGISTER_REQ_T *) p_uprim;
                CsrPmemFree(prim->service_rec);
                break;
            }
        default:
            {
                break;
            }
    }

    CsrPmemFree(p_uprim);
}


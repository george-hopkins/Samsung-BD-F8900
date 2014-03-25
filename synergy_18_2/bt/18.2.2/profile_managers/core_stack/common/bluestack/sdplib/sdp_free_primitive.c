/****************************************************************************

                (c) CSR plc 2010

                All rights reserved

FILE:           sdp_free_primitive.c

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
 *      sdp_free_primitive
 *
 *  DESCRIPTION
 *      Free downstream and upstream SDP primitives, including any referenced memory.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void sdp_free_primitive(
    SDS_UPRIM_T *p_uprim
    )
{
    if (!p_uprim)
        return;

    switch (p_uprim->type)
    {
        case SDC_SERVICE_SEARCH_REQ:
        case SDC_SERVICE_ATTRIBUTE_REQ:
        case SDC_SERVICE_SEARCH_ATTRIBUTE_REQ:
        case SDS_REGISTER_REQ:
            {
                sdp_free_downstream_primitive(p_uprim);
                break;
            }
        case SDC_SERVICE_SEARCH_CFM:
        case SDC_SERVICE_ATTRIBUTE_CFM:
        case SDC_SERVICE_SEARCH_ATTRIBUTE_CFM:
            {
                sdp_free_upstream_primitive(p_uprim);
                break;
            }

        default:
            {
                CsrPmemFree(p_uprim);
                break;
            }
    }
}


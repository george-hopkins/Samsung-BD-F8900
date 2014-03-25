/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_free.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_free
 *
 *  DESCRIPTION
 *      Free an ATT primitive and all data asocciated to it.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_free(
    ATT_UPRIM_T *p_prim
    )
{
    if (p_prim == NULL)
    {
        return;
    }

    /* Free data associated to the primitive */
    switch (p_prim->type)
    {
        case ATT_ADD_DB_REQ:
            CsrPmemFree(p_prim->att_add_db_req.db);
            break;

#ifdef BUILD_FOR_HOST
        case ATT_ADD_REQ:
        {
            att_attr_t *p;
            att_attr_t **pp;

            for (pp = &p_prim->att_add_req.attrs; (p = *pp) != NULL; /* nop */)
            {
                *pp = p->next;
                if (p->value)
                {
                    CsrPmemFree(p->value);
                }
                CsrPmemFree(p);
            }

            break;
        }
#endif

        case ATT_FIND_BY_TYPE_VALUE_REQ:
            CsrPmemFree(p_prim->att_find_by_type_value_req.value);
            break;

        case ATT_READ_BY_TYPE_CFM:
            CsrPmemFree(p_prim->att_read_by_type_cfm.value);
            break;

        case ATT_READ_CFM:
            CsrPmemFree(p_prim->att_read_cfm.value);
            break;

        case ATT_READ_BLOB_CFM:
            CsrPmemFree(p_prim->att_read_blob_cfm.value);
            break;

        case ATT_READ_MULTI_REQ:
            CsrPmemFree(p_prim->att_read_multi_req.handles);
            break;

        case ATT_READ_MULTI_CFM:
            CsrPmemFree(p_prim->att_read_multi_cfm.value);
            break;

        case ATT_READ_BY_GROUP_TYPE_CFM:
            CsrPmemFree(p_prim->att_read_by_group_type_cfm.value);
            break;

        case ATT_WRITE_REQ:
            CsrPmemFree(p_prim->att_write_req.value);
            break;

        case ATT_PREPARE_WRITE_REQ:
            CsrPmemFree(p_prim->att_prepare_write_req.value);
            break;

        case ATT_PREPARE_WRITE_CFM:
            CsrPmemFree(p_prim->att_prepare_write_cfm.value);
            break;

        case ATT_HANDLE_VALUE_REQ:
            CsrPmemFree(p_prim->att_handle_value_req.value);
            break;

        case ATT_HANDLE_VALUE_IND:
            CsrPmemFree(p_prim->att_handle_value_ind.value);
            break;

        case ATT_ACCESS_IND:
            CsrPmemFree(p_prim->att_access_ind.value);
            break;

        case ATT_ACCESS_RSP:
            CsrPmemFree(p_prim->att_access_rsp.value);
            break;

        case ATT_DEBUG_IND:
            CsrPmemFree(p_prim->att_debug_ind.debug);
            break;

        default:
            break;
    }

    CsrPmemFree(p_prim);
}

#endif /* INSTALL_ATT_MODULE */

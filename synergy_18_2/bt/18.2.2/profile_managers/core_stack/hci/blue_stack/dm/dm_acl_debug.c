/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_acl_debug.c

\brief  Debugging utilities for ACL Manager.
*/

#include "csr_synergy.h"
#include "csr_bt_bluestack_types.h" /* path modified */
#include "bluetooth.h" /* path modified */
#include "dm_prim.h"


#include "error.h"
#include "dm_acl_core.h"
#include "dm_acl_connection_fsm_definitions.h"

#ifdef DM_ACL_DEBUG
const DM_ACL_T *p_panic_acl;
void dm_acl_debug_check(char *file, CsrUint16 line, const DM_ACL_T *p_acl_check, CsrBool allow_dead)
{
    if (p_acl_check != NULL)
    {
        DM_ACL_T *p_acl;

        /* Make sure that ACL record is in list. */
        for (p_acl = p_acl_list; p_acl != NULL && p_acl != p_acl_check;
                p_acl = p_acl->p_next)
            ;

        if (p_acl != NULL && p_acl->p_next != p_acl)
        {
            /* Check consistency */
            switch (p_acl_check->sanity)
            {
                case 0xBEEF:
                    return;

                case 0xDEAD:
                    if (allow_dead && p_acl_check->connection_state == ACL_ST_IDLE)
                        return;
                    break;
            }
        }

        p_panic_acl = p_acl_check;
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
    }
}

void *dm_acl_client_get_data(char *file, CsrUint16 line, DM_ACL_T *p_acl, void *client)
{
    dm_acl_debug_check(file, line, p_acl, FALSE);
    return client;
}

DM_ACL_T * dm_acl_client_get_acl(char *file, CsrUint16 line, void *client, CsrUint16 offset)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)(((CsrUint8*)client) - offset);

    if (client == NULL || p_acl == NULL)
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);

    dm_acl_debug_check(file, line, p_acl, FALSE);

    return p_acl;
}
#endif

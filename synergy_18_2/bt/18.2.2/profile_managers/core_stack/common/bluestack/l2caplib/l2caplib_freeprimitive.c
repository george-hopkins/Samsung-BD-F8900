/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_FreePrimitive.c

\brief  L2CAP access library - provides functions for building and
        sending downstream L2CAP primitives.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_mblk.h"
#include "csr_bt_common.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Discard a configuration block

    Free pointers from a configuration placeholder.
 */
void L2CA_FreeConfigPtrs(L2CA_CONFIG_T *config)
{
    if (config != NULL)
    {
        bpfree(config->unknown);
        bpfree(config->qos);
        bpfree(config->flow);
        bpfree(config->flowspec);

        config->unknown = NULL;
        config->unknown_length = 0;
        config->qos = NULL;
        config->flow = NULL;
        config->flowspec = NULL;
    }
}

/*! \brief Destroy L2CAP primitive

    Destroy an L2CAP primtive including any associated MBLKs or data blocks.

    \param prim Pointer to L2CAP primitive to destroy.
*/
void L2CA_FreePrimitive(L2CA_UPRIM_T *prim)
{
    if (prim == NULL)
        return;

    /* Handle any special cases. We must use bpree() in downstream
     * signals to deal with VM const data. On-host this is macrofied
     * to a normal CsrPmemFree() */
    switch(prim->type)
    {
        case L2CA_DATAWRITE_REQ:
            CsrMblkDestroy(prim->l2ca_datawrite_req.data);
            break;

        case L2CA_DATAREAD_IND:
            CsrMblkDestroy(prim->l2ca_dataread_ind.data);
            break;

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
        case L2CA_CONFIG_REQ:
            L2CA_FreeConfigPtrs(&prim->l2ca_config_req.config);
            break;

        case L2CA_CONFIG_RSP:
            L2CA_FreeConfigPtrs(&prim->l2ca_config_rsp.config);
            break;

        case L2CA_CONFIG_CFM:
            L2CA_FreeConfigPtrs(&prim->l2ca_config_cfm.config);
            break;

        case L2CA_CONFIG_IND:
            L2CA_FreeConfigPtrs(&prim->l2ca_config_ind.config);
            break;
#endif

#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
        case L2CA_MULTICAST_REQ:
            CsrMblkDestroy(prim->l2ca_multicast_req.data);
            break;
#endif

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
        case L2CA_PING_REQ:
            bpfree(prim->l2ca_ping_req.data);
            break;

        case L2CA_PING_CFM:
            CsrPmemFree(prim->l2ca_ping_cfm.data);
            break;

        case L2CA_GETINFO_CFM:
            CsrPmemFree(prim->l2ca_getinfo_cfm.info_data);
            break;
#endif
        case L2CA_RAW_DATA_REQ:
            CsrMblkDestroy(prim->l2ca_raw_data_req.data);
            break;

        case L2CA_RAW_DATA_IND:
            CsrMblkDestroy(prim->l2ca_raw_data_ind.data);
            break;

#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
        case L2CA_REGISTER_FIXED_CID_REQ:
            L2CA_FreeConfigPtrs(&prim->l2ca_register_fixed_cid_req.config);
            break;
#endif /* FIXED_CHANNEL_SUPPORT */

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
        case L2CA_AUTO_CONNECT_REQ:
            bpfree(prim->l2ca_auto_connect_req.conftab);
            break;

        case L2CA_AUTO_CONNECT_RSP:
            bpfree(prim->l2ca_auto_connect_rsp.conftab);
            break;

        case L2CA_AUTO_CONNECT_CFM:
            L2CA_FreeConfigPtrs(&prim->l2ca_auto_connect_cfm.config);
            break;
#endif
    }

    /* Free primitive memory block */
    CsrPmemFree(prim);
}

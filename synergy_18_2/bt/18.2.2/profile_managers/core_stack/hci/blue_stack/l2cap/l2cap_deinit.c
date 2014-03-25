/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_deinit.c

\brief  This file handles provides shutdown and cleanup capabilities
        for L2CAP. It's generally only used when L2CAP runs on a
        host-processor.
*/

#include "csr_synergy.h"

#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "csr_bt_common.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_mblk.h"

#ifdef ENABLE_SHUTDOWN

#include "l2cap_common.h"
#include "l2cap_signal.h"
#include "l2cap_chme.h"
#include "l2cap_con_handle.h"
#include "l2cap_sig_handle.h"
#include "l2cap_cid.h"
#include "l2cap_cidme.h"
#include "l2cap_control.h"
#include "l2cap_deinit.h"
#ifdef INSTALL_AMP_SUPPORT
#include "l2cap_amp_utils.h"
#endif
#include "l2cap_auto.h"
#include "dmlib.h"
#include "error.h"
#include "l2caplib.h"

/*! \brief CIDCB kill function

    Helper function to silently kill any CIDCB without notifying
    anyone about it.
*/
static void L2CA_KillCidcb(CIDCB_T *cidcb)
{
    CH_FlushCidcbData(cidcb->chcb, cidcb, L2CA_DATA_SILENT); /* be silent */
    CIDME_FreeCidcb(cidcb);
    cidcb = NULL;
}

/*! \brief Called from the ACL Manager while it is deinitialising.

    L2CAP must clean up the chcb before the ACL Manager can free it.
    Free the dynamic members of the CHCB. The pointer itself will be
    freed in the DM.

    \param chcb Pointer to connection handle control block.
*/
void dm_acl_client_deinit_l2cap(L2CAP_CHCB_T *chcb)
{
    CIDCB_T *cidcb;

    /* Free list of CIDCBs */
    while(chcb->cidcb_list != NULL)
    {
        cidcb = chcb->cidcb_list->next_ptr;
        L2CA_KillCidcb(chcb->cidcb_list);
        chcb->cidcb_list = cidcb;
    }

    /* Flush the remaining bits using the common function */
    (void)CH_FlushChcb(chcb, L2CA_DATA_SILENT); /* be silent */
}

/*! \brief L2CAP deinitialisation

    The is the L2CAP deinit function that gets called by the scheduler
    when the L2CAP task must shut down. Note that the freeing of CHCBs
    and related bits take place in the dm_acl_client_deinit_l2cap().
*/
void L2CAP_Deinit(void **gash)
{
    CsrUint16 mi;
    L2CA_UPRIM_T *l2cap_prim;

    PARAM_UNUSED(gash);

    /* Free scheduler queue */
    while(CsrSchedMessageGet(&mi, (void**)&l2cap_prim))
    {
        if(mi == DM_PRIM)
        {
            dm_free_upstream_primitive((DM_UPRIM_T*)l2cap_prim);
        }
        else if((mi == 0) || (mi == L2CAP_PRIM))
        {
            L2CA_FreePrimitive(l2cap_prim);
        }
#ifdef BUILD_FOR_HOST
        else if (mi == CSR_SCHED_PRIM)
        {
            CsrPmemFree(l2cap_prim);
        }
#endif
        else
        {
            BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_BLUESTACK_PRIMITIVE);
        }
    }

#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
    /* Free fixed channels */
    while(mcb.fixcid_list != NULL)
    {
        FIXCID_T *next = mcb.fixcid_list->next_ptr;
        CsrPmemFree(mcb.fixcid_list->config.unknown);
        CsrPmemFree(mcb.fixcid_list->config.qos);
        CsrPmemFree(mcb.fixcid_list->config.flow);
        CsrPmemFree(mcb.fixcid_list->config.flowspec);
        CsrPmemFree(mcb.fixcid_list);
        mcb.fixcid_list = next;
    }
#endif

    /* Autoconnect instances. The deinit function will free the entire
     * list */
    if(mcb.autoconnect != NULL)
    {
        L2CAUTO_Deinit();
        mcb.autoconnect = NULL;
    }

    /* Free PSMs */
    while(mcb.psm_list != NULL)
    {
        PSM_T *next = mcb.psm_list->next_ptr;
        CsrPmemFree(mcb.psm_list);
        mcb.psm_list = next;
    }

    /* Free retry_cid_fifo */
    while(mcb.retry_cid_fifo != NULL)
        (void)MCB_RetryCidFifoPop();

    /* Free connectionless CHCB */
#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
    if(mcb.connectionless != NULL)
    {
        CIDCB_T *cidcb;

        /* Free list of CIDCBs */
        while(mcb.connectionless->cidcb_list != NULL)
        {
            cidcb = mcb.connectionless->cidcb_list->next_ptr;
            L2CA_KillCidcb(mcb.connectionless->cidcb_list);
            mcb.connectionless->cidcb_list = cidcb;
        }

        /* Free the CHCB */
        (void)CH_FlushChcb(mcb.connectionless, L2CA_DATA_SILENT);/* be silent */
        CsrPmemFree(mcb.connectionless);
        mcb.connectionless = NULL;
    }
#endif
}

#endif /* ENABLE_SHUTDOWN */

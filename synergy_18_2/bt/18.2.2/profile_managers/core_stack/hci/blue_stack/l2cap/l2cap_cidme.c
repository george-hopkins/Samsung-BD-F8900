/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_cidme.c

\brief  This file handles L2CAP channel managment.
*/

#include "csr_synergy.h"

#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "csr_bt_common.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_sched.h"

#include "l2cap_common.h"
#include "l2cap_config.h"
#include "l2cap_cid.h"
#include "l2cap_cidme.h"
#include "l2cap_cid_fsm_definitions.h"
#include "l2cap_control.h"
#include "l2cap_interface.h"
#include "l2cap_con_handle.h"
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
#include "l2cap_flow.h"
#endif
#ifdef INSTALL_AMP_SUPPORT
#include "l2cap_amp_utils.h"
#endif
#include "l2cap_auto.h"

/*! \brief Allocate CIDCB instance.

    This function attempts to allocate new CIDCB instance, this can fail if
    we have reached the CID instance limit, or we are out of memory.  The new
    CID is stored in the CIDCB structure.
*/
static CIDCB_T *CIDME_AllocCidcb(void)
{
    l2ca_cid_t index;

    /* Scan through CID allocation map */
    for (index = 0; index < L2CAP_MAX_NUM_CIDS; index++)
    {
        /* Check if this entry is free */
        if (MCB_CidIsAvailable(index))
        {
            /* Allocate new CIDCB instance */
            CIDCB_T *cidcb = xzpnew(CIDCB_T);
            if (cidcb != NULL)
            {
                CsrUint16 cid;

                /* The CID is composed of an incrementing counter (high 10
                 * bits) and an index into the channel array (low 6 bits).
                 * We makes sure that the cid we produce is in the dynamic
                 * connection-oriented range of 0x0040 - 0xFF40.
                 * If streams are installed then we make sure they aren't
                 * already using this channel. */
#if !defined(BUILD_FOR_HOST) && \
     defined(INSTALL_STREAM_MODULE) && \
    !defined(DISABLE_L2CAP_CONNECTION_FSM_SUPPORT)
#define L2CA_CID_IN_USE(cid) (stream_l2cap_get(cid) != NULL)
#else
#define L2CA_CID_IN_USE(cid) FALSE
#endif
                do
                {
                    cid = (CsrUint16)(++mcb.cid_counter &
                                    (0xFFFF >> (L2CAP_CID_RANDOM_LSH + 1)));
                    cid = (CsrUint16)((cid + 1) << L2CAP_CID_RANDOM_LSH);
                    cid |= index;
                } while (L2CA_CID_IN_USE(cid));

                /* Store entry in table */
                mcb.cid_table[index] = cidcb;

                /* Initialise local CID */
                cidcb->local_cid = (l2ca_cid_t)(cid);
                cidcb->p_handle = CSR_SCHED_QID_INVALID;
#ifdef INSTALL_L2CAP_ROUTER_SUPPORT
                cidcb->p_handle_data = CSR_SCHED_QID_INVALID;
#endif

                /* Allow retry on first attempt. */
                cidcb->allow_retry = TRUE;
            }

            /* Return pointer to CIDCB or NULL */
            return cidcb;
        }
    }

    /* Can't create new CID, so return NULL */
    return NULL;
}

/*! \brief Create new channel CIDCB instance.

    Allocate a new CIDCB structure and return a pointer to it.
*/
CIDCB_T *CIDME_NewCidcb(l2ca_cid_t local_cid)
{
    CIDCB_T *cidcb;

    if (local_cid == L2CA_CID_INVALID)
    {
        /* New connection, so allocate new CID. */
        cidcb = CIDME_AllocCidcb();
    }
    else if ((cidcb = xzpnew(CIDCB_T)) != NULL)
    {
        /* Retry, so preserve existing CID. */
        mcb.cid_table[local_cid & L2CAP_CID_NUMBER_MASK] = cidcb;
        cidcb->local_cid = local_cid;
    }
    else
    {
        cidcb = NULL;
    }

    return cidcb;
}


/*! \brief Initialise basic members in a CIDCB structure

    This function initialises all common members in a CIDCB structure.
    Note that the alloc function zeros the structure, so there's no
    need to initialise variables to NULL/false/0.
*/
void CIDME_InitCidcb(CIDCB_T *cidcb,
                     L2CAP_CHCB_T *chcb,
                     l2ca_cid_t remote_cid,
                     struct psm_tag_t *psm_reg,
                     psm_t remote_psm,
                     CsrUint16 context)
{
    cidcb->remote_cid     = remote_cid;

    if ((cidcb->local_psm = psm_reg) != NULL)
        cidcb->p_handle = psm_reg->p_handle;

#ifdef INSTALL_L2CAP_ROUTER_SUPPORT
    cidcb->p_handle_data  = cidcb->p_handle;
#endif
    cidcb->remote_psm     = remote_psm;
    cidcb->chcb           = chcb;
    cidcb->context        = context;
    cidcb->local_mtu      = L2CA_MTU_DEFAULT;
    cidcb->remote_mtu     = L2CA_MTU_DEFAULT;
    cidcb->local_flush_to = FLUSH_INFINITE_TIMEOUT;
    cidcb->state          = CID_ST_NULL;

    /* Don't allow L2CAP retry for new ACLs */
    if (!CH_IS_CONNECTED(cidcb->chcb))
        cidcb->allow_retry = FALSE;
}

/*! \brief Unroute data for CIDCB

    When closing CIDCBs normally, we need to check whether the data
    was routed, and if so, inform the handler that it's no longer
    routed
*/
#ifdef INSTALL_L2CAP_ROUTER_SUPPORT
void CIDME_Unroute(CIDCB_T *cidcb)
{
    /* Check if there is a seperate phandle processing data, if so we
     * should inform them that the channel is dead */
    if (cidcb->p_handle != cidcb->p_handle_data)
    {
        L2CA_UnrouteDataInd(cidcb);

        /* Restore phandle */
        cidcb->p_handle_data = cidcb->p_handle;
    }
}
#endif

/*! \brief Free CID instance

    This function frees up a CID instance. Caller must make sure that
    any queued data blocks or signals have been freed before hand.
*/
void CIDME_FreeCidcb(CIDCB_T *cidcb)
{
    L2CAP_CHCB_T *chcb_ptr;
    CIDCB_T *cidcb_ptr;
    CIDCB_T **cidcb_ptr_ptr;

    if ((chcb_ptr = cidcb->chcb) != NULL)
    {
        /* Remove CIDCB from list */
        for (cidcb_ptr_ptr = &chcb_ptr->cidcb_list;
             (cidcb_ptr = *cidcb_ptr_ptr) != NULL;
             cidcb_ptr_ptr = &cidcb_ptr->next_ptr)
        {
            /* Check if we have found instance to remove */
            if (cidcb_ptr == cidcb)
            {
                /* Remove instance from list and exit loop */
                *cidcb_ptr_ptr = cidcb_ptr->next_ptr;
                break;
            }
        }
    }

    /* Remove from table */
    mcb.cid_table[cidcb->local_cid & L2CAP_CID_NUMBER_MASK] = NULL;

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
    /* Free configuration containers */
    timer_cancel(&cidcb->config_timer_id);
    if(cidcb->confbuf != NULL)
    {
        L2CAP_CONFIG_BUFFERENUM_T cb;
        /* xapncc ignore '=': cast of 'int' to differing enum */
        for (cb = CONFBUF_MIN; cb != CONFBUF_ALL; ++cb)
            CsrPmemFree(cidcb->confbuf->buffer[cb].buf);

        CsrPmemFree(cidcb->confbuf);
    }
#endif

    CsrPmemFree(cidcb->dm_sm_service);

    /* Flow control instance */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    if(CID_IsFlowControl(cidcb))
    {
        FLOW_Free(cidcb, L2CA_DATA_SILENT);
    }
#endif

    /* Flowspecs */
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
    /* Free flowspecs */
    if(cidcb->tx_flowspec)
    {
        CsrPmemFree(cidcb->tx_flowspec);
    }
    if(cidcb->rx_flowspec)
    {
        CsrPmemFree(cidcb->rx_flowspec);
    }
#endif

    /* AMP bits */
#ifdef INSTALL_AMP_SUPPORT
    AMP_PruneQueues(cidcb->chcb);
    AMP_FreeInfo(cidcb);
#endif

    /* Auto connect */
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
    L2CAUTO_Free(NULL, cidcb->local_cid);
#endif

    /* Free CIDCB structure */
#ifdef INSTALL_L2CAP_DEBUG
    CsrMemSet(cidcb, 0, sizeof(CIDCB_T));
#endif
    CsrPmemFree(cidcb);
}

/*! \brief Get CID instance via the local CID

    Search the list of CIDs and return the one with the matching local
    CID number.
*/
CIDCB_T *CIDME_GetCidcb(l2ca_cid_t cid)
{
    /* Remove random top bits */
    l2ca_cid_t cidtab = cid & L2CAP_CID_NUMBER_MASK;

    /* Check CID is within range */
    if(cidtab < L2CAP_MAX_NUM_CIDS)
    {
        /* Look into CID table */
        CIDCB_T *cidcb = mcb.cid_table[cidtab];

        /* Check that the CID exists and that the full number matches */
        if((cidcb != NULL) &&
           (cidcb->local_cid == cid))
        {
            return cidcb;
        }
    }

    /* Match not found or CID out of range */
    return NULL;
}

/*! \brief Get CID instance using the connection context

    Scan the list of CIDCBs and return the one with matching
    context.
*/
CIDCB_T *CIDME_GetCidcbWithContext(CsrUint16 context)
{
    l2ca_cid_t index;

    /* Scan list */
    for(index = 0; index < L2CAP_MAX_NUM_CIDS; index++)
    {
        CIDCB_T *cidcb = mcb.cid_table[index];

        /* Return pointer if context matches */
        if((cidcb != NULL) &&
           (cidcb->context == context))
        {
            return cidcb;
        }
    }

    /* Match not found */
    return NULL;
}

/*! \brief Get CID instance via the CHCB handle

    Obtain CID instance via local CID and connection handle.
*/
struct cidtag *CIDME_GetCidcbWithHandle(const L2CAP_CHCB_T *chcb, l2ca_cid_t cid)
{
    /* Remove random top bits */
    l2ca_cid_t cidtab = cid & L2CAP_CID_NUMBER_MASK;

    /* Check CID is within range */
    if(cidtab < L2CAP_MAX_NUM_CIDS)
    {
        /* Look into CID table */
        CIDCB_T *cidcb = mcb.cid_table[cidtab];

        /* Check we have valid CIDCB and it's owned by the right
         * CHCB. And that the full CID number matches! */
        if((cidcb != NULL) &&
           (cidcb->chcb == chcb) &&
           (cidcb->local_cid == cid))
        {
            return cidcb;
        }
    }

    /* Match not found or CID out of range */
    return NULL;
}

/*! \brief Get CID instance from CHCB list with remote CID

    Obtain CID instance via remote CID and connection handle.
*/
struct cidtag *CIDME_GetCidcbRemoteWithHandle(const L2CAP_CHCB_T *chcb, l2ca_cid_t remote_cid)
{
    return CIDME_GetCLCidcbRemote(chcb, remote_cid, L2CA_PSM_INVALID);
}

struct cidtag *CIDME_GetCLCidcbRemote(const L2CAP_CHCB_T *chcb, l2ca_cid_t remote_cid, psm_t cl_psm)
{
    CIDCB_T *cidcb;

    for (cidcb = chcb->cidcb_list;
            cidcb != NULL
                && (cidcb->remote_cid != remote_cid ||
                    (cl_psm != L2CA_PSM_INVALID &&
                     (cidcb->local_psm == NULL ||
                      cidcb->local_psm->psm != cl_psm)));
            cidcb = cidcb->next_ptr)
        ;

    return cidcb;
}

CsrUint16 CIDME_RegistrationContext(const CIDCB_T *cidcb)
{
    if (cidcb == NULL || cidcb->local_psm == NULL)
        return 0;

    return cidcb->local_psm->reg_ctx;
}

psm_t CIDME_LocalPsm(const CIDCB_T *cidcb)
{
    if (cidcb == NULL || cidcb->local_psm == NULL)
        return 0;

    return cidcb->local_psm->psm;
}

/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_auto.c

\brief  This file implements the L2CAP automatic connect/configuration

        L2CAP auto-connect is a helper system that can do connect and
        configuration in a single step as seen from the upper
        layer. The upper layer simply provides a set of
        key,value-pairs with the acceptable parameters and L2CAP will
        then do the full connect and configuration negotiation -- and
        hopefully end up with a usable channel.
*/

#include "csr_synergy.h"

#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "csr_bt_common.h"
#include "error.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_mblk.h"
#include "bkeyval.h"

#include "l2cap_common.h"
#include "l2cap_signal.h"
#include "l2cap_chme.h"
#include "l2cap_con_handle.h"
#include "l2cap_sig_handle.h"
#include "l2cap_cid.h"
#include "l2cap_cidme.h"
#include "l2cap_control.h"
#include "l2cap_flow.h"
#include "l2cap_interface.h"
#include "l2cap_auto.h"
#include "l2caplib.h"

#ifndef BUILD_FOR_HOST
#include "vm_const.h"
#include "stream.h"
#endif

#ifdef INSTALL_L2CAP_DEBUG
#ifdef BUILD_FOR_HOST
#include "csr_exceptionhandler.h"
#define DEBOUT(x)  CsrGeneralWarning(NULL, (CsrUint16)(x), 0, "DEBOUT")
#else

#define DEBOUT(x)  { fault((faulCsrSchedTid)(x)); }
#endif
#else
#define DEBOUT(x)
#endif

#define L2CAP_MAX_TX_WINDOW 8

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT

static L2CAUTO_INSTANCE_T *L2CAUTO_Create(void);
static CsrBool L2CAUTO_KickConfig(L2CAUTO_INSTANCE_T *instance);
#ifdef BUILD_FOR_HOST
#define L2CAUTO_conftab_to_pmalloc(conftab, length)
#else
/*! \brief Check to see if a conftab is vm_const.
           If it is then copy to pmalloc.

    \param conftab Pointer to pointer to configuration table.
    \param length Length of configuration table.
*/
static void L2CAUTO_conftab_to_pmalloc(CsrUint16 **conftab, CsrUint16 length)
{
    CsrUint16 *oldtab = *conftab;

    if (vm_const_is_encoded(oldtab))
    {
        CsrUint16 *newtab;
        CsrUint16 *end;

        *conftab = newtab = (CsrUint16*)CsrPmemAlloc(length * sizeof(CsrUint16));

        for (end = oldtab + length; oldtab != end; ++oldtab, ++newtab)
            *newtab = vm_const_fetch(oldtab);
    }
}
#endif

/*! \brief Send a L2CA_AUTO_CONNECT_CFM

    This message constructs and sends a L2CA_AUTO_CONNECT_CFM message
    to the given phandle. Function will pass down the config member
    pointers directly without copying or modifing them!

    The function will consume the 'config' structure and free it.  The
    pointer will be cleared.
*/
static void L2CAUTO_SendAutoConnectCfm(L2CAUTO_INSTANCE_T   *instance,
                                       l2ca_conn_result_t    response,
                                       L2CA_CONFIG_T       **config)
{
    L2CA_AUTO_CONNECT_CFM_T *prim = zpnew(L2CA_AUTO_CONNECT_CFM_T);

    prim->type = L2CA_AUTO_CONNECT_CFM;
    prim->phandle = instance->phandle;
    prim->con_ctx = instance->con_ctx;
    prim->reg_ctx = instance->reg_ctx;
    bd_addr_copy(&prim->bd_addr, &instance->bd_addr);
    prim->cid = instance->cid;
    prim->psm_local = instance->local_psm;
    prim->result = response;
    if((config != NULL) && (*config != NULL))
    {
        CsrMemCpy(&prim->config, *config, sizeof(prim->config));

        /* Members have been transferred. Free container */
        CsrPmemFree(*config);
        *config = NULL;

        /* Fill in default values in case of unspecified MTU and
         * flush */
        if(!(prim->config.options & L2CA_SPECIFY_MTU))
        {
            prim->config.mtu = L2CA_MTU_DEFAULT;
        }
        if(!(prim->config.options & L2CA_SPECIFY_FLUSH))
        {
            prim->config.flush_to = L2CA_FLUSH_TO_DEFAULT;
        }
    }
    prim->ext_feats = instance->ext_feats;

    if ((instance->state & L2CAUTO_AWAITING_AUTO_CONNECT_RSP) == 0)
        L2CA_PutMessage(instance->phandle, L2CAP_PRIM, prim);
    else
    {
        /* We're not allowed to send it until we get L2CA_AUTO_CONNECT_RSP */
        CsrPmemFree(instance->queued_confirm);
        instance->queued_confirm = prim;
    }
}

/*! \brief Fatal error function

    Auto-connect reached a fatal error - internal or caused by peer.
    Take the appropiate action: Inform peer and possibly free the
    instance, or issue a disconnect.
*/
static void L2CAUTO_FatalError(L2CAUTO_INSTANCE_T *instance,
                               l2ca_conn_result_t result)
{
    /* We are now dying */
    instance->state |= L2CAUTO_DYING;

    /* Have the connection been established at all? */
    if(instance->state & L2CAUTO_CONNECTION_OK)
    {
        /* Connection up, so we need to disconnect and wait for the
         * confirm to return before informating peer */
        instance->disable_reconf = result;
        L2CA_DisconnectReq(instance->cid);
        DEBOUT(0x4000);
    }
    else
    {
        /* Connection not up, so we can just inform peer now that
         * things went wrong */
        if(instance->phandle != CSR_SCHED_QID_INVALID)
        {
            L2CAUTO_SendAutoConnectCfm(instance,
                                       result,
                                       NULL);
        }

        /* If this instance was created by L2CAP itself, free it now
         * as we never got around to setup the CIDCB */
        L2CAUTO_Free(instance, L2CA_CID_INVALID);
        DEBOUT(0x4001);
    }
}

/*! \brief Send either a L2CA_CONNECT_RSP or L2CA_CREATE_CHANNEL_RSP

    The auto-connect code is mostly AMP-agnostic, but in the channel
    setup process we need to reply with the correct response signal.
*/
#ifdef INSTALL_AMP_SUPPORT
static void L2CAUTO_SendCreateConnectRsp(L2CAUTO_INSTANCE_T *instance,
                                         l2ca_identifier_t identifier,
                                         l2ca_cid_t cid,
                                         l2ca_conn_result_t response,
                                         CsrUint16 con_ctx)
{
    if(instance->state & L2CAUTO_AMP)
    {
        L2CA_CreateChannelRsp(identifier,
                              cid,
                              response,
                              con_ctx);
    }
    else
    {
        /* Pass response to L2CAP */
        L2CA_ConnectRsp(cid,
                        identifier,
                        response,
                        con_ctx);
    }
}
#else
#define L2CAUTO_SendCreateConnectRsp(instance, id, cid, response, con_ctx) \
    L2CA_ConnectRsp((cid), (id), (response), (con_ctx))
#endif

/*! \brief Send a L2CA_AUTO_CONNECT_IND

    This message constructs and sends a L2CA_AUTO_CONNECT_IND message
    to the given phandle.
*/
static void L2CAUTO_SendAutoConnectInd(L2CAUTO_INSTANCE_T *instance,
                                       l2ca_identifier_t   identifier,
                                       l2ca_controller_t   local_control)
{
    L2CA_AUTO_CONNECT_IND_T *prim = zpnew(L2CA_AUTO_CONNECT_IND_T);

    prim->type = L2CA_AUTO_CONNECT_IND;
    prim->phandle = instance->phandle;
    prim->identifier = identifier;
    bd_addr_copy(&prim->bd_addr, &instance->bd_addr);
    prim->cid = instance->cid;
    prim->reg_ctx = instance->reg_ctx;
    prim->psm_local = instance->local_psm;
    prim->local_control = local_control;

    L2CA_PutMessage(instance->phandle, L2CAP_PRIM, prim);

    instance->state |= L2CAUTO_AWAITING_AUTO_CONNECT_RSP;
}

/*! \brief Send empty configuration request

    If conftab is empty or when doing negative reconfigurations we
    may need to send an empty configuration request. This function
    does exactly that
*/
static void L2CAUTO_EmptyConfigReq(l2ca_cid_t cid)
{
    L2CA_CONFIG_T config;
    CsrMemSet(&config, 0, sizeof(config));
    L2CA_ConfigReqCs(cid, &config);
}

/*! \brief Send L2CA_CONFIG_UNACCEPTABLE configuration response */
static void L2CAUTO_RejectReconfiguration(l2ca_cid_t cid,
                                         l2ca_identifier_t identifier)
{
    L2CA_CONFIG_T config;
    CsrMemSet(&config, 0, sizeof(config));
    config.mtu = L2CA_MTU_DEFAULT;
    config.flush_to = L2CA_FLUSH_TO_DEFAULT;
    /* Done by CsrMemSet() config.fcs = L2CA_FCS_OFF; */
    L2CA_ConfigRspCs(cid, identifier, L2CA_CONFIG_UNACCEPTABLE, &config);
}

/*! \brief Store agreed config results

    When the auto-connect procedure is done we need to store the
    agreed results (that the upper layer wants to know about)
    somewhere. The parameters come from either what we told the peer
    (request/local side), or what she told us (respond/remote side),
    or what both sides agreed/defaulted to (shared)

    - MTU           : remote
    - Flush timeout : local
    - QoS           : local (responder side may add best-effort)
    - Flow control  : remote
    - FCS           : remote (special - not set if override in use)
    - Flowspec      : local
    - Extended win  : remote
*/
static void L2CAUTO_StoreResults(L2CAUTO_INSTANCE_T *instance,
                                 L2CA_CONFIG_T *config,
                                 CsrBool toair)
{
    l2ca_options_t res_options, cfg_options;
    L2CA_CONFIG_T *results;
    L2CA_FLOW_T *cfg_flow;

    if(instance->results == NULL)
    {
        instance->results = zpnew(L2CA_CONFIG_T);
    }

    /* Cached dereferences */
    results = instance->results;
    res_options = results->options;
    cfg_options = config->options;
    cfg_flow = config->flow;

    if(toair)
    {
        /* Flush timeout */
        if(cfg_options & L2CA_SPECIFY_FLUSH)
        {
            res_options |= L2CA_SPECIFY_FLUSH;
            results->flush_to = config->flush_to;
        }
        else
        {
            /* Force spec default to be set */
            results->flush_to = L2CA_FLUSH_TO_DEFAULT;
        }

        /* QoS */
        if((cfg_options & L2CA_SPECIFY_QOS) && (config->qos != NULL))
        {
            res_options |= L2CA_SPECIFY_QOS;
            CsrPmemFree(results->qos);
            results->qos = pcopy(config->qos, sizeof(L2CA_QOS_T));
        }

        /* Flowspec */
        if((cfg_options & L2CA_SPECIFY_FLOWSPEC) && (config->flowspec != NULL))
        {
            res_options |= L2CA_SPECIFY_FLOWSPEC;
            CsrPmemFree(results->flowspec);
            results->flowspec = pcopy(config->flowspec, sizeof(L2CA_FLOWSPEC_T));
        }

        /* FCS */
        if(cfg_options & L2CA_SPECIFY_FCS)
        {
            res_options |= L2CA_SPECIFY_FCS;
            /* We should store back the result of config process
             * anyway as it is needed as a part of
             * L2CA_AUTO_CONNECT_CFM sent to the application.
            */
            results->fcs = config->fcs;
        }
        else if((cfg_flow == NULL) || (cfg_flow->mode == L2CA_FLOW_MODE_BASIC))
        {
            /* Force spec default to be set */
            results->fcs = L2CA_FCS_OFF;
        }
        else
        {
            /* Force spec default to be set */
            results->fcs = L2CA_FCS_ON;
        }

    }
    /* Respond side */
    else
    {
        /* MTU */
        if(cfg_options & L2CA_SPECIFY_MTU)
        {
            res_options |= L2CA_SPECIFY_MTU;
            results->mtu = config->mtu;
        }
        else
        {
            /* Force spec default to be set */
            results->mtu = L2CA_MTU_DEFAULT;
        }

        /* Flow control */
        if((cfg_options & L2CA_SPECIFY_FLOW) && (cfg_flow != NULL))
        {
            res_options |= L2CA_SPECIFY_FLOW;
            CsrPmemFree(results->flow);
            results->flow = pcopy(cfg_flow, sizeof(L2CA_FLOW_T));
        }

        /* QoS special case: Add a best-effort QoS structure if
         * responder side wants it and we don't have one already */
#ifdef INSTALL_L2CAP_QOS_SUPPORT
        if((cfg_options & L2CA_SPECIFY_QOS) &&
           !(res_options & L2CA_SPECIFY_QOS))
        {
            res_options |= L2CA_SPECIFY_QOS;
            results->qos = L2CA_AllocQoS();
        }
#endif

        /* Extended windows */
        if(cfg_options & L2CA_SPECIFY_EXT_WINDOW)
        {
            res_options |= L2CA_SPECIFY_EXT_WINDOW;
            results->ext_window = config->ext_window;

            /* Zero old-school window field to force app to use
             * extended one */
            if((cfg_options & L2CA_SPECIFY_FLOW) && (cfg_flow != NULL))
            {
                results->flow->tx_window = 0;
            }
        }

        /* FCS */
        if(cfg_options & L2CA_SPECIFY_FCS)
        {
            res_options |= L2CA_SPECIFY_FCS;
            results->fcs = config->fcs;
        }
        else if((cfg_flow == NULL) || (cfg_flow->mode == L2CA_FLOW_MODE_BASIC))
        {
            /* Force spec default to be set */
            results->fcs = L2CA_FCS_OFF;
        }
        else
        {
            /* Force spec default to be set */
            results->fcs = L2CA_FCS_ON;
        }
    }

    results->options = res_options;
}

/*! \brief Clear results

    If local side, or peer, rejects results we need to remove those
    options from the accumulated result buffer.

    \param instance pointer to the auto connect instance
    \param clear_mask is the bitmask with the options that shall
           be cleared.
    \param cfm_dir results cleared in the confirm direction
*/
static void L2CAUTO_ClearResults(L2CAUTO_INSTANCE_T *instance,
                                 l2ca_options_t clear_mask,
                                 CsrBool toair)
{
    L2CA_CONFIG_T *res;

    res = instance->results;
    if(res == NULL)
    {
        /* Nothing to clear, bail out */
        return;
    }

    /* Modify clear mask to take direction into account. See
     * StoreResults() for details */
    if(toair)
    {
        clear_mask &= ~(L2CA_SPECIFY_FLUSH |
                        L2CA_SPECIFY_QOS |
                        L2CA_SPECIFY_FCS |
                        L2CA_SPECIFY_FLOWSPEC);
    }
    else
    {
        clear_mask &= ~(L2CA_SPECIFY_MTU |
                        L2CA_SPECIFY_FLOW |
                        L2CA_SPECIFY_FCS |
                        L2CA_SPECIFY_EXT_WINDOW);
    }

    /* Clear using the bitmask. Bits set are those to be cleared, so
     * AND with inverted mask */
    res->options &= ~(clear_mask);

    /* Free pointers if option isn't there anymore */
    if(!(res->options & L2CA_SPECIFY_QOS))
    {
        CsrPmemFree(res->qos);
        res->qos = NULL;
    }
    if(!(res->options & L2CA_SPECIFY_FLOW))
    {
        CsrPmemFree(res->flow);
        res->flow = NULL;
    }
    if(!(res->options & L2CA_SPECIFY_FLOWSPEC))
    {
        CsrPmemFree(res->flowspec);
        res->flowspec = NULL;
    }

    /* Can we free the buffer entirely? */
    if(res->options == 0)
    {
        CsrPmemFree(res);
        instance->results = NULL;
    }
}

/*! \brief Find instance from list

    This function will find the auto-connect instance from the list in the
    MCB. If not found, it returns NULL.
*/
static L2CAUTO_INSTANCE_T *L2CAUTO_FindInstance(l2ca_cid_t cid,
                                                const BD_ADDR_T *bd_addr,
                                                psm_t local_psm)
{
    L2CAUTO_INSTANCE_T *instance;

    for(instance = mcb.autoconnect;
        instance != NULL;
        instance = instance->next)
    {
        /* Match for CID - when connections are up */
        if((cid != L2CA_CID_INVALID) &&
           (cid == instance->cid))
        {
            return instance;
        }
        /* Match for psm and addr - this matches channels being
         * set up */
        else if((bd_addr != NULL) &&
                (instance->cid == L2CA_CID_INVALID) &&
                (local_psm != L2CA_PSM_INVALID) &&
                (local_psm == instance->local_psm) &&
                bd_addr_eq(bd_addr, &instance->bd_addr))
        {
            return instance;
        }
    }
    return NULL;
}

/*! \brief Does config contain a certain option

    Determine whether a configuration structure contain a specied
    option
*/
static CsrBool L2CAUTO_ConfHas(L2CA_CONFIG_T *config,
                              l2ca_options_t option)
{
    CsrBool option_present = FALSE;
    if ((config->options & option) != 0)
        option_present = TRUE;

    switch(option)
    {
        case L2CA_SPECIFY_FLOW:
            if (config->flow != NULL)
                return option_present;
            break;

        case L2CA_SPECIFY_QOS:
            if (config->qos != NULL)
                return option_present;
            break;

        case L2CA_SPECIFY_FLOWSPEC:
            if (config->flowspec != NULL)
                return option_present;

            break;

        case L2CA_SPECIFY_MTU:
            /* -fallthrough */
        case L2CA_SPECIFY_FLUSH:
            /* -fallthrough */
        case L2CA_SPECIFY_FCS:
            /* -fallthrough */
        case L2CA_SPECIFY_EXT_WINDOW:
            /* -fallthrough */
            return option_present;
    }

    return FALSE;
}


/*! \brief Choose 'best' value given min, max and backoff

    Select peer if between min and max, otherwise select boundary
    based on what side peer is out of bounds. Return TRUE if selected
    value is within bounds, FALSE otherwise.

    Note that if min > max, we invert the meaning of min and max in
    the sense that minimum values are preferred over maximum.
*/
#if defined(INSTALL_L2CAP_FLOWSPEC_SUPPORT) || defined(INSTALL_L2CAP_QOS_SUPPORT)
static CsrBool L2CAUTO_Best32(CsrUint32 *min,
                             CsrUint32 prefer,
                             CsrUint32 *peer)
{
    CsrBool result;
    CsrUint32 alt;
    alt = (CsrUint32)(peer != NULL ? *peer : prefer);

    if(*min <= prefer)
    {
        if(alt < *min)
        {
            result = FALSE;
            /* want value in min */
        }
        else if(alt > prefer)
        {
            result = FALSE;
            *min = prefer;
        }
        else
        {
            result = TRUE;
            *min = alt;
        }
    }
    else
    {
        if(alt < prefer)
        {
            result = FALSE;
            *min = prefer;
        }
        else if(alt > *min)
        {
            result = FALSE;
            /* want value in min */
        }
        else
        {
            result = TRUE;
            *min = alt;
        }
    }
    return result;
}
#endif

static CsrBool L2CAUTO_Best16(CsrUint16 *min,
                             CsrUint16 prefer,
                             CsrUint16 *peer)
{
    CsrBool result;
    CsrUint16 alt;
    alt = (CsrUint16)(peer != NULL ? *peer : prefer);

    if(*min <= prefer)
    {
        if(alt < *min)
        {
            result = FALSE;
            /* want value in min */
        }
        else if(alt > prefer)
        {
            result = FALSE;
            *min = prefer;
        }
        else
        {
            result = TRUE;
            *min = alt;
        }
    }
    else
    {
        if(alt < prefer)
        {
            result = FALSE;
            *min = prefer;
        }
        else if(alt > *min)
        {
            result = FALSE;
            /* want value in min */
        }
        else
        {
            result = TRUE;
            *min = alt;
        }
    }
    return result;
}

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
static CsrBool L2CAUTO_Best8(CsrUint8 *min,
                            CsrUint8 prefer,
                            CsrUint8 *peer)
{
    CsrBool result;
    CsrUint8 alt;
    alt = (CsrUint8)(peer != NULL ? *peer : prefer);

    if(*min <= prefer)
    {
        if(alt < *min)
        {
            result = FALSE;
            /* want value in min */
        }
        else if(alt > prefer)
        {
            result = FALSE;
            *min = prefer;
        }
        else
        {
            result = TRUE;
            *min = alt;
        }
    }
    else
    {
        if(alt < prefer)
        {
            result = FALSE;
            *min = prefer;
        }
        else if(alt > *min)
        {
            result = FALSE;
            /* want value in min */
        }
        else
        {
            result = TRUE;
            *min = alt;
        }
    }
    return result;
}
#endif

/*! \brief Flow mode indication - backoff or reject

    What flow control mode depends on what the peer wants and whether
    we allow backoffs. And what modes the peer actually supports.

    \param prefer Single preferred mode
    \param allow Mask of altenative modes we allow to be used
    \param peer Peer's preferred mode
    \param ext_feats Peer's extended features bitmask
*/
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
static CsrBool L2CAUTO_BestFlowModeReq(l2ca_flow_mode_t *prefer,
                                      l2ca_flow_mode_t allow,
                                      l2ca_flow_mode_t peer,
                                      l2ca_ext_feats_t ext_feats)
{
    l2ca_flow_mode_t feat_mode;

    /* Convert extended features in to a mode mask */
    feat_mode = (L2CA_FLOW_MODE_BASIC |
                 (l2ca_flow_mode_t)((ext_feats & L2CA_EXT_FEAT_ENH_RETRANS)
                                    ? L2CA_FLOW_MODE_ENHANCED_RETRANS : 0) |
                 (l2ca_flow_mode_t)((ext_feats & L2CA_EXT_FEAT_STREAMING)
                                    ? L2CA_FLOW_MODE_STREAMING : 0));

    /* Quick shortcut - modes match and peer supports feature */
    if(((*prefer == peer) || (peer == (l2ca_flow_mode_t)~0)) &&
       ((feat_mode & *prefer) == *prefer))
    {
        return TRUE;
    }

    /* Catch legacy modes - we don't support there */
    if((peer == L2CA_FLOW_MODE_RTM_OBSOLETE) ||
       (peer == L2CA_FLOW_MODE_FC_OBSOLETE))
    {
        return FALSE;
    }

    /* Allow must include our preferred mode */
    allow |= L2CA_MODE_MASK(*prefer);

    /* Have peer sent us a request? */
    if(peer != (l2ca_flow_mode_t)~0)
    {
        if (peer < *prefer)
        {
            l2ca_flow_mode_t accept_peer;

            /* Peer's mode is higher priority than ours. If we support
               peer's mode then we may as well go for that. Otherwise,
               we look for a mode that we do support that is higher still. */
            accept_peer = allow & L2CA_MODE_MASK(peer);
            if (accept_peer != 0)
                allow = accept_peer;
            else
                allow &= ~(0xFF << peer);
        }
        else
        {
            /* Our mode is higher priority, so peer should not have rejected. Fail. */
            allow = 0;
        }
    }

    /* Choose "highest" mode. Bail out if a compromise has been
     * found. */
    if(allow)
    {
        if(allow & L2CA_MODE_MASK_BASIC)
        {
            *prefer = L2CA_FLOW_MODE_BASIC;
            return TRUE;
        }
        else if((allow & L2CA_MODE_MASK_ENHANCED_RETRANS) &&
                (ext_feats & L2CA_EXT_FEAT_ENH_RETRANS))
        {
            *prefer = L2CA_FLOW_MODE_ENHANCED_RETRANS;
            return TRUE;
        }
        else if((allow & L2CA_MODE_MASK_STREAMING) &&
                (ext_feats & L2CA_EXT_FEAT_STREAMING))
        {
            *prefer = L2CA_FLOW_MODE_STREAMING;
            return TRUE;
        }
    }

    /* Peer dones't support any of our modes */
    return FALSE;
}
#endif

/*! \brief Flow control mode indication - accept or reject

    Given our preferred modes, can we accept what peer proposes?
*/
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
static CsrBool L2CAUTO_BestFlowModeInd(l2ca_flow_mode_t *prefer,
                                      l2ca_flow_mode_t allow,
                                      l2ca_flow_mode_t peer)
{
    /* Quick shortcut - modes match */
    if(*prefer == peer)
    {
        return TRUE;
    }

    /* Catch old modes - we don't support there */
    if((peer == L2CA_FLOW_MODE_RTM_OBSOLETE) ||
       (peer == L2CA_FLOW_MODE_FC_OBSOLETE))
    {
        return FALSE;
    }

    /* Allow must include preferred mode */
    allow |= L2CA_MODE_MASK(*prefer);

    /* Priority is: basic(0), ertm(3), streaming(4), ie. reverse */
    if (*prefer < peer)
        return FALSE;

    /* Do we support what peer wants? */
    if(L2CA_MODE_MASK(peer) & allow)
    {
            /* Back off as peer's mode is higher */
            *prefer = peer;
            return TRUE;
        }
        else
        {
        /* Peer's mode is higher and we don't support it. Try for Basic mode. */
        if ((allow & L2CA_MODE_MASK_BASIC) != 0)
            *prefer = L2CA_FLOW_MODE_BASIC;

        /* No - reject */
        return FALSE;
    }
}
#endif

/*! \brief Choose 'best' QoS service type given min, allowed and backoff

    Choose old-style QoS mode based on what's possible and allowed.

    \param prefer Single preferred mode
    \param peer Peer's mode
    \param ext_feats Peer's extended features bitmask
*/
#if defined(INSTALL_L2CAP_QOS_SUPPORT) || defined(INSTALL_L2CAP_FLOWSPEC_SUPPORT)
static CsrBool L2CAUTO_BestQosService(l2ca_service_type_t prefer,
                                     l2ca_service_type_t peer,
                                     l2ca_ext_feats_t ext_feats)
{
    /* TODO: We need to do ext_feat checks here */

    /* Best effort doesn't work with guaranteed */
    if(( (prefer == L2CA_QOS_TYPE_BEST_EFFORT) &&
         (peer == L2CA_QOS_TYPE_GUARANTEED)) ||
       ( (prefer == L2CA_QOS_TYPE_GUARANTEED) &&
         (peer == L2CA_QOS_TYPE_BEST_EFFORT)) )
    {
        return FALSE;
    }

    /* Seems to be OK */
    return TRUE;
}
#endif

/*! \brief Merge backoff information

    Peer has rejected a certain option, so merge this data into our
    backoff buffer. Note that we have to copy the lot to ensure that
    the data isn't cluttering up the ACL buffer.
*/
static void L2CAUTO_MergeBackoff(L2CAUTO_INSTANCE_T *instance,
                                 L2CA_CONFIG_T *config)
{
    /* Allocate backoff buffer if not there */
    if(instance->backoff == NULL)
    {
        instance->backoff = zpnew(L2CA_CONFIG_T);
    }

    /* Merge bits in */
    L2CA_MergeConfigPtrs(instance->backoff, config);
}

/*! \brief Read directional min/max L2CAP config

    With the current block, read in the min/max L2CAP configuration
    options from the conftab. We either read the 'request direction'
    or the 'indication direction' keys depending on the value of
    req_dir.

    We don't do any analysis of the parameters, except when we need to
    up or down scale some parameters and when detecting whether or not
    to use extended windows.
*/
static CsrBool L2CAUTO_ConftabRead(L2CAUTO_INSTANCE_T *instance,
                                  CsrUint16 block,
                                  CsrBool req_dir,
                                  L2CA_CONFIG_T *mincf,
                                  L2CA_CONFIG_T *maxcf)
{
    BKV_ITERATOR_T iter;
    CsrUint32 min32, max32;
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    CsrUint16 min16, max16;
#endif

    /* Setup conftab access and iterator */
    iter.iterator = 0;
    iter.block = instance->conftab;
    iter.size = instance->length;
    CsrMemSet(mincf, 0, sizeof(L2CA_CONFIG_T));
    CsrMemSet(maxcf, 0, sizeof(L2CA_CONFIG_T));

    /* Allow empty conftab to be used */
    if((instance->conftab != NULL) &&
       (instance->curr_block == 0))
    {
        /* Conftab in use - jump to iterator or die */
        if(!BKV_JumpToBlock(&iter, block))
        {
            /* Error in iterator */
            return FALSE;
        }
    }

    /* Flow control mode mode, FCS and extended windows.  Direction
     * for local/remote is per-setting */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT

    /* Frame check sequence (downscale - note special encoding!) */
    /* Possible to only send this option in the conftab */
    if(BKV_Scan16Single(&iter, L2CA_AUTOPT_FCS | BKV_TYPE_EXACT, &min16))
    {
        maxcf->options |= L2CA_SPECIFY_FCS;
        maxcf->fcs = (l2ca_fcs_t)(min16 & 0x03); /* only two LSO bits used */
    }
    else
    {
        maxcf->fcs = L2CA_FCS_ON;
    }

    if(BKV_KeyExists(&iter, L2CA_AUTOPT_FLOW_MODE))
    {
        L2CA_FLOW_T *mincf_flow, *maxcf_flow;

        /* New options */
        maxcf->options |= L2CA_SPECIFY_FLOW;
        mincf->flow = mincf_flow = zpnew(L2CA_FLOW_T);
        maxcf->flow = maxcf_flow = zpnew(L2CA_FLOW_T);

        /* Mode (downscale to 8 bit) */
        if(!BKV_Scan16Single(&iter,
                             L2CA_AUTOPT_FLOW_MODE,
                             &min16))
        {
#ifdef INSTALL_L2CAP_DEBUG
            BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
#endif
        }
        mincf_flow->mode = (l2ca_flow_mode_t)(min16 & 0xff); /* fallback */
        maxcf_flow->mode = (l2ca_flow_mode_t)(min16 >> 8);   /* prefered */

        /* If preferred mode is Basic then add it to the fallback mask.
           The assumption here is that the specification does not intend that
           a preference or demand for Basic mode should lead to a 'state 2'
           algorithm. By ensuring that Basic is also a fallback mode, we
           guarantee a state 1 algorithm. See B-71448 and Bluetooth
           specification errata 3322. For the definition of state 1 and
           state 2 algorithms, and the fallback procedure itself, see
           L2CAP 3.0+HS Section 5.4. */
        if (maxcf_flow->mode == L2CA_FLOW_MODE_BASIC)
            mincf_flow->mode |= L2CA_MODE_MASK_BASIC;

        /* If we allow any sort of fallback at all then we should allow
           fallback to our preferred mode. */
        if (mincf_flow->mode != 0)
            mincf_flow->mode |= L2CA_MODE_MASK(maxcf_flow->mode);

        /* Remove legacy modes from fallback mask. The ObsoleteCheck
         * fct will ensure that we die if prefered mode is a legacy */
        mincf_flow->mode &= ~(L2CA_MODE_MASK_FC_OBSOLETE |
                               L2CA_MODE_MASK_RTM_OBSOLETE);

        /* Tx window - downscale and detect extended windows
         * Request direction is local value */
        if(!BKV_Scan16Range(&iter,
                            (CsrUint16)(req_dir ? L2CA_AUTOPT_FLOW_WINDOW_IN : L2CA_AUTOPT_FLOW_WINDOW_OUT),
                            &mincf->ext_window, &maxcf->ext_window))
        {
            mincf->ext_window = (l2ca_ext_window_size_t)(req_dir
                                                         ? L2CAP_DEFAUTO_FLOW_TX_WINDOW_IN_MIN
                                                         : L2CAP_DEFAUTO_FLOW_TX_WINDOW_OUT_MIN);
            maxcf->ext_window = (l2ca_ext_window_size_t)(req_dir
                                                         ? L2CAP_DEFAUTO_FLOW_TX_WINDOW_IN_MAX
                                                         : L2CAP_DEFAUTO_FLOW_TX_WINDOW_OUT_MAX);
        }

        /* Max retransmit
         * Request direction is remote value */
        if(!BKV_Scan16Range(&iter,
                            (CsrUint16)(req_dir ? L2CA_AUTOPT_FLOW_MAX_RETX_OUT : L2CA_AUTOPT_FLOW_MAX_RETX_IN),
                            &min16, &max16))
        {
            min16 = (l2ca_max_retransmit_t)(req_dir
                                            ? L2CAP_DEFAUTO_FLOW_MAX_RETX_OUT_MIN
                                            : L2CAP_DEFAUTO_FLOW_MAX_RETX_IN_MIN);
            max16 = (l2ca_max_retransmit_t)(req_dir
                                            ? L2CAP_DEFAUTO_FLOW_MAX_RETX_OUT_MAX
                                            : L2CAP_DEFAUTO_FLOW_MAX_RETX_IN_MAX);
        }
        mincf_flow->max_retransmit = (l2ca_max_retransmit_t)(min16 & 0xff);
        maxcf_flow->max_retransmit = (l2ca_max_retransmit_t)(max16 & 0xff);

        /* Maximum PDU payload size
         * Request direction is local value */
        if(!BKV_Scan16Range(&iter,
                            (CsrUint16)(req_dir ? L2CA_AUTOPT_FLOW_MAX_PDU_IN : L2CA_AUTOPT_FLOW_MAX_PDU_OUT),
                            &mincf_flow->maximum_pdu, &maxcf_flow->maximum_pdu))
        {
            mincf_flow->maximum_pdu = (l2ca_mtu_t)(req_dir
                                                    ? L2CAP_DEFAUTO_FLOW_MPS_IN_MIN
                                                    : L2CAP_DEFAUTO_FLOW_MPS_OUT_MIN);
            maxcf_flow->maximum_pdu = (l2ca_mtu_t)(req_dir
                                                    ? L2CAP_DEFAUTO_FLOW_MPS_IN_MAX
                                                    : L2CAP_DEFAUTO_FLOW_MPS_OUT_MAX);
        }
   }
#endif

    /* Flowspec. Request direction is outgoing parameters. */
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
    if(BKV_KeyExists(&iter, L2CA_AUTOPT_FS_SERVICE))
    {
        CsrUint16 min16;

        /* New options */
        maxcf->options |= L2CA_SPECIFY_FLOWSPEC;
        mincf->flowspec = zpnew(L2CA_FLOWSPEC_T);
        maxcf->flowspec = zpnew(L2CA_FLOWSPEC_T);

        /* Service type (downscale) */
        (void)BKV_Scan16Single(&iter,
                               L2CA_AUTOPT_FS_SERVICE | BKV_TYPE_EXACT,
                               &min16);
        mincf->flowspec->fs_service_type = (l2ca_service_type_t)(min16 & 0xff); /* allow mask */
        maxcf->flowspec->fs_service_type = (l2ca_service_type_t)(min16 >> 8);   /* prefered */

        /* Max SDU */
        if(!BKV_Scan16Range(&iter,
                            (CsrUint16)(req_dir ? L2CA_AUTOPT_FS_SDU_SIZE_OUT : L2CA_AUTOPT_FS_SDU_SIZE_IN),
                            &mincf->flowspec->fs_max_sdu,  &maxcf->flowspec->fs_max_sdu))
        {
            mincf->flowspec->fs_max_sdu = L2CAP_DEFAUTO_FLOWSPEC_MAX_SDU_MIN;
            maxcf->flowspec->fs_max_sdu = L2CAP_DEFAUTO_FLOWSPEC_MAX_SDU_MAX;
        }

        /* Interarrival */
        if(!BKV_Scan32Range(&iter,
                            (CsrUint16)(req_dir ? L2CA_AUTOPT_QOS_RATE_OUT : L2CA_AUTOPT_QOS_RATE_IN),
                            &mincf->flowspec->fs_interarrival, &maxcf->flowspec->fs_interarrival))
        {
            mincf->flowspec->fs_interarrival = L2CAP_DEFAUTO_FLOWSPEC_INTARR_MIN;
            maxcf->flowspec->fs_interarrival = L2CAP_DEFAUTO_FLOWSPEC_INTARR_MAX;
        }

        /* Access latency */
        if(!BKV_Scan32Range(&iter,
                            (CsrUint16)(req_dir ? L2CA_AUTOPT_QOS_LATENCY_OUT : L2CA_AUTOPT_QOS_LATENCY_IN),
                            &mincf->flowspec->fs_latency, &maxcf->flowspec->fs_latency))
        {
            mincf->flowspec->fs_latency = L2CAP_DEFAUTO_FLOWSPEC_LATENCY_MIN;
            maxcf->flowspec->fs_latency = L2CAP_DEFAUTO_FLOWSPEC_LATENCY_MAX;
        }

        /* Flush timeout */
        if(!BKV_Scan32Range(&iter,
                            (CsrUint16)(req_dir ? L2CA_AUTOPT_FLUSH_OUT : L2CA_AUTOPT_FLUSH_IN),
                            &mincf->flowspec->fs_flush_to, &maxcf->flowspec->fs_flush_to))
        {
            mincf->flowspec->fs_flush_to = L2CAP_DEFAUTO_FLOWSPEC_FLUSH_TO_MIN;
            maxcf->flowspec->fs_flush_to = L2CAP_DEFAUTO_FLOWSPEC_FLUSH_TO_MAX;
        }
    }
#endif

    /* Old QoS. Request direction has the outgoing parameters. */
#ifdef INSTALL_L2CAP_QOS_SUPPORT
    if(BKV_KeyExists(&iter, L2CA_AUTOPT_QOS_SERVICE))
    {
        CsrUint16 min16;

        /* New options */
        maxcf->options |= L2CA_SPECIFY_QOS;
        mincf->qos = zpnew(L2CA_QOS_T);
        maxcf->qos = zpnew(L2CA_QOS_T);

        /* Service type (downscale) */
        (void)BKV_Scan16Single(&iter,
                               L2CA_AUTOPT_QOS_SERVICE | BKV_TYPE_EXACT,
                               &min16);
        mincf->qos->service_type = (l2ca_service_type_t)(min16 & 0xff); /* send-option */
        maxcf->qos->service_type = (l2ca_service_type_t)(min16 >> 8);   /* prefered */

        /* Token rate */
        if(!BKV_Scan32Range(&iter,
                            (CsrUint16)(req_dir ? L2CA_AUTOPT_QOS_RATE_OUT : L2CA_AUTOPT_QOS_RATE_IN),
                            &mincf->qos->token_rate, &maxcf->qos->token_rate))
        {
            mincf->qos->token_rate = L2CAP_DEFAUTO_QOS_TOKEN_RATE_MIN;
            maxcf->qos->token_rate = L2CAP_DEFAUTO_QOS_TOKEN_RATE_MAX;
        }

        /* Token bucket */
        if(!BKV_Scan32Range(&iter,
                            (CsrUint16)(req_dir ? L2CA_AUTOPT_QOS_BUCKET_OUT : L2CA_AUTOPT_QOS_BUCKET_IN),
                            &mincf->qos->token_bucket, &maxcf->qos->token_bucket))
        {
            mincf->qos->token_bucket = L2CAP_DEFAUTO_QOS_TOKEN_BUCKET_MIN;
            maxcf->qos->token_bucket = L2CAP_DEFAUTO_QOS_TOKEN_BUCKET_MAX;
        }

        /* Peak bandwidth */
        if(!BKV_Scan32Range(&iter,
                            (CsrUint16)(req_dir ? L2CA_AUTOPT_QOS_PEAK_OUT : L2CA_AUTOPT_QOS_PEAK_IN),
                            &mincf->qos->peak_bw, &maxcf->qos->peak_bw))
        {
            mincf->qos->peak_bw = L2CAP_DEFAUTO_QOS_PEAK_BW_MIN;
            maxcf->qos->peak_bw = L2CAP_DEFAUTO_QOS_PEAK_BW_MAX;
        }

        /* Latency */
        if(!BKV_Scan32Range(&iter,
                            (CsrUint16)(req_dir ? L2CA_AUTOPT_QOS_LATENCY_OUT : L2CA_AUTOPT_QOS_LATENCY_IN),
                            &mincf->qos->latency, &maxcf->qos->latency))
        {
            mincf->qos->latency = L2CAP_DEFAUTO_QOS_LATENCY_MIN;
            maxcf->qos->latency = L2CAP_DEFAUTO_QOS_LATENCY_MAX;
        }

        /* Delay variation */
        if(!BKV_Scan32Range(&iter,
                            (CsrUint16)(req_dir ? L2CA_AUTOPT_QOS_DELAY_OUT : L2CA_AUTOPT_QOS_DELAY_IN),
                            &mincf->qos->delay_var, &maxcf->qos->delay_var))
        {
            mincf->qos->delay_var = L2CAP_DEFAUTO_QOS_DELAY_VAR_MIN;
            maxcf->qos->delay_var = L2CAP_DEFAUTO_QOS_DELAY_VAR_MAX;
        }
    }
#endif

    /* MTU. Request path has the incoming parameters. */
    if(BKV_Scan16Single(&iter,
                       (CsrUint16)(req_dir ? L2CA_AUTOPT_MTU_IN : L2CA_AUTOPT_MTU_OUT),
                       &mincf->mtu))
    {
        maxcf->options |= L2CA_SPECIFY_MTU;

        /* MTU is not negotiated, so just make sure that the value
         * from the table adheres to the spec */
        mincf->mtu = CSRMAX(mincf->mtu, L2CA_MTU_MINIMUM);
        maxcf->mtu = CSRMAX(mincf->mtu, L2CA_MTU_MINIMUM);
    }
    else
    {
        mincf->mtu = L2CAP_DEFAUTO_MTU_MIN;
        maxcf->mtu = L2CAP_DEFAUTO_MTU_MAX;
    }

    /* Flush timeout. Request path has the outgoing parameters. */
    if(BKV_Scan32Range(&iter,
                       (CsrUint16)(req_dir ? L2CA_AUTOPT_FLUSH_OUT : L2CA_AUTOPT_FLUSH_IN),
                       &min32, &max32))
    {
        maxcf->options |= L2CA_SPECIFY_FLUSH;

        /* We must have special support for the infinite flush
         * timeout, and we must downscale from us to ms - and then
         * apply the HCI slot limit. Also note that we must deal with
         * swapped ranges to handle max,min */
        mincf->flush_to = (l2ca_timeout_t)((min32 == L2CA_FLOWSPEC_FLUSH_TO_INFINITE)
                                           ? L2CA_FLUSH_TO_INFINITE
                                           : CSRMIN(((min32 / 1000) & 0xffff), L2CA_FLUSH_TO_MAX_HCI_LIMIT_MS));
        mincf->flush_to = CSRMAX(mincf->flush_to, L2CA_FLUSH_TO_IMMEDIATE);

        maxcf->flush_to = (l2ca_timeout_t)((max32 == L2CA_FLOWSPEC_FLUSH_TO_INFINITE)
                                           ? L2CA_FLUSH_TO_INFINITE
                                           : CSRMIN(((max32 / 1000) & 0xffff), L2CA_FLUSH_TO_MAX_HCI_LIMIT_MS));
        maxcf->flush_to = CSRMAX(maxcf->flush_to, L2CA_FLUSH_TO_IMMEDIATE);
    }
    else
    {
        /* No flush-timeout mandates a reliable link */
        mincf->flush_to = L2CAP_DEFAUTO_FLUSH_DEFAULT;
        maxcf->flush_to = L2CAP_DEFAUTO_FLUSH_DEFAULT;
    }

    return TRUE;
}

/*! \brief Determine flow mode

    Note that this should be called after flow mode has been decided and
    written to mincf.

    \param instance Autoconnect instance
    \param mincf Proposed configuration
    \returns L2CAP flow mode
*/
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
static l2ca_flow_mode_t L2CAUTO_DetermineMode(L2CAUTO_INSTANCE_T *instance,
                                              L2CA_CONFIG_T *mincf)
{
    if ((mincf->options & L2CA_SPECIFY_FLOW) == 0)
    {
        CIDCB_T *cidcb = NULL;

        /* Current proposed config does not specify flow. So look to
           see if it's already set to something. This will only be the
           case if this is a reconfiguration. */
        if ((cidcb = CIDME_GetCidcb(instance->cid)) != NULL
              && (cidcb->opened) /* We are sure it's a reconfig since the link is open */
              && (cidcb->flow))
        {
            return cidcb->flow->config.mode;
        }
    }
    else if (mincf->flow != NULL)
    {
        return mincf->flow->mode;
    }

    return L2CA_FLOW_MODE_BASIC;
}
#endif

/*! \brief Outgoing configuration request

    Generate the next outgoing configuration request based on the
    conftab table. We rely on the current iterator to be valid.

    The plan for picking/generating the configuration is:
    1, flow control
       a, select best mode using required/backoff algorithm
       b, apply parameters
    2, flowspec
    3, qos
    4, fcs
    5, extended windows
    6, mtu
    7, flush timeout
    8, send config request

    Whenever we want to fill in an option we check whether the remote
    peer actually knows about it (ext-feats and the 'unknown to peer'
    options). If an option is unavoidable in the sense that the
    default is not acceptable, we move on to the next conftab block
    and restart.

    We will build the options in the "mincf" variable as we move along
    (ie. move paramters from max/backoff when possible). Note that
    (prefer/backoff) values are stored in (maxcf/mincf) respectively.

    \return TRUE if configuration was sent, FALSE if the current
            block couldn't be used
*/
static CsrBool L2CAUTO_ConftabReq(L2CAUTO_INSTANCE_T *instance)
{
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    l2ca_flow_mode_t mode;
#endif
    L2CA_CONFIG_T mincf;
    L2CA_CONFIG_T maxcf;
    L2CA_CONFIG_T *backoff;

    /* If dying, exist immediately. Disconnect already sent. */
    if(instance->state & L2CAUTO_DYING)
    {
        return FALSE;
    }

    /* Read in REQ directional min/max options from conftab. If
     * conftab is empty, mincf/maxcf will be cleared and empty */
    if(!L2CAUTO_ConftabRead(instance,
                            instance->curr_block,
                            TRUE,  &mincf, &maxcf))
    {
        /* Error in iterator - prepare to fail */
        instance->curr_block = instance->num_blocks + 1;
        instance->retry = L2CAP_AUTOCONNECT_RETRY;
        instance->peer_modes_left = (CsrUint16)~0;
#ifdef INSTALL_L2CAP_DEBUG
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
#endif
        return FALSE;
    }

    /* If peer has not given us any backoff information (including
     * aksing for options), we cunningly fill out the backoff with our
     * own maximised/prefered ones */
    backoff = instance->backoff;
    if (backoff == NULL)
        backoff = &maxcf;

    /* Flow and error control */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    if(L2CAUTO_ConfHas(&maxcf, L2CA_SPECIFY_FLOW))
    {
        l2ca_flow_mode_t peer_mode = L2CA_FLOW_MODE_BASIC;

        /* What mode does peer want? */
        if(!(instance->unknown & L2CA_SPECIFY_FLOW) &&
           L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_FLOW))
        {
            /* Have peer really told us, or are we just defaulting?
             * The default in this case is ALL modes, ie. all bits */
            peer_mode = (l2ca_flow_mode_t)~0;

            if (backoff != &maxcf)
                peer_mode = backoff->flow->mode;
        }

        /* Choose mode */
        if(!L2CAUTO_BestFlowModeReq(&maxcf.flow->mode,  /* prefered */
                                    mincf.flow->mode,   /* allow mask */
                                    peer_mode,
                                    instance->ext_feats))
        {
            /* Modes were completely uncompatible */
            L2CA_FreeConfigPtrs(&mincf);
            L2CA_FreeConfigPtrs(&maxcf);
            DEBOUT(0x1000);
            return FALSE;
        }

        /* If non-basic mode, enable option and transfer parameters */
        if(maxcf.flow->mode != L2CA_FLOW_MODE_BASIC)
        {
            CsrBool hasback;
            CsrBool peersm;
            CsrUint16 peer_win;
            mincf.options |= L2CA_SPECIFY_FLOW;
            mincf.flow->mode = maxcf.flow->mode;

            /* Allow use of backoff values if they're
             * present. However, don't use certain values if peer
             * wants SM as they'll be zero... */
            hasback = L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_FLOW);
            peersm = (hasback && (backoff->flow->mode == L2CA_FLOW_MODE_STREAMING)
                      ? TRUE : FALSE);

            /* Select best value taking backoff into account */
            if(!L2CAUTO_Best8(&mincf.flow->max_retransmit, maxcf.flow->max_retransmit, NULL)
               || !L2CAUTO_Best16(&mincf.flow->maximum_pdu, maxcf.flow->maximum_pdu, NULL))
            {
                L2CA_FreeConfigPtrs(&mincf);
                L2CA_FreeConfigPtrs(&maxcf);
                DEBOUT(0x1001);
                return FALSE;
            }

            /* Check for mandatory support for extended windows. This
             * is achieved if both min and max are higher that the
             * standard range */
            if((mincf.ext_window >= L2CAP_STD_MAX_SEQUENCE) &&
               (maxcf.ext_window >= L2CAP_STD_MAX_SEQUENCE))
            {
                /* Make sure peer supports the extended windows - or
                 * fail hard */
                if((instance->unknown & L2CA_SPECIFY_EXT_WINDOW) ||
                   !(instance->ext_feats & L2CA_EXT_FEAT_EXT_WIN_SIZE))
                {
                    /* Window size out of range */
                    L2CA_FreeConfigPtrs(&mincf);
                    L2CA_FreeConfigPtrs(&maxcf);
                    DEBOUT(0x1002);
                    return FALSE;
                }
            }

            peer_win = maxcf.ext_window;
            if (!peersm)
            {
                if(L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_EXT_WINDOW))
                {
                    /* Extended windows take priority */
                    peer_win = backoff->ext_window;
                }
                else if(L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_FLOW))
                {
                    /* Use standard FEC */
                    peer_win = backoff->flow->tx_window;
                }
            }

            /* Select optimal window size */
            if(!L2CAUTO_Best16(&(mincf.ext_window), maxcf.ext_window, &peer_win))
            {
                /* Window size is not negotiated in ERTM, so if
                 * optimal value fails, go with default */
                if(maxcf.flow->mode == L2CA_FLOW_MODE_ENHANCED_RETRANS)
                {
                    mincf.ext_window = maxcf.ext_window;
                }
                else
                {
                    /* Window size out of range */
                    L2CA_FreeConfigPtrs(&mincf);
                    L2CA_FreeConfigPtrs(&maxcf);
                    DEBOUT(0x1003);
                    return FALSE;
                }
            }

            /* If the window size did spill into the extended range,
             * set the option but cull the standard range */
            if(mincf.ext_window > L2CAP_STD_MAX_SEQUENCE)
            {
                mincf.flow->tx_window = L2CAP_STD_MAX_SEQUENCE - 1;
                mincf.options |= L2CA_SPECIFY_EXT_WINDOW;
            }
            else
            {
                mincf.flow->tx_window = (l2ca_window_size_t)mincf.ext_window;
            }
        }
        else
        {
            /* No need for the flow structures in basic mode */
            CsrPmemFree(mincf.flow);
            CsrPmemFree(maxcf.flow);
            mincf.flow = NULL;
            maxcf.flow = NULL;
            mincf.options &= ~L2CA_SPECIFY_FLOW;
        }
    }
    else if(L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_FLOW))
    {
        /* Peer wants flow control, but we dont. Fail */
        L2CA_FreeConfigPtrs(&mincf);
        L2CA_FreeConfigPtrs(&maxcf);
        DEBOUT(0x1004);
        return FALSE;

    }

    /* Frame check sequence.

       Only use this with ERTM/SM, so first determine flow mode.
    */
    mode = L2CAUTO_DetermineMode(instance, &mincf);
    if ((mode == L2CA_FLOW_MODE_ENHANCED_RETRANS
                || mode == L2CA_FLOW_MODE_STREAMING)
            && (instance->ext_feats & L2CA_EXT_FEAT_OPT_FCS) != 0)
    {
        mincf.options |= L2CA_SPECIFY_FCS;
        mincf.fcs = maxcf.fcs;

        if(instance->backoff)
        {
            if(L2CAUTO_ConfHas(instance->backoff, L2CA_SPECIFY_FCS)
                && maxcf.fcs == instance->backoff->fcs)
            {
                /* Peer has already responded for the FCS sent in our configure
                 request, we have to enforce the default value if the peer
                 rejected this value. Default is FCS enabled.
                */
                 mincf.options &= ~L2CA_SPECIFY_FCS;
                 mincf.fcs = L2CA_FCS_ON;
                 maxcf.fcs = L2CA_FCS_ON;
             }
        }
    }

    else
    {
        mincf.fcs = L2CA_FCS_OFF;
        maxcf.fcs = L2CA_FCS_OFF;
        mincf.options &= ~L2CA_SPECIFY_FCS;
    }

#endif

    /* Flowspecs - may be disabled completely during reconfigurations
     * which means "keep current one". We must have these for AMP
     * and must not have them for BR/EDR. */
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
    if ((instance->state & (L2CAUTO_NO_FLOWSPEC_RECONF | L2CAUTO_AMP))
            == L2CAUTO_AMP)
    {
        l2ca_fs_service_t peer;
        CsrBool hasback;

        /* Emergency generation of local flowspec (best effort) */
        if(!L2CAUTO_ConfHas(&maxcf, L2CA_SPECIFY_FLOWSPEC))
        {
            maxcf.options |= L2CA_SPECIFY_FLOWSPEC;
            maxcf.flowspec = L2CA_AllocFlowspec();
            mincf.flowspec = L2CA_AllocFlowspec();
        }

        /* What mode does peer want? */
        if(L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_FLOWSPEC))
        {
            peer = backoff->flowspec->fs_service_type;
        }
        else
        {
            peer = L2CA_QOS_TYPE_BEST_EFFORT;
        }

        /* Choose service type */
        if(!L2CAUTO_BestQosService(maxcf.flowspec->fs_service_type,
                                   peer,
                                   instance->ext_feats))
        {
            /* Modes completely uncompatible */
            /* Marker: The spec is unclear whether we *shall*
             * disconnect here, or whether we are allowed to fail
             * normally due to exhaustion */
            L2CA_FreeConfigPtrs(&mincf);
            L2CA_FreeConfigPtrs(&maxcf);
            DEBOUT(0x1005);
            return FALSE;
        }

        /* Enable option */
        mincf.options |= L2CA_SPECIFY_FLOWSPEC;
        mincf.flowspec->fs_service_type = maxcf.flowspec->fs_service_type;
        mincf.flowspec->fs_identifier = 0; /* auto-detect */

        /* Transfer options, adhering to min, max and backoff */
        hasback = L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_FLOWSPEC);

        if(!L2CAUTO_Best16(&mincf.flowspec->fs_max_sdu, maxcf.flowspec->fs_max_sdu,
                           (CsrUint16*)(hasback ? &(backoff->flowspec->fs_max_sdu) : NULL)) ||
           !L2CAUTO_Best32(&mincf.flowspec->fs_interarrival, maxcf.flowspec->fs_interarrival,
                           (CsrUint32*)(hasback ? &(backoff->flowspec->fs_interarrival) : NULL)) ||
           !L2CAUTO_Best32(&mincf.flowspec->fs_latency, maxcf.flowspec->fs_latency,
                           (CsrUint32*)(hasback ? &(backoff->flowspec->fs_latency) : NULL)) ||
           !L2CAUTO_Best32(&mincf.flowspec->fs_flush_to, maxcf.flowspec->fs_flush_to,
                           (CsrUint32*)(hasback ? &(backoff->flowspec->fs_flush_to) : NULL)))
        {
            /* Error in selection - outside range */
            L2CA_FreeConfigPtrs(&mincf);
            L2CA_FreeConfigPtrs(&maxcf);
            DEBOUT(0x1006);
            return FALSE;
        }
    }
    else
    {
        /* We always support this and can do fallback to
         * best-effort */
    }
#endif

    /* Old school QoS */
#ifdef INSTALL_L2CAP_QOS_SUPPORT
    if(L2CAUTO_ConfHas(&maxcf, L2CA_SPECIFY_QOS) &&
       !(instance->unknown & L2CA_SPECIFY_QOS))
    {
        l2ca_service_type_t peer;

        /* What mode does peer want? */
        if(L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_QOS))
        {
            peer = backoff->qos->service_type;
        }
        else
        {
            peer = L2CA_QOS_TYPE_BEST_EFFORT;
        }

        /* Choose service type */
        if(!L2CAUTO_BestQosService(maxcf.qos->service_type,
                                   peer,
                                   instance->ext_feats))
        {
            /* Modes completely uncompatible */
            L2CA_FreeConfigPtrs(&mincf);
            L2CA_FreeConfigPtrs(&maxcf);
            DEBOUT(0x1007);
            return FALSE;
        }

        /* Enable option if either we or peer uses it */
        if((mincf.qos->service_type != 0) ||
           (maxcf.qos->service_type != L2CA_QOS_TYPE_BEST_EFFORT) ||
           (peer != L2CA_QOS_TYPE_BEST_EFFORT))
        {
            CsrBool hasback;
            mincf.options |= L2CA_SPECIFY_QOS;
            mincf.qos->service_type = maxcf.qos->service_type;
            mincf.qos->flags = 0; /* always zero */

            /* Transfer options, adhering to min, max and backoff */
            hasback =  L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_QOS);

            if(!L2CAUTO_Best32(&mincf.qos->token_rate, maxcf.qos->token_rate,
                               (CsrUint32*)(hasback ? &(backoff->qos->token_rate) : NULL)) ||
               !L2CAUTO_Best32(&mincf.qos->token_bucket, maxcf.qos->token_bucket,
                               (CsrUint32*)(hasback ? &(backoff->qos->token_bucket) : NULL)) ||
               !L2CAUTO_Best32(&mincf.qos->peak_bw, maxcf.qos->peak_bw,
                               (CsrUint32*)(hasback ? &(backoff->qos->peak_bw) : NULL)) ||
               !L2CAUTO_Best32(&mincf.qos->latency, maxcf.qos->latency,
                               (CsrUint32*)(hasback ? &(backoff->qos->latency) : NULL)) ||
               !L2CAUTO_Best32(&mincf.qos->delay_var, maxcf.qos->delay_var,
                               (CsrUint32*)(hasback ? &(backoff->qos->delay_var) : NULL)))
            {
                /* Error in selection - outside range */
                L2CA_FreeConfigPtrs(&mincf);
                L2CA_FreeConfigPtrs(&maxcf);
                DEBOUT(0x1008);
                return FALSE;
            }
        }
        else
        {
            /* No need for the QoS structure in best effort mode */
            CsrPmemFree(mincf.qos);
            CsrPmemFree(maxcf.qos);
            mincf.qos = NULL;
            maxcf.qos = NULL;
        }
    }
    else if(L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_QOS))
    {
        /* Peer wants QoS, but we dont. Fail */
        L2CA_FreeConfigPtrs(&mincf);
        L2CA_FreeConfigPtrs(&maxcf);
        DEBOUT(0x1009);
        return FALSE;
    }
#endif

    /* MTU */
    if(!(instance->unknown & L2CA_SPECIFY_MTU))
    {
        /* Note that maxcf will either contain our conftab maxmimum,
         * or the L2CAP spec default (even if bit not specified) */

        if(L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_MTU) &&
           (backoff->mtu > maxcf.mtu))
        {
            /* Peer wants us to accept something larger than what we
             * support */
            L2CA_FreeConfigPtrs(&mincf);
            L2CA_FreeConfigPtrs(&maxcf);
            DEBOUT(0x100A);
            return FALSE;
        }
        else if(L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_MTU) &&
                (backoff->mtu <= maxcf.mtu))
        {
            /* Peer is asking for a harmless, non-default MTU, so
             * accept */
            mincf.options |= L2CA_SPECIFY_MTU;
            mincf.mtu = backoff->mtu;
        }
        else if(L2CAUTO_ConfHas(&maxcf, L2CA_SPECIFY_MTU))
        {
            /* Ask for MTU */
            mincf.options |= L2CA_SPECIFY_MTU;
            mincf.mtu = maxcf.mtu;
        }
    }

    /* Flush timeout */
    if(!(instance->unknown & L2CA_SPECIFY_FLUSH) &&
       !(mincf.options & L2CA_SPECIFY_FLOWSPEC))
    {
        if(L2CAUTO_ConfHas(&maxcf, L2CA_SPECIFY_FLUSH))
        {
            /* We will now ask for this option */
            CsrBool hasback;
            mincf.options |= L2CA_SPECIFY_FLUSH;

            /* If peer has suggested a value see if it's usable or die. If
             * no suggested value just go with our maximum */
            hasback =  L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_FLUSH);

            if(!L2CAUTO_Best16(&mincf.flush_to, maxcf.flush_to,
                               (CsrUint16*)(hasback ? &(backoff->flush_to) : NULL)))
            {
                L2CA_FreeConfigPtrs(&mincf);
                L2CA_FreeConfigPtrs(&maxcf);
                DEBOUT(0x100B);
                return FALSE;
            }
        }
        else if(L2CAUTO_ConfHas(backoff, L2CA_SPECIFY_FLUSH) &&
                (backoff->flush_to != L2CA_FLUSH_TO_DEFAULT))
        {
            /* Peer is asking for a non-default flush timeout and our
             * conftab doesn't include any flush timeout.  This means
             * we're switching from reliable to non-reliable - this
             * isn't legal so drop out */
            L2CA_FreeConfigPtrs(&mincf);
            L2CA_FreeConfigPtrs(&maxcf);
            DEBOUT(0x100C);
            return FALSE;
        }
    }

    /* L2CAP channel priority. */
    mincf.options |= (instance->channel_priority << L2CA_OPTION_PRIORITY_SHIFT)
        & L2CA_SPECIFY_PRIORITY_INTERNAL;

    /* Store agreed results and send request we've built in
     * the "mincf" - this will transfer ownership of pointers. Free
     * the "maxcf" pointers */
    L2CAUTO_StoreResults(instance, &mincf, TRUE);
    L2CA_FreeConfigPtrs(&maxcf);
    L2CA_ConfigReqCs(instance->cid, &mincf);

    /* All good */
    return TRUE;
}

/*! \brief Scan for unknown options

    Scan the incoming config request for unknown options and reject
    peer's request if any are found. Return TRUE if we passed (no
    unknown options found), FALSE if unknown options found and
    rejected.
*/
static CsrBool L2CAUTO_ConfigIndUnknown(l2ca_cid_t cid,
                                       l2ca_identifier_t identifier,
                                       L2CA_CONFIG_T *config)

{
    l2ca_options_t unknown;

    /* Remove things we know about - this is compiler flag
     * dependent */
    unknown = (l2ca_options_t)(config->options &
            ~(L2CA_SPECIFY_MTU | L2CA_SPECIFY_QOS | L2CA_SPECIFY_FLUSH));

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    unknown &= ~(L2CA_SPECIFY_FLOW | L2CA_SPECIFY_FCS);
#endif
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
    unknown &= ~(L2CA_SPECIFY_FLOWSPEC);
#endif
#ifdef INSTALL_AMP_SUPPORT
    unknown &= ~(L2CA_SPECIFY_EXT_WINDOW);
#endif

    /* Check for any unknown options. If present, reject them
     * immediately */
    if((config->unknown_length > 0) ||
       (unknown != 0))
    {
        /* We know we will reject something, thus we can reuse what
         * the peer sent us. Simply remove the bits we know - what's
         * left are the unknown parts that we need to reply with */
        config->options = unknown;
        L2CA_CullConfigPtrs(config);

        L2CA_ConfigRspCs(cid,
                         identifier,
                         L2CA_CONFIG_UNKNOWN,
                         config);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*! \brief Incoming configuration indication

    Scan the peer's configuration indication to see whether the
    options can be accepted based on the conftab. This implies that we
    rely on the iterator to be valid.

    The plan for validating the indication:
    1, check for unknown parameters and reject if found
    2, check flow mode
       a, if we require FEC, either backoff or reject
       b, if we don't want FEC and peer does, reject
    3, flowspec
    4, qos
    5, fcs
    6, extended windows
    7, mtu
    8, flush timeout
    9, send success or unacceptable

    \return TRUE for now - can be ignored.
*/
static CsrBool L2CAUTO_ConftabInd(L2CAUTO_INSTANCE_T *instance,
                                 L2CA_CONFIG_IND_T *prim)
{
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    l2ca_flow_mode_t mode;
#endif
    L2CA_CONFIG_T mincf;
    L2CA_CONFIG_T maxcf;
    l2ca_conf_result_t result;

    /* Read in RSP directional min/max options from conftab. If
     * conftab is empty, mincf/maxcf will be cleared and empty */
    if(!L2CAUTO_ConftabRead(instance,
                            instance->curr_block,
                            FALSE, &mincf, &maxcf))
    {
        /* Error in iterator - prepare to fail */
        instance->curr_block = instance->num_blocks + 1;
        instance->retry = L2CAP_AUTOCONNECT_RETRY;
        instance->peer_modes_left = (CsrUint16)~0;
#ifdef INSTALL_L2CAP_DEBUG
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
#endif
        DEBOUT(0x4600);
        L2CAUTO_FatalError(instance, L2CA_CONNECT_INVALID_CONFTAB);
        return FALSE;
    }

    result = L2CA_CONFIG_SUCCESS;

    /* Flow and error control check */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    if(L2CAUTO_ConfHas(&maxcf, L2CA_SPECIFY_FLOW))
    {
        l2ca_flow_mode_t peer;
        CsrBool haspeer;

        /* What har peer just asked for? */
        if(L2CAUTO_ConfHas(&prim->config, L2CA_SPECIFY_FLOW))
        {
            peer = prim->config.flow->mode;
            haspeer = TRUE;
        }
        else
        {
            peer = L2CA_FLOW_MODE_BASIC;
            haspeer = FALSE;
        }

        /* Choose best mode */
        if(!L2CAUTO_BestFlowModeInd(&maxcf.flow->mode, /* preferred */
                                    mincf.flow->mode,  /* allow mask */
                                    peer))
        {
            /* We can't use peer's mode. Reject */
            DEBOUT(0x2000);
            result = L2CA_CONFIG_UNACCEPTABLE;

            /* The specification mandates us to issue a disconnect if
             * we don't have any backoff modes -- or if peer insists
             * on the same mode multiple times */
            if((mincf.flow->mode == 0) ||
               !(instance->peer_modes_left & L2CA_MODE_MASK(peer)))
            {
                DEBOUT(0x4800);
                L2CA_FreeConfigPtrs(&maxcf);
                L2CA_FreeConfigPtrs(&mincf);
                L2CAUTO_FatalError(instance, L2CA_CONNECT_CONFTAB_EXHAUSTED);
                return FALSE;
            }

            /* Peer can no longer ask for this particular mode */
            instance->peer_modes_left &= ~L2CA_MODE_MASK(peer);
        }

        /* Basic is all zeros */
        if(maxcf.flow->mode == L2CA_FLOW_MODE_BASIC)
        {
            CsrMemSet(mincf.flow, 0, sizeof(L2CA_FLOW_T));

            /* Explicitly send F&EC options if peer asked for
             * non-basic */
            if(peer != L2CA_FLOW_MODE_BASIC)
                mincf.options |= L2CA_SPECIFY_FLOW;
        }
        /* Otherwise, deal with whatever mode it is */
        else if(!(instance->unknown & L2CA_SPECIFY_FLOW))
        {
            /* Reject or not, we're going with FEC */
            mincf.options |= L2CA_SPECIFY_FLOW;
            mincf.flow->mode = maxcf.flow->mode;

            if(haspeer)
            {
                if(!L2CAUTO_Best16(&mincf.flow->maximum_pdu,
                                   maxcf.flow->maximum_pdu,
                                   &prim->config.flow->maximum_pdu))
                {
                    /* Don't reject MPS if out of range, just return
                     * smaller value in response */
                    mincf.flow->maximum_pdu = prim->config.flow->maximum_pdu;
                }
                else
                {
                    /* Values in prim->config are what we report to the
                     * upper layer, so force whatever value we told the
                     * peer we'll use */
                    prim->config.flow->maximum_pdu = mincf.flow->maximum_pdu;
                }
            }

            /* Streaming mode doesn't want timers or window sizes */
            if(mincf.flow->mode == L2CA_FLOW_MODE_STREAMING)
            {
                /* All except MPS is zero in SM */
                mincf.flow->tx_window = 0;
                mincf.flow->max_retransmit = 0;
                mincf.flow->retrans_timeout = 0;
                mincf.flow->monitor_timeout = 0;
            }
            else
            {
                l2ca_ext_window_size_t peer_win;

                /* Retransmission and monitor timer and max_retransmit must be passed straight
                 * through */
                if(haspeer)
                {
                    mincf.flow->max_retransmit = prim->config.flow->max_retransmit;
                    mincf.flow->retrans_timeout = prim->config.flow->retrans_timeout;
                    mincf.flow->monitor_timeout = prim->config.flow->monitor_timeout;
                }

                /* Check peer's window size against our requirement */
                if(L2CAUTO_ConfHas(&prim->config, L2CA_SPECIFY_EXT_WINDOW))
                {
                    peer_win = prim->config.ext_window;
                }
                else if(haspeer)
                {
                    peer_win = prim->config.flow->tx_window;
                }
                else
                {
                    /* Assume optimistic default */
                    peer_win = maxcf.ext_window;
                }

                /* Defend against peer's insanity. */
                if (peer_win == 0)
                    peer_win = 1;

#ifndef BUILD_FOR_HOST
                {
                    CsrUint16 mps_limit;

                    /* When on chip, buffer sizes will constrain our ability to
                       saturate a large TX window / MPS combination. Unless we
                       can saturate this window, we will be at the mercy of
                       whatever ACKing algorithm the peer implements. So clip
                       MPS/TX window to fit a 1K buffer. This probably means
                       that you definitely won't want RFCOMM going over ERTM.
                       But then you'd have to be mad to do that, surely... */

                    /* Clip TX_Window to something manageable. */
                    if (peer_win > L2CAP_MAX_TX_WINDOW)
                        peer_win = L2CAP_MAX_TX_WINDOW;

                    /* Clip MPS so that we can saturate TX Window with
                       a 1K buffer. */
                    mps_limit =
                        BUFFER_SIZE_IN_BYTES(BUFFER_OF_SIZE(L2CAP_BUFFER_SIZE))
                        / peer_win;

                    if (mincf.flow->maximum_pdu > mps_limit)
                        mincf.flow->maximum_pdu = mps_limit;
                }
#endif

                /* Don't reject window if out of range, just return smaller value in response */
                if (peer_win <= mincf.ext_window || peer_win <= maxcf.ext_window)
                {
                    /* Peer window in range, so accept */
                    mincf.ext_window = peer_win;
                }

                /* If the window size did spill into the extended range,
                 * set the option but cull the standard range */
                if(mincf.ext_window >= L2CAP_STD_MAX_SEQUENCE)
                {
                    mincf.flow->tx_window = L2CAP_STD_MAX_SEQUENCE - 1;
                    mincf.options |= L2CA_SPECIFY_EXT_WINDOW;
                }
                else
                {
                    mincf.flow->tx_window = (l2ca_window_size_t)mincf.ext_window;
                }
            }
        }
    }
    else if(L2CAUTO_ConfHas(&prim->config, L2CA_SPECIFY_FLOW)
            && prim->config.flow->mode != L2CA_FLOW_MODE_BASIC)
    {
        /* Only peer wants FEC - we have to reject this as no FEC on
         * our side means only basic mode is acceptable */
        DEBOUT(0x2003);
        result = L2CA_CONFIG_UNACCEPTABLE;

        /* Response explicitly requests basic mode. */
        CsrPmemFree(mincf.flow);
        mincf.flow = zpnew(L2CA_FLOW_T);
        mincf.options |= L2CA_SPECIFY_FLOW;
    }


    /* FCS. Because 'on' is the default and also takes
     * priority, simply allow this one to go through if one of
     * us has set it. --But only ever send it with ERTM/SM */
    mode = L2CAUTO_DetermineMode(instance, &mincf);
    if ((mode == L2CA_FLOW_MODE_ENHANCED_RETRANS
                || mode == L2CA_FLOW_MODE_STREAMING)
            && (instance->ext_feats & L2CA_EXT_FEAT_OPT_FCS) != 0)
    {
        /* Do note the special value encoding form the
         * conftab: 0=don't care, 1=use, 2=avoid */
        l2ca_fcs_t peer_fcs;
        l2ca_fcs_t our_fcs;

        peer_fcs = (l2ca_fcs_t)(L2CAUTO_ConfHas(&prim->config, L2CA_SPECIFY_FCS)
                                ? prim->config.fcs : L2CA_FCS_ON);
        our_fcs =   (l2ca_fcs_t)(L2CAUTO_ConfHas(&maxcf, L2CA_SPECIFY_FCS)
                                 ? maxcf.fcs : L2CA_FCS_ON);

        /* We're going to ask for FCS no matter what */
        mincf.options |= L2CA_SPECIFY_FCS;
        if (peer_fcs == L2CA_FCS_OFF && our_fcs == L2CA_FCS_OFF)
        {
            mincf.fcs = L2CA_FCS_OFF;
        }
        else
        {
            mincf.fcs = L2CA_FCS_ON;
        }

        /* Brute-force the correct FCS value in to the results
         * by setting the prim->result */
        prim->config.fcs = mincf.fcs;
        /* Since prim is used for making the results and
          * we would like to fill the correct value of FCS
          * in the results so, we forcefully set the options
          * with L2CA_SPECIFY_FCS.
        */
        prim->config.options |= L2CA_SPECIFY_FCS;
    }
#endif

    /* Flowspecs */
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
    if((instance->state & L2CAUTO_AMP) != 0 && L2CAUTO_ConfHas(&maxcf, L2CA_SPECIFY_FLOWSPEC))
    {
        /* We need Flowspecs and we've got one. */
        l2ca_fs_service_t peer;
        CsrBool haspeer;

        /* What does peer want? */
        if(L2CAUTO_ConfHas(&prim->config, L2CA_SPECIFY_FLOWSPEC))
        {
            peer = prim->config.flowspec->fs_service_type;
            haspeer = TRUE;
        }
        else
        {
            peer = maxcf.flowspec->fs_service_type;
            haspeer = FALSE;
        }

        /* Choose service type */
        if(!L2CAUTO_BestQosService(maxcf.flowspec->fs_service_type,
                                   peer,
                                   instance->ext_feats))
        {
            /* We can't use peer's mode. Reject */
            /* Marker: The spec is unclear whether we *shall*
             * disconnect here, or whether we are allowed to fail
             * normally due to exhaustion */
            DEBOUT(0x2004);
            result = L2CA_CONFIG_UNACCEPTABLE;
        }

        /* Reject or not - we're going to ask for flowspecs */
        mincf.options |= L2CA_SPECIFY_FLOWSPEC;
        mincf.flowspec->fs_service_type = maxcf.flowspec->fs_service_type;
        mincf.flowspec->fs_identifier = 0; /* auto-detect */

        if(!L2CAUTO_Best16(&mincf.flowspec->fs_max_sdu, maxcf.flowspec->fs_max_sdu,
                    (CsrUint16*)(haspeer ? &(prim->config.flowspec->fs_max_sdu) : NULL)))
        {
            DEBOUT(0x2005);
            result = L2CA_CONFIG_FLOWSPEC_REJECTED;
        }
        if(!L2CAUTO_Best32(&mincf.flowspec->fs_interarrival, maxcf.flowspec->fs_interarrival,
                    (CsrUint32*)(haspeer ? &(prim->config.flowspec->fs_interarrival) : NULL)))
        {
            DEBOUT(0x2005);
            result = L2CA_CONFIG_FLOWSPEC_REJECTED;
        }
        if(!L2CAUTO_Best32(&mincf.flowspec->fs_latency, maxcf.flowspec->fs_latency,
                    (CsrUint32*)(haspeer ? &(prim->config.flowspec->fs_latency) : NULL)))
        {
            DEBOUT(0x2005);
            result = L2CA_CONFIG_FLOWSPEC_REJECTED;
        }
        if(!L2CAUTO_Best32(&mincf.flowspec->fs_flush_to, maxcf.flowspec->fs_flush_to,
                    (CsrUint32*)(haspeer ? &(prim->config.flowspec->fs_flush_to) : NULL)))
        {
            /* Some of the other values were unacceptable */
            DEBOUT(0x2005);
            result = L2CA_CONFIG_FLOWSPEC_REJECTED;
        }
    }
    else if(L2CAUTO_ConfHas(&prim->config, L2CA_SPECIFY_FLOWSPEC) &&
            prim->config.flowspec->fs_service_type != L2CA_QOS_TYPE_BEST_EFFORT)
    {
        /* Peer wants guaranteed flowspec and we don't support it. */
        DEBOUT(0x2006);
        result = L2CA_CONFIG_UNACCEPTABLE;

        CsrPmemFree(mincf.flowspec);
        mincf.flowspec = L2CA_AllocFlowspec();
    }
#endif

    /* Old school QoS */
#ifdef INSTALL_L2CAP_QOS_SUPPORT
    if(L2CAUTO_ConfHas(&maxcf, L2CA_SPECIFY_QOS))
    {
        l2ca_fs_service_t peer;
        CsrBool haspeer;

        /* What does peer want? */
        if(L2CAUTO_ConfHas(&prim->config, L2CA_SPECIFY_QOS))
        {
            peer = prim->config.qos->service_type;
            haspeer = TRUE;
        }
        else
        {
            peer = L2CA_QOS_TYPE_BEST_EFFORT;
            haspeer = FALSE;
        }

        /* Choose service type */
        if(!L2CAUTO_BestQosService(maxcf.qos->service_type,
                                   peer,
                                   instance->ext_feats))
        {
            /* We can't use peer's mode. Reject */
            DEBOUT(0x2007);
            result = L2CA_CONFIG_UNACCEPTABLE;
        }

        /* Reject or not - we're going to ask for QoS */
        mincf.options |= L2CA_SPECIFY_QOS;
        mincf.qos->service_type = maxcf.qos->service_type;
        mincf.qos->flags = 0; /* always zero */

        if(!L2CAUTO_Best32(&mincf.qos->token_rate, maxcf.qos->token_rate,
                           (CsrUint32*)(haspeer ? &(prim->config.qos->token_rate) : NULL)))
        {
            DEBOUT(0x2008);
            result = L2CA_CONFIG_UNACCEPTABLE;
        }
        if(!L2CAUTO_Best32(&mincf.qos->token_bucket, maxcf.qos->token_bucket,
                           (CsrUint32*)(haspeer ? &(prim->config.qos->token_bucket) : NULL)))
        {
            DEBOUT(0x2008);
            result = L2CA_CONFIG_UNACCEPTABLE;
        }
        if(!L2CAUTO_Best32(&mincf.qos->peak_bw, maxcf.qos->peak_bw,
                           (CsrUint32*)(haspeer ? &(prim->config.qos->peak_bw) : NULL)))
        {
            DEBOUT(0x2008);
            result = L2CA_CONFIG_UNACCEPTABLE;
        }
        if(!L2CAUTO_Best32(&mincf.qos->latency, maxcf.qos->latency,
                           (CsrUint32*)(haspeer ? &(prim->config.qos->latency) : NULL)))
        {
            DEBOUT(0x2008);
            result = L2CA_CONFIG_UNACCEPTABLE;
        }
        if(!L2CAUTO_Best32(&mincf.qos->delay_var, maxcf.qos->delay_var,
                           (CsrUint32*)(haspeer ? &(prim->config.qos->delay_var) : NULL)))
        {
            DEBOUT(0x2008);
            result = L2CA_CONFIG_UNACCEPTABLE;
        }
    }
    else
#endif
    if(L2CAUTO_ConfHas(&prim->config, L2CA_SPECIFY_QOS) &&
            (prim->config.qos->service_type != L2CA_QOS_TYPE_BEST_EFFORT))
    {
        /* Peer wants non-best effort QoS and we don't have any QoS
         * (and thus wants best-effort). Unacceptable. */
        DEBOUT(0x2009);
        result = L2CA_CONFIG_UNACCEPTABLE;

        /* Response explicitly requests best effort. */
        CsrPmemFree(mincf.qos);
        mincf.qos = zpnew(L2CA_QOS_T);
        mincf.options |= L2CA_SPECIFY_QOS;

        /* Set non-zero best-effor defaults. */
        mincf.qos->service_type = L2CA_QOS_TYPE_BEST_EFFORT;
        mincf.qos->latency = L2CA_QOS_DEFAULT_LATENCY;
        mincf.qos->delay_var = L2CA_QOS_DEFAULT_DELAY_VAR;
    }

    /* MTU */
    if(L2CAUTO_ConfHas(&maxcf, L2CA_SPECIFY_MTU) ||
       (L2CAUTO_ConfHas(&prim->config, L2CA_SPECIFY_MTU) &&
        (prim->config.mtu != L2CA_MTU_DEFAULT)))
    {
        l2ca_mtu_t peer;
        mincf.options |= L2CA_SPECIFY_MTU;

        /* What does peer want */
        peer = (l2ca_mtu_t)(L2CAUTO_ConfHas(&prim->config, L2CA_SPECIFY_MTU)
                            ? prim->config.mtu
                            : L2CA_MTU_DEFAULT);

        /* The MTU is specified as a minimum only, so no need to
         * complain unless peer is below */
        if(peer < mincf.mtu)
        {
            DEBOUT(0x200A);
            result = L2CA_CONFIG_UNACCEPTABLE;
        }
        else
        {
            /* As we can accept what the peer wants, be nice and
             * reflect what she requested */
            mincf.mtu = peer;
        }
    }

    /* Flush timeout */
    if(!(mincf.options & L2CA_SPECIFY_FLOWSPEC) &&
       (L2CAUTO_ConfHas(&maxcf, L2CA_SPECIFY_FLUSH) ||
        (L2CAUTO_ConfHas(&prim->config, L2CA_SPECIFY_FLUSH) &&
         (prim->config.flush_to != L2CA_FLUSH_TO_DEFAULT))))
    {
        l2ca_timeout_t peer;
        mincf.options |= L2CA_SPECIFY_FLUSH;

        /* What does peer want */
        if(L2CAUTO_ConfHas(&prim->config, L2CA_SPECIFY_FLUSH))
        {
            peer = prim->config.flush_to;
        }
        else
        {
            peer = L2CA_FLUSH_TO_DEFAULT;
        }

        /* Can it be accepted? */
        if(!L2CAUTO_Best16(&mincf.flush_to, maxcf.flush_to, &peer))
        {
            DEBOUT(0x200B);
            result = L2CA_CONFIG_UNACCEPTABLE;
        }
        else
        {
            /* As we can accept what the peer wants, be nice and
             * reflect what she requested */
            mincf.flush_to = peer;
        }
    }

    /* Store results and send response */
    if(result == L2CA_CONFIG_SUCCESS)
    {
        L2CAUTO_StoreResults(instance, &prim->config, FALSE);
        instance->state |= L2CAUTO_CONFIG_REMOTE_OK;
    }
    else
    {
        /* Unaceptable - clear results */
        L2CAUTO_ClearResults(instance, prim->config.options,
                             TRUE); /* response direction */
    }

    L2CA_FreeConfigPtrs(&maxcf);
    L2CA_ConfigRspCs(prim->cid,
                     prim->identifier,
                     result,
                     &mincf);

    /* All good */
    return TRUE;
}

/*! \brief Scan for one-shot cached elements

    Scan the conftab table, making sure that the the full length
    actually matches where the key,value set ends.

    The conftab may include cached elements such as the getinfo
    extended features. Extract these and update the internal state
*/
static void L2CAUTO_ConftabCache(L2CAUTO_INSTANCE_T *instance)
{
    BKV_ITERATOR_T iter;

    iter.block = instance->conftab;
    iter.size = instance->length;

    /* Reset iterator and count number of blocks */
    instance->curr_block = 1;
    instance->num_blocks = BKV_CountBlocks(iter.block, iter.size, FALSE); /* don't cross barrier */
    instance->retry = L2CAP_AUTOCONNECT_RETRY;
    instance->peer_modes_left = (CsrUint16)~0;
    instance->channel_priority = L2CAP_MAX_TX_QUEUES - 1;

    /* Static options must be in first block */
    if(BKV_JumpToBlock(&iter, instance->curr_block))
    {
        CsrUint32 ext_feats;
        DM_SM_SERVICE_T service;

        /* Extract extended features from first conftab */
        if(BKV_Scan32Single(&iter,
                            L2CA_AUTOPT_EXT_FEATS,
                            &ext_feats))
        {
            instance->state |= (L2CAUTO_FEATS_REQUESTED |
                                L2CAUTO_FEATS_OK);

            instance->ext_feats = (l2ca_ext_feats_t)ext_feats;
        }

        /* Extract reconfiguration disable */
        (void)BKV_Scan16Single(&iter,
                               L2CA_AUTOPT_DISABLE_RECONF,
                               &instance->disable_reconf);

        /* Extract channel priority, defaulting to least priority. */
        if(!BKV_Scan16Single(&iter,
                             L2CA_AUTOPT_CHANNEL_PRIORITY,
                             &instance->channel_priority))
        {
            /* No priority defaults to lowest */
            instance->channel_priority = L2CAP_MAX_TX_QUEUES - 1;
        }

        if (instance->channel_priority > L2CAP_MAX_TX_QUEUES - 1)
            instance->channel_priority = L2CAP_MAX_TX_QUEUES - 1;

        /* Extract alternative security service description */
        CsrPmemFree(instance->dm_sm_service);
        instance->dm_sm_service = NULL;
        if (BKV_Scan16Single(&iter,
                             L2CA_AUTOPT_SECURITY_PROTOCOL,
                             &service.protocol_id) &&
            BKV_Scan16Single(&iter,
                             L2CA_AUTOPT_SECURITY_CHANNEL,
                             &service.channel))
        {
            instance->dm_sm_service = pcopy(&service, sizeof(DM_SM_SERVICE_T));
        }
    }
    else if(iter.block == NULL)
    {
        /* Empty conftabs use special iterator of zero. This allows us
         * to use "default" values and still fail if they can't be
         * accepted */
        instance->curr_block = 0;
    }
}

/*! \brief Check if CID has been connected and configured */
static CsrBool L2CAUTO_IsComplete(L2CAUTO_INSTANCE_T *instance)
{
    /* If dying, exist immediately. Disconnect already sent. */
    if(instance->state & L2CAUTO_DYING)
    {
        return FALSE;
    }

    if((instance->state & L2CAUTO_CONNECTION_OK) &&
       (instance->state & L2CAUTO_FEATS_OK) &&
       (instance->state & L2CAUTO_CONFIG_LOCAL_OK) &&
       (instance->state & L2CAUTO_CONFIG_REMOTE_OK))
    {
        /* Enter active state */
        instance->state |= L2CAUTO_ACTIVE_OK;

        /* Clear other flags to prepare for reconfiguration */
        instance->state &= ~(L2CAUTO_CONFIG_REQUESTED |
                             L2CAUTO_CONFIG_LOCAL_OK |
                             L2CAUTO_CONFIG_REMOTE_OK |
                             L2CAUTO_NO_FLOWSPEC_RECONF);

        if(instance->saved_conftab)
        {
            /* Free temporary buffers */
            bpfree(instance->conftab);
            instance->conftab = NULL;
            instance->length = 0;

            if(instance->backoff)
            {
                L2CA_FreeConfigPtrs(instance->backoff);
                CsrPmemFree(instance->backoff);
                instance->backoff = NULL;
            }

            /* We had a queued request, trigger the re-config with this conftab */
            instance->conftab = instance->saved_conftab;
            instance->length = instance->saved_length;
            instance->saved_conftab = NULL;
            instance->saved_length = 0;

            /* Clear state and kick reconfiguration */
            instance->state &= ~(L2CAUTO_CONFIG_REQUESTED |
                                 L2CAUTO_CONFIG_LOCAL_OK |
                                 L2CAUTO_CONFIG_REMOTE_OK |
                                 L2CAUTO_ACTIVE_OK);
            return L2CAUTO_KickConfig(instance);
        }

        /* Inform upper layer. This function will consume and clear
         * the results */
        L2CAUTO_SendAutoConnectCfm(instance,
                                   L2CA_CONNECT_SUCCESS,
                                   &instance->results);

        /* Free temporary buffers */
        bpfree(instance->conftab);
        instance->conftab = NULL;
        instance->length = 0;
        if(instance->backoff)
        {
            L2CA_FreeConfigPtrs(instance->backoff);
            CsrPmemFree(instance->backoff);
            instance->backoff = NULL;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*! \brief Check for obsolete/unsupported requests

    Make sure application isn't being naughty and prohibit
    it from requesting legacy F&EC modes
*/
static void L2CAUTO_ObsoleteCheck(L2CAUTO_INSTANCE_T *instance)
{
    BKV_ITERATOR_T iter;
    CsrUint16 block;

    iter.block = instance->conftab;
    iter.size = instance->length;

    /* Scan all blocks */
    for(block = 1; BKV_JumpToBlock(&iter, block); block++)
    {
        CsrUint16 mode;
        if(BKV_Scan16Single(&iter,
                            L2CA_AUTOPT_FLOW_MODE,
                            &mode))
        {
            mode >>= 8;

            /* Check for illegal mode */
            if(mode == L2CA_FLOW_MODE_RTM_OBSOLETE ||
                    mode == L2CA_FLOW_MODE_FC_OBSOLETE)
            {
                instance->state |= L2CAUTO_DYING;
                return;
            }
        }
    }

}
/*! \brief Make sure peer supports the mandatory features
    \return TRUE for normal exit, FALSE for fatal exception.
 */
static CsrBool L2CAUTO_ExtFeatsCheck(L2CAUTO_INSTANCE_T *instance)
{
    l2ca_ext_feats_t my_feats;
    CsrUint16 block;

    /* We're not ready to check for this, so bail out */
    if(!(instance->state & L2CAUTO_FEATS_OK))
    {
        return TRUE;
    }

    /* If our config-block is empty, we're asking for basic mode and
     * this can never fail */
    if(instance->num_blocks == 0)
    {
        return TRUE;
    }

    /* If we have requested an AMP and peer doesn't have flowspecs or
     * fixed channels, this always fails */
    if((instance->state & L2CAUTO_AMP) &&
       !(instance->ext_feats & L2CA_EXT_FEAT_FIXED_CH))
    {
        /* Fatal error */
        DEBOUT(0x3000);
        L2CAUTO_FatalError(instance, L2CA_CONNECT_CONFTAB_UNSUPPORTED);
        return FALSE;
    }

    /* Expect we require everything, and then subtract bits as we find
     * blocks that doesn't certain features. We don't check against
     * FCS, fixed channels and extended window sizes as fallback is
     * always guaranteed for those */
    my_feats = (L2CA_EXT_FEAT_ENH_RETRANS |
                L2CA_EXT_FEAT_STREAMING |
                L2CA_EXT_FEAT_BIDIR_QOS);

    /* Scan all blocks */
    for(block = 1; (block <= instance->num_blocks) && (my_feats != 0); block++)
    {
         L2CA_CONFIG_T mincf;
         L2CA_CONFIG_T maxcf;

        (void)L2CAUTO_ConftabRead(instance, block, TRUE, &mincf, &maxcf);

        /* Flow control options */
        if(L2CAUTO_ConfHas(&maxcf, L2CA_SPECIFY_FLOW))
        {
            /* We need to combine preferred with allowed */
            mincf.flow->mode |= L2CA_MODE_MASK(maxcf.flow->mode);

            /* Basic mode removes all requirements */
            if(mincf.flow->mode & L2CA_MODE_MASK_BASIC)
            {
                my_feats &= ~(L2CA_EXT_FEAT_ENH_RETRANS |
                              L2CA_EXT_FEAT_STREAMING);
            }

            /* Remove feature requirement one at the time */
            if(!(mincf.flow->mode & L2CA_MODE_MASK_ENHANCED_RETRANS))
            {
                my_feats &= ~(L2CA_EXT_FEAT_ENH_RETRANS);
            }
            if(!(mincf.flow->mode & L2CA_MODE_MASK_STREAMING))
            {
                my_feats &= ~(L2CA_EXT_FEAT_STREAMING);
            }
        }
        else
        {
            /* No FEC requirements in - drop all */
            my_feats &= ~(L2CA_EXT_FEAT_ENH_RETRANS |
                          L2CA_EXT_FEAT_STREAMING);
        }

        /* Old QoS */
        if(L2CAUTO_ConfHas(&maxcf, L2CA_SPECIFY_QOS))
        {
            /* Drop requirement if best effort is allowed */
            if(mincf.qos->service_type == L2CA_QOS_TYPE_BEST_EFFORT)
                my_feats &= ~(L2CA_EXT_FEAT_BIDIR_QOS);
        }
        else
        {
            /* No QoS requirement - drop all */
            my_feats &= ~(L2CA_EXT_FEAT_BIDIR_QOS);
        }

        L2CA_FreeConfigPtrs(&mincf);
        L2CA_FreeConfigPtrs(&maxcf);
    }

    /* If we require features, make sure peer supports them */
    if((instance->ext_feats & my_feats) != my_feats)
    {
        DEBOUT(0x3001);
        L2CAUTO_FatalError(instance, L2CA_CONNECT_CONFTAB_UNSUPPORTED);
        return FALSE;
    }

    return TRUE;
}

/*! \brief Initiate configuration
    \returns TRUE for normal exit, FALSE for fatal exception. */
static CsrBool L2CAUTO_KickConfig(L2CAUTO_INSTANCE_T *instance)
{
    /* If dying, exist immediately. Disconnect already sent. */
    if(instance->state & L2CAUTO_DYING)
    {
        return TRUE;
    }

    /* We can only move forward when we know the link is up */
    if(instance->state & L2CAUTO_CONNECTION_OK)
    {
        /* Kick off GETINFO if not done so already */
        if(!(instance->state & L2CAUTO_FEATS_REQUESTED))
        {
            CsrUint16 n;
            BKV_ITERATOR_T iter;
            iter.block = instance->conftab;
            iter.size = instance->length;

            instance->state |= L2CAUTO_FEATS_REQUESTED;

            /* Only need to do GetInfo for enhanced L2CAP modes.
               So search conftab for any of these modes. */
            for(n = 0;;)
            {
                CsrUint16 modes;

                /* Jump to the next block. */
                if (!BKV_JumpToBlock(&iter, ++n))
                {
                    /* We've fallen off the end without finding eL2CAP modes */
                    instance->state |= L2CAUTO_FEATS_OK;
                    break;
                }

                /* Search block for eL2CAP modes */
                if (BKV_Scan16Single(&iter,
                                     L2CA_AUTOPT_FLOW_MODE,
                                     &modes))
                {
                    /* The MSO of modes is the prefered mode
                       The LSO of modes is a bitmask of acceptable fallbacks */
                    if (L2CA_FLOW_MODE_BASIC != (modes >> 8) || 0 != (modes &
                                (L2CA_MODE_MASK_ENHANCED_RETRANS |
                                 L2CA_MODE_MASK_STREAMING)))
                    {
                        /* Found one, so must do GetInfo */
                        L2CA_GetInfoReq(&instance->bd_addr,
                                        instance->phandle,
                                        L2CA_GETINFO_TYPE_EXT_FEAT,
                                        (CsrUint16)instance->cid, /* con_ctx */
                                        0); /* BR/EDR only */
                        return TRUE;
                    }
                }
            }
        }

        /* Kick off config if not already done so */
        if(!(instance->state & L2CAUTO_CONFIG_REQUESTED) &&
                (instance->state & L2CAUTO_FEATS_OK))
        {
            /* This is the main configuration retry loop. We keep
             * trying the conftab until either a request has been
             * sent, or we've run out of options. Note that if conftab
             * is NULL, curr_block will be zero so we still allow one
             * go */
            while((instance->curr_block <= instance->num_blocks) &&
                  !(instance->state & L2CAUTO_DYING))
            {
                if(L2CAUTO_ConftabReq(instance))
                {
                    instance->state |= L2CAUTO_CONFIG_REQUESTED;
                    break;
                }
                instance->curr_block++;
                instance->retry = L2CAP_AUTOCONNECT_RETRY;
                instance->peer_modes_left = (CsrUint16)~0;
            }

            /* If configuration has not been sent here, we're
             * exhausted and must start the failure shutdown */
            if(!(instance->state & L2CAUTO_CONFIG_REQUESTED))
            {
                DEBOUT(0x4100);
                L2CAUTO_FatalError(instance, L2CA_CONNECT_CONFTAB_EXHAUSTED);
                return FALSE;
            }

            /* Detect bad peers - retry counter exhausted */
            instance->retry--;
            if(instance->retry == 0)
            {
                DEBOUT(0x4700);
                L2CAUTO_FatalError(instance, L2CA_CONNECT_CONFTAB_EXHAUSTED);
                return FALSE;
            }
        }
        else
        {
            /* Both getinfo and config already requested. We're
             * waiting for the peer at this point */
        }
    }

    return TRUE;
}

/*! \brief Handle a L2CA_GETINFO_CFM

    Handle a get information response with the extended features
    bitmask.  Note that very old devices may not know about this
    feature, so error results are to be expected
*/
static void L2CAUTO_HandleL2caGetInfoCfm(L2CAUTO_INSTANCE_T *instance,
                                         L2CA_GETINFO_CFM_T *prim)
{
    /* Features received, error or not */
    instance->state |= L2CAUTO_FEATS_OK;

    /* Store bitmask if valid */
    if((prim->result == L2CA_GETINFO_SUCCESS) &&
       (prim->length == L2CA_GETINFO_SIZE_EXT_FEAT))
    {
        const CsrUint8 *data = prim->info_data;
        instance->ext_feats = (l2ca_ext_feats_t)read_uint32(&data);
    }

    /* Check for mandatory info support */
    if (!L2CAUTO_ExtFeatsCheck(instance))
        return;

    /* Move on to next step */
    (void)L2CAUTO_KickConfig(instance);
}

/*! \brief Handle L2CA_CONNECT_IND

    Handle a L2CA_CONNECT_IND sent from L2CAP. This means allocating
    an instance (if none such exist) and then inform application of
    this.
*/
static void L2CAUTO_HandleL2caCreateConnectInd(L2CAUTO_INSTANCE_T *instance,
                                               CsrSchedQid phandle,
                                               psm_t local_psm,
                                               BD_ADDR_T *bd_addr,
                                               l2ca_identifier_t identifier,
                                               l2ca_cid_t cid,
                                               CsrUint16 reg_ctx,
                                               l2ca_controller_t local_control)
{
    instance->phandle = phandle;
    instance->local_psm = local_psm;
    instance->cid = cid;
    instance->reg_ctx = reg_ctx;

    if (local_control != L2CA_AMP_CONTROLLER_BT)
        instance->state |= L2CAUTO_AMP;

    bd_addr_copy(&instance->bd_addr, bd_addr);

    /* Inform upper layer about connection */
    L2CAUTO_SendAutoConnectInd(instance, identifier, local_control);

}

/*! \brief Handle L2CA_CONNECT_CFM

    Handle a L2CA_CONNECT_CFM sent from L2CAP. Translate everything
    but "success" into L2CA_AUTO_CONNECT_CFM messages. A success
    starts the configuration phase.
*/
static void L2CAUTO_HandleL2caCreateConnectCfm(L2CAUTO_INSTANCE_T *instance,
                                               CsrSchedQid phandle,
                                               CsrUint16 con_ctx,
                                               CsrUint16 reg_ctx,
                                               BD_ADDR_T *bd_addr,
                                               l2ca_cid_t cid,
                                               psm_t local_psm,
                                               l2ca_conn_result_t result)
{
    /* If dying, exist immediately. Disconnect already sent. */
    if(instance->state & L2CAUTO_DYING)
    {
        return;
    }

    /* May as well always store these things. */
    instance->phandle = phandle;
    instance->cid = cid;
    instance->con_ctx = con_ctx;
    instance->reg_ctx = reg_ctx;

    /* Copy psm and bd_addr also if valid */
    instance->local_psm = local_psm;
    if (bd_addr != NULL)
        bd_addr_copy(&instance->bd_addr, bd_addr);

    /* Inform upper layer of status when not success (success means
     * move onwards) */
    if(result == L2CA_CONNECT_SUCCESS)
    {
        instance->state |= L2CAUTO_CONNECTION_OK;
        (void)L2CAUTO_KickConfig(instance);
    }
    else
    {
        L2CAUTO_SendAutoConnectCfm(instance,
                                   result,
                                   NULL);
    }
}

/* Determine whether the L2CAP reconfiguration can be accepted without
   bothering the upper layers. */
static CsrBool L2CAUTO_ReconfigIsTrivial(l2ca_cid_t cid, L2CA_CONFIG_T *config)
{
    CIDCB_T *cidcb;
    l2ca_options_t options;

    if (config == NULL || (cidcb = CIDME_GetCidcb(cid)) == NULL)
        return FALSE;

    options = config->options;

    /* Can't auto-accept MTU reductions. */
    if ((options & L2CA_SPECIFY_MTU) != 0 && config->mtu < cidcb->remote_mtu)
        return FALSE;

    /* Can't auto-accept Flush changes. */
    if ((options & L2CA_SPECIFY_FLUSH) != 0
            && config->flush_to != cidcb->local_flush_to)
        return FALSE;

    /* Application can deal with QOS and FLOWSPEC changes. */
    if ((options & (L2CA_SPECIFY_QOS | L2CA_SPECIFY_FLOWSPEC)) != 0)
        return FALSE;

    /* We cannot auto-accept illegal changes (mode, tx_window,
       maximum_pdu and max_retransmit. */
    if ((options & L2CA_SPECIFY_FLOW) != 0)
    {
        L2CA_FLOW_T *new_flow = config->flow;
        l2ca_flow_mode_t current_mode = L2CA_FLOW_MODE_BASIC;

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
        FLOWINFO_T *current_flow = cidcb->flow;

        if (current_flow != NULL)
        {
            current_mode = current_flow->config.mode;

            if (new_flow->tx_window != current_flow->config.remote_window)
                return FALSE;

            if (new_flow->maximum_pdu != current_flow->config.remote_mps)
                return FALSE;

            if (new_flow->max_retransmit != current_flow->config.max_retransmit)
                return FALSE;
        }

        /* Attempts to turn off FCS in ERTM or SM must be sent to the
           application. */

        if (current_mode != L2CA_FLOW_MODE_BASIC
                && (options & L2CA_SPECIFY_FCS) != 0
                && config->fcs == L2CA_FCS_OFF
                && current_flow->config.fcs == L2CA_FCS_ON)
            return FALSE;
#endif

        if (current_mode != new_flow->mode)
            return FALSE;
    }

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT

    if ((cidcb->flow)
     && (cidcb->flow->config.mode & (L2CA_FLOW_MODE_ENHANCED_RETRANS | L2CA_FLOW_MODE_STREAMING)) != 0
     && (options & L2CA_SPECIFY_FCS) != 0
     && config->fcs != cidcb->flow->config.fcs)
        return FALSE;
#endif

    return TRUE;
}

/*! \brief Handle L2CA_CONFIG_IND

    Handle a L2CA_CONFIG_IND sent from L2CAP.
*/
static void L2CAUTO_HandleL2caConfigInd(L2CAUTO_INSTANCE_T *instance,
                                        L2CA_CONFIG_IND_T *prim)
{
    /* If dying, exit immediately. Disconnect already sent. */
    if(instance->state & L2CAUTO_DYING)
    {
        return;
    }

    /* Is this a reconfiguration? */
    if(instance->state & L2CAUTO_ACTIVE_OK)
    {
        instance->state &= ~(L2CAUTO_CONFIG_REQUESTED |
                             L2CAUTO_CONFIG_LOCAL_OK |
                             L2CAUTO_CONFIG_REMOTE_OK |
                             L2CAUTO_ACTIVE_OK);

        if (!L2CAUTO_ReconfigIsTrivial(prim->cid, &prim->config))
        {
            /* Ward off reconfiguration attempts? */
            if(instance->disable_reconf == L2CA_RECONF_DISABLE_ALL)
            {
                /* Reject due to unacceptable parameters. Send the
                 * options we could otherwise accept */
                L2CAUTO_RejectReconfiguration(prim->cid, prim->identifier);

                /* Bail out and expect peer to send us a new and
                 * acceptable option before we continue */
                return;
            }
            else
            {
                /* Inform app about reconfig */
                L2CAUTO_SendAutoConnectInd(instance,
                                           prim->identifier,
                                           L2CA_AMP_CONTROLLER_UNKNOWN);

                /* Store peer's options in the backoff buffer for now. In
                 * the RSP handler we will detect this (as CONNECTION_OK
                 * is set for reconfigs), we can then inject a config-ind
                 * to ourselves */
                L2CA_FreeConfigPtrs(instance->backoff);
                bpfree(instance->backoff);
                instance->backoff = NULL;
                L2CAUTO_MergeBackoff(instance, &prim->config);

                /* Bail out now and expect upper layer to send down a new
                 * conftab so we can move forward */
                return;
            }
        }
    }

    /* Handle unknown options, and bail out if any are found */
    if(!L2CAUTO_ConfigIndUnknown(prim->cid, prim->identifier, &(prim->config)))
    {
        return;
    }

    /* TODO: Before we process the ConfigInd, we must make sure that
     * the extended features have been read. If this isn't the case,
     * defer the block like what is done for the reconfig stuff
     * above */

    /* Analyse indication and move forward */
    if(!L2CAUTO_ConftabInd(instance, prim))
    {
        /* Fatal error raised */
        return;
    }

    /* Make sure that our own config has been sent as well */
    if (!L2CAUTO_KickConfig(instance))
        return;

    /* We may have finished configuring */
    (void)L2CAUTO_IsComplete(instance);
}

/*! \brief Handle L2CA_CONFIG_CFM

    Peer has responded to our latest configuration request.  Analyse
    the result and possibly progress the conftab iterator in case the
    options were rejected

    Then plan for checking the confirm is dependent on the result code:
    SUCCESS, done
    PENDING, no action
    UNACCEPTABLE, backoff and retry current block
    REJECTED, try next block
    UNKNOWN, mark 'unavailable, try next block
    (other)

    In case we need to update the block counter, this function will
    update the "curr_block" itself (or delegate this task). The kick
    function runs the re-request loop if necessary.
*/
static void L2CAUTO_HandleL2caConfigCfm(L2CAUTO_INSTANCE_T *instance,
                                        L2CA_CONFIG_CFM_T *prim)
{
    /* If dying, exist immediately. Disconnect already sent. */
    if(instance->state & L2CAUTO_DYING)
    {
        return;
    }

    switch(prim->result)
    {
        case L2CA_CONFIG_SUCCESS:
            /* Local config is now OK */
            instance->state |= L2CAUTO_CONFIG_LOCAL_OK;
            break;

        case L2CA_CONFIG_PENDING:
            /* Nothing to do here */
            break;

        case L2CA_CONFIG_FLOWSPEC_REJECTED:
            /* -fallthrough */
        case L2CA_CONFIG_UNACCEPTABLE:
            /* Merge backoff data. Exchaustion checks happens in kick
             * function when we try to build the request again */
            L2CAUTO_ClearResults(instance, prim->config.options,
                                 FALSE); /* confirm direction */
            instance->state &= ~(L2CAUTO_CONFIG_REQUESTED);
            L2CAUTO_MergeBackoff(instance, &prim->config);

            /* Special treatment for a non-negotiable config value FCS */
            if(instance->results)
            {
                L2CA_CONFIG_T *res;
                res = instance->results;
                if(res->options & L2CA_SPECIFY_FCS
                    && !(prim->config.options & L2CA_SPECIFY_FCS))
                {
                    instance->backoff->options |= L2CA_SPECIFY_FCS;
                    instance->backoff->fcs = res->fcs;
                }
            }
            break;

        case L2CA_CONFIG_REJECTED:
            /* Whole block is rejected - try with next */
            L2CAUTO_ClearResults(instance, prim->config.options,
                                 FALSE); /* confirm direction */
            instance->state &= ~(L2CAUTO_CONFIG_REQUESTED);
            instance->curr_block++;
            instance->retry = L2CAP_AUTOCONNECT_RETRY;
            instance->peer_modes_left = (CsrUint16)~0;
            break;

        case L2CA_CONFIG_UNKNOWN:
            /* Unknown parameters - mark them and see if we can
             * continue on block with current set */
            instance->unknown |= prim->config.options;
            instance->state &= ~(L2CAUTO_CONFIG_REQUESTED);
            break;

        default:
            /* This shouldn't happen. Ignore. */
            break;
    }

    /* Automatically send new request if needed */
    if (!L2CAUTO_KickConfig(instance))
        return;

    /* We may have finished configuring */
    (void)L2CAUTO_IsComplete(instance);
}

/*! \brief Handle a L2CA_AUTO_CONNECT_REQ

    This function kicks off an outgoing auto-connect. The caller is
    responsible for freeing the pointers sent to this function. That
    is, we don't take ownership.

    \return TRUE if request was sent, FALSE if error in parameters
*/
static CsrBool L2CAUTO_HandleAutoConnectReq(L2CAUTO_INSTANCE_T *instance,
                                           l2ca_cid_t          cid,
                                           psm_t               local_psm,
                                           const BD_ADDR_T    *p_bd_addr,
                                           psm_t               remote_psm,
                                           CsrUint16            con_ctx,
                                           l2ca_controller_t   remote_control,
                                           l2ca_controller_t   local_control,
                                           CsrUint16            conftab_length,
                                           CsrUint16          **conftab)
{
#ifdef INSTALL_AMP_SUPPORT
    CsrBool amp;
#endif

    L2CAUTO_conftab_to_pmalloc(conftab, conftab_length);

    /* Obtain phandle from either instance or PSM */
    if(instance->phandle == CSR_SCHED_QID_INVALID)
    {
        instance->phandle = MCB_GetPhandle(local_psm);
    }

    /* For new connections, may as well set the context now. */
    if(cid == L2CA_CID_INVALID)
        instance->con_ctx = con_ctx;

    /* Validate conftab buffer */
    if(!BKV_Validate(*conftab, conftab_length))
    {
        DEBOUT(0x4200);
        L2CAUTO_FatalError(instance, L2CA_CONNECT_INVALID_CONFTAB);
        return FALSE;
    }

    /* Case of reconfiguration with an already queued request? */
    if(cid != L2CA_CID_INVALID && instance->saved_conftab)
    {
        /* We had the ownership of conftab and dont' need it */
        bpfree(*conftab);
        *conftab = NULL;

        L2CAUTO_SendAutoConnectCfm(instance,
                                   L2CA_CONNECT_CONFIG_ONGOING,
                                   NULL);
        return TRUE;
    }

    /* Claim ownership of the primtive conftab */
    bpfree(instance->conftab);
    instance->conftab = *conftab;
    instance->length  = conftab_length;
    *conftab = NULL;

    /* Scan for cached elements in the conftab */
    L2CAUTO_ConftabCache(instance);

    /* Check for obsolete bits */
    L2CAUTO_ObsoleteCheck(instance);

    /* Check the extended features as they may have been cached
     * in the conftab */
    if(!(instance->state & L2CAUTO_DYING))
    {
        if (!L2CAUTO_ExtFeatsCheck(instance))
            return FALSE;
    }
    if(instance->state & L2CAUTO_DYING)
    {
        DEBOUT(0x4300);
        L2CAUTO_FatalError(instance, L2CA_CONNECT_CONFTAB_UNSUPPORTED);
        return FALSE;
    }

    /* If 'cid' is unassigned, this is a new connection request */
    if(cid == L2CA_CID_INVALID)
    {
#ifdef INSTALL_AMP_SUPPORT
        if((remote_control != L2CA_AMP_CONTROLLER_BT) ||
           (local_control != L2CA_AMP_CONTROLLER_BT))
        {
            L2CA_CreateChannelReq(local_psm,
                                  p_bd_addr,
                                  remote_psm,
                                  con_ctx,
                                  remote_control,
                                  local_control,
                                  instance->dm_sm_service);
            amp = TRUE;
        }
        else
#endif
        {
            L2CA_ConnectReq(p_bd_addr,
                            local_psm,
                            remote_psm,
                            con_ctx,
                            instance->dm_sm_service);
#ifdef INSTALL_AMP_SUPPORT
            amp = FALSE;
#endif
        }

        /* Store bits in the instance */
#ifdef INSTALL_AMP_SUPPORT
        instance->state      |= (L2CAUTO_OUTGOING | (CsrUint16)(amp ? L2CAUTO_AMP : 0));
#else
        instance->state      |= L2CAUTO_OUTGOING;
#endif
        instance->cid        = L2CA_CID_INVALID; /* don't know til CFM arrives */
        instance->local_psm  = local_psm;
        instance->remote_psm = remote_psm;
        bd_addr_copy(&(instance->bd_addr), p_bd_addr);
    }
    /* If CID is set, this is a reconfiguration request */
    else
    {
        if(instance->state & L2CAUTO_CONFIG_REQUESTED)
        {
            instance->saved_conftab = instance->conftab;
            instance->saved_length  = instance->length;
            instance->conftab = NULL;
            instance->length = 0;
        }
        else
        {
            /* Clear state and kick reconfiguration */
            instance->state &= ~(L2CAUTO_CONFIG_LOCAL_OK |
                                 L2CAUTO_CONFIG_REMOTE_OK |
                                 L2CAUTO_ACTIVE_OK);
            return L2CAUTO_KickConfig(instance);
        }
    }

    return TRUE;
}

/*! \brief Handle a L2CA_AUTO_CONNECT_RSP

    This function responds to a L2CA_AUTO_CONNECT_RSP, driving the
    connection setup forward.

    \returns TRUE if response was sent, FALSE if error in parameters
*/
static CsrBool L2CAUTO_HandleAutoConnectRsp(L2CAUTO_INSTANCE_T *instance,
                                           l2ca_identifier_t   identifier,
                                           l2ca_cid_t          cid,
                                           l2ca_conn_result_t  response,
                                           CsrUint16            con_ctx,
                                           CsrUint16            conftab_length,
                                           CsrUint16          **conftab)
{
    L2CAUTO_conftab_to_pmalloc(conftab, conftab_length);

    instance->con_ctx = con_ctx;

    /* Check to see if we're just waiting for the RSP so that we can
       send the CFM indicating failure. */
    instance->state &= ~L2CAUTO_AWAITING_AUTO_CONNECT_RSP;
    if (instance->queued_confirm != NULL)
    {
        instance->queued_confirm->con_ctx = con_ctx;
        L2CA_PutMessage(instance->phandle,
                        L2CAP_PRIM,
                        instance->queued_confirm);
        instance->queued_confirm = NULL;

        if ((instance->state & L2CAUTO_DESTRUCTION_ATTEMPTED) != 0)
            L2CAUTO_Free(instance, L2CA_CID_INVALID);

        return FALSE;
    }

    /* Validate conftab buffer */
    if(!BKV_Validate(*conftab,conftab_length))
    {
        /* Inform upper layer by message if using the message based
         * interface */
        L2CAUTO_SendAutoConnectCfm(instance,
                                   L2CA_CONNECT_INVALID_CONFTAB,
                                   NULL);

        /* Note that the CIDCB already exists at this point and has
         * been tied to the instance in the connect-ind handler. This
         * means that we can't do any freeing here. This will instead
         * happen automatically as a part of the CIDCB cleanup */
        return FALSE;
    }

    /* Store bits in the instance in case of success error code from
     * peer */
    if(response == L2CA_CONNECT_SUCCESS)
    {
        /* Claim ownership of the primtive conftab */
        bpfree(instance->conftab);
        instance->conftab = *conftab;
        instance->length  = conftab_length;
        *conftab = NULL;

        /* Scan for cached elements in the conftab */
        L2CAUTO_ConftabCache(instance);
    }
    else
    {
        /* Error of some sort - clear the conftab and ALL results */
        L2CAUTO_ClearResults(instance, (l2ca_options_t)~0,
                             TRUE); /* response direction */
        L2CAUTO_ClearResults(instance, (l2ca_options_t)~0,
                             FALSE); /* confirm direction */
        L2CA_FreeConfigPtrs(instance->backoff);
        CsrPmemFree(instance->backoff);
        instance->backoff = NULL;
    }

    /* Check for obsolete bits */
    L2CAUTO_ObsoleteCheck(instance);

    /* Check the extended features as they may have been cached
     * in the conftab */
    if(!(instance->state & L2CAUTO_DYING))
    {
        if (!L2CAUTO_ExtFeatsCheck(instance))
            return FALSE;
    }
    if(instance->state & L2CAUTO_DYING)
    {
        DEBOUT(0x4400);
        L2CAUTO_FatalError(instance, L2CA_CONNECT_CONFTAB_UNSUPPORTED);
        return FALSE;
    }

    /* Reconfiguration in progess as the connection is up */
    if(instance->state & L2CAUTO_CONNECTION_OK)
    {
        /* Application accepts reconfiguration */
        if(response == L2CA_CONNECT_SUCCESS)
        {
            L2CA_CONFIG_IND_T *confind = pnew(L2CA_CONFIG_IND_T);

            /* Disable use of flowspecs in this reconfiguration? */
            if(!L2CAUTO_ConfHas(instance->backoff, L2CA_SPECIFY_FLOWSPEC))
            {
                instance->state |= L2CAUTO_NO_FLOWSPEC_RECONF;
            }

            /* We then need to parse the options that the peer has
             * already sent us by resurrecting the config-ind */
            confind->type = L2CA_CONFIG_IND;
            confind->phandle = instance->phandle;
            confind->cid = cid;
            confind->con_ctx = instance->con_ctx;
            confind->identifier = identifier;

            /* Transfer peer's config options */
            CsrMemCpy(&confind->config, instance->backoff, sizeof(L2CA_CONFIG_T));
            CsrPmemFree(instance->backoff);
            instance->backoff = NULL;

            /* Inject regenerated config-ind signal into own
             * handler */
            L2CAUTO_HandleL2caConfigInd(instance, confind);
            L2CA_FreePrimitive((L2CA_UPRIM_T*)confind);
        }
        /* Application rejects reconfiguration */
        else
        {
            /* Reject peer's options */
            L2CAUTO_RejectReconfiguration(cid, identifier);

            /* Send empty request */
            instance->state |= L2CAUTO_CONFIG_REQUESTED;
            L2CAUTO_EmptyConfigReq(cid);
        }
    }
    /* Connection not up - send response */
    else
    {
        L2CAUTO_SendCreateConnectRsp(instance,
                                     identifier,
                                     cid,
                                     response,
                                     con_ctx);

        if (response == L2CA_CONNECT_SUCCESS)
        {
            /* Connection setup complete, now do configuration */
            instance->state |= L2CAUTO_CONNECTION_OK;
            return L2CAUTO_KickConfig(instance);
        }
        /* Peer doesn't want to allow this connection */
        else
        {
            /* Send reject here and then enter normal failure */
            DEBOUT(0x4500);
            L2CAUTO_FatalError(instance, L2CA_CONNECT_FAILED);
        }
    }

    return TRUE;
}

/*! \brief Disconnect indication handler

    Peer issued a disconnect. Figure out whether we can handle it here
    or whether it must be passed on to the application.
*/
static L2CAUTO_RESULT_T L2CAUTO_HandleL2caDisconnectInd(L2CAUTO_INSTANCE_T *instance,
                                                        L2CA_DISCONNECT_IND_T *prim)
{
    /* Handle this if connection isn't active */
    if(!(instance->state & L2CAUTO_ACTIVE_OK))
    {
        l2ca_conn_result_t result;

        DEBOUT(0x5000);

        /* Respond */
        L2CA_DisconnectRsp(prim->identifier, prim->cid);

        /* Pull the result code if we internally initiated this
         * disconnect. The 'disable_reconf' holds the result when
         * dying */
        result = (l2ca_conn_result_t)((instance->state & L2CAUTO_DYING)
                                      ? instance->disable_reconf
                                      : L2CA_CONNECT_PEER_ABORTED);

        /* And then die. Cleanup is done by L2CAP when the CIDCB is
         * taken down, so all we need is to inform the upper layer */
        L2CAUTO_SendAutoConnectCfm(instance,
                                   result,
                                   NULL);

        return L2RS_HANDLED;
    }
    else
    {
        return L2RS_UNHANDLED;
    }
}

/*! \brief Disconnect confirm handler

    Channel has been disconnected after we locally initiated the
    disconnect procedure. Investigate whether we need to do something
    about this
*/
static L2CAUTO_RESULT_T L2CAUTO_HandleL2caDisconnectCfm(L2CAUTO_INSTANCE_T *instance,
                                                        L2CA_DISCONNECT_CFM_T *prim)
{
    PARAM_UNUSED(prim);

    /* Handle this if connection isn't active */
    if(!(instance->state & L2CAUTO_ACTIVE_OK))
    {
        l2ca_conn_result_t result;

        DEBOUT(0x5100);

        /* Pull the result code if we internally initiated this
         * disconnect. The 'disable_reconf' holds the result when
         * dying */
        result = (l2ca_conn_result_t)((instance->state & L2CAUTO_DYING)
                                      ? instance->disable_reconf
                                      : L2CA_CONNECT_PEER_ABORTED);

        L2CAUTO_SendAutoConnectCfm(instance,
                                   result,
                                   NULL);

        /* Silently consume this guy */
        return L2RS_HANDLED;
    }
    else
    {
        return L2RS_UNHANDLED;
    }
}

/*! \brief Auto-connect handler

    This function is the auto-connect main handler function. You can
    inject any L2CAP primitive into this function. If the handler was
    able to process and and consume (including freeing any members),
    it returns TRUE. Otherwise FALSE.
*/
L2CAUTO_RESULT_T L2CAUTO_HandleL2ca(l2ca_cid_t cid,
                                    const BD_ADDR_T *p_bd_addr,
                                    psm_t local_psm,
                                    L2CAUTO_INSTANCE_T *user_inst,
                                    L2CA_UPRIM_T *prim)
{
    L2CAUTO_INSTANCE_T *instance;

    /* User supplied instance */
    if(user_inst != NULL)
    {
        instance = user_inst;
    }
    /* Look up instance using CID or psm,bd_addr,ctx set */
    else
    {
        instance = L2CAUTO_FindInstance(cid, p_bd_addr, local_psm);
    }

    /* If still NULL at this point check if the PSM have auto connect
     * enabled and create the instance ourselves in that case */
    if((instance == NULL) && (local_psm != L2CA_PSM_INVALID))
    {
        PSM_T *psmcb;
        psmcb = MCB_FindProtocol(local_psm);
        if((psmcb != NULL) && (psmcb->regflags & L2CA_REGISTER_NOAUTO) == 0)
        {
            /* Create instance and add to internal L2CAP list */
            instance = L2CAUTO_Create();
        }
    }

    /* If still NULL we're not using auto connect for this channel */
    if(instance == NULL)
    {
        return L2RS_UNHANDLED;
    }

    /* Handle different primitives */
    switch(prim->type)
    {
        case L2CA_AUTO_CONNECT_REQ:
        {
            L2CA_AUTO_CONNECT_REQ_T *lcr = (L2CA_AUTO_CONNECT_REQ_T*)prim;
            (void)L2CAUTO_HandleAutoConnectReq(instance,
                                               lcr->cid,
                                               lcr->psm_local,
                                               &lcr->bd_addr,
                                               lcr->psm_remote,
                                               lcr->con_ctx,
                                               lcr->remote_control,
                                               lcr->local_control,
                                               lcr->conftab_length,
                                               &(lcr->conftab));
            break;
        }

        case L2CA_AUTO_CONNECT_RSP:
        {
            L2CA_AUTO_CONNECT_RSP_T *lcr = (L2CA_AUTO_CONNECT_RSP_T*)prim;
            (void)L2CAUTO_HandleAutoConnectRsp(instance,
                                               lcr->identifier,
                                               lcr->cid,
                                               lcr->response,
                                               lcr->con_ctx,
                                               lcr->conftab_length,
                                               &(lcr->conftab));
            break;
        }

        case L2CA_CONNECT_IND:
        {
            L2CA_CONNECT_IND_T *lci = (L2CA_CONNECT_IND_T*)prim;
            L2CAUTO_HandleL2caCreateConnectInd(instance,
                                               lci->phandle,
                                               lci->psm_local,
                                               &lci->bd_addr,
                                               lci->identifier,
                                               lci->cid,
                                               lci->reg_ctx,
                                               L2CA_AMP_CONTROLLER_BT);
            break;
        }

        case L2CA_CONNECT_CFM:
        {
            L2CA_CONNECT_CFM_T *lcc = (L2CA_CONNECT_CFM_T*)prim;
            L2CAUTO_HandleL2caCreateConnectCfm(instance,
                                               lcc->phandle,
                                               lcc->con_ctx,
                                               lcc->reg_ctx,
                                               &lcc->bd_addr,
                                               lcc->cid,
                                               lcc->psm_local,
                                               lcc->result);
            break;
        }

        case L2CA_CONFIG_IND:
            L2CAUTO_HandleL2caConfigInd(instance,
                                        (L2CA_CONFIG_IND_T*)prim);
            break;

        case L2CA_CONFIG_CFM:
            L2CAUTO_HandleL2caConfigCfm(instance,
                                        (L2CA_CONFIG_CFM_T*)prim);
            break;

        case L2CA_GETINFO_CFM:
            L2CAUTO_HandleL2caGetInfoCfm(instance,
                                         (L2CA_GETINFO_CFM_T*)prim);
            break;

        case L2CA_DISCONNECT_IND:
            /* Note: Disconnects may not be for us, so return
             * handled/unhandled result code from function  */
            return L2CAUTO_HandleL2caDisconnectInd(instance,
                                                   (L2CA_DISCONNECT_IND_T*)prim);

        case L2CA_DISCONNECT_CFM:
            /* Note: Disconnects may not be for us, so return
             * handled/unhandled result code from function  */
            return L2CAUTO_HandleL2caDisconnectCfm(instance,
                                                   (L2CA_DISCONNECT_CFM_T*)prim);


#ifdef INSTALL_AMP_SUPPORT
        case L2CA_CREATE_CHANNEL_IND:
        {
            L2CA_CREATE_CHANNEL_IND_T *lcci = (L2CA_CREATE_CHANNEL_IND_T*)prim;
            L2CAUTO_HandleL2caCreateConnectInd(instance,
                                               lcci->phandle,
                                               lcci->psm_local,
                                               &lcci->bd_addr,
                                               lcci->identifier,
                                               lcci->cid,
                                               lcci->reg_ctx,
                                               lcci->local_control);
            break;
        }

        case L2CA_CREATE_CHANNEL_CFM:
        {
            L2CA_CREATE_CHANNEL_CFM_T *lccc = (L2CA_CREATE_CHANNEL_CFM_T*)prim;
            L2CAUTO_HandleL2caCreateConnectCfm(instance,
                                               lccc->phandle,
                                               lccc->con_ctx,
                                               lccc->reg_ctx,
                                               &lccc->bd_addr,
                                               lccc->cid,
                                               lccc->psm_local,
                                               lccc->result);
            break;
        }
#endif

        default:
            /* Don't care about any other primitive */
            return L2RS_UNHANDLED;
    }

    /* As not catched by the default above we know we've handled this
     * one */
    return L2RS_HANDLED;
}

/*! \brief Auto-connect initialisation

    This function will initialise the L2CAP auto-connect instance
    data.  Ie. simply allocate, zero and return the instance.
*/
static L2CAUTO_INSTANCE_T *L2CAUTO_Create(void)
{
    L2CAUTO_INSTANCE_T *inst = zpnew(L2CAUTO_INSTANCE_T);
    inst->phandle = CSR_SCHED_QID_INVALID;
    inst->next = mcb.autoconnect;
    mcb.autoconnect = inst;

    return inst;
}

/*! \brief Auto-connect deinitialisation

    This function can be used to shutdown and cleanup the L2CAP
    auto-connnection system. This function should always be called
    when cleaning up -- even if the auto-connect instance have been
    freed... You know, just in case...
*/
void L2CAUTO_Free(L2CAUTO_INSTANCE_T *instance, l2ca_cid_t cid)
{
    L2CAUTO_INSTANCE_T **elpp, *elp;

    for (elpp = &mcb.autoconnect; (elp = *elpp) != NULL; elpp = &elp->next)
    {
        if (elp == instance || (cid != L2CA_CID_INVALID && elp->cid == cid))
        {
            elp->state |= L2CAUTO_DESTRUCTION_ATTEMPTED;

            if ((elp->state & L2CAUTO_AWAITING_AUTO_CONNECT_RSP) == 0)
            {
                *elpp = elp->next;

                L2CA_FreeConfigPtrs(elp->results);
                CsrPmemFree(elp->results);
                L2CA_FreeConfigPtrs(elp->backoff);
                CsrPmemFree(elp->backoff);
                bpfree(elp->conftab);
                CsrPmemFree(elp->dm_sm_service);
                CsrPmemFree(elp);
            }

            break;
        }
    }
}

/*! \brief Auto-connect deinitialisation

    This function is called when L2CAP itself is shutting down.
    At that point it's time to free the entire autoconnect list.
*/
#ifdef ENABLE_SHUTDOWN
void L2CAUTO_Deinit()
{
    while (mcb.autoconnect != NULL)
    {
        mcb.autoconnect->state = 0;
        L2CAUTO_Free(mcb.autoconnect, L2CA_CID_INVALID);
    }
}
#endif

#endif

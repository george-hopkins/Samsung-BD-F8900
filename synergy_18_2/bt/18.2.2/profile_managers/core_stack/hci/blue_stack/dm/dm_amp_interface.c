/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_amp_interface.c

\brief  Bluestack interface to the AMP manager.
*/

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "l2cap_prim.h"
#include "dm_prim.h"

#include "csr_util.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "dm_amp_interface.h"
#include "dm_acl_core.h"
#include "dm_mgt_interface.h"
#include "dm_hci_interface.h"
#include "dm_layer_manager.h"
#include "l2cap_con_handle.h"
#include "l2cap_cid.h"
#include "l2cap_types.h"
#include "dm_sm_types.h"
#include "dmlib.h"
#include "error.h"

#ifdef INSTALL_AMP_SUPPORT
static DM_HCI_FLOW_CONTROL_T *dm_amp_get_fc(l2ca_controller_t controller_id,
                               DM_AMP_ACL_TYPE_T type)
{
    DM_AMP_CHANNEL_T *p_amp;

    if (controller_id == L2CA_AMP_CONTROLLER_BT)
        return DM_GET_FC_TYPE(type);

    if ((p_amp = dm_amp_find(controller_id)) != NULL)
#ifdef SUPPORT_SEPARATE_LE_BUFFERS
        return ((type == DM_AMP_ACL_TYPE_BR_EDR)
                      ? &p_amp->fc : &p_amp->le_fc);
#else
        return (&p_amp->fc);
#endif

    return NULL;
}
#ifdef SUPPORT_SEPARATE_LE_BUFFERS
#define dm_amp_fragmentable(fc, type) (((type) == DM_AMP_ACL_TYPE_BR_EDR) \
                               ? (fc)->fragmentable : TRUE)
#else
#define dm_amp_fragmentable(fc, type) ((fc)->fragmentable)
#endif
#else
#define dm_amp_get_fc(controller, type) DM_GET_FC_TYPE(type)
#define dm_amp_fragmentable(fc, type) TRUE
#endif

#ifdef SUPPORT_SEPARATE_LE_BUFFERS

/*! \brief Function returns the correct flow control data structure
       from the layer manager. The FC data structure corresponds
       to the controller type BR/EDR or, LE.

    \param p_acl Pointer to ACL structure.
    \return a pointer to the corresponding flow control ds.
*/
DM_HCI_FLOW_CONTROL_T * dm_amp_get_fc_type( DM_ACL_T *p_acl)
{
    DM_AMP_ACL_TYPE_T type;

    type = CH_GET_ACL_TYPE(DM_ACL_CLIENT_GET_DATA(p_acl, dm_acl_client_l2cap));
    return DM_GET_FC_TYPE(type);
}
#endif

/*! \brief Make sure that the queue has one credit reserved just for it.
    \param queue Pointer to L2CAP queue.
    \param type for the reservation of credits in the corresponding fc data
               structures of BR/EDR or, LE.
*/
void dm_amp_reserve_credit(TXQUEUE_T *queue,
                               DM_AMP_ACL_TYPE_T type)
{
    if (queue->credits_taken == 0 && queue->reserved_credit == 0)
    {
        DM_HCI_FLOW_CONTROL_T *fc;

#ifdef INSTALL_AMP_SUPPORT
        if ((fc = dm_amp_get_fc(queue->controller_id, type)) == NULL)
            return;
#else
        /* With the type param get the corresponding BR/EDR
             or, LE flow data structure, to initialize the reserverd
             credits based on the value of initialized HCI credits
             and used HCI credits.
        */
        fc = DM_GET_FC_TYPE(type);
#endif

        if (fc->used_data_blocks < fc->total_num_data_blocks)
        {
            ++fc->used_data_blocks;
            queue->reserved_credit = 1;
        }
    }
}

/*! \brief Work out how much data we can send.

    This function is called when there is data to be sent and we need to
    know how much of it we're allowed to send.

    \param queue Pointer to L2CAP queue.
    \param sizes Pointer to L2CAP data sizes.
    \param type tells the acl link type (BR/EDR or, LE).
    \return Credits consumed.
*/
CsrUint16 dm_amp_getdatacredits(TXQUEUE_T *queue,
                               L2CAP_CH_DATA_SIZES_T *sizes,
                               DM_AMP_ACL_TYPE_T type)
{
    CsrUint16 available_data_blocks = queue->reserved_credit;
    CsrUint16 data_size = sizes->header + sizes->body + sizes->trailer;

    /* An extra argument 'type' is passed to give the flexibility of selecting the
        relevant flow control structure type pertaining to the data queue.
    */
    DM_HCI_FLOW_CONTROL_T *fc = dm_amp_get_fc(queue->controller_id, type);

    if (fc != NULL)
    {
        /* Clip data size by max HCI data packet length */
        if (data_size > fc->max_acl_data_packet_length)
        {
            if (!dm_amp_fragmentable(fc, type))
                BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);

            data_size = fc->max_acl_data_packet_length;
        }

        /* Ensure that we will consume at least one credit. */
        if (data_size != 0)
        {
            /* Limit the number of credits by available buffer space. */
            if (fc->used_data_blocks < fc->total_num_data_blocks)
                available_data_blocks +=
                    fc->total_num_data_blocks - fc->used_data_blocks;

            /* Stall data if we're not set up or there's no credits left. */
            if (available_data_blocks != 0)
            {
                CsrUint16 credits_returned, credits_consumed;
                l2ca_hci_buffer_size_t available_hci_buffer_space;

                available_hci_buffer_space = available_data_blocks
                    * (l2ca_hci_buffer_size_t)fc->data_block_length;

                /* Consume credits. Avoid doing the division if possible */
                if (data_size >= available_hci_buffer_space)
                {
                    if (data_size > available_hci_buffer_space
                            && !dm_amp_fragmentable(fc, type))
                        return 0;

                    /* Clip data size by buffer space available and
                       consume all credits */
                    data_size = (CsrUint16)(available_hci_buffer_space & 0xffff);
                    credits_returned = available_data_blocks;
                }
#ifdef BUILD_FOR_HOST
                else if (fc->max_acl_data_packet_length== fc->data_block_length)
                {
                    credits_returned = 1;
                }
#endif
                else
                {
                    /* Consuming arbitrary number of credits - do division */
                    credits_returned = 1 + (data_size-1)/fc->data_block_length;
                }

                /* One credit may be the queue's private credit. */
                credits_consumed = credits_returned - queue->reserved_credit;
                queue->reserved_credit = 0;
                fc->used_data_blocks += credits_consumed;
                queue->credits_taken += credits_returned;

                /* Adjust data size and trailer size */
                data_size -= sizes->header;
                if(data_size > sizes->body)
                {
                    sizes->trailer = data_size - sizes->body;
                }
                else
                {
                    sizes->trailer = 0;
                }

                sizes->body = data_size - sizes->trailer;

                return credits_returned;
            }
        }
    }

    return 0;
}

#ifdef INSTALL_AMP_SUPPORT

static void dm_amp_send_to_ampm(void *message, const dm_prim_t type);
static void dm_amp_read_data_block_size(l2ca_controller_t local_amp_id);

static DM_AMP_CHANNEL_T *amp_list;
static CsrSchedQid amp_phandle = CSR_SCHED_QID_INVALID;
static BD_ADDR_T *local_bd_addr;

#ifdef BUILD_FOR_HOST
void dm_amp_init(void)
{
    amp_list = NULL;
    local_bd_addr = NULL;
    amp_phandle = CSR_SCHED_QID_INVALID;
}
#endif

void dm_amp_device_ready(void)
{
    /* The device has just become ready. If amp_phandle is not CSR_SCHED_QID_INVALID
       then we have received a dm_ampm_register_req from the AMP mananger but
       have largely ignored it. So fake another one and deal with that. */
    if (amp_phandle != CSR_SCHED_QID_INVALID)
        dm_ampm_register_req(amp_phandle);
}

#ifdef ENABLE_SHUTDOWN
void dm_amp_deinit()
{
    while (amp_list != NULL)
        dm_amp_free(amp_list->amp_id);

    CsrPmemFree(local_bd_addr);

    dm_amp_init();
}

void dm_acl_client_deinit_amp(DM_AMP_T *p_amp)
{
    CsrPmemFree(p_amp->link_key_ind);
    p_amp->link_key_ind = NULL;
}
#endif

static void dm_amp_read_bd_addr(CsrUint8 sender)
{
    HCI_READ_BD_ADDR_T *p_hci_prim = pnew(HCI_READ_BD_ADDR_T);
    p_hci_prim->common.op_code = HCI_READ_BD_ADDR;
    p_hci_prim->common.length = sender;
    send_to_hci((DM_UPRIM_T*)p_hci_prim);
}

DM_AMP_CHANNEL_T *dm_amp_find(const l2ca_controller_t amp_id)
{
    DM_AMP_CHANNEL_T *p_amp;

    for (p_amp = amp_list; p_amp != NULL && p_amp->amp_id != amp_id;
            p_amp = p_amp->p_next)
        ;

    return p_amp;
}

DM_AMP_CHANNEL_T *dm_amp_create(const l2ca_controller_t amp_id,
                               const CsrBool fragmentable,
                               const CsrUint16 l2cap_max_pdu_length,
                               const CsrUint16 max_acl_data_packet_length,
                               const CsrUint16 data_block_length,
                               const CsrUint16 total_num_data_blocks)
{
    DM_AMP_CHANNEL_T *p_amp = NULL;

    /* Don't want to add BR/EDR or a duplicate. */
    if (amp_id != L2CA_AMP_CONTROLLER_BT)
    {
        if ((p_amp = dm_amp_find(amp_id)) == NULL)
        {
            if ((p_amp = xzpnew(DM_AMP_CHANNEL_T)) == NULL)
                return NULL;

            p_amp->amp_id = amp_id;

            /* Add to start of list. */
            p_amp->p_next = amp_list;
            amp_list = p_amp;
        }

        p_amp->l2cap_max_pdu_length = l2cap_max_pdu_length;
        p_amp->fc.fragmentable = fragmentable;
        p_amp->fc.max_acl_data_packet_length = max_acl_data_packet_length;
        p_amp->fc.data_block_length = data_block_length;
        p_amp->fc.total_num_data_blocks = total_num_data_blocks;
    }

    return p_amp;
}

void dm_amp_free(const l2ca_controller_t amp_id)
{
    DM_AMP_CHANNEL_T *p_amp, **pp_amp;

    for (pp_amp = &amp_list; (p_amp = *pp_amp) != NULL; pp_amp = &p_amp->p_next)
    {
        if (p_amp->amp_id == amp_id)
        {
            /* Remove from list and free. */
            *pp_amp = p_amp->p_next;
            CsrPmemFree(p_amp);
            return;
        }
    }
}

static void dm_amp_handle_ncp(DM_AMP_CHANNEL_T *p_amp, DM_AMPM_NCB_T **pp_ncb, CsrUint8 num_handles)
{
    for (;; ++pp_ncb)
    {
        DM_AMPM_NCB_T *p_ncb = *pp_ncb;
        DM_AMPM_NCB_T *p_ncb_end = p_ncb + DM_AMPM_HANDLE_COMPLETES_PER_PTR;

        /* For each pointer, read the maximum number of handle-completes. */
        for (; p_ncb != p_ncb_end; ++p_ncb, --num_handles)
        {
            if (num_handles == 0)
                return;

            /* Process NCB event */
            /* TODO Do we need the 'pending credits' stuff to deal with link loss? */
            /* TODO Debug here */
            p_amp->fc.used_data_blocks -= p_ncb->num_of_completed_blocks;

            CH_AmpCompletedPackets(p_ncb);
        }
    }
}

void dm_amp_handler(DM_UPRIM_T *const p_uprim)
{
    DM_ACL_T *p_acl;
    DM_AMP_CHANNEL_T *p_amp;
    dm_prim_t type = p_uprim->type;

    /* Deal with NCP events without delay. */
    if (type == DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ)
    {
        if ((p_amp = dm_amp_find(p_uprim->dm_ampm_number_completed_data_blocks_req.local_amp_id)) != NULL)
        {
            p_amp->fc.total_num_data_blocks = p_uprim->dm_ampm_number_completed_data_blocks_req.total_num_data_blocks;

            /* See if we need to re-read the buffer size. */
            if (p_amp->fc.total_num_data_blocks == 0)
                dm_amp_read_data_block_size(p_amp->amp_id);

            dm_amp_handle_ncp(p_amp,
                              p_uprim->dm_ampm_number_completed_data_blocks_req.num_completed_blks_ptr,
                              p_uprim->dm_ampm_number_completed_data_blocks_req.number_of_handles);
        }

        pdestroy_array((void **)p_uprim->dm_ampm_number_completed_data_blocks_req.num_completed_blks_ptr,
                DM_AMPM_NUM_COMPLETED_BLOCKS_PTRS);
        return;
    }

    /* Deal with primitives that do not contain a Bluetooth address. */
    switch (type)
    {
        case DM_AMPM_REGISTER_REQ:
            amp_phandle = p_uprim->dm_ampm_register_req.phandle;

            /* Postpone the rest until the LYM has confirmed that we're ready */
            if (LYMCB_TEST(LYM_Confirmations_Sent))
            {
                if (local_bd_addr == NULL)
                    dm_amp_read_bd_addr(DM_HCI_SENDER_DM_AMP);
                else
                    dm_amp_send_to_ampm(pnew(DM_AMPM_REGISTER_CFM_T),
                                        DM_AMPM_REGISTER_CFM);
            }
            return;

        case DM_AMPM_READ_BD_ADDR_REQ:
            dm_amp_read_bd_addr(DM_HCI_SENDER_AMPM);
            return;

        case DM_AMPM_DISCONNECT_CHANNEL_RSP:
            /* Don't care */
            return;

        case DM_AMPM_READ_DATA_BLOCK_SIZE_RSP:
            if (p_uprim->dm_ampm_read_data_block_size_rsp.status == HCI_SUCCESS)
            {
                DM_AMPM_NCB_T ncb;

                dm_amp_create(p_uprim->dm_ampm_read_data_block_size_rsp.local_amp_id,
                              p_uprim->dm_ampm_read_data_block_size_rsp.fragmentable,
                              p_uprim->dm_ampm_read_data_block_size_rsp.max_pdu_length,
                              p_uprim->dm_ampm_read_data_block_size_rsp.max_acl_data_packet_length,
                              p_uprim->dm_ampm_read_data_block_size_rsp.data_block_length,
                              p_uprim->dm_ampm_read_data_block_size_rsp.total_num_data_blocks);

                /* Force MPS recalculation */
                CH_AmpRecalculateMPS(p_uprim->dm_ampm_read_data_block_size_rsp.local_amp_id);

                /* Kick L2CAP with a dummy NCP event to start sending queued data. */
                ncb.logical_link_handle = 0;
                ncb.num_of_completed_packets = 0;
                ncb.num_of_completed_blocks = 0;
                CH_AmpCompletedPackets(&ncb);
            }
            else
            {
                DM_ACL_T *p_acl;

                /* This means that we can't use the AMP. Treat as multiple link loss. */
                for(p_acl = DM_ACL_FIRST(); p_acl != NULL; p_acl = p_acl->p_next)
                {
                    CH_AmpDisconnectInd(&p_acl->dm_acl_client_l2cap,
                                        p_uprim->dm_ampm_read_data_block_size_rsp.local_amp_id,
                                        HCI_ERROR_HARDWARE_FAIL);
                }
            }

            return;
    }

    /* All remaining primitives have a Bluetooth address and none of
       them require a response. Just pass the information on to L2CAP
       if we can. */
    if ((p_acl = dm_acl_find_by_bd_addr(&p_uprim->dm_common.bd_addr)) != NULL)
    {
        L2CAP_CHCB_T *p_chcb = &p_acl->dm_acl_client_l2cap;

        switch (type)
        {
            case DM_AMPM_CONNECT_RSP:
                /* Reply to our physical channel connect request. Relay to L2CAP. */
                CH_AmpPhysicalConnectCfm(p_chcb,
                                         p_uprim->dm_ampm_connect_rsp.status,
                                         p_uprim->dm_ampm_connect_rsp.cid,
                                         p_uprim->dm_ampm_connect_rsp.remote_amp_id);
                break;

            case DM_AMPM_CONNECT_CHANNEL_RSP:
                /* Relay to L2CAP reply to our logical channel connect req. */
                CH_AmpLogicalConnectCfm(p_chcb,
                                        &p_uprim->dm_ampm_connect_channel_rsp);

                break;

            case DM_AMPM_DISCONNECT_REQ:
                /* Notice of AMP link loss. Relay to L2CAP. */
                CH_AmpDisconnectInd(p_chcb,
                                    p_uprim->dm_ampm_disconnect_req.local_amp_id,
                                    p_uprim->dm_ampm_disconnect_req.reason);

                /* If this was the last physical link used by this AMP
                   then it's time to discard our buffer size information */
                if (p_uprim->dm_ampm_disconnect_req.active_links == 0)
                    dm_amp_free(p_uprim->dm_ampm_disconnect_req.local_amp_id);

                break;

            case DM_AMPM_VERIFY_PHYSICAL_LINK_RSP:
                /* Notice of whether a local controller ID exists. Relay to L2CAP. */
                CH_AmpVerifyPhysicalLinkCfm(p_chcb,
                                            p_uprim->dm_ampm_verify_physical_link_rsp.identifier,
                                            p_uprim->dm_ampm_verify_physical_link_rsp.exists,
                                            p_uprim->dm_ampm_verify_physical_link_rsp.link_supervision_timeout,
                                            p_uprim->dm_ampm_verify_physical_link_rsp.best_effort_flush_timeout);
                break;
        }
    }
}

void dm_amp_physical_connect_req(const DM_ACL_T *const p_acl,
                                 const l2ca_cid_t cid,
                                 const l2ca_controller_t local_amp_id,
                                 const l2ca_controller_t remote_amp_id)
{
    DM_AMPM_CONNECT_IND_T *p_prim = pnew(DM_AMPM_CONNECT_IND_T);

    p_prim->cid = cid;
    p_prim->local_amp_id = local_amp_id;
    p_prim->remote_amp_id = remote_amp_id;

    tbdaddr_copy_to_bd_addr(&p_prim->bd_addr, &p_acl->addrt);

    /* If request refers to BR/EDR then just fake a response from AMPM. */
    if (local_amp_id == L2CA_AMP_CONTROLLER_BT || remote_amp_id == L2CA_AMP_CONTROLLER_BT)
    {
        CsrUint8 status = HCI_SUCCESS;

        /* Check for naughtiness */
        if (local_amp_id != remote_amp_id)
            status = HCI_ERROR_ILLEGAL_FORMAT;

        dm_ampm_connect_rsp(p_prim, status);
        CsrPmemFree(p_prim);
    }
    else
        dm_amp_send_to_ampm(p_prim, DM_AMPM_CONNECT_IND);
}

void dm_amp_logical_connect_req(const DM_ACL_T *const p_acl,
                                const l2ca_cid_t cid,
                                const l2ca_controller_t local_amp_id,
                                const L2CA_FLOWSPEC_T *flow_spec_tx,
                                const L2CA_FLOWSPEC_T *flow_spec_rx)
{
    DM_AMPM_CONNECT_CHANNEL_IND_T *p_prim = pnew(DM_AMPM_CONNECT_CHANNEL_IND_T);

    p_prim->cid = cid;
    p_prim->local_amp_id = local_amp_id;

    p_prim->handle = p_acl->handle;
    tbdaddr_copy_to_bd_addr(&p_prim->bd_addr, &p_acl->addrt);
    (void)CsrMemCpy(&p_prim->tx_flowspec, flow_spec_tx, sizeof(L2CA_FLOWSPEC_T));
    (void)CsrMemCpy(&p_prim->rx_flowspec, flow_spec_rx, sizeof(L2CA_FLOWSPEC_T));

    /* If request refers to BR/EDR then just fake a response from AMPM. */
    if (local_amp_id == L2CA_AMP_CONTROLLER_BT)
    {
        /* TODO try to use invalid logical link ID, rather than 0 */
        dm_ampm_connect_channel_rsp(p_prim, 0, 0, HCI_BR_EDR_PHANDLE, HCI_SUCCESS);
        CsrPmemFree(p_prim);
    }
    else
    {
        /* Read buffer size if AMP is new to us. */
        if (dm_amp_find(local_amp_id) == NULL)
            dm_amp_read_data_block_size(local_amp_id);

        dm_amp_send_to_ampm(p_prim, DM_AMPM_CONNECT_CHANNEL_IND);
    }
}

void dm_amp_disconnect_req(const DM_ACL_T *const p_acl,
                           const l2ca_cid_t cid,
                           const l2ca_controller_t local_amp_id)
{
    DM_AMPM_DISCONNECT_CHANNEL_IND_T *p_prim = pnew(DM_AMPM_DISCONNECT_CHANNEL_IND_T);

    p_prim->cid = cid;
    p_prim->local_amp_id = local_amp_id;

    /* If request refers to BR/EDR then just fake a response from AMPM. */
    if (local_amp_id == L2CA_AMP_CONTROLLER_BT)
    {
        dm_ampm_disconnect_channel_rsp(p_prim, HCI_SUCCESS);
        CsrPmemFree(p_prim);
    }
    else
        dm_amp_send_to_ampm(p_prim, DM_AMPM_DISCONNECT_CHANNEL_IND);
}

void dm_amp_verify_physical_link_req(const DM_ACL_T *const p_acl,
                                     const CsrUint16 identifier,
                                     const l2ca_controller_t local_amp_id)
{
    /* If request refers to BR/EDR then just fake a response from AMPM. */
    if (local_amp_id == L2CA_AMP_CONTROLLER_BT)
    {
        dm_ampm_verify_physical_link_rsp(&p_acl->addrt.addr,
                                         identifier,
                                         TRUE,
                                         0,
                                         0);
    }
    else
    {
        DM_AMPM_VERIFY_PHYSICAL_LINK_IND_T *p_prim =
                pnew(DM_AMPM_VERIFY_PHYSICAL_LINK_IND_T);

        tbdaddr_copy_to_bd_addr(&p_prim->bd_addr, &p_acl->addrt);
        p_prim->identifier = identifier;
        p_prim->local_amp_id = local_amp_id;

        dm_amp_send_to_ampm(p_prim, DM_AMPM_VERIFY_PHYSICAL_LINK_IND);
    }
}

void dm_amp_link_key_notification_ind(DM_ACL_T *p_acl,
                                      const CsrUint8 key_type,
                                      const CsrUint8 *const link_key,
                                      const DM_AMP_LINK_KEY_STATE_T state)
{
    DM_AMPM_LINK_KEY_NOTIFICATION_IND_T *p_prim =
            pnew(DM_AMPM_LINK_KEY_NOTIFICATION_IND_T);

    p_prim->key_type = key_type;

    tbdaddr_copy_to_bd_addr(&p_prim->bd_addr, &p_acl->addrt);

    CsrMemCpy(p_prim->key, link_key, sizeof(p_prim->key));

    /* Store primitive in client space. */
    CsrPmemFree(p_acl->dm_acl_client_amp.link_key_ind);
    p_acl->dm_acl_client_amp.link_key_ind = p_prim;

    /* Send now if link key is already active. */
    if (state == DM_AMP_LINK_KEY_ACTIVE)
        dm_amp_link_key_active_ind(p_acl, HCI_SUCCESS);
}

void dm_amp_link_key_active_ind(DM_ACL_T *p_acl, CsrUint8 status)
{
    if (status == HCI_SUCCESS &&
        p_acl->dm_acl_client_amp.link_key_ind  != NULL)
    {
        dm_amp_send_to_ampm(p_acl->dm_acl_client_amp.link_key_ind,
                DM_AMPM_LINK_KEY_NOTIFICATION_IND);
    }
    else
        CsrPmemFree(p_acl->dm_acl_client_amp.link_key_ind);

    p_acl->dm_acl_client_amp.link_key_ind = NULL;
}

void dm_amp_read_bd_addr_cc(const CsrUint8 sender,
                            const hci_return_t status,
                            const BD_ADDR_T *const p_bd_addr)
{
    if ((sender & DM_HCI_SENDER_DM_AMP) != 0)
    {
        if (status == HCI_SUCCESS)
        {
            CsrPmemFree(local_bd_addr);
            local_bd_addr = pcopy(p_bd_addr, sizeof(BD_ADDR_T));

            dm_amp_send_to_ampm(pnew(DM_AMPM_REGISTER_CFM_T),
                                DM_AMPM_REGISTER_CFM);
        }
        else
            dm_amp_read_bd_addr(DM_HCI_SENDER_DM_AMP);
    }

    if ((sender & DM_HCI_SENDER_AMPM) != 0)
    {
        DM_AMPM_READ_BD_ADDR_CFM_T *p_prim = zpnew(DM_AMPM_READ_BD_ADDR_CFM_T);

        p_prim->status = status;
        if (status == HCI_SUCCESS)
            bd_addr_copy(&p_prim->bd_addr, p_bd_addr);

        dm_amp_send_to_ampm(p_prim, DM_AMPM_READ_BD_ADDR_CFM);
    }
}

/* ACL Manager callbacks */
void dm_amp_acl_opened(DM_ACL_T *p_acl, CsrUint8 status)
{
#if !(defined(BUILD_FOR_HOST) && defined(INSTALL_BUILD_FOR_HOST_DM_ACL_CON_HANDLE_IND))
    DM_ACL_OPENED_IND_T *p_prim = zpnew(DM_ACL_OPENED_IND_T);
#else
    DM_ACL_CONN_HANDLE_IND_T *p_prim = zpnew(DM_ACL_CONN_HANDLE_IND_T);
    p_prim->acl_conn_handle = p_acl->handle;
#endif

    tbdaddr_copy(&p_prim->addrt, &p_acl->addrt);
    p_prim->dev_class = p_acl->dev_class;
    p_prim->status = status;

    if ((p_acl->flags & DM_ACL_INITIATOR) != DM_ACL_INIT_LOCAL)
        p_prim->flags |= DM_ACL_FLAG_INCOMING;

#if !(defined(BUILD_FOR_HOST) && defined(INSTALL_BUILD_FOR_HOST_DM_ACL_CON_HANDLE_IND))
    dm_amp_send_to_ampm(p_prim, DM_ACL_OPENED_IND);
#else
    dm_amp_send_to_ampm(p_prim, DM_ACL_CONN_HANDLE_IND);
#endif
}

CsrUint16 dm_amp_acl_closed(DM_ACL_T *p_acl, CsrUint16 reason)
{
    DM_ACL_CLOSED_IND_T *p_prim = zpnew(DM_ACL_CLOSED_IND_T);

    p_prim->reason = (CsrUint8)(reason & 0xFF);
    tbdaddr_copy(&p_prim->addrt, &p_acl->addrt);

    if ((p_acl->flags & DM_ACL_INITIATOR) != DM_ACL_INIT_LOCAL)
        p_prim->flags |= DM_ACL_FLAG_INCOMING;

    dm_amp_send_to_ampm(p_prim, DM_ACL_CLOSED_IND);

    CsrPmemFree(p_acl->dm_acl_client_amp.link_key_ind);
    p_acl->dm_acl_client_amp.link_key_ind = NULL;

    return 0;
}

void dm_amp_acl_mode_changed(DM_ACL_T *p_acl, CsrUint8 status, CsrUint16 length)
{
    DM_HCI_MODE_CHANGE_EVENT_IND_T *p_prim;

    p_prim = pnew(DM_HCI_MODE_CHANGE_EVENT_IND_T);
    p_prim->status = status;
    tbdaddr_copy_to_bd_addr(&p_prim->bd_addr, &p_acl->addrt);
    p_prim->mode = p_acl->current_mode;
    p_prim->length = length;

    dm_amp_send_to_ampm(p_prim, DM_HCI_MODE_CHANGE_EVENT_IND);
}

static void dm_amp_read_data_block_size(l2ca_controller_t local_amp_id)
{
    DM_AMPM_READ_DATA_BLOCK_SIZE_IND_T *p_prim = pnew(DM_AMPM_READ_DATA_BLOCK_SIZE_IND_T);

    p_prim->local_amp_id = local_amp_id;

    dm_amp_send_to_ampm(p_prim, DM_AMPM_READ_DATA_BLOCK_SIZE_IND);
}

static void dm_amp_send_to_ampm(void *message, const dm_prim_t type)
{
    if (amp_phandle == CSR_SCHED_QID_INVALID)
    {
        CsrPmemFree(message);
    }
    else
    {
        ((DM_UPSTREAM_COMMON_T*)message)->type = type;
        ((DM_UPSTREAM_COMMON_T*)message)->phandle = amp_phandle;
        DM_SendMessage(message);
    }
}

static void dm_amp_serialise_bd_addr(CsrUint8 *buf, const BD_ADDR_T *bd_addr)
{
    write_uints(&buf,
                URW_FORMAT(CsrUint16, 1, CsrUint8, 2, CsrUint16, 1),
                (CsrUint16)(bd_addr->lap & 0xFFFF),
                (CsrUint8)(bd_addr->lap >> 16),
                bd_addr->uap,
                bd_addr->nap);
}

#define DM_AMP_BD_ADDR_BYTES 6

CsrBool dm_amp_reject_remote_request(const DM_ACL_T *const p_acl)
{
    CsrUint8 buf_loc[DM_AMP_BD_ADDR_BYTES], buf_rem[DM_AMP_BD_ADDR_BYTES];
    CsrUint8 *locp, *remp;

    if (p_acl != NULL && local_bd_addr != NULL)
    {
        /* Splat local and remote BD_ADDRs into CsrUint8 arrays. */
        dm_amp_serialise_bd_addr(buf_loc, local_bd_addr);
        dm_amp_serialise_bd_addr(buf_rem, &p_acl->addrt.addr);

        /* Follow conflict resolution algorithm given in
           BT3.0 Vol3 Part A Section 7.7 */
        for (locp = buf_loc, remp = buf_rem;
                locp != buf_loc + DM_AMP_BD_ADDR_BYTES;
                ++locp, ++remp)
        {
            if (*locp > *remp)
                return TRUE;

            if (*locp < *remp)
                return FALSE;
        }
    }

    /* Getting here means that either we're not set up correctly, or the
       two addresses are equal. This should never happen. May as well
       reject the request because nothing is going to work anyway. */
    return TRUE;
}

void dm_amp_simple_pairing_enabled(void)
{
    dm_amp_send_to_ampm(pnew(DM_AMPM_SIMPLE_PAIRING_ENABLED_IND_T),
                        DM_AMPM_SIMPLE_PAIRING_ENABLED_IND);
}

void dm_amp_remote_features(const TYPED_BD_ADDR_T *addrt, CsrUint16 acl_flags)
{
    if ((acl_flags & DM_SM_FLAG_RSF_RHSF_KNOWN) == DM_SM_FLAG_RSF_RHSF_KNOWN)
    {
        DM_AMPM_REMOTE_FEATURES_IND_T *prim =
            zpnew(DM_AMPM_REMOTE_FEATURES_IND_T);

        (void)tbdaddr_copy_to_bd_addr(&prim->bd_addr, addrt);

        if ((acl_flags & DM_SM_FLAG_RSF_RHSF_SSP) == DM_SM_FLAG_RSF_RHSF_SSP)
            prim->flags |= DM_AMPM_REMOTE_FEATURES_SSP;

        dm_amp_send_to_ampm(prim, DM_AMPM_REMOTE_FEATURES_IND);
    }
}

#endif /* INSTALL_AMP_SUPPORT */

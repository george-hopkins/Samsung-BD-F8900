/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_acl_reassemble.c

\brief  Low-level L2CAP ACL packet reassembly. This function is invoked
        directly from hci-top. Once an ACL packet has been fully
        assembled, it's injected onto the DM HCI interface queue.

        Header file is l2cap_con_handle.h
*/

#include "csr_synergy.h"

#include "l2cap_con_handle.h"

#ifdef BUILD_FOR_HOST

#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "csr_bt_common.h"
#include "csr_mblk.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_sched.h"
#include "csr_bt_tasks.h"
#include "error.h"

#include "hci_prim.h"
#include "dm_prim.h"
#include "l2cap_prim.h"

#include "l2cap_cid.h"
#include "l2cap_cidme.h"
#include "l2cap_chme.h"
#include "dm_acl_manager.h"
#include "dm_acl_policy_manager.h"

/*! \brief ACL reassembly

    For host builds we assemble ACL packets in L2CAP itself. This lets
    us do early sanity checks against the configured MTU. The function
    is called by the DM HCI handler. Once a packet is complete, we
    simply return it. If non-complete, return NULL. The DM is
    responsible for passing the assembled packet into L2CAP.
*/
CsrMblk *L2CA_AclReassemble(hci_connection_handle_t hci_flags,
                           CsrMblk *mblk)
{
    hci_connection_handle_t handle;
    CsrMblkSize size;
    L2CAP_CHCB_T *chcb;

    /* Extract handle and size */
    size = CsrMblkGetLength(mblk);
    handle = hci_flags & HCI_CONNECTION_HANDLE_MASK;

    /* Obtain CHCB handle */
    chcb = CHME_GetConHandle(handle);
    if(chcb == NULL)
    {
        /* Bogus handle */
        BLUESTACK_WARNING(CON_HANDLE_ERR);
        CsrMblkDestroy(mblk);
        return NULL;
    }

    /* If not a continuation, it's a start/start-flushable */
    if((hci_flags & HCI_PKT_BOUNDARY_MASK) != HCI_PKT_BOUNDARY_FLAG_CONT)
    {
        CIDCB_T *cidcb;
        CsrUint16 cid;
        CsrUint16 len;
        CsrUint16 mtu;
        CsrUint8 *map;

        /* Free any existing reassembly buffer */
        if(chcb->reassem.mblk != NULL)
        {
            /* Last packet was not reasembled correctly. Discard it. */
            BLUESTACK_WARNING(CON_HANDLE_ERR);
            CsrMblkDestroy(chcb->reassem.mblk);
            chcb->reassem.mblk = NULL;
        }

        /* L2CAP basic header must be in packet */
        if(size < L2CAP_SIZEOF_CID_HEADER)
        {
            /* Bogus - throw away */
            BLUESTACK_WARNING(CON_HANDLE_ERR);
            CsrMblkDestroy(mblk);
            return NULL;
        }

        /* Extract CID and length from packet - we need to map it in
         * first */
        map = CsrMblkMap(mblk, 0, L2CAP_SIZEOF_CID_HEADER);
        if(map == NULL)
        {
            /* Error in mapping */
            BLUESTACK_WARNING(CON_HANDLE_ERR);
            CsrMblkDestroy(mblk);
            return NULL;
        }
        len = UINT16_R(map, L2CAP_PKT_POS_LENGTH);
        cid = UINT16_R(map, L2CAP_PKT_POS_CID);
        CsrMblkUnmap(mblk, map);

        /* If we have a CIDCB, get the MTU for this channel */
        cidcb = CIDME_GetCidcbWithHandle(chcb, cid);
        mtu = (CsrUint16)((cidcb != NULL) ? cidcb->local_mtu : L2CA_MTU_DEFAULT);

        /* Make sure MTU isn't exceeded. Note that more fine grained
         * checks will be performed at a later stage, i.e. once we
         * know how to decode the header(s) based on the channel
         * configuration */
        if((len > (mtu + L2CAP_ACL_MTU_MAX)) ||       /* checks header 'length' field */
           (size > (mtu + L2CAP_ACL_RAW_LENGTH_MAX))) /* checks actual data size */
        {
            BLUESTACK_WARNING(CON_HANDLE_ERR);
            CsrMblkDestroy(mblk);
            return NULL;
        }

        /* Packet OK. Append it to the reassembly */
        chcb->reassem.length = len;
        chcb->reassem.offset = size;
        chcb->reassem.mblk = mblk;
    }
    /* Continuation - make sure we already have a reassembly buffer */
    else if(chcb->reassem.mblk != NULL)
    {
        /* Drop everything if this ACL packet will make us exceed the
         * actual L2CAP packet size */
        if((size + chcb->reassem.offset) > (chcb->reassem.length + L2CAP_SIZEOF_CID_HEADER))
        {
            BLUESTACK_WARNING(CON_HANDLE_ERR);
            CsrMblkDestroy(mblk);
            CsrMblkDestroy(chcb->reassem.mblk);
            chcb->reassem.mblk = NULL;
            return NULL;
        }

        /* Continuation seems to be OK. Append it */
        chcb->reassem.mblk = CsrMblkAddTail(mblk, chcb->reassem.mblk);
        chcb->reassem.offset += size;
    }
    else
    {
        /* Continuation packets received out of order */
        BLUESTACK_WARNING(CON_HANDLE_ERR);
        CsrMblkDestroy(mblk);
        return NULL;
    }

    /* Check for completion */
    if((chcb->reassem.mblk != NULL) &&
       (chcb->reassem.offset == (chcb->reassem.length + L2CAP_SIZEOF_CID_HEADER)))
    {
        /* Return the completed MBLK and reset buffer */
        mblk = chcb->reassem.mblk;
        chcb->reassem.mblk = NULL;
        return mblk;
    }
    else
    {
        /* Nothing ready this time */
        return NULL;
    }
}

#endif /* BUILD_FOR_HOST */

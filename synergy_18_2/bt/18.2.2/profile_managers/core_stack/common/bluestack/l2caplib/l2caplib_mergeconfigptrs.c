/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_MergeConfigPtrs.c

\brief  Merge config pointer block.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
/*! \brief Merge config pointer block

    Copy/merge configuration pointer blocks, possibly allocating them
    on the way. Note that we *merge* bits. We don't remove bits.
*/
void L2CA_MergeConfigPtrs(L2CA_CONFIG_T *target,
                          L2CA_CONFIG_T *source)
{
    target->options |= source->options;

    /* MTU */
    if(source->options & L2CA_SPECIFY_MTU)
    {
        target->mtu = source->mtu;
    }

    /* Flush timeout */
    if(source->options & L2CA_SPECIFY_FLUSH)
    {
        target->flush_to = source->flush_to;
    }

    /* QoS - allocates structure if needed */
    if(source->options & L2CA_SPECIFY_QOS)
    {
        if(target->qos == NULL)
        {
            target->qos = pnew(L2CA_QOS_T);
        }
        CsrMemCpy(target->qos, source->qos, sizeof(L2CA_QOS_T));
    }

    /* Flow control - allocates structure if needed */
    if(source->options & L2CA_SPECIFY_FLOW)
    {
        if(target->flow == NULL)
        {
            target->flow = pnew(L2CA_FLOW_T);
        }
        CsrMemCpy(target->flow, source->flow, sizeof(L2CA_FLOW_T));
    }

    /* Frame check sequence */
    if(source->options & L2CA_SPECIFY_FCS)
    {
        target->fcs = source->fcs;
    }

    /* FlowSpec - allocates structure if needed */
    if(source->options & L2CA_SPECIFY_FLOWSPEC)
    {
        if(target->flowspec == NULL)
        {
            target->flowspec = pnew(L2CA_FLOWSPEC_T);
        }
        CsrMemCpy(target->flowspec, source->flowspec, sizeof(L2CA_FLOWSPEC_T));
    }

    /* Extended windows */
    if(source->options & L2CA_SPECIFY_EXT_WINDOW)
    {
        target->ext_window = source->ext_window;
    }
}
#endif

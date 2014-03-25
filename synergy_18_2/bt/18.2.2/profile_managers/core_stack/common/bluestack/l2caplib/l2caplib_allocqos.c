/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_AllocQoS.c

\brief  Allocate and return a best effort QoS structure.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Allocate and return a best effort QoS structure
*/
#ifdef INSTALL_L2CAP_QOS_SUPPORT
L2CA_QOS_T *L2CA_AllocQoS(void)
{
    L2CA_QOS_T *qos = pnew(L2CA_QOS_T);
    qos->flags        = 0;
    qos->service_type = L2CA_QOS_DEFAULT_SERVICE_TYPE;
    qos->token_rate   = L2CA_QOS_DEFAULT_TOKEN_RATE;
    qos->token_bucket = L2CA_QOS_DEFAULT_TOKEN_BUCKET;
    qos->peak_bw      = L2CA_QOS_DEFAULT_PEAK_BW;
    qos->latency      = L2CA_QOS_DEFAULT_LATENCY;
    qos->delay_var    = L2CA_QOS_DEFAULT_DELAY_VAR;
    return qos;
}
#endif

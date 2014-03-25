/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_AllocFlowspec.c

\brief  L2CAP access library - provides functions for building and
        sending downstream L2CAP primitives.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Allocate and return a best effort flowspec

    We need flowspecs before we can create or move channels. It may be
    necessary to supply best-effort flowspecs in case the application
    doesn't care.
*/
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
L2CA_FLOWSPEC_T *L2CA_AllocFlowspec(void)
{
    L2CA_FLOWSPEC_T *fs = pnew(L2CA_FLOWSPEC_T);
    fs->fs_identifier   = 1; /* Best effort always uses id 1 */
    fs->fs_service_type = L2CA_QOS_TYPE_BEST_EFFORT;
    fs->fs_max_sdu      = L2CA_FLOWSPEC_MAX_SDU;
    fs->fs_interarrival = L2CA_FLOWSPEC_INTERAR_DEFAULT;
    fs->fs_latency      = L2CA_FLOWSPEC_ACCESS_DEFAULT;
    fs->fs_flush_to     = L2CA_FLOWSPEC_FLUSH_TO_INFINITE;
    return fs;
}
#endif /* FLOWSPEC_SUPPORT */

/****************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:           csr_bt_bnep_lib.c

DESCRIPTION:    BNEP Protocol (BNEP) library definitions

REVISION:       $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE

#include "csr_types.h"
#include "csr_msg_transport.h"
#include "csr_bt_bnep_lib.h"

void CsrBtBnepMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_BNEP_IFACEQUEUE, CSR_BT_BNEP_PRIM, msg);
}

#endif

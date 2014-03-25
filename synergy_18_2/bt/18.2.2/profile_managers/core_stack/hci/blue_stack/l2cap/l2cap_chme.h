/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_chme.h

\brief  This file defines L2CAP connection managment.
*/

#ifndef _L2CAP_CHME_H_
#define _L2CAP_CHME_H_

#include "csr_synergy.h"

#include "l2cap_con_handle.h"

#ifdef __cplusplus
extern "C" {
#endif

extern struct chtag *CHME_GetConHandleBdAddr(const TYPED_BD_ADDR_T *addrt);

#ifdef BUILD_FOR_HOST
extern struct chtag *CHME_GetConHandle(hci_connection_handle_t handle);
#endif

#ifdef __cplusplus
}
#endif
#endif

/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_acl_amp_types.h

\brief  AMP Manager definitions required for the ACL record structure.
*/

#ifndef _DM_ACL_AMP_TYPES_H_
#define _DM_ACL_AMP_TYPES_H_

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    DM_AMPM_LINK_KEY_NOTIFICATION_IND_T *link_key_ind;
} DM_AMP_T;

#ifdef __cplusplus
}
#endif

#endif

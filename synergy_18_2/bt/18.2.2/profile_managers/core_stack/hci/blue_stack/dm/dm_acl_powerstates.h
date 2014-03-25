#ifndef _DM_ACL_POWERSTATES_H_
#define _DM_ACL_POWERSTATES_H_

#include "csr_synergy.h"
/****************************************************************************

                   (c) CSR plc 2010

                   All rights reserved

FILE:              dm_acl_powerstates.h

DESCRIPTION:       The ACL powerstates management entity of the
                   BlueStack Device Manager.



****************************************************************************/

#include "csr_bt_bluestack_types.h" /* path modified */
#include "bluetooth.h" /* path modified */
#include "hci_prim.h" /* path modified */
#include "l2cap_prim.h"
#include "dm_prim.h" /* path modified */
#include "dm_acl_core.h"

#ifdef __cplusplus
extern "C" {
#endif

void dm_acl_lp_write_powerstates_req(const DM_UPRIM_T *const p_uprim);

#ifdef INSTALL_DM_POWERSTATES_MODULE

#define DM_ACL_IS_USING_POWERSTATES(p_acl) \
        (((p_acl)->dm_acl_client_policy.powerstates) != NULL)

CsrBool dm_acl_powerstates_activity(DM_ACL_T *p_acl);
void dm_acl_policy_powerstates(DM_ACL_T *const p_acl);
void dm_acl_client_deinit_policy_powerstates(DM_POLICY_T *p_policy);

#else

#define DM_ACL_IS_USING_POWERSTATES(p_acl) (FALSE)
#define dm_acl_policy_powerstates(a)
#define dm_acl_client_deinit_policy_powerstates(a)

#endif /* INSTALL_DM_POWERSTATES_MODULE */

#ifdef __cplusplus
}
#endif

#endif /* _DM_ACL_POWERSTATES_H_ */

/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_chme.c

\brief  This file handles L2CAP connection managment.
*/

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "l2cap_types.h"
#include "l2cap_chme.h"
#include "l2cap_con_handle.h"

/*! \brief Retrieve connection handle instance for Bluetooth Address.

    This function returns the connection handle instance for the
    specified _fully_ _connected_ remote device address.

    \return Pointer to CHCB instance, or NULL if no matching instance exists.
*/
L2CAP_CHCB_T *CHME_GetConHandleBdAddr(const TYPED_BD_ADDR_T *addrt)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_tbdaddr(addrt)) != NULL
            && dm_acl_is_connected(p_acl))
        return CH_GET_CHCB(p_acl);

    return NULL;
}


/*! \brief Retrieve connection handle instance for HCI connection handle.

    This function returns the connection handle instance for the specified
    HCI connection handle. It walks down the linked list of CHCB instances
    looking for the first instance with matching HCI handle.

    \return Pointer to CHCB instance, or NULL if no matching instance exists.
*/
#ifdef BUILD_FOR_HOST
L2CAP_CHCB_T *CHME_GetConHandle(hci_connection_handle_t handle)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_handle(handle)) != NULL)
        return CH_GET_CHCB(p_acl);

    return NULL;
}
#endif

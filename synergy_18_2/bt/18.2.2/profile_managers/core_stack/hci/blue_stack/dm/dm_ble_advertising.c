/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_ble_advertising.c

\brief  Look after BLE advertising

\details
This is a BLE advertising helper module. Currently I see it performing two
functions:

1) Allow the application to change advertisement parameters without disabling
   and re-enabling advertising.
2) Remember old advertising mode when Bluestack makes a switch to directed
   connectible advertising. When that has finished, we switch back to the
   old advertising mode if it is appropriate.

At some future point we might decide to provide the option of an automatic
fallback from undirected connectible advertising to discoverable advertising
when a connection is established, and the restoration of undirected
connectible advertising when we no-longer have any connections.

*/

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"
#include "csr_bt_common.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_panic.h"

#include "dm_ble_advertising.h"
#include "dm_mgt_interface.h"
#include "dm_hci_interface.h"
#include "dm_acl_manager.h"
#include "tbdaddr.h"

#ifdef INSTALL_ULP

/* Stored advertising state to be restored after directed advertising ends */
typedef struct
{
    DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T *prim;
    BITFIELD(CsrBool, connect_undirected, 1);
    BITFIELD(CsrBool, connect_directed, 1);
    BITFIELD(CsrUint8, connect_filter_policy, 7);
    BITFIELD(CsrUint8, app_enabled, 8);
} DM_BLE_ADVERTISING_T;

DM_BLE_ADVERTISING_T dm_ble_advertising;

/*! \brief Enable/Disable advertising and make a note of what we've done.

    The request will have come from Bluestack, so don't update 'app_enabled'.

    \param advertising_enable HCI_ULP_ADVERTISING_DISABLED or
                              HCI_ULP_ADVERTISING_ENABLED.
    \param sender DM_HCI_SENDER_APPLICATION or DM_HCI_SENDER_ACL_MANAGER.
*/
static void dm_ble_advertising_set_enable(CsrUint8 advertising_enable,
                                          CsrUint8 sender)
{
    HCI_ULP_SET_ADVERTISE_ENABLE_T *prim = pnew(HCI_ULP_SET_ADVERTISE_ENABLE_T);

    prim->common.op_code = HCI_ULP_SET_ADVERTISE_ENABLE;
    prim->common.length = sender;
    prim->advertising_enable = advertising_enable;

    send_to_hci((DM_UPRIM_T *) prim);
}

/*! \brief Enable/Disable advertising and make a note of what we've done.

    The request will have come from the application, so update 'app_enabled'.

    \param uprim Pointer to primitive cast to (DM_UPRIM_T*)
*/
void dm_ble_advertising_set_enable_req(const DM_UPRIM_T * const uprim)
{
    dm_ble_advertising.app_enabled =
            uprim->dm_ulp_set_advertise_enable_req.advertising_enable;

    dm_ble_advertising_set_enable(dm_ble_advertising.app_enabled,
                                  DM_HCI_SENDER_APPLICATION);
}

/*! \brief Pause advertising for new parameters.

    \param prim Pointer to DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ primitive.
    \param sender Entity sending the primitive. Can be one of:
        DM_HCI_SENDER_NONE          No notification to application but final
                                    advertising state given by
                                    dm_ble_advertising.app_enabled
        DM_HCI_SENDER_APPLICATION   Notification to application and final
                                    advertising state given by
                                    dm_ble_advertising.app_enabled
        DM_HCI_SENDER_ACL_MANAGER   No notification to application and final
                                    advertising state is
                                    HCI_ULP_ADVERTISING_ENABLED
*/
static void dm_ble_advertising_new_parameters(
        DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T *prim,
        CsrUint8 sender)
{
    if (prim != NULL)
    {
        /* Set op_code. This gets done here because both Bluestack and
           Application will require it. Bluestack requires it because
           it is building the primitive from scrath. Application requires
           it because DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ !=
           HCI_ULP_SET_ADVERTISING_PARAMETERS. So may as well delay it
           until here and deal with both cases together. */
        prim->common.op_code = HCI_ULP_SET_ADVERTISING_PARAMETERS;

        /* Record sender so DM_HCI can route the Command Complete event */
        prim->common.length = sender;

        /* Disable advertising */
        dm_ble_advertising_set_enable(HCI_ULP_ADVERTISING_DISABLED,
                                      DM_HCI_SENDER_ACL_MANAGER);

        send_to_hci((DM_UPRIM_T*)prim);
    }

    /* Re-enable advertising if required. We always want to enable it
       if the update is coming from the ACL Manager */
    if (sender == DM_HCI_SENDER_ACL_MANAGER
            || dm_ble_advertising.app_enabled != HCI_ULP_ADVERTISING_DISABLED)
        dm_ble_advertising_set_enable(HCI_ULP_ADVERTISING_ENABLED,
                                      DM_HCI_SENDER_ACL_MANAGER);
}

/*! \brief Change advertising parameters to allow connect-as-slave.

    Temporarily turn on directed connectable advertising, or undirected
    connectable advertising using the whitelist. When we get
    DM_ACL_OPENED_IND, we must restore the application's parameters and
    then re-enable advertising if appropriate.

    \param peer_addrt Pointer to Blueooth address of peer, or NULL to do
                        undirected connectable advertising.
    \param advertising_filter_policy whether or not undirected uses whitelist.
*/
void dm_ble_advertising_slave_connect(const TYPED_BD_ADDR_T * peer_addrt,
                                      CsrUint8 advertising_filter_policy)
{
    DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T *prim =
            zpnew(DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T);

    /* No need to set op_code. That gets done in
       dm_ble_advertising_new_parameters() */

    if (peer_addrt == NULL)
    {
        const DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T *p =
                p_dm_ble_conn_params == NULL ?
                &dm_ble_default_conn_params : p_dm_ble_conn_params;
        /* Undirected connectable with optional whitelist */
        prim->adv_interval_min = p->adv_interval_min;
        prim->adv_interval_max = p->adv_interval_max;
        prim->advertising_type = HCI_ULP_EV_ADVERT_CONNECTABLE_UNDIRECTED;
        prim->advertising_filter_policy = advertising_filter_policy |
            (dm_ble_advertising.prim == NULL ? 0 :
                (dm_ble_advertising.prim->advertising_filter_policy
                    & HCI_ULP_ADV_FP_ALLOW_CONNECTIONS));

        dm_ble_advertising.connect_undirected = TRUE;
        dm_ble_advertising.connect_directed = FALSE;
        dm_ble_advertising.connect_filter_policy = advertising_filter_policy;
    }
    else
    {
        dm_ble_advertising.connect_undirected = FALSE;
        dm_ble_advertising.connect_directed = TRUE;
        prim->advertising_type = HCI_ULP_EV_ADVERT_CONNECTABLE_DIRECTED;
        prim->direct_address_type = tbdaddr_copy_to_bd_addr(
                &prim->direct_address, peer_addrt);
    }
    prim->own_address_type = sm_privacy_address_type();
    prim->advertising_channel_map = HCI_ULP_ADVERT_CHANNEL_ALL;

    /* Set sender to DM_HCI_SENDER_ACL_MANAGER so the application is not
       notified of completion and we force the advertising to start. */
    dm_ble_advertising_new_parameters(prim, DM_HCI_SENDER_ACL_MANAGER);
}

/*! \brief Take copy of stored advertising parameters and send to HCI.

    \param sender See dm_ble_advertising_new_parameters().
*/
void dm_ble_advertising_apply_parameters(CsrUint8 sender)
{
    DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T *prim = dm_ble_advertising.prim;

    /* If the primitive is not NULL then we'll need to take
       a copy as we're sending the original to HCI. */
    if (prim != NULL)
        dm_ble_advertising.prim = pcopy(prim,
                sizeof(DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T));

    dm_ble_advertising.connect_undirected = FALSE;

    dm_ble_advertising_new_parameters(prim, sender);
}

/*! \brief Application request to set advertising parameters.

    \param uprim Pointer to primitive cast to (DM_UPRIM_T*)
*/
void dm_ble_advertising_set_parameters_req(const DM_UPRIM_T * const uprim)
{
    /* The op-code is currently wrong. It is set to
       DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ, which is not the same as
       HCI_ULP_SET_ADVERTISING_PARAMETERS. We ignore it for now. It gets
       handled in dm_ble_advertising_new_parameters(). */

    /* Store the parameters so that we can interupt and restore later.
       We take a copy because uprim gets freed by the caller. */
    CsrPmemFree(dm_ble_advertising.prim);
    dm_ble_advertising.prim = pcopy(
            &uprim->dm_ulp_set_advertising_parameters_req,
            sizeof(DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T));

    /* DM_HCI_SENDER_APPLICATION so the application is notified
       of completion and we don't force the advertising to start. */
    dm_ble_advertising_apply_parameters(DM_HCI_SENDER_APPLICATION);
}

/*! \brief The local address has changed. This probably means that
           privacy has been enabled or disabled, or the resolvable
           address timer has fired and we need to change the
           private address. */
void dm_ble_advertising_local_address_changed(CsrUint8 address_type)
{
    /* Change address type in application-controlled adverts. */
    if (dm_ble_advertising.prim != NULL)
        dm_ble_advertising.prim->own_address_type = address_type;

    if (dm_ble_advertising.connect_undirected)
    {
        /* Currently doing connect-as-slave with undirected advertising.
           We need to change the address we're putting in the adverts. */
        dm_ble_advertising_slave_connect(
                NULL,
                dm_ble_advertising.connect_filter_policy);
    }
    else if (!dm_ble_advertising.connect_directed)
    {
        dm_ble_advertising_apply_parameters(DM_HCI_SENDER_BLUESTACK);
    }
}

#endif /* INSTALL_ULP */

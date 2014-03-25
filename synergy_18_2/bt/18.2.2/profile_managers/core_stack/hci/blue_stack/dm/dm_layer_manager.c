/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_layer_manager.c

\brief  Device Manager functions.
*/

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "csr_bt_common.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_sched.h"
#ifdef BUILD_FOR_HOST
#include "csr_bt_tasks.h"
#endif
#include "csr_bt_core_stack_pmalloc.h"
#include "error.h"

#include "hci_prim.h"
#include "l2cap_prim.h"
#include "dm_prim.h"
#include "dm_acl_manager.h"
#include "dm_security_manager.h"
#include "dm_mgt_interface.h"
#include "dm_hci_interface.h"
#include "csr_bt_usr_config.h"
#include "dm_layer_manager.h"
#include "l2cap_control.h"
#include "sm.h"

/* Private Defines */

#if defined(SUPPORT_SEPARATE_LE_BUFFERS) && !defined(INSTALL_ULP)
#error "Cannot SUPPORT_SEPARATE_LE_BUFFERS on a chip that doesn't have INSTALL_ULP defined"
#endif


/* LYM_BT_Versions_Supported bitfield */
#define SUPPORT_BT_VERSION_2p0      0x0000  /* BT 2.0 is always supported */
#define SUPPORT_BT_VERSION_2p1      0x0001
#define SUPPORT_BT_VERSION_3p0      0x0002
#define SUPPORT_BT_VERSION_4p0      0x0004

/*
 * Event Masks
 */
/* Events present since BT1.1 */
#define HCI_EV_MASK_LO_ALL  ((hci_event_mask_t)0xFFFFFFFF)

/* BT1.2 and BT2.0 events */
#define HCI_EV_MASK_HI_2p0  ( HCI_EV_MASK_FLOW_SPEC_COMPLETE_HI \
                            | HCI_EV_MASK_INQUIRY_RESULT_RSSI_HI \
                            | HCI_EV_MASK_RR_EXT_FEATURES_HI \
                            | HCI_EV_MASK_FIXED_ADDRESS_HI \
                            | HCI_EV_MASK_ALIAS_ADDRESS_HI \
                            | HCI_EV_MASK_GENERATE_ALIAS_REQ_HI \
                            | HCI_EV_MASK_ACTIVE_ADDRESS_HI \
                            | HCI_EV_MASK_ALLOW_PRIVATE_PAIRING_HI \
                            | HCI_EV_MASK_ALIAS_ADDRESS_REQ_HI \
                            | HCI_EV_MASK_ALIAS_NOT_RECOGNISED_HI \
                            | HCI_EV_MASK_FIXED_ADDRESS_ATTEMPT_HI \
                            | HCI_EV_MASK_SYNC_CONN_COMP_HI \
                            | HCI_EV_MASK_SYNC_CONN_CHANGED_HI )

/* BT2.1 events (includes all BT1.2 and BT2.0 events) */
#define HCI_EV_MASK_HI_2p1  ( HCI_EV_MASK_HI_2p0 \
                            | HCI_EV_MASK_SNIFF_SUB_RATE_HI \
                            | HCI_EV_MASK_EXTENDED_INQUIRY_RES_HI \
                            | HCI_EV_MASK_ENCRYPTION_KEY_REFRESH_COMPLETE_HI \
                            | HCI_EV_MASK_IO_CAPABILITY_REQUEST_HI \
                            | HCI_EV_MASK_IO_CAPABILITY_RESPONSE_HI \
                            | HCI_EV_MASK_USER_CONFIRMATION_REQUEST_HI \
                            | HCI_EV_MASK_USER_PASSKEY_REQUEST_HI \
                            | HCI_EV_MASK_REMOTE_OOB_DATA_REQUEST_HI \
                            | HCI_EV_MASK_SIMPLE_PAIRING_COMPLETE_HI \
                            | HCI_EV_MASK_LST_CHANGE_HI \
                            | HCI_EV_MASK_ENHANCED_FLUSH_COMPLETE_EV_HI \
                            | HCI_EV_MASK_USER_PASSKEY_NOTIFICATION_HI \
                            | HCI_EV_MASK_KEYPRESS_NOTIFICATION_HI \
                            | HCI_EV_MASK_REM_HOST_SUPPORTED_FEATURES_HI)

#define HCI_EV_MASK_HI_BLE  HCI_EV_MASK_ULP_HI

#define HCI_EV_MASK_HI_4p0  (HCI_EV_MASK_HI_2p1 | HCI_EV_MASK_HI_BLE)

/*
 * Required features for BT 2.1
 */
#define FEATURES_2p1_OCTET_5    \
    ( HCI_FEATURE_MASK(LMP_FEATURES_SNIFF_SUB_RATE_BIT) \
    | HCI_FEATURE_MASK(LMP_FEATURES_ENCRYPTION_PAUSE_RESUME_BIT) )
#define FEATURES_2p1_OCTET_6    \
    ( HCI_FEATURE_MASK(LMP_FEATURES_EXTENDED_INQUIRY_RESPONSE_BIT) \
    | HCI_FEATURE_MASK(LMP_FEATURES_SIMPLE_PAIRING_BIT) \
    | HCI_FEATURE_MASK(LMP_FEATURES_ENCAPSULATED_PDU_BIT) \
    | HCI_FEATURE_MASK(LMP_FEATURES_NON_FLUSHABLE_PBF_BIT) )
#define FEATURES_2p1_OCTET_7    \
    ( HCI_FEATURE_MASK(LMP_FEATURES_LINK_SUPERVISION_TIMEOUT_BIT) \
    | HCI_FEATURE_MASK(LMP_FEATURES_INQUIRY_RESPONSE_TX_POWER_LEVEL_BIT))

#define FEATURES_4p0_OCTET_4    \
    (HCI_FEATURE_MASK(LMP_FEATURES_LE_SUPPORTED_CONTROLLER_BIT))

#if BT_VERSION_MAX > 15
#error "BT_VERSION_MAX > 15 - LYM_BT_Versions_Supported bitfield too small"
#endif

/* Data */
LYMCB_T LYMCB;

/* Private Function Prototypes */
static void LYM_Chip_Active(CsrUint16 dummy1, void *dumm2);

/*! \brief Initialise the Layer manager */
void LYM_Initialise(void)
{
#ifdef BUILD_FOR_HOST
    CsrMemSet(&LYMCB, 0, sizeof(LYMCB));
#endif

#ifdef INSTALL_AMP_SUPPORT
    LYMCB.fc.fragmentable = TRUE;
#endif

    /* Initialise non-zero defaults. */

    /* Current BT version is the lowest 4 bits and thus we can initialise the
       whole field with the lowest version. */
    LYMCB.BT_Version = BT_VERSION_2p0;
    LYMCB.flags = LYMCB_flags_default;
    LYMCB.Supported_Packet_Types = (HCI_PKT_DM1 + HCI_PKT_DM3 + HCI_PKT_DM5 +
                                    HCI_PKT_DH1 + HCI_PKT_DH3 + HCI_PKT_DH5 +
                                    HCI_PKT_HV1 + HCI_PKT_HV2 + HCI_PKT_HV3);

    /* Start a timer to ensure that, should the Host Controller not send an
       HCI_NOP on start-up, the LYM_Chip_Active will be called */
    timer_start(&LYMCB.CsrSchedTid_Chip_Active,
                DM_CONFIG_POWERUP_TIMEOUT,
                LYM_Chip_Active,
                0,
                NULL);
}

/*! \brief HCI event giving controller buffer sizes for ACL and SCO packets.

    \param p_prim Pointer to primitive.
*/
void LYM_Buffer_Size_Ind(HCI_READ_BUFFER_SIZE_RET_T *p_prim)
{
    /* Amount of data per HCI credit */
    LYMCB.fc.data_block_length = p_prim->acl_data_pkt_length;

    /* Total buffer space that exists. */
    LYMCB.fc.total_num_data_blocks = p_prim->total_acl_data_pkts;

    /* Amount of buffer space currently free (start off with all of it free) */
    LYMCB.fc.used_data_blocks = 0;

    /* Maximum amount of data that may be transmitted in a single HCI
       data packet. Note that normally this is equal to the amount of
       data associated with an HCI credit, but when we are on chip we
       optimize by combining HCI credits, which is equivalent to the
       HCI block-based flow-control for AMP. */
    LYMCB.fc.max_acl_data_packet_length = LYMCB.fc.data_block_length
#ifndef BUILD_FOR_HOST
        * LYMCB.fc.total_num_data_blocks
#endif
        ;

    /* prim->sco_data_pkt_length and prim->total_sco_data_pkts
     * are ignored because Bluestack does not handle SCO data.
     */

    LYMCB_SET(LYM_ControllerResponded);

    /* Send pending confirmations and notify L2CAP */
    if (!LYMCB_TEST(LYM_Confirmations_Sent))
    {
        LYMCB_SET(LYM_Confirmations_Sent);

        MCB_DeviceReady();

        if (am_phandle != CSR_SCHED_QID_INVALID)
            send_dm_am_register_cfm();

        dm_amp_device_ready();
    }

}

#ifdef INSTALL_ULP
void LYM_ULP_Buffer_Size_Ind(CsrUint16 data_packet_length, CsrUint8 num_data_packets)
{
    /* A non zero packet length is illegal for ULP and thus we will just
       panic.
    */
    if (data_packet_length != 0)
    {
#ifndef SUPPORT_SEPARATE_LE_BUFFERS
        BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_ULP_BUFFER_SIZE_RESPONSE);
#else
        /* Total buffer space that exists, credits supplied by the controller */
        LYMCB.le_fc.total_num_data_blocks = num_data_packets;

        /* Amount of data per HCI credit */
        LYMCB.le_fc.data_block_length = data_packet_length;

        /* Amount of buffer space currently free (start off with all of it free) */
        LYMCB.le_fc.used_data_blocks = 0;

        /* Maximum amount of data that may be transmitted in a single HCI
            data packet. This is equal to the amount of data associated with an
            HCI credit.
        */
        LYMCB.le_fc.max_acl_data_packet_length =  LYMCB.le_fc.data_block_length;

#endif
    }
#ifdef SUPPORT_SEPARATE_LE_BUFFERS
    else
    {
        /* Its virtually not possible that the chip returns a data packet length of
           0 bytes. Just rasie an exception at this point.
        */
        BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_ULP_BUFFER_SIZE_RESPONSE);
    }
#endif

}
#endif

/*! \brief Request to interrogate the controller.

    See if it's time to send all of the HCI initialise commands.
*/
void LYM_InterrogateController(void)
{
    /* Only interrogate controller if everyone's ready and we've not
       already done it. */
    if (LYMCB_TEST(LYM_ControllerReady | LYM_ApplicationReady)
           && !LYMCB_TEST(LYM_ControllerInterrogated))
    {
        HCI_READ_BUFFER_SIZE_T *rbs_prim;
        HCI_READ_LOCAL_SUPP_FEATURES_T *rlsf_prim;


        /* Send the HCI Commands to initialise the Host */
        rlsf_prim = pnew(HCI_READ_LOCAL_SUPP_FEATURES_T);
        rlsf_prim->common.op_code = HCI_READ_LOCAL_SUPP_FEATURES;
        rlsf_prim->common.length = DM_HCI_SENDER_LAYER_MANAGER;
        send_to_hci((DM_UPRIM_T*)rlsf_prim);


#ifdef BUILD_FOR_HOST
        {
            /* Synergy also sends HCI_HOST_BUFFER_SIZE to control the
               size of packets received from chip. See B-63221. */
            HCI_HOST_BUFFER_SIZE_T *hbs_prim;

            hbs_prim = pnew(HCI_HOST_BUFFER_SIZE_T);
            hbs_prim->common.op_code = HCI_HOST_BUFFER_SIZE;
            hbs_prim->common.length = DM_HCI_SENDER_LAYER_MANAGER;
            hbs_prim->acl_packet_len = DM_CONFIG_HOST_ACL_DATA_PACKET_LENGTH;
            hbs_prim->sco_packet_len = DM_CONFIG_HOST_SCO_DATA_PACKET_LENGTH;
            hbs_prim->acl_total_packets =
                    DM_CONFIG_HOST_TOTAL_NUM_ACL_DATA_PACKETS;
            hbs_prim->sco_total_packets =
                    DM_CONFIG_HOST_TOTAL_NUM_SCO_DATA_PACKETS;
            send_to_hci((DM_UPRIM_T*)hbs_prim);
        }
#endif /* BUILD_FOR_HOST */


        rbs_prim = pnew(HCI_READ_BUFFER_SIZE_T);
        rbs_prim->common.op_code = HCI_READ_BUFFER_SIZE;
        rbs_prim->common.length = DM_HCI_SENDER_LAYER_MANAGER;
        send_to_hci((DM_UPRIM_T*)rbs_prim);

        LYMCB_SET(LYM_ControllerInterrogated);
    }
}

#ifdef INSTALL_ULP
/*! \brief Request to interrogate the controller (LE only).

    See if it's time to send all of the HCI LE initialise commands.
*/
void LYM_ULP_Interrogate_Controller(void)
{
    /* Only need to do this if :
       a) it hasnt been done before
       b) the current BT version is 4.0.
    */

    /*
     * The code under if statement would be executed only once in life
     * time of Bluestack when set version request=4 is called.
     * Next time when request for setting version=4, comes,
     * the function will simply send confirmation
     * to upper layer without processing the HCI commands second time.
     *
     * This change is based on assumption that when setting version request
     * comes in a sequence of 4 and then 2, the DM state, sm module, and
     * variables related to LE stay intact and need not to be reinitialized
     * when request for version 4 comes again.
     */
    if (!LYMCB_TEST(LYM_ULP_ControllerReady))
    {
        HCI_ULP_READ_BUFFER_SIZE_T *urbs_prim;
        HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_T *urlsf_prim;
        HCI_WRITE_LE_HOST_SUPPORT_T *wlhs_prim;

        sm_init();

        urlsf_prim = pnew(HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_T);
        urlsf_prim->common.op_code = HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES;
        urlsf_prim->common.length = DM_HCI_SENDER_LAYER_MANAGER;
        send_to_hci((DM_UPRIM_T*)urlsf_prim);

        urbs_prim = pnew(HCI_ULP_READ_BUFFER_SIZE_T);
        urbs_prim->common.op_code = HCI_ULP_READ_BUFFER_SIZE;
        urbs_prim->common.length = DM_HCI_SENDER_LAYER_MANAGER;
        send_to_hci((DM_UPRIM_T*)urbs_prim);


        wlhs_prim = zpnew(HCI_WRITE_LE_HOST_SUPPORT_T);
        wlhs_prim->common.op_code = HCI_WRITE_LE_HOST_SUPPORT;
        wlhs_prim->common.length = DM_HCI_SENDER_LAYER_MANAGER;
        wlhs_prim->le_supported_host = 1;
        send_to_hci((DM_UPRIM_T*)wlhs_prim);

        LYMCB_SET(LYM_ULP_ControllerReady);
    }
    else
    {
        LYM_Set_BT_Version_CFM(HCI_SUCCESS);
    }

}


/*! \brief Stores LE local supported features from HCI event.

    \param features Pointer to HCI event return.
*/
void LYM_ULP_Store_Local_Supported_Features(
                                HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_RET_T * features)
{
    CsrUint8 *ulmpsf;

    LYMCB_CLEAR(LYM_ULP_Encryption_Supported);

    ulmpsf = features->feature_set;


    /* Definitions of supported features as found in core 4.0 spec, volume 6,  table 4.3 */
    if (ULP_FEATURE_IS_SUPPORTED(ulmpsf,ULP_FEATURE_ENCRYPTION))
        LYMCB_SET(LYM_ULP_Encryption_Supported);
}
#endif


/*! \brief Stores local supported features from HCI event.

    \param features Pointer to HCI event return.
*/
void LYM_Store_Local_Supported_Features(
                                HCI_READ_LOCAL_SUPP_FEATURES_RET_T * features)
{
    CsrUint8 *lmpsf;

    LYMCB_CLEAR(LYM_Role_Switch_Supported | LYM_Encryption_Supported
            | LYM_Packet_Boundary_Flag_Supported);

    lmpsf = features->lmp_supp_features;

    /* Definitions of supported features as found in LMP spec table 5.3 */
    if (HCI_FEATURE_IS_SUPPORTED(LMP_FEATURES_SWITCH_BIT, lmpsf))
        LYMCB_SET(LYM_Role_Switch_Supported);

    if (HCI_FEATURE_IS_SUPPORTED(LMP_FEATURES_ENCRYPTION_BIT, lmpsf))
        LYMCB_SET(LYM_Encryption_Supported);

    if (HCI_FEATURE_IS_SUPPORTED(LMP_FEATURES_NON_FLUSHABLE_PBF_BIT, lmpsf))
        LYMCB_SET(LYM_Packet_Boundary_Flag_Supported);

    LYMCB.Supported_Packet_Types = HCI_PKT_DM1 + HCI_PKT_DH1 + HCI_PKT_HV1;

    if (HCI_FEATURE_IS_SUPPORTED(LMP_FEATURES_THREE_SLOT_PACKETS_BIT, lmpsf))
                    LYMCB.Supported_Packet_Types += HCI_PKT_DM3 + HCI_PKT_DH3;
    if (HCI_FEATURE_IS_SUPPORTED(LMP_FEATURES_FIVE_SLOT_PACKETS_BIT, lmpsf))
                    LYMCB.Supported_Packet_Types += HCI_PKT_DM5 + HCI_PKT_DH5;
    if (HCI_FEATURE_IS_SUPPORTED(LMP_FEATURES_HV2_PACKETS_BIT, lmpsf))
                    LYMCB.Supported_Packet_Types += HCI_PKT_HV2;
    if (HCI_FEATURE_IS_SUPPORTED(LMP_FEATURES_HV3_PACKETS_BIT, lmpsf))
                    LYMCB.Supported_Packet_Types += HCI_PKT_HV3;

    /*
     * Set BT versions bitfield from supported features
     */

    /* We always support BT2.0 */
    LYMCB.BT_Versions_Supported = SUPPORT_BT_VERSION_2p0;

    /* BT 2.1 and 3.0 */
    if ((~lmpsf[5] & FEATURES_2p1_OCTET_5) == 0 &&
        (~lmpsf[6] & FEATURES_2p1_OCTET_6) == 0 &&
        (~lmpsf[7] & FEATURES_2p1_OCTET_7) == 0)
    {
        LYMCB.BT_Versions_Supported |= (SUPPORT_BT_VERSION_2p1 | SUPPORT_BT_VERSION_3p0);
    }

#ifdef INSTALL_ULP
    /* BT 4.0 */
    if ((~lmpsf[4] & FEATURES_4p0_OCTET_4) == 0)
    {
        LYMCB.BT_Versions_Supported |= SUPPORT_BT_VERSION_4p0;
    }
#endif
}

/*! \brief Application request to set Bluetooth Version.

    We check it against the supported versions.
    If it's supported then we set the event mask.

    \param version Requested version, or BT_VERSION_CURRENT to read current.
*/
void LYM_Set_BT_Version(CsrUint8 version)
{
    HCI_SET_EVENT_MASK_T *p_prim;
    CsrUint32 masks[] = {0, HCI_EV_MASK_HI_2p0,
                        HCI_EV_MASK_HI_2p1, HCI_EV_MASK_HI_2p1,
                        HCI_EV_MASK_HI_4p0};
    CsrUint8 vers;
    HCI_ULP_SET_EVENT_MASK_T *usem_prim;


    /* Clear the requested and used versions in the LYMCB structure. */
    LYMCB.BT_Version &= LYM_READ_CURRENT_BT_VERSION_MASK;


    if (version == BT_VERSION_CURRENT)
    {
        /* As we have already cleared the used and requested versions and we
           know that the current version is in the lowest bits, we can just
           read BT_Version directly here.
        */
        version = (CsrUint8)LYMCB.BT_Version;
    }


     /*
      * Compare if the version requested is already been set.
      * If requested version is same as of current set version,
      * send confirmation to upper layers without setting the version
      * again.
      *
      * Inside the 'if' statement, LYM_Set_BT_Version_CFM could also have
      * been called with status HCI_SUCCESS as an alternative implementation,
      * but it is preferred to avoid a function call for efficiency and directly
      * send message to DM.
      */
    if(version==(CsrUint8)LYMCB.BT_Version)
    {
        DM_SET_BT_VERSION_CFM_T *p_prim;
        p_prim = pnew(DM_SET_BT_VERSION_CFM_T);
        p_prim->type = DM_SET_BT_VERSION_CFM;
        p_prim->version = version;
        p_prim->status = HCI_SUCCESS;

        DM_SendMessageAMphandle(p_prim);
        return;
    }




    /* Record the requested version in the NEW_BT_VERSION part */
    LYM_WRITE_REQ_BT_VERSION(LYMCB.BT_Version, version);

    /* Find the highest supported version up to and including the one
       requested. */
    for (vers = version; vers >= BT_VERSION_2p0 && vers <= BT_VERSION_MAX; vers-- )
    {
        if (vers == BT_VERSION_2p0 || (LYMCB.BT_Versions_Supported & (1 << (vers-2))) != 0)
        {
            /* Version supported so try to set the event mask */
            p_prim = pnew(HCI_SET_EVENT_MASK_T);
            p_prim->common.op_code = HCI_SET_EVENT_MASK;
            p_prim->event_mask[0] = HCI_EV_MASK_LO_ALL;
            p_prim->event_mask[1] = masks[vers];
            p_prim->common.length = DM_HCI_SENDER_LAYER_MANAGER;
            send_to_hci((DM_UPRIM_T*)p_prim);

            LYM_WRITE_USED_BT_VERSION(LYMCB.BT_Version, vers);


            if(BT_VERSION_4p0==LYM_READ_USED_BT_VERSION(LYMCB.BT_Version))
            {
                usem_prim = pnew(HCI_ULP_SET_EVENT_MASK_T);
                usem_prim->common.op_code = HCI_ULP_SET_EVENT_MASK;
                usem_prim->ulp_event_mask[0] = HCI_EV_MASK_ULP_DEFAULT_LO;
                usem_prim->ulp_event_mask[1] = HCI_EV_MASK_ULP_DEFAULT_HI;
                usem_prim->common.length = DM_HCI_SENDER_LAYER_MANAGER;
                send_to_hci((DM_UPRIM_T*)usem_prim);
            }

            return ;
        }
    }

    LYM_Set_BT_Version_CFM(HCI_ERROR_UNSUPPORTED_FEATURE);
}

/*! \brief Send DM_SET_BT_VERSION_CFM to application.

    \param status Indication of success or failure.
*/
void LYM_Set_BT_Version_CFM(CsrUint8 status)
{
    DM_SET_BT_VERSION_CFM_T *p_prim;
    CsrUint16 new_version = LYM_READ_USED_BT_VERSION(LYMCB.BT_Version);

    /* New version -> current version */
    if (status == HCI_SUCCESS)
    {
        if (LYM_READ_REQ_BT_VERSION(LYMCB.BT_Version) != new_version)
        {
            status = HCI_ERROR_UNSUPPORTED_FEATURE;
        }

        LYM_WRITE_CURRENT_BT_VERSION(LYMCB.BT_Version, new_version);
    }

    p_prim = pnew(DM_SET_BT_VERSION_CFM_T);
    p_prim->type = DM_SET_BT_VERSION_CFM;
    p_prim->version = LYM_READ_CURRENT_BT_VERSION(LYMCB.BT_Version);
    p_prim->status = status;

    DM_SendMessageAMphandle(p_prim);
}

/*! \brief Called by statup timer to make sure we don't wait forever
           for the controller to tell us that it's ready.

    \param dummy1 Ignored.
    \param dummy2 Ignored.
*/
static void LYM_Chip_Active(CsrUint16 dummy1, void *dummy2)
{
    CSR_UNUSED(dummy1);
    CSR_UNUSED(dummy2);

    TIMER_EXPIRED(LYMCB.CsrSchedTid_Chip_Active);

    if(!LYMCB_IS_FLAG_SET(LYM_ControllerReady))
    {
        HCI_RESET_T *hr;

        hr = pnew(HCI_RESET_T);
        hr->common.op_code = HCI_RESET;
        hr->common.length = DM_HCI_SENDER_LAYER_MANAGER;
        send_to_hci((DM_UPRIM_T*)hr);

        LYMCB_SET(LYM_ControllerReady);
    }

    LYM_InterrogateController();
}


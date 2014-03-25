/*!

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

\file              dm_security_manager.c

\brief             The security management entity of the BlueStack Device Manager.



\details
The Security Manager deals with authorisation, authentication, encryption and
bonding. The processes can be initiated either directly by the application
or as a result of an access request from L2CAP or higher layer protocol before
connecting a service or profile that requires security.

\par Authorisation
The security manager asks the application if an incoming or outgoing
connection, involving some service or profile, is permitted with some remote
device. We don't need to have any proof that they are who they say they
are. The question is: if they are who they say they are then can they
come in? The application may defer to the user. In security modes 2 and 3,
we do authorisation after authentication, but in security mode 4, we do it
the other way around. Authorisation tends to be cheaper, both from the
point of view of processor time and user annoyance, so it's best done first.
Authorisation only makes sense as part of an access request and so cannot
be initiated directly by the application.

\par Authentication
We try to make sure that they are who they say they are. We do this by
pairing with them and generating a shared link key, or by verifying an
existing link key. As of Bluetooth 2.1, Secure Simple Pairing (SSP) is
used to generate link keys.

\par Encryption
Data transfer on the ACL between the two devices can be encrypted. You
must authenticate before encrypting as encryption keys are derived from
link keys.

\par Bonding
This is a special case of authentication. When bonding, the Security
Manager deletes any existing link key for the remote device, creates
a connection, authenticates the connection, and then closes the connection.

\par Security Manager state machine.
http://noo-noo/~ab12/upper_layers_wiki/dm_sm_DM_SM.html

\par GAP Security Modes
The General Access Profile defines security modes for compliant devices.
As of Bluetooth 2.1, we are to use security mode 4. Support for other modes
is only maintained to allow interopability with legacy devices.

\par
Mode 0. This isn't a GAP security mode, but our security manager uses the
concept to mean that Bluestack isn't handling security and so isn't in any
mode. The application is then supposed to do what's needed. Bluestack
defaults to Mode 0 when it is initialised.

\par
Mode 1. No security. The security manager does not initiate security when in
this mode. It is functionally equivalent to Mode 0.

\par
Mode 2. Service-based security. The security manager does not initiate
security until there has been an L2CAP (or higher layer) connection attempt.
For outgoing connections, the security manager will be sent an ACCESS request
before the protocol sends the connection request. The connection request will
only be sent if the security manager consents. For incoming connections,
the security manager is notified before the connection is accepted.

\par
Mode 3. Link level security. In this mode, authentication and encryption
are initiated during ACL setup. Authorisation is still service-based.

\par
Mode 4. Service-based security with secure simple pairing. Mode 4 is like
Mode 2, but we use secure simple pairing to authenticate. In addition, the
link is always encrypted after authentication. Security processes are triggered
before an outgoing connection request is sent and before an incoming
connection attempt is accepted. The protocol layer issues a DM_SM_ACCESS_REQ
to the security manager containing details of the service and the pending
connection. The security manager looks for the service in its database
and determines the security requirements. It will then ask the application
for authorisation (if required). Then the security manager checks the ACL
to see if it has already been authenticated and, if so, what sort of link
key has been generated. If the link has not yet been authenticated, or the
associated link key isn't the right type, then the security manager will
initiate pairing by issuing HCI_AUTH_REQ. This will cause HCI_EV_LINK_KEY_REQ
to be generated. The security manager checks its device
database for a link key. If there's nothing suitable, the security manager will
pass on the request to the application. If the application doesn't have one
either then we reject the HCI link key request and pairing will start.
Otherwise we reply with the link key, which the controller will try to
authenticate. If authentication/pairing completes successfully, the
security manager will turn on encryption for the link and finally report
success to the protocol layer that called it.

\par Secure Simple Pairing (SSP)
Two Bluetooth 2.1 devices will use SSP to generate a shared link key.
Pairing starts when the initiator rejects the HCI link key request event
after issuing HCI_AUTH_REQ. The first step is for the two devices to
exchange IO capability information to determine what sort of user input
(if any) is required. At this stage, the devices also disclose their
authentication requirements (whether they require Man In The Middle (MITM)
protection and whether they wish to bond) and whether or not they have
any Out Of Band (OOB) data for the remote device. The devices use this
information to determine the association model for the pairing. If either
device has OOB data then the OOB model will be used and OOB data is
exchanged. Otherwise, if neither device has said that it needs MITM
protection then the 'Just Works' model will be used. Otherwise either
'numerical comparison' or 'passkey entry' is used, depending on the
IO capabilities of the two devices. The devices then calculate the
link key, authenticate it, and issue it to the security manager. Finally
the security manager gets HCI_EV_AUTH_COMPLETE and
HCI_EV_SIMPLE_PAIRING_COMPLETE.

\par IO Capabilities Exchange.
The security manager on the initiator receives HCI_EV_IO_CAPABILITY_REQUEST.
We issue DM_SM_IO_CAPABILITY_REQUEST_IND to the application. The application
replies with DM_SM_IO_CAPABILITY_REQUEST_RSP, providing the security manager
with any OOB data it has at this point. The security manager stores the
parameters and any OOB data and then replies to the HCI event with
HCI_IO_CAPABILITY_RESPONSE. The security manager on the responder then
receives HCI_EV_IO_CAPABILITY_RESPONSE, which it passes on to the application.
We then get the same events the other way around. The responder gets
HCI_EV_IO_CAPABILITY_REQUEST and passes DM_SM_IO_CAPABILITY_REQUEST_IND to
its application. It receives DM_SM_IO_CAPABILITY_REQUEST_RSP and stores the
data before sending HCI_IO_CAPABILITY_RESPONSE across HCI. Finally the
security manager on the initiator receives HCI_EV_IO_CAPABILITY_RESPONSE,
which it passes on to the application.

\par OOB data.
If one of the security managers claimed to have OOB data then this is
requested with HCI_EV_REMOTE_OOB_DATA_REQUEST. The security manager
gets the OOB data it stored during the IO capabilities exchange and then
replies to HCI with HCI_REMOTE_OOB_DATA_REQUEST_REPLY.

\par Just Works.
The security managers on both devices receive HCI_EV_USER_CONFIRMATION_REQUEST
and they reply automatically with HCI_USER_CONFIRMATION_REQUEST_REPLY. They
don't tell the application.

\par Numerical Comparison.
The security managers on both devices receive HCI_EV_USER_CONFIRMATION_REQUEST
and they pass this on to the application with
DM_SM_USER_CONFIRMATION_REQUEST_IND. These primitives contain a six digit
number which must be displayed by both devices until the simple pairing
complete event is received. The user is instructed to compare the numbers
and confirm that they are the same.

\par Passkey Entry.
The security manager on the device with a keyboard receives
HCI_EV_USER_PASSKEY_REQUEST and the security manager on the device with a
display receives HCI_EV_USER_PASSKEY_NOTIFICATION. These are passed to the
application in DM_SM_USER_PASSKEY_REQUEST_IND and
DM_SM_USER_PASSKEY_NOTIFICATION_IND respectively. A passkey is displayed on
the device with a screen and must be typed in on the device with a keyboard.
When a key is pressed on the keyboard, a DM_SM_SEND_KEYPRESS_NOTIFICATION_REQ
is sent to the DM, which becomes a HCI_SEND_KEYPRESS_NOTIFICATION, is sent
across the air to become a HCI_EV_KEYPRESS_NOTIFICATION and then a
DM_SM_KEYPRESS_NOTIFICATION_IND on the device with a display. In this way,
the device with a display learns about keypresses on the keyboard and can
do cool things with asterisks to help prevent the user from becoming confused.

\par Legacy pairing.
If either device is Bluetooth 2.0 or earlier, we drop back to using legacy
pairing. After the link key request is rejected, the initiator gets
HCI_EV_PIN_CODE_REQ, which it passes to the application as
DM_SM_PIN_REQUEST_IND. The application replies with DM_SM_PIN_REQUEST_RSP,
which goes across HCI as HCI_PIN_CODE_REQ_REPLY. This is then repeated
on the responder. PIN codes can be up to 16 characters. There is no
HCI_EV_SIMPLE_PAIRING_COMPLETE.

\par Link key types.
Legacy pairing produces HCI_COMBINATION_KEY.
'Just works' produces HCI_UNAUTHENTICATED_COMBINATION_KEY.
Other SSP association models produce HCI_AUTHENTICATED_COMBINATION_KEY.
HCI_AUTHENTICATED_COMBINATION_KEY provides good eavesdropper and MITM
protection. HCI_UNAUTHENTICATED_COMBINATION_KEY provides good
eavesdropper but no MITM protection. HCI_COMBINATION_KEY provides some
eavesdropper and MITM protection, depending on the choice and length of
PIN code.
*/

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"
#include "csr_bt_common.h"
#include "csr_mblk.h"
#include "error.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_util.h"

#ifdef BUILD_FOR_HOST
#include "csr_bt_core_stack_pmalloc.h"
#else
#include "io_defs.h"
#include "csr_util.h"
#endif

#include "csr_bt_usr_config.h"
#include "dm_acl_manager.h"
#include "dm_security_manager.h"
#include "dm_layer_manager.h"
#include "dm_hci_interface.h"
#include "dm_mgt_interface.h"
#include "dm_amp_interface.h"
#include "dm_sm_fsm_definitions.h"
#include "dm_acl_connection_fsm_definitions.h"
#include "sm.h"
#include "tbdaddr.h"

#ifndef DISABLE_DM_BREDR


/* We just need a DM prim type that will never be a valid security request.
   DM_SM_REGISTER_REQ is used because it doesn't initiate any
   security process. It will never appear on the request queue. */
#define DM_SM_ZOMBIE_REQ DM_SM_REGISTER_REQ

/*! authentication_requirements bits */
#define DM_SM_AUTH_REQ_NONE                 0x00
#define DM_SM_AUTH_REQ_MITM                 0x01
#define DM_SM_AUTH_REQ_DEDICATED_BONDING    0x02
#define DM_SM_AUTH_REQ_GENERAL_BONDING      0x04

/*! Interpretation of multiple bit options in DM_SM_INIT_REQ_T.
    See dm_prim.h */
#define DM_SM_INIT_SECURITY_MODE4           (1<<1)
#define DM_SM_INIT_AUTHENTICATION           (1<<2)
#define DM_SM_INIT_SM4_DEBUG_MODE           (1<<5)
#define DM_SM_INIT_ENCRYPTION               (1<<8)

/*! Shift bits in DM_SM_INIT_CFM_T status so we can store them in the
    options field of our stored copy of DM_SM_INIT_REQ_T while we're
    dealing with the primitive. */
#define DM_SM_INIT_SHIFT(bit)               ((bit) << 11)

#ifndef BUILD_FOR_HOST
/* Make sure that the above stays consistent with dm_prim.h */
CSR_BT_COMPILE_TIME_ASSERT((DM_SM_INIT_SECURITY_MODE & DM_SM_INIT_SECURITY_MODE4) != 0, DM_SM_INIT_REQ_options_definitions_inconsistent1);
CSR_BT_COMPILE_TIME_ASSERT((DM_SM_INIT_SECURITY_MODE & DM_SM_INIT_AUTHENTICATION) != 0, DM_SM_INIT_REQ_options_definitions_inconsistent2);
CSR_BT_COMPILE_TIME_ASSERT((DM_SM_INIT_SECURITY_MODE & DM_SM_INIT_ENCRYPTION) != 0, DM_SM_INIT_REQ_options_definitions_inconsistent3);
CSR_BT_COMPILE_TIME_ASSERT((DM_SM_INIT_CONFIG & DM_SM_INIT_SM4_DEBUG_MODE) != 0, DM_SM_INIT_REQ_options_definitions_inconsistent4);
CSR_BT_COMPILE_TIME_ASSERT((DM_SM_INIT_MODE3_ENC & DM_SM_INIT_ENCRYPTION) != 0, DM_SM_INIT_REQ_options_definitions_inconsistent5);
CSR_BT_COMPILE_TIME_ASSERT((DM_SM_INIT_ALL & (0xFFFF & ~(CsrUint16)(DM_SM_INIT_SHIFT(1) - 1))) == 0, DM_SM_INIT_REQ_options_definitions_inconsistent6);
#endif

/* Security Timers. */
typedef enum
{
    DM_SM_TIMER_SECURITY_DETACH,
    DM_SM_TIMER_REAUTHENTICATION_DELAY,

    DM_SM_TIMER_MAX         /* Keep this one last. */
} DM_SM_TIMER_T;

/*===========================================================================*
    Private Data
 *===========================================================================*/

static struct {
    /*! Default security level, defaulting to SECL_DEFAULT */
    dm_security_level_t secl_default;

    /*! Current security mode. */
    dm_security_mode_t security_mode;

    /*! Configuration bitfield. Bottom two bits store mode3_enc. */
#define DM_SM_SEC_MODE_CONFIG_MODE3_ENC_MASK        0x0003
    CsrUint16 config;

    /*! Use of HCI_WRITE_AUTH_ENABLE to avoid legacy timeout */
    CsrUint16 write_auth_enable;

    /*! Track parameter change request. */
    DM_SM_INIT_REQ_T *change;
} SMCB;

/*! Private Function Prototypes */

/*! Send Message to upper layers. */
static void send_dm_sm_authorise_ind(DM_SM_ACCESS_REQ_T *p_req);
static void send_dm_sm_authenticate_cfm(const TYPED_BD_ADDR_T *const addrt, const CsrBool success);
static void send_dm_sm_encrypt_cfm(const TYPED_BD_ADDR_T *const addrt, const CsrBool success, const CsrBool encrypted);
static void send_dm_sm_encryption_change(const TYPED_BD_ADDR_T *const addrt, const CsrBool encrypted);
static void send_dm_sm_pin_request_ind(const TYPED_BD_ADDR_T *const addrt);


static void send_dm_sm_keys_ind(const TYPED_BD_ADDR_T *const addrt, const CsrUint8 key_type, const CsrUint8 key[SIZE_LINK_KEY]);
static void send_dm_sm_key_request_ind(const TYPED_BD_ADDR_T *const addrt);

#ifdef BUILD_FOR_HOST_OLD_LINK_KEY_API
static DM_UPRIM_T *dm_sm_api_translate(DM_UPRIM_T *uprim);
#endif /* BUILD_FOR_HOST_OLD_LINK_KEY_API */

static void send_dm_sm_bonding_cfm(const TYPED_BD_ADDR_T *const addrt, const hci_error_t status);
static void send_dm_sm_access_cfm(const DM_SM_ACCESS_REQ_T *const p_req, const hci_return_t status);

static void send_hci_link_key_req_neg_reply(const BD_ADDR_T *const p_bd_addr);
static void send_hci_link_key_req_reply(const BD_ADDR_T *const p_bd_addr, const CsrUint8 key[SIZE_LINK_KEY]);
static void send_hci_pin_code_req_neg_reply(const BD_ADDR_T *const p_bd_addr);
static void send_hci_pin_code_req_reply(const BD_ADDR_T *const p_bd_addr, CsrUint8 pin_code_length, const CsrUint8 *pin);
static void send_hci_auth_req(const hci_connection_handle_t handle);
static void send_hci_set_conn_encryption(const hci_connection_handle_t handle, const hci_link_enc_t enc_enable);
static void send_hci_io_capability_response(const TYPED_BD_ADDR_T *const addrt, DM_SM_SSP_T *const p_sm_ssp);
static void send_hci_io_capability_neg_response(const BD_ADDR_T *const bd_addr, const hci_error_t reason);
static void send_sm_to_hci(HCI_UPRIM_T *hci_uprim, const hci_op_code_t op_code);
#ifndef DISABLE_DM_BREDR
static void send_dm_sm_io_capability_request_ind(const TYPED_BD_ADDR_T *const addrt);
static void send_dm_sm_user_confirmation_request_ind(const TYPED_BD_ADDR_T *const addrt, const CsrUint32 *const numeric_value, CsrUint8 io_cap_local, CsrUint8 io_cap_remote, CsrUint16 response_required);
static void send_hci_write_enc_mode(const CsrUint8 mode);
static void send_hci_write_simple_pairing_mode(const CsrUint8 simple_pairing_mode);
#endif

/*! Link key type utilities. */
static CsrBool dm_sm_is_legacy(DM_ACL_T *p_acl);
static CsrUint8 bonding_auth_req(void);

/*! State machine utilities. */
static DM_SM_REQUEST_T *add_sm_request(DM_ACL_T *p_acl);
static void sm_state_machine(DM_ACL_T *p_acl, const fsm_event_t event, void *pv_data);
static void dm_sm_free_request(DM_SM_REQUEST_T **pp_req);
static void kick_request_process(DM_ACL_T *p_acl);
static CsrBool dm_sm_is_initiator(const TYPED_BD_ADDR_T *const addrt);

/*! Timed event functions. */
static void dm_sm_retry_transaction(CsrUint16 event, void *ptr);
static void detach_after_delay(CsrUint16 unused, void *ptr);

/*! Other static functions. */
static void send_hci_refresh_encryption_key(DM_ACL_T *p_acl);
static void dm_sm_simple_pairing_complete(DM_ACL_T *p_acl, const CsrUint8 status);
#ifndef DISABLE_DM_BREDR
static void send_hci_user_confirmation_request_reply(hci_op_code_t opcode, const TYPED_BD_ADDR_T *const addrt);
static void dm_sm_parameter_init(CsrUint16 failure);
#endif

/*! ACL Manager functions. */
static DM_ACL_RESULT_T dm_sm_acl_open(const TYPED_BD_ADDR_T *const addrt, const CsrBool incoming, DM_ACL_T **pp_acl);
static void dm_sm_acl_close(DM_ACL_T *const p_acl, hci_reason_t reason, const ACL_IDLE_TIMEOUT_MULTIPLIER_T idle_multiplier);
static void dm_sm_acl_set_timeout(DM_ACL_T *p_acl);

/*! Public functions to handle application requests. */

/*! \brief Change the security mode, default security level and config.

    For new devices (Bluetooth V2.1), this should be used to enter security
    mode 4 before the device is made discoverable or connectable and it
    should never be called again.

    \param p_uprim Pointer to primitive.
*/
void dm_sm_handle_init_req(DM_UPRIM_T *const p_uprim)
{
    /* Record new parameters. */
    CsrPmemFree(SMCB.change);
    SMCB.change = pcopy(p_uprim, sizeof(DM_SM_INIT_REQ_T));

    /* Start moving towards the new parameters. */
    dm_sm_parameter_init(0);
}

#ifndef SM_HAS_FUNCTION_FOR_RAND
void dm_sm_ble_init_complete(void)
{
    dm_sm_parameter_init(0);
}
#endif

static void dm_sm_parameter_init(CsrUint16 failure)
{
    DM_SM_INIT_CFM_T *prim;
    DM_SM_INIT_REQ_T change;

    /* Make sure we are trying to initialise parameters. */
    if (SMCB.change == NULL)
        return;

    change = *SMCB.change;
    change.mode3_enc &= DM_SM_SEC_MODE_CONFIG_MODE3_ENC_MASK;
    change.options |= failure;

    /* Skip mode changes if new mode is the same as the current mode. */
    if (SMCB.security_mode == change.security_mode)
        change.options &=~(DM_SM_INIT_SECURITY_MODE4|DM_SM_INIT_AUTHENTICATION);

    /* Handle change to Security Mode 4 */
    if((change.options & DM_SM_INIT_SECURITY_MODE4) != 0)
    {
        change.options &= ~DM_SM_INIT_SECURITY_MODE4;

        if (change.security_mode == SEC_MODE4_SSP)
        {
            send_hci_write_simple_pairing_mode(
                    HCI_SIMPLE_PAIRING_MODE_ENABLED);
            *SMCB.change = change;
            return;
        }
    }

    /* Ensure host is not allowed to leave Security Mode 4 */
    if (SMCB.security_mode == SEC_MODE4_SSP)
        change.security_mode = SEC_MODE4_SSP;

    /* Handle change to/from Security Mode 3 */
    if((change.options & DM_SM_INIT_AUTHENTICATION) != 0)
    {
        change.options &= ~DM_SM_INIT_AUTHENTICATION;

        /* Change mode */
        switch (change.security_mode)
        {
            case SEC_MODE0_OFF:
            case SEC_MODE1_NON_SECURE:
            case SEC_MODE2_SERVICE:
            case SEC_MODE2a_DEVICE:
            case SEC_MODE4_SSP:
                if (SMCB.security_mode == SEC_MODE3_LINK)
                {
                    /* Disable authentication mode */
                    send_hci_write_auth_enable(HCI_AUTH_ENABLE_OFF,
                            DM_HCI_SENDER_SECURITY_MANAGER);

                    *SMCB.change = change;
                    return;
                }
                break;

            case SEC_MODE3_LINK:
                /* Enable authentication mode */
                send_hci_write_auth_enable(HCI_AUTH_ENABLE_ON,
                        DM_HCI_SENDER_SECURITY_MANAGER);
                *SMCB.change = change;
                return;

            default:
                /* Ignore unknown security mode. */
                change.security_mode = SMCB.security_mode;
                change.options |=
                    DM_SM_INIT_SHIFT(DM_SM_INIT_FAIL_UNSUPPORTED_SECURITY_MODE);
                break;
        }
    }

    if ((change.options & DM_SM_INIT_ENCRYPTION) != 0)
    {
        change.options &= ~DM_SM_INIT_ENCRYPTION;

        if (change.security_mode != SEC_MODE3_LINK)
            change.mode3_enc = HCI_ENC_MODE_OFF;
        else if ((change.options & DM_SM_INIT_MODE3_ENC) == 0)
            change.mode3_enc =
                    SMCB.config & DM_SM_SEC_MODE_CONFIG_MODE3_ENC_MASK;

        if (change.mode3_enc !=
                (SMCB.config & DM_SM_SEC_MODE_CONFIG_MODE3_ENC_MASK))
        {
            /* Change the encryption mode */
            send_hci_write_enc_mode(change.mode3_enc);

            *SMCB.change = change;
            return;
        }
    }

    if ((change.options & DM_SM_INIT_SM4_DEBUG_MODE) != 0)
    {
        change.options &= ~DM_SM_INIT_SM4_DEBUG_MODE;

        if (change.security_mode == SEC_MODE4_SSP)
        {
            /* Change the debug mode */
            HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE_T *hci_prim;

            hci_prim = zpnew(HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE_T);

            if ((change.config & DM_SM_SEC_MODE_CONFIG_DEBUG_MODE) !=0)
                hci_prim->simple_pairing_debug_mode = 1;

            send_sm_to_hci((HCI_UPRIM_T*)hci_prim,
                    HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE);
            *SMCB.change = change;
            return;
        }
    }

    if (!sm_update_ir_er_div(
            (CsrUint16)(change.options & (DM_SM_INIT_SM_STATE)),
            &change.sm_key_state,
            &change.sm_div_state,
            &change.sm_sign_counter))
    {
        /* SM still waiting for random numbers. */
        change.options &= ~DM_SM_INIT_SM_STATE;
        *SMCB.change = change;
        return;
    }

    if((change.options & DM_SM_INIT_ENC_SIZE_CONF) != 0)
    {
        change.options &= ~DM_SM_INIT_ENC_SIZE_CONF;
        sm_set_enc_key_sizes(change.max_enc_key_size_thres, change.min_enc_key_size_thres);
    }

    /* All done. Update parameters and send confirmation to application. */
    prim = zpnew(DM_SM_INIT_CFM_T);
    prim->type = DM_SM_INIT_CFM;

    if ((change.options & DM_SM_INIT_SECURITY_MODE) != 0)
        SMCB.security_mode = change.security_mode;
    prim->security_mode = SMCB.security_mode = change.security_mode;

    if ((change.options & DM_SM_INIT_SECURITY_LEVEL_DEFAULT) != 0)
        SMCB.secl_default = change.security_level_default;
    prim->security_level_default = SMCB.secl_default;

    if ((change.options & DM_SM_INIT_CONFIG) != 0)
        SMCB.config = change.config;
    prim->config = SMCB.config;

    if ((change.options & (DM_SM_INIT_MODE3_ENC|DM_SM_INIT_SECURITY_MODE)) != 0)
    {
        SMCB.config = (SMCB.config & ~DM_SM_SEC_MODE_CONFIG_MODE3_ENC_MASK)
                | change.mode3_enc;
    }
    prim->mode3_enc = (SMCB.config & DM_SM_SEC_MODE_CONFIG_MODE3_ENC_MASK);

    if ((change.options & DM_SM_INIT_WRITE_AUTH_ENABLE) != 0)
        SMCB.write_auth_enable = change.write_auth_enable;
    prim->write_auth_enable = SMCB.write_auth_enable;

    prim->sm_key_state = change.sm_key_state;
    prim->sm_div_state = change.sm_div_state;

    send_dm_sm_common((DM_UPRIM_T *)prim, NULL);

    CsrPmemFree(SMCB.change);
    SMCB.change = NULL;
}

/*! \brief Send a link key negative response to the Host Controller.

    \param  p_uprim Pointer to primitive.
*/
void dm_sm_handle_key_request_neg_rsp(DM_UPRIM_T *const p_uprim)
{
    TYPED_BD_ADDR_T *addrt = &p_uprim->dm_sm_key_request_neg_rsp.addrt;

#ifdef INSTALL_SM_MODULE
    DM_ACL_T *acl;

    if ((acl = dm_acl_find_by_tbdaddr(addrt)) != NULL && dm_acl_is_ble(acl))
        sm_handle_key_request_neg_rsp(p_uprim);
    else
#endif /* INSTALL_SM_MODULE */
        send_hci_link_key_req_neg_reply(&TBDADDR_ADDR(*addrt));
}


/*! \brief Send a link key response to the Host Controller.

    \param  p_uprim Pointer to primitive.
*/
void dm_sm_handle_key_request_rsp(DM_UPRIM_T *const p_uprim)
{
    TYPED_BD_ADDR_T *addrt = &p_uprim->dm_sm_key_request_rsp.addrt;
    CsrUint8 key[SIZE_LINK_KEY];
#if defined(INSTALL_SM_MODULE) || defined(INSTALL_AMP_SUPPORT)
    DM_ACL_T *acl = dm_acl_find_by_tbdaddr(addrt);
#endif

#ifdef INSTALL_SM_MODULE
    if (acl != NULL && dm_acl_is_ble(acl))
        sm_handle_key_request_rsp(p_uprim);
    else
#endif /* INSTALL_SM_MODULE */
    if (p_uprim->dm_sm_key_request_rsp.key_type == DM_SM_KEY_ENC_BREDR)
    {
        CsrBtMemCpyUnpack(key,
                      p_uprim->dm_sm_key_request_rsp.key.enc_bredr->link_key,
                      SIZE_LINK_KEY/2);


        dm_sm_update_device_link_key(
                addrt,
                key,
                (CsrUint8)p_uprim->dm_sm_key_request_rsp.key.enc_bredr->link_key_type);

        if (dm_sm_current_link_key_acceptable(addrt, NULL) == DM_SM_ACCEPTABLE)
        {
#ifdef INSTALL_AMP_SUPPORT
            if (acl != NULL)
            {
                /* Give new link key to AMPM. */
                dm_amp_link_key_notification_ind(
                        acl,
                        p_uprim->dm_sm_key_request_rsp.key.enc_bredr->link_key_type,
                        key,
                        DM_AMP_LINK_KEY_PENDING);
            }
#endif
            send_hci_link_key_req_reply(&TBDADDR_ADDR(*addrt), key);
        }
        else
        {
            send_hci_link_key_req_neg_reply(&TBDADDR_ADDR(*addrt));
        }
    }

    /* Free the key memory */
    if(p_uprim->dm_sm_key_request_rsp.key_type!=DM_SM_KEY_DIV&&
       p_uprim->dm_sm_key_request_rsp.key_type!=DM_SM_KEY_DIV_CSRK)
        CsrPmemFree(p_uprim->dm_sm_key_request_rsp.key.none);
    return;
}



/*! \brief Send a Pin code response to the Host Controller.

    \param  p_uprim Pointer to primitive.
*/
void dm_sm_handle_pin_request_rsp(DM_UPRIM_T *const p_uprim)
{
    if (p_uprim->dm_sm_pin_request_rsp.pin_length)
        send_hci_pin_code_req_reply(
                &TBDADDR_ADDR(p_uprim->dm_sm_pin_request_rsp.addrt),
                p_uprim->dm_sm_pin_request_rsp.pin_length,
                p_uprim->dm_sm_pin_request_rsp.pin);
    else
        send_hci_pin_code_req_neg_reply(
                &TBDADDR_ADDR(p_uprim->dm_sm_pin_request_rsp.addrt));
}

/*! \brief Authorisation response from the application.

    Finds matching request and passes response into state machine.

    \param  p_uprim Pointer to primitive.
*/
void dm_sm_handle_authorise_rsp(DM_UPRIM_T *const p_uprim)
{
    DM_ACL_T *p_acl;
    DM_SM_REQUEST_T *p_req;
    fsm_event_t event = SM_EV_AUTHORISE_FAIL;
    CsrUint16 authorisation = p_uprim->dm_sm_authorise_rsp.authorisation;

    if ((authorisation & (1 << DM_SM_AUTHORISE_ACCEPT_BIT)) != 0)
        event = SM_EV_AUTHORISE_OK;

    /* Get the ACL instance */
    if ((p_acl = dm_acl_find_by_tbdaddr(
                &p_uprim->dm_sm_authorise_rsp.cs.connection.addrt)) != NULL)
    {
        /* Check that the current request matches */
        if ((p_req = p_acl->dm_acl_client_sm.p_requests) != NULL &&
            p_req->u.type == DM_SM_ACCESS_REQ &&
            connection_setup_match(&p_uprim->dm_sm_authorise_rsp.cs,
                &p_req->u.access_req.conn_setup))
        {
            sm_state_machine(p_acl, event, NULL);

            /* We might need to store the answer for the life of the ACL */
            dm_sm_store_authorisation(
                    &p_uprim->dm_sm_authorise_rsp.cs.connection,
                    authorisation);
        }
    }
}

/*! \brief A request from the application to authenticate the link.

    This is largely just for legacy applications. Bluetooth 2.1 applications
    should not need to use it. There is now a primitive for dedicated
    bonding - DM_SM_BONDING_REQ - and pairing for general bonding or
    non-bonding will when the protocol manager issues DM_SM_ACCESS_REQ.

    \param p_uprim Pointer to primitive.
*/
void dm_sm_handle_authenticate_req(DM_UPRIM_T *const p_uprim)
{
    DM_ACL_T *p_acl;
    DM_SM_REQUEST_T *p_req;

    /* Get instance and add request to list */
    if ((p_acl = dm_acl_find_connected_by_bd_addr(
                    &p_uprim->dm_sm_authenticate_req.bd_addr)) != NULL
            && (p_req = add_sm_request(p_acl)) != NULL)
    {
        CsrMemCpy(&p_req->u.auth_req, &p_uprim->dm_sm_authenticate_req,
                sizeof(DM_SM_AUTHENTICATE_REQ_T));

        /* Kick the request processing */
        kick_request_process(p_acl);
    }
    else
    {
        TYPED_BD_ADDR_T addrt;

        tbdaddr_copy_from_public_bd_addr(
                &addrt,
                &p_uprim->dm_sm_authenticate_req.bd_addr);

        /* The Security Manager could not accept the request */
        send_dm_sm_authenticate_cfm(&addrt, FALSE);
    }
}

/*! \brief A request from the application to set encryption on the link.

    This is largely just for legacy applications. Bluetooth 2.1 applications
    should not need to use it. In 2.1, encryption is always enabled after
    authentication or pairing.

    \param p_uprim Pointer to primitive.
*/
void dm_sm_handle_encrypt_req(DM_UPRIM_T *const p_uprim)
{
    DM_ACL_T *p_acl;
    DM_SM_REQUEST_T *p_req;
    CsrBool encrypted = FALSE;
    TYPED_BD_ADDR_T addrt;

    /* Get instance */
    if ((p_acl = dm_acl_find_connected_by_bd_addr(
                    &p_uprim->dm_sm_encrypt_req.bd_addr)) != NULL)
    {
        /* Add request to list for ACL */
        if ((p_req = add_sm_request(p_acl)) != NULL)
        {
            CsrMemCpy(&p_req->u.enc_req, &p_uprim->dm_sm_encrypt_req,
                    sizeof(DM_SM_ENCRYPT_REQ_T));

            /* Kick the request processing */
            kick_request_process(p_acl);
            return;
        }

        /* Set current encryption state in failure message */
        encrypted = p_acl->dm_acl_client_sm.encrypted;
    }

    /* Inform the app of failure, including the enc status where possible */
    tbdaddr_copy_from_public_bd_addr(&addrt,
                                     &p_uprim->dm_sm_encrypt_req.bd_addr);
    send_dm_sm_encrypt_cfm(&addrt, FALSE, encrypted);
}

/*! \brief Application has responded with its IO Capabilities.

    During Secure Simple Pairing (SSP), the devices swap IO capabilities
    information, together with authentication requirements and availability
    of Out Of Band (OOB) data. The host controllers will use this information
    to decide which association model to use for authentication.

    The security manager stores the information so that it knows whether or
    not to send a user confirmation request to the application (or to ignore
    the application and do 'just works').

    Unlike the HCI interface, we get any OOB data from the application at
    the same time, so that we don't need to ask again later.

    \param p_uprim Pointer to primitive.
*/
void dm_sm_handle_io_capability_request_rsp( DM_UPRIM_T *const p_uprim )
{
    DM_ACL_T *p_acl;
    DM_SM_SSP_T *p_sm_ssp;

    if ((p_acl = dm_acl_find_by_tbdaddr(
                    &p_uprim->dm_sm_io_capability_request_rsp.addrt)) == NULL)
        return;

    if (dm_acl_is_ble(p_acl))
    {
        sm_handle_io_capability_request_rsp(p_uprim);
        return;
    }

    if ((p_sm_ssp = p_acl->dm_acl_client_sm.u.p_ssp) != NULL)
    {
        CsrUint8 mitm_local =
           p_uprim->dm_sm_io_capability_request_rsp.authentication_requirements;

        p_sm_ssp->io_cap_local =
            p_uprim->dm_sm_io_capability_request_rsp.io_capability;

        /* Authentication requirements are decided differently in the case
           of bonding. We also allow the application to insist
           on Man-In-The-Middle (MITM) protection and to request general
           bonding if we're not already doing dedicated bonding. */
        if (p_acl->dm_acl_client_sm.state == SM_ST_BONDING ||
                (p_sm_ssp->mitm_remote & DM_SM_AUTH_REQ_DEDICATED_BONDING) != 0)
        {
            /* Dedicated Bonding */
            p_sm_ssp->mitm_local = (mitm_local & DM_SM_AUTH_REQ_MITM)
                | bonding_auth_req();
        }
        else if ((mitm_local & DM_SM_AUTH_REQ_GENERAL_BONDING) != 0 &&
                ((p_sm_ssp->mitm_remote & DM_SM_AUTH_REQ_GENERAL_BONDING) != 0
                    || p_acl->dm_acl_client_sm.state
                        == SM_ST_ACCESS_AUTHENTICATING))
        {
            /* General Bonding */
            p_sm_ssp->mitm_local = (mitm_local | bonding_auth_req())
                & ~DM_SM_AUTH_REQ_DEDICATED_BONDING;
        }
        else
        {
            /* No Bonding */
            p_sm_ssp->mitm_local =
                (mitm_local & ~DM_SM_AUTH_REQ_DEDICATED_BONDING)
                | auth_key_required(&p_acl->addrt);
        }



        if (p_uprim->dm_sm_io_capability_request_rsp.oob_data_present == 1)
        {
            p_sm_ssp->oob_hash_c =
                p_uprim->dm_sm_io_capability_request_rsp.oob_hash_c;
            p_sm_ssp->oob_rand_r =
                p_uprim->dm_sm_io_capability_request_rsp.oob_rand_r;

            send_hci_io_capability_response(&p_acl->addrt, p_sm_ssp);

            /* Return early to avoid freeing OOB data. */
            return;
        }

        send_hci_io_capability_response(&p_acl->addrt, p_sm_ssp);
    }

    CsrPmemFree(p_uprim->dm_sm_io_capability_request_rsp.oob_hash_c);
    CsrPmemFree(p_uprim->dm_sm_io_capability_request_rsp.oob_rand_r);
}

/*! \brief Application has rejected the IO capabilities request

    \param  p_uprim Pointer to primitive.
*/
void dm_sm_handle_io_capability_request_neg_rsp(DM_UPRIM_T *const p_uprim)
{
#ifdef INSTALL_SM_MODULE
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_tbdaddr(
                    &p_uprim->dm_sm_io_capability_request_neg_rsp.addrt)) == NULL)
        return;

    if (dm_acl_is_ble(p_acl))
    {
        sm_handle_io_capability_request_neg_rsp(p_uprim);
    }
    else
#endif
    {
        send_hci_io_capability_neg_response(
                &TBDADDR_ADDR(p_uprim->dm_sm_io_capability_request_neg_rsp.addrt),
                p_uprim->dm_sm_io_capability_request_neg_rsp.reason);
    }
}

/*! \brief Request from the application to do dedicated bonding with a device.

    In dedicated bonding, we first delete the link key. Then we bring up
    the ACL, if it is not already up. Then we pair to get a link key. Then
    we drop the ACL, if we're the only owner. We end up with a link key
    that we can use for future connections.

    If the remote device is legacy then, depending on configuration, we
    might try to pair during link setup. In addition, if an ACL already
    exists then we might try to drop it first so that we can then bring
    it back up again and pair during link setup. This is an attempt to
    avoid legacy timeout issues. The relevant configuration options are
    set with DM_SM_SEC_MODE_CONFIG_REQ. See dm_prim.h for more details.

    \param p_uprim Pointer to primitive.
*/
void dm_sm_handle_bonding_req( DM_UPRIM_T *const p_uprim )
{
    DM_ACL_T *p_acl;
    TYPED_BD_ADDR_T *addrt = &p_uprim->dm_sm_bonding_req.addrt;

    /* Delete link keys from Bluestack and the LM */
    dm_sm_update_device_link_key(addrt, NULL, DM_SM_LINK_KEY_NONE);
#ifndef DISABLE_DM_BREDR
    Send_HCI_Delete_Stored_Link_Key(&addrt->addr,
                                    DM_HCI_SENDER_SECURITY_MANAGER);
#endif

    /* Bring up the ACL */
    if (dm_sm_acl_open(addrt, FALSE, &p_acl) != DM_ACL_FAILED)
    {
        DM_SM_REQUEST_T *p_req;

        if ((p_req = add_sm_request(p_acl)) != NULL)
        {
            /* Pairing must occur for successful bonding. So set a failing
               default status here and only change it if pairing occurs. */
            p_req->bonding_status = DM_SM_BONDING_NO_PAIRING;
            /* Add request to list */
            CsrMemCpy(&p_req->u.bond_req, p_uprim, sizeof(DM_SM_BONDING_REQ_T));
            kick_request_process(p_acl);
            return;
        }

        dm_sm_acl_close(p_acl,
                        HCI_ERROR_OETC_USER,
                        ACL_IDLE_TIMEOUT_AT_MOST_NONE);
    }

    send_dm_sm_bonding_cfm(addrt, HCI_ERROR_MEMORY_FULL);
}

/*! \brief Attempt to cancel a previous bonding request.

    The application has decided that it doesn't want to bond anymore, so
    we do our best to cancel. If we haven't started bonding yet then we
    just remove the request from the list. Otherwise we change state
    immediately and tell the ACL manager that we don't need the ACL anymore.

    If the 'force' option has been used then we destroy the ACL, whether
    or not anyone else is using it.

    \param p_uprim Pointer to primitive.
*/
void dm_sm_handle_bonding_cancel_req( DM_UPRIM_T *const p_uprim )
{
    DM_ACL_T *p_acl;
    DM_SM_REQUEST_T **pp_req, *p_req;

    if ((p_acl = dm_acl_find_by_tbdaddr(
                    &p_uprim->dm_sm_bonding_cancel_req.addrt)) != NULL)
    {
        if (p_acl->dm_acl_client_sm.state != SM_ST_BONDING
                && p_acl->dm_acl_client_sm.state != SM_ST_BONDING_PENDING)
        {
            /* Remove the first queued request */
            for (pp_req = &p_acl->dm_acl_client_sm.p_requests;
                    (p_req = *pp_req) != NULL;
                    pp_req = &p_req->p_next)
            {
                if (p_req->u.type == DM_SM_BONDING_REQ)
                {
                    dm_sm_free_request(pp_req);
                    dm_acl_close(p_acl,
                                (p_uprim->dm_sm_bonding_cancel_req.flags & DM_ACL_FLAG_FORCE) == 0
                                ? dm_acl_client_sm : dm_acl_client_max,
                                HCI_ERROR_OETC_USER,
                                ACL_IDLE_TIMEOUT_AT_MOST_NONE);

                    send_dm_sm_bonding_cfm(
                            &p_uprim->dm_sm_bonding_cancel_req.addrt,
                            DM_SM_BONDING_CANCELLED);
                    break;
                }
            }

            return;
        }

        p_acl->dm_acl_client_sm.p_requests->u.type = DM_SM_BONDING_CANCEL_REQ;
        sm_state_machine(p_acl, SM_EV_CANCEL_BONDING, NULL);

        /* We're in the middle of a bonding request. Try to close the ACL. We
           unlock the ACL an extra time just incase we're in the middle of SSP,
           which locks the ACL a second time. If we're not then this won't
           cause any harm. */
        dm_acl_unlock(p_acl, dm_acl_client_sm);
        dm_acl_close(
            p_acl,
            (p_uprim->dm_sm_bonding_cancel_req.flags & DM_ACL_FLAG_FORCE) == 0
                    ? dm_acl_client_sm : dm_acl_client_max,
            HCI_ERROR_OETC_USER,
            ACL_IDLE_TIMEOUT_AT_MOST_NONE);
    }
}

/*! \brief Public functions for protocol layer requests */

static dm_security_level_t dm_sm_security_level(
        const TYPED_BD_ADDR_T *addrt,
        const DM_SM_SERVICE_T *service,
        CsrBool incoming)
{
    dm_security_level_t seclevel = SECL_NONE;
    DM_SM_SERVICE_RECORD_T *p_rec;

    /* Get security level */
    switch(SMCB.security_mode)
    {
        case SEC_MODE2_SERVICE:
        case SEC_MODE3_LINK:
        case SEC_MODE4_SSP:
            /* Service-based security requirements */
            p_rec = *find_service(service, incoming ? NULL : &TBDADDR_ADDR(*addrt));

            seclevel =
                (p_rec != NULL ? p_rec->security_level : SMCB.secl_default) &
                (incoming ? SECL_IN : SECL_OUT);

            /* If we're not in mode 4 then we clear all mode 4 settings */
            if (SMCB.security_mode != SEC_MODE4_SSP)
                seclevel &= SECL_MODE_LEGACY;
            break;

        case SEC_MODE2a_DEVICE:
             /* MODE2a: Trusted devices do not require authentication,
                all other devices must authenticate. */
            if (dm_sm_trust(addrt, NULL) != DM_SM_ACCEPTABLE)
                seclevel = SECL_IN_AUTHENTICATION | SECL_OUT_AUTHENTICATION;
            break;
    }

    return seclevel;
}

/*! \brief An access request from a protocol layer

    This is called whenever a protocol layer, such as L2CAP or RFCOMM, is
    about to request a connection to a remote device or accept a connection
    from a remote device. The security manager will go through the usual
    steps of Authorisation -> Authentication -> Encryption and then send a
    message back to the protocol layer to say whether it is safe to proceed
    or if the connection should be rejected/abandoned.

    \param Pointer to primitive.
*/
void dm_sm_handle_access_req(DM_UPRIM_T *const p_uprim)
{
    hci_return_t status = HCI_ERROR_NO_CONNECTION;
    dm_security_level_t seclevel;

    seclevel = dm_sm_security_level(
            &p_uprim->dm_sm_access_req.conn_setup.connection.addrt,
            &p_uprim->dm_sm_access_req.conn_setup.connection.service,
            p_uprim->dm_sm_access_req.conn_setup.incoming);

    /* When RFCOMM is tunnelling its security requirements through L2CAP,
       it will only want to implement the SSP side of it. It won't want
       to do legacy or authorisation. See B-98570. */
    if ((p_uprim->dm_sm_access_req.conn_setup.connection.service.protocol_id
                & SEC_PROTOCOL_MODIFY_SSP_ONLY) != 0)
    {
        seclevel &= (SECL4_SSP | SECL4_MITM);
    }

    /* If security required then add to queue, otherwise reply now */
    if (seclevel != SECL_NONE)
    {
        DM_ACL_T *p_acl;

        status = HCI_ERROR_NO_CONNECTION;

        /* Security required, so add to queue. */
        if ((p_acl = dm_acl_find_by_tbdaddr(
                &p_uprim->dm_sm_access_req.conn_setup.connection.addrt))
                        != NULL)
        {
            DM_SM_REQUEST_T *p_req;

            if ((p_req = add_sm_request(p_acl)) != NULL)
            {
                CsrMemCpy(&p_req->u.access_req, p_uprim,
                        sizeof(DM_SM_ACCESS_REQ_T));
                p_req->seclevel = seclevel;

                /* Kick the request processing */
                kick_request_process(p_acl);
                return;
            }
            status = HCI_ERROR_MEMORY_FULL;
        }
    }
    else
        status = HCI_SUCCESS;

    /* Either no security required or we can't accept the request. */
    send_dm_sm_access_cfm(&p_uprim->dm_sm_access_req, status);
}

/*! \brief Work out whether to handle a command in the BLE or BREDR SM.
    If it's the BLE SM then we free the HCI prim as we won't be needing
    it. */
#ifdef INSTALL_SM_MODULE
static CsrBool dm_sm_handle_in_ble_sm(const TYPED_BD_ADDR_T *addrt, HCI_UPRIM_T **hci_prim)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_tbdaddr(addrt)) != NULL && dm_acl_is_ble(p_acl))
    {
        /* Don't need HCI prim any more. */
        CsrPmemFree(*hci_prim);
        hci_prim = NULL;
        return TRUE;
    }

    return FALSE;
}
#endif

/*! \brief Handle Confirmation/Passkey responses by translating type
           to op_code. Note that this will have to change when the
           BLE Security Manager is supported.

    \param p_uprim Pointer to primitive cast to (DM_UPRIM_T*).
*/
void dm_sm_convert_to_hci(DM_UPRIM_T *const p_uprim)
{
    HCI_UPRIM_T *hci_prim =
            (HCI_UPRIM_T*)zpnew(HCI_USER_PASSKEY_REQUEST_REPLY_T);

    /* Convert from DM type to HCI op_code. Note that User Passkey
       Request Negative Reply case is missing because it is being
       used as the default. */
    switch (p_uprim->type)
    {
        case DM_SM_USER_CONFIRMATION_REQUEST_RSP:
#ifdef INSTALL_SM_MODULE
            if (dm_sm_handle_in_ble_sm(&p_uprim->dm_sm_user_confirmation_request_rsp.addrt, &hci_prim))
            {
                sm_handle_user_confirmation_request_rsp(p_uprim);
                return;
            }
#endif
            hci_prim->op_code = HCI_USER_CONFIRMATION_REQUEST_REPLY;
            break;

        case DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP:
#ifdef INSTALL_SM_MODULE
            if (dm_sm_handle_in_ble_sm(&p_uprim->dm_sm_user_confirmation_request_neg_rsp.addrt, &hci_prim))
            {
                sm_handle_user_confirmation_request_neg_rsp(p_uprim);
                return;
            }
#endif
            hci_prim->op_code = HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY;
            break;

        case DM_SM_USER_PASSKEY_REQUEST_NEG_RSP:
#ifdef INSTALL_SM_MODULE
            if (dm_sm_handle_in_ble_sm(&p_uprim->dm_sm_user_passkey_request_neg_rsp.addrt, &hci_prim))
            {
                sm_handle_passkey_neg_response(p_uprim);
                return;
            }
#endif
            hci_prim->op_code = HCI_USER_PASSKEY_REQUEST_NEG_REPLY;
            break;

        case DM_SM_USER_PASSKEY_REQUEST_RSP:
            hci_prim->op_code = HCI_USER_PASSKEY_REQUEST_REPLY;
#ifdef INSTALL_SM_MODULE
            if (dm_sm_handle_in_ble_sm(&p_uprim->dm_sm_user_passkey_request_rsp.addrt, &hci_prim))
            {
                sm_handle_passkey_response(p_uprim);
                return;
            }
#endif
            hci_prim->hci_user_passkey_request_reply.numeric_value
                    = p_uprim->dm_sm_user_passkey_request_rsp.numeric_value;
            break;
    }

    hci_prim->hci_cmd.length = DM_HCI_SENDER_APPLICATION;
    tbdaddr_copy_to_bd_addr(&hci_prim->hci_cmd_with_bd_addr.bd_addr,
                            &p_uprim->dm_common_rsp.addrt);
    send_to_hci((DM_UPRIM_T*)hci_prim);
}

/*! \brief The function is designed to check whether the remote supported
       features and remote extended/host features are known to the DM
       by the time this fucntion is called.

    \param p_acl Pointer to ACL record.
    \return fsm_event_t type.Tells whether SM is ready to kick off
*/
static fsm_event_t dm_sm_remote_features_known(DM_ACL_T *p_acl)
{
    if(dm_acl_is_connected(p_acl) &&
       ( (p_acl->dm_acl_client_sm.flags & DM_SM_FLAG_RSF_RHSF_KNOWN)
                == DM_SM_FLAG_RSF_RHSF_KNOWN))
    {
        return SM_EV_ACL_READY;
    }

    return FSM_EVENT_NULL;
}
/*! \brief The Security manager keeps track of the remote controller's support
           for Secure Simple Pairing. This function is called whenever the
           corresponding HCI event is received.

    \param p_acl Pointer to ACL record.
    \param ssp_suppport. Non-zero indicates SSP support. Zero indicates no SSP support.
*/

void dm_sm_remote_ssp_controller_support(DM_ACL_T *p_acl, CsrUint16 ssp_support)
{
    CsrUint16 mask = DM_SM_FLAG_RSF_KNOWN;
    fsm_event_t fsm_event = FSM_EVENT_NULL;
    CsrUint8 status = HCI_SUCCESS;

    if (ssp_support != 0)
        mask = (DM_SM_FLAG_RSF_KNOWN | DM_SM_FLAG_RSF_SSP);

    p_acl->dm_acl_client_sm.flags |= mask;

    /* Kick the SM state machine in the event of recieving the
      * remote supported features and remote host supported features.
    */
    fsm_event = dm_sm_remote_features_known(p_acl);
    sm_state_machine(p_acl, fsm_event, &status);

    dm_amp_remote_features(&p_acl->addrt, p_acl->dm_acl_client_sm.flags);
}

/*! \brief Public functions for upstream HCI events */

/*! \brief The Security manager keeps track of the remote host's support
           for Secure Simple Pairing and Security Mode 4. This function is called
           whenever the corresponding HCI event is received.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param status Indication of success or failure.
    \param host_features Remote host supported features.
*/
void dm_sm_hci_remote_host_supported_features(
        const hci_connection_handle_t handle,
        CsrUint8 status,
        const CsrUint16 host_features)
{
    DM_ACL_T *p_acl;
    CsrUint16 mask = DM_SM_FLAG_RHSF_KNOWN;
    fsm_event_t fsm_event = FSM_EVENT_NULL;

    if ((p_acl = dm_acl_find_by_handle(handle)) != NULL)
    {
        /* We don't need to clear the DM_ACL_RHSF_SSP bit because devices
         * are not allowed to leave SM4 once they have entered it. Once
         * the bit has been set, it never needs to be cleared. */
        if (status == HCI_SUCCESS && (host_features &
                    (1U << LMP_EXT_FEATURE_SSP_HOST_SUPPORT_BIT)) != 0)
            mask = (DM_SM_FLAG_RHSF_KNOWN | DM_SM_FLAG_RHSF_SSP);

        p_acl->dm_acl_client_sm.flags |= mask;

        /* Kick the SM state machine in the event of recieving the
          * remote supported features and remote host supported features.
        */
        fsm_event = dm_sm_remote_features_known(p_acl);
        sm_state_machine(p_acl, fsm_event, &status);

        dm_amp_remote_features(&p_acl->addrt, p_acl->dm_acl_client_sm.flags);
    }
}


/*! \brief Handle a (legacy) PIN code request from the Host Controller.

    If we are configured for backwards compatibility then the
    request is issued direct to the application. Otherwise we
    reply to HCI in the negative.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
void dm_sm_hci_pin_code_request(const BD_ADDR_T *const p_bd_addr)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_bd_addr(p_bd_addr)) != NULL)
    {
        /* Clear any note that we're dealing with a link key request.
           This is to allow us to make sure that any 'KEY_MISSING'
           error really is for a link key and not a rejected PIN code
           request (which can also produce that error). */
        p_acl->dm_acl_client_sm.flags &= ~DM_SM_FLAG_KEY_PENDING;

        if (p_acl->dm_acl_client_sm.state != SM_ST_BONDING_CANCEL
                && legacy_security_ok(&p_acl->addrt))
        {
            /* If this is initiated by the remote device then extend
               any idle timeout that might be running. */
            if (p_acl->dm_acl_client_sm.state == SM_ST_IDLE)
                dm_sm_acl_set_timeout(p_acl);

            send_dm_sm_pin_request_ind(&p_acl->addrt);
            return;
        }
    }

    send_hci_pin_code_req_neg_reply(p_bd_addr);
}

/*! \brief Handle a link key request from the Host Controller.

    These will always occur if we initiate authentication and will also
    occur if the remote device has initiated authentication and has
    a link key that it wishes to authenticate.

    We automatically reject the request during bonding.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
void dm_sm_hci_link_key_request(const BD_ADDR_T *const p_bd_addr)
{
    DM_ACL_T *p_acl;
    TYPED_BD_ADDR_T addrt;

    tbdaddr_copy_from_public_bd_addr(&addrt, p_bd_addr);

    /* Reject request if we can't find the ACL, or we're bonding, or
       we're re-pairing after a missing link key error. */
    if ((p_acl = dm_acl_find_by_tbdaddr(&addrt)) == NULL
            || p_acl->dm_acl_client_sm.state == SM_ST_BONDING
            || p_acl->dm_acl_client_sm.state == SM_ST_BONDING_PENDING
            || p_acl->dm_acl_client_sm.state == SM_ST_BONDING_CANCEL
            || (p_acl->dm_acl_client_sm.flags & DM_SM_FLAG_KEY_MISSING) != 0)
    {
        send_hci_link_key_req_neg_reply(p_bd_addr);
    }
    else
    {
        CsrUint16 *p_key;

        /* Make a note that we're dealing with a link key request.
           This is to allow us to make sure that any 'KEY_MISSING'
           error really is for a link key and not a rejected PIN code
           request (which can also produce that error). */
        p_acl->dm_acl_client_sm.flags |= DM_SM_FLAG_KEY_PENDING;

        /* If this is initiated by the remote device then extend
           any idle timeout that might be running. */
        if (p_acl->dm_acl_client_sm.state == SM_ST_IDLE)
            dm_sm_acl_set_timeout(p_acl);

        if (dm_sm_current_link_key_acceptable(&addrt, &p_key)
                == DM_SM_ACCEPTABLE)
        {
            CsrUint8 key[SIZE_LINK_KEY];
            CsrBtMemCpyUnpack(key, p_key, SIZE_LINK_KEY/2);

            /* Found link key in device database, so send it now. */
            send_hci_link_key_req_reply(p_bd_addr, key);
        }
        else
        {
            /* No suitable key found, so issue request to the application. */
            send_dm_sm_key_request_ind(&addrt);
        }
    }
}

/*! \brief Handle a link key notification event from the Host Controller.

    If the remote device already has an entry in the device database,
    that entry is updated with the new link key. The link key is always
    forwarded to the application for non-volatile storage.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param key_type The type of link key generated.
    \param key Link key in CsrUint8 array of size SIZE_LINK_KEY.
*/
void dm_sm_hci_link_key_notification(const BD_ADDR_T *const p_bd_addr,
                                     CsrUint8 key_type,
                                     const CsrUint8 key[SIZE_LINK_KEY])
{
    TYPED_BD_ADDR_T addrt;
    DM_ACL_T *p_acl;

    tbdaddr_copy_from_public_bd_addr(&addrt, p_bd_addr);

    /* Store link key in device database */
    dm_sm_update_device_link_key(&addrt, key, key_type);

    /* Get the ACL instance */
    if ((p_acl = dm_acl_find_by_tbdaddr(&addrt)) != NULL)
    {
        DM_SM_REQUEST_T *p_req;

        /* Link key indication usually implies we've paired. */
        if (key_type != HCI_CHANGED_COMBINATION_KEY
                && (p_req = p_acl->dm_acl_client_sm.p_requests) != NULL)
            p_req->bonding_status = (hci_error_t)HCI_SUCCESS;

        /* We have a new key, so reset the 'missing' flag. */
        p_acl->dm_acl_client_sm.flags &= ~DM_SM_FLAG_KEY_MISSING;

        /* Generation of link key implies simple pairing complete */
        dm_sm_simple_pairing_complete(p_acl, HCI_SUCCESS);

        /* If the link is already encrypted and we're both in mode 4 then make
           a note that we need an encryption key refresh at some point. */
        if (p_acl->dm_acl_client_sm.encrypted && !dm_sm_is_legacy(p_acl))
            p_acl->dm_acl_client_sm.flags |= DM_SM_FLAG_NEED_ENCRYPTION_REFRESH;

        /* Issue the link key to the AMPM, if it exists. */
#ifdef INSTALL_AMP_SUPPORT
        dm_amp_link_key_notification_ind(p_acl, key_type, key,
                DM_AMP_LINK_KEY_ACTIVE);
#endif
    }

    /* The database might have a more useful
       idea of link key type (in the case of a changed link key). */
    key_type = dm_sm_get_device_link_key(&addrt, NULL);

    /* Issue link key to application */
    send_dm_sm_keys_ind(&addrt, key_type, key);
}

/*! \brief Notification of successful completion of authentication.

    We do a final check to make sure that the link key generated is
    acceptable and then inform the SM state machine for the link.

    \param handle HCI connection handle.
*/
void dm_sm_hci_auth_success(const hci_connection_handle_t handle)
{
    DM_ACL_T *p_acl;
    DM_SM_REQUEST_T *p_req;
    fsm_event_t event = FSM_EVENT_NULL;

    /* Get the ACL instance */
    if ((p_acl = dm_acl_find_by_handle(handle)) != NULL)
    {
        /* Successful authentication, so reset transaction_retries */
        if ((p_req = p_acl->dm_acl_client_sm.p_requests) != NULL)
            p_req->transaction_retries = 0;

        /* It is possible to get an unauthenticated key even when we've
           asked for an authenticated one. We do a final check for link
           key acceptability here. It's also possible that we have no
           link key at all and the controller has authenticated a
           legacy key that it already had. In that case we know
           that the link key must be a legacy combination key, which
           is always acceptable. We don't prod the AMP code in that
           case because AMP doesn't work with legacy keys.
        */
        switch (dm_sm_current_link_key_acceptable(&p_acl->addrt, NULL))
        {
            case DM_SM_ACCEPTABLE:
#ifdef INSTALL_AMP_SUPPORT
                dm_amp_link_key_active_ind(p_acl, HCI_SUCCESS);
#endif
            /* fall through */
            case DM_SM_NOT_FOUND:
                event = SM_EV_HCI_AUTH_OK;
                break;
            default:
                event = SM_EV_HCI_AUTH_FAIL;
                break;
        }

        /* Send an auth ok/fail event into the state machine */
        sm_state_machine(p_acl, event, NULL);
    }
}

/*! \brief Notification of unsuccessful completion of authentication.

    Inform the SM state machine for the link of the failure.

    \param handle HCI connection handle.
    \param status Indication of what went wrong.
*/
void dm_sm_hci_auth_failure(const hci_connection_handle_t handle,
                            hci_return_t status)
{
    DM_ACL_T *p_acl;
    DM_SM_REQUEST_T *p_req;
    fsm_event_t delayed_event;
    fsm_event_t event = SM_EV_HCI_AUTH_FAIL;

    /* Get the ACL instance */
    if ((p_acl = dm_acl_find_by_handle(handle)) != NULL)
    {
        if ((p_req = p_acl->dm_acl_client_sm.p_requests) != NULL)
        {
            /* See if we need to retry authentication.

               If we do then we start a timer to avoid HCI_ERROR_AUTH_REPEATED
               and then retry authentication. Otherwise, we just send the
               auth failure event straight away, but we still start the timer
               to cause a holdup before this request is released. The queue
               is stalled to prevent any subsequent requests from triggering
               HCI_ERROR_AUTH_REPEATED. */
            delayed_event = dm_sm_retry_authentication_event(p_acl, &status);

            if (delayed_event != FSM_EVENT_NULL)
                event = FSM_EVENT_NULL;

            timer_start(&p_req->transaction_holdoff_timer,
                        DM_CONFIG_SM_REAUTHENTICATION_TIMEOUT,
                        dm_sm_retry_transaction,
                        (CsrUint16)delayed_event,
                        p_acl);
        }

        /* Clear link keys and kick state machine */
        dm_sm_update_device_link_key(&p_acl->addrt,
                                     NULL,
                                     DM_SM_LINK_KEY_NONE);

        sm_state_machine(p_acl, event, &status);
    }
}

/*! \brief Notification of completion of encryption.

    Inform the SM state machine for the link. If status != HCI_SUCCESS
    then the 'encrypted' argument is ignored.

    \param handle HCI connection handle.
    \param status Indication of success or failure.
    \param encrypted Whether or not the link is encrypted.
*/
void dm_sm_hci_enc_complete(const hci_connection_handle_t handle,
    CsrUint8 status, const CsrBool encrypted)
{
    DM_ACL_T *p_acl;
    DM_SM_REQUEST_T *p_req;
    fsm_event_t event = SM_EV_HCI_ENC_CHANGE_FAIL;

    /* Get the ACL instance */
    if ((p_acl = dm_acl_find_by_handle(handle)) != NULL)
    {
#ifdef BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE
        p_acl->dm_acl_client_sm.encrypting= FALSE;
#endif /* BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE */

        switch (status)
        {
            case HCI_SUCCESS:
                /* Store the ACL encryption status */
                p_acl->dm_acl_client_sm.flags &= ~DM_SM_FLAG_NEED_ENCRYPTION_REFRESH;
                p_acl->dm_acl_client_sm.encrypted = FALSE;

                if (encrypted)
                {
                    p_acl->dm_acl_client_sm.encrypted = TRUE;
                    /* Encryption means that we don't need huge timeouts */
                    dm_acl_idle_timeout_change(
                            p_acl,
                            ACL_IDLE_TIMEOUT_AT_MOST_SHORT,
                            HCI_ERROR_OETC_USER);
                }
                /* If we're in the ENCRYPTING state then the state machine
                   will notify the app. So we only have to do it ourselves
                   when we're not in the ENCRYPTING state. */
                if (p_acl->dm_acl_client_sm.state != SM_ST_ENCRYPTING)
                    send_dm_sm_encryption_change(&p_acl->addrt, encrypted);

                event = SM_EV_HCI_ENC_CHANGE_OK;
                break;

            case HCI_ERROR_LMP_TRANSACTION_COLLISION:
                if ((p_req = p_acl->dm_acl_client_sm.p_requests) != NULL &&
                        ++p_req->transaction_retries < DM_SM_ENC_RETRIES_MAX)
                {
                    /* Delay re-pairing to avoid HCI_ERROR_AUTH_REPEATED. */
                    timer_start(&p_req->transaction_holdoff_timer,
                                DM_CONFIG_SM_REENCRYPTION_TIMEOUT,
                                dm_sm_retry_transaction,
                                (CsrUint16)SM_EV_RETRY_ENCRYPTION,
                                p_acl);
                    event = FSM_EVENT_NULL;
                }
                break;
        }

        /* Send an encryption change event into the state machine */
        sm_state_machine(p_acl, event, &status);

        if (dm_acl_is_ble(p_acl))
            sm_encryption_changed(handle, encrypted, status);
    }
}

#ifndef DISABLE_DM_BREDR
/*! \brief Notification that authentication at link setup has been enabled.

    We see if the encryption mode needs to be changed.

    \param status Indication of success or failure.
*/
CsrBool dm_sm_hci_auth_enable(const hci_return_t status)
{
    CsrUint16 failure;

    /* If we're not in the middle of a mode change then this event is
       for some other entity. Return FALSE so caller will try elsewhere. */
    if (SMCB.change == NULL)
        return FALSE;

    failure = 0;
    if (status != HCI_SUCCESS)
    {
        SMCB.change->security_mode = SMCB.security_mode;
        failure = DM_SM_INIT_SHIFT(DM_SM_INIT_FAIL_WRITE_AUTHENTICATION_ENABLE);
    }

    dm_sm_parameter_init(failure);

    return TRUE;
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Notification of change of link setup encryption mode.

    Only change encryption mode on success, but always send
    successful confirmation of security mode because that
    has already been changed.

    \param status Indication of success or failure.
*/
void dm_sm_hci_write_enc_mode_complete(const hci_return_t status)
{
    if (SMCB.change != NULL)
    {
        CsrUint16 failure = 0;

        if (status != HCI_SUCCESS)
        {
            SMCB.change->mode3_enc =
                (CsrUint8)(SMCB.config & DM_SM_INIT_ENCRYPTION);
            failure = DM_SM_INIT_SHIFT(DM_SM_INIT_FAIL_WRITE_ENCRYPTION_MODE);
        }

        dm_sm_parameter_init(failure);
    }
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Request from HCI for our IO capability.

    In addition to IO capability, we must also get information
    about OOB data and authentication requirements. We may have to
    ask the application.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
void dm_sm_hci_io_capability_request(const BD_ADDR_T * const p_bd_addr)
{
    DM_ACL_T *p_acl;
    TYPED_BD_ADDR_T addrt;

    tbdaddr_copy_from_public_bd_addr(&addrt, p_bd_addr);

    if (dm_sm_acl_open(&addrt, TRUE, &p_acl) != DM_ACL_FAILED
            && p_acl->dm_acl_client_sm.state != SM_ST_BONDING_CANCEL)
    {
        /* Clear any note that we're dealing with a link key request.
           This is to allow us to make sure that any 'KEY_MISSING'
           error really is for a link key and not a rejected PIN code
           request (which can also produce that error). */
        p_acl->dm_acl_client_sm.flags &= ~DM_SM_FLAG_KEY_PENDING;

        if (p_acl->dm_acl_client_sm.u.p_ssp == NULL)
            p_acl->dm_acl_client_sm.u.p_ssp = zpnew(DM_SM_SSP_T);

        send_dm_sm_io_capability_request_ind(&addrt);
        return;
    }

    send_hci_io_capability_neg_response(p_bd_addr,
            HCI_ERROR_PAIRING_NOT_ALLOWED);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Notification of remote device's IO capabilities.

    As part of SSP, we are given the IO capabilities of the remote device.
    We forward these to the application and make a note of them for later.

    \param p_bd_addr Pointer to Bluetooth address of remote device
    \param io_cap_remote IO capability of remote device.
    \param oob_data_present Whether or not the remote device has OOB data.
    \param authentication_requirements Authentication requirements of
           remote device.
*/
void dm_sm_hci_io_capability_response(const BD_ADDR_T *const p_bd_addr,
                                      const CsrUint8 io_cap_remote,
                                      const CsrUint8 oob_data_present,
                                      const CsrUint8 authentication_requirements)
{
    DM_SM_IO_CAPABILITY_RESPONSE_IND_T *p_prim;
    DM_ACL_T *p_acl;
    TYPED_BD_ADDR_T addrt;

    tbdaddr_copy_from_public_bd_addr(&addrt, p_bd_addr);

    /* This could be the first we know about Secure Simple Pairing so we
       must create the SSP structure if it is not already there. */
    if ((p_acl = dm_acl_find_by_tbdaddr(&addrt)) != NULL)
    {
        if (p_acl->dm_acl_client_sm.u.p_ssp == NULL)
            p_acl->dm_acl_client_sm.u.p_ssp = zpnew(DM_SM_SSP_T);

        /* Store authentication requirements and IO caps of remote host.
           Note that we use a bitwise OR for authentication_requirements
           because we might already have sneakily set the MITM flag in
           send_hci_io_capability_response() to fool ourselves into
           thinking that the remote device wants MITM protection.
         */
        p_acl->dm_acl_client_sm.u.p_ssp->mitm_remote |= authentication_requirements;
        p_acl->dm_acl_client_sm.u.p_ssp->io_cap_remote = io_cap_remote;

        /* Inform application. */
        p_prim = zpnew(DM_SM_IO_CAPABILITY_RESPONSE_IND_T);
        p_prim->type = DM_SM_IO_CAPABILITY_RESPONSE_IND;
        p_prim->io_capability = io_cap_remote;
        p_prim->oob_data_present = oob_data_present;
        p_prim->authentication_requirements = authentication_requirements;

        send_dm_sm_common((DM_UPRIM_T*)p_prim, &addrt);
    }
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Command complete event after enabling SSP in controller.

    We enabled SSP in the controller after entering mode 4 and
    now we get a command complete event. If the command was not
    successful then we report failure to the application, along
    with the existing security mode settings. If we're successful
    and we were in mode 3 then we must turn off link-level security
    before reporting success to the application.

    \param status Indication of success or failure.
*/
void dm_sm_hci_write_simple_pairing_mode_cc(const CsrUint8 status)
{
    if (SMCB.change != NULL)
    {
        CsrUint16 failure = 0;

        if (status == HCI_SUCCESS)
            dm_amp_simple_pairing_enabled();
        else
        {
            SMCB.change->security_mode = SMCB.security_mode;
            failure =
                DM_SM_INIT_SHIFT(DM_SM_INIT_FAIL_WRITE_SIMPLE_PAIRING_MODE);
        }

        dm_sm_parameter_init(failure);
    }
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Command complete event after changing SSP debug mode in controller.

    After receiving DM_SM_SEC_MODE_CONFIG_REQ, we turned SSP debug mode
    on or off in the controller. Now we can report back to the application.

    \param status Indication of success or failure.
*/
void dm_sm_hci_write_simple_pairing_debug_mode_cc(const CsrUint8 status)
{
    if (SMCB.change != NULL)
    {
        CsrUint16 failure = 0;

        if (status != HCI_SUCCESS)
        {
            SMCB.change->config =
                ((SMCB.change->config & ~DM_SM_SEC_MODE_CONFIG_DEBUG_MODE)
                    | (SMCB.config & DM_SM_SEC_MODE_CONFIG_DEBUG_MODE));
            failure = DM_SM_INIT_SHIFT(
                    DM_SM_INIT_FAIL_WRITE_SIMPLE_PAIRING_DEBUG_MODE);
        }

        dm_sm_parameter_init(failure);
    }
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Command complete event after reading local OOB data.

    Our request for local OOB data has returned across HCI.
    We forward to the application. Part of SSP.

    \param oob_hash_c OOB data hash.
    \param oob_rand_r OOB data randomiser.
    \param status Indication of success or failure.
*/
void dm_sm_hci_read_local_oob_data_cc(const CsrUint8 *const oob_hash_c,
                                        const CsrUint8 *const oob_rand_r, const CsrUint8 status)
{
    DM_SM_READ_LOCAL_OOB_DATA_CFM_T *p_prim;

    p_prim = zpnew(DM_SM_READ_LOCAL_OOB_DATA_CFM_T);
    p_prim->type = DM_SM_READ_LOCAL_OOB_DATA_CFM;
    p_prim->status = status;

    if (status == HCI_SUCCESS)
    {
        p_prim->oob_hash_c = (CsrUint8*)xpcopy(oob_hash_c, SIZE_OOB_DATA);
        p_prim->oob_rand_r = (CsrUint8*)xpcopy(oob_rand_r, SIZE_OOB_DATA);

        if (p_prim->oob_hash_c == NULL || p_prim->oob_rand_r == NULL)
        {
            /* Can't allocate for oob data, so free any memory that was
               allocated and return failure to application */
            CsrPmemFree(p_prim->oob_hash_c);
            CsrPmemFree(p_prim->oob_rand_r);
            p_prim->status = HCI_ERROR_MEMORY_FULL;
        }
    }

    send_dm_sm_common((DM_UPRIM_T*)p_prim, NULL);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief See if MITM protection is a requirement.

    We will insist on MITM protection if we're asking for it,
    unless we're doing bonding and unauthenticated is
    the best we can manage - effectively we bond a subset of
    services (see B-38972) If only the remote device is insisting
    on an authenticated link key then it's up to them to enforce
    it (see B-27160).

    \param auth_local Local authentication requirements.
    \returns TRUE if an unauthenticated link key is acceptable to us.
             Otherwise FALSE.
*/
static CsrBool dm_sm_allow_unauthenticated(CsrUint16 auth_local)
{
    if ((auth_local & DM_SM_AUTH_REQ_MITM) != 0
        && (auth_local &
                (DM_SM_AUTH_REQ_DEDICATED_BONDING |
                 DM_SM_AUTH_REQ_GENERAL_BONDING)) == 0)
        return FALSE;

    return TRUE;
}
#endif

#ifndef DISABLE_DM_BREDR
typedef enum
{
    DM_SM_USER_CONF_SEND_AND_WAIT,
    DM_SM_USER_CONF_SEND_AND_ACCEPT,
    DM_SM_USER_CONF_JUST_ACCEPT,
    DM_SM_USER_CONF_JUST_REJECT
} DM_SM_USER_CONF_POLICY_T;

/*! \brief Determine application involvement in numerical comparison.

    \param io_cap_local Effective local IO capabilities.
    \param io_cap_remote Effective remote IO capabilities.
    \param allow_unauthenticated TRUE if an unauthenticated key is enough.
    \return value from dm_sm_user_conf_policy_t enum.
*/
static DM_SM_USER_CONF_POLICY_T dm_sm_user_conf_decide(
        CsrUint8 io_cap_local,
        CsrUint8 io_cap_remote,
        CsrBool allow_unauthenticated)
{
    /* This is the only way to generate an authenticated key. */
    if (io_cap_local == HCI_IO_CAP_DISPLAY_YES_NO
            && io_cap_remote == HCI_IO_CAP_DISPLAY_YES_NO)
        return DM_SM_USER_CONF_SEND_AND_WAIT;

    /* Authenticated key no longer an option. Can we continue? */
    if (allow_unauthenticated)
    {
        /* We might still have to wait for a response from the application. */
        if (io_cap_local == HCI_IO_CAP_DISPLAY_YES_NO
                || !dm_sm_auto_accept_just_works())
            return DM_SM_USER_CONF_SEND_AND_WAIT;

        /* We can auto-accept, but a message upstream might still be needed. */
        if (io_cap_local == HCI_IO_CAP_DISPLAY_ONLY)
            return DM_SM_USER_CONF_SEND_AND_ACCEPT;

        return DM_SM_USER_CONF_JUST_ACCEPT;
    }

    /* Authenticated key required but impossible to generate. */
    return DM_SM_USER_CONF_JUST_REJECT;
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Request from HCI for user confirmation.

    This event is generated during Simple Secure Pairing after the
    devices have exchanged IO capabilities information and decided
    to use the Numerical Comparison or Just Works association models.

    There are three possible outcomes and three possible primitives to build:
    we can reply immediately and accept the request;
    we can reply immediately and reject the request;
    or we can ask the application.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param numeric_value Pointer to numerical value from 0 to 999999.
*/
void dm_sm_hci_user_confirmation_request(const BD_ADDR_T *const p_bd_addr,
                                         const CsrUint32 *const numeric_value)
{
    DM_ACL_T *p_acl;
    DM_SM_SSP_T *p_sm_ssp;
    TYPED_BD_ADDR_T addrt;

    tbdaddr_copy_from_public_bd_addr(&addrt, p_bd_addr);

    if ((p_acl = dm_acl_find_by_tbdaddr(&addrt)) != NULL
            && p_acl->dm_acl_client_sm.state != SM_ST_BONDING_CANCEL
            && (p_sm_ssp = p_acl->dm_acl_client_sm.u.p_ssp) != NULL)
    {
        CsrUint16 response_required;
        CsrUint8 io_cap_local = HCI_IO_CAP_NO_INPUT_NO_OUTPUT; /* Just works */
        CsrUint8 io_cap_remote = HCI_IO_CAP_NO_INPUT_NO_OUTPUT; /* Just works */

        if ((p_sm_ssp->mitm_local & DM_SM_AUTH_REQ_MITM) != 0
            || (p_sm_ssp->mitm_remote & DM_SM_AUTH_REQ_MITM) != 0)
        {
            /* We are not using 'Just works' so need actual IO capabilities. */
            io_cap_local = p_sm_ssp->io_cap_local;
            io_cap_remote = p_sm_ssp->io_cap_remote;
        }

        response_required = DM_SM_FLAGS_NONE;
        switch (dm_sm_user_conf_decide(
                    io_cap_local,
                    io_cap_remote,
                    dm_sm_allow_unauthenticated(p_sm_ssp->mitm_local)))
        {
            case DM_SM_USER_CONF_SEND_AND_WAIT:
                response_required = DM_SM_FLAGS_RESPONSE_REQUIRED;
                /* fallthrough */

            case DM_SM_USER_CONF_SEND_AND_ACCEPT:
                send_dm_sm_user_confirmation_request_ind(
                        &addrt,
                        numeric_value,
                        io_cap_local,
                        io_cap_remote,
                        response_required);
                /* fallthrough */

            case DM_SM_USER_CONF_JUST_ACCEPT:
                if (response_required == DM_SM_FLAGS_NONE)
                    send_hci_user_confirmation_request_reply(
                            HCI_USER_CONFIRMATION_REQUEST_REPLY, &addrt);
                return;

            case DM_SM_USER_CONF_JUST_REJECT:
                /* We just send negative response before exiting function. */
                break;
        }
    }

    send_hci_user_confirmation_request_reply(
            HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY, &addrt);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Request from HCI for passkey confirmation.

   This event is generated during Simple Secure Pairing after the
   devices have exchanged IO capabilities information and decided
   to use the Passkey entry association model.

   The user must enter a passkey on this device, which will be compared
   with one entered or displayed on the remote device. We don't need to
   worry about automatically accepting or rejecting (as we did with
   the user_confirmation_request above as the passkey event is only
   generated when an authenticated link key is required and possible.

   \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
void dm_sm_hci_user_passkey_request(const BD_ADDR_T *const p_bd_addr)
{
    DM_SM_USER_PASSKEY_REQUEST_IND_T *p_dm_prim;
    HCI_USER_PASSKEY_REQUEST_NEG_REPLY_T *p_hci_prim;
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_bd_addr(p_bd_addr)) != NULL
            && p_acl->dm_acl_client_sm.state != SM_ST_BONDING_CANCEL)
    {
        p_dm_prim = pnew(DM_SM_USER_PASSKEY_REQUEST_IND_T);
        p_dm_prim->type = DM_SM_USER_PASSKEY_REQUEST_IND;
        p_dm_prim->dev_class = p_acl->dev_class;
        p_dm_prim->flags = dm_sm_is_initiator(&p_acl->addrt)
            ? DM_SM_FLAGS_RESPONSE_REQUIRED | DM_SM_FLAGS_INITIATOR
            : DM_SM_FLAGS_RESPONSE_REQUIRED;

        send_dm_sm_common((DM_UPRIM_T*)p_dm_prim, &p_acl->addrt);
    }
    else
    {
        p_hci_prim = pnew(HCI_USER_PASSKEY_REQUEST_NEG_REPLY_T);
        bd_addr_copy(&p_hci_prim->bd_addr, p_bd_addr);
        send_sm_to_hci((HCI_UPRIM_T*)p_hci_prim,
                       HCI_USER_PASSKEY_REQUEST_NEG_REPLY);
    }
}
#endif

#ifndef DISABLE_DM_BREDR
void dm_sm_hci_simple_pairing_complete(HCI_UPRIM_T *hci_uprim)
{
    DM_ACL_T *p_acl;

    /* We only pass on failures. A success implies that a link key is on
       its way, so we'll wait for that. */
    if ((p_acl = dm_acl_find_by_bd_addr(
                &hci_uprim->hci_simple_pairing_complete_event.bd_addr)) != NULL)
    {
        if (hci_uprim->hci_simple_pairing_complete_event.status != HCI_SUCCESS)
        {
            dm_sm_simple_pairing_complete(
                    p_acl,
                    hci_uprim->hci_simple_pairing_complete_event.status);
        }
    }
}
#endif

/*! \brief Tidy up and notify application after Simple Pairing has completed.

    If the SSP structure exists then we CsrSchedTidy up and inform the application.
    If it doesn't then we must have done it already, so we do nothing.

    We must not inform the state machine as this will be done if and
    when we get an authentication failure event.

    This function is also called internally by the security manager
    to avoid duplication of functionality.

    \param p_sm Pointer to ACL record structure.
    \param status Reason for failure.
*/
static void dm_sm_simple_pairing_complete(DM_ACL_T *p_acl, const CsrUint8 status)
{
    DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *p_prim;
    DM_SM_SSP_T *p_sm_ssp;

    DM_ACL_DEBUG_CHECK(p_acl);

    if (dm_acl_is_ble(p_acl))
    {
        sm_acl_closed(p_acl->dm_acl_client_sm.u.p_sm);
    }
    else if ((p_sm_ssp = p_acl->dm_acl_client_sm.u.p_ssp) != NULL)
    {
        /* Tidy up. */
        CsrPmemFree(p_sm_ssp->oob_hash_c);
        CsrPmemFree(p_sm_ssp->oob_rand_r);
        CsrPmemFree(p_sm_ssp);
        p_acl->dm_acl_client_sm.u.p_ssp = NULL;

        /* Notify application. */
        p_prim = zpnew(DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T);
        p_prim->type = DM_SM_SIMPLE_PAIRING_COMPLETE_IND;
        p_prim->status = status;

        send_dm_sm_common((DM_UPRIM_T*)p_prim, &p_acl->addrt);

        /* Unlock the ACL without necessarily triggering ACL closure.
           Using dm_sm_acl_close() would trigger the closing of the ACL
           after a timeout. We don't want to do that. We just want to
           release the lock so that we don't keep it open if other clients
           wish to close it. */
        dm_acl_unlock(p_acl, dm_acl_client_sm);
        if (p_acl->dm_acl_client_sm.state == SM_ST_IDLE)
            dm_acl_idle_timeout_extend_short(p_acl);
    }
}

#ifndef DISABLE_DM_BREDR
/*! \brief Request from HCI for our OOB data.

    We've just finished exchanging IO capabilities information with the
    remote device and now we're being asked for our OOB data. If there
    is any to be had then we will have been given it by now, so if we
    can't find it then we reject the request.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
void dm_sm_hci_remote_oob_data_request(const BD_ADDR_T *const p_bd_addr)
{
    HCI_REMOTE_OOB_DATA_REQUEST_REPLY_T *hci_prim;
    DM_ACL_T *p_acl;
    DM_SM_SSP_T *p_sm_ssp;
    hci_op_code_t op_code;

    /* NEG_REPLY is unlikely ever to be used, because we only set the
       oob_data_present parameter in the IO capabilities exchange once
       we've received the data from the host. So we pmalloc for the
       larger primitive even when doing NEG_REPLY. */
    hci_prim = pnew(HCI_REMOTE_OOB_DATA_REQUEST_REPLY_T);

    /* Search for ACL and associated OOB data */
    op_code = HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY;
    if ((p_acl = dm_acl_find_by_bd_addr(p_bd_addr)) != NULL
            && (p_sm_ssp = p_acl->dm_acl_client_sm.u.p_ssp) != NULL
            && p_sm_ssp->oob_hash_c != NULL)
    {
        CsrMemCpy(&hci_prim->c, p_sm_ssp->oob_hash_c, SIZE_OOB_DATA);
        CsrMemCpy(&hci_prim->r, p_sm_ssp->oob_rand_r, SIZE_OOB_DATA);
        op_code = HCI_REMOTE_OOB_DATA_REQUEST_REPLY;
    }

    bd_addr_copy(&hci_prim->bd_addr, p_bd_addr);
    send_sm_to_hci((HCI_UPRIM_T*)hci_prim, op_code);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Request from HCI to display passkey locally.

    We must display the passkey until we get a simple pairing complete
    event so that it may be entered into the remote device.

    We just pass the request to the application.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param passkey Pointer to passkey - number from 0 to 999999.
*/
void dm_sm_hci_user_passkey_notification(const BD_ADDR_T *const p_bd_addr,
                                         const CsrUint32 *const passkey)
{
    DM_SM_USER_PASSKEY_NOTIFICATION_IND_T *p_prim;

    p_prim = zpnew(DM_SM_USER_PASSKEY_NOTIFICATION_IND_T);
    p_prim->type = DM_SM_USER_PASSKEY_NOTIFICATION_IND;
    p_prim->passkey = *passkey;
    p_prim->dev_class = dm_acl_get_cod_by_bd_addr(p_bd_addr);
    tbdaddr_copy_from_public_bd_addr(&p_prim->addrt, p_bd_addr);
    if (dm_sm_is_initiator(&p_prim->addrt))
        p_prim->flags = DM_SM_FLAGS_INITIATOR;

    DM_SendMessageAMphandle(p_prim);
}

#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Notification from HCI of keypresses during
           passkey entry on remote device.

    Keys are being pressed on the remote device while we're displaying the
    passkey for them to type. Perhaps the application is trying to display
    asterisks to help them, so just pass along the information.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param notification_type Indication of nature of keypress.
*/
void dm_sm_hci_keypress_notification(const BD_ADDR_T *const p_bd_addr,
                                     const CsrUint8 notification_type)
{
    DM_SM_KEYPRESS_NOTIFICATION_IND_T *p_prim;
    TYPED_BD_ADDR_T addrt;

    tbdaddr_copy_from_public_bd_addr(&addrt, p_bd_addr);

    p_prim = zpnew(DM_SM_KEYPRESS_NOTIFICATION_IND_T);
    p_prim->type = DM_SM_KEYPRESS_NOTIFICATION_IND;
    p_prim->notification_type = notification_type;

    send_dm_sm_common((DM_UPRIM_T*)p_prim, &addrt);
}
#endif

/*! \brief Handle refresh encryption key complete event.

    \param hci_prim Pointer to HCI event primitive.
*/
void dm_sm_hci_refresh_encryption_key_complete(hci_connection_handle_t handle,
                                               hci_return_t status)
{
    DM_ACL_T *p_acl;
    fsm_event_t event;

    if ((p_acl = dm_acl_find_by_handle(handle)) != NULL)
    {
        DM_SendTBDADDRCommandComplete(DM_HCI_REFRESH_ENCRYPTION_KEY_IND,
                                      status,
                                      &p_acl->addrt);

        if (dm_acl_is_ble(p_acl))
        {
            sm_encryption_changed(handle, (CsrBool)(status == HCI_SUCCESS), status);
        }
        else if ((p_acl->dm_acl_client_sm.flags
                    & DM_SM_FLAG_NEED_ENCRYPTION_REFRESH) != 0)
        {
            /* For transaction collisions, just try again.*/
            if (status == HCI_ERROR_LMP_TRANSACTION_COLLISION)
            {
                send_hci_refresh_encryption_key(p_acl);
                return;
            }

            event = SM_EV_HCI_ENC_CHANGE_OK;
            if (status != HCI_SUCCESS)
                event = SM_EV_HCI_ENC_CHANGE_FAIL;

            p_acl->dm_acl_client_sm.flags &= ~DM_SM_FLAG_NEED_ENCRYPTION_REFRESH;

            sm_state_machine(p_acl, event, &status);
        }
    }
}


/*! \brief Interface with ACL manager */

/*! \brief Called from the ACL manager when a connected ACL has been closed.

    The Security Manager needs to reject any outstanding requests and
    CsrSchedTidy up the SSP structure.

    \param p_sm Pointer to ACL record structure for doomed ACL.
*/
CsrUint16 dm_sm_acl_closed(DM_ACL_T *p_acl, CsrUint16 reason)
{
    TYPED_BD_ADDR_T *p_bd_addr = &p_acl->addrt;

    DM_ACL_DEBUG_CHECK(p_acl);

    if ((reason & DM_ACL_REASON_RECONNECTING_AND_LOCKED) == 0)
    {
        /* Reject outstanding requests */
        while (p_acl->dm_acl_client_sm.p_requests != NULL)
        {
            switch (p_acl->dm_acl_client_sm.p_requests->u.type)
            {
                case DM_SM_ACCESS_REQ:
                    send_dm_sm_access_cfm(
                            &p_acl->dm_acl_client_sm.p_requests->u.access_req,
                            (hci_return_t)HCI_ERROR_NO_CONNECTION);
                    break;

                case DM_SM_AUTHENTICATE_REQ:
                    send_dm_sm_authenticate_cfm(p_bd_addr, FALSE);
                    break;

                case DM_SM_ENCRYPT_REQ:
                    send_dm_sm_encrypt_cfm(p_bd_addr, FALSE, FALSE);
                    break;

                case DM_SM_BONDING_REQ:
                    send_dm_sm_bonding_cfm(p_bd_addr, HCI_ERROR_NO_CONNECTION);
                    break;

                case DM_SM_BONDING_CANCEL_REQ:
                    send_dm_sm_bonding_cfm(p_bd_addr,
                                           DM_SM_BONDING_CANCELLED);
                /* break; */
            }

            dm_sm_free_request(&p_acl->dm_acl_client_sm.p_requests);
        }
    }

    /* Cleanup simple pairing stuff */
    dm_sm_simple_pairing_complete(p_acl, HCI_ERROR_AUTH_FAIL);

    /* To avoid redundancy we call dm_sm_acl_reset */
    dm_sm_acl_reset(&p_acl->dm_acl_client_sm);

    /* Time to mask away the DM SM flags as well */
    p_acl->dm_acl_client_sm.flags = 0;

    return 0;
}

/*! \brief Called from the ACL manager when an ACL has been connected.

    The ACL manager tells us when an ACL is connected in case we
    are trying to bond or we have put authentication on hold while
    the ACL comes up.

    \param p_sm Pointer to ACL record structure for new ACL.
    \param status Indication of success or failure.
*/
void dm_sm_acl_opened(DM_ACL_T *p_acl, CsrUint8 status)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    if (status != HCI_SUCCESS)
    {
        dm_sm_remove_device(&p_acl->addrt, TRUE);
        sm_state_machine(p_acl, SM_EV_HCI_AUTH_FAIL, &status);
        dm_sm_acl_closed(p_acl, HCI_ERROR_OETC_USER);
    }
}

/*! \brief The ACL manager wants to know whether to pair during link setup.

    Before bringing up an ACL, the ACL manager may issue
    HCI_WRITE_AUTH_ENABLE to ward off legacy timeout issues.

    If we're in mode 4 and the service requires legacy authentication then
    we tell the ACL manager how far it should go to pair during link setup.

    \param p_connection Pointer to structure defining remote device and
    service, or NULL.
    \param locks Bitmap of current owners of ACL.
    \returns ACL connection manager FSM event.
 */
DM_ACL_CONNECT_T dm_sm_pair_at_link_setup(
        const DM_ACL_T *p_acl,
        const DM_SM_SERVICE_T *service)
{
    if ((SMCB.security_mode != SEC_MODE4_SSP
                && SMCB.security_mode != SEC_MODE2_SERVICE)
            || SMCB.write_auth_enable == DM_SM_WAE_NEVER
            || service == NULL
            || service->protocol_id == SEC_PROTOCOL_NONE)
        return DM_ACL_CONNECT;

    /* If we're bonding then we always need to pair (obviously!)
       but if not then we check the requirements for the service. */
    if (service->protocol_id != SEC_PROTOCOL_SM_BONDING)
    {
        dm_security_level_t seclevel;
        DM_SM_SERVICE_RECORD_T *p_rec = *find_service(
                                                service,
                                                &TBDADDR_ADDR(p_acl->addrt));

        seclevel = p_rec == NULL ? SMCB.secl_default : p_rec->security_level;

        /* Check need for outgoing authentication to legacy devices. */
        if ((seclevel & (SECL_OUT_AUTHENTICATION|SECL_OUT_ENCRYPTION)) == 0)
            return DM_ACL_CONNECT;
    }

    /* See if we need to drop the ACL and then reconnect. */
    if (SMCB.write_auth_enable != DM_SM_WAE_ACL_NONE
            && (SMCB.write_auth_enable & p_acl->locks) == p_acl->locks)
    {
        /* Only drop if remote device is pre-BT2.1 */
        if (!p_acl->dm_acl_client_sm.encrypted
                /* Note that DM_SM_FLAG_RSF_RHSF_SSP is multi-bit and all
                   must be set for SSP support. */
                && (p_acl->dm_acl_client_sm.flags & DM_SM_FLAG_RSF_RHSF_SSP)
                        != DM_SM_FLAG_RSF_RHSF_SSP)
        {
            return DM_ACL_AUTH_RECONNECT;
        }
    }

    return DM_ACL_AUTH_CONNECT;
}

/*! \brief Decide whether or not to automatically start pairing after we
    receive authentication failure

    Key missing.
    The 2.1 specification says that we should not do this in the case of
    2.1 V 2.1. Interoperability means that we should consider this for
    2.1 V 2.0. See B-32313.

    Transaction collision.
    We should retry transactions a limited number of times.

    \param p_acl Pointer to ACL record structure.
    \param status Indication of type of failure.
    \returns HCI_ERROR_KEY_MISSING, HCI_ERROR_LMP_TRANSACTION_COLLISION or FSM_EVENT_NULL
*/
fsm_event_t dm_sm_retry_authentication_event(DM_ACL_T *p_acl, CsrUint8 *status)
{
    fsm_event_t event = FSM_EVENT_NULL;

    DM_ACL_DEBUG_CHECK(p_acl);

    switch (*status)
    {
        case HCI_ERROR_KEY_MISSING:
            /* Make sure that this really is a missing link key (and
               not a rejected PIN code request, which can also
               generate this error code). */
            if ((p_acl->dm_acl_client_sm.flags & DM_SM_FLAG_KEY_PENDING) != 0)
            {
                /* Link key missing, so see if we can go straight to pairing. */
                if ((DM_SM_SEC_MODE_CONFIG_LEGACY_AUTO_PAIR_MISSING_LINK_KEY
                            & SMCB.config) != 0)
                {
                    /* Make sure that the remote device is definitely legacy. */
                    if (dm_sm_is_legacy(p_acl))
                    {
                        p_acl->dm_acl_client_sm.flags |= DM_SM_FLAG_KEY_MISSING;
                        event = SM_EV_RETRY_AUTHENTICATE;
                    }
                }
            }
            else
            {
                /* This isn't really a missing link key, so use the
                   generic auth-failure code so as not to confuse L2CAP */
                *status = HCI_ERROR_AUTH_FAIL;
            }
            break;

        case HCI_ERROR_LMP_TRANSACTION_COLLISION:
            if (p_acl->dm_acl_client_sm.p_requests != NULL &&
                    ++p_acl->dm_acl_client_sm.p_requests->transaction_retries < DM_SM_AUTH_RETRIES_MAX)
                event = SM_EV_RETRY_AUTHENTICATE;

            break;
    }

    p_acl->dm_acl_client_sm.flags &= ~DM_SM_FLAG_KEY_PENDING;

    return event;
}


/*! Private Function Implementations */


/*! \brief Drive the SM state machine

    If the state machine ends up in the Idle state then the current
    request is released and the next one is started.

    \param p_sm Pointer to ACL record structure.
    \param event FSM event.
    \param pv_data Pointer to data to pass on to action function.
*/
static void sm_state_machine(DM_ACL_T *p_acl, const fsm_event_t event, void *pv_data)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    fsm_16bit_run(&dm_sm_fsm, &p_acl->dm_acl_client_sm.state, event, p_acl, pv_data);
    fsm_16bit_run(&dm_sm_fsm, &p_acl->dm_acl_client_sm.state, SM_EV_PURGE, p_acl, NULL);
    kick_request_process(p_acl);
}

/*! \brief FSM action functions */

/*! \brief Kill expired request.

    \param pv_arg1 p_acl - pointer to ACL record structure.
    \returns FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_kill_request)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;

    CSR_UNUSED(pv_arg2);

    DM_ACL_DEBUG_CHECK(p_acl);

    if (p_acl->dm_acl_client_sm.p_requests != NULL)
    {
        if (p_acl->dm_acl_client_sm.p_requests->transaction_holdoff_timer == 0)
        {
            dm_sm_free_request(&p_acl->dm_acl_client_sm.p_requests);
            return SM_EV_PURGED;
        }
        else
        {
            /* Zombify dead requests to prevent sending duplicate
               responses to the application. */
            p_acl->dm_acl_client_sm.p_requests->u.type = DM_SM_ZOMBIE_REQ;
        }
    }

    return FSM_EVENT_NULL;
}

/*! \brief Check if a change of encryption is required.

    \param pv_arg1 p_acl - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns SM_EV_OK or FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_encrypt_req)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;
    CsrBool encrypt = p_acl->dm_acl_client_sm.p_requests->u.enc_req.encrypt;

    DM_ACL_DEBUG_CHECK(p_acl);

    if (!p_acl->dm_acl_client_sm.encrypted)
    {
        /* We're not encrypted. See if we need to encrypt. */
        if (encrypt)
        {
            /* We need to encrypt, so authenticate first. */
            send_hci_auth_req(p_acl->handle);
            return FSM_EVENT_NULL;
        }
    }
    else
    {
        /* We're encrypted. See if we need to unencrypt. */
        if (!encrypt)
        {
            /* We need to unencrypt. No need to authenticate for that. */
            return SM_EV_HCI_AUTH_OK;
        }
    }

    return SM_EV_OK;
}

/*! \brief Request link encryption to be changed.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_hci_set_encryption)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;
    CsrUint8 encryption = HCI_ENC_OFF;

    CSR_UNUSED(pv_arg2);

    DM_ACL_DEBUG_CHECK(p_acl);

    if (p_acl->dm_acl_client_sm.p_requests->u.enc_req.encrypt)
        encryption = HCI_ENC_ON;

    send_hci_set_conn_encryption(p_acl->handle, encryption);

    return FSM_EVENT_NULL;
}

/*! \brief Issue an encryption failure confirmation.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_enc_cfm_fail)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;

    DM_ACL_DEBUG_CHECK(p_acl);

    CSR_UNUSED(pv_arg2);

    send_dm_sm_encrypt_cfm(&p_acl->addrt, FALSE,
            p_acl->dm_acl_client_sm.encrypted);

    return FSM_EVENT_NULL;
}

/*! \brief Issue an encryption success confirmation.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_enc_cfm_ok)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;

    DM_ACL_DEBUG_CHECK(p_acl);

    CSR_UNUSED(pv_arg2);

    send_dm_sm_encrypt_cfm(&p_acl->addrt, TRUE,
            p_acl->dm_acl_client_sm.encrypted);

    return FSM_EVENT_NULL;
}

/*! \brief Request link authentication.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_authenticate_req)
{
    CSR_UNUSED(pv_arg2);

    DM_ACL_DEBUG_CHECK((DM_ACL_T*)pv_arg1);

    send_hci_auth_req(((DM_ACL_T*)pv_arg1)->handle);

    return FSM_EVENT_NULL;
}

/*! \brief Request link authentication (bonding).

    \param pv_arg1 p_acl - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_bond_req)
{
    CSR_UNUSED(pv_arg2);

    DM_ACL_DEBUG_CHECK((DM_ACL_T*)pv_arg1);

    if (dm_sm_current_link_key_acceptable(&((DM_ACL_T*)pv_arg1)->addrt, NULL)
            != DM_SM_ACCEPTABLE)
    {
        send_hci_auth_req(((DM_ACL_T*)pv_arg1)->handle);
        return FSM_EVENT_NULL;
    }

    /* We must have successfully paired during ACL setup. */
    ((DM_ACL_T*)pv_arg1)->dm_acl_client_sm.p_requests->bonding_status =
        (hci_error_t)HCI_SUCCESS;
    return SM_EV_HCI_AUTH_OK;
}

/*! \brief Issue an authentication success confirmation.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_auth_cfm_ok)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;

    CSR_UNUSED(pv_arg2);

    DM_ACL_DEBUG_CHECK(p_acl);

    send_dm_sm_authenticate_cfm(&p_acl->addrt, TRUE);

    return FSM_EVENT_NULL;
}

/*! \brief Issue an authentication failure confirmation.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_auth_cfm_fail)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;

    CSR_UNUSED(pv_arg2);

    DM_ACL_DEBUG_CHECK(p_acl);

    send_dm_sm_authenticate_cfm(&p_acl->addrt, FALSE);

    return FSM_EVENT_NULL;
}

/*! \brief Issue a bonding success confirmation.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_bond_cfm_ok)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;

    CSR_UNUSED(pv_arg2);

    DM_ACL_DEBUG_CHECK(p_acl);

    dm_sm_acl_close(p_acl, HCI_ERROR_OETC_USER, ACL_IDLE_TIMEOUT_AT_MOST_BONDING);
    send_dm_sm_bonding_cfm(&p_acl->addrt,
            p_acl->dm_acl_client_sm.p_requests->bonding_status);

    return FSM_EVENT_NULL;
}

/*! \brief Issue a bonding failure confirmation.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_bond_cfm_fail)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;

    DM_ACL_DEBUG_CHECK(p_acl);

    dm_sm_acl_close(p_acl, HCI_ERROR_AUTH_FAIL, ACL_IDLE_TIMEOUT_AT_MOST_NONE);
    send_dm_sm_bonding_cfm(&p_acl->addrt, (hci_error_t)
            (pv_arg2 == NULL ? HCI_ERROR_UNSPECIFIED : *(hci_error_t*)pv_arg2));

    return FSM_EVENT_NULL;
}

/*! \brief Issue a bonding cancelled confirmation.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_bond_cfm_cancel)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;

    CSR_UNUSED(pv_arg2);

    DM_ACL_DEBUG_CHECK(p_acl);

    dm_sm_acl_close(p_acl, HCI_ERROR_OETC_USER, ACL_IDLE_TIMEOUT_AT_MOST_NONE);
    send_dm_sm_bonding_cfm(&p_acl->addrt, DM_SM_BONDING_CANCELLED);

    return FSM_EVENT_NULL;
}

/*! \brief Decide whether authentication is needed.


    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns SM_EV_HCI_AUTH_FAIL if it's hopeless, SM_EV_HCI_AUTH_OK if it's
             not needed, or FSM_EVENT_NULL if we must authenticate.
*/
FSM_ACTION_FN(sm_af_access_authenticate)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;
    DM_SM_REQUEST_T *p_req = p_acl->dm_acl_client_sm.p_requests;
    dm_security_level_t seclevel = p_req->seclevel;

    CSR_UNUSED(pv_arg2);

    DM_ACL_DEBUG_CHECK(p_acl);

    /* If the link's encrypted and the link key's OK, we need nothing more. */
    if (p_acl->dm_acl_client_sm.encrypted
            && dm_sm_current_link_key_acceptable(&p_acl->addrt, NULL)
            == DM_SM_ACCEPTABLE)
        return SM_EV_HCI_AUTH_OK;

    if (dm_sm_is_legacy(p_acl))
    {
        /* Legacy, but no legacy security needed */
        if ((seclevel & SECL_AUTHENTICATION) == 0)
            return SM_EV_HCI_AUTH_OK;

        /* We aren't both in mode 4, so it's legacy or fail. */
        if ((seclevel & SECL4_NO_LEGACY) != 0)
            return SM_EV_HCI_AUTH_FAIL;
    }
    else
    {
        /* In Security Mode 4, encryption must be enabled before we receive
           any access requests for incoming connections, save for SDP.
           If it is not enabled then we must drop the ACL. We delay 2 seconds
           first just incase there is a race between the encryption event and
           an l2cap connect request. */
        if (p_req->u.access_req.conn_setup.incoming)
        {
            if (p_req->u.access_req.conn_setup.connection.service.protocol_id != SEC_PROTOCOL_L2CAP
                    || p_req->u.access_req.conn_setup.connection.service.channel != SDP_PSM)
            {
                if (!p_acl->dm_acl_client_sm.encrypted)
                {
                    timer_start(&p_req->detach_timer,
                                DM_CONFIG_SM_DETACH_TIMEOUT,
                                detach_after_delay,
                                0,
                                p_acl);
                    return FSM_EVENT_NULL;
                }
            }
        }

        if ((seclevel & SECL4_SSP) == 0)
            return SM_EV_HCI_AUTH_OK;
    }

    send_hci_auth_req(p_acl->handle);
    return FSM_EVENT_NULL;
}

/*! \brief Decide whether authorisation is needed.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns SM_EV_AUTHORISE_OK if it's not needed, otherwise FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_access_authorise)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;
    DM_SM_REQUEST_T *p_req = p_acl->dm_acl_client_sm.p_requests;

    CSR_UNUSED(pv_arg2);

    DM_ACL_DEBUG_CHECK(p_acl);

    if ((p_req->seclevel & SECL_AUTHORISATION) != 0)
    {
        switch (dm_sm_trust((&p_acl->addrt), &p_req->u.access_req.conn_setup.connection.service))
        {
            case DM_SM_NOT_FOUND:
                /* Unknown or untrusted device. Get authorisation. */
                send_dm_sm_authorise_ind(&p_req->u.access_req);
                return FSM_EVENT_NULL;

            case DM_SM_UNACCEPTABLE:
                /* Device prohibited from accessing this service. */
                return SM_EV_AUTHORISE_FAIL;

            default:
                break;
        }
    }

    return SM_EV_AUTHORISE_OK;
}

/*! \brief Decide whether to encrypt the link.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns SM_EV_OK if it's not needed, otherwise FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_access_encrypt)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;

    CSR_UNUSED(pv_arg2);

    DM_ACL_DEBUG_CHECK(p_acl);

    if (!p_acl->dm_acl_client_sm.encrypted)
    {
        dm_security_level_t enc_test;

        /* Decide whether we check for SSP or legacy encryption rules. */
        enc_test = SECL4_SSP;
        if (dm_sm_is_legacy(p_acl))
            enc_test = SECL_ENCRYPTION;

        /* Check to see if we need to encrypt. */
        if ((p_acl->dm_acl_client_sm.p_requests->seclevel & enc_test) != 0)
        {
            send_hci_set_conn_encryption(p_acl->handle, HCI_ENC_ON);
            return FSM_EVENT_NULL;
        }
    }
    else if ((p_acl->dm_acl_client_sm.flags & DM_SM_FLAG_NEED_ENCRYPTION_REFRESH) != 0)
    {
        /* Already encrypted but we need to refresh the encryption key. */
        send_hci_refresh_encryption_key(p_acl);
        return FSM_EVENT_NULL;
    }

    /* No encryption change needed. */
    return SM_EV_HCI_ENC_CHANGE_OK;
}

/*! \brief Inform the calling protocol of the access request failure.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 p_status - pointer to failure reason, or NULL if unknown.
    \returns FSM_EVENT_NULL
*/
FSM_ACTION_FN(sm_af_access_fail)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;
    DM_SM_REQUEST_T *p_req = p_acl->dm_acl_client_sm.p_requests;
    hci_return_t *p_status = (hci_return_t*)pv_arg2;

    DM_ACL_DEBUG_CHECK(p_acl);

    send_dm_sm_access_cfm(&p_req->u.access_req,
        (hci_return_t)(p_status == NULL ? HCI_ERROR_UNSPECIFIED : *p_status));

    return FSM_EVENT_NULL;
}

/*! \brief Inform the calling protocol of the access request acceptance.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns FSM_EVENT_NULL
*/
FSM_ACTION_FN(sm_af_access_success)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;
    DM_SM_REQUEST_T *p_req = p_acl->dm_acl_client_sm.p_requests;

    CSR_UNUSED(pv_arg2);

    DM_ACL_DEBUG_CHECK(p_acl);

    send_dm_sm_access_cfm(&p_req->u.access_req, HCI_SUCCESS);

    return FSM_EVENT_NULL;
}

/*! \brief Check ACL connection state before access request.

    If we're connected then we start an access request. Otherwise
    we do nothing and wait for the ACL to finish coming up.

    \param pv_arg1 p_acl - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns SM_EV_ACL_READY or FSM_EVENT_NULL.
*/
FSM_ACTION_FN(sm_af_check_connection)
{
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;
    fsm_event_t fsm_event = FSM_EVENT_NULL;

    CSR_UNUSED(pv_arg2);

    DM_ACL_DEBUG_CHECK(p_acl);

    /* Check whether by this time the ACL is in connected state plus
    * remote supported features and remote host supported features
    * are known to DM. A null event is returned otherwise.
    */
    fsm_event = dm_sm_remote_features_known(p_acl);

    return fsm_event;
}

/*! \brief See if we can cancel a timed detach request.

    There has been a change of encryption, which may mean that we don't
    have to disconnect the link after all.

    \param pv_arg1 p_sm - pointer to ACL record structure.
    \param pv_arg2 Unused.
    \returns FSM_EVENT_NULL or SM_EV_RETRY_AUTHENTICATE.
*/
FSM_ACTION_FN(sm_af_check_acl_detach)
{
    fsm_event_t event = FSM_EVENT_NULL;
    DM_ACL_T *p_acl = (DM_ACL_T *) pv_arg1;
    DM_SM_REQUEST_T *p_req = p_acl->dm_acl_client_sm.p_requests;

    DM_ACL_DEBUG_CHECK(p_acl);

    if (p_req != NULL && p_req->detach_timer != 0)
    {
        /* We are waiting to detach. */
        if (p_acl->dm_acl_client_sm.encrypted)
        {
            /* We've encrypted in the meantime, so cancel detach
               and retry the authentication part. */
            timer_cancel(&p_req->detach_timer);

            event = SM_EV_RETRY_AUTHENTICATE;
        }
    }

    return event;
}

/*! \brief Issue DM_SM_ACCESS_CFM and DM_SM_ACCESS_IND primitives.

    The _REQ, _IND and _CFM primitives are identical up to and including
    the incoming parameter, so we use the offset of the following parameter
    and memcpy.

    \param p_req Pointer to request structure.
    \param status Indication of success or failure.
*/
static void send_dm_sm_access_cfm(const DM_SM_ACCESS_REQ_T *const p_req,
                                                        const hci_return_t status)
{
    DM_SM_ACCESS_IND_T *p_ind;
    DM_SM_ACCESS_CFM_T *p_cfm;
    p_cfm = pnew(DM_SM_ACCESS_CFM_T);

    /* Start of REQ/CFM primitives identical (except for type) */
    CsrMemCpy(p_cfm, p_req, sizeof(DM_SM_ACCESS_REQ_T));
    p_cfm->type = DM_SM_ACCESS_CFM;
    p_cfm->status = status;

    /* Send copy to application */
    p_ind = pcopy(p_cfm, sizeof(DM_SM_ACCESS_IND_T));
    p_ind->type = DM_SM_ACCESS_IND;
    send_dm_sm_common((DM_UPRIM_T *)p_ind, NULL);

    DM_SendMessage(p_cfm);
}

/*! \brief Issue a DM_SM_AUTHORISE_IND primitive.

    \param p_req Pointer to access request primitive.
*/
static void send_dm_sm_authorise_ind(DM_SM_ACCESS_REQ_T *p_req)
{
    DM_SM_AUTHORISE_IND_T *p_prim;

    p_prim = pnew(DM_SM_AUTHORISE_IND_T);
    p_prim->type = DM_SM_AUTHORISE_IND;
    CsrMemCpy(&p_prim->cs, &p_req->conn_setup, sizeof(DM_CONNECTION_SETUP_T));

    send_dm_sm_common((DM_UPRIM_T *)p_prim, NULL);
}

/*! \brief Issue a DM_SM_AUTHENTICATE_CFM primitive.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param success Indication of success or failure.
*/
static void send_dm_sm_authenticate_cfm(const TYPED_BD_ADDR_T *const addrt, const CsrBool success)
{
    DM_SM_AUTHENTICATE_CFM_T *p_prim;

    p_prim = pnew(DM_SM_AUTHENTICATE_CFM_T);
    p_prim->type = DM_SM_AUTHENTICATE_CFM;
    p_prim->success = success;
    (void)tbdaddr_copy_to_bd_addr(&p_prim->bd_addr, addrt);

    send_dm_sm_common((DM_UPRIM_T *)p_prim, NULL);
}

/*! \brief Issue a DM_SM_ENCRYPT_CFM primitive.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param success Indication of success or failure.
    \param encrypted Encryption state of link.
*/
static void send_dm_sm_encrypt_cfm(const TYPED_BD_ADDR_T *const addrt,
                                   const CsrBool success,
                                   const CsrBool encrypted)
{
    DM_SM_ENCRYPT_CFM_T *p_prim;

    p_prim = pnew(DM_SM_ENCRYPT_CFM_T);
    p_prim->type = DM_SM_ENCRYPT_CFM;
    p_prim->success = success;
    p_prim->encrypted = encrypted;
    (void)tbdaddr_copy_to_bd_addr(&p_prim->bd_addr, addrt);

    send_dm_sm_common((DM_UPRIM_T *)p_prim, NULL);
}


#ifdef BUILD_FOR_HOST_OLD_LINK_KEY_API
/* Returns pointer to newly allocated translated primitive, or NULL if primitive does not translate. */
static DM_UPRIM_T *dm_sm_api_translate(DM_UPRIM_T *uprim)
{
    switch (uprim->type)
    {
        case DM_SM_KEYS_IND:
            if (uprim->dm_sm_keys_ind.keys.present == DM_SM_KEY_ENC_BREDR)
            {
                DM_SM_LINK_KEY_IND_T *prim = pnew(DM_SM_LINK_KEY_IND_T);
                prim->type = DM_SM_LINK_KEY_IND;
                prim->phandle = uprim->dm_sm_keys_ind.phandle;
                tbdaddr_copy_to_bd_addr(&prim->bd_addr, &uprim->dm_sm_keys_ind.addrt);
                prim->key_type = (CsrUint8)(uprim->dm_sm_keys_ind.keys.u[0].enc_bredr->link_key_type);
                CsrBtMemCpyUnpack(prim->key, uprim->dm_sm_keys_ind.keys.u[0].enc_bredr->link_key, SIZE_LINK_KEY/2);
                return (DM_UPRIM_T *)prim;
             }
             break;

        case DM_SM_KEY_REQUEST_IND:
            if (uprim->dm_sm_key_request_ind.key_type == DM_SM_KEY_ENC_BREDR)
            {
                DM_SM_LINK_KEY_REQUEST_IND_T *prim = pnew(DM_SM_LINK_KEY_REQUEST_IND_T);
                prim->type = DM_SM_LINK_KEY_REQUEST_IND;
                prim->phandle = uprim->dm_sm_key_request_ind.phandle;
                tbdaddr_copy_to_bd_addr(&prim->bd_addr, &uprim->dm_sm_key_request_ind.addrt);
                prim->dev_class = uprim->dm_sm_key_request_ind.dev_class;
                prim->authenticated = !!uprim->dm_sm_key_request_ind.security_requirements;
                prim->initiated_outgoing = uprim->dm_sm_key_request_ind.initiated_outgoing;
                prim->rhsf = uprim->dm_sm_key_request_ind.rhsf;
                return (DM_UPRIM_T *)prim;
            }
            break;
    }

    return NULL;
}
#endif /* BUILD_FOR_HOST_OLD_LINK_KEY_API */
/*! \brief Issue a DM_SM_KEY_REQUEST_IND primitive.

    \param addrt Pointer to Bluetooth address of remote device.
*/
static void send_dm_sm_key_request_ind(const TYPED_BD_ADDR_T *const addrt)
{
    DM_SM_KEY_REQUEST_IND_T *prim;
    DM_ACL_T *acl;
    CsrUint16 rhsf = 0xFFFF;     /* Remote host supported features unknown */

    prim = zpnew(DM_SM_KEY_REQUEST_IND_T);
    prim->type = DM_SM_KEY_REQUEST_IND;
    prim->dev_class = dm_acl_get_cod_by_bd_addr(&TBDADDR_ADDR(*addrt));
    prim->key_type = DM_SM_KEY_ENC_BREDR;

    if ((acl = dm_acl_find_by_tbdaddr(addrt)) != NULL)
    {
        if (dm_acl_is_connected(acl))
        {
            /* Report cached remote host supported features. Note that
               DM_SM_FLAG_RSF_RHSF_SSP is multi-bit and all must be set
               for genuine SSP host support in the peer. */
            rhsf = 0;
            if ((acl->dm_acl_client_sm.flags & DM_SM_FLAG_RSF_RHSF_SSP)
                    == DM_SM_FLAG_RSF_RHSF_SSP)
            {
                rhsf = 1;
                prim->security_requirements = auth_key_required(addrt);
            }
        }

        /* Try to determine whether or not we are initiating security for
           an outgoing connection. If we are doing security as part of an
           ACCESS request then we can use the 'incoming' parameter from that
           primitive. Otherwise we can leave it as the default - FALSE. */
        if (acl->dm_acl_client_sm.state == SM_ST_ACCESS_AUTHENTICATING)
            prim->initiated_outgoing = !acl->dm_acl_client_sm.p_requests->u.access_req.conn_setup.incoming;
    }

    prim->rhsf = rhsf;

    tbdaddr_copy(&prim->addrt, addrt);


#ifdef BUILD_FOR_HOST_OLD_LINK_KEY_API
    send_dm_sm_common(dm_sm_api_translate((DM_UPRIM_T *)prim), NULL);
    CsrPmemFree(prim);
#else /* BUILD_FOR_HOST_OLD_LINK_KEY_API */
    send_dm_sm_common((DM_UPRIM_T *)prim, NULL);
#endif /* BUILD_FOR_HOST_OLD_LINK_KEY_API */
}
/*! \brief Issue a DM_SM_PIN_REQUEST_IND primitive.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
static void send_dm_sm_pin_request_ind(const TYPED_BD_ADDR_T *const addrt)
{
    DM_SM_PIN_REQUEST_IND_T *p_prim;

    p_prim = pnew(DM_SM_PIN_REQUEST_IND_T);
    p_prim->type = DM_SM_PIN_REQUEST_IND;
    p_prim->dev_class = dm_acl_get_cod_by_bd_addr(&TBDADDR_ADDR(*addrt));
    p_prim->initiator = dm_sm_is_initiator(addrt);

    send_dm_sm_common((DM_UPRIM_T *)p_prim, addrt);
}

/*! \brief Issue a DM_SM_KEYS_IND primitive.

    \param addrt Pointer to Bluetooth address of remote device.
    \param key_type Type of link key.
    \param key Link key in CsrUint8 array of size SIZE_LINK_KEY.
*/
static void send_dm_sm_keys_ind(
        const TYPED_BD_ADDR_T *const addrt,
        const CsrUint8 key_type,
        const CsrUint8 key[SIZE_LINK_KEY])
{
    DM_SM_KEYS_IND_T *prim = zpnew (DM_SM_KEYS_IND_T);

    prim->type = DM_SM_KEYS_IND;
    if (key_type == DM_SM_LINK_KEY_AUTHENTICATED
            || key_type == DM_SM_LINK_KEY_LEGACY)
    {
        prim->keys.security_requirements =
            DM_SM_SECURITY_AUTHENTICATED_NO_BONDING;
    }
    else
    {
        prim->keys.security_requirements =
            DM_SM_SECURITY_UNAUTHENTICATED_NO_BONDING;
    }
    prim->keys.present = DM_SM_KEY_ENC_BREDR;
    prim->keys.u[0].enc_bredr = pnew(DM_SM_KEY_ENC_BREDR_T);
    CsrBtMemCpyPack(prim->keys.u[0].enc_bredr->link_key, key, SIZE_LINK_KEY/2);
    prim->keys.u[0].enc_bredr->link_key_type = key_type;

    tbdaddr_copy(&prim->addrt, addrt);

#ifdef BUILD_FOR_HOST_OLD_LINK_KEY_API
{
    send_dm_sm_common(dm_sm_api_translate((DM_UPRIM_T *)prim), NULL);
    CsrPmemFree(prim->keys.u[0].enc_bredr);
    CsrPmemFree(prim);
}
#else /* BUILD_FOR_HOST_OLD_LINK_KEY_API */
    send_dm_sm_common((DM_UPRIM_T *)prim, NULL);
#endif /* BUILD_FOR_HOST_OLD_LINK_KEY_API */

}

#ifndef DISABLE_DM_BREDR
/*! \brief Issue a DM_SM_IO_CAPABILITY_REQUEST_IND primitive.

    We ask the application for its io_capability for SSP.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
static void send_dm_sm_io_capability_request_ind(const TYPED_BD_ADDR_T *const addrt)
{
    DM_SM_IO_CAPABILITY_REQUEST_IND_T *p_prim;

    p_prim = pnew(DM_SM_IO_CAPABILITY_REQUEST_IND_T);
    p_prim->type = DM_SM_IO_CAPABILITY_REQUEST_IND;
    p_prim->dev_class = dm_acl_get_cod_by_bd_addr(&TBDADDR_ADDR(*addrt));
    p_prim->flags = dm_sm_is_initiator(addrt)
        ? DM_SM_FLAGS_RESPONSE_REQUIRED | DM_SM_FLAGS_INITIATOR
        : DM_SM_FLAGS_RESPONSE_REQUIRED;

    send_dm_sm_common((DM_UPRIM_T *)p_prim, addrt);
}
#endif

/*! \brief Issue a DM_SM_BONDING_CFM primitive.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param status Indication of success or failure.
*/
static void send_dm_sm_bonding_cfm(const TYPED_BD_ADDR_T *const addrt, const hci_error_t status)
{
    DM_SM_BONDING_CFM_T *p_prim;

    p_prim = zpnew(DM_SM_BONDING_CFM_T);
    p_prim->type = DM_SM_BONDING_CFM;
    p_prim->status = status;

    send_dm_sm_common((DM_UPRIM_T*)p_prim, addrt);
}

#ifndef DISABLE_DM_BREDR
/*! \brief Issue a DM_SM_USER_CONFIRMATION_REQUEST_IND primitive.

    \param addrt Pointer to Bluetooth address of remote device.
    \param numeric_value Pointer to numerical value for comparison.
    \param io_cap_local Effective local IO capabilities.
    \param io_cap_remote Effective remote IO capabilities.
    \param response_required TRUE if the application must respond.
*/
static void send_dm_sm_user_confirmation_request_ind(
        const TYPED_BD_ADDR_T *const addrt,
        const CsrUint32 *const numeric_value,
        CsrUint8 io_cap_local,
        CsrUint8 io_cap_remote,
        CsrUint16 response_required)
{
    DM_SM_USER_CONFIRMATION_REQUEST_IND_T *p_dm_prim;

    p_dm_prim = pnew(DM_SM_USER_CONFIRMATION_REQUEST_IND_T);
    p_dm_prim->type = DM_SM_USER_CONFIRMATION_REQUEST_IND;
    p_dm_prim->dev_class = dm_acl_get_cod_by_bd_addr(&TBDADDR_ADDR(*addrt));
    p_dm_prim->numeric_value = *numeric_value;
    p_dm_prim->io_cap_local = io_cap_local;
    p_dm_prim->io_cap_remote = io_cap_remote;
    p_dm_prim->flags = response_required | (dm_sm_is_initiator(addrt)
            ? DM_SM_FLAGS_INITIATOR : 0);
    send_dm_sm_common((DM_UPRIM_T*)p_dm_prim, addrt);
}
#endif

/* Send HCI primitives */

/*! \brief Inform controller that we don't have a link key for this device.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
static void send_hci_link_key_req_neg_reply(const BD_ADDR_T *const p_bd_addr)
{
    HCI_LINK_KEY_REQ_NEG_REPLY_T *p_prim;

    p_prim = pnew(HCI_LINK_KEY_REQ_NEG_REPLY_T);
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);

    send_sm_to_hci((HCI_UPRIM_T*)p_prim, HCI_LINK_KEY_REQ_NEG_REPLY);
}

/*! \brief Provide controller with link key for remote device.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param key Array of size SIZE_LINK_KEY containing link key.
*/
static void send_hci_link_key_req_reply(const BD_ADDR_T *const p_bd_addr,
        const CsrUint8 key[SIZE_LINK_KEY])
{
    HCI_LINK_KEY_REQ_REPLY_T *p_prim;

    p_prim = pnew(HCI_LINK_KEY_REQ_REPLY_T);
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);
    CsrMemCpy(p_prim->key_val, key, SIZE_LINK_KEY);

    send_sm_to_hci((HCI_UPRIM_T*)p_prim, HCI_LINK_KEY_REQ_REPLY);
}

/*! \brief Reject a PIN code request.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
static void send_hci_pin_code_req_neg_reply(const BD_ADDR_T *const p_bd_addr)
{
    HCI_PIN_CODE_REQ_NEG_REPLY_T *p_prim;

    p_prim = pnew(HCI_PIN_CODE_REQ_NEG_REPLY_T);
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);

    send_sm_to_hci((HCI_UPRIM_T*)p_prim, HCI_PIN_CODE_REQ_NEG_REPLY);
}

/*! \brief Provide controller with PIN code for authentication of remote device.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param pin_code_length Length of PIN code.
    \param pin Pointer to PIN code.
*/
static void send_hci_pin_code_req_reply(const BD_ADDR_T *const p_bd_addr,
        CsrUint8 pin_code_length, const CsrUint8 *pin)
{
    HCI_PIN_CODE_REQ_REPLY_T *p_prim = zpnew(HCI_PIN_CODE_REQ_REPLY_T);
    CsrUint16 *hci_pin = p_prim->pin;


    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);

    /* pin_code_length clipped by HCI maximum. */
    if (pin_code_length > HCI_MAX_PIN_LENGTH)
        pin_code_length = HCI_MAX_PIN_LENGTH;
    p_prim->pin_code_length = pin_code_length;

    /* We need to pack the CsrUint8 array into a CsrUint16 array. */
    for (; pin_code_length > 1; pin_code_length -=2, ++hci_pin)
        *hci_pin = read_uint16(&pin);

    /* Copy final character in case length is odd. */
    if (pin_code_length != 0)
        *hci_pin = *pin;

    send_sm_to_hci((HCI_UPRIM_T*)p_prim, HCI_PIN_CODE_REQ_REPLY);
}

/*! \brief Request authentication of remote device.

    \param handle ACL handle of existing connection to remote device.
*/
static void send_hci_auth_req(const hci_connection_handle_t handle)
{
    HCI_AUTH_REQ_T *p_prim;

    p_prim = zpnew(HCI_AUTH_REQ_T);
    p_prim->handle = handle;

    send_sm_to_hci((HCI_UPRIM_T*)p_prim, HCI_AUTH_REQ);
}

/*! \brief Turn encryption on or off for a connection.

    \param handle ACL handle of existing connection to remote device.
    \param enc_enable Whether to turn encryption on or off.
*/
static void send_hci_set_conn_encryption(const hci_connection_handle_t handle,
                                                    const hci_link_enc_t enc_enable)
{
    HCI_SET_CONN_ENCRYPTION_T *p_prim;

    p_prim = zpnew(HCI_SET_CONN_ENCRYPTION_T);
    p_prim->handle = handle;
    p_prim->enc_enable = enc_enable;

    send_sm_to_hci((HCI_UPRIM_T*)p_prim, HCI_SET_CONN_ENCRYPTION);
}

#ifndef DISABLE_DM_BREDR
/*! \brief Change option to encrypt during link setup.

    \param mode Encryption mode.
*/
static void send_hci_write_enc_mode(const CsrUint8 mode)
{
    HCI_WRITE_ENC_MODE_T *p_prim;

    p_prim = pnew(HCI_WRITE_ENC_MODE_T);
    p_prim->mode = mode;

    send_sm_to_hci((HCI_UPRIM_T*)p_prim, HCI_WRITE_ENC_MODE);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Turn Simple pairing on in the controller.

    \param simple_pairing_mode
*/
static void send_hci_write_simple_pairing_mode(const CsrUint8 simple_pairing_mode)
{
    HCI_WRITE_SIMPLE_PAIRING_MODE_T *p_prim;

    p_prim = pnew(HCI_WRITE_SIMPLE_PAIRING_MODE_T);

    p_prim->simple_pairing_mode = simple_pairing_mode;

    send_sm_to_hci((HCI_UPRIM_T*)p_prim, HCI_WRITE_SIMPLE_PAIRING_MODE);
}
#endif

/*! \brief Reply to controller with host's IO capabilities.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param p_sm_ssp Pointer to IO capability parameters.
*/
static void send_hci_io_capability_response(const TYPED_BD_ADDR_T *const addrt,
                                            DM_SM_SSP_T *const p_sm_ssp)
{
    HCI_IO_CAPABILITY_RESPONSE_T *p_prim;

    p_prim = pnew(HCI_IO_CAPABILITY_RESPONSE_T);

    tbdaddr_copy_to_bd_addr(&p_prim->bd_addr, addrt);

    p_prim->io_capability = p_sm_ssp->io_cap_local;
    p_prim->oob_data_present = p_sm_ssp->oob_hash_c == NULL ? 0 : 1;

    if ((SMCB.config & DM_SM_SEC_MODE_CONFIG_ALWAYS_TRY_MITM) != 0)
    {
        /* Application always wants to try for MITM protection, but not
           enforce it. So sneak it in here at the last minute and pretend
           that the remote device has requested it, because that will get
           us to do the right thing during the USER_CONFIRMATION bit.
         */
        p_sm_ssp->mitm_remote |= DM_SM_AUTH_REQ_MITM;
    }

    p_prim->authentication_requirements = p_sm_ssp->mitm_local;

    send_sm_to_hci((HCI_UPRIM_T*)p_prim, HCI_IO_CAPABILITY_RESPONSE);
}

/*! \brief Reject IO capabilities request and stop pairing.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param reason Reason for rejecting IO capabilities request.
*/
void send_hci_io_capability_neg_response(const BD_ADDR_T *const p_bd_addr,
                                         const hci_error_t reason)
{
    HCI_IO_CAPABILITY_REQUEST_NEG_REPLY_T *p_prim;

    p_prim = pnew(HCI_IO_CAPABILITY_REQUEST_NEG_REPLY_T);

    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);
    p_prim->reason = reason;

    send_sm_to_hci((HCI_UPRIM_T*)p_prim, HCI_IO_CAPABILITY_REQUEST_NEG_REPLY);
}

static void send_sm_to_hci(HCI_UPRIM_T *hci_uprim, const hci_op_code_t op_code)
{
    hci_uprim->hci_cmd.op_code = op_code;
    hci_uprim->hci_cmd.length = DM_HCI_SENDER_SECURITY_MANAGER;

    send_to_hci((DM_UPRIM_T *)hci_uprim);
}

/*! \brief Add a new request structure to the tail of the request
           queue for the supplied ACL instance.

    \param p_sm Pointer to ACL record structure.
    \returns Pointer to request structure, or NULL on failure.
*/
static DM_SM_REQUEST_T *add_sm_request(DM_ACL_T *p_acl)
{
    DM_SM_REQUEST_T *p_req;

    DM_ACL_DEBUG_CHECK(p_acl);

    if ((p_req = xzpnew(DM_SM_REQUEST_T)) != NULL)
    {
    DM_SM_REQUEST_T **pp_list, *p;

        for(pp_list = &p_acl->dm_acl_client_sm.p_requests; (p = *pp_list) != NULL;
                                                    pp_list = &p->p_next)
        /* nothing */ ;

        *pp_list = p_req;
    }

    return p_req;
}

/*! \brief Decides whether the next request in the list for
           this ACL can be started.

    \param p_acl Pointer to ACL record structure.
*/
static void kick_request_process(DM_ACL_T *p_acl)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    if (p_acl->dm_acl_client_sm.state == SM_ST_IDLE
            && p_acl->dm_acl_client_sm.p_requests != NULL)
    {
        fsm_event_t event = FSM_EVENT_NULL;

        /* Convert the request type into an event for the state machine */
        switch (p_acl->dm_acl_client_sm.p_requests->u.type)
        {
            case DM_SM_ACCESS_REQ:
                event = SM_EV_ACCESS_REQ;
                break;

            case DM_SM_AUTHENTICATE_REQ:
                event = SM_EV_AUTHENTICATE_REQ;
                break;

            case DM_SM_ENCRYPT_REQ:
                event = SM_EV_ENCRYPT_REQ;
                break;

            case DM_SM_BONDING_REQ:
                event = SM_EV_BONDING_REQ;
                /*break;*/

            /* NOTE: DM_SM_BONDING_CANCEL_REQ is never queued
                     and so doesn't appear here */

            /* default: TODO Unknown SM request. Should we panic here? */
        }

        /* Start the state machine */
        sm_state_machine(p_acl, event, NULL);
    }
}

/*! \brief Transaction holdoff timer has fired.

    It should now be safe to send whatever event we were holding off.

    \param event FSM event to be sent.
    \param ptr Pointer to ACL record structure cast to void*.
*/
static void dm_sm_retry_transaction(CsrUint16 event, void *ptr)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)ptr;

    DM_ACL_DEBUG_CHECK(p_acl);

    TIMER_EXPIRED(p_acl->dm_acl_client_sm.p_requests->transaction_holdoff_timer);

    sm_state_machine(p_acl, (fsm_event_t)event, NULL);
}

/*! \brief Timed event to detach an ACL.

    We make sure that we still want to disconnect and then go for it.

    \param ptr Pointer to ACL record structure cast to (void*).
    \param unused unused.
*/
static void detach_after_delay(CsrUint16 unused, void *ptr)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)ptr;

    CSR_UNUSED(unused);

    DM_ACL_DEBUG_CHECK(p_acl);

    TIMER_EXPIRED(p_acl->dm_acl_client_sm.p_requests->detach_timer);

    dm_acl_close(p_acl, dm_acl_client_max, HCI_ERROR_AUTH_FAIL,
            ACL_IDLE_TIMEOUT_AT_MOST_NONE);
}

/*! \brief Given a bluetooth address, determines whether we require an
           authenticated link key.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \returns HCI_MITM_NOT_REQUIRED_NO_BONDING or HCI_MITM_REQUIRED_NO_BONDING.
*/
CsrUint8 auth_key_required(const TYPED_BD_ADDR_T *const addrt)
{
    DM_ACL_T *p_acl;
    DM_SM_REQUEST_T *p_req;

    /* Are we in mode 4? */
    if (SMCB.security_mode == SEC_MODE4_SSP)
    {
        /* Does an ACL exist? */
        if ((p_acl = dm_acl_find_by_tbdaddr(addrt)) != NULL)
        {
            /* If we already have an authenticated link key then it would
               be dangerous to downgrade */
            if (dm_sm_get_device_link_key(addrt, NULL)
                    == DM_SM_LINK_KEY_AUTHENTICATED)
                return HCI_MITM_REQUIRED_NO_BONDING;

            /* Is this an access request? */
            if ((p_req = p_acl->dm_acl_client_sm.p_requests) != NULL
                    && p_req->u.type == DM_SM_ACCESS_REQ
                    && tbdaddr_eq(
                        addrt,
                        &p_req->u.access_req.conn_setup.connection.addrt))
            {
                if ((p_req->seclevel & SECL4_MITM) != 0)
                    return HCI_MITM_REQUIRED_NO_BONDING;

                return HCI_MITM_NOT_REQUIRED_NO_BONDING;
            }

            /* If it's not an access request then we must be the responder.
               We don't know what service they require so we only go
               for MITM protection if all of our services need it. */
            if ((SMCB.secl_default & SECL4_IN_MITM) != 0)
            {
                return dm_sm_mitm_services(SECL4_IN_MITM)
                            == DM_SM_MITM_SERVICES_ALL
                    ? HCI_MITM_REQUIRED_NO_BONDING
                    : HCI_MITM_NOT_REQUIRED_NO_BONDING;
            }
        }
    }

    return HCI_MITM_NOT_REQUIRED_NO_BONDING;
}

/*! \brief Determines authentication requirements for dedicated bonding.

    For dedicated bonding, we are bonding all of our services with the
    remote device. So we look through them all and if there's at least
    one service that requires MITM protection, then we insist on having
    it, otherwise we don't.

    \returns HCI_MITM_REQUIRED_DEDICATED_BONDING or
             HCI_MITM_NOT_REQUIRED_DEDICATED_BONDING.
*/
static CsrUint8 bonding_auth_req(void)
{
    /* Require MITM if default or any service requires it. */
    return ((SMCB.secl_default & SECL4_MITM) == 0
            && dm_sm_mitm_services(SECL4_MITM) == DM_SM_MITM_SERVICES_NONE)
        ? HCI_MITM_NOT_REQUIRED_DEDICATED_BONDING
        : HCI_MITM_REQUIRED_DEDICATED_BONDING;
}

/*! \brief Given a bluetooth address, determines whether we allow legacy
           security procedures.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \returns TRUE if we allow legacy pairing, FALSE if we don't.
*/
CsrBool legacy_security_ok(const TYPED_BD_ADDR_T *const addrt)
{
    DM_ACL_T *p_acl;
    DM_SM_REQUEST_T *p_req;
    dm_security_level_t seclevel;

    if (SMCB.security_mode == SEC_MODE4_SSP)
    {
        seclevel = SMCB.secl_default;
        /* Is this an access request? */
        if ((p_acl = dm_acl_find_by_tbdaddr(addrt)) != NULL
                && (p_req = p_acl->dm_acl_client_sm.p_requests) != NULL
                && p_req->u.type == DM_SM_ACCESS_REQ)
            seclevel = p_req->seclevel;

        /* Otherwise use default */
        return (seclevel & (SECL4_NO_LEGACY)) == 0;
    }

    return TRUE;
}

static CsrBool dm_sm_is_legacy(DM_ACL_T *p_acl)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    /* Note that DM_SM_FLAG_RSF_RHSF_SSP is multi-bit and all must be set
       for SSP support. */
    return SMCB.security_mode != SEC_MODE4_SSP
        || (p_acl->dm_acl_client_sm.flags & DM_SM_FLAG_RSF_RHSF_SSP)
            != DM_SM_FLAG_RSF_RHSF_SSP;
}

void dm_sm_acl_reset(DM_SM_T *p_sm)
{
    dm_sm_remove_device(DM_ACL_CLIENT_GET_TBDADDR(p_sm,dm_acl_client_sm), TRUE);
    p_sm->state = SM_ST_IDLE;
    p_sm->encrypted = FALSE;
}

static DM_ACL_RESULT_T dm_sm_acl_open(const TYPED_BD_ADDR_T *const addrt,
                                      const CsrBool incoming,
                                      DM_ACL_T **pp_acl)
{
    DM_SM_SERVICE_T service = { SEC_PROTOCOL_SM_BONDING, 0 };
    CsrUint16 flags = (CsrUint16)(incoming ? (DM_ACL_CONNECTION_BR_EDR | DM_ACL_INIT_PEER)
                                         : (DM_ACL_CONNECTION_BR_EDR | DM_ACL_INIT_LOCAL));

    return dm_acl_open(addrt, &service, dm_acl_client_sm, flags, pp_acl);
}

static void dm_sm_acl_close(DM_ACL_T *const p_acl,
                            hci_reason_t reason,
                            const ACL_IDLE_TIMEOUT_MULTIPLIER_T idle_multiplier)
{
    DM_ACL_DEBUG_CHECK(p_acl);
    dm_acl_close(p_acl, dm_acl_client_sm, reason, idle_multiplier);
}

static void dm_sm_acl_set_timeout(DM_ACL_T *p_acl)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    /* If we're initiator then we don't need a timeout as the ACL
       will already have been locked elsewhere. If we're responder then
       the timeout will depend on whether it is an SM4 pairing or a legacy
       pairing. For SM4 pairing, we can get away with a medium timeout as
       we tend to have a better idea of when pairing starts and ends. For
       legacy pairing we use a long timeout as we can never be sure when
       we might get a pin code request. */
    if (p_acl->dm_acl_client_sm.state != SM_ST_IDLE)
        dm_acl_idle_timeout_change(p_acl,
                                   dm_sm_acl_timeout_security(p_acl),
                                   HCI_ERROR_OETC_USER);
}

ACL_IDLE_TIMEOUT_MULTIPLIER_T dm_sm_acl_timeout_security(DM_ACL_T *p_acl)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    /* If we're encrypted then we can use a very small timeout. */
    if (p_acl->dm_acl_client_sm.encrypted)
        return ACL_IDLE_TIMEOUT_AT_LEAST_SHORT;

    /* Medium timeout if we're both SM4, otherwise long timeout
       to allow for PIN code entry. Note that DM_SM_FLAG_RSF_RHSF_SSP
       is multi-bit and all must be set for SSP support. */
    if ((p_acl->dm_acl_client_sm.flags & DM_SM_FLAG_RSF_RHSF_SSP)
                == DM_SM_FLAG_RSF_RHSF_SSP
            && SMCB.security_mode == SEC_MODE4_SSP)
        return ACL_IDLE_TIMEOUT_AT_LEAST_MEDIUM;

    return ACL_IDLE_TIMEOUT_AT_LEAST_LONG;
}

/*! \brief Send encryption key refresh request to HCI.

    \param p_acl Pointer to ACL record structure
*/
static void send_hci_refresh_encryption_key(DM_ACL_T *p_acl)
{
    HCI_REFRESH_ENCRYPTION_KEY_T *p_prim;

    DM_ACL_DEBUG_CHECK(p_acl);

    p_prim = pnew(HCI_REFRESH_ENCRYPTION_KEY_T);
    p_prim->handle = p_acl->handle;
    tbdaddr_copy(&p_prim->addrt, &p_acl->addrt);
    send_sm_to_hci((HCI_UPRIM_T*)p_prim, HCI_REFRESH_ENCRYPTION_KEY);
}

#ifndef DISABLE_DM_BREDR
/*! \brief Reply to HCI_USER_CONFIRMATION_REQUEST

    \param opcode
    \param TYPED_BD_ADDR_T *addrt
*/
static void send_hci_user_confirmation_request_reply(
        hci_op_code_t opcode,
        const TYPED_BD_ADDR_T *const addrt
        )
{
    HCI_USER_CONFIRMATION_REQUEST_REPLY_T *p_prim;

    p_prim = pnew(HCI_USER_CONFIRMATION_REQUEST_REPLY_T);
    (void)tbdaddr_copy_to_bd_addr(&p_prim->bd_addr, addrt);
    send_sm_to_hci((HCI_UPRIM_T*)p_prim, HCI_USER_CONFIRMATION_REQUEST_REPLY);
}
#endif

/*! \brief Free a security manager request.

    \param pp_req Pointer to pointer to request structure.
*/
static void dm_sm_free_request(DM_SM_REQUEST_T **pp_req)
{
    DM_SM_REQUEST_T *p_req = *pp_req;

    timer_cancel(&p_req->detach_timer);
    timer_cancel(&p_req->transaction_holdoff_timer);
    *pp_req = p_req->p_next;
    CsrPmemFree(p_req);
}

/*! \brief Guess whether we initiated the authentication/pairing
           that is causing current security-related events from HCI.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \returns TRUE if we think we initiated this, otherwise FALSE.
*/
static CsrBool dm_sm_is_initiator(const TYPED_BD_ADDR_T *const addrt)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_tbdaddr(addrt)) != NULL)
    {
        switch (p_acl->dm_acl_client_sm.state)
        {
            case SM_ST_AUTHENTICATING:
            case SM_ST_ENCRYPTING_AUTH:
            case SM_ST_ACCESS_AUTHENTICATING:
            case SM_ST_BONDING:
                return TRUE;
        }
    }

    return FALSE;
}

void dm_sm_init(void)
{
    dm_sm_device_db_init();
    dm_sm_service_db_init();
    SMCB.change = NULL;

    SMCB.config = 0;
    SMCB.write_auth_enable = 0;

    SMCB.security_mode = SEC_MODE0_OFF;

    SMCB.secl_default = SECL_DEFAULT;
}

#ifdef ENABLE_SHUTDOWN
void dm_sm_deinit(void)
{
    dm_sm_unregister_device_record();
    dm_sm_unregister_service_record();
    CsrPmemFree(SMCB.change);
    dm_sm_init();
}

/*! \brief Cleans up pending application/protocol requests.

    Used when deinit for DM is called.
*/
void dm_acl_client_deinit_sm(DM_SM_T *p_sm, CsrBool is_ble)
{
    if (is_ble)
    {
        sm_free(p_sm->u.p_sm);
        p_sm->u.p_sm = NULL;
    }
    else if (p_sm->u.p_ssp != NULL)
    {
        CsrPmemFree(p_sm->u.p_ssp->oob_hash_c);
        CsrPmemFree(p_sm->u.p_ssp->oob_rand_r);
        CsrPmemFree(p_sm->u.p_ssp);
        p_sm->u.p_ssp = NULL;
    }

    while (p_sm->p_requests != NULL)
        dm_sm_free_request(&p_sm->p_requests);
}

#endif /* ENABLE_SHUTDOWN */
#endif /* DISABLE_DM_BREDR */

/*! \brief Common code for sending upstream primitives.

    We fill in am_phandle and, possibly, the Bluetooth address.

    \param p_uprim Pointer to primitive cast to (DM_UPRIM_T*).
    \param p_bd_addr Pointer to Bluetooth address or NULL.
*/
void send_dm_sm_common(DM_UPRIM_T *p_uprim,
                       const TYPED_BD_ADDR_T *const addrt)
{
    if (addrt != NULL)
        tbdaddr_copy(&p_uprim->dm_upstream_typed_bd_addr_common.addrt, addrt);

    DM_SendMessageAMphandle(p_uprim);
}

/*! \brief Issue a DM_SM_ENCRYPTION_CHANGE_IND primitive.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param encrypted Encryption state of link.
*/
static void send_dm_sm_encryption_change(const TYPED_BD_ADDR_T *const addrt, const CsrBool encrypted)
{
    DM_SM_ENCRYPTION_CHANGE_IND_T *p_prim;

    p_prim = pnew(DM_SM_ENCRYPTION_CHANGE_IND_T);
    p_prim->type = DM_SM_ENCRYPTION_CHANGE_IND;
    p_prim->encrypted = encrypted;

    send_dm_sm_common((DM_UPRIM_T *)p_prim, addrt);
}

CsrBool dm_sm_auto_accept_just_works(void)
{
#ifdef DISABLE_DM_BREDR
    return TRUE;
#else
    return (SMCB.config & DM_SM_SEC_MODE_CONFIG_NEVER_AUTO_ACCEPT) == 0;
#endif
}

#ifdef DISABLE_DM_BREDR

/*! \brief Call the BLE Security Manager and issue the encryption changed
           primitive to the host.
*/
void dm_sm_hci_enc_complete(
        const hci_connection_handle_t handle,
        CsrUint8 status,
        const CsrBool encrypted)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_handle(handle)) != NULL)
    {
        send_dm_sm_encryption_change(&p_acl->addrt, encrypted);
        sm_encryption_changed(handle, encrypted, status);
    }
}

/*! \brief Handle refresh encryption key complete event.

    \param handle HCI connection handle.
    \param status HCI return/error code.
*/
void dm_sm_hci_refresh_encryption_key_complete(hci_connection_handle_t handle,
                                               hci_return_t status)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_handle(handle)) != NULL)
    {
        DM_SendTBDADDRCommandComplete(DM_HCI_REFRESH_ENCRYPTION_KEY_IND,
                                      status,
                                      &p_acl->addrt);
        sm_encryption_changed(handle, (CsrBool)(status == HCI_SUCCESS), status);
    }
}
#endif /* DISABLE_DM_BREDR */

/*============================================================================*
    End Of File
 *============================================================================*/


#ifndef _DM_ACL_COMMON_H_
#define _DM_ACL_COMMON_H_

#include "csr_synergy.h"
/*!
 Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
     All rights reserved.

\file   dm_acl_policy_manager.c

\brief  The core of the ACL Manager part of the Device Manager.
*/

#include "csr_bt_core_stack_fsm.h"
#include "csr_bt_usr_config.h"
#include <stddef.h>

/* Includes for client definitions. */
#include "l2cap_types.h"                /* L2CAP */
#include "dm_sm_types.h"                /* Security Manager */
#include "dm_acl_policy_types.h"        /* Policy Manager */
#include "dm_sync_types.h"              /* Sync Manager */
#include "dm_acl_amp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Client Index.

   This must increase monotonically from zero.
   Many places in the code assume this:
        1) The type is used as an index in 'for' loops.
        2) The type is used in a shift operation to define bit-masks.
*/
typedef enum
{
    dm_acl_client_first = 0,
    dm_acl_client_sm = 0,
    dm_acl_client_application,
    dm_acl_client_l2cap,
#ifdef INSTALL_DM_SYNC_MODULE
    dm_acl_client_sync,
#endif
#ifdef INSTALL_AMP_SUPPORT
    dm_acl_client_amp,
#endif
#ifndef DISABLE_DM_BREDR
    dm_acl_client_policy,
#endif
    dm_acl_client_max               /* N.B. Keep this at the end. */
} DM_ACL_CLIENT_T;

#if defined(INSTALL_DM_SYNC_MODULE) && defined(INSTALL_AMP_SUPPORT)
typedef CsrUint32 dm_acl_client_locks_t;
#else
typedef CsrUint16 dm_acl_client_locks_t;
#endif

#define ACL_IDLE_TIMEOUT_AT_MOST 0x4000

typedef enum
{
    ACL_IDLE_TIMEOUT_AT_LEAST_NONE       = 0,
    ACL_IDLE_TIMEOUT_AT_LEAST_TINY       = DM_CONFIG_ACL_IDLE_TIMEOUT_TINY,
    ACL_IDLE_TIMEOUT_AT_LEAST_SHORT      = DM_CONFIG_ACL_IDLE_TIMEOUT_SHORT,
    ACL_IDLE_TIMEOUT_AT_LEAST_MEDIUM     = DM_CONFIG_ACL_IDLE_TIMEOUT_MEDIUM,
    ACL_IDLE_TIMEOUT_AT_LEAST_LONG       = DM_CONFIG_ACL_IDLE_TIMEOUT_LONG,
    ACL_IDLE_TIMEOUT_AT_LEAST_BONDING    = DM_CONFIG_ACL_IDLE_TIMEOUT_BONDING,
    ACL_IDLE_TIMEOUT_AT_MOST_NONE        = (ACL_IDLE_TIMEOUT_AT_MOST | 0),
    ACL_IDLE_TIMEOUT_AT_MOST_TINY        = (ACL_IDLE_TIMEOUT_AT_MOST | DM_CONFIG_ACL_IDLE_TIMEOUT_TINY),
    ACL_IDLE_TIMEOUT_AT_MOST_SHORT       = (ACL_IDLE_TIMEOUT_AT_MOST | DM_CONFIG_ACL_IDLE_TIMEOUT_SHORT),
    ACL_IDLE_TIMEOUT_AT_MOST_MEDIUM      = (ACL_IDLE_TIMEOUT_AT_MOST | DM_CONFIG_ACL_IDLE_TIMEOUT_MEDIUM),
    ACL_IDLE_TIMEOUT_AT_MOST_LONG        = (ACL_IDLE_TIMEOUT_AT_MOST | DM_CONFIG_ACL_IDLE_TIMEOUT_LONG),
    ACL_IDLE_TIMEOUT_AT_MOST_BONDING     = (ACL_IDLE_TIMEOUT_AT_MOST | DM_CONFIG_ACL_IDLE_TIMEOUT_BONDING)
} ACL_IDLE_TIMEOUT_MULTIPLIER_T;

/* Extra Mode for when we don't give a damn. */
#define HCI_BT_MODE_ANY                         ((hci_bt_mode_t)0x07)

/* DM_ACL_T Flags. TODO. See if we really need all of these. */
/* Role flag */
#define DM_ACL_ROLE                             ((CsrUint16) 0x0001) /* DM_ACL_ROLE_MASTER or DM_ACL_ROLE_SLAVE. */

/* Special connection flags */
#define DM_ACL_CONNECTION_ENUM(value)           ((CsrUint16)(value) << L2CA_CONFLAG_ENUM_OFFSET )

/* Disconnect Flag */
#define DM_ACL_DISCONNECT_RADIO_TYPE            ((CsrUint16) 0x0040)

#define DM_ACL_CONNECTION_MASK                  ((CsrUint16)0x001C)

/** Modeled the 3rd nibble and 1st bit of the 4th bibble according to  \ref l2ca_conflags_t */
#define DM_ACL_CONNECTION_LE_ADVERT_TIMEOUT     L2CA_CONFLAG_PAGE_TIMEOUT                                   /*! < Page timeout for adverts or Timeouts for connect */
#define DM_ACL_CONNECTION_BR_EDR                DM_ACL_CONNECTION_ENUM(L2CA_CONNECTION_BR_EDR)              /*!< Channel to be used for UCD */
#define DM_ACL_CONNECTION_BR_EDR_BROADCAST      DM_ACL_CONNECTION_ENUM(L2CA_CONNECTION_BR_EDR_BROADCAST)    /*!< Channel to be used for broadcast connectionless */
#define DM_ACL_CONNECTION_LE_MASTER_DIRECTED    DM_ACL_CONNECTION_ENUM(L2CA_CONNECTION_LE_MASTER_DIRECTED)  /*!< Connect as Master */
#define DM_ACL_CONNECTION_LE_MASTER_WHITELIST   DM_ACL_CONNECTION_ENUM(L2CA_CONNECTION_LE_MASTER_WHITELIST) /*!< Connect as Master using whitelist */
#define DM_ACL_CONNECTION_LE_SLAVE_DIRECTED     DM_ACL_CONNECTION_ENUM(L2CA_CONNECTION_LE_SLAVE_DIRECTED)   /*!< Start directed adverts */
#define DM_ACL_CONNECTION_LE_SLAVE_WHITELIST    DM_ACL_CONNECTION_ENUM(L2CA_CONNECTION_LE_SLAVE_WHITELIST)  /*!< Use whitelist for undirected adverts */
#define DM_ACL_CONNECTION_LE_SLAVE_UNDIRECTED   DM_ACL_CONNECTION_ENUM(L2CA_CONNECTION_LE_SLAVE_UNDIRECTED) /*!< Start undirected adverts */
/* Seperation threshold for BR/EDR and LE flags */
#define DM_ACL_BR_EDR_FLAG_THRESHOLD            DM_ACL_CONNECTION_ENUM(L2CA_CONNECTION_LE_MASTER_DIRECTED - 1)

/* Mode / Policy flags */
#define DM_ACL_MODE_CHANGE_REQUESTED            ((CsrUint16) 0x0080) /* A mode change request for this ACL has been sent to the controller. */
#define DM_ACL_SCO_PENDING                      ((CsrUint16) 0x0100) /* A SCO link (associated with this ACL) is being connected. */
#define DM_ACL_RSF_SNIFF                        ((CsrUint16) 0x0200) /* The Remote device supports sniff mode. */
#define DM_ACL_BLOCK_POLICY_MANAGER             ((CsrUint16) 0x0400) /* The policy manager holdoff timer active. */
#define DM_ACL_ROLE_SWITCH_MASTER               ((CsrUint16) 0x0800) /* Request sent to controller to switch role to master. */
#define DM_ACL_ROLE_SWITCH_SLAVE                ((CsrUint16) 0x1000) /* Request sent to controller to switch role to slave. */
#define DM_ACL_ROLE_SWITCH_PENDING (DM_ACL_ROLE_SWITCH_MASTER | DM_ACL_ROLE_SWITCH_SLAVE) /* Roleswitch request sent. */

/* ACL Connect/Disconnect Flags */
#define DM_ACL_WAE                              ((CsrUint16) 0x2000) /* Authentication during link-setup required for this link. */
#define DM_ACL_DISCONNECT                       ((CsrUint16) 0x4000) /* We've had a request to close the link. */
#define DM_ACL_INITIATOR                        ((CsrUint16) 0x8000) /* DM_ACL_INIT_LOCAL or DM_ACL_INIT_PEER. */
#define DM_ACL_DISCONNECT_IS_LE                 DM_ACL_DISCONNECT_RADIO_TYPE /* Indication of LE link disconnection */
#define DM_ACL_DISCONNECT_IS_BR_EDR             ((CsrUint16) 0x0000) /* Indication of BR/EDR link disconnection */

#define DM_ACL_ROLE_MASTER                      ((CsrUint16) 0x0000) /* We're master. */
#define DM_ACL_ROLE_SLAVE                       DM_ACL_ROLE         /* We're slave. */
#define DM_ACL_INIT_LOCAL                       ((CsrUint16) 0x0000) /* We created the connection. */
#define DM_ACL_INIT_PEER                        DM_ACL_INITIATOR    /* They created the connection. */
#define DM_ACL_INTERFACE_MASK                   (DM_ACL_CONNECTION_MASK | DM_ACL_CONNECTION_LE_ADVERT_TIMEOUT | DM_ACL_INITIATOR)

/* ACL Connection instance. */
typedef struct DM_ACL_T_tag
{
    struct DM_ACL_T_tag *p_next;
#ifdef DM_ACL_DEBUG
    CsrUint16 sanity;
#endif
    hci_connection_handle_t handle;     /* HCI connection handle */
    TYPED_BD_ADDR_T addrt;                      /* Peer BD address */
    CsrUint24 dev_class;                 /* Peer device class (if peer-init) */
    fsm_state_t connection_state;
    CsrUint16 flags;                     /* Bitmap of flags for this ACL */
    dm_acl_client_locks_t locks;        /* We know that policy never locks ACL... */

/* Everything beyond this point will be zeroed and reset upon ACL closure.
   All memory must already have been freed by that point.
   We do this to return us to a default state in case we receive a
   new connect request before we have a chance to delete the structure
   and have to reuse the existing one. */
#define DM_ACL_RESET_START(a) (&(a)->pending_credits)
#define DM_ACL_RESET_SIZE (sizeof(DM_ACL_T) - offsetof(DM_ACL_T, pending_credits))

    CsrUint16 pending_credits;           /* Pending credits for sent data on dying ACL. */
    hci_bt_mode_t current_mode;
    CsrSchedTid disconnect_timer_id;          /* Idle timer ID. */
    /* Number of times to re-run the idle timer before disconnecting. */
    CsrUint16 disconnect_timer_repeats;
#ifdef INSTALL_ULP
    CsrSchedTid   advert_timeout_timer_id;    /* Advert timer ID */
    DM_ACL_BLE_CONN_PARAMS_T *ble_conn_params; /* BLE connection parameters */
#endif
    /* Client Data */
    DM_SM_T dm_acl_client_sm;           /* Security Manager */
    L2CAP_CHCB_T dm_acl_client_l2cap;   /* L2CAP */
#ifdef INSTALL_DM_SYNC_MODULE
    DM_SYNC_T *dm_acl_client_sync;      /* Sync Manager */
#endif
#ifdef INSTALL_AMP_SUPPORT
    DM_AMP_T dm_acl_client_amp;         /* AMPM Interface */
#endif
#ifndef DISABLE_DM_BREDR
    DM_POLICY_T dm_acl_client_policy;   /* Policy Manager */
#endif
} DM_ACL_T;

/* Count number of locks for specified client. */
#define DM_ACL_CLIENT_LOCKS(p_acl, client) \
    (((p_acl)->locks >> ((client) << 2)) & 0xF)

#ifndef DM_ACL_DEBUG

/* Given pointer to ACL and client name, returns pointer to client data */
#define DM_ACL_CLIENT_GET_DATA(p_acl, client) (&(p_acl)->client)

/* Given pointer to client data and client name, returns pointer to ACL */
#define DM_ACL_CLIENT_GET_ACL(client_data, client) \
    ((DM_ACL_T*)(((CsrUint8*)(client_data)) - offsetof(DM_ACL_T, client)))

#define DM_ACL_DEBUG_CHECK(p_acl)
#define DM_ACL_DEBUG_CHECK_ALLOW_DEAD(p_acl)

#else /* DM_ACL_DEBUG */

#ifndef DM_ACL_DEBUG_CHECK
/* DEBUG Perform sanity checks on pointer to ACL record structure. */
void dm_acl_debug_check(char *file, CsrUint16 line, const DM_ACL_T *p_acl_check, CsrBool allow_dead);
#define DM_ACL_DEBUG_CHECK(p_acl) dm_acl_debug_check(__FILE__, __LINE__, p_acl, FALSE)
#define DM_ACL_DEBUG_CHECK_ALLOW_DEAD(p_acl) dm_acl_debug_check(__FILE__, __LINE__, p_acl, TRUE)
#endif /* DM_ACL_DEBUG_CHECK */

#ifndef DM_ACL_CLIENT_GET_DATA
/* DEBUG Given pointer to ACL and client name, returns pointer to client data */
void *dm_acl_client_get_data(char *file, CsrUint16 line, DM_ACL_T *p_acl, void *client);
#define DM_ACL_CLIENT_GET_DATA(p_acl, client) \
        dm_acl_client_get_data(__FILE__, __LINE__, (p_acl), &(p_acl)->client)
#endif /* DM_ACL_CLIENT_GET_DATA */

#ifndef DM_ACL_CLIENT_GET_ACL
/* DEBUG Given pointer to client data and client name, returns pointer to ACL */
DM_ACL_T *dm_acl_client_get_acl(char *file, CsrUint16 line, void *client, CsrUint16 offset);
#define DM_ACL_CLIENT_GET_ACL(client_data, client) \
    dm_acl_client_get_acl(__FILE__, __LINE__, client_data, offsetof(DM_ACL_T, client))
#endif /* DM_ACL_CLIENT_GET_ACL */

#endif /* DM_ACL_DEBUG */

/* Given pointer to client data and client name, returns pointer to BD_ADDR */
#define DM_ACL_CLIENT_GET_TBDADDR(client_data, client) \
        (&DM_ACL_CLIENT_GET_ACL(client_data, client)->addrt)
/* Given pointer to client data and client name, returns handle */
#define DM_ACL_CLIENT_GET_HANDLE(client_data, client) \
        (DM_ACL_CLIENT_GET_ACL(client_data, client)->handle)

/* Pointer to first ACL record */
#define DM_ACL_FIRST() p_acl_list

/* Get the latest time */
#define DM_ACL_POLICY_UPDATE_LAST_ACL_TIME(p_acl) \
        ((p_acl)->dm_acl_client_policy.last_acl_time = get_deci_time())

typedef void (*DM_ACL_CALLBACK_OPENED_T)(DM_ACL_T *p_acl, CsrUint8 status);
typedef CsrUint16 (*DM_ACL_CALLBACK_CLOSED_T)(DM_ACL_T *p_acl, CsrUint16 reason);
typedef void (*DM_ACL_CALLBACK_MODE_CHANGED_T)(DM_ACL_T *p_acl, CsrUint8 status, CsrUint16 length);

/* TODO move to private header file */
extern DM_ACL_T *p_acl_list;

/* DM Init/Deinit */
void dm_acl_init(void);
#ifdef ENABLE_SHUTDOWN
void dm_acl_deinit(void);
#endif

/* ACL record structure creation/destruction. */
DM_ACL_T *dm_acl_new(const TYPED_BD_ADDR_T *const addrt, const CsrUint16 flags);
void dm_acl_cleanup(void);

/* ACL lookup. */
DM_ACL_T *dm_acl_find_by_tbdaddr(const TYPED_BD_ADDR_T *const addrt);
DM_ACL_T *dm_acl_find_by_bd_addr(const BD_ADDR_T *const p_bd_addr);
DM_ACL_T *dm_acl_find_connected_by_bd_addr(const BD_ADDR_T *const p_bd_addr);
DM_ACL_T *dm_acl_find_by_handle(hci_connection_handle_t handle);
CsrBool dm_acl_exists(DM_ACL_T *p_acl);
#ifdef INSTALL_ULP
DM_ACL_T * dm_acl_find_white_list_or_undirected(void);
#endif


/* ACL data lookup/conversion. */
void dm_acl_get_bd_addr_by_handle(BD_ADDR_T *const addrt, const hci_connection_handle_t handle);
CsrUint24 dm_acl_get_cod_by_bd_addr(const BD_ADDR_T *const bd_addr);
#define DM_ACL_GET_CONNECTION_FLAGS(p_acl) ((CsrUint16)((p_acl)->flags & DM_ACL_CONNECTION_MASK))
#ifdef INSTALL_ULP
CsrBool dm_acl_is_ble(const DM_ACL_T *p_acl);
#else
#define dm_acl_is_ble(p_acl) FALSE
#endif

/* Callbacks */
void dm_acl_callback_opened(DM_ACL_T *const p_acl, CsrUint8 status);

/* The ACL Manager uses the top 8 bits of the reason code to communicate
   additional information about the disconnect. Currently this is used
   to tell clients that a given detach is a result of somebody using the
   DM_ACL_FORCE_DETACH_REQ primitive, or its equivalent. And also if the
   ACL is being reconnected for that particular client. The bottom 8 bits
   are used for the HCI reason code. */
#define DM_ACL_REASON_FORCE_DETACH 0x8000
#define DM_ACL_REASON_RECONNECTING_AND_LOCKED 0x4000
CsrUint16 dm_acl_callback_closed(DM_ACL_T *const p_acl, CsrUint16 reason);

/* ACL locking and unlocking by client */
void dm_acl_lock(DM_ACL_T *const p_acl, const DM_ACL_CLIENT_T client);
dm_acl_client_locks_t dm_acl_unlock(DM_ACL_T *const p_acl, const DM_ACL_CLIENT_T client);

/* DM_HCI interface. */
void dm_acl_send_to_hci(HCI_UPRIM_T *hci_uprim, hci_op_code_t op_code);
void dm_acl_send_to_hci_with_handle(HCI_UPRIM_T *hci_uprim, hci_op_code_t op_code);

#ifdef ENABLE_SHUTDOWN
void dm_acl_reset_con_flags(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _DM_ACL_COMMON_H_ */

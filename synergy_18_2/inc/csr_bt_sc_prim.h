#ifndef CSR_BT_SC_PRIM_H__
#define CSR_BT_SC_PRIM_H__

#include "csr_synergy.h"
/***************************************************************************************
    Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
            All rights reserved.


REVISION:      $Revision: #3 $
******************************************************************************************/

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "csr_bt_result.h"
#include "csr_bt_sc_db.h"
#include "dm_prim.h"
#include "csr_bt_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* search_string="CsrBtScPrim" */
/* conversion_rule="UPPERCASE_START_AND_REMOVE_UNDERSCORES" */

typedef CsrPrim                        CsrBtScPrim;
typedef CsrUint16                      CsrBtScTimeout;

/* CsrBtSc configuration flags. Unused bits are reserved, and must be set to zero. */
typedef CsrUint32                      CsrBtScConfigMask;

#define CSR_BT_SC_CONFIG_MASK_USE_STANDARD          ((CsrBtScConfigMask) 0x00000000)
#define CSR_BT_SC_CONFIG_MASK_FORCE_NAME_UPDATE     ((CsrBtScConfigMask) 0x00000001)
#define CSR_BT_SC_CONFIG_MASK_ALWAYS_TRY_MITM       ((CsrBtScConfigMask) 0x00000002)

/* ---------- Defines the Security Controller (SC) CsrBtResultCode ----------*/
#define CSR_BT_RESULT_CODE_SC_SUCCESS                ((CsrBtResultCode) (0x0000))
#define CSR_BT_RESULT_CODE_SC_BONDING_FAILED         ((CsrBtResultCode) (0x0001))
#define CSR_BT_RESULT_CODE_SC_BONDING_CANCELLED      ((CsrBtResultCode) (0x0002))
#define CSR_BT_RESULT_CODE_SC_SECURITY_FAIL          ((CsrBtResultCode) (0x0003))
#define CSR_BT_RESULT_CODE_SC_SECURITY_MODE_NOT_SET  ((CsrBtResultCode) (0x0004))
#define CSR_BT_RESULT_CODE_SC_INTERNAL_ERROR         ((CsrBtResultCode) (0x0005))
#define CSR_BT_RESULT_CODE_SC_ERROR_UNSPECIFIED      ((CsrBtResultCode) (0x0006))
#define CSR_BT_RESULT_CODE_SC_NO_SUCH_DEVICE         ((CsrBtResultCode) (0x0007))
#define CSR_BT_RESULT_CODE_SC_BONDING_ACCEPT_TIMEOUT ((CsrBtResultCode) (0x0008))

#define CSR_BT_SC_PAIRING_NONE                     ((CsrUint8) 0x00)
#define CSR_BT_SC_PAIRING_LEGACY                   ((CsrUint8) 0x01)
#define CSR_BT_SC_PAIRING_NO_MITM                  ((CsrUint8) 0x02)
#define CSR_BT_SC_PAIRING_MITM                     ((CsrUint8) 0x03)

/* Definitions for internal functions */
#define CSR_BT_SC_DB_OP_READ                       ((CsrUint8) 0x00)
#define CSR_BT_SC_DB_OP_WRITE                      ((CsrUint8) 0x01)
#define CSR_BT_SC_LE_AUTH_IGNORE                   ((CsrUint16)0xFFFF)

/* Database entries. Internally used values - may change at any time! */
#define CSR_BT_SC_DB_ENTRY_AMP_KEY_SSP             ((CsrUint8) 0x00)
#define CSR_BT_SC_DB_ENTRY_AMP_KEY_GAMP            ((CsrUint8) 0x01)
#define CSR_BT_SC_DB_ENTRY_AMP_KEY_DOT11           ((CsrUint8) 0x02)
#define CSR_BT_SC_DB_ENTRY_AMP_KEY_UWB             ((CsrUint8) 0x03)

/* Low energy connection method */
#define CSR_BT_SC_LE_CENTRAL                       L2CA_CONFLAG_ENUM(L2CA_CONNECTION_LE_MASTER_DIRECTED)
#define CSR_BT_SC_LE_PERIPHERAL                    L2CA_CONFLAG_ENUM(L2CA_CONNECTION_LE_SLAVE_DIRECTED)
#define CSR_BT_SC_LE_BREDR                         L2CA_CONFLAG_ENUM(L2CA_CONNECTION_BR_EDR)

/* Low energy key distribution */
#define CSR_BT_SC_LE_KEYDIST_ENCRYPTION            ((CsrUint16)(DM_SM_KEY_DIST_INITIATOR_ENC_CENTRAL \
                                                                | DM_SM_KEY_DIST_RESPONDER_ENC_CENTRAL))
#define CSR_BT_SC_LE_KEYDIST_ID                    ((CsrUint16)(DM_SM_KEY_DIST_INITIATOR_ID \
                                                                | DM_SM_KEY_DIST_RESPONDER_ID))
#define CSR_BT_SC_LE_KEYDIST_SIGN                  ((CsrUint16)(DM_SM_KEY_DIST_INITIATOR_SIGN \
                                                                | DM_SM_KEY_DIST_RESPONDER_SIGN))

/* Low energy security requirements */
#define CSR_BT_SC_LE_SECURITY_ENCRYPTION           (DM_SM_SECURITY_ENCRYPTION)
#define CSR_BT_SC_LE_SECURITY_MITM                 (DM_SM_SECURITY_MITM_PROTECTION)
/* For LE bonding, enable key distribution */

/*******************************************************************************
 * Primitive definitions
 *******************************************************************************/
#define CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST                        (0x0000)

#define CSR_BT_SC_REGISTER_REQ                     ((CsrBtScPrim) (0x0000 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_BOND_REQ                         ((CsrBtScPrim) (0x0001 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_DEBOND_REQ                       ((CsrBtScPrim) (0x0002 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_PASSKEY_RES                      ((CsrBtScPrim) (0x0003 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_SET_SECURITY_MODE_REQ            ((CsrBtScPrim) (0x0004 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_ENCRYPTION_REQ                   ((CsrBtScPrim) (0x0005 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_AUTHORISE_RES                    ((CsrBtScPrim) (0x0006 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_DEREGISTER_REQ                   ((CsrBtScPrim) (0x0007 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_ACTIVATE_REQ                     ((CsrBtScPrim) (0x0008 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_SET_TRUST_LEVEL_REQ              ((CsrBtScPrim) (0x0009 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_READ_DEVICE_RECORD_REQ           ((CsrBtScPrim) (0x000A + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_UPDATE_DEVICE_REQ                ((CsrBtScPrim) (0x000B + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_CANCEL_BOND_REQ                  ((CsrBtScPrim) (0x000C + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_AUTHENTICATE_REQ                 ((CsrBtScPrim) (0x000D + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_SET_IO_CAPABILITY_REQ            ((CsrBtScPrim) (0x000E + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_SSP_PASSKEY_RES                  ((CsrBtScPrim) (0x000F + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_SSP_NOTIFICATION_RES             ((CsrBtScPrim) (0x0010 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_SSP_COMPARE_RES                  ((CsrBtScPrim) (0x0011 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_SSP_JUSTWORKS_RES                ((CsrBtScPrim) (0x0012 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ    ((CsrBtScPrim) (0x0013 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_MODE_REQ                         ((CsrBtScPrim) (0x0014 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ          ((CsrBtScPrim) (0x0015 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ          ((CsrBtScPrim) (0x0016 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ        ((CsrBtScPrim) (0x0017 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_REBOND_RES                       ((CsrBtScPrim) (0x0018 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_SET_EVENT_MASK_REQ               ((CsrBtScPrim) (0x0019 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_SSP_PAIRING_RES                  ((CsrBtScPrim) (0x001A + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_DEBUG_MODE_REQ                   ((CsrBtScPrim) (0x001B + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_CONFIG_REQ                       ((CsrBtScPrim) (0x001C + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_DATABASE_REQ                     ((CsrBtScPrim) (0x001D + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_HOUSE_CLEANING                   ((CsrBtScPrim) (0x001E + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_ACCEPT_BOND_REQ                  ((CsrBtScPrim) (0x001F + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ           ((CsrBtScPrim) (0x0020 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ          ((CsrBtScPrim) (0x0021 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
/* Only used for LE */
#define CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ          ((CsrBtScPrim) (0x0022 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ      ((CsrBtScPrim) (0x0023 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST))

#define CSR_BT_SC_PRIM_DOWNSTREAM_HIGHEST                         (0x0023 + CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST)

/*******************************************************************************/

#define CSR_BT_SC_PRIM_UPSTREAM_LOWEST                           (0x0000 + CSR_PRIM_UPSTREAM)

#define CSR_BT_SC_PASSKEY_IND                      ((CsrBtScPrim) (0x0000 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_BOND_CFM                         ((CsrBtScPrim) (0x0001 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_DEBOND_CFM                       ((CsrBtScPrim) (0x0002 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_SET_SECURITY_MODE_CFM            ((CsrBtScPrim) (0x0003 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_ENCRYPTION_CFM                   ((CsrBtScPrim) (0x0004 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_AUTHORISE_IND                    ((CsrBtScPrim) (0x0005 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_BOND_IND                         ((CsrBtScPrim) (0x0006 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_READ_DEVICE_RECORD_IND           ((CsrBtScPrim) (0x0007 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_READ_DEVICE_RECORD_CFM           ((CsrBtScPrim) (0x0008 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_UPDATE_TRUST_LEVEL_IND           ((CsrBtScPrim) (0x0009 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_AUTHENTICATE_CFM                 ((CsrBtScPrim) (0x000A + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_SSP_PASSKEY_IND                  ((CsrBtScPrim) (0x000B + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_SSP_NOTIFICATION_IND             ((CsrBtScPrim) (0x000C + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_SSP_COMPARE_IND                  ((CsrBtScPrim) (0x000D + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_SSP_JUSTWORKS_IND                ((CsrBtScPrim) (0x000E + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM          ((CsrBtScPrim) (0x000F + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_SSP_PAIRING_IND                  ((CsrBtScPrim) (0x0010 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND    ((CsrBtScPrim) (0x0011 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_MODE_IND                         ((CsrBtScPrim) (0x0012 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_REBOND_IND                       ((CsrBtScPrim) (0x0013 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_DEBUG_MODE_CFM                   ((CsrBtScPrim) (0x0014 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_AUTHORISE_EXT_IND                ((CsrBtScPrim) (0x0015 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_DATABASE_CFM                     ((CsrBtScPrim) (0x0016 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_CANCEL_ACCEPT_BOND_IND           ((CsrBtScPrim) (0x0017 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_LE_SECURITY_IND                  ((CsrBtScPrim) (0x0018 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM          ((CsrBtScPrim) (0x0019 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST))

#define CSR_BT_SC_PRIM_UPSTREAM_HIGHEST                           (0x0019 + CSR_BT_SC_PRIM_UPSTREAM_LOWEST)

#define CSR_BT_SC_PRIM_DOWNSTREAM_COUNT            (CSR_BT_SC_PRIM_DOWNSTREAM_HIGHEST + 1 - CSR_BT_SC_PRIM_DOWNSTREAM_LOWEST)
#define CSR_BT_SC_PRIM_UPSTREAM_COUNT              (CSR_BT_SC_PRIM_UPSTREAM_HIGHEST + 1 - CSR_BT_SC_PRIM_UPSTREAM_LOWEST)
/*******************************************************************************
 * End primitive definitions
 *******************************************************************************/
typedef CsrUint16 CsrBtScReadRecordFlags;
#define CSR_BT_SC_DEFAULT         ((CsrBtScReadRecordFlags) 0x0000)
#define CSR_BT_SC_ADD_HDL_PAIRS   ((CsrBtScReadRecordFlags) 0x0001)


typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrSchedQid             phandle;                    /* protocol handle */
    CsrUint32               maxNumOfBytes;              /* CSRMAX number of bytes an indication is allowed to fill */
    CsrBtScReadRecordFlags  flags;                      /* For intern use only */
} CsrBtScReadDeviceRecordReq;

typedef struct
{
    CsrBtScPrim                 type;                   /* primitive/message identity */
    CsrUint32                   devicePropertiesLength; /* num of devices contained within this indication */
    CsrBtDevicePropertiesType   *deviceProperties;      /* devices from SC_DB with their respective properties */
    CsrUint32                   serviceChangedListLength; 
    CsrUint32                   *serviceChangedList;   /* A list of Service Changed handles */
} CsrBtScReadDeviceRecordInd;

typedef struct
{
    CsrBtScPrim                 type;                   /* primitive/message identity */
    CsrUint32                   totalNumOfDevices;      /* num of devices that were contained in SC_DB */
    CsrUint32                   devicePropertiesLength; /* num of devices contained within this indication */
    CsrBtDevicePropertiesType   *deviceProperties;      /* devices from SC_DB with their respective properties */
    CsrUint32                   serviceChangedListLength; 
    CsrUint32                   *serviceChangedList;   /* A list of Service Changed handles */
} CsrBtScReadDeviceRecordCfm;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrSchedQid             phandle;                    /* protocol handle */
    CsrBtDeviceAddr         deviceAddr;
    CsrUtf8String           *remoteName;
    CsrBtClassOfDevice      classOfDevice;
    CsrUint32               knownServices11_00_31;
    CsrUint32               knownServices11_32_63;
    CsrUint32               knownServices12_00_31;
    CsrUint32               knownServices13_00_31;
    CsrBool                 authorised;
} CsrBtScUpdateDeviceReq;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrSchedQid             phandle;                    /* protocol handle */
} CsrBtScActivateReq;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    dm_protocol_id_t        protocolId;                 /* protocol at which security is to be applied */
    CsrUint16               channel;                    /* channel for the protocol */
    uuid16_t                profileUuid;                /* service uuid16_t for local service */
    CsrBool                 outgoingOk;                 /* TRUE if this also applies to outgoing connections */
    dm_security_level_t     secLevel;                   /* level of security to be applied */
} CsrBtScRegisterReq;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    dm_protocol_id_t        protocolId;                 /* protocol at which security is to be applied */
    CsrUint16               channel;                    /* channel for the protocol */
} CsrBtScDeregisterReq;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrSchedQid             phandle;                    /* protocol handle */
    CsrBtDeviceAddr         deviceAddr;                 /* address of device to bond with. */
} CsrBtScBondReq;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
    CsrBtClassOfDevice      cod;                        /* The class of device of the remote device */
    CsrBtDeviceAddr         deviceAddr;                 /* address of device to bond with. */
    CsrBool                 addedToScDbList;            /* If TRUE the device is added to the Security Manager's device database */
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScBondInd;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
    CsrBtClassOfDevice      cod;                        /* The class of device of the remote device */
    CsrBtDeviceAddr         deviceAddr;                 /* address of device to bond with. */
    CsrBool                 addedToScDbList;            /* If TRUE the device is added to the Security Manager's device database */
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScBondCfm;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrSchedQid             phandle;                    /* protocol handle */
    CsrBtDeviceAddr         deviceAddr;                 /* address of device to bond with. */
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScDebondReq;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
    CsrBtDeviceAddr         deviceAddr;                 /* address of device to bond with. */
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScDebondCfm;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrUint32               cod;                        /* device class of peer device */
    CsrBtDeviceAddr         deviceAddr;                 /* address of device for which a passkey is requested */
    CsrUtf8String          *deviceName;                 /* name of device for which a passkey is requested */
    CsrUint8                paired;
    CsrBool                 initiator;                  /* True if we're initiator */
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScPasskeyInd;

typedef struct
{
    CsrBtScPrim              type;                       /* primitive/message identity */
    CsrBool                  accept;                     /* TRUE to accept the passkey request, FALSE to reject */
    CsrBtDeviceAddr          deviceAddr;                 /* address of device for which a passkey is requested */
    CsrBool                  authorised;                 /* TRUE if authorisation is automatic granted, FALSE if not */
    CsrBool                  addDevice;                  /* If TRUE the device is added to the Security Manager's device database */
    CsrUint8                 passKeyLength;              /* The length of the passkey. The maximum number is 16 */
    CsrUint8                 passKey[CSR_BT_PASSKEY_MAX_LEN];   /* The Passkey */
    CsrBtAddressType         addressType;                /* LE address type */
} CsrBtScPasskeyRes;

typedef struct
{
    CsrBtScPrim              type;                       /* primitive/message identity */
    CsrSchedQid              phandle;                    /* protocol handle */
    dm_security_mode_t       securityMode;               /* the applied security mode */
} CsrBtScSetSecurityModeReq;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrSchedQid             phandle;                    /* protocol handle */
    CsrBool                 encEnable;                    /* the requested encryption mode */
    CsrBtDeviceAddr         deviceAddr;                 /* address of device */
} CsrBtScEncryptionReq;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
    CsrBool                 encEnabled;                 /* encryption mode status */
    CsrBtDeviceAddr         deviceAddr;                 /* address of device for which a passkey is requested */
} CsrBtScEncryptionCfm;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
} CsrBtScSetSecurityModeCfm;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrBtDeviceAddr         deviceAddr;                 /* address of device */
    CsrUtf8String          *deviceName;                 /* name of device */
    uuid16_t                serviceId;                  /* the id of the service for which authorisation is required */
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScAuthoriseInd;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrBtDeviceAddr         deviceAddr;                 /* address of device */
    CsrUtf8String           *deviceName;                /* name of device */
    uuid16_t                serviceId;                  /* the id of the service for which authorisation is required */
    dm_protocol_id_t        protocolId;                 /* protocol at which security is to be applied */
    CsrUint16               channel;                    /* Channel for that protocol (e.g. RFCOMM server channel number, L2CAP PSM) */
    CsrBool                 incoming;                   /* Flag indicating peer-initiated (TRUE) or locally-initiated (FALSE)       */
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScAuthoriseExtInd;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrBool                 authorised;                 /* TRUE if authorisation granted, FALSE to reject */
    CsrBtDeviceAddr         deviceAddr;                 /* address of device */
} CsrBtScAuthoriseRes;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
} CsrBtScHouseCleaning;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrBtDeviceAddr         deviceAddr;                 /* address of device */
    CsrBool                 authorised;
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScSetTrustLevelReq;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrBtDeviceAddr         deviceAddr;                 /* address of device */
    CsrBool                 authorised;
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScUpdateTrustLevelInd;

typedef struct
{
    CsrBtScPrim             type;                       /* primitive/message identity */
    CsrSchedQid             phandle;                    /* protocol handle */
    CsrBtDeviceAddr         deviceAddr;                 /* address of device to bond with. */
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScCancelBondReq;

typedef struct
{
    CsrBtScPrim             type;
    CsrSchedQid             phandle;
    CsrBtDeviceAddr         deviceAddr;
} CsrBtScAuthenticateReq;

typedef struct
{
    CsrBtScPrim             type;
    CsrBtDeviceAddr         deviceAddr;
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScAuthenticateCfm;

typedef struct
{
    CsrBtScPrim             type;
    CsrTime                 duration;
    CsrUint8                mode;
} CsrBtScModeReq;

typedef struct
{
    CsrBtScPrim             type;
    CsrUint8                mode;
} CsrBtScModeInd;

typedef struct
{
    CsrBtScPrim             type;
    CsrUint8                ioCapability;
} CsrBtScSetIoCapabilityReq;

typedef struct
{
    CsrBtScPrim             type;
    CsrUint8                authRequirements;          /* BR/EDR requirements, HCI_MITM_... */
    CsrUint16               leAuthRequirements;        /* Low energy requirements, DM_SM_SECURITY_... */
} CsrBtScSetAuthRequirementsReq;

typedef struct
{
    CsrBtScPrim             type;
    CsrUint32               cod;
    CsrBtDeviceAddr         deviceAddr;
    CsrUtf8String           *deviceName;
    CsrUint8                paired;
    CsrUint32               authRequirements;
    CsrUint32               numericValue;
    CsrBool                 initiator;                  /* True if we're initiator */
    CsrBtAddressType        addressType;                /* LE address type */
    CsrUint32               localAuthRequirements;
} CsrBtScSspCompareInd;

typedef struct
{
    CsrBtScPrim             type;
    CsrBool                 accept;
    CsrBtDeviceAddr         deviceAddr;
    CsrBool                 authorised;
    CsrBool                 addDevice;
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScSspCompareRes;

typedef CsrBtScSspCompareInd CsrBtScSspNotificationInd;

typedef CsrBtScSspCompareRes CsrBtScSspNotificationRes;

typedef struct
{
    CsrBtScPrim             type;
    CsrUint32               cod;
    CsrBtDeviceAddr         deviceAddr;
    CsrUtf8String           *deviceName;
    CsrUint8                paired;
    CsrUint32               authRequirements;
    CsrBool                 initiator;                  /* True if we're initiator */
    CsrBtAddressType        addressType;                /* LE address type */
    CsrUint32               localAuthRequirements;
} CsrBtScSspPasskeyInd;

typedef struct
{
    CsrBtScPrim              type;
    CsrBool                  accept;
    CsrBtDeviceAddr          deviceAddr;
    CsrBool                  authorised;
    CsrBool                  addDevice;
    CsrUint32                numericValue;
    CsrBtAddressType         addressType;                /* LE address type */
} CsrBtScSspPasskeyRes;

typedef CsrBtScSspPasskeyInd CsrBtScSspJustworksInd;

typedef CsrBtScSspCompareRes CsrBtScSspJustworksRes;

typedef struct
{
    CsrBtScPrim             type;
    CsrBtDeviceAddr         deviceAddr;
    CsrUint8                notificationType;
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScSspKeypressNotificationReq;

typedef CsrBtScSspKeypressNotificationReq CsrBtScSspKeypressNotificationInd;

typedef struct
{
    CsrBtScPrim             type;
    CsrUint32               cod;
    CsrBtDeviceAddr         deviceAddr;
    CsrUtf8String          *deviceName;
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScRebondInd;

typedef struct
{
    CsrBtScPrim             type;
    CsrBool                 accept;
    CsrBtDeviceAddr         deviceAddr;
} CsrBtScRebondRes;

typedef struct
{
    CsrBtScPrim             type;
    CsrUint32               cod;
    CsrBtDeviceAddr         deviceAddr;
    CsrUtf8String           *deviceName;
    CsrBool                 authValid;
    CsrUint32               authRequirements;
    CsrUint8                paired;
    CsrBool                 initiator;                  /* True if we're initiator */
    CsrBtAddressType        addressType;                /* LE address type */
    CsrUint32               localAuthRequirements;
} CsrBtScSspPairingInd;

typedef struct
{
    CsrBtScPrim             type;
    CsrBool                 accept;
    CsrBtDeviceAddr         deviceAddr;
    CsrBool                 authorised;
    CsrBool                 addDevice;
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScSspPairingRes;

typedef struct
{
    CsrBtScPrim             type;
    CsrUint32               eventMask;
} CsrBtScSetEventMaskReq;

typedef struct
{
    CsrBtScPrim             type;
    CsrSchedQid             phandle;
} CsrBtScReadLocalOobDataReq;

typedef struct
{
    CsrBtScPrim             type;
    CsrBtDeviceAddr         deviceAddr;
    CsrUint32               oobHashCLength;
    CsrUint8                *oobHashC;
    CsrUint32               oobRandRLength;
    CsrUint8                *oobRandR;
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScReadLocalOobDataCfm;

typedef struct
{
    CsrBtScPrim             type;
    CsrBtDeviceAddr         deviceAddr;
    CsrUint8                oobHashC[SIZE_OOB_DATA];
    CsrUint8                oobRandR[SIZE_OOB_DATA];
} CsrBtScAddRemoteOobDataReq;

typedef struct
{
    CsrBtScPrim             type;
    CsrSchedQid             phandle;
    CsrBool                 enable;
} CsrBtScDebugModeReq;

typedef struct
{
    CsrBtScPrim             type;
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
    CsrBool                 enabled;
} CsrBtScDebugModeCfm;

typedef struct
{
    CsrBtScPrim             type;
    CsrBtScConfigMask       configMask;
} CsrBtScConfigReq;

typedef struct
{
    CsrBtScPrim             type;
    CsrSchedQid             appHandle;
    CsrBtDeviceAddr         deviceAddr;
    CsrUint8                opcode;
    CsrUint8                entry;
    CsrUint8                keyType;
    CsrUint8                length;
    CsrUint8               *data;
    CsrBtAddressType        addressType;
} CsrBtScDatabaseReq;

typedef struct
{
    CsrBtScPrim             type;
    CsrBtDeviceAddr         deviceAddr;
    CsrUint8                opcode;
    CsrUint8                entry;
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
    CsrUint8                keyType;
    CsrUint8                length;
    CsrUint8               *data;
    CsrBtAddressType        addressType;                /* LE address type */
} CsrBtScDatabaseCfm;

typedef struct
{
    CsrBtScPrim             type;
    CsrSchedQid             phandle;
    CsrSchedTid             time;       /* time in msec where the device shall be discoverable from the time the request is handled */
} CsrBtScAcceptBondReq;

typedef struct
{
    CsrBtScPrim             type;
    CsrSchedQid             phandle;
} CsrBtScCancelAcceptBondReq;

typedef struct
{
    CsrBtScPrim             type;
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
} CsrBtScCancelAcceptBondInd;

typedef struct
{
    CsrBtScPrim             type;                       /* CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ */
    CsrUint16               keyDistribution;            /* Key distribution for low energy. Use
                                                         * DM_SM_KEY_DIST_ bitmask */
} CsrBtScLeKeyDistributionReq;

typedef struct
{
    CsrBtScPrim             type;                       /* Primitive/message identity */
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
    CsrBtTypedAddr          address;                    /* Address of device to bond with. */
    CsrBool                 addedToScDbList;            /* If TRUE the device was added to the device database */
} CsrBtScLeSecurityInd;

typedef struct
{
    CsrBtScPrim             type;                       /* CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ */
    CsrSchedQid             appHandle;                  /* Requesting application */
    CsrBtTypedAddr          address;                    /* Peer address */
} CsrBtScEncryptionKeySizeReq;

typedef struct
{
    CsrBtScPrim             type;                       /* CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM */
    CsrBtTypedAddr          address;                    /* Peer address */
    CsrUint16               keySize;
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
} CsrBtScEncryptionKeySizeCfm;

typedef struct
{
    CsrBtScPrim             type;                       /* CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ */
    CsrUint8                minKeySize;
    CsrUint8                maxKeySize;
} CsrBtScSetEncryptionKeySizeReq;

#ifdef __cplusplus
}
#endif

#endif


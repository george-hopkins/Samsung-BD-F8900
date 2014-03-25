#ifndef CSR_BT_PAS_PRIM_H__
#define CSR_BT_PAS_PRIM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_result.h"
#include "csr_bt_profiles.h"
#include "csr_bt_obex.h"

#ifdef __cplusplus
extern "C" {
#endif

/* search_string="CsrBtPasPrim" */
/* conversion_rule="UPPERCASE_START_AND_REMOVE_UNDERSCORES" */

typedef CsrPrim    CsrBtPasPrim;

/*******************************************************************************
 * Primitive definitions
 *******************************************************************************/
#define CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST                         (0x0000)

#define CSR_BT_PAS_ACTIVATE_REQ                    ((CsrBtPasPrim) (0x0000 + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_DEACTIVATE_REQ                  ((CsrBtPasPrim) (0x0001 + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_CONNECT_RES                     ((CsrBtPasPrim) (0x0002 + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_AUTHENTICATE_REQ                ((CsrBtPasPrim) (0x0003 + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_AUTHENTICATE_RES                ((CsrBtPasPrim) (0x0004 + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_PB_RES                     ((CsrBtPasPrim) (0x0005 + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_PB_SIZE_RES                ((CsrBtPasPrim) (0x0006 + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_MCH_RES                    ((CsrBtPasPrim) (0x0007 + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_MCH_SIZE_RES               ((CsrBtPasPrim) (0x0008 + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_SET_FOLDER_RES                  ((CsrBtPasPrim) (0x0009 + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_VCARD_LIST_RES             ((CsrBtPasPrim) (0x000A + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES        ((CsrBtPasPrim) (0x000B + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES         ((CsrBtPasPrim) (0x000C + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES    ((CsrBtPasPrim) (0x000D + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_VCARD_ENTRY_RES            ((CsrBtPasPrim) (0x000E + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_NEXT_RES                        ((CsrBtPasPrim) (0x000F + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAS_SECURITY_IN_REQ                 ((CsrBtPasPrim) (0x0010 + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST))

#define CSR_BT_PAS_PRIM_DOWNSTREAM_HIGHEST                        (0x0010 + CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST)

/*******************************************************************************/

#define CSR_BT_PAS_PRIM_UPSTREAM_LOWEST                           (0x0000 + CSR_PRIM_UPSTREAM)

#define CSR_BT_PAS_DEACTIVATE_CFM                  ((CsrBtPasPrim) (0x0000 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_CONNECT_IND                     ((CsrBtPasPrim) (0x0001 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_AUTHENTICATE_CFM                ((CsrBtPasPrim) (0x0002 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_AUTHENTICATE_IND                ((CsrBtPasPrim) (0x0003 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_PB_IND                     ((CsrBtPasPrim) (0x0004 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_PB_SIZE_IND                ((CsrBtPasPrim) (0x0005 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_MCH_IND                    ((CsrBtPasPrim) (0x0006 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_MCH_SIZE_IND               ((CsrBtPasPrim) (0x0007 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_SET_FOLDER_IND                  ((CsrBtPasPrim) (0x0008 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_SET_BACK_FOLDER_IND             ((CsrBtPasPrim) (0x0009 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_SET_ROOT_FOLDER_IND             ((CsrBtPasPrim) (0x000A + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_VCARD_LIST_IND             ((CsrBtPasPrim) (0x000B + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND        ((CsrBtPasPrim) (0x000C + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND         ((CsrBtPasPrim) (0x000D + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND    ((CsrBtPasPrim) (0x000E + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_PULL_VCARD_ENTRY_IND            ((CsrBtPasPrim) (0x000F + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_NEXT_IND                        ((CsrBtPasPrim) (0x0010 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_ABORT_IND                       ((CsrBtPasPrim) (0x0011 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_DISCONNECT_IND                  ((CsrBtPasPrim) (0x0012 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAS_SECURITY_IN_CFM                 ((CsrBtPasPrim) (0x0013 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST))

#define CSR_BT_PAS_PRIM_UPSTREAM_HIGHEST                          (0x0013 + CSR_BT_PAS_PRIM_UPSTREAM_LOWEST)

#define CSR_BT_PAS_PRIM_DOWNSTREAM_COUNT           (CSR_BT_PAS_PRIM_DOWNSTREAM_HIGHEST + 1 - CSR_BT_PAS_PRIM_DOWNSTREAM_LOWEST)
#define CSR_BT_PAS_PRIM_UPSTREAM_COUNT             (CSR_BT_PAS_PRIM_UPSTREAM_HIGHEST + 1 - CSR_BT_PAS_PRIM_UPSTREAM_LOWEST)
/*******************************************************************************
 * End primitive definitions
 *******************************************************************************/

#define CSR_BT_PAS_SUPPORT_FEATURE_DOWNLOAD        (0x01)
#define CSR_BT_PAS_SUPPORT_FEATURE_BROWSING        (0x02)

#define CSR_BT_PAS_SUPPORT_REPOSITORY_PB           (0x01)
#define CSR_BT_PAS_SUPPORT_REPOSITORY_SIM          (0x02)

/* == SIGNAL TYPES =========================================================== */
/* downstream */
typedef struct
{
    CsrBtPasPrim             type;
    CsrSchedQid              appHandle;
    CsrUint8                 feature;
    CsrUint8                 repository;
    CsrUint16                obexMaxPacketSize;
    CsrUint16                windowSize;
    CsrBool                  srmEnable;
} CsrBtPasActivateReq;

typedef struct
{
    CsrBtPasPrim             type;
} CsrBtPasDeactivateReq;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrBtObexResponseCode    responseCode;
} CsrBtPasConnectRes;

/* upstream */
typedef struct
{
    CsrBtPasPrim             type;
} CsrBtPasDeactivateCfm;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint16                obexPeerMaxPacketSize;
    CsrBtDeviceAddr          deviceAddr;
    CsrUint32                length;
    CsrUint32                count;
    CsrBtConnId              btConnId;               /* Global Bluetooth connection ID */
} CsrBtPasConnectInd;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint16                realmLength;
    CsrUint8                *realm;
    CsrUint16                passwordLength;
    CsrUint8                *password;
    CsrCharString           *userId; /*Max length is BT_OBEX_MAX_AUTH_USERID_LENGTH */
} CsrBtPasAuthenticateReq;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint8                *password;
    CsrUint16                passwordLength;
    CsrCharString           *userId;/*Max length is BT_OBEX_MAX_AUTH_USERID_LENGTH */
} CsrBtPasAuthenticateRes;

typedef struct
{
    CsrBtPasPrim             type;
} CsrBtPasAuthenticateCfm;

typedef struct
{
    CsrBtPasPrim             type;
    CsrBtDeviceAddr          deviceAddr;
    CsrUint8                 options;
    CsrUint16                realmLength;
    CsrUint8                *realm;
} CsrBtPasAuthenticateInd;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrBtObexResponseCode    responseCode;
} CsrBtPasPullPbRes;

typedef CsrUint16            CsrBtPasSrc;
#define CSR_BT_PAS_SRC_PHONE       (CsrBtPasSrc)(0)
#define CSR_BT_PAS_SRC_SIM         (CsrBtPasSrc)(1)

typedef CsrUint8             CsrBtPasFormat;
#define CSR_BT_PAS_FORMAT_VCARD2_1 (CsrBtPasFormat)(0x00)
#define CSR_BT_PAS_FORMAT_VCARD3_0 (CsrBtPasFormat)(0x01)

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint16                ucs2nameOffset;      /* Payload relative offset, zero-terminated */
    CsrUint8                 filter[8];
    CsrBtPasSrc              src;
    CsrBtPasFormat           format;
    CsrUint16                maxListCnt;
    CsrUint16                listStartOffset;
    CsrUint8                *payload;
    CsrUint16                payloadLength;
} CsrBtPasPullPbInd;


typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint16                pbSize;
    CsrBtObexResponseCode   responseCode;
    CsrBool                  srmpOn;
} CsrBtPasPullPbSizeRes;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint16                ucs2nameOffset;      /* Payload relative offset, zero-terminated */
    CsrBtPasSrc              src;
    CsrUint8                *payload;
    CsrUint16                payloadLength;
} CsrBtPasPullPbSizeInd;


typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint8                 newMissedCall;
    CsrBtObexResponseCode    responseCode;
    CsrBool                  srmpOn;
} CsrBtPasPullMchRes;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint8                 filter[8];
    CsrBtPasSrc              src;
    CsrBtPasFormat           format;
    CsrUint16                maxListCnt;
    CsrUint16                listStartOffset;
} CsrBtPasPullMchInd;


typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint16                pbSize;
    CsrUint8                 newMissedCall;
    CsrBtObexResponseCode    responseCode;
    CsrBool                  srmpOn;
} CsrBtPasPullMchSizeRes;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrBtPasSrc              src;
} CsrBtPasPullMchSizeInd;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrBtObexResponseCode    responseCode;
} CsrBtPasSetFolderRes;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint16                ucs2nameOffset;      /* Payload relative offset, zero-terminated */
    CsrUint8                *payload;
    CsrUint16                payloadLength;
} CsrBtPasSetFolderInd;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
} CsrBtPasSetBackFolderInd;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
} CsrBtPasSetRootFolderInd;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrBtObexResponseCode    responseCode;
    CsrBool                  srmpOn;
} CsrBtPasPullVcardListRes;

typedef CsrUint8             CsrBtPasOrder;
#define CSR_BT_PAS_ORDER_ALPHABETICAL  (CsrBtPasOrder)(0x01)
#define CSR_BT_PAS_ORDER_INDEXED       (CsrBtPasOrder)(0x00)
#define CSR_BT_PAS_ORDER_PHONETICAL    (CsrBtPasOrder)(0x02)

typedef CsrUint8     CsrBtPasSearchAtt;
#define CSR_BT_PAS_SEARCH_ATT_NAME     (CsrBtPasSearchAtt)(0x00)
#define CSR_BT_PAS_SEARCH_ATT_NUMBER   (CsrBtPasSearchAtt)(0x01)
#define CSR_BT_PAS_SEARCH_ATT_SOUND    (CsrBtPasSearchAtt)(0x02)

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint16                ucs2nameOffset;      /* Payload relative offset, zero-terminated */
    CsrBtPasOrder            order;
    CsrUint16                uft8SearchValOffset; /* Payload relative offset */
    CsrUint16                searchValLength;
    CsrBtPasSearchAtt        searchAtt;
    CsrUint16                maxListCnt;
    CsrUint16                listStartOffset;
    CsrUint8                *payload;
    CsrUint16                payloadLength;
} CsrBtPasPullVcardListInd;


typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint16                pbSize;
    CsrBtObexResponseCode    responseCode;
    CsrBool                  srmpOn;
} CsrBtPasPullVcardListSizeRes;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint16                ucs2nameOffset;      /* Payload relative offset, zero-terminated */
    CsrUint8                *payload;
    CsrUint16                payloadLength;
} CsrBtPasPullVcardListSizeInd;


typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint8                 newMissedCall;
    CsrBtObexResponseCode    responseCode;
    CsrBool                  srmpOn;
} CsrBtPasPullVcardMchListRes;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrBtPasOrder            order;
    CsrUint16                uft8SearchValOffset; /* Payload relative offset */
    CsrUint16                searchValLength;
    CsrBtPasSearchAtt        searchAtt;
    CsrUint16                maxListCnt;
    CsrUint16                listStartOffset;
    CsrUint8                *payload;
    CsrUint16                payloadLength;
} CsrBtPasPullVcardMchListInd;


typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint16                pbSize;
    CsrUint8                 newMissedCall;
    CsrBtObexResponseCode    responseCode;
    CsrBool                  srmpOn;
} CsrBtPasPullVcardMchListSizeRes;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
} CsrBtPasPullVcardMchListSizeInd;


typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint8                 finalFlag;
    CsrBtObexResponseCode    responseCode;
    CsrBool                  srmpOn;
} CsrBtPasPullVcardEntryRes;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint16                ucs2nameOffset;      /* Payload relative offset, zero-terminated */
    CsrUint8                 filter[8];
    CsrBtPasFormat           format;
    CsrUint8                *payload;
    CsrUint16                payloadLength;
} CsrBtPasPullVcardEntryInd;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint8                 finalFlag;
    CsrBtObexResponseCode    responseCode;
    CsrUint16                bodyLength;
    CsrUint8                *body;
    CsrBool                  srmpOn;
} CsrBtPasNextRes;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
} CsrBtPasNextInd;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrUint16                descriptionOffset;   /* Payload relative offset */
    CsrUint16                descriptionLength;
    CsrUint8                *payload;
    CsrUint16                payloadLength;
} CsrBtPasAbortInd;

typedef struct
{
    CsrBtPasPrim             type;
    CsrUint32                connectionId;
    CsrBtDeviceAddr          deviceAddr;
    CsrBtReasonCode          reasonCode;
    CsrBtSupplier            reasonSupplier;
} CsrBtPasDisconnectInd;

typedef struct
{
    CsrBtPasPrim             type;
    CsrSchedQid              appHandle;
    CsrUint16                secLevel;
} CsrBtPasSecurityInReq;

typedef struct
{
    CsrBtPasPrim             type;
    CsrBtResultCode          resultCode;
    CsrBtSupplier            resultSupplier;
} CsrBtPasSecurityInCfm;

#ifdef __cplusplus
}
#endif

#endif


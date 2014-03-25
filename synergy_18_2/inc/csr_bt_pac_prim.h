#ifndef CSR_BT_PAC_PRIM_H__
#define CSR_BT_PAC_PRIM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_unicode.h"
#include "csr_bt_result.h"
#include "csr_bt_profiles.h"
#include "csr_bt_obex.h"

#ifdef __cplusplus
extern "C" {
#endif

/* search_string="CsrBtPacPrim" */
/* conversion_rule="UPPERCASE_START_AND_REMOVE_UNDERSCORES" */

typedef CsrPrim           CsrBtPacPrim;

#define PAC_DOWNLOAD_SUPPORT        0x01
#define PAC_BROWSING_SUPPORT        0x02

typedef CsrUint16        CsrBtPacSrcType;
#define CSR_BT_PAC_SRC_NONE    (CsrBtPacSrcType)(0x00)
#define CSR_BT_PAC_SRC_PHONE   (CsrBtPacSrcType)(0x01)
#define CSR_BT_PAC_SRC_SIM     (CsrBtPacSrcType)(0x02)


typedef CsrUint8             CsrBtPacFormatType;
#define CSR_BT_PAC_FORMAT_VCARD2_1 (CsrBtPacFormatType)(0x00)
#define CSR_BT_PAC_FORMAT_VCARD3_0 (CsrBtPacFormatType)(0x01)

/*******************************************************************************
 * Primitive definitions
 *******************************************************************************/
#define CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST                             (0x0000)

#define CSR_BT_PAC_CONNECT_REQ                         ((CsrBtPacPrim) (0x0000 + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_AUTHENTICATE_RES                    ((CsrBtPacPrim) (0x0001 + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_PULL_PB_REQ                         ((CsrBtPacPrim) (0x0002 + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_PULL_PB_RES                         ((CsrBtPacPrim) (0x0003 + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_SET_FOLDER_REQ                      ((CsrBtPacPrim) (0x0004 + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_SET_BACK_FOLDER_REQ                 ((CsrBtPacPrim) (0x0005 + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_SET_ROOT_FOLDER_REQ                 ((CsrBtPacPrim) (0x0006 + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_PULL_VCARD_LIST_REQ                 ((CsrBtPacPrim) (0x0007 + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_PULL_VCARD_LIST_RES                 ((CsrBtPacPrim) (0x0008 + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_PULL_VCARD_ENTRY_REQ                ((CsrBtPacPrim) (0x0009 + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_PULL_VCARD_ENTRY_RES                ((CsrBtPacPrim) (0x000A + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_ABORT_REQ                           ((CsrBtPacPrim) (0x000B + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_DISCONNECT_REQ                      ((CsrBtPacPrim) (0x000C + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_CANCEL_CONNECT_REQ                  ((CsrBtPacPrim) (0x000D + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_PAC_SECURITY_OUT_REQ                    ((CsrBtPacPrim) (0x000E + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))

#define CSR_BT_PAC_PRIM_DOWNSTREAM_HIGHEST                            (0x000E + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST)

/*  Not part of interface, used internally only */
#define CSR_BT_PAC_HOUSE_CLEANING                      ((CsrBtPacPrim) (0x007F + CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST))

/*******************************************************************************/

#define CSR_BT_PAC_PRIM_UPSTREAM_LOWEST                               (0x0000 + CSR_PRIM_UPSTREAM)

#define CSR_BT_PAC_CONNECT_CFM                         ((CsrBtPacPrim) (0x0000 + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAC_AUTHENTICATE_IND                    ((CsrBtPacPrim) (0x0001 + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAC_PULL_PB_CFM                         ((CsrBtPacPrim) (0x0002 + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAC_PULL_PB_IND                         ((CsrBtPacPrim) (0x0003 + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAC_SET_FOLDER_CFM                      ((CsrBtPacPrim) (0x0004 + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAC_SET_BACK_FOLDER_CFM                 ((CsrBtPacPrim) (0x0005 + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAC_SET_ROOT_FOLDER_CFM                 ((CsrBtPacPrim) (0x0006 + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAC_PULL_VCARD_LIST_CFM                 ((CsrBtPacPrim) (0x0007 + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAC_PULL_VCARD_LIST_IND                 ((CsrBtPacPrim) (0x0008 + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAC_PULL_VCARD_ENTRY_CFM                ((CsrBtPacPrim) (0x0009 + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAC_PULL_VCARD_ENTRY_IND                ((CsrBtPacPrim) (0x000A + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAC_ABORT_CFM                           ((CsrBtPacPrim) (0x000B + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAC_DISCONNECT_IND                      ((CsrBtPacPrim) (0x000C + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_PAC_SECURITY_OUT_CFM                    ((CsrBtPacPrim) (0x000D + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST))

#define CSR_BT_PAC_PRIM_UPSTREAM_HIGHEST                              (0x000D + CSR_BT_PAC_PRIM_UPSTREAM_LOWEST)

#define CSR_BT_PAC_PRIM_DOWNSTREAM_COUNT               (CSR_BT_PAC_PRIM_DOWNSTREAM_HIGHEST + 1 - CSR_BT_PAC_PRIM_DOWNSTREAM_LOWEST)
#define CSR_BT_PAC_PRIM_UPSTREAM_COUNT                 (CSR_BT_PAC_PRIM_UPSTREAM_HIGHEST + 1 - CSR_BT_PAC_PRIM_UPSTREAM_LOWEST)
/*******************************************************************************
 * End primitive definitions
 *******************************************************************************/

typedef struct
{
    CsrBtPacPrim           type;
    CsrSchedQid            appHandle;
    CsrUint16              maxPacketSize;
    CsrBtDeviceAddr        destination;
    CsrBool                authorize;
    CsrUint8               realmLength;
    CsrUint8              *realm;
    CsrUint16              passwordLength;
    CsrUint8              *password;
    CsrCharString         *userId; /*Max length is BT_OBEX_MAX_AUTH_USERID_LENGTH */
    CsrUint32              count;
    CsrUint32              length; 
    CsrUint16              windowSize;
    CsrBool                srmEnable;
} CsrBtPacConnectReq;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
    CsrUint16               obexPeerMaxPacketSize;
    CsrBtPacSrcType         supportedRepositories;
    CsrBtConnId             btConnId;               /* Global Bluetooth connection ID */
} CsrBtPacConnectCfm;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBtDeviceAddr         deviceAddr;
    CsrUint8                options;
    CsrUint16               realmLength;
    CsrUint8               *realm;
} CsrBtPacAuthenticateInd;

typedef struct
{
    CsrBtPacPrim            type;
    CsrUint8               *password;
    CsrUint16               passwordLength;
    CsrCharString          *userId;/*Max length is BT_OBEX_MAX_AUTH_USERID_LENGTH */
} CsrBtPacAuthenticateRes;

typedef struct
{
    CsrBtPacPrim            type;
    CsrUcs2String          *ucs2name; /* Unicode name, zero-terminated */
    CsrBtPacSrcType         src;
    CsrUint8                filter[8];
    CsrBtPacFormatType      format;
    CsrUint16               maxListCnt;
    CsrUint16               listStartOffset;
    CsrBool                 srmpOn;
} CsrBtPacPullPbReq;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBtObexResponseCode   responseCode;
    CsrUint16               pbSize;
    CsrUint8                newMissedCall;
} CsrBtPacPullPbCfm;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBool                 srmpOn;
} CsrBtPacPullPbRes;

typedef struct
{
    CsrBtPacPrim             type;
    CsrUint16                bodyLength;
    CsrUint16                bodyOffset;
    CsrUint8                *payload;
    CsrUint16                payloadLength;
} CsrBtPacPullPbInd;


typedef struct
{
    CsrBtPacPrim            type;
    CsrUcs2String          *ucs2name; /* Unicode name, zero-terminated */
} CsrBtPacSetFolderReq;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBtObexResponseCode   responseCode;
} CsrBtPacSetFolderCfm;

typedef struct
{
    CsrBtPacPrim            type;
} CsrBtPacSetBackFolderReq;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBtObexResponseCode   responseCode;
} CsrBtPacSetBackFolderCfm;

typedef struct
{
    CsrBtPacPrim            type;
} CsrBtPacSetRootFolderReq;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBtObexResponseCode   responseCode;
} CsrBtPacSetRootFolderCfm;


typedef CsrUint8     CsrBtPacOrderType;
#define CSR_BT_PAC_ORDER_ALPHABETICAL  (CsrBtPacOrderType)(0x01)
#define CSR_BT_PAC_ORDER_INDEXED       (CsrBtPacOrderType)(0x00)
#define CSR_BT_PAC_ORDER_PHONETICAL    (CsrBtPacOrderType)(0x02)

typedef CsrUint8     CsrBtPacSearchAtt;
#define CSR_BT_PAC_SEARCH_ATT_NAME     (CsrBtPacSearchAtt)(0x00)
#define CSR_BT_PAC_SEARCH_ATT_NUMBER   (CsrBtPacSearchAtt)(0x01)
#define CSR_BT_PAC_SEARCH_ATT_SOUND    (CsrBtPacSearchAtt)(0x02)

typedef struct
{
    CsrBtPacPrim            type;
    CsrUcs2String          *ucs2name; /* Unicode name, zero-terminated */
    CsrBtPacOrderType       order;
    CsrUint8               *searchVal;
    CsrUint16               searchValLen;
    CsrBtPacSearchAtt       searchAtt;
    CsrUint16               maxListCnt;
    CsrUint16               listStartOffset;
    CsrBool                 srmpOn;
} CsrBtPacPullVcardListReq;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBtObexResponseCode   responseCode;
    CsrUint16               pbSize;
    CsrUint8                newMissedCall;
} CsrBtPacPullVcardListCfm;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBool                 srmpOn;
} CsrBtPacPullVcardListRes;

typedef struct
{
    CsrBtPacPrim            type;
    CsrUint16               bodyLength;
    CsrUint16               bodyOffset;
    CsrUint8               *payload;
    CsrUint16               payloadLength;
} CsrBtPacPullVcardListInd;


typedef struct
{
    CsrBtPacPrim            type;
    CsrUcs2String          *ucs2name; /* Unicode name, zero-terminated */
    CsrUint8                filter[8];
    CsrBtPacFormatType      format;
    CsrBool                 srmpOn;
} CsrBtPacPullVcardEntryReq;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBtObexResponseCode   responseCode;
} CsrBtPacPullVcardEntryCfm;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBool                 srmpOn;
} CsrBtPacPullVcardEntryRes;

typedef struct
{
    CsrBtPacPrim            type;
    CsrUint16               bodyLength;
    CsrUint16               bodyOffset;
    CsrUint8               *payload;
    CsrUint16               payloadLength;
} CsrBtPacPullVcardEntryInd;

typedef struct
{
    CsrBtPacPrim            type;
} PacLpSupervisionTimer;

typedef struct
{
    CsrBtPacPrim            type;
} CsrBtPacHouseCleaning;

typedef struct
{
    CsrBtPacPrim            type;
} CsrBtPacAbortReq;

typedef struct
{
    CsrBtPacPrim            type;
} CsrBtPacAbortCfm;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBool                 normalDisconnect;
} CsrBtPacDisconnectReq;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBtReasonCode         reasonCode;
    CsrBtSupplier           reasonSupplier;
} CsrBtPacDisconnectInd;

typedef struct
{
    CsrBtPacPrim            type;
} CsrBtPacCancelConnectReq;

typedef struct
{
    CsrBtPacPrim            type;
    CsrSchedQid             appHandle;
    CsrUint16               secLevel;
} CsrBtPacSecurityOutReq;

typedef struct
{
    CsrBtPacPrim            type;
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
} CsrBtPacSecurityOutCfm;

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_PAC_PRIM_H__ */


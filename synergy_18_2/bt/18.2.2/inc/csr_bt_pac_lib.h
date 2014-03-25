#ifndef CSR_BT_PAC_LIB_H__
#define CSR_BT_PAC_LIB_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "csr_bt_pac_prim.h"
#include "csr_bt_tasks.h"
#include "csr_pmem.h"
#include "csr_util.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_PAC_IS_SUPPORT_FEATURE_DOWNLOAD(flag)   (CsrBool)((flag) & 0x01)
#define CSR_BT_PAC_IS_SUPPORT_FEATURE_BROWSING(flag)   (CsrBool)((flag) & 0x02)
#define CSR_BT_PAC_IS_SUPPORT_REPOSITORY_PB(flag)      (CsrBool)((flag) & 0x01)
#define CSR_BT_PAC_IS_SUPPORT_REPOSITORY_SIM(flag)     (CsrBool)((flag) & 0x02)

/* Common put_message function to reduce code size */
void CsrBtPacMsgTransport(void* msg);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacConnectReqSend
 *
 *  DESCRIPTION
 *      Try to make a connection the a peer device.
 *
 *  PARAMETERS
 *      appHandle:          ...
 *      maxPacketSize:      ...
 *      destination:        ...
 *      authorize:          ...
 *      realmLength:        ...
 *      realm:              ...
 *      passwordLength:     ...
 *      password:           ...
 *      userId:             ...
 *      length:               Length is used to express the approximate total
 *                            length of the bodies of all the objects in the
 *                            transaction
 *      count:                Count is used to indicate the number of objects
 *                            that will be sent during this connection
 *----------------------------------------------------------------------------*/
#define CsrBtPacConnectReqSend(_appHandle, _maxPacketSize, _destination, _authorize,_realmLength,_realm,_passwordLength,_password,_userId,_length,_count,_windowSize,_srmEnable) { \
        CsrBtPacConnectReq *msg__ = (CsrBtPacConnectReq *)CsrPmemAlloc(sizeof(CsrBtPacConnectReq)); \
        msg__->type              = CSR_BT_PAC_CONNECT_REQ;              \
        msg__->appHandle         = _appHandle;                          \
        msg__->maxPacketSize     = _maxPacketSize;                      \
        msg__->destination       = _destination;                        \
        msg__->authorize         = _authorize;                          \
        msg__->realmLength       = _realmLength;                        \
        msg__->realm             = _realm;                              \
        msg__->passwordLength    = _passwordLength;                     \
        msg__->password          = _password;                           \
        msg__->userId            = _userId;                             \
        msg__->length            = _length;                             \
        msg__->count             = _count;                              \
        msg__->windowSize        = _windowSize;                         \
        msg__->srmEnable         = _srmEnable;                          \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacAuthenticateResSend
 *
 *  DESCRIPTION
 *      Try to make a connection the a peer device.
 *
 *  PARAMETERS
 *        password:          ...
 *        passwordLength:    ...
 *        userId:            ...
 *----------------------------------------------------------------------------*/
#define CsrBtPacAuthenticateResSend(_password, _passwordLength, _userId) { \
        CsrBtPacAuthenticateRes *msg__ = (CsrBtPacAuthenticateRes *)CsrPmemAlloc(sizeof(CsrBtPacAuthenticateRes)); \
        msg__->type = CSR_BT_PAC_AUTHENTICATE_RES;                      \
        msg__->password          = _password;                           \
        msg__->passwordLength    = _passwordLength;                     \
        msg__->userId            = _userId;                             \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacPullPbReqSend
 *
 *  DESCRIPTION
 *      Try to download a phonebook.
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtPacPullPbReqSend(_ucs2name, _src, _filter, _format, _maxLstCnt, _listStartOffset, _srmpOn) { \
        CsrBtPacPullPbReq *msg__ = (CsrBtPacPullPbReq *)CsrPmemAlloc(sizeof(CsrBtPacPullPbReq)); \
        msg__->type = CSR_BT_PAC_PULL_PB_REQ;                           \
        msg__->ucs2name= _ucs2name;                                     \
        msg__->src = _src;                                              \
        CsrMemCpy(msg__->filter, _filter, sizeof(msg__->filter));       \
        msg__->format = _format;                                        \
        msg__->maxListCnt = _maxLstCnt;                                 \
        msg__->listStartOffset = _listStartOffset;                      \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacPullPbResSend
 *
 *  DESCRIPTION
 *      Try to download a phonebook.
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtPacPullPbResSend(_srmpOn) {                                \
        CsrBtPacPullPbRes *msg__ = ( CsrBtPacPullPbRes *)CsrPmemAlloc(sizeof(CsrBtPacPullPbRes)); \
        msg__->type = CSR_BT_PAC_PULL_PB_RES;                           \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacSetFolderReqSend
 *
 *  DESCRIPTION
 *      Try top set the path where a PB entry must be downloaded from.
 *      Remark it's only allowed to move one step per call,
 *      e.g. if you have the folder structure:
 *     root  --|--[telecom] ---[pb]--- 0.vcf
 *             |
 *             |--[SIM1]---[telecom]---[pb]--- 0.vcf
 *      and you download /telecom/pb/0.vcf
 *      then you must call CsrBtPacSetFolderReqSend twice; one to set folder to telecom
 *      and one afterward to set folder to pb.
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtPacSetFolderReqSend(_ucs2name) {                           \
        CsrBtPacSetFolderReq *msg__ = (CsrBtPacSetFolderReq *)CsrPmemAlloc(sizeof(CsrBtPacSetFolderReq)); \
        msg__->type = CSR_BT_PAC_SET_FOLDER_REQ;                        \
        msg__->ucs2name= _ucs2name;                                     \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacSetBackFolderReqSend
 *
 *  DESCRIPTION
 *      Try top set the path one step back.
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtPacSetBackFolderReqSend() {                                \
        CsrBtPacSetBackFolderReq *msg__ =(CsrBtPacSetBackFolderReq *) CsrPmemAlloc(sizeof(CsrBtPacSetBackFolderReq)); \
        msg__->type = CSR_BT_PAC_SET_BACK_FOLDER_REQ;                   \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacSetRootFolderReqSend
 *
 *  DESCRIPTION
 *      Set path to root.
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtPacSetRootFolderReqSend() {                                \
        CsrBtPacSetRootFolderReq *msg__ = (CsrBtPacSetRootFolderReq *)CsrPmemAlloc(sizeof(CsrBtPacSetRootFolderReq)); \
        msg__->type = CSR_BT_PAC_SET_ROOT_FOLDER_REQ;                   \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacPullVcardListReqSend
 *
 *  DESCRIPTION
 *      Try to download a the phonebook-listing object.
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtPacPullVcardListReqSend(_ucs2name, _order, _searchVal, _searchAtt, _maxListCnt, _listStartOffset, _srmpOn) { \
        CsrBtPacPullVcardListReq *msg__ =  (CsrBtPacPullVcardListReq *)CsrPmemAlloc(sizeof(CsrBtPacPullVcardListReq)); \
        msg__->type = CSR_BT_PAC_PULL_VCARD_LIST_REQ;                   \
        msg__->ucs2name= _ucs2name;                                     \
        msg__->order = _order;                                          \
        msg__->searchVal = _searchVal;                                  \
        if (_searchVal)                                                 \
        {                                                               \
            msg__->searchValLen = (CsrUint16)(CsrStrLen((char*)_searchVal) + 1); \
        }                                                               \
        else                                                            \
        {                                                               \
            msg__->searchValLen = 0;                                    \
        }                                                               \
        msg__->searchAtt = _searchAtt;                                  \
        msg__->maxListCnt = _maxListCnt;                                \
        msg__->listStartOffset = _listStartOffset;                      \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacPullVcardListResSend
 *
 *  DESCRIPTION
 *      Try to download a the phonebook-listing object.
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtPacPullVcardListResSend(_srmpOn) {                         \
        CsrBtPacPullVcardListRes *msg__ = (CsrBtPacPullVcardListRes *)CsrPmemAlloc(sizeof(CsrBtPacPullVcardListRes)); \
        msg__->type = CSR_BT_PAC_PULL_VCARD_LIST_RES;                   \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacPullVcardEntryReqSend
 *
 *  DESCRIPTION
 *      Try to download a entry from the phonebook.
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtPacPullVcardEntryReqSend(_ucs2name, _filter, _format, _srmpOn) { \
        CsrBtPacPullVcardEntryReq *msg__ =(CsrBtPacPullVcardEntryReq *) CsrPmemAlloc(sizeof(CsrBtPacPullVcardEntryReq)); \
        msg__->type = CSR_BT_PAC_PULL_VCARD_ENTRY_REQ;                  \
        msg__->ucs2name= _ucs2name;                                     \
        CsrMemCpy(msg__->filter, _filter, sizeof(msg__->filter));       \
        msg__->format = _format;                                        \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacPullVcardEntryResSend
 *
 *  DESCRIPTION
 *      Try to download a entry from the phonebook.
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtPacPullVcardEntryResSend(_srmpOn) {                        \
        CsrBtPacPullVcardEntryRes *msg__ = (CsrBtPacPullVcardEntryRes *)CsrPmemAlloc(sizeof(CsrBtPacPullVcardEntryRes)); \
        msg__->type = CSR_BT_PAC_PULL_VCARD_ENTRY_RES;                  \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacAbortReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      None
 *----------------------------------------------------------------------------*/
#define CsrBtPacAbortReqSend() {                                        \
        CsrBtPacAbortReq *msg__ = (CsrBtPacAbortReq *)CsrPmemAlloc(sizeof(CsrBtPacAbortReq)); \
        msg__->type = CSR_BT_PAC_ABORT_REQ;                             \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacDisconnectReqSend
 *
 *  DESCRIPTION
 *      The OBEX - and the Bluetooth connection is release
 *
 *  PARAMETERS
 *      theNormalDisconnect : FALSE defines an Abnormal disconnect sequence where
 *                            the Bluetooth connection is release direct. TRUE
 *                            defines a normal disconnect sequence where the OBEX
 *                            connection is release before the Bluetooth connection
 *----------------------------------------------------------------------------*/
#define CsrBtPacDisconnectReqSend(_normalDisconnect) {                  \
        CsrBtPacDisconnectReq *msg__ = (CsrBtPacDisconnectReq *)CsrPmemAlloc(sizeof(CsrBtPacDisconnectReq)); \
        msg__->type = CSR_BT_PAC_DISCONNECT_REQ;                        \
        msg__->normalDisconnect = _normalDisconnect;                    \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacCancelConnectReqSend
 *
 *  DESCRIPTION
 *      Try to cancel the connection which is being establish to a peer device.
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtPacCancelConnectReqSend() {                                \
        CsrBtPacCancelConnectReq *msg__ =(CsrBtPacCancelConnectReq *) CsrPmemAlloc(sizeof(CsrBtPacCancelConnectReq)); \
        msg__->type = CSR_BT_PAC_CANCEL_CONNECT_REQ;                    \
        CsrBtPacMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacSecurityOutReqSend
 *
 *  DESCRIPTION
 *      Set the default security settings for new incoming/outgoing connections
 *
 *  PARAMETERS
 *       secLevel        The security level to use
 *
 *----------------------------------------------------------------------------*/
#define CsrBtPacSecurityOutReqSend(_appHandle, _secLevel) {             \
        CsrBtPacSecurityOutReq *msg;                                    \
        msg = (CsrBtPacSecurityOutReq*)CsrPmemAlloc(sizeof(CsrBtPacSecurityOutReq)); \
        msg->type = CSR_BT_PAC_SECURITY_OUT_REQ;                        \
        msg->appHandle = _appHandle;                                    \
        msg->secLevel = _secLevel;                                      \
        CsrBtPacMsgTransport(msg);}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPacFreeUpstreamMessageContents
 *
 *  DESCRIPTION
 *      During Bluetooth shutdown all allocated payload in the Synergy BT PAC
 *      message must be deallocated. This is done by this function
 *
 *
 *    PARAMETERS
 *      eventClass :  Must be CSR_BT_PAC_PRIM,
 *      msg:          The message received from Synergy BT PAC
 *----------------------------------------------------------------------------*/
void CsrBtPacFreeUpstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif


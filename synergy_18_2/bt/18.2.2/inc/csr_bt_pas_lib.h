#ifndef CSR_BT_PAS_LIB_H__
#define CSR_BT_PAS_LIB_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_msg_transport.h"
#include "csr_bt_profiles.h"
#include "csr_bt_pas_prim.h"
#include "csr_bt_tasks.h"
#include "csr_pmem.h"
#include "csr_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Common put_message function to reduce code size */
void CsrBtPasMsgTransport(void* msg);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasActivateReqSend
 *
 *  DESCRIPTION
 *      Set the Phonebook Access server in discoverable mode and in connectable mode
 *
 *  PARAMETERS
 *      hApp:       ...
 *      feature:    ...
 *      repository: ...
 *
 *----------------------------------------------------------------------------*/
#define CsrBtPasActivateReqSend(_hApp, _feature, _repository, _obexMaxPacketSize, _windowSize, _srmEnable) { \
        CsrBtPasActivateReq *msg__ = CsrPmemAlloc(sizeof(CsrBtPasActivateReq)); \
        msg__->type = CSR_BT_PAS_ACTIVATE_REQ;                          \
        msg__->appHandle = _hApp;                                       \
        msg__->feature = _feature;                                      \
        msg__->repository = _repository;                                \
        msg__->obexMaxPacketSize = _obexMaxPacketSize;                  \
        msg__->windowSize = _windowSize;                                \
        msg__->srmEnable = _srmEnable;                                  \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasDeactivateReqSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      None.
 *----------------------------------------------------------------------------*/
#define CsrBtPasDeactivateReqSend() {                                   \
        CsrBtPasDeactivateReq *msg__ = CsrPmemAlloc(sizeof(CsrBtPasDeactivateReq)); \
        msg__->type = CSR_BT_PAS_DEACTIVATE_REQ;                        \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasConnectResSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      obexMaxPacketSize:      ...
 *      connectionId:           ...
 *      responseCode:                 ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasConnectResSend(_connectionId, _responseCode) {          \
        CsrBtPasConnectRes *msg__ = CsrPmemAlloc(sizeof(CsrBtPasConnectRes)); \
        msg__->type              = CSR_BT_PAS_CONNECT_RES;              \
        msg__->connectionId      = _connectionId;                       \
        msg__->responseCode      = _responseCode;                       \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      PasAuthReq
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      realmLength:        ...
 *      realm:              ...
 *      passwordLength:     ...
 *      password:           ...
 *      userId:             ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasAuthReqSend(_realmLength,_realm,_passwordLength,_password,_userId) { \
        CsrBtPasAuthenticateReq *msg__ = CsrPmemAlloc(sizeof(CsrBtPasAuthenticateReq)); \
        msg__->type = CSR_BT_PAS_AUTHENTICATE_REQ;                      \
        msg__->realmLength = _realmLength;                              \
        msg__->realm = _realm;                                          \
        msg__->password = _password;                                    \
        msg__->passwordLength = _passwordLength;                        \
        msg__->userId = _userId;                                        \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      PasAuthRes
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      passwordLength:     ...
 *      password:           ...
 *      userId:             ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasAuthResSend(_passwordLength,_password,_userId) {        \
        CsrBtPasAuthenticateRes *msg__ = CsrPmemAlloc(sizeof(CsrBtPasAuthenticateRes)); \
        msg__->type = CSR_BT_PAS_AUTHENTICATE_RES;                      \
        msg__->password = _password;                                    \
        msg__->passwordLength = _passwordLength;                        \
        msg__->userId = _userId;                                        \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasPullPbResSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      connectionId:    ...
 *      responseCode:          ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasPullPbResSend(_connectionId, _responseCode) {           \
        CsrBtPasPullPbRes *msg__ = CsrPmemAlloc(sizeof(CsrBtPasPullPbRes)); \
        msg__->type         = CSR_BT_PAS_PULL_PB_RES;                   \
        msg__->connectionId = _connectionId;                            \
        msg__->responseCode = _responseCode;                            \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasPullPbSizeResSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      connectionId:    ...
 *      responseCode:          ...
 *      pbSize:          ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasPullPbSizeResSend(_connectionId, _responseCode, _pbSize, _srmpOn) { \
        CsrBtPasPullPbSizeRes *msg__ = CsrPmemAlloc(sizeof(CsrBtPasPullPbSizeRes)); \
        msg__->type          = CSR_BT_PAS_PULL_PB_SIZE_RES;             \
        msg__->connectionId  = _connectionId;                           \
        msg__->pbSize        = _pbSize;                                 \
        msg__->responseCode  = _responseCode;                           \
        msg__->srmpOn        = _srmpOn;                                 \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasPullMchResSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      connectionId:    ...
 *      responseCode:          ...
 *      newMissedCall:   ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasPullMchResSend(_connectionId, _responseCode, _newMissedCalls, _srmpOn) { \
        CsrBtPasPullMchRes *msg__ = CsrPmemAlloc(sizeof(CsrBtPasPullMchRes)); \
        msg__->type               = CSR_BT_PAS_PULL_MCH_RES;            \
        msg__->connectionId       = _connectionId;                      \
        msg__->responseCode       = _responseCode;                      \
        msg__->newMissedCall      = _newMissedCalls;                    \
        msg__->srmpOn             = _srmpOn;                            \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasPullMchSizeResSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      connectionId:    ...
 *      responseCode:          ...
 *      pbSize:          ...
 *      newMissedCall:   ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasPullMchSizeResSend(_connectionId, _responseCode, _pbSize, _newMissedCalls, _srmpOn) { \
        CsrBtPasPullMchSizeRes *msg__ = CsrPmemAlloc(sizeof(CsrBtPasPullMchSizeRes)); \
        msg__->type          = CSR_BT_PAS_PULL_MCH_SIZE_RES;            \
        msg__->connectionId  = _connectionId;                           \
        msg__->responseCode  = _responseCode;                           \
        msg__->pbSize        = _pbSize;                                 \
        msg__->newMissedCall = _newMissedCalls;                         \
        msg__->srmpOn        = _srmpOn;                                 \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasSetFolderResSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      connectionId:    ...
 *      responseCode:          ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasSetFolderResSend(_connectionId, _responseCode) {        \
        CsrBtPasSetFolderRes *msg__ = CsrPmemAlloc(sizeof(CsrBtPasSetFolderRes)); \
        msg__->type          = CSR_BT_PAS_SET_FOLDER_RES;               \
        msg__->connectionId  = _connectionId;                           \
        msg__->responseCode  = _responseCode;                           \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasPullVcardListResSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      connectionId:    ...
 *      responseCode:          ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasPullVcardListResSend(_connectionId, _responseCode, _srmpOn) { \
        CsrBtPasPullVcardListRes *msg__ = CsrPmemAlloc(sizeof(CsrBtPasPullVcardListRes)); \
        msg__->type          = CSR_BT_PAS_PULL_VCARD_LIST_RES;          \
        msg__->connectionId  = _connectionId;                           \
        msg__->responseCode  = _responseCode;                           \
        msg__->srmpOn        = _srmpOn;                                 \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasPullVcardListSizeResSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      connectionId:    ...
 *      responseCode:          ...
 *      pbSize:          ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasPullVcardListSizeResSend(_connectionId, _responseCode, _pbSize, _srmpOn) { \
        CsrBtPasPullVcardListSizeRes *msg__ = CsrPmemAlloc(sizeof(CsrBtPasPullVcardListSizeRes)); \
        msg__->type = CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES;              \
        msg__->connectionId = _connectionId;                            \
        msg__->responseCode = _responseCode;                            \
        msg__->pbSize       = _pbSize;                                  \
        msg__->srmpOn       = _srmpOn;                                  \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasPullVcardMchListResSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      connectionId:    ...
 *      responseCode:          ...
 *      newMissedCall:   ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasPullVcardMchListResSend(_connectionId, _responseCode, _newMissedCall, _srmpOn) { \
        CsrBtPasPullVcardMchListRes *msg__ = CsrPmemAlloc(sizeof(CsrBtPasPullVcardMchListRes)); \
        msg__->type          = CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES;      \
        msg__->connectionId  = _connectionId;                           \
        msg__->responseCode  = _responseCode;                           \
        msg__->newMissedCall = _newMissedCall;                          \
        msg__->srmpOn        = _srmpOn;                                 \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasPullVcardMchListSizeResSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      connectionId:    ...
 *      responseCode:          ...
 *      pbSize:          ...
 *      newMissedCall:   ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasPullVcardMchListSizeResSend(_connectionId, _responseCode, _pbSize, _newMissedCall, _srmpOn) { \
        CsrBtPasPullVcardMchListSizeRes *msg__ = CsrPmemAlloc(sizeof(CsrBtPasPullVcardMchListSizeRes)); \
        msg__->type          = CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES; \
        msg__->connectionId  = _connectionId;                           \
        msg__->responseCode  = _responseCode;                           \
        msg__->pbSize        = _pbSize;                                 \
        msg__->newMissedCall = _newMissedCall;                          \
        msg__->srmpOn        = _srmpOn;                                 \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasPullVcardEntryResSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      connectionId:    ...
 *      finalFlag:       ...
 *      responseCode:          ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasPullVcardEntryResSend(_connectionId, _finalFlag, _responseCode, _srmpOn) { \
        CsrBtPasPullVcardEntryRes *msg__ = CsrPmemAlloc(sizeof(CsrBtPasPullVcardEntryRes)); \
        msg__->type          = CSR_BT_PAS_PULL_VCARD_ENTRY_RES;         \
        msg__->connectionId  = _connectionId;                           \
        msg__->finalFlag     = _finalFlag;                              \
        msg__->responseCode  = _responseCode;                           \
        msg__->srmpOn        = _srmpOn;                                 \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasNextResSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *      connectionId:    ...
 *      finalFlag:       ...
 *      responseCode:          ...
 *      body:            ...
 *      bodyLen:         ...
 *----------------------------------------------------------------------------*/
#define CsrBtPasNextResSend(_connectionId, _finalFlag, _responseCode, _bodyLen, _body, _srmpOn) { \
        CsrBtPasNextRes *msg__ = CsrPmemAlloc(sizeof(CsrBtPasNextRes)); \
        msg__->type          = CSR_BT_PAS_NEXT_RES;                     \
        msg__->connectionId  = _connectionId;                           \
        msg__->finalFlag     = _finalFlag;                              \
        msg__->responseCode  = _responseCode;                           \
        msg__->body          = _body;                                   \
        msg__->bodyLength    = _bodyLen;                                \
        msg__->srmpOn        = _srmpOn;                                 \
        CsrBtPasMsgTransport(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasSecurityInReqSend
 *
 *  DESCRIPTION
 *      Set the default security settings for new incoming/outgoing connections
 *
 *  PARAMETERS
 *       secLevel        The security level to use
 *
 *----------------------------------------------------------------------------*/
#define CsrBtPasSecurityInReqSend(_appHandle, _secLevel) {              \
        CsrBtPasSecurityInReq *msg = (CsrBtPasSecurityInReq*)CsrPmemAlloc(sizeof(CsrBtPasSecurityInReq)); \
        msg->type = CSR_BT_PAS_SECURITY_IN_REQ;                         \
        msg->appHandle = _appHandle;                                    \
        msg->secLevel = _secLevel;                                      \
        CsrBtPasMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtPasFreeUpstreamMessageContents
 *
 *  DESCRIPTION
 *      During Bluetooth shutdown all allocated payload in the Synergy BT PAS
 *      message must be deallocated. This is done by this function
 *
 *
 *    PARAMETERS
 *      eventClass :  Must be CSR_BT_PAS_PRIM,
 *      msg:          The message received from Synergy BT PAS
 *----------------------------------------------------------------------------*/
void CsrBtPasFreeUpstreamMessageContents(CsrUint16 eventClass, void * message);



#ifdef __cplusplus
}
#endif

#endif


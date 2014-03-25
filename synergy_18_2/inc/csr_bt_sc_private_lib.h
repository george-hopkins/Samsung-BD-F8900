#ifndef CSR_BT_SC_PRIVATE_LIB_H__
#define CSR_BT_SC_PRIVATE_LIB_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_result.h"
#include "csr_bt_profiles.h"
#include "csr_bt_sc_db.h"
#include "dm_prim.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_private_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Common put_message function to reduce code size */
void CsrBtScMessagePutDownstream(void* msg);
void CsrBtScMsgTransport(void* msg);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScSetSecInLevel
 *
 *  DESCRIPTION
 *
 *  PARAMETERS
 *      secInLevel:         the mapped security settings
 *      secLevel:           the security level
 *      secManLevel:        the mandatory security level
 *      secDefLevel:        the default security level
 *      successCode:        the value to return if operation succeeds
 *      errorCode:          the value to return if operation fails
 *----------------------------------------------------------------------------*/
CsrBtResultCode CsrBtScSetSecInLevel(CsrUint16 *secOutLevel,
                                     CsrUint16 secLevel,
                                     CsrUint16 secManLevel,
                                     CsrUint16 secDefLevel,
                                     CsrBtResultCode successCode,
                                     CsrBtResultCode errorCode);
void CsrBtScMapSecInLevel(CsrUint16 secInput, CsrUint16 *secOutput);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScSetSecOutLevel
 *
 *  DESCRIPTION
 *
 *  PARAMETERS
 *      secOutLevel:        the mapped security settings
 *      secLevel:           the security level
 *      secManLevel:        the mandatory security level
 *      secDefLevel:        the default security level
 *      successCode:        the value to return if operation succeeds
 *      errorCode:          the value to return if operation fails
 *----------------------------------------------------------------------------*/
CsrBtResultCode CsrBtScSetSecOutLevel(CsrUint16 *secOutLevel,
                                      CsrUint16 secLevel,
                                      CsrUint16 secManLevel,
                                      CsrUint16 secDefLevel,
                                      CsrBtResultCode successCode,
                                      CsrBtResultCode errorCode);
void CsrBtScMapSecOutLevel(CsrUint16 secInput, CsrUint16 *secOutput);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScRegisterReqSend
 *
 *  DESCRIPTION
 *      Function to build and send a CSR_BT_SC_REGISTER_REQ message to the SC message
 *      queue.
 *
 *  PARAMETERS
 *      profileUuid:        Service uuid16_t for server service
 *      protocolId:         Protocol at which security is to be applied
 *      secLevel:           Level of security to be applied
 *      channel:            Channel for that protocol
 *      outgoingOk:         TRUE if this also applies to outgoing connections
 *----------------------------------------------------------------------------*/
#define CsrBtScRegisterReqSend(_theProfileUuid, _theChannel, _outgoingOk, _theProtocolId, _theSecLevel) { \
        CsrBtScRegisterReq *msg__ = (CsrBtScRegisterReq *) CsrPmemAlloc(sizeof(CsrBtScRegisterReq)); \
        msg__->type = CSR_BT_SC_REGISTER_REQ;                           \
        msg__->profileUuid = _theProfileUuid;                           \
        msg__->protocolId = _theProtocolId;                             \
        msg__->secLevel = _theSecLevel;                                 \
        msg__->channel = _theChannel;                                   \
        msg__->outgoingOk = _outgoingOk;                                \
        CsrBtScMessagePutDownstream(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScDeregisterReqSend
 *
 *  DESCRIPTION
 *      Function to build and send a CSR_BT_SC_DEREGISTER_REQ message to the SC message
 *      queue.
 *
 *  PARAMETERS
 *      protocolId:         Protocol at which security is to be applied
 *      channel:            Channel for that protocol
 *----------------------------------------------------------------------------*/
void CsrBtScDeregisterReqSend(dm_protocol_id_t protocolId, CsrUint16 channel);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScDatabaseReqSend
 *
 *  DESCRIPTION
 *      Function to build and send a CSR_BT_SC_DATABASE_REQ message to the SC message
 *      queue.
 *
 *  PARAMETERS
 *      appHandle   where the CFM shall be sent to
 *      deviceAddr  what BT device in the database to access
 *      opcode      DB operation code
 *      entry       what DB entry (link key index) to access
 *      keyType     key type of data (only used during write operations)
 *      length      length of data
 *      data        pointer to data
 *----------------------------------------------------------------------------*/
#define CsrBtScDatabaseReqSend(_appHandle, _deviceAddr, _opcode, _entry, _keyType, _length, _data) { \
        CsrBtScDatabaseReq *msg__ = (CsrBtScDatabaseReq*)CsrPmemAlloc(sizeof(CsrBtScDatabaseReq)); \
        msg__->type = CSR_BT_SC_DATABASE_REQ;                           \
        msg__->appHandle = _appHandle;                                  \
        msg__->deviceAddr = *_deviceAddr;                               \
        msg__->opcode = _opcode;                                        \
        msg__->entry = _entry;                                          \
        msg__->keyType = _keyType;                                      \
        msg__->length = _length;                                        \
        msg__->addressType = CSR_BT_ADDR_PUBLIC;                        \
        if(_length > 0)                                                 \
        {                                                               \
            msg__->data = (CsrUint8*)CsrPmemAlloc(_length);             \
            CsrMemCpy(msg__->data, _data, _length);                     \
        }                                                               \
        else                                                            \
        {                                                               \
            msg__->data = NULL;                                         \
        }                                                               \
        CsrBtScMessagePutDownstream(msg__);}


#ifdef CSR_BT_LE_ENABLE

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScLeSecurityReqSend
 *
 *  DESCRIPTION
 *      Send a LE security request to DM_SM/SM.
 *
 *----------------------------------------------------------------------------*/
#define CsrBtScLeSecurityReqSend(_appHandle, _securityRequirements, _address, _l2caConFlags) { \
        CsrBtScLeSecurityReq *msg__ = (CsrBtScLeSecurityReq*)CsrPmemAlloc(sizeof(CsrBtScLeSecurityReq)); \
        msg__->type = CSR_BT_SC_LE_SECURITY_REQ;                        \
        msg__->phandle = _appHandle;                                    \
        msg__->securityRequirements = _securityRequirements;            \
        CsrBtAddrCopy(&msg__->address, &_address);                      \
        msg__->l2caConFlags = _l2caConFlags;                            \
        CsrBtScMessagePutDownstream(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScLeAddServiceChangedReqSend
 *
 *  DESCRIPTION
 *      Add the Attribute Handle of the Service Changed Characteristic Value
 *
 *----------------------------------------------------------------------------*/
#define CsrBtScLeAddServiceChangedReqSend(_addr, _addressType, _serviceChangedHandle) { \
        CsrBtScLeAddServiceChangedReq *msg__ = (CsrBtScLeAddServiceChangedReq*)CsrPmemAlloc(sizeof(CsrBtScLeAddServiceChangedReq)); \
        msg__->type = CSR_BT_SC_LE_ADD_SERVICE_CHANGED_REQ;             \
        msg__->serviceChangedHandle = _serviceChangedHandle;            \
        msg__->addr = _addr;                                            \
        msg__->addressType = _addressType;                              \
        CsrBtScMessagePutDownstream(msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtScReadDeviceRecordWithHdlPairsReqSend
 *
 *  DESCRIPTION
 *      Function to read the entire SC DB
 *
 *    PARAMETERS
 *        phandle:        handle to the queue where SC must return indications and confirm
 *        maxNumOfBytes:    maximum allowed bytes in a result indication
 *
 *----------------------------------------------------------------------------*/
#define CsrBtScReadDeviceRecordWithHdlPairsReqSend(_phandle, _maxNumOfBytes) { \
        CsrBtScReadDeviceRecordReq *msg__ = (CsrBtScReadDeviceRecordReq *) CsrPmemAlloc(sizeof(CsrBtScReadDeviceRecordReq)); \
        msg__->type = CSR_BT_SC_READ_DEVICE_RECORD_REQ;                 \
        msg__->maxNumOfBytes = _maxNumOfBytes;                          \
        msg__->phandle = _phandle;                                      \
        msg__->flags   = CSR_BT_SC_ADD_HDL_PAIRS;                       \
        CsrBtScMsgTransport(msg__);}

#endif /* CSR_BT_LE_ENABLE */

#ifdef __cplusplus
}
#endif

#endif


#ifndef CSR_BT_SD_PRIVATE_LIB_H__
#define CSR_BT_SD_PRIVATE_LIB_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_bt_profiles.h"
#include "csr_bt_sd_private_prim.h"
#include "csr_bt_sd_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtSdRegisterAvailableServiceReqSend
 *
 *  DESCRIPTION
 *      Function to build and send a CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ message
 *      to the SD message queue.
 *
 *  PARAMETERS
 *      service:  The service a given client wants to search/connect to
 *----------------------------------------------------------------------------*/
void CsrBtSdRegisterAvailableServiceReqSend(CsrBtUuid32 service);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtSdUpdateDeviceReqSend
 *
 *  DESCRIPTION
 *
 *  PARAMETERS
 *      details: Use the SD_UPDATE_DEVICE_[...] flags specified in csr_bt_sd_private_prim.h
 *----------------------------------------------------------------------------*/
#define CsrBtSdUpdateDeviceReqSend(_addr,_cod,_details) {               \
        CsrBtSdUpdateDeviceReq *msg__;                                  \
        msg__ = (CsrBtSdUpdateDeviceReq*)CsrPmemAlloc(sizeof(CsrBtSdUpdateDeviceReq)); \
        msg__->type = CSR_BT_SD_UPDATE_DEVICE_REQ;                      \
        msg__->addr = _addr;                                            \
        msg__->cod = _cod;                                              \
        msg__->details = _details;                                      \
        msg__->addressType = CSR_BT_ADDR_PUBLIC;                        \
        msg__->serviceChangeHandle = CSR_BT_GATT_SERVICE_CHANGED_NOT_FOUND; \
        CsrBtSdMsgTransport(msg__);}

#define CsrBtSdUpdateDeviceReqSendEx(_addr,_cod,_details,_addressType) { \
        CsrBtSdUpdateDeviceReq *msg__;                                  \
        msg__ = (CsrBtSdUpdateDeviceReq*)CsrPmemAlloc(sizeof(CsrBtSdUpdateDeviceReq)); \
        msg__->type = CSR_BT_SD_UPDATE_DEVICE_REQ;                      \
        msg__->addr = _addr;                                            \
        msg__->cod = _cod;                                              \
        msg__->details = _details;                                      \
        msg__->addressType = _addressType;                              \
        msg__->serviceChangeHandle = CSR_BT_GATT_SERVICE_CHANGED_NOT_FOUND; \
        CsrBtSdMsgTransport(msg__);}

#define CsrBtSdUpdateServiceChangedReqSend(_addr,_addressType,_serviceChangeHandle) { \
        CsrBtSdUpdateDeviceReq *msg__;                                  \
        msg__ = (CsrBtSdUpdateDeviceReq*)CsrPmemAlloc(sizeof(CsrBtSdUpdateDeviceReq)); \
        msg__->type = CSR_BT_SD_UPDATE_DEVICE_REQ;                      \
        msg__->addr = _addr;                                            \
        msg__->cod = 0;                                                 \
        msg__->details = SD_UPDATE_SERVICE_CHANGED;                     \
        msg__->addressType = _addressType;                              \
        msg__->serviceChangeHandle = _serviceChangeHandle;              \
        CsrBtSdMsgTransport(msg__);}

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_SD_PRIVATE_LIB_H__ */


#ifndef CSR_BT_GATT_SEF_H__
#define CSR_BT_GATT_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/
#include "csr_bt_gatt_main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Covers Registration and Un-register an application instance to Gatt */
CsrBool CsrBtGattRegisterReqHandler(GattMainInst *inst);
CsrBool CsrBtGattUnregisterReqHandler(GattMainInst *inst);

/* Covers DataBase Configuration */
CsrBool CsrBtGattDbAllocReqHandler(GattMainInst *inst);
CsrBool CsrBtGattDbDeallocReqHandler(GattMainInst *inst);
CsrBool CsrBtGattDbAddReqHandler(GattMainInst *inst);
CsrBool CsrBtGattDbRemoveReqHandler(GattMainInst *inst);
CsrBool CsrBtGattDbAccessResHandler(GattMainInst *inst);

/* Covers Server Initiated Notification and Indication  */
CsrBool CsrBtGattEventSendReqHandler(GattMainInst *inst);

/* Covers LE connection oriented procedures */
CsrBool CsrBtGattCentralReqHandler(GattMainInst *inst);
CsrBool CsrBtGattPeripheralReqHandler(GattMainInst *inst);
CsrBool CsrBtGattBredrConnectReqHandler(GattMainInst *inst);
CsrBool CsrBtGattBredrAcceptReqHandler(GattMainInst *inst);
CsrBool CsrBtGattDisconnectReqHandler(GattMainInst *inst);

/* Covers LE observe/broadcast procedures */
CsrBool CsrBtGattScanReqHandler(GattMainInst *inst);
CsrBool CsrBtGattAdvertiseReqHandler(GattMainInst *inst);

/* LE scan/advertise/connection parameters */
CsrBool CsrBtGattParamScanReqHandler(GattMainInst *inst);
CsrBool CsrBtGattParamAdvertiseReqHandler(GattMainInst *inst);
#ifdef CSR_BT_INSTALL_GATT_PARAM_CONNECTION
CsrBool CsrBtGattParamConnectionReqHandler(GattMainInst *inst);
#endif
CsrBool CsrBtGattParamConnUpdateReqHandler(GattMainInst *inst);
CsrBool CsrBtGattParamConnUpdateResHandler(GattMainInst *inst);

/* Covers SDS Register/Unregister Error handling */
void CsrBtGattSdsRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu);
void CsrBtGattSdsRequestHandler(GattMainInst *inst, CsrUint16 event);

/* Covers item 1, Server Configuration, in the GATT feature table */
void CsrBtGattExchangeMtuHandler(GattMainInst   *inst, 
                                 CsrUint16      mtu, 
                                 CsrUint16      cid, 
                                 CsrBtTypedAddr address);

/* Covers item 2, Primary Service Discovery, in the GATT feature table  */
CsrBool CsrBtGattDiscoverServicesReqHandler(GattMainInst *inst);

/* Covers item 3, Relationship Discovery, in the GATT feature table */
CsrBool CsrBtGattFindInclServicesReqHandler(GattMainInst *inst);

/* Covers item 4, Characteristic Discovery, in the GATT feature table */
CsrBool CsrBtGattDiscoverCharacReqHandler(GattMainInst *inst);

/* Covers item 5, Characteristic Descriptor Discovery, in the GATT feature table */
CsrBool CsrBtGattDiscoverCharacDescriptorsReqHandler(GattMainInst *inst);

/* Covers item 6, Characteristic Value Read, and  item 10, 
   Read Characteristic Descriptor Value,in the GATT feature table */
CsrBool CsrBtGattReadReqHandler(GattMainInst *inst);
CsrBool CsrBtGattReadByUuidReqHandler(GattMainInst *inst);
CsrBool CsrBtGattReadMultiReqHandler(GattMainInst *inst);
CsrBool CsrBtGattReadByUuidSecurityHandler(void            *gattInst,
                                           void            *qElem,
                                           CsrBtResultCode result, 
                                           CsrBtSupplier   supplier);

/* Covers item 7, Characteristic Value Write, in the GATT feature table */
void CsrBtGattGetAttPrepareWriteSend(CsrBtGattQueueElement *qElem,
                                     CsrUint16             prepareWriteOffset,
                                     CsrUint16             mtu,
                                     CsrUint16             valueLength,
                                     CsrUint8              *value);

CsrBool CsrBtGattWriteReqHandler(GattMainInst *inst);
CsrBool CsrBtGattPrepareWriteSecurityHandler(void            *gattInst, 
                                             void            *qElem,
                                             CsrBtResultCode result, 
                                             CsrBtSupplier   supplier);

/* Covers item 8 and 9, Characteristic Value Indication/Notification, in the GATT feature table */
CsrBool CsrBtGattWriteClientConfigurationReqHandler(GattMainInst *inst);
CsrBool CsrBtGattSubscriptionReqHandler(GattMainInst *inst);

/* Event Mask Handler*/
CsrBool CsrBtGattSetEventMaskReqHandler(GattMainInst *inst);

/* Cancel */
CsrBool CsrBtGattCancelReqHandler(GattMainInst *inst);

/*  Checks if the handle of the Service Changed Characteristic Value is known or not */
void CsrBtGattFindServiceChangedHandler(GattMainInst *inst, CsrUint16 cid, CsrBtTypedAddr address);

/* Read Remote Le name via ATT */
void CsrBtGattReadRemoteLeNameHandler(GattMainInst *inst, CsrUint16 cid, l2ca_conflags_t flags);

/* Whitelist control */
CsrBool CsrBtGattWhitelistAddReqHandler(GattMainInst *inst);
CsrBool CsrBtGattWhitelistClearReqHandler(GattMainInst *inst);
CsrBool CsrBtGattWhitelistReadReqHandler(GattMainInst *inst);

/* Security */
CsrBool CsrBtGattSecurityReqHandler(GattMainInst *inst);

/* Prototypes - free_downsteam */
void CsrBtGattFreeDownstreamMessageContents(CsrUint16 eventClass, void *message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_GATT_SEF_H__ */


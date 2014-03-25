#ifndef CSR_BT_GATT_UPSTREAM_H__
#define CSR_BT_GATT_UPSTREAM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_bt_gatt_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtGattMessagePut(CsrSchedQid phandle, void *msg);

void CsrBtGattStdCfmSend(CsrBtGattPrim   type,
                         CsrBtGattId     gattId,
                         CsrBtResultCode resultCode,
                         CsrBtSupplier   resultSupplier);

void CsrBtGattStdBtConnIdCfmSend(CsrBtGattPrim   type,
                                 CsrBtGattId     gattId,
                                 CsrBtResultCode resultCode,
                                 CsrBtSupplier   resultSupplier,
                                  CsrBtConnId     btConnId);

void CsrBtGattDisconnectIndSend(CsrBtGattId gattId,
                                CsrBtResultCode resultCode,
                                CsrBtSupplier resultSupplier,
                                CsrBtConnId btConnId,
                                CsrBtTypedAddr *address,
                                CsrBtGattConnInfo connInfo);

void CsrBtGattConnectIndSend(CsrBtGattPrim type,
                             CsrBtGattId gattId,
                             CsrBtResultCode resultCode,
                             CsrBtSupplier resultSupplier,
                             CsrBtConnId btConnId,
                             CsrBtGattConnInfo connFlags,
                             CsrBtTypedAddr *address,
                             CsrUint16        mtu);

void CsrBtGattRegisterCfmSend(CsrSchedQid       qid,
                              CsrBtGattId       gattId,
                              CsrBtResultCode   resultCode, 
                              CsrBtSupplier     resultSupplier,
                              CsrUint16         context);

void CsrBtGattDbAllocCfmSend(CsrBtGattId       gattId,
                             CsrBtResultCode   resultCode, 
                             CsrBtSupplier     resultSupplier,
                             CsrBtGattHandle   start,
                             CsrBtGattHandle   end,
                             CsrUint16         preferredHandle);

void CsrBtGattDbDeallocCfmSend(CsrBtGattId       gattId,
                               CsrBtResultCode   resultCode, 
                               CsrBtSupplier     resultSupplier,
                               CsrBtGattHandle   start,
                               CsrBtGattHandle   end);

void CsrBtGattDbRemoveCfmSend(CsrBtGattId     gattId,
                              CsrBtResultCode resultCode,
                              CsrBtSupplier   resultSupplier,
                              CsrUint16       numOfAttr);

void CsrBtGattMtuChangedIndSend(CsrBtGattId     gattId,
                                CsrBtConnId     btConnId,
                                CsrUint16       mtu);

void CsrBtGattReportIndSend(CsrBtGattId gattId,
                            CsrBtGattReportEvent eventType,
                            CsrBtTypedAddr *address,
                            CsrBtTypedAddr *permanentAddress,
                            CsrUint8 lengthData,
                            CsrUint8 *data,
                            CsrInt8 rssi);

void CsrBtGattDiscoverServicesIndSend(CsrBtGattQueueElement *qElem,
                                      CsrBtConnId           btConnId,
                                      CsrBtGattHandle       startHandle,
                                      CsrBtGattHandle       endHandle,
                                      CsrUint16             length,
                                      CsrUint8              *data);

void CsrBtGattDiscoverCharacIndSend(CsrBtGattQueueElement   *qElem,
                                    CsrBtConnId             btConnId,
                                    CsrBtGattHandle         declarationHandle,
                                    CsrBtUuid               uuid,
                                    CsrUint8                *data);

void CsrBtGattFindInclServicesIndSend(CsrBtGattQueueElement *qElem,
                                      CsrBtConnId           btConnId,
                                      CsrBtGattHandle       attrHandle,
                                      CsrUint16             length,
                                      CsrUint8              *data);

void CsrBtGattDiscoverCharacDescriptorsIndSend(CsrBtGattQueueElement *qElem,
                                               CsrBtConnId           btConnId,
                                               CsrBtGattHandle       descriptorHandle,
                                               att_uuid_type_t       uuidType,
                                               CsrUint32             *attUuid);

void CsrBtGattReadCfmHandler(CsrBtGattReadReq   *prim,
                             CsrBtResultCode    resultCode,
                             CsrBtSupplier      resultSupplier,
                             CsrUint16          valueLength,
                             CsrUint8           **value);

void CsrBtGattStdReadCfmSend(CsrBtGattPrim   type,
                             CsrBtGattId     gattId,
                             CsrBtResultCode resultCode,
                             CsrBtSupplier   resultSupplier,
                             CsrBtConnId     btConnId,
                             CsrUint16       valueLength,
                             CsrUint8        *value);

void CsrBtGattNotificationIndSend(CsrBtGattId gattId,
                                  CsrBtConnId btConnId,
                                  CsrBtTypedAddr address,
                                  CsrBtGattHandle valueHandle,
                                  CsrUint16 valueLength,
                                  CsrUint8 *value,
                                  CsrBtGattConnInfo connInfo);

void CsrBtGattReadByUuidIndSend(CsrBtGattId     gattId,
                                CsrBtConnId     btConnId,
                                CsrBtGattHandle valueHandle,
                                CsrUint16       valueLength,
                                CsrUint8        *value);

void CsrBtGattWhitelistReadCfmSend(CsrBtGattId gattId,
                                   CsrUint16 addressCount,
                                   CsrBtTypedAddr *address);

void CsrBtGattPhysicalLinkStatusIndSend(CsrBtGattId       gattId, 
                                        CsrBtTypedAddr    address,
                                        CsrBtGattConnInfo connInfo,
                                        CsrBool           status);

void CsrBtGattAccessReadIndSend(CsrSchedQid appHandle,
                                CsrBtGattId gattId,
                                CsrBtConnId btConnId,
                                CsrBtGattHandle handle,
                                CsrUint16 offset,
                                CsrUint16 mtu,
                                CsrBtGattAccessCheck check,
                                CsrBtGattConnInfo connInfo,
                                CsrBtTypedAddr address);

void CsrBtGattAccessWriteIndSend(CsrSchedQid appHandle,
                                 CsrBtGattId gattId,
                                 CsrBtConnId btConnId,
                                 CsrBtGattAccessCheck check,
                                 CsrBtGattConnInfo connInfo,
                                 CsrBtTypedAddr address,
                                 CsrUint16 writeUnitCount,
                                 CsrBtGattAttrWritePairs *writeUnit,
                                 CsrBtGattHandle handle);

void CsrBtGattDbAccessCompleteIndSend(CsrSchedQid appHandle,
                                      CsrBtGattId gattId,
                                      CsrBtConnId btConnId,
                                      CsrBtGattConnInfo connInfo,
                                      CsrBtTypedAddr address,
                                      CsrBtGattHandle attrHandle,
                                      CsrBool commit);

void CsrBtGattWhitelistChangeIndSend(CsrBtGattId gattId);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_GATT_UPSTREAM_H__ */


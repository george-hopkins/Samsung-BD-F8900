#ifndef CSR_BT_GATT_ATT_SEF_H__
#define CSR_BT_GATT_ATT_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_gatt_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtGattAttRegisterCfmHandler(GattMainInst *inst);
void CsrBtGattAttAddCfmHandler(GattMainInst *inst);
void CsrBtGattAttRemoveCfmHandler(GattMainInst *inst);
void CsrBtGattAttConnectCfmHandler(GattMainInst *inst);
void CsrBtGattAttConnectIndHandler(GattMainInst *inst);
void CsrBtGattAttDisconnectIndHandler(GattMainInst *inst);
void CsrBtGattAttExchangeMtuCfmHandler(GattMainInst *inst);
void CsrBtGattAttExchangeMtuIndHandler(GattMainInst *inst);
void CsrBtGattAttFindInfoCfmHandler(GattMainInst *inst);
void CsrBtGattAttFindByTypeValueCfmHandler(GattMainInst *inst);
void CsrBtGattAttReadByTypeCfmHandler(GattMainInst *inst);
void CsrBtGattAttReadCfmHandler(GattMainInst *inst);
void CsrBtGattAttReadBlobCfmHandler(GattMainInst *inst);
void CsrBtGattAttReadMultiCfmHandler(GattMainInst *inst);
void CsrBtGattAttReadByGroupTypeCfmHandler(GattMainInst *inst);
void CsrBtGattAttWriteCfmHandler(GattMainInst *inst);
void CsrBtGattAttPrepareWriteCfmHandler(GattMainInst *inst);
void CsrBtGattAttExecuteWriteCfmHandler(GattMainInst *inst);
void CsrBtGattAttHandleValueCfmHandler(GattMainInst *inst);
void CsrBtGattAttHandleValueIndHandler(GattMainInst *inst);
void CsrBtGattAttAccessIndHandler(GattMainInst *inst);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_GATT_ATT_SEF_H__ */


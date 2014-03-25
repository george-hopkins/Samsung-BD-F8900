/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_gatt_private.h"

static void csrBtGattRemoveNotBondedSubscribeElements(CsrCmnListElm_t *elem, void *value)
{ /* GATT shall remove all the handles the application has subscribe for
     to the given addr as the two devices are not bonded. E.g. the 
     handles cannot be consider as valid any more */
    CsrBtGattAppElement *appElement = (CsrBtGattAppElement *) elem;
    
    CSR_BT_GATT_SUBSCRIBE_INST_ITERATE_REMOVE(appElement->subscribeInst,
                                              CsrBtGattSubscribeInstFindByAddr,
                                              value);
}

void CsrBtGattSdReadDeviceInfoCfmHandler(GattMainInst *inst)
{
    CsrBtTypedAddr address;
    CsrBtSdReadDeviceInfoCfm *prim = (CsrBtSdReadDeviceInfoCfm *)inst->msg;

    address.addr = prim->deviceAddr;
    address.type = prim->addressType;

    if (prim->resultSupplier == CSR_BT_SUPPLIER_SD &&
        prim->resultCode == CSR_BT_RESULT_CODE_SD_SUCCESS &&
        (prim->deviceStatus & CSR_BT_SD_STATUS_PAIRED) == CSR_BT_SD_STATUS_PAIRED)
    { 
        if (prim->context == CSR_BT_GATT_SD_READ_DEVICE_INFO_READ)
        {
            if (prim->serviceChangedHandle != CSR_BT_GATT_SERVICE_CHANGED_NOT_FOUND)
            { /* Stored the handle of the Service Changed Characteristic Value. 
                 Note this handle is remove from GATT when the link is release */                
                CsrBtGattAddHandleForServiceChanged(inst, 
                                                    address, 
                                                    CSR_BT_GATT_GET_SERVICE_CHANGED_HANDLE(prim->serviceChangedHandle));
            }
        }
        /* Else - Context must be CSR_BT_GATT_SD_READ_DEVICE_INFO_DELETE.
                  Nothing to delete because the devices are bonded */
    }
    else if (prim->context == CSR_BT_GATT_SD_READ_DEVICE_INFO_DELETE)
    { /* ATT_DISCONNECT_IND is received and the two devices are not bonded.
         Remove all the handles the application has subscribe for, as there 
         are not valid anymore */   
        CSR_BT_GATT_APP_INST_ITERATE(inst->appInst, 
                                     csrBtGattRemoveNotBondedSubscribeElements, 
                                     &address);
    }
    /* Else - Ignore, SD do not know the handle of the 
              Service Changed Characteristic Value */
}



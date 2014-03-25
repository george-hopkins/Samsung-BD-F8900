#ifndef CSR_BT_UUIDS_H__
#define CSR_BT_UUIDS_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_profiles.h"

#ifdef __cplusplus
extern "C" {
#endif

/* GATT Services */
#define CSR_BT_GAP_UUID                                     ((CsrBtUuid16) 0x1800)
#define CSR_BT_GATT_UUID                                    ((CsrBtUuid16) 0x1801) 

/* GATT Attributes Types */
#define CSR_BT_GATT_PRIMARY_SERVICE_UUID                    ((CsrBtUuid16) 0x2800) 
#define CSR_BT_GATT_SECONDARY_SERVICE_UUID                  ((CsrBtUuid16) 0x2801) 
#define CSR_BT_GATT_INCLUDE_UUID                            ((CsrBtUuid16) 0x2802) 
#define CSR_BT_GATT_CHARACTERISTIC_UUID                     ((CsrBtUuid16) 0x2803) 

/* GATT Characteristic Types */
#define CSR_BT_GATT_DEVICE_NAME_UUID                        ((CsrBtUuid16) 0x2A00) /* Device Name; vol 3 part C section 12.1 */
#define CSR_BT_GATT_APPEARANCE_UUID                         ((CsrBtUuid16) 0x2A01) /* Appearance, vol 3 part C section 12.2 */
#define CSR_BT_GATT_PERIPHERAL_PRIVACY_FLAG_UUID            ((CsrBtUuid16) 0x2A02) /* Peripheral, vol 3 part C section 12.3 */
#define CSR_BT_GATT_RECONNECTION_ADDRESS_UUID               ((CsrBtUuid16) 0x2A03) /* Reconnection Address, vol 3 part C section 12.4 */
#define CSR_BT_GATT_PERIPHERAL_CONNECTION_PARAM_UUID        ((CsrBtUuid16) 0x2A04) /* Peripheral Preferred Connection Parameters, vol 3 part C section 12.5 */
#define CSR_BT_GATT_SERVICE_CHANGED_UUID                    ((CsrBtUuid16) 0x2A05) /* Service Changed; vol 3 part G section 7.1 */

/* GATT Characteristic Descriptor Declarations Types */
#define CSR_BT_GATT_CHARACTERISTIC_EXTENDED_PROPERTIES      ((CsrBtUuid16) 0x2900)
#define CSR_BT_GATT_CHARACTERISTIC_USER_DESCRIPTION         ((CsrBtUuid16) 0x2901)    
#define CSR_BT_GATT_CLIENT_CHARACTERISTIC_CONFIGURATION     ((CsrBtUuid16) 0x2902)
#define CSR_BT_GATT_SERVER_CHARACTERISTIC_CONFIGURATION     ((CsrBtUuid16) 0x2903)
#define CSR_BT_GATT_CHARACTERISTIC_PRESENTATION_FORMAT      ((CsrBtUuid16) 0x2904)
#define CSR_BT_GATT_CHARACTERISTIC_AGGREGATE_FORMAT         ((CsrBtUuid16) 0x2905)

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_UUIDS_H__ */

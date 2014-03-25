#ifndef CSR_BT_GATT_SERVICE_DEFINES_H__
#define CSR_BT_GATT_SERVICE_DEFINES_H__

#include "csr_synergy.h"
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 

REVISION:      $Revision: #4 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_GATT_SERVICE_GAP_UUID                          0x1800 /* GAP service                                  */
#define CSR_BT_GATT_SERVICE_GATT_UUID                         0x1801 /* GATT Service                                 */
#define CSR_BT_GATT_SERVICE_LINKLOSS_UUID                     0x1803 /* Link Loss Service as part of Proximity       */
#define CSR_BT_GATT_SERVICE_IALERT_UUID                       0x1802 /* Immediate alert Service as part of Proximity */
#define CSR_BT_GATT_SERVICE_TX_POWER_UUID                     0x1804 /* Tx Power Service as part of Proximity        */
#define CSR_BT_GATT_SERVICE_THERMOMETER_UUID                  0x1809 /* Health Thermometer Service                   */
#define CSR_BT_GATT_SERVICE_HEALTH_DEV_INFO_UUID              0x180A /* info for Health devices                      */
#define CSR_BT_GATT_SERVICE_BATTERY_UUID                      0x180F /* Battery service                              */

/*Proximity service specific */
#define CSR_BT_GATT_SERVICE_LINKLOSS_CHARAC_UUID                    0x2A06
#define CSR_BT_GATT_SERVICE_IALERT_CHARAC_UUID                      0x2A06
#define CSR_BT_GATT_SERVICE_TX_POWER_CHARAC_UUID                    0x2A07
/* Battery service specific*/
#define CSR_BT_GATT_SERVICE_BATT_MASK_CHARAC_UUID                   0x2A1A
#define CSR_BT_GATT_SERVICE_BATT_LEVEL_CHARAC_UUID                  0x2A19
#define CSR_BT_GATT_SERVICE_BATT_SERVICE_REQUIRED_CHARAC_UUID       0x2A3B
/* Temperature/Health Device Info specific */
#define CSR_BT_GATT_SERVICE_HEALTH_INFO_MODEL_NUM_CHARAC_UUID       0x2A24
#define CSR_BT_GATT_SERVICE_HEALTH_INFO_SERIAL_NUM_CHARAC_UUID      0x2A25
#define CSR_BT_GATT_SERVICE_HEALTH_INFO_FW_REVISION_CHARAC_UUID     0x2A26
#define CSR_BT_GATT_SERVICE_HEALTH_INFO_HW_REVISION_CHARAC_UUID     0x2A27
#define CSR_BT_GATT_SERVICE_HEALTH_INFO_SW_REVISION_CHARAC_UUID     0x2A28
#define CSR_BT_GATT_SERVICE_HEALTH_INFO_MANUFACT_NAME_CHARAC_UUID   0x2A29
/* Thermometer service specific */
#define CSR_BT_GATT_SERVICE_TEMP_MEASUREMENT_CHARAC_UUID            0x2A1C

/* HID service specific */
#define CSR_BT_GATT_SERVICE_HID_REPORT_DESCRIPTOR_CHARAC_UUID  0x2c00
#define CSR_BT_GATT_SERVICE_HID_CONTROL_REPORT_CHARAC_UUID     0x2c02
#define CSR_BT_GATT_SERVICE_HID_INTERUPT_REPORT_CHARAC_UUID    0x2c03

#define CSR_BT_GATT_SERVICE_ALERT_LEVEL_OFF  0
#define CSR_BT_GATT_SERVICE_ALERT_LEVEL_LOW  1
#define CSR_BT_GATT_SERVICE_ALERT_LEVEL_HIGH 2

    

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_GATT_SERVICE_DEFINES_H__ */


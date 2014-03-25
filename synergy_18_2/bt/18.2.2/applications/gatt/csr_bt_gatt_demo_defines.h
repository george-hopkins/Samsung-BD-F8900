#ifndef CSR_BT_GATT_APP_DEFINES_H__
#define CSR_BT_GATT_APP_DEFINES_H__

#include "csr_synergy.h"
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 


REVISION:      $Revision: #4 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_GATT_APP_ALERT_LEVEL_OFF  0
#define CSR_BT_GATT_APP_ALERT_LEVEL_LOW  1
#define CSR_BT_GATT_APP_ALERT_LEVEL_HIGH 2

    /* Characteristic properties bits */
#define CSR_BT_GATT_APP_CHARAC_PROP_BITS                8
#define CSR_BT_GATT_APP_CHARAC_PROP_BROADCAST           0x01
#define CSR_BT_GATT_APP_CHARAC_PROP_READ                0x02
#define CSR_BT_GATT_APP_CHARAC_PROP_WRITE_NO_RESPONSE   0x04
#define CSR_BT_GATT_APP_CHARAC_PROP_WRITE               0x08
#define CSR_BT_GATT_APP_CHARAC_PROP_NOTIFY              0x10
#define CSR_BT_GATT_APP_CHARAC_PROP_INDICATE            0x20
#define CSR_BT_GATT_APP_CHARAC_PROP_AUTH_WRITES         0x40
#define CSR_BT_GATT_APP_CHARAC_PROP_EXTENDED            0x80


    /* UUIDs implemented in demo apps */
    /* Services */
#define CSR_BT_GATT_APP_GAP_UUID              0x1800 /* GAP service*/
#define CSR_BT_GATT_APP_GATT_UUID             0x1801 /* GATT Service */
#define CSR_BT_GATT_APP_LINKLOSS_UUID         0x1803 /* Link Loss Service as part of Proximity */
#define CSR_BT_GATT_APP_IALERT_UUID           0x1802 /* Immediate alert Service as part of Proximity */
#define CSR_BT_GATT_APP_TX_POWER_UUID         0x1804 /* Tx Power Service as part of Proximity */
#define CSR_BT_GATT_APP_CURRENT_TIME_UUID     0x1805 /* Current Time */
#define CSR_BT_GATT_APP_THERMOMETER_UUID      0x1809 /* Health Thermometer Service*/
#define CSR_BT_GATT_APP_DEVICE_INFO_UUID      0x180A /* Device Information Service */
#define CSR_BT_GATT_APP_HEART_RATE_UUID       0x180D /* Heart Rate Service */
#define CSR_BT_GATT_APP_BATTERY_UUID          0x180F /* Battery service */

/* Characteristics */
#define CSR_BT_GATT_APP_ALERT_LEVEL_CHARAC_UUID             0x2A06  /* Alert Level both used by 'link loss' and 'immidiate alert'*/
#define CSR_BT_GATT_APP_APPEARANCE_CHARAC_UUID              0x2A01
#define CSR_BT_GATT_APP_DATE_TIME_CHARAC_UUID               0x2A08
#define CSR_BT_GATT_APP_DEVICE_NAME_CHARAC_UUID             0x2A00
#define CSR_BT_GATT_APP_FIRMWARE_REVISION_CHARAC_UUID       0x2A26
#define CSR_BT_GATT_APP_HARDWARE_REVISION_CHARAC_UUID       0x2A27  /* Hardware Revision String */
#define CSR_BT_GATT_APP_CERT_DATA_CHARAC_UUID               0x2A2A  /* IEEE 11073-20601 Regulatory Certification Data List */
#define CSR_BT_GATT_APP_INTER_TEMP_CHARAC_UUID              0x2A1E  /* Intermediate Temperature */
#define CSR_BT_GATT_APP_MANUFACTURER_CHARAC_UUID            0x2A29  /* Manufacturer Name String */
#define CSR_BT_GATT_APP_MEAS_INTERVAL_CHARAC_UUID           0x2A21  /* Measurement Interval */
#define CSR_BT_GATT_APP_MODEL_NUMBER_CHARAC_UUID            0x2A24  /* Model Number String */
#define CSR_BT_GATT_APP_PREFER_CONN_CHARAC_UUID             0x2A04  /* Peripheral Preferred Connection Parameters */
#define CSR_BT_GATT_APP_PERIPH_PRIVACY_CHARAC_UUID          0x2A02  /* Peripheral Privacy Flag */
#define CSR_BT_GATT_APP_RECONN_ADDR_CHARAC_UUID             0x2A03  /* Reconnection Address */
#define CSR_BT_GATT_APP_SERIAL_NUMBER_CHARAC_UUID           0x2A25  /* Serial Number String */
#define CSR_BT_GATT_APP_SERVICE_CHANGED_CHARAC_UUID         0x2A05  /* Service Changed */
#define CSR_BT_GATT_APP_SOFTWARE_REVISION_CHARAC_UUID       0x2A28  /* Software Revision String */
#define CSR_BT_GATT_APP_SYSTEM_ID_CHARAC_UUID               0x2A23  /* System ID */
#define CSR_BT_GATT_APP_TEMP_TYPE_CHARAC_UUID               0x2A1D  /* Temperature Type */

/* Descriptors */
#define CSR_BT_GATT_APP_AGGREGATE_FORMAT_DESCR_UUID         0x2905 /* Characteristic Aggregate Format */
#define CSR_BT_GATT_APP_EXTENDED_PROP_DESCR_UUID            0x2900 /* Characteristic Extended Properties */
#define CSR_BT_GATT_APP_PRESENTATION_FORMAT_DESCR_UUID      0x2904 /* Characteristic Presentation Format */
#define CSR_BT_GATT_APP_USER_DESCRIPTION_DESCR_UUID         0x2901 /* Characteristic User Description */
#define CSR_BT_GATT_APP_CLIENT_CONFIG_DESCR_UUID            0x2902 /* Client Characteristic Configuration */
#define CSR_BT_GATT_APP_SERVER_CONFIG_DESCR_UUID            0x2903 /* Server Characteristic Configuration */
#define CSR_BT_GATT_APP_VALID_RANGE_DESCR_UUID              0x2906

/*#define CSR_BT_GATT_APP_LINKLOSS_CHARAC_UUID               0x2A06*/
#define CSR_BT_GATT_APP_TX_POWER_CHARAC_UUID               0x2A07
#define CSR_BT_GATT_APP_BATT_MASK_CHARAC_UUID              0x2A1A
#define CSR_BT_GATT_APP_BATT_LEVEL_CHARAC_UUID             0x2A19
#define CSR_BT_GATT_APP_TEMP_MEASUREMENT_CHARAC_UUID       0x2A1C
#define CSR_BT_GATT_APP_BATT_SERVICE_REQUIRED_CHARAC_UUID  0x2A3B
#define CSR_BT_GATT_APP_HID_REPORT_DESCRIPTOR_CHARAC_UUID  0x2c00
#define CSR_BT_GATT_APP_HID_CONTROL_REPORT_CHARAC_UUID     0x2c02
#define CSR_BT_GATT_APP_HID_INTERUPT_REPORT_CHARAC_UUID    0x2c03

/* Battery status values */
#define CSR_BT_GATT_BATT_STATUS_MASK                       0x03

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_GATT_APP_DEFINES_H__ */


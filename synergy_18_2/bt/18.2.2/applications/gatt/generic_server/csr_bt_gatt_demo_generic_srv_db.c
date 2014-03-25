/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 

   REVISION:      $Revision: #3 $
 ****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_gatt_demo_generic_srv_db.h"
#include "csr_bt_gatt_demo_defines.h"
#include "csr_bt_profiles.h"
#include "csr_bt_gatt_utils.h"
#include "csr_bt_gatt_demo_generic_srv_prim.h"
#include "csr_pmem.h"

CsrBtGattDb * CsrBtGenericSrvDbCreate(CsrUint16 handleOffset)
{
    /********* Service 1 *********
     *
     * Structure:
     * Handle   |   UUID    |   Value                                                            | Permission
     * ------------------------------------------------------------------------------------------------------
     * 0x0001   |   0x2800  |   Primary Service, UUID=0x1800 (Generic Attribute Profile)         | Read
     * 0x0002   |   0x2803  |   Charac Descriptor, R, Handle:0x0003, UUID:0xxxxx                 | Read
     * 0x0003   |   0xXXXX  |   xxxxx                                                            | Read
     *
     *  and more

     Already added by GATT*/

    /********* Service 2 *********
     *
     * Structure:
     * Handle   |   UUID    |   Value                                                            | Permission
     * ------------------------------------------------------------------------------------------------------
     * 0x0008   |   0x2800  |   Primary Service, UUID=0x1801 (Attribute Profile)                 | Read
     * 0x0009   |   0x2803  |   Charac Descriptor, R, Handle:0x000a, UUID:0xxxxx                 | Read
     *
     * More

     Already added by GATT*/

    /********* Service 3 *********
     *
     * Structure:
     * Handle   |   UUID    |   Value                                                            | Permission
     * ------------------------------------------------------------------------------------------------------
     * 0x0010   |   0x2800  |   Primary Service, UUID=0x1808 (Temperature Service)               | Read
     * 0x0011   |   0x2803  |   Charac Descriptor, R, Handle:0x0012, UUID:0xB1E0                 | Read
     * 0x0012   |   0xB1E0  |   0x00                                                             | Read
     * 0x0013   |   0x2803  |   Charac Descriptor, R, Handle:0x001a, UUID:0xB1E4                 | Read
     * 0x0014   |   0xB1E4  |   0x0B                                                             | Read
     * 0x0013   |   0x2803  |   Charac Descriptor, R, Handle:0x001a, UUID:0xB1E4                 | Read
     * 0x0014   |   0xB1E4  |   0x0B                                                             | Read
     * 0x0013   |   0x2803  |   Charac Descriptor, R, Handle:0x001a, UUID:0xB1E4                 | Read
     * 0x0014   |   0xB1E4  |   0x0B                                                             | Read
     * 0x0013   |   0x2803  |   Charac Descriptor, R, Handle:0x001a, UUID:0xB1E4                 | Read
     * 0x0014   |   0xB1E4  |   0x0B                                                             | Read
     * 0x0013   |   0x2803  |   Charac Descriptor, R, Handle:0x001a, UUID:0xB1E4                 | Read
     * 0x0014   |   0xB1E4  |   0x0B                                                             | Read
     * 0x0013   |   0x2803  |   Charac Descriptor, R, Handle:0x001a, UUID:0xB1E4                 | Read
     * 0x0014   |   0xB1E4  |   0x0B                                                             | Read
     */

    CsrBtGattDb    *head           = NULL;
    CsrBtGattDb    *tail           = NULL;
    CsrBtGattHandle attrHandle; 
    CsrBtGattPropertiesBits    properties;        
    CsrBtGattAttrFlags         attrValueFlags;
    CsrBtUuid16                uuid16;
    CsrUint8        *value;
    CsrUint8        *value1;
    CsrUint8        *value2;
    CsrUint8        *value3;
    CsrUint8        *value4;
    CsrUint8        *value5;
    CsrUint8        *value6;
    CsrUint8        *value7;
    CsrUint8        *value8;

    CsrUtf8String             *userDescription;

    properties           =  CSR_BT_GATT_CHARAC_PROPERTIES_READ;
    attrValueFlags       =  CSR_BT_GATT_ATTR_FLAGS_NONE;
    uuid16               =  CSR_BT_GATT_APP_MANUFACTURER_CHARAC_UUID;
    value                =  CsrPmemZalloc(CsrStrLen("CSR") + 1);
    value[0]             =  'C'; 
    value[1]             =  'S'; 
    value[2]             =  'R';
    attrHandle           =  CSR_BT_GENERIC_SRV_HEALTH_INFO_HANDLE + handleOffset;
    
    head = CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(head,
                                                          &attrHandle,
                                                          CSR_BT_GATT_APP_DEVICE_INFO_UUID,
                                                          FALSE,
                                                          &tail);
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            (CsrBtGattPropertiesBits)(properties | CSR_BT_GATT_CHARAC_PROPERTIES_BROADCAST), 
                                                            uuid16,
                                                            attrValueFlags,
                                                            4, /*value size*/
                                                            value,
                                                            &tail);
    head = CsrBtGattUtilCreateServerCharacConfiguration(head,
                                                        &attrHandle,
                                                        attrValueFlags,
                                                        CSR_BT_GATT_SERVER_CHARAC_CONFIG_DISABLE,
                                                        &tail);
    CsrPmemFree(value);
    value1                =  CsrPmemZalloc(CsrStrLen("1.0") +1);
    value1[0]             =  '1'; 
    value1[1]             =  '.'; 
    value1[2]             =  '4';
    uuid16               =  CSR_BT_GATT_APP_MODEL_NUMBER_CHARAC_UUID;
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            (CsrBtGattPropertiesBits)(properties | CSR_BT_GATT_CHARAC_PROPERTIES_WRITE), 
                                                            uuid16,
                                                            attrValueFlags,
                                                            4, /*value size*/
                                                            value1,
                                                            &tail);  

    CsrPmemFree(value1);
    value2                =  CsrPmemZalloc(CsrStrLen("1.0") + 1);
    value2[0]             =  '1'; 
    value2[1]             =  '.'; 
    value2[2]             =  '0';
    uuid16               =  CSR_BT_GATT_APP_SERIAL_NUMBER_CHARAC_UUID;
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            (CsrBtGattPropertiesBits)(properties | CSR_BT_GATT_CHARAC_PROPERTIES_AUTH_SIGNED_WRITES), 
                                                            uuid16,
                                                            attrValueFlags,
                                                            4, /*value size*/
                                                            value2,
                                                            &tail);  
    head = CsrBtGattUtilCreateClientCharacConfiguration(head,
                                                        &attrHandle,
                                                        CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                        &tail);
    CsrPmemFree(value2);

    value3                =  CsrPmemZalloc(CsrStrLen("1.0") + 1);
    value3[0]             =  '1'; 
    value3[1]             =  '.'; 
    value3[2]             =  '2';
    uuid16               =  CSR_BT_GATT_APP_HARDWARE_REVISION_CHARAC_UUID;
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            (CsrBtGattPropertiesBits)(properties | CSR_BT_GATT_CHARAC_PROPERTIES_EXTENDED_PROPERTIES), 
                                                            uuid16,
                                                            attrValueFlags,
                                                            4, /*value size*/
                                                            value3,
                                                            &tail);  

    head = CsrBtGattUtilCreateCharacExtProperties(head,
                                                  &attrHandle,
                                                  CSR_BT_GATT_CHARAC_EXT_PROPERTIES_RELIABLE_WRITE,
                                                 &tail);
    CsrPmemFree(value3);

    value4                =  CsrPmemZalloc(CsrStrLen("1.0") + 1);
    value4[0]             =  '1'; 
    value4[1]             =  '.'; 
    value4[2]             =  '2';
    uuid16               =  CSR_BT_GATT_APP_FIRMWARE_REVISION_CHARAC_UUID;
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            (CsrBtGattPropertiesBits)(properties | CSR_BT_GATT_CHARAC_PROPERTIES_EXTENDED_PROPERTIES), 
                                                            uuid16,
                                                            attrValueFlags,
                                                            4, /*value size*/
                                                            value4,
                                                            &tail);  
    CsrPmemFree(value4);


    userDescription = (CsrUint8*)CsrStrDup("Dummy");
    head = CsrBtGattUtilCreateCharacUserDescription(head,
                                                    &attrHandle,
                                                    userDescription,
                                                    CSR_BT_GATT_PERM_FLAGS_READ | CSR_BT_GATT_PERM_FLAGS_WRITE,
                                                    CSR_BT_GATT_ATTR_FLAGS_DYNLEN, 
                                                    &tail);

    head = CsrBtGattUtilCreateCharacExtProperties(head,
                                                &attrHandle,
                                                CSR_BT_GATT_CHARAC_EXT_PROPERTIES_WRITE_AUX,
                                                &tail);



    value5                =  CsrPmemZalloc(CsrStrLen("1.0") + 1);
    value5[0]             =  '1'; 
    value5[1]             =  '.'; 
    value5[2]             =  '2';
    uuid16               =  CSR_BT_GATT_APP_SOFTWARE_REVISION_CHARAC_UUID;
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            (CsrBtGattPropertiesBits)(properties | CSR_BT_GATT_CHARAC_PROPERTIES_EXTENDED_PROPERTIES), 
                                                            uuid16,
                                                            attrValueFlags,
                                                            4, /*value size*/
                                                            value5,
                                                            &tail);  
    CsrPmemFree(value5);
    head = CsrBtGattUtilCreateCharacExtProperties(head,
                                                  &attrHandle,
                                                  CSR_BT_GATT_CHARAC_EXT_PROPERTIES_WRITE_AUX | CSR_BT_GATT_CHARAC_EXT_PROPERTIES_RELIABLE_WRITE,
                                                  &tail);
    /********* Service 4 *********
     *
     * Structure:
     * Handle   |   UUID    |   Value                                                            | Permission
     * ------------------------------------------------------------------------------------------------------
     * 0x0020   |   0x2800  |   Primary Service, UUID=0x1808 (Temperature Service)               | Read
     * 0x0021   |   0x2803  |   Charac Descriptor, R/W, Handle:0x0012, UUID:0xB1E0               | Read
     * 0x0022   |   0xB1E0  |   0x00                                                             | indicate
     * 0x0023   |   0x2803  |   Charac Descriptor, R, Handle:0x001a, UUID:0xB1E4                 | Read
     * 0x0024   |   0xB1E4  |   0x0B                                                             | write
     */
    properties           =  CSR_BT_GATT_CHARAC_PROPERTIES_READ | CSR_BT_GATT_CHARAC_PROPERTIES_INDICATE;
    attrValueFlags       =  CSR_BT_GATT_ATTR_FLAGS_IRQ_WRITE;
    uuid16               =  CSR_BT_GATT_APP_TEMP_MEASUREMENT_CHARAC_UUID;
    value                =  CsrPmemAlloc(5);
    value[0]             =  0x00; /*flags*/
    value[1]             =  0x00; /*value*/
    value[2]             =  0x00; /*value*/
    value[3]             =  0x00; /*value*/
    value[4]             =  0x00; /*value*/
    attrHandle           =  CSR_BT_GENERIC_SRV_THERMOMETER_HANDLE + handleOffset;

    head = CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(head,
                                                          &attrHandle,
                                                          CSR_BT_GATT_APP_THERMOMETER_UUID,
                                                          FALSE,
                                                          &tail);
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            properties, 
                                                            uuid16,
                                                            attrValueFlags,
                                                            5, /*value size*/
                                                            value,
                                                            &tail);  

    head = CsrBtGattUtilCreateClientCharacConfiguration(head,
                                                        &attrHandle,
                                                        CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                        &tail);

    /*Freeing the pointer*/
    CsrPmemFree(value);
    /*----*/

    /********* Service 5 *********
     *
     * Structure:
     * Handle   |   UUID    |   Value                                                            | Permission
     * ------------------------------------------------------------------------------------------------------
     * 0x0019   |   0x2800  |   Primary Service, UUID=0x180F (Battery Service)                   | Read
     * 0x001a   |   0x2803  |   Charac Descriptor, R, Handle:0x0019, UUID:0xB1E3                 | Read
     * 0x001b   |   0xB1E3  |   0x0000                                                           | Read
     * 0x001a   |   0x2803  |   Charac Descriptor, R, Handle:0x001a, UUID:0xB1E4                 | Read
     * 0x001c   |   0xB1E4  |   0x0B                                                             | Read
     * 0x001d   |   0x2803  |   Charac Descriptor, R, Handle:0x001c, UUID:0xB1E5                 | Read
     * 0x001e   |   0xB1E5  |   0x01                                                             | Read
     */

    properties          = CSR_BT_GATT_CHARAC_PROPERTIES_READ;
    attrValueFlags      = CSR_BT_GATT_ATTR_FLAGS_IRQ_WRITE;
    uuid16     = CSR_BT_GATT_APP_BATT_MASK_CHARAC_UUID;
    value6      = CsrPmemAlloc(1);
    *value6     = (CsrUint8) 0xAB; /* Battery present, Not discharging, not charging, Good level */

    attrHandle = CSR_BT_GENERIC_SRV_BATTERY_HANDLE + handleOffset;

    head = CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(head,
                                                          &attrHandle,
                                                          CSR_BT_GATT_APP_BATTERY_UUID,
                                                          FALSE,
                                                          &tail);
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            (CsrBtGattPropertiesBits)(properties | CSR_BT_GATT_CHARAC_PROPERTIES_NOTIFY), 
                                                            uuid16,
                                                            attrValueFlags,
                                                            1, /*value size */
                                                            value6,
                                                            &tail);  
    head = CsrBtGattUtilCreateClientCharacConfiguration(head,
                                                        &attrHandle,
                                                        CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                        &tail);
    /*Freeing the pointer*/
    CsrPmemFree(value6);

    value7      = CsrPmemAlloc(1);
    *value7     = (CsrUint8) 0x64;
    uuid16     = CSR_BT_GATT_APP_BATT_LEVEL_CHARAC_UUID;

    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            (CsrBtGattPropertiesBits)(properties | CSR_BT_GATT_CHARAC_PROPERTIES_WRITE_WITHOUT_RESPONSE), 
                                                            uuid16,
                                                            CSR_BT_GATT_ATTR_FLAGS_IRQ_WRITE,
                                                            1, /*value size*/
                                                            value7,
                                                            &tail);
    /*Freeing the pointer*/
    CsrPmemFree(value7);

    value8      = CsrPmemAlloc(1);
    *value8     = (CsrUint8) 0x01; /* No service required */
    uuid16      = CSR_BT_GATT_APP_BATT_SERVICE_REQUIRED_CHARAC_UUID;

    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            (CsrBtGattPropertiesBits)(properties | CSR_BT_GATT_CHARAC_PROPERTIES_WRITE_WITHOUT_RESPONSE), 
                                                            uuid16,
                                                            CSR_BT_GATT_ATTR_FLAGS_IRQ_WRITE,
                                                            1, /*value size*/
                                                            value8,
                                                            &tail);
    /*Freeing the pointer*/
    CsrPmemFree(value8);
    /* Adding primary service without characteristics, 
       to validate db readings continue thougth empty service found */
    head = CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(head,
                                                          &attrHandle,
                                                          0xc001, /* none existing UUID */
                                                          FALSE,
                                                          &tail);

    return head;
}


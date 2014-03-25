/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 

   REVISION:      $Revision: #5 $
 ****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_therm_srv_db.h"
#include "csr_bt_gatt_services_defines.h"
#include "csr_bt_profiles.h"
#include "csr_bt_gatt_utils.h"
#include "csr_bt_therm_srv_prim.h"
#include "csr_pmem.h"

CsrBtGattDb * CsrBtThermSrvDbCreate(CsrUint16 handleOffset)
{

    CsrBtGattDb    *head           = NULL;
    CsrBtGattDb    *tail           = NULL;
    CsrBtGattHandle attrHandle; 
    CsrBtGattPropertiesBits    properties;        
    CsrBtGattAttrFlags         attrValueFlags;
    CsrBtUuid16                uuid16;
    CsrUint8        *value;
    
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
    properties           =  CSR_BT_GATT_CHARAC_PROPERTIES_READ;
    attrValueFlags       =  CSR_BT_GATT_ATTR_FLAGS_NONE;
    uuid16               =  CSR_BT_GATT_SERVICE_HEALTH_INFO_MANUFACT_NAME_CHARAC_UUID;
    attrHandle           =  CSR_BT_THERM_SRV_HEALTH_INFO_HANDLE + handleOffset;

    head = CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(head,
                                                          &attrHandle,
                                                          CSR_BT_GATT_SERVICE_HEALTH_DEV_INFO_UUID,
                                                          TRUE,
                                                          &tail);
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            properties, 
                                                            uuid16,
                                                            attrValueFlags,
                                                            (CsrUint16)CsrStrLen("CSR"), /*value size*/
                                                            (const CsrUint8*)"CSR",
                                                            &tail);  

    uuid16               =  CSR_BT_GATT_SERVICE_HEALTH_INFO_MODEL_NUM_CHARAC_UUID;
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            properties, 
                                                            uuid16,
                                                            attrValueFlags,
                                                            (CsrUint16)CsrStrLen("1.0"), /*value size*/
                                                            (const CsrUint8*)"1.0",
                                                            &tail);  

    uuid16               =  CSR_BT_GATT_SERVICE_HEALTH_INFO_SERIAL_NUM_CHARAC_UUID;
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            properties, 
                                                            uuid16,
                                                            attrValueFlags,
                                                            (CsrUint16)CsrStrLen("1.0"), /*value size*/
                                                            (const CsrUint8*)"1.0",
                                                            &tail);  

    uuid16               =  CSR_BT_GATT_SERVICE_HEALTH_INFO_HW_REVISION_CHARAC_UUID;
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            properties, 
                                                            uuid16,
                                                            attrValueFlags,
                                                            (CsrUint16)CsrStrLen("1.0"), /*value size*/
                                                            (const CsrUint8*)"1.0",
                                                            &tail);  

    uuid16               =  CSR_BT_GATT_SERVICE_HEALTH_INFO_FW_REVISION_CHARAC_UUID;
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            properties, 
                                                            uuid16,
                                                            attrValueFlags,
                                                            (CsrUint16)CsrStrLen("1.0"), /*value size*/
                                                            (const CsrUint8*)"1.0",
                                                            &tail);  

    uuid16               =  CSR_BT_GATT_SERVICE_HEALTH_INFO_SW_REVISION_CHARAC_UUID;
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            properties, 
                                                            uuid16,
                                                            attrValueFlags,
                                                            (CsrUint16)CsrStrLen("1.0"), /*value size*/
                                                            (const CsrUint8*)"1.0",
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
    properties           =  CSR_BT_GATT_CHARAC_PROPERTIES_INDICATE;
    attrValueFlags       =  CSR_BT_GATT_ATTR_FLAGS_NONE;
    uuid16               =  CSR_BT_GATT_SERVICE_TEMP_MEASUREMENT_CHARAC_UUID;
    value                =  CsrPmemAlloc(5);
    value[0]             =  0x01; /*flags*/
    value[1]             =  0x00; /*value*/
    value[2]             =  0x00; /*value*/
    value[3]             =  0x00; /*value*/
    value[4]             =  0x00; /*value*/
    attrHandle           =  CSR_BT_THERM_SRV_THERMOMETER_HANDLE + handleOffset;

    head = CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(head,
                                                          &attrHandle,
                                                          CSR_BT_GATT_SERVICE_THERMOMETER_UUID,
                                                          TRUE,
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
     * 0x0019   |   0x2800  |   Primary Service, UUID=0x1805 (Battery Service)                   | Read
     * 0x001a   |   0x2803  |   Charac Descriptor, R, Handle:0x0019, UUID:0xB1E3                 | Read
     * 0x001b   |   0xB1E3  |   0x0000                                                           | Read
     * 0x001a   |   0x2803  |   Charac Descriptor, R, Handle:0x001a, UUID:0xB1E4                 | Read
     * 0x001c   |   0xB1E4  |   0x0B                                                             | Read
     * 0x001d   |   0x2803  |   Charac Descriptor, R, Handle:0x001a, UUID:0xB1E5                 | Read
     * 0x001e   |   0xB1E5  |   0x01                                                             | Read
     */

    properties          = CSR_BT_GATT_CHARAC_PROPERTIES_READ;
    attrValueFlags      = CSR_BT_GATT_ATTR_FLAGS_IRQ_READ;
    uuid16     = CSR_BT_GATT_SERVICE_BATT_MASK_CHARAC_UUID;
    value      = CsrPmemAlloc(1);
    *value     = (CsrUint8) 0x0B;
    attrHandle = CSR_BT_THERM_SRV_BATTERY_HANDLE + handleOffset;

    head = CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(head,
                                                          &attrHandle,
                                                          CSR_BT_GATT_SERVICE_BATTERY_UUID,
                                                          TRUE,
                                                          &tail);
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            properties, 
                                                            uuid16,
                                                            attrValueFlags,
                                                            1, /*value size */
                                                            value,
                                                            &tail);  
    /*Freeing the pointer*/
    CsrPmemFree(value);
    value      = CsrPmemAlloc(1);
    *value     = (CsrUint8) 0x64;
    uuid16     = CSR_BT_GATT_SERVICE_BATT_LEVEL_CHARAC_UUID;

    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            properties, 
                                                            uuid16,
                                                            attrValueFlags,
                                                            1, /*value size*/
                                                            value,
                                                            &tail);  
    CsrPmemFree(value);
    value      = CsrPmemAlloc(1);
    *value     = (CsrUint8) 0x1;
    uuid16     = CSR_BT_GATT_SERVICE_BATT_SERVICE_REQUIRED_CHARAC_UUID;

    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            properties, 
                                                            uuid16,
                                                            attrValueFlags,
                                                            1, /*value size*/
                                                            value,
                                                            &tail);
    CsrPmemFree(value);
    return head;
}


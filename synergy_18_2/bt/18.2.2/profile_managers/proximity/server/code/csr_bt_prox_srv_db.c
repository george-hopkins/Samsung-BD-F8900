/***************************************************************************
    Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
        
          All rights reserved.

REVISION:      $Revision#48 $
 ****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_prox_srv_db.h"
#include "csr_bt_prox_srv_prim.h"
#include "csr_bt_gatt_services_defines.h"
#include "csr_bt_profiles.h"
#include "csr_bt_gatt_utils.h"
#include "csr_pmem.h"

CsrBtGattDb * CsrBtProxSrvDbCreate(CsrUint16 handleOffset)
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
     * 0x0010   |   0x2800  |   Primary Service, UUID=0x1802 (Link Loss Service)                 | Read
     * 0x0011   |   0x2803  |   Charac Descriptor, R/W, Handle:0x0012, UUID:0xB1E0               | Read
     * 0x0012   |   0xB1E0  |   0x00                                                             | Read/Write
     */

    properties          = (CSR_BT_GATT_CHARAC_PROPERTIES_READ | CSR_BT_GATT_CHARAC_PROPERTIES_WRITE );
    attrValueFlags      = CSR_BT_GATT_ATTR_FLAGS_IRQ_WRITE;
    uuid16     = CSR_BT_GATT_SERVICE_LINKLOSS_CHARAC_UUID;
    value      = CsrPmemZalloc(2);
    attrHandle = CSR_BT_PROX_SRV_LINKLOSS_HANDLE + handleOffset;

    head = CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(head,
                                                          &attrHandle,
                                                          CSR_BT_GATT_SERVICE_LINKLOSS_UUID,
                                                          FALSE,
                                                          &tail);
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            properties, 
                                                            uuid16,
                                                            attrValueFlags,
                                                            2, /*value size*/
                                                            value,
                                                            &tail);  

    /*Freeing the pointer*/
    CsrPmemFree(value);
    /*----*/

    /********* Service 4 *********
     *
     * Structure:
     * Handle   |   UUID    |   Value                                                            | Permission
     * ------------------------------------------------------------------------------------------------------
     * 0x0013   |   0x2800  |   Primary Service, UUID=0x1803 (Immediate Alert Service)           | Read
     * 0x0014   |   0x2803  |   Charac Descriptor, R/W, Handle:0x0015, UUID:0x2A06               | Read
     * 0x0015   |   0x2A06  |   0x00                                                             | Write cmd
     */

    properties          = (CSR_BT_GATT_CHARAC_PROPERTIES_WRITE_WITHOUT_RESPONSE);
    attrValueFlags      = CSR_BT_GATT_ATTR_FLAGS_IRQ_WRITE;
    uuid16     = CSR_BT_GATT_SERVICE_IALERT_CHARAC_UUID;
    value      = CsrPmemZalloc(2);
    attrHandle = CSR_BT_PROX_SRV_IALERT_HANDLE + handleOffset;

    head = CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(head,
                                                          &attrHandle,
                                                          CSR_BT_GATT_SERVICE_IALERT_UUID,
                                                          FALSE,
                                                          &tail);
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            properties, 
                                                            uuid16,
                                                            attrValueFlags,
                                                            2, /*value size*/
                                                            value,
                                                            &tail);  

    /*Freeing the pointer*/
    CsrPmemFree(value);
    /*----*/

    /********* Service 5 *********
     *
     * Structure:
     * Handle   |   UUID    |   Value                                                            | Permission
     * ------------------------------------------------------------------------------------------------------
     * 0x0016   |   0x2800  |   Primary Service, UUID=0x1804 (TX Power Service)                  | Read
     * 0x0017   |   0x2803  |   Charac Descriptor, R/W, Handle:0x0018, UUID:0x2A07               | Read
     * 0x0018   |   0x2A07  |   0x12                                                             | Read/Write
     */

    properties          = CSR_BT_GATT_CHARAC_PROPERTIES_READ | CSR_BT_GATT_CHARAC_PROPERTIES_NOTIFY;
    attrValueFlags      = CSR_BT_GATT_ATTR_FLAGS_IRQ_READ;
    uuid16     = CSR_BT_GATT_SERVICE_TX_POWER_CHARAC_UUID;
    value      = CsrPmemAlloc(1);
    *value     = (CsrUint8) 0x00;
    attrHandle = CSR_BT_PROX_SRV_TX_POWER_HANDLE + handleOffset;

    head = CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(head,
                                                          &attrHandle,
                                                          CSR_BT_GATT_SERVICE_TX_POWER_UUID,
                                                          FALSE,
                                                          &tail);
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle, 
                                                            properties, 
                                                            uuid16,
                                                            attrValueFlags,
                                                            1, /*value size*/
                                                            value,
                                                            &tail);  
    head = CsrBtGattUtilCreateClientCharacConfiguration(head,
                                                        &attrHandle,
                                                        CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                        &tail);

    /*Freeing the pointer*/
    CsrPmemFree(value);
    /*----*/


    /********* Service 6 *********
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
    *value     = (CsrUint8) 0x00;
    attrHandle = CSR_BT_PROX_SRV_BATTERY_HANDLE + handleOffset;

    head = CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(head,
                                                          &attrHandle,
                                                          CSR_BT_GATT_SERVICE_BATTERY_UUID,
                                                          FALSE,
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
    /*Freeing the pointer*/
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
    /*----*/


    return head;
}


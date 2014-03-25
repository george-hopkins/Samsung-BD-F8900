#ifndef CSR_BT_GATT_PRIVATE_PRIM_H__
#define CSR_BT_GATT_PRIVATE_PRIM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

****************************************************************************/

#include "csr_bt_gatt_prim.h"
#include "csr_bt_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* search_string="CsrBtGattPrim" */
/* conversion_rule="UPPERCASE_START_AND_REMOVE_UNDERSCORES" */
#define CSR_BT_GATT_PRIM_DOWNSTREAM_PRIVATE_LOWEST  (0x0100)

/*  Not part of interface, used internally by GATT only */
#define CSR_BT_GATT_EXCHANGE_MTU_REQ        ((CsrBtGattPrim)(0x0000 + CSR_BT_GATT_PRIM_DOWNSTREAM_PRIVATE_LOWEST))
#define CSR_BT_GATT_PRIVATE_UNREGISTER_REQ  ((CsrBtGattPrim)(0x0001 + CSR_BT_GATT_PRIM_DOWNSTREAM_PRIVATE_LOWEST))
#define CSR_BT_GATT_SDS_REQ                 ((CsrBtGattPrim)(0x0002 + CSR_BT_GATT_PRIM_DOWNSTREAM_PRIVATE_LOWEST))


#define CSR_BT_GATT_PRIVATE_PRIM (CSR_BT_GATT_PRIM)

/*******************************************************************************
 * Structures for internal use GATT only
 *******************************************************************************/
typedef struct
{
    CsrBtGattPrim       type;                   /* Identity */
    CsrBtConnId         btConnId;               /* Connection identifier */
    CsrUint16           mtu;                    /* Client receive MTU size */
    CsrBtTypedAddr      address;                /* Peer Address */
} CsrBtGattExchangeMtuReq;

typedef struct
{
    CsrBtGattPrim       type;                   /* Identity */
    CsrBtGattId         gattId;                 /* The application handle */
    CsrBtConnId         btConnId;               /* Connection identifier */
} CsrBtGattPrivateUnregisterReq;

typedef struct
{
    CsrBtGattPrim       type;                   /* Identity */
} CsrBtGattSdsReq;

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_GATT_PRIVATE_PRIM_H__ */

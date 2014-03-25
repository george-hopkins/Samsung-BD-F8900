#ifndef CSR_BT_HIDC_APP_PRIVATE_PRIM_H__
#define CSR_BT_HIDC_APP_PRIVATE_PRIM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_profiles.h"
#include "csr_bt_app_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef CsrPrim CsrBtHidcAppPrim;

#define CSR_BT_HIDC_APP_REGISTER_REQ               0
#define CSR_BT_HIDC_APP_REGISTER_CFM               1
#define CSR_BT_HIDC_APP_HOUSE_CLEANING             2

typedef struct
{
    CsrBtHidcAppPrim type;
    CsrSchedQid phandle;
} CsrBtHidcAppRegisterReq;

typedef struct
{
    CsrBtHidcAppPrim type;
    CsrSchedQid queueId;
    CsrUint8 instance;
} CsrBtHidcAppRegisterCfm;

typedef struct
{
    CsrSchedQid type;
} CsrBtHidcAppHouseCleaning;


#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_HIDC_APP_PRIVATE_PRIM_H__ */

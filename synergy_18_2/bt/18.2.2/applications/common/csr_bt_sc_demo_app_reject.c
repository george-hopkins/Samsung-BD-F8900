/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

/* Standard include files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* Profile manager include files */
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sc_demo_app.h"

static void scAppRejectPasskeyIndSend(ScAppInstData_t *instData)
{
    CsrBtScPasskeyNegResSend(instData->deviceAddr);
}

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
static void scAppRejectSspPasskeyIndSend(ScAppInstData_t *instData)
{
    CsrBtScSspPasskeyNegResSend(instData->deviceAddr);
}

static void scAppRejectSspNotificationIndSend(ScAppInstData_t *instData)
{
    CsrBtScSspNotificationNegResSend(instData->deviceAddr);
}

static void scAppRejectSspCompareIndSend(ScAppInstData_t *instData)
{
    CsrBtScSspCompareNegResSend(instData->deviceAddr);
}

static void scAppRejectSspJustWorksIndSend(ScAppInstData_t *instData)
{
    CsrBtScSspJustWorksNegResSend(instData->deviceAddr);
}

static void scAppRejectRebondIndSend(ScAppInstData_t *instData)
{
    CsrBtScRebondNegResSend(instData->deviceAddr);
}

static void scAppRejectSspPairingIndSend(ScAppInstData_t *instData)
{
    CsrBtScSspPairingNegResSend(instData->deviceAddr);
}

#endif

const scStateHandlerType rejectPairingFunc[CSR_BT_SC_PRIM_UPSTREAM_COUNT] =
{
    scAppRejectPasskeyIndSend,              /* CSR_BT_SC_PASSKEY_IND                           */
    NULL,                                   /* CSR_BT_SC_BOND_CFM                              */
    NULL,                                   /* CSR_BT_SC_DEBOND_CFM                            */
    NULL,                                   /* CSR_BT_SC_SET_SECURITY_MODE_CFM                 */
    NULL,                                   /* CSR_BT_SC_ENCRYPTION_CFM                        */
    NULL,                                   /* CSR_BT_SC_AUTHORISE_IND                         */
    NULL,                                   /* CSR_BT_SC_BOND_IND                              */
    NULL,                                   /* CSR_BT_SC_READ_DEVICE_RECORD_IND                */
    NULL,                                   /* CSR_BT_SC_READ_DEVICE_RECORD_CFM                */
    NULL,                                   /* CSR_BT_SC_UPDATE_TRUST_LEVEL_IND                */
    NULL,                                   /* CSR_BT_SC_AUTHENTICATE_CFM                      */
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    scAppRejectSspPasskeyIndSend,           /* CSR_BT_SC_SSP_PASSKEY_IND                       */
    scAppRejectSspNotificationIndSend,      /* CSR_BT_SC_SSP_NOTIFICATION_IND                  */
    scAppRejectSspCompareIndSend,           /* CSR_BT_SC_SSP_COMPARE_IND                       */
    scAppRejectSspJustWorksIndSend,         /* CSR_BT_SC_SSP_JUSTWORKS_IND                     */
    NULL,                                   /* CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM               */
    scAppRejectSspPairingIndSend,           /* CSR_BT_SC_SSP_PAIRING_IND                       */
    NULL,                                   /* CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND         */
    NULL,                                   /* CSR_BT_SC_MODE_IND                              */
    scAppRejectRebondIndSend,               /* CSR_BT_SC_REBOND_IND                            */
#else
    NULL,                                   /* CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM               */
    NULL,                                   /* CSR_BT_SC_SSP_PAIRING_IND                       */
    NULL,                                   /* CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND         */
    NULL,                                   /* CSR_BT_SC_MODE_IND                              */
    NULL,                                   /* CSR_BT_SC_REBOND_IND                            */
#endif
};


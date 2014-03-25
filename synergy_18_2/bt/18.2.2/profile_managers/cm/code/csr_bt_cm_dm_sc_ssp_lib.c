/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"
#include "dm_prim.h"
#include "csr_types.h"
#include "bluetooth.h"

#ifdef CSR_BT_INSTALL_CM_SC_MODE_CONFIG
void CsrBtCmScDmSecModeConfigReq(CsrUint16 writeAuthEnable, CsrUint16 config)
{
    CsrBtCmSmSecModeConfigReq    *prim;

    prim                  = pnew(CsrBtCmSmSecModeConfigReq);
    prim->type            = CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ;
    prim->writeAuthEnable = writeAuthEnable;
    prim->config          = config;
    CsrBtCmPutMessageDownstream(prim);
}
#endif

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1

void CsrBtCmScDmIoCapabilityRequestRes(CsrBtDeviceAddr deviceAddr,
                                       CsrUint8 ioCapability,
                                       CsrUint8 authenticationRequirements,
                                       CsrUint8 oobDataPresent,
                                       CsrUint8 *oobHashC,
                                       CsrUint8 *oobRandR,
                                       CsrBtAddressType addressType,
                                       CsrUint16 keyDistribution)
{
    CsrBtCmSmIoCapabilityRequestRes *prim = pnew(CsrBtCmSmIoCapabilityRequestRes);
    prim->type           = CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES;
    prim->deviceAddr     = deviceAddr;
    prim->ioCapability   = ioCapability;
    prim->authenticationRequirements = authenticationRequirements;
    prim->oobDataPresent = oobDataPresent;
    CsrMemCpy(prim->oobHashC, oobHashC, SIZE_OOB_DATA);
    CsrMemCpy(prim->oobRandR, oobRandR, SIZE_OOB_DATA);
    prim->addressType = addressType;
    prim->keyDistribution = keyDistribution;
    CsrBtCmPutMessageDownstream(prim);
}

void CsrBtCmScDmIoCapabilityRequestNegRes(CsrBtDeviceAddr deviceAddr,
                                          hci_error_t reason,
                                          CsrBtAddressType addressType)
{
    CsrBtCmSmIoCapabilityRequestNegRes    *prim;

    prim             = pnew(CsrBtCmSmIoCapabilityRequestNegRes);
    prim->type       = CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES;
    prim->deviceAddr = deviceAddr;
    prim->reason     = reason;
    prim->addressType = addressType;

    CsrBtCmPutMessageDownstream(prim);
}

void CsrBtCmScDmUserConfirmationRequestNegRes(CsrBtDeviceAddr deviceAddr,
                                              CsrBtAddressType addressType)
{
    CsrBtCmSmUserConfirmationRequestNegRes    *prim;

    prim             = pnew(CsrBtCmSmUserConfirmationRequestNegRes);
    prim->type       = CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES;
    prim->deviceAddr = deviceAddr;
    prim->reason     = HCI_ERROR_AUTH_FAIL; /* user rejected */
    prim->addressType = addressType;
    CsrBtCmPutMessageDownstream(prim);
}

void CsrBtCmScDmUserPasskeyRequestNegRes(CsrBtDeviceAddr deviceAddr,
                                         CsrBtAddressType addressType)
{
    CsrBtCmSmUserPasskeyRequestNegRes    *prim;

    prim             = pnew(CsrBtCmSmUserPasskeyRequestNegRes);
    prim->type       = CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES;
    prim->deviceAddr = deviceAddr;
    prim->numericValue = 0;
    prim->addressType = addressType;
    CsrBtCmPutMessageDownstream(prim);
}

void CsrBtCmSmSendRepairRes(CsrBtDeviceAddr deviceAddr,
                            CsrBool accept,
                            CsrUint16 repairId,
                            CsrBtAddressType addressType)
{
    CsrBtCmSmRepairRes    *prim;

    prim                   = pnew(CsrBtCmSmRepairRes);
    prim->type             = CSR_BT_CM_SM_REPAIR_RES;
    prim->deviceAddr       = deviceAddr;
    prim->accept           = accept;
    prim->repairId         = repairId;
    prim->addressType = addressType;
    CsrBtCmPutMessageDownstream(prim);
}

#endif

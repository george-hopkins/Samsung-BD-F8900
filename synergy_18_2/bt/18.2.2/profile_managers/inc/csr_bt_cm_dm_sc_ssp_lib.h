#ifndef CSR_BT_CM_DM_SC_SSP_LIB_H__
#define CSR_BT_CM_DM_SC_SSP_LIB_H__

#include "csr_synergy.h"

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "bluetooth.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CSR_BT_INSTALL_CM_SC_MODE_CONFIG
void CsrBtCmScDmSecModeConfigReq(CsrUint16 writeAuthEnable, CsrUint16 config);
#endif
void CsrBtCmScDmSspLinkKeyRequestRes(CsrBtDeviceAddr deviceAddr, CsrUint8 keyType, CsrUint8 *key,
                                     CsrBtAddressType addressType);
void CsrBtCmScDmSspLinkKeyRequestNegRes(CsrBtDeviceAddr deviceAddr,
                                        CsrBtAddressType addressTypeM);

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
void CsrBtCmScDmIoCapabilityRequestRes(CsrBtDeviceAddr deviceAddr,
                                       CsrUint8 ioCapability,
                                       CsrUint8 authenticationRequirements,
                                       CsrUint8 oobDataPresent,
                                       CsrUint8 *oobHashC,
                                       CsrUint8 *oobRandR,
                                       CsrBtAddressType addressType,
                                       CsrUint16 keyDistribution);
void CsrBtCmScDmIoCapabilityRequestNegRes(CsrBtDeviceAddr deviceAddr,
                                          hci_error_t reason,
                                          CsrBtAddressType addressType);

#define CsrBtCmScDmUserConfirmationRequestRes(_deviceAddr, _addressType){ \
        CsrBtCmSmUserConfirmationRequestRes *prim__ = pnew(CsrBtCmSmUserConfirmationRequestRes); \
        prim__->type       = CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES; \
        prim__->deviceAddr = _deviceAddr;                               \
        prim__->reason     = HCI_SUCCESS; /* user accepted */           \
        prim__->addressType = _addressType;                             \
        CsrBtCmPutMessageDownstream(prim__);}

void CsrBtCmScDmUserConfirmationRequestNegRes(CsrBtDeviceAddr deviceAddr,
                                              CsrBtAddressType addressType);
#define CsrBtCmScDmUserPasskeyRequestRes(_deviceAddr, _numericValue, _addressType){ \
        CsrBtCmSmUserPasskeyRequestRes *prim__ = pnew(CsrBtCmSmUserPasskeyRequestRes); \
        prim__->type         = CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES;   \
        prim__->deviceAddr   = _deviceAddr;                             \
        prim__->numericValue = _numericValue;                           \
        prim__->addressType = _addressType;                             \
        CsrBtCmPutMessageDownstream(prim__);}

void CsrBtCmScDmUserPasskeyRequestNegRes(CsrBtDeviceAddr deviceAddr,
                                         CsrBtAddressType addressType);

#define CsrBtCmScDmReadLocalOobDataReq() {                              \
        CsrBtCmSmReadLocalOobDataReq *prim__ = pnew(CsrBtCmSmReadLocalOobDataReq); \
        prim__->type             = CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ; \
        CsrBtCmPutMessageDownstream(prim__);}


#define CsrBtCmScDmSendKeypressNotificationReq(_deviceAddr, _notificationType, _addressType){ \
        CsrBtCmSmSendKeypressNotificationReq *prim__ = pnew(CsrBtCmSmSendKeypressNotificationReq); \
        prim__->type             = CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ; \
        prim__->notificationType = _notificationType;                   \
        prim__->deviceAddr       = _deviceAddr;                         \
        prim__->addressType = _addressType;                             \
        CsrBtCmPutMessageDownstream(prim__);}

void CsrBtCmSmSendRepairRes(CsrBtDeviceAddr deviceAddr,
                            CsrBool accept,
                            CsrUint16 repairId,
                            CsrBtAddressType addressType);
#else
#define CsrBtCmScDmIoCapabilityRequestRes(deviceAddr, ioCapability, authenticationRequirements, oobDataPresent,oobHashC, oobRandR, addressType)
#define CsrBtCmScDmIoCapabilityRequestNegRes(deviceAddr, reason, addressType)
#define CsrBtCmScDmUserConfirmationRequestRes(deviceAddr, addressType)
#define CsrBtCmScDmUserConfirmationRequestNegRes(deviceAddr, addressType)
#define CsrBtCmScDmUserPasskeyRequestRes(deviceAddr,  numericValue, addressType)
#define CsrBtCmScDmUserPasskeyRequestNegRes(deviceAddr, addressType)
#define CsrBtCmScDmReadLocalOobDataReq()
#define CsrBtCmScDmSendKeypressNotificationReq(deviceAddr, notificationType, addressType)
#define CsrBtCmSmSendRepairRes(deviceAddr, accept, repairId, addressType);
#endif

#ifdef __cplusplus
}
#endif

#endif

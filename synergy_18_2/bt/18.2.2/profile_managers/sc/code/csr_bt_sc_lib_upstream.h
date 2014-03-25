#ifndef CSR_BT_SC_LIB_UPSTREAM_H__
#define CSR_BT_SC_LIB_UPSTREAM_H__

#include "csr_synergy.h"
/****************************************************************************

Copyright Cambridge Silicon Radio Limited and its licensors 2011. All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_bt_sc_main.h"
#include "csr_sched.h"
#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif


void CsrBtScMessagePut(CsrSchedQid phandle, void* msg);

#define CsrBtScSspCompareIndSend(_phandle, _cod, _deviceAddr, _deviceName, _paired, _numericValue, _remoteAuth, _initiator, _addressType, _localAuth){\
    CsrBtScSspCompareInd *_scPrim = pnew(CsrBtScSspCompareInd);\
    _scPrim->type          = CSR_BT_SC_SSP_COMPARE_IND;\
    _scPrim->cod           = _cod;\
    _scPrim->paired        = _paired;\
    _scPrim->numericValue  = _numericValue;\
    _scPrim->authRequirements = _remoteAuth;\
    _scPrim->initiator     = _initiator;\
    CsrBtBdAddrCopy(&_scPrim->deviceAddr, _deviceAddr);\
    _scPrim->deviceName = CsrUtf8StrDup(_deviceName);\
    _scPrim->addressType = _addressType;\
    _scPrim->localAuthRequirements = _localAuth;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScSspNotificationIndSend(_phandle, _cod, _deviceAddr, _deviceName, _paired, _numericValue, _remoteAuth, _initiator, _addressType, _localAuth)\
{\
    CsrBtScSspNotificationInd *_scPrim = pnew(CsrBtScSspNotificationInd);\
    _scPrim->type          = CSR_BT_SC_SSP_NOTIFICATION_IND;\
    _scPrim->cod           = _cod;\
    _scPrim->paired        = _paired;\
    _scPrim->numericValue  = _numericValue;\
    _scPrim->authRequirements = _remoteAuth;\
    _scPrim->initiator     = _initiator;\
    CsrBtBdAddrCopy(&_scPrim->deviceAddr, _deviceAddr);\
    _scPrim->deviceName = CsrUtf8StrDup(_deviceName);\
    _scPrim->addressType = _addressType;\
    _scPrim->localAuthRequirements = _localAuth;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScSspPasskeyIndSend(_phandle, _cod, _deviceAddr, _deviceName, _paired, _remoteAuth, _initiator, _addressType, _localAuth)\
{\
    CsrBtScSspPasskeyInd *_scPrim = pnew(CsrBtScSspPasskeyInd);\
    _scPrim->type          = CSR_BT_SC_SSP_PASSKEY_IND;\
    _scPrim->cod           = _cod;\
    _scPrim->paired        = _paired;\
    _scPrim->authRequirements = _remoteAuth;\
    _scPrim->initiator     = _initiator;\
    CsrBtBdAddrCopy(&_scPrim->deviceAddr, _deviceAddr);\
    _scPrim->deviceName = CsrUtf8StrDup(_deviceName);\
    _scPrim->addressType = _addressType;\
    _scPrim->localAuthRequirements = _localAuth;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScSspJustWorksIndSend(_phandle, _cod, _deviceAddr, _deviceName, _paired, _remoteAuth, _initiator, _addressType, _localAuth)\
{\
    CsrBtScSspJustworksInd *_scPrim = pnew(CsrBtScSspJustworksInd);\
    _scPrim->type          = CSR_BT_SC_SSP_JUSTWORKS_IND;\
    _scPrim->cod           = _cod;\
    _scPrim->paired        = _paired;\
    _scPrim->authRequirements = _remoteAuth;\
    _scPrim->initiator     = _initiator;\
    CsrBtBdAddrCopy(&_scPrim->deviceAddr, _deviceAddr);\
    _scPrim->deviceName = CsrUtf8StrDup(_deviceName);\
    _scPrim->addressType = _addressType;\
    _scPrim->localAuthRequirements = _localAuth;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScSspKeypressNotificationIndSend(_phandle, _deviceAddr, _notificationType, _addressType)\
{\
    CsrBtScSspKeypressNotificationInd *_scPrim = pnew(CsrBtScSspKeypressNotificationInd);\
    _scPrim->type             = CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND;\
    _scPrim->notificationType = _notificationType;\
    CsrBtBdAddrCopy(&_scPrim->deviceAddr, _deviceAddr);\
    _scPrim->addressType = _addressType;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScPasskeyIndSend(_phandle, _cod, _deviceAddr, _name, _paired, _initiator, _addressType)\
{\
    CsrBtScPasskeyInd    *_scPrim = (CsrBtScPasskeyInd *) CsrPmemAlloc(sizeof(CsrBtScPasskeyInd));\
    _scPrim->type    = CSR_BT_SC_PASSKEY_IND;\
    _scPrim->cod     = _cod;\
    CsrBtBdAddrCopy(&_scPrim->deviceAddr, _deviceAddr);\
    _scPrim->deviceName = CsrUtf8StrDup(_name);\
    _scPrim->paired  = _paired;\
    _scPrim->initiator = _initiator;\
    _scPrim->addressType = _addressType;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScDebondCfmSend(_phandle, _resultCode, _resultSupplier, _deviceAddr, _addressType)\
{\
    CsrBtScDebondCfm *_scPrim = (CsrBtScDebondCfm *) CsrPmemAlloc(sizeof(CsrBtScDebondCfm));\
    _scPrim->type           = CSR_BT_SC_DEBOND_CFM;\
    _scPrim->resultCode     = _resultCode;\
    _scPrim->resultSupplier = _resultSupplier;\
    _scPrim->deviceAddr     = _deviceAddr;\
    _scPrim->addressType    = _addressType;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScAuthoriseIndSend(_phandle, _serviceId, _deviceAddr, _name, _addressType)\
{\
    CsrBtScAuthoriseInd *_scPrim = (CsrBtScAuthoriseInd *) CsrPmemAlloc(sizeof(CsrBtScAuthoriseInd));\
    _scPrim->type          = CSR_BT_SC_AUTHORISE_IND;\
    _scPrim->deviceAddr    = _deviceAddr;\
    _scPrim->serviceId     = _serviceId;\
    _scPrim->deviceName    = CsrUtf8StrDup(_name);\
    _scPrim->addressType   = _addressType;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScAuthoriseExtIndSend(_phandle, _serviceId, _deviceAddr, _name, _protocolId, _channel, _incoming, _addressType)\
{\
    CsrBtScAuthoriseExtInd *_scPrim = (CsrBtScAuthoriseExtInd *) CsrPmemAlloc(sizeof(CsrBtScAuthoriseExtInd));\
    _scPrim->type          = CSR_BT_SC_AUTHORISE_EXT_IND;\
    _scPrim->deviceAddr    = _deviceAddr;\
    _scPrim->serviceId     = _serviceId;\
    _scPrim->deviceName    = CsrUtf8StrDup(_name);\
    _scPrim->protocolId    = _protocolId;\
    _scPrim->channel       = _channel;\
    _scPrim->incoming      = _incoming;\
    _scPrim->addressType   = _addressType;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}


#define CsrBtScUpdateTrustLevelIndSend(_deviceAddr, _authorised, _addressType)\
{\
    CsrBtScUpdateTrustLevelInd *_scPrim = (CsrBtScUpdateTrustLevelInd *) CsrPmemAlloc(sizeof(CsrBtScUpdateTrustLevelInd));\
    _scPrim->type        = CSR_BT_SC_UPDATE_TRUST_LEVEL_IND;\
    _scPrim->deviceAddr  = _deviceAddr;\
    _scPrim->authorised  = _authorised;\
    _scPrim->addressType = _addressType;\
    CsrBtScMessagePut(CSR_BT_SD_IFACEQUEUE, _scPrim);\
}

#define CsrBtScBondCfmSend(_phandle, _resultCode, _resultSupplier, _deviceAddr, _theCod, _addedToScDbList, _addressType)\
{\
    CsrBtScBondCfm *_scPrim = (CsrBtScBondCfm *) CsrPmemAlloc(sizeof(CsrBtScBondCfm));\
    _scPrim->type            = CSR_BT_SC_BOND_CFM;\
    _scPrim->addedToScDbList = _addedToScDbList;\
    _scPrim->deviceAddr      = _deviceAddr;\
    _scPrim->cod             = _theCod;\
    _scPrim->resultCode      = _resultCode;\
    _scPrim->resultSupplier  = _resultSupplier;\
    _scPrim->addressType     = _addressType;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScBondIndSend(_phandle, _deviceAddr, _theCod, _resultCode, _resultSupplier, _addedToScDbList, _addressType)\
{\
    CsrBtScBondInd *_scPrim  = (CsrBtScBondInd *) CsrPmemAlloc(sizeof(CsrBtScBondInd));\
    _scPrim->type            = CSR_BT_SC_BOND_IND;\
    _scPrim->deviceAddr      = _deviceAddr;\
    _scPrim->cod             = _theCod;\
    _scPrim->addedToScDbList = _addedToScDbList;\
    _scPrim->resultCode      = _resultCode;\
    _scPrim->resultSupplier  = _resultSupplier;\
    _scPrim->addressType     = _addressType;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScAuthenticateCfmSend(_phandle, _resultCode, _resultSupplier, _deviceAddr, _addressType)\
{\
    CsrBtScAuthenticateCfm *_scPrim = (CsrBtScAuthenticateCfm *) CsrPmemAlloc(sizeof(CsrBtScAuthenticateCfm));\
    _scPrim->type            = CSR_BT_SC_AUTHENTICATE_CFM;\
    _scPrim->deviceAddr      = _deviceAddr;\
    _scPrim->resultCode      = _resultCode;\
    _scPrim->resultSupplier  = _resultSupplier;\
    _scPrim->addressType = _addressType;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScEncryptionCfmSend(_phandle, _resultCode, _resultSupplier, _encEnable, _deviceAddr)\
{\
    CsrBtScEncryptionCfm *_scPrim = (CsrBtScEncryptionCfm *) CsrPmemAlloc(sizeof(CsrBtScEncryptionCfm));\
    _scPrim->type           = CSR_BT_SC_ENCRYPTION_CFM;\
    _scPrim->resultCode     = _resultCode;\
    _scPrim->resultSupplier = _resultSupplier;\
    _scPrim->encEnabled     = _encEnable;\
    _scPrim->deviceAddr     = _deviceAddr;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScReadDeviceRecordIndSend(_phandle, _deviceProperties, _numOfDevices, _serviceChangedList)\
{\
    CsrBtScReadDeviceRecordInd *_scPrim = (CsrBtScReadDeviceRecordInd *) CsrPmemAlloc(sizeof(CsrBtScReadDeviceRecordInd));\
    _scPrim->type                     = CSR_BT_SC_READ_DEVICE_RECORD_IND;\
    _scPrim->devicePropertiesLength   = _numOfDevices;\
    _scPrim->deviceProperties         = _deviceProperties;\
    if (serviceChangedList)\
    {\
        _scPrim->serviceChangedListLength = _numOfDevices;\
    }\
    else\
    {\
        _scPrim->serviceChangedListLength = 0;\
    }\
    _scPrim->serviceChangedList = _serviceChangedList;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScReadDeviceRecordCfmSend(_phandle, _deviceProperties, _numOfDevices, _totalNumOfDevices, _serviceChangedList)\
{\
    CsrBtScReadDeviceRecordCfm *_scPrim = (CsrBtScReadDeviceRecordCfm *) CsrPmemAlloc(sizeof(CsrBtScReadDeviceRecordCfm));\
    _scPrim->type                     = CSR_BT_SC_READ_DEVICE_RECORD_CFM;\
    _scPrim->devicePropertiesLength   = _numOfDevices;\
    _scPrim->totalNumOfDevices        = _totalNumOfDevices;\
    _scPrim->deviceProperties         = _deviceProperties;\
    _scPrim->serviceChangedListLength = _numOfDevices;\
    if (_serviceChangedList)\
    {\
        _scPrim->serviceChangedListLength = _numOfDevices;\
    }\
    else\
    {\
        _scPrim->serviceChangedListLength = 0;\
    }\
    _scPrim->serviceChangedList = _serviceChangedList;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScSetSecurityModeCfmSend(_phandle, _resultCode, _resultSupplier)\
{\
    CsrBtScSetSecurityModeCfm *_scPrim = (CsrBtScSetSecurityModeCfm *) CsrPmemAlloc(sizeof(CsrBtScSetSecurityModeCfm));\
    _scPrim->type           = CSR_BT_SC_SET_SECURITY_MODE_CFM;\
    _scPrim->resultCode     = _resultCode;\
    _scPrim->resultSupplier = _resultSupplier;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScModeIndSend(_phandle, _mode)\
{\
    CsrBtScModeInd *_scPrim = (CsrBtScModeInd *) CsrPmemAlloc(sizeof(CsrBtScModeInd));\
    _scPrim->type = CSR_BT_SC_MODE_IND;\
    _scPrim->mode = _mode;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScRebondIndSend(_phandle, _cod, _deviceAddr, _name, _addressType)\
{\
    CsrBtScRebondInd *_scPrim = (CsrBtScRebondInd *) CsrPmemAlloc(sizeof(CsrBtScRebondInd));\
    _scPrim->type    = CSR_BT_SC_REBOND_IND;\
    _scPrim->cod     = _cod;\
    CsrBtBdAddrCopy(&_scPrim->deviceAddr, _deviceAddr);\
    _scPrim->deviceName = CsrUtf8StrDup(_name);\
    _scPrim->addressType = _addressType;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScSspPairingIndSend(_phandle, _cod, _deviceAddr, _name, _authValid, _authRequirements, _paired, _initiator, _addressType, _localAuth)\
{\
    CsrBtScSspPairingInd *_scPrim = (CsrBtScSspPairingInd *) CsrPmemAlloc(sizeof(CsrBtScSspPairingInd));\
    _scPrim->type              = CSR_BT_SC_SSP_PAIRING_IND;\
    _scPrim->cod               = _cod;\
    _scPrim->authValid         = _authValid;\
    _scPrim->authRequirements  = _authRequirements;\
    _scPrim->paired            = _paired;\
    _scPrim->initiator         = _initiator;\
    CsrBtBdAddrCopy(&_scPrim->deviceAddr, _deviceAddr);\
    _scPrim->deviceName = CsrUtf8StrDup(_name);\
    _scPrim->addressType = _addressType;\
    _scPrim->localAuthRequirements = _localAuth;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScReadLocalOobDataCfmSend(_localAddr,_phandle, _oobHashC,_oobRandR, _resultCode, _resultSupplier, _addressType)\
{\
    CsrBtScReadLocalOobDataCfm *_scPrim = (CsrBtScReadLocalOobDataCfm *) CsrPmemAlloc(sizeof(CsrBtScReadLocalOobDataCfm));\
    _scPrim->type           = CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM;\
    _scPrim->deviceAddr     = _localAddr;\
    _scPrim->oobHashC       = _oobHashC;\
    _scPrim->oobRandR       = _oobRandR;\
    _scPrim->oobHashCLength = _oobHashC ? SIZE_OOB_DATA : 0;\
    _scPrim->oobRandRLength = _oobRandR ? SIZE_OOB_DATA : 0;\
    _scPrim->resultCode     = _resultCode;\
    _scPrim->resultSupplier = _resultSupplier;\
    _scPrim->addressType = _addressType;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}

#define CsrBtScDebugModeCfmSend(_phandle, _resultCode, _resultSupplier, _enabled)\
{\
    CsrBtScDebugModeCfm *_scPrim = (CsrBtScDebugModeCfm *) CsrPmemAlloc(sizeof(CsrBtScDebugModeCfm));\
    _scPrim->type            = CSR_BT_SC_DEBUG_MODE_CFM;\
    _scPrim->resultCode      = _resultCode;\
    _scPrim->resultSupplier  = _resultSupplier;\
    _scPrim->enabled         = _enabled;\
    CsrBtScMessagePut(_phandle, _scPrim);\
}
#define CsrBtScDatabaseCfmSend(_appHandle, _deviceAddr, _opcode, _entry, _keyType, _length, _data, _resultCode, _addressType)\
{\
    CsrBtScDatabaseCfm *_scPrim = (CsrBtScDatabaseCfm*)CsrPmemAlloc(sizeof(CsrBtScDatabaseCfm));\
    _scPrim->type = CSR_BT_SC_DATABASE_CFM;\
    _scPrim->deviceAddr = *_deviceAddr;\
    _scPrim->opcode = _opcode;\
    _scPrim->entry = _entry;\
    _scPrim->resultCode = _resultCode;\
    _scPrim->resultSupplier = CSR_BT_SUPPLIER_SC;\
    _scPrim->keyType = _keyType;\
    _scPrim->addressType = _addressType;\
    /* Because input data is from the database and contained therein,\
     * we need to copy it before passing in onwards */\
    _scPrim->length = _length;\
    if(_length > 0)\
    {\
        _scPrim->data = (CsrUint8*)CsrPmemAlloc(_length);\
        CsrMemCpy(_scPrim->data, _data, _length);\
    }\
    else\
    {\
        _scPrim->data = NULL;\
    }\
    CsrBtScMessagePut(_appHandle, _scPrim);\
}

#ifdef CSR_BT_LE_ENABLE
#define CsrBtScLeSecurityCfmSend(_type, _appHandle, _resultCode, _resultSupplier, _address, _addressType, _addedToScDbList)\
{\
    CsrBtScLeSecurityCfm *_scPrim = (CsrBtScLeSecurityCfm*)CsrPmemAlloc(sizeof(CsrBtScLeSecurityCfm));\
    _scPrim->type = _type;\
    _scPrim->resultCode = _resultCode;\
    _scPrim->resultSupplier = _resultSupplier;\
    CsrBtAddrCopyWithType(&_scPrim->address, _addressType, &_address);\
    _scPrim->addedToScDbList = _addedToScDbList;\
    CsrBtScMessagePut(_appHandle, _scPrim);\
}
#endif

#define CsrBtScEncryptionKeySizeCfmSend(_qid, _addr, _keySize, _resultCode, _resultSupplier)\
{\
    CsrBtScEncryptionKeySizeCfm *_scPrim = (CsrBtScEncryptionKeySizeCfm*)CsrPmemAlloc(sizeof(CsrBtScEncryptionKeySizeCfm));\
    _scPrim->type = CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM;\
    _scPrim->address = _addr;\
    _scPrim->keySize = _keySize;\
    _scPrim->resultCode = _resultCode;\
    _scPrim->resultSupplier = _resultSupplier;\
    CsrBtScMessagePut(_qid, _scPrim);\
}

#ifdef __cplusplus
}
#endif

#endif /* Defined CSR_BT_SC_LIB_UPSTREAM_H__ */

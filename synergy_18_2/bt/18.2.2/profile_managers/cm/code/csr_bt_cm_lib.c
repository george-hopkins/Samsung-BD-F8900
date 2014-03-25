/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_profiles.h"
#include "csr_pmem.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

/* Common put_message functions to reduce code size */
void CsrBtCmMsgTransport(void* __msg)
{
    CsrMsgTransport(CSR_BT_CM_IFACEQUEUE,CSR_BT_CM_PRIM,__msg);
}

void CsrBtCmPutMessageDownstream(void* __msg)
{
    CsrSchedMessagePut(CSR_BT_CM_IFACEQUEUE,CSR_BT_CM_PRIM,__msg);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtcmWriteLinkSuperVisionTimeoutReqSend
 *
 *  DESCRIPTION
 *      This command will write the value of the link supervision timeout
 *        parameter. It is used to monitor link loss. Value of 0x0000 disables it.
 *        The timeout value N range from 0x0001 - 0xffff. In seconds it means from
 *        N*0.625ms = 0,625ms - 40.9 seconds.
 *
 *  PARAMETERS
 *      phandle:         protocol handle
 *      CsrBtDeviceAddr:    theDeviceAddr
 *      CsrUint16:        Link supervision timeout value.
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_WRITE_LINK_SUPERVISION_TIMEOUT
CsrBtCmWriteLinkSupervTimeoutReq *CsrBtCmWriteLinkSuperVisionTimeoutReq_struct(CsrSchedQid       thePhandle,
                                                                               CsrBtDeviceAddr    theDeviceAddr,
                                                                               CsrUint16        timeout)
{
    CsrBtCmWriteLinkSupervTimeoutReq *prim;

    prim = (CsrBtCmWriteLinkSupervTimeoutReq*) CsrPmemAlloc(sizeof(CsrBtCmWriteLinkSupervTimeoutReq));
    prim->type = CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ;
    prim->phandle = thePhandle;
    prim->deviceAddr = theDeviceAddr;
    prim->timeout = timeout;
    return(prim);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmReadLocalBdAddrReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      phandle:            protocol handle
 *----------------------------------------------------------------------------*/
CsrBtCmReadLocalBdAddrReq *CsrBtCmReadLocalBdAddrReq_struct(CsrSchedQid    thePhandle)
{
    CsrBtCmReadLocalBdAddrReq *prim;

    prim = (CsrBtCmReadLocalBdAddrReq*) CsrPmemAlloc(sizeof(CsrBtCmReadLocalBdAddrReq));
    prim->type = CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ;
    prim->phandle = thePhandle;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmReadLocalNameReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      phandle:            protocol handle
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_READ_LOCAL_NAME 
CsrBtCmReadLocalNameReq *CsrBtCmReadLocalNameReq_struct(CsrSchedQid    thePhandle)
{
    CsrBtCmReadLocalNameReq *prim;

    prim = (CsrBtCmReadLocalNameReq*) CsrPmemAlloc(sizeof(CsrBtCmReadLocalNameReq));
    prim->type = CSR_BT_CM_READ_LOCAL_NAME_REQ;
    prim->phandle = thePhandle;
    return(prim);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmReadRemoteNameReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      phandle:               protocol handle
 *      deviceAddr:            BT address of the device to read remote name
 *----------------------------------------------------------------------------*/
void CsrBtCmReadRemoteNameReqSend(CsrSchedQid       thePhandle,
                                  CsrBtDeviceAddr    theDeviceAddr)
{
    CsrBtCmReadRemoteNameReq *prim;

    prim = (CsrBtCmReadRemoteNameReq*) CsrPmemAlloc(sizeof(CsrBtCmReadRemoteNameReq));
    prim->type = CSR_BT_CM_READ_REMOTE_NAME_REQ;
    prim->phandle = thePhandle;
    prim->deviceAddr = theDeviceAddr;
    CsrBtCmMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmReadRemoteVersionReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      phandle:               protocol handle
 *      deviceAddr:            BT address of the device to read remote version
 *----------------------------------------------------------------------------*/
void CsrBtCmReadRemoteVersionReqSend(CsrSchedQid       thePhandle,
                                     CsrBtDeviceAddr    theDeviceAddr)
{
    CsrBtCmReadRemoteVersionReq *prim;

    prim = (CsrBtCmReadRemoteVersionReq*) CsrPmemAlloc(sizeof(CsrBtCmReadRemoteVersionReq));
    prim->type = CSR_BT_CM_READ_REMOTE_VERSION_REQ;
    prim->appHandle = thePhandle;
    prim->deviceAddr = theDeviceAddr;
    CsrBtCmMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmAclDetachReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      phandle:               protocol handle
 *      deviceAddr:            BT address of the device acl to detach
 *      flags:                 Set to CM_ACL_FLAG_ALL in order to always detach. Alternatives can
 *                             be found in csr_bt_cm_prim.h
 *----------------------------------------------------------------------------*/
void CsrBtCmAclDetachReqSendEx(CsrSchedQid thePhandle,
                               CsrBtDeviceAddr theDeviceAddr,
                               CsrBtCmDetach flags,
                               CsrBtAddressType addressType)
{
    CsrBtCmAclDetachReq *prim;

    prim = (CsrBtCmAclDetachReq *) CsrPmemAlloc(sizeof(CsrBtCmAclDetachReq));
    prim->type = CSR_BT_CM_ACL_DETACH_REQ;
    prim->phandle = thePhandle;
    prim->deviceAddr = theDeviceAddr;
    prim->flags = flags;
    prim->addressType = addressType;
    CsrBtCmMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmReadTxPowerLevelReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      phandle:            protocol handle
 *      deviceAddr:         BT address of the device to read remote name
 *      levelType:          The maximum power level as defined in the Bluetooth HCI
 *                          specification.
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_READ_TX_POWER_LEVEL
CsrBtCmReadTxPowerLevelReq *CsrBtCmReadTxPowerLevelReq_struct(CsrSchedQid thePhandle,
                                                              CsrBtDeviceAddr theDeviceAddr,
                                                              CsrUint8 theLevelType,
                                                              CsrBtAddressType addressType)
{
    CsrBtCmReadTxPowerLevelReq *prim;

    prim = (CsrBtCmReadTxPowerLevelReq*) CsrPmemAlloc(sizeof(CsrBtCmReadTxPowerLevelReq));
    prim->type = CSR_BT_CM_READ_TX_POWER_LEVEL_REQ;
    prim->appHandle = thePhandle;
    prim->deviceAddr = theDeviceAddr;
    prim->levelType = theLevelType;
    prim->addressType = addressType;
    return(prim);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcSearchReqSend
 *      CsrBtCmSdcSearchExtReqSend
 *
 *  DESCRIPTION
 *      Submits a search request to the SDC sub-system
 *
 *  PARAMETERS
 *      appHandle:           protocol handle
 *      deviceAddr:          BT address of the device to read remote name
 *      serviceList:         A list of Services (UUID) to search for
 *      serviceListSize:     Number of services to search for
 *      extendedUuidSearch:  Defines when a UUID must be consider valid
 *----------------------------------------------------------------------------*/
void CsrBtCmSdcSearchReqSendFunc(CsrSchedQid          appHandle,
                                 CsrBtDeviceAddr deviceAddr,
                                 CsrBtUuid32     *serviceList,
                                 CsrUint8        serviceListSize,
                                 CsrBool         extendedUuidSearch)
{
    CsrBtCmSdcSearchReq *prim;

    prim = (CsrBtCmSdcSearchReq*) CsrPmemAlloc(sizeof(CsrBtCmSdcSearchReq));
    prim->type = CSR_BT_CM_SDC_SEARCH_REQ;
    prim->appHandle = appHandle;
    prim->deviceAddr = deviceAddr;
    prim->serviceList =  serviceList;
    prim->serviceListSize = serviceListSize;
    prim->extendedUuidSearch = extendedUuidSearch;
    CsrBtCmMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcUuid128SearchReqSend
 *
 *  DESCRIPTION
 *      Submits a search request with a 128 bit uuid16_t to the SDC sub-system
 *
 *  PARAMETERS
 *      appHandle:             protocol handle
 *      deviceAddr:            BT address of the device to read remote name
 *      serviceList:           A list of 128 bit Services (UUID128) to search for
 *      serviceListSize:       Number of services of 128 bit to search for
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
CsrBtCmSdcUuid128SearchReq *CsrBtCmSdcUuid128SearchReq_struct(CsrSchedQid      appHandle,
                                                              CsrBtDeviceAddr   deviceAddr,
                                                              CsrBtUuid128      *serviceList,
                                                              CsrUint8        serviceListSize)
{
    CsrBtCmSdcUuid128SearchReq *prim;

    prim = (CsrBtCmSdcUuid128SearchReq*) CsrPmemAlloc(sizeof(CsrBtCmSdcUuid128SearchReq));
    prim->type = CSR_BT_CM_SDC_UUID128_SEARCH_REQ;
    prim->appHandle = appHandle;
    prim->deviceAddr = deviceAddr;
    prim->serviceList = serviceList;
    prim->serviceListSize = serviceListSize;
    return(prim);
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcAttributeReqSend
 *      CsrBtCmSdcAttributeRangeReqSend
 *
 *  DESCRIPTION
 *      Submits an attribute request to the SDC sub-system
 *
 *  PARAMETERS
 *        serviceHandle:                The handle of the service
 *        attributeIdentifier:          The attribute to retrieve. Note, if
 *                                      CsrBtCmSdcAttributeRangeReqSend is used
 *                                      then it defines the beginning attributeId
 *                                      of the range
 *        upperRangeAttributeIdentifier:The ending attributeId of the range
 *        maxBytesToReturn:             The maximum number of attribute bytes
 *                                      to be returned
 *----------------------------------------------------------------------------*/
void CsrBtCmSdcAttributeRangeReqSend(CsrBtUuid32 serviceHandle,
                                     CsrUint16   attributeIdentifier,
                                     CsrUint16   upperRangeAttributeIdentifier,
                                     CsrUint16   maxBytesToReturn)
{
    CsrBtCmSdcAttributeReq *prim;

    prim = (CsrBtCmSdcAttributeReq*) CsrPmemAlloc(sizeof(CsrBtCmSdcAttributeReq));
    prim->type = CSR_BT_CM_SDC_ATTRIBUTE_REQ;
    prim->serviceHandle = serviceHandle;
    prim->attributeIdentifier = attributeIdentifier;
    prim->maxBytesToReturn = maxBytesToReturn;
    prim->upperRangeAttributeIdentifier = upperRangeAttributeIdentifier;
    CsrBtCmMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcCloseReqSend
 *
 *  DESCRIPTION
 *      Closing the SDC channel
 *
 *  PARAMETERS
 *      appHandle:            protocol handle
 *----------------------------------------------------------------------------*/
void CsrBtCmSdcCloseReqSend(CsrSchedQid appHandle)
{
    CsrBtCmSdcCloseReq *prim;
    prim = (CsrBtCmSdcCloseReq*) CsrPmemAlloc(sizeof(CsrBtCmSdcCloseReq));
    prim->type = CSR_BT_CM_SDC_CLOSE_REQ;
    prim->appHandle = appHandle;
    CsrBtCmMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdsRegisterReqSend
 *
 *  DESCRIPTION
 *      Request to register a service with the service discovery server
 *
 *  PARAMETERS
 *      appHandle:             protocol handle
 *      serviceRecord:         The service record
 *      serviceRecordSize:     Size of the service record
 *      context                Opaque context number
 *----------------------------------------------------------------------------*/
void CsrBtCmSdsRegisterReqExtSend(CsrSchedQid appHandle,
                                  CsrUint8   *serviceRecord,
                                  CsrUint16   serviceRecordSize,
                                  CsrUint16   context)
{
    CsrBtCmSdsRegisterReq *prim;

    prim = (CsrBtCmSdsRegisterReq*) CsrPmemAlloc(sizeof(CsrBtCmSdsRegisterReq));
    prim->type = CSR_BT_CM_SDS_REGISTER_REQ;
    prim->appHandle = appHandle;
    prim->serviceRecord = serviceRecord;
    prim->serviceRecordSize = serviceRecordSize;
    prim->context = context;
    CsrBtCmMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdsUnRegisterReqSend
 *
 *  DESCRIPTION
 *      Request to unregister a service with the service discovery server
 *
 *  PARAMETERS
 *      appHandle:            protocol handle
 *      serviceRecHandle:     The service record handle
 *      context               Opaque context number
 *----------------------------------------------------------------------------*/
void CsrBtCmSdsUnRegisterExtReqSend(CsrSchedQid appHandle,
                                    CsrUint32   serviceRecHandle,
                                    CsrUint16   context)
{
    CsrBtCmSdsUnregisterReq *prim;

    prim = (CsrBtCmSdsUnregisterReq*) CsrPmemAlloc(sizeof(CsrBtCmSdsUnregisterReq));
    prim->type = CSR_BT_CM_SDS_UNREGISTER_REQ;
    prim->appHandle = appHandle;
    prim->serviceRecHandle = serviceRecHandle;
    prim->context = context;
    CsrBtCmMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CmWriteInquirySettingsReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *      phandle:           protocol handle
 *      interval:          the inquiry scan interval
 *      window:            the inquiry scan window
 *----------------------------------------------------------------------------*/
void CsrBtCmWriteInquiryScanSettingsReqSend(CsrSchedQid  ph,
                                            CsrUint16   interval,
                                            CsrUint16   window)
{
    CsrBtCmWriteInquiryscanSettingsReq *prim;

    prim = (CsrBtCmWriteInquiryscanSettingsReq*)CsrPmemAlloc(sizeof(CsrBtCmWriteInquiryscanSettingsReq));
    prim->type = CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ;
    prim->appHandle = ph;
    prim->interval = interval;
    prim->window = window;

    CsrBtCmMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmReadRemoteFeaturesReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *      phandle:          protocol handle
 *      theDeviceAddr:    the device address of the device which features is
 *                        requested
 *----------------------------------------------------------------------------*/
CsrBtCmReadRemoteFeaturesReq *CsrBtCmReadRemoteFeaturesReq_struct(CsrSchedQid     thePhandle,
                                                                  CsrBtDeviceAddr  theDeviceAddr)
{
    CsrBtCmReadRemoteFeaturesReq *prim;

    prim = (CsrBtCmReadRemoteFeaturesReq*) CsrPmemAlloc(sizeof(CsrBtCmReadRemoteFeaturesReq));
    prim->type = CSR_BT_CM_READ_REMOTE_FEATURES_REQ;
    prim->appHandle = thePhandle;
    prim->deviceAddr = theDeviceAddr;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmRoleDiscoveryReqSend
 *
 *  DESCRIPTION
 *      Discover the current role (Master or Slave)
 *
 *  PARAMETERS
 *      appHandle:             protocol handle
 *      deviceAddr:            BT address
 *----------------------------------------------------------------------------*/
void CsrBtCmRoleDiscoveryReqSend(CsrSchedQid       appHandle,
                                 CsrBtDeviceAddr    deviceAddr)
{
    CsrBtCmRoleDiscoveryReq *prim;

    prim = (CsrBtCmRoleDiscoveryReq*) CsrPmemAlloc(sizeof(CsrBtCmRoleDiscoveryReq));
    prim->type = CSR_BT_CM_ROLE_DISCOVERY_REQ;
    prim->phandle = appHandle;
    prim->deviceAddr = deviceAddr;
    CsrBtCmMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmCancelReadRemoteNameReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      apphandle:        protocol handle
 *      deviceAddr:       BT address of the device that is being read
 *----------------------------------------------------------------------------*/
void CsrBtCmCancelReadRemoteNameReqSend(CsrSchedQid      theAppHandle,
                                        CsrBtDeviceAddr   theDeviceAddr)
{
    CsrBtCmCancelReadRemoteNameReq *prim;

    prim = (CsrBtCmCancelReadRemoteNameReq*) CsrPmemAlloc(sizeof(CsrBtCmCancelReadRemoteNameReq));
    prim->type = CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ;
    prim->appHandle = theAppHandle;
    prim->deviceAddr = theDeviceAddr;
    CsrBtCmMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmEirUpdateManufacturerDataReqSend
 *
 *  DESCRIPTION
 *      Used for setting a the manufacturer specific data in an Extended Inquiry
 *      Response.
 *
 *    PARAMETERS
 *      appHandle:                  Application handle
 *        manufacturerDataSettings:   Settings for handling the manufacturer data.
 *                                  Refer to the documentation for further details.
 *      manufacturerDataLength:     Length of the data in *manufacturerData
 *      manufacturerData:           The actual manufacturer data as it will
 *                                  appear in the EIR.
 *----------------------------------------------------------------------------*/
void CsrBtCmEirUpdateManufacturerDataReqSend(CsrSchedQid     appHandle,
                                             CsrUint8        manufacturerDataSettings,
                                             CsrUint8        manufacturerDataLength,
                                             CsrUint8       *manufacturerData)
{
    CsrBtCmEirUpdateManufacturerDataReq *msg;

    msg                             = (CsrBtCmEirUpdateManufacturerDataReq *)CsrPmemAlloc(sizeof(CsrBtCmEirUpdateManufacturerDataReq));
    msg->type                       = CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ;
    msg->appHandle                  = appHandle;
    msg->manufacturerDataSettings   = manufacturerDataSettings;
    msg->manufacturerDataLength     = manufacturerDataLength;

    if (manufacturerData != NULL && manufacturerDataLength != 0)
    {
        msg->manufacturerData       = manufacturerData;
    }
    else
    {
        msg->manufacturerData       = NULL;
        msg->manufacturerDataLength = 0;
    }

    CsrBtCmMsgTransport(msg);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmWriteMajorMinorCodReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *      phandle:   protocol handle
 *      service:   A Class of device value given from the application
 *      major:     A Major Class of device value given from the application
 *      minor:     A Minor Class of device value given from the application
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_WRITE_COD
CsrBtCmWriteCodReq *CsrBtCmWriteCodReq_struct(CsrSchedQid          phandle,
                                              CsrBtCmUpdateFlags   updateFlags,
                                              CsrBtClassOfDevice   service,
                                              CsrBtClassOfDevice   major,
                                              CsrBtClassOfDevice   minor)
{
    CsrBtCmWriteCodReq *prim;

    prim                       = (CsrBtCmWriteCodReq*) CsrPmemAlloc(sizeof(CsrBtCmWriteCodReq));
    prim->type                 = CSR_BT_CM_WRITE_COD_REQ;
    prim->appHandle            = phandle;
    prim->updateFlags          = updateFlags;
    prim->serviceClassOfDevice = service;
    prim->majorClassOfDevice   = major;
    prim->minorClassOfDevice   = minor;

    return(prim);
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSetEventMaskReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        phandle    :  Protocol handle
 *        eventMask  :  Describes which extended information an application
 *                      will subscribe for
 *        condition  :  Filter condition
 *----------------------------------------------------------------------------*/
void CsrBtCmSetEventMaskReqSend(CsrSchedQid phandle, CsrUint32 eventMask, CsrUint32 conditionMask)
{
    CsrBtCmSetEventMaskReq *prim;

    prim = (CsrBtCmSetEventMaskReq *) CsrPmemAlloc(sizeof(CsrBtCmSetEventMaskReq));
    prim->type          = CSR_BT_CM_SET_EVENT_MASK_REQ;
    prim->phandle       = phandle;
    prim->eventMask     = eventMask;
    prim->conditionMask = conditionMask;

    CsrBtCmMsgTransport(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSwitchRoleReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        phandle       :    protocol handle
 *        deviceAddr    :    BT address of the device to read remote version
 *        role          :    Requested role (master/slave/unknown)
 *        roleType      :    Requested role (only CSR_BT_CM_SWITCH_ROLE_TYPE_ONESHOT supported)
 *        config        :    RFU - shall be zero
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_SWITCH_ROLE_PUBLIC
void CsrBtCmSwitchRoleReqSend(CsrSchedQid     phandle,
                              CsrBtDeviceAddr deviceAddr,
                              CsrUint8        role,
                              CsrBtCmRoleType roleType,
                              CsrUint32       config)
{
    CsrBtCmSwitchRoleReq *prim;

    prim = (CsrBtCmSwitchRoleReq*) CsrPmemAlloc(sizeof(CsrBtCmSwitchRoleReq));
    prim->type        = CSR_BT_CM_SWITCH_ROLE_REQ;
    prim->appHandle   = phandle;
    prim->deviceAddr  = deviceAddr;
    prim->role        = role;
    prim->roleType    = roleType;
    prim->config      = config;
    CsrBtCmMsgTransport(prim);
}
#endif

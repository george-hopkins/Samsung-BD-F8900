/****************************************************************************

            Copyright Cambridge Silicon Radio Limited and its licensors 2011.

            All rights reserved.

REVISION:   $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_profiles.h"
#include "csr_pmem.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_l2cap_conftab.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmInquiryReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      phandle:                    Protocol handle
 *      configMask:                 Config mask passed from sd serach request
 *      inquiryAccessCode:          The inquiry access code to use - defined in csr_bt_cm_prim.h
 *      inquiryTxPowerLevel:        TX power level to be used when inquiring only
 *----------------------------------------------------------------------------*/
CsrBtCmInquiryReq *CsrBtCmInquiryReq_struct(CsrSchedQid      appHandle,
                                            CsrUint24   inquiryAccessCode,
                                            CsrInt8     inquiryTxPowerLevel,
                                            CsrUint32   configMask)
{
    CsrBtCmInquiryReq *prim;

    prim                            = (CsrBtCmInquiryReq*) CsrPmemAlloc(sizeof(CsrBtCmInquiryReq));
    prim->type                      = CSR_BT_CM_INQUIRY_REQ;
    prim->appHandle                 = appHandle;
    prim->inquiryAccessCode         = inquiryAccessCode;
    prim->configMask                = configMask;
    prim->inquiryTxPowerLevel       = inquiryTxPowerLevel;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmCancelInquiryReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *      phandle:        protocol handle
 *----------------------------------------------------------------------------*/
CsrBtCmCancelInquiryReq *CsrBtCmCancelInquiryReq_struct(CsrSchedQid appHandle)

{
    CsrBtCmCancelInquiryReq *prim;

    prim            = (CsrBtCmCancelInquiryReq*) CsrPmemAlloc(sizeof(CsrBtCmCancelInquiryReq));
    prim->type      = CSR_BT_CM_CANCEL_INQUIRY_REQ;
    prim->phandle   = appHandle;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmReadIacReqSend
 *
 *  DESCRIPTION
 *      Submits to read the current Inquiry Access Code of the BC chip
 *
 *  PARAMETERS
 *        appHandle:             application handle
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_IAC
CsrBtCmReadIacReq *CsrBtCmReadIacReq_struct(CsrSchedQid    appHandle)
{
    CsrBtCmReadIacReq *prim;

    prim                        = (CsrBtCmReadIacReq *)CsrPmemAlloc(sizeof(CsrBtCmReadIacReq));
    prim->type                  = CSR_BT_CM_READ_IAC_REQ;
    prim->appHandle             = appHandle;
    return(prim);
}
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmWriteIacReqSend
 *
 *  DESCRIPTION
 *      Submits to write a new Inquiry Access Code to the BC chip
 *
 *  PARAMETERS
 *        appHandle:             protocol handle
 *      iac:                 The new Inquiry Access Code
 *----------------------------------------------------------------------------*/
CsrBtCmWriteIacReq *CsrBtCmWriteIacReq_struct(CsrSchedQid appHandle, CsrUint24 iac)
{
    CsrBtCmWriteIacReq *prim;

    prim                        = (CsrBtCmWriteIacReq *)CsrPmemAlloc(sizeof(CsrBtCmWriteIacReq));
    prim->type                  = CSR_BT_CM_WRITE_IAC_REQ;
    prim->appHandle             = appHandle;
    prim->iac                   = iac;
    return(prim);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcOpenReqSend
 *
 *  DESCRIPTION
 *      Submits to open a SDC channel. Must only be used if application starts
 *      reading Attr direct hereafter
 *
 *  PARAMETERS
 *        appHandle:             protocol handle
 *        deviceAddr:             BT address of the device to read remote name
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_SDC
CsrBtCmSdcOpenReq *CsrBtCmSdcOpenReq_struct(CsrSchedQid        appHandle,
                                            CsrBtDeviceAddr     deviceAddr)
{
    CsrBtCmSdcOpenReq *prim;

    prim                        = (CsrBtCmSdcOpenReq *) CsrPmemAlloc(sizeof(CsrBtCmSdcOpenReq));
    prim->type                  = CSR_BT_CM_SDC_OPEN_REQ;
    prim->appHandle             = appHandle;
    prim->deviceAddr            = deviceAddr;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcCancelOpenReqSend
 *
 *  DESCRIPTION
 *      Cancel a CSR_BT_CM_SDC_OPEN_REQ
 *
 *  PARAMETERS
 *        appHandle:             protocol handle
 *        deviceAddr:             BT address of the device to read remote name
 *----------------------------------------------------------------------------*/
CsrBtCmSdcCancelSearchReq *CsrBtCmSdcCancelOpenReq_struct(CsrSchedQid       appHandle,
                                                          CsrBtDeviceAddr    deviceAddr)

{
    CsrBtCmSdcCancelSearchReq *prim;

    prim                        = (CsrBtCmSdcCancelSearchReq *) CsrPmemAlloc(sizeof(CsrBtCmSdcCancelSearchReq));
    prim->type                  = CSR_BT_CM_SDC_CANCEL_SEARCH_REQ;
    prim->appHandle             = appHandle;
    prim->deviceAddr            = deviceAddr;
    prim->typeToCancel          = CSR_BT_CM_SDC_OPEN_REQ;
    return(prim);
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmAlwaysSupportMasterRoleReqSend
 *
 *  DESCRIPTION
 *      Make sure that Synergy BT always tries to be MASTER, even if it only got one
 *      ACL connection
 *
 *  PARAMETERS
 *        alwaysSupportMasterRole:
 *              TRUE then Synergy BT always tries to be master. FALSE then BCHS
 *              only tries to be MASTER if it got more than one ACL connection
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_ALWAYS_SUPPORT_MASTER_ROLE
CsrBtCmAlwaysSupportMasterRoleReq *CsrBtCmAlwaysSupportMasterRoleReq_struct(CsrBool  theAlwaysSupportMasterRole)

{
    CsrBtCmAlwaysSupportMasterRoleReq *prim;

    prim                            = (CsrBtCmAlwaysSupportMasterRoleReq *) CsrPmemAlloc(sizeof(CsrBtCmAlwaysSupportMasterRoleReq));
    prim->type                      = CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ;
    prim->alwaysSupportMasterRole   = theAlwaysSupportMasterRole;
    return(prim);
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmCommonScoConnectPrepare
 *
 *  DESCRIPTION
 *      Allocate a buffer '*parms' that points to some memory large enought to contain
 *      'count' number of eSCO parameters.
 *
 *  PARAMETERS
 *      parms
 *      parmsOffset
 *      parmsLen:
 *      count
 *----------------------------------------------------------------------------*/
void CsrBtCmCommonScoConnectPrepare(CsrBtCmScoCommonParms                      **parms,
                                    CsrUint16        *parmsOffset,
                                    CsrUint16        *parmsLen,
                                    CsrUint16        count)
{
    *parmsOffset    = 0;
    *parmsLen       = count;
    *parms          = CsrPmemAlloc(*parmsLen * sizeof(CsrBtCmScoCommonParms));
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmCommonScoConnectBuild
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      parms
 *      parmsOffset
 *      audioQuality:
 *      txBandwidth
 *      rxBandwidth
 *      maxLatency
 *      voiceSettings
 *      reTxEffort
 *----------------------------------------------------------------------------*/
void CsrBtCmCommonScoConnectBuild(CsrBtCmScoCommonParms                    *parms,
                                  CsrUint16           *parmsOffset,
                                  hci_pkt_type_t     theAudioQuality,
                                  CsrUint32           theTxBandwidth,
                                  CsrUint32           theRxBandwidth,
                                  CsrUint16           theMaxLatency,
                                  CsrUint16           theVoiceSettings,
                                  CsrUint8            theReTxEffort)
{
    CsrUint16 offset = *parmsOffset;

    parms[offset].audioQuality = theAudioQuality;
    parms[offset].txBandwidth = theTxBandwidth;
    parms[offset].rxBandwidth = theRxBandwidth;
    parms[offset].maxLatency = theMaxLatency;
    parms[offset].voiceSettings = theVoiceSettings;
    parms[offset].reTxEffort = theReTxEffort;

    *parmsOffset += 1;
}

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcRfcSearchReqSend
 *
 *  DESCRIPTION
 *      Submits a search request to the SDC sub-system
 *
 *  PARAMETERS
 *        appHandle:             protocol handle
 *        deviceAddr:             BT address of the device to read remote name
 *        serviceList:         A list of Services (UUID) to search for
 *      serviceListSize:     Number of services to search for
 *        localServerChannel     The local server Channel
 *----------------------------------------------------------------------------*/
CsrBtCmSdcRfcSearchReq *CsrBtCmSdcRfcSearchReq_struct(CsrSchedQid        appHandle,
                                                      CsrBtDeviceAddr    deviceAddr,
                                                      CsrBtUuid32        * theServiceList,
                                                      CsrUint8        theServiceListSize,
                                                      CsrUint8    theLocalServerChannel)
{
    CsrBtCmSdcRfcSearchReq *prim;

    prim                        = (CsrBtCmSdcRfcSearchReq *) CsrPmemAlloc(sizeof(CsrBtCmSdcRfcSearchReq));
    prim->type                  = CSR_BT_CM_SDC_RFC_SEARCH_REQ;
    prim->appHandle             = appHandle;
    prim->deviceAddr            = deviceAddr;
    prim->serviceList           = theServiceList;
    prim->serviceListSize       = theServiceListSize;
    prim->localServerChannel    = theLocalServerChannel;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcCancelRfcSearchReqSend
 *
 *  DESCRIPTION
 *      Cancel a CSR_BT_CM_SDC_RFC_SEARCH_REQ
 *
 *  PARAMETERS
 *        appHandle:             protocol handle
 *        deviceAddr:             BT address of the device to read remote name
 *----------------------------------------------------------------------------*/
CsrBtCmSdcCancelSearchReq *CsrBtCmSdcCancelRfcSearchReq_struct(CsrSchedQid    appHandle,
                                                               CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmSdcCancelSearchReq *prim;

    prim                        = (CsrBtCmSdcCancelSearchReq *) CsrPmemAlloc(sizeof(CsrBtCmSdcCancelSearchReq));
    prim->type                  = CSR_BT_CM_SDC_CANCEL_SEARCH_REQ;
    prim->appHandle             = appHandle;
    prim->deviceAddr            = deviceAddr;
    prim->typeToCancel          = CSR_BT_CM_SDC_RFC_SEARCH_REQ;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcUuid128RfcSearchReqSend
 *
 *  DESCRIPTION
 *      Submits a 128bit search request to the SDC sub-system
 *
 *  PARAMETERS
 *        appHandle:             protocol handle
 *        deviceAddr:             BT address of the device to read remote name
 *        serviceList:         A list of 128 bit Services (UUID128) to search for
 *      serviceListSize:     Number of services to search for
 *        localServerChannel     The local server Channel
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
CsrBtCmSdcUuid128RfcSearchReq *CsrBtCmSdcUuid128RfcSearchReq_struct(
    CsrSchedQid        appHandle,
    CsrBtDeviceAddr    deviceAddr,
    CsrBtUuid128        * theServiceList,
    CsrUint8            theServiceListSize,
    CsrUint8    theLocalServerChannel)
{
    CsrBtCmSdcUuid128RfcSearchReq * prim;

    prim                        = (CsrBtCmSdcUuid128RfcSearchReq *) CsrPmemAlloc(sizeof(CsrBtCmSdcUuid128RfcSearchReq));
    prim->type                  = CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ;
    prim->appHandle             = appHandle;
    prim->deviceAddr            = deviceAddr;
    prim->serviceList           = theServiceList;
    prim->serviceListSize       = theServiceListSize;
    prim->localServerChannel    = theLocalServerChannel;
    return(prim);
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcCancelUuid128RfcSearchReqSend
 *
 *  DESCRIPTION
 *      Cancel a CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ
 *
 *  PARAMETERS
 *        appHandle:             protocol handle
 *        deviceAddr:             BT address of the device to read remote name
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_128_BIT_SERVICE_SEARCH
CsrBtCmSdcCancelSearchReq *CsrBtCmSdcCancelUuid128RfcSearchReq_struct(CsrSchedQid    appHandle,
                                                                      CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmSdcCancelSearchReq *prim;

    prim                        = (CsrBtCmSdcCancelSearchReq *) CsrPmemAlloc(sizeof(CsrBtCmSdcCancelSearchReq));
    prim->type                  = CSR_BT_CM_SDC_CANCEL_SEARCH_REQ;
    prim->appHandle             = appHandle;
    prim->deviceAddr            = deviceAddr;
    prim->typeToCancel          = CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ;
    return(prim);
}
#endif
/*----------------------------------------------------------------------------*
 *    NAME
 *        CsrBtCmSdcRfcExtendedSearchReqSend
 *
 *    DESCRIPTION
 *        Submits a search request to the SDC sub-system for the ADDITIONAL_PROTOCOL_DESCRIPTOR_LIST
 *
 *  PARAMETERS
 *        appHandle:             protocol handle
 *        deviceAddr:          BT address of the device to read remote name
 *        serviceList:         A list of Services (UUID) to search for
 *        serviceListSize:     Number of services to search for
 *        localServerChannel     The local server Channel
 *----------------------------------------------------------------------------*/
CsrBtCmSdcRfcExtendedSearchReq *CsrBtCmSdcRfcExtendedSearchReq_struct(
    CsrSchedQid      appHandle,
    CsrBtDeviceAddr  deviceAddr,
    CsrBtUuid32          *theServiceList,
    CsrUint8       theServiceListSize,
    CsrUint8 theLocalServerChannel)
{
    CsrBtCmSdcRfcExtendedSearchReq *prim;

    prim                        = (CsrBtCmSdcRfcExtendedSearchReq *) CsrPmemAlloc(sizeof(CsrBtCmSdcRfcExtendedSearchReq));
    prim->type                  = CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ;
    prim->appHandle             = appHandle;
    prim->deviceAddr            = deviceAddr;
    prim->serviceList           = theServiceList;
    prim->serviceListSize       = theServiceListSize;
    prim->localServerChannel    = theLocalServerChannel;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcCancelRfcExtendedSearchReqSend
 *
 *  DESCRIPTION
 *      Cancel a CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ
 *
 *  PARAMETERS
 *        appHandle:             protocol handle
 *        deviceAddr:             BT address of the device to read remote name
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_CANCEL_RFC_EXTENDED_SEARCH
CsrBtCmSdcCancelSearchReq *CsrBtCmSdcCancelRfcExtendedSearchReq_struct(CsrSchedQid    appHandle,
                                                                       CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmSdcCancelSearchReq *prim;

    prim                        = (CsrBtCmSdcCancelSearchReq *) CsrPmemAlloc(sizeof(CsrBtCmSdcCancelSearchReq));
    prim->type                  = CSR_BT_CM_SDC_CANCEL_SEARCH_REQ;
    prim->appHandle             = appHandle;
    prim->deviceAddr            = deviceAddr;
    prim->typeToCancel          = CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ;
    return(prim);
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcServiceSearchReqSend
 *
 *  DESCRIPTION
 *      Submits a search request to the SDC sub-system
 *
 *  PARAMETERS
 *        appHandle:                protocol handle
 *        deviceAddr:                BT address of the device to search for services
 *        uuidSetLength:          The size of uuidSet in bytes
 *      uuidSet:                A search pattern of UUIDs. All of these are
 *                              contained in each returned service record
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_SDC
CsrBtCmSdcServiceSearchReq *CsrBtCmSdcServiceSearchReq_struct(CsrSchedQid    appHandle,
                                                              CsrBtDeviceAddr deviceAddr,
                                                              CsrUint16     theUuidSetLength,
                                                              CsrUint8      *theUuidSet)
{
    CsrBtCmSdcServiceSearchReq *prim;

    prim                        = (CsrBtCmSdcServiceSearchReq *)CsrPmemAlloc(sizeof(CsrBtCmSdcServiceSearchReq));
    prim->type                  = CSR_BT_CM_SDC_SERVICE_SEARCH_REQ;
    prim->appHandle             = appHandle;
    prim->deviceAddr            = deviceAddr;
    prim->uuidSetLength         = theUuidSetLength;
    prim->uuidSet               = theUuidSet;
    return(prim);
}
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcCancelServiceSearchReqSend
 *
 *  DESCRIPTION
 *      Cancel a CSR_BT_CM_SDC_SERVICE_SEARCH_REQ
 *
 *  PARAMETERS
 *        appHandle:             protocol handle
 *        deviceAddr:             BT address of the device to read remote name
 *----------------------------------------------------------------------------*/
CsrBtCmSdcCancelSearchReq *CsrBtCmSdcCancelServiceSearchReq_struct(CsrSchedQid    appHandle,
                                                                   CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmSdcCancelSearchReq *prim;

    prim                        = (CsrBtCmSdcCancelSearchReq *) CsrPmemAlloc(sizeof(CsrBtCmSdcCancelSearchReq));
    prim->type                  = CSR_BT_CM_SDC_CANCEL_SEARCH_REQ;
    prim->appHandle             = appHandle;
    prim->deviceAddr            = deviceAddr;
    prim->typeToCancel          = CSR_BT_CM_SDC_SERVICE_SEARCH_REQ;
    return(prim);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSdcReleaseResourcesReqSend
 *
 *  DESCRIPTION
 *      Releases the SDC search resources, and allow the application with
 *        "apphandle" to perform a new SDC search
 *
 *  PARAMETERS
 *        appHandle:            protocol handle
 *        localServerChannel     The local server Channel
 *----------------------------------------------------------------------------*/
CsrBtCmSdcReleaseResourcesReq *CsrBtCmSdcReleaseResourcesReq_struct(
    CsrSchedQid     appHandle,
    CsrBtDeviceAddr  deviceAddr,
    CsrUint8 theLocalServerChannel)
{
    CsrBtCmSdcReleaseResourcesReq        *prim;

    prim                        = (CsrBtCmSdcReleaseResourcesReq *) CsrPmemAlloc(sizeof(CsrBtCmSdcReleaseResourcesReq));
    prim->type                  = CSR_BT_CM_SDC_RELEASE_RESOURCES_REQ;
    prim->appHandle             = appHandle;
    prim->deviceAddr            = deviceAddr;
    prim->localServerChannel    = theLocalServerChannel;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmCancelAcceptConnectReqSend
 *      CsrBtCmContextCancelAcceptConnectReqSend
 *
 *  DESCRIPTION
 *      ...

 *
 *  PARAMETERS
 *        phandle:                protocol handle
 *        serverChannel:          local server channel
 *        context                 Opaque context number returned only if there
 *                                is one to cancel otherwise the context
 *                                given in CsrBtCmContextConnectAcceptReqSend
 *----------------------------------------------------------------------------*/
CsrBtCmCancelAcceptConnectReq *CsrBtCmCancelAcceptConnectReq_struct(CsrSchedQid          phandle,
                                                                    CsrUint8   serverChannel,
                                                                    CsrUint16       context)
{
    CsrBtCmCancelAcceptConnectReq    *prim;

    prim                  = (CsrBtCmCancelAcceptConnectReq *) CsrPmemAlloc(sizeof(CsrBtCmCancelAcceptConnectReq));
    prim->type            = CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ;
    prim->phandle         = phandle;
    prim->serverChannel   = serverChannel;
    prim->context         = context;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmConnectReqSend
 *      CsrBtCmContextConnectReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        appHandle:            protocol handle
 *        localServerCh:        local server channel
 *      serviceHandle:        The handle of the service
 *        profileMaxFrameSize:maximum frame size (bytes)
 *        requestPortPar:
 *        validportPar:
 *        portPar:            Port parameter typedefs
 *        secLevel:           Level of security to be applied
 *        CsrBtDeviceAddr:    deviceAddr
 *        context             Opaque context number
 *        modemStatus         modem status data
 *        breakSignal         break signal data
 *        mscTimeout          Time in msec that the RFC shall wait for MSC
 *----------------------------------------------------------------------------*/
CsrBtCmConnectReq *CsrBtCmConnectReq_struct(CsrSchedQid                appHandle,
                                            CsrUint8         localServerCh,
                                            CsrBtUuid32           serviceHandle,
                                            CsrUint16             profileMaxFrameSize,
                                            CsrBool               requestPortPar,
                                            CsrBool               validportPar,
                                            RFC_PORTNEG_VALUES_T  portPar,
                                            dm_security_level_t   secLevel,
                                            CsrBtDeviceAddr       deviceAddr,
                                            CsrUint16             context,
                                            CsrUint8            modemStatus,
                                            CsrUint8            breakSignal,
                                            CsrUint8            mscTimeout)
{
    CsrBtCmConnectReq        *prim;

    prim                        = (CsrBtCmConnectReq *) CsrPmemAlloc(sizeof(CsrBtCmConnectReq));
    prim->type                  = CSR_BT_CM_CONNECT_REQ;
    prim->appHandle             = appHandle;
    prim->localServerCh         = localServerCh;
    prim->serviceHandle         = serviceHandle;
    prim->profileMaxFrameSize   = profileMaxFrameSize;
    prim->requestPortPar        = requestPortPar;
    prim->validPortPar          = validportPar;
    if(validportPar == TRUE)
    {
        prim->portPar                = portPar;
    }
    else
    {
        CsrMemSet(&(prim->portPar), 0, sizeof(prim->portPar));
    }
    prim->portPar               = portPar;
    prim->secLevel              = secLevel;
    prim->deviceAddr            = deviceAddr;
    prim->context               = context;
    prim->modemStatus           = modemStatus;
    prim->breakSignal           = breakSignal;
    prim->mscTimeout            = mscTimeout;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmConnectExtReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        appHandle:            protocol handle
 *        localServerCh:        local server channel
 *      remoteServerCh:     remote server channel
 *        profileMaxFrameSize:maximum frame size (bytes)
 *        requestPortPar:
 *        validportPar:
 *        portPar:            Port parameter typedefs
 *        secLevel:           Level of security to be applied
 *        CsrBtDeviceAddr:        deviceAddr
 *        modemStatus         modem status data
 *        breakSignal         break signal data
 *        mscTimeout          Time in msec that the RFC shall wait for MSC
*----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_CONNECT_EXT
CsrBtCmConnectExtReq *CsrBtCmConnectExtReq_struct(   CsrSchedQid           appHandle,
                                                     CsrUint8       theLocalServerChannel,
                                                     CsrUint8       theRemoteServerChannel,
                                                     CsrUint16            theProfileMaxFrameSize,
                                                     CsrBool              theRequestPortPar,
                                                     CsrBool              theValidPortPar,
                                                     RFC_PORTNEG_VALUES_T thePortPar,
                                                     dm_security_level_t theSecLevel,
                                                     CsrBtDeviceAddr        deviceAddr,
                                                     CsrUint8            modemStatus,
                                                     CsrUint8            breakSignal,
                                                     CsrUint8            mscTimeout)
{
    CsrBtCmConnectExtReq    *msg;

    msg                         = (CsrBtCmConnectExtReq *) CsrPmemAlloc(sizeof(CsrBtCmConnectExtReq));
    msg->type                   = CSR_BT_CM_CONNECT_EXT_REQ;
    msg->appHandle              = appHandle;
    msg->localServerCh          = theLocalServerChannel;
    msg->remoteServerCh         = theRemoteServerChannel;
    msg->profileMaxFrameSize    = theProfileMaxFrameSize;
    msg->requestPortPar         = theRequestPortPar;
    msg->validPortPar           = theValidPortPar;
    if(theValidPortPar == TRUE)
    {
        msg->portPar                = thePortPar;
    }
    else
    {
        CsrMemSet(&(msg->portPar), 0, sizeof(msg->portPar));
    }
    msg->secLevel               = theSecLevel;
    msg->deviceAddr             = deviceAddr;
    msg->modemStatus            = modemStatus;
    msg->breakSignal            = breakSignal;
    msg->mscTimeout             = mscTimeout;
    return(msg);
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmCancelConnectReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        appHandle:            protocol handle
 *        localServerCh:        local server channel
 *        CsrBtDeviceAddr:        deviceAddr
 *----------------------------------------------------------------------------*/
CsrBtCmCancelConnectReq *CsrBtCmCancelConnectReq_struct(CsrSchedQid     appHandle,
                                                        CsrUint8 theLocalServerChannel,
                                                        CsrBtDeviceAddr  deviceAddr)
{
    CsrBtCmCancelConnectReq    *msg;

    msg                         = (CsrBtCmCancelConnectReq *) CsrPmemAlloc(sizeof(CsrBtCmCancelConnectReq));
    msg->type                   = CSR_BT_CM_CANCEL_CONNECT_REQ;
    msg->appHandle              = appHandle;
    msg->localServerCh          = theLocalServerChannel;
    msg->deviceAddr             = deviceAddr;
    msg->typeToCancel           = CSR_BT_CM_CONNECT_REQ;
    return(msg);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmCancelConnectExtReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        appHandle:            protocol handle
 *        localServerCh:        local server channel
 *        CsrBtDeviceAddr:        deviceAddr
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_CONNECT_EXT
CsrBtCmCancelConnectReq *CsrBtCmCancelConnectExtReq_struct(CsrSchedQid      appHandle,
                                                           CsrUint8  theLocalServerChannel,
                                                           CsrBtDeviceAddr   deviceAddr)
{
    CsrBtCmCancelConnectReq    *msg;

    msg                         = (CsrBtCmCancelConnectReq *) CsrPmemAlloc(sizeof(CsrBtCmCancelConnectReq));
    msg->type                   = CSR_BT_CM_CANCEL_CONNECT_REQ;
    msg->appHandle              = appHandle;
    msg->localServerCh          = theLocalServerChannel;
    msg->deviceAddr             = deviceAddr;
    msg->typeToCancel           = CSR_BT_CM_CONNECT_EXT_REQ;
    return(msg);
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmConnectAcceptReqSend
 *      CsrBtCmContextConnectAcceptReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        appHandle:            protocol handle
 *        classOfDevice:        The Class Of Device of the profile
 *        timeout:            connection establish time out timer (range: secs)
 *        profileMaxFrameSize:maximum frame size (bytes)
 *        serverChannel:        local server channel
 *        secLevel:           Level of security to be applied
 *      profileUuid:        The local profile Uuid
 *        context             Opaque context number
 *        modemStatus         modem status data
 *        breakSignal         break signal data
 *        mscTimeout          Time in msec that the RFC shall wait for MSC
 *----------------------------------------------------------------------------*/
CsrBtCmConnectAcceptReq *CsrBtCmConnectAcceptReq_struct(CsrSchedQid              appHandle,
                                                        CsrUint24           classOfDevice,
                                                        CsrUint16           timeout,
                                                        CsrUint16           profileMaxFrameSize,
                                                        CsrUint8       serverChannel,
                                                        dm_security_level_t secLevel,
                                                        uuid16_t            profileUuid,
                                                        CsrUint16           context,
                                                        CsrUint8            modemStatus,
                                                        CsrUint8            breakSignal,
                                                        CsrUint8            mscTimeout)
{
    CsrBtCmConnectAcceptReq        *prim;

    prim                        = (CsrBtCmConnectAcceptReq *) CsrPmemAlloc(sizeof(CsrBtCmConnectAcceptReq));
    prim->type                  = CSR_BT_CM_CONNECT_ACCEPT_REQ;
    prim->timeout               = timeout;
    prim->profileMaxFrameSize   = profileMaxFrameSize;
    prim->serverChannel         = serverChannel;
    prim->profileUuid           = profileUuid;
    prim->secLevel              = secLevel;
    prim->appHandle             = appHandle;
    prim->classOfDevice         = classOfDevice;
    prim->context               = context;
    prim->modemStatus           = modemStatus;
    prim->breakSignal           = breakSignal;
    prim->mscTimeout            = mscTimeout;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmRegisterReqSend
 *      CsrBtCmContextRegisterReqSend
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *        phandle:            protocol handle
 *        context             Opaque context number returned in CsrBtCmRegisterCfm
 *        serverChannel       server channel asked for
 *----------------------------------------------------------------------------*/
CsrBtCmRegisterReq *CsrBtCmRegisterReq_struct(CsrSchedQid phandle, CsrUint16 context, CsrUint8 serverChannel)
{
    CsrBtCmRegisterReq        *prim;

    prim                        = (CsrBtCmRegisterReq *) CsrPmemAlloc(sizeof(CsrBtCmRegisterReq));
    prim->type                  = CSR_BT_CM_REGISTER_REQ;
    prim->phandle               = phandle;
    prim->context               = context;
    prim->serverChannel         = serverChannel;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmUnRegisterReqSend
 *
 *  DESCRIPTION
 *      ....
 *
 *  PARAMETERS
 *        serverChannel:        local server channel to unregister
 *----------------------------------------------------------------------------*/
void CsrBtCmUnRegisterReqSend(CsrUint8    theSErverChannel)
{
    CsrBtCmUnregisterReq        *prim;

    prim                    = (CsrBtCmUnregisterReq *) CsrPmemAlloc(sizeof(CsrBtCmUnregisterReq));
    prim->type              = CSR_BT_CM_UNREGISTER_REQ;
    prim->serverChannel     = theSErverChannel;
    CsrBtCmPutMessageDownstream(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmDisconnectReqSend
 *      CsrBtCmContextDisconnectReqSend
 *
 *  DESCRIPTION
 *      ......
 *
 *  PARAMETERS
 *        btConnId              connection Id
 *        context               Opaque context number returned only if no
 *                              connection is present otherwise the context
 *                              given in CsrBtCmExtConnectAcceptReqSend or
 *                              CsrBtCmContextConnectReqSend is return
 *----------------------------------------------------------------------------*/
CsrBtCmDisconnectReq *CsrBtCmDisconnectReq_struct(CsrBtConnId    btConnId,
                                                  CsrUint16      context)
{
    CsrBtCmDisconnectReq        *prim;

    prim                    = (CsrBtCmDisconnectReq *) CsrPmemAlloc(sizeof(CsrBtCmDisconnectReq));
    prim->type              = CSR_BT_CM_DISCONNECT_REQ;
    prim->btConnId          = btConnId;
    prim->context           = context;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmScoConnectReqSend
 *
 *  DESCRIPTION
 *      ......
 *
 *  PARAMETERS
 *      pcmSlot
 *      pcmReassign
 *      parms
 *      parmsLen
 *      btConnId
 *----------------------------------------------------------------------------*/
CsrBtCmScoConnectReq *CsrBtCmScoConnectReq_struct(CsrSchedQid            appHandle,
                                                  CsrUint8         pcmSlot,
                                                  CsrBool          pcmReassign,
                                                  CsrBtCmScoCommonParms       *parms,
                                                  CsrUint16        parmsLen,
                                                  CsrBtConnId     btConnId)
{
    CsrBtCmScoConnectReq    *prim;

    prim                    = (CsrBtCmScoConnectReq *)CsrPmemAlloc(sizeof(CsrBtCmScoConnectReq));
    prim->type              = CSR_BT_CM_SCO_CONNECT_REQ;
    prim->appHandle         = appHandle;
    prim->pcmSlot           = pcmSlot;
    prim->pcmReassign       = pcmReassign;
    prim->parms             = parms;
    prim->parmsLen          = parmsLen;
    prim->btConnId          = btConnId;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmScoAcceptConnectReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        btConnId
 *        audioQuality:
 *      txBandwidth
 *      rxBandwidth
 *      maxLatency
 *      voiceSettings
 *      reTxEffort
 *----------------------------------------------------------------------------*/
CsrBtCmScoAcceptConnectReq *CsrBtCmScoAcceptConnectReq_struct(CsrSchedQid     appHandle,
                                                              CsrBtConnId     btConnId,
                                                              hci_pkt_type_t  theAUdioQuality,
                                                              CsrUint32       theTXBandwidth,
                                                              CsrUint32       theRXBandwidth,
                                                              CsrUint16       theMAxLatency,
                                                              CsrUint16       theVOiceSettings,
                                                              CsrUint8        theRETxEffort)
{
    CsrBtCmScoAcceptConnectReq      *prim;

    prim                    = (CsrBtCmScoAcceptConnectReq *)CsrPmemAlloc(sizeof(CsrBtCmScoAcceptConnectReq));
    prim->type              = CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ;
    prim->appHandle         = appHandle;
    prim->btConnId          = btConnId;
    prim->audioQuality      = theAUdioQuality;
    prim->txBandwidth       = theTXBandwidth;
    prim->rxBandwidth       = theRXBandwidth;
    prim->maxLatency        = theMAxLatency;
    prim->voiceSettings     = theVOiceSettings;
    prim->reTxEffort        = theRETxEffort;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmMapScoPcmResSend
 *
 *  DESCRIPTION
 *      ......
 *
 *  PARAMETERS
 *        btConnId
 *----------------------------------------------------------------------------*/
CsrBtCmMapScoPcmRes *CsrBtCmMapScoPcmRes_struct(CsrBtConnId             btConnId,
                                                hci_error_t             acceptResponse,
                                                CsrBtCmScoCommonParms   *parms,
                                                CsrUint8                 thePcmSlot,
                                                CsrBool                  thePcmReassign)
{
    CsrBtCmMapScoPcmRes    *prim;

    prim                    = (CsrBtCmMapScoPcmRes *)CsrPmemAlloc(sizeof(CsrBtCmMapScoPcmRes));
    prim->type              = CSR_BT_CM_MAP_SCO_PCM_RES;
    prim->btConnId          = btConnId;
    prim->acceptResponse    = acceptResponse;
    prim->parms             = parms;
    prim->parmsLen          = parms ? 1 : 0;
    prim->pcmSlot           = thePcmSlot;
    prim->pcmReassign       = thePcmReassign;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtcmScoRenegotiateReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        btConnId
 *        audioQuality:
 *      maxLatency
 *      reTxEffort
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_SCO_RENEGOTIATE
CsrBtCmScoRenegotiateReq *CsrBtCmScoRenegotiateReq_struct(CsrSchedQid     appHandle,
                                                          CsrBtConnId     btConnId,
                                                          hci_pkt_type_t  theAUdioQuality,
                                                          CsrUint16       theMAxLatency,
                                                          CsrUint8        theRETxEffort)
{
    CsrBtCmScoRenegotiateReq  * prim;

    prim                    = (CsrBtCmScoRenegotiateReq *)CsrPmemAlloc(sizeof(CsrBtCmScoRenegotiateReq));
    prim->type              = CSR_BT_CM_SCO_RENEGOTIATE_REQ;
    prim->appHandle         = appHandle;
    prim->btConnId          = btConnId;
    prim->audioQuality      = theAUdioQuality;
    prim->maxLatency        = theMAxLatency;
    prim->reTxEffort        = theRETxEffort;
    return(prim);
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmScoDisconnectReqSend
 *
 *  DESCRIPTION
 *      ......
 *
 *  PARAMETERS
 *        btConnId
 *----------------------------------------------------------------------------*/
CsrBtCmScoDisconnectReq *CsrBtCmScoDisconnectReq_struct(CsrSchedQid        appHandle,
                                                        CsrBtConnId        btConnId)
{
    CsrBtCmScoDisconnectReq        *prim;

    prim                    = (CsrBtCmScoDisconnectReq *)CsrPmemAlloc(sizeof(CsrBtCmScoDisconnectReq));
    prim->type              = CSR_BT_CM_SCO_DISCONNECT_REQ;
    prim->appHandle         = appHandle;
    prim->btConnId          = btConnId;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmScoCancelReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        btConnId
 *----------------------------------------------------------------------------*/
CsrBtCmScoCancelAcceptConnectReq *CsrBtCmScoCancelReq_struct(CsrSchedQid         appHandle,
                                                             CsrBtConnId         btConnId)
{
    CsrBtCmScoCancelAcceptConnectReq        *prim;

    prim                    = (CsrBtCmScoCancelAcceptConnectReq *)CsrPmemAlloc(sizeof(CsrBtCmScoCancelAcceptConnectReq));
    prim->type              = CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ;
    prim->appHandle         = appHandle;
    prim->btConnId          = btConnId;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmDataReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        btConnId
 *        payloadLength:        length of the payload
 *        *payload:            pointer to the data
 *----------------------------------------------------------------------------*/
CsrBtCmDataReq *CsrBtCmDataReq_struct(CsrBtConnId      btConnId,
                                      CsrUint16        thePAyloadLength,
                                      CsrUint8        *thePAyload )
{
    CsrBtCmDataReq            *prim;

    prim                    = (CsrBtCmDataReq *)CsrPmemAlloc(sizeof(CsrBtCmDataReq));
    prim->type              = CSR_BT_CM_DATA_REQ;
    prim->btConnId          = btConnId;
    prim->payloadLength     = thePAyloadLength;
    prim->payload           = thePAyload;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmDataResSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        btConnId
 *----------------------------------------------------------------------------*/
CsrBtCmDataRes *CsrBtCmDataRes_struct(CsrBtConnId     btConnId)
{
    CsrBtCmDataRes            *prim;

    prim                    = (CsrBtCmDataRes *) CsrPmemAlloc(sizeof(CsrBtCmDataRes));
    prim->type              = CSR_BT_CM_DATA_RES;
    prim->btConnId          = btConnId;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmControlReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        btConnId
 *        modemstatus:        modemstatus
 *        break_signal        break signal (7 LSBs)
 *----------------------------------------------------------------------------*/
CsrBtCmControlReq *CsrBtCmControlReq_struct(CsrBtConnId     btConnId,
                                            CsrUint8        theMOdemstatus,
                                            CsrUint8        theBReakSignal)
{
    CsrBtCmControlReq        *prim;

    prim                    = (CsrBtCmControlReq *) CsrPmemAlloc(sizeof(CsrBtCmControlReq));
    prim->type              = CSR_BT_CM_CONTROL_REQ;
    prim->btConnId          = btConnId;
    prim->modemstatus       = theMOdemstatus;
    prim->break_signal      = theBReakSignal;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmRfcModeChangeReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        btConnId
 *        theMode:            Requested link policy mode
 *----------------------------------------------------------------------------*/
CsrBtCmRfcModeChangeReq *CsrBtCmRfcModeChangeReq_struct(CsrBtConnId btConnId,
                                                        CsrUint8 theMode,
                                                        CsrBool forceSniff)
{
    CsrBtCmRfcModeChangeReq *prim;

    prim                    = (CsrBtCmRfcModeChangeReq *) CsrPmemAlloc(sizeof(CsrBtCmRfcModeChangeReq));
    prim->type              = CSR_BT_CM_RFC_MODE_CHANGE_REQ;
    prim->btConnId          = btConnId;
    prim->requestedMode     = theMode;
    prim->forceSniff        = forceSniff;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmDmModeSettingsReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_MODE_SETTINGS
CsrBtCmDmModeSettingsReq *CsrBtCmDmModeSettingsReq_struct(CsrBtConnId                    btConnId,
                                                          CsrBtSniffSettings            *theSniffSettings,
                                                          CsrBtParkSettings             *theParkSettings,
                                                          CsrBtSsrSettingsDownstream    *theSsrSettings,
                                                          CsrUint8                       lowPowerPriority)
{
    CsrBtCmDmModeSettingsReq *prim;

    prim                    = (CsrBtCmDmModeSettingsReq *) CsrPmemAlloc(sizeof(CsrBtCmDmModeSettingsReq));
    prim->type              = CSR_BT_CM_DM_MODE_SETTINGS_REQ;
    prim->btConnId          = btConnId;

    if (theParkSettings)
    {
        prim->parkSettings          = theParkSettings;
        prim->parkSettingsSize      = 1;
    }
    else
    {
        prim->parkSettings          = NULL;
        prim->parkSettingsSize      = 0;
    }

    if (theSniffSettings)
    {
        prim->sniffSettings         = theSniffSettings;
        prim->sniffSettingsSize     = 1;
    }
    else
    {
        prim->sniffSettings         = NULL;
        prim->sniffSettingsSize     = 0;
    }

    if (theSsrSettings)
    {
        prim->ssrSettings         = theSsrSettings;
        prim->ssrSettingsSize     = 1;
    }
    else
    {
        prim->ssrSettings         = NULL;
        prim->ssrSettingsSize     = 0;
    }
    prim->lowPowerPriority        = lowPowerPriority;
    return(prim);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmPortnegResSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        btConnId
 *        PortVar:            pmalloc pointer to Port parameter
 *----------------------------------------------------------------------------*/
CsrBtCmPortnegRes *CsrBtCmPortnegRes_struct(CsrBtConnId           btConnId,
                                            RFC_PORTNEG_VALUES_T *thePOrtVar)
{
    CsrBtCmPortnegRes        *prim;

    prim                    = (CsrBtCmPortnegRes *) CsrPmemAlloc(sizeof(CsrBtCmPortnegRes));
    prim->type              = CSR_BT_CM_PORTNEG_RES;
    prim->btConnId          = btConnId;
    prim->portPar           = *thePOrtVar;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmPortnegReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        btConnId              connection ID
 *        PortVar:              Port parameter
 *        context:              Opaque context number returned only if there
 *                              is one to negotiateport parameters on otherwise
 *                              the context CsrBtCmPortnegReqSend
 *----------------------------------------------------------------------------*/
CsrBtCmPortnegReq *CsrBtCmPortnegReq_struct(CsrBtConnId btConnId,
                                            RFC_PORTNEG_VALUES_T *thePortVar,
                                            CsrUint16 context)
{
    CsrBtCmPortnegReq        *prim;

    prim                    = (CsrBtCmPortnegReq *) CsrPmemAlloc(sizeof(CsrBtCmPortnegReq));
    prim->type              = CSR_BT_CM_PORTNEG_REQ;
    prim->btConnId          = btConnId;
    prim->portPar           = *thePortVar;
    prim->context           = context;

    return(prim);
}

#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCml2caRegisterReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        appHandle:           Protocol handle
 *        localPsm:            The local PSM channel
 *        mode_mask:           L2CAP flow control modes bitmask
 *        flags:               Special L2CAP registration flags
 *----------------------------------------------------------------------------*/
CsrBtCmL2caRegisterReq *CsrBtCml2caRegisterReq_struct(CsrSchedQid appHandle, psm_t theLocalPsm,
                                                      CsrUint16 mode_mask, CsrUint16 flags, 
                                                      CsrUint16 context)
{
    CsrBtCmL2caRegisterReq    *prim;

    prim                        = (CsrBtCmL2caRegisterReq *) CsrPmemAlloc(sizeof(CsrBtCmL2caRegisterReq));
    prim->type                  = CSR_BT_CM_L2CA_REGISTER_REQ;
    prim->phandle               = appHandle;
    prim->localPsm              = theLocalPsm;
    prim->mode_mask             = mode_mask;
    prim->flags                 = flags;
    prim->context               = context;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCml2caUnRegisterReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        appHandle:           Protocol handle
 *        localPsm:            The local PSM channel
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_L2CA_UNREGISTER
CsrBtCmL2caUnregisterReq *CsrBtCml2caUnRegisterReq_struct(CsrSchedQid appHandle, psm_t theLocalPsm)
{
    CsrBtCmL2caUnregisterReq    *prim;

    prim                        = (CsrBtCmL2caUnregisterReq *) CsrPmemAlloc(sizeof(CsrBtCmL2caUnregisterReq));
    prim->type                  = CSR_BT_CM_L2CA_UNREGISTER_REQ;
    prim->phandle               = appHandle;
    prim->localPsm              = theLocalPsm;
    return(prim);
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCml2caCancelConnectReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      appHandle:           protocol handle
 *      CsrBtDeviceAddr:        deviceAddr
 *      localPsm:            The local PSM channel
 *----------------------------------------------------------------------------*/
CsrBtCmCancelL2caConnectReq *CsrBtCml2caCancelConnectReq_struct(CsrSchedQid    appHandle,
                                                                CsrBtDeviceAddr deviceAddr,
                                                                psm_t        theLocalPsm)
{
    CsrBtCmCancelL2caConnectReq *prim;

    prim                    = (CsrBtCmCancelL2caConnectReq *) CsrPmemAlloc(sizeof(CsrBtCmCancelL2caConnectReq));
    prim->type              = CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ;
    prim->phandle           = appHandle;
    prim->deviceAddr        = deviceAddr;
    prim->localPsm          = theLocalPsm;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCml2caConnectReqSend
 *      CsrBtCmJsr82l2caConnectReqSend
 *      CsrBtCml2caConnectHighDataPriorityReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        appHandle:          Protocol handle
 *        deviceAddr:         BT address of device to bonded with
 *        localPsm:           The local PSM channel
 *        remotePsm:          Remote PSM channel to connect to
 *        framesize:          Maximum frame size (bytes) (outgoing MTU)
 *        secLevel:           Level of security to be applied
 *----------------------------------------------------------------------------*/
CsrBtCmL2caConnectReq *CsrBtCml2caConnectReq_struct(CsrSchedQid         appHandle,
                                                    CsrBtDeviceAddr     deviceAddr,
                                                    psm_t               theLocalPsm,
                                                    psm_t               theRemotePsm,
                                                    l2ca_mtu_t          theFramesize,
                                                    dm_security_level_t theSecLevel,
                                                    l2ca_mtu_t          transmitMtu,
                                                    CsrUint16           dataPriority)
{
    CsrBtCmL2caConnectReq *prim;
    CsrUint16 *conftab;
    CsrUint16 idx;

    prim            = (CsrBtCmL2caConnectReq *) CsrPmemAlloc(sizeof(CsrBtCmL2caConnectReq));
    prim->type      = CSR_BT_CM_L2CA_CONNECT_REQ;
    prim->addr      = deviceAddr;
    prim->phandle   = appHandle;
    prim->localPsm  = theLocalPsm;
    prim->remotePsm = theRemotePsm;
    prim->secLevel  = theSecLevel;
    prim->context   = CSR_BT_CM_CONTEXT_UNUSED; 
    conftab = NULL;
    idx = 0;
    CsrBtCmL2caConftabInit(&conftab, &idx);
    CsrBtCmL2caConftabFlushToAllowAnyPeer(conftab, &idx);
    CsrBtCmL2caConftabMtu(conftab, &idx, TRUE, theFramesize); /* incoming */
    CsrBtCmL2caConftabMtu(conftab, &idx, TRUE, transmitMtu);  /* outgoing */
    CsrBtCmL2caConftabHighDataPriority(conftab, &idx, dataPriority);
    CsrBtCmL2caConftabNoReconf(conftab, &idx);
    CsrBtCmL2caConftabFinalize(conftab, &idx);
    CsrBtCmL2caConftabCull(&conftab, &idx);

    prim->conftab = conftab;
    prim->conftabCount = idx;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCml2caConnectReqSendExt
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        appHandle:            Protocol handle
 *        deviceAddr:            BT address of device to bonded with
 *        localPsm:            The local PSM channel
 *        remotePsm:            Remote PSM channel to connect to
 *        framesize:            Maximum frame size (bytes)
 *        flushTimeout:        Flush timeout value
 *        serviceQuality:        QoS flow parameters
 *        secLevel:           Level of security to be applied
 *----------------------------------------------------------------------------*/
CsrBtCmL2caConnectReq *CsrBtCml2caConnectReqSendExt_struct(CsrSchedQid              appHandle,
                                                           CsrBtDeviceAddr     deviceAddr,
                                                           psm_t               localPsm,
                                                           psm_t               remotePsm,
                                                           l2ca_mtu_t          framesize,
                                                           l2ca_mtu_t          transmitMtu,
                                                           CsrUint16           flushTimeout,
                                                           L2CA_QOS_T         *serviceQuality,
                                                           L2CA_FLOW_T        *flow,
                                                           CsrBool             fallbackBasicMode,
                                                           dm_security_level_t secLevel)
{
    CsrBtCmL2caConnectReq *prim;
    CsrUint16 *conftab;
    CsrUint16 idx;

    prim            = (CsrBtCmL2caConnectReq *) CsrPmemAlloc(sizeof(CsrBtCmL2caConnectReq));
    prim->type      = CSR_BT_CM_L2CA_CONNECT_REQ;
    prim->addr      = deviceAddr;
    prim->phandle   = appHandle;
    prim->localPsm  = localPsm;
    prim->remotePsm = remotePsm;
    prim->secLevel  = secLevel;
    prim->context   = CSR_BT_CM_CONTEXT_UNUSED;

    conftab = NULL;
    idx = 0;
    CsrBtCmL2caConftabInit(&conftab, &idx);
    CsrBtCmL2caConftabFlushToAllowAnyPeer(conftab, &idx);
    CsrBtCmL2caConftabFlushTo(conftab, &idx, FALSE, flushTimeout); /* outgoing */
    CsrBtCmL2caConftabMtu(conftab, &idx, TRUE, framesize); /* incoming */
    CsrBtCmL2caConftabMtu(conftab, &idx, TRUE, transmitMtu);  /* outgoing */

    if(flow != NULL)
    {
        CsrBtCmL2caConftabFlow(conftab, &idx, flow, fallbackBasicMode);
        CsrPmemFree(flow);
    }

    /* QoS isn't supported */
    CsrPmemFree(serviceQuality);
    CsrBtCmL2caConftabNoReconf(conftab, &idx);
    CsrBtCmL2caConftabFinalize(conftab, &idx);
    CsrBtCmL2caConftabCull(&conftab, &idx);

    prim->conftab = conftab;
    prim->conftabCount = idx;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCml2caConnectAcceptReqSend
 *      CsrBtCmJsr82l2caConnectAcceptReqSend
 *      CsrBtCml2caConnectAcceptSecondaryReqSend
 *      CsrBtCml2caConnectAcceptHighDataPriorityReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      appHandle:          Protocol handle
 *      localPsm:           The local PSM channel
 *      classOfDevice:      The profile COD
 *      secLevel:           Level of security to be applied
 *      framesize:          Maximum frame size (bytes)
 *      profileUuid:        The local profile Uuid
 *      primaryAcceptor     If TRUE it makes the host controller connectable,
 *                          e.g write the cod value and set it in page scan.
 *                          If FALSE the host controller is not set connecable
 *                          and the SM queue is not used. E.g this message is
 *                          handle at once by the CM.
 *----------------------------------------------------------------------------*/
CsrBtCmL2caConnectAcceptReq *CsrBtCml2caConnectAcceptReq_struct(CsrSchedQid          appHandle,
                                                                psm_t               theLocalPsm,
                                                                CsrUint24           theClassOfDevice,
                                                                dm_security_level_t theSecLevel,
                                                                l2ca_mtu_t          theFramesize,
                                                                uuid16_t            theProfileUuid,
                                                                l2ca_mtu_t          transmitMtu,
                                                                CsrBool             primaryAcceptor,
                                                                CsrUint16           dataPriority)
{
    CsrBtCmL2caConnectAcceptReq *prim;
    CsrUint16 *conftab;
    CsrUint16 idx;

    prim                    = (CsrBtCmL2caConnectAcceptReq *) CsrPmemAlloc(sizeof(CsrBtCmL2caConnectAcceptReq));
    prim->type              = CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ;
    prim->phandle           = appHandle;
    prim->localPsm          = theLocalPsm;
    prim->classOfDevice     = theClassOfDevice;
    prim->secLevel          = theSecLevel;
    prim->profileUuid       = theProfileUuid;
    prim->primaryAcceptor   = primaryAcceptor;
    prim->context           = CSR_BT_CM_CONTEXT_UNUSED;

    conftab = NULL;
    idx = 0;
    CsrBtCmL2caConftabInit(&conftab, &idx);
    CsrBtCmL2caConftabFlushToAllowAnyPeer(conftab, &idx);
    CsrBtCmL2caConftabMtu(conftab, &idx, TRUE, theFramesize); /* incoming */
    CsrBtCmL2caConftabMtu(conftab, &idx, TRUE, transmitMtu);  /* outgoing */
    CsrBtCmL2caConftabHighDataPriority(conftab, &idx, dataPriority);
    CsrBtCmL2caConftabNoReconf(conftab, &idx);
    CsrBtCmL2caConftabFinalize(conftab, &idx);
    CsrBtCmL2caConftabCull(&conftab, &idx);

    prim->conftab = conftab;
    prim->conftabCount = idx;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCml2caConnectAcceptReqSendExt
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      appHandle:      Protocol handle
 *      localPsm:       The local PSM channel
 *      classOfDevice:  The profile COD
 *      secLevel:       Level of security to be applied
 *      framesize:      Maximum frame size (bytes)
 *      flushTimeout:   Flush timeout value
 *      serviceQuality: QoS flow parameters
 *      profileUuid:    The local profile Uuid
 *      primaryAcceptor:If TRUE it makes the host controller connectable,
 *                      e.g write the cod value and set it in page scan.
 *                      If FALSE the host controller is not set connecable
 *                      and the SM queue is not used. E.g this message is
 *                      handle at once by the CM.
 *----------------------------------------------------------------------------*/
CsrBtCmL2caConnectAcceptReq *CsrBtCml2caConnectAcceptReqSendExt_struct(CsrSchedQid              appHandle,
                                                                       psm_t               localPsm,
                                                                       CsrUint24           classOfDevice,
                                                                       dm_security_level_t secLevel,
                                                                       l2ca_mtu_t          framesize,
                                                                       CsrUint16           flushTimeout,
                                                                       L2CA_QOS_T         *serviceQuality,
                                                                       L2CA_FLOW_T        *flow,
                                                                       CsrBool             fallbackBasicMode,
                                                                       uuid16_t            profileUuid,
                                                                       CsrBool             primaryAcceptor)
{
    CsrBtCmL2caConnectAcceptReq *prim;
    CsrUint16 *conftab;
    CsrUint16 idx;

    prim                    = (CsrBtCmL2caConnectAcceptReq *) CsrPmemAlloc(sizeof(CsrBtCmL2caConnectAcceptReq));
    prim->type              = CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ;
    prim->phandle           = appHandle;
    prim->localPsm          = localPsm;
    prim->classOfDevice     = classOfDevice;
    prim->secLevel          = secLevel;
    prim->primaryAcceptor   = primaryAcceptor;
    prim->profileUuid       = profileUuid;
    prim->context           = CSR_BT_CM_CONTEXT_UNUSED;

    conftab = NULL;
    idx = 0;
    CsrBtCmL2caConftabInit(&conftab, &idx);
    CsrBtCmL2caConftabFlushToAllowAnyPeer(conftab, &idx);
    CsrBtCmL2caConftabFlushTo(conftab, &idx, FALSE, flushTimeout); /* outgoing */
    CsrBtCmL2caConftabMtu(conftab, &idx, TRUE, framesize); /* incoming */

    if(flow != NULL)
    {
        CsrBtCmL2caConftabFlow(conftab, &idx, flow, fallbackBasicMode);
        CsrPmemFree(flow);
    }

    /* QoS isn't supported */
    CsrPmemFree(serviceQuality);

    CsrBtCmL2caConftabNoReconf(conftab, &idx);
    CsrBtCmL2caConftabFinalize(conftab, &idx);
    CsrBtCmL2caConftabCull(&conftab, &idx);

    prim->conftab = conftab;
    prim->conftabCount = idx;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmL2caConnectReqConftabSend
 *
 *  DESCRIPTION
 *      CM L2CAP connect request using conftabs
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_L2CA_CONNECT_CONFTAB
CsrBtCmL2caConnectReq *CsrBtCmL2caConnectReqConftab_struct(CsrSchedQid appHandle,
                                                           CsrBtDeviceAddr deviceAddr,
                                                           psm_t localPsm,
                                                           psm_t remotePsm,
                                                           dm_security_level_t secLevel,
                                                           CsrUint16 context,        
                                                           CsrUint16 conftabCount,
                                                           CsrUint16 *conftab)
{
    CsrBtCmL2caConnectReq *prim;

    prim            = (CsrBtCmL2caConnectReq *) CsrPmemAlloc(sizeof(CsrBtCmL2caConnectReq));
    prim->type      = CSR_BT_CM_L2CA_CONNECT_REQ;
    prim->addr      = deviceAddr;
    prim->phandle   = appHandle;
    prim->localPsm  = localPsm;
    prim->remotePsm = remotePsm;
    prim->secLevel  = secLevel;
    prim->context   = context;
    prim->conftab   = conftab;
    prim->conftabCount = conftabCount;

    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmL2caConnectAcceptReqConftabSend
 *
 *  DESCRIPTION
 *      CM L2CAP connect accept request using conftabs
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
CsrBtCmL2caConnectAcceptReq *CsrBtCmL2caConnectAcceptReqConftab_struct(CsrSchedQid appHandle,
                                                                       psm_t localPsm,
                                                                       CsrUint24 cod,
                                                                       dm_security_level_t secLevel,
                                                                       uuid16_t uuid,
                                                                       CsrBool primaryAcceptor,
                                                                       CsrUint16 context,
                                                                       CsrUint16 conftabCount,
                                                                       CsrUint16 *conftab)
{
    CsrBtCmL2caConnectAcceptReq *prim;

    prim                    = (CsrBtCmL2caConnectAcceptReq *) CsrPmemAlloc(sizeof(CsrBtCmL2caConnectAcceptReq));
    prim->type              = CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ;
    prim->phandle           = appHandle;
    prim->localPsm          = localPsm;
    prim->classOfDevice     = cod;
    prim->secLevel          = secLevel;
    prim->profileUuid       = uuid;
    prim->primaryAcceptor   = primaryAcceptor;
    prim->context           = context;
    prim->conftab           = conftab;
    prim->conftabCount      = conftabCount;

    return prim;
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCml2caCancelConnectAcceptReqSend
 *      CsrBtCmContextl2caCancelConnectAcceptReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        appHandle:            Protocol handle
 *        localPsm:            The local PSM channel
 *----------------------------------------------------------------------------*/
CsrBtCmL2caCancelConnectAcceptReq *CsrBtCml2caCancelConnectAcceptReq_struct(CsrSchedQid    appHandle,
                                                                            psm_t        theLocalPsm,
                                                                            CsrUint16    context)
{
    CsrBtCmL2caCancelConnectAcceptReq        *prim;

    prim                    = (CsrBtCmL2caCancelConnectAcceptReq *) CsrPmemAlloc(sizeof(CsrBtCmL2caCancelConnectAcceptReq));
    prim->type              = CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ;
    prim->phandle           = appHandle;
    prim->localPsm          = theLocalPsm;
    prim->context           = context;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCml2caDisconnectReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        btConnId            :            Channel identifier
 *----------------------------------------------------------------------------*/
CsrBtCmL2caDisconnectReq *CsrBtCml2caDisconnectReq_struct(CsrBtConnId btConnId,
                                                          CsrUint16   context)
{
    CsrBtCmL2caDisconnectReq        *prim;

    prim                    = (CsrBtCmL2caDisconnectReq *) CsrPmemAlloc(sizeof(CsrBtCmL2caDisconnectReq));
    prim->type              = CSR_BT_CM_L2CA_DISCONNECT_REQ;
    prim->btConnId          = btConnId;
    prim->context           = context;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCml2caDataReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        btConnId:                Channel identifier
 *        payloadLength:        Length of the payload
 *        *payload:            Pointer to the data
 *        context:             a context value (passed back in the confirm)
 *----------------------------------------------------------------------------*/
CsrBtCmL2caDataReq *CsrBtCml2caDataReq_struct(CsrBtConnId btConnId,
                                              CsrUint16    thePayloadLength,
                                              void         *thePayload,
                                              CsrUint16    context)
{
    CsrBtCmL2caDataReq        *prim;

    prim                    = (CsrBtCmL2caDataReq *) CsrPmemAlloc(sizeof(CsrBtCmL2caDataReq));
    prim->type              = CSR_BT_CM_L2CA_DATA_REQ;
    prim->btConnId          = btConnId;
    prim->length            = 0;
    prim->payload           = (thePayloadLength == 0)? thePayload : CsrMblkDataCreate(thePayload, thePayloadLength, TRUE);
    prim->context           = context;

    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCml2caDataResSend
 *
 *  DESCRIPTION
 *      Acknowledge data indication
 *
 *  PARAMETERS
 *        btConnId:           Channel identifier
 *----------------------------------------------------------------------------*/
CsrBtCmL2caDataRes *CsrBtCmL2caDataRes_struct(CsrBtConnId btConnId)
{
    CsrBtCmL2caDataRes *prim;
    prim = (CsrBtCmL2caDataRes*)CsrPmemAlloc(sizeof(CsrBtCmL2caDataRes));
    prim->type = CSR_BT_CM_L2CA_DATA_RES;
    prim->btConnId = btConnId;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCml2caDataAbortReqSend
 *
 *  DESCRIPTION
 *      Acknowledge data indication
 *
 *  PARAMETERS
 *        btConnId:            Channel identifier
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_L2CA_DATA_ABORT
CsrBtCmL2caDataAbortReq *CsrBtCmL2caDataAbortReq_struct(CsrBtConnId btConnId)
{
    CsrBtCmL2caDataAbortReq *prim;
    prim = (CsrBtCmL2caDataAbortReq*)CsrPmemAlloc(sizeof(CsrBtCmL2caDataAbortReq));
    prim->type = CSR_BT_CM_L2CA_DATA_ABORT_REQ;
    prim->btConnId = btConnId;
    return prim;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCml2caModeChangeReqSend
 *      CsrBtCml2caAmpForceModeChangeReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        btConnId                :            Channel identifier
 *        theMode            :            Requested link policy mode
 *----------------------------------------------------------------------------*/
CsrBtCmL2caModeChangeReq *CsrBtCml2caModeChangeReq_struct(CsrBtConnId btConnId,
                                                          CsrUint8    mode,
                                                          CsrBool     forceSniff)
{
    CsrBtCmL2caModeChangeReq        *prim;

    prim                = (CsrBtCmL2caModeChangeReq *) CsrPmemAlloc(sizeof(CsrBtCmL2caModeChangeReq));
    prim->type          = CSR_BT_CM_L2CA_MODE_CHANGE_REQ;
    prim->btConnId      = btConnId;
    prim->requestedMode = mode;
    prim->forceSniff    = forceSniff;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CmDml2caModeSettingsReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_MODE_SETTINGS
CsrBtCmDmL2caModeSettingsReq *CsrBtCmDmL2caModeSettingsReq_struct(CsrBtConnId                   btConnId,
                                                                  CsrBtSniffSettings              *sniffSettings,
                                                                  CsrBtParkSettings               *parkSettings,
                                                                  CsrBtSsrSettingsDownstream     *ssrSettings,
                                                                  CsrUint8                       lowPowerPriority)
{
    CsrBtCmDmL2caModeSettingsReq        *prim;

    prim                            = (CsrBtCmDmL2caModeSettingsReq *) CsrPmemAlloc(sizeof(CsrBtCmDmL2caModeSettingsReq));
    prim->type                      = CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ;
    prim->btConnId                  = btConnId;

    if (parkSettings)
    {
        prim->parkSettings          = parkSettings;
        prim->parkSettingsSize      = 1;
    }
    else
    {
        prim->parkSettings          = NULL;
        prim->parkSettingsSize      = 0;
    }

    if (sniffSettings)
    {
        prim->sniffSettings         = sniffSettings;
        prim->sniffSettingsSize     = 1;
    }
    else
    {
        prim->sniffSettings         = NULL;
        prim->sniffSettingsSize     = 0;
    }

    if (ssrSettings)
    {
        prim->ssrSettings          = ssrSettings;
        prim->ssrSettingsSize      = 1;
    }
    else
    {
        prim->ssrSettings          = NULL;
        prim->ssrSettingsSize      = 0;
    }
    prim->lowPowerPriority         = lowPowerPriority;
    return(prim);
}
#endif

#ifdef CSR_BT_INSTALL_L2CAP_CONNLESS_SUPPORT
CsrBtCmL2caConnectionlessDataReq *CsrBtCmL2caConnectionlessDataReq_struct(psm_t                     connectionlessPsm,
                                                                          CsrUint16                 length,
                                                                          CsrUint8                 *payload,
                                                                          const CsrBtDeviceAddr    *deviceAddr)
{
    CsrBtCmL2caConnectionlessDataReq *prim;
    prim = (CsrBtCmL2caConnectionlessDataReq*)CsrPmemAlloc(sizeof(CsrBtCmL2caConnectionlessDataReq));
    prim->type = CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ;
    prim->connectionlessPsm = connectionlessPsm;
    prim->length = length;
    prim->payload = payload;
    prim->deviceAddr = *deviceAddr;

    return prim;
}
#endif

#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmBnepRegisterReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        disableExtended:
 *        disableStack:
 *        manualBridge:
 *        phandle:
 *        deviceAddr:
 *        serviceList:        A list of Services (UUID) to search for
 *      serviceListSize:    Number of services to search for
 *----------------------------------------------------------------------------*/
CsrBtCmBnepRegisterReq *CsrBtCmBnepRegisterReq_struct(CsrBool          theDisableExtended,
                                                      CsrBool          theDisableStack,
                                                      CsrBool          theManualBridge,
                                                      CsrSchedQid      appHandle,
                                                      CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmBnepRegisterReq        *prim;

    prim                        = (CsrBtCmBnepRegisterReq *) CsrPmemZalloc(sizeof(CsrBtCmBnepRegisterReq));
    prim->type                  = CSR_BT_CM_BNEP_REGISTER_REQ;
    prim->disableExtended       = theDisableExtended;
    prim->disableStack          = theDisableStack;
    prim->manualBridge          = theManualBridge;
    prim->phandle               = appHandle;
    prim->deviceAddr            = deviceAddr;
    return(prim);
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmBnepConnectReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        flags:
 *        remoteAddr:
 *        maxFrameSize:
 *        delay:
 *      secLevel
 *----------------------------------------------------------------------------*/
CsrBtCmBnepConnectReq *CsrBtCmBnepConnectReq_struct(BNEP_CONNECT_REQ_FLAGS theFlags,
                                                    ETHER_ADDR               theRemoteAddr,
                                                    CsrUint16               theMaxFrameSize,
                                                    dm_security_level_t    theSecLevel)
{
    CsrBtCmBnepConnectReq        *prim;

    prim                        = (CsrBtCmBnepConnectReq *) CsrPmemAlloc(sizeof(CsrBtCmBnepConnectReq));
    prim->type                  = CSR_BT_CM_BNEP_CONNECT_REQ;
    prim->flags                 = theFlags;
    prim->rem_addr              = theRemoteAddr;
    prim->profileMaxFrameSize   = theMaxFrameSize;
    prim->secLevel              = theSecLevel;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmCancelBnepConnectReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *    PARAMETERS
 *        remoteAddr:
 *----------------------------------------------------------------------------*/
CsrBtCmCancelBnepConnectReq *CsrBtCmCancelBnepConnectReq_struct(ETHER_ADDR remoteAddr)
{
    CsrBtCmCancelBnepConnectReq        *prim;

    prim                        = (CsrBtCmCancelBnepConnectReq *) CsrPmemAlloc(sizeof(CsrBtCmCancelBnepConnectReq));
    prim->type                    = CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ;
    prim->rem_addr                = remoteAddr;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmBnepSetConnectReqFlagsFromRoles
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        localRole:
 *        remoteRole:
 *        accept_in:
 *----------------------------------------------------------------------------*/
BNEP_CONNECT_REQ_FLAGS CsrBtCmBnepSetConnectReqFlagsFromRoles(CsrBtBslPanRole localRole, CsrBtBslPanRole remoteRole, CsrBool accept_in, CsrBool single_user)
{
    BNEP_CONNECT_REQ_FLAGS    connFlags;

    CsrMemSet(&connFlags, 0, sizeof(connFlags));

    /* clear flags */
    connFlags.no_switch = TRUE;
    connFlags.accept_in = accept_in;
    connFlags.lgn = FALSE;
    connFlags.lnap = FALSE;
    connFlags.lpanu = FALSE;
    connFlags.rgn = FALSE;
    connFlags.rnap = FALSE;
    connFlags.rpanu = FALSE;
    connFlags.on_demand = FALSE;
    connFlags.persist = FALSE;

    if (single_user)
    {
        connFlags.single_user = TRUE;
    }

    if (localRole & CSR_BT_BSL_NAP_ROLE)
    {
        connFlags.lnap = TRUE;
    }
    if (localRole & CSR_BT_BSL_GN_ROLE)
    {
        connFlags.lgn = TRUE;
    }
    if (localRole & CSR_BT_BSL_PANU_ROLE)
    {
        connFlags.lpanu = TRUE;
    }

    if (remoteRole & CSR_BT_BSL_NAP_ROLE)
    {
        connFlags.rnap = TRUE;
    }
    if (remoteRole & CSR_BT_BSL_GN_ROLE)
    {
        connFlags.rgn = TRUE;
    }
    if (remoteRole & CSR_BT_BSL_PANU_ROLE)
    {
        connFlags.rpanu = TRUE;
    }

    return connFlags;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmBnepConnectAcceptReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        flags:
 *        remoteAddr:
 *        maxFrameSize:
 *        classOfDevice:
 *        secLevel:           Level of security to be applied
 *----------------------------------------------------------------------------*/
CsrBtCmBnepConnectAcceptReq *CsrBtCmBnepConnectAcceptReq_struct(BNEP_CONNECT_REQ_FLAGS    theFlags,
                                                                ETHER_ADDR                theRemoteAddr,
                                                                CsrUint16                theMaxFrameSize,
                                                                CsrUint24                theClassOfDevice,
                                                                dm_security_level_t     theSecLevel)
{
    CsrBtCmBnepConnectAcceptReq    *prim;

    prim                        = (CsrBtCmBnepConnectAcceptReq *) CsrPmemAlloc(sizeof(CsrBtCmBnepConnectAcceptReq));
    prim->type                  = CSR_BT_CM_BNEP_CONNECT_ACCEPT_REQ;
    prim->flags                 = theFlags;
    prim->rem_addr              = theRemoteAddr;
    prim->classOfDevice         = theClassOfDevice;
    prim->secLevel              = theSecLevel;
    prim->profileMaxFrameSize   = theMaxFrameSize;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmBnepCancelConnectAcceptReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        ......
 *----------------------------------------------------------------------------*/
CsrBtCmBnepCancelConnectAcceptReq *CsrBtCmBnepCancelConnectAcceptReq_struct(void)
{
    CsrBtCmBnepCancelConnectAcceptReq        *prim;

    prim                        = (CsrBtCmBnepCancelConnectAcceptReq *) CsrPmemAlloc(sizeof(CsrBtCmBnepCancelConnectAcceptReq));
    prim->type                  = CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmBnepExtendedDataReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        theEtherType:
 *        theId:
 *        theLength:
 *        *thePayload:
 *        theDestEtherAddr:
 *        theSrcEtherAddr:
 *----------------------------------------------------------------------------*/
CsrBtCmBnepExtendedDataReq *CsrBtCmBnepExtendedDataReq_struct(CsrUint16        theETherType,
                                                              CsrUint16        theID,
                                                              CsrUint16        theLEngth,
                                                              CsrUint8        *thePAyload,
                                                              ETHER_ADDR    theDEstEtherAddr,
                                                              ETHER_ADDR    theSRcEtherAddr)
{
    CsrBtCmBnepExtendedDataReq        *prim;
    prim                = (CsrBtCmBnepExtendedDataReq *) CsrPmemAlloc(sizeof(CsrBtCmBnepExtendedDataReq));
    prim->type          = CSR_BT_CM_BNEP_EXTENDED_DATA_REQ;
    prim->id            = theID;
    prim->etherType     = theETherType;
    prim->dstAddr       = theDEstEtherAddr;
    prim->srcAddr       = theSRcEtherAddr;
    prim->length        = theLEngth;
    prim->payload       = thePAyload;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmBnepExtendedMultiCastDataReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        theEtherType:
 *        theIdNot:
 *        theLength:
 *        *thePayload:
 *        theDestEtherAddr:
 *        theSrcEtherAddr:
 *----------------------------------------------------------------------------*/
CsrBtCmBnepExtendedMulticastDataReq *CsrBtCmBnepExtendedMultiCastDataReq_struct(
    CsrUint16    theETherType,
    CsrUint16    theIDNot,
    CsrUint16    theLEngth,
    CsrUint8        *thePAyload,
    ETHER_ADDR    theDEstEtherAddr,
    ETHER_ADDR    theSRcEtherAddr)
{
    CsrBtCmBnepExtendedMulticastDataReq        *prim;

    prim                = (CsrBtCmBnepExtendedMulticastDataReq *)CsrPmemAlloc(sizeof(CsrBtCmBnepExtendedMulticastDataReq));
    prim->type          = CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ;
    prim->idNot         = theIDNot;
    prim->etherType     = theETherType;
    prim->dstAddr       = theDEstEtherAddr;
    prim->srcAddr       = theSRcEtherAddr;
    prim->length        = theLEngth;
    prim->payload       = thePAyload;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmBnepDisconnectReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        theFlags:
 *        theId:
 *----------------------------------------------------------------------------*/
CsrBtCmBnepDisconnectReq *CsrBtCmBnepDisconnectReq_struct(CsrUint16 theFLags,
                                                          CsrUint16 theID)
{
    CsrBtCmBnepDisconnectReq        *prim;

    prim                = (CsrBtCmBnepDisconnectReq *)CsrPmemAlloc(sizeof(CsrBtCmBnepDisconnectReq));
    prim->type          = CSR_BT_CM_BNEP_DISCONNECT_REQ;
    prim->flags         = theFLags;
    prim->id            = theID;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmBnepDisconnectResSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        theFlags:
 *        theId:
 *----------------------------------------------------------------------------*/
CsrBtCmBnepDisconnectRes *CsrBtCmBnepDisconnectRes_struct(CsrUint16 theFLags,
                                                          CsrUint16 theID)
{
    CsrBtCmBnepDisconnectRes        *prim;

    prim                = (CsrBtCmBnepDisconnectRes *)CsrPmemAlloc(sizeof(CsrBtCmBnepDisconnectRes));
    prim->type          = CSR_BT_CM_BNEP_DISCONNECT_RES;
    prim->flags         = theFLags;
    prim->id            = theID;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmBnepSwitchRoleReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        id:
 *        role:
 *----------------------------------------------------------------------------*/
CsrBtCmBnepSwitchRoleReq *CsrBtCmBnepSwitchRoleReq_struct(CsrUint16    theID,
                                                          CsrUint8    theRole)
{
    CsrBtCmBnepSwitchRoleReq    *prim;

    prim                        = (CsrBtCmBnepSwitchRoleReq *) CsrPmemAlloc(sizeof(CsrBtCmBnepSwitchRoleReq));
    prim->type                  = CSR_BT_CM_BNEP_SWITCH_ROLE_REQ;
    prim->id                    = theID;
    prim->role                  = theRole;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmBnepModeChangeReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *  PARAMETERS
 *        id:
 *        requestedMode:
 *----------------------------------------------------------------------------*/
CsrBtCmBnepModeChangeReq *CsrBtCmBnepModeChangeReq_struct(CsrUint16    theID,
                                                          CsrUint8    theRequestedMode)
{
    CsrBtCmBnepModeChangeReq        *prim;

    prim                        = (CsrBtCmBnepModeChangeReq *) CsrPmemAlloc(sizeof(CsrBtCmBnepModeChangeReq));
    prim->type                  = CSR_BT_CM_BNEP_MODE_CHANGE_REQ;
    prim->id                    = theID;
    prim->requestedMode         = theRequestedMode;
    return(prim);
}

#ifdef CSR_BT_INSTALL_CM_PRI_MODE_SETTINGS
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmDmBnepModeSettingsReqSend
 *
 *  DESCRIPTION
 *        ......
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrBtCmDmBnepModeSettingsReq *CsrBtCmDmBnepModeSettingsReq_struct(CsrUint16                       theId,
                                                                  CsrBtSniffSettings               *theSniffSettings,
                                                                  CsrBtParkSettings                *theParkSettings,
                                                                  CsrBtSsrSettingsDownstream      *theSsrSettings,
                                                                  CsrUint8                        lowPowerPriority)
{
    CsrBtCmDmBnepModeSettingsReq *prim;

    prim                    = (CsrBtCmDmBnepModeSettingsReq *) CsrPmemAlloc(sizeof(CsrBtCmDmBnepModeSettingsReq));
    prim->type              = CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ;
    prim->id                = theId;

    if (theParkSettings)
    {
        prim->parkSettings          = theParkSettings;
        prim->parkSettingsSize      = 1;
    }
    else
    {
        prim->parkSettings          = NULL;
        prim->parkSettingsSize      = 0;
    }

    if (theSniffSettings)
    {
        prim->sniffSettings         = theSniffSettings;
        prim->sniffSettingsSize     = 1;
    }
    else
    {
        prim->sniffSettings         = NULL;
        prim->sniffSettingsSize     = 0;
    }

    if (theSsrSettings)
    {
        prim->ssrSettings          = theSsrSettings;
        prim->ssrSettingsSize      = 1;
    }
    else
    {
        prim->ssrSettings          = NULL;
        prim->ssrSettingsSize      = 0;
    }
    prim->lowPowerPriority         = lowPowerPriority;
    return(prim);
}
#endif /* CSR_BT_INSTALL_CM_PRI_MODE_SETTINGS */
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmReadEncryptionStatusReqSend
 *
 *  DESCRIPTION
 *      Reads the encryption status of an ACL link
 *
 *  PARAMETERS
 *        appHandle:             protocol handle
 *        deviceAddr:             BD address of the connected device
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
CsrBtCmReadEncryptionStatusReq *CsrBtCmReadEncryptionStatusReq_struct(CsrSchedQid    appHandle,
                                                                      CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmReadEncryptionStatusReq *prim;

    prim                = (CsrBtCmReadEncryptionStatusReq *)CsrPmemAlloc(sizeof(CsrBtCmReadEncryptionStatusReq));
    prim->type          = CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ;
    prim->appHandle     = appHandle;
    prim->deviceAddr    = deviceAddr;
    return(prim);
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmClearEventFilterReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        .....
 *----------------------------------------------------------------------------*/
CsrBtCmClearEventFilterReq *CsrBtCmClearEventFilterReq_struct(CsrSchedQid appHandle,
                                                              CsrUint8   theFilter)
{
    CsrBtCmClearEventFilterReq *msg;

    msg            = (CsrBtCmClearEventFilterReq *)CsrPmemAlloc(sizeof(CsrBtCmClearEventFilterReq));
    msg->type      = CSR_BT_CM_CLEAR_EVENT_FILTER_REQ;
    msg->appHandle = appHandle;
    msg->filter    = theFilter;
    return(msg);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSetEventFilterCodReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        .....
 *----------------------------------------------------------------------------*/
CsrBtCmSetEventFilterCodReq *CsrBtCmSetEventFilterCodReq_struct(CsrSchedQid appHandle,
                                                                CsrBool    theSelectInquiryFilter,
                                                                CsrUint8   theAutoAccept,
                                                                CsrUint24  theCod,
                                                                CsrUint24  theCodMask)
{
    CsrBtCmSetEventFilterCodReq *msg;

    msg                      = (CsrBtCmSetEventFilterCodReq *)CsrPmemAlloc(sizeof(CsrBtCmSetEventFilterCodReq));
    msg->type                = CSR_BT_CM_SET_EVENT_FILTER_COD_REQ;
    msg->appHandle           = appHandle;
    msg->selectInquiryFilter = theSelectInquiryFilter;
    msg->autoAccept          = theAutoAccept;
    msg->cod                 = theCod;
    msg->codMask             = theCodMask;
    return(msg);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSetEventFilterBdaddrReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        .....
 *----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_CM_PRI_SET_EVENT_FILTER_BDADDR
CsrBtCmSetEventFilterBdaddrReq *CsrBtCmSetEventFilterBdaddrReq_struct(
    CsrSchedQid    appHandle,
    CsrBool       theSelectInquiryFilter,
    CsrUint8      theAutoAccept,
    CsrBtDeviceAddr theAddress)
{
    CsrBtCmSetEventFilterBdaddrReq *msg;

    msg                      = (CsrBtCmSetEventFilterBdaddrReq *)CsrPmemAlloc(sizeof(CsrBtCmSetEventFilterBdaddrReq));
    msg->type                = CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ;
    msg->appHandle           = appHandle;
    msg->selectInquiryFilter = theSelectInquiryFilter;
    msg->autoAccept          = theAutoAccept;
    msg->address             = theAddress;
    return(msg);
}
#endif

#ifdef CSR_BT_INSTALL_CM_WRITE_VOICE_SETTINGS
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmWriteVoiceSettingsReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *      phandle:               protocol handle
 *      voiceSettings:         voice settings
 *----------------------------------------------------------------------------*/
CsrBtCmWriteVoiceSettingsReq *CsrBtCmWriteVoiceSettingsReq_struct(CsrSchedQid       appHandle,
                                                                  CsrUint16  voiceSettings)
{
    CsrBtCmWriteVoiceSettingsReq *prim;

    prim = (CsrBtCmWriteVoiceSettingsReq *) CsrPmemAlloc(sizeof(CsrBtCmWriteVoiceSettingsReq));
    prim->type = CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ;
    prim->phandle = appHandle;
    prim->voiceSettings = voiceSettings;
    return(prim);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmLogicalChannelTypeReqSend
 *
 *  DESCRIPTION
 *      Information about the type of channel used for the connection
 *
 *  PARAMETERS
 *        logicalChannelTypeMask:       Type of channel of the connection
 *        address:                      bd address connected to
 *        btConnId                      Connection ID
 *----------------------------------------------------------------------------*/
CsrBtCmLogicalChannelTypeReq *CsrBtCmLogicalChannelTypeReq_struct(CsrBtLogicalChannelType  logicalChannelTypeMask,
                                                                  CsrBtDeviceAddr address,
                                                                  CsrBtConnId btConnId)
{
    CsrBtCmLogicalChannelTypeReq *prim;

    prim = (CsrBtCmLogicalChannelTypeReq *)CsrPmemAlloc(sizeof(CsrBtCmLogicalChannelTypeReq));
    prim->type          = CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ;
    prim->logicalChannelTypeMask    = logicalChannelTypeMask;
    prim->deviceAddr    = address;
    prim->btConnId      = btConnId;

    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmRegisterHandlerReq
 *
 *  DESCRIPTION
 *      Register AMPM in the CM
 *
 *  PARAMETERS
 *      ampmHandle:         Handle for the AMPM
 *      flags:              Unused, must be zero
 *----------------------------------------------------------------------------*/
CsrBtCmRegisterHandlerReq *CsrBtCmRegisterHandlerReq_struct(CsrUint8 handlerType,
                                                            CsrSchedQid handle,
                                                            CsrUint32 flags)
{
    CsrBtCmRegisterHandlerReq *prim;

    prim = (CsrBtCmRegisterHandlerReq*)CsrPmemAlloc(sizeof(CsrBtCmRegisterHandlerReq));
    prim->type = CSR_BT_CM_REGISTER_HANDLER_REQ;
    prim->handlerType = handlerType;
    prim->handle = handle;
    prim->flags = flags;

    return prim;
}

#ifdef CSR_AMP_ENABLE
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmMoveChannelReqSend
 *
 *  DESCRIPTION
 *      Request the L2CAP channel to be moved to an AMP or back to the BR/EDR
 *      radio.
 *
 *  PARAMETERS
 *      btConnId        L2CAP channel identifier
 *      remoteControl   Remote AMP controller ID
 *      localControl    Local AMP controller ID
 *----------------------------------------------------------------------------*/
CsrBtCmMoveChannelReq *CsrBtCmMoveChannelReq_struct(CsrBtConnId        btConnId,
                                                            CsrBtAmpController remoteControl,
                                                            CsrBtAmpController localControl)
{
    CsrBtCmMoveChannelReq *prim;
    prim = (CsrBtCmMoveChannelReq*)CsrPmemAlloc(sizeof(CsrBtCmMoveChannelReq));
    prim->type = CSR_BT_CM_MOVE_CHANNEL_REQ;
    prim->btConnId = btConnId;
    prim->remoteControl = remoteControl;
    prim->localControl = localControl;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmMoveChannelResSend
 *
 *  DESCRIPTION
 *      L2CAP move channel channel response generator.
 *
 *  PARAMETERS
 *      btConnId              L2CAP channel identifier
 *      accept           Does the app accept the move or not?
 *----------------------------------------------------------------------------*/
CsrBtCmMoveChannelRes *CsrBtCmMoveChannelRes_struct(CsrBtConnId    btConnId,
                                                            CsrBool accept)
{
    CsrBtCmMoveChannelRes *prim;
    prim = (CsrBtCmMoveChannelRes*)CsrPmemAlloc(sizeof(CsrBtCmMoveChannelRes));
    prim->type = CSR_BT_CM_MOVE_CHANNEL_RES;
    prim->btConnId = btConnId;
    prim->accept = accept;
    return prim;
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmA2dpBitRateReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *  PARAMETERS
 *        deviceAddr:        address of the remote device
 *        streamIdx:         unique Id for the stream in question
 *        bitRate:           bit rate used if known; else CSR_BT_A2DP_BIT_RATE_UNKNOWN
 *
 *----------------------------------------------------------------------------*/
CsrBtCmA2dpBitRateReq *CsrBtCmA2dpBitRateReq_struct(CsrBtDeviceAddr  deviceAddr,
                                                    CsrUint8         streamIdx,
                                                    CsrUint32        bitRate)
{
    CsrBtCmA2dpBitRateReq *prim;

    prim = (CsrBtCmA2dpBitRateReq *)CsrPmemAlloc(sizeof(CsrBtCmA2dpBitRateReq));
    prim->type          = CSR_BT_CM_A2DP_BIT_RATE_REQ;
    prim->deviceAddr    = deviceAddr;
    prim->streamIdx     = streamIdx;
    prim->bitRate       = bitRate;

    return(prim);
}

CsrBtCmGetSecurityConfRes *CsrBtCmGetSecurityConfRes_struct(CsrUint16           options,
                                                            dm_security_mode_t  securityMode,
                                                            dm_security_level_t securityLevelDefault,
                                                            CsrUint16           config,
                                                            CsrUint16           writeAuthEnable,
                                                            CsrUint8            mode3enc,
                                                            CsrUint16 *leEr, /* NULL, or points to 8*CsrUint16 */
                                                            CsrUint16 *leIr, /* NULL, or points to 8*CsrUint16 */
                                                            CsrUint16 leSmDivState,
                                                            CsrUint32 leSmSignCounter)
{
    CsrBtCmGetSecurityConfRes *prim = (CsrBtCmGetSecurityConfRes *)
                                       CsrPmemZalloc(sizeof(CsrBtCmGetSecurityConfRes));

    prim->type                  = CSR_BT_CM_GET_SECURITY_CONF_RES;
    prim->options               = options;
    prim->securityMode          = securityMode;
    prim->securityLevelDefault  = securityLevelDefault;
    prim->config                = config;
    prim->writeAuthEnable       = writeAuthEnable;
    prim->mode3enc              = mode3enc;

    if(leEr)
    {
        prim->leEr = leEr;
        prim->leErCount = 8; /* count always 8 */
    }
    /* else: zalloc used - don't clear anything */
    
    if(leIr)
    {
        prim->leIr = leIr;
        prim->leIrCount = 8; /* count always 8 */
    }
    /* else: zalloc used - don't clear anything */

    prim->leSmDivState = leSmDivState;
    prim->leSmSignCounter = leSmSignCounter;
    return(prim);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmDataBufferEmptyReq
 *
 *  DESCRIPTION
 *      Get told when data Tx pipeline is empty
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
CsrBtCmDataBufferEmptyReq *CsrBtCmDataBufferEmptyReq_struct(CsrBtConnId btConnId)
{
    CsrBtCmDataBufferEmptyReq *prim;
    prim = (CsrBtCmDataBufferEmptyReq*)CsrPmemAlloc(sizeof(CsrBtCmDataBufferEmptyReq));

    prim->type = CSR_BT_CM_DATA_BUFFER_EMPTY_REQ;
    prim->btConnId = btConnId;

    return prim;
}

#ifdef CSR_BT_LE_ENABLE
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmLeScanReq
 *
 *  DESCRIPTION
 *      Toggle low energy scanning and parameter setup
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
CsrBtCmLeScanReq *CsrBtCmLeScanReq_struct(CsrSchedQid appHandle,
                                          CsrUint8 mode,
                                          CsrUint8 paramChange,
                                          CsrUint8 scanType,
                                          CsrUint16 scanInterval,
                                          CsrUint16 scanWindow,
                                          CsrUint8 scanningFilterPolicy,
                                          CsrUint8 filterDuplicates)
{
    CsrBtCmLeScanReq *prim;
    prim = (CsrBtCmLeScanReq*)CsrPmemAlloc(sizeof(CsrBtCmLeScanReq));

    prim->type = CSR_BT_CM_LE_SCAN_REQ;
    prim->appHandle = appHandle;
    prim->mode = mode;
    prim->paramChange = paramChange;
    prim->scanType = scanType;
    prim->scanInterval = scanInterval;
    prim->scanWindow = scanWindow;
    prim->scanningFilterPolicy = scanningFilterPolicy;
    prim->filterDuplicates = filterDuplicates;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmLeAdvertiseReq
 *
 *  DESCRIPTION
 *      Toggle low energy advertising and parameter setup
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
CsrBtCmLeAdvertiseReq *CsrBtCmLeAdvertiseReq_struct(CsrSchedQid appHandle,
                                                    CsrUint8 mode,
                                                    CsrUint8 paramChange,
                                                    CsrUint8 advertisingDataLength,
                                                    CsrUint8 *advertisingData,
                                                    CsrUint8 scanResponseDataLength,
                                                    CsrUint8 *scanResponseData,
                                                    CsrUint16 advIntervalMin,
                                                    CsrUint16 advIntervalMax,
                                                    CsrUint8 advertisingType,
                                                    CsrBtTypedAddr directAddress,
                                                    CsrUint8 advertisingChannelMap,
                                                    CsrUint8 advertisingFilterPolicy)
{
    CsrBtCmLeAdvertiseReq *prim;
    prim = (CsrBtCmLeAdvertiseReq*)CsrPmemAlloc(sizeof(CsrBtCmLeAdvertiseReq));

    prim->type = CSR_BT_CM_LE_ADVERTISE_REQ;
    prim->appHandle = appHandle;
    prim->mode = mode;
    prim->paramChange = paramChange;
    prim->advertisingDataLength = advertisingDataLength;
    prim->advertisingData = advertisingData;
    prim->scanResponseDataLength = scanResponseDataLength;
    prim->scanResponseData = scanResponseData;
    prim->advIntervalMin = advIntervalMin;
    prim->advIntervalMax = advIntervalMax;
    prim->advertisingType = advertisingType;
    prim->directAddress = directAddress;
    prim->advertisingChannelMap = advertisingChannelMap;
    prim->advertisingFilterPolicy = advertisingFilterPolicy;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSmLeSecurityReq
 *
 *  DESCRIPTION
 *      Highten security level towards peer.
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
CsrBtCmSmLeSecurityReq *CsrBtCmSmLeSecurityReq_struct(CsrBtTypedAddr addr,
                                                      CsrUint16 l2caConFlags,
                                                      CsrUint16 context,
                                                      CsrUint16 securityRequirements)
{
    CsrBtCmSmLeSecurityReq *prim;
    prim = (CsrBtCmSmLeSecurityReq*)CsrPmemAlloc(sizeof(CsrBtCmSmLeSecurityReq));
    prim->type = CSR_BT_CM_SM_LE_SECURITY_REQ;
    CsrBtAddrCopy(&prim->addr, &addr);
    prim->l2caConFlags = l2caConFlags;
    prim->context = context;
    prim->securityRequirements = securityRequirements;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSmKeyRequestRes
 *
 *  DESCRIPTION
 *      Send key request response
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
CsrBtCmSmKeyRequestRes *CsrBtCmSmKeyRequestRes_struct(CsrBtDeviceAddr *address,
                                                      CsrUint16 securityRequirements,
                                                      CsrUint16 keyType,
                                                      CsrUint16 keySize,
                                                      CsrUint8 *key,
                                                      CsrBtAddressType addressType)
{
    CsrBtCmSmKeyRequestRes *prim;
    prim = (CsrBtCmSmKeyRequestRes*)CsrPmemAlloc(sizeof(CsrBtCmSmKeyRequestRes));
    CsrBtBdAddrCopy(&(prim->address), address);
    prim->type = CSR_BT_CM_SM_KEY_REQUEST_RES;
    prim->securityRequirements = securityRequirements;
    prim->keyType = keyType;
    prim->keySize = keySize;
    prim->key = key;
    prim->addressType = addressType;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmLeWhitelistSetReq
 *
 *  DESCRIPTION
 *      Control whitelist on low energy
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
CsrBtCmLeWhitelistSetReq *CsrBtCmLeWhitelistSetReq_struct(CsrSchedQid appHandle,
                                                          CsrUint8 operation,
                                                          CsrBtTypedAddr address)
{
    CsrBtCmLeWhitelistSetReq *prim;
    prim = (CsrBtCmLeWhitelistSetReq*)CsrPmemAlloc(sizeof(CsrBtCmLeWhitelistSetReq));
    prim->type = CSR_BT_CM_LE_WHITELIST_SET_REQ;
    prim->appHandle = appHandle;
    prim->operation = operation;
    prim->address = address;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmLeConnparamReq
 *
 *  DESCRIPTION
 *      Set LE connection parameters and acceptable values
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
CsrBtCmLeConnparamReq *CsrBtCmLeConnparamReq_struct(CsrSchedQid appHandle,
                                                    CsrUint16 scanInterval,
                                                    CsrUint16 scanWindow,
                                                    CsrUint16 connIntervalMin,
                                                    CsrUint16 connIntervalMax,
                                                    CsrUint16 connLatency,
                                                    CsrUint16 supervisionTimeout,
                                                    CsrUint16 connAttemptTimeout,
                                                    CsrUint16 advIntervalMin,
                                                    CsrUint16 advIntervalMax,
                                                    CsrUint16 connLatencyMax,
                                                    CsrUint16 supervisionTimeoutMin,
                                                    CsrUint16 supervisionTimeoutMax)
{
    CsrBtCmLeConnparamReq *prim;
    prim = (CsrBtCmLeConnparamReq*)CsrPmemAlloc(sizeof(CsrBtCmLeConnparamReq));

    prim->type = CSR_BT_CM_LE_CONNPARAM_REQ;
    prim->appHandle = appHandle;
    prim->scanInterval = scanInterval;
    prim->scanWindow = scanWindow;
    prim->connIntervalMin = connIntervalMin;
    prim->connIntervalMax = connIntervalMax;
    prim->connLatency = connLatency;
    prim->supervisionTimeout = supervisionTimeout;
    prim->connAttemptTimeout = connAttemptTimeout;
    prim->advIntervalMin = advIntervalMin;
    prim->advIntervalMax = advIntervalMax;
    prim->connLatencyMax = connLatencyMax;
    prim->supervisionTimeoutMin = supervisionTimeoutMin;
    prim->supervisionTimeoutMax = supervisionTimeoutMax;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmLeConnparamUpdateReq
 *
 *  DESCRIPTION
 *      Update LE connection parameters for existing connection
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
CsrBtCmLeConnparamUpdateReq *CsrBtCmLeConnparamUpdateReq_struct(CsrSchedQid appHandle,
                                                                CsrBtTypedAddr address,
                                                                CsrUint16 connIntervalMin,
                                                                CsrUint16 connIntervalMax,
                                                                CsrUint16 connLatency,
                                                                CsrUint16 supervisionTimeout,
                                                                CsrUint16 minimumCeLength,
                                                                CsrUint16 maximumCeLength)
{
    CsrBtCmLeConnparamUpdateReq *prim;
    prim = (CsrBtCmLeConnparamUpdateReq*)CsrPmemAlloc(sizeof(CsrBtCmLeConnparamUpdateReq));
    prim->type = CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ;

    prim->appHandle = appHandle;
    prim->address = address;
    prim->connIntervalMin = connIntervalMin;
    prim->connIntervalMax = connIntervalMax;
    prim->connLatency = connLatency;
    prim->supervisionTimeout = supervisionTimeout;
    prim->minimumCeLength = minimumCeLength;
    prim->maximumCeLength = maximumCeLength;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmLeAcceptConnparamUpdateRes
 *
 *  DESCRIPTION
 *      Accept or reject LE Slave REquest to Update LE connection parameters 
 *      for existing connection
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
CsrBtCmLeAcceptConnparamUpdateRes *CsrBtCmLeAcceptConnparamUpdateRes_struct(CsrBtTypedAddr    address, 
                                                                            l2ca_identifier_t l2caSignalId, 
                                                                            CsrUint16         connIntervalMin, 
                                                                            CsrUint16         connIntervalMax, 
                                                                            CsrUint16         connLatency,
                                                                            CsrUint16         supervisionTimeout, 
                                                                            CsrBool           accept)
{
    CsrBtCmLeAcceptConnparamUpdateRes *prim = (CsrBtCmLeAcceptConnparamUpdateRes*)
                                               CsrPmemAlloc(sizeof(CsrBtCmLeAcceptConnparamUpdateRes));

    prim->type               = CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES;
    prim->address            = address;
    prim->l2caSignalId       = l2caSignalId;  
    prim->connIntervalMin    = connIntervalMin;
    prim->connIntervalMax    = connIntervalMax;
    prim->connLatency        = connLatency;
    prim->supervisionTimeout = supervisionTimeout;
    prim->accept             = accept;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmSmSetEncryptionKeySizeReqSend
 *
 *  DESCRIPTION
 *      Set low energy min/max encryption key sizes
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
CsrBtCmSmSetEncryptionKeySizeReq *CsrBtCmSmSetEncryptionKeySizeReq_struct(CsrUint8 minKeySize,
                                                                          CsrUint8 maxKeySize)
{
    CsrBtCmSmSetEncryptionKeySizeReq *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtCmSmSetEncryptionKeySizeReq));
    prim->type = CSR_BT_CM_SM_SET_ENCRYPTION_KEY_SIZE_REQ;
    prim->minKeySize = minKeySize;
    prim->maxKeySize = maxKeySize;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmLePhysicalLinkStatusReqSend
 *
 *  DESCRIPTION
 *      Inform CM about LE physical link
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
CsrBtCmLePhysicalLinkStatusReq *CsrBtCmLePhysicalLinkStatusReq_struct(CsrBtTypedAddr address,
                                                                      CsrBool        radioType,
                                                                      CsrBool        status)
{
    CsrBtCmLePhysicalLinkStatusReq *prim;
    prim            = CsrPmemAlloc(sizeof(CsrBtCmLePhysicalLinkStatusReq));
    prim->type      = CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ;
    prim->address   = address;
    prim->radioType = radioType;
    prim->status    = status;
    return prim;
}
#endif /* CSR_BT_LE_ENABLE */
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmReadEncryptionKeySizeReqSend
 *
 *  DESCRIPTION
 *      Read the encryption key size for particular link
 *
 *  PARAMETERS
 *
 *----------------------------------------------------------------------------*/
CsrBtCmReadEncryptionKeySizeReq *CsrBtCmReadEncryptionKeySizeReq_struct(CsrSchedQid appHandle,
                                                                        CsrBtTypedAddr addr,
                                                                        CsrUint16 context)
{
    CsrBtCmReadEncryptionKeySizeReq *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtCmReadEncryptionKeySizeReq));
    prim->type = CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_REQ;
    prim->appHandle = appHandle;
    prim->address = addr;
    prim->context = context;
    return prim;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtCmL2caGetChannelInfoReqSend
 *
 *  DESCRIPTION
 *      Get the ACL channel ID and remote CID
 *
 *  PARAMETERS
 *        btConnId:            Channel identifier
 *        appHandle:           application handle to answer to
 *----------------------------------------------------------------------------*/
CsrBtCmL2caGetChannelInfoReq *CsrBtCmL2caGetChannelInfoReq_struct(CsrBtConnId btConnId,
                                                                  CsrSchedQid appHandle)
{
    CsrBtCmL2caGetChannelInfoReq *prim;
    prim = (CsrBtCmL2caGetChannelInfoReq*)CsrPmemAlloc(sizeof(CsrBtCmL2caGetChannelInfoReq));
    prim->type = CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ;
    prim->btConnId  = btConnId;
    prim->appHandle = appHandle;
    return prim;
}



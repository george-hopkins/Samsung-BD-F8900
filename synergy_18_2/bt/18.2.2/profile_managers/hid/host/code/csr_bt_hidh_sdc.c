/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "sds_prim.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_hidh_main.h"
#include "csr_bt_hidh_prim.h"
#include "csr_bt_hidh_con.h"
#include "csr_bt_hidh_data.h"
#include "csr_bt_hidh_sigup.h"
#include "csr_bt_hidh_util.h"

#define HIDH_MAX_SDP_ATTRIBUTE    300 /* maximum SDP attribute size to read */

/* list of attributes of interest */
typedef enum
{
    HID_PARSER_VERSION            = 0x0201,
    HID_DEVICE_SUBCLASS,
    HID_COUNTRY_CODE,
    HID_VIRTUAL_CABLE,
    HID_RECONNECT_INITIATE,
    HID_DESCRIPTOR_LIST,
    HID_LANGID_BASE_LIST,
    HID_SDP_DISABLE,
    HID_BATTERY_POWER,
    HID_REMOTE_WAKE,
    HID_PROFILE_VERSION,
    HID_SUPERVISION_TIMEOUT,
    HID_NORMALLY_CONNECTABLE,
    HID_BOOT_DEVICE,
    ENUM_HID_ATTRIBUTE_END
} hidh_sdp_attrib_t;

#define HID_MAX_ATTRIBUTE_IDS (ENUM_HID_ATTRIBUTE_END - HID_PARSER_VERSION)

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpParserVersion
 *
 *  DESCRIPTION
 *      Store parser version
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpParserVersion(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrBtSdcGetUint16Direct(prim->attributeListSize,
                       prim->attributeList,
                       attributeId,
                       &subInstData->sdpInfo.parserVersion);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpDeviceSubclass
 *
 *  DESCRIPTION
 *      Store device subclass
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpDeviceSubclass(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrBtSdcGetUint8Direct(prim->attributeListSize,
                      prim->attributeList,
                      attributeId,
                      &subInstData->sdpInfo.deviceSubclass);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpCountryCode
 *
 *  DESCRIPTION
 *      Store country code
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpCountryCode(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrBtSdcGetUint8Direct(prim->attributeListSize,
                      prim->attributeList,
                      attributeId,
                      &subInstData->sdpInfo.countryCode);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpVirtualCable
 *
 *  DESCRIPTION
 *      Store virtual cable flag
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpVirtualCable(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrUint8    flag;

    CsrBtSdcGetBoolDirect(prim->attributeListSize, prim->attributeList, attributeId, &flag);

    if( flag )
    {
        subInstData->sdpInfo.flags |= CSR_BT_FLAGS_VIRTUAL_CABLE_BIT;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpReconnectInitiate
 *
 *  DESCRIPTION
 *      Store reconnect initiate flag
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpReconnectInitiate(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrUint8    flag;

    CsrBtSdcGetBoolDirect(prim->attributeListSize, prim->attributeList, attributeId, &flag);

    if( flag )
    {
        subInstData->sdpInfo.flags |= CSR_BT_FLAGS_RECONNECT_INIT_BIT;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpDescriptorList
 *
 *  DESCRIPTION
 *      Store descriptor list
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpDescriptorList(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrPmemFree(subInstData->descriptor);
    subInstData->descriptor = (CsrUint8 *) CsrPmemAlloc(prim->attributeListSize - 3);
    CsrMemCpy(subInstData->descriptor, prim->attributeList + 3, prim->attributeListSize - 3);
    subInstData->descriptorLength = prim->attributeListSize - 3;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpLangidBaseList
 *
 *  DESCRIPTION
 *      Value not stored, included only for simplified reading of SDP record
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpLangidBaseList(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    /* NOP */
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpSdpDisable
 *
 *  DESCRIPTION
 *      Store SDP disable flag
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpSdpDisable(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrUint8    flag;

    CsrBtSdcGetBoolDirect(prim->attributeListSize, prim->attributeList, attributeId, &flag);

    if( flag )
    {
        subInstData->sdpInfo.flags |= CSR_BT_FLAGS_SDP_DISABLE_BIT;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpBatteryPower
 *
 *  DESCRIPTION
 *      Store battery power
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpBatteryPower(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrUint8    flag;

    CsrBtSdcGetBoolDirect(prim->attributeListSize, prim->attributeList, attributeId, &flag);

    if( flag )
    {
        subInstData->sdpInfo.flags |= CSR_BT_FLAGS_BATTERY_POWER_BIT;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpRemoteWake
 *
 *  DESCRIPTION
 *      Store remote wake flag
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpRemoteWake(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrUint8    flag;

    CsrBtSdcGetBoolDirect(prim->attributeListSize, prim->attributeList, attributeId, &flag);

    if( flag )
    {
        subInstData->sdpInfo.flags |= CSR_BT_FLAGS_REMOTE_WAKE_BIT;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpProfileVersion
 *
 *  DESCRIPTION
 *      Store profile version
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpProfileVersion(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrBtSdcGetUint16Direct(prim->attributeListSize,
                       prim->attributeList,
                       attributeId,
                       &subInstData->sdpInfo.profileVersion);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpSupervisionTimeout
 *
 *  DESCRIPTION
 *      Store link supervision timeout value
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpSupervisionTimeout(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrBtSdcGetUint16Direct(prim->attributeListSize,
                       prim->attributeList,
                       attributeId,
                       &subInstData->sdpInfo.supervisionTimeout);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpNormallyConnectable
 *
 *  DESCRIPTION
 *      Store 'normally connectable' flag
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpNormallyConnectable(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrUint8    flag;

    CsrBtSdcGetBoolDirect(prim->attributeListSize, prim->attributeList, attributeId, &flag);

    if( flag )
    {
        subInstData->sdpInfo.flags |= CSR_BT_FLAGS_NORMALLY_CONNECT_BIT;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpBootDevice
 *
 *  DESCRIPTION
 *      Store boot mode flag
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpBootDevice(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrUint8    flag;

    CsrBtSdcGetBoolDirect(prim->attributeListSize, prim->attributeList, attributeId, &flag);

    if( flag )
    {
        subInstData->sdpInfo.flags |= CSR_BT_FLAGS_BOOT_DEVICE_BIT;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSdpServiceName
 *
 *  DESCRIPTION
 *      Store boot mode flag
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhSdpServiceName(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim)
{
    CsrUint32 dummySize, dummyIndex;

    CsrPmemFree(subInstData->serviceName);
    subInstData->serviceName = CsrPmemAlloc(prim->attributeListSize);

    CsrBtSdcGetTextStringDirect((CsrUint8 *)subInstData->serviceName,
                           128, prim->attributeListSize,
                           prim->attributeList,
                           attributeId,
                           &dummySize,
                           &dummyIndex);
}

typedef void (* hidhSdpAttribHandler_t)(hidh_subInstData_t * subInstData, CsrUint16 attributeId, CsrBtCmSdcAttributeCfm *prim);

/* ---------- HIDH attribute handler jump table ---------- */
static  hidhSdpAttribHandler_t hidhSdpAttribHandler[HID_MAX_ATTRIBUTE_IDS] =
{
    CsrBtHidhSdpParserVersion,                /* HID_PARSER_VERSION */
    CsrBtHidhSdpDeviceSubclass,                /* HID_DEVICE_SUBCLASS */
    CsrBtHidhSdpCountryCode,                    /* HID_COUNTRY_CODE */
    CsrBtHidhSdpVirtualCable,                /* HID_VIRTUAL_CABLE */
    CsrBtHidhSdpReconnectInitiate,            /* HID_RECONNECT_INITIATE */
    CsrBtHidhSdpDescriptorList,                /* HID_DESCRIPTOR_LIST */
    CsrBtHidhSdpLangidBaseList,                /* HID_LANGID_BASE_LIST */
    CsrBtHidhSdpSdpDisable,                    /* HID_SDP_DISABLE */
    CsrBtHidhSdpBatteryPower,                /* HID_BATTERY_POWER */
    CsrBtHidhSdpRemoteWake,                    /* HID_REMOTE_WAKE */
    CsrBtHidhSdpProfileVersion,                /* HID_PROFILE_VERSION */
    CsrBtHidhSdpSupervisionTimeout,            /* HID_SUPERVISION_TIMEOUT */
    CsrBtHidhSdpNormallyConnectable,            /* HID_NORMALLY_CONNECTABLE */
    CsrBtHidhSdpBootDevice                    /* HID_BOOT_DEVICE */
};

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhCmSdcSearchInd
 *
 *  DESCRIPTION
 *      Handle CSR_BT_CM_SDC_SEARCH_IND. Store SDP record handle (if at least one HID
 *        service record is present)
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhCmSdcSearchInd(hidh_instData_t *instData)
{
    CsrBtCmSdcSearchInd *prim;

    prim = (CsrBtCmSdcSearchInd*) instData->recvMsgP;

    if (instData->subInstData[instData->connectDeviceId].subState == SDP_SEARCH && prim->serviceHandleListCount > 0)
    {
        if( prim->service == CSR_BT_HID_PROFILE_UUID)
        {
            instData->hidServiceRecordHandle = *prim->serviceHandleList;
        }
        else if( prim->service == CSR_BT_PNP_INFORMATION_UUID)
        {
            instData->devIdServiceRecordHandle = *prim->serviceHandleList;
        }
    }
    CsrPmemFree(prim->serviceHandleList);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhCmSdcSearchCfm
 *
 *  DESCRIPTION
 *      Handle CSR_BT_CM_SDC_SEARCH_CFM. Start reading attributes
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhCmSdcSearchCfm(hidh_instData_t *instData)
{
    if( instData->hidServiceRecordHandle )
    {
        instData->sdpAttrib = CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER;
        CsrBtCmSdcAttributeReqSend( instData->hidServiceRecordHandle, instData->sdpAttrib, HIDH_MAX_SDP_ATTRIBUTE);
    }
    else
    {
        /* no HID service was found */
        CsrBtHidhConnectCfmSend( instData->ctrlHandle,
                                 CSR_BT_RESULT_CODE_HIDH_SDC_SEARCH_FAILED,
                                 CSR_BT_SUPPLIER_HIDH,
                                 instData->subInstData[instData->connectDeviceId].bdAddr,
                                 instData->connectDeviceId,
                                 NULL,
                                 CSR_BT_CONN_ID_INVALID);

        freeSubInst(instData, instData->connectDeviceId);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhCmSdcAttributeCfm
 *
 *  DESCRIPTION
 *      Handle CSR_BT_CM_SDC_ATTRIBUTE_CFM.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhCmSdcAttributeCfm(hidh_instData_t *instData)
{
    CsrBtCmSdcAttributeCfm        *prim;
    hidh_subInstData_t            *subInstData;
    CsrUint16                     jumpIndex = 0;

    subInstData = &instData->subInstData[instData->connectDeviceId];

    prim    = (CsrBtCmSdcAttributeCfm *) instData->recvMsgP;

    if( instData->sdpAttrib == CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER)
    {
        if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            CsrBtHidhSdpServiceName(subInstData, instData->sdpAttrib, prim);
            CsrPmemFree(prim->attributeList);
        }
        instData->sdpAttrib = HID_PARSER_VERSION;
        CsrBtCmSdcAttributeReqSend( instData->hidServiceRecordHandle, instData->sdpAttrib, HIDH_MAX_SDP_ATTRIBUTE);
        return;
    }

    if( instData->hidServiceRecordHandle )
    {
        if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            jumpIndex    = instData->sdpAttrib - HID_PARSER_VERSION;
            if( jumpIndex < HID_MAX_ATTRIBUTE_IDS)
            {
                hidhSdpAttribHandler[jumpIndex](subInstData, instData->sdpAttrib, prim);
            }
        }

        instData->sdpAttrib++;
        jumpIndex = instData->sdpAttrib - HID_PARSER_VERSION;
    }

    if( instData->hidServiceRecordHandle && (jumpIndex < HID_MAX_ATTRIBUTE_IDS) )
    {
        CsrBtCmSdcAttributeReqSend( instData->hidServiceRecordHandle, instData->sdpAttrib, HIDH_MAX_SDP_ATTRIBUTE);
    }
    else
    {
        instData->subInstData[instData->connectDeviceId].sdpInfo.isValid = TRUE;
        instData->hidServiceRecordHandle = 0; /* reset to signal that we are done with the HID record */

        if( instData->devIdServiceRecordHandle )
        {
            /* a device ID service record is present, collect info from it */

            switch( instData->sdpAttrib )
            {
                case ENUM_HID_ATTRIBUTE_END:
                    {
                        instData->sdpAttrib = CSR_BT_VENDOR_ID_ATTRIBUTE_IDENTIFIER;
                        CsrBtCmSdcAttributeReqSend( instData->devIdServiceRecordHandle, instData->sdpAttrib, HIDH_MAX_SDP_ATTRIBUTE);
                        break;
                    }
                case CSR_BT_VENDOR_ID_ATTRIBUTE_IDENTIFIER:
                    {
                        if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                        {
                            CsrBtSdcGetUint16Direct(prim->attributeListSize,
                                               prim->attributeList,
                                               instData->sdpAttrib,
                                               &subInstData->sdpInfo.vendorId);
                        }
                        else
                        {
                            subInstData->sdpInfo.vendorId = 0xFFFF; /* reserved default */
                        }
                        instData->sdpAttrib = CSR_BT_PRODUCT_ID_ATTRIBUTE_IDENTIFIER;
                        CsrBtCmSdcAttributeReqSend( instData->devIdServiceRecordHandle, instData->sdpAttrib, HIDH_MAX_SDP_ATTRIBUTE);
                        break;
                    }
                case CSR_BT_PRODUCT_ID_ATTRIBUTE_IDENTIFIER:
                    {
                        if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                        {
                            CsrBtSdcGetUint16Direct(prim->attributeListSize,
                                               prim->attributeList,
                                               instData->sdpAttrib,
                                               &subInstData->sdpInfo.productId);
                        }
                        else
                        {
                            subInstData->sdpInfo.productId = 0;
                        }
                        instData->sdpAttrib = CSR_BT_VERSION_ATTRIBUTE_IDENTIFIER;
                        CsrBtCmSdcAttributeReqSend( instData->devIdServiceRecordHandle, instData->sdpAttrib, HIDH_MAX_SDP_ATTRIBUTE);
                        break;
                    }
                case CSR_BT_VERSION_ATTRIBUTE_IDENTIFIER:
                    {
                        if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                        {
                            CsrBtSdcGetUint16Direct(prim->attributeListSize,
                                               prim->attributeList,
                                               instData->sdpAttrib,
                                               &subInstData->sdpInfo.deviceVersion);
                        }
                        else
                        {
                            subInstData->sdpInfo.deviceVersion  = 0;
                        }

                        /* done collecting info, closing SDP session */
                        CsrBtCmSdcCloseReqSend(CSR_BT_HIDH_IFACEQUEUE);
                        break;
                    }
                default:
                    {
                        break;
                    }
            }
        }
        else
        {
            /* no device ID information present, closing SDP */
            subInstData->sdpInfo.vendorId       = 0xFFFF; /* reserved default */
            subInstData->sdpInfo.productId      = 0;
            subInstData->sdpInfo.deviceVersion  = 0;

            CsrBtCmSdcCloseReqSend(CSR_BT_HIDH_IFACEQUEUE);
        }
    }
    /* Remember to free the attribute list received*/
    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrPmemFree(prim->attributeList);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhCmSdcCloseInd
 *
 *  DESCRIPTION
 *      Handle CSR_BT_CM_SDC_CLOSE_IND.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrBtHidhCmSdcCloseInd(hidh_instData_t *instData)
{
    hidh_subInstData_t        *subInstData;

    subInstData = &instData->subInstData[instData->connectDeviceId];
    if( subInstData->subState == SDP_SEARCH )
    {
        if( subInstData->sdpInfo.isValid )
        {
            if( !instData->doTerminate )
            {
                CsrBtCml2caConnectReqSend( CSR_BT_HIDH_IFACEQUEUE,
                                      subInstData->bdAddr,
                                      CSR_BT_HID_CTRL_PSM,
                                      CSR_BT_HID_CTRL_PSM,
                                      CSR_BT_HIDH_PROFILE_DEFAULT_MTU_SIZE,
                                      instData->secOutgoing);

                CSR_BT_HIDH_SUB_STATE_CHANGE(instData->connectDeviceId, ESTABLISH_CTRL);
            }
            else
            {
                freeSubInst(instData, instData->connectDeviceId);
            }
        }
        else
        {
            if( !instData->doTerminate )
            {
                CsrBtHidhConnectCfmSend( instData->ctrlHandle,
                                         CSR_BT_RESULT_CODE_HIDH_SDC_SEARCH_FAILED,
                                         CSR_BT_SUPPLIER_HIDH,
                                         subInstData->bdAddr,
                                         instData->connectDeviceId,
                                         NULL,
                                         CSR_BT_CONN_ID_INVALID);
            }

            freeSubInst(instData, instData->connectDeviceId);
        }
    }
}



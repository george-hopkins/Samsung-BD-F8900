/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "sds_prim.h"
#include "csr_bt_av_main.h"

/*************************************************************************************
 Service Records
************************************************************************************/
#define AV_SUPPORTED_FEATURES_INDEX        46

/* Audio Source Service Record */
static const CsrUint8 sdsA2dSrcServiceRecord[] =
{
    /* service class ID list */
    0x09, 0x00, 0x01,    /* AttrId = ServiceClassIdList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x11, 0x0A,    /* UUID(2 bytes), Service class = Audio Source = 0x110A */

    /* protocol descriptor list */
    0x09, 0x00, 0x04,    /* AttrId = ProtocolDescriptorList */
    0x35, 0x10,          /* Data element seq. 16 bytes */
    /*    L2CAP */
    0x35, 0x06,          /* Data element seq. 6 bytes */
    0x19, 0x01, 0x00,    /* UUID(2 bytes), Protocol = L2CAP */
    0x09, 0x00, 0x19,    /* PSM value = AVDTP = 0x0019 */
    /*  AVDTP */
    0x35, 0x06,          /* Data element seq. 6 bytes */
    0x19, 0x00, 0x19,    /* UUID(2 bytes), Protocol = AVDTP */
    0x09, 0x01, 0x02,    /* Version = 1.2 = 0x0102 */

    /* bluetooth profile descriptor list */
    0x09, 0x00, 0x09,    /* AttrId = BluetoothProfileDescriptorList */
    0x35, 0x08,          /* Data element seq. 8 bytes */
    0x35, 0x06,          /* Data element seq. 6 bytes */
    0x19, 0x11, 0x0D,    /* UUID(2 bytes), Profile = Advanced Audio Distribution */
    0x09, 0x01, 0x02,    /* Version = 1.2 = 0x0102 */

    /* supported features */
    0x09, 0x03, 0x11,    /* AttrId = Supported features */
    0x09, 0x00, 0x00,    /* 16 bit value, default to zero, index 46/47 */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,    /* AttrId = BrowseGroupList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,    /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

/* Audio Sink Service Record */
static const CsrUint8 sdsA2dSnkServiceRecord[] =
{
    /* service class ID list */
    0x09, 0x00, 0x01,    /* AttrId = ServiceClassIdList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x11, 0x0B,    /* UUID(2 bytes), Service class = Audio Sink = 0x110B */

    /* protocol descriptor list */
    0x09, 0x00, 0x04,    /* AttrId = ProtocolDescriptorList */
    0x35, 0x10,          /* Data element seq. 16 bytes */
    /*    L2CAP */
    0x35, 0x06,          /* Data element seq. 6 bytes */
    0x19, 0x01, 0x00,    /* UUID(2 bytes), Protocol = L2CAP */
    0x09, 0x00, 0x19,    /* PSM value = AVDTP = 0x0019 */
    /*  AVDTP */
    0x35, 0x06,          /* Data element seq. 6 bytes */
    0x19, 0x00, 0x19,    /* UUID(2 bytes), Protocol = AVDTP */
    0x09, 0x01, 0x02,    /* Version = 1.3 = 0x0102 */

    /* bluetooth profile descriptor list */
    0x09, 0x00, 0x09,    /* AttrId = BluetoothProfileDescriptorList */
    0x35, 0x08,          /* Data element seq. 8 bytes */
    0x35, 0x06,          /* Data element seq. 6 bytes */
    0x19, 0x11, 0x0D,    /* UUID(2 bytes), Profile = Advanced Audio Distribution */
    0x09, 0x01, 0x02,    /* Version = 1.3 = 0x0102 */

    /* supported features */
    0x09, 0x03, 0x11,    /* AttrId = Supported features */
    0x09, 0x00, 0x00,    /* 16 bit value, default to zero, index 46/47 */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,    /* AttrId = BrowseGroupList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,    /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

/* Video Source Service Record */
static const CsrUint8 sdsVdpSrcServiceRecord[] =
{
    /* service class ID list */
    0x09, 0x00, 0x01,    /* AttrId = ServiceClassIdList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x13, 0x03,    /* UUID(2 bytes), Service class = Video Source = 0x1303 */

    /* protocol descriptor list */
    0x09, 0x00, 0x04,    /* AttrId = ProtocolDescriptorList */
    0x35, 0x10,          /* Data element seq. 16 bytes */
    /* L2CAP */
    0x35, 0x06,          /* Data element seq. 6 bytes */
    0x19, 0x01, 0x00,    /* UUID(2 bytes), Protocol = L2CAP */
    0x09, 0x00, 0x19,    /* PSM value = AVDTP = 0x0019 */
    /* AVDTP */
    0x35, 0x06,          /* Data element seq. 6 bytes */
    0x19, 0x00, 0x19,    /* UUID(2 bytes), Protocol = AVDTP */
    0x09, 0x01, 0x02,    /* Version = 1.3 = 0x0102 */

    /* bluetooth profile descriptor list */
    0x09, 0x00, 0x09,    /* AttrId = BluetoothProfileDescriptorList */
    0x35, 0x08,          /* Data element seq. 8 bytes */
    0x35, 0x06,          /* Data element seq. 6 bytes */
    0x19, 0x13, 0x05,    /* UUID(2 bytes), Profile = Video Distribution */
    0x09, 0x01, 0x00,    /* Version = 1.00 = 0x0100 */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,    /* AttrId = BrowseGroupList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,    /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

/* Video Sink Service Record */
static const CsrUint8 sdsVdpSnkServiceRecord[] =
{
    /* service class ID list */
    0x09, 0x00, 0x01,    /* AttrId = ServiceClassIdList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x13, 0x04,    /* UUID(2 bytes), Service class = Video Sink = 0x1304 */

    /* protocol descriptor list */
    0x09, 0x00, 0x04,    /* AttrId = ProtocolDescriptorList */
    0x35, 0x10,          /* Data element seq. 16 bytes */
    /* L2CAP */
    0x35, 0x06,          /* Data element seq. 6 bytes */
    0x19, 0x01, 0x00,    /* UUID(2 bytes), Protocol = L2CAP */
    0x09, 0x00, 0x19,    /* PSM value = AVDTP = 0x0019 */
    /* AVDTP */
    0x35, 0x06,          /* Data element seq. 6 bytes */
    0x19, 0x00, 0x19,    /* UUID(2 bytes), Protocol = AVDTP */
    0x09, 0x01, 0x02,    /* Version = 1.3 = 0x0102 */

    /* bluetooth profile descriptor list */
    0x09, 0x00, 0x09,    /* AttrId = BluetoothProfileDescriptorList */
    0x35, 0x08,          /* Data element seq. 8 bytes */
    0x35, 0x06,          /* Data element seq. 6 bytes */
    0x19, 0x13, 0x05,    /* UUID(2 bytes), Profile = Video Distribution */
    0x09, 0x01, 0x00,    /* Version = 1.00 = 0x0100 */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,    /* AttrId = BrowseGroupList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,    /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

/*----------------------------------------------------------------------------*
 *  NAME
 *      addProviderName
 *
 *  DESCRIPTION
 *      Add a 'provider name' to the reference service record
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
static void addProviderName(CsrUint16 *index, CsrUint8 *record)
{
    const CsrUint8 attrId[5] = { 0x09, 0x01, 0x02, 0x25, 0x00};
    CsrUint8 length;

    length = (CsrUint8) CsrStrLen(CSR_BT_AV_PROVIDER_NAME);

    if( length != 0 )
    {
        CsrMemCpy(record+(*index), attrId, sizeof(attrId));
        *index += sizeof(attrId);
        record[*index-1] = length;
        CsrMemCpy(record+(*index), CSR_BT_AV_PROVIDER_NAME, length);
        *index = (CsrUint16)(*index + length);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      addServiceName
 *
 *  DESCRIPTION
 *      Add a 'provider name' to the reference service record
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
static void addServiceName(CsrUint16 *index, CsrUint8 *record)
{
    const CsrUint8 attrId[5] = { 0x09, 0x01, 0x00, 0x25, 0x00};
    CsrUint8 length;

    length = (CsrUint8) CsrStrLen(CSR_BT_AV_SERVICE_NAME);

    if( length != 0 )
    {
        CsrMemCpy(record+(*index), attrId, sizeof(attrId));
        *index += sizeof(attrId);
        record[*index-1] = length;
        CsrMemCpy(record+(*index), CSR_BT_AV_SERVICE_NAME, length);
        *index = (CsrUint16)(*index + length);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      buildServiceRecord
 *
 *  DESCRIPTION
 *      Build a service record for the specified role
 *
 *  RETURNS
 *      Size of the created service record on success, otherwise 0
 *
 *---------------------------------------------------------------------------*/
static CsrUint16 buildServiceRecord(CsrBtAvRole role, CsrUint8 **record)
{
    CsrUint16 index, num_rec_bytes = 0;

    if( CsrStrLen(CSR_BT_AV_PROVIDER_NAME) != 0)
    {
        num_rec_bytes = (CsrUint16) (num_rec_bytes + 5 + CsrStrLen(CSR_BT_AV_PROVIDER_NAME));
    }
    if( CsrStrLen(CSR_BT_AV_SERVICE_NAME) != 0)
    {
        num_rec_bytes = (CsrUint16) (num_rec_bytes + 5 + CsrStrLen(CSR_BT_AV_SERVICE_NAME));
    }

    switch (role)
    {
        case CSR_BT_AV_AUDIO_SOURCE:
            {
                /* build the A2DP SRC SDP record in SDS */
                index = (CsrUint16) sizeof(sdsA2dSrcServiceRecord);
                num_rec_bytes = (CsrUint16) (num_rec_bytes + index);
                *record = (CsrUint8 *) CsrPmemAlloc(num_rec_bytes );
                CsrMemCpy(*record, sdsA2dSrcServiceRecord, index);
                *(*record + AV_SUPPORTED_FEATURES_INDEX) = (CsrUint8) (CSR_BT_AV_SUPPORTED_FEATURES>>8);
                *(*record + AV_SUPPORTED_FEATURES_INDEX + 1) = (CsrUint8) (CSR_BT_AV_SUPPORTED_FEATURES & 0x00FF);
                break;
            }
        case CSR_BT_AV_AUDIO_SINK:
            {
                /* build the A2DP SINK SDP record in SDS */
                index = (CsrUint16) sizeof(sdsA2dSnkServiceRecord);
                num_rec_bytes = (CsrUint16) (num_rec_bytes + index);
                *record = (CsrUint8 *) CsrPmemAlloc(num_rec_bytes );
                CsrMemCpy(*record, sdsA2dSnkServiceRecord, index);
                *(*record + AV_SUPPORTED_FEATURES_INDEX) = (CsrUint8) (CSR_BT_AV_SUPPORTED_FEATURES>>8);
                *(*record + AV_SUPPORTED_FEATURES_INDEX + 1) = (CsrUint8) (CSR_BT_AV_SUPPORTED_FEATURES & 0x00FF);
                break;
            }
        case CSR_BT_AV_VIDEO_SOURCE:
            {
                /* build the VDP SRC SDP record in SDS */
                index = (CsrUint16) sizeof(sdsVdpSrcServiceRecord);
                num_rec_bytes = (CsrUint16) (num_rec_bytes + index);;
                *record = (CsrUint8 *) CsrPmemAlloc(num_rec_bytes);
                CsrMemCpy(*record,sdsVdpSrcServiceRecord, index);
                break;
            }
        case CSR_BT_AV_VIDEO_SINK:
            {
                /* build the VDP SINK SDP record in SDS */
                index = (CsrUint16) sizeof(sdsVdpSnkServiceRecord);
                num_rec_bytes = (CsrUint16) (num_rec_bytes + index);
                *record = (CsrUint8 *) CsrPmemAlloc(num_rec_bytes);
                CsrMemCpy(*record,sdsVdpSnkServiceRecord, index);
                break;
            }
        default:
            {
                /* invalid role */
                return 0;
            }
    }

    addProviderName( &index, *record);
    addServiceName( &index, *record);

    return num_rec_bytes;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvRegisterRetry
 *
 *  DESCRIPTION
 *      Re-attempt a service registration
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvRegisterRetry(CsrUint16 dummy, av_instData_t *instData)
{
    CsrUint8 *record;
    CsrUint16 size;

    CSR_UNUSED(dummy);
    size = buildServiceRecord(instData->role, &record);

    CsrBtCmSdsRegisterReqSend(CSR_BT_AV_IFACEQUEUE, record, size);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvUnRegisterRetry
 *
 *  DESCRIPTION
 *      Re-attempt a service deregistration
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvUnRegisterRetry(CsrUint16 dummy, av_instData_t *instData)
{
    /* retry to unregister */
    CSR_UNUSED(dummy);
    CsrBtCmSdsUnRegisterReqSend(CSR_BT_AV_IFACEQUEUE, instData->serviceRecordHandles[instData->role]);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvActivateReqHandler
 *
 *  DESCRIPTION
 *      Handle a request for AV service activation
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvActivateReqHandler(av_instData_t *instData)
{
    CsrUint8 *record;
    CsrUint16 size;
    CsrBtAvActivateReq *prim;

    prim = (CsrBtAvActivateReq *) instData->recvMsgP;

    instData->appHandle = prim->phandle;

    if(instData->roleRegister[prim->localRole] == 0)
    {
        /* the service is not registered, register it */
        if ( (size = buildServiceRecord(prim->localRole, &record)) > 0)
        {
            instData->role = prim->localRole;

            instData->registerRetryCount = 0;

            CsrBtCmSdsRegisterReqSend(CSR_BT_AV_IFACEQUEUE, record, size);
        }
        else
        {
            CsrBtAvActivateCfmSend(prim->phandle, CSR_BT_RESULT_CODE_AV_INVALID_ROLE,
                CSR_BT_SUPPLIER_AV);
        }
    }
    else
    {
        /* this service is already registered */
        instData->roleRegister[prim->localRole]++;

        if( !instData->isConnectable
            && (getNumIncomingCon(instData) < getNumActivations(instData->roleRegister)) )
        {
            CsrBtAvMakeConnectable(instData);
        }

        CsrBtAvActivateCfmSend(instData->appHandle, CSR_BT_RESULT_CODE_AV_SUCCESS,
            CSR_BT_SUPPLIER_AV);

        CsrBtAvStatusIndSend(instData, CSR_BT_AV_ACTIVATE_CFM, prim->localRole, 0);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvDeactivateReqHandler
 *
 *  DESCRIPTION
 *      Handle a request for AV service deactivation
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvDeactivateReqHandler( av_instData_t *instData)
{
    CsrBtAvDeactivateReq *prim;

    prim = (CsrBtAvDeactivateReq *) instData->recvMsgP;
    if ( (prim->localRole <= CSR_BT_AV_VIDEO_SINK)
         && (instData->roleRegister[prim->localRole] > 0) )
    {
        instData->role = prim->localRole;

        if( instData->isConnectable && (getNumActivations(instData->roleRegister) - 1) <= getNumIncomingCon(instData))
        {
            CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_AV_IFACEQUEUE, CSR_BT_AVDTP_PSM);
            instData->doDeactivate = TRUE;
        }
        else
        {
            CsrBtAvDeactivateCfmSend(instData->appHandle, CSR_BT_RESULT_CODE_AV_SUCCESS,
                CSR_BT_SUPPLIER_AV);
            CsrBtAvStatusIndSend(instData, CSR_BT_AV_DEACTIVATE_CFM, instData->role, 0);
        }

        instData->roleRegister[instData->role]--;
        if(instData->roleRegister[instData->role] == 0)
        {
            instData->registerRetryCount = 0;
            CsrBtCmSdsUnRegisterReqSend(CSR_BT_AV_IFACEQUEUE, instData->serviceRecordHandles[instData->role]);
        }
    }
    else
    {
        /* this role has not been registered, reject */
        CsrBtAvDeactivateCfmSend(instData->appHandle, CSR_BT_RESULT_CODE_AV_INVALID_ROLE,
            CSR_BT_SUPPLIER_AV);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmSdsRegisterCfmHandler
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_SDS_REGISTER_CFM
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmSdsRegisterCfmHandler( av_instData_t *instData)
{
    CsrBtCmSdsRegisterCfm *prim;

    prim = (CsrBtCmSdsRegisterCfm *) instData->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        instData->serviceRecordHandles[instData->role] = prim->serviceRecHandle;

        instData->roleRegister[instData->role]++;

        if( !instData->isConnectable
            && (getNumIncomingCon(instData) < getNumActivations(instData->roleRegister)) )
        {
            CsrBtAvMakeConnectable(instData);
        }

        CsrBtAvActivateCfmSend(instData->appHandle, CSR_BT_RESULT_CODE_AV_SUCCESS,
            CSR_BT_SUPPLIER_AV);

        CsrBtAvStatusIndSend(instData, CSR_BT_AV_ACTIVATE_CFM, instData->role, 0);
    }
    else
    {
        if (instData->registerRetryCount < AV_SDP_REG_RETRIES)
        {
            CsrSchedTimerSet( 500000, (void (*) (CsrUint16, void *)) CsrBtAvRegisterRetry, 0, (void *) instData);
            instData->registerRetryCount++;
        }
        else
        {
            instData->registerRetryCount = 0;
            CsrBtAvActivateCfmSend(instData->appHandle, prim->resultCode, prim->resultSupplier);
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmSdsUnregisterCfmHandler
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_SDS_UNREGISTER_CFM
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmSdsUnregisterCfmHandler( av_instData_t *instData)
{
    CsrBtCmSdsUnregisterCfm                    *prim;

    prim = (CsrBtCmSdsUnregisterCfm *) (instData->recvMsgP);

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        instData->serviceRecordHandles[instData->role] = 0;
    }
    else
    {
        if (instData->registerRetryCount < AV_SDP_REG_RETRIES)
        {
            CsrSchedTimerSet( 500000, (void (*) (CsrUint16, void *)) CsrBtAvUnRegisterRetry, 0, (void *) instData);
            instData->registerRetryCount++;
        }
        else
        {
            /* give up trying to unregister */
            instData->registerRetryCount = 0;
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvRegisterStreamHandleReqHandler
 *
 *  DESCRIPTION
 *      Handle a request for registering an application stream handler task
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvRegisterStreamHandleReqHandler( av_instData_t *instData)
{
    CsrBtAvRegisterStreamHandleReq *prim;

    prim = (CsrBtAvRegisterStreamHandleReq*) instData->recvMsgP;

    instData->streamAppHandle = prim->streamHandle;
    CsrBtAvRegisterStreamHandleCfmSend(prim->streamHandle);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmL2caRegisterCfmHandler
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_L2CA_REGISTER_CFM
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmL2caRegisterCfmHandler(av_instData_t *instData)
{
    CsrBtCmL2caRegisterCfm        *prim;

    prim = (CsrBtCmL2caRegisterCfm *) instData->recvMsgP;

    if (prim->localPsm == CSR_BT_AVDTP_PSM)
    {
        instData->state = READY_S;

        CsrBtAvSendHouseCleaning(instData);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmSdsRegisterCfmHandlerCleanup
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_SDS_REGISTER_CFM
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmSdsRegisterCfmHandlerCleanup( av_instData_t *instData)
{
    CsrBtCmSdsRegisterCfm *prim;

    prim = (CsrBtCmSdsRegisterCfm *) instData->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtCmSdsUnRegisterReqSend(CSR_BT_AV_IFACEQUEUE, prim->serviceRecHandle);

        instData->serviceRecordHandles[instData->role] = prim->serviceRecHandle;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvCmSdsUnregisterCfmHandlerCleanup
 *
 *  DESCRIPTION
 *      Handle a CSR_BT_CM_SDS_UNREGISTER_CFM
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvCmSdsUnregisterCfmHandlerCleanup( av_instData_t *instData)
{
    CsrUintFast8 i;
    CsrBtCmSdsUnregisterCfm *prim;

    prim = (CsrBtCmSdsUnregisterCfm *) (instData->recvMsgP);

    for(i=0; i<4; i++)
    {
        if (prim->serviceRecHandle == instData->serviceRecordHandles[i])
        {
            instData->serviceRecordHandles[i] = 0;
            break;
        }
    }

    CsrBtAvIsCleanUpFinished(instData);
}



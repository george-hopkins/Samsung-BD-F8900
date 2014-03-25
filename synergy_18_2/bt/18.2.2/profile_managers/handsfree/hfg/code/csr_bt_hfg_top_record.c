/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_hfg_prim.h"
#include "csr_bt_hfg_main.h"
#include "csr_bt_hfg_proto.h"

/* Index to network parameter and supported features parameter The
 * network parameter is a single byte. The supported features are 2
 * bytes, and the value must be stored with MSB first */
#define CSR_BT_AG_SERVER_CHANNEL_INDEX                 ((CsrUint8) 31)
#define CSR_BT_HFG_SERVER_CHANNEL_INDEX                ((CsrUint8) 27)
#define CSR_BT_HFG_SERVICE_NAME_LEN_INDEX              ((CsrUint8) 45)
#define CSR_BT_HFG_NETWORK_INDEX                       ((CsrUint8)(CSR_BT_HFG_SERVICE_NAME_LEN_INDEX + 5))
#define CSR_BT_HFG_SUPPORTED_FEATURES_INDEX            ((CsrUint8)(CSR_BT_HFG_NETWORK_INDEX + 5))

static const CsrUint8 sdsHfgServiceRecord_1[] =
{
    /* Service class ID list */
    0x09, 0x00, 0x01,      /* AttrID , ServiceClassIDList */
    0x35, 0x06,            /* 6 bytes in total DataElSeq */

    /* AG Handsfree */
    0x19, 0x11, 0x1F,      /* 2 byte UUID, Service class = AG Handsfree = 0x111F */

    /* Generic Audio */
    0x19, 0x12, 0x03,      /* 2 byte UUID, Service class = Generic Audio = 0x1203 */

    /* protocol descriptor list */
    0x09, 0x00, 0x04,      /* AttrId ProtocolDescriptorList */
    0x35, 0x0C,            /* 12 bytes in total DataElSeq */

    /* L2CAP */
    0x35, 0x03,            /* 3 bytes in DataElSeq */
    0x19, 0x01, 0x00,      /* 2 byte UUID, Protocol = L2CAP */

    /* RFCOMM */
    0x35, 0x05,            /* 5 bytes in DataElSeq */
    0x19, 0x00, 0x03,      /* 2 byte UUID Protocol = RFCOMM */
    0x08, 0x00,            /* 1 byte UINT - server channel template value 0 - to be filled in by app (index:27) */

    /* Bluetooth profile descriptor list */
    0x09, 0x00, 0x09,      /* AttrId ProtocolDescriptorList */
    0x35, 0x08,            /* 8 bytes in total DataElSeq */

    /* Supported profile */
    0x35, 0x06,            /* 6 bytes in DataElSeq */
    0x19, 0x11, 0x1E,      /* 2 byte UUID, Supported profile = Handsfree = 0x111E */

    /* Profile version */
    0x09, 0x01, 0x06,      /* 2 byte UINT - Profile version = 0x0106 = HFP spec version 1.6 */

    /* Service name */
    0x09, 0x01, 0x00,      /* AttrId - Service Name. Use language base attribute 0x0100 (primary language) */
    0x25, 0x00             /* Service name length - string must be copied in */
};

static const CsrUint8 sdsHfgServiceRecord_2[] =
{
    /* Network */
    0x09, 0x03, 0x01,      /* AttrId - Network. Value 0x0301) */
    0x08, 0x01,            /* 1 byte UINT - Network capabilities 1 - default set to 1 (GSM like) */

    /* Supported Features */
    0x09, 0x03, 0x11,      /* AttrId - Supported features. Value 0x0311) */
    0x09, 0x00, 0x00,      /* 2 byte UINT - Supported features 9 - default set to 9 (in-band ring and three way calling) */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,    /* AttrId = BrowseGroupList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,    /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

static const CsrUint8 sdsAgServiceRecord[] =
{
    /* Service class ID list */
    0x09, 0x00, 0x01,      /* AttrID , ServiceClassIDList */
    0x35, 0x06,            /* 6 bytes in total DataElSeq */
    0x19, 0x11, 0x12,      /* 2 byte UUID, Service class = Voice gateway = 0x1112 */
    0x19, 0x12, 0x03,      /* 2 byte UUID Service class = GenericAudio = 0x1203 */

    /* protocol descriptor list */
    0x09, 0x00, 0x04,      /* AttrId ProtocolDescriptorList */
    0x35, 0x10,            /* 16 Bytes in total DataElSeq */
    0x35, 0x05,            /* 5 Bytes in DataElSeq */
    0x1a, 0x00, 0x00, 0x01, 0x00,

    0x35, 0x07,            /* 7 Bytes in DataElSeq */
    0x1a, 0x00, 0x00, 0x00, 0x03, 0x08,
    0x01,                  /* server channel */

    /* Bluetooth profile descriptor list */
    0x09, 0x00, 0x09,      /* AttrId ProtocolDescriptorList */
    0x35, 0x0a,            /* 10 bytes in total DataElSeq */

    /* Supported profile */
    0x35, 0x08,                   /* 8 bytes in DataElSeq */
    0x1a, 0x00, 0x00, 0x11, 0x08, /* CSR_BT_HS_PROFILE_UUID = 0x1108 */

    /* Profile version */
    0x09, 0x01, 0x02, /* Profile version = 0x0102 = HSP version 1.02 */
    0x09, 0x01, 0x00, 0x25, 0x08, 'V', 'o', 'i', 'c', 'e', ' ', 'G', 'W',

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,    /* AttrId = BrowseGroupList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,    /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

/* Local downstream: Send HFG SDS register */
static void csrBtHfgSendHfgSdsRegisterReq(HfgMainInstance_t *inst, CsrUint8 index)
{
    CsrUint8 *record;
    CsrUint16 length;
    CsrUint8 strl;

    /* Copy the record */
    strl = (CsrUint8)CsrUtf8StringLengthInBytes(inst->localName);
    length = (sizeof(sdsHfgServiceRecord_1) +
              sizeof(sdsHfgServiceRecord_2) +
              strl);
    record = (CsrUint8*)CsrPmemAlloc(length);

    /* Copy in part 1, service string and part 2 */
    CsrMemCpy(record,
           sdsHfgServiceRecord_1,
           sizeof(sdsHfgServiceRecord_1));
    CsrMemCpy(record + sizeof(sdsHfgServiceRecord_1),
           inst->localName,
           strl);
    CsrMemCpy(record + sizeof(sdsHfgServiceRecord_1) + strl,
           sdsHfgServiceRecord_2,
           sizeof(sdsHfgServiceRecord_2));

    /* Patch specific bytes */
    record[CSR_BT_HFG_SERVER_CHANNEL_INDEX] = inst->service[index].chan;
    record[CSR_BT_HFG_SERVICE_NAME_LEN_INDEX] = strl;

    /* The supported features element in the service record should
     * only be bit 0 to 4 of the supported features used in +BRSF:
     * xx */
    record[CSR_BT_HFG_NETWORK_INDEX + strl] = (inst->locSupFeatures & CSR_BT_HFG_SUPPORT_ABILITY_TO_REJECT_CALL
                                        ? 1
                                        : 0);

    /* Mask out least significant byte */
    record[CSR_BT_HFG_SUPPORTED_FEATURES_INDEX + 1 + strl] = (CsrUint8)(inst->locSupFeatures & CSR_BT_HFP_SDP_SUPPORT_MASK);
    if (inst->locSupFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION)
    {
        record[CSR_BT_HFG_SUPPORTED_FEATURES_INDEX + 1 + strl] |= CSR_BT_HFP_SDP_CODEC_NEGOTIATION;
    }
    record[CSR_BT_HFG_SUPPORTED_FEATURES_INDEX + 0 + strl] = 0;

    inst->activeService = index;
    CsrBtCmSdsRegisterReqSend(CSR_BT_HFG_IFACEQUEUE,
                         record,
                         length);
}

/* Local downstream: Send AG SDS register */
static void csrBtHfgSendAgSdsRegisterReq(HfgMainInstance_t *inst, CsrUint8 index)
{
    CsrUint8 *record;
    CsrUint16 len;

    /* Copy the record */
    len = sizeof(sdsAgServiceRecord);
    record = (CsrUint8*)CsrPmemAlloc(len);
    CsrMemCpy(record, sdsAgServiceRecord, len);

    /* Insert the server channel found during register in appropriate
     * place */
    record[CSR_BT_AG_SERVER_CHANNEL_INDEX] = inst->service[index].chan;

    inst->activeService = index;
    CsrBtCmSdsRegisterReqSend(CSR_BT_HFG_IFACEQUEUE, record, len);
}

/* Local: Get number of SDS-active connections */
static CsrUint8 csrBtHfgSdsConnections(HfgMainInstance_t *inst)
{
    CsrUintFast8 i;
    CsrUint8 num;

    num = 0;
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if((inst->linkData[i].state == Activate_s) ||
           (inst->linkData[i].state == Connect_s) ||
           (inst->linkData[i].state == ServiceSearch_s))
        {
            num++;
        }
    }
    return num;
}

/* Local: Count number of fully-SLC-connected connections */
static CsrUint8 csrBtHfgOnlineConnections(HfgMainInstance_t *inst)
{
    CsrUintFast8 i;
    CsrUint8 num;

    num = 0;
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        if(inst->linkData[i].state == Connected_s)
        {
            num++;
        }
    }
    return num;
}

/* Local: Remove all SDS records which is quite simple - check if
 * records are registered and unregister the first one and return */
static CsrBool csrBtHfgRecordRemove(HfgMainInstance_t *inst)
{
    CsrUintFast8 i;

    for(i=0; i<CSR_BT_HFG_NUM_CHANNELS; i++)
    {
        if(inst->service[i].registered)
        {
            inst->activeService = (CsrUint8)i;
            if ((inst->deRegisterTime == 0) || (inst->state == MainDeactivate_s))
            {
                CsrBtCmSdsUnRegisterReqSend(CSR_BT_HFG_IFACEQUEUE, inst->service[i].recHandle);
            }
            else
            {
                HfgInstance_t *link;
                HfgService_t *ser = &(inst->service[i]);
                CsrUintFast8 j;
                for(j=0; j<CSR_BT_HFG_NUM_SERVERS; j++)
                {
                    link = &(inst->linkData[j]);
                    if((link->state == Connected_s) && (link->serverChannel == ser->chan))
                    {
                        link->deRegisterTimer = CsrSchedTimerSet((CsrTime)(inst->deRegisterTime * 1000000),CsrBtHfgDeRegisterTimeout,
                                         (CsrUint16)ser->chan,
                                         (void*)inst);
                        return TRUE;
                    }
                }
                /* if the function has not returned yet, it means that no link was connected.... just de-register the record */
                CsrBtCmSdsUnRegisterReqSend(CSR_BT_HFG_IFACEQUEUE, inst->service[i].recHandle);
            }
            return TRUE;
        }
    }
    return FALSE;
}

/* Local: Check that the server channels in records are not in use by
 * a "SLC connected" connection. If found, unregister it */
static CsrBool csrBtHfgRecordInUse(HfgMainInstance_t *inst)
{
    HfgInstance_t *link;
    HfgService_t *ser;
    CsrUintFast8 i;
    CsrUintFast8 j;

    for(i=0; i<CSR_BT_HFG_NUM_CHANNELS; i++)
    {
        ser = &(inst->service[i]);

        for(j=0; j<CSR_BT_HFG_NUM_SERVERS; j++)
        {
            link = &(inst->linkData[j]);

            /* Connections in 'connected' or 'CSR_UNUSED' state should not
             * have service-records */
            if(((link->state == Connected_s) || (link->state == Unused_s)) &&
               (link->serverChannel == ser->chan) &&
               (ser->registered))
            {
                inst->activeService = (CsrUint8)i;
                if ((link->state == Connected_s) && (inst->deRegisterTime != 0) && (inst->state != MainDeactivate_s))
                {
                    link->deRegisterTimer = CsrSchedTimerSet((CsrTime)(inst->deRegisterTime * 1000000),CsrBtHfgDeRegisterTimeout,
                                         (CsrUint16)ser->chan,(void*)inst);
                }
                else
                {
                    CsrBtCmSdsUnRegisterReqSend(CSR_BT_HFG_IFACEQUEUE,
                                           ser->recHandle);
                }
                return TRUE;
            }
        }
    }

    /* No match found */
    return FALSE;
}

/* Local: Register unregistered service records. Before this one is
 * called records that are no longer usable must have been removed */
static CsrBool csrBtHfgRecordSetup(HfgMainInstance_t *inst)
{
    CsrUintFast8 i;
    CsrUintFast8 j;
    CsrUint8 found;
    HfgService_t *ser;

    /* Scan each 'block' of service structures to see if a whole block
     * is _not_ registered. If so, register the given record */
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        found = 0;
        ser = NULL;

        for(j=(i*CSR_BT_HFG_NUM_RECORDS); j<((i+1)*CSR_BT_HFG_NUM_RECORDS); j++)
        {
            ser = &(inst->service[j]);
            found = found | ser->registered;
            if(found != 0)
            {
                break;
            }
        }

        if(found == 0)
        {
            /* An unregistered block found */

            for(j=(i*CSR_BT_HFG_NUM_RECORDS); j<((i+1)*CSR_BT_HFG_NUM_RECORDS); j++)
            {
                /* Scan for the first CSR_UNUSED serverchannel and use
                 * that */

                ser = &(inst->service[j]);
                if(!CsrBtHfgOccupiedServerChannel(inst, ser->chan))
                {
                    /* Unregistered and CSR_UNUSED serverchannel found,
                     * register it */
                    if(ser->type == CSR_BT_HFG_CONNECTION_AG)
                    {
                        csrBtHfgSendAgSdsRegisterReq(inst,(CsrUint8) j);
                    }
                    else
                    {
                        csrBtHfgSendHfgSdsRegisterReq(inst,(CsrUint8) j);
                    }
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

/* Service record update function. This function will use the number
 * of active connections and the state of the two service records to
 * update the active SDS records and server channels. Return TRUE if a
 * request was sent, otherwise FALSE (ie. false also means SDS setup
 * has been completed) */
CsrBool CsrBtHfgRecordUpdate(HfgMainInstance_t *inst)
{
    /* General plan:
     *
     * Take down record(s) and bail out if:
     * - Main state is null/deactivate
     * - Maximum number of SLC-connections reached
     * - No connections are in SDS state (activated/connect/search)
     * - Bail out if records already down but otherwise match above
     *
     * Scan connections and records
     * - Take down if connection that uses it is in non-SDS-state
     *
     * Setup records
     * - Register record if a whole block (AG/HFG) is not registered
 */

    /* Check state, number of SLC-connections and number of records
     * requiring a SDS-record */
    if((inst->state == MainNull_s) ||
       (inst->state == MainDeactivate_s) ||
       (csrBtHfgOnlineConnections(inst) >= inst->maxConnections) ||
       (csrBtHfgSdsConnections(inst) == 0))
    {
        /* State is not ready for connections */
        if(csrBtHfgRecordRemove(inst))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    /* Zero or more - remove invalid records first */
    if(csrBtHfgRecordInUse(inst))
    {
        return TRUE;
    }

    /* Setup missing records */
    if(csrBtHfgRecordSetup(inst))
    {
        return TRUE;
    }

    /* Everything is fine */
    return FALSE;
}

/* Update the global instance data for a received SDS register confirm
 * success primitive */
void CsrBtHfgRecordRegisterConfirm(HfgMainInstance_t *inst,
                              CsrUint32 recHandle)
{
    if(inst->activeService<CSR_BT_HFG_NUM_CHANNELS)
    {
        HfgService_t *ser;
        ser = &(inst->service[inst->activeService]);
        ser->registered = TRUE;
        ser->recHandle = recHandle;
    }
    else
    {
        /* This shouldn't happen */
    }
    inst->activeService = CSR_BT_HFG_NO_CONID;
}

/* Update the global instance data for a received SDS unregister
 * confirm success primitive */
void CsrBtHfgRecordUnregisterConfirm(HfgMainInstance_t *inst,
                                CsrUint32 recHandle)
{
    CsrUintFast8 i;

    for (i=0; i<CSR_BT_HFG_NUM_CHANNELS; i++)
    {
        HfgService_t *ser;
        ser = &(inst->service[i]);
        if (ser->recHandle == recHandle)
        {
            ser->registered = FALSE;
            ser->recHandle = 0;
            i = CSR_BT_HFG_NUM_CHANNELS;
        }
    }

    inst->activeService = CSR_BT_HFG_NO_CONID;
}

/* Do we still have a reference to a specific record? */
CsrBool CsrBtHfgRecordExists(HfgMainInstance_t *inst,
                                CsrUint32 recHandle)
{
    CsrUintFast8 i;

    for (i=0; i<CSR_BT_HFG_NUM_CHANNELS; i++)
    {
        HfgService_t *ser;
        ser = &(inst->service[i]);
        if (ser->recHandle == recHandle)
        {
            return TRUE;
        }
    }
    return FALSE;
}



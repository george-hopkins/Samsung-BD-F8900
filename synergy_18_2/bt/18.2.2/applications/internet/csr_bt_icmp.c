/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_internet_primitives.h"
#include "csr_bt_ip_prim.h"
#include "csr_bt_icmp_prim.h"
#include "csr_bt_internet_checksum.h"
#include "csr_pmem.h"
#include "csr_bt_tasks.h"
#include "csr_sched.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define SECONDS_BEFORE_PING_UNREACHABLE  20
#define MAX_OUTSTANDING_PING_REQUESTS    10
#define INVALID_APPLICATION_QUEUE        0x0000

typedef struct
{
    CsrUint16 receivedEvent;
    void * receivedMessage;
    CsrUint16 currentNumberOfOutstandingPings;
    CsrSchedQid pingApplicationQueue[MAX_OUTSTANDING_PING_REQUESTS];
    CsrTime pingTransmitTime[MAX_OUTSTANDING_PING_REQUESTS];
    CsrUint8 pingUnreachableTimeCounter[MAX_OUTSTANDING_PING_REQUESTS];
    CsrSchedTid pingTimerId;
} IcmpInstanceData;


static void pingTimer(CsrUint16 unused, void * instanceDataPointer)
{
    IcmpInstanceData * instanceData;
    CsrUint16 index;

    instanceData = (IcmpInstanceData *) instanceDataPointer;

    for (index = 0; index < MAX_OUTSTANDING_PING_REQUESTS; index++)
    {
        if (instanceData->pingApplicationQueue[index] != INVALID_APPLICATION_QUEUE)
        {
            if (--instanceData->pingUnreachableTimeCounter[index] == 0)
            {
                CsrBtIcmpPingCfm * pingCfm;

                pingCfm = (CsrBtIcmpPingCfm *) CsrPmemAlloc(sizeof(CsrBtIcmpPingCfm));
                pingCfm->type = CSR_BT_ICMP_PING_CFM;
                pingCfm->result = CSR_BT_ICMP_RESULT_UNREACHABLE;
                pingCfm->responseTime = 0;

                CsrSchedMessagePut(instanceData->pingApplicationQueue[index], CSR_BT_ICMP_PRIM, pingCfm);

                instanceData->pingApplicationQueue[index] = INVALID_APPLICATION_QUEUE;
                instanceData->currentNumberOfOutstandingPings--;
            }
        }
    }
    if (instanceData->currentNumberOfOutstandingPings)
    {
        instanceData->pingTimerId = CsrSchedTimerSet(1000000, pingTimer, 0, (void *) instanceData);
    }
    else
    {
        instanceData->pingTimerId = 0;
    }
}

static void handleIcmpIpReception(IcmpInstanceData * instanceData)
{
    CsrBtIpDataInd * ipDataInd;

    ipDataInd = (CsrBtIpDataInd *) instanceData->receivedMessage;

    if (CalculateCheckSum(ipDataInd->ipDataLength, (CsrUint8 *) ipDataInd->ipData) == 0x0000)
    {
        switch (CSR_BT_LAN_WORD(ipDataInd->ipData[0]) & 0xFF00)
        {
            case 0x0800:    /* type: echo request */
                {
                    CsrBtIpDataReq * ipDataReq;
                    CsrUint16 newCheckSum;

                    if (ipDataInd->ipDataLength & 0x0001)
                    {
                        ipDataInd->ipData[ipDataInd->ipDataLength / 2] = CSR_BT_LAN_WORD(CSR_BT_LAN_WORD(ipDataInd->ipData[ipDataInd->ipDataLength / 2]) & 0xFF00);
                    }
                    newCheckSum = CalculateCheckSum(ipDataInd->ipDataLength, (CsrUint8 *) ipDataInd->ipData);

                    ipDataReq = (CsrBtIpDataReq *) CsrPmemAlloc(sizeof(CsrBtIpDataReq));
                    ipDataReq->type = CSR_BT_IP_DATA_REQ;
                    ipDataReq->typeOfService = 0;
                    ipDataReq->timeToLive = 128;
                    ipDataReq->protocol = 1;
                    ipDataReq->destinationAddress[0] = ipDataInd->ipHeader[6];
                    ipDataReq->destinationAddress[1] = ipDataInd->ipHeader[7];
                    ipDataReq->sourceAddress[0] = ipDataInd->ipHeader[8];
                    ipDataReq->sourceAddress[1] = ipDataInd->ipHeader[9];
                    ipDataReq->ipDataLength = ipDataInd->ipDataLength;
                    ipDataReq->ipData = ipDataInd->ipData;
                    ipDataInd->ipData = NULL;
                    ipDataReq->ipData[0] = CSR_BT_LAN_WORD(0x0000); /* Echo reply */
                    ipDataReq->ipData[1] = CSR_BT_LAN_WORD(0x0000); /* reset check sum */
                    newCheckSum = CalculateCheckSum(ipDataReq->ipDataLength, (CsrUint8 *) ipDataReq->ipData);
                    ipDataReq->ipData[1] = CSR_BT_LAN_WORD(newCheckSum);
                    CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, ipDataReq);
                    break;
                }

            case 0x0000:    /* type: echo response */
                {
                    CsrUint16 index;

                    index = CSR_BT_LAN_WORD(ipDataInd->ipData[2]);
                    if ((index < MAX_OUTSTANDING_PING_REQUESTS) && (instanceData->pingApplicationQueue[index] != INVALID_APPLICATION_QUEUE))
                    {
                        CsrBtIcmpPingCfm * pingCfm;
                        CsrTime currentTime;

                        currentTime = CsrTimeGet(NULL);

                        /* TODO: verify data response is correct */

                        pingCfm = (CsrBtIcmpPingCfm *) CsrPmemAlloc(sizeof(CsrBtIcmpPingCfm));
                        pingCfm->type = CSR_BT_ICMP_PING_CFM;
                        pingCfm->result = CSR_BT_ICMP_RESULT_SUCCESS;
                        pingCfm->responseTime = CsrTimeSub(currentTime, instanceData->pingTransmitTime[index]);

                        CsrSchedMessagePut(instanceData->pingApplicationQueue[index], CSR_BT_ICMP_PRIM, pingCfm);

                        instanceData->pingApplicationQueue[index] = INVALID_APPLICATION_QUEUE;
                        instanceData->currentNumberOfOutstandingPings--;
                        if ((instanceData->currentNumberOfOutstandingPings == 0) && (instanceData->pingTimerId != 0))
                        {
                            CsrSchedTimerCancel(instanceData->pingTimerId, NULL, NULL);
                            instanceData->pingTimerId = 0;
                        }
                    }
                    break;
                }

            case 0x0300:    /* type: destination unreachable */
                switch (CSR_BT_LAN_WORD(ipDataInd->ipData[0]) & 0x00FF)
                {
                    case 0x0000:    /* code: net unreachable */
                    case 0x0001:    /* code: host unreachable */
                    case 0x0002:    /* code: protocol unreachable */
                    case 0x0003:    /* code: port unreachable */
                    case 0x0004:    /* code: fragmentation needed and DF set */
                    case 0x0005:    /* code: source route failed */
                        {
                            switch (CSR_BT_LAN_WORD(ipDataInd->ipData[4 + 4]) & 0x00FF)    /* switch on IP protocol field */
                            {
                                case CSR_BT_IP_PROTOCOL_UDP:
                                    {
                                        instanceData->receivedMessage = NULL;
                                        ipDataInd->type = CSR_BT_ICMP_PORT_UNREACHABLE_IND;
                                        CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_ICMP_PRIM, ipDataInd);
                                        break;
                                    }

                                case CSR_BT_IP_PROTOCOL_ICMP:
                                    {
                                        CsrUint16 originalDataOffset;

                                        originalDataOffset = 2 * ((CSR_BT_LAN_WORD(ipDataInd->ipData[4 + 0]) >> 8) & 0x000F);
                                        if (CSR_BT_LAN_WORD(ipDataInd->ipData[4 + originalDataOffset + 0]) == 0x0800) /* echo request */
                                        {
                                            CsrUint16 index;

                                            index = CSR_BT_LAN_WORD(ipDataInd->ipData[4 + originalDataOffset + 2]);
                                            if ((index < MAX_OUTSTANDING_PING_REQUESTS) && (instanceData->pingApplicationQueue[index] != INVALID_APPLICATION_QUEUE))
                                            {
                                                CsrBtIcmpPingCfm * pingCfm;

                                                pingCfm = (CsrBtIcmpPingCfm *) CsrPmemAlloc(sizeof(CsrBtIcmpPingCfm));
                                                pingCfm->type = CSR_BT_ICMP_PING_CFM;
                                                pingCfm->result = CSR_BT_ICMP_RESULT_UNREACHABLE;
                                                pingCfm->responseTime = 0;

                                                CsrSchedMessagePut(instanceData->pingApplicationQueue[index], CSR_BT_ICMP_PRIM, pingCfm);

                                                instanceData->pingApplicationQueue[index] = INVALID_APPLICATION_QUEUE;
                                                instanceData->currentNumberOfOutstandingPings--;
                                            }
                                        }
                                        break;
                                    }

                                    /* TODO: handle other protocols */
                            }
                            break;
                        }
                }
                break;

            default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("ICMP",
                                        (CsrUint16) CSR_BT_LAN_WORD(ipDataInd->ipData[0]),
                                        0,
                                        0);
#endif
                break;
        }
    }
    if ((instanceData->receivedMessage != NULL) && (ipDataInd->ipData != NULL))
    {
        CsrPmemFree(ipDataInd->ipData);
    }
}

void CsrBtIcmpInit(void ** gash)
{
    IcmpInstanceData * instanceData;
    CsrUint16 index;

    instanceData = (IcmpInstanceData *) CsrPmemAlloc(sizeof(IcmpInstanceData));
    *gash = (void *) instanceData;
    instanceData->currentNumberOfOutstandingPings = 0;
    for (index = 0; index < MAX_OUTSTANDING_PING_REQUESTS; index++)
    {
        instanceData->pingApplicationQueue[index] = INVALID_APPLICATION_QUEUE;
    }
    instanceData->pingTimerId = 0;
}

void CsrBtIcmpHandler(void ** gash)
{
    IcmpInstanceData * instanceData;

    instanceData = (IcmpInstanceData *) *gash;

    if (CsrSchedMessageGet(&instanceData->receivedEvent, &instanceData->receivedMessage))
    {
        switch (instanceData->receivedEvent)
        {
            case CSR_BT_IP_PRIM:
                switch (*((CsrUint16 *) instanceData->receivedMessage))
                {
                    case CSR_BT_IP_DATA_IND:
                        handleIcmpIpReception(instanceData);
                        break;

                    case CSR_BT_IP_SHUTDOWN_IND:
                        {
                            CsrUint16 index;

                            for (index = 0; index < MAX_OUTSTANDING_PING_REQUESTS; index++)
                            {
                                if (instanceData->pingApplicationQueue[index] != INVALID_APPLICATION_QUEUE)
                                {
                                    CsrBtIcmpPingCfm * pingCfm;

                                    pingCfm = (CsrBtIcmpPingCfm *) CsrPmemAlloc(sizeof(CsrBtIcmpPingCfm));
                                    pingCfm->type = CSR_BT_ICMP_PING_CFM;
                                    pingCfm->result = CSR_BT_ICMP_RESULT_SHUTDOWN;
                                    pingCfm->responseTime = 0;

                                    CsrSchedMessagePut(instanceData->pingApplicationQueue[index], CSR_BT_ICMP_PRIM, pingCfm);

                                    instanceData->pingApplicationQueue[index] = INVALID_APPLICATION_QUEUE;
                                }
                            }
                            instanceData->currentNumberOfOutstandingPings = 0;
                            if (instanceData->pingTimerId != 0)
                            {
                                CsrSchedTimerCancel(instanceData->pingTimerId, NULL, NULL);
                                instanceData->pingTimerId = 0;
                            }
                            break;
                        }

                    default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("ICMP",
                                        instanceData->receivedEvent,
                                        *(CsrUint16*)instanceData->receivedMessage,
                                        0);
#endif
                        break;
                }
                break;

            case CSR_BT_ICMP_PRIM:
                switch (*((CsrUint16 *) instanceData->receivedMessage))
                {
                    case CSR_BT_ICMP_PING_REQ:
                        if (instanceData->currentNumberOfOutstandingPings >= MAX_OUTSTANDING_PING_REQUESTS)
                        {
                            CsrBtIcmpPingReq * pingReq;
                            CsrBtIcmpPingCfm * pingCfm;

                            pingReq = (CsrBtIcmpPingReq *) instanceData->receivedMessage;
                            pingCfm = (CsrBtIcmpPingCfm *) CsrPmemAlloc(sizeof(CsrBtIcmpPingCfm));
                            pingCfm->type = CSR_BT_ICMP_PING_CFM;
                            pingCfm->result = CSR_BT_ICMP_RESULT_BUSY;
                            pingCfm->responseTime = 0;

                            CsrSchedMessagePut(pingReq->applicationQueue, CSR_BT_ICMP_PRIM, pingCfm);
                        }
                        else
                        {
                            CsrBtIcmpPingReq * pingReq;
                            CsrBtIpDataReq * ipDataReq;
                            CsrUint16 newCheckSum;
                            CsrUint8 * data;
                            CsrUint16 index;
                            CsrUint16 dataIndex;

                            instanceData->currentNumberOfOutstandingPings++;

                            pingReq = (CsrBtIcmpPingReq *) instanceData->receivedMessage;
                            for (index = 0; index < MAX_OUTSTANDING_PING_REQUESTS; index++)
                            {
                                if (instanceData->pingApplicationQueue[index] == INVALID_APPLICATION_QUEUE)
                                {
                                    break;
                                }
                            }
                            instanceData->pingApplicationQueue[index] = pingReq->applicationQueue;

                            ipDataReq = (CsrBtIpDataReq *) CsrPmemAlloc(sizeof(CsrBtIpDataReq));
                            ipDataReq->type = CSR_BT_IP_DATA_REQ;
                            ipDataReq->typeOfService = 0;
                            ipDataReq->timeToLive = 128;
                            ipDataReq->protocol = 1;
                            ipDataReq->destinationAddress[0] = pingReq->destinationAddress[0];
                            ipDataReq->destinationAddress[1] = pingReq->destinationAddress[1];
                            ipDataReq->sourceAddress[0] = 0;
                            ipDataReq->sourceAddress[1] = 0;
                            ipDataReq->ipDataLength = pingReq->dataLength + 8;
                            ipDataReq->ipData = (CsrUint16 *) CsrPmemAlloc(pingReq->dataLength + 8 + (pingReq->dataLength & 0x1));
                            ipDataReq->ipData[0] = CSR_BT_LAN_WORD(0x0800); /* Echo request, code 0 */
                            ipDataReq->ipData[1] = CSR_BT_LAN_WORD(0x0000); /* reset check sum */
                            ipDataReq->ipData[2] = CSR_BT_LAN_WORD(index);  /* identifier points to the queue index */
                            ipDataReq->ipData[3] = CSR_BT_LAN_WORD(0x0000); /* sequence number currently not used */
                            data = (CsrUint8 *) &ipDataReq->ipData[4];
                            for (dataIndex = 0; dataIndex < pingReq->dataLength; dataIndex++)
                            {
                                data[dataIndex] = ((CsrUint8) ('a' + (dataIndex % 23)));    /* [a..w] */
                            }
                            if (pingReq->dataLength & 0x1)
                            {
                                data[dataIndex] = 0;
                            }
                            newCheckSum = CalculateCheckSum(ipDataReq->ipDataLength, (CsrUint8 *) ipDataReq->ipData);
                            ipDataReq->ipData[1] = CSR_BT_LAN_WORD(newCheckSum);
                            CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, ipDataReq);

                            if (instanceData->pingTimerId == 0)
                            {
                                instanceData->pingTimerId = CsrSchedTimerSet(1000000, pingTimer, 0, (void *) instanceData);
                            }
                            instanceData->pingUnreachableTimeCounter[index] = SECONDS_BEFORE_PING_UNREACHABLE;
                            instanceData->pingTransmitTime[index] = CsrTimeGet(NULL);
                        }
                        break;

                    case CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ:
                        {
                            CsrBtIcmpDestinationUnreachableReq * unreachableReq;
                            CsrBtIpDataReq * ipDataReq;
                            CsrUint16 newCheckSum;
                            CsrUint16 index;

                            unreachableReq = (CsrBtIcmpDestinationUnreachableReq *) instanceData->receivedMessage;

                            ipDataReq = (CsrBtIpDataReq *) CsrPmemAlloc(sizeof(CsrBtIpDataReq));
                            ipDataReq->type = CSR_BT_IP_DATA_REQ;
                            ipDataReq->typeOfService = 0;
                            ipDataReq->timeToLive = 128;
                            ipDataReq->protocol = 1;
                            ipDataReq->destinationAddress[0] = unreachableReq->ipHeader[6];
                            ipDataReq->destinationAddress[1] = unreachableReq->ipHeader[7];
                            ipDataReq->sourceAddress[0] = unreachableReq->ipHeader[8];
                            ipDataReq->sourceAddress[1] = unreachableReq->ipHeader[9];
                            ipDataReq->ipDataLength = 8 + unreachableReq->ipHeaderLength + 8;
                            ipDataReq->ipData = (CsrUint16 *) CsrPmemAlloc(ipDataReq->ipDataLength);
                            ipDataReq->ipData[0] = CSR_BT_LAN_WORD(0x0300 | ((CsrUint16) unreachableReq->code)); /* Destination Unreachable, code as specified in request */
                            ipDataReq->ipData[1] = CSR_BT_LAN_WORD(0x0000); /* reset check sum */
                            ipDataReq->ipData[2] = 0x0000;  /* CSR_UNUSED */
                            ipDataReq->ipData[3] = 0x0000;  /* CSR_UNUSED */
                            for (index = 0; index < unreachableReq->ipHeaderLength / 2; index++)
                            {
                                ipDataReq->ipData[4 + index] = unreachableReq->ipHeader[index];
                            }
                            for (index = 0; index < 4; index++)
                            {
                                ipDataReq->ipData[4 + unreachableReq->ipHeaderLength / 2 + index] = unreachableReq->ipData[index];
                            }
                            newCheckSum = CalculateCheckSum(ipDataReq->ipDataLength, (CsrUint8 *) ipDataReq->ipData);
                            ipDataReq->ipData[1] = CSR_BT_LAN_WORD(newCheckSum);

                            CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, ipDataReq);

                            CsrPmemFree(unreachableReq->ipData);
                            break;
                        }

                    default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("ICMP",
                                        instanceData->receivedEvent,
                                        *(CsrUint16*)instanceData->receivedMessage,
                                        0);
#endif
                        break;
                }
                break;

            default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("ICMP",
                                        instanceData->receivedEvent,
                                        0,
                                        0);
#endif
                break;
        }

        if (instanceData->receivedMessage != NULL)
        {
            CsrPmemFree(instanceData->receivedMessage);
            instanceData->receivedMessage = NULL;
        }
    }
}



#ifdef ENABLE_SHUTDOWN
void CsrBtIcmpDeinit(void ** gash)
{
    IcmpInstanceData * instanceData;
    instanceData = (IcmpInstanceData *)(* gash);
    CsrSchedTimerCancel(instanceData->pingTimerId,NULL,NULL);

    CsrPmemFree(instanceData);/* instanceData->receivedMessage is free'ed in CsrBtIcmpHandler */
}
#endif

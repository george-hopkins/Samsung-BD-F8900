/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_internet_primitives.h"
#include "csr_bt_internet_checksum.h"
#include "csr_bt_ip_prim.h"
#include "csr_bt_icmp_prim.h"
#include "csr_bt_udp_prim.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

typedef struct UdpPortRegistryTag
{
    CsrUint16 port;
    CsrSchedQid destinationQueue;
    struct UdpPortRegistryTag * next;
} UdpPortRegistry;

typedef struct
{
    CsrUint16 nextDynamicPortNumber;
    UdpPortRegistry * udpPortRegistryList;
    CsrUint16 ownAddress[2];
} UdpInstanceData;

void CsrBtUdpInit(void ** gash)
{
    UdpInstanceData * instanceData;

    instanceData = (UdpInstanceData *) CsrPmemAlloc(sizeof(UdpInstanceData));
    instanceData->udpPortRegistryList = NULL;
    instanceData->nextDynamicPortNumber = 0xC000;
    instanceData->ownAddress[0] = 0;
    instanceData->ownAddress[1] = 0;
    *gash = (void *) instanceData;
}

void CsrBtUdpHandler(void ** gash)
{
    UdpInstanceData * instanceData;
    CsrUint16 mi=0;
    void * mv=NULL;

    instanceData = (UdpInstanceData *) *gash;

    if (CsrSchedMessageGet(&mi, &mv))
    {
        switch (mi)
        {
            case CSR_BT_IP_PRIM:
                {
                    switch (*((CsrUint16 *) mv))
                    {
                        case CSR_BT_IP_DATA_IND:
                            {
                                CsrBtIpDataInd * ipDataInd;

                                ipDataInd = (CsrBtIpDataInd *) mv;
                                if (ipDataInd->ipData != NULL)
                                {
                                    unsigned long udpPreliminaryCalculatedCheckSum;
                                    unsigned short udpCalculatedCheckSum;
                                    unsigned short udpCheckSum;
                                    unsigned short protocol;

                                    /* check the check-sum */
                                    udpCheckSum = CSR_BT_LAN_WORD(ipDataInd->ipData[3]);
                                    if (udpCheckSum != 0x0000)
                                    {
                                        ipDataInd->ipData[3] = 0x0000;
                                        if (ipDataInd->ipDataLength & 0x0001)
                                        {
                                            ipDataInd->ipData[ipDataInd->ipDataLength / 2] = CSR_BT_LAN_WORD(CSR_BT_LAN_WORD(ipDataInd->ipData[ipDataInd->ipDataLength / 2]) & 0xFF00);
                                        }
                                        udpPreliminaryCalculatedCheckSum = CalculateCheckSumFirstFragment(8, (CsrUint8 *) &ipDataInd->ipHeader[6]);
                                        protocol = CSR_BT_LAN_WORD(CSR_BT_LAN_WORD(ipDataInd->ipHeader[4]) & 0x00FF);
                                        udpPreliminaryCalculatedCheckSum = CalculateCheckSumFragment(udpPreliminaryCalculatedCheckSum, 2, (unsigned char *) &protocol);
                                        udpPreliminaryCalculatedCheckSum = CalculateCheckSumFragment(udpPreliminaryCalculatedCheckSum, 2, (CsrUint8 *) &ipDataInd->ipData[2]);
                                        udpCalculatedCheckSum = CalculateCheckSumLastFragment(udpPreliminaryCalculatedCheckSum, ipDataInd->ipDataLength, (CsrUint8 *) ipDataInd->ipData);
                                        if (udpCalculatedCheckSum == 0x0000)
                                        {
                                            udpCalculatedCheckSum = 0xFFFF;
                                        }
                                    }
                                    else
                                    {
                                        udpCalculatedCheckSum = 0x0000;
                                    }

                                    /* if check-sum OK */
                                    if (udpCheckSum == udpCalculatedCheckSum)
                                    {
                                        UdpPortRegistry * udpPortRegistryEntry;

                                        /* find the destination queue related to the destination port */
                                        for (udpPortRegistryEntry = instanceData->udpPortRegistryList; udpPortRegistryEntry != NULL; udpPortRegistryEntry = udpPortRegistryEntry->next)
                                        {
                                            if (udpPortRegistryEntry->port == ipDataInd->ipData[1])
                                            {
                                                break;
                                            }
                                        }
                                        if (udpPortRegistryEntry != NULL)
                                        {
                                            CsrBtUdpDataInd * dataInd;

                                            dataInd = (CsrBtUdpDataInd *) CsrPmemAlloc(sizeof(CsrBtUdpDataInd));
                                            dataInd->type = CSR_BT_UDP_DATA_IND;
                                            dataInd->sourceAddress[0] = ipDataInd->ipHeader[6];
                                            dataInd->sourceAddress[1] = ipDataInd->ipHeader[7];
                                            dataInd->udpDataLength = ipDataInd->ipDataLength;
                                            dataInd->udpData = ipDataInd->ipData;

                                            CsrSchedMessagePut(udpPortRegistryEntry->destinationQueue, CSR_BT_UDP_PRIM, dataInd);
                                        }
                                        else
                                        {
                                            CsrBtIcmpDestinationUnreachableReq * unreachableReq;

                                            unreachableReq = (CsrBtIcmpDestinationUnreachableReq *) CsrPmemAlloc(sizeof(CsrBtIcmpDestinationUnreachableReq));
                                            unreachableReq->type = CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ;
                                            unreachableReq->code = CSR_BT_PORT_UNREACHABLE_CODE;

                                            /* We need to copy array */
                                            unreachableReq->ipHeaderLength = ipDataInd->ipHeaderLength;
                                            CsrMemCpy(unreachableReq->ipHeader,
                                                   ipDataInd->ipHeader,
                                                   sizeof(ipDataInd->ipHeader));

                                            /* But ipData pointer changes owner, so don't free it */
                                            unreachableReq->ipDataLength = ipDataInd->ipDataLength;
                                            unreachableReq->ipData = ipDataInd->ipData;

                                            CsrSchedMessagePut(CSR_BT_ICMP_IFACEQUEUE, CSR_BT_ICMP_PRIM, unreachableReq);
                                            CsrPmemFree(ipDataInd);
                                            ipDataInd = NULL;
                                            mv = NULL;
                                        }
                                    }
                                    else
                                    {
                                        CsrPmemFree(ipDataInd->ipData);
                                    }
                                }
                                break;
                            }

                        case CSR_BT_IP_ADDRESS_IND:
                            {
                                CsrBtIpAddressInd * addressInd;

                                addressInd = (CsrBtIpAddressInd *) mv;
                                instanceData->ownAddress[0] = addressInd->ipAddress[0];
                                instanceData->ownAddress[1] = addressInd->ipAddress[1];
                                break;
                            }

                        case CSR_BT_IP_SHUTDOWN_IND:
                            {
                                CsrBtUdpShutdownInd * shutdownInd;
                                UdpPortRegistry * previousPtr;
                                UdpPortRegistry * searchPtr;

                                while (instanceData->udpPortRegistryList)
                                {

                                    previousPtr = instanceData->udpPortRegistryList;
                                    searchPtr = previousPtr->next;
                                    while (searchPtr != NULL)
                                    {
                                        if (searchPtr->destinationQueue == instanceData->udpPortRegistryList->destinationQueue)
                                        {
                                            previousPtr->next = searchPtr->next;
                                            CsrPmemFree(searchPtr);
                                        }
                                        else
                                        {
                                            previousPtr = searchPtr;
                                        }
                                        searchPtr = previousPtr->next;
                                    }

                                    shutdownInd = (CsrBtUdpShutdownInd *) CsrPmemAlloc(sizeof(CsrBtUdpShutdownInd));
                                    shutdownInd->type = CSR_BT_UDP_SHUTDOWN_IND;
                                    CsrSchedMessagePut(instanceData->udpPortRegistryList->destinationQueue, CSR_BT_UDP_PRIM, shutdownInd);

                                    searchPtr = instanceData->udpPortRegistryList;
                                    instanceData->udpPortRegistryList = instanceData->udpPortRegistryList->next;
                                    CsrPmemFree(searchPtr);
                                }
                                break;
                            }

                        default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                            CsrStateEventException("UDP",
                                                    mi,
                                                    *((CsrUint16 *) mv),
                                                    0);
#endif
                            break;
                    }
                    break;
                }

            case CSR_BT_UDP_PRIM:
                {
                    switch (*((CsrUint16 *) mv))
                    {
                        case CSR_BT_UDP_DATA_REQ:
                            {
                                CsrBtUdpDataReq * dataReq;
                                CsrBtIpDataReq * ipDataReq;
                                unsigned long udpPreliminaryCalculatedCheckSum;
                                unsigned short udpCalculatedCheckSum;
                                CsrUint16 protocol;

                                dataReq = (CsrBtUdpDataReq *) mv;
                                ipDataReq = (CsrBtIpDataReq *) CsrPmemAlloc(sizeof(CsrBtIpDataReq));
                                ipDataReq->type = CSR_BT_IP_DATA_REQ;
                                ipDataReq->typeOfService = dataReq->typeOfService;
                                ipDataReq->timeToLive = dataReq->timeToLive;
                                ipDataReq->protocol = CSR_BT_IP_PROTOCOL_UDP;
                                ipDataReq->destinationAddress[0] = dataReq->destinationAddress[0];
                                ipDataReq->destinationAddress[1] = dataReq->destinationAddress[1];
                                ipDataReq->sourceAddress[0] = instanceData->ownAddress[0];
                                ipDataReq->sourceAddress[1] = instanceData->ownAddress[1];
                                ipDataReq->ipDataLength = dataReq->udpDataLength;
                                ipDataReq->ipData = dataReq->udpData;
                                ipDataReq->ipData[2] = CSR_BT_LAN_WORD(dataReq->udpDataLength);
                                ipDataReq->ipData[3] = 0x0000; /* reset check sum */
                                /* make sure that the last word is zero padded if the length is odd */
                                if (ipDataReq->ipDataLength & 0x0001)
                                {
                                    ipDataReq->ipData[ipDataReq->ipDataLength / 2] = CSR_BT_LAN_WORD(CSR_BT_LAN_WORD(ipDataReq->ipData[ipDataReq->ipDataLength / 2]) & 0xFF00);
                                }
                                udpPreliminaryCalculatedCheckSum = CalculateCheckSumFirstFragment(4, (CsrUint8 *) ipDataReq->destinationAddress);
                                udpPreliminaryCalculatedCheckSum = CalculateCheckSumFragment(udpPreliminaryCalculatedCheckSum, 4, (CsrUint8 *) instanceData->ownAddress);
                                protocol = CSR_BT_LAN_WORD(CSR_BT_IP_PROTOCOL_UDP);
                                udpPreliminaryCalculatedCheckSum = CalculateCheckSumFragment(udpPreliminaryCalculatedCheckSum, 2, (unsigned char *) &protocol);
                                udpPreliminaryCalculatedCheckSum = CalculateCheckSumFragment(udpPreliminaryCalculatedCheckSum, 2, (CsrUint8 *) &ipDataReq->ipData[2]);
                                udpCalculatedCheckSum = CalculateCheckSumLastFragment(udpPreliminaryCalculatedCheckSum, ipDataReq->ipDataLength, (CsrUint8 *) ipDataReq->ipData);
                                if (udpCalculatedCheckSum == 0x0000)
                                {
                                    udpCalculatedCheckSum = 0xFFFF;
                                }
                                ipDataReq->ipData[3] = CSR_BT_LAN_WORD(udpCalculatedCheckSum);

                                CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, ipDataReq);

                                break;
                            }

                        case CSR_BT_UDP_REGISTER_PORT_REQ:
                            {
                                CsrBtUdpRegisterPortReq * registerReq;
                                CsrBtUdpRegisterPortCfm * registerCfm;

                                registerReq = (CsrBtUdpRegisterPortReq *) mv;
                                registerCfm = (CsrBtUdpRegisterPortCfm *) CsrPmemAlloc(sizeof(CsrBtUdpRegisterPortCfm));
                                registerCfm->type = CSR_BT_UDP_REGISTER_PORT_CFM;

                                /* if port not zero, first check if it is already known */
                                if (registerReq->port != 0x0000)
                                {
                                    UdpPortRegistry * udpPortRegistryEntry;

                                    registerCfm->port = registerReq->port;

                                    /* find the destination queue related to the destination port */
                                    for (udpPortRegistryEntry = instanceData->udpPortRegistryList; udpPortRegistryEntry != NULL; udpPortRegistryEntry = udpPortRegistryEntry->next)
                                    {
                                        if (udpPortRegistryEntry->port == registerReq->port)
                                        {
                                            break;
                                        }
                                    }
                                    if (udpPortRegistryEntry == NULL)
                                    {
                                        UdpPortRegistry * newEntry;


                                        registerCfm->result = CSR_BT_UDP_SUCCESS;

                                        /* add registry entry to udpPortRegistryList */
                                        newEntry = (UdpPortRegistry *) CsrPmemAlloc(sizeof(UdpPortRegistry));
                                        newEntry->port = registerCfm->port;
                                        newEntry->destinationQueue = registerReq->destinationQueue;
                                        newEntry->next = instanceData->udpPortRegistryList;
                                        instanceData->udpPortRegistryList = newEntry;
                                    }
                                    else
                                    {
                                        /* set result to CSR_BT_UDP_PORT_ALREADY_IN_USE */
                                        registerCfm->result = CSR_BT_UDP_PORT_ALREADY_IN_USE;
                                    }
                                }
                                else
                                {
                                    UdpPortRegistry * udpPortRegistryEntry;
                                    UdpPortRegistry * newEntry;

                                    /* allocate a dynamic port */
                                    do
                                    {
                                        registerCfm->port = CSR_BT_LAN_WORD(instanceData->nextDynamicPortNumber++);
                                        if (instanceData->nextDynamicPortNumber == 0x0000)
                                        {
                                            instanceData->nextDynamicPortNumber = 0xC000;
                                        }

                                        /* find the destination queue related to the destination port */
                                        for (udpPortRegistryEntry = instanceData->udpPortRegistryList; udpPortRegistryEntry != NULL; udpPortRegistryEntry = udpPortRegistryEntry->next)
                                        {
                                            if (udpPortRegistryEntry->port == registerCfm->port)
                                            {
                                                break;
                                            }
                                        }
                                    } while (udpPortRegistryEntry != NULL);


                                    registerCfm->result = CSR_BT_UDP_SUCCESS;

                                    /* add registry entry to udpPortRegistryList */
                                    newEntry = (UdpPortRegistry *) CsrPmemAlloc(sizeof(UdpPortRegistry));
                                    newEntry->port = registerCfm->port;
                                    newEntry->destinationQueue = registerReq->destinationQueue;
                                    newEntry->next = instanceData->udpPortRegistryList;
                                    instanceData->udpPortRegistryList = newEntry;
                                }

                                /* send confirmation */
                                CsrSchedMessagePut(registerReq->destinationQueue, CSR_BT_UDP_PRIM, registerCfm);
                                break;
                            }

                        case CSR_BT_UDP_DEREGISTER_PORT_REQ:
                            {
                                CsrBtUdpDeregisterPortReq * deregisterReq;
                                UdpPortRegistry * udpPortRegistryEntry;
                                UdpPortRegistry * previousUdpPortRegistryEntry;

                                deregisterReq = (CsrBtUdpDeregisterPortReq *) mv;

                                /* find the destination queue related to the destination port */
                                for (previousUdpPortRegistryEntry = NULL, udpPortRegistryEntry = instanceData->udpPortRegistryList; udpPortRegistryEntry != NULL; udpPortRegistryEntry = udpPortRegistryEntry->next)
                                {
                                    if (udpPortRegistryEntry->port == deregisterReq->port)
                                    {
                                        break;
                                    }
                                    previousUdpPortRegistryEntry = udpPortRegistryEntry;
                                }
                                if (udpPortRegistryEntry != NULL)
                                {
                                    if (previousUdpPortRegistryEntry != NULL)
                                    {
                                        previousUdpPortRegistryEntry->next = udpPortRegistryEntry->next;
                                    }
                                    else
                                    {
                                        instanceData->udpPortRegistryList = udpPortRegistryEntry->next;
                                    }
                                    CsrPmemFree(udpPortRegistryEntry);
                                }
                                break;
                            }

                        default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                            CsrStateEventException("UDP",
                                                    mi,
                                                    *((CsrUint16 *) mv),
                                                    0);
#endif
                            break;
                    }
                    break;
                }

            case CSR_BT_ICMP_PRIM:
                {
                    switch (*((CsrUint16 *) mv))
                    {
                        case CSR_BT_ICMP_PORT_UNREACHABLE_IND:
                            {
                                CsrBtIpDataInd * ipDataInd;
                                UdpPortRegistry * udpPortRegistryEntry;
                                CsrUint16 udpDataIndex;

                                ipDataInd = (CsrBtIpDataInd *) mv;

                                /* find port and send port unreachable to higher layer */
                                udpDataIndex = (CSR_BT_LAN_WORD(ipDataInd->ipData[4 + 0]) & 0x0F00) >> 7;
                                for (udpPortRegistryEntry = instanceData->udpPortRegistryList; udpPortRegistryEntry != NULL; udpPortRegistryEntry = udpPortRegistryEntry->next)
                                {
                                    if (udpPortRegistryEntry->port == ipDataInd->ipData[4 + udpDataIndex + 0])
                                    {
                                        break;
                                    }
                                }
                                if (udpPortRegistryEntry != NULL)
                                {
                                    CsrBtUdpPortUnreachableInd * unreachableInd;

                                    unreachableInd = (CsrBtUdpPortUnreachableInd *) CsrPmemAlloc(sizeof(CsrBtUdpPortUnreachableInd));
                                    unreachableInd->type = CSR_BT_UDP_PORT_UNREACHABLE_IND;
                                    unreachableInd->localPort = udpPortRegistryEntry->port;
                                    unreachableInd->remotePort = ipDataInd->ipData[4 + udpDataIndex + 1];

                                    CsrSchedMessagePut(udpPortRegistryEntry->destinationQueue, CSR_BT_UDP_PRIM, unreachableInd);
                                }
                                CsrPmemFree(ipDataInd->ipData);

                                break;
                            }

                        default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                            CsrStateEventException("UDP",
                                                    mi,
                                                    *((CsrUint16 *) mv),
                                                    0);
#endif
                            break;
                    }
                    break;
                }

            default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                            CsrStateEventException("UDP",
                                                    mi,
                                                    0,
                                                    0);
#endif
                break;
        }

        CsrPmemFree(mv);
    }
}


#ifdef ENABLE_SHUTDOWN
void CsrBtUdpDeinit(void ** gash)
{

    UdpInstanceData  *instData;
    UdpPortRegistry  *pTmp;
    UdpPortRegistry  *pTmp2;
    instData = (UdpInstanceData *) (*gash);

    pTmp = instData->udpPortRegistryList;
    while(pTmp!=NULL)
    {
        pTmp2=pTmp->next;
        CsrPmemFree(pTmp);
        pTmp=pTmp2;
    }
    CsrPmemFree(instData);
}
#endif

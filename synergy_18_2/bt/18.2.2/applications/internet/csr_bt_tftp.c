/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#define VERBOSE_INFORMATION

#ifdef VERBOSE_INFORMATION
#include <stdio.h>
#define PRINTOUT(fmt) printf fmt
#else
#define PRINTOUT(fmt)
#endif /* VERBOSE_INFORMATION */

#include "csr_bt_platform.h"
#include "csr_bt_util.h"
#include "csr_bt_internet_primitives.h"
#include "csr_bt_udp_prim.h"
#include "csr_pmem.h"
#include "csr_bt_file.h"
#include "csr_bt_demoapp.h"

#define MAX_NUMBER_OF_RETRANSMISSIONS       10
#define DATA_RETRANSMISSION_TIMEOUT         1000000
#define DALLYING_TIMEOUT                    1000000
#define CONNECTION_LOST_TIMEOUT             5000000

#define MEMORY_WRITE_OPERATION              0x0001
#define RUN_OPERATION                       0x0002
#define MEMORY_READ_OPERATION               0x0003
#define LOG_READ_OPERATION                  0x0004
#define FLASH_WRITE_OPERATION               0x0005
#define FLASH_READ_OPERATION                0x0006

typedef struct
{
    CsrUint16 remotePort;
    CsrUint16 remoteAddress[2];
    CsrUint16 localPort;
    CsrUint16 numberOfRetransmissions;
    CsrSchedTid recoveryTimerId;
    CsrUint16 currentBlockNumber;
    CsrFileHandle *filePointer;
    CsrUint16 currentOperation;
    CsrUint32 startMemoryAddress;
    CsrUint32 startRunAddress;
    CsrUint32 stopMemoryAddress;
} TftpInstanceData;

static void sendTftpData(CsrUint16 * remoteAddress, CsrUint16 remotePort,
                         CsrUint16 localPort, CsrUint16 blockNumber,
                         CsrUint16 length, CsrFileHandle *filePointer)
{
    CsrBtUdpDataReq * data;

    data = (CsrBtUdpDataReq *) CsrPmemAlloc(sizeof(CsrBtUdpDataReq));
    data->type = CSR_BT_UDP_DATA_REQ;
    data->typeOfService = 0;
    data->timeToLive = 128;
    data->destinationAddress[0] = remoteAddress[0];
    data->destinationAddress[1] = remoteAddress[1];
    data->udpDataLength = 8 + 4 + length;
    data->udpData = (CsrUint16 *) CsrPmemZalloc(data->udpDataLength + (data->udpDataLength & 0x0001));
    data->udpData[0] = localPort;
    data->udpData[1] = remotePort;
    data->udpData[4] = CSR_BT_LAN_WORD(0x0003);
    data->udpData[5] = CSR_BT_LAN_WORD(blockNumber);
    CsrBtFseek(filePointer, 512L * (((long) blockNumber) - 1L), SEEK_SET);
    CsrBtFread((void *) &data->udpData[6], length, 1, filePointer);

    CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_PRIM, data);
}

static void sendTftpAcknowledgement(CsrUint16 * remoteAddress, CsrUint16 remotePort, CsrUint16 localPort, CsrUint16 blockNumber)
{
    CsrBtUdpDataReq * ack;

    ack = (CsrBtUdpDataReq *) CsrPmemAlloc(sizeof(CsrBtUdpDataReq));
    ack->type = CSR_BT_UDP_DATA_REQ;
    ack->typeOfService = 0;
    ack->timeToLive = 128;
    ack->destinationAddress[0] = remoteAddress[0];
    ack->destinationAddress[1] = remoteAddress[1];
    ack->udpDataLength = 8 + 4;
    ack->udpData = (CsrUint16 *) CsrPmemZalloc(ack->udpDataLength);
    ack->udpData[0] = localPort;
    ack->udpData[1] = remotePort;
    ack->udpData[4] = CSR_BT_LAN_WORD(0x0004);
    ack->udpData[5] = CSR_BT_LAN_WORD(blockNumber);

    CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_PRIM, ack);
}

static void sendTftpError(CsrUint16 * remoteAddress, CsrUint16 remotePort, CsrUint16 localPort, CsrUint16 errorCode, char * errorString)
{
    CsrBtUdpDataReq * error;
    char * errorStringDestination;

    error = (CsrBtUdpDataReq *) CsrPmemAlloc(sizeof(CsrBtUdpDataReq));
    error->type = CSR_BT_UDP_DATA_REQ;
    error->typeOfService = 0;
    error->timeToLive = 128;
    error->destinationAddress[0] = remoteAddress[0];
    error->destinationAddress[1] = remoteAddress[1];
    error->udpDataLength = 8 + 4 + (errorString != NULL ? (CsrUint16) CsrStrLen(errorString) : 0) + 1;
    error->udpData = (CsrUint16 *) CsrPmemZalloc(error->udpDataLength + (error->udpDataLength & 0x0001));
    error->udpData[0] = localPort;
    error->udpData[1] = remotePort;
    error->udpData[4] = CSR_BT_LAN_WORD(0x0005);
    error->udpData[5] = CSR_BT_LAN_WORD(errorCode);
    errorStringDestination = (char *) &error->udpData[6];
    if (errorString != NULL)
    {
        CsrStrCpy(errorStringDestination, errorString);
    }
    else
    {
        *errorStringDestination = '\0';
    }

    CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_PRIM, error);
}

static void sendUdpPortRegisterReq(CsrSchedQid queueId, CsrUint16 portNumber)
{
    CsrBtUdpRegisterPortReq * registerPortReq;

    registerPortReq = (CsrBtUdpRegisterPortReq *) CsrPmemAlloc(sizeof(CsrBtUdpRegisterPortReq));
    registerPortReq->type = CSR_BT_UDP_REGISTER_PORT_REQ;
    registerPortReq->port = portNumber;
    registerPortReq->destinationQueue = queueId;
    CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_PRIM, registerPortReq);
}

static void sendUdpPortDeregisterReq(CsrUint16 portNumber)
{
    CsrBtUdpDeregisterPortReq * deregisterPortReq;

    deregisterPortReq = (CsrBtUdpDeregisterPortReq *) CsrPmemAlloc(sizeof(CsrBtUdpDeregisterPortReq));
    deregisterPortReq->type = CSR_BT_UDP_DEREGISTER_PORT_REQ;
    deregisterPortReq->port = portNumber;
    CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_PRIM, deregisterPortReq);
}

static void connectionLostTimeout(CsrUint16 mi, void * mv)
{
    TftpInstanceData * instanceData;

    instanceData = (TftpInstanceData *) mv;

    PRINTOUT(("CONNECTION LOST!\n"));

    /* Connection lost - resetting... */
    CsrBtFclose(instanceData->filePointer);

    instanceData->filePointer = NULL;
    if (instanceData->recoveryTimerId != 0)
    {
        CsrSchedTimerCancel(instanceData->recoveryTimerId, NULL, NULL);
        instanceData->recoveryTimerId = 0;
    }

    /* De-register the old dynamic port and register a new dynamic port (rfc 1350, page 4, 2nd paragraph) */
    sendUdpPortDeregisterReq(instanceData->localPort);
    instanceData->localPort = 0;
    sendUdpPortRegisterReq(CSR_BT_TFTP_IFACEQUEUE, 0);
}

static void dataRetransmissionTimeout(CsrUint16 mi, void * mv)
{
    TftpInstanceData * instanceData;

    instanceData = (TftpInstanceData *) mv;

    if (instanceData->numberOfRetransmissions < MAX_NUMBER_OF_RETRANSMISSIONS)
    {
        PRINTOUT(("+"));
        sendTftpData(instanceData->remoteAddress, instanceData->remotePort, instanceData->localPort, instanceData->currentBlockNumber, ((CsrUint16) (instanceData->startRunAddress >= 512 ? 512 : instanceData->startRunAddress)), instanceData->filePointer);
        instanceData->recoveryTimerId = CsrSchedTimerSet(DATA_RETRANSMISSION_TIMEOUT, dataRetransmissionTimeout, 0, (void *) instanceData);
        instanceData->numberOfRetransmissions++;
    }
    else
    {
        PRINTOUT(("!"));
        /* Connection lost - resetting... */
        CsrBtFclose(instanceData->filePointer);
        instanceData->filePointer = NULL;
        instanceData->recoveryTimerId = 0;

        /* De-register the old dynamic port and register a new dynamic port (rfc 1350, page 4, 2nd paragraph) */
        sendUdpPortDeregisterReq(instanceData->localPort);
        instanceData->localPort = 0;
        sendUdpPortRegisterReq(CSR_BT_TFTP_IFACEQUEUE, 0);
    }
}

static void dallyingTimeout(CsrUint16 mi, void * mv)
{
    TftpInstanceData * instanceData;

    instanceData = (TftpInstanceData *) mv;

    PRINTOUT(("\nDONE!\n"));

    instanceData->recoveryTimerId = 0;

    /* De-register the old dynamic port and register a new dynamic port (rfc 1350, page 4, 2nd paragraph) */
    sendUdpPortDeregisterReq(instanceData->localPort);
    instanceData->localPort = 0;
    sendUdpPortRegisterReq(CSR_BT_TFTP_IFACEQUEUE, 0);

    if (instanceData->currentOperation == RUN_OPERATION)
    {
        /* TODO: close down and jump to instanceData->startRunAddress! */
    }
}

#ifdef VERBOSE_INFORMATION
static void printWRQInfo(CsrBtUdpDataInd * dataInd)
{
    printf("\nWRQ:\n");
    printf("- Filename: %s\n", (char *) &dataInd->udpData[5]);
    printf("- Mode: %s\n", &(((char *) &dataInd->udpData[5])[CsrStrLen((char *) &dataInd->udpData[5]) + 1]));
    printf("- Tftp message length: %d\n", dataInd->udpDataLength - 8);
    printf("- Remote address: %04X%04X\n", CSR_BT_LAN_WORD(dataInd->sourceAddress[0]), CSR_BT_LAN_WORD(dataInd->sourceAddress[1]));
    printf("- Remote port: %d\n", CSR_BT_LAN_WORD(dataInd->udpData[0]));
    if (((CsrUint16) (dataInd->udpDataLength - 8)) > 2 + CsrStrLen((char *) &dataInd->udpData[5]) + 1 + CsrStrLen(&(((char *) &dataInd->udpData[5])[CsrStrLen((char *) &dataInd->udpData[5]) + 1])) + 1)
    {
        printf("- Options!\n");
    }
}

static void printRRQInfo(CsrBtUdpDataInd * dataInd)
{
    printf("\nRRQ:\n");
    printf("- Filename: %s\n", (char *) &dataInd->udpData[5]);
    printf("- Mode: %s\n", &(((char *) &dataInd->udpData[5])[CsrStrLen((char *) &dataInd->udpData[5]) + 1]));
    printf("- Remote address: %04X%04X\n", CSR_BT_LAN_WORD(dataInd->sourceAddress[0]), CSR_BT_LAN_WORD(dataInd->sourceAddress[1]));
    printf("- Remote port: %d\n", CSR_BT_LAN_WORD(dataInd->udpData[0]));
}
#endif

void CsrBtTftpInit(void ** gash)
{
    TftpInstanceData * instanceData;

    instanceData = (TftpInstanceData *) CsrPmemAlloc(sizeof(TftpInstanceData));
    instanceData->remotePort = 0;
    instanceData->localPort = 0;
    instanceData->filePointer = NULL;
    instanceData->recoveryTimerId = 0;
    instanceData->currentOperation = 0;
    instanceData->startMemoryAddress = 0L;
    instanceData->startRunAddress = 0L;
    *gash = (void *) instanceData;

    sendUdpPortRegisterReq(CSR_BT_TFTP_IFACEQUEUE, CSR_BT_LAN_WORD(69));
    sendUdpPortRegisterReq(CSR_BT_TFTP_IFACEQUEUE, 0);
}

void CsrBtTftpHandler(void ** gash)
{
    TftpInstanceData * instanceData;
    CsrUint16 mi=0;
    void * mv=NULL;

    instanceData = (TftpInstanceData *) *gash;

    if (CsrSchedMessageGet(&mi, &mv))
    {
        switch (mi)
        {
            case CSR_BT_UDP_PRIM:
            {
                switch (*((CsrUint16 *) mv))
                {
                    case CSR_BT_UDP_DATA_IND:
                    {
                        CsrBtUdpDataInd * dataInd;

                        dataInd = (CsrBtUdpDataInd *) mv;
                        switch (CSR_BT_LAN_WORD(dataInd->udpData[4]))
                        {
                            case 0x0001:    /* RRQ so return first data */
                                if (instanceData->localPort != 0)
                                {
                                    char * fileName;
                                    CsrBtPosixStatBuffer fileInformationBuffer;
#ifdef VERBOSE_INFORMATION
                                    printRRQInfo(dataInd);
#endif
                                    fileName = (char *) &dataInd->udpData[5];

                                    if (CsrBtDiskstat(fileName, &fileInformationBuffer) == 0)
                                    {
                                        instanceData->currentOperation = MEMORY_READ_OPERATION;
                                        instanceData->filePointer = CsrBtFopen(fileName, "rb");
                                        if (instanceData->filePointer != NULL)
                                        {
                                            instanceData->startRunAddress = fileInformationBuffer.st_size;
                                            instanceData->remoteAddress[0] = dataInd->sourceAddress[0];
                                            instanceData->remoteAddress[1] = dataInd->sourceAddress[1];
                                            instanceData->remotePort = dataInd->udpData[0];
                                            instanceData->currentBlockNumber = 1;
                                            sendTftpData(instanceData->remoteAddress, instanceData->remotePort, instanceData->localPort, instanceData->currentBlockNumber, ((CsrUint16) (instanceData->startRunAddress >= 512 ? 512 : instanceData->startRunAddress)), instanceData->filePointer);
                                            instanceData->numberOfRetransmissions = 0;
                                            CsrSchedTimerCancel(instanceData->recoveryTimerId,NULL,NULL);
                                            instanceData->recoveryTimerId = CsrSchedTimerSet(DATA_RETRANSMISSION_TIMEOUT, dataRetransmissionTimeout, 0, (void *) instanceData);
                                        }
                                        else
                                        {
                                            sendTftpError(dataInd->sourceAddress, dataInd->udpData[0], instanceData->localPort, 0, "Illegal file name!");
                                        }
                                    }
                                    else
                                    {
                                        sendTftpError(dataInd->sourceAddress, dataInd->udpData[0], instanceData->localPort, 0, "Illegal file name!");
                                    }
                                }
                                else
                                {
                                    sendTftpError(dataInd->sourceAddress, dataInd->udpData[0], instanceData->localPort, 0, "Busy try again later!");
                                }
                                break;

                            case 0x0002:    /* WRQ so return ack #0 */
                                if (instanceData->localPort != 0)
                                {
                                    char * fileName;
#ifdef VERBOSE_INFORMATION
                                    printWRQInfo(dataInd);
#endif
                                    fileName = (char *) &dataInd->udpData[5];
                                    instanceData->currentOperation = MEMORY_WRITE_OPERATION;
                                    instanceData->filePointer = CsrBtFopen(fileName, "wb");
                                    if (instanceData->filePointer != NULL)
                                    {
                                        instanceData->remoteAddress[0] = dataInd->sourceAddress[0];
                                        instanceData->remoteAddress[1] = dataInd->sourceAddress[1];
                                        instanceData->remotePort = dataInd->udpData[0];
                                        instanceData->currentBlockNumber = 0;
                                        sendTftpAcknowledgement(instanceData->remoteAddress, instanceData->remotePort, instanceData->localPort, instanceData->currentBlockNumber++);
                                        CsrSchedTimerCancel(instanceData->recoveryTimerId,NULL,NULL);
                                        instanceData->recoveryTimerId = CsrSchedTimerSet(CONNECTION_LOST_TIMEOUT, connectionLostTimeout, 0, (void *) instanceData);
                                    }
                                    else
                                    {
                                        sendTftpError(dataInd->sourceAddress, dataInd->udpData[0], instanceData->localPort, 0, "Illegal file name!");
                                    }
                                }
                                else
                                {
                                    sendTftpError(dataInd->sourceAddress, dataInd->udpData[0], instanceData->localPort, 0, "Busy try again later!");
                                }
                                break;

                            case 0x0003:    /* data */
                                PRINTOUT(("."));
                                sendTftpAcknowledgement(instanceData->remoteAddress, instanceData->remotePort, instanceData->localPort, (CsrUint16) CSR_BT_LAN_WORD(dataInd->udpData[5]));
                                if ((instanceData->filePointer != NULL) && (instanceData->currentBlockNumber == CSR_BT_LAN_WORD(dataInd->udpData[5])))
                                {
                                    CsrBtFwrite((void *) &dataInd->udpData[6], dataInd->udpDataLength - 8 - 4, 1, instanceData->filePointer);
                                    instanceData->currentBlockNumber++;
                                }
                                if (instanceData->filePointer && (dataInd->udpDataLength < 524))    /* last packet? (data length < 512) */
                                {
                                    CsrBtFclose(instanceData->filePointer);
                                    instanceData->filePointer = NULL;

                                    CsrSchedTimerCancel(instanceData->recoveryTimerId,NULL,NULL);
                                    instanceData->recoveryTimerId = CsrSchedTimerSet(DALLYING_TIMEOUT, dallyingTimeout, 0, (void *) instanceData);
                                }
                                else
                                {
                                    CsrSchedTimerCancel(instanceData->recoveryTimerId,NULL,NULL);
                                    instanceData->recoveryTimerId = CsrSchedTimerSet(CONNECTION_LOST_TIMEOUT, connectionLostTimeout, 0, (void *) instanceData);
                                }
                                break;

                            case 0x0004:    /* acknowledgement */
                                if (instanceData->currentBlockNumber == CSR_BT_LAN_WORD(dataInd->udpData[5]))
                                {
                                    PRINTOUT(("*"));
                                    instanceData->numberOfRetransmissions = 0;
                                    CsrSchedTimerCancel(instanceData->recoveryTimerId,NULL,NULL);
                                    if (instanceData->startRunAddress != 0)
                                    {
                                        instanceData->currentBlockNumber++;
                                        if (instanceData->startRunAddress >= 512)
                                        {
                                            instanceData->startRunAddress -= 512;
                                        }
                                        else
                                        {
                                            instanceData->startRunAddress = 0;
                                        }
                                        sendTftpData(instanceData->remoteAddress, instanceData->remotePort, instanceData->localPort, instanceData->currentBlockNumber, ((CsrUint16) (instanceData->startRunAddress >= 512 ? 512 : instanceData->startRunAddress)), instanceData->filePointer);
                                        instanceData->recoveryTimerId = CsrSchedTimerSet(DATA_RETRANSMISSION_TIMEOUT, dataRetransmissionTimeout, 0, (void *) instanceData);
                                    }
                                    else    /* DONE! */
                                    {
                                        PRINTOUT(("\nDONE!\n"));
                                        CsrBtFclose(instanceData->filePointer);
                                        instanceData->filePointer = NULL;
                                        instanceData->recoveryTimerId = 0;

                                        /* De-register the old dynamic port and register a new dynamic port (rfc 1350, page 4, 2nd paragraph) */
                                        sendUdpPortDeregisterReq(instanceData->localPort);
                                        instanceData->localPort = 0;
                                        sendUdpPortRegisterReq(CSR_BT_TFTP_IFACEQUEUE, 0);
                                    }
                                }
                                else
                                {
                                    PRINTOUT(("\nWrong block number in ACK!?!?\n"));
                                }
                                break;

                            case 0x0005:
                                PRINTOUT(("\nError: #%d - \"%s\"\n", CSR_BT_LAN_WORD(dataInd->udpData[5]), (char *) &dataInd->udpData[6]));
                                CsrSchedTimerCancel(instanceData->recoveryTimerId,NULL,NULL);
                                CsrBtFclose(instanceData->filePointer);
                                instanceData->filePointer = NULL;
                                instanceData->recoveryTimerId = 0;

                                /* De-register the old dynamic port and register a new dynamic port (rfc 1350, page 4, 2nd paragraph) */
                                sendUdpPortDeregisterReq(instanceData->localPort);
                                instanceData->localPort = 0;
                                sendUdpPortRegisterReq(CSR_BT_TFTP_IFACEQUEUE, 0);
                                break;

                            default:
                                /* reply with error message "Illegal TFTP operation" */
                                sendTftpError(dataInd->sourceAddress, dataInd->udpData[0], instanceData->localPort, 4, NULL);
                                break;
                        }
                        CsrPmemFree(dataInd->udpData);
                        break;
                    }

                    case CSR_BT_UDP_REGISTER_PORT_CFM:
                    {
                        CsrBtUdpRegisterPortCfm * registerCfm;

                        registerCfm = (CsrBtUdpRegisterPortCfm *) mv;
                        if (registerCfm->result == CSR_BT_UDP_SUCCESS)
                        {
                            if (registerCfm->port != CSR_BT_LAN_WORD(69))
                            {
                                instanceData->localPort = registerCfm->port;
                            }
                        }
                        else
                        {
                            /* TODO: add register ports failure handling */
                            PRINTOUT(("CSR_BT_UDP_REGISTER_PORT_CFM: result=%d?\n", registerCfm->result));
                        }
                        break;
                    }

                    case CSR_BT_UDP_PORT_UNREACHABLE_IND:
                    {
                        CsrBtUdpPortUnreachableInd * unreachableInd;

                        unreachableInd = (CsrBtUdpPortUnreachableInd *) mv;

                        if ((unreachableInd->localPort == instanceData->localPort) && (unreachableInd->remotePort == instanceData->remotePort))
                        {
                            if (instanceData->startRunAddress >= 512)
                            {
                                PRINTOUT(("CSR_BT_UDP_PORT_UNREACHABLE_IND - terminating connection...\n"));
                            }
                            else
                            {
                                PRINTOUT(("\nDONE?\n"));
                            }

                            CsrSchedTimerCancel(instanceData->recoveryTimerId,NULL,NULL);
                            CsrBtFclose(instanceData->filePointer);
                            instanceData->filePointer = NULL;
                            instanceData->recoveryTimerId = 0;

                            /* De-register the old dynamic port and register a new dynamic port (rfc 1350, page 4, 2nd paragraph) */
                            sendUdpPortDeregisterReq(instanceData->localPort);
                            instanceData->localPort = 0;
                            sendUdpPortRegisterReq(CSR_BT_TFTP_IFACEQUEUE, 0);
                        }
                        break;
                    }

                    case CSR_BT_UDP_SHUTDOWN_IND:
                    {
                        if (instanceData->recoveryTimerId != 0)
                        {
                            CsrSchedTimerCancel(instanceData->recoveryTimerId, NULL, NULL);
                            instanceData->recoveryTimerId = 0;
                        }
                        if (instanceData->filePointer != NULL)
                        {
                            CsrBtFclose(instanceData->filePointer);
                        }
                        instanceData->filePointer = NULL;
                        instanceData->remotePort = 0;
                        instanceData->localPort = 0;
                        instanceData->currentOperation = 0;
                        instanceData->startMemoryAddress = 0L;
                        instanceData->startRunAddress = 0L;
                        break;
                    }

                    default:
                        PRINTOUT(("CsrBtTftpHandler: unknown type (%04X)!?!?\n", *((CsrUint16 *) mv)));
                        break;
                }
                break;
            }

            default:
                PRINTOUT(("CsrBtTftpHandler: unknown mi (%04X)!?!?\n", mi));
                break;
        }

        CsrPmemFree(mv);
    }
}

#ifdef ENABLE_SHUTDOWN
void CsrBtTftpDeinit(void ** gash)
{
    TftpInstanceData * instanceData;
    instanceData = (TftpInstanceData *)(* gash);

    CsrSchedTimerCancel(instanceData->recoveryTimerId, NULL, NULL);

    if (instanceData->filePointer != NULL)
    {
        CsrBtFclose(instanceData->filePointer);
    }

    CsrPmemFree(instanceData);
    *gash = NULL;
}
#endif


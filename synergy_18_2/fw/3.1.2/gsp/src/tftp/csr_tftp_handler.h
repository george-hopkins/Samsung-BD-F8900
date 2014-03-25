#ifndef CSR_TFTP_HANDLER_H__
#define CSR_TFTP_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_list.h"
#include "csr_tftp_prim.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrTftpLto);

#define CSR_TFTP_STATE_IDLE                          (0x0)
#define CSR_TFTP_STATE_ACTIVATING                    (0x1)
#define CSR_TFTP_STATE_ACTIVATED                     (0x2)
#define CSR_TFTP_STATE_DEACTIVATING                  (0x3)

#define CSR_TFTP_STATE_CONNECTION_IDLE               (0x0)
#define CSR_TFTP_STATE_CONNECTION_CONNECTING         (0x1)
#define CSR_TFTP_STATE_CONNECTION_CREATING           (0x2)
#define CSR_TFTP_STATE_CONNECTION_BINDING            (0x3)
#define CSR_TFTP_STATE_CONNECTION_ACTIVE             (0x4)
#define CSR_TFTP_STATE_CONNECTION_READ               (0x5)
#define CSR_TFTP_STATE_CONNECTION_READ_DATA_SENDING  (0x6)
#define CSR_TFTP_STATE_CONNECTION_READ_CLOSE         (0x7)
#define CSR_TFTP_STATE_CONNECTION_WRITE              (0x8)
#define CSR_TFTP_STATE_CONNECTION_WRITE_DATA_SENDING (0x9)
#define CSR_TFTP_STATE_CONNECTION_WRITE_WAIT_CLOSE   (0xA)
#define CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING (0xB)
#define CSR_TFTP_STATE_CONNECTION_CLOSE              (0xC)

#define CSR_TFTP_PORT                                (69)
#define CSR_TFTP_DEFAULT_BLKSIZE                     (512)
#define CSR_TFTP_MIN_DATA_PACKET_LENGTH              (4)
#define CSR_TFTP_DATA_RETRANSMISSION_TIMEOUT         ((CsrTime) (3 * CSR_SCHED_SECOND))
#define CSR_TFTP_MAX_NUMBER_OF_RETRANSMISSIONS       (3)
#define CSR_TFTP_INVALID_SOCKET_HANDLE               (0xFFFF)

#define CSR_TFTP_OPCODE_DATA                         (3)
#define CSR_TFTP_OPCODE_ACK                          (4)
#define CSR_TFTP_OPCODE_ERROR                        (5)

#define CSR_TFTP_ERROR_CODE_NOT_DEFINED              (0)
#define CSR_TFTP_ERROR_CODE_ILLEGAL_TFTP_OPERATION   (4)

#define CSR_TFTP_ERROR_CODE_REJECTED_NAME            "Rejected"
#define CSR_TFTP_ERROR_CODE_REJECTED_NAME_LEN        (8)

#define CSR_TFTP_MODE_OCTET_NAME                     "octet"
#define CSR_TFTP_MODE_OCTET_NAME_LEN                 (5)

typedef struct
{
    CsrUint8  state;
    CsrUint16 socketHandle;
    CsrUint8  ipAddress[4];
    CsrUint16 port;
    CsrUint16 dataLength;
    CsrUint8 *data;
    CsrBool   retransmit;
} CsrTftpUdpDataReq;

typedef struct CsrTftpFifoElement
{
    struct CsrTftpFifoElement *next;
    void                      *data;
} CsrTftpFifoElement;

typedef struct CsrTftpFifo
{
    CsrTftpFifoElement *front;
    CsrTftpFifoElement *back;
} CsrTftpFifo;

typedef struct
{
    CsrUint8 state;

    CsrTftpFifo udpDataReqList;
    CsrUint16   socketHandle;
    CsrUint8    ipAddress[16];
    CsrUint16   localPort;
} CsrTftpConnection;

typedef struct
{
    CsrTftpConnection conn;

    CsrUint16 remotePort;
    CsrBool   readRequest;

    CsrUint16 lastDataSize;
    CsrUint16 blockNumber;

    /* Retransmission */
    CsrSchedTid timer;
    CsrUint8    numberOfRetransmissions;
    CsrUint16   dataLength;
    CsrUint8   *data;
} CsrTftpConnectionClient;

typedef struct
{
    void                   *msg;
    CsrUint8                state;
    CsrSchedQid             qid;
    CsrTftpConnection       conn;
    CsrTftpConnectionClient client;
} CsrTftpInstanceData;

extern void CsrTftpConnectionInit(CsrTftpConnection *conn, CsrUint16 port);
extern void CsrTftpConnectionClientInit(CsrTftpConnectionClient *conn);

extern void CsrTftpFifoQueue(CsrTftpFifo *fifo, void *data);
extern void *CsrTftpFifoDequeue(CsrTftpFifo *fifo);

#define CSR_TFTP_UDP_DATA_QUEUE(fifo, data) (CsrTftpFifoQueue(fifo, data))
#define CSR_TFTP_UDP_DATA_DEQUEUE(fifo)     ((CsrTftpUdpDataReq *) CsrTftpFifoDequeue(fifo))

extern void CsrTftpActivateCfmSend(CsrSchedQid queue, CsrResult result);
extern void CsrTftpDeactivateCfmSend(CsrSchedQid queue);
extern void CsrTftpConnectIndSend(CsrSchedQid queue, CsrTftpFamily family, CsrUint8 *ipAddress, CsrUint16 port, CsrTftpOpcode opcode, CsrCharString *fileName);
extern void CsrTftpReadIndSend(CsrSchedQid queue);
extern void CsrTftpWriteIndSend(CsrSchedQid queue, CsrUint16 dataLength, CsrUint8 *data);
extern void CsrTftpDisconnectIndSend(CsrSchedQid queue);

#ifdef __cplusplus
}
#endif

#endif

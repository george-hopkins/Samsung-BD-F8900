/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <Windows.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_serial_com.h"
#include "csr_transport.h"

#include "csr_log_text_2.h"

#include "platform/csr_serial_init.h"

#include "csr_macro.h"

#define NUM_SIMULTANEOUS_UARTS 2

#define TX_BUF_MAX_SIZE         (9000) /* the buffer size for incoming and outgoing characters */
#define RX_BUF_MAX_SIZE         (9000)

#define CLOSE_DOWN_EVENT         0
#define WROTE_DATA_EVENT         1
#define NEW_TX_DATA_EVENT        2
#define NO_OF_TX_EVENTS          3
#define RX_DATA_EVENT            1
#define DATA_READ_EVENT          2
#define NO_OF_RX_EVENTS          3

/* Log Text */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrSerialComLto);

static CsrBool instInUse[NUM_SIMULTANEOUS_UARTS] = {FALSE, FALSE};
static CsrUint8 instIdx[NUM_SIMULTANEOUS_UARTS];

/* the UART file handler */
static HANDLE comHandle[NUM_SIMULTANEOUS_UARTS] = {INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE};

/* internal events used to signal the thread */
static HANDLE newTxDataEvent[NUM_SIMULTANEOUS_UARTS];
static HANDLE txCloseDownEvent[NUM_SIMULTANEOUS_UARTS];
static HANDLE txDownEvent[NUM_SIMULTANEOUS_UARTS];
static HANDLE rxCloseDownEvent[NUM_SIMULTANEOUS_UARTS];
static HANDLE rxDownEvent[NUM_SIMULTANEOUS_UARTS];
static HANDLE dataReadEvent[NUM_SIMULTANEOUS_UARTS];

/* ensure mutual exclusion for write and read */
static CRITICAL_SECTION txMutex[NUM_SIMULTANEOUS_UARTS];
static CRITICAL_SECTION rxMutex[NUM_SIMULTANEOUS_UARTS];

/*    the threads for rx and tx */
static HANDLE txThread[NUM_SIMULTANEOUS_UARTS];
static HANDLE rxThread[NUM_SIMULTANEOUS_UARTS];

static CsrUint32 txIn[NUM_SIMULTANEOUS_UARTS];
static CsrUint32 txOut[NUM_SIMULTANEOUS_UARTS];
static CsrUint32 rxIn[NUM_SIMULTANEOUS_UARTS];
static CsrUint32 rxOut[NUM_SIMULTANEOUS_UARTS];
static CsrUint32 txSize[NUM_SIMULTANEOUS_UARTS];
static CsrUint32 rxSize[NUM_SIMULTANEOUS_UARTS];
static CsrUint8 txBuf[NUM_SIMULTANEOUS_UARTS][TX_BUF_MAX_SIZE];
static CsrUint8 rxBuf[NUM_SIMULTANEOUS_UARTS][RX_BUF_MAX_SIZE];

static CsrUint32 curBaud[NUM_SIMULTANEOUS_UARTS];
static CsrUint32 instBaud[NUM_SIMULTANEOUS_UARTS];
static CsrUint32 instResetBaud[NUM_SIMULTANEOUS_UARTS];
static CsrUartDrvDataBits instDataBits[NUM_SIMULTANEOUS_UARTS];
static CsrUartDrvParity instParity[NUM_SIMULTANEOUS_UARTS];
static CsrUartDrvStopBits instStopBits[NUM_SIMULTANEOUS_UARTS];
static CsrBool instFlowControl[NUM_SIMULTANEOUS_UARTS];
static CsrUint8 instToken[NUM_SIMULTANEOUS_UARTS];
static CsrBool instUseToken[NUM_SIMULTANEOUS_UARTS];

static CsrSchedBgint rxBgint[NUM_SIMULTANEOUS_UARTS] = {CSR_SCHED_BGINT_INVALID, CSR_SCHED_BGINT_INVALID};
static CsrUartDrvDataRx rxDataCb[NUM_SIMULTANEOUS_UARTS] = {NULL, NULL};

static CsrCharString instDevice[NUM_SIMULTANEOUS_UARTS][128];

static CsrBool handleRxData(CsrUint8 idx, DWORD len, CsrUint8 *data)
{
    EnterCriticalSection(&rxMutex[idx]);
    rxSize[idx] = rxSize[idx] + (CsrUint32) len;
    LeaveCriticalSection(&rxMutex[idx]);

    if ((rxIn[idx] + len) == RX_BUF_MAX_SIZE)
    {
        rxIn[idx] = 0;
    }
    else
    {
        rxIn[idx] = rxIn[idx] + (CsrUint32) len;
    }

    if (instUseToken[idx] &&
        (memchr(data, instToken[idx], len) == NULL))
    {
        return TRUE;
    }

    CsrSchedBgintSet(rxBgint[idx]);
    return TRUE;
}

static void txThreadFunc(void *handle)
{
    OVERLAPPED osWrote = {0};
    HANDLE wroteDataEvent;
    DWORD bytesWritten;
    DWORD event;
    CsrBool txBusy;
    CsrUint8 idx = *((CsrUint8 *) handle);
    HANDLE txEvents[NO_OF_TX_EVENTS];

    osWrote.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    wroteDataEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if ((osWrote.hEvent == NULL) || (wroteDataEvent == NULL))
    {
        CSR_LOG_TEXT_CRITICAL((CsrSerialComLto, 0,
                               "TX thread event create failed"));
        exit(0);
    }

    txEvents[CLOSE_DOWN_EVENT] = txCloseDownEvent[idx];
    txEvents[WROTE_DATA_EVENT] = osWrote.hEvent;
    txEvents[NEW_TX_DATA_EVENT] = newTxDataEvent[idx];

    txBusy = FALSE;

    while (TRUE)
    {
        event = WaitForMultipleObjects(NO_OF_TX_EVENTS, txEvents, FALSE, INFINITE);
        switch (event)
        {
            case WAIT_OBJECT_0 + CLOSE_DOWN_EVENT:
            {
                ResetEvent(txCloseDownEvent[idx]);
                break;
            }
            case WAIT_OBJECT_0 + WROTE_DATA_EVENT:
            {
                BOOL success;

                ResetEvent(osWrote.hEvent);

                txBusy = FALSE;
                success = GetOverlappedResult(comHandle[idx], &osWrote, &bytesWritten, FALSE);
                if (success)
                {
                    if (bytesWritten > 0)
                    {
                        txOut[idx] = txOut[idx] + (CsrUint32) bytesWritten;
                        if (txOut[idx] >= TX_BUF_MAX_SIZE)
                        {
                            txOut[idx] = 0;
                        }
                    }
                    else
                    {
                        bytesWritten = 0;
                    }
                    EnterCriticalSection(&txMutex[idx]);
                    txSize[idx] = txSize[idx] - (CsrUint32) bytesWritten;
                    LeaveCriticalSection(&txMutex[idx]);
                }
                else
                {
                    CSR_LOG_TEXT_WARNING((CsrSerialComLto, 0,
                                          "Error when sending data"));
                }

                if (txSize[idx] > 0)
                {
                    SetEvent(newTxDataEvent[idx]);
                }
                break;
            }

            case WAIT_OBJECT_0 + NEW_TX_DATA_EVENT:
            {
                ResetEvent(newTxDataEvent[idx]);

                /*    do not try to send while busy */
                if (!txBusy)
                {
                    CsrUint32 size;
                    CsrUint32 no2Send;

                    EnterCriticalSection(&txMutex[idx]);
                    size = txSize[idx];
                    LeaveCriticalSection(&txMutex[idx]);

                    if (size == 0)
                    {
                        /*    no data so take another loop */
                        break;
                    }

                    no2Send = size;
                    if (size + txOut[idx] > TX_BUF_MAX_SIZE)
                    {
                        no2Send = TX_BUF_MAX_SIZE - txOut[idx];
                    }

                    if (WriteFile(comHandle[idx], &(txBuf[idx][txOut[idx]]), no2Send, &bytesWritten, &osWrote))
                    {
                        if (bytesWritten > 0)
                        {
                            txOut[idx] = txOut[idx] + (CsrUint32) bytesWritten;
                            if (txOut[idx] >= TX_BUF_MAX_SIZE)
                            {
                                txOut[idx] = 0;
                            }
                        }
                        else
                        {
                            bytesWritten = 0;
                        }
                        EnterCriticalSection(&txMutex[idx]);
                        txSize[idx] = txSize[idx] - (CsrUint32) bytesWritten;
                        LeaveCriticalSection(&txMutex[idx]);
                        if (txSize[idx] > 0)
                        {
                            SetEvent(newTxDataEvent[idx]);
                        }
                    }
                    else
                    {
                        if (GetLastError() != ERROR_IO_PENDING)
                        {
                            CSR_LOG_TEXT_WARNING((CsrSerialComLto, 0,
                                                  "Serious error in write file (new data event) in tx thread"));
                            break;
                        }
                        else
                        {
                            /*    write pending */
                            txBusy = TRUE;
                        }
                    }
                }
                break;
            }

            default:
            {
                CSR_LOG_TEXT_WARNING((CsrSerialComLto, 0,
                                      "Unknown event"));
                break;
            }
        }    /*    end switch */
        if (event == WAIT_OBJECT_0 + CLOSE_DOWN_EVENT)
        {
            break;
        }
    }    /*    end while */

    CloseHandle(osWrote.hEvent);
    CloseHandle(wroteDataEvent);

    SetEvent(txDownEvent[idx]);

    ExitThread(0);
}

static void rxThreadFunc(void *handle)
{
    CsrUint8 *rxData;
    OVERLAPPED osRead = {0};
    HANDLE rxEvents[NO_OF_RX_EVENTS];
    CsrUint8 idx;

    idx = *((CsrUint8 *) handle);

    osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if ((osRead.hEvent == NULL))
    {
        CSR_LOG_TEXT_CRITICAL((CsrSerialComLto, 0,
                               "RX thread event create failed"));
        exit(0);
    }

    rxEvents[CLOSE_DOWN_EVENT] = rxCloseDownEvent[idx];
    rxEvents[RX_DATA_EVENT] = osRead.hEvent;
    rxEvents[DATA_READ_EVENT] = dataReadEvent[idx];

    rxData = rxBuf[idx];
    rxSize[idx] = 0;

    while (TRUE)
    {
        CsrUint32 theRxSize, theRxFreeSize;
        CsrBool startRead, asyncRead /*lint -esym(644,asyncRead) */;

        if (WaitForSingleObject(rxCloseDownEvent[idx], 0) == WAIT_OBJECT_0)
        {
            ResetEvent(rxCloseDownEvent[idx]);
            break;
        }

        EnterCriticalSection(&rxMutex[idx]);
        theRxSize = rxSize[idx];
        LeaveCriticalSection(&rxMutex[idx]);
        theRxFreeSize = RX_BUF_MAX_SIZE - theRxSize;

        /*    find next position for rx data in and if any space available in buffer */
        if (theRxSize < RX_BUF_MAX_SIZE)
        {
            startRead = TRUE;
        }
        else
        {
            /*    wait for available space in the buffer */
            startRead = FALSE;
        }

        /*    kick the read process if allowed (space left in rx buffer) */
        if (startRead)
        {
            DWORD bytesRead, numberToRead;
            DWORD errors;
            COMSTAT comStat;
            CsrBool readSuccess;

            rxData = rxBuf[idx] + rxIn[idx];

            /* find out how many to read */
            if (!ClearCommError(comHandle[idx], &errors, &comStat))
            {
                /* serious error */
                CSR_LOG_TEXT_WARNING((CsrSerialComLto, 0,
                                      "Serious error in read file in rx thread"));
            }
            numberToRead = comStat.cbInQue > 0 ? comStat.cbInQue : 1;
            numberToRead = CSRMIN(numberToRead, theRxFreeSize);

            if (rxIn[idx] + numberToRead > RX_BUF_MAX_SIZE)
            {
                numberToRead = RX_BUF_MAX_SIZE - rxIn[idx];
            }

            readSuccess = ReadFile(comHandle[idx], rxData, numberToRead, &bytesRead, &osRead);
            if (!readSuccess)
            {
                asyncRead = TRUE;

                if (GetLastError() != ERROR_IO_PENDING)
                {
                    CSR_LOG_TEXT_WARNING((CsrSerialComLto, 0,
                                          "Serious error in read file in rx thread"));
                }
            }
            else
            {
                asyncRead = FALSE;

                if (bytesRead > 0)
                {
                    handleRxData(idx, bytesRead, rxData);
                }

                ResetEvent(osRead.hEvent);
            }
        }

        if (!startRead || asyncRead)
        {
            DWORD event;

            do
            {
                event = WaitForMultipleObjects(NO_OF_RX_EVENTS, rxEvents, FALSE, INFINITE);
                switch (event)
                {
                    case CLOSE_DOWN_EVENT:
                    {
                        ResetEvent(rxCloseDownEvent[idx]);
                        break;
                    }
                    case RX_DATA_EVENT:
                    {
                        DWORD bytesRead;
                        CsrBool success;

                        /*    receive complete, store the data */
                        success = GetOverlappedResult(comHandle[idx], &osRead, &bytesRead, FALSE);

                        if ((bytesRead > 0) && success)
                        {
                            handleRxData(idx, bytesRead, rxData);
                        }

                        startRead = FALSE;
                        ResetEvent(osRead.hEvent);
                        break;
                    }
                    case DATA_READ_EVENT:
                    {
                        /* sufficient data has been read from the rx buffer to restart the reader thread */
                        ResetEvent(dataReadEvent[idx]);
                        break;
                    }
                    default:
                    {
                        /*    error occurred */
                        CSR_LOG_TEXT_WARNING((CsrSerialComLto, 0,
                                              "Unknown event"));
                        break;
                    }
                }
            } while (startRead && (event != CLOSE_DOWN_EVENT));

            if (event == CLOSE_DOWN_EVENT)
            {
                break;
            }
        }
    }    /*    end while */

    CloseHandle(osRead.hEvent);

    SetEvent(rxDownEvent[idx]);

    ExitThread(0);
}

static void clearBuffer(CsrUint8 idx)
{
    if (comHandle[idx] != INVALID_HANDLE_VALUE)
    {
        /* flush the port for any operations waiting and any data */
        PurgeComm(comHandle[idx], PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
    }
    /*    the internal queue pointers */
    txSize[idx] = 0;
    rxSize[idx] = 0;
    rxIn[idx] = 0;
    rxOut[idx] = 0;
    txIn[idx] = 0;
    txOut[idx] = 0;
}

static CsrBool init(CsrUint8 idx, CsrUint32 baud)
{
    CsrBool success = TRUE;
    DCB dcb;
    COMMTIMEOUTS comTimeouts;

    clearBuffer(idx);

    /*    mutex for sync of write and read */
    InitializeCriticalSection(&rxMutex[idx]);
    InitializeCriticalSection(&txMutex[idx]);

    /*    event for internal communication */
    newTxDataEvent[idx] = CreateEvent(NULL, TRUE, FALSE, NULL);
    txCloseDownEvent[idx] = CreateEvent(NULL, TRUE, FALSE, NULL);
    txDownEvent[idx] = CreateEvent(NULL, TRUE, FALSE, NULL);
    rxCloseDownEvent[idx] = CreateEvent(NULL, TRUE, FALSE, NULL);
    rxDownEvent[idx] = CreateEvent(NULL, TRUE, FALSE, NULL);
    dataReadEvent[idx] = CreateEvent(NULL, TRUE, FALSE, NULL);
    if ((newTxDataEvent[idx] == NULL) ||
        (txCloseDownEvent[idx] == NULL) ||
        (rxCloseDownEvent[idx] == NULL) ||
        (txDownEvent[idx] == NULL) ||
        (rxDownEvent[idx] == NULL) ||
        (dataReadEvent[idx] == NULL))
    {
        CSR_LOG_TEXT_CRITICAL((CsrSerialComLto, 0,
                               "Create event failure in init"));
        return FALSE;
    }

    /*    create the file handle */
    comHandle[idx] = CreateFile(instDevice[idx], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (comHandle[idx] == INVALID_HANDLE_VALUE)
    {
        CSR_LOG_TEXT_CRITICAL((CsrSerialComLto, 0,
                               "Create file handle failed"));
        return FALSE;
    }

    if (!GetCommState(comHandle[idx], &dcb))
    {
        success = FALSE;
        CSR_LOG_TEXT_CRITICAL((CsrSerialComLto, 0,
                               "Can not read current DCB"));
    }

    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fDsrSensitivity = 0;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;

    if (instFlowControl[idx])
    {
        dcb.fOutxCtsFlow = TRUE;
        dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
        dcb.XonLim = 0;
        dcb.XoffLim = 0;
    }
    else
    {
        dcb.fOutxCtsFlow = FALSE;
        dcb.fRtsControl = RTS_CONTROL_DISABLE;
    }

    dcb.BaudRate = baud;
    dcb.ByteSize = instDataBits[idx];
    dcb.Parity = instParity[idx];
    dcb.StopBits = instStopBits[idx];

    comTimeouts.ReadIntervalTimeout = MAXDWORD;
    comTimeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
    comTimeouts.ReadTotalTimeoutConstant = 300000;
    comTimeouts.WriteTotalTimeoutMultiplier = 0;
    comTimeouts.WriteTotalTimeoutConstant = 0;

    if (!SetCommMask(comHandle[idx], 0))
    {
        success = FALSE;
        CSR_LOG_TEXT_WARNING((CsrSerialComLto, 0,
                              "SetCommMask failed during initialisation"));
    }

    if (!SetCommTimeouts(comHandle[idx], &comTimeouts))
    {
        success = FALSE;
        CSR_LOG_TEXT_WARNING((CsrSerialComLto, 0,
                              "SetCommTimeouts failed during initialisation"));
    }

    if (!SetCommState(comHandle[idx], &dcb))
    {
        success = FALSE;
        CSR_LOG_TEXT_WARNING((CsrSerialComLto, 0,
                              "SetCommState failed during initialisation"));
    }

    if (success == FALSE)
    {
        CloseHandle(comHandle[idx]);
        comHandle[idx] = INVALID_HANDLE_VALUE;
    }

    return success;
}

CsrBool CsrUartDrvStart(void *handle, CsrUint8 reset)
{
    DWORD threadId;
    CsrUint32 baud;
    CsrUint8 idx = *((CsrUint8 *) handle);

    if (reset == 0)
    {
        baud = instBaud[idx];
    }
    else
    {
        baud = instResetBaud[idx];
    }

    curBaud[idx] = baud;

    if (!init(idx, baud))
    {
        return FALSE;
    }

    /*    tx and rx threads */
    txThread[idx] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) txThreadFunc, handle, 0, &threadId);
    if (txThread[idx] == INVALID_HANDLE_VALUE)
    {
        CSR_LOG_TEXT_CRITICAL((CsrSerialComLto, 0,
                               "TX thread create failed"));
        return FALSE;
    }
    rxThread[idx] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) rxThreadFunc, handle, 0, &threadId);
    if (rxThread[idx] == INVALID_HANDLE_VALUE)
    {
        CSR_LOG_TEXT_CRITICAL((CsrSerialComLto, 0,
                               "RX thread create failed"));
        return FALSE;
    }

    return TRUE;
}

CsrBool CsrUartDrvStop(void *handle)
{
    DWORD threadExitCode;
    CsrUint8 idx = *((CsrUint8 *) handle);

    SetEvent(txCloseDownEvent[idx]);
    SetEvent(rxCloseDownEvent[idx]);

    while (WaitForSingleObject(txDownEvent[idx], INFINITE) != WAIT_OBJECT_0)
    {
        printf("WaitForSingleObject(txDownEvent, INFINITE) NOT SIGNALED????\n");
    }
    do
    {
        if (!GetExitCodeThread(txThread[idx], &threadExitCode))
        {
            printf("GetExitCodeThread failed!\n");
            break;
        }
        else
        {
            if (threadExitCode == STILL_ACTIVE)
            {
                Sleep(0);
            }
        }
    } while (threadExitCode == STILL_ACTIVE);
    CloseHandle(newTxDataEvent[idx]);
    CloseHandle(txCloseDownEvent[idx]);
    CloseHandle(txDownEvent[idx]);
    CloseHandle(txThread[idx]);

    while (WaitForSingleObject(rxDownEvent[idx], INFINITE) != WAIT_OBJECT_0)
    {
        printf("WaitForSingleObject(rxDownEvent, INFINITE) NOT SIGNALED????\n");
    }
    do
    {
        if (!GetExitCodeThread(rxThread[idx], &threadExitCode))
        {
            printf("GetExitCodeThread failed!\n");
            break;
        }
        else
        {
            if (threadExitCode == STILL_ACTIVE)
            {
                Sleep(0);
            }
        }
    } while (threadExitCode == STILL_ACTIVE);
    CloseHandle(rxCloseDownEvent[idx]);
    CloseHandle(rxDownEvent[idx]);
    CloseHandle(dataReadEvent[idx]);
    CloseHandle(rxThread[idx]);

    CloseHandle(comHandle[idx]);
    comHandle[idx] = INVALID_HANDLE_VALUE;
    curBaud[idx] = 0;

    DeleteCriticalSection(&rxMutex[idx]);
    DeleteCriticalSection(&txMutex[idx]);

    return TRUE;
}

CsrBool CsrUartDrvTx(void *handle, const CsrUint8 *data, CsrUint32 dataLength, CsrUint32 *numSent)
{
    CsrUint32 size;
    CsrUint8 idx = *((CsrUint8 *) handle);

    *numSent = 0;
    EnterCriticalSection(&txMutex[idx]);
    size = txSize[idx];
    LeaveCriticalSection(&txMutex[idx]);

    /* check if enough space in buffer for new data - if not return FALSE */
    if (dataLength + size > TX_BUF_MAX_SIZE)
    {
        return FALSE;
    }

    if (txIn[idx] + dataLength > TX_BUF_MAX_SIZE)
    {
        CsrUint32 remainingLength;

        memcpy(&txBuf[idx][txIn[idx]], data, TX_BUF_MAX_SIZE - txIn[idx]);
        remainingLength = dataLength - (TX_BUF_MAX_SIZE - txIn[idx]);
        memcpy(txBuf[idx], &data[TX_BUF_MAX_SIZE - txIn[idx]], remainingLength);
        txIn[idx] = remainingLength;
    }
    else
    {
        memcpy(&txBuf[idx][txIn[idx]], data, dataLength);
        if (txIn[idx] + dataLength == TX_BUF_MAX_SIZE)
        {
            txIn[idx] = 0;
        }
        else
        {
            txIn[idx] += (CsrUint32) dataLength;
        }
    }
    *numSent = dataLength;

    EnterCriticalSection(&txMutex[idx]);
    txSize[idx] = txSize[idx] + (*numSent);
    LeaveCriticalSection(&txMutex[idx]);

    /*    signal tx thread that new data has arrived */
    SetEvent(newTxDataEvent[idx]);
    return TRUE;
}

CsrUint32 CsrUartDrvGetTxSpace(void *handle)
{
    CsrUint32 size;
    CsrUint8 idx = *((CsrUint8 *) handle);

    EnterCriticalSection(&txMutex[idx]);
    size = TX_BUF_MAX_SIZE - txSize[idx];
    LeaveCriticalSection(&txMutex[idx]);

    return size;
}

CsrUint32 CsrUartDrvGetRxAvailable(void *handle)
{
    CsrUint32 theSize;
    CsrUint8 idx = *((CsrUint8 *) handle);

    EnterCriticalSection(&rxMutex[idx]);
    theSize = rxSize[idx];
    LeaveCriticalSection(&rxMutex[idx]);

    return theSize;
}

/*************************************************************************
    void CsrUartDrvRx(void *handle)

    Takes the bytes from the input queue ( CSRMAX BytesToAbcsp numbers)
    send it to the ABCSP. ABCSP returns the number consumed this is used
    for updating the pointers / number of bytes in queue.

  RETURN
    None
*************************************************************************/
void CsrUartDrvRx(void *handle)
{
    CsrUint32 bytesConsumed;
    CsrUint32 noOfBytes;
    CsrUint8 idx = *((CsrUint8 *) handle);

    EnterCriticalSection(&rxMutex[idx]);
    noOfBytes = rxSize[idx];
    LeaveCriticalSection(&rxMutex[idx]);

    if (noOfBytes > (RX_BUF_MAX_SIZE - rxOut[idx]))
    {
        noOfBytes = (RX_BUF_MAX_SIZE - rxOut[idx]);
    }
    bytesConsumed = rxDataCb[idx](&rxBuf[idx][rxOut[idx]], noOfBytes);
    rxOut[idx] = rxOut[idx] + bytesConsumed;

    if (rxOut[idx] >= RX_BUF_MAX_SIZE)
    {
        rxOut[idx] = 0x00; /* Buffer start from beginning*/
    }

    EnterCriticalSection(&rxMutex[idx]);
    rxSize[idx] = rxSize[idx] - bytesConsumed;
    LeaveCriticalSection(&rxMutex[idx]);

    if (bytesConsumed > 0)
    {
        SetEvent(dataReadEvent[idx]);
    }

    if (rxSize[idx])
    {
        CsrSchedBgintSet(rxBgint[idx]);
    }
}

CsrUint32 CsrUartDrvLowLevelTransportTxBufLevel(void *handle)
{
    CsrUint32 bufferLevel;
    CsrUint8 idx = *((CsrUint8 *) handle);
    EnterCriticalSection(&txMutex[idx]);
    bufferLevel = txSize[idx];
    LeaveCriticalSection(&txMutex[idx]);
    return bufferLevel;
}

CsrUint32 CsrUartDrvLowLevelTransportRx(void *handle, CsrUint32 dataLength, CsrUint8 *data)
{
    CsrUint32 rxCount;
    CsrUint8 idx = *((CsrUint8 *) handle);

    EnterCriticalSection(&rxMutex[idx]);
    rxCount = rxSize[idx];
    LeaveCriticalSection(&rxMutex[idx]);

    if (rxCount > dataLength)
    {
        rxCount = dataLength;
    }

    if (rxCount > (RX_BUF_MAX_SIZE - rxOut[idx]))
    {
        rxCount = (RX_BUF_MAX_SIZE - rxOut[idx]);
    }
    memcpy(data, &rxBuf[idx][rxOut[idx]], rxCount);

    rxOut[idx] = rxOut[idx] + rxCount;

    if (rxOut[idx] >= RX_BUF_MAX_SIZE)
    {
        rxOut[idx] = 0x00; /* Buffer start from beginning*/
    }

    EnterCriticalSection(&rxMutex[idx]);
    rxSize[idx] = rxSize[idx] - rxCount;
    LeaveCriticalSection(&rxMutex[idx]);

    if (rxCount > 0)
    {
        SetEvent(dataReadEvent[idx]);
    }

    if (rxSize[idx])
    {
        CsrSchedBgintSet(rxBgint[idx]);
    }
    return rxCount;
}

CsrUint32 CsrUartDrvGetBaudrate(void *handle)
{
    CsrUint8 idx = *((CsrUint8 *) handle);
    return curBaud[idx];
}

void CsrUartDrvRegister(void *handle, CsrUartDrvDataRx rxDataFn, CsrSchedBgint rxBgintHdl)
{
    CsrUint8 idx = *((CsrUint8 *) handle);
    rxDataCb[idx] = rxDataFn;
    rxBgint[idx] = rxBgintHdl;
}

void *CsrUartDrvInitialise(const CsrCharString *device,
    CsrUint32 baud, CsrUint32 resetBaud,
    CsrUartDrvDataBits dataBits,
    CsrUartDrvParity parity,
    CsrUartDrvStopBits stopBits,
    CsrBool flowControl,
    const CsrUint8 *token)
{
    CsrUint8 idx;

    CSR_LOG_TEXT_REGISTER(&CsrSerialComLto, "SERIAL_COM", 0, NULL);

    for (idx = 0; idx < NUM_SIMULTANEOUS_UARTS; idx++)
    {
        if (!instInUse[idx])
        {
            instInUse[idx] = TRUE;
            instIdx[idx] = idx;

            curBaud[idx] = 0;
            instBaud[idx] = baud;
            instResetBaud[idx] = resetBaud;
            instDataBits[idx] = dataBits;
            instParity[idx] = parity;
            instStopBits[idx] = stopBits;
            instFlowControl[idx] = flowControl;
            if (token != NULL)
            {
                instUseToken[idx] = TRUE;
                instToken[idx] = *token;
            }
            sprintf(instDevice[idx], "\\\\.\\");
            strcat(instDevice[idx], device);

            return &instIdx[idx];
        }
    }
    return NULL;
}

void CsrUartDrvDeinitialise(void *handle)
{
    CsrUint8 idx = *((CsrUint8 *) handle);
    instInUse[idx] = FALSE;
}

CsrUint32 CsrUartDrvBaudrateActualGet(CsrUint32 wantedBaudrate)
{
    return wantedBaudrate;
}

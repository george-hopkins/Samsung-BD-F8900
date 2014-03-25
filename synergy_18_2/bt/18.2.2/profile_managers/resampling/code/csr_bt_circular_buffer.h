#ifndef CSR_BT_CIRCULAR_BUFFER_H__
#define CSR_BT_CIRCULAR_BUFFER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
#include "csr_pmem.h"


#define CIRC_BUFFER_ERROR            0
#define CIRC_BUFFER_SUCCESS           1
#define CIRC_BUFFER_DATA             2
#define CIRC_BUFFER_NO_DATA          3
#define CIRC_TOTAL_EMPTY             0
#define CIRC_TOTAL_FULL              1
#define CIRC_NORMAL                  2


/* Circurlar buffer */
typedef struct
{
    CsrUint32    head;       /* Head point at the first free element */
    CsrUint32    tail;       /* Tail point oldest element in the buffer */
    CsrUint32    readSize;   /* Number of samples to read each time CircBufferGet is called */
    CsrUint32    writeSize;  /* Number of samples to write each time CircBufferPut is called */
    CsrUint32    bufferSize; /* Total size of buffer */
    CsrUint8     flag;       /* Indication of buffer status total empty (TOTAL_EMPTY), total full (TOTAL_FULL) or (NORMAL) */
    CsrInt16     *data;      /* Point to data */
} circBuf_t;

/*--------------------------------------------------------------------------*
 *  NAME
 *      CircBufferInit
 *
 *  DESCRIPTION
 *      Initilization of buffer variables and allocate memory
 *
 *  PARAMETERS
 *      writeSize:      Size of input data
 *      readSize:       Number of data samples to be read
 *      bufferSize:     Size of buffer
 *      circBuf_t:      Pointer to circular buffers instance data
 *--------------------------------------------------------------------------*/
circBuf_t* CircBufferInit(CsrUint32 writeSize, CsrUint32 readSize, CsrUint32 bufferSize);

/*--------------------------------------------------------------------------*
 *  NAME
 *      CircBufferDeInit
 *
 *  DESCRIPTION
 *      Free allocated memory
 *
 *  PARAMETERS
 *      inst:       Instance data
 *      return:     Status code {BUFFER_ERROR, BUFFER_SUCCESS}
 *--------------------------------------------------------------------------*/
CsrUint8 CircBufferDeInit(circBuf_t *instanceData);

/*--------------------------------------------------------------------------*
 *  NAME
 *      CircBufferPut
 *
 *  DESCRIPTION
 *      Insert one 16bit sample into the buffer. If the buffer is full and
 *      bufferPut is called data is NOT written into the buffer, an error
 *      code is returned to the calling function.
 *      NOTE!!!. It is only possible to write one sample at the time.
 *
 *  RETURNS
 *      Function status {BUFFER_ERROR, BUFFER_SUCCESS}
 *--------------------------------------------------------------------------*/
CsrUint8 CircBufferPut(circBuf_t *instanceData, CsrInt16 *data);

/*--------------------------------------------------------------------------*
 *  NAME
 *      CircBufferGet
 *
 *  DESCRIPTION
 *      bufferGet read data form the buffer at write it into the input array
 *      variable data. The numbere af data read is defined at initilatzation
 *      of the buffer. If it is possible to read new data the return value will
 *      be DATA_READY.
 *
 *  RETURNS
 *      Function status {BUFFER_ERROR, BUFFER_DATA_READY, BUFFER_DATA_NOT_READY}
 *--------------------------------------------------------------------------*/
CsrUint8  CircBufferGet(circBuf_t *instanceData, CsrInt16 *data);

#ifdef __cplusplus
}
#endif


#endif     /* CIRCULAR_BUFFER */


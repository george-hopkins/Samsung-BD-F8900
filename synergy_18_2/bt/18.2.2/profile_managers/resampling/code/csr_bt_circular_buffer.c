/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
/* */
#include "csr_bt_circular_buffer.h"

/*
 *  CircBufferInit
 **/
circBuf_t * CircBufferInit(CsrUint32 writeSize, CsrUint32 readSize, CsrUint32 bufferSize)
{
    circBuf_t *inst = NULL;

    inst                = CsrPmemAlloc(sizeof(circBuf_t));
    if(inst == NULL)
    {
        /* ERROR! - Not possible to allocate memory */
    }
    else
    {
        inst->writeSize     = writeSize;
        inst->readSize      = readSize;
        inst->bufferSize    = bufferSize;
        inst->head          = 0;
        inst->tail          = 0;
        inst->flag          = CIRC_TOTAL_EMPTY;
        inst->data          = CsrPmemAlloc(sizeof(CsrInt16)*inst->bufferSize);
    }

    return inst;
}

/*
 * CircBufferDeInit
 */
CsrUint8 CircBufferDeInit(circBuf_t *inst)
{
    /* Free buffer data */
    CsrPmemFree(inst->data);
    /* Free instance data */
    CsrPmemFree(inst);

    return CIRC_BUFFER_SUCCESS;
}

/*
 *  CircBufferPut. NOTE that it is only possible to put one sample at the time
 **/
CsrUint8 CircBufferPut(circBuf_t *inst, CsrInt16 *data)
{
    if( (inst->head == inst->tail) && (inst->flag == CIRC_TOTAL_FULL) )
    {
        /* ERROR! - Buffer overload */
        return CIRC_BUFFER_ERROR;
    }

    inst->data[inst->head++] = *data;

    /* Update buffer status */
    if( inst->head == inst->bufferSize )
    {
        inst->head = 0;
    }

    if( inst->head == inst->tail )
    {
        inst->flag  = CIRC_TOTAL_FULL;
    }
    else
    {
        inst->flag  = CIRC_NORMAL;
    }

    return CIRC_BUFFER_SUCCESS;
}

/*
 *  CircBufferGet
 **/
CsrUint8 CircBufferGet(circBuf_t *inst, CsrInt16 *data)
{
    CsrInt32 samples = 0; /* Number of samples in buffer */
    CsrInt32 i = 0;

    /* Get status of buffer */
    if(inst->head < inst->tail)
    {
        samples = inst->bufferSize + (inst->head - inst->tail);
    }
    else if(inst->head > inst->tail)
    {
        samples = inst->head - inst->tail;
    }
    else /* head == tail */
    {
        if(inst->flag == CIRC_TOTAL_FULL)
        {
            samples = inst->bufferSize;
        }
        else
        {
            return CIRC_BUFFER_NO_DATA;
        }
    }
    if( samples < (CsrInt32)inst->readSize )
    {
        return CIRC_BUFFER_NO_DATA;
    }

    /* Get data in buffer */
    for(i=0; i< (CsrInt32)inst->readSize; i++)
    {
        data[i] = inst->data[inst->tail++];
        if( inst->tail==inst->bufferSize )
        {
            inst->tail = 0;
        }
    }

    /* Update status */
    if( inst->tail == inst->head )
    {
        inst->flag = CIRC_TOTAL_EMPTY;
    }

    return CIRC_BUFFER_DATA;
}

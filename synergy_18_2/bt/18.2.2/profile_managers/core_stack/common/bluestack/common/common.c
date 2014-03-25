/*!
        (c) CSR plc 2010
        All rights reserved

\file   common.c

\brief  This file contains the common code functions.
*/

#include "csr_synergy.h"

#include <stdarg.h>
#include "csr_util.h"
#include "csr_bt_bluestack_types.h" /* path modified */
#include "bluetooth.h" /* path modified */
#include "csr_bt_common.h"
#include "error.h"
#include "csr_bt_core_stack_pmalloc.h"

#ifndef BUILD_FOR_HOST
#include "vm_const.h"
#endif

/*! Timed events */
void timer_start(CsrSchedTid *p_id,
                 CsrTime delay,
                 void (*fn)(CsrUint16, void*),
                 CsrUint16 fniarg,
                 void *fnvarg)
{
    CsrSchedTid id = CsrSchedTimerSet(delay, fn, fniarg, fnvarg);

    /* TODO debug only */
    if (p_id == NULL)
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);

    timer_cancel(p_id);
    *p_id = id;
}

void timer_cancel(CsrSchedTid *p_id)
{
    /* TODO debug only */
    if (p_id == NULL)
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);

    if (*p_id != 0)
    {
        CsrSchedTimerCancel(*p_id, NULL, NULL);
        *p_id = 0;
    }
}

#ifndef BUILD_FOR_HOST
/*----------------------------------------------------------------------------*
 *  NAME
 *      bpfree
 *
 *  DESCRIPTION
 *      Bluestack CsrPmemFree.
 *      Makes sure that we're not pointing at VM const space and then passes
 *      to CsrPmemFree. So this is not for BCHS use.
 *----------------------------------------------------------------------------*/
void bpfree(void *ptr)
{
    if (vm_const_is_pmalloc(ptr))
        CsrPmemFree(ptr);

}
#endif

/*! \brief Write a CsrUint8 to the place pointed to and increment the pointer.

    \param buf Pointer to pointer to place in buffer.
    \param val Value to be written.
*/
void write_uint8(CsrUint8 **buf, CsrUint8 val)
{
    *((*buf)++) = val & 0xFF;
}

/*! \brief Read a CsrUint8 from the place pointed to and increment the pointer.

    \param buf Pointer to pointer to place in buffer.
    \returns Value read.
*/
CsrUint8 read_uint8(const CsrUint8 **buf)
{
    return 0xFF & *((*buf)++);
}

/*! \brief Write a CsrUint16 to the place pointed to and increment the pointer.

    \param buf Pointer to pointer to place in buffer.
    \param val Value to be written.
*/
void write_uint16(CsrUint8 **buf, CsrUint16 val)
{
    write_uint8(buf, (CsrUint8)(val & 0xFF));
    write_uint8(buf, (CsrUint8)(val >> 8));
}

/*! \brief Read a CsrUint16 from the place pointed to and increment the pointer.

    \param buf Pointer to pointer to place in buffer.
    \returns Value read.
*/
CsrUint16 read_uint16(const CsrUint8 **buf)
{
    CsrUint16 val_low = read_uint8(buf);
    CsrUint16 val_high = read_uint8(buf);

    return val_low | (val_high << 8);
}

/*! \brief Write a CsrUint32 to the place pointed to and increment the pointer.

    \param buf Pointer to pointer to place in buffer.
    \param p_val Pointer to value to be written.
*/
void write_uint32(CsrUint8 **buf, CsrUint32 *p_val)
{
    write_uint16(buf, (CsrUint16)(*p_val & 0xFFFF));
    write_uint16(buf, (CsrUint16)(*p_val >> 16));
}

/*! \brief Read a CsrUint32 from the place pointed to and increment the pointer.

    \param buf Pointer to pointer to place in buffer.
    \returns Value read.
*/
CsrUint32 read_uint32(const CsrUint8 **buf)
{
    CsrUint16 val_low = read_uint16(buf);
    CsrUint16 val_high = read_uint16(buf);

    return val_low | (((CsrUint32)val_high) << 16);
}

/*! \brief Read or write a sequence of types to/from a CsrUint8 buffer.

    The var args are read using the format argument. It's best to use
    the URW_FORMAT macro to create the format. However, the details
    are explained below.

    format is interpreted as three five-bit values. In each five-bit
    value, the least significant three bits determines the quantity,
    and the most significant two the type. So from LSB to MSB, format
    is as follows:

    Bits   0-2    3-4   5-7    8-9   10-12  13-14 15
           quant1 type1 quant2 type2 quant3 type3 0(reserved)

    The function starts with the least significant 5 bits, once it has
    decremented the quantity part to zero, it shifts right by 5 and then
    repeats until it runs out of non-zero bits.

    \param format Number and types of var args.
    \param buf Pointer to pointer to buffer. Type CsrUint8**.
    \param ... variables to be written.
*/
void write_uints(CsrUint8 **buf, unsigned int format, ...)
{
    va_list ap;
    va_start(ap, format);

    /* Loop over the type/quantity pairs. */
    for(; format != 0; format >>= URW_TOTAL_BIT_OFFSET)
    {
        /* Loop through the quantity. */
        for(; (format & URW_SIZE_MASK) != 0; --format)
        {
            unsigned int type = (format & URW_TYPE_MASK);

            /* Read from var args and write to buffer. */
            if (type == URW_TYPE_CsrUint32)
            {
                write_uint32(buf, va_arg(ap, CsrUint32*));
            }
            else
            {
                unsigned int val = va_arg(ap, unsigned int);

                if (type == URW_TYPE_CsrUint8)
                {
                    write_uint8(buf, (CsrUint8)val);
                }
                else if (type == URW_TYPE_CsrUint16)
                {
                    write_uint16(buf, (CsrUint16)val);
                }
            }
        }
    }
    va_end(ap);
}

void read_uints(const CsrUint8 **buf, unsigned int format, ...)
{
    va_list ap;
    va_start(ap, format);

    /* Loop over the type/quantity pairs. */
    for (; format != 0; format >>= URW_TOTAL_BIT_OFFSET)
    {
        /* Loop through the quantity. */
        for(; (format & URW_SIZE_MASK) != 0; --format)
        {
            CsrUint32 ignore;
            void *p_val;

            /* Read address from var args. If it's NULL then we still
               proceed to read from buffer, but we ignore the result.
               This ensures that the buffer pointer gets incremented. */
            if ((p_val = va_arg(ap, void*)) == NULL)
                p_val = (void*)&ignore;

            /* Read from buffer and write to address from var args. */
            switch (format & URW_TYPE_MASK)
            {
                case URW_TYPE_CsrUint8:
                    *((CsrUint8*)p_val) = read_uint8(buf);
                    break;

                case URW_TYPE_CsrUint16:
                    *((CsrUint16*)p_val) = read_uint16(buf);
                    break;

                case URW_TYPE_CsrUint32:
                    *((CsrUint32*)p_val) = read_uint32(buf);
                    break;
            }
        }
    }

    va_end(ap);
}

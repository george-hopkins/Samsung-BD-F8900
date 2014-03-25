/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#include "platform/csr_s3c6410.h"
#include "platform/csr_i2c.h"
#include "platform/csr_delay.h"

#define I2C_ACK_ENABLE      1       /* Enable Acknowledge Generation */
#define I2C_IRQ_ENABLE      1       /* Enable Interrupt (required for proper operation) */

#define I2C_DIVIDER         0       /* 375kHz transmit speed */
#define I2C_SCALER          10

#define I2C_CONFIGURATION   ((I2C_ACK_ENABLE << 7) | (I2C_DIVIDER << 6) | (I2C_IRQ_ENABLE << 5) | I2C_SCALER)

/* Operational Mode */
#define I2C_MASTER_RX(ch)               REG_SET(rIICSTAT(ch), 7, 6, 2)
#define I2C_MASTER_TX(ch)               REG_SET(rIICSTAT(ch), 7, 6, 3)
#define I2C_IO_ENABLE(ch)               BIT_SET(rIICSTAT(ch), 4)
#define I2C_IO_DISABLE(ch)              BIT_CLEAR(rIICSTAT(ch), 4)

/* Status */
#define I2C_ACK_RECEIVED(ch)            (!BIT_CHECK(rIICSTAT(ch), 0))

/* Interrupt */
#define I2C_INTERRUPT_PENDING(ch)       BIT_CHECK(rIICCON(ch), 4)
#define I2C_RESUME(ch)                  BIT_CLEAR(rIICCON(ch), 4)

/* Line Filter */
#define I2C_LINEFILTER_ENABLE(ch)       BIT_SET(rIICLC(ch), 2)
#define I2C_LINEFILTER_DISABLE(ch)      BIT_CLEAR(rIICLC(ch), 2)

/* SDA Delay Cycles */
#define I2C_SDADELAY_SET_0(ch)          REG_SET(rIICLC(ch), 1, 0, 0)
#define I2C_SDADELAY_SET_5(ch)          REG_SET(rIICLC(ch), 1, 0, 1)
#define I2C_SDADELAY_SET_10(ch)         REG_SET(rIICLC(ch), 1, 0, 2)
#define I2C_SDADELAY_SET_15(ch)         REG_SET(rIICLC(ch), 1, 0, 3)

/* Commands */
#define I2C_START(ch)                   BIT_SET(rIICSTAT(ch), 5)
#define I2C_STOP(ch)                    BIT_CLEAR(rIICSTAT(ch), 5)

CsrResult I2CWrite(CsrUint8 channel, CsrUint8 address, CsrUint32 count, CsrUint8 *data)
{
    CsrResult res = CSR_RESULT_SUCCESS;

    /* Restrict channel to 0/1 */
    channel &= 1;

    /* Configure */
    rIICCON(channel) = I2C_CONFIGURATION;

    /* Set Master Transmit Mode */
    I2C_MASTER_TX(channel);

    /* Enable Rx/Tx */
    I2C_IO_ENABLE(channel);

    /* Write Address */
    rIICDS(channel) = (address << 1) & 0xFF;

    I2C_START(channel);

    while (!I2C_INTERRUPT_PENDING(channel))
    {
        /* Wait until address transmit completes */
    }

    if (!I2C_ACK_RECEIVED(channel))
    {
        res = CSR_RESULT_FAILURE;
    }
    else
    {
        /* Transmit Data */
        while (count--)
        {
            rIICDS(channel) = *(data++);
            I2C_RESUME(channel); /* Resume */
            while (!I2C_INTERRUPT_PENDING(channel))
            {
                /* Wait until transmit completes */
            }

            if (!I2C_ACK_RECEIVED(channel))
            {
                res = CSR_RESULT_FAILURE;
                break;
            }
        }
    }

    I2C_STOP(channel);
    I2C_RESUME(channel);

    /* Disable Rx/Tx */
    I2C_IO_DISABLE(channel);

    return res;
}

CsrResult I2CRead(CsrUint8 channel, CsrUint8 address, CsrUint32 count, CsrUint8 *data)
{
    CsrInt32 res = CSR_RESULT_SUCCESS;

    /* Restrict channel to 0/1 */
    channel &= 1;

    /* Configure */
    rIICCON(channel) = I2C_CONFIGURATION;

    /* Set Master Receive Mode */
    I2C_MASTER_RX(channel);

    /* Enable Rx/Tx */
    I2C_IO_ENABLE(channel);

    /* Write Address */
    rIICDS(channel) = (address << 1) & 0xFF;

    I2C_START(channel);

    while (!I2C_INTERRUPT_PENDING(channel))
    {
        /* Wait until address transmit completes */
    }

    if (!I2C_ACK_RECEIVED(channel))
    {
        res = CSR_RESULT_FAILURE;
    }
    else
    {
        /* Receive Data */
        while (count--)
        {
            I2C_RESUME(channel); /* Resume */
            while (!I2C_INTERRUPT_PENDING(channel))
            {
                /* Wait until receive completes */
            }
            if (!I2C_ACK_RECEIVED(channel))
            {
                res = CSR_RESULT_FAILURE;
                break;
            }
            *(data++) = rIICDS(channel);
        }
    }

    I2C_STOP(channel);
    I2C_RESUME(channel);

    /* Disable Rx/Tx */
    I2C_IO_DISABLE(channel);

    return res;
}

/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#include "platform/csr_s3c24a0.h"
#include "platform/csr_i2c.h"
#include "platform/csr_delay.h"

#define I2C_ACK_ENABLE      1       /* Enable Acknowledge Generation */
#define I2C_IRQ_ENABLE      1       /* Enable Interrupt (required for proper operation) */

#define I2C_DIVIDER         0       /* 390.625 kHz transmit speed */
#define I2C_SCALER          15

#define I2C_CONFIGURATION   ((I2C_ACK_ENABLE << 7) | (I2C_DIVIDER << 6) | (I2C_IRQ_ENABLE << 5) | I2C_SCALER)

/* Operational Mode */
#define I2C_MASTER_RX()                 REG_SET(rIICSTAT, 7, 6, 2)
#define I2C_MASTER_TX()                 REG_SET(rIICSTAT, 7, 6, 3)
#define I2C_IO_ENABLE()                 BIT_SET(rIICSTAT, 4)
#define I2C_IO_DISABLE()                BIT_CLEAR(rIICSTAT, 4)

/* Status */
#define I2C_ACK_RECEIVED()              (!BIT_CHECK(rIICSTAT, 0))

/* Interrupt */
#define I2C_INTERRUPT_PENDING()         BIT_CHECK(rIICCON, 4)
#define I2C_RESUME()                    BIT_CLEAR(rIICCON, 4)

/* Line Filter */
#define I2C_LINEFILTER_ENABLE()         BIT_SET(rSDADLY, 2)
#define I2C_LINEFILTER_DISABLE()        BIT_CLEAR(rSDADLY, 2)

/* SDA Delay Cycles */
#define I2C_SDADELAY_SET_0()            REG_SET(rSDADLY, 1, 0, 0)
#define I2C_SDADELAY_SET_5()            REG_SET(rSDADLY, 1, 0, 1)
#define I2C_SDADELAY_SET_10()           REG_SET(rSDADLY, 1, 0, 2)
#define I2C_SDADELAY_SET_15()           REG_SET(rSDADLY, 1, 0, 3)

/* Commands */
#define I2C_START()                     BIT_SET(rIICSTAT, 5)
#define I2C_STOP()                      BIT_CLEAR(rIICSTAT, 5)

CsrResult I2CWrite(CsrUint8 channel, CsrUint8 address, CsrUint32 count, CsrUint8 *data)
{
    CsrResult res = CSR_RESULT_SUCCESS;

    /* Configure */
    rIICCON = I2C_CONFIGURATION;

    /* Set Master Transmit Mode */
    I2C_MASTER_TX();

    /* Enable Rx/Tx */
    I2C_IO_ENABLE();

    /* Write Address */
    rIICDS = (address << 1) & 0xFF;

    I2C_START();

    while (!I2C_INTERRUPT_PENDING())
    {
        /* Wait until address transmit completes */
    }

    if (!I2C_ACK_RECEIVED())
    {
        res = CSR_RESULT_FAILURE;
    }
    else
    {
        /* Transmit Data */
        while (count--)
        {
            rIICDS = *(data++);
            I2C_RESUME(); /* Resume */
            while (!I2C_INTERRUPT_PENDING())
            {
                /* Wait until transmit completes */
            }

            if (!I2C_ACK_RECEIVED())
            {
                res = CSR_RESULT_FAILURE;
                break;
            }
        }
    }

    I2C_STOP();
    I2C_RESUME();

    /* Disable Rx/Tx */
    I2C_IO_DISABLE();

    return res;
}

CsrResult I2CRead(CsrUint8 channel, CsrUint8 address, CsrUint32 count, CsrUint8 *data)
{
    CsrInt32 res = CSR_RESULT_SUCCESS;

    /* Configure */
    rIICCON = I2C_CONFIGURATION;

    /* Set Master Receive Mode */
    I2C_MASTER_RX();

    /* Enable Rx/Tx */
    I2C_IO_ENABLE();

    /* Write Address */
    rIICDS = (address << 1) & 0xFF;

    I2C_START();

    while (!I2C_INTERRUPT_PENDING())
    {
        /* Wait until address transmit completes */
    }

    if (!I2C_ACK_RECEIVED())
    {
        res = CSR_RESULT_FAILURE;
    }
    else
    {
        /* Receive Data */
        while (count--)
        {
            I2C_RESUME(); /* Resume */
            while (!I2C_INTERRUPT_PENDING())
            {
                /* Wait until receive completes */
            }
            if (!I2C_ACK_RECEIVED())
            {
                res = CSR_RESULT_FAILURE;
                break;
            }
            *(data++) = rIICDS;
        }
    }

    I2C_STOP();
    I2C_RESUME();

    /* Disable Rx/Tx */
    I2C_IO_DISABLE();

    return res;
}

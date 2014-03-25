#ifndef CSR_I2C_H__
#define CSR_I2C_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_result.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Read a number of bytes from a specific address of the I2C device */
CsrResult I2CRead(CsrUint8 channel, CsrUint8 address, CsrUint32 count, CsrUint8 *data);

/* Write a number of bytes to a specific address of the I2C device */
CsrResult I2CWrite(CsrUint8 channel, CsrUint8 address, CsrUint32 count, CsrUint8 *data);

#ifdef __cplusplus
}
#endif

#endif

/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_type_a_private.h"

CsrBool CsrSdioAProtSetChipQuirks(CsrSdioAInstanceData *instData)
{
    CsrUint16 vendorId, deviceId;

    vendorId = instData->sdioHandle->sdioId.manfId;
    deviceId = instData->sdioHandle->sdioId.cardId;

    /*
     * BlueCore6 quirks:
     *
     *  - read and write retries produce bad data
     *  - read acks are required.
     *  - must poll bit0 of TO_HOST_SCRATCH0 after enabling function.
     * XXX: BC7 hardcoded to the same.  Use CIS.
     */
    if (0x032a == vendorId)
    {
        instData->readNeedAck = 1;

        if (0x4 == deviceId)
        {
            instData->maxRxTries = 1;
            instData->maxTxTries = 1;
        }
        else
        {
            instData->maxRxTries = 10;
            instData->maxTxTries = 10;
        }
        return TRUE;
    }
    return FALSE;
}

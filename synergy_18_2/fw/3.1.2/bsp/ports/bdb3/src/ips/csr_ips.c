/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ips.h"
#include "nucleus.h"

void CsrIpsConfigure(CsrUint32 currentInterval, CsrUint32 maximumInterval, CsrUint32 intervalLength)
{
    NU_Ips_Configure((UINT32) currentInterval, (UINT32) maximumInterval, (UINT32) intervalLength);
}

CsrUint32 *CsrIpsInsert(CsrIpsContext context, CsrUint32 *measurements)
{
    return (CsrUint32 *) NU_Ips_Insert((NU_IPS_CONTEXT) context, (UINT32 *) measurements);
}

CsrUint32 *CsrIpsMeasurementsGet(CsrIpsContext context)
{
    return (CsrUint32 *) NU_Ips_Measurements_Get((NU_IPS_CONTEXT) context);
}

void CsrIpsEnable(void)
{
    NU_Ips_Enable();
}

void CsrIpsDisable(void)
{
    NU_Ips_Disable();
}

CsrUint32 CsrIpsResolutionGet(void)
{
    return (CsrUint32) NU_Ips_Resolution_Get();
}

CsrUint32 CsrIpsCurrentIntervalGet(void)
{
    return (CsrUint32) NU_Ips_Current_Interval_Get();
}

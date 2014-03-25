/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_internet_checksum.h"

CsrUint16 CalculateCheckSum(CsrUint16 length, CsrUint8 * data)
{
    CsrUint16 index;
    CsrUint32 calculatedCheckSum;

    calculatedCheckSum = 0L;
    for (index = 0; index < length; index += 2)
    {
        calculatedCheckSum += ((((CsrUint16) data[index]) << 8) | ((CsrUint16) data[index + 1]));
    }
    calculatedCheckSum = ((CsrUint16) calculatedCheckSum) + ((CsrUint16) (calculatedCheckSum >> 16));
    return ((CsrUint16) ((~(((CsrUint16) calculatedCheckSum) + ((CsrUint16) (calculatedCheckSum >> 16)))) & 0x0000FFFFL));
}

CsrUint32 CalculateCheckSumFirstFragment(CsrUint16 length, CsrUint8 * data)
{
    CsrUint16 index;
    CsrUint32 preliminaryCheckSum;

    preliminaryCheckSum = 0L;
    for (index = 0; index < length; index += 2)
    {
        preliminaryCheckSum += ((((CsrUint16) data[index]) << 8) | ((CsrUint16) data[index + 1]));
    }
    return preliminaryCheckSum;
}

CsrUint32 CalculateCheckSumFragment(CsrUint32 preliminaryCheckSum, CsrUint16 length, CsrUint8 * data)
{
    CsrUint16 index;

    for (index = 0; index < length; index += 2)
    {
        preliminaryCheckSum += ((((CsrUint16) data[index]) << 8) | ((CsrUint16) data[index + 1]));
    }
    return preliminaryCheckSum;
}

CsrUint16 CalculateCheckSumLastFragment(CsrUint32 preliminaryCheckSum, CsrUint16 length, CsrUint8 * data)
{
    CsrUint16 index;

    for (index = 0; index < length; index += 2)
    {
        preliminaryCheckSum += ((((CsrUint16) data[index]) << 8) | ((CsrUint16) data[index + 1]));
    }
    preliminaryCheckSum = ((CsrUint16) preliminaryCheckSum) + ((CsrUint16) (preliminaryCheckSum >> 16));
    return ((CsrUint16) ((~(((CsrUint16) preliminaryCheckSum) + ((CsrUint16) (preliminaryCheckSum >> 16)))) & 0x0000FFFFL));
}

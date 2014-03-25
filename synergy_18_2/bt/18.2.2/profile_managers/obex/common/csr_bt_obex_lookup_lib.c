/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "csr_bt_obex.h"
#include "csr_bt_util.h"
#include "csr_pmem.h"
#include "csr_bt_obex_common.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_private_lib.h"

/* Common scanner */
static CsrUint16 obexScanIndex(CsrBtObexSession  session,
                              CsrUint8 *data, CsrUint16 *dataLength,
                              CsrUint8 headerType, CsrUint8 altHeaderType, CsrBool useAlt)
{
    CsrUint16 index=3;
    CsrUint16 length;
    CsrUint16 oldIndex;
    CsrUint16 nextIndex;

    length = (CsrUint16)(CSR_BT_OBEX_GET_REQUEST_LENGTH(data));
    *dataLength = 0;

    /* Response headers can't be autodetected */
    if(session == CSR_BT_OBEX_RESPONSE_CONNECT)
    {
        index = 7;
    }
    else if(session == CSR_BT_OBEX_RESPONSE_NORMAL)
    {
    }
    else
    {
        /* Request headers can... */
        if (data[0] == CSR_BT_OBEX_CONNECT_OPCODE)
        {
            /* Session CONNECT REQUEST header length is 7 */
            index = 7;
        }
        else if(data[0] == CSR_BT_OBEX_SET_PATH_OPCODE)
        {
            /* Session SETPATH REQUEST is length 5 */
            index = 5;
        }
        else
        {
            /* All other session headers are length 3 */
        }
    }

    /* Standard search */
    while (index < length)
    {
        switch (data[index] & CSR_BT_OBEX_HI_MASK)
        {
            case CSR_BT_OBEX_HI_UNICODE:
            case CSR_BT_OBEX_HI_BYTESEQ:
            { /* Byte sequence or Unicode has a 2-byte length prefix */
                *dataLength = (CsrUint16)CSR_BT_OBEX_GET_WORD_AT(data, index + 1);
                nextIndex   = (CsrUint16)(index + (*dataLength));
                break;
            }
            case CSR_BT_OBEX_HI_ONEBYTE:
            { /* 1 byte quantity */
                nextIndex = (CsrUint16)(index + 2);
                *dataLength = 1;
                break;
            }
            case CSR_BT_OBEX_HI_FOURBYTE:
            { /* 4 byte quantity - transmitted in network byte order (high byte first) */
                nextIndex = (CsrUint16)(index + 5);
                *dataLength = 4;
                break;
            }
            default:
            { /* Dead in the water, break out */
                *dataLength = 0;
                nextIndex   = index;
                break;
            }
        }

        if((data[index] == headerType) || (useAlt && (data[index] == altHeaderType)))
        { /* Got it, bail out */
            return index;
        }
        else
        {
            *dataLength = 0;
        }

        oldIndex = index;
        index    = nextIndex;

        /* Catch index deadlock */
        if(oldIndex == index)
        {
            break;
        }
    }
    return 0;
}

CsrUint16 CsrBtObexHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint8 headerType, CsrUint16 *headerLength)
{
    return obexScanIndex(session, data, headerLength, headerType, 0, FALSE);
}

CsrUint16 CsrBtObexBodyHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *bodyLength)
{
    return obexScanIndex(session, data, bodyLength, CSR_BT_OBEX_BODY_HEADER, CSR_BT_OBEX_END_OF_BODY_HEADER, TRUE);
}

CsrUint16 CsrBtObexNameHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *nameLength)
{
    return obexScanIndex(session, data, nameLength, CSR_BT_OBEX_NAME_HEADER, 0, FALSE);
}

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
CsrUint16 CsrBtObexDescriptionHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *descriptionLength)
{
    return obexScanIndex(session, data, descriptionLength, CSR_BT_OBEX_DESCRIPTION_HEADER, 0, FALSE);
}
#endif

CsrUint16 CsrBtObexObjectLengthHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint32 *lengthOfObjectValue)
{
    CsrUint16 index;
    CsrUint16 length;

    *lengthOfObjectValue = 0;

    index = obexScanIndex(session, data, &length, CSR_BT_OBEX_LENGTH_HEADER, 0, FALSE);

    if(index > 0)
    {
        *lengthOfObjectValue = CSR_BT_OBEX_GET_D_WORD_AT(data, index + 1);
    }
    return index;
}

CsrUint16 CsrBtObexCountHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint32 *countValue)
{
    CsrUint16 index;
    CsrUint16 length;

    *countValue = 0;

    index = obexScanIndex(session, data, &length, CSR_BT_OBEX_COUNT_HEADER, 0, FALSE);

    if(index > 0)
    {
        *countValue = CSR_BT_OBEX_GET_D_WORD_AT(data, index + 1);
    }
    return index;
}

#ifdef CSR_BT_INSTALL_OBEX_UTIL_IMG_OFFSET
CsrUint16 CsrBtObexImgHandleHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *imgHandleLength)
{
    return obexScanIndex(session, data, imgHandleLength, CSR_BT_OBEX_IMG_HANDLE_HEADER, 0, FALSE);
}

CsrUint16 CsrBtObexImgDescriptorHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *imgDescriptorLength)
{
    return obexScanIndex(session, data, imgDescriptorLength, CSR_BT_OBEX_IMG_DESCRIPTOR_HEADER, 0, FALSE);
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID
CsrUint16 CsrBtObexTargetHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *targetLength)
{
    return obexScanIndex(session, data, targetLength, CSR_BT_OBEX_TARGET_HEADER, 0, FALSE);
}
#endif

CsrUint16 CsrBtObexTypeHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint16 *typeLength)
{
    return obexScanIndex(session, data, typeLength, CSR_BT_OBEX_TYPE_HEADER, 0, FALSE);
}

#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
CsrUint16 CsrBtObexAppParamIndex(CsrUint8 *data)
{
    CsrUint16 typeLength;
    return obexScanIndex(CSR_BT_OBEX_REQUEST, data, &typeLength, CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER, 0, FALSE);
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID
CsrUint16 CsrBtObexConnectionIdHeaderIndex(CsrBtObexSession session, CsrUint8 *data, CsrUint32 *connectionIdValue)
{
    CsrUint16 index;
    CsrUint16 length;

    *connectionIdValue = 0;
    index = obexScanIndex(session, data, &length, CSR_BT_OBEX_CONNECTION_ID_HEADER, 0, FALSE);

    if(index > 0)
    {
        *connectionIdValue = CSR_BT_OBEX_GET_D_WORD_AT(data, index + 1);
    }

    return index;
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
CsrBool CsrBtObexGetSingleResponseModeParameter(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint8 *srmValue)
{
    CsrUint16   length;
    CsrUint16   index  = obexScanIndex(session, obexPacket, &length, CSR_BT_OBEX_SINGLE_RESPONSE_MODE_HEADER, 0, FALSE);

    if(index)
    {
        *srmValue   = obexPacket[index  + 1];
        return (TRUE);
    }
    return (FALSE);
}

CsrBool CsrBtObexGetSrmpParameter(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint8 *srmpValue)
{
    CsrUint16   length;
    CsrUint16   index  = obexScanIndex(session, obexPacket, &length, CSR_BT_OBEX_SRMP_HEADER, 0, FALSE);

    if(index)
    {
        *srmpValue   = obexPacket[index  + 1];
        return (TRUE);
    }
    return (FALSE);
}

CsrBool CsrBtObexGetPermissionsParameter(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint32 *permissionsValue)
{
    CsrUint16   length;
    CsrUint16   index  = obexScanIndex(session, obexPacket, &length, CSR_BT_OBEX_PERMISSIONS_HEADER, 0, FALSE);

    if(index)
    {
        *permissionsValue = CSR_BT_OBEX_GET_D_WORD_AT(obexPacket, index + 1);
        return (TRUE);
    }
    return (FALSE);
}

CsrBool CsrBtObexGetActionIdParameter(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint8 *actionIdValue)
{
    CsrUint16   length;
    CsrUint16   index  = obexScanIndex(session, obexPacket, &length, CSR_BT_OBEX_ACTION_ID_HEADER, 0, FALSE);

    if(index)
    {
        *actionIdValue = obexPacket[index  + 1];
        return (TRUE);
    }
    return (FALSE);
}
#endif


/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_bt_obex_common.h"
#include "csr_bt_obex_lookup_lib.h"
#include "csr_bt_profiles.h"
#include "csr_bt_util.h"
#include "csr_pmem.h"

#include "csr_unicode.h"


/* Returns the maximum Obex packet length from the OBEX connect operation */
CsrUint16 CsrBtObexGetMaxPacketLength(CsrUint8 *obexPacket)
{
    return (CsrUint16)(CSR_BT_OBEX_GET_WORD_AT(obexPacket, 5));
}


#ifdef CSR_BT_INSTALL_OBEX_UTIL_CREATE_NAME
/* Look for a name header (0x01) and create a buffer for it if it exists */
CsrUint8 *CsrBtObexCreateName(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint16 *nameLength)
{
    CsrUint8 *name;
    CsrUint16 nameIndex;

    *nameLength = 0;
    nameIndex = CsrBtObexNameHeaderIndex(session, obexPacket, nameLength);

    if (nameIndex && (*nameLength > CSR_BT_OBEX_HEADER_LENGTH))
    {
        /* Adjust name length, create buffer and zero-terminate it */
        *nameLength -= CSR_BT_OBEX_HEADER_LENGTH;
        name = CsrPmemAlloc(*nameLength);
        CsrMemCpy(name, &obexPacket[nameIndex + CSR_BT_OBEX_HEADER_LENGTH], *nameLength);
        name[*nameLength - 1] = '\0';
    }
    else
    {
        name = NULL;
        *nameLength = 0;
    }
    return name;
}
#endif
static CsrBool csrBtObexGetNullTerminatedUnicodeTextString(CsrBtObexSession session,
                                                           CsrUint8         *obexPacket,
                                                           CsrUint16        *offset,
                                                           CsrUint8         headerType)
{
    CsrUint16 index;
    CsrUint16 length;
    *offset   = 0;

    index = CsrBtObexHeaderIndex(session, obexPacket, headerType, &length);

    /* Index ok, and offset is beyond the header */
    if (index && (length > CSR_BT_OBEX_HEADER_LENGTH))
    {
        *offset = (CsrUint16)(index + CSR_BT_OBEX_HEADER_LENGTH);
        /* Make sure string is zero-terminated */
        length -= CSR_BT_OBEX_HEADER_LENGTH;
        if(obexPacket[*offset + length - 1] != '\0')
        {
            obexPacket[*offset + length - 1] = '\0';
        }
        return (TRUE);
    }
    return (FALSE);
}

/* Return TRUE if a Name header (0x01) is present, and set the offset of the
 * name header in *offset.
 * Note: Name is always zero-terminated. */
CsrBool CsrBtObexGetNameOffset(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint16 *offset)
{
    if (csrBtObexGetNullTerminatedUnicodeTextString(session, obexPacket, offset, CSR_BT_OBEX_NAME_HEADER))
    {
        return (TRUE);
    }
    return (FALSE);
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
/* Return TRUE if a destName header (0x15) is present, and set the offset of the
 * destName header in *offset.
 * Note: destName is always zero-terminated. */
CsrBool CsrBtObexGetDestNameOffset(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint16 *offset)
{
    if (csrBtObexGetNullTerminatedUnicodeTextString(session, obexPacket, offset, CSR_BT_OBEX_DESTNAME_HEADER))
    {
        return (TRUE);
    }
    return (FALSE);
}
#endif

/* Return TRUE if a Body (0x48) or an End if Body header (0x49) is present.
 * Also, the header type is returned in the bodyType parameter, the body
 * payload offset is returned in the bodyOffset parameter and finally the length of the
 * body payload is returned in the bodyLength parameter */
#ifdef CSR_BT_INSTALL_OBEX_UTIL_BODY_OFFSET 
CsrBool CsrBtObexGetBodyOffset(CsrBtObexSession session, CsrUint8 *obexPacket,
                         CsrUint8 *bodyType, CsrUint16 *bodyOffset, CsrUint16 *bodyLength)
{
    CsrUint16 length;
    CsrUint16 index;
    CsrBool found;

    *bodyOffset  = 0;
    *bodyLength  = 0;
    *bodyType    = 0;

    found = FALSE;
    index = CsrBtObexBodyHeaderIndex(session, obexPacket, &length);
    if(index)
    {
        *bodyType = obexPacket[index];
        if (length > CSR_BT_OBEX_HEADER_LENGTH)
        {
            *bodyOffset  = (CsrUint16)(index + CSR_BT_OBEX_HEADER_LENGTH);
            *bodyLength  = (CsrUint16)(length - CSR_BT_OBEX_HEADER_LENGTH);
        }
        found = TRUE;
    }

    return found;
}
#endif

/* Return result code for fetching the body header. If the body header is found, set the
 * *bodyOffset and *bodyLength parameters, and fill the *isFinal if packet is last in
 * a sequence */
CsrUint8 CsrBtObexGetBodyHeaderOffset(CsrBtObexSession session, CsrUint8 *obexPacket,
                                CsrUint16 *headerOffset, CsrUint16 *headerLength, CsrUint8 *isFinal)
{
    CsrUint16 index;

    *headerOffset = 0;
    index = CsrBtObexBodyHeaderIndex(session, obexPacket, headerLength);
    *isFinal = FALSE;

    if(index)
    {
        *headerLength -= CSR_BT_OBEX_HEADER_LENGTH;

        if (*headerLength <= CSR_BT_MAX_OBEX_SIGNAL_LENGTH)
        {
            *headerOffset = (CsrUint16)(index + CSR_BT_OBEX_HEADER_LENGTH);

            if (obexPacket[index] == CSR_BT_OBEX_END_OF_BODY_HEADER)
            {
                *isFinal = TRUE;
            }
            if (obexPacket[index] == CSR_BT_OBEX_BODY_HEADER)
            {
                *isFinal = FALSE;
            }
        }
        else
        {
            return CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE | CSR_BT_OBEX_FINAL_BIT_MASK;
        }
    }
    else
    {
        *headerLength = 0;
        if (CSR_BT_OBEX_GET_REQUEST_OPCODE(obexPacket) == (CSR_BT_OBEX_PUT_OPCODE | CSR_BT_OBEX_FINAL_BIT_MASK))
        {
            *isFinal = TRUE;
        }
    }
    return CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
}


/* Return TRUE if image handle is found and decode the handle by
 * copying it into the imgHandle parameter */
#ifdef CSR_BT_INSTALL_OBEX_UTIL_COPY_IMAGE 
CsrBool CsrBtObexCopyDecodedImgHandle(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint8 *imgHandle)
{
    CsrUint16 length;
    CsrBool   returnValue = FALSE;   
    CsrUint16 index       = CsrBtObexHeaderIndex(session, obexPacket, CSR_BT_OBEX_IMG_HANDLE_HEADER, &length);

    if (imgHandle && index && length > CSR_BT_OBEX_HEADER_LENGTH)
    {
        CsrUint8 *tempString = CsrUcs2ByteString2Utf8((CsrUcs2String *)&obexPacket[index + CSR_BT_OBEX_HEADER_LENGTH]);
        
        if (tempString && CsrStrLen((CsrCharString *)tempString) == CSR_BT_IMG_HANDLE_LENGTH)
        {
            CsrStrCpy((char *)imgHandle, (char *)tempString);
            returnValue = TRUE;
        }
        CsrPmemFree(tempString);
    }
    return returnValue;
}
#endif

/* Return TRUE if a ConnectionId header (0xCB) is present, and it value is
 * return in the connectionId parameter */
#ifdef CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID
CsrBool CsrBtObexGetConnectionId(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint32 *connectionId)
{
    CsrUint16 index;
    index = CsrBtObexConnectionIdHeaderIndex(session, obexPacket, connectionId);
    return (CsrBool)index;
}
#endif

/* Return TRUE if the required Request Digest string is present. The
 * userId (CSRMAX length is 20 bytes) is only required if the
 * digest-challenge contains an options field with the userid bit
 * set to 1. The nonce is optional. */
#if defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE) || defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE)
CsrBool CsrBtObexCopyAuthResponse(CsrBtObexSession session,
                                  CsrUint8         *obexPacket,
                                  CsrUint8         *requestDigest,
                                  CsrUint8         *userId,
                                  CsrUint8         *userIdLength)
{
    CsrUint16    length;
    CsrUint16    index;

    *userIdLength = 0;
    index         = CsrBtObexHeaderIndex(session, obexPacket, CSR_BT_OBEX_AUTHENTICATE_RESPONSE_HEADER, &length);

    if (index)
    {
        CsrBool      passValue    = FALSE;
        CsrUint16    count        = CSR_BT_OBEX_HEADER_LENGTH;

        while (count < length)
        {
            switch (obexPacket[(index + count)])
            {
                case CSR_BT_DIGEST_REQUEST_TAG:
                    {
                        if (obexPacket[(index + count + 1)] == CSR_BT_DIGEST_REQUEST_LENGTH)
                        {
                            CsrMemCpy(requestDigest, &obexPacket[(index + count + CSR_BT_AUTH_TAG_LENGTH_SCHEME)], CSR_BT_DIGEST_REQUEST_LENGTH);
                            count += CSR_BT_DIGEST_REQUEST_LENGTH + CSR_BT_AUTH_TAG_LENGTH_SCHEME;
                            passValue = TRUE;
                        }
                        else
                        {
                            return FALSE;
                        }
                        break;
                    }
                case CSR_BT_DIGEST_NONCE_TAG :
                    {
                        if (obexPacket[(index + count + 1)] == CSR_BT_DIGEST_NONCE_LENGTH)
                        {
                            count += CSR_BT_DIGEST_NONCE_LENGTH + CSR_BT_AUTH_TAG_LENGTH_SCHEME;
                        }
                        else
                        {
                            return FALSE;
                        }
                        break;
                    }
                case CSR_BT_DIGEST_USERID_TAG :
                    {
                        CsrUint8    idLength = obexPacket[(index + count + 1)];

                        if (idLength <=  CSR_BT_OBEX_MAX_AUTH_USERID_LENGTH)
                        {
                            *userIdLength    = idLength;
                            CsrMemCpy(userId, &obexPacket[(index + count + CSR_BT_AUTH_TAG_LENGTH_SCHEME)], idLength);
                            count = (CsrUint16)(count + idLength + CSR_BT_AUTH_TAG_LENGTH_SCHEME);
                        }
                        else
                        {
                            return FALSE;
                        }
                        break;
                    }
                default :
                    {
                        return FALSE;
                    }
            }
        }
        return passValue;
    }
    return FALSE;
}
#endif
/* Return TRUE if the required Nonce string is present. The options are present
 * if the optionsPresent parameter is set to TRUE. If a realm is present, the length
 * is return in the realmLength parameter and the realm string is returned in
 * the realm parameter.
 * Note: We copy things into the *nonce parameter.
 * Note: We allocate things for the realm parameter.
 * Note: options/optionsPresent are set directly (one-byte values).
 */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
CsrBool CsrBtObexCreateAuthChallenge(CsrBtObexSession session, CsrInt8 *obexPacket, CsrUint8 *nonce, CsrUint8 *options,
                               CsrBool *optionsPresent, CsrUint8 **realm, CsrUint16  *realmLength)
{
    CsrUint16    index;
    CsrUint16    length;

    *realmLength    = 0;
    *optionsPresent = FALSE;

    index    = CsrBtObexHeaderIndex(session, (CsrUint8*)obexPacket, CSR_BT_OBEX_AUTHENTICATE_CHALLENGE_HEADER, &length);

    if (index)
    {
        CsrBool   passValue = FALSE;
        CsrUint16 count     = CSR_BT_OBEX_HEADER_LENGTH;

        while (count < length)
        {
            switch (obexPacket[index + count])
            {
                case CSR_BT_NONCE_TAG :
                    {
                        if (obexPacket[index + count + 1] == CSR_BT_NONCE_LENGTH)
                        {
                            CsrMemCpy(nonce, &obexPacket[index + count + CSR_BT_AUTH_TAG_LENGTH_SCHEME], CSR_BT_NONCE_LENGTH);
                            count += CSR_BT_NONCE_LENGTH + CSR_BT_AUTH_TAG_LENGTH_SCHEME;
                            passValue = TRUE;
                        }
                        else
                        {
                            CsrPmemFree(*realm);
                            *realm = NULL;
                            return FALSE;
                        }
                        break;
                    }
                case CSR_BT_OPTIONS_TAG :
                    {
                        if (obexPacket[index + count + 1] == CSR_BT_OPTIONS_LENGTH)
                        {
                            *optionsPresent  = TRUE;
                            *options         = obexPacket[index + count + CSR_BT_AUTH_TAG_LENGTH_SCHEME];
                            count            += CSR_BT_OPTIONS_LENGTH + CSR_BT_AUTH_TAG_LENGTH_SCHEME;
                        }
                        else
                        {
                            CsrPmemFree(*realm);
                            *realm = NULL;
                            return FALSE;
                        }
                        break;
                    }
                case CSR_BT_REALM_TAG :
                    {
                        CsrUint16 rLength  = obexPacket[index + count + 1];
                        *realm            = (CsrUint8 *) CsrPmemAlloc(rLength);
                        *realmLength      = rLength;
                        CsrMemCpy(*realm, &obexPacket[index + count + CSR_BT_AUTH_TAG_LENGTH_SCHEME], rLength);
                        count = (CsrUint16)(count + rLength + CSR_BT_AUTH_TAG_LENGTH_SCHEME);
                        break;
                    }
                default :
                    {
                        CsrPmemFree(*realm);
                        *realm = NULL;
                        return FALSE;
                    }
            }
        }
        if(!passValue)
        {
            CsrPmemFree(*realm);
        }
        return passValue;
    }
    return FALSE;
}
#endif
/* Return TRUE if a Type header (0x42) is present, and set the offset of the
 * type header in *offset. Note: Type is always zero-terminated.                    */
CsrBool CsrBtObexGetTypeOffset(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint16 *offset, CsrUint16 *typeLength)
{
    CsrUint16    index;
    CsrUint16    length;
    *offset = 0;
    index   = CsrBtObexTypeHeaderIndex(session, obexPacket, &length);

    if (index && length > CSR_BT_OBEX_HEADER_LENGTH)
    {
        *typeLength = (CsrUint16)(length - CSR_BT_OBEX_HEADER_LENGTH);
        *offset     = (CsrUint16)(index + CSR_BT_OBEX_HEADER_LENGTH);
        return (TRUE);
    }
    return (FALSE);
}

/* Return TRUE if a App parameters header (0x4C) is present, and set the offset of
   the App parameters header in *offset.                                            */
CsrBool CsrBtObexGetAppParametersOffset(CsrBtObexSession session, CsrUint8 *obexPacket,
                                        CsrUint16 obexPacketLength, CsrUint16 *offset,
                                        CsrUint16 *length)
{
    CsrUint16 index;
    CsrUint16 tempLength;

    *offset = 0;
    *length = 0;
    index   = CsrBtObexHeaderIndex(session, obexPacket, CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER, &tempLength);

    /* Index ok, and offset is beyond the header */
    if (index && (tempLength > CSR_BT_OBEX_HEADER_LENGTH) && (index + tempLength <= obexPacketLength))
    {
        *offset  = (CsrUint16)(index + CSR_BT_OBEX_HEADER_LENGTH);
        *length  = (CsrUint16)(tempLength - CSR_BT_OBEX_HEADER_LENGTH);
        return (TRUE);
    }
    return (FALSE);
}

/* Return TRUE if a Img Descriptor header (0x71) is present, and set the offset of
 * the in *offset.                                                                  */
#ifdef CSR_BT_INSTALL_OBEX_UTIL_IMG_OFFSET
CsrBool CsrBtObexGetImgDescriptorOffset(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint16 *offset, CsrUint16 *imgDescriptorLength)
{
    CsrUint16    index;
    CsrUint16    length;
    *offset                 = 0;
    *imgDescriptorLength    = 0;
    index =  CsrBtObexImgDescriptorHeaderIndex(session, obexPacket, &length);

    if (index && length >= CSR_BT_OBEX_HEADER_LENGTH)
    {
        *offset                 = (CsrUint16)(index + CSR_BT_OBEX_HEADER_LENGTH);
        *imgDescriptorLength    = (CsrUint16)(length - CSR_BT_OBEX_HEADER_LENGTH);
        return (TRUE);
    }
    return (FALSE);
}

/* Return TRUE if a Img Handle header (0x30) is present, and set the offset of
 * the in *offset.                                                                  */
CsrBool CsrBtObexGetImgHandleOffset(CsrBtObexSession session, CsrUint8 *obexPacket, CsrUint16 *offset)
{
    CsrUint16    index;
    CsrUint16    length;

    *offset     = 0;
    index       =  CsrBtObexImgHandleHeaderIndex(session, obexPacket, &length);

    if (index && length > CSR_BT_OBEX_HEADER_LENGTH)
    {
        *offset = (CsrUint16)(index + CSR_BT_OBEX_HEADER_LENGTH);
        return (TRUE);
    }
    return (FALSE);
}
#endif


/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_RFC_MODULE

#include "csr_bt_obex_util.h"
#include "csr_bt_obex_private_util.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_cmn_sdp_lib.h"
#include "bkeyval.h"
#include "l2cap_prim.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */


static void obexUtilTransportDataIndFirstFragment(ObexUtilPrivateInstType *priInst,
                                                  CsrUint16               payloadLength,
                                                  CsrUint8                *payload);

/*------------------------------------------------------------------------------*
 * Static Obex utilfunction to build and send the transport packets             *
 *------------------------------------------------------------------------------*/
static CsrUint8 * obexUtilBuildOneByteOpcode(CsrUint8     headerType,
                                             CsrUint8   *transportBuffer)
{ /* The function adds a one byte opcode */
    *transportBuffer = headerType;
    transportBuffer++;
    return (transportBuffer);
}

static CsrUint8 * obexUtilBuildFirstByteLength(CsrUint16   headerLength,
                                               CsrUint8  *transportBuffer)
{ /* The function adds the first byte of a two byte length tag */
    *transportBuffer = (CsrUint8) (headerLength >> 8);
    transportBuffer++;
    return (transportBuffer);
}

static CsrUint8 * obexUtilBuildSecondByteLength(CsrUint16   headerLength,
                                                CsrUint8  *transportBuffer)
{ /* The function add the second byte of a two byte length tag */
    *transportBuffer = (CsrUint8) (headerLength);
    transportBuffer++;
    return (transportBuffer);
}

static CsrUint8 * obexUtilBuildCommonOpcodeHeaderHandler(CsrUint8    headerType,
                                                         CsrUint16   totalHeaderLength,
                                                         CsrUint8  *transportBuffer)
{ /* The function add the common header (One byte opcode + two byte length */
    transportBuffer = obexUtilBuildOneByteOpcode(headerType, transportBuffer);
    transportBuffer = obexUtilBuildFirstByteLength(totalHeaderLength, transportBuffer);
    transportBuffer = obexUtilBuildSecondByteLength(totalHeaderLength, transportBuffer);
    return (transportBuffer);
}

#if defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE) || defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE)
static CsrUint8 * obexUtilBuildTagLengthHandler(CsrUint8    tag,
                                                CsrUint8    tagLength,
                                                CsrUint8 * transportBuffer)
{ /* This function add one byte tag and one byte length. This Tag-Length
     encoding scheme is used in a OBEX Authenticate Challenge and Response
     header */
    *transportBuffer = tag;
    transportBuffer++;
    *transportBuffer = tagLength;
    transportBuffer++;
    return (transportBuffer);
}
#endif

static CsrUint8 * obexUtilBuildMandatoryConnectHeader(CsrUint8  *buffer,
                                                      CsrUint16   maxObexPacketLength)
{
    *buffer        = CSR_BT_OBEX_VERSION_1_0;
    buffer++;
    *buffer        = CSR_BT_OBEX_CONNECTION_FLAGS;
    buffer++;
    *buffer        = (CsrUint8) (maxObexPacketLength >> 8);
    buffer++;
    *buffer        = (CsrUint8) (maxObexPacketLength);
    buffer++;
    return buffer;
}

#ifdef CSR_BT_INSTALL_OBEX_CLI_SET_PATH
static CsrUint8 * obexUtilBuildMandatorySetPathHeader(CsrUint8  *buffer,
                                                      CsrUint8    flags,
                                                      CsrUint8    constants)
{
    *buffer        = flags;
    buffer++;
    *buffer        = constants;
    buffer++;
    return buffer;
}
#endif

static CsrUint8 * obexUtilBuildCsrUint32Header(CsrUint8 *buffer,
                                               CsrUint8   headerId,
                                               CsrUint32  headerValue)
{
    *buffer        = headerId;
    buffer++;
    *buffer        = (CsrUint8) (headerValue     >> 24);
    buffer++;
    *buffer        = (CsrUint8) (headerValue     >> 16);
    buffer++;
    *buffer        = (CsrUint8) (headerValue     >> 8);
    buffer++;
    *buffer        = (CsrUint8) (headerValue);
    buffer++;
    return buffer;
}

static CsrUint16 obexUtilReturnTypeHeaderLength(CsrUint8 * type)
{
    CsrUint16 length;

    if(type != NULL)
    {
        length = (CsrUint16)CsrStrLen((char *)type);
    }
    else
    {
        length = 0;
    }

    if (length > 0)
    {
        length++;
        length += CSR_BT_OBEX_HEADER_LENGTH;
    }
    else
    {
        length = CSR_BT_OBEX_HEADER_LENGTH;
    }
    return length;
}

  /* This function return the total length of the body/end body header, e.g.
     the body header (one byte opcode + two byte length) plus body header
     'payload' */
#define obexUtilBodyLengthHandler(priInst) (CsrUint16)((priInst)->bodyLength + CSR_BT_OBEX_HEADER_LENGTH)


#if defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE) || defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE)
CsrUint8 * obexUtilReturnAuthenticatePointer(CsrUint8 headerType,
                                             CsrUint8 tag1, CsrUint8 tag1Length, CsrUint8 * tag1Payload,
                                             CsrUint8 tag2, CsrUint8 tag2Length, CsrUint8 * tag2Payload,
                                             CsrUint8 tag3, CsrUint8 tag3Length, CsrUint8 * tag3Payload,
                                             CsrUint16  *headerLength)
{
    /* This function will either build the OBEX Digest Challenge or
     * Response header */
    CsrUint8  *header;
    CsrUint8  *tempHeader;
    CsrUint16 length         = CSR_BT_OBEX_HEADER_LENGTH;

    if (tag1Payload && tag1Length > 0)
    { /* tag1 must be included in the header */
        length += (tag1Length + CSR_BT_AUTH_TAG_LENGTH_SCHEME);
    }

    if (tag2Payload && tag2Length > 0)
    { /* tag2 must be included in the header */
        length += (tag2Length + CSR_BT_AUTH_TAG_LENGTH_SCHEME);
    }

    if (tag3Payload && tag3Length > 0)
    { /* tag3 must be included in the header */
        length += (tag3Length + CSR_BT_AUTH_TAG_LENGTH_SCHEME);
    }

    header      = (CsrUint8 *) CsrPmemAlloc(length);
    tempHeader  = header;
    tempHeader  = obexUtilBuildCommonOpcodeHeaderHandler(headerType, length, tempHeader);

    if (tag1Payload && tag1Length > 0)
    { /* Add tag1 to the header */
        tempHeader = obexUtilBuildTagLengthHandler(tag1, tag1Length, tempHeader);
        CsrMemCpy(tempHeader, tag1Payload, tag1Length);
        tempHeader += (tag1Length);
    }

    if (tag2Payload && tag2Length > 0)
    { /* Add tag2 to the header */
        tempHeader = obexUtilBuildTagLengthHandler(tag2, tag2Length, tempHeader);
        CsrMemCpy(tempHeader, tag2Payload, tag2Length);
        tempHeader += (tag2Length);
    }

    if (tag3Payload && tag3Length > 0)
    { /* Add tag3 to the header */
        tempHeader = obexUtilBuildTagLengthHandler(tag3, tag3Length, tempHeader);
        CsrMemCpy(tempHeader, tag3Payload, tag3Length);
    }
    *headerLength = length;
    return (header);
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
static void obexUtilBuildObexChallengePointer(ObexUtilPrivateInstType     * priInst,
                                              ObexUtilDigestChallengeType * digestChallenge)
{
    if (digestChallenge)
    {
        CsrBtGetNonce(digestChallenge->nonce);
        priInst->digestChallenge = digestChallenge;

        priInst->digestChallengeHeader = obexUtilReturnAuthenticatePointer(CSR_BT_OBEX_AUTHENTICATE_CHALLENGE_HEADER,
                                                                           CSR_BT_NONCE_TAG, CSR_BT_NONCE_LENGTH, digestChallenge->nonce, CSR_BT_OPTIONS_TAG,
                                                                           CSR_BT_OPTIONS_LENGTH, &(digestChallenge->options), CSR_BT_REALM_TAG,
                                                                           (CsrUint8)digestChallenge->realmLength, digestChallenge->realm,
                                                                           &priInst->digestChallengeLength);
    }
    else
    {
        priInst->digestChallenge       = NULL;
        priInst->digestChallengeHeader = NULL;
        priInst->digestChallengeLength = 0;
    }
}
#endif

static CsrUint8 * obexUtilBuildCommonObexRequestHeader(ObexUtilPrivateInstType *priInst,
                                                       CsrUint8                headerType,
                                                       CsrUint16               totalHeaderLength,
                                                       CsrUint8              *transportBuffer)
{ /* This function will add as must of the commen OBEX header (one byte opcode +
     two byte length) as possible into the transport packet. */
    CsrUint16 bytesAvailable = (CsrUint16)(priInst->maxTransportPacketLength - priInst->transportPacketIndex);

    if (bytesAvailable > 0)
    { /* There is at least one byte free in the transport packet */
        CsrUint16 bytesTaken;

        switch (priInst->headerPayloadIndex)
        {
            case 0:
                { /* No part of the common header has been send */
                    switch (bytesAvailable)
                    {
                        case 1:
                            { /* Only the opcode be added to the transport packet */
                                transportBuffer = obexUtilBuildOneByteOpcode(headerType, transportBuffer);
                                bytesTaken      = 1;
                                break;
                            }
                        case 2:
                            { /* Only the opcode and the first byte of the length can
                                 be added to the transport packet */
                                transportBuffer = obexUtilBuildOneByteOpcode(headerType, transportBuffer);
                                transportBuffer = obexUtilBuildFirstByteLength(totalHeaderLength, transportBuffer);
                                bytesTaken      = 2;
                                break;
                            }
                        default:
                            { /* All part of the common OBEX header can be
                                 added to the transport packet */
                                transportBuffer = obexUtilBuildCommonOpcodeHeaderHandler(headerType,
                                                                                         totalHeaderLength,
                                                                                         transportBuffer);
                                bytesTaken      = 3;
                                break;
                            }
                    }
                    break;
                }
            case 1:
                { /* Only the Type byte has been send */
                    transportBuffer = obexUtilBuildFirstByteLength(totalHeaderLength, transportBuffer);
                    bytesTaken      = 1;
                    if (bytesAvailable >= 2)
                    { /* The rest of the common OBEX header can be added to
                         the transport packet */
                        transportBuffer = obexUtilBuildSecondByteLength(totalHeaderLength, transportBuffer);
                        bytesTaken++;
                    }
                    break;
                }
            default:
                { /* The Type and the first length field has been sent.
                     Insert the last length field */
                    transportBuffer = obexUtilBuildSecondByteLength(totalHeaderLength, transportBuffer);
                    bytesTaken      = 1;
                    break;
                }
        }
        priInst->headerPayloadIndex   = (CsrUint16) (priInst->headerPayloadIndex + bytesTaken);
        priInst->transportPacketIndex = (CsrUint16) (priInst->transportPacketIndex + bytesTaken);
    }
    else
    {
        ;
    }
    return (transportBuffer);
}

static CsrUint8 * obexUtilCopyCommonHeader(ObexUtilPrivateInstType *priInst,
                                           CsrUint8                *header,
                                           CsrUint16               headerLength,
                                           CsrUint8                offset,
                                           CsrUint8              *transportBuffer,
                                           CsrBool                 *next)
{
    /* This function will copy as must as possible of OBEX header into
     * the transport packet. If *next is set to FALSE the transport
     * packet filled, if TRUE where are space for data from the next
     * header */
    CsrUint16 bytesAvailable = (CsrUint16)(priInst->maxTransportPacketLength - priInst->transportPacketIndex);
    *next = FALSE;

    if (bytesAvailable > 0)
    {
        /* There are bytes available in the transport packet. Find out
         * how many bytes of the header that still needs to be send */
        CsrUint16 payloadLength = (CsrUint16)(headerLength - priInst->headerPayloadIndex);

        if (bytesAvailable >= payloadLength)
        {
            /* All/the rest of the header can be placed in the
             * transport packet. Set next to TRUE to indicate that the
             * OBEX header is added to the transport packet */
            *next = TRUE;
        }
        else
        {
            /* Only a part of the header can be placed in the transport packet*/
            payloadLength =  bytesAvailable;
        }

        if (payloadLength > 0)
        {
            /* CsrMemCpy the header 'payload' into the transportpacket */
            CsrMemCpy(transportBuffer, &header[(priInst->headerPayloadIndex - offset)], payloadLength);
            priInst->transportPacketIndex = (CsrUint16) (priInst->transportPacketIndex + payloadLength);
            priInst->headerPayloadIndex   =  (CsrUint16)(priInst->headerPayloadIndex + payloadLength);
            transportBuffer += (payloadLength);
        }
        else
        {
            /* No 'payload' to copy. E.g. it could be the empty name
             * header that is being added */
            ;
        }
    }
    else
    {
        /* There are no bytes available in the transport packet */
        ;
    }
    return (transportBuffer);
}

static CsrUint8 * obexUtilCommonRequestHeaderHandler(ObexUtilPrivateInstType *priInst,
                                                     CsrUint8                headerType,
                                                     CsrUint8                *headerPayload,
                                                     CsrUint16               headerLength,
                                                     CsrUint8              *transportBuffer,
                                                     CsrBool                 *next)
{
    /* This functions makes sure that the common OBEX header (one byte
     * header opcode + two byte length) is added to the transport
     * packet and copies as must as possible of the header 'payload'
     * to the transport packet */
    if (priInst->headerPayloadIndex < CSR_BT_OBEX_HEADER_LENGTH)
    {
        /* The OBEX common header has not be created. Add as must of
         * this header as possible in the transport packet */
        transportBuffer = obexUtilBuildCommonObexRequestHeader(priInst,
                                                               headerType,
                                                               headerLength,
                                                               transportBuffer);
    }
    else
    {
        /* The common header of the given OBEX header has allready
         * been create and send in a transport packet. The bytes there
         * is available in the transport packet can be used to send
         * the OBEX header 'payload' */
        ;
    }

    /* Copies as must as possible of the OBEX header 'payload' to the
     * transport packet. The offset is CSR_BT_OBEX_HEADER_LENGTH (3)
     * because one byte opcode + two byte length in just added by the
     * function obexUtilBuildCommonObexRequestHeader and the function
     * obexUtilCopyCommonHeader expect that the total header in given
     * not only the header 'payload' (E.g the parameter headerLength
     * represent the total length of the header not only the header
     * 'payload') */
    transportBuffer = obexUtilCopyCommonHeader(priInst,
                                               headerPayload,
                                               headerLength,
                                               CSR_BT_OBEX_HEADER_LENGTH,
                                               transportBuffer,
                                               next);

    return (transportBuffer);
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static CsrUint8 * obexUtilAddMblkBodyHandler(ObexUtilPrivateInstType *priInst,
                                       CsrUint8                 headerType,
                                       CsrUint8                *transportBuffer)
{
    CsrMblk *bodyMblk = NULL;
    
    transportBuffer = obexUtilBuildCommonObexRequestHeader(priInst,
                                                           headerType,
                                                           obexUtilBodyLengthHandler(priInst),
                                                           transportBuffer);

    bodyMblk = CsrMblkDataCreate(priInst->body, priInst->bodyLength, TRUE);
    priInst->outgoingMblkPkt = CsrMblkAddTail(bodyMblk, priInst->outgoingMblkPkt);
    
    if( (priInst->preProcessState == CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_IDLE_PENDING_P_S)
        || (priInst->preProcessState == CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_IDLE_PENDING_P_S))
    {
        /* need to keep a copy of the body when sending the first put/get in case
           we need to resend it due to authentication */
        priInst->body = CsrMemDup(priInst->body, priInst->bodyLength);
    }
    else
    {
        priInst->body = NULL;
    }

    return transportBuffer;
}
#endif

static CsrUint8 * obexUtilBuildConnectRequestHeader(ObexUtilPrivateInstType *priInst,
                                                    CsrUint8              *transportBuffer,
                                                    CsrBool                 *next)
{
    /* This function handles all regarding adding the mandatory data
     * of an OBEX Connect Request packet into the transport packet. As
     * this is the first fragment of the OBEX/transport packet and the
     * mandatory data of an OBEX Connect Request is only 7 bytes it
     * must be able to be in the transport packet */
    priInst->transportPacketIndex = CSR_BT_OBEX_UTIL_CONNECT_LENGTH;
    *next           = TRUE;
    transportBuffer = obexUtilBuildCommonOpcodeHeaderHandler(CSR_BT_OBEX_CONNECT_OPCODE,
                                                             priInst->outgoingObexPacketLength,
                                                             transportBuffer);

    transportBuffer = obexUtilBuildMandatoryConnectHeader(transportBuffer,
                                                          priInst->maxLocalObexPacketLength);
    return (transportBuffer);
}

static CsrUint8 * obexUtilBuildConnectResponseHeader(ObexUtilPrivateInstType *priInst,
                                                     CsrUint8              *transportBuffer,
                                                     CsrBool                 *next)
{
    /* This function handles all regarding adding the mandatory data
     * of an OBEX Connect Response packet into the transport
     * packet. As this is the first fragment of the OBEX/transport
     * packet and the mandatory data of an OBEX Connect Response is
     * only 7 bytes it must be able to be in the transport packet */
    priInst->transportPacketIndex = CSR_BT_OBEX_UTIL_CONNECT_LENGTH;
    *next           = TRUE;
    transportBuffer = obexUtilBuildCommonOpcodeHeaderHandler((CsrUint8)priInst->responseCode,
                                                             priInst->outgoingObexPacketLength,
                                                             transportBuffer);

    if ((priInst->responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE &&
         priInst->servInst)
    { /* The Application has accepted the OBEX connection   */
        priInst->servInst->obexConnected = TRUE;
    }
    transportBuffer = obexUtilBuildMandatoryConnectHeader(transportBuffer,
                                                          priInst->maxLocalObexPacketLength);
    return (transportBuffer);
}

static CsrUint8 * obexUtilCommonRequestOperationHandler(ObexUtilPrivateInstType *priInst,
                                                        CsrUint8              *transportBuffer,
                                                        CsrBool                 *next,
                                                        CsrUint8                opCode)
{
    /* This function handles all regarding adding the mandatory data
     * of an common OBEX Request packet, e.g. add one byte opcode plus
     * two bytes length */
    priInst->transportPacketIndex    = CSR_BT_OBEX_HEADER_LENGTH;
    *next           = TRUE;
    transportBuffer = obexUtilBuildCommonOpcodeHeaderHandler(opCode,
                                                             priInst->outgoingObexPacketLength,
                                                             transportBuffer);
    return (transportBuffer);
}

static CsrUint8 * obexUtilBuildDisconnectRequestHeader(ObexUtilPrivateInstType *priInst,
                                                       CsrUint8              *transportBuffer,
                                                       CsrBool                 *next)
{ /* This function handles all regarding adding the mandatory data of
     an OBEX Disconnect Request packet into the transport packet. As this is
     the first fragment of the OBEX/transport packet and the mandatory data
     of an OBEX Disconnect Request is only 3 bytes it must be able to be in
     the transport packet */
    return (obexUtilCommonRequestOperationHandler(priInst, transportBuffer, next, CSR_BT_OBEX_DISCONNECT_OPCODE));
}

static CsrUint8 * obexUtilBuildPutRequestHeader(ObexUtilPrivateInstType *priInst,
                                                CsrUint8              *transportBuffer,
                                                CsrBool                 *next)
{ /* This function handles all regarding adding the mandatory data of
     an OBEX Put Request packet (where the final bit is not set) into the
     transport packet. As this is the first fragment of the OBEX/transport
     packet and the mandatory data of an OBEX Put Request is only 3 bytes
     it must be able to be in the transport packet */
    return (obexUtilCommonRequestOperationHandler(priInst, transportBuffer, next, CSR_BT_OBEX_PUT_OPCODE));
}

static CsrUint8 * obexUtilBuildPutFinalRequestHeader(ObexUtilPrivateInstType *priInst,
                                                     CsrUint8              *transportBuffer,
                                                     CsrBool                 *next)
{ /* This function handles all regarding adding the mandatory data of
     an OBEX Put Request packet (where the final bit is set) into the
     transport packet. As this is the first fragment of the OBEX/transport
     packet and the mandatory data of an OBEX Put Request is only 3 bytes
     it must be able to be in the transport packet */
    return (obexUtilCommonRequestOperationHandler(priInst,
                                                  transportBuffer,
                                                  next,
                                                  (CSR_BT_OBEX_PUT_OPCODE  | CSR_BT_OBEX_FINAL_BIT_MASK)));
}

#ifdef CSR_BT_INSTALL_OBEX_CLI_GET
static CsrUint8 * obexUtilGetRequestOperationHandler(ObexUtilPrivateInstType *priInst,
                                                     CsrUint8              *transportBuffer,
                                                     CsrBool                 *next)
{ /* This function handles all regarding adding the mandatory data of
     an OBEX Get Request packet (where the final bit is not set) into the
     transport packet. As this is the first fragment of the OBEX/transport
     packet and the mandatory data of an OBEX Get Request is only 3 bytes
     it must be able to be in the transport packet */
    return (obexUtilCommonRequestOperationHandler(priInst, transportBuffer, next, CSR_BT_OBEX_GET_OPCODE));
}

static CsrUint8 * obexUtilGetFinalRequestOperationHandler(ObexUtilPrivateInstType *priInst,
                                                          CsrUint8              *transportBuffer,
                                                          CsrBool                 *next)
{ /* This function handles all regarding adding the mandatory data of
     an OBEX Get Request packet (where the final bit is set) into the
     transport packet. As this is the first fragment of the OBEX/transport
     packet and the mandatory data of an OBEX Get Request is only 3 bytes
     it must be able to be in the transport packet */
    return (obexUtilCommonRequestOperationHandler(priInst,
                                                  transportBuffer,
                                                  next,
                                                  (CSR_BT_OBEX_GET_OPCODE  | CSR_BT_OBEX_FINAL_BIT_MASK)));
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_CLI_SET_PATH
static CsrUint8 * obexUtilSetpathRequestOperationHandler(ObexUtilPrivateInstType  *priInst,
                                                         CsrUint8               *transportBuffer,
                                                         CsrBool                  *next)
{ /* This function handles all regarding adding the mandatory data of
     an OBEX Setpath Request packet into the transport packet. As this is
     the first fragment of the OBEX/transport packet and the mandatory data
     of an OBEX Setpath Request is only 5 bytes it must be able to be in
     the transport packet */
    priInst->transportPacketIndex = CSR_BT_OBEX_UTIL_SETPATH_REQUEST_LENGTH;

    *next           = TRUE;
    transportBuffer = obexUtilBuildCommonOpcodeHeaderHandler(CSR_BT_OBEX_SET_PATH_OPCODE,
                                                             priInst->outgoingObexPacketLength,
                                                             transportBuffer);

    transportBuffer = obexUtilBuildMandatorySetPathHeader(transportBuffer,
                                                          priInst->cliInst->setpathFlags,
                                                          0x00);
    return (transportBuffer);
}
#endif

static CsrUint8 * obexUtilAbortRequestOperationHandler(ObexUtilPrivateInstType    *priInst,
                                                       CsrUint8                 *transportBuffer,
                                                       CsrBool                    *next)
{ /* This function handles all regarding adding the mandatory data of
     an OBEX Abort Request packet into the transport packet. As this
     is the first fragment of the OBEX/transport packet and the mandatory
     data of an OBEX Abort Request is only 3 bytes it must be able to be
     in the transport packet */
    return (obexUtilCommonRequestOperationHandler(priInst,
                                                  transportBuffer,
                                                  next,
                                                  (CSR_BT_OBEX_ABORT_OPCODE  | CSR_BT_OBEX_FINAL_BIT_MASK)));
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static CsrUint8 * obexUtilActionRequestOperationHandler(ObexUtilPrivateInstType  *priInst,
                                                        CsrUint8               *transportBuffer,
                                                        CsrBool                  *next)
{ /* This function handles all regarding adding the first 3 bytes of
     an OBEX Action Request packet into the transport packet. As this is
     the first fragment of the OBEX/transport packet and there fore it
     must be able to be in the transport packet. Please note only these headers
     are included in this funtion (one byte opcode + two byte length ) */
    return (obexUtilCommonRequestOperationHandler(priInst, transportBuffer, next, CSR_BT_OBEX_ACTION_OPCODE));
}
#endif


static CsrUint8 * obexUtilCommonResponseOperationHandler(ObexUtilPrivateInstType  *priInst,
                                                         CsrUint8               *transportBuffer,
                                                         CsrBool                  *next)
{ /* This function handles all regarding adding the mandatory data of
     an common OBEX Response packet into the transport packet. As this is
     the first fragment of the OBEX/transport packet and the mandatory data
     of an common OBEX Response is only 3 bytes it must be able to be in
     the transport packet */
    priInst->transportPacketIndex    = CSR_BT_OBEX_HEADER_LENGTH;
    *next           = TRUE;
    transportBuffer = obexUtilBuildCommonOpcodeHeaderHandler((CsrUint8)priInst->responseCode,
                                                             priInst->outgoingObexPacketLength,
                                                             transportBuffer);
    return (transportBuffer);
}

static CsrUint16 obexUtilCsrUcs2StringHeaderLengthHandler(CsrUcs2String *csrUcs2StringHeader)
{ /* Returns the total length of an CsrUcs2String header, e.g. the
     header (one byte opcode + two byte length) plus header 'payload' */
    CsrUint16 length = (CsrUint16) CsrUcs2ByteStrLen(csrUcs2StringHeader);

    if (length > 0)
    {
        length = (CsrUint16)(length + 2 + CSR_BT_OBEX_HEADER_LENGTH);
    }
    else
    {
        length = CSR_BT_OBEX_HEADER_LENGTH;
    }
    return (length);
}


   /* Returns the total length of the name header, e.g. the name header
     (one byte opcode + two byte length) plus name header 'payload' */
#define obexUtilNameLengthHandler(priInst) (obexUtilCsrUcs2StringHeaderLengthHandler((priInst)->name))

static CsrUint8 * obexUtilNameHeaderHandler(ObexUtilPrivateInstType *priInst,
                                            CsrUint8              *transportBuffer,
                                            CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX Name header into
     a transport packet */
    transportBuffer = obexUtilCommonRequestHeaderHandler(priInst,
                                                         CSR_BT_OBEX_NAME_HEADER,
                                                         priInst->name,
                                                         obexUtilNameLengthHandler(priInst),
                                                         transportBuffer,
                                                         next);
    return (transportBuffer);
}

static CsrUint8 * obexUtilEmptyNameHeaderHandler(ObexUtilPrivateInstType *priInst,
                                                 CsrUint8              *transportBuffer,
                                                 CsrBool                 *next)
{ /* This function handles all regarding adding an empty OBEX Name header
     into a transport packet */
    transportBuffer = obexUtilCommonRequestHeaderHandler(priInst,
                                                         CSR_BT_OBEX_NAME_HEADER,
                                                         NULL,
                                                         CSR_BT_OBEX_HEADER_LENGTH,
                                                         transportBuffer,
                                                         next);
    return (transportBuffer);
}

static CsrUint8 * obexUtilTypeHeaderHandler(ObexUtilPrivateInstType *priInst,
                                            CsrUint8              *transportBuffer,
                                            CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX Type header into
     a transport packet. The offset is set to 0 because in priInst->type
     is the one byte opcode + two byte length field already included */
    transportBuffer = obexUtilCopyCommonHeader(priInst,
                                               priInst->type,
                                               priInst->typeLength,
                                               0,
                                               transportBuffer,
                                               next);
    return (transportBuffer);
}

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    /* Returns the total length of the descriptor header, e.g. the descriptor
     header (one byte opcode + two byte length) plus descriptor header 'payload' */
#define obexUtilDescriptionLengthHandler(priInst) obexUtilCsrUcs2StringHeaderLengthHandler((priInst)->description)

static CsrUint8 * obexUtilDescriptionHeaderHandler(ObexUtilPrivateInstType *priInst,
                                                   CsrUint8              *transportBuffer,
                                                   CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX Description
     header into a transport packet */
    transportBuffer = obexUtilCommonRequestHeaderHandler(priInst,
                                                         CSR_BT_OBEX_DESCRIPTION_HEADER,
                                                         priInst->description,
                                                         obexUtilDescriptionLengthHandler(priInst),
                                                         transportBuffer,
                                                         next);
    return (transportBuffer);
}
#endif

static CsrUint8 * obexUtilLengthOfObjectHeaderHandler(ObexUtilPrivateInstType *priInst,
                                                      CsrUint8              *transportBuffer,
                                                      CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX Length header into
     a transport packet */
    CsrUint16 bytesAvailable = (CsrUint16)(priInst->maxTransportPacketLength - priInst->transportPacketIndex);

    if (bytesAvailable >= CSR_BT_OBEX_UTIL_LENGTH_OF_OBJECT_LENGTH)
    { /* The OBEX length header is added to the transport packet */
        transportBuffer = obexUtilBuildCsrUint32Header(transportBuffer,
                                                       CSR_BT_OBEX_LENGTH_HEADER,
                                                       priInst->lengthOfObject);

        priInst->transportPacketIndex =  (CsrUint16)(priInst->transportPacketIndex + CSR_BT_OBEX_UTIL_LENGTH_OF_OBJECT_LENGTH);
        *next           = TRUE;
    }
    else
    { /* The OBEX length header could not be placed in the transport packet */
        *next           = FALSE;
    }
    return (transportBuffer);
}

    /* This function return the total length of the target/who header, e.g.
     the target/who header (one byte opcode + two byte length) */
#define obexUtilTargetWhoLengthHandler(priInst) (CsrUint16)((priInst)->targetWhoHeaderLength + CSR_BT_OBEX_HEADER_LENGTH)

#ifdef CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID
static CsrUint8 * obexUtilTargetHeaderHandler(ObexUtilPrivateInstType *priInst,
                                              CsrUint8              *transportBuffer,
                                              CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX Target header into
     a transport packet. */
    transportBuffer = obexUtilCommonRequestHeaderHandler(priInst,
                                                         CSR_BT_OBEX_TARGET_HEADER,
                                                         priInst->targetWho,
                                                         obexUtilTargetWhoLengthHandler(priInst),
                                                         transportBuffer,
                                                         next);
    return (transportBuffer);
}
#endif
#ifdef CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID
static CsrUint8 * obexUtilWhoHeaderHandler(ObexUtilPrivateInstType *priInst,
                                           CsrUint8              *transportBuffer,
                                           CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX Who header into a
     transport packet. */
    transportBuffer = obexUtilCommonRequestHeaderHandler(priInst,
                                                         CSR_BT_OBEX_WHO_HEADER,
                                                         priInst->targetWho,
                                                         obexUtilTargetWhoLengthHandler(priInst),
                                                         transportBuffer,
                                                         next);
    return (transportBuffer);
}
#endif
static CsrUint8 * obexUtilBodyHeaderHandler(ObexUtilPrivateInstType *priInst,
                                            CsrUint8              *transportBuffer,
                                            CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX body
     header into a transport packet */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if(priInst->outgoingMblkPkt != NULL)
    {
        transportBuffer = obexUtilAddMblkBodyHandler(priInst, CSR_BT_OBEX_BODY_HEADER, transportBuffer);
    }
    else
#endif
    {
        transportBuffer = obexUtilCommonRequestHeaderHandler(priInst,
                                                              CSR_BT_OBEX_BODY_HEADER,
                                                              priInst->body,
                                                              obexUtilBodyLengthHandler(priInst),
                                                              transportBuffer,
                                                              next);
    }
    
    return (transportBuffer);
}

static CsrUint8 * obexUtilEndBodyHeaderHandler(ObexUtilPrivateInstType *priInst,
                                               CsrUint8              *transportBuffer,
                                               CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX End body
     header into a transport packet */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if(priInst->outgoingMblkPkt != NULL)
    {
        transportBuffer = obexUtilAddMblkBodyHandler(priInst, CSR_BT_OBEX_END_OF_BODY_HEADER, transportBuffer);
    }
    else
#endif
    {
        transportBuffer = obexUtilCommonRequestHeaderHandler(priInst,
                                                              CSR_BT_OBEX_END_OF_BODY_HEADER,
                                                              priInst->body,
                                                              obexUtilBodyLengthHandler(priInst),
                                                              transportBuffer,
                                                              next);
    }

    return (transportBuffer);
}

#ifdef CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID
static CsrUint8 * obexUtilConnectionIdHeaderHandler(ObexUtilPrivateInstType *priInst,
                                                    CsrUint8              *transportBuffer,
                                                    CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX ConnectionId
     header into a transport packet */
    CsrUint16 bytesAvailable = (CsrUint16)(priInst->maxTransportPacketLength - priInst->transportPacketIndex);

    if (bytesAvailable >= CSR_BT_OBEX_UTIL_CONNECTION_ID_LENGTH)
    { /* Add the OBEX ConnectionId header to the transport packet */
        transportBuffer = obexUtilBuildCsrUint32Header(transportBuffer,
                                                       CSR_BT_OBEX_CONNECTION_ID_HEADER,
                                                       priInst->connectionId);

        priInst->transportPacketIndex = (CsrUint16)(priInst->transportPacketIndex + CSR_BT_OBEX_UTIL_CONNECTION_ID_LENGTH);
        *next           = TRUE;
    }
    else
    { /* The OBEX ConnectionId header could not be placed in the transport
         packet */
        *next           = FALSE;
    }
    return (transportBuffer);
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
static CsrUint8 * obexUtilAppParametersHeaderHandler(ObexUtilPrivateInstType *priInst,
                                                     CsrUint8              *transportBuffer,
                                                     CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX application
     parameter header into a transport packet. The offset is set to 0
     because in priInst->appParametersHeader is the one byte opcode +
     two byte length field already included */
    transportBuffer = obexUtilCopyCommonHeader(priInst,
                                               priInst->appParametersHeader,
                                               priInst->appParametersHeaderLength,
                                               0,
                                               transportBuffer,
                                               next);

    return (transportBuffer);
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
static CsrUint8 * obexUtilUserDefinedHeaderHandler(ObexUtilPrivateInstType *priInst,
                                                   CsrUint8              *transportBuffer,
                                                   CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX user defined
     header into a transport packet. The offset is set to 0 because in
     priInst->userDefinedHeader is the one byte opcode + two byte length
     field already included */
    transportBuffer = obexUtilCopyCommonHeader(priInst,
                                               priInst->userDefinedHeader,
                                               priInst->userDefinedHeaderLength,
                                               0,
                                               transportBuffer,
                                               next);

    return (transportBuffer);
}
#endif

static CsrUint8 * obexUtilCountHeaderHandler(ObexUtilPrivateInstType *priInst,
                                             CsrUint8              *transportBuffer,
                                             CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX Count header into
     a transport packet */
    CsrUint16 bytesAvailable = (CsrUint16)(priInst->maxTransportPacketLength - priInst->transportPacketIndex);

    if (bytesAvailable >= CSR_BT_OBEX_UTIL_COUNT_LENGTH)
    { /* The OBEX Count header is added to the transport packet */
        transportBuffer = obexUtilBuildCsrUint32Header(transportBuffer,
                                                       CSR_BT_OBEX_COUNT_HEADER,
                                                       priInst->count);

        priInst->transportPacketIndex =  (CsrUint16)(priInst->transportPacketIndex + CSR_BT_OBEX_UTIL_COUNT_LENGTH);
        *next           = TRUE;
    }
    else
    { /* The OBEX Count header could not be placed in the transport packet */
        *next           = FALSE;
    }
    return (transportBuffer);
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static CsrUint8 * obexUtilSingleResponseModeHeaderHandler(ObexUtilPrivateInstType *priInst,
                                                          CsrUint8              *transportBuffer,
                                                          CsrBool                 *next)
{ /* This function adds the OBEX Single Response Mode header to the
     transport packet. Note no need to check that this header can be placed
     in the OBEX packet as it is one of the first being build/send */
    if (priInst->singleResponseModeHeader)
    {
        transportBuffer = obexUtilCopyCommonHeader(priInst,
                                                   priInst->singleResponseModeHeader,
                                                   CSR_BT_OBEX_UTIL_SINGLE_RESPONSE_MODE_LENGTH,
                                                   0,
                                                   transportBuffer,
                                                   next);
    }
    else
    { /* The OBEX Single Response Mode header is remove because it is not an
         multi-step OBEX operation */
        *next = TRUE;
    }
    return (transportBuffer);
}

     /* Returns the total length of the destName header, e.g. the destName header
     (one byte opcode + two byte length) plus destName header 'payload' */
#define obexUtilDestNameLengthHandler(priInst) (obexUtilCsrUcs2StringHeaderLengthHandler((priInst)->destName))

static CsrUint8 * obexUtilDestNameHeaderHandler(ObexUtilPrivateInstType *priInst,
                                                CsrUint8              *transportBuffer,
                                                CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX Name header into
     a transport packet */
    transportBuffer = obexUtilCommonRequestHeaderHandler(priInst,
                                                         CSR_BT_OBEX_DESTNAME_HEADER,
                                                         priInst->destName,
                                                         obexUtilDestNameLengthHandler(priInst),
                                                         transportBuffer,
                                                         next);
    return (transportBuffer);
}

static CsrUint8 * obexUtilPermissionsHeaderHandler(ObexUtilPrivateInstType *priInst,
                                                   CsrUint8              *transportBuffer,
                                                   CsrBool                 *next)
{ /* This function handles all regarding adding the OBEX Permissions header
     into a transport packet */
    CsrUint16 bytesAvailable = (CsrUint16)(priInst->maxTransportPacketLength - priInst->transportPacketIndex);

    if (bytesAvailable >= CSR_BT_OBEX_UTIL_PERMISSIONS_LENGTH)
    { /* The OBEX Permission header is added to the transport packet */
        transportBuffer = obexUtilBuildCsrUint32Header(transportBuffer,
                                                       CSR_BT_OBEX_PERMISSIONS_HEADER,
                                                       priInst->permission);

        priInst->transportPacketIndex = (CsrUint16)(priInst->transportPacketIndex + CSR_BT_OBEX_UTIL_PERMISSIONS_LENGTH);
        *next = TRUE;
    }
    else
    { /* The OBEX Permissions header could not be placed in the transport packet*/
        *next = FALSE;
    }
    return (transportBuffer);
}

static CsrUint8 * obexUtilBuildCsrUint8Header(CsrUint8 *buffer,
                                              CsrUint8   headerId,
                                              CsrUint8   headerValue)
{
    *buffer        = headerId;
    buffer++;
    *buffer        = headerValue;
    buffer++;
    return buffer;
}

static CsrUint8 * obexUtilActionIdHeaderHandler(ObexUtilPrivateInstType *priInst,
                                                CsrUint8              *transportBuffer,
                                                CsrBool                 *next)
{
    /* This function adds the OBEX ActionId header to the transport
     * packet. Note no need to check that this header can be placed in
     * the OBEX packet as it is one of the first being build/send */
    transportBuffer = obexUtilBuildCsrUint8Header(transportBuffer,
                                                  CSR_BT_OBEX_ACTION_ID_HEADER,
                                                  priInst->actionId);

    priInst->transportPacketIndex = (CsrUint16)(priInst->transportPacketIndex
                                                + CSR_BT_OBEX_UTIL_ACTION_ID_LENGTH);
    *next                         = TRUE;
    return (transportBuffer);
}

static CsrUint8 * obexUtilBuildSrmpHeader(ObexUtilPrivateInstType *priInst,
                                          CsrUint8              *transportBuffer,
                                          CsrBool                 *next)
{ /* This function adds the OBEX SRMP header to the transport packet. Note no
     need to check that this header can be placed in the OBEX packet as it is
     one of the first being build/send */
    if (priInst->srmpHeader)
    {
        transportBuffer = obexUtilCopyCommonHeader(priInst,
                                                   priInst->srmpHeader,
                                                   CSR_BT_OBEX_UTIL_SRMP_LENGTH,
                                                   0,
                                                   transportBuffer,
                                                   next);
    }
    else
    {
        /* The OBEX Single Response Mode header were remove because it
         * is not an multi-step OBEX operation. The SRMP header must
         * only be used if the SRM is used */
        *next = TRUE;
    }
    return (transportBuffer);
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE

static CsrUint8 * obexUtilAuthRspHeaderHandler(ObexUtilPrivateInstType * priInst,
                                               CsrUint8         * transportBuffer,
                                               CsrBool             * next)
{ /* This function handles all regarding adding the OBEX Digest Response
     header into a transport packet */
    if (priInst->digestResponseHeader)
    { /* The OBEX client or server has been challenge. The offset is set to
         0 because in priInst->digestResponseHeader is the one byte opcode
         + two byte length field already included */
        transportBuffer = obexUtilCopyCommonHeader(priInst,
                                                   priInst->digestResponseHeader,
                                                   priInst->digestResponseLength,
                                                   0,
                                                   transportBuffer,
                                                   next);
    }
    else
    { /* The OBEX client has not been challenge by the by the server */
        *next = TRUE;
    }
    return (transportBuffer);
}
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE

static CsrUint8 * obexUtilAuthChalHeaderHandler(ObexUtilPrivateInstType * priInst,
                                                CsrUint8         * transportBuffer,
                                                CsrBool             * next)
{ /* This function handles all regarding adding the OBEX Auth challenge
     header into a transport packet */
    if (priInst->digestChallengeHeader)
    { /* The OBEX client or server has been challenge. The offset is set to
         0 because in priInst->digestChallengeHeader is the one byte opcode
         + two byte length field already included */
        transportBuffer = obexUtilCopyCommonHeader(priInst,
                                                   priInst->digestChallengeHeader,
                                                   priInst->digestChallengeLength,
                                                   0,
                                                   transportBuffer,
                                                   next);
    }
    else
    { /* The OBEX client has not been challenge by the by the server */
        *next = TRUE;
    }
    return (transportBuffer);
}
#endif

static const ObexUtilBuildPacketHandlerType obexUtilBuildTransportPacketHandlers[CSR_BT_OBEX_UTIL_NUM_OF_HEADERS] =
{
    obexUtilBuildConnectRequestHeader,          /* CSR_BT_OBEX_UTIL_CONNECT_REQUEST_OPERATION */
    obexUtilBuildConnectResponseHeader,         /* CSR_BT_OBEX_UTIL_CONNECT_RESPONSE_OPERATION */
    obexUtilBuildDisconnectRequestHeader,       /* CSR_BT_OBEX_UTIL_DISCONNECT_REQUEST_OPERATION */
    obexUtilBuildPutRequestHeader,              /* CSR_BT_OBEX_UTIL_PUT_REQUEST_OPERATION */
    obexUtilBuildPutFinalRequestHeader,         /* CSR_BT_OBEX_UTIL_PUT_FINAL_REQUEST_OPERATION */
#ifdef CSR_BT_INSTALL_OBEX_CLI_GET
    obexUtilGetRequestOperationHandler,         /* CSR_BT_OBEX_UTIL_GET_REQUEST_OPERATION */
    obexUtilGetFinalRequestOperationHandler,    /* CSR_BT_OBEX_UTIL_GET_FINAL_REQUEST_OPERATION */
#else
    NULL,
    NULL,
#endif
#ifdef CSR_BT_INSTALL_OBEX_CLI_SET_PATH
    obexUtilSetpathRequestOperationHandler,     /* CSR_BT_OBEX_UTIL_SETPATH_REQUEST_OPERATION */
#else
    NULL,                                       /* CSR_BT_OBEX_UTIL_SETPATH_REQUEST_OPERATION */
#endif
    obexUtilAbortRequestOperationHandler,       /* CSR_BT_OBEX_UTIL_ABORT_REQUEST_OPERATION */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    obexUtilActionRequestOperationHandler,      /* CSR_BT_OBEX_UTIL_ACTION_REQUEST_OPERATION */
#else
    NULL,                                       /* CSR_BT_OBEX_UTIL_ACTION_REQUEST_OPERATION */
#endif
    obexUtilCommonResponseOperationHandler,     /* CSR_BT_OBEX_UTIL_COMMON_RESPONSE_OPERATION */
    obexUtilNameHeaderHandler,                  /* CSR_BT_OBEX_UTIL_NAME_HEADER */
    obexUtilEmptyNameHeaderHandler,             /* CSR_BT_OBEX_UTIL_EMPTY_NAME_HEADER */
    obexUtilTypeHeaderHandler,                  /* CSR_BT_OBEX_UTIL_TYPE_HEADER */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    obexUtilDescriptionHeaderHandler,           /* CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER */
#else
    NULL,
#endif    
    obexUtilLengthOfObjectHeaderHandler,        /* CSR_BT_OBEX_UTIL_LENGTH_HEADER */
#ifdef CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID
    obexUtilTargetHeaderHandler,                /* CSR_BT_OBEX_UTIL_TARGET_HEADER */
#else
    NULL,
#endif    
#ifdef CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID
    obexUtilWhoHeaderHandler,                   /* CSR_BT_OBEX_UTIL_WHO_HEADER */
#else
    NULL,
#endif    
    obexUtilBodyHeaderHandler,                  /* CSR_BT_OBEX_UTIL_BODY_HEADER */
    obexUtilEndBodyHeaderHandler,               /* CSR_BT_OBEX_UTIL_END_BODY_HEADER */
#ifdef CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID
    obexUtilConnectionIdHeaderHandler,          /* CSR_BT_OBEX_UTIL_CONNECTIONID_HEADER */
#else
    NULL,
#endif    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
    obexUtilAppParametersHeaderHandler,         /* CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    obexUtilAuthChalHeaderHandler,              /* CSR_BT_OBEX_UTIL_AUTH_CHALLENGE_HEADER */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
    obexUtilAuthRspHeaderHandler,               /* CSR_BT_OBEX_UTIL_AUTH_RESPONSE_HEADER */
#else
    NULL,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    obexUtilUserDefinedHeaderHandler,           /* CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER */
#else    
    NULL,
#endif    
    obexUtilCountHeaderHandler,                 /* CSR_BT_OBEX_UTIL_COUNT_HEADER */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    obexUtilSingleResponseModeHeaderHandler,    /* CSR_BT_OBEX_UTIL_SINGLE_RESPONSE_MODE_HEADER */
    obexUtilDestNameHeaderHandler,              /* CSR_BT_OBEX_UTIL_DESTNAME_HEADER */
    obexUtilPermissionsHeaderHandler,           /* CSR_BT_OBEX_UTIL_PERMISSIONS_HEADER */
    obexUtilActionIdHeaderHandler,              /* CSR_BT_OBEX_UTIL_ACTION_ID_HEADER */
    obexUtilBuildSrmpHeader,                    /* CSR_BT_OBEX_UTIL_SRMP_HEADER */
#endif
};

CsrUintFast16 obexUtilHeaderLength(ObexUtilPrivateInstType * priInst, CsrUintFast16 headerType)
{
    switch(headerType)
    {
    case CSR_BT_OBEX_UTIL_CONNECT_REQUEST_OPERATION:
    case CSR_BT_OBEX_UTIL_CONNECT_RESPONSE_OPERATION:
        {
            return CSR_BT_OBEX_UTIL_CONNECT_LENGTH;
        }
#ifdef CSR_BT_INSTALL_OBEX_CLI_SET_PATH
    case CSR_BT_OBEX_UTIL_SETPATH_REQUEST_OPERATION:
        {
            return CSR_BT_OBEX_UTIL_SETPATH_REQUEST_LENGTH;
        }
#endif    
    case CSR_BT_OBEX_UTIL_NAME_HEADER:
        {
            return obexUtilCsrUcs2StringHeaderLengthHandler(priInst->name);
        }
    case CSR_BT_OBEX_UTIL_TYPE_HEADER:
        {
            return priInst->typeLength;
        }
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    case CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER:
        {
            return obexUtilCsrUcs2StringHeaderLengthHandler(priInst->description);
        }
#endif
    case CSR_BT_OBEX_UTIL_LENGTH_HEADER:
        {
            return CSR_BT_OBEX_UTIL_LENGTH_OF_OBJECT_LENGTH;
        }
#if defined(CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID) || defined(CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID)
    case CSR_BT_OBEX_UTIL_TARGET_HEADER:
    case CSR_BT_OBEX_UTIL_WHO_HEADER:
        {
            return priInst->targetWhoHeaderLength + CSR_BT_OBEX_HEADER_LENGTH;
        }
#endif
    case CSR_BT_OBEX_UTIL_BODY_HEADER:
    case CSR_BT_OBEX_UTIL_END_BODY_HEADER:
        {
            return (CsrUintFast16)(priInst->bodyLength + CSR_BT_OBEX_HEADER_LENGTH);
        }
    case CSR_BT_OBEX_UTIL_CONNECTIONID_HEADER:
        {
            return CSR_BT_OBEX_UTIL_CONNECTION_ID_LENGTH;
        }
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
    case CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER:
        {
            return priInst->appParametersHeaderLength;
        }
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    case CSR_BT_OBEX_UTIL_AUTH_CHALLENGE_HEADER:
        {
            if (priInst->digestChallengeHeader)
            {/* Return the length of Digest Challenge Header */
                return (priInst->digestChallengeLength);
            }
            else
            {/* There is no OBEX Digest Challenge header, therefore the length is 0 */
                return (0);
            }
        }
#endif    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
    case CSR_BT_OBEX_UTIL_AUTH_RESPONSE_HEADER:
        {
            if (priInst->digestResponseHeader)
            { /* Return the length of Digest Response Header */
                return (priInst->digestResponseLength);
            }
            else
            { /* There is no OBEX Digest Response header, therefore the length is 0 */
                return (0);
            }
        }
#endif    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    case CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER:
        {
            return priInst->userDefinedHeaderLength;
        }
#endif
    case CSR_BT_OBEX_UTIL_COUNT_HEADER:
        {
            return CSR_BT_OBEX_UTIL_COUNT_LENGTH;
        }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    case CSR_BT_OBEX_UTIL_SINGLE_RESPONSE_MODE_HEADER:
        {
            if (priInst->singleResponseModeHeader)
            {
                return(CSR_BT_OBEX_UTIL_SINGLE_RESPONSE_MODE_LENGTH);
            }
            else
            {
                return 0;
            }
        }
    case CSR_BT_OBEX_UTIL_DESTNAME_HEADER:
        {
            return (obexUtilCsrUcs2StringHeaderLengthHandler(priInst->destName));
        }
    case CSR_BT_OBEX_UTIL_PERMISSIONS_HEADER:
        {
            return(CSR_BT_OBEX_UTIL_PERMISSIONS_LENGTH);
        }
    case CSR_BT_OBEX_UTIL_ACTION_ID_HEADER:
        {
            return(CSR_BT_OBEX_UTIL_ACTION_ID_LENGTH);
        }
    case CSR_BT_OBEX_UTIL_SRMP_HEADER:
        {
            return(CSR_BT_OBEX_UTIL_SRMP_LENGTH);
        }
#endif
    default:
        {
            /* CSR_BT_OBEX_UTIL_DISCONNECT_REQUEST_OPERATION */
            /* CSR_BT_OBEX_UTIL_PUT_REQUEST_OPERATION */
            /* CSR_BT_OBEX_UTIL_PUT_FINAL_REQUEST_OPERATION */
            /* CSR_BT_OBEX_UTIL_GET_REQUEST_OPERATION */
            /* CSR_BT_OBEX_UTIL_GET_FINAL_REQUEST_OPERATION */
            /* CSR_BT_OBEX_UTIL_ABORT_REQUEST_OPERATION */
            /* CSR_BT_OBEX_UTIL_ACTION_REQUEST_OPERATION */
            /* CSR_BT_OBEX_UTIL_COMMON_RESPONSE_OPERATION */
            /* CSR_BT_OBEX_UTIL_EMPTY_NAME_HEADER */
            return CSR_BT_OBEX_HEADER_LENGTH;
        }
    } 
}

/*------------------------------------------------------------------------------*
 * Static Obex util functions                                                   *
 *------------------------------------------------------------------------------*/
static void obexUtilSetPtrFromDoublePtr(void **dest, void **src)
{ /* This function set the content of src to dest and set 
     the src content to NULL */
    if (src)
    {
        *dest = *src;
        *src  = NULL;
    }
    else
    {
        *dest = NULL;
    }
}

static void obexUtilPfreeDoublePtr(void **ptr)
{ /* This function the CsrPmemFree the content of the double pointer and set 
     the content to NULL */
    if (ptr)
    {
        CsrPmemFree(*ptr);
        *ptr = NULL;
    }
}

#define obexUtilCmMsgResultHandler(_resultCode, _resultSupplier) \
    /* Checks if the message received from the CM is a success or not */ \
    ((_resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS) && (_resultSupplier == CSR_BT_SUPPLIER_CM))

static void obexUtilCmMsgSetResultHandler(ObexUtilPrivateInstType * priInst,
                                          CsrBtResultCode         resultCode,
                                          CsrBtSupplier           resultSupplier)
{ /* Checks if the message received from the CM fails because it were cancelled
     or not */
    if (priInst->processState == CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECT_REQUEST_P_S)
    { /* The procedure fails because it was cancelled */
        priInst->resultCode     = CSR_BT_RESULT_CODE_OBEX_CONNECT_CANCELLED;
        priInst->resultSupplier = CSR_BT_SUPPLIER_OBEX_PROFILES;
    }
    else
    { /* The procedure just fails */
        priInst->resultCode     = resultCode;
        priInst->resultSupplier = resultSupplier;
    }
}

static void obexUtilSetProcessState(ObexUtilPrivateInstType * priInst, CsrUint8 processState)
{ /* Set preProcessState to processState and then set processState */
    priInst->preProcessState = priInst->processState;
    priInst->processState    = processState;
}

static void obexUtilLpSuperVisionTimeoutHandler(CsrUint16 dummy, void *mv)
{
    /* This function is called whenever the lpSuperVisionTimer fires */
    ObexUtilPrivateInstType *priInst = (ObexUtilPrivateInstType*)mv;

    CSR_UNUSED(dummy);

    if (priInst->processState   == CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S &&
        priInst->actualLinkMode == CSR_BT_ACTIVE_MODE)
    {
        /* No OBEX operation is running. Request to enter SNIFF mode */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if(CSR_BT_CONN_ID_IS_L2CA(priInst->btConnId))
        {
            /* The transport connection is a l2cap connection */
            CsrBtCml2caModeChangeReqSend(priInst->btConnId, CSR_BT_SNIFF_MODE);
        }
        else
#endif
        {
            /* The transport connection is a rfcomm connection */
            CsrBtCmRfcModeChangeReqSend(priInst->btConnId, CSR_BT_SNIFF_MODE);
        }
        priInst->requestedLinkMode = CSR_BT_SNIFF_MODE;
    }
    else
    { /* Just ignore this */
        ;
    }
    priInst->lpSuperVisionTimerId = 0;
}

static void obexUtilResetObexCmdSuperVisionTimer(ObexUtilPrivateInstType *priInst)
{
    /* This function resets the obexCmdSuperVisionTimer */
    if (priInst->obexCmdSuperVisionTimerId != 0)
    {
        /* The obexCmdSuperVisionTimer is running, cancel it */
        CsrSchedTimerCancel(priInst->obexCmdSuperVisionTimerId, NULL, NULL);
        priInst->obexCmdSuperVisionTimerId  = 0;
    }
    else
    {
        /* No obexCmdSuperVisionTimer is running */
        ;
    }
}

static void obexUtilReleaseTransportConnection(ObexUtilPrivateInstType * priInst)
{
    /* This function request to release the established transport connection */
    if(priInst->cmState != CSR_BT_OBEX_UTIL_BT_DISCONNECTING_S)
    {
        ObexUtilInstType *inst = priInst->obexUtilOwnInstData;

        /* Reset the ObexCmdSuperVisionTimer */
        obexUtilResetObexCmdSuperVisionTimer(priInst);

        if (priInst->cmState == CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S)
        {
            /* The deactivation procedure is running, keep this state */
            ;
        }
        else
        {
            /* Change the state */
            priInst->cmState = CSR_BT_OBEX_UTIL_BT_DISCONNECTING_S;
        }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if (CSR_BT_CONN_ID_IS_L2CA(priInst->btConnId))
        {
            /* The transport connection is a L2CAP connection */
            CsrBtCmContextl2caDisconnectReqSend(priInst->btConnId, inst->instId);
        }
        else
#endif
        {
            /* The transport connection is a RFCOMM connection */
            CsrBtCmContextDisconnectReqSend(priInst->btConnId,
                                            inst->instId);
        }
    }
    else
    { /* The transport connection is allready being release */
        ;
    }
}

static void obexUtilReleaseTransportHandler(ObexUtilPrivateInstType *priInst,
                                            CsrBtResultCode         resultCode,
                                            CsrUint8                processState)
{
    priInst->resultCode     = resultCode;
    priInst->resultSupplier = CSR_BT_SUPPLIER_OBEX_PROFILES;
    obexUtilSetProcessState(priInst, processState);
    obexUtilReleaseTransportConnection(priInst);
}

static void obexUtilResetLpSuperVisionTimer(ObexUtilPrivateInstType * priInst)
{
    /* This function resets the lpSuperVisionTimer */
    if (priInst->lpSuperVisionTimerId != 0)
    {
        /* The lpSuperVisionTimer is running, cancel it */
        CsrSchedTimerCancel(priInst->lpSuperVisionTimerId, NULL, NULL);
        priInst->lpSuperVisionTimerId  = 0;
    }
    else
    {
        /* No lpSuperVisionTimer is running */
        ;
    }
}

static void obexUtilActivateLpSuperVisionTimer(ObexUtilPrivateInstType * priInst)
{
    /* This function check if the lpSuperVisionTimer must be activate or not */
    if (priInst->lpSuperVisionTimeout != CSR_BT_INFINITE_TIME)
    {
        /* The lpSuperVisionTimer must be activated */
        obexUtilResetLpSuperVisionTimer(priInst);
        priInst->lpSuperVisionTimerId = CsrSchedTimerSet(priInst->lpSuperVisionTimeout,
                                                         obexUtilLpSuperVisionTimeoutHandler,
                                                         0, priInst);
    }
    else
    {
        /* The lpSuperVisionTimer shall not be activated */
        ;
    }
}


static void obexUtilObexConnectSuperVisionTimeoutHandler(CsrUint16 dummy,
                                                         void *mv)
{
    ObexUtilPrivateInstType *priInst = (ObexUtilPrivateInstType*)mv;

    /* This function is called whenever the obexCmdSuperVisionTimerId fires. This
     * timer fires if the client don't get a response on a OBEX Connect Request */
    CSR_UNUSED(dummy);
    obexUtilReleaseTransportHandler(priInst, 
                                    CSR_BT_RESULT_CODE_OBEX_RESPONSE_TIMEOUT, 
                                    CSR_BT_OBEX_UTIL_BT_DISCONNECT_NORMAL_REQUEST_P_S);

    priInst->obexCmdSuperVisionTimerId = 0;
}

static void obexUtilObexDisconnectSuperVisionTimeoutHandler(CsrUint16 dummy,
                                                            void *mv)
{
    ObexUtilPrivateInstType *priInst = (ObexUtilPrivateInstType*)mv;
    /* This function is called whenever the obexCmdSuperVisionTimerId fires. This
     * timer fires if the client don't get a response on a OBEX Disonnect Request */
    CSR_UNUSED(dummy);
    obexUtilReleaseTransportHandler(priInst, 
                                    CSR_BT_RESULT_CODE_OBEX_TRANSPORT_CONNECTION_CLOSED_DIRECT, 
                                    CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
    
    
    priInst->obexCmdSuperVisionTimerId = 0;
}

static void obexUtilActivateObexCmdSuperVisionTimer(ObexUtilPrivateInstType *priInst,
                                                    CsrTime                obexCmdSuperVisionTimeout,
                                                    CsrUint8                opCode)
{
    /* This function check if the obexCmdSuperVisionTimer must be activate or not */
    if (obexCmdSuperVisionTimeout != CSR_BT_INFINITE_TIME)
    {
        /* The obexCmdSuperVisionTimeout must be activated */
        obexUtilResetObexCmdSuperVisionTimer(priInst);

        if (opCode == CSR_BT_OBEX_CONNECT_OPCODE)
        {
            priInst->obexCmdSuperVisionTimerId = CsrSchedTimerSet(obexCmdSuperVisionTimeout,
                                                                  obexUtilObexConnectSuperVisionTimeoutHandler,
                                                                  0, priInst);
        }
        else
        {
            /* opCode must be CSR_BT_OBEX_DISCONNECT_OPCODE */
            priInst->obexCmdSuperVisionTimerId = CsrSchedTimerSet(obexCmdSuperVisionTimeout,
                                                                  obexUtilObexDisconnectSuperVisionTimeoutHandler,
                                                                  0, priInst);
        }
    }
    else
    {
        /* The obexCmdSuperVisionTimer shall not be activated */
        ;
    }
}

static void obexUtilLowpowerHandler(ObexUtilPrivateInstType * priInst)
{ /* This function is called whenever a OBEX procedure is initiated. It reset
     the lpSuperVisionTimer and if necessary it make sure that the transport
     connection goes to CSR_BT_ACTIVE_MODE */
    obexUtilResetLpSuperVisionTimer(priInst);

    if (priInst->actualLinkMode     != CSR_BT_ACTIVE_MODE ||
        priInst->requestedLinkMode  == CSR_BT_SNIFF_MODE)
    {
        /* The transport connection is in a lowpower mode, Request to goto ACTIVE */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if (CSR_BT_CONN_ID_IS_L2CA(priInst->btConnId))
        {
            /* The transport connection is a l2cap connection */
            CsrBtCml2caModeChangeReqSend(priInst->btConnId, CSR_BT_ACTIVE_MODE);
        }
        else
#endif
        {
            /* The transport connection is a rfcomm connection */
            CsrBtCmRfcModeChangeReqSend(priInst->btConnId, CSR_BT_ACTIVE_MODE);
        }
        priInst->requestedLinkMode = CSR_BT_ACTIVE_MODE;
    }
    else
    { /* The transport connection is already active, just ignore */
        ;
    }
}

static void obexUtilResetAbortRequestSuperVisionTimer(ObexUtilCliInstType * cliInst)
{
    /* This function resets the abortSuperVisionTimer */
    if (cliInst->abortSuperVisionTimerId != 0)
    {
        /* The abortSuperVisionTimer is running, cancel it */
        CsrSchedTimerCancel(cliInst->abortSuperVisionTimerId, NULL, NULL);
        cliInst->abortSuperVisionTimerId  = 0;
    }
    else
    {
        /* No abortSuperVisionTimer is running */
        ;
    }
}

static void obexUtilDeallocateClientInstData(ObexUtilPrivateInstType * priInst)
{ /* Deallocate all cliInst */
    if (priInst->cliInst)
    {
        ObexUtilCliInstType  * cliInst = priInst->cliInst;

        CsrBtUtilBllFreeLinkedList(&(cliInst->sdpTag), CsrBtUtilBllPfreeWrapper);
        CsrBtUtilSdcRfcDeinit(&cliInst->rfcConnectInst);
        obexUtilResetAbortRequestSuperVisionTimer(cliInst);
        CsrPmemFree(cliInst->abortDescription);
        CsrPmemFree(cliInst->abortIncomingObexPacket);
        CsrPmemFree(cliInst);
        priInst->cliInst = NULL;
    }
}

static void obexUtilDeallocateServerInstData(ObexUtilPrivateInstType * priInst)
{ /* Deallocate all servInst */
    if (priInst->servInst)
    {
        ObexUtilServInstType * servInst = priInst->servInst;
        CsrPmemFree(servInst->serviceRecord);
        CsrPmemFree(servInst);
        priInst->servInst = NULL;
    }
}

/* Defines used to adjust the number of parameters to a function, based on the compiler settings. */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
#define OBEX_PARAM_DIGEST_CHALLENGE(_x) _x,
#else
#define OBEX_PARAM_DIGEST_CHALLENGE(_x) 
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define OBEX_PARAM_DESCRIPTION(_x) _x,
#else
#define OBEX_PARAM_DESCRIPTION(_x) 
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
#define OBEX_PARAM_APP_PARAMS(_x) ,_x
#else
#define OBEX_PARAM_APP_PARAMS(_x) 
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
#define OBEX_PARAM_USER_DEFINE(_x) ,_x
#else
#define OBEX_PARAM_USER_DEFINE(_x) 
#endif

#define obexUtilPfreeStdPointers(_digestChallenge,\
                                 _description,\
                                 _name,\
                                 _userDefinedHeader,\
                                 _appParametersHeader) \
        obexUtilPfreeStdPointersEx(OBEX_PARAM_DIGEST_CHALLENGE(_digestChallenge)\
                                   OBEX_PARAM_DESCRIPTION(_description)\
                                   _name\
                                   OBEX_PARAM_USER_DEFINE(_userDefinedHeader)\
                                   OBEX_PARAM_APP_PARAMS(_appParametersHeader))

static void obexUtilPfreeStdPointersEx(
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                     ObexUtilDigestChallengeType  * digestChallenge,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                     CsrUcs2String                * description,
#endif
                                     CsrUcs2String                * name
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                     ,CsrUint8                     * userDefinedHeader
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                                     ,CsrUint8                     * appParametersHeader
#endif
                                     )
{ /* This function deallocate the standard pointers used by this library */

    CsrPmemFree(name);

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    CsrPmemFree(description);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    CsrPmemFree(userDefinedHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
    CsrPmemFree(appParametersHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    ObexUtilPfreeDigestChallengePointer(&digestChallenge);
#endif
}

static void obexUtilDeallocatePrivateInstHeaderPointers(ObexUtilPrivateInstType *priInst,
                                                        CsrBool                 freeDigestChallenge)
{ /* This function deallocate all pointers in the private instance data */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    CsrPmemFree(priInst->description);
#endif
    CsrPmemFree(priInst->name);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    CsrPmemFree(priInst->userDefinedHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
    CsrPmemFree(priInst->appParametersHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    if (freeDigestChallenge)
    {
        ObexUtilPfreeDigestChallengePointer(&(priInst->digestChallenge));
    }
#else
    CSR_UNUSED(freeDigestChallenge);
#endif    

    CsrPmemFree(priInst->incomingObexPacket);
#if defined(CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID) || defined(CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID)
    CsrPmemFree(priInst->targetWho);
#endif
    CsrPmemFree(priInst->type);
    CsrPmemFree(priInst->body);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
    CsrPmemFree(priInst->digestResponseHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    CsrPmemFree(priInst->digestChallengeHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    CsrPmemFree(priInst->destName);
    CsrPmemFree(priInst->singleResponseModeHeader);
    CsrPmemFree(priInst->srmpHeader);
#endif
#ifdef CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
    ObexUtilPfreeDigestChallengePointer(&(priInst->clientDigestChallenge));
#endif /* CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */
}

static void obexUtilInitRxQueueDataInst(ObexUtilPrivateInstType * priInst)
{
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    priInst->rxQueue        = NULL;
    priInst->rxQueueCount   = priInst->windowSize;
    priInst->rxDataPending  = FALSE;
#else
    CSR_UNUSED(priInst);
#endif
}

static void obexUtilDeallocateRxQueueDataInst(ObexUtilPrivateInstType * priInst)
{
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    CsrUint16 eventClass;
    void      *msg;
                
    while(CsrMessageQueuePop(&priInst->rxQueue, &eventClass, &msg))
    {
        CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
        CsrPmemFree(msg);
    }
    priInst->rxQueueCount = priInst->windowSize;
#else
    CSR_UNUSED(priInst);
#endif
}

static void obexUtilInitPrivateInstHeaderPointers(ObexUtilPrivateInstType *priInst,
                                                  CsrBool                 initDigestChallenge)
{ /* This function sets all the pointers in the private instance data to NULL */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    if (initDigestChallenge)
    {
        priInst->digestChallenge        = NULL;
    }
    priInst->digestChallengeHeader      = NULL;
#else
    CSR_UNUSED(initDigestChallenge);
#endif
    priInst->incomingObexPacketLength   = 0;
    priInst->lengthOfObject             = 0;
    priInst->count                      = 0;
    priInst->incomingObexPacket         = NULL;
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
    priInst->appParametersHeader        = NULL;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    priInst->userDefinedHeader          = NULL;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
    priInst->digestResponseHeader       = NULL;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    priInst->description                = NULL;
#endif
    priInst->name                       = NULL;
#if defined(CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID) || defined(CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID)
    priInst->targetWho                  = NULL;
#endif
    priInst->type                       = NULL;
    priInst->body                       = NULL;
    priInst->bodyLength                 = 0;
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    priInst->actionId                   = CSR_BT_OBEX_UTIL_ACTION_ID_INVALID;
    priInst->permission                 = CSR_BT_OBEX_UTIL_ACTION_ALL_RESET_PERMISSION;
    priInst->singleResponseModeHeader   = NULL;
    priInst->srmpHeader                 = NULL;
    priInst->destName                   = NULL;   
#endif
#ifdef CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
    priInst->clientDigestChallenge      = NULL;
#endif /* CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */
}

static void obexUtilDeallocatePrivateInstData(ObexUtilInstType * inst)
{ /* Deallocate the private instance data used by this common OBEX util lib */
    if (inst->privateInst)
    {
        ObexUtilPrivateInstType * priInst = inst->privateInst;
        obexUtilDeallocateRxQueueDataInst(priInst);
        obexUtilDeallocateClientInstData(priInst);
        obexUtilDeallocateServerInstData(priInst);
        obexUtilResetLpSuperVisionTimer(priInst);
        obexUtilDeallocatePrivateInstHeaderPointers(priInst, TRUE);
        CsrPmemFree(priInst);
        inst->privateInst = NULL;
    }
}

static void obexUtilInitPrivateInstData(ObexUtilInstType        * inst,
                                        ObexUtilPrivateInstType * priInst,
                                        CsrBool                 srmSupported)
{ /* This function initialize the common OBEX util lib private instance data */
    obexUtilInitPrivateInstHeaderPointers(priInst, TRUE);
    obexUtilInitRxQueueDataInst(priInst);
    priInst->cliInst                    = NULL;
    priInst->servInst                   = NULL;
#if defined(CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID) || defined(CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID)
    priInst->connectionId               = CSR_BT_UNUSED_CONNECTION_ID;
#endif
    priInst->responseCode               = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
    priInst->resultCode                 = CSR_BT_RESULT_CODE_CM_SUCCESS;
    priInst->resultSupplier             = CSR_BT_SUPPLIER_CM;
    priInst->localServerCh              = CSR_BT_NO_SERVER;
    priInst->localPsm                   = L2CA_PSM_INVALID;
    priInst->btConnId                   = CSR_BT_CONN_ID_INVALID;
    priInst->profilesOwnInstData        = inst->profilesOwnInstData;
    priInst->actualLinkMode             = CSR_BT_ACTIVE_MODE;
    priInst->requestedLinkMode          = CSR_BT_ACTIVE_MODE;
    priInst->outgoingObexPacketLength   = 0;
    priInst->obexUtilOwnInstData        = inst;
    priInst->maxPeerObexPacketLength    = 0xFFFF;
    priInst->lpSuperVisionTimerId       = 0;
    priInst->lpSuperVisionTimeout       = 0;
    priInst->obexCmdSuperVisionTimerId  = 0;
    priInst->transmittingData           = FALSE;

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    priInst->srmpState                  = CSR_BT_OBEX_UTIL_SRMP_DISABLED;
       
    if (srmSupported)
    { /* The local profile support Single Response Mode */
        priInst->srmState = CSR_BT_OBEX_UTIL_SRM_DISABLED;
    }
    else
    { /* The local profile do not support Single Response Mode */
        priInst->srmState = CSR_BT_OBEX_UTIL_SRM_NOT_SUPPORTED;
    }
#else
    CSR_UNUSED(srmSupported);
#endif
}

static CsrUint16 obexUtilGetMaxObexPacketLength(ObexUtilPrivateInstType * priInst, CsrUint16 withdraw)
{ /* Calculate the maximum number of bytes the profiles must sent */
    if (priInst->maxPeerObexPacketLength > CSR_BT_MAX_OBEX_SIGNAL_LENGTH)
    { /* The peer device can received more data than the local device can send */
        if ((priInst->maxPeerObexPacketLength - withdraw) > CSR_BT_MAX_OBEX_SIGNAL_LENGTH)
        {
            return (CSR_BT_MAX_OBEX_SIGNAL_LENGTH);
        }
        else
        {
            CsrUint16 remainder = (CsrUint16)(priInst->maxPeerObexPacketLength - CSR_BT_MAX_OBEX_SIGNAL_LENGTH);
            return ((CsrUint16)(CSR_BT_MAX_OBEX_SIGNAL_LENGTH - withdraw + remainder));
        }
    }
    else
    { /* The peer device can received the same or less than the local device */
        return ((CsrUint16)(priInst->maxPeerObexPacketLength - withdraw));
    }
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static void obexUtilRxQueueAbortHandler(ObexUtilPrivateInstType * priInst)
{
    if(CSR_BT_CONN_ID_IS_L2CA(priInst->btConnId))
    {
        if (priInst->rxQueueCount == 0)
        { /* Request the CM for one more packet */
            CsrBtCmL2caDataResSend(priInst->btConnId);
        }
        obexUtilDeallocateRxQueueDataInst(priInst);  
        obexUtilInitRxQueueDataInst(priInst);
    }
    else
    { /* Running OBEX over RFCOMM, just ignore */ 
        ;
    }
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static void obexUtilRxQueueDataAckHandler(ObexUtilPrivateInstType * priInst) 
{ /* The profile is ready to received another OBEX packet */
    if(CSR_BT_CONN_ID_IS_L2CA(priInst->btConnId))
    {
        CsrUint16 eventClass;
        void      *msg;
    
        if (priInst->rxQueueCount == 0)
        { /* Request the CM for one more packet */
            CsrBtCmL2caDataResSend(priInst->btConnId);
        }

        if (priInst->rxQueueCount < priInst->windowSize)
        {
            priInst->rxQueueCount++;
        }
        
        if (CsrMessageQueuePop(&priInst->rxQueue, &eventClass, &msg))
        {
            CsrBtCmL2caDataInd *pMsg = (CsrBtCmL2caDataInd *) msg;
            obexUtilTransportDataIndFirstFragment(priInst, pMsg->length, pMsg->payload);
            CsrPmemFree(msg);
        }
        else
        {
            priInst->rxDataPending = FALSE; 
        }
    }
    else
    { /* Running OBEX over RFCOMM, just ignore */ 
        ;
    }
}
#endif
static void obexUtilRxQueueDataRequestHandler(ObexUtilPrivateInstType * priInst)
{
    priInst->rxDataPending = TRUE;
    if (priInst->rxQueueCount > 0)
    { 
        priInst->rxQueueCount--;
    }

    if (priInst->rxQueueCount > 0)
    { /* There are still space in the buffer tell CM that OBEX it able
         to receive one more data packet */
        CsrBtCmL2caDataResSend(priInst->btConnId);
    }
    else
    { /* The buffer is full, do not ask for one more. Must wait 
         until the profiles to ack the last received messages */
            ;
    }
}
#endif

static void obexUtilSendTransportPacket(ObexUtilPrivateInstType * priInst,
                                        CsrUint8            * transportPacket,
                                        CsrUint16           transportPacketLength)
{ /* Send the transport packet over RFCOMM or L2CAP */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if (CSR_BT_CONN_ID_IS_L2CA(priInst->btConnId))
    {
        /* OBEX is running over L2CAP. This usually acks whatever we
         * received. Otherwise, ack is harmless -- send data. */
        ObexUtilInstType     *inst = priInst->obexUtilOwnInstData;
        priInst->transmittingData  = TRUE;
        if( priInst->outgoingMblkPkt != NULL)
        {
            CsrBtCml2caDataReqSendExt(priInst->btConnId, 0, (CsrUint8 *) priInst->outgoingMblkPkt, inst->instId);
        }
        else
        {
            CsrBtCml2caDataReqSendExt(priInst->btConnId, transportPacketLength, transportPacket, inst->instId);
        }
        obexUtilRxQueueDataAckHandler(priInst);
    }
    else
#endif
    {
        /* OBEX is running over RFCOMM */
        CsrBtCmDataReqSend(priInst->btConnId, transportPacketLength, transportPacket);
        priInst->transmittingData = TRUE;
    }
}

static void obexUtilCompleteObexResponsePacketSend(ObexUtilPrivateInstType * priInst)
{
    /* This function check if the have send a complete OBEX response
     * packet. In the case that a complete OBEX response packet is
     * sent, the server needs to deallocate all pointers in the
     * private instance data and initialise it again. Note the reason
     * why this check is made in the obexUtilBuildTransportPacket
     * function and not in the Data confirm handler is that we cannot
     * guarantee what we received RFC_DATA_CFM message on the last
     * sent OBEX fragment before we received a new RFC_DATA_IND
     * message from the client. E.g this happens if the server is
     * running out of credits when the last fragment is send, and the
     * the peer device don't give any new credits until after a
     * RFC_DATA_IND is received */
    if (priInst->servInst)
    {
        if (priInst->currentHeaderTypeIndex  < priInst->lastHeaderTypeIndex)
        {
            /* Still some OBEX data fragment what must be send */
            ;
        }
        else
        {
            /* A complete OBEX response packet is send. */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                obexUtilDeallocatePrivateInstHeaderPointers(priInst, priInst->servInst->deleteDigestChallenge);
#else
                obexUtilDeallocatePrivateInstHeaderPointers(priInst, FALSE);
#endif
             
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                obexUtilInitPrivateInstHeaderPointers(priInst,priInst->servInst->deleteDigestChallenge);
#else
                obexUtilInitPrivateInstHeaderPointers(priInst, FALSE);
#endif

            if (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S)
            { /* As the L2cap buffer may still be full, don't Activate the 
                 lpSuperVisionTimer until the CSR_BT_CM_DATA_BUFFER_EMPTY_CFM
                 message is received */                 
                CsrBtCmDataBufferEmptyReqSend(priInst->btConnId);
            }
        }
    }
    else
    {
        /* The client is running, it will not be a problem for the
         * client as it deallocate all pointers in the private
         * instance data and initialise it again every time a corfirm
         * message is sent to the application. E.g it cannot received
         * a OBEX response message until it has sent a new OBEX
         * request message */
        ;
    }    
}

static void obexUtilBuildTransportPacket(ObexUtilPrivateInstType * priInst, CsrBool headerSplit)
{
    if (!priInst->transmittingData)
    {
        /* Build one transport packet */
        CsrUint8   index;
        CsrUint8 * transportPacket;
        CsrUint8 * transportTempPacket;
        CsrBool    next = TRUE;

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        /* find out if MBLKs can be used:
           1. Must use L2CAP connections as RFCOMM connections will need fragmentation anyway.
           2. All OBEX headers can be sent in a single OBEX packet
           3. The outgoingObexPacketLength must be larger than the bodylength. Note does only 
              happens if the Server application violates the API.
           4. The body/end-body header must be last in order to just link the body at the tail */
        if( (CSR_BT_CONN_ID_IS_L2CA(priInst->btConnId)) &&
             !headerSplit                               &&
             priInst->outgoingObexPacketLength > priInst->bodyLength &&
            ((priInst->headerTypeTable[priInst->numOfheaderTypes-1] == CSR_BT_OBEX_UTIL_BODY_HEADER)
             || (priInst->headerTypeTable[priInst->numOfheaderTypes-1] == CSR_BT_OBEX_UTIL_END_BODY_HEADER)))
        {
            CsrUint16 transportPktLength;

            /* calculate and allocate space for all headers, except the actual body data */
            transportPktLength = priInst->outgoingObexPacketLength - priInst->bodyLength;
            transportPacket = (CsrUint8 *) CsrPmemAlloc(transportPktLength);

            /* make it an MBLK */            
            priInst->outgoingMblkPkt = CsrMblkDataCreate( transportPacket,
                                                          transportPktLength,
                                                          TRUE);
        }
        else
#endif
        {
            priInst->outgoingMblkPkt = NULL;
            transportPacket = (CsrUint8 *) CsrPmemAlloc(priInst->maxTransportPacketLength);
        }
        
        transportTempPacket = transportPacket;

        if (headerSplit)
        {
            /* The OBEX headers could not be sent in a single OBEX
             * packet, must add build the mandatory PUT or GET Request
             * header. This happens seldom */
            index               = priInst->headerTypeTable[0];
            transportTempPacket = obexUtilBuildTransportPacketHandlers[index](priInst,
                                                                              transportTempPacket,
                                                                              &next);

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE_PENDING
                && priInst->srmpHeader)
            {
                /* The Client must have requested SRMP (WAIT) make
                 * sure that the SRMP header is sent again, so when
                 * all headers is send the application still have the
                 * possibility to use the SRMP header in the next GET
                 * Request packet */
                transportTempPacket = obexUtilBuildSrmpHeader(priInst,
                                                              transportTempPacket,
                                                              &next);
            }
#endif
        }
        else
        {
            /* This is the first part of an OBEX parket */
            ;
        }

        /* This while loop will keep copying data into the transport
         * packet until the transport packet in filled up (next ==
         * FALSE) or to all OBEX data is sent
         * (priInst->currentHeaderTypeIndex <
         * priInst->lastHeaderTypeIndex) */
        while (next && priInst->currentHeaderTypeIndex < priInst->lastHeaderTypeIndex)
        {
            /* Inserting data in the transportTempPacket */
            index = priInst->headerTypeTable[priInst->currentHeaderTypeIndex];

            transportTempPacket = obexUtilBuildTransportPacketHandlers[index](priInst,
                                                                              transportTempPacket, &next);

            if (next)
            {
                /* A hole OBEX header is sent, start on the next OBEX header */
                priInst->headerPayloadIndex = 0;
                priInst->currentHeaderTypeIndex++;
            }
        }

        /* Send the transport packet over L2CAP or RFCOMM */
        obexUtilSendTransportPacket(priInst, transportPacket, priInst->transportPacketIndex);
        priInst->transportPacketIndex = 0;
        priInst->headerSplit          = FALSE;
        obexUtilCompleteObexResponsePacketSend(priInst);
    }
    else
    {
        /* Wait to build the transportpacket until we are allow to send it */
        priInst->headerSplit = headerSplit;
    }
}

static void obexUtilFindOutgoingObexPacketLengthHandler(ObexUtilPrivateInstType * priInst,
                                                        CsrUint8            startIndex)
{ /* This function returns the entire length of the OBEX packet, including the
     opcode/responseCode and length bytes */
    CsrUint8  index;
    CsrUintFast16 length              = 0;
    priInst->lastHeaderTypeIndex = startIndex;

    if (startIndex == 0)
    { /* This is a new OBEX operation */
        priInst->outgoingObexPacketLength = 0;
    }
    else
    { /* The OBEX headers could not be sent in a single OBEX packet.
         Send the next OBEX packet to ensure that all header are sent.
         Note this must be an OBEX PUT or GET Request operation as these
         operations may consist of more OBEX packets, all other OBEX
         operations must always fit into one OBEX packet */
        priInst->outgoingObexPacketLength = CSR_BT_OBEX_HEADER_LENGTH;
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE_PENDING
            && priInst->srmpHeader)
        { /* The Client must have requested SRMP (WAIT) make sure that the SRMP
             header is sent again, so when all headers is send the application
             still have the possibility to use the SRMP header in the next
             GET Request packet */
            priInst->outgoingObexPacketLength = (CsrUint16)
                (priInst->outgoingObexPacketLength + CSR_BT_OBEX_UTIL_SRMP_LENGTH);
        }
#endif
    }

    for (; startIndex < priInst->numOfheaderTypes; startIndex++)
    {
        index  = priInst->headerTypeTable[startIndex];
        length = obexUtilHeaderLength(priInst,index);

        if ((length > priInst->maxPeerObexPacketLength) && priInst->cliInst)
        { /* A single header cannot fit into one OBEX packet. Note this only
             demand the client. If the server breaks the API currently we just
             respond the OBEX packet anyway and let the peer OBEX client handle
             it. The reason for this is that we cannot signal this to the local
             server profile */
            priInst->outgoingObexPacketLength = 0;
            break;
        }
        else
        { /* The header can fit into one OBEX packet */
            if ((length + priInst->outgoingObexPacketLength) <= priInst->maxPeerObexPacketLength)
            { /* This header do fit into the OBEX packet */
                priInst->outgoingObexPacketLength = (CsrUint16)(priInst->outgoingObexPacketLength + length);
                priInst->lastHeaderTypeIndex++;
            }
            else
            { /* No more headers fits into this OBEX packet */
                break;
            }
        }
    }
}

static CsrBtObexResponseCode obexUtilValidateObexPacket(ObexUtilPrivateInstType * priInst,
                                                        CsrBool             allowMultiplePackets,
                                                        CsrUint8            startIndex)
{
    /* This function will validate the OBEX packet, e.g. check if it
     * fullfill length requirement */
    priInst->firstHeaderTypeIndex   = startIndex;
    priInst->currentHeaderTypeIndex = 0;
    priInst->headerPayloadIndex     = 0;
    priInst->transportPacketIndex   = 0;

    /* Find the entire length of the OBEX packet */
    obexUtilFindOutgoingObexPacketLengthHandler(priInst, startIndex);

    if (allowMultiplePackets && priInst->outgoingObexPacketLength > 0)
    { /* This OBEX packet is allowed to be split into multiple OBEX packets,
         e.g. it must be a PUT or GET operation */
        if (priInst->lastHeaderTypeIndex == priInst->numOfheaderTypes)
        { /* Check is a body header should be sent along to */
            if ((priInst->outgoingObexPacketLength + CSR_BT_OBEX_UTIL_THRESHOLD) <
                priInst->maxPeerObexPacketLength)
            { /* Return CSR_BT_OBEX_CONTINUE_RESPONSE_CODE to indicate that a body
                 header can included in the packet */
                return (CSR_BT_OBEX_CONTINUE_RESPONSE_CODE);
            }
            else
            { /* All the headers can be in a single OBEX Put/Get packet, but
                 there are no space for sending a body header along with it */
                return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
            }
        }
        else
        { /* All the headers could not be sent in a single OBEX Put/Get packet.
             Return CSR_BT_OBEX_MULTIPLE_CHOICES_RESPONSE_CODE to indicate that the
             headers must be sent as sequence of request/responses */
            return (CSR_BT_OBEX_MULTIPLE_CHOICES_RESPONSE_CODE);
        }
    }
    else
    { /* The OBEX packet must fit into one OBEX packet. e.g it must be a
         Connect, Disconnect, setpath or abort operation */
        if (priInst->lastHeaderTypeIndex == priInst->numOfheaderTypes)
        { /* The Request fit in one OBEX packet. */
            return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        }
        else
        { /* This packet don't fit into one OBEX packet or
             outgoingObexPacketLength == 0. Return CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE
             to indicate that one of the headers is to long */
            return (CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE);
        }
    }
}

static void obexUtilUpdateHeaderTable(ObexUtilPrivateInstType *priInst,
                                      CsrUint8                obexOperation,
                                      CsrUint8                numOfHeaders,
                                      const CsrUint8          *headers)
{ /* This function build the table which keep track of which OBEX packet must
     be send and which headers that must be included */
    CsrUintFast8 i;
    CsrUint8 index               = 0;
    priInst->headerTypeTable[index] = obexOperation;
    index++;

#if defined(CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID) || defined(CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID)
    if (priInst->targetWho)
    { /* A target or a who header must be include, in the OBEX CONNECT request
         or response packet */

        if (obexOperation == CSR_BT_OBEX_UTIL_CONNECT_REQUEST_OPERATION)
        { /* A Target header must be included */
            priInst->headerTypeTable[index] = CSR_BT_OBEX_UTIL_TARGET_HEADER;
        }
        else
        { /* Must be a OBEX Connect Response, e.g a who header must be include */
            priInst->headerTypeTable[index] = CSR_BT_OBEX_UTIL_WHO_HEADER;
        }
        index++;
    }
    if (priInst->connectionId != CSR_BT_UNUSED_CONNECTION_ID)
    { /* The connectionId header must be included. */
        priInst->headerTypeTable[index] = CSR_BT_OBEX_UTIL_CONNECTIONID_HEADER;
        index++;
    }
#endif

    /* Always make space for the digest challenge header */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    priInst->headerTypeTable[index] = CSR_BT_OBEX_UTIL_AUTH_CHALLENGE_HEADER;
    index++;
#endif    
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if (priInst->singleResponseModeHeader)
    { /* The Single Response Mode header must be included */
        priInst->headerTypeTable[index] = CSR_BT_OBEX_UTIL_SINGLE_RESPONSE_MODE_HEADER;
        index++;
    }

    if (priInst->srmpHeader)
    { /* The SRMP header must be included */
        priInst->headerTypeTable[index] = CSR_BT_OBEX_UTIL_SRMP_HEADER;
        index++;
    }
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
    /* Always make space for the digest response header */
    priInst->headerTypeTable[index]     = CSR_BT_OBEX_UTIL_AUTH_RESPONSE_HEADER;
    index++;
#endif
    for (i = 0; i < numOfHeaders; i++)
    { /* Insert other headers in the order that the profile has requested */
        if (headers[i] != CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)
        {
            priInst->headerTypeTable[index] = headers[i];
            index++;
        }
    }
    priInst->numOfheaderTypes = index;
}

#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
static ObexUtilPrivateInstType * obexUtilVerifyPrivateInstData(void    *obexUtilInstData,
                                                               CsrBool role)
{ /* This function checks if the instance data used for the client part is
     valid or not */
    if (obexUtilInstData)
    { /* The obexUtilInstData is present, check if the right private instance
         is present */
        ObexUtilInstType * inst = obexUtilInstData;

        if (inst->privateInst)
        { /* The private instance data is present */
            if (role == CSR_BT_OBEX_UTIL_CLIENT_ROLE)
            { /* Check if the private client instance data is present */
                if (inst->privateInst->cliInst)
                { /* All instance data is available return private instance data*/
                    return (inst->privateInst);
                }
            }
            else
            { /* The role must be CST_BT_OBEX_UTIL_SERVER_ROLE. Check if the
                 private server instance data is present */
                if (inst->privateInst->servInst)
                { /* All instance data is available return private instance data*/
                    return (inst->privateInst);
                }
            }
        }
    }
    return (NULL);
}
#endif

static void obexUtilInitClientInstData(ObexUtilCliInstType *cliInst)
{ /* This function initialize the common OBEX client private instance data */
    cliInst->sdpTag                         = NULL;
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
    cliInst->authResultFunc                 = NULL;
#endif
    cliInst->abortDescription               = NULL;
    cliInst->abortIncomingObexPacket        = NULL;
    cliInst->obtainedServer                 = FALSE;
    cliInst->abortSuperVisionTimerId        = 0;
    cliInst->abortIncomingObexPacketLength  = 0;
}

static void obexUtilInitServerInstData(ObexUtilServInstType *servInst)
{ /* This function initialize the common OBEX server private instance data */
    servInst->serviceRecordHandle       = CMN_SDR_INVALID_SERVICE_HANDLE;
    servInst->nonConnectableCfmHandler  = NULL;
    servInst->deactivateCfmHandler      = NULL;
    servInst->putContinueIndHandler     = NULL;
#ifdef CSR_BT_INSTALL_OBEX_SRV_GET
    servInst->getContinueIndHandler     = NULL;
#endif
    servInst->rfcommMtu                 = 0;
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    servInst->deleteDigestChallenge     = TRUE;
#endif
    servInst->rfcSubProcessState        = CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S;
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    servInst->l2caSubProcessState       = CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S;
#endif
}

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION 
static CsrUint8 obexUtilValidateObexDescriptionHeader(ObexUtilPrivateInstType * priInst)
{ /* Validate if the description is used or not */
    if (priInst->description)
    { /* The description header is valid and must be used. */
        return (CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER);
    }
    else
    { /* The description header is invalid */
        return (CSR_BT_OBEX_UTIL_UNDEFINE_HEADER);
    }
}
#endif

static void obexUtilClearSingleResponseModeState(ObexUtilPrivateInstType * priInst)
{
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if (priInst->srmState != CSR_BT_OBEX_UTIL_SRM_NOT_SUPPORTED)
    {
        priInst->srmState = CSR_BT_OBEX_UTIL_SRM_DISABLED;
    }
    else
    { /* Single Response Mode is not supported by the profile */
        ;
    }
    priInst->srmpState = CSR_BT_OBEX_UTIL_SRMP_DISABLED;
#else
    CSR_UNUSED(priInst);
#endif
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static CsrUint8 * buildSingleResponseModeHeader(CsrUint8 parameterValue)
{ /* This function builds the Single Response Mode Header (0x97) */
    CsrUint8 *singleResponseModeHeader = (CsrUint8 *) CsrPmemAlloc(CSR_BT_OBEX_UTIL_SINGLE_RESPONSE_MODE_LENGTH);
    singleResponseModeHeader[0] = CSR_BT_OBEX_SINGLE_RESPONSE_MODE_HEADER;
    singleResponseModeHeader[1] = parameterValue;
    return (singleResponseModeHeader);
}

static CsrUint8 * buildSrmpHeader(CsrUint8 parameterValue)
{ /* This function builds the SRMP Header (0x98) */
    CsrUint8 *srmpHeader = (CsrUint8 *) CsrPmemAlloc(CSR_BT_OBEX_UTIL_SRMP_LENGTH);

    srmpHeader[0] = CSR_BT_OBEX_SRMP_HEADER;
    srmpHeader[1] = parameterValue;
    return (srmpHeader);
}

static CsrBool obexUtilSingleResponseModeEnabled(CsrUint8 *obexPacket)
{
    CsrUint8 srmValue;

    if (obexPacket && CsrBtObexGetSingleResponseModeParameter(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &srmValue))
    {
        if (srmValue == CSR_BT_OBEX_UTIL_SRM_ENABLED)
        {
            return (TRUE);
        }
    }
    return (FALSE);
}

static CsrBool obexUtilSrmpWait(CsrUint8 *obexPacket)
{
    CsrUint8 srmpValue;

    if (CsrBtObexGetSrmpParameter(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &srmpValue))
    {
        if (srmpValue == CSR_BT_OBEX_SRMP_WAIT)
        {
            return (TRUE);
        }
    }
    return (FALSE);
}

static void obexUtilEnableSingleResponseModeState(ObexUtilPrivateInstType *priInst)
{
    /* Single Response Mode is now enable */
    priInst->srmpState = CSR_BT_OBEX_UTIL_SRMP_DISABLED;
    priInst->srmState  = CSR_BT_OBEX_UTIL_SRM_ACTIVE;
}

static void obexUtilSrmIdleSrmpStateHandler(ObexUtilPrivateInstType *priInst,
                                            CsrBool                 srmpSupported)
{
    if (srmpSupported)
    { /* SRMP wait is requested */
        if (priInst->srmpState == CSR_BT_OBEX_UTIL_SRMP_WAIT_CLIENT_REQUEST)
        { /* Both the Client and the Server has requested SRMP. This must be an
             OBEX GET operation, as the Client only is allowed used SRMP during
             a GET operation */
            priInst->srmpState = CSR_BT_OBEX_UTIL_SRMP_WAIT_CLIENT_SERVER_REQUEST;
        }
        else
        { /* Only the Server has requested SRMP */
            priInst->srmpState = CSR_BT_OBEX_UTIL_SRMP_WAIT_SERVER_REQUEST;
        }
        priInst->srmState = CSR_BT_OBEX_UTIL_SRM_ACTIVE_PENDING;
    }
    else
    { /* The SRMP header must be included in the first OBEX Request or Response
         packet, and it cannot be enable later. If it does it must be ignore */
        if (priInst->srmpState == CSR_BT_OBEX_UTIL_SRMP_WAIT_CLIENT_REQUEST)
        { /* The Client had included SRMP header in the first GET Request Packet
             E.g. the Client must have the possibility to use the SRMP header in
             the next OBEX GET Request as well */
            priInst->srmState = CSR_BT_OBEX_UTIL_SRM_ACTIVE_PENDING;
        }
        else
        { /* Set Single Response Mode 100% enable, because it is not legal to
             enable SRMP later. */
            obexUtilEnableSingleResponseModeState(priInst);
        }
    }
}

static CsrBool obexUtilSrmpHeaderValueHandler(ObexUtilPrivateInstType *priInst,
                                              CsrUint8                srmpValue,
                                              CsrBool                 localInitiated,
                                              CsrUint8                *obexPacket)
{
    if (localInitiated)
    { /* Check if the local device still want to use the SRMP (Wait) header. */
        if (srmpValue == CSR_BT_OBEX_SRMP_WAIT)
        { /* The local device want to use the SRMP (Wait) header. Build it */
            priInst->srmpHeader = buildSrmpHeader(srmpValue);
        }
        else
        { /* The local device don't want to use the SRMP (Wait) header */
            return (FALSE);
        }
    }
    else
    { /* Check if the peer device has included an SRMP (Wait) header or not */
        if (obexUtilSrmpWait(obexPacket))
        { /* The peer device has included an SRMP (Wait) header */
            srmpValue = CSR_BT_OBEX_SRMP_WAIT;
        }
        else
        { /* The peer device has not included an SRMP (Wait) header. */
            srmpValue = CSR_BT_OBEX_SRMP_INVALID;
            return (FALSE);
        }
    }
    return (TRUE);
}

static void obexUtilServerSrmpHeaderHandler(ObexUtilPrivateInstType *priInst)
{ /* The SRMP header is not required by the Server anymore */
    switch (priInst->srmpState)
    {
        case CSR_BT_OBEX_UTIL_SRMP_WAIT_SERVER_REQUEST:
            { /* Only the Server was using SRMP. Set Single Response Mode 100%
                 enable, as it is not legal to enable SRMP later */
                obexUtilEnableSingleResponseModeState(priInst);
                break;
            }
        case CSR_BT_OBEX_UTIL_SRMP_WAIT_CLIENT_SERVER_REQUEST:
            { /* The Server don't want to used the SRMP header anymore,
                 but the Client must still have the possibility to use
                 the SRMP header in the next OBEX GET Request */
                priInst->srmpState = CSR_BT_OBEX_UTIL_SRMP_WAIT_CLIENT_REQUEST;
                break;
            }
        default:
            { /* Just ignore as the Server is not allowed to use the SRMP
                 header. */
                break;
            }
    }
}

static void obexUtilClientSrmpHeaderHandler(ObexUtilPrivateInstType *priInst)
{ /* The SRMP header is not required by the Client anymore */
    switch (priInst->srmpState)
    {
        case CSR_BT_OBEX_UTIL_SRMP_WAIT_CLIENT_REQUEST:
            { /* Only the Client was using SRMP. Set Single Response Mode 100%
                 enable, as it is not legal to enable SRMP later */
                obexUtilEnableSingleResponseModeState(priInst);
                break;
            }
        case CSR_BT_OBEX_UTIL_SRMP_WAIT_CLIENT_SERVER_REQUEST:
            { /* The Client don't want to used the SRMP header anymore, but the
                 Server must still have the possibility to use the SRMP header in
                 the next OBEX GET Response Packet */
                priInst->srmpState = CSR_BT_OBEX_UTIL_SRMP_WAIT_SERVER_REQUEST;
                break;
            }
        default:
            { /* Just ignore as the Client is not allowed to use the SRMP
                 header. */
                break;
            }
    }
}
#endif

#ifndef CSR_BT_INSTALL_OBEX_GOEP_20
#define obexUtilSrmpHeaderHandler(priInst, srmpValue, localInitiated, localRole, obexPacket) 
#else
static void obexUtilSrmpHeaderHandler(ObexUtilPrivateInstType *priInst,
                                      CsrUint8                srmpValue,
                                      CsrBool                 localInitiated,
                                      CsrBool                 localRole,
                                      CsrUint8                *obexPacket)
{
    if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE_PENDING)
    { /* The SRMP header were include in the last OBEX Request or Response
         packet. Check if it still required */
        if (obexUtilSrmpHeaderValueHandler(priInst, srmpValue, localInitiated, obexPacket))
        { /* The SRMP header must still be used, keep the srmpState */
            ;
        }
        else
        { /* The SRMP header is not required anymore */
            if (localRole == CSR_BT_OBEX_UTIL_SERVER_ROLE)
            { /* The Server don't want to used the SRMP header anymore */
                obexUtilServerSrmpHeaderHandler(priInst);
            }
            else
            { /* Must be CSR_BT_OBEX_UTIL_CLIENT_ROLE. E.g. the Client don't
                 want to used the SRMP header anymore */
                obexUtilClientSrmpHeaderHandler(priInst);
            }
        }
    }
    else
    { /* Just ignore, as the SRMP header must not be use anymore */
        ;
    }
}
#endif

static void obexUtilCliValidateSingleResponseModeHeader(ObexUtilPrivateInstType * priInst)
{
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_IDLE)
    { /* Check if Single Response Mode is accepted by the server */
        if (obexUtilSingleResponseModeEnabled(priInst->incomingObexPacket))
        { /* Single Response Mode is accepted by the server. Check is SRMP
             shall be used or not */
            obexUtilSrmIdleSrmpStateHandler(priInst,
                                            obexUtilSrmpWait(priInst->incomingObexPacket));
        }
        else
        { /* Single Response Mode is rejected by the server */
            obexUtilClearSingleResponseModeState(priInst);
        }
    }    
    else
    { /* Must check if SRMP is being used or not */
        obexUtilSrmpHeaderHandler(priInst,
                                  CSR_BT_OBEX_SRMP_INVALID,
                                  FALSE,
                                  CSR_BT_OBEX_UTIL_SERVER_ROLE,
                                  priInst->incomingObexPacket);
    }
#else
    CSR_UNUSED(priInst);
#endif
}

static void obexUtilObexSingleResponseModeResponseHandler(ObexUtilPrivateInstType *priInst)
{
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if ((priInst->responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) != CSR_BT_OBEX_CONTINUE_RESPONSE_CODE &&
        priInst->srmState != CSR_BT_OBEX_UTIL_SRM_REJECTED)
    { /* The OBEX operation is finish */
        obexUtilClearSingleResponseModeState(priInst);
    }
#else
    CSR_UNUSED(priInst);
#endif
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static CsrBool obexUtilSingleResponseModeSupported(ObexUtilPrivateInstType * priInst)
{
    if (CSR_BT_CONN_ID_IS_L2CA(priInst->btConnId)
        && priInst->srmState != CSR_BT_OBEX_UTIL_SRM_NOT_SUPPORTED)
    {
        /* OBEX using l2cap as transport and the profile support
         * Single Response Mode */
        return (TRUE);
    }
    else
    {
        /* OBEX is either using RFCOMM as transport or the profile
         * don't support Single Response Mode */
        return (FALSE);
    }
}

static void obexUtilCliSingleResponseModeInitiator(ObexUtilPrivateInstType *priInst,
                                                   CsrUint8                srmpValue)
{
    if (obexUtilSingleResponseModeSupported(priInst))
    { /* Single Response Mode is supported */
        priInst->srmState                   = CSR_BT_OBEX_UTIL_SRM_IDLE;
        priInst->singleResponseModeHeader   = buildSingleResponseModeHeader(CSR_BT_OBEX_UTIL_SRM_ENABLED);
        if (srmpValue == CSR_BT_OBEX_SRMP_WAIT)
        {
            priInst->srmpState  = CSR_BT_OBEX_UTIL_SRMP_WAIT_CLIENT_REQUEST;
            priInst->srmpHeader = buildSrmpHeader(srmpValue);
        }
        else
        {
            priInst->srmpState = CSR_BT_OBEX_UTIL_SRMP_IDLE;
        }
    }
    else
    {
        obexUtilClearSingleResponseModeState(priInst);
    }
}

static CsrBool obexUtilServSingleResponseModeAcceptor(ObexUtilPrivateInstType *priInst,
                                                      CsrUint8                opCode,
                                                      CsrUint8              *obexPacket)
{
    if (obexUtilSingleResponseModeSupported(priInst))
    {
        /* Single Response Mode is supported */
        if (priInst->srmState           == CSR_BT_OBEX_UTIL_SRM_DISABLED &&
            priInst->preProcessState    == CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S)
        {
            /* Check if an Single Response Mode header is present */
            if (obexUtilSingleResponseModeEnabled(obexPacket))
            {
                /* The Client has request for Single Response
                 * Mode. Note that even if the Client is not using the
                 * SRMP (Wait) header, the local server must still
                 * have the possibility to use the SRMP header in the
                 * PUT or GET Response packet */
                priInst->srmState  = CSR_BT_OBEX_UTIL_SRM_IDLE;
                if (opCode == CSR_BT_OBEX_GET_OPCODE && obexUtilSrmpWait(obexPacket))
                {
                    /* The Client is using the SRMP (Wait) header. */
                    priInst->srmpState  = CSR_BT_OBEX_UTIL_SRMP_WAIT_CLIENT_REQUEST;
                }
                else
                {
                    /* The Client is not using or is not allowed to
                     * use the SRMP (Wait) header. However the local
                     * server must still have the possibility to use
                     * the SRMP header in the following PUT or GET
                     * Response packet */
                    priInst->srmpState  = CSR_BT_OBEX_UTIL_SRMP_IDLE;
                }
            }
        }
        else if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE_PENDING)
        {
            /* Check if the peer Client is using SRMP or not */
            obexUtilSrmpHeaderHandler(priInst, CSR_BT_OBEX_SRMP_INVALID, FALSE, CSR_BT_OBEX_UTIL_CLIENT_ROLE, obexPacket);
        }
        else if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE)
        {
            /* Single response Mode is enabled */
            return (TRUE);
        }
    }
    return (FALSE);
}

static CsrBool obexUtilServValidateObexActionRequestPacket(ObexUtilPrivateInstType *priInst,
                                                           CsrUint8                *actionId,
                                                           CsrUint32               *permissions,
                                                           CsrUint16               *nameOffset,
                                                           CsrUint16               *destNameOffset)
{
    /* This function validate the the received OBEX Action Request
     * packet is correct. E.g a mandate that the Name header (0x01)
     * and the Action Id header (0x94) is included. If the Action
     * identifier is 0x00 (Copy Object) or 0x01 (Move/Rename Object)
     * the destName header (0x15) must be present. If the Action
     * identifier is 0x02 (Set Object Permissions) the permissions
     * Header (0xD6) must be present. */
    if (CsrBtObexGetNameOffset(CSR_BT_OBEX_REQUEST, priInst->incomingObexPacket, nameOffset))
    { /* The Name header is present */
        if (CsrBtObexGetActionIdParameter(CSR_BT_OBEX_REQUEST, priInst->incomingObexPacket, actionId))
        { /* The Action Id header is present */
            switch (*actionId)
            {
                case CSR_BT_OBEX_UTIL_ACTION_ID_COPY_OBJECT:
                case CSR_BT_OBEX_UTIL_ACTION_ID_MOVE_RENAME_OBJECT:
                    {
                        if (CsrBtObexGetDestNameOffset(CSR_BT_OBEX_REQUEST, priInst->incomingObexPacket, destNameOffset))
                        {
                            *permissions = CSR_BT_OBEX_UTIL_ACTION_ALL_RESET_PERMISSION;
                            return(TRUE);
                        }
                        return (FALSE);
                    }
                case CSR_BT_OBEX_UTIL_ACTION_ID_SET_OBJECT_PERMISSIONS:
                    {
                        if (CsrBtObexGetPermissionsParameter(CSR_BT_OBEX_REQUEST, priInst->incomingObexPacket, permissions))
                        {
                            *destNameOffset = 0;
                            return(TRUE);
                        }
                        return (FALSE);
                    }
            }
        }
    }
    return (FALSE);
}
#endif

static CsrBool obexUtilObexDisconnectRequestHandler(ObexUtilPrivateInstType *priInst,
                                                    CsrBool                 normalDisconnect)
{
    switch (priInst->processState)
    {
        case CSR_BT_OBEX_UTIL_BT_RFC_CONNECTING_P_S:
            { /* A rfcomm connection is being establish. Call CsrBtUtilRfcConCancel to
                 cancel this procedure. Note that when this procedure is finished
                 a notification is given through the
                 obexUtilCmRfcConnectResultHandler function */
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECT_REQUEST_P_S);
                CsrBtUtilRfcConCancel(priInst, priInst->cliInst->rfcConnectInst);
                return (FALSE);
            }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        case CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTING_P_S:
            {
                /* A l2cap connection is being establish. Call
                 * CsrBtCml2caCancelConnectReqSend to cancel this
                 * procedure */
                ObexUtilInstType * inst = priInst->obexUtilOwnInstData;

                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECT_REQUEST_P_S);
                CsrBtCml2caCancelConnectReqSend(inst->phandle, priInst->deviceAddr, priInst->localPsm);
                return (FALSE);
            }
        case CSR_BT_OBEX_UTIL_BT_L2CA_REGISTER_PSM_P_S:
            {
                /* Either is the SDP seach being cancelled, or a PSM
                 * is being registered. Set the processState to
                 * CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECT_REQUEST_P_S and
                 * wait for either the function
                 * obexUtilCmRfcConnectResultHandler is called or for
                 * the CSR_BT_CM_L2CA_REGISTER_CFM message. */
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECT_REQUEST_P_S);
                return (FALSE);
            }
#endif
        case CSR_BT_OBEX_UTIL_OBEX_CONNECT_REQUEST_P_S:
            { /* The transport connection is estalished and the OBEX connection
                 is being established. Release the transport connection */
                obexUtilReleaseTransportHandler(priInst, 
                                                CSR_BT_RESULT_CODE_OBEX_CONNECT_CANCELLED, 
                                                CSR_BT_OBEX_UTIL_BT_DISCONNECT_NORMAL_REQUEST_P_S);
                return (FALSE);
            }
        case CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S:
            { /* The OBEX connection is established, and it is idle, e.g. an OBEX
                 Disconnect Request packet is allowed to be send */
                if (!normalDisconnect)
                { /* The transport connection must be release without sending an
                     OBEX Disconnect Request packet */
                    obexUtilReleaseTransportHandler(priInst, 
                                                    CSR_BT_RESULT_CODE_OBEX_TRANSPORT_CONNECTION_CLOSED_DIRECT, 
                                                    CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
                    return (FALSE);
                }
                else
                { /* An OBEX Disconnect Request packet must be send before the
                     transport is released */
                    CsrUint8 headerPriorityTable = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER;
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                     headerPriorityTable = obexUtilValidateObexDescriptionHeader(priInst);
#endif
                    /* Update the table which defines which OBEX headers must be
                       included in the OBEX Disconnect packet */
                    obexUtilUpdateHeaderTable(priInst, CSR_BT_OBEX_UTIL_DISCONNECT_REQUEST_OPERATION, 1, &headerPriorityTable);

                    /* Validate the OBEX Disconnect Request packet and sets the
                       instance data regarding building and sending the OBEX packet
                       correct. */
                    if (obexUtilValidateObexPacket(priInst, FALSE, 0) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                    { /* The OBEX Disconnect Request packet fit in one OBEX packet */
                        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_DISCONNECT_REQUEST_P_S);
                        return (TRUE);
                    }
                    else
                    { /* The OBEX Disconnect Request packet cannot fit into one OBEX
                         packet. Release the transport connection */
                        obexUtilReleaseTransportHandler(priInst, 
                                                        CSR_BT_RESULT_CODE_OBEX_TRANSPORT_CONNECTION_CLOSED_DIRECT, 
                                                        CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
                        return (FALSE);
                    }
                }
            }
        default:
            { /* An OBEX procedure is in progress */
                if (priInst->processState != CSR_BT_OBEX_UTIL_BT_DISCONNECT_NORMAL_REQUEST_P_S)
                { /* Release the transport connection direct */
                    obexUtilReleaseTransportHandler(priInst, 
                                                    CSR_BT_RESULT_CODE_OBEX_TRANSPORT_CONNECTION_CLOSED_DIRECT, 
                                                    CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
                }
                else
                { /* The transport connection is being released, just ignore */
                    ;
                }
                return (FALSE);
            }
    }
}

static CsrUint8 * obexUtilGetObexPacketPointer(ObexUtilPrivateInstType *priInst,
                                               CsrUint16               *obexPacketLength)
{
    CsrUint8           *obexPacket  = priInst->incomingObexPacket;
    *obexPacketLength                 = priInst->incomingObexPacketLength;
    priInst->incomingObexPacket       = NULL;
    priInst->incomingObexPacketLength = 0;
    return (obexPacket);
}

static void obexUtilCommonObexRequestOperationResultHandler(ObexUtilPrivateInstType * priInst)
{
    /* The OBEX operation is finished */
    CsrUint16           obexPacketLength;
    ObexUtilCliInstType * cliInst = priInst->cliInst;
    CsrUint8          *obexPacket = obexUtilGetObexPacketPointer(priInst, &obexPacketLength);
    priInst->obexOperationIndex   = CSR_BT_OBEX_UTIL_IDLE_RESULT_HANDLER;
    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S);
    obexUtilClearSingleResponseModeState(priInst);
    obexUtilDeallocatePrivateInstHeaderPointers(priInst, TRUE);
    obexUtilInitPrivateInstHeaderPointers(priInst, TRUE);
    obexUtilActivateLpSuperVisionTimer(priInst);
    
    cliInst->commonResultFunc(priInst->profilesOwnInstData,
                              priInst->responseCode,
                              obexPacket,
                              obexPacketLength);
}

static void obexUtilAbortRequestFinalResultHandler(ObexUtilPrivateInstType  *priInst,
                                                   CsrBtObexResponseCode    responseCode)
{
    if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        /* A successful OBEX response message is received after a OBEX
         * Abort Request is sent */
        if (priInst->incomingObexPacket &&
            priInst->incomingObexPacketIndex < priInst->incomingObexPacketLength)
        {
            /* Only part of the OBEX response packet is received when
             * the abortSuperVisionTimer fired. E.g it has taken more
             * than CSR_BT_OBEX_UTIL_SECOND_ABORT_DELAY micro seconds
             * to get a fully OBEX response message from the server
             * after we already have receive one OBEX response message
             * after the OBEX Abort Request were sent.  Release the
             * transport connection */
            obexUtilReleaseTransportHandler(priInst, 
                                            CSR_BT_RESULT_CODE_OBEX_INTERNAL_ERROR, 
                                            CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
        }
        else
        {
            /* The Obex Abort Request Procedure is finish */
            ObexUtilCliInstType * cliInst  = priInst->cliInst;

            priInst->responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

            if (!priInst->incomingObexPacket)
            {
                /* Only one successful OBEX response message is given
                 * on the OBEX Abort Request message where we don't
                 * known if we shall expect one or two. E.g the
                 * abortSuperVisionTimer has fired
                 * CSR_BT_OBEX_UTIL_SECOND_ABORT_DELAY micro seconds
                 * after the first successful OBEX response message is
                 * received. We expect that the received OBEX response
                 * message belongs to the OBEX Abort Request */
                priInst->incomingObexPacket        = cliInst->abortIncomingObexPacket;
                priInst->incomingObexPacketLength  = cliInst->abortIncomingObexPacketLength;
            }
            else
            {
                /* Two successful OBEX response messages is given
                 * after the OBEX Abort Request message is sent. The
                 * OBEX response message we received last must belong
                 * to the OBEX Abort Request. Pfree the first received
                 * OBEX response message to prevent a memory leak */
                CsrPmemFree(cliInst->abortIncomingObexPacket);
            }
            cliInst->abortIncomingObexPacket        = NULL;
            cliInst->abortIncomingObexPacketLength  = 0;
            obexUtilCommonObexRequestOperationResultHandler(priInst);
        }
    }
    else
    {
        /* The response to the OBEX Abort Request is different from
         * CSR_BT_OBEX_SUCCESS_RESPONSE_CODE. Release the transport
         * connection */
        obexUtilReleaseTransportHandler(priInst, 
                                        CSR_BT_RESULT_CODE_OBEX_INVALID_OBEX_ABORT_RESPONSE_CODE, 
                                        CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
    }
}

static void obexUtilAbortRequestSuperVisionTimeoutHandler(CsrUint16 responseCode,
                                                          void *mv)
{
    ObexUtilPrivateInstType *priInst = (ObexUtilPrivateInstType*)mv;
    /* This function is called whenever the abortSuperVisionTimer fires */
    priInst->cliInst->abortSuperVisionTimerId = 0;
    obexUtilAbortRequestFinalResultHandler(priInst, (CsrBtObexResponseCode)(responseCode));
}

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
#define obexUtilObexAbortRequestHandler(priInst, desc, num, flush) \
        obexUtilObexAbortRequestHandlerExt(priInst, desc, num, flush)
#else
#define obexUtilObexAbortRequestHandler(priInst, desc, num, flush) \
        obexUtilObexAbortRequestHandlerExt(priInst, num, flush)
#endif

static void obexUtilObexAbortRequestHandlerExt(ObexUtilPrivateInstType * priInst,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                            CsrUcs2String           *description,
#endif
                                            CsrUint8                numOfRsp,
                                            CsrBool                 flushL2caBuff)
{
    ObexUtilCliInstType * cliInst   = priInst->cliInst;
    cliInst->abortNumOfRsp          = numOfRsp;
    priInst->obexOperationIndex     = CSR_BT_OBEX_UTIL_ABORT_REQUEST_RESULT_HANDLER;

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    /* An OBEX Abort Request is received from the profile 
       Flush the intern buffer as there are no need to send 
       these packet to the profile. Note intern flowcontrol 
       is not needed anymore */
    obexUtilRxQueueAbortHandler(priInst);        
#endif

    if (priInst->transmittingData)
    {
        /* Is in the middle of sending the OBEX Request packet. The
         * OBEX ABORT Request packet must first be sent after this
         * ongoing OBEX Request packet is sent */
        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_ABORT_REQUEST_PENDING_P_S);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        cliInst->abortDescription   = description;
#endif
    }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    else if (flushL2caBuff && CSR_BT_CONN_ID_IS_L2CA(priInst->btConnId))
    { /* OBEX is running over L2CAP and the l2cap buffer must be flush
         before the OBEX ABORT REQUEST message is sent */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        cliInst->abortDescription   = description;
#endif
        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_ABORT_REQUEST_PENDING_P_S);
        CsrBtCmL2caDataAbortReqSend(priInst->btConnId);
    }
#endif
    else
    {
        /* Either: The complete OBEX Request packet has already been
         * sent and we are waiting for or is receiving the OBEX
         * response packet. Or: No OBEX packet is currently being
         * sent, we are waiting for a reply from the profile, e.g. a
         * ObexUtilPutContinueRequest or a
         * ObexUtilGetContinueRequest. */
        CsrUint8 headerPriorityTable = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER;

        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_ABORT_REQUEST_P_S);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        priInst->description  = description;
        headerPriorityTable   = obexUtilValidateObexDescriptionHeader(priInst);
#endif
        obexUtilResetAbortRequestSuperVisionTimer(cliInst);
        cliInst->abortSuperVisionTimerId = CsrSchedTimerSet(CSR_BT_OBEX_UTIL_FIRST_ABORT_DELAY,
                                                            obexUtilAbortRequestSuperVisionTimeoutHandler,
                                                            (CsrUint16)CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE,
                                                            priInst);

        /* Update the table which defines which OBEX headers must be included in
           the OBEX Abort Request packet */
        obexUtilUpdateHeaderTable(priInst,
                                  CSR_BT_OBEX_UTIL_ABORT_REQUEST_OPERATION,
                                  1,
                                  &headerPriorityTable);

        if (obexUtilValidateObexPacket(priInst, FALSE, 0) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
        { /* The OBEX Abort Request do fit into one OBEX packet */
            ;
        }
        else
        { /* The OBEX Abort Request do not fit into one OBEX packet
             because the description header is to large, remove it */
            headerPriorityTable = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER;
            obexUtilUpdateHeaderTable(priInst,
                                      CSR_BT_OBEX_UTIL_ABORT_REQUEST_OPERATION,
                                      1,
                                      &headerPriorityTable);

            obexUtilValidateObexPacket(priInst, FALSE, 0);
        }

        /* Building and sending the OBEX Abort Request packet */
        obexUtilBuildTransportPacket(priInst, FALSE);
    }
}

static CsrBool obexUtilObexPutRequestHandler(ObexUtilPrivateInstType  * priInst,
                                             CsrBtObexResponseCode    responseCode,
                                             CsrBool                   headerSplit)
{
    if (responseCode == CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE)
    {
        /* One the the headers is to large */
        obexUtilClearSingleResponseModeState(priInst);
        return (FALSE);
    }
    else
    {
        if (responseCode == CSR_BT_OBEX_MULTIPLE_CHOICES_RESPONSE_CODE)
        {
            /* All the headers cannot be sent in the first OBEX Put
             * packet.  Start building the first OBEX Put Request
             * packet */
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_SPLIT_P_S);
            obexUtilBuildTransportPacket(priInst, headerSplit);
        }
        else
        {
            /* The responseCode must be
             * CSR_BT_OBEX_CONTINUE_RESPONSE_CODE or
             * CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, indicating that all
             * the headers can be include in the first OBEX Put
             * Request packet */
            ObexUtilCliInstType * cliInst = priInst->cliInst;

            if (cliInst->putContinueFunc)
            {
                /* The application has a body header which later must
                 * be included. Check if some of the body header can
                 * be include in the first OBEX packet */
                if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE && !headerSplit)
                {
                    /* A body header can be incluced. Set the processState to
                     * CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_IDLE_PENDING_P_S which indicates
                     * that the body header must be include in the first OBEX
                     * Put Request packet. If headerSplit is TRUE then all the
                     * headers could not be sent in the first packet */
                    CsrUint16 withDraw = (CsrUint16)(priInst->outgoingObexPacketLength +
                                                     CSR_BT_OBEX_HEADER_LENGTH                      +
                                                     CSR_BT_OBEX_UTIL_MAX_DIGEST_RESPONSE_LENGTH);

                    CsrUint16 length   = obexUtilGetMaxObexPacketLength(priInst, withDraw);

                    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_IDLE_PENDING_P_S);
                    cliInst->putContinueFunc(priInst->profilesOwnInstData, length);
                }
                else
                { /* A body header cannot be include */
                    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_P_S);
                    obexUtilBuildTransportPacket(priInst, headerSplit);
                }
            }
            else
            { /* The application don't wish to include a body header to the
                 OBEX Put Request packet. As this is the first and last packet
                 the current OBEX operation must be change from
                 CSR_BT_OBEX_UTIL_PUT_REQUEST_OPERATION to
                 CSR_BT_OBEX_UTIL_PUT_FINAL_REQUEST_OPERATION. Start building and
                 sent the OBEX Put Request packet. */
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_P_S);
                priInst->headerTypeTable[0] = CSR_BT_OBEX_UTIL_PUT_FINAL_REQUEST_OPERATION;
                obexUtilBuildTransportPacket(priInst, headerSplit);
            }
        }
        return (TRUE);
    }
}

#ifdef CSR_BT_INSTALL_OBEX_CLI_GET
static CsrBool  obexUtilObexGetRequestHandler(ObexUtilPrivateInstType * priInst,
                                              CsrBtObexResponseCode   responseCode,
                                              CsrBool                 headerSplit)
{
    switch (responseCode)
    {
        case CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE:
            { /* One the the headers is to large, call getResultHandler with
                 the responseCode CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE */
                obexUtilClearSingleResponseModeState(priInst);
                return (FALSE);
            }
        case CSR_BT_OBEX_MULTIPLE_CHOICES_RESPONSE_CODE:
            { /* All the headers cannot be sent in the OBEX Get packet.
                 Start building it */
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_SPLIT_P_S);
                obexUtilBuildTransportPacket(priInst, headerSplit);
                return (TRUE);
            }
        default:
            { /* The responseCode must be either CSR_BT_OBEX_CONTINUE_RESPONSE_CODE
                 or CSR_BT_OBEX_SUCCESS_RESPONSE_CODE. This means that all the
                 headers (but not body) can be included in the first OBEX Get
                 request packet. */
                ObexUtilCliInstType * cliInst = priInst->cliInst;

                if (cliInst->putContinueFunc)
                { /* The application a body header which must be
                     included in the OBEX GET Request packet. Check if some of the
                     body header can be include in the first OBEX packet */
                    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE && !headerSplit)
                    { /* A body header can be incluced. Set the processState to
                         CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_IDLE_PENDING_P_S which indicates
                         that the body header must be include in the first OBEX
                         Get Request packet. If headerSplit is TRUE then all the
                         headers could not be sent in the first packet */
                        CsrUint16 withDraw = (CsrUint16)(priInst->outgoingObexPacketLength +
                                                         CSR_BT_OBEX_HEADER_LENGTH         +
                                                         CSR_BT_OBEX_UTIL_MAX_DIGEST_RESPONSE_LENGTH);

                        CsrUint16 length   = obexUtilGetMaxObexPacketLength(priInst, withDraw);

                        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_IDLE_PENDING_P_S);
                        cliInst->putContinueFunc(priInst->profilesOwnInstData, length);
                    }
                    else
                    { /* A body header cannot be include */
                        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_P_S);
                        priInst->headerTypeTable[0] = CSR_BT_OBEX_UTIL_GET_REQUEST_OPERATION;
                        obexUtilBuildTransportPacket(priInst, headerSplit);
                    }
                }
                else
                {/* Change CSR_BT_OBEX_UTIL_GET_REQUEST_OPERATION to
                    CSR_BT_OBEX_UTIL_GET_FINAL_REQUEST_OPERATION.
                    Start building and sent the OBEX Get Request packet */
                    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_P_S);
                    priInst->headerTypeTable[0] = CSR_BT_OBEX_UTIL_GET_FINAL_REQUEST_OPERATION;
                    obexUtilBuildTransportPacket(priInst, headerSplit);
                }
                return (TRUE);
            }
    }
}
#endif

static CsrBool obexUtilValidateMaximumObexPacketLength (ObexUtilPrivateInstType * priInst)
{
    priInst->maxPeerObexPacketLength = CsrBtObexGetMaxPacketLength(priInst->incomingObexPacket);

    if (priInst->maxPeerObexPacketLength > priInst->maxLocalObexPacketLength)
    { /* Make sure that the maxPeerObexPacketLength never is larger than 
         maxLocalObexPacketLength                                               */
        priInst->maxPeerObexPacketLength = priInst->maxLocalObexPacketLength;
    }


    if (priInst->maxPeerObexPacketLength < CSR_BT_MIN_OBEX_PACKET_LENGTH)
    { /* The peer device do not follow the OBEX specification as the minimum
         size of the OBEX maximum packet length allowed for negotiation is
         255 bytes */
        return (FALSE);
    }
    else
    { /* Return TRUE to indicate the maximum OBEX packet length is valid */
        return (TRUE);
    }
}

#if defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE) || defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE)
static CsrBtResultCode obexUtilValidateDigestResponse(ObexUtilPrivateInstType *priInst,
                                                      CsrBtObexSession        session)
{ /* This function is the local client/server has authentication the peer
     device. If it has it verifies that the password is correct. */
    if (priInst->digestChallenge && session != CSR_BT_OBEX_ABORT_OPCODE)
    { /* The local client/server  has authenticate the peer device. Validate
         that the password is correct */
        CsrUint8 digest[16], userId[20], idLength;

        if(CsrBtObexCopyAuthResponse(session, priInst->incomingObexPacket, digest, userId, &idLength))
        { /* A Digest Response Header is included in the OBEX response
             packet */
            ObexUtilDigestChallengeType * digestChallenge = priInst->digestChallenge;

            if (CsrBtDigestValidate(digestChallenge->nonce, digestChallenge->password,
                                    digestChallenge->passwordLength, digest))
            {
                if(digestChallenge->options)
                { /* The userId must be included in the authenticate
                     response */
                    CsrUint32 userIdLength = CsrStrLen((char *)digestChallenge->userId);
                    if( userIdLength == idLength )
                    { /* The length of the userId is correct */
                        if(!CsrMemCmp(digestChallenge->userId, userId, userIdLength) )
                        { /* The userId is correct */
                            return (CSR_BT_RESULT_CODE_OBEX_SUCCESS);
                        }
                        else
                        { /* An invalid userId is returned */
                            return (CSR_BT_RESULT_CODE_OBEX_USERID_INVALID);
                        }
                    }
                    else
                    { /* An invalid userId is returned */
                        return (CSR_BT_RESULT_CODE_OBEX_USERID_INVALID);
                    }
                }
                else
                { /* The password is correct */
                    return (CSR_BT_RESULT_CODE_OBEX_SUCCESS);
                }
            }
            else
            { /* An invalid password is returned */
                return (CSR_BT_RESULT_CODE_OBEX_PASSWORD_INVALID);
            }
        }
        else
        { /* A Digest Response Header is not included in the OBEX packet */
            return (CSR_BT_RESULT_CODE_OBEX_DIGEST_RESPONSE_HEADER_MISSING);
        }
    }
    else
    { /* The local client/server has not authenticate the peer device */
        return (CSR_BT_RESULT_CODE_OBEX_SUCCESS);
    }
}
#endif
static void obexUtilCliCmUnregisterHandler(ObexUtilPrivateInstType *priInst)
{
    if (priInst->cliInst->obtainedServer)
    { /* A local server channel has been obtained duing the connect procedure.
         Release the obtained server channel again */
        CsrBtCmUnRegisterReqSend(priInst->localServerCh);
    }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if (priInst->localPsm != L2CA_PSM_INVALID)
    {
        /* A dynamic PSM has been obtained duing the l2cap connect
         * procedure.  Release it again */
        ObexUtilInstType * inst = priInst->obexUtilOwnInstData;
        CsrBtCml2caUnRegisterReqSend(inst->phandle, priInst->localPsm);
    }
#endif
}

static CsrBool obexUtilValidateObexConnectResponse(ObexUtilPrivateInstType *priInst,
                                                   CsrUint16               *maxObexPacketSize)
{ /* This function returns TRUE if the OBEX connection has been established and
     the headers in OBEX connect response packet is valid according to the
     OBEX specification */
    *maxObexPacketSize = 0;

    if (priInst->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    { /* The OBEX connection has been established, drag data out of the OBEX
         Connect Response packet and check the the peer device follow the
         OBEX specification */
        if (!obexUtilValidateMaximumObexPacketLength(priInst))
        { /* The maximum OBEX packet length is not valid */
            priInst->resultCode     = CSR_BT_RESULT_CODE_OBEX_MAX_PACKET_LENGTH_TO_SMALL;
            priInst->resultSupplier = CSR_BT_SUPPLIER_OBEX_PROFILES;
            return (FALSE);
        }
        else
        { /* The maximum OBEX packet length is valid. */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE 
            priInst->resultCode = obexUtilValidateDigestResponse(priInst,
                                                                 CSR_BT_OBEX_RESPONSE_CONNECT);
            if (priInst->resultCode == CSR_BT_RESULT_CODE_OBEX_SUCCESS)
            { /* The Authenticate response is correct (if needed). Calculate the
                 maximum OBEX packet size that must be given to the application.
                 Note the reason for with drawing is that we used make sure that
                 we must have space to include the longest operation opcode
                 which is setpath request plus the standard header length
                 (1 byte opcode + 2 bytes length) */

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE 
                if (priInst->cliInst->authResultFunc)
                { /* OBEX Athentification is supported by the calling profile */
                    *maxObexPacketSize = obexUtilGetMaxObexPacketLength(priInst,
                                                                        CSR_BT_OBEX_UTIL_CONNECT_REQUEST_WITHDRAW);
                }
                else
#endif                    
#endif                    
                { /* OBEX Athentification is not supported by the calling
                     profile */
                    *maxObexPacketSize = obexUtilGetMaxObexPacketLength(priInst,
                                                                        CSR_BT_OBEX_UTIL_CONNECT_REQUEST_NO_AUTH_WITHDRAW);
                }

#ifdef CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID
                if (priInst->targetWho)
                { /* Check that the connectionId header is present */
                    if (CsrBtObexGetConnectionId(CSR_BT_OBEX_RESPONSE_CONNECT,
                                                 priInst->incomingObexPacket, &priInst->connectionId))
                    { /* A connectionId header is present, return TRUE to indicate
                         that the OBEX connection has been established */
                        return (TRUE);
                    }
                    else
                    { /* The peer device do not follow the OBEX GOEP specification
                         as the connectionId header must be used if the target
                         header is used in the OBEX Connect Request packet */
                        priInst->resultCode     = CSR_BT_RESULT_CODE_OBEX_CONNECTIONID_HEADER_MISSING;
                        priInst->resultSupplier = CSR_BT_SUPPLIER_OBEX_PROFILES;
                        return (FALSE);
                    }
                }
                else
#endif                    
                { /* No more information need to be drag out of the OBEX
                     Connect Response packet. Return TRUE to indicate that the
                     OBEX connection has been established */
                    return (TRUE);
                }
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
            }
            else
            { /* The Authenticate response is not correct */
                priInst->resultSupplier = CSR_BT_SUPPLIER_OBEX_PROFILES;
                return (FALSE);
            }
#endif            
        }
    }
    else
    { /* The OBEX connection could not be established because the OBEX server
         has rejected the request. Make sure that the transport connection is
         released before informing the profile */
        priInst->resultCode     = (CsrBtResultCode) priInst->responseCode;
        priInst->resultSupplier = CSR_BT_SUPPLIER_IRDA_OBEX;
        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_DISCONNECT_NORMAL_REQUEST_P_S);
        return (FALSE);
    }
}

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
static CsrBool obexUtilChallengeHeaderFind(CsrUint8 *obexPacket)
{ /* This function returns TRUE if a valid Challenge Header is found in the obexPacket */
#ifdef CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
    CsrUint16 length;

    if (CsrBtObexHeaderIndex((CsrBtObexSession) CSR_BT_OBEX_CONNECT_OPCODE, 
                            obexPacket, 
                            CSR_BT_OBEX_AUTHENTICATE_CHALLENGE_HEADER, 
                            &length))
    {
        return TRUE;
    }
#else
    CsrUint16        realmLength;
    CsrBool          optionsPresent;
    CsrUint8         *realm     = NULL;
    CsrUint8         options    = 0;
    CsrUint8         nonce[CSR_BT_NONCE_LENGTH];

    if (CsrBtObexCreateAuthChallenge((CsrBtObexSession) CSR_BT_OBEX_CONNECT_OPCODE,
                                     obexPacket,
                                     nonce,
                                     &options,
                                     &optionsPresent,
                                     &realm,
                                     &realmLength))
    {
        CsrPmemFree(realm);
        return (TRUE);
    }
#endif /* CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */
    return (FALSE);
}
#else
#define obexUtilChallengeHeaderFind(x) (FALSE)
#endif /* CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE */

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
static CsrBool obexUtilAuthenticateChallengeHeaderHandler(ObexUtilPrivateInstType * priInst)
{
    /* This function checks if the server has authenticate the client or not */
    if (priInst->responseCode == CSR_BT_OBEX_UNAUTHORIZED_CODE                     &&
        priInst->processState != CSR_BT_OBEX_UTIL_OBEX_ABORT_REQUEST_P_S           &&
        priInst->processState != CSR_BT_OBEX_UTIL_OBEX_ABORT_REQUEST_PENDING_P_S   &&
        priInst->processState != CSR_BT_OBEX_UTIL_OBEX_DISCONNECT_REQUEST_P_S)
    {
        /* Need to check that an Authenticate Challenge header is
         * present in the OBEX response packet */
        CsrUint16        realmLength;
        CsrBool          optionsPresent;
        CsrBtObexSession   session;
        CsrUint8         *realm      = NULL;
        CsrUint8         options     = 0;

        /* Response headers can not be autodetected */
        if (priInst->headerTypeTable[0] == CSR_BT_OBEX_UTIL_CONNECT_REQUEST_OPERATION)
        { /* An OBEX Connect Request has been issued */
            session = CSR_BT_OBEX_RESPONSE_CONNECT;
        }
        else
        { /* An OBEX Request different from Connect has been issued */
            session = CSR_BT_OBEX_RESPONSE_NORMAL;
        }

        if (CsrBtObexCreateAuthChallenge(session, priInst->incomingObexPacket,
                                         priInst->peerChallengeNonce, &options, &optionsPresent, &realm, &realmLength))
        { /* The server authenticates the client, because the incoming OBEX
             packet include an Authenticate Challenge header along with
             UNAUTHORIZED response code */
            ObexUtilCliInstType * cliInst  = priInst->cliInst;

            if (cliInst->authResultFunc)
            {
                CsrPmemFree(priInst->incomingObexPacket);
                CsrPmemFree(priInst->digestResponseHeader);
                priInst->incomingObexPacket         = NULL;
                priInst->digestResponseHeader       = NULL;
                priInst->incomingObexPacketLength   = 0;
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_AUTH_PENDING_P_S);
                cliInst->authResultFunc(priInst->profilesOwnInstData, priInst->deviceAddr, options, realmLength, realm);
            }
            else
            { /* The application has not registered a authenticateIndHandler
                 callback function. The only thing to do is to release the
                 transport connection */
                obexUtilReleaseTransportHandler(priInst, 
                                                CSR_BT_RESULT_CODE_OBEX_AUTHENTICATION_NOT_SUPPORTED, 
                                                CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
            }
            return (TRUE);
        }
        else
        { /* No Authenticate Challenge header is included, e.g the server has
             not anthenticated the client */
            return (FALSE);
        }
    }
    else
    { /* Return FALSE to indicate that either has the server has not
         anthenticated the client, or the client is aborting the operation
         that is being authenticated, or the client is releasing the OBEX
         connection */
        return (FALSE);
    }
}
#endif

static CsrUint8 obexUtilClientUnhandleFunctionCall(ObexUtilPrivateInstType * priInst)
{
    if (priInst)
    { /* Either is the callback function is not defined, or the function is
         received in an invalid state, return Exception */
        return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
    }
    else
    { /* Return Accepted because this function call can cross with an
         abnormal disconnect of the transport connection */
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
}

static CsrUint8 obexUtilServUnhandleFunctionCall(ObexUtilPrivateInstType * priInst)
{
    if (priInst)
    { /* Return Accepted because this function call can cross with an OBEX
         Abort Request or an  abnormal disconnect of the transport connection */
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* Either is the callback function is not defined, or the function is
         received in an invalid state, return Exception */
        return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
    }
}

/*------------------------------------------------------------------------------*
 * Static Obex for callback function handling                                   *
 *------------------------------------------------------------------------------*/
static void obexUtilObexIdleResultHandler(ObexUtilPrivateInstType *priInst)
{ /* This function is called if the OBEX client received an unexpected 
     data packet from the server */
    obexUtilReleaseTransportHandler(priInst,
                                    CSR_BT_RESULT_CODE_OBEX_SRM_VIOLATION_BY_REMOTE_DEVICE,
                                    CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
}

static void obexUtilObexConnectRequestResultHandler(ObexUtilPrivateInstType *priInst)
{ /* This function check if the OBEX connect has been established or not and
     calls the registered callback fuction */
    ObexUtilCliInstType * cliInst = priInst->cliInst;
    
    if (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_CONNECT_REQUEST_P_S)
    { /* An OBEX Connect Response is received. Get the data out of the OBEX
         Connect Response packet and check that the peer device follow the
         OBEX specification */
        CsrUint16 maxObexPacketSize;

        if (obexUtilValidateObexConnectResponse(priInst, &maxObexPacketSize))
        { /* The OBEX connection has been established */
            CsrUint16  obexPacketLength;
            CsrUint8 *obexPacket  = obexUtilGetObexPacketPointer(priInst, &obexPacketLength);

            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S);
            obexUtilDeallocatePrivateInstHeaderPointers(priInst, TRUE);
            obexUtilInitPrivateInstHeaderPointers(priInst, TRUE);
            priInst->obexOperationIndex = CSR_BT_OBEX_UTIL_IDLE_RESULT_HANDLER;
            cliInst->connectResultHandler(priInst->profilesOwnInstData,
                                          (CsrBtResultCode) CSR_BT_OBEX_SUCCESS_RESPONSE_CODE,
                                          CSR_BT_SUPPLIER_IRDA_OBEX,
                                          priInst->deviceAddr,
                                          priInst->btConnId,
                                          maxObexPacketSize,
                                          cliInst->sdpTag,
                                          obexPacketLength,
                                          obexPacket);

            cliInst->sdpTag             = NULL;
            CsrBtUtilSdcRfcDeinit(&cliInst->rfcConnectInst);
            obexUtilActivateLpSuperVisionTimer(priInst);
        }
        else
        { /* The OBEX connection could not be established, or the peer device
             do not follow the OBEX specification. Release the transport
             connection before informing the profile */
            obexUtilReleaseTransportConnection(priInst);
        }
    }
    else
    { /* The transport connection could not be established */
        if (priInst->processState == CSR_BT_OBEX_UTIL_BT_DISCONNECT_NORMAL_REQUEST_P_S)
        { /* The transport connection is release because the OBEX server has
             rejected the OBEX Connect Request */
            CsrUint16  obexPacketLength;
            CsrUint8 *obexPacket  = obexUtilGetObexPacketPointer(priInst, &obexPacketLength);

            cliInst->connectResultHandler(priInst->profilesOwnInstData,
                                          priInst->resultCode, priInst->resultSupplier,
                                          priInst->deviceAddr, 0, 0, NULL, 0,
                                          obexPacket);
        }
        else
        { /* The transport connection is release duing the OBEX Connect Request
             operation */
            cliInst->connectResultHandler(priInst->profilesOwnInstData,
                                          priInst->resultCode, priInst->resultSupplier,
                                          priInst->deviceAddr, priInst->btConnId, 0, NULL, 0 , NULL);
        }

        /* Check if the local server channel or the local dynamic PSM must be
           unregister or not */
        obexUtilCliCmUnregisterHandler(priInst);
        obexUtilDeallocatePrivateInstData(priInst->obexUtilOwnInstData);
    }
}

static void obexUtilObexDisconnectRequestResultHandler(ObexUtilPrivateInstType * priInst)
{
    /* This function is called whenever the an ongoing OBEX Connect
     * procedure has been cancel, or the transport connection has been
     * release */
    if (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_DISCONNECT_REQUEST_P_S)
    {

        /* The OBEX Disconnect Response packet is received. Note that
         * even if the peer device don't response with the
         * CSR_BT_OBEX_SUCCESS_RESPONSE_CODE the transport connection
         * is release */
        if (priInst->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
        { /* Setting the processState to CSR_BT_OBEX_UTIL_BT_DISCONNECT_NORMAL_REQUEST_P_S
             indicates that the received OBEX packet is valid and must be
             send to the application */
            priInst->resultCode     = (CsrBtResultCode) CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            priInst->resultSupplier = CSR_BT_SUPPLIER_IRDA_OBEX;
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_DISCONNECT_NORMAL_REQUEST_P_S);
        }
        else
        { /* The peer device has rejected the OBEX Disconnect Request. This
             could because the OBEX Disconnect Request contains invalid
             information. Just release the transport connection and set
             the processState to CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S
             to indicate that the received OBEX packet is invalid and must not
             send to the application */
            priInst->resultCode     = CSR_BT_RESULT_CODE_OBEX_INVALID_OBEX_DISCONNECT_RESPONSE_CODE;
            priInst->resultSupplier = CSR_BT_SUPPLIER_OBEX_PROFILES;
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
        }
        obexUtilReleaseTransportConnection(priInst);
    }
    else
    {
        /* The transport connection is released, inform profile */
        ObexUtilCliInstType * cliInst = priInst->cliInst;

        if (priInst->processState == CSR_BT_OBEX_UTIL_BT_DISCONNECT_NORMAL_REQUEST_P_S)
        {
            /* The OBEX Disconect procedure is finish */
            CsrUint16  obexPacketLength;
            CsrUint8 *obexPacket  = obexUtilGetObexPacketPointer(priInst, &obexPacketLength);

            cliInst->disconnectResultFunc(priInst->profilesOwnInstData,
                                          (CsrBtReasonCode) priInst->resultCode,
                                          priInst->resultSupplier,
                                          obexPacket,
                                          obexPacketLength);
        }
        else
        { /* The transport connection is released */
            cliInst->disconnectResultFunc(priInst->profilesOwnInstData,
                                          (CsrBtReasonCode) priInst->resultCode,
                                          priInst->resultSupplier,
                                          NULL, 0);
        }

        /* Check if the local server channel or the local dynamic PSM must be
           unregister or not */
        obexUtilCliCmUnregisterHandler(priInst);
        obexUtilDeallocatePrivateInstData(priInst->obexUtilOwnInstData);
    }
}

#ifdef CSR_BT_INSTALL_OBEX_CLI_SET_PATH
static void obexUtilObexSetpathRequestResultHandler(ObexUtilPrivateInstType * priInst)
{
    obexUtilCommonObexRequestOperationResultHandler(priInst);
}
#endif

static void obexUtilObexPutRequestResultFinalHandler(ObexUtilPrivateInstType * priInst,
                                                     CsrBool                 localInitiated)
{ /* This function is called when response of a PUT request is received */
    if (priInst->responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE && !localInitiated)
        { /* A OBEX response packet with responseCode CSR_BT_OBEX_CONTINUE_RESPONSE_CODE
             is received on a OBEX PUT REQUEST while SRM is 100% enable. This is
             wrong according to the specification and the transport link must be
             release */                       
             obexUtilReleaseTransportHandler(priInst,
                                             CSR_BT_RESULT_CODE_OBEX_SRM_VIOLATION_BY_REMOTE_DEVICE,
                                             CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
        }
        else
#else 
        CSR_UNUSED(localInitiated);
#endif /* CSR_BT_INSTALL_OBEX_GOEP_20 */
        {
            obexUtilCliValidateSingleResponseModeHeader(priInst);

            if (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_P_S)
            {
                /* Only body header must be sent in the next OBEX packet */
                ObexUtilCliInstType * cliInst = priInst->cliInst;

                if (cliInst->putContinueFunc)
                {
                    /* The application has a body header which must be
                     * included in the OBEX Put Request packet. Set the
                     * processState to
                     * CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_PENDING_P_S to
                     * indicate that only the body header must be
                     * included */
                    CsrUint16 length = obexUtilGetMaxObexPacketLength(priInst,
                                                                      CSR_BT_OBEX_UTIL_PUT_REQUEST_WITHDRAW);

                    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_PENDING_P_S);
                    obexUtilDeallocatePrivateInstHeaderPointers(priInst, TRUE);
                    obexUtilInitPrivateInstHeaderPointers(priInst, TRUE);
                    cliInst->putContinueFunc(priInst->profilesOwnInstData, length);
                }
                else
                {
                    /* The application has not registered a
                     * putContinueFunc callback function */
                    obexUtilReleaseTransportHandler(priInst,
                                                    CSR_BT_RESULT_CODE_OBEX_MULTIPLE_PUT_OPERATION_NOT_SUPPORTED,
                                                    CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
                }
            }
            else
            {
                /* The processState must be
                 * CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_SPLIT_P_S which means
                 * that the OBEX headers could not be sent in a single
                 * OBEX packet */
                CsrBtObexResponseCode responseCode;

                priInst->firstHeaderTypeIndex   = priInst->lastHeaderTypeIndex;
                responseCode = obexUtilValidateObexPacket(priInst, TRUE, priInst->firstHeaderTypeIndex);
                if (obexUtilObexPutRequestHandler(priInst, responseCode, TRUE))
                {
                    /* The OBEX Put Request packet is being build and sent */
                    CsrPmemFree(priInst->incomingObexPacket);
                    priInst->incomingObexPacket = NULL;
                }
                else
                {
                    /* One the the headers is to large. The only thing to
                     * do is to release the transport connection */
                    obexUtilReleaseTransportHandler(priInst,
                                                    CSR_BT_RESULT_CODE_OBEX_LOCAL_DEFINED_OBEX_HEADER_TOO_LARGE,
                                                    CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
                }
            }
        }
    }
    else
    {
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        /* The OBEX Put operation is finished */
        if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE)
        { /* SRM is enable */
            priInst->srmState = CSR_BT_OBEX_UTIL_SRM_REJECTED;
            
            if (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_PENDING_P_S)
            { /* An OBEX PUT Multi-step operation has been rejected by
                 the server while Single Response Mode is enable and
                 while waiting for ObexUtilPutContinueRequest is called
                 The ObexUtilPutContinueRequest function can see this because
                 priInst->srmState now is set to CSR_BT_OBEX_UTIL_SRM_REJECTED */
                ;
            }
            else
            { /* Note by setting priInst->srmState to CSR_BT_OBEX_UTIL_SRM_REJECTED
                 this function will not be called if the CSR_BT_CM_L2CA_DATA_CFM 
                 is crossing */
                if (priInst->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                { /* The PUT operation is finish with success. 
                     Notified the application */   
                    obexUtilCommonObexRequestOperationResultHandler(priInst);
                }
                else
                { /* The peer server has rejected an OBEX PUT operation while 
                     SRM is enable. Flush the l2cap buffer otherwise the server 
                     will continue to receive OBEX PUT Request messages and the 
                     next OBEX operation will be delay as all data in the buffer 
                     must be sent first. Note this feature is a CSR feature this 
                     is not demanded by the spec */
                    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_FLUSH_PUT_REQUEST_PENDING_P_S);
                    CsrBtCmL2caDataAbortReqSend(priInst->btConnId);
                }
            }
        }
        else
#endif
        { /* SRM is disable and the PUT operation is finish. Notified the application */
            obexUtilCommonObexRequestOperationResultHandler(priInst);
        }
    }
}

static void obexUtilObexPutRequestResultHandler(ObexUtilPrivateInstType * priInst)
{
    obexUtilObexPutRequestResultFinalHandler(priInst, FALSE);
}

#ifdef CSR_BT_INSTALL_OBEX_CLI_GET
static void obexUtilObexGetResponseHandler(ObexUtilPrivateInstType * priInst)
{
    CsrUint16           obexPacketLength;
    CsrUint16           bodyOffset  = 0;
    CsrUint16           bodyLength  = 0;
    CsrBool             bodyHeader  = FALSE;
    ObexUtilCliInstType *cliInst    = priInst->cliInst;
    CsrUint8          *obexPacket = obexUtilGetObexPacketPointer(priInst, &obexPacketLength);

    if (obexPacket)
    {
        /* The OBEX response packet is valid, look for a Body/End OF Body header */
        CsrUint8     bodyType;
        if (CsrBtObexGetBodyOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &bodyType, &bodyOffset, &bodyLength))
        {
            bodyHeader = TRUE;
        }
    }

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    obexUtilObexSingleResponseModeResponseHandler(priInst);
#endif

    obexUtilDeallocatePrivateInstHeaderPointers(priInst, TRUE);
    obexUtilInitPrivateInstHeaderPointers(priInst, TRUE);

    cliInst->getResultFunc(priInst->profilesOwnInstData, priInst->responseCode, bodyHeader,
                           bodyLength, bodyOffset, obexPacket, obexPacketLength);
}

static void obexUtilObexGetRequestResultHandler(ObexUtilPrivateInstType * priInst)
{
    /* This function is called when response of a GET request is received */
    if (priInst->responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        /* The reponse is large enough to require multiple GET requests */
        obexUtilCliValidateSingleResponseModeHeader(priInst);

        if (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_P_S)
        {
            /* All the headers has been sent */
            ObexUtilCliInstType * cliInst = priInst->cliInst;
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            if (priInst->srmState != CSR_BT_OBEX_UTIL_SRM_ACTIVE)
#endif
            {
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_PENDING_P_S);
            }

            if (!cliInst->putContinueFunc)
            {
                /* The final final bit has been set. E.g all headers
                 * are sent and the body header is (if any) */
                obexUtilObexGetResponseHandler(priInst);
            }
            else
            {
                /* The application has more body payload to send in
                 * the OBEX GET Request operation */
                CsrUint16 length = obexUtilGetMaxObexPacketLength(priInst,
                                                                  CSR_BT_OBEX_UTIL_GET_RESPONSE_WITHDRAW);

                obexUtilDeallocatePrivateInstHeaderPointers(priInst, TRUE);
                obexUtilInitPrivateInstHeaderPointers(priInst, TRUE);
                cliInst->putContinueFunc(priInst->profilesOwnInstData, length);
            }
        }
        else
        {
            /* The processState must be
             * CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_SPLIT_P_S which means
             * that the OBEX headers could not be sent in a single
             * OBEX packet */
            CsrBtObexResponseCode responseCode;

            priInst->firstHeaderTypeIndex   = priInst->lastHeaderTypeIndex;
            responseCode = obexUtilValidateObexPacket(priInst, TRUE, priInst->firstHeaderTypeIndex);

            if (obexUtilObexGetRequestHandler(priInst, responseCode, TRUE))
            {
                /* The OBEX Get Request packet is being build and sent */
                CsrPmemFree(priInst->incomingObexPacket);
                priInst->incomingObexPacket = NULL;
            }
            else
            {
                /* One the the headers is to large. The only thing to
                 * do is to release the transport connection. Note
                 * that if the function obexUtilObexGetRequestHandler
                 * returns FALSE it is responsible of clearing the SRM
                 * and SRMP states */
                obexUtilReleaseTransportHandler(priInst,
                                                CSR_BT_RESULT_CODE_OBEX_LOCAL_DEFINED_OBEX_HEADER_TOO_LARGE,
                                                CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
            }
        }
    }
    else
    { /* The OBEX Get operation is finished */
        priInst->obexOperationIndex = CSR_BT_OBEX_UTIL_IDLE_RESULT_HANDLER;
        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S);
        obexUtilActivateLpSuperVisionTimer(priInst);
        obexUtilObexGetResponseHandler(priInst);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        obexUtilRxQueueDataAckHandler(priInst);
#endif
    }
}
#endif

static void obexUtilObexAbortRequestResultHandler(ObexUtilPrivateInstType * priInst)
{
    ObexUtilCliInstType * cliInst  = priInst->cliInst;
    obexUtilResetAbortRequestSuperVisionTimer(cliInst);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_REJECTED &&
        priInst->responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        /* A multi-step OBEX GET operation were aborted while Single
         * Response Mode were enable. Discharged this message and
         * restart the timer */
        cliInst->abortSuperVisionTimerId = CsrSchedTimerSet(CSR_BT_OBEX_UTIL_FIRST_ABORT_DELAY,
                                                            obexUtilAbortRequestSuperVisionTimeoutHandler,
                                                            (CsrUint16)CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE, priInst);

        CsrPmemFree(priInst->incomingObexPacket);
        priInst->incomingObexPacket         = NULL;
        priInst->incomingObexPacketLength   = 0;
    }
    else
#endif
    {
        cliInst->abortNumOfRsp--;

        if (cliInst->abortNumOfRsp == 0)
        { /* A OBEX Abort response message is received */
            obexUtilAbortRequestFinalResultHandler(priInst, priInst->responseCode);
        }
        else
        { /* Must either wait for another OBEX response messages or until the
             abortSuperVisionTimer fires. As we don't known if this OBEX response
             message is for the OBEX Request we are trying to abort or for the OBEX
             Abort Request message */
            cliInst->abortSuperVisionTimerId = CsrSchedTimerSet(CSR_BT_OBEX_UTIL_SECOND_ABORT_DELAY,
                                                                obexUtilAbortRequestSuperVisionTimeoutHandler,
                                                                (CsrUint16) priInst->responseCode, priInst);

            if (priInst->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
            { /* Need to save the incoming OBEX message in another variable as
                 priInst->incomingObexPacket must be NULL in order to save the next
                 OBEX response packet */
                cliInst->abortIncomingObexPacket        = priInst->incomingObexPacket;
                cliInst->abortIncomingObexPacketLength  = priInst->incomingObexPacketLength;
            }
            else
            { /* Free the incoming OBEX message to prevent a memory leak. No need
                 to save incoming packet because if the response to an OBEX Abort
                 Request is anything else than CSR_BT_OBEX_SUCCESS_RESPONSE_CODE
                 the client should release the transport connection */
                CsrPmemFree(priInst->incomingObexPacket);
            }
            priInst->incomingObexPacket         = NULL;
            priInst->incomingObexPacketLength   = 0;
        }
    }
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static void obexUtilObexActionRequestResultHandler(ObexUtilPrivateInstType * priInst)
{
    obexUtilCommonObexRequestOperationResultHandler(priInst);
}
#endif

static const ObexUtilCallBackHandleType obexUtilObexOperationsHandlers[CSR_BT_OBEX_UTIL_NUM_OF_REQUEST_RESULT_HANDLER] =
{
    obexUtilObexIdleResultHandler,              /* CSR_BT_OBEX_UTIL_IDLE_RESULT_HANDLER */
    obexUtilObexConnectRequestResultHandler,    /* CSR_BT_OBEX_UTIL_CONNECT_REQUEST_RESULT_HANDLER */
    obexUtilObexDisconnectRequestResultHandler, /* CSR_BT_OBEX_UTIL_DISCONNECT_REQUEST_RESULT_HANDLER */
#ifdef CSR_BT_INSTALL_OBEX_CLI_SET_PATH
    obexUtilObexSetpathRequestResultHandler,    /* CSR_BT_OBEX_UTIL_SETPATH_REQUEST_RESULT_HANDLER */
#else    
    NULL,                                       
#endif
    obexUtilObexPutRequestResultHandler,        /* CSR_BT_OBEX_UTIL_PUT_REQUEST_RESULT_HANDLER */
#ifdef CSR_BT_INSTALL_OBEX_CLI_GET
    obexUtilObexGetRequestResultHandler,        /* CSR_BT_OBEX_UTIL_GET_REQUEST_RESULT_HANDLER */
#else
    NULL,
#endif
    obexUtilObexAbortRequestResultHandler,      /* CSR_BT_OBEX_UTIL_ABORT_REQUEST_RESULT_HANDLER */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    obexUtilObexActionRequestResultHandler,     /* CSR_BT_OBEX_UTIL_ACTION_REQUEST_RESULT_HANDLER */
#endif
};

/*------------------------------------------------------------------------------*
 * Static util functions only used by the OBEX activate/deactivate procedure    *
 *------------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static CsrUint16 obexUtilGetConftab(CsrUint16 mtuIn,
                                    CsrUint16 windowIn,
                                    CsrUint16 **conftab)
{
    CsrUint16 idx;
    CsrUint16 *tab;

    tab = CsrPmemZalloc(CSR_BT_OBEX_CONFTAB_WORK_SIZE * sizeof(CsrUint16));
    idx = 0;

    tab[idx++] = BKV_SEPARATOR; /* key */
    tab[idx++] = L2CA_AUTOPT_FLOW_MODE; /* single 16 */
    tab[idx++] = (CsrUint16)(L2CA_FLOW_MODE_ENHANCED_RETRANS << 8);
    tab[idx++] = L2CA_AUTOPT_FLOW_WINDOW_IN; /* range 16 */
    tab[idx++] = windowIn;
    tab[idx++] = windowIn;
    tab[idx++] = L2CA_AUTOPT_FLOW_MAX_RETX_IN; /* range 16 */
    tab[idx++] = CSR_BT_OBEX_UTIL_MAX_RETRANS;
    tab[idx++] = CSR_BT_OBEX_UTIL_MAX_RETRANS;
    tab[idx++] = L2CA_AUTOPT_MTU_IN; /* single 16 */
    tab[idx++] = mtuIn;
    tab[idx++] = CSR_BT_CM_AUTOPT_AUTO_FCS_OFF_AMP; /* single 16 */
    tab[idx++] = CSR_BT_CM_AUTOPT_TRUE;
    *conftab = tab;
    return idx;
}

static void obexUtilSendCmL2caConnectAcceptReq(ObexUtilInstType *inst,
                                               ObexUtilPrivateInstType *priInst,
                                               ObexUtilServInstType *servInst)
{
    CsrUint16 *conftab;
    CsrUint16 conflen;

    conflen = obexUtilGetConftab(priInst->maxLocalObexPacketLength,
                                 priInst->windowSize,
                                 &conftab);

    CsrBtCmL2caConnectAcceptReqConftabSend(inst->phandle,
                                           priInst->localPsm,
                                           servInst->classOfDevice,
                                           servInst->secIncoming,
                                           servInst->profileUuid,
                                           FALSE, /* not primary acceptor */
                                           inst->instId,
                                           conflen,
                                           conftab);
}


static void obexUtilSendCmL2caConnectReq(ObexUtilInstType *inst,
                                         ObexUtilPrivateInstType *priInst)
{
    CsrUint16 *conftab;
    CsrUint16 conflen;

    conflen = obexUtilGetConftab(priInst->maxLocalObexPacketLength,
                                 priInst->windowSize,
                                 &conftab);

    CsrBtCmL2caConnectReqConftabSend(inst->phandle,
                                     priInst->deviceAddr,
                                     priInst->localPsm,
                                     priInst->cliInst->remotePsm,
                                     priInst->cliInst->outgoingSecurityLevel,
                                     inst->instId,
                                     conflen,
                                     conftab);
}
#endif

static void obexUtilRegisterServiceRecordHandler(ObexUtilPrivateInstType * priInst)
{ /* This function makes a SDP record and request SDP to register it */
    ObexUtilInstType     *inst      = priInst->obexUtilOwnInstData;
    ObexUtilServInstType *servInst  = priInst->servInst;

    if (servInst->serviceRecord)
    { /* The profile has a service record it need to register */
        CsrUint8 *sdpRecord = (CsrUint8 *) CsrPmemAlloc(servInst->serviceRecordLength);

        CsrMemCpy(sdpRecord, servInst->serviceRecord, servInst->serviceRecordLength);

        if (servInst->serverChServiceRecordIndex != CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX)
        { /* The local server support OBEX over RFCOMM */
            sdpRecord[servInst->serverChServiceRecordIndex] = priInst->localServerCh;
        }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if (servInst->psmServiceRecordIndex != CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX)
        { /* The local server support OBEX over L2cap */
            sdpRecord[servInst->psmServiceRecordIndex]     = (CsrUint8)((priInst->localPsm)>>8);
            sdpRecord[servInst->psmServiceRecordIndex + 1] = (CsrUint8)((priInst->localPsm) & 0x00FF);
        }
#endif
        if (servInst->servChServiceRecordAltIndex != CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX)
        { /* The local server support both OBEX over L2cap and RFCOMM */
            sdpRecord[servInst->servChServiceRecordAltIndex] = priInst->localServerCh;
        }
        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_REGISTER_SDP_RECORD_REQUEST_P_S);
        CsrBtCmContextSdsRegisterReqSend(inst->phandle,
                                         sdpRecord,
                                         servInst->serviceRecordLength,
                                         inst->instId);
    }
    else
    { /* The profile do not want to register a service record. Please note the
         priInst->processState must be set before the call  back function
         (servInst->activateCfmHandler) is called. This will allow the profile
         to call ObexUtilServConnectableStart direct from the registered callback
         function (servInst->activateCfmHandler) */
        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_ACTIVATED_P_S);
        servInst->activateCfmHandler(priInst->profilesOwnInstData,
                                     priInst->localServerCh,
                                     priInst->localPsm,
                                     CSR_BT_RESULT_CODE_CM_SUCCESS,
                                     CSR_BT_SUPPLIER_CM);
    }
}

static void obexUtilRegisterBtMultiplexerHandle(ObexUtilPrivateInstType * priInst)
{ /* This function checks if a local server channel and or a local psm must be
     obtained or it can register the SDP record direct */
    ObexUtilInstType     *inst     = priInst->obexUtilOwnInstData;
    ObexUtilServInstType *servInst = priInst->servInst;

    if (priInst->localServerCh               == CSR_BT_NO_SERVER &&
        servInst->serverChServiceRecordIndex != CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX)
    {  /* The calling profile has not obtain a local server channel, obtain one */
        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_REGISTER_SERVERCH_REQUEST_P_S);
        CsrBtCmContextRegisterReqSend(inst->phandle, inst->instId);
    }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    else if (priInst->localPsm               == L2CA_PSM_INVALID &&
             servInst->psmServiceRecordIndex != CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX)
    { /* The calling profile has not obtain a local psm, obtain one */
        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_L2CA_REGISTER_PSM_P_S);
        CsrBtCmContextl2caRegisterReqSend(inst->phandle,
                                          CSR_BT_ASSIGN_DYNAMIC_PSM,
                                          L2CA_MODE_MASK_ENHANCED_RETRANS,
                                          0, /* flags */
                                          inst->instId);
    }                       
#endif
    else
    { /* The calling profile has already obtained a local server channel
         Make a serviceRecord and register it */
        obexUtilRegisterServiceRecordHandler(priInst);
    }
}

static void obexUtilSetDeviceConnectable(ObexUtilPrivateInstType * priInst)
{ /* This function set the local device connectable and tell the CM that it
     will accept an incoming connection */
    ObexUtilServInstType    *servInst = priInst->servInst;
    ObexUtilInstType        *inst     = priInst->obexUtilOwnInstData;

    if (priInst->processState == CSR_BT_OBEX_UTIL_BT_PENDING_CONNECTABLE_P_S)
    { /* Must wait for a ObexUtilServConnectResponse. */
        ;
    }
    else
    {
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if (servInst->rfcSubProcessState  == CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S &&
            servInst->l2caSubProcessState == CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S)
#else
            if (servInst->rfcSubProcessState  == CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S)
#endif
            { /* Both transport is in a state where where are ready to accept an
                 incoming connection */
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_CONNECTABLE_P_S);

                if (servInst->serverChServiceRecordIndex != CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX)
                { /* The local server support OBEX over RFCOMM */
                    CsrUint8 modemStatus = CSR_BT_MODEM_SEND_CTRL_DCE_DEFAULT;
                    servInst->rfcSubProcessState        = CSR_BT_OBEX_UTIL_BT_CONNECTABLE_SERVER_P_S;
                    CsrBtCmContextConnectAcceptReqSend(inst->phandle, servInst->classOfDevice, 0,
                                                       servInst->rfcommMtu, priInst->localServerCh,
                                                       servInst->secIncoming, servInst->profileUuid,
                                                       inst->instId, modemStatus, CSR_BT_DEFAULT_BREAK_SIGNAL,
                                                       CSR_BT_DEFAULT_MSC_TIMEOUT);

                }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                if (servInst->psmServiceRecordIndex != CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX)
                { /* The local server support OBEX over L2CAP */
                    servInst->l2caSubProcessState       = CSR_BT_OBEX_UTIL_BT_CONNECTABLE_SERVER_P_S;
                    obexUtilSendCmL2caConnectAcceptReq(inst, priInst, servInst);
                }
#endif
            }
            else
            { /* Both the l2cap and rfcomm instance must be in the right state,
                 before the two instance is activated again */
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_ACTIVATED_P_S);
            }
    }
}

static void obexUtilSetDeviceNonConnectable(ObexUtilPrivateInstType * priInst)
{ /* This function set the local device non-connectable and tell the CM that it
     will not accept an incoming connection */
    ObexUtilInstType     *inst     = priInst->obexUtilOwnInstData;
    ObexUtilServInstType *servInst = priInst->servInst;

    if (servInst->rfcSubProcessState == CSR_BT_OBEX_UTIL_BT_CONNECTABLE_SERVER_P_S)
    { /* Tell the CM that the Rfcomm instance will not accept an incoming
         rfcomm connection */
        servInst->rfcSubProcessState = CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECTABLE_SERVER_P_S;
        CsrBtCmContextCancelAcceptConnectReqSend(inst->phandle,
                                                 priInst->localServerCh,
                                                 inst->instId);
    }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if (servInst->l2caSubProcessState == CSR_BT_OBEX_UTIL_BT_CONNECTABLE_SERVER_P_S)
    { /* Tell the CM that the l2cap instance will not accept an incoming
         l2cap connection */
        servInst->l2caSubProcessState = CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECTABLE_SERVER_P_S;
        CsrBtCmContextl2caCancelConnectAcceptReqSend(inst->phandle, priInst->localPsm, inst->instId);
    }
#endif
}

static void obexUtilServUnregisterServerChannel(ObexUtilPrivateInstType * priInst,
                                                CsrUint8           localServerCh)
{ /* This function checks if the local server channel must be kept or not */
    if (priInst->processState == CSR_BT_OBEX_UTIL_BT_LIMITED_DEACTIVATING_P_S)
    { /* The profile want to keep the obtained server channel */
        priInst->localServerCh = localServerCh;
    }
    else
    { /* The profile don't want to keep the obtained server channel,
         unregister it again */
        CsrBtCmUnRegisterReqSend(localServerCh);
        priInst->localServerCh = CSR_BT_NO_SERVER;
    }
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static void obexUtilServUnregisterPsm(ObexUtilPrivateInstType * priInst,
                                      psm_t                   localPsm)
{ /* This function checks if the local server channel must be kept or not */
    if (priInst->processState == CSR_BT_OBEX_UTIL_BT_LIMITED_DEACTIVATING_P_S)
    { /* The profile want to keep the obtained psm */
        priInst->localPsm = localPsm;
    }
    else
    { /* The profile don't want to keep the obtained psm, unregister it again */
        ObexUtilInstType *inst = priInst->obexUtilOwnInstData;
        CsrBtCml2caUnRegisterReqSend(inst->phandle, localPsm);
        priInst->localPsm = L2CA_PSM_INVALID;
    }
}
#endif

static void obexUtilServDeactivateCfmHandler(ObexUtilPrivateInstType * priInst)
{
    ObexUtilServDeactivateFuncType deactivateCfmFunc = priInst->servInst->deactivateCfmHandler;
    void          *obexUtilOwnInstData               = priInst->profilesOwnInstData;
    CsrUint8 localServerCh                      = priInst->localServerCh;
    psm_t         localPsm                           = priInst->localPsm;
    obexUtilDeallocatePrivateInstData(priInst->obexUtilOwnInstData);
    deactivateCfmFunc(obexUtilOwnInstData, localServerCh, localPsm);
}

static void obexUtilUnregisterSdpRecord(ObexUtilPrivateInstType * priInst)
{ /* This function will unregister the sdp record (if any) */
    if (priInst->servInst->serviceRecordHandle != CMN_SDR_INVALID_SERVICE_HANDLE)
    { /* unregister the sdp record */
        ObexUtilInstType *inst = priInst->obexUtilOwnInstData;
        CsrBtCmContextSdsUnRegisterReqSend(inst->phandle,
                                           priInst->servInst->serviceRecordHandle,
                                           inst->instId);
    }
    else
    { /* No sdp record */
        obexUtilServUnregisterServerChannel(priInst, priInst->localServerCh);
        obexUtilServDeactivateCfmHandler(priInst);
    }
}

static void obexUtilNonConnectableHandler(ObexUtilPrivateInstType *priInst)
{
    ObexUtilServInstType *servInst = priInst->servInst;
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if (servInst->rfcSubProcessState  == CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S &&
        servInst->l2caSubProcessState == CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S)
#else
        if (servInst->rfcSubProcessState  == CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S)
#endif
        { /* Both the l2cap and the rfcomm instance is set non-connectable. Check
             if the deactivation or the non-connecable procedure is running */
            if (priInst->processState == CSR_BT_OBEX_UTIL_BT_NON_CONNECTABLE_P_S)
            { /* The OBEX server must be non-connectable */
                ObexUtilServInstType  *servInst = priInst->servInst;
                priInst->cmState                = CSR_BT_OBEX_UTIL_BT_ACTIVATED_S;
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_ACTIVATED_P_S);
                servInst->nonConnectableCfmHandler(priInst->profilesOwnInstData,
                                                   priInst->localServerCh,
                                                   priInst->localPsm);
            }
            else
            { /* The OBEX server must be deactivate, unregister the service record */
                obexUtilUnregisterSdpRecord(priInst);
            }
        }
        else
        { /* Must wait until both the l2cap and the rfcomm instance is set
             non-connectable */
            ;
        }
}

static CsrUint8 obexUtilDeactivateHandler(ObexUtilPrivateInstType *priInst,
                                          ObexUtilServInstType    *servInst,
                                          CsrUint8                processState)
{
    switch (priInst->cmState)
    {
        case CSR_BT_OBEX_UTIL_BT_RFC_CONNECTED_S:
            { /* Release the Rfcomm transport connection. The l2cap instance is
                 either non-connectable or being set non-connetable */
                ObexUtilInstType *inst = priInst->obexUtilOwnInstData;
                servInst->rfcSubProcessState = CSR_BT_OBEX_UTIL_BT_DISCONNECTING_SERVER_P_S;
                CsrBtCmContextDisconnectReqSend(priInst->btConnId,
                                                inst->instId);
                break;
            }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        case CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTED_S:
            {
                /* Release the L2cap transport connection. The RFCOMM
                 * instance is either non-connectable or being set
                 * non-connetable */
                ObexUtilInstType *inst = priInst->obexUtilOwnInstData;
                servInst->l2caSubProcessState = CSR_BT_OBEX_UTIL_BT_DISCONNECTING_SERVER_P_S;
                CsrBtCmContextl2caDisconnectReqSend(priInst->btConnId, inst->instId);
                break;
            }
#endif
        case CSR_BT_OBEX_UTIL_BT_ACTIVATED_S:
            {
                if (processState == CSR_BT_OBEX_UTIL_BT_NON_CONNECTABLE_P_S &&
                    (priInst->processState != CSR_BT_OBEX_UTIL_BT_CONNECTABLE_P_S ||
                     priInst->processState != CSR_BT_OBEX_UTIL_BT_PENDING_CONNECTABLE_P_S))
                { /* The profile is not allowed to start the non-connectable
                     procedure. */
                    return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
                }
                else
                {
                    switch (priInst->processState)
                    {
                        case CSR_BT_OBEX_UTIL_BT_CONNECTABLE_P_S:
                            { /* The server is connectable, make it non-connectable */
                                obexUtilSetDeviceNonConnectable(priInst);
                                break;
                            }
                        case CSR_BT_OBEX_UTIL_BT_ACTIVATED_P_S:
                        case CSR_BT_OBEX_UTIL_BT_PENDING_CONNECTABLE_P_S:
                            { /* Either the local server is activated or the transport
                                 connection were released while waiting for a
                                 ObexUtilServConnectResponse. This means that the local
                                 device is non-connectable. Set priInst->processState
                                 to processState and call obexUtilNonConnectableHandler.
                                 This function checks if it is because the local server
                                 wants to be non-connectable or if it want to deactivate*/
                                obexUtilSetProcessState(priInst, processState);
                                obexUtilNonConnectableHandler(priInst);
                                break;
                            }
                        case CSR_BT_OBEX_UTIL_BT_REGISTER_SERVERCH_REQUEST_P_S:
                        case CSR_BT_OBEX_UTIL_BT_REGISTER_SDP_RECORD_REQUEST_P_S:
                        case CSR_BT_OBEX_UTIL_BT_L2CA_REGISTER_PSM_P_S:
                        case CSR_BT_OBEX_UTIL_BT_NON_CONNECTABLE_P_S:
                            { /* Currently a local server channel or a PSM is being
                                 obtained, or a sdp record is being registered, or the
                                 device is being set non-connectable. Can not take any
                                 action before the ongoing procedure is finish */
                                break;
                            }
                        default :
                            { /* The profile is not allow to called the deactivation
                                 procedure because it is allready running. E.g. the
                                 processState must be either
                                 CSR_BT_OBEX_UTIL_BT_DEACTIVATING_P_S or
                                 CSR_BT_OBEX_UTIL_BT_LIMITED_DEACTIVATING_P_S. */
                                return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
                            }
                    }
                }
                break;
            }
        default:
            { /* The cmState must be in CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S,
                 change the processState in order to track which procedure is
                 running. E.g. it can either be a non-connectable or one of the
                 two deactivating procedure */
                break;
            }
    }
    obexUtilSetProcessState(priInst, processState);
    priInst->cmState = CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S;
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
static CsrUint8 obexUtilAuthenticateResponse(ObexUtilPrivateInstType        *priInst,
                                             CsrUint16                      passwordLength,
                                             CsrUint8                       *password,
                                             CsrCharString                      *userId,
                                             CsrBool                        role,
                                             ObexUtilDigestChallengeType    *chal)
{ /* This function adds the Authenticate Response Header to a OBEX Request or
     Response packet. E.g. either the client has authenticated the server or
     virsa versa, by sending a OBEX Request/Response packet with an Authenticate
     Challenge Header. To be authenticated, the OBEX packet must contain an
     Authenticate Response Header.
     In addition this function challenge the peer server back if *chal is not
     equal to NULL */
    CsrUint8  returnValue;

    if (priInst && priInst->processState == CSR_BT_OBEX_UTIL_OBEX_AUTH_PENDING_P_S)
    { /* The client or server is allow to add an Authenticate Response Header to
         an OBEX packet */
        CsrUint8  requestDigest[CSR_BT_DIGEST_REQUEST_LENGTH];
        CsrUint8  userIdLen = 0;

        if(userId)
        { /* A userId is defined, find the length */
            userIdLen = (CsrUint8)CsrStrLen((char *)userId);
        }

        if (role == CSR_BT_OBEX_UTIL_CLIENT_ROLE)
        {
            CsrBtGetDigest(priInst->peerChallengeNonce, password, passwordLength, requestDigest);
#ifdef CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
            if (chal)
            {/* Build the Authenticate Challenge Header */
                ObexUtilPfreeDigestChallengePointer(&(priInst->digestChallenge));
                CsrBtGetNonce(chal->nonce);
                priInst->digestChallenge = chal;

                priInst->digestChallengeHeader = obexUtilReturnAuthenticatePointer(CSR_BT_OBEX_AUTHENTICATE_CHALLENGE_HEADER,
                                                                                   CSR_BT_NONCE_TAG, CSR_BT_NONCE_LENGTH, chal->nonce, CSR_BT_OPTIONS_TAG,
                                                                                   CSR_BT_OPTIONS_LENGTH, &(chal->options), CSR_BT_REALM_TAG,
                                                                                   (CsrUint8)chal->realmLength, chal->realm,
                                                                                   &priInst->digestChallengeLength);
            }
#endif /* CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */
        }
        else
        {
#ifdef CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
            if (priInst->clientDigestChallenge)
            {
                CsrBtGetDigest(priInst->clientDigestChallenge->nonce, password, passwordLength, requestDigest);
            }
            else
#endif /* CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */
            {
                CsrBtGetDigest(priInst->digestChallenge->nonce, password, passwordLength, requestDigest);
            }
        }

        /* Build the Authenticate Response Header */
        priInst->digestResponseHeader = obexUtilReturnAuthenticatePointer(CSR_BT_OBEX_AUTHENTICATE_RESPONSE_HEADER,
                                                                          CSR_BT_DIGEST_REQUEST_TAG,
                                                                          CSR_BT_DIGEST_REQUEST_LENGTH,
                                                                          requestDigest,
                                                                          CSR_BT_DIGEST_USERID_TAG,
                                                                          userIdLen,
                                                                          (CsrUint8 *)userId,
                                                                          0,
                                                                          0,
                                                                          NULL,
                                                                          &priInst->digestResponseLength);

        obexUtilSetProcessState(priInst, priInst->preProcessState);
        obexUtilValidateObexPacket(priInst, FALSE, priInst->firstHeaderTypeIndex);

        if (priInst->firstHeaderTypeIndex == 0)
        {
            obexUtilBuildTransportPacket(priInst, FALSE);
        }
        else
        {
            obexUtilBuildTransportPacket(priInst, TRUE);
        }
        returnValue = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
    }
    else
    { /* Either the function ObexUtilCliAuthenticateResponse or the function
         ObexUtilServAuthenticateResponse in called in an invalid state */
        if (role == CSR_BT_OBEX_UTIL_CLIENT_ROLE)
        {
            returnValue = obexUtilClientUnhandleFunctionCall(priInst);
        }
        else
        {
            returnValue = obexUtilServUnhandleFunctionCall(priInst);
        }
    }
    CsrPmemFree(userId);
    CsrPmemFree(password);
    return (returnValue);
}
#endif

static void obexUtilCliIncomingObexPacketHandler(ObexUtilPrivateInstType * priInst)
{
    /* An entire OBEX Response packet is received */
    priInst->responseCode =
        (CsrBtObexResponseCode)(CSR_BT_OBEX_GET_RESPONSE_CODE(priInst->incomingObexPacket)
                                & ~CSR_BT_OBEX_FINAL_BIT_MASK);

    /* Reset the ObexCmdSuperVisionTimer */
    obexUtilResetObexCmdSuperVisionTimer(priInst);

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
    if (obexUtilAuthenticateChallengeHeaderHandler(priInst))
    {
        /* The server has authenticated the client. If a callback
         * function is defined for this it is called, if not the
         * transport connection is being released */
        ;
    }
    else
#endif        
    {
        /* The server has not anthenticated the client */
        obexUtilObexOperationsHandlers[priInst->obexOperationIndex](priInst);
    }
}

static void obexUtilObexAbortIndHandler(ObexUtilPrivateInstType *priInst)
{
    switch (priInst->processState)
    {
#ifdef CSR_BT_INSTALL_OBEX_SRV_SET_PATH
        case CSR_BT_OBEX_UTIL_OBEX_SETPATH_RESPONSE_PENDING_P_S:
#endif            
        case CSR_BT_OBEX_UTIL_OBEX_PUT_RESPONSE_PENDING_P_S:
        case CSR_BT_OBEX_UTIL_OBEX_GET_RESPONSE_PENDING_P_S:
        case CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_PENDING_P_S:
        case CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_PENDING_P_S:
        case CSR_BT_OBEX_UTIL_OBEX_ACTION_RESPONSE_PENDING_P_S:
            { /* An OBEX Abort Request packet is received or this library needs
                 to send an OBEX response packet with a responseCode that indicates
                 an error while local server is ind a middle of an OBEX SETPATH,
                 PUT, GET, or ACTION operation. Inform the server that the active
                 OBEX operation is aborted, by calling the callback function
                 abortIndHandler */
                CsrUint16 descriptionLength = 0;
                CsrUint16 descriptionOffset = 0;
                CsrUint16 obexPacketLength;
                CsrUint8 *obexPacket = obexUtilGetObexPacketPointer(priInst, &obexPacketLength);
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S);

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                if (obexPacket)
                { /* An OBEX Abort Request packet is received. Check if an OBEX
                     description header is included */
                    descriptionOffset = CsrBtObexDescriptionHeaderIndex(CSR_BT_OBEX_REQUEST,
                                                                        obexPacket,
                                                                        &descriptionLength);

                    if (descriptionOffset > 0 && (descriptionLength > CSR_BT_OBEX_HEADER_LENGTH))
                    { /* A valid descriptor is included */
                        descriptionLength = (CsrUint16) (descriptionLength - CSR_BT_OBEX_HEADER_LENGTH);
                        descriptionOffset = (CsrUint16) (descriptionOffset + CSR_BT_OBEX_HEADER_LENGTH);

                        /* Check taken from the FTS profile, it ensures an IOP problem*/
                        obexPacket[descriptionOffset + descriptionLength - 1] = '\0';
                    }
                    else
                    { /* The header is invalid */
                        descriptionLength = 0;
                        descriptionOffset = 0;
                    }
                }
#endif
                priInst->servInst->abortIndHandler(priInst->profilesOwnInstData,
                                                   descriptionOffset,
                                                   descriptionLength,
                                                   obexPacketLength,
                                                   obexPacket);
                break;
            }
        default:
            { /* The local server is not aware of it is in the middle of an OBEX
                 operation. Just ignore it */
                break;
            }
    }
}

static void obexUtilServObexResponsePacketHandler(ObexUtilPrivateInstType *priInst,
                                                  CsrUint8                opCode,
                                                  CsrBtObexResponseCode   responseCode)
{
    /* Build and start sending an OBEX Response packet */
    priInst->responseCode = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);

    obexUtilObexSingleResponseModeResponseHandler(priInst);

    /* Note priInst->incomingObexPacket must be set NULL before the
     * function obexUtilObexAbortIndHandler is called */
    CsrPmemFree(priInst->incomingObexPacket);
    priInst->incomingObexPacket         = NULL;
    priInst->incomingObexPacketLength   = 0;

    if (opCode == CSR_BT_OBEX_CONNECT_OPCODE)
    {
        /* Send an OBEX Connect Response Packet */
        obexUtilUpdateHeaderTable(priInst, CSR_BT_OBEX_UTIL_CONNECT_RESPONSE_OPERATION, 0, NULL);
    }
    else
    {
        /* Send an standard OBEX response Packet. If an OBEX operation
         * is rejected by this library make sure that the local server
         * is notified by calling the abort callback function */
        obexUtilUpdateHeaderTable(priInst, CSR_BT_OBEX_UTIL_COMMON_RESPONSE_OPERATION, 0, NULL);
        obexUtilObexAbortIndHandler(priInst);
    }
    obexUtilValidateObexPacket(priInst, FALSE, 0);
    obexUtilBuildTransportPacket(priInst, FALSE);
}

#ifdef CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID
static CsrUint8 * obexUtilSrvGetTargetHeader(ObexUtilPrivateInstType *priInst,
                                             CsrUint16               *targetHeaderLength)
{ /* This function check the target header is included in the OBEX Connect
     Request packet. If it is a allocated target header is returned, if not
     NULL is returned */
    CsrUint16 lengthOfTarget, index;
    CsrUint8  *target   = NULL;
    *targetHeaderLength = 0;

    index = CsrBtObexTargetHeaderIndex(CSR_BT_OBEX_CONNECT_OPCODE,
                                       priInst->incomingObexPacket, &lengthOfTarget);

    if (index && lengthOfTarget > CSR_BT_OBEX_HEADER_LENGTH)
    { /* A target header is included in the OBEX Connect Request packet. */
        lengthOfTarget      = (CsrUint8)(lengthOfTarget - CSR_BT_OBEX_HEADER_LENGTH);
        *targetHeaderLength = lengthOfTarget;
        target              = (CsrUint8 *) CsrPmemAlloc(lengthOfTarget);
        CsrMemCpy(target, &(priInst->incomingObexPacket[index + CSR_BT_OBEX_HEADER_LENGTH]), lengthOfTarget);
    }
    return (target);
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
static CsrBool obexUtilSrvObexClientChallengeServHandler(ObexUtilPrivateInstType *priInst)
{ /* This function checks if the Client authenticated the server. E.g. need to
     find out if the Authenticate Challenge Header is include in the  incoming
     OBEX packet or not. Note if the Authenticate Challenge Header is included
     this information is save in priInst->digestChallenge as well. E.g.
     priInst->digestChallenge is also used when the server wants to challenge
     the client. The same variable can be used because if the client challenge
     the server and the server in the response function (could be the
     ObexUtilServConnectResponse) challenge the client the client must repeat
     the same command with an Authenticate Response include. This means that
     every time the server challenge the client the priInst->digestChallenge
     must be CsrPmemFree before it is set again. This is done in the function
     obexUtilValidateSrvResponseCode. */
    CsrUint16        realmLength;
    CsrBool          optionsPresent;
    CsrUint8         *realm     = NULL;
    CsrUint8         options    = 0;
    CsrUint8         nonce[CSR_BT_NONCE_LENGTH];

    if (CsrBtObexCreateAuthChallenge((CsrBtObexSession) CSR_BT_OBEX_CONNECT_OPCODE,
                                     priInst->incomingObexPacket,
                                     nonce, &options, &optionsPresent,
                                     &realm, &realmLength))
    { /* The client has challenge the server, because the incoming OBEX packet
         include an Authenticate Challenge header */
        if (priInst->servInst->authenticateIndHandler)
        { /* The profile support this */
            ObexUtilDigestChallengeType * digestChallenge;
            digestChallenge = (ObexUtilDigestChallengeType *)
                CsrPmemAlloc(sizeof(ObexUtilDigestChallengeType));

            digestChallenge->passwordLength = 0;
            digestChallenge->password       = NULL;
            digestChallenge->options        = options;
            digestChallenge->realm          = realm;
            digestChallenge->realmLength    = realmLength;
            digestChallenge->userId         = NULL;
            CsrMemCpy(digestChallenge->nonce, nonce, CSR_BT_NONCE_LENGTH);
            priInst->digestChallenge        = digestChallenge;
        }
        else
        { /* The profile don't support this. The function
             obexUtilServObexResponsePacketHandler sends a OBEX response packet
             with the given response code and CsrPmemFree
             priInst->incomingObexPacket */
            obexUtilServObexResponsePacketHandler(priInst, CSR_BT_OBEX_CONNECT_OPCODE,
                                                  CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE);
            return (FALSE);
        }
    }
    else
    { /* The client has not challenge the server */
        ;
    }
    return (TRUE);
}

static CsrBool obexUtilSrvObexAuthenticateChallengeHandler(ObexUtilPrivateInstType * priInst,
                                                           CsrUint8                opCode,
                                                           CsrBool                 *authenticated)
{
    *authenticated = FALSE;

    if (obexUtilValidateDigestResponse(priInst, (CsrBtObexSession) opCode) == CSR_BT_RESULT_CODE_OBEX_SUCCESS)
    { /* The Authenticate response is correct (if needed). */

        if (priInst->digestChallenge)
        { /* Make sure that the digestChallenge is deallocated */
            ObexUtilPfreeDigestChallengePointer(&(priInst->digestChallenge));
            *authenticated           = TRUE;
        }

        if (opCode == CSR_BT_OBEX_CONNECT_OPCODE)
        { /* Need to check if the Client authenticated the server. */
            return (obexUtilSrvObexClientChallengeServHandler(priInst));
        }
        else
        { /* No need to check it as it is expected that the client only
             challenge the server duing a OBEX Connect Request */
            ;
        }
    }
    else
    { /* The Digest Response header were invalid, e.g. either the password or
         the userId were not correct. The function
         obexUtilServObexResponsePacketHandler sends a OBEX response
         packet with the given response code and CsrPmemFree
         priInst->incomingObexPacket */
        obexUtilServObexResponsePacketHandler(priInst, opCode, CSR_BT_OBEX_UNAUTHORIZED_CODE);
        return (FALSE);
    }
    return (TRUE);
}
#endif

static CsrUint16 obexUtilSrvGetMaxObexPacketLength(ObexUtilPrivateInstType * priInst,
                                                   CsrUint8                noChallenge,
                                                   CsrUint8                withChallenge)
{
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    if (priInst->digestChallenge)
    { /* The client has challenge the server */
        return(obexUtilGetMaxObexPacketLength(priInst, withChallenge));
    }
    else
#else
    CSR_UNUSED(withChallenge);
#endif
    { /* The client has not challenge the server */
        return(obexUtilGetMaxObexPacketLength(priInst, noChallenge));
    }
}

static CsrBool obexUtilValidateSrvResponseCode(ObexUtilPrivateInstType     *priInst,
                                               ObexUtilDigestChallengeType *digestChallenge,
                                               CsrBtObexResponseCode       responseCode,
                                               CsrBool                     directAuthenticateResponse)
{ /* This function validate the response code retrieve from the local server.
     If TRUE is returned response is consider as a success, if FALSE not */
    CsrBool returnValue                 = FALSE;
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    CsrBool digestChallengePtrInvalid   = TRUE;
#endif
    priInst->responseCode = (responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);

    switch (responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK)
    {
        case CSR_BT_OBEX_SUCCESS_RESPONSE_CODE:
        case CSR_BT_OBEX_CONTINUE_RESPONSE_CODE:
            {
                returnValue = TRUE;
                break;
            }
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
        case CSR_BT_OBEX_UNAUTHORIZED_CODE:
            {
                if (digestChallenge &&
                    (priInst->preProcessState == CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S ||
                     priInst->preProcessState == CSR_BT_OBEX_UTIL_BT_CONNECTED_P_S))
                { /* The local server want to authenticate the OBEX client, in the
                     first response. Consider this response code as a success */
                    returnValue                 = TRUE;
                    digestChallengePtrInvalid   = FALSE;
                }
                else
                { /* The local server don't want to authenticate the OBEX client,
                     consider this response code as a error */
                    ;
                }
                break;
            }
#endif        
        default:
            { /* The local server has responded with an response code that indicates
                 the it want to reject the current OBEX procedure. There are no need
                 to include the challenge header */
                break;
            }
    }

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    if (digestChallengePtrInvalid)
    { /* The local server is not allow to set the digestChallenge. Make sure
         that this pointer is not set */
        if (digestChallenge)
        { /* The local server has set the digestChallenge pointer, but is
             not allow to do it. CsrPmemFree it to prevent a memory leak and set
             to NULL */
            ObexUtilPfreeDigestChallengePointer(&digestChallenge);
        }
    }

    if (digestChallenge || returnValue == FALSE)
    { /* The local server has either rejected the process or it want to
         authenticate the OBEX client. If the client has challenge the server
         this must be ignore. This is done by making sure that
         priInst->digestChallenge is CsrPmemFree and set NULL. For more info see
         also the text in the function
         obexUtilSrvObexAuthenticateChallengeHandler */
#ifdef CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
        ObexUtilPfreeDigestChallengePointer(&(priInst->clientDigestChallenge));

        if (digestChallenge && directAuthenticateResponse)
        { /* The Auth Rsp must be send along with the Auth Chal header.
             Note this do not follow the IrDa specification */
            priInst->clientDigestChallenge = priInst->digestChallenge;
            priInst->digestChallenge       = NULL;
        }
        else
#endif /* CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */
        {
            ObexUtilPfreeDigestChallengePointer(&(priInst->digestChallenge));
        }
    }

    if (digestChallenge)
    { /* The local server want to authenticate the OBEX client. Build the
         Challenge pointer */
        obexUtilBuildObexChallengePointer(priInst, digestChallenge);
    }
    else
    { /* The local server don't want to authenticate the OBEX client. */
        priInst->digestChallengeHeader = NULL;
        priInst->digestChallengeLength = 0;
    }
#endif    
    return (returnValue);
}

static void obexUtilServResponsePacketHandler(ObexUtilPrivateInstType *priInst,
                                              CsrUint8                numOfHeaders,
                                              const CsrUint8          *headers,
                                              CsrUint8                obexOperation)
{
    /* This function updates the OBEX header table, and either send
     * the response packet direct or request the server to answer the
     * authentification first */

    /* Update the table which defines which OBEX headers must be
     * included in the OBEX Response packet */
    obexUtilObexSingleResponseModeResponseHandler(priInst);
    obexUtilUpdateHeaderTable(priInst, obexOperation, numOfHeaders, headers);

    obexUtilValidateObexPacket(priInst, FALSE, 0);

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    if (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_AUTH_PENDING_P_S)
    {
        /* The client has authenticate the server */
        ObexUtilServInstType  *servInst = priInst->servInst;
#ifdef CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
        if (priInst->clientDigestChallenge)
        {
            servInst->authenticateIndHandler(priInst->profilesOwnInstData,
                                             priInst->deviceAddr,
                                             priInst->clientDigestChallenge->options,
                                             priInst->clientDigestChallenge->realmLength,
                                             priInst->clientDigestChallenge->realm);
            priInst->clientDigestChallenge->realm = NULL;
        }
        else
#endif /* CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */
        {
            servInst->authenticateIndHandler(priInst->profilesOwnInstData,
                                             priInst->deviceAddr,
                                             priInst->digestChallenge->options,
                                             priInst->digestChallenge->realmLength,
                                             priInst->digestChallenge->realm);
            priInst->digestChallenge->realm = NULL;
        }
    }
    else
#endif        
    {
        /* The server has either accepted or rejected the OBEX
         * command.  Build and send the OBEX Response packet */
        obexUtilBuildTransportPacket(priInst, FALSE);
    }
}

static void obexUtilServValidateSingleResponseHandler(ObexUtilPrivateInstType     *priInst,
                                                      ObexUtilDigestChallengeType *digestChallenge,
                                                      CsrBtObexResponseCode       responseCode,
                                                      CsrBool                     directAuthenticateResponse)
{ /* This function is to priInst->processState when the response packet must fit
     a single packet, which applies for the following OBEX operation. CONNECT,
     SETPATH, SESSION */
    if (obexUtilValidateSrvResponseCode(priInst, digestChallenge, responseCode, directAuthenticateResponse))
    { /* The server has either accepted the OBEX operation, or want to
         authenticate the client */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
        if (digestChallenge)
        { /* The server has authenticated the client, the client must
             send a new OBEX Request packet */
            priInst->servInst->deleteDigestChallenge = FALSE;

#ifdef CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
            if (priInst->clientDigestChallenge)
            { /* The Auth Rsp must be send along with the Auth Chal header.
                 Note this do not follow the IrDa specification */
                priInst->processState = CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S;
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_AUTH_PENDING_P_S);
            }
            else
#endif /* CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */
            {
                obexUtilSetProcessState(priInst, priInst->preProcessState);
            }
        }
        else if (priInst->digestChallenge)
        { /* The server has accepted the OBEX command, but the client has
             authenticated the server */
            priInst->processState = CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S;
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_AUTH_PENDING_P_S);
        }
        else
#endif            
        { /* Both sides has accepted the OBEX command */
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S);
        }
    }
    else
    { /* The profile has rejected the OBEX command */
        obexUtilSetProcessState(priInst, priInst->preProcessState);
    }
}

static void obexUtilServValidateMultiStepResponseHandler(ObexUtilPrivateInstType     *priInst,
                                                         CsrBtObexResponseCode       responseCode,
                                                         ObexUtilDigestChallengeType *digestChallenge,
                                                         CsrUint8                    continueState,
                                                         CsrUint8                    srmpValue)
{
    if (obexUtilValidateSrvResponseCode(priInst, digestChallenge, responseCode, FALSE))
    {
        switch (priInst->responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK)
        {
            case CSR_BT_OBEX_CONTINUE_RESPONSE_CODE:
                {
                    /* This is a multi-step PUT or GET operation, e.g
                     * it consist of more than one OBEX PUT or GET
                     * Request packets */
                    obexUtilSetProcessState(priInst, continueState);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                    if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_IDLE)
                    {
                        /* It must be the first PUT or GET response
                         * message in this multi-step operation. */
                        CsrBool srmpSupported;

                        priInst->singleResponseModeHeader = buildSingleResponseModeHeader(CSR_BT_OBEX_UTIL_SRM_ENABLED);

                        if (srmpValue == CSR_BT_OBEX_SRMP_WAIT)
                        {
                            /* The application has requested the SRMP
                             * is included in the first OBEX GET/PUT
                             * Response packet */
                            srmpSupported = TRUE;
                            priInst->srmpHeader = buildSrmpHeader(srmpValue);
                        }
                        else
                        {
                            /* The application don't want to use SRMP */
                            srmpSupported = FALSE;
                        }
                        obexUtilSrmIdleSrmpStateHandler(priInst, srmpSupported);
                    }
                    else
                    {
                        /* Must check if SRMP is being used or not */
                        obexUtilSrmpHeaderHandler(priInst, srmpValue, TRUE, CSR_BT_OBEX_UTIL_SERVER_ROLE, NULL);
                    }
#endif
                    break;
                }
            default:
                {
                    /* Must either be
                     * CSR_BT_OBEX_SUCCESS_RESPONSE_CODE or
                     * CSR_BT_OBEX_UNAUTHORIZED_CODE, e.g the local
                     * server wants to challenge the client */
                    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                    if (digestChallenge)
                    {
                        /* The server has authenticated the client,
                         * the client must send a new OBEX Request
                         * packet */
                        priInst->servInst->deleteDigestChallenge = FALSE;
                    }
#endif                    
                    break;
                }
        }
    }
    else
    {
        /* The local server has rejected the OBEX PUT or GET operation */
        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if (priInst->srmState != CSR_BT_OBEX_UTIL_SRM_NOT_SUPPORTED)
        {
            /* An multi-step OBEX PUT operation is rejected while
             * Single Response Mode is enabled. Set srmState to
             * CSR_BT_OBEX_UTIL_SRM_REJECTED as the following OBEX PUT
             * Request packets must be discharged until an non-body
             * header OBEX PUT Request or OBEX command has been
             * received */
            priInst->srmState = CSR_BT_OBEX_UTIL_SRM_REJECTED;
        }
#endif
    }
}

static CsrUint8 obexUtilServPutResponsePacketHandler(void                        *obexUtilInstData,
                                                     CsrBtObexResponseCode       responseCode,
                                                     ObexUtilDigestChallengeType *digestChallenge,
                                                     CsrUint8                    numOfHeaders,
                                                     CsrUint8                    *headerTable,
                                                     CsrUcs2String               *name,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                                     CsrUcs2String               *description,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                                                     CsrUint16                   appParametersHeaderLength,
                                                     CsrUint8                    *appParametersHeader,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                                     CsrUint16                   userDefinedHeaderLength,
                                                     CsrUint8                    *userDefinedHeader,
#endif
                                                     CsrUint8                    srmpValue)
{
    CsrUint8 result;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_SERVER_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif

    if (priInst &&
        (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_PUT_RESPONSE_PENDING_P_S 
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                                                                                    ||
         (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_PENDING_P_S    && 
          priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE)
#endif
        ))   
    {
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
        priInst->userDefinedHeaderLength    = userDefinedHeaderLength;
        priInst->userDefinedHeader          = userDefinedHeader;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        priInst->description                = description;
#endif
        priInst->name                       = name;
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
        priInst->appParametersHeaderLength  = appParametersHeaderLength;
        priInst->appParametersHeader        = appParametersHeader;
#endif
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if((priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE) &&
           ((responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE))
        { /* Do not send continue packets when SRM is active                    */
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_PENDING_P_S);
            obexUtilRxQueueDataAckHandler(priInst);
        }
        else
#endif
        {
            obexUtilServValidateMultiStepResponseHandler(priInst,
                                                         responseCode,
                                                         digestChallenge,
                                                         CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_PENDING_P_S,
                                                         srmpValue);

            obexUtilServResponsePacketHandler(priInst, numOfHeaders, headerTable,
                                              CSR_BT_OBEX_UTIL_COMMON_RESPONSE_OPERATION);
        }

        result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
    }
    else
    {
        /* The profile is currently not allowed to call this function,
         * make sure that the pointers is CsrPmemFree to prevent a
         * memoryleak. */
         
        obexUtilPfreeStdPointers(digestChallenge, description, name, userDefinedHeader, appParametersHeader);
        result = obexUtilServUnhandleFunctionCall(priInst);
    }
    return result;
}

#ifdef CSR_BT_INSTALL_OBEX_SRV_GET
static CsrUint8 obexUtilServGetResponsePacketHandler(void                        *obexUtilInstData,
                                                     CsrBtObexResponseCode       responseCode,
                                                     ObexUtilDigestChallengeType *digestChallenge,
                                                     CsrUint8                    numOfHeaders,
                                                     CsrUint8                    *headerTable,
                                                     CsrUint8                    bodyHeaderPriorityIndex,
                                                     CsrUint32                   lengthOfObject,
                                                     CsrUcs2String               *name,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                                     CsrUcs2String               *description,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                                                     CsrUint16                   appParametersHeaderLength,
                                                     CsrUint8                    *appParametersHeader,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                                     CsrUint16                   userDefinedHeaderLength,
                                                     CsrUint8                    *userDefinedHeader,
#endif
                                                     CsrUint8                    *body,
                                                     CsrUint16                   bodyLength,
                                                     CsrUint8                    srmpValue)
{
    CsrUint8 result;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK 
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_SERVER_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif

    if (priInst && priInst->processState == CSR_BT_OBEX_UTIL_OBEX_GET_RESPONSE_PENDING_P_S)
    {
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
        priInst->userDefinedHeaderLength    = userDefinedHeaderLength;
        priInst->userDefinedHeader          = userDefinedHeader;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        priInst->description                = description;
#endif
        priInst->name                       = name;
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
        priInst->appParametersHeaderLength  = appParametersHeaderLength;
        priInst->appParametersHeader        = appParametersHeader;
#endif
        priInst->lengthOfObject             = lengthOfObject;

        if (bodyHeaderPriorityIndex < CSR_BT_OBEX_UTIL_MAX_NUM_OF_HEADERS_IN_GET_RESPONSE)
        {
            if ((responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
            {
                /* This is the last OBEX Get Response packet, make
                 * sure that the opcode for the body header is set to
                 * End of Body (0x49) */
                headerTable[bodyHeaderPriorityIndex] = CSR_BT_OBEX_UTIL_END_BODY_HEADER;
            }
            else
            {
                /* This is not the last body fragment, make sure that
                 * the opcode for the body header is set Body
                 * (0x48) */
                headerTable[bodyHeaderPriorityIndex] = CSR_BT_OBEX_UTIL_BODY_HEADER;
            }
            priInst->bodyLength                 = bodyLength;
            priInst->body                       = body;
        }
        else
        {
            /* No body header must be included in the OBEX Response
             * packet. CsrPmemFree the body to prevent a memory
             * leak */
            CsrPmemFree(body);
        }

        obexUtilServValidateMultiStepResponseHandler(priInst,
                                                     responseCode,
                                                     digestChallenge,
                                                     CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_PENDING_P_S,
                                                     srmpValue);

        obexUtilServResponsePacketHandler(priInst, numOfHeaders, headerTable,
                                          CSR_BT_OBEX_UTIL_COMMON_RESPONSE_OPERATION);

        result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
    }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    else if (priInst                                                &&
             priInst->servInst->opCode == CSR_BT_OBEX_GET_OPCODE    &&
             priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE       &&
             priInst->processState == CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_PENDING_P_S)
    {
        /* The application has rejected the OBEX GET operation while
         * Single Response mode is enable and before the final bit has
         * been set.  Save this response because GET operations with
         * SRM enabled shall not send a GET response until the GET
         * request + Final Bit has been issued */
        priInst->responseCode = (responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);
        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_SRM_REJECTED_P_S);
        obexUtilPfreeStdPointers(digestChallenge, description, name, userDefinedHeader, appParametersHeader);
        CsrPmemFree(body);
        result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
    }
#endif
    else
    {
        /* The profile is currently not allowed to call this function,
         * make sure that the pointers is CsrPmemFree to prevent a
         * memoryleak. */
        obexUtilPfreeStdPointers(digestChallenge, description, name, userDefinedHeader, appParametersHeader);
        CsrPmemFree(body);
        result = obexUtilServUnhandleFunctionCall(priInst);
    }

    return result;
}
#endif
static CsrUint8 * obexUtilServCommonPutGetRequestHandler(ObexUtilPrivateInstType *priInst,
                                                         CsrBool                 authenticated,
                                                         CsrUint16               withDraw,
                                                         CsrUint16               withDrawNoAuth,
                                                         CsrUint8                nextProcessState,
                                                         CsrUint16               *obexPacketLength,
                                                         CsrUint16               *maxDataInResponse)
{
    /* It is important that priInst->incomingObexPacket is set to NULL
     * here, and that priInst->processState is changed to either
     * CSR_BT_OBEX_UTIL_OBEX_PUT_RESPONSE_PENDING_P_S or
     * CSR_BT_OBEX_UTIL_OBEX_GET_RESPONSE_PENDING_P_S before the
     * callback function is called. This allows the server to call
     * either of:
     *  ObexUtilServSetPutContinueFunc
     *  ObexUtilServPutWithHeaderResponse
     *  ObexUtilServPutResponse
     *  ObexUtilServSetGetContinueFunc
     *  ObexUtilServGetWithHeaderResponse
     *  ObexUtilServGetResponse
     * recursively */
    ObexUtilServInstType *servInst    = priInst->servInst;
    CsrUint8           *obexPacket  = obexUtilGetObexPacketPointer(priInst, obexPacketLength);

    if (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S)
    {
        /* This is the first OBEX PUT Request packet. E.g. the server
         * is allow to challenge the client */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_REJECTED)
        {
            /* An multi-step OBEX PUT operation has been rejected
             * while Single Response Mode were enabled. */
            if (nextProcessState == CSR_BT_OBEX_UTIL_OBEX_PUT_RESPONSE_PENDING_P_S)
            {
                /* Must check if OBEX PUT Request packets must be
                 * discharge or not. Set nextProcessState to
                 * CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S in order to
                 * make sure that the function
                 * obexUtilServObexPutRequestHandler checks this */
                nextProcessState = CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S;
            }
            else
            { /* All OBEX PUT Request packet must have been discharge as
                 this is an OBEX GET command */
                obexUtilClearSingleResponseModeState(priInst);
            }
        }
#endif
        servInst->putContinueIndHandler = NULL;
#ifdef CSR_BT_INSTALL_OBEX_SRV_GET
        servInst->getContinueIndHandler = NULL;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
        servInst->authenticated         = authenticated;
#else
        CSR_UNUSED(authenticated);
#endif
        *maxDataInResponse              = obexUtilGetMaxObexPacketLength(priInst, withDraw);
    }
    else
    { /* This is not the first OBEX Put Request packet. E.g. the server is not
         allow to challenge the client */
        *maxDataInResponse = obexUtilGetMaxObexPacketLength(priInst, withDrawNoAuth);
    }
    obexUtilSetProcessState(priInst, nextProcessState);
    return (obexPacket);
}

static void obexUtilServObexConnectRequestHandler(ObexUtilPrivateInstType *priInst,
                                                  CsrBool                 authenticated)
{ /* This function is called when the server receives an OBEX Connect Request
     packet. Validate is the packet is valid or not */
    if (obexUtilValidateMaximumObexPacketLength(priInst))
    { /* The maximum OBEX packet lenght is valid, check if Digest Response
         (if needed) */
        CsrUint16  targetHeaderLength = 0;
        CsrUint16  obexPacketLength;
        CsrUint16  maxObexLength;
        CsrUint32  length;
        CsrUint32  count;
        CsrUint8   *target      = NULL;
        CsrUint8 *obexPacket;
        CsrUint16 maxDataInResponse;

#ifdef CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID
        target      = obexUtilSrvGetTargetHeader(priInst, &targetHeaderLength);
#endif
        obexPacket  = obexUtilGetObexPacketPointer(priInst, &obexPacketLength);
        maxDataInResponse = obexUtilSrvGetMaxObexPacketLength(priInst,
                                                                        CSR_BT_OBEX_UTIL_CONNECT_RESPONSE_WITHDRAW,
                                                                        CSR_BT_OBEX_UTIL_CONNECT_RESPONSE_CLI_AUTH_WITHDRAW);

        CsrBtObexObjectLengthHeaderIndex(CSR_BT_OBEX_RESPONSE_CONNECT, obexPacket, &length);
        CsrBtObexCountHeaderIndex(CSR_BT_OBEX_RESPONSE_CONNECT, obexPacket, &count);

        if (priInst->maxPeerObexPacketLength > CSR_BT_MAX_OBEX_SIGNAL_LENGTH)
        { /* The peer device can received more data than the local device can
             send */
            maxObexLength = CSR_BT_MAX_OBEX_SIGNAL_LENGTH;
        }
        else
        { /* The peer device can received the same or less than the local device*/
            maxObexLength = priInst->maxPeerObexPacketLength;
        }

        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_CONNECT_RESPONSE_PENDING_P_S);

        priInst->servInst->connectIndHandler(priInst->profilesOwnInstData,
                                             priInst->deviceAddr,
                                             priInst->btConnId,
                                             maxObexLength,
                                             maxDataInResponse,
                                             length,
                                             count,
                                             targetHeaderLength,
                                             target,
                                             authenticated,
                                             obexUtilChallengeHeaderFind(obexPacket),
                                             obexPacketLength,
                                             obexPacket);
    }
    else
    { /* The maximum OBEX packet length is not valid. The function
         obexUtilServObexResponsePacketHandler sends a OBEX response packet with
         the given response code and CsrPmemFree priInst->incomingObexPacket */
        obexUtilServObexResponsePacketHandler(priInst, CSR_BT_OBEX_CONNECT_OPCODE,
                                              CSR_BT_OBEX_PRECONDITION_FAILED_RESPONSE_CODE);
    }
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static CsrBool obexUtilServObexPutRequestDischargeHandler(ObexUtilPrivateInstType *priInst,
                                                          CsrUint16               bodyLength,
                                                          CsrUint16               bodyOffset,
                                                          CsrUint8              *obexPacket,
                                                          CsrUint16               obexPacketLength)
{
    if (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S)
    {
        /* Must check if the received OBEX PUT Request packet must be
         * discharged or not */
        CsrUint32 commonLength;

        if (bodyOffset > 0)
        {
            /* A body header is included */
            bodyLength = (CsrUint16)(bodyLength + CSR_BT_OBEX_HEADER_LENGTH);
        }

#ifdef CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID
        if (CsrBtObexGetConnectionId(CSR_BT_OBEX_REQUEST, obexPacket, &commonLength))
        {
            /* A connection Identifier header is included */
            commonLength = (CsrUint32)(CSR_BT_OBEX_UTIL_CONNECTION_ID_LENGTH + CSR_BT_OBEX_HEADER_LENGTH);
        }
        else
#endif            
        { /* A connection Identifier Header is not included */
            commonLength = CSR_BT_OBEX_HEADER_LENGTH;
        }

        if (obexPacketLength > commonLength &&
            (obexPacketLength - commonLength) > bodyLength)
        {
            /* A non-body header must be included in the received OBEX
             * PUT Request Packet. E.g. this PUT packet must be the
             * first PUT Packet in this operation */
            obexUtilClearSingleResponseModeState(priInst);
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_PUT_RESPONSE_PENDING_P_S);
        }
        else
        {
            /* This OBEX PUT Request packet must be discharge */
            return (TRUE);
        }
    }
    return (FALSE);
}
#endif

static void obexUtilServObexPutRequestHandler(ObexUtilPrivateInstType *priInst,
                                              CsrBool                 authenticated)
{
    /* This function is called when the server receives an OBEX PUT
     * Request packet */
    ObexUtilServInstType  *servInst = priInst->servInst;

    if (servInst->putIndHandler)
    {
        /* The server support the OBEX Put command. */
        CsrBool     finalBitSet;
        CsrUint16   bodyLength;
        CsrUint16   bodyOffset;
        CsrUint16   maxDataInResponse;
        CsrUint16   obexPacketLength;
        CsrUint8    respVal;
        CsrUint8    *obexPacket = obexUtilServCommonPutGetRequestHandler(priInst,
                                                                       authenticated,
                                                                       CSR_BT_OBEX_UTIL_PUT_RESPONSE_WITHDRAW,
                                                                       CSR_BT_OBEX_UTIL_PUT_RESPONSE_NO_AUTH_WITHDRAW,
                                                                       CSR_BT_OBEX_UTIL_OBEX_PUT_RESPONSE_PENDING_P_S,
                                                                       &obexPacketLength,
                                                                       &maxDataInResponse);

        respVal = CsrBtObexGetBodyHeaderOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &bodyOffset, &bodyLength, &finalBitSet);
         if (CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE & respVal)
        {/* Error: header payload length is bigger the total obex packet length. Abort and reject */
            obexUtilServObexResponsePacketHandler(priInst, CSR_BT_OBEX_PUT_OPCODE,
                                              CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE);
            obexUtilObexAbortIndHandler(priInst);
        }
        else
        {
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            if (!obexUtilServObexPutRequestDischargeHandler(priInst,
                                                            bodyLength,
                                                            bodyOffset,
                                                            obexPacket,
                                                            obexPacketLength))
            {
                /* This is a valid OBEX PUT Request packet */
                if (obexUtilServSingleResponseModeAcceptor(priInst, CSR_BT_OBEX_PUT_OPCODE, obexPacket))
                {
                    /* Single response Mode is enabled */
                    if (finalBitSet)
                    {
                        /* The final OBEX PUT packet is received */
                        obexUtilClearSingleResponseModeState(priInst);
                    }
                    else
                    {
                        /* More packets to come */
                        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_PENDING_P_S);
                    }
                }
#endif

                if (servInst->putContinueIndHandler)
                {
                    /* This cannot be the first OBEX PUT Request message
                     * as the servInst->putContinueIndHandler is
                     * set. E.g. can only be set after the first packet is
                     * received */
                    servInst->putContinueIndHandler(priInst->profilesOwnInstData,
                                                    maxDataInResponse,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                                    servInst->authenticated,
#else
                                                    FALSE,
#endif
                                                    finalBitSet,
                                                    bodyLength,
                                                    bodyOffset,
                                                    obexPacketLength,
                                                    obexPacket);
                }
                else
                {
                    servInst->putIndHandler(priInst->profilesOwnInstData,
                                            maxDataInResponse,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                            servInst->authenticated,
#else
                                            FALSE,
#endif
                                            finalBitSet,
                                            bodyLength,
                                            bodyOffset,
                                            obexPacketLength,
                                            obexPacket);
                }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            }
            else
            { /* This OBEX PUT Request packet must be discharged */
                obexUtilRxQueueDataAckHandler(priInst);                
                CsrPmemFree(obexPacket);
            }
#endif
        }
    }
    else
    {
        /* The profile don't support the OBEX Put command. The
         * function obexUtilSrvInvalidObexPacketResponseHandler sends
         * a OBEX response packet with the given response code and
         * CsrPmemFree priInst->incomingObexPacket */
        obexUtilServObexResponsePacketHandler(priInst, CSR_BT_OBEX_PUT_OPCODE,
                                              CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE);
    }
}

#ifdef CSR_BT_INSTALL_OBEX_SRV_GET
static void obexUtilServObexGetRequestHandler(ObexUtilPrivateInstType *priInst,
                                              CsrBool                 authenticated,
                                              CsrBool                 finalBitSet)
{
/* This function is called when the server receives an OBEX GET Request
   packet. Validate is the packet is valid or not */
    ObexUtilServInstType  *servInst = priInst->servInst;

    if (servInst->getIndHandler)
    { /* The server support the OBEX Get command. */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_SRM_REJECTED_P_S)
        { /* The application has rejected the OBEX GET operation while SRM is
             enable and before a GET Request + Final Bit is received. Discharge
             this packet and find out if an OBEX Response Packet must be issue */
            CsrPmemFree(priInst->incomingObexPacket);
            priInst->incomingObexPacket = NULL;

            if (finalBitSet)
            { /* A GET Request + Final Bit has been issued. The function
                 obexUtilServObexResponsePacketHandler sends a OBEX response
                 packet with the given response code and CsrPmemFree
                 priInst->incomingObexPacket. Must also set
                 priInst->processState to CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S
                 in order to make sure that servInst->abortIndHandler is not
                 call */
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S);
                obexUtilServObexResponsePacketHandler(priInst,
                                                      CSR_BT_OBEX_GET_OPCODE,
                                                      priInst->responseCode);
            }
            else
            { /* Still waiting the a GET Request + Final Bit */
                obexUtilRxQueueDataAckHandler(priInst);
                CsrPmemFree(priInst->incomingObexPacket);
                priInst->incomingObexPacket = NULL;
            }
        }
        else
#endif
        {
            CsrUint16   maxDataInResponse;
            CsrUint16   obexPacketLength;
            CsrUint8  *obexPacket = obexUtilServCommonPutGetRequestHandler(priInst,
                                                                           authenticated,
                                                                           CSR_BT_OBEX_UTIL_GET_RESPONSE_WITHDRAW,
                                                                           CSR_BT_OBEX_UTIL_GET_RESPONSE_NO_AUTH_WITHDRAW,
                                                                           CSR_BT_OBEX_UTIL_OBEX_GET_RESPONSE_PENDING_P_S,
                                                                           &obexPacketLength,
                                                                           &maxDataInResponse);

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            if (obexUtilServSingleResponseModeAcceptor(priInst, CSR_BT_OBEX_GET_OPCODE, obexPacket))
            {
                /* Single Response Mode is enabled */
                if (!finalBitSet)
                {
                    /* The GET request + Final Bit has not been
                     * issued. Set process state to
                     * CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_PENDING_P_S
                     * because the application shall not send a response
                     * unless it want to reject it */
                    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_PENDING_P_S);
                }
            }
#endif
            /* Find out which callback function that must be call */
            if (servInst->getContinueIndHandler)
            {
                /* This cannot be the first OBEX GET Request message
                 * as the servInst->putContinueIndHandler is
                 * set. E.g. can only be set after the first packet is
                 * received */
                servInst->getContinueIndHandler(priInst->profilesOwnInstData,
                                                maxDataInResponse,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                                servInst->authenticated,
#else
                                                FALSE,
#endif                                                
                                                finalBitSet,
                                                obexPacketLength,
                                                obexPacket);
            }
            else
            {
                servInst->getIndHandler(priInst->profilesOwnInstData,
                                        maxDataInResponse,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                        servInst->authenticated,
#else
                                                FALSE,
#endif                                                
                                        finalBitSet,
                                        obexPacketLength,
                                        obexPacket);
            }
        }
    }
    else
    { /* The profile don't support the OBEX Get command. The function
         obexUtilSrvInvalidObexPacketResponseHandler sends a OBEX response
         packet with the given response code and CsrPmemFree
         priInst->incomingObexPacket */
        obexUtilServObexResponsePacketHandler(priInst,
                                              CSR_BT_OBEX_GET_OPCODE,
                                              CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE);
    }
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_SRV_SET_PATH
static void obexUtilServObexSetpathRequestHandler(ObexUtilPrivateInstType *priInst,
                                                  CsrBool                 authenticated)
{ /* This function is called when the server receives an OBEX SETPATH Request
     packet. Validate is the packet is valid or not */
    ObexUtilServInstType  *servInst = priInst->servInst;

    if (servInst->setpathIndHandler &&
        priInst->incomingObexPacketLength >= CSR_BT_OBEX_UTIL_SETPATH_REQUEST_LENGTH)
    { /* The server support the OBEX Setpath command. */
        CsrUint16  obexPacketLength;
        CsrUint8 *obexPacket      = obexUtilGetObexPacketPointer(priInst, &obexPacketLength);
        CsrUint8  flags             = (CsrUint8) (obexPacket[CSR_BT_OBEX_UTIL_SETPATH_FLAGS_INDEX]);
        CsrUint8  constants         = (CsrUint8) (obexPacket[CSR_BT_OBEX_UTIL_SETPATH_CONSTANTS_INDEX]);
        CsrUint16 maxDataInResponse = obexUtilGetMaxObexPacketLength(priInst,
                                                                     CSR_BT_OBEX_UTIL_SETPATH_RESPONSE_WITHDRAW);

        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_SETPATH_RESPONSE_PENDING_P_S);

        servInst->setpathIndHandler(priInst->profilesOwnInstData,
                                    maxDataInResponse, flags,
                                    constants, authenticated,
                                    obexPacketLength,
                                    obexPacket);
    }
    else
    { /* The profile don't support the OBEX Setpath command. The function
         obexUtilSrvInvalidObexPacketResponseHandler sends a OBEX response
         packet with the given response code and CsrPmemFree
         priInst->incomingObexPacket */
        obexUtilServObexResponsePacketHandler(priInst, CSR_BT_OBEX_SET_PATH_OPCODE,
                                              CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE);
    }
}
#endif

static void obexUtilServObexAbortRspHandler(ObexUtilPrivateInstType *priInst)
{
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE)
    { /* SRM is enable - OBEX over L2cap is used. Make sure that the l2cap 
         TX buffer is flushed before the OBEX ABORT RESPONSE message is sent as
         the TX buffer may be full - A GET operation is running */
        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_ABORT_RESPONSE_PENDING_P_S);
        CsrBtCmL2caDataAbortReqSend(priInst->btConnId);
    }
    else
#endif 
    { /* The OBEX ABORT Response packet can be sendt right away */
        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S);
        obexUtilServObexResponsePacketHandler(priInst,
                                              CSR_BT_OBEX_ABORT_OPCODE,
                                              CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
    }
}

static void obexUtilServObexAbortRequestHandler(ObexUtilPrivateInstType *priInst)
{
    obexUtilObexAbortIndHandler(priInst);

    if (priInst->transmittingData)
    { /* Is in the middle of sending an OBEX Response packet. The OBEX ABORT
         Response packet must first be sent after this ongoing OBEX Response
         packet is sent */
        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_ABORT_RESPONSE_PENDING_P_S);
    }
    else
    { /* Check if the OBEX ABORT Response packet can be sendt right away or
         the L2cap TX buffer shall be flush first */
        obexUtilServObexAbortRspHandler(priInst);
    }
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static void obexUtilServObexActionRequestHandler(ObexUtilPrivateInstType *priInst,
                                                 CsrBool                 authenticated)
{ /* This function is called when the server receives an OBEX ACTION Request
     packet. Validate is the packet is valid or not */
    CsrBtObexResponseCode responseCode;
    ObexUtilServInstType  *servInst = priInst->servInst;

    if (CSR_BT_CONN_ID_IS_L2CA(priInst->btConnId))
    {
        /* The peer device is allowed to send an Action command
         * because L2CAP is used as the transport */
        if (servInst->actionIndHandler)
        {
            /* The server support the OBEX Action command. */
            CsrUint8              actionId;
            CsrUint32             permissions;
            CsrUint16             nameOffset;
            CsrUint16             destNameOffset;

            if (obexUtilServValidateObexActionRequestPacket(priInst,
                                                            &actionId,
                                                            &permissions,
                                                            &nameOffset,
                                                            &destNameOffset))
            { /* The OBEX ACTION Request Packet is correct */
                CsrUint16  obexPacketLength;
                CsrUint8 *obexPacket      = obexUtilGetObexPacketPointer(priInst,
                                                                         &obexPacketLength);

                CsrUint16 maxDataInResponse = obexUtilGetMaxObexPacketLength(priInst,
                                                                             CSR_BT_OBEX_UTIL_ACTION_RESPONSE_WITHDRAW);

                responseCode                = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

                obexUtilSetProcessState(priInst,
                                        CSR_BT_OBEX_UTIL_OBEX_ACTION_RESPONSE_PENDING_P_S);

                servInst->actionIndHandler(priInst->profilesOwnInstData,
                                           maxDataInResponse,
                                           actionId,
                                           nameOffset,
                                           destNameOffset,
                                           permissions,
                                           authenticated,
                                           obexPacketLength,
                                           obexPacket);
            }
            else
            { /* The received OBEX ACTION Request packet do not follow the
                 specifikation */
                responseCode = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
            }
        }
        else
        { /* The profile don't support the OBEX Action command. */
            responseCode = CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE;
        }
    }
    else
    { /* RFCOMM is used as the transport, e.g. the OBEX ACTION operation is not
         supported */
        responseCode = CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE;
    }

    if (responseCode != CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    { /* The OBEX Action command is rejected. The function
         obexUtilSrvInvalidObexPacketResponseHandler sends a OBEX response packet
         with the given response code and CsrPmemFree priInst->incomingObexPacket */
        obexUtilServObexResponsePacketHandler(priInst,
                                              servInst->opCode,
                                              responseCode);
    }
}
#endif

static void obexUtilServObexDisconnectRequestHandler(ObexUtilPrivateInstType *priInst)
{ /* This function is called when the server receives an OBEX DISCONNECT Request
     packet. Validate is the packet is valid or not */
    CsrUint16  obexPacketLength;
    CsrUint8 *obexPacket            = obexUtilGetObexPacketPointer(priInst, &obexPacketLength);
    
    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_CONNECTED_P_S);
    
    obexUtilServObexResponsePacketHandler(priInst,
                                          CSR_BT_OBEX_DISCONNECT_OPCODE,
                                          CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);

    priInst->servInst->disconnectIndHandler(priInst->profilesOwnInstData,
                                            priInst->deviceAddr,
                                            CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                            CSR_BT_SUPPLIER_OBEX_PROFILES,
                                            obexPacketLength,
                                            obexPacket);
}


static void obexUtilSrvIncomingObexPacketHandler(ObexUtilPrivateInstType * priInst)
{
    ObexUtilServInstType  *servInst = priInst->servInst;

    servInst->opCode                = CSR_BT_OBEX_GET_REQUEST_OPCODE(priInst->incomingObexPacket);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
    servInst->deleteDigestChallenge = TRUE;
#endif
    if (priInst->incomingObexPacketLength > priInst->maxLocalObexPacketLength)
    {
        /* The peer device do not follow the OBEX specification as the
         * incoming OBEX packet is larger than the size the local
         * device can receive. The function
         * obexUtilServObexResponsePacketHandler sends a OBEX response
         * packet with the given response code and CsrPmemFree
         * priInst->incomingObexPacket */
        obexUtilServObexResponsePacketHandler(priInst,
                                              servInst->opCode,
                                              CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE);
    }
    else
    {
        CsrBool authenticated=FALSE; /* Init needed when CHALLENGE is disabled. */

        /* Makes sure than the OBEX connectionId Header is only return in a
           OBEX Connect Response Packet */
#ifdef CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID
        priInst->connectionId = CSR_BT_UNUSED_CONNECTION_ID;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
        if (obexUtilSrvObexAuthenticateChallengeHandler(priInst, servInst->opCode, &authenticated))
        {
#endif            
            if (!servInst->obexConnected && servInst->opCode != CSR_BT_OBEX_CONNECT_OPCODE)
            { /* This ensure that the server only accept OBEX PUT/GET/SETPATH/
                 ABORT/ACTION/DISCONNECT Request packets when it is OBEX connected  */    
                servInst->opCode = CSR_BT_OBEX_INVALID_OPCODE;
            }

            /* Need to find out what we must do with the incoming OBEX packet       */
            switch (servInst->opCode)
            {
                case CSR_BT_OBEX_CONNECT_OPCODE:
                {
                    /* An OBEX CONNECT Request packet is received */
                    obexUtilServObexConnectRequestHandler(priInst, authenticated);
                    break;
                }
                case CSR_BT_OBEX_PUT_OPCODE:
                case CSR_BT_OBEX_PUT_OPCODE | CSR_BT_OBEX_FINAL_BIT_MASK:
                {
                    /* An OBEX Put Request packet is received */
                    obexUtilServObexPutRequestHandler(priInst, authenticated);
                    break;
                }
#ifdef CSR_BT_INSTALL_OBEX_SRV_GET
                case CSR_BT_OBEX_GET_OPCODE:
                {
                    /* An OBEX Get Request packet is received, and
                     * the final Bit is not set */
                    obexUtilServObexGetRequestHandler(priInst, authenticated, FALSE);
                    break;
                }
                case CSR_BT_OBEX_GET_OPCODE | CSR_BT_OBEX_FINAL_BIT_MASK:
                {
                    /* An OBEX Get Request packet is received, and
                     * the final Bit is set */
                    obexUtilServObexGetRequestHandler(priInst, authenticated, TRUE);
                    break;
                }
#endif                
#ifdef CSR_BT_INSTALL_OBEX_SRV_SET_PATH
                case CSR_BT_OBEX_SET_PATH_OPCODE:
                {
                    /* An OBEX Setpath Request packet is received */
                    obexUtilServObexSetpathRequestHandler(priInst, authenticated);
                    break;
                }
#endif                
                case CSR_BT_OBEX_ABORT_OPCODE:
                {
                    /* An OBEX Abort Request packet is received */
                    obexUtilServObexAbortRequestHandler(priInst);
                    break;
                }
                case CSR_BT_OBEX_DISCONNECT_OPCODE:
                {
                    /* An OBEX Disconnect Request packet is
                     * received. */
                    obexUtilServObexDisconnectRequestHandler(priInst);
                    servInst->obexConnected = FALSE;
                    break;
                }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                case CSR_BT_OBEX_ACTION_OPCODE:
                {
                    /* An OBEX Action Request packet is received */
                    obexUtilServObexActionRequestHandler(priInst, authenticated);
                    break;
                }
#endif
                default:
                {
                     /* obexUtilServObexResponsePacketHandler sends
                      * a OBEX response packet with the given
                      * response code and CsrPmemFree
                      * priInst->incomingObexPacket                     */
                    if (servInst->opCode == CSR_BT_OBEX_INVALID_OPCODE)
                    { /* No OBEX connection has been made, reject 
                         this request                                   */
                        obexUtilServObexResponsePacketHandler(priInst,
                                                              servInst->opCode,
                                                              CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE);
                        
                    }
                    else
                    { /* Could not understand the opcode    */
                        obexUtilServObexResponsePacketHandler(priInst,
                                                              servInst->opCode,
                                                              CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE);
                    }
                    break;
                }
            }
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
        }
        else
        {
            /* The OBEX authentification procedure went wrong, the
             * function obexUtilSrvObexAuthenticateChallengeHandler
             * makes sure that the server has send an OBEX response
             * packet */
            ;
        }
#endif        
    }
}

static void obexUtilIncomingObexPacketHandler(ObexUtilPrivateInstType * priInst)
{
    /* An entire OBEX packet is received, find out what to do with it */
    if (priInst->cliInst)
    {
        /* Represent a client, extract the responseCode */
        obexUtilCliIncomingObexPacketHandler(priInst);
    }
    else
    {
        /* Represent a server, extract the Opcode */
        obexUtilSrvIncomingObexPacketHandler(priInst);
    }
}

static void obexUtilTransportDataIndFirstFragment(ObexUtilPrivateInstType * priInst,
                                                  CsrUint16                payloadLength,
                                                  CsrUint8                 * payload)
{
    priInst->incomingObexPacketLength = (CsrUint16)(CSR_BT_OBEX_GET_REQUEST_LENGTH(payload));
    priInst->incomingObexPacketIndex  = payloadLength;

    if (priInst->incomingObexPacketLength > priInst->maxLocalObexPacketLength)
    {
        /* The peer device does not follow the OBEX specification, as
         * the incoming OBEX packet is larger than the size the local
         * device can received */
        if (priInst->cliInst)
        {
            /* Represent a client, release the transport
             * connection. No need to send acks */
            obexUtilReleaseTransportHandler(priInst,
                                            CSR_BT_RESULT_CODE_OBEX_INCOMING_OBEX_PACKET_TOO_LARGE,
                                            CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
        }
        else
        {
            /* Represent a server. Save the first 3 bytes (one byte
             * opcode + two byte length) */
            priInst->incomingObexPacket = (CsrUint8 *) CsrPmemAlloc(CSR_BT_OBEX_HEADER_LENGTH);
            CsrMemCpy(priInst->incomingObexPacket, payload, CSR_BT_OBEX_HEADER_LENGTH);

            if (payloadLength < priInst->incomingObexPacketLength)
            {
                /* Must received the entire OBEX packet before an OBEX
                 * response can we send */
                ;
            }
            else
            {
                /* The entire OBEX packet is received, the function
                 * obexUtilSrvIncomingObexPacketHandler will send a OBEX
                 * response packet with response code
                 * CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE */
                obexUtilSrvIncomingObexPacketHandler(priInst);
            }
        }
        CsrPmemFree(payload);
    }
    else
    {
        /* The length of the incoming OBEX packet is valid */
        if (payloadLength < priInst->incomingObexPacketLength)
        {
            /* Only a fragment of the OBEX packet is received */
            priInst->incomingObexPacket = (CsrUint8 *) CsrPmemAlloc(priInst->incomingObexPacketLength);
            CsrMemCpy(priInst->incomingObexPacket, payload, payloadLength);
            CsrPmemFree(payload);
        }
        else
        {
            /* The entire OBEX packet is received */
            priInst->incomingObexPacket = payload;
            obexUtilIncomingObexPacketHandler(priInst);
        }
    }

    if (priInst->servInst)
    {
        /* The first part or a entire OBEX Request is received, make sure that
           the transport connection goes to CSR_BT_ACTIVE_MODE */
        obexUtilLowpowerHandler(priInst);
    }
}

static void obexUtilTransportDataIndNextFragment(ObexUtilPrivateInstType * priInst,
                                                 CsrUint16                payloadLength,
                                                 CsrUint8                 * payload)
{
    CsrUint16 tmplength = (CsrUint16)(priInst->incomingObexPacketIndex + payloadLength);

    if (tmplength > priInst->incomingObexPacketLength)
    {
        /* Cannot CsrMemCpy the entire payload into the incomingObexPacket ptr */
        tmplength = (CsrUint16)(priInst->incomingObexPacketLength - priInst->incomingObexPacketIndex);
    }
    else
    {
        /* Can CsrMemCpy the entire payload into the incomingObexPacket ptr */
        tmplength = payloadLength;
    }

    if (priInst->incomingObexPacketLength <= priInst->maxLocalObexPacketLength)
    {
        CsrMemCpy(&(priInst->incomingObexPacket[priInst->incomingObexPacketIndex]), payload, tmplength);
    }
    else
    {
        /* This only occurres if the peer client trying to send an
         * OBEX packet there are to large to our OBEX Server. The
         * reason for doing like this is that the entire OBEX packet
         * must be received, before an OBEX response packet with the
         * response code CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE can be
         * send to the client */
        ;
    }

    priInst->incomingObexPacketIndex = (CsrUint16)(priInst->incomingObexPacketIndex + tmplength);
    CsrPmemFree(payload);

    if (priInst->incomingObexPacketIndex < priInst->incomingObexPacketLength)
    {
        /* The entire OBEX packet is not received. Wait for the next fragment */
        ;
    }
    else
    {
        /* The entire OBEX packet is received */
        obexUtilIncomingObexPacketHandler(priInst);
    }
}

static void obexUtilTransportDataIndHandler(ObexUtilPrivateInstType * priInst,
                                            CsrUint16                payloadLength,
                                            CsrUint8                 * payload)
{
   
    /* This function is responsible of gathering all the tranport
     * packet into one OBEX packet */
    if (priInst->incomingObexPacket)
    {
        /* The OBEX packet is being sent in multiple fragments */
        obexUtilTransportDataIndNextFragment(priInst, payloadLength, payload);
    }
    else
    {
        /* First data fragment, retrieve the OBEX packet length */
        obexUtilTransportDataIndFirstFragment(priInst, payloadLength, payload);
    }
}

static void obexUtilTransportDataCfmHandler(ObexUtilPrivateInstType * priInst,
                                            CsrBool                 flushL2caBuff)
{
    /* The previous data was successfully sent over the transport channel */
    priInst->transmittingData = FALSE;

    if (priInst->currentHeaderTypeIndex  < priInst->lastHeaderTypeIndex)
    {
        /* Still some OBEX data that need to be sent. Build and send
         * the next transport packet */
        obexUtilBuildTransportPacket(priInst, priInst->headerSplit);
    }
    else
    {
        /* All OBEX data has been sent. Determine the role this OBEX
         * instance represent */
        if (priInst->cliInst)
        {
            /* Represent a client */
            if (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_ABORT_REQUEST_PENDING_P_S)
            {
                /* Were waiting for the complete OBEX Request packet
                 * to be sent.  Build and sent the OBEX ABORT Request
                 * packet */
                ObexUtilCliInstType * cliInst = priInst->cliInst;
                CsrUint8 numOfRes = (cliInst->abortIncomingObexPacket != NULL)? 1 : 2; /* If we have already received a response, only expect one more. */
                obexUtilObexAbortRequestHandler(priInst, 
                                                cliInst->abortDescription, 
                                                numOfRes, 
                                                flushL2caBuff);
                cliInst->abortDescription = NULL;
            }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            else
            {
                if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE &&
                    priInst->headerTypeTable[0] != CSR_BT_OBEX_UTIL_GET_FINAL_REQUEST_OPERATION)
                {
                    /* Single Response Mode is enabled. This can only happen
                     * during a OBEX PUT or GET operation. Note the for
                     * CSR_BT_OBEX_UTIL_GET_FINAL_REQUEST_OPERATION because GET
                     * operations with SRM enabled shall not send a GET response
                     * until the GET request + Final Bit has been issued. Until
                     * this point is reached, the Client's GET request phase is
                     * ongoing and multiple unacknowledged GET requests may be
                     * issued. Once the GET request + Final bit has been issued,
                     * the GET request phase is complete and no further GET
                     * requests shall be issued (unless triggered by a SRMP
                     * header). After the GET request phase is complete, the
                     * Server's GET responses shall begin. */
                    if (priInst->obexOperationIndex == CSR_BT_OBEX_UTIL_PUT_REQUEST_RESULT_HANDLER )
                    { /* A PUT operation is running */
                        obexUtilObexPutRequestResultFinalHandler(priInst, TRUE);
                    }
#ifdef CSR_BT_INSTALL_OBEX_CLI_GET
                    else
                    { /* A GET operation is running */
                        obexUtilObexGetRequestResultHandler(priInst);
                    }
#endif                    
                }
                else
                {
                    /* Waiting for the server to response */
                    ;
                }
            }
#endif
        }
        else
        {
            /* Represent a server */
            if (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_ABORT_RESPONSE_PENDING_P_S)
            {
                /* Were waiting for the complete OBEX Response packet
                 * to be sent. Check if the OBEX ABORT Response packet 
                 * can be sendt right away or the L2cap TX buffer shall 
                 * be flush first */
                obexUtilServObexAbortRspHandler(priInst);
            }
#if defined(CSR_BT_INSTALL_OBEX_GOEP_20) && defined(CSR_BT_INSTALL_OBEX_SRV_GET)
            else
            {
                if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE &&
                    priInst->servInst->opCode == (CSR_BT_OBEX_GET_OPCODE | CSR_BT_OBEX_FINAL_BIT_MASK))
                {
                    /* Ask the application for the next packet right away */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                    obexUtilServObexGetRequestHandler(priInst,
                                                      priInst->servInst->authenticated,
                                                      TRUE);
#else
                    obexUtilServObexGetRequestHandler(priInst,
                                                      FALSE,
                                                      TRUE);
#endif                                                      
                }
                else
                { /* Waiting for a new OBEX Request packet */
                    ;
                }
            }
#endif
        }
    }
}

static void obexUtilCliTransportDisconnectIndHandler(ObexUtilPrivateInstType *priInst,
                                                     CsrBtReasonCode         reasonCode,
                                                     CsrBtSupplier           reasonSupplier)
{
    switch (priInst->processState)
    {
        case CSR_BT_OBEX_UTIL_BT_DISCONNECT_NORMAL_REQUEST_P_S:
            {
                if (priInst->preProcessState == CSR_BT_OBEX_UTIL_OBEX_CONNECT_REQUEST_P_S)
                { /* The transport connection is released after an OBEX Server has
                     rejected an OBEX Connect Request */
                    obexUtilObexConnectRequestResultHandler(priInst);
                }
                else
                { /* The transport connection is released after the OBEX
                     connection is disconnected with
                     CSR_BT_OBEX_SUCCESS_RESPONSE_CODE */
                    obexUtilObexDisconnectRequestResultHandler(priInst);
                }
                break;
            }
        case CSR_BT_OBEX_UTIL_OBEX_CONNECT_REQUEST_P_S:
            { /* The transport connection has been released while the OBEX
                 Connect Request procedure is running. */
                priInst->resultCode     = (CsrBtResultCode) reasonCode;
                priInst->resultSupplier = reasonSupplier;
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
                obexUtilObexConnectRequestResultHandler(priInst);
                break;
            }
        case CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECT_REQUEST_P_S:
            { /* The l2cap transport connection has been release in order to
                 cancel the OBEX Connect procedure */
                priInst->resultCode     = CSR_BT_RESULT_CODE_OBEX_CONNECT_CANCELLED;
                priInst->resultSupplier = CSR_BT_SUPPLIER_OBEX_PROFILES;
                obexUtilObexConnectRequestResultHandler(priInst);
                break;
            }
        case CSR_BT_OBEX_UTIL_OBEX_DISCONNECT_REQUEST_P_S:
            { /* The transport connection has been released while the OBEX
                 Connect Disconnect procedure is running. */
                priInst->resultCode     = (CsrBtResultCode) reasonCode;
                priInst->resultSupplier = reasonSupplier;
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
                obexUtilObexDisconnectRequestResultHandler(priInst);
                break;
            }
        default:
            { /* The transport connection is release either by the local or
                 remote device without running the OBEX Disconnect procedure */
                if (priInst->cmState == CSR_BT_OBEX_UTIL_BT_RFC_CONNECTED_S 
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                    || priInst->cmState == CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTED_S
#endif            
                    )
                {
                    priInst->resultCode     = (CsrBtResultCode) reasonCode;
                    priInst->resultSupplier = reasonSupplier;
                }
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
                obexUtilObexDisconnectRequestResultHandler(priInst);
                break;
            }
    }
}

static void obexUtilSrvTransportDisconnectIndCleanup(ObexUtilPrivateInstType *priInst)
{ /* Cleanup the priInst used by an OBEX server connection */
    obexUtilDeallocateRxQueueDataInst(priInst);
    obexUtilInitRxQueueDataInst(priInst);
    obexUtilDeallocatePrivateInstHeaderPointers(priInst, TRUE);
    obexUtilInitPrivateInstHeaderPointers(priInst, TRUE);
    obexUtilResetLpSuperVisionTimer(priInst);
    priInst->responseCode                    = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
    priInst->resultCode                      = CSR_BT_RESULT_CODE_CM_SUCCESS;
    priInst->resultSupplier                  = CSR_BT_SUPPLIER_CM;
    priInst->actualLinkMode                  = CSR_BT_ACTIVE_MODE;
    priInst->requestedLinkMode               = CSR_BT_ACTIVE_MODE;
    priInst->maxPeerObexPacketLength         = 0xFFFF;
    priInst->transmittingData                = FALSE;
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    priInst->servInst->deleteDigestChallenge = TRUE;
#endif
    obexUtilClearSingleResponseModeState(priInst);
}

static void obexUtilSrvTransportDisconnectIndConnectStateHandler(ObexUtilPrivateInstType *priInst,
                                                                 CsrBtReasonCode         reasonCode,
                                                                 CsrBtSupplier           reasonSupplier)
{
    obexUtilSrvTransportDisconnectIndCleanup(priInst);
    priInst->cmState = CSR_BT_OBEX_UTIL_BT_ACTIVATED_S;

    switch(priInst->processState)
    {
        case CSR_BT_OBEX_UTIL_BT_CONNECTED_P_S:
            { /* The transport connection is release either after an OBEX
                 DISCONNECT response packet or after an OBEX CONNECT response
                 with error is send, or before an OBEX CONNECT Request packet
                 is received */
                obexUtilSetDeviceConnectable(priInst);
                break;
            }
        case CSR_BT_OBEX_UTIL_OBEX_CONNECT_RESPONSE_PENDING_P_S:
            { /* The connection is released while waiting for a OBEX Connect
                 Response. Wait for this reponse. Note the local server is not
                 set connectable before is calls ObexUtilServConnectResponse */
                priInst->resultCode     = (CsrBtResultCode) reasonCode;
                priInst->resultSupplier = reasonSupplier;
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_PENDING_CONNECTABLE_P_S);
                break;
            }
        default:
            { /* The connection is release abnormal, e.g. an OBEX Disconnect
                 Request packet is not sent, inform the application and tell
                 the CM that we want to accept a new one incoming connection */
                priInst->servInst->disconnectIndHandler(priInst->profilesOwnInstData,
                                                        priInst->deviceAddr,
                                                        reasonCode,
                                                        reasonSupplier,
                                                        0,
                                                        NULL);

                obexUtilSetDeviceConnectable(priInst);
                break;
            }
    }
}

static void obexUtilTransportDisconnectIndHandler(ObexUtilPrivateInstType *priInst,
                                                  CsrBtReasonCode         reasonCode,
                                                  CsrBtSupplier           reasonSupplier,
                                                  CsrUint8                transportType)
{
    if (priInst->cliInst)
    { /* Represent a client. Reset the ObexCmdSuperVisionTimer */
        obexUtilResetObexCmdSuperVisionTimer(priInst);
        obexUtilCliTransportDisconnectIndHandler(priInst, reasonCode, reasonSupplier);
    }
    else
    { /* Represent a server */
        ObexUtilServInstType  *servInst = priInst->servInst;
        servInst->obexConnected         = FALSE;
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if (transportType == CSR_BT_OBEX_UTIL_L2CAP_TRANSPORT)
        {
            /* The l2cap transport connection is release */
            servInst->l2caSubProcessState = CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S;
            priInst->btConnId = CSR_BT_CONN_ID_INVALID;
        }
        else
#endif
        { /* The rfcomm transport connection is release */
            servInst->rfcSubProcessState  = CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S;
            priInst->btConnId = CSR_BT_CONN_ID_INVALID;
        }

        switch (priInst->cmState)
        {
            case CSR_BT_OBEX_UTIL_BT_RFC_CONNECTED_S:
                {
                    if (transportType == CSR_BT_OBEX_UTIL_L2CAP_TRANSPORT)
                    { /* The l2cap transport connection is release because a rfcomm
                         transport is being used as the OBEX connection. */
                        ;
                    }
                    else
                    { /* Either the rfcomm transport connection is released by the
                         peer OBEX client or it is released abnormal */
                        obexUtilSrvTransportDisconnectIndConnectStateHandler(priInst,
                                                                             reasonCode,
                                                                             reasonSupplier);
                    }
                    break;
                }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            case CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTED_S:
                {
                    if (transportType == CSR_BT_OBEX_UTIL_RFC_TRANSPORT)
                    { /* The rfcomm transport connection is release because a l2cap
                         transport is being used as the OBEX connection. */
                    }
                    else
                    { /* Either the l2cap transport connection is released by the
                         peer OBEX client or it is released abnormal */
                        obexUtilSrvTransportDisconnectIndConnectStateHandler(priInst,
                                                                             reasonCode,
                                                                             reasonSupplier);
                    }
                    break;
                }
#endif
            case CSR_BT_OBEX_UTIL_BT_ACTIVATED_S:
                { /* A transport connection is release. Check if both the l2cap
                     instance and rfcomm instance is ready to set the local
                     device connectable */
                    obexUtilSetDeviceConnectable(priInst);
                    break;
                }
            case CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S:
                { /* The server has released the transport connection. The function
                     obexUtilNonConnectableHandler checks if it is because it want
                     to be non-connectable or because if it want to deactivate */
                    obexUtilSrvTransportDisconnectIndCleanup(priInst);
                    obexUtilNonConnectableHandler(priInst);
                    break;
                }
            default:
                {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    if (transportType == CSR_BT_OBEX_UTIL_RFC_TRANSPORT)
                    {
                        CsrGeneralException("OBEX_UTIL_SERV", (CsrUint16)priInst->cmState,
                                            CSR_BT_CM_DISCONNECT_IND, "CSR_BT_CM_DISCONNECT_IND received in invalid cm state");
                    }
                    else
                    {
                        CsrGeneralException("OBEX_UTIL_SERV", (CsrUint16)priInst->cmState,
                                            CSR_BT_CM_L2CA_DISCONNECT_IND, "CSR_BT_CM_L2CA_DISCONNECT_IND received in invalid cm state");
                    }
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
                    break;
                }
        }
    }
}

static CsrBool obexUtilModeChangeIndHandler(ObexUtilPrivateInstType * priInst, CsrUint8 mode)
{
    if (priInst->processState           == CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S &&
        priInst->lpSuperVisionTimerId   == 0                                   &&
        priInst->actualLinkMode         == CSR_BT_SNIFF_MODE                   &&
        priInst->requestedLinkMode      == CSR_BT_SNIFF_MODE                   &&
        mode                            == CSR_BT_ACTIVE_MODE)
    { /* The transport connection is set in CSR_BT_ACTIVE_MODE while OBEX is
         idle and while the lpSuperVisionTimer is deactivated. Activate the
         lpSuperVisionTimer */
        obexUtilActivateLpSuperVisionTimer(priInst);
    }

    priInst->actualLinkMode = mode;

#ifdef CSR_BT_INSTALL_CM_PARK_MODE
    if (mode == CSR_BT_PARK_MODE)
    { /* The peer device has set the connection into park mode. Return TRUE to
         indicate that the calling function must request to goto
         CSR_BT_ACTIVE_MODE */
        return (TRUE);
    }
    else
#endif
    { /* The device is not set into park mode. Return FALSE to indicate that
         calling function shall do nothing */
        return (FALSE);
    }
}


/*------------------------------------------------------------------------------*
 * Static util functions only used by the OBEX connect procedure                *
 *------------------------------------------------------------------------------*/
static CsrBool obexUtilAssignLocalServerChannel(CmnCsrBtLinkedListStruct * sdpTag)
{ /* This function find out if the calling profile allready have assigned a
     local server channel, or if it want to assign one doing the connect
     procedure. In the case where the local server channel is assign doing the
     connect procedure, this library must unregister the local server channel
     when the RFCOMM connection is released */
    CsrUint16 channel  = CSR_BT_NO_SERVER;

    (void)CsrBtUtilSdrGetLocalServerChannel(sdpTag, 0, &channel);

    if (channel == CSR_BT_NO_SERVER)
    { /* The profile has not assign a local server channel. */
        return (TRUE);
    }
    else
    { /* The profile has allready assigned a local servel channel */
        return (FALSE);
    }
}

static CsrUint8 * obexUtilObexHeaderPointer(CsrUint8 headerType,
                                            CsrUint16 headerLength,
                                            CsrUint8  *headerPayload)
{ /* Build a common OBEX header, e.g 1 byte opcode plus two bytes length plus
     header 'payload' */
    CsrUint8 * header       = (CsrUint8 *) CsrPmemAlloc(headerLength);
    CsrUint8 * headerTemp   = header;

    headerTemp = obexUtilBuildCommonOpcodeHeaderHandler(headerType, headerLength, headerTemp);
    CsrMemCpy(headerTemp, headerPayload, (headerLength - CSR_BT_OBEX_HEADER_LENGTH));
    return (header);
}

static CsrUint8 * obexUtilBuildObexTypePointer(const CsrUint8 * type, CsrUint16 *typeLength)
{ /* This function check if the calling profile is using the type header or
     not */
    if (type)
    { /* The TYPE header must be added */
        *typeLength  = obexUtilReturnTypeHeaderLength((CsrUint8 *)type);
        return(obexUtilObexHeaderPointer(CSR_BT_OBEX_TYPE_HEADER, *typeLength, (CsrUint8 *) type));
    }
    else
    { /* The type header shall not be included */
        *typeLength = 0;
        return (NULL);
    }
}

static void obexUtilObexConnectRequestHandler(ObexUtilPrivateInstType * priInst,
                                              CsrUint8                cmState)
{ /* This function start building and sending the OBEX Connect Request packet */
    CsrTime  obexConnectSuperVisionTimeout = CSR_BT_OBEX_CONNECT_TIMEOUT;
    priInst->cmState                        = cmState;


#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    if (priInst->digestChallengeHeader)
    {
        obexConnectSuperVisionTimeout = CSR_BT_OBEX_CONNECT_WITH_AUTH_TIMEOUT;
    }
#endif
    /* Start the ObexCmdSuperVisionTimer in order to ensure that that
       transport connection is release if the server do not response
       the OBEX Connect Request message */
    obexUtilActivateObexCmdSuperVisionTimer(priInst,
                                            obexConnectSuperVisionTimeout,
                                            CSR_BT_OBEX_CONNECT_OPCODE);

    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_CONNECT_REQUEST_P_S);
    obexUtilBuildTransportPacket(priInst, FALSE);
}

static void obexUtilCliSdcSelectServiceHandler(void                     *instData,
                                               void                     *cmSdcRfcInstData,
                                               CsrBtDeviceAddr          deviceAddr,
                                               CsrUint8            serverChannel,
                                               CsrUint16                entriesInSdpTaglist,
                                               CmnCsrBtLinkedListStruct *sdpTagList)
{
    CsrBtUuid32    service;
    CsrUint16      bchsResult, bchsTask, bchsTaskResult;
    CsrUintFast16      sdpRecordIndex;
    CsrUintFast16               idx        = 0;
    ObexUtilPrivateInstType *priInst   = instData;
    CsrUint16  *serviceHandleIndexList = CsrPmemAlloc(sizeof(CsrUint16) * entriesInSdpTaglist);

    for (sdpRecordIndex = 0; sdpRecordIndex < entriesInSdpTaglist; sdpRecordIndex++)
    {
        if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList, sdpRecordIndex, &service,
                                                  &bchsResult, &bchsTask, &bchsTaskResult))
        {
            if (bchsResult == SDR_SDC_SEARCH_SUCCESS)
            {
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                CsrUint32 psm;
#endif
                serviceHandleIndexList[idx] = (CsrUint16)sdpRecordIndex;
                idx++;
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                /* If results contain the OBEX-over-L2CAP PSM, then use that
                 * procedure instead */
                if(CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(sdpTagList,
                                                                          sdpRecordIndex,
                                                                          CSR_BT_OBEX_L2CAP_PSM_ATTRIBUTE,
                                                                          &psm))
                {
                    CsrUint16           dataLen;
                    ObexUtilCliInstType *cliInst = priInst->cliInst;
                    CsrUint8            *data    = CsrBtUtilBllGetDataPointerEx(sdpTagList, sdpRecordIndex, &dataLen);
                    cliInst->sdpTag              = CsrBtUtilBllCreateNewEntry(NULL, data, dataLen);
                    CsrBtUtilBllSetDataPointerEx(sdpTagList, sdpRecordIndex, NULL, 0);

                    priInst->cliInst->remotePsm  = (CsrUint16)(psm & 0xFFFF);
                    CsrPmemFree(serviceHandleIndexList);
                    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_L2CA_REGISTER_PSM_P_S);
                    CsrBtUtilRfcConCancel(priInst, cmSdcRfcInstData);
                    return;
                }
#endif
            }
        }
    }

    /* The remote OBEX server don't support OBEX over l2cap */
    CsrBtUtilRfcConSetServiceHandleIndexList(priInst, cmSdcRfcInstData, serviceHandleIndexList,(CsrUint16) idx);
}

static void obexUtilCmRfcConnectResultHandler(void                     *instData,
                                              CsrUint8                 localServerCh,
                                              CsrUint32                 btConnId,
                                              CsrBtDeviceAddr          deviceAddr,
                                              CsrUint16                maxFrameSize,
                                              CsrBool                  validPortPar,
                                              RFC_PORTNEG_VALUES_T     portPar,
                                              CsrBtResultCode          resultCode,
                                              CsrBtSupplier            resultSupplier,
                                              CmnCsrBtLinkedListStruct *sdpTag)
{ /* This function is called whenever the attempt to establihed an rfcomm
     function is finished */
    ObexUtilPrivateInstType * priInst = instData;
    ObexUtilCliInstType * cliInst     = priInst->cliInst;

    if (obexUtilCmMsgResultHandler(resultCode, resultSupplier))
    {
        /* An RFCOMM connection is established, start sending the OBEX
         * Connect Request packet. */
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL,
                                         deviceAddr,
                                         btConnId);

        priInst->btConnId                  = btConnId;
        priInst->maxTransportPacketLength   = maxFrameSize;
        cliInst->sdpTag                     = sdpTag;
        priInst->localServerCh              = localServerCh;
        obexUtilObexConnectRequestHandler(priInst, CSR_BT_OBEX_UTIL_BT_RFC_CONNECTED_S);
    }
    else
    {
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if (priInst->processState == CSR_BT_OBEX_UTIL_BT_L2CA_REGISTER_PSM_P_S)
        { /* The RFCOMM connect procedure were cancel by this library because
             the peer device supports OBEX over l2cap. Register a Dynamic PSM
             that support enhanced retransmission mode */
            ObexUtilInstType * inst = priInst->obexUtilOwnInstData;
            priInst->cmState        = CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTING_S;

            CsrBtCmContextl2caRegisterReqSend(inst->phandle,
                                              CSR_BT_ASSIGN_DYNAMIC_PSM,
                                              L2CA_MODE_MASK_ENHANCED_RETRANS,
                                              0, /* flags */
                                              inst->instId);
        }
        else
#endif
        { /* Failed to established a rfcomm connection cleanup and inform the
             calling process. */
            obexUtilCmMsgSetResultHandler(priInst, resultCode, resultSupplier);
        }
        /* No need to un registrer the local server channel, because if a local
           server channel has been obtained duing the rfc connect procedure,
           the rfc connect procedure is responsible for releasing the local
           server channel again */
        cliInst->obtainedServer = FALSE;
        CsrBtUtilBllFreeLinkedList(&sdpTag, CsrBtUtilBllPfreeWrapper);
    }
    CSR_UNUSED(validPortPar);
    CSR_UNUSED(portPar);
    CSR_UNUSED(deviceAddr);
}

static void obexUtilServActivateCfmHandler(ObexUtilPrivateInstType *priInst,
                                           CsrBtResultCode         resultCode,
                                           CsrBtSupplier           resultSupplier)
{
    ObexUtilServActivateFuncType activateCfmFunc = priInst->servInst->activateCfmHandler;
    void  *obexUtilOwnInstData                   = priInst->profilesOwnInstData;
    CsrUint8 localServerCh                  = priInst->localServerCh;
    psm_t         localPsm                       = priInst->localPsm;
    obexUtilDeallocatePrivateInstData(priInst->obexUtilOwnInstData);
    activateCfmFunc(obexUtilOwnInstData, localServerCh, localPsm, resultCode, resultSupplier);
}

static void obexUtilSelectSdpRecordHandler(void                     *instData,
                                           void                     *cmSdcRfcInstData,
                                           CsrBtDeviceAddr          deviceAddr,
                                           CsrUint8            serverChannel,
                                           CsrUint16                entriesInSdpTaglist,
                                           CmnCsrBtLinkedListStruct *sdpTagList)
{ /* This function is called whenever the profile must select a service record */
    ObexUtilPrivateInstType * priInst = instData;
    ObexUtilCliInstType * cliInst     = priInst->cliInst;

    if (cliInst->selectSdpRecordFunc)
    {
        cliInst->selectSdpRecordFunc(priInst->profilesOwnInstData, entriesInSdpTaglist, sdpTagList);
    }
    else
    { /* No callback function has been registered, cancel this request */

        CsrBtUtilRfcConCancel(instData, cliInst->rfcConnectInst);
        CsrBtUtilBllFreeLinkedList(&sdpTagList, CsrBtUtilBllPfreeWrapper);
    }
    CSR_UNUSED(deviceAddr);
    CSR_UNUSED(serverChannel);
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
/*------------------------------------------------------------------------------*
 * Static CM_L2CA upstream messages handler function                            *
 *------------------------------------------------------------------------------*/
static void obexUtilCmL2caRegisterCfmCmConnectingStateHandler(ObexUtilPrivateInstType *priInst,
                                                              void                    * msg)
{ /* Confirmation to earlier l2cap register request, doing the connect process */
    CsrBtCmL2caRegisterCfm *pMsg = (CsrBtCmL2caRegisterCfm *) msg;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    { /* A dynamic PSM is obtained. Establish a L2cap connection */
        priInst->localPsm = pMsg->localPsm;

        if (priInst->processState == CSR_BT_OBEX_UTIL_BT_L2CA_REGISTER_PSM_P_S)
        { /* Establish a L2cap connection */
            ObexUtilInstType * inst = priInst->obexUtilOwnInstData;

            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTING_P_S);
            obexUtilSendCmL2caConnectReq(inst, priInst);
        }
        else
        { /* The application has issue a disconnect/cancel connect request
             while a dynamic PSM were being register, e.g priInst->processState
             must be CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECT_REQUEST_P_S. Note the
             function obexUtilObexConnectRequestResultHandler will unregister
             the dynamic PSM again */
            priInst->resultCode     = CSR_BT_RESULT_CODE_OBEX_CONNECT_CANCELLED;
            priInst->resultSupplier = CSR_BT_SUPPLIER_OBEX_PROFILES;
            obexUtilObexConnectRequestResultHandler(priInst);
        }
    }
    else
    { /* Fail to obtain a dynamic PSM */
        obexUtilCmMsgSetResultHandler(priInst, pMsg->resultCode, pMsg->resultSupplier);
        obexUtilObexConnectRequestResultHandler(priInst);
    }


}

static void obexUtilCmL2caRegisterCfmActivatedStateHandler(ObexUtilPrivateInstType *priInst,
                                                           void                    **msg)
{ /* Confirmation to earlier l2cap register request, doing the activation
     process */
    CsrBtCmL2caRegisterCfm *pMsg = (CsrBtCmL2caRegisterCfm *) *msg;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    { /* A dynamic PSM is obtained. Insert the psm in the service record and
         register it */
        priInst->localPsm = pMsg->localPsm;
        obexUtilRegisterServiceRecordHandler(priInst);
    }
    else
    { /* Could not obtained a psm */
        obexUtilServActivateCfmHandler(priInst, pMsg->resultCode, pMsg->resultSupplier);
    }
}

static void obexUtilCmL2caRegisterCfmDeactivatingStateHandler(ObexUtilPrivateInstType *priInst,
                                                              void                    **msg)
{
    CsrBtCmL2caRegisterCfm   *pMsg  = (CsrBtCmL2caRegisterCfm *) *msg;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    { /* A local psm has been obtained, check if it shall be kept or not */
        obexUtilServUnregisterPsm(priInst, pMsg->localPsm);
    }
    else
    { /* Could not obtained a psm, return the psm that were given in the
         function ObexUtilServActivateStart */
        ;
    }
    /* A server channel has been obtained, check if it shall be kept or not */
    obexUtilServUnregisterServerChannel(priInst, priInst->localServerCh);
    obexUtilServDeactivateCfmHandler(priInst);
}


static void obexUtilCmL2caCancelConnectAcceptCfmHandler(ObexUtilPrivateInstType *priInst,
                                                        void                    **msg)
{ /* This event will be used to indicate to the higher layer that its previous
     request to not accept any incoming l2cap connection is finish */
    CsrBtCmL2caCancelConnectAcceptCfm *pMsg = (CsrBtCmL2caCancelConnectAcceptCfm *) *msg;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    { /* The CM will not accept any incoming l2cap connection on the given
         local PSM. */
        priInst->servInst->l2caSubProcessState = CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S;
        if (priInst->cmState == CSR_BT_OBEX_UTIL_BT_ACTIVATED_S)
        { /* Check if it needs to be connectable again */
            obexUtilSetDeviceConnectable(priInst);
        }
        else
        { /* The l2cap instance is set non-connectable because a rfcomm
             transport connection has been established */
            ;
        }
    }
    else
    { /* Ignore this message, must wait for a CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        ;
    }
}

static void obexUtilCmL2caCancelConnectAcceptCfmDeactivateStateHandler(ObexUtilPrivateInstType *priInst,
                                                                       void                    **msg)
{ /* This event will be used to indicate to the higher layer that its previous
     request to not accept any incoming l2cap connection is finish */
    CsrBtCmL2caCancelConnectAcceptCfm *pMsg = (CsrBtCmL2caCancelConnectAcceptCfm *) *msg;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    { /* The CM will not accept any incoming l2cap connection on the given
         local psm. The function obexUtilNonConnectableHandler
         checks if it is because it want to be non-connectable or because if
         it want to deactivate */
        priInst->servInst->l2caSubProcessState = CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S;
        obexUtilNonConnectableHandler(priInst);
    }
    else
    { /* Ignore this message, must wait for a CSR_BT_CM_CONNECT_ACCEPT_CFM */
        ;
    }
}

static void obexUtilCmL2caConnectAcceptCfmActivatedStateHandler(ObexUtilPrivateInstType *priInst,
                                                                void                    **msg)
{ /* This event indicates that an incoming rfcomm connection has been setup or
     not */
    CsrBtCmL2caConnectAcceptCfm *pMsg = (CsrBtCmL2caConnectAcceptCfm *) *msg;
    ObexUtilServInstType *servInst    = priInst->servInst;
    ObexUtilInstType        *inst     = priInst->obexUtilOwnInstData;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    { /* An l2cap connection has been established by the peer device, e.g
         the OBEX client. */
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL,
                                         pMsg->deviceAddr,pMsg->btConnId);

        if (priInst->processState == CSR_BT_OBEX_UTIL_BT_PENDING_CONNECTABLE_P_S)
        {
            /* An incoming l2cap connection has been established just
             * after the local server has informed the CM that it not
             * will accept any incoming connection. E.g. a
             * CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ is crossing
             * with CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM. Currently this
             * library is waiting for the ObexUtilServConnectResponse
             * function to be called, which means that we cannot accept
             * this incoming connection even that priInst->cmState ==
             * CSR_BT_OBEX_UTIL_BT_ACTIVATED_S */
            servInst->l2caSubProcessState = CSR_BT_OBEX_UTIL_BT_DISCONNECTING_SERVER_P_S;
            CsrBtCmContextl2caDisconnectReqSend(pMsg->btConnId, inst->instId);
        }
        else
        { /* The local server is ready to accept the incoming l2cap connection,
             tell CM that the rfcomm instance not will accept an incoming rfcomm
             connection anymore */
            priInst->cmState                  = CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTED_S;
            priInst->deviceAddr               = pMsg->deviceAddr;
            priInst->btConnId                 = pMsg->btConnId;
            priInst->maxTransportPacketLength = pMsg->mtu;
            if (priInst->maxLocalObexPacketLength > pMsg->localMtu)
            { /* Makes sure that the MAX OBEX PACKET SIZE in a 
                 OBEX_CONNECT_RESPONSE never is larger than our MTU             */
                priInst->maxLocalObexPacketLength = pMsg->localMtu;
            }
            
            servInst->l2caSubProcessState     = CSR_BT_OBEX_UTIL_BT_CONNECTED_SERVER_P_S;
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_CONNECTED_P_S);

            if (servInst->rfcSubProcessState == CSR_BT_OBEX_UTIL_BT_CONNECTABLE_SERVER_P_S)
            { /* The rfcomm is instance is currently connectable. Tell the CM
                 that it not shall be connectable any more, because OBEX now
                 has a transport connection */
                servInst->rfcSubProcessState  = CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECTABLE_SERVER_P_S;
                CsrBtCmContextCancelAcceptConnectReqSend(inst->phandle, priInst->localServerCh, inst->instId);
            }
        }
    }
    else
    { /* Failed to established an incoming L2CAP connection. */
        if (servInst->l2caSubProcessState == CSR_BT_OBEX_UTIL_BT_CONNECTABLE_SERVER_P_S)
        { /* The local server is still ready to accept an incoming transport
             connection. Tell the CM that we still want to accept an incoming
             l2cap transport connection */
            obexUtilSendCmL2caConnectAcceptReq(inst, priInst, servInst);
        }
        else
        { /* The local server were in a state where it would not accept an
             incoming l2cap transport connection */
            servInst->l2caSubProcessState = CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S;
            obexUtilSetDeviceConnectable(priInst);
        }
    }
}

static void obexUtilCmL2caConnectAcceptCfmHandler(ObexUtilPrivateInstType *priInst,
                                                  void                    **msg)
{ /* This event indicates that an incoming l2cap connection has been setup or
     not, while an rfcomm transport also is established */
    CsrBtCmL2caConnectAcceptCfm *pMsg = (CsrBtCmL2caConnectAcceptCfm *) *msg;
    ObexUtilServInstType *servInst    = priInst->servInst;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    {
        ObexUtilInstType        *inst     = priInst->obexUtilOwnInstData;
        /* An l2cap connection has been established by the peer
         * device, make sure that it is release again */
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL,
                                         pMsg->deviceAddr,
                                         pMsg->btConnId);
        servInst->l2caSubProcessState = CSR_BT_OBEX_UTIL_BT_DISCONNECTING_SERVER_P_S;
        CsrBtCmContextl2caDisconnectReqSend(pMsg->btConnId, inst->instId);
    }
    else
    {
        /* Failed to established an incoming L2cap connection. */
        servInst->l2caSubProcessState = CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S;

        if (priInst->cmState == CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S)
        {
            /* The server is set non-connectable. The function
             * obexUtilNonConnectableHandler checks if it is because
             * it want to be non-connectable or because if it want to
             * deactivate */
            obexUtilNonConnectableHandler(priInst);
        }
        else
        {
            /* priInst->cmState must be CSR_BT_OBEX_UTIL_BT_RFC_CONNECTED_S */
            ;
        }
    }
}

static void obexUtilCmL2caConnectCfmHandler(ObexUtilPrivateInstType * priInst,
                                            void                    * msg)
{ /* Confirmation to earlier l2cap connection request */
    CsrBtCmL2caConnectCfm *pMsg = (CsrBtCmL2caConnectCfm *) msg;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    {
        /* The l2cap transport connection is established */
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL,
                                         pMsg->deviceAddr,
                                         pMsg->btConnId);
        priInst->btConnId                        = pMsg->btConnId;
        priInst->maxTransportPacketLength   = pMsg->mtu;

        if (priInst->processState == CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTING_P_S)
        {
            /* An l2cap connection is established, start sending the
             * OBEX Connect Request packet */
            obexUtilObexConnectRequestHandler(priInst, CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTED_S);
        }
        else
        {
            /* The state must be CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECT_REQUEST_P_S */
            priInst->resultCode     = CSR_BT_RESULT_CODE_OBEX_CONNECT_CANCELLED;
            priInst->resultSupplier = CSR_BT_SUPPLIER_OBEX_PROFILES;
            obexUtilReleaseTransportConnection(priInst);
        }
    }
    else
    {
        /* The l2cap transport connection were not established */
        obexUtilCmMsgSetResultHandler(priInst, pMsg->resultCode, pMsg->resultSupplier);
        obexUtilObexConnectRequestResultHandler(priInst);
    }
}

static void obexUtilCmL2caDataIndHandler(ObexUtilPrivateInstType *priInst,
                                         void                    **msg)
{
    /* This event indicates to the higher port entity that l2cap data
     * has arrived from the remote device and is stored in the
     * location given by payload. Called generic function there is
     * responsible of gathering all the tranport packet into one OBEX
     * packet */
    CsrBtCmL2caDataInd *pMsg = (CsrBtCmL2caDataInd *) *msg;

    if (pMsg->length >= CSR_BT_OBEX_HEADER_LENGTH &&
        pMsg->length >= CSR_BT_OBEX_GET_REQUEST_LENGTH(pMsg->payload) &&
        priInst->incomingObexPacket == NULL)
    { /* The incoming OBEX packet looks allright, just proceed */     
        CsrBool preRxDataPending = priInst->rxDataPending; 
        
        if (priInst->cliInst)
        { /* Represent a client */
            if (priInst->obexOperationIndex == CSR_BT_OBEX_UTIL_GET_REQUEST_RESULT_HANDLER)
            { /* Check if there are more space in the intern buffer */
                obexUtilRxQueueDataRequestHandler(priInst);
            }
            else
            { /* Intern flow control is not needed. */
                CsrBtCmL2caDataResSend(priInst->btConnId);
            }
        }
        else
        { /* Represent a server */
            switch (CSR_BT_OBEX_GET_REQUEST_OPCODE(pMsg->payload))
            {
                case CSR_BT_OBEX_ABORT_OPCODE:
                { /* An OBEX Abort Request message must always process immediately
                     Flush the intern buffer as there are no need to send these
                     packet to the profile. Note intern flowcontrol is not needed */
                    preRxDataPending = FALSE;
                    obexUtilRxQueueAbortHandler(priInst);
                    CsrBtCmL2caDataResSend(priInst->btConnId);
                    break;
                }
                case CSR_BT_OBEX_PUT_OPCODE:
                case CSR_BT_OBEX_PUT_OPCODE | CSR_BT_OBEX_FINAL_BIT_MASK:
                { /* Check if there are more space in the intern buffer 
                     and check if the incoming OBEX Request packet must be save or not*/
                    obexUtilRxQueueDataRequestHandler(priInst);
                    break;
                }
                default:
                { /* Intern flow control is not needed. check if the incoming 
                     OBEX Request packet must be save or not */
                    CsrBtCmL2caDataResSend(priInst->btConnId);
                    break;
                }
            }
        }
        
        /* Check if the incoming OBEX packet must be saved or not */
        if (preRxDataPending || priInst->rxQueue)
        { /* The OBEX profile is busy or another incoming messages is 
             already placed on the queue. Save the incoming message */
            CsrMessageQueuePush(&priInst->rxQueue, CSR_BT_CM_PRIM, *msg);
            *msg = NULL;
        }
        else
        { /* The OBEX profile is ready to proceed. 
             Note that obexUtilTransportDataIndFirstFragment is 
             called direct as one OBEX packet must fit into one 
             L2CAP packet. E.g no need to called 
             obexUtilTransportDataIndHandler first */
            obexUtilTransportDataIndFirstFragment(priInst, pMsg->length, pMsg->payload);
        }
    }
    else
    { /* An invalid OBEX packet is received. Release the transport connection 
         and CsrPmemFree the incoming data packet to prevent a memory leak */
        CsrPmemFree(pMsg->payload);
        obexUtilReleaseTransportHandler(priInst, 
                                    CSR_BT_RESULT_CODE_OBEX_INCOMING_OBEX_PACKET_INVALID, 
                                    CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S);
    }
}

static void obexUtilCmL2caDataCfmHandler(ObexUtilPrivateInstType *priInst,
                                         void                    **msg)
{
    /* This event indicates that the previous data was sent from L2cap */
    obexUtilTransportDataCfmHandler(priInst, TRUE);
    CSR_UNUSED(msg);
}

static void obexUtilCmL2caDisconnectIndHandler(ObexUtilPrivateInstType *priInst,
                                               void                    **msg)
{
    /* This event is an indication that the l2cap data link connection
     * referenced by the btConnId is disconnected. */
    CsrBtCmL2caDisconnectInd *pMsg = (CsrBtCmL2caDisconnectInd *) *msg;

    CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                                     priInst->deviceAddr,
                                     pMsg->btConnId);
    obexUtilTransportDisconnectIndHandler(priInst,
                                          pMsg->reasonCode,
                                          pMsg->reasonSupplier,
                                          CSR_BT_OBEX_UTIL_L2CAP_TRANSPORT);
}

static void obexUtilCmL2caModeChangeIndHandler(ObexUtilPrivateInstType *priInst,
                                               void                    **msg)
{ /* This event is used to indicate that when the device associated
     with the local server channel/ACL connection changes between Active mode,
     Sniff mode, and Park mode. */
    CsrBtCmL2caModeChangeInd *pMsg = (CsrBtCmL2caModeChangeInd *) *msg;

    if (obexUtilModeChangeIndHandler(priInst, pMsg->mode))
    {
        /* The peer device has set the connection into park
         * mode. Request to goto CSR_BT_ACTIVE_MODE */
        CsrBtCml2caModeChangeReqSend(pMsg->btConnId,
                                     CSR_BT_ACTIVE_MODE);
    }
    else
    {
        /* No mode change request is required */
        ;
    }
}

static void obexUtilCmL2caDataAbortCfmHandler(ObexUtilPrivateInstType *priInst,
                                              void                    **msg)
{ /* This event indicates that all unsent SDUs queued for transmission have 
     been discarded */
    switch (priInst->processState)
    {
        case CSR_BT_OBEX_UTIL_OBEX_ABORT_REQUEST_PENDING_P_S:
        { /* All unsent SDUs were flush because ObexUtilAbortRequest
             were called */
            obexUtilTransportDataCfmHandler(priInst, FALSE);
            break;
        }
        case CSR_BT_OBEX_UTIL_OBEX_FLUSH_PUT_REQUEST_PENDING_P_S:
        { /* All unsent SDUs were flush because an OBEX response 
             packet with an error response were received during a multiple
             PUT operation with SRM is enable. */
            obexUtilCommonObexRequestOperationResultHandler(priInst);
            break;
        }
        case CSR_BT_OBEX_UTIL_OBEX_ABORT_RESPONSE_PENDING_P_S:
        { /* All unsent SDUs were flush because an OBEX ABORT Request
             packet were received. Send the OBEX Abort Response Message */
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S);

            obexUtilServObexResponsePacketHandler(priInst,
                                                  CSR_BT_OBEX_ABORT_OPCODE,
                                                  CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
            break;
        }
        default:
        { /* Just ignore */
            break;
        }
    }
    CSR_UNUSED(msg);
}
#endif

/*------------------------------------------------------------------------------*
 * Static CM RFC upstream messages handler function                             *
 *------------------------------------------------------------------------------*/
static void obexUtilCmDataIndHandler(ObexUtilPrivateInstType * priInst,
                                     void                    * msg)
{ /* This event indicates to the higher port entity that rfommm data has
     arrived from the remote device and is stored in the location given by
     payload. Called generic function there is responsible of gathering all
     the tranport packet into one OBEX packet, and called the CsrBtCmDataResSend
     function in order to receive more data */
    CsrBtCmDataInd *pMsg = (CsrBtCmDataInd *) msg;

    CsrBtCmDataResSend(pMsg->btConnId);
    obexUtilTransportDataIndHandler(priInst, pMsg->payloadLength, pMsg->payload);
}

static void obexUtilCmDataCfmHandler(ObexUtilPrivateInstType * priInst,
                                     void                    * msg)
{ /* This event indicates that the previous data was successfully
     sent from RFCOMM */
    obexUtilTransportDataCfmHandler(priInst, FALSE);
    CSR_UNUSED(msg);
}

static void obexUtilCmDisconnectIndHandler(ObexUtilPrivateInstType * priInst,
                                           void                    * msg)
{ /* This event is an indication that the rfcomm data link connection
     referenced by the server channel within the multiplexor session
     indexed by mux_id has closed down. */
    CsrBtCmDisconnectInd *pMsg = (CsrBtCmDisconnectInd *) msg;

    if (!pMsg->status)
    { /* Failed to release the rfcomm connection, try again */
        ObexUtilInstType *inst = priInst->obexUtilOwnInstData;

        CsrBtCmContextDisconnectReqSend(pMsg->btConnId,
                                        inst->instId);
    }
    else
    { /* The rfcomm transport connection has been released */
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,priInst->deviceAddr,
                                         pMsg->btConnId);
        obexUtilTransportDisconnectIndHandler(priInst,
                                              pMsg->reasonCode,
                                              pMsg->reasonSupplier,
                                              CSR_BT_OBEX_UTIL_RFC_TRANSPORT);
    }
}

static void obexUtilIgnoreCmMsgHandler(ObexUtilPrivateInstType * priInst,
                                       void                    * msg)
{ /* Received an CM upstream message which must be ignore, called
     CsrBtCmFreeUpstreamMessageContents to make sure that 'payload' is free */
    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg);
    CSR_UNUSED(priInst);
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static void obexUtilIgnoreCmL2caMsgHandler(ObexUtilPrivateInstType *priInst,
                                           void                    **msg)
{ /* Received an CM upstream message which must be ignore, called
     CsrBtCmFreeUpstreamMessageContents to make sure that 'payload' is free */
    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, *msg);
    CSR_UNUSED(priInst);
}
#endif

static void obexUtilCmPortNegIndHandler(ObexUtilPrivateInstType * priInst,
                                        void                    * msg)
{ /* This event indicates to the higher port entity that the remote port
     entity requests it to set port parameters for the DLCI as given in the
     Port Parameters */
    CsrBtCmPortnegInd *pMsg = (CsrBtCmPortnegInd *) msg;

    if (pMsg->request)
    { /* The peer device request that the local device sends its current
         port parameter setting for the given connection id. Just return with the
         default setting */
        RFC_PORTNEG_VALUES_T portPar;

        CsrBtPortParDefault(&portPar);
        CsrBtCmPortnegResSend(pMsg->btConnId, &(portPar));
    }
    else
    { /* Negotiates the port parameters. The local must agree/disagree. Just
         always agree */
        CsrBtCmPortnegResSend(pMsg->btConnId, &(pMsg->portPar));
    }
    CSR_UNUSED(priInst);
}

static void obexUtilCmRfcModeChangeIndHandler(ObexUtilPrivateInstType * priInst,
                                              void                    * msg)
{ /* This event is used to indicate that when the device associated
     with the local server channel/ACL connection changes between Active mode,
     Sniff mode, and Park mode. */
    CsrBtCmRfcModeChangeInd *pMsg = (CsrBtCmRfcModeChangeInd *) msg;

    if (obexUtilModeChangeIndHandler(priInst, pMsg->mode))
    { /* The peer device has set the connection into park mode. Request to goto
         CSR_BT_ACTIVE_MODE */
        CsrBtCmRfcModeChangeReqSend(pMsg->btConnId, CSR_BT_ACTIVE_MODE);
    }
    else
    { /* No mode change request is required */
        ;
    }
}

static void obexUtilCmRegisterCfmActivatedStateHandler(ObexUtilPrivateInstType * priInst,
                                                       void                    * msg)
{ /* This event will be used to indicate to the higher layer that its previous
     registration of a protocol handle with an CSR_BT_CM_REGISTER_REQ event had been
     accepted or not. The server channel number assigned is also returned. */
    CsrBtCmRegisterCfm *pMsg = (CsrBtCmRegisterCfm *) msg;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    { /* A local server channel has been obtained. Find out if a psm shall be
         obtained or if the SDP record can be register direct */
        priInst->localServerCh = pMsg->serverChannel;
        obexUtilRegisterBtMultiplexerHandle(priInst);
    }
    else
    { /* Could not obtained a local server channel */
        obexUtilServActivateCfmHandler(priInst, pMsg->resultCode, pMsg->resultSupplier);
    }
}

static void obexUtilCmRegisterCfmDeactivatingStateHandler(ObexUtilPrivateInstType * priInst,
                                                          void                    * msg)
{ /* What this event indicates is describe in the function
     obexUtilCmRegisterCfmActivatedStateHandler. */
    CsrBtCmRegisterCfm   *pMsg      = (CsrBtCmRegisterCfm *) msg;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    { /* A local server channel has been obtained, check if it shall be kept
         or not */
        obexUtilServUnregisterServerChannel(priInst, pMsg->serverChannel);
    }
    else
    { /* Could not obtained a local server channel, return the server channel
         the were given in the function ObexUtilServActivateStart */
        ;
    }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    obexUtilServUnregisterPsm(priInst, priInst->localPsm);
#endif
    obexUtilServDeactivateCfmHandler(priInst);
}

static void obexUtilCmCancelAcceptConnectCfmHandler(ObexUtilPrivateInstType * priInst,
                                                    void                    * msg)
{ /* This event will be used to indicate to the higher layer that its previous
     request to not accept any incoming rfcomm connection is finish */
    CsrBtCmCancelAcceptConnectCfm *pMsg     = (CsrBtCmCancelAcceptConnectCfm *) msg;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    { /* The CM will not accept any incoming rfcomm connection on the given
         local server channel. */

        priInst->servInst->rfcSubProcessState = CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S;

        if (priInst->cmState == CSR_BT_OBEX_UTIL_BT_ACTIVATED_S)
        { /* Check if it needs to be connectable again */
            obexUtilSetDeviceConnectable(priInst);
        }
        else
        { /* The l2cap instance is set non-connectable because a rfcomm
             transport connection has been established */
            ;
        }
    }
    else
    { /* Ignore this message, must wait for a CSR_BT_CM_CONNECT_ACCEPT_CFM */
        ;
    }
}

static void obexUtilCmConnectAcceptCfmActivatedStateHandler(ObexUtilPrivateInstType * priInst,
                                                            void                    * msg)
{ /* This event indicates that an incoming rfcomm connection has been setup or
     not */
    CsrBtCmConnectAcceptCfm *pMsg  = (CsrBtCmConnectAcceptCfm *) msg;
    ObexUtilServInstType *servInst = priInst->servInst;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    { /* An rfcomm connection has been established by the peer device, e.g
         the OBEX client. */
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL,
                                         pMsg->deviceAddr,
                                         pMsg->btConnId);

        priInst->deviceAddr  = pMsg->deviceAddr;
        priInst->btConnId    = pMsg->btConnId;

        if (priInst->processState == CSR_BT_OBEX_UTIL_BT_PENDING_CONNECTABLE_P_S)
        {
            /* An incoming rfcomm connection has been established just
             * after the local server has informed the CM that it not
             * will accept any incoming connection. E.g. a
             * CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ is crossing with
             * CSR_BT_CM_CONNECT_ACCEPT_CFM. Currently this library is
             * waiting for the ObexUtilServConnectResponse function to
             * be called, which means that we cannot accept this
             * incoming connection even that priInst->cmState ==
             * CSR_BT_OBEX_UTIL_BT_ACTIVATED_S */
            ObexUtilInstType *inst = priInst->obexUtilOwnInstData;
            servInst->rfcSubProcessState = CSR_BT_OBEX_UTIL_BT_DISCONNECTING_SERVER_P_S;
            CsrBtCmContextDisconnectReqSend(pMsg->btConnId,
                                            inst->instId);
        }
        else
     { /* The local server is ready to accept the incoming rfcomm connection,
             tell CM that the l2cap instance not will accept an incoming l2cap
             connection anymore                                                 */
            priInst->cmState                  = CSR_BT_OBEX_UTIL_BT_RFC_CONNECTED_S;
            priInst->maxTransportPacketLength = pMsg->profileMaxFrameSize;
            servInst->rfcSubProcessState      = CSR_BT_OBEX_UTIL_BT_CONNECTED_SERVER_P_S;
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_CONNECTED_P_S);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            if (servInst->l2caSubProcessState == CSR_BT_OBEX_UTIL_BT_CONNECTABLE_SERVER_P_S)
            { /* The l2cap is instance is currently connectable. Tell the CM
                 that it not shall be connectable any more, because OBEX now
                 has a transport connection */
                ObexUtilInstType *inst        = priInst->obexUtilOwnInstData;
                servInst->l2caSubProcessState = CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECTABLE_SERVER_P_S;
                CsrBtCmContextl2caCancelConnectAcceptReqSend(inst->phandle, priInst->localPsm, inst->instId);
            }
#endif
        }
    }
    else
    { /* Failed to established an incoming RFCOMM connection. */
        if (servInst->rfcSubProcessState == CSR_BT_OBEX_UTIL_BT_CONNECTABLE_SERVER_P_S)
        { /* The local server is still ready to accept an incoming transport
             connection. Tell the CM that we still want to accept an incoming
             rfcomm transport connection */
            ObexUtilServInstType    *servInst = priInst->servInst;
            ObexUtilInstType        *inst     = priInst->obexUtilOwnInstData;
            CsrUint8 modemStatus = CSR_BT_MODEM_SEND_CTRL_DCE_DEFAULT;
            CsrBtCmContextConnectAcceptReqSend(inst->phandle, servInst->classOfDevice, 0,
                                               servInst->rfcommMtu, priInst->localServerCh,
                                               servInst->secIncoming, servInst->profileUuid,
                                               inst->instId, modemStatus, CSR_BT_DEFAULT_BREAK_SIGNAL,
                                               CSR_BT_DEFAULT_MSC_TIMEOUT);
        }
        else
        { /* The local server were in a state where it would not accept an
             incoming rfcomm transport connection */
            servInst->rfcSubProcessState = CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S;
            obexUtilSetDeviceConnectable(priInst);
        }
    }
}

static void obexUtilCmConnectAcceptCfmHandler(ObexUtilPrivateInstType * priInst,
                                              void                    * msg)
{ /* This event indicates that an incoming rfcomm connection has been setup or
     not */
    CsrBtCmConnectAcceptCfm *pMsg     = (CsrBtCmConnectAcceptCfm *) msg;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    {
        /* An RFCOMM connection has been established by the peer
         * device, release it again */
        ObexUtilInstType *inst                  = priInst->obexUtilOwnInstData;
        priInst->servInst->rfcSubProcessState   = CSR_BT_OBEX_UTIL_BT_DISCONNECTING_SERVER_P_S;

        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL,
                                         pMsg->deviceAddr,
                                         pMsg->btConnId);

        CsrBtCmContextDisconnectReqSend(priInst->btConnId,
                                        inst->instId);
    }
    else
    { /* Failed to established a Rfcomm transport connection */
        priInst->servInst->rfcSubProcessState = CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S;

        if (priInst->cmState == CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S)
        { /* The server is set non-connectable. The function
             obexUtilNonConnectableHandler checks if it is because it want to
             be non-connectable or because if it want to deactivate */
            obexUtilNonConnectableHandler(priInst);
        }
        else
        { /* priInst->cmState must be CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTED_S */
            ;
        }
    }
}

static void obexUtilCmCancelAcceptConnectCfmDeactivatingStateHandler(ObexUtilPrivateInstType * priInst,
                                                                     void                    * msg)
{ /* This event will be used to indicate to the higher layer that its previous
     request to not accept any incoming rfcomm connection is finish */
    CsrBtCmCancelAcceptConnectCfm *pMsg     = (CsrBtCmCancelAcceptConnectCfm *) msg;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    { /* The CM will not accept any incoming rfcomm connection on the given
         local server channel. The function obexUtilNonConnectableHandler
         checks if it is because it want to be non-connectable or because if
         it want to deactivate */
        priInst->servInst->rfcSubProcessState = CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S;
        obexUtilNonConnectableHandler(priInst);
    }
    else
    { /* Ignore this message, must wait for a CSR_BT_CM_CONNECT_ACCEPT_CFM */
        ;
    }
}

static void obexUtilCmSdsRegisterCfmHandler(ObexUtilPrivateInstType * priInst,
                                            void                    * msg)
{ /* This event will be used to indicate to the higher layer that its previous
     registration of a service record had been accepted or not */
    CsrBtCmSdsExtRegisterCfm *pMsg     = (CsrBtCmSdsExtRegisterCfm *) msg;
    ObexUtilServInstType  *servInst = priInst->servInst;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    { /* The service record has been register with success. Saved the service
         record handle, in order to be able to unregister it later */
        servInst->serviceRecordHandle = pMsg->serviceRecHandle;

        if (priInst->cmState == CSR_BT_OBEX_UTIL_BT_ACTIVATED_S)
        { /* Please note the priInst->processState must be set before the call
             back function (servInst->activateCfmHandler) is called. This will
             allow the profile to call ObexUtilServConnectableStart direct from
             the registered callback function (servInst->activateCfmHandler) */
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_ACTIVATED_P_S);
            servInst->activateCfmHandler(priInst->profilesOwnInstData,
                                         priInst->localServerCh,
                                         priInst->localPsm,
                                         pMsg->resultCode, pMsg->resultSupplier);
        }
        else
        { /* The cmState must equals CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S, the
             service record must be unregister again */
            obexUtilUnregisterSdpRecord(priInst);
        }
    }
    else
    { /* Failed to register the service record */
        if (priInst->cmState == CSR_BT_OBEX_UTIL_BT_ACTIVATED_S)
        { /* The activation procedure went wrong, keep the obtained local
             server channel. */
            obexUtilServActivateCfmHandler(priInst, pMsg->resultCode, pMsg->resultSupplier);
        }
        else
        { /* The cmState must equals CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S, check
             if the obtained local server channel must be kept or not */
            obexUtilServUnregisterServerChannel(priInst, priInst->localServerCh);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            obexUtilServUnregisterPsm(priInst, priInst->localPsm);
#endif
            obexUtilServDeactivateCfmHandler(priInst);
        }
    }
}

static void obexUtilCmSdsUnregisterCfmHandler(ObexUtilPrivateInstType * priInst,
                                              void                    * msg)
{ /* This event will be used to indicate to the higher layer that its previous
     unregistration of a service record has be accepted or not. */
    CsrBtCmSdsExtUnregisterCfm *pMsg = (CsrBtCmSdsExtUnregisterCfm *) msg;

    if (obexUtilCmMsgResultHandler(pMsg->resultCode, pMsg->resultSupplier))
    { /* The service record is unregistered, check if the obtained local server
         channel must be kept or not */
        obexUtilServUnregisterServerChannel(priInst, priInst->localServerCh);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        obexUtilServUnregisterPsm(priInst, priInst->localPsm);
#endif
        obexUtilServDeactivateCfmHandler(priInst);
    }
    else
    { /* Failed to unregister the service record, try again */
        obexUtilUnregisterSdpRecord(priInst);
    }
}

#ifdef CSR_BT_INSTALL_OBEX_CONTEXT_ID
static CsrUint8 obexUtilCmCancelAcceptConnectCfmContext(void * msg)
{
    CsrBtCmCancelAcceptConnectCfm *pMsg = (CsrBtCmCancelAcceptConnectCfm *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmConnectCfmContext(void * msg)
{
    CsrBtCmConnectCfm *pMsg = (CsrBtCmConnectCfm *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmConnectAcceptCfmContext(void * msg)
{
    CsrBtCmConnectAcceptCfm *pMsg     = (CsrBtCmConnectAcceptCfm *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmRegisterCfmContext(void * msg)
{
    CsrBtCmRegisterCfm *pMsg = (CsrBtCmRegisterCfm *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmDisconnectIndContext(void * msg)
{
    CsrBtCmDisconnectInd *pMsg = (CsrBtCmDisconnectInd *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmDataIndContext(void * msg)
{
    CsrBtCmDataInd *pMsg = (CsrBtCmDataInd *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmDataCfmContext(void * msg)
{
    CsrBtCmDataCfm *pMsg = (CsrBtCmDataCfm *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmControlIndContext(void * msg)
{
    CsrBtCmControlInd *pMsg = (CsrBtCmControlInd *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmRfcModeChangeIndContext(void * msg)
{
    CsrBtCmRfcModeChangeInd *pMsg = (CsrBtCmRfcModeChangeInd *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmPortnegIndContext(void * msg)
{
    CsrBtCmPortnegInd *pMsg = (CsrBtCmPortnegInd *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmSdcSearchIndContext(void * msg)
{
    CsrBtCmSdcSearchInd *pMsg = (CsrBtCmSdcSearchInd *) msg;
    return ((CsrUint8) pMsg->localServerChannel);
}

static CsrUint8 obexUtilCmSdcSearchCfmContext(void * msg)
{
    CsrBtCmSdcSearchCfm *pMsg = (CsrBtCmSdcSearchCfm *) msg;
    return ((CsrUint8) pMsg->localServerChannel);
}

static CsrUint8 obexUtilCmSdcAttributeCfmContext(void * msg)
{
    CsrBtCmSdcAttributeCfm *pMsg = (CsrBtCmSdcAttributeCfm *) msg;
    return ((CsrUint8) pMsg->localServerChannel);
}

static CsrUint8 obexUtilCmSdcCloseIndContext(void * msg)
{
    CsrBtCmSdcCloseInd *pMsg = (CsrBtCmSdcCloseInd *) msg;
    return ((CsrUint8) pMsg->localServerChannel);
}

static CsrUint8 obexUtilCmSdcReleaseResourcesCfmContext(void * msg)
{
    CsrBtCmSdcReleaseResourcesCfm *pMsg = (CsrBtCmSdcReleaseResourcesCfm *) msg;
    return ((CsrUint8) pMsg->localServerChannel);
}

static CsrUint8 obexUtilCmSdsExtRegisterCfmContext(void * msg)
{
    CsrBtCmSdsExtRegisterCfm *pMsg = (CsrBtCmSdsExtRegisterCfm *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmSdsExtUnregisterCfmContext(void * msg)
{
    CsrBtCmSdsExtUnregisterCfm *pMsg = (CsrBtCmSdsExtUnregisterCfm *) msg;
    return ((CsrUint8) pMsg->context);
}

static const ObexUtilCmUpstreamMsgContextHandleType obexUtilCmRfcUpstreamMsgContextHandler
[CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT] =
{
    obexUtilCmCancelAcceptConnectCfmContext,            /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
    obexUtilCmConnectCfmContext,                        /* CSR_BT_CM_CONNECT_CFM */
    obexUtilCmConnectAcceptCfmContext,                  /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
    obexUtilCmRegisterCfmContext,                       /* CSR_BT_CM_REGISTER_CFM */
    obexUtilCmDisconnectIndContext,                     /* CSR_BT_CM_DISCONNECT_IND */
    NULL,                                               /* CSR_BT_CM_SCO_CONNECT_CFM */
    NULL,                                               /* CSR_BT_CM_SCO_DISCONNECT_IND */
    NULL,                                               /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
    obexUtilCmDataIndContext,                           /* CSR_BT_CM_DATA_IND */
    obexUtilCmDataCfmContext,                           /* CSR_BT_CM_DATA_CFM */
    obexUtilCmControlIndContext,                        /* CSR_BT_CM_CONTROL_IND */
    obexUtilCmRfcModeChangeIndContext,                  /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
    obexUtilCmPortnegIndContext,                        /* CSR_BT_CM_PORTNEG_IND */
    NULL,                                               /* CSR_BT_CM_PORTNEG_CFM */
};

static const ObexUtilCmUpstreamMsgContextHandleType obexUtilCmSdpUpstreamMsgContextHandler
[CSR_BT_CM_SDC_PRIM_EXT_UPSTREAM_COUNT] =
{
    obexUtilCmSdcSearchIndContext,                      /* CSR_BT_CM_SDC_SEARCH_IND */
    obexUtilCmSdcSearchCfmContext,                      /* CSR_BT_CM_SDC_SEARCH_CFM */
    NULL,                                               /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
    NULL,                                               /* CSR_BT_CM_SDC_OPEN_CFM */
    obexUtilCmSdcAttributeCfmContext,                   /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
    obexUtilCmSdcCloseIndContext,                       /* CSR_BT_CM_SDC_CLOSE_IND */
    obexUtilCmSdcReleaseResourcesCfmContext,            /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
    NULL,                                               /* CSR_BT_CM_SDS_REGISTER_CFM */
    NULL,                                               /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    obexUtilCmSdsExtRegisterCfmContext,                 /* CSR_BT_CM_SDS_EXT_REGISTER_CFM */
    obexUtilCmSdsExtUnregisterCfmContext,               /* CSR_BT_CM_SDS_EXT_UNREGISTER_CFM */
};
#endif

static const ObexUtilCmRfcUpstreamMsgHandleType obexUtilCmRfcUpstreamMsgStateHandlers
[CSR_BT_OBEX_UTIL_BT_RFC_CONNECTING_S][CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT] =
{
    /* CSR_BT_OBEX_UTIL_BT_RFC_CONNECTED_S */
    {
        NULL,                                                       /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                       /* CSR_BT_CM_REGISTER_CFM */
        obexUtilCmDisconnectIndHandler,                             /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        obexUtilCmDataIndHandler,                                   /* CSR_BT_CM_DATA_IND */
        obexUtilCmDataCfmHandler,                                   /* CSR_BT_CM_DATA_CFM */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_CONTROL_IND */
        obexUtilCmRfcModeChangeIndHandler,                          /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        obexUtilCmPortNegIndHandler,                                /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                       /* CSR_BT_CM_PORTNEG_CFM */
    },
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20 
    /* CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTED_S */
    {
        obexUtilCmCancelAcceptConnectCfmHandler,                    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_CONNECT_CFM */
        obexUtilCmConnectAcceptCfmHandler,                          /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                       /* CSR_BT_CM_REGISTER_CFM */
        obexUtilCmDisconnectIndHandler,                             /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_DATA_IND */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_DATA_CFM */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_CONTROL_IND */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                       /* CSR_BT_CM_PORTNEG_CFM */
    },
#endif
    /* CSR_BT_OBEX_UTIL_BT_DISCONNECTING_S */
    {
        NULL,                                                       /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                       /* CSR_BT_CM_REGISTER_CFM */
        obexUtilCmDisconnectIndHandler,                             /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_DATA_IND */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_DATA_CFM */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_CONTROL_IND */
        obexUtilCmRfcModeChangeIndHandler,                          /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                       /* CSR_BT_CM_PORTNEG_CFM */
    },

    /* CSR_BT_OBEX_UTIL_BT_ACTIVATED_S */
    {
        obexUtilCmCancelAcceptConnectCfmHandler,                    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_CONNECT_CFM */
        obexUtilCmConnectAcceptCfmActivatedStateHandler,            /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        obexUtilCmRegisterCfmActivatedStateHandler,                 /* CSR_BT_CM_REGISTER_CFM */
        obexUtilCmDisconnectIndHandler,                             /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_DATA_IND */
        NULL,                                                       /* CSR_BT_CM_DATA_CFM */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_CONTROL_IND */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                       /* CSR_BT_CM_PORTNEG_CFM */
    },

    /* CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S */
    {
        obexUtilCmCancelAcceptConnectCfmDeactivatingStateHandler,   /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_CONNECT_CFM */
        obexUtilCmConnectAcceptCfmHandler,                          /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        obexUtilCmRegisterCfmDeactivatingStateHandler,              /* CSR_BT_CM_REGISTER_CFM */
        obexUtilCmDisconnectIndHandler,                             /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_DATA_IND */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_DATA_CFM */
        obexUtilIgnoreCmMsgHandler,                                 /* CSR_BT_CM_CONTROL_IND */
        obexUtilCmRfcModeChangeIndHandler,                          /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        obexUtilCmPortNegIndHandler,                                /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                       /* CSR_BT_CM_PORTNEG_CFM */
    },
};

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
#ifdef CSR_BT_INSTALL_OBEX_CONTEXT_ID
static CsrUint8 obexUtilCmL2caRegisterCfmContext(void * msg)
{
    CsrBtCmL2caRegisterCfm *pMsg = (CsrBtCmL2caRegisterCfm *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmL2caConnectCfmContext(void * msg)
{
    CsrBtCmL2caConnectCfm *pMsg = (CsrBtCmL2caConnectCfm *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmL2caConnectAcceptCfmContext(void * msg)
{
    CsrBtCmL2caConnectAcceptCfm *pMsg = (CsrBtCmL2caConnectAcceptCfm *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmL2caCancelConnectAcceptCfmContext(void * msg)
{
    CsrBtCmL2caCancelConnectAcceptCfm *pMsg = (CsrBtCmL2caCancelConnectAcceptCfm *) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmL2caDataCfmContext(void * msg)
{
    CsrBtCmL2caDataCfm *pMsg = (CsrBtCmL2caDataCfm*) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmL2caDataIndContext(void * msg)
{
    CsrBtCmL2caDataInd *pMsg = (CsrBtCmL2caDataInd*) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmL2caDisconnectIndContext(void * msg)
{
    CsrBtCmL2caDisconnectInd *pMsg = (CsrBtCmL2caDisconnectInd*) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmL2caModeChangeIndContext(void * msg)
{
    CsrBtCmL2caModeChangeInd *pMsg = (CsrBtCmL2caModeChangeInd*) msg;
    return ((CsrUint8) pMsg->context);
}

static CsrUint8 obexUtilCmL2caDataAbortCfmContext(void * msg)
{
    CsrBtCmL2caDataAbortCfm *pMsg = (CsrBtCmL2caDataAbortCfm*) msg;
    return ((CsrUint8) pMsg->context);
}

static const ObexUtilCmUpstreamMsgContextHandleType obexUtilCmL2caUpstreamMsgContextHandler
[CSR_BT_CM_L2CA_PRIM_UPSTREAM_COUNT] =
{
    obexUtilCmL2caRegisterCfmContext,                               /* CSR_BT_CM_L2CA_REGISTER_CFM */
    obexUtilCmL2caConnectCfmContext,                                /* CSR_BT_CM_L2CA_CONNECT_CFM */
    obexUtilCmL2caConnectAcceptCfmContext,                          /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
    obexUtilCmL2caCancelConnectAcceptCfmContext,                    /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
    obexUtilCmL2caDataCfmContext,                                   /* CSR_BT_CM_L2CA_DATA_CFM */
    obexUtilCmL2caDataIndContext,                                   /* CSR_BT_CM_L2CA_DATA_IND */
    obexUtilCmL2caDisconnectIndContext,                             /* CSR_BT_CM_L2CA_DISCONNECT_IND */
    NULL,                                                           /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
    NULL,                                                           /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
    NULL,                                                           /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
    obexUtilCmL2caModeChangeIndContext,                             /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    obexUtilCmL2caDataAbortCfmContext,                              /* CSR_BT_CM_L2CA_DATA_ABORT_CFM*/
};
#endif

static const ObexUtilCmL2caUpstreamMsgHandleType obexUtilCmL2capUpstreamMsgStateHandlers
[CSR_BT_OBEX_UTIL_BT_RFC_CONNECTING_S][CSR_BT_CM_L2CA_PRIM_UPSTREAM_COUNT] =
{
    /* CSR_BT_OBEX_UTIL_BT_RFC_CONNECTED_S */
    {
        NULL,                                                       /* CSR_BT_CM_L2CA_REGISTER_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_CONNECT_CFM */
        obexUtilCmL2caConnectAcceptCfmHandler,                      /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        obexUtilCmL2caCancelConnectAcceptCfmHandler,                /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        obexUtilIgnoreCmL2caMsgHandler,                             /* CSR_BT_CM_L2CA_DATA_CFM */
        obexUtilIgnoreCmL2caMsgHandler,                             /* CSR_BT_CM_L2CA_DATA_IND */
        obexUtilCmL2caDisconnectIndHandler,                         /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        obexUtilIgnoreCmL2caMsgHandler,                             /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
        obexUtilIgnoreCmL2caMsgHandler,                             /* CSR_BT_CM_L2CA_DATA_ABORT_CFM */
    },

    /* CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTED_S */
    {
        NULL,                                                       /* CSR_BT_CM_L2CA_REGISTER_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        obexUtilCmL2caDataCfmHandler,                               /* CSR_BT_CM_L2CA_DATA_CFM */
        obexUtilCmL2caDataIndHandler,                               /* CSR_BT_CM_L2CA_DATA_IND */
        obexUtilCmL2caDisconnectIndHandler,                         /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        obexUtilCmL2caModeChangeIndHandler,                         /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
        obexUtilCmL2caDataAbortCfmHandler,                          /* CSR_BT_CM_L2CA_DATA_ABORT_CFM */
    },

    /* CSR_BT_OBEX_UTIL_BT_DISCONNECTING_S */
    {
        NULL,                                                       /* CSR_BT_CM_L2CA_REGISTER_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        obexUtilIgnoreCmL2caMsgHandler,                             /* CSR_BT_CM_L2CA_DATA_CFM */
        obexUtilIgnoreCmL2caMsgHandler,                             /* CSR_BT_CM_L2CA_DATA_IND */
        obexUtilCmL2caDisconnectIndHandler,                         /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        obexUtilCmL2caModeChangeIndHandler,                         /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
        obexUtilIgnoreCmL2caMsgHandler,                             /* CSR_BT_CM_L2CA_DATA_ABORT_CFM */
    },

    /* CSR_BT_OBEX_UTIL_BT_ACTIVATED_S */
    {
        obexUtilCmL2caRegisterCfmActivatedStateHandler,             /* CSR_BT_CM_L2CA_REGISTER_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_CONNECT_CFM */
        obexUtilCmL2caConnectAcceptCfmActivatedStateHandler,        /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        obexUtilCmL2caCancelConnectAcceptCfmHandler,                /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_DATA_CFM */
        obexUtilIgnoreCmL2caMsgHandler,                             /* CSR_BT_CM_L2CA_DATA_IND */
        obexUtilCmL2caDisconnectIndHandler,                         /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        obexUtilIgnoreCmL2caMsgHandler,                             /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
        obexUtilIgnoreCmL2caMsgHandler,                             /* CSR_BT_CM_L2CA_DATA_ABORT_CFM */
    },

    /* CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S */
    {
        obexUtilCmL2caRegisterCfmDeactivatingStateHandler,          /* CSR_BT_CM_L2CA_REGISTER_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_CONNECT_CFM */
        obexUtilCmL2caConnectAcceptCfmHandler,                      /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        obexUtilCmL2caCancelConnectAcceptCfmDeactivateStateHandler, /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        obexUtilIgnoreCmL2caMsgHandler,                             /* CSR_BT_CM_L2CA_DATA_CFM */
        obexUtilIgnoreCmL2caMsgHandler,                             /* CSR_BT_CM_L2CA_DATA_IND */
        obexUtilCmL2caDisconnectIndHandler,                         /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                                       /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        obexUtilCmL2caModeChangeIndHandler,                         /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
        obexUtilIgnoreCmL2caMsgHandler,                             /* CSR_BT_CM_L2CA_DATA_ABORT_CFM */
    },
};
#endif

/*------------------------------------------------------------------------------*
 * Public Obex Util functions which is common, e.g is used by server and client *
 *------------------------------------------------------------------------------*/
void * ObexUtilInit(CsrSchedQid pHandle,
                    void        *instData,
                    CsrUint8    instIdentifier)
{ /* Allocates the Obex util instance data */
    ObexUtilInstType * inst     = (ObexUtilInstType *) CsrPmemAlloc(sizeof(ObexUtilInstType));
    inst->phandle               = pHandle;
    inst->instId                = instIdentifier;
    inst->profilesOwnInstData   = instData;
    inst->privateInst           = NULL;
    return (inst);
}

void ObexUtilDeinit(void ** obexUtilInstData)
{ /* Pfree the instance data allocated by ObexUtilInit and all private inst data*/
    ObexUtilInstType * inst = *obexUtilInstData;

    if (inst)
    { /* Deallocate priInst and inst */
        obexUtilDeallocatePrivateInstData(inst);
        CsrPmemFree(inst);
        *obexUtilInstData = NULL;
    }
    else
    { /* Nothing to deallocate */
        ;
    }
}

CsrUint8 ObexUtilCmMsgHandler(void * obexUtilInstData, void **msg)
{
    ObexUtilInstType * inst = obexUtilInstData;
    CsrPrim *type           = *msg;

    if (inst && inst->privateInst)
    { /* This function is allowed to be called because the obexUtilInstData and
         privateInst data is allocated */
        ObexUtilPrivateInstType * priInst = inst->privateInst;

        if ((priInst->cmState < CSR_BT_OBEX_UTIL_BT_ACTIVATED_S) ||
            (priInst->cmState < CSR_BT_OBEX_UTIL_BT_RFC_CONNECTING_S && priInst->servInst))
        { /* A transport connection is established or being released for the
             client. Find out if the Cm upstream message comes from RFCOMM or
             L2CAP */
            if (*type <= CSR_BT_CM_RFC_PRIM_UPSTREAM_HIGHEST)
            { /* This CM upsteam messages comes from RFCOMM */
                CsrUint16 index = (CsrUint16)(*type - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST);
                if (obexUtilCmRfcUpstreamMsgStateHandlers[priInst->cmState][index] != NULL)
                { /* The library is able to handle the received message */
                    obexUtilCmRfcUpstreamMsgStateHandlers[priInst->cmState][index](priInst, *msg);
                    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
                }
            }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            else if (*type >= CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST &&
                     *type <= CSR_BT_CM_L2CA_PRIM_UPSTREAM_HIGHEST)
            { /* This CM upsteam messages comes from L2CAP */
                CsrUint16 index = (CsrUint16)(*type - CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST);
                if (obexUtilCmL2capUpstreamMsgStateHandlers[priInst->cmState][index] != NULL)
                { /* The library is able to handle the received message */
                    obexUtilCmL2capUpstreamMsgStateHandlers[priInst->cmState][index](priInst, msg);
                    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
                }
            }
#endif
            else if (*type == CSR_BT_CM_SDS_EXT_REGISTER_CFM)
            { /* This CM upsteam messages comes from SDS and indicates that
                 the profile has try to register a service record */
                obexUtilCmSdsRegisterCfmHandler(priInst, *msg);
                return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
            }
            else if (*type == CSR_BT_CM_SDS_EXT_UNREGISTER_CFM)
            { /* This CM upsteam messages comes from SDS and indicates that
                 the profile has try to un-register a service record */
                obexUtilCmSdsUnregisterCfmHandler(priInst, *msg);
                return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
            }
            else if (*type == CSR_BT_CM_DATA_BUFFER_EMPTY_CFM)
            { /* A complete OBEX response packet is send. */
                if (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S)
                { /* Activate the lpSuperVisionTimer */
                    obexUtilActivateLpSuperVisionTimer(priInst);
                }
                return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
            }
        }
        else if (priInst->cliInst)
        { /* A client is establishing a transport connection. Find out if it is
             a RFCOMM or L2CAP connection that is being establish */
            if(priInst->cmState == CSR_BT_OBEX_UTIL_BT_RFC_CONNECTING_S && CsrBtUtilRfcConVerifyCmMsg(*msg))
            { /* An RFCOMM connection is being establish */
                if (CsrBtUtilRfcConCmMsgHandler(priInst, priInst->cliInst->rfcConnectInst, *msg))
                {
                    if (priInst->cmState == CSR_BT_OBEX_UTIL_BT_RFC_CONNECTED_S)
                    { /* The rfcomm transport connect has been established and
                         the OBEX Connect Request Packet is being send. See the
                         function obexUtilCmRfcConnectResultHandler */
                        ;
                    }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                    else if (priInst->cmState == CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTING_S)
                    { /* The RFCOMM connect procedure were cancel because the
                         peer device supports OBEX over l2cap, and a Dynamic
                         PSM that support enhanced retransmission mode is being
                         registered. See the function
                         obexUtilCmRfcConnectResultHandler */
                        ;
                    }
#endif
                    else
                    { /* The rfcomm transport connection were not established */
                        obexUtilObexConnectRequestResultHandler(priInst);
                    }
                }
                else
                { /* just ignore this procedure is not finish yet */
                    ;
                }
                return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
            }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            else if (priInst->cmState == CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTING_S)
            {
                if (*type == CSR_BT_CM_L2CA_CONNECT_CFM)
                { /* Find out if the L2CAP connection has been establish or not */
                    obexUtilCmL2caConnectCfmHandler(priInst, *msg);
                    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
                }
                else if (*type == CSR_BT_CM_L2CA_REGISTER_CFM)
                { /* Find out if a Dynamic PSM is obtained or not */
                    obexUtilCmL2caRegisterCfmCmConnectingStateHandler(priInst, *msg);
                    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
                }
            }
#endif
        }
    }
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    if (*type == CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM)
    { /* No action required for this signal */
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* This library function is not able to handle the incoming CM
         message. Return CSR_BT_OBEX_UTIL_STATUS_EXCEPTION as it is
         consider as a state event error has occurred */
        return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
    }
#else
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
#endif
}

/*------------------------------------------------------------------------------*
 * Public Obex Request functions/Client functions                               *
 *------------------------------------------------------------------------------*/
CsrUint8 ObexUtilConnectRequestEx(void                               *obexUtilInstData,
                                CmnCsrBtLinkedListStruct           **sdpTag,
                                CsrBtDeviceAddr                    deviceAddr,
                                dm_security_level_t                outgoingSecurityLevel,
                                CsrUint16                          maxLocalObexPacketLength,
                                CsrUint16                          rfcommMtu,
                                CsrUint8                           **target,
                                CsrUint8                           targetHeaderLength,
                                CsrUint8                           numOfHeaders,
                                const CsrUint8                     *headerPriorityTable,
                                CsrTime                            lpSuperVisionTimeout,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                ObexUtilDigestChallengeType        **digestChallenge,
#endif
                                CsrUint32                          length,
                                CsrUint32                          count,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                CsrUcs2String                      **description,
#endif                                
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                CsrUint16                          userDefinedHeaderLength,
                                CsrUint8                           **userDefinedHeader,
#endif                                
                                ObexUtilConnectCfmFuncType         connectResultHandler,
                                ObexUtilAuthenticateIndFuncType    authenticateIndHandler,
                                ObexUtilDisconnectIndFuncType      disconnectIndHandler,
                                ObexUtilSelectSdpRecordFuncType    selectSdpRecordIndHandler,
                                CsrUint16                          windowSize,
                                CsrBool                            localSrmEnable)

{ /* Start the OBEX CONNECT Request procedure. */
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    CsrUint8 result;
    
    if (obexUtilInstData && sdpTag)
    { /* The obexUtilInstData is present, Check if the profile is allowed
         to called this function */
        ObexUtilInstType * inst = obexUtilInstData;

        if (!inst->privateInst && connectResultHandler && disconnectIndHandler)
        { /* Allow to start an OBEX Connect procedure */
#else
            ObexUtilInstType * inst = obexUtilInstData;
#endif
            CsrUint8 modemStatus = CSR_BT_MODEM_SEND_CTRL_DCE_DEFAULT;
            /* Allocates the common private instance data */
            ObexUtilPrivateInstType * priInst = (ObexUtilPrivateInstType *)
                CsrPmemAlloc(sizeof(ObexUtilPrivateInstType));

            /* Allocates the client private instance data */
            ObexUtilCliInstType     * cliInst = (ObexUtilCliInstType *)
                CsrPmemAlloc(sizeof(ObexUtilCliInstType));

            /* Initialize the common private instance data */
            obexUtilInitPrivateInstData(inst, priInst, localSrmEnable);

            /* Initialize the client private instance data */
            obexUtilInitClientInstData(cliInst);

            if (selectSdpRecordIndHandler)
            {
                cliInst->selectSdpRecordFunc  = selectSdpRecordIndHandler;
                cliInst->rfcConnectInst       = CsrBtUtilSdpRfcInit(obexUtilSelectSdpRecordHandler,
                                                                           NULL,
                                                                           obexUtilCmRfcConnectResultHandler,
                                                                           inst->phandle,
                                                                           inst->instId);
            }
            else
            {
                cliInst->selectSdpRecordFunc  = NULL;
                cliInst->rfcConnectInst       = CsrBtUtilSdpRfcInit(obexUtilCliSdcSelectServiceHandler,
                                                                           NULL,
                                                                           obexUtilCmRfcConnectResultHandler,
                                                                           inst->phandle,
                                                                           inst->instId);
            }

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
            cliInst->authResultFunc         = authenticateIndHandler;
#endif
            cliInst->connectResultHandler   = connectResultHandler;
            cliInst->disconnectResultFunc   = disconnectIndHandler;
            priInst->cliInst                = cliInst;

            if (CsrBtUtilRfcConStart(priInst, cliInst->rfcConnectInst, *sdpTag,
                                     deviceAddr, outgoingSecurityLevel, FALSE, NULL,
                                     rfcommMtu, modemStatus, CSR_BT_DEFAULT_MSC_TIMEOUT))
            { /* Failed to start the RFC connect procedure. Note that the
                 profile is notify through the obexUtilCmRfcConnectResultHandler
                 which is called by the CMN RFC CONNECT UTIL library, and that
                 sdpTag pointer it also own by this library.This is the reason
                 why it is not free here. */
                ;
            }
            else
            { /* The RFC connect procedure is started */

                /* Find out if the calling profile allready have an assign a
                   local serverchannel or not. */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                priInst->windowSize                 = (CsrUint16)(windowSize != 0 ? windowSize : CSR_BT_OBEX_UTIL_TX_WINDOW);
                priInst->rxQueueCount               = priInst->windowSize;
#else
                CSR_UNUSED(windowSize);
#endif
                cliInst->obtainedServer             = obexUtilAssignLocalServerChannel(*sdpTag);
                cliInst->outgoingSecurityLevel      = outgoingSecurityLevel;
#ifdef CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID
                priInst->targetWhoHeaderLength      = targetHeaderLength;
                obexUtilSetPtrFromDoublePtr((void **)&priInst->targetWho, (void **)target);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                priInst->userDefinedHeaderLength    = userDefinedHeaderLength;
                obexUtilSetPtrFromDoublePtr((void **)&priInst->userDefinedHeader, (void **)userDefinedHeader);
#endif
                priInst->deviceAddr                 = deviceAddr;
                priInst->lengthOfObject             = length;
                priInst->count                      = count;
                priInst->lpSuperVisionTimeout       = lpSuperVisionTimeout;
                priInst->cmState                    = CSR_BT_OBEX_UTIL_BT_RFC_CONNECTING_S;
                priInst->obexOperationIndex         = CSR_BT_OBEX_UTIL_CONNECT_REQUEST_RESULT_HANDLER;
                priInst->maxLocalObexPacketLength   = maxLocalObexPacketLength;
                inst->privateInst                   = priInst;
                
                if (priInst->maxLocalObexPacketLength > CSR_BT_MAX_OBEX_SIGNAL_LENGTH)
                {
                    priInst->maxLocalObexPacketLength = CSR_BT_MAX_OBEX_SIGNAL_LENGTH;
                }
                
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                if (digestChallenge)
                {
                    obexUtilBuildObexChallengePointer(priInst, *digestChallenge);
                    *digestChallenge = NULL;
                }
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                obexUtilSetPtrFromDoublePtr((void **)&priInst->description, (void **) description);
#endif
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_RFC_CONNECTING_P_S);

                /* Update the table which defines which OBEX headers must be
                   included in the OBEX Connect Request packet. */
                obexUtilUpdateHeaderTable(priInst, CSR_BT_OBEX_UTIL_CONNECT_REQUEST_OPERATION,
                                          numOfHeaders, headerPriorityTable);
                /* Note that there is no need to check the result of this
                   function, as the peer device maximum OBEX packet length is
                   not known */
                obexUtilValidateObexPacket(priInst, FALSE, 0);
            }
            *sdpTag = NULL;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
            result  = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED; 
        }
        else
        { /* This procedure allready running, or the one of the mandatory
             (e.g. connectResultHandler, disconnectHandler) callback functions
             is not defined.  */
            result = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION; 
        }
    }
    else
    { /* No obexUtilInstData is present or the parameter sdpTag  is NULL */
        result = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
    }
#endif
    /* Make sure that all the pointers is CsrPmemFree to prevent a memoryleak */
#ifdef CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID
    obexUtilPfreeDoublePtr((void **) target);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    obexUtilPfreeDoublePtr((void **) description);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    obexUtilPfreeDoublePtr((void **) userDefinedHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    ObexUtilPfreeDigestChallengePointer(digestChallenge);
#endif    
    if (sdpTag)
    {
        CsrBtUtilBllFreeLinkedList(sdpTag, CsrBtUtilBllPfreeWrapper);
    }
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    return (result);
#else
    return CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
#endif
}

#ifdef CSR_BT_INSTALL_OBEX_UTIL_SET_SERVICE_HANDLE_RES
CsrUint8 ObexUtilSetServiceHandleIndexListResponse(void            *obexUtilInstData,
                                                   CsrUint16       **serviceHandleIndexList,
                                                   CsrUint16       nofServiceHandleIndicis)
{
    CsrUint8 result = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_CLIENT_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif

    if (priInst)
    { /* The profile is allow to call this function */
        if (priInst->processState == CSR_BT_OBEX_UTIL_BT_RFC_CONNECTING_P_S)
        {
            if (nofServiceHandleIndicis > 0 && serviceHandleIndexList && *serviceHandleIndexList)
            {
                CsrBtUtilRfcConSetServiceHandleIndexList(priInst,
                                                         priInst->cliInst->rfcConnectInst,
                                                         *serviceHandleIndexList,
                                                         nofServiceHandleIndicis);
                *serviceHandleIndexList = NULL;
            }
            else
            {
                CsrBtUtilRfcConCancel(priInst, priInst->cliInst->rfcConnectInst);
            }
            result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
        }
    }
    /* Make sure that serviceHandleIndexList is CsrPmemFree */
    obexUtilPfreeDoublePtr((void **) (void **) serviceHandleIndexList);
    return (result);
}
#endif

CsrUint8 ObexUtilDisconnectRequestExt(void          *obexUtilInstData,
                                   CsrBool       normalDisconnect
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                   , CsrUcs2String **description
#endif                                   
                                   )
{ /* This function will either cancel an ongoing OBEX CONNECT operation
     or release the OBEX connection if it is allready connected. */
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_CLIENT_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif   
    
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    if (priInst)
    { /* The profile is allow to call this function */
#endif
        /* Deallocate all header pointers */
        obexUtilDeallocatePrivateInstHeaderPointers(priInst, TRUE);

        /* Initialize all header pointers */
        obexUtilInitPrivateInstHeaderPointers(priInst, TRUE);

        /* Reset the ObexCmdSuperVisionTimer */
        obexUtilResetObexCmdSuperVisionTimer(priInst);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        obexUtilSetPtrFromDoublePtr((void **)&priInst->description, (void **) description);
#endif
        if (obexUtilObexDisconnectRequestHandler(priInst, normalDisconnect))
        { /* An OBEX Disconnect Request packet must be issue */
            obexUtilLowpowerHandler(priInst);

            /* Start the ObexCmdSuperVisionTimer in order to ensure that that
               transport connection is release if the server do not response
               the OBEX Disconnect Request message */
            obexUtilActivateObexCmdSuperVisionTimer(priInst,
                                                    CSR_BT_OBEX_DISCONNECT_TIMEOUT,
                                                    CSR_BT_OBEX_DISCONNECT_OPCODE);

            priInst->obexOperationIndex = CSR_BT_OBEX_UTIL_DISCONNECT_REQUEST_RESULT_HANDLER;
            obexUtilBuildTransportPacket(priInst, FALSE);
        }
        else
        { /* Either the OBEX Connect procedure is being cancel or the
             transport connection is being release without issueing
             a OBEX Diconnect Request. */
            ;
        }
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    }
    else
    { /* The profile is currently not allowed to call this function. Return
         Accepted because this function call can cross with an abnormal
         disconnect of the transport connection */
    }
#endif
    /* Make sure that description is CsrPmemFree */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    obexUtilPfreeDoublePtr((void **) (void **) description);
#endif
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 ObexUtilAbortRequestEx(void                          *obexUtilInstData,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                              CsrUcs2String                 **description,
#endif
                              ObexUtilCommonRequestFuncType abortResultHandler)
{ /* Start the OBEX ABORT Request procedure */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    CsrUint8 *descrip;
#endif
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_CLIENT_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif 

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    obexUtilSetPtrFromDoublePtr((void **)&descrip, (void **) description);
#endif
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK 
    if (priInst && abortResultHandler)
#endif 
    {
        ObexUtilCliInstType * cliInst = priInst->cliInst;

        switch (priInst->processState)
        {
            case CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S:
                { /* Nothing to abort, call the abort callback
                     function with the responseCode CSR_BT_OBEX_SUCCESS_RESPONSE_CODE.
                     Make sure that the descrip pointer is CsrPmemFree to prevent
                     a memoryleak */
                    abortResultHandler(priInst->profilesOwnInstData, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, NULL, 0);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                    CsrPmemFree(descrip);
#endif
                    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
                }
#ifdef CSR_BT_INSTALL_OBEX_CLI_SET_PATH
            case CSR_BT_OBEX_UTIL_OBEX_SETPATH_REQUEST_P_S:
#endif                
            case CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_P_S:
            case CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_SPLIT_P_S:
            case CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_SPLIT_P_S:
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            case CSR_BT_OBEX_UTIL_OBEX_ACTION_REQUEST_P_S:
#endif
                { /* Is in the middle of either an OBEX SETPATH/PUT/GET/ACTION
                     operation. */
                    cliInst->commonResultFunc = abortResultHandler;
                    obexUtilClearSingleResponseModeState(priInst);
                    obexUtilObexAbortRequestHandler(priInst, descrip, 2, TRUE);
                    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
                }
            case CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_P_S:
                { /* Is in the middle of an OBEX GET Request procedure */
                    cliInst->commonResultFunc = abortResultHandler;
                    obexUtilObexAbortRequestHandler(priInst, descrip, 2, TRUE);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                    if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE)
                    { /* Single Response Mode is enable, we must be able
                         handle that the server send many OBEX Response packets */
                        priInst->srmState = CSR_BT_OBEX_UTIL_SRM_REJECTED;
                    }
                    else
                    { /* Single Response Mode is not enabled */
                        obexUtilClearSingleResponseModeState(priInst);
                    }
#endif
                    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
                }
            case CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_PENDING_P_S:
                { /* Is in the middle of an OBEX PUT Request procedure. No OBEX
                     packet is currently being sent, as it is waiting for a reply
                     from the profile, e.g. a ObexUtilPutContinueRequest */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                    switch (priInst->srmState)
                    { /* Check Single Response Mode state */
                        case CSR_BT_OBEX_UTIL_SRM_ACTIVE:
                            { /* Single Response Mode is enable. Build and sent the OBEX
                                 ABORT Request packet, and set number of response
                                 to two as the server may reject this PUT operation
                                 before it receives the OBEX ABORT Request */
                                cliInst->commonResultFunc = abortResultHandler;
                                obexUtilObexAbortRequestHandler(priInst, descrip, 2, TRUE);
                                break;
                            }
                        case CSR_BT_OBEX_UTIL_SRM_REJECTED:
                            { /* The Server has rejected a multi PUT operation while
                                 Single Response Mode were enable. Nothing to Abort.
                                 Call the abort callback function with the responseCode
                                 CSR_BT_OBEX_SUCCESS_RESPONSE_CODE and make sure that
                                 the descrip pointer is CsrPmemFree to prevent a
                                 memoryleak */
                                CsrPmemFree(priInst->incomingObexPacket);
                                priInst->incomingObexPacket         = NULL;
                                priInst->incomingObexPacketLength   = 0;
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                CsrPmemFree(descrip);
#endif
                                abortResultHandler(priInst->profilesOwnInstData,
                                                   CSR_BT_OBEX_SUCCESS_RESPONSE_CODE,
                                                   NULL,
                                                   0);
                                break;
                            }
                        default:
                            { /* Single Response Mode is not used. Build and sent the
                                 OBEX ABORT Request packet, and set number of response
                                 to one because this will be the next packet the server
                                 must respond to */
                                cliInst->commonResultFunc = abortResultHandler;
                                obexUtilObexAbortRequestHandler(priInst, descrip, 2, TRUE);
                                break;
                            }
                    }
                    obexUtilClearSingleResponseModeState(priInst);
#else
                    /*  Build and sent the OBEX ABORT Request packet, and set
                        number of response to one because this will be the next
                        packet the server must respond to */
                    cliInst->commonResultFunc = abortResultHandler;
                    obexUtilObexAbortRequestHandler(priInst, descrip, 2, TRUE);
#endif
                    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
                }
            case CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_PENDING_P_S:
                { /* Is in the middle of an OBEX GET Request procedure.
                     No OBEX packet is currently being sent, currently waiting
                     for a reply from the profile, e.g. a
                     ObexUtilGetContinueRequest. Build and sent the OBEX ABORT
                     Request packet. Please note that this library never can be
                     in this state if Single Response Mode is enabled */
                    cliInst->commonResultFunc = abortResultHandler;
                    obexUtilObexAbortRequestHandler(priInst, descrip, 1, TRUE);
                    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
                }
            case CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_IDLE_PENDING_P_S:
            case CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_IDLE_PENDING_P_S:
                { /* The OBEX PUT/GET (with body( Request procedure has never been
                     startet, currently waiting for a reply from the profile, e.g. a
                     ObexUtilPutContinueRequest or ObexUtilGetBodyRequest where the
                     body header is piggy bag along with the others headers.
                     Make sure that the descrip pointer is CsrPmemFree to prevent
                     a memoryleak */
                    cliInst->commonResultFunc           = abortResultHandler;
                    priInst->responseCode               = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                    obexUtilCommonObexRequestOperationResultHandler(priInst);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                    CsrPmemFree(descrip);
#endif
                    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
                }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            case CSR_BT_OBEX_UTIL_OBEX_FLUSH_PUT_REQUEST_PENDING_P_S:
            { /* All unsent SDUs is being flush because an OBEX response 
                 packet with an error response were received during a multiple
                 PUT operation with SRM is enable. The application is not notified 
                 about this until a CsrBtCmL2caDataAbortCfm is received. 
                 Change the callback function cliInst->commonResultFunc
                 to abortResultHandler so this function when CsrBtCmL2caDataAbortCfm 
                 is received. Note the saved OBEX response packet must be CsrPmemFree and 
                 priInst->responseCode must be set to CSR_BT_OBEX_SUCCESS_RESPONSE_CODE
                 because the saved OBEX response message must be a PUT 
                 response message with an error */
                CsrPmemFree(priInst->incomingObexPacket);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                CsrPmemFree(descrip);
#endif
                priInst->incomingObexPacket       = NULL;    
                priInst->incomingObexPacketLength = 0;
                priInst->responseCode             = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
                cliInst->commonResultFunc         = abortResultHandler;
                return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
            }
#endif
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
            default:
                { /* The processState is either:
                     CSR_BT_OBEX_UTIL_OBEX_ABORT_REQUEST_P_S
                     CSR_BT_OBEX_UTIL_OBEX_ABORT_REQUEST_PENDING_P_S
                     CSR_BT_OBEX_UTIL_OBEX_DISCONNECT_REQUEST_P_S
                     CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECT_REQUEST_P_S
                     CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S
                     CSR_BT_OBEX_UTIL_BT_DISCONNECT_NORMAL_REQUEST_P_S
                     CSR_BT_OBEX_UTIL_BT_RFC_CONNECTING_P_S
                     CSR_BT_OBEX_UTIL_BT_L2CA_REGISTER_PSM_P_S
                     CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTING_P_S
                     CSR_BT_OBEX_UTIL_OBEX_CONNECT_REQUEST_P_S
                     which means that the profile is currently not allowed to call
                     this function. Make sure that the descrip pointer is CsrPmemFree
                     to prevent a memoryleak */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                    CsrPmemFree(descrip);
#endif
                    return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
                }
#endif 
        }
    }
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    else
    { /* The profile is currently not allowed to call this function, make sure
         that the descrip pointer is CsrPmemFree to prevent a memoryleak. */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        CsrPmemFree(descrip);
#endif
        return (obexUtilClientUnhandleFunctionCall(priInst));
    }
#else
    return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION); /* This will never happen, since we will return in the switch() */
#endif 
}

#ifdef CSR_BT_INSTALL_OBEX_CLI_SET_PATH
CsrUint8 ObexUtilSetPathRequestEx(void                             *obexUtilInstData,
                                CsrUint8                         flags,
                                CsrUint8                         headerPriorityTable[3],
                                CsrUcs2String                    **name,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                CsrUcs2String                    **description,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                CsrUint16                        userDefinedHeaderLength,
                                CsrUint8                         **userDefinedHeader,
#endif
                                ObexUtilCommonRequestFuncType    setPathResultHandler)
{ /* Start the OBEX SETPATH Request procedure */
    CsrUint8 result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_CLIENT_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif   

    if (priInst && setPathResultHandler && priInst->processState == CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S)
    { /* The profile is allow to call this function */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
        priInst->userDefinedHeaderLength    = userDefinedHeaderLength;
        obexUtilSetPtrFromDoublePtr((void **)&priInst->userDefinedHeader, (void **)userDefinedHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        obexUtilSetPtrFromDoublePtr((void **)&priInst->description, (void **) description);
#endif
        obexUtilSetPtrFromDoublePtr((void **)&priInst->name, (void **) name);

        /* Update the table which defines which OBEX headers must be included in
           the OBEX Setpath Request packet */
        obexUtilUpdateHeaderTable(priInst, CSR_BT_OBEX_UTIL_SETPATH_REQUEST_OPERATION,
                                  3, headerPriorityTable);

        if (obexUtilValidateObexPacket(priInst, FALSE, 0) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
        { /* The OBEX Setpath Request do fit into one OBEX packet, and the
             instance data regarding building and sending the OBEX packet
             is set. Start to build and send the OBEX Setpath Request packet */
            ObexUtilCliInstType * cliInst        = priInst->cliInst;

            obexUtilLowpowerHandler(priInst);
            priInst->obexOperationIndex     = CSR_BT_OBEX_UTIL_SETPATH_REQUEST_RESULT_HANDLER;
            cliInst->setpathFlags           = flags;
            cliInst->commonResultFunc       = setPathResultHandler;
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_SETPATH_REQUEST_P_S);
            obexUtilBuildTransportPacket(priInst, FALSE);
        }
        else
        { /* The Setpath Request packet is to large, call the setpath callback
             function with the responseCode CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE */
            obexUtilDeallocatePrivateInstHeaderPointers(priInst, TRUE);
            obexUtilInitPrivateInstHeaderPointers(priInst, TRUE);
            setPathResultHandler(priInst->profilesOwnInstData, CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE, NULL, 0);
        }
    }
    else
    { /* The profile is currently not allowed to call this function, make sure
         that all the pointers is CsrPmemFree to prevent a memoryleak. */
        result= obexUtilClientUnhandleFunctionCall(priInst);
    }
    /* Free pointers if they are not stored */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    obexUtilPfreeDoublePtr((void **) description);
#endif
    obexUtilPfreeDoublePtr((void **) name);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    obexUtilPfreeDoublePtr((void **) userDefinedHeader);
#endif
    return result;
}
#endif /* CSR_BT_INSTALL_OBEX_SRV_SET_PATH */

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
CsrUint8 ObexUtilActionRequestEx(void                          *obexUtilInstData,
                               CsrUint8                      actionId,
                               CsrUint8                      headerPriorityTable[2],
                               CsrUcs2String                 **name,
                               CsrUcs2String                 **destName,
                               CsrUint32                     permission,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                               CsrUcs2String                 **description,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                               CsrUint16                     userDefinedHeaderLength,
                               CsrUint8                      **userDefinedHeader,
#endif
                               ObexUtilCommonRequestFuncType actionResultHandler)
{ /* Start the OBEX ACTION Request procedure */
    CsrUint8 result;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_CLIENT_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif   

    if (priInst && actionResultHandler && priInst->processState == CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S)
    {
        /* The profile is allow to call this function */
        if (CSR_BT_CONN_ID_IS_L2CA(priInst->btConnId)
            && actionId <= CSR_BT_OBEX_UTIL_ACTION_ID_SET_OBJECT_PERMISSIONS)
        {
            /* Is allowed to use the Action command because L2CAP is
             * used as the transport */
            CsrUint8 priorityTable[CSR_BT_OBEX_UTIL_TOTAL_NUM_OF_ACTION_CMD_HEADERS];
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
            priInst->userDefinedHeaderLength = userDefinedHeaderLength;
            obexUtilSetPtrFromDoublePtr((void **)&priInst->userDefinedHeader, (void **)userDefinedHeader);
#endif
            priInst->actionId                = actionId;
            priorityTable[0]                 = CSR_BT_OBEX_UTIL_ACTION_ID_HEADER;
            priorityTable[1]                 = CSR_BT_OBEX_UTIL_NAME_HEADER;
            obexUtilSetPtrFromDoublePtr((void **)&priInst->name, (void **) name);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
            obexUtilSetPtrFromDoublePtr((void **)&priInst->description, (void **) description);
#endif

            if (priInst->actionId == CSR_BT_OBEX_UTIL_ACTION_ID_SET_OBJECT_PERMISSIONS)
            {
                priInst->permission = permission;
                priorityTable[2]    = CSR_BT_OBEX_UTIL_PERMISSIONS_HEADER;
            }
            else
            {
                obexUtilSetPtrFromDoublePtr((void **)&priInst->destName, (void **) destName);
                priorityTable[2]  = CSR_BT_OBEX_UTIL_DESTNAME_HEADER;
            }
            priorityTable[3] = headerPriorityTable[0];
            priorityTable[4] = headerPriorityTable[1];

            /* Update the table which defines which OBEX headers must be
               included in the OBEX Action Request packet */
            obexUtilUpdateHeaderTable(priInst,
                                      CSR_BT_OBEX_UTIL_ACTION_REQUEST_OPERATION,
                                      CSR_BT_OBEX_UTIL_TOTAL_NUM_OF_ACTION_CMD_HEADERS,
                                      priorityTable);

            if (obexUtilValidateObexPacket(priInst, FALSE, 0) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
            { /* The OBEX Action Request do fit into one OBEX packet, and the
                 instance data regarding building and sending the OBEX packet
                 is set. Start to build and send the OBEX Action Request packet */
                ObexUtilCliInstType * cliInst   = priInst->cliInst;

                obexUtilLowpowerHandler(priInst);
                priInst->obexOperationIndex     = CSR_BT_OBEX_UTIL_ACTION_REQUEST_RESULT_HANDLER;
                cliInst->commonResultFunc       = actionResultHandler;
                obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_ACTION_REQUEST_P_S);
                obexUtilBuildTransportPacket(priInst, FALSE);
            }
            else
            { /* The Action Request packet is to large, call the action callback
                 function with the responseCode
                 CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE */
                obexUtilDeallocatePrivateInstHeaderPointers(priInst, TRUE);
                obexUtilInitPrivateInstHeaderPointers(priInst, TRUE);
                actionResultHandler(priInst->profilesOwnInstData,
                                    CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE,
                                    NULL,
                                    0);
            }
        }
        else
        { /* RFCOMM is use as transport. E.g. the Action command is not
             supported */
            actionResultHandler(priInst->profilesOwnInstData,
                                CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE,
                                NULL,
                                0);

        }
        result = (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* The profile is currently not allowed to call this function, make sure
         that all the pointers is CsrPmemFree to prevent a memoryleak. */
        result = obexUtilClientUnhandleFunctionCall(priInst);
    }

    /* make sure that all the pointers is CsrPmemFree to prevent a memoryleak */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    obexUtilPfreeDoublePtr((void **) description);
#endif
    obexUtilPfreeDoublePtr((void **) name);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    obexUtilPfreeDoublePtr((void **) userDefinedHeader);
#endif
    obexUtilPfreeDoublePtr((void **) destName);
    return (result);
}
#endif 

CsrUint8 ObexUtilPutRequestEx(void                                 *obexUtilInstData,
                            CsrUint8                             headerPriorityTable[6],
                            CsrUint32                            lengthOfObject,
                            const CsrUint8                       *type,
                            CsrUcs2String                        **name,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                            CsrUcs2String                        **description,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                            CsrUint16                            appParametersHeaderLength,
                            CsrUint8                             **appParametersHeader,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                            CsrUint16                            userDefinedHeaderLength,
                            CsrUint8                             **userDefinedHeader,
#endif
                            ObexUtilPutContinueRequestFuncType   putContinueHandler,
                            ObexUtilCommonRequestFuncType        putResultHandler)
{
    /* Start the OBEX Put Request procedure */
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_CLIENT_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif   

    if (priInst && putResultHandler && priInst->processState == CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S)
    {
        /* The profile is allow to call this function */
        CsrBtObexResponseCode responseCode;

        ObexUtilCliInstType * cliInst       = priInst->cliInst;
        priInst->lengthOfObject             = lengthOfObject;
        priInst->type                       = obexUtilBuildObexTypePointer(type, &priInst->typeLength);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
        priInst->userDefinedHeaderLength    = userDefinedHeaderLength;
        obexUtilSetPtrFromDoublePtr((void **)&priInst->userDefinedHeader, (void **)userDefinedHeader);
#endif
        priInst->obexOperationIndex         = CSR_BT_OBEX_UTIL_PUT_REQUEST_RESULT_HANDLER;
        cliInst->commonResultFunc           = putResultHandler;
        cliInst->putContinueFunc            = putContinueHandler;
        obexUtilSetPtrFromDoublePtr((void **)&priInst->name, (void **) name);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        obexUtilSetPtrFromDoublePtr((void **)&priInst->description, (void **) description);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
        priInst->appParametersHeaderLength  = appParametersHeaderLength;
        obexUtilSetPtrFromDoublePtr((void **)&priInst->appParametersHeader, (void **) appParametersHeader);    
#endif        

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        if (putContinueHandler)
        {
            /* This can be a multiple PUT operation. Check if SRM must be enable */
            obexUtilCliSingleResponseModeInitiator(priInst, CSR_BT_OBEX_SRMP_INVALID);
        }
        else
        {
            obexUtilClearSingleResponseModeState(priInst);
        }
#endif
        /* Update the table which defines which OBEX headers must be
         * included in the OBEX Put Request packet */
        obexUtilUpdateHeaderTable(priInst, CSR_BT_OBEX_UTIL_PUT_REQUEST_OPERATION,
                                  6, headerPriorityTable);

        responseCode = obexUtilValidateObexPacket(priInst, TRUE, 0);

        if (obexUtilObexPutRequestHandler(priInst, responseCode, FALSE))
        {
            /* The headers can be placed in one or more OBEX Put Request packets */
            obexUtilLowpowerHandler(priInst);
        }
        else
        {
            /* One the the headers is to large, call putResultHandler
             * with the responseCode
             * CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE */
            obexUtilDeallocatePrivateInstHeaderPointers(priInst, TRUE);
            obexUtilInitPrivateInstHeaderPointers(priInst, TRUE);
            putResultHandler(priInst->profilesOwnInstData,
                             CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE,
                             NULL, 0);
        }
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* The profile is currently not allowed to call this function, make sure
         that all the pointers is CsrPmemFree to prevent a memoryleak. Return Ignore
         because this function call can cross with an abnormal disconnect of
         the transport connection */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        obexUtilPfreeDoublePtr((void **) description);
#endif
        obexUtilPfreeDoublePtr((void **) name);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
        obexUtilPfreeDoublePtr((void **) userDefinedHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
        obexUtilPfreeDoublePtr((void **) appParametersHeader);    
#endif
        return (obexUtilClientUnhandleFunctionCall(priInst));
    }
}

CsrUint8 ObexUtilPutContinueRequest(void       *obexUtilInstData,
                                    CsrBool    finalFlag,
                                    CsrUint8   **body,
                                    CsrUint16  bodyLength)
{
    /* This function send a PUT Request packet and is part of a
     * multi-PUT operation */
    CsrUint8 result;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_CLIENT_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif   

    if (priInst)
    {
        /* The instance data used for the client part is valid */
        CsrUint8 bodyType;
        CsrUint8 obexOperation;

        if (!finalFlag)
        {
            /* This OBEX Put procedure consists of more request
             * packets, make sure that the final bit in the opcode is
             * not set (0x02), and the opcode for the body header is
             * set to Body (0x48) */
            bodyType            = CSR_BT_OBEX_UTIL_BODY_HEADER;
            obexOperation       = CSR_BT_OBEX_UTIL_PUT_REQUEST_OPERATION;
        }
        else
        {
            /* This is the last OBEX Put Request message, make sure
             * that the final bit in the opcode is set (0x82), and the
             * opcode for the body header is set to End of Body
             * (0x49) */
            bodyType            = CSR_BT_OBEX_UTIL_END_BODY_HEADER;
            obexOperation       = CSR_BT_OBEX_UTIL_PUT_FINAL_REQUEST_OPERATION;
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            obexUtilClearSingleResponseModeState(priInst);

            if (priInst->singleResponseModeHeader)
            {
                /* Remove the SRM header as this is not a multiple
                 * OBEX PUT operation. Note that the SRMP header
                 * cannot be initiated by a OBEX PUT Request operation
                 * so there is no need to remove the SRMP header */
                CsrPmemFree(priInst->singleResponseModeHeader);
                priInst->singleResponseModeHeader = NULL;
                priInst->outgoingObexPacketLength =
                    (CsrUint16)(priInst->outgoingObexPacketLength -
                                CSR_BT_OBEX_UTIL_SINGLE_RESPONSE_MODE_LENGTH);
            }
#endif
        }

        switch (priInst->processState)
        {
            /* Check if the profile is allow to call this function or not */
            case CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_PENDING_P_S:
                {
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
                    if (priInst->srmState == CSR_BT_OBEX_UTIL_SRM_REJECTED)
                    { /* The PUT operation was rejected by the server while 
                         SRM is enable and while waiting for this called,
                         Flush the l2cap buffer otherwise the server will 
                         continue to receive OBEX PUT Request messages and 
                         the next OBEX operation will be delay as all data 
                         in the buffer must be sent first. Note this 
                         feature is a CSR feature this is not demanded by 
                         the spec */
                         obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_FLUSH_PUT_REQUEST_PENDING_P_S);
                         CsrBtCmL2caDataAbortReqSend(priInst->btConnId);
                    }
                    else
#endif
                    {
                        /* Start building and sending the OBEX Put
                         * Request packet, where only the body header
                         * is included */
                        priInst->bodyLength = bodyLength;
                        obexUtilSetPtrFromDoublePtr((void **)&priInst->body, (void **) body);
                        obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_P_S);
                        obexUtilUpdateHeaderTable(priInst, obexOperation, 1, &bodyType);
                        obexUtilValidateObexPacket(priInst, FALSE, 0);
                        obexUtilBuildTransportPacket(priInst, FALSE);
                    }
                    result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
                    break;
                }

            case CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_IDLE_PENDING_P_S:
                {
                    /* This is the first OBEX Put Request packet that
                     * must be build and sent to the peer device. Add
                     * the body header to the table which defines
                     * which OBEX headers must be included in this
                     * OBEX packet */
                    priInst->headerTypeTable[0]                         = obexOperation;
                    priInst->headerTypeTable[priInst->numOfheaderTypes] = bodyType;
                    priInst->bodyLength                                 = bodyLength;
                    obexUtilSetPtrFromDoublePtr((void **)&priInst->body, (void **) body);
                    
                    priInst->outgoingObexPacketLength                   =
                        (CsrUint16)(obexUtilBodyLengthHandler(priInst) + priInst->outgoingObexPacketLength);
                    priInst->numOfheaderTypes++;
                    priInst->lastHeaderTypeIndex++;
                    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_P_S);
                    obexUtilBuildTransportPacket(priInst, FALSE);
                    result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
                    break;
                }

            default:
                {
                    /* The profile is currently not allowed to call
                     * this function */
                    result = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
                    break;
                }
        }
    }
    else
    {
        /* The profile is currently not allowed to call this function.
         * Return Accepted because this function call can
         * cross with an abnormal disconnect of the transport
         * connection */
        result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
    }
    /* make sure that the body pointer is CsrPmemFree */
    obexUtilPfreeDoublePtr((void **) body);
    return result;
}

#ifdef CSR_BT_INSTALL_OBEX_CLI_GET
CsrUint8 ObexUtilGetRequestEx(void                                *obexUtilInstData,
                            CsrUint8                            headerPriorityTable[5],
                            const CsrUint8                      *type,
                            CsrUcs2String                       **name,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                            CsrUcs2String                       **description,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                            CsrUint16                           appParametersHeaderLength,
                            CsrUint8                            **appParametersHeader,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                            CsrUint16                           userDefinedHeaderLength,
                            CsrUint8                            **userDefinedHeader,
#endif
                            CsrUint8                            srmpValue,
                            ObexUtilGetRequestFuncType          getResultHandler,
                            ObexUtilPutContinueRequestFuncType  getBodyHandler)
{
    /* Start the OBEX Get procedure */
    CsrUint8 result;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_CLIENT_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif   

    if (priInst
        && getResultHandler
        && priInst->processState == CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S)
    {
        /* The profile is allow to call this function */
        CsrBtObexResponseCode responseCode;

        ObexUtilCliInstType * cliInst       = priInst->cliInst;
        priInst->type                       = obexUtilBuildObexTypePointer(type, &priInst->typeLength);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
        priInst->userDefinedHeaderLength    = userDefinedHeaderLength;
        obexUtilSetPtrFromDoublePtr((void **)&priInst->userDefinedHeader, (void **)userDefinedHeader);
#endif        
        priInst->obexOperationIndex         = CSR_BT_OBEX_UTIL_GET_REQUEST_RESULT_HANDLER;
        cliInst->getResultFunc              = getResultHandler;
        cliInst->putContinueFunc            = getBodyHandler;
        obexUtilSetPtrFromDoublePtr((void **)&priInst->name, (void **) name);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        obexUtilSetPtrFromDoublePtr((void **)&priInst->description, (void **) description);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
        priInst->appParametersHeaderLength  = appParametersHeaderLength;
        obexUtilSetPtrFromDoublePtr((void **)&priInst->appParametersHeader, (void **) appParametersHeader); 
#endif        
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        /* Check if SRM must be enabled */
        obexUtilCliSingleResponseModeInitiator(priInst, srmpValue);
#endif

        /* Update the table which defines which OBEX headers must be
         * included in the OBEX Get Request packet */
        obexUtilUpdateHeaderTable(priInst, CSR_BT_OBEX_UTIL_GET_REQUEST_OPERATION, 5, headerPriorityTable);

        responseCode = obexUtilValidateObexPacket(priInst, TRUE, 0);

        if (obexUtilObexGetRequestHandler(priInst, responseCode, FALSE))
        {
            /* The headers can be placed in one or more OBEX Get Request packets */
            obexUtilLowpowerHandler(priInst);
        }
        else
        {
            /* One the the headers is to large, call getResultHandler
             * with the responseCode
             * CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE. Note that if the
             * function obexUtilObexGetRequestHandler returns FALSE it
             * is responsible of clearing the SRM and SRMP states */
            obexUtilDeallocatePrivateInstHeaderPointers(priInst, TRUE);
            obexUtilInitPrivateInstHeaderPointers(priInst, TRUE);
            getResultHandler(priInst->profilesOwnInstData,
                             CSR_BT_OBEX_TOO_LARGE_RESPONSE_CODE,
                             FALSE,
                             0,
                             0,
                             NULL,
                             0);
        }
        result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
    }
    else
    {
        /* The profile is currently not allowed to call this function,
         * make sure that all the pointers is CsrPmemFree to prevent a
         * memoryleak. Return Ignore because this function call can
         * cross with an abnormal disconnect of the transport
         * connection */
        result = obexUtilClientUnhandleFunctionCall(priInst);
    }
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    obexUtilPfreeDoublePtr((void **) description);
#endif
    obexUtilPfreeDoublePtr((void **) name);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    obexUtilPfreeDoublePtr((void **) userDefinedHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
    obexUtilPfreeDoublePtr((void **) appParametersHeader);
#endif
    return result;
}

CsrUint8 ObexUtilGetBodyRequest(void        *obexUtilInstData,
                                CsrBool     finalFlag,
                                CsrUint8    **body,
                                CsrUint16   bodyLength,
                                CsrUint8    srmpValue)
{
    /* This function send a GET Request packet with a body header */
    CsrUint8 result;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_CLIENT_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif   

    if (priInst)
    {
        /* The instance data used for the client part is valid */
        CsrUint8 bodyType;
        CsrUint8 obexOperation;

        if (!finalFlag)
        {
            /* This OBEX Get procedure consists of more request
             * packet, make sure that the final bit in the opcode is
             * not set (0x03), and the opcode for the body header is
             * set to Body (0x48) */
            bodyType       = CSR_BT_OBEX_UTIL_BODY_HEADER;
            obexOperation  = CSR_BT_OBEX_UTIL_GET_REQUEST_OPERATION;
        }
        else
        {
            /* This is the last OBEX Get Request message with bodies,
             * make sure that the final bit in the opcode is set
             * (0x83), and the opcode for the body header is set to
             * End of Body (0x49). Also makes sure that
             * cliInst->putContinueFunc is set NULL */
            bodyType                            = CSR_BT_OBEX_UTIL_END_BODY_HEADER;
            obexOperation                       = CSR_BT_OBEX_UTIL_GET_FINAL_REQUEST_OPERATION;
            priInst->cliInst->putContinueFunc   = NULL;
        }

        switch (priInst->processState)
        {
            /* Check if the profile is allow to call this function or not */
            case CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_PENDING_P_S:
                {
                    /* Start building and sending the OBEX GET Request
                     * packet, where only the body header is
                     * included */
                    priInst->bodyLength             = bodyLength;
                    obexUtilSetPtrFromDoublePtr((void **)&priInst->body, (void **) body);
                    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_P_S);
                    obexUtilSrmpHeaderHandler(priInst, srmpValue, TRUE, CSR_BT_OBEX_UTIL_CLIENT_ROLE, NULL);
                    obexUtilUpdateHeaderTable(priInst, obexOperation, 1, &bodyType);
                    obexUtilValidateObexPacket(priInst, FALSE, 0);
                    obexUtilBuildTransportPacket(priInst, FALSE);
                    result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
                    break;
                }
            case CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_IDLE_PENDING_P_S:
                {
                    /* This is the first OBEX Get Request packet that
                     * must be build and sent to the peer device. Add
                     * the body header to the table which defines
                     * which OBEX headers must be included in this
                     * OBEX packet. Please note that the SRMP header
                     * is ignore as the SRMP header value given in the
                     * ObexUtilGetRequest function is used */
                    priInst->headerTypeTable[0]                         = obexOperation;
                    priInst->headerTypeTable[priInst->numOfheaderTypes] = bodyType;
                    priInst->bodyLength                                 = bodyLength;
                    obexUtilSetPtrFromDoublePtr((void **)&priInst->body, (void **) body);
                    priInst->outgoingObexPacketLength                   =
                        (CsrUint16)(obexUtilBodyLengthHandler(priInst) + priInst->outgoingObexPacketLength);

                    priInst->numOfheaderTypes++;
                    priInst->lastHeaderTypeIndex++;
                    obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_P_S);
                    obexUtilBuildTransportPacket(priInst, FALSE);
                    result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
                    break;
                }
            default:
                {
                    /* The profile is currently not allowed to call
                     * this function */
                    result = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
                    break;
                }
        }
    }
    else
    {
        /* The profile is currently not allowed to call this function,
         * Return Accepted because this function call can
         * cross with an abnormal disconnect of the transport
         * connection */
        result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
    }

    /* make sure that the body pointer is CsrPmemFree */
    obexUtilPfreeDoublePtr((void **) body);
    return (result);
}

CsrUint8 ObexUtilGetContinueRequest(void * obexUtilInstData, CsrUint8 srmpValue)
{
    /* This function send a GET Request packet with the final bit set
     * and no new headers. E.g this is a multi-GET operation */
    CsrUint8 result;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_CLIENT_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif   

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    if (priInst && priInst->srmState == CSR_BT_OBEX_UTIL_SRM_ACTIVE)
    {
        /* SRM enabled. Do not send anyting */
        result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
        obexUtilRxQueueDataAckHandler(priInst);
    }
    else
#endif
    {    
        if (priInst &&
            priInst->processState == CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_PENDING_P_S)
        {
            /* Not using SRM. Send Get Request */
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_P_S);
            obexUtilSrmpHeaderHandler(priInst, srmpValue, TRUE, CSR_BT_OBEX_UTIL_CLIENT_ROLE, NULL);
            obexUtilUpdateHeaderTable(priInst, CSR_BT_OBEX_UTIL_GET_FINAL_REQUEST_OPERATION, 0, NULL);
            obexUtilValidateObexPacket(priInst, FALSE, 0);
            obexUtilBuildTransportPacket(priInst, FALSE);
            result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
        }
        else
        {
            /* Return Ignore because this function call can cross with an
             * abnormal disconnect of the transport connection */
            result = obexUtilClientUnhandleFunctionCall(priInst);
        }
    }    
    return result;
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
CsrUint8 ObexUtilCliAuthenticateResponse(void                           *obexUtilInstData,
                                         CsrUint16                      passwordLength,
                                         CsrUint8                       **password,
                                         CsrCharString                  **userId,
                                         ObexUtilDigestChallengeType    **chal)
{ /* This function adds the Authenticate Response Header to the OBEX Request
     packet. E.g the peer server has authenticated the local client by sending
     a OBEX Response packet with an Authenticate Challenge Header. To be
     authenticated, the client's request packet must contain an Authenticate
     Response Header.
     In addition this function challenge the peer server back if *chal is not
     equal to NULL */
    CsrUint8                    *passWd;
    CsrCharString               *usrId;
    ObexUtilDigestChallengeType *challenge;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_CLIENT_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif   

    obexUtilSetPtrFromDoublePtr((void **)&challenge, (void **) chal);
    obexUtilSetPtrFromDoublePtr((void **)&passWd, (void **) password);
    obexUtilSetPtrFromDoublePtr((void **)&usrId, (void **) userId);
    return (obexUtilAuthenticateResponse(priInst,
                                         passwordLength,
                                         passWd,
                                         usrId,
                                         CSR_BT_OBEX_UTIL_CLIENT_ROLE,
                                         challenge));
}
#endif

/*------------------------------------------------------------------------------*
 * Public Obex Response functions/Server functions                              *
 *------------------------------------------------------------------------------*/
CsrUint8 ObexUtilServActivateStart(void                             *obexUtilInstData,
                                   dm_security_level_t              secIncoming,
                                   uuid16_t                         profileUuid,
                                   CsrTime                          lpSuperVisionTimeout,
                                   CsrUint8                         serverChannel,
                                   psm_t                            psm,
                                   CsrUint16                        serviceRecordLength,
                                   CsrUint16                        servChServiceRecordIndex,
                                   CsrUint16                        psmServiceRecordIndex,
                                   CsrUint16                        servChServiceRecordAltIndex,
                                   CsrUint8                         **serviceRecord,
                                   CsrUint24                        classOfDevice,
                                   ObexUtilServConnectFuncType      connectIndHandler,
                                   ObexUtilServPutFuncType          putIndHandler,
                                   ObexUtilServGetFuncType          getIndHandler,
                                   ObexUtilServSetpathFuncType      setpathIndHandler,
                                   ObexUtilServAbortFuncType        abortIndHandler,
                                   ObexUtilServDisconnectFuncType   disconnectIndHandler,
                                   ObexUtilAuthenticateIndFuncType  authenticateIndHandler,
                                   ObexUtilServActivateFuncType     activateCfmHandler,
                                   ObexUtilServActionFuncType       actionIndHandler,
                                   CsrUint16                        localMaxPacketSize,
                                   CsrUint16                        windowSize,
                                   CsrBool                          localSrmEnable)
{
    /* This function will activate the OBEX server, e.g. register some
     * callback functions and register a SDP record and obtaining a
     * local server channel */
    CsrUint8 result;

    if (obexUtilInstData && connectIndHandler && abortIndHandler && disconnectIndHandler && activateCfmHandler)
    {
        /* The obexUtilInstData is present, Check if the profile is
         * allowed to called this function */
        ObexUtilInstType * inst = obexUtilInstData;

        if (!inst->privateInst)
        {
            /* The profile is allow to activate the OBEX Server */

            /* Allocates the common private instance data */
            ObexUtilPrivateInstType * priInst = (ObexUtilPrivateInstType *)
                CsrPmemAlloc(sizeof(ObexUtilPrivateInstType));

            /* Allocates the client private instance data */
            ObexUtilServInstType   * servInst = (ObexUtilServInstType *)
                CsrPmemAlloc(sizeof(ObexUtilServInstType));

            /* Initialize the common private instance data */
            obexUtilInitPrivateInstData(inst, priInst,
                                        localSrmEnable);

            /* Initialize the server private instance data */
            obexUtilInitServerInstData(servInst);
            obexUtilSetPtrFromDoublePtr((void **)&servInst->serviceRecord, (void **) serviceRecord);
            priInst->cmState                        = CSR_BT_OBEX_UTIL_BT_ACTIVATED_S;
            priInst->lpSuperVisionTimeout           = lpSuperVisionTimeout;
            priInst->localServerCh                  = serverChannel;
            priInst->localPsm                       = psm;
            priInst->servInst                       = servInst;
            priInst->maxLocalObexPacketLength       = localMaxPacketSize;
            
            servInst->obexConnected                 = FALSE;
            servInst->profileUuid                   = profileUuid;
            servInst->classOfDevice                 = classOfDevice;
            servInst->secIncoming                   = secIncoming;
            servInst->serverChServiceRecordIndex    = servChServiceRecordIndex;
            servInst->servChServiceRecordAltIndex   = servChServiceRecordAltIndex;
            servInst->serviceRecordLength           = serviceRecordLength;
            servInst->connectIndHandler             = connectIndHandler;
            servInst->putIndHandler                 = putIndHandler;
#ifdef CSR_BT_INSTALL_OBEX_SRV_GET
            servInst->getIndHandler                 = getIndHandler;
#else
            CSR_UNUSED(setpathIndHandler);
#endif
#ifdef CSR_BT_INSTALL_OBEX_SRV_SET_PATH
            servInst->setpathIndHandler             = setpathIndHandler;
#else
            CSR_UNUSED(setpathIndHandler);
#endif
            servInst->abortIndHandler               = abortIndHandler;
            servInst->disconnectIndHandler          = disconnectIndHandler;
            servInst->authenticateIndHandler        = authenticateIndHandler;
            servInst->activateCfmHandler            = activateCfmHandler;
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            servInst->psmServiceRecordIndex         = psmServiceRecordIndex;
            servInst->actionIndHandler              = actionIndHandler;
            priInst->windowSize                     = (CsrUint16)(windowSize != 0 ? windowSize : CSR_BT_OBEX_UTIL_TX_WINDOW);
            priInst->rxQueueCount                   = priInst->windowSize;
#else
            CSR_UNUSED(psmServiceRecordIndex);
            CSR_UNUSED(actionIndHandler);
            CSR_UNUSED(windowSize);
#endif
            inst->privateInst                       = priInst;

            if (priInst->maxLocalObexPacketLength > CSR_BT_MAX_OBEX_SIGNAL_LENGTH)
            {
                priInst->maxLocalObexPacketLength = CSR_BT_MAX_OBEX_SIGNAL_LENGTH;
            }

            /* This function checks if a local server channel and or a local
               psm must be obtained or it can register the SDP record direct */
            obexUtilRegisterBtMultiplexerHandle(priInst);
            result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
        }
        else
        { /* This local server is allready activated. */
            result = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
        }
    }
    else
    { /* No obexUtilInstData or callbacks is present */
        result = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
    }
    /* make sure that the serviceRecord pointer is CsrPmemFree */
    obexUtilPfreeDoublePtr((void **) serviceRecord);
    return (result);
}

CsrUint8 ObexUtilServConnectableStart(void      *obexUtilInstData,
                                      CsrUint16 rfcommMtu,
                                      CsrUint16 l2caMtu)
{ /* This function will set the OBEX server connectable, e.g the library will
     make sure that the server always are connectable until it received a
     ObexUtilServNonConnectableStart. */
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_SERVER_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif

    if (priInst && priInst->processState == CSR_BT_OBEX_UTIL_BT_ACTIVATED_P_S)
    { /* The profile is allow to call this function, set the local device
         connectable */
        priInst->servInst->rfcommMtu = rfcommMtu;
        obexUtilSetDeviceConnectable(priInst);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* No obexUtilInstData or the profile is not allowed to call this function*/
        return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
    }
}

#ifdef CSR_BT_INSTALL_OBEX_UTIL_SERV_NON_CONNECTABLE
CsrUint8 ObexUtilServNonConnectableStart(void                           *obexUtilInstData,
                                         ObexUtilServDeactivateFuncType nonConnectableCfmHandler)
{ /* This function will set the OBEX server non-connectable. In the case that
     a transport connection is present it will be release */
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_SERVER_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif
    if (priInst && nonConnectableCfmHandler && priInst->cmState != CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S)
    { /* Start the non-connectable procedure */
        ObexUtilServInstType    *servInst   = priInst->servInst;
        servInst->nonConnectableCfmHandler  = nonConnectableCfmHandler;
        return (obexUtilDeactivateHandler(priInst, servInst, CSR_BT_OBEX_UTIL_BT_NON_CONNECTABLE_P_S));
    }
    else
    { /* No obexUtilInstData or no callback function or the non-connectable
         procedure is already running or the deactivating procedure is running.
         E.g if the cmState is CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S it either
         means that the non-connectable procedure is running or the deactivating
         procedure is running */
        return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
    }
}
#endif

CsrUint8 ObexUtilServDeactivateStart(void                           *obexUtilInstData,
                                     CsrUint8                  serverChannel,
                                     psm_t                          psm,
                                     ObexUtilServDeactivateFuncType deactivateCfmHandler)
{ /* This function will deactivate the OBEX server, e.g. unregister some
     callback functions, unregister the service record and releaseing the
     obtained local server channel (if serverChannel == CSR_BT_NO_SERVER).
     Note if the local server is connectable it will be set non-connectable
     and if a transport connection is present it will be release */
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_SERVER_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif

    if (priInst && deactivateCfmHandler)
    { /* Note that it ok that the cmState is CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S,
         because the non-connectable can be cancel by this procedure */
 
        ObexUtilServInstType    *servInst   = priInst->servInst;
        servInst->deactivateCfmHandler      = deactivateCfmHandler;

        if (serverChannel == CSR_BT_NO_SERVER)
        { /* The profile don't want to reuse the local server channel later,
             set the processState to CSR_BT_OBEX_UTIL_BT_DEACTIVATING_P_S to
             indicate that the server channel must be relase doing the
             deactivating procedure */
            return (obexUtilDeactivateHandler(priInst, servInst, CSR_BT_OBEX_UTIL_BT_DEACTIVATING_P_S));
        }
        else
        { /* The profile want to reuse the local server channel later, set the
             processState to CSR_BT_OBEX_UTIL_BT_LIMITED_DEACTIVATING_P_S to
             indicate that the server channel must NOT be relase doing the
             deactivating procedure */
            return (obexUtilDeactivateHandler(priInst, servInst, CSR_BT_OBEX_UTIL_BT_LIMITED_DEACTIVATING_P_S));
        }
    }
    else
    { /* No obexUtilInstData or no callback function */
        return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
    }
}

CsrUint8 ObexUtilServConnectResponseEx(void                           *obexUtilInstData,
                                     CsrBtObexResponseCode          responseCode,
#ifdef CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID
                                     CsrUint16                      whoHeaderLength,
                                     CsrUint8                       **who,
#endif
                                     CsrUint32                      connectionId,
                                     CsrUint8                       numOfHeaders,
                                     const CsrUint8                 *headerPriorityTable
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                     ,ObexUtilDigestChallengeType    **digestChallenge
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                     ,CsrUcs2String                  **description
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                     ,CsrUint16                      userDefinedHeaderLength
                                     ,CsrUint8                       **userDefinedHeader
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                     ,CsrBool                        directAuthenticateResponse
#endif
                                     )
{
    /* This function will send an OBEX CONNECT response packet */
    CsrUint8 result;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_SERVER_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif

    if (priInst && priInst->processState == CSR_BT_OBEX_UTIL_OBEX_CONNECT_RESPONSE_PENDING_P_S)
    {
        /* The server has either accepted, rejected or challenge the
         * incoming OBEX connection */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
        ObexUtilDigestChallengeType *chal;
#endif
#ifdef CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID
        priInst->targetWhoHeaderLength      = whoHeaderLength;
        obexUtilSetPtrFromDoublePtr((void **)&priInst->targetWho, (void **) who);
        priInst->connectionId               = connectionId;
#endif        
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
        priInst->userDefinedHeaderLength    = userDefinedHeaderLength;
        obexUtilSetPtrFromDoublePtr((void **)&priInst->userDefinedHeader, (void **)userDefinedHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        obexUtilSetPtrFromDoublePtr((void **)&priInst->description, (void **) description);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
        obexUtilSetPtrFromDoublePtr((void **)&chal, (void **) digestChallenge);
        obexUtilServValidateSingleResponseHandler(priInst,
                                                  chal,
                                                  responseCode,
                                                  directAuthenticateResponse);
#else
        obexUtilServValidateSingleResponseHandler(priInst,
                                                  NULL,
                                                  responseCode,
                                                  FALSE);
#endif        
        obexUtilServResponsePacketHandler(priInst,
                                          numOfHeaders,
                                          headerPriorityTable,
                                          CSR_BT_OBEX_UTIL_CONNECT_RESPONSE_OPERATION);

        result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
    }
    else
    { /* Either is the transport connection were released while waiting for
         this response or the local server is currently not allowed to call
         this function. */
        if (priInst && priInst->processState == CSR_BT_OBEX_UTIL_BT_PENDING_CONNECTABLE_P_S)
        { /* The transport connection were released while waiting for this
             response */
            if ((responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
            { /* The server has accepted the incoming OBEX request. Called the
                 disconnect callback function to indicate that the OBEX
                 connection is closed */
                priInst->servInst->disconnectIndHandler(priInst->profilesOwnInstData,
                                                        priInst->deviceAddr,
                                                        (CsrBtReasonCode) priInst->resultCode,
                                                        priInst->resultSupplier,
                                                        0,
                                                        NULL);
            }
            else
            { /* The server has either rejected the incoming OBEX connection or
                 it wants to challenge the client first. In both cases the OBEX
                 connection is not connected. Just ignore this request */
                ;
            }
            priInst->resultCode     = CSR_BT_RESULT_CODE_CM_SUCCESS;
            priInst->resultSupplier = CSR_BT_SUPPLIER_CM;
            obexUtilSetProcessState(priInst, CSR_BT_OBEX_UTIL_BT_ACTIVATED_P_S);
            obexUtilSetDeviceConnectable(priInst);
            result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
        }
        else
        { /* The profile is currently not allowed to call this function */
            result = (obexUtilServUnhandleFunctionCall(priInst));
        }
    }

    /* Make sure that all the pointers is CsrPmemFree to prevent a memoryleak */
#ifdef CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID
    obexUtilPfreeDoublePtr((void **) who);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    obexUtilPfreeDoublePtr((void **) description);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    obexUtilPfreeDoublePtr((void **) userDefinedHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    ObexUtilPfreeDigestChallengePointer(digestChallenge);
#endif
    return (result); 
}

#ifdef CSR_BT_INSTALL_OBEX_SRV_SET_PATH
CsrUint8 ObexUtilServSetpathResponseEx(void                        *obexUtilInstData,
                                       CsrBtObexResponseCode       responseCode
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                       , ObexUtilDigestChallengeType **digestChallenge
#endif
                                       , CsrUint8                    headerPriorityTable[3]
                                       , CsrUcs2String               **name
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                       , CsrUcs2String               **description
#endif                                     
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                       , CsrUint16                   userDefinedHeaderLength
                                       , CsrUint8                    **userDefinedHeader
#endif
                                     )
{ /* This function will send an OBEX SETPATH response packet */
    CsrUint8 result = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_SERVER_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif

    if (priInst && priInst->processState == CSR_BT_OBEX_UTIL_OBEX_SETPATH_RESPONSE_PENDING_P_S)
    { /* The server has either accepted, rejected or challenge the OBEX Setpath
         Request */
        ObexUtilDigestChallengeType *chal=NULL;
        obexUtilSetPtrFromDoublePtr((void **)&priInst->name, (void **) name);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        obexUtilSetPtrFromDoublePtr((void **)&priInst->description, (void **) description);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
        priInst->userDefinedHeaderLength    = userDefinedHeaderLength;
        obexUtilSetPtrFromDoublePtr((void **)&priInst->userDefinedHeader, (void **)userDefinedHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
        obexUtilSetPtrFromDoublePtr((void **)&chal, (void **) digestChallenge);    
#endif
        obexUtilServValidateSingleResponseHandler(priInst, chal, responseCode, FALSE);
        obexUtilServResponsePacketHandler(priInst, 3, headerPriorityTable,
                                          CSR_BT_OBEX_UTIL_COMMON_RESPONSE_OPERATION);

    }
    else
    { /* The profile is currently not allowed to call this function, make sure
         that the pointers is CsrPmemFree to prevent a memoryleak. */
        result =  obexUtilServUnhandleFunctionCall(priInst);
    }

obexUtilPfreeDoublePtr((void **) name);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    obexUtilPfreeDoublePtr((void **) description);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    obexUtilPfreeDoublePtr((void **) userDefinedHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    ObexUtilPfreeDigestChallengePointer(digestChallenge);
#endif
    return result;
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_UTIL_SERV_PUT_GET_WITH_HEADER_EXT
CsrUint8 ObexUtilServPutWithHeaderResponseEx(void                        *obexUtilInstData,
                                           CsrBtObexResponseCode       responseCode,
                                           ObexUtilDigestChallengeType **digestChallenge,
                                           CsrUint8                    headerPriorityTable[4],
                                           CsrUcs2String               **name,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                           CsrUcs2String               **description,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                                           CsrUint16                   appParametersHeaderLength,
                                           CsrUint8                    **appParametersHeader,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                           CsrUint16                   userDefinedHeaderLength,
                                           CsrUint8                    **userDefinedHeader,
#endif
                                           CsrUint8                    srmpValue)
{ /* This function will send a OBEX PUT response packet. This function allows
     the local server to include some additionals headers to this packet */
    ObexUtilDigestChallengeType *chal;
    CsrUcs2String               *nameHeader;
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    CsrUcs2String               *descrip;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
    CsrUint8                    *appHeader;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    CsrUint8                    *userHeader;
    obexUtilSetPtrFromDoublePtr((void **)&userHeader, (void **) userDefinedHeader);
#endif

    obexUtilSetPtrFromDoublePtr((void **)&chal, (void **) digestChallenge);
    obexUtilSetPtrFromDoublePtr((void **)&nameHeader, (void **) name);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    obexUtilSetPtrFromDoublePtr((void **)&descrip, (void **) description);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
    obexUtilSetPtrFromDoublePtr((void **)&appHeader, (void **) appParametersHeader);
#endif
    return (obexUtilServPutResponsePacketHandler(obexUtilInstData,
                                                 responseCode,
                                                 chal,
                                                 4,
                                                 headerPriorityTable,
                                                 nameHeader,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                                 descrip,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                                                 appParametersHeaderLength,
                                                 appHeader,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                                 userDefinedHeaderLength,
                                                 userHeader,
#endif
                                                 srmpValue));
}
#endif

CsrUint8 ObexUtilServPutResponse(void                  *obexUtilInstData,
                                 CsrBtObexResponseCode responseCode,
                                 CsrUint8              srmpValue)
{ /* This function will send a OBEX PUT response packet. With this function the
     local server cannot add any additionals headers */
    return (obexUtilServPutResponsePacketHandler(obexUtilInstData,
                                                 responseCode,
                                                 NULL,
                                                 0,
                                                 NULL,
                                                 NULL,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                                 NULL,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                                                 0,
                                                 NULL,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                                 0,
                                                 NULL,
#endif                                                 
                                                 srmpValue));
}

CsrUint8 ObexUtilServSetPutContinueFunc(void                        *obexUtilInstData,
                                        ObexUtilServPutFuncType     putContinueIndHandler)
{ /* This function will register a temporary callback function that must be
     called when the next OBEX PUT Request packet is received. */
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_SERVER_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif

    if (priInst &&
        (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_PUT_RESPONSE_PENDING_P_S ||
         priInst->processState == CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_PENDING_P_S))
    { /* The local server is allow to set a temporary callback function */
        priInst->servInst->putContinueIndHandler = putContinueIndHandler;
    }
    else
    { /* Return Accepted because this function call can cross with an abnormal
         disconnect of the transport connection, or an OBEX Abort Request */
        ;
    }
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

#ifdef CSR_BT_INSTALL_OBEX_SRV_GET
#ifdef CSR_BT_INSTALL_OBEX_UTIL_SERV_PUT_GET_WITH_HEADER_EXT
CsrUint8 ObexUtilServGetWithHeaderResponseEx(void                        *obexUtilInstData,
                                           CsrBtObexResponseCode       responseCode,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                           ObexUtilDigestChallengeType **digestChallenge,
#endif
                                           CsrUint8                    headerPriorityTable[6],
                                           CsrUint8                    bodyHeaderPriorityIndex,
                                           CsrUint32                   lengthOfObject,
                                           CsrUcs2String               **name,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                           CsrUcs2String               **description,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                                           CsrUint16                   appParametersHeaderLength,
                                           CsrUint8                    **appParametersHeader,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                           CsrUint16                   userDefinedHeaderLength,
                                           CsrUint8                    **userDefinedHeader,
#endif
                                           CsrUint8                    **body,
                                           CsrUint16                   bodyLength,
                                           CsrUint8                    srmpValue)
{
    /* This function will send a OBEX GET response packet. This
     * function allows the local server to include some additionals
     * headers to this packet */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    ObexUtilDigestChallengeType *chal;
#endif
    CsrUcs2String               *nameHeader;
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    CsrUcs2String               *descrip;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
    CsrUint8                    *appHeader;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    CsrUint8                    *userHeader;
#endif
    CsrUint8                    *bodyPointer; 

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    obexUtilSetPtrFromDoublePtr((void **)&chal, (void **) digestChallenge);
#endif
    obexUtilSetPtrFromDoublePtr((void **)&nameHeader, (void **) name);
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    obexUtilSetPtrFromDoublePtr((void **)&descrip, (void **) description);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
    obexUtilSetPtrFromDoublePtr((void **)&appHeader, (void **) appParametersHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    obexUtilSetPtrFromDoublePtr((void **)&userHeader, (void **) userDefinedHeader);
#endif
    obexUtilSetPtrFromDoublePtr((void **)&bodyPointer, (void **) body);

    return (obexUtilServGetResponsePacketHandler(obexUtilInstData,
                                                 responseCode,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                                 chal,
#else
                                                 NULL,
#endif
                                                 6,
                                                 headerPriorityTable,
                                                 bodyHeaderPriorityIndex,
                                                 lengthOfObject,
                                                 nameHeader,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                                 descrip,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                                                 appParametersHeaderLength,
                                                 appHeader,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                                 userDefinedHeaderLength,
                                                 userHeader,
#endif
                                                 bodyPointer,
                                                 bodyLength,
                                                 srmpValue));
}
#endif

CsrUint8 ObexUtilServGetResponse(void                  *obexUtilInstData,
                                 CsrBtObexResponseCode responseCode,
                                 CsrUint8              **body,
                                 CsrUint16             bodyLength,
                                 CsrUint8              srmpValue)
{
    /* This function will send a OBEX GET response packet. With this
     * function the local server cannot add any additionals headers
     * except a body header which is always sent */
    CsrUint8 bodyType = CSR_BT_OBEX_UTIL_BODY_HEADER;
    CsrUint8 *bodyPointer; 
    obexUtilSetPtrFromDoublePtr((void **)&bodyPointer, (void **) body);
    return (obexUtilServGetResponsePacketHandler(obexUtilInstData,
                                                 responseCode,
                                                 NULL,
                                                 1,
                                                 &bodyType,
                                                 0,
                                                 0,
                                                 NULL,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                                 NULL,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
                                                 0,
                                                 NULL,
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                                 0,
                                                 NULL,
#endif
                                                 bodyPointer,
                                                 bodyLength,
                                                 srmpValue));
}


CsrUint8 ObexUtilServSetGetContinueFunc(void                        *obexUtilInstData,
                                        ObexUtilServGetFuncType     getContinueIndHandler)
{
    /* This function will register a temporary callback function that
     * must be called when the next OBEX GET Request packet is
     * received. */
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_SERVER_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif

    if (priInst &&
        (priInst->processState == CSR_BT_OBEX_UTIL_OBEX_GET_RESPONSE_PENDING_P_S ||
         priInst->processState == CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_PENDING_P_S))
    { /* The local server is allow to set a temporary callback function */
        priInst->servInst->getContinueIndHandler = getContinueIndHandler;
    }
    else
    { /* Return Accepted because this function call can cross with an abnormal
         disconnect of the transport connection, or an OBEX Abort Request */
        ;
    }
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
CsrUint8 ObexUtilServAuthenticateResponse(void       *obexUtilInstData,
                                          CsrUint16  passwordLength,
                                          CsrUint8   **password,
                                          CsrCharString  **userId)
{
    /* This function adds the Authenticate Response Header to the OBEX
     * response packet. E.g the peer client has authenticated the
     * local server by sending a OBEX Request packet with an
     * Authenticate Challenge Header. To be authenticated, the
     * server's response packet must contain the
     * CSR_BT_OBEX_SUCCESS_RESPONSE_CODE response code and an
     * Authenticate Response Header. */
    CsrUint8                *passWd;
    CsrCharString           *usrId;
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_SERVER_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif
    
    obexUtilSetPtrFromDoublePtr((void **)&passWd, (void **) password);
    obexUtilSetPtrFromDoublePtr((void **)&usrId, (void **) userId);

    return (obexUtilAuthenticateResponse(priInst,
                                         passwordLength,
                                         passWd,
                                         usrId,
                                         CSR_BT_OBEX_UTIL_SERVER_ROLE,
                                         NULL));
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
CsrUint8 ObexUtilServActionResponseEx(void                        *obexUtilInstData,
                                    CsrBtObexResponseCode       responseCode,
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
                                    ObexUtilDigestChallengeType **digestChallenge,
#endif
                                    CsrUint8                    headerPriorityTable[2]
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
                                    ,CsrUcs2String               **description
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
                                    ,CsrUint16                   userDefinedHeaderLength
                                    ,CsrUint8                    **userDefinedHeader
#endif
                                    )
{
    /* This function will send an OBEX SETPATH response packet */
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_SERVER_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif

    if (priInst && priInst->processState == CSR_BT_OBEX_UTIL_OBEX_ACTION_RESPONSE_PENDING_P_S)
    {
        /* The server has either accepted, rejected or challenge the
         * OBEX Action Request */
        ObexUtilDigestChallengeType *chal = NULL;
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
        priInst->userDefinedHeaderLength    = userDefinedHeaderLength;
        obexUtilSetPtrFromDoublePtr((void **)&priInst->userDefinedHeader, (void **)userDefinedHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        obexUtilSetPtrFromDoublePtr((void **)&priInst->description, (void **) description);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
        obexUtilSetPtrFromDoublePtr((void **)&chal, (void **) digestChallenge);    
#endif
        obexUtilServValidateSingleResponseHandler(priInst, chal, responseCode, FALSE);
        obexUtilServResponsePacketHandler(priInst, 2, headerPriorityTable,
                                          CSR_BT_OBEX_UTIL_COMMON_RESPONSE_OPERATION);

        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    {
        /* The profile is currently not allowed to call this function,
         * make sure that the pointers is CsrPmemFree to prevent a
         * memoryleak. */
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        obexUtilPfreeDoublePtr((void **) description);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
        obexUtilPfreeDoublePtr((void **) userDefinedHeader);
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
        ObexUtilPfreeDigestChallengePointer(digestChallenge);
#endif
        return (obexUtilServUnhandleFunctionCall(priInst));
    }
}
#endif

void ObexUtilServChangeIncomingSecurity(void                 *obexUtilInstData,
                                        dm_security_level_t  secIncoming)
{
#ifdef CSR_BT_INSTALL_OBEX_ERROR_CHECK
    ObexUtilPrivateInstType *priInst = obexUtilVerifyPrivateInstData(obexUtilInstData, 
                                                                     CSR_BT_OBEX_UTIL_SERVER_ROLE);
#else
    ObexUtilInstType * inst = obexUtilInstData;
    ObexUtilPrivateInstType *priInst = inst->privateInst;
#endif
    if (priInst)
    {
        priInst->servInst->secIncoming = secIncoming;
    }
}

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
ObexUtilDigestChallengeType * ObexUtilReturnDigestChallengePointer(CsrBool       authtorize,
                                                                   CsrUint8      **password,
                                                                   CsrUint16     passwordLength,
                                                                   CsrCharString **userId,
                                                                   CsrUint8      **realm,
                                                                   CsrUint16     realmLength)
{
    if (authtorize)
    {
        ObexUtilDigestChallengeType * digestChallenge;
        digestChallenge                   = (ObexUtilDigestChallengeType *) CsrPmemAlloc(sizeof(ObexUtilDigestChallengeType));
        digestChallenge->options          = (CsrUint8) (*userId ? 1 : 0);        
        digestChallenge->realmLength      = realmLength;
        digestChallenge->passwordLength   = passwordLength;
        obexUtilSetPtrFromDoublePtr((void **)&digestChallenge->userId, (void **) userId);
        obexUtilSetPtrFromDoublePtr((void **)&digestChallenge->realm, (void **) realm);
        obexUtilSetPtrFromDoublePtr((void **)&digestChallenge->password, (void **) password);    
        return (digestChallenge);
    }
    else
    {
        obexUtilPfreeDoublePtr((void **) password);
        obexUtilPfreeDoublePtr((void **) userId);
        obexUtilPfreeDoublePtr((void **) realm);
        return (NULL);
    }
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
void ObexUtilPfreeDigestChallengePointer(ObexUtilDigestChallengeType ** digestChallenge)
{
    if (digestChallenge)
    {
        ObexUtilDigestChallengeType *chal = *digestChallenge;
        if (chal)
        {
            CsrPmemFree(chal->userId);
            CsrPmemFree(chal->realm);
            CsrPmemFree(chal->password);
            obexUtilPfreeDoublePtr((void **) digestChallenge);
        }
    }
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_CONTEXT_ID
CsrBool ObexUtilGetInstIdentifierFromCmUpstreamMessage(void * msg, CsrUint8 *id)
{
    CsrPrim *type = msg;
    *id           = 0;

    if (*type <= CSR_BT_CM_RFC_PRIM_UPSTREAM_HIGHEST)
    { /* This CM upsteam messages comes from RFCOMM */
        CsrUint16 index = (CsrUint16)(*type - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST);

        if (obexUtilCmRfcUpstreamMsgContextHandler[index] != NULL)
        {
            *id = obexUtilCmRfcUpstreamMsgContextHandler[index](msg);
            return (TRUE);
        }
    }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    else if (*type >= CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST &&
             *type <= CSR_BT_CM_L2CA_PRIM_UPSTREAM_HIGHEST)
    {
        CsrUint16 index = (CsrUint16)(*type - CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST);
        if (obexUtilCmL2caUpstreamMsgContextHandler[index] != NULL)
        {
            *id = obexUtilCmL2caUpstreamMsgContextHandler[index](msg);
            return (TRUE);
        }
    }
#endif
    else if (*type >= CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST &&
             *type <= CSR_BT_CM_SDC_PRIM_EXT_UPSTREAM_HIGHEST)
    { /* This CM upsteam messages comes from either SDC or SDS */
        CsrUint16 index = (CsrUint16)(*type - CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST);
        if (obexUtilCmSdpUpstreamMsgContextHandler[index] != NULL)
        {
            *id = obexUtilCmSdpUpstreamMsgContextHandler[index](msg);
            return (TRUE);
        }
    }
    else if (*type == CSR_BT_CM_DATA_BUFFER_EMPTY_CFM)
    {
        CsrBtCmDataBufferEmptyCfm *pMsg = (CsrBtCmDataBufferEmptyCfm *) msg;
        *id = (CsrUint8) pMsg->context;
        return (TRUE);
    }
    return (FALSE);
}
#endif

#endif /* EXCLUDE_CSR_BT_RFC_MODULE */


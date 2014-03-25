/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_bt_result.h"
#include "csr_bt_pac_handler.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_pb_obex.h"

#define CSR_BT_OBEX_PB_HEADER_APP_FILTER_LENGTH        (10)
#define CSR_BT_OBEX_PB_HEADER_APP_MAXLISTCNT_LENGTH    (0x04)
#define CSR_BT_OBEX_PB_HEADER_APP_SEARCHVAL_LENGTH     (0x02)
#define CSR_BT_OBEX_PB_HEADER_APP_ORDER_LENGTH         (0x03)
#define CSR_BT_OBEX_PB_HEADER_APP_LSTSTOFFSET_LENGTH   (0X04)

#define CSR_BT_PAC_TARGET_UUID {0x79, 0x61, 0x35, 0xF0, 0xF0, 0xC5, 0x11, 0xD8, 0x09, 0x66, 0x08, 0x00, 0x20, 0x0C, 0x9A, 0x66}
#define CSR_BT_PAC_TARGET_LENGTH 0x10

#define CSR_BT_PAC_OBEX_APP_PAR_LIST_FLAG  (PbAppParFlag)(CSR_BT_PB_OBEX_ORDER_FLAG | CSR_BT_PB_OBEX_SEARCHATT_FLAG \
                                                          | CSR_BT_PB_OBEX_MAX_LIST_CNT_FLAG | CSR_BT_PB_OBEX_LISTSTARTOFFSET_FLAG)

static const CsrUint8 serviceRecord[] =
{
    /* Service class ID list */
    0x09, 0x00, 0x01,   /* AttrID , ServiceClassIDList */
    0x35, 0x03,         /* 3 bytes in total DataElSeq */
    0x19, 0x11, 0x2E,   /* 2 byte UUID, Phonebook Access Uuid16 0x112E PBAP Client Class */

    /* BrowseGroupList */
    0x09, 0x00, 0x05,   /* AttrId = BrowseGroupList */
    0x35, 0x03,         /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,   /* 2 byte UUID, PublicBrowseGroup = 0x1002 */

    /* bluetooth profile descriptor list */
    0x09, 0x00, 0x09,   /* AttrId ProtocolDescriptorList */
    0x35, 0x08,         /* 8 bytes in total DataElSeq */

    /* Supported profile */
    0x35, 0x06,         /* 6 bytes in DataElSeq */
    0x19, 0x11, 0x30,   /* 2 byte UUID, Supported profile 0x1130 Phonebook Access */

    /* Profile version */
    0x09, 0x01, 0x00,   /* 2 byte UINT - Profile version = 0x0100 = version 1.0 */

    /* service name */
    0x09, 0x01, 0x00,   /* AttrId - Service Name. Use language base attribute 0x0100 (primary language) */
    0x25, 20,           /* length of service string */
    'P', 'h', 'o', 'n', 'e', 'b', 'o', 'o', 'k', ' ', 'A', 'c', 'c', 'e', 's', 's', ' ', 'P', 'C', 'E', /* string - "Phonebook Access PCE" */
};

static const CsrUint8 SZ_PHONEBOOK_TYPE[]     = {"x-bt/phonebook"};
static const CsrUint8 SZ_VCARD_LISTING_TYPE[] = {"x-bt/vcard-listing"};
static const CsrUint8 SZ_VCARD_TYPE[]         = {"x-bt/vcard"};
static const CsrUint8 wTelecom[]              = {0, 't', 0, 'e', 0, 'l', 0, 'e', 0, 'c', 0, 'o', 0, 'm', 0, '/', 0, 0};
static const CsrUint8 wSim1[]                 = {0, 'S', 0, 'I', 0, 'M', 0, '1', 0, '/', 0, 0};

static CsrUint16 csrBtPacExtractSupportedRepositories(CmnCsrBtLinkedListStruct * sdpTag)
{
    CsrUint16 supportedRepositories = CSR_BT_PAC_SRC_NONE;

    if (sdpTag)
    {
        CsrBtUuid32    service;
        CsrUint16    bchsResult, bchsTask, bchsTaskResult;

        if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTag, 0, &service,
                                                  &bchsResult, &bchsTask, &bchsTaskResult))
        {
            if (bchsResult == SDR_SDC_SEARCH_SUCCESS)
            {
                CsrUint32  data;

                if (CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(sdpTag, 0,
                                                                           CSR_BT_SUPPORTED_REPOSITORIES_ATTRIBUTE_IDENTIFIER, &data))
                {
                    supportedRepositories = (CsrUint16)data;
                }
            }
        }
    }
    return (supportedRepositories);
}


static void PbBuildSearchAttHeader(CsrUint8 **ppBuf, CsrUint8 *pAppTotalLen, CsrBtPacSearchAtt searchAtt)
{
    CsrUint8 *pBuf = *ppBuf;

    *pBuf = CSR_BT_OBEX_PB_SEARCH_ATT_ID;
    pBuf++;
    *pBuf = 1;
    pBuf++;
    *pBuf = searchAtt;
    pBuf++;
    (*pAppTotalLen) += CSR_BT_OBEX_PB_HEADER_APP_ORDER_LENGTH; /* update total length */
    *ppBuf = pBuf;
}

static CsrUint16 pbRetAppParaSearchValLen(CsrUint8 *szVal)
{
    return (CsrUint16)(2 + (CsrStrLen((char *)szVal) + 1));
}

static void pbBuildOrderHeader(CsrUint8 **ppBuf, CsrUint8 *pAppTotalLen, CsrUint8 order)
{
    CsrUint8 *pBuf = *ppBuf;

    *pBuf = CSR_BT_OBEX_PB_ORDER_ID;
    pBuf++;
    *pBuf = 1;
    pBuf++;
    *pBuf = order;
    pBuf++;
    (*pAppTotalLen) += CSR_BT_OBEX_PB_HEADER_APP_ORDER_LENGTH; /* update total length */
    *ppBuf = pBuf;
}

static void pbBuildSearchValHeader(CsrUint8 **ppBuf, CsrUint8 *pAppTotalLen, CsrUint8 *pSearch)
{
    CsrUint8 *pBuf = *ppBuf;

    *pBuf = CSR_BT_OBEX_PB_SEARCH_VAL_ID;
    pBuf++;
    *pBuf = (CsrUint8)(CsrStrLen((char *)pSearch) + 1);
    pBuf++;
    CsrMemCpy(pBuf, pSearch, *(pBuf-1));
    (*pAppTotalLen) += CSR_BT_OBEX_PB_HEADER_APP_SEARCHVAL_LENGTH + *(pBuf-1); /* update total length */
    pBuf += *(pBuf-1);
    *ppBuf = pBuf;
}

static void pbBuildMaxListCntHeader(CsrUint8 **ppBuf, CsrUint8 *pAppTotalLen, CsrUint16 nMaxListCnt)
{
    CsrUint8 *pBuf = *ppBuf;

    *pBuf = CSR_BT_OBEX_PB_MAX_LIST_CNT_ID; /* tag */
    pBuf++;
    *pBuf = 0x02; /* length */
    pBuf++;
    pBuf[0] = (CsrUint8)(nMaxListCnt >> 8); /* value */
    pBuf[1] = (CsrUint8)(nMaxListCnt);
    pBuf += 2;
    (*pAppTotalLen) += CSR_BT_OBEX_PB_HEADER_APP_MAXLISTCNT_LENGTH; /* update total length */
    *ppBuf = pBuf;
}

static void pbBuildListOffsetHeader(CsrUint8 **ppBuf, CsrUint8 *pAppTotalLen, CsrUint16 nOffset)
{
    CsrUint8 *pBuf = *ppBuf;

    *pBuf = CSR_BT_OBEX_PB_LIST_START_OFFSET_ID; /* tag */
    pBuf++;
    *pBuf = 0x02; /* length */
    pBuf++;
    pBuf[0] = (CsrUint8)(nOffset >> 8); /* value */
    pBuf[1] = (CsrUint8)(nOffset);
    pBuf += 2;
    (*pAppTotalLen) += CSR_BT_OBEX_PB_HEADER_APP_LSTSTOFFSET_LENGTH; /* update total length */
    *ppBuf = pBuf;
}

static void pbBuildFilterHeader(CsrUint8 **ppBuf, CsrUint8 *pAppTotalLen, CsrUint8 *filter)
{
    CsrUint8 *pBuf = *ppBuf;

    *pBuf = CSR_BT_OBEX_PB_FILTER_ID; /* tag */
    pBuf++;
    *pBuf = 0x08; /* length */
    pBuf++;
    pBuf[0] = filter[7];
    pBuf[1] = filter[6];
    pBuf[2] = filter[5];
    pBuf[3] = filter[4];
    pBuf[4] = filter[3];
    pBuf[5] = filter[2];
    pBuf[6] = filter[1];
    pBuf[7] = filter[0];
    pBuf += 8;
    (*pAppTotalLen) += CSR_BT_OBEX_PB_HEADER_APP_FILTER_LENGTH; /* update total length */
    *ppBuf = pBuf;
}

static void pbBuildFormatHeader(CsrUint8 **ppBuf, CsrUint8 *pAppTotalLen, CsrUint8 format)
{
    CsrUint8 *pBuf = *ppBuf;

    *pBuf = CSR_BT_OBEX_PB_FORMAT_ID; /* tag */
    pBuf++;
    *pBuf = 0x01; /* length */
    pBuf++;
    *pBuf = format; /* value */
    pBuf++;
    (*pAppTotalLen) += CSR_BT_OBEX_APP_PAR_FORMAT_SIZE; /* update total length */
    *ppBuf = pBuf;
}

static CsrBool pbGetObexPhonebookSize(CsrBtObexSession session, CsrUint8 *pObexPacket, CsrUint16 *pnPbSize)
{
    CsrUint16 nLen;
    CsrUint16 nIndex = CsrBtObexHeaderIndex(session, pObexPacket, CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER, &nLen);

    if (nIndex && nLen > CSR_BT_OBEX_HEADER_LENGTH)
    {
        nLen += nIndex - 1;
        nIndex += CSR_BT_OBEX_HEADER_LENGTH;
        while (nIndex < nLen)
        {
            if (pObexPacket[nIndex] == CSR_BT_OBEX_PB_PHONEBOOK_SIZE_ID)
            {
                *pnPbSize = CSR_BT_OBEX_GET_WORD_AT(pObexPacket, (nIndex + CSR_BT_AUTH_TAG_LENGTH_SCHEME));
                return TRUE;
            }
            nIndex += pObexPacket[nIndex + 1] + CSR_BT_AUTH_TAG_LENGTH_SCHEME;
        }
    }
    return FALSE;
}

static CsrBool pbGetObexMissedCall(CsrBtObexSession session, CsrUint8 *pObexPacket, CsrUint8 *pnNewMissedCall)
{
    CsrUint16 nLen;
    CsrUint16 nIndex = CsrBtObexHeaderIndex(session, pObexPacket, CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER, &nLen);

    if (nIndex && nLen > CSR_BT_OBEX_HEADER_LENGTH)
    {
        nLen += nIndex - 1;
        nIndex += CSR_BT_OBEX_HEADER_LENGTH;
        while (nIndex < nLen)
        {
            if (pObexPacket[nIndex] == CSR_BT_OBEX_PB_NEW_MISSED_CALLS_ID)
            {
                *pnNewMissedCall = CSR_BT_OBEX_GET_BYTE_AT(pObexPacket, (nIndex + CSR_BT_AUTH_TAG_LENGTH_SCHEME));
                return TRUE;
            }
            nIndex += pObexPacket[nIndex + 1] + CSR_BT_AUTH_TAG_LENGTH_SCHEME;
        }
    }
    return FALSE;
}

static CsrUcs2String * csrBtPacGenerateAbsolutePath(CsrBtPacSrcType src, CsrUcs2String * ucs2name)
{ /* Generate absolute path. This do not give alignment problems because
     PAC internal handles pMsg->name as a octet */
    CsrUint32        nNameNofBytes;
    CsrUcs2String    * wName;

    if (CSR_BT_PAC_SRC_SIM == src)
    { /* use simcard path */
        nNameNofBytes = sizeof(wTelecom) + sizeof(wSim1) + CsrUcs2ByteStrLen(ucs2name);
        wName = (CsrUcs2String *) CsrPmemAlloc(nNameNofBytes); /* must be free in ocs */
        CsrBtObexWcsCpy(wName, wSim1);                        /* "SIM1\\" */
        CsrBtObexWcsCat(wName, wTelecom);                     /* "SIM1\\telecom\\" */
        CsrBtObexWcsCat(wName, (CsrUint8 *) ucs2name);
    }
    else
    { /* use telecom path */
        nNameNofBytes = sizeof(wTelecom) + CsrUcs2ByteStrLen(ucs2name);
        wName = (CsrUcs2String *) CsrPmemAlloc(nNameNofBytes);  /* must be free in ocs */
        CsrBtObexWcsCpy(wName, wTelecom);                      /* "telecom\\" */
        CsrBtObexWcsCat(wName, (CsrUint8*) ucs2name);
    }
    return (wName);
}

static CsrUint8 * csrBtPacGeneratePullPbApplicationHeader(CsrBtPacPullPbReq *pMsg, CsrUint16 *appParametersHeaderLength)
{
    CsrUint8  * appHeader, * tmpBuf, * pnTotalAppLen;
    CsrUint16 appHeaderLength;

    if (pMsg->listStartOffset > 0)
    {
        appHeaderLength = PbRetAppParaHeaderLen(CSR_BT_PB_OBEX_FILTER_FLAG | CSR_BT_PB_OBEX_FORMAT_FLAG | CSR_BT_PB_OBEX_MAX_LIST_CNT_FLAG | CSR_BT_PB_OBEX_LISTSTARTOFFSET_FLAG);
    }
    else
    {
        appHeaderLength = PbRetAppParaHeaderLen(CSR_BT_PB_OBEX_FILTER_FLAG | CSR_BT_PB_OBEX_FORMAT_FLAG | CSR_BT_PB_OBEX_MAX_LIST_CNT_FLAG);
    }
    appHeader   = (CsrUint8 *) CsrPmemAlloc(appHeaderLength);
    tmpBuf      = appHeader;

    PbBuildAppHeader(&tmpBuf, &pnTotalAppLen);
    pbBuildFilterHeader(&tmpBuf, pnTotalAppLen, pMsg->filter);
    pbBuildFormatHeader(&tmpBuf, pnTotalAppLen, pMsg->format);
    pbBuildMaxListCntHeader(&tmpBuf, pnTotalAppLen, pMsg->maxListCnt);

    if (pMsg->listStartOffset > 0)
    {
        pbBuildListOffsetHeader(&tmpBuf, pnTotalAppLen, pMsg->listStartOffset);
    }

    *appParametersHeaderLength  = appHeaderLength;
    return (appHeader);
}

static CsrUint8 * csrBtPacGeneratePullvCardEntryApplicationHeader(CsrBtPacPullVcardEntryReq *pMsg, CsrUint16 *appParametersHeaderLength)
{
    CsrUint8  * pnTotalAppLen;
    CsrUint16 appHeaderLength = PbRetAppParaHeaderLen((PbAppParFlag)(CSR_BT_PB_OBEX_FILTER_FLAG | CSR_BT_PB_OBEX_FORMAT_FLAG));
    CsrUint8  * appHeader     = (CsrUint8 *) CsrPmemAlloc(appHeaderLength);
    CsrUint8  * tmpBuf        = appHeader;

    PbBuildAppHeader(&tmpBuf, &pnTotalAppLen);
    pbBuildFilterHeader(&tmpBuf, pnTotalAppLen, pMsg->filter);
    pbBuildFormatHeader(&tmpBuf, pnTotalAppLen, pMsg->format);
    *appParametersHeaderLength  = appHeaderLength;
    return (appHeader);
}

static CsrUint8 * csrBtPacGeneratePullvCardListApplicationHeader(CsrBtPacPullVcardListReq *pMsg, CsrUint16 *appParametersHeaderLength)
{
    CsrUint8  * appHeader, * tmpBuf, * pnTotalAppLen;
    CsrUint16 appHeaderLength = (CsrUint16)(PbRetAppParaHeaderLen(CSR_BT_PAC_OBEX_APP_PAR_LIST_FLAG) + CSR_BT_OBEX_APP_PAR_SEARCH_ATT);

    if (pMsg->searchVal)
    {
        appHeaderLength = (CsrUint16)(appHeaderLength + pbRetAppParaSearchValLen((CsrUint8 *)pMsg->searchVal));
    }

    appHeader           = (CsrUint8 *) CsrPmemAlloc(appHeaderLength);
    tmpBuf              = appHeader;

    PbBuildAppHeader(&tmpBuf, &pnTotalAppLen);
    pbBuildOrderHeader(&tmpBuf, pnTotalAppLen, pMsg->order);
    if (pMsg->searchVal)
    {
        pbBuildSearchValHeader(&tmpBuf, pnTotalAppLen, pMsg->searchVal);
    }
    PbBuildSearchAttHeader(&tmpBuf, pnTotalAppLen, pMsg->searchAtt);
    pbBuildMaxListCntHeader(&tmpBuf, pnTotalAppLen, pMsg->maxListCnt);
    pbBuildListOffsetHeader(&tmpBuf, pnTotalAppLen, pMsg->listStartOffset);
    *appParametersHeaderLength  = appHeaderLength;
    return (appHeader);
}

static CsrBool csrBtPacExtractApplicationHeaders(PacInst *pInst, CsrUint8 * obexPacket)
{
    if (obexPacket)
    {
        if (pInst->type == CSR_BT_PAC_PULL_PB_REQ ||
            pInst->type == CSR_BT_PAC_PULL_VCARD_LIST_REQ)
        {
            return (TRUE);
        }
    }
    return (FALSE);
}

static void csrBtPacMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_PAC_PRIM, msg);
}

static void csrBtPacPullPbCfmSend(PacInst *pInst, CsrBtObexResponseCode responseCode)
{
    CsrBtPacPullPbCfm *pMsg  = (CsrBtPacPullPbCfm *) CsrPmemAlloc(sizeof(CsrBtPacPullPbCfm));
    pMsg->type          = CSR_BT_PAC_PULL_PB_CFM;
    pMsg->newMissedCall = pInst->newMissedCall;
    pMsg->pbSize        = pInst->pbSize;
    pMsg->responseCode  = responseCode;
    csrBtPacMessagePut(pInst->appHandle, pMsg);
}

static void csrBtPacPullPbIndSend(CsrSchedQid appHandle, CsrUint16 bodyOffset, CsrUint16 bodyLength,
                                  CsrUint8 * obexPacket, CsrUint16 obexPacketLength)
{
    CsrBtPacPullPbInd *pMsg  = (CsrBtPacPullPbInd *) CsrPmemAlloc(sizeof(CsrBtPacPullPbInd));
    pMsg->type          = CSR_BT_PAC_PULL_PB_IND;
    pMsg->bodyLength    = bodyLength;
    pMsg->bodyOffset    = bodyOffset;
    pMsg->payload       = obexPacket;
    pMsg->payloadLength = obexPacketLength;
    csrBtPacMessagePut(appHandle, pMsg);
}

static void csrBtPacPullvCardEntryCfmSend(CsrSchedQid appHandle, CsrBtObexResponseCode responseCode)
{
    CsrBtPacPullVcardEntryCfm *pMsg = (CsrBtPacPullVcardEntryCfm *) CsrPmemAlloc(sizeof(CsrBtPacPullVcardEntryCfm));
    pMsg->type                      = CSR_BT_PAC_PULL_VCARD_ENTRY_CFM;
    pMsg->responseCode              = responseCode;
    csrBtPacMessagePut(appHandle, pMsg);
}

static void csrBtPacPullvCardEntryIndSend(CsrSchedQid appHandle, CsrUint16 bodyOffset,
                                          CsrUint16 bodyLength, CsrUint8 * obexPacket, CsrUint16 obexPacketLength)

{
    CsrBtPacPullVcardEntryInd *pMsg  = (CsrBtPacPullVcardEntryInd *) CsrPmemAlloc(sizeof(CsrBtPacPullVcardEntryInd));
    pMsg->type                  = CSR_BT_PAC_PULL_VCARD_ENTRY_IND;
    pMsg->bodyLength            = bodyLength;
    pMsg->bodyOffset            = bodyOffset;
    pMsg->payload               = obexPacket;
    pMsg->payloadLength         = obexPacketLength;
    csrBtPacMessagePut(appHandle, pMsg);
}

static void csrBtPacPullvCardListIndSend(CsrSchedQid appHandle, CsrUint16 bodyOffset,
                                         CsrUint16 bodyLength, CsrUint8 * obexPacket, CsrUint16 obexPacketLength)
{
    CsrBtPacPullVcardListInd *pMsg = (CsrBtPacPullVcardListInd *) CsrPmemAlloc(sizeof(CsrBtPacPullVcardListInd));
    pMsg->type                = CSR_BT_PAC_PULL_VCARD_LIST_IND;
    pMsg->bodyLength          = bodyLength;
    pMsg->bodyOffset          = bodyOffset;
    pMsg->payload             = obexPacket;
    pMsg->payloadLength       = obexPacketLength;
    csrBtPacMessagePut(appHandle, pMsg);
}

static void csrBtPacPullvCardListCfmSend(PacInst *pInst, CsrBtObexResponseCode responseCode)
{
    CsrBtPacPullVcardListCfm *pMsg   = (CsrBtPacPullVcardListCfm *) CsrPmemAlloc(sizeof(CsrBtPacPullVcardListCfm));
    pMsg->type                  = CSR_BT_PAC_PULL_VCARD_LIST_CFM;
    pMsg->newMissedCall         = pInst->newMissedCall;
    pMsg->pbSize                = pInst->pbSize;
    pMsg->responseCode          = responseCode;
    csrBtPacMessagePut(pInst->appHandle, pMsg);
}

static void csrBtPacPullIndHandler(PacInst *pInst, CsrUint16 bodyOffset, CsrUint16 bodyLength,
                                   CsrUint8 * obexPacket, CsrUint16 obexPacketLength)
{
    switch (pInst->type)
    {
        case CSR_BT_PAC_PULL_PB_REQ:
            {
                csrBtPacPullPbIndSend(pInst->appHandle, bodyOffset, bodyLength, obexPacket, obexPacketLength);
                break;
            }
        case CSR_BT_PAC_PULL_VCARD_LIST_REQ:
            {
                csrBtPacPullvCardListIndSend(pInst->appHandle, bodyOffset, bodyLength, obexPacket, obexPacketLength);
                break;
            }
        default :
            {/* CSR_BT_PAC_PULL_VCARD_ENTRY_REQ */
                csrBtPacPullvCardEntryIndSend(pInst->appHandle, bodyOffset, bodyLength, obexPacket, obexPacketLength);
                break;
            }
    }
}

static void csrBtPacPullCfmHandler(PacInst *pInst, CsrBtObexResponseCode responseCode)
{
    switch (pInst->type)
    {
        case CSR_BT_PAC_PULL_PB_REQ:
            {
                csrBtPacPullPbCfmSend(pInst, responseCode);
                break;
            }
        case CSR_BT_PAC_PULL_VCARD_LIST_REQ:
            {
                csrBtPacPullvCardListCfmSend(pInst, responseCode);
                break;
            }
        default :
            {/* CSR_BT_PAC_PULL_VCARD_ENTRY_REQ */
                csrBtPacPullvCardEntryCfmSend(pInst->appHandle, responseCode);
                break;
            }
    }
}

static void csrBtPacConnectResultHandler(void                     *instData,
                                         CsrBtResultCode          resultCode,
                                         CsrBtSupplier            resultSupplier,
                                         CsrBtDeviceAddr          deviceAddr,
                                         CsrBtConnId              cid,
                                         CsrUint16                maxPeerObexPacketLength,
                                         CmnCsrBtLinkedListStruct *sdpTag,
                                         CsrUint16                obexPacketLength,
                                         CsrUint8               *obexPacket)
{
    PacInst *pInst              = instData;
    CsrBtPacConnectCfm *pMsg    = (CsrBtPacConnectCfm *) CsrPmemAlloc(sizeof(CsrBtPacConnectCfm));

    pMsg->type                  = CSR_BT_PAC_CONNECT_CFM;
    pMsg->resultCode            = resultCode;
    pMsg->resultSupplier        = resultSupplier;
    pMsg->obexPeerMaxPacketSize = maxPeerObexPacketLength;
    pMsg->supportedRepositories = csrBtPacExtractSupportedRepositories(sdpTag);
    pMsg->btConnId              = cid;

    csrBtPacMessagePut(pInst->appHandle, pMsg);

    CsrBtUtilBllFreeLinkedList(&(sdpTag), CsrBtUtilBllPfreeWrapper);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(deviceAddr);
    CsrPmemFree(obexPacket);

}

static void csrBtPacAuthenticateIndHandler(void          *instData,
                                           CsrBtDeviceAddr deviceAddr,
                                           CsrUint8       options,
                                           CsrUint16      realmLength,
                                           CsrUint8    *realm)
{
    PacInst * pInst          = instData;
    CsrBtPacAuthenticateInd *pMsg = (CsrBtPacAuthenticateInd *)CsrPmemAlloc(sizeof(CsrBtPacAuthenticateInd));
    pMsg->type               = CSR_BT_PAC_AUTHENTICATE_IND;
    pMsg->deviceAddr         = deviceAddr;
    pMsg->options            = options;
    pMsg->realmLength        = realmLength;
    pMsg->realm              = realm;
    csrBtPacMessagePut(pInst->appHandle, pMsg);
}

static void csrBtPacDisconnectResultHandler(void                *instData,
                                            CsrBtReasonCode     reasonCode,
                                            CsrBtSupplier reasonSupplier,
                                            CsrUint8          *obexPacket,
                                            CsrUint16            obexPacketLength)
{
    PacInst * pInst        = instData;
    CsrBtPacDisconnectInd *pMsg = (CsrBtPacDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtPacDisconnectInd));
    pMsg->type             = CSR_BT_PAC_DISCONNECT_IND;
    pMsg->reasonCode       = reasonCode;
    pMsg->reasonSupplier   = reasonSupplier;
    csrBtPacMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtPacCommonPullResultHandler(void                    *instData,
                                            CsrBtObexResponseCode   responseCode,
                                            CsrBool                 bodyHeader,
                                            CsrUint16               bodyLength,
                                            CsrUint16               bodyOffset,
                                            CsrUint8              *obexPacket,
                                            CsrUint16               obexPacketLength)
{
    PacInst * pInst = instData;

    if (csrBtPacExtractApplicationHeaders(pInst, obexPacket))
    { /* Try to extract the phone book size plus missed called from the OBEX
         application parameters header */
        pbGetObexPhonebookSize(CSR_BT_OBEX_REQUEST, obexPacket, &pInst->pbSize);
        pbGetObexMissedCall(CSR_BT_OBEX_REQUEST, obexPacket, &pInst->newMissedCall);
    }

    switch (responseCode)
    {
        case CSR_BT_OBEX_CONTINUE_RESPONSE_CODE:
            {
                if (bodyHeader && bodyLength > 0 && bodyOffset > 0)
                {
                    csrBtPacPullIndHandler(pInst, bodyOffset, bodyLength, obexPacket, obexPacketLength);
                    obexPacket = NULL;
                }
                else
                {
                    ObexUtilGetContinueRequest(pInst->obexInst, CSR_BT_OBEX_SRMP_INVALID);
                }
                break;
            }
        case CSR_BT_OBEX_SUCCESS_RESPONSE_CODE:
            {
                if (bodyHeader && bodyLength > 0 && bodyOffset > 0)
                {
                    csrBtPacPullIndHandler(pInst, bodyOffset, bodyLength, obexPacket, obexPacketLength);
                    obexPacket      = NULL;
                    pInst->isFinal  = TRUE;
                }
                else
                {
                    csrBtPacPullCfmHandler(pInst, responseCode);
                }
                break;
            }
        default:
            {
                csrBtPacPullCfmHandler(pInst, responseCode);
                break;
            }
    }
    CsrPmemFree(obexPacket);
}

static void csrBtPacSetFolderResultHandler(void                  *instData,
                                           CsrBtObexResponseCode responseCode,
                                           CsrUint8            *obexPacket,
                                           CsrUint16              obexPacketLength)
{
    PacInst * pInst            = instData;
    CsrBtPacSetFolderCfm *pMsg = (CsrBtPacSetFolderCfm *) CsrPmemAlloc(sizeof(CsrBtPacSetFolderCfm));
    pMsg->type                 = CSR_BT_PAC_SET_FOLDER_CFM;
    pMsg->responseCode         = responseCode;
    csrBtPacMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtPacSetBackFolderResultHandler(void                  *instData,
                                               CsrBtObexResponseCode responseCode,
                                               CsrUint8            *obexPacket,
                                               CsrUint16              obexPacketLength)
{
    PacInst * pInst                = instData;
    CsrBtPacSetBackFolderCfm *pMsg = (CsrBtPacSetBackFolderCfm *) CsrPmemAlloc(sizeof(CsrBtPacSetBackFolderCfm));
    pMsg->type                     = CSR_BT_PAC_SET_BACK_FOLDER_CFM;
    pMsg->responseCode             = responseCode;
    csrBtPacMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtPacSetRootFolderResultHandler(void                  *instData,
                                               CsrBtObexResponseCode responseCode,
                                               CsrUint8            *obexPacket,
                                               CsrUint16              obexPacketLength)
{
    PacInst * pInst                = instData;
    CsrBtPacSetRootFolderCfm *pMsg = (CsrBtPacSetRootFolderCfm *) CsrPmemAlloc(sizeof(CsrBtPacSetRootFolderCfm));
    pMsg->type                     = CSR_BT_PAC_SET_ROOT_FOLDER_CFM;
    pMsg->responseCode             = responseCode;
    csrBtPacMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtPacAbortResultHandler(void                  *instData,
                                       CsrBtObexResponseCode responseCode,
                                       CsrUint8            *obexPacket,
                                       CsrUint16              obexPacketLength)
{
    PacInst * pInst     = instData;
    CsrBtPacAbortCfm *pMsg   = (CsrBtPacAbortCfm *) CsrPmemAlloc(sizeof(CsrBtPacAbortCfm));
    pMsg->type          = CSR_BT_PAC_ABORT_CFM;
    csrBtPacMessagePut(pInst->appHandle, pMsg);

    CSR_UNUSED(responseCode);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

CsrUint8 CsrBtPacConnectReqHandler(PacInst *pInst, void *msg)
{
    CsrUint16 index;
    CsrUint8   numOfHeaders             = 0;
    CsrBtPacConnectReq *pMsg            = (CsrBtPacConnectReq *) msg;
    CmnCsrBtLinkedListStruct * sdpTag   = NULL;
    CsrUint8 tempTarget[16]             = CSR_BT_PAC_TARGET_UUID;
    CsrUint8   headerPriorityTable[2]   = {CSR_BT_OBEX_UTIL_LENGTH_HEADER, CSR_BT_OBEX_UTIL_COUNT_HEADER};
    ObexUtilDigestChallengeType * chal  = ObexUtilReturnDigestChallengePointer(pMsg->authorize,
                                                                               &pMsg->password,
                                                                               pMsg->passwordLength,
                                                                               &pMsg->userId,
                                                                               &pMsg->realm,
                                                                               pMsg->realmLength);
    CsrUint8 *target                    = (CsrUint8 *) CsrPmemAlloc(CSR_BT_PAC_TARGET_LENGTH);
    pInst->appHandle                    = pMsg->appHandle;
    pInst->localMaxPacketSize = pMsg->maxPacketSize;
    pInst->windowSize = pMsg->windowSize;
    pInst->localSrmEnable = pMsg->srmEnable;

    CsrMemCpy(target, tempTarget, CSR_BT_PAC_TARGET_LENGTH);

    sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, CSR_BT_OBEX_PBA_SERVER_PROFILE_UUID, &index);
    CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_SUPPORTED_REPOSITORIES_ATTRIBUTE_IDENTIFIER, NULL, 0);
    CsrBtUtilSdrInsertLocalServerChannel(sdpTag, index, CSR_BT_NO_SERVER);

    if (pMsg->length > 0)
    {
        numOfHeaders = (CsrUint8)(numOfHeaders + 1);
    }

    if (pMsg->count > 0)
    {
        numOfHeaders = (CsrUint8)(numOfHeaders + 1);
    }

    return(ObexUtilConnectRequest(pInst->obexInst,
                                  &sdpTag,
                                  pMsg->destination,
                                  pInst->secOutgoing,
                                  pInst->localMaxPacketSize,
                                  CSR_BT_PAC_PROFILE_DEFAULT_MTU_SIZE,
                                  &target,
                                  CSR_BT_PAC_TARGET_LENGTH,
                                  numOfHeaders,
                                  headerPriorityTable,
                                  (CSR_BT_PAC_LP_SUPERVISION_TIMEOUT * 1000),
                                  &chal,
                                  pMsg->length,
                                  pMsg->count,
                                  NULL,
                                  0,
                                  NULL,
                                  csrBtPacConnectResultHandler,
                                  csrBtPacAuthenticateIndHandler,
                                  csrBtPacDisconnectResultHandler,
                                  NULL,
                                  pInst->windowSize,
                                  pInst->localSrmEnable));
}

CsrUint8 CsrBtPacAuthenticateResHandler(PacInst *pInst, void *msg)
{
    CsrBtPacAuthenticateRes *pMsg = (CsrBtPacAuthenticateRes *) msg;

    CsrUint8 userIdLength  = 0;

    if(pMsg->userId)
    {
        userIdLength  = (CsrUint8) CsrStrLen((const char *)pMsg->userId);
    }

    if (userIdLength > CSR_BT_OBEX_MAX_AUTH_USERID_LENGTH)
    {
        pMsg->userId[CSR_BT_OBEX_MAX_AUTH_USERID_LENGTH] = '\0';
    }

    return(ObexUtilCliAuthenticateResponse(pInst->obexInst, pMsg->passwordLength, &pMsg->password, &pMsg->userId, NULL));
}

CsrUint8 CsrBtPacPullPbReqHandler(PacInst *pInst, void *msg)
{
    CsrBtPacPullPbReq *pMsg      = (CsrBtPacPullPbReq *) msg;
    pInst->pbSize           = 0;
    pInst->newMissedCall    = 0;
    pInst->isFinal          = FALSE;
    pInst->type             = CSR_BT_PAC_PULL_PB_REQ;

    if (pMsg->ucs2name)
    {
        CsrUint16    appParametersHeaderLength;
        CsrUint8     headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_TYPE_HEADER, CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
        CsrUint8     * appParametersHeader   = csrBtPacGeneratePullPbApplicationHeader(pMsg, &appParametersHeaderLength);
        CsrUint8     * absolutePath          = csrBtPacGenerateAbsolutePath(pMsg->src, pMsg->ucs2name);

        CsrPmemFree(pMsg->ucs2name);
        return(ObexUtilGetRequest(pInst->obexInst,
                                  headerPriorityTable,
                                  SZ_PHONEBOOK_TYPE,
                                  &absolutePath,
                                  NULL,
                                  appParametersHeaderLength,
                                  &appParametersHeader,
                                  0,
                                  NULL,
                                  CSR_BT_OBEX_SRMP_INVALID,
                                  csrBtPacCommonPullResultHandler,
                                  NULL));
    }
    else
    {
        csrBtPacCommonPullResultHandler(pInst, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE, FALSE,
                                        0, 0, NULL, 0);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
}

CsrUint8 CsrBtPacPullPbResHandler(PacInst *pInst, void *msg)
{
    CSR_UNUSED(msg);
    if (pInst->isFinal)
    {
        csrBtPacPullPbCfmSend(pInst, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    {
        return(ObexUtilGetContinueRequest(pInst->obexInst, CSR_BT_OBEX_SRMP_INVALID));
    }
}

CsrUint8 CsrBtPacPullvCardEntryReqHandler(PacInst *pInst, void *msg)
{
    CsrUint16    appParametersHeaderLength;
    CsrUint8     headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER, CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtPacPullVcardEntryReq *pMsg          = (CsrBtPacPullVcardEntryReq *) msg;
    CsrUint8     * appParametersHeader   = csrBtPacGeneratePullvCardEntryApplicationHeader(pMsg, &appParametersHeaderLength);
    pInst->isFinal                      = FALSE;
    pInst->type                         = CSR_BT_PAC_PULL_VCARD_ENTRY_REQ;

    return(ObexUtilGetRequest(pInst->obexInst,
                              headerPriorityTable,
                              SZ_VCARD_TYPE,
                              &pMsg->ucs2name,
                              NULL,
                              appParametersHeaderLength,
                              &appParametersHeader,
                              0,
                              NULL,
                              CSR_BT_OBEX_SRMP_INVALID,
                              csrBtPacCommonPullResultHandler,
                              NULL));
}

CsrUint8 CsrBtPacPullvCardEntryResHandler(PacInst *pInst, void *msg)
{
    CSR_UNUSED(msg);
    if (pInst->isFinal)
    {
        csrBtPacPullvCardEntryCfmSend(pInst->appHandle, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    {
        return(ObexUtilGetContinueRequest(pInst->obexInst, CSR_BT_OBEX_SRMP_INVALID));
    }
}

CsrUint8 CsrBtPacPullvCardListReqHandler(PacInst *pInst, void *msg)
{
    CsrUint16    appParametersHeaderLength;
    CsrUint8     headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER, CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtPacPullVcardListReq *pMsg           = (CsrBtPacPullVcardListReq*) msg;
    CsrUint8     * appParametersHeader   = csrBtPacGeneratePullvCardListApplicationHeader(pMsg, &appParametersHeaderLength);
    pInst->pbSize                       = 0;
    pInst->newMissedCall                = 0;
    pInst->isFinal                      = FALSE;
    pInst->type                         = CSR_BT_PAC_PULL_VCARD_LIST_REQ;
    CsrPmemFree(pMsg->searchVal);
    return(ObexUtilGetRequest(pInst->obexInst,
                              headerPriorityTable,
                              SZ_VCARD_LISTING_TYPE,
                              &pMsg->ucs2name,
                              NULL,
                              appParametersHeaderLength,
                              &appParametersHeader,
                              0,
                              NULL,
                              CSR_BT_OBEX_SRMP_INVALID,
                              csrBtPacCommonPullResultHandler,
                              NULL));
}

CsrUint8 CsrBtPacPullvCardListResHandler(PacInst *pInst, void *msg)
{
    CSR_UNUSED(msg);
    if (pInst->isFinal)
    {
        csrBtPacPullvCardListCfmSend(pInst, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    {
        return(ObexUtilGetContinueRequest(pInst->obexInst, CSR_BT_OBEX_SRMP_INVALID));
    }
}

CsrUint8 CsrBtPacSetFolderReqHandler(PacInst *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[3]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtPacSetFolderReq *pMsg           = (CsrBtPacSetFolderReq *) msg;

    return (ObexUtilSetPathRequest(pInst->obexInst, 0x02, headerPriorityTable, &pMsg->ucs2name, NULL, 0, NULL, csrBtPacSetFolderResultHandler));
}

CsrUint8 CsrBtPacSetBackFolderReqHandler(PacInst *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[3]  = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CSR_UNUSED(msg);

    return (ObexUtilSetPathRequest(pInst->obexInst, 0x03, headerPriorityTable, NULL, NULL, 0, NULL, csrBtPacSetBackFolderResultHandler));
}

CsrUint8 CsrBtPacSetRootFolderReqHandler(PacInst *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[3]  = {CSR_BT_OBEX_UTIL_EMPTY_NAME_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CSR_UNUSED(msg);

    return (ObexUtilSetPathRequest(pInst->obexInst, 0x02, headerPriorityTable, NULL, NULL, 0, NULL, csrBtPacSetRootFolderResultHandler));
}

CsrUint8 CsrBtPacAbortReqHandler(PacInst *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return(ObexUtilAbortRequest(pInst->obexInst, NULL, csrBtPacAbortResultHandler));
}

CsrUint8 CsrBtPacDisconnectReqHandler(PacInst *pInst, void *msg)
{
    CsrBtPacDisconnectReq *pMsg = (CsrBtPacDisconnectReq *) msg;
    return (ObexUtilDisconnectRequest(pInst->obexInst, pMsg->normalDisconnect, NULL));
}

CsrUint8 CsrBtPacCancelConnectReqHandler(PacInst *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return (ObexUtilDisconnectRequest(pInst->obexInst, FALSE, NULL));
}

CsrUint8 CsrBtPacSecurityOutReqHandler(PacInst *pInst, void *msg)
{
    CsrBtPacSecurityOutCfm *prim;
    CsrBtPacSecurityOutReq *pMsg;

    prim = (CsrBtPacSecurityOutCfm *) CsrPmemAlloc(sizeof(CsrBtPacSecurityOutCfm));
    pMsg = (CsrBtPacSecurityOutReq *) msg;

    prim->type              = CSR_BT_PAC_SECURITY_OUT_CFM;
    prim->resultCode        = CsrBtScSetSecOutLevel(&pInst->secOutgoing, pMsg->secLevel,
                                                    CSR_BT_PBAP_MANDATORY_SECURITY_OUTGOING,
                                                    CSR_BT_PBAP_DEFAULT_SECURITY_OUTGOING,
                                                    CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                                    CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    prim->resultSupplier = CSR_BT_SUPPLIER_OBEX_PROFILES;

    csrBtPacMessagePut(pMsg->appHandle, prim);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

void CsrBtPacCmSdsRegisterReqHandler(PacInst *pInst)
{
    CsrUint8 *record = (CsrUint8 *) CsrPmemAlloc(sizeof(serviceRecord));

    CsrMemCpy(record, serviceRecord,sizeof(serviceRecord));
    CsrBtCmSdsRegisterReqSend(CSR_BT_PAC_IFACEQUEUE, record, sizeof(serviceRecord));
}

void CsrBtPacCmSdsRegistertCfmHandler(PacInst *pInst, void *msg)
{
    CsrBtCmSdsRegisterCfm *pMsg = (CsrBtCmSdsRegisterCfm * )msg;

    if (pMsg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && pMsg->resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The service record has been register with success. */
        ;
    }
    else
    { /* Fail to register the service record. Keep going until the
         request is a success. CM makes sure it is done with a small delay */
        CsrBtPacCmSdsRegisterReqHandler(pInst);
    }
}

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_pas_handler.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_pb_obex.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define CSR_BT_PAS_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX  (24)
#define CSR_BT_PAS_RFCOMM_FEATURES_SERVICE_RECORD_INDEX        (72)
#define CSR_BT_PAS_RFCOMM_REPOSITORIES_SERVICE_RECORD_INDEX    (77)
#define CSR_BT_PAS_UUID_STR_LEN                                (16)
#define CSR_BT_PAS_HEADER_SIZE                                 (16)
#define CSR_BT_PAS_NO_HEADER                                 0x0000
#define CSR_BT_PAS_HEADER_OK                                 0x0001

static const char CSR_BT_PAS_TEXTTYPE_PB_PHONEBOOK[]        = {"x-bt/phonebook"};
static const char CSR_BT_PAS_TEXTTYPE_PB_VCARD_LISTING[]    = {"x-bt/vcard-listing"};
static const char CSR_BT_PAS_TEXTTYPE_PB_VCARD[]            = {"x-bt/vcard"};

static CsrUint8 csrBtPasActivateStartHandler(PasInst *pInst);

static const CsrUint8 serviceRecord[] =
{
    /* Service class ID list */
    0x09, 0x00, 0x01,   /* AttrID , ServiceClassIDList */
    0x35, 0x03,         /* 6 bytes in total DataElSeq */
    /* PBAP */
    0x19, 0x11, 0x2F,   /* 2 byte UUID, Phonebook Access Uuid16 0x112F PBAP Service Class */

    /* protocol descriptor list */
    0x09, 0x00, 0x04,   /* AttrId ProtocolDescriptorList */
    0x35, 0x11,         /* 17 bytes in total DataElSeq */
    /* L2CAP */
    0x35, 0x03,         /* 3 bytes in DataElSeq */
    0x19, 0x01, 0x00,   /* 2 byte UUID, Protocol = L2CAP */
    /* RFCOMM */
    0x35, 0x05,         /* 5 bytes in DataElSeq */
    0x19, 0x00, 0x03,   /* 2 byte UUID Protocol = RFCOMM */
    0x08, 0x00,         /* 1 byte UINT - server channel template value 0 - to be filled in by app (index:24) */
    /* OBEX */
    0x35, 0x03,         /* 3 bytes in DataElSeq */
    0x19, 0x00, 0x08,   /* 2 byte UUID, Protocol = OBEX */
    /* bluetooth profile descriptor list */
    0x09, 0x00, 0x09,   /* AttrId ProtocolDescriptorList */
    0x35, 0x08,         /* 8 bytes in total DataElSeq */
    /* Supported profile */
    0x35, 0x06,         /* 6 bytes in DataElSeq */
    0x19, 0x11, 0x30,   /* 2 byte UUID, Supported profile = PBAP = 0x1130 */
    /* Profile version */
    0x09, 0x01, 0x00,   /* 2 byte UINT - Profile version = 0x0100 = version 1.0 */
    /* service name */
    0x09, 0x01, 0x00,   /* AttrId - Service Name. Use language base attribute 0x0100 (primary language) */
    0x25, 0x14,          /* length of service string */
    'P', 'h', 'o', 'n', 'e', 'b', 'o', 'o', 'k', ' ', 'A', 'c', 'c', 'e', 's', 's', ' ', 'P', 'S', 'E', /* string - "Phonebook Access PSE" */
    /* Supported Features */
    0x09, 0x03, 0x11,   /* AttrId - Supported Formats List */
    0x08, 0x03,         /* Bit 0 - Phonebook Download, Bit 1 - Phonebook Browsing */
    /* Supported Repositories */
    0x09, 0x03, 0x14,   /* AttrId - Supported  Repositories */
    0x08, 0x03,          /* Bit 0 - Local Phonebook, Bit 1 - SIM card Phonebook */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,    /* AttrId = BrowseGroupList */
    0x35, 0x03,          /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,    /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

static const CsrUint8 pasUuidString[CSR_BT_PAS_UUID_STR_LEN] = {0x79, 0x61, 0x35, 0xF0, 0xF0, 0xC5, 0x11, 0xD8, 0x09, 0x66, 0x08, 0x00, 0x20, 0x0C, 0x9A, 0x66};
static const CsrUint8 szTelecom[] =  {0,'t',0,'e',0,'l',0,'e',0,'c',0,'o',0,'m',0,'/',0,0}; /* Big endian */
static const CsrUint8 szSim1[] = {0,'S',0,'I',0,'M',0,'1',0,'/',0,0};
static const CsrUint8 wzMch[] = {0,'M',0,'C',0,'H',0,'.',0,'V',0,'C',0,'F',0,0};
static const CsrUint8 wzMchFolder[] = {0,'M',0,'C',0,'H',0,0};

static void csrBtPasMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_PAS_PRIM, msg);
}

static void csrBtPasSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtPasSecurityInCfm *prim = (CsrBtPasSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtPasSecurityInCfm));
    prim->type                  = CSR_BT_PAS_SECURITY_IN_CFM;
    prim->resultCode            = resultCode;
    prim->resultSupplier        = resultSupplier;
    csrBtPasMessagePut(appHandle, prim);
}

static void csrBtPasConnectIndSend(CsrSchedQid           hApp, 
                                   CsrUint32        connectionId, 
                                   CsrUint16        nPacketSizeMax, 
                                   CsrBtDeviceAddr  *pDeviceAddr,
                                   CsrUint32        length,
                                   CsrUint32        count,
                                   CsrBtConnId      btConnId)
{
    CsrBtPasConnectInd *pMsg        = CsrPmemAlloc(sizeof(CsrBtPasConnectInd));
    pMsg->type                      = CSR_BT_PAS_CONNECT_IND;
    pMsg->connectionId              = connectionId;
    pMsg->obexPeerMaxPacketSize     = (CsrUint16)(nPacketSizeMax - CSR_BT_PAS_HEADER_SIZE);
    pMsg->deviceAddr                = *pDeviceAddr;
    pMsg->length                    = length;
    pMsg->count                     = count;
    pMsg->btConnId                  = btConnId;

    csrBtPasMessagePut(hApp, pMsg);
}

static void csrBtPasSetBackFolderIndSend(CsrSchedQid hApp, CsrUint32 connectionId)
{
    CsrBtPasSetBackFolderInd *pMsg  = CsrPmemAlloc(sizeof(CsrBtPasSetBackFolderInd));
    pMsg->type                      = CSR_BT_PAS_SET_BACK_FOLDER_IND;
    pMsg->connectionId              = connectionId;
    csrBtPasMessagePut(hApp, pMsg);
}

static void csrBtPasSetRootFolderIndSend(CsrSchedQid hApp, CsrUint32 connectionId)
{
    CsrBtPasSetRootFolderInd *pMsg  = CsrPmemAlloc(sizeof(CsrBtPasSetRootFolderInd));
    pMsg->type                      = CSR_BT_PAS_SET_ROOT_FOLDER_IND;
    pMsg->connectionId              = connectionId;
    csrBtPasMessagePut(hApp, pMsg);
}

static void csrBtPasSetFolderIndSend(CsrSchedQid     hApp, 
                                     CsrUint32  connectionId, 
                                     CsrUint16  ucs2NameOffset,
                                     CsrUint8 *pPayload, 
                                     CsrUint16  nPayloadLen)
{
    CsrBtPasSetFolderInd *pMsg  = CsrPmemAlloc(sizeof(CsrBtPasSetFolderInd));
    pMsg->type                  = CSR_BT_PAS_SET_FOLDER_IND;
    pMsg->connectionId          = connectionId;
    pMsg->ucs2nameOffset        = ucs2NameOffset;
    pMsg->payload               = pPayload;
    pMsg->payloadLength         = nPayloadLen;
    csrBtPasMessagePut(hApp, pMsg);
}

static void csrBtPasPullMchSizeIndSend(CsrSchedQid       hApp, 
                                       CsrUint32    connectionId, 
                                       CsrBtPasSrc  src)
{
    CsrBtPasPullMchSizeInd *pMsg = CsrPmemAlloc(sizeof(CsrBtPasPullMchSizeInd));
    pMsg->type                   = CSR_BT_PAS_PULL_MCH_SIZE_IND;
    pMsg->connectionId           = connectionId;
    pMsg->src                    = src;
    csrBtPasMessagePut(hApp, pMsg);
}

static void csrBtPasPullMchIndSend(CsrSchedQid           hApp, 
                                   CsrUint32        connectionId, 
                                   CsrBtPasSrc      src, 
                                   CsrUint8         filter[8],
                                   CsrBtPasFormat   format, 
                                   CsrUint16        nMaxListCnt, 
                                   CsrUint16        nStartOffset)
{
    CsrBtPasPullMchInd *pMsg = CsrPmemAlloc(sizeof(CsrBtPasPullMchInd));

    pMsg->type               = CSR_BT_PAS_PULL_MCH_IND;
    pMsg->connectionId       = connectionId;
    CsrMemCpy(pMsg->filter, filter, sizeof(pMsg->filter));
    pMsg->format             = format;
    pMsg->maxListCnt         = nMaxListCnt;
    pMsg->src                = src;
    pMsg->listStartOffset    = nStartOffset;
    csrBtPasMessagePut(hApp, pMsg);
}

static void csrBtPasPullPbSizeIndSend(CsrSchedQid        hApp, 
                                      CsrUint32     connectionId, 
                                      CsrUint16     ucs2NameOffset,
                                      CsrBtPasSrc   src, 
                                      CsrUint8    *pObexData, 
                                      CsrUint16     nObexDataLen)
{
    CsrBtPasPullPbSizeInd *pMsg = CsrPmemAlloc(sizeof(CsrBtPasPullPbSizeInd));
    pMsg->type                  = CSR_BT_PAS_PULL_PB_SIZE_IND;
    pMsg->connectionId          = connectionId;
    pMsg->ucs2nameOffset        = ucs2NameOffset;
    pMsg->src                   = src;
    pMsg->payload               = pObexData;
    pMsg->payloadLength         = nObexDataLen;
    csrBtPasMessagePut(hApp, pMsg);
}

static void csrBtPasPullPbIndSend(CsrSchedQid            hApp, 
                                  CsrUint32         connectionId, 
                                  CsrUint16         ucs2NameOffset, 
                                  CsrBtPasSrc       src,
                                  CsrUint8          filter[8], 
                                  CsrBtPasFormat    format, 
                                  CsrUint16         nMaxListCnt,
                                  CsrUint16         nStartOffset, 
                                  CsrUint8        *pObexData, 
                                  CsrUint16         nObexDataLen)
{
    CsrBtPasPullPbInd *pMsg = CsrPmemAlloc(sizeof(CsrBtPasPullPbInd));
    pMsg->type              = CSR_BT_PAS_PULL_PB_IND;
    pMsg->connectionId      = connectionId;
    pMsg->ucs2nameOffset    = ucs2NameOffset;
    CsrMemCpy(pMsg->filter, filter, sizeof(pMsg->filter));
    pMsg->format            = format;
    pMsg->maxListCnt        = nMaxListCnt;
    pMsg->listStartOffset   = nStartOffset;
    pMsg->src               = src;
    pMsg->payload           = pObexData;
    pMsg->payloadLength     = nObexDataLen;
    csrBtPasMessagePut(hApp, pMsg);
}

static void csrBtPasPullvCardMchListSizeIndSend(CsrSchedQid hApp, CsrUint32 connectionId)
{
    CsrBtPasPullVcardMchListSizeInd *pMsg   = CsrPmemAlloc(sizeof(CsrBtPasPullVcardMchListSizeInd));
    pMsg->type                              = CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND;
    pMsg->connectionId                      = connectionId;
    csrBtPasMessagePut(hApp, pMsg);
}

static void csrBtPasPullvCardMchListIndSend(CsrSchedQid              hApp, 
                                            CsrUint32           connectionId, 
                                            CsrBtPasOrder       order,
                                            CsrUint16           nSearchVal, 
                                            CsrUint16           nSearchValLen, 
                                            CsrBtPasSearchAtt   searchAtt,
                                            CsrUint16           nMaxLstCnt, 
                                            CsrUint16           nStartOffset,
                                            CsrUint8          *pPayload, 
                                            CsrUint16           nPayloadLen)
{
    CsrBtPasPullVcardMchListInd *pMsg   = CsrPmemAlloc(sizeof(CsrBtPasPullVcardMchListInd));

    pMsg->type                          = CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND;
    pMsg->connectionId                  = connectionId;
    pMsg->order                         = order;
    pMsg->uft8SearchValOffset           = nSearchVal;
    pMsg->searchValLength               = nSearchValLen;
    pMsg->searchAtt                     = searchAtt;
    pMsg->maxListCnt                    = nMaxLstCnt;
    pMsg->listStartOffset               = nStartOffset;
    pMsg->payload                       = pPayload;
    pMsg->payloadLength                 = nPayloadLen;
    csrBtPasMessagePut(hApp, pMsg);
}

static void csrBtPasPullvCardListSizeIndSend(CsrSchedQid     hApp, 
                                             CsrUint32  connectionId, 
                                             CsrUint16  ucs2NameOffset,
                                             CsrUint8 *pPayload, 
                                             CsrUint16  nPayloadLen)
{
    CsrBtPasPullVcardListSizeInd *pMsg = CsrPmemAlloc(sizeof(CsrBtPasPullVcardListSizeInd));

    pMsg->type              = CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND;
    pMsg->connectionId      = connectionId;
    pMsg->ucs2nameOffset    = ucs2NameOffset;
    pMsg->payload           = pPayload;
    pMsg->payloadLength     = nPayloadLen;
    csrBtPasMessagePut(hApp, pMsg);
}

static void csrBtPasPullvCardListIndSend(CsrSchedQid             hApp, 
                                         CsrUint32          connectionId, 
                                         CsrUint16          ucs2NameOffset,
                                         CsrBtPasOrder      order, 
                                         CsrUint16          nSearchVal, 
                                         CsrUint16          nSearchValLen,
                                         CsrBtPasSearchAtt  searchAtt, 
                                         CsrUint16          nMaxLstCnt,
                                         CsrUint16          nStartOffset, 
                                         CsrUint8         *pPayload, 
                                         CsrUint16          nPayloadLen)
{
    CsrBtPasPullVcardListInd *pMsg  = CsrPmemAlloc(sizeof(CsrBtPasPullVcardListInd));
    pMsg->type                      = CSR_BT_PAS_PULL_VCARD_LIST_IND;
    pMsg->connectionId              = connectionId;
    pMsg->ucs2nameOffset            = ucs2NameOffset;
    pMsg->order                     = order;
    pMsg->uft8SearchValOffset       = nSearchVal;
    pMsg->searchValLength           = nSearchValLen;
    pMsg->searchAtt                 = searchAtt;
    pMsg->maxListCnt                = nMaxLstCnt;
    pMsg->listStartOffset           = nStartOffset;
    pMsg->payload                   = pPayload;
    pMsg->payloadLength             = nPayloadLen;
    csrBtPasMessagePut(hApp, pMsg);
}

static void csrBtPasPullvCardEntryIndSend(CsrSchedQid            hApp, 
                                          CsrBtConnId       cid, 
                                          CsrUint16         ucs2NameOffset, 
                                          CsrUint8          filter[8],
                                          CsrBtPasFormat    format, 
                                          CsrUint8        *pPayload, 
                                          CsrUint16         nPayloadLen)
{
    CsrBtPasPullVcardEntryInd *pMsg = CsrPmemAlloc(sizeof(CsrBtPasPullVcardEntryInd));
    pMsg->type                      = CSR_BT_PAS_PULL_VCARD_ENTRY_IND;
    pMsg->connectionId              = cid;
    pMsg->ucs2nameOffset            = ucs2NameOffset;
    CsrMemCpy(pMsg->filter, filter, sizeof(pMsg->filter));
    pMsg->format                    = format;
    pMsg->payload                   = pPayload;
    pMsg->payloadLength             = nPayloadLen;
    csrBtPasMessagePut(hApp, pMsg);
}

static void csrBtPasAuthCfmSend(CsrSchedQid hApp)
{   /* Digest validation success */
    CsrBtPasAuthenticateCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtPasAuthenticateCfm));

    pMsg->type = CSR_BT_PAS_AUTHENTICATE_CFM;
    csrBtPasMessagePut(hApp, pMsg);
}

static void pbBuildPhonebookSizeHeader(CsrUint8 **ppBuf, CsrUint8 *pAppTotalLen, CsrUint16 nPbSize)
{
    CsrUint8 *pBuf = *ppBuf;

    *pBuf = CSR_BT_OBEX_PB_PHONEBOOK_SIZE_ID; /* tag */
    pBuf++;
    *pBuf = 0x02; /* length */
    pBuf++;
    pBuf[0] = (CsrUint8)(nPbSize >> 8); /* value */
    pBuf[1] = (CsrUint8)(nPbSize);
    pBuf += 2;
    (*pAppTotalLen) += CSR_BT_OBEX_APP_PAR_PB_SIZE; /* update total length */
    *ppBuf = pBuf;
}

static void pbBuildMissedCallHeader(CsrUint8 **ppBuf, CsrUint8 *pAppTotalLen, CsrUint8 nNewMissedCall)
{
    CsrUint8 *pBuf = *ppBuf;

    *pBuf = CSR_BT_OBEX_PB_NEW_MISSED_CALLS_ID; /* tag */
    pBuf++;
    *pBuf = 0x01; /* length */
    pBuf++;
    pBuf[0] = nNewMissedCall; /* value */
    pBuf++;
    (*pAppTotalLen) += CSR_BT_OBEX_APP_PAR_NEWMISSEDCALL_SIZE; /* update total length */
    *ppBuf = pBuf;
}

static CsrBool csrBtPasCheckTargetHeader(CsrUint8   *target, 
                                         CsrUint16  targetHeaderLength)
{
    if (target && targetHeaderLength == CSR_BT_PAS_UUID_STR_LEN)
    {
        if (!CsrMemCmp(target, pasUuidString, CSR_BT_PAS_UUID_STR_LEN))
        {
            return (TRUE);
        }
    }
    return (FALSE);       
}

static CsrBool isVirtualFolder(PasFolder *pFolder, PasFolderId id)
{
    return (CsrBool)(id == pFolder->id ? TRUE : FALSE);
}

static signed int ObexNICmp(const CsrUint8 *str1, const CsrUint8 *str2, CsrSize count)
{
    CsrSize n = 0;
    signed int iResult = 0;

    for ( ; n < count; n += 2)
    {
        if (0 == str1[n] && 0 == str2[n])
        {
            if (CSR_TOUPPER(str1[n+1]) != CSR_TOUPPER(str2[n+1]))
            {
                if (CSR_TOUPPER(str1[n+1]) > CSR_TOUPPER(str2[n+1]))
                {
                    iResult = 1;
                }
                else
                {
                    iResult = -1;
                }
                break;
            }
        }
        else if (str1[n] != str2[n])
        {
            if (str1[n] > str2[n])
            {
                iResult = 1;
            }
            else
            {
                iResult = -1;
            }
            break;
        }
        else if (str1[n+1] != str2[n+1])
        {
            if (str1[n+1] > str2[n+1])
            {
                iResult = 1;
            }
            else
            {
                iResult = -1;
            }
            break;
        }
        if (0 == str1[n] && 0 == str1[n+1])
        {
            break;
        }
    }
    return iResult;
}

static void setBackVirtualFolder(PasFolder **ppFolder)
{
    if ((*ppFolder)->pParent)
    {
        *ppFolder = (*ppFolder)->pParent;
    }
}

static void setRootVirtualFolder(PasFolder **ppFolder)
{
    while ((*ppFolder)->pParent)
    {
        *ppFolder = (*ppFolder)->pParent;
    }
}

static void setVirtualFolder(PasFolder **ppFolder, CsrUint8 *wzName)
{
    PasFolder *p = (*ppFolder)->pChild;

    while (p)
    {
        if (0 == ObexNICmp((CsrUint8*)p->pwzName, wzName, CsrBtObexWcsLen(wzName)))
        {
            *ppFolder = p;
            break;
        }
        p = p->pNext;
    }
}

static CsrBool pbGetObexFilter(CsrUint8   *obexPacket, 
                               CsrUint16  appParametersLength, 
                               CsrUint8   *pFilter)
{
    CsrUint16 i = 0;

    while (i < appParametersLength)
    {
        if (obexPacket[i] == CSR_BT_OBEX_PB_FILTER_ID)
        {
            pFilter[7] = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, (i + CSR_BT_AUTH_TAG_LENGTH_SCHEME));
            pFilter[6] = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, (i + CSR_BT_AUTH_TAG_LENGTH_SCHEME + 1));
            pFilter[5] = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, (i + CSR_BT_AUTH_TAG_LENGTH_SCHEME + 2));
            pFilter[4] = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, (i + CSR_BT_AUTH_TAG_LENGTH_SCHEME + 3));
            pFilter[3] = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, (i + CSR_BT_AUTH_TAG_LENGTH_SCHEME + 4));
            pFilter[2] = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, (i + CSR_BT_AUTH_TAG_LENGTH_SCHEME + 5));
            pFilter[1] = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, (i + CSR_BT_AUTH_TAG_LENGTH_SCHEME + 6));
            pFilter[0] = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, (i + CSR_BT_AUTH_TAG_LENGTH_SCHEME + 7));
            return TRUE;
        }
        i += (CsrUint16)obexPacket[i + 1] + CSR_BT_AUTH_TAG_LENGTH_SCHEME;
    }
    return FALSE;
}

static CsrBool PbGetObexFormat(CsrUint8       *obexPacket, 
                               CsrUint16      appParametersLength, 
                               CsrBtPasFormat *pFormat)
{
    CsrUint16 i = 0;

    while (i < appParametersLength)
    {
        if (obexPacket[i] == CSR_BT_OBEX_PB_FORMAT_ID)
        {
            *pFormat = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, (i + CSR_BT_AUTH_TAG_LENGTH_SCHEME));
            return TRUE;
        }
        i += (CsrUint16)obexPacket[i + 1] + CSR_BT_AUTH_TAG_LENGTH_SCHEME;
    }
    return FALSE;
}

static CsrBool pbGetObexMaxListCnt(CsrUint8       *obexPacket, 
                                   CsrUint16      appParametersLength, 
                                   CsrUint16      *pnMaxListCnt)
{
    CsrUint16 i = 0;

    while (i < appParametersLength)
    {
        if (obexPacket[i] == CSR_BT_OBEX_PB_MAX_LIST_CNT_ID)
        {
            *pnMaxListCnt = CSR_BT_OBEX_GET_WORD_AT(obexPacket, (i + CSR_BT_AUTH_TAG_LENGTH_SCHEME));
            return TRUE;
        }
        i += (CsrUint16)obexPacket[i + 1] + CSR_BT_AUTH_TAG_LENGTH_SCHEME;
    }
    return FALSE;
}

static CsrBool pbGetObexListOffset(CsrUint8     *obexPacket, 
                                   CsrUint16    appParametersLength, 
                                   CsrUint16    *pnListStartOffset)
{
    CsrUint16 i = 0;

    while (i < appParametersLength)
    {
        if (obexPacket[i] == CSR_BT_OBEX_PB_LIST_START_OFFSET_ID)
        {
            *pnListStartOffset = CSR_BT_OBEX_GET_WORD_AT(obexPacket, (i + CSR_BT_AUTH_TAG_LENGTH_SCHEME));
            return TRUE;
        }
        i += (CsrUint16)obexPacket[i + 1] + CSR_BT_AUTH_TAG_LENGTH_SCHEME;
    }
    return FALSE;
}

static CsrBool PbGetObexOrder(CsrUint8      *obexPacket, 
                              CsrUint16     appParametersLength,  
                              CsrBtPasOrder *pOrder)
{
    CsrUint16 i = 0;

    while (i < appParametersLength)
    {
        if (obexPacket[i] == CSR_BT_OBEX_PB_ORDER_ID)
        {
            *pOrder = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, (i + CSR_BT_AUTH_TAG_LENGTH_SCHEME));
            return TRUE;
        }
        i += (CsrUint16)obexPacket[i + 1] + CSR_BT_AUTH_TAG_LENGTH_SCHEME;
    }
    return FALSE;
}

static CsrBool pbGetObexSearchVal(CsrUint8      *obexPacket, 
                                  CsrUint16     appParametersLength, 
                                  CsrUint16     *pUtf8SearchValOffset, 
                                  CsrUint16     *pnSearchValLen)
{
    CsrUint16 i = 0;

    while (i < appParametersLength)
    {
        if (obexPacket[i] == CSR_BT_OBEX_PB_SEARCH_VAL_ID)
        {
            *pnSearchValLen       = (CsrUint16)(obexPacket[i + 1]);
            *pUtf8SearchValOffset = (CsrUint16)(i + 2);
            return TRUE;
        }
        i += (CsrUint16)obexPacket[i + 1] + CSR_BT_AUTH_TAG_LENGTH_SCHEME;
    }
    return FALSE;
}

static CsrBool PbGetObexSearchAtt(CsrUint8          *obexPacket, 
                                  CsrUint16         appParametersLength,  
                                  CsrBtPasSearchAtt *pSearchAtt)
{
    CsrUint16 i = 0;

    while (i < appParametersLength)
    {
        if (obexPacket[i] == CSR_BT_OBEX_PB_SEARCH_ATT_ID)
        {
            *pSearchAtt = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, (i + CSR_BT_AUTH_TAG_LENGTH_SCHEME));
            return TRUE;
        }
        i += (CsrUint16)obexPacket[i + 1] + CSR_BT_AUTH_TAG_LENGTH_SCHEME;
    }
    return FALSE;
}

static CsrBool getSrcFromName(CsrBtPasSrc *pSrc, CsrUint8 *szPath, CsrUint16 *pnName)
{
    CsrSize nLen = CsrBtObexWcsLen(szPath);
    CsrInt32 iTelecom = CsrBtObexRevIWcsNMatch(szPath, szTelecom, nLen);

    if (iTelecom >= 0)
    {   /* we found a telecom path */
        CsrInt32 iSim1 = CsrBtObexRevIWcsNMatch(szPath, szSim1, (iTelecom + 2) - sizeof(szTelecom));

        if (iSim1 >= 0)
        {   /* found sim1 path */
            *pSrc = CSR_BT_PAS_SRC_SIM;
        }
        else
        {
            *pSrc = CSR_BT_PAS_SRC_PHONE;
        }
        CsrBtObexWcsCpy(szPath, szPath + iTelecom + 2);
        *pnName = (CsrUint16)(*pnName + iTelecom + 2);
        return TRUE;
    }
    *pSrc = CSR_BT_PAS_SRC_PHONE;
    return FALSE;
}

static CsrUint16 csrBtPasGetPullPbIndHandler(PasInst    *pInst, 
                                             CsrUint8 *obexPacket,
                                             CsrUint16  obexPacketLength,
                                             CsrUint16  nameOffset,
                                             CsrUint16  appParametersOffset,
                                             CsrUint16  appParametersLength,
                                             CsrBool    *freeObexPacket)
{   /* first indication, then parse header info */
    CsrUint8        filterTmp[8];
    CsrUint16       nStartTmp;
    CsrBtPasFormat  nFormatTmp;

    CsrUint8        *pwzName        = obexPacket + nameOffset;
    CsrUint8        filter[8]       = {0, 0, 0, 0, 0, 0, 0, 0}; 
    CsrBtPasFormat  format          = CSR_BT_PAS_FORMAT_VCARD2_1;
    CsrBtPasSrc     src             = CSR_BT_PAS_SRC_PHONE;
    CsrUint16       nMaxListCnt     = 0xffff;
    CsrUint16       nMaxListCntTmp  = 0;
    CsrUint16       nStartOffset    = 0;   

    /* parse app. parameter */
    if (pbGetObexFilter(&obexPacket[appParametersOffset], appParametersLength, filterTmp))
    {
        CsrMemCpy(filter, filterTmp, sizeof(filter));
    }
    if (PbGetObexFormat(&obexPacket[appParametersOffset], appParametersLength, &nFormatTmp))
    {
        format = nFormatTmp;
    }
    if (pbGetObexMaxListCnt(&obexPacket[appParametersOffset], appParametersLength, &nMaxListCntTmp))
    {
        nMaxListCnt = nMaxListCntTmp;
    }
    if (pbGetObexListOffset(&obexPacket[appParametersOffset], appParametersLength, &nStartTmp))
    {
        nStartOffset = nStartTmp;
    }
    /* find cmd type */
    if (getSrcFromName(&src, pwzName, &nameOffset) )
    {
        /* name == "mch.vcf" */
        if (0 == ObexNICmp(pwzName, wzMch, 8))
        {
            if (0 == nMaxListCnt)
            {
                csrBtPasPullMchSizeIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh, src);
            }
            else
            {
                csrBtPasPullMchIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh, 
                                       src, filter, format, nMaxListCnt, nStartOffset);
            }
        }
        else
        {
            /* MaxListCount == 0 => get pb size */
            if (0 == nMaxListCnt)
            {
                csrBtPasPullPbSizeIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh, 
                                          nameOffset, src, obexPacket, obexPacketLength);
            }
            else
            {
                csrBtPasPullPbIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh, 
                                      nameOffset, src, filter, format, nMaxListCnt, 
                                      nStartOffset, obexPacket, obexPacketLength);
            }
            *freeObexPacket = FALSE;
        }
        return (nameOffset);
    }
    else
    {   /* error send obex error */
        return (CSR_BT_PAS_NO_HEADER);
    }
}

static void csrBtPasGetPullvCardListIndHandler(PasInst    *pInst, 
                                               CsrUint8 *obexPacket,
                                               CsrUint16  obexPacketLength,
                                               CsrUint16  nameOffset,
                                               CsrUint16  appParametersOffset,
                                               CsrUint16  appParametersLength,
                                               CsrBool    *freeObexPacket)
{
    CsrBtPasOrder       orderTmp;
    CsrUint16           nSearchValTmp;
    CsrBtPasSearchAtt   searchAttTmp;
    CsrUint16           nMaxListCntTmp;
    CsrUint16           nListStartOffsetTmp;

    CsrUint8            *pwzName         = obexPacket + nameOffset;
    CsrBtPasOrder       order            = CSR_BT_PAS_ORDER_INDEXED;
    CsrUint16           nSearchVal       = 0;
    CsrUint16           nSearchValLen    = 0;
    CsrBtPasSearchAtt   searchAtt        = CSR_BT_PAS_SEARCH_ATT_NAME;
    CsrUint16           nMaxListCnt      = 0;
    CsrUint16           nListStartOffset = 0;
        
    /* parse app. parameter */
    if (PbGetObexOrder(&obexPacket[appParametersOffset], appParametersLength, &orderTmp))
    {
        order = orderTmp;
    }
    if (pbGetObexSearchVal(&obexPacket[appParametersOffset], appParametersLength, &nSearchValTmp, &nSearchValLen))
    {
        nSearchVal = nSearchValTmp + appParametersOffset;
    }
    if (PbGetObexSearchAtt(&obexPacket[appParametersOffset], appParametersLength, &searchAttTmp))
    {
        searchAtt = searchAttTmp;
    }
    if (pbGetObexMaxListCnt(&obexPacket[appParametersOffset], appParametersLength, &nMaxListCntTmp))
    {
        nMaxListCnt = nMaxListCntTmp;
    }
    if (pbGetObexListOffset(&obexPacket[appParametersOffset], appParametersLength, &nListStartOffsetTmp))
    {
        nListStartOffset = nListStartOffsetTmp;
    }
    /* Special handling of mch */
    if (isVirtualFolder(pInst->pFolder, PAS_MCH_ID) || (NULL != pwzName && 0 == ObexNICmp(pwzName, wzMchFolder, 4)))
    {
        if (0 == nMaxListCnt)
        {
            csrBtPasPullvCardMchListSizeIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh);
        }
        else
        {
            csrBtPasPullvCardMchListIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh, 
                                            order, nSearchVal, nSearchValLen, searchAtt,
                                            nMaxListCnt, nListStartOffset, obexPacket, obexPacketLength);
            *freeObexPacket = FALSE;
        }
    }
    else
    {
        if (0 == nMaxListCnt)
        {   /* PCE request the size of the PB */
            csrBtPasPullvCardListSizeIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh, 
                                             nameOffset, obexPacket, obexPacketLength);
        }
        else
        {
            csrBtPasPullvCardListIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh, 
                                         nameOffset, order, nSearchVal, nSearchValLen, searchAtt,
                                         nMaxListCnt, nListStartOffset, obexPacket, obexPacketLength);
        }
        *freeObexPacket = FALSE;
    }
}

static void csrBtPasGetPullvCardEntryIndHandler(PasInst    *pInst, 
                                                CsrUint8 *obexPacket,
                                                CsrUint16  obexPacketLength,
                                                CsrUint16  nameOffset,
                                                CsrUint16  appParametersOffset,
                                                CsrUint16  appParametersLength,
                                                CsrBool    *freeObexPacket)
{   /* first indication, then parse header info */
    CsrUint8        filter[8] = {0,0,0,0,0,0,0,0};
    CsrBtPasFormat  format    = CSR_BT_PAS_FORMAT_VCARD2_1;

    /* parse app. parameter */
    pbGetObexFilter(&obexPacket[appParametersOffset], appParametersLength, filter);
    PbGetObexFormat(&obexPacket[appParametersOffset], appParametersLength, &format);
    csrBtPasPullvCardEntryIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh, 
                                  nameOffset, filter, format, obexPacket, obexPacketLength);
    *freeObexPacket = FALSE;
}


static CsrUint8 csrBtPasConnectResponseHandler(PasInst                      *pInst,
                                               CsrBtObexResponseCode        responseCode, 
                                               ObexUtilDigestChallengeType  *digestChallenge,
                                               CsrUcs2String                *description)
{
    CsrUint8  numOfHeaders           = 0;
    CsrUint8  headerPriorityTable[1] = {CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER};
    CsrUint16 whoHeaderLength        = CSR_BT_PAS_UUID_STR_LEN;
    CsrUint8  *who                   = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
    responseCode                     = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);
    
    if (description)
    {
        numOfHeaders = 1;
    }
    CsrMemCpy(who, pasUuidString, whoHeaderLength); 
    
    return (ObexUtilServConnectResponse(pInst->obexInst, 
                                        responseCode, 
                                        whoHeaderLength, 
                                        &who, 
                                        (CsrUint32) pInst->localServerCh, 
                                        numOfHeaders, 
                                        headerPriorityTable, 
                                        &digestChallenge, 
                                        &description, 
                                        0, 
                                        NULL,
                                        FALSE));
}

static CsrUint8 csrBtPasSetpathResponseHandler(PasInst                      *pInst,
                                               CsrBtObexResponseCode        responseCode, 
                                               ObexUtilDigestChallengeType  *digestChallenge)
{
    CsrUint8  headerPriorityTable[3] = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    responseCode = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);
    return(ObexUtilServSetpathResponse(pInst->obexInst, responseCode, &digestChallenge, headerPriorityTable, NULL, NULL, 0, NULL));
}

static CsrUint8 csrBtPasGetWithHeadersResponseHandler(PasInst                     *pInst,
                                                      CsrUint8                    firstHeader,
                                                      CsrUint8                    bodyHeaderPriorityIndex,
                                                      ObexUtilDigestChallengeType *digestChallenge,
                                                      CsrBtObexResponseCode       responseCode, 
                                                      CsrUint16                   appParametersHeaderLength,
                                                      CsrUint8                    *appParametersHeader,
                                                      CsrUint16                   bodyLength,
                                                      CsrUint8                    *body)
{
    CsrUint8 headerPriorityTable[6];
    
    CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);
    headerPriorityTable[0] = firstHeader;

    return (ObexUtilServGetWithHeaderResponse(pInst->obexInst, 
                                              responseCode,
                                              &digestChallenge, 
                                              headerPriorityTable, 
                                              bodyHeaderPriorityIndex, 
                                              0,
                                              NULL, 
                                              NULL, 
                                              appParametersHeaderLength, 
                                              &appParametersHeader, 
                                              0, 
                                              NULL, 
                                              &body, 
                                              bodyLength,
                                              CSR_BT_OBEX_SRMP_INVALID)); 
}

static CsrUint8 csrBtPasMchResHandler(PasInst               *pInst, 
                                      CsrBtObexResponseCode responseCode,
                                      CsrUint8              newMissedCall)
{
    CsrUint8  *appHeader        = NULL;
    CsrUint16 appHeaderLength   = 0;
    CsrUint8  firstHeader       = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER;

    if ((responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE ||
        (responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        responseCode = (CSR_BT_OBEX_CONTINUE_RESPONSE_CODE | CSR_BT_OBEX_FINAL_BIT_MASK);

        if (newMissedCall)
        {
            CsrUint8  *pnTotalAppLen;
            CsrUint8  *tmpBuf;
        
            firstHeader     = CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER;
            appHeaderLength = PbRetAppParaHeaderLen(CSR_BT_PB_OBEX_NEWMISSEDCALL_FLAG);
            appHeader       = (CsrUint8 *) CsrPmemAlloc(appHeaderLength);
            tmpBuf          = appHeader;

            PbBuildAppHeader(&tmpBuf, &pnTotalAppLen);
            pbBuildMissedCallHeader(&tmpBuf, pnTotalAppLen, newMissedCall);
        }
    }
    
    return (csrBtPasGetWithHeadersResponseHandler(pInst,
                                                  firstHeader,
                                                  CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                  NULL,
                                                  responseCode, 
                                                  appHeaderLength,
                                                  appHeader,
                                                  0,
                                                  NULL));
}

static CsrUint8 csrBtPasPullSizeResHandler(PasInst               *pInst, 
                                           CsrBtObexResponseCode responseCode,
                                           CsrUint16             pbSize)
{
    CsrUint8  *appHeader        = NULL;
    CsrUint16 appHeaderLength   = 0;
    CsrUint8  firstHeader       = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER;

    if ((responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        CsrUint8  *pnTotalAppLen;
        CsrUint8  *tmpBuf;
        
        firstHeader     = CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER;
        appHeaderLength = PbRetAppParaHeaderLen(CSR_BT_PB_OBEX_PBSIZE_FLAG);
        appHeader       = (CsrUint8 *) CsrPmemAlloc(appHeaderLength);
        tmpBuf          = appHeader;
        PbBuildAppHeader(&tmpBuf, &pnTotalAppLen);
        pbBuildPhonebookSizeHeader(&tmpBuf, pnTotalAppLen, pbSize);
        
    }

    return (csrBtPasGetWithHeadersResponseHandler(pInst,
                                                  firstHeader,
                                                  CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                  NULL,
                                                  responseCode, 
                                                  appHeaderLength,
                                                  appHeader,
                                                  0,
                                                  NULL));
}

static CsrUint8 csrBtPasMchPullSizeResHandler(PasInst               *pInst, 
                                              CsrBtObexResponseCode responseCode,
                                              CsrUint16             pbSize,
                                              CsrUint8              newMissedCall)
{
    CsrUint8  *appHeader         = NULL;
    CsrUint16 appHeaderLength    = 0;
    CsrUint8  firstHeader        = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER;

    if ((responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        CsrUint8  *pnTotalAppLen;
        CsrUint8  *tmpBuf;
        
        firstHeader = CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER;
    
        if (newMissedCall)
        {
            appHeaderLength = PbRetAppParaHeaderLen(CSR_BT_PB_OBEX_PBSIZE_FLAG | CSR_BT_PB_OBEX_NEWMISSEDCALL_FLAG);
        }
        else
        {
            appHeaderLength = PbRetAppParaHeaderLen(CSR_BT_PB_OBEX_PBSIZE_FLAG);
        }
        appHeader = (CsrUint8 *) CsrPmemAlloc(appHeaderLength);
        tmpBuf    = appHeader;

        PbBuildAppHeader(&tmpBuf, &pnTotalAppLen);
        pbBuildPhonebookSizeHeader(&tmpBuf, pnTotalAppLen, pbSize);
        
        if (newMissedCall)
        {
            pbBuildMissedCallHeader(&tmpBuf, pnTotalAppLen, newMissedCall);
        }
    }

    return (csrBtPasGetWithHeadersResponseHandler(pInst,
                                                  firstHeader,
                                                  CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                  NULL,
                                                  responseCode, 
                                                  appHeaderLength,
                                                  appHeader,
                                                  0,
                                                  NULL));
}

static void csrBtPasConnectIndHandler(void            *instData,
                                      CsrBtDeviceAddr deviceAddr,
                                      CsrBtConnId     cid,
                                      CsrUint16       maxPeerObexPacketLength,
                                      CsrUint16       obexResponsePacketLength,
                                      CsrUint32       length,
                                      CsrUint32       count,
                                      CsrUint16       targetHeaderLength,
                                      CsrUint8        *target,
                                      CsrBool         authenticated,
                                      CsrBool         challenged,
                                      CsrUint16       obexPacketLength,
                                      CsrUint8      *obexPacket)
{
    PasInst * pInst     = (PasInst *) instData;
    pInst->obexCommand  = CSR_BT_OBEX_CONNECT_OPCODE;

    if (csrBtPasCheckTargetHeader(target, targetHeaderLength))
    {   
        if (authenticated)
        { 
            csrBtPasAuthCfmSend(pInst->appHandle);
        }
                
        csrBtPasConnectIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh,
                               maxPeerObexPacketLength, &deviceAddr, length, count,cid);
    }
    else
    {   
        CsrUcs2String *description = CsrUtf82Ucs2ByteString((CsrUint8 *) "Target missing");
        csrBtPasConnectResponseHandler(pInst, CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE, NULL,
                                       description);
    }   
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(challenged);
    CsrPmemFree(obexPacket);
    CsrPmemFree(target);
}

static void csrBtPasGetContinueHandler(void         *instData,
                                       CsrUint16    obexResponsePacketLength,
                                       CsrBool      authenticated,
                                       CsrBool      finalBitSet,
                                       CsrUint16    obexPacketLength,
                                       CsrUint8   *obexPacket)
{
    PasInst * pInst         = (PasInst *) instData;
    CsrBtPasNextInd *pMsg   = (CsrBtPasNextInd *) CsrPmemAlloc(sizeof(CsrBtPasNextInd));
    pMsg->type              = CSR_BT_PAS_NEXT_IND;
    pMsg->connectionId      = (CsrUint32) pInst->localServerCh;
    csrBtPasMessagePut(pInst->appHandle, pMsg);

    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtPasGetIndHandler(void          *instData,
                                  CsrUint16     obexResponsePacketLength,
                                  CsrBool       authenticated,
                                  CsrBool       finalBitSet,
                                  CsrUint16     obexPacketLength,
                                  CsrUint8    *obexPacket)
{ /* The first Get Request packet is received                                   */
    CsrUint16 typeLength, nameLength;
    PasInst * pInst                 = (PasInst *) instData;    
    CsrUint16 nameOffset            = CSR_BT_PAS_NO_HEADER;
    CsrUint16 typeOffset            = CSR_BT_PAS_NO_HEADER;
    CsrUint16 appParametersOffset   = CSR_BT_PAS_NO_HEADER;
    CsrUint16 appParametersLength   = 0;
    CsrUint16 badRequest            = CSR_BT_PAS_HEADER_OK;
    CsrBool   freeObexPacket        = TRUE;
    CsrBool   emptyNameHeader       = FALSE;
    CsrBool   nameHeader            = FALSE;
    
    pInst->obexCommand              = CSR_BT_OBEX_GET_OPCODE;


    if (authenticated)
    { 
        csrBtPasAuthCfmSend(pInst->appHandle);
    }

    
    /* Look for the name header                                                 */
    if (CsrBtObexNameHeaderIndex(CSR_BT_OBEX_REQUEST, obexPacket, &nameLength))
    {
        nameHeader = TRUE;

        /* Name header is present try to see if there is any content */
        if (!CsrBtObexGetNameOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &nameOffset))
        {
            emptyNameHeader = TRUE;
        }
    }

    /* Look for the App parameters header                                       */
    if (CsrBtObexGetAppParametersOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, 
                                        obexPacketLength, &appParametersOffset, &appParametersLength))
    { /* A App parameters header is found                                       */
        ;
    }
    else
    { /* Did not find the App parameters header                                 */ 
        appParametersOffset = CSR_BT_PAS_NO_HEADER;
        appParametersLength = 0;
    }
    
    if (nameHeader && CsrBtObexGetTypeOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &typeOffset, &typeLength))
    { 
        /* A name and a type header is found                                      */
        if (!emptyNameHeader && obexPacketLength >= (typeOffset + sizeof(CSR_BT_PAS_TEXTTYPE_PB_PHONEBOOK)) &&
            (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_PAS_TEXTTYPE_PB_PHONEBOOK, sizeof(CSR_BT_PAS_TEXTTYPE_PB_PHONEBOOK))))
        {
            badRequest = csrBtPasGetPullPbIndHandler(pInst, obexPacket, obexPacketLength, nameOffset, 
                                                     appParametersOffset, appParametersLength, &freeObexPacket);
        }
        else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_PAS_TEXTTYPE_PB_VCARD_LISTING)) &&
            (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_PAS_TEXTTYPE_PB_VCARD_LISTING, sizeof(CSR_BT_PAS_TEXTTYPE_PB_VCARD_LISTING))))
        {
            csrBtPasGetPullvCardListIndHandler(pInst, obexPacket, obexPacketLength, nameOffset, 
                                               appParametersOffset, appParametersLength, &freeObexPacket);


        }
        else if (!emptyNameHeader && obexPacketLength >= (typeOffset + sizeof(CSR_BT_PAS_TEXTTYPE_PB_VCARD)) &&
                (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_PAS_TEXTTYPE_PB_VCARD, sizeof(CSR_BT_PAS_TEXTTYPE_PB_VCARD))))
        {
            csrBtPasGetPullvCardEntryIndHandler(pInst, obexPacket, obexPacketLength, nameOffset, 
                                                appParametersOffset, appParametersLength, &freeObexPacket);
        }
        else 
        {
            badRequest = CSR_BT_PAS_NO_HEADER; 
        }
    }

    if (badRequest == CSR_BT_PAS_NO_HEADER)
    { /* PAS expect that all the headers are received in the first OBEX packet
         and a type and a name header always is present                         */
        csrBtPasGetWithHeadersResponseHandler(pInst,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                              NULL,
                                              CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE, 
                                              0,
                                              NULL,
                                              0,
                                              NULL);
    }
    else
    {
        ObexUtilServSetGetContinueFunc(pInst->obexInst, csrBtPasGetContinueHandler);
    }

    if (freeObexPacket)
    {
        CsrPmemFree(obexPacket);
    }
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexResponsePacketLength);
}

static void csrBtPasSetpathIndHandler(void          *instData,
                                      CsrUint16     obexResponsePacketLength,
                                      CsrUint8      flags,
                                      CsrUint8      constants,
                                      CsrBool       authenticated,
                                      CsrUint16     obexPacketLength,
                                      CsrUint8    *obexPacket)
{
    CsrUint16  nameOffset;
    
    CsrUint8   *namePtr = NULL;
    PasInst    *pInst   = (PasInst *) instData;
    pInst->obexCommand  = CSR_BT_OBEX_SET_PATH_OPCODE;

    if (authenticated)
    { 
        csrBtPasAuthCfmSend(pInst->appHandle);
    }

    if (CsrBtObexGetNameOffset(CSR_BT_OBEX_REQUEST, obexPacket, &nameOffset))
    {
        namePtr = obexPacket + nameOffset;
        if (namePtr[0] == 0 && namePtr[1] == 0)
        {
            namePtr = NULL;
        }
    }

    if ((0x1 & flags))
    {
        setBackVirtualFolder(&pInst->pFolder);
        csrBtPasSetBackFolderIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh);
    }
    else if (namePtr == NULL)
    {
        setRootVirtualFolder(&pInst->pFolder);
        csrBtPasSetRootFolderIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh);
    }
    else
    {   /* make it word alignment safe */
        setVirtualFolder(&pInst->pFolder, namePtr);
        csrBtPasSetFolderIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh, 
                                                    nameOffset, obexPacket, obexPacketLength);
        obexPacket = NULL;
    }
    CSR_UNUSED(constants);
    CSR_UNUSED(obexResponsePacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtPasAbortIndHandler(void              *instData,
                                    CsrUint16         descriptionOffset,
                                    CsrUint16         descriptionLength,
                                    CsrUint16         obexPacketLength,
                                    CsrUint8        *obexPacket)
{
    PasInst * pInst         = (PasInst *) instData;
    CsrBtPasAbortInd *pMsg  = CsrPmemAlloc(sizeof(CsrBtPasAbortInd));
    pMsg->type              = CSR_BT_PAS_ABORT_IND;
    pMsg->connectionId      = (CsrUint32) pInst->localServerCh;
    pMsg->descriptionOffset = descriptionOffset;
    pMsg->descriptionLength = descriptionLength;
    pInst->obexCommand      = CSR_BT_OBEX_ABORT_OPCODE;
    if (descriptionOffset == 0)
    { /* An OBEX Descriptor header is not included, CsrPmemFree obexPacket         */
        CsrPmemFree(obexPacket);
        obexPacket          = NULL;
        obexPacketLength    = 0;     
    }
    pMsg->payload           = obexPacket;
    pMsg->payloadLength     = obexPacketLength;
    csrBtPasMessagePut(pInst->appHandle, pMsg);
}

static void csrBtPasAuthenticateIndHandler(void            *instData,
                                           CsrBtDeviceAddr deviceAddr,
                                           CsrUint8        options,
                                           CsrUint16       realmLength,
                                           CsrUint8      *realm)
{
    PasInst * pInst               = (PasInst *) instData;
    CsrBtPasAuthenticateInd *pMsg = CsrPmemAlloc(sizeof(CsrBtPasAuthenticateInd));
    pMsg->type                    = CSR_BT_PAS_AUTHENTICATE_IND;
    pMsg->deviceAddr              = deviceAddr;
    pMsg->options                 = options;
    pMsg->realmLength             = realmLength;
    pMsg->realm                   = realm;
    csrBtPasMessagePut(pInst->appHandle, pMsg);
}

static void csrBtPasDisconnectIndHandler(void            *instData,
                                         CsrBtDeviceAddr deviceAddr,
                                         CsrBtReasonCode reasonCode,
                                         CsrBtSupplier   reasonSupplier,
                                         CsrUint16       obexPacketLength,
                                         CsrUint8      *obexPacket)
{
    PasInst * pInst             = (PasInst *) instData;
    CsrBtPasDisconnectInd *pMsg = CsrPmemAlloc(sizeof(CsrBtPasDisconnectInd));
    pMsg->type                  = CSR_BT_PAS_DISCONNECT_IND;
    pMsg->connectionId          = (CsrUint32) pInst->localServerCh;
    pMsg->deviceAddr            = deviceAddr;
    pMsg->reasonCode            = reasonCode;
    pMsg->reasonSupplier        = reasonSupplier;
    pInst->obexCommand          = CSR_BT_OBEX_DISCONNECT_OPCODE;
    csrBtPasMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtPasActivateCfmHandler(void            *instData,
                                       CsrUint8   serverChannel,
                                       psm_t           psm,
                                       CsrBtResultCode resultCode,
                                       CsrBtSupplier   resultSupplier)
{
    PasInst * pInst = (PasInst *) instData;

    pInst->localServerCh = serverChannel;
    
    if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS && 
        resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The PAS is activate. Note the profile is allow to called this 
         function from the callback function                                    */
        ObexUtilServConnectableStart(pInst->obexInst, 
                                     CSR_BT_PAS_PROFILE_DEFAULT_MTU_SIZE,
                                     pInst->maxObexPacketSize);
    }
    else
    { /* The PAS cannot send this information to the application
         as it has no API for it. Only thing to do is to activate it again      */
        csrBtPasActivateStartHandler(pInst);
    }
    CSR_UNUSED(psm);
}

static CsrUint8 csrBtPasActivateStartHandler(PasInst *pInst)
{
    CsrUint16 recordLength    = (CsrUint16) (sizeof(serviceRecord));
    CsrUint8  *record         = (CsrUint8 *)CsrPmemAlloc(recordLength);
    CsrMemCpy(record, serviceRecord, recordLength);

    /* insert supported feature */
    record[CSR_BT_PAS_RFCOMM_FEATURES_SERVICE_RECORD_INDEX] = pInst->nSdpFeature;
    /* insert supported repositories */
    record[CSR_BT_PAS_RFCOMM_REPOSITORIES_SERVICE_RECORD_INDEX] = pInst->nSdpRepository;
    
    return (ObexUtilServActivateStart(pInst->obexInst,
                                      pInst->secIncoming,
                                      CSR_BT_OBEX_PBA_SERVER_PROFILE_UUID,
                                      CSR_BT_PAS_LP_SUPERVISION_TIMEOUT,
                                      pInst->localServerCh,
                                      L2CA_PSM_INVALID,
                                      recordLength,
                                      CSR_BT_PAS_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX,
                                      CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                      CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                      &record,
                                      CSR_BT_OBJECT_TRANSFER_COD,
                                      csrBtPasConnectIndHandler,
                                      NULL,
                                      csrBtPasGetIndHandler,
                                      csrBtPasSetpathIndHandler,
                                      csrBtPasAbortIndHandler,
                                      csrBtPasDisconnectIndHandler,
                                      csrBtPasAuthenticateIndHandler,
                                      csrBtPasActivateCfmHandler,
                                      NULL,
                                      pInst->maxObexPacketSize,
                                      pInst->windowSize,
                                      pInst->localSrmEnable));
}

static void csrBtPasDeactivateCfmHandler(void          *instData,
                                         CsrUint8 serverChannel,
                                         psm_t         psm) 
{
    PasInst * pInst             = (PasInst *) instData;
    CsrBtPasDeactivateCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtPasDeactivateCfm));
    pInst->localServerCh        = serverChannel;
    pMsg->type                  = CSR_BT_PAS_DEACTIVATE_CFM;
    csrBtPasMessagePut(pInst->appHandle, pMsg);    
    CSR_UNUSED(psm);
}

CsrUint8 CsrBtPasActivateReqHandler(PasInst *pInst, void *msg)
{
    CsrBtPasActivateReq *pMsg = (CsrBtPasActivateReq *) msg;
    pInst->appHandle          = pMsg->appHandle;
    pInst->nSdpFeature        = pMsg->feature;
    pInst->nSdpRepository     = pMsg->repository;
    pInst->maxObexPacketSize  = pMsg->obexMaxPacketSize;
    pInst->windowSize         = pMsg->windowSize;
    pInst->localSrmEnable     = pMsg->srmEnable;
    return (csrBtPasActivateStartHandler(pInst));
}

CsrUint8 CsrBtPasDeactivateReqHandler(PasInst *pInst, void *msg)
{
    return (ObexUtilServDeactivateStart(pInst->obexInst, 
                                        pInst->localServerCh, 
                                        L2CA_PSM_INVALID,
                                        csrBtPasDeactivateCfmHandler));
}

CsrUint8 CsrBtPasConnectResHandler(PasInst *pInst, void *msg)
{
    CsrBtPasConnectRes *pMsg = (CsrBtPasConnectRes *) msg;
    return (csrBtPasConnectResponseHandler(pInst, pMsg->responseCode, NULL, NULL));
}

CsrUint8 CsrBtPasAuthenticateReqHandler(PasInst *pInst, void *msg) 
{
    CsrBtPasAuthenticateReq *pMsg       = (CsrBtPasAuthenticateReq *) msg;
    ObexUtilDigestChallengeType * chal  = ObexUtilReturnDigestChallengePointer(TRUE, 
                                                                               &pMsg->password, 
                                                                               pMsg->passwordLength,  
                                                                               &pMsg->userId, 
                                                                               &pMsg->realm, 
                                                                               pMsg->realmLength);

    switch (pInst->obexCommand)
    {
        case CSR_BT_OBEX_CONNECT_OPCODE:
        {
            return (csrBtPasConnectResponseHandler(pInst, CSR_BT_OBEX_UNAUTHORIZED_CODE, 
                                                   chal, NULL));
        }
        case CSR_BT_OBEX_GET_OPCODE:
        {
            return (csrBtPasGetWithHeadersResponseHandler(pInst,
                                                          CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                          CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                          chal,
                                                          CSR_BT_OBEX_UNAUTHORIZED_CODE, 
                                                          0,
                                                          NULL,
                                                          0,
                                                          NULL));
        }
        case CSR_BT_OBEX_SET_PATH_OPCODE:
        {
            return(csrBtPasSetpathResponseHandler(pInst, CSR_BT_OBEX_UNAUTHORIZED_CODE, chal));
        }
        default:
        {
            ObexUtilPfreeDigestChallengePointer(&chal);
            return CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
        }
    } 
}

CsrUint8 CsrBtPasAuthenticateResHandler(PasInst *pInst, void *msg)
{
    CsrBtPasAuthenticateRes *pMsg = (CsrBtPasAuthenticateRes *) msg;
    return (ObexUtilServAuthenticateResponse(pInst->obexInst, pMsg->passwordLength, &pMsg->password, &pMsg->userId));
}

CsrUint8 CsrBtPasPullPbResHandler(PasInst *pInst, void *msg)
{
    CsrBtPasPullPbRes *pMsg = (CsrBtPasPullPbRes *) msg;

    if ((pMsg->responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
       pMsg->responseCode = (CSR_BT_OBEX_CONTINUE_RESPONSE_CODE | CSR_BT_OBEX_FINAL_BIT_MASK);
    }
    return (csrBtPasGetWithHeadersResponseHandler(pInst,
                                                  CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                  CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                  NULL,
                                                  pMsg->responseCode, 
                                                  0,
                                                  NULL,
                                                  0,
                                                  NULL));
}

CsrUint8 CsrBtPasPullPbSizeResHandler(PasInst *pInst, void *msg)
{
    CsrBtPasPullPbSizeRes *pMsg = (CsrBtPasPullPbSizeRes *) msg;
    return (csrBtPasPullSizeResHandler(pInst, pMsg->responseCode, pMsg->pbSize));
}

CsrUint8 CsrBtPasPullMchResHandler(PasInst *pInst, void *msg)
{
    CsrBtPasPullMchRes *pMsg    = (CsrBtPasPullMchRes *) msg;
    return(csrBtPasMchResHandler(pInst, pMsg->responseCode, pMsg->newMissedCall));
}

CsrUint8 CsrBtPasPullMchSizeResHandler(PasInst *pInst, void *msg)
{
    CsrBtPasPullMchSizeRes *pMsg = (CsrBtPasPullMchSizeRes *) msg;
    return (csrBtPasMchPullSizeResHandler(pInst, pMsg->responseCode, pMsg->pbSize, pMsg->newMissedCall)); 
}

CsrUint8 CsrBtPasSetFolderResHandler(PasInst *pInst, void *msg)
{
    CsrBtPasSetFolderRes *pMsg = (CsrBtPasSetFolderRes *) msg;
    return(csrBtPasSetpathResponseHandler(pInst, pMsg->responseCode, NULL));
}

CsrUint8 CsrBtPasPullvCardListResHandler(PasInst *pInst, void *msg)
{
    CsrBtPasPullVcardListRes *pMsg = (CsrBtPasPullVcardListRes *) msg;
    
    if ((pMsg->responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
       pMsg->responseCode = (CSR_BT_OBEX_CONTINUE_RESPONSE_CODE | CSR_BT_OBEX_FINAL_BIT_MASK);
    }
    return (csrBtPasGetWithHeadersResponseHandler(pInst,
                                                  CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                  CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                  NULL,
                                                  pMsg->responseCode, 
                                                  0,
                                                  NULL,
                                                  0,
                                                  NULL));
}

CsrUint8 CsrBtPasPullvCardListSizeResHandler(PasInst *pInst, void *msg)
{
    CsrBtPasPullVcardListSizeRes *pMsg = (CsrBtPasPullVcardListSizeRes *) msg;
    return (csrBtPasPullSizeResHandler(pInst, pMsg->responseCode, pMsg->pbSize));
}

CsrUint8 CsrBtPasPullvCardMchListResHandler(PasInst *pInst, void *msg)
{
    CsrBtPasPullVcardMchListRes *pMsg = (CsrBtPasPullVcardMchListRes *) msg;
    return(csrBtPasMchResHandler(pInst, pMsg->responseCode, pMsg->newMissedCall));
}

CsrUint8 CsrBtPasPullvCardMchListSizeResHandler(PasInst *pInst, void *msg)
{
    CsrBtPasPullVcardMchListSizeRes *pMsg = (CsrBtPasPullVcardMchListSizeRes *) msg;
    return (csrBtPasMchPullSizeResHandler(pInst, pMsg->responseCode, pMsg->pbSize, pMsg->newMissedCall)); 
}

CsrUint8 CsrBtPasPullvCardEntryResHandler(PasInst *pInst, void *msg)
{
    CsrBtPasPullVcardEntryRes *pMsg = (CsrBtPasPullVcardEntryRes *)msg;

    if ((pMsg->responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        pMsg->responseCode = (CSR_BT_OBEX_CONTINUE_RESPONSE_CODE | CSR_BT_OBEX_FINAL_BIT_MASK);
    }
    return (csrBtPasGetWithHeadersResponseHandler(pInst,
                                                  CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                  CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                  NULL,
                                                  pMsg->responseCode, 
                                                  0,
                                                  NULL,
                                                  0,
                                                  NULL));
}

CsrUint8 CsrBtPasNextResHandler(PasInst *pInst, void *msg)
{
    CsrBtPasNextRes *pMsg = (CsrBtPasNextRes *) msg;

    if ((pMsg->responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && !pMsg->finalFlag)
    {
        pMsg->responseCode = (CSR_BT_OBEX_CONTINUE_RESPONSE_CODE | CSR_BT_OBEX_FINAL_BIT_MASK);
    }
    
    return (ObexUtilServGetResponse(pInst->obexInst,
                                    pMsg->responseCode,
                                    &pMsg->body,
                                    pMsg->bodyLength,
                                    CSR_BT_OBEX_SRMP_INVALID));
}

CsrUint8 CsrBtPasSecurityInReqHandler(PasInst *pInst, void *msg)
{
    CsrBtPasSecurityInReq *prim = (CsrBtPasSecurityInReq*)msg;

    CsrBtResultCode rval = CsrBtScSetSecInLevel(&pInst->secIncoming, prim->secLevel,
        CSR_BT_PBAP_MANDATORY_SECURITY_INCOMING,
        CSR_BT_PBAP_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);
    ObexUtilServChangeIncomingSecurity(pInst->obexInst, pInst->secIncoming);
    csrBtPasSecurityInCfmSend(prim->appHandle, rval, CSR_BT_SUPPLIER_OBEX_PROFILES);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}


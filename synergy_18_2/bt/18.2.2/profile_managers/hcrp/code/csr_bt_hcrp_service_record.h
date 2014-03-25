#ifndef CSR_BT_HCRP_SERVICE_RECORD_H__
#define CSR_BT_HCRP_SERVICE_RECORD_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

static const CsrUint8 hcrpServiceRecord[] =
{
    /* service class ID list. */
    0x09,0x00,0x01,                /* AttrID, ServiceClassIDList. */
    0x35,0x03,                     /* 3 bytes in total DataElSeq. */
    0x19,0x11,0x26,                /* 2 byte UUID, service class = HCR_Print.       */

    /* protocol descriptor list. */
    0x09,0x00,0x04,                /* AttrId ProtocolDescriptorList. */
    0x35,0x0d,                     /* 13 bytes in total DataElSeq. */

    0x35,0x06,                     /* 6 bytes in DataElSeq. */
    0x19,0x01,0x00,                /* 2 byte UUID, protocol = L2CAP. */
    0x09,0x20,0x01,                /* 2 byte CsrUint16, PSM. */

    0x35,0x03,                     /* 3 bytes in DataElSeq. */
    0x19,0x00,0x12,                /* 2 byte UUID Protocol = Hardcopy Control channel. */

    /* service name. */
    0x09,0x01,0x00,                /* AttrId - service name. */
    0x25,0x1A,                     /* 4 byte string. */
    'H','a','r','d','c','o','p','y',' ','C','a','b','l','e',' ','R','e','p','l','a','c','e','m','e','n','t',

    /* bluetooth profile descriptor list. */
    0x09,0x00,0x09,                /* AttrId, ProfileDescriptorList. */
    0x35,0x08,                     /* 8 bytes in total DataElSeq. */

    0x35,0x06,                     /* 6 bytes in total DataElSeq. */
    0x19,0x11,0x25,                /* 2 byte UUID, Service class = HardcopyCable Replacement. */
    0x09,0x01,0x00,                /* 2 byte CsrUint16, version = 0x0100. */

    /* language base list */
    0x09,0x00,0x06,                /* AttrId LanguageBaseAttributeIDList. */
    0x35,0x09,                     /* 9 bytes in total DataElSeq. */
    0x09,0x65,0x6e,                /* 2 byte CsrUint16, natural language = en (english). */
    0x09,0x00,0x6a,                /* 2 byte CsrUint16, character encoding = UTF-8. */
    0x09,0x01,0x00,                /* 2 byte CsrUint16, base attr id = 0x0100. */

    /* additional protocol descriptor list. */
    0x09,0x00,0x0d,                /* AttrId ProtocolDescriptorList. */
    0x35,0x0f,                     /* 15 bytes in total DataElSeq. */

    0x35,0x0d,                     /* 13 bytes in DataElSeq. */

    0x35,0x06,                     /* 6 bytes in DataElSeq. */
    0x19,0x01,0x00,                /* 2 byte UUID, protocol = L2CAP. */
    0x09,0x20,0x03,                /* 2 byte uint, PSM. */

    0x35,0x03,                     /* 3 bytes in DataElSeq. */
    0x19,0x00,0x14,                /* 1 byte UUID protocol = Hardcopy Data channel. */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,      /* AttrId = BrowseGroupList */
    0x35, 0x03,            /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,      /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

#ifdef __cplusplus
}
#endif

#endif


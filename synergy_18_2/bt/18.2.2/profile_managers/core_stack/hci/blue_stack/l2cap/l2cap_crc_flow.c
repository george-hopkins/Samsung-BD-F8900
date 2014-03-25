/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_crc_flow.c

\brief  CRC-16 algorithm and support functions for L2CAP 1.2 when
        running off-chip. For the on-chip case, the FCS is done in
        hardware.
*/

#include "csr_synergy.h"

#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "csr_bt_common.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_mblk.h"

#if defined(INSTALL_L2CAP_ENHANCED_SUPPORT) && !defined(INSTALL_L2CAP_CRC)

#include "l2cap_common.h"
#include "l2cap_config.h"
#include "l2cap_signal.h"
#include "l2cap_chme.h"
#include "l2cap_con_handle.h"
#include "l2cap_cid.h"
#include "l2cap_cidme.h"
#include "l2cap_interface.h"
#include "l2cap_flow.h"
#include "l2cap_crc_flow.h"
#include "l2caplib.h"
#include "l2cap_cid_fsm_definitions.h"

/*! \brief CRC-16 lookup table.

    CRC-16 is based on the polynomial x^16+x^15+x^2+1, ie. the
    CRC-CCITT. The bits are inserted from least to most significant.

    Table and algorithm was implemented by CSR plc. Other
    implementations can be found on the Internet - most of them
    are public domain (i.e. not copyrighted). For example, see:
    http://www.lammertbies.nl/comm/info/crc-calculation.html
*/
static const CsrUint16 CRC_FLOW_Tab[256] =
{
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

/*! \brief Calculate the CRC16 using the lookup table.

     This is the actual calculation function.
 */
static CsrUint16 CRC_FLOW_Calc(CsrUint16 preload, const CsrUint8 *buf, CsrUint16 length)
{
    CsrUint16 crc16 = preload;
    const CsrUint8 *p, *buf_end = buf + length;

    for (p = buf; p != buf_end; ++p)
        crc16 = CRC_FLOW_Tab[(crc16 ^ *p) & 0xff] ^ (crc16 >> 8);

    return crc16;
}

/*! \brief MBLK map iteration function

    For MBLK chains, we itererate through each MBLK separetely.  This
    function will calculate the FCS for a single MBLK in the chain,
    using the user data as both preload and result storage.
*/
static CsrBool CRC_FLOW_Iterate(const void *data,
                               CsrMblkSize offset,
                               CsrMblkSize size,
                               void *userdata)
{
    CsrUint16 *crc = (CsrUint16*)userdata;
    *crc = CRC_FLOW_Calc(*crc, (const CsrUint8*)data, size);
    return TRUE;
}

/*! \brief Calculate and append the FCS for a TXQE

    Calculate the FCS (16 bit CRC) for the header and MBLK in a TXQE.
    The result will be added as the special "tailer" bytes in the TXQE
    itself
*/
void CRC_FLOW_AddFcs(TXQE_T *txqe)
{
    CsrUint16 crc;

    /* Calcualte CRC16 for the header */
    crc = CRC_FLOW_Calc(0, txqe->header, txqe->headersize);

    /* Calculate it for the MBLK by iterating all MBLKs in the chain
     * using the very clever CsrMblkIterateRegion */
    if(txqe->mblk != NULL)
    {
        CsrMblkIterateRegion(txqe->mblk,                  /* mblk to iterate */
                            0,                           /* offset */
                            CsrMblkGetLength(txqe->mblk), /* size */
                            CRC_FLOW_Iterate,            /* iteration function */
                            &crc);                       /* function user data */
    }

    /* Add FCS to the TXQE tail bytes */
    txqe->trailer[0] = (crc & 0xff);
    txqe->trailer[1] = (crc >> 8);
    txqe->trailersize = 2;
}

/*! \brief Verify FCS of a TXQE

    Check that an incoming PDU has the correct FCS. The entire packet
    must be passed as an MBLK. The basic L2CAP header (cid+length) is
    passed as the header-pointer as the con_handle code has already
    extracted that for us.
*/
CsrBool CRC_FLOW_CheckFcs(CsrMblk *mblk,
                         CsrUint16 *payload_size,
                         CsrUint8 *header)
{
    CsrUint16 crc;
    CsrUint8 fcs[L2CAP_SIZEOF_FCS_FIELD];

    /* Extract FCS tail from packet */
    if(CsrMblkReadTail(&mblk, fcs, L2CAP_SIZEOF_FCS_FIELD) != L2CAP_SIZEOF_FCS_FIELD)
    {
        return FALSE;
    }
    (*payload_size) -= 2;

    /* Calculate FCS for basic L2CAP header */
    crc = CRC_FLOW_Calc(0, header, L2CAP_SIZEOF_CID_HEADER);

    /* Use the CsrMblkIterateRegion to traverse all MBLKs in the
     * chain */
    CsrMblkIterateRegion(mblk,                  /* mblk to iterate */
                        0,                     /* offset */
                        CsrMblkGetLength(mblk), /* size */
                        CRC_FLOW_Iterate,      /* iteration function */
                        &crc);                 /* function user data */

    /* Return status based on CRC match */
    return UINT16_R(fcs, 0) == crc;
}

#endif /* INSTALL_L2CAP_ENHANCED_SUPPORT && !INSTALL_L2CAP_CRC */

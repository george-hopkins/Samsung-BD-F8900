/*!
        (c) CSR plc 2010
        All rights reserved

\file   sdp_control.c

\brief  Library of functions shared by SDC and SDS
        Big endian reading and writing.
        L2CAP Conftab generation
*/

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "sdp_control.h"
#include "bkeyval.h"
#include "csr_bt_core_stack_pmalloc.h"

/*! \brief Read 16bit big endian number from octet array */
CsrUint16 sdp_uint16_r(const CsrUint8 *ptr, const CsrUint16 offset)
{
    return ((CsrUint16)SDP_UINT8_R(ptr, offset) << 8) |
                      SDP_UINT8_R(ptr, offset + 1);
}

/*! \brief Read 32bit big endian number from octet array */
CsrUint32 sdp_uint32_r(const CsrUint8 *ptr, const CsrUint16 offset)
{
    return ((CsrUint32)sdp_uint16_r(ptr, offset) << 16) |
        sdp_uint16_r(ptr, (CsrUint16)(offset + 2));
}

/*! \brief Write 16bit big endian number to octet array */
void sdp_uint16_w(CsrUint8 *ptr, const CsrUint16 offset, const CsrUint16 data)
{
    /* output is big-endian */
    ptr[offset] = (CsrUint8)(data >> 8) & 0xFF;
    ptr[offset+1] = (CsrUint8)(data & 0xFF);
}

/*! \brief Write 32bit big endian number to octet array */
void sdp_uint32_w(CsrUint8 *ptr, const CsrUint16 offset, const CsrUint32 data)
{
    sdp_uint16_w(ptr, offset, (CsrUint16)((data >> 16) & 0xFFFF));
    sdp_uint16_w(ptr, (CsrUint16)(offset + 2), (CsrUint16)(data & 0xFFFF));
}

/*! \brief Generate L2CAP conftab for SDP */
CsrUint16 *sdp_control_conftab(l2ca_mtu_t mtu)
{
    CsrUint16 *conftab = CsrPmemAlloc(SDP_L2CAP_CONFTAB_SIZE * sizeof(CsrUint16));

    conftab[0] = BKV_SEPARATOR;
    conftab[1] = L2CA_AUTOPT_DISABLE_RECONF;
    conftab[2] = L2CA_RECONF_DISABLE_ALL;
    conftab[3] = L2CA_AUTOPT_MTU_IN;
    conftab[4] = mtu;

    return conftab;
}

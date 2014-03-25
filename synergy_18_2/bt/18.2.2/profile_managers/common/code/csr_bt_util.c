/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_util.h"
#include "csr_bt_assert.h"
#include "csr_bt_common.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtBdAddrDup
 *
 *  DESCRIPTION
 *      Duplicate BD Addresses
 *
 *  RETURNS
 *      BD_ADDR_T
 *
 *----------------------------------------------------------------------------*/

#if defined(CSR_CHIP_MANAGER_ENABLE) || defined(CSR_BT_INSTALL_OPTIONAL_UTIL)
BD_ADDR_T *CsrBtBdAddrDup(const BD_ADDR_T *p_bd_addr_1)
{
    BD_ADDR_T *bdaddr = NULL;

    if (p_bd_addr_1)
    {
        bdaddr = (BD_ADDR_T *) CsrPmemAlloc(sizeof(BD_ADDR_T));
        CsrBtBdAddrCopy(bdaddr, p_bd_addr_1);
    }

    return bdaddr;
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtBdAddrZero
 *
 *  DESCRIPTION
 *      Set the bdaddress to all zeroes.
 *
 *  RETURNS
 *      CsrBool - TRUE if equal, FALSE if different
 *
 *----------------------------------------------------------------------------*/
void CsrBtBdAddrZero(BD_ADDR_T *pktBdAddr)
{
    pktBdAddr->lap = 0;
    pktBdAddr->uap = 0;
    pktBdAddr->nap = 0;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtBdAddrEqZero
 *
 *  DESCRIPTION
 *      Compare a bdaddress with zero.
 *
 *  RETURNS
 *      CsrBool - TRUE if equal, FALSE if different
 *
 *----------------------------------------------------------------------------*/
CsrBool CsrBtBdAddrEqZero(const BD_ADDR_T *pktBdAddr)
{
    if ((pktBdAddr->lap == 0) &&
        (pktBdAddr->uap == 0) &&
        (pktBdAddr->nap == 0))
    {
        return TRUE;
    }

    return FALSE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtBdAddrEq
 *
 *  DESCRIPTION
 *      Compare two BD Addresses for equality.
 *
 *  RETURNS
 *      CsrBool - TRUE if equal, FALSE if different
 *
 *----------------------------------------------------------------------------*/

CsrBool CsrBtBdAddrEq(const BD_ADDR_T *p_bd_addr_1, const BD_ADDR_T *p_bd_addr_2 )
{
    if ((p_bd_addr_1->lap == p_bd_addr_2->lap) &&
        (p_bd_addr_1->uap == p_bd_addr_2->uap) &&
        (p_bd_addr_1->nap == p_bd_addr_2->nap))
    {
        return TRUE;
    }

    return FALSE;
}

#ifdef CSR_BT_INSTALL_OPTIONAL_UTIL
/* Duplicates a string excluding the NULL-terminator and updates the 'len' parameter with the length of the string */
char *CsrBtStrDupLen(const char *string, CsrUint32 *len)
{
    char *copy;
    *len = 0;

    copy = NULL;
    if(string != NULL)
    {
        *len = CsrStrLen(string);
        copy = CsrPmemAlloc(*len);
        CsrMemCpy(copy, string, *len);
    }
    return copy;
}
#endif

/* #ifndef EXCLUDE_CSR_BT_RFC_MODULE */
/*************************************************************************************
 CsrBtMapSendingControlSignal
************************************************************************************/
CsrUint8 CsrBtMapSendingControlSignal(CsrUint8 theModemStatus, CsrUint8 theDataRole)
{
    CsrUint8 mapModemSignal = 0;

    if(theDataRole == CSR_BT_DCE)
    {
        if(theModemStatus & CSR_BT_MODEM_CTS_MASK)
        {
            mapModemSignal |= RFC_MSC_RTR_BIT;
        }

        if(theModemStatus & CSR_BT_MODEM_DSR_MASK)
        {
            mapModemSignal |= RFC_MSC_RTC_BIT;
        }

        if(theModemStatus & CSR_BT_MODEM_RI_MASK)
        {
            mapModemSignal |= RFC_MSC_IC_BIT;
        }

        if(theModemStatus & CSR_BT_MODEM_DCD_MASK)
        {
            mapModemSignal |= RFC_MSC_DV_BIT;
        }
    }
    else
    {/* theDataRole must be CSR_BT_DTE */
        if(theModemStatus & CSR_BT_MODEM_RTS_MASK)
        {
            mapModemSignal |= RFC_MSC_RTR_BIT;
        }

        if(theModemStatus & CSR_BT_MODEM_DTR_MASK)
        {
            mapModemSignal |= RFC_MSC_RTC_BIT;
        }

        mapModemSignal &= ~RFC_MSC_IC_BIT;
        mapModemSignal |= RFC_MSC_DV_BIT;
    }
    return mapModemSignal;
}


/*************************************************************************************
 CsrBtMapReceivedControlSignal
************************************************************************************/
CsrUint8 CsrBtMapReceivedControlSignal(CsrUint8 theModemStatus, CsrUint8 theDataRole)
{
    CsrUint8 mapModemSignal = 0;

    if(theDataRole == CSR_BT_DCE)
    {
        if(theModemStatus & RFC_MSC_RTR_BIT)
        {
            mapModemSignal |= CSR_BT_MODEM_RTS_MASK;
        }

        if(theModemStatus & RFC_MSC_RTC_BIT)
        {
            mapModemSignal |= CSR_BT_MODEM_DTR_MASK;
        }
    }
    else
    { /* theDataRole must be CSR_BT_DTE */
        if(theModemStatus & RFC_MSC_RTC_BIT)
        {
            mapModemSignal |= CSR_BT_MODEM_DSR_MASK;
        }

        if(theModemStatus & RFC_MSC_RTR_BIT)
        {
            mapModemSignal |= CSR_BT_MODEM_CTS_MASK;
        }

        if(theModemStatus & RFC_MSC_IC_BIT)
        {
            mapModemSignal |= CSR_BT_MODEM_RI_MASK;
        }

        if(theModemStatus & RFC_MSC_DV_BIT)
        {
            mapModemSignal |= CSR_BT_MODEM_DCD_MASK;
        }
    }
    return mapModemSignal;
}

/*************************************************************************************
 CsrBtPortParDefault
************************************************************************************/
void CsrBtPortParDefault(RFC_PORTNEG_VALUES_T *thePortPar)
{
    thePortPar->baud_rate = CSR_BT_DEFAULT_PORT_SPEED;
    thePortPar->data_bits = CSR_BT_DEFAULT_DATA_BITS;
    thePortPar->stop_bits = CSR_BT_DEFAULT_STOP_BITS;
    thePortPar->parity = CSR_BT_DEFAULT_PARITY;
    thePortPar->parity_type = CSR_BT_DEFAULT_PARITY_TYPE;
    thePortPar->flow_ctrl_mask = CSR_BT_DEFAULT_FLOW_CTRL_MASK;
    thePortPar->xon = CSR_BT_DEFAULT_XON;
    thePortPar->xoff = CSR_BT_DEFAULT_XOFF;
    thePortPar->parameter_mask = CSR_BT_DEFAULT_PARAMETER_MASK;
}
/* #endif */

/******************************************************************************

    function CsrBtBdAddrToEtherAddr  --  convert a BD_ADDR_T to an ETHER_ADDR

******************************************************************************/
#if !defined(EXCLUDE_CSR_BT_BNEP_MODULE) || !defined(EXCLUDE_CSR_BT_BSL_MODULE)
ETHER_ADDR CsrBtBdAddrToEtherAddr (const BD_ADDR_T *bdAddr)
{
    ETHER_ADDR etherAddr;

    etherAddr.w[0] = bdAddr->nap;
    etherAddr.w[1] = (CsrUint16) (bdAddr->uap << 8 | (CsrUint16) (bdAddr->lap >> 16));
    etherAddr.w[2] = (CsrUint16) bdAddr->lap;

    return etherAddr;
}

/******************************************************************************

    function CsrBtBdAddrFromEtherAddr  --  convert  an ETHER_ADDR to a BD_ADDR_T

******************************************************************************/
BD_ADDR_T CsrBtBdAddrFromEtherAddr (const ETHER_ADDR *etherAddr)
{
    BD_ADDR_T bdAddr;

    bdAddr.nap = etherAddr->w[0];
    bdAddr.uap = (CsrUint8) (etherAddr->w[1] >> 8);
    bdAddr.lap = (CsrUint32) (etherAddr->w[1] & 0xff) << 16 | etherAddr->w[2];

    return bdAddr;
}

ETHER_ADDR CsrBtEtherAddrWordSwap(ETHER_ADDR *addr)
{
    ETHER_ADDR etherAddr;

    etherAddr.w[0] = (CsrUint16) ((addr->w[0] >> 8) | (addr->w[0] << 8));
    etherAddr.w[1] = (CsrUint16) ((addr->w[1] >> 8) | (addr->w[1] << 8));
    etherAddr.w[2] = (CsrUint16) ((addr->w[2] >> 8) | (addr->w[2] << 8));
    return etherAddr;
}

/******************************************************************************

    function CsrBtConvBdAddr2EtherBig  --
    convert a ETHER_ADDR to big endian

******************************************************************************/

ETHER_ADDR CsrBtConvBdAddr2EtherBig (BD_ADDR_T *bdAddr)
{
    ETHER_ADDR    etherAddr;
    ETHER_ADDR    returnAddr;

    etherAddr = CsrBtBdAddrToEtherAddr(bdAddr);
    returnAddr = CsrBtEtherAddrWordSwap(&etherAddr);
    return returnAddr;
}

#if !defined(EXCLUDE_CSR_BT_BNEP_MODULE)
CsrUint16 CsrBtReadUint16Val(CsrUint8 *buf, CsrInt32 offset)
{
    return (CsrUint16) buf[offset] << 16 | buf[offset+1];
}

void CsrBtWriteUint8Val(CsrUint8 **buf, CsrUint8 val)
{
    *((*buf)++) = val & 0xFF;
}

void CsrBtWriteUint16Val(CsrUint8 **buf, CsrUint16 val)
{
    CsrBtWriteUint8Val(buf, (CsrUint8)(val >> 8));
    CsrBtWriteUint8Val(buf, (CsrUint8)(val & 0x00FF));
}

void CsrBtWriteUint32Val(CsrUint8 **buf, CsrUint32 val)
{
    CsrBtWriteUint16Val(buf, (CsrUint16)(val >> 16));
    CsrBtWriteUint16Val(buf, (CsrUint16)(val & 0xFFFF));
}

void CsrBtWriteBnepAddr(CsrUint8 **buf, const ETHER_ADDR *addr)
{
    CsrBtWriteUint16Val(buf, addr->w[0]);
    CsrBtWriteUint16Val(buf, addr->w[1]);
    CsrBtWriteUint16Val(buf, addr->w[2]);
}

#endif /* !defined(EXCLUDE_CSR_BT_BNEP_MODULE) */
#endif /* !defined(EXCLUDE_CSR_BT_BNEP_MODULE) || !defined(EXCLUDE_CSR_BT_BSL_MODULE) */

#ifdef CSR_BT_INSTALL_OPTIONAL_UTIL
char* CsrBtGetBaseName(char *file)
{
    char *pch;

    CSR_BT_RET_VAL_IF_FAIL(file != NULL, NULL);

    pch = file + CsrStrLen(file)-1;

    CSR_BT_RET_VAL_IF_FAIL(file != pch, file);

    while (*pch != '\\' && *pch != '/' && *pch != ':')
    {
        if (pch == file)
        {
            return pch;
        }
        --pch;
    }

    return ++pch;
}
#endif
CsrSize CsrUcs2ByteStrLen(const CsrUcs2String *str)
{
    CsrSize length;

    length = 0;
    if (str != NULL)
    {
        while( *str || *(str+1) )
        {
            length += 2;
            str += 2;
        }
    }
    return length;
}

/* This function should use CsrUtf8String* (D-14407)*/
#ifdef CSR_BT_INSTALL_UTIL_UCS2
CsrUcs2String * CsrUtf82Ucs2ByteString(const CsrUint8* utf8String)
{
    CsrUint32 i, length = 0;
    CsrUint32 sourceLength;

    if (utf8String != NULL)
    {
        CsrUint8 *dest;

        sourceLength = 1 + CsrStrLen((char *) utf8String);

        for (i = 0; i < sourceLength; i++)
        {
            if (utf8String[i] < 0x80)
            {
                ;
            }
            else if (utf8String[i] < 0xE0)
            {
                i += 1;
            }
            else
            {
                i += 2;
            }
            length +=2;
        }
        dest = CsrPmemAlloc(length);
        length = 0;

        for (i = 0; i < sourceLength; i++)
        {
            if (utf8String[i] < 0x80)
            {
                dest[length]        = '\0';
                dest[length + 1]    = utf8String[i];
            }
            else if (utf8String[i] < 0xE0)
            {

                dest[length] = (CsrUint8)((utf8String[i] & 0x1C) >> 2);
                dest[length + 1] = (CsrUint8)(((utf8String[i] & 0x03) << 6) + (utf8String[i + 1] & 0x3F));
                i += 1;
            }
            else
            {
                dest[length] = (CsrUint8)(((utf8String[i] & 0x0F) << 4) + ((utf8String[i + 1] & 0x3C) >> 2));
                dest[length + 1] = (CsrUint8)(((utf8String[i + 1] & 0x03) << 6) + (utf8String[i + 2] & 0x3F));
                i += 2;
            }
            length +=2;
        }
        return (dest);
    }
    return NULL;
}

/* This function should return CsrUtf8String* (D-14407)*/
CsrUint8 * CsrUcs2ByteString2Utf8(const CsrUcs2String *source)
{
    CsrUint8  *dest;
    CsrUint32    i;
    CsrUint16    x;
    CsrUint32    length;
    CsrUint32    sourceLength;

    if(!source)
    {
        return NULL;
    }

    i = 0;
    x = 0;
    length = 0;
    sourceLength = 2 + CsrUcs2ByteStrLen(source);

    for (i = 0; i < sourceLength; i += 2)
    {
        x = (CsrUint16)( ((CsrUint16)((CsrUint16) source[i]) << 8)  |
                         (CsrUint16) source[i + 1]);
        if (x < 0x0080)
        {
            length++;
        }
        else if (x < 0x0800)
        {
            length += 2;
        }
        else
        {
            length += 3;
        }
    }

    dest    = (CsrUint8 *) CsrPmemAlloc(length);
    length  = 0;
    for (i = 0; i < sourceLength; i += 2)
    {
        x = (CsrUint16)((CsrUint16)(((CsrUint16) source[i]) << 8)  | (CsrUint16) source[i + 1]);

        if (x < 0x0080)
        {
            dest[length] = (CsrUint8) x;
            length++;
        }
        else if (x < 0x0800)
        {
            dest[length]        = (CsrUint8) ((x >> 6) | 0xC0);
            dest[length + 1]    = (CsrUint8) ((x & 0x3F) | 0x80);
            length += 2;
        }
        else
        {
            dest[length]        = (CsrUint8) ((x >> 12) | 0xE0);
            dest[length + 1]    = (CsrUint8) (((x >> 6) & 0x3F) | 0x80);
            dest[length + 2]    = (CsrUint8) ((x & 0x3F) | 0x80);
            length += 3;
        }
    }
    return dest;
}
#endif

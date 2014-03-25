/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_abcsp_config_txmsg.h"
#include "csr_abcsp.h"
#include "csr_abcsp_crc.h"
#include "csr_abcsp_prototype.h"

/****************************************************************************
NAME
        CsrAbcspTxSlipInit  -  initialise the slip encoder
*/

void CsrAbcspTxSlipInit(void)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    _this->txslip.escaping = FALSE;
    _this->txslip.ubufsiz = 0;
}

/****************************************************************************
NAME
        get_uart_buffer  -  slip encode buffer and send to uart
*/

static
CsrInt32 get_uart_buffer(void)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    if (_this->txslip.ubufsiz == 0)
    {
        _this->txslip.ubufsiz = (CsrUint32) ABCSP_UART_GETTXBUF(_this);
        _this->txslip.ubufindex = 0;
    }

    return _this->txslip.ubufsiz - _this->txslip.ubufindex;
}

/****************************************************************************
NAME
        CsrAbcspTxSlipMsgDelim  -  send slip fame message delimiter to uart
*/

CsrBool CsrAbcspTxSlipMsgDelim(void)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    if (get_uart_buffer() <= 0)
    {
        return FALSE;
    }

    _this->txslip.ubuf[_this->txslip.ubufindex++] = 0xc0;
    return TRUE;
}

/****************************************************************************
NAME
        xabcsp_txslip_sendbuf -  slip encode buffer and send to uart
*/

static
CsrUint16 xabcsp_txslip_sendbuf(CsrUint8 *buf, CsrUint16 bufsiz)
{
    CsrUint16 nsent = 0;
    CsrUint8 c;
    CsrUint32 spaces, spaces_at_start;
    CsrUint8 *ubuf;
    abcsp *const _this = &CsrAbcspInstanceData;


    spaces = spaces_at_start = get_uart_buffer();
    ubuf = (CsrUint8 *) _this->txslip.ubuf + _this->txslip.ubufindex;

    if (_this->txslip.escaping && spaces)
    {
        --spaces;
        *ubuf++ = *buf == 0xc0 ? 0xdc : 0xdd;
        ++buf;
        ++nsent;
        _this->txslip.escaping = FALSE;
    }

    while (nsent < bufsiz && spaces)
    {
        --spaces;
        switch (c = *buf++)
        {
            case 0xc0:
                *ubuf++ = 0xdb;
                if (spaces != 0)
                {
                    --spaces;
                    *ubuf++ = 0xdc;
                    break;
                }
                _this->txslip.escaping = TRUE;
                _this->txslip.ubufindex += spaces_at_start - spaces;
                return nsent;

            case 0xdb:
                *ubuf++ = 0xdb;
                if (spaces != 0)
                {
                    --spaces;
                    *ubuf++ = 0xdd;
                    break;
                }
                _this->txslip.escaping = TRUE;
                _this->txslip.ubufindex += spaces_at_start - spaces;
                return nsent;

            default:
                *ubuf++ = c;
                break;
        }
        ++nsent;
    }

    _this->txslip.ubufindex += spaces_at_start - spaces;
    return nsent;
}

/****************************************************************************
NAME
        CsrAbcspTxSlipSendBuf -  slip encode buffer and send to uart
*/

CsrUint16 CsrAbcspTxSlipSendBuf(CsrUint8 *buf, CsrUint16 bufsiz)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    CsrUint16 nsent = xabcsp_txslip_sendbuf(buf, bufsiz);

#ifdef  CSR_ABCSP_TXCRC
    if ((nsent > 0) && _this->txrx.txcrc)
    {
        CsrAbcspCrcUpdate(&_this->txmsg.crc, buf, nsent);
    }
#endif  /* CSR_ABCSP_TXCRC */

    return nsent;
}

/****************************************************************************
NAME
        CsrAbcspTxSlipFlush  -  send buffer to uart
*/

void CsrAbcspTxSlipFlush(void)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    if (_this->txslip.ubufsiz != 0)
    {
        ABCSP_UART_SENDBYTES(_this->txslip.ubufindex);
        _this->txslip.ubufsiz = 0;
    }
}

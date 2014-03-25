/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_util.h"

#include "csr_abcsp.h"
#include "csr_abcsp_prototype.h"
#include "csr_abcsp_events.h"
#include "csr_abcsp_config_rxmsg.h"
#include "csr_abcsp_config_event.h"
#include "csr_abcsp_crc.h"
#include "csr_log_gsp.h"

MessageStructure CsrAbcspRxMsgStruct;

/****************************************************************************
NAME
        abcsp_rxmsg_put  -  process an entire received message
*/


static CsrBool abcsp_rxmsg_put(CsrUint8 chan, CsrBool rel,
    CsrUint32 len, CsrUint8 *in_buf)
{
    CsrAbcspRxMsgStruct.buf = in_buf;
    CsrAbcspRxMsgStruct.buflen = len;
    CsrAbcspRxMsgStruct.chan = chan;
    ABCSP_DELIVERMSG(&CsrAbcspRxMsgStruct, chan, (CsrUint32) rel);

    return TRUE;
}

/****************************************************************************
NAME
        abcsp_rxmsgdemux_putmsg  -  process an entire BCSP message
*/

static
CsrBool abcsp_rxmsgdemux_putmsg(CsrUint8 chan, CsrBool rel,
    CsrUint32 len, CsrUint8 *buf)
{
    /* Is this a BCSP Link Establishment message? */
    if ((chan == 1) && (rel == 0))
    {
        return CsrAbcspBcspLeMsgPut(buf, len);
    }
    else
    {
        return abcsp_rxmsg_put(chan, rel, len, buf);
    }
}

/****************************************************************************
NAME
        abcsp_rxbcsp_putmsg  -  decode and entire bcsp message
*/

static
void abcsp_rxbcsp_putmsg(/*const*/ CsrUint8 *msg, CsrUint32 len)
{
    CsrUint32 hdr_len, hdr_ack, hdr_seq;
    CsrUint8 hdr_chan;
    CsrBool hdr_rel, hdr_crc_present;
    abcsp *const _this = &CsrAbcspInstanceData;

    if (len < 4)
    {
        ABCSP_EVENT(ABCSP_EVT_SHORT_PAYLOAD);
        return;
    }

    if (((msg[0] + msg[1] + msg[2] + msg[3]) & 0xff) != 0xff)
    {
        ABCSP_EVENT(ABCSP_EVT_CHECKSUM);
        return;
    }

    hdr_rel = (msg[0] & 0x80) != 0;
    hdr_crc_present = (msg[0] & 0x40) != 0;
    hdr_ack = (msg[0] >> 3) & 0x07;
    hdr_seq = msg[0] & 0x07;
    hdr_chan = msg[1] & 0x0f;
    hdr_len = ((msg[1] >> 4) & 0x0f) | ((CsrUint16) msg[2] << 4);

    /* Cancel timer on ACK. */
    if (hdr_ack == _this->txmsg.txseq)
    {
        ABCSP_CANCEL_BCSP_TIMER();
    }

    if (_this->txrx.choke && !((hdr_chan == 1) && !hdr_rel))
    {
        ABCSP_EVENT(ABCSP_EVT_RX_CHOKE_DISCARD);
        return;
    }

#ifdef ABCSP_RXMSG_MAX_PAYLOAD_LEN
    if (hdr_len > ABCSP_RXMSG_MAX_PAYLOAD_LEN)
    {
        ABCSP_EVENT(ABCSP_EVT_OVERSIZE_DISCARD);
        return;
    }
#endif

    if (hdr_len + 4 + (hdr_crc_present ? 2 : 0) != len)
    {
        if (hdr_len + 4 + (hdr_crc_present ? 2 : 0) < len)
        {
            ABCSP_EVENT(ABCSP_EVT_SHORT_PAYLOAD);
        }
        else
        {
            ABCSP_EVENT(ABCSP_EVT_OVERSIZE_DISCARD);
        }
        return;
    }

#ifdef CSR_ABCSP_RXCRC
    if (hdr_crc_present && (CsrAbcspCrcBlock(msg, len - 2) != (msg[len - 2] << 8) + msg[len - 1]))
    {
        ABCSP_EVENT(ABCSP_EVT_CRC_FAIL);
        return;
    }
#endif /* CSR_ABCSP_RXCRC */

    if (hdr_rel && (hdr_seq != _this->txrx.rxseq_txack))
    {
        ABCSP_EVENT(ABCSP_EVT_MISSEQ_DISCARD);

        /* BCSP must acknowledge all reliable packets to avoid deadlock. */
        _this->txrx.txack_req = 1;

        /* Wake the tx path so that it can discard the acknowledged
           message(s). */
        ABCSP_REQ_PUMPTXMSGS(_this);
    }
    else
    {
        /* We don't deliver ack msgs (chan zero). */
        if (hdr_chan != 0)
        {
            if (!abcsp_rxmsgdemux_putmsg(hdr_chan, hdr_rel, hdr_len,
                    msg + 4))
            {
                return;
            }
        }

        /* If the message is reliable we need to note the next rel rxseq we
           will accept.  This is numerically identical to the value that we
           send back to the peer in outbound packets' ack fields to tell the
           host that we've got this message, i.e., the ack value sent to the
           peer is one more than the packet being acknowledged, modulo 8.
         */
        if (hdr_rel)
        {
            _this->txrx.rxseq_txack = (_this->txrx.rxseq_txack + 1) % 8;
            _this->txrx.txack_req = 1;

            /* Wake the tx path to send the new ack val back to the peer. */
            ABCSP_REQ_PUMPTXMSGS(_this);
        }
    }

    /* We accept rxack acknowledgement info from any intact packet,
       reliable or unreliable.  This includes reliable messages with the
       wrong seq number. */

    if (hdr_ack != _this->txrx.rxack)
    {
        _this->txrx.rxack = hdr_ack;

        /* Wake the tx path so that it can discard the
           acknowledged message(s). */

        ABCSP_REQ_PUMPTXMSGS(_this);
    }
}

/****************************************************************************
NAME
        CsrAbcspUartInit -  initialise the slip reception code block
*/

void CsrAbcspUartInit(void)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    _this->rxslip.state = rxslipstate_init_nosync;
    _this->rxslip.index = 0;
}

/****************************************************************************
NAME
        csrAbcspUartBytesDeliver  -  push received uart bytes into the library

FUNCTION
        Pushes the UART bytes into the slip decoder and then through
        to the rxbcsp (bcsp message analyser) engine.

RETURNS
    The number of bytes consumed.

    This function will stop if it cannot consume a byte or if ir
    reaches the end of a message.
*/

static CsrUint32 csrAbcspUartBytesDeliver(const CsrUint8 *ibuf, CsrUint32 len)
{
    const CsrUint8 *const buf = ibuf;
    CsrUint8 c;
    const CsrUint8 *p = buf, *end = buf + len;
    abcsp *const _this = &CsrAbcspInstanceData;

    if (len == 0)
    {
        return 0;
    }

    c = *p++;

    /*lint -save -e646 */
    switch (_this->rxslip.state)
    {
        default:
        case rxslipstate_uninit:
            ABCSP_EVENT(ABCSP_EVT_UNINITIATED);
            return 0;
        case rxslipstate_init_nosync:
            if (c != 0xc0)
            {
                return 1;
            }
            ABCSP_EVENT(ABCSP_EVT_SLIP_SYNC);
        /* FALLTHROUGH */
        case rxslipstate_nosync:
            if (c != 0xc0)
            {
                _this->rxslip.state = rxslipstate_init_nosync;
                return 1;
            }
            if (p == end)
            {
                _this->rxslip.state = rxslipstate_start;
                return 1;
            }
            c = *p++;
        /* FALLTHROUGH */
        case rxslipstate_start:
            while (c == 0xc0)
            {
                if (p == end)
                {
                    _this->rxslip.state = rxslipstate_start;
                    return (CsrUint32) (p - buf);
                }
                else
                {
                    c = *p++;
                }
            }
            _this->rxslip.state = rxslipstate_body;
            _this->rxslip.index = 0;
        /* FALLTHROUGH. */
        case rxslipstate_body:
            do
            {
                if (c == 0xc0)
                {
                    abcsp_rxbcsp_putmsg(_this->rxslip.buf, _this->rxslip.index);
                    /* Deliver message and signal "no more" to UART. */
                    _this->rxslip.state = rxslipstate_nosync;
                    return (CsrUint32) (p - buf);
                }
                else if (c != 0xdb)
                {
                    if (_this->rxslip.index >= (ABCSP_RXMSG_MAX_PAYLOAD_LEN + 4 + 2))
                    {
                        ABCSP_EVENT(ABCSP_EVT_OVERSIZE_DISCARD);
                        _this->rxslip.state = rxslipstate_nosync;
                        return (CsrUint32) (p - buf);
                    }
                    _this->rxslip.buf[_this->rxslip.index++] = c;
                    continue;
                }
                if (p == end)
                {
                    _this->rxslip.state = rxslipstate_body_esc;
                    return (CsrUint32) (p - buf);
                }
                c = *p++;
                /* FALLTHROUGH */
                case rxslipstate_body_esc:
                    if (c == 0xdc)
                    {
                        if (_this->rxslip.index >= (ABCSP_RXMSG_MAX_PAYLOAD_LEN + 4 + 2))
                        {
                            ABCSP_EVENT(ABCSP_EVT_OVERSIZE_DISCARD);
                            _this->rxslip.state = rxslipstate_nosync;
                            return (CsrUint32) (p - buf);
                        }
                        _this->rxslip.buf[_this->rxslip.index++] = 0xc0;
                    }
                    else if (c == 0xdd)
                    {
                        if (_this->rxslip.index >= (ABCSP_RXMSG_MAX_PAYLOAD_LEN + 4 + 2))
                        {
                            ABCSP_EVENT(ABCSP_EVT_OVERSIZE_DISCARD);
                            _this->rxslip.state = rxslipstate_nosync;
                            return (CsrUint32) (p - buf);
                        }
                        _this->rxslip.buf[_this->rxslip.index++] = 0xdb;
                    }
                    else
                    {
                        /* Byte sequence error.  Abandon current message. */
                        ABCSP_EVENT(ABCSP_EVT_SLIP_SYNC_LOST);
                        _this->rxslip.state = rxslipstate_init_nosync;
                        return (CsrUint32) (p - buf);
                    }
                    _this->rxslip.state = rxslipstate_body;
            } while (p < end && (c = *p++, 1));
            break;
    }
    /*lint -restore */
    return (CsrUint32) (p - buf);
}

CsrUint32 CsrTmBlueCoreBcspDeliverMsg(const CsrUint8 *data, CsrUint32 dataLength)
{
    CsrUint32 consumed = csrAbcspUartBytesDeliver(data, dataLength);

    CSR_LOG_TRANSPORT(TRANSPORT_TYPE_BCSP, CSR_LOG_TRANSPORT_INCOMING,
        consumed, dataLength, data);

    return consumed;
}

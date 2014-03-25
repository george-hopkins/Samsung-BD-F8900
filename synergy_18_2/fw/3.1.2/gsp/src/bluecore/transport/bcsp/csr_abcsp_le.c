/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_abcsp.h"
#include "csr_abcsp_config_event.h"
#include "csr_abcsp_events.h"
#include "csr_abcsp_config_timer.h"
#include "csr_util.h"

const CsrUint8 CsrAbcspLeSyncMsg[] = {0xda, 0xdc, 0xed, 0xed};
const CsrUint8 CsrAbcspLeSyncrespMsg[] = {0xac, 0xaf, 0xef, 0xee};
const CsrUint8 CsrAbcspLeConfMsg[] = {0xad, 0xef, 0xac, 0xed};
const CsrUint8 CsrAbcspLeConfrespMsg[] = {0xde, 0xad, 0xd0, 0xd0};

/* Database mapping messages sent to/from the peer bcsp-le state machine
to message identifiers. */

typedef struct
{
    lemsgid         id;         /* Message identifier. */
    const CsrUint8 *msg;           /* The message itself. */
} LEMSG;

static const LEMSG lemsgs[] =
{
    {lemsgid_sync, CsrAbcspLeSyncMsg},
    {lemsgid_sync_resp, CsrAbcspLeSyncrespMsg},
    {lemsgid_conf, CsrAbcspLeConfMsg},
    {lemsgid_conf_resp, CsrAbcspLeConfrespMsg},
    {lemsgid_none, (const CsrUint8 *) (NULL)}
};


/* Forward references. */
static void abcsp_lm_fsm(lemsgid msg);
static lemsgid match_lemsgid(const CsrUint8 *buf, CsrUint32 len);
static void req_bcsple_msg(lemsgid id);


/****************************************************************************
NAME
        CsrAbcspBcspLeInit  -  initialise the link establishment entity
*/

void CsrAbcspBcspLeInit(void)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    /* Attempt to prevent any existing timed events. */
    ABCSP_CANCEL_TCONF_TIMER();
    ABCSP_CANCEL_TSHY_TIMER();

    /* Start by setting most of the shared data to zero */
    CsrMemSet(&_this->txrx, 0x00, sizeof(struct txrx_info));
    _this->txrx.choke = TRUE;

    /* Configure the initial state of the bcsp-le state machine. */
#ifdef ABCSP_USE_BCSP_LE_PASSIVE_START
    _this->txrx.bcsple_muzzled = TRUE;
#else
    _this->txrx.bcsple_muzzled = FALSE;
#endif

    /* Stop most BCSP traffic flowing. */
    _this->txrx.choke = TRUE;

    /* Arrange to be called after a respectful interval. */
    ABCSP_START_TSHY_TIMER();

    /* Emit the first sync message if not using passive-start. */
#ifndef ABCSP_USE_BCSP_LE_PASSIVE_START
    req_bcsple_msg(lemsgid_sync);
#endif

    /* State machine's initial state. */
    _this->le.state = state_shy;
}

/****************************************************************************
NAME
        CsrAbcspBcspLeDeinit  -  de-initialise the link establishment entity
*/

void CsrAbcspBcspLeDeinit(void)
{
    /* Attempt to prevent any existing timed events. */
    ABCSP_CANCEL_TCONF_TIMER();
    ABCSP_CANCEL_TSHY_TIMER();

    ABCSP_CANCEL_BCSP_TIMER();
}

/****************************************************************************
NAME
        CsrAbcspBcspLeMsgPut  -  process an entire bcsp-le message
*/

CsrBool CsrAbcspBcspLeMsgPut(const CsrUint8 *buf, CsrUint32 len)
{
    abcsp_lm_fsm(match_lemsgid(buf, len));
    return TRUE;
}

/****************************************************************************
NAME
        CsrAbcspTshyTimedEvent  -  report a tshy timeout event to the fsm
*/

void CsrAbcspTshyTimedEvent(void)
{
    abcsp_lm_fsm(lemsgid_tshy_timeout);
}

/****************************************************************************
NAME
        CsrAbcspTconfTimedEvent  -  report a tconf timeout event to the fsm
*/

void CsrAbcspTconfTimedEvent(void)
{
    abcsp_lm_fsm(lemsgid_tconf_timeout);
}

/****************************************************************************
NAME
        abcsp_lm_fsm  -  the bcsp link establishment entity state machine

FUNCTION
        The message "msg" is fed into the bcsp-le state machine.
*/

static
void abcsp_lm_fsm(lemsgid msg)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    switch (_this->le.state)
    {
        case state_shy:
            switch (msg)
            {
                case lemsgid_tshy_timeout:
                    if (!_this->txrx.bcsple_muzzled)
                    {
                        req_bcsple_msg(lemsgid_sync);
                    }
                    ABCSP_START_TSHY_TIMER();
                    break;
                case lemsgid_sync:
                    _this->txrx.bcsple_muzzled = FALSE;
                    req_bcsple_msg(lemsgid_sync_resp);
                    break;
                case lemsgid_sync_resp:
                    ABCSP_EVENT(ABCSP_EVT_LE_SYNC);
#ifdef ABCSP_USE_OLD_BCSP_LE
                    _this->txrx.choke = FALSE;
                    _this->le.conf_cnt = 0;
#else
                    req_bcsple_msg(lemsgid_conf);
#endif
                    ABCSP_CANCEL_TSHY_TIMER();
                    ABCSP_START_TCONF_TIMER();
                    _this->le.state = state_curious;
                    break;
                default:
                    break;
            }
            break;
        case state_curious:
            switch (msg)
            {
                case lemsgid_tconf_timeout:
#ifdef ABCSP_USE_OLD_BCSP_LE
                    if (_this->le.conf_cnt < ABCSP_USE_BCSP_LE_CONF_CNT_LIMIT)
                    {
                        ++_this->le.conf_cnt;
                        req_bcsple_msg(lemsgid_conf);
                        ABCSP_START_TCONF_TIMER();
                    }
#else
                    req_bcsple_msg(lemsgid_conf);
                    ABCSP_START_TCONF_TIMER();
#endif
                    break;
                case lemsgid_sync:
                    req_bcsple_msg(lemsgid_sync_resp);
                    break;
                case lemsgid_conf:
                    req_bcsple_msg(lemsgid_conf_resp);
                    break;
                case lemsgid_conf_resp:
                    ABCSP_EVENT(ABCSP_EVT_LE_CONF);
#ifndef ABCSP_USE_OLD_BCSP_LE
                    _this->txrx.choke = FALSE;
#endif
                    ABCSP_CANCEL_TCONF_TIMER();
                    _this->le.state = state_garrulous;
                    break;
                default:
                    break;
            }
            break;
        case state_garrulous:
            switch (msg)
            {
                case lemsgid_conf:
                    req_bcsple_msg(lemsgid_conf_resp);
                    break;
                case lemsgid_sync:
                    /* Peer has apparently restarted. */
                    ABCSP_EVENT(ABCSP_EVT_LE_SYNC_LOST);
                    /* HERE Re-do the link establishment */
                    CsrAbcspBcspLeInit();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

/****************************************************************************
NAME
        match_lemsgid  -  match a link establishment message

RETURNS
        An identifier for the bcsp-le wire message at "msg", or
        lemsgid_none if the message could not be recognised.
*/

static
lemsgid match_lemsgid(const CsrUint8 *buf, CsrUint32 len)
{
    const LEMSG *m;

    /* The message should be 4 bytes long.  If so, match this against
    the expected message byte patterns and deliver any corresponding
    token to the bcsp-le state machine. */

    if (len == BCSPLE_MSGLEN)
    {
        for (m = lemsgs; m->msg != (const CsrUint8 *) (NULL); m++)
        {
            if (CsrMemCmp(buf, m->msg, BCSPLE_MSGLEN) == 0)
            {
                return m->id;
            }
        }
    }

    return lemsgid_none;
}

/****************************************************************************
NAME
        req_bcsple_msg  -  request transmission of a bcsp-le message

FUNCTION
        Requests the transmit path of the abcsp library to emit one
        of the (fixed content) bcsp-le messages.
*/

static
void req_bcsple_msg(lemsgid id)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    /* <Sigh.>  The bitfield abcsp_txrx means we can't table-drive. */
    switch (id)
    {
        case lemsgid_sync:
            _this->txrx.txsync_req = 1;
            break;
        case lemsgid_sync_resp:
            _this->txrx.txsyncresp_req = 1;
            break;
        case lemsgid_conf:
            _this->txrx.txconf_req = 1;
            break;
        case lemsgid_conf_resp:
            _this->txrx.txconfresp_req = 1;
            break;
        default:
            return;
    }

    /* Kick the transmit path into wakefulness. */
    ABCSP_REQ_PUMPTXMSGS(_this);
}

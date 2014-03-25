/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_tm_bluecore_transport.h"

void CsrTmBlueCoreTransportDelete(void *hdl)
{
    /* do nothing */
}

CsrBool CsrTmBlueCoreTransportStart(void *hdl)
{
    struct CsrTmBlueCoreTransport *p = hdl;
    CsrBool res = FALSE;
    if ((p) && (p)->start)
    {
        res = (p)->start();
        p->started = res;
    }
    return res;
}

CsrBool CsrTmBlueCoreTransportStop(void *hdl)
{
    struct CsrTmBlueCoreTransport *p = hdl;
    CsrBool res = FALSE;
    if ((p) && (p)->stop)
    {
        res = (p)->stop();
        p->started = !res;
    }
    return res;
}

CsrUint16 CsrTmBlueCoreTransportQuery(void *hdl)
{
    struct CsrTmBlueCoreTransport *p = hdl;
    CsrUint16 res = 0;
    if ((p) && (p)->query)
    {
        res = (p)->query();
    }
    return res;
}

void CsrTmBlueCoreTransportMsgTx(void *hdl, void *msg)
{
    struct CsrTmBlueCoreTransport *p = hdl;
    if ((p) && (p)->msgtx)
    {
        (p)->msgtx(msg);
    }
}

void CsrTmBlueCoreTransportMsgRx(void *hdl, void *msg)
{
    struct CsrTmBlueCoreTransport *p = hdl;
    if ((p) && (p)->msgrx)
    {
        (p)->msgrx(msg);
    }
}

void CsrTmBlueCoreTransportMsgClean(void *hdl, void *msg)
{
    struct CsrTmBlueCoreTransport *p = hdl;
    if ((p) && (p)->msgclean)
    {
        (p)->msgclean(msg);
    }
}

void CsrTmBlueCoreTransportScoTx(void *hdl, void *scoData)
{
    struct CsrTmBlueCoreTransport *p = hdl;
    if ((p) && (p)->scotx)
    {
        (p)->scotx(scoData);
    }
}

void CsrTmBlueCoreTransportRegisterMsgRx(void *hdl, void (*msgrx)(void *msg))
{
    struct CsrTmBlueCoreTransport *p = hdl;
    if ((p) && (msgrx))
    {
        (p)->msgrx = msgrx;
    }
}

CsrBool CsrTmBlueCoreTransportDriverRestart(void *hdl, CsrUint8 reset)
{
    struct CsrTmBlueCoreTransport *p = hdl;
    CsrBool res = FALSE;
    if ((p) && (p)->driverRestart)
    {
        res = (p)->driverRestart(reset);
        p->started = res;
    }
    return res;
}

void CsrTmBlueCoreTransportRestart(void *hdl)
{
    struct CsrTmBlueCoreTransport *p = hdl;
    if ((p) && (p)->restart)
    {
        (p)->restart();
    }
}

void CsrTmBlueCoreTransportClose(void *hdl)
{
    struct CsrTmBlueCoreTransport *p = hdl;
    if ((p) && (p)->close)
    {
        (p)->close();
    }
}

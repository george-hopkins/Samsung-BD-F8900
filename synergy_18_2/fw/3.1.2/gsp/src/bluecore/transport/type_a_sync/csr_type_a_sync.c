/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"

#include "csr_sdio.h"
#include "csr_type_a_sdio.h"

static void csrTypeAInsertCb(CsrSdioFunction *f);
static void csrTypeARemovedCb(CsrSdioFunction *f);
static CsrSdioInterruptDsrCallback csrTypeAIntrCb(CsrSdioFunction *);

static void csrTypeAIntrHLCb(CsrSdioFunction *);

static CsrSdioFunction *func = NULL;

static void (*insertCb)(CsrSdioFunction *) = NULL;
static void (*removedCb)(CsrSdioFunction *) = NULL;
static CsrSdioInterruptDsrCallback (*intrCb)(CsrSdioFunction *) = NULL;

static CsrSdioFunctionDriver myDrv =
{
    csrTypeAInsertCb,
    csrTypeARemovedCb,
    csrTypeAIntrCb,
    NULL, /* filled in by CsrTypeAFunctionDriverRegister */
    NULL, /* filled in by CsrTypeAFunctionDriverRegister */
    NULL, /* filled in by CsrTypeAFunctionDriverRegister */
    0,    /* filled in by CsrTypeAFunctionDriverRegister */
    NULL  /* filled in by CsrTypeAFunctionDriverRegister */
};

static CsrSchedBgint bgint_intr = CSR_SCHED_BGINT_INVALID;

static CsrSdioInterruptDsrCallback csrTypeAIntrCb(CsrSdioFunction *f)
{
    CsrSdioInterruptDsrCallback cb;

    cb = csrTypeAIntrHLCb;

    return cb;
}

static void csrTypeAIntrHLCb(CsrSdioFunction *f)
{
    CsrSchedBgintSet(bgint_intr);
}

static void csrTypeABgintIntr(void *unused)
{
    CsrSdioInterruptDsrCallback hlcb;

    hlcb = intrCb(func);
    if (hlcb != NULL)
    {
        hlcb(func);
    }
}

static void csrTypeAInsertCb(CsrSdioFunction *f)
{
    func = f;

    insertCb(f);
}

static void csrTypeARemovedCb(CsrSdioFunction *f)
{
    func = NULL;

    removedCb(f);
}

CsrResult CsrTypeAFunctionDriverRegister(CsrSdioFunctionDriver *driver)
{
    CsrResult rv;

    insertCb = driver->inserted;
    removedCb = driver->removed;
    intrCb = driver->intr;

    myDrv.suspend = driver->suspend;
    myDrv.resume = driver->resume;
    myDrv.ids = driver->ids;
    myDrv.idsCount = driver->idsCount;

    bgint_intr = CsrSchedBgintReg(csrTypeABgintIntr, NULL, "typea_sync");
    if (bgint_intr == CSR_SCHED_BGINT_INVALID)
    {
        rv = CSR_RESULT_FAILURE;
    }
    else
    {
        rv = CsrSdioFunctionDriverRegister(&myDrv);

        if (rv == CSR_RESULT_FAILURE)
        {
            CsrSchedBgintUnreg(bgint_intr);
            bgint_intr = CSR_SCHED_BGINT_INVALID;
        }
    }

    return rv;
}

void CsrTypeAFunctionDriverUnregister(CsrSdioFunctionDriver *driver)
{
    CsrSdioFunctionDriverUnregister(driver);

    CsrSchedBgintUnreg(bgint_intr);
    bgint_intr = CSR_SCHED_BGINT_INVALID;

    func = NULL;

    insertCb = NULL;
    removedCb = NULL;
    intrCb = NULL;
}

void CsrTypeARead8(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 *data,
    CsrSdioAsyncCallback cb)
{
    CsrResult rv;
    CsrSdioAsyncDsrCallback hlcb;

    rv = CsrSdioRead8(f, addr, data);

    hlcb = cb(f, rv);
    if (hlcb != NULL)
    {
        hlcb(f, rv);
    }
}

void CsrTypeAF0Read8(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 *data,
    CsrSdioAsyncCallback cb)
{
    CsrResult rv;
    CsrSdioAsyncDsrCallback hlcb;

    rv = CsrSdioF0Read8(f, addr, data);

    hlcb = cb(f, rv);
    if (hlcb != NULL)
    {
        hlcb(f, rv);
    }
}

void CsrTypeARead(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 *data,
    CsrUint32 bytes, CsrSdioAsyncCallback cb)
{
    CsrResult rv;
    CsrSdioAsyncDsrCallback hlcb;

    rv = CsrSdioRead(f, addr, data, bytes);

    hlcb = cb(f, rv);
    if (hlcb != NULL)
    {
        hlcb(f, rv);
    }
}

void CsrTypeAWrite8(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 data,
    CsrSdioAsyncCallback cb)
{
    CsrResult rv;
    CsrSdioAsyncDsrCallback hlcb;

    rv = CsrSdioWrite8(f, addr, data);

    hlcb = cb(f, rv);
    if (hlcb != NULL)
    {
        hlcb(f, rv);
    }
}

void CsrTypeAF0Write8(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 data,
    CsrSdioAsyncCallback cb)
{
    CsrResult rv;
    CsrSdioAsyncDsrCallback hlcb;

    rv = CsrSdioF0Write8(f, addr, data);

    hlcb = cb(f, rv);
    if (hlcb != NULL)
    {
        hlcb(f, rv);
    }
}

void CsrTypeAWrite(CsrSdioFunction *f, CsrUint32 addr, CsrUint8 *data,
    CsrUint32 bytes, CsrSdioAsyncCallback cb)
{
    CsrResult rv;
    CsrSdioAsyncDsrCallback hlcb;

    rv = CsrSdioWrite(f, addr, data, bytes);

    hlcb = cb(f, rv);
    if (hlcb != NULL)
    {
        hlcb(f, rv);
    }
}

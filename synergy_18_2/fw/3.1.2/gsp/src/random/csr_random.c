/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_time.h"

/* Notes about the algorithm:

    Name: 32bit KISS by George Marsaglia
    Published: http://groups.google.com/group/comp.lang.fortran/msg/6edb8ad6ec5421a5
    Period: ~2^121

    Seeding requirements:
        x = any 32bit integer
        y != 0
        z/w = any 31bit unsigned integer, not a multiple of 7559
        c = 0 or 1

    Actual seeding uses the timestamp along with the address of the state
    structure to add a non-time related component to the entropy. */

typedef struct
{
    CsrUint32 x;
    CsrUint32 y;
    CsrUint32 z;
    CsrUint32 w;
    CsrUint32 c;
} CsrRandomState;

void *CsrRandomSeed(void)
{
    CsrRandomState *s = CsrPmemAlloc(sizeof(CsrRandomState));
    CsrTimeUtc tod;
    CsrTime low, high;

    CsrTimeUtcGet(&tod, &low, &high);
    s->x = tod.sec + tod.msec + low + high;
    s->y = (CsrUint32) (CsrUintptr) s;
    s->z = s->w = (s->x + s->y) & ~(((CsrUint32) 1) << 31);
    if ((s->z % 7559) == 0)
    {
        s->z++;
        s->w++;
    }
    s->c = 0;
    return s;
}

CsrUint32 CsrRandom(void *randomState)
{
    CsrRandomState *s = (CsrRandomState *) randomState;
    CsrUint32 t;

    s->x += 545925293;
    s->y ^= (s->y << 13);
    s->y ^= (s->y >> 17);
    s->y ^= (s->y << 5);
    t = s->z + s->w + s->c;
    s->z = s->w;
    s->c = (t >> 31);
    s->w = t & 0x7FFFFFFF;

    return s->x + s->y + s->w;
}

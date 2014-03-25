/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "platform/csr_s3c24a0.h"
#include "platform/csr_mmu.h"

/* Force ARM mode, as some of the assembler instructions below require it. */
#if !(__ARMCC_VERSION < 200000)
#pragma arm
#endif

void MMUSetMTT(CsrUint32 mtt, CsrUint32 vaddrStart, CsrUint32 vaddrEnd, CsrUint32 paddrStart, CsrUint32 attr)
{
    CsrUint32 *pTT;
    CsrUint32 i, nSec;

    pTT = (CsrUint32 *) mtt + (vaddrStart >> 20);
    nSec = (vaddrEnd >> 20) - (vaddrStart >> 20);

    for (i = 0; i <= nSec; i++)
    {
        *pTT++ = attr | (((paddrStart >> 20) + i) << 20);
    }
}

void MMUShutdown(void)
{
    /* Disable Data Cache */
    MMUDisableDCache();

    /* Disable MMU */
    MMUDisableMMU();
}

void MMUInitialise(CsrUint32 MTTBase)
{
    /* Disable Alignment Fault */
    MMUDisableAlignFault();

    /* Reset Process ID */
    MMUSetProcessId(0);

    /* Set MTT Base */
    MMUSetMTTBase(MTTBase);

    /* Set Domain Access Permissions */
    MMUSetDomainAccess(0xFFFFFFFF);

    /* Enable Alignment Fault Checking */
    MMUEnableAlignFault();

    /* Clean D Cache (Write-back) */
    MMUCleanDCache();

    /* Invalidate D and I Cache */
    MMUInvalidateIDCache();

    /* Invalidate TLB */
    MMUInvalidateTLB();

    /* Enable MMU, Instruction and Data Cache */
    MMUEnableMIDCache();
}

void MMUCleanInvalidateDCacheRegion(CsrUint32 address, CsrUint32 size)
{
    int mva = address & ~0x1F;
    do
    {
        __asm{mcr p15, 0, mva, c7, c14, 1};
        mva += 32;
    } while (mva < (address + size));
}

void MMUInvalidateDCacheRegion(CsrUint32 address, CsrUint32 size)
{
    int mva = address & ~0x1F;
    do
    {
        __asm{mcr p15, 0, mva, c7, c6, 1};
        mva += 32;
    } while (mva < (address + size));
}

void MMUCleanDCacheRegion(CsrUint32 address, CsrUint32 size)
{
    int mva = address & ~0x1F;
    do
    {
        __asm{mcr p15, 0, mva, c7, c10, 1};
        mva += 32;
    } while (mva < (address + size));
}

#define CACHE_LINE_SIZE    32

void *MMUCacheLineAlignedMemoryAllocate(CsrSize size, void *(*memAlloc)(CsrSize size))
{
    void *freePtr, *alignedPtr;

    /* Round up requested size to a multiple of cache line size, and allocate
       extra space for aligning chunk to cache line size, and for writing the
       freePtr before the chunk. */
    size = ((size - 1) | (CACHE_LINE_SIZE - 1)) + 1 + CACHE_LINE_SIZE - 1 + sizeof(void *);

    /* Allocate the chunk */
    freePtr = memAlloc(size);

    /* Make room for free pointer and align to cache line boundary */
    alignedPtr = (void *) (((((CsrUintptr) freePtr) - 1 + sizeof(void *)) | (CACHE_LINE_SIZE - 1)) + 1);

    /* Write free pointer right before alignedPtr */
    *((void **) (((CsrUintptr) alignedPtr) - sizeof(void *))) = freePtr;

    return alignedPtr;
}

#ifdef CSR_MEMALLOC_PROFILING
void *MMUCacheLineAlignedMemoryAllocateDebug(CsrSize size, CsrSize header,
    void **hdr, void *(*memAlloc)(CsrSize size))
{
    /*
     * Same as MMUCacheLineAlignedMemoryAllocate() except there
     * is additional headroom for a header.
     */
    void *freePtr, *alignedPtr;

    /* Round up requested size to a multiple of cache line size, and allocate
       extra space for aligning chunk to cache line size, and for writing the
       freePtr before the chunk. */
    size = ((size - 1) | (CACHE_LINE_SIZE - 1)) + 1 + CACHE_LINE_SIZE - 1 + sizeof(void *) + header;

    /* Allocate the chunk */
    freePtr = memAlloc(size);
    *hdr = freePtr;

    /* Make room for free pointer and align to cache line boundary */
    alignedPtr = (void *) (((((CsrUintptr) freePtr) - 1 + sizeof(void *)) | (CACHE_LINE_SIZE - 1)) + 1);

    /* Write free pointer right before alignedPtr */
    *((void **) (((CsrUintptr) alignedPtr) - sizeof(void *))) = freePtr;

    return alignedPtr;
}
#endif

void MMUCacheLineAlignedMemoryFree(void *ptr, void (*memFree)(void *ptr))
{
    memFree(*(((void **) ptr) - 1));
}

void MMUEnableMIDCache(void)
{
    int r;
    __asm{mrc p15, 0, r, c1, c0, 0};
    __asm{orr r, r, 1};
    __asm{orr r, r, 1 << 12};
    __asm{orr r, r, 1 << 2};
    __asm{mcr p15, 0, r, c1, c0, 0};
}

void MMUDisableMIDCache(void)
{
    int r;
    __asm{mrc p15, 0, r, c1, c0, 0};
    __asm{bic r, r, 1};
    __asm{bic r, r, 1 << 12};
    __asm{bic r, r, 1 << 2};
    __asm{mcr p15, 0, r, c1, c0, 0};
}

void MMUEnableICache(void)
{
    int r;
    __asm{mrc p15, 0, r, c1, c0, 0};
    __asm{orr r, r, 1 << 12};
    __asm{mcr p15, 0, r, c1, c0, 0};
}

void MMUDisableICache(void)
{
    int r;
    __asm{mrc p15, 0, r, c1, c0, 0};
    __asm{bic r, r, 1 << 12};
    __asm{mcr p15, 0, r, c1, c0, 0};
}

void MMUEnableDCache(void)
{
    int r;
    __asm{mrc p15, 0, r, c1, c0, 0};
    __asm{orr r, r, 1 << 2};
    __asm{mcr p15, 0, r, c1, c0, 0};
}

void MMUDisableDCache(void)
{
    int r;
    __asm{mrc p15, 0, r, c1, c0, 0};
    __asm{bic r, r, 1 << 2};
    __asm{mcr p15, 0, r, c1, c0, 0};
}

void MMUEnableAlignFault(void)
{
    int r;
    __asm{mrc p15, 0, r, c1, c0, 0};
    __asm{orr r, r, 1 << 1};
    __asm{mcr p15, 0, r, c1, c0, 0};
}

void MMUDisableAlignFault(void)
{
    int r;
    __asm{mrc p15, 0, r, c1, c0, 0};
    __asm{bic r, r, 1 << 1};
    __asm{mcr p15, 0, r, c1, c0, 0};
}

void MMUEnableMMU(void)
{
    int r;
    __asm{mrc p15, 0, r, c1, c0, 0};
    __asm{orr r, r, 1};
    __asm{mcr p15, 0, r, c1, c0, 0};
}

void MMUDisableMMU(void)
{
    int r;
    __asm{mrc p15, 0, r, c1, c0, 0};
    __asm{bic r, r, 1};
    __asm{mcr p15, 0, r, c1, c0, 0};
}

void MMUSetMTTBase(CsrUint32 base)
{
    base = base & ~0x3FFF;
    __asm{mcr p15, 0, base, c2, c0, 0};
}

void MMUSetDomainAccess(CsrUint32 domainAccess)
{
    __asm{mcr p15, 0, domainAccess, c3, c0, 0};
}

CsrUint32 MMUReadDFSR(void)
{
    CsrUint32 dfsr;
    __asm{mrc p15, 0, dfsr, c5, c0, 0};
    return dfsr;
}

CsrUint32 MMUReadIFSR(void)
{
    CsrUint32 ifsr;
    __asm{mrc p15, 0, ifsr, c5, c0, 1};
    return ifsr;
}

CsrUint32 MMUReadFAR(void)
{
    CsrUint32 far;
    __asm{mrc p15, 0, far, c6, c0, 0};
    return far;
}

#if __ARMCC_VERSION < 200000
void MMUCleanDCache(void)
{
    __asm
    {
        loop:
        mrc p15, 0, r15, c7, c10, 3;
        bne loop;
    }
}
#else
__asm void MMUCleanDCache(void)
{
    ARM
0   mrc p15, 0, r15, c7, c10, 3
    bne %B0
    bx lr
}
#endif

void MMUCleanDCacheMVA(CsrUint32 mva)
{
    __asm{mcr p15, 0, mva, c7, c10, 1};
}

void MMUCleanDCacheSET(CsrUint32 set)
{
    __asm{mcr p15, 0, set, c7, c10, 2};
}

void MMUInvalidateDCache(void)
{
    CsrUint32 r = 0;
    __asm{mcr p15, 0, r, c7, c6, 0};
}

void MMUInvalidateDCacheMVA(CsrUint32 mva)
{
    __asm{mcr p15, 0, mva, c7, c6, 1};
}

void MMUInvalidateDCacheSET(CsrUint32 set)
{
    __asm{mcr p15, 0, set, c7, c6, 2};
}

#if __ARMCC_VERSION < 200000
void MMUCleanInvalidateDCache(void)
{
    __asm
    {
        loop:
        mrc p15, 0, r15, c7, c14, 3;
        bne loop;
    }
}
#else
__asm void MMUCleanInvalidateDCache(void)
{
    ARM
0   mrc p15, 0, r15, c7, c14, 3
    bne %B0
    bx lr
}
#endif

void MMUCleanInvalidateDCacheMVA(CsrUint32 mva)
{
    __asm{mcr p15, 0, mva, c7, c14, 1};
}

void MMUCleanInvalidateDCacheSET(CsrUint32 set)
{
    __asm{mcr p15, 0, set, c7, c14, 2};
}

void MMUInvalidateICache(void)
{
    CsrUint32 r = 0;
    __asm{mcr p15, 0, r, c7, c5, 0};
}

void MMUInvalidateICacheMVA(CsrUint32 mva)
{
    __asm{mcr p15, 0, mva, c7, c5, 1};
}

void MMUInvalidateICacheSET(CsrUint32 set)
{
    __asm{mcr p15, 0, set, c7, c5, 2};
}

void MMUPrefetchICacheMVA(CsrUint32 mva)
{
    __asm{mcr p15, 0, mva, c7, c13, 1};
}

void MMUDrainWriteBuffer(void)
{
    CsrUint32 r = 0;
    __asm{mcr p15, 0, r, c7, c10, 4};
}

void MMUInvalidateIDCache(void)
{
    CsrUint32 r = 0;
    __asm{mcr p15, 0, r, c7, c7, 0};
}

void MMUWaitForInterrupt(void)
{
    CsrUint32 r = 0;
    __asm{mcr p15, 0, r, c7, c0, 4};
}

void MMUInvalidateTLB(void)
{
    CsrUint32 r = 0;
    __asm{mcr p15, 0, r, c8, c7, 0};
}

void MMUInvalidateTLBMVA(CsrUint32 mva)
{
    __asm{mcr p15, 0, mva, c8, c7, 1};
}

void MMUInvalidateITLB(void)
{
    CsrUint32 r = 0;
    __asm{mcr p15, 0, r, c8, c5, 0};
}

void MMUInvalidateITLBMVA(CsrUint32 mva)
{
    __asm{mcr p15, 0, mva, c8, c5, 1};
}

void MMUInvalidateDTLB(void)
{
    CsrUint32 r = 0;
    __asm{mcr p15, 0, r, c8, c6, 0};
}

void MMUInvalidateDTLBMVA(CsrUint32 mva)
{
    __asm{mcr p15, 0, mva, c8, c6, 1};
}

void MMUSetDCacheLockdownWay(CsrUint32 way)
{
    CsrUint32 r;
    __asm{mrc p15, 0, r, c9, c0, 0};
    __asm{orr r, r, way};
    __asm{mcr p15, 0, r, c9, c0, 0};
}

void MMUSetDCacheUnlockdownWay(CsrUint32 way)
{
    CsrUint32 r;
    __asm{mrc p15, 0, r, c9, c0, 0};
    __asm{bic r, r, way};
    __asm{mcr p15, 0, r, c9, c0, 0};
}

void MMUSetICacheLockdownWay(CsrUint32 way)
{
    CsrUint32 r;
    __asm{mrc p15, 0, r, c9, c0, 1};
    __asm{orr r, r, way};
    __asm{mcr p15, 0, r, c9, c0, 1};
}

void MMUSetICacheUnlockdownWay(CsrUint32 way)
{
    CsrUint32 r;
    __asm{mrc p15, 0, r, c9, c0, 1};
    __asm{bic r, r, way};
    __asm{mcr p15, 0, r, c9, c0, 1};
}

void MMUSetDTLBLockdown(CsrUint32 addr)
{
    CsrUint32 r = addr;
    __asm{mcr p15, 0, r, c8, c7, 1}; /* invalidate TLB single entry to ensure that addr is not already in the TLB */
    __asm{mrc p15, 0, addr, c10, c0, 0}; /* read the lockdown register */
    addr = addr | 1; /* set the preserve bit */
    __asm{mcr p15, 0, addr, c10, c0, 0}; /* write to the lockdown register */
    __asm{ldr r, [r]}; /* TLB will miss, and entry will be loaded */
    __asm{mrc p15, 0, addr, c10, c0, 0}; /* read the lockdown register (victim will have incremented) */
    addr = addr & ~1; /* clear preserve bit */
    __asm{mcr p15, 0, addr, c10, c0, 0}; /* write to the lockdown register */
}

void MMUSetProcessId(CsrUint32 pid)
{
    __asm{mcr p15, 0, pid, c13, c0, 0};
}

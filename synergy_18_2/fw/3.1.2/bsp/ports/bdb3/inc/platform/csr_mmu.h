#ifndef CSR_MMU_H__
#define CSR_MMU_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*  Simple Cache Operation

    1) Shutdown/reset the MMU:

        MMUShutdown();

    2) Initialise the Master Translation Table (using default value for base):

        MMUSetMTT(MMU_DEFAULT_MTT_BASE, 0x00000000, 0xFFFFFFFF, 0x00000000, MMU_RW_NCNB);

    3) Configure the entire RAM area (less the last 3MiB) as cached
       (write-through), and 1-to-1 mapped:

        MMUSetMTT(MMU_DEFAULT_MTT_BASE, SDRAM_BASE, SDRAM_TOP - 1024 * 1024 * 3 - 1, SDRAM_BASE, MMU_RW_CNB);

    4) Initialise the MMU:

        MMUInitialise(MMU_DEFAULT_MTT_BASE); */


/* Default location for MTT is at the end of the SDRAM */
#define MMU_DEFAULT_MTT_BASE     0x5FFFC000

#define MMU_MTT_SECTIONENTRY     0x00000002
#define MMU_MTT_FAULTENTRY       0x00000000
#define MMU_MTT_COARSEENTRY      0x00000001

#define MMU_CB                  (3 << 2)  /* Cache on, write-back */
#define MMU_CNB                 (2 << 2)  /* Cache on, write-through */
#define MMU_NCB                 (1 << 2)  /* Cache off, buffered writes */
#define MMU_NCNB                (0 << 2)  /* Cache off, non-buffered writes */

#define MMU_AP_RW               (3 << 10) /* Supervisor=RW, User=RW */
#define MMU_AP_RO               (2 << 10) /* Supervisor=RW, User=RO */
#define MMU_AP_NO               (1 << 10) /* Supervisor=RW, User=No access */

#define MMU_RW_CB               (MMU_AP_RW | MMU_CB | MMU_MTT_SECTIONENTRY)
#define MMU_RW_CNB              (MMU_AP_RW | MMU_CNB | MMU_MTT_SECTIONENTRY)
#define MMU_RW_NCB              (MMU_AP_RW | MMU_NCB | MMU_MTT_SECTIONENTRY)
#define MMU_RW_NCNB             (MMU_AP_RW | MMU_NCNB | MMU_MTT_SECTIONENTRY)

/* Set an entry in the Master Translation Table (MTT) */
void MMUSetMTT(CsrUint32 mtt, CsrUint32 vaddrStart, CsrUint32 vaddrEnd, CsrUint32 paddrStart, CsrUint32 attr);

/* Completely (and safely) shutdown all MMU related functionality and reset it to a known (disabled) state.
    1) Disable MMU and invalidate the TLB (Translation-Lookaside-Buffer)
    2) Disable, clean and invalidate Data Cache
    3) Disable and flush Instruction Cache */
void MMUShutdown(void);

/* Initialise and enable the MMU, Instruction and Data Cache */
void MMUInitialise(CsrUint32 MTTBase);

/* Clean (Writeback) and Invalidate (Flush) the part of the Data Cache that
   stores entries pertaining to the given memory region (start address and size) */
void MMUCleanInvalidateDCacheRegion(CsrUint32 address, CsrUint32 size);

/* Invalidate (Flush) the part of the Data Cache that stores
   entries pertaining to the given memory region (start address and size) */
void MMUInvalidateDCacheRegion(CsrUint32 address, CsrUint32 size);

/* Clean (Writeback) the part of the Data Cache that stores
   entries pertaining to the given memory region (start address and size) */
void MMUCleanDCacheRegion(CsrUint32 address, CsrUint32 size);


/* Wrapper functions for allocating memory that:

    1) Is aligned to a cache line boundary.
    2) Extends to the end of the last cache line it overlaps with.

   The second parameter to the function calls is a standard allocate function
   that is allowed to return arbitrarily aligned memory, that will be used by
   the wrapper. The wrapper works by allocating a memory area that is larger
   than requested to allow the memory area to be realigned on a cache line
   boundary, and to make sure that the allocated area extends to the end of the
   last cache line it overlaps with.

   Memory allocation should only be wrapped by these functions if both of these
   conditions are true:

   1) The memory is passed to a driver that uses DMA to perform the transfer.
   2) The cache configuration for the memory is write-back.

   If one or both of these are not true, it is not necessary to use these
   functions for wrapping the memory allocation. */

void *MMUCacheLineAlignedMemoryAllocate(CsrSize size, void *(*memAlloc)(CsrSize size));
#ifdef CSR_MEMALLOC_PROFILING
void *MMUCacheLineAlignedMemoryAllocateDebug(CsrSize size, CsrSize header,
    void **hdr, void *(*memAlloc)(CsrSize size));
#endif
void MMUCacheLineAlignedMemoryFree(void *ptr, void (*memFree)(void *ptr));


void MMUEnableMIDCache(void);
void MMUDisableMIDCache(void);
void MMUEnableICache(void);
void MMUDisableICache(void);
void MMUEnableDCache(void);
void MMUDisableDCache(void);
void MMUEnableAlignFault(void);
void MMUDisableAlignFault(void);
void MMUEnableMMU(void);
void MMUDisableMMU(void);
void MMUSetMTTBase(CsrUint32 base);
void MMUSetDomainAccess(CsrUint32 domain);
CsrUint32 MMUReadDFSR(void);
CsrUint32 MMUReadIFSR(void);
CsrUint32 MMUReadFAR(void);
CsrUint32 MMUReadIFAR(void);
void MMUInvalidateIDCache(void);
void MMUInvalidateICache(void);
void MMUInvalidateICacheMVA(CsrUint32 mva);
void MMUInvalidateICacheSET(CsrUint32 set);
void MMUPrefetchICacheMVA(CsrUint32 mva);
void MMUInvalidateDCache(void);
void MMUInvalidateDCacheMVA(CsrUint32 mva);
void MMUInvalidateDCacheSET(CsrUint32 set);
void MMUCleanDCache(void);
void MMUCleanDCacheMVA(CsrUint32 mva);
void MMUCleanDCacheSET(CsrUint32 set);
void MMUCleanInvalidateDCacheMVA(CsrUint32 mva);
void MMUCleanInvalidateDCacheSET(CsrUint32 set);
void MMUCleanInvalidateDCache(void);
void MMUDrainWriteBuffer(void);
void MMUWaitForInterrupt(void);
void MMUInvalidateTLB(void);
void MMUInvalidateTLBMVA(CsrUint32 mva);
void MMUInvalidateITLB(void);
void MMUInvalidateITLBMVA(CsrUint32 mva);
void MMUInvalidateDTLB(void);
void MMUInvalidateDTLBMVA(CsrUint32 mva);
void MMUSetDCacheLockdownWay(CsrUint32 way);
void MMUSetDCacheUnlockdownWay(CsrUint32 way);
void MMUSetICacheLockdownWay(CsrUint32 way);
void MMUSetICacheUnlockdownWay(CsrUint32 way);
void MMUSetDTLBLockdown(CsrUint32 addr);
void MMUSetProcessId(CsrUint32 pid);

#ifdef __cplusplus
}
#endif

#endif

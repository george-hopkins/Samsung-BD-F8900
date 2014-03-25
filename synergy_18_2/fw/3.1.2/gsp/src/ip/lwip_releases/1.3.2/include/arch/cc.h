#ifndef CC_H__
#define CC_H__
/*cc.h       - Architecture environment, some compiler specific, some
             environment specific (probably should move env stuff
             to sys_arch.h.)
*/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_types.h"
#include "csr_usr_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*  Typedefs for the types used by lwip -*/
typedef CsrUint8 u8_t;
#define U8_F "d"
typedef CsrInt8 s8_t;
#define S8_F "d"
#define X8_F "X"
typedef CsrUint16 u16_t;
#define U16_F "d"
typedef CsrInt16 s16_t;
#define S16_F "d"
#define X16_F "X"
typedef CsrUint32 u32_t;
#define U32_F "u"
typedef CsrInt32 s32_t;
#define S32_F "d"
#define X32_F "X"
typedef CsrUintptr mem_ptr_t;

#ifdef CSR_IP_USE_BIG_ENDIAN
#define LWIP_BYTE_ORDER LWIP_BIG_ENDIAN
#else
#define LWIP_BYTE_ORDER LWIP_LITTLE_ENDIAN
#define LWIP_PLATFORM_BYTESWAP 1
#define LWIP_PLATFORM_HTONS(x) ((u16_t) ((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF)))
#define LWIP_PLATFORM_HTONL(x) ((u32_t) ((((x) & 0xFF) << 24) | \
                                         (((x) & 0xFF00) << 8) | \
                                         (((x) >> 8) & 0xFF00) | \
                                         (((x) >> 24) & 0xFF)))
#endif

/* Select optimised built-in checksum algorithm */
#define LWIP_CHKSUM_ALGORITHM 3

/* Compiler hints for packing lwip's structures */
#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

#define LWIP_PLATFORM_DIAG(message)
#define LWIP_PLATFORM_ASSERT(message)

/* - fatal, print message and abandon execution.*/
/*
  "lightweight" synchronization mechanisms -

    SYS_ARCH_DECL_PROTECT(x) - declare a protection state variable.
    SYS_ARCH_PROTECT(x)      - enter protection mode.
    SYS_ARCH_UNPROTECT(x)    - leave protection mode.

  If the compiler does not provide CsrMemSet() this file must include a
  definition of it, or include a file which defines it.

  This file must either include a system-local <errno.h> which defines
  the standard *nix error codes, or it should #define LWIP_PROVIDE_ERRNO
  to make lwip/arch.h define the codes which are used throughout.
*/
/*
perf.h     - Architecture specific performance measurement.
  Measurement calls made throughout lwip, these can be defined to nothing.
    PERF_START               - start measuring something.
    PERF_STOP(x)             - stop measuring something, and record the result.

sys_arch.h - Tied to sys_arch.c

  Arch dependent types for the following objects:
    sys_sem_t, sys_mbox_t, sys_thread_t,
  And, optionally:
    sys_prot_t

  Defines to set vars of sys_mbox_t and sys_sem_t to NULL.
    SYS_MBOX_NULL NULL
    SYS_SEM_NULL NULL
*/

#ifdef __cplusplus
}
#endif

#endif

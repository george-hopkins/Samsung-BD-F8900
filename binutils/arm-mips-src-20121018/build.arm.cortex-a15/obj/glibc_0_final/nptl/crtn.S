	.cpu cortex-a15
	.eabi_attribute 27, 3
	.fpu vfpv3
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
#APP
	
#include "defs.h"
	
#if defined __i686 && defined __ASSEMBLER__
	
#undef __i686
	
#define __i686 __i686
	
#endif
	
/*@HEADER_ENDS*/
/*@_init_EPILOG_BEGINS*/
@ 0 "" 2
@ 92 "../nptl/sysdeps/pthread/pt-initfini.c" 1
	.section .init
@ 0 "" 2
	ldmfd	sp!, {r3, pc}
#APP
	END_INIT
	
/*@_init_EPILOG_ENDS*/
/*@_fini_EPILOG_BEGINS*/
@ 0 "" 2
@ 121 "../nptl/sysdeps/pthread/pt-initfini.c" 1
	.section .fini
@ 0 "" 2
	ldmfd	sp!, {r3, pc}
#APP
	END_FINI
	
/*@_fini_EPILOG_ENDS*/
/*@TRAILER_BEGINS*/
	.hidden	__pthread_initialize_minimal_internal
	.ident	"GCC: (Linaro GCC 4.6-2012.08-1~dev) 4.6.4"
	.section	.note.GNU-stack,"",%progbits

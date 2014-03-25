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
/*@_init_PROLOG_BEGINS*/
	.section .init
	.section	.init,"ax",%progbits
	.align	2
	.global	_init
	.type	_init, %function
_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r3, lr}
	bl	__pthread_initialize_minimal_internal(PLT)
#APP
@ 87 "../nptl/sysdeps/pthread/pt-initfini.c" 1
	ALIGN
@ 0 "" 2
@ 88 "../nptl/sysdeps/pthread/pt-initfini.c" 1
	END_INIT
@ 0 "" 2
@ 90 "../nptl/sysdeps/pthread/pt-initfini.c" 1
	
/*@_init_PROLOG_ENDS*/
/*@_fini_PROLOG_BEGINS*/
	.section .fini
	.section	.fini,"ax",%progbits
	.align	2
	.global	_fini
	.type	_fini, %function
_fini:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r3, lr}
#APP
@ 107 "../nptl/sysdeps/pthread/pt-initfini.c" 1
	ALIGN
@ 0 "" 2
@ 108 "../nptl/sysdeps/pthread/pt-initfini.c" 1
	END_FINI
@ 0 "" 2
@ 109 "../nptl/sysdeps/pthread/pt-initfini.c" 1
	
/*@_fini_PROLOG_ENDS*/
/*@TRAILER_BEGINS*/
	.hidden	__pthread_initialize_minimal_internal
	.ident	"GCC: (Linaro GCC 4.6-2012.08-1~dev) 4.6.4"
	.section	.note.GNU-stack,"",%progbits

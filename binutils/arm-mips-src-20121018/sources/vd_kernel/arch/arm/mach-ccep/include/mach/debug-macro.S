/* arch/arm/mach-ccep/include/mach/debug-macro.S
 *
 * Debugging macro include header
 *
 *  Copyright (C) 2010 tukho.kim@samsung.com
 *
*/
#if 0
#include <mach/platform.h>
#include <plat/sdp_serial.h>
		.macro	addruart,rx
		mrc	p15, 0, \rx, c1, c0
		tst	\rx, #1				@ MMU enabled?
		moveq	\rx, #PA_UART_BASE&0xFF000000	@ physical base address
		movne	\rx, #VA_UART_BASE&0xFF000000	@ virtual base
		add	\rx, \rx, #PA_UART_BASE&0x00FF0000
		add	\rx, \rx, #PA_UART_BASE&0x0000FF00
		add	\rx, \rx, #PA_UART_BASE&0x000000FF
		add	\rx, \rx, #0x40	@ UART1 offset
		.endm

		.macro	senduart,rd,rx
		str	\rd, [\rx, #0x20]
		.endm

		.macro waituart,rd,rx
		1001:
		ldr	\rd, [\rx, #0x10
		ands	\rd, \rd, #4
		beq	1001b
		.endm

		.macro	busyuart,rd,rx
		.endm
#endif

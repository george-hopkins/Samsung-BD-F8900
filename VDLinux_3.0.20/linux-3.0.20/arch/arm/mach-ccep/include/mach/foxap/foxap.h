/*
 *  arch/arm/mach-ccep/include/mach/foxap.h
 *
 *  Copyright (C) 2012 Samsung Electronics
 *  Author: seungjun.heo@samsung.com 
 *
 */

#ifndef __FOXAP_H
#define __FOXAP_H

#include <mach/irqs.h>
#include <mach/ccep_soc.h>

/* clock parameters, see clocks.h */
#define INPUT_FREQ		24000000
#define FIN			INPUT_FREQ

/* Only using initialize kernel */
#define PCLK			(161750000)

/* system timer tick or high-resolution timer*/
#define SYS_TICK		HZ

/* timer definition  */
#define SYS_TIMER		0
#define SYS_TIMER_PRESCALER 	10 	// PCLK = 647 * 625 * 4 * 100 

#define TIMER_CLOCK		(REQ_PCLK)
#define SDP_GET_TIMERCLK(x)	sdp1202_get_clock(x)


// 20080523 HRTimer (clock source) define
#define CLKSRC_TIMER		1
// 20080523 HRTimer (clock source) define end
/* system timer tick or high-resolution timer end */ 


/* Memory Define */
/* Move to arch/arm/mach-ccep/include/mach/memory.h */
#include <mach/memory.h>

/* System Kernel memory size */
#define KERNEL_MEM_SIZE			(SYS_MEM0_SIZE + SYS_MEM1_SIZE)	// 100MByte

/* flash chip locations(SMC banks) */
#define PA_FLASH_BANK0			(0x0)
#define PA_FLASH_BANK1			(0x04000000)
#define FLASH_MAXSIZE			(32 << 20)		

// UART definition 
#define SDP_UART_NR			(4)

#define SER_BAUD_9600			(9600)
#define SER_BAUD_19200			(19200)
#define SER_BAUD_38400			(38400)
#define SER_BAUD_57600			(57600)
#define SER_BAUD_115200			(115200)
#define CURRENT_BAUD_RATE		(SER_BAUD_115200)

#define UART_CLOCK			(REQ_PCLK)
#define SDP_GET_UARTCLK(x)		sdp1202_get_clock(x)

#endif


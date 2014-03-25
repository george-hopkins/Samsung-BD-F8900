/*
 *  linux/include/asm-arm/hardware/serial_s5h2x.h
 *
 *  Internal header file for Samsung SDP serial ports (UART0, 1)
 *
 *  Copyright (C) 2006 Samsung Electronics.
 *
 *  Adapted from:
 *
 */
#ifndef ASM_ARM_HARDWARE_SERIAL_SDP_H
#define ASM_ARM_HARDWARE_SERIAL_SDP_H

/* register offset */
#define SDP_UARTLCON_OFF      (0x00)
#define SDP_UARTCON_OFF       (0x04)
#define SDP_UARTFCON_OFF      (0x08)
#define SDP_UARTMCON_OFF      (0x0C)
#define SDP_UARTTRSTAT_OFF    (0x10)
#define SDP_UARTERSTAT_OFF    (0x14)
#define SDP_UARTFSTAT_OFF     (0x18)
#define SDP_UARTMSTAT_OFF     (0x1C)
#define SDP_UARTTXH0_OFF      (0x20)
#define SDP_UARTRXH0_OFF      (0x24)
#define SDP_UARTBRDIV_OFF     (0x28)

#define SDP_UART_GAP		(0x40)

#define SDP_UART0_OFF         (0x0)
#define SDP_UART1_OFF         (0x40)
#define SDP_UART2_OFF         (0x80)
#define SDP_UART3_OFF         (0xC0)

#define SDP_LCON_CFGMASK      (0x3F)

/* LCON Word length */
#define SDP_LCON_CS5          (0x0)
#define SDP_LCON_CS6          (0x1)
#define SDP_LCON_CS7          (0x2)
#define SDP_LCON_CS8          (0x3)

/* LCON Parity */
#define SDP_LCON_PNONE        (0x0)
#define SDP_LCON_PODD         ((0x4)<<3)
#define SDP_LCON_PEVEN        ((0x5)<<3)


#define SDP_UCON_RXIRQMODE		(0x1)
#define SDP_UCON_RXIRQMODE_MASK	(0x3)
#define SDP_UCON_TXIRQMODE		(0x1 << 2)
#define SDP_UCON_TXIRQMODE_MASK	(0x3 << 2)
#define SDP_UCON_SBREAK		(1<<4)
#define SDP_UCON_RXFIFO_TOI	(1<<7)
// [8:9] must be 0's
#define SDP_UCON_CLKSEL		(0<<10)
#define SDP_UCON_RXIE			(1<<12)
#define SDP_UCON_TXIE			(1<<13)
#define SDP_UCON_ERIE			(1<<14)

#define SDP_UCON_DEFAULT	(SDP_UCON_TXIE   \
				| SDP_UCON_RXIE		\
				| SDP_UCON_ERIE		\
				| SDP_UCON_CLKSEL  \
				| SDP_UCON_TXIRQMODE \
				| SDP_UCON_RXIRQMODE \
				| SDP_UCON_RXFIFO_TOI)

#define SDP_UFCON_FIFOMODE    (1<<0)
#define SDP_UFCON_TXTRIG0     (0<<6)
#define SDP_UFCON_TXTRIG1     (1<<6)
#define SDP_UFCON_TXTRIG12     (3<<6)
#define SDP_UFCON_RXTRIG8     (1<<4)
#define SDP_UFCON_RXTRIG12    (2<<4)
#define SDP_UFCON_RXTRIG16    (3<<4)
#define SDP_UFCON_RXTRIG4    (0<<4)
#define SDP_UFCON_TXFIFORESET    (1<<2)
#define SDP_UFCON_RXFIFORESET    (1<<1)

#define SDP_UFCON_DEFAULT   (SDP_UFCON_FIFOMODE | \
				 SDP_UFCON_TXTRIG0 | \
				 SDP_UFCON_RXTRIG4 | \
				 SDP_UFCON_TXFIFORESET | \
				 SDP_UFCON_RXFIFORESET )

#define SDP_UTRSTAT_CLEAR  (0xF0)
#define SDP_UTRSTAT_MI      (1<<7)
#define SDP_UTRSTAT_EI      (1<<6)
#define SDP_UTRSTAT_TXI      (1<<5)
#define SDP_UTRSTAT_RXI      (1<<4)
#define SDP_UTRSTAT_TXE	(1<<2)
#define SDP_UTRSTAT_TXFE      (1<<1)
#define SDP_UTRSTAT_RXDR      (1<<0)

#define SDP_UERSTAT_OE	(1 << 0)


#endif /* ASM_ARM_HARDWARE_SERIAL_SDP_H */


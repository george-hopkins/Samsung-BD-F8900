/*
 * sdp92_irq.h
 *
 * Copyright (c) 2009 Samsung Electronics
 * Ikjoon Jang <ij.jang@samsung.com>
 *
 */

#ifndef _SDP92_IRQS_H_
#define _SDP92_IRQS_H_

#define NR_IRQS			32

#define IRQ_TSD      		0
#define IRQ_AIO      		1
#define IRQ_AE       		2
#define IRQ_MPEG0    		3
#define IRQ_MPEG1    		4
#define IRQ_DISP     		5
#define IRQ_GA2D     		6
#define IRQ_GA3D     		7

#define IRQ_PCI 	   	8
#define IRQ_MFD     		9
#define IRQ_AVD     		10
#define IRQ_VROAD	   	11
#define IRQ_CSSYS 	   	12
#define IRQ_DMA      		13
#define IRQ_USB_EHCI0		14
#define IRQ_EMAC		15

#define IRQ_USB_EHCI1		16
#define IRQ_SE			17
#define IRQ_IRB_IRR   		18
#define IRQ_UART    		19
#define IRQ_USB_OHCI0    	20
#define IRQ_USB_OHCI1   	21
#define IRQ_SPI	   		22
#define IRQ_TIMER    		23

#define IRQ_I2C     		24
#define IRQ_SCI     		25
#define IRQ_DSP			26
#define IRQ_NAND      		27
#define IRQ_EXT0      		28
#define IRQ_EXT1     		29
#define IRQ_EXT2     		30
#define IRQ_SUBINT   		31	/* 2~6 */

#endif


/*
 * irqs-sdp1103fpga.h
 *
 * Copyright (c) 2011 Samsung Electronics
 * sh.chon <sh.chon@samsung.com>
 *
 */

#ifndef _IRQS_SDP1103FPGA_H_
#define _IRQS_SDP1103FPGA_H_

#define NR_IRQS			32

#define IRQ_TSD      		0
#define IRQ_AIO      		1
#define IRQ_AE       		2
#define IRQ_MPEG0    		3
#define IRQ_MPEG1    		4
#define IRQ_DISP     		5
#define IRQ_GA2D     		6
#define IRQ_GA3D     		7

#define IRQ_PCIE 	   		8
#define IRQ_MFD     		9
#define IRQ_AVD     		10
#define IRQ_FROAD	   		11
#define IRQ_CSSYS 	   		12
#define IRQ_CPUDMA     		13
#define IRQ_EHCI0			14
#define IRQ_EMAC			15

#define IRQ_EHCI1			16
#define IRQ_SE				17
#define IRQ_TSMUX   		18
#define IRQ_UART    		19
#define IRQ_OHCI0	    	20
#define IRQ_OHCI1		   	21
#define IRQ_HDMIRX 			22
#define IRQ_TIMER    		23

#define IRQ_I2C     		24
#define IRQ_SCI     		25
#define IRQ_DSP				26
#define IRQ_MMCIF      		27
#define IRQ_OSDP      		28
#define IRQ_MERGER0		   	29 /* IRQ_IRR, IRQ_RTC, IRQ_IRB, IRQ_SPI, IRQ_SMCDMA, IRQ_AMSDMA */
#define IRQ_MERGER1     	30 /* IRQ_EXT 0 ~ 7 */
#define IRQ_SUB   			31 /* IRQ_USB0_VBUSVALID, IRQ_USB1_VBUSVALID */

#endif


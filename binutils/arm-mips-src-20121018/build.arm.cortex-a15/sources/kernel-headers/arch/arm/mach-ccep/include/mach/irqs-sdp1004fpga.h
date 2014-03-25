/*
 * sdp1004_irq.h
 *
 * Copyright (c) 2010 Samsung Electronics
 * tukho.kim <tukho.kim@samsung.com>
 *
 */

#ifndef _SDP1004_FPGA_IRQS_H_
#define _SDP1004_FPGA_IRQS_H_


#if !defined(CONFIG_SDP_INTC64) 
#define NR_IRQS			32	

#define IRQ_TSD      		0
#define IRQ_AIO      		1
#define IRQ_BDGA       		2
#define IRQ_TIMER    		3
#define IRQ_PSD    		4
#define IRQ_SE     		5
#define IRQ_AE     		6
#define IRQ_HDMI     		7

#define IRQ_SATA0 	   	8
#define IRQ_SATA1_PCIE 		9
#define IRQ_RMI     		10
#define IRQ_RESERVED	   	11
#define IRQ_BDDP 	   	12
#define IRQ_USB_EHCI0  		13
#define IRQ_USB_EHCI1		14
#define IRQ_MFD			15

#define IRQ_EMAC		16
#define IRQ_SDMMC		17
#define IRQ_GDMA		18
#define IRQ_PL310    		19
#define IRQ_NFCON    		20
#define IRQ_SMC			21
#define IRQ_USB_OHCI0   	22
#define IRQ_USB_OHCI1   	23

#define IRQ_USB0_OVERCUR 	24
#define IRQ_USB1_OVERCUR 	25
#define IRQ_UART     		26
#define IRQ_SPI     		27
#define IRQ_I2C			28
#define IRQ_CSSYS      		29
#define IRQ_BUS      		30
#define IRQ_EXT   		31	/* 0 ~ 7 */

#else	/* irq resource 64*/
#define NR_IRQS			64

#define IRQ_TSD      		0
#define IRQ_AIO      		1
#define IRQ_BDGA       		2
#define IRQ_TIMER    		3
#define IRQ_PSD    		4
#define IRQ_SE     		5
#define IRQ_AE     		6
#define IRQ_HDMI     		7

#define IRQ_SATA0 	   	8
#define IRQ_SATA1_PCIE 		9
#define IRQ_GP     		10
#define IRQ_COP0	   	11
#define IRQ_BDDP 	   	12
#define IRQ_USB_EHCI0  		13
#define IRQ_USB_EHCI1		14
#define IRQ_MFD			15

#define IRQ_EMAC		16
#define IRQ_SDMMC		17
#define IRQ_GDMA		18
#define IRQ_HDMI_DDC    	19
#define IRQ_NFCON    		20
#define IRQ_COP1		21
#define IRQ_USB_OHCI0   	22
#define IRQ_USB_OHCI1   	23

#define IRQ_GA2D 		24
#define IRQ_GA3D 		25
#define IRQ_GSCL     		26
#define IRQ_RLD     		27
#define IRQ_PNG			28
#define IRQ_JPEG      		29
#define IRQ_CSC      		30
#define IRQ_SDP   		31	

#define IRQ_UART0		32
#define IRQ_UART1		33
#define IRQ_UART2		34
#define IRQ_UART3		35
#define IRQ_UDMA0		36
#define IRQ_UDMA1		37
#define IRQ_I2C1_4		38
#define IRQ_SPI			39

#define IRQ_SATA0_REDEF		40
#define IRQ_SATA1_PCIE_REDEF	41
#define IRQ_RMI			42
#define IRQ_RESERVED		43
#define IRQ_BUS			44
#define IRQ_CSSYS		45
#define IRQ_USB0_OVERCUR	46
#define IRQ_USB1_OVERCUR	47

#define IRQ_EMAC_REDEF		48
#define IRQ_SDMMC_REDEF		49
#define IRQ_GDMA_REDEF		50
#define IRQ_PL310		51
#define IRQ_NFCON_REDEF		52
#define IRQ_SMC			53
#define IRQ_USB_OHCI0_REDEF   	54
#define IRQ_USB_OHCI1_REDEF   	55

#define IRQ_EXTINT0		56
#define IRQ_EXTINT1		57
#define IRQ_EXTINT2		58
#define IRQ_EXTINT3		59
#define IRQ_EXTINT4		60
#define IRQ_EXTINT5		61
#define IRQ_EXTINT6		62
#define IRQ_EXTINT7		63

#endif /* CONFIG_SDP_INTC64 */


#endif /* _SDP1004_FPGA_IRQS_H_ */


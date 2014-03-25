/*
 * linux/include/asm-arm/arch-sdp/sdp_pcie.h
 *
 * Author: Samsung Electronics Co, tukho.kim@samsung.com
 *
 */

#ifndef __SDP_PCIE_H
#define __SDP_PCIE_H

/*	-------------------	*/
/* 	Define BASE Address 	*/
/*	-------------------	*/

#ifndef PA_PCIE_CDM_BASE
#  define PA_PCIE_CDM_BASE		(0xFFFF0000UL)	/* CDM Region 0x000 ~ 0x03E */
#  define PA_PCIE_ELBI_BASE		(0xFFFF8000UL)	/* ELBI Region 0x000 ~ 0x1A0 */
#  define PA_PCIE_PHY_BASE		(0x30030000UL)	/* PHY CMU Region 0x000 ~ 0x0FF */
#  define PA_PCIE_PHY_CMN_LANE_BLK_BASE	(0x30032800UL)	/* PHY CMU Region 0x000 ~ 0x0FF */
#endif /* P_PCIE_CDM_BASE */



#if 0
#define PCI_MEM_BAR0			0xC0000000
#define PCI_MEM_BAR1			0xD0000000 + 0x0
#define PCI_MEM_BAR2			0xD0000000 + 0x12000000
#endif

//Define physical address

#define AHB_PCI_CONFIG_TYPE0_BASE 	0xC0000000UL	// pci config0 base // only C0000000
#define AHB_PCI_CONFIG_TYPE0_SIZE 	(0x00001000UL)	// size 2MB
#define AHB_PCI_CONFIG_TYPE1_BASE 	0xC0008000UL	// pci config1 base // not support
#define AHB_PCI_CONFIG_TYPE1_SIZE 	(0x00001000UL)	// size 2MB

#define AHB_PCI_MEM0_BASE		0xE0000000UL	 
#define AHB_PCI_MEM0_SIZE		0x08000000UL	// size 128MB
#define AHB_PCI_MEM1_BASE		(AHB_PCI_MEM0_BASE+AHB_PCI_MEM0_SIZE)
#define AHB_PCI_MEM1_SIZE		0x08000000UL	// size 128MB

#define AHB_PCI_IO_BASE			0xF0000000UL	// pci io base	
#define AHB_PCI_IO_SIZE			(1 << 20) 	// size 4MB

                                                                                
#define AHB_PCI_MEM_BASE		AHB_PCI_MEM0_BASE
#define AHB_PCI_MEM_SIZE		(AHB_PCI_MEM0_SIZE + AHB_PCI_MEM1_SIZE)

#define PCI_MEM_ADDR			AHB_PCI_MEM_BASE
#define PCI_IO_ADDR			0x00000000

#define PCI_MEM_SIZE			AHB_PCI_MEM_SIZE	// mem region limit 64MB
#define PCI_IO_SIZE			AHB_PCI_IO_SIZE		// io region limit 4MB



/*	---------------------------	*/
/* 	Define Block Offset Address 	*/
/*	---------------------------	*/
#define OFFSET_PCI_PWM			(0x040)
#define OFFSET_PCI_MSI			(0x050)
#define OFFSET_PCI_PCIE			(0x070)
#define OFFSET_PCI_MSI_X		(0x0B0)		/* Not support SDP92 */
#define OFFSET_PCI_SLOT_ID		(0x0C0)		/* Not support SDP92 */
#define OFFSET_PCI_VPD			(0x0D0)		/* Not support SDP92 */
#define OFFSET_PCIE_EXT			(0x100)
#define OFFSET_PCIE_PORT_LOGIC		(0x700)

/*	---------------------------	*/
/* 	Define Block Offset PHY Address */
/*	---------------------------	*/
#define OFFSET_PCIE_PHY_CMU		(0x0000)
#define OFFSET_PCIE_PHY_LANE0		(0x0800)
#define OFFSET_PCIE_PHY_CMN_LANE_BLOCK	(0x2800)


/*	---------------		*/
/* 	Register Define 	*/
/*	---------------		*/
#ifdef __KERNEL__
#include <asm/types.h>
#else
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u8;
#endif

/* PCIE CDM Register 4KByte 0x1000 */
/* PCI-Compatible Header */
typedef volatile struct {
	volatile u16	vendorId;			/* 0x000 */
	volatile u16	deviceId;                       /* 0x002 */
	volatile u16	command; 	               	/* 0x004 */
	volatile u16	status;        		        /* 0x006 */
	volatile u16	revisionId;                     /* 0x008 */
	volatile u16	classCode;                      /* 0x00A */
	volatile u8	cacheLineSize;                  /* 0x00C */
	volatile u8	latencyTimer;                   /* 0x00D */
	volatile u8	headerType;                     /* 0x00E */
	volatile u8	bist;                           /* 0x00F */
	volatile u32	baseAddress0;		  	/* 0x010 */
	volatile u32	baseAddress1;           	/* 0x014 */
#if defined(CONFIG_SDP_PCIE)
	volatile u8	primaryBusNr;                   /* 0x018 */
	volatile u8	secondaryBusNr;                 /* 0x019 */
	volatile u8	subOrdinaryBusNr;               /* 0x01A */
	volatile u8	secondaryLatencyTimer;          /* 0x01B */
	volatile u8	ioBase;                         /* 0x01C */
	volatile u8	ioLimit;                        /* 0x01D */
	volatile u16	secondaryStatus;                /* 0x01E */
	volatile u16	memoryBase;                     /* 0x020 */
	volatile u16	memoryLimit;                    /* 0x022 */
	volatile u16	prefechableMemBase;             /* 0x024 */
	volatile u16	prefechableMemLimit;            /* 0x026 */
	volatile u32	prefechableBaseUp32bit;         /* 0x028 */
	volatile u32	prefechableLimitUp32bit;        /* 0x02C */
	volatile u16	ioBaseUp16bit;                  /* 0x030 */
	volatile u16	ioLimitUp16bit;                 /* 0x032 */
	volatile u8	capPtr;				/* 0x034 */
	volatile u8	reserved[3];			/* 0x035 */
	volatile u32	expansionRomBase;               /* 0x038 */
	volatile u8	interruptLine;                  /* 0x03C */
	volatile u8	interruptPin;                   /* 0x03D */
	volatile u16	bridgeControl;                  /* 0x03E */
#elif defined(CONFIG_SDP_PCIE_EP)
	volatile u32	baseAddress2;           	/* 0x018 */
	volatile u32	baseAddress3;           	/* 0x01C */
	volatile u32	baseAddress4;           	/* 0x020 */
	volatile u32	baseAddress5;           	/* 0x024 */
	volatile u32	cardBusCisPointer;           	/* 0x028 */
	volatile u16	subSystemVendorId;           	/* 0x02C */
	volatile u16	subSystemId;           		/* 0x02E */
	volatile u32	expansionRomBase;          	/* 0x030 */
	volatile u8	capPtr;          		/* 0x034 */
	volatile u8	reserved0[3];			/* 0x035 */
	volatile u32 	reserved1;			/* 0x038 */
	volatile u8	interruptLine;                  /* 0x03C */
	volatile u8	interruptPin;                   /* 0x03D */
	volatile u8	minGrant;                  	/* 0x03E */
	volatile u8	maxLatency;                  	/* 0x03F */
#endif
}PCI_HEADER_T;

/* PCI Power Management 0x40 ~ */
typedef volatile struct {
	volatile u8	capabilityId;			/* 0x040 */
	volatile u8	nextCapabilityPointer;		/* 0x041 */
	volatile u16	powerManagementCapability;	/* 0x042 */
	volatile u16	powerManagementControlStatus;	/* 0x044 */
	volatile u8	pmcsrBseBridgeExt;		/* 0x046 */
	volatile u8	pmcData;			/* 0x047 */
}PCI_PWM_T;

/* Message Signal Interrupt(MSI) 0x50 ~ */
typedef volatile struct {
	volatile u8 	capabilityId;			/* 0x050 */
	volatile u8	nextCapabilityPointer;		/* 0x051 */
	volatile u16	messageControl;			/* 0x052 */
	volatile u32	msiLower32bitAddress;		/* 0x054 */
	volatile u32	msiUpper32bitAddress;		/* 0x058 */
	volatile u16	msiData;			/* 0x05C */
}PCI_MSI_T;

/* PCI Express Capability 0x70 ~ */
typedef volatile struct {
	volatile u8 	capabilityId;			/* 0x070 */
	volatile u8	nextCapabilityPointer;		/* 0x071 */
	volatile u16	pcieCapability;			/* 0x072 */
	volatile u32	deviceCapabilities;		/* 0x074 */
	volatile u16	deviceControl;			/* 0x078 */
	volatile u16	deviceStatus;			/* 0x07A */
	volatile u32	linkCapabilities;		/* 0x07C */
	volatile u16	linkControl;			/* 0x080 */
	volatile u16	linkStatus;			/* 0x082 */
	volatile u32	slotCapabilities;		/* 0x084 */
	volatile u16	slotControl;			/* 0x088 */
	volatile u16	slotStatus;			/* 0x08A */
	volatile u32	rootCapabilities;		/* 0x08C */
	volatile u16	rootControl;			/* 0x090 */
	volatile u16	rootStatus;			/* 0x092 */
	volatile u32	deviceCapabilities2;		/* 0x094 */
	volatile u16	deviceControl2;			/* 0x098 */
	volatile u16	deviceStatus2;			/* 0x09A */
	volatile u32	linkCapabilities2;		/* 0x09C */
	volatile u16	linkControl2;			/* 0x0A0 */
	volatile u16	linkStatus2;			/* 0x0A2 */
	volatile u32	slotCapabilities2;		/* 0x0A4 */
	volatile u16	slotControl2;			/* 0x0A8 */
	volatile u16	slotStatus2;			/* 0x0AA */
}PCI_PCIE_T;

/* Not support MSI-X 0xB0 ~ */
typedef volatile struct {
	volatile u32	reserved[4];
} PCI_MSI_X_T;

/* Slot ID Capability 0xC0 ~ */
typedef volatile struct {
	volatile u32	reserved[4];
} PCI_SLOT_ID_T;

/* Not support VPD  0xD0 ~ */
typedef volatile struct {
	volatile u32	reserved[4];
} PCI_VPD_T;

/* PCIe Extended Capability Register  0x100 ~ */
typedef volatile struct {
	volatile u32 	pcieExtCapabilityHeader;	/* 0x100 */
	volatile u32 	uncorrectableErrorStaust;	/* 0x104 */
	volatile u32 	uncorrectableErrorMask;		/* 0x108 */
	volatile u32 	uncorrectableErrorSeverity;	/* 0x10C */
	volatile u32 	correctableErrorStatus;		/* 0x110 */
	volatile u32 	correctableErrorMask;		/* 0x114 */
	volatile u32 	advErrorCapaControl;		/* 0x118 */
	volatile u32	headerLog[4];			/* 0x11C */
	volatile u32	rootErrorCommand;		/* 0x12C */
	volatile u32	rootErrorStatus;		/* 0x130 */
	volatile u32	errorSourceIdentification;	/* 0x134 */
}PCIE_EXT_T;

/* PCIe Port Logic Register 0x700 ~ */
typedef volatile struct {
	volatile u32 	ackLatencyTimer_replayTimer;	/* 0x700 */
	volatile u32 	otherMessage;			/* 0x704 */
	volatile u32 	portForceLink;			/* 0x708 */
	volatile u32 	ackFrequency;			/* 0x70C */
	volatile u32 	portLinkControl;		/* 0x710 */
	volatile u32 	laneSkew;			/* 0x714 */
	volatile u32 	symbolNumber;			/* 0x718 */
	volatile u32 	symbolTimer_FilterMask;		/* 0x71C */
	volatile u32 	filterMask2;			/* 0x720 */
	volatile u32 	reserved0;			/* 0x724 */
	volatile u32	debug0;				/* 0x728 */
	volatile u32	debug1;				/* 0x72C */
	volatile u32	txPostedFCCreditStatus;		/* 0x730 */
	volatile u32	txNonPostedFCCreditStatus;	/* 0x734 */
	volatile u32	txCompletFCCreditStatus;	/* 0x738 */
	volatile u32	queueStatus;			/* 0x73C */
	volatile u32	vcTxArbitration1;		/* 0x740 */
	volatile u32	vcTxArbitration2;		/* 0x744 */
	volatile u32	vcoPostedRxQueueCtrl;		/* 0x748 */
	volatile u32	vcoNonPostedRxQueueCtrl;	/* 0x74C */
	volatile u32	vcoCompleteRxQueueCtrl;		/* 0x750 */
	volatile u32	reserved1[3];			/* 0x754 ~ 0x75C */
	volatile u32	reserved2[4];			/* 0x760 ~ 0x76C */
	volatile u32	reserved3[4];			/* 0x770 ~ 0x77C */
	volatile u32	reserved4[4];			/* 0x780 ~ 0x78C */
	volatile u32	reserved5[4];			/* 0x790 ~ 0x79C */
	volatile u32	reserved6[2];			/* 0x7A0 ~ 0x7A4 */
	volatile u32 	vcoPostedBufferDepth;		/* 0x7A8 */
	volatile u32 	vcoNonPostedBufferDepth;	/* 0x7AC */
	volatile u32 	vcoCompleteBufferDepth;		/* 0x7B0 */
	volatile u32	reserved7[3];			/* 0x7B4 ~ 0x7BC */
	volatile u32	reserved8[16];			/* 0x7C0, D, E, 0x7FC */
	volatile u32	reserved9[3];			/* 0x800 ~ 0x808 */
	volatile u32	gen2;				/* 0x80C */
	volatile u32	phyStatus;			/* 0x810 */
	volatile u32	phyControl;			/* 0x814 */
}PCIE_PORT_LOGIC_T;


/* ________________________ */
/* PCIE ELBI Register block */
typedef volatile struct {
/* PCIe->CPU region configuration register InBound on CPU*/
	volatile u32 	pim0MemAddrStartLow;		/* 0x000 */
	volatile u32 	pim0MemAddrStartHigh;           /* 0x004 */
	volatile u32 	pim1MemAddrStartLow;            /* 0x008 */
	volatile u32 	pim1MemAddrStartHigh;           /* 0x00C */
	volatile u32 	pim2MemAddrStartLow;            /* 0x010 */
	volatile u32 	pim2MemAddrStartHigh;           /* 0x014 */
	volatile u32 	pim3MemAddrStartLow;            /* 0x018 */
	volatile u32 	pim3MemAddrStartHigh;           /* 0x01C */
	volatile u32 	pimIoAddrStartLow;              /* 0x020 */
	volatile u32 	pimIoAddrStartHigh;             /* 0x024 */
	volatile u32 	pimRomAddrStartLow;             /* 0x028 */
	volatile u32 	pimRomAddrStartHigh;            /* 0x02C */
/* PCIe->CPU region configuration register InBound on CPU End*/

/* CPU->PCIe region configuration register Outbound on Device*/
	volatile u32 	pom0MemAddrStartLow;            /* 0x030 */
	volatile u32 	pom0MemAddrStartHigh;           /* 0x034 */
	volatile u32 	pom1MemAddrStartLow;            /* 0x038 */
	volatile u32 	pom1MemAddrStartHigh;           /* 0x03C */
	volatile u32 	pomIoAddrStartLow;              /* 0x040 */
	volatile u32 	pomIoAddrStartHigh;             /* 0x044 */
	volatile u32 	pom0Cfg0AddrStartLow;           /* 0x048 */
	volatile u32 	pom0Cfg0AddrStartHigh;          /* 0x04C */
	volatile u32 	pom1Cfg1AddrStartLow;           /* 0x050 */
	volatile u32 	pom1Cfg1AddrStartHigh;          /* 0x054 */
/* CPU->PCIe region configuration register Outbound on Device End*/

/* CPU->PCIe region configuration register Outbound on ARM*/
	volatile u32 	in0MemAddrStartLow;             /* 0x058 */
	volatile u32 	in0MemAddrStartHigh;            /* 0x05C */
	volatile u32 	in0MemAddrLimitLow;             /* 0x060 */
	volatile u32 	in0MemAddrLimitHigh;            /* 0x064 */
	volatile u32 	in1MemAddrStartLow;             /* 0x068 */
	volatile u32 	in1MemAddrStartHigh;            /* 0x06C */
	volatile u32 	in1MemAddrLimitLow;             /* 0x070 */
	volatile u32 	in1MemAddrLimitHigh;            /* 0x074 */
	volatile u32 	inIoAddrStartLow;               /* 0x078 */
	volatile u32 	inIoAddrStartHigh;              /* 0x07C */
	volatile u32 	inIoAddrLimitLow;               /* 0x080 */
	volatile u32 	inIoAddrLimitHigh;              /* 0x084 */
	volatile u32 	inCfg0AddrStartLow;             /* 0x088 */
	volatile u32 	inCfg0AddrStartHigh;            /* 0x08C */
	volatile u32 	inCfg0AddrLimitLow;             /* 0x090 */
	volatile u32 	inCfg0AddrLimitHigh;            /* 0x094 */
	volatile u32 	inCfg1AddrStartLow;             /* 0x098 */
	volatile u32 	inCfg1AddrStartHigh;            /* 0x09C */
	volatile u32 	inCfg1AddrLimitLow;             /* 0x0A0 */
	volatile u32 	inCfg1AddrLimitHigh;            /* 0x0A4 */
/* CPU->PCIe region configuration register Outbound on ARM End*/

/* PCIe->CPU region configuration register Inbound on Device*/
	volatile u32 	pin0MemAddrStartLow;            /* 0x0A8 */ 
	volatile u32 	pin0MemAddrStartHigh;           /* 0x0AC */
	volatile u32 	pin1MemAddrStartLow;            /* 0x0B0 */
	volatile u32 	pin1MemAddrStartHigh;           /* 0x0B4 */
	volatile u32 	pin2MemAddrStartLow;            /* 0x0B8 */
	volatile u32 	pin2MemAddrStartHigh;           /* 0x0BC */
	volatile u32 	pinIoAddrStartLow;              /* 0x0C0 */
	volatile u32 	pinIoAddrStartHigh;             /* 0x0C4 */
	volatile u32 	pin0MemAddrLimitLow;            /* 0x0C8 */
	volatile u32 	pin0MemAddrLimitHigh;           /* 0x0CC */
	volatile u32 	pin1MemAddrLimitLow;            /* 0x0D0 */
	volatile u32 	pin1MemAddrLimitHigh;           /* 0x0D4 */
	volatile u32 	pin2MemAddrLimitLow;            /* 0x0D8 */
	volatile u32 	pin2MemAddrLimitHigh;           /* 0x0DC */
	volatile u32 	pinIoAddrLimitLow;              /* 0x0E0 */
	volatile u32 	pinIoAddrLimitHigh;             /* 0x0E4 */
/* PCIe->CPU region configuration register Inbound on Device End*/

	volatile u32	mailBox0;                       /* 0x0E8 */
	volatile u32	mailBox1;                       /* 0x0EC */
	volatile u32	mailBox2;                       /* 0x0F0 */
	volatile u32	mailBox3;                       /* 0x0F4 */
	volatile u32	intMsi;                         /* 0x0F8 */
	volatile u32	doorBell;                       /* 0x0FC */
	volatile u32	userConfig0;			/* 0x100 */
	volatile u32	userConfig1;                    /* 0x104 */
	volatile u32	userConfig2;                    /* 0x108 */
	volatile u32	userConfig3;                    /* 0x10C */
	volatile u32	userConfig4;                    /* 0x110 */
	volatile u32	userConfig5;                    /* 0x114 */
	volatile u32	userConfig6;                    /* 0x118 */
	volatile u32	userConfig7;                    /* 0x11C */
	volatile u32	userConfig8;                    /* 0x120 ! interrupt status*/
	volatile u32	userConfig9;                    /* 0x124 ! interrupt mask */
	volatile u32	reserved0[2];			/* 0x128, 0x12C */
	volatile u32	userOnePulseConfig0;		/* 0x130 ! interrupt status*/
	volatile u32	userOnePulseConfig1;		/* 0x134 ! interrupt mask*/
	volatile u32	reserved1[2];			/* 0x138, 0x13C */
	volatile u32	mstrCtrl;			/* 0x140 */
	volatile u32	mstrBCtrl;			/* 0x144 */	
	volatile u32	mstrRCtrl;      		/* 0x148 */
	volatile u32	mstrAWCtrl;     		/* 0x14C */
	volatile u32	mstrARCtrl;     		/* 0x150 */
	volatile u32	slvCtrl;        		/* 0x154 */
	volatile u32	slvAWCtrl;      		/* 0x158 */
	volatile u32	slvARCtrl;      		/* 0x15C */
	volatile u32	slvBCtrl;       		/* 0x160 */
	volatile u32	slvRCtrl;       		/* 0x164 */
	volatile u32	reserved2[6];			/* 0x168 ~ 0x17C */
	volatile u32	userCtrl0;			/* 0x180 */
	volatile u32	userCtrl1;                      /* 0x184 */
	volatile u32	userCtrl2;                      /* 0x188 */
	volatile u32	userCtrl3;                      /* 0x18C */
	volatile u32	userCtrl4;                      /* 0x190 */
	volatile u32	reserved3[3];			/* 0x194 ~ 0x19C */
	volatile u32	userOnePulseCtrl0;		/* 0x1A0 */
}PCIE_ELBI_T;                                           

#define ELBI_SLVACTL_MEM	(0x80)
#define ELBI_SLVACTL_IO		(0x82)
#define ELBI_SLVACTL_CFG0	(0x84)
#define ELBI_SLVACTL_CFG1	(0x5)


typedef struct {
	/* valid 00 ~ 114, 0xEF, 0xFF */
	volatile u32	reg[255];
}PCIE_PHY_CMU_T;


typedef struct {
	volatile u32 	reg[81];
}PCIE_PHY_LANE0_T;


typedef struct {
	volatile u32 	reg[150];
}PCIE_PHY_CMN_LANE_BLOCK_T;


#define PCIE_ELBI_UCTRL0_APP_LTSSM_ENABLE 	(1 << 31)

#endif /* __PCIE_SDP_H */

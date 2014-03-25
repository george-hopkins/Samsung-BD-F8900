////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¨MStar Confidential Information〃) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#include <linux/platform_device.h>
#include "ehci.h"
#include "ehci-mstar.h"

#define MDrv_Timer_Delayms(x)  mdelay(x)

#ifdef CONFIG_MSTAR_EUCLID
#define INIT_MOVE2_BOOTLOADER
#endif

#if 0	//tony
static int ehci_mstar_reinit(struct ehci_hcd *ehci)
{
	int	retval;

	retval = ehci_init(ehci);
	ehci_port_power(ehci, 0);

	return retval;
}
#endif

#if 0
static const struct hc_driver ehci_driver = {
	.description =		hcd_name,

	/*
	 * generic hardware linkage
	 */
	.irq =			ehci_irq,
	.flags =		HCD_MEMORY | HCD_USB2,

	/*
	 * basic lifecycle operations
	 */
	.reset =		ehci_hc_reset,
	.start =		ehci_start,
#ifdef	CONFIG_PM
	.suspend =		ehci_suspend,
	.resume =		ehci_resume,
#endif
	.stop =			ehci_stop,

	/*
	 * memory lifecycle (except per-request)
	 */
	.hcd_alloc =		ehci_hcd_alloc,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue =		ehci_urb_enqueue,
	.urb_dequeue =		ehci_urb_dequeue,
	.endpoint_disable =	ehci_endpoint_disable,

	/*
	 * scheduling support
	 */
	.get_frame_number =	ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data =	ehci_hub_status_data,
	.hub_control =		ehci_hub_control,
	.hub_suspend =		ehci_hub_suspend,
	.hub_resume =		ehci_hub_resume,
};
#endif

static const struct hc_driver ehci_mstar_hc_driver = {
	.description = hcd_name,
	.product_desc = "Mstar EHCI",
	.hcd_priv_size = sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq = ehci_irq,
	.flags = HCD_MEMORY | HCD_USB2,

	/*
	 * basic lifecycle operations
	 *
	 * FIXME -- ehci_init() doesn't do enough here.
	 * See ehci-ppc-soc for a complete implementation.
	 */
#if 1	//tony
	.reset = ehci_init,
#else
	.reset = ehci_mstar_reinit,
#endif
	.start = ehci_run,
	.stop = ehci_stop,
	.shutdown = ehci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue = ehci_urb_enqueue,
	.urb_dequeue = ehci_urb_dequeue,
	.endpoint_disable = ehci_endpoint_disable,

	/*
	 * scheduling support
	 */
	.get_frame_number = ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data = ehci_hub_status_data,
	.hub_control = ehci_hub_control,
	.bus_suspend = ehci_bus_suspend,
	.bus_resume = ehci_bus_resume,
	.relinquish_port = ehci_relinquish_port,
	.port_handed_over = ehci_port_handed_over,

	//Colin, 101106 for external hub
	.clear_tt_buffer_complete	= ehci_clear_tt_buffer_complete,
};

#ifdef CONFIG_Triton
static void uranus_start_ehc(struct platform_device *dev)
{
	u8 tmp=0;
	//===========================================================================
	//writeb(0x00,0xbf804b80);
	//writeb(0x00,0xbf804b80);
	//XBYTE[0x3aac]|=0x04;							//Select current switch from DVI
	//writeb(readb(0xbf807558)|0x04,0xbf807558);
	//writeb(readb(0xbf806258)|0x04,0xbf806258);	//bit2=0
	//XBYTE[0x3a86]|=0x04;							//HSTXIEN
	//writeb(readb(0xbf80750c)|0x04,0xbf80750c);
	writeb(readb(0xbf80620c)|0x04,0xbf80620c);		//
	//XBYTE[0x3a80]=0x0;							//disable power down over write
	//XBYTE[0x3a81]=0x0;							//disable power down over write high byte
	//writeb(0x00,0xbf807500);
	writeb(0x00,0xbf806200);
	//writeb(0x00,0xbf807501);
	writeb(0x00,0xbf806201);

	//XBYTE[0x0700]|=0x28;							//disable init suspend state
	writeb(readb(0xbf805e00)|0x28,0xbf805e00);		//usbc disable init suspend state
	//XBYTE[0x0702]|=0x01;					  		//UHC select enable
	writeb(readb(0xbf805e04)|0x01,0xbf805e04);		//usbc UHC select enable

	//XBYTE[0x2434] |= 0x40;
	//writeb(readb(0xbf804868)|0x40,0xbf804868);
	writeb(readb(0xbf805c68)|0x40,0xbf805c68);
	//MDrv_Timer_Delayms(2);
	mdelay(2);
	//XBYTE[0x2440] &=~0x10;						//EHCI Vbsu turn on
	//writeb(readb(0xbf804880)&~0x10,0xbf804880);
	writeb(readb(0xbf805c80)&~0x10,0xbf805c80);
	mdelay(2);
	//===========================================================================
	//UHC_XBYTE(0x34)&=0xBF;						//set suspend
	//writeb(readb(0xbf804868)&0xbf,0xbf804868);
	writeb(readb(0xbf805c68)&0xbf,0xbf805c68);
	//UHC_XBYTE(0x34)|=0x40;						//clr suspend
	//writeb(readb(0xbf804868)|0x40,0xbf804868);
	writeb(readb(0xbf805c68)|0x40,0xbf805c68);
	//MDrv_Timer_Delayms(2);
	mdelay(2);
	//XBYTE[UTMIBaseAddr+0x00]|=0x01;			// override mode enable for power down control
	//writeb(readb(0xbf807500)|0x01,0xbf807500);
	writeb(readb(0xbf806200)|0x01,0xbf806200);
	//XBYTE[UTMIBaseAddr+0x01]|=0x40;			// enable IREF power down
	//writeb(readb(0xbf807501)|0x40,0xbf807501);
	writeb(readb(0xbf806201)|0x40,0xbf806201);
	//XBYTE[UTMIBaseAddr+0x01]|=0x02;			// enable PLL power down
	//writeb(readb(0xbf807501)|0x02,0xbf807501);
	writeb(readb(0xbf806201)|0x02,0xbf806201);
	//XBYTE[UTMIBaseAddr+0x01]&=0xFD;			// disable PLL power down
	//writeb(readb(0xbf807501)&0xfd,0xbf807501);
	writeb(readb(0xbf806201)&0xfd,0xbf806201);

	writeb(readb(0xbf805c20)|0x01,0xbf805c20);	//HC Reset
	mdelay(1);
	//writeb(readb(0xbf807500)&0xfe,0xbf807500);//disable override mode for power down control
	writeb(readb(0xbf806200)&0xfe,0xbf806200);	//disable override mode for power down control
	//===========================================================================
	//writeb(readb(0xbf80750c)|0x44,0xbf80750c);//Force HS TX current enable and CDR stage select
	writeb(readb(0xbf80620c)|0x44,0xbf80620c); 	//Force HS TX current enable and CDR stage select
	//writeb(readb(0xbf80750c)|0x03,0xbf80750c);//reset UTMI
	writeb(readb(0xbf80620c)|0x03,0xbf80620c);	//reset UTMI
	//writeb(readb(0xbf80750c)&0xfc,0xbf80750c);
	writeb(readb(0xbf80620c)&0xfc,0xbf80620c);
	mdelay(1);
	//writeb(readb(0xbf807551)|0x08,0xbf807551);//disable full speed retime
	writeb(0x08,0xbf806251);					//disable full speed retime
	//writeb(0xa8,0xbf807505);
	writeb(0xa8,0xbf806205);
#if 1
	//XBYTE[UTMIBaseAddr+0x07]|=0x02;
	//writeb(readb(0xbf80750d)&0xfd,0xbf80750d);
	writeb(readb(0xbf80620d)&0xfd,0xbf80620d);
	//writeb(readb(0xbf807558)|0xc5,0xbf807558);
	writeb(readb(0xbf806258)|0xc1,0xbf806258);	//for triton
	//XBYTE[UTMIBaseAddr+0x2d]=0x3b;			//enable TX common mode,
	//writeb(0x3b,0xbf807559);
	writeb(0x3b,0xbf806259);
	//XBYTE[UTMIBaseAddr+0x2f]|=0x0e;			//preemsis
	//writeb(readb(0xbf80755d)|0x0e,0xbf80755d);
	writeb(readb(0xbf80625d)|0x0e,0xbf80625d);
#else
	//XBYTE[UTMIBaseAddr+0x2c]|=0x01;
	writeb(readb(0xbf807558)|0x01,0xbf807558);
	//XBYTE[UTMIBaseAddr+0x2d]=0x38;         	//disable TX common mode
	writeb(0x38,0xbf807559);
#endif
	//writeb(readb(0xbf807525)|0x02,0xbf807525);//ISI improvement
	writeb(readb(0xbf806225)|0x02,0xbf806225);	//ISI improvement
	//writeb(readb(0xbf807511)|0xe0,0xbf807511);//patch low tempture,FL meta issue and enable new FL RX engin
	writeb(readb(0xbf806211)|0xe0,0xbf806211);	//patch low tempture,FL meta issue and enable new FL RX engin
	//writeb(readb(0xbf80754d)&0xf3,0xbf80754d);
	writeb(readb(0xbf80624d)&0xf3,0xbf80624d);
	//writeb(readb(0xbf80754d)|0x08,0xbf80754d);
	writeb(readb(0xbf80624d)|0x08,0xbf80624d);
	//writeb(readb(0xbf807554)&0xf0,0xbf807554);
	writeb(readb(0xbf806254)&0xf0,0xbf806254);
	//writeb(readb(0xbf807529)|0x20,0xbf807529);
	writeb(readb(0xbf806229)|0x20,0xbf806229);
	//writeb(readb(0xbf804880)&~0x10,0xbf804880);
	writeb(readb(0xbf805c80)&~0x10,0xbf805c80);

	//writeb(readb(0xbf804868)|0x08,0xbf804868);
	writeb(readb(0xbf805c68)|0x08,0xbf805c68);
	//writeb(readb(0xbf804868)&0xfb,0xbf804868);
	writeb(readb(0xbf805c68)&0xfb,0xbf805c68);
#if 0
	writeb(readb(0xbf804868) & 0xf3,0xbf804868);
#endif
	writeb(readb(0xbf806211) | 0x08,0xbf806211);//anti dead lock for HS
	tmp=readb(0xbf806258);
	//===========================================================================
}
#else

/*
 * 이걸 실행하지 않으면, H.264 채널에서 부팅시 no-signal 발생함.
 */
void uranus_disable_power_down(void)
{
	writeb(0x00,(void*)0xbf807500);								//disable power down over write
	writeb(0x00,(void*)0xbf807501);								//disable power down over write high byte
}

void IssueTestPacket(unsigned int UTMI_base, unsigned int USBC_base, unsigned int UHC_base)
{
	u8 *databuf, *testdata;
	size_t len=53,datreg32 = 53;
	u32 tempaddress;
	databuf = kmalloc(len, GFP_DMA);
	printk("databuf: 0x%x\n", (u32)databuf);
	testdata = ioremap_nocache(virt_to_phys((void*)databuf), len);
	printk("testdata: 0x%x\n", (u32)testdata);

	printk("----USB port send Test Packet (UHC: 0x%x)----\n", UHC_base);
	mdelay(500);
	*(testdata+0)=0x0;
  	*(testdata+1)=0x0;
	*(testdata+2)=0x0;
  	*(testdata+3)=0x0;
	*(testdata+4)=0x0;
	*(testdata+5)=0x0;
	*(testdata+6)=0x0;
	*(testdata+7)=0x0;
	*(testdata+8)=0x00;
	*(testdata+9)=0xaa;
	*(testdata+10)=0xaa;
	*(testdata+11)=0xaa;
	*(testdata+12)=0xaa;
	*(testdata+13)=0xaa;
	*(testdata+14)=0xaa;
	*(testdata+15)=0xaa;
	*(testdata+16)=0xaa;
	*(testdata+17)=0xee;
	*(testdata+18)=0xee;
	*(testdata+19)=0xee;
	*(testdata+20)=0xee;
	*(testdata+21)=0xee;
	*(testdata+22)=0xee;
	*(testdata+23)=0xee;
	*(testdata+24)=0xee;
	*(testdata+25)=0xfe;
	*(testdata+26)=0xff;
	*(testdata+27)=0xff;
	*(testdata+28)=0xff;
	*(testdata+29)=0xff;
	*(testdata+30)=0xff;
	*(testdata+31)=0xff;
	*(testdata+32)=0xff;
	*(testdata+33)=0xff;
	*(testdata+34)=0xff;
	*(testdata+35)=0xff;
	*(testdata+36)=0xff;
	*(testdata+37)=0x7f;
	*(testdata+38)=0xbf;
	*(testdata+39)=0xdf;
	*(testdata+40)=0xef;
	*(testdata+41)=0xf7;
	*(testdata+42)=0xfb;
	*(testdata+43)=0xfd;
	*(testdata+44)=0xfc;
	*(testdata+45)=0x7e;
	*(testdata+46)=0xbf;
	*(testdata+47)=0xdf;
	*(testdata+48)=0xfb;
	*(testdata+49)=0xfd;
	*(testdata+50)=0xfb;
	*(testdata+51)=0xfd;
	*(testdata+52)=0x7e;

	mdelay(500);

	//UHC_XBYTE(0x50)|=0x14; //enable test packet and lookback
	writeb(readb((void*)(UHC_base+0x50*2)) | 0x14, (void*) (UHC_base+0x50*2));
	//XBYTE[0x3a86]|=0x03; //TR/RX reset
	writeb(readb((void*)(UTMI_base+0x06*2)) | 0x03, (void*) (UTMI_base+0x06*2));
	//XBYTE[0x3a86]&=0xFC; //
	writeb(readb((void*)(UTMI_base+0x06*2)) & 0xFC, (void*) (UTMI_base+0x06*2));

	tempaddress = virt_to_phys((void*)databuf);
	//printk("tempaddress: %x\n", tempaddress);
	//printk("testdata address2:%x\n",tempaddress);
	mdelay(500);
	while(1)
	{
		//set DMA memory base address
		//UHC_XBYTE(0x74)=(U8)DMAAddress;
		writeb((u8)tempaddress,(void*) (UHC_base+0x74*2));
		//UHC_XBYTE(0x75)=(U8)(DMAAddress>>8);
		writeb((u8)(tempaddress>>8),(void*) (UHC_base+0x75*2-1));
		//UHC_XBYTE(0x76)=(U8)(DMAAddress>>16);
		writeb((u8)(tempaddress>>16),(void*) (UHC_base+0x76*2));
		//UHC_XBYTE(0x77)=(U8)(DMAAddress>>24);
		writeb((u8)(tempaddress>>24),(void*) (UHC_base+0x77*2-1));

		datreg32 = datreg32 << 8;
    		datreg32 = datreg32 | 0x02;

		//UHC_XBYTE(0x70)=(U8)datreg32;
		writeb((u8)datreg32,(void*) (UHC_base+0x70*2));
		//UHC_XBYTE(0x71)=(U8)(datreg32>>8);
		writeb((u8)(datreg32 >>8 ),(void*) (UHC_base+0x71*2-1));
		//UHC_XBYTE(0x72)=(U8)(datreg32>>16);
		writeb((u8)(datreg32 >>16 ),(void*) (UHC_base+0x72*2));
		//UHC_XBYTE(0x73)=(U8)(datreg32>>24);
		writeb((u8)(datreg32 >>24 ),(void*) (UHC_base+0x73*2-1));

		//UHC_XBYTE(0x70)|=0x01;//DMA start
		writeb(readb((void*) (UHC_base+0x70*2)) | 0x01,(void*) (UHC_base+0x70*2));

		while(!(readb((void*) (UHC_base+0x44*2)) & 0x08))
		{
		     ;//printk("2444:%x\n",readb((void*)0xbf804888));
		}
		mdelay(20);
	}

}

void uranus_start_ehc1(struct platform_device *dev, unsigned int flag)
{
	writeb(readb((void*)0xbf807525)|0x70,(void*)0xbf807525);	// add USB0 internal impedence, samuel,090129
	writeb(readb((void*)0xbf807425)|0x70,(void*)0xbf807425);	// add USB1 internal impedence, samuel,090130
	writeb(readb((void*)0xbf804b80)&0xEF,(void*)0xbf804b80);
	writeb(readb((void*)0xbf807558)|0x04,(void*)0xbf807558);	//Select current switch from DVI
	//writeb(readb((void*)0xbf80750c)|0x04,(void*)0xbf80750c);	//HSTXIEN
	writeb(0x00,(void*)0xbf807500);								//disable power down over write
	writeb(0x00,(void*)0xbf807501);								//disable power down over write high byte
	writeb(readb((void*)0xbf800e00)|0x28,(void*)0xbf800e00);	//disable init suspend state
	writeb(readb((void*)0xbf800e04)|0x01,(void*)0xbf800e04);	//UHC select enable
	writeb(readb((void*)0xbf800e00)|0x02,(void*)0xbf800e00);	//reset uhc registers
	mdelay(1);
	writeb(readb((void*)0xbf800e00)&0xfd,(void*)0xbf800e00);	//reset uhc registers
	writeb(readb((void*)0xbf804868)|0x40,(void*)0xbf804868);
	mdelay(2);

	if ( !(flag & EHCFLAG_TESTPKG) )
	{
	writeb((unsigned char)(readb((void*)0xbf804880)&~0x10),(void*)0xbf804880);	//EHCI Vbsu turn on
	}
	mdelay(2);
	writeb(readb((void*)0xbf804868)&0xbf,(void*)0xbf804868);	//set suspend
	writeb(readb((void*)0xbf804868)|0x40,(void*)0xbf804868);	//clr suspend
	mdelay(2);
	writeb(readb((void*)0xbf807500)|0x01,(void*)0xbf807500);	//override mode enable for power down control
	writeb(readb((void*)0xbf807501)|0x40,(void*)0xbf807501);	//enable IREF power down
	writeb(readb((void*)0xbf807501)|0x02,(void*)0xbf807501);	//enable PLL power down
	writeb(readb((void*)0xbf807501)&0xfd,(void*)0xbf807501);	//disable PLL power down
	mdelay(1);
	writeb(readb((void*)0xbf807500)&0xfe,(void*)0xbf807500);	//disable override mode for power down control
	writeb(readb((void*)0xbf80750c)&0xdb,(void*)0xbf80750c);	//Force HS TX current enable and CDR stage select
	writeb(readb((void*)0xbf80750c)|0x03,(void*)0xbf80750c);	//reset UTMI
	writeb(readb((void*)0xbf80750c)&0xfc,(void*)0xbf80750c);
	writeb(readb((void*)0xbf80750c)|0x44,(void*)0xbf80750c);//Force HS TX current enable and CDR stage select
	mdelay(1);
	writeb(readb((void*)0xbf807551)|0x08,(void*)0xbf807551);	//disable full speed retime
	//writeb(0xa8,(void*)0xbf807505);
	writeb(readb((void*)0xbf807505)|0x28,(void*)0xbf807505);
	writeb(readb((void*)0xbf807505)&0xef,(void*)0xbf807505);
	writeb(readb((void*)0xbf80750d)&0xfd,(void*)0xbf80750d);
#if 1 //tony	important for signal quality
	writeb(readb((void*)0xbf807558)&0xcd,(void*)0xbf807558);
	writeb(readb((void*)0xbf807558)|0xc5,(void*)0xbf807558);
	writeb(readb((void*)0xbf807559)|0x03,(void*)0xbf807559);//enable common mode voltage,don't follow wallace's document
	//writeb(readb((void*)0xbf807559)|0x01,(void*)0xbf807559);//enable common mode voltage,don't follow wallace's document
	writeb(readb((void*)0xbf80755d)|0x4a,(void*)0xbf80755d);
#endif
	//writeb(readb((void*)0xbf807525)|0x02,(void*)0xbf807525);	//ISI improvement
	writeb(readb((void*)0xbf807511)|0x81,(void*)0xbf807511);//patch low tempture,FL meta issue and enable new FL RX engin
	writeb(readb((void*)0xbf80754d)&0xfb,(void*)0xbf80754d);
	writeb(readb((void*)0xbf80754d)|0x08,(void*)0xbf80754d);
	writeb(readb((void*)0xbf807554)&0xf0,(void*)0xbf807554);
	writeb(readb((void*)0xbf807529)|0x20,(void*)0xbf807529);
	if ( !(flag & EHCFLAG_TESTPKG) )
	{
	writeb((unsigned char)(readb((void*)0xbf804880)&~0x10),(void*)0xbf804880);
	}
	//writeb(readb((void*)0xbf804868) & 0xf3,(void*)0xbf804868);
#if defined(CONFIG_MSTAR_TITANIA2_REV_U02)
	writeb(readb((void*)0xbf804901)|0x40,(void*)0xbf804901);	//byte-alignment and ehci active flag
#elif defined(CONFIG_MSTAR_TITANIA2_REV_U03)
	writeb(readb((void*)0xbf804901)|0xc0,(void*)0xbf804901);	//byte-alignment and ehci active flag
#else
	writeb(readb((void*)0xbf804901)|0xc0,(void*)0xbf804901);	//byte-alignment and ehci active flag
#endif
	writeb(readb((void*)0xbf804880)|0x08,(void*)0xbf804880);
	writeb(readb((void*)0xbf804901)|0xc0,(void*)0xbf804901);	//Enable MIU alignment & interrupt trans. short packet

	if ( flag & EHCFLAG_TESTPKG )
	{
		writeb(0x05,(void*)0xbf807558);
		writeb(0x00,(void*)0xbf80755c);
		writeb(readb((void*)0xbf804864)|0x01,(void*)0xbf804864);	//Force issue test packet
		IssueTestPacket(_MSTAR_UTMI0_BASE, _MSTAR_USBC0_BASE, _MSTAR_UHC0_BASE);
	}

}

//-------------------------------------------------------------------------------------------------------
//tony Second EHCI H.W Initial
void uranus_start_ehc2(struct platform_device *dev, unsigned int flag)
{
	writeb(readb((void*)0xbf807525)|0x70,(void*)0xbf807525);	// add USB0 internal impedence, samuel,090129
	writeb(readb((void*)0xbf807425)|0x70,(void*)0xbf807425);	// add USB1 internal impedence, samuel,090130
	writeb(readb((void*)0xbf804b80)&0xEF,(void*)0xbf804b80);
	writeb(readb((void*)0xbf807458)|0x04,(void*)0xbf807458);
	//writeb(readb((void*)0xbf80740c)|0x04,(void*)0xbf80740c);
	writeb(0x00,(void*)0xbf807400);
	writeb(0x00,(void*)0xbf807401);
	writeb(readb((void*)0xbf800f00)|0x28,(void*)0xbf800f00);
	writeb(readb((void*)0xbf800f04)|0x01,(void*)0xbf800f04);
#if 1
	writeb(readb((void*)0xbf800f00)|0x02,(void*)0xbf800f00);	//reset uhc registers
	mdelay(1);
	writeb(readb((void*)0xbf800f00)&0xfd,(void*)0xbf800f00);	//reset uhc registers
	writeb(readb((void*)0xbf801a68)|0x40,(void*)0xbf801a68);
#endif
	mdelay(2);
	if ( !(flag & EHCFLAG_TESTPKG) )
	{
		writeb((unsigned char)(readb((void*)0xbf801a80)&~0x10),(void*)0xbf801a80);	//EHCI Vbsu turn on
	}
	mdelay(2);
	writeb(readb((void*)0xbf801a68)&0xbf,(void*)0xbf801a68);
	writeb(readb((void*)0xbf801a68)|0x40,(void*)0xbf801a68);
	mdelay(2);
	writeb(readb((void*)0xbf807400)|0x01,(void*)0xbf807400);
	writeb(readb((void*)0xbf807401)|0x40,(void*)0xbf807401);
	writeb(readb((void*)0xbf807401)|0x02,(void*)0xbf807401);
	writeb(readb((void*)0xbf807401)&0xfd,(void*)0xbf807401);
	mdelay(1);
	writeb(readb((void*)0xbf807400)&0xfe,(void*)0xbf807400);	//disable override mode for power down control
	writeb(readb((void*)0xbf80740c)&0xdb,(void*)0xbf80740c);	//Force HS TX current enable and CDR stage select
	writeb(readb((void*)0xbf80740c)|0x03,(void*)0xbf80740c);	//reset UTMI
	writeb(readb((void*)0xbf80740c)&0xfc,(void*)0xbf80740c);
	writeb(readb((void*)0xbf80740c)|0x44,(void*)0xbf80740c);//Force HS TX current enable and CDR stage select
	//writeb(readb((void*)0xbf80740d)&0xfd,(void*)0xbf80740d);
	mdelay(1);
	writeb(readb((void*)0xbf807451)|0x08,(void*)0xbf807451);	//disable full speed retime
	writeb(readb((void*)0xbf807405) | 0x28,(void*)0xbf807405);
	writeb(readb((void*)0xbf807405) & 0xef,(void*)0xbf807405);
	writeb(readb((void*)0xbf80740d)&0xfd,(void*)0xbf80740d);
#if 1 //tony	important for signal quality
	writeb(readb((void*)0xbf807458)&0xcd,(void*)0xbf807458);
      writeb(readb((void*)0xbf807458)|0xc5,(void*)0xbf807458);
	//writeb(readb((void*)0xbf807459)|0x03,(void*)0xbf807459);	//enable common mode voltage,don't follow wallace's document
	writeb(readb((void*)0xbf807459)|0x03,(void*)0xbf807459);	//disbale common mode voltage,don't follow wallace's document //balup_081229_0x01 -> 0x03
	writeb(readb((void*)0xbf80745d)|0x4a,(void*)0xbf80745d);
#endif
	//writeb(readb((void*)0xbf807425)|0x02,(void*)0xbf807425);//ISI improvement
	writeb(readb((void*)0xbf807411) | 0x81,(void*)0xbf807411);	//patch low tempture,FL meta issue and enable new FL RX engin
	writeb(readb((void*)0xbf80744d) & 0xfb,(void*)0xbf80744d);
	writeb(readb((void*)0xbf80744d)|0x08,(void*)0xbf80744d);
	writeb(readb((void*)0xbf807454)&0xf0,(void*)0xbf807454);
	writeb(readb((void*)0xbf807429) | 0x20,(void*)0xbf807429);		//enable Chirp signal source select
	//writeb(readb((void*)0xbf807429) & 0xdf,(void*)0xbf807429);		// disable Chirp signal source select
	if ( !(flag & EHCFLAG_TESTPKG) )
	{
	writeb((unsigned char)(readb((void*)0xbf801a80)&~0x10),(void*)0xbf801a80);
	}
	//writeb(readb((void*)0xbf801a80) | 0x80,(void*)0xbf801a80);
	//writeb(readb((void*)0xbf801a68)|0x0c,(void*)0xbf801a68);
	//writeb(readb((void*)0xbf801a68)&0xf3,(void*)0xbf801a68);
#if defined(CONFIG_MSTAR_TITANIA2_REV_U02)
	writeb(readb((void*)0xbf801b01)|0x40,(void*)0xbf801b01);	//byte-alignment and ehci active flag
#elif defined(CONFIG_MSTAR_TITANIA2_REV_U03)
	writeb(readb((void*)0xbf801b01)|0xc0,(void*)0xbf801b01);	//byte-alignment and ehci active flag
#else
	writeb(readb((void*)0xbf801b01)|0xc0,(void*)0xbf801b01);	//byte-alignment and ehci active flag
#endif
	writeb(readb((void*)0xbf801a80)|0x08,(void*)0xbf801a80);
	writeb(readb((void*)0xbf801b01)|0xc0,(void*)0xbf801b01);	//Enable MIU alignment & interrupt trans. short packet
	if ( flag & EHCFLAG_TESTPKG )
	{
		writeb(0x05,(void*)0xbf807458);
		writeb(0x00,(void*)0xbf80745c);
		writeb(readb((void*)0xbf801a64)|0x01,(void*)0xbf801a64);	//Force issue test packet
		IssueTestPacket(_MSTAR_UTMI1_BASE, _MSTAR_USBC1_BASE, _MSTAR_UHC1_BASE);
	}

}
#endif



void MDrv_USB_WriteByte(unsigned int addr, unsigned char value)
{
    if(addr & 1)
        writeb(value, (void*)(_MSTAR_USB_BASEADR+((addr-1)<<1)+1));
    else
        writeb(value, (void*)(_MSTAR_USB_BASEADR+(addr<<1)));
}

unsigned char MDrv_USB_ReadByte(unsigned int addr)
{
    if(addr & 1)
        return readb((void*)(_MSTAR_USB_BASEADR+((addr-1)<<1)+1));
    else
        return readb((void*)(_MSTAR_USB_BASEADR+(addr<<1)));
}

void MDrv_USB_WriteRegBit(unsigned int addr, bool bEnable, unsigned char value)
{

    if (bEnable)
        MDrv_USB_WriteByte(addr, MDrv_USB_ReadByte(addr)|value);
    else
        MDrv_USB_WriteByte(addr, (unsigned char)(MDrv_USB_ReadByte(addr)&(~value)));

}


void euclid_start_ehc1(void)
{
    unsigned int UTMI0_Base=0x3A80;
    unsigned int USBC0_Base=0x0700;
    unsigned int UHC0_Base=0x2400;

    //printk("USB0_Init_Configure\n");

#ifndef INIT_MOVE2_BOOTLOADER
    printk("Move to boot loader\n");
    MDrv_USB_WriteRegBit(UTMI0_Base+0x2c, ENABLE, BIT2);  // switch to XIN_LV
    MDrv_USB_WriteRegBit(UTMI0_Base+0x20, DISABLE, BIT1); // switch to XIN_LV   //Add
    MDrv_USB_WriteRegBit(UTMI0_Base+0x20, ENABLE, BIT0);  // switch to XIN_LV
    MDrv_USB_WriteRegBit(UTMI0_Base+0x02, ENABLE, BIT2);  // switch to XIN_LV
#endif


    MDrv_USB_WriteRegBit(USBC0_Base+0x1, ENABLE, 0x2);  //Added by Jonas
    MDrv_USB_WriteByte(USBC0_Base+0x0, 0x0A);  // Disable MAC initial suspend, Reset UHC
    MDrv_Timer_Delayms(1);
    MDrv_USB_WriteByte(USBC0_Base+0x0, 0x68);  // Release UHC reset, enable UHC and OTG XIU function
    MDrv_Timer_Delayms(3);
    MDrv_USB_WriteRegBit(USBC0_Base+0x02, ENABLE, BIT0);  //UHC select enable //Add

#ifndef INIT_MOVE2_BOOTLOADER
    printk("Move to boot loader\n");
    MDrv_USB_WriteRegBit(UTMI0_Base+0x09, DISABLE, BIT3); // Disable force_pll_on
    MDrv_USB_WriteRegBit(UTMI0_Base+0x08, DISABLE, BIT7); // Enable band-gap current
    #if 1
    MDrv_USB_WriteByte(UTMI0_Base+0x01, 0x6B);  // Turn on reference voltage and regulator
    #else
    MDrv_USB_WriteRegBit(UTMI0_Base+0x01, ENABLE, 0x62);  // Turn on reference voltage and regulator
    #endif
    MDrv_USB_WriteByte(UTMI0_Base+0x00, 0xC3);  // reg_pdn: bit<15>, bit <2> ref_pdn
    MDrv_Timer_Delayms(1);
    #if 1
    MDrv_USB_WriteByte(UTMI0_Base+0x01, 0x69);  // Turn on UPLL, reg_pdn: bit<1>
    #else
    MDrv_USB_WriteRegBit(UTMI0_Base+0x01, DISABLE, BIT2);  // Turn on UPLL, reg_pdn: bit<1>
    #endif

    MDrv_Timer_Delayms(2);
    MDrv_USB_WriteByte(UTMI0_Base+0x00, 0x01);  // Turn all (including hs_current) use override mode
    MDrv_USB_WriteByte(UTMI0_Base+0x01, 0x00);
    MDrv_USB_WriteRegBit(UTMI0_Base+0x03, DISABLE, BIT7);
    //MDrv_WriteRegBit(UTMI0_Base+0x2c, ENABLE, BIT2);
#endif

    MDrv_USB_WriteRegBit(UHC0_Base+0x40, DISABLE, BIT4); //VBUS_ON
    MDrv_Timer_Delayms(2);
    //MDrv_USB_WriteRegBit(UHC0_Base+0x34, DISABLE, BIT6); //set suspend
    //MDrv_USB_WriteRegBit(UHC0_Base+0x34, ENABLE, BIT6);  //clr suspend
    //MDrv_Timer_Delayms(2);

    MDrv_USB_WriteRegBit(UHC0_Base+0x10, ENABLE, BIT1); //mbHost20_USBCMD_HCReset_Set();
    while(MDrv_USB_ReadByte(UHC0_Base+0x10)&BIT1);

	MDrv_USB_WriteRegBit(UHC0_Base+0x40, ENABLE, BIT3); // INT_POLARITY

    MDrv_USB_WriteRegBit(UTMI0_Base+0x06, ENABLE, BIT6);  //Force HS TX current enable and CDR stage select
    MDrv_USB_WriteRegBit(UTMI0_Base+0x06, DISABLE, BIT5); //clear bit 5
    MDrv_USB_WriteRegBit(UTMI0_Base+0x06, ENABLE, (BIT0|BIT1)); //reset UTMI
    MDrv_Timer_Delayms(2);

    MDrv_USB_WriteRegBit(UTMI0_Base+0x06, DISABLE, (BIT0|BIT1)); //UTMI_ANDXBYTE(0x06,0xfc);
    //MDrv_USB_WriteByte(UTMI0_Base+0x29, 0x08); //disable full speed retime
    MDrv_USB_WriteRegBit(UTMI0_Base+0x29, DISABLE, BIT3); //disable full speed retime
    MDrv_USB_WriteByte(UTMI0_Base+0x03, 0xa8); //for device disconnect status bit

    MDrv_USB_WriteRegBit(UTMI0_Base+0x2c, ENABLE, 0xc1);
    MDrv_USB_WriteRegBit(UTMI0_Base+0x2d, ENABLE, 0x03); //enable TX common mode,
    MDrv_USB_WriteRegBit(UTMI0_Base+0x2f, ENABLE, 0x4a); //preemsis

    MDrv_USB_WriteRegBit(UTMI0_Base+0x13, ENABLE, BIT1); //ISI improvement
    MDrv_USB_WriteRegBit(UTMI0_Base+0x0b, ENABLE, BIT7); //Euclid only //set reg_ck_inv_reserved[6] to solve timing problem

    MDrv_USB_WriteByte(UTMI0_Base+0x09, 0x81); //patch low tempture,FL meta issue and enable new FL RX engin

    MDrv_USB_WriteRegBit(UTMI0_Base+0x27, DISABLE, (BIT2|BIT3)); //UTMI_ANDXBYTE(0x27,0xf3);
    MDrv_USB_WriteRegBit(UTMI0_Base+0x27, ENABLE, BIT3); //(1) Offset 27 (‥h3AA7) bit <3:2> set 2…b10   // RX bias current => 60uA (default 40uA)

    MDrv_USB_WriteByte(UTMI0_Base+0x2a, 0x07); //UTMI_SETXBYTE(0x2a,0x07);
    MDrv_USB_WriteRegBit(UTMI0_Base+0x15, ENABLE, BIT5); //HOST CHIRP Detect

	MDrv_USB_WriteRegBit(UHC0_Base+0x81, ENABLE, (BIT7|BIT6)); //byte-alignment and ehci active flag

    MDrv_USB_WriteRegBit(UTMI0_Base+0x0, DISABLE, BIT0); //disable override mode for power down control
    MDrv_Timer_Delayms(2);//2ms

    //printk("\r\n USB0_Init_Configure end\n");



}

void euclid_start_ehc2(void)
{
    unsigned int UTMI1_Base=0x3A00;
    unsigned int USBC1_Base=0x0780;
    unsigned int UHC1_Base=0x0D00;

    //printk("\r\n USB1_Init_Configure");

    MDrv_USB_WriteRegBit(UTMI1_Base+0x2c, ENABLE, BIT2);  // switch to XIN_LV
    MDrv_USB_WriteRegBit(UTMI1_Base+0x20, DISABLE, BIT1); // switch to XIN_LV   //Add
    MDrv_USB_WriteRegBit(UTMI1_Base+0x20, ENABLE, BIT0);  // switch to XIN_LV
    MDrv_USB_WriteRegBit(UTMI1_Base+0x02, ENABLE, BIT2);  // switch to XIN_LV

    MDrv_USB_WriteRegBit(USBC1_Base+0x1, ENABLE, 0x2);  //Added by Jonas
    MDrv_USB_WriteByte(USBC1_Base+0x0, 0x0A);  // Disable MAC initial suspend, Reset UHC
    MDrv_Timer_Delayms(1);
    MDrv_USB_WriteByte(USBC1_Base+0x0, 0x68);  // Release UHC reset, enable UHC and OTG XIU function
    MDrv_Timer_Delayms(3);
    MDrv_USB_WriteRegBit(USBC1_Base+0x02, ENABLE, BIT0);  //UHC select enable //Add

    MDrv_USB_WriteRegBit(UTMI1_Base+0x09, DISABLE, BIT3); // Disable force_pll_on
    MDrv_USB_WriteRegBit(UTMI1_Base+0x08, DISABLE, BIT7); // Enable band-gap current
    #if 1
    MDrv_USB_WriteByte(UTMI1_Base+0x01, 0x6B);  // Turn on reference voltage and regulator
    #else
    MDrv_USB_WriteRegBit(UTMI1_Base+0x01, ENABLE, 0x62);  // Turn on reference voltage and regulator
    #endif
    MDrv_USB_WriteByte(UTMI1_Base+0x00, 0xC3);  // reg_pdn: bit<15>, bit <2> ref_pdn
    MDrv_Timer_Delayms(1);
    #if 1
    MDrv_USB_WriteByte(UTMI1_Base+0x01, 0x69);  // Turn on UPLL, reg_pdn: bit<1>
    #else
    MDrv_USB_WriteRegBit(UTMI1_Base+0x01, DISABLE, BIT2);  // Turn on UPLL, reg_pdn: bit<1>
    #endif
    MDrv_Timer_Delayms(2);
    MDrv_USB_WriteByte(UTMI1_Base+0x00, 0x01);  // Turn all (including hs_current) use override mode
    MDrv_USB_WriteByte(UTMI1_Base+0x01, 0x00);  //UTMI_SETXBYTE(0x1,0x0);
    MDrv_USB_WriteRegBit(UTMI1_Base+0x03, DISABLE, BIT7); //UTMI_ANDXBYTE(0x3,0x7F);
    //MDrv_WriteRegBit(UTMI1_Base+0x2c, ENABLE, BIT2);  //UTMI_ORXBYTE(0x2c,0x4);

    MDrv_USB_WriteRegBit(UHC1_Base+0x40, DISABLE, BIT4); //VBUS_ON
    MDrv_Timer_Delayms(2);
    //MDrv_USB_WriteRegBit(UHC1_Base+0x34, DISABLE, BIT6); //set suspend
    //MDrv_USB_WriteRegBit(UHC1_Base+0x34, ENABLE, BIT6);  //clr suspend
    //MDrv_Timer_Delayms(2);

    MDrv_Timer_Delayms(1);
    MDrv_USB_WriteRegBit(UHC1_Base+0x10, ENABLE, BIT1); //mbHost20_USBCMD_HCReset_Set();
    while(MDrv_USB_ReadByte(UHC1_Base+0x10)&BIT1);

	MDrv_USB_WriteRegBit(UHC1_Base+0x40, ENABLE, BIT3); // INT_POLARITY

    MDrv_USB_WriteRegBit(UTMI1_Base+0x06, ENABLE, BIT6);  //Force HS TX current enable and CDR stage select
    MDrv_USB_WriteRegBit(UTMI1_Base+0x06, DISABLE, BIT5); //clear bit 5
    MDrv_USB_WriteRegBit(UTMI1_Base+0x06, ENABLE, (BIT0|BIT1)); //reset UTMI
    MDrv_Timer_Delayms(2);
    MDrv_USB_WriteRegBit(UTMI1_Base+0x06, DISABLE, (BIT0|BIT1));
    //MDrv_USB_WriteByte(UTMI1_Base+0x29, 0x08); ///disable full speed retime
    MDrv_USB_WriteRegBit(UTMI1_Base+0x29, DISABLE, BIT3); //disable full speed retime
    MDrv_USB_WriteByte(UTMI1_Base+0x03, 0xa8); //for device disconnect status bit

    MDrv_USB_WriteRegBit(UTMI1_Base+0x2c, ENABLE, 0xc1);
    MDrv_USB_WriteRegBit(UTMI1_Base+0x2d, ENABLE, 0x03); //enable TX common mode,
    MDrv_USB_WriteRegBit(UTMI1_Base+0x2f, ENABLE, 0x4a); //preemsis

    MDrv_USB_WriteRegBit(UTMI1_Base+0x13, ENABLE, BIT1); //ISI improvement
    MDrv_USB_WriteRegBit(UTMI1_Base+0x0b, ENABLE, BIT7); //Euclid only //set reg_ck_inv_reserved[6] to solve timing problem

    MDrv_USB_WriteByte(UTMI1_Base+0x09, 0x81);   //patch low tempture,FL meta issue and enable new FL RX engin

    MDrv_USB_WriteRegBit(UTMI1_Base+0x27, DISABLE, (BIT2|BIT3)); //UTMI_ANDXBYTE(0x27,0xf3);
    MDrv_USB_WriteRegBit(UTMI1_Base+0x27, ENABLE, BIT3); //(1) Offset 27 (‥h3AA7) bit <3:2> set 2…b10   // RX bias current => 60uA (default 40uA)

    MDrv_USB_WriteByte(UTMI1_Base+0x2a, 0x07);
    MDrv_USB_WriteRegBit(UTMI1_Base+0x15, ENABLE, BIT5); //HOST CHIRP Detect

	MDrv_USB_WriteRegBit(UHC1_Base+0x81, ENABLE, (BIT7|BIT6)); //byte-alignment and ehci active flag

    MDrv_USB_WriteRegBit(UTMI1_Base+0x0, DISABLE, BIT0); //disable override mode for power down control
    MDrv_Timer_Delayms(2);//2ms

    //printk("\r\n USB1_Init_Configure end");



}

#if defined(CONFIG_MSTAR_TITANIA3) || defined(CONFIG_MSTAR_TITANIA8) || defined(CONFIG_MSTAR_TITANIA9) || defined(CONFIG_MSTAR_URANUS4) || defined(CONFIG_MSTAR_AMBER1) || defined(CONFIG_MSTAR_AMBER5)

static void Titania3_start_ehc1(void)
{
	printk("Titania3_later_start_ehc1 start\n");

	writeb(0x0a, (void*) (0xbf000000+0x100700*2)); // Disable MAC initial suspend, Reset UHC
	writeb(0x28, (void*) (0xbf000000+0x100700*2)); // Release UHC reset, enable UHC and OTG XIU function

    ///--- Removed for A1 & A5 ---
	// writeb(readb((void*)0xbf000000+(0x103a80+0x22)*2) | 0xE0, (void*) (0xbf000000+(0x103a80+0x22)*2)); // Set PLL_TEST[23:21] for enable 480MHz clock
	// writeb(readb((void*)0xbf000000+(0x103a80+0x20)*2) | 0x02, (void*) (0xbf000000+(0x103a80+0x20)*2)); // Set PLL_TEST[1] for PLL multiplier 20X
	// ----------------------------
	writeb(readb((void*)0xbf000000+(0x103a80+0x09)*2-1) & ~0x08, (void*) (0xbf000000+(0x103a80+0x09)*2-1)); // Disable force_pll_on
	writeb(readb((void*)0xbf000000+(0x103a80+0x08)*2) & ~0x80, (void*) (0xbf000000+(0x103a80+0x08)*2)); // Enable band-gap current
	writeb(0xC3, (void*) (0xbf000000+0x103a80*2)); // reg_pdn: bit<15>, bit <2> ref_pdn
	mdelay(1);	// delay 1ms

	writeb(0x69, (void*) (0xbf000000+(0x103a80+0x01)*2-1)); // Turn on UPLL, reg_pdn: bit<9>
	mdelay(2);	// delay 2ms

	writeb(0x01, (void*) (0xbf000000+0x103a80*2)); // Turn all (including hs_current) use override mode
	writeb(0, (void*) (0xbf000000+(0x103a80+0x01)*2-1)); // Turn on UPLL, reg_pdn: bit<9>

	writeb(0x0A, (void*) (0xbf000000+0x100700*2)); // Disable MAC initial suspend, Reset UHC
	writeb(0x28, (void*) (0xbf000000+0x100700*2)); // Release UHC reset, enable UHC XIU function

	writeb(readb((void*)0xbf000000+(0x103A80+0x3C)*2) | 0x01, (void*) (0xbf000000+(0x103A80+0x3C)*2)); // set CA_START as 1
	mdelay(10);

	writeb(readb((void*)0xbf000000+(0x103A80+0x3C)*2) & ~0x01, (void*) (0xbf000000+(0x103A80+0x3C)*2)); // release CA_START

	while ((readb((void*)0xbf000000+(0x103A80+0x3C)*2) & 0x02) == 0);	// polling bit <1> (CA_END)

	writeb(readb((void*)0xbf000000+(0x100700+0x02)*2) & ~0x03, (void*) (0xbf000000+(0x100700+0x02)*2)); //UHC select enable
	writeb(readb((void*)0xbf000000+(0x100700+0x02)*2) | 0x01, (void*) (0xbf000000+(0x100700+0x02)*2)); //UHC select enable

	writeb(readb((void*)0xbf000000+(0x102400+0x40)*2) & ~0x10, (void*) (0xbf000000+(0x102400+0x40)*2)); //0: VBUS On.
	mdelay(1);	// delay 1ms

	writeb(readb((void*)0xbf000000+(0x102400+0x40)*2) | 0x08, (void*) (0xbf000000+(0x102400+0x40)*2)); // Active HIGH
	mdelay(1);	// delay 1ms

	writeb(readb((void*)0xbf000000+(0x102400+0x81)*2-1) | 0x8F, (void*) (0xbf000000+(0x102400+0x81)*2-1)); //improve the efficiency of USB access MIU when system is busy
    
	writeb(readb((void*)0xbf000000+(0x103a80+0x06)*2) | 0x40, (void*) (0xbf000000+(0x103a80+0x06)*2)); //reg_tx_force_hs_current_enable
	writeb(readb((void*)0xbf000000+(0x103a80+0x06)*2) & ~0x20, (void*) (0xbf000000+(0x103a80+0x06)*2)); //reg_tx_force_hs_current_enable

	writeb(readb((void*)0xbf000000+(0x103a80+0x03)*2-1) | 0x28, (void*) (0xbf000000+(0x103a80+0x03)*2-1)); //Disconnect window select
	writeb(readb((void*)0xbf000000+(0x103a80+0x03)*2-1) & 0xef, (void*) (0xbf000000+(0x103a80+0x03)*2-1)); //Disconnect window select

	writeb(readb((void*)0xbf000000+(0x103a80+0x07)*2-1) & 0xfd, (void*) (0xbf000000+(0x103a80+0x07)*2-1)); //Disable improved CDR
	writeb(readb((void*)0xbf000000+(0x103a80+0x09)*2-1) |0x81, (void*) (0xbf000000+(0x103a80+0x09)*2-1)); // UTMI RX anti-dead-loc, ISI effect improvement
	writeb(readb((void*)0xbf000000+(0x103a80+0x0b)*2-1) |0x80, (void*) (0xbf000000+(0x103a80+0x0b)*2-1)); // TX timing select latch path
	writeb(readb((void*)0xbf000000+(0x103a80+0x15)*2-1) |0x20, (void*) (0xbf000000+(0x103a80+0x15)*2-1)); // Chirp signal source select

    #if 1
	writeb(readb((void*)0xbf000000+(0x103a80+0x2c)*2) |0x98, (void*) (0xbf000000+(0x103a80+0x2c)*2));
	writeb(readb((void*)0xbf000000+(0x103a80+0x2d)*2-1) |0x02, (void*) (0xbf000000+(0x103a80+0x2d)*2-1));
	writeb(readb((void*)0xbf000000+(0x103a80+0x2e)*2) |0x10, (void*) (0xbf000000+(0x103a80+0x2e)*2));
	writeb(readb((void*)0xbf000000+(0x103a80+0x2f)*2-1) |0x01, (void*) (0xbf000000+(0x103a80+0x2f)*2-1));
    #else
	writeb(readb((void*)0xbf000000+(0x103a80+0x2c)*2) |0x10, (void*) (0xbf000000+(0x103a80+0x2c)*2));
	writeb(readb((void*)0xbf000000+(0x103a80+0x2d)*2-1) |0x02, (void*) (0xbf000000+(0x103a80+0x2d)*2-1));
	writeb(readb((void*)0xbf000000+(0x103a80+0x2f)*2-1) |0x81, (void*) (0xbf000000+(0x103a80+0x2f)*2-1));
    #endif
    
}

void Titania3_start_ehc2(void)
{
	printk("Titania3_later_start_ehc2 start\n");

	writeb(0x0a, (void*) (0xbf000000+0x100780*2)); // Disable MAC initial suspend, Reset UHC
	writeb(0x28, (void*) (0xbf000000+0x100780*2)); // Release UHC reset, enable UHC and OTG XIU function

    ///--- Removed for A1 & A5 ---
	//writeb(readb((void*)0xbf000000+(0x103a00+0x22)*2) | 0xE0, (void*) (0xbf000000+(0x103a00+0x22)*2)); // Set PLL_TEST[23:21] for enable 480MHz clock
	//writeb(readb((void*)0xbf000000+(0x103a00+0x20)*2) | 0x02, (void*) (0xbf000000+(0x103a00+0x20)*2)); // Set PLL_TEST[1] for PLL multiplier 20X
	//----------------------------
	writeb(readb((void*)0xbf000000+(0x103a00+0x09)*2-1) & ~0x08, (void*) (0xbf000000+(0x103a00+0x09)*2-1)); // Disable force_pll_on
	writeb(readb((void*)0xbf000000+(0x103a00+0x08)*2) & ~0x80, (void*) (0xbf000000+(0x103a00+0x08)*2)); // Enable band-gap current
	writeb(0xC3, (void*) (0xbf000000+0x103a00*2)); // reg_pdn: bit<15>, bit <2> ref_pdn
	mdelay(1);	// delay 1ms

	writeb(0x69, (void*) (0xbf000000+(0x103a00+0x01)*2-1)); // Turn on UPLL, reg_pdn: bit<9>
	mdelay(2);	// delay 2ms

	writeb(0x01, (void*) (0xbf000000+0x103a00*2)); // Turn all (including hs_current) use override mode
	writeb(0, (void*) (0xbf000000+(0x103a00+0x01)*2-1)); // Turn on UPLL, reg_pdn: bit<9>

	writeb(0x0A, (void*) (0xbf000000+0x100780*2)); // Disable MAC initial suspend, Reset UHC
	writeb(0x28, (void*) (0xbf000000+0x100780*2)); // Release UHC reset, enable UHC XIU function

	writeb(readb((void*)0xbf000000+(0x103a00+0x3C)*2) | 0x01, (void*) (0xbf000000+(0x103a00+0x3C)*2)); // set CA_START as 1
	mdelay(10);

	writeb(readb((void*)0xbf000000+(0x103a00+0x3C)*2) & ~0x01, (void*) (0xbf000000+(0x103a00+0x3C)*2)); // release CA_START

	while ((readb((void*)0xbf000000+(0x103a00+0x3C)*2) & 0x02) == 0);	// polling bit <1> (CA_END)

	writeb(readb((void*)0xbf000000+(0x100780+0x02)*2) & ~0x03, (void*) (0xbf000000+(0x100780+0x02)*2)); //UHC select enable
	writeb(readb((void*)0xbf000000+(0x100780+0x02)*2) | 0x01, (void*) (0xbf000000+(0x100780+0x02)*2)); //UHC select enable

	writeb(readb((void*)0xbf000000+(0x100D00+0x40)*2) & ~0x10, (void*) (0xbf000000+(0x100D00+0x40)*2)); //0: VBUS On.
	mdelay(1);	// delay 1ms

	writeb(readb((void*)0xbf000000+(0x100D00+0x40)*2) | 0x08, (void*) (0xbf000000+(0x100D00+0x40)*2)); // Active HIGH
	mdelay(1);	// delay 1ms

	writeb(readb((void*)0xbf000000+(0x100D00+0x81)*2-1) | 0x8F, (void*) (0xbf000000+(0x100D00+0x81)*2-1)); //improve the efficiency of USB access MIU when system is busy

	writeb(readb((void*)0xbf000000+(0x103a00+0x06)*2) | 0x40, (void*) (0xbf000000+(0x103a00+0x06)*2)); //reg_tx_force_hs_current_enable
	writeb(readb((void*)0xbf000000+(0x103a00+0x06)*2) & ~0x20, (void*) (0xbf000000+(0x103a00+0x06)*2)); //reg_tx_force_hs_current_enable

	writeb(readb((void*)0xbf000000+(0x103a00+0x03)*2-1) | 0x28, (void*) (0xbf000000+(0x103a00+0x03)*2-1)); //Disconnect window select
	writeb(readb((void*)0xbf000000+(0x103a00+0x03)*2-1) & 0xef, (void*) (0xbf000000+(0x103a00+0x03)*2-1)); //Disconnect window select

	writeb(readb((void*)0xbf000000+(0x103a00+0x07)*2-1) & 0xfd, (void*) (0xbf000000+(0x103a00+0x07)*2-1)); //Disable improved CDR
	writeb(readb((void*)0xbf000000+(0x103a00+0x09)*2-1) |0x81, (void*) (0xbf000000+(0x103a00+0x09)*2-1)); // UTMI RX anti-dead-loc, ISI effect improvement
	writeb(readb((void*)0xbf000000+(0x103a00+0x0b)*2-1) |0x80, (void*) (0xbf000000+(0x103a00+0x0b)*2-1)); // TX timing select latch path
	writeb(readb((void*)0xbf000000+(0x103a00+0x15)*2-1) |0x20, (void*) (0xbf000000+(0x103a00+0x15)*2-1)); // Chirp signal source select

    #if 1
	writeb(readb((void*)0xbf000000+(0x103a00+0x2c)*2) |0x98, (void*) (0xbf000000+(0x103a00+0x2c)*2));
	writeb(readb((void*)0xbf000000+(0x103a00+0x2d)*2-1) |0x02, (void*) (0xbf000000+(0x103a00+0x2d)*2-1));
	writeb(readb((void*)0xbf000000+(0x103a00+0x2e)*2) |0x10, (void*) (0xbf000000+(0x103a00+0x2e)*2));
	writeb(readb((void*)0xbf000000+(0x103a00+0x2f)*2-1) |0x01, (void*) (0xbf000000+(0x103a00+0x2f)*2-1));
    #else
	writeb(readb((void*)0xbf000000+(0x103a00+0x2c)*2) |0x10, (void*) (0xbf000000+(0x103a00+0x2c)*2));
	writeb(readb((void*)0xbf000000+(0x103a00+0x2d)*2-1) |0x02, (void*) (0xbf000000+(0x103a00+0x2d)*2-1));
	writeb(readb((void*)0xbf000000+(0x103a00+0x2f)*2-1) |0x81, (void*) (0xbf000000+(0x103a00+0x2f)*2-1));
    #endif
    
}

//-----------------------------------------
void Titania3_series_start_ehc(unsigned int UTMI_base, unsigned int USBC_base, unsigned int UHC_base, unsigned int flag)
{
	printk("Titania3_series_start_ehc start\n");

    if (flag & EHCFLAG_TESTPKG)
    {
	    writew(0x2084, (void*) (0xbf000000+(UTMI_base+0x2)*2)); //
        #if defined(CONFIG_MSTAR_AMBER1) || defined(CONFIG_MSTAR_AMBER5)
	    	writew(0x8051, (void*) (0xbf000000+(UTMI_base+0x20)*2)); //
        #else
	    writew(0x0003, (void*) (0xbf000000+(UTMI_base+0x20)*2)); //
        #endif 
    }

	writeb(0x0a, (void*) (0xbf000000+USBC_base*2)); // Disable MAC initial suspend, Reset UHC
	writeb(0x28, (void*) (0xbf000000+USBC_base*2)); // Release UHC reset, enable UHC and OTG XIU function

	writeb(readb((void*)0xbf000000+(UTMI_base+0x09)*2-1) & ~0x08, (void*) (0xbf000000+(UTMI_base+0x09)*2-1)); // Disable force_pll_on
	writeb(readb((void*)0xbf000000+(UTMI_base+0x08)*2) & ~0x80, (void*) (0xbf000000+(UTMI_base+0x08)*2)); // Enable band-gap current
	writeb(0xC3, (void*) (0xbf000000+UTMI_base*2)); // reg_pdn: bit<15>, bit <2> ref_pdn
	mdelay(1);	// delay 1ms

	writeb(0x69, (void*) (0xbf000000+(UTMI_base+0x01)*2-1)); // Turn on UPLL, reg_pdn: bit<9>
	mdelay(2);	// delay 2ms

	writeb(0x01, (void*) (0xbf000000+UTMI_base*2)); // Turn all (including hs_current) use override mode
	writeb(0, (void*) (0xbf000000+(UTMI_base+0x01)*2-1)); // Turn on UPLL, reg_pdn: bit<9>

	writeb(0x0A, (void*) (0xbf000000+USBC_base*2)); // Disable MAC initial suspend, Reset UHC
	writeb(0x28, (void*) (0xbf000000+USBC_base*2)); // Release UHC reset, enable UHC XIU function

	writeb(readb((void*)0xbf000000+(UTMI_base+0x3C)*2) | 0x01, (void*) (0xbf000000+(UTMI_base+0x3C)*2)); // set CA_START as 1
	mdelay(10);

	writeb(readb((void*)0xbf000000+(UTMI_base+0x3C)*2) & ~0x01, (void*) (0xbf000000+(UTMI_base+0x3C)*2)); // release CA_START

	while ((readb((void*)0xbf000000+(UTMI_base+0x3C)*2) & 0x02) == 0);	// polling bit <1> (CA_END)

    if (flag & EHCFLAG_DPDM_SWAP)
    	writeb(readb((void*)0xbf000000+(UTMI_base+0x0b)*2-1) |0x20, (void*) (0xbf000000+(UTMI_base+0x0b)*2-1)); // dp dm swap

	writeb(readb((void*)0xbf000000+(USBC_base+0x02)*2) & ~0x03, (void*) (0xbf000000+(USBC_base+0x02)*2)); //UHC select enable
	writeb(readb((void*)0xbf000000+(USBC_base+0x02)*2) | 0x01, (void*) (0xbf000000+(USBC_base+0x02)*2)); //UHC select enable

	writeb(readb((void*)0xbf000000+(UHC_base+0x40)*2) & ~0x10, (void*) (0xbf000000+(UHC_base+0x40)*2)); //0: VBUS On.
	mdelay(1);	// delay 1ms

	writeb(readb((void*)0xbf000000+(UHC_base+0x40)*2) | 0x08, (void*) (0xbf000000+(UHC_base+0x40)*2)); // Active HIGH
	mdelay(1);	// delay 1ms

	writeb(readb((void*)0xbf000000+(UHC_base+0x81)*2-1) | 0x8F, (void*) (0xbf000000+(UHC_base+0x81)*2-1)); //improve the efficiency of USB access MIU when system is busy

	writeb((readb((void*)0xbf000000+(UTMI_base+0x06)*2) & 0x9F) | 0x40, (void*) (0xbf000000+(UTMI_base+0x06)*2)); //reg_tx_force_hs_current_enable

	writeb(readb((void*)0xbf000000+(UTMI_base+0x03)*2-1) | 0x28, (void*) (0xbf000000+(UTMI_base+0x03)*2-1)); //Disconnect window select
	writeb(readb((void*)0xbf000000+(UTMI_base+0x03)*2-1) & 0xef, (void*) (0xbf000000+(UTMI_base+0x03)*2-1)); //Disconnect window select

	writeb(readb((void*)0xbf000000+(UTMI_base+0x07)*2-1) & 0xfd, (void*) (0xbf000000+(UTMI_base+0x07)*2-1)); //Disable improved CDR
	writeb(readb((void*)0xbf000000+(UTMI_base+0x09)*2-1) |0x81, (void*) (0xbf000000+(UTMI_base+0x09)*2-1)); // UTMI RX anti-dead-loc, ISI effect improvement
	writeb(readb((void*)0xbf000000+(UTMI_base+0x0b)*2-1) |0x80, (void*) (0xbf000000+(UTMI_base+0x0b)*2-1)); // TX timing select latch path
	writeb(readb((void*)0xbf000000+(UTMI_base+0x15)*2-1) |0x20, (void*) (0xbf000000+(UTMI_base+0x15)*2-1)); // Chirp signal source select

    #if defined(CONFIG_MSTAR_AMBER1) || defined(CONFIG_MSTAR_AMBER5)
		writeb(readb((void*)0xbf000000+(UTMI_base+0x2c)*2) |0x98, (void*) (0xbf000000+(UTMI_base+0x2c)*2));
		writeb(readb((void*)0xbf000000+(UTMI_base+0x2e)*2) |0x10, (void*) (0xbf000000+(UTMI_base+0x2e)*2));
		writeb(readb((void*)0xbf000000+(UTMI_base+0x2f)*2-1) |0x01, (void*) (0xbf000000+(UTMI_base+0x2f)*2-1));
#else
	writeb(readb((void*)0xbf000000+(UTMI_base+0x2c)*2) |0x10, (void*) (0xbf000000+(UTMI_base+0x2c)*2));
	writeb(readb((void*)0xbf000000+(UTMI_base+0x2f)*2-1) |0x81, (void*) (0xbf000000+(UTMI_base+0x2f)*2-1));
#endif
	writeb(readb((void*)0xbf000000+(UTMI_base+0x2d)*2-1) |0x02, (void*) (0xbf000000+(UTMI_base+0x2d)*2-1));


    if (flag & EHCFLAG_TESTPKG)
    {
        #if (!defined(CONFIG_MSTAR_AMBER1)) && (!defined(CONFIG_MSTAR_AMBER5))
        // === Remove for A1 & A5 ===
	    writew(0x0210, (void*) (0xbf000000+(UTMI_base+0x2C)*2)); //
	    writew(0x8100, (void*) (0xbf000000+(UTMI_base+0x2E)*2)); //
        #endif 

	    writew(0x0600, (void*) (0xbf000000+(UTMI_base+0x14)*2)); //
	    writew(0x0038, (void*) (0xbf000000+(UTMI_base+0x10)*2)); //
	    writew(0x0BFE, (void*) (0xbf000000+(UTMI_base+0x32)*2)); //
    }

}

#elif defined(CONFIG_MSTAR_AMBER3) || defined(CONFIG_MSTAR_EDISON) || defined(CONFIG_MSTAR_EMERALD)
//-----------------------------------------
void Titania3_series_start_ehc(unsigned int UTMI_base, unsigned int USBC_base, unsigned int UHC_base, unsigned int flag)
{
	printk("Titania3_series_start_ehc start\n");

	if (flag & EHCFLAG_TESTPKG)
	{
		writew(0x2084, (void*) (UTMI_base+0x2*2)); //
		writew(0x8051, (void*) (UTMI_base+0x20*2)); //
    }

	writeb(0x0a, (void*) (USBC_base)); // Disable MAC initial suspend, Reset UHC
	writeb(0x28, (void*) (USBC_base)); // Release UHC reset, enable UHC and OTG XIU function

	writeb((unsigned char)(readb((void*)(UTMI_base+0x09*2-1)) & ~0x08), (void*) (UTMI_base+0x09*2-1)); // Disable force_pll_on
	writeb((unsigned char)(readb((void*)(UTMI_base+0x08*2)) & ~0x80), (void*) (UTMI_base+0x08*2)); // Enable band-gap current
	writeb(0xC3, (void*)(UTMI_base)); // reg_pdn: bit<15>, bit <2> ref_pdn
	mdelay(1);	// delay 1ms

	writeb(0x69, (void*) (UTMI_base+0x01*2-1)); // Turn on UPLL, reg_pdn: bit<9>
	mdelay(2);	// delay 2ms

	writeb(0x01, (void*) (UTMI_base)); // Turn all (including hs_current) use override mode
	writeb(0, (void*) (UTMI_base+0x01*2-1)); // Turn on UPLL, reg_pdn: bit<9>

	writeb(0x0A, (void*) (USBC_base)); // Disable MAC initial suspend, Reset UHC
	writeb(0x28, (void*) (USBC_base)); // Release UHC reset, enable UHC XIU function

	writeb(readb((void*)(UTMI_base+0x3C*2)) | 0x01, (void*) (UTMI_base+0x3C*2)); // set CA_START as 1
	mdelay(10);

	writeb((unsigned char)(readb((void*)(UTMI_base+0x3C*2)) & ~0x01), (void*) (UTMI_base+0x3C*2)); // release CA_START

	while ((readb((void*)(UTMI_base+0x3C*2)) & 0x02) == 0);	// polling bit <1> (CA_END)

    if (flag & EHCFLAG_DPDM_SWAP)
    	writeb(readb((void*)(UTMI_base+0x0b*2-1)) |0x20, (void*) (UTMI_base+0x0b*2-1)); // dp dm swap

	writeb((unsigned char)(readb((void*)(USBC_base+0x02*2)) & ~0x03), (void*) (USBC_base+0x02*2)); //UHC select enable
	writeb(readb((void*)(USBC_base+0x02*2)) | 0x01, (void*) (USBC_base+0x02*2)); //UHC select enable

	writeb((unsigned char)(readb((void*)(UHC_base+0x40*2)) & ~0x10), (void*) (UHC_base+0x40*2)); //0: VBUS On.
	mdelay(1);	// delay 1ms

	writeb(readb((void*)(UHC_base+0x40*2)) | 0x08, (void*) (UHC_base+0x40*2)); // Active HIGH
	mdelay(1);	// delay 1ms

	writeb(readb((void*)(UHC_base+0x81*2-1)) | 0x8F, (void*) (UHC_base+0x81*2-1)); //improve the efficiency of USB access MIU when system is busy

	writeb((unsigned char)((readb((void*)(UTMI_base+0x06*2)) & 0x9F) | 0x40), (void*) (UTMI_base+0x06*2)); //reg_tx_force_hs_current_enable

	writeb(readb((void*)(UTMI_base+0x03*2-1)) | 0x28, (void*) (UTMI_base+0x03*2-1)); //Disconnect window select
	writeb(readb((void*)(UTMI_base+0x03*2-1)) & 0xef, (void*) (UTMI_base+0x03*2-1)); //Disconnect window select

	writeb(readb((void*)(UTMI_base+0x07*2-1)) & 0xfd, (void*) (UTMI_base+0x07*2-1)); //Disable improved CDR
#if defined(CONFIG_MSTAR_EDISON)
	writeb(readb((void*)(UTMI_base+0x08*2)) | 0x08, (void*) (UTMI_base+0x08*2)); // bit<3> for 240's phase as 120's clock set 1, bit<4> for 240Mhz in mac 0 for faraday 1 for etron
#endif
	writeb(readb((void*)(UTMI_base+0x09*2-1)) |0x81, (void*) (UTMI_base+0x09*2-1)); // UTMI RX anti-dead-loc, ISI effect improvement
	writeb(readb((void*)(UTMI_base+0x0b*2-1)) |0x80, (void*) (UTMI_base+0x0b*2-1)); // TX timing select latch path
	writeb(readb((void*)(UTMI_base+0x15*2-1)) |0x20, (void*) (UTMI_base+0x15*2-1)); // Chirp signal source select
#if defined(CONFIG_MSTAR_EDISON) || defined(CONFIG_MSTAR_EMERALD)
	writeb(readb((void*)(UTMI_base+0x15*2-1)) |0x40, (void*) (UTMI_base+0x15*2-1)); // change to 55 interface
#endif

#ifdef CONFIG_MSTAR_AMBER3
	if(UTMI_base == _MSTAR_UTMI0_BASE)      // UTMI setting only for Port0, couse of Eye diagram
	{
		writeb(0x60, (void*) (UTMI_base+0x2c*2));
		writeb(0x00, (void*) (UTMI_base+0x2e*2));
		writeb(0x01, (void*) (UTMI_base+0x2f*2-1));
		writeb(0x03, (void*) (UTMI_base+0x2d*2-1));
	}
	else if(UTMI_base == _MSTAR_UTMI1_BASE) // UTMI setting only for Port1, couse of Eye diagram
	{
		writeb(0x60, (void*) (UTMI_base+0x2c*2));
		writeb(0x00, (void*) (UTMI_base+0x2e*2));
		writeb(0x00, (void*) (UTMI_base+0x2f*2-1));
		writeb(0x03, (void*) (UTMI_base+0x2d*2-1));
	}
#ifdef ENABLE_THIRD_EHC
	else    // UTMI seeting only for Port2, couse of Eye diagram
	{
		writeb(readb((void*)(UTMI_base+0x2c*2)) |0x98, (void*) (UTMI_base+0x2c*2));
		writeb(readb((void*)(UTMI_base+0x2e*2)) |0x10, (void*) (UTMI_base+0x2e*2));
		writeb(readb((void*)(UTMI_base+0x2f*2-1)) |0x01, (void*) (UTMI_base+0x2f*2-1));

		writeb(readb((void*)(UTMI_base+0x2d*2-1)) |0x02, (void*) (UTMI_base+0x2d*2-1));
	}
#endif
#elif defined(CONFIG_MSTAR_EDISON)
	if(UTMI_base == _MSTAR_UTMI0_BASE)      // UTMI setting only for Port0, couse of Eye diagram
	{
		writeb(0x08, (void*) (UTMI_base+0x2c*2));
		writeb(0x02, (void*) (UTMI_base+0x2d*2-1));
		writeb(0x03, (void*) (UTMI_base+0x2e*2));
		writeb(0x01, (void*) (UTMI_base+0x2f*2-1));
	}
	else	// for other Ports
	{
		writeb(0x18, (void*) (UTMI_base+0x2c*2));
		writeb(0x02, (void*) (UTMI_base+0x2d*2-1));
		writeb(0x03, (void*) (UTMI_base+0x2e*2));
		writeb(0x01, (void*) (UTMI_base+0x2f*2-1));
	}
#elif defined(CONFIG_MSTAR_EMERALD)
	writeb(readb((void*)(UTMI_base+0x2c*2)) |0x98, (void*) (UTMI_base+0x2c*2));
	writeb(readb((void*)(UTMI_base+0x2d*2-1)) |0x00, (void*) (UTMI_base+0x2d*2-1));
	writeb(readb((void*)(UTMI_base+0x2e*2)) |0x10, (void*) (UTMI_base+0x2e*2));
	writeb(readb((void*)(UTMI_base+0x2f*2-1)) |0x00, (void*) (UTMI_base+0x2f*2-1));
#else
	writeb(readb((void*)(UTMI_base+0x2c*2)) |0x98, (void*) (UTMI_base+0x2c*2));
	writeb(readb((void*)(UTMI_base+0x2e*2)) |0x10, (void*) (UTMI_base+0x2e*2));
	writeb(readb((void*)(UTMI_base+0x2f*2-1)) |0x01, (void*) (UTMI_base+0x2f*2-1));

	writeb(readb((void*)(UTMI_base+0x2d*2-1)) |0x02, (void*) (UTMI_base+0x2d*2-1));
#endif

#if defined(ENABLE_LS_CROSS_POINT_ECO)
	writeb(readb((void*)(UTMI_base+0x04*2)) | 0x40, (void*) (UTMI_base+0x04*2));  //enable deglitch SE0〃(low-speed cross point)
#endif

#if defined(ENABLE_PWR_NOISE_ECO)
	writeb(readb((void*)(USBC_base+0x02*2)) | 0x40, (void*) (USBC_base+0x02*2)); //enable use eof2 to reset state machine〃 (power noise)
#endif

#if defined(ENABLE_TX_RX_RESET_CLK_GATING_ECO)
	writeb(readb((void*)(UTMI_base+0x04*2)) | 0x20, (void*) (UTMI_base+0x04*2)); //enable hw auto deassert sw reset(tx/rx reset)
#endif

#if defined(ENABLE_LOSS_SHORT_PACKET_INTR_ECO)
	#if defined(CONFIG_MSTAR_AMBER3)
	writeb(readb((void*)(USBC_base+0x0b*2)) | 0x04, (void*) (USBC_base+0x0b*2)); //enable patch for the assertion of interrupt(Lose short packet interrupt)
	#else
	writeb(readb((void*)(USBC_base+0x04*2)) & 0x7f, (void*) (USBC_base+0x04*2)); //enable patch for the assertion of interrupt(Lose short packet interrupt)
	#endif
#endif

#if defined(ENABLE_BABBLE_ECO)
	writeb(readb((void*)(USBC_base+0x04*2)) | 0x40, (void*) (USBC_base+0x04*2)); //enable add patch to Period_EOF1(babble problem)
#endif

#if defined(ENABLE_MDATA_ECO)
	#if defined(CONFIG_MSTAR_EDISON) || defined(CONFIG_MSTAR_EMERALD)
	writeb(readb((void*)USBC_base) | 0x10, (void*) USBC_base); //enable short packet MDATA in Split transaction clears ACT bit (LS dev under a HS hub)
	#else
	writeb(readb((void*)(USBC_base+0x0f*2-1)) | 0x10, (void*) (USBC_base+0x0f*2-1)); //enable short packet MDATA in Split transaction clears ACT bit (LS dev under a HS hub)
	#endif
#endif

    if (flag & EHCFLAG_TESTPKG)
    {
	    writew(0x0600, (void*) (UTMI_base+0x14*2)); //
	    writew(0x0038, (void*) (UTMI_base+0x10*2)); //
	    writew(0x0BFE, (void*) (UTMI_base+0x32*2)); //
    }
}
#endif

static void uranus_stop_ehc(struct platform_device *dev)
{

}

#if 0
//get_usb_base_addr() return value => 0:success; 1:fail
int get_usb_base_addr(int busnum, unsigned int *pUTMI_base, unsigned int *pUSBC_base, unsigned int *pUHC_base)
{
    int ret = 0;

	if (busnum==2)
	{
        *pUTMI_base = _MSTAR_UTMI_BASE;
        *pUSBC_base = _MSTAR_USBC_BASE;
        *pUHC_base = _MSTAR_UHC_BASE;
	}
    else if (busnum==1)
    {
        *pUTMI_base = _MSTAR_UTMI1_BASE;
        *pUSBC_base = _MSTAR_USBC1_BASE;
        *pUHC_base = _MSTAR_UHC1_BASE;
    }
    #ifdef ENABLE_THIRD_EHC
    else if (busnum==3)
    {
        *pUTMI_base = _MSTAR_UTMI2_BASE;
        *pUSBC_base = _MSTAR_USBC2_BASE;
        *pUHC_base = _MSTAR_UHC2_BASE;
    }
    #endif
	else
	{
		printk("Invalid busnum for issue_port_reset\n");
        ret = 1;
	}

    return ret;
}
#endif

/*-------------------------------------------------------------------------*/

/* configure so an HC device and id are always provided */
/* always called with process context; sleeping is OK */

/**
 * usb_ehci_au1xxx_probe - initialize Au1xxx-based HCDs
 * Context: !in_interrupt()
 *
 * Allocates basic resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it
 * through the hotplug entry's driver_data.
 *
 */

#if defined(CONFIG_MSTAR_TITANIA3) || defined(CONFIG_MSTAR_TITANIA8) || defined(CONFIG_MSTAR_TITANIA9) || defined(CONFIG_MSTAR_URANUS4) || defined(CONFIG_MSTAR_AMBER1) || defined(CONFIG_MSTAR_AMBER5)

#define UTMI0_BASE  0x103A80
#define UTMI1_BASE  0x103A00
#define USBC0_BASE  0x100700
#define USBC1_BASE  0x100780
#define UHC0_BASE   0x102400
#define UHC1_BASE   0x100D00
#endif

#if defined(CONFIG_MSTAR_URANUS4)
#define UTMI2_BASE  0x102A00
#define USBC2_BASE  0x110200
#define UHC2_BASE   0x110300
#endif

#if defined(CONFIG_MSTAR_AMBER5)
#define UTMI2_BASE  0x103900
#define USBC2_BASE  0x113800
#define UHC2_BASE   0x113900
#endif


int usb_ehci_mstar_probe(const struct hc_driver *driver,
		struct usb_hcd **hcd_out, struct platform_device *dev)
{
	int retval=0;
	struct usb_hcd *hcd;
	struct ehci_hcd *ehci;

	//tony---------------------------------------
	if( 0==strcmp(dev->name, "Mstar-ehci-1") )
	{
		printk("Mstar-ehci-1 H.W init\n");
        #if defined(CONFIG_MSTAR_EUCLID)
		euclid_start_ehc1();
        #elif defined(CONFIG_MSTAR_TITANIA3) || defined(CONFIG_MSTAR_TITANIA8) || defined(CONFIG_MSTAR_TITANIA9) || defined(CONFIG_MSTAR_AMBER1) || defined(CONFIG_MSTAR_AMBER5)

		Titania3_start_ehc1();
		//Titania3_series_start_ehc(UTMI0_BASE, USBC0_BASE, UHC0_BASE, 0);
        #elif defined(CONFIG_MSTAR_URANUS4)
		Titania3_series_start_ehc(UTMI0_BASE, USBC0_BASE, UHC0_BASE, 0);
        #elif defined(CONFIG_MSTAR_AMBER3) || defined(CONFIG_MSTAR_EDISON) || defined(CONFIG_MSTAR_EMERALD)
                Titania3_series_start_ehc(_MSTAR_UTMI0_BASE, _MSTAR_USBC0_BASE, _MSTAR_UHC0_BASE, 0);
        #else
		#if 0 //Set 1 if you want to send test packet on port 0
		uranus_start_ehc1(dev, EHCFLAG_TESTPKG);
		#else
		uranus_start_ehc1(dev, 0);
		#endif
        #endif
	}
	else if( 0==strcmp(dev->name, "Mstar-ehci-2") )
	{
		printk("Mstar-ehci-2 H.W init\n");
        #if defined(CONFIG_MSTAR_EUCLID)
		euclid_start_ehc2();
        #elif defined(CONFIG_MSTAR_TITANIA3) || defined(CONFIG_MSTAR_TITANIA8) || defined(CONFIG_MSTAR_TITANIA9) || defined(CONFIG_MSTAR_AMBER1) || defined(CONFIG_MSTAR_AMBER5)

		Titania3_start_ehc2();
		//Titania3_series_start_ehc(UTMI1_BASE, USBC1_BASE, UHC1_BASE, 0);
        #elif defined(CONFIG_MSTAR_URANUS4)
    	Titania3_series_start_ehc(UTMI1_BASE, USBC1_BASE, UHC1_BASE, 0);
		    #elif defined(CONFIG_MSTAR_AMBER3) || defined(CONFIG_MSTAR_EDISON) || defined(CONFIG_MSTAR_EMERALD)
		    Titania3_series_start_ehc(_MSTAR_UTMI1_BASE, _MSTAR_USBC1_BASE, _MSTAR_UHC1_BASE, 0);
        #else
		#if 0 //Set 1 if you want to send test packet on port 1
		uranus_start_ehc2(dev, EHCFLAG_TESTPKG);
		#else
		uranus_start_ehc2(dev, 0);
		#endif
        #endif
	}
    #ifdef ENABLE_THIRD_EHC
	else if( 0==strcmp(dev->name, "Mstar-ehci-3") )
	{
		printk("Mstar-ehci-3 H.W init\n");
        #if defined(CONFIG_MSTAR_URANUS4) 
		Titania3_series_start_ehc(UTMI2_BASE, USBC2_BASE, UHC2_BASE, EHCFLAG_DPDM_SWAP);
        #elif defined(CONFIG_MSTAR_AMBER5)
		Titania3_series_start_ehc(UTMI2_BASE, USBC2_BASE, UHC2_BASE, 0);
        #elif defined(CONFIG_MSTAR_AMBER3) || defined(CONFIG_MSTAR_EDISON)
		    Titania3_series_start_ehc(_MSTAR_UTMI2_BASE, _MSTAR_USBC2_BASE, _MSTAR_UHC2_BASE, 0);
		#endif
	}
    #endif
	//-------------------------------------------
	//
	if (dev->resource[2].flags != IORESOURCE_IRQ) {
		pr_debug("resource[1] is not IORESOURCE_IRQ");
		retval = -ENOMEM;
	}
	hcd = usb_create_hcd(driver, &dev->dev, "mstar");
	if (!hcd)
		return -ENOMEM;
	hcd->rsrc_start = dev->resource[1].start;
	hcd->rsrc_len = dev->resource[1].end - dev->resource[1].start + 0;
#if _USB_XIU_TIMEOUT_PATCH
	hcd->usb_reset_lock = __SPIN_LOCK_UNLOCKED(hcd->usb_reset_lock);
#endif

	if (!request_mem_region((resource_size_t)(hcd->rsrc_start), (resource_size_t)(hcd->rsrc_len), hcd_name)) {
		pr_debug("request_mem_region failed");
		retval = -EBUSY;
		goto err1;
	}

	//hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);	//tony
	hcd->regs = (void *)(u32)(hcd->rsrc_start);	        	//tony
	if (!hcd->regs) {
		pr_debug("ioremap failed");
		retval = -ENOMEM;
		goto err2;
	}

	ehci = hcd_to_ehci(hcd);
	ehci->caps = hcd->regs;
	//ehci->regs = hcd->regs + HC_LENGTH(readl(&ehci->caps->hc_capbase));

	ehci->regs = (struct ehci_regs *)((u32)hcd->regs + HC_LENGTH(ehci, ehci_readl(ehci, &ehci->caps->hc_capbase)));


	//printk("\nDean: [%s] ehci->regs: 0x%x\n", __FILE__, (unsigned int)ehci->regs);
	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);


	/* ehci_hcd_init(hcd_to_ehci(hcd)); */
	if( 0==strcmp(dev->name, "Mstar-ehci-1") )
	{
		hcd->port_index = 1;
		hcd->utmi_base = _MSTAR_UTMI0_BASE;
		hcd->ehc_base = _MSTAR_UHC0_BASE;
		hcd->usbc_base = _MSTAR_USBC0_BASE;
	}
	else if( 0==strcmp(dev->name, "Mstar-ehci-2") )
	{
		hcd->port_index = 2;
		hcd->utmi_base = _MSTAR_UTMI1_BASE;
		hcd->ehc_base = _MSTAR_UHC1_BASE;
		hcd->usbc_base = _MSTAR_USBC1_BASE;
	}
#ifdef ENABLE_THIRD_EHC
	else if( 0==strcmp(dev->name, "Mstar-ehci-3") )
	{
		hcd->port_index = 3;
		hcd->utmi_base = _MSTAR_UTMI2_BASE;
		hcd->ehc_base = _MSTAR_UHC2_BASE;
		hcd->usbc_base = _MSTAR_USBC2_BASE;
	}
#endif
	retval = usb_add_hcd(hcd, dev->resource[2].start, SA_INTERRUPT);//tony

	hcd->root_port_devnum=0;
	hcd->enum_port_flag=0;
	hcd->enum_dbreset_flag=0;
	hcd->rootflag=0;
	//hcd->lock_usbreset=SPIN_LOCK_UNLOCKED;
	hcd->lock_usbreset=__SPIN_LOCK_UNLOCKED(hcd->lock_usbreset);

	//usb_add_hcd(hcd, dev->resource[2].start, IRQF_DISABLED | IRQF_SHARED);
	if (retval == 0)
		return retval;

	uranus_stop_ehc(dev);
	//iounmap(hcd->regs);		//tony
err2:
	release_mem_region((resource_size_t)hcd->rsrc_start, (resource_size_t)hcd->rsrc_len);
err1:
	usb_put_hcd(hcd);
	return retval;
}

/* may be called without controller electrically present */
/* may be called with controller, bus, and devices active */

/**
 * usb_ehci_hcd_au1xxx_remove - shutdown processing for Au1xxx-based HCDs
 * @dev: USB Host Controller being removed
 * Context: !in_interrupt()
 *
 * Reverses the effect of usb_ehci_hcd_au1xxx_probe(), first invoking
 * the HCD's stop() method.  It is always called from a thread
 * context, normally "rmmod", "apmd", or something similar.
 *
 */
void usb_ehci_mstar_remove(struct usb_hcd *hcd, struct platform_device *dev)
{
	usb_remove_hcd(hcd);
	iounmap(hcd->regs);
	release_mem_region((resource_size_t)hcd->rsrc_start, (resource_size_t)hcd->rsrc_len);
	usb_put_hcd(hcd);
	uranus_stop_ehc(dev);
}

/*-------------------------------------------------------------------------*/

static int ehci_hcd_mstar_drv_probe(struct platform_device *pdev)
{
	struct usb_hcd *hcd = NULL;
	int ret;

	pr_debug("In ehci_hcd_mstar_drv_probe\n");

	if (usb_disabled())
		return -ENODEV;

	/* FIXME we only want one one probe() not two */
	ret = usb_ehci_mstar_probe(&ehci_mstar_hc_driver, &hcd, pdev);
	return ret;
}

static int ehci_hcd_mstar_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	/* FIXME we only want one one remove() not two */
	usb_ehci_mstar_remove(hcd, pdev);
	return 0;
}

#if 0
static int ehci_hcd_mstar_drv_suspend(struct platform_device *pdev, pm_message_t state)
{
	//struct usb_hcd *hcd = platform_get_drvdata(pdev);

	printk("ehci_hcd_mstar_drv_suspend...\n");
	//usb_ehci_mstar_remove(hcd, pdev);
	return 0;
}
#endif

#if 0
static int ehci_hcd_mstar_drv_resume(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	printk("ehci_hcd_mstar_drv_resume...\n");
	//usb_ehci_mstar_probe(&ehci_mstar_hc_driver, &hcd, pdev);

	if( 0==strcmp(pdev->name, "Mstar-ehci-1") )
		Titania3_series_start_ehc(_MSTAR_UTMI0_BASE, _MSTAR_USBC0_BASE, _MSTAR_UHC0_BASE, 0);
	else if( 0==strcmp(pdev->name, "Mstar-ehci-2") )
		Titania3_series_start_ehc(_MSTAR_UTMI1_BASE, _MSTAR_USBC1_BASE, _MSTAR_UHC1_BASE, 0);
#ifdef ENABLE_THIRD_EHC
	else if( 0==strcmp(pdev->name, "Mstar-ehci-3") )
		Titania3_series_start_ehc(_MSTAR_UTMI2_BASE, _MSTAR_USBC2_BASE, _MSTAR_UHC2_BASE, 0 );
#endif
	enable_irq(hcd->irq);
	return 0;
}
#endif

/*-------------------------------------------------------------------------*/

static struct platform_driver ehci_hcd_mstar_driver = {
	.probe 		= ehci_hcd_mstar_drv_probe,
	.remove 	= ehci_hcd_mstar_drv_remove,
//	.suspend	= ehci_hcd_uranus_drv_suspend,
//	.resume		= ehci_hcd_uranus_drv_resume,
	.driver = {
		.name	= "Mstar-ehci-1",
//		.bus	= &platform_bus_type,
	}
};


static struct platform_driver second_ehci_hcd_mstar_driver = {
	.probe 		= ehci_hcd_mstar_drv_probe,
	.remove 	= ehci_hcd_mstar_drv_remove,
//	.suspend	= ehci_hcd_uranus_drv_suspend,
//	.resume		= ehci_hcd_uranus_drv_resume,
	.driver = {
		.name 	= "Mstar-ehci-2",
//		.bus	= &platform_bus_type,
	}
};
#ifdef ENABLE_THIRD_EHC
static struct platform_driver third_ehci_hcd_mstar_driver = {
	.probe 		= ehci_hcd_mstar_drv_probe,
	.remove 	= ehci_hcd_mstar_drv_remove,
//	.suspend	= ehci_hcd_uranus_drv_suspend,
//	.resume		= ehci_hcd_uranus_drv_resume,
	.driver = {
		.name 	= "Mstar-ehci-3",
//		.bus	= &platform_bus_type,
	}
};
#endif

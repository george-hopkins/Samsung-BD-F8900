/*------------------------------------------------------------------------------
	Copyright (c) 2008 MStar Semiconductor, Inc.  All rights reserved.
------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
    PROJECT: amber3

	FILE NAME: arch/arm/mach-amber3/amber3_arch.c

    DESCRIPTION:
          Power Management Driver

    HISTORY:
         <Date>     <Author>    <Modification Description>
        2008/07/18  Fred Cheng  Add IO tables for ITCM and DTCM
        2008/07/22  Evan Chang  Add SD card init

------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    Include Files
------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/memory.h>
#include <mach/io.h>
#include <asm/mach/map.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/serial_8250.h>
#include <asm/irq.h>
#include <asm/hardware/cache-l2x0.h>
#include <asm/hardware/gic.h>
#include <asm/smp_twd.h>
#include <asm/cacheflush.h>

#include "chip_int.h"
/*------------------------------------------------------------------------------
    External Function Prototypes
-------------------------------------------------------------------------------*/
/* initialize amber3's IRQ */
extern void amber3_init_irq(void);
/* Add SD Card driver */
extern void __init amber3_add_device_mmc(short mmc_id);
extern void __init amber3_add_device_memstick(s16 memstick_id);
extern int Mstar_ehc_platform_init(void);


/*------------------------------------------------------------------------------
    External Global Variable
-------------------------------------------------------------------------------*/
/* amber3's system timer */
extern struct sys_timer amber3_timer;
extern struct sys_timer amber3_ptimer;
/*------------------------------------------------------------------------------
    Global Variable
-------------------------------------------------------------------------------*/
/* platform device for amber3_board_init() */
struct platform_device amber3_device_usbgadget =
{
	.name		  = "amber3-usbgadget",
	.id		  = -1,
};

/* platform device array for amber3_board_init() */
static struct platform_device *amber3_devices[] __initdata =
{
	&amber3_device_usbgadget,
};

/* IO tables for Registers, ITCM and DTCM */
static struct map_desc amber3_io_desc[] __initdata =
{
    /* Define Registers' physcial and virtual addresses */
    {
	    .virtual        = IO_VIRT,
	    .pfn            = __phys_to_pfn(IO_PHYS),
	    .length         = IO_SIZE,
	    .type           = MT_DEVICE
    },

    /* Define periphral physcial and virtual addresses */
    {
	    .virtual        = PERI_VIRT,
	    .pfn            = __phys_to_pfn(PERI_PHYS),
	    .length         = PERI_SIZE,
	    .type           = MT_DEVICE
    },	 	

    {
	    .virtual	    = L2_CACHE_VIRT,
	    .pfn	    = __phys_to_pfn(L2_CACHE_PHYS),
	    .length	    = L2_CACHE_SIZE,
	    .type	    = MT_DEVICE,
    },



    #if 0   
    /* Define ITCM' physcial and virtual addresses */
    {
		.virtual	= amber3_ITCM_VIRT,
		.pfn		= __phys_to_pfn(amber3_ITCM_PHYS),
		.length		= amber3_ITCM_SIZE,
		.type		= MT_DEVICE
    },
	/* Define DTCM' physcial and virtual addresses */
	{
		.virtual	= amber3_DTCM_VIRT,
		.pfn		= __phys_to_pfn(amber3_DTCM_PHYS),
		.length		= amber3_DTCM_SIZE,
		.type		= MT_DEVICE
	},
    /* Define MEM51' physcial and virtual addresses */
	{
		.virtual	= amber3_MEM51_VIRT,
		.pfn		= __phys_to_pfn(amber3_MEM51_PHY),
		.length		= amber3_MEM51_SIZE,
		.type		= MT_MEMORY_NONCACHED
	}
	#endif
};

/*------------------------------------------------------------------------------
    Local Function
-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    Function: amber3_map_io

    Description:
        init amber3 IO tables
    Input: (The arguments were used by caller to input data.)
        None.
    Output: (The arguments were used by caller to receive data.)
        None.
    Return:
        None.
    Remark:
        None.
-------------------------------------------------------------------------------*/
void __init amber3_map_io(void)
{
#ifdef CONFIG_HAVE_ARM_TWD
    twd_base=((void __iomem *)PERI_ADDRESS(0x16000600)); //private timer& watchdog base
#endif
    iotable_init(amber3_io_desc, ARRAY_SIZE(amber3_io_desc) );
}


void __init serial_init(void)
{

#ifdef CONFIG_SERIAL_8250
    struct uart_port s, u, k;

    memset(&s, 0, sizeof(s));
    s.type = PORT_16550;
    s.iobase = 0xFD201300; //virtual RIU_BASE = 0xFD000000   
    s.irq = E_IRQ_UART0;

#ifdef CONFIG_MSTAR_AMBER3_BD_FPGA
    s.uartclk = 12000000; //FPGA
#elif  CONFIG_MSTAR_AMBER3_BD_GENERIC  
    s.uartclk = 123000000; //real chip
#endif
    s.iotype = 0;
    s.regshift = 0;
    s.fifosize = 16 ; // use the 8 byte depth FIFO well
    s.line = 0;

    if (early_serial_setup(&s) != 0) {
        printk(KERN_ERR "Serial(0) setup failed!\n");
    }
	memset(&u, 0, sizeof(u));
	u.type = PORT_16550;
	u.iobase = 0xFD220C00;
	u.irq = E_IRQEXPL_UART1;
	u.uartclk = 108000000;
	u.iotype = 0;
	u.regshift = 0;
	u.fifosize = 16 ; // use the 8 byte depth FIFO well
	u.line = 1;
	
	if (early_serial_setup(&u) != 0) {
		printk(KERN_ERR "Serial piu_uart1 setup failed!\n");
	}
	
	memset(&k, 0, sizeof(k));
	k.type = PORT_16550;
	k.iobase = 0xFD220D00;
	k.irq = E_IRQEXPH_UART2MCU;
	k.uartclk = 123000000;
	k.iotype = 0;
	k.regshift = 0;
	k.fifosize = 16 ; // use the 8 byte depth FIFO well
	k.line = 2;

	if (early_serial_setup(&k) != 0) {
		printk(KERN_ERR "Serial fuart setup failed!\n");
	}
#endif
}

extern void back_trace(void);

static irqreturn_t amber3_xiu_timeout_interrupt(int irq, void *dev_id)
{


    #ifdef CONFIG_HANG_ISSUE_DEBUG
    char * ptr= NULL;
    #endif

    printk("XIU Time Out Occurred!\n");
    printk("Address is 0x%x\n",*(volatile  short*)(0xfd200224));  
   
    #ifdef CONFIG_HANG_ISSUE_DEBUG
    back_trace(); 
    #endif
     
    #ifdef CONFIG_HANG_ISSUE_DEBUG
    *ptr = 1; 
    #endif

    return IRQ_HANDLED;
} 

static struct irqaction amber3_xiu_timeout_irq = {
    .name = "Amber3 Timeout IRQ",
    .flags = IRQF_TIMER | IRQF_IRQPOLL | IRQF_DISABLED,
    .handler = amber3_xiu_timeout_interrupt,

};


void __init amber3_l2c_init(void)
{
#ifdef CONFIG_CACHE_L2X0
	unsigned int val = 0;

	void __iomem * l2x0_base = (void __iomem *)(L2_CACHE_ADDRESS(L2_CACHE_PHYS));

#if L2_LINEFILL
	val = L2_CACHE_read( L2_CACHE_PHYS + PREFETCH_CTL_REG );
	L2_CACHE_write(( val | DOUBLE_LINEFILL_ENABLE | LINEFILL_WRAP_DISABLE ), L2_CACHE_PHYS + PREFETCH_CTL_REG);
#endif

#if L2_PREFETCH
	val = L2_CACHE_read( L2_CACHE_PHYS + PREFETCH_CTL_REG );
	L2_CACHE_write(( val | I_PREFETCH_ENABLE | D_PREFETCH_ENABLE | PREFETCH_OFFSET ), L2_CACHE_PHYS + PREFETCH_CTL_REG );
#endif

	/* set RAM latencies to 1 cycle for this core tile. */
	writel(0x113, l2x0_base + L2X0_TAG_LATENCY_CTRL);
	writel(0x111, l2x0_base + L2X0_DATA_LATENCY_CTRL);

	l2x0_init(l2x0_base, 0x00400000, 0xfe0fffff);
#endif
}

/*------------------------------------------------------------------------------
    Function: amber3_board_init

    Description:
        amber3 board init function
    Input: (The arguments were used by caller to input data.)
        None.
    Output: (The arguments were used by caller to receive data.)
        None.
    Return:
        None.
    Remark:
        None.
-------------------------------------------------------------------------------*/
static void __init amber3_board_init(void)
{

	platform_add_devices(amber3_devices, ARRAY_SIZE(amber3_devices));

#if defined(CONFIG_USB_EHCI_HCD) || defined(CONFIG_USB_EHCI_HCD_MODULE)
	Mstar_ehc_platform_init();
#endif

#if defined(CONFIG_MEMSTICK_AMBER3_MS)
	amber3_add_device_memstick(0);
#endif

	//amber3_l2c_init();

	*(volatile unsigned short *)(0xfd200200) = 0x1 ;
	*(volatile unsigned short *)(0xfd200250) = 0xFFFF ;
	*(volatile unsigned short *)(0xfd200254) = 0xFFFF ;

        setup_irq(E_FIQ_XIU_TIMEOUT , &amber3_xiu_timeout_irq);


}


/*************************************
*		Mstar chip flush function
*************************************/
#define _BIT(x)                      (1<<(x))
static DEFINE_SPINLOCK(l2prefetch_lock);

inline void _chip_flush_miu_pipe(void)
{
	unsigned long   dwLockFlag = 0;    
	unsigned short dwReadData = 0;

	spin_lock_irqsave(&l2prefetch_lock, dwLockFlag);
	//toggle the flush miu pipe fire bit 
	*(volatile unsigned short *)(0xfd203114) = 0x0;
	*(volatile unsigned short *)(0xfd203114) = 0x1;

	do
	{
		dwReadData = *(volatile unsigned short *)(0xfd203140);
		dwReadData &= _BIT(12);  //Check Status of Flush Pipe Finish

	} while(dwReadData == 0);

	spin_unlock_irqrestore(&l2prefetch_lock, dwLockFlag);

}

#ifndef CONFIG_OUTER_CACHE
struct outer_cache_fns outer_cache;
#endif

void Chip_Flush_Cache_Range(unsigned long u32Addr, unsigned long u32Size)
{
	if(  u32Addr == NULL )
        {
                printk("u32Addr is invalid\n");
                return;
        }        	
	//Clean L1 & Inv L1   
	dmac_flush_range(u32Addr,u32Addr + u32Size);
       
#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{
		if(!virt_addr_valid(u32Addr) || !virt_addr_valid(u32Addr+ u32Size - 1))
			//Clean&Inv L2 by Way
			outer_cache.flush_all();
		else 
			//Clean&Inv L2 by Range
			outer_cache.flush_range(__pa(u32Addr) ,__pa(u32Addr)+ u32Size);
	}
#endif

#ifndef CONFIG_OUTER_CACHE //flush miu pipe for L2 disabled case
	_chip_flush_miu_pipe();
#endif

}

void Chip_Clean_Cache_Range(unsigned long u32Addr, unsigned long u32Size)
{
        if(  u32Addr == NULL )
        {
                printk("u32Addr is invalid\n");
                return;
        }
	//Clean L1
	dmac_map_area(u32Addr,u32Size,1);

#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{
		if(!virt_addr_valid(u32Addr) || !virt_addr_valid(u32Addr+ u32Size - 1))
			//Clean L2 by Way
			outer_cache.clean_all();
		else 
			//Clean L2 by Range 
			outer_cache.clean_range( __pa(u32Addr),__pa(u32Addr) + u32Size);
	}
#endif

#ifndef CONFIG_OUTER_CACHE //flush miu pipe for L2 disabled case
	_chip_flush_miu_pipe();
#endif
}


void Chip_Inv_Cache_Range(unsigned long u32Addr, unsigned long u32Size)
{
        if(  u32Addr == NULL )
        {
                printk("u32Addr is invalid\n");
                return;
        }
#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{

		if(!virt_addr_valid(u32Addr) || !virt_addr_valid(u32Addr+ u32Size - 1))
			printk(KERN_DEBUG "Input VA can't be converted to PA\n");   
		else
			//Inv L2 by range 
			outer_cache.inv_range(__pa(u32Addr) , __pa(u32Addr) + u32Size);
	}
#endif
	//Inv L1   
	dmac_map_area(u32Addr,u32Size,2); 
}

void Chip_Flush_Cache_Range_VA_PA(unsigned long u32VAddr,unsigned long u32PAddr,unsigned long u32Size)
{
        if(  u32VAddr == NULL )
        {
                printk("u32VAddr is invalid\n");
                return;
        }
	//Clean & Invalid L1
	dmac_flush_range(u32VAddr, u32VAddr + u32Size);

#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{
		//Clean&Inv L2 by range
		outer_cache.flush_range(u32PAddr,u32PAddr + u32Size);
	}
#endif

#ifndef CONFIG_OUTER_CACHE
	_chip_flush_miu_pipe();
#endif
}

void Chip_Clean_Cache_Range_VA_PA(unsigned long u32VAddr,unsigned long u32PAddr,unsigned long u32Size)
{
        if(  u32VAddr == NULL )
        {
                printk("u32VAddr is invalid\n");
                return;
        }
	//Clean L1
	dmac_map_area(u32VAddr,u32Size,1);

#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{
		//Clean L2 by Way
		outer_cache.clean_range(u32PAddr,u32PAddr + u32Size);
	}
#endif

#ifndef CONFIG_OUTER_CACHE
	_chip_flush_miu_pipe();
#endif
}

void Chip_Inv_Cache_Range_VA_PA(unsigned long u32VAddr,unsigned long u32PAddr,unsigned long u32Size)
{
        if(  u32VAddr == NULL )
        {
                printk("u32VAddr is invalid\n");
                return;
        }

#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{
		//Inv L2 by range
		outer_cache.inv_range( u32PAddr ,u32PAddr + u32Size );
	}
#endif
	//Inv L1
	dmac_map_area(u32VAddr,u32Size,2);
}

void Chip_Flush_Cache_All()
{
	unsigned long flags;

	local_irq_save(flags);

	//Clean & Inv All L1
	__cpuc_flush_kern_all();

	local_irq_restore(flags);

	smp_call_function(__cpuc_flush_kern_all, NULL, 1);

	local_irq_save(flags);

#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.is_enable()) //check if L2 is enabled
	{
		//Clean&Inv L2 by Way
		outer_cache.flush_all();
	}
#endif
	//Clean L1  & Inv L1
	//dmac_flush_range(u32Addr,u32Addr + u32Size );

#ifndef CONFIG_OUTER_CACHE
	_chip_flush_miu_pipe();
#endif 
	
	local_irq_restore(flags);
}

//need to be modified
void Chip_L2_cache_wback_inv( unsigned long addr, unsigned long size)
{
	outer_cache.flush_all();
}

//need to be modified
void Chip_L2_cache_wback( unsigned long addr, unsigned long size)
{
	//Flush L2 by Way, change to by Addr later
	outer_cache.clean_all();
}

//need to be modified
void Chip_L2_cache_inv( unsigned long addr, unsigned long size)
{
	//Inv L2 by Way, change to by Addr later
	outer_cache.inv_all();
}

void Chip_Flush_Memory_Range(unsigned long pAddress , unsigned long  size)
{
	//2012/01/12 Modify by MStar: Add outer_cache.sync for enabling the non-cache bufferable mode
	if(outer_cache.sync)
		outer_cache.sync();
	else
		_chip_flush_miu_pipe();
}

void Chip_Flush_Memory(void)
{
	//2012/01/12 Modify by MStar: Add outer_cache.sync for enabling the non-cache bufferable mode
	if(outer_cache.sync)
		outer_cache.sync();
	else
		_chip_flush_miu_pipe();
}

void Chip_Read_Memory_Range(unsigned long pAddress , unsigned long  size)
{
	//2012/01/12 Modify by MStar: Add outer_cache.sync for enabling the non-cache bufferable mode
	if(outer_cache.sync)
		outer_cache.sync();
	else
		_chip_flush_miu_pipe();
}

void Chip_Read_Memory(void)
{
	//2012/01/12 Modify by MStar: Add outer_cache.sync for enabling the non-cache bufferable mode
	if(outer_cache.sync)
		outer_cache.sync();
	else
		_chip_flush_miu_pipe();
}

void Chip_Query_L2_Config(void)
{
}

unsigned int Chip_Query_Rev(void)
{
	unsigned int    dwRevisionId = 0;
	dwRevisionId = *((volatile unsigned int*)(0xFD003D9C));
	dwRevisionId >>= 8;

	return dwRevisionId;
}

#if 0 //modify later
unsigned int Chip_Query_CLK(void)
{
    return (12*reg_readw(0x1F22184c)); // 1 = 12(Hz)

}
#endif


EXPORT_SYMBOL(Chip_Flush_Cache_Range);
EXPORT_SYMBOL(Chip_Flush_Cache_All);
EXPORT_SYMBOL(Chip_Clean_Cache_Range);
EXPORT_SYMBOL(Chip_Inv_Cache_Range);
EXPORT_SYMBOL(Chip_L2_cache_wback_inv);
EXPORT_SYMBOL(Chip_L2_cache_wback);
EXPORT_SYMBOL(Chip_L2_cache_inv);
EXPORT_SYMBOL(Chip_Flush_Memory);
EXPORT_SYMBOL(Chip_Flush_Memory_Range);
EXPORT_SYMBOL(Chip_Read_Memory);
EXPORT_SYMBOL(Chip_Read_Memory_Range);
EXPORT_SYMBOL(Chip_Flush_Cache_Range_VA_PA);
EXPORT_SYMBOL(Chip_Clean_Cache_Range_VA_PA);
EXPORT_SYMBOL(Chip_Inv_Cache_Range_VA_PA);

/**
 ** please refer to include/asm-arm/mach/arch.h for further details
 **
 ** #define MACHINE_START(_type,_name)				\
 ** static const struct machine_desc __mach_desc_##_type	\
 **  __used							\
 **   __attribute__((__section__(".arch.info.init"))) = {	\
 **   	.nr		= MACH_TYPE_##_type,			\
 **   	.name		= _name,
 **
 **
 ** #define MACHINE_END			\
 ** };
 **
 **
 **/
//#ifdef CONFIG_MSTAR_amber3
MACHINE_START(AMBER3, "amber3")
//#endif
    .boot_params    = PHYS_OFFSET + 0x00000100,
    .map_io = amber3_map_io,
    .init_irq = amber3_init_irq,
    .init_machine = amber3_board_init,
#ifdef CONFIG_GENERIC_CLOCKEVENTS
    .timer = &amber3_timer,
#else
    .timer = &amber3_ptimer,
#endif
MACHINE_END

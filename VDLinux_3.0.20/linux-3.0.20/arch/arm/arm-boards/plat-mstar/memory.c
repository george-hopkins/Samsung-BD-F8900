/*
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 1999,2000 MIPS Technologies, Inc.  All rights reserved.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * PROM library functions for acquiring/using memory descriptors given to
 * us from the YAMON.
 */
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/bootmem.h>
#include <linux/pfn.h>
#include <linux/string.h>
#include <linux/module.h>

//#include <asm/bootinfo.h>
#include <asm/page.h>
#include <asm/sections.h>
#include <asm/arm-boards/prom.h>
#include <Board.h> // #include "../titania2/board/Board.h"
/*#define DEBUG*/

/*-----------------------------------------------------------------------------
    Include Files
------------------------------------------------------------------------------*/
unsigned long get_BBAddr(void);

enum yamon_memtypes {
    yamon_dontuse,
    yamon_prom,
    yamon_free,
};
//static struct prom_pmemblock mdesc[PROM_MAX_PMEMBLOCKS];

#ifdef DEBUG
static char *mtypes[3] = {
    "Dont use memory",
    "YAMON PROM memory",
    "Free memmory",
};
#endif

/* determined physical memory size, not overridden by command line args  */
unsigned long physical_memsize = 0L;
#if 0
static struct prom_pmemblock * __init prom_getmdesc(void)
{
    char *memsize_str;
    unsigned int memsize;
    char cmdline[CL_SIZE], *ptr;

    /* otherwise look in the environment */
    memsize_str = prom_getenv("memsize");
    if (!memsize_str) {
        printk(KERN_WARNING
               "memsize not set in boot prom, set to default (32Mb)\n");
        physical_memsize = 0x02000000;
    } else {
#ifdef DEBUG
        pr_debug("prom_memsize = %s\n", memsize_str);
#endif
        physical_memsize = simple_strtol(memsize_str, NULL, 0);
    }

#ifdef CONFIG_CPU_BIG_ENDIAN
    /* SOC-it swaps, or perhaps doesn't swap, when DMA'ing the last
       word of physical memory */
    physical_memsize -= PAGE_SIZE;
#endif

    /* Check the command line for a memsize directive that overrides
       the physical/default amount */
    strcpy(cmdline, arcs_cmdline);
    ptr = strstr(cmdline, "memsize=");
    if (ptr && (ptr != cmdline) && (*(ptr - 1) != ' '))
        ptr = strstr(ptr, " memsize=");

    if (ptr)
        memsize = memparse(ptr + 8, &ptr);
    else
        memsize = physical_memsize;

    memset(mdesc, 0, sizeof(mdesc));

    mdesc[0].type = yamon_dontuse;
    mdesc[0].base = 0x00000000;
    mdesc[0].size = 0x00001000;

    mdesc[1].type = yamon_prom;
    mdesc[1].base = 0x00001000;
    mdesc[1].size = 0x000ef000;

#ifdef CONFIG_MIPS_MALTA
    /*
     * The area 0x000f0000-0x000fffff is allocated for BIOS memory by the
     * south bridge and PCI access always forwarded to the ISA Bus and
     * BIOSCS# is always generated.
     * This mean that this area can't be used as DMA memory for PCI
     * devices.
     */
    mdesc[2].type = yamon_dontuse;
    mdesc[2].base = 0x000f0000;
    mdesc[2].size = 0x00010000;
#else
    mdesc[2].type = yamon_prom;
    mdesc[2].base = 0x000f0000;
    mdesc[2].size = 0x00010000;
#endif

    mdesc[3].type = yamon_dontuse;
    mdesc[3].base = 0x00100000;
    mdesc[3].size = CPHYSADDR(PFN_ALIGN((unsigned long)&_end)) - mdesc[3].base;

    mdesc[4].type = yamon_free;
    mdesc[4].base = CPHYSADDR(PFN_ALIGN(&_end));
    mdesc[4].size = memsize - mdesc[4].base;

    return &mdesc[0];
}

static int __init prom_memtype_classify(unsigned int type)
{
    switch (type) {
    case yamon_free:
        return BOOT_MEM_RAM;
    case yamon_prom:
        return BOOT_MEM_ROM_DATA;
    default:
        return BOOT_MEM_RESERVED;
    }
}
#endif

# define NR_BANKS 8

struct membank {
        unsigned long start;
        unsigned long size;
        unsigned int highmem;
};

struct meminfo {
        int nr_banks;
        struct membank bank[NR_BANKS];
};

extern struct meminfo meminfo;

//2010/10/05 modify by mstar: hardcode EMACmem without getting from bootcommand
static unsigned long LXmem2Addr=0x0, LXmem2Size=0x0, LXmemAddr=0x0, LXmemSize=0x0, EMACaddr=0x0, EMACmem=0x0, DRAMlen=0x0, BBAddr=0x0, MPoolSize=0x0;
unsigned long lx_mem_addr = 0x40200000;
unsigned long lx_mem_size=0x20000000;
unsigned long lx_mem2_addr=0xA0000000;
EXPORT_SYMBOL(lx_mem_addr);
EXPORT_SYMBOL(lx_mem_size);
EXPORT_SYMBOL(lx_mem2_addr);



//2011/8/17 modified by mstar: add LX_MEM function to dynamically change Linux memory address and size 
static int __init LX_MEM_setup(char *str)
{
    //printk("LX_MEM= %s\n", str);
    if( str != NULL )
    {
        sscanf(str,"%lx,%lx",&LXmemAddr,&LXmemSize);
        meminfo.nr_banks = 0;
        lx_mem_addr = LXmemAddr;
        lx_mem_size = LXmemSize;
    }
    else
    {
        printk("\nLX_MEM not set\n");
    }
    return 0;
}
//2010/10/05 Removed by mstar: Remove un-used functions: EMAC_MEM_setup  DRAM_LEN_setup  MPoolSize_setup,
//Because we no longer use the return value of above function
static int __init LX_MEM2_setup(char *str)
{
    //printk("LX_MEM2= %s\n", str);
    if( str != NULL )
    {
        sscanf(str,"%lx,%lx",&LXmem2Addr,&LXmem2Size);
        lx_mem2_addr = LXmem2Addr;
    }
    else
    {
        printk("\nLX_MEM2 not set\n");
    }
    return 0;
}

//2011/8/17 modified by mstar: add EMAC_setup function to dynamically change EMAC driver address and  size 
static int __init EMAC_MEM_setup(char *str)
{
    //printk("LX_MEM= %s\n", str);
    if( str != NULL )
    {
        sscanf(str,"%lx,%lx",&EMACaddr,&EMACmem);
    }
    else
    {
        printk("\nEMMAC not set\n");
    }
    return 0;
}


static int __init BBAddr_setup(char *str)
{
    //printk("LX_MEM2= %s\n", str);
    if( str != NULL )
    {
        sscanf(str,"%lx",&BBAddr);
    }

    return 0;
}

//2010/10/05 removed by mstar: Remove no longer used paramater got from bootcommand
early_param("LX_MEM", LX_MEM_setup);
early_param("LX_MEM2", LX_MEM2_setup);
early_param("EMAC_MEM", EMAC_MEM_setup);
early_param("BB_ADDR", BBAddr_setup);

static char bUseDefMMAP=0;
 
#if defined(CONFIG_MSTAR_OFFSET_FOR_SBOOT)
#define SBOOT_LINUX_MEM_START 0x00400000    //4M
#define SBOOT_LINUX_MEM_LEN   0x01400000    //20M
#define SBOOT_EMAC_MEM_LEN    0x100000      //1M
void get_boot_mem_info_sboot(BOOT_MEM_INFO type, unsigned int *addr, unsigned int *len)
{
    switch (type)
    {
    case LINUX_MEM:
        *addr = SBOOT_LINUX_MEM_START;
        *len = SBOOT_LINUX_MEM_LEN;
        break;

    case EMAC_MEM:
        *addr = SBOOT_LINUX_MEM_START+SBOOT_LINUX_MEM_LEN;
        *len = SBOOT_EMAC_MEM_LEN;
        break;

    case MPOOL_MEM:
        *addr = 0;
        *len = 256*1024*1024;
        break;

    case LINUX_MEM2:
        *addr = 0;
        *len = 0;
        break;

    default:
        *addr = 0;
        *len = 0;
        break;
    }
}
#endif//CONFIG_MSTAR_OFFSET_FOR_SBOOT


void get_boot_mem_info(BOOT_MEM_INFO type, unsigned int *addr, unsigned int *len)
{
#if defined(CONFIG_MSTAR_OFFSET_FOR_SBOOT)
    get_boot_mem_info_sboot(type, addr, len);
    printk("!!!!!!!!!!SBOOT memory type=%x addr=%x, len=%x!!!!!!!!!!\n",type, *addr, *len);
    return;
#endif//CONFIG_MSTAR_OFFSET_FOR_SBOOT

    if (bUseDefMMAP == 0)
    {
        switch (type)
        {
//2011/8/17 modified by mstar: Modify valiable for dynamically passing arguments
        case LINUX_MEM:
            *addr = LXmemAddr;
            *len = LXmemSize & ~(0x00100000UL - 1) ;
            break;

        case EMAC_MEM:
            *addr = EMACaddr;
            *len = EMACmem & ~(0x00100000UL - 1) ;
            break;

        case MPOOL_MEM:
            //2010/10/05 removed by mstar: Remove redandunt code 
            *addr = LINUX_MEM_BASE_ADR + LXmemSize;
            //We don't map LINUX_MEM, EMAC_MEM, LINUX_MEM2 area
            *len = MPoolSize;
            break;

        case LINUX_MEM2:
            if (LXmem2Addr!=0 && LXmem2Size!=0)
            {
                *addr = LXmem2Addr;
                *len = LXmem2Size & ~(0x00100000UL - 1) ;
            }
            else
            {
                *addr = 0;
                *len = 0;
            }
            break;

        default:
            *addr = 0;
            *len = 0;
            break;
        }
    }
    else
    {
        switch (type)
        {
        case LINUX_MEM:
            *addr = LINUX_MEM_BASE_ADR;
            *len = LINUX_MEM_LEN & ~(0x100000 - 1) ;
            break;

        case EMAC_MEM:
            *addr = EMAC_MEM_ADR;
            *len = EMAC_MEM_LEN & ~(0x100000 - 1) ;
            break;

        case MPOOL_MEM:
            *addr = MPOOL_ADR;
            *len = MPOOL_LEN;
            break;

        case LINUX_MEM2:
            if (LXmem2Addr!=0 && LXmem2Size!=0)
            {
                *addr = LXmem2Addr;
                *len  = LXmem2Size & ~(0x100000UL - 1) ;
            }
            else
            {
                #ifdef LINUX_MEM2_BASE_ADR    // reserved miu1 memory for linux
                *addr = LINUX_MEM2_BASE_ADR;
                *len = LINUX_MEM2_LEN & ~(0x100000UL - 1) ;
             //   printk("LINUX_MEM2 addr %08x, len %08x\n", *addr, *len);
                #else
                *addr = 0;
                *len = 0;
                #endif
            }
            break;

        default:
            *addr = 0;
            *len = 0;
            break;
        }
    }
}
EXPORT_SYMBOL(get_boot_mem_info);

extern int  arm_add_memory(unsigned long start, unsigned long size);
void __init prom_meminit(void)
{

    unsigned int linux_memory_address = 0, linux_memory_length = 0;
    unsigned int linux_memory2_address = 0, linux_memory2_length = 0;
    unsigned int linux_memory3_address = 0, linux_memory3_length = 0;

    get_boot_mem_info(LINUX_MEM, &linux_memory_address, &linux_memory_length);
    get_boot_mem_info(LINUX_MEM2, &linux_memory2_address, &linux_memory2_length);

    // 2011/0726 modify by mstar: add another memory region for MIU0 high memory part support
    if (linux_memory_length != 0)
    {
        arm_add_memory(linux_memory_address, linux_memory_length);
    }

    if (linux_memory2_length != 0)
        arm_add_memory(linux_memory2_address, linux_memory2_length);

    //printk("hardcode memory add !!!!!!\n");
    //arm_add_memory(0x50000000, 0x10000000);

    printk("\n");
    if(linux_memory_length > 0x10000000)
       printk("LX_MEM  = 0x%X, 0x%X\n", linux_memory_address,0x10000000);
    else
    printk("LX_MEM  = 0x%X, 0x%X\n", linux_memory_address,linux_memory_length);
    printk("LX_MEM2 = 0x%X, 0x%X\n",linux_memory2_address, linux_memory2_length);
    if(linux_memory_length > 0x10000000)
    printk("LX_MEM3 = 0x%X, 0x%X\n",linux_memory3_address, linux_memory3_length);
    //printk("CPHYSADDR(PFN_ALIGN(&_end))= 0x%X\n", CPHYSADDR(PFN_ALIGN(&_end)));
    printk("EMACaddr= 0x%lX\n", EMACaddr);
    printk("EMACmem= 0x%lX\n", EMACmem);
    printk("DRAM_LEN= 0x%lX\n", DRAMlen);

    #if defined(CONFIG_MSTAR_AMBER3) || defined(CONFIG_MSTAR_EDISON)
        if ( DRAMlen > 0x20000000 )
            printk("ENV VARIABLE DRAM_LEN SETTING ERROR!!! LARGER THAN MAXIMUM MAPPING SIZE!!!\n");
    #endif
}

inline unsigned long get_BBAddr(void)
{
    return BBAddr;
}

//void __init prom_free_prom_memory(void)
//{
//    unsigned long addr;
//    int i;
//
//    for (i = 0; i < boot_mem_map.nr_map; i++) {
//        if (boot_mem_map.map[i].type != BOOT_MEM_ROM_DATA)
//            continue;
//
//        addr = boot_mem_map.map[i].addr;
//        free_init_pages("prom memory",
//                addr, addr + boot_mem_map.map[i].size);
//    }
//}

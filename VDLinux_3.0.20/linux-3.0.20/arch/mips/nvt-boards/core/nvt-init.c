/*
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
 */


#include <linux/init.h>
#include <linux/string.h>
#include <linux/kernel.h>

#include <asm/bootinfo.h>
//#include <asm/gt64120.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/cacheflush.h>
#include <asm/traps.h>

#include <asm/mips-boards/prom.h>
#include <asm/mips-boards/generic.h>
#include <asm/mips-boards/bonito64.h>
#include <asm/mips-boards/msc01_pci.h>
#include <asm/sections.h>
#include <linux/module.h>

void kernel_protect_range_get(void);
//#include <asm/mips-boards/malta.h>

unsigned long prom_argc;
int *_prom_argv, *_prom_envp;

/*
 * YAMON (32-bit PROM) pass arguments and environment as 32-bit pointer.
 * This macro take care of sign extension, if running in 64-bit mode.
 */
#define prom_envp(index) ((char *)(long)_prom_envp[(index)])

int init_debug = 0;


/* Bonito64 system controller register base. */
unsigned long _pcictrl_bonito;
unsigned long _pcictrl_bonito_pcicfg;

/* GT64120 system controller register base */
unsigned long _pcictrl_gt64120;

/* MIPS System controller register base */
unsigned long _pcictrl_msc;

char *prom_getenv(char *envname)
{
	/*
	 * Return a pointer to the given environment variable.
	 * In 64-bit mode: we're using 64-bit pointers, but all pointers
	 * in the PROM structures are only 32-bit, so we need some
	 * workarounds, if we are running in 64-bit mode.
	 */
	size_t i;
	int index=0;

	i = strlen(envname);

	while (prom_envp(index)) {
		if(strncmp(envname, prom_envp(index), i) == 0) {
			return(prom_envp(index+1));
		}
		index += 2;
	}

	return NULL;
}

#if 0
static inline unsigned char str2hexnum(unsigned char c)
{
	if (c >= '0' && c <= '9')
		return (unsigned char)(c - '0');
	if (c >= 'a' && c <= 'f')
		return (unsigned char)(c - 'a' + 10);
	return 0; /* foo */
}

static inline void str2eaddr(unsigned char *ea, unsigned char *str)
{
	int i;

	for (i = 0; i < 6; i++) {
		unsigned char num;

		if((*str == '.') || (*str == ':'))
			str++;
		num = str2hexnum(*str++) << 4;
		num |= (str2hexnum(*str++));
		ea[i] = num;
	}
}

int get_ethernet_addr(char *ethernet_addr)
{
        char *ethaddr_str;

        ethaddr_str = prom_getenv("ethaddr");
	if (!ethaddr_str) {
	        printk("ethaddr not set in boot prom\n");
		return -1;
	}
	str2eaddr(ethernet_addr, ethaddr_str);

	if (init_debug > 1) {
	        int i;
		printk("get_ethernet_addr: ");
	        for (i=0; i<5; i++)
		        printk("%02x:", (unsigned char)*(ethernet_addr+i));
		printk("%02x\n", *(ethernet_addr+i));
	}

	return 0;
}
#endif

#ifdef CONFIG_SERIAL_8250_CONSOLE
static void __init console_config(void)
{
	char console_string[40];
	int baud = 0;
	char parity = '\0', bits = '\0', flow = '\0';
	char *s;
        pr_info("CMDLINE:%s\n", prom_getcmdline());
	if ((strstr(prom_getcmdline(), "console=")) == NULL) {
		s = prom_getenv("modetty0");
		if (s) {
			while (*s >= '0' && *s <= '9')
				baud = baud*10 + *s++ - '0';
			if (*s == ',') s++;
			if (*s) parity = *s++;
			if (*s == ',') s++;
			if (*s) bits = *s++;
			if (*s == ',') s++;
			if (*s == 'h') flow = 'r';
		}
		if (baud == 0)
			baud = 38400;
		if (parity != 'n' && parity != 'o' && parity != 'e')
			parity = 'n';
		if (bits != '7' && bits != '8')
			bits = '8';
		if (flow == '\0')
			flow = 'r';
		snprintf(console_string, strlen(console_string), " console=ttyS0,%d%c%c%c", baud, parity, bits, flow);
		strncat(prom_getcmdline(), console_string, strlen(console_string));
		pr_info("Config serial console:%s\n", console_string);
	}
}
#endif

#if 0
static void __init mips_nmi_setup(void)
{
	void *base;
	extern char except_vec_nmi;

	base = cpu_has_veic ?
		(void *)(CAC_BASE + 0xa80) :
		(void *)(CAC_BASE + 0x380);
	memcpy(base, &except_vec_nmi, 0x80);
	flush_icache_range((unsigned long)base, (unsigned long)base + 0x80);
}

static void __init mips_ejtag_setup(void)
{
	void *base;
	extern char except_vec_ejtag_debug;

	base = cpu_has_veic ?
		(void *)(CAC_BASE + 0xa00) :
		(void *)(CAC_BASE + 0x300);
	memcpy(base, &except_vec_ejtag_debug, 0x80);
	flush_icache_range((unsigned long)base, (unsigned long)base + 0x80);
}
#endif

static int __cpuinitdata mem_hi = -1, mem_lo = -1;

static int __init mem_setup(char *str)
{
        if(mem_lo == -1)
                get_option(&str, &mem_lo);
        else
                get_option(&str, &mem_hi);

        return 1;
}

__setup("mem=", mem_setup);

unsigned long Ker_ROSectionStart = 0xffffffff, Ker_ROSectionEnd = 0xffffffff;
unsigned long Ker_LoSpaceStart = 0xffffffff, Ker_LoSpaceEnd = 0xffffffff;
unsigned long Ker_HiSpaceStart = 0xffffffff, Ker_HiSpaceEnd = 0xffffffff;
void kernel_protect_range_get(void)
{
        Ker_ROSectionStart = (unsigned long)_text & 0x0FFFFFFF;
        Ker_ROSectionEnd = (unsigned long)(__end_rodata - 4) & 0x0FFFFFFF;
        Ker_LoSpaceStart = (unsigned long)_text & 0x0FFFFFFF;
        Ker_LoSpaceEnd = (unsigned long)(mem_lo*1024*1024 - 4);
        if(mem_hi != -1)
	{
		unsigned long reg;
		reg = *(volatile unsigned long *)(0xBD020100); //get CPU ID
		reg &= 0xFFFF;
		if(reg == 0x2569) // for NT13
		{
			Ker_HiSpaceStart = (unsigned long)0x50000000; //start from 1280M
			Ker_HiSpaceEnd = (unsigned long)((mem_hi*1024*1024 - 4)|0x50000000);
		}
		else 	
		{
			Ker_HiSpaceStart = (unsigned long)0x30000000; //start from 768M
			Ker_HiSpaceEnd = (unsigned long)((mem_hi*1024*1024 - 4)|0x30000000);
		}
		printk("============================================================\n");
		printk("Kernel range1 [RO sections]: 0x%08x ~ 0x%08x\n", (unsigned int)Ker_ROSectionStart, (unsigned int)Ker_ROSectionEnd);
		printk("Kernel range2 [low space]: 0x%08x ~ 0x%08x\n", (unsigned int)Ker_LoSpaceStart, (unsigned int)Ker_LoSpaceEnd);
		printk("Kernel range3 [high space]: 0x%08x ~ 0x%08x\n", (unsigned int)Ker_HiSpaceStart, (unsigned int)Ker_HiSpaceEnd);
		printk("============================================================\n");
	}
		else
		{
			printk("============================================================\n");
			printk("Kernel range1 [RO sections]: 0x%08x ~ 0x%08x\n", (unsigned int)Ker_ROSectionStart, (unsigned int)Ker_ROSectionEnd);
			printk("Kernel range2 [low space]: 0x%08x ~ 0x%08x\n", (unsigned int)Ker_LoSpaceStart, (unsigned int)Ker_LoSpaceEnd);
			printk("============================================================\n");
		}
	}

extern struct plat_smp_ops msmtc_smp_ops;

void __init prom_init(void)
{
	prom_argc = fw_arg0;
        _prom_argv = (int *) fw_arg1;
        _prom_envp = (int *) fw_arg2;

        mips_display_message("LINUX");

	set_io_port_base(KSEG1);	
	board_nmi_handler_setup = NULL; // mips_nmi_setup
	board_ejtag_handler_setup = NULL; // mips_ejtag_setup

	pr_info("\nLINUX started...\n");
	prom_init_cmdline();
	prom_meminit();
#ifdef CONFIG_SERIAL_8250_CONSOLE
	console_config();
#endif
#ifdef CONFIG_MIPS_CMP
	register_smp_ops(&cmp_smp_ops);
#endif
#ifdef CONFIG_MIPS_MT_SMP
	register_smp_ops(&vsmp_smp_ops);
#endif
#ifdef CONFIG_MIPS_MT_SMTC
	register_smp_ops(&msmtc_smp_ops);
#endif
}
EXPORT_SYMBOL(Ker_ROSectionStart);
EXPORT_SYMBOL(Ker_ROSectionEnd);
EXPORT_SYMBOL(Ker_LoSpaceStart);
EXPORT_SYMBOL(Ker_LoSpaceEnd);
EXPORT_SYMBOL(Ker_HiSpaceStart);
EXPORT_SYMBOL(Ker_HiSpaceEnd);


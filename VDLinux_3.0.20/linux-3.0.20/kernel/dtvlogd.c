/*
 * dtvlogd.c
 *
 * Copyright (C) 2011, 2012 Samsung Electronics
 * Created by choi young-ho, kim gun-ho, mun kye-ryeon
 *
 * This used to save console message to RAM area.
 *
 * NOTE:
 *     DTVLOGD       - Automatically store printk/printf messages
 *                     to kernel buffer whenever developer use printk/printf.
 *     USBLOG        - Periodically store DTVLOGD buffer to USB
 *                     Storage when there are dtvlogd.conf in usb.
 *     EMERGENCY LOG - Automatically store printk/printf messages
 *                     to kernel ouside buffer. Saved messages would
 *                     be stored once on /mtd_rwarea/emeg_log.txt at
 *                     emergency boot time after crash the system.
 *                     This function reuse DTVLOGD routine. So it has
 *                     dependency with DTVLOGD.
 *
 * KERNEL DEFAULT CONFIG:
 *     [ ] DTVLOGD   -+-- [ ] USBLOG        // use this only for debug.
 *                    |
 *                    +-- [ ] EMERGENCY LOG // use this after fix the kernel
 *                                             outside buffer address.
 * CAUTION:
 *
 * If you are now porting DTVLOGD to new architecture, please be careful.
 * The emergency log will crash the memory, if you don't modify Emergency Log
 * Buffer address.
 * Previous address would be set with the other archtecture address.
 *
 * You should fix it first with current architecture memory map
 * or temporarily disable it.
 */

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/console.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/nmi.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/interrupt.h>            /* For in_interrupt() */
#include <linux/delay.h>
#include <linux/smp.h>
#include <linux/security.h>
#include <linux/bootmem.h>
#include <linux/syscalls.h>
#include <linux/proc_fs.h>
#include <linux/ctype.h>
#include <linux/namei.h>
#include <linux/vfs.h>
#include <linux/fcntl.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/kthread.h>
#include <asm/system.h>

/* Control Version */
#define DTVLOGD_VERSION	"4.1.1"

/* Control Preemption */
#define DTVLOGD_PREEMPT_COUNT_SAVE(cnt) \
	do {				\
		cnt = preempt_count();	\
		while (preempt_count())	\
			preempt_enable_no_resched(); \
	} while (0)

#define DTVLOGD_PREEMPT_COUNT_RESTORE(cnt) \
	do {				\
		int i;			\
		for (i = 0; i < cnt; i++)	\
			preempt_disable(); \
	} while (0)


#define USBLOG_SAVE_INTERVAL	5 /* Interval at which Log is saved to USB */

/* Buffer Size Config */
#define CONFIG_DLOG_BUF_SHIFT     17  /* 128KB */

/* Dtvlogd main Log Buffer */
#define __DLOG_BUF_LEN	(1 << CONFIG_DLOG_BUF_SHIFT)
#define DLOG_BUF_MASK	(dlog_buf_len - 1)
#define DLOG_BUF(idx)	dlog_buf[(idx) & DLOG_BUF_MASK]
#define DLOG_INDEX(idx)	((idx) & DLOG_BUF_MASK)


int opt_dtvlogd; /* Flag to track USB logging */

#if (defined(CONFIG_DTVLOGD_HUB_KDEBUGD) \
		|| defined(CONFIG_DTVLOGD_PROC_READ) \
		|| defined(CONFIG_DTVLOGD_KERN_READ_CALLER_BUFFER) \
		|| defined(CONFIG_DTVLOGD_KERN_READ_SELF_BUFFER))
#define  DTVLOGD_ENABLE_LOGGING_START_STOP
#endif

#ifdef DTVLOGD_ENABLE_LOGGING_START_STOP
static int dtvlogd_write_start_stop = 1; /* Flag to write DTVLOGD Buffer */
#endif

static char __dlog_buf[__DLOG_BUF_LEN];
static char *dlog_buf = __dlog_buf;
static int dlog_buf_len = __DLOG_BUF_LEN;

static int dlogbuf_start, dlogbuf_end;
static int dlogged_chars;
static int dlogbuf_writeok;
static int dlogbuf_writefail;

static DEFINE_SPINLOCK(dlogbuf_lock);
static DEFINE_SEMAPHORE(dlogbuf_sem);

static struct completion dlogbuf_completion;

wait_queue_head_t dlogbuf_wq;
int dlogbuf_complete;
#ifdef CONFIG_DTVLOGD_PROC_READ
static atomic_t proc_is_open = ATOMIC_INIT(0);
#endif

/*
 * Emergency Log Memory Map
 *
 * +--------------------------+ 0x0002 0000 (128KB)
 * |  Reserved Area           |
 * |   (63.9KB)               |
 * +--------------------------+ 0x0001 0010 (64KB + 16 Bytes)
 * | Mgmt Data (16 Bytes)     |
 * +--------------------------+ 0x0001 0000 (64KB)
 * | Emeg Log Buffer          |
 * |    (64KB)    (B)         |
 * +--------------------------+ 0x0000 0000 (0KB)    ( A )
 *
 * 128KB Memory must be assigned for this.
 *
 * (A) - This is the base address of emergnecy log.
 *       Base address is defined with CONFIG_DTVLOGD_EMEG_LOGD_BUFFER_ADDRESS.
 *       The user can configure on kernel configuration.
 *
 * (B) - This is the buffer size to save emergency log.
 *       Currently 64KB characters are saved.
 *       __DRAM_BUF_LEN is defined for this.
 *
 *       Don't increase this value by force, basic implementation was done
 *       with this size. If you fix it, it can do wrong operation.
 */

#ifdef CONFIG_DTVLOGD_EMERGENCY_LOG

#if defined(CONFIG_ARCH_CCEP)
#define __DRAM_BUF_LEN	(256 * 1024)       /* Current 256Kb */
#elif defined(CONFIG_NVT_NT72568)
#define __DRAM_BUF_LEN	(128 * 1024)       /* Current 128Kb */
#elif defined(CONFIG_MSTAR_AMBER3)
#define __DRAM_BUF_LEN	(256 * 1024)       /* PreX12 : 256Kb */
#elif defined(CONFIG_MSTAR_EDISON)
#define __DRAM_BUF_LEN	(256 * 1024)       /* X12 : 256Kb */
#elif defined(CONFIG_MSTAR_EMERALD)
#define __DRAM_BUF_LEN	(256 * 1024)       /* X13 : 256Kb */
#else
#define __DRAM_BUF_LEN	(64 * 1024)        /* Default 64Kb */
#endif

#define DRAM_BUF_MASK	(__DRAM_BUF_LEN - 1)
#define DRAM_INDEX(idx)	((idx) & DRAM_BUF_MASK)

#ifdef CONFIG_DTVLOGD_DYNAMIC_BUFFER_ADDRESS
resource_size_t RAM_BUF_ADDR = CONFIG_DTVLOGD_EMEG_LOGD_BUFFER_ADDRESS;
EXPORT_SYMBOL(RAM_BUF_ADDR);
#else
#define RAM_BUF_ADDR CONFIG_DTVLOGD_EMEG_LOGD_BUFFER_ADDRESS
#endif

/*
 * We need to set CONFIG_DTVLOGD_EMEG_LOGD_BUFFER_ADDRESS in .config file.
 * If RAM_BUF_ADDR use 0, We can not compile.
 */
#if (CONFIG_DTVLOGD_EMEG_LOGD_BUFFER_ADDRESS == 0)
#error "CONFIG_DTVLOGD_EMEG_LOGD_BUFFER_ADDRESS is 0"
#endif

/*
 * The dtvlogd need to use alignment memory with following:
 * (64KB) 0xFFFF & CONFIG_DTVLOGD_EMEG_LOGD_BUFFER_ADDRESS
 */
#if (DRAM_INDEX(CONFIG_DTVLOGD_EMEG_LOGD_BUFFER_ADDRESS))
#error "CONFIG_DTVLOGD_EMEG_LOGD_BUFFER_ADDRESS isn't aligned with 0xFFFF"
#endif

#define RAM_DATA_ADDR(buf_addr)  (buf_addr + __DRAM_BUF_LEN)

#define RAM_INFO_ADDR(base)    (base + 0x0)
#define RAM_START_ADDR(base)   (base + 0x4)
#define RAM_END_ADDR(base) (base + 0x8)
#define RAM_CHARS_ADDR(base)   (base + 0xC)

const char *RAM_FLUSH_FILE = "/mtd_rwarea/emeg_log.txt";

static int dram_buf_len = __DRAM_BUF_LEN;

void __iomem *ram_char_buf;
#ifdef CONFIG_SCHED_HISTORY_ON_DDR
void __iomem *ram_context_switch_log_buf;
#endif
void __iomem *ram_info_buf;
static int ramwrite_start;
static int ram_clear_flush;
static unsigned int ram_log_magic;

static int drambuf_start, drambuf_end, drambuf_chars;
#endif

/* Enable/Disable debugging */
#ifdef CONFIG_DTVLOGD_DEBUG
/* This flag is used to test the logging into flash.
 * Sometimes, it's difficult to get the way to not reset RAM.
 * This can be used in such scenarios to verify the general
 * functionality of emergency logging without having to reboot
 * Available only in DTVLOGD_DEBUG
 */
#define TEST_DTV_EMERGENCY_LOG
#if defined(TEST_DTV_EMERGENCY_LOG) && defined(CONFIG_DTVLOGD_EMERGENCY_LOG)
int test_do_ram_flush(void)
{
	/* currently mtd_rwarea is read-only on X6 */
	RAM_FLUSH_FILE = "/tmp/emeg_log.txt";
	ram_clear_flush = 1;
	return 0;
}
EXPORT_SYMBOL(test_do_ram_flush);
#endif

#define ddebug_info(fmt, args...) {		\
		printk(KERN_ERR			\
			"dtvlogd::%s " fmt,	\
			__func__, ## args);	\
}

#define ddebug_enter(void) {			\
		printk(KERN_ERR			\
			"dtvlogd::%s %d\n",	\
			__func__, __LINE__);	\
}

#define ddebug_err(fmt, args...) {			\
		printk(KERN_ERR				\
			"ERR:: dtvlogd::%s %d " fmt,	\
			__func__, __LINE__, ## args);	\
}

#else
#define ddebug_err(fmt, args...) do {} while (0)
#define ddebug_info(fmt, args...) do {} while (0)
#define ddebug_enter(void) do {} while (0)
#endif

#define ddebug_print(fmt, args...) {		\
		printk(KERN_CRIT		\
			 fmt, ## args);	\
}

/*
 * Uninitialized memory operation
 */
#ifdef CONFIG_DTVLOGD_EMERGENCY_LOG
static void write_ram_info(void)
{
	writel(drambuf_start, (void __iomem *)RAM_START_ADDR(ram_info_buf));
	writel(drambuf_end, (void __iomem *)RAM_END_ADDR(ram_info_buf));
	writel(drambuf_chars, (void __iomem *)RAM_CHARS_ADDR(ram_info_buf));
}
#endif

/*
 * rambuf_init()
 */
#ifdef CONFIG_DTVLOGD_EMERGENCY_LOG
static void rambuf_init(void)
{
	unsigned int magic = 0xbabe;
	unsigned int temp;
	resource_size_t ram_buf_addr = 0;

#ifdef CONFIG_SCHED_HISTORY_ON_DDR
	resource_size_t ram_context_switch_log_buf_addr = 0;
#endif
	ram_buf_addr = RAM_BUF_ADDR;

	ram_char_buf = ioremap(ram_buf_addr, dram_buf_len);
	ram_info_buf = ioremap(RAM_DATA_ADDR(ram_buf_addr), sizeof(u32) * 4);

#ifdef CONFIG_SCHED_HISTORY_ON_DDR
	ram_context_switch_log_buf_addr = RAM_DATA_ADDR(ram_buf_addr) + sizeof(u32) * 4;
	ram_context_switch_log_buf = ioremap(ram_context_switch_log_buf_addr,0x3000);	/* 3page */
#endif

	temp = (unsigned int)readl(ram_info_buf);

	ddebug_info("magic number read: %x (%x)\n", temp, magic);
	ram_log_magic = temp;
	if (magic == temp) {
		ram_clear_flush = 1;
	} else {
		ramwrite_start = 1;
		drambuf_start = drambuf_end = drambuf_chars = 0;
		writel(magic, ram_info_buf);
	}
}
#endif

/*
 *	USB I/O FUNCTIONS
 */

#define LOGNAME_SIZE	64

char logname[LOGNAME_SIZE];

/*
 *	print_option()
 */

static void print_option(void)
{
	int mylen = dlog_buf_len;
	int mystart = dlogbuf_start;
	int myend = dlogbuf_end;
	int mychars = dlogged_chars;

	ddebug_print("\n\n\n");
	ddebug_print("==================================================\n");
	ddebug_print("= DTVLOGD v%s\n", DTVLOGD_VERSION);
	ddebug_print("==================================================\n");
#ifdef CONFIG_DTVLOGD_USB_LOG
	ddebug_print("USB Log File   = %s\n",
		(logname[0] == '/') ? logname : "(not found)");
	ddebug_print("Use DTVLOGD?   = %s\n", (opt_dtvlogd) ? "Yes" : "No");
	ddebug_print("Save Interval  = %d sec\n", USBLOG_SAVE_INTERVAL);
#else
	ddebug_print("DTVLOGD - USB Log : disabled by kernel config\n");
#endif

#ifdef CONFIG_DTVLOGD_EMERGENCY_LOG
	ddebug_print("EMERG MAGIC    = 0x%x (0xbabe)\n", ram_log_magic);
#endif
	ddebug_print("\n");

	ddebug_print("Buffer Size    = %d\n", mylen);
	ddebug_print("Start Index    = %d\n", mystart);
	ddebug_print("End Index      = %d\n", myend);
	ddebug_print("Saved Chars    = %d\n", mychars);
	ddebug_print("Write Ok       = %d\n", dlogbuf_writeok);
	ddebug_print("Write Fail     = %d\n", dlogbuf_writefail);
	ddebug_print("\n");

}



#ifdef DTVLOGD_ENABLE_LOGGING_START_STOP
void dtvlogd_write_start(void)
{
	dtvlogd_write_start_stop = 1;
}

void dtvlogd_write_stop(void)
{
	dtvlogd_write_start_stop = 0;
}
#endif


/*
 *	usblog_open()
 */

static struct file *usblog_open(void)
{
	struct file *fp;

	fp = filp_open(logname, O_CREAT|O_WRONLY|O_APPEND|O_LARGEFILE, 0600);

	return fp;
}

/*
 *	usblog_close()
 */

int usblog_close(struct file *filp, fl_owner_t id)
{
	return filp_close(filp, id);
}

/*
 *	__usblog_write()
 */


static int __usblog_write(struct file *file, char *str, int len)
{
	int ret;

	ret = file->f_op->write(file, str, len, &file->f_pos);

	return 0;
}

/*
 *	usblog_write()
 */

void usblog_write(char *s, int len)
{
	struct file *file;

	file = usblog_open();

	if (!IS_ERR(file)) {
		__usblog_write(file, s, len);
		usblog_close(file, NULL);
		ddebug_info("log flushed");
		dlogbuf_writeok++;
	} else {
		ddebug_err("error in opening USB file, disable usb logging\n");
		memset(logname, 0, LOGNAME_SIZE);
		opt_dtvlogd = 0;
		dlogbuf_writefail++;
	}
}

/*
 *	usblog_flush()
 *
 *
 *	Flush all buffer message to usb.
 */

void usblog_flush(void)
{
#ifdef CONFIG_DTVLOGD_USB_LOG
	static char flush_buf[__DLOG_BUF_LEN];
	int len = 0;

	if (dlogged_chars == 0) {
		ddebug_info("no characters logged\n");
		return;
	}

	if (!opt_dtvlogd) {
		ddebug_err("usb log not enabled\n");
		return;
	}

	spin_lock_irq(&dlogbuf_lock);

	if (DLOG_INDEX(dlogbuf_start) >= DLOG_INDEX(dlogbuf_end))
		len = dlog_buf_len - DLOG_INDEX(dlogbuf_start);
	else
		len = DLOG_INDEX(dlogbuf_end) - DLOG_INDEX(dlogbuf_start);

	memcpy(&flush_buf[0], &DLOG_BUF(dlogbuf_start), len);

	if (DLOG_INDEX(dlogbuf_start) >= DLOG_INDEX(dlogbuf_end)) {
		memcpy(&flush_buf[len], &DLOG_BUF(0), DLOG_INDEX(dlogbuf_end));
		len += DLOG_INDEX(dlogbuf_end);
	}

	dlogbuf_start = dlogbuf_end;
	dlogged_chars = 0;

	spin_unlock_irq(&dlogbuf_lock);

	usblog_write(flush_buf, len);
#endif
}


#ifdef CONFIG_DTVLOGD_HUB_KDEBUGD
int dtvlogd_buffer_printf(void)
{
	int len, i;
	int start, end, chars;


	ddebug_print("*******************************************\n");
	ddebug_print("******** Emergency Log printing *************\n");
	ddebug_print("*******************************************\n\n\n\n");

	spin_lock_irq(&dlogbuf_lock);
	dtvlogd_write_stop();

	start = (int)readl((void __iomem *)RAM_START_ADDR(ram_info_buf));
	end  = (int)readl((void __iomem *)RAM_END_ADDR(ram_info_buf));
	chars = (int)readl((void __iomem *)RAM_CHARS_ADDR(ram_info_buf));

	if (DRAM_INDEX(start) >= DRAM_INDEX(end))
		len = dram_buf_len - DRAM_INDEX(start);
	else
		len = DRAM_INDEX(end) - DRAM_INDEX(start);


	for (i = start ; i < (start + len) ; i++)
		printk("%c", (char)readb(ram_char_buf + DRAM_INDEX(i)));

	if (DRAM_INDEX(start) >= DRAM_INDEX(end)) {
		for (i = 0 ; i < DRAM_INDEX(end) ; i++)
			printk("%c", (char)readb(ram_char_buf + DRAM_INDEX(i)));
		len += DRAM_INDEX(end);
	}

	dtvlogd_write_start();
	spin_unlock_irq(&dlogbuf_lock);

	return 1;
}
#endif


#ifdef CONFIG_DTVLOGD_EMERGENCY_LOG
static void ram_flush(void)
{
	char ram_logname[LOGNAME_SIZE];
	static char flush_buf[__DRAM_BUF_LEN];
	int len, i;
	int count = 0;
	int start, end, chars;
	struct file *fp;

	ddebug_print("\n\n\n*******************************************\n");
	ddebug_print("*******************************************\n");
	ddebug_print("******** Emergency Log Backup *************\n");
	ddebug_print("*******************************************\n");
	ddebug_print("*******************************************\n\n\n\n");


	snprintf(ram_logname, LOGNAME_SIZE, RAM_FLUSH_FILE);
	ddebug_info("ram_logname: %s\n", RAM_FLUSH_FILE);

	fp = filp_open(ram_logname, O_CREAT|O_WRONLY|O_TRUNC|O_LARGEFILE, 0644);
	if (IS_ERR(fp))	{
		ddebug_err("error in opening log file\n");
		return;
	}

	spin_lock_irq(&dlogbuf_lock);

	start = (int)readl((void __iomem *)RAM_START_ADDR(ram_info_buf));
	end  = (int)readl((void __iomem *)RAM_END_ADDR(ram_info_buf));
	chars = (int)readl((void __iomem *)RAM_CHARS_ADDR(ram_info_buf));

	if (DRAM_INDEX(start) >= DRAM_INDEX(end))
		len = dram_buf_len - DRAM_INDEX(start);
	else
		len = DRAM_INDEX(end) - DRAM_INDEX(start);

	for (i = start ; i < (start + len) ; i++) {
		flush_buf[count] = (char)readb(ram_char_buf + DRAM_INDEX(i));
		count++;
	}

	if (DRAM_INDEX(start) >= DRAM_INDEX(end)) {
		for (i = 0 ; i < DRAM_INDEX(end) ; i++) {
			flush_buf[count] =
				(char)readb(ram_char_buf + DRAM_INDEX(i));
			count++;
		}
		len += DRAM_INDEX(end);
	}

	spin_unlock_irq(&dlogbuf_lock);

	fp->f_op->write(fp, flush_buf, len, &fp->f_pos);

	filp_close(fp, NULL);
	ddebug_info("file written to flash\n");

	ramwrite_start = 1;
	drambuf_start = drambuf_end = drambuf_chars = 0;
}
#endif

static void dlogbuf_write(char c)
{
	/* this function gets called from printk and printf,
	 * thus ideally irq needs to be save and restored.
	 *
	 * overhead of locking is for every character sent for print
	 *
	 * if corruption occurs, it would be a few characters only,
	 * however, in testing, there hasn't been any corruption.
	 *
	 * If required, protect it using
	 * - spin_lock_irqsave(&dlogbuf_lock_i, flags);
	 * - spin_unlock_irqrestore(&dlogbuf_lock_i, flags);
	 */
	DLOG_BUF(dlogbuf_end) = c;

#ifdef CONFIG_DTVLOGD_EMERGENCY_LOG
	if (ramwrite_start == 1) {
		writeb(c, ram_char_buf + DRAM_INDEX(drambuf_end));
		drambuf_end++;

		if (drambuf_end - drambuf_start > dram_buf_len)
			drambuf_start = drambuf_end - dram_buf_len;
		if (drambuf_chars < dram_buf_len)
			drambuf_chars++;

		write_ram_info();
	}
#endif

	dlogbuf_end++;

	if (dlogbuf_end - dlogbuf_start > dlog_buf_len)
		dlogbuf_start = dlogbuf_end - dlog_buf_len;
	if (dlogged_chars < dlog_buf_len)
		dlogged_chars++;
}

int dtvlogd_print(char *str, int len)
{
	int printed_len = 0;
	char *p;
	static int start_newline = 1;
	static char print_buf[1024];

	/* Prevent overflow */
	if (len > 1023)
		len = 1023;

	printed_len = snprintf(print_buf, len + 1, "%s\n", str);

	/* Copy the output into log buf */
	for (p = print_buf; *p ; p++) {
		if (start_newline) {
			start_newline = 0;
			if (!*p)
				break;
		}

		dlogbuf_write(*p);

		if (*p == '\n')
			start_newline = 1;
	}
	return printed_len;
}

/*
 *	is_hw_reset()
 */

int is_hw_reset(const char *str, int len)
{
	int i;
	/*
	 * power off (h/w reset) serial protocol
	 */
	static char hw_reset[9] = { 0xff, 0xff, 0x1d, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x1d };
	static char hw_reset2[9] = { 0xff, 0xff, 0x12, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x12 };

	if (len != 9)
		return 0;

	for (i = 0 ; i < 9 ; i++) {
		if ((str[i] != hw_reset[i]) && (str[i] != hw_reset2[i]))
			return 0;
	}

	ddebug_info("hw reset found\n");
	return 1;
}


/*
 * @str   : Serial string (printf, printk msg)
 * @count : Serial string count
 * @type  : 1->printf, 2->printk
 */

int dtvlogd_write(const char __user *str, int count, int type)
{
	int len;

	char usr_buf[256];
	char *buf = NULL;

#ifdef DTVLOGD_ENABLE_LOGGING_START_STOP
	if (!dtvlogd_write_start_stop)
		return 1;
#endif

	if (type == 1) {
		/* printf */

		/* Prevent overflow */
		if (count > 256)
			count = 256;

		if (copy_from_user(usr_buf, str, count)) {
			ddebug_err("printf::error\n");
			return -EFAULT;
		}

		buf = usr_buf;
	} else if (type == 2) {
		/* printk */
		buf = (char *)str;
	} else {
		BUG();
	}

	/*
	 * Don't save micom command. It's not a ascii character.
	 * The exeDSP write a micom command to serial.
	 */

	if (!isascii(buf[0]) || count == 0) {
#if defined(CONFIG_DTVLOGD_USB_LOG) || defined(CONFIG_DTVLOGD_EMERGENCY_LOG)
		if (is_hw_reset(buf, count)) {
			ddebug_info("sending power off\n");
			/*
			 * flush the messages remaining in dlog buffer,
			 * and disable magic number
			 */
			do_dtvlog(5, NULL, 1); /* len=1 means power off */
		}
#endif
		return 1;
	}

	len = dtvlogd_print(buf, count);

	return 1;
}

/*
 *	LOG FUNCTIONS
 */

/*
 *  Referenced do_syslog()
 *
 *  Commands to do_dtvlog:
 *
 *      3 -- Write printk messages to dlog buffer.
 *      4 -- Read and clear all messages remaining in the dlog buffer.
 *           This is called internally by dtvlogd.
 *           External callers should use command 5.
 *      5 -- Synchronously read and clear all messages remaining in
 *           the dlog buffer.
 *           This should be used by external callers as it provides protection.
 *      6 -- Read from ram buffer when booting time.
 *     10 -- Print number of unread characters in the dtvlog buffer.
 *     11 -- Write printf messages to dlog buffer.
 */
int do_dtvlog(int type, const char __user *buf, int len)
{
	int error = 0;
#ifdef CONFIG_DTVLOGD_USB_LOG
	int preempt;
#endif
	int down_ret;

	switch (type) {
	case 3:
		/* Write some data to ring buffer. */
		dtvlogd_write(buf, len, 2);     /* from printk */
		break;
	case 4:
#ifdef CONFIG_DTVLOGD_USB_LOG
		/* actual usb flush is done in this function
		 * all the external users should use
		 * - do_dtvlog(5, ...)
		 */
		down_ret = down_interruptible(&dlogbuf_sem);
		usblog_flush();
		up(&dlogbuf_sem);

		if (dlogbuf_complete) {
			complete(&dlogbuf_completion);
			dlogbuf_complete = 0;
		}
#endif
		break;
	case 5:
#ifdef CONFIG_DTVLOGD_EMERGENCY_LOG
		/*
		 * When die or panic was occurred, write magic number
		 *
		 *   - len = 0 : this call came from panic, die and stop
		 *   - len = 1 : this call came from power off
		 */
		if (len == 1) {
			ddebug_info("erasing magic number\n");
			writel(0xdead, ram_info_buf);
		}
#endif
#ifdef CONFIG_DTVLOGD_USB_LOG
		if (!opt_dtvlogd)
			break;

		DTVLOGD_PREEMPT_COUNT_SAVE(preempt);
		dlogbuf_complete = 1;

		/*
		 * this triggers the dtvlogd daemon to do actual usb flushing
		 */

		wake_up_interruptible(&dlogbuf_wq);

		wait_for_completion(&dlogbuf_completion);
		DTVLOGD_PREEMPT_COUNT_RESTORE(preempt);
#endif
		break;
#ifdef CONFIG_DTVLOGD_EMERGENCY_LOG
	case 6:
		down_ret = down_interruptible(&dlogbuf_sem);
		ram_flush();
		up(&dlogbuf_sem);
		break;
#endif
	case 9:
		/* Number of chars in log buffer */
		error = dlogbuf_end - dlogbuf_start;
		break;
	case 10:
		/* Print number of unread characters in the dtvlog buffer. */
		print_option();
		break;

	case 11:
		/* Write some data to ring buffer. */
		dtvlogd_write(buf, len, 1);     /* from printf */
		break;

#if defined(TEST_DTV_EMERGENCY_LOG) && defined(CONFIG_DTVLOGD_EMERGENCY_LOG)
	case 12:
		test_do_ram_flush();
		break;
#endif
	default:
		error = -EINVAL;
		break;
	}

	return error;

}


/*
 *	dtvlogd()
 */

static int dtvlogd(void *unused)
{
#ifdef CONFIG_DTVLOGD_EMERGENCY_LOG
	rambuf_init();
	ddebug_info("rambuf_init done\n");
#endif

	/* run after 15sec */
	wait_event_interruptible_timeout(dlogbuf_wq, dlogbuf_complete, 15*HZ);

	while (!kthread_should_stop()) {
#ifdef CONFIG_DTVLOGD_EMERGENCY_LOG
		if (ram_clear_flush == 1) {
			/*
			 * read from RAM buffer and create emergency log file
			 */
			do_dtvlog(6, NULL, 0);
			ram_clear_flush = 0;
		}
#endif

#ifdef CONFIG_DTVLOGD_USB_LOG
		if (opt_dtvlogd) {
			/* flush the dlog buffer to a USB file */
			do_dtvlog(4, NULL, 0);
		}
#endif

		wait_event_interruptible_timeout(dlogbuf_wq,
						dlogbuf_complete,
						USBLOG_SAVE_INTERVAL*HZ);
	}

	return 0;
}

#ifdef CONFIG_DTVLOGD_USB_LOG
void dtvlogd_usblog_start(char *mount_dir)
{
	int len = strnlen(mount_dir, LOGNAME_SIZE - 1);
	ddebug_enter();

	ddebug_info("strlen(mount_dir) = %d\n", strlen(mount_dir));

	memset(logname, 0, LOGNAME_SIZE);
	memcpy(logname, mount_dir, len);

	/* length can't be zero as its passed from user,
	 * added check just for prevent warning
	 *  - for negative offset in array
	 */
	if (!len)
		return;

	if ((logname[len-1] < '0') || (logname[len-1] > 'z'))
		logname[len-1] = 0;

	/* We try to append the file-name to the directory
	 * provided by user. In case of error, full filename won't be written.
	 */
	strncat(logname, "/log.txt", LOGNAME_SIZE - len - 1);

	ddebug_info("setting usb log file: %s\n", logname);

	opt_dtvlogd = 1;
}

void dtvlogd_usblog_stop(void)
{
	ddebug_enter();

	if (!opt_dtvlogd) {
		ddebug_print("Dtvlogd Usb Log is not started -it is  Already Stop\n");
		return;
	}

	do_dtvlog(5, NULL, 0);	/* flush the remained log */

	ddebug_info("reset usb log\n");
	memset(logname, 0, LOGNAME_SIZE);	/* remove mount location */
	opt_dtvlogd = 0;			/* disable usblog */
}
#endif


/*
 *   ####################
 *
 *	Add /proc/dtvlogd
 *
 *   ####################
 */
#ifdef CONFIG_DTVLOGD_PROC_READ
static int dmsg_release(struct inode *inode, struct file * file)
{
	WARN_ON(!atomic_read(&proc_is_open));
	atomic_set(&proc_is_open, 0);
	return 0;
}

static ssize_t dmsg_open(struct inode *inode, struct file *file)
{
	if (atomic_read(&proc_is_open))
		return -EACCES;

	atomic_set(&proc_is_open, 1);
	return 0;
}

static loff_t dmsg_llseek(struct file *file, loff_t offset, int origin)
{
	long long retval = -EINVAL;
	WARN_ON(!atomic_read(&proc_is_open));

	/* TODO: Check if this is needed, Also test more */
	switch (origin) {
	case SEEK_END:
		offset = dlogged_chars;
		retval = offset;
		break;

	default:
		/* we ignore other values of origin*/
		return retval;
	}

	return retval;
}
#endif

static ssize_t dmsg_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
#ifdef CONFIG_DTVLOGD_PROC_READ
	char ch;
	int ret;
	int read_len;

	WARN_ON(!atomic_read(&proc_is_open));

	dtvlogd_write_stop();
	ddebug_info("count: %d", count);
	ddebug_info("-> dlogbuf_start: %d, dlogbuf_end: %d, dlogged_chars: %d\n",
			dlogbuf_start, dlogbuf_end, dlogged_chars);

	ret = -EINVAL;

	if (!buf)
		goto out;

	ret = 0;
	if (!count)
		goto out;
	if (!access_ok(VERIFY_WRITE, buf, count)) {
		ret = -EFAULT;
		goto out;
	}

	read_len = 0;
	spin_lock_irq(&dlogbuf_lock);
	/* loop the DTVLOGD buffer for no. of characters logged
	 * from dlogbuf_start to dlogbuf_end.
	 *
	 *  - Read the characters from buffer in ch and copy to user
	 *  - Log the no. of characters copied into read_len
	 *  - in case of error, return negative.
	 */
	while (!ret && (dlogbuf_start != dlogbuf_end) && read_len < count) {
		ch = DLOG_BUF(dlogbuf_start);
		dlogbuf_start++;
		dlogged_chars--;
		spin_unlock_irq(&dlogbuf_lock);
		ret = __put_user(ch, buf);
		buf++;
		read_len++;
		cond_resched();
		spin_lock_irq(&dlogbuf_lock);
	}
	spin_unlock_irq(&dlogbuf_lock);
	/* No error, return the no. of characters read so far */
	if (!ret)
		ret = read_len;

out:
	if (ret <= 0) {
		ddebug_info("no more characters (%d)...\n", ret);
		dtvlogd_write_start(); /* resume logging */
	}

	return ret;

#else
	int len;

	if ((file->f_flags & O_NONBLOCK))
		return -EAGAIN;

	if (*ppos > 0)
		return 0;

	len = snprintf(buf, 2, "%d\n", opt_dtvlogd);

	*ppos = len;

	return len;
#endif

}

#ifdef CONFIG_DTVLOGD_KERN_READ_CALLER_BUFFER
/** Get the size of the available dtvlogd buffer.
 *
 * This should be used to pass the argument to dtvlogd_get_buffer()
 *
 */
int dtvlogd_get_bufflen(void)
{
	return dlogged_chars;
}

/** Function to get the dtvlogd buffer from kernel
 *
 * pbuf: A valid buffer atleast count size
 * count: Size of the buffer to copy
 *
 * returns < 0  in case of error
 *         else no. of characters written
 */
int dtvlogd_get_buffer(char *pbuf, int count)
{
	int ret = -EINVAL;
	int len = count;
	int len1;

	/* some sanity */
	if (!pbuf || count < 0)
		goto out;
	ret = 0;
	if (!count || !dlogged_chars)
		goto out;

	dtvlogd_write_stop();
	spin_lock_irq(&dlogbuf_lock);

	/*
	 * the buffer might have been read from user/usblogging thread
	 */
	if (dlogged_chars < len)
		len = dlogged_chars;

	/* get the partial len 1 */
	if (DLOG_INDEX(dlogbuf_start) >= DLOG_INDEX(dlogbuf_end))
		len1 = dlog_buf_len - DLOG_INDEX(dlogbuf_start);
	else
		len1 = DLOG_INDEX(dlogbuf_end) - DLOG_INDEX(dlogbuf_start);

	if (len1 > len) {
		memcpy(&pbuf[0], &DLOG_BUF(dlogbuf_start), len);
	} else {
		memcpy(&pbuf[0], &DLOG_BUF(dlogbuf_start), len1);
		memcpy(&pbuf[len1], &DLOG_BUF(0), (len - len1));
	}

	ret = len;
	spin_unlock_irq(&dlogbuf_lock);
	dtvlogd_write_start();
out:

	return ret;
}
#endif

#ifdef CONFIG_DTVLOGD_KERN_READ_SELF_BUFFER
/*
 * Provide the dtvlogd buffer to the caller.
 * It has two parts:
 * buf1: Start address having len1 as valid length.
 * buf2: Remaining part of buffer having len2 as valid len.
 */
int acquire_dtvlogd_buffer(char **buf1, int *len1, char **buf2, int *len2)
{
	if (!dlogged_chars) {
		*len1 = 0;
		*len2 = 0;
		return 0;
	}

	spin_lock_irq(&dlogbuf_lock);

	*buf1 = &DLOG_BUF(dlogbuf_start);
	if (DLOG_INDEX(dlogbuf_start) >= DLOG_INDEX(dlogbuf_end)) {
		*len1 = dlog_buf_len - DLOG_INDEX(dlogbuf_start);
		*len2 = DLOG_INDEX(dlogbuf_end);
		*buf2 = &DLOG_BUF(0);
	} else {
		*len1 = DLOG_INDEX(dlogbuf_end) - DLOG_INDEX(dlogbuf_start);
		*len2 = 0;
	}

	spin_unlock_irq(&dlogbuf_lock);

	return 0;
}
#endif

#define MAX_DMSG_WRITE_BUFFER	64

static ssize_t dmsg_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
	char buffer[MAX_DMSG_WRITE_BUFFER];
	long idx;

	if (!count)
		return count;

	if (count >= MAX_DMSG_WRITE_BUFFER)
		count = MAX_DMSG_WRITE_BUFFER - 1;

	/*
	 * Prevent Tainted Scalar Warning:
	 * Buffer can't be tainted because:
	 * 1. The count never exceeds MAX_DMSG_WRITE_BUFFER i.e. buffer size.
	 * 2. copy_from_user returns 0 in case of correct copy.
	 *So, we don't need to sanitize buffer.
	 *
	 */
	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	buffer[count] = '\0';

	if (buffer[0] == '/')
		idx = 3;
	else if (strict_strtol(buffer, 0, &idx) != 0)
		return -EINVAL;

#if 0
	printk(KERN_CRIT"[%s] idx = %d\n", __func__, idx);  /* for debug */
#endif

	switch (idx) {
	case 1:
		/* print the dtvlogd status information */
		do_dtvlog(10, NULL, 0);
		break;
	case 2:
#ifdef CONFIG_DTVLOGD_USB_LOG
		/* flush the remaining buffer to USB */
		do_dtvlog(5, NULL, 0);
#endif
		break;
	case 3:
#ifdef CONFIG_DTVLOGD_USB_LOG
		dtvlogd_usblog_start(buffer);
#endif
		break;
	case 4:
#ifdef CONFIG_DTVLOGD_USB_LOG
		dtvlogd_usblog_stop();
#endif
		break;
	case 5:
#ifdef CONFIG_DTVLOGD_DEBUG
		/* This is used to flush RAM during run time */
		do_dtvlog(12, NULL, 0);
#endif
		break;
	default:
		printk(KERN_CRIT"\nUsage(DTVLOGD):\n");
		printk(KERN_CRIT"    echo 1 > /proc/dtvlogd             # show info\n");
#ifdef CONFIG_DTVLOGD_USB_LOG
		printk(KERN_CRIT"    echo 2 > /proc/dtvlogd             # flush log to usb\n");
		printk(KERN_CRIT"    echo /dtv/usb/sda1 > /proc/dtvlogd # start usb log\n");
		printk(KERN_CRIT"    echo 4 > /proc/dtvlogd             # stop usb log\n");
#endif
#ifdef CONFIG_DTVLOGD_DEBUG
		printk(KERN_CRIT"    echo 5 > /proc/dtvlogd             # debug commmand\n");
#endif
	}


	return count;
}

/* Define the operation allowed on dtvlogd
 * We just use:
 *   dmsg_write: To give commands (e.g. echo 1 > /proc/dtvlogd)
 *   dmsg_read: To print if USB logging is enabled or not.
 *
 *   dmsg_poll, dmsg_open and dmsg_release are not used
 */
const struct file_operations proc_dmsg_operations = {
	.read       = dmsg_read,
	.write      = dmsg_write,
#ifdef CONFIG_DTVLOGD_PROC_READ
	.open       = dmsg_open,
	.release    = dmsg_release,
	.llseek     = dmsg_llseek,
#endif

};


#ifdef CONFIG_DTVLOGD_EMERGENCY_LOG
static ssize_t rambufaddr_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	
	int len;

	if ((file->f_flags & O_NONBLOCK))
		return -EAGAIN;

	if (*ppos > 0)
		return 0;

	len = snprintf(buf, 11, "0x%x\n", RAM_BUF_ADDR);

	*ppos = len;
	
	return len;	
}

const struct file_operations proc_emerglog_operations = {
	.read       = rambufaddr_read,
};
#endif

/*
 * Create Proc Entry
 */
static int __init init_procfs_msg(void)
{
	proc_create("dtvlogd", S_IRUSR | S_IWUSR, NULL, &proc_dmsg_operations);

#ifdef CONFIG_DTVLOGD_EMERGENCY_LOG
	proc_create("emergloginfo", S_IRUSR, NULL, &proc_emerglog_operations);
#endif

	return 0;
}

/*
 *	dtvlogd_init()
 */

static int __init dtvlogd_init(void)
{
	struct task_struct *task;
	init_procfs_msg();

	init_completion(&dlogbuf_completion);

	init_waitqueue_head(&dlogbuf_wq);

	task = kthread_run(dtvlogd, NULL, "kdtvlogd");
	if (IS_ERR(task)) {
		printk(KERN_ERR "DTVLogd: unable to create kernel thread: %ld\n",
				PTR_ERR(task));
		return PTR_ERR(task);
	}

#ifdef DTVLOGD_ENABLE_LOGGING_START_STOP
	dtvlogd_write_start();
#endif
	return 0;
}

module_init(dtvlogd_init);


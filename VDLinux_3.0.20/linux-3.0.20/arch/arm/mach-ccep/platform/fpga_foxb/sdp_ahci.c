/*
 *  ahci.c - AHCI SATA support
 *
 *  Maintained by:  Jeff Garzik <jgarzik@pobox.com>
 *    		    Please ALWAYS copy linux-ide@vger.kernel.org
 *		    on emails.
 *
 *  Copyright 2004-2005 Red Hat, Inc.
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 * libata documentation is available via 'make {ps|pdf}docs',
 * as Documentation/DocBook/libata.*
 *
 * AHCI hardware documentation:
 * http://www.intel.com/technology/serialata/pdf/rev1_0.pdf
 * http://www.intel.com/technology/serialata/pdf/rev1_1.pdf
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/device.h>
#include <linux/dmi.h>
#include <scsi/scsi_host.h>
#include <scsi/scsi_cmnd.h>
#include <linux/libata.h>
#include <linux/slab.h>

#include <linux/platform_device.h>

#define ES_REAL_TARGET
#if defined (ES_REAL_TARGET)
#define MAX_AHCI_DEV_NUM	(2)
#else
#define MAX_AHCI_DEV_NUM	(1)
#endif

#define DRV_NAME	"ahci"
#define DRV_VERSION	"3.0"

/* Enclosure Management Control */
#define EM_CTRL_MSG_TYPE              0x000f0000

/* Enclosure Management LED Message Type */
#define EM_MSG_LED_HBA_PORT           0x0000000f
#define EM_MSG_LED_PMP_SLOT           0x0000ff00
#define EM_MSG_LED_VALUE              0xffff0000
#define EM_MSG_LED_VALUE_ACTIVITY     0x00070000
#define EM_MSG_LED_VALUE_OFF          0xfff80000
#define EM_MSG_LED_VALUE_ON           0x00010000

static int ahci_skip_host_reset;
static int ahci_ignore_sss;

module_param_named(skip_host_reset, ahci_skip_host_reset, int, 0444);
MODULE_PARM_DESC(skip_host_reset, "skip global host reset (0=don't skip, 1=skip)");

module_param_named(ignore_sss, ahci_ignore_sss, int, 0444);
MODULE_PARM_DESC(ignore_sss, "Ignore staggered spinup flag (0=don't ignore, 1=ignore)");

static int ahci_enable_alpm(struct ata_port *ap,
		enum link_pm policy);
static void ahci_disable_alpm(struct ata_port *ap);
static ssize_t ahci_led_show(struct ata_port *ap, char *buf);
static ssize_t ahci_led_store(struct ata_port *ap, const char *buf,
			      size_t size);
static ssize_t ahci_transmit_led_message(struct ata_port *ap, u32 state,
					ssize_t size);
#define MAX_SLOTS 8
#define MAX_RETRY 15

enum {
//	AHCI_PCI_BAR		= 5,
	AHCI_MAX_PORTS		= 32,
	AHCI_MAX_SG		= 168, /* hardware max is 64K */
	AHCI_DMA_BOUNDARY	= 0xffffffff,
	AHCI_MAX_CMDS		= 32,
	AHCI_CMD_SZ		= 32,
	AHCI_CMD_SLOT_SZ	= AHCI_MAX_CMDS * AHCI_CMD_SZ,
	AHCI_RX_FIS_SZ		= 256,
	AHCI_CMD_TBL_CDB	= 0x40,
	AHCI_CMD_TBL_HDR_SZ	= 0x80,
	AHCI_CMD_TBL_SZ		= AHCI_CMD_TBL_HDR_SZ + (AHCI_MAX_SG * 16),
	AHCI_CMD_TBL_AR_SZ	= AHCI_CMD_TBL_SZ * AHCI_MAX_CMDS,
	AHCI_PORT_PRIV_DMA_SZ	= AHCI_CMD_SLOT_SZ + AHCI_CMD_TBL_AR_SZ +
				  AHCI_RX_FIS_SZ,
	AHCI_IRQ_ON_SG		= (1 << 31),
	AHCI_CMD_ATAPI		= (1 << 5),
	AHCI_CMD_WRITE		= (1 << 6),
	AHCI_CMD_PREFETCH	= (1 << 7),
	AHCI_CMD_RESET		= (1 << 8),
	AHCI_CMD_CLR_BUSY	= (1 << 10),

	RX_FIS_D2H_REG		= 0x40,	/* offset of D2H Register FIS data */
	RX_FIS_SDB		= 0x58, /* offset of SDB FIS data */
	RX_FIS_UNK		= 0x60, /* offset of Unknown FIS data */

	board_ahci		= 0,
	board_ahci_vt8251	= 1,
	board_ahci_ign_iferr	= 2,
	board_ahci_sb600	= 3,
	board_ahci_mv		= 4,
	board_ahci_sb700	= 5, /* for SB700 and SB800 */
	board_ahci_mcp65	= 6,
	board_ahci_nopmp	= 7,
	board_ahci_yesncq	= 8,
	board_ahci_Firenze	= 9,

	/* global controller registers */
	HOST_CAP		= 0x00, /* host capabilities */
	HOST_CTL		= 0x04, /* global host control */
	HOST_IRQ_STAT		= 0x08, /* interrupt status */
	HOST_PORTS_IMPL		= 0x0c, /* bitmap of implemented ports */
	HOST_VERSION		= 0x10, /* AHCI spec. version compliancy */
	HOST_EM_LOC		= 0x1c, /* Enclosure Management location */
	HOST_EM_CTL		= 0x20, /* Enclosure Management Control */

	/* HOST_CTL bits */
	HOST_RESET		= (1 << 0),  /* reset controller; self-clear */
	HOST_IRQ_EN		= (1 << 1),  /* global IRQ enable */
	HOST_AHCI_EN		= (1 << 31), /* AHCI enabled */

	/* HOST_CAP bits */
	HOST_CAP_EMS		= (1 << 6),  /* Enclosure Management support */
	HOST_CAP_SSC		= (1 << 14), /* Slumber capable */
	HOST_CAP_PMP		= (1 << 17), /* Port Multiplier support */
	HOST_CAP_CLO		= (1 << 24), /* Command List Override support */
	HOST_CAP_ALPM		= (1 << 26), /* Aggressive Link PM support */
	HOST_CAP_SSS		= (1 << 27), /* Staggered Spin-up */
	HOST_CAP_SNTF		= (1 << 29), /* SNotification register */
	HOST_CAP_NCQ		= (1 << 30), /* Native Command Queueing */
	HOST_CAP_64		= (1 << 31), /* PCI DAC (64-bit DMA) support */

	/* registers for each SATA port */
	PORT_LST_ADDR		= 0x00, /* command list DMA addr */
	PORT_LST_ADDR_HI	= 0x04, /* command list DMA addr hi */
	PORT_FIS_ADDR		= 0x08, /* FIS rx buf addr */
	PORT_FIS_ADDR_HI	= 0x0c, /* FIS rx buf addr hi */
	PORT_IRQ_STAT		= 0x10, /* interrupt status */
	PORT_IRQ_MASK		= 0x14, /* interrupt enable/disable mask */
	PORT_CMD		= 0x18, /* port command */
	PORT_TFDATA		= 0x20,	/* taskfile data */
	PORT_SIG		= 0x24,	/* device TF signature */
	PORT_CMD_ISSUE		= 0x38, /* command issue */
	PORT_SCR_STAT		= 0x28, /* SATA phy register: SStatus */
	PORT_SCR_CTL		= 0x2c, /* SATA phy register: SControl */
	PORT_SCR_ERR		= 0x30, /* SATA phy register: SError */
	PORT_SCR_ACT		= 0x34, /* SATA phy register: SActive */
	PORT_SCR_NTF		= 0x3c, /* SATA phy register: SNotification */

	/* PORT_IRQ_{STAT,MASK} bits */
	PORT_IRQ_COLD_PRES	= (1 << 31), /* cold presence detect */
	PORT_IRQ_TF_ERR		= (1 << 30), /* task file error */
	PORT_IRQ_HBUS_ERR	= (1 << 29), /* host bus fatal error */
	PORT_IRQ_HBUS_DATA_ERR	= (1 << 28), /* host bus data error */
	PORT_IRQ_IF_ERR		= (1 << 27), /* interface fatal error */
	PORT_IRQ_IF_NONFATAL	= (1 << 26), /* interface non-fatal error */
	PORT_IRQ_OVERFLOW	= (1 << 24), /* xfer exhausted available S/G */
	PORT_IRQ_BAD_PMP	= (1 << 23), /* incorrect port multiplier */

	PORT_IRQ_PHYRDY		= (1 << 22), /* PhyRdy changed */
	PORT_IRQ_DEV_ILCK	= (1 << 7), /* device interlock */
	PORT_IRQ_CONNECT	= (1 << 6), /* port connect change status */
	PORT_IRQ_SG_DONE	= (1 << 5), /* descriptor processed */
	PORT_IRQ_UNK_FIS	= (1 << 4), /* unknown FIS rx'd */
	PORT_IRQ_SDB_FIS	= (1 << 3), /* Set Device Bits FIS rx'd */
	PORT_IRQ_DMAS_FIS	= (1 << 2), /* DMA Setup FIS rx'd */
	PORT_IRQ_PIOS_FIS	= (1 << 1), /* PIO Setup FIS rx'd */
	PORT_IRQ_D2H_REG_FIS	= (1 << 0), /* D2H Register FIS rx'd */

	PORT_IRQ_FREEZE		= PORT_IRQ_HBUS_ERR |
				  PORT_IRQ_IF_ERR |
				  PORT_IRQ_CONNECT |
				  PORT_IRQ_PHYRDY |
				  PORT_IRQ_UNK_FIS |
				  PORT_IRQ_BAD_PMP,
	PORT_IRQ_ERROR		= PORT_IRQ_FREEZE |
				  PORT_IRQ_TF_ERR |
				  PORT_IRQ_HBUS_DATA_ERR,
	DEF_PORT_IRQ		= PORT_IRQ_ERROR | PORT_IRQ_SG_DONE |
				  PORT_IRQ_SDB_FIS | PORT_IRQ_DMAS_FIS |
				  PORT_IRQ_PIOS_FIS | PORT_IRQ_D2H_REG_FIS,

	/* PORT_CMD bits */
	PORT_CMD_ASP		= (1 << 27), /* Aggressive Slumber/Partial */
	PORT_CMD_ALPE		= (1 << 26), /* Aggressive Link PM enable */
	PORT_CMD_ATAPI		= (1 << 24), /* Device is ATAPI */
	PORT_CMD_PMP		= (1 << 17), /* PMP attached */
	PORT_CMD_LIST_ON	= (1 << 15), /* cmd list DMA engine running */
	PORT_CMD_FIS_ON		= (1 << 14), /* FIS DMA engine running */
	PORT_CMD_FIS_RX		= (1 << 4), /* Enable FIS receive DMA engine */
	PORT_CMD_CLO		= (1 << 3), /* Command list override */
	PORT_CMD_POWER_ON	= (1 << 2), /* Power up device */
	PORT_CMD_SPIN_UP	= (1 << 1), /* Spin up device */
	PORT_CMD_START		= (1 << 0), /* Enable port DMA engine */

	PORT_CMD_ICC_MASK	= (0xf << 28), /* i/f ICC state mask */
	PORT_CMD_ICC_ACTIVE	= (0x1 << 28), /* Put i/f in active state */
	PORT_CMD_ICC_PARTIAL	= (0x2 << 28), /* Put i/f in partial state */
	PORT_CMD_ICC_SLUMBER	= (0x6 << 28), /* Put i/f in slumber state */

	/* hpriv->flags bits */
	AHCI_HFLAG_NO_NCQ		= (1 << 0),
	AHCI_HFLAG_IGN_IRQ_IF_ERR	= (1 << 1), /* ignore IRQ_IF_ERR */
	AHCI_HFLAG_IGN_SERR_INTERNAL	= (1 << 2), /* ignore SERR_INTERNAL */
	AHCI_HFLAG_32BIT_ONLY		= (1 << 3), /* force 32bit */
	AHCI_HFLAG_MV_PATA		= (1 << 4), /* PATA port */
	AHCI_HFLAG_NO_MSI		= (1 << 5), /* no PCI MSI */
	AHCI_HFLAG_NO_PMP		= (1 << 6), /* no PMP */
	AHCI_HFLAG_NO_HOTPLUG		= (1 << 7), /* ignore PxSERR.DIAG.N */
	AHCI_HFLAG_SECT255		= (1 << 8), /* max 255 sectors */
	AHCI_HFLAG_YES_NCQ		= (1 << 9), /* force NCQ cap on */
	AHCI_HFLAG_NO_SUSPEND		= (1 << 10), /* don't suspend */

	/* ap->flags bits */

	AHCI_FLAG_COMMON		= ATA_FLAG_SATA | ATA_FLAG_NO_LEGACY |
					  ATA_FLAG_MMIO | ATA_FLAG_PIO_DMA |
					  ATA_FLAG_ACPI_SATA | ATA_FLAG_AN |
					  ATA_FLAG_IPM,

	ICH_MAP				= 0x90, /* ICH MAP register */

	/* em_ctl bits */
	EM_CTL_RST			= (1 << 9), /* Reset */
	EM_CTL_TM			= (1 << 8), /* Transmit Message */
	EM_CTL_ALHD			= (1 << 26), /* Activity LED */
};

struct ahci_cmd_hdr {
	__le32			opts;
	__le32			status;
	__le32			tbl_addr;
	__le32			tbl_addr_hi;
	__le32			reserved[4];
};

struct ahci_sg {
	__le32			addr;
	__le32			addr_hi;
	__le32			reserved;
	__le32			flags_size;
};

struct ahci_em_priv {
	enum sw_activity blink_policy;
	struct timer_list timer;
	unsigned long saved_activity;
	unsigned long activity;
	unsigned long led_state;
};

struct ahci_host_priv {
	unsigned int		flags;		/* AHCI_HFLAG_* */
	u32			cap;		/* cap to use */
	u32			port_map;	/* port map to use */
	u32			saved_cap;	/* saved initial cap */
	u32			saved_port_map;	/* saved initial port_map */
	u32 			em_loc; /* enclosure management location */
	u32			irq;
	void __iomem*	iomap;
};

struct ahci_port_priv {
	struct ata_link		*active_link;
	struct ahci_cmd_hdr	*cmd_slot;
	dma_addr_t		cmd_slot_dma;
	void			*cmd_tbl;
	dma_addr_t		cmd_tbl_dma;
	void			*rx_fis;
	dma_addr_t		rx_fis_dma;
	/* for NCQ spurious interrupt analysis */
	unsigned int		ncq_saw_d2h:1;
	unsigned int		ncq_saw_dmas:1;
	unsigned int		ncq_saw_sdb:1;
	u32 			intr_mask;	/* interrupts to enable */
	struct ahci_em_priv	em_priv[MAX_SLOTS];/* enclosure management info
					 	 * per PM slot */
};

static int ahci_scr_read(struct ata_link *link, unsigned int sc_reg, u32 *val);
static int ahci_scr_write(struct ata_link *link, unsigned int sc_reg, u32 val);
static unsigned int ahci_qc_issue(struct ata_queued_cmd *qc);
static bool ahci_qc_fill_rtf(struct ata_queued_cmd *qc);
static int ahci_port_start(struct ata_port *ap);
static void ahci_port_stop(struct ata_port *ap);
static void ahci_qc_prep(struct ata_queued_cmd *qc);
static void ahci_freeze(struct ata_port *ap);
static void ahci_thaw(struct ata_port *ap);
static void ahci_pmp_attach(struct ata_port *ap);
static void ahci_pmp_detach(struct ata_port *ap);
static int ahci_softreset(struct ata_link *link, unsigned int *class,
			  unsigned long deadline);
static int ahci_sb600_softreset(struct ata_link *link, unsigned int *class,
			  unsigned long deadline);
static int ahci_hardreset(struct ata_link *link, unsigned int *class,
			  unsigned long deadline);
static int ahci_vt8251_hardreset(struct ata_link *link, unsigned int *class,
				 unsigned long deadline);
static void ahci_postreset(struct ata_link *link, unsigned int *class);
static void ahci_error_handler(struct ata_port *ap);
static void ahci_post_internal_cmd(struct ata_queued_cmd *qc);
static int ahci_port_resume(struct ata_port *ap);
static void ahci_dev_config(struct ata_device *dev);
static unsigned int ahci_fill_sg(struct ata_queued_cmd *qc, void *cmd_tbl);
static void ahci_fill_cmd_slot(struct ahci_port_priv *pp, unsigned int tag,
			       u32 opts);
#ifdef CONFIG_PM
static int ahci_port_suspend(struct ata_port *ap, pm_message_t mesg);
static int ahci_device_suspend(struct platform_device* pdev, pm_message_t mesg);
static int ahci_device_resume(struct platform_device* pdev);
#endif
static ssize_t ahci_activity_show(struct ata_device *dev, char *buf);
static ssize_t ahci_activity_store(struct ata_device *dev,
				   enum sw_activity val);
static void ahci_init_sw_activity(struct ata_link *link);
static void ahci_sata_phy0_init(void);
static void ahci_sata_phy1_init(void);

static struct device_attribute *ahci_shost_attrs[] = {
	&dev_attr_link_power_management_policy,
	&dev_attr_em_message_type,
	&dev_attr_em_message,
	NULL
};

static struct device_attribute *ahci_sdev_attrs[] = {
	&dev_attr_sw_activity,
	&dev_attr_unload_heads,
	NULL
};

static struct scsi_host_template ahci_sht = {
	ATA_NCQ_SHT(DRV_NAME),
	.can_queue		= AHCI_MAX_CMDS - 1,
	.sg_tablesize		= AHCI_MAX_SG,
	.dma_boundary		= AHCI_DMA_BOUNDARY,
	.shost_attrs		= ahci_shost_attrs,
	.sdev_attrs		= ahci_sdev_attrs,
};

static struct ata_port_operations ahci_ops = {
	.inherits		= &sata_pmp_port_ops,

	.qc_defer		= sata_pmp_qc_defer_cmd_switch,
	.qc_prep		= ahci_qc_prep,
	.qc_issue		= ahci_qc_issue,
	.qc_fill_rtf		= ahci_qc_fill_rtf,

	.freeze			= ahci_freeze,
	.thaw			= ahci_thaw,
	.softreset		= ahci_softreset,
	.hardreset		= ahci_hardreset,
	.postreset		= ahci_postreset,
	.pmp_softreset		= ahci_softreset,
	.error_handler		= ahci_error_handler,
	.post_internal_cmd	= ahci_post_internal_cmd,
	.dev_config		= ahci_dev_config,

	.scr_read		= ahci_scr_read,
	.scr_write		= ahci_scr_write,
	.pmp_attach		= ahci_pmp_attach,
	.pmp_detach		= ahci_pmp_detach,

	.enable_pm		= ahci_enable_alpm,
	.disable_pm		= ahci_disable_alpm,
	.em_show		= ahci_led_show,
	.em_store		= ahci_led_store,
	.sw_activity_show	= ahci_activity_show,
	.sw_activity_store	= ahci_activity_store,
#ifdef CONFIG_PM
	.port_suspend		= ahci_port_suspend,
	.port_resume		= ahci_port_resume,
#endif
	.port_start		= ahci_port_start,
	.port_stop		= ahci_port_stop,
};

static struct ata_port_operations ahci_vt8251_ops = {
	.inherits		= &ahci_ops,
	.hardreset		= ahci_vt8251_hardreset,
};

static struct ata_port_operations ahci_sb600_ops = {
	.inherits		= &ahci_ops,
	.softreset		= ahci_sb600_softreset,
	.pmp_softreset		= ahci_sb600_softreset,
};

#define AHCI_HFLAGS(flags)	.private_data	= (void *)(flags)

static const struct ata_port_info ahci_port_info[] = {
	/* board_ahci */
	{
		.flags		= AHCI_FLAG_COMMON,
		.pio_mask	= ATA_PIO4,
		.udma_mask	= ATA_UDMA6,
		.port_ops	= &ahci_ops,
	},
	/* board_ahci_vt8251 */
	{
		AHCI_HFLAGS	(AHCI_HFLAG_NO_NCQ | AHCI_HFLAG_NO_PMP),
		.flags		= AHCI_FLAG_COMMON,
		.pio_mask	= ATA_PIO4,
		.udma_mask	= ATA_UDMA6,
		.port_ops	= &ahci_vt8251_ops,
	},
	/* board_ahci_ign_iferr */
	{
		AHCI_HFLAGS	(AHCI_HFLAG_IGN_IRQ_IF_ERR),
		.flags		= AHCI_FLAG_COMMON,
		.pio_mask	= ATA_PIO4,
		.udma_mask	= ATA_UDMA6,
		.port_ops	= &ahci_ops,
	},
	/* board_ahci_sb600 */
	{
		AHCI_HFLAGS	(AHCI_HFLAG_IGN_SERR_INTERNAL |
				 AHCI_HFLAG_32BIT_ONLY | AHCI_HFLAG_NO_MSI |
				 AHCI_HFLAG_SECT255),
		.flags		= AHCI_FLAG_COMMON,
		.pio_mask	= ATA_PIO4,
		.udma_mask	= ATA_UDMA6,
		.port_ops	= &ahci_sb600_ops,
	},
	/* board_ahci_mv */
	{
		AHCI_HFLAGS	(AHCI_HFLAG_NO_NCQ | AHCI_HFLAG_NO_MSI |
				 AHCI_HFLAG_MV_PATA | AHCI_HFLAG_NO_PMP),
		.flags		= ATA_FLAG_SATA | ATA_FLAG_NO_LEGACY |
				  ATA_FLAG_MMIO | ATA_FLAG_PIO_DMA,
		.pio_mask	= ATA_PIO4,
		.udma_mask	= ATA_UDMA6,
		.port_ops	= &ahci_ops,
	},
	/* board_ahci_sb700, for SB700 and SB800 */
	{
		AHCI_HFLAGS	(AHCI_HFLAG_IGN_SERR_INTERNAL),
		.flags		= AHCI_FLAG_COMMON,
		.pio_mask	= ATA_PIO4,
		.udma_mask	= ATA_UDMA6,
		.port_ops	= &ahci_sb600_ops,
	},
	/* board_ahci_mcp65 */
	{
		AHCI_HFLAGS	(AHCI_HFLAG_YES_NCQ),
		.flags		= AHCI_FLAG_COMMON,
		.pio_mask	= ATA_PIO4,
		.udma_mask	= ATA_UDMA6,
		.port_ops	= &ahci_ops,
	},
	/* board_ahci_nopmp */
	{
		AHCI_HFLAGS	(AHCI_HFLAG_NO_PMP),
		.flags		= AHCI_FLAG_COMMON,
		.pio_mask	= ATA_PIO4,
		.udma_mask	= ATA_UDMA6,
		.port_ops	= &ahci_ops,
	},
	/* board_ahci_yesncq */
	{
		AHCI_HFLAGS	(AHCI_HFLAG_YES_NCQ),
		.flags		= AHCI_FLAG_COMMON,
		.pio_mask	= ATA_PIO4,
		.udma_mask	= ATA_UDMA6,
		.port_ops	= &ahci_ops,
	},
	
	/* board_ahci */
	{
		.flags		= AHCI_FLAG_COMMON,
		.pio_mask	= ATA_PIO4,
		.udma_mask	= ATA_UDMA6,
		.port_ops	= &ahci_ops,
	},

};

static int ahci_em_messages = 1;
module_param(ahci_em_messages, int, 0444);
/* add other LED protocol types when they become supported */
MODULE_PARM_DESC(ahci_em_messages,
	"Set AHCI Enclosure Management Message type (0 = disabled, 1 = LED");

#if defined(CONFIG_PATA_MARVELL) || defined(CONFIG_PATA_MARVELL_MODULE)
static int marvell_enable;
#else
static int marvell_enable = 1;
#endif
module_param(marvell_enable, int, 0644);
MODULE_PARM_DESC(marvell_enable, "Marvell SATA via AHCI (1 = enabled)");


static inline int ahci_nr_ports(u32 cap)
{
	return (cap & 0x1f) + 1;
}

static inline void __iomem *__ahci_port_base(struct ata_host *host,
					     unsigned int port_no)
{
	void __iomem *mmio = (void __iomem *)host->iomap;

	return mmio + 0x100 + (port_no * 0x80);
}

static inline void __iomem *ahci_port_base(struct ata_port *ap)
{
	return __ahci_port_base(ap->host, ap->port_no);
}

static void ahci_enable_ahci(void __iomem *mmio)
{
	int i;
	u32 tmp;

	/* turn on AHCI_EN */
	tmp = readl(mmio + HOST_CTL);
	if (tmp & HOST_AHCI_EN)
		return;

	/* Some controllers need AHCI_EN to be written multiple times.
	 * Try a few times before giving up.
	 */
	for (i = 0; i < 5; i++) {
		tmp |= HOST_AHCI_EN;
		writel(tmp, mmio + HOST_CTL);
		tmp = readl(mmio + HOST_CTL);	/* flush && sanity check */
		if (tmp & HOST_AHCI_EN)
			return;
		msleep(10);
	}

	WARN_ON(1);
}

/**
 *	ahci_save_initial_config - Save and fixup initial config values
 *	@pdev: target PCI device
 *	@hpriv: host private area to store config values
 *
 *	Some registers containing configuration info might be setup by
 *	BIOS and might be cleared on reset.  This function saves the
 *	initial values of those registers into @hpriv such that they
 *	can be restored after controller reset.
 *
 *	If inconsistent, config values are fixed up by this function.
 *
 *	LOCKING:
 *	None.
 */
static void ahci_save_initial_config(struct platform_device* pdev,   struct ahci_host_priv* hpriv)
{
	void __iomem *mmio = hpriv->iomap;
	u32 cap, port_map;
	int i;

	/* make sure AHCI mode is enabled before accessing CAP */
	ahci_enable_ahci(mmio);

	/* Values prefixed with saved_ are written back to host after
	 * reset.  Values without are used for driver operation.
	 */
	hpriv->saved_cap = cap = readl(mmio + HOST_CAP);
	hpriv->saved_port_map = port_map = readl(mmio + HOST_PORTS_IMPL);

	/* some chips have errata preventing 64bit use */
	if ((cap & HOST_CAP_64) && (hpriv->flags & AHCI_HFLAG_32BIT_ONLY)) {
		dev_printk(KERN_INFO, &pdev->dev,
			   "controller can't do 64bit DMA, forcing 32bit\n");
		cap &= ~HOST_CAP_64;
	}

	if ((cap & HOST_CAP_NCQ) && (hpriv->flags & AHCI_HFLAG_NO_NCQ)) {
		dev_printk(KERN_INFO, &pdev->dev,
			   "controller can't do NCQ, turning off CAP_NCQ\n");
		cap &= ~HOST_CAP_NCQ;
	}

	if (!(cap & HOST_CAP_NCQ) && (hpriv->flags & AHCI_HFLAG_YES_NCQ)) {
		dev_printk(KERN_INFO, &pdev->dev,
			   "controller can do NCQ, turning on CAP_NCQ\n");
		cap |= HOST_CAP_NCQ;
	}

	if ((cap & HOST_CAP_PMP) && (hpriv->flags & AHCI_HFLAG_NO_PMP)) {
		dev_printk(KERN_INFO, &pdev->dev,
			   "controller can't do PMP, turning off CAP_PMP\n");
		cap &= ~HOST_CAP_PMP;
	}

	/* cross check port_map and cap.n_ports */
	if (port_map) {
		int map_ports = 0;

		for (i = 0; i < AHCI_MAX_PORTS; i++)
			if (port_map & (1 << i))
				map_ports++;

		/* If PI has more ports than n_ports, whine, clear
		 * port_map and let it be generated from n_ports.
		 */
		if (map_ports > ahci_nr_ports(cap)) {
			dev_printk(KERN_WARNING, &pdev->dev,
				   "implemented port map (0x%x) contains more "
				   "ports than nr_ports (%u), using nr_ports\n",
				   port_map, ahci_nr_ports(cap));
			port_map = 0;
		}
	}

	/* fabricate port_map from cap.nr_ports */
	if (!port_map) {
		port_map = (1 << ahci_nr_ports(cap)) - 1;
		dev_printk(KERN_WARNING, &pdev->dev,
			   "forcing PORTS_IMPL to 0x%x\n", port_map);

		/* write the fixed up value to the PI register */
		hpriv->saved_port_map = port_map;
	}

	/* record values to use during operation */
	hpriv->cap = cap;
	hpriv->port_map = port_map;
}

/**
 *	ahci_restore_initial_config - Restore initial config
 *	@host: target ATA host
 *
 *	Restore initial config stored by ahci_save_initial_config().
 *
 *	LOCKING:
 *	None.
 */
static void ahci_restore_initial_config(struct ata_host *host)
{
	struct ahci_host_priv *hpriv = host->private_data;
	void __iomem *mmio = (void __iomem *)host->iomap;

	writel(hpriv->saved_cap, mmio + HOST_CAP);
	writel(hpriv->saved_port_map, mmio + HOST_PORTS_IMPL);
	(void) readl(mmio + HOST_PORTS_IMPL);	/* flush */
}

static unsigned ahci_scr_offset(struct ata_port *ap, unsigned int sc_reg)
{
	static const int offset[] = {
		[SCR_STATUS]		= PORT_SCR_STAT,
		[SCR_CONTROL]		= PORT_SCR_CTL,
		[SCR_ERROR]		= PORT_SCR_ERR,
		[SCR_ACTIVE]		= PORT_SCR_ACT,
		[SCR_NOTIFICATION]	= PORT_SCR_NTF,
	};
	struct ahci_host_priv *hpriv = ap->host->private_data;

	if (sc_reg < ARRAY_SIZE(offset) &&
	    (sc_reg != SCR_NOTIFICATION || (hpriv->cap & HOST_CAP_SNTF)))
		return offset[sc_reg];
	return 0;
}

static int ahci_scr_read(struct ata_link *link, unsigned int sc_reg, u32 *val)
{
	void __iomem *port_mmio = ahci_port_base(link->ap);
	int offset = ahci_scr_offset(link->ap, sc_reg);

	if (offset) {
		*val = readl(port_mmio + offset);
		return 0;
	}
	return -EINVAL;
}

static int ahci_scr_write(struct ata_link *link, unsigned int sc_reg, u32 val)
{
	void __iomem *port_mmio = ahci_port_base(link->ap);
	int offset = ahci_scr_offset(link->ap, sc_reg);

	if (offset) {
		writel(val, port_mmio + offset);
		return 0;
	}
	return -EINVAL;
}

static void ahci_start_engine(struct ata_port *ap)
{
	void __iomem *port_mmio = ahci_port_base(ap);
	u32 tmp;

	/* start DMA */
	tmp = readl(port_mmio + PORT_CMD);
	tmp |= PORT_CMD_START;
	writel(tmp, port_mmio + PORT_CMD);
	readl(port_mmio + PORT_CMD); /* flush */
}

static int ahci_stop_engine(struct ata_port *ap)
{
	void __iomem *port_mmio = ahci_port_base(ap);
	u32 tmp;

	tmp = readl(port_mmio + PORT_CMD);

	/* check if the HBA is idle */
	if ((tmp & (PORT_CMD_START | PORT_CMD_LIST_ON)) == 0)
		return 0;

	/* setting HBA to idle */
	tmp &= ~PORT_CMD_START;
	writel(tmp, port_mmio + PORT_CMD);

	/* wait for engine to stop. This could be as long as 500 msec */
	tmp = ata_wait_register(port_mmio + PORT_CMD,
				PORT_CMD_LIST_ON, PORT_CMD_LIST_ON, 1, 500);
	if (tmp & PORT_CMD_LIST_ON)
		return -EIO;

	return 0;
}

static void ahci_start_fis_rx(struct ata_port *ap)
{
	void __iomem *port_mmio = ahci_port_base(ap);
	struct ahci_host_priv *hpriv = ap->host->private_data;
	struct ahci_port_priv *pp = ap->private_data;
	u32 tmp;

	/* set FIS registers */
	if (hpriv->cap & HOST_CAP_64)
		writel((pp->cmd_slot_dma >> 16) >> 16,
		       port_mmio + PORT_LST_ADDR_HI);
	writel(pp->cmd_slot_dma & 0xffffffff, port_mmio + PORT_LST_ADDR);

	if (hpriv->cap & HOST_CAP_64)
		writel((pp->rx_fis_dma >> 16) >> 16,
		       port_mmio + PORT_FIS_ADDR_HI);
	writel(pp->rx_fis_dma & 0xffffffff, port_mmio + PORT_FIS_ADDR);

	/* enable FIS reception */
	tmp = readl(port_mmio + PORT_CMD);
	tmp |= PORT_CMD_FIS_RX;
	writel(tmp, port_mmio + PORT_CMD);

	/* flush */
	readl(port_mmio + PORT_CMD);
}

static int ahci_stop_fis_rx(struct ata_port *ap)
{
	void __iomem *port_mmio = ahci_port_base(ap);
	u32 tmp;

	/* disable FIS reception */
	tmp = readl(port_mmio + PORT_CMD);
	tmp &= ~PORT_CMD_FIS_RX;
	writel(tmp, port_mmio + PORT_CMD);

	/* wait for completion, spec says 500ms, give it 1000 */
	tmp = ata_wait_register(port_mmio + PORT_CMD, PORT_CMD_FIS_ON,
				PORT_CMD_FIS_ON, 10, 1000);
	if (tmp & PORT_CMD_FIS_ON)
		return -EBUSY;

	return 0;
}

static void ahci_power_up(struct ata_port *ap)
{
	struct ahci_host_priv *hpriv = ap->host->private_data;
	void __iomem *port_mmio = ahci_port_base(ap);
	u32 cmd;

	cmd = readl(port_mmio + PORT_CMD) & ~PORT_CMD_ICC_MASK;

	/* spin up device */
	if (hpriv->cap & HOST_CAP_SSS) {
		cmd |= PORT_CMD_SPIN_UP;
		writel(cmd, port_mmio + PORT_CMD);
	}

	/* wake up link */
	writel(cmd | PORT_CMD_ICC_ACTIVE, port_mmio + PORT_CMD);
}

static void ahci_disable_alpm(struct ata_port *ap)
{
	struct ahci_host_priv *hpriv = ap->host->private_data;
	void __iomem *port_mmio = ahci_port_base(ap);
	u32 cmd;
	struct ahci_port_priv *pp = ap->private_data;

	/* IPM bits should be disabled by libata-core */
	/* get the existing command bits */
	cmd = readl(port_mmio + PORT_CMD);

	/* disable ALPM and ASP */
	cmd &= ~PORT_CMD_ASP;
	cmd &= ~PORT_CMD_ALPE;

	/* force the interface back to active */
	cmd |= PORT_CMD_ICC_ACTIVE;

	/* write out new cmd value */
	writel(cmd, port_mmio + PORT_CMD);
	cmd = readl(port_mmio + PORT_CMD);

	/* wait 10ms to be sure we've come out of any low power state */
	msleep(10);

	/* clear out any PhyRdy stuff from interrupt status */
	writel(PORT_IRQ_PHYRDY, port_mmio + PORT_IRQ_STAT);

	/* go ahead and clean out PhyRdy Change from Serror too */
	ahci_scr_write(&ap->link, SCR_ERROR, ((1 << 16) | (1 << 18)));

	/*
 	 * Clear flag to indicate that we should ignore all PhyRdy
 	 * state changes
 	 */
	hpriv->flags &= ~AHCI_HFLAG_NO_HOTPLUG;

	/*
 	 * Enable interrupts on Phy Ready.
 	 */
	pp->intr_mask |= PORT_IRQ_PHYRDY;
	writel(pp->intr_mask, port_mmio + PORT_IRQ_MASK);

	/*
 	 * don't change the link pm policy - we can be called
 	 * just to turn of link pm temporarily
 	 */
}

static int ahci_enable_alpm(struct ata_port *ap,
	enum link_pm policy)
{
	struct ahci_host_priv *hpriv = ap->host->private_data;
	void __iomem *port_mmio = ahci_port_base(ap);
	u32 cmd;
	struct ahci_port_priv *pp = ap->private_data;
	u32 asp;

	/* Make sure the host is capable of link power management */
	if (!(hpriv->cap & HOST_CAP_ALPM))
		return -EINVAL;

	switch (policy) {
	case MAX_PERFORMANCE:
	case NOT_AVAILABLE:
		/*
 		 * if we came here with NOT_AVAILABLE,
 		 * it just means this is the first time we
 		 * have tried to enable - default to max performance,
 		 * and let the user go to lower power modes on request.
 		 */
		ahci_disable_alpm(ap);
		return 0;
	case MIN_POWER:
		/* configure HBA to enter SLUMBER */
		asp = PORT_CMD_ASP;
		break;
	case MEDIUM_POWER:
		/* configure HBA to enter PARTIAL */
		asp = 0;
		break;
	default:
		return -EINVAL;
	}

	/*
 	 * Disable interrupts on Phy Ready. This keeps us from
 	 * getting woken up due to spurious phy ready interrupts
	 * TBD - Hot plug should be done via polling now, is
	 * that even supported?
 	 */
	pp->intr_mask &= ~PORT_IRQ_PHYRDY;
	writel(pp->intr_mask, port_mmio + PORT_IRQ_MASK);

	/*
 	 * Set a flag to indicate that we should ignore all PhyRdy
 	 * state changes since these can happen now whenever we
 	 * change link state
 	 */
	hpriv->flags |= AHCI_HFLAG_NO_HOTPLUG;

	/* get the existing command bits */
	cmd = readl(port_mmio + PORT_CMD);

	/*
 	 * Set ASP based on Policy
 	 */
	cmd |= asp;

	/*
 	 * Setting this bit will instruct the HBA to aggressively
 	 * enter a lower power link state when it's appropriate and
 	 * based on the value set above for ASP
 	 */
	cmd |= PORT_CMD_ALPE;

	/* write out new cmd value */
	writel(cmd, port_mmio + PORT_CMD);
	cmd = readl(port_mmio + PORT_CMD);

	/* IPM bits should be set by libata-core */
	return 0;
}

#ifdef CONFIG_PM
static void ahci_power_down(struct ata_port *ap)
{
	struct ahci_host_priv *hpriv = ap->host->private_data;
	void __iomem *port_mmio = ahci_port_base(ap);
	u32 cmd, scontrol;

	if (!(hpriv->cap & HOST_CAP_SSS))
		return;

	/* put device into listen mode, first set PxSCTL.DET to 0 */
	scontrol = readl(port_mmio + PORT_SCR_CTL);
	scontrol &= ~0xf;
	writel(scontrol, port_mmio + PORT_SCR_CTL);

	/* then set PxCMD.SUD to 0 */
	cmd = readl(port_mmio + PORT_CMD) & ~PORT_CMD_ICC_MASK;
	cmd &= ~PORT_CMD_SPIN_UP;
	writel(cmd, port_mmio + PORT_CMD);
}
#endif

static void ahci_start_port(struct ata_port *ap)
{
	struct ahci_port_priv *pp = ap->private_data;
	struct ata_link *link;
	struct ahci_em_priv *emp;
	ssize_t rc;
	int i;

	/* enable FIS reception */
	ahci_start_fis_rx(ap);

	/* enable DMA */
	ahci_start_engine(ap);

	/* turn on LEDs */
	if (ap->flags & ATA_FLAG_EM) {
		ata_for_each_link(link, ap, EDGE) {
			emp = &pp->em_priv[link->pmp];

			/* EM Transmit bit maybe busy during init */
			for (i = 0; i < MAX_RETRY; i++) {
				rc = ahci_transmit_led_message(ap,
							       emp->led_state,
							       4);
				if (rc == -EBUSY)
					udelay(100);
				else
					break;
			}
		}
	}

	if (ap->flags & ATA_FLAG_SW_ACTIVITY)
		ata_for_each_link(link, ap, EDGE)
			ahci_init_sw_activity(link);

}

static int ahci_deinit_port(struct ata_port *ap, const char **emsg)
{
	int rc;

	/* disable DMA */
	rc = ahci_stop_engine(ap);
	if (rc) {
		*emsg = "failed to stop engine";
		return rc;
	}

	/* disable FIS reception */
	rc = ahci_stop_fis_rx(ap);
	if (rc) {
		*emsg = "failed stop FIS RX";
		return rc;
	}

	return 0;
}

static int ahci_reset_controller(struct ata_host *host)
{
	void __iomem *mmio = (void __iomem *)host->iomap;
	u32 tmp;

	/* we must be in AHCI mode, before using anything
	 * AHCI-specific, such as HOST_RESET.
	 */
	ahci_enable_ahci(mmio);

	/* global controller reset */
	if (!ahci_skip_host_reset) {
		tmp = readl(mmio + HOST_CTL);
		if ((tmp & HOST_RESET) == 0) {
			writel(tmp | HOST_RESET, mmio + HOST_CTL);
			readl(mmio + HOST_CTL); /* flush */
		}

		/*
		 * to perform host reset, OS should set HOST_RESET
		 * and poll until this bit is read to be "0".
		 * reset must complete within 1 second, or
		 * the hardware should be considered fried.
		 */
		tmp = ata_wait_register(mmio + HOST_CTL, HOST_RESET,
					HOST_RESET, 10, 1000);

		if (tmp & HOST_RESET) {
			dev_printk(KERN_ERR, host->dev,
				   "controller reset failed (0x%x)\n", tmp);
			return -EIO;
		}

		/* turn on AHCI mode */
		ahci_enable_ahci(mmio);

		/* Some registers might be cleared on reset.  Restore
		 * initial values.
		 */
		ahci_restore_initial_config(host);
	} else
		dev_printk(KERN_INFO, host->dev,
			   "skipping global host reset\n");

	return 0;
}

static void ahci_sw_activity(struct ata_link *link)
{
	struct ata_port *ap = link->ap;
	struct ahci_port_priv *pp = ap->private_data;
	struct ahci_em_priv *emp = &pp->em_priv[link->pmp];

	if (!(link->flags & ATA_LFLAG_SW_ACTIVITY))
		return;

	emp->activity++;
	if (!timer_pending(&emp->timer))
		mod_timer(&emp->timer, jiffies + msecs_to_jiffies(10));
}

static void ahci_sw_activity_blink(unsigned long arg)
{
	struct ata_link *link = (struct ata_link *)arg;
	struct ata_port *ap = link->ap;
	struct ahci_port_priv *pp = ap->private_data;
	struct ahci_em_priv *emp = &pp->em_priv[link->pmp];
	unsigned long led_message = emp->led_state;
	u32 activity_led_state;
	unsigned long flags;

	led_message &= EM_MSG_LED_VALUE;
	led_message |= ap->port_no | (link->pmp << 8);

	/* check to see if we've had activity.  If so,
	 * toggle state of LED and reset timer.  If not,
	 * turn LED to desired idle state.
	 */
	spin_lock_irqsave(ap->lock, flags);
	if (emp->saved_activity != emp->activity) {
		emp->saved_activity = emp->activity;
		/* get the current LED state */
		activity_led_state = led_message & EM_MSG_LED_VALUE_ON;

		if (activity_led_state)
			activity_led_state = 0;
		else
			activity_led_state = 1;

		/* clear old state */
		led_message &= ~EM_MSG_LED_VALUE_ACTIVITY;

		/* toggle state */
		led_message |= (activity_led_state << 16);
		mod_timer(&emp->timer, jiffies + msecs_to_jiffies(100));
	} else {
		/* switch to idle */
		led_message &= ~EM_MSG_LED_VALUE_ACTIVITY;
		if (emp->blink_policy == BLINK_OFF)
			led_message |= (1 << 16);
	}
	spin_unlock_irqrestore(ap->lock, flags);
	ahci_transmit_led_message(ap, led_message, 4);
}

static void ahci_init_sw_activity(struct ata_link *link)
{
	struct ata_port *ap = link->ap;
	struct ahci_port_priv *pp = ap->private_data;
	struct ahci_em_priv *emp = &pp->em_priv[link->pmp];

	/* init activity stats, setup timer */
	emp->saved_activity = emp->activity = 0;
	setup_timer(&emp->timer, ahci_sw_activity_blink, (unsigned long)link);

	/* check our blink policy and set flag for link if it's enabled */
	if (emp->blink_policy)
		link->flags |= ATA_LFLAG_SW_ACTIVITY;
}

static ssize_t ahci_transmit_led_message(struct ata_port *ap, u32 state,
					ssize_t size)
{
	struct ahci_host_priv *hpriv = ap->host->private_data;
	struct ahci_port_priv *pp = ap->private_data;
	void __iomem *mmio = (void __iomem *)ap->host->iomap;
	u32 em_ctl;
	u32 message[] = {0, 0};
	unsigned long flags;
	int pmp;
	struct ahci_em_priv *emp;

	/* get the slot number from the message */
	pmp = (state & EM_MSG_LED_PMP_SLOT) >> 8;
	if (pmp < MAX_SLOTS)
		emp = &pp->em_priv[pmp];
	else
		return -EINVAL;

	spin_lock_irqsave(ap->lock, flags);

	/*
	 * if we are still busy transmitting a previous message,
	 * do not allow
	 */
	em_ctl = readl(mmio + HOST_EM_CTL);
	if (em_ctl & EM_CTL_TM) {
		spin_unlock_irqrestore(ap->lock, flags);
		return -EBUSY;
	}

	/*
	 * create message header - this is all zero except for
	 * the message size, which is 4 bytes.
	 */
	message[0] |= (4 << 8);

	/* ignore 0:4 of byte zero, fill in port info yourself */
	message[1] = ((state & ~EM_MSG_LED_HBA_PORT) | ap->port_no);

	/* write message to EM_LOC */
	writel(message[0], mmio + hpriv->em_loc);
	writel(message[1], mmio + hpriv->em_loc+4);

	/* save off new led state for port/slot */
	emp->led_state = state;

	/*
	 * tell hardware to transmit the message
	 */
	writel(em_ctl | EM_CTL_TM, mmio + HOST_EM_CTL);

	spin_unlock_irqrestore(ap->lock, flags);
	return size;
}

static ssize_t ahci_led_show(struct ata_port *ap, char *buf)
{
	struct ahci_port_priv *pp = ap->private_data;
	struct ata_link *link;
	struct ahci_em_priv *emp;
	int rc = 0;

	ata_for_each_link(link, ap, EDGE) {
		emp = &pp->em_priv[link->pmp];
		rc += sprintf(buf, "%lx\n", emp->led_state);
	}
	return rc;
}

static ssize_t ahci_led_store(struct ata_port *ap, const char *buf,
				size_t size)
{
	int state;
	int pmp;
	struct ahci_port_priv *pp = ap->private_data;
	struct ahci_em_priv *emp;

	state = simple_strtoul(buf, NULL, 0);

	/* get the slot number from the message */
	pmp = (state & EM_MSG_LED_PMP_SLOT) >> 8;
	if (pmp < MAX_SLOTS)
		emp = &pp->em_priv[pmp];
	else
		return -EINVAL;

	/* mask off the activity bits if we are in sw_activity
	 * mode, user should turn off sw_activity before setting
	 * activity led through em_message
	 */
	if (emp->blink_policy)
		state &= ~EM_MSG_LED_VALUE_ACTIVITY;

	return ahci_transmit_led_message(ap, state, size);
}

static ssize_t ahci_activity_store(struct ata_device *dev, enum sw_activity val)
{
	struct ata_link *link = dev->link;
	struct ata_port *ap = link->ap;
	struct ahci_port_priv *pp = ap->private_data;
	struct ahci_em_priv *emp = &pp->em_priv[link->pmp];
	u32 port_led_state = emp->led_state;

	/* save the desired Activity LED behavior */
	if (val == OFF) {
		/* clear LFLAG */
		link->flags &= ~(ATA_LFLAG_SW_ACTIVITY);

		/* set the LED to OFF */
		port_led_state &= EM_MSG_LED_VALUE_OFF;
		port_led_state |= (ap->port_no | (link->pmp << 8));
		ahci_transmit_led_message(ap, port_led_state, 4);
	} else {
		link->flags |= ATA_LFLAG_SW_ACTIVITY;
		if (val == BLINK_OFF) {
			/* set LED to ON for idle */
			port_led_state &= EM_MSG_LED_VALUE_OFF;
			port_led_state |= (ap->port_no | (link->pmp << 8));
			port_led_state |= EM_MSG_LED_VALUE_ON; /* check this */
			ahci_transmit_led_message(ap, port_led_state, 4);
		}
	}
	emp->blink_policy = val;
	return 0;
}

static ssize_t ahci_activity_show(struct ata_device *dev, char *buf)
{
	struct ata_link *link = dev->link;
	struct ata_port *ap = link->ap;
	struct ahci_port_priv *pp = ap->private_data;
	struct ahci_em_priv *emp = &pp->em_priv[link->pmp];

	/* display the saved value of activity behavior for this
	 * disk.
	 */
	return sprintf(buf, "%d\n", emp->blink_policy);
}

static void ahci_port_init(struct device *pdev, struct ata_port *ap,
			   int port_no, void __iomem *mmio,
			   void __iomem *port_mmio)
{
	const char *emsg = NULL;
	int rc;
	u32 tmp;

	/* make sure port is not active */
	rc = ahci_deinit_port(ap, &emsg);
	if (rc)
		dev_printk(KERN_WARNING, pdev,
			   "%s (%d)\n", emsg, rc);

	/* clear SError */
	tmp = readl(port_mmio + PORT_SCR_ERR);
	VPRINTK("PORT_SCR_ERR 0x%x\n", tmp);
	writel(tmp, port_mmio + PORT_SCR_ERR);

	/* clear port IRQ */
	tmp = readl(port_mmio + PORT_IRQ_STAT);
	VPRINTK("PORT_IRQ_STAT 0x%x\n", tmp);
	if (tmp)
		writel(tmp, port_mmio + PORT_IRQ_STAT);

	writel(1 << port_no, mmio + HOST_IRQ_STAT);
}

static void ahci_init_controller(struct ata_host *host)
{
	struct device *pdev = host->dev;
	void __iomem *mmio = (void __iomem *)host->iomap;
	int i;
	void __iomem *port_mmio;
	u32 tmp;

	for (i = 0; i < host->n_ports; i++) {
		struct ata_port *ap = host->ports[i];

		port_mmio = ahci_port_base(ap);
		if (ata_port_is_dummy(ap))
			continue;

		ahci_port_init(pdev, ap, i, mmio, port_mmio);
	}

	tmp = readl(mmio + HOST_CTL);
	VPRINTK("HOST_CTL 0x%x\n", tmp);
	writel(tmp | HOST_IRQ_EN, mmio + HOST_CTL);
	tmp = readl(mmio + HOST_CTL);
	VPRINTK("HOST_CTL 0x%x\n", tmp);
}

static void ahci_dev_config(struct ata_device *dev)
{
	struct ahci_host_priv *hpriv = dev->link->ap->host->private_data;

	if (hpriv->flags & AHCI_HFLAG_SECT255) {
		dev->max_sectors = 255;
		ata_dev_printk(dev, KERN_INFO,
			       "SB600 AHCI: limiting to 255 sectors per cmd\n");
	}
}

static unsigned int ahci_dev_classify(struct ata_port *ap)
{
	void __iomem *port_mmio = ahci_port_base(ap);
	struct ata_taskfile tf;
	u32 tmp;

	tmp = readl(port_mmio + PORT_SIG);
	tf.lbah		= (tmp >> 24)	& 0xff;
	tf.lbam		= (tmp >> 16)	& 0xff;
	tf.lbal		= (tmp >> 8)	& 0xff;
	tf.nsect	= (tmp)		& 0xff;

	return ata_dev_classify(&tf);
}

static void ahci_fill_cmd_slot(struct ahci_port_priv *pp, unsigned int tag,
			       u32 opts)
{
	dma_addr_t cmd_tbl_dma;

	cmd_tbl_dma = pp->cmd_tbl_dma + tag * AHCI_CMD_TBL_SZ;

	pp->cmd_slot[tag].opts = cpu_to_le32(opts);
	pp->cmd_slot[tag].status = 0;
	pp->cmd_slot[tag].tbl_addr = cpu_to_le32(cmd_tbl_dma & 0xffffffff);
	pp->cmd_slot[tag].tbl_addr_hi = cpu_to_le32((cmd_tbl_dma >> 16) >> 16);
}

static int ahci_kick_engine(struct ata_port *ap, int force_restart)
{
	void __iomem *port_mmio = ahci_port_base(ap);
	struct ahci_host_priv *hpriv = ap->host->private_data;
	u8 status = readl(port_mmio + PORT_TFDATA) & 0xFF;
	u32 tmp;
	int busy, rc;

	/* do we need to kick the port? */
	busy = status & (ATA_BUSY | ATA_DRQ);
	if (!busy && !force_restart)
		return 0;

	/* stop engine */
	rc = ahci_stop_engine(ap);
	if (rc)
		goto out_restart;

	/* need to do CLO? */
	if (!busy) {
		rc = 0;
		goto out_restart;
	}

	if (!(hpriv->cap & HOST_CAP_CLO)) {
		rc = -EOPNOTSUPP;
		goto out_restart;
	}

	/* perform CLO */
	tmp = readl(port_mmio + PORT_CMD);
	tmp |= PORT_CMD_CLO;
	writel(tmp, port_mmio + PORT_CMD);

	rc = 0;
	tmp = ata_wait_register(port_mmio + PORT_CMD,
				PORT_CMD_CLO, PORT_CMD_CLO, 1, 500);
	if (tmp & PORT_CMD_CLO)
		rc = -EIO;

	/* restart engine */
 out_restart:
	ahci_start_engine(ap);
	return rc;
}

static int ahci_exec_polled_cmd(struct ata_port *ap, int pmp,
				struct ata_taskfile *tf, int is_cmd, u16 flags,
				unsigned long timeout_msec)
{
	const u32 cmd_fis_len = 5; /* five dwords */
	struct ahci_port_priv *pp = ap->private_data;
	void __iomem *port_mmio = ahci_port_base(ap);
	u8 *fis = pp->cmd_tbl;
	u32 tmp;

	/* prep the command */
	ata_tf_to_fis(tf, pmp, is_cmd, fis);
	ahci_fill_cmd_slot(pp, 0, cmd_fis_len | flags | (pmp << 12));

	/* issue & wait */
	writel(1, port_mmio + PORT_CMD_ISSUE);

	if (timeout_msec) {
		tmp = ata_wait_register(port_mmio + PORT_CMD_ISSUE, 0x1, 0x1,
					1, timeout_msec);
		if (tmp & 0x1) {
			ahci_kick_engine(ap, 1);
			return -EBUSY;
		}
	} else
		readl(port_mmio + PORT_CMD_ISSUE);	/* flush */

	return 0;
}

static int ahci_do_softreset(struct ata_link *link, unsigned int *class,
			     int pmp, unsigned long deadline,
			     int (*check_ready)(struct ata_link *link))
{
	struct ata_port *ap = link->ap;
	const char *reason = NULL;
	unsigned long now, msecs;
	struct ata_taskfile tf;
	int rc;

	DPRINTK("ENTER\n");

	/* prepare for SRST (AHCI-1.1 10.4.1) */
	rc = ahci_kick_engine(ap, 1);
	if (rc && rc != -EOPNOTSUPP)
		ata_link_printk(link, KERN_WARNING,
				"failed to reset engine (errno=%d)\n", rc);

	ata_tf_init(link->device, &tf);

	/* issue the first D2H Register FIS */
	msecs = 0;
	now = jiffies;
	if (time_after(now, deadline))
		msecs = jiffies_to_msecs(deadline - now);

	tf.ctl |= ATA_SRST;
	if (ahci_exec_polled_cmd(ap, pmp, &tf, 0,
				 AHCI_CMD_RESET | AHCI_CMD_CLR_BUSY, msecs)) {
		rc = -EIO;
		reason = "1st FIS failed";
		goto fail;
	}

	/* spec says at least 5us, but be generous and sleep for 1ms */
	msleep(1);

	/* issue the second D2H Register FIS */
	tf.ctl &= ~ATA_SRST;
	ahci_exec_polled_cmd(ap, pmp, &tf, 0, 0, 0);

	/* wait for link to become ready */
	rc = ata_wait_after_reset(link, deadline, check_ready);
	/* link occupied, -ENODEV too is an error */
	if (rc) {
		reason = "device not ready";
		goto fail;
	}
	*class = ahci_dev_classify(ap);

	DPRINTK("EXIT, class=%u\n", *class);
	return 0;

 fail:
	ata_link_printk(link, KERN_ERR, "softreset failed (%s)\n", reason);
	return rc;
}

static int ahci_check_ready(struct ata_link *link)
{
	void __iomem *port_mmio = ahci_port_base(link->ap);
	u8 status = readl(port_mmio + PORT_TFDATA) & 0xFF;

	return ata_check_ready(status);
}

static int ahci_softreset(struct ata_link *link, unsigned int *class,
			  unsigned long deadline)
{
	int pmp = sata_srst_pmp(link);

	DPRINTK("ENTER\n");

	return ahci_do_softreset(link, class, pmp, deadline, ahci_check_ready);
}

static int ahci_sb600_check_ready(struct ata_link *link)
{
	void __iomem *port_mmio = ahci_port_base(link->ap);
	u8 status = readl(port_mmio + PORT_TFDATA) & 0xFF;
	u32 irq_status = readl(port_mmio + PORT_IRQ_STAT);

	/*
	 * There is no need to check TFDATA if BAD PMP is found due to HW bug,
	 * which can save timeout delay.
	 */
	if (irq_status & PORT_IRQ_BAD_PMP)
		return -EIO;

	return ata_check_ready(status);
}

static int ahci_sb600_softreset(struct ata_link *link, unsigned int *class,
				unsigned long deadline)
{
	struct ata_port *ap = link->ap;
	void __iomem *port_mmio = ahci_port_base(ap);
	int pmp = sata_srst_pmp(link);
	int rc;
	u32 irq_sts;

	DPRINTK("ENTER\n");

	rc = ahci_do_softreset(link, class, pmp, deadline,
			       ahci_sb600_check_ready);

	/*
	 * Soft reset fails on some ATI chips with IPMS set when PMP
	 * is enabled but SATA HDD/ODD is connected to SATA port,
	 * do soft reset again to port 0.
	 */
	if (rc == -EIO) {
		irq_sts = readl(port_mmio + PORT_IRQ_STAT);
		if (irq_sts & PORT_IRQ_BAD_PMP) {
			ata_link_printk(link, KERN_WARNING,
					"failed due to HW bug, retry pmp=0\n");
			rc = ahci_do_softreset(link, class, 0, deadline,
					       ahci_check_ready);
		}
	}

	return rc;
}

static int ahci_hardreset(struct ata_link *link, unsigned int *class,
			  unsigned long deadline)
{
	const unsigned long *timing = sata_ehc_deb_timing(&link->eh_context);
	struct ata_port *ap = link->ap;
	struct ahci_port_priv *pp = ap->private_data;
	u8 *d2h_fis = pp->rx_fis + RX_FIS_D2H_REG;
	struct ata_taskfile tf;
	bool online;
	int rc;

	DPRINTK("ENTER\n");

	ahci_stop_engine(ap);

	/* clear D2H reception area to properly wait for D2H FIS */
	ata_tf_init(link->device, &tf);
	tf.command = 0x80;
	ata_tf_to_fis(&tf, 0, 0, d2h_fis);

	rc = sata_link_hardreset(link, timing, deadline, &online,
				 ahci_check_ready);

	ahci_start_engine(ap);

	if (online)
		*class = ahci_dev_classify(ap);

	DPRINTK("EXIT, rc=%d, class=%u\n", rc, *class);
	return rc;
}

static int ahci_vt8251_hardreset(struct ata_link *link, unsigned int *class,
				 unsigned long deadline)
{
	struct ata_port *ap = link->ap;
	bool online;
	int rc;

	DPRINTK("ENTER\n");

	ahci_stop_engine(ap);

	rc = sata_link_hardreset(link, sata_ehc_deb_timing(&link->eh_context),
				 deadline, &online, NULL);

	ahci_start_engine(ap);

	DPRINTK("EXIT, rc=%d, class=%u\n", rc, *class);

	/* vt8251 doesn't clear BSY on signature FIS reception,
	 * request follow-up softreset.
	 */
	return online ? -EAGAIN : rc;
}

static void ahci_postreset(struct ata_link *link, unsigned int *class)
{
	struct ata_port *ap = link->ap;
	void __iomem *port_mmio = ahci_port_base(ap);
	u32 new_tmp, tmp;

	ata_std_postreset(link, class);

	/* Make sure port's ATAPI bit is set appropriately */
	new_tmp = tmp = readl(port_mmio + PORT_CMD);
	if (*class == ATA_DEV_ATAPI)
		new_tmp |= PORT_CMD_ATAPI;
	else
		new_tmp &= ~PORT_CMD_ATAPI;
	if (new_tmp != tmp) {
		writel(new_tmp, port_mmio + PORT_CMD);
		readl(port_mmio + PORT_CMD); /* flush */
	}
}

static unsigned int ahci_fill_sg(struct ata_queued_cmd *qc, void *cmd_tbl)
{
	struct scatterlist *sg;
	struct ahci_sg *ahci_sg = cmd_tbl + AHCI_CMD_TBL_HDR_SZ;
	unsigned int si;

	VPRINTK("ENTER\n");

	/*
	 * Next, the S/G list.
	 */
	for_each_sg(qc->sg, sg, qc->n_elem, si) {
		dma_addr_t addr = sg_dma_address(sg);
		u32 sg_len = sg_dma_len(sg);

		ahci_sg[si].addr = cpu_to_le32(addr & 0xffffffff);
		ahci_sg[si].addr_hi = cpu_to_le32((addr >> 16) >> 16);
		ahci_sg[si].flags_size = cpu_to_le32(sg_len - 1);
	}

	return si;
}

static void ahci_qc_prep(struct ata_queued_cmd *qc)
{
	struct ata_port *ap = qc->ap;
	struct ahci_port_priv *pp = ap->private_data;
	int is_atapi = ata_is_atapi(qc->tf.protocol);
	void *cmd_tbl;
	u32 opts;
	const u32 cmd_fis_len = 5; /* five dwords */
	unsigned int n_elem;

	/*
	 * Fill in command table information.  First, the header,
	 * a SATA Register - Host to Device command FIS.
	 */
	cmd_tbl = pp->cmd_tbl + qc->tag * AHCI_CMD_TBL_SZ;

	ata_tf_to_fis(&qc->tf, qc->dev->link->pmp, 1, cmd_tbl);
	if (is_atapi) {
		memset(cmd_tbl + AHCI_CMD_TBL_CDB, 0, 32);
		memcpy(cmd_tbl + AHCI_CMD_TBL_CDB, qc->cdb, qc->dev->cdb_len);
	}

	n_elem = 0;
	if (qc->flags & ATA_QCFLAG_DMAMAP)
		n_elem = ahci_fill_sg(qc, cmd_tbl);

	/*
	 * Fill in command slot information.
	 */
	opts = cmd_fis_len | n_elem << 16 | (qc->dev->link->pmp << 12);
	if (qc->tf.flags & ATA_TFLAG_WRITE)
		opts |= AHCI_CMD_WRITE;
	if (is_atapi)
		opts |= AHCI_CMD_ATAPI | AHCI_CMD_PREFETCH;

	ahci_fill_cmd_slot(pp, qc->tag, opts);
}

static void ahci_error_intr(struct ata_port *ap, u32 irq_stat)
{
	struct ahci_host_priv *hpriv = ap->host->private_data;
	struct ahci_port_priv *pp = ap->private_data;
	struct ata_eh_info *host_ehi = &ap->link.eh_info;
	struct ata_link *link = NULL;
	struct ata_queued_cmd *active_qc;
	struct ata_eh_info *active_ehi;
	u32 serror;

	/* determine active link */
	ata_for_each_link(link, ap, EDGE)
		if (ata_link_active(link))
			break;
	if (!link)
		link = &ap->link;

	active_qc = ata_qc_from_tag(ap, link->active_tag);
	active_ehi = &link->eh_info;

	/* record irq stat */
	ata_ehi_clear_desc(host_ehi);
	ata_ehi_push_desc(host_ehi, "irq_stat 0x%08x", irq_stat);

	/* AHCI needs SError cleared; otherwise, it might lock up */
	ahci_scr_read(&ap->link, SCR_ERROR, &serror);
	ahci_scr_write(&ap->link, SCR_ERROR, serror);
	host_ehi->serror |= serror;

	/* some controllers set IRQ_IF_ERR on device errors, ignore it */
	if (hpriv->flags & AHCI_HFLAG_IGN_IRQ_IF_ERR)
		irq_stat &= ~PORT_IRQ_IF_ERR;

	if (irq_stat & PORT_IRQ_TF_ERR) {
		/* If qc is active, charge it; otherwise, the active
		 * link.  There's no active qc on NCQ errors.  It will
		 * be determined by EH by reading log page 10h.
		 */
		if (active_qc)
			active_qc->err_mask |= AC_ERR_DEV;
		else
			active_ehi->err_mask |= AC_ERR_DEV;

		if (hpriv->flags & AHCI_HFLAG_IGN_SERR_INTERNAL)
			host_ehi->serror &= ~SERR_INTERNAL;
	}

	if (irq_stat & PORT_IRQ_UNK_FIS) {
		u32 *unk = (u32 *)(pp->rx_fis + RX_FIS_UNK);

		active_ehi->err_mask |= AC_ERR_HSM;
		active_ehi->action |= ATA_EH_RESET;
		ata_ehi_push_desc(active_ehi,
				  "unknown FIS %08x %08x %08x %08x" ,
				  unk[0], unk[1], unk[2], unk[3]);
	}

	if (sata_pmp_attached(ap) && (irq_stat & PORT_IRQ_BAD_PMP)) {
		active_ehi->err_mask |= AC_ERR_HSM;
		active_ehi->action |= ATA_EH_RESET;
		ata_ehi_push_desc(active_ehi, "incorrect PMP");
	}

	if (irq_stat & (PORT_IRQ_HBUS_ERR | PORT_IRQ_HBUS_DATA_ERR)) {
		host_ehi->err_mask |= AC_ERR_HOST_BUS;
		host_ehi->action |= ATA_EH_RESET;
		ata_ehi_push_desc(host_ehi, "host bus error");
	}

	if (irq_stat & PORT_IRQ_IF_ERR) {
		host_ehi->err_mask |= AC_ERR_ATA_BUS;
		host_ehi->action |= ATA_EH_RESET;
		ata_ehi_push_desc(host_ehi, "interface fatal error");
	}

	if (irq_stat & (PORT_IRQ_CONNECT | PORT_IRQ_PHYRDY)) {
		ata_ehi_hotplugged(host_ehi);
		ata_ehi_push_desc(host_ehi, "%s",
			irq_stat & PORT_IRQ_CONNECT ?
			"connection status changed" : "PHY RDY changed");
	}

	/* okay, let's hand over to EH */

	if (irq_stat & PORT_IRQ_FREEZE)
		ata_port_freeze(ap);
	else
		ata_port_abort(ap);
}

static void ahci_port_intr(struct ata_port *ap)
{
	void __iomem *port_mmio = ahci_port_base(ap);
	struct ata_eh_info *ehi = &ap->link.eh_info;
	struct ahci_port_priv *pp = ap->private_data;
	struct ahci_host_priv *hpriv = ap->host->private_data;
	int resetting = !!(ap->pflags & ATA_PFLAG_RESETTING);
	u32 status, qc_active;
	int rc;

	status = readl(port_mmio + PORT_IRQ_STAT);
	writel(status, port_mmio + PORT_IRQ_STAT);

	/* ignore BAD_PMP while resetting */
	if (unlikely(resetting))
		status &= ~PORT_IRQ_BAD_PMP;

	/* If we are getting PhyRdy, this is
 	 * just a power state change, we should
 	 * clear out this, plus the PhyRdy/Comm
 	 * Wake bits from Serror
 	 */
	if ((hpriv->flags & AHCI_HFLAG_NO_HOTPLUG) &&
		(status & PORT_IRQ_PHYRDY)) {
		status &= ~PORT_IRQ_PHYRDY;
		ahci_scr_write(&ap->link, SCR_ERROR, ((1 << 16) | (1 << 18)));
	}

	if (unlikely(status & PORT_IRQ_ERROR)) {
		ahci_error_intr(ap, status);
		return;
	}

	if (status & PORT_IRQ_SDB_FIS) {
		/* If SNotification is available, leave notification
		 * handling to sata_async_notification().  If not,
		 * emulate it by snooping SDB FIS RX area.
		 *
		 * Snooping FIS RX area is probably cheaper than
		 * poking SNotification but some constrollers which
		 * implement SNotification, ICH9 for example, don't
		 * store AN SDB FIS into receive area.
		 */
		if (hpriv->cap & HOST_CAP_SNTF)
			sata_async_notification(ap);
		else {
			/* If the 'N' bit in word 0 of the FIS is set,
			 * we just received asynchronous notification.
			 * Tell libata about it.
			 */
			const __le32 *f = pp->rx_fis + RX_FIS_SDB;
			u32 f0 = le32_to_cpu(f[0]);

			if (f0 & (1 << 15))
				sata_async_notification(ap);
		}
	}

	/* pp->active_link is valid iff any command is in flight */
	if (ap->qc_active && pp->active_link->sactive)
		qc_active = readl(port_mmio + PORT_SCR_ACT);
	else
		qc_active = readl(port_mmio + PORT_CMD_ISSUE);

	rc = ata_qc_complete_multiple(ap, qc_active);

	/* while resetting, invalid completions are expected */
	if (unlikely(rc < 0 && !resetting)) {
		ehi->err_mask |= AC_ERR_HSM;
		ehi->action |= ATA_EH_RESET;
		ata_port_freeze(ap);
	}
}

static irqreturn_t ahci_interrupt(int irq, void *dev_instance)
{
	struct ata_host *host = dev_instance;
	struct ahci_host_priv *hpriv;
	unsigned int i, handled = 0;
	void __iomem *mmio;
	u32 irq_stat, irq_masked;

	VPRINTK("ENTER\n");

	hpriv = host->private_data;
	mmio = (void __iomem *)host->iomap;

	/* sigh.  0xffffffff is a valid return from h/w */
	irq_stat = readl(mmio + HOST_IRQ_STAT);
	if (!irq_stat)
		return IRQ_NONE;

	irq_masked = irq_stat & hpriv->port_map;

	spin_lock(&host->lock);

	for (i = 0; i < host->n_ports; i++) {
		struct ata_port *ap;

		if (!(irq_masked & (1 << i)))
			continue;

		ap = host->ports[i];
		if (ap) {
			ahci_port_intr(ap);
			VPRINTK("port %u\n", i);
		} else {
			VPRINTK("port %u (no irq)\n", i);
			if (ata_ratelimit())
				dev_printk(KERN_WARNING, host->dev,
					"interrupt on disabled port %u\n", i);
		}

		handled = 1;
	}

	/* HOST_IRQ_STAT behaves as level triggered latch meaning that
	 * it should be cleared after all the port events are cleared;
	 * otherwise, it will raise a spurious interrupt after each
	 * valid one.  Please read section 10.6.2 of ahci 1.1 for more
	 * information.
	 *
	 * Also, use the unmasked value to clear interrupt as spurious
	 * pending event on a dummy port might cause screaming IRQ.
	 */
	writel(irq_stat, mmio + HOST_IRQ_STAT);

	spin_unlock(&host->lock);

	VPRINTK("EXIT\n");

	return IRQ_RETVAL(handled);
}

static unsigned int ahci_qc_issue(struct ata_queued_cmd *qc)
{
	struct ata_port *ap = qc->ap;
	void __iomem *port_mmio = ahci_port_base(ap);
	struct ahci_port_priv *pp = ap->private_data;

	/* Keep track of the currently active link.  It will be used
	 * in completion path to determine whether NCQ phase is in
	 * progress.
	 */
	pp->active_link = qc->dev->link;

	if (qc->tf.protocol == ATA_PROT_NCQ)
		writel(1 << qc->tag, port_mmio + PORT_SCR_ACT);
	writel(1 << qc->tag, port_mmio + PORT_CMD_ISSUE);

	ahci_sw_activity(qc->dev->link);

	return 0;
}

static bool ahci_qc_fill_rtf(struct ata_queued_cmd *qc)
{
	struct ahci_port_priv *pp = qc->ap->private_data;
	u8 *d2h_fis = pp->rx_fis + RX_FIS_D2H_REG;

	ata_tf_from_fis(d2h_fis, &qc->result_tf);
	return true;
}

static void ahci_freeze(struct ata_port *ap)
{
	void __iomem *port_mmio = ahci_port_base(ap);

	/* turn IRQ off */
	writel(0, port_mmio + PORT_IRQ_MASK);
}

static void ahci_thaw(struct ata_port *ap)
{
	void __iomem *mmio = (void __iomem *)ap->host->iomap;
	void __iomem *port_mmio = ahci_port_base(ap);
	u32 tmp;
	struct ahci_port_priv *pp = ap->private_data;

	/* clear IRQ */
	tmp = readl(port_mmio + PORT_IRQ_STAT);
	writel(tmp, port_mmio + PORT_IRQ_STAT);
	writel(1 << ap->port_no, mmio + HOST_IRQ_STAT);

	/* turn IRQ back on */
	writel(pp->intr_mask, port_mmio + PORT_IRQ_MASK);
}

static void ahci_error_handler(struct ata_port *ap)
{
	if (!(ap->pflags & ATA_PFLAG_FROZEN)) {
		/* restart engine */
		ahci_stop_engine(ap);
		ahci_start_engine(ap);
	}

	sata_pmp_error_handler(ap);
}

static void ahci_post_internal_cmd(struct ata_queued_cmd *qc)
{
	struct ata_port *ap = qc->ap;

	/* make DMA engine forget about the failed command */
	if (qc->flags & ATA_QCFLAG_FAILED)
		ahci_kick_engine(ap, 1);
}

static void ahci_pmp_attach(struct ata_port *ap)
{
	void __iomem *port_mmio = ahci_port_base(ap);
	struct ahci_port_priv *pp = ap->private_data;
	u32 cmd;

	cmd = readl(port_mmio + PORT_CMD);
	cmd |= PORT_CMD_PMP;
	writel(cmd, port_mmio + PORT_CMD);

	pp->intr_mask |= PORT_IRQ_BAD_PMP;
	writel(pp->intr_mask, port_mmio + PORT_IRQ_MASK);
}

static void ahci_pmp_detach(struct ata_port *ap)
{
	void __iomem *port_mmio = ahci_port_base(ap);
	struct ahci_port_priv *pp = ap->private_data;
	u32 cmd;

	cmd = readl(port_mmio + PORT_CMD);
	cmd &= ~PORT_CMD_PMP;
	writel(cmd, port_mmio + PORT_CMD);

	pp->intr_mask &= ~PORT_IRQ_BAD_PMP;
	writel(pp->intr_mask, port_mmio + PORT_IRQ_MASK);
}

static int ahci_port_resume(struct ata_port *ap)
{
	ahci_power_up(ap);
	ahci_start_port(ap);

	if (sata_pmp_attached(ap))
		ahci_pmp_attach(ap);
	else
		ahci_pmp_detach(ap);

	return 0;
}

#ifdef CONFIG_PM
static int ahci_port_suspend(struct ata_port *ap, pm_message_t mesg)
{
	const char *emsg = NULL;
	int rc;

	rc = ahci_deinit_port(ap, &emsg);
	if (rc == 0)
		ahci_power_down(ap);
	else {
		ata_port_printk(ap, KERN_ERR, "%s (%d)\n", emsg, rc);
		ahci_start_port(ap);
	}

	return rc;
}

static int ahci_device_suspend(struct platform_device* pdev, pm_message_t mesg)
{
	struct ata_host *host = dev_get_drvdata(&pdev->dev);
	void __iomem *mmio = (void __iomem *)host->iomap;
	u32 ctl;
	int rc;

	if (mesg.event == PM_EVENT_SUSPEND) {
		/* AHCI spec rev1.1 section 8.3.3:
		 * Software must disable interrupts prior to requesting a
		 * transition of the HBA to D3 state.
		 */
		ctl = readl(mmio + HOST_CTL);
		ctl &= ~HOST_IRQ_EN;
		writel(ctl, mmio + HOST_CTL);
		readl(mmio + HOST_CTL); /* flush */
	}

	rc = ata_host_suspend(host, mesg);
	if (rc)
		return rc;

	return 0;
}

static int ahci_device_resume(struct platform_device* pdev)
{
	struct ata_host *host = dev_get_drvdata(&pdev->dev);
	struct ahci_host_priv *hpriv = host->private_data;
	int rc;

	if ( hpriv ) {
		if ( hpriv->irq == IRQ_SATA0 )	{
			ahci_sata_phy0_init();
		}
		else {
			ahci_sata_phy1_init();
		}
	}

	if (pdev->dev.power.power_state.event == PM_EVENT_SUSPEND) {
		rc = ahci_reset_controller(host);
		if (rc)
			return rc;
		ahci_init_controller(host);
	}
	ata_host_resume(host);

	return 0;
}

#endif

static int ahci_port_start(struct ata_port *ap)
{
	struct device *dev = ap->host->dev;
	struct ahci_port_priv *pp;
	void *mem;
	dma_addr_t mem_dma;

	pp = devm_kzalloc(dev, sizeof(*pp), GFP_KERNEL);
	if (!pp)
		return -ENOMEM;

	mem = dmam_alloc_coherent(dev, AHCI_PORT_PRIV_DMA_SZ, &mem_dma,
				  GFP_KERNEL);
	if (!mem)
		return -ENOMEM;
	memset(mem, 0, AHCI_PORT_PRIV_DMA_SZ);

	/*
	 * First item in chunk of DMA memory: 32-slot command table,
	 * 32 bytes each in size
	 */
	pp->cmd_slot = mem;
	pp->cmd_slot_dma = mem_dma;

	mem += AHCI_CMD_SLOT_SZ;
	mem_dma += AHCI_CMD_SLOT_SZ;

	/*
	 * Second item: Received-FIS area
	 */
	pp->rx_fis = mem;
	pp->rx_fis_dma = mem_dma;

	mem += AHCI_RX_FIS_SZ;
	mem_dma += AHCI_RX_FIS_SZ;

	/*
	 * Third item: data area for storing a single command
	 * and its scatter-gather table
	 */
	pp->cmd_tbl = mem;
	pp->cmd_tbl_dma = mem_dma;

	/*
	 * Save off initial list of interrupts to be enabled.
	 * This could be changed later
	 */
	pp->intr_mask = DEF_PORT_IRQ;

	ap->private_data = pp;

	/* engage engines, captain */
	return ahci_port_resume(ap);
}

static void ahci_port_stop(struct ata_port *ap)
{
	const char *emsg = NULL;
	int rc;

	/* de-initialize port */
	rc = ahci_deinit_port(ap, &emsg);
	if (rc)
		ata_port_printk(ap, KERN_WARNING, "%s (%d)\n", emsg, rc);
}

static void ahci_print_info(struct ata_host *host)
{
	struct ahci_host_priv *hpriv = host->private_data;
	struct device *pdev = host->dev;
	void __iomem *mmio = (void __iomem *)host->iomap;
	u32 vers, cap, impl, speed;
	const char *speed_s;
	const char *scc_s;

	vers = readl(mmio + HOST_VERSION);
	cap = hpriv->cap;
	impl = hpriv->port_map;

	speed = (cap >> 20) & 0xf;
	if (speed == 1)
		speed_s = "1.5";
	else if (speed == 2)
		speed_s = "3";
	else
		speed_s = "?";

	scc_s = "SATA";

	dev_printk(KERN_INFO, pdev,
		"AHCI %02x%02x.%02x%02x "
		"%u slots %u ports %s Gbps 0x%x impl %s mode\n"
		,

		(vers >> 24) & 0xff,
		(vers >> 16) & 0xff,
		(vers >> 8) & 0xff,
		vers & 0xff,

		((cap >> 8) & 0x1f) + 1,
		(cap & 0x1f) + 1,
		speed_s,
		impl,
		scc_s);

	dev_printk(KERN_INFO, pdev,
		"flags: "
		"%s%s%s%s%s%s%s"
		"%s%s%s%s%s%s%s\n"
		,

		cap & (1 << 31) ? "64bit " : "",
		cap & (1 << 30) ? "ncq " : "",
		cap & (1 << 29) ? "sntf " : "",
		cap & (1 << 28) ? "ilck " : "",
		cap & (1 << 27) ? "stag " : "",
		cap & (1 << 26) ? "pm " : "",
		cap & (1 << 25) ? "led " : "",

		cap & (1 << 24) ? "clo " : "",
		cap & (1 << 19) ? "nz " : "",
		cap & (1 << 18) ? "only " : "",
		cap & (1 << 17) ? "pmp " : "",
		cap & (1 << 15) ? "pio " : "",
		cap & (1 << 14) ? "slum " : "",
		cap & (1 << 13) ? "part " : ""
		);
}


static int sata_ahci_probe(struct platform_device *pdev)
{
	int i;
	static int printed_version;
	struct ahci_host_priv *hpriv;
	int irq;
	struct ata_host *host;
	struct ata_port_info pi = ahci_port_info[board_ahci];
	const struct ata_port_info *ppi[] = { &pi, NULL };
	struct resource		*res;

	VPRINTK("ENTER\n");
	WARN_ON(ATA_MAX_QUEUE > AHCI_MAX_CMDS);

	if (!printed_version++)
		dev_printk(KERN_DEBUG, &pdev->dev, "version " DRV_VERSION "\n");

	hpriv = kzalloc(sizeof(struct ahci_host_priv), GFP_KERNEL);
	if (!hpriv)
		goto error_exit_with_cleanup;

	hpriv->flags |= (unsigned long)pi.private_data;

	/* Get the IRQ */
	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		goto error_exit_with_cleanup;

	/* irq number */
	hpriv->irq = irq;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if ( !res ) 
		goto error_exit_with_cleanup;

	/* mapping iomap */
	hpriv->iomap = ioremap_nocache(res->start, (res->end - res->start + 1));

	/* save initial config */
	ahci_save_initial_config(pdev, hpriv);

	/* prepare host */
	if (hpriv->cap & HOST_CAP_NCQ)
		pi.flags |= ATA_FLAG_NCQ;

	if (hpriv->cap & HOST_CAP_PMP)
		pi.flags |= ATA_FLAG_PMP;

	host = ata_host_alloc_pinfo(&pdev->dev, ppi, 1);
	if (!host)
		goto error_exit_with_cleanup;

	host->iomap = hpriv->iomap;
	host->private_data = hpriv;

	for (i = 0; i < host->n_ports; i++) {
		struct ata_port *ap = host->ports[i];
		void __iomem *port_mmio = ahci_port_base(ap);

		/* set initial link pm policy */
		ap->pm_policy = NOT_AVAILABLE;

		/* standard SATA port setup */
		if (hpriv->port_map & (1 << i))
			ap->ioaddr.cmd_addr = port_mmio;

		/* disabled/not-implemented port */
		else
			ap->ops = &ata_dummy_port_ops;
	}

	/* initialize adapter */
	if (ahci_reset_controller(host))
		goto error_exit_with_cleanup;

	ahci_init_controller(host);
	ahci_print_info(host);

	ata_host_activate(host, irq, ahci_interrupt, IRQF_SHARED, &ahci_sht);

	return 0;

error_exit_with_cleanup:

	if (hpriv->iomap)
		iounmap(hpriv->iomap);

	if (hpriv)
		kfree(hpriv);

	return 0;

}

static int sata_ahci_remove(struct platform_device* pdev)
{
	struct ata_host *host = dev_get_drvdata(&pdev->dev);
	struct ahci_host_priv *hpriv = host->private_data;

	ata_host_detach(host);

#if 0	// platform_driver_unregister executes belows
	dev_set_drvdata(&pdev->dev, NULL);
	free_irq(hpriv->irq, host);
#endif
	
	iounmap(hpriv->iomap);
	
	kfree(hpriv);

	return 0;
}


static struct platform_driver  ahci_platform_driver[2] = {
	{
	.probe                = sata_ahci_probe,
	.remove               = sata_ahci_remove,
#ifdef CONFIG_PM
	.suspend			= ahci_device_suspend,
	.resume			= ahci_device_resume,
#endif
	.driver               = 
		{
	  	.name       = "ahci-sata0",
	  	.owner      = THIS_MODULE,
		},
	},

	{
	.probe				  = sata_ahci_probe,
	.remove 			  = sata_ahci_remove,
#ifdef CONFIG_PM
	.suspend			= ahci_device_suspend,
	.resume			= ahci_device_resume,
#endif
	.driver 			  = 
	  {
	  .name 	  = "ahci-sata1",
	  .owner	  = THIS_MODULE,
	  },
	},

};


#include <mach/hardware.h>

static void ahci_set_reg(u32 addr, unsigned int val)
{
	iowrite32(val, addr + DIFF_IO_BASE0);
}

static u32 ahci_get_reg(u32 addr)
{
	return ioread32(addr + DIFF_IO_BASE0);
}

static void ahci_sata_phy0_init(void)
{
	unsigned int regval;
	int i;
	
//	int cnt;
//	unsigned int p, m, s, k, ahbclk;
//	unsigned int cwMin, cwMax, ciMin, ciMax;
	
	// Added this line to make module
	regval=ahci_get_reg(0x300908c0)&~0x00001000; // SW_RESET0[21]: SATA0 Phy Reset 
	ahci_set_reg(0x300908c0, regval);

	// release HSATA 0 reset
	//regval=ahci_get_reg(0x300908c0)|0x00400000; // 2010.08.30 - jcshin
	regval=ahci_get_reg(0x300908c0)|0x00200000; // SW_RESET0[21]: SATA0 Controller Reset 
	ahci_set_reg(0x300908c0, regval);


// 2010.08.30 - jcshin 
/*
	regval = ahci_get_reg(0x30090808);
	p = PMU_PLL_P_VALUE(regval);
	m = PMU_PLL_M_VALUE(regval);
	s = PMU_PLL_S_VALUE(regval);
	k = ahci_get_reg(0x3009081C)&0xffff;

	ahbclk = (((m+k/1024) * ((24000000 >> s) / p)) >> 2) / 1000; //khz

	cwMin = CWMIN_DFT * ahbclk / ABHCLK_DFT;
	cwMax = CWMAX_DFT * ahbclk / ABHCLK_DFT;
	ciMin = CIMIN_DFT * ahbclk / ABHCLK_DFT;	
	ciMax = CIMAX_DFT * ahbclk / ABHCLK_DFT;		

	// OOBR - adjust values according to the Bus(SMX) clock 
	regval=ahci_get_reg(0x300100bc)|0x80000000;
	ahci_set_reg(0x300100bc, regval);
	regval = 0x80000000 | (cwMin << 24) | (cwMax << 16) | (ciMin << 8) | (ciMax << 0);
	ahci_set_reg(0x300100bc, regval);
	regval=ahci_get_reg(0x300100bc)&0x7fffffff;
	ahci_set_reg(0x300100bc, regval);
*/

	// RXERR override (PHYCR[9] should be 1)
	regval=ahci_get_reg(0x30010178)&0xfffffdff;
	regval=regval|0x00000200;
	ahci_set_reg(0x30010178, regval);

	// release SATA 1 PHY reset
	//regval=ahci_get_reg(0x300908c0)|0x00002000; // 2010.08.30 - jcshin
	regval=ahci_get_reg(0x300908c0)|0x00001000; // SW_RESET0[12]: SATA0 Reset 
	ahci_set_reg(0x300908c0, regval);

	// may need time for resetting PHY
	for( i = 0; i < 100; i++ );
	
	// PHYCR[14] = 1 release CMU PowerDown Exit
	regval=ahci_get_reg(0x30010178)|0x00004000;
	ahci_set_reg(0x30010178, regval);
	for( i = 0; i < 100; i++ );

	// CMU
	ahci_set_reg(0x300f0000, 0x00000006);
	ahci_set_reg(0x300f0004, 0x00000000);
	ahci_set_reg(0x300f0008, 0x00000088);
	ahci_set_reg(0x300f000c, 0x00000000);
	ahci_set_reg(0x300f0010, 0x0000007b);
	ahci_set_reg(0x300f0014, 0x000000c9);
	ahci_set_reg(0x300f0018, 0x00000003);
	ahci_set_reg(0x300f001c, 0x00000000);
	ahci_set_reg(0x300f0020, 0x00000000);
	ahci_set_reg(0x300f0024, 0x00000000);
	ahci_set_reg(0x300f0028, 0x00000000);
	ahci_set_reg(0x300f002c, 0x00000000);
	ahci_set_reg(0x300f0030, 0x00000000);
	ahci_set_reg(0x300f0034, 0x00000000);
	ahci_set_reg(0x300f0038, 0x00000000);
	ahci_set_reg(0x300f003c, 0x00000000);
	ahci_set_reg(0x300f0040, 0x00000000);
	ahci_set_reg(0x300f0044, 0x00000000);
	ahci_set_reg(0x300f0048, 0x00000000);
	ahci_set_reg(0x300f004c, 0x00000000);
	ahci_set_reg(0x300f0050, 0x00000000);
	ahci_set_reg(0x300f0054, 0x00000000);
	ahci_set_reg(0x300f0058, 0x00000000);
	ahci_set_reg(0x300f005c, 0x00000000);
	ahci_set_reg(0x300f0060, 0x00000000);
	ahci_set_reg(0x300f0064, 0x00000000);
	ahci_set_reg(0x300f0068, 0x00000000);
	ahci_set_reg(0x300f006c, 0x00000000);
	ahci_set_reg(0x300f0070, 0x00000000);
	ahci_set_reg(0x300f0074, 0x00000000);
	ahci_set_reg(0x300f0078, 0x00000000);
	ahci_set_reg(0x300f007c, 0x00000000);
	ahci_set_reg(0x300f0080, 0x00000000);
	ahci_set_reg(0x300f0084, 0x00000000);
	ahci_set_reg(0x300f0088, 0x000000a0);
	ahci_set_reg(0x300f008c, 0x00000054);
	ahci_set_reg(0x300f0090, 0x00000000);
	ahci_set_reg(0x300f0094, 0x00000000);
	ahci_set_reg(0x300f0098, 0x00000000);
	ahci_set_reg(0x300f009c, 0x00000000);
	ahci_set_reg(0x300f00a0, 0x00000000);
	ahci_set_reg(0x300f00a4, 0x00000000);
	ahci_set_reg(0x300f00a8, 0x00000000);
	ahci_set_reg(0x300f00ac, 0x00000000);
	ahci_set_reg(0x300f00b0, 0x00000000);
	ahci_set_reg(0x300f00b4, 0x00000000);
	ahci_set_reg(0x300f00b8, 0x00000004);
	ahci_set_reg(0x300f00bc, 0x00000050);
	ahci_set_reg(0x300f00c0, 0x00000070);
	ahci_set_reg(0x300f00c4, 0x00000002);
	ahci_set_reg(0x300f00c8, 0x00000025);
	ahci_set_reg(0x300f00cc, 0x00000040);
	ahci_set_reg(0x300f00d0, 0x00000001);
	ahci_set_reg(0x300f00d4, 0x00000040);
	ahci_set_reg(0x300f00d8, 0x00000000);
	ahci_set_reg(0x300f00dc, 0x00000000);
	ahci_set_reg(0x300f00e0, 0x00000000);
	ahci_set_reg(0x300f00e4, 0x00000000);
	ahci_set_reg(0x300f00e8, 0x00000000);
	ahci_set_reg(0x300f00ec, 0x00000000);
	ahci_set_reg(0x300f00f0, 0x00000000);
	ahci_set_reg(0x300f00f4, 0x00000000);
	ahci_set_reg(0x300f00f8, 0x00000000);
	ahci_set_reg(0x300f00fc, 0x00000000);
	ahci_set_reg(0x300f0100, 0x00000000);
	ahci_set_reg(0x300f0104, 0x00000000);
	ahci_set_reg(0x300f0108, 0x00000000);
	ahci_set_reg(0x300f010c, 0x00000000);
	ahci_set_reg(0x300f0110, 0x00000000);
	ahci_set_reg(0x300f0114, 0x00000000);
	ahci_set_reg(0x300f0118, 0x00000000);
	ahci_set_reg(0x300f011c, 0x00000000);
	ahci_set_reg(0x300f0120, 0x00000000);
	ahci_set_reg(0x300f0124, 0x00000000);
	ahci_set_reg(0x300f0128, 0x00000000);
	ahci_set_reg(0x300f012c, 0x00000000);
	ahci_set_reg(0x300f0130, 0x00000000);
	ahci_set_reg(0x300f0134, 0x00000000);
	ahci_set_reg(0x300f0138, 0x00000000);
	ahci_set_reg(0x300f013c, 0x00000000);
	ahci_set_reg(0x300f0140, 0x00000000);
	ahci_set_reg(0x300f0144, 0x00000000);
	ahci_set_reg(0x300f0148, 0x00000000);
	ahci_set_reg(0x300f014c, 0x00000000);
	ahci_set_reg(0x300f0150, 0x00000000);
	ahci_set_reg(0x300f0154, 0x00000000);
	ahci_set_reg(0x300f0158, 0x00000000);
	ahci_set_reg(0x300f015c, 0x00000000);
	ahci_set_reg(0x300f0160, 0x00000000);
	ahci_set_reg(0x300f0164, 0x00000000);
	ahci_set_reg(0x300f0168, 0x00000000);
	ahci_set_reg(0x300f016c, 0x00000000);
	ahci_set_reg(0x300f0170, 0x00000000);
	ahci_set_reg(0x300f0174, 0x00000000);
	ahci_set_reg(0x300f0178, 0x00000000);
	ahci_set_reg(0x300f017c, 0x00000000);
	ahci_set_reg(0x300f0180, 0x00000000);
	ahci_set_reg(0x300f0184, 0x0000002e);
	ahci_set_reg(0x300f0188, 0x00000000);
	ahci_set_reg(0x300f018c, 0x0000005e);
	ahci_set_reg(0x300f0190, 0x00000000);
	ahci_set_reg(0x300f0194, 0x00000042);
	ahci_set_reg(0x300f0198, 0x000000d1);
	ahci_set_reg(0x300f019c, 0x00000020);
	ahci_set_reg(0x300f01a0, 0x00000028);
	ahci_set_reg(0x300f01a4, 0x00000078);
	ahci_set_reg(0x300f01a8, 0x0000002c);
	ahci_set_reg(0x300f01ac, 0x000000b9);
	ahci_set_reg(0x300f01b0, 0x0000005e);
	//ahci_set_reg(0x300f01b4, 0x00000000); //external differential reference clock input
	ahci_set_reg(0x300f01b4, 0x00000003); //internal refclk_core_i input
	ahci_set_reg(0x300f01b8, 0x00000000);
	ahci_set_reg(0x300f01bc, 0x00000000);
	ahci_set_reg(0x300f01c0, 0x00000000);
	ahci_set_reg(0x300f01c4, 0x00000000);
	ahci_set_reg(0x300f01c8, 0x00000000);

	// LANE0
	ahci_set_reg(0x300f0800, 0x00000002);
	ahci_set_reg(0x300f0804, 0x00000000);
	ahci_set_reg(0x300f0808, 0x00000000);
	ahci_set_reg(0x300f080c, 0x00000000);
	ahci_set_reg(0x300f0810, 0x00000000);
	ahci_set_reg(0x300f0814, 0x00000010);
	ahci_set_reg(0x300f0818, 0x00000084);
	ahci_set_reg(0x300f081c, 0x00000004);
	ahci_set_reg(0x300f0820, 0x000000e0);
	ahci_set_reg(0x300f0840, 0x00000023);
	ahci_set_reg(0x300f0844, 0x00000000);
	ahci_set_reg(0x300f0848, 0x00000040);
	ahci_set_reg(0x300f084c, 0x00000005);
	ahci_set_reg(0x300f0850, 0x000000d0);
	ahci_set_reg(0x300f0854, 0x00000017);
	ahci_set_reg(0x300f0858, 0x00000000);
	ahci_set_reg(0x300f085c, 0x00000068);
	ahci_set_reg(0x300f0860, 0x000000f2);
	ahci_set_reg(0x300f0864, 0x0000001e);
	ahci_set_reg(0x300f0868, 0x00000018);
	ahci_set_reg(0x300f086c, 0x0000000d);
	ahci_set_reg(0x300f0870, 0x0000000c);
	ahci_set_reg(0x300f0874, 0x00000000);
	ahci_set_reg(0x300f0878, 0x00000000);
	ahci_set_reg(0x300f087c, 0x00000000);
	ahci_set_reg(0x300f0880, 0x00000000);
	ahci_set_reg(0x300f0884, 0x00000000);
	ahci_set_reg(0x300f0888, 0x00000000);
	ahci_set_reg(0x300f088c, 0x00000000);
	ahci_set_reg(0x300f0890, 0x00000000);
	ahci_set_reg(0x300f0894, 0x00000000);
	ahci_set_reg(0x300f0898, 0x00000000);
	ahci_set_reg(0x300f089c, 0x00000000);
	ahci_set_reg(0x300f08a0, 0x00000000);
	ahci_set_reg(0x300f08a4, 0x00000000);
	ahci_set_reg(0x300f08a8, 0x00000000);
	ahci_set_reg(0x300f08ac, 0x00000000);
	ahci_set_reg(0x300f08b0, 0x00000000);
	ahci_set_reg(0x300f08b4, 0x00000000);
	ahci_set_reg(0x300f08b8, 0x00000000);
	ahci_set_reg(0x300f08bc, 0x00000000);
	ahci_set_reg(0x300f08c0, 0x00000000);
	ahci_set_reg(0x300f08c4, 0x00000000);
	ahci_set_reg(0x300f08c8, 0x00000000);
	ahci_set_reg(0x300f08cc, 0x00000000);
	ahci_set_reg(0x300f08d0, 0x00000000);
	ahci_set_reg(0x300f08d4, 0x00000000);
	ahci_set_reg(0x300f08d8, 0x00000000);
	ahci_set_reg(0x300f08dc, 0x00000000);
	ahci_set_reg(0x300f08e0, 0x00000000);
	ahci_set_reg(0x300f08e4, 0x00000000);
	ahci_set_reg(0x300f08e8, 0x00000000);
	ahci_set_reg(0x300f08ec, 0x00000000);
	ahci_set_reg(0x300f08f0, 0x00000000);
	ahci_set_reg(0x300f08f4, 0x00000000);
	ahci_set_reg(0x300f08f8, 0x00000000);
	ahci_set_reg(0x300f08fc, 0x00000000);
	ahci_set_reg(0x300f0900, 0x00000000);
	ahci_set_reg(0x300f0904, 0x00000000);
	ahci_set_reg(0x300f0908, 0x00000000);
	ahci_set_reg(0x300f090c, 0x00000000);
	ahci_set_reg(0x300f0910, 0x00000000);
	ahci_set_reg(0x300f0914, 0x00000000);
	ahci_set_reg(0x300f0918, 0x00000000);
	ahci_set_reg(0x300f091c, 0x00000000);
	ahci_set_reg(0x300f0920, 0x00000000);
	ahci_set_reg(0x300f0924, 0x00000000);
	ahci_set_reg(0x300f0928, 0x00000000);
	ahci_set_reg(0x300f092c, 0x00000000);
	ahci_set_reg(0x300f0930, 0x00000000);
	ahci_set_reg(0x300f0934, 0x00000000);
	ahci_set_reg(0x300f0938, 0x00000000);
	ahci_set_reg(0x300f093c, 0x00000000);
	ahci_set_reg(0x300f0940, 0x00000060);
	ahci_set_reg(0x300f0944, 0x0000000f);

	// COMLANE
	ahci_set_reg(0x300f2800, 0x00000000);
	ahci_set_reg(0x300f2804, 0x00000020);
	ahci_set_reg(0x300f2808, 0x00000000);
	ahci_set_reg(0x300f280c, 0x00000040);
	ahci_set_reg(0x300f2810, 0x00000024);
	ahci_set_reg(0x300f2814, 0x000000ae);
	ahci_set_reg(0x300f2818, 0x00000019);
	ahci_set_reg(0x300f281c, 0x00000049);
	ahci_set_reg(0x300f2820, 0x00000004);
	ahci_set_reg(0x300f2824, 0x00000083);
	ahci_set_reg(0x300f2828, 0x0000004b);
	ahci_set_reg(0x300f282c, 0x000000c5);
	ahci_set_reg(0x300f2830, 0x00000001);
	ahci_set_reg(0x300f2834, 0x00000003);
	ahci_set_reg(0x300f2838, 0x00000028);
	ahci_set_reg(0x300f283c, 0x00000098);
	ahci_set_reg(0x300f2840, 0x00000019);
	ahci_set_reg(0x300f2844, 0x00000000);
	ahci_set_reg(0x300f2848, 0x00000000);
	ahci_set_reg(0x300f284c, 0x00000080);
	ahci_set_reg(0x300f2850, 0x000000f0);
	ahci_set_reg(0x300f2854, 0x000000d0);
	ahci_set_reg(0x300f2858, 0x00000000);
	ahci_set_reg(0x300f285c, 0x00000000);
	ahci_set_reg(0x300f28c0, 0x00000000);
	ahci_set_reg(0x300f28c4, 0x00000000);
	ahci_set_reg(0x300f28c8, 0x00000000);
	ahci_set_reg(0x300f28cc, 0x00000000);
	ahci_set_reg(0x300f28d0, 0x00000000);
	ahci_set_reg(0x300f28d4, 0x00000000);
	ahci_set_reg(0x300f28d8, 0x00000000);
	ahci_set_reg(0x300f28dc, 0x00000000);
	ahci_set_reg(0x300f28e0, 0x00000000);
	ahci_set_reg(0x300f28e4, 0x000000a0);
	ahci_set_reg(0x300f28e8, 0x000000a0);
	ahci_set_reg(0x300f28ec, 0x000000a0);
	ahci_set_reg(0x300f28f0, 0x000000a0);
	ahci_set_reg(0x300f28f4, 0x000000a0);
	ahci_set_reg(0x300f28f8, 0x000000a0);
	ahci_set_reg(0x300f28fc, 0x000000a0);
	ahci_set_reg(0x300f2900, 0x00000054);
	ahci_set_reg(0x300f2904, 0x00000000);
	ahci_set_reg(0x300f2908, 0x00000080);
	ahci_set_reg(0x300f290c, 0x00000058);
	ahci_set_reg(0x300f2910, 0x00000000);
	ahci_set_reg(0x300f2914, 0x00000044);
	ahci_set_reg(0x300f2918, 0x0000005c);
	ahci_set_reg(0x300f291c, 0x00000086);
	ahci_set_reg(0x300f2920, 0x0000008d);
	ahci_set_reg(0x300f2924, 0x000000d0);
	ahci_set_reg(0x300f2928, 0x00000009);
	ahci_set_reg(0x300f292c, 0x00000090);
	ahci_set_reg(0x300f2930, 0x00000007);
	ahci_set_reg(0x300f2934, 0x00000040);
	ahci_set_reg(0x300f2938, 0x00000000);
	ahci_set_reg(0x300f293c, 0x00000000);
	ahci_set_reg(0x300f2940, 0x00000000);
	ahci_set_reg(0x300f2944, 0x00000020);
	ahci_set_reg(0x300f2948, 0x00000032);
	ahci_set_reg(0x300f294c, 0x00000000);
	ahci_set_reg(0x300f2950, 0x00000000);
	ahci_set_reg(0x300f2954, 0x00000000);
	ahci_set_reg(0x300f2958, 0x00000000);
	ahci_set_reg(0x300f295c, 0x00000000);
	ahci_set_reg(0x300f2960, 0x00000000);
	ahci_set_reg(0x300f2964, 0x00000000);
	ahci_set_reg(0x300f2968, 0x00000000);
	ahci_set_reg(0x300f296c, 0x00000000);
	ahci_set_reg(0x300f2970, 0x00000000);
	ahci_set_reg(0x300f2974, 0x00000000);
	ahci_set_reg(0x300f2978, 0x00000000);
	ahci_set_reg(0x300f297c, 0x00000000);
	ahci_set_reg(0x300f2980, 0x00000000);
	ahci_set_reg(0x300f2984, 0x00000000);
	ahci_set_reg(0x300f2988, 0x00000000);
	ahci_set_reg(0x300f298c, 0x00000000);
	ahci_set_reg(0x300f2990, 0x00000000);
	ahci_set_reg(0x300f2994, 0x00000000);
	ahci_set_reg(0x300f2998, 0x00000000);
	ahci_set_reg(0x300f299c, 0x00000000);
	ahci_set_reg(0x300f29a0, 0x00000000);
	ahci_set_reg(0x300f29a4, 0x00000000);
	ahci_set_reg(0x300f29a8, 0x00000000);
	ahci_set_reg(0x300f29ac, 0x00000000);
	ahci_set_reg(0x300f29b0, 0x00000000);
	ahci_set_reg(0x300f29b4, 0x00000000);
	ahci_set_reg(0x300f29b8, 0x00000000);
	ahci_set_reg(0x300f29bc, 0x00000000);
	ahci_set_reg(0x300f29c0, 0x00000000);
	ahci_set_reg(0x300f29c4, 0x00000000);
	ahci_set_reg(0x300f29c8, 0x00000000);
	ahci_set_reg(0x300f29cc, 0x00000000);
	ahci_set_reg(0x300f29d0, 0x00000000);
	ahci_set_reg(0x300f29d4, 0x00000000);
	ahci_set_reg(0x300f29d8, 0x00000000);
	ahci_set_reg(0x300f29dc, 0x00000000);
	ahci_set_reg(0x300f29e0, 0x00000000);
	ahci_set_reg(0x300f29e4, 0x00000000);
	ahci_set_reg(0x300f29e8, 0x00000000);
	ahci_set_reg(0x300f29ec, 0x00000000);
	ahci_set_reg(0x300f29f0, 0x00000000);
	ahci_set_reg(0x300f29f4, 0x00000000);
	ahci_set_reg(0x300f29f8, 0x00000000);
	ahci_set_reg(0x300f29fc, 0x000000d8);
	ahci_set_reg(0x300f2a00, 0x0000001a);
	ahci_set_reg(0x300f2a04, 0x000000ff);
	ahci_set_reg(0x300f2a08, 0x00000011);
	ahci_set_reg(0x300f2a0c, 0x00000000);
	ahci_set_reg(0x300f2a10, 0x00000000);
	ahci_set_reg(0x300f2a14, 0x00000000);
	ahci_set_reg(0x300f2a18, 0x00000000);
	ahci_set_reg(0x300f2a1c, 0x000000f0);
	ahci_set_reg(0x300f2a20, 0x000000ff);
	ahci_set_reg(0x300f2a24, 0x000000ff);
	ahci_set_reg(0x300f2a28, 0x000000ff);
	ahci_set_reg(0x300f2a2c, 0x000000ff);
	ahci_set_reg(0x300f2a30, 0x0000001c);
	ahci_set_reg(0x300f2a34, 0x000000c2);
	ahci_set_reg(0x300f2a38, 0x000000c3);
	ahci_set_reg(0x300f2a3c, 0x0000003f);
	ahci_set_reg(0x300f2a40, 0x0000000a);
	ahci_set_reg(0x300f2a44, 0x00000000);
	ahci_set_reg(0x300f2a48, 0x00000000);
	ahci_set_reg(0x300f2a4c, 0x00000000);
	ahci_set_reg(0x300f2a50, 0x00000000);
	ahci_set_reg(0x300f2a54, 0x00000000);
	ahci_set_reg(0x300f2a58, 0x000000f8);
	ahci_set_reg(0x300f0000, 0x00000007);


	// PHYCR[25] = 1 release CMU reset
	regval=ahci_get_reg(0x30010178)|0x02000000;
	ahci_set_reg(0x30010178, regval);

	// Wait PHYSR[0] CMU_OK (it takes about 300us)
//	regval=ahci_get_reg(0x3001008c)&0x00000001;
	regval=ahci_get_reg(0x3001017c)&0x00000001;

	while(regval!=0x00000001)
	{
		regval=ahci_get_reg(0x3001017c)&0x00000001;
	}

	ahci_set_reg(0x300f2800, 0x00000000);
	ahci_set_reg(0x300f2800, 0x00000002);

	// PHYCR[27] = 1 release LANE0 reset
	regval=ahci_get_reg(0x30010178)|0x08000000;
	ahci_set_reg(0x30010178, regval);

	// Wait PHYSR[13] LN0_OK (it takes about 388us)
	regval=ahci_get_reg(0x3001017c)&0x00002000;
	while(regval!=0x00002000)
	{
		regval=ahci_get_reg(0x3001017c)&0x00002000;
	}


// 2010.08.30 - jcshin
/*
	cnt = SATA_PHY_INIT_RETRY_CNT;
	regval = 0;
	while(regval!=0x00000003 && cnt--)
	{
		i = SATA_PHY_INIT_WAIT_CNT;
		while(i--);
		
		regval=ahci_get_reg(0x30010024)&0x0000000f;

		ahci_set_reg(0x3001002c, 0x301);
		i = 100;
		while(i--);
		ahci_set_reg(0x3001002c, 0x300);
	}
*/


}



static void ahci_sata_phy1_init(void)
{
	unsigned int regval;
	int i;
	
//	int cnt;
//	unsigned int p, m, s, k, ahbclk;
//	unsigned int cwMin, cwMax, ciMin, ciMax;
	
	// Added this line to make module
	regval=ahci_get_reg(0x300908c0)&~0x00002000; // SW_RESET0[22]: SATA1 Phy Reset 
	ahci_set_reg(0x300908c0, regval);

	// release HSATA 1 reset
	//regval=ahci_get_reg(0x300908c0)|0x08000000; // 2010.08.30 - jcshin
	regval=ahci_get_reg(0x300908c0)|0x04000000; // SW_RESET0[22]: SATA1 Controller Reset 
	ahci_set_reg(0x300908c0, regval);

	// select SATA1/PCIe  1=SATA1, 0=PCIe(default)
	regval=ahci_get_reg(0x300d000c)|0x00000001;
	ahci_set_reg(0x300d000c, regval);


// 2010.08.30 - jcshin 
/*
	regval = ahci_get_reg(0x30090808);
	p = PMU_PLL_P_VALUE(regval);
	m = PMU_PLL_M_VALUE(regval);
	s = PMU_PLL_S_VALUE(regval);
	k = ahci_get_reg(0x3009081C)&0xffff;

	ahbclk = (((m+k/1024) * ((24000000 >> s) / p)) >> 2) / 1000;

	cwMin = CWMIN_DFT * ahbclk / ABHCLK_DFT;
	cwMax = CWMAX_DFT * ahbclk / ABHCLK_DFT;
	ciMin = CIMIN_DFT * ahbclk / ABHCLK_DFT;	
	ciMax = CIMAX_DFT * ahbclk / ABHCLK_DFT;

	// OOBR - adjust values according to the Bus(SMX) clock
	regval=ahci_get_reg(0x300180bc)|0x80000000;
	ahci_set_reg(0x300180bc, regval);
	//regval=0x89111e35;
	//regval = 0x870d182a;
	regval = 0x80000000 | (cwMin << 24) | (cwMax << 16) | (ciMin << 8) | (ciMax << 0);	
	ahci_set_reg(0x300180bc, regval);
	regval=ahci_get_reg(0x300180bc)&0x7fffffff;
	ahci_set_reg(0x300180bc, regval);
*/

	// RXERR override (PHYCR[9] should be 1)
	regval=ahci_get_reg(0x30018178)&0xfffffdff;
	regval=regval|0x00000200;
	ahci_set_reg(0x30018178, regval);

	// release SATA 1 PHY reset
	//regval=ahci_get_reg(0x300908c0)|0x00004000; // 2010.08.30 - jcshin
	regval=ahci_get_reg(0x300908c0)|0x00002000; // SW_RESET0[13]: SATA1 PHY Reset
	ahci_set_reg(0x300908c0, regval);

	// may need time for resetting PHY
	for( i = 0; i < 100; i++ );

	// PHYCR[14] = 1 release CMU PowerDown Exit
	regval=ahci_get_reg(0x30018178)|0x00004000;
	ahci_set_reg(0x30018178, regval);
	for( i = 0; i < 100; i++ );

	// CMU
	ahci_set_reg(0x300f8000, 0x00000006);
	ahci_set_reg(0x300f8004, 0x00000000);
	ahci_set_reg(0x300f8008, 0x00000088);
	ahci_set_reg(0x300f800c, 0x00000000);
	ahci_set_reg(0x300f8010, 0x0000007b);
	ahci_set_reg(0x300f8014, 0x000000c9);
	ahci_set_reg(0x300f8018, 0x00000003);
	ahci_set_reg(0x300f801c, 0x00000000);
	ahci_set_reg(0x300f8020, 0x00000000);
	ahci_set_reg(0x300f8024, 0x00000000);
	ahci_set_reg(0x300f8028, 0x00000000);
	ahci_set_reg(0x300f802c, 0x00000000);
	ahci_set_reg(0x300f8030, 0x00000000);
	ahci_set_reg(0x300f8034, 0x00000000);
	ahci_set_reg(0x300f8038, 0x00000000);
	ahci_set_reg(0x300f803c, 0x00000000);
	ahci_set_reg(0x300f8040, 0x00000000);
	ahci_set_reg(0x300f8044, 0x00000000);
	ahci_set_reg(0x300f8048, 0x00000000);
	ahci_set_reg(0x300f804c, 0x00000000);
	ahci_set_reg(0x300f8050, 0x00000000);
	ahci_set_reg(0x300f8054, 0x00000000);
	ahci_set_reg(0x300f8058, 0x00000000);
	ahci_set_reg(0x300f805c, 0x00000000);
	ahci_set_reg(0x300f8060, 0x00000000);
	ahci_set_reg(0x300f8064, 0x00000000);
	ahci_set_reg(0x300f8068, 0x00000000);
	ahci_set_reg(0x300f806c, 0x00000000);
	ahci_set_reg(0x300f8070, 0x00000000);
	ahci_set_reg(0x300f8074, 0x00000000);
	ahci_set_reg(0x300f8078, 0x00000000);
	ahci_set_reg(0x300f807c, 0x00000000);
	ahci_set_reg(0x300f8080, 0x00000000);
	ahci_set_reg(0x300f8084, 0x00000000);
	ahci_set_reg(0x300f8088, 0x000000a0);
	ahci_set_reg(0x300f808c, 0x00000054);
	ahci_set_reg(0x300f8090, 0x00000000);
	ahci_set_reg(0x300f8094, 0x00000000);
	ahci_set_reg(0x300f8098, 0x00000000);
	ahci_set_reg(0x300f809c, 0x00000000);
	ahci_set_reg(0x300f80a0, 0x00000000);
	ahci_set_reg(0x300f80a4, 0x00000000);
	ahci_set_reg(0x300f80a8, 0x00000000);
	ahci_set_reg(0x300f80ac, 0x00000000);
	ahci_set_reg(0x300f80b0, 0x00000000);
	ahci_set_reg(0x300f80b4, 0x00000000);
	ahci_set_reg(0x300f80b8, 0x00000004);
	ahci_set_reg(0x300f80bc, 0x00000050);
	ahci_set_reg(0x300f80c0, 0x00000070);
	ahci_set_reg(0x300f80c4, 0x00000002);
	ahci_set_reg(0x300f80c8, 0x00000025);
	ahci_set_reg(0x300f80cc, 0x00000040);
	ahci_set_reg(0x300f80d0, 0x00000001);
	ahci_set_reg(0x300f80d4, 0x00000040);
	ahci_set_reg(0x300f80d8, 0x00000000);
	ahci_set_reg(0x300f80dc, 0x00000000);
	ahci_set_reg(0x300f80e0, 0x00000000);
	ahci_set_reg(0x300f80e4, 0x00000000);
	ahci_set_reg(0x300f80e8, 0x00000000);
	ahci_set_reg(0x300f80ec, 0x00000000);
	ahci_set_reg(0x300f80f0, 0x00000000);
	ahci_set_reg(0x300f80f4, 0x00000000);
	ahci_set_reg(0x300f80f8, 0x00000000);
	ahci_set_reg(0x300f80fc, 0x00000000);
	ahci_set_reg(0x300f8100, 0x00000000);
	ahci_set_reg(0x300f8104, 0x00000000);
	ahci_set_reg(0x300f8108, 0x00000000);
	ahci_set_reg(0x300f810c, 0x00000000);
	ahci_set_reg(0x300f8110, 0x00000000);
	ahci_set_reg(0x300f8114, 0x00000000);
	ahci_set_reg(0x300f8118, 0x00000000);
	ahci_set_reg(0x300f811c, 0x00000000);
	ahci_set_reg(0x300f8120, 0x00000000);
	ahci_set_reg(0x300f8124, 0x00000000);
	ahci_set_reg(0x300f8128, 0x00000000);
	ahci_set_reg(0x300f812c, 0x00000000);
	ahci_set_reg(0x300f8130, 0x00000000);
	ahci_set_reg(0x300f8134, 0x00000000);
	ahci_set_reg(0x300f8138, 0x00000000);
	ahci_set_reg(0x300f813c, 0x00000000);
	ahci_set_reg(0x300f8140, 0x00000000);
	ahci_set_reg(0x300f8144, 0x00000000);
	ahci_set_reg(0x300f8148, 0x00000000);
	ahci_set_reg(0x300f814c, 0x00000000);
	ahci_set_reg(0x300f8150, 0x00000000);
	ahci_set_reg(0x300f8154, 0x00000000);
	ahci_set_reg(0x300f8158, 0x00000000);
	ahci_set_reg(0x300f815c, 0x00000000);
	ahci_set_reg(0x300f8160, 0x00000000);
	ahci_set_reg(0x300f8164, 0x00000000);
	ahci_set_reg(0x300f8168, 0x00000000);
	ahci_set_reg(0x300f816c, 0x00000000);
	ahci_set_reg(0x300f8170, 0x00000000);
	ahci_set_reg(0x300f8174, 0x00000000);
	ahci_set_reg(0x300f8178, 0x00000000);
	ahci_set_reg(0x300f817c, 0x00000000);
	ahci_set_reg(0x300f8180, 0x00000000);
	ahci_set_reg(0x300f8184, 0x0000002e);
	ahci_set_reg(0x300f8188, 0x00000000);
	ahci_set_reg(0x300f818c, 0x0000005e);
	ahci_set_reg(0x300f8190, 0x00000000);
	ahci_set_reg(0x300f8194, 0x00000042);
	ahci_set_reg(0x300f8198, 0x000000d1);
	ahci_set_reg(0x300f819c, 0x00000020);
	ahci_set_reg(0x300f81a0, 0x00000028);
	ahci_set_reg(0x300f81a4, 0x00000078);
	ahci_set_reg(0x300f81a8, 0x0000002c);
	ahci_set_reg(0x300f81ac, 0x000000b9);
	ahci_set_reg(0x300f81b0, 0x0000005e);
	//ahci_set_reg(0x300f81b4, 0x00000000); //external differential reference clock input
	ahci_set_reg(0x300f81b4, 0x00000003); //internal refclk_core_i input
	ahci_set_reg(0x300f81b8, 0x00000000);
	ahci_set_reg(0x300f81bc, 0x00000000);
	ahci_set_reg(0x300f81c0, 0x00000000);
	ahci_set_reg(0x300f81c4, 0x00000000);
	ahci_set_reg(0x300f81c8, 0x00000000);

	// LANE0
	ahci_set_reg(0x300f8800, 0x00000002);
	ahci_set_reg(0x300f8804, 0x00000000);
	ahci_set_reg(0x300f8808, 0x00000000);
	ahci_set_reg(0x300f880c, 0x00000000);
	ahci_set_reg(0x300f8810, 0x00000000);
	ahci_set_reg(0x300f8814, 0x00000010);
	ahci_set_reg(0x300f8818, 0x00000084);
	ahci_set_reg(0x300f881c, 0x00000004);
	ahci_set_reg(0x300f8820, 0x000000e0);
	ahci_set_reg(0x300f8840, 0x00000023);
	ahci_set_reg(0x300f8844, 0x00000000);
	ahci_set_reg(0x300f8848, 0x00000040);
	ahci_set_reg(0x300f884c, 0x00000005);
	ahci_set_reg(0x300f8850, 0x000000d0);
	ahci_set_reg(0x300f8854, 0x00000017);
	ahci_set_reg(0x300f8858, 0x00000000);
	ahci_set_reg(0x300f885c, 0x00000068);
	ahci_set_reg(0x300f8860, 0x000000f2);
	ahci_set_reg(0x300f8864, 0x0000001e);
	ahci_set_reg(0x300f8868, 0x00000018);
	ahci_set_reg(0x300f886c, 0x0000000d);
	ahci_set_reg(0x300f8870, 0x0000000c);
	ahci_set_reg(0x300f8874, 0x00000000);
	ahci_set_reg(0x300f8878, 0x00000000);
	ahci_set_reg(0x300f887c, 0x00000000);
	ahci_set_reg(0x300f8880, 0x00000000);
	ahci_set_reg(0x300f8884, 0x00000000);
	ahci_set_reg(0x300f8888, 0x00000000);
	ahci_set_reg(0x300f888c, 0x00000000);
	ahci_set_reg(0x300f8890, 0x00000000);
	ahci_set_reg(0x300f8894, 0x00000000);
	ahci_set_reg(0x300f8898, 0x00000000);
	ahci_set_reg(0x300f889c, 0x00000000);
	ahci_set_reg(0x300f88a0, 0x00000000);
	ahci_set_reg(0x300f88a4, 0x00000000);
	ahci_set_reg(0x300f88a8, 0x00000000);
	ahci_set_reg(0x300f88ac, 0x00000000);
	ahci_set_reg(0x300f88b0, 0x00000000);
	ahci_set_reg(0x300f88b4, 0x00000000);
	ahci_set_reg(0x300f88b8, 0x00000000);
	ahci_set_reg(0x300f88bc, 0x00000000);
	ahci_set_reg(0x300f88c0, 0x00000000);
	ahci_set_reg(0x300f88c4, 0x00000000);
	ahci_set_reg(0x300f88c8, 0x00000000);
	ahci_set_reg(0x300f88cc, 0x00000000);
	ahci_set_reg(0x300f88d0, 0x00000000);
	ahci_set_reg(0x300f88d4, 0x00000000);
	ahci_set_reg(0x300f88d8, 0x00000000);
	ahci_set_reg(0x300f88dc, 0x00000000);
	ahci_set_reg(0x300f88e0, 0x00000000);
	ahci_set_reg(0x300f88e4, 0x00000000);
	ahci_set_reg(0x300f88e8, 0x00000000);
	ahci_set_reg(0x300f88ec, 0x00000000);
	ahci_set_reg(0x300f88f0, 0x00000000);
	ahci_set_reg(0x300f88f4, 0x00000000);
	ahci_set_reg(0x300f88f8, 0x00000000);
	ahci_set_reg(0x300f88fc, 0x00000000);
	ahci_set_reg(0x300f8900, 0x00000000);
	ahci_set_reg(0x300f8904, 0x00000000);
	ahci_set_reg(0x300f8908, 0x00000000);
	ahci_set_reg(0x300f890c, 0x00000000);
	ahci_set_reg(0x300f8910, 0x00000000);
	ahci_set_reg(0x300f8914, 0x00000000);
	ahci_set_reg(0x300f8918, 0x00000000);
	ahci_set_reg(0x300f891c, 0x00000000);
	ahci_set_reg(0x300f8920, 0x00000000);
	ahci_set_reg(0x300f8924, 0x00000000);
	ahci_set_reg(0x300f8928, 0x00000000);
	ahci_set_reg(0x300f892c, 0x00000000);
	ahci_set_reg(0x300f8930, 0x00000000);
	ahci_set_reg(0x300f8934, 0x00000000);
	ahci_set_reg(0x300f8938, 0x00000000);
	ahci_set_reg(0x300f893c, 0x00000000);
	ahci_set_reg(0x300f8940, 0x00000060);
	ahci_set_reg(0x300f8944, 0x0000000f);

	// COMLANE
	ahci_set_reg(0x300fa800, 0x00000000);
	ahci_set_reg(0x300fa804, 0x00000020);
	ahci_set_reg(0x300fa808, 0x00000000);
	ahci_set_reg(0x300fa80c, 0x00000040);
	ahci_set_reg(0x300fa810, 0x00000024);
	ahci_set_reg(0x300fa814, 0x000000ae);
	ahci_set_reg(0x300fa818, 0x00000019);
	ahci_set_reg(0x300fa81c, 0x00000049);
	ahci_set_reg(0x300fa820, 0x00000004);
	ahci_set_reg(0x300fa824, 0x00000083);
	ahci_set_reg(0x300fa828, 0x0000004b);
	ahci_set_reg(0x300fa82c, 0x000000c5);
	ahci_set_reg(0x300fa830, 0x00000001);
	ahci_set_reg(0x300fa834, 0x00000003);
	ahci_set_reg(0x300fa838, 0x00000028);
	ahci_set_reg(0x300fa83c, 0x00000098);
	ahci_set_reg(0x300fa840, 0x00000019);
	ahci_set_reg(0x300fa844, 0x00000000);
	ahci_set_reg(0x300fa848, 0x00000000);
	ahci_set_reg(0x300fa84c, 0x00000080);
	ahci_set_reg(0x300fa850, 0x000000f0);
	ahci_set_reg(0x300fa854, 0x000000d0);
	ahci_set_reg(0x300fa858, 0x00000000);
	ahci_set_reg(0x300fa85c, 0x00000000);
	ahci_set_reg(0x300fa8c0, 0x00000000);
	ahci_set_reg(0x300fa8c4, 0x00000000);
	ahci_set_reg(0x300fa8c8, 0x00000000);
	ahci_set_reg(0x300fa8cc, 0x00000000);
	ahci_set_reg(0x300fa8d0, 0x00000000);
	ahci_set_reg(0x300fa8d4, 0x00000000);
	ahci_set_reg(0x300fa8d8, 0x00000000);
	ahci_set_reg(0x300fa8dc, 0x00000000);
	ahci_set_reg(0x300fa8e0, 0x00000000);
	ahci_set_reg(0x300fa8e4, 0x000000a0);
	ahci_set_reg(0x300fa8e8, 0x000000a0);
	ahci_set_reg(0x300fa8ec, 0x000000a0);
	ahci_set_reg(0x300fa8f0, 0x000000a0);
	ahci_set_reg(0x300fa8f4, 0x000000a0);
	ahci_set_reg(0x300fa8f8, 0x000000a0);
	ahci_set_reg(0x300fa8fc, 0x000000a0);
	ahci_set_reg(0x300fa900, 0x00000054);
	ahci_set_reg(0x300fa904, 0x00000000);
	ahci_set_reg(0x300fa908, 0x00000080);
	ahci_set_reg(0x300fa90c, 0x00000058);
	ahci_set_reg(0x300fa910, 0x00000000);
	ahci_set_reg(0x300fa914, 0x00000044);
	ahci_set_reg(0x300fa918, 0x0000005c);
	ahci_set_reg(0x300fa91c, 0x00000086);
	ahci_set_reg(0x300fa920, 0x0000008d);
	ahci_set_reg(0x300fa924, 0x000000d0);
	ahci_set_reg(0x300fa928, 0x00000009);
	ahci_set_reg(0x300fa92c, 0x00000090);
	ahci_set_reg(0x300fa930, 0x00000007);
	ahci_set_reg(0x300fa934, 0x00000040);
	ahci_set_reg(0x300fa938, 0x00000000);
	ahci_set_reg(0x300fa93c, 0x00000000);
	ahci_set_reg(0x300fa940, 0x00000000);
	ahci_set_reg(0x300fa944, 0x00000020);
	ahci_set_reg(0x300fa948, 0x00000032);
	ahci_set_reg(0x300fa94c, 0x00000000);
	ahci_set_reg(0x300fa950, 0x00000000);
	ahci_set_reg(0x300fa954, 0x00000000);
	ahci_set_reg(0x300fa958, 0x00000000);
	ahci_set_reg(0x300fa95c, 0x00000000);
	ahci_set_reg(0x300fa960, 0x00000000);
	ahci_set_reg(0x300fa964, 0x00000000);
	ahci_set_reg(0x300fa968, 0x00000000);
	ahci_set_reg(0x300fa96c, 0x00000000);
	ahci_set_reg(0x300fa970, 0x00000000);
	ahci_set_reg(0x300fa974, 0x00000000);
	ahci_set_reg(0x300fa978, 0x00000000);
	ahci_set_reg(0x300fa97c, 0x00000000);
	ahci_set_reg(0x300fa980, 0x00000000);
	ahci_set_reg(0x300fa984, 0x00000000);
	ahci_set_reg(0x300fa988, 0x00000000);
	ahci_set_reg(0x300fa98c, 0x00000000);
	ahci_set_reg(0x300fa990, 0x00000000);
	ahci_set_reg(0x300fa994, 0x00000000);
	ahci_set_reg(0x300fa998, 0x00000000);
	ahci_set_reg(0x300fa99c, 0x00000000);
	ahci_set_reg(0x300fa9a0, 0x00000000);
	ahci_set_reg(0x300fa9a4, 0x00000000);
	ahci_set_reg(0x300fa9a8, 0x00000000);
	ahci_set_reg(0x300fa9ac, 0x00000000);
	ahci_set_reg(0x300fa9b0, 0x00000000);
	ahci_set_reg(0x300fa9b4, 0x00000000);
	ahci_set_reg(0x300fa9b8, 0x00000000);
	ahci_set_reg(0x300fa9bc, 0x00000000);
	ahci_set_reg(0x300fa9c0, 0x00000000);
	ahci_set_reg(0x300fa9c4, 0x00000000);
	ahci_set_reg(0x300fa9c8, 0x00000000);
	ahci_set_reg(0x300fa9cc, 0x00000000);
	ahci_set_reg(0x300fa9d0, 0x00000000);
	ahci_set_reg(0x300fa9d4, 0x00000000);
	ahci_set_reg(0x300fa9d8, 0x00000000);
	ahci_set_reg(0x300fa9dc, 0x00000000);
	ahci_set_reg(0x300fa9e0, 0x00000000);
	ahci_set_reg(0x300fa9e4, 0x00000000);
	ahci_set_reg(0x300fa9e8, 0x00000000);
	ahci_set_reg(0x300fa9ec, 0x00000000);
	ahci_set_reg(0x300fa9f0, 0x00000000);
	ahci_set_reg(0x300fa9f4, 0x00000000);
	ahci_set_reg(0x300fa9f8, 0x00000000);
	ahci_set_reg(0x300fa9fc, 0x000000d8);
	ahci_set_reg(0x300faa00, 0x0000001a);
	ahci_set_reg(0x300faa04, 0x000000ff);
	ahci_set_reg(0x300faa08, 0x00000011);
	ahci_set_reg(0x300faa0c, 0x00000000);
	ahci_set_reg(0x300faa10, 0x00000000);
	ahci_set_reg(0x300faa14, 0x00000000);
	ahci_set_reg(0x300faa18, 0x00000000);
	ahci_set_reg(0x300faa1c, 0x000000f0);
	ahci_set_reg(0x300faa20, 0x000000ff);
	ahci_set_reg(0x300faa24, 0x000000ff);
	ahci_set_reg(0x300faa28, 0x000000ff);
	ahci_set_reg(0x300faa2c, 0x000000ff);
	ahci_set_reg(0x300faa30, 0x0000001c);
	ahci_set_reg(0x300faa34, 0x000000c2);
	ahci_set_reg(0x300faa38, 0x000000c3);
	ahci_set_reg(0x300faa3c, 0x0000003f);
	ahci_set_reg(0x300faa40, 0x0000000a);
	ahci_set_reg(0x300faa44, 0x00000000);
	ahci_set_reg(0x300faa48, 0x00000000);
	ahci_set_reg(0x300faa4c, 0x00000000);
	ahci_set_reg(0x300faa50, 0x00000000);
	ahci_set_reg(0x300faa54, 0x00000000);
	ahci_set_reg(0x300faa58, 0x000000f8);
	ahci_set_reg(0x300f8000, 0x00000007);


	// PHYCR[25] = 1 release CMU reset
	regval=ahci_get_reg(0x30018178)|0x02000000;
	ahci_set_reg(0x30018178, regval);

	// Wait PHYSR[0] CMU_OK (it takes about 300us)
	regval=ahci_get_reg(0x3001817c)&0x00000001;
	while(regval!=0x00000001)
	{
		regval=ahci_get_reg(0x3001817c)&0x00000001;
	}


	ahci_set_reg(0x300fa800, 0x00000000);
	ahci_set_reg(0x300fa800, 0x00000002);


	// PHYCR[27] = 1 release LANE0 reset
	regval=ahci_get_reg(0x30018178)|0x08000000;
	ahci_set_reg(0x30018178, regval);

	// Wait PHYSR[13] LN0_OK (it takes about 388us)
	regval=ahci_get_reg(0x3001817c)&0x00002000;
	while(regval!=0x00002000)
	{
		regval=ahci_get_reg(0x3001817c)&0x00002000;
	}


// 2010.08.30 - jcshin
/*
	cnt = SATA_PHY_INIT_RETRY_CNT;
	regval = 0;
	while(regval!=0x00000003 && cnt--)
	{
		i = SATA_PHY_INIT_WAIT_CNT;
		while(i--);
		
		regval=ahci_get_reg(0x30018024)&0x0000000f;

		ahci_set_reg(0x3001802c, 0x301);
		i = 100;
		while(i--);
		ahci_set_reg(0x3001802c, 0x300);
  }
*/
}

static void ahci_set_phy(void)
{
#if defined (ES_REAL_TARGET)
	ahci_sata_phy0_init();
	ahci_sata_phy1_init();
#else
	u32 regval;
	
	// enable sata slave access (only in FPGA)
	ahci_set_reg( 0x300d0008, 0x2);
	
	// select SATA1/PCIe  1=SATA1, 0=PCIe(default)
	regval=ahci_get_reg(0x300d000c)|0x00000001;
	ahci_set_reg( 0x300d000c, regval);
	
	// only for FPGA
	// sig-level valid sampling with rbc
	ahci_set_reg( 0x30010178, 0x00000100);
	
	// OOBR
	ahci_set_reg( 0x300100bc, 0x80000000);
	ahci_set_reg( 0x300100bc, 0x860a121f);

	// PHY Configuration - no need for FPGA
	// de-assert PHY reset
	// SATA0_PHYCR[27] = LN0_RSTN
	regval=ahci_get_reg(0x30010178)|0x08000000;
	ahci_set_reg( 0x30010178, regval);
#endif
}

static int __init ahci_init(void)
{
	int i;

	ahci_set_phy();
	
	for ( i = 0 ; i < MAX_AHCI_DEV_NUM; i++)
	{
		platform_driver_register(&ahci_platform_driver[i]);
	}

	return 0;

}

static void __exit ahci_exit(void)
{
	int i;

	for ( i = 0; i < MAX_AHCI_DEV_NUM; i++)
	{
		platform_driver_unregister(&ahci_platform_driver[i]);
	}

}


MODULE_AUTHOR("Jeff Garzik");
MODULE_DESCRIPTION("AHCI SATA low-level driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);

module_init(ahci_init);
module_exit(ahci_exit);

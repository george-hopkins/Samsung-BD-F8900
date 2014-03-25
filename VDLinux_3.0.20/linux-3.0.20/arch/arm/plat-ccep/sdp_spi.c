/*
 * Driver for Cirrus Logic EP93xx SPI controller.
 *
 * Copyright (c) 2010 Mika Westerberg
 *
 * Explicit FIFO handling code was inspired by amba-pl022 driver.
 *
 * Chip select support using other than built-in GPIOs by H. Hartley Sweeten.
 *
 * For more information about the SPI controller see documentation on Cirrus
 * Logic web site:
 *     http://www.cirrus.com/en/pubs/manual/EP93xx_Users_Guide_UM1.pdf
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * Modified for SDP SPI Master Controller.
 * by drain.lee.

 * 20110308_ 16bit write/read bug fix
 * 20110324_  if bpw>8, odd size data length error handling
 * 20110614_ porting for kernel 2.6.35.11 by drain.lee
 * 20120221_ add mach specific inin func.
 * 20120221_ remove ifdef MODULE for clk lib.
 * 20120312_ porting for kernel-3.0.20 and fix debug macro
 * 20120312_ add speed chack
 * 20121227_ add default chip select ops.
 * 20121227_ add max clock limit.
 * 20130101_ fix prevent defect(Uninitialized scalar variable).
 */

/* debug macro */
#ifdef CONFIG_SDP_SPI_DEBUG
#warning CONFIG_SDP_SPI_DEBUG Enabled!!!!
#define DEBUG
#else
#undef DEBUG
#endif

#include <linux/module.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/bitops.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/spi/spi.h>

#include <plat/sdp_spi.h>
#include <plat/sdp_spi_regs.h>

#if 0
#define TRACE(fmt, ...)	printk(fmt, ##__VA_ARGS__)
#undef dev_dbg
#define dev_dbg(dev, format, arg...)		\
	dev_printk(KERN_CRIT, dev, format, ##arg)
#else
#define TRACE(fmt, ...)
#endif

/* timeout in milliseconds */
#define SPI_TIMEOUT		5
/* maximum depth of RX/TX FIFO */
#define SPI_FIFO_SIZE		8

///////////////////////////////
/**
 * struct sdp_spi - sdp SPI controller structure
 * @lock: spinlock that protects concurrent accesses to fields @running,
 *        @current_msg and @msg_queue
 * @pdev: pointer to platform device
 * @clk: clock for the controller
 * @ext_clkrate: operational clock rate (HZ) if clock is not provided
 * @regs_base: pointer to ioremap()'d registers
 * @irq: IRQ number used by the driver
 * @min_rate: minimum clock rate (in Hz) supported by the controller
 * @max_rate: maximum clock rate (in Hz) supported by the controller
 * @running: is the queue running
 * @wq: workqueue used by the driver
 * @msg_work: work that is queued for the driver
 * @wait: wait here until given transfer is completed
 * @msg_queue: queue for the messages
 * @current_msg: message that is currently processed (or %NULL if none)
 * @tx: current byte in transfer to transmit
 * @rx: current byte in transfer to receive
 * @fifo_level: how full is FIFO (%0..%SPI_FIFO_SIZE - %1). Receiving one
 *              frame decreases this level and sending one frame increases it.
 *
 * This structure holds sdp SPI controller specific information. When
 * @running is %true, driver accepts transfer requests from protocol drivers.
 * @current_msg is used to hold pointer to the message that is currently
 * processed. If @current_msg is %NULL, it means that no processing is going
 * on.
 *
 * Most of the fields are only written once and they can be accessed without
 * taking the @lock. Fields that are accessed concurrently are: @current_msg,
 * @running, and @msg_queue.
 */
struct sdp_spi {
	spinlock_t					lock;
	const struct platform_device	*pdev;
	void __iomem					*regs_base;
	struct clk					*clk;
	unsigned long 					ext_clkrate;
	int							irq;
	unsigned long					min_rate;
	unsigned long					max_rate;
	bool							running;
	struct workqueue_struct		*wq;
	struct work_struct				msg_work;
	struct completion				wait;
	struct list_head				msg_queue;
	struct spi_message			*current_msg;
	size_t						tx;
	size_t						rx;
	size_t						fifo_level;
};

/**
 * struct sdp_spi_chip - SPI device hardware settings
 * @spi: back pointer to the SPI device
 * @rate: max rate in hz this chip supports
 * @div_cpsr: cpsr (pre-scaler) divider
 * @div_scr: scr divider
 * @dss: bits per word (4 - 16 bits)
 * @ops: private chip operations
 *
 * This structure is used to store hardware register specific settings for each
 * SPI device. Settings are written to hardware by function
 * sdp_spi_chip_setup().
 */

enum sdp_spi_frame_format {
	MOTOROLA_SPI = 0x0,
	TI_SYNC_SERIAL = 0x1,
	NATIONAL_MICRO_WIRE = 0x2,
};

struct sdp_spi_chip {
	const struct spi_device		*spi;
	unsigned long			rate;
	u8				div_cpsr;
	u8				div_scr;
	u8				dss;
	//add drain.lee loopback
	enum sdp_spi_frame_format	frame_format;
	struct sdp_spi_chip_ops	*ops;
};

/* converts bits per word to CR0.DSS value */
#define bits_per_word_to_dss(bpw)	((bpw) - 1)

static irqreturn_t
sdp_spi_interrupt(int irq, void *dev_id);

/*
 * Bagic OPs
*/
#if !defined(CONFIG_FOXAP_SMC_SPI)
static inline void
sdp_spi_write_u8(const struct sdp_spi *sdpspi, u16 reg, u8 value)
{
	__raw_writeb(value, sdpspi->regs_base + reg);
}

static inline u8
sdp_spi_read_u8(const struct sdp_spi *spi, u16 reg)
{
	return __raw_readb(spi->regs_base + reg);
}

static inline void
sdp_spi_write_u16(const struct sdp_spi *sdpspi, u16 reg, u16 value)
{
	__raw_writew(value, sdpspi->regs_base + reg);
}

static inline u16
sdp_spi_read_u16(const struct sdp_spi *spi, u16 reg)
{
	return __raw_readw(spi->regs_base + reg);
}
#else
#define SLUGGISH_BUSNUM		(2)
#define SLUGGISH_BUS_RDELAY	(1)
#define SLUGGISH_BUS_WDELAY	(1)
static inline void sdp_spi_bus_delay_read(const struct sdp_spi *spi)
{
	if (spi->pdev && spi->pdev->id != SLUGGISH_BUSNUM)
		return;
	if (SLUGGISH_BUS_RDELAY)
		udelay(SLUGGISH_BUS_RDELAY);
}
static inline void sdp_spi_bus_delay_write(const struct sdp_spi *spi)
{
	if (spi->pdev && spi->pdev->id != SLUGGISH_BUSNUM)
		return;
	if (SLUGGISH_BUS_WDELAY)
		udelay(SLUGGISH_BUS_WDELAY);
}
/* quick and dirty 32 bit wrapper, only for LE
 * This is temporary Do not use these on 8bit-accessible bus */
static inline void
sdp_spi_write_u8(const struct sdp_spi *spi, u16 reg, u8 value)
{
	u32 tmp;
	BUG_ON(reg & 0x3);

	if (reg != SSPDR) {
		/* only DR has an effect on read */
		tmp = readl_relaxed(spi->regs_base + reg);
		tmp &= 0xffffff00;
		tmp |= value;
	} else
		tmp = value;
	TRACE("writeu8 %p := %x\n", spi->regs_base + reg, tmp);
	writel(tmp, spi->regs_base + reg);
	sdp_spi_bus_delay_write(spi);
}

static inline u8
sdp_spi_read_u8(const struct sdp_spi *spi, u16 reg)
{
	u32 tmp;
	BUG_ON(reg & 0x3);
	sdp_spi_bus_delay_read(spi);
	tmp = readl_relaxed(spi->regs_base + reg);
	TRACE ("readu8 %p = %x\n", spi->regs_base + reg, (u8)tmp);
	return (u8)tmp;
}

static inline void
sdp_spi_write_u16(const struct sdp_spi *spi, u16 reg, u16 value)
{
	u32 tmp;
	BUG_ON(reg & 0x3);

	if (reg != SSPDR) {
		/* only DR has an effect on read */
		tmp = readl_relaxed(spi->regs_base + reg);
		tmp &= 0xffff0000;
		tmp |= value;
	} else
		tmp = value;
	TRACE("writeu16 %p := %x\n", spi->regs_base + reg, tmp);
	writel(tmp, spi->regs_base + reg);
	sdp_spi_bus_delay_write(spi);
}

static inline u16
sdp_spi_read_u16(const struct sdp_spi *spi, u16 reg)
{
	u32 tmp;
	BUG_ON(reg & 0x3);
	sdp_spi_bus_delay_read(spi);
	tmp = readl_relaxed(spi->regs_base + reg);
	TRACE ("readu16 %p = %x\n", spi->regs_base + reg, (u16)tmp);
	return (u16)tmp;
}
#endif

/*
 * enable/disable
*/
static int
sdp_spi_enable(const struct sdp_spi *sdpspi)
{
	u8 regval;
	int err;

	if (sdpspi->clk) {
		err = clk_enable(sdpspi->clk);
		if (err)
			return err;
	}

	regval = sdp_spi_read_u8(sdpspi, SSPCR1);
	regval |= SSPCR1_SSE;
	sdp_spi_write_u8(sdpspi, SSPCR1, regval);

	return 0;
}

static void
sdp_spi_disable(const struct sdp_spi *sdpspi)
{
	u8 regval;

	regval = sdp_spi_read_u8(sdpspi, SSPCR1);
	regval &= ~SSPCR1_SSE;
	sdp_spi_write_u8(sdpspi, SSPCR1, regval);

	if (sdpspi->clk)
		clk_disable(sdpspi->clk);
}

static void
sdp_spi_enable_interrupts(const struct sdp_spi *sdpspi)
{
	u8 regval;

	regval = sdp_spi_read_u8(sdpspi, SSPCR1);
	regval |= (SSPCR1_RORIE | SSPCR1_TIE | SSPCR1_RIE);
	sdp_spi_write_u8(sdpspi, SSPCR1, regval);
}

static void
sdp_spi_disable_interrupts(const struct sdp_spi *sdpspi)
{
	u8 regval;

	regval = sdp_spi_read_u8(sdpspi, SSPCR1);
	regval &= ~(SSPCR1_RORIE | SSPCR1_TIE | SSPCR1_RIE);
	sdp_spi_write_u8(sdpspi, SSPCR1, regval);
}

static unsigned long sdp_spi_clkrate(const struct sdp_spi *sdpspi)
{
	if (sdpspi->clk)
		return clk_get_rate(sdpspi->clk);
	else
		return sdpspi->ext_clkrate;
}

/**
 * sdp_spi_calc_divisors() - calculates SPI clock divisors
 * @sdpspi: sdp SPI controller struct
 * @chip: divisors are calculated for this chip
 * @rate: desired SPI output clock rate
 *
 * Function calculates cpsr (clock pre-scaler) and scr divisors based on
 * given @rate and places them to @chip->div_cpsr and @chip->div_scr. If,
 * for some reason, divisors cannot be calculated nothing is stored and
 * %-EINVAL is returned.
 */
static int
sdp_spi_calc_divisors(const struct sdp_spi *sdpspi,
				    struct sdp_spi_chip *chip,
				    unsigned long rate)
{
	unsigned long spi_clk_rate = sdp_spi_clkrate(sdpspi);
	int cpsr, scr;

	/*
	 * Make sure that max value is between values supported by the
	 * controller. Note that minimum value is already checked in
	 * sdp_spi_transfer().
	 */
	rate = clamp(rate, sdpspi->min_rate, sdpspi->max_rate);

	/*
	 * Calculate divisors so that we can get speed according the
	 * following formula:
	 *	rate = spi_clock_rate / (cpsr * (1 + scr))
	 *
	 * cpsr must be even number and starts from 2, scr can be any number
	 * between 0 and 255.
	 */
	for (cpsr = 2; cpsr <= 254; cpsr += 2) {
		for (scr = 0; scr <= 255; scr++) {
			if ((spi_clk_rate / (cpsr * (scr + 1))) <= rate) {
				chip->div_scr = (u8)scr;
				chip->div_cpsr = (u8)cpsr;
				dev_dbg(&sdpspi->pdev->dev, "Calculated real clock is %ldHz\n", spi_clk_rate / (cpsr * (scr + 1)));
				return 0;
			}
		}
	}

	return -EINVAL;
}

static void
sdp_spi_cs_control(struct spi_device *spi, bool control)
{
	struct sdp_spi_chip *chip = spi_get_ctldata(spi);
	int value = (spi->mode & SPI_CS_HIGH) ? control : !control;

	if (chip->ops && chip->ops->cs_control)
		chip->ops->cs_control(spi, value);
	//for debug drain.lee
	//dev_info(&spi->dev, "sdp_spi_cs_control is %s\n", control ? "select" : "not select");
}


/**
 * sdp_spi_setup() - setup an SPI device
 * @spi: SPI device to setup
 *
 * This function sets up SPI device mode, speed etc. Can be called multiple
 * times for a single device. Returns %0 in case of success, negative error in
 * case of failure. When this function returns success, the device is
 * deselected.
 */
static int
sdp_spi_setup(struct spi_device *spi)
{
	struct sdp_spi *sdpspi = spi_master_get_devdata(spi->master);
	struct sdp_spi_chip *chip;

	/* is check supported mode. */
	if(unlikely(spi->mode & ~(spi->master->mode_bits) )) {
		dev_err(&sdpspi->pdev->dev, "Not supported  SPI Mode(%#x).\n", spi->mode);
		return -EINVAL;
	}

	/* is check supported speed (device max speed >= master min speed ). */
	if(unlikely(spi->max_speed_hz < sdpspi->min_rate)) {
		dev_err(&sdpspi->pdev->dev, "Not supported  SPI Speed(%dHz).\n", spi->max_speed_hz);
		return -EINVAL;
	}

	//add drain.lee if bpw=0 then pbw=8.
	if(unlikely(spi->bits_per_word == 0)) {
		spi->bits_per_word = 8;
		dev_info(&sdpspi->pdev->dev, "auto set : bits per word is 8bits\n");
	}

	if ( unlikely(spi->bits_per_word < 4 || spi->bits_per_word > 16) ) {
		dev_err(&sdpspi->pdev->dev, "invalid bits per word %d\n",
			spi->bits_per_word);
		return -EINVAL;
	}

	//if chip not exist than allocate chip.
	chip = spi_get_ctldata(spi);
	if (!chip) {
		dev_dbg(&sdpspi->pdev->dev, "initial setup for %s\n",
			spi->modalias);

		chip = kzalloc(sizeof(*chip), GFP_KERNEL);
		if (!chip)
			return -ENOMEM;

		chip->spi = spi;
		chip->ops = spi->controller_data;

		/* set default chip ops */
		if(!chip->ops) {
			struct sdp_spi_info *sdpspi_info = NULL;

			sdpspi_info = sdpspi->pdev->dev.platform_data;
			if(sdpspi_info && sdpspi_info->default_chip_ops) {
				dev_info(&sdpspi->pdev->dev, "%s %s is used default chip ops.\n",
					spi->modalias, dev_name(&spi->dev));
				chip->ops = sdpspi_info->default_chip_ops;
			}
		}

		//execute to slave device specific code.
		if (chip->ops && chip->ops->setup) {
			int ret = chip->ops->setup(spi);
			if (ret) {
				kfree(chip);
				return ret;
			}
		}

		spi_set_ctldata(spi, chip);
	}

	//chip setup.
	if (spi->max_speed_hz != chip->rate) {
		int err;

		err = sdp_spi_calc_divisors(sdpspi, chip, spi->max_speed_hz);
		if (err != 0) {
			dev_dbg(&sdpspi->pdev->dev, "error in calc_divisors.\n");
			spi_set_ctldata(spi, NULL);
			kfree(chip);
			return err;
		}
		chip->rate = spi->max_speed_hz;
	}

	chip->dss = bits_per_word_to_dss(spi->bits_per_word);

	if(unlikely(chip->frame_format > 2) ) {
		dev_dbg(&sdpspi->pdev->dev, "invalied frame format.\n");
		return -EINVAL;
	}

	sdp_spi_cs_control(spi, false);
	return 0;
}

/**
 * sdp_spi_transfer() - queue message to be transferred
 * @spi: target SPI device
 * @msg: message to be transferred
 *
 * This function is called by SPI device drivers when they are going to transfer
 * a new message. It simply puts the message in the queue and schedules
 * workqueue to perform the actual transfer later on.
 *
 * Returns %0 on success and negative error in case of failure.
 */
static int
sdp_spi_transfer(struct spi_device *spi, struct spi_message *msg)
{
	struct sdp_spi *sdpspi = spi_master_get_devdata(spi->master);
	struct spi_transfer *t;
	unsigned long flags;

	if (!msg || !msg->complete)
	{
		return -EINVAL;
	}

	/* first validate each transfer */
	list_for_each_entry(t, &msg->transfers, transfer_list) {
		if (t->bits_per_word) {
			if (t->bits_per_word < 4 || t->bits_per_word > 16)
			{
				dev_err(&sdpspi->pdev->dev, "Not Supported Bit per word.\n");
				return -EINVAL;
			}
		}

		if (t->speed_hz && t->speed_hz < sdpspi->min_rate) {
				dev_err(&sdpspi->pdev->dev, "Not Supported SPI Speed.\n");
				return -EINVAL;
		}

		/* 3-Wire Mode is one buffer must be null */
		if(spi->mode & SPI_3WIRE) {
			if( (t->rx_buf != NULL && t->tx_buf != NULL) || (t->rx_buf == NULL && t->tx_buf == NULL) ) {
				dev_err(&sdpspi->pdev->dev, "3-Wire Mode is one buffer must be null.\n");
				return -EINVAL;
			}
		}
	}

	/*
	 * Now that we own the message, let's initialize it so that it is
	 * suitable for us. We use @msg->status to signal whether there was
	 * error in transfer and @msg->state is used to hold pointer to the
	 * current transfer (or %NULL if no active current transfer).
	 */
	msg->state = NULL;
	msg->status = 0;
	msg->actual_length = 0;

	spin_lock_irqsave(&sdpspi->lock, flags);
	if (!sdpspi->running) {
		spin_unlock_irqrestore(&sdpspi->lock, flags);
		return -ESHUTDOWN;
	}
	list_add_tail(&msg->queue, &sdpspi->msg_queue);
	queue_work(sdpspi->wq, &sdpspi->msg_work);
	spin_unlock_irqrestore(&sdpspi->lock, flags);

	return 0;
}

/**
 * sdp_spi_cleanup() - cleans up master controller specific state
 * @spi: SPI device to cleanup
 *
 * This function releases master controller specific state for given @spi
 * device.
 */
static void
sdp_spi_cleanup(struct spi_device *spi)
{
	struct sdp_spi_chip *chip;

	chip = spi_get_ctldata(spi);
	if (chip) {
		if (chip->ops && chip->ops->cleanup)
			chip->ops->cleanup(spi);
		spi_set_ctldata(spi, NULL);
		kfree(chip);
	}
}


/**
 * sdp_spi_chip_setup() - configures hardware according to given @chip
 * @sdpspi: sdp SPI controller struct
 * @chip: chip specific settings
 *
 * This function sets up the actual hardware registers with settings given in
 * @chip. Note that no validation is done so make sure that callers validate
 * settings before calling this.
 */
static void
sdp_spi_chip_setup(const struct sdp_spi *sdpspi,
				  const struct sdp_spi_chip *chip)
{
	u16 cr0;
	u8 cr1;

	//serial clock rate
	cr0 = chip->div_scr << SSPCR0_SCR_SHIFT;
	//mode
	cr0 |= (chip->spi->mode & (SPI_CPHA|SPI_CPOL)) << SSPCR0_MODE_SHIFT;
	//frame format
	cr0 |= chip->frame_format << SSPCR0_FRF_SHIFT;
	//bits per word
	cr0 |= chip->dss;


	//add drain.lee by loopback mode setting
	cr1 = sdp_spi_read_u8(sdpspi, SSPCR1);
	if(chip->spi->mode & SPI_LOOP) {
		cr1 |= SSPCR1_LBM;
	} else {
		cr1 &= ~SSPCR1_LBM;
	}

	dev_dbg(&sdpspi->pdev->dev,
		"sdp_spi_chip_setup: FrameFormet=%0x, Mode=%#x, Loopback=%s, cpsr=%d, scr=%d, dss=%d\n",
		chip->frame_format,
		chip->spi->mode & (SPI_CPHA|SPI_CPOL),
		(cr1 & SSPCR1_LBM)?"ON":"OFF",
		chip->div_cpsr, chip->div_scr, chip->dss);

	dev_dbg(&sdpspi->pdev->dev, "sdp_spi_chip_setup: Regdump CR0=%#x, CR1=%#x, SSPCPSR=%#x\n", cr0, cr1, chip->div_cpsr);

	sdp_spi_write_u8(sdpspi, SSPCPSR, chip->div_cpsr);
	sdp_spi_write_u16(sdpspi, SSPCR0, cr0);
	sdp_spi_write_u8(sdpspi, SSPCR1, cr1);
}


////////////////////////// Process Messages
static inline int
bits_per_word(const struct sdp_spi *sdpspi)
{
	struct spi_message *msg = sdpspi->current_msg;
	struct spi_transfer *t = msg->state;

	return t->bits_per_word ? t->bits_per_word : msg->spi->bits_per_word;
}

static void
sdp_do_write(struct sdp_spi *sdpspi, struct spi_transfer *t)
{
	if (bits_per_word(sdpspi) > 8) {
		u16 tx_val = 0;

		if (t->tx_buf)
			tx_val = ((u16 *)t->tx_buf)[ sdpspi->tx/sizeof(tx_val) ];
		sdp_spi_write_u16(sdpspi, SSPDR, tx_val);
		sdpspi->tx += sizeof(tx_val);
		dev_dbg(&sdpspi->pdev->dev, "sdp_do_write Data 0x%x\n", tx_val);
	} else {
		u8 tx_val = 0;

		if (t->tx_buf)
			tx_val = ((u8 *)t->tx_buf)[ sdpspi->tx/sizeof(tx_val) ];
		sdp_spi_write_u8(sdpspi, SSPDR, tx_val);
		sdpspi->tx += sizeof(tx_val);
		dev_dbg(&sdpspi->pdev->dev, "sdp_do_write Data 0x%x\n", tx_val);
	}
}

static void
sdp_do_read(struct sdp_spi *sdpspi, struct spi_transfer *t)
{
	if (bits_per_word(sdpspi) > 8) {
		u16 rx_val;

		rx_val = sdp_spi_read_u16(sdpspi, SSPDR);
		if (t->rx_buf)
			((u16 *)t->rx_buf)[ sdpspi->rx/sizeof(rx_val) ] = rx_val;
		sdpspi->rx += sizeof(rx_val);
		dev_dbg(&sdpspi->pdev->dev, "sdp_do_read Data 0x%x\n", rx_val);
	} else {
		u8 rx_val;

		rx_val = sdp_spi_read_u8(sdpspi, SSPDR);
		if (t->rx_buf)
			((u8 *)t->rx_buf)[sdpspi->rx/sizeof(rx_val) ] = rx_val;
		sdpspi->rx += sizeof(rx_val);
		dev_dbg(&sdpspi->pdev->dev, "sdp_do_read Data 0x%x\n", rx_val);
	}
}

//for 3-wire interface
//MISO and MOSI wire shere MIMO(SISO)
// 3-wire read op is to wrire 0xFF.
//Operate the MIMO line with a pull-up resistor and an open drain driver.
//You have to write 0xFF to the SPI bus during read phases


/**
 * sdp_spi_read_write() - perform next RX/TX transfer
 * @sdpspi: sdp SPI controller struct
 *
 * This function transfers next bytes (or half-words) to/from RX/TX FIFOs. If
 * called several times, the whole transfer will be completed. Returns
 * %-EINPROGRESS when current transfer was not yet completed otherwise %0.
 *
 * When this function is finished, RX FIFO should be empty and TX FIFO should be
 * full.
 */
static int
sdp_spi_read_write(struct sdp_spi *sdpspi)
{
	struct spi_message *msg = sdpspi->current_msg;
	struct spi_transfer *t = msg->state;
	/*
	dev_dbg(&sdpspi->pdev->dev, "## fifo_level%d, sdpspi->rx%d, sdpspi->tx%d, t->len%d\n",
		sdpspi->fifo_level, sdpspi->rx, sdpspi->tx, t->len);
	*/
	/* read as long as RX FIFO has frames in it */
	while ((sdp_spi_read_u8(sdpspi, SSPSR) & SSPSR_RNE)) {
		sdp_do_read(sdpspi, t);
		sdpspi->fifo_level--;
	}

	/* write as long as TX FIFO has room */
	while (sdpspi->fifo_level < SPI_FIFO_SIZE && sdpspi->tx < t->len) {
		sdp_do_write(sdpspi, t);
		sdpspi->fifo_level++;
	}

	if (sdpspi->rx == t->len) {
		msg->actual_length += t->len;
		return 0;
	}

	return -EINPROGRESS;
}

/**
 * sdp_spi_process_transfer() - processes one SPI transfer
 * @sdpspi: sdp SPI controller struct
 * @msg: current message
 * @t: transfer to process
 *
 * This function processes one SPI transfer given in @t. Function waits until
 * transfer is complete (may sleep) and updates @msg->status based on whether
 * transfer was succesfully processed or not.
 */
static void
sdp_spi_process_transfer(struct sdp_spi *sdpspi,
					struct spi_message *msg,
					struct spi_transfer *t)
{
	struct sdp_spi_chip *chip = spi_get_ctldata(msg->spi);
	u8 use_bpw;

	msg->state = t;

	//drain.lee : add If bpw > 8, t->len will be an even number.(odd is error!!)
	use_bpw = (t->bits_per_word ? t->bits_per_word : chip->dss+1 );
	if(use_bpw > 8 && (t->len % 2 != 0))
	{
		dev_err(&sdpspi->pdev->dev,
				"Error: current using BPW(%d) > 8 but spi_transfer.len(%d) is odd number.\n",
				use_bpw, t->len);
		msg->status = -EINVAL;
		return;
	}

	/*
	 * Handle any transfer specific settings if needed. We use
	 * temporary chip settings here and restore original later when
	 * the transfer is finished.
	 */
	if (t->speed_hz || t->bits_per_word) {
		struct sdp_spi_chip tmp_chip = *chip;

		if (t->speed_hz) {
			int err;

			err = sdp_spi_calc_divisors(sdpspi, &tmp_chip,
						       t->speed_hz);
			if (err) {
				dev_err(&sdpspi->pdev->dev,
					"failed to adjust speed\n");
				msg->status = err;
				return;
			}
		}

		if (t->bits_per_word)
			tmp_chip.dss = bits_per_word_to_dss(t->bits_per_word);

		/*
		 * Set up temporary new hw settings for this transfer.
		 */
		sdp_spi_chip_setup(sdpspi, &tmp_chip);
	}

	sdpspi->rx = 0;
	sdpspi->tx = 0;

	/*
	 * Now everything is set up for the current transfer. We prime the TX
	 * FIFO, enable interrupts, and wait for the transfer to complete.
	 */
	if (sdp_spi_read_write(sdpspi)) {

		if(sdpspi->irq >= 0)
		{
			/* interrupt */
			sdp_spi_enable_interrupts(sdpspi);
			wait_for_completion(&sdpspi->wait);
		}
		else
		{
			/* polling */
			int ret = 0;
			sdp_spi_enable_interrupts(sdpspi);
			/* check */
			while(!try_wait_for_completion(&sdpspi->wait))
			{
				/* check interrupt in enabled interrupts */
				if((sdp_spi_read_u8(sdpspi, SSPCR1)&(SSPCR1_RORIE|SSPCR1_RIE|SSPCR1_TIE))
					& (sdp_spi_read_u8(sdpspi, SSPIIR)&(SSPIIR_RORIS|SSPIIR_RIS|SSPIIR_TIS)) )
				{
					ret = sdp_spi_interrupt(0, sdpspi);
					if(ret != IRQ_HANDLED)
					{
						dev_err(&sdpspi->pdev->dev,
							"interrupt is not handled.\n");
					}
				}
				schedule();
			}
		}

	}

	/*
	 * In case of error during transmit, we bail out from processing
	 * the message.
	 */
	if (msg->status)
		return;

	/*
	 * After this transfer is finished, perform any possible
	 * post-transfer actions requested by the protocol driver.
	 */
	if (t->delay_usecs) {
		set_current_state(TASK_UNINTERRUPTIBLE);
		schedule_timeout(usecs_to_jiffies(t->delay_usecs));
	}
	if (t->cs_change) {
		if (!list_is_last(&t->transfer_list, &msg->transfers)) {
			/*
			 * In case protocol driver is asking us to drop the
			 * chipselect briefly, we let the scheduler to handle
			 * any "delay" here.
			 */
			sdp_spi_cs_control(msg->spi, false);
			cond_resched();
			sdp_spi_cs_control(msg->spi, true);
		}
	}

	if (t->speed_hz || t->bits_per_word)
		sdp_spi_chip_setup(sdpspi, chip);
}

/*
 * sdp_spi_process_message() - process one SPI message
 * @sdpspi: sdp SPI controller struct
 * @msg: message to process
 *
 * This function processes a single SPI message. We go through all transfers in
 * the message and pass them to sdp_spi_process_transfer(). Chipselect is
 * asserted during the whole message (unless per transfer cs_change is set).
 *
 * @msg->status contains %0 in case of success or negative error code in case of
 * failure.
 */
static void
sdp_spi_process_message(struct sdp_spi *sdpspi,
				       struct spi_message *msg)
{
	unsigned long timeout;
	struct spi_transfer *t;
	int err;

	sdp_spi_chip_setup(sdpspi, spi_get_ctldata(msg->spi));

	/*
	 * Enable the SPI controller and its clock.
	 */
	err = sdp_spi_enable(sdpspi);
	if (err) {
		dev_err(&sdpspi->pdev->dev, "failed to enable SPI controller\n");
		msg->status = err;
		return;
	}

	/*
	 * Just to be sure: flush any data from RX FIFO.
	 */
	timeout = jiffies + msecs_to_jiffies(SPI_TIMEOUT);
	//if receive fifo not empty
	while (sdp_spi_read_u16(sdpspi, SSPSR) & SSPSR_RNE) {
		if (time_after(jiffies, timeout)) {
			dev_warn(&sdpspi->pdev->dev,
				 "timeout while flushing RX FIFO\n");
			msg->status = -ETIMEDOUT;
			return;
		}
		sdp_spi_read_u16(sdpspi, SSPDR);
	}

	/*
	 * We explicitly handle FIFO level. This way we don't have to check TX
	 * FIFO status using %SSPSR_TNF bit which may cause RX FIFO overruns.
	 */
	sdpspi->fifo_level = 0;

	/*
	 * Update SPI controller registers according to spi device and assert
	 * the chipselect.
	 */
	/* for test. move to above.
	sdp_spi_chip_setup(sdpspi, spi_get_ctldata(msg->spi));
	*/
	sdp_spi_cs_control(msg->spi, true);

	list_for_each_entry(t, &msg->transfers, transfer_list) {
		sdp_spi_process_transfer(sdpspi, msg, t);
		if (msg->status)
			break;
	}

	/*
	 * Now the whole message is transferred (or failed for some reason). We
	 * deselect the device and disable the SPI controller.
	 */
	sdp_spi_cs_control(msg->spi, false);
	sdp_spi_disable(sdpspi);
}

#define work_to_sdpspi(work) (container_of((work), struct sdp_spi, msg_work))

/**
 * sdp_spi_work() - sdp SPI workqueue worker function
 * @work: work struct
 *
 * Workqueue worker function. This function is called when there are new
 * SPI messages to be processed. Message is taken out from the queue and then
 * passed to sdp_spi_process_message().
 *
 * After message is transferred, protocol driver is notified by calling
 * @msg->complete(). In case of error, @msg->status is set to negative error
 * number, otherwise it contains zero (and @msg->actual_length is updated).
 */
static void
sdp_spi_work(struct work_struct *work)
{
	struct sdp_spi *sdpspi = work_to_sdpspi(work);
	struct spi_message *msg;

	spin_lock_irq(&sdpspi->lock);
	if (!sdpspi->running || sdpspi->current_msg ||
		list_empty(&sdpspi->msg_queue)) {
		spin_unlock_irq(&sdpspi->lock);
		return;
	}
	msg = list_first_entry(&sdpspi->msg_queue, struct spi_message, queue);
	list_del_init(&msg->queue);
	sdpspi->current_msg = msg;
	spin_unlock_irq(&sdpspi->lock);

	sdp_spi_process_message(sdpspi, msg);

	/*
	 * Update the current message and re-schedule ourselves if there are
	 * more messages in the queue.
	 */
	spin_lock_irq(&sdpspi->lock);
	sdpspi->current_msg = NULL;
	if (sdpspi->running && !list_empty(&sdpspi->msg_queue))
		queue_work(sdpspi->wq, &sdpspi->msg_work);
	spin_unlock_irq(&sdpspi->lock);

	/* notify the protocol driver that we are done with this message */
	msg->complete(msg->context);
}

static irqreturn_t
sdp_spi_interrupt(int irq, void *dev_id)
{
	struct sdp_spi *sdpspi = dev_id;
	u8 irq_status = sdp_spi_read_u8(sdpspi, SSPIIR);

	/*
	 * If we got ROR (receive overrun) interrupt we know that something is
	 * wrong. Just abort the message.
	 */
	if (unlikely(irq_status & SSPIIR_RORIS)) {
		/* clear the overrun interrupt */
		sdp_spi_write_u8(sdpspi, SSPICR, 0);
		dev_warn(&sdpspi->pdev->dev,
			 "receive overrun, aborting the message\n");
		sdpspi->current_msg->status = -EIO;
	} else {
		/*
		 * Interrupt is either RX (RIS) or TX (TIS). For both cases we
		 * simply execute next data transfer.
		 */
		if (sdp_spi_read_write(sdpspi)) {
			/*
			 * In normal case, there still is some processing left
			 * for current transfer. Let's wait for the next
			 * interrupt then.
			 */
			return IRQ_HANDLED;
		}
	}

	/*
	 * Current transfer is finished, either with error or with success. In
	 * any case we disable interrupts and notify the worker to handle
	 * any post-processing of the message.
	 */
	sdp_spi_disable_interrupts(sdpspi);
	complete(&sdpspi->wait);
	return IRQ_HANDLED;
}


///////////////////////////
static int
sdp_spi_probe(struct platform_device *pdev)
{
	struct spi_master *master;
	struct sdp_spi_info *spi_info;
	struct sdp_spi *sdpspi;
	struct resource *res;
	int error = 0;
	char wq_name[16];

	//spi_info is platform spectific data.
	spi_info = pdev->dev.platform_data;

	if(!spi_info->init) {
		dev_warn(&pdev->dev, "Board initialization code is not available!\n");
	} else {
		int init_ret = 0;
		/* call init code */
		init_ret = spi_info->init();
		if(init_ret < 0) {
			dev_err(&pdev->dev, "failed to board initialization!!(%d)\n", init_ret);
			return init_ret;
		}
	}

	//alloc and init master.
	master = spi_alloc_master(&pdev->dev, sizeof(*sdpspi));
	if (!master) {
		dev_err(&pdev->dev, "failed to allocate spi master\n");
		return -ENOMEM;
	}

	//callback func setting
	master->setup = sdp_spi_setup;
	master->transfer = sdp_spi_transfer;
	master->cleanup = sdp_spi_cleanup;
	master->bus_num = pdev->id;
	master->num_chipselect = spi_info->num_chipselect;

	master->mode_bits = SPI_CPOL | SPI_CPHA | SPI_LOOP;
	master->flags= 0;

	platform_set_drvdata(pdev, master);

	sdpspi = spi_master_get_devdata(master);

	if (spi_info->ext_clkrate) {
		sdpspi->clk = NULL;
		sdpspi->ext_clkrate = spi_info->ext_clkrate;
	} else {
		sdpspi->clk = clk_get(&pdev->dev, "PCLK");

		if (IS_ERR(sdpspi->clk)) {
			dev_err(&pdev->dev, "unable to get spi clock\n");
			error = PTR_ERR(sdpspi->clk);
			goto fail_release_master;
		}
	}

	spin_lock_init(&sdpspi->lock);
	init_completion(&sdpspi->wait);

	//clock setup
	/*
	 * Calculate maximum and minimum supported clock rates
	 * for the controller.
	 */
	sdpspi->max_rate = sdp_spi_clkrate(sdpspi) / 2;
	if((spi_info->max_clk_limit > 0) && (sdpspi->max_rate > spi_info->max_clk_limit)) {
		sdpspi->max_rate = spi_info->max_clk_limit;
	}
	sdpspi->min_rate = sdp_spi_clkrate(sdpspi) / (254 * 256);
	sdpspi->pdev = pdev;

	sdpspi->irq = platform_get_irq(pdev, 0);
	if (sdpspi->irq < 0) {
/* commented for support polling mode */
//		error = -EBUSY;
//		dev_err(&pdev->dev, "failed to get irq resources\n");
//		goto fail_put_clock;
		dev_info(&pdev->dev, "IRQ# not found. Start polling mode!\n");
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "unable to get iomem resource\n");
		error = -ENODEV;
		goto fail_put_clock;
	}

	res = request_mem_region(res->start, resource_size(res), pdev->name);
	if (!res) {
		dev_err(&pdev->dev, "unable to request iomem resources\n");
		error = -EBUSY;
		goto fail_put_clock;
	}

	sdpspi->regs_base = ioremap(res->start, resource_size(res));
	if (!sdpspi->regs_base) {
		dev_err(&pdev->dev, "failed to map resources\n");
		error = -ENODEV;
		goto fail_free_mem;
	}
	if(sdpspi->irq >= 0)
	{
		error = request_irq(sdpspi->irq, sdp_spi_interrupt, 0,
				    "sdp-spi", sdpspi);
		if (error) {
			dev_err(&pdev->dev, "failed to request irq\n");
			goto fail_unmap_regs;
		}
	}
	sprintf(wq_name, "sdp_spid%02d", master->bus_num);
	sdpspi->wq = create_singlethread_workqueue(wq_name);
	if (!sdpspi->wq) {
		dev_err(&pdev->dev, "unable to create workqueue\n");
		goto fail_free_irq;
	}
	INIT_WORK(&sdpspi->msg_work, sdp_spi_work);
	INIT_LIST_HEAD(&sdpspi->msg_queue);
	sdpspi->running = true;

	/* make sure that the hardware is disabled */
	//sdp_spi_write_u8(sdpspi, SSPCR1, 0);

	if(sdpspi->irq >= 0)
	{
		dev_info(&pdev->dev, "SDP SPI Controller. iomem 0x%08lx irq %d\n",
			 (unsigned long)sdpspi->regs_base, sdpspi->irq);
	}
	else
	{
		dev_info(&pdev->dev, "SDP SPI Controller. iomem 0x%08lx Polling mode\n",
			 (unsigned long)sdpspi->regs_base);
	}
	
	dev_info(&pdev->dev, "SPI Support Slave Clock [Min %ld - Max %ld]\n",
		sdpspi->min_rate, sdpspi->max_rate);
	dev_info(&pdev->dev, "SPI Support number of CS %d\n",
		master->num_chipselect);
	
	dev_dbg(&pdev->dev,"SPI Regs Dump\n");
	dev_dbg(&pdev->dev, "iomem base 0x%8p\n", sdpspi->regs_base);
	dev_dbg(&pdev->dev, "SSPCR0  0x%04x\n", sdp_spi_read_u16(sdpspi, SSPCR0));
	dev_dbg(&pdev->dev, "SSPCR1  0x%04x\n", sdp_spi_read_u16(sdpspi, SSPCR1));
	dev_dbg(&pdev->dev, "SSPDR   0x%04x\n", sdp_spi_read_u16(sdpspi, SSPDR));
	dev_dbg(&pdev->dev, "SSPSR   0x%04x\n", sdp_spi_read_u16(sdpspi, SSPSR));
	dev_dbg(&pdev->dev, "SSPCPSR 0x%04x\n", sdp_spi_read_u16(sdpspi, SSPCPSR));
	dev_dbg(&pdev->dev, "SSPIIR  0x%04x\n", sdp_spi_read_u16(sdpspi, SSPIIR));
	
	error = spi_register_master(master);
	if (error) {
		dev_err(&pdev->dev, "failed to register SPI master\n");
		goto fail_free_queue;
	}

	return 0;

fail_free_queue:
	destroy_workqueue(sdpspi->wq);
fail_free_irq:
	if(sdpspi->irq >= 0) free_irq(sdpspi->irq, sdpspi);
fail_unmap_regs:
	iounmap(sdpspi->regs_base);
fail_free_mem:
	release_mem_region(res->start, resource_size(res));
fail_put_clock:
	if (sdpspi->clk)
		clk_put(sdpspi->clk);
fail_release_master:
	spi_master_put(master);
	platform_set_drvdata(pdev, NULL);

	return error;
}

static int __devexit
sdp_spi_remove(struct platform_device *pdev)
{
	struct spi_master *master = platform_get_drvdata(pdev);
	struct sdp_spi *sdpspi = spi_master_get_devdata(master);
	struct resource *res;

	spin_lock_irq(&sdpspi->lock);
	sdpspi->running = false;
	spin_unlock_irq(&sdpspi->lock);

	destroy_workqueue(sdpspi->wq);

	/*
	 * Complete remaining messages with %-ESHUTDOWN status.
	 */
	spin_lock_irq(&sdpspi->lock);
	while (!list_empty(&sdpspi->msg_queue)) {
		struct spi_message *msg;

		msg = list_first_entry(&sdpspi->msg_queue,
				       struct spi_message, queue);
		list_del_init(&msg->queue);
		msg->status = -ESHUTDOWN;
		spin_unlock_irq(&sdpspi->lock);
		msg->complete(msg->context);
		spin_lock_irq(&sdpspi->lock);
	}
	spin_unlock_irq(&sdpspi->lock);

	if(sdpspi->irq >= 0) free_irq(sdpspi->irq, sdpspi);
	iounmap(sdpspi->regs_base);
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	release_mem_region(res->start, resource_size(res));
	if (sdpspi->clk)
		clk_put(sdpspi->clk);
	platform_set_drvdata(pdev, NULL);

	spi_unregister_master(master);
	return 0;
}


static struct platform_driver sdp_spi_driver /*__initdata*/ = {
	.driver		= {
		.name	= "sdp-spi",
		.owner	= THIS_MODULE,
	},
	.probe		= sdp_spi_probe,
	.remove		= __devexit_p(sdp_spi_remove),
};

static int __init
sdp_spi_init(void)
{
	return platform_driver_register(&sdp_spi_driver);
}
module_init(sdp_spi_init);

static void __exit
sdp_spi_exit(void)
{
	platform_driver_unregister(&sdp_spi_driver);
}
module_exit(sdp_spi_exit);

MODULE_DESCRIPTION("SDP SPI Master Controller.");
MODULE_AUTHOR("modified by drain.lee <drain.lee@samsung.com>");
MODULE_LICENSE("GPL");

#undef DEBUG

